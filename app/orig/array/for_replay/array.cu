#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cuda_runtime.h>

__global__ void kernel(){
  
}

int main(){

  float *d;
  cudaError_t res;

  res = cudaMalloc((void**)&d,sizeof(float)*100);
  
  printf("Address : %p(res: %d)\n",d,res);

  res = cudaDeviceReset();

  printf("device reset : %d\n",res);

  /*malloc array*/
  cudaArray_t array;
  const struct cudaChannelFormatDesc desc;
  size_t width,height;

  width = 100;
  height = 100;

  desc.w = 10;
  desc.x = 10;
  desc.y = 10;
  desc.z = 10;

  res = cudaMallocArray(&array,&desc,width,height,0);

  printf("cudaMallocArray : %d\n",res);
  printf("x: %d, y: %d, z: %d, w: %d\n",desc.x,desc.y,desc.z,desc.w);

  res = cudaMalloc((void**)&d,sizeof(float)*100);

  printf("Address : %p(res: %d)\n",d,res);  
  
  return 0;
}
