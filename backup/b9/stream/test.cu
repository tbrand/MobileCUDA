#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

__global__ void __add(float* a,float* b,int size,int blockNum,int streamNum,int streamNo){

  int streamLength = size/streamNum;
  int blockSize    = streamLength/blockNum;
  int threadSize   = blockSize/blockDim.x;

  int startIndex   = streamLength*streamNo + blockSize*blockIdx.x + threadSize*threadIdx.x;

  for(int i = 0 ; i < threadSize ; i ++){
    a[startIndex+i] += b[startIndex+i];
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

  int streamNum = 8;
  int threadNum = 1024;
  int blockNum  = 1;

  int size = streamNum * threadNum * blockNum * 40000;

  int ite = 90;

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

  printf("size == %d\n",size);

  cudaStream_t* streams;

  streams = (cudaStream_t*)malloc(sizeof(cudaStream_t)*streamNum);

  for(int i = 0 ; i < streamNum ; i ++){

    cudaStreamCreate(&streams[i]);

  }

  for(int i = 0 ; i < ite ; i ++){
    for(int j = 0 ; j < streamNum ; j ++){
      __add<<<blocks,threads,0,streams[j]>>>(d_a,d_b,size,blockNum,streamNum,j);
    }
  }

  for(int i = 0 ; i < streamNum ; i ++){

    cudaStreamSynchronize(streams[i]);
    cudaStreamDestroy(streams[i]);

  }

  cudaMemcpy(h_a,d_a,sizeof(float)*size,cudaMemcpyDeviceToHost);

  int pass = 1;
  int firstFailedIndex = 0;

  for(int i = 0 ; i < size ; i ++){

    if(h_a[i] != ite){
      firstFailedIndex = i;
      pass = 0;
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
