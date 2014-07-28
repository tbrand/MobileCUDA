#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#define THREADS 1024
#define SIZE    (THREADS*250000)

__device__ float device_array0[SIZE];
__device__ float device_array1[SIZE];

__global__ void devmem_kernel(){

  int id = blockDim.x*blockIdx.x + threadIdx.x;

  device_array0[id] += 1.0f;
  device_array1[id] += device_array0[id];

}

static float elapsed(struct timeval tv0,struct timeval tv1){
  return (float)(tv1.tv_sec - tv0.tv_sec)
    + (float)(tv1.tv_usec - tv0.tv_usec)
    * 0.000001f;
}

int main(){

  struct timeval t0,t1;

  gettimeofday(&t0,NULL);

  printf("Vector SIZE : %d[Mbyte]\n",sizeof(float)*SIZE >> 20);


  float *result;
  int   niter;
  cudaError_t res
;
  result = (float*)malloc(sizeof(float)*SIZE);

  dim3  threads (THREADS,1,1);
  dim3  blocks  (SIZE/THREADS,1,1);

  niter = 2000;

  for(int i = 0 ; i < SIZE ; i ++){
    result[i] = 0.0f;
  }

  res = cudaMemcpyToSymbol(device_array0,result,sizeof(float)*SIZE,0,cudaMemcpyHostToDevice);
  res = cudaMemcpyToSymbol(device_array1,result,sizeof(float)*SIZE,0,cudaMemcpyHostToDevice);

  printf("cudaMemcpyToSymbol(%d)\n",res);

  for(int i = 0 ; i < niter ; i ++){

    devmem_kernel<<<blocks,threads>>>();

    cudaDeviceSynchronize();

  }

  res = cudaMemcpyFromSymbol(result,device_array1,sizeof(float)*SIZE,0,cudaMemcpyDeviceToHost);

  printf("cudaMemcpyFromSymbol(%d)\n",res);

  int pass = 1;

  for(int i = 0 ; i < SIZE ; i ++){
    if(result[i] != (float)(((niter+1)*niter)/2.0f) ){
      pass = 0;
      printf("result[%d] : %f\n",i,result[i]);
      break;
    }
  }

  if(pass){
    printf("Result test : PASS\n");
  }else{
    printf("Result test : Failed\n");
  }

  gettimeofday(&t1,NULL);

  printf("TIME RESULT : %f[sec](DEV MEM)\n",elapsed(t0,t1));

  return 0;
}
