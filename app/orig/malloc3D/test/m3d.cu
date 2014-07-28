#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <unistd.h>

int main(int argc,char* argv[]){

  cudaError_t res;
  int i,j,k;

  size_t width,height,depth,prevpitch = 512;
  size_t prevwidth=0,prevheight=0,prevdepth=0;

  struct cudaPitchedPtr pitchedDevPtr;
  struct cudaExtent extent;

  for(i = 0 ; i < 10000 ; i += 100){
    for(j = 0 ; j < 2000 ; j += 10){
      for(k = 0 ; k < 2000 ; k += 10){

	width = 1*(i+1);
	height = 1*(j+1);
	depth = 1*(k+1);

	extent.width = width;
	extent.height = height;
	extent.depth = depth;

	res = cudaMalloc3D(&pitchedDevPtr,extent);

	if(res != cudaSuccess){

	  printf("extent.width  : %lu\n",extent.width);
	  printf("extent.height : %lu\n",extent.height);
	  printf("extent.depth  : %lu\n",extent.depth);
	  
	  printf("What ...?(%d)\n",res);
	  exit(-1);

	}else{

	  if(prevpitch != pitchedDevPtr.pitch){
	    printf("comfirm differing pitch\n");
	    printf("width:%lu\theight:%lu\tdepth:%lu\tpitch%lu\n",prevwidth,prevheight,prevdepth,pitchedDevPtr.pitch);
	    printf("width:%lu\theight:%lu\tdepth:%lu\tpitch%lu\n",width,height,depth,pitchedDevPtr.pitch);
	    prevpitch = pitchedDevPtr.pitch;
	  }
	}

	prevwidth = width;
	prevheight = height;
	prevdepth = depth;

	cudaFree(pitchedDevPtr.ptr);

	size_t expected_pitch;

	expected_pitch = (width/512 + 1)*512;

	if(expected_pitch != pitchedDevPtr.pitch){
	  printf("Oh ...\n");
	  exit(-1);
	}

	/*
	size_t expected_pitch;

	//    expected_pitch = extent.width*extent.height*extent.depth != 0 ? ((extent.width*extent.height*extent.depth-1)/(5120000)+1)*512 : 0;

	expected_pitch = extent.width*extent.height != 0 ? ((extent.width*extent.height-1)/(51200*extent.depth)+1)*512 : 0;

	if(expected_pitch != pitchedDevPtr.pitch){

	  //      printf("extent.width  : %lu\n",extent.width);
	  //      printf("extent.height : %lu\n",extent.height);
	  //      printf("extent.depth  : %lu\n",extent.depth);
	  printf("width*height*depth : %lu\n",extent.width*extent.height*extent.depth);

	  //    printf("\txsize : %lu\n",pitchedDevPtr.xsize);
	  //    printf("\tysize : %lu\n",pitchedDevPtr.ysize);
	  printf("\tpitch : %lu\n",pitchedDevPtr.pitch);
	  //      printf("\tptr   : %p \n",pitchedDevPtr.ptr);
	  //    printf("expected_pitch : %lu\n",expected_pitch);
	  //      exit(1);
	}else{
	  printf("OK *** i,j,k : %d,%d,%d\n",i,j,k);
	}
	*/
      }
    }
  }

  return 0;
}
