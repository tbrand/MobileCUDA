#include <mocu.h>

#define NOREQUEST 0

int received_signal = 0;

void mocu_request(){

  if(!connected)mocu_connect();

  if(profile_status == PROFILE_DONE){
    mocu_destroy_profile();
    mocu_send_profile();
    profile_status = PROFILE_QUIT;
  }

  if(received_signal == NOREQUEST)return;

  printf("Received request from DAEMON\n");

  mocu.cp->msg->REQUEST = CANRECEIVE;

  SEND;

  RECV;

  received_signal = NOREQUEST;

}

void mocu_receive_signal(int sig){

  if(sig == SIGUSR1) received_signal = 1;

}

void mocu_register_signal(){
  if(SIG_ERR == signal(SIGUSR1,mocu_receive_signal)){
    printf("Failed to register signal function(SIGUSR1)\n");
  }
}
