#include <stdio.h>
#include <iso_hps.h>
#include <iso_fields.h>
#include <tlv_autho.h>
#include <tlv_fields.h>
#include <security_data.h>

#include <int1_struct.h>
#include <int1.h>


static int BuildTimeOutReversalMsg(TSIsoInfo * IsoInfo, TSIsoInfo * IsoRevInfo);
static int Int1AcquirerTimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg);
static int  Int1IssuerTimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg);
int Int1TimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg);


static int Int1AcquirerTimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg)
{
	TSIsoInfo       IsoInfo;
	TSIsoInfo       IsoRevInfo;
	char            sTlvBuffer[LG_MAX + 1];
	char            sProcCode[12 + 1];
	char			sTransFlag[24];
	TSTlvAutho      sTlvInfo;
	char            sLine[MAX_LINE_TRC];
	int             tmpFieldLen;
	int             nLength;
	int             nEventCode = OK;
	int             nRetCode;
	char            SourceId[8 + 1];
	int             Length;
	char            sSecurityData[64 + 1];
	TSBalExchange   BalMessage;
	char			SourceNodeId[NODE_ID_LEN + 1];
	int				nBal;

	trace_event("Start AcquirerTimeProcMsg()", PROCESSING);

	if (sTimeMsg->sData.nFstPrvtDataLen != 0) 
	{
		trace_event("Private Data > 0", PROCESSING);

		if (memcmp(sTimeMsg->sData.sFstPrvtData, "SAF", sTimeMsg->sData.nFstPrvtDataLen) == 0) 
		{
			trace_event("SAF Time Out Processing ", PROCESSING);
			Int1SafTimeOutTraitement(nIndiceCtx, sTimeMsg->sData.sSndBuf);
			return (OK);
		}
	}

	InitIsoInfo(&IsoInfo);
	InitIsoInfo(&IsoRevInfo);
	DumpIso(sTimeMsg->sData.sFstBuf);
	AnalyseIso(sTimeMsg->sData.sFstBuf, &IsoInfo);
	dump_p7_struct(&IsoInfo);

	strncpy(sTlvBuffer,sTimeMsg->sData.sSndBuf,sizeof(sTlvBuffer) - 1);

	GetTlvBuffer(AUTO_TRANSACTION_FLAG, sTlvBuffer, sTransFlag, &nLength);

	switch (IsoInfo.nMsgType) {
	case 1100:
	case 1200:

		memset(sProcCode, 0, sizeof(sProcCode));
		GetIsoField(ISO_PROC_CODE, &IsoInfo, sProcCode, &tmpFieldLen);

		sprintf(sLine, "==> sProcCode=[%s]", sProcCode);
		trace_event(sLine, FATAL);

		if (memcmp(ReversalGeneration, "T", 1) == 0 && sTransFlag[0]== 'T' ) 
		{
			InitTlvAutho(&sTlvInfo);
			memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
			BuildTimeOutReversalMsg(&IsoInfo, &IsoRevInfo);
			IsoToTlv(nIndiceCtx, &IsoRevInfo, &sTlvInfo);
			PutTlvAutho(AUTO_CAPTURE_CODE, &sTlvInfo, ResourceId, 6);
			PutTlvAutho(AUTO_ROUTING_CODE, &sTlvInfo, ResourceId, 6);
			AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
			PrintTlvBuffer(sTlvBuffer);
			InsertionSaf(nIndiceCtx, sTlvBuffer, ResourceId, ResourceId);
		}
		/** ******************* **/
		/* forward it to requester */

		PutIsoField(ISO_RESPONSE_CODE, &IsoInfo, P7_F039_ISS_UNV, P7_F039_LEN);
		memset(sSecurityData, 0, sizeof(sSecurityData));
		memset(SourceId, 0, sizeof(SourceId));
		memset(SourceNodeId, 0, sizeof(SourceNodeId));
		GetIsoField(ISO_SECURITY_DATA, &IsoInfo, sSecurityData, &Length);
		GetSecurityData(SOURCE_PID, sSecurityData, SourceId, &Length);
		GetSecurityData(SOURCE_NODE_ID, sSecurityData, SourceNodeId, &Length);

		dump_p7_struct(&IsoInfo);

		IsoInfo.nMsgType = IsoInfo.nMsgType + (IsoInfo.nMsgType % 2 > 0 ? 9 : 10);

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
		BalMessage.sHeader.nCommand = IsoInfo.nMsgType;
		Length = IsoBuildMsg(BalMessage.sBody, &IsoInfo);
		BalMessage.sHeader.nLen = Length;

		if (WriteBalMsg(nBal, &BalMessage) != OK) 
		{
			trace_event("End FCT_AUT_REP_FROM_NW(): WriteBal() failed.", ERROR);
			return (NOK);
		}

		break;
	}

	MsgTimeout(sTimeMsg->sData.msgId, ResourceId);
	trace_event("End   AcquirerTimeProcMsg()", PROCESSING);
	return (OK);
}

