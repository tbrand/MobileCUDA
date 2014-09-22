#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cuda_runtime.h>
#include <nvml.h>

#define IS 5000
#define JS 5000
#define I 50000
#define J 50000

int main(){

  /*Create context*/
  float* d;
  cudaMalloc((void**)&d,sizeof(float)*1);
  cudaFree(d);

  FILE *fp;
  char *fname = "array_data.csv";

  fp = fopen(fname,"w");

  int failed_counter = 0;

  cudaError_t res;
  cudaArray_t array;

  nvmlReturn_t nres;
  nvmlMemory_t mem;
  nvmlDevice_t dev;
  
  size_t width = 0,height = 0;
  size_t prev_width = 0,prev_height = 0;
  struct cudaChannelFormatDesc desc;

  size_t before_mem = 0;
  size_t after_mem  = 0;
  size_t prev_mem   = 0;

  desc = cudaCreateChannelDesc(32,0,0,0,cudaChannelFormatKindFloat);
  //  desc = cudaCreateChannelDesc(16,16,16,16,cudaChannelFormatKindSigned);

  nres = nvmlInit();

  if(nres != NVML_SUCCESS){
    printf("NVML ERROR : %d\n",nres);
    exit(-1);
  }

  nres = nvmlDeviceGetHandleByIndex(0,&dev);

  if(nres != NVML_SUCCESS){
    printf("NVML ERROR : %d\n",nres);
    exit(-1);
  }
  
#if 1

  int max = 0 , min = 100000000;

  //  for(int i = IS ; i < IS+I ; i ++){
  for(int i = IS ; i < IS+I ; i += 32){

    width = i;

    //    for(int j = JS ; j < JS+J ; j ++){
    for(int j = JS ; j < JS+J ; j += 32){

      height = j;
      
      nres = nvmlDeviceGetMemoryInfo(dev,&mem);

      if(nres != NVML_SUCCESS){
	printf("NVML ERROR : %d\n",nres);
	exit(-1);
      }

      before_mem = mem.free;

      res = cudaMallocArray(&array,&desc,width,height,0);

      if(res == cudaSuccess){

	nres = nvmlDeviceGetMemoryInfo(dev,&mem);
	
	if(nres != NVML_SUCCESS){
	  printf("NVML ERROR : %d\n",nres);
	  exit(-1);
	}

	after_mem = mem.free;

	size_t used;
	size_t expected;

	used = before_mem-after_mem;

	expected = ((desc.w+desc.x+desc.y+desc.z+7)/8)
	  *((width+31)&~(size_t)31)
	  *((height+127)&~(size_t)127)
	  + (2<<20);

	if(expected < used){
	  printf("expected : %lu\n",expected);
	  printf("used     : %lu\n",used);
	  printf("width    : %lu\n",width);
	  printf("height   : %lu\n",height);
	  exit(-1);
	}

	if(min > expected-used){
	  min = expected-used;
	}

	if(max < expected-used){
	  max = expected-used;
	}

	printf("%lu\t%lu\t%lu\t%lu\t%lu\n",expected-used,min,max,width,height);

	/*
	fprintf(fp,"%lu",used);

	if(j < J-1){
	  fprintf(fp,",");
	}else{
	  fprintf(fp,"\n");
	}
	*/

	/*

	size_t expected;

	if(before_mem-after_mem != expected){
	  printf("Failed\n");
	  printf("width        : %lu\n",width);
	  printf("height       : %lu\n",height);
	  printf("width*height : %lu\n",width*height);
	  printf("used         : %lu\n",before_mem-after_mem);
	  printf("be expected as : %lu\n",expected);
	  if(++failed_counter >= 10)
	    exit(-1);
	}else{
	  //	  printf("PASS(%lu,%lu:%lu)",width,height,expected);
	}

	size_t e;

	e = ((width-1)/32+1)*131072+((height-1)/128+1)*524288;
	
	expected = e > 2097152 ? e : 2097152;

	if(expected-used > max){
	  max = expected-used;
	}

	if(min > (int)expected-(int)used){
	  min = expected-used;
	}

	printf("max:%d\tmin:%d\tused:%lu\texpected:%lu\twidth:%d\theight:%d\n",max,min,used,expected,width,height);

	if(min < 0){
	  printf("Detected min < 0\n");
	  printf("width:%d,height:%d,used:%d,expected:%d\n",width,height,used,expected);
	  exit(-1);
	}

	//	size_t left  = used/w-used/2048;
	//	size_t right = -960+1966080/w;

	if(left != right){
	  printf("\t%lu\t%lu\n",left,right);
	  exit(-1);
	}

	*/

	if((before_mem-after_mem) > prev_mem){

	  /*
	  printf("width  : %lu\n",width);
	  printf("height : %lu\n",height);
	  printf("used   : %lu\n",before_mem-after_mem);
	  printf("diff(width)  : %lu\n",width-prev_width);
	  printf("diff(height) : %lu\n",height-prev_height);
	  printf("diff         : %lu\n",(before_mem-after_mem)-prev_mem);
	  printf("diff(width*height) : %lu\n",width*height-prev_width*prev_height);
	  printf("(used-2097152)/131072: %f\n",(float)(before_mem-after_mem-2097152)/(float)131072);
	  */

	  prev_mem = before_mem-after_mem;
	  prev_width = width;
	  prev_height = height;
	}
	

	cudaFreeArray(array);

      }else{

	printf("\nERROR : %d\n",res);
	printf("Failed Counter : %d\n",failed_counter);
	exit(-1);
	
      }
    }
  }

#endif

  return 0;
}
