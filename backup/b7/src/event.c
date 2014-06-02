#include <mocu.h>

void mocu_event_update(){
  mocu_event* ep;
  float f;

  mocu.mocudaEventCreateWithFlags(&mocu.cp->e1->e,cudaEventDefault);
  mocu.mocudaEventRecord(mocu.cp->e1->e,0);
  mocu.mocudaEventSynchronize(mocu.cp->e1->e);

  ep = mocu.cp->e0->next;

  while(ep->mode >= 0){
    switch(ep->mode){
    case 0:
      break;
    case 1:
      mocu.mocudaEventElapsedTime(&ep->charge, ep->e, mocu.cp->e1->e);
      ep->mode = 2;
      break;
    case 2:
      mocu.mocudaEventElapsedTime(&f,mocu.cp->e0->e,mocu.cp->e1->e);
      ep->charge += f;
      break;
    }
    ep = ep->next;
  }
}

void mocu_event_restore(){
  mocu_event* ep;
  cudaError_t res1,res2;

  res1 = mocu.mocudaEventCreateWithFlags(&mocu.cp->e0->e,cudaEventDefault);
  res2 = mocu.mocudaEventCreateWithFlags(&mocu.cp->e1->e,cudaEventDefault);
  if(res1 != cudaSuccess || res2 != cudaSuccess){
    printf("Failed to create event @ mocu_event_restore():res1==%d,res2==%d\n",res1,res2);
    exit(1);
  }

  ep = mocu.cp->e0->next;
  while(ep->mode >= 0){
    res1 = mocu.mocudaEventCreateWithFlags(&ep->e,cudaEventDefault);
    if(res1 != cudaSuccess){
      printf("Failed to create event @ mocu_event_restore():res1==%d\n",res1);
      exit(1);
    }
    ep = ep->next;
  }

  mocu.mocudaEventRecord(mocu.cp->e0->e,NULL);
}
