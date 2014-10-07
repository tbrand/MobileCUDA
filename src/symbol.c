#include <mocu.h>

unsigned long total_symbol_size(){
  symbol *s;
  unsigned long size = 0;
  
  s = mocu.cp->sm0->next;

  while(s->next != NULL){
    //    printf("Symbol size : %u\n",s->size);
    size += s->size;
    s = s->next;
  }

  return size;
}


void print_symbol(){

  printf("\t*****************************\n");

  symbol* stemp;

  stemp = mocu.cp->sm0->next;

  while(stemp->next != NULL){

    printf("\thostVar : %p\n",stemp->hostVar);
    printf("\tdeviceAddress : %p\n",stemp->deviceAddress);
    printf("\tdeviceName : %s\n",stemp->deviceName);
    printf("\text : %d\n",stemp->ext);
    printf("\tsize : %d\n",stemp->size);
    printf("\tconstant : %d\n",stemp->constant);
    printf("\tglobal : %d\n",stemp->global);

    stemp = stemp->next;
  }

  printf("\t*****************************\n");
}

void mocu_add_symbol(char* hostVar,
		     char* deviceAddress,
		     char* deviceName,
		     int ext,
		     int size,
		     int constant,
		     int global
		     ){
  
  symbol* s;
  
  s = (symbol*)malloc(sizeof(symbol));

  s->hostVar = hostVar;
  s->deviceAddress = deviceAddress;
  s->deviceName = deviceName;
  s->ext = ext;
  s->size = size;
  s->constant = constant;
  s->global = global;

  s->prev = mocu.cp->sm1->prev;
  s->next = mocu.cp->sm1;
  s->prev->next = s;
  s->next->prev = s;

  //  print_symbol();
}

int mocu_symbol_registered(symbol* s){

  apilog* a;

  a = mocu.cp->a0->next;
  while(a->next != NULL){
    if(a->type == REGISTERVAR){
      if(a->data.registerVar.hostVar == s->hostVar){
	return 1;
      }
    }
    a = a->next;
  }

  return 0;
}

void mocu_symbol_backup(){

  if(!created_context)return;

  //  printf("Symbol backup !!!!\n");

  symbol* s;
  cudaError_t res;

  s = mocu.cp->sm0->next;

  while(s->next != NULL){

    if(mocu_symbol_registered(s)){

      s->backup = (char*)malloc(s->size);
    
      res = mocu.mocudaMemcpyFromSymbol(s->backup,s->hostVar,s->size,0,cudaMemcpyDeviceToHost);

#if 0
      if(res == cudaSuccess){
	printf("Success to backup (symbol backup)\n");
      }else{
	printf("Failed to backup (symbol backup)(%d)(%p)\n",res,s->hostVar);
      }
#endif

    }else{

      //      printf("this symbol is not registered(%s)\n",s->deviceName);

    }
    
    s = s->next;
  }
}

void mocu_symbol_restore(){

  if(!created_context)return;

  //  printf("Symbol restore !!!!\n");

  cudaError_t res;
  symbol* s;
  
  s = mocu.cp->sm0->next;

  while(s->next != NULL){

    if(mocu_symbol_registered(s)){

      res = mocu.mocudaMemcpyToSymbol(s->hostVar,s->backup,s->size,0,cudaMemcpyHostToDevice);

#if 0
      if(res == cudaSuccess){
	printf("Success to restore(symbol)\n");
      }else{
	printf("Failed to restore(symbol)(%d)\n",res);
	exit(-1);
      }
#endif

    }else{

      //      printf("this symbol is not registered(%s)\n",s->deviceName);

    }

    s = s->next;
  }
}
