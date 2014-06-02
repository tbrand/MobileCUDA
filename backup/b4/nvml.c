#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/stat.h>

#include <__cudaFatFormat.h>
#include <mocu.h>
#include <nvml.h>

#define MAX_PROCS 64

int mocu_num_of_proc_at_device(int pos){
  nvmlReturn_t res;
  nvmlProcessInfo_t* info;
  unsigned int num = MAX_PROCS;

  info = (nvmlProcessInfo_t*)malloc(sizeof(nvmlProcessInfo_t)*num);
      
  res = nvmlDeviceGetComputeRunningProcesses(mocu.nvml_dev[pos],&num,info);
   
  if(res != NVML_SUCCESS){
    printf("Failed to get num of compute-running-processes @device%d(%d)\n",pos,res);
    return 0;
  }
   
  return num;
}

nvmlProcessInfo_t* mocu_get_proc_info_at_device(int pos){
  nvmlReturn_t res;
  nvmlProcessInfo_t* info;
  unsigned int num = MAX_PROCS;

  info = (nvmlProcessInfo_t*)malloc(sizeof(nvmlProcessInfo_t)*num);
      
  res = nvmlDeviceGetComputeRunningProcesses(mocu.nvml_dev[pos],&num,info);
   
  if(res != NVML_SUCCESS){
    printf("Failed to get info of compute-running-processes @device%d(%d)\n",pos,res);
    return NULL;
  }
   
  return info;
}

int mocu_get_optimum_device(){

  int opt = 0;
  int min = mocu_num_of_proc_at_device(0);
  int i;

  printf("Pos0:%d\n",min);

  for(i = 1 ; i < mocu.ndev ; i ++){
    int num = mocu_num_of_proc_at_device(i);
    printf("Pos%d:%d\n",i,num);
    if(num < min){
      opt = i;
      min = num;
    }
  }

  printf("Opt pos : %d\n",opt);

  return opt;

}

unsigned long mocu_get_amount_of_memory_used(){
  region *r;
  unsigned long size = 0;
    
  r = mocu.cp->d0->next;

  while(r->next != NULL){
    size += r->size;
    r = r->next;
  }

  return size;
}

nvmlMemory_t mocu_get_mem_info_at_device(int pos){
  nvmlMemory_t mem_info;
  nvmlReturn_t res;

  res = nvmlDeviceGetMemoryInfo(mocu.nvml_dev[pos],&mem_info);

  if(res != NVML_SUCCESS){
    printf("Failed to get mem-info @device%d(%d)\n",pos,res);
  }
    
  return mem_info;
}
