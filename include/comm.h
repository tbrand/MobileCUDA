#include <driver_types.h>
#include <cuda_runtime_api.h>
#include <__cudaFatFormat.h>
#include <vector_types.h>
#include <signal.h>
#include <cupti.h>

/*BOTH*/
#define CONNECT     1

/*Deamon to client*/
#define GOAHEAD     18
#define MIGRATE       3
#define SUSPEND       7
#define CANNOTENTER   11
#define STAY          12
#define CCHECK_OK     21
#define CCHECK_FAILED 22
#define RECVREQUEST   23

/*Client to deamon*/
#define CANRECEIVE     2
#define FAILEDTOALLOC  4
#define RENEW          6
#define CUDAMALLOC     9
#define CUDAFREE       25
#define MIGDONE        10
#define BACKUPED       13
#define MALLOCDONE     19
#define CONTEXT_CHECK  20
#define CREATE_CONTEXT 21
#define PROFILE        24

/*Console to deamon*/
#define CONSOLE     14

/*Deamon to console*/
#define CONS_ADD         15
#define CONS_REMOVE      16
#define CONS_RENEW       17

#define NONE      0
#define CANNOTMIG (1 << 0)
#define EXCLUSIVE (1 << 1)

typedef struct RuntimeApiTrace_st {
  CUpti_EventGroup eventGroup;
  uint64_t inst;
  uint64_t gld;
  uint64_t gst;
} RuntimeApiTrace_t;

typedef struct _proc_data{
  int REQUEST;
  pid_t pid;
  int flag;
  int pos;
  size_t mem;
  size_t req;
  size_t sym;
  RuntimeApiTrace_t trace;
} proc_data;
