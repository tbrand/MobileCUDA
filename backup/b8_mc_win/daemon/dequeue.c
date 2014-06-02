#include <dem.h>

//#define M64 0
#define M64 64 >> 20

void dequeueSpecifyProc(proc* p){

  nvmlReturn_t res;
  nvmlMemory_t mem;

  if(p == NULL || p->queued != 1)return;

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

    if(mem.free > p->data->mem + M64){

      dem.flags[devPos].sd = p->sd;
      dem.flags[devPos].flag = 1;

      p->queued = 0;
      p->data->REQUEST = MIGRATE;
      p->data->pos = devPos;
      send(p->sd,p->data,sizeof(proc_data),0);

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

    if(ptemp->queued == 1){
      
      res = nvmlDeviceGetMemoryInfo(dem.devs[devNum],&mem);
      
      if(res != NVML_SUCCESS){
	printf("Failed to get Memory Info in dequeue\n");
	exit(-1);
      }
      
      if(mem.free > ptemp->data->mem + M64){
	
	dem.flags[devNum].sd = ptemp->sd;
	dem.flags[devNum].flag = 1;
	ptemp->queued = 0;
	ptemp->data->REQUEST = MIGRATE;
	ptemp->data->pos = devNum;
	send(ptemp->sd,ptemp->data,sizeof(proc_data),0);

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

    if(ptemp->queued == 1){

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

    if(ptemp->queued == 1){
      res++;
    }
    
    ptemp = ptemp->next;

  }

  return res;
}
