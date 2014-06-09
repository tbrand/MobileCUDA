#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

__global__ void malloc_in_kernel(float* d_a,int n,int length){

  float* x;

  length = 5;

  x = (float*)malloc(sizeof(float)*length);

  for(int i = 0 ; i < length ; i ++){
    x[i] = 1.0f;
  }

  int length_per_block;
  int length_per_thread;
  int start,end;

  length_per_block  = n/gridDim.x;
  length_per_thread = length_per_block/blockDim.x;

  start = length_per_block*blockIdx.x + length_per_thread*threadIdx.x;
  end   = length_per_block*blockIdx.x + length_per_thread*(threadIdx.x+1);

  for(int i = start ; i < end ; i ++){
    for(int j = 0 ; j < length ; j ++){
      d_a[i] += x[j];
    }
  }
  
  free(x);
}

static float elapsed(struct timeval tv0,struct timeval tv1){
  return (float)(tv1.tv_sec - tv0.tv_sec)
    + (float)(tv1.tv_usec - tv0.tv_usec)
    * 0.000001f;
}

int main(){

  struct timeval t0,t1;

  gettimeofday(&t0,NULL);

  int n = 600000000;
  int iter = 2;
  int length = 5;

  int thread_num = 8;
  int block_num  = 4;
  
  dim3 threads(thread_num,1,1);
  dim3 blocks(block_num,1,1);

  float* h_a;
  float* d_a;

  h_a = (float*)malloc(sizeof(float)*n);

  cudaMalloc((void**)&d_a,sizeof(float)*n);
  cudaMemcpy(d_a,h_a,sizeof(float)*n,cudaMemcpyHostToDevice);

  for(int i = 0 ; i < iter ; i ++){
    h_a[i] = 0.0f;
  }

  for(int i = 0 ; i < iter ; i ++){

    malloc_in_kernel<<<blocks,threads>>>(d_a,n,length);

  }

  cudaMemcpy(h_a,d_a,sizeof(float)*n,cudaMemcpyDeviceToHost);

  int test = 1;

  for(int i = 0 ; i < n ; i ++){
    if(h_a[i] != iter*length){
      test = 0;
      break;
    }
  }

  if(test){
    printf("Result test PASS\n");
  }else{
    printf("Result test Failed\n");
  }

  gettimeofday(&t1,NULL);

  printf("TIME RESULT : %f[sec](MIK)\n",elapsed(t0,t1));
}
