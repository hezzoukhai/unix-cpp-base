#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <define.h>
#include <queue.h>
#include <iso_hps.h>
#include <param_arg.h>
#include <errno.h>

#include <int1_struct.h>
#include <int1.h>

/***********************************************************************/
int UserInt1AdmProcMsg(int nIndexCtx, TSBalExchange* pkBalExchange);
/***********************************************************************/


void* int1_thread_ADMIN(void *arg)
{
	TSBalExchange   sBalMsg;

	pthread_detach(pthread_self());

	ThreadMaskSignals();

	AllocateOracleCtx(ADM_THREAD_ID);
	ConnectOracle(ADM_THREAD_ID);

	while (1) 
	{
		if (ReadBalMsg(nBalAdm, getpid(), !IPC_NOWAIT, &sBalMsg) < 0) 
		{
            fprintf(stderr, " Error Reading Message from BAL with ThreadID [%lu] ( errno : %d ) \n", p7_thread_id(), errno);
			trace_event("Error Reading Message from queue ", FATAL);
			pthread_exit(NULL);
		}
		UserInt1AdmProcMsg(ADM_THREAD_ID, &sBalMsg);
	}
}
