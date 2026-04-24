
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <define.h>
#include <mq_com.h>


/* todo: remove sQueueName */
int OpenMQManager(	MQHCONN*	pHcon, 
	MQCD*		pClientConn, 
	MQCNO*		pConnectOpt,
	char*		sApplicationName, 
	char*		sApplicationId,
	char*		szQManager)
{   


	MQLONG   CompCode;  
	MQLONG   Reason;    
	MQCD	 kClientConn = {MQCD_CLIENT_CONN_DEFAULT};
	MQCNO    kConnect_options = {MQCNO_DEFAULT};
	MQCHAR   QMName[MQ_Q_MGR_NAME_LENGTH];
    char     sLine[MAX_LINE_TRC];
	
	memset(sLine,'\0',sizeof(sLine));
	
	sprintf(sLine, "Start OpenMQManager(%s,%s,%s)", sApplicationName,sApplicationId,szQManager);
	trace_event(sLine, PROCESSING);
	(*pClientConn) = kClientConn;
	(*pConnectOpt) = kConnect_options;

	strncpy(pClientConn->ConnectionName,sApplicationName,MQ_CONN_NAME_LENGTH);
	strncpy(pClientConn->ChannelName,sApplicationId,MQ_CHANNEL_NAME_LENGTH);
	strncpy(QMName,szQManager,MQ_Q_MGR_NAME_LENGTH);

	pConnectOpt->ClientConnPtr	= pClientConn;
	pConnectOpt->Version			= MQCNO_VERSION_2;
	pConnectOpt->Options			= MQCNO_HANDLE_SHARE_BLOCK;


	MQCONNX(QMName,
		pConnectOpt,
		pHcon,
		&CompCode,
		&Reason);

	if (CompCode == MQCC_FAILED)
	{
		sprintf(sLine, "END   OpenMQManager(NOK) <MQCONN ended with reason code [%ld] CompCode[%ld]>", Reason, CompCode);
		trace_event(sLine, ERROR);
		return(NOK);
	}

	trace_event("END   OpenMQManager(OK)", PROCESSING);
	return( OK );
}


/* ----------------------------------------------------------------------- */
int OpenIncQueue(MQHCONN* pHcon, MQHOBJ* pHobj, char *szQueue )
{
	MQOD od = {MQOD_DEFAULT};            /* Object Descriptor               */

	MQLONG options;                      /* MQOPEN options                  */
	MQLONG CompCode;                     /* completion code                 */
	MQLONG Reason;                       /* reason code for MQCONN          */

	char   sLine[256];  

	sprintf(sLine, "Start OpenIncQueue(%s)", szQueue);
	trace_event(sLine, PROCESSING);

	strncpy(od.ObjectName, szQueue, (size_t)MQ_Q_NAME_LENGTH);   

	/* Open the named message queue for input; exclusive or shared */
	/* use of the queue is controlled by the queue definition here */
	options =         /* open queue for input            */
		MQOO_FAIL_IF_QUIESCING		/* but not if MQM stopping         */
		+ MQOO_INPUT_SHARED;
	MQOPEN ((*pHcon),                        /* connection handle               */
		&od,                         /* object descriptor for inQueue   */
		options,                     /* open options                    */
		pHobj,                       /* object handle                   */
		&CompCode,                   /* completion code                 */
		&Reason);                    /* reason code                     */

	/* report reason, if any; stop if failed */
	if (Reason != MQRC_NONE)
	{
		sprintf(sLine, "END   OpenIncQueue(NOK) <MQOPEN IN ended with reason code [%ld] CompCode[%ld]>", Reason, CompCode);
		trace_event(sLine, ERROR);
		return(NOK);
	}

	if (CompCode == MQCC_FAILED)
	{
		sprintf(sLine, "END   OpenIncQueue(NOK) <unable to open queue for input");
		trace_event(sLine, ERROR);
		return(NOK);
	}

	trace_event("END   OpenIncQueue(OK)", PROCESSING);
	return ( OK );
}
/* ----------------------------------------------------------------------- */
int OpenOutgQueue(MQHCONN* pHcon, MQHOBJ* pHobj, char *szQueue )
{
	MQOD od = {MQOD_DEFAULT};            /* Object Descriptor               */

	MQLONG options;                      /* MQOPEN options                  */
	MQLONG CompCode;                     /* completion code                 */
	MQLONG Reason;                       /* reason code for MQCONN          */

	char   sLine[256];  

	sprintf(sLine, "Start OpenOutgQueue(%s)", szQueue); 
	trace_event(sLine, PROCESSING);

	strncpy(od.ObjectName, szQueue, (size_t)MQ_Q_NAME_LENGTH);

	/* Open the output message queue */
	options =  MQOO_OUTPUT               /* open queue for output           */
		+ MQOO_FAIL_IF_QUIESCING
		;
	MQOPEN ((*pHcon),                           
		&od,                         /* object descriptor for outQueue  */
		options,                      
		pHobj,                       /* object handle for outQueue      */
		&CompCode,                      
		&Reason);    

	if (Reason != MQRC_NONE)
	{
		sprintf(sLine, "END   OpenOutgQueue(NOK) <MQOPEN OUT ended with reason code [%ld] CompCode[%ld]>", Reason,CompCode);
		trace_event(sLine, ERROR);
		return(NOK);
	}

	if (CompCode == MQCC_FAILED)
	{
		sprintf(sLine, "END   OpenOutgQueue(NOK) <unable to open queue for output>");
		trace_event(sLine, ERROR);
		exit ((int)Reason);
	}

	trace_event("END   openOutQueue(OK)", PROCESSING);
	return (OK);
}

