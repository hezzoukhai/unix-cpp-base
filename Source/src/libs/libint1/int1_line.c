#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <define.h>
#include <errno.h>
#include <struct_line.h>
#include <param_arg.h>
#include <thread_var.h>
#include <resources.h>
#include <event_log.h>
#include <int1_struct.h>
#include <int1.h>

conn_id_t	nSrvConnId;
/***********************************************************************/
static int Int1ConnectInterface(int P_nIndiceCtx);
static int Int1ReconnectInterface(int P_nIndiceCtx);
/***********************************************************************/
int UserInt1LineProcMsg(int nIndexCtx, TSLineExchange* pkLineExchange);
/***********************************************************************/

static void* int1_thread_proc_LINE(void *arg)
{
	TSLineExchange  sLineMsg;
	int             nIndiceCtx;
	char            sLine[MAX_LINE_TRC];
	struct timeval	lastTime;
	int		nResult;

	pthread_detach(pthread_self());
	nIndiceCtx = ((TSArgParam *) arg)->nIndice;
	free(arg);

	sprintf(sLine, "THREAD LINE : %d", nIndiceCtx);
	trace_event(sLine, TRACE);

	ConnectOracle(nIndiceCtx);


	ThreadMaskSignals();


	gettimeofday(&lastTime,NULL);
	while (1) {

		CLineWaitMsg();
		CLineGetMsg(&sLineMsg);
		CLineFreeEntry();

		trace_event("Message Received (C)", PROCESSING);
		nResult = CheckAndRefreshDBCon(nIndiceCtx,lastTime);
		nResult = UserInt1LineProcMsg(nIndiceCtx, &sLineMsg);
		gettimeofday(&lastTime,NULL);
	}
}



void* int1_thread_wait_LINE(void *arg)
{
	TSLineExchange  sLineMsg;
	int             i;
	pthread_attr_t  pattr;
	TSArgParam     *p_arg;
	int             CodeRet;

	pthread_detach(pthread_self());

	ThreadMaskSignals();

	CodeRet = Int1ConnectInterface(MAIN_THREAD_ID);
	if (OK != CodeRet) 
	{
		trace_event("ConnectInterface(MAIN_THREAD_ID) failed.", FATAL);
		exit(0);
	}
	
	

	for (i = 0; i < max_thread; i++) 
	{
		pthread_attr_init(&pattr);
		if (i < nUserThread)
			pthread_attr_setscope(&pattr, PTHREAD_SCOPE_PROCESS);
		else
			pthread_attr_setscope(&pattr, PTHREAD_SCOPE_SYSTEM);
		p_arg = (TSArgParam *) malloc(sizeof(TSArgParam));
		AllocateOracleCtx(i + MAX_BAL_THREAD);
		p_arg->nIndice = i + MAX_BAL_THREAD;
		if (pthread_create(&(tab_ThrInfo[i + MAX_BAL_THREAD].tid), &pattr, int1_thread_proc_LINE, p_arg) < 0) {
			fprintf(stderr, "pthread_create Error %d\n", errno);
			free(p_arg);
			pthread_exit(NULL);
		}
	}

	while (1) 
	{
		CodeRet = ReadLineMsg(nConnId,sLineMsg.sBuffer, &sLineMsg.nLength);
		if (CodeRet!= OK) 
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
			CodeRet = Int1ReconnectInterface(MAIN_THREAD_ID);
			if (OK != CodeRet) 
			{
				trace_event("ReconnectInterface(MAIN_THREAD_ID) failed.", FATAL);
				exit(0);
			}
			continue;
		}

		trace_event("Message Received (P)", PROCESSING);

		if (debug_option == 1)
			dump_buffer(sLineMsg.sBuffer, sLineMsg.nLength, 'A', 'L', 'I');

		
		PLineWaitEntry();
		PLinePutMsg(&sLineMsg);
		PLineConsoInfo();
	}
}


