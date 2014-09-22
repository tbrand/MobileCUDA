#include <dem.h>

unsigned long launchPos = 0;

void _FIN(int sd){

#if 0
  if(!has_proc_sd(sd)){

    cons* c;

    c = get_cons(sd);

    remove_cons(c);

    printf("CONS LEAVE\n");

    return;
  }
#endif

  dem.procCounter--;

  printf("FIN(SD:%d) (NUM OF PROCS:%d)\n",dem.procCounter);

  proc *p;
  int devPos,i,flag;

  for(i = 0 ; i < dem.ndev ; i ++){
    if(dem.flags[i].sd == sd){
      printf("\tFIND FAILED PROC(SD:%d)\n",sd);
      dem.flags[i].sd   = 0;
    }
  }

  p = get_proc(sd);

  devPos = p->data->pos;

  flag = p->data->flag;

  printf("\tPID : %d\n",p->data->pid);

  p->queued = FIN;

  TIME_STAMP(p);

  if(dem.flags[devPos].exclusive)exclusive_check(devPos);

  if(!p->created_context){
    dem.flags[p->data->pos].reserved -= (M64 + p->data->sym);
    dem.flags[p->data->pos].context--;
  }

  remove_proc(p);

  if(flag&CANNOTMIG){

    printf("\tStaying proc @ %d finished.\n",devPos);
    printf("\tRemains staying procs == %d\n",dem.staying_procs);

    dem.flags[devPos].stayed = 0;

    if(num_of_staying_exclusive_procs() > 0){

      proc* excp;

      excp = staying_exclusive_proc();

      if(excp == NULL){
	printf("Failed to find staying exclusive-proc\n");
	exit(-1);
      }

      dem.staying_procs --;

      dem.flags[devPos].stayed = 1;

      dem.flags[devPos].exclusive = 1;

      dem.flags[devPos].reserved += M64;

      excp->queued = EXC_READY;

      excp->data->pos = devPos;

      exclusive_check(devPos);

      TIME_STAMP(excp);

      return;

    }else if(dem.staying_procs > 0){

      proc* stayp;

      stayp = (proc*)staying_proc();

      if(stayp == NULL){
	printf("oops ... failed to find staying proc...\n");
	exit(-1);
      }

      dem.staying_procs --;

      stayp->queued = ACTIVE;

      stayp->data->pos = devPos;

      MSEND(stayp->sd,CONNECT,0,0,devPos,0,0);

      dem.flags[devPos].stayed = 1;

      dem.flags[devPos].reserved += M64;

      dem.flags[devPos].context ++;

      TIME_STAMP(stayp);

      return;

    }else{

      dequeueSpecifyDevNO(devPos);

    }

  }else{

    if(dem.flags[devPos].stayed){

      if(has_staying_cannot_move_proc(devPos)){

	proc* ps;

	ps = (proc*)get_proc_staying_pos(devPos);

	if(ps == NULL){

	  printf("Failed to find active staying proc on device %d\n",devPos);
	  exit(-1);

	}

	nvmlReturn_t res;
	nvmlMemory_t mem;

	res = nvmlDeviceGetMemoryInfo(dem.devs[devPos],&mem);

	if(res != NVML_SUCCESS){
	  printf("Failed to get Memory Information\n");
	  exit(-1);
	}

	printf("\tUSED : %u\n",ps->data->mem);
	printf("\tFREE : %u\n",mem.free);

	if(ps->created_context){

	  if(mem.free > ps->data->req + M64 + dem.flags[devPos].reserved){

	    dem.flags[devPos].reserved += ps->data->req;

	    MSEND(ps->sd,GOAHEAD,0,0,devPos,0,0);

	    ps->queued = ACTIVE;

	    TIME_STAMP(ps);

	    return;

	  }

	}else{

	  if(mem.free > ps->data->req + ps->data->sym + M64 + dem.flags[devPos].reserved){

	    dem.flags[devPos].reserved += ps->data->req + ps->data->sym;

	    MSEND(ps->sd,GOAHEAD,0,0,devPos,0,0);

	    ps->queued = ACTIVE;

	    TIME_STAMP(ps);

	    return;

	  } 
	}
      }

    }else{

      if(dem.staying_procs > 0){

	proc* ps;

	if(num_of_staying_exclusive_procs() > 0){

	  ps = staying_exclusive_proc();

	  if(ps == NULL){
	    printf("Failed to find staying exclusive proc\n");
	    exit(-1);
	  }

	  dem.flags[devPos].reserved += ps->data->sym + M64;

	  dem.flags[devPos].stayed = 1;

	  dem.flags[devPos].exclusive = 1;

	  dem.staying_procs --;

	  ps->queued = EXC_READY;

	  ps->data->pos = devPos;

	  exclusive_check(devPos);

	  TIME_STAMP(ps);

	}else{

	  ps = staying_proc();

	  if(ps == NULL){
	    printf("Failed to find staying proc(not exclusive)\n");
	    exit(-1);
	  }

	  dem.flags[devPos].reserved += ps->data->sym + M64;

	  MSEND(ps->sd,CONNECT,0,0,devPos,0,0);

	  dem.staying_procs --;

	  dem.flags[devPos].stayed = 1;

	  dem.flags[devPos].context ++;

	  ps->queued = ACTIVE;

	  ps->data->pos = devPos;

	  TIME_STAMP(ps);

	}
	
      }else{

	dequeueSpecifyDevNO(devPos);

      }
    }
  }
}

