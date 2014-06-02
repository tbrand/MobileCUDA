#include <mocu.h>

#define OPT 1

unsigned long mocu_get_amount_of_memory_used(){
  region *r;
  symbol *s;
  unsigned long size = 0;
  
  r = mocu.cp->d0->next;
  
  while(r->next != NULL){
    size += r->size;
    r = r->next;
  }

  s = mocu.cp->sm0->next;

  while(s->next != NULL){
    //    printf("Symbol size : %u\n",s->size);
    size += s->size;
    s = s->next;
  }

  return size;
}

void mocu_device_backup(){

#if OPT

  region *r;
    
  r = mocu.cp->d0->next;
  while(r->mode >= 0){
    if(r->flag){
      r->backup = (char*)malloc(r->size);
      mocu.mocudaMemcpy(r->backup,r->base,r->size,cudaMemcpyDeviceToHost);
      mocu.mocudaFree(r->base);
    }else{
      //      printf("\t\t\tTHIS REGION WILL NOT BE BACKUPED\n");
      mocu.mocudaFree(r->base);
    }
    r = r->next;
  }

#else

  region *r;
    
  r = mocu.cp->d0->next;
  while(r->mode >= 0){
    r->backup = (char*)malloc(r->size);
    mocu.mocudaMemcpy(r->backup,r->base,r->size,cudaMemcpyDeviceToHost);
    mocu.mocudaFree(r->base);
    r = r->next;
  }
  
#endif

}


void mocu_device_restore(){
  cudaError_t res;
  region* r;

#if OPT
    
  r = mocu.cp->d0->next;
  while(r->mode >= 0){
    if(r->flag){
      res = mocu.mocudaMemcpy(r->base,r->backup,r->size,cudaMemcpyHostToDevice);
      if(res!=cudaSuccess){
	free(r->base);
	printf("Failed to cudaMemcpy @ mocu_device_restore():res==%d\n",res);
	exit(1);
      }else{
	free(r->backup);
      }
    }else{
      //      printf("\t\t\tTHIS REGION WILL NOT BE RESTORED\n");
    }
    r = r->next;
  }

#else

  r = mocu.cp->d0->next;
  while(r->mode >= 0){
    res = mocu.mocudaMemcpy(r->base,r->backup,r->size,cudaMemcpyHostToDevice);
    if(res!=cudaSuccess){
      free(r->base);
      printf("Failed to cudaMemcpy @ mocu_device_restore():res==%d\n",res);
      exit(1);
    }else{
      free(r->backup);
    }
    r = r->next;
  }
  
#endif

}
