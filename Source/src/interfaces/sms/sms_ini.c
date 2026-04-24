#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread_var.h>
#include <queue.h>
#include <sms_var.h>
#include <resources.h>
#include <signal.h>
#include <define.h>
#include <sys/types.h>
#include <pthread.h>
#include <sms_param.h>
#include <errno.h>
#include <event_log.h>
#include <main_thr.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <sms_define.h>

#include <msg_flow_mapping.h>

/*Start PLUTONL-1844 Versioning */
#ifndef GIT_TAG 

#define GIT_TAG				"Undefined"
#define GIT_BRANCH			"Undefined"
#define GIT_COMMIT_HASH		"Undefined"

#endif
/*End PLUTONL-1844 Versioning */

int  ThreadMaskSignals()
{

#if defined (SS_64BIT_SERVER) || defined (SOLARIS) || defined (LINUX)
	sigset_t  set;
#else
	struct sigset_t  set;
#endif


	if (sigemptyset(&set) < 0) {
		fprintf(stderr, "Error when calling sigemptyset()\n");
		fprintf(stdout, "End   ThreadMaskSignal(FATAL)\n");
		return (ERROR);
	}

	if (sigaddset(&set, SIGPIPE) < 0) {
		fprintf(stderr, "Error when calling sigaddset(SIGPIPE)\n");
		fprintf(stdout, "End   ThreadMaskSignal(FATAL)\n");
		return (ERROR);
	}

	if (sigaddset(&set, SIGCHLD) < 0) {
		fprintf(stderr, "Error when calling sigaddset(SIGCHLD)\n");
		fprintf(stdout, "End   ThreadMaskSignal(FATAL)\n");
		return (ERROR);
	}

	if (sigaddset(&set, SIGUSR2) < 0) {
		fprintf(stderr, "Error when calling sigaddset(SIGUSR2)\n");
		fprintf(stdout, "End   ThreadMaskSignal(FATAL)\n");
		return (ERROR);
	}


#ifdef SOLARIS
	if (thr_sigsetmask (SIG_BLOCK, &set, NULL ) < 0) {
		fprintf(stderr, "Error when calling thr_sigsetmask(SIG_BLOCK)\n");
		fprintf(stdout, "End   ThreadMaskSignal(FATAL)\n");
		return (ERROR);
	}
#elif defined(LINUX) || defined(HPUX)
	if (pthread_sigmask(SIG_BLOCK, &set, NULL) < 0) {
		fprintf(stderr, "Error when calling pthread_sigmask(SIG_BLOCK)\n");
		fprintf(stdout, "End   ThreadMaskSignal(FATAL)\n");
		return (ERROR);
	}
#else
	if (sigthreadmask(SIG_BLOCK, &set, NULL) < 0) {
		fprintf(stderr, "Error when calling sigthreadmask(SIG_BLOCK)\n");
		fprintf(stdout, "End   ThreadMaskSignal(FATAL)\n");
		return (ERROR);
	}
#endif

	fprintf(stdout, "End   ThreadMaskSignal(OK)\n");
	return (OK);

}

void sig_segv(int X)
{
	char	szDetail[256];
	sprintf(szDetail, "SEGV  : %lu,(%lu), X=%d\n", pthread_self(), p7_thread_id(), X);
	fprintf(stderr, szDetail);
	trace_event(szDetail, ERROR);
	dump_backtrace();

	/*EBE140724: If ever its the ADM thread that crashed, we have no choice but exit badly because
	* it will never have the chance to read stop interface from bal. it'll keep trying to reexcute the instruction
	* that made it crash so it will never exit and it will keep filling the adm bal*/
	if( pthread_self() == tab_ThrInfo[ADM_THREAD_ID].tid )
	{
		trace_event("ADM thread Crash: Force exit", FATAL);
		exit (-1);
	}

	pthread_kill(tab_ThrInfo[SIG_THREAD_ID].tid, SIGUSR2);
	sleep(10);
}

void sig_alarm(int X)
{
	fprintf(stderr, "TIMEOUT  : %lu\n", pthread_self());

}