static int  Int1IssuerTimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg)
{
	MsgInfo			kMsgInfo;
	MsgInfo			kNewMsgInfo;
	TSIsoInfo       IsoInfo;
	TSTlvAutho      sTlvInfo;
	char            sTlvBuffer[LG_MAX];
	char            sLine[MAX_LINE_TRC];
	char			sStan[6 + 1];
	int             nLength;
	int             nEventCode = OK;
	int             nRetCode;

	trace_event("IssuerTimeProcMsg() Start", PROCESSING);

	InitIsoInfo(&IsoInfo);
	InitNetworkMsgInfo(&kMsgInfo);
	InitNetworkMsgInfo(&kNewMsgInfo);
	
	AnalyseMsg(sTimeMsg->sData.sFstBuf, &kMsgInfo);

	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	memset(sStan, 0, sizeof(sStan));
	memcpy(sTlvBuffer, sTimeMsg->sData.sSndBuf, sTimeMsg->sData.nSndBufLen);

	PrintTlvBuffer(sTlvBuffer);

	
	Int1UserNwMsgToIso(nIndiceCtx, &kNewMsgInfo, &IsoInfo,&kMsgInfo);

	switch (IsoInfo.nMsgType) {
	case 1100:
	case 1200:
		nRetCode = OnTimeoutIsoServiceRequest(nIndiceCtx,sTimeMsg->sens,&IsoInfo,sTlvBuffer);
		if( nRetCode == ROUTAGE )
		{
			trace_event(" ROUTAGE Returned ", PROCESSING);
			/* put back our original ISO Stan nbr */
			GetTlvBuffer ( AUTO_INTERNAL_STAN , sTlvBuffer , sStan ,&nLength);
			PutIsoField(ISO_AUDIT_NBR, &IsoInfo, sStan, nLength);
			if (Int1ForwardAutRequest(nIndiceCtx, &IsoInfo, sTlvBuffer, &kMsgInfo) != OK) 
			{
				/* respond that issuer was not available */
				PutIsoField(ISO_RESPONSE_CODE, &IsoInfo, ISSUER_UNAVAILABLE_ACTION, 3);
				nRetCode = NOK;
			} 
			else 
			{	
				/* request was successfully passed on */
				trace_event("End  IssuerTimeProcMsg(OK)", PROCESSING);
				return (OK);
			}
		}
		else
		{
			InitTlvAutho ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso    ( nIndiceCtx  ,&sTlvInfo , &IsoInfo  , RESPONSE );
		}

		switch (nRetCode) {
		case OK:	/* the interface itself replies to the network */
		case NOK:	/* the interface itself replies to the network */
			trace_event(" Retour AuthRequProcessing :  NOK/OK ", PROCESSING);
			if (Int1ReplyToNw(nIndiceCtx, &IsoInfo, &kMsgInfo) == OK) 
			{
				trace_event("End  FCT_AUT_REQ_FROM_NW(OK)", PROCESSING);
				return (OK);
			} 
			else 
			{
				trace_event("End  FCT_AUT_REQ_FROM_NW(NOK)", PROCESSING);
				return (NOK);
			}
			break;
		case SEVERE_ERROR:	/* shutdown interface */
			trace_event(" Retour AuthRequProcessing :  SEVERE_ERROR ", PROCESSING);
			Int1ReplyToNw(nIndiceCtx, &IsoInfo, &kMsgInfo);
			trace_event("End  FCT_AUT_REQ_FROM_NW(SEVERE_ERROR)", PROCESSING);
			return (NOK);
			break;
		}
	}
	trace_event("End   IssuerTimeProcMsg()", PROCESSING);
	return (OK);
}


int Int1TimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg)
{
	trace_event("Time Out Detected ....", PROCESSING);

	if (sTimeMsg->sens == 'B')
		AcquirerTimeProcMsg(nIndiceCtx, sTimeMsg);
	else if (sTimeMsg->sens == 'L')
		IssuerTimeProcMsg(nIndiceCtx, sTimeMsg);
	else if (sTimeMsg->sens == 'S')
		FCT_SHUTDOWN_TIME(nIndiceCtx);

	return OK;
}

