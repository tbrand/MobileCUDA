#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <comm.h>
#include <nvml.h>

typedef struct _procs{
  proc_data *data;
  int sd;
  int queued;
  int staying_pos;
  int created_context;
  struct _procs *prev,*next;
} proc;

typedef struct _cons{
  int sd;
  struct _cons *prev,*next;
} cons;

typedef struct _dflag{
  int sd;
  int flag;
  int stayed;
  int exclusive;
  int context;
  size_t reserved;
} dflag;

typedef struct _DEM{
  proc *p0,*p1;
  cons *c0,*c1;
  nvmlDevice_t* devs;
  unsigned int ndev;
  dflag *flags;
  unsigned int procCounter;
  unsigned int staying_procs;
} DEM;

extern int MAXPROC;
extern int CONTEXT_NUM;

#define M64 (64 << 20)

#define TRUE  1
#define FALSE 0

#define ACTIVE        0
#define QUEUED        1
#define STAYED        2
#define STAYED_QUEUED 3
#define BACKUP        4
#define EXC_READY     5
#define SENT_REQUEST  6
#define FIN           7

extern DEM dem;

//dequeue.c
extern void dequeueSpecifyProc(proc*);
extern void dequeueSpecifyDevNO(int);
extern void dequeue();
extern int queue_size();
extern void exclusive_check(int);

//proc.c
extern void print_procs();
extern void init_proc();
extern proc* get_proc(int);
extern proc* get_proc_active(int);
extern proc* get_proc_staying_pos(int);
extern void add_proc(proc*);
extern void remove_proc(proc*);
extern proc* create_proc(int);
extern void mocu_check();
extern proc* staying_proc();
extern proc* staying_exclusive_proc();
extern int has_proc_sd(int);
extern int num_of_active_procs(int);
extern int num_of_staying_exclusive_procs();
extern proc* ready_exclusive_proc(int);
extern int has_staying_cannot_move_proc(int);
extern int exclusive_proc_can_migrate_to(int);

//connect.c
extern void _FIN(int);
extern void _CONNECT(int,proc_data*);
extern void _RENEW(int,proc_data*);
extern void _MIGDONE(int,proc_data*);
extern void _CANRECEIVE(int,proc_data*);
extern void _FAILEDTOALLOC(int,proc_data*);
extern void _CUDAMALLOC(int,proc_data*);
extern void _BACKUPED(int,proc_data*);
extern void _CONTEXT_CHECK(int,proc_data*);
extern void _CONSOLE(int);
extern void MSEND(int,int,pid_t,int,int,size_t,size_t);

//cons.c
extern void init_cons();
extern cons* get_cons(int);
extern void add_cons(cons*);
extern void remove_cons(cons*);
extern cons* create_cons(int);
extern void print_cons();

//stamp.c
extern void TIME_STAMP(proc*);
extern void print_records();
extern void register_name(int);
