#include <mocu.h>
#include <fatBinaryCtl.h>

int sem_id;
int mocu_pos;

MOCU mocu;

static int initialized = 0;

__attribute__((constructor)) void init_mocu(){

  if(initialized)return;

  mocu_load_cuda_lib();

  mocu_register_signal();

  mocu_pos = -1;

  mocu.cp = (context*)malloc(sizeof(context));

  mocu.cp->d0 = (region*)malloc(sizeof(region));
  mocu.cp->d1 = (region*)malloc(sizeof(region));
  mocu.cp->d0->mode = mocu.cp->d1->mode = -1;
  mocu.cp->d0->next = mocu.cp->d1;
  mocu.cp->d1->prev = mocu.cp->d0;
  mocu.cp->d1->next = NULL;
  mocu.cp->d0->prev = NULL;

  mocu.cp->s0 = (mocu_stream*)malloc(sizeof(mocu_stream));
  mocu.cp->s1 = (mocu_stream*)malloc(sizeof(mocu_stream));
  mocu.cp->s0->mode = mocu.cp->s1->mode = -1;
  mocu.cp->s0->next = mocu.cp->s1;
  mocu.cp->s1->prev = mocu.cp->s0;
  mocu.cp->s1->next = NULL;
  mocu.cp->s0->prev = NULL;

  mocu.cp->e0 = (mocu_event*)malloc(sizeof(mocu_event));
  mocu.cp->e1 = (mocu_event*)malloc(sizeof(mocu_event));
  mocu.cp->e0->mode = mocu.cp->e1->mode = -1;
  mocu.cp->e0->next = mocu.cp->e1;
  mocu.cp->e1->prev = mocu.cp->e0;
  mocu.cp->e1->next = NULL;
  mocu.cp->e0->prev = NULL;

  mocu.cp->a0 = (apilog*)malloc(sizeof(apilog));
  mocu.cp->a1 = (apilog*)malloc(sizeof(apilog));
  mocu.cp->a0->next = mocu.cp->a1;
  mocu.cp->a1->prev = mocu.cp->a0;
  mocu.cp->a1->next = NULL;
  mocu.cp->a0->prev = NULL;

  mocu.cp->sm0 = (symbol*)malloc(sizeof(symbol));
  mocu.cp->sm1 = (symbol*)malloc(sizeof(symbol));
  mocu.cp->sm0->next = mocu.cp->sm1;
  mocu.cp->sm1->prev = mocu.cp->sm0;
  mocu.cp->sm1->next = NULL;
  mocu.cp->sm0->prev = NULL;

  mocu.cp->msg = (proc_data*)malloc(sizeof(proc_data));
  mocu.cp->msg->REQUEST = CONNECT;
  mocu.cp->msg->pid = getpid();
  mocu.cp->msg->pos = mocu_pos;
  mocu.cp->msg->flag = 0;
  mocu.cp->msg->mem = 0;
  mocu.cp->msg->req = 0;

  mocu.cp->rmsg = (proc_data*)malloc(sizeof(proc_data));
  mocu.cp->rmsg->REQUEST = CONNECT;
  mocu.cp->rmsg->pid = 0;
  mocu.cp->rmsg->pos = 0;
  mocu.cp->rmsg->flag = 0;
  mocu.cp->rmsg->mem = 0;
  mocu.cp->rmsg->req = 0;

  mocu_connect();

  initialized = 1;

  return;

}



cudaError_t __cudaRegisterDeviceFunction(){

  TRACE("__cudaRegisterDeviceFunction");

  ENTER;


  return mocu.__mocudaRegisterDeviceFunction();

}

void __cudaMutexOperation(int lock){

  TRACE("__cudaMutexOperation");

  ENTER;


  return mocu.__mocudaMutexOperation(lock);

}



void __cudaTextureFetch(const void* tex,void* index,int integer,void* val){

  TRACE("__cudaTextureFetch");

  ENTER;


  return mocu.__mocudaTextureFetch(tex,index,integer,val);

}

int __cudaSynchronizeThreads(void** one,void* two){

  TRACE("__cudaSynchronizeThreads");

  ENTER;


  return mocu.__mocudaSynchronizeThreads(one,two);

}

void __cudaRegisterShared(void** fatCubinHandle,void **devicePtr){

  TRACE("__cudaRegisterShared");

  ENTER;


  return mocu.__mocudaRegisterShared(fatCubinHandle,devicePtr);

}

void __cudaRegisterSharedVar(void** fatCubinHandle,void **devicePtr,size_t size,size_t alignment,int storage){

  TRACE("__cudaRegisterSharedVar");

  ENTER;


  return mocu.__mocudaRegisterSharedVar(fatCubinHandle,devicePtr,size,alignment,storage);

}

void** __cudaRegisterFatBinary(void* fatCubin){

  TRACE("__cudaRegisterFatBinary");

  ENTER;

  if(!initialized)init_mocu();

  void **fHandle = mocu.__mocudaRegisterFatBinary(fatCubin);

#if 0
  printf("fatCubin       : %p\n",fatCubin);
  printf("fatCubinHandle : %p\n",fHandle);
#endif

#if 0
  apilog* a;

  a = (apilog*)malloc(sizeof(apilog));

  a->type = REGISTERFATBINARY;
  a->data.registerFatBinary.fatCubin = fatCubin;
  a->data.registerFatBinary.fatCubinHandle = fHandle;
  a->prev = mocu.cp->a1->prev;
  a->next = mocu.cp->a1;
  a->prev->next = a;
  a->next->prev = a;
#endif

  parse(fatCubin);

  return fHandle;

}

void __cudaUnregisterFatBinary(void** fatCubinHandle){

  TRACE("__cudaUnregisterFatBinary");

  ENTER;

  FINISH;

  return mocu.__mocudaUnregisterFatBinary(fatCubinHandle);

}

void __cudaRegisterVar(void** fatCubinHandle,char *hostVar,char *deviceAddress,const char *deviceName,int ext,int size,int constant,int global){

  TRACE("__cudaRegisterVar");

  ENTER;

  mocu.__mocudaRegisterVar(fatCubinHandle,hostVar,deviceAddress,deviceName,ext,size,constant,global);

#if 0
  printf("\thostVar : %p\n",hostVar);
  printf("\tdeviceAddress : %p\n",deviceAddress);
  printf("\tdeviceName : %s\n",deviceName);
  printf("\text : %d\n",ext);
  printf("\tsize : %d\n",size);
  printf("\tconstant : %d\n",constant);
  printf("\tglobal : %d\n",global);
#endif

  mocu_add_symbol(hostVar,deviceAddress,(char*)deviceName,ext,size,constant,global);

  mocu_register_var();

#if 0
  apilog* a;

  a = (apilog*)malloc(sizeof(apilog));

  a->type = REGISTERVAR;

  a->data.registerVar.hostVar = hostVar;
  a->data.registerVar.deviceAddress = deviceAddress;
  a->data.registerVar.deviceName = (char*)deviceName;
  a->data.registerVar.ext = ext;
  a->data.registerVar.size = size;
  a->data.registerVar.constant = constant;
  a->data.registerVar.global = global;

  a->prev = mocu.cp->a1->prev;
  a->next = mocu.cp->a1;
  a->prev->next = a;
  a->next->prev = a;
#endif

}

