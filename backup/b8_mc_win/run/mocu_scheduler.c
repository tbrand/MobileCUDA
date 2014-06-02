#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#include <nvml.h>

#define num 100

#define P1 "/home/taichirou/migrate_runtime/app/0_Simple/matrixMul/matrixMul"
#define P2 "/home/taichirou/migrate_runtime/app/0_Simple/matrixMulBig/matrixMul"
#define P3 "/home/taichirou/migrate_runtime/app/0_Simple/matrixMulSmall/matrixMul"
#define P4 "/home/taichirou/migrate_runtime/app/orig/test/test"
#define P5 "/home/taichirou/migrate_runtime/app/orig/testBig/test"
#define P6 "/home/taichirou/migrate_runtime/app/orig/testSmall/test"

pid_t PIDs[num];
int   Procs[num];

static float elapsed(struct timeval tv0,struct timeval tv1){
  return (float)(tv1.tv_sec - tv0.tv_sec);
  //    + (float)(tv1.tv_usec - tv0.tv_usec)
  //    * 0.000001f;
}

void method1(){
  printf("Suspended ...\n");
  int i;

  for(i = 0 ; i < num ; i ++ ){
    kill(PIDs[i],SIGINT);
  }
}

void register_signal(){
  signal(SIGINT,method1);
}

void main(int argc,char* argv[]){

  struct timeval tv0,tv1;

  gettimeofday(&tv0,NULL);

  int res = 0;
  int launched_num = 0;
  int received_num = 0;

  register_signal();

  srand(100);

  while(launched_num < num){

    pid_t child;
    int random;

    random = rand()%6;
    
    child = fork();
    
    if(child == 0){

      //      printf("Rand = %d\n",random);

      //      random += 3;

      switch(random){
      case 0:
	execl(P1,NULL);
	break;
      case 1:
	execl(P2,NULL);
	break;
      case 2:
	execl(P3,NULL);
	break;
      case 3:
	execl(P4,NULL);
	break;
      case 4:
	execl(P5,NULL);
	break;
      case 5:
	execl(P6,NULL);
	break;
      }

      res ++;
      
      exit(0);
    }else{

      PIDs[launched_num] = child;
      Procs[launched_num] = random;
      launched_num++;
    }
  }

  while(received_num < num){

    pid_t res;
    int status;

    res = wait(&status);

    received_num++;
  }

  printf("FINISH!!!!!!!\n");

  gettimeofday(&tv1,NULL);
  
  printf("Result time : %f[sec]\n",elapsed(tv0,tv1));
}