/* ----------------------------------------------------------------------- */
int CloseQueue(MQHCONN Hcon, MQHOBJ* pHobj)
{
	MQLONG options;
	MQLONG CompCode;
	MQLONG Reason;  

	char   sLine[MAX_LINE_TRC];

	trace_event("START CloseQueue()", PROCESSING);


	/*pthread_mutex_lock(&MqCloseMutex);*/
	/*   Close the target queue (if it was opened) */
	options = 0;                     /* no close options                 */
	MQCLOSE (Hcon,                   /* connection handle                */
		pHobj,                  /* object handle                    */
		options,
		&CompCode,              /* completion code                  */
		&Reason);               /* reason code                      */

	/*pthread_mutex_unlock(&MqCloseMutex);*/
	if (Reason != MQRC_NONE)
	{
		sprintf(sLine, "END   CloseQueue(NOK) <MQCLOSE ended with reason code [%ld] CompCode[%ld]>", Reason,CompCode);
		trace_event(sLine, FATAL);
		return(NOK);
	}

	trace_event("END   CloseQueue(OK)", PROCESSING);
	return(OK);
}



/* ----------------------------------------------------------------------- */
int CloseMQManager(MQHCONN* pHcon)
{
	MQLONG CompCode;
	MQLONG Reason;  

	char   sLine[MAX_LINE_TRC];

	trace_event("START CloseMQManager()", PROCESSING);

	/*pthread_mutex_lock(&MqDiscMutex);*/
	MQDISC( pHcon,                   /* pointer on connection handle */
		&CompCode,               /* completion code              */
		&Reason);                /* reason code                  */

	/*pthread_mutex_unlock(&MqDiscMutex);*/

	if (Reason != MQRC_NONE)
	{
		sprintf(sLine, "END   CloseMQManager(NOK) <MQCLOSE ended with reason code [%ld] CompCode[%ld]>", Reason,CompCode);
		trace_event(sLine, PROCESSING);
		return(NOK);
	}

	trace_event("END   CloseMQManager(OK)", PROCESSING);
	return(OK);   
}


