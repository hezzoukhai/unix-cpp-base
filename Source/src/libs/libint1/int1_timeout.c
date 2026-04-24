#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <define.h>
#include <struct_time.h>
#include <param_arg.h>
#include <thread_var.h>
#include <int1_struct.h>
#include <int1.h>

/***********************************************************************/
int UserInt1TimeProcMsg(int nIndexCtx, TSTimeExchange* pkTimeExchange);
/***********************************************************************/

static void* int1_thread_proc_TIME(void *arg)
{
	TSTimeExchange  sTimeMsg;
	int             nIndiceCtx;
	char            sLine[MAX_LINE_TRC];
	struct timeval	lastTime;
	int				nResult;


	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	trace_event("> Start thread_TIMEOUT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);

	pthread_detach(pthread_self());
	nIndiceCtx = ((TSArgParam *) arg)->nIndice;
	free(arg);

	sprintf(sLine, "THREAD TIME : %d", nIndiceCtx);
	trace_event(sLine, TRACE);

	ConnectOracle(nIndiceCtx);
	ThreadMaskSignals();

	gettimeofday(&lastTime,NULL);
	while (1) 
	{
		CTimeWaitMsg();
		CTimeGetMsg(&sTimeMsg);
		CTimeFreeEntry();

		nResult = CheckAndRefreshDBCon(nIndiceCtx,lastTime);
		UserInt1TimeProcMsg(nIndiceCtx, &sTimeMsg);
		gettimeofday(&lastTime,NULL);
	}
}

void* int1_thread_TIMEOUT(void *arg)
{
	TSTimeExchange  sTimeMsg;
	int             i;
	pthread_attr_t  pattr;
	TSArgParam     *p_arg;

	unsigned int nMaxSleepTime, nMinSleepTime, nSleepTime;
	struct	timeval		stNextTimeout;

	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	trace_event("> Start thread_TIMEOUT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);

	pthread_detach(pthread_self());
	ThreadMaskSignals();

	/**** Creation des Threads de traitement ***/
	for (i = 0; i < 5; i++) 
	{
		pthread_attr_init(&pattr);
		pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);
		p_arg = (TSArgParam *) malloc(sizeof(TSArgParam));
		AllocateOracleCtx(i + 15);
		p_arg->nIndice = i + 15;
		if (pthread_create(&(tab_ThrInfo[i + 15].tid), &pattr,int1_thread_proc_TIME, p_arg) < 0) 
		{
				fprintf(stderr, "pthread_create Error %d\n", errno);
				free(p_arg);
				pthread_exit(NULL);
		}
	}

	nMinSleepTime = 100000; /* 100 ms */
	nMaxSleepTime = 3000000; /* 3 seconds */

	while (1) {

		FreeTimeOutRequest ('B');
		FreeTimeOutRequest ('L');
		GetNextTimeOut(&stNextTimeout);
		nSleepTime = stNextTimeout.tv_sec*1000000 + stNextTimeout.tv_usec;

		if( nSleepTime == 0 || nSleepTime > nMaxSleepTime )
		{
			nSleepTime = nMaxSleepTime;
		}
		else if( nSleepTime < nMinSleepTime )
		{
			nSleepTime = nMinSleepTime;
		}
		usleep(nSleepTime + 1);

	}
}
