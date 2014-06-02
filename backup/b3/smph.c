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

void mocu_detach_smph(){
  int res = semctl(sem_id,0,IPC_RMID);
  //  printf("Detach semaphore : %d\n",res);
}

void mocu_lock(){
  struct sembuf sops;
    
  sops.sem_num = 0;
  sops.sem_op = -1;
  sops.sem_flg = SEM_UNDO;
  int res = semop(sem_id,&sops,1);
  //  printf("Lock other procs(%d)\n",res);
}

void mocu_unlock(){
  struct sembuf sops;
    
  sops.sem_num = 0;
  sops.sem_op = 1;
  sops.sem_flg = SEM_UNDO;
  int res = semop(sem_id,&sops,1);
  //  printf("Unlock other procs(%d)\n",res);
}
