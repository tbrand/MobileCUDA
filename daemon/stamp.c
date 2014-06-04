#include <dem.h>
#include <time.h>

#define DATA_SIZE 200

typedef struct _record{
  int sd;
  int status;
  int pos;
  pid_t pid;
  int h,m,s;
  struct _record *next,*prev;
} record;

typedef struct _data{
  int sd;
  char* name;
} data;

int idx = 0;
int init = 0;
record *r0,*r1;
data d[DATA_SIZE];

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

char* get_process_name(int sd){
  int i;
  for(i = 0 ; i < DATA_SIZE ; i ++){
    if(d[i].sd == sd){
      return d[i].name;
    }
  }

  return "Unknown Process Name";
}

void print_records(){

  record* r;

  r = r0->next;
  while(r->next != NULL){
    fprintf(fp,"%02d:%02d:%02d\tSD[%d]\tSTATUS[%d]\t%s\n",r->h,r->m,r->s,r->sd,r->status,get_process_name(r->sd));
    r = r->next;
  }

  fclose(fp);
}

void register_name(int sd){

  proc* p;
  data  _d;
  nvmlReturn_t res;

  p = get_proc(sd);

  _d.sd = sd;
  _d.name = (char*)malloc(80);

  res = nvmlSystemGetProcessName(p->data->pid,_d.name,80);

  if(res != NVML_SUCCESS){
    printf("Failed to register process name\n");
    exit(-1);
  }

  if(DATA_SIZE < idx - 1){
    printf("Over max procs... DATA_SIZE:%d idx:%d",DATA_SIZE,idx);
    printf("Failed to register process name(but daemon will not be exited.)\n");
    return;
  }

  d[idx++] = _d;

}
