#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>

#include <define.h>
#include <queue.h>
#include <iso_hps.h>
#include <param_arg.h>


/***********************************************************************/
void *thread_dbcon_refresh(void * arg)
{

	int				i;
	int				nIndiceCtx;
	

	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	trace_event("> Start thread_dbcon_refresh >>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);

	pthread_detach(pthread_self());
	nIndiceCtx = ((TSArgParam *)arg)->nIndice;
	free(arg);

	ThreadMaskSignals(); 

	while(1)
	{
		WaitDBConRequest(nIndiceCtx);
		ProcessDBConRequest(nIndiceCtx);
		FreeDBConThread(nIndiceCtx);
	}


	trace_event("End   thread_dbcon_refresh()", PROCESSING);

	pthread_exit((void *)OK);
}
