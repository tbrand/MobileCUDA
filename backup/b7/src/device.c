#include <mocu.h>

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

void mocu_device_backup(){
  region *r;
    
  r = mocu.cp->d0->next;
  while(r->mode >= 0){
    r->backup = (char*)malloc(r->size);
    mocu.mocudaMemcpy(r->backup,r->base,r->size,cudaMemcpyDeviceToHost);
    mocu.mocudaFree(r->base);
    r = r->next;
  }
}


void mocu_device_restore(){
  cudaError_t res;
  region* r;
    
  r = mocu.cp->d0->next;
  while(r->mode >= 0){
    res = mocu.mocudaMemcpy(r->base,r->backup,r->size,cudaMemcpyHostToDevice);
    if(res!=cudaSuccess){
      printf("Failed to cudaMemcpy @ mocu_device_restore():res==%d\n",res);
      exit(1);
    }
    r = r->next;
  }
}
