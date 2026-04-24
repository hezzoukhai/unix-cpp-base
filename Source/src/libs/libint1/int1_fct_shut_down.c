#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <dump.h>
#include <iso_hps.h>
#include <define.h>
#include <struct_line.h>
#include <param_arg.h>
#include <thread_var.h>
#include <queue.h>
#include <event_log.h>
#include <resources.h>
#include <iso8583.h>

#include <int1_struct.h>
#include <int1.h>

/*********************************************************************/
int UserInt1OnShutdownResource(int nIndexCtx , char *ResourceId  );
/*********************************************************************/
/*********************************************************************/
/**********************************************************************/
static int SendShutDownSignalsToLineThreads()
{
	MsgInfo		kMsgInfo;
	char        FieldData[256];
	char        sBuffer[1024];
	char        sentRefNbr[13];
	int         i,nLength;
	TSLineExchange   sLineMsg;

	InitNetworkMsgInfo(&kMsgInfo);
	kMsgInfo.nMsgType = 1700;

	current_gmt_date (FieldData);
	AddMsgField (ISO_XMIT_TIME, &kMsgInfo, FieldData + 2 , 10);

	current_julian_date_qjjj (sentRefNbr);
	current_hour (sentRefNbr + 4);
	memcpy( sentRefNbr + 6 , FieldData , 6 );
	AddMsgField (ISO_REFERENCE_NBR, &kMsgInfo, sentRefNbr, 12);

	

	sLineMsg.nLength = BuildMsgInfoBuffer(sLineMsg.sBuffer, &kMsgInfo);
	 

	for ( i = 0; i < max_thread ; i++ )
	{
		PLineWaitEntry();
		PLinePutMsg   (&sLineMsg);
		PLineConsoInfo();
	}

	return ( OK );
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
int int1_shutdown_resource ( int nIndexCtx , char *ResourceId  )
{
	TSBalExchange   BalMessage;
	int             i;
	char           app_pid  [9];
	char           app_ppid [9];
	int            result;

	trace_event("Shutdown Resource ...In Progress", FATAL);


		
	UserInt1OnShutdownResource(nIndexCtx,ResourceId);
	

	sprintf(app_pid,  "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());
	
	CloseTransactionsContainers();
	CloseMsgFlowInfoQueue();

	/*use status STOP istead of OFF*/ 
	memcpy (ResourceStatus, STOPI, 3);         /*23062020 PLUTONL-1956*/
	
	LogEvent (nIndexCtx, ResourceId, "##", INTERFACE_SIGN_OFF, "##",
		"##", "##", app_pid, app_ppid);
	sleep(1);

	EmptyQueue ( nBalMsg, getpid());
	EmptyQueue ( nBalAdm, getpid());



	trace_event("Stopping Line Threads           ", FATAL);
	SendShutDownSignalsToLineThreads();

	trace_event("Stopping Bal Threads           ", FATAL);
	SendShutDownSignalsToBalThreads();

	trace_event("Stopping Time Threads           ", FATAL);
	SendShutDownSignalsToTimeThreads();

	trace_event("Stopping Communication          ", FATAL);


	/*EBETODO: close connexion, client or server */
	StopCltCom(nConnId);

	if ( UpdateResource ( nIndexCtx , ResourceId, NOTLIVE, OFF ) != OK )
		trace_event ( "Cannot update resource status at ShutDown ", PROCESSING );

	if ( ResourceActivity ( nIndexCtx , ResourceId, STOP ) != OK )
		trace_event ( "Cannot update resource activity at ShutDown ", PROCESSING );

	LogEvent (nIndexCtx, ResourceId, "##", INTERFACE_STOP, "##",
		"##", "##", app_pid, app_ppid);

	trace_event("Deconnecting from Oracle        ", FATAL);
	DeconnectOracle(nIndexCtx);
	FreeOracleCtx(nIndexCtx);
	trace_event("About to Exit The Program       ", FATAL);



	sleep(3);

	exit (0);
	return(OK);
}

