#include <dem.h>

#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define UPPER_LIMIT 200
#define KEY 102028

int sockfd;
//proc *p0,*p1;

proc* procs;

void add_item(proc* item,proc_data* data){

  /*
    item->prev = p1->prev;
    item->next = p1;
    item->prev->next = item;
    item->next->prev = item;
    item->data = data;
  */

  int i;
  for(i = 0 ; i < UPPER_LIMIT ; i ++){
    if(procs[i].sd == -1){
      procs[i].sd = item->sd;
      procs[i].queued = item->queued;
      procs[i].staying_pos = item->staying_pos;
      procs[i].data = (proc_data*)malloc(sizeof(proc_data));
      procs[i].data->pid = data->pid;
      procs[i].data->canmig = data->canmig;
      procs[i].data->pos = data->pos;
      procs[i].data->mem = data->mem;

      printf("TEST\n");
      printf("data->pid : %d\n",procs[i].data->pid);
      break;
    } 
  }
}

void remove_item(proc* p){

  /*
    p->next->prev = p->prev;
    p->prev->next = p->next;
  */

  int i;
  for(i = 0 ; i < UPPER_LIMIT ; i ++){
    if(procs[i].sd == p->sd){
      procs[i].sd = -1;
      break;
    }
  }
}

proc* get_item(int pid){

  /*
    proc* ptemp;

    ptemp = p0->next;
    while(ptemp->next != NULL){
    if(ptemp->data->pid == pid)return ptemp;
    ptemp = ptemp->next;
    }
  */

  int i;
  for(i = 0 ; i < UPPER_LIMIT ; i ++){
    if(procs[i].data->pid == pid)return &procs[i];
  }
}

proc* get_item_sd(int sd){

  /*
    proc* ptemp;

    ptemp = p0->next;
    while(ptemp->next != NULL){
    if(ptemp->sd == sd)return ptemp;
    ptemp = ptemp->next;
    }
  */

  int i;
  for(i = 0 ; i < UPPER_LIMIT ; i ++){
    if(procs[i].sd == sd)return &procs[i];
  }
}

int get_item_pos(int pid){
  
}

void print_items(){

  /*
    int counter = 0;
    proc* p = p0->next;

    while(p->next != NULL){

    printf("proc[%d]\n",counter++);
    printf("\tsd     : %d\n",p->sd);
    printf("\tqueued : %d\n",p->queued);
    printf("\t\tpid : %d\n",p->data->pid);
    printf("\t\tpos : %d\n",p->data->pos);
    printf("\t\tmem : %d[MB]\n",p->data->mem >> 20);

    p = p->next;
    }

    printf("\n***********************************\n\n");
  */

  int i;
  for(i = 0 ; i < UPPER_LIMIT ; i ++){

    if(procs[i].sd != -1){
      printf("proc[%d]\n",procs[i].sd);
      printf("\tqueued : %d\n",procs[i].queued);
      printf("\t\tpid : %d\n",procs[i].data->pid);
      printf("\t\tpos : %d\n",procs[i].data->pos);
      printf("\t\tmem : %d[MB]\n",procs[i].data->mem >> 20);
    }
  }
}

