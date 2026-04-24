#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <define.h>
#include <signal.h>
#include <iso_visa.h>
#include <iso_hps.h>
#include <int1_struct.h>
#include <int1.h>



void* int1_thread_SAF(void *arg)
{
	char            sLine[MAX_LINE_TRC];
	struct timeval	lastTime;
	int				nResult;
	int				nIndiceCtx;
	int				nSleepTime = 1;


	trace_event(">==============================<", FATAL);
	trace_event(">=== thread_SAF ===============<", FATAL);
	trace_event(">==============================<", FATAL);
	pthread_detach(pthread_self());
	ThreadMaskSignals();
	AllocateOracleCtx(SAF_THREAD_ID);
	ConnectOracle(SAF_THREAD_ID);

	nIndiceCtx=SAF_THREAD_ID;
	gettimeofday(&lastTime,NULL);
	while (1)
   	{
		sleep(nSleepTime);
		if (!memcmp(ResourceStatus, SIGNON, 3))
		{
			nResult = CheckAndRefreshDBCon(nIndiceCtx,lastTime);
			nResult = Int1SafManagement( nIndiceCtx , ResourceId );
			if( nResult == OK )
				nSleepTime = 5;
			else
				nSleepTime = 20;
			gettimeofday(&lastTime,NULL);
		}
		else
			sleep(2*10);
		
   	}
}
