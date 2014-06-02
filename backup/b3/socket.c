#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/stat.h>

#include <__cudaFatFormat.h>
#include <mocu.h>
#include <nvml.h>
#include <vector_types.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int sockfd;

void mocu_connect(){
  
  int result;
  struct sockaddr_un address;

  sockfd = socket(AF_UNIX,SOCK_STREAM,0);

  address.sun_family = AF_UNIX;

  strcpy(address.sun_path,"/home/taichirou/migrate_runtime/daemon/mocu_server");

  result = connect(sockfd,(struct sockaddr*)&address,sizeof(address));

  if(result == -1){
    perror("oops: client\n");
  }

  proc_data* proc = (proc_data*)malloc(sizeof(proc_data));
  
  proc->pid = getpid();
  proc->pos = mocu_pos;
  //  proc->mem = mocu_get_amount_of_memory_used();
  proc->REQUEST = CONNECT;

  send(sockfd,proc,sizeof(proc_data),0);
  recv(sockfd,proc,sizeof(proc_data),0);

  request_from_daemon(proc);

  printf("Success to connect mocu server!!\n");
}

void send_renew(){

  proc_data* data;

  data = (proc_data*)malloc(sizeof(proc_data));
  data->REQUEST = RENEW;
  data->pid = getpid();
  data->pos = mocu_pos;
  data->mem = mocu_get_amount_of_memory_used();
  
  send(sockfd,data,sizeof(proc_data),0);
}

void try_to_allocate(int size){
  proc_data* data;

  data = (proc_data*)malloc(sizeof(proc_data));
  data->REQUEST = CUDAMALLOC;
  data->pid = getpid();
  data->pos = mocu_pos;
  data->mem = mocu_get_amount_of_memory_used()+size;

  send(sockfd,data,sizeof(proc_data),0);
  printf("Come here...\n");
  recv(sockfd,data,sizeof(proc_data),0);
  request_from_daemon(data);
}

void failed_to_get(int size){

  printf("Failed to GET ......................................\n");

  proc_data* data;

  data = (proc_data*)malloc(sizeof(proc_data));
  data->REQUEST = FAILEDTOGET;
  data->pid = getpid();
  data->pos = mocu_pos;
  data->mem = mocu_get_amount_of_memory_used()+size;
  
  send(sockfd,data,sizeof(proc_data),0);

  mocu_backup();
  mocu.mocudaDeviceReset();

  suspended();
}

void suspended(){

  proc_data* data;

  data = (proc_data*)malloc(sizeof(proc_data));

  recv(sockfd,data,sizeof(proc_data),0);

  request_from_daemon(data);
}

void request_from_daemon(proc_data* data){

  printf("Request From Daemon ...(%d)\n",data->REQUEST);

  switch(data->REQUEST){
  case STARTPOS:
    printf("Receive STARTPOS(%d) request!!!!!!!!!!!!!!!\n",data->pos);
    mocu.mocudaSetDevice(data->pos);
    break;
  case CHANGEPOS:
    mocu_migrate(data->pos);
    break;
  case MIGRATE:
    printf("Receive MIGRATE request!!!!!!!!!!!!!!!\n");
    mocu_backup();
    mocu.mocudaDeviceReset();
    mocu_migrate(data->pos);
    break;
  case SUSPEND:
    printf("Receive SUSPEND request !!!!!!!!!!!!!!!!!!!!!!\n");
    mocu_backup();
    mocu.mocudaDeviceReset();
    suspended();
    break;
  case CONNECT:
    printf("Receive CONNECT to do nothing..............\n");
    break;
  }
}
