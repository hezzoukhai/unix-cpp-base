#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

#include <errno.h>
#include <define.h>
#include <signal.h>
#include <struct_line.h>
#include <resources.h>
#include <sms_param.h>
#include <param_arg.h>
#include <thread_var.h>
#include <event_log.h>

void * thread_proc_LINE(void * arg)
{
	TSLineExchange   sLineMsg;
	char             sLine   [MAX_LINE_TRC];
	int              nIndiceCtx;
	struct timeval	lastTime;
	int			nResult;

	pthread_detach(pthread_self());

	/* - oracle connection -------------------------------------------------- */
	nIndiceCtx = ((TSArgParam *)arg)->nIndice;
	free(arg);

	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);
	sprintf    (sLine, "> Start thread_proc_LINE (%d) >>>>>>>>>>>>>>>>>", nIndiceCtx);
	trace_event(sLine, TRACE);
	trace_event(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>", TRACE);


	ConnectOracle ( nIndiceCtx ); 


	ThreadMaskSignals();

	gettimeofday(&lastTime,NULL);
	while (1)
	{
		CLineWaitMsg();
		CLineGetMsg (&sLineMsg);
		CLineFreeEntry();

		trace_event("Message Received (C)", PROCESSING);
		nResult = CheckAndRefreshDBCon(nIndiceCtx,lastTime);
		LineProcMsg(nIndiceCtx, &sLineMsg);
		gettimeofday(&lastTime,NULL);
		memset((void *)&sLineMsg, 0, sizeof(TSLineExchange));
	}
}

