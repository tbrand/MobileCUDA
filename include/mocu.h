#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/stat.h>

#include <driver_types.h>
#include <cuda_runtime_api.h>
#include <vector_types.h>
#include <signal.h>
#include <__cudaFatFormat.h>

#include "comm.h"

typedef struct _stream{
  struct _stream *prev,*next;
  cudaStream_t s;
  unsigned int flags;
  int mode;
} mocu_stream;

typedef struct _event{
  struct _event *prev,*next;
  cudaEvent_t e;
  unsigned int flags;
  int mode;
  float charge;
} mocu_event;

typedef struct _array{
  struct _array *prev,*next;
  cudaArray_t ar;
  char* backup;
  size_t width;
  size_t height;
  size_t backup_size;
  unsigned int flags;
  struct cudaExtent extent;
  struct cudaChannelFormatDesc desc;
  int mode;
  /*
    mode
    0 : cudaMallocArray
    1 : cudaMalloc3DArray
  */
} mocu_array;

typedef struct _textureObject{
  struct _textureObject *prev,*next;
  cudaTextureObject_t tx;
  struct cudaResourceDesc rdesc;
  struct cudaTextureDesc tdesc;
  struct cudaResourceViewDesc vdesc;
} mocu_texture;

typedef struct _region{
  struct _region *prev,*next;
  size_t size;
  void *devPtr;
  void *base;
  char* backup;
  int mode;
  int flag;
} region;

typedef struct _symbol{

  struct _symbol *prev,*next;

#if 0
  char* sym;
  char* backup;
  size_t size;
  void *devPtr;
#endif

  char* backup;
  char* hostVar;
  char* deviceAddress;
  char* deviceName;
  int ext;
  int size;
  int constant;
  int global;

} symbol;


typedef struct{
  void* devPtr;
  size_t size;
} Malloc;

typedef struct{
  void* devPtr;
} Free;

typedef struct{
  void* fatCubin;
  void** fatCubinHandle;
} RegisterFatBinary;

typedef struct{
  void** fatCubinHandle;
  const char* hostFun;
  char* deviceFun;
  const char* deviceName;
  int thread_limit;
  void* tid;
  void* bid;
  void* bDim;
  void* gDim;
  int*  wSize;
} RegisterFunction;

typedef struct{
  char *hostVar;
  char *deviceAddress;
  char *deviceName;
  int ext;
  int size;
  int constant;
  int global;
} RegisterVar;

typedef struct{
  void* ptr;
  size_t size;
  unsigned int flags;
} HostRegister;

typedef struct{
  void *devPtr;
  size_t pitch;
  size_t width;
  size_t height;
} MallocPitch;

typedef struct{
  struct cudaPitchedPtr pitchedDevPtr;
  struct cudaExtent extent;
} Malloc3D;

enum{
  MALLOC,
  MALLOCPITCH,
  MALLOC3D,
  FREE,
  REGISTERFATBINARY,
  REGISTERFUNCTION,
  REGISTERVAR,
  HOSTREGISTER,
  CUPTI_INIT,
  CUPTI_DESTROY,
};

typedef struct _apilog{
  struct _apilog *prev,*next;
  unsigned int type;
  union{
    Malloc malloc;
    MallocPitch mallocPitch;
    Malloc3D malloc3D;
    Free free;
    RegisterFatBinary registerFatBinary;
    RegisterFunction registerFunction;
    RegisterVar registerVar;
    HostRegister hostRegister;
  } data;
} apilog;

typedef struct _context{
  mocu_stream *s0,*s1;
  mocu_event *e0,*e1;
  mocu_array *ar0,*ar1;
  mocu_texture *tx0,*tx1;
  region *d0,*d1;
  apilog *a0,*a1;
  symbol *sm0,*sm1;
  proc_data *msg,*rmsg;
} context;

