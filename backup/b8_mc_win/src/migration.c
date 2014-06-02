#include <mocu.h>

#define BACKUP 1

void mocu_backup(){
#if BACKUP
  printf("[MOCU] Start mocu_backup()\n");
#endif

  mocu_event_update();

#if BACKUP
  printf("[MOCU] Finished events update\n");
#endif

  mocu_device_backup();

#if BACKUP
  printf("[MOCU] Finished device backup\n");
#endif

}

void mocu_migrate(int devID){

  mocu.mocudaSetDevice(devID);

  apilog* a;

  a = mocu.cp->a0;

  while(a->next != NULL){

    if(replay(a) == -1){
      printf("Exit(-1)\n");
      exit(-1);
      break;
    }

    a = a->next;

  }

  mocu_event_restore();
  mocu_stream_restore();
  mocu_device_restore();

  mocu_pos = devID;

  mig_done();
}
