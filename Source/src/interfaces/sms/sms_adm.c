#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <param_arg.h>
#include <thread_var.h>
#include <signal.h>

#include <define.h>
#include <queue.h>

/*****************************************************************/
void * thread_ADMIN(void * arg)
{
	TSBalExchange   sBalMsg;
	int				nIndiceCtx;
	int				nResult;
	struct timeval	lastTime;

   pthread_detach(pthread_self());

	ThreadMaskSignals();

   trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
   trace_event("> Start thread_ADMIN >>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
   trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);

   AllocateOracleCtx(ADM_THREAD_ID);
   ConnectOracle (ADM_THREAD_ID);
   
	nIndiceCtx= ADM_THREAD_ID;
	gettimeofday(&lastTime,NULL);
   while (1)
   {
      if ( ReadBalMsg(nBalAdm, getpid(), !IPC_NOWAIT, &sBalMsg) < 0)
      {
         fprintf(stderr," Error Reading Message \n");
         pthread_exit(NULL);
      }
		nResult = CheckAndRefreshDBCon(nIndiceCtx,lastTime);

      AdmProcMsg(ADM_THREAD_ID, &sBalMsg);
   }
}

