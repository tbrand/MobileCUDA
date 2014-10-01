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

  mocu_symbol_backup();

#if BACKUP
  printf("[MOCU] Finished symbol backup\n");
#endif

  mocu_array_backup();

#if BACKUP
  printf("[MOCU] Finished array backup\n");
#endif

}

void mocu_migrate(int devID){

  mocu.mocudaSetDevice(devID);

  apilog* a;

  a = mocu.cp->a0->next;

  while(a->next != NULL){

    if(replay(a) == -1){
      printf("Exit(-1)\n");
      exit(-1);
      break;
    }

    a = a->next;

  }

  printf("\t===> REPLAY SUCCESS\n");

  mocu_event_restore();
  mocu_stream_restore();
  mocu_device_restore();
  mocu_symbol_restore();
  mocu_array_restore();

  mocu_pos = devID;

  mocu_mig_done();
}
