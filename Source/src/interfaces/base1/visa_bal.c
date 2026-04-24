#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <define.h>
#include <queue.h>
#include <param_arg.h>
#include <thread_var.h>
#include <errno.h>
#include <limits.h>

void           *
thread_proc_BAL(void *arg)
{
	TSBalExchange   sBalMsg;
	int             nIndiceCtx;
	char            sLine[MAX_LINE_TRC];
	struct timeval	lastTime;
	int				nResult;

	pthread_detach(pthread_self());
	nIndiceCtx = ((TSArgParam *) arg)->nIndice;
	free(arg);
	sprintf(sLine, "THREAD BAL : %d", nIndiceCtx);
	trace_event(sLine, TRACE);

	ConnectOracle(nIndiceCtx);

	ThreadMaskSignals();

	gettimeofday(&lastTime,NULL);
	while (1) 
	{
		CBalWaitMsg();
		CBalGetMsg(&sBalMsg);
		CBalFreeEntry();
		nResult = CheckAndRefreshDBCon(nIndiceCtx,lastTime);

		BalProcMsg(nIndiceCtx, &sBalMsg);
		gettimeofday(&lastTime,NULL);
		memset((char *)&sBalMsg, 0, sizeof(TSBalExchange));
	}
}

void           *
thread_wait_BAL(void *arg)
{
	TSBalExchange   sBalMsg;
	int             i;
	pthread_attr_t  pattr;
	TSArgParam     *p_arg;
	int             rc;/*YDA20180208(PROD00053000)*/

	pthread_detach(pthread_self());


	ThreadMaskSignals();


	/**** Creation des Threads de traitement ***/
	for (i = 0; i < max_thread; i++) 
	{
		pthread_attr_init(&pattr);
		/*YDA20180208(PROD00053000)*/
		/*if (i < nUserThread)
			pthread_attr_setscope(&pattr, PTHREAD_SCOPE_PROCESS);
		else
			pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);*/

#ifdef SOLARIS
		if (rc = pthread_attr_setstacksize(&pattr, 8192 * 128 ))
#else
		if (rc = pthread_attr_setstacksize(&pattr, PTHREAD_STACK_MIN + 1024 * 1024))
#endif
		/*if (rc = pthread_attr_setstacksize(&pattr, PTHREAD_STACK_MIN + 1024*1024))*/
		{
			fprintf(stderr,"ERROR: rc from pthread_attr_setstacksize() is %d\n", rc);
		}
		p_arg = (TSArgParam *) malloc(sizeof(TSArgParam));
		AllocateOracleCtx(i + RESERVED_THREAD);
		p_arg->nIndice = i + RESERVED_THREAD;

		if (pthread_create(&(tab_ThrInfo[i + RESERVED_THREAD].tid), &pattr,
				   thread_proc_BAL, p_arg) < 0) {
			fprintf(stderr, "pthread_create Error %d\n", errno);
			free(p_arg);
			pthread_exit(NULL);
		}
	}


	while (1) {
		if (ReadBalMsg(nBalMsg, getpid(), !IPC_NOWAIT, &sBalMsg) < 0) 
		{
			fprintf(stderr, " >>> !!! Error Reading Message !!! \n");
			pthread_exit(NULL);
		}
		gettimeofday(&sBalMsg.StimeB,NULL);/*PLUTONL-3762*/
		PBalWaitEntry();
		PBalPutMsg(&sBalMsg);
		PBalConsoInfo();
		memset((char *)&sBalMsg, 0, sizeof(TSBalExchange));
	}
}