void _CONNECT(int sd,proc_data* data){

  printf("CONNECT(%d) (QUEUESIZE   :%d)\n",sd,queue_size());
  printf("            (NUM OF PROCS:%d)\n",dem.procCounter);

  proc* p;
  int i;
  size_t prev_sym;
  
  if(data->pos == -1){

    dem.procCounter++;
    
    p = create_proc(sd);
    
    add_proc(p);

    p->data = (proc_data*)malloc(sizeof(proc_data));

    memcpy(p->data,data,sizeof(proc_data));

    if(dem.procCounter > MAXPROC){
      
      printf("\tOver MAXPROCS limit\n");
      
      if(p->data->flag&CANNOTMIG){

	if(!(p->data->flag&EXCLUSIVE)){

	  dem.staying_procs ++;
	  
	  p->queued = STAYED_QUEUED;
	  
	  printf("\tQueued cannot-move proc(%d)\n",dem.staying_procs);

	  TIME_STAMP(p);

	  return;
	  
	}
	
      }else{

	p->queued = QUEUED;

	printf("\tQueued proc(%d)\n",queue_size());

	TIME_STAMP(p);

	return;
	
      }
    }

    prev_sym = 0;

  }else{

    p = get_proc(sd);

    prev_sym = p->data->sym;

    memcpy(p->data,data,sizeof(proc_data));

  }

  printf("\tPID : %d\n",p->data->pid);

  nvmlReturn_t res;
  nvmlMemory_t mem;

  if(p->data->pos >= 0)
    dem.flags[p->data->pos].reserved -= prev_sym;

  if(!p->created_context){
    dem.flags[p->data->pos].reserved -= M64;
    dem.flags[p->data->pos].context --;
  }

  int *devopt = opt_devs();
  
  for(i = 0 ; i < dem.ndev ; i ++){

#if 1
    if(dem.flags[devopt[i]].stayed)
      continue;

    if(dem.flags[devopt[i]].context >= CONTEXT_NUM && (!p->data->flag&EXCLUSIVE))
      continue;
    
    res = nvmlDeviceGetMemoryInfo(dem.devs[devopt[i]],&mem);

    if(res != NVML_SUCCESS){
      printf("Failed to get memory info(%d)\n",res);
      exit(-1);
    }

    if(mem.free > p->data->sym + dem.flags[devopt[i]].reserved + M64){

      if(p->data->flag&CANNOTMIG){

	if(p->data->flag&EXCLUSIVE){

	  printf("\tENTERING EXCLUSIVE MODE!!!\n");

	  dem.flags[devopt[i]].reserved += p->data->sym + M64;

	  dem.flags[devopt[i]].stayed = 1;

	  dem.flags[devopt[i]].exclusive = 1;

	  p->queued = EXC_READY;

	  p->data->pos = devopt[i];

	  exclusive_check(devopt[i]);

	  TIME_STAMP(p);

	  return;
	    
	}else{

	  printf("\tGOAHEAD(%d)\n",devopt[i]);
	    
	  dem.flags[devopt[i]].reserved += p->data->sym + M64;
	    
	  dem.flags[devopt[i]].stayed = 1;

	  dem.flags[devopt[i]].context ++;

	  p->queued = ACTIVE;

	  p->data->pos = devopt[i];

	  MSEND(sd,CONNECT,0,0,devopt[i],0,0);

	  TIME_STAMP(p);
	    
	  return; 
	    
	}

      }else{

	printf("\tGOAHEAD(%d)\n",devopt[i]);

	dem.flags[devopt[i]].reserved += p->data->sym + M64;

	dem.flags[devopt[i]].context ++;

	p->queued = ACTIVE;

	p->data->pos = devopt[i];

	MSEND(sd,CONNECT,0,0,devopt[i],0,0);

	TIME_STAMP(p);

	return;

      }
    }
#else    
    if(dem.flags[i].stayed)
      continue;

    if(dem.flags[i].context >= CONTEXT_NUM)
      continue;
    
    res = nvmlDeviceGetMemoryInfo(dem.devs[i],&mem);

    if(res != NVML_SUCCESS){
      printf("Failed to get memory info(%d)\n",res);
      exit(-1);
    }

    if(mem.free > p->data->sym + dem.flags[i].reserved + M64){

      if(p->data->flag&CANNOTMIG){

	if(p->data->flag&EXCLUSIVE){

	  printf("\tENTERING EXCLUSIVE MODE!!!\n");

	  dem.flags[i].reserved += p->data->sym + M64;

	  dem.flags[i].stayed = 1;

	  dem.flags[i].exclusive = 1;

	  p->queued = EXC_READY;

	  p->data->pos = i;

	  exclusive_check(i);

	  TIME_STAMP(p);

	  return;
	    
	}else{

	  printf("\tGOAHEAD(%d)\n",i);
	    
	  dem.flags[i].reserved += p->data->sym + M64;
	    
	  dem.flags[i].stayed = 1;

	  dem.flags[i].context ++;

	  p->queued = ACTIVE;

	  p->data->pos = i;

	  MSEND(sd,CONNECT,0,0,i,0,0);

	  TIME_STAMP(p);
	    
	  return; 
	    
	}

      }else{

	printf("\tGOAHEAD(%d)\n",i);

	dem.flags[i].reserved += p->data->sym + M64;

	dem.flags[i].context ++;

	p->queued = ACTIVE;

	p->data->pos = i;

	MSEND(sd,CONNECT,0,0,i,0,0);

	TIME_STAMP(p);

	return;

      }
    }
#endif
  }

  printf("\tOOPS\n");

  if(p->data->flag&CANNOTMIG){

    dem.staying_procs ++;
    
    p->queued = STAYED_QUEUED;
    
    printf("\tQueued staying procs[%d]\n",dem.staying_procs);

    TIME_STAMP(p);
    
  }else{

    p->queued = QUEUED;

    TIME_STAMP(p);
    
  }
}

