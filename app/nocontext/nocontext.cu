#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

__device__ float test1[1000];
__device__ float test2[1000];

__global__ void kernel(){

  test1[0] = 0.0f;
  test2[0] = 0.0f;

}

int main(int argc,char* argv[]){

  return 0;
}
