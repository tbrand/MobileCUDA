#include <dem.h>

unsigned long launchPos = 0;

void _FIN(int sd){

  if(!has_proc_sd(sd)){

    cons* c;

    c = get_cons(sd);

    remove_cons(c);

    printf("CONS LEAVE\n");

    return;
  }

  dem.procCounter--;

  printf("FIN(%d) (NUM OF PROCS:%d)\n",sd,dem.procCounter);
  
  proc *p;
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

  if(!(p->data->flag&CANMIG)){

    printf("Staying proc @ %d finished.\n",devNum);

    dem.flags[devNum].stayed = 0;

    if(dem.staying_procs > 0){

      proc* stayp;

      stayp = (proc*)staying_proc();

      if(stayp == NULL){
	printf("oops ... failed to find staying proc...\n");
	exit(-1);
      }

      dem.staying_procs --;

      stayp->queued = ACTIVE;

      MSEND(stayp->sd,CONNECT,0,0,devNum,0,0);

      dem.flags[devNum].stayed = 1;
      
    }else{

      dequeueSpecifyDevNO(devNum);

    }
	
  }else{

    if(dem.flags[devNum].stayed){

      proc* ps;

      ps = (proc*)get_proc_staying_pos(devNum);

      if(ps == NULL){

	printf("PROC (CANNOT MIGRATE) IS ACTIVE NOW(%d)\n",devNum);

#if 0
	dequeueSpecifyDevNO(devNum);
#endif

      }else{

	nvmlReturn_t res;
	nvmlMemory_t mem;

	res = nvmlDeviceGetMemoryInfo(dem.devs[devNum],&mem);
      
	if(res != NVML_SUCCESS){
	  printf("Failed to get Memory Information\n");
	  exit(-1);
	}
      
	printf("USED : %u\n",ps->data->mem);
	printf("FREE : %u\n",mem.free);

	if(mem.free > ps->data->req + M64 + dem.flags[devNum].reserved){

	  MSEND(ps->sd,GOAHEAD,0,0,devNum,0,0);

	  ps->queued = ACTIVE;

	}
#if 0
	else{
	
	  dequeueSpecifyDevNO(devNum);
	
	}
#endif

      }

    }else{

      if(dem.staying_procs > 0){

	proc* ps;

	ps = staying_proc();

	if(ps == NULL){
	  printf("What ...? 1\n");
	  exit(-1);
	}

	MSEND(ps->sd,CONNECT,0,0,devNum,0,0);

	dem.staying_procs --;

	dem.flags[devNum].stayed = 1;

	ps->queued = ACTIVE;
	
      }else{

	dequeueSpecifyDevNO(devNum);

      }
    }
  }

  cons_remove(p);
    
  remove_proc(p);
}

void _CONNECT(int sd,proc_data* data){

  printf("CONNECT(%d) (QUEUESIZE   :%d)\n",sd,queue_size());
  printf("            (NUM OF PROCS:%d)\n",dem.procCounter);

  proc* p;
  int i;
  
  if(data->pos == -1){

    dem.procCounter++;
    
    p = create_proc(sd);
    
    add_proc(p);

    p->data = (proc_data*)malloc(sizeof(proc_data));
    memcpy(p->data,data,sizeof(proc_data));

    if(dem.procCounter > MAXPROC){
      
      if(!(p->data->flag&CANMIG)){

	dem.staying_procs ++;
	
	p->queued = STAYED_QUEUED;
	
      }else{

	p->queued = QUEUED;
	
      }

      return;

    }

  }else{

    p = get_proc(sd);
    memcpy(p->data,data,sizeof(proc_data));

  }

  for(i = 0 ; i < dem.ndev ; i ++){
    
    if(!(p->data->flag&CANMIG))
      if(dem.flags[i].stayed)
	continue;

    size_t _mem;
    size_t _req;
    nvmlReturn_t res;
    nvmlMemory_t mem;

    _mem = p->data->mem;
    _req = p->data->req;

    res = nvmlDeviceGetMemoryInfo(dem.devs[i],&mem);

    printf("mem.free : %lu\n",mem.free);
    printf("_mem : %lu\n",_mem);
    printf("_req : %lu\n",_req);

    if(mem.free > _mem + _req + dem.flags[i].reserved + M64){

      if(!(p->data->flag&CANMIG))
	dem.flags[i].stayed = 1;

      MSEND(sd,CONNECT,0,0,i,0,0);

      p->data->pos = i;
      printf("\t\t\tDEV POS : %d\n",i);

      return;

    }
  }

  if(!(p->data->flag&CANMIG)){

    dem.staying_procs ++;
    
    p->queued = STAYED_QUEUED;
    
    printf("Queued staying procs[%d]\n",dem.staying_procs);
    
  }else{
    
    p->queued = QUEUED;
    
  }
}

void _RENEW(int sd,proc_data* data){

  printf("RENEW(%d)\n",sd);

  proc* p;

  p = get_proc(sd);

  memcpy(p->data,data,sizeof(proc_data));

  cons_renew(p);
}

