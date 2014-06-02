#include <mocu.h>

#define NOREQUEST 0

int received_signal = 0;

void mocu_request(){

  if(received_signal == NOREQUEST)return;

  proc_data* proc = (proc_data*)malloc(sizeof(proc_data));

  proc->pid = getpid();
  proc->pos = mocu_pos;
  proc->REQUEST = CANRECEIVE;
  proc->mem = mocu_get_amount_of_memory_used();

  send(sockfd,proc,sizeof(proc_data),0);
  recv(sockfd,proc,sizeof(proc_data),0);

  printf("Receive proc : \n");

  request_from_daemon(proc);

  received_signal = NOREQUEST;

}

void mocu_receive_signal(int sig){

  if(sig == SIGUSR1){
    received_signal = 1;
  }
}

void mocu_register_signal(){
  if(SIG_ERR == signal(SIGUSR1,mocu_receive_signal)){
    printf("Failed to register signal function(SIGUSR1)\n");
  }
}