void _RENEW(int sd,proc_data* data){

#if 0

  printf("RENEW(%d)\n",sd);

  proc* p;

  p = get_proc(sd);

  printf("\tPID : %d\n",p->data->pid);

  memcpy(p->data,data,sizeof(proc_data));

  //  cons_renew(p);

#endif

}

void _MIGDONE(int sd,proc_data* data){

  proc* p;
  int devPos;

  devPos = data->pos;

  printf("MIGDONE(%d) (%d)\n",sd,devPos);

  p = get_proc(sd);

  printf("\tPID : %d\n",p->data->pid);

  memcpy(p->data,data,sizeof(proc_data));

  dem.flags[devPos].reserved -= p->data->mem;

  if(p->data->flag&CANNOTMIG){

    printf("PROC(CANMIG==NO) STARTED\n");

    dem.flags[devPos].sd = -1;
    
  }else{

    if(dem.flags[devPos].sd == sd){

      printf("\tSUCCESS TO MIGRATE\n");

    }else{

      printf("\tFAILED TO MIGRATE(GET INCORRECT INFORMATION)\n");
      printf("\tTHE RECEIVED  DATA IS %d(SD)\n",sd);
      printf("\tTHE REGISTERD DATA IS %d(FLAG)\n",dem.flags[devPos].sd);

    }

    dem.flags[devPos].sd = -1;

    dequeueSpecifyDevNO(devPos);
  }
}