/* ----------------------------------------------------------------------- */
int ReadMQMsg(MQHCONN Hcon, MQHOBJ Hobj, char *buffer, int nBufLen, int *MsgLen,TSQMsgOpt* pkQMsgOpt, int pnTimer)
{
	MQMD      md = {MQMD_DEFAULT};          /* Message Descriptor              */
	MQGMO     gmo = {MQGMO_DEFAULT};         /* get message options             */
	MQLONG    CompCode;                      /* completion code                 */
	MQLONG    Reason;                        /* reason code for MQCONN          */
	char      sLine       [MAX_LINE_TRC];
	int       nRet = -1;


	memset(md.CorrelId, 0, sizeof(md.CorrelId));
	memset(sLine, '\0', sizeof(sLine));
	memset((char*) pkQMsgOpt,0,sizeof(TSQMsgOpt));

	trace_event("Start ReadMQMsg()", PROCESSING);



	/* Use these options when connecting to Queue Managers that also support */
	/* them, see the Application Programming Reference for details.          */
	/* These options cause the MsgId and CorrelId to be replaced, so that    */
	/* there is no need to reset them before each MQGET                      */ 

	gmo.Options =  MQGMO_WAIT            /* wait for new messages            */
		+ MQGMO_CONVERT;        /* convert if necessary             */

	if (pnTimer) /** NOT ZERO **/
		gmo.WaitInterval = pnTimer * 1000;             /* pnTimer is in Seconds, whereas WaitInterval is in Milli-Seconds **/
	else /** ZERO **/
		gmo.WaitInterval = MQWI_UNLIMITED; /** UNLIMITED WAITING **/

	/* Get messages from the message queue / Loop until there is a failure   */
	CompCode = 0;
	while  (CompCode != MQCC_FAILED ) 
	{
		/* The following two statements are not required if the MQGMO version */
		/* is set to MQGMO_VERSION_2 and gmo.MatchOptions is set to MQGMO_NONE*/

		/* In order to read the messages in sequence, MsgId and CorrelID      */
		/* must have the default value. MQGET sets them to the values in      */
		/* for message it returns, so re-initialise them before every call    */
		memcpy(md.MsgId, MQMI_NONE, sizeof(md.MsgId));
		memcpy(md.CorrelId, MQCI_NONE, sizeof(md.CorrelId));

		/* MQGET sets Encoding and CodedCharSetId to the values in the        */
		/* message returned, so these fields should be reset to the           */
		/* default values before every call, as MQGMO_CONVERT is specified    */
		md.Encoding       = MQENC_NATIVE;
		md.CodedCharSetId = MQCCSI_Q_MGR;

		MQGET ( Hcon,                /* connection handle    */
			    Hobj,                /* object handle        */
			    &md,                 /* message descriptor   */
			    &gmo,                /* get message options  */
			    nBufLen,             /* buffer length        */
			    buffer,              /* message buffer       */
			    MsgLen,			     /* &message length      */
			    &CompCode,           /* completion code      */
			    &Reason);            /* reason code          */



		if (Reason == MQRC_NO_MSG_AVAILABLE)
		{
			sprintf(sLine, "END   ReadMQMsg(NOK) <TIMEOUT: NO AVAILABLE MESSAGE ON QUEUE AFTER %d S>", pnTimer);
			trace_event(sLine, ERROR);
			return (NOK);
		}

		if (CompCode == MQCC_WARNING )
		{
			trace_event("Message received(WARNING)", PROCESSING);
			dump_buffer( md.CorrelId, MQ_CORREL_ID_LENGTH, 'A','X','X');
			sprintf(sLine, "END   ReadMQMsg(OK) <MQCC_WARNING. Reasoncode=[%ld]>", Reason);
			trace_event(sLine, ERROR);
			return (OK);
		}

		if (CompCode != MQCC_OK )
		{
			sprintf(sLine, "END   ReadMQMsg(NOK) <MQGET ended with CompCode=[%d] and Reasoncode=[%ld]>", CompCode, Reason);
			trace_event(sLine, ERROR);
			return(NOK);
		}

		trace_event("Message received", PROCESSING);
		
		

		HexToAscii(md.MsgId,pkQMsgOpt->sMsgId,MQ_CORREL_ID_LENGTH*2);
		HexToAscii(md.CorrelId,pkQMsgOpt->sCorrId,MQ_CORREL_ID_LENGTH*2);
		pkQMsgOpt->sMsgId[MQ_CORREL_ID_LENGTH*2]='\0';
		pkQMsgOpt->sCorrId[MQ_CORREL_ID_LENGTH*2]='\0';
		
		
		
		sprintf(sLine,"--> MSG ID %s",pkQMsgOpt->sMsgId);
		trace_event(sLine, PROCESSING);
		sprintf(sLine,"--> CORR ID %s",pkQMsgOpt->sCorrId);
		trace_event(sLine, PROCESSING);
		
		if( debug_option == 1 )
		{
			dump_buffer(buffer, *MsgLen,'A','L','I');
		}

		trace_event("END   ReadMQMsg(OK)", PROCESSING);
		return (OK);
	} 

}


