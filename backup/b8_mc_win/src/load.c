#include <mocu.h>

void* cuda_handle;

void mocu_load_cuda_lib(){

  cuda_handle = dlopen(CUDA_LIB_PATH,RTLD_LAZY);

  mocu.mocudaDeviceReset= (cudaError_t (*)(void)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceReset");
  mocu.mocudaDeviceSynchronize= (cudaError_t (*)(void)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceSynchronize");
  mocu.mocudaDeviceSetLimit= (cudaError_t (*)(enum cudaLimit limit,  size_t value)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceSetLimit");
  mocu.mocudaDeviceGetLimit= (cudaError_t (*)(size_t *pValue,  enum cudaLimit limit)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceGetLimit");
  mocu.mocudaDeviceGetCacheConfig= (cudaError_t (*)(enum cudaFuncCache *pCacheConfig)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceGetCacheConfig");
  mocu.mocudaDeviceSetCacheConfig= (cudaError_t (*)(enum cudaFuncCache cacheConfig)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceSetCacheConfig");
  mocu.mocudaDeviceGetSharedMemConfig= (cudaError_t (*)(enum cudaSharedMemConfig *pConfig)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceGetSharedMemConfig");
  mocu.mocudaDeviceSetSharedMemConfig= (cudaError_t (*)(enum cudaSharedMemConfig config)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceSetSharedMemConfig");
  mocu.mocudaDeviceGetByPCIBusId= (cudaError_t (*)(int *device,  char *pciBusId)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceGetByPCIBusId");
  mocu.mocudaDeviceGetPCIBusId= (cudaError_t (*)(char *pciBusId,  int len,  int device)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceGetPCIBusId");
  mocu.mocudaIpcGetEventHandle= (cudaError_t (*)(cudaIpcEventHandle_t *handle,  cudaEvent_t event)) (cudaError_t*)dlsym(cuda_handle,"cudaIpcGetEventHandle");
  mocu.mocudaIpcOpenEventHandle= (cudaError_t (*)(cudaEvent_t *event,  cudaIpcEventHandle_t handle)) (cudaError_t*)dlsym(cuda_handle,"cudaIpcOpenEventHandle");
  mocu.mocudaIpcGetMemHandle= (cudaError_t (*)(cudaIpcMemHandle_t *handle,  void *devPtr)) (cudaError_t*)dlsym(cuda_handle,"cudaIpcGetMemHandle");
  mocu.mocudaIpcOpenMemHandle= (cudaError_t (*)(void **devPtr,  cudaIpcMemHandle_t handle,  unsigned int flags)) (cudaError_t*)dlsym(cuda_handle,"cudaIpcOpenMemHandle");
  mocu.mocudaIpcCloseMemHandle= (cudaError_t (*)(void *devPtr)) (cudaError_t*)dlsym(cuda_handle,"cudaIpcCloseMemHandle");
  mocu.mocudaThreadExit= (cudaError_t (*)(void)) (cudaError_t*)dlsym(cuda_handle,"cudaThreadExit");
  mocu.mocudaThreadSynchronize= (cudaError_t (*)(void)) (cudaError_t*)dlsym(cuda_handle,"cudaThreadSynchronize");
  mocu.mocudaThreadSetLimit= (cudaError_t (*)(enum cudaLimit limit,  size_t value)) (cudaError_t*)dlsym(cuda_handle,"cudaThreadSetLimit");
  mocu.mocudaThreadGetLimit= (cudaError_t (*)(size_t *pValue,  enum cudaLimit limit)) (cudaError_t*)dlsym(cuda_handle,"cudaThreadGetLimit");
  mocu.mocudaThreadGetCacheConfig= (cudaError_t (*)(enum cudaFuncCache *pCacheConfig)) (cudaError_t*)dlsym(cuda_handle,"cudaThreadGetCacheConfig");
  mocu.mocudaThreadSetCacheConfig= (cudaError_t (*)(enum cudaFuncCache cacheConfig)) (cudaError_t*)dlsym(cuda_handle,"cudaThreadSetCacheConfig");
  mocu.mocudaGetLastError= (cudaError_t (*)(void)) (cudaError_t*)dlsym(cuda_handle,"cudaGetLastError");
  mocu.mocudaPeekAtLastError= (cudaError_t (*)(void)) (cudaError_t*)dlsym(cuda_handle,"cudaPeekAtLastError");
  mocu.mocudaGetErrorString= (const char* (*)(cudaError_t error)) (const char**)dlsym(cuda_handle,"cudaGetErrorString");
  mocu.mocudaGetDeviceCount= (cudaError_t (*)(int *count)) (cudaError_t*)dlsym(cuda_handle,"cudaGetDeviceCount");
  mocu.mocudaGetDeviceProperties= (cudaError_t (*)(struct cudaDeviceProp *prop,  int device)) (cudaError_t*)dlsym(cuda_handle,"cudaGetDeviceProperties");
  mocu.mocudaDeviceGetAttribute= (cudaError_t (*)(int *value,  enum cudaDeviceAttr attr,  int device)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceGetAttribute");
  mocu.mocudaChooseDevice= (cudaError_t (*)(int *device,  const struct cudaDeviceProp *prop)) (cudaError_t*)dlsym(cuda_handle,"cudaChooseDevice");
  mocu.mocudaSetDevice= (cudaError_t (*)(int device)) (cudaError_t*)dlsym(cuda_handle,"cudaSetDevice");
  mocu.mocudaGetDevice= (cudaError_t (*)(int *device)) (cudaError_t*)dlsym(cuda_handle,"cudaGetDevice");
  mocu.mocudaSetValidDevices= (cudaError_t (*)(int *device_arr,  int len)) (cudaError_t*)dlsym(cuda_handle,"cudaSetValidDevices");
  mocu.mocudaSetDeviceFlags= (cudaError_t (*)( unsigned int flags )) (cudaError_t*)dlsym(cuda_handle,"cudaSetDeviceFlags");
  mocu.mocudaStreamCreate= (cudaError_t (*)(cudaStream_t *pStream)) (cudaError_t*)dlsym(cuda_handle,"cudaStreamCreate");
  mocu.mocudaStreamCreateWithFlags= (cudaError_t (*)(cudaStream_t *pStream,  unsigned int flags)) (cudaError_t*)dlsym(cuda_handle,"cudaStreamCreateWithFlags");
  mocu.mocudaStreamDestroy= (cudaError_t (*)(cudaStream_t stream)) (cudaError_t*)dlsym(cuda_handle,"cudaStreamDestroy");
  mocu.mocudaStreamWaitEvent= (cudaError_t (*)(cudaStream_t stream,  cudaEvent_t event,  unsigned int flags)) (cudaError_t*)dlsym(cuda_handle,"cudaStreamWaitEvent");
  mocu.mocudaStreamAddCallback= (cudaError_t (*)(cudaStream_t stream, cudaStreamCallback_t callback,  void *userData,  unsigned int flags)) (cudaError_t*)dlsym(cuda_handle,"cudaStreamAddCallback");
  mocu.mocudaStreamSynchronize= (cudaError_t (*)(cudaStream_t stream)) (cudaError_t*)dlsym(cuda_handle,"cudaStreamSynchronize");
  mocu.mocudaStreamQuery= (cudaError_t (*)(cudaStream_t stream)) (cudaError_t*)dlsym(cuda_handle,"cudaStreamQuery");
  mocu.mocudaEventCreate= (cudaError_t (*)(cudaEvent_t *event)) (cudaError_t*)dlsym(cuda_handle,"cudaEventCreate");
  mocu.mocudaEventCreateWithFlags= (cudaError_t (*)(cudaEvent_t *event,  unsigned int flags)) (cudaError_t*)dlsym(cuda_handle,"cudaEventCreateWithFlags");
  mocu.mocudaEventRecord= (cudaError_t (*)(cudaEvent_t event,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaEventRecord");
  mocu.mocudaEventQuery= (cudaError_t (*)(cudaEvent_t event)) (cudaError_t*)dlsym(cuda_handle,"cudaEventQuery");
  mocu.mocudaEventSynchronize= (cudaError_t (*)(cudaEvent_t event)) (cudaError_t*)dlsym(cuda_handle,"cudaEventSynchronize");
  mocu.mocudaEventDestroy= (cudaError_t (*)(cudaEvent_t event)) (cudaError_t*)dlsym(cuda_handle,"cudaEventDestroy");
  mocu.mocudaEventElapsedTime= (cudaError_t (*)(float *ms,  cudaEvent_t start,  cudaEvent_t end)) (cudaError_t*)dlsym(cuda_handle,"cudaEventElapsedTime");
  mocu.mocudaConfigureCall= (cudaError_t (*)(dim3 gridDim,  dim3 blockDim,  size_t sharedMem ,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaConfigureCall");
  mocu.mocudaSetupArgument= (cudaError_t (*)(const void *arg,  size_t size,  size_t offset)) (cudaError_t*)dlsym(cuda_handle,"cudaSetupArgument");
  mocu.mocudaFuncSetCacheConfig= (cudaError_t (*)(const void *func,  enum cudaFuncCache cacheConfig)) (cudaError_t*)dlsym(cuda_handle,"cudaFuncSetCacheConfig");
  mocu.mocudaFuncSetSharedMemConfig= (cudaError_t (*)(const void *func,  enum cudaSharedMemConfig config)) (cudaError_t*)dlsym(cuda_handle,"cudaFuncSetSharedMemConfig");
  mocu.mocudaLaunch= (cudaError_t (*)(const void *func)) (cudaError_t*)dlsym(cuda_handle,"cudaLaunch");
  mocu.mocudaFuncGetAttributes= (cudaError_t (*)(struct cudaFuncAttributes *attr,  const void *func)) (cudaError_t*)dlsym(cuda_handle,"cudaFuncGetAttributes");
  mocu.mocudaSetDoubleForDevice= (cudaError_t (*)(double *d)) (cudaError_t*)dlsym(cuda_handle,"cudaSetDoubleForDevice");
  mocu.mocudaSetDoubleForHost= (cudaError_t (*)(double *d)) (cudaError_t*)dlsym(cuda_handle,"cudaSetDoubleForHost");
  mocu.mocudaMalloc= (cudaError_t (*)(void **devPtr,  size_t size)) (cudaError_t*)dlsym(cuda_handle,"cudaMalloc");
  mocu.mocudaMallocHost= (cudaError_t (*)(void **ptr,  size_t size)) (cudaError_t*)dlsym(cuda_handle,"cudaMallocHost");
  mocu.mocudaMallocPitch= (cudaError_t (*)(void **devPtr,  size_t *pitch,  size_t width,  size_t height)) (cudaError_t*)dlsym(cuda_handle,"cudaMallocPitch");
  mocu.mocudaMallocArray= (cudaError_t (*)(cudaArray_t *array,  const struct cudaChannelFormatDesc *desc,  size_t width,  size_t height ,  unsigned int flags )) (cudaError_t*)dlsym(cuda_handle,"cudaMallocArray");
  mocu.mocudaFree= (cudaError_t (*)(void *devPtr)) (cudaError_t*)dlsym(cuda_handle,"cudaFree");
  mocu.mocudaFreeHost= (cudaError_t (*)(void *ptr)) (cudaError_t*)dlsym(cuda_handle,"cudaFreeHost");
  mocu.mocudaFreeArray= (cudaError_t (*)(cudaArray_t array)) (cudaError_t*)dlsym(cuda_handle,"cudaFreeArray");
  mocu.mocudaFreeMipmappedArray= (cudaError_t (*)(cudaMipmappedArray_t mipmappedArray)) (cudaError_t*)dlsym(cuda_handle,"cudaFreeMipmappedArray");
  mocu.mocudaHostAlloc= (cudaError_t (*)(void **pHost,  size_t size,  unsigned int flags)) (cudaError_t*)dlsym(cuda_handle,"cudaHostAlloc");
  mocu.mocudaHostRegister= (cudaError_t (*)(void *ptr,  size_t size,  unsigned int flags)) (cudaError_t*)dlsym(cuda_handle,"cudaHostRegister");
  mocu.mocudaHostUnregister= (cudaError_t (*)(void *ptr)) (cudaError_t*)dlsym(cuda_handle,"cudaHostUnregister");
  mocu.mocudaHostGetDevicePointer= (cudaError_t (*)(void **pDevice,  void *pHost,  unsigned int flags)) (cudaError_t*)dlsym(cuda_handle,"cudaHostGetDevicePointer");
  mocu.mocudaHostGetFlags= (cudaError_t (*)(unsigned int *pFlags,  void *pHost)) (cudaError_t*)dlsym(cuda_handle,"cudaHostGetFlags");
  mocu.mocudaMalloc3D= (cudaError_t (*)(struct cudaPitchedPtr* pitchedDevPtr,  struct cudaExtent extent)) (cudaError_t*)dlsym(cuda_handle,"cudaMalloc3D");
  mocu.mocudaMalloc3DArray= (cudaError_t (*)(cudaArray_t *array,  const struct cudaChannelFormatDesc* desc,  struct cudaExtent extent,  unsigned int flags )) (cudaError_t*)dlsym(cuda_handle,"cudaMalloc3DArray");
  mocu.mocudaMallocMipmappedArray= (cudaError_t (*)(cudaMipmappedArray_t *mipmappedArray,  const struct cudaChannelFormatDesc* desc,  struct cudaExtent extent,  unsigned int numLevels,  unsigned int flags )) (cudaError_t*)dlsym(cuda_handle,"cudaMallocMipmappedArray");
  mocu.mocudaGetMipmappedArrayLevel= (cudaError_t (*)(cudaArray_t *levelArray,  cudaMipmappedArray_const_t mipmappedArray,  unsigned int level)) (cudaError_t*)dlsym(cuda_handle,"cudaGetMipmappedArrayLevel");
  mocu.mocudaMemcpy3D= (cudaError_t (*)(const struct cudaMemcpy3DParms *p)) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy3D");
  mocu.mocudaMemcpy3DPeer= (cudaError_t (*)(const struct cudaMemcpy3DPeerParms *p)) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy3DPeer");
  mocu.mocudaMemcpy3DAsync= (cudaError_t (*)(const struct cudaMemcpy3DParms *p,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy3DAsync");
  mocu.mocudaMemcpy3DPeerAsync= (cudaError_t (*)(const struct cudaMemcpy3DPeerParms *p,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy3DPeerAsync");
  mocu.mocudaMemGetInfo= (cudaError_t (*)(size_t *free,  size_t *total)) (cudaError_t*)dlsym(cuda_handle,"cudaMemGetInfo");
  mocu.mocudaArrayGetInfo= (cudaError_t (*)(struct cudaChannelFormatDesc *desc,  struct cudaExtent *extent,  unsigned int *flags,  cudaArray_t array)) (cudaError_t*)dlsym(cuda_handle,"cudaArrayGetInfo");
  mocu.mocudaMemcpy= (cudaError_t (*)(void *dst,  const void *src,  size_t count,  enum cudaMemcpyKind kind)) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy");
  mocu.mocudaMemcpyPeer= (cudaError_t (*)(void *dst,  int dstDevice,  const void *src,  int srcDevice,  size_t count)) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyPeer");
  mocu.mocudaMemcpyToArray= (cudaError_t (*)(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t count,  enum cudaMemcpyKind kind)) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyToArray");
  mocu.mocudaMemcpyFromArray= (cudaError_t (*)(void *dst,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t count,  enum cudaMemcpyKind kind)) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyFromArray");
  mocu.mocudaMemcpyArrayToArray= (cudaError_t (*)(cudaArray_t dst,  size_t wOffsetDst,  size_t hOffsetDst,  cudaArray_const_t src,  size_t wOffsetSrc,  size_t hOffsetSrc,  size_t count,  enum cudaMemcpyKind kind )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyArrayToArray");
  mocu.mocudaMemcpy2D= (cudaError_t (*)(void *dst,  size_t dpitch,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind)) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy2D");
  mocu.mocudaMemcpy2DToArray= (cudaError_t (*)(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind)) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy2DToArray");
  mocu.mocudaMemcpy2DFromArray= (cudaError_t (*)(void *dst,  size_t dpitch,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t width,  size_t height,  enum cudaMemcpyKind kind)) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy2DFromArray");
  mocu.mocudaMemcpy2DArrayToArray= (cudaError_t (*)(cudaArray_t dst,  size_t wOffsetDst,  size_t hOffsetDst,  cudaArray_const_t src,  size_t wOffsetSrc,  size_t hOffsetSrc,  size_t width,  size_t height,  enum cudaMemcpyKind kind )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy2DArrayToArray");
  mocu.mocudaMemcpyToSymbol= (cudaError_t (*)(const void *symbol,  const void *src,  size_t count,  size_t offset ,  enum cudaMemcpyKind kind )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyToSymbol");
  mocu.mocudaMemcpyFromSymbol= (cudaError_t (*)(void *dst,  const void *symbol,  size_t count,  size_t offset ,  enum cudaMemcpyKind kind )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyFromSymbol");
  mocu.mocudaMemcpyAsync= (cudaError_t (*)(void *dst,  const void *src,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyAsync");
  mocu.mocudaMemcpyPeerAsync= (cudaError_t (*)(void *dst,  int dstDevice,  const void *src,  int srcDevice,  size_t count,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyPeerAsync");
  mocu.mocudaMemcpyToArrayAsync= (cudaError_t (*)(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyToArrayAsync");
  mocu.mocudaMemcpyFromArrayAsync= (cudaError_t (*)(void *dst,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t count,  enum cudaMemcpyKind kind,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyFromArrayAsync");
  mocu.mocudaMemcpy2DAsync= (cudaError_t (*)(void *dst,  size_t dpitch,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy2DAsync");
  mocu.mocudaMemcpy2DToArrayAsync= (cudaError_t (*)(cudaArray_t dst,  size_t wOffset,  size_t hOffset,  const void *src,  size_t spitch,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy2DToArrayAsync");
  mocu.mocudaMemcpy2DFromArrayAsync= (cudaError_t (*)(void *dst,  size_t dpitch,  cudaArray_const_t src,  size_t wOffset,  size_t hOffset,  size_t width,  size_t height,  enum cudaMemcpyKind kind,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpy2DFromArrayAsync");
  mocu.mocudaMemcpyToSymbolAsync= (cudaError_t (*)(const void *symbol,  const void *src,  size_t count,  size_t offset,  enum cudaMemcpyKind kind,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyToSymbolAsync");
  mocu.mocudaMemcpyFromSymbolAsync= (cudaError_t (*)(void *dst,  const void *symbol,  size_t count,  size_t offset,  enum cudaMemcpyKind kind,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemcpyFromSymbolAsync");
  mocu.mocudaMemset= (cudaError_t (*)(void *devPtr,  int value,  size_t count)) (cudaError_t*)dlsym(cuda_handle,"cudaMemset");
  mocu.mocudaMemset2D= (cudaError_t (*)(void *devPtr,  size_t pitch,  int value,  size_t width,  size_t height)) (cudaError_t*)dlsym(cuda_handle,"cudaMemset2D");
  mocu.mocudaMemset3D= (cudaError_t (*)(struct cudaPitchedPtr pitchedDevPtr,  int value,  struct cudaExtent extent)) (cudaError_t*)dlsym(cuda_handle,"cudaMemset3D");
  mocu.mocudaMemsetAsync= (cudaError_t (*)(void *devPtr,  int value,  size_t count,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemsetAsync");
  mocu.mocudaMemset2DAsync= (cudaError_t (*)(void *devPtr,  size_t pitch,  int value,  size_t width,  size_t height,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemset2DAsync");
  mocu.mocudaMemset3DAsync= (cudaError_t (*)(struct cudaPitchedPtr pitchedDevPtr,  int value,  struct cudaExtent extent,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaMemset3DAsync");
  mocu.mocudaGetSymbolAddress= (cudaError_t (*)(void **devPtr,  const void *symbol)) (cudaError_t*)dlsym(cuda_handle,"cudaGetSymbolAddress");
  mocu.mocudaGetSymbolSize= (cudaError_t (*)(size_t *size,  const void *symbol)) (cudaError_t*)dlsym(cuda_handle,"cudaGetSymbolSize");
  mocu.mocudaPointerGetAttributes= (cudaError_t (*)(struct cudaPointerAttributes *attributes,  const void *ptr)) (cudaError_t*)dlsym(cuda_handle,"cudaPointerGetAttributes");
  mocu.mocudaDeviceCanAccessPeer= (cudaError_t (*)(int *canAccessPeer,  int device,  int peerDevice)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceCanAccessPeer");
  mocu.mocudaDeviceEnablePeerAccess= (cudaError_t (*)(int peerDevice,  unsigned int flags)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceEnablePeerAccess");
  mocu.mocudaDeviceDisablePeerAccess= (cudaError_t (*)(int peerDevice)) (cudaError_t*)dlsym(cuda_handle,"cudaDeviceDisablePeerAccess");
  mocu.mocudaGraphicsUnregisterResource= (cudaError_t (*)(cudaGraphicsResource_t resource)) (cudaError_t*)dlsym(cuda_handle,"cudaGraphicsUnregisterResource");
  mocu.mocudaGraphicsResourceSetMapFlags= (cudaError_t (*)(cudaGraphicsResource_t resource,  unsigned int flags)) (cudaError_t*)dlsym(cuda_handle,"cudaGraphicsResourceSetMapFlags");
  mocu.mocudaGraphicsMapResources= (cudaError_t (*)(int count,  cudaGraphicsResource_t *resources,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaGraphicsMapResources");
  mocu.mocudaGraphicsUnmapResources= (cudaError_t (*)(int count,  cudaGraphicsResource_t *resources,  cudaStream_t stream )) (cudaError_t*)dlsym(cuda_handle,"cudaGraphicsUnmapResources");
  mocu.mocudaGraphicsResourceGetMappedPointer= (cudaError_t (*)(void **devPtr,  size_t *size,  cudaGraphicsResource_t resource)) (cudaError_t*)dlsym(cuda_handle,"cudaGraphicsResourceGetMappedPointer");
  mocu.mocudaGraphicsSubResourceGetMappedArray= (cudaError_t (*)(cudaArray_t *array,  cudaGraphicsResource_t resource,  unsigned int arrayIndex,  unsigned int mipLevel)) (cudaError_t*)dlsym(cuda_handle,"cudaGraphicsSubResourceGetMappedArray");
  mocu.mocudaGraphicsResourceGetMappedMipmappedArray= (cudaError_t (*)(cudaMipmappedArray_t *mipmappedArray,  cudaGraphicsResource_t resource)) (cudaError_t*)dlsym(cuda_handle,"cudaGraphicsResourceGetMappedMipmappedArray");
  mocu.mocudaGetChannelDesc= (cudaError_t (*)(struct cudaChannelFormatDesc *desc,  cudaArray_const_t array)) (cudaError_t*)dlsym(cuda_handle,"cudaGetChannelDesc");
  mocu.mocudaCreateChannelDesc= (struct cudaChannelFormatDesc (*)(int x,  int y,  int z,  int w,  enum cudaChannelFormatKind f)) (struct cudaChannelFormatDesc*)dlsym(cuda_handle,"cudaCreateChannelDesc");
  mocu.mocudaBindTexture= (cudaError_t (*)(size_t *offset,  const struct textureReference *texref,  const void *devPtr,  const struct cudaChannelFormatDesc *desc,  size_t size )) (cudaError_t*)dlsym(cuda_handle,"cudaBindTexture");
  mocu.mocudaBindTexture2D= (cudaError_t (*)(size_t *offset,  const struct textureReference *texref,  const void *devPtr,  const struct cudaChannelFormatDesc *desc,  size_t width,  size_t height,  size_t pitch)) (cudaError_t*)dlsym(cuda_handle,"cudaBindTexture2D");
  mocu.mocudaBindTextureToArray= (cudaError_t (*)(const struct textureReference *texref,  cudaArray_const_t array,  const struct cudaChannelFormatDesc *desc)) (cudaError_t*)dlsym(cuda_handle,"cudaBindTextureToArray");
  mocu.mocudaBindTextureToMipmappedArray= (cudaError_t (*)(const struct textureReference *texref,  cudaMipmappedArray_const_t mipmappedArray,  const struct cudaChannelFormatDesc *desc)) (cudaError_t*)dlsym(cuda_handle,"cudaBindTextureToMipmappedArray");
  mocu.mocudaUnbindTexture= (cudaError_t (*)(const struct textureReference *texref)) (cudaError_t*)dlsym(cuda_handle,"cudaUnbindTexture");
  mocu.mocudaGetTextureAlignmentOffset= (cudaError_t (*)(size_t *offset,  const struct textureReference *texref)) (cudaError_t*)dlsym(cuda_handle,"cudaGetTextureAlignmentOffset");
  mocu.mocudaGetTextureReference= (cudaError_t (*)(const struct textureReference **texref,  const void *symbol)) (cudaError_t*)dlsym(cuda_handle,"cudaGetTextureReference");
  mocu.mocudaBindSurfaceToArray= (cudaError_t (*)(const struct surfaceReference *surfref,  cudaArray_const_t array,  const struct cudaChannelFormatDesc *desc)) (cudaError_t*)dlsym(cuda_handle,"cudaBindSurfaceToArray");
  mocu.mocudaGetSurfaceReference= (cudaError_t (*)(const struct surfaceReference **surfref,  const void *symbol)) (cudaError_t*)dlsym(cuda_handle,"cudaGetSurfaceReference");
  mocu.mocudaCreateTextureObject= (cudaError_t (*)(cudaTextureObject_t *pTexObject,  const struct cudaResourceDesc *pResDesc,  const struct cudaTextureDesc *pTexDesc,  const struct cudaResourceViewDesc *pResViewDesc)) (cudaError_t*)dlsym(cuda_handle,"cudaCreateTextureObject");
  mocu.mocudaDestroyTextureObject= (cudaError_t (*)(cudaTextureObject_t texObject)) (cudaError_t*)dlsym(cuda_handle,"cudaDestroyTextureObject");
  mocu.mocudaGetTextureObjectResourceDesc= (cudaError_t (*)(struct cudaResourceDesc *pResDesc,  cudaTextureObject_t texObject)) (cudaError_t*)dlsym(cuda_handle,"cudaGetTextureObjectResourceDesc");
  mocu.mocudaGetTextureObjectTextureDesc= (cudaError_t (*)(struct cudaTextureDesc *pTexDesc,  cudaTextureObject_t texObject)) (cudaError_t*)dlsym(cuda_handle,"cudaGetTextureObjectTextureDesc");
  mocu.mocudaGetTextureObjectResourceViewDesc= (cudaError_t (*)(struct cudaResourceViewDesc *pResViewDesc,  cudaTextureObject_t texObject)) (cudaError_t*)dlsym(cuda_handle,"cudaGetTextureObjectResourceViewDesc");
  mocu.mocudaCreateSurfaceObject= (cudaError_t (*)(cudaSurfaceObject_t *pSurfObject,  const struct cudaResourceDesc *pResDesc)) (cudaError_t*)dlsym(cuda_handle,"cudaCreateSurfaceObject");
  mocu.mocudaDestroySurfaceObject= (cudaError_t (*)(cudaSurfaceObject_t surfObject)) (cudaError_t*)dlsym(cuda_handle,"cudaDestroySurfaceObject");
  mocu.mocudaGetSurfaceObjectResourceDesc= (cudaError_t (*)(struct cudaResourceDesc *pResDesc,  cudaSurfaceObject_t surfObject)) (cudaError_t*)dlsym(cuda_handle,"cudaGetSurfaceObjectResourceDesc");
  mocu.mocudaDriverGetVersion= (cudaError_t (*)(int *driverVersion)) (cudaError_t*)dlsym(cuda_handle,"cudaDriverGetVersion");
  mocu.mocudaRuntimeGetVersion= (cudaError_t (*)(int *runtimeVersion)) (cudaError_t*)dlsym(cuda_handle,"cudaRuntimeGetVersion");
  mocu.mocudaGetExportTable= (cudaError_t (*)(const void **ppExportTable,  const cudaUUID_t *pExportTableId)) (cudaError_t*)dlsym(cuda_handle,"cudaGetExportTable");
  mocu.__mocudaRegisterVar = (void (*)(void **fatCubinHandle,char *hostVar,char *deviceAddress,const char *deviceName,int ext,int size,int constant,int global))(void*)dlsym(cuda_handle,"__cudaRegisterVar");
  mocu.__mocudaRegisterTexture = (void (*)(void **fatCubinHandle,const struct textureReference *hostVar,const void **deviceAddress,const char *deviceName,int dim,int norm,int ext))(void*)dlsym(cuda_handle,"__cudaRegisterTexture");
  mocu.__mocudaRegisterSurface = (void (*)(void **fatCubinHandle,const struct surfaceReference  *hostVar,const void **deviceAddress,const char *deviceName,int dim,int ext))(void*)dlsym(cuda_handle,"__cudaRegisterSurface");
  mocu.__mocudaRegisterFunction = (void (*)(void **fatCubinHandle,const char *hostFun,char *deviceFun,const char *deviceName,int thread_limit,uint3 *tid,uint3 *bid,dim3 *bDim,dim3 *gDim,int *wSize))(void*)dlsym(cuda_handle,"__cudaRegisterFunction");
  mocu.__mocudaRegisterShared = (void (*)(void **fatCubinHandle,void **devicePtr))(void*)dlsym(cuda_handle,"__cudaRegisterShared");
  mocu.__mocudaRegisterSharedVar = (void (*)(void **fatCubinHandle,void **devicePtr,size_t size,size_t alignment, int storage))(void *)dlsym(cuda_handle,"__cudaRegisterSharedVar");
  mocu.__mocudaSynchronizeThreads = (int (*)(void** one,void* two))(int *)dlsym(cuda_handle,"__cudaSynchronizeThreads");
  mocu.__mocudaTextureFetch = (void (*)(const void* tex,void* index,int integer,void* val))(void *)dlsym(cuda_handle,"__cudaTextureFetch");
  mocu.__mocudaMutexOperation = (void (*)(int lock))(void *)dlsym(cuda_handle,"__cudaMutexOperation");
  mocu.__mocudaRegisterDeviceFunction = (cudaError_t (*)())(cudaError_t *)dlsym(cuda_handle,"__cudaRegisterDeviceFunction");
  mocu.__mocudaRegisterFatBinary = (void** (*)(void*))(void ***)dlsym(cuda_handle,"__cudaRegisterFatBinary");
  mocu.__mocudaUnregisterFatBinary = (void (*)(void**))(void *)dlsym(cuda_handle,"__cudaUnregisterFatBinary");
}

int dlclose_cuda(){
  int res;

  res = dlclose(cuda_handle);
    
  return res;
}
