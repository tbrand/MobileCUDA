#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <regex.h>

#include <__cudaFatFormat.h>
#include <mocu.h>

#define LINE_SIZE 1024
#define PATH_SIZE 64
#define FILE_SIZE 1024 << 15
#define DATA_SIZE 10

char path[PATH_SIZE];

int offset = 0;
char buf[FILE_SIZE];
long start,end;
  
char* get_line(){
    
  int size = 0;
  char* line;

  if(buf[offset] == '\0')return NULL;

  while(buf[offset+size] != '\n'){
    size ++;
  }

  line = (char*)malloc(sizeof(char)*size + 1);
    
  memcpy(line,(const void*)&buf[offset],size);

  offset += size+1;

  return line;
}

int contains_dev_nvidia(char* src){
  int res_runtime,res_driver,res_device,res_nvml;

  res_runtime = 0;
  res_driver = 0;
  res_device = 0;
  res_nvml = 0;

#if 0
  if(strstr(src,"/opt/cuda/tk5.0/lib64/libcudart.so")){
    res_runtime = 1;
    printf("found runtime!\n");
  }
#endif

#if 1
  if(strstr(src,"libcuda.so")){
    res_driver = 1;
    printf("found driver!\n");
  }
#endif

#if 1    
  if(strstr(src,"/dev/nvidia")){
    res_device = 1;    
    printf("found device!\n");
  }
#endif

#if 0
  if(strstr(src,"libnvidia-ml.so")){
    res_nvml = 1;    
    printf("found nvml!\n");
  }
#endif

  return res_runtime|res_driver|res_device|res_nvml;
}

char* get_match(const char* str,int so,int eo){
  char* tmp = (char*)malloc(sizeof(char)*64);

  str += so;
  eo  -= so;

  strncpy(tmp,str,eo);

  tmp[eo] = '\0';

  return tmp;
}

long get_address(){
  return start;
}

void mapped_compile(char* line){

  regex_t regst;
  regmatch_t match[3];

  char *reg = "[a-z0-9-]*";
    
  if(regcomp(&regst,reg,REG_EXTENDED)){
    printf("Failed to compile...\n");
    return;
  }

  if(!regexec(&regst,line,3,match,0)){

    char *test,*_start,*_end;

    test = get_match(line,match[0].rm_so,match[0].rm_eo);
      
    _start = strtok(test,"-");
    _end   = strtok(NULL,"-");

    start = strtol(_start,NULL,16);
    end   = strtol(_end,NULL,16);
      
  }

  regfree(&regst);
    
}

size_t get_size(){
  return end-start;
}

void read_maps(long* _start,long* _end){

  FILE *file;
  char* line;
  int counter = 0;

  offset = 0;

  sprintf(path,"/proc/%d/maps",getpid());

  file = fopen(path,"r");

  fread(buf,FILE_SIZE,1,file);

  while( (line=get_line()) != NULL){

    if(contains_dev_nvidia(line)){

      mapped_compile(line);

      _start[counter] = start;
      _end[counter] = end;

      printf("start[%d]:%ld\n",counter,start);
      printf("end  [%d]:%ld\n",counter,end);

      counter ++;
    }
  }
}
