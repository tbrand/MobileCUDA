#include <dem.h>

FILE* fp = NULL;

int isDaemon = 0;

int end_server = FALSE;

DEM dem;

void _end_server(int sig){
  if(sig == SIGINT)
    end_server = TRUE;
}

void mocu_check(){}

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