void _CANRECEIVE(int sd,proc_data* data){

  printf("CANRECEIVE(%d) : PID %d\n",sd,data->pid);

  proc* p;

  p = get_proc(sd);

  MSEND(sd,SUSPEND,0,0,0,0,0);

  p->queued = BACKUP;

  if(!p->created_context){
    dem.flags[p->data->pos].reserved -= M64;
    dem.flags[p->data->pos].context--;
  }

  TIME_STAMP(p);

}

void _FAILEDTOALLOC(int sd,proc_data* data){

  printf("FAILEDTOALLOC(%d)\n",sd);

  proc* p;
  int i;

  p = get_proc(sd);

  printf("\tPID : %d\n",p->data->pid);

  memcpy(p->data,data,sizeof(proc_data));
  p->queued = BACKUP;

  TIME_STAMP(p);

  for(i = 0 ; i < dem.ndev ; i ++){
    if(dem.flags[i].sd == p->sd){
      printf("Failed to MIGRATE!!!!!!!!!!!!!!\n");
      dem.flags[i].sd = -1;
    }
  }

#if 1
  printf("REQ : %lu\n",data->req);
  printf("SYM : %lu\n",data->sym);
  printf("MEM : %lu\n",data->mem);
  printf("POS : %lu\n",data->pos);

  nvmlMemory_t mem;

  nvmlDeviceGetMemoryInfo(dem.devs[data->pos],&mem);

  printf("FREE : %lu\n",mem.free);
#endif

  dequeueSpecifyProc(p);

}

void _MALLOCDONE(int sd,proc_data* data){

  printf("MALLOCDONE(%d)\n",sd);
  
  proc* p;
  int devp;

  p = get_proc(sd);

  printf("\tPID : %d\n",p->data->pid);

  memcpy(p->data,data,sizeof(proc_data));

  devp = p->data->pos;

  dem.flags[devp].reserved -= p->data->req;

  dequeueSpecifyDevNO(p->data->pos);
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

  if(dem.flags[devp].exclusive){

    if(!p->created_context){
      dem.flags[devp].reserved -= M64;
      dem.flags[devp].context --;
    }

    MSEND(sd,SUSPEND,0,0,0,0,0);
    
    p->queued = BACKUP;

    TIME_STAMP(p);

    return;
  }

  if(p->created_context){
    
    if(mem.free > p->data->req + M64 + dem.flags[devp].reserved){
      
      printf("\tGOAHEAD (REQ : %lu[MB])\n",p->data->req >> 20);
      
      MSEND(sd,GOAHEAD,0,0,0,0,0);
      
      dem.flags[devp].reserved += p->data->req;

      return;
      
    }
    
  }else{

    if(mem.free > p->data->req + p->data->sym + M64 + dem.flags[devp].reserved){
      
      printf("\tGOAHEAD (REQ : %lu[MB])\n",p->data->req >> 20);
      
      MSEND(sd,GOAHEAD,0,0,0,0,0);
      
      dem.flags[devp].reserved += p->data->req + p->data->sym;

      return;
      
    }
    
  }

  if(p->data->flag&CANNOTMIG){

    printf("\tOOPS(BUT THIS PROC CANNOT MOVE)\n");

    p->queued = STAYED;
    p->staying_pos = devp;

    TIME_STAMP(p);
      
  }else{

    printf("\tOOPS(POS:%d)\n",devp);
    printf("SUSPEND (%d) from %d\n",p->sd,devp);

    if(!p->created_context){
      dem.flags[devp].reserved -= M64;
      dem.flags[devp].context--;
    }

    p->queued = BACKUP;

    MSEND(sd,SUSPEND,0,0,0,0,0);

    TIME_STAMP(p);
  }

}

