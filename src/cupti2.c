#include <mocu.h>
#include <cuda.h>
#include <cupti.h>
#include <cupti_events.h>
#include <pthread.h>
#include <sys/time.h>

#define CUPTI_RESULT(res) printf("\tcuptiErr : %d\n",res);

CUcontext _context;
CUdevice device;
int status;
int profile_status = PROFILE_BEFORE;
char *eventName;
char *_inst_executed = "inst_executed";
char *_gld_request   = "gld_request";
char *_gst_request   = "gst_request";
pthread_t pThread;

RuntimeApiTrace_t trace;

void* sampling_func(void *arg){

  printf("Come here!!\n");

  CUptiResult cuptiErr;
  CUpti_EventGroup eventGroup;
  CUpti_EventID instId,gldId,gstId;
  size_t bytesRead; 
  uint64_t eventVal;

  cuptiErr = cuptiSetEventCollectionMode(_context, CUPTI_EVENT_COLLECTION_MODE_CONTINUOUS);

  CUPTI_RESULT(cuptiErr);

  cuptiErr = cuptiEventGroupCreate(_context, &eventGroup, 0);

  CUPTI_RESULT(cuptiErr);

  cuptiErr = cuptiEventGetIdFromName(device, _inst_executed, &instId);
  CUPTI_RESULT(cuptiErr);
  cuptiErr = cuptiEventGetIdFromName(device, _gld_request, &gldId);
  CUPTI_RESULT(cuptiErr);
  cuptiErr = cuptiEventGetIdFromName(device, _gst_request, &gstId);
  CUPTI_RESULT(cuptiErr);printf("0\n");

  cuptiErr = cuptiEventGroupAddEvent(eventGroup, instId);
  CUPTI_RESULT(cuptiErr);printf("1\n");
  cuptiErr = cuptiEventGroupAddEvent(eventGroup, gldId);
  CUPTI_RESULT(cuptiErr);
  cuptiErr = cuptiEventGroupAddEvent(eventGroup, gstId);
  CUPTI_RESULT(cuptiErr);

  cuptiErr = cuptiEventGroupEnable(eventGroup);
  CUPTI_RESULT(cuptiErr);

  while(profile_status == PROFILING){

    bytesRead = sizeof(eventVal);

    cuptiErr = cuptiEventGroupReadEvent(eventGroup, 
					CUPTI_EVENT_READ_FLAG_NONE, 
					instId, &bytesRead, &eventVal);
    CUPTI_RESULT(cuptiErr);
  
    /*
      if (bytesRead != sizeof(eventVal)) {
      printf("Failed to read event %d\n",instId);
      exit(-1);
      }
    */

    if((unsigned long long)eventVal != 0){
      //      printf("\t\t%s: %llu\n",_inst_executed,(unsigned long long)eventVal);
      trace.inst = (uint64_t)eventVal;
    }

    cuptiErr = cuptiEventGroupReadEvent(eventGroup, 
					CUPTI_EVENT_READ_FLAG_NONE, 
					gldId, &bytesRead, &eventVal);
    CUPTI_RESULT(cuptiErr);

    /*
      if (bytesRead != sizeof(eventVal)) {
      printf("Failed to read event %d\n",gldId);
      exit(-1);
      }
    */

    if((unsigned long long)eventVal != 0){
      //      printf("\t\t%s: %llu\n",_gld_request,(unsigned long long)eventVal);
      trace.gld = (uint64_t)eventVal;
    }

    cuptiErr = cuptiEventGroupReadEvent(eventGroup, 
					CUPTI_EVENT_READ_FLAG_NONE, 
					gstId, &bytesRead, &eventVal);
    CUPTI_RESULT(cuptiErr);

    /*
      if (bytesRead != sizeof(eventVal)) {
      printf("Failed to read event %d\n",gstId);
      exit(-1);
      }
    */

    if((unsigned long long)eventVal != 0){
      //      printf("\t\t%s: %llu\n",_gst_request,(unsigned long long)eventVal);
      trace.gst = (uint64_t)eventVal;
    }

    if(trace.inst != 0 &&
       trace.gld  != 0 &&
       trace.gst  != 0){
      profile_status = PROFILE_DONE;
    }

    printf("%llu\t$llu\t%llu",trace.inst,trace.gld,trace.gst);

    usleep(1000000);

  }

  cuptiErr = cuptiEventGroupDisable(eventGroup);
  CUPTI_RESULT(cuptiErr);

  cuptiErr = cuptiEventGroupDestroy(eventGroup);
  CUPTI_RESULT(cuptiErr);

}

void mocu_init_profile(){

  printf("\tProfile Init\n");

  CUresult res;
  
  res = cuCtxGetCurrent(&_context);

  //  printf("cuCtxGetCurrent : %d\n",res);

  res = cuDeviceGet(&device,mocu_pos);

  //  printf("cuDeviceGet : %d\n",res);
}

void mocu_take_profile(){

  printf("\tProfile Take\n");

  profile_status = PROFILING;
  
  status = pthread_create(&pThread, NULL, sampling_func, NULL);

  //  printf("pthread_create status : %d\n",status);

  if(status != 0){
    exit(-1);
  }
}

void mocu_destroy_profile(){

  printf("\tProfile Destroy\n");

  profile_status = PROFILE_DONE;

  pthread_join(pThread, NULL);
  
}

void getProfileTrace(RuntimeApiTrace_t *t){
  *t = trace;
}