void __cudaRegisterTexture(void **fatCubinHandle,const struct textureReference *hostVar,const void **deviceAddress,const char *deviceName,int dim,int norm,int ext){

  TRACE("__cudaRegisterTexture");

  ENTER;


  return mocu.__mocudaRegisterTexture(fatCubinHandle,hostVar,deviceAddress,deviceName,dim,norm,ext);

}

void __cudaRegisterSurface(void **fatCubinHandle,const struct surfaceReference *hostVar,const void **deviceAddress,const char *deviceName,int dim,int ext){

  TRACE("__cudaRegisterSurface");

  ENTER;


  return mocu.__mocudaRegisterSurface(fatCubinHandle,hostVar,deviceAddress,deviceName,dim,ext);

}

void __cudaRegisterFunction(void **fatCubinHandle,const char *hostFun,char *deviceFun,const char *deviceName,int thread_limit,uint3 *tid,uint3 *bid,dim3 *bDim,dim3 *gDim,int *wSize){

  TRACE("__cudaRegisterFunction");

  ENTER;
    
  mocu.__mocudaRegisterFunction(fatCubinHandle,hostFun,deviceFun,deviceName,thread_limit,tid,bid,bDim,gDim,wSize);

#if 0

  printf("fatCubinHandle : %p\n",fatCubinHandle);
  printf("fatCubinHandle : %p\n",fatCubinHandle);
  printf("hostFun        : %s\n",hostFun);
  printf("deviceFun      : %s\n",deviceFun);
  printf("deviceName     : %s\n",deviceName);
  printf("thread_limit   : %d\n",thread_limit);
  printf("tid            : %p\n",tid);
  printf("bid            : %p\n",bid);
  printf("bDim           : %p\n",bDim);
  printf("gDim           : %p\n",gDim);
  printf("wSize          : %p\n",wSize);

#endif

#if 0
  apilog* a;

  a = (apilog*)malloc(sizeof(apilog));

  a->type = REGISTERFUNCTION;
  a->data.registerFunction.fatCubinHandle = fatCubinHandle;
  a->data.registerFunction.hostFun = hostFun;
  a->data.registerFunction.deviceFun = deviceFun;
  a->data.registerFunction.deviceName = deviceName;
  a->data.registerFunction.thread_limit = thread_limit;
  a->data.registerFunction.tid = (void*)tid;
  a->data.registerFunction.bid = (void*)bid;
  a->data.registerFunction.bDim = (void*)bDim;
  a->data.registerFunction.gDim = (void*)gDim;
  a->data.registerFunction.wSize = wSize;

  a->prev = mocu.cp->a1->prev;
  a->next = mocu.cp->a1;
  a->prev->next = a;
  a->next->prev = a;
#endif

}

cudaError_t cudaDeviceReset(void){

  TRACE("cudaDeviceReset");

  ENTER;

  cudaError_t res = mocu.mocudaDeviceReset();

  return res;

}


cudaError_t cudaDeviceSynchronize(void){

  //  TRACE("cudaDeviceSynchronize");

  ENTER;


  return mocu.mocudaDeviceSynchronize();

}


cudaError_t cudaDeviceSetLimit(enum cudaLimit limit,  size_t value){

  TRACE("cudaDeviceSetLimit");

  ENTER;


  return mocu.mocudaDeviceSetLimit(limit, value);

}


cudaError_t cudaDeviceGetLimit(size_t *pValue,  enum cudaLimit limit){

  TRACE("cudaDeviceGetLimit");

  ENTER;


  return mocu.mocudaDeviceGetLimit(pValue, limit);

}


cudaError_t cudaDeviceGetCacheConfig(enum cudaFuncCache *pCacheConfig){

  TRACE("cudaDeviceGetCacheConfig");

  ENTER;


  return mocu.mocudaDeviceGetCacheConfig(pCacheConfig);

}


cudaError_t cudaDeviceSetCacheConfig(enum cudaFuncCache cacheConfig){

  TRACE("cudaDeviceSetCacheConfig");

  ENTER;


  return mocu.mocudaDeviceSetCacheConfig(cacheConfig);

}


cudaError_t cudaDeviceGetSharedMemConfig(enum cudaSharedMemConfig *pConfig){

  TRACE("cudaDeviceGetSharedMemConfig");

  ENTER;


  return mocu.mocudaDeviceGetSharedMemConfig(pConfig);

}


cudaError_t cudaDeviceSetSharedMemConfig(enum cudaSharedMemConfig config){

  TRACE("cudaDeviceSetSharedMemConfig");

  ENTER;


  return mocu.mocudaDeviceSetSharedMemConfig(config);

}


cudaError_t cudaDeviceGetByPCIBusId(int *device,  char *pciBusId){

  TRACE("cudaDeviceGetByPCIBusId");

  ENTER;


  return mocu.mocudaDeviceGetByPCIBusId(device, pciBusId);

}


cudaError_t cudaDeviceGetPCIBusId(char *pciBusId,  int len,  int device){

  TRACE("cudaDeviceGetPCIBusId");

  ENTER;


  return mocu.mocudaDeviceGetPCIBusId(pciBusId, len, device);

}


cudaError_t cudaIpcGetEventHandle(cudaIpcEventHandle_t *handle,  cudaEvent_t event){

  TRACE("cudaIpcGetEventHandle");

  ENTER;


  return mocu.mocudaIpcGetEventHandle(handle, event);

}


cudaError_t cudaIpcOpenEventHandle(cudaEvent_t *event,  cudaIpcEventHandle_t handle){

  TRACE("cudaIpcOpenEventHandle");

  ENTER;


  return mocu.mocudaIpcOpenEventHandle(event, handle);

}


cudaError_t cudaIpcGetMemHandle(cudaIpcMemHandle_t *handle,  void *devPtr){

  TRACE("cudaIpcGetMemHandle");

  ENTER;


  return mocu.mocudaIpcGetMemHandle(handle, devPtr);

}


cudaError_t cudaIpcOpenMemHandle(void **devPtr,  cudaIpcMemHandle_t handle,  unsigned int flags){

  TRACE("cudaIpcOpenMemHandle");

  ENTER;


  return mocu.mocudaIpcOpenMemHandle(devPtr, handle, flags);

}