/* ----------------------------------------------------------------------- */
int WriteMQMsg (MQHCONN Hcon, MQHOBJ Hobj , char *buffer, int nBufLen,TSQMsgOpt* pkQMsgOpt)
{

	MQMD    md = {MQMD_DEFAULT}; 
	MQPMO   pmo = {MQPMO_DEFAULT};
	MQLONG  CompCode;             
	MQLONG  Reason;               

	MQBYTE24	psCorreId; 

	char     sLine       [MAX_LINE_TRC];
	int      nRet = -1;

	memset(sLine      , '\0', sizeof(sLine));
	
	memcpy (md.MsgId,MQMI_NONE, sizeof(md.MsgId) );
	memcpy (md.CorrelId,MQCI_NONE, sizeof(md.CorrelId) );



	trace_event("Start WriteMQMsg()", PROCESSING);

	if( strlen(pkQMsgOpt->sMsgId) > 0 )
	{
		AsciiToHex(pkQMsgOpt->sMsgId,md.MsgId,MQ_CORREL_ID_LENGTH*2);
	}
	if( strlen(pkQMsgOpt->sCorrId) > 0 )
	{
		AsciiToHex(pkQMsgOpt->sCorrId,md.CorrelId,MQ_CORREL_ID_LENGTH*2);
	}

	memcpy (md.Format,  MQFMT_STRING, (size_t)MQ_FORMAT_LENGTH);

	MQPUT(   Hcon,           /* connection handle         */
		Hobj,                /* object handle             */
		&md,                 /* message descriptor        */
		&pmo,                /* default options (datagram)*/
		nBufLen,             /* message length            */
		buffer,               /* message buffer            */
		&CompCode,           /* completion code           */
		&Reason);            /* reason code               */

	if (Reason != MQRC_NONE)
	{
		sprintf(sLine, "END   WriteMQMsg() <MQPUT ended with reason code [%ld] CompCode[%ld]>", Reason, CompCode);
		trace_event(sLine, ERROR);
		return(NOK);
	}

	HexToAscii(md.MsgId,pkQMsgOpt->sMsgId,MQ_CORREL_ID_LENGTH*2);
	HexToAscii(md.CorrelId,pkQMsgOpt->sCorrId,MQ_CORREL_ID_LENGTH*2);
	
	pkQMsgOpt->sMsgId[MQ_CORREL_ID_LENGTH*2]='\0';
	pkQMsgOpt->sCorrId[MQ_CORREL_ID_LENGTH*2]='\0';

	sprintf(sLine,"--> MSG ID %s",pkQMsgOpt->sMsgId);
	trace_event(sLine, PROCESSING);
	sprintf(sLine,"--> CORR ID %s",pkQMsgOpt->sCorrId);
	trace_event(sLine, PROCESSING);

	dump_buffer( buffer, nBufLen, 'A','L','O');

	trace_event("END   WriteMQMsg(OK)", PROCESSING);
	return( OK );
}

