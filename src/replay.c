#include <mocu.h>

void** mocu_cubin_handle;

int replay(apilog* a){

  cudaError_t res;

  switch(a->type){

  case MALLOC:

    REPLAY("cudaMalloc");

    void* ptr;
    res = mocu.mocudaMalloc(&ptr,a->data.malloc.size);
    if(res != cudaSuccess || ptr != a->data.malloc.devPtr){
      printf("replay failed in cudaMalloc()\n");
      printf("a->devPtr   : %p\n",a->data.malloc.devPtr);
      printf("ptr         : %p\n",ptr);
      printf("result code : %d\n",res);
      return -1;
    }else{
      //      printf("\tAddr : %p\n",ptr);
      return 0;
    }

  case FREE:

    REPLAY("cudaFree");
      
    res = mocu.mocudaFree(a->data.free.devPtr);
    if(res != cudaSuccess){
      printf("replay failed in cudaFree()\n");
      printf("result code : %d\n",res);
      return -1;
    }else{
      return 0;
    }

  case REGISTERVAR:

    REPLAY("cudaRegisterVar");

    //    printf("\thostVar : %p\n",a->data.registerVar.hostVar);

    mocu.__mocudaRegisterVar(
			     mocu_cubin_handle,
			     a->data.registerVar.hostVar,
			     a->data.registerVar.deviceAddress,
			     (const char*)a->data.registerVar.deviceName,
			     a->data.registerVar.ext,
			     a->data.registerVar.size,
			     a->data.registerVar.constant,
			     a->data.registerVar.global
			     );

    return 0;

  case REGISTERFUNCTION:

    //    return 0;

#if 1

    REPLAY("cudaRegisterFunction");

    //    printf("fatCubinHandle : %p\n",mocu_cubin_handle);

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

    //    printf("fatCubin       : %p\n",fatCubin);

    fatCubinHandle = mocu.__mocudaRegisterFatBinary(fatCubin);

    //    printf("fatCubinHandle : %p\n",fatCubinHandle);

    mocu_cubin_handle = fatCubinHandle;

    return 0;

#endif

  case HOSTREGISTER:

    REPLAY("cudaHostRegister");

    res = mocu.mocudaHostRegister(
				  a->data.hostRegister.ptr,
				  a->data.hostRegister.size,
				  a->data.hostRegister.flags|cudaHostRegisterPortable
				  );

    if(res != cudaSuccess){
      printf("replay failed in cudaHostRegister()\n");
      printf("ptr  : %p\n",a->data.hostRegister.ptr);
      printf("size : %d\n",a->data.hostRegister.size);
      printf("a->flags : %d\n",a->data.hostRegister.flags);
      printf("result code : %d\n",res);
      return -1;      
    }else{
      return 0;
    }
    
  default:

    REPLAY("Invalid function(Error)");

    return -1;
  }
}
