#include <mocu.h>
#include <cupti.h>
#include <cuda.h>

typedef struct cupti_eventData_st{
  CUpti_EventGroup eventGroup;
} cupti_eventData;

CUpti_EventID inst_executed, gst_request, gld_request;
CUpti_SubscriberHandle subscriber;
CUptiResult res;
RuntimeApiTrace_t trace;

void CUPTIAPI getEventValueCallback(void *userdata, CUpti_CallbackDomain domain,CUpti_CallbackId cbid, const CUpti_CallbackData *cbInfo){

  RuntimeApiTrace_t *traceData;
  size_t bytesRead;

  traceData = (RuntimeApiTrace_t*)userdata;

  if(cbid != CUPTI_RUNTIME_TRACE_CBID_cudaLaunch_v3020){
    printf("%s:%d: unexpected cbid %d\n", __FILE__, __LINE__, cbid);
    exit(-1);
  }

  if(cbInfo->callbackSite == CUPTI_API_ENTER){

    cudaThreadSynchronize();

    res = cuptiSetEventCollectionMode(cbInfo->context,CUPTI_EVENT_COLLECTION_MODE_KERNEL);

    //    printf("res collection mode : %d\n",res);

    if(traceData->eventGroup == NULL){
      printf("NULL : \n");
    }

    res = cuptiEventGroupEnable(traceData->eventGroup);

    //    printf("res enable          : %d\n",res);
  }

  if(cbInfo->callbackSite == CUPTI_API_EXIT){

    cudaThreadSynchronize();

    bytesRead = sizeof(uint64_t);

    cuptiEventGroupReadEvent(
			     traceData->eventGroup,
			     CUPTI_EVENT_READ_FLAG_NONE,
			     inst_executed,
			     &bytesRead, &traceData->inst
			     );

    bytesRead = sizeof(uint64_t);

    cuptiEventGroupReadEvent(
			     traceData->eventGroup,
			     CUPTI_EVENT_READ_FLAG_NONE,
			     gld_request,
			     &bytesRead, &traceData->gld
			     );

    bytesRead = sizeof(uint64_t);

    cuptiEventGroupReadEvent(
			     traceData->eventGroup,
			     CUPTI_EVENT_READ_FLAG_NONE,
			     gst_request,
			     &bytesRead, &traceData->gst
			     );

    cuptiEventGroupDisable(traceData->eventGroup);
  }
}

void init_cupti(){

  CUresult res;
  CUcontext ctx;
  CUdevice dev;
  size_t bufsize;

  CUptiResult cupti_res;
  CUpti_EventDomainID *ev_domain_buffer;
  CUpti_EventID *event_id_buffer;
  size_t event_domain_size;
  uint32_t num_event_domain;
  char namebuf[512];

  int i,j;

  res = cuCtxGetCurrent(&ctx);

  //  printf("\t\t\tRESULT : %d\n",res);

  cuDeviceGet(&dev,mocu_pos);

  cuptiDeviceGetNumEventDomains(dev,&num_event_domain);

  ev_domain_buffer = (CUpti_EventDomainID*)malloc(sizeof(CUpti_EventDomainID)*num_event_domain);

  event_domain_size = sizeof(CUpti_EventDomainID)*num_event_domain;

  cuptiDeviceEnumEventDomains(dev,
			      &event_domain_size,
			      ev_domain_buffer);

  for(i = 0 ; i < num_event_domain ; i ++){

    bufsize = sizeof(namebuf);

    uint32_t num_event;

    cuptiDeviceGetEventDomainAttribute(dev,
				       ev_domain_buffer[i],
				       CUPTI_EVENT_DOMAIN_ATTR_NAME,
				       &bufsize,
				       namebuf);

    cuptiEventDomainGetNumEvents(ev_domain_buffer[i], &num_event);

    event_id_buffer = (CUpti_EventID *)malloc(sizeof(CUpti_EventID) * num_event);

    size_t evsize = sizeof(CUpti_EventID) * num_event;

    cuptiEventDomainEnumEvents(ev_domain_buffer[i], &evsize, event_id_buffer);

    for (j = 0; j < num_event; j++) {

      bufsize = sizeof(namebuf);
      res = cuptiEventGetAttribute(event_id_buffer[j],
				   CUPTI_EVENT_ATTR_NAME,
				   &bufsize,
				   namebuf);

      if (strcmp(namebuf, "gst_request") == 0) {
	gst_request = event_id_buffer[j];
	//	printf("gst_request\n");
      }

      if (strcmp(namebuf, "gld_request") == 0) {
	gld_request = event_id_buffer[j];
	//	printf("gld_request\n");
      }

      if (strcmp(namebuf, "inst_executed") == 0) {
	inst_executed = event_id_buffer[j];
	//	printf("inst_executed\n");
      }
    }
  }
  
  cupti_res = cuptiEventGroupCreate(ctx, &trace.eventGroup, 0);
  //  printf("res=%d\n", res);
  cupti_res = cuptiEventGroupAddEvent(trace.eventGroup, inst_executed);
  //  printf("res=%d\n", res);
  cupti_res = cuptiEventGroupAddEvent(trace.eventGroup, gld_request);
  //  printf("res=%d\n", res);
  cupti_res = cuptiEventGroupAddEvent(trace.eventGroup, gst_request);
  //  printf("res=%d\n", res);
  
  cuptiSubscribe(&subscriber, (CUpti_CallbackFunc)getEventValueCallback,&trace);
  cuptiEnableCallback(1,
		      subscriber,
		      CUPTI_CB_DOMAIN_RUNTIME_API,
		      CUPTI_RUNTIME_TRACE_CBID_cudaLaunch_v3020);
}

void cupti_destroy(){

  /*

  //  printf("inst : %llu\n",(unsigned long long)trace.inst);
  //  printf("gld  : %llu\n",(unsigned long long)trace.gld);
  //  printf("gst  : %llu\n",(unsigned long long)trace.gst);
  
  res = cuptiEventGroupRemoveEvent(trace.eventGroup, inst_executed);

  //  printf("Remove event(inst_executed) : %d\n",res);

  res = cuptiEventGroupRemoveEvent(trace.eventGroup, gld_request);

  //  printf("Remove event(gld_request) : %d\n",res);

  res = cuptiEventGroupRemoveEvent(trace.eventGroup, gst_request);

  //  printf("Remove event(gst_request) : %d\n",res);  

  res = cuptiEventGroupDestroy(trace.eventGroup);

  //  printf("Destroy : %d\n",res);

  res = cuptiUnsubscribe(subscriber);

  //  printf("Unsubscribe : %d\n",res);

  */
}

int getTrace(RuntimeApiTrace_t* _trace){

  *_trace = trace;

  return 0;

}
