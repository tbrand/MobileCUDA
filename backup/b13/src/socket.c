#include <mocu.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int sockfd;
int canmig;
int created_context = 0;

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
  mocu.cp->msg->sym = 0;
  mocu.cp->msg->REQUEST = CONNECT;

  SEND;

  RECV;
}

void mocu_register_var(size_t size){

  //  mocu.cp->msg->mem = mocu_get_amount_of_memory_used();
  //  mocu.cp->msg->req = size;
  mocu.cp->msg->sym += size;//= total_symbol_size();
  mocu.cp->msg->REQUEST = CONNECT;

  SEND;

  RECV;
}

void mocu_malloc_done(size_t size){

  mocu.cp->msg->REQUEST = MALLOCDONE;
  mocu.cp->msg->mem += size;//mocu_get_amount_of_memory_used();
  //  mocu.cp->msg->req = size;
  //  mocu.cp->msg->sym = total_symbol_size();

  SEND;

  mocu.cp->msg->req = 0;

  //  RECV;
}

void mocu_send_renew(){

#if 0
  mocu.cp->msg->REQUEST = RENEW;
  mocu.cp->msg->pos = mocu_pos;
  mocu.cp->msg->flag = canmig;
  mocu.cp->msg->mem = mocu_get_amount_of_memory_used();
  mocu.cp->msg->req = 0;
  mocu.cp->msg->sym = total_symbol_size();

  SEND;
#endif

}

void mocu_mig_done(){

  //  printf("MIGDONE : %d\n",mocu_pos);

  mocu.cp->msg->REQUEST = MIGDONE;
  mocu.cp->msg->pos = mocu_pos;
  
  SEND;
}

void mocu_try_to_allocate(size_t size){

  //  printf("TRY TO ALLOCATE!!!(MEM : %d)(SIZE : %d)\n",mocu_get_amount_of_memory_used(),size);

  mocu.cp->msg->REQUEST = CUDAMALLOC;
  //  mocu.cp->msg->pos = mocu_pos;
  //  mocu.cp->msg->mem = mocu_get_amount_of_memory_used();
  mocu.cp->msg->req = size;
  //  mocu.cp->msg->sym = total_symbol_size();

  SEND;

  RECV;
}

void mocu_failed_to_get(int size){

  mocu.cp->msg->REQUEST = FAILEDTOALLOC;
  //  mocu.cp->msg->pos = mocu_pos;
  //  mocu.cp->msg->mem = mocu_get_amount_of_memory_used();
  //  mocu.cp->msg->req = size;
  //  mocu.cp->msg->sym = total_symbol_size();

  SEND;

  mocu_suspended();
}

void mocu_cannot_enter(){

  RECV;

}

void mocu_suspended(){

  mocu_backup();
  mocu.mocudaDeviceReset();

  mocu.cp->msg->REQUEST = BACKUPED;

  SEND;

  RECV;
}

void mocu_leave(){

  if(created_context) return;

  mocu.cp->msg->REQUEST = CONTEXT_CHECK;
  //  mocu.cp->msg->pos = mocu_pos;
  //  mocu.cp->msg->mem = mocu_get_amount_of_memory_used();
  //  mocu.cp->msg->req = 0;
  //  mocu.cp->msg->sym = total_symbol_size();

  SEND;

  RECV;
}

void create_context(){

#if 0
  mocu.cp->msg->REQUEST = CREATE_CONTEXT;

  SEND;

  created_context = 1;
#endif
}

void mocu_request_from_daemon(proc_data* data){

  switch(data->REQUEST){
  case MIGRATE:
    //    printf("MIGRATE REQUEST!!!!!!(%d to %d)\n",mocu_pos,data->pos);
    mocu_migrate(data->pos);
    break;
  case SUSPEND:
    mocu_suspended();
    break;
  case CANNOTENTER:
    mocu_cannot_enter();
    break;
  case CONNECT:
    //    printf("\t\t\tCONNECT FROM DAEMON(%d)\n",data->pos);
    if(mocu_pos != data->pos){
      //      mocu.mocudaDeviceReset();
      mocu.mocudaSetDevice(data->pos);
      mocu_pos = data->pos;
      mocu.cp->msg->pos = mocu_pos;
    }
    break;
  case GOAHEAD:
    //to do nothing
    break;
  case CCHECK_OK:

    created_context = 1;
    

    break;
  case CCHECK_FAILED:
    //to do nothing

    break;
  }
}

void mocu_send(){

  int result;

  result = send(sockfd,mocu.cp->msg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to send msg\n");
    exit(-1);
  }  
}

void mocu_recv(){

  int result;

  result = recv(sockfd,mocu.cp->rmsg,sizeof(proc_data),0);

  if(result == -1){
    printf("Failed to recv msg\n");
    exit(-1);
  }

  mocu_request_from_daemon(mocu.cp->rmsg);  
  
}
