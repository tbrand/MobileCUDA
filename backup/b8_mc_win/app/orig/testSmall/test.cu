#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

/**
   1.5[MB]
   div == 4, size = * 48000

   2.0[MB]
   div == 8, size = * 32000

   2.4[MB]
   div == 8, size = * 37000
**/

__global__ void __add(float* a,float* b,int size,int div){

  int idx = blockDim.x * blockIdx.x + threadIdx.x;

#pragma unroll
  for(int i = 0 ; i < div ; i ++){
    a[idx + (size*i)/div] += b[idx + (size*i)/div];
  }

}

static float elapsed(struct timeval tv0,struct timeval tv1){
  return (float)(tv1.tv_sec - tv0.tv_sec)
    + (float)(tv1.tv_usec - tv0.tv_usec)
    * 0.000001f;
}

int main(){

  struct timeval t0,t1;

  gettimeofday(&t0,NULL);

  float *h_a = NULL;
  float *h_b = NULL;
  float *d_a = NULL;
  float *d_b = NULL;

  int div = 4;
  int threadNum = 1024;

  unsigned int size = (threadNum*div) * 48000;

  int blockNum  = size/(threadNum*div);

  printf("blockNum  : %d\n",blockNum);
  printf("threadNum : %d\n",threadNum);
  printf("size      : %d\n",size);

  printf("vector size : %d\n",sizeof(float)*size);

  int ite = 6000;

  cudaMalloc((void**)&d_a,sizeof(float)*size);
  cudaMalloc((void**)&d_b,sizeof(float)*size);

  h_a = (float*)malloc(sizeof(float)*size);
  h_b = (float*)malloc(sizeof(float)*size);

  for(int i = 0 ; i < size ; i ++){
    h_a[i] = 0.0f;
    h_b[i] = 1.0f;
  }

  dim3 threads(threadNum,1,1);
  dim3 blocks(blockNum,1,1);

  cudaMemcpy(d_a,h_a,sizeof(float)*size,cudaMemcpyHostToDevice);
  cudaMemcpy(d_b,h_b,sizeof(float)*size,cudaMemcpyHostToDevice);

  for(int i = 0 ; i < ite ; i ++){
    __add<<<blocks,threads>>>(d_a,d_b,size,div);
  }

  cudaMemcpy(h_a,d_a,sizeof(float)*size,cudaMemcpyDeviceToHost);

  int pass = 1;
  int firstFailedIndex = 0;

  for(int i = 0 ; i < size ; i ++){

    //    printf("h_a[%d]:%f ",i,h_a[i]);

    if(h_a[i] != ite){
      firstFailedIndex = i;
      pass = 0;
      break;
    }
  }

  if(pass){
    printf("Result test PASS!\n");
  }else{
    printf("Result test Failed\n");
    printf("h_a[%d] == %f\n",firstFailedIndex,h_a[firstFailedIndex]);
  }

  gettimeofday(&t1,NULL);

  printf("TIME RESULT : %f\n",elapsed(t0,t1));

  return 0;

}
