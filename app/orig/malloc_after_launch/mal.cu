#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#define VSIZE 1024*50000
#define TSIZE 1024
#define BSIZE VSIZE/TSIZE
#define ITE 10

__global__ void add(float* a,float* b){

  int idx = blockDim.x * blockIdx.x + threadIdx.x;

  b[idx] += 1;
  a[idx] += b[idx];

}

int main(){

  float *ha,*hb;
  float *da,*db;

  ha = (float*)malloc(sizeof(float)*VSIZE);
  hb = (float*)malloc(sizeof(float)*VSIZE);

  cudaMalloc((void**)&da,sizeof(float)*VSIZE);
  cudaMalloc((void**)&db,sizeof(float)*VSIZE);

  for(int i = 0 ; i < VSIZE ; i ++){
    ha[i] = 0.0f;
    hb[i] = 0.0f;
  }

  cudaMemcpy(da,ha,sizeof(float)*VSIZE,cudaMemcpyHostToDevice);
  cudaMemcpy(db,hb,sizeof(float)*VSIZE,cudaMemcpyHostToDevice);

  dim3 threads(TSIZE,1,1);
  dim3 blocks (BSIZE,1,1);

  printf("threads : %d\n",threads.x);
  printf("blocks  : %d\n",blocks.x);

  for(int i = 0 ; i < ITE ; i ++){
    add<<<blocks,threads>>>(da,db);
    cudaDeviceSynchronize();
  }

  cudaMemcpy(ha,da,sizeof(float)*VSIZE,cudaMemcpyDeviceToHost);

  for(int i = 0 ; i < VSIZE ; i ++){
    if(ha[i] != ((ITE+1)*ITE)/2 ){
      printf("ha[%d]\t%f\n",i,ha[i]);
      printf("Result TEST : FAILED\n");
      exit(-1);
    }
  }

  printf("Result TEST : PASS\n");

  free(ha);
  free(hb);
  cudaFree(da);
  cudaFree(db);

  return 0;
}
