#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <list_thr.h>
#include <define.h>
#include <errno.h>
#include <queue.h>
#include <p7_sid.h>
#include <iso_fields.h>
#include <thread_var.h>
#include <iso_hps.h>
#include <tlv_autho.h>
#include <param_arg.h>

TSNodeStruct ListHeadFrd;

void*  thread_frd_proc(void *arg);
void*  thread_frd_timeout(void *arg);

/*EBETODO Need to watch the new Bal for orphan messages*/


int InitFraudModule()
{

	pthread_mutexattr_t mattr;


	trace_event("Start InitFraudModule()", PROCESSING);
	
	ListHeadFrd.Next = NULL;

	pthread_mutexattr_init(&mattr);
	pthread_mutex_init( &(ListHeadFrd.lock) , &mattr);
	pthread_mutexattr_destroy(&mattr);

	InitNodeData( &(ListHeadFrd.data));


	nBalFrd = OpenBalCom(BAL_FRD);
	if (nBalFrd < 0)
	{
		trace_event("Can Not Open Fraud Queue", FATAL);
		exit(0);
	}

	EmptyQueue(nBalFrd, getpid());

	if (pthread_create(&(tab_ThrInfo[FRAUD_PROC_THREAD_ID].tid), NULL, thread_frd_proc, NULL) < 0)
	{
		fprintf(stderr, "pthread_create (Fraud Proc) Error %d\n", errno);
		exit(0);
	}

	if (pthread_create(&(tab_ThrInfo[FRAUD_TO_THREAD_ID].tid), NULL, thread_frd_timeout, NULL) < 0)
	{
		fprintf(stderr, "pthread_create (Fraud timeout) Error %d\n", errno);
		exit(0);
	}

	trace_event("End	InitFraudModule(OK)", PROCESSING);
	return OK;
}



void* thread_frd_proc(void *arg)
{
	TSBalExchange   sBalMsg;
	TSIsoInfo       IsoInfo;
	TSTlvAutho		sTlvInfo;
	TSNodeData      Node;
	TSArgParam     *p_arg;
	int				result;
	int				nFieldLength;
	char			sFieldData[BUF_LG_MAX];
	char			sLine[MAX_LINE_TRC];


	pthread_detach(pthread_self());

	ThreadMaskSignals();

	while (1) 
	{
		if (ReadBalMsg(nBalFrd, getpid(), !IPC_NOWAIT, &sBalMsg) < 0) 
		{
			fprintf(stderr, " Error Reading Message \n");
			pthread_exit(NULL);
		}

		InitIsoInfo(&IsoInfo);
		AnalyseIso(sBalMsg.sBody, &IsoInfo);


		InitNodeData(&Node);

		GetIsoField(ISO_AUDIT_NBR, &IsoInfo, sFieldData, &nFieldLength);
		memcpy(Node.szFstKey, sFieldData, P7_F011_LEN);

		GetIsoField(ISO_XMIT_TIME, &IsoInfo, sFieldData, &nFieldLength);
		memcpy(Node.szFstKey + P7_F011_LEN, sFieldData, nFieldLength);
		
		memset(sFieldData,0, strlen(sFieldData));

		GetIsoField(ISO_RESPONSE_CODE, &IsoInfo, sFieldData, &nFieldLength);

		result = UpdateFraudRequest (&Node, sFieldData);
		if( result != OK )
		{
			sprintf(sLine,"Error Update Fraud Request[%d][%s][%s]",result,Node.szFstKey,sFieldData);
			continue;
		}
		
	}
}


void* thread_frd_timeout(void *arg)
{
	TSHsmExchange   sBalMsg;
	TSArgParam     *p_arg;

	pthread_detach(pthread_self());

	ThreadMaskSignals();

	while (1) 
	{
		sleep(5);
		FreeTimeOutFraudRequest();
	}

}

