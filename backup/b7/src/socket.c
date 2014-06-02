#include <mocu.h>

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
    perror("oops: client(Failed to connect to daemon)\n");
    exit(-1);
  }

  proc_data* proc = (proc_data*)malloc(sizeof(proc_data));
  
  proc->pid = getpid();
  proc->pos = mocu_pos;
  //  proc->mem = mocu_get_amount_of_memory_used();
  proc->REQUEST = CONNECT;

  result = send(sockfd,proc,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

  result = recv(sockfd,proc,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to recv msg\n");
    exit(-1);
  }

  request_from_daemon(proc);

  printf("Success to connect mocu server!!\n");
}

void send_renew(){
  int result;
  proc_data* data;

  data = (proc_data*)malloc(sizeof(proc_data));
  data->REQUEST = RENEW;
  data->pid = getpid();
  data->pos = mocu_pos;
  data->mem = mocu_get_amount_of_memory_used();
  
  result = send(sockfd,data,sizeof(proc_data),0);
  
  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }
}

void mig_done(){

  printf(":::::::::::::::::migdone\n");

  int result;
  proc_data* data;

  data = (proc_data*)malloc(sizeof(proc_data));
  data->REQUEST = MIGDONE;
  data->pid = getpid();
  data->pos = mocu_pos;
  data->mem = mocu_get_amount_of_memory_used();
  
  result = send(sockfd,data,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }
}

void try_to_allocate(int size){
  int result;
  proc_data* data;

  data = (proc_data*)malloc(sizeof(proc_data));
  data->REQUEST = CUDAMALLOC;
  data->pid = getpid();
  data->pos = mocu_pos;
  data->mem = mocu_get_amount_of_memory_used()+size;

  result = send(sockfd,data,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

  result = recv(sockfd,data,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to recv msg\n");
    exit(-1);
  }

  request_from_daemon(data);
}

void failed_to_get(int size){

  printf("::::::::::::::::::::::::failed to allocate\n");

  int result;
  proc_data* data;

  data = (proc_data*)malloc(sizeof(proc_data));
  data->REQUEST = FAILEDTOGET;
  data->pid = getpid();
  data->pos = mocu_pos;
  data->mem = mocu_get_amount_of_memory_used()+size;
  
  result = send(sockfd,data,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

  mocu_backup();
  mocu.mocudaDeviceReset();

  suspended();
}

void suspended(){

  printf("::::::::::::::::::suspended...\n");

  int result;
  proc_data* data;

  data = (proc_data*)malloc(sizeof(proc_data));

  result = recv(sockfd,data,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to recv msg\n");
    exit(-1);
  }

  request_from_daemon(data);
}

void request_from_daemon(proc_data* data){

  printf("Request From Daemon ...(%d)\n",data->REQUEST);

  switch(data->REQUEST){
  case MIGRATE:
    printf("MIGRATE REQUEST !!!!!!!!!!!!!!!\n");
    mocu_migrate(data->pos);
    break;
  case SUSPEND:
    printf("SUSPENDED!!!!!!!!!!!!!!!!!!!!!!\n");
    mocu_backup();
    mocu.mocudaDeviceReset();
    suspended();
    break;
  case CONNECT:
    printf("JUST CONNECTED!!!!!!!!!!!!!!!!!\n");
    //TODO Nothing
    break;
  }
}
