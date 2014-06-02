#include <mocu.h>
#include <string.h>
#include <fatBinaryCtl.h>
#include <fatbinary.h>
#include <elf.h>

void mocu_parse(void* fatCubin){

  __fatBinC_Wrapper_t* fbinw;
  struct fatBinaryHeader* header;

  fbinw = (__fatBinC_Wrapper_t*)fatCubin;

#if 0
  printf("magic   : %p\n",fbinw->magic);
  printf("version : %u\n",fbinw->version);
  printf("data    : %p\n",fbinw->data);
  printf("data    : %p\n",*(fbinw->data));
#endif

  header = (struct fatBinaryHeader*)fbinw->data;

#if 0
  printf("header->magic      : %p\n",header->magic);
  printf("header->versoin    : %d\n",header->version);
  printf("header->headerSize : %d\n",header->headerSize);
  printf("header->fatSize    : %d\n",header->fatSize);
#endif

  char *cubin;
  int i,len_header,len_fat;

  len_header = header->headerSize;
  len_fat    = header->fatSize;

  cubin = (char*)fbinw->data;

  while(1){

    if(
       *cubin     == 0x7f &&
       *(cubin+1) == 'E'  &&
       *(cubin+2) == 'L'  &&
       *(cubin+3) == 'F'
       ){

      //      printf("Found 0x7f E L F [%p]\n",cubin);

      break;

    }

    cubin++;

  }

  int find = 0;
  char *p,*q;
  Elf64_Ehdr *elfhdr;
  Elf64_Phdr *phdr;
  Elf64_Shdr *shdr;
  Elf64_Half phentsize;
  Elf64_Half phnum;
  Elf64_Half shentsize;
  Elf64_Half shnum;

  elfhdr = (Elf64_Ehdr *)cubin;

  phdr = (Elf64_Phdr *)(cubin + elfhdr->e_phoff);
  shdr = (Elf64_Shdr *)(cubin + elfhdr->e_shoff);

  phentsize = elfhdr->e_phentsize;
  phnum = elfhdr->e_phnum;
  shentsize = elfhdr->e_shentsize;
  shnum = elfhdr->e_shnum;

  for(i = 0 ; i < shnum ; i ++){
    shdr = (Elf64_Shdr *)(cubin + elfhdr->e_shoff + i*shentsize);

    if(shdr->sh_type == SHT_STRTAB){
      p = cubin + shdr->sh_offset;
      q = p + shdr->sh_size;

      while(p < q){

	while(*p == '\0') p++;

	if(p >= q)break;

	if(strncmp(p,"malloc",6) == 0){

	  //	  printf("find malloc!!\n");
	  find = 1;

	}else{
	  
	  //	  printf("Symbol found : %s\n",p);

	  //	  mocu_add_symbol(p);

	}

	while(*p != '\0') p++;

	p++;

      }
    }

    if(find)break;
  }
}
