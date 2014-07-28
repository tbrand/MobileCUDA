#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>

#define THREADS 1024

__global__ void kernel(struct cudaPitchedPtr pitchedPointer){

  int id;
  float *d;

  id = blockDim.x*blockIdx.x + threadIdx.x;

  d = (float*)pitchedPointer.ptr;

  d[id] += 1.0f;

}

int main(){

  int i,ite = 100,size = 40;

  cudaError_t res;
  struct cudaExtent extent;
  struct cudaPitchedPtr pitchedPointer;
  float* ans;

  ans = (float*)malloc(sizeof(float)*size*size*size*THREADS);

  for(i = 0 ; i < size*size*size*THREADS ; i ++){
    ans[i] = 0.0f;
  }

  extent.width = sizeof(float)*size*THREADS;
  extent.height = size;
  extent.depth = size;

  res = cudaMalloc3D(&pitchedPointer,extent);

  if(res != cudaSuccess){
    printf("Oh...(%d)\n",res);
    exit(-1);
  }

  printf("Pointer : %p\n",pitchedPointer.ptr);

  res = cudaMemcpy(pitchedPointer.ptr,ans,sizeof(float)*size*size*size*THREADS,cudaMemcpyHostToDevice);

  if(res != cudaSuccess){
    printf("Oh...(%d)\n",res);
    exit(-1);
  }

  dim3 threads(THREADS,1,1);
  dim3 blocks(size*size*size,1,1);

  for(i = 0 ; i < ite ; i ++){
    kernel<<<blocks,threads>>>(pitchedPointer);
  }

  res = cudaMemcpy(ans,pitchedPointer.ptr,sizeof(float)*size*size*size*THREADS,cudaMemcpyDeviceToHost);
  
  if(res != cudaSuccess){
    printf("Oops ...\n");
    exit(-1);
  }

  for(i = 0 ; i < size*size*size*THREADS ; i ++){
    if(ans[i] != ite){
      printf("ans[%d] == %f\n",i,ans[i]);
      exit(-1);
    }
  }

  printf("Elements : %d\n",size*size*size*THREADS);
  sleep(10);

  res = cudaFree(pitchedPointer.ptr);

  if(res != cudaSuccess){
    printf("Oops ...\n");
    exit(-1);
  }

  return 0;
}
