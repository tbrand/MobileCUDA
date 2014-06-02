#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#include <nvml.h>

#define num 20

#define P1  "/home/taichirou/migrate_runtime/app/0_Simple/matrixMul/matrixMul"
#define P2  "/home/taichirou/migrate_runtime/app/0_Simple/matrixMulBig/matrixMul"
#define P3  "/home/taichirou/migrate_runtime/app/0_Simple/matrixMulSmall/matrixMul"
#define P4  "/home/taichirou/migrate_runtime/app/orig/test/test"
#define P5  "/home/taichirou/migrate_runtime/app/orig/testBig/test"
#define P6  "/home/taichirou/migrate_runtime/app/orig/testSmall/test"
#define P7  "/home/taichirou/migrate_runtime/app/orig/mem/test"
#define P8  "/home/taichirou/migrate_runtime/app/orig/memBig/test"
#define P9  "/home/taichirou/migrate_runtime/app/orig/memSmall/test"
#define P10 "/home/taichirou/migrate_runtime/app/orig/devmem/devmem"

typedef struct _record{
  pid_t pid;
  int proc;
  int s_h,s_m,s_s;
  int e_h,e_m,e_s;
} record;

record records[num];

time_t tt;
struct tm* ts;

void method1(){
  printf("Suspended ...\n");
  int i;

  for(i = 0 ; i < num ; i ++ ){
    kill(records[i].pid,SIGINT);
  }
}

void register_signal(){
  signal(SIGINT,method1);
}

void main(int argc,char* argv[]){

  int s_h,s_m,s_s;
  int e_h,e_m,e_s;

  time(&tt);
  ts = localtime(&tt);
  s_h = ts->tm_hour;
  s_m = ts->tm_min;
  s_s = ts->tm_sec;  

  int res = 0;
  int launched_num = 0;
  int received_num = 0;

  register_signal();

  srand(111);

  while(launched_num < num){

    pid_t child;
    int random;

    random = rand()%9;
    
    child = fork();
    
    if(child == 0){

      switch(random){
      case 0:
	execl(P1,P1,NULL);
	break;
      case 1:
	execl(P2,P2,NULL);
	break;
      case 2:
	execl(P3,P3,NULL);
	break;
      case 3:
	execl(P4,P4,NULL);
	break;
      case 4:
	execl(P5,P5,NULL);
	break;
      case 5:
	execl(P6,P6,NULL);
	break;
      case 6:
	execl(P7,P7,NULL);
	break;
      case 7:
	execl(P8,P8,NULL);
	break;
      case 8:
	execl(P9,P9,NULL);
	break;
      }

      res ++;
      
      exit(0);
    }else{

      records[launched_num].pid = child;
      records[launched_num].proc = random;

      time(&tt);
      ts = localtime(&tt);
      records[launched_num].s_h = ts->tm_hour;
      records[launched_num].s_m = ts->tm_min;
      records[launched_num].s_s = ts->tm_sec;

      launched_num++;

    }
  }

  while(received_num < num){

    pid_t res;
    int status;
    int i;

    res = wait(&status);

    for(i = 0 ; i < num ; i ++){

      if(records[i].pid == res){

	time(&tt);
	ts = localtime(&tt);
	records[i].e_h = ts->tm_hour;
	records[i].e_m = ts->tm_min;
	records[i].e_s = ts->tm_sec;
	
      }
    }

    received_num++;
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

  int i;
  for(i = 0 ; i < num ; i ++){
    printf("PROC[%d]\n",i);
    printf("\tIden : %d\n",records[i].proc);
    printf("\tPid  : %d\n",records[i].pid);
    printf("\t\tTIME  : %d[sec]\n",
	   (records[i].e_h-records[i].s_h)*60*60+
	   (records[i].e_m-records[i].s_m)*60+
	   (records[i].e_s-records[i].s_s)
	   );
    printf("\t\tSTART : %02d:%02d:%02d\n",records[i].s_h,records[i].s_m,records[i].s_s); 
    printf("\t\tEND   : %02d:%02d:%02d\n",records[i].e_h,records[i].e_m,records[i].e_s); 
  }
}
