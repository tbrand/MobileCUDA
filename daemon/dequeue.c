#include <dem.h>

#define OPT 1

void dequeueSpecifyProc(proc* p){

  if(p == NULL || p->queued != QUEUED)return;

  nvmlReturn_t res;
  nvmlMemory_t mem;
  int  devPos;

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

      if(mem.free > p->data->mem + p->data->req + M64 + dem.flags[devPos].reserved + MARGIN){

	dem.flags[devPos].reserved += p->data->mem + p->data->req;
	
	MSEND(p->sd,MIGRATE,0,0,devPos,0,0);

	p->data->pos = devPos;
	p->queued = ACTIVE;

	dem.flags[devPos].flag = 1;
	dem.flags[devPos].sd = p->sd;

	TIME_STAMP(p);

	return;
	
      }
      
    }else{

      if(mem.free > p->data->sym + p->data->req + M64 + dem.flags[devPos].reserved + MARGIN){

	dem.flags[devPos].reserved += p->data->sym + p->data->req + M64;

	MSEND(p->sd,CONNECT,0,0,devPos,0,0);

	dem.flags[devPos].sd = -1;
	dem.flags[devPos].flag = 0;
	dem.flags[devPos].context ++;

	p->data->pos = devPos;
	p->queued = ACTIVE;

	TIME_STAMP(p);

	return;
	
      }

    }

    dem.flags[devPos].flag = 0;
    
  }
}

void dequeueSpecifyDevNO(int devPos){

  if(dem.flags[devPos].flag||dem.flags[devPos].stayed)
    return;

  dem.flags[devPos].flag = 1;

  proc* p;
  nvmlReturn_t res;
  nvmlMemory_t mem;
  
  p = dem.p0->next;
  
  while(p->next != NULL){

    if(p->queued == QUEUED){
      
      res = nvmlDeviceGetMemoryInfo(dem.devs[devPos],&mem);
      
      if(res != NVML_SUCCESS){
	printf("Failed to get Memory Info in dequeue\n");
	exit(-1);
      }

      if(p->created_context){

	if(mem.free > p->data->mem + p->data->req + M64 + dem.flags[devPos].reserved + MARGIN){

	  p->data->pos = devPos;
	  p->queued = ACTIVE;

	  dem.flags[devPos].reserved += p->data->mem + p->data->req;

	  MSEND(p->sd,MIGRATE,0,0,devPos,0,0);

	  dem.flags[devPos].flag = 1;
	  dem.flags[devPos].sd = p->sd;

	  printf("MIGRATE to %d\n",devPos);

	  TIME_STAMP(p);

	  return;
	}
	
      }else{

	if(mem.free > p->data->sym + p->data->req + M64 + dem.flags[devPos].reserved + MARGIN){

	  p->data->pos = devPos;
	  p->queued = ACTIVE;
	  
	  dem.flags[devPos].reserved += p->data->sym + p->data->req + M64;

	  MSEND(p->sd,CONNECT,0,0,devPos,0,0);

	  dem.flags[devPos].sd = -1;
	  dem.flags[devPos].flag = 0;
	  dem.flags[devPos].context ++;

	  TIME_STAMP(p);

	  return;
	}
      }
    }

    p = p->next;

  }

  dem.flags[devPos].flag = 0;

}

void dequeue(){

  proc* p;
  
  p = dem.p0->next;
  
  while(p->next != NULL){

    if(p->queued == QUEUED){

      dequeueSpecifyProc(p);

    }

    p = p->next;
  }
}

int queue_size(){

  proc* p;
  int res = 0;

  p = dem.p0->next;
  while(p->next != NULL){

    if(p->queued == QUEUED){
      res++;
    }
    
    p = p->next;

  }

  return res;
}

typedef struct{
  int devpos;
  int numofproc;
} dev_data;

int* opt_devs(){

  dev_data* data;
  int* result;
  int i,j;

  result = (int*)malloc(sizeof(int)*dem.ndev);

  data = (dev_data*)malloc(sizeof(dev_data)*dem.ndev);

  for(i = 0 ; i < dem.ndev ; i ++){

    data[i].devpos = i;
    data[i].numofproc = num_of_active_procs(i);

  }

  for(i = 0 ; i < dem.ndev-1 ; i ++){
    for(j = i+1 ; j < dem.ndev ; j ++){
      if(data[i].numofproc > data[j].numofproc){
	dev_data temp = data[i];
	data[i] = data[j];
	data[j] = temp;
      }
    }
  }

  for(i = 0 ; i < dem.ndev ; i ++){
    result[i] = data[i].devpos;
  }

  return result;
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
	dem.flags[pos].reserved -= M64;
	dem.flags[pos].stayed = 0;
	return;
      }

      MSEND(p->sd,CONNECT,0,0,pos,0,0);

      dem.flags[pos].exclusive = 0;
      dem.flags[pos].context ++;

      p->queued = ACTIVE;

      TIME_STAMP(p);

    }
  }
}