typedef struct _mocu{
  context* cp;

  cudaError_t (*mocudaDeviceReset)(void);
  cudaError_t (*mocudaDeviceSynchronize)(void);
  cudaError_t (*mocudaDeviceSetLimit)(enum cudaLimit limit,  size_t value);
  cudaError_t (*mocudaDeviceGetLimit)(size_t *pValue,  enum cudaLimit limit);
  cudaError_t (*mocudaDeviceGetCacheConfig)(enum cudaFuncCache *pCacheConfig);
  cudaError_t (*mocudaDeviceSetCacheConfig)(enum cudaFuncCache cacheConfig);
  cudaError_t (*mocudaDeviceGetSharedMemConfig)(enum cudaSharedMemConfig *pConfig);
  cudaError_t (*mocudaDeviceSetSharedMemConfig)(enum cudaSharedMemConfig config);
  cudaError_t (*mocudaDeviceGetByPCIBusId)(int *device,  char *pciBusId);
  cudaError_t (*mocudaDeviceGetPCIBusId)(char *pciBusId,  int len,  int device);
  cudaError_t (*mocudaIpcGetEventHandle)(cudaIpcEventHandle_t *handle,  cudaEvent_t event);
  cudaError_t (*mocudaIpcOpenEventHandle)(cudaEvent_t *event,  cudaIpcEventHandle_t handle);
  cudaError_t (*mocudaIpcGetMemHandle)(cudaIpcMemHandle_t *handle,  void *devPtr);
  cudaError_t (*mocudaIpcOpenMemHandle)(void **devPtr,  cudaIpcMemHandle_t handle,  unsigned int flags);
  cudaError_t (*mocudaIpcCloseMemHandle)(void *devPtr);
  cudaError_t (*mocudaThreadExit)(void);
  cudaError_t (*mocudaThreadSynchronize)(void);
  cudaError_t (*mocudaThreadSetLimit)(enum cudaLimit limit,  size_t value);
  cudaError_t (*mocudaThreadGetLimit)(size_t *pValue,  enum cudaLimit limit);
  cudaError_t (*mocudaThreadGetCacheConfig)(enum cudaFuncCache *pCacheConfig);
  cudaError_t (*mocudaThreadSetCacheConfig)(enum cudaFuncCache cacheConfig);
  cudaError_t (*mocudaGetLastError)(void);
  cudaError_t (*mocudaPeekAtLastError)(void);
  const char* (*mocudaGetErrorString)(cudaError_t error);
  cudaError_t (*mocudaGetDeviceCount)(int *count);
  cudaError_t (*mocudaGetDeviceProperties)(struct cudaDeviceProp *prop,  int device);
  cudaError_t (*mocudaDeviceGetAttribute)(int *value,  enum cudaDeviceAttr attr,  int device);
  cudaError_t (*mocudaChooseDevice)(int *device,  const struct cudaDeviceProp *prop);
  cudaError_t (*mocudaSetDevice)(int device);
  cudaError_t (*mocudaGetDevice)(int *device);
  cudaError_t (*mocudaSetValidDevices)(int *device_arr,  int len);
  cudaError_t (*mocudaSetDeviceFlags)( unsigned int flags );
  cudaError_t (*mocudaStreamCreate)(cudaStream_t *pStream);
  cudaError_t (*mocudaStreamCreateWithFlags)(cudaStream_t *pStream,  unsigned int flags);
  cudaError_t (*mocudaStreamDestroy)(cudaStream_t stream);
  cudaError_t (*mocudaStreamWaitEvent)(cudaStream_t stream,  cudaEvent_t event,  unsigned int flags);
  cudaError_t (*mocudaStreamAddCallback)(cudaStream_t stream, cudaStreamCallback_t callback,  void *userData,  unsigned int flags);
  cudaError_t (*mocudaStreamSynchronize)(cudaStream_t stream);
  cudaError_t (*mocudaStreamQuery)(cudaStream_t stream);
  cudaError_t (*mocudaEventCreate)(cudaEvent_t *event);
  cudaError_t (*mocudaEventCreateWithFlags)(cudaEvent_t *event,  unsigned int flags);
  cudaError_t (*mocudaEventRecord)(cudaEvent_t event,  cudaStream_t stream );
  cudaError_t (*mocudaEventQuery)(cudaEvent_t event);
  cudaError_t (*mocudaEventSynchronize)(cudaEvent_t event);
  cudaError_t (*mocudaEventDestroy)(cudaEvent_t event);
  cudaError_t (*mocudaEventElapsedTime)(float *ms,  cudaEvent_t start,  cudaEvent_t end);
  cudaError_t (*mocudaConfigureCall)(dim3 gridDim,  dim3 blockDim,  size_t sharedMem ,  cudaStream_t stream );
  cudaError_t (*mocudaSetupArgument)(const void *arg,  size_t size,  size_t offset);
  cudaError_t (*mocudaFuncSetCacheConfig)(const void *func,  enum cudaFuncCache cacheConfig);
  cudaError_t (*mocudaFuncSetSharedMemConfig)(const void *func,  enum cudaSharedMemConfig config);
  cudaError_t (*mocudaLaunch)(const void *func);
  cudaError_t (*mocudaFuncGetAttributes)(struct cudaFuncAttributes *attr,  const void *func);
  cudaError_t (*mocudaSetDoubleForDevice)(double *d);
  cudaError_t (*mocudaSetDoubleForHost)(double *d);
  cudaError_t (*mocudaMalloc)(void **devPtr,  size_t size);
  cudaError_t (*mocudaMallocHost)(void **ptr,  size_t size);
  cudaError_t (*mocudaMallocPitch)(void **devPtr,  size_t *pitch,  size_t width,  size_t height);
  cudaError_t (*mocudaMallocArray)(cudaArray_t *array,  const struct cudaChannelFormatDesc *desc,  size_t width,  size_t height ,  unsigned int flags );
  cudaError_t (*mocudaFree)(void *devPtr);
  cudaError_t (*mocudaFreeHost)(void *ptr);
  cudaError_t (*mocudaFreeArray)(cudaArray_t array);
  cudaError_t (*mocudaFreeMipmappedArray)(cudaMipmappedArray_t mipmappedArray);
  cudaError_t (*mocudaHostAlloc)(void **pHost,  size_t size,  unsigned int flags);
  cudaError_t (*mocudaHostRegister)(void *ptr,  size_t size,  unsigned int flags);
  cudaError_t (*mocudaHostUnregister)(void *ptr);
  cudaError_t (*mocudaHostGetDevicePointer)(void **pDevice,  void *pHost,  unsigned int flags);
  cudaError_t (*mocudaHostGetFlags)(unsigned int *pFlags,  void *pHost);
  cudaError_t (*mocudaMalloc3D)(struct cudaPitchedPtr* pitchedDevPtr,  struct cudaExtent extent);
  cudaError_t (*mocudaMalloc3DArray)(cudaArray_t *array,  const struct cudaChannelFormatDesc* desc,  struct cudaExtent extent,  unsigned int flags );
  cudaError_t (*mocudaMallocMipmappedArray)(cudaMipmappedArray_t *mipmappedArray,  const struct cudaChannelFormatDesc* desc,  struct cudaExtent extent,  unsigned int numLevels,  unsigned int flags );
  cudaError_t (*mocudaGetMipmappedArrayLevel)(cudaArray_t *levelArray,  cudaMipmappedArray_const_t mipmappedArray,  unsigned int level);
  cudaError_t (*mocudaMemcpy3D)(const struct cudaMemcpy3DParms *p);
  cudaError_t (*mocudaMemcpy3DPeer)(const struct cudaMemcpy3DPeerParms *p);
  cudaError_t (*mocudaMemcpy3DAsync)(const struct cudaMemcpy3DParms *p,  cudaStream_t stream );
  cudaError_t (*mocudaMemcpy3DPeerAsync)(const struct cudaMemcpy3DPeerParms *p,  cudaStream_t stream );
  cudaError_t (*mocudaMemGetInfo)(size_t *free,  size_t *total);
  cudaError_t (*mocudaArrayGetInfo)(struct cudaChannelFormatDesc *desc,  struct cudaExtent *extent,  unsigned int *flags,  cudaArray_t array);
  cudaError_t (*mocudaMemcpy)(void *dst,  const void *src,  size_t count,  enum cudaMemcpyKind kind);
  cudaError_t (*mocudaMemcpyPeer)(void *dst,  int dstDevice,  const void *src,  int srcDevice,  size_t count);
  cudaError_t (*mocudaMemcpyToArray)(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t count,  enum cudaMemcpyKind kind);
  cudaError_t (*mocudaMemcpyFromArray)(void *dst,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t count,  enum cudaMemcpyKind kind);
  cudaError_t (*mocudaMemcpyArrayToArray)(cudaArray_t dst,  size_t wOffsetDst,  size_t hOffsetDst,  cudaArray_const_t src,  size_t wOffsetSrc,  size_t hOffsetSrc,  size_t count,  enum cudaMemcpyKind kind );
  cudaError_t (*mocudaMemcpy2D)(void *dst,  size_t dpitch,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind);
  cudaError_t (*mocudaMemcpy2DToArray)(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind);
  cudaError_t (*mocudaMemcpy2DFromArray)(void *dst,  size_t dpitch,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t width,  size_t height,  enum cudaMemcpyKind kind);
  cudaError_t (*mocudaMemcpy2DArrayToArray)(cudaArray_t dst,  size_t wOffsetDst,  size_t hOffsetDst,  cudaArray_const_t src,  size_t wOffsetSrc,  size_t hOffsetSrc,  size_t width,  size_t height,  enum cudaMemcpyKind kind );
  cudaError_t (*mocudaMemcpyToSymbol)(const void *symbol,  const void *src,  size_t count,  size_t offset ,  enum cudaMemcpyKind kind );
  cudaError_t (*mocudaMemcpyFromSymbol)(void *dst,  const void *symbol,  size_t count,  size_t offset ,  enum cudaMemcpyKind kind );
  cudaError_t (*mocudaMemcpyAsync)(void *dst,  const void *src,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t stream );
  cudaError_t (*mocudaMemcpyPeerAsync)(void *dst,  int dstDevice,  const void *src,  int srcDevice,  size_t count,  cudaStream_t stream );
  cudaError_t (*mocudaMemcpyToArrayAsync)(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t stream );
  cudaError_t (*mocudaMemcpyFromArrayAsync)(void *dst,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t stream );
  cudaError_t (*mocudaMemcpy2DAsync)(void *dst,  size_t dpitch,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream );
  cudaError_t (*mocudaMemcpy2DToArrayAsync)(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream );
  cudaError_t (*mocudaMemcpy2DFromArrayAsync)(void *dst,  size_t dpitch,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream );
  cudaError_t (*mocudaMemcpyToSymbolAsync)(const void *symbol,  const void *src,  size_t count,  size_t offset,  enum cudaMemcpyKind kind,  cudaStream_t stream );
  cudaError_t (*mocudaMemcpyFromSymbolAsync)(void *dst,  const void *symbol,  size_t count,  size_t offset,  enum cudaMemcpyKind kind,  cudaStream_t stream );
  cudaError_t (*mocudaMemset)(void *devPtr,  int value,  size_t count);
  cudaError_t (*mocudaMemset2D)(void *devPtr,  size_t pitch,  int value,  size_t width,  size_t height);
  cudaError_t (*mocudaMemset3D)(struct cudaPitchedPtr pitchedDevPtr,  int value,  struct cudaExtent extent);
  cudaError_t (*mocudaMemsetAsync)(void *devPtr,  int value,  size_t count,  cudaStream_t stream );
  cudaError_t (*mocudaMemset2DAsync)(void *devPtr,  size_t pitch,  int value,  size_t width,  size_t height,  cudaStream_t stream );
  cudaError_t (*mocudaMemset3DAsync)(struct cudaPitchedPtr pitchedDevPtr,  int value,  struct cudaExtent extent,  cudaStream_t stream );
  cudaError_t (*mocudaGetSymbolAddress)(void **devPtr,  const void *symbol);
  cudaError_t (*mocudaGetSymbolSize)(size_t *size,  const void *symbol);
  cudaError_t (*mocudaPointerGetAttributes)(struct cudaPointerAttributes *attributes,  const void *ptr);
  cudaError_t (*mocudaDeviceCanAccessPeer)(int *canAccessPeer,  int device,  int peerDevice);
  cudaError_t (*mocudaDeviceEnablePeerAccess)(int peerDevice,  unsigned int flags);
  cudaError_t (*mocudaDeviceDisablePeerAccess)(int peerDevice);
  cudaError_t (*mocudaGraphicsUnregisterResource)(cudaGraphicsResource_t resource);
  cudaError_t (*mocudaGraphicsResourceSetMapFlags)(cudaGraphicsResource_t resource,  unsigned int flags);
  cudaError_t (*mocudaGraphicsMapResources)(int count,  cudaGraphicsResource_t *resources,  cudaStream_t stream );
  cudaError_t (*mocudaGraphicsUnmapResources)(int count,  cudaGraphicsResource_t *resources,  cudaStream_t stream );
  cudaError_t (*mocudaGraphicsResourceGetMappedPointer)(void **devPtr,  size_t *size,  cudaGraphicsResource_t resource);
  cudaError_t (*mocudaGraphicsSubResourceGetMappedArray)(cudaArray_t *array,  cudaGraphicsResource_t resource,  unsigned int arrayIndex,  unsigned int mipLevel);
  cudaError_t (*mocudaGraphicsResourceGetMappedMipmappedArray)(cudaMipmappedArray_t *mipmappedArray,  cudaGraphicsResource_t resource);
  cudaError_t (*mocudaGetChannelDesc)(struct cudaChannelFormatDesc *desc,  cudaArray_const_t array);
  struct cudaChannelFormatDesc (*mocudaCreateChannelDesc)(int x,  int y,  int z,  int w,  enum cudaChannelFormatKind f);
  cudaError_t (*mocudaBindTexture)(size_t *offset,  const struct textureReference *texref,  const void *devPtr,  const struct cudaChannelFormatDesc *desc,  size_t size );
  cudaError_t (*mocudaBindTexture2D)(size_t *offset,  const struct textureReference *texref,  const void *devPtr,  const struct cudaChannelFormatDesc *desc,  size_t width,  size_t height,  size_t pitch);
  cudaError_t (*mocudaBindTextureToArray)(const struct textureReference *texref,  cudaArray_const_t array,  const struct cudaChannelFormatDesc *desc);
  cudaError_t (*mocudaBindTextureToMipmappedArray)(const struct textureReference *texref,  cudaMipmappedArray_const_t mipmappedArray,  const struct cudaChannelFormatDesc *desc);
  cudaError_t (*mocudaUnbindTexture)(const struct textureReference *texref);
  cudaError_t (*mocudaGetTextureAlignmentOffset)(size_t *offset,  const struct textureReference *texref);
  cudaError_t (*mocudaGetTextureReference)(const struct textureReference **texref,  const void *symbol);
  cudaError_t (*mocudaBindSurfaceToArray)(const struct surfaceReference *surfref,  cudaArray_const_t array,  const struct cudaChannelFormatDesc *desc);
  cudaError_t (*mocudaGetSurfaceReference)(const struct surfaceReference **surfref,  const void *symbol);
  cudaError_t (*mocudaCreateTextureObject)(cudaTextureObject_t *pTexObject,  const struct cudaResourceDesc *pResDesc,  const struct cudaTextureDesc *pTexDesc,  const struct cudaResourceViewDesc *pResViewDesc);
  cudaError_t (*mocudaDestroyTextureObject)(cudaTextureObject_t texObject);
  cudaError_t (*mocudaGetTextureObjectResourceDesc)(struct cudaResourceDesc *pResDesc,  cudaTextureObject_t texObject);
  cudaError_t (*mocudaGetTextureObjectTextureDesc)(struct cudaTextureDesc *pTexDesc,  cudaTextureObject_t texObject);
  cudaError_t (*mocudaGetTextureObjectResourceViewDesc)(struct cudaResourceViewDesc *pResViewDesc,  cudaTextureObject_t texObject);
  cudaError_t (*mocudaCreateSurfaceObject)(cudaSurfaceObject_t *pSurfObject,  const struct cudaResourceDesc *pResDesc);
  cudaError_t (*mocudaDestroySurfaceObject)(cudaSurfaceObject_t surfObject);
  cudaError_t (*mocudaGetSurfaceObjectResourceDesc)(struct cudaResourceDesc *pResDesc,  cudaSurfaceObject_t surfObject);
  cudaError_t (*mocudaDriverGetVersion)(int *driverVersion);
  cudaError_t (*mocudaRuntimeGetVersion)(int *runtimeVersion);
  cudaError_t (*mocudaGetExportTable)(const void **ppExportTable,  const cudaUUID_t *pExportTableId);
  void** (*__mocudaRegisterFatBinary)(void* fatCubin);
  void (*__mocudaUnregisterFatBinary)(void **fatCubinHandle);
  void (*__mocudaRegisterVar)(void **fatCubinHandle,char *hostVar,char *deviceAddress,const char *deviceName,int ext,int size,int constant,int global);
  void (*__mocudaRegisterTexture)(void **fatCubinHandle,const struct textureReference *hostVar,const void **deviceAddress,const char *deviceName,int dim,int norm,int ext);
  void (*__mocudaRegisterSurface)(void **fatCubinHandle,const struct surfaceReference  *hostVar,const void **deviceAddress,const char *deviceName,int dim,int ext);
  void (*__mocudaRegisterFunction)(void **fatCubinHandle,const char *hostFun,char *deviceFun,const char *deviceName,int thread_limit,uint3 *tid,uint3 *bid,dim3 *bDim,dim3 *gDim,int *wSize);
  void (*__mocudaRegisterShared)(void **fatCubinHandle, void **devicePtr);
  void (*__mocudaRegisterSharedVar)(void **fatCubinHandle,void **devicePtr,size_t size,size_t alignment,int storage);
  int (*__mocudaSynchronizeThreads)(void** one,void* two);
  void (*__mocudaTextureFetch)(const void* tex,void* index,int integer,void* val);
  void (*__mocudaMutexOperation)(int lock);
  cudaError_t (*__mocudaRegisterDeviceFunction)();

} MOCU;

