#include <dem.h>

unsigned long launchPos = 0;

void _FIN(int sd){

  dem.procCounter--;

  printf("FIN(NUM OF PROCS:%d)\n",dem.procCounter);
  
  proc* p;
  int devNum,i;

  for(i = 0 ; i < dem.ndev ; i ++){
    if(dem.flags[i].sd == sd){
      printf("\tFIND FAILED PROC(SD:%d)\n",sd);
      dem.flags[i].flag = 0;
      dem.flags[i].sd   = 0;
    }
  }

  p = get_proc(sd);

  devNum = p->data->pos;

  remove_proc(p);

  dequeueSpecifyDevNO(devNum);
  
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

    sendProc->REQUEST = CANNOTENTER;
    p->queued = 1;

  }else{

    sendProc->REQUEST = CONNECT;

    usleep(500000);//1.0[sec]

  }

  send(sd,sendProc,sizeof(proc_data),0);
}

void _RENEW(int sd,proc_data* data){

  printf("RENEW\n");

  proc* p;

  p = get_proc(sd);

  renew_proc(data,sd);
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
    printf("\tTHE RECEIVED  DATA IS %d(SD)\n",sd);
    printf("\tTHE REGISTERD DATA IS %d(FLAG)\n",dem.flags[devPos]);

  }

  dem.flags[devPos].sd = -1;
  dem.flags[devPos].flag = 0;

  renew_proc(data,sd);
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
  int i;

  p = get_proc(sd);

  p->data = data;
  p->queued = 1;

  for(i = 0 ; i < dem.ndev ; i ++){
    if(dem.flags[i].sd == p->sd){
      printf("Failed to MIGRATE!!!!!!!!!!!!!!\n");
      dem.flags[i].flag = 0;
      dem.flags[i].sd = -1;
    }
  }
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

  if(mem.free > data->mem - prevRegion){

    printf("\tGOAHEAD\n");

    sendProc->REQUEST = CONNECT;
    send(sd,sendProc,sizeof(proc_data),0);

  }else{

    printf("\tOOPS\n");

    sendProc->REQUEST = SUSPEND;
    p->queued = 1;

    send(sd,sendProc,sizeof(proc_data),0);

    dequeueSpecifyProc(p);

  }
}
