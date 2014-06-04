#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

__global__ void kernel(){
  
}

int main(){

  int res,length;

  res = cudaSetDeviceFlags(cudaDeviceMapHost);

  printf("cudaSetDeviceFlags(%d)\n",res);

  float* h;
  float* d;

  length = 100;

  res = cudaHostAlloc((void**)&h,sizeof(float)*length,cudaHostAllocMapped|cudaHostAllocPortable);

  printf("cudaHostAlloc(%d)\n",res);

  res = cudaHostGetDevicePointer((void**)&d,h,0);

  printf("cudaHostGetDevicePointer(%d)\n",res);
  printf("device address : %p\n",d);

  

  return 0;

}
