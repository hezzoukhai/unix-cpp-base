/***************************************************************************/
/* */
/* FILE MAINTENANCE MESSAGES (0302 - 0312)                  */
/***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <define.h>
#include <p7_sid.h>
#include <visa_define.h>
#include <iso_hps.h>
#include <iso_visa.h>
#include <visa_param.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <queue.h>
#include <security_data.h>
#include <tlv_autho.h>
#include <nw_raw_msg.h>
#include <iso8583.h>



/****************************************************************************/
/* FCT_UPD_REQ_TO_NW()                                                    */
/* */
/* Sends an update file request message (0302) to VISA.                   */
/* Input:  ISO formatted message                                          */
/* Output: VISA formatted message to be sent                              */
/****************************************************************************/
int 
FCT_UPD_REQ_TO_NW(int nIndexCtx, 
					TSIsoInfo * IsoInfo,
					char *sTlvBuffer, /*PLUTONL-4445*/
					char *sSafInd)
{
	TSBalExchange   BalMessage;
	TSVisaInfo      VisaInfo;
	TSNodeData      Node;
	char            tmpField[256];
	int             tmpFieldLen;
	int             Length;
	char            sBuffer[1024];
	char            FctCode[4];
	char            sLine[MAX_LINE_TRC];
	/*time_t          lAcqPurgeTime;*/
	struct timeval	sAcqPurgeTimeVal;
	int             result;
	/*PLUTONL-4445: commented
	TSTlvAutho 		sTlvInfo;
	char    		sTlvBuffer  [LG_MAX];*/
	char 			sSecurityData [128 + 1];
	
	trace_event("Start FCT_UPD_REQ_TO_NW()", PROCESSING);
	

	MsgFwdReqRetrieved(IsoInfo->msgId);

	dump_p7_struct(IsoInfo);


	memset (sSecurityData, 0, sizeof(sSecurityData));

	/* Insert original request 1304 in queue for later processing */
	
	InitNodeData(&Node);
	/*InitTlvAutho(&sTlvInfo);*//*PLUTONL-4445*/
	/* 	
	GetIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen);
	GetSecurityData(PURGE_TIME, tmpField, PurgeTime, &tmpFieldLen);

	if (0.000 == atol(PurgeTime)) 
	{
		GetAcquirerPurgeTime(&lAcqPurgeTime);
		Node.tPurgeTime = lAcqPurgeTime;	
	} 
	else 
	{
		Node.tPurgeTime = atol(PurgeTime);
	}
	*/
	GetAcquirerPurgeTimeMS(&sAcqPurgeTimeVal);
	Node.tPurgeTime = sAcqPurgeTimeVal.tv_sec;
	Node.stPurgeTime.tv_sec = sAcqPurgeTimeVal.tv_sec;
	Node.stPurgeTime.tv_usec = sAcqPurgeTimeVal.tv_usec;

	GetIsoField(ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen);
	memcpy(Node.szFstKey, "1304", 4);
	memcpy(Node.szFstKey + 4, tmpField, 12);
	Node.msgId =IsoInfo->msgId;

	sprintf(sLine, "Insert Key=[%.16s]", Node.szFstKey);
	trace_event(sLine, FATAL);

/*Start ICH20191018 CIS/BASE1 Excp file mngt*/

	if(sSafInd[0]=='Y')
	{
		memcpy (Node.sFstPrvtData , "SAF" , 3);
		Node.nFstPrvtDataLen = 3;
	}
/*End ICH20191018 CIS/BASE1 Excp file mngt*/

	/* CFE_20200420 CIS/BASE1 Excp file mngt */

	/*PLUTONL-4445: commented
	result = IsoToTlv ( nIndexCtx, IsoInfo, &sTlvInfo );
	if ( result != OK )
	{
		trace_event ( "IsoToTlv Error ...", ERROR );
		return ( NOK );
	}*/

	/*AuthoBuildTlv( sTlvBuffer, &sTlvInfo);*//*PLUTONL-4445*/
	memcpy(Node.sSndBuf, sTlvBuffer, strlen(sTlvBuffer));
	Node.nSndBufLen = strlen(sTlvBuffer);
	
	/* CFE_20200420 CIS/BASE1 Excp file mngt */
	

	Length = IsoBuildMsg(Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	if (InsertRequest('B', &Node) != OK) 
	{
		trace_event("End FCT_UPD_REQ_TO_NW(NOK) ERROR", PROCESSING);
		return (NOK);
	}

	InitVisaInfo(&VisaInfo);
	VisaInfo.nMsgType = 302;

	/* (2) Primary Account Number */
	if (GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_CARD_NBR, &VisaInfo, tmpField, tmpFieldLen);

	/* (7) Transaction Date/Time */
	GetIsoField(ISO_XMIT_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddVisaField(VISA_XMIT_TIME, &VisaInfo, tmpField, 10);

	/* (11) Trace Audit Number */
	if (GetIsoField(ISO_AUDIT_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_AUDIT_NBR, &VisaInfo, tmpField, 6);
	else 
	{
		memset(tmpField, 0, sizeof(tmpField));
		GetInternalStan(nIndexCtx, tmpField);
		AddVisaField(VISA_AUDIT_NBR, &VisaInfo, tmpField, 6);
	}

	/* (20): PAN Country Code */
	if (GetIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_PAN_COUNTRY, &VisaInfo, tmpField, 3);

	/* (32) Acquiring Institution Code */
	if (GetIsoField(ISO_ACQR_ID, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_ACQR_ID, &VisaInfo, tmpField, tmpFieldLen);

	/* Field 34: PAN Extended */
	if (GetIsoField(ISO_PAN_EXTENDED, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_PAN_EXTENDED /*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/, &VisaInfo, tmpField, tmpFieldLen);

	/* (37) Reference Number */
	GetIsoField(ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddVisaField(VISA_REFERENCE_NBR, &VisaInfo, tmpField, 12);

	/* (73) Action Date */
	/* not used in case of record delete or inquiry */
	GetIsoField(ISO_FUNCTION_CODE, IsoInfo, FctCode, &tmpFieldLen);
	if ((memcmp(FctCode, P7_F024_DEL_RECORD, P7_F024_LEN) != 0) && 
		(memcmp(FctCode, P7_F024_INQ_RECORD, P7_F024_LEN) != 0)) 
	{
		if (GetIsoField(ISO_ACTION_DATE, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddVisaField(VISA_ACTION_DATE, &VisaInfo, tmpField, 6);
	}

	/* (91) File Update Code */
	switch (atoi(FctCode)) 
	{
	case 301:		/* add record */
		tmpField[0]='1';
		break;
	case 302:		/* change record */
		tmpField[0]='2';
		break;
	case 303:		/* delete record */
		tmpField[0]='3';
		break;
	case 304:		/* replace record */
		tmpField[0]='4';
		break;
	case 305:		/* inquire */
		tmpField[0]='5';
		break;
	}

	AddVisaField(VISA_FILE_UPDATE, &VisaInfo, tmpField, 1);

	/* (92) File Security Code */
	if (GetIsoField(ISO_FILE_SEC_CODE, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_FILE_SECURITY, &VisaInfo, tmpField, 2);
	else
		AddVisaField(VISA_FILE_SECURITY, &VisaInfo, "00", 2);

	/* (101) Filename */
	GetIsoField(ISO_FILE_NAME, IsoInfo, tmpField, &tmpFieldLen);
	AddVisaField(VISA_FILE_NAME, &VisaInfo, tmpField, tmpFieldLen);

	/* (127) File Records - Action & Data */
	if (GetIsoField(ISO_RECORD_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		AddVisaField(VISA_RECORD_DATA, &VisaInfo, tmpField, tmpFieldLen);
	}
	/* send message */
	Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, &VisaInfo);
	VisaBuildHeader(REQUEST, &(VisaInfo.sHeader), NULL, Length, StationId);
	memcpy(sBuffer, (char *) &(VisaInfo.sHeader), VISA_HDR_LEN);
	Length += VISA_HDR_LEN;

	/*Start IBO20200527 RAWMSG*/
	if (g_sCaptureMsg[NW_OUTG_REQ] == 'Y')
	{
		LogVisaMessage(nIndexCtx, &VisaInfo, IsoInfo, OUTG_REQ);
	}
	/*End IBO20200527 RAWMSG*/

	dump_buffer(sBuffer, Length, 'E', 'L', 'O');

	DumpVisa(sBuffer);

	if (WriteLineMsg(sBuffer, Length)) 
	{
		trace_event("FCT_UPD_REQ_TO_NW(): WriteLineMsg failed.", ERROR);
		FetchRequest('B', &Node);
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}

	MsgFwdReqSent(IsoInfo->msgId);

	trace_event("End   FCT_UPD_REQ_TO_NW(OK)", PROCESSING);
	return (OK);
}


/****************************************************************************/
/* FCT_UPD_REP_FROM_NW()                                                  */
/* */
/* Processes a file update response message (0312) received from VISA.    */
/* Input:  VISA received message                                          */
/* Output: ISO formatted message                                          */
/****************************************************************************/
int 
FCT_UPD_REP_FROM_NW(int nIndexCtx, TSVisaInfo * VisaInfo)
{
	TSBalExchange   BalMessage;
	TSNodeData      Node;
	TSIsoInfo       IsoInfo;
	TSTlvAutho     	 sTlvInfo;/*ICH20191018 CIS/BASE1 Excp file mngt*/

	char            tmpField[256];
	char            sResponseCode[3+1];
	int             tmpFieldLen;
	char            SourceId[8 + 1];
	int             Length,nResult;
	char            sLine[MAX_LINE_TRC];
	char           	sTlvBuffer[LG_MAX];/*ICH20191018 CIS/BASE1 Excp file mngt*/
	char 			sSecurityData [128 + 1];


	char			SourceNodeId [NODE_ID_LEN + 1];
	int				nBal;

	trace_event("Start   FCT_UPD_REP_FROM_NW()", PROCESSING);

	memset(tmpField, 0, sizeof(tmpField));
	memset(sResponseCode, 0, sizeof(sResponseCode));
	memset (sTlvBuffer,      0, sizeof(sTlvBuffer));/*ICH20191018 CIS/BASE1 Excp file mngt*/
	memset(SourceNodeId, 0, sizeof(SourceNodeId));
	memset (sSecurityData, 0, sizeof(sSecurityData));

	/* Retrieve original requester */
	GetVisaField(VISA_REFERENCE_NBR, VisaInfo, tmpField, &tmpFieldLen);

	InitNodeData(&Node);
	memcpy(Node.szFstKey, "1304", 4);
	memcpy(Node.szFstKey + 4, tmpField, 12);

	sprintf(sLine, "==> Fetch Key=[%.16s]", Node.szFstKey);
	trace_event(sLine, FATAL);

	switch (FetchRequest('B', &Node)) 
	{
	case ENOEXIST:
		trace_event("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event("End FCT_UPD_REP_FROM_NW (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;

	}

	InitIsoInfo(&IsoInfo);
	AnalyseIso(Node.sFstBuf, &IsoInfo);
	IsoInfo.nMsgType = 1314;

	MsgFwdRepReceived(Node.msgId);


	/* get just the response code, all other ISO fields remain unchanged */
	GetVisaField(VISA_RESPONSE_CODE, VisaInfo, tmpField, &tmpFieldLen);
	TranslateRespCode(nIndexCtx, ResourceId, tmpField, "RP", sResponseCode);
	InsertIsoField(ISO_RESPONSE_CODE, &IsoInfo, sResponseCode, 3);

/*Start ICH20191018 CIS/BASE1 Excp file mngt*/
	if (memcmp(Node.sFstPrvtData, "SAF",3) == 0) {

		trace_event("SAF  Processing ", PROCESSING);

		InitTlvAutho(&sTlvInfo);
		nResult = IsoToTlv ( nIndexCtx , &IsoInfo , &sTlvInfo );
		if ( nResult != OK )
		{
			trace_event ("FCT_UPD_REP_FROM_NW (NOK): IsoToTlv() Failed", PROCESSING);
			return ( NOK );
		}

		AuthoBuildTlv( sTlvBuffer, &sTlvInfo);

		SafResponseTraitement(nIndexCtx,sTlvBuffer,"1314", sResponseCode,tmpField);
		trace_event ("End   FCT_UPD_REP_FROM_NW(OK)", PROCESSING);
		return (OK);
	}
/*End ICH20191018 CIS/BASE1 Excp file mngt*/

	/* Put answer message in queue for pick up by requesting party */
	GetIsoField(ISO_SECURITY_DATA, &IsoInfo, sSecurityData, &tmpFieldLen);
	memset(SourceId, 0, sizeof(SourceId));
	GetSecurityData(SOURCE_PID, sSecurityData, SourceId, &tmpFieldLen);
	GetSecurityData(SOURCE_PID, sSecurityData, SourceNodeId, &tmpFieldLen);

	/*Start IBO20200517 RAWMSG*/
	if (g_sCaptureMsg[NW_INC_RESP] == 'Y')
	{
		LogVisaMessage(nIndexCtx, VisaInfo, &IsoInfo, INC_RESP);
	}
	/*End IBO20200517 RAWMSG*/

	if (memcmp(NodeId, SourceNodeId, NODE_ID_LEN) != 0)
	{
		BalMessage.sHeader.nPartnerId = getuid();
		nBal = nBalBrr;
	}
	else
	{
		BalMessage.sHeader.nPartnerId = atoi(SourceId);
		nBal = nBalMsg;
	}
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1314;
	Length = IsoBuildMsg(BalMessage.sBody, &IsoInfo);
	BalMessage.sHeader.nLen = Length;
	BalMessage.sHeader.msgId = Node.msgId;

	if (WriteBalMsg(nBal, &BalMessage) != OK)
	{
		trace_event("WriteBalMsg failed", ERROR);
		IntWriteBalExeption(nIndexCtx,&IsoInfo); /* ELKAM PLUTONL-4061 */
		return (NOK);
	}

	MsgFwdRepReplied(BalMessage.sHeader.msgId);

	trace_event("End   FCT_UPD_REP_FROM_NW(OK)", PROCESSING);
	return (OK);
}