cudaError_t cudaIpcCloseMemHandle(void *devPtr){

  TRACE("cudaIpcCloseMemHandle");

  ENTER;


  return mocu.mocudaIpcCloseMemHandle(devPtr);

}


cudaError_t cudaThreadExit(void){

  TRACE("cudaThreadExit");

  ENTER;

  cudaError_t res = mocu.mocudaThreadExit();

  return res;

}


cudaError_t cudaThreadSynchronize(void){

  TRACE("cudaThreadSynchronize");

  ENTER;


  return mocu.mocudaThreadSynchronize();

}


cudaError_t cudaThreadSetLimit(enum cudaLimit limit,  size_t value){

  TRACE("cudaThreadSetLimit");

  ENTER;


  return mocu.mocudaThreadSetLimit(limit, value);

}


cudaError_t cudaThreadGetLimit(size_t *pValue,  enum cudaLimit limit){

  TRACE("cudaThreadGetLimit");

  ENTER;


  return mocu.mocudaThreadGetLimit(pValue, limit);

}


cudaError_t cudaThreadGetCacheConfig(enum cudaFuncCache *pCacheConfig){

  TRACE("cudaThreadGetCacheConfig");

  ENTER;


  return mocu.mocudaThreadGetCacheConfig(pCacheConfig);

}


cudaError_t cudaThreadSetCacheConfig(enum cudaFuncCache cacheConfig){

  TRACE("cudaThreadSetCacheConfig");

  ENTER;


  return mocu.mocudaThreadSetCacheConfig(cacheConfig);

}


cudaError_t cudaGetLastError(void){

  TRACE("cudaGetLastError");

  ENTER;


  return mocu.mocudaGetLastError();

}


cudaError_t cudaPeekAtLastError(void){

  TRACE("cudaPeekAtLastError");

  ENTER;


  return mocu.mocudaPeekAtLastError();

}


const char* cudaGetErrorString(cudaError_t error){

  TRACE("cudaGetErrorString");

  ENTER;


  return mocu.mocudaGetErrorString(error);

}


cudaError_t cudaGetDeviceCount(int *count){

  TRACE("cudaGetDeviceCount");

  ENTER;


  return mocu.mocudaGetDeviceCount(count);

}


cudaError_t cudaGetDeviceProperties(struct cudaDeviceProp *prop,  int device){

  TRACE("cudaGetDeviceProperties");

  ENTER;


  return mocu.mocudaGetDeviceProperties(prop, device);

}


cudaError_t cudaDeviceGetAttribute(int *value,  enum cudaDeviceAttr attr,  int device){

  TRACE("cudaDeviceGetAttribute");

  ENTER;


  return mocu.mocudaDeviceGetAttribute(value, attr, device);

}


cudaError_t cudaChooseDevice(int *device,  const struct cudaDeviceProp *prop){

  TRACE("cudaChooseDevice");

  ENTER;


  return mocu.mocudaChooseDevice(device, prop);

}


cudaError_t cudaSetDevice(int device){

  TRACE("cudaSetDevice");

  ENTER;

  mocu_pos = device;

  return mocu.mocudaSetDevice(device);

}


cudaError_t cudaGetDevice(int *device){

  TRACE("cudaGetDevice");

  ENTER;


  return mocu.mocudaGetDevice(device);

}


cudaError_t cudaSetValidDevices(int *device_arr,  int len){

  TRACE("cudaSetValidDevices");

  ENTER;

  return mocu.mocudaSetValidDevices(device_arr, len);

}


cudaError_t cudaSetDeviceFlags( unsigned int flags ){

  TRACE("cudaSetDeviceFlags");

  ENTER;

  //    return mocu.mocudaSetDeviceFlags(flags );
  return cudaSuccess;

}


cudaError_t cudaStreamCreate(cudaStream_t *pStream){

#if ENCAP

  TRACE("cudaStreamCreate");

  ENTER;

  mocu_stream* sp;
  cudaStream_t s;
  cudaError_t res;

  res = mocu.mocudaStreamCreate(&s);

  sp = (mocu_stream*)malloc(sizeof(mocu_stream));
  sp->s = s;
  sp->mode = 0;
  sp->prev = mocu.cp->s1->prev;
  sp->next = mocu.cp->s1;
  sp->prev->next = sp;
  sp->next->prev = sp;

  *pStream = (cudaStream_t)sp;

  return res;

#else

  return mocu.mocudaStreamCreate(pStream);

#endif

}


cudaError_t cudaStreamCreateWithFlags(cudaStream_t *pStream,  unsigned int flags){

  TRACE("cudaStreamCreateWithFlags");

  ENTER;

  return mocu.mocudaStreamCreateWithFlags(pStream, flags);

}


cudaError_t cudaStreamDestroy(cudaStream_t iStream){

  TRACE("cudaStreamDestroy");

  ENTER;

#if ENCAP

  mocu_stream* sp;
  cudaError_t res;

  sp = (mocu_stream*)iStream;

  res = mocu.mocudaStreamDestroy(sp->s);

  sp->next->prev = sp->prev;
  sp->prev->next = sp->next;

  return res;

#else

  return mocu.mocudaStreamDestroy(iStream);

#endif
}


cudaError_t cudaStreamWaitEvent(cudaStream_t stream,  cudaEvent_t event,  unsigned int flags){

  TRACE("cudaStreamWaitEvent");

  ENTER;


  return mocu.mocudaStreamWaitEvent(stream, event, flags);

}


#if 0
cudaError_t cudaStreamAddCallback(cudaStream_t stream, cudaStreamCallback_t callback,  void *userData,  unsigned int flags){

  TRACE("cudaStreamAddCallback");

  ENTER;


  return mocu.mocudaStreamAddCallback(stream, callback, userData, flags);

}
#endif

cudaError_t cudaStreamSynchronize(cudaStream_t stream){

  TRACE("cudaStreamSynchronize");

  ENTER;

#if ENCAP

  mocu_stream* sp;

  sp = (mocu_stream*)stream;

  return mocu.mocudaStreamSynchronize(sp->s);

#else

  return mocu.mocudaStreamSynchronize(stream);

#endif

}


cudaError_t cudaStreamQuery(cudaStream_t stream){

  TRACE("cudaStreamQuery");

  ENTER;


  return mocu.mocudaStreamQuery(stream);

}