void _CUDAFREE(int sd,proc_data* data){
  
  printf("CUDAFREE(%d)\n",sd);

  proc* p;

  p = get_proc(sd);

  memcpy(p->data,data,sizeof(proc_data));
  
}

void _BACKUPED(int sd,proc_data* data){

  printf("BACKUPED(%d)\n",sd);

  proc* p;

  p = get_proc(sd);

  if(p == NULL){
    printf("Failed to find a proc which has sd %d\n",sd);
    exit(-1);
  }

  printf("\tPID : %d\n",p->data->pid);
  printf("\tREQ : %lu\n",p->data->req);
  printf("\tMEM : %lu\n",p->data->mem);
  
  memcpy(p->data,data,sizeof(proc_data));

  p->queued = QUEUED;

  TIME_STAMP(p);

  if(dem.flags[p->data->pos].exclusive){

    exclusive_check(p->data->pos);
    
  }else{

    dequeueSpecifyDevNO(p->data->pos);
    dequeueSpecifyProc(p);
    
  }

}

void _CONTEXT_CHECK(int sd,proc_data* data){

  printf("CONTEXT CHECK(%d)\n",sd);

  proc* p;
  int find = 0,infoCount;
  nvmlProcessInfo_t* infos;
  nvmlReturn_t res;

  p = get_proc(sd);

  printf("\tPID : %d(Checking : %d)\n",p->data->pid,p->data->pos);

  memcpy(p->data,data,sizeof(proc_data));

  infoCount = 100;
  infos = (nvmlProcessInfo_t*)malloc(sizeof(nvmlProcessInfo_t)*infoCount);

  res = nvmlDeviceGetComputeRunningProcesses(dem.devs[p->data->pos],&infoCount,infos);

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

  if(find){

    printf("\tFOUND\n");

    MSEND(p->sd,CCHECK_OK,0,0,0,0,0);

    dem.flags[p->data->pos].reserved -= (p->data->sym+M64);
    dem.flags[p->data->pos].context --;

    p->data->mem += p->data->sym;
    p->data->sym = 0;

    p->created_context = 1;

    register_name(sd);

    TIME_STAMP(p);

  }else{

    printf("\tNOT FOUND\n");

    MSEND(p->sd,CCHECK_FAILED,0,0,0,0,0);

  }
}

void _PROFILE(int sd, proc_data* data){

  printf("PROFILE(%d)\n",sd);

  proc* p;
  uint64_t inst,gld,gst;

  inst = data->trace.inst;
  gld  = data->trace.gld;
  gst  = data->trace.gst;

  printf("inst:%lu\tgld:%lu\tgst:%lu\n",inst,gld,gst);

  float _inst,_gld,_gst;

  _inst = (float)inst/(float)inst;
  _gld  = (float)gld/(float)inst;
  _gst  = (float)gst/(float)inst;

  printf("_inst:%f\t_gld:%f\t_gst:%f\n",_inst,_gld,_gst);

  p = get_proc(sd);

  p->_inst = _inst;
  p->_gld  = _gld;
  p->_gst  = _gst;

  MSEND(sd,GOAHEAD,0,0,data->pos,0,0);

}

void _CONSOLE(int sd){

#if 0
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

#endif

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

