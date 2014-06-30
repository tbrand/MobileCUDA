#include <stdio.h>		
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define PATH_TO_PROG1 "../../app/0_Simple/matrixMul/matrixMul"
#define PATH_TO_PROG2 "../../app/0_Simple/matrixMulBig/matrixMul"
#define PATH_TO_PROG3 "../../app/0_Simple/matrixMulSmall/matrixMul"
#define PATH_TO_PROG4 "../../app/orig/test/test"
#define PATH_TO_PROG5 "../../app/orig/testBig/test"
#define PATH_TO_PROG6 "../../app/orig/testSmall/test"
#define PATH_TO_PROG7 "../../app/orig/mem/test"
#define PATH_TO_PROG8 "../../app/orig/memBig/test"
#define PATH_TO_PROG9 "../../app/orig/memSmall/test"
#define PATH_TO_PROG10 "/home/taichirou/migrate_runtime/app/orig/devmem/devmem"
#define PATH_TO_PROG11  "/home/taichirou/migrate_runtime/app/orig/malloc_in_kernel/mik" 
#define PATH_TO_PROG12  "/home/taichirou/migrate_runtime/app/map/map"

#define PROC_NUM 100
#define DEV_NUM 1

time_t tt;
struct tm* ts;

typedef struct _record{
  pid_t pid;
  int proc;
  int pos;
  int s_h,s_m,s_s;
  int e_h,e_m,e_s;
} record;

pid_t pids[DEV_NUM];

record records[PROC_NUM];

int status;
int proc_counter;

void fork_orig_proc(int);
pid_t wait_proc();

int main(){

  srand(109);

  int s_h,s_m,s_s;
  int e_h,e_m,e_s;

  time(&tt);
  ts = localtime(&tt);
  s_h = ts->tm_hour;
  s_m = ts->tm_min;
  s_s = ts->tm_sec;

  proc_counter = 0;

  int i;
  for(i = 0 ; i < DEV_NUM ; i ++){
    fork_orig_proc(i);
  }

  int fin = 0;

  while(!fin){
    pid_t res;
    int j;

    res = wait_proc();

    for(j = 0 ; j < DEV_NUM ; j ++){
      if(pids[j] == res){

	proc_counter ++;

	if(proc_counter + DEV_NUM <= PROC_NUM){
	  fork_orig_proc(j);
	  break;
	}else if(proc_counter == PROC_NUM){
	  fin = 1;
	  break;
	}

      }
    }
  }

  time(&tt);
  ts = localtime(&tt);
  e_h = ts->tm_hour;
  e_m = ts->tm_min;
  e_s = ts->tm_sec;

  printf("\n\nResult Time : %d[sec]\n\n",
	 (e_h-s_h)*60*60+
	 (e_m-s_m)*60+
	 (e_s-s_s)
	 );

  for(i = 0 ; i < PROC_NUM ; i ++){
    printf("PROC[%d]\n",i);
    printf("\tIden : %d\n",records[i].proc);
    printf("\tPid  : %d\n",records[i].pid);
    printf("\tPos  : %d\n",records[i].pos);
    printf("\t\tTIME  : %d[sec]\n",
	   (records[i].e_h-records[i].s_h)*60*60+
	   (records[i].e_m-records[i].s_m)*60+
	   (records[i].e_s-records[i].s_s)
	   );
    printf("\t\tSTART : %02d:%02d:%02d\n",records[i].s_h,records[i].s_m,records[i].s_s); 
    printf("\t\tEND   : %02d:%02d:%02d\n",records[i].e_h,records[i].e_m,records[i].e_s); 
  }

  return 0;
}

int switch_counter = 0;
int rpos = 0;

void fork_orig_proc(int pos){

  int random = rand()%12;
  
  pids[pos] = fork();

  if(pids[pos] < 0){

    exit(-1);

  }else if(pids[pos] == 0){

    if(pos == 0)
      putenv("CUDA_VISIBLE_DEVICES=0");
    else if(pos == 1)
      putenv("CUDA_VISIBLE_DEVICES=1");
    else if(pos == 2)
      putenv("CUDA_VISIBLE_DEVICES=2");
    else if(pos == 3)
      putenv("CUDA_VISIBLE_DEVICES=3");

    switch(random){
    case 0:
      execl(PATH_TO_PROG1,PATH_TO_PROG1,NULL);
      break;
    case 1:
      execl(PATH_TO_PROG2,PATH_TO_PROG2,NULL);
      break;
    case 2:
      execl(PATH_TO_PROG3,PATH_TO_PROG3,NULL);
      break;
    case 3:
      execl(PATH_TO_PROG4,PATH_TO_PROG4,NULL);
      break;
    case 4:
      execl(PATH_TO_PROG5,PATH_TO_PROG5,NULL);
      break;
    case 5:
      execl(PATH_TO_PROG6,PATH_TO_PROG6,NULL);
      break;
    case 6:
      execl(PATH_TO_PROG7,PATH_TO_PROG7,NULL);
      break;
    case 7:
      execl(PATH_TO_PROG8,PATH_TO_PROG8,NULL);
      break;
    case 8:
      execl(PATH_TO_PROG9,PATH_TO_PROG9,NULL);
      break;
    case 9:
      execl(PATH_TO_PROG10,PATH_TO_PROG10,NULL);
      break;
    case 10:
      execl(PATH_TO_PROG11,PATH_TO_PROG11,NULL);
      break;
    case 11:
      execl(PATH_TO_PROG12,PATH_TO_PROG12,NULL);
      break;
    }

    exit(-1);

  }else{
    
      records[rpos].pid = pids[pos];
      records[rpos].proc = random;

      time(&tt);
      ts = localtime(&tt);
      records[rpos].s_h = ts->tm_hour;
      records[rpos].s_m = ts->tm_min;
      records[rpos].s_s = ts->tm_sec;
      records[rpos].pos = pos;
      rpos++;

  }
}

pid_t wait_proc(){

  pid_t r = waitpid(-1,&status,0);

  if(r < 0){
    perror("waitpid");
    exit(-1);
  }

  int i;

  for(i = 0 ; i < PROC_NUM ; i ++){
    
    if(records[i].pid == r){
      
      time(&tt);
      ts = localtime(&tt);
      records[i].e_h = ts->tm_hour;
      records[i].e_m = ts->tm_min;
      records[i].e_s = ts->tm_sec;
    }
  }
  
  if(WIFEXITED(status)){
    //child process complete.
    printf("child %d complete\n",proc_counter);
    return r;
  }
}