cudaError_t cudaEventCreateWithFlags(cudaEvent_t *iEvent,  unsigned int flags){

  TRACE("cudaEventCreateWithFlags");

  ENTER;

#if ENCAP

  mocu_event* ep;
  cudaEvent_t e;
  cudaError_t res;

  res = mocu.mocudaEventCreateWithFlags(&e,flags);

  ep = (mocu_event*)malloc(sizeof(mocu_event));
  ep->e = e;
  ep->flags = flags;
  ep->mode = 0;
  ep->prev = mocu.cp->e1->prev;
  ep->next = mocu.cp->e1;
  ep->prev->next = ep;
  ep->next->prev = ep;

  *iEvent = (cudaEvent_t)ep;

  return res;

#else

  return mocu.mocudaEventCreateWithFlags(iEvent,flags);

#endif

}


cudaError_t cudaEventCreate(cudaEvent_t *iEvent){

  TRACE("cudaEventCreate");

  ENTER;

#if ENCAP

  //return cudaEventCreateWithFlags(iEvent,cudaEventDefault);

#if 1

  mocu_event* ep;
  cudaEvent_t e;
  cudaError_t res;

  res = mocu.mocudaEventCreate(&e);

  ep = (mocu_event*)malloc(sizeof(mocu_event));
  ep->e = e;
  ep->mode = 0;
  ep->prev = mocu.cp->e1->prev;
  ep->next = mocu.cp->e1;
  ep->prev->next = ep;
  ep->next->prev = ep;

  *iEvent = (cudaEvent_t)ep;

  return res;

#endif

#else

  return mocu.mocudaEventCreate(iEvent);

#endif

}

cudaError_t cudaEventRecord(cudaEvent_t iEvent,  cudaStream_t iStream ){

  TRACE("cudaEventRecord");

  ENTER;

#if ENCAP

  mocu_event* ep;
  mocu_stream* sp;

  ep = (mocu_event*)iEvent;
  sp = (mocu_stream*)iStream;

  ep->mode = 1;

  return mocu.mocudaEventRecord(ep->e, sp != NULL ? sp->s : NULL);

#else

  return mocu.mocudaEventRecord(iEvent,iStream);

#endif

}


cudaError_t cudaEventQuery(cudaEvent_t event){

  TRACE("cudaEventQuery");

  ENTER;


  return mocu.mocudaEventQuery(event);

}


cudaError_t cudaEventSynchronize(cudaEvent_t event){

  TRACE("cudaEventSynchronize");

  ENTER;

#if ENCAP

  mocu_event* ep;
  cudaError_t res;

  ep = (mocu_event*)event;

  res = mocu.mocudaEventSynchronize(ep->e);

  return res;

#else

  return mocu.mocudaEventSynchronize(event);

#endif

}


cudaError_t cudaEventDestroy(cudaEvent_t event){

  TRACE("cudaEventDestroy");

  ENTER;

#if ENCAP

  mocu_event* ep;
  cudaError_t res;

  ep = (mocu_event*)event;

  res = mocu.mocudaEventDestroy(ep->e);

  ep->next->prev = ep->prev;
  ep->prev->next = ep->next;

  return res;

#else

  return mocu.mocudaEventDestroy(event);

#endif

}


cudaError_t cudaEventElapsedTime(float *ms,  cudaEvent_t start,  cudaEvent_t end){

  TRACE("cudaEventElapsedTime");

  ENTER;

#if ENCAP

  mocu_event *ep1,*ep2;
  cudaError_t res;

  ep1 = (mocu_event*)start;
  ep2 = (mocu_event*)end;

  res = mocu.mocudaEventElapsedTime(ms,ep1->e,ep2->e);

  return res;

#else

  return mocu.mocudaEventElapsedTime(ms,start,end);

#endif

}


cudaError_t cudaConfigureCall(dim3 gridDim,  dim3 blockDim,  size_t sharedMem ,  cudaStream_t stream ){

  //  TRACE("cudaConfigureCall");

  ENTER;

#if ENCAP

  mocu_stream *sp;

  sp = (mocu_stream*)stream;

  return mocu.mocudaConfigureCall(gridDim, blockDim, sharedMem , sp != NULL ? sp->s : NULL);

#else

  return mocu.mocudaConfigureCall(gridDim, blockDim, sharedMem , stream);

#endif

}


cudaError_t cudaSetupArgument(const void *arg,  size_t size,  size_t offset){

  //  TRACE("cudaSetupArgument");

  ENTER;


  return mocu.mocudaSetupArgument(arg, size, offset);

}


cudaError_t cudaFuncSetCacheConfig(const void *func,  enum cudaFuncCache cacheConfig){

  TRACE("cudaFuncSetCacheConfig");

  ENTER;


  return mocu.mocudaFuncSetCacheConfig(func, cacheConfig);

}


cudaError_t cudaFuncSetSharedMemConfig(const void *func,  enum cudaSharedMemConfig config){

  TRACE("cudaFuncSetSharedMemConfig");

  ENTER;


  return mocu.mocudaFuncSetSharedMemConfig(func, config);

}

cudaError_t cudaLaunch(const void *func){

  //  TRACE("cudaLaunch");

  ENTER;

  region* rtemp;

  rtemp = mocu.cp->d0->next;
  while(rtemp->next != NULL){
    if(!rtemp->flag){
      rtemp->flag = 1;
      //      printf("\t\tFIND NOT FLAGED REGION\n");
    }
    rtemp = rtemp->next;
  }

  return mocu.mocudaLaunch(func);

}


cudaError_t cudaFuncGetAttributes(struct cudaFuncAttributes *attr,  const void *func){

  TRACE("cudaFuncGetAttributes");

  ENTER;


  return mocu.mocudaFuncGetAttributes(attr, func);

}


cudaError_t cudaSetDoubleForDevice(double *d){

  TRACE("cudaSetDoubleForDevice");

  ENTER;


  return mocu.mocudaSetDoubleForDevice(d);

}


cudaError_t cudaSetDoubleForHost(double *d){

  TRACE("cudaSetDoubleForHost");

  ENTER;


  return mocu.mocudaSetDoubleForHost(d);

}


cudaError_t cudaMalloc(void **devPtr,  size_t size){

  TRACE("cudaMalloc");

  ENTER;

  try_to_allocate(size);

  cudaError_t res;
  region* r;
  apilog* a;

  res = mocu.mocudaMalloc(devPtr,size);

  if(res == cudaSuccess){

    malloc_done(size);

    a = (apilog*)malloc(sizeof(apilog));
    a->type = MALLOC;
    a->data.malloc.size = size;
    a->data.malloc.devPtr = *devPtr;
    a->prev = mocu.cp->a1->prev;
    a->next = mocu.cp->a1;
    a->prev->next = a;
    a->next->prev = a;

    r = (region*)malloc(sizeof(region));
    r->size = size;
    r->base = *devPtr;
    r->mode = 0;
    r->flag = 0;
    r->prev = mocu.cp->d1->prev;
    r->next = mocu.cp->d1;
    r->prev->next = r;
    r->next->prev = r;

  }else if(res == cudaErrorMemoryAllocation){

    failed_to_get(size);

    res = mocu.mocudaMalloc(devPtr,size);

  }

  return res;

}


