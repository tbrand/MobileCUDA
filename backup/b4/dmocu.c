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

#include <mocu.h>
#include <nvml.h>

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
} DEM;

DEM dem;

FILE* fp = NULL;

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

void print_proc(){

  proc* ptemp = dem.p0->next;

  while(ptemp->next != NULL){
    fprintf(fp,"--------------\n");
    fprintf(fp,"proc->sd : %d\n",ptemp->sd);
    fprintf(fp,"proc->data->pid : %d\n",ptemp->data->pid);
    fprintf(fp,"proc->data->pos : %d\n",ptemp->data->pos);
    fprintf(fp,"proc->data->mem : %d[MB]\n",ptemp->data->mem >> 20);
    ptemp = ptemp->next;
  }
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
    if(ptemp->queued)res++;
    ptemp = ptemp->next;
  }

  return res;
}

void dequeue(){
  
  proc* ptemp;
  nvmlReturn_t res;
  nvmlMemory_t mem;
  
  ptemp = dem.p0->next;
  
  while(ptemp->next != NULL){
    
    if(ptemp->queued){

      int devPos;

      for(devPos = 0 ; devPos < dem.ndev ; devPos ++){
	
	if(dem.flags[devPos].flag)continue;
	
	res = nvmlDeviceGetMemoryInfo(dem.devs[devPos],&mem);
	
	if(res != NVML_SUCCESS){
	  printf("Failed to get Memory Info in dequeue\n");
	  exit(-1);
	}
	
	if(mem.free > ptemp->data->mem){
	  dem.flags[devPos].sd = ptemp->sd;
	  dem.flags[devPos].flag = 1;
	  
	  ptemp->queued = 0;
	  ptemp->data->REQUEST = MIGRATE;
	  ptemp->data->pos = devPos;
	  send(ptemp->sd,ptemp->data,sizeof(proc_data),0);
	  break;
	}
      } 
    }

    ptemp = ptemp->next;
  }
}

void _end_server(int sig){
  if(sig == SIGINT)
    end_server = TRUE;
}

void mocu_check(){
  //May be a Knapsack Problem...?
}

int sem_id;

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

  fprintf(fp,"Start constructing deamon for Mobile CUDA processes\n");

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

  fprintf(fp,"Success to get handle of each device (num of dev == %d)\n",dem.ndev);

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

  rc = listen(listen_sd,32);

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

  fprintf(fp,"Success to construct deamon, Entering loop...\n");

  long counter = 0;

  /**Entering main loop**/

  do{

    fprintf(fp,"[Loop(%u)]\n",++counter);

    memcpy(&working_set,&master_set,sizeof(master_set));

    rc = select(max_sd+1, &working_set, NULL, NULL, NULL);

    if(rc < 0){
      perror("select() failed\n");
      break;
    }

    if(rc == 0){
      fprintf(fp,"select() time out. End program.\n");
      break;
    }

    desc_ready = rc;

    for(i = 0 ; i < max_sd+1 && desc_ready > 0 ; ++i){
      
      if(FD_ISSET(i,&working_set)){

	fprintf(fp,"%d tried to connect to me!!\n",i);

	desc_ready = -1;

	if(i == listen_sd){

	  fprintf(fp,"Listening socket is readable\n");

	  new_sd = accept(listen_sd,NULL,NULL);

	  if(new_sd < 0){
	    fprintf(fp,"accept() failed");
	    end_server = TRUE;
	  }

	  FD_SET(new_sd,&master_set);

	  if(new_sd > max_sd){
	    max_sd = new_sd;
	  }

	  proc* p = create_proc(new_sd);

	  add_proc(p);

	}else{

	  fprintf(fp,"Descriptor %d is readable\n",i);

	  proc_data* receivedProc;
	  proc_data* sendProc;
	  proc* p;
	  int devPos;
	  size_t prevRegion;
	  nvmlReturn_t res;
	  nvmlMemory_t mem;

	  receivedProc = (proc_data*)malloc(sizeof(proc_data));
	  sendProc     = (proc_data*)malloc(sizeof(proc_data));
	  p            = get_proc(i);

	  devPos = receivedProc->pos;

	  rc = recv(i,receivedProc,sizeof(proc_data),0);

	  if(rc <= 0){

	    fprintf(fp,"Connection closed from %d\n",i);
	    FD_CLR(i,&master_set);

	    remove_proc(p);

	    dequeue();

	  }else{

	    switch(receivedProc->REQUEST){

	    case CONNECT:

	      if(queue_size() > 0){

		sendProc->REQUEST = SUSPEND;
		p->queued = 1;

	      }else{

		sendProc->REQUEST = CONNECT;

	      }

	      send(i,sendProc,sizeof(proc_data),0);

	      dequeue();

	      break;
	    case RENEW:

	      renew_proc(receivedProc,i);

	      dequeue();

	      break;
	    case MIGDONE:

	      dem.flags[devPos].sd = -1;
	      dem.flags[devPos].flag = 0;

	      renew_proc(receivedProc,i);

	      dequeue();

	      break;
	    case CANRECEIVE:

	      send(i,p->data,sizeof(proc_data),0);

	      p->data->REQUEST = CONNECT;

	      break;
	    case FAILEDTOGET:

	      p->data = receivedProc;
	      p->queued = 1;

	      break;
	    case CUDAMALLOC:

	      printf("point 9\n");

	      prevRegion = p->data->mem;

	      printf("point 10\n");

	      p->data = receivedProc;

	      printf("point 11\n");

	      printf("receivedProc->mem : %d\n",receivedProc->mem >> 20);
	      printf("prevRegion        : %d\n",prevRegion >> 20);

	      res = nvmlDeviceGetMemoryInfo(dem.devs[devPos],&mem);

	      printf("point 0\n");

	      if(res != NVML_SUCCESS){
		printf("Failed to get Memory Information\n");
		exit(-1);
	      }

	      printf("mem.free          : %d\n",mem.free >> 20);

	      if(mem.free > receivedProc->mem-prevRegion){

		printf("point 1\n");

		sendProc->REQUEST = CONNECT;

		send(i,sendProc,sizeof(proc_data),0);

		printf("point 2\n");

	      }else{

		printf("point 3\n");

		sendProc->REQUEST = SUSPEND;
		p->queued = 1;

		send(i,sendProc,sizeof(proc_data),0);

		printf("point 4\n");

		dequeue();

		printf("point 5\n");

	      }
	      break;
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