void * thread_wait_LINE(void * arg)
{
	TSLineExchange   sLineMsg;
	int i;
	pthread_attr_t  pattr;
	TSArgParam     * p_arg;
	int CodeRet;

	pthread_detach(pthread_self());

	ThreadMaskSignals();

	/**** Creation des Threads de traitement ***/
	for (i = 0; i < max_thread; i++)
	{
		pthread_attr_init(&pattr);
		if ( i < nUserThread)
			pthread_attr_setscope (&pattr, PTHREAD_SCOPE_PROCESS);
		else
			pthread_attr_setscope (&pattr, PTHREAD_SCOPE_SYSTEM);
		p_arg = (TSArgParam *) malloc(sizeof(TSArgParam));
		AllocateOracleCtx(i + MAX_BAL_THREAD);
		p_arg->nIndice = i + MAX_BAL_THREAD;
		if (pthread_create(&(tab_ThrInfo[i+MAX_BAL_THREAD].tid), &pattr, 
			thread_proc_LINE, p_arg) < 0)
		{
			fprintf(stderr,"pthread_create Error %d\n", errno);
			free(p_arg);
			pthread_exit(NULL);
		}
	}

	/*
	** main loop that reads the message from the line and gives it
	** to the processing thread
	*/
	CodeRet = ConnectInterface(MAIN_THREAD_ID);
	if (OK != CodeRet)
	{
		trace_event ("ConnectInterface(MAIN_THREAD_ID) failed.", FATAL);
		exit(0);
	}
	
   CodeRet = retrieval_advice_on(MAIN_THREAD_ID);

	while (1)
	{
		/* - reading the message from the line ------------------------------- */
		CodeRet = ReadLineMsg(sLineMsg.sBuffer, &(sLineMsg.nLength));

		if (CodeRet != OK) /** A Network Problem **/
		{
			/*Start ICH20170706 readline raised by stop we don't need to reconnect 23062020 PLUTONL-1956*/
			if (memcmp(ResourceStatus, STOPI, 3) == 0 )
			{
				pthread_exit ( NULL );
			}
			/*End 23062020 PLUTONL-1956*/

			if (CodeRet == MALFUNCTION )
				continue;
			/* A Network Disconnect Was Received */
			if (ReconnectInterface(MAIN_THREAD_ID) != OK)
			{
				trace_event("ReconnectInterface(MAIN_THREAD_ID) failed.", FATAL);
				exit(0);
			}
			continue;
		}

		trace_event("Message Received (P)", PROCESSING);

		DumpSms(sLineMsg.sBuffer);

		gettimeofday(&sLineMsg.Stime,NULL);/*PLUTONL-3762*/
		PLineWaitEntry();
		PLinePutMsg   (&sLineMsg);
		PLineConsoInfo();
		memset((void *)&sLineMsg, 0, sizeof(TSLineExchange));
	}
}
/********************************************************************************/
/**************************   ConnectInterface   ********************************/
/********************************************************************************/
int 
ConnectInterface(int P_nIndiceCtx)
{
	TSResources    sResource;

	char            app_pid[9];
	char            app_ppid[9];

	char            szDetail[256];
	char            calling_adress[17];

	int             nRetCode;
	int				nResult;

	struct timeval	lastTime;

	sprintf(szDetail, "Start ConnectInterface(%d)", P_nIndiceCtx);
	trace_event(szDetail, PROCESSING);

	sprintf(app_pid,  "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());

	sleep(2);

	memset((char *) &sResource, 0, sizeof(sResource));
	memset(calling_adress, 0, sizeof(calling_adress));

	/* DB Connection */
	AllocateOracleCtx(P_nIndiceCtx);
	ConnectOracle(P_nIndiceCtx);

	/* Get Resource Parameters */
	memcpy (sResource.resource_id, ResourceId , 6);
	if ( GetResources ( P_nIndiceCtx, &sResource) != OK)
	{
		shutdown_resource( P_nIndiceCtx , ResourceId );
		trace_event("GetResources() failed.", FATAL);
		DeconnectOracle(P_nIndiceCtx);
		FreeOracleCtx(P_nIndiceCtx);
		exit(0);
	}


	if (UpdateResource(P_nIndiceCtx, ResourceId, CRITICAL, CONNECT) != OK) 
	{
		trace_event("UpdateResource() failed.", FATAL);
		shutdown_resource(P_nIndiceCtx, ResourceId);
		DeconnectOracle(P_nIndiceCtx);
		FreeOracleCtx(P_nIndiceCtx);
		exit(0);
	}

	/*AMER: 20160314
	LogEvent(P_nIndiceCtx, ResourceId, "", INTERFACE_SIGN_ON, "##", "##", "##", app_pid, app_ppid);
	*/

	/* Initialising outgoing call */

	if (memcmp (g_pris_connect_mode, "S", 1) == 0)
	{
	while (InitCltComms() != OK) 
	{
		trace_event(" Problem while Calling InitCltComms(). Wait 30s Before Retry", PROCESSING);
		sleep(30);
	}

	
	if (initial_sign_on_to_nw(P_nIndiceCtx) != OK) 
	{
		trace_event("initial_sign_on_to_nw() failed.", FATAL);
		shutdown_resource(P_nIndiceCtx, ResourceId);
		DeconnectOracle(P_nIndiceCtx);
		FreeOracleCtx(P_nIndiceCtx);
		StopCltCom(nFdLocal);
		exit(0);
	}
	}
	else
	{
		/******** Wait for incoming call ***************/
		if (InitSrvComms(atoi(LocalAppId)) != OK) 
		{
			shutdown_resource(P_nIndiceCtx, ResourceId);
			trace_event("InitSrvComms() failed.", FATAL);
			DeconnectOracle(P_nIndiceCtx);
			FreeOracleCtx(P_nIndiceCtx);
			exit(0);
		}
		memcpy(ResourceStatus, CONNECT, 3);

		nFdConnect = AcceptCallSrv(calling_adress);
		if ( nFdConnect < 0 ) 
		{
			shutdown_resource(P_nIndiceCtx, ResourceId);
			trace_event("AcceptCall() failed.", FATAL);
			DeconnectOracle(P_nIndiceCtx);
			FreeOracleCtx(P_nIndiceCtx);
			StopCltCom(nFdConnect);
			exit(0);
		}

		nResult = CheckAndRefreshDBCon(P_nIndiceCtx,lastTime);
	}


	if (UpdateResource(P_nIndiceCtx, ResourceId, LIVE, SIGNON) != OK) 
	{
		trace_event("UpdateResource() failed.", FATAL);
		shutdown_resource(P_nIndiceCtx, ResourceId);
		DeconnectOracle(P_nIndiceCtx);
		FreeOracleCtx(P_nIndiceCtx);
		StopCltCom(nFdLocal);
		exit(0);
	}

	sprintf(app_pid, "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());

	LogEvent(P_nIndiceCtx, ResourceId, "", INTERFACE_SIGN_ON, "##", "##", "##", app_pid, app_ppid);

	memcpy(ResourceStatus, SIGNON /**CONNECT**/ , 3);
	


	DeconnectOracle(P_nIndiceCtx);
	FreeOracleCtx(P_nIndiceCtx);

	sprintf(szDetail, "End   ConnectInterface(%d)", P_nIndiceCtx);
	trace_event(szDetail, PROCESSING);
	return (OK);
}

/*************************************************************************************/
/*******************************   Reconnect Interface  ******************************/
/*************************************************************************************/
int ReconnectInterface(int      P_nIndiceCtx)
{
	TSResources     sResource;
	

	char            app_pid[9];
	char            app_ppid[9];

	char            szDetail[256];
	char            calling_adress[17];

	int             nRetCode;
	int				nResult;

	struct timeval	lastTime;

	/*YK11042007 not stop the interface, case 16 and 17 sms certification, to let the reversal generation*/
	sprintf(szDetail, "Start ReconnectInterface(%d)", P_nIndiceCtx);
	trace_event(szDetail, PROCESSING);

	memset((char *)&sResource, 0, sizeof(sResource));
	memset(calling_adress, 0, sizeof(calling_adress));

	sprintf(app_pid,  "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());

	/********* Deallocate the previous connection *********/
	StopCltCom(nFdConnect);

	/************** DB Connection ****************/
	AllocateOracleCtx(P_nIndiceCtx);
	ConnectOracle(P_nIndiceCtx);

	/** Update Resource Status Localy and on DB **/
	memcpy(sResource.resource_id, ResourceId, 6);
	if (GetResources(P_nIndiceCtx, &sResource) != OK) 
	{
		trace_event("GetResources() failed.", FATAL);
		shutdown_resource(P_nIndiceCtx, ResourceId);
		DeconnectOracle(P_nIndiceCtx);
		FreeOracleCtx(P_nIndiceCtx);
		StopCltCom(nFdLocal);
		exit(0);
	}


		
	/*AMERA: 20160314
        A network disconnect was received
	*/
	LogEvent(P_nIndiceCtx, ResourceId, "", INTERFACE_ABNORMAL_STOP, "##", "##", "##", app_pid, app_ppid);
	/*AMERA: 20160314*/

	if (UpdateResource(P_nIndiceCtx, ResourceId, CRITICAL, CONNECT) != OK) 
	{
		trace_event("UpdateResource() failed.", FATAL);
		shutdown_resource(P_nIndiceCtx, ResourceId);
		DeconnectOracle(P_nIndiceCtx);
		FreeOracleCtx(P_nIndiceCtx);
		StopCltCom(nFdLocal);
		exit(0);
	}
	
	/*AMER: 20160314
	LogEvent(P_nIndiceCtx, ResourceId, "", INTERFACE_SIGN_ON, "##", "##", "##", app_pid, app_ppid);
	*/	

	/* Initialising outgoing call */
	trace_event(" Wait 30s Before Retrying the InitCltComms() Call", PROCESSING);
	sleep(30);

	if (memcmp (g_pris_connect_mode, "S", 1) == 0)
	{
	while (InitCltComms() != OK) 
	{
		trace_event(" Problem while Calling InitCltComms(). Wait 30s Before Retry", PROCESSING);
		sleep(30);
	}

	if (initial_sign_on_to_nw(P_nIndiceCtx) != OK) 
	{
		trace_event("initial_sign_on_to_nw() failed.", FATAL);
		shutdown_resource(P_nIndiceCtx, ResourceId);
		DeconnectOracle(P_nIndiceCtx);
		FreeOracleCtx(P_nIndiceCtx);
		StopCltCom(nFdLocal);
		exit(0);
	}
	}

	else
	{
			/* Wait for incoming call */
		if (InitSrvComms(atoi(LocalAppId)) != OK) 
		{
			shutdown_resource(P_nIndiceCtx, ResourceId);
			trace_event("InitSrvComms() failed.", FATAL);
			DeconnectOracle(P_nIndiceCtx);
			FreeOracleCtx(P_nIndiceCtx);
			exit(0);
		}
		memcpy(ResourceStatus, CONNECT, 3);

		nFdConnect = AcceptCallSrv(calling_adress);
		if ( nFdConnect < 0 ) 
		{
			shutdown_resource(P_nIndiceCtx, ResourceId);
			trace_event("AcceptCall() failed.", FATAL);
			DeconnectOracle(P_nIndiceCtx);
			FreeOracleCtx(P_nIndiceCtx);
			StopCltCom(nFdConnect);
			exit(0);
		}

		nResult = CheckAndRefreshDBCon(P_nIndiceCtx,lastTime);

	}

	if (UpdateResource(P_nIndiceCtx, ResourceId, LIVE, SIGNON) != OK) 
	{
		trace_event("UpdateResource() failed.", FATAL);
		shutdown_resource(P_nIndiceCtx, ResourceId);
		DeconnectOracle(P_nIndiceCtx);
		FreeOracleCtx(P_nIndiceCtx);
		StopCltCom(nFdLocal);
		exit(0);
	}

	sprintf(app_pid, "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());

	LogEvent(P_nIndiceCtx, ResourceId, "", INTERFACE_SIGN_ON, "##", "##", "##", app_pid, app_ppid);

	memcpy(ResourceStatus, SIGNON /**CONNECT**/ , 3);



	DeconnectOracle(P_nIndiceCtx);
	FreeOracleCtx(P_nIndiceCtx);

	sprintf(szDetail, "End   ReconnectInterface(%d)", P_nIndiceCtx);
	trace_event(szDetail, PROCESSING);
	return (OK);
}