void  GetGlobals (TSResources  *sResource)
{
	/* dump variables */

	InitDumpParam();
	memcpy (trace_file, sResource->pris_log_file_name, 20);
	max_size = sResource->pris_log_max_size;
	trace_level = atoi (sResource->pris_trace_level);

	strncpy(NodeId, sResource->node_id, NODE_ID_LEN);
	strncpy(CommsId, sResource->pris_comms_id, RES_COMMS_ID_LEN);
	strncpy(ProtocolId, sResource->pris_protocol_id, RES_PROTO_ID_LEN);
	strncpy(DeviceName, sResource->pris_device_name, RES_DEV_NAME_LEN);
	strncpy(RemoteAppName, sResource->pris_remote_application_name, RES_REMOTE_APP_NAME_LEN);
	strncpy(RemoteAppId, sResource->pris_remote_application_id, RES_REMOTE_APP_ID_LEN);
	strncpy(LocalAppName, sResource->pris_local_application_name, RES_LOCAL_APP_NAME_LEN);
	strncpy(LocalAppId, sResource->pris_local_application_id, RES_LOCAL_APP_ID_LEN);

	/* purge time variables */
	nTimerAcq    = sResource->pris_timer_acquirer;

	nIssAcqDelta = sResource->pris_delta_timer_iss_acq;

	/* troubelshoot indicators */
	MinTroublesInd = sResource->pris_min_troubles_ind;

	MaxTroublesInd = sResource->pris_max_troubles_ind;

	max_thread  = sResource->pris_nb_static_threads;
	nUserThread = sResource->pris_nb_user_threads_vs_kernel;
	max_request = max_thread * 20;

	memset ( ReversalGeneration , 0, 2);
	memcpy ( ReversalGeneration , sResource->pris_gen_reversal_timeout_late, 1);
	memcpy(AcquirerBank, sResource->pris_bank_code, ACQUIRER_BANK_LEN);
	/* Echo Test Timer */
	EchoTestTimer = sResource->pris_echo_test_timer;
}