void cons_connect(){
  
  int result;
  int pnum;
  struct sockaddr_un address;

  sockfd = socket(AF_UNIX,SOCK_STREAM,0);

  address.sun_family = AF_UNIX;

  strcpy(address.sun_path,"/home/taichirou/migrate_runtime/daemon/mocu_server");

  result = connect(sockfd,(struct sockaddr*)&address,sizeof(address));

  if(result == -1){
    perror("oops: client(Failed to connect to daemon)\n");
    exit(-1);
  }

  proc_data* _proc = (proc_data*)malloc(sizeof(proc_data));
  
  _proc->pid = getpid();
  _proc->pos = 0;
  _proc->mem = 0;
  _proc->canmig = 0;
  _proc->REQUEST = CONSOLE;

  result = send(sockfd,_proc,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

  pnum = 0;

  result = recv(sockfd,&pnum,sizeof(int),0);

  if(result == -1){
    printf("Failed to recv msg\n");
    exit(-1);
  }

  if(pnum > 0){

    proc* procs;
    proc_data* data;

    procs = (proc*)malloc(sizeof(proc)*pnum);

    data = (proc_data*)malloc(sizeof(proc_data)*pnum);

    result = recv(sockfd,procs,sizeof(proc)*pnum,0);
  
    if(result == -1){
      printf("Failed to recv msg\n");
      exit(-1);
    }

    result = recv(sockfd,data,sizeof(proc_data)*pnum,0);
  
    if(result == -1){
      printf("Failed to recv msg\n");
      exit(-1);
    }

    int i;
    for(i = 0 ; i < pnum ; i ++){
      add_item(&procs[i],&data[i]);
    }

  }
}

void console_loop(){

  while(1){

    int result;
    proc*      p;
    proc_data* data;
    
    data = (proc_data*)malloc(sizeof(proc_data));
    
    result = recv(sockfd,data,sizeof(proc_data),0);
    
    if(result == -1){
      printf("Failed to recv msg\n");
      exit(-1);
    }

    printf("REQUEST : : : : : : : %d\n",data->REQUEST);

    if(data->REQUEST == CONS_ADD){

      //      printf("\tCONS ADD\n");

      p = (proc*)malloc(sizeof(proc));

      result = recv(sockfd,p,sizeof(proc),0);
      
      if(result == -1){
	printf("Failed to recv msg\n");
	exit(-1);
      }

      add_item(p,data);

      /*

      proc* ptemp;

      ptemp = (proc*)malloc(sizeof(proc));
      ptemp->sd = p->sd;
      ptemp->queued = p->queued;
      ptemp->staying_pos = p->staying_pos;

      add_item(ptemp,data);

      */
      
    }else if(data->REQUEST == CONS_REMOVE){

      //      printf("\tCONS REMOVE\n");

      /*
      proc* ptemp;

      ptemp = get_item(data->pid);

      remove_item(ptemp);
      */

    }else if(data->REQUEST == CONS_RENEW){

      /*

      printf("\tCONS RENEW\n");

      p = (proc*)malloc(sizeof(proc));
      
      result = recv(sockfd,p,sizeof(proc),0);
      
      if(result == -1){
	printf("Failed to recv msg\n");
	exit(-1);
      }

      proc* ptemp;

      ptemp = get_item(data->pid);
      ptemp->sd = p->sd;
      ptemp->queued = p->queued;
      ptemp->staying_pos = p->staying_pos;
      ptemp->data = data;

      */

    }else{
      printf("Unkown request[%d](daemon fin?)\n",data->REQUEST);
      break;
    }

    //    print_items();
  }
}

int main(){

  /*
    p0 = (proc*)malloc(sizeof(proc));
    p1 = (proc*)malloc(sizeof(proc));
    
    p0->next = p1;
    p0->prev = NULL;
    p1->next = NULL;
    p1->prev = p0;
  */

  int id;

  id = shmget(IPC_PRIVATE,sizeof(proc)*UPPER_LIMIT,0600);

  int child = fork();

  if(child == 0){

    //    id = shmget(KEY,sizeof(proc)*UPPER_LIMIT,IPC_CREAT|SHM_W|SHM_R);
    //    id = shmget(IPC_PRIVATE,sizeof(proc)*UPPER_LIMIT,0600);

    procs = (proc*)shmat(id,NULL,0);

    int i;
    for(i = 0 ; i < UPPER_LIMIT ; i ++){
      procs[i].sd = -1;
    }

    cons_connect();
    
    console_loop();
    
  }else{

    procs = (proc*)shmat(id,NULL,0);

    while(1){

      sleep(5);

      print_items();

    }
    
  }

  return 0;
}
