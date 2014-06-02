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

typedef struct _DEM{
  proc *p0,*p1;
  //  proc *q0,*q1;
  nvmlDevice_t* devs;
  unsigned int ndev;
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

void dequeue(int sd){

  proc* ptemp;
  int devPos = 0;
  int found = 0;
  nvmlReturn_t res;
  nvmlMemory_t mem;

  ptemp = get_proc(sd);

  for(devPos = 0 ; devPos < dem.ndev ; devPos ++){
    
    res = nvmlDeviceGetMemoryInfo(dem.devs[devPos],&mem);

    if(res != NVML_SUCCESS){
      printf("Failed to get Memory Info in dequeue\n");
      exit(-1);
    }

    if(mem.free > ptemp->data->mem){
      found = 1;
      break;
    }
  }

  if(found){

    ptemp->queued = 0;
    ptemp->data->REQUEST = CHANGEPOS;
    ptemp->data->pos = devPos;
    send(ptemp->sd,ptemp->data,sizeof(proc_data),0);

  }
}

void mocu_lock(){
  int res;
  struct sembuf sops;

  sops.sem_num = 0;
  sops.sem_op  = -1;
  sops.sem_flg = SEM_UNDO;
  res = semop(sem_id,&sops,1);
}

void mocu_unlock(){
  int res;
  struct sembuf sops;

  sops.sem_num = 0;
  sops.sem_op  = 1;
  sops.sem_flg = SEM_UNDO;
  res = semop(sem_id,&sops,1);
}

void init_smph(){
  /**Initialize Lock**/
  sem_id = semget(KEY,1,IPC_CREAT|S_IRUSR|S_IWUSR|IPC_EXCL);
  
  if(sem_id == -1){
    sem_id = semget(KEY,1,S_IRUSR|S_IWUSR);
  }else{
    semctl(sem_id,0,SETVAL,1);
  }
}

void _end_server(int sig){
  if(sig == SIGINT)
    end_server = TRUE;
}

void mocu_check(){
  nvmlReturn_t res;
  nvmlMemory_t mems[dem.ndev];
  int i,req = 0;
  proc* ptemp;
  
  for(i = 0 ; i < dem.ndev ; i ++){

    res = nvmlDeviceGetMemoryInfo(dem.devs[i],&mems[i]);

    if(res != NVML_SUCCESS){
      perror("Failed to get memory info...\n");
      exit(-1);
    }
  }

  ptemp = dem.p0->next;
  while(ptemp->next != NULL){
    
    int pos;//is my device position

    pos = ptemp->data->pos;
    for(i = 0 ; i < dem.ndev ; i ++){

      if(i == pos)continue;
      
      long _old;
      long _new;

      _new = mems[i].free   - ptemp->data->mem;
      _old = mems[pos].free + ptemp->data->mem;

      if(_new >= 0){//the device has enough region to accept ptemp's migration

	if(_new > _old){

	  ptemp->data->REQUEST = MIGRATE;
	  ptemp->data->pos = i;

	  kill(ptemp->data->pid,SIGUSR1);

	  req = 1;
	  break;
	}
      }
    }

    if(req)break;

    ptemp = ptemp->next;
  }
}

int sem_id;

int main(int argc,char* argv[]){

  /**Initialize signal**/

  signal(SIGINT ,_end_server);
  signal(SIGUSR1,_end_server);

  init_smph();
  
  /**Initialize struct proc**/
  init_proc();
  //  init_queue();

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

  for(i = 0 ; i < dem.ndev ; i ++){

    nres = nvmlDeviceGetHandleByIndex(i,&dem.devs[i]);

    if(nres != NVML_SUCCESS){
      perror("Failed to get device handle\n");
      exit(-1);
    }
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

  /* timeout.tv_sec = 3*60; */
  /* timeout.tv_usec = 0; */

  /* fprintf(fp,"Success to construct deamon, Entering loop...\n"); */

  /* long counter = 0; */

  /* /\**Entering main loop**\/ */

  /* do{ */

  /*   fprintf(fp,"[Loop(%u)]\n",++counter); */

  /*   memcpy(&working_set,&master_set,sizeof(master_set)); */

  /*   rc = select(max_sd+1, &working_set, NULL, NULL, NULL); */

  /*   if(rc < 0){ */
  /*     perror("select() failed\n"); */
  /*     break; */
  /*   } */

  /*   if(rc == 0){ */
  /*     fprintf(fp,"select() time out. End program.\n"); */
  /*     break; */
  /*   } */

  /*   desc_ready = rc; */

  /*   for(i = 0 ; i < max_sd+1 && desc_ready > 0 ; ++i){ */
      
  /*     if(FD_ISSET(i,&working_set)){ */

  /* 	fprintf(fp,"%d tried to connect to me!!\n",i); */

  /* 	desc_ready = -1; */

  /* 	if(i == listen_sd){ */

  /* 	  fprintf(fp,"Listening socket is readable\n"); */

  /* 	  new_sd = accept(listen_sd,NULL,NULL); */

  /* 	  if(new_sd < 0){ */
  /* 	    fprintf(fp,"accept() failed"); */
  /* 	    end_server = TRUE; */
  /* 	  } */

  /* 	  FD_SET(new_sd,&master_set); */

  /* 	  if(new_sd > max_sd){ */
  /* 	    max_sd = new_sd; */
  /* 	  } */

  /* 	  proc* p = create_proc(new_sd); */

  /* 	  add_proc(p); */

  /* 	}else{ */

  /* 	  fprintf(fp,"Descriptor %d is readable\n",i); */

  /* 	  proc_data* receivedProc = (proc_data*)malloc(sizeof(proc_data)); */
  /* 	  proc_data* sendProc = (proc_data*)malloc(sizeof(proc_data)); */
  /* 	  proc* p = get_proc(i); */

  /* 	  rc = recv(i,receivedProc,sizeof(proc_data),0); */

  /* 	  if(rc <= 0){ */

  /* 	    fprintf(fp,"Connection closed from %d\n",i); */
  /* 	    FD_CLR(i,&master_set); */

  /* 	    remove_proc(p); */

  /* 	    proc* ptemp; */
  /* 	    int found = 0; */

  /* 	    ptemp = dem.p0->next; */

  /* 	    while(ptemp->next != NULL){ */

  /* 	      if(ptemp->queued){ */
  /* 		found = 1; */
  /* 		break; */
  /* 	      } */

  /* 	      ptemp = ptemp->next; */
  /* 	    } */

  /* 	    if(found){ */
	      
  /* 	      printf("Find Queued Proc!!!\n"); */
  /* 	      dequeue(ptemp->sd); */

  /* 	    } */

  /* 	  }else{ */

  /* 	    if(receivedProc->REQUEST == CONNECT){ */

  /* 	      if(queue_size() > 0){ */
  /* 		sendProc->REQUEST = SUSPEND; */
  /* 		p->queued = 1; */
  /* 	      }else{ */
  /* 		sendProc->REQUEST = CONNECT; */
  /* 	      } */

  /* 	      send(i,sendProc,sizeof(proc_data),0); */

  /* 	    }else if(receivedProc->REQUEST == RENEW){ */

  /* 	      renew_proc(receivedProc,i); */

  /* 	    }else if(receivedProc->REQUEST == CANRECEIVE){ */

  /* 	      /\* */
  /* 	      proc* sendData; */

  /* 	      sendData = get_proc(i); */

  /* 	      send(i,sendData->data,sizeof(proc_data),0); */

  /* 	      sendData->data->REQUEST = CONNECT; */
  /* 	      *\/ */

  /* 	      send(i,p->data,sizeof(proc_data),0); */

  /* 	      p->data->REQUEST = CONNECT; */

  /* 	    }else if(receivedProc->REQUEST == FAILEDTOGET){ */

  /* 	      printf("Find Proc witch was failed to allocate memory\n"); */

  /* 	      proc* recievedData; */

  /* 	      recievedData = get_proc(i); */

  /* 	      recievedData->data = receivedProc; */
  /* 	      recievedData->queued = 1; */

  /* 	    }else if(receivedProc->REQUEST == CUDAMALLOC){ */
	      
  /* 	      printf("Find Proc witch try to allocate new memory region\n"); */

  /* 	      proc* tryData; */
  /* 	      int devPos; */
  /* 	      size_t prevRegion; */
  /* 	      nvmlReturn_t res; */
  /* 	      nvmlMemory_t mem; */

  /* 	      tryData = get_proc(i); */

  /* 	      prevRegion = tryData->data->mem; */

  /* 	      tryData->data = receivedProc; */

  /* 	      devPos = receivedProc->pos; */

  /* 	      res = nvmlDeviceGetMemoryInfo(dem.devs[devPos],&mem); */

  /* 	      if(res != NVML_SUCCESS){ */
  /* 		printf("Failed to get Memory Information\n"); */
  /* 		exit(-1); */
  /* 	      } */

  /* 	      if(mem.free > receivedProc->mem-prevRegion){ */

  /* 		printf("\tGoahead!!\n"); */

  /* 		sendProc->REQUEST = CONNECT; */

  /* 		send(i,sendProc,sizeof(proc_data),0); */

  /* 	      }else{ */

  /* 		printf("\tOops.. Wait!!\n"); */

  /* 		int j; */
  /* 		int found = 0; */

  /* 		for(j = 0 ; j < dem.ndev ; j ++){ */
		  
  /* 		  if(j == devPos)continue; */

  /* 		  res = nvmlDeviceGetMemoryInfo(dem.devs[j],&mem); */

  /* 		  if(res != NVML_SUCCESS){ */
  /* 		    printf("Failed to get memory inf...\n"); */
		    exit(-1);
		  }

		  if(mem.free > receivedProc->mem){
		    found = 1;
		    break;
		  }

		}

		if(found){
		  sendProc->REQUEST = MIGRATE;
		  sendProc->pos = j;
		}else{
		  sendProc->REQUEST = SUSPEND;
		  tryData->queued = 1;
		}

		send(i,sendProc,sizeof(proc_data),0);

	      }
	    }
	  }
	}
      }
    }

    //    mocu_check();

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
