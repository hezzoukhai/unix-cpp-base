#include <iso_hps.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <list_thr.h>
#include <security_data.h>
#include <queue.h>
#include <hsm_inc.h>
#include <tlv_fields.h>
#include <tlv_privfields.h>
#include <int1_struct.h>
#include <int1.h>


/****************************************************************************/
/* INT1_FCT_ADV_REQ_FROM_NW()												*/
/*																			*/
/****************************************************************************/

int INT1_FCT_ADV_REQ_FROM_NW(int nIndexCtx, MsgInfo * pkMsgInfo, TSIsoInfo*  pkIsoInfo, char* sTblInd)
{
	MsgInfo			kRespMsgInfo;
	
	char            sTlvBuffer[LG_MAX];
	char            ProcessingCode[6 + 1];
	int             Length, result;
	char			sLocAcquirerBank[6 + 1];
	char			sBankCode[6 + 1];
	char			szAcquirerId[24];
	char			szCardAccId[24];
	char			szTermId[24];
	char			sPrivData[24];
	int				nResult;
	int				nLength;


	trace_event("Start  INT1_FCT_ADV_REQ_FROM_NW()", PROCESSING);

	memset(sLocAcquirerBank,'\0',sizeof(sLocAcquirerBank));
	memset(sBankCode,'\0',sizeof(sBankCode));
	memset(szAcquirerId,'\0',sizeof(szAcquirerId));
	memset(szCardAccId,'\0',sizeof(szCardAccId));
	memset(szTermId,'\0',sizeof(szTermId));
	memset(sPrivData,'\0',sizeof(sPrivData));
	
	InitNetworkMsgInfo(&kRespMsgInfo);
	

	pkIsoInfo->msgId = MsgIsoReqReceived(ResourceId,pkIsoInfo);
	trace_message(PROCESSING, "[SWMON1]: IsoInfo.msgId (%lu)",pkIsoInfo->msgId );

	GetMsgField(ISO_ACQR_ID, pkMsgInfo, szAcquirerId, &nLength);
	GetMsgField(ISO_OUTLET_NBR, pkMsgInfo, szCardAccId, &nLength);
	GetMsgField(ISO_TERMINAL_NBR, pkMsgInfo, szTermId, &nLength);

	memcpy(sLocAcquirerBank,AcquirerBank,6);

	GetIsoField(ISO_PROC_CODE, pkIsoInfo, ProcessingCode, &Length);

	/* Call the pre-control module */
	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	
	result = ServiceProcessing( nIndexCtx,
		                        ResourceId,
		                        sTblInd,
		                        sLocAcquirerBank,
		                        "Y",
		                        "G",
		                        "N",
		                        sTlvBuffer, 
		                        pkIsoInfo);

	trace_message(PROCESSING, "[SWMON2]: IsoInfo.msgId (%lu)", pkIsoInfo->msgId );


	switch (result) 
	{
	case ROUTAGE:
		/* forward request for authorization */
		if (Int1ForwardAutRequest(nIndexCtx, pkIsoInfo, sTlvBuffer, pkMsgInfo) != OK) 
		{
			/* respond to VISA that issuer was not available */
			PutIsoField(ISO_RESPONSE_CODE, pkIsoInfo, ISSUER_UNAVAILABLE_ACTION, 3);
			trace_event("End  INT1_FCT_ADV_REQ_FROM_NW(OK)", PROCESSING);
			return (I1RET_PROC_ERR);
		} 
		else 
		{	
			/* request was successfully passed on to issuer interface */
			trace_event("End  INT1_FCT_ADV_REQ_FROM_NW(OK)", PROCESSING);
			return (I1RET_ROUTED);
		}
		break;

	case MALFUNCTION:
	case NOK:		/* the interface itself replies to the network */
	case OK:		/* the interface itself replies to the network */

		trace_message(PROCESSING, "[SWMON3]: IsoInfo.msgId (%d)", pkIsoInfo->msgId );
		return I1RET_PROCESSED;
		break;

	case SEVERE_ERROR:	/* shutdown interface */
		trace_event("End  INT1_FCT_ADV_REQ_FROM_NW(SEVERE_ERROR)", PROCESSING);
		return (I1RET_PROC_ERR);
		break;
	}

	trace_event("End  INT1_FCT_ADV_REQ_FROM_NW(NOK)", PROCESSING);
	return (I1RET_PROC_ERR);

}

