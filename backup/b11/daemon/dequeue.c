#include <dem.h>

void dequeueSpecifyProc(proc* p){

  if(p == NULL || p->queued != QUEUED)return;

  nvmlReturn_t res;
  nvmlMemory_t mem;
  int devPos;

  for(devPos = 0 ; devPos < dem.ndev ; devPos ++){

    if(dem.flags[devPos].flag){
      printf("\tdevice[%d] cannot be selected...\n",devPos);
      continue;
    }

    dem.flags[devPos].flag = 1;

    res = nvmlDeviceGetMemoryInfo(dem.devs[devPos],&mem);

    if(res != NVML_SUCCESS){
      printf("Failed to get Memory Info in dequeue\n");
      exit(-1);
    }

    if(mem.free > p->data->mem + p->data->req + M64 + dem.flags[devPos].reserved){

      dem.flags[devPos].sd = p->sd;
      dem.flags[devPos].flag = 1;

      p->queued = ACTIVE;

      MSEND(p->sd,MIGRATE,0,0,devPos,0,0);

      dem.flags[devPos].reserved += p->data->mem + p->data->req;

      printf("MIGRATE(%d) to %d\n",p->sd,devPos);

      break;

    }else{

      dem.flags[devPos].flag = 0;

    }
  }
}

void dequeueSpecifyDevNO(int devNum){

  if(dem.flags[devNum].flag)return;

  dem.flags[devNum].flag = 1;

  proc* ptemp;
  nvmlReturn_t res;
  nvmlMemory_t mem;
  int find;

  find = 0;
  
  ptemp = dem.p0->next;
  
  while(ptemp->next != NULL){

    if(ptemp->queued == QUEUED){
      
      res = nvmlDeviceGetMemoryInfo(dem.devs[devNum],&mem);
      
      if(res != NVML_SUCCESS){
	printf("Failed to get Memory Info in dequeue\n");
	exit(-1);
      }
      
      if(mem.free > ptemp->data->mem + ptemp->data->req + M64 + dem.flags[devNum].reserved){
	
	dem.flags[devNum].sd = ptemp->sd;
	dem.flags[devNum].flag = 1;

	ptemp->queued = ACTIVE;

	MSEND(ptemp->sd,MIGRATE,0,0,devNum,0,0);

	dem.flags[devNum].reserved += ptemp->data->mem + ptemp->data->req;

	printf("MIGRATE(%d) to %d\n",ptemp->sd,devNum);

	find = 1;
	
	break;
      }
    }

    ptemp = ptemp->next;

  }

  if(!find){

    dem.flags[devNum].flag = 0;

  }
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
