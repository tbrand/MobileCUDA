#include <dem.h>

void init_cons(){
  dem.c0 = (cons*)malloc(sizeof(cons));
  dem.c1 = (cons*)malloc(sizeof(cons));

  dem.c0->next = dem.c1;
  dem.c0->prev = NULL;
  dem.c1->next = NULL;
  dem.c1->prev = dem.c0;
}

cons* get_cons(int sd){
  cons* ctemp;
  
  ctemp = dem.c0->next;
  while(ctemp->next != NULL){
    if(ctemp->sd == sd)return ctemp;
    ctemp = ctemp->next;
  }

  return NULL;
}

void add_cons(cons *c){
  c->prev = dem.c1->prev;
  c->next = dem.c1;
  c->prev->next = c;
  c->next->prev = c;
}

void remove_cons(cons *c){
  c->next->prev = c->prev;
  c->prev->next = c->next;
  free(c);
}

cons* create_cons(int sd){
  cons* c;

  c = (cons*)malloc(sizeof(cons));
  c->sd = sd;

  return c;
}

void cons_renew_data(proc_data* data){

  cons* c;

  c = dem.c0->next;
  while(c->next != NULL){

    send(c->sd,data,sizeof(proc_data),0);
    
    c = c->next;
  }
}

void print_cons(){

  cons* ctemp;
  int counter = 0;

  ctemp = dem.c0->next;

  while(ctemp->next != NULL){
    
    printf("\tCONS[%d] : %d\n",++counter,ctemp->sd);

    ctemp = ctemp->next;

  }
}

void cons_add(proc* p){

  cons* ctemp;

  ctemp = dem.c0->next;
  while(ctemp->next != NULL){

    MSEND(ctemp->sd,CONS_ADD,p->data->pid,p->data->flag,p->data->pos,p->data->mem,0);

    int result = send(ctemp->sd,p,sizeof(proc),0);

    if(result == -1){
      printf("Failed to send data to cons\n");
      exit(-1);
    }

    ctemp = ctemp->next;
  }
}

void cons_remove(proc* p){
  
  cons* ctemp;

  ctemp = dem.c0->next;
  while(ctemp->next != NULL){

    MSEND(ctemp->sd,CONS_REMOVE,p->data->pid,p->data->flag,p->data->pos,p->data->mem,0);

    ctemp = ctemp->next;
  }
}

void cons_renew(proc* p){
  
  cons* ctemp;

  ctemp = dem.c0->next;
  while(ctemp->next != NULL){

    MSEND(ctemp->sd,CONS_RENEW,p->data->pid,p->data->flag,p->data->pos,p->data->mem,p->data->req);

    int result = send(ctemp->sd,p,sizeof(proc),0);

    if(result == -1){
      printf("Failed to send data to cons\n");
      exit(-1);
    }

    ctemp = ctemp->next;
  }
}
