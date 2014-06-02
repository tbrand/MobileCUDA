#include <dem.h>

#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int sockfd;
proc *p0,*p1;

void add_item(proc* item,proc_data* data){

  item->prev = p1->prev;
  item->next = p1;
  item->prev->next = item;
  item->next->prev = item;
  item->data = data;
  
}

void remove_item(proc* p){
  p->next->prev = p->prev;
  p->prev->next = p->next;
  //  free(p);
}

proc* get_item(int pid){
  proc* ptemp;

  ptemp = p0->next;
  while(ptemp->next != NULL){
    if(ptemp->data->pid == pid)return ptemp;
    ptemp = ptemp->next;
  }
}

proc* get_item_sd(int sd){
  proc* ptemp;

  ptemp = p0->next;
  while(ptemp->next != NULL){
    if(ptemp->sd == sd)return ptemp;
    ptemp = ptemp->next;
  }
}

void print_items(){

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

  printf("***********************************\n\n");
  
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

  }else{
    printf("No procs...\n");
  }

  print_items();

}

void console_loop(){

  int result;
  proc*      p;
  proc_data* data;

  while(1){
    
    data = (proc_data*)malloc(sizeof(proc_data));
    
    result = recv(sockfd,data,sizeof(proc_data),0);
    
    if(result == -1){
      printf("Failed to recv msg\n");
      exit(-1);
    }

    if(data->REQUEST == CONS_ADD){

      printf("\tCONS ADD\n");

      p = (proc*)malloc(sizeof(proc));

      result = recv(sockfd,p,sizeof(proc),0);
      
      if(result == -1){
	printf("Failed to recv msg\n");
	exit(-1);
      }

      proc* ptemp;

      ptemp = (proc*)malloc(sizeof(proc));
      ptemp->sd = p->sd;
      ptemp->queued = p->queued;
      ptemp->staying_pos = p->staying_pos;

      add_item(ptemp,data);
      
    }else if(data->REQUEST == CONS_REMOVE){

      printf("\tCONS REMOVE\n");

      proc* ptemp;

      ptemp = get_item(data->pid);

      remove_item(ptemp);

    }else if(data->REQUEST == CONS_RENEW){

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

    }else{
      printf("Daemon fin...\n");
      break;
    }

    //    print_items();
  }
}

int main(){

  p0 = (proc*)malloc(sizeof(proc));
  p1 = (proc*)malloc(sizeof(proc));
    
  p0->next = p1;
  p0->prev = NULL;
  p1->next = NULL;
  p1->prev = p0;

  int recv = fork();

  if(recv < 0){
    printf("Failed to fork\n");
    exit(-1);
  }else if(recv == 0){
    
    cons_connect();
    
    console_loop();
    
  }else{
    printf("I'm parent!!\n");

    while(1){

      print_items();

      sleep(5);
    }

  }

  return 0;
}