cudaError_t cudaMallocHost(void **ptr,  size_t size){

  TRACE("cudaMallocHost");

  ENTER;


  return mocu.mocudaMallocHost(ptr, size);

}


cudaError_t cudaMallocPitch(void **devPtr,  size_t *pitch,  size_t width,  size_t height){

  TRACE("cudaMallocPitch");

  ENTER;


  return mocu.mocudaMallocPitch(devPtr, pitch, width, height);

}


cudaError_t cudaMallocArray(cudaArray_t *array,  const struct cudaChannelFormatDesc *desc,  size_t width,  size_t height ,  unsigned int flags ){

  TRACE("cudaMallocArray");

  ENTER;


  return mocu.mocudaMallocArray(array, desc, width, height , flags );

}


cudaError_t cudaFree(void *devPtr){

  TRACE("cudaFree");

  ENTER;

  apilog *a;
  region *r;
  cudaError_t res;

  res = mocu.mocudaFree(devPtr);

  if(res == cudaSuccess){

    a = (apilog*)malloc(sizeof(apilog));
    a->type = FREE;
    a->data.free.devPtr = devPtr;
    a->prev = mocu.cp->a1->prev;
    a->next = mocu.cp->a1;
    a->prev->next = a;
    a->next->prev = a;
    
    r = mocu.cp->d0->next;

    while(r->mode >= 0){
      if(r->base == devPtr){
	r->next->prev = r->prev;
	r->prev->next = r->next;
	free(r);
	break;
      }
      r = r->next;
    }

    send_renew();
  }

  return res;

}


cudaError_t cudaFreeHost(void *ptr){

  TRACE("cudaFreeHost");

  ENTER;


  return mocu.mocudaFreeHost(ptr);

}


cudaError_t cudaFreeArray(cudaArray_t array){

  TRACE("cudaFreeArray");

  ENTER;


  return mocu.mocudaFreeArray(array);

}


cudaError_t cudaFreeMipmappedArray(cudaMipmappedArray_t mipmappedArray){

  TRACE("cudaFreeMipmappedArray");

  ENTER;


  return mocu.mocudaFreeMipmappedArray(mipmappedArray);

}


cudaError_t cudaHostAlloc(void **pHost,  size_t size,  unsigned int flags){

  TRACE("cudaHostAlloc");

  ENTER;

  return mocu.mocudaHostAlloc(pHost, size, flags|cudaHostAllocPortable);

}


cudaError_t cudaHostRegister(void *ptr,  size_t size,  unsigned int flags){

  TRACE("cudaHostRegister");

  ENTER;


  return mocu.mocudaHostRegister(ptr, size, flags);

}


cudaError_t cudaHostUnregister(void *ptr){

  TRACE("cudaHostUnregister");

  ENTER;


  return mocu.mocudaHostUnregister(ptr);

}


cudaError_t cudaHostGetDevicePointer(void **pDevice,  void *pHost,  unsigned int flags){

  TRACE("cudaHostGetDevicePointer");

  ENTER;


  return mocu.mocudaHostGetDevicePointer(pDevice, pHost, flags);

}


cudaError_t cudaHostGetFlags(unsigned int *pFlags,  void *pHost){

  TRACE("cudaHostGetFlags");

  ENTER;


  return mocu.mocudaHostGetFlags(pFlags, pHost);

}


cudaError_t cudaMalloc3D(struct cudaPitchedPtr* pitchedDevPtr,  struct cudaExtent extent){

  TRACE("cudaMalloc3D");

  ENTER;


  return mocu.mocudaMalloc3D(pitchedDevPtr, extent);

}


cudaError_t cudaMalloc3DArray(cudaArray_t *array,  const struct cudaChannelFormatDesc* desc,  struct cudaExtent extent,  unsigned int flags ){

  TRACE("cudaMalloc3DArray");

  ENTER;


  return mocu.mocudaMalloc3DArray(array, desc, extent, flags );

}


cudaError_t cudaMallocMipmappedArray(cudaMipmappedArray_t *mipmappedArray,  const struct cudaChannelFormatDesc* desc,  struct cudaExtent extent,  unsigned int numLevels,  unsigned int flags ){

  TRACE("cudaMallocMipmappedArray");

  ENTER;


  return mocu.mocudaMallocMipmappedArray(mipmappedArray, desc, extent, numLevels, flags );

}


cudaError_t cudaGetMipmappedArrayLevel(cudaArray_t *levelArray,  cudaMipmappedArray_const_t mipmappedArray,  unsigned int level){

  TRACE("cudaGetMipmappedArrayLevel");

  ENTER;


  return mocu.mocudaGetMipmappedArrayLevel(levelArray, mipmappedArray, level);

}


cudaError_t cudaMemcpy3D(const struct cudaMemcpy3DParms *p){

  TRACE("cudaMemcpy3D");

  ENTER;


  return mocu.mocudaMemcpy3D(p);

}


cudaError_t cudaMemcpy3DPeer(const struct cudaMemcpy3DPeerParms *p){

  TRACE("cudaMemcpy3DPeer");

  ENTER;


  return mocu.mocudaMemcpy3DPeer(p);

}


cudaError_t cudaMemcpy3DAsync(const struct cudaMemcpy3DParms *p,  cudaStream_t stream ){

  TRACE("cudaMemcpy3DAsync");

  ENTER;


  return mocu.mocudaMemcpy3DAsync(p, stream );

}


cudaError_t cudaMemcpy3DPeerAsync(const struct cudaMemcpy3DPeerParms *p,  cudaStream_t stream ){

  TRACE("cudaMemcpy3DPeerAsync");

  ENTER;


  return mocu.mocudaMemcpy3DPeerAsync(p, stream );

}


cudaError_t cudaMemGetInfo(size_t *free,  size_t *total){

  TRACE("cudaMemGetInfo");

  ENTER;


  return mocu.mocudaMemGetInfo(free, total);

}


cudaError_t cudaArrayGetInfo(struct cudaChannelFormatDesc *desc,  struct cudaExtent *extent,  unsigned int *flags,  cudaArray_t array){

  TRACE("cudaArrayGetInfo");

  ENTER;


  return mocu.mocudaArrayGetInfo(desc, extent, flags, array);

}


