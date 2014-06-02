#include <dem.h>

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

void renew_proc(proc_data *data,int sd){
  proc* p;

  p = get_proc(sd);

  if(p==NULL)return;

  p->data->pos = data->pos;
  p->data->mem = data->mem;
  p->data->REQUEST = data->REQUEST;
}

proc* create_proc(int sd){
  proc* p;

  p = (proc*)malloc(sizeof(proc));
  p->sd = sd;
  p->queued = ACTIVE;
  p->staying_pos = 0;
  p->data = (proc_data*)malloc(sizeof(proc_data));
  p->data->pid = 0;
  p->data->mem = 0;
  p->data->pos = 0;

  return p;
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

  printf("\t\t\tCHECKING OUTOF MOCU PROCS\n");

  int infoCount,i;
  nvmlProcessInfo_t* infos;
  nvmlReturn_t res;

  for(i = 0 ; i < dem.ndev ; i ++){
    
    infoCount = 10;
    infos = (nvmlProcessInfo_t*)malloc(sizeof(nvmlProcessInfo_t)*infoCount);

    res = nvmlDeviceGetComputeRunningProcesses(dem.devs[i],&infoCount,infos);

    if(res != NVML_SUCCESS){
      printf("Failed to get proc info\n");
      exit(-1);
    }

    printf("\t\t\tDEV[%d] : PROCS == %d\n",i,infoCount);

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
