#include <dem.h>
#include <time.h>

typedef struct _record{
  int sd;
  int status;
  int pos;
  pid_t pid;
  int h,m,s;
  struct _record *next,*prev;
} record;

int init = 0;
record *r0,*r1;

FILE* fp = NULL;

void init_stamp(){
  
  r0 = (record*)malloc(sizeof(record));
  r1 = (record*)malloc(sizeof(record));

  r0->next = r1;
  r0->prev = NULL;
  r1->next = NULL;
  r1->prev = r0;

  char log[32];

  sprintf(log,"logs/log.%u",getpid());

  fp = fopen(log,"w+");

  init = 1;

}

void TIME_STAMP(proc* p){

  if(!init)init_stamp();
  
  record* r;
  time_t tt;
  struct tm* ts;

  r = (record*)malloc(sizeof(record));
  r->sd = p->sd;
  r->status = p->queued;
  r->pid = p->data->pid;
  r->pos = p->data->pos;
  r->prev = r1->prev;
  r->next = r1;
  r->prev->next = r;
  r->next->prev = r;

  time(&tt);
  ts = localtime(&tt);

  r->h = ts->tm_hour;
  r->m = ts->tm_min;
  r->s = ts->tm_sec;
}

void print_records(){

  record* r;

  r = r0->next;
  while(r->next != NULL){
    fprintf(fp,"%02d:%02d:%02d\tSD[%d]\tSTATUS[%d]\n",r->h,r->m,r->s,r->sd,r->status);
    r = r->next;
  }

  fclose(fp);
}
