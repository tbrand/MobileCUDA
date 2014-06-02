#include <driver_types.h>
#include <cuda_runtime_api.h>
#include <__cudaFatFormat.h>
#include <vector_types.h>
#include <signal.h>

/*BOTH*/
#define CONNECT     1
#define GOAHEAD     18

/*Deamon to client*/
#define MIGRATE     3
#define SUSPEND     7
#define CANNOTENTER 11
#define STAY        12

/*Client to deamon*/
#define CANRECEIVE  2
#define FAILEDTOGET 4
#define RENEW       6
#define CUDAMALLOC  9
#define MIGDONE     10
#define BACKUPED    13
#define MALLOCDONE  19

/*Console to deamon*/
#define CONSOLE     14

/*Deamon to console*/
#define CONS_ADD         15
#define CONS_REMOVE      16
#define CONS_RENEW       17

#define NONE      0
#define CANMIG    (1 << 0)
#define EXCLUSIVE (1 << 1)

typedef struct _proc_data{
  int REQUEST;
  pid_t pid;
  int flag;
  int pos;
  size_t mem;
  size_t req;
} proc_data;
