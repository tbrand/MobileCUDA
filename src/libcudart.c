#include <mocu.h>
#include <fatBinaryCtl.h>

int sem_id;
int mocu_pos;

MOCU mocu;

int initialized = 0;

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

  mocu.cp->ar0 = (mocu_array*)malloc(sizeof(mocu_array));
  mocu.cp->ar1 = (mocu_array*)malloc(sizeof(mocu_array));
  mocu.cp->ar0->mode = mocu.cp->ar1->mode = -1;
  mocu.cp->ar0->next = mocu.cp->ar1;
  mocu.cp->ar1->prev = mocu.cp->ar0;
  mocu.cp->ar1->next = NULL;
  mocu.cp->ar0->prev = NULL;

  mocu.cp->tx0 = (mocu_texture*)malloc(sizeof(mocu_texture));
  mocu.cp->tx1 = (mocu_texture*)malloc(sizeof(mocu_texture));
  mocu.cp->tx0->next = mocu.cp->tx1;
  mocu.cp->tx1->prev = mocu.cp->tx0;
  mocu.cp->tx1->next = NULL;
  mocu.cp->tx0->prev = NULL;

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

  mocu_parse(fatCubin);

  mocu_connect();

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

  mocu_register_var(size);

  mocu_add_symbol(hostVar,deviceAddress,(char*)deviceName,ext,size,constant,global);

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

  cudaError_t res;

  res = mocu.mocudaDeviceReset();

  LEAVE;

  return res;

}


cudaError_t cudaDeviceSynchronize(void){

  //  TRACE("cudaDeviceSynchronize");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceSynchronize();

  LEAVE;

  return res;

}