void _MIGDONE(int sd,proc_data* data){

  proc* p;
  int devPos;

  devPos = data->pos;

  printf("MIGDONE(%d) (%d)\n",sd,devPos);

  p = get_proc(sd);

  memcpy(p->data,data,sizeof(proc_data));

  dem.flags[devPos].reserved -= p->data->mem;

  if(p->data->flag&CANMIG){
	      
    if(dem.flags[devPos].sd == sd){

      printf("\tSUCCESS TO MIGRATE\n");

    }else{

      printf("\tFAILED TO MIGRATE(GET INCORRECT INFORMATION)\n");
      printf("\tTHE RECEIVED  DATA IS %d(SD)\n",sd);
      printf("\tTHE REGISTERD DATA IS %d(FLAG)\n",dem.flags[devPos].sd);

    }

    dem.flags[devPos].sd = -1;
    dem.flags[devPos].flag = 0;

    dequeueSpecifyDevNO(devPos);
  
  }else{

    printf("PROC(CANMIG==NO) STARTED\n");

    dem.flags[devPos].sd = -1;
    dem.flags[devPos].flag = 0;

  }

  cons_renew(p);

}

void _CANRECEIVE(int sd,proc_data* data){

  printf("CANRECEIVE(%d)\n",sd);

  proc* p;

  p = get_proc(sd);

  send(sd,p->data,sizeof(proc_data),0);

  p->data->REQUEST = CONNECT;
}

void _FAILEDTOGET(int sd,proc_data* data){

  printf("FAILEDTOGET(%d)\n",sd);

  proc* p;
  int i;

  p = get_proc(sd);

  memcpy(p->data,data,sizeof(proc_data));
  p->queued = BACKUP;

  for(i = 0 ; i < dem.ndev ; i ++){
    if(dem.flags[i].sd == p->sd){
      printf("Failed to MIGRATE!!!!!!!!!!!!!!\n");
      dem.flags[i].flag = 0;
      dem.flags[i].sd = -1;
    }
  }

  dequeueSpecifyProc(p);

  cons_renew(p);
}

void _MALLOCDONE(int sd,proc_data* data){

  printf("MALLOCDONE(%d)\n",sd);
  
  proc* p;
  int devp;

  p = get_proc(sd);

  memcpy(p->data,data,sizeof(proc_data));

  devp = p->data->pos;

  dem.flags[devp].reserved -= p->data->req;

  printf("\tRESERVED[%d] : %lu[byte]\n",devp,dem.flags[devp].reserved);

  p->data->mem += p->data->req;
  p->data->req = 0;
}

void _CUDAMALLOC(int sd,proc_data* data){

  printf("CUDAMALLOC(%d)\n",sd);

  proc* p;
  nvmlReturn_t res;
  nvmlMemory_t mem;
  int devp;

  p = get_proc(sd);
  memcpy(p->data,data,sizeof(proc_data));

  devp = data->pos;

  printf("DEVPOS : %d\n",data->pos);

  res = nvmlDeviceGetMemoryInfo(dem.devs[devp],&mem);

  if(res != NVML_SUCCESS){
    printf("Failed to get Memory Information\n");
    exit(-1);
  }

  if(mem.free > p->data->req + M64 + dem.flags[devp].reserved){

    printf("\tGOAHEAD\n");

    MSEND(sd,GOAHEAD,0,0,0,0,0);

    dem.flags[devp].reserved += p->data->req;
    
    printf("\tRESERVED[%d] : %lu[byte]\n",devp,dem.flags[devp].reserved);

  }else{

    if(p->data->flag&CANMIG){
      
      printf("\tOOPS(POS:%d)\n",devp);
#if 0
      printf("\t\tFREE : %lu\n",mem.free);
      printf("\t\tUSED : %lu\n",mem.used);
      printf("\t\tTOTAL: %lu\n",mem.total);
      printf("\t\tWANT : %lu\n",data->req);
#endif

      printf("SUSPEND (%d) from %d\n",p->sd,devp);

      //      p->queued = QUEUED;
      p->queued = BACKUP;

      MSEND(sd,SUSPEND,0,0,0,0,0);
      
    }else{
      
      printf("\tOOPS(BUT THIS PROC CANNOT MOVE)\n");

      p->queued = STAYED;
      p->staying_pos = devp;

    }
  }

  cons_renew(p);
}

void _BACKUPED(int sd,proc_data* data){

  printf("BACKUPED(%d)\n",sd);

  proc* p;

  p = get_proc(sd);
  
  memcpy(p->data,data,sizeof(proc_data));

  p->queued = QUEUED;

  dequeueSpecifyDevNO(p->data->pos);
  dequeueSpecifyProc(p);

  cons_renew(p);
}

void _CONSOLE(int sd){

  printf("CONSOLE JOIN\n");

  cons* c;
  proc* p;
  proc* sendp;
  proc_data* sendd;
  int pnum;

  c = create_cons(sd);

  add_cons(c);

  pnum = 0;

  p = dem.p0->next;
  while(p->next != NULL){
    pnum++;
    p = p->next;
  }

  send(sd,&pnum,sizeof(int),0);

  sendp = (proc*)malloc(sizeof(proc)*pnum);
  p = dem.p0->next;
  pnum = 0;

  while(p->next != NULL){
    sendp[pnum++] = *p;
    p = p->next;
  }

  send(sd,sendp,sizeof(proc)*pnum,0);

  sendd = (proc_data*)malloc(sizeof(proc_data)*pnum);
  p = dem.p0->next;
  pnum = 0;

  while(p->next != NULL){
    sendd[pnum++] = *p->data;
    p = p->next;
  }

  send(sd,sendd,sizeof(proc_data)*pnum,0);

  print_cons();

}

void MSEND(int sd,int REQUEST,pid_t pid,int flag,int pos,size_t mem,size_t req){
  proc_data* data;

  data = (proc_data*)malloc(sizeof(proc_data));
  data->REQUEST = REQUEST;
  data->pid = pid;
  data->flag = flag;
  data->pos = pos;
  data->mem = mem;
  data->req = req;

  send(sd,data,sizeof(proc_data),0);

  free(data);
}
