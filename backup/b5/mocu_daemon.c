#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <comm.h>
#include <nvml.h>

#define MAXPROC 8

#define TRUE  1
#define FALSE 0

typedef struct _procs{
  proc_data *data;
  int sd;
  int queued;
  struct _procs *prev,*next;
} proc;

typedef struct _dflag{
  int sd;
  int flag;
} dflag;

typedef struct _DEM{
  proc *p0,*p1;
  nvmlDevice_t* devs;
  unsigned int ndev;
  dflag *flags;
  unsigned int procCounter;
} DEM;

DEM dem;

FILE* fp = NULL;

int isDaemon = 0;
int queueStatus = 1;

int end_server = FALSE;

DEM dem;

void init_proc(){
  dem.p0 = (proc*)malloc(sizeof(proc));
  dem.p1 = (proc*)malloc(sizeof(proc));

  dem.p0->next = dem.p1;
  dem.p0->prev = NULL;
  dem.p1->next = NULL;
  dem.p1->prev = dem.p0;
}

proc* get_proc(int sd){
  proc* ptemp;
  
  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    if(ptemp->sd == sd)return ptemp;
    ptemp = ptemp->next;
  }

  return NULL;
}

void add_proc(proc *p){
  p->prev = dem.p1->prev;
  p->next = dem.p1;
  p->prev->next = p;
  p->next->prev = p;
}

void remove_proc(proc *p){
  p->next->prev = p->prev;
  p->prev->next = p->next;
  free(p);
}

void renew_proc(proc_data *data,int sd){
  proc* p;

  p = get_proc(sd);

  if(p==NULL)return;

  p->data->pos = data->pos;
  p->data->mem = data->mem;
  p->data->REQUEST = data->REQUEST;
}

proc* create_proc(int sd){
  proc* p;

  p = (proc*)malloc(sizeof(proc));
  p->sd = sd;
  p->queued = 0;
  p->data = (proc_data*)malloc(sizeof(proc_data));
  p->data->pid = 0;
  p->data->mem = 0;
  p->data->pos = 0;

  return p;
}

int queue_size(){
  
  proc* ptemp;
  int res = 0;

  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    if(ptemp->queued == 1)res++;
    ptemp = ptemp->next;
  }

  return res;
}

void dequeueSpecifyProc(proc* p){

  if(!queueStatus)return;

  nvmlReturn_t res;
  nvmlMemory_t mem;
  int flag;

  flag = 1;

  if(p == NULL || p->queued != 1)return;

  flag = 0;

  int devPos;

  for(devPos = 0 ; devPos < dem.ndev ; devPos ++){

    if(dem.flags[devPos].flag){
      printf("\tdevice[%d] cannot be selected...\n",devPos);
      continue;
    }

    res = nvmlDeviceGetMemoryInfo(dem.devs[devPos],&mem);

    if(res != NVML_SUCCESS){
      printf("Failed to get Memory Info in dequeue\n");
      exit(-1);
    }

    if(mem.free > p->data->mem){

      dem.flags[devPos].sd = p->sd;
      dem.flags[devPos].flag = 1;

      p->queued = 0;
      p->data->REQUEST = MIGRATE;
      p->data->pos = devPos;
      send(p->sd,p->data,sizeof(proc_data),0);

      flag = 1;

      break;
    }
  }

  if(!flag) queueStatus = 0;

}

void dequeue(){

  if(!queueStatus)return;

  proc* ptemp;
  nvmlReturn_t res;
  nvmlMemory_t mem;
  
  ptemp = dem.p0->next;
  
  while(ptemp->next != NULL){

    if(ptemp->queued == 1){

      dequeueSpecifyProc(ptemp);

    }

    if(!queueStatus)break;

    ptemp = ptemp->next;
  }
}

void _end_server(int sig){
  if(sig == SIGINT)
    end_server = TRUE;
}

void mocu_check(){}

int sem_id;

void _FIN(int sd){

  dem.procCounter--;

  printf("FIN(NUM OF PROCS:%d)\n",dem.procCounter);
  
  proc* p;

  p = get_proc(sd);

  remove_proc(p);

  queueStatus = 1;
  
  dequeue();
  
}