cudaError_t cudaMemcpy(void *dst,  const void *src,  size_t count,  enum cudaMemcpyKind kind){

  //  TRACE("cudaMemcpy");

  ENTER;

  //  printf("\t\tcudaMemcpy (DIVICE FLAG CHECK)\n");

  region* rtemp;

  rtemp = mocu.cp->d0->next;

  if(kind == cudaMemcpyHostToDevice){
    while(rtemp->next != NULL){
      if(rtemp->base == dst){
	if(!rtemp->flag){
	  rtemp->flag = 1;
	  //	  printf("\t\tFIND NOT FLAGED LOG (FLAG => ON)\n");
	  break;
	}
      }
      rtemp = rtemp->next;
    }
  }else{
    while(rtemp->next != NULL){
      if(rtemp->base == src){
	if(rtemp->flag){
	  rtemp->flag = 1;
	  //	  printf("\t\tFIND NOT FLAGED LOG (FLAG => ON)\n");
	  break;
	}
      }
      rtemp = rtemp->next;
    }
  }

  return mocu.mocudaMemcpy(dst, src, count, kind);

}


cudaError_t cudaMemcpyPeer(void *dst,  int dstDevice,  const void *src,  int srcDevice,  size_t count){

  TRACE("cudaMemcpyPeer");

  ENTER;


  return mocu.mocudaMemcpyPeer(dst, dstDevice, src, srcDevice, count);

}


cudaError_t cudaMemcpyToArray(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t count,  enum cudaMemcpyKind kind){

  TRACE("cudaMemcpyToArray");

  ENTER;


  return mocu.mocudaMemcpyToArray(dst, wOffset, hOffset, src, count, kind);

}


cudaError_t cudaMemcpyFromArray(void *dst,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t count,  enum cudaMemcpyKind kind){

  TRACE("cudaMemcpyFromArray");

  ENTER;


  return mocu.mocudaMemcpyFromArray(dst, src, wOffset, hOffset, count, kind);

}


cudaError_t cudaMemcpyArrayToArray(cudaArray_t dst,  size_t wOffsetDst,  size_t hOffsetDst,  cudaArray_const_t src,  size_t wOffsetSrc,  size_t hOffsetSrc,  size_t count,  enum cudaMemcpyKind kind ){

  TRACE("cudaMemcpyArrayToArray");

  ENTER;


  return mocu.mocudaMemcpyArrayToArray(dst, wOffsetDst, hOffsetDst, src, wOffsetSrc, hOffsetSrc, count, kind );

}


cudaError_t cudaMemcpy2D(void *dst,  size_t dpitch,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind){

  TRACE("cudaMemcpy2D");

  ENTER;


  return mocu.mocudaMemcpy2D(dst, dpitch, src, spitch, width, height, kind);

}


cudaError_t cudaMemcpy2DToArray(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind){

  TRACE("cudaMemcpy2DToArray");

  ENTER;


  return mocu.mocudaMemcpy2DToArray(dst, wOffset, hOffset, src, spitch, width, height, kind);

}


cudaError_t cudaMemcpy2DFromArray(void *dst,  size_t dpitch,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t width,  size_t height,  enum cudaMemcpyKind kind){

  TRACE("cudaMemcpy2DFromArray");

  ENTER;


  return mocu.mocudaMemcpy2DFromArray(dst, dpitch, src, wOffset, hOffset, width, height, kind);

}


cudaError_t cudaMemcpy2DArrayToArray(cudaArray_t dst,  size_t wOffsetDst,  size_t hOffsetDst,  cudaArray_const_t src,  size_t wOffsetSrc,  size_t hOffsetSrc,  size_t width,  size_t height,  enum cudaMemcpyKind kind ){

  TRACE("cudaMemcpy2DArrayToArray");

  ENTER;


  return mocu.mocudaMemcpy2DArrayToArray(dst, wOffsetDst, hOffsetDst, src, wOffsetSrc, hOffsetSrc, width, height, kind );

}


cudaError_t cudaMemcpyToSymbol(const void *symbol,  const void *src,  size_t count,  size_t offset ,  enum cudaMemcpyKind kind ){

  TRACE("cudaMemcpyToSymbol");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpyToSymbol(symbol,src,count,offset,kind);

  return res;

}


cudaError_t cudaMemcpyFromSymbol(void *dst,  const void *symbol,  size_t count,  size_t offset ,  enum cudaMemcpyKind kind ){

  TRACE("cudaMemcpyFromSymbol");

  ENTER;

#if 0
  cudaError_t res;

  res = mocu.mocudaMemcpyFromSymbol(dst, symbol, count, offset , kind );

  if(res == cudaErrorMemoryAllocation){

    failed_to_get(mocu_get_amount_of_memory_used());
    res = mocu.mocudaMemcpyFromSymbol(dst, symbol, count, offset , kind );

  }

  return res;
#endif

  return mocu.mocudaMemcpyFromSymbol(dst, symbol, count, offset , kind );

}


cudaError_t cudaMemcpyAsync(void *dst,  const void *src,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t iStream ){

  TRACE("cudaMemcpyAsync");

  ENTER;

#if ENCAP

  mocu_stream *sp;
    
  sp = (mocu_stream*)iStream;

  return mocu.mocudaMemcpyAsync(dst, src, count, kind, sp != NULL ? sp->s : NULL);

#else

  return mocu.mocudaMemcpyAsync(dst, src, count, kind, iStream);

#endif
}


cudaError_t cudaMemcpyPeerAsync(void *dst,  int dstDevice,  const void *src,  int srcDevice,  size_t count,  cudaStream_t stream ){

  TRACE("cudaMemcpyPeerAsync");

  ENTER;


  return mocu.mocudaMemcpyPeerAsync(dst, dstDevice, src, srcDevice, count, stream );

}


cudaError_t cudaMemcpyToArrayAsync(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpyToArrayAsync");

  ENTER;


  return mocu.mocudaMemcpyToArrayAsync(dst, wOffset, hOffset, src, count, kind, stream );

}


cudaError_t cudaMemcpyFromArrayAsync(void *dst,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpyFromArrayAsync");

  ENTER;


  return mocu.mocudaMemcpyFromArrayAsync(dst, src, wOffset, hOffset, count, kind, stream );

}


cudaError_t cudaMemcpy2DAsync(void *dst,  size_t dpitch,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpy2DAsync");

  ENTER;


  return mocu.mocudaMemcpy2DAsync(dst, dpitch, src, spitch, width, height, kind, stream );

}


cudaError_t cudaMemcpy2DToArrayAsync(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpy2DToArrayAsync");

  ENTER;


  return mocu.mocudaMemcpy2DToArrayAsync(dst, wOffset, hOffset, src, spitch, width, height, kind, stream );

}


cudaError_t cudaMemcpy2DFromArrayAsync(void *dst,  size_t dpitch,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpy2DFromArrayAsync");

  ENTER;


  return mocu.mocudaMemcpy2DFromArrayAsync(dst, dpitch, src, wOffset, hOffset, width, height, kind, stream );

}


