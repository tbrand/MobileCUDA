#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <nvml.h>
#include <time.h>
#include <signal.h>

#define START_FROM_8_PROCS 1

#define PROC_NUM 20
#define DEV_NUM 4
#define PATH_TO_PROG   "/home/taichirou/mocu/mocu-5.5/app/0_Simple/matrixMul/matrixMul"
#define PATH_TO_PROG2  "/home/taichirou/mocu/mocu-5.5/app/orig/memoryBound"
//#define PATH_TO_PROG2 "../../app/1_Utilities/bandwidthTest/bandwidthTest"

#define MATRIX_MEMORY    2123//[MB]

int status;

nvmlDevice_t dev[DEV_NUM];
nvmlMemory_t mem;
time_t tt;
struct tm *ts,*te;

int proc1 = 0;
int proc2 = 0;

int proc1_counter = 0;
int proc2_counter = 0;

typedef struct my_pid_time{
  int proc;
  pid_t my_pid;
  struct timeval start;
  struct timeval end;
} __pid;

void nothing_method1(int pid){}
void nothing_method2(int pid){}

void register_signal(){
  if(SIG_ERR == signal(SIGUSR1,nothing_method1)){
    printf("failed to register signal handler\n");
  }
  if(SIG_ERR == signal(SIGUSR2,nothing_method2)){
    printf("failed to register signal handler\n");
  }
}

int launched_proc = 0;
int received_proc = 0;

void fork_child_proc();
int can_sub_proc();

__pid DATA[PROC_NUM];

static float elapsed(struct timeval tv0,struct timeval tv1){
  return (float)(tv1.tv_sec - tv0.tv_sec);
  //    + (float)(tv1.tv_usec - tv0.tv_usec)
  //    * 0.000001f;
}

void time_stamp(){
  printf("--------------\n");
  tt = time(NULL);
  ts = localtime(&tt);
  printf("%02d:%02d:%02d\n", ts->tm_hour, ts->tm_min, ts->tm_sec);
  printf("matrixMul to %d\n",proc1);
  printf("memcpy    to %d\n",proc2);
  printf("--------------\n");
}

int main(){

  register_signal();

  tt = time(NULL);
  ts = localtime(&tt);
  printf("%02d:%02d:%02d\n", ts->tm_hour, ts->tm_min, ts->tm_sec);

  srand(110);

  nvmlReturn_t res;

  res = nvmlInit();

  int i;
  for(i = 0 ; i < DEV_NUM ; i ++){
    res = nvmlDeviceGetHandleByIndex(i,&dev[i]);
  }

  struct timeval tv0,tv1;

  gettimeofday(&tv0,NULL);

#if START_FROM_8_PROCS
  while(can_sub_proc() && launched_proc < DEV_NUM*2){
#else
    while(launched_proc < PROC_NUM){
#endif

      fork_child_proc();
      launched_proc++;
      printf("Process %d launch.\n",launched_proc);
      //    sleep(9);//default
      sleep(2);
    }

    printf("First Step End...\n");

#if START_FROM_8_PROCS
    while(launched_proc++ < PROC_NUM){
      pid_t res;
      int status;

      res = wait(&status);

      struct timeval end;
      gettimeofday(&end,NULL);

      int j;
      for(j = 0 ; j < PROC_NUM ; j ++){
	if(DATA[j].my_pid == res){
	  DATA[j].end = end;
	  if(DATA[j].proc == 0){
	    proc1--;
	  }else{
	    proc2--;
	  }
	  time_stamp();
	}
      }
    
      received_proc++;

      printf("Process %d finished.\n",received_proc);

      if(can_sub_proc()){
	fork_child_proc();

	printf("Process %d launch.\n",launched_proc);
      }
    }

    printf("Second Step End...\n");
#endif

    while(received_proc++ < PROC_NUM){
      pid_t res;
      int status;

      res = wait(&status);

      struct timeval end;
      gettimeofday(&end,NULL);

      int j;
      for(j = 0 ; j < PROC_NUM ; j ++){
	if(DATA[j].my_pid == res){
	  DATA[j].end = end;
	  if(DATA[j].proc == 0){
	    proc1--;
	  }else{
	    proc2--;
	  }
	  time_stamp();
	}
      }
      printf("Process %d finished.\n",received_proc);
      printf("pid == %lld finish ... \n",res);
    }

    printf("Every processes completed!\n");

    gettimeofday(&tv1,NULL);

    printf("Result time : %f[sec]\n",elapsed(tv0,tv1));

    tt = time(NULL);
    te = localtime(&tt);
    printf("%02d:%02d:%02d\n", te->tm_hour, te->tm_min, te->tm_sec);

    int k;

    unsigned long long base = DATA[0].start.tv_sec;
  
    for(k = 0 ; k < PROC_NUM ; k ++){
      DATA[k].start.tv_sec -= base;
      DATA[k].end.tv_sec -= base;
    }

    for(k = 0 ; k < PROC_NUM ; k ++){
      printf("PID == %lld\n",DATA[k].my_pid);
      printf("pos : %d\n",k);
      printf("Proc : %d\n",DATA[k].proc);
      printf("Start : %d(%d)\n",DATA[k].start.tv_sec,(DATA[k].start.tv_sec*4)/5);
      printf("End   : %d\n",DATA[k].end.tv_sec);
      printf("Time  : %d(%d)\n",DATA[k].end.tv_sec - DATA[k].start.tv_sec,((DATA[k].end.tv_sec - DATA[k].start.tv_sec)*4)/5);
    }

    printf("matrixMul       : %d\n",proc1_counter);
    printf("memcpyIteration : %d\n",proc2_counter);

  }

  int can_sub_proc(){

    nvmlReturn_t res;

    int i;
    for(i = 0 ; i < DEV_NUM ; i ++){
      long memFree;

      res = nvmlDeviceGetMemoryInfo(dev[i],&mem);
      memFree = mem.free >> 20;

      if(memFree - MATRIX_MEMORY > 0){
	return 1;
      }
    }
    return 0;
  }

  void fork_child_proc(){

    int random = rand()%100;

    pid_t child = fork();
  
    if(child == 0){
      if(random < 50)
	execl(PATH_TO_PROG,NULL);
      else
	execl(PATH_TO_PROG2,NULL);
      exit(0);
    }else{
      struct timeval start;
      gettimeofday(&start,NULL);
      int i;
      for(i = 0 ; i < PROC_NUM ; i ++){
	if(DATA[i].my_pid == 0){
	  DATA[i].my_pid = child;
	  DATA[i].start = start;
	  if(random < 50){
	    DATA[i].proc = 0;
	    proc1++;
	    proc1_counter++;
	  }else{
	    DATA[i].proc = 1;
	    proc2++;
	    proc2_counter++;
	  }
	  time_stamp();
	  i = PROC_NUM;
	}
      }
    }
  }
