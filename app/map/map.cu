#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

__global__ void kernel(float* d,float* d1,int size){
  
  int length_per_block;
  int length_per_thread;
  int start,end;

  length_per_block  = size/gridDim.x;
  length_per_thread = length_per_block/blockDim.x;

  start = length_per_block*blockIdx.x + length_per_thread*threadIdx.x;
  end   = length_per_block*blockIdx.x + length_per_thread*(threadIdx.x+1);

  for(int i = start ; i < end ; i ++ ){
    d1[i] += 1.0f;
    d[i]  += d1[i];
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

  int res,length,ite;

  ite = 10;

  res = cudaSetDeviceFlags(cudaDeviceMapHost);

  printf("cudaSetDeviceFlags(%d)\n",res);

  float *h;
  float *d,*d1;

  length = 500000000;

  //  res = cudaHostAlloc((void**)&h,sizeof(float)*length,cudaHostAllocMapped|cudaHostAllocPortable);

  h = (float*)valloc(sizeof(float)*length);

  res = cudaHostRegister(h,sizeof(float)*length,cudaHostRegisterMapped);

  printf("cudaHostAlloc(%d)\n",res);

  res = cudaHostGetDevicePointer((void**)&d,h,0);

  printf("cudaHostGetDevicePointer(%d)\n",res);
  printf("device address : %p\n",d);

  h = (float*)malloc(sizeof(float)*length);

  for(int i = 0 ; i < length ; i ++){
    h[i] = 0.0f;
  }

  dim3 threads(1000,1,1);
  dim3 blocks(1000,1,1);

  res = cudaMemcpy(d,h,sizeof(float)*length,cudaMemcpyHostToDevice);

  printf("cudaMemcpyHostToDevice(%d)\n",res);

  res = cudaMalloc((void**)&d1,sizeof(float)*length);

  printf("cudaMalloc(%d) : Address %p\n",res,d1);

  res = cudaMemcpy(d1,h,sizeof(float)*length,cudaMemcpyHostToDevice);

  printf("cudaMemcpyHostToDevice(%d)\n",res);

  for(int i = 0 ; i < ite ; i ++){

    kernel<<<blocks,threads>>>(d,d1,length);

  }

  res = cudaMemcpy(h,d,sizeof(float)*length,cudaMemcpyDeviceToHost);

  printf("cudaMemcpyDeviceToHost(%d)\n",res);
  
  for(int i = 0 ; i < length ; i ++){
    if(h[i] != ((ite+1)*ite)/2 ){
      printf("Result test : Failed\n");
      printf("h[%d] == %f\n",i,h[i]);
      return -1;
    }
  }

  printf("Result test PASS\n");

  gettimeofday(&t1,NULL);

  printf("TIME RESULT : %f[sec](MAP)\n",elapsed(t0,t1));
      
  return 0;

}