cudaError_t cudaMemcpyToSymbolAsync(const void *symbol,  const void *src,  size_t count,  size_t offset,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpyToSymbolAsync");

  ENTER;


  return mocu.mocudaMemcpyToSymbolAsync(symbol, src, count, offset, kind, stream );

}


cudaError_t cudaMemcpyFromSymbolAsync(void *dst,  const void *symbol,  size_t count,  size_t offset,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpyFromSymbolAsync");

  ENTER;


  return mocu.mocudaMemcpyFromSymbolAsync(dst, symbol, count, offset, kind, stream );

}


cudaError_t cudaMemset(void *devPtr,  int value,  size_t count){

  TRACE("cudaMemset");

  ENTER;

  region* rtemp;

  rtemp = mocu.cp->d0->next;

  while(rtemp->next != NULL){
    if(rtemp->base == devPtr){
      if(!rtemp->flag){
	rtemp->flag = 1;
	//	printf("\t\tFIND NOT FLAGED LOG (FLAG => ON)\n");
	break;
      }
    }
    rtemp = rtemp->next;
  }
  
  return mocu.mocudaMemset(devPtr, value, count);

}


cudaError_t cudaMemset2D(void *devPtr,  size_t pitch,  int value,  size_t width,  size_t height){

  TRACE("cudaMemset2D");

  ENTER;


  return mocu.mocudaMemset2D(devPtr, pitch, value, width, height);

}


cudaError_t cudaMemset3D(struct cudaPitchedPtr pitchedDevPtr,  int value,  struct cudaExtent extent){

  TRACE("cudaMemset3D");

  ENTER;


  return mocu.mocudaMemset3D(pitchedDevPtr, value, extent);

}


cudaError_t cudaMemsetAsync(void *devPtr,  int value,  size_t count,  cudaStream_t stream ){

  TRACE("cudaMemsetAsync");

  ENTER;


  return mocu.mocudaMemsetAsync(devPtr, value, count, stream );

}


cudaError_t cudaMemset2DAsync(void *devPtr,  size_t pitch,  int value,  size_t width,  size_t height,  cudaStream_t stream ){

  TRACE("cudaMemset2DAsync");

  ENTER;


  return mocu.mocudaMemset2DAsync(devPtr, pitch, value, width, height, stream );

}


cudaError_t cudaMemset3DAsync(struct cudaPitchedPtr pitchedDevPtr,  int value,  struct cudaExtent extent,  cudaStream_t stream ){

  TRACE("cudaMemset3DAsync");

  ENTER;


  return mocu.mocudaMemset3DAsync(pitchedDevPtr, value, extent, stream );

}


cudaError_t cudaGetSymbolAddress(void **devPtr,  const void *symbol){

  TRACE("cudaGetSymbolAddress");

  ENTER;

#if 0

  cudaError_t res;

  res = mocu.mocudaGetSymbolAddress(devPtr, symbol);

  if(res == cudaErrorMemoryAllocation){
    
    failed_to_get(mocu_get_amount_of_memory_used());
    res = mocu.mocudaGetSymbolAddress(devPtr, symbol);

  }

  return res;

#endif

  return mocu.mocudaGetSymbolAddress(devPtr, symbol);

}


cudaError_t cudaGetSymbolSize(size_t *size,  const void *symbol){

  TRACE("cudaGetSymbolSize");

  ENTER;

#if 0
  cudaError_t res;

  res = mocu.mocudaGetSymbolSize(size, symbol);

  if(res == cudaErrorMemoryAllocation){

    failed_to_get(mocu_get_amount_of_memory_used());
    res = mocu.mocudaGetSymbolSize(size, symbol);

  }

  return res;
#endif

  return mocu.mocudaGetSymbolSize(size, symbol);
}


cudaError_t cudaPointerGetAttributes(struct cudaPointerAttributes *attributes,  const void *ptr){

  TRACE("cudaPointerGetAttributes");

  ENTER;


  return mocu.mocudaPointerGetAttributes(attributes, ptr);

}


cudaError_t cudaDeviceCanAccessPeer(int *canAccessPeer,  int device,  int peerDevice){

  TRACE("cudaDeviceCanAccessPeer");

  ENTER;


  return mocu.mocudaDeviceCanAccessPeer(canAccessPeer, device, peerDevice);

}


cudaError_t cudaDeviceEnablePeerAccess(int peerDevice,  unsigned int flags){

  TRACE("cudaDeviceEnablePeerAccess");

  ENTER;


  return mocu.mocudaDeviceEnablePeerAccess(peerDevice, flags);

}


cudaError_t cudaDeviceDisablePeerAccess(int peerDevice){

  TRACE("cudaDeviceDisablePeerAccess");

  ENTER;


  return mocu.mocudaDeviceDisablePeerAccess(peerDevice);

}


cudaError_t cudaGraphicsUnregisterResource(cudaGraphicsResource_t resource){

  TRACE("cudaGraphicsUnregisterResource");

  ENTER;


  return mocu.mocudaGraphicsUnregisterResource(resource);

}


cudaError_t cudaGraphicsResourceSetMapFlags(cudaGraphicsResource_t resource,  unsigned int flags){

  TRACE("cudaGraphicsResourceSetMapFlags");

  ENTER;


  return mocu.mocudaGraphicsResourceSetMapFlags(resource, flags);

}


cudaError_t cudaGraphicsMapResources(int count,  cudaGraphicsResource_t *resources,  cudaStream_t stream ){

  TRACE("cudaGraphicsMapResources");

  ENTER;


  return mocu.mocudaGraphicsMapResources(count, resources, stream );

}


cudaError_t cudaGraphicsUnmapResources(int count,  cudaGraphicsResource_t *resources,  cudaStream_t stream ){

  TRACE("cudaGraphicsUnmapResources");

  ENTER;


  return mocu.mocudaGraphicsUnmapResources(count, resources, stream );

}


cudaError_t cudaGraphicsResourceGetMappedPointer(void **devPtr,  size_t *size,  cudaGraphicsResource_t resource){

  TRACE("cudaGraphicsResourceGetMappedPointer");

  ENTER;


  return mocu.mocudaGraphicsResourceGetMappedPointer(devPtr, size, resource);

}


cudaError_t cudaGraphicsSubResourceGetMappedArray(cudaArray_t *array,  cudaGraphicsResource_t resource,  unsigned int arrayIndex,  unsigned int mipLevel){

  TRACE("cudaGraphicsSubResourceGetMappedArray");

  ENTER;


  return mocu.mocudaGraphicsSubResourceGetMappedArray(array, resource, arrayIndex, mipLevel);

}


