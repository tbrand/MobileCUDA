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

  printf("FIN(SD:%d) (NUM OF PROCS:%d)\n",dem.procCounter);
  
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

  printf("\tPID : %d\n",p->data->pid);

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

  printf("\tPID : %d\n",p->data->pid);

  size_t _mem;
  size_t _req;
  size_t _sym;
  nvmlReturn_t res;
  nvmlMemory_t mem;
  
  for(i = 0 ; i < dem.ndev ; i ++){
    
    if(!(p->data->flag&CANMIG))
      if(dem.flags[i].stayed)
	continue;

    _mem = p->data->mem;
    _req = p->data->req;
    _sym = p->data->sym;

    res = nvmlDeviceGetMemoryInfo(dem.devs[i],&mem);

#if 0
    printf("mem.free : %lu\n",mem.free);
    printf("reserved : %lu\n",dem.flags[i].reserved);
    printf("_mem     : %lu\n",_mem);
    printf("_req     : %lu\n",_req);
#endif

    if(p->data->pos == i){

      if(mem.free > _req + dem.flags[i].reserved + M64){

	printf("\tGOAHEAD(%d)\n",i);

	if(!(p->data->flag&CANMIG))
	  dem.flags[i].stayed = 1;
	
	dem.flags[i].reserved += _req;
	
	MSEND(sd,CONNECT,0,0,i,0,0);

	return ;
	
      }
      
    }else{
      
      if(mem.free > _mem + dem.flags[i].reserved + M64){
	
	printf("\tGOAHEAD(%d)*\n",i);
	
	if(!(p->data->flag&CANMIG))
	  dem.flags[i].stayed = 1;

	dem.flags[p->data->pos].reserved -= ( _mem - _req );
	dem.flags[i].reserved += _mem;

	p->data->pos = i;

	MSEND(sd,CONNECT,0,0,i,0,0);

	return ;
	
      }
    }
  }

  printf("\tOOPS\n");
  printf("mem.free : %lu\n",mem.free);
  printf("_req     : %lu\n",_req);

  if(!(p->data->flag&CANMIG)){

    dem.staying_procs ++;
    
    p->queued = STAYED_QUEUED;
    
    printf("Queued staying procs[%d]\n",dem.staying_procs);
    
  }else{

    p->queued = QUEUED;

    dem.flags[p->data->pos].reserved -= ( _mem -_req );
    
  }
}

void _RENEW(int sd,proc_data* data){

  printf("RENEW(%d)\n",sd);

  proc* p;

  p = get_proc(sd);

  printf("\tPID : %d\n",p->data->pid);

  memcpy(p->data,data,sizeof(proc_data));

  cons_renew(p);
}

void _MIGDONE(int sd,proc_data* data){

  proc* p;
  int devPos;

  devPos = data->pos;

  printf("MIGDONE(%d) (%d)\n",sd,devPos);

  p = get_proc(sd);

  printf("\tPID : %d\n",p->data->pid);

  memcpy(p->data,data,sizeof(proc_data));

  if(p->created_context){

    dem.flags[devPos].reserved -= p->data->mem;

  }

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

  printf("\tPID : %d\n",p->data->pid);

  send(sd,p->data,sizeof(proc_data),0);

  p->data->REQUEST = CONNECT;
}

void _FAILEDTOALLOC(int sd,proc_data* data){

  printf("FAILEDTOALLOC(%d)\n",sd);

  proc* p;
  int i;

  p = get_proc(sd);

  printf("\tPID : %d\n",p->data->pid);

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

  printf("\tPID : %d\n",p->data->pid);

  memcpy(p->data,data,sizeof(proc_data));

  devp = p->data->pos;

  if(!p->suspended){

    dem.flags[devp].reserved -= p->data->req;

  }else{

    p->suspended = 0;

  }

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
  printf("\tPID : %d\n",p->data->pid);

  res = nvmlDeviceGetMemoryInfo(dem.devs[devp],&mem);

  if(res != NVML_SUCCESS){
    printf("Failed to get Memory Information\n");
    exit(-1);
  }

  if(mem.free > p->data->req + M64 + dem.flags[devp].reserved){

    printf("\tGOAHEAD (REQ : %lu[MB])\n",p->data->req >> 20);

    MSEND(sd,GOAHEAD,0,0,0,0,0);

    dem.flags[devp].reserved += p->data->req;
    
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

      p->queued = BACKUP;
      p->suspended = 1;
      //      p->data->mem += p->data->req;
      //      p->data->req = 0;

      MSEND(sd,SUSPEND,0,0,0,0,0);
      
    }else{
      
      printf("\tOOPS(BUT THIS PROC CANNOT MOVE)\n");

      p->queued = STAYED;
      p->staying_pos = devp;
      //      p->data->mem += p->data->req;
      //      p->data->req = 0;

    }
  }

  cons_renew(p);
}

void _BACKUPED(int sd,proc_data* data){

  printf("BACKUPED(%d)\n",sd);

  proc* p;

  p = get_proc(sd);

  printf("\tPID : %d\n",p->data->pid);
  
  memcpy(p->data,data,sizeof(proc_data));

  p->queued = QUEUED;

  dequeueSpecifyDevNO(p->data->pos);
  dequeueSpecifyProc(p);

  cons_renew(p);
}

void _CONTEXT_CHECK(int sd,proc_data* data){

  printf("CONTEXT CHECK(%d)\n",sd);

  proc* p;
  int find = 0,infoCount,i;
  nvmlProcessInfo_t* infos;
  nvmlReturn_t res;

  p = get_proc(sd);

  printf("\tPID : %d\n",p->data->pid);

  memcpy(p->data,data,sizeof(proc_data));
  
  for(i = 0 ; i < dem.ndev ; i ++){
    
    infoCount = 64;
    infos = (nvmlProcessInfo_t*)malloc(sizeof(nvmlProcessInfo_t)*infoCount);

    res = nvmlDeviceGetComputeRunningProcesses(dem.devs[i],&infoCount,infos);

    if(res != NVML_SUCCESS){
      printf("Failed to get proc info\n");
      exit(-1);
    }

    int j;

    for(j = 0 ; j < infoCount ; j ++){

      if(infos[j].pid == p->data->pid){
	find = 1;
      }      
    }
  }

  if(find){

    printf("FOUND\n");

    MSEND(p->sd,CCHECK_OK,0,0,0,0,0);

    dem.flags[p->data->pos].reserved -= p->data->sym;

    p->created_context = 1;

  }else{

    printf("NOT FOUND\n");

    MSEND(p->sd,CCHECK_FAILED,0,0,0,0,0);

  }
}

void _CREATE_CONTEXT(int sd){

  printf("CREATE CONTEXT\n");

  proc* p;

  p = get_proc(sd);
  p->created_context = 1;

  printf("\tPID : %d\n",p->data->pid);

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

