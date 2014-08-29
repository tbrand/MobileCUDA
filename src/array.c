#include <mocu.h>

void mocu_array_backup(){
  
  mocu_array* arp;
  cudaError_t res;

  arp = mocu.cp->ar0->next;
  while(arp->next != NULL){

    arp->backup = (char*)malloc(arp->backup_size);

    res = mocu.mocudaMemcpyFromArray(arp->backup,arp->ar,0,0,arp->backup_size,cudaMemcpyHostToDevice);

    if(res != cudaSuccess){
      //
      exit(-1);
    }

    arp = arp->next;
  }

}

void mocu_array_restore(){
  
  mocu_array* arp;
  cudaError_t res;

  arp = mocu.cp->ar0->next;
  while(arp->next != NULL){

    if(arp->mode == 0){

      res = mocu.mocudaMallocArray(&arp->ar,&arp->desc,arp->width,arp->height,arp->flags);

    }else if(arp->mode == 1){

      res = mocu.mocudaMalloc3DArray(&arp->ar,&arp->desc,arp->extent,arp->flags);
      
    }else{
      printf("What ...?\n");
      exit(-1);
    }

    if(res != cudaSuccess){
      printf("What...?\n");
      exit(-1);
    }

    res = mocu.mocudaMemcpyToArray(arp->ar,0,0,arp->backup,arp->backup_size,cudaMemcpyHostToDevice);

    if(res != cudaSuccess){
      printf("What...?\n");
      exit(-1);
    }

    free(arp->backup);
    
    arp = arp->next;
  }

}