static int BuildTimeOutReversalMsg(TSIsoInfo * IsoInfo, TSIsoInfo * IsoRevInfo)
{

	char            OrigDataElements[P7_F056_MAX_LEN + 1];
	char            tmpField[BUF_LG_MAX];
	int             tmpFieldLen;
	int             nPos;
	char			AcquirerId[]="12345"; /*EBETODO*/

	dump_p7_struct(IsoInfo);

	IsoRevInfo->nMsgType = 1420;

	/*---------------------------------------------------------------------------------*/
	/* (2) Primary Account Number */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_CARD_NBR, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (3) Processing Code */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_PROC_CODE, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_PROC_CODE, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (4): Transaction Amount */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_TRANS_AMOUNT, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_TRANS_AMOUNT, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (7) Transaction Date/Time */
	/*---------------------------------------------------------------------------------*/
	current_gmt_date(tmpField);
	AddIsoField(ISO_XMIT_TIME, IsoRevInfo, tmpField + 2, 10);

	/*---------------------------------------------------------------------------------*/
	/* (11) Trace Audit Number */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_AUDIT_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_AUDIT_NBR, IsoRevInfo, tmpField, tmpFieldLen);


	/*---------------------------------------------------------------------------------*/
	/* (12) & (13) Local Transaction Time & Date */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_TRANS_DATE_TIME, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_TRANS_DATE_TIME, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (18) Merchant Type                 */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_MERCHANT_TYPE, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_MERCHANT_TYPE, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (19) Acquiring Country Code        */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_ACQR_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_ACQR_COUNTRY, IsoRevInfo, tmpField, tmpFieldLen);


	/*---------------------------------------------------------------------------------*/
	/* (20): PAN Country Code */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_PAN_COUNTRY, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (22) POS Entry Mode */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_POS_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_POS_DATA, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (32) Acquiring Institution Id Code */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_ACQR_ID, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_ACQR_ID, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (33) Forwarding Institution Id */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_FORWD_ID, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_FORWD_ID, IsoRevInfo, tmpField, tmpFieldLen);
	else
		AddIsoField(ISO_FORWD_ID, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (37) Reference Number */
	/*---------------------------------------------------------------------------------*/
	GetIsoField(ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddIsoField(ISO_REFERENCE_NBR, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (38) Auth Code          */
	/*---------------------------------------------------------------------------------*/
	AddIsoField(ISO_AUTHOR_ID, IsoRevInfo, "A99999", 6);

	/*---------------------------------------------------------------------------------*/
	/* (39) Response Code Code */
	/*---------------------------------------------------------------------------------*/
	AddIsoField(ISO_RESPONSE_CODE, IsoRevInfo, "911", 3);


	/*---------------------------------------------------------------------------------*/
	/* (41) Card Acceptor Terminal Id */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_TERMINAL_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_TERMINAL_NBR, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (42) Card Acceptor Name/Location */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_OUTLET_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_OUTLET_NBR, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (43) Card Acceptor Name/Location */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField(ISO_TERMINAL_ADR, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_TERMINAL_ADR, IsoRevInfo, tmpField, tmpFieldLen);

    if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_ADTNL_DATA_PRIV, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (49) Transaction Currency Code */
	/*---------------------------------------------------------------------------------*/
	GetIsoField(ISO_TRANS_CRNCY, IsoInfo, tmpField, &tmpFieldLen);
	AddIsoField(ISO_TRANS_CRNCY, IsoRevInfo, tmpField, 3);

	/*---------------------------------------------------------------------------------*/
	/* (90) Original Data Elements  */
	/*---------------------------------------------------------------------------------*/
	nPos = 0;
	memset(OrigDataElements, 0, sizeof(OrigDataElements));
	sprintf(OrigDataElements,"%.4d",IsoInfo->nMsgType);
	nPos += 4;

	GetIsoField(ISO_AUDIT_NBR, IsoInfo, tmpField, &tmpFieldLen);
	memcpy(OrigDataElements + nPos, tmpField, 6);
	nPos += 6;

	memcpy(OrigDataElements + nPos, "00", 2);
	nPos += 2;

	GetIsoField(ISO_XMIT_TIME, IsoInfo, tmpField, &tmpFieldLen);
	memcpy(OrigDataElements + nPos, tmpField, 10);
	nPos += 10;

	sprintf(OrigDataElements + nPos, "%02lu", strlen(AcquirerId));
	nPos += 2;
	sprintf(OrigDataElements + nPos, "%011s", AcquirerId);
	nPos += 11;
	memcpy(OrigDataElements + nPos, "0000000000000", 13);
	nPos += 13;


	AddIsoField(ISO_ORIG_DATA, IsoRevInfo, OrigDataElements, nPos);

	dump_p7_struct(IsoRevInfo);

	trace_event("End  BuildTimeOutReversalMsg (OK)", PROCESSING);
	return (OK);
}


