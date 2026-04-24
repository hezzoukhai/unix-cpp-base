#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <define.h>
#include <signal.h>
#include <iso_sms.h>
#include <iso_hps.h>
#include <sms_param.h>


void * thread_SAF(void * arg)
{
	char   sLine [ MAX_LINE_TRC ];
	struct timeval	lastTime;
	int			nResult;
	int			nIndiceCtx;
	int			nSleepTime = 1;
	
	
	

	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	trace_event(">>>>>> Start thread_proc_SAF >>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);


	pthread_detach(pthread_self());


	ThreadMaskSignals();

	AllocateOracleCtx(SAF_THREAD_ID);
	ConnectOracle (SAF_THREAD_ID );

	fprintf ( stderr,"\n Thread SAF ID = %lu ", pthread_self() );
	nIndiceCtx=SAF_THREAD_ID;
	gettimeofday(&lastTime,NULL);

	while (1)
	{
		sleep(nSleepTime);
		if (!memcmp(ResourceStatus, SIGNON, 3))
		{
			nResult = CheckAndRefreshDBCon(nIndiceCtx,lastTime);
			nResult = SafManagement( nIndiceCtx , ResourceId );
			if( nResult == OK )
				nSleepTime = 1;
			else
				nSleepTime = 20;
			gettimeofday(&lastTime,NULL);
		}
		else
		{
			nSleepTime = 20;
		}

	}
}