cudaError_t cudaDeviceSetLimit(enum cudaLimit limit,  size_t value){

  TRACE("cudaDeviceSetLimit");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceSetLimit(limit, value);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceGetLimit(size_t *pValue,  enum cudaLimit limit){

  TRACE("cudaDeviceGetLimit");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceGetLimit(pValue, limit);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceGetCacheConfig(enum cudaFuncCache *pCacheConfig){

  TRACE("cudaDeviceGetCacheConfig");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceGetCacheConfig(pCacheConfig);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceSetCacheConfig(enum cudaFuncCache cacheConfig){

  TRACE("cudaDeviceSetCacheConfig");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceSetCacheConfig(cacheConfig);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceGetSharedMemConfig(enum cudaSharedMemConfig *pConfig){

  TRACE("cudaDeviceGetSharedMemConfig");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceGetSharedMemConfig(pConfig);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceSetSharedMemConfig(enum cudaSharedMemConfig config){

  TRACE("cudaDeviceSetSharedMemConfig");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceSetSharedMemConfig(config);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceGetByPCIBusId(int *device,  char *pciBusId){

  TRACE("cudaDeviceGetByPCIBusId");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceGetByPCIBusId(device, pciBusId);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceGetPCIBusId(char *pciBusId,  int len,  int device){

  TRACE("cudaDeviceGetPCIBusId");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceGetPCIBusId(pciBusId, len, device);

  LEAVE;

  return res;

}


cudaError_t cudaIpcGetEventHandle(cudaIpcEventHandle_t *handle,  cudaEvent_t event){

  TRACE("cudaIpcGetEventHandle");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaIpcGetEventHandle(handle, event);

  LEAVE;

  return res;

}


cudaError_t cudaIpcOpenEventHandle(cudaEvent_t *event,  cudaIpcEventHandle_t handle){

  TRACE("cudaIpcOpenEventHandle");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaIpcOpenEventHandle(event, handle);

  LEAVE;

  return res;

}


cudaError_t cudaIpcGetMemHandle(cudaIpcMemHandle_t *handle,  void *devPtr){

  TRACE("cudaIpcGetMemHandle");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaIpcGetMemHandle(handle, devPtr);

  LEAVE;

  return res;

}


cudaError_t cudaIpcOpenMemHandle(void **devPtr,  cudaIpcMemHandle_t handle,  unsigned int flags){

  TRACE("cudaIpcOpenMemHandle");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaIpcOpenMemHandle(devPtr, handle, flags);

  LEAVE;

  return res;

}


cudaError_t cudaIpcCloseMemHandle(void *devPtr){

  TRACE("cudaIpcCloseMemHandle");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaIpcCloseMemHandle(devPtr);

  LEAVE;

  return res;

}


cudaError_t cudaThreadExit(void){

  TRACE("cudaThreadExit");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaThreadExit();

  LEAVE;

  return res;

}


cudaError_t cudaThreadSynchronize(void){

  TRACE("cudaThreadSynchronize");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaThreadSynchronize();

  LEAVE;

  return res;

}


cudaError_t cudaThreadSetLimit(enum cudaLimit limit,  size_t value){

  TRACE("cudaThreadSetLimit");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaThreadSetLimit(limit, value);

  LEAVE;

  return res;

}


cudaError_t cudaThreadGetLimit(size_t *pValue,  enum cudaLimit limit){

  TRACE("cudaThreadGetLimit");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaThreadGetLimit(pValue, limit);

  LEAVE;

  return res;

}


cudaError_t cudaThreadGetCacheConfig(enum cudaFuncCache *pCacheConfig){

  TRACE("cudaThreadGetCacheConfig");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaThreadGetCacheConfig(pCacheConfig);

  LEAVE;

  return res;

}


cudaError_t cudaThreadSetCacheConfig(enum cudaFuncCache cacheConfig){

  TRACE("cudaThreadSetCacheConfig");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaThreadSetCacheConfig(cacheConfig);

  LEAVE;

  return res;

}


cudaError_t cudaGetLastError(void){

  TRACE("cudaGetLastError");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGetLastError();

  LEAVE;

  return res;

}


cudaError_t cudaPeekAtLastError(void){

  TRACE("cudaPeekAtLastError");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaPeekAtLastError();

  LEAVE;

  return res;

}


const char* cudaGetErrorString(cudaError_t error){

  TRACE("cudaGetErrorString");

  ENTER;

  const char* res;
  
  res = mocu.mocudaGetErrorString(error);

  LEAVE;

  return res;

}


cudaError_t cudaGetDeviceCount(int *count){

  TRACE("cudaGetDeviceCount");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGetDeviceCount(count);

  LEAVE;

  return res;

}


cudaError_t cudaGetDeviceProperties(struct cudaDeviceProp *prop,  int device){

  TRACE("cudaGetDeviceProperties");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGetDeviceProperties(prop, device);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceGetAttribute(int *value,  enum cudaDeviceAttr attr,  int device){

  TRACE("cudaDeviceGetAttribute");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceGetAttribute(value, attr, device);

  LEAVE;

  return res;

}


cudaError_t cudaChooseDevice(int *device,  const struct cudaDeviceProp *prop){

  TRACE("cudaChooseDevice");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaChooseDevice(device, prop);

  LEAVE;

  return res;

}


cudaError_t cudaSetDevice(int device){

  TRACE("cudaSetDevice");

  ENTER;

  printf("\tWarning[Mobile CUDA] : CANNOT CALL cudaSetDevice()\n");
  printf("\tAlways return \"cudaSuccess\"\n");

  /*
    mocu_pos = device;

    cudaError_t res;

    res = mocu.mocudaSetDevice(device);
  */

  LEAVE;

  //  return res;
  return cudaSuccess;

}


cudaError_t cudaGetDevice(int *device){

  TRACE("cudaGetDevice");

  ENTER;

  /*
    cudaError_t res;

    res = mocu.mocudaGetDevice(device);
  */

  *device = 1;

  LEAVE;

  //  return res;
  return cudaSuccess;

}


cudaError_t cudaSetValidDevices(int *device_arr,  int len){

  TRACE("cudaSetValidDevices");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaSetValidDevices(device_arr, len);

  LEAVE;

  return res;

}


cudaError_t cudaSetDeviceFlags( unsigned int flags ){

  TRACE("cudaSetDeviceFlags");

  ENTER;

  //    return mocu.mocudaSetDeviceFlags(flags );
  cudaError_t res;

  res = cudaSuccess;

  LEAVE;

  return res;

}


cudaError_t cudaStreamCreate(cudaStream_t *pStream){

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

  LEAVE;

  return res;

}


cudaError_t cudaStreamCreateWithFlags(cudaStream_t *pStream,  unsigned int flags){

  TRACE("cudaStreamCreateWithFlags");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaStreamCreateWithFlags(pStream, flags);

  LEAVE;

  return res;

}


cudaError_t cudaStreamDestroy(cudaStream_t iStream){

  TRACE("cudaStreamDestroy");

  ENTER;

  mocu_stream* sp;
  cudaError_t res;

  sp = (mocu_stream*)iStream;

  res = mocu.mocudaStreamDestroy(sp->s);

  sp->next->prev = sp->prev;
  sp->prev->next = sp->next;

  LEAVE;

  return res;

}


cudaError_t cudaStreamWaitEvent(cudaStream_t stream,  cudaEvent_t event,  unsigned int flags){

  TRACE("cudaStreamWaitEvent");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaStreamWaitEvent(stream, event, flags);

  LEAVE;

  return res;

}


#if 0
cudaError_t cudaStreamAddCallback(cudaStream_t stream, cudaStreamCallback_t callback,  void *userData,  unsigned int flags){

  TRACE("cudaStreamAddCallback");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaStreamAddCallback(stream, callback, userData, flags);

  LEAVE;

  return res;

}
#endif

cudaError_t cudaStreamSynchronize(cudaStream_t stream){

  TRACE("cudaStreamSynchronize");

  ENTER;

  mocu_stream* sp;
  cudaError_t res;

  sp = (mocu_stream*)stream;

  res = mocu.mocudaStreamSynchronize(sp->s);

  LEAVE;

  return res;

}


cudaError_t cudaStreamQuery(cudaStream_t stream){

  TRACE("cudaStreamQuery");

  ENTER;


  cudaError_t res;

  res = mocu.mocudaStreamQuery(stream);

  LEAVE;

  return res;

}

cudaError_t cudaEventCreateWithFlags(cudaEvent_t *iEvent,  unsigned int flags){

  TRACE("cudaEventCreateWithFlags");

  ENTER;

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

  LEAVE;

  return res;

}


cudaError_t cudaEventCreate(cudaEvent_t *iEvent){

  TRACE("cudaEventCreate");

  ENTER;

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

  LEAVE;

  return res;

}

cudaError_t cudaEventRecord(cudaEvent_t iEvent,  cudaStream_t iStream ){

  TRACE("cudaEventRecord");

  ENTER;

  mocu_event* ep;
  mocu_stream* sp;
  cudaError_t res;

  ep = (mocu_event*)iEvent;
  sp = (mocu_stream*)iStream;

  ep->mode = 1;

  res =  mocu.mocudaEventRecord(ep->e, sp != NULL ? sp->s : NULL);

  LEAVE;

  return res;

}


cudaError_t cudaEventQuery(cudaEvent_t event){

  TRACE("cudaEventQuery");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaEventQuery(event);

  LEAVE;

  return res;

}


cudaError_t cudaEventSynchronize(cudaEvent_t event){

  TRACE("cudaEventSynchronize");

  ENTER;

  mocu_event* ep;
  cudaError_t res;

  ep = (mocu_event*)event;

  res = mocu.mocudaEventSynchronize(ep->e);

  LEAVE;

  return res;

}


cudaError_t cudaEventDestroy(cudaEvent_t event){

  TRACE("cudaEventDestroy");

  ENTER;

  mocu_event* ep;
  cudaError_t res;

  ep = (mocu_event*)event;

  res = mocu.mocudaEventDestroy(ep->e);

  ep->next->prev = ep->prev;
  ep->prev->next = ep->next;

  LEAVE;

  return res;

}


cudaError_t cudaEventElapsedTime(float *ms,  cudaEvent_t start,  cudaEvent_t end){

  TRACE("cudaEventElapsedTime");

  ENTER;

  mocu_event *ep1,*ep2;
  cudaError_t res;

  ep1 = (mocu_event*)start;
  ep2 = (mocu_event*)end;

  res = mocu.mocudaEventElapsedTime(ms,ep1->e,ep2->e);

  LEAVE;

  return res;

}


cudaError_t cudaConfigureCall(dim3 gridDim,  dim3 blockDim,  size_t sharedMem ,  cudaStream_t stream ){

  //  TRACE("cudaConfigureCall");

  ENTER;

  mocu_stream *sp;
  cudaError_t res;

  sp = (mocu_stream*)stream;

  res = mocu.mocudaConfigureCall(gridDim, blockDim, sharedMem , sp != NULL ? sp->s : NULL);

  LEAVE;

  return res;

}


cudaError_t cudaSetupArgument(const void *arg,  size_t size,  size_t offset){

  //  TRACE("cudaSetupArgument");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaSetupArgument(arg, size, offset);

  LEAVE;

  return res;

}


cudaError_t cudaFuncSetCacheConfig(const void *func,  enum cudaFuncCache cacheConfig){

  TRACE("cudaFuncSetCacheConfig");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaFuncSetCacheConfig(func, cacheConfig);

  LEAVE;

  return res;

}


cudaError_t cudaFuncSetSharedMemConfig(const void *func,  enum cudaSharedMemConfig config){

  TRACE("cudaFuncSetSharedMemConfig");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaFuncSetSharedMemConfig(func, config);

  LEAVE;

  return res;

}

int profiled = 1;

cudaError_t cudaLaunch(const void *func){

  //  TRACE("cudaLaunch");

  ENTER;

  cudaError_t res;
  region* rtemp;

  rtemp = mocu.cp->d0->next;
  while(rtemp->next != NULL){
    if(!rtemp->flag){
      rtemp->flag = 1;
      //      printf("\t\tFIND NOT FLAGED REGION\n");
    }
    rtemp = rtemp->next;
  }

  if(!profiled){

    apilog* ac0;
    apilog* ac1;

    init_cupti();

    /*
    ac0 = (apilog*)malloc(sizeof(apilog));
    ac0->type = CUPTI_INIT;
    ac0->prev = mocu.cp->a1->prev;
    ac0->next = mocu.cp->a1;
    ac0->prev->next = ac0;
    ac0->next->prev = ac0;

    printf("\t>>>ADD LOG(CUPTI_INIT)\n");
    */

    res = mocu.mocudaLaunch(func);

    //    mocu_send_profile();

    cupti_destroy();

    /*
    ac1 = (apilog*)malloc(sizeof(apilog));
    ac1->type = CUPTI_DESTROY;
    ac1->prev = mocu.cp->a1->prev;
    ac1->next = mocu.cp->a1;
    ac1->prev->next = ac1;
    ac1->next->prev = ac1;

    printf("\t>>>ADD LOG(CUPTI_DESTROY)\n");
    */

    profiled = 1;

  }else{

    res = mocu.mocudaLaunch(func);

  }

  LEAVE;

  return res;

}


cudaError_t cudaFuncGetAttributes(struct cudaFuncAttributes *attr,  const void *func){

  TRACE("cudaFuncGetAttributes");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaFuncGetAttributes(attr, func);

  LEAVE;

  return res;

}


cudaError_t cudaSetDoubleForDevice(double *d){

  TRACE("cudaSetDoubleForDevice");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaSetDoubleForDevice(d);

  LEAVE;

  return res;

}


cudaError_t cudaSetDoubleForHost(double *d){

  TRACE("cudaSetDoubleForHost");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaSetDoubleForHost(d);

  LEAVE;

  return res;

}


cudaError_t cudaMalloc(void **devPtr,  size_t size){

  TRACE("cudaMalloc");

  ENTER;

  mocu_try_to_allocate(size);

  cudaError_t res;
  region* r;
  apilog* a;

  res = mocu.mocudaMalloc(devPtr,size);

  if(res == cudaSuccess){

    mocu_malloc_done(size);

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

    mocu_failed_to_get(size);

    res = mocu.mocudaMalloc(devPtr,size);

  }

  LEAVE;

  return res;

}


cudaError_t cudaMallocHost(void **ptr,  size_t size){

  TRACE("cudaMallocHost");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMallocHost(ptr, size);

  LEAVE;

  return res;

}


cudaError_t cudaMallocPitch(void **devPtr,  size_t *pitch,  size_t width,  size_t height){

  TRACE("cudaMallocPitch");

  ENTER;

  size_t expected_pitch;
  cudaError_t res;
  apilog* a;

  expected_pitch = width*height != 0 ? ((width+511)/512)*512 : 0;

  mocu_try_to_allocate(expected_pitch*height);

  res = mocu.mocudaMallocPitch(devPtr, pitch, width, height);

  if(res == cudaSuccess){

    mocu_malloc_done(expected_pitch*height);

    a = (apilog*)malloc(sizeof(apilog));
    a->type = MALLOCPITCH;
    a->data.mallocPitch.devPtr = *devPtr;
    a->data.mallocPitch.pitch = *pitch;
    a->data.mallocPitch.width = width;
    a->data.mallocPitch.height = height;
    a->prev = mocu.cp->a1->prev;
    a->next = mocu.cp->a1;
    a->prev->next = a;
    a->next->prev = a;

  }

  LEAVE;

  return res;

}


cudaError_t cudaMallocArray(cudaArray_t *array,  const struct cudaChannelFormatDesc *desc,  size_t width,  size_t height ,  unsigned int flags ){

  TRACE("cudaMallocArray");

  ENTER;

  size_t expected;
  mocu_array* arp;
  cudaArray_t ar;
  cudaError_t res;
  
  expected = ((desc->w+desc->x+desc->y+desc->z+7)/8)
    *((width+31)&~(size_t)31)
    *((height+127)&~(size_t)127)
    + (2<<20);

  mocu_try_to_allocate(expected);
  
  res = mocu.mocudaMallocArray(&ar, desc, width, height , flags );

  if(res == cudaSuccess){

    mocu_malloc_done(expected);

    arp = (mocu_array*)malloc(sizeof(mocu_array));
    arp->ar = ar;
    arp->desc = *desc;
    arp->width = width;
    arp->height = height;
    arp->backup_size = width*height*((desc->x+desc->y+desc->z+desc->w+7)/8);
    arp->flags = flags;
    arp->mode = 0;

    arp->prev = mocu.cp->ar1->prev;
    arp->next = mocu.cp->ar1;
    arp->prev->next = arp;
    arp->next->prev = arp;

    *array = (cudaArray_t)arp;

  }

  LEAVE;

  return res;

}


cudaError_t cudaFree(void *devPtr){

  TRACE("cudaFree");

  ENTER;

  apilog *a;
  region *r;
  cudaError_t res;

  res = mocu.mocudaFree(devPtr);

  if(res == cudaSuccess){

    mocu_cuda_free(devPtr);

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
  }

  LEAVE;
  
  return res;

}


cudaError_t cudaFreeHost(void *ptr){

  TRACE("cudaFreeHost");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaFreeHost(ptr);

  LEAVE;

  return res;

}


cudaError_t cudaFreeArray(cudaArray_t array){

  TRACE("cudaFreeArray");

  ENTER;

  mocu_array* arp;
  cudaError_t res;

  arp = (mocu_array*)array;

  res = mocu.mocudaFreeArray(arp->ar);

  arp->next->prev = arp->prev;
  arp->prev->next = arp->next;

  LEAVE;

  return res;

}


cudaError_t cudaFreeMipmappedArray(cudaMipmappedArray_t mipmappedArray){

  TRACE("cudaFreeMipmappedArray");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaFreeMipmappedArray(mipmappedArray);

  LEAVE;

  return res;

}


cudaError_t cudaHostAlloc(void **pHost,  size_t size,  unsigned int flags){

  TRACE("cudaHostAlloc");

  ENTER;

  cudaError_t res;

  if(flags&cudaHostAllocMapped){

    *pHost = valloc(size);

    res = cudaHostRegister(*pHost,size,flags|cudaHostRegisterMapped);

  }else{
    
    res = mocu.mocudaHostAlloc(pHost, size, flags|cudaHostAllocPortable);

  }

  LEAVE;

  return res;

}


cudaError_t cudaHostRegister(void *ptr,  size_t size,  unsigned int flags){

  TRACE("cudaHostRegister");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaHostRegister(ptr, size, flags|cudaHostRegisterPortable);

  apilog* a;

  a = (apilog*)malloc(sizeof(apilog));

  a->type = HOSTREGISTER;
  a->data.hostRegister.ptr = ptr;
  a->data.hostRegister.size = size;
  a->data.hostRegister.flags = flags;
  a->prev = mocu.cp->a1->prev;
  a->next = mocu.cp->a1;
  a->prev->next = a;
  a->next->prev = a;

  LEAVE;

  return res;

}


cudaError_t cudaHostUnregister(void *ptr){

  TRACE("cudaHostUnregister");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaHostUnregister(ptr);

  LEAVE;

  return res;

}


cudaError_t cudaHostGetDevicePointer(void **pDevice,  void *pHost,  unsigned int flags){

  TRACE("cudaHostGetDevicePointer");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaHostGetDevicePointer(pDevice, pHost, flags);

  LEAVE;

  return res;

}


cudaError_t cudaHostGetFlags(unsigned int *pFlags,  void *pHost){

  TRACE("cudaHostGetFlags");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaHostGetFlags(pFlags, pHost);

  LEAVE;

  return res;

}


cudaError_t cudaMalloc3D(struct cudaPitchedPtr* pitchedDevPtr,  struct cudaExtent extent){

  TRACE("cudaMalloc3D");

  ENTER;

  size_t expected_pitch;
  cudaError_t res;
  apilog* a;

  expected_pitch = (extent.width/512+1)*512;

  mocu_try_to_allocate(expected_pitch*extent.height*extent.depth);

  res = mocu.mocudaMalloc3D(pitchedDevPtr,extent);

  if(res == cudaSuccess){

    mocu_malloc_done(expected_pitch*extent.height*extent.depth);

    a = (apilog*)malloc(sizeof(apilog));
    a->type = MALLOC3D;
    a->data.malloc3D.pitchedDevPtr = *pitchedDevPtr;
    a->data.malloc3D.extent = extent;
    a->prev = mocu.cp->a1->prev;
    a->next = mocu.cp->a1;
    a->prev->next = a;
    a->next->prev = a;

  }

  LEAVE;

  return res;

}


cudaError_t cudaMalloc3DArray(cudaArray_t *array,  const struct cudaChannelFormatDesc* desc,  struct cudaExtent extent,  unsigned int flags ){

  TRACE("cudaMalloc3DArray");

  ENTER;

  mocu_array* arp;
  cudaArray_t ar;
  cudaError_t res;

  res = mocu.mocudaMalloc3DArray(&ar, desc, extent, flags);

  arp = (mocu_array*)malloc(sizeof(mocu_array));
  arp->ar = ar;
  arp->desc = *desc;
  arp->extent = extent;
  arp->backup_size = 
    extent.width*extent.height*extent.depth*((desc->x+desc->y+desc->z+desc->w+7)/8);
  arp->flags = flags;
  arp->mode = 1;
  arp->prev->next = arp;
  arp->next->prev = arp;

  *array = (cudaArray_t)arp;

  LEAVE;

  return res;

}


cudaError_t cudaMallocMipmappedArray(cudaMipmappedArray_t *mipmappedArray,  const struct cudaChannelFormatDesc* desc,  struct cudaExtent extent,  unsigned int numLevels,  unsigned int flags ){

  TRACE("cudaMallocMipmappedArray");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMallocMipmappedArray(mipmappedArray, desc, extent, numLevels, flags );

  LEAVE;

  return res;

}


cudaError_t cudaGetMipmappedArrayLevel(cudaArray_t *levelArray,  cudaMipmappedArray_const_t mipmappedArray,  unsigned int level){

  TRACE("cudaGetMipmappedArrayLevel");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGetMipmappedArrayLevel(levelArray, mipmappedArray, level);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpy3D(const struct cudaMemcpy3DParms *p){

  TRACE("cudaMemcpy3D");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpy3D(p);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpy3DPeer(const struct cudaMemcpy3DPeerParms *p){

  TRACE("cudaMemcpy3DPeer");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpy3DPeer(p);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpy3DAsync(const struct cudaMemcpy3DParms *p,  cudaStream_t stream ){

  TRACE("cudaMemcpy3DAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpy3DAsync(p, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemcpy3DPeerAsync(const struct cudaMemcpy3DPeerParms *p,  cudaStream_t stream ){

  TRACE("cudaMemcpy3DPeerAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpy3DPeerAsync(p, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemGetInfo(size_t *free,  size_t *total){

  TRACE("cudaMemGetInfo");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemGetInfo(free, total);

  LEAVE;

  return res;

}


cudaError_t cudaArrayGetInfo(struct cudaChannelFormatDesc *desc,  struct cudaExtent *extent,  unsigned int *flags,  cudaArray_t array){

  TRACE("cudaArrayGetInfo");

  ENTER;

  mocu_array* arp;
  cudaError_t res;

  arp = (mocu_array*)array;

  res = mocu.mocudaArrayGetInfo(desc, extent, flags, arp->ar);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpy(void *dst,  const void *src,  size_t count,  enum cudaMemcpyKind kind){

  //  TRACE("cudaMemcpy");

  ENTER;

  //  printf("\t\tcudaMemcpy (DIVICE FLAG CHECK)\n");

  region* rtemp;
  cudaError_t res;

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

  res = mocu.mocudaMemcpy(dst, src, count, kind);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyPeer(void *dst,  int dstDevice,  const void *src,  int srcDevice,  size_t count){

  TRACE("cudaMemcpyPeer");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpyPeer(dst, dstDevice, src, srcDevice, count);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyToArray(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t count,  enum cudaMemcpyKind kind){

  TRACE("cudaMemcpyToArray");

  ENTER;

  cudaError_t res;
  mocu_array* arp;

  arp = (mocu_array*)dst;

  res = mocu.mocudaMemcpyToArray(arp->ar , wOffset, hOffset, src, count, kind);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyFromArray(void *dst,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t count,  enum cudaMemcpyKind kind){

  TRACE("cudaMemcpyFromArray");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpyFromArray(dst, src, wOffset, hOffset, count, kind);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyArrayToArray(cudaArray_t dst,  size_t wOffsetDst,  size_t hOffsetDst,  cudaArray_const_t src,  size_t wOffsetSrc,  size_t hOffsetSrc,  size_t count,  enum cudaMemcpyKind kind ){

  TRACE("cudaMemcpyArrayToArray");

  ENTER;

  cudaError_t res;
  mocu_array* arp;

  arp = (mocu_array*)dst;

  res = mocu.mocudaMemcpyArrayToArray(arp->ar , wOffsetDst, hOffsetDst, src, wOffsetSrc, hOffsetSrc, count, kind );
  LEAVE;

  return res;

}


cudaError_t cudaMemcpy2D(void *dst,  size_t dpitch,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind){

  TRACE("cudaMemcpy2D");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpy2D(dst, dpitch, src, spitch, width, height, kind);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpy2DToArray(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind){

  TRACE("cudaMemcpy2DToArray");

  ENTER;

  cudaError_t res;
  mocu_array* arp;

  arp = (mocu_array*)dst;

  res = mocu.mocudaMemcpy2DToArray(arp->ar, wOffset, hOffset, src, spitch, width, height, kind);

  LEAVE;

  return res;

}

cudaError_t cudaMemcpy2DFromArray(void *dst,  size_t dpitch,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t width,  size_t height,  enum cudaMemcpyKind kind){

  TRACE("cudaMemcpy2DFromArray");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpy2DFromArray(dst, dpitch, src, wOffset, hOffset, width, height, kind);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpy2DArrayToArray(cudaArray_t dst,  size_t wOffsetDst,  size_t hOffsetDst,  cudaArray_const_t src,  size_t wOffsetSrc,  size_t hOffsetSrc,  size_t width,  size_t height,  enum cudaMemcpyKind kind ){

  TRACE("cudaMemcpy2DArrayToArray");

  ENTER;

  cudaError_t res;
  mocu_array* arp;

  arp = (mocu_array*)dst;

  res = mocu.mocudaMemcpy2DArrayToArray(arp->ar, wOffsetDst, hOffsetDst, src, wOffsetSrc, hOffsetSrc, width, height, kind );

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyToSymbol(const void *symbol,  const void *src,  size_t count,  size_t offset ,  enum cudaMemcpyKind kind ){

  TRACE("cudaMemcpyToSymbol");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpyToSymbol(symbol,src,count,offset,kind);

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyFromSymbol(void *dst,  const void *symbol,  size_t count,  size_t offset ,  enum cudaMemcpyKind kind ){

  TRACE("cudaMemcpyFromSymbol");

  ENTER;

#if 0
  cudaError_t res;

  res = mocu.mocudaMemcpyFromSymbol(dst, symbol, count, offset , kind );

  if(res == cudaErrorMemoryAllocation){

    mocu_failed_to_get(mocu_get_amount_of_memory_used());
    res = mocu.mocudaMemcpyFromSymbol(dst, symbol, count, offset , kind );

  }

  LEAVE;

  return res;
#endif

  cudaError_t res;

  res = mocu.mocudaMemcpyFromSymbol(dst, symbol, count, offset , kind );

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyAsync(void *dst,  const void *src,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t iStream ){

  TRACE("cudaMemcpyAsync");

  ENTER;

  mocu_stream *sp;
  cudaError_t res;
    
  sp = (mocu_stream*)iStream;

  res = mocu.mocudaMemcpyAsync(dst, src, count, kind, sp != NULL ? sp->s : NULL);

  LEAVE;

  return res;
}


cudaError_t cudaMemcpyPeerAsync(void *dst,  int dstDevice,  const void *src,  int srcDevice,  size_t count,  cudaStream_t stream ){

  TRACE("cudaMemcpyPeerAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpyPeerAsync(dst, dstDevice, src, srcDevice, count, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyToArrayAsync(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpyToArrayAsync");

  ENTER;

  cudaError_t res;
  mocu_array* arp;

  arp = (mocu_array*)dst;

  res = mocu.mocudaMemcpyToArrayAsync(arp->ar, wOffset, hOffset, src, count, kind, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyFromArrayAsync(void *dst,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpyFromArrayAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpyFromArrayAsync(dst, src, wOffset, hOffset, count, kind, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemcpy2DAsync(void *dst,  size_t dpitch,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpy2DAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpy2DAsync(dst, dpitch, src, spitch, width, height, kind, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemcpy2DToArrayAsync(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpy2DToArrayAsync");

  ENTER;

  cudaError_t res;
  mocu_array* arp;

  arp = (mocu_array*)dst;

  res = mocu.mocudaMemcpy2DToArrayAsync(arp->ar , wOffset, hOffset, src, spitch, width, height, kind, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemcpy2DFromArrayAsync(void *dst,  size_t dpitch,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpy2DFromArrayAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpy2DFromArrayAsync(dst, dpitch, src, wOffset, hOffset, width, height, kind, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyToSymbolAsync(const void *symbol,  const void *src,  size_t count,  size_t offset,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpyToSymbolAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpyToSymbolAsync(symbol, src, count, offset, kind, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemcpyFromSymbolAsync(void *dst,  const void *symbol,  size_t count,  size_t offset,  enum cudaMemcpyKind kind,  cudaStream_t stream ){

  TRACE("cudaMemcpyFromSymbolAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemcpyFromSymbolAsync(dst, symbol, count, offset, kind, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemset(void *devPtr,  int value,  size_t count){

  TRACE("cudaMemset");

  ENTER;

  region* rtemp;
  cudaError_t res;

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
  
  res = mocu.mocudaMemset(devPtr, value, count);
 
  LEAVE;

  return res;

}


cudaError_t cudaMemset2D(void *devPtr,  size_t pitch,  int value,  size_t width,  size_t height){

  TRACE("cudaMemset2D");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemset2D(devPtr, pitch, value, width, height);

  LEAVE;

  return res;

}


cudaError_t cudaMemset3D(struct cudaPitchedPtr pitchedDevPtr,  int value,  struct cudaExtent extent){

  TRACE("cudaMemset3D");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemset3D(pitchedDevPtr, value, extent);

  LEAVE;

  return res;

}


cudaError_t cudaMemsetAsync(void *devPtr,  int value,  size_t count,  cudaStream_t stream ){

  TRACE("cudaMemsetAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemsetAsync(devPtr, value, count, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemset2DAsync(void *devPtr,  size_t pitch,  int value,  size_t width,  size_t height,  cudaStream_t stream ){

  TRACE("cudaMemset2DAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemset2DAsync(devPtr, pitch, value, width, height, stream );

  LEAVE;

  return res;

}


cudaError_t cudaMemset3DAsync(struct cudaPitchedPtr pitchedDevPtr,  int value,  struct cudaExtent extent,  cudaStream_t stream ){

  TRACE("cudaMemset3DAsync");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaMemset3DAsync(pitchedDevPtr, value, extent, stream );

  LEAVE;

  return res;

}


cudaError_t cudaGetSymbolAddress(void **devPtr,  const void *symbol){

  TRACE("cudaGetSymbolAddress");

  ENTER;

#if 0

  cudaError_t res;

  res = mocu.mocudaGetSymbolAddress(devPtr, symbol);

  if(res == cudaErrorMemoryAllocation){
    
    mocu_failed_to_get(mocu_get_amount_of_memory_used());
    res = mocu.mocudaGetSymbolAddress(devPtr, symbol);

  }

  LEAVE;

  return res;

#endif

  cudaError_t res;

  res = mocu.mocudaGetSymbolAddress(devPtr, symbol);

  LEAVE;

  return res;

}


cudaError_t cudaGetSymbolSize(size_t *size,  const void *symbol){

  TRACE("cudaGetSymbolSize");

  ENTER;

#if 0
  cudaError_t res;

  res = mocu.mocudaGetSymbolSize(size, symbol);

  if(res == cudaErrorMemoryAllocation){

    mocu_failed_to_get(mocu_get_amount_of_memory_used());
    res = mocu.mocudaGetSymbolSize(size, symbol);

  }

  LEAVE;

  return res;
#endif

  cudaError_t res;

  res = mocu.mocudaGetSymbolSize(size, symbol);

  LEAVE;

  return res;

}


cudaError_t cudaPointerGetAttributes(struct cudaPointerAttributes *attributes,  const void *ptr){

  TRACE("cudaPointerGetAttributes");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaPointerGetAttributes(attributes, ptr);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceCanAccessPeer(int *canAccessPeer,  int device,  int peerDevice){

  TRACE("cudaDeviceCanAccessPeer");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceCanAccessPeer(canAccessPeer, device, peerDevice);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceEnablePeerAccess(int peerDevice,  unsigned int flags){

  TRACE("cudaDeviceEnablePeerAccess");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceEnablePeerAccess(peerDevice, flags);

  LEAVE;

  return res;

}


cudaError_t cudaDeviceDisablePeerAccess(int peerDevice){

  TRACE("cudaDeviceDisablePeerAccess");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDeviceDisablePeerAccess(peerDevice);

  LEAVE;

  return res;

}


cudaError_t cudaGraphicsUnregisterResource(cudaGraphicsResource_t resource){

  TRACE("cudaGraphicsUnregisterResource");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGraphicsUnregisterResource(resource);

  LEAVE;

  return res;

}


cudaError_t cudaGraphicsResourceSetMapFlags(cudaGraphicsResource_t resource,  unsigned int flags){

  TRACE("cudaGraphicsResourceSetMapFlags");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGraphicsResourceSetMapFlags(resource, flags);

  LEAVE;

  return res;

}


cudaError_t cudaGraphicsMapResources(int count,  cudaGraphicsResource_t *resources,  cudaStream_t stream ){

  TRACE("cudaGraphicsMapResources");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGraphicsMapResources(count, resources, stream );

  LEAVE;

  return res;

}


cudaError_t cudaGraphicsUnmapResources(int count,  cudaGraphicsResource_t *resources,  cudaStream_t stream ){

  TRACE("cudaGraphicsUnmapResources");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGraphicsUnmapResources(count, resources, stream );

  LEAVE;

  return res;

}


cudaError_t cudaGraphicsResourceGetMappedPointer(void **devPtr,  size_t *size,  cudaGraphicsResource_t resource){

  TRACE("cudaGraphicsResourceGetMappedPointer");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGraphicsResourceGetMappedPointer(devPtr, size, resource);

  LEAVE;

  return res;

}


cudaError_t cudaGraphicsSubResourceGetMappedArray(cudaArray_t *array,  cudaGraphicsResource_t resource,  unsigned int arrayIndex,  unsigned int mipLevel){

  TRACE("cudaGraphicsSubResourceGetMappedArray");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGraphicsSubResourceGetMappedArray(array, resource, arrayIndex, mipLevel);

  LEAVE;

  return res;

}


cudaError_t cudaGraphicsResourceGetMappedMipmappedArray(cudaMipmappedArray_t *mipmappedArray,  cudaGraphicsResource_t resource){

  TRACE("cudaGraphicsResourceGetMappedMipmappedArray");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGraphicsResourceGetMappedMipmappedArray(mipmappedArray, resource);

  LEAVE;

  return res;

}


cudaError_t cudaGetChannelDesc(struct cudaChannelFormatDesc *desc,  cudaArray_const_t array){

  TRACE("cudaGetChannelDesc");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGetChannelDesc(desc, array);

  LEAVE;

  return res;

}


struct cudaChannelFormatDesc cudaCreateChannelDesc(int x,  int y,  int z,  int w,  enum cudaChannelFormatKind f){

  TRACE("cudaCreateChannelDesc");

  ENTER;

  struct cudaChannelFormatDesc res;

  res = mocu.mocudaCreateChannelDesc(x, y, z, w, f);

  LEAVE;

  return res;

}


cudaError_t cudaBindTexture(size_t *offset,  const struct textureReference *texref,  const void *devPtr,  const struct cudaChannelFormatDesc *desc,  size_t size ){

  TRACE("cudaBindTexture");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaBindTexture(offset, texref, devPtr, desc, size );

  LEAVE;

  return res;

}


cudaError_t cudaBindTexture2D(size_t *offset,  const struct textureReference *texref,  const void *devPtr,  const struct cudaChannelFormatDesc *desc,  size_t width,  size_t height,  size_t pitch){

  TRACE("cudaBindTexture2D");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaBindTexture2D(offset, texref, devPtr, desc, width, height, pitch);

  LEAVE;

  return res;

}


cudaError_t cudaBindTextureToArray(const struct textureReference *texref,  cudaArray_const_t array,  const struct cudaChannelFormatDesc *desc){

  TRACE("cudaBindTextureToArray");

  ENTER;

  cudaError_t res;
  mocu_array *arp;

  arp = (mocu_array*)array;

  //  res = mocu.mocudaBindTextureToArray(texref, array, desc);
  res = mocu.mocudaBindTextureToArray(texref, arp->ar, desc);

  LEAVE;

  return res;

}


cudaError_t cudaBindTextureToMipmappedArray(const struct textureReference *texref,  cudaMipmappedArray_const_t mipmappedArray,  const struct cudaChannelFormatDesc *desc){

  TRACE("cudaBindTextureToMipmappedArray");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaBindTextureToMipmappedArray(texref, mipmappedArray, desc);

  LEAVE;

  return res;

}


cudaError_t cudaUnbindTexture(const struct textureReference *texref){

  TRACE("cudaUnbindTexture");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaUnbindTexture(texref);

  LEAVE;

  return res;

}


cudaError_t cudaGetTextureAlignmentOffset(size_t *offset,  const struct textureReference *texref){

  TRACE("cudaGetTextureAlignmentOffset");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGetTextureAlignmentOffset(offset, texref);

  LEAVE;

  return res;

}


cudaError_t cudaGetTextureReference(const struct textureReference **texref,  const void *symbol){

  TRACE("cudaGetTextureReference");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGetTextureReference(texref, symbol);

  LEAVE;

  return res;

}


cudaError_t cudaBindSurfaceToArray(const struct surfaceReference *surfref,  cudaArray_const_t array,  const struct cudaChannelFormatDesc *desc){

  TRACE("cudaBindSurfaceToArray");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaBindSurfaceToArray(surfref, array, desc);

  LEAVE;

  return res;

}


cudaError_t cudaGetSurfaceReference(const struct surfaceReference **surfref,  const void *symbol){

  TRACE("cudaGetSurfaceReference");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGetSurfaceReference(surfref, symbol);

  LEAVE;

  return res;

}


cudaError_t cudaCreateTextureObject(cudaTextureObject_t *pTexObject,  const struct cudaResourceDesc *pResDesc,  const struct cudaTextureDesc *pTexDesc,  const struct cudaResourceViewDesc *pResViewDesc){

  TRACE("cudaCreateTextureObject");

  ENTER;

  cudaError_t res;
  mocu_texture* tp;
  cudaTextureObject_t t;
  struct cudaResourceDesc rdesc;
  struct cudaTextureDesc tdesc;
  struct cudaResourceViewDesc vdesc;

  //  res = mocu.mocudaCreateTextureObject(pTexObject, pResDesc, pTexDesc, pResViewDesc);
  res = mocu.mocudaCreateTextureObject(&t, pResDesc, pTexDesc, pResViewDesc);

  rdesc = (struct cudaResourceDesc)*pResDesc;
  tdesc = (struct cudaTextureDesc)*pTexDesc;
  vdesc = (struct cudaResourceViewDesc)*pResViewDesc;

  if(res == cudaSuccess){

    tp = (mocu_texture*)malloc(sizeof(mocu_texture));
    tp->tx = t;
    tp->rdesc = rdesc;
    tp->tdesc = tdesc;
    tp->vdesc = vdesc;
    
    tp->prev = mocu.cp->tx1->prev;
    tp->next = mocu.cp->tx1;
    tp->prev->next = tp;
    tp->next->prev = tp;
    
  }

  LEAVE;

  return res;

}


cudaError_t cudaDestroyTextureObject(cudaTextureObject_t texObject){

  TRACE("cudaDestroyTextureObject");

  ENTER;

  cudaError_t res;
  mocu_texture *tp;

  tp = (mocu_texture*)texObject;

  res = mocu.mocudaDestroyTextureObject(tp->tx);

  if(res == cudaSuccess){

    tp->next->prev = tp->prev;
    tp->prev->next = tp->next;
    
    free(tp);
  }

  LEAVE;

  return res;

}


cudaError_t cudaGetTextureObjectResourceDesc(struct cudaResourceDesc *pResDesc,  cudaTextureObject_t texObject){

  TRACE("cudaGetTextureObjectResourceDesc");

  ENTER;

  cudaError_t res;
  mocu_texture *tp;

  tp = (mocu_texture*)texObject;

  res = mocu.mocudaGetTextureObjectResourceDesc(pResDesc, tp->tx);

  LEAVE;

  return res;

}


cudaError_t cudaGetTextureObjectTextureDesc(struct cudaTextureDesc *pTexDesc,  cudaTextureObject_t texObject){

  TRACE("cudaGetTextureObjectTextureDesc");

  ENTER;

  cudaError_t res;
  mocu_texture *tp;

  tp = (mocu_texture*)texObject;

  res = mocu.mocudaGetTextureObjectTextureDesc(pTexDesc, tp->tx);

  LEAVE;

  return res;

}


cudaError_t cudaGetTextureObjectResourceViewDesc(struct cudaResourceViewDesc *pResViewDesc,  cudaTextureObject_t texObject){

  TRACE("cudaGetTextureObjectResourceViewDesc");

  ENTER;

  cudaError_t res;
  mocu_texture *tp;

  tp = (mocu_texture*)texObject;

  res = mocu.mocudaGetTextureObjectResourceViewDesc(pResViewDesc, tp->tx);

  LEAVE;

  return res;

}


cudaError_t cudaCreateSurfaceObject(cudaSurfaceObject_t *pSurfObject,  const struct cudaResourceDesc *pResDesc){

  TRACE("cudaCreateSurfaceObject");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaCreateSurfaceObject(pSurfObject, pResDesc);

  LEAVE;

  return res;

}


cudaError_t cudaDestroySurfaceObject(cudaSurfaceObject_t surfObject){

  TRACE("cudaDestroySurfaceObject");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDestroySurfaceObject(surfObject);

  LEAVE;

  return res;

}


cudaError_t cudaGetSurfaceObjectResourceDesc(struct cudaResourceDesc *pResDesc,  cudaSurfaceObject_t surfObject){

  TRACE("cudaGetSurfaceObjectResourceDesc");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGetSurfaceObjectResourceDesc(pResDesc, surfObject);

  LEAVE;

  return res;

}


cudaError_t cudaDriverGetVersion(int *driverVersion){

  TRACE("cudaDriverGetVersion");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaDriverGetVersion(driverVersion);

  LEAVE;

  return res;

}


cudaError_t cudaRuntimeGetVersion(int *runtimeVersion){

  TRACE("cudaRuntimeGetVersion");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaRuntimeGetVersion(runtimeVersion);

  LEAVE;

  return res;

}


cudaError_t cudaGetExportTable(const void **ppExportTable,  const cudaUUID_t *pExportTableId){

  TRACE("cudaGetExportTable");

  ENTER;

  cudaError_t res;

  res = mocu.mocudaGetExportTable(ppExportTable, pExportTableId);

  LEAVE;

  return res;

}
