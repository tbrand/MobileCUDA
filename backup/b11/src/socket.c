#include <mocu.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int sockfd;
int canmig;

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

  canmig = CANMIG;
  char* mocu_env;

  mocu_env = getenv("MOCU_CANMIG");

  if(mocu_env != NULL)
    if(strcmp(mocu_env,"no") == 0)
      canmig = 0;

  mocu.cp->msg->pos = mocu_pos;
  mocu.cp->msg->flag = canmig;
  mocu.cp->msg->mem = 0;
  mocu.cp->msg->req = 0;
  mocu.cp->msg->REQUEST = CONNECT;

  result = send(sockfd,mocu.cp->msg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

  result = recv(sockfd,mocu.cp->rmsg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to recv msg\n");
    exit(-1);
  }

  request_from_daemon(mocu.cp->rmsg);
}

void mocu_register_var(){

  int result;

  mocu.cp->msg->mem = 0;
  mocu.cp->msg->req = mocu_get_amount_of_memory_used();
  mocu.cp->msg->REQUEST = CONNECT;

  result = send(sockfd,mocu.cp->msg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

  result = recv(sockfd,mocu.cp->rmsg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to recv msg\n");
    exit(-1);
  }

  request_from_daemon(mocu.cp->rmsg);
  
}

void malloc_done(size_t size){

  int result;

  mocu.cp->msg->REQUEST = MALLOCDONE;
  mocu.cp->msg->mem = mocu_get_amount_of_memory_used();
  mocu.cp->msg->req = size;

  result = send(sockfd,mocu.cp->msg,sizeof(proc_data),0);
  
  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

}

void send_renew(){
  int result;

  mocu.cp->msg->REQUEST = RENEW;
  mocu.cp->msg->pos = mocu_pos;
  mocu.cp->msg->flag = canmig;
  mocu.cp->msg->mem = mocu_get_amount_of_memory_used();
  mocu.cp->msg->req = 0;

  result = send(sockfd,mocu.cp->msg,sizeof(proc_data),0);
  
  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

}

void mig_done(){

  printf("MIGDONE : %d\n",mocu_pos);

  int result;

  mocu.cp->msg->REQUEST = MIGDONE;
  mocu.cp->msg->pos = mocu_pos;
  
  result = send(sockfd,mocu.cp->msg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

}

void try_to_allocate(size_t size){
  int result;

  printf("TRY TO ALLOCATE!!!(MEM : %d)(SIZE : %d)\n",mocu_get_amount_of_memory_used(),size);

  mocu.cp->msg->REQUEST = CUDAMALLOC;
  mocu.cp->msg->pos = mocu_pos;
  mocu.cp->msg->mem = mocu_get_amount_of_memory_used();
  mocu.cp->msg->req = size;

  result = send(sockfd,mocu.cp->msg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

  result = recv(sockfd,mocu.cp->rmsg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to recv msg\n");
    exit(-1);
  }

  request_from_daemon(mocu.cp->rmsg);

}

void failed_to_get(int size){

  int result;

  mocu.cp->msg->REQUEST = FAILEDTOGET;
  mocu.cp->msg->pos = mocu_pos;
  mocu.cp->msg->mem = mocu_get_amount_of_memory_used();
  mocu.cp->msg->req = size;

  result = send(sockfd,mocu.cp->msg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

  suspended();
}

void cannot_enter(){

  int result;

  result = recv(sockfd,mocu.cp->rmsg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to recv msg\n");
    exit(-1);
  }

  request_from_daemon(mocu.cp->rmsg);
}

void suspended(){

  int result;

  mocu_backup();
  mocu.mocudaDeviceReset();

  mocu.cp->msg->REQUEST = BACKUPED;
  mocu.cp->msg->pos = mocu_pos;

  result = send(sockfd,mocu.cp->msg,sizeof(proc_data),0);
  
  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }

  result = recv(sockfd,mocu.cp->rmsg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to recv msg\n");
    exit(-1);
  }

  request_from_daemon(mocu.cp->rmsg);
  
}

void request_from_daemon(proc_data* data){

  switch(data->REQUEST){
  case MIGRATE:
    printf("MIGRATE REQUEST!!!!!!(%d to %d)\n",mocu_pos,data->pos);
    mocu_migrate(data->pos);
    break;
  case SUSPEND:
    suspended();
    break;
  case CANNOTENTER:
    cannot_enter();
    break;
  case CONNECT:
    printf("\t\t\tCONNECT FROM DAEMON(%d)\n",data->pos);
    mocu.mocudaSetDevice(data->pos);
    mocu_pos = data->pos;
    break;
  case GOAHEAD:
    //to do nothing
    break;
  }
}
