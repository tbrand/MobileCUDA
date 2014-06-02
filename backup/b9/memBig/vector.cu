/**
   Sample for Mobile CUDA
   Simple Adding Vectors Application.

   Authoer @ Taichirou Suzuki
**/

#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

/**
   Simple Kernel.
**/
__global__ void ___add(float* a,float* b,unsigned long size){
  int _x = blockDim.x * blockIdx.x + threadIdx.x;
  int _y = blockDim.y * blockIdx.y + threadIdx.y;
  
  unsigned long id = _x + _y * size;

  a[id] += b[id];
}

static float elapsed(struct timeval tv0,struct timeval tv1){
  return (float)(tv1.tv_sec - tv0.tv_sec)
    + (float)(tv1.tv_usec - tv0.tv_usec)
    * 0.000001f;
}

int main(void){

  struct timeval t0,t1;

  gettimeofday(&t0,NULL);
  
  /**
     Define Vector Size.
  **/
  //  unsigned long _hen = 11000;
  unsigned long _hen = 18000;
  //  unsigned long _hen = 18000;
  unsigned long size = _hen * _hen;
  printf("gyouretu size : %lu\n",size);
  /**
     Number Of Launch Kernel.
  **/
  int numOfLaunchKernel = 1;
  //int numOfLaunchKernel = 1;

  cudaSetDevice(0);

  //  float* h_a = (float*)malloc(sizeof(float)*size);
  //  float* h_b = (float*)malloc(sizeof(float)*size);

  float* d_a = NULL;
  float* d_b = NULL;
  //  float* d_c = NULL;

  cudaMalloc((void**)&d_a,sizeof(float)*size);
  cudaMalloc((void**)&d_b,sizeof(float)*size);
  //  cudaMalloc((void**)&d_c,sizeof(float)*size);

  float* h_a = NULL;
  float* h_b = NULL;

  /*
  cudaError_t res;
  res = cudaHostAlloc((void **)&h_a,sizeof(float)*size,0);
  printf("cudaHostAlloc : %d\n",res);
  res = cudaHostAlloc((void **)&h_b,sizeof(float)*size,0);
  printf("cudaHostAlloc : %d\n",res);
  */

  h_a = (float*)malloc(sizeof(float)*size);
  h_b = (float*)malloc(sizeof(float)*size);  

  //  float* h_c = (float*)malloc(sizeof(float)*size);

  printf("This Sample Application Uses %d[Mbyte] per vector.(Total : %d[Mbyte])\n",sizeof(float)*size >> 20,sizeof(float)*size*2 >> 20);

  for(int i =  0 ; i < size ; i ++){
    h_a[i] = 0.0f;
    h_b[i] = 1.0f;
  }

  //  int ite = 140;
  int ite = 155;
  //  int ite = 1000000;

  for(int j = 0 ; j < ite ; j ++){

    cudaMemcpy(d_a,h_a,sizeof(float)*size,cudaMemcpyHostToDevice);
    cudaMemcpy(d_b,h_b,sizeof(float)*size,cudaMemcpyHostToDevice);

    int _size = 10;
    dim3 threads(_size,_size,1);
    dim3 grid(_hen/_size,_hen/_size,1);

    for(int i = 0 ; i < numOfLaunchKernel ; i ++){
      //__add<<<grid,threads>>>(d_c,d_a,d_b,_hen);
      ___add<<<grid,threads>>>(d_a,d_b,_hen);
      /**
	 Main thread can sleep at here.
      **/
      //    sleep(1);
    }

    //  cudaMemcpy(h_c,d_c,sizeof(float)*size,cudaMemcpyDeviceToHost);
    cudaMemcpy(h_a,d_a,sizeof(float)*size,cudaMemcpyDeviceToHost);

  }

  int pass = 1;
  for(int i = 0 ; i < size ; i ++){
    //    if(h_c[i] != numOfLaunchKernel){
    //    if(h_a[i] != numOfLaunchKernel){
    //    printf("H_A[%d] : %d",i,h_a[i]);
    if(h_a[i] != ite){
      pass = 0;
    }
  }

  if(pass){
    printf(">Result TEST : PASS\n");
  }else{
    printf(">Result TEST : FAILED\n");
  }

  cudaFree(d_a);
  cudaFree(d_b);
  //  cudaFree(d_c);

  free(h_a);
  free(h_b);
  //  cudaFreeHost(h_a);
  //  cudaFreeHost(h_b);
  //  free(h_c);

  printf("Application Closed...\n");

  gettimeofday(&t1,NULL);

  printf("My RESULT : %f\n",elapsed(t0,t1));

  return 0;
}
