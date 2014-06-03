#include <dem.h>

void dequeueSpecifyProc(proc* p){

  if(p == NULL || p->queued != QUEUED)return;

  nvmlReturn_t res;
  nvmlMemory_t mem;
  int devPos;

  for(devPos = 0 ; devPos < dem.ndev ; devPos ++){

    if(dem.flags[devPos].flag||dem.flags[devPos].stayed){
      continue;
    }

    dem.flags[devPos].flag = 1;

    res = nvmlDeviceGetMemoryInfo(dem.devs[devPos],&mem);

    if(res != NVML_SUCCESS){
      printf("Failed to get Memory Info in dequeue\n");
      exit(-1);
    }

    if(p->created_context){

      if(mem.free > p->data->mem + p->data->req + M64 + dem.flags[devPos].reserved){

	dem.flags[devPos].reserved += p->data->mem + p->data->req;
	
	MSEND(p->sd,MIGRATE,0,0,devPos,0,0);

	p->queued = ACTIVE;

	dem.flags[devPos].flag = 1;
	dem.flags[devPos].sd = p->sd;

	TIME_STAMP(p);

	return;
	
      }
      
    }else{

      if(mem.free > p->data->sym + p->data->req + M64 + dem.flags[devPos].reserved){

	dem.flags[devPos].reserved += p->data->sym + p->data->req;

	MSEND(p->sd,CONNECT,0,0,devPos,0,0);

	dem.flags[devPos].sd = -1;
	dem.flags[devPos].flag = 0;

	p->queued = ACTIVE;

	TIME_STAMP(p);

	return;
	
      }

    }

    dem.flags[devPos].flag = 0;
    
  }
}

void dequeueSpecifyDevNO(int devPos){

  if(dem.flags[devPos].flag||dem.flags[devPos].stayed){
    return;
  }

  dem.flags[devPos].flag = 1;

  proc* ptemp;
  nvmlReturn_t res;
  nvmlMemory_t mem;
  
  ptemp = dem.p0->next;
  
  while(ptemp->next != NULL){

    if(ptemp->queued == QUEUED){
      
      res = nvmlDeviceGetMemoryInfo(dem.devs[devPos],&mem);
      
      if(res != NVML_SUCCESS){
	printf("Failed to get Memory Info in dequeue\n");
	exit(-1);
      }

      if(ptemp->created_context){

	if(mem.free > ptemp->data->mem + ptemp->data->req + M64 + dem.flags[devPos].reserved){

	  ptemp->queued = ACTIVE;

	  dem.flags[devPos].reserved += ptemp->data->mem + ptemp->data->req;

	  MSEND(ptemp->sd,MIGRATE,0,0,devPos,0,0);

	  dem.flags[devPos].flag = 1;
	  dem.flags[devPos].sd = ptemp->sd;

	  printf("MIGRATE to %d\n",devPos);

	  TIME_STAMP(ptemp);

	  return;
	}
	
      }else{

	if(mem.free > ptemp->data->sym + ptemp->data->req + M64 + dem.flags[devPos].reserved){

	  ptemp->queued = ACTIVE;

	  dem.flags[devPos].reserved += ptemp->data->sym + ptemp->data->req;

	  MSEND(ptemp->sd,CONNECT,0,0,devPos,0,0);

	  dem.flags[devPos].sd = -1;
	  dem.flags[devPos].flag = 0;

	  TIME_STAMP(ptemp);

	  return;
	}
      }
    }

    ptemp = ptemp->next;

  }

  dem.flags[devPos].flag = 0;

}

void dequeue(){

  proc* ptemp;
  
  ptemp = dem.p0->next;
  
  while(ptemp->next != NULL){

    if(ptemp->queued == QUEUED){

      dequeueSpecifyProc(ptemp);

    }

    ptemp = ptemp->next;
  }
}

int queue_size(){

  proc* ptemp;
  int res = 0;

  ptemp = dem.p0->next;
  while(ptemp->next != NULL){

    if(ptemp->queued == QUEUED){
      res++;
    }
    
    ptemp = ptemp->next;

  }

  return res;
}

void exclusive_check(int pos){

  if(!dem.flags[pos].exclusive){
    printf("Wrong request ...\n");
    exit(-1);
    return;
  }

  proc* p;

  if(num_of_active_procs(pos) > 0){

    int res;

    printf("\tThere are active procs on device %d(%d)\n",pos,num_of_active_procs(pos));

    p = get_proc_active(pos);

    res = kill(p->data->pid,SIGUSR1);

    p->queued = SENT_REQUEST;

    TIME_STAMP(p);

    printf("\tSIGNAL RESULT : %d\n",res);

  }else{

    if(exclusive_proc_can_migrate_to(pos)){

      printf("\tAll active procs are suspended\n");
      printf("\tSo exclusive proc can entering device %d\n",pos);

      p = ready_exclusive_proc(pos);
    
      if(p == NULL){
	printf("\tFailed to find staying exclusive proc\n");
	printf("\tMay be the proc was interruped??\n");
	dem.flags[pos].exclusive = 0;

	return;
      }

      MSEND(p->sd,CONNECT,0,0,pos,0,0);

      dem.flags[pos].exclusive = 0;

      p->queued = ACTIVE;

      TIME_STAMP(p);

    }
  }
}
