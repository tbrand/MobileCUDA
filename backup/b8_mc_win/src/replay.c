#include <mocu.h>

void** mocu_cubin_handle;

int replay(apilog* a){

  switch(a->type){

  case MALLOC:

    REPLAY("cudaMalloc");

    cudaError_t res;
    void* ptr;
    res = mocu.mocudaMalloc(&ptr,a->data.malloc.size);
    if(res != cudaSuccess || ptr != a->data.malloc.devPtr){
      printf("replay failed in cudaMalloc()\n");
      printf("a->devPtr   : %p\n",a->data.malloc.devPtr);
      printf("ptr         : %p\n",ptr);
      printf("result code : %d\n",res);
      return -1;
    }else{
      return 0;
    }

  case FREE:

    REPLAY("cudaFree");
      
    cudaError_t _res;
    _res = mocu.mocudaFree(a->data.free.devPtr);
    if(_res != cudaSuccess){
      printf("replay failed in cudaFree()\n");
      printf("result code : %d\n",_res);
      return -1;
    }else{
      return 0;
    }

  case REGISTERFUNCTION:

    //    return 0;

#if 1

    REPLAY("cudaRegisterFunction");

    printf("fatCubinHandle : %p\n",mocu_cubin_handle);

    mocu.__mocudaRegisterFunction(
				  mocu_cubin_handle,//a->data.registerFunction.fatCubinHandle,
				  a->data.registerFunction.hostFun,
				  a->data.registerFunction.deviceFun,
				  a->data.registerFunction.deviceName,
				  a->data.registerFunction.thread_limit,
				  (uint3*)a->data.registerFunction.tid,
				  (uint3*)a->data.registerFunction.bid,
				  (dim3*)a->data.registerFunction.bDim,
				  (dim3*)a->data.registerFunction.gDim,
				  a->data.registerFunction.wSize
				  );

    return 0;

#endif

  case REGISTERFATBINARY:

    //    return 0;

#if 1

    REPLAY("cudaRegisterFatBinary");

    void** fatCubinHandle;
    void*  fatCubin;

    fatCubin = a->data.registerFatBinary.fatCubin;

    printf("fatCubin       : %p\n",fatCubin);

    fatCubinHandle = mocu.__mocudaRegisterFatBinary(fatCubin);

    printf("fatCubinHandle : %p\n",fatCubinHandle);

    mocu_cubin_handle = fatCubinHandle;

    return 0;

#endif

  default:

    REPLAY("Invalid function(Error)");

    return -1;
  }
}