cudaError_t cudaGraphicsResourceGetMappedMipmappedArray(cudaMipmappedArray_t *mipmappedArray,  cudaGraphicsResource_t resource){

  TRACE("cudaGraphicsResourceGetMappedMipmappedArray");

  ENTER;


  return mocu.mocudaGraphicsResourceGetMappedMipmappedArray(mipmappedArray, resource);

}


cudaError_t cudaGetChannelDesc(struct cudaChannelFormatDesc *desc,  cudaArray_const_t array){

  TRACE("cudaGetChannelDesc");

  ENTER;


  return mocu.mocudaGetChannelDesc(desc, array);

}


struct cudaChannelFormatDesc cudaCreateChannelDesc(int x,  int y,  int z,  int w,  enum cudaChannelFormatKind f){

  TRACE("cudaCreateChannelDesc");

  ENTER;


  return mocu.mocudaCreateChannelDesc(x, y, z, w, f);

}


cudaError_t cudaBindTexture(size_t *offset,  const struct textureReference *texref,  const void *devPtr,  const struct cudaChannelFormatDesc *desc,  size_t size ){

  TRACE("cudaBindTexture");

  ENTER;


  return mocu.mocudaBindTexture(offset, texref, devPtr, desc, size );

}


cudaError_t cudaBindTexture2D(size_t *offset,  const struct textureReference *texref,  const void *devPtr,  const struct cudaChannelFormatDesc *desc,  size_t width,  size_t height,  size_t pitch){

  TRACE("cudaBindTexture2D");

  ENTER;


  return mocu.mocudaBindTexture2D(offset, texref, devPtr, desc, width, height, pitch);

}


cudaError_t cudaBindTextureToArray(const struct textureReference *texref,  cudaArray_const_t array,  const struct cudaChannelFormatDesc *desc){

  TRACE("cudaBindTextureToArray");

  ENTER;


  return mocu.mocudaBindTextureToArray(texref, array, desc);

}


cudaError_t cudaBindTextureToMipmappedArray(const struct textureReference *texref,  cudaMipmappedArray_const_t mipmappedArray,  const struct cudaChannelFormatDesc *desc){

  TRACE("cudaBindTextureToMipmappedArray");

  ENTER;


  return mocu.mocudaBindTextureToMipmappedArray(texref, mipmappedArray, desc);

}


cudaError_t cudaUnbindTexture(const struct textureReference *texref){

  TRACE("cudaUnbindTexture");

  ENTER;


  return mocu.mocudaUnbindTexture(texref);

}


cudaError_t cudaGetTextureAlignmentOffset(size_t *offset,  const struct textureReference *texref){

  TRACE("cudaGetTextureAlignmentOffset");

  ENTER;


  return mocu.mocudaGetTextureAlignmentOffset(offset, texref);

}


cudaError_t cudaGetTextureReference(const struct textureReference **texref,  const void *symbol){

  TRACE("cudaGetTextureReference");

  ENTER;


  return mocu.mocudaGetTextureReference(texref, symbol);

}


cudaError_t cudaBindSurfaceToArray(const struct surfaceReference *surfref,  cudaArray_const_t array,  const struct cudaChannelFormatDesc *desc){

  TRACE("cudaBindSurfaceToArray");

  ENTER;


  return mocu.mocudaBindSurfaceToArray(surfref, array, desc);

}


cudaError_t cudaGetSurfaceReference(const struct surfaceReference **surfref,  const void *symbol){

  TRACE("cudaGetSurfaceReference");

  ENTER;


  return mocu.mocudaGetSurfaceReference(surfref, symbol);

}


cudaError_t cudaCreateTextureObject(cudaTextureObject_t *pTexObject,  const struct cudaResourceDesc *pResDesc,  const struct cudaTextureDesc *pTexDesc,  const struct cudaResourceViewDesc *pResViewDesc){

  TRACE("cudaCreateTextureObject");

  ENTER;


  return mocu.mocudaCreateTextureObject(pTexObject, pResDesc, pTexDesc, pResViewDesc);

}


cudaError_t cudaDestroyTextureObject(cudaTextureObject_t texObject){

  TRACE("cudaDestroyTextureObject");

  ENTER;


  return mocu.mocudaDestroyTextureObject(texObject);

}


cudaError_t cudaGetTextureObjectResourceDesc(struct cudaResourceDesc *pResDesc,  cudaTextureObject_t texObject){

  TRACE("cudaGetTextureObjectResourceDesc");

  ENTER;


  return mocu.mocudaGetTextureObjectResourceDesc(pResDesc, texObject);

}


cudaError_t cudaGetTextureObjectTextureDesc(struct cudaTextureDesc *pTexDesc,  cudaTextureObject_t texObject){

  TRACE("cudaGetTextureObjectTextureDesc");

  ENTER;


  return mocu.mocudaGetTextureObjectTextureDesc(pTexDesc, texObject);

}


cudaError_t cudaGetTextureObjectResourceViewDesc(struct cudaResourceViewDesc *pResViewDesc,  cudaTextureObject_t texObject){

  TRACE("cudaGetTextureObjectResourceViewDesc");

  ENTER;


  return mocu.mocudaGetTextureObjectResourceViewDesc(pResViewDesc, texObject);

}


cudaError_t cudaCreateSurfaceObject(cudaSurfaceObject_t *pSurfObject,  const struct cudaResourceDesc *pResDesc){

  TRACE("cudaCreateSurfaceObject");

  ENTER;


  return mocu.mocudaCreateSurfaceObject(pSurfObject, pResDesc);

}


cudaError_t cudaDestroySurfaceObject(cudaSurfaceObject_t surfObject){

  TRACE("cudaDestroySurfaceObject");

  ENTER;


  return mocu.mocudaDestroySurfaceObject(surfObject);

}


cudaError_t cudaGetSurfaceObjectResourceDesc(struct cudaResourceDesc *pResDesc,  cudaSurfaceObject_t surfObject){

  TRACE("cudaGetSurfaceObjectResourceDesc");

  ENTER;


  return mocu.mocudaGetSurfaceObjectResourceDesc(pResDesc, surfObject);

}


cudaError_t cudaDriverGetVersion(int *driverVersion){

  TRACE("cudaDriverGetVersion");

  ENTER;


  return mocu.mocudaDriverGetVersion(driverVersion);

}


cudaError_t cudaRuntimeGetVersion(int *runtimeVersion){

  TRACE("cudaRuntimeGetVersion");

  ENTER;


  return mocu.mocudaRuntimeGetVersion(runtimeVersion);

}


cudaError_t cudaGetExportTable(const void **ppExportTable,  const cudaUUID_t *pExportTableId){

  TRACE("cudaGetExportTable");

  ENTER;


  return mocu.mocudaGetExportTable(ppExportTable, pExportTableId);

}