/****************************************************************************/
/* Forwards an authorization request to the issuer interface.              */
/****************************************************************************/
/*int Int1ForwardAutRequest(int nIndexCtx, TSIsoInfo * IsoInfo, char *sTlvBuffer, MsgInfo * pkMsgInfo)
{
	TSNodeData      Node;
	TSBalExchange   BalMessage;
	MsgInfo			kNewMsgInfo;
	char            sRespCode[3 + 1];
	char            tmpField[256];
	char			DestNodeId[NODE_ID_LEN + 1];
	char			DestResourceId[RESOURCE_ID_LEN + 1];
	char            DestPid[PROC_PID_LEN + 1];
	char			BridgeResourceId[RESOURCE_ID_LEN + 1];
	char            sBuffer[1024];
	int             tmpFieldLen;
	int             Length;
	char            sPurgeTime[9 + 1];
	char            sSecurityData[128 + 1];
	int             i, result;
	int             nHeaderLength;
	int             nRetCode;
	time_t          sAcqPurgeTime;
	time_t          sIssPurgeTime;
	int             nIssTime;
	int             nAcqTime;
	char            sLine[MAX_LINE_TRC];
	pid_t			nDestPid;
	int				nIsExternal = NOK;




	trace_event("Start  Int1ForwardAutRequest()", PROCESSING);

	memset(DestResourceId, 0, sizeof(DestResourceId));


	/* Build node to insert request into list * /
	InitNodeData(&Node);


	GetIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, &tmpFieldLen);
	GetSecurityData(DESTINATION_NODE_ID, sSecurityData, DestNodeId, &tmpFieldLen);
	GetSecurityData(DESTINATION_RESOURCE, sSecurityData, DestResourceId, &tmpFieldLen);
	GetSecurityData(DESTINATION_PID, sSecurityData, DestPid, &tmpFieldLen);
	nDestPid = atol(DestPid);

	
	GetAcquirerPurgeTime(&sAcqPurgeTime);
	GetIssuerPurgeTime(&sIssPurgeTime);
	nAcqTime = nTimerAcq;
	

	sAcqPurgeTime -= 1;
	sIssPurgeTime -= 1;

	Node.tPurgeTime = sAcqPurgeTime;

	GetMsgField(ISO_AUDIT_NBR, pkMsgInfo, tmpField, &tmpFieldLen);
	sprintf(Node.szFstKey, "%04d", pkMsgInfo->nMsgType);
	memcpy(Node.szFstKey + 4, tmpField, 6);

	memset(tmpField, 0, sizeof(tmpField));
	tmpFieldLen = 0;

	GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen);
	memcpy(Node.szFstKey + 4 + 6, tmpField, tmpFieldLen);


	Node.nFstBufLen = BuildMsgInfoBuffer(Node.sFstBuf, pkMsgInfo);


	memcpy(Node.sSndBuf, sTlvBuffer, strlen(sTlvBuffer));
	Node.nSndBufLen = strlen(sTlvBuffer);

	sprintf(sLine, "==> Insertkey = [%.32s] ", Node.szFstKey);
	trace_event(sLine, TRACE);

	sprintf(sLine, "==> node id[%s], DestNodeId[%s] ", NodeId,DestNodeId);
	trace_event(sLine, TRACE);

	if( memcmp(NodeId,DestNodeId,NODE_ID_LEN) != 0 )
	{
		nRetCode = GetBridgeResourceId(nIndexCtx, NodeId, BridgeResourceId,&nDestPid);
		if( nRetCode != OK )
		{
			sprintf(sLine,">> No defined bridge for node id [%s]",NodeId);
			trace_event(sLine, ERROR);
			return NOK;
		}
		nIsExternal = OK;
	}

	Node.msgId = IsoInfo->msgId;

	if (InsertRequest('L', &Node) != OK) 
	{
		trace_event("Int1ForwardAutRequest(): InsertRequest() failed", PROCESSING);
		return (NOK);
	}

	/* Add issuer purge time to Security Data Field (53) * /
	sprintf(sPurgeTime, "%.*lu",PURGE_TIME_LEN, sIssPurgeTime);
	PutSecurityData(PURGE_TIME, sSecurityData, sPurgeTime, PURGE_TIME_LEN);
	PutIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, strlen(sSecurityData));
	dump_p7_struct(IsoInfo);


	BalMessage.sHeader.nPartnerId = nDestPid;
	BalMessage.sHeader.nSourceId = getpid();
	if( nIsExternal != OK )
	{
		BalMessage.sHeader.nCommand = 1100;
		Length = IsoBuildMsg(BalMessage.sBody, IsoInfo);
	}
	else
	{
		PutTlvBuffer(AUTO_SECURITY_DATA, sTlvBuffer, sSecurityData, strlen(sSecurityData));
		BalMessage.sHeader.nCommand = 4100;
		strncpy(BalMessage.sBody, sTlvBuffer, sizeof(BalMessage.sBody) - 1);
		Length = strlen(BalMessage.sBody);
		trace_event(">>>> IS EXTERNAL", PROCESSING);
	}

	BalMessage.sHeader.nLen = Length;
	BalMessage.sHeader.msgId = IsoInfo->msgId;

	if (WriteBalMsg(nBalMsg, &BalMessage) != OK) 
	{
		trace_event("ForwardAutRequest(): WriteBalMsg() failed", PROCESSING);
		/* free request from list * /
		FetchRequest('L', &Node);
		return (NOK);
	}

	MsgReqForwarded(IsoInfo->msgId,DestResourceId,BalMessage.sHeader.nPartnerId);

	trace_event("End  ForwardAutRequest(OK)", PROCESSING);
	return (OK);
}*/


