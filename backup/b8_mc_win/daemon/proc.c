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
  p->queued = 0;
  p->data = (proc_data*)malloc(sizeof(proc_data));
  p->data->pid = 0;
  p->data->mem = 0;
  p->data->pos = 0;

  return p;
}
