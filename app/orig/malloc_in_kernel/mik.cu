#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

__global__ void malloc_in_kernel(int n){

  float* x;

  x = (float*)malloc(sizeof(float)*n);

  for(int i = 0 ; i < n ; i ++){
    x[i] = 1.0f;
  }
}

int main(){

  int n = 1000000;
  int iter = 1000;
  
  dim3 threads(1024,1,1);
  dim3 blocks(100,1,1);

  for(int i = 0 ; i < iter ; i ++){

    malloc_in_kernel<<<blocks,threads>>>(n);

  }

  sleep(10);

  printf("FIN\n");

}
