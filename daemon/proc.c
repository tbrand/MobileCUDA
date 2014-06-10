#include <dem.h>

void print_procs(){

  printf("*****************PRINT PROCS*******************\n");

  proc* p;

  p = dem.p0->next;
  while(p->next != NULL){

    printf("PROC[%d]\n",p->sd);
    printf("\tSTATUS : %d\n",p->queued);
    printf("\tPOS    : %d\n",p->data->pos);
    printf("\tPID    : %d\n",p->data->pid);
    printf("\thas context : %d\n",p->created_context);
    
    p = p->next;
  }
}

void init_proc(){
  dem.p0 = (proc*)malloc(sizeof(proc));
  dem.p1 = (proc*)malloc(sizeof(proc));

  dem.p0->next = dem.p1;
  dem.p0->prev = NULL;
  dem.p1->next = NULL;
  dem.p1->prev = dem.p0;
}

proc* get_proc(int sd){
  proc* ptemp;
  
  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    if(ptemp->sd == sd)return ptemp;
    ptemp = ptemp->next;
  }

  return NULL;
}

proc* get_proc_active(int pos){
  proc* ptemp;
  
  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    if(ptemp->data->pos == pos && ptemp->queued == ACTIVE)return ptemp;
    ptemp = ptemp->next;
  }

  return NULL;
}

proc* get_proc_staying_pos(int pos){
  proc* ptemp;
  
  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    if(ptemp->data->pos == pos && ptemp->queued == STAYED)return ptemp;
    ptemp = ptemp->next;
  }

  return NULL;
}

void add_proc(proc *p){
  p->prev = dem.p1->prev;
  p->next = dem.p1;
  p->prev->next = p;
  p->next->prev = p;
}

void remove_proc(proc *p){
  p->next->prev = p->prev;
  p->prev->next = p->next;
  free(p);
}

proc* create_proc(int sd){
  proc* p;

  p = (proc*)malloc(sizeof(proc));
  p->sd = sd;
  p->queued = ACTIVE;
  p->staying_pos = 0;
  p->created_context = 0;
  p->data = (proc_data*)malloc(sizeof(proc_data));
  p->data->pid = 0;
  p->data->mem = 0;
  p->data->pos = 0;

  return p;
}

int num_of_active_procs(int pos){

  int res;
  proc* p;

  res = 0;

  p = dem.p0->next;
  while(p->next != NULL){
    if(p->queued == ACTIVE && p->data->pos == pos){
      res++;
    }
    p = p->next;
  }

  return res;
}

int has_staying_cannot_move_proc(int pos){

  proc* p;

  p = dem.p0->next;
  while(p->next != NULL){
    if(p->queued == STAYED && (p->data->flag&CANNOTMIG) && p->data->pos == pos){
      return 1;
    }
    p = p->next;
  }
  
  return 0;
}


int num_of_staying_exclusive_procs(){

  int res;
  proc* p;

  res = 0;

  p = dem.p0->next;
  while(p->next != NULL){
    if(p->queued == STAYED_QUEUED && (p->data->flag&EXCLUSIVE)){
      res++;
    }
    p = p->next;
  }

  return res;
}

int exclusive_proc_can_migrate_to(int pos){

  proc* p;

  p = dem.p0->next;
  while(p->next != NULL){
    if(p->data->pos == pos && p->queued != QUEUED && p->queued != EXC_READY)
      return 0;
    p = p->next;
  }

  return 1;
}

proc* ready_exclusive_proc(int pos){

  proc* ptemp;

  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    if(
       ptemp->queued == EXC_READY && 
       (ptemp->data->flag&EXCLUSIVE) &&
       ptemp->data->pos == pos
       ){
      return ptemp;
    }
    ptemp = ptemp->next;
  }
  
  return NULL;
  
}

proc* staying_exclusive_proc(){

  proc* ptemp;

  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    if(ptemp->queued == STAYED_QUEUED && (ptemp->data->flag&EXCLUSIVE)){
      return ptemp;
    }
    ptemp = ptemp->next;
  }
  
  return NULL;
}

proc* staying_proc(){
  
  proc* ptemp;

  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    if(ptemp->queued == STAYED_QUEUED){
      return ptemp;
    }
    ptemp = ptemp->next;
  }
  
  return NULL;
}

int has_proc(int pid){

  proc* ptemp;

  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    if(ptemp->data->pid == pid)return 1;
    ptemp = ptemp->next;
  }

  return 0;
}

int has_proc_sd(int sd){

  proc* ptemp;

  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    if(ptemp->sd == sd)return 1;
    ptemp = ptemp->next;
  }

  return 0;
}

void mocu_check(){

#if 1
  
  int infoCount,i;
  nvmlProcessInfo_t* infos;
  nvmlReturn_t res;

  printf("\t--------------------------------\n");

  for(i = 0 ; i < dem.ndev ; i ++){
    
    infoCount = 100;
    infos = (nvmlProcessInfo_t*)malloc(sizeof(nvmlProcessInfo_t)*infoCount);

    res = nvmlDeviceGetComputeRunningProcesses(dem.devs[i],&infoCount,infos);

    if(res != NVML_SUCCESS){
      printf("Failed to get proc info\n");
      exit(-1);
    }else{

#if 1
      printf("\tDEV[%d](Exclusive %d)(Stayed %d)\n",i,dem.flags[i].exclusive,dem.flags[i].stayed);
      printf("\t\t: PROCS             : %d\n",infoCount);
      printf("\t\t: RESERVED          : %lu[byte]\n",dem.flags[i].reserved);
      printf("\t\t: CONTEXT(Reserved) : %d\n",dem.flags[i].context);
#endif

      int j;

      for(j = 0 ; j < infoCount ; j ++){

	if(!has_proc(infos[j].pid) && infos[j].usedGpuMemory > 0){

	  int kres;

	  kres = kill(infos[j].pid,SIGINT);

	  printf("\t\t\t\tFIND OUT OF CONTROL PROC(kill:%d)\n",kres);
	}
      }
    }
  }

  printf("\t--------------------------------\n");

#endif

}
