#include <mocu.h>

void mocu_stream_restore(){
  mocu_stream* sp;
  cudaError_t res;

  sp = mocu.cp->s0->next;
  while(sp->mode >= 0){
    res = mocu.mocudaStreamCreate(&sp->s);
    if(res != cudaSuccess){
      printf("Failed to create stream @ mocu_stream_restore():res==%d\n",res);
      exit(1);
    }
    sp = sp->next;
  }
}