void _CONNECT(int sd){

  dem.procCounter++;

  printf("CONNECT (QUEUESIZE   :%d)\n",queue_size());
  printf("        (NUM OF PROCS:%d)\n",dem.procCounter);

  proc_data* sendProc;
  proc* p;

  sendProc = (proc_data*)malloc(sizeof(proc_data));
  p = get_proc(sd);

  if(queue_size() > 0 || dem.procCounter > MAXPROC){

    sendProc->REQUEST = SUSPEND;
    p->queued = 1;

  }else{

    sendProc->REQUEST = CONNECT;

  }

  send(sd,sendProc,sizeof(proc_data),0);
}

void _RENEW(int sd,proc_data* data){

  printf("RENEW\n");

  proc_data* sendProc;
  proc* p;

  sendProc = (proc_data*)malloc(sizeof(proc_data));
  p = get_proc(sd);

  renew_proc(data,sd);

  dequeue();
}

void _MIGDONE(int sd,proc_data* data){

  proc* p;
  int devPos;

  devPos = data->pos;

  printf("MIGDONE(%d)\n",devPos);

  p = get_proc(sd);
	      
  if(dem.flags[devPos].sd == sd){

    printf("\tSUCCESS TO MIGRATE\n");

  }else{

    printf("\tFAILED TO MIGRATE(GET INCORRECT INFORMATION)\n");
    printf("\tTHE RECEIVED  DATA IS %d[SD]\n",sd);
    printf("\tTHE REGISTERD DATA IS %d[SD]\n",dem.flags[devPos]);
		
  }

  dem.flags[devPos].sd = -1;
  dem.flags[devPos].flag = 0;

  renew_proc(data,sd);

  queueStatus = 1;

  dequeue();
}

void _CANRECEIVE(int sd,proc_data* data){

  printf("CANRECEIVE\n");

  proc_data* sendProc;
  proc* p;

  sendProc = (proc_data*)malloc(sizeof(proc_data));
  p = get_proc(sd);

  send(sd,p->data,sizeof(proc_data),0);

  p->data->REQUEST = CONNECT;
}

void _FAILEDTOGET(int sd,proc_data* data){

  printf("FAILEDTOGET\n");

  proc* p;

  p = get_proc(sd);

  p->data = data;
  p->queued = 1;

  queueStatus = 0;
}

void _CUDAMALLOC(int sd,proc_data* data){

  printf("CUDAMALLOC\n");

  proc_data* sendProc;
  proc* p;
  size_t prevRegion;
  nvmlReturn_t res;
  nvmlMemory_t mem;
  int devp;

  sendProc = (proc_data*)malloc(sizeof(proc_data));
  p = get_proc(sd);

  prevRegion = p->data->mem;

  p->data = data;

  devp = data->pos;

  res = nvmlDeviceGetMemoryInfo(dem.devs[devp],&mem);

  if(res != NVML_SUCCESS){
    printf("Failed to get Memory Information\n");
    exit(-1);
  }

  if(mem.free > data->mem-prevRegion){

    printf("\tGOAHEAD\n");

    sendProc->REQUEST = CONNECT;
    send(sd,sendProc,sizeof(proc_data),0);

  }else{

    printf("\tOOPS\n");

    sendProc->REQUEST = SUSPEND;
    p->queued = 1;

    send(sd,sendProc,sizeof(proc_data),0);

    //    dequeueWithsd(sd);
    dequeueSpecifyProc(p);

  }
}

