#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <dump.h>
#include <iso_visa.h>
#include <iso_hps.h>
#include <define.h>
#include <struct_line.h>
#include <param_arg.h>
#include <thread_var.h>
#include <queue.h>
#include <event_log.h>
#include <resources.h>
#include <tcp_var.h>

/*********************************************************************/
/*********************************************************************/
/**********************************************************************/
int SendShutDownSignalsToLineThreads()
{
	TSVisaInfo  VisaInfo;
	char        FieldData[256];
	char        sBuffer[1024];
	char        sentRefNbr[13];
	int         i,nLength;
	TSLineExchange   sLineMsg;

	InitVisaInfo ( &VisaInfo );
	VisaInfo.nMsgType = 1700;

	current_gmt_date (FieldData);
	AddVisaField (VISA_XMIT_TIME, &VisaInfo, FieldData + 2 , 10);

	current_julian_date_qjjj (sentRefNbr);
	current_hour (sentRefNbr + 4);
	memcpy( sentRefNbr + 6 , FieldData , 6 );
	AddVisaField (VISA_REFERENCE_NBR, &VisaInfo, sentRefNbr, 12);

	AddVisaField ( VISA_NW_MNGMT_INFO, &VisaInfo, "002", 3);

	/* send message */
	nLength = VisaBuildMsg(sBuffer + VISA_HDR_LEN, &VisaInfo);
	VisaBuildHeader(REQUEST , &(VisaInfo.sHeader), NULL, nLength, "000000");
	memcpy(sBuffer, (char *) &(VisaInfo.sHeader), VISA_HDR_LEN);
	nLength += VISA_HDR_LEN;

	sLineMsg.nLength = nLength;
	memcpy ( sLineMsg.sBuffer , sBuffer , nLength );

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
int shutdown_resource ( int nIndexCtx , char *ResourceId  )
{

	char           app_pid  [9];
	char           app_ppid [9];
	int            result;

	trace_event("Shutdown Resource ...In Progress", FATAL);

	sprintf(app_pid,  "%08d", getpid());
	sprintf(app_ppid, "%08d", getppid());

	memcpy (ResourceStatus, STOPI, 3); /*23062020 PLUTONL-1956*/

	if ( UpdateResource ( nIndexCtx , ResourceId, NOTLIVE, OFF ) != OK )
		trace_event ( "Cannot update resource status at ShutDown ", PROCESSING );

	sign_off( nIndexCtx );

	result = retrieval_advice_off( nIndexCtx );

	trace_event("Stopping Line Threads           ", FATAL);
	SendShutDownSignalsToLineThreads();

	trace_event("Stopping Bal Threads           ", FATAL);
	SendShutDownSignalsToBalThreads();

	trace_event("Stopping Time Threads           ", FATAL);
	SendShutDownSignalsToTimeThreads();

	LogEvent (nIndexCtx, ResourceId, "##", INTERFACE_SIGN_OFF, "##",
		"##", "##", app_pid, app_ppid);
	sleep(1);

	EmptyQueue ( nBalMsg, getpid());
	EmptyQueue ( nBalAdm, getpid());

	trace_event("Stopping Communication          ", FATAL);
	StopCltCom(nFdConnect);

	CloseTransactionsContainers();
	CloseMsgFlowInfoQueue();

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

