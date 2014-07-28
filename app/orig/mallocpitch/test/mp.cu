#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>

int main(int argc,char* argv[]){

  cudaError_t res;
  float* d;
  int i,j;
  size_t pitch,width,height;

  for(i = 0 ; i < 1000 ; i ++){

    for(j = 0 ; j < 100 ; j ++){

      width = 5*i;
      height = 100*j;

      res = cudaMallocPitch(&d,&pitch,width,height);

      //      size_t expected_pitch = width*height != 0 ? ((width*height-1)/(512*100)+1)*512 : 0;

      size_t expected_pitch;

      expected_pitch = width*height != 0 ? (width/512 + 1)*512 : 0;

      //      printf("pitch : %lu\n",pitch);
      //      printf("expected_pitch : %lu\n",expected_pitch);

      if(pitch != expected_pitch){
	printf("width : %lu\n",width);
	printf("height : %lu\n",height);
	exit(1);
      }

      if(res != cudaSuccess){
	printf("What ... ?(%d)\n",res);
	exit(1);
      }

      if(expected_pitch != pitch){

	printf("pitch == %d\n",pitch);
	printf("expected pitch == %d\n",expected_pitch);
	printf("Total device memory usage : %d\n",width*height);

      }
    }
    
    cudaFree(d);
    
  }

  return 0;
}