#define CUDA_LIB_PATH  "/usr/local/cuda-5.5/lib64/libcudart.so"  
#define TRACE(func)  //printf("[TRACE -runtime-] %-25s\n",func);
#define REPLAY(func) printf("[REPLAY] %-25s\n",func);
#define ENCAP 1

extern MOCU mocu;
extern int mocu_pos;
extern int created_context;
extern int initialized;
extern int connected;

// migration.c
extern void mocu_backup();
extern void mocu_migrate(int);

//load.c
extern void mocu_load_cuda_lib();
extern int dlclose_cuda();

//event.c
extern void mocu_event_update();
extern void mocu_event_restore();

//device.c
extern unsigned long mocu_get_amount_of_memory_used();
extern void mocu_device_backup();
extern void mocu_device_restore();

//stream.c
extern void mocu_stream_restore();

//array.c
extern void mocu_array_restore();

//replay.c
extern int replay(apilog*);

//signal.c
extern void mocu_request();
extern void mocu_register_signal();

//socket.c
extern int sockfd;
extern void mocu_connect();
extern void mocu_request_from_daemon(proc_data*);
extern void mocu_send_renew();
extern void mocu_failed_to_get(int);
extern void mocu_suspended();
extern void mocu_register_var(size_t);
extern void mocu_try_to_allocate(size_t);
extern void mocu_cuda_free(void*);
extern void mocu_leave();
extern void mocu_malloc_done(size_t);
extern void mocu_mig_done();
extern void mocu_send();
extern void mocu_recv();

//parse.c
extern void mocu_parse(void*);

//symbol.c
extern void mocu_add_symbol(char*,char*,char*,int,int,int,int);
extern void mocu_symbol_backup();
extern void mocu_symbol_restore();
extern int mocu_symbol_registered(symbol*);

//cupti.c
extern void init_cupti();
extern void cupti_destroy();
extern int  getTrace(RuntimeApiTrace_t*);

#define ENTER  mocu_request();
#define FINISH 
#define LEAVE  mocu_leave();
#define SEND mocu_send();
#define RECV mocu_recv();
