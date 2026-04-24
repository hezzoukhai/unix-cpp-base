#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <define.h>
#include <visa_param.h>
#include <event_log.h>
#include <thread_var.h>
#include <main_thr.h>
#include <queue.h>
#include <resources.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <visa_define.h>
#include <iso_visa.h>   /*NAB07042020*/
#include <iso8583.h> 	           /*PLUTONL-1822*/ 

/*Start PLUTONL-1844 Versioning */
#ifndef GIT_TAG 

#define GIT_TAG				"Undefined"
#define GIT_BRANCH			"Undefined"
#define GIT_COMMIT_HASH		"Undefined"

#endif
/*End PLUTONL-1844 Versioning */

int ThreadMaskSignals()
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

#if defined(SOLARIS)
	if (thr_sigsetmask(SIG_BLOCK, &set, NULL) < 0)
#elif defined(SS_64BIT_SERVER)
	if (pthread_sigmask(SIG_BLOCK, &set, (sigset_t *)NULL) != 0)
#elif defined(LINUX)
	if (pthread_sigmask(SIG_BLOCK, &set, NULL) < 0)
#else
	if (sigthreadmask(SIG_BLOCK, &set, NULL) < 0)
#endif
	{
		fprintf(stderr, "Error when calling pthread_sigmask(SIG_BLOCK)\n");
		fprintf(stdout, "End   ThreadMaskSignal(FATAL)\n");
		return (ERROR);
	}

	fprintf(stdout, "End   ThreadMaskSignal(OK)\n");
	return(OK);
}


void sig_segv(int X)
{
	
	char	szDetail[256];
	sprintf(szDetail, "SEGV  : %lu,(%lu), X=%d\n", pthread_self(), p7_thread_id(), X);
	fprintf(stderr, szDetail);
	trace_event(szDetail, ERROR);
	dump_backtrace();

	/*EBE140724: If ever its the ADM thread  that crashed, we have no choice but exit badly because
	* it will never have the chance to read stop interface from bal. it'll keep trying to reexcute the instruction
	* that made it crash so it will never exit and it will keep filling the adm bal*/
	if (pthread_self() == tab_ThrInfo[ADM_THREAD_ID].tid)
	{
		trace_event("ADM thread Crash: Force exit", FATAL);
		exit(-1);
	}

	pthread_kill(tab_ThrInfo[SIG_THREAD_ID].tid, SIGUSR2);
	sleep(10);
}

void sig_alarm(int X)
{
	fprintf(stderr, "TIMEOUT  : [%lu]\n", pthread_self());
}

void GetGlobals(TSResources * sResource)
{
	/* dump variables */

	InitDumpParam();
	strncpy(trace_file, sResource->pris_log_file_name, 20);
	max_size = sResource->pris_log_max_size;
	trace_level = atoi(sResource->pris_trace_level);

	strncpy(NodeId, sResource->node_id, NODE_ID_LEN);
	strncpy(CommsId, sResource->pris_comms_id, RES_COMMS_ID_LEN);
	strncpy(ProtocolId, sResource->pris_protocol_id, RES_PROTO_ID_LEN);
	strncpy(DeviceName, sResource->pris_device_name, RES_DEV_NAME_LEN);
	strncpy(RemoteAppName, sResource->pris_remote_application_name, RES_REMOTE_APP_NAME_LEN);
	strncpy(RemoteAppId, sResource->pris_remote_application_id, RES_REMOTE_APP_ID_LEN);
	strncpy(LocalAppName, sResource->pris_local_application_name, RES_LOCAL_APP_NAME_LEN);
	strncpy(LocalAppId, sResource->pris_local_application_id, RES_LOCAL_APP_ID_LEN);

	/* purge time variables */
	nTimerAcq = sResource->pris_timer_acquirer;

	nIssAcqDelta = sResource->pris_delta_timer_iss_acq;

	/* troubelshoot indicators */
	MinTroublesInd = sResource->pris_min_troubles_ind;

	MaxTroublesInd = sResource->pris_max_troubles_ind;

	max_thread = sResource->pris_nb_static_threads;
	nUserThread = sResource->pris_nb_user_threads_vs_kernel;
	max_request = max_thread * 20;

	memcpy(ReversalGeneration, sResource->pris_gen_reversal_timeout_late, 1);
	strncpy(AcquirerBank, sResource->pris_bank_code, ACQUIRER_BANK_LEN);

	/*HAL26082020 PLUTONL-2185*/
	/* Echo Test Timer */
	EchoTestTimer = sResource->pris_echo_test_timer;
	/*HAL26082020 PLUTONL-2185*/
}


