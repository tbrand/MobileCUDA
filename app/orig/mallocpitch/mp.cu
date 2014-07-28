#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>

#define THREADS 1024

__global__ void kernel(float* d_1,float* d_2){

  int id;

  id = blockDim.x*blockIdx.x + threadIdx.x;

  d_1[id] += 1.0f;
  d_2[id] += d_1[id];
}

int main(){

  cudaError_t res;
  float *d_1,*d_2,*h;
  size_t pitch;
  size_t size = THREADS * 10;
  int i,ite = 1000;

  h = (float*)malloc(sizeof(float)*size*size);

  for(i = 0 ; i < size*size ; i ++)
    h[i] = 0.0f;

  res = cudaMallocPitch(&d_1,&pitch,sizeof(float)*size,size);
  
  if(res != cudaSuccess){
    printf("Oops ...\n");
    exit(-1);
  }

  res = cudaMallocPitch(&d_2,&pitch,sizeof(float)*size,size);
  
  if(res != cudaSuccess){
    printf("Oops ...\n");
    exit(-1);
  }

  printf("pitch : %lu\n",pitch);
  printf("height : %lu\n",size);
  printf("region : %lu[MB]\n",pitch*size >> 20);

  res = cudaMemcpy2D(d_1,pitch,h,sizeof(float)*size,sizeof(float)*size,size,cudaMemcpyHostToDevice);

  if(res != cudaSuccess){
    printf("Oops ...\n");
    exit(-1);
  }

  res = cudaMemcpy2D(d_2,pitch,h,sizeof(float)*size,sizeof(float)*size,size,cudaMemcpyHostToDevice);

  if(res != cudaSuccess){
    printf("Oops ...\n");
    exit(-1);
  }

  dim3 threads(THREADS,1,1);
  dim3 blocks(size/THREADS,1,1);

  for(i = 0 ; i < ite ; i ++){

    kernel<<<blocks,threads>>>(d_1,d_2);

  }

  res = cudaMemcpy(h,d_2,sizeof(float)*size*size,cudaMemcpyDeviceToHost);

  if(res != cudaSuccess){
    printf("Oops ...\n");
    exit(-1);
  }

  for(i = 0 ; i < size ; i ++){
    if(h[i] != ((ite+1)*ite)/2.0f  ){
      printf("h[%d] == %f\n",i,h[i]);
      exit(-1);
    }
  }

  sleep(10);

  return 0;
}