int main(int argc,char* argv[]){

  /**Initialize signal**/

  signal(SIGINT ,_end_server);
  signal(SIGUSR1,_end_server);

  /**Initialize struct proc**/
  init_proc();

  /**Process becomes dem**/
  pid_t process_id = 0;
  pid_t sid = 0;

  if(argc >= 2){

    process_id = fork();

    if(process_id < 0){
      printf("fork failed ..\n");
      exit(1);
    }

    if(process_id > 0){
      exit(0);
    }

    umask(0);

    sid = setsid();

    if(sid < 0){
      exit(1);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

  }else{
    sid = getpid();
  }

  /**Setup the log file**/

  char log[32];

  sprintf(log,"log.%u",sid);

  fp = fopen(log,"w+");

  //  printf("Start constructing deamon for Mobile CUDA processes\n");

  /**Start Initialize nvidia management library from Here!!**/

  nvmlReturn_t nres;
  int i;
  
  nres = nvmlInit();

  if(nres != NVML_SUCCESS){
    perror("Failed to initialize Nvidia Managerment Library...\n");
    exit(-1);
  }

  nres = nvmlDeviceGetCount(&dem.ndev);

  if(nres != NVML_SUCCESS){
    perror("Failed to get num of device...\n");
    exit(-1);
  }

  dem.devs = (nvmlDevice_t*)malloc(sizeof(nvmlDevice_t)*dem.ndev);
  dem.flags = (dflag*)malloc(sizeof(dflag)*dem.ndev);

  for(i = 0 ; i < dem.ndev ; i ++){

    nres = nvmlDeviceGetHandleByIndex(i,&dem.devs[i]);

    if(nres != NVML_SUCCESS){
      perror("Failed to get device handle\n");
      exit(-1);
    }

    dem.flags[i].sd = -1;
    dem.flags[i].flag = 0;
  }

  dem.procCounter = 0;

  //  printf("Success to get handle of each device (num of dev == %d)\n",dem.ndev);

  /**Setup the socket**/

  int len,rc,on = 1;
  int listen_sd,max_sd,new_sd;
  int desc_ready;
  int close_conn;

  struct sockaddr_un addr;
  struct timeval timeout;
  fd_set master_set,working_set;

  listen_sd = socket(AF_UNIX,SOCK_STREAM,0);

  if(listen_sd < 0){
    perror("socket() failed\n");
    exit(-1);
  }

  rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char*)&on,sizeof(on));

  if(rc < 0){
    perror("setsockopt() failed\n");
    exit(-1);
  }

  unlink("mocu_server");

  memset(&addr,0,sizeof(addr));

  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path,"mocu_server");

  rc = bind(listen_sd,(struct sockaddr*)&addr,sizeof(addr));

  if(rc < 0){
    perror("bind() failed");
    close(listen_sd);
    exit(-1);
  }

  rc = listen(listen_sd,10);

  if(rc < 0){
    perror("listen() failed");
    close(listen_sd);
    exit(-1);
  }

  FD_ZERO(&master_set);
  max_sd = listen_sd;
  FD_SET(listen_sd,&master_set);

  timeout.tv_sec = 3*60;
  timeout.tv_usec = 0;

  //  printf("Success to construct deamon, Entering loop...\n");

  //  printf("SOMAXCONN : %d\n",SOMAXCONN);

  long counter = 0;

  /**Entering main loop**/

  do{

    //    printf("[Loop(%u)]\n",++counter);

    memcpy(&working_set,&master_set,sizeof(master_set));

    rc = select(max_sd+1, &working_set, NULL, NULL, NULL);

    if(rc < 0){
      perror("select() failed\n");
      break;
    }

    if(rc == 0){
      printf("select() time out. End program.\n");
      break;
    }

    desc_ready = rc;

    for(i = 0 ; i < max_sd+1 && desc_ready > 0 ; ++i){
      
      if(FD_ISSET(i,&working_set)){

	//	printf("%d tried to connect to me!!\n",i);

	desc_ready = -1;

	if(i == listen_sd){

	  //	  printf("Listening socket is readable\n");

	  new_sd = accept(listen_sd,NULL,NULL);

	  if(new_sd < 0){
	    printf("accept() failed");
	    end_server = TRUE;
	  }

	  FD_SET(new_sd,&master_set);

	  if(new_sd > max_sd){
	    max_sd = new_sd;
	  }

	  proc* p = create_proc(new_sd);

	  add_proc(p);

	}else{

	  proc_data* receivedProc = (proc_data*)malloc(sizeof(proc_data));

	  rc = recv(i,receivedProc,sizeof(proc_data),0);

	  if(rc <= 0){

	    FD_CLR(i,&master_set);

	    _FIN(i);

	  }else{

	    if(receivedProc->REQUEST == CONNECT){

	      _CONNECT(i);

	    }else if(receivedProc->REQUEST == RENEW){

	      _RENEW(i,receivedProc);
	      
	    }else if(receivedProc->REQUEST == MIGDONE){

	      _MIGDONE(i,receivedProc);
	      
	    }else if(receivedProc->REQUEST == CANRECEIVE){

	      _CANRECEIVE(i,receivedProc);
	      
	    }else if(receivedProc->REQUEST == FAILEDTOGET){

	      _FAILEDTOGET(i,receivedProc);
	      
	    }else if(receivedProc->REQUEST == CUDAMALLOC){

	      _CUDAMALLOC(i,receivedProc);

	    }
	  }
	}
      }
    }

    mocu_check();

  }while(end_server == FALSE);

  int closed = 0;

  for(i = 0 ; i < max_sd ; i ++){
    if(FD_ISSET(i,&master_set)){
      close(i);
      closed = 1;
    }
  }

  fclose(fp);

  if(!closed){
    semctl(sem_id,0,IPC_RMID);
  }

  return 0;
}