static int ConnectInterfaceCom(int P_nIndiceCtx)
{
	

	char            app_pid[9];
	char            app_ppid[9];

	char            szDetail[256];
	int             nRetCode;

	sprintf(szDetail, "Start ConnectInterfaceCom(%d)", P_nIndiceCtx);
	trace_event(szDetail, PROCESSING);


	sprintf(app_pid, "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());

	if (UpdateResource(P_nIndiceCtx, ResourceId, CRITICAL, CONNECT) != OK) 
	{
		trace_event("UpdateResource() failed.", FATAL);
		int1_shutdown_resource(P_nIndiceCtx, ResourceId);
		DeconnectOracle(P_nIndiceCtx);
		FreeOracleCtx(P_nIndiceCtx);
		StopCltCom(nConnId);
		exit(0);
	}
	

	/* Initialising outgoing call */
	do 
	{
		if( ConnectMode[0] == 'M' )
		{
			nRetCode = ConnectLine(nSrvConnId,LocalAppName,atoi(LocalAppId));
		}
		else
		{
			nRetCode = ConnectLine(nConnId,RemoteAppName,atoi(RemoteAppId));
		}

		if( nRetCode == OK )
		{
			break;
		}
		trace_event(" Problem while Calling InitCltComms(). Wait 30s Before Retry", PROCESSING);
		sleep(30);
	} while( 1 );

	
	if( ConnectMode[0] == 'M' )
	{
		if ( AcceptCallSrv(nSrvConnId,&nConnId) != OK )
		{
			trace_event("AcceptCall Failed ...", FATAL);
			/*...*/
		}
	}
	else
	{
		if (UserInt1OnConnect(P_nIndiceCtx) != OK) 
		{
			trace_event("UserInt1OnConnect() failed.", FATAL);
			int1_shutdown_resource(P_nIndiceCtx, ResourceId);
			DeconnectOracle(P_nIndiceCtx);
			FreeOracleCtx(P_nIndiceCtx);
			StopCltCom(nConnId);
			exit(0);
		}
	}

	if (UpdateResource(P_nIndiceCtx, ResourceId, LIVE, SIGNON) != OK) 
	{
		trace_event("UpdateResource() failed.", FATAL);
		int1_shutdown_resource(P_nIndiceCtx, ResourceId);
		DeconnectOracle(P_nIndiceCtx);
		FreeOracleCtx(P_nIndiceCtx);
		StopCltCom(nConnId);
		exit(0);
	}


	sprintf(app_pid, "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());

	LogEvent(P_nIndiceCtx, ResourceId, "", INTERFACE_SIGN_ON, "##", "##", "##", app_pid, app_ppid);

	memcpy(ResourceStatus, SIGNON , 3);
	

	sprintf(szDetail, "End   ConnectInterfaceCom(%d)", P_nIndiceCtx);
	trace_event(szDetail, PROCESSING);
	return (OK);
}

/******************************************************************************/
static int Int1ConnectInterface(int P_nIndiceCtx)
{
	

	char            app_pid[9];
	char            app_ppid[9];

	char            szDetail[256];
	int             nRetCode;

	sprintf(szDetail, "Start Int1ConnectInterface(%d)", P_nIndiceCtx);
	trace_event(szDetail, PROCESSING);

	sleep(2);

	/* DB Connection */
	AllocateOracleCtx(P_nIndiceCtx);
	ConnectOracle(P_nIndiceCtx);

	ConnectInterfaceCom(P_nIndiceCtx);
	
	DeconnectOracle(P_nIndiceCtx);
	FreeOracleCtx(P_nIndiceCtx);

	sprintf(szDetail, "End   ConnectInterface(%d)", P_nIndiceCtx);
	trace_event(szDetail, PROCESSING);
	return (OK);
}


/******************************************************************************/
static int Int1ReconnectInterface(int P_nIndiceCtx)
{
	

	char            app_pid[9];
	char            app_ppid[9];

	char            szDetail[256];
	int             nRetCode;

	sprintf(szDetail, "Start ReconnectInterface(%d)", P_nIndiceCtx);
	trace_event(szDetail, PROCESSING);

	

	/* Deallocate the previous connection */
	StopCltCom(nConnId);
	StopSrvCom(nSrvConnId);

	/* DB Connection */
	AllocateOracleCtx(P_nIndiceCtx);
	ConnectOracle(P_nIndiceCtx);


	sprintf(app_pid, "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());
	
	LogEvent(P_nIndiceCtx, ResourceId, "", INTERFACE_SIGN_OFF, "##", "##", "##", app_pid, app_ppid);
	
	
	ConnectInterfaceCom(P_nIndiceCtx);
	
	DeconnectOracle(P_nIndiceCtx);
	FreeOracleCtx(P_nIndiceCtx);


	sprintf(szDetail, "End   ReconnectInterface(%d)", P_nIndiceCtx);
	trace_event(szDetail, PROCESSING);
	return (OK);
}
/****************************************************************************************/