int InitModule(char *ResourceId)
{
	TSResources     sResource;
	char			app_pid[9];
	char			app_ppid[9];
	char			sLine[MAX_LINE_TRC];
	int				result;
	int				flag_alarm = NOK;
	int				nTimes;
	char			szHostName[512];
	char			szHostUser[64 + 1];
	char*			sPtr;
	int				nLength;
	char			eCom_tmp [1+1];/*NAB08042020 PLUTONL-1477*/
	char			sAuthReplyBackPeriod[20 + 1]; /*ZKO10032022 PLUTONL-4411*/

	memset(sAuthReplyBackPeriod,'\0',sizeof(sAuthReplyBackPeriod)); /*ZKO10032022 PLUTONL-4411*/
	memset(szHostName,'\0',sizeof(szHostName));
	memset(szHostUser,'\0',sizeof(szHostUser));
	memset(eCom_tmp,'\0',sizeof(eCom_tmp));/*NAB08042020*/

	if ((nBalMsg = OpenBalCom(BAL_MSG)) < 0) 
	{
		fprintf(stderr, "OpenBalCom(BAL_MSG) failed. \n");
		exit(0);
	}
	EmptyQueue(nBalMsg, getpid());

	if ((nBalAdm = OpenBalCom(BAL_ADM)) < 0) 
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
		thread_ADMIN, NULL) < 0) {
			fprintf(stderr, "pthread_create Error %d\n", errno);
			exit(0);
	}
	/**** Creation du Thread de Gestion des signaux         ***/
	if (pthread_create(&(tab_ThrInfo[SIG_THREAD_ID].tid), NULL,
		thread_SIG, NULL) < 0) {
			fprintf(stderr, "pthread_create Error %d\n", errno);
			exit(0);
	}
	/*** Ajouter les fonctions d'initialisation Necessaires ***/
	AllocateOracleCtx(MAIN_THREAD_ID);
	if (ConnectOracle(MAIN_THREAD_ID) != OK) {
		fprintf(stderr, "ConnectOracle() failed.\n");
		exit(0);
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


	fprintf(stderr, "Resource = %.6s\n", ResourceId);

	memcpy(sResource.resource_id, ResourceId, 6);


	if (GetResources(MAIN_THREAD_ID, &sResource) != OK) 
	{
		fprintf(stderr, "GetResources() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit(0);
	}
	/* Clean any old messages */
	EmptyQueue(nBalMsg, sResource.pris_pid);
	EmptyQueue(nBalAdm, sResource.pris_pid);

	/*
	* update resource activity table to indicate that resource was
	* launched
	*/
	if (ResourceActivity(MAIN_THREAD_ID, ResourceId, START) != OK) 
	{
		fprintf(stderr, "Resources_Ativity table update failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit(0);
	}

	if (GetVisaParams(MAIN_THREAD_ID, ResourceId) != OK) 
	{
		fprintf(stderr, "GetVisaParams() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit(0);
	}

	/*Start NAB07042020 */
	result = GetGlobalsVar(MAIN_THREAD_ID,PG_ECOM_CAVV,eCom_tmp);
		
	if (result != OK)
	{	
		BASE1_REJ_NON3DS = 'Y' ;
	}
	else
	{
		BASE1_REJ_NON3DS = eCom_tmp[0];
	}
	fprintf(stderr,"BASE1_REJ_NON3DS  value.......= %c\n",        BASE1_REJ_NON3DS);		
	
	/*End NAB07042020 PLUTONL-1477*/
	
	
	/* fill-in all global variables from sResource */
	GetGlobals(&sResource);
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
	DumpVisaGlobals();
	/* Catching the segmentation Violation Signal */



	install_handler(SIGSEGV, sig_segv);
	install_handler(SIGQUIT, sig_segv);
	install_handler(SIGILL, sig_segv);
	install_handler(SIGTRAP, sig_segv);
	install_handler(SIGIOT, sig_segv);
#ifndef LINUX
	install_handler(SIGEMT, sig_segv);
#endif
	install_handler(SIGFPE, sig_segv);
	install_handler(SIGBUS, sig_segv);
	install_handler(SIGSYS, sig_segv);
	install_handler(SIGALRM, sig_alarm);

	/* initialize trace */
	if (init_trace() != OK) 
	{
		fprintf(stderr, "init_trace() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit(0);
	}

	/*Start PLUTONL-1822*/
	InitBase1Protocol();

	SetNetworkProtocolId(ISO8583_VISA_BASE1);
	/*End PLUTONL-1822*/ 

	trace_event("***************************************", PROCESSING);
	trace_event("**         New Session Started       **", PROCESSING);
	trace_event("***************************************", PROCESSING);

	sprintf(sLine, "pid[%08u], ppid[%08u]", getpid(), getppid()); trace_event(sLine, PROCESSING);
	/*Start PLUTONL-1844 Versioning */
	sprintf(sLine, ">>>>> MODULE BUILT [%s][%s]", __DATE__, __TIME__); trace_event(sLine, FATAL);
	sprintf(sLine, ">>>>> VERSION TAG:[%s] BRANCH:[%s] HASH[%s]", GIT_TAG,GIT_BRANCH,GIT_COMMIT_HASH); trace_event(sLine, FATAL);
	/*End PLUTONL-1844 Versioning */

	/*Start RAWMSG IBO20200513*/
	result = InitMessagesCaptureModule(MAIN_THREAD_ID);
	/*End RAWMSG IBO20200513*/

	/************************************************************************/
	/* Inistialisation Threads HSM                                           */
	/************************************************************************/

	InitTabHsm(MAIN_THREAD_ID, sResource.node_id, HSM_ISS_VERIF);

	EmptyQueue(nBalHsmVerf, sResource.pris_pid);
	EmptyQueue(nBalHsmProd, sResource.pris_pid);

	/* reset resource & get its internal id */
	if (ResetResource(MAIN_THREAD_ID, ResourceId, ResourceInternalId) != OK) 
	{
		fprintf(stderr, "ResetResource() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		exit(0);
	}

	memcpy(ResourceStatus, OFF, 3);
	memcpy(sResource.pris_processing_step, OFF, 3);
	memcpy(sResource.resource_live, CRITICAL, 1);
	memcpy (g_pris_connect_mode, sResource.pris_connect_mode, 1);

	if (UpdateResource(MAIN_THREAD_ID, ResourceId, CRITICAL, SIGNOFF) != OK) 
	{
		fprintf(stderr, "UpdateResource() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		StopCltCom();
		exit(0);
	}

	if (GetLocDBServId(MAIN_THREAD_ID, NodeId, g_LocDBServ) != OK) 
	{
		fprintf(stderr, "GetLocDBServId() failed.\n");
		DeconnectOracle(MAIN_THREAD_ID);
		FreeOracleCtx(MAIN_THREAD_ID);
		StopCltCom();
		exit(0);
	}


	sprintf(app_pid, "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());
	LogEvent(MAIN_THREAD_ID, ResourceId, "", INTERFACE_START, "",
		"", "", app_pid, app_ppid);

	InitList();
	InitTabAlarm();
	InitBalProdConso();
	InitLineProdConso();
	InitTimeProdConso();
	InitDBConThreads();
	/*InitTabBridge(MAIN_THREAD_ID,NodeId);  */  /*IZL 15042022: PLUTONL-4596*/ 
	OpenMsgFlowInfoQueue(MAIN_THREAD_ID);


	result = InitIntConfig(MAIN_THREAD_ID, "INT_BASE1");

	DeconnectOracle(MAIN_THREAD_ID);
	FreeOracleCtx(MAIN_THREAD_ID);
	return(OK);
}