/*************************************************************/
int InitModule(char  *ResourceId)
{
	TSResources    sResource;
	char           app_pid  [9];
	char           app_ppid [9];
	char           sLine [ MAX_LINE_TRC ];
	int            result;
	int            flag_alarm=NOK;
	char				szHostName[512];
	char			szHostUser[64 + 1];
	char*			sPtr;
	char			sAuthReplyBackPeriod[20 + 1]; /*ZKO10032022 PLUTONL-4411*/

	memset(szHostName, '\0', sizeof(szHostName));
	memset(szHostUser,'\0',sizeof(szHostUser));
	memset(sAuthReplyBackPeriod,'\0',sizeof(sAuthReplyBackPeriod)); /*ZKO10032022 PLUTONL-4411*/

	if (( nBalMsg = OpenBalCom(BAL_MSG)) < 0)
	{
		fprintf(stderr, "OpenBalCom(BAL_MSG) failed. \n");
		exit(0);
	}

	EmptyQueue(nBalMsg, getpid());

	if (( nBalAdm = OpenBalCom(BAL_ADM) ) < 0)
	{
		fprintf(stderr, "OpenBalCom(BAL_ADM) failed. \n");
		exit(0);
	}
	EmptyQueue(nBalAdm, getpid());

	/*SKARROUMI PLUTONL-4900*/
	/* - BridgeR Queue Opening -------------------------------------- */
	if (( nBalBrr = OpenBalCom(BAL_BRIDGE_R) ) < 0)
	{
		fprintf(stderr, "OpenBalCom(BAL_BRIDGE_R) failed. \n");
		exit(0);
	}
	/*SKARROUMI PLUTONL-4900*/

	/**** Creation du Thread  D'Administartion d'interface  ***/
	if (pthread_create(&(tab_ThrInfo[ADM_THREAD_ID].tid), NULL, 
		thread_ADMIN, NULL) < 0)
	{
		fprintf(stderr,"pthread_create Error %d\n", errno);
		exit(0);
	}

	/**** Creation du Thread de Gestion des signaux         ***/
	if (pthread_create(&(tab_ThrInfo[SIG_THREAD_ID].tid), NULL,
		thread_SIG, NULL) < 0)
	{
		fprintf(stderr,"pthread_create Error %d\n", errno);
		exit(0);
	}


	/*** Ajouter les fonctions d'initialisation Necessaires ***/  
	AllocateOracleCtx(MAIN_THREAD_ID);

	if ( ConnectOracle (MAIN_THREAD_ID) != OK )
	{
		fprintf (stderr,"ConnectOracle() failed.\n");
		exit (0);
	}

	/*EBE140724: a resource should only run on its configured node */
	result = gethostname(szHostName, sizeof(szHostName)-1);
	sPtr = getenv("USER");
	if( sPtr )
	{
		strncpy(szHostUser,sPtr,sizeof(szHostUser) - 1);
	}
	result = CheckResourceNode(MAIN_THREAD_ID, ResourceId, szHostName,szHostUser);
	if (result != OK)
	{
		fprintf(stderr, "Resource [%s] Not allowed for Host [%s,%s]\n", ResourceId, szHostName,szHostUser);
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit(0);
	}

	fprintf(stderr,"Resource = %.6s\n", ResourceId);

	memcpy (sResource.resource_id, ResourceId, 6);

	if ( GetResources ( MAIN_THREAD_ID, &sResource) != OK)
	{
		fprintf (stderr,"GetResources() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit (0);
	}

	/* Clean any old messages */
	EmptyQueue(nBalMsg, sResource.pris_pid);
	EmptyQueue(nBalAdm, sResource.pris_pid);

	fprintf(stderr,"Resource = %.6s\n", ResourceId);

	/* update resource activity table to indicate that resource was launched */
	if (ResourceActivity (MAIN_THREAD_ID, ResourceId, START) != OK)
	{
		fprintf (stderr,"Resources_Ativity table update failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit (0);
	}

	/* fill-in all global variables from sResource */
	GetGlobals (&sResource);
	/*DumpSmsGlobals();*/
	/* Catching the segmentation Violation Signal */

	/*Start ZKO10032022 PLUTONL-4411 */
	result = GetGlobalsVar(MAIN_THREAD_ID,PG_AUTH_REPLY_BACK_PERIOD,sAuthReplyBackPeriod);
	if (result != OK)
	{	
		g_nAuthReplyBackPeriod = 500; /*500ms*/
	}
	else 
	{
		g_nAuthReplyBackPeriod = atoi(sAuthReplyBackPeriod);
		if(g_nAuthReplyBackPeriod >= nIssAcqDelta)
		{
			fprintf(stderr, "AuthReplyBackPeriod should not be greater than nIssAcqDelta.\n");
			exit(0);
		}
	}	
	/*End ZKO10032022 PLUTONL-4411*/

	install_handler(SIGSEGV, sig_segv);
	install_handler(SIGQUIT, sig_segv);
	install_handler(SIGILL,  sig_segv);
	install_handler(SIGTRAP, sig_segv);
	install_handler(SIGIOT,  sig_segv);
#ifndef LINUX
	install_handler(SIGEMT,  sig_segv);
#endif
	install_handler(SIGFPE,  sig_segv);
	install_handler(SIGBUS,  sig_segv);
	install_handler(SIGSYS,  sig_segv);
	install_handler(SIGALRM, sig_alarm);

	/* initialize trace */
	if ( init_trace() != OK)
	{
		fprintf (stderr,"init_trace() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit (0);
	}

	/*InitSmsProtocol();
	SetNetworkProtocolId(ISO8583_VISA_SMS);*/

	trace_event ( "***************************************", PROCESSING );
	trace_event ( "**         New Session Started       **", PROCESSING );
	trace_event ( "***************************************", PROCESSING );

	sprintf(sLine, "pid[%08u], ppid[%08u]", getpid(), getppid()); trace_event(sLine, PROCESSING);
	/*Start PLUTONL-1844 Versioning */
	sprintf(sLine, ">>>>> MODULE BUILT [%s][%s]", __DATE__, __TIME__); trace_event(sLine, FATAL);
	sprintf(sLine, ">>>>> VERSION TAG:[%s] BRANCH:[%s] HASH[%s]", GIT_TAG,GIT_BRANCH,GIT_COMMIT_HASH); trace_event(sLine, FATAL);
	/*End PLUTONL-1844 Versioning */
	if (GetSmsParams (MAIN_THREAD_ID, ResourceId) != OK)
	{
		fprintf (stderr,"GetSmsParams() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit (0);
	}

	 
	DumpSmsGlobals();
    /*Start RAWMSG IBO20200513*/
	/*result = InitMessagesCaptureModule(MAIN_THREAD_ID);*/
	/*End RAWMSG IBO20200513*/

    EmptyQueue(nBalHsmVerf, sResource.pris_pid);
	EmptyQueue(nBalHsmProd, sResource.pris_pid);

	/* reset resource & get its internal id */
	if (ResetResource (MAIN_THREAD_ID, ResourceId, ResourceInternalId) != OK)
	{
		fprintf (stderr,"ResetResource() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit (0);
	}

    if (UpdateResource(MAIN_THREAD_ID, ResourceId, CRITICAL, SIGNOFF) != OK) 
	{
		fprintf(stderr, "UpdateResource() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		StopCltCom();
		exit (0);
	}
	if (GetLocDBServId(MAIN_THREAD_ID, NodeId, g_LocDBServ) != OK) 
	{
		fprintf(stderr, "GetLocDBServId() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
        StopCltCom();
		exit(0);
	}

	memcpy (ResourceStatus, OFF, 3);
	memcpy (sResource.pris_processing_step, OFF, 3);
	memcpy (sResource.resource_live, CRITICAL, 1);
    memcpy (g_pris_connect_mode, sResource.pris_connect_mode, 1);

	sprintf(app_pid,  "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());
	LogEvent (MAIN_THREAD_ID, ResourceId, "", INTERFACE_START, "",
		"", "", app_pid, app_ppid);
	/* up to this point ResourceStatus = OFF */


	InitList();
	InitTabAlarm();
	InitBalProdConso();
	InitLineProdConso();
	InitTimeProdConso();
 
	InitTabHsm(MAIN_THREAD_ID, sResource.node_id, HSM_ISS_VERIF);
	InitDBConThreads();


	OpenMsgFlowInfoQueue( MAIN_THREAD_ID );


	result = InitIntConfig(MAIN_THREAD_ID, "INT_SMS");

	DeconnectOracle(MAIN_THREAD_ID);
	FreeOracleCtx(MAIN_THREAD_ID);

	return (OK);
}
