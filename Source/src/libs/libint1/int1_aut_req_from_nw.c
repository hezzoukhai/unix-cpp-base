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
/* FCT_AUT_REQ_FROM_NW()													*/
/*																			*/
/****************************************************************************/
int INT1_FCT_AUT_REQ_FROM_NW(int nIndexCtx, MsgInfo * pkMsgInfo, TSIsoInfo*  pkIsoInfo, char* sTblInd)
{
	MsgInfo			kRespMsgInfo;
	char            sTlvBuffer[LG_MAX];
	int             Length, result;	
	int				nResult;
	int				nLength;


	trace_event("Start  INT1_FCT_AUT_REQ_FROM_NW()", PROCESSING);

	memset(sTlvBuffer      ,'\0',sizeof(sTlvBuffer      ));
	InitNetworkMsgInfo(&kRespMsgInfo);
	

	pkIsoInfo->msgId = MsgIsoReqReceived(ResourceId,pkIsoInfo);
	
	result = ServiceProcessing(		nIndexCtx,
									ResourceId,
									sTblInd,
									AcquirerBank,
									"Y",
									"G",
									"N",
									sTlvBuffer, 
									pkIsoInfo);

	switch (result) 
	{
	case ROUTAGE:
		/* forward request for authorization */
		if (Int1ForwardAutRequest(nIndexCtx, pkIsoInfo, sTlvBuffer, pkMsgInfo) != OK) 
		{
			/* respond to NW that issuer was not available */
			PutIsoField(ISO_RESPONSE_CODE, pkIsoInfo, ISSUER_UNAVAILABLE_ACTION, 3);
			trace_event("End  INT1_FCT_AUT_REQ_FROM_NW(OK)", PROCESSING);
			return (I1RET_PROC_ERR);
		} 
		else 
		{	
			/* request was successfully passed on to issuer interface */
			trace_event("End  INT1_FCT_AUT_REQ_FROM_NW(OK)", PROCESSING);
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
		trace_event("End  INT1_FCT_AUT_REQ_FROM_NW(SEVERE_ERROR)", PROCESSING);
		return (I1RET_PROC_ERR);
		break;
	}

	trace_event("End  INT1_FCT_AUT_REQ_FROM_NW(NOK)", PROCESSING);
	return (I1RET_PROC_ERR);

}

/****************************************************************************/
/* Forwards an authorization request to the issuer interface.              */
/****************************************************************************/
int Int1ForwardAutRequest(int nIndexCtx, TSIsoInfo * IsoInfo, char *sTlvBuffer, MsgInfo * pkMsgInfo)
{
	TSNodeData      Node;
	TSBalExchange   BalMessage;
	MsgInfo			kNewMsgInfo;
	char            sRespCode[3 + 1];
	char            tmpField[1024];
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
	/*time_t          sAcqPurgeTime;
	time_t          sIssPurgeTime;
	int             nIssTime;
	int             nAcqTime;*/
	char            sLine[MAX_LINE_TRC];
	pid_t			nDestPid;
	int				nIsExternal = NOK;
	int				nBal = nBalMsg;
	/*Start IBO20200225*/
	char            sPurgeTimeMS[4];
	struct timeval	sAcqPurgeTimeVal;	
	struct timeval	sIssPurgeTimeVal;
	int             FieldLen;
	char            DestId[8 + 1];
	/*End IBO20200225*/


	memset(sLine           ,'\0',sizeof(sLine           ));
    memset(tmpField        ,'\0',sizeof(tmpField        ));
    memset(DestNodeId      ,'\0',sizeof(DestNodeId      ));
    memset(DestResourceId  ,'\0',sizeof(DestResourceId  ));
    memset(DestPid         ,'\0',sizeof(DestPid         ));
    memset(BridgeResourceId,'\0',sizeof(BridgeResourceId));
    memset(sPurgeTime      ,'\0',sizeof(sPurgeTime      ));
    memset(sSecurityData   ,'\0',sizeof(sSecurityData   ));

	trace_event("Start  Int1ForwardAutRequest()", PROCESSING);


	/* Build node to insert request into list */
	InitNodeData(&Node);


	GetIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, &tmpFieldLen);
	GetSecurityData(DESTINATION_NODE_ID, sSecurityData, DestNodeId, &tmpFieldLen);
	GetSecurityData(DESTINATION_RESOURCE, sSecurityData, DestResourceId, &tmpFieldLen);
	GetSecurityData(DESTINATION_PID, sSecurityData, DestPid, &tmpFieldLen);
	nDestPid = atol(DestPid);

	/*RevEBE: need timeout in milliseconds*/
	/*GetAcquirerPurgeTime(&sAcqPurgeTime);
	GetIssuerPurgeTime(&sIssPurgeTime);
	nAcqTime = nTimerAcq;
	

	sAcqPurgeTime -= 1;
	sIssPurgeTime -= 1;

	Node.tPurgeTime = sAcqPurgeTime;*/
	
	/*Start IBO20200225*/
	GetAcquirerPurgeTimeMS(&sAcqPurgeTimeVal);
	GetIssuerPurgeTimeMS(&sIssPurgeTimeVal);


	sAcqPurgeTimeVal.tv_sec -= 1;
	sIssPurgeTimeVal.tv_sec -= 1;

	Node.tPurgeTime = sAcqPurgeTimeVal.tv_sec;
	Node.stPurgeTime.tv_sec = sAcqPurgeTimeVal.tv_sec;
	Node.stPurgeTime.tv_usec = sAcqPurgeTimeVal.tv_usec;
	/*End IBO20200225*/

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

	sprintf(sLine, "==> Insertkey = [%.10s***] ", Node.szFstKey);
	trace_event(sLine, TRACE);

	sprintf(sLine, "==> node id[%s], DestNodeId[%s] ", NodeId,DestNodeId);
	trace_event(sLine, TRACE);

	if( memcmp(NodeId,DestNodeId,NODE_ID_LEN) != 0 )
	{
		/*SKA20220829 PLUTONL-5061 We do no longer need to get the resource id of the specific bridgeR located in the same node, 
		because when we determine that the request is to be forwarded, we simply input it in the newly created queue indepently of which bridgeR is going to pick it up.
		nResult = GetBridgeResourceId(nIndiceCtx, NodeId, BridgeResourceId,&nDestPid);
		if( nResult != OK )
		{
			sprintf(sLine,">> No defined bridge for node id [%s]",NodeId);
			trace_event(sLine, ERROR);
			return NOK;
		}*/
		nIsExternal = OK;
	}

	Node.msgId = IsoInfo->msgId;

	if (InsertRequest('L', &Node) != OK) 
	{
		trace_event("Int1ForwardAutRequest(): InsertRequest() failed", PROCESSING);
		return (NOK);
	}

	/* Add issuer purge time to Security Data Field (53) */
	/*sprintf(sPurgeTime, "%.*lu",PURGE_TIME_LEN, sIssPurgeTime);
	PutSecurityData(PURGE_TIME, sSecurityData, sPurgeTime, PURGE_TIME_LEN);*/
	
	/*Start IBO20200225*/
	GetIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, &FieldLen);
	GetSecurityData(DESTINATION_PID, sSecurityData, DestId, &FieldLen);
	sprintf(sPurgeTime, "%.*lu",PURGE_TIME_LEN, sIssPurgeTimeVal.tv_sec); 
	PutSecurityData(PURGE_TIME, sSecurityData, sPurgeTime, PURGE_TIME_LEN);
	sprintf(sPurgeTimeMS, "%03ld", (sIssPurgeTimeVal.tv_usec)/1000);
	PutSecurityData(PURGE_TIME_MS, sSecurityData, sPurgeTimeMS, PURGE_TIME_MS_LEN);	
    /*End IBO20200225*/
	
	PutIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, strlen(sSecurityData));
	dump_p7_struct(IsoInfo);


	BalMessage.sHeader.nPartnerId = nDestPid;
	/*SKARROUMI PLUTONL-4900*/
	if (nIsExternal == OK)
	{
		BalMessage.sHeader.nPartnerId = getuid();
		nBal = nBalBrr;
	}
	/*SKARROUMI PLUTONL-4900*/
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1100;
	Length = IsoBuildMsg(BalMessage.sBody, IsoInfo);
	BalMessage.sHeader.nLen = Length;
	BalMessage.sHeader.msgId = IsoInfo->msgId;

	if (WriteBalMsg(nBal, &BalMessage) != OK) 
	{
		trace_event("ForwardAutRequest(): WriteBalMsg() failed", PROCESSING);
		/* free request from list */
		FetchRequest('L', &Node);
		return (NOK);
	}

	MsgReqForwarded(IsoInfo->msgId,DestResourceId,BalMessage.sHeader.nPartnerId);

	trace_event("End  ForwardAutRequest(OK)", PROCESSING);
	return (OK);
}


