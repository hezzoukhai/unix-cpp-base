#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <define.h>
#include <signal.h>
#include <iso_sms.h>
#include <iso_hps.h>
#include <sms_param.h>


void * thread_OUTG(void * arg)
{
	char   sLine [ MAX_LINE_TRC ];




	pthread_detach(pthread_self());

	ThreadMaskSignals();

	AllocateOracleCtx(OUTG_THREAD_ID);
	ConnectOracle (OUTG_THREAD_ID );

	fprintf ( stderr,"\n Thread OUTGOING ID = %d ", pthread_self() );
	while (1)
	{
		OutgoingManagement( OUTG_THREAD_ID , ResourceId );
	}

	return(NULL);
}

