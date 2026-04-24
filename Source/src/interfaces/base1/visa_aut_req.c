#include <define.h>
#include <visa_define.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <iso_hps.h>
#include <iso_cps.h>
#include <iso_visa.h>
#include <visa_puf.h>
#include <visa_puf_fields.h>
#include <visa_param.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <list_thr.h>
#include <security_data.h>
#include <security_flags.h>
#include <queue.h>
#include <hsm_inc.h>
#include <resources.h>
#include <p7_define.h>
#include <p7_sid.h>
#include <visa_define.h>
#include <p7_common.h>
#include <p7_globals.h>
#include <iso8583.h>
#include <tlv_fields.h>
#include <tlv_privfields.h>

#include <msg_flow_mapping.h>
#include <nw_raw_msg.h>

/*************************************************************************/
/* FCT_AUT_REQ_FROM_NW()                                               */
/* */
/* Processes a BASEI authorization request (message 0100)              */
/* It calls the function to convert the message into an ISO format     */
/* structure, it then calls the pre-control module, then depending     */
/* on the action code it either sends the response message to VISA     */
/* or passes the request on to the server fo authorization.            */
/* Input:  VisaInfo: received message                                  */
/* nIndexCtx: Oracle context                                   */
/*************************************************************************/
int
	FCT_AUT_REQ_FROM_NW(int nIndexCtx, TSVisaInfo * VisaInfo)
{
	TSVisaInfo      NewVisaInfo;
	TSIsoInfo       IsoInfo;
	char            sTlvBuffer[LG_MAX];
	char            ProcessingCode[6 + 1];
	int             Length, result;
/*	char			sLocAcquirerBank[6 + 1];
	char			sBankCode[6 + 1];
	char			szAcquirerId[24];
	char			szCardAccId[24];
	char			szTermId[24];
	char			sPrivData[24];*/
	int				nResult;
	int				nLength;


	trace_event("Start  FCT_AUT_REQ_FROM_NW()", PROCESSING);
	/*memset(sLocAcquirerBank,'\0',sizeof(sLocAcquirerBank));
	memset(sBankCode,'\0',sizeof(sBankCode));
	memset(szAcquirerId,'\0',sizeof(szAcquirerId));
	memset(szCardAccId,'\0',sizeof(szCardAccId));
	memset(szTermId,'\0',sizeof(szTermId));
	memset(sPrivData,'\0',sizeof(sPrivData));*/

	result = BuildAutReqFromNw ( nIndexCtx , VisaInfo, &IsoInfo);
	if(result != OK)
	{
		AutReplyToNw (nIndexCtx, &IsoInfo, VisaInfo);
		trace_event ("End  FCT_AUT_REQ_FROM_NW(NOK)", PROCESSING);
		return (NOK);
	}
    /*PLUTONL-3762*/
	/*IsoInfo.msgId = MsgIsoReqReceived(ResourceId,&IsoInfo);*/
	/*trace_message(PROCESSING, "[SWMON1]: IsoInfo.msgId (%lu)",IsoInfo.msgId );*/
	IsoInfo.msgId = VisaInfo->msgId;
	 /*PLUTONL-3762*/

/*Start ICH20170809 (PROD00045701) Move call to load issuer data in order to get ACQ bank by bank
	GetVisaField(VISA_ACQR_ID, VisaInfo, szAcquirerId, &nLength);
	GetVisaField(VISA_OUTLET_NBR, VisaInfo, szCardAccId, &nLength);
	GetVisaField(VISA_TERMINAL_NBR, VisaInfo, szTermId, &nLength);


	nResult = get_mer_acq_bank(nIndexCtx, sBankCode ,NET_VISA,szAcquirerId,szCardAccId,szTermId,sPrivData,sLocAcquirerBank);
	if( nResult != OK )
	{
		memcpy(sLocAcquirerBank,AcquirerBank,6);
	}

	trace_message(PROCESSING, ">>>>> (%s)",sLocAcquirerBank );
End ICH20170809*/
	/* Call the pre-control module */
	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	GetIsoField(ISO_PROC_CODE, &IsoInfo, ProcessingCode, &Length);

	result = ServiceProcessing(		nIndexCtx,
		ResourceId,
		"ADM",
		AcquirerBank,
		/*sLocAcquirerBank,ICH20170809*/
		"Y",
		"G",
		"N",
		sTlvBuffer,
		&IsoInfo);

	/*Start IBO20200517 RAWMSG*/
	if (g_sCaptureMsg[NW_INC_REQ] == 'Y')
	{
		LogVisaMessage(nIndexCtx, VisaInfo, &IsoInfo, INC_REQ);
	}
	/*End IBO20200517 RAWMSG*/

	trace_message(PROCESSING, "[SWMON2]: IsoInfo.msgId (%lu)", IsoInfo.msgId );


	switch (result)
	{
	case ROUTAGE:
		/* forward request for authorization */
		if (ForwardAutRequest(nIndexCtx, &IsoInfo, sTlvBuffer, VisaInfo) != OK)
		{
			/* respond to VISA that issuer was not available */
			PutIsoField(ISO_RESPONSE_CODE, &IsoInfo, ISSUER_UNAVAILABLE_ACTION, 3);
			AutReplyToNw(nIndexCtx, &IsoInfo, VisaInfo);
			trace_event("End  FCT_AUT_REQ_FROM_NW(OK)", PROCESSING);
			return (OK);
		}
		else
		{
			/* request was successfully passed on to issuer interface */
			trace_event("End  FCT_AUT_REQ_FROM_NW(OK)", PROCESSING);
			return (OK);
		}
		break;

	case MALFUNCTION:
	case NOK:		/* the interface itself replies to the network */
	case OK:		/* the interface itself replies to the network */

		trace_message(PROCESSING, "[SWMON3]: IsoInfo.msgId (%lu)", IsoInfo.msgId );/*YDA20180103*/
		if (AutReplyToNw(nIndexCtx, &IsoInfo, VisaInfo) != OK) {
			trace_event("End  FCT_AUT_REQ_FROM_NW(NOK)", PROCESSING);
			return (NOK);
		}
		break;

	case SEVERE_ERROR:	/* shutdown interface */
		AutReplyToNw(nIndexCtx, &IsoInfo, VisaInfo);
		/*shutdown_resource(nIndexCtx, ResourceId);*//*PLUTONL-4242 ZKO 27012022*/
		trace_event("End  FCT_AUT_REQ_FROM_NW(SEVERE_ERROR)", PROCESSING);
		return (NOK);
		break;
	}

	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	InitIsoInfo(&IsoInfo);
	trace_event("End  FCT_AUT_REQ_FROM_NW(NOK)", PROCESSING);
	return (OK);

}
/*************************************************************************/
/* FCT_AUT_REQ_TO_NW()                                                 */
/* */
/* Sends an authorization request message to BASEI                     */
/*************************************************************************/

int
	FCT_AUT_REQ_TO_NW(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	TSVisaInfo      VisaInfo;
	TSNodeData      Node;
	int             result;
	int             retour;
	char            TrxType[3];
	char            tmpField[256];
	char            sBuffer[1024];
	int             tmpFieldLen;
	int             Length;
	char            sPurgeTime[10];
	char            sLine[MAX_LINE_TRC];
    /*20200301 PLUTON-779*/
	char            sPurgeTimeMS[4];
    struct timeval	sCreationTime;
	int				nPurgeTimeMS;
	long 			nSec;
	int 			nUsec;
	/*20200301 PLUTON-779*/
	char 			sSecurityData [128 + 1];

	trace_event("Start  FCT_AUT_REQ_TO_NW()", PROCESSING);

	dump_p7_struct(IsoInfo);

	MsgFwdReqRetrieved(IsoInfo->msgId);		/*EBE130925:SWMON*/

	memset(sBuffer, 0, sizeof(sBuffer));
	memset (sSecurityData, 0, sizeof(sSecurityData));

	/* Insert original ISO message in list for later retrieval */
	InitNodeData(&Node);
	GetIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, &tmpFieldLen);
	GetSecurityData(PURGE_TIME, sSecurityData, sPurgeTime, &tmpFieldLen);


    /* Node.tPurgeTime = atol(sPurgeTime);*/
	/*20200301 PLUTON-779*/
	nSec = atol(sPurgeTime);

	GetSecurityData(PURGE_TIME_MS, sSecurityData, sPurgeTimeMS, &Length);
	nPurgeTimeMS = atoi(sPurgeTimeMS);
	nUsec = nPurgeTimeMS* 1000;
	Node.tPurgeTime = nSec;
	Node.stPurgeTime.tv_sec = nSec;
	Node.stPurgeTime.tv_usec = nUsec;
	/*20200301 PLUTON-779*/
        if(BuildAutReqToNw(nIndexCtx, IsoInfo, &VisaInfo))
        {
                trace_event("FCT_AUT_REQ_TO_NW(): Convertion failed", PROCESSING);
                return (NOK);
        }


 	/*
	YDA20170913 : insert in Node with external STAN, supposedly filled from BuildAutReqToNw.
	and not with internal STAN. see PROD00045544
	GetIsoField(ISO_AUDIT_NBR, IsoInfo, tmpField, &tmpFieldLen);
	*/
	GetVisaField (VISA_AUDIT_NBR, &VisaInfo, tmpField, &tmpFieldLen);

	memcpy(Node.szFstKey, "1100", 4);
	memcpy(Node.szFstKey + 4, tmpField, 6);

	GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen);
	memcpy(Node.szFstKey + 4 + 6, tmpField, tmpFieldLen);

	/*AMER 20160727 Start: PADSS Fix*/
	if(debug_option == 1)
	{
		sprintf(sLine, "==> InsertKey = [%.32s] ", Node.szFstKey);
		trace_event(sLine, PROCESSING);
	}
	/*AMER 20160727 End: PADSS Fix*/
	/*
	if(BuildAutReqToNw(nIndexCtx, IsoInfo, &VisaInfo))
	{
		trace_event("FCT_AUT_REQ_TO_NW(): Convertion failed", PROCESSING);
		return (NOK);
	}
	*/
	Length = IsoBuildMsg(Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	Node.msgId = IsoInfo->msgId;				/*EBE130925:SWMON*/

	retour = InsertRequest('B', &Node);
	if (retour == EEXIST)
		VisaInfo.nMsgType = 101;
	else if (retour != OK)
	{
		trace_event("FCT_AUT_REQ_TO_NW(): InsertRequest() failed", PROCESSING);
		return (NOK);
	}
	/* send request to network */
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

	dump_buffer(sBuffer, Length, 'A', 'L', 'O');

	DumpVisa(sBuffer);

	if (WriteLineMsg(sBuffer, Length))
	{
		trace_event("FCT_AUT_REQ_TO_NW(): WriteLineMsg() failed", PROCESSING);
		FetchRequest('B', &Node);
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}

	MsgFwdReqSent(IsoInfo->msgId);						/*EBE130925:SWMON*/

	memset(sBuffer, 0, sizeof(sBuffer));
	memset(tmpField, 0, sizeof(tmpField));
	
	InitVisaInfo(&VisaInfo);
	InitNodeData(&Node);
	
	
	trace_event("End  FCT_AUT_REQ_TO_NW(OK)", PROCESSING);

	return (OK);
}


/****************************************************************************/
/* Forwards an authorization request to the issuer interface.              */
/****************************************************************************/
int 
	ForwardAutRequest(int nIndexCtx, TSIsoInfo * IsoInfo, char *sTlvBuffer, TSVisaInfo * VisaInfo)
{
	TSNodeData      Node;
	TSBalExchange   BalMessage;
	
	char            tmpField[256];
	char			DestNodeId[NODE_ID_LEN + 1];
	char			DestResourceId[RESOURCE_ID_LEN + 1];
	char            DestPid[PROC_PID_LEN + 1];
	char			BridgeResourceId[RESOURCE_ID_LEN + 1];
	char            sBuffer[1024];
	int             tmpFieldLen;
	int             Length;
	char            sPurgeTime[9 + 1];
	char            sPurgeTimeMS[4];
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
	int 			nBal = nBalMsg;
	struct timeval	sAcqPurgeTimeVal;
	struct timeval	sIssPurgeTimeVal;

	trace_event("Start  ForwardAutRequest()", PROCESSING);

	memset(DestResourceId, 0, sizeof(DestResourceId));


	/* Build node to insert request into list */
	InitNodeData(&Node);


	GetIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, &tmpFieldLen);
	GetSecurityData(DESTINATION_NODE_ID, sSecurityData, DestNodeId, &tmpFieldLen);
	GetSecurityData(DESTINATION_RESOURCE, sSecurityData, DestResourceId, &tmpFieldLen);
	GetSecurityData(DESTINATION_PID, sSecurityData, DestPid, &tmpFieldLen);
	nDestPid = atol(DestPid);

	nRetCode = GetTimers(nIndexCtx,
		sTlvBuffer,
		ResourceId,
		DestResourceId,
		&nAcqTime,
		&nIssTime,
		&sAcqPurgeTime,
		&sIssPurgeTime
		);
	if (nRetCode != OK) 
	{
		/*GetAcquirerPurgeTime(&sAcqPurgeTime);
		GetIssuerPurgeTime(&sIssPurgeTime);
		nAcqTime = nTimerAcq;*/
		
		/*GetAcquirerPurgeTimeMS(&sAcqPurgeTimeVal);
		GetIssuerPurgeTimeMS(&sIssPurgeTimeVal);*/
		GetCaptureIntPurgeTime(&VisaInfo->kCtime, &sAcqPurgeTimeVal);
		GetRoutingIntPurgeTime(&VisaInfo->kCtime, &sIssPurgeTimeVal);
		
	}

	/*sAcqPurgeTime -= 1;
	sIssPurgeTime -= 1;

	Node.tPurgeTime = sAcqPurgeTime;*/
	/*PLUTONL-4411: ZKO10032022
	sAcqPurgeTimeVal.tv_sec -= 1;
	sIssPurgeTimeVal.tv_sec -= 1;*/

	Node.tPurgeTime = sAcqPurgeTimeVal.tv_sec;
	Node.stPurgeTime.tv_sec = sAcqPurgeTimeVal.tv_sec;
	Node.stPurgeTime.tv_usec = sAcqPurgeTimeVal.tv_usec;

	GetVisaField(VISA_AUDIT_NBR, VisaInfo, tmpField, &tmpFieldLen);
	sprintf(Node.szFstKey, "%04d", VisaInfo->nMsgType);
	memcpy(Node.szFstKey + 4, tmpField, 6);

	memset(tmpField, 0, sizeof(tmpField));
	tmpFieldLen = 0;

	GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen);
	memcpy(Node.szFstKey + 4 + 6, tmpField, tmpFieldLen);


	memcpy(Node.sFstBuf, (char *) &(VisaInfo->sHeader), VISA_HDR_LEN);
	memset(sBuffer, 0, sizeof(sBuffer));
	Length = VisaBuildMsg(sBuffer, VisaInfo);
	memcpy(Node.sFstBuf + VISA_HDR_LEN, sBuffer, Length);

	Node.nFstBufLen = Length + VISA_HDR_LEN;	


	memcpy(Node.sSndBuf, sTlvBuffer, strlen(sTlvBuffer));
	Node.nSndBufLen = strlen(sTlvBuffer);

	sprintf(sLine, "==> Insertkey = [%.32s] ", Node.szFstKey);
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
		trace_event("ForwardAutRequest(): InsertRequest() failed", PROCESSING);
		return (NOK);
	}

	/* Add issuer purge time to Security Data Field (53) */
	/*sprintf(sPurgeTime, "%.*lu",PURGE_TIME_LEN, sIssPurgeTime);*/
	sprintf(sPurgeTime, "%.*lu",PURGE_TIME_LEN, sIssPurgeTimeVal.tv_sec);
	PutSecurityData(PURGE_TIME, sSecurityData, sPurgeTime, PURGE_TIME_LEN);

	sprintf(sPurgeTimeMS, "%03ld", (sIssPurgeTimeVal.tv_usec)/1000);
	PutSecurityData(PURGE_TIME_MS, sSecurityData, sPurgeTimeMS, PURGE_TIME_MS_LEN);

	PutIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, strlen(sSecurityData));
	dump_p7_struct(IsoInfo);


	BalMessage.sHeader.nPartnerId = nDestPid;
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1100;
	/*SKARROUMI PLUTONL-4900*/
	if (nIsExternal == OK)
	{
		BalMessage.sHeader.nPartnerId = getuid();
		nBal = nBalBrr;
	}
	/*SKARROUMI PLUTONL-4900*/
	Length = IsoBuildMsg(BalMessage.sBody, IsoInfo);
	BalMessage.sHeader.nLen = Length;
	BalMessage.sHeader.msgId = IsoInfo->msgId;

	if (WriteBalMsg(nBal, &BalMessage) != OK)
	{
		trace_event("ForwardAutRequest(): WriteBalMsg() failed", PROCESSING);
		/* free request from list */
		FetchRequest('L', &Node);
		IntWriteBalExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-4061 */
		return (NOK);
	}

	MsgReqForwarded(IsoInfo->msgId,DestResourceId,BalMessage.sHeader.nPartnerId);

	trace_event("End  ForwardAutRequest(OK)", PROCESSING);
	return (OK);
}

/*************************************************************************/
/* BuildAutReqToNw()                                                   */
/* */
/* Prepares an authorization request message (0100) to be sent         */
/* to BASEI.                                                           */
/* Input:  ISO format message                                          */
/* Output: VISA format message                                         */
/*************************************************************************/
int
	BuildAutReqToNw(int nIndexCtx,
	TSIsoInfo * IsoInfo,
	TSVisaInfo * VisaInfo)
{
	TSTlvPrivate    kTlvInfo;
	TSTlvPrivate    kNwTlvData;
	TSTlvPrivate    TlvNetPOSData;
	TSCpsInfo       CpsInfo;
	TlvInfo 		kIsoPayFacTlv;
	TlvInfo			kVisaPayFacTlv;
	TlvInfo			kVisaTransSpecTlv;
	TlvInfo			kVisaElectComTlv;   		/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	TlvInfo			kVisaSuppInfTlv;   			/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	TlvInfo			kVisaTlvInfo;   			/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17 and 9.1.4*/
	TlvInfo 		kVisaAccEnvAddDataInfo;		/*AEA20210712 Enh21.Q4 Art9.1.2*/
	TlvInfo			kVisaRecurringPayTlv;	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	TlvInfo			kIsoRecurringPayTlv;	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	TlvInfo			kVisaEcomAddData;		/*ELKAM20230210 PLUTONL-5830*/
	char            tmpField[BUF_LG_MAX];	/*AEA20210824 PLUTONL-3379*/
	char            tmpField1[256];
	char            isoProcCode[7];
	char            sLine[MAX_LINE_TRC];
	int             tmpFieldLen;
	char            Pan[20];
	char            ExpiryDate[5];
	char            MerchantType[5];
	char            isoPosMode[13];
	char            CVV[4];
	char            PVV[6];
	char            ServiceCode[4];
	char            T1_Data[77];
	char            T2_Data[38];
	char            sRespCode[3 + 1];
	char            sVisaRespCode[2 + 1];
	char            PinBlock[16 + 1];
	char            format_in[2 + 1];
	char            Transkey [256 + 1];
	char            ZPK[256 + 1];
	char            security_format[2 + 1];
	char            sChCert[128];
	char            sMerCert[128];
	char            sTransId[128];
	char            sTransStain[128];
	char            sRefNbr[12 + 1];
	char            szWork[256 + 1];
	char            szDate[12 + 1];
	char            sData[12 + 1];
	char            sStan[6 + 1];
	char            translation_mode;
	char 			OutletNumber[15 +1 ];
	char 			AquirerBin[12 +1 ];
	char            PosMode[7];
	char            PosCond[3];
	char 			AddPos[24+1];/*IBO20220816 PLUTONL-5085*/
	int             field_61_presence = 0;
	int             AddPosLen;
	int             nRetCode;
	int             vpuf_presence = 0;
	TSVisaPufInfo   VisapufInfo;
	char            sCvc2Data[12];
	char            sVisaPufData[512];
	char            sMcc[12];
	int             FieldLen, i;
	char            Priv_Use_Field[255 + 1];
	char            vBitMap[3 +1];
	char       	    v_pos_condition[2 + 1];
	char			szAcquirerBank[6 + 1];
	int			    tlvPrivateAnalysed=0;/*YDA20170913*/
	char            isoFunctionCode[4];
	char			isoTrxAmnt[13];
	int				acc_verification_only = 0;		/*Account verification*/
	char			RecurringFlag[2];
	TSTagIso		IsoIccInfo;
	TSTagVisa		VisaIccInfo;
	char            sIccBuffer[1024];
	char         	FieldData[256];
	char            sAddAuthInfo[P7_F048_P64_LEN + 1];
	char 			sPosExtData[P7_F048_P61_LEN +1];
	char 			sAuthExcem[1+1];
	TSPufInfo		PufInfo;
	int				tagPresence = 0;			/*HAL PROD00073823 20202801 Enh Q2.2020*/
	char 			sEcomAuthInfo[1+1];
	TlvInfo			kTlvData;
	TlvInfo			kVisaTlvInfoSuppDta;
	BerInfo			kVisaBerInfoSuppDta; 	 /*IBE PLUTONL-5640*/
	char            sPufData[1 + 1];		/*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	char			sRecuringFlag[2];	/*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	char			sAuthAddInfo[P7_F048_P64_LEN+1];	/*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	int				nNbrTags = 0;		/*ZKO 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	trace_event("Start  BuildAutReqToNw()", PROCESSING);

	memset (v_pos_condition      , 0, sizeof(v_pos_condition));
	memset(PinBlock, 0, sizeof(PinBlock));
	memset(format_in, 0, sizeof(format_in));
	memset(Transkey, 0, sizeof(Transkey));
	memset(ZPK, 0, sizeof(ZPK));
	memset(Pan, 0, sizeof(Pan));
	memset(isoPosMode, 0, sizeof(isoPosMode));
	memset(isoProcCode, 0, sizeof(isoProcCode));
	memset(MerchantType, 0, sizeof(MerchantType));
	memset(security_format, 0, sizeof(security_format));
	memset(PosMode, 0, sizeof(PosMode));
	memset(PosCond, 0, sizeof(PosCond));
	memset(AddPos, 0, sizeof(AddPos));
	memset(szAcquirerBank, 0, sizeof(szAcquirerBank));
	memset(OutletNumber, 0, sizeof(OutletNumber));/*NZE21042016*/
	memset(isoFunctionCode, 0, sizeof(isoFunctionCode));
	memset(isoTrxAmnt, 0, sizeof(isoTrxAmnt));
	memset(RecurringFlag, 0, sizeof(RecurringFlag));
	memset(FieldData, 0, sizeof(FieldData));
	memset(sAddAuthInfo, 0, sizeof(sAddAuthInfo));
	memset(sPosExtData, '\0', sizeof(sPosExtData));
	memset(sAuthExcem, '\0', sizeof(sAuthExcem));
	memset(sEcomAuthInfo, '\0', sizeof(sEcomAuthInfo));
	memset(sPufData, 0, sizeof(sPufData));		/*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	memset(sRecuringFlag, 0, sizeof(sRecuringFlag));		/*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	memset(sAuthAddInfo, 0, sizeof(sAuthAddInfo));		/*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/

	/* Initialize Visa structure */
	InitVisaInfo(VisaInfo);
	InitTlvPrivate(&kTlvInfo);
	InitTlvPrivate(&TlvNetPOSData);
	IniIsoPayFacDataTlv(&kIsoPayFacTlv);
	InitVisaTransSpecDataTlv(&kVisaTransSpecTlv);
	InitVisaPayFacDataTlv(&kVisaPayFacTlv);
	InitVisaElectComDataTlv(&kVisaElectComTlv);			/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	InitVisaSupportInfDataTlv(&kVisaSuppInfTlv);		/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	InitVisaRecurringPayDataTlv(&kVisaRecurringPayTlv);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	IniIsoInstallmentLoanTlv(&kIsoRecurringPayTlv);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		tlvPrivateAnalysed=1;
		AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);
	}

	/* Message type is 0100 */
	if (IsoInfo->nMsgType == 1200)
		VisaInfo->nMsgType = 100;
	else
		VisaInfo->nMsgType = IsoInfo->nMsgType - 1000;


	/* (2) Primary Account Number */
	if (GetIsoField(ISO_CARD_NBR, IsoInfo, Pan, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_CARD_NBR, VisaInfo, Pan, tmpFieldLen);
	else 
	{
		if (GetIsoField(ISO_T2_DATA, IsoInfo, T2_Data, &tmpFieldLen) == SUCCESS) 
		{
			GetTrack2Data(T2_Data, Pan, ExpiryDate, ServiceCode, PVV, CVV);
			AddVisaField(VISA_CARD_NBR, VisaInfo, Pan, strlen(Pan));
		}
	}

	/* (3) Processing Code */
	memset(sMcc, 0, sizeof(sMcc));
	memcpy(sMcc, "0000", 4);
	GetIsoField(ISO_PROC_CODE, IsoInfo, isoProcCode, &tmpFieldLen);
	GetIsoField(ISO_MERCHANT_TYPE, IsoInfo, sMcc, &tmpFieldLen);
	IsoToVisaProcessingCode(isoProcCode, tmpField, sMcc);
	AddVisaField(VISA_PROC_CODE, VisaInfo, tmpField, 6);

	/* (4): Transaction Amount */
	if (GetIsoField(ISO_TRANS_AMOUNT, IsoInfo, isoTrxAmnt /*tmpField*/, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_TRANS_AMOUNT, VisaInfo, isoTrxAmnt/*tmpField*/, 12);

	/* (7) Transaction Date/Time */
	GetIsoField(ISO_XMIT_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddVisaField(VISA_XMIT_TIME, VisaInfo, tmpField, 10);

	memset(sData, 0, sizeof(sData));
	current_gmt_date(sData);



	/* (11) Trace Audit Number */
	/*YDA20180115: fill VISA audit number with the external stan we received from ISO (see PROD00045544)*/
	/*Fill internal stan if external stan is not retrieved*/
	if (tlvPrivateAnalysed == 1 && GetTlvPrivate( EXTERNAL_STAN_TAG , &kTlvInfo,sStan, &tmpFieldLen) == SUCCES)
	{
		AddVisaField (VISA_AUDIT_NBR, VisaInfo, sStan, 6);
	}
	else if (GetIsoField (ISO_AUDIT_NBR, IsoInfo, sStan, &tmpFieldLen) == SUCCESS)
	{
		AddVisaField (VISA_AUDIT_NBR, VisaInfo, sStan, 6);
	}

	GetIsoField(ISO_MERCHANT_TYPE, IsoInfo, MerchantType, &tmpFieldLen);
	
	/*AMER20201026 PLUTONL-2425*/
	/*if (!memcmp(MerchantType, "6011", 4)) 
	{*//*PLUTONL-3285*/
	if(GetIsoField(ISO_TRANS_DATE_TIME, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AddVisaField (VISA_TRANS_TIME, VisaInfo, tmpField+6, 6);
		AddVisaField (VISA_TRANS_DATE, VisaInfo, tmpField+2, 4);
	}		
	/*}*/
	
	/* (14) Expiry Date */
	if (GetIsoField(ISO_EXPIRY_DATE, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_EXPIRY_DATE, VisaInfo, tmpField, 4);
	else if (GetIsoField(ISO_T2_DATA, IsoInfo, T2_Data, &tmpFieldLen) == SUCCESS) {
		GetTrack2Data(T2_Data, Pan, ExpiryDate, ServiceCode, PVV, CVV);
		AddVisaField(VISA_EXPIRY_DATE, VisaInfo, ExpiryDate, 4);
	}
	/* (18) Merchant Type */
	
	AddVisaField(VISA_MERCHANT_TYPE, VisaInfo, MerchantType, 4);

	/* (19) Acquiring Institution Country Code */
	GetIsoField(ISO_ACQR_COUNTRY, IsoInfo, tmpField, &tmpFieldLen);
	AddVisaField(VISA_ACQR_COUNTRY, VisaInfo, tmpField, 3);

	/* (20): PAN Country Code */
	if (GetIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_PAN_COUNTRY, VisaInfo, tmpField, 3);

	/* (22) POS Entry Mode */
	if (GetIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		GetSecurityData(SOURCE_RESOURCE, tmpField, tmpField1, &tmpFieldLen);


	
	/*if (GetIsoField(ISO_POS_DATA, IsoInfo, isoPosMode, &tmpFieldLen) == SUCCESS && memcmp(isoPosMode, "1", 1))*/
	if (GetIsoField(ISO_POS_DATA, IsoInfo, isoPosMode, &tmpFieldLen) == SUCCESS )	/*AMER PROD00054205: We don't need to check if ISS F22.1 != '1' (Manual, no terminal)*/
	{
		IsoToVisaPosMode(isoPosMode, tmpField);
		/*IsoToVisaPosMode(isoPosMode, tmpField);*/
		/*AMER PROD00054205: The below hardcoded value removed because the mapping is done in IsoToVisaPosMode*/ 
		/*if (GetIsoField(ISO_CARD_SEQUENCE_NBR, IsoInfo, tmpField1, &tmpFieldLen) == SUCCESS &&		
			GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField1, &tmpFieldLen) != SUCCESS)
			memcpy(tmpField, "9010", 4);*/
		AddVisaField(VISA_POS_ENTRY_MODE, VisaInfo, tmpField, 4);
	}
	/* (23) CARD SEQUENCE NUMBER */
	if (GetIsoField(ISO_CARD_SEQUENCE_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_CARD_SEQUENCE_NBR, VisaInfo, tmpField, 3);

	
	/* (25) POS Condition Code */
	/*IsoToVisaPosCondition(isoPosMode, MerchantType, tmpField);*/
	
	/*Start AMER20180313 Enh18.Q2 PROD00054205*/
	GetIsoField(ISO_FUNCTION_CODE, IsoInfo, isoFunctionCode, &tmpFieldLen);
	if(memcmp(isoFunctionCode, P7_F024_ACC_VERIF, P7_F024_LEN) == 0 && atoi(isoTrxAmnt) == 0)
		acc_verification_only = 1;
	IsoToVisaPosCondition(isoPosMode, MerchantType, acc_verification_only, tmpField);
	/*End AMER20180313 Enh18.Q2 PROD00054205*/
	AddVisaField(VISA_POS_CONDITION, VisaInfo, tmpField, 2);


	/**if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{

	AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);**/
	/*AMER PROD00054205: The below code should be commented because there is no relation between CAVV and Account verification*/
	/*if (GetTlvPrivate(ECOM_AAV_TAG, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		if (GetIsoField(ISO_TRANS_AMOUNT, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
		{
			if (memcmp(tmpField,"000000000000",12) == 0)
			{

				memcpy(v_pos_condition,B1_F025_ACC_VERIF,B1_F025_LEN); 
				AddVisaField(VISA_POS_CONDITION, VisaInfo, v_pos_condition, 2 ); 
			}
		}
	}
	else 
		trace_event("Fail getting P72...",PROCESSING);*/
	/*}*/

	/* (26) Pos Pin Capture Code     (n 2) */
	if (GetIsoField(ISO_PIN_DATA, IsoInfo, tmpField, &FieldLen) == SUCCES)
		AddVisaField(VISA_POS_PIN_CAPTURE, VisaInfo, "12", 2);

	/* (32) Acquiring Institution Id Code */
	/*EBE
	if (GetIsoField (ISO_OUTLET_NBR, IsoInfo, OutletNumber, &tmpFieldLen) == SUCCESS)
	{   

	nRetCode = GetVisaAquirer( nIndexCtx, "01", OutletNumber , AquirerBin)  ;

	if (nRetCode != OK)
	{
	trace_event("End BuildAutReqToNw (NOK) : ERROR Getting AquirerBin",TRACE);
	return(NOK);
	}
	else
	{
	AddVisaField (VISA_ACQR_ID, VisaInfo, AquirerBin , strlen(AquirerBin));			
	}
	}*/
	if (GetTlvPrivate(ACQUIRER_BANK_TAG, &kTlvInfo, szAcquirerBank , &tmpFieldLen) != SUCCESS)
	{
		trace_event("End BuildAutReqToNw (NOK) : ERROR Getting Acquirer bank",ERROR);
		return(NOK);
	}
	nRetCode = GetAquirerBin( nIndexCtx, szAcquirerBank, NET_VISA, AquirerBin)  ;
	if (nRetCode != OK)
	{
		sprintf(sLine,"End BuildAutReqToNw (NOK) : ERROR Getting AquirerBin for bank[%s]",szAcquirerBank);
		trace_event(sLine,ERROR);
		return(NOK);
	}
	AddVisaField (VISA_ACQR_ID, VisaInfo, AquirerBin , strlen(AquirerBin));	
	PutIsoField (ISO_ACQR_ID, IsoInfo, AquirerBin, strlen(AquirerBin));	/* AEA17082021 PLUTONL-3337 */

	/* (33) Forwarding Institution Id */

	/* (34) PAN Extended */
	/*AMER PROD00054205: The below code must be commented because there is no F34 in the specs*/
	/*if (GetIsoField(ISO_PAN_EXTENDED, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_PAN_EXTENDED, VisaInfo, tmpField, tmpFieldLen);*/


	InitVisaAccEnvAddDataTlv(&kVisaAccEnvAddDataInfo);	/*AEA20210712 Enh21.Q4 Art9.1.2*/
	/*Start IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	if (GetTlvPrivate(AUTH_ADD_INFO, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		if (tmpField[P7_F048_P64_INITIATING_PARTY] == P7_F048_P64_P4_MER_INIT) 
		{
			sAuthExcem[0]=tmpField[P7_F048_P64_INITIATING_PARTY];
			AddTlvInfoField(B1_F34T02_INITIATING_PARTY_ID, &kVisaAccEnvAddDataInfo, sAuthExcem, 1);	/*AEA20210712 Enh21.Q4 Art9.1.2*/
		}
	}
	/*End IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/

	/*Start ZKO16082019 PROD00070403 Enh19.Q4 Art 9.1.2*/
	InitVisaEuPsd2ScaCsc(&kVisaTlvInfo);
     if (GetTlvPrivate( ECOM_AUTH_INFO, &kTlvInfo, tmpField, &tmpFieldLen) == SUCCESS)
     {
		if(tmpField[P7_F048_P66_DELG_AUTH_OFST] == '1')
		{
			sEcomAuthInfo[0]=tmpField[P7_F048_P66_DELG_AUTH_OFST];
		    AddTlvInfoField(B1_F34T4A_DELG_AUTH, &kVisaTlvInfo, sEcomAuthInfo, 1);
			
		}
		/* ELKAM20230210 PLUTONL-5830 START*/
		if(tmpField[P7_F048_P66_AUTHEN_FRMWRK_OFST] == '1')
		{
			InitVisaEcomAddDataTlv(&kVisaEcomAddData);
			AddTlvInfoField (B1_F34T01_AUTHENTICATION_PROGRAM , &kVisaEcomAddData, VISA_DAF, 2);
			tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kVisaTlvInfo);
			if( tmpFieldLen > 0 )
				AddTlvInfoField(B1_F34_AUTHEN_DATA, &kVisaElectComTlv, tmpField, tmpFieldLen);
		}
		/* ELKAM20230210 PLUTONL-5830 END*/
     }
	/*End ZKO16082019 PROD00070403 Enh19.Q4 Art 9.1.2*/
	
	/*Start HAL 20201903 Enh20.Q2 Art 9.3.3*/
    if (GetTlvPrivate( PSD2_DATA, &kTlvInfo, tmpField, &tmpFieldLen) == SUCCESS)
    {
		InitExempAuthInfoDataTlv(&kTlvData);
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kTlvData);
        
		/*Start AEA20210712 Enh21.Q4 Art9.3.3*/
		/*if( GetTlvInfoField (P7_F048_S27_REASON_HONOR_EXEM, &kTlvData, tmpField, &tmpFieldLen) == SUCCESS )
        {
			AddTlvInfoField(B1_F34T4A_REASON_HONOR_EXEM, &kVisaTlvInfo, tmpField, tmpFieldLen);	 20202003 Enh20.Q2 Art 9.3.3
		}*/
		/*End AEA20210712 Enh21.Q4 Art9.3.3*/
		
		if( GetTlvInfoField (P7_F048_S27_AUTH_EXEMPTION_INDIC, &kTlvData, tmpField, &tmpFieldLen) == SUCCESS )
		{
			if(tmpField[P7_F048_S27_001_TRST_MER_EXEM] == '1')
			{
				sAuthExcem[0]=tmpField[P7_F048_S27_001_TRST_MER_EXEM];
				AddTlvInfoField(B1_F34T4A_TRST_MER_EXEM, &kVisaTlvInfo, sAuthExcem, 1);	/* 20202003 Enh20.Q2 Art 9.3.3*/
			}	
				
			if(tmpField[P7_F048_S27_001_TRA] == '1')
			{
				sAuthExcem[0]=tmpField[P7_F048_S27_001_TRA];
				AddTlvInfoField(B1_F34T4A_TRA, &kVisaTlvInfo, sAuthExcem, 1);		/* 20202003 Enh20.Q2 Art 9.3.4*/
			}

			/*Start AEA20210712 Enh21.Q4 Art9.1.2*/
			if(tmpField[P7_F048_S27_001_AUT_OUTAGE_EXCEPTION] == '0' || tmpField[P7_F048_S27_001_AUT_OUTAGE_EXCEPTION] == '1')
			{
				sAuthExcem[0]=tmpField[P7_F048_S27_001_AUT_OUTAGE_EXCEPTION];
				AddTlvInfoField(B1_F34T02_AUTH_OUTAGE_INDICATOR, &kVisaAccEnvAddDataInfo, sAuthExcem, 1);
			}
			/*End AEA20210712 Enh21.Q4 Art9.1.2*/

			/*Start AEA20210712 Enh21.Q4 Art9.3.3*/
			if(tmpField[P7_F048_S27_001_DELG_AUTH] == '1')
			{
				sAuthExcem[0]=tmpField[P7_F048_S27_001_DELG_AUTH];
				AddTlvInfoField(B1_F34T4A_DELG_AUTH, &kVisaTlvInfo, sAuthExcem, 1);
			}
			/*End AEA20210712 Enh21.Q4 Art9.3.3*/
			
			/*ZKO29122021 Start PLUTONL-3345*/
			if(tmpField[P7_F048_S27_001_LOW_VAL_EXEM] == '1')
			{
				sAuthExcem[0]=tmpField[P7_F048_S27_001_LOW_VAL_EXEM];
				AddTlvInfoField(B1_F34T4A_LOW_VAL_EXEM, &kVisaTlvInfo, sAuthExcem, 1);
			}
			if(tmpField[P7_F048_S27_001_SCP] == '1')
			{
				sAuthExcem[0]=tmpField[P7_F048_S27_001_SCP];
				AddTlvInfoField(B1_F34T4A_SPC, &kVisaTlvInfo, sAuthExcem, 1);
			}
			/*ZKO29122021 End PLUTONL-3345*/

		}
    }
	/*End HAL 20201903 Enh20.Q2 Art 9.3.3*/
	
	/*ZKO29122021 Start PLUTONL-3345

	/*Start IBO20190719 PROD00070403 Enh19.Q4 Art 9.1.3*
	if (GetTlvPrivate(AUTH_EXEMPTION_IND, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		/**InitVisaEuPsd2ScaCsc(&kVisaTlvInfo);**
		/*if(tmpField[P7_F048_K09_TRST_MER_EXEM] == '1')
		{
			sAuthExcem[0]=tmpField[P7_F048_K09_TRST_MER_EXEM];
			AddTlvInfoField(B1_F34T4A_TRST_MER_EXEM, &kVisaTlvInfo, sAuthExcem, 1);
		}*
		if(tmpField[P7_F048_K09_LOW_VAL_EXEM] == '1')
		{
			sAuthExcem[0]=tmpField[P7_F048_K09_LOW_VAL_EXEM];
			AddTlvInfoField(B1_F34T4A_LOW_VAL_EXEM, &kVisaTlvInfo, sAuthExcem, 1);
		}
		if(tmpField[P7_F048_K09_SCP] == '1')
		{
			sAuthExcem[0]=tmpField[P7_F048_K09_SCP];
			AddTlvInfoField(B1_F34T4A_SPC, &kVisaTlvInfo, sAuthExcem, 1);
		}
		/*if(tmpField[P7_F048_K09_TRA] == '1')
		{
			sAuthExcem[0]=tmpField[P7_F048_K09_TRA];
			AddTlvInfoField(B1_F34T4A_TRA, &kVisaTlvInfo, sAuthExcem, 1);
		}*
	}
		ZKO29122021 End PLUTONL-3345
	*/
	InitVisaSupplementalData(&kVisaBerInfoSuppDta);	/*Start NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/

	if (GetTlvPrivate( ECOM_AUTHENTIF_DATA, &kTlvInfo, tmpField, &tmpFieldLen) == SUCCESS)
    {       
		/*InitVisaSupplementalData(&kVisaBerInfoSuppDta);*/
		InitEcomAuthentificationInfoDataTlv(&kTlvData);
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kTlvData);

		    if( GetTlvInfoField (P7_F048S26_ECOM_AUT_DATA_SESSION_ID, &kTlvData, tmpField, &tmpFieldLen) == SUCCESS )
		    {
			    AddTlvInfoField(B1_F34T56_SESSION_ID, &kVisaBerInfoSuppDta, tmpField, tmpFieldLen);	
		    }
    }

	/*Start NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
    if (GetTlvPrivate(DATA_3DS, &kTlvInfo, tmpField, &tmpFieldLen) == SUCCESS)
    {
		InitInfo3DSDataTlv(&kTlvData);
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kTlvData);

		if( GetTlvInfoField (P7_F048_S29_THIRD_PARTY_ID, &kTlvData, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddBerInfoField(B1_F34T56_THIRD_PARTY_ID, &kVisaBerInfoSuppDta, tmpField, tmpFieldLen);	
		}
		if( GetTlvInfoField (P7_F048_S29_THIRD_PARTY_RISK_SCORE, &kTlvData, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddBerInfoField(B1_F34T56_THIRD_PARTY_RISK_SCORE, &kVisaBerInfoSuppDta, tmpField, tmpFieldLen);	
		}
	}
	/*End NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
	
		tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kVisaTlvInfo);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(B1_F34_EU_PSD2_SCA_CSC, &kVisaElectComTlv, tmpField, tmpFieldLen);
	}

	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kVisaBerInfoSuppDta);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(B1_F34_SUPP_DATA, &kVisaElectComTlv, tmpField, tmpFieldLen);
	}  

	/*Start AEA20210712 Enh21.Q4 Art9.1.2*/
	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kVisaAccEnvAddDataInfo);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(B1_F34_ACCEPTANCE_ENV_ADD_DATA, &kVisaElectComTlv, tmpField, tmpFieldLen);
	}
	/*End AEA20210712 Enh21.Q4 Art9.1.2*/

	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kVisaElectComTlv);
	if (tmpFieldLen > 0)
		{
			AddVisaField(VISA_PAN_EXTENDED /*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/, VisaInfo, tmpField, tmpFieldLen);
		}
	
	/*End IBO20190719 PROD00070403 Enh19.Q4 Art 9.1.3*/

	/* (35) Track II Data */
	if (GetIsoField(ISO_T2_DATA, IsoInfo, T2_Data, &tmpFieldLen) == SUCCESS) 
	{
		replace(T2_Data, '=', 'D', tmpFieldLen);
		AddVisaField(VISA_T2_DATA, VisaInfo, T2_Data, tmpFieldLen);
	}


	/* (37) Reference Number */
	memset(sRefNbr, 0, sizeof(sRefNbr));
	GetIsoField(ISO_XMIT_TIME, IsoInfo, tmpField, &tmpFieldLen);
	memset(szDate, 0, sizeof(szDate));
	memset(szWork, 0, sizeof(szWork));
	current_date_yyyymmdd(szWork);
	memcpy(szDate, tmpField + 2, 2);
	memcpy(szDate + 2, tmpField, 2);
	memcpy(szDate + 4, szWork, 4);
	memset(szWork, 0, sizeof(szWork));
	nRetCode = convert_to_julian(szDate, szWork);
	sprintf(sLine, "Date %s,JulianDate : %s|%d|%s|%s|", szDate, szWork, nRetCode, tmpField, sStan);
	trace_event(sLine, PROCESSING);
	sRefNbr[0] = szDate[7];
	memcpy(sRefNbr + 1, szWork, 3);
	memcpy(sRefNbr + 4, tmpField + 4, 2);
	memcpy(sRefNbr + 6, sStan, 6);
	AddVisaField(VISA_REFERENCE_NBR, VisaInfo, sRefNbr, 12);
	AddTlvPrivate(NET_REFERENCE_NUMBER,&kTlvInfo,sRefNbr,12); /*HAL 20201303 ENHS ART 1.1*/

	sprintf(sLine, "==> 001. sRefNbr=[%s]", sRefNbr);
	trace_event(sLine, FATAL);

	/* (41) Card Acceptor Terminal Id */
	if (GetIsoField(ISO_TERMINAL_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_TERMINAL_NBR, VisaInfo, tmpField, 8);


	/* (42) Card Acceptor Id *//*NZE21042016 Activation de recuperation du champ 42*/
	if (GetIsoField (ISO_OUTLET_NBR, IsoInfo, OutletNumber, &tmpFieldLen) == SUCCESS) 
	{
		AddVisaField (VISA_OUTLET_NBR, VisaInfo, OutletNumber, 15);
	}

	/* (43) Card Acceptor Name/Location */
	if (GetIsoField(ISO_TERMINAL_ADR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		replace(tmpField, '>', ' ', tmpFieldLen);
		replace(tmpField, '_', ' ', tmpFieldLen);
		AddVisaField(VISA_TERMINAL_ADR, VisaInfo, tmpField, 40);
	}
	/* (45) TrackI Data */
	if (GetIsoField(ISO_T1_DATA, IsoInfo, T1_Data, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_T1_DATA, VisaInfo, T1_Data, tmpFieldLen);

	/* Field 48: Additional Private Data */
	/**if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
	InitTlvPrivate(&kTlvInfo);
	AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);*/
/*
	tmpField1[0]='#';
	if (GetTlvPrivate(ADDRESS_VERIF_REQ_TYPE_TAG, &kTlvInfo, tmpField1 + 1, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_ADTNL_DATA_PRIV, VisaInfo, tmpField1, tmpFieldLen); */
	/*}*/
	/* (49) Transaction Currency Code */
	GetIsoField(ISO_TRANS_CRNCY, IsoInfo, tmpField, &tmpFieldLen);
	AddVisaField(VISA_TRANS_CRNCY, VisaInfo, tmpField, 3);

	/* (52) PIN Data */
	if (GetIsoField(ISO_PIN_DATA, IsoInfo, PinBlock, &tmpFieldLen) == SUCCESS) 
	{
		/**InitTlvPrivate(&kTlvInfo);
		if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
		{
		AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);*/
		GetTlvPrivate(ENCR_PIN_BLOCK_KEY_TAG, &kTlvInfo, Transkey, &tmpFieldLen);
		/*}*/
		GetAWK(nIndexCtx, ResourceId, ZPK);

		GetIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen);
		GetSecurityData(SECURITY_FORMAT, tmpField, security_format, &tmpFieldLen);
		GetSecurityData(PIN_BLOCK_FORMAT, tmpField, format_in, &tmpFieldLen);

		if (memcmp(security_format, "01", 2) == 0)
			translation_mode = TERMINAL_MODE;
		else
			translation_mode = SCHEME_MODE;

		pin_translate(nIndexCtx, Pan, Transkey, ZPK, PinBlock, format_in, "01", translation_mode);
		AddVisaField(VISA_PIN_DATA, VisaInfo, PinBlock, 16);
	}

	/* (53) Security Data */
	if (GetIsoField(ISO_PIN_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		memset(tmpField, 0, sizeof(tmpField));
		memcpy(tmpField, "2001010100000000", 16);

		GetIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField1, &tmpFieldLen);
		IsoToVisaSecData(tmpField1, tmpField);
		AddVisaField(VISA_SECURITY_DATA, VisaInfo, tmpField, 16);
	}

	/* (55) Additional POS Info */
	if (GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		/*Start AMER20180307 PROD00054205: Enh18.Q2 Art 8.1.1*/
		InitIsoIcTag(&IsoIccInfo);
		AnalyseIsoIc(tmpField, tmpFieldLen , &IsoIccInfo);
		InitVisaIcTag(&VisaIccInfo);
		memset(FieldData, 0, sizeof(FieldData));

		if(GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &IsoIccInfo, FieldData, &FieldLen ) == SUCCES ) 
			AddVisaIcTag(VISA_TAG_APP_CRYPTOGRAM, &VisaIccInfo, FieldData,  FieldLen );

		if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag(VISA_TAG_ISS_APP_DATA , &VisaIccInfo, FieldData,  FieldLen );

		if (GetIsoIcTag (ISO_TAG_UNPRED_NUMBER , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_UNPRED_NUMBER , &VisaIccInfo, FieldData,  FieldLen );

		if (GetIsoIcTag (ISO_TAG_ATC , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_ATC , &VisaIccInfo, FieldData,  FieldLen );

		if (GetIsoIcTag (ISO_TAG_TVR , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TVR , &VisaIccInfo, FieldData,  FieldLen );

		if (GetIsoIcTag (ISO_TAG_TRANS_DATE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TRANS_DATE , &VisaIccInfo, FieldData,  FieldLen );

		if (GetIsoIcTag (ISO_TAG_TRANS_TYPE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TRANS_TYPE , &VisaIccInfo, FieldData,  FieldLen );

		if (GetIsoIcTag (ISO_TAG_TRANS_AMOUNT , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TRANS_AMOUNT , &VisaIccInfo, FieldData,  FieldLen);

		if (GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TRANS_CUR_CODE , &VisaIccInfo, FieldData,  FieldLen );

		if (GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_APP_INTER_PROFILE , &VisaIccInfo, FieldData,  FieldLen );

		if (GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES ) 
			AddVisaIcTag (VISA_TAG_TERM_COUNTRY_CODE , &VisaIccInfo,FieldData , FieldLen);

		if ( GetIsoIcTag (ISO_TAG_TERM_CAP , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TERM_CAP , &VisaIccInfo, FieldData,  FieldLen );

		if ( GetIsoIcTag (ISO_TAG_DED_FILE_NAME , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_DED_FILE_NAME , &VisaIccInfo, FieldData,  FieldLen );

		/* Not defined in Visa
		if ( GetIsoIcTag (ISO_TAG_TRANS_CATEG_CODE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TRANS_CATEG_CODE , &VisaIccInfo, FieldData,  FieldLen );


		if ( GetIsoIcTag (ISO_TAG_TERM_APP_VER_NUM , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_TERM_APP_VER_NUM , &VisaIccInfo, FieldData,  FieldLen );
		*/
		
		if (GetIsoField(ISO_ACQR_REFERENCE_DATA, IsoInfo, FieldData, &FieldLen) == SUCCESS)
		{
			if(GetTlvPrivate(ENCR_PIN_BLOCK_KEY_TAG, &kTlvInfo, Transkey, &FieldLen) != SUCCESS)
			{
				IsoInfo->nMsgType += 10;
				ReplyIsoMsgToBal(nBalMsg, IsoInfo, "", CRYPTOGRAPHIC_ERROR_ACTION);
				trace_event("End  BuildAutReqToNw TPK/ZPK Not Present(NOK)", PROCESSING);
				return (NOK);
			}
			
			if(GetAWK(nIndexCtx, ResourceId, ZPK)!= OK )
			{
				trace_event("GetAWK Failed ",PROCESSING);
				IsoInfo->nMsgType += 10;
				ReplyIsoMsgToBal(nBalMsg, IsoInfo, "", CRYPTOGRAPHIC_ERROR_ACTION);
				trace_event("End  BuildAutReqToNw ZPK EPS ERROR(NOK)", PROCESSING);
				return (NOK);
			}
			
			GetIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField, &FieldLen);
			GetSecurityData(SECURITY_FORMAT, tmpField, security_format, &FieldLen);
			GetSecurityData(PIN_BLOCK_FORMAT, tmpField, format_in, &FieldLen);

			if (memcmp(security_format, "01", 2) == 0)
				translation_mode = TERMINAL_MODE;
			else
				translation_mode = SCHEME_MODE;

			if(pin_translate(nIndexCtx, Pan, Transkey, ZPK, FieldData, format_in, "01", translation_mode)!= OK)
			{
				trace_event ("secondary pin translation Failed", PROCESSING); 
				IsoInfo->nMsgType += 10;
				ReplyIsoMsgToBal (nBalMsg, IsoInfo, 0, CRYPTOGRAPHIC_ERROR_ACTION);
				trace_event("End  BuildAutReqToNw Translation EPS Error(NOK)", PROCESSING);
				return (NOK); 
			}
			
			AddVisaIcTag (VISA_TAG_SEC_PIN_BLOCK , &VisaIccInfo, FieldData,  FieldLen);
		}
			
		/*Start AMER20180307 PROD00054205: Enh18.Q2 Art 3.6*/
		if ( GetIsoIcTag (ISO_TAG_FFI , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddVisaIcTag (VISA_TAG_FFI , &VisaIccInfo, FieldData,  FieldLen );
		/*End AMER20180307 PROD00054205: Enh18.Q2 Art 3.6*/
		
		FieldLen = VisaBuildIcFld (sIccBuffer, &VisaIccInfo);
		AddVisaField(VISA_ICC_DATA, VisaInfo, sIccBuffer , FieldLen);
		/*End AMER20180307 PROD00054205: Enh18.Q2 Art 2.4*/
		
		/*memset(tmpField1, 0, sizeof(tmpField1));
		tmpField1[0] = 0x01;
		tmpField1[1] = 0x00;
		tmpField1[2] = tmpFieldLen;*/
		/*memcpy(tmpField1 + 3, tmpField, tmpFieldLen);	
		AddVisaField(VISA_ICC_DATA, VisaInfo, tmpField1, tmpFieldLen + 3);*/
			
	}

	/* (60) Additional POS Info */
	if (GetIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		GetSecurityData(SOURCE_RESOURCE, tmpField, tmpField1, &tmpFieldLen);

	GetTlvPrivate( RECURRING_FLAG_TAG , &kTlvInfo, RecurringFlag, &tmpFieldLen);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	GetTlvPrivate( AUTH_ADD_INFO , &kTlvInfo, sAddAuthInfo, &tmpFieldLen);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	/*if (GetIsoField(ISO_POS_DATA, IsoInfo, isoPosMode, &tmpFieldLen) == SUCCESS && memcmp(isoPosMode, "1", 1))*/ /*AMER PROD00054205: Don't need to check if ISS F22.1 != '1' (Manual, no terminal) in order to send F60 to base1 ? In fact this field contains Ecom indicator*/ 
	if (GetIsoField(ISO_POS_DATA, IsoInfo, isoPosMode, &tmpFieldLen) == SUCCESS)
	{
		memset(AddPos, 0, sizeof(AddPos));
		/*GetTlvPrivate( RECURRING_FLAG_TAG , &kTlvInfo, RecurringFlag, &tmpFieldLen); */
		/*GetTlvPrivate( AUTH_ADD_INFO , &kTlvInfo, sAddAuthInfo, &tmpFieldLen);*/		/*AMER20190219 PROD00065991 Enh19.Q2 Art 2.1*/
		GetTlvPrivate( POS_DATA_EXT_TAG , &kTlvInfo, sPosExtData, &tmpFieldLen);	/*PLUTONL-3326*/
		BuildAddPosInfo(isoPosMode, MerchantType, isoProcCode, RecurringFlag, sAddAuthInfo,sPosExtData, AddPos, IsoInfo);	/*AMER20180309 PROD00054205 Enh18.Q2 Art 1.1: Adding RecurringFlag argument*//*PLUTONL-3326*/
		AddVisaField(VISA_ADTNL_POS_INFO, VisaInfo, AddPos, strlen(AddPos));
	}

	/* (62) CPS Data */
	InitCpsInfo(&CpsInfo); /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART 9.1.1*/
	if (!memcmp(MerchantType, "6011", 4)) 
	{
		AddCpsSubField(CPS_ACI, &CpsInfo, "Y", 1);
	}
	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART 9.1.1*/
	if (GetTlvPrivate(MVV_TAG, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{	
		AddCpsSubField(CPS_MVV, &CpsInfo, tmpField, tmpFieldLen);	
	}

	tmpFieldLen = CpsBuildField(tmpField, &CpsInfo);
	if (tmpFieldLen > 8)
	AddVisaField(VISA_CPS_FIELDS, VisaInfo, tmpField, tmpFieldLen);
	
	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART 9.1.1*/
	/* (63) Visa Private use field*/
	/*memset(Priv_Use_Field, 0, sizeof(Priv_Use_Field));
	vBitMap[0] = 0x80;
	vBitMap[1] = 0x00;
	vBitMap[2] = 0x00;
	memcpy(Priv_Use_Field,vBitMap,3);
	Priv_Use_Field[3] = 0x00;
	Priv_Use_Field[4] = 0x00;
	AddVisaField(VISA_PRIV_USE_FIELDS, VisaInfo, Priv_Use_Field, 5);*/
	/*Start IBO20190823 PROD00071113 Enh19.Q4 Art 2.1*/
	InitPufInfo (&PufInfo);
	AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0000", 4); /* Priority Routing */
	if (GetIsoField (ISO_FUNCTION_CODE, IsoInfo,tmpField,&tmpFieldLen) == SUCCESS)
	{
		if(memcmp(tmpField, P7_F024_DEFERRED_AUTH, tmpFieldLen) == 0)
		AddPufSubField (PUF_MSG_REASON, &PufInfo, MSG_REASON_DEFERRED_AUTH, 4);
	}
	tmpFieldLen = PufBuildField (tmpField, &PufInfo);                          
	AddVisaField (VISA_PRIV_USE_FIELDS, VisaInfo, tmpField, tmpFieldLen);
	/*End IBO20190823 PROD00071113 Enh19.Q4 Art 2.1*/
	/**if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
	InitTlvPrivate(&kTlvInfo);
	AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);      */
	/*AMER PROD00054205: To remove the code below because we put CAVV in verification data field*/
	/*if (GetTlvPrivate(ECOM_AAV_TAG, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_ADR_VERIF_DATA, VisaInfo, tmpField, tmpFieldLen);*/
	/*}*/
	
	/*Start AMER20190219 PROD00065991: Enh19.Q2 Art 2.4*/
	if (GetTlvPrivate(MERCHANT_ADDITIONAL_DATA, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kIsoPayFacTlv);
		
		if( GetTlvInfoField (P7_F048P21_PAYFAC_ID, &kIsoPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			PutTlvInfoField(B1_F104T56_PAY_FAC_ID, &kVisaPayFacTlv, tmpField, tmpFieldLen);
		}
		
		if( GetTlvInfoField (P7_F048P21_SUB_MER_ID, &kIsoPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			PutTlvInfoField(B1_F104T56_SUB_MER_ID, &kVisaPayFacTlv, tmpField, tmpFieldLen);
		}
		
		if( GetTlvInfoField (P7_F048P21_ISO_ID, &kIsoPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			PutTlvInfoField(B1_F104T56_ISO_ID, &kVisaPayFacTlv, tmpField, tmpFieldLen);
		}
		
		tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kVisaPayFacTlv);
		if( tmpFieldLen > 0 )
		{
			AddTlvInfoField(B1_F104_PAY_FAC_DATA, &kVisaTransSpecTlv, tmpField, tmpFieldLen);
		}

		/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17
		tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kVisaTransSpecTlv);
		if (tmpFieldLen > 0)
			AddVisaField(VISA_TRANS_DESC, VisaInfo, tmpField, tmpFieldLen);*/
	}

	/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if (GetTlvPrivate(P7_F048_LOAN_DATA, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kIsoRecurringPayTlv);

		if( GetTlvInfoField (P7_F048S24_NBR_INSTALMENTS, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(B1_F104T18_NBR_RECURRING_PAY, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_FREQ_RECUR_PAY, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(B1_F104T18_FREQ_RECURRING_PAY, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_REGISTRATION_REF_NBR, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(B1_F104T18_REGISTR_REF_NBR, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_MONTHLY_RPYMNT_AMNT, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(B1_F104T18_MAX_RECURRING_PAY_AMNT, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}
		/*Start IZL 26092022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/
		if( GetTlvInfoField (P7_F048S24_VALIDATION_IND, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(B1_F104T18_VALIDATION_IND, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
	}
		/*End IZL 26092022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/
	}

	/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if(memcmp(isoFunctionCode, P7_F024_RECURRING_MODIF, P7_F024_LEN) == 0 && atoi(isoTrxAmnt) == 0)
	{
		AddTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_MODIFICATION, B1_F104T18_RECURRING_PAY_TYPE_LEN);
		nNbrTags++;
	}
	else if(memcmp(isoFunctionCode, P7_F024_RECURRING_CANCEL, P7_F024_LEN) == 0 && atoi(isoTrxAmnt) == 0)
	{
		AddTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_CANCELLATION, B1_F104T18_RECURRING_PAY_TYPE_LEN);
		nNbrTags++;
	}
	else if (strlen(RecurringFlag) > 0 /*IBO20230413 PLUTONL-6184*/
		&&(RecurringFlag[0] ==P7_F048_P71_CRED_ON_FILE
		||RecurringFlag[0]==P7_F048_P71_INSTALLMENT
		||RecurringFlag[0]==P7_F048_P71_RECURRING
		||RecurringFlag[0]==P7_F048_P71_STANDING))
	{
		if(strlen(sAddAuthInfo > P7_F048_P64_CRDNTL_REASON)
			&& sAddAuthInfo[P7_F048_P64_CRDNTL_REASON] != P7_F048_P64_P7_UNSPECIFIED)
			{
				AddTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_REGISTRATION_FIRST_TRANS, B1_F104T18_RECURRING_PAY_TYPE_LEN);
				nNbrTags++;
			}
			else
			{
				AddTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_SUBSEQ_TRANS, B1_F104T18_RECURRING_PAY_TYPE_LEN);
				nNbrTags++;
			}
	}
	/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if (strlen(RecurringFlag) > 0 /*IBO20230413 PLUTONL-6184*/
		&&(RecurringFlag[0]==P7_F048_P71_RECURRING
		||RecurringFlag[0]==P7_F048_P71_INSTALLMENT))
	{
		AddTlvInfoField(B1_F104T18_PAY_AMNT_IND_PER_TRANS, &kVisaRecurringPayTlv, B1_F104T18_T81_FIX_AMNT_IND_PER_TRANS, B1_F104T18_PAY_AMNT_IND_PER_TRANS_LEN);
		nNbrTags++;
	}

	if(strlen(RecurringFlag) > 0 /*IBO20230413 PLUTONL-6184*/
		&&(RecurringFlag[0]==P7_F048_P71_STANDING
		||RecurringFlag[0]==P7_F048_P71_CRED_ON_FILE))
	{
		AddTlvInfoField(B1_F104T18_PAY_AMNT_IND_PER_TRANS, &kVisaRecurringPayTlv, B1_F104T18_T81_RECUR_PAY_MAX_AMNT, B1_F104T18_PAY_AMNT_IND_PER_TRANS_LEN);
		nNbrTags++;
	}
	/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kVisaRecurringPayTlv);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(B1_F104_RECURRING_PAY, &kVisaTransSpecTlv, tmpField, tmpFieldLen);
	}

	if(strlen(RecurringFlag) > 0
    	&& nNbrTags != VISA_F104_T18_NB_FIELDS /*- 1 */ /*IZL 26092022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/
    	&& GetIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS
    	&& memcmp(tmpField,"356", 3) == 0) /*Indian Cards*/
    {
    	trace_message(P7_TL_ERROR, "Tags missing in TLV Field 104, Usage 2, Dataset ID 18 for Indian Card");
    	IsoInfo->nMsgType += 10;
    	ReplyIsoMsgToBal(nBalMsg, IsoInfo, "", INVALID_TRANSACTION_ACTION);
    	trace_message(P7_TL_PROCESSING, "End  BuildAutReqToNw ERROR(NOK)");
    	return (NOK);
    }
    /*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kVisaTransSpecTlv);	/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if (tmpFieldLen > 0)
		AddVisaField(VISA_TRANS_DESC, VisaInfo, tmpField, tmpFieldLen);

	/*End AMER20190219 PROD00065991: Enh19.Q2*/

	/* Field 126: VISA Private Use Fields */
	/**if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
	InitTlvPrivate(&kTlvInfo);
	AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);**/
	vpuf_presence = 0;
	InitVisaPufInfo(&VisapufInfo);
	/*Start AMER20180313 PROD00054205: Enh2018.Q2 Art 1.1*/
	if ( GetTlvPrivate( TRANS_XID_TAG , &kTlvInfo,tmpField, &FieldLen) == SUCCES)
	{
		AddVisaPufSubField(VPUF_TRX_ID_VSEC, &VisapufInfo, tmpField, 40);
		vpuf_presence = 1;
	}

	if ( GetTlvPrivate( AAV_TAG , &kTlvInfo,tmpField, &FieldLen) == SUCCES)
	{
		AddVisaPufSubField(VPUF_CAVV , &VisapufInfo , sTransStain, 40);
		vpuf_presence = 1;
	}
	else if ( GetTlvPrivate( UCAF_TAG , &kTlvInfo,tmpField, &FieldLen) == SUCCES)
	{
		Base64_Decode(nIndexCtx, tmpField, sTransStain);
		AddVisaPufSubField(VPUF_CAVV , &VisapufInfo , sTransStain, 40);
		vpuf_presence = 1;
	}
	/*End AMER20180313 PROD00054205: Enh2018.Q2 Art 1.1*/

	/*if ( GetTlvPrivate( TRANS_XID_TAG , &kTlvInfo,tmpField, &FieldLen) == SUCCES)
	{
		memset(sChCert, 0, sizeof(sChCert));
		memset(sMerCert, 0, sizeof(sMerCert));
		memset(sTransId, 0, sizeof(sTransId));
		memset(sTransStain, 0, sizeof(sTransStain));
		memset(sChCert, '0', 34);
		sprintf(sChCert, "%02d", FieldLen);
		memset(sChCert + 2, '0', 32);
		memcpy(sChCert + 2 + 32 - FieldLen, tmpField, FieldLen);
		sprintf(sLine, "CH CERTIFICAT %s ", sChCert);
		trace_event(sLine, TRACE);

		memset(sChCert, '0', 34);
		AddVisaPufSubField(VPUF_CARDHOLDER_VSEC, &VisapufInfo, sChCert, 34);
		memset(sMerCert, '0', 34);
		AddVisaPufSubField(VPUF_MERCHANT_VSEC, &VisapufInfo, sMerCert, 34);
		memset(sTransId, '0', 40);
		memcpy	 ( sTransId, tmpField ,  FieldLen);
		AddVisaPufSubField(VPUF_TRX_ID_VSEC, &VisapufInfo, sTransId, 40);

		if ( GetTlvPrivate( UCAF_TAG , &kTlvInfo,tmpField, &FieldLen) == SUCCES)
		{
			sprintf (sLine, "ucaf value [%d][%s]....",strlen(tmpField),tmpField);
			trace_event (sLine, PROCESSING);
			if ((strlen(tmpField))>=40)
			{
				tmpField[40]=0;
				memcpy	 ( sTransStain, tmpField ,  40);
				AddVisaPufSubField(VPUF_CAVV , &VisapufInfo , sTransStain, 40);
			}
		}
		else
		{
			trace_event ("no ucaf getting", PROCESSING);
			memset(sTransStain, '0', 40);
			AddVisaPufSubField(VPUF_CAVV , &VisapufInfo , sTransStain, 40);

		}

		vpuf_presence = 1;

	}*/
	/* CVC2 MNG */
	if (GetTlvPrivate(CVC2_DATA_TAG, &kTlvInfo, tmpField, &FieldLen) == SUCCES)
	{
		memset(sCvc2Data, ' ', 6);
		memset(tmpField1,0,sizeof(tmpField1));
		if (strlen(tmpField) >= 3)
			memcpy(tmpField1,tmpField,3);
		else
			memcpy(tmpField1,"000",3);
		tmpField1[3]=0;


		sprintf(sCvc2Data, "11 %.3s", tmpField);

		sCvc2Data[6]=0;

		AddVisaPufSubField(VPUF_CVV2, &VisapufInfo, sCvc2Data, 6);
		vpuf_presence = 1;

	}

	/*if ( GetTlvPrivate( RECURRING_FLAG_TAG , &kTlvInfo,tmpField, &FieldLen) == SUCCES)
	{
		AddVisaPufSubField(VPUF_RECURRING, &VisapufInfo , tmpField, FieldLen);
		vpuf_presence = 1;
	}*/

	/*Start IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	if (GetTlvPrivate(RECURRING_FLAG_TAG, &kTlvInfo,sRecuringFlag, &tmpFieldLen) == SUCCES)
	{
		if (sRecuringFlag[0] == P7_F048_P71_RECURRING
			||sRecuringFlag[0] == P7_F048_P71_STANDING)
		{
			sPufData[0] = B1_F126_P13_RECURRING;
		}
		else if (sRecuringFlag[0] == P7_F048_P71_INSTALLMENT)
		{
			sPufData[0] = B1_F126_P13_INSTALLMENT;
		}
		else if (sRecuringFlag[0] == P7_F048_P71_CRED_ON_FILE)
		{
			sPufData[0] = B1_F126_P13_CREDENTIAL;
		}
		AddVisaPufSubField(VPUF_RECURRING, &VisapufInfo , sPufData, 1);
		vpuf_presence = 1;
	}
	else if (GetTlvPrivate(AUTH_ADD_INFO, &kTlvInfo, sAuthAddInfo , &tmpFieldLen) == SUCCES
			&& sAuthAddInfo[P7_F048_P64_CRDNTL_REASON] == P7_F048_P64_P7_CH_CRDTL_ONFILE)
	{
		sPufData[0] = B1_F126_P13_CREDENTIAL;
		AddVisaPufSubField(VPUF_RECURRING, &VisapufInfo , sPufData, 1);
		vpuf_presence = 1;
	}
	/*End IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	/*ZKO:Start 08112018*/
	if ( GetTlvPrivate( DCC_DATA , &kTlvInfo,tmpField, &FieldLen) == SUCCES
			&& tmpField[0] == P7_F048_P56_DCC_APPROVED)
	{
		AddVisaPufSubField(VPUF_DCC_INDICATOR, &VisapufInfo , "1", 1);
		vpuf_presence = 1;
	}
	/*ZKO:End */

	/*Start HAL 20201903 ENHS 2020Q2 ART 9.3.3*/
    if (GetIsoField(ISO_NETWORK_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitTlvPrivate(&kNwTlvData);
		AnalyseTlvPrivate(tmpField,tmpFieldLen,&kNwTlvData);

		if(GetTlvPrivate( NET_VISA_MERC_ID , &kNwTlvData, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AddVisaPufSubField(VPUF_MER_ID, &VisapufInfo , tmpField, tmpFieldLen);
			vpuf_presence = 1;
		}
	}
    /*End HAL 20201903 ENHS 2020Q2 ART 9.3.3*/


	/*}*/
	/*
	* Mise a jour du champs 61 ISO pour mettre les valeur 22, 25 et 60
	* pour mise a jour d'autjo_activity
	*/
	/* Field 61: contains VISA fields 22, 25, & 60 in TLV format */

	GetVisaField(VISA_POS_ENTRY_MODE, VisaInfo, PosMode, &tmpFieldLen);
	GetVisaField(VISA_POS_CONDITION, VisaInfo, PosCond, &tmpFieldLen);
	GetVisaField(VISA_ADTNL_POS_INFO, VisaInfo, AddPos, &AddPosLen);


	field_61_presence = 0;
	if (strlen(PosMode) > 0)
	{
		field_61_presence = 1;
		AddTlvPrivate(POS_ENTRY_MODE_TAG, &TlvNetPOSData, PosMode, 4);
	}
	if (strlen(PosCond) > 0)
	{
		field_61_presence = 1;
		AddTlvPrivate(POS_CONDITION_CODE_TAG, &TlvNetPOSData, PosCond, 2);
	}
	if (strlen(AddPos) > 0)
	{
		field_61_presence = 1;
		AddTlvPrivate(ADTNL_POS_DATA_TAG, &TlvNetPOSData, AddPos, AddPosLen);
	}
	if (field_61_presence == 1)
	{
		tmpFieldLen = PrivateBuildTlv(tmpField, &TlvNetPOSData);
		PutIsoField(ISO_NET_POS_DATA, IsoInfo, tmpField, tmpFieldLen);
	}

	/*Field 125 : Supporting Information*/
	/*Start IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	if (GetTlvPrivate(ORIGINAL_TRX_ID, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		if(memcmp (tmpField,"0100000000000000",tmpFieldLen) != 0)
		{
			InitVisaAddOriginalDataTlv(&kVisaTlvInfo);
			AddTlvInfoField(B1_F125T03_ORIGINAL_TRA_ID, &kVisaTlvInfo, tmpField, tmpFieldLen);
			tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kVisaTlvInfo);
			if( tmpFieldLen > 0 )
			{
				AddTlvInfoField(B1_F125_ADD_ORIGINAL_DATA, &kVisaSuppInfTlv, tmpField, tmpFieldLen);
			}
			tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kVisaSuppInfTlv);
			if (tmpFieldLen > 0)
				AddVisaField(VISA_SUPPORT_INFO, VisaInfo, tmpField, tmpFieldLen);
		}

	}
	/*End IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/

	/*Start IBO20220816 PLUTONL-5085*/
	if(vpuf_presence == 1)
	{
		memset(sVisaPufData, 0, sizeof(sVisaPufData));

		FieldLen = VisaPufBuildField(sVisaPufData,&VisapufInfo);
		if (FieldLen > 0)
			AddVisaField (VISA_PRIV_USE_FIELD , VisaInfo, sVisaPufData, FieldLen);
	}
	/*End IBO20220816 PLUTONL-5085*/
	/*Start HAL ENHQ2.20 completion advice Art 1.1*/
	tmpFieldLen = PrivateBuildTlv(tmpField, &kTlvInfo);
	if (tmpFieldLen > 0)
		PutIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, tmpFieldLen);
    /*End HAL ENHQ2.20 completion advice Art 1.1*/

	dump_p7_struct(IsoInfo);


	memset (tmpField      , 0, sizeof(tmpField));
	memset (tmpField1      , 0, sizeof(tmpField1));
	memset (Pan      , 0, sizeof(Pan));
	memset (ExpiryDate      , 0, sizeof(ExpiryDate));
	memset (CVV      , 0, sizeof(CVV));
	memset (PVV      , 0, sizeof(PVV));
	memset (T1_Data      , 0, sizeof(T1_Data));
	memset (T2_Data      , 0, sizeof(T2_Data));
	memset (PinBlock      , 0, sizeof(PinBlock));
	memset (Transkey      , 0, sizeof(Transkey));
	memset (ZPK      , 0, sizeof(ZPK));
	memset (szWork      , 0, sizeof(szWork));
	memset (sCvc2Data      , 0, sizeof(sCvc2Data));
	memset (sVisaPufData      , 0, sizeof(sVisaPufData));
	memset (Priv_Use_Field      , 0, sizeof(Priv_Use_Field));
	memset (sIccBuffer      , 0, sizeof(sIccBuffer));
	memset (FieldData      , 0, sizeof(FieldData));
	InitTlvPrivate(&kTlvInfo);
	InitVisaPufInfo(&VisapufInfo);
	InitIsoIcTag(&IsoIccInfo);
	InitVisaIcTag(&VisaIccInfo);
	InitPufInfo (&PufInfo);


	trace_event("End  BuildAutReqToNw(OK)", PROCESSING);
	return (OK);
}


/****************************************************************************/
/* BuildAutReqFromNw()                                                    */
/* */
/* Converts a received authorization request message (0100) from BASEI    */
/* into an ISO-format message for processing.                             */
/* Input:  VisaInfo: received message                                     */
/* Output: IsoInfo:  generated ISO message                                */
/****************************************************************************/
int BuildAutReqFromNw(int nIndexCtx,
	TSVisaInfo * VisaInfo,
	TSIsoInfo * IsoInfo)
{
	TSTlvPrivate    kTlvInfo;
	TSCpsInfo       CpsInfo;
	char            tmpField[BUF_LG_MAX];
	char            tmpField1[BUF_LG_MAX];
	int             tmpFieldLen;
	int             AddPosLen;
	char            Pan[20];
	char            ExpiryDate[5];
	char            CVV[4];
	char            PVV[6];
	char            ServiceCode[4];
	char            T1_Data[77];
	char            T2_Data[38];
	char            MerchantType[5];
	char            ProcCode[7];
	char            PosMode[7];
	char            PosCond[3];
	char            AddPos[24];
	char		AciCode[4]; /* AMA */
	char            sIsoProcCode[7];
	char            sPid[8 + 1];
	char            sLine[MAX_LINE_TRC];
	char            sRespCode[3 + 1];
	char            sVisaRespCode[2 + 1];
	int             field_61_presence = 0;
	TSVisaPufInfo   VisapufInfo;
	char            sPufData[1024];
	char            IwkLmk[256 + 1];
	char 		  sCAVV[128];
	char 		  sCAVV_Com[128];
	char 		  sXID [128];
	char			sTrxAmount[32];
	char			sBillingAmount[32];
	char			sTrxCurrency[4];
	char			sBillingCurrency[4];
	char                   tmpField2 [256 +1];
	char                   tmpField3 [256 +1];
	char            EcommerceData [P7_F048_P66_LEN  +1];/*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	char            nEcommerceDatalen =0;/*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	char			sAuthId[64];
	char			sTrnId[64];
	char			sAuthCycleCode[12];
	/*char			sECI[8];*/
	char			sFunctionCode[3 + 1];
	char			sVisaReasonCode[4 + 1];
	char			sBAI[12];
	char			sAvsRequest[1+1];
	int             nLength;
	int             i;
	TSPufInfo		kPufInfo;
	TlvInfo			kVisaTransSpecTlv;
	TlvInfo			kIsoMoneyTransfTlv;
	TlvInfo			kIsoPayFacTlv;
	TlvInfo			kVisaPayFacTlv;
	BitMappedInfo	kVIPufData;
	char            szGMTDate[12 + 1];      /*AMER 20160719*/
	char            szTrxDateTime[12 + 1];  /*AMER 20160619*/
	char			sAfdRefNbr[32+1];       /*AMER20170313*/
	char			RecurringFlag[1 + 1];
	int             nRetFlag; /*NAB16102018*/
	char            sAddAuthInfo[P7_F048_P64_LEN + 1];
	TlvInfo			kVisaElectComTlv;   		/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	int				tagPresence = 0;			/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	TlvInfo			kVisaSuppInfTlv;   			/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	TlvInfo			kVisaTlvInfo;   			/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17 and 9.1.4*/
	TlvInfo			kVisaBerInfo; 				/*IBE PLUTONL-5640*/  	
	char			sEcomAuthInfo[P7_F048_P66_LEN + 1];
    char			sAvsData[51 + 1];/*ACH20191010*/
	char          	sIsoPosMode[12 + 1]; 						/*ACH 20191107*/
	int				nEcomInfoLen = 0;
	char			sOtherAmnt[32];    /*NAB11032020*/
	char			sTrxOtherAmnt[32]; /*NAB11032020*/
	TSTagIso		IsoIccInfo;                           /* NAB12032020*/
	char         	FieldData[256];                      /* NAB12032020*/
	int             FieldLen;                            /* NAB12032020*/
	TlvInfo    		isoExemption;       /*HAL  20201803 Enh Q2.2020 ART 9.3.3*/
	char 			sPSD2AuthExempInd[16 + 1];
	char 			sPSD2AuthExempRes[8 + 1];/*PLUTONL-6119  Enh 2023Q2  Visa 9.3.1 AHA20230403 */			
	int				nExempIndLen = 0;
	int				nExempResLen = 0;
	TlvInfo			isoEcomAuthentifData;
	TlvInfo			k3DSData;/*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
	char			sExtStipReasonCode[1+1]; /* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/
	int nFlag;/*PLUTONL-3326*/
	char sPosExtData[ P7_F048_P61_LEN+1];/*PLUTONL-3326*/
	char 			sIsoTermAddr[40+1];/*PLUTONL-3660*/
	char            sTokenResponseVisa[1 + 1];  /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	char			sInitParty[2]; /*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/

	TlvInfo    		kTokenDataIso;    		/*HAL2402 PLUTONL-4391*/
	TlvInfo			visaPanMappingData;  	/*HAL2402 PLUTONL-4391*/
	TlvInfo			kVisaVerifDataTlv;  		/*HAL2402 PLUTONL-4391*/
	TlvInfo			visaAddVerifMappingData;  	/*HAL2402 PLUTONL-4391*/
	TlvInfo         isoDigitizationDataTlv;      /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	TlvInfo         isoSaleInfoDataPropTlv;      /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	TlvInfo			kVisaAppIdentTlv; /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/

	char            sPostalCode[9 + 1];
	char            sAddStreet[40 + 1];
	int 			nLenZipCode;
	int	 			nLenAddr;
	int 			nLenTotal;
	nLenTotal = 0;

	/* En23Q2-Hotfix-PLUTONL-6134 */ 
	char        szFieldAdd [256 +1];
	int         nLen = 0;
	int         int_temp = 0;


	trace_event("Start  BuildAutReqFromNw()", PROCESSING);

	memset(tmpField, 0, sizeof(tmpField));
	memset(tmpField1, 0, sizeof(tmpField1));
	memset(EcommerceData, ' ', sizeof(EcommerceData));/*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	memset(Pan, 0, sizeof(Pan));
	memset(ExpiryDate, 0, sizeof(ExpiryDate));
	memset(CVV, 0, sizeof(CVV));
	memset(PVV, 0, sizeof(PVV));
	memset(ServiceCode, 0, sizeof(ServiceCode));
	memset(T1_Data, 0, sizeof(T1_Data));
	memset(T2_Data, 0, sizeof(T2_Data));
	memset (sCAVV,      	 0, sizeof(sCAVV));
	memset (sXID,      	 0, sizeof(sXID));
	memset (tmpField2, 0, sizeof(tmpField2));
	memset (tmpField3, 0, sizeof(tmpField3));
	memset (sVisaReasonCode, 0, sizeof(sVisaReasonCode));
	memset(szGMTDate,'\0',sizeof(szGMTDate));       /*AMER 20160619*/
	memset(szTrxDateTime,'\0',sizeof(szTrxDateTime));       /*AMER 20160619*/
	memset (sTrxAmount,      	 0, sizeof(sTrxAmount));
	memset (sBillingAmount,      0, sizeof(sBillingAmount));
	memset (sTrxCurrency,		0, sizeof(sTrxCurrency));
	memset (sBillingCurrency,   0, sizeof(sBillingCurrency));
	memset (sAuthId,   0, sizeof(sAuthId));
	memset (sTrnId,   0, sizeof(sTrnId));
	memset (sAuthCycleCode,   0, sizeof(sAuthCycleCode));
	memset (sFunctionCode,   0, sizeof(sFunctionCode));
	/*memset (sECI,   0, sizeof(sECI));*/
	memset (sBAI,   0, sizeof(sBAI));
	memset(AciCode, 0, sizeof(AciCode)); /* AMA */
	memset (sAfdRefNbr,  0, sizeof(sAfdRefNbr));	/*AMER20170313*/
	memset (RecurringFlag,  0, sizeof(RecurringFlag));
	memset(sEcomAuthInfo, '\0', sizeof(sEcomAuthInfo));
 	memset(sEcomAuthInfo, ' ', P7_F048_P66_LEN);
    memset (sAvsRequest,  0, sizeof(sAvsRequest));
	memset (sIsoPosMode,   0, sizeof(sIsoPosMode)); 			/*ACH 20191107*/
	memset (sOtherAmnt,      	 0, sizeof(sOtherAmnt));/*NAB11032020*/
	memset (sTrxOtherAmnt,       0, sizeof(sTrxOtherAmnt));/*NAB11032020*/
	memset (FieldData,           0, sizeof(FieldData));/*NAB11032020*/
 	memset(sPSD2AuthExempInd, '0', sizeof(sPSD2AuthExempInd)); /*HAL 20201803 ENHS 2020Q2 ART 9.3.3*/ /*AEA20210712 Enh21.Q4*/
	memset(sPSD2AuthExempRes, ' ', sizeof(sPSD2AuthExempRes)); /*HAL 20201803 ENHS 2020Q2 ART 9.3.3*/
    memset (sExtStipReasonCode,  0, sizeof(sExtStipReasonCode)); /* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/
	memset(sPosExtData  ,' ',sizeof(sPosExtData ));/*PLUTONL-3326*/
	memset(sTokenResponseVisa, 0, sizeof(sTokenResponseVisa));    /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	memset(sInitParty, 0, sizeof(sInitParty));     /*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	memset (sAddAuthInfo,  '0', sizeof(sAddAuthInfo));
	memset(sPostalCode, 0, sizeof(sPostalCode));
	memset(sAddStreet, 0, sizeof(sAddStreet));

	memset(szFieldAdd, 0, sizeof(szFieldAdd)); /* En23Q2-Hotfix-PLUTONL-6134 */

	sAddAuthInfo[P7_F048_P64_CRDNTL_REASON] = P7_F048_P64_P7_UNSPECIFIED;/* IZL 20220131 PLUTONL-4015 ENH 22Q2 MC AN5524*/


    nRetFlag = OK ; /*NAB16102018*/

	/* Initialize ISO info structure */
	InitIsoInfo(IsoInfo);
	InitVisaPufInfo(&VisapufInfo);
	InitCpsInfo(&CpsInfo);
	IniIsoMoneyTransDataTlv(&kIsoMoneyTransfTlv);
	IniIsoPayFacDataTlv(&kIsoPayFacTlv);
	InitVisaTransSpecDataTlv(&kVisaTransSpecTlv);
	InitVisaPayFacDataTlv(&kVisaPayFacTlv);
	InitVisaVIPPufDataTlv(&kVIPufData);
	InitPufInfo (&kPufInfo);
	current_gmt_date (szGMTDate);   /*AMER 20160619*/
	InitVisaElectComDataTlv(&kVisaElectComTlv);			/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	InitVisaSupportInfDataTlv(&kVisaSuppInfTlv);		/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/

	IniTknDataTlv(&kTokenDataIso);					/*HAL2402 PLUTONL-4391*/
	InitB1TokenDataTlvInfo(&visaPanMappingData);	/*HAL2402 PLUTONL-4391*/
	InitB1VerifDataTlvInfo(&kVisaVerifDataTlv);			/*HAL2402 PLUTONL-4391*/

	InitB1AddrVerifDataTlvInfo(&visaAddVerifMappingData);	/*HAL2402 PLUTONL-4391*/
	IniDigitDataTlv(&isoDigitizationDataTlv);   /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	IniSaleInfoDataTlv(&isoSaleInfoDataPropTlv);	/*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	InitEcomAuthentificationInfoDataTlv(&isoEcomAuthentifData); /*IBO20221018 PLUTONL-5240*/
	InitInfo3DSDataTlv(&k3DSData);                     /*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
	InitVisaAppIdentDataTlv(&kVisaAppIdentTlv);  /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/

	/* Message Type is 1100 */
	switch (VisaInfo->nMsgType)
	{
	case 101:
		IsoInfo->nMsgType = 1101;break;
	case 120:
		IsoInfo->nMsgType = 1120;break;
	case 121:
		IsoInfo->nMsgType = 1121;break;
	default:
		IsoInfo->nMsgType = 1100;break;
	}

	if (GetVisaField(VISA_T2_DATA, VisaInfo, T2_Data, &tmpFieldLen) == SUCCESS)
	{
		GetTrack2Data(T2_Data, Pan, ExpiryDate, ServiceCode, PVV, CVV);
	}
	/*Start IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	if (GetVisaField(VISA_PAN_EXTENDED /*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaElectComTlv);
	}
	/*End IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	if (GetVisaField(VISA_PRIV_USE_FIELD, VisaInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		AnalyseVisaPuf(tmpField, &VisapufInfo);
	}
	if (GetVisaField (VISA_CPS_FIELDS, VisaInfo, tmpField, &tmpFieldLen)== SUCCESS)
	{
		AnalyseCps     (tmpField, &CpsInfo);
	}

	if (GetVisaField (VISA_PRIV_USE_FIELDS, VisaInfo, tmpField, &tmpFieldLen)== SUCCESS)
	{
		AnalyseBitMappedInfo(tmpField, tmpFieldLen, &kVIPufData);
	}

	/* AMA */
	if (GetVisaField (VISA_PRIV_USE_FIELDS, VisaInfo,tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalysePuf (tmpField, &kPufInfo);
	}



	/* Field 2: Primary Account Number */
	if (GetVisaField(VISA_CARD_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_CARD_NBR, IsoInfo, tmpField, tmpFieldLen);
	else if ( strlen(Pan)  > 0 )
	{
		AddIsoField(ISO_CARD_NBR, IsoInfo, Pan, strlen(Pan));
	}
	/* Field 3: Processing Code */
	/*
	* Attention Le Processing Code Est Reajuster Dans Le Cas De Trx
	* E-Com Voir Champs 22
	*/
	memset(sIsoProcCode, 0, sizeof(sIsoProcCode));
	GetVisaField(VISA_PROC_CODE, VisaInfo, ProcCode, &tmpFieldLen);
	GetVisaField(VISA_MERCHANT_TYPE, VisaInfo, MerchantType, &tmpFieldLen);
	VisaToIsoProcessingCode(ProcCode, MerchantType, sIsoProcCode);
	AddIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, 6);
	/*Start ICH20170413*/
	if (memcmp (sIsoProcCode ,P7_F003_BAL_INQ, P7_F003_P1_LEN) != 0)
	{
		/*NAB11032020 put the cash back processing code if the trx is cash back --SIA*/
		if ((memcmp (sIsoProcCode ,P7_F003_PURCHASE, P7_F003_P1_LEN) == 0)
			&&(GetVisaField (VISA_OTHER_AMOUNTS, VisaInfo,sOtherAmnt, &tmpFieldLen) == SUCCES) )
		{
			memcpy(sTrxOtherAmnt, sOtherAmnt,12);

			if(atoi(sTrxOtherAmnt) > 0)
			{
				memcpy(sIsoProcCode,P7_F003_CASHBACK,P7_F003_P1_LEN);
				PutIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, 6);
			}
		}
		/*NAB11032020 */
		/* Field 4: Transaction Amount */
		if (GetVisaField(VISA_TRANS_AMOUNT, VisaInfo, sTrxAmount, &tmpFieldLen) == SUCCESS)
				AddIsoField(ISO_TRANS_AMOUNT, IsoInfo, sTrxAmount, 12);
		else if ( memcmp(sIsoProcCode,P7_F003_PIN_UNBLOCK, P7_F003_P1_LEN) == 0 
				|| memcmp(sIsoProcCode,P7_F003_PIN_CHANGE, P7_F003_P1_LEN) == 0 )/*NAB12032020 IF TRX Amount is abscent at pin chanage/unblock request we fill it  by zeros*/
		{   
				AddIsoField(ISO_TRANS_AMOUNT, IsoInfo,   "000000000000", 12);
				PutIsoField(ISO_BILLING_AMOUNT, IsoInfo, "000000000000", 12);

		}

		if( ( memcmp(sIsoProcCode,P7_F003_PIN_UNBLOCK, P7_F003_P1_LEN) == 0 && atoi(sTrxAmount) > 0) ||
			memcmp(ProcCode,B1_F003_P1_PP_ACTIV_N_LD, B1_F003_P1_LEN) == 0 )
		{
			memcpy(sIsoProcCode,P7_F003_PAYMENT,P7_F003_P1_LEN);
			PutIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, 6);
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, &kIsoMoneyTransfTlv, PAY_TYPE_PP_INIT_LD, PAY_TYPE_LEN);
		}

		/* Field 5: Settlement Amount */
		if (GetVisaField(VISA_BILLING_AMOUNT, VisaInfo, sBillingAmount, &tmpFieldLen) == SUCCESS)
			AddIsoField(ISO_STLMNT_AMOUNT, IsoInfo, sBillingAmount, 12);
		else if( strlen(sTrxAmount) > 0 )
			AddIsoField(ISO_STLMNT_AMOUNT, IsoInfo, sTrxAmount, 12);

		/* Field 6: Cardholder Billing Amount */
		if (strlen(sBillingAmount) > 0)
			AddIsoField(ISO_BILLING_AMOUNT, IsoInfo, sBillingAmount, 12);
		else if( strlen(sTrxAmount) > 0 )
			AddIsoField(ISO_BILLING_AMOUNT, IsoInfo, sTrxAmount, 12);
	}
	else
	{
		/* (4) TRANSACTION AMOUNT */
		AddIsoField(ISO_TRANS_AMOUNT, IsoInfo, "000000000000", 12);

		/* (5) SETTELMENT AMOUNT */
		AddIsoField(ISO_STLMNT_AMOUNT, IsoInfo, "000000000000", 12);

		/* (6) BILLING AMOUNT */
		AddIsoField(ISO_BILLING_AMOUNT, IsoInfo, "000000000000", 12);
	}
	/*End ICH20170413*/
	/* Field 7: Transmission Date and Time */
	GetVisaField(VISA_XMIT_TIME, VisaInfo, tmpField, &tmpFieldLen);
	AddIsoField(ISO_XMIT_TIME, IsoInfo, tmpField, 10);

	/* Field 10: Cardholder Billing Conversion Rate */
	if (GetVisaField(VISA_BILLING_CNVRSN_RATE, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_BILLING_CNVRSN_RATE, IsoInfo, tmpField, 8);

	/* Field 11: System Trace Audit Number */
	GetInternalStan(nIndexCtx, tmpField);
	AddIsoField(ISO_AUDIT_NBR, IsoInfo, tmpField, 6);

	/* (12) Trans Date & Time */
	current_date_yymmdd(tmpField2);
	if ((GetVisaField(VISA_TRANS_DATE, VisaInfo, szTrxDateTime + 2 /*tmpField*/, &tmpFieldLen) == SUCCESS) && /*AMER 20160619*/
		(GetVisaField(VISA_TRANS_TIME, VisaInfo, tmpField1, &tmpFieldLen) == SUCCESS))
	{
		

		/*AMER 20160619 Start: PROD00029786*/
		SetLocalYear(szTrxDateTime,szGMTDate);
		memcpy(szTrxDateTime + 6,tmpField1,6);
		AddIsoField (ISO_TRANS_DATE_TIME, IsoInfo, szTrxDateTime, 12);
		/*AMER 20160619 End*/
	}
	else
	{
		current_hour(tmpField3);
		memcpy(tmpField2 + 6, tmpField3, 6);
		AddIsoField(ISO_TRANS_DATE_TIME, IsoInfo, tmpField2, 12);
	}

	/* Field 14: Expiry Date */
	if (GetVisaField(VISA_EXPIRY_DATE, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_EXPIRY_DATE, IsoInfo, tmpField, 4);
	else if (strlen(ExpiryDate) > 0)
		AddIsoField(ISO_EXPIRY_DATE, IsoInfo, ExpiryDate, 4);

	/* Field 18: Merchant Type */
	AddIsoField(ISO_MERCHANT_TYPE, IsoInfo, MerchantType, 4);

	/* Field 19: Acquiring Institution Country Code */
	GetVisaField(VISA_ACQR_COUNTRY, VisaInfo, tmpField, &tmpFieldLen);
	AddIsoField(ISO_ACQR_COUNTRY, IsoInfo, tmpField, 3);


	/* Field 20: PAN Country Code */
	if (GetVisaField(VISA_PAN_COUNTRY, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, 3);

	/* Field 22: POS Entry Mode */
	memset(PosMode, 0, sizeof(PosMode));
	memset(PosCond, 0, sizeof(PosCond));
	memset(AddPos, 0, sizeof(AddPos));

	GetVisaField(VISA_POS_ENTRY_MODE, VisaInfo, PosMode, &tmpFieldLen);
	GetVisaField(VISA_POS_CONDITION, VisaInfo, PosCond, &tmpFieldLen);
	GetVisaField(VISA_ADTNL_POS_INFO, VisaInfo, AddPos, &AddPosLen);

	VisaToIsoPosMode(MerchantType, PosMode, PosCond, AddPos, tmpField, sEcomAuthInfo, &nEcomInfoLen);/*IBO20221025 PLUTONL-5272*/
	AddIsoField(ISO_POS_DATA, IsoInfo, tmpField, 12);
	/* Reajustement Processing Code En Cas De Trx E-Comm */



	/* Field 23: Card Seq Number           */
	if (GetVisaField(VISA_CARD_SEQUENCE_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_CARD_SEQUENCE_NBR, IsoInfo, tmpField, tmpFieldLen);



	/* Field 24: Function Code */
	if (memcmp(ProcCode, B1_F003_P1_BALANCE_INQ, B1_F003_P1_LEN) == 0)
	{
		memcpy(sFunctionCode,P7_F024_BAL_REQUEST, P7_F024_LEN);
	}
	else if( memcmp(PosCond,B1_F025_COMPLETION,B1_F025_LEN) == 0 )
	{
		memcpy(sFunctionCode,P7_F024_AUTH_PREV_AMNT_DIFF, P7_F024_LEN);
	}
	else if( memcmp(PosCond,B1_F025_ACC_VERIF,B1_F025_LEN) == 0 )	/*Set function code for non-fin auth*/
	{
		memcpy(sFunctionCode,P7_F024_ACC_VERIF, P7_F024_LEN);
	}
	else if( memcmp(PosCond,B1_F025_NORMAL_TRN,B1_F025_LEN) == 0 &&
		GetBitMappedInfoField (B1_F063_PREAUTH_TIME_LIMIT, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
	{
		memcpy(sFunctionCode,P7_F024_PRE_AUTH, P7_F024_LEN);
		sAuthCycleCode[0]='2'; 
		memcpy(sAuthCycleCode+1,tmpField+2,2);
	}
	else if (memcmp(MerchantType,MCC_AFD,P7_F018_LEN) == 0)	/*PLUTONL-2481*/
	{
		memcpy(sFunctionCode,P7_F024_PRE_AUTH, P7_F024_LEN);
	}
	else
	{
		memcpy(sFunctionCode,P7_F024_INIT_REQUEST, P7_F024_LEN);
	}

	/* AMA  Field 25 :Reason Code*/
	if (GetPufSubField (PUF_MSG_REASON, &kPufInfo, sVisaReasonCode, &tmpFieldLen) == SUCCESS)
	{
		if(memcmp(sVisaReasonCode,MSG_REASON_INC_AUTH,4) == 0)
		{
			memcpy(sFunctionCode,P7_F024_SUPP_AMNT, P7_F024_LEN);		
		}
		/*Start IBO20190823 PROD00071113 Enh19.Q4 Art 2.1*/
		if(memcmp(sVisaReasonCode,MSG_REASON_DEFERRED_AUTH,4) == 0)
		{
			memcpy(sFunctionCode,P7_F024_DEFERRED_AUTH, P7_F024_LEN);		
		}
		/*End IBO20190823 PROD00071113 Enh19.Q4 Art 2.1*/

	}


	GetCpsSubField(CPS_ACI, &CpsInfo, AciCode, &tmpFieldLen);

	if (memcmp(AciCode,"I",1) == 0)
	{

		if (memcmp(sFunctionCode,P7_F024_PRE_AUTH,P7_F024_LEN) == 0 )
			memcpy(sFunctionCode,P7_F024_SUPP_AMNT_ESTMD, P7_F024_LEN);
		else
			memcpy(sFunctionCode,P7_F024_SUPP_AMNT, P7_F024_LEN);


	}

	AddIsoField(ISO_FUNCTION_CODE, IsoInfo, sFunctionCode,P7_F024_LEN);

	if( strlen(sVisaReasonCode) > 0 )
	{
		AddIsoField (ISO_MSG_REASON, IsoInfo, sVisaReasonCode, P7_F025_LEN);
	}
	/* END AMA */


	/* Field 27: Authorization Code Length - Always = 6 */
	AddIsoField(ISO_AUTHOR_ID_LENGTH, IsoInfo, "6", 1);

	/* Field 32: Acquiring Institution Identification Code */
	GetVisaField(VISA_ACQR_ID, VisaInfo, tmpField, &tmpFieldLen);
	AddIsoField(ISO_ACQR_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 33: Forwarding Institution Id */
	if (GetVisaField(VISA_FORWD_ID, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_FORWD_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 35: Track II Data */
	if (GetVisaField(VISA_T2_DATA, VisaInfo, T2_Data, &tmpFieldLen) == SUCCESS) 
	{
		AddIsoField(ISO_T2_DATA, IsoInfo, T2_Data, tmpFieldLen);
		GetTrack2Data(T2_Data, Pan, ExpiryDate, ServiceCode, PVV, CVV);
	}
	/* Field 37: Retrieval Reference Number */
	GetVisaField(VISA_REFERENCE_NBR, VisaInfo, tmpField, &tmpFieldLen);
	AddIsoField(ISO_REFERENCE_NBR, IsoInfo, tmpField, 12);


	/*memcpy(sAuthId + 2 + P7_F011_LEN, tmpField, P7_F037_LEN);*/

	/* Field 38: Auth Number */
	if (GetVisaField(VISA_AUTHOR_ID, VisaInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_AUTHOR_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 39: Response Code */
	if (GetVisaField(VISA_RESPONSE_CODE, VisaInfo, tmpField, &tmpFieldLen) == SUCCES) 
	{
		TranslateRespCode(nIndexCtx, ResourceId, tmpField, "RP", sRespCode);
		AddIsoField(ISO_RESPONSE_CODE, IsoInfo, sRespCode, 3);
	}
	/* Field 40: Service Restriction Code */
	if (GetVisaField(VISA_SERVICE_CODE, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_SERVICE_CODE, IsoInfo, tmpField, 3);
	else 
	{
		if (strlen(ServiceCode) > 0)	/* extract service code from trackII */
			AddIsoField(ISO_SERVICE_CODE, IsoInfo, ServiceCode, 3);
		else  if (GetVisaField(VISA_T1_DATA, VisaInfo, T1_Data, &tmpFieldLen) == SUCCESS) 
		{
			GetTrack1Data(T1_Data, Pan, ExpiryDate, ServiceCode, PVV, CVV);
			AddIsoField(ISO_SERVICE_CODE, IsoInfo, ServiceCode, 3);
		}
	}

	/* Field 41: Terminal Identification */
	if (GetVisaField(VISA_TERMINAL_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_TERMINAL_NBR, IsoInfo, tmpField, 8);

	/* Field 42: Card Acceptor Identification Code */
	if (GetVisaField(VISA_OUTLET_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_OUTLET_NBR, IsoInfo, tmpField, 15);

	/* Field 43: Card Acceptor Location */
	/*PLUTONL-3660*/
	/*if (GetVisaField(VISA_TERMINAL_ADR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_TERMINAL_ADR, IsoInfo, tmpField, tmpFieldLen);*/
	if (GetVisaField(VISA_TERMINAL_ADR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		VisaToIsoAcceptorName(nIndexCtx ,tmpField,sIsoTermAddr);
        AddIsoField(ISO_TERMINAL_ADR, IsoInfo, sIsoTermAddr, tmpFieldLen);

	}
	/*PLUTONL-3660*/	

	/* Field 45: TrackI Data */
	if (GetVisaField(VISA_T1_DATA, VisaInfo, T1_Data, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_T1_DATA, IsoInfo, T1_Data, strlen(T1_Data));


	/* Field 48: Additional Private Data, it contains VISA fields 44 & */
	/* 48 in TLV format. */
	InitTlvPrivate(&kTlvInfo);
	AddTlvPrivate(HPS_ACQUIRER_BANK_TAG, &kTlvInfo, AcquirerBank, 6);
	/*Start IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	if (GetVisaField(VISA_SUPPORT_INFO, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaSuppInfTlv);

			if( GetTlvInfoField (B1_F125_ADD_ORIGINAL_DATA, &kVisaSuppInfTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				InitVisaAddOriginalDataTlv(&kVisaTlvInfo);	
				AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTlvInfo);

				if( GetTlvInfoField (B1_F125T03_ORIGINAL_TRA_ID, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
				{
					AddTlvPrivate(ORIGINAL_TRX_ID, &kTlvInfo, tmpField, tmpFieldLen);	
				}
			}
			/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
			if( GetTlvInfoField (B1_F125_TOKEN_DEVICE, &kVisaSuppInfTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				InitTokenDevice(&kVisaTlvInfo);
				AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTlvInfo);
				if( GetTlvInfoField (B1_F125_T01_IP_ADDR, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
				{
					PutTlvInfoField(P7_F048S22_CUSTOMER_IP, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);
			    }
			}
			if( GetTlvInfoField (B1_F125_WALLET_PROVIDER, &kVisaSuppInfTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				InitVisaWalletProviderDataTlv(&kVisaTlvInfo);
				AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTlvInfo);
				if( GetTlvInfoField (B1_F125T02_WP_DEVICE_SCORE, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S25_WP_DEVICE_SCORE, &isoDigitizationDataTlv, tmpField, tmpFieldLen);
				}

				if( GetTlvInfoField (B1_F125T02_WP_ACCOUNT_SCORE, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S25_WP_ACCOUNT_SCORE, &isoDigitizationDataTlv, tmpField, tmpFieldLen);
				}
			}
			/*End PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
		}
	/*End IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	
	if(GetVisaField(VISA_AUDIT_NBR, VisaInfo, tmpField, &tmpFieldLen)  == SUCCES)
		AddTlvPrivate(EXTERNAL_STAN_TAG, &kTlvInfo, tmpField, 6);

	/*memcpy(sAuthId + 2, tmpField, P7_F011_LEN);*/

	/*AMER20170313 Start*/
	if ( memcmp(MerchantType,MCC_AFD,P7_F018_LEN) == 0 || memcmp(MerchantType,MCC_SERVICE_STATIONS,P7_F018_LEN) == 0)	/*AMER20171128 PROD00049633: Add Service stations MCC*/
	{
		if (GetVisaField(VISA_REFERENCE_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 6.1.2*/
		{
			memcpy(sAfdRefNbr,tmpField,tmpFieldLen);
			AddTlvPrivate(AFD_REF_NBR, &kTlvInfo, sAfdRefNbr, tmpFieldLen);
		}
	}
	/*AMER20170313 End*/

	VisaBuildAuthId(VisaInfo,sAuthId);
	AddTlvPrivate(AUTHORIZATION_IDENTIFIER, &kTlvInfo, sAuthId, strlen(sAuthId));
	if (GetCpsSubField(CPS_TRANS_ID, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		strncpy(sTrnId,tmpField,sizeof(sTrnId));
		AddTlvPrivate(TRANSACTION_IDENTIFIER, &kTlvInfo, sTrnId, strlen(sTrnId));
	}


	/* IWK */
	if (GetVisaField(VISA_PIN_DATA, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		memset(IwkLmk, 0, sizeof(IwkLmk));
		if (GetIWK(nIndexCtx, ResourceId, IwkLmk) == OK)
			AddTlvPrivate(ENCR_PIN_BLOCK_KEY_TAG, &kTlvInfo, IwkLmk, strlen(IwkLmk));
	}



	memset(sPufData, 0, sizeof(sPufData));
	if (GetVisaPufSubField(VPUF_CVV2, &VisapufInfo, sPufData, &nLength) == SUCCES &&
		sPufData[0] == B1_F126_P10_CVV2_P) 
	{
		AddTlvPrivate(CVC2_DATA_TAG, &kTlvInfo, sPufData + 3, 3);
	}

	/*if ( GetVisaPufSubField(VPUF_RECURRING , &VisapufInfo, sPufData, &nLength ) == SUCCES )
	{
		AddTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , sPufData , nLength );
	}*/

	/*Start IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	if ( GetVisaPufSubField(VPUF_RECURRING , &VisapufInfo, sPufData, &nLength ) == SUCCES)
	{
		if (GetTlvInfoField (B1_F34_ACCEPTANCE_ENV_ADD_DATA, &kVisaElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			InitVisaAccEnvAddDataTlv(&kVisaTlvInfo);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTlvInfo);
			GetTlvInfoField (B1_F34T02_INITIATING_PARTY_ID, &kVisaTlvInfo, sInitParty, &tmpFieldLen);
		}
		if(sPufData[0] == B1_F126_P13_RECURRING)
		{
			RecurringFlag[0] = P7_F048_P71_RECURRING;
			PutTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , RecurringFlag , 1);
			if (sInitParty[0] != B1_F048_P64_P4_MIT)
			{
				sAddAuthInfo[P7_F048_P64_CRDNTL_REASON] = P7_F048_P64_P7_RECURRING;
				tagPresence = 1;
			}
		}
		else if(sPufData[0] == B1_F126_P13_INSTALLMENT)
		{
			RecurringFlag[0] = P7_F048_P71_INSTALLMENT;
			PutTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , RecurringFlag , 1);
			if (sInitParty[0] != B1_F048_P64_P4_MIT)
			{
				sAddAuthInfo[P7_F048_P64_CRDNTL_REASON] = P7_F048_P64_P7_INSTALLMENT;
				tagPresence = 1;
			}
		}
		else if(sPufData[0] == B1_F126_P13_CREDENTIAL)
		{
			if (sInitParty[0] == B1_F048_P64_P4_MIT)
			{
				RecurringFlag[0] = P7_F048_P71_CRED_ON_FILE;
				PutTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , RecurringFlag , 1);
			}
		}
	} 
	/*End IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/

	if (GetVisaPufSubField(VPUF_CAVV, &VisapufInfo, sCAVV, &tmpFieldLen)== SUCCESS)
	{
		AddTlvPrivate (CAVV_TAG, &kTlvInfo, sCAVV, strlen(sCAVV));
	}

	/*EBE:ECI*
	if(strlen(AddPos) >= 10 )
	{
		if(memcmp(AddPos + 8, "02",2) == 0 )
		{
			PutTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , "R" , 1 );
		}
		if(memcmp(AddPos + 8, B1_F060_POS8_INSTALL_PAY,2) == 0 )	/*AMER20171123: PROD00049633*
		{
			RecurringFlag[0] = P7_F048_P71_INSTALLMENT;
			PutTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , RecurringFlag , 1);
		}
		/*AMER20180309 PROD00054205 Enh18.Q2 Art 1.1: We don't need to fill in ECOM_AUTH_INFO, 
		because it is used mainly for the downgrade reason received as acquirer*/
		/*else if(memcmp(AddPos + 8, "05",2) == 0 )
		{
			sECI[0]=P7_ECI_AUTHENTICATED;
		}
		else if(memcmp(AddPos + 8, "06",2) == 0 )
		{
			sECI[0]=P7_ECI_NON_AUTHEN_ATTEMPT;
		}
		else if(memcmp(AddPos + 8, "07",2) == 0 )
		{
			sECI[0]=P7_ECI_NON_AUTHENTICATED;
		}
		else if(memcmp(AddPos + 8, "08",2) == 0 )
		{
			sECI[0]=P7_ECI_NON_SECURE;
		}
		else
		{
			sECI[0]=P7_ECI_UNSPECIFIED;  
		}
		PutTlvPrivate ( ECOM_AUTH_INFO , &kTlvInfo  , sECI , strlen(sECI) );*/
	/*}
	/*: AMA :*/

	if( strlen(AddPos) >= 12 )
	{
		if( AddPos[11] == '1' || AddPos[11] == '3' )
			sPosExtData[0]='1';
		else
			sPosExtData[0]='0';

		sPosExtData[1]='0';
		/*PutTlvPrivate ( POS_DATA_EXT_TAG , &kTlvInfo  , tmpField, 2 );*//*PLUTONL-3326*/
		nFlag=1;/*PLUTONL-3326*/
	}
	
	
	/*PLUTONL-3326*/
	if( AddPos[1] == '9' )
	{
		sPosExtData[P7_F048_P61_MPOS_IND_OFST]= P7_F048_P61_POS21_MPOS_TERM;
		nFlag=1;

	}
	/*PLUTONL-3326*/

	/*Start AMER20190219 PROD00065991: Enh19.Q2 Art2.1*/
	if(memcmp(MerchantType, MCC_QUASI_CASH_MER, B1_F018_LEN) == 0)
	{
		if(AddPos[3] == '7')
		{
			sAddAuthInfo[P7_F048_P64_CRYPTO_CURR_OFST] = P7_F048_P64_P3_CRPT_CURR_PUR;
		}
		else
		{
			sAddAuthInfo[P7_F048_P64_CRYPTO_CURR_OFST] = P7_F048_P64_P3_UNSPECIFIED;
		}
		tagPresence = 1;	/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	}
	/*End AMER20190219 PROD00065991: Enh19.Q2 Art2.1*/
	if (AddPos[6] == '4')
	{
		sAddAuthInfo[P7_F048_P64_TOKEN_AUTH_OSFT] = P7_F048_P64_P8_TOKEN_BASED_TRANS;
		tagPresence = 1;  
	}
	/*Start IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	if (GetVisaField(VISA_PAN_EXTENDED /*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitExempAuthInfoDataTlv(&isoExemption);
		/*AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaElectComTlv);*/   /*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/

		if( GetTlvInfoField (B1_F34_ACCEPTANCE_ENV_ADD_DATA, &kVisaElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			InitVisaAccEnvAddDataTlv(&kVisaTlvInfo);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTlvInfo);

			if( GetTlvInfoField (B1_F34T02_INITIATING_PARTY_ID, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{

				if (tmpField[0] == '1')
				{
					sAddAuthInfo[P7_F048_P64_INITIATING_PARTY] = P7_F048_P64_P4_MER_INIT;
				}
				else
				{
					sAddAuthInfo[P7_F048_P64_INITIATING_PARTY] = P7_F048_P64_P4_UNSPECIFIED;
				}
				tagPresence = 1;
			}

			/*Start AEA20210712 Enh21.Q4 Art9.1.2*/
			if( GetTlvInfoField (B1_F34T02_AUTH_OUTAGE_INDICATOR, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				sPSD2AuthExempInd[P7_F048_S27_001_AUT_OUTAGE_EXCEPTION] = tmpField[0];
				nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_AUT_OUTAGE_EXCEPTION +1);
			}
			/*End AEA20210712 Enh21.Q4 Art9.1.2*/

			/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/
			if( GetTlvInfoField (B1_F34T02_CRDNTL_MER_RELATIONSHIP_IND, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				sAddAuthInfo[P7_F048_P64_CRDNTL_MER_RELATIONSHIP_IND] = tmpField[0];
				tagPresence = 1;
			}
			/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/

			/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
			if( GetTlvInfoField (B1_F34T02_PAYFAC_STREET_ADDR, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_STREET_ADDR, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (B1_F34T02_PAYFAC_CITY_NAME, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_CITY_NAME, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (B1_F34T02_PAYFAC_STATE_CODE, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_STATE_CODE, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (B1_F34T02_PAYFAC_POSTAL_CODE, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_POSTAL_CODE, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}

			if( GetTlvInfoField (B1_F34T02_PAYFAC_CTRY_CODE, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_CTRY_CODE, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
		}

		/*Start HAL PROD00073823 20202801 Enh Q2.2020 Art 3.10*/		
		if( GetTlvInfoField (B1_F34_AUTHEN_DATA, &kVisaElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			InitVisaEcomAddDataTlv(&kVisaTlvInfo);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTlvInfo);

			if( GetTlvInfoField (B1_F34T01_PROTOCOL_VERSION_NUMBER, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{	
				if (tmpField[0] == '1')
				{
					sEcomAuthInfo[1] = P7_F048_P66_POS2_3DSEC_1_0;
				}
				else if (tmpField[0] == '2')
				{
					sEcomAuthInfo[1] = P7_F048_P66_POS2_3DSEC_2_0;
				}
				else
				{
					sEcomAuthInfo[1] = P7_F048_P66_POS2_3DSEC_UNKNOWN;
				}	
				  
				nEcomInfoLen=MAX(nEcomInfoLen,1+1); /*PLUTONL-5548 Enh 2023Q1  Visa 1.2 AHA20221221*/
			}
			/*START PLUTONL-5548 Enh 2023Q1  Visa 1.2 AHA20221221*/
			if( GetTlvInfoField (B1_F34T01_AUTHENTICATION_PROGRAM, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				if (memcmp(tmpField,VISA_DAF,2)==0 )
				{
					sEcomAuthInfo[P7_F048_P66_AUTHEN_FRMWRK_OFST] = P7_F048_P66_VISA_DAF;
				}
				else
				{
					sEcomAuthInfo[P7_F048_P66_AUTHEN_FRMWRK_OFST] = P7_F048_P66_AF_UNSPECIFED;
				}
				
				nEcomInfoLen = MAX(nEcomInfoLen,P7_F048_P66_AUTHEN_FRMWRK_OFST+1);
			}
			/*END PLUTONL-5548 Enh 2023Q1  Visa 1.2 AHA20221221*/		
			/*Start PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
			if( GetTlvInfoField (B1_F34T01_AUTHEN_DATA_QUALITY_INDC, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
               	AddTlvInfoField (P7_F048S26_AUTHEN_DATA_QUALITY_INDC , &isoEcomAuthentifData, tmpField, tmpFieldLen);
			}
			/*End PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/	
			/*Start NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
			if( GetTlvInfoField (B1_F34T01_BROWSER_IP_ADDRESS, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField (P7_F048_S29_3DS_Browser_IP_Address ,&k3DSData, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (B1_F34T01_3DS_APP_IP_ADDRESS, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField (P7_F048_S29_3DS_APP_IP_Address ,&k3DSData, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (B1_F34T01_SHIPPING_INDICATOR, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField (P7_F048_S29_SHIPPING_INDICATOR ,&k3DSData, tmpField, tmpFieldLen);
			}
			/*End NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
		}
		/*End HAL PROD00073823 20202801 Enh Q2.2020 Art 3.10*/
		/*Start NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
		if( GetTlvInfoField (B1_F34_DEVICE_PLATFORM_DATA, &kVisaElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			InitVisaDevPlatformDataTlv(&kVisaTlvInfo);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTlvInfo);

			if( GetTlvInfoField (B1_F34T06_PLATFORM_DEVICE_ID, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField (P7_F048_S29_DEVICE_ID ,&k3DSData, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (B1_F34T06_PLATFORM_DEVICE_TYPE, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField (P7_F048_S29_DEVICE_TYPE ,&k3DSData, tmpField, tmpFieldLen);
			}
		}
		if( GetTlvInfoField (B1_F34_DEVICE_COMMON_DATA, &kVisaElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			InitVisaDevCommonDataTlv(&kVisaTlvInfo);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTlvInfo);

			if( GetTlvInfoField (B1_F34T07_SDK_APP_ID, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField (P7_F048_S29_SDK_APP_ID ,&k3DSData, tmpField, tmpFieldLen);
			}
		}
		/*End NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/

		if( GetTlvInfoField (B1_F34_EU_PSD2_SCA_CSC, &kVisaElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			/*InitExempAuthInfoDataTlv(&isoExemption); 	*/
			InitVisaEuPsd2ScaCsc(&kVisaTlvInfo);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTlvInfo);
			
			/*Start ZKO16082019 PROD00070403 Enh19.Q4 Art 9.1.2*/				
			if( GetTlvInfoField (B1_F34T4A_DELG_AUTH, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				/*Start AEA20210712 Enh21.Q4 Art9.3.3*/
				if(tmpField[0] == '1')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_DELG_AUTH] = '1';
					nExempIndLen=MAX(nExempIndLen,P7_F048_S27_001_DELG_AUTH+1);
				}
				else if(tmpField[0] == '2' || tmpField[0] == '3')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_DELG_AUTH] = '1';
					nExempIndLen=MAX(nExempIndLen,P7_F048_S27_001_DELG_AUTH+1);

					sPSD2AuthExempRes[P7_F048_EXMP_RES_DELG_AUTH] = tmpField[0];
					nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_DELG_AUTH +1);
				}
				/*End AEA20210712 Enh21.Q4 Art9.3.3*/
			}
			
			/*End ZKO16082019 PROD00070403 Enh19.Q4 Art 9.1.2*/
			
			/*Start HAL 20201803 ENHS ART 9.3.3*/
			if( GetTlvInfoField (B1_F34T4A_REASON_HONOR_EXEM, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField (P7_F048_S27_REASON_HONOR_EXEM , &isoExemption, tmpField, tmpFieldLen);

			}

			if( GetTlvInfoField (B1_F34T4A_TRA_SCORE, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField (P7_F048_S27_TRA_SCORE , &isoExemption, tmpField, tmpFieldLen);

			}
			
			if( GetTlvInfoField (B1_F34T4A_TRST_MER_EXEM, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				if (tmpField[0] == '1')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_TRST_MER_EXEM] = '1';	/*AEA20210712 Enh21.Q4*/
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_TRST_MER_EXEM +1);	/*AEA20210712 Enh21.Q4*/
				}
				
				/*Start AEA20210712 Enh21.Q4*/
				else if (tmpField[0] == '2' || tmpField[0] == '3')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_TRST_MER_EXEM] = '1';
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_TRST_MER_EXEM +1);

					sPSD2AuthExempRes[P7_F048_EXMP_RES_TRST_MER] = tmpField[0];
					nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_TRST_MER +1);
				}
				/*End AEA20210712 Enh21.Q4*/
			}

			if( GetTlvInfoField (B1_F34T4A_LOW_VAL_EXEM, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				if (tmpField[0] == '1')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_LOW_VAL_EXEM] = '1';	/*AEA20210712 Enh21.Q4*/
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_LOW_VAL_EXEM +1);	/*AEA20210712 Enh21.Q4*/
				}
				/*Start PLUTONL-5548  Enh 2023Q2  Visa 9.3.1 AHA20221221*/
				else if (tmpField[0] == '2' || tmpField[0] == '3')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_LOW_VAL_EXEM] = '1';
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_LOW_VAL_EXEM +1);

					sPSD2AuthExempRes[P7_F048_EXMP_RES_LOW_VAL] = tmpField[0];  /*PLUTONL-6119 Enh 2023Q2 Visa 9.3.1 AHA20230403*/
					nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_LOW_VAL +1);  /*PLUTONL-6119 Enh 2023Q2 Visa 9.3.1 AHA20230403*/
				}
				/*End PLUTONL-5548  Enh 2023Q2  Visa 9.3.1 AHA20221221*/
			}
			if( GetTlvInfoField (B1_F34T4A_SPC, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				if (tmpField[0] == '1')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_SCP] = '1';	/*AEA20210712 Enh21.Q4*/
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_SCP +1);	/*AEA20210712 Enh21.Q4*/
				}
				/*Start PLUTONL-5548  Enh 2023Q2  Visa 9.3.1 AHA20221221*/
				else if (tmpField[0] == '2' || tmpField[0] == '3')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_SCP] = '1';
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_SCP +1);

					sPSD2AuthExempRes[P7_F048_EXMP_RES_SCP] = tmpField[0];  /*PLUTONL-6119 Enh 2023Q2 Visa 9.3.1 AHA20230403*/
					nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_SCP +1); /*PLUTONL-6119 Enh 2023Q2 Visa 9.3.1 AHA20230403*/
				}
				/*End PLUTONL-5548  Enh 2023Q2  Visa 9.3.1 AHA20221221*/
			}
			if( GetTlvInfoField (B1_F34T4A_TRA, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				if (tmpField[0] == '1')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_TRA] = '1';	/*AEA20210712 Enh21.Q4*/
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_TRA +1);	/*AEA20210712 Enh21.Q4*/
				}
				
				/*Start AEA20210712 Enh21.Q4*/
				else if(tmpField[0] == '2' || tmpField[0] == '3')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_TRA] = '1';
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_TRA +1);

					sPSD2AuthExempRes[P7_F048_EXMP_RES_TRA] = tmpField[0];
					nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_TRA +1);
				}
				/*End AEA20210712 Enh21.Q4*/

			}
		}

		if( GetTlvInfoField (B1_F34_SUPP_DATA, &kVisaElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{   
			/*InitEcomAuthentificationInfoDataTlv(&isoEcomAuthentifData); 	IBO20221018 PLUTONL-5240*/
			InitVisaSupplementalData(&kVisaBerInfo);
			AnalyseBerInfo(tmpField, tmpFieldLen, &kVisaBerInfo);
				 
            if( GetTlvInfoField (B1_F34T56_SESSION_ID, &kVisaBerInfo, tmpField, &tmpFieldLen) == SUCCESS )
			    AddTlvInfoField (P7_F048S26_ECOM_AUT_DATA_SESSION_ID , &isoEcomAuthentifData, tmpField, tmpFieldLen);
			/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/

			if(GetTlvInfoField (B1_F34T56_CAVV_VERSION_NUMBER, &kVisaBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			{
				memcpy(EcommerceData,tmpField,P7_F063_N23_CAVV_VERSION_NBR_LEN);
				nEcommerceDatalen=P7_F063_N23_CAVV_VERSION_NBR_LEN;
			}
			if( GetTlvInfoField (B1_F34T56_CAVV_TYPE, &kVisaBerInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				memcpy(EcommerceData+P7_F063_N23_CAVV_TYPE_OFST,tmpField,P7_F063_N23_CAVV_TYPE_LEN);
				nEcommerceDatalen=P7_F063_N23_CAVV_TYPE_OFST+P7_F063_N23_CAVV_TYPE_LEN;
			}
			if(GetTlvInfoField (B1_F34T56_IP_ADDR_VELOCITY_COUNT, &kVisaBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			{
				memcpy(EcommerceData+P7_F063_N23_IP_ADDR_VELC_CNT_OFST,tmpField,P7_F063_N23_IP_ADDR_VELC_CNT_LEN);
				nEcommerceDatalen=P7_F063_N23_IP_ADDR_VELC_CNT_OFST+P7_F063_N23_IP_ADDR_VELC_CNT_LEN;
			}
			if(GetTlvInfoField (B1_F34T56_DEVICE_ID_VELOCITY_COUNT, &kVisaBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			{
                memcpy(EcommerceData+P7_F063_N23_DEVICE_ID_VELC_CNT_OFST,tmpField,P7_F063_N23_DEVICE_ID_VELC_CNT_LEN);
				nEcommerceDatalen=P7_F063_N23_DEVICE_ID_VELC_CNT_OFST+P7_F063_N23_DEVICE_ID_VELC_CNT_LEN;
			}
			if((GetBerInfoField (B1_F34T56_VISA_RISK_AUTHEN_SCORE, &kVisaBerInfo, tmpField, &tmpFieldLen) == SUCCESS )||
			   (GetBerInfoField (B1_F034_T56_VCAS_SCORE, &kVisaBerInfo, tmpField, &tmpFieldLen )== SUCCESS))//*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
			{
				memcpy(EcommerceData+P7_F063_N23_VISA_RISK_BASED_AUTN_SCORE_OFST,tmpField,P7_F063_N23_VISA_RISK_BASED_AUTN_SCORE_LEN);
				nEcommerceDatalen=P7_F063_N23_VISA_RISK_BASED_AUTN_SCORE_OFST+P7_F063_N23_VISA_RISK_BASED_AUTN_SCORE_LEN;
			}
			
			/*Start NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
			if(GetBerInfoField (B1_F34T56_THIRD_PARTY_ID, &kVisaBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			{
				AddTlvInfoField (P7_F048_S29_THIRD_PARTY_ID ,&k3DSData, tmpField, tmpFieldLen);
			}
			if(GetBerInfoField (B1_F34T56_THIRD_PARTY_RISK_SCORE, &kVisaBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			{
				AddTlvInfoField (P7_F048_S29_THIRD_PARTY_RISK_SCORE ,&k3DSData, tmpField, tmpFieldLen);
			}
			/*End NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/

			/*End PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
		}



		if(nExempIndLen > 0)
		{		
			AddTlvInfoField (P7_F048_S27_AUTH_EXEMPTION_INDIC, &isoExemption, sPSD2AuthExempInd, nExempIndLen);
			/*ZKO29122021 Start PLUTONL-3345
			AddTlvPrivate(AUTH_EXEMPTION_IND, &kTlvInfo, sPSD2AuthExempInd, nExempIndLen);*/	/*AEA20210712 Enh21.Q4*/
		}

		
		if(nExempResLen > 0)
			AddTlvInfoField (P7_F048_S27_AUTH_EXEMPTION_RES , &isoExemption, sPSD2AuthExempRes, nExempResLen);
		


		
        tmpFieldLen = BuildTlvInfoBuffer (tmpField, &isoEcomAuthentifData);	/*End HAL 20201803 ENHS ART 9.3.3 /4*/
		if( tmpFieldLen > 0 )
			AddTlvPrivate(ECOM_AUTHENTIF_DATA, &kTlvInfo, tmpField, tmpFieldLen);

		tmpFieldLen = BuildTlvInfoBuffer (tmpField, &isoExemption);	/*End HAL 20201803 ENHS ART 9.3.3 /4*/	
		if( tmpFieldLen > 0 )
			AddTlvPrivate(PSD2_DATA, &kTlvInfo, tmpField, tmpFieldLen);

		tmpFieldLen = BuildTlvInfoBuffer (tmpField, &k3DSData);/*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
		if( tmpFieldLen > 0 )
			AddTlvPrivate(DATA_3DS, &kTlvInfo, tmpField, tmpFieldLen);
	
	}


	/*PLUTONL-3326*/
	if(nFlag=1)
	{
		PutTlvPrivate ( POS_DATA_EXT_TAG , &kTlvInfo  , sPosExtData,P7_F048_P61_LEN );
	}
	/*PLUTONL-3326*/

	if (tagPresence == 1)
	{	
		AddTlvPrivate(AUTH_ADD_INFO, &kTlvInfo, sAddAuthInfo, P7_F048_P64_LEN);
	}

	if (nEcomInfoLen > 0)	/*HAL PROD00073823 20202801 Enh Q2.2020 Art 3.10*/
	{
		AddTlvPrivate(ECOM_AUTH_INFO, &kTlvInfo, sEcomAuthInfo, nEcomInfoLen);
	}
	/*End IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/

	
	if (GetVisaField(VISA_ADTNL_RESP_DATA, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		/*START HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/
		if (VisaInfo->nMsgType == 120 &&  tmpFieldLen >= 4)
		{
			sExtStipReasonCode[0] = tmpField[3];
		}
		/*END HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/

		/*Start ACH20191210  PLUTONL-500 */ 
		GetIsoField (ISO_POS_DATA, IsoInfo, sIsoPosMode, &tmpFieldLen);
		int T2DataPresenceFlag= GetVisaField(VISA_T2_DATA, VisaInfo, T2_Data, &tmpFieldLen);
		int IccDataPresenceFlag= GetVisaField(VISA_ICC_DATA, VisaInfo, tmpField1, &tmpFieldLen);
		VisaToIsoAdtnlRespData(tmpField, T2DataPresenceFlag, IccDataPresenceFlag, sIsoPosMode, 0, &kTlvInfo);	
		/*End ACH20191210  PLUTONL-500 */
		
		/*Start NAB07042020 'Y' for reject and 'N' for decision matrix PLUTONL-1477*/
		if (BASE1_REJ_NON3DS == 'Y' && strlen(tmpField) > 14 && (VisaInfo->nMsgType == 100 || VisaInfo->nMsgType == 101))
		{
			/*start NAB16102018  PROD00061852*/
			if ((tmpField[14] == '0' ) || (tmpField[14] == '4' ) || (tmpField[14] == '7' ) || (tmpField[14] == '9' ) 
			||(tmpField[14] == '1' ))/*NAB21062019 Certif SG '1' CAVV failed verification�authentication*/
			{
				if(memcmp (sIsoProcCode ,P7_F003_REFUND, P7_F003_P1_LEN) != 0) /*IBO20221226 PLUTONL-5591*/
				{
					trace_event ("CAVV verification failed, nRetFlag = NOK", PROCESSING);
					nRetFlag = NOK;
				}

			}

			/*END NAB16102018*/
		}
		/*End NAB07042020 PLUTONL-1477*/
	}

	/*AMER20190527 PROD00068232: TODO put the correct mapping*/
	/*if (GetVisaField(VISA_ADTNL_DATA_PRIV, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddTlvPrivate(ADDRESS_VERIF_REQ_TYPE_TAG, &kTlvInfo, tmpField + 1, tmpFieldLen - 1);*/


	if ( GetVisaPufSubField(VPUF_ACCOUNT_RESULT , &VisapufInfo, sPufData, &nLength ) == SUCCES )
	{
		AddTlvPrivate ( ACCOUNT_RESULT_TAG , &kTlvInfo  , sPufData , nLength );
	}

	/*Start ICH See PROD00039494*/
	if ( GetVisaPufSubField(VPUF_DCC_INDICATOR , &VisapufInfo, sPufData, &nLength ) == SUCCES )
	{
		AddTlvPrivate ( DCC_DATA , &kTlvInfo  , sPufData , 1 );
	}
	/*End ICH See PROD00039494*/

	if ( GetVisaPufSubField(VPUF_ADD_AUTH_METHOD , &VisapufInfo, sPufData, &nLength ) == SUCCES )
	{
		AddTlvPrivate ( ADD_AUTH_METHOD_TAG , &kTlvInfo  , sPufData , nLength );
	}

	if ( GetVisaPufSubField(VPUF_ADD_AUTH_RES_CODE , &VisapufInfo, sPufData, &nLength ) == SUCCES )
	{
		AddTlvPrivate ( ADD_AUTH_RES_CODE_TAG , &kTlvInfo  , sPufData , nLength );
	}	


	if (GetCpsSubField (CPS_MVV , &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AddTlvPrivate (MVV_TAG, &kTlvInfo, tmpField,tmpFieldLen);
	}

	/*Start AMER20190219 PROD00065991: Enh19.Q2 Art2.4*/
	if (GetVisaField(VISA_TRANS_DESC, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTransSpecTlv);
		
		if( GetTlvInfoField (B1_F104_PAY_FAC_DATA, &kVisaTransSpecTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaPayFacTlv);
			
			if( GetTlvInfoField (B1_F104T56_PAY_FAC_ID, &kVisaPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_ID, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			
			if( GetTlvInfoField (B1_F104T56_SUB_MER_ID, &kVisaPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_SUB_MER_ID, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			
			if( GetTlvInfoField (B1_F104T56_ISO_ID, &kVisaPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				PutTlvInfoField(P7_F048P21_ISO_ID, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}		
		}
		
	}



	/*End AMER20190219 PROD00065991: Enh19.Q2*/
	
	/*Start AMER20190723 PROD00070511*/
	if (GetCpsSubField(CPS_RISK_INFO, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AddTlvPrivate (FRAUD_SCORE, &kTlvInfo, tmpField, 2);
	}
	/*End AMER20190723 PROD00070511*/
	if( memcmp(sIsoProcCode,P7_F003_PAYMENT,P7_F003_P1_LEN) == 0 )
	{
		/*VisaToIsoMoneyTransData(VisaInfo,&kIsoMoneyTransfTlv,sBAI);*/   
		VisaToIsoMoneyTransData(VisaInfo,&kIsoMoneyTransfTlv,&kIsoPayFacTlv,sBAI);   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
		if (GetVisaField(VISA_ADTNL_DATA_PRIV, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS &&
			memcmp(tmpField,"OCT",3) == 0 )
		{
			AddTlvPrivate (WATCH_LIST_SCORING_RES, &kTlvInfo, tmpField+7,3);
		}
		if( memcmp(sBAI,B1_F104T57_AA,B1_F104T57_LEN) == 0 || memcmp(sBAI,B1_F104T57_PP,B1_F104T57_LEN) == 0 )
		{
			AddTlvPrivate (PAYMENT_AUTH_INFO, &kTlvInfo, "Y",1);
		}
		else
		{
			AddTlvPrivate (PAYMENT_AUTH_INFO, &kTlvInfo, "N",1);
		}

	}
	/*Start IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kIsoPayFacTlv);
	if( tmpFieldLen > 0 )
	{
		AddTlvPrivate(MERCHANT_ADDITIONAL_DATA, &kTlvInfo, tmpField, tmpFieldLen);
	}	
	/*End IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
	/*Start ACH20191010 */

	/*Start HAL2402 PLUTONL-4391*/
	if (GetVisaField(VISA_ADR_VERIF_DATA, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{

		AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaVerifDataTlv);

		if (GetTlvInfoField(B1_F123_ADDR_VERIF_DATA, &kVisaVerifDataTlv, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AnalyseTlvInfo(tmpField, tmpFieldLen, &visaAddVerifMappingData);
			
			sAvsRequest[0] = P7_F048_P82_ADR_VERIF_REQ;
			AddTlvPrivate(ADDRESS_VERIF_REQ_TYPE_TAG, &kTlvInfo, sAvsRequest, 1);
			
			memset(sAvsData, ' ', sizeof(sAvsData));
			sAvsData[0] = '1';

			if (GetTlvInfoField(B1_F123_T66_POSTAL_CD, &visaAddVerifMappingData, tmpField, &tmpFieldLen) == SUCCESS)
			{
				nLenZipCode = tmpFieldLen > 9 ? 9 : tmpFieldLen;
				memcpy(sAvsData + P7_F048_S21_FORMAT_LEN, tmpField, nLenZipCode);
				tagPresence = 1;
			}

			if (GetTlvInfoField(B1_F123_T66_STREE_ADDR, &visaAddVerifMappingData, tmpField, &tmpFieldLen) == SUCCESS)
			{
				memcpy(sAvsData + P7_F048_S21_FORMAT_LEN + P7_F048_S21_ZIP_CODE_LEN, tmpField, tmpFieldLen);
				tagPresence = 1;
			}
			/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
			if (GetTlvInfoField(B1_F123_T66_CH_SHIP_HASH, &visaAddVerifMappingData, tmpField, &tmpFieldLen) == SUCCESS)
			{
				AddTlvInfoField(P7_F048S22_CH_SHIP_HASH, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);	
			}
			/*End PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/

			if (tagPresence == 1)
			{
				AddTlvPrivate(ADRESS_VERIFICATION_DATA, &kTlvInfo, sAvsData, P7_F048_S21_FORMAT_LEN + P7_F048_S21_ZIP_CODE_LEN + P7_F048_S21_ADR_LEN);
			}
		}


		if (GetTlvInfoField(B1_F123_TOKEN_DATA, &kVisaVerifDataTlv, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AnalyseTlvInfo(tmpField, tmpFieldLen, &visaPanMappingData);

			if (GetTlvInfoField(B1_F123_T68_TOKEN, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TOKEN, &kTokenDataIso, tmpField, tmpFieldLen);
			
			if (GetTlvInfoField(B1_F123_T68_TKN_ASS_LVL, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_ASSURANCE_LVL, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_REQ_ID, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_REQ_ID, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_RANGE, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_PAN, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_REFERENCE_ID, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_PAN_REF_ID, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_EXPIRATION_DATE, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_EXP_DT, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_TYPE, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_TYPE, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_STATUS, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_STATUS, &kTokenDataIso, tmpField, tmpFieldLen);
			/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/
			if (GetTlvInfoField(B1_F123_T68_AUTO_FILL_IND, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_AUTO_FILL, &kTokenDataIso, tmpField, tmpFieldLen);
			/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/

			/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
			if (GetTlvInfoField(B1_F123_T68_TKN_AUTHENTICATION_AMNT, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
			{
				/* En23Q2-Hotfix-PLUTONL-6134 */ 				
				nLen = HexToAscii(tmpField, szFieldAdd, tmpFieldLen*2); 
				int_temp = strtol(szFieldAdd, NULL, 16);
				memset(tmpField, 0, sizeof(tmpField));
				sprintf(tmpField, "%012d", int_temp);

				AddTlvInfoField(P7_F048S22_PURCHASE_AMOUNT, &isoSaleInfoDataPropTlv, tmpField, 12);
				/*AddTlvInfoField(P7_F048S22_PURCHASE_AMOUNT, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);*/

				/* En23Q2-Hotfix-PLUTONL-6134 */ 
			}
			/*End PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
		}

				/*
		Comented because field 123 changer from Usage-1Verification Data (Fixed Format)
		to Usage 2—Verification & Token Data (TLV Format)
		*/

		/*sAvsRequest[0]= P7_F048_P82_ADR_VERIF_REQ;
		AddTlvPrivate ( ADDRESS_VERIF_REQ_TYPE_TAG , &kTlvInfo  , sAvsRequest, 1 );

		int nLenZipCode;
		int nLenAddr;
		int nLenTotal;
		nLenTotal = 0;
		memset(sAvsData, ' ', sizeof(sAvsData));
		sAvsData[0]='1';

		nLenZipCode = tmpFieldLen > 9 ? 9: tmpFieldLen;
		memcpy(sAvsData + P7_F048_S21_FORMAT_LEN,tmpField, nLenZipCode);

		if( tmpFieldLen > 9 )
		{
			nLenAddr = tmpFieldLen - 9;
			memcpy(sAvsData + P7_F048_S21_FORMAT_LEN + P7_F048_S21_ZIP_CODE_LEN, tmpField + 9 ,nLenAddr);

		}

		AddTlvPrivate ( ADRESS_VERIFICATION_DATA , &kTlvInfo  , sAvsData, P7_F048_S21_FORMAT_LEN + P7_F048_S21_ZIP_CODE_LEN + P7_F048_S21_ADR_LEN);
		*/

		/* If TLV format*/
		/*InitB1VerifDataTlvInfo(&b1VerifDataTlv);
		AnalyseTlvInfo(tmpField, tmpFieldLen, &b1VerifDataTlv);
		if( GetTlvInfoField (B1_F123_ADDR_VERIF_DATA, &b1VerifDataTlv, tmpField, &tmpFieldLen) == SUCCESS ){

			InitB1AddrVerifDataTlvInfo(&b1AddrVerifDataTlv);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &b1AddrVerifDataTlv);
			if( GetTlvInfoField (B1_F123_T66_POSTAL_CD, &b1AddrVerifDataTlv, tmpField, &tmpFieldLen) == SUCCESS )
				memcpy(sAvsData + P7_F048_S21_FORMAT_LEN,tmpField, 9);/*ZIP CODE* /
			if( GetTlvInfoField (B1_F123_T66_STREE_ADDR, &b1AddrVerifDataTlv, tmpField, &tmpFieldLen) == SUCCESS )
				memcpy(sAvsData + P7_F048_S21_FORMAT_LEN,tmpField, 40);/*ADDRESS* /


		}*/
	}
	/*End HAL2402 PLUTONL-4391*/
	/*End ACH20191010 */

	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kIsoMoneyTransfTlv);
	if( tmpFieldLen > 0 )
	{
		AddTlvPrivate(MONEY_TRANSFER_DATA, &kTlvInfo, tmpField, tmpFieldLen);
	}
	/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &isoSaleInfoDataPropTlv);
	if (tmpFieldLen > 0)
		PutTlvPrivate(SOLUTION_IDENTIFIER_DATA, &kTlvInfo, tmpField, tmpFieldLen);
	/*End PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	
	/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &isoDigitizationDataTlv);
	if (tmpFieldLen > 0)
		PutTlvPrivate(DIGITIZATION_DATA, &kTlvInfo, tmpField, tmpFieldLen);
    /*End PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/

	/*Start HAL2402 PLUTONL-4391*/
	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kTokenDataIso);
	if (tmpFieldLen > 0)
		PutTlvPrivate(TOKEN_DATA, &kTlvInfo, tmpField, tmpFieldLen);
	/*End HAL2402 PLUTONL-4391*/

	tmpFieldLen = PrivateBuildTlv(tmpField, &kTlvInfo);
	if (tmpFieldLen > 0)
		AddIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, tmpFieldLen);

	/* Field 49: Transaction Currency Code */
	GetVisaField(VISA_TRANS_CRNCY, VisaInfo, sTrxCurrency, &tmpFieldLen);
	AddIsoField(ISO_TRANS_CRNCY, IsoInfo, sTrxCurrency, 3);

	/* Field 50: Settlement Currency Code */
	if (GetVisaField(VISA_BILLING_CRNCY, VisaInfo, sBillingCurrency, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_STLMNT_CRNCY, IsoInfo, sBillingCurrency, 3);
	else if( strlen(sTrxCurrency) > 0 ) 
		AddIsoField(ISO_STLMNT_CRNCY, IsoInfo, sTrxCurrency, 3);

	/* Field 51: Cardholder Billing Currency Code */
	if (strlen(sBillingCurrency) > 0)
		AddIsoField(ISO_BILLING_CRNCY, IsoInfo, sBillingCurrency, 3);
	else if( strlen(sTrxCurrency) > 0 )
		AddIsoField(ISO_BILLING_CRNCY, IsoInfo, sTrxCurrency, 3);

	/* Field 52: Personal Identification Number Data */
	if (GetVisaField(VISA_PIN_DATA, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_PIN_DATA, IsoInfo, tmpField, 16);

	/* Field 53: Security Data */
	memset(tmpField1, 0, sizeof(tmpField1));
	if (GetVisaField(VISA_PIN_DATA, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		PutSecurityData(SECURITY_FORMAT, tmpField1, "02", 2);
		PutSecurityData(PIN_BLOCK_FORMAT, tmpField1, "01", 2);
	} 
	else 
	{
		PutSecurityData(SECURITY_FORMAT, tmpField1, "00", 2);
		PutSecurityData(PIN_BLOCK_FORMAT, tmpField1, "99", 2);
	}

	PutSecurityData(PIN_ENCRYPTION_KEY_INDEX, tmpField1, "000", 3);
	PutSecurityData(MAC_KEY_INDEX, tmpField1, "000", 3);

	memset(sPid, 0, sizeof(sPid));
	sprintf(sPid, "%08d", getpid());
	PutSecurityData(SOURCE_PID, tmpField1, sPid, 8);
	PutSecurityData(SOURCE_RESOURCE, tmpField1, ResourceId, 6);
	PutSecurityData(SOURCE_NODE_ID, tmpField1, NodeId, 4);
	PutSecurityData(DESTINATION_PID, tmpField1, "00000000", 8);
	PutSecurityData(DESTINATION_RESOURCE, tmpField1, "000000", 6);
	PutSecurityData(DESTINATION_NODE_ID, tmpField1, "0000", 4);
	PutSecurityData(USER, tmpField1, "VISA           ", 15);
	PutSecurityData(PURGE_TIME, tmpField1, "000000000", 9);
	AddIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField1, strlen(tmpField1));

	/* START MOD CFE PROD00045718 20170828 */
	/*: AMA (54) Additional Amount :*/
	memset(tmpField1, 0, sizeof(tmpField1));
	VisaToIsoAddAmount(VisaInfo,tmpField1);
	if ( strlen(tmpField1) > 0) 
			AddIsoField(ISO_ADTNL_AMOUNTS, IsoInfo, tmpField1, strlen(tmpField1));

	/*: END-AMA :*/
  /* END MOD CFE PROD00045718 20170828 */
	/* Field 55: Chip Data */

	if (GetVisaField(VISA_ICC_DATA, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS && tmpFieldLen > 3 ) 
	{

		dump_buffer(tmpField, tmpFieldLen, 'A', 'X', 'X');
		/*Start IBO20220801 PLUTONL-4969*/
		TSTagVisa      VisaIccInfo;
		char           sIccBuffer[BUF_LG_MAX];

		InitVisaIcTag(&VisaIccInfo);
		InitIsoIcTag(&IsoIccInfo);
		memset(sIccBuffer, 0, sizeof(sIccBuffer));

		AnalyseVisaIc  ( tmpField, tmpFieldLen , &VisaIccInfo );

		if(GetVisaIcTag  (VISA_TAG_SEC_PIN_BLOCK , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			PutIsoField(ISO_ACQR_REFERENCE_DATA, IsoInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_APP_INTER_PROFILE , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_APP_INTER_PROFILE, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_DED_FILE_NAME , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_DED_FILE_NAME, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_TVR , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TVR, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_TRANS_DATE , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TRANS_DATE, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_TRANS_TYPE , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TRANS_TYPE, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_TRANS_CUR_CODE , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TRANS_CUR_CODE, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_TRANS_AMOUNT , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TRANS_AMOUNT, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_OTHER_AMOUNT , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_OTHER_AMOUNT, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_ISS_APP_DATA , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_ISS_APP_DATA, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_TERM_COUNTRY_CODE , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TERM_COUNTRY_CODE, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_APP_CRYPTOGRAM , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_APP_CRYPTOGRAM, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_TERM_CAP , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TERM_CAP, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_ATC , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_ATC, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_UNPRED_NUMBER , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_UNPRED_NUMBER, &IsoIccInfo, FieldData, FieldLen);
		if(GetVisaIcTag  (VISA_TAG_FFI , &VisaIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_FFI, &IsoIccInfo, FieldData, FieldLen);

		tmpFieldLen = IsoBuildIcFld(sIccBuffer, &IsoIccInfo);
		if (tmpFieldLen > 0)
			AddIsoField(ISO_ICC_DATA, IsoInfo, sIccBuffer, tmpFieldLen);

		/*End IBO20220801 PLUTONL-4969*/

		/*NAB12032020 Put the new pin data received from base1 at F55.C0 to the ISS filed 31 --SIA*/
		/*
		AnalyseIsoIc(tmpField + 3, tmpFieldLen - 3 , &IsoIccInfo);
		memset(FieldData, 0, sizeof(FieldData));*/

		/*if(GetIsoIcTag (ISO_TAG_SEC_PIN_DATA, &IsoIccInfo, FieldData, &FieldLen ) == SUCCES )
			PutIsoField(ISO_ACQR_REFERENCE_DATA, IsoInfo, FieldData, FieldLen);*/
		/*NAB12032020*/
		/*AddIsoField(ISO_ICC_DATA, IsoInfo, tmpField + 3, tmpFieldLen - 3);*/
	}

	if( memcmp(sFunctionCode, P7_F024_PRE_AUTH, P7_F024_LEN) == 0 && strlen(sAuthCycleCode) > 0 )
	{
		AddIsoField(ISO_AUTHORIZATION_CYCLE_CODE, IsoInfo, sAuthCycleCode, P7_F057_LEN);
	}

	/* Field 60: Additional POS Data */
	InitTlvPrivate(&kTlvInfo);
	if (strlen(AddPos) > 2)	/* Chip condition code */
		AddTlvPrivate(CHIP_CONDITION_CODE, &kTlvInfo, AddPos + 2, 1);
	if (strlen(AddPos) > 6)	/* Chip transaction indicator */
		AddTlvPrivate(CCPS_TRANSACTION_IND, &kTlvInfo, AddPos + 6, 1);
	if (strlen(AddPos) > 7)	/* Chip authentication reliability indicator */
		AddTlvPrivate(CARD_AUTHEN_RELIABIL_IND, &kTlvInfo, AddPos + 7, 1);
	if (tmpFieldLen = PrivateBuildTlv(tmpField, &kTlvInfo))
		AddIsoField(ISO_ADTNL_POS_INFO, IsoInfo, tmpField, tmpFieldLen);

	/* Field 61: contains VISA fields 22, 25, & 60 in TLV format */
	InitTlvPrivate(&kTlvInfo);
	field_61_presence = 0;
	if (strlen(PosMode) > 0) 
	{
		field_61_presence = 1;
		AddTlvPrivate(POS_ENTRY_MODE_TAG, &kTlvInfo, PosMode, 4);
	}
	if (strlen(PosCond) > 0) 
	{
		field_61_presence = 1;
		AddTlvPrivate(POS_CONDITION_CODE_TAG, &kTlvInfo, PosCond, 2);
	}
	if (strlen(AddPos) > 0) 
	{
		field_61_presence = 1;
		AddTlvPrivate(ADTNL_POS_DATA_TAG, &kTlvInfo, AddPos, AddPosLen);
	}
	if (field_61_presence == 1) 
	{
		tmpFieldLen = PrivateBuildTlv(tmpField, &kTlvInfo);
		AddIsoField(ISO_NET_POS_DATA, IsoInfo, tmpField, tmpFieldLen);
	}

	/* Field 62: CPS Data */
	if (GetVisaField(VISA_CPS_FIELDS, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		InitTlvPrivate(&kTlvInfo);

		if (GetCpsSubField(CPS_ACI, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddTlvPrivate(CPS_ACI_TAG, &kTlvInfo, tmpField, 1);

		if (GetCpsSubField(CPS_TRANS_ID, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddTlvPrivate(CPS_TRANS_ID_TAG, &kTlvInfo, tmpField, 15);

		if (GetCpsSubField(CPS_VALIDATION_CODE, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddTlvPrivate(CPS_VALIDATION_DATE_TAG, &kTlvInfo, tmpField, tmpFieldLen);

		if (GetCpsSubField (CPS_MVV , &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AddTlvPrivate(CPS_MVV_TAG ,&kTlvInfo,tmpField,tmpFieldLen);
		}
		if (GetCpsSubField (CPS_PRODUCT_ID , &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AddTlvPrivate(CPS_PRODUCT_ID_TAG ,&kTlvInfo,tmpField,tmpFieldLen);
		}  

		tmpFieldLen = PrivateBuildTlv(tmpField, &kTlvInfo);
		AddIsoField(ISO_CPS_FIELDS, IsoInfo, tmpField, tmpFieldLen);
	}

	InitTlvPrivate(&kTlvInfo);
	
	/*Start AMER20171004 PROD00046795 Enh17.Q4*/
	if (VisaInfo->nMsgType == 120 || VisaInfo->nMsgType == 121)
	{
      	if ( GetBitMappedInfoField (B1_F063_STIP_SWITCH_RSN_CD, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
        {	
		    AddTlvPrivate(NET_STIP_REASON_CODE_TAG, &kTlvInfo, tmpField, tmpFieldLen);
	    }

		/*Start HAL PROD00073823 20200402 Enh Q2.2020 Art 1.1*/
    	if ( GetBitMappedInfoField (B1_F063_MSG_RSN_CODE, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
        {
	        if( memcmp(tmpField,MSG_REASON_COMPLETION_ADVICE,VISA_F063_RSN_CD_LEN) == 0 )
	        {
				        
		        memcpy(sFunctionCode,P7_F024_AUTH_PREV_AMNT_DIFF, P7_F024_LEN);
		        PutIsoField(ISO_FUNCTION_CODE, IsoInfo, sFunctionCode,P7_F024_LEN);    
	        }
		}
		/*Start HAL PROD00073823 20200402 Enh Q2.2020 Art 1.1*/
	}

	if( memcmp(sIsoProcCode,P7_F003_PAYMENT,P7_F003_P1_LEN) == 0 )
	{
		if (GetVisaField(VISA_ADTNL_DATA_PRIV, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS &&
			memcmp(tmpField,"OCT",3) == 0 )
		{
			AddTlvPrivate (NET_VISA_ACT_CHK_RES, &kTlvInfo, tmpField+10,1);
		}

		/*tmpFieldLen = PrivateBuildTlv(tmpField, &kTlvInfo);
		if( tmpFieldLen > 0 )
		{
			AddIsoField(ISO_NETWORK_DATA, IsoInfo, tmpField, tmpFieldLen);
		}*/
	}
    
	/*Start HAL 20201803 ENHS 2020Q2 ART 9.3.3*/
	if (GetVisaPufSubField(VPUF_MER_ID, &VisapufInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		AddTlvPrivate (NET_VISA_MERC_ID, &kTlvInfo, tmpField,tmpFieldLen);
	}
	/*Start HAL 20201803 ENHS 2020Q2 ART 9.3.3*/

	/*Start HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/
	if (strlen(sExtStipReasonCode) > 0)
	{
		AddTlvPrivate (NET_VISA_EXT_STIP_REASON_CODE, &kTlvInfo, sExtStipReasonCode, 1);
	}
	/*End HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/
	
	/*Start IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	if (GetVisaField(VISA_ADTNL_RESP_DATA, VisaInfo, FieldData, &FieldLen) == SUCCES)
	{
		sTokenResponseVisa[0]=FieldData[2];
		AddTlvPrivate (NET_TOKEN_PROGAM, &kTlvInfo, sTokenResponseVisa ,1);
	}
	/*End IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/

	/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.1*/

	if( GetTlvInfoField (B1_F125_WALLET_PROVIDER, &kVisaSuppInfTlv, tmpField, &tmpFieldLen) == SUCCESS )
	{
		InitVisaWalletProviderDataTlv(&kVisaTlvInfo);
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTlvInfo);
		if( GetTlvInfoField (B1_F125T02_OVERALL_ASSESSMENT, &kVisaTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvPrivate(NET_VISA_OVERALL_ASSESSMENT_SECURE_ECOM, &kTlvInfo, tmpField, tmpFieldLen);
		}
	}
	/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.1*/

	/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art8.3.1*/
	if( GetCpsSubField (CPS_PURCHASE_IDENTIFIER, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS )
	{
		AddTlvPrivate (NET_VISA_PURCHASE_IDENTIFIER, &kTlvInfo, tmpField,tmpFieldLen);
	}
	/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art8.3.1*/
	/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	if(nEcommerceDatalen > 0)
	{
		AddTlvPrivate(NET_VISA_ECOM_DATA , &kTlvInfo, EcommerceData, nEcommerceDatalen);
	}
	/*End PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/

	/*Start PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	if( GetTlvInfoField (B1_F104_BUSINESS_APP_ID, &kVisaTransSpecTlv, tmpField, &tmpFieldLen) == SUCCESS )
	{ 
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaAppIdentTlv);
		memset(tmpField, '\0', sizeof(tmpField));
		if( GetTlvInfoField (B1_F104_T57_MAX_PROC_DATE, &kVisaAppIdentTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			memset(tmpField1, '\0', sizeof(tmpField1));
			julian_to_gmt(tmpField,tmpField1);
			AddTlvPrivate(NET_MAX_PROC_DATE_TAG, &kTlvInfo, tmpField1,strlen(tmpField1));
		}
    }
	/*End PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	
	memset(tmpField, 0, sizeof(tmpField));
	tmpFieldLen = PrivateBuildTlv(tmpField, &kTlvInfo);
	
	if( tmpFieldLen > 0 )
	{
		AddIsoField(ISO_NETWORK_DATA, IsoInfo, tmpField, tmpFieldLen);
	}

	/* Ajout Du Champs 100  pour mettre la banque acquirer */
	AddIsoField(ISO_RECEIVER_ID, IsoInfo, AcquirerBank, 6);

	/*Start NAB07042020 PLUTONL-1477*/
	if (nRetFlag  !=OK)
	{ 	
		/*InsertErrorMsg( IsoInfo ,BAD_CVV_MSG , strlen(BAD_CVV_MSG));*/ 
		PutIsoField(ISO_RESPONSE_CODE, IsoInfo, P7_F039_DECLINE, P7_F039_LEN );
		
		AuthorLog(	nIndexCtx ,
			IsoInfo ,
			ResourceId ,
			STAND_IN_RESOURCE ,
			"ADM"
			);
		trace_event ("End  BuildAutReqFromNw(NOK)", PROCESSING);

		return(nRetFlag) ;
	}
    /*Start NAB16102018 PROD00061852*/
	/*if (nRetFlag != OK)
	{
		PutIsoField(ISO_RESPONSE_CODE, IsoInfo, P7_F039_DECLINE, P7_F039_LEN );*/
		/*InsertErrorMsg( IsoInfo ,sErrMsg , strlen(sErrMsg));*/
	/*}*/
	/*End NAB07042020 PLUTONL-1477*/
	dump_p7_struct(IsoInfo);

	memset(tmpField, 0, sizeof(tmpField));
	memset(tmpField1, 0, sizeof(tmpField1));
	memset(Pan, 0, sizeof(Pan));
	memset(ExpiryDate, 0, sizeof(ExpiryDate));
	memset(CVV, 0, sizeof(CVV));
	memset(PVV, 0, sizeof(PVV));
	memset(T1_Data, 0, sizeof(T1_Data));
	memset(T2_Data, 0, sizeof(T2_Data));
	memset (sCAVV,      	 0, sizeof(sCAVV));
	memset (sXID,      	 0, sizeof(sXID));
	memset (tmpField2, 0, sizeof(tmpField2));
	memset (tmpField3, 0, sizeof(tmpField3));
 	memset(sEcomAuthInfo, ' ', P7_F048_P66_LEN);
	
	InitTlvPrivate(&kTlvInfo);
	InitVisaPufInfo(&VisapufInfo);
	InitCpsInfo(&CpsInfo);
	IniIsoMoneyTransDataTlv(&kIsoMoneyTransfTlv);
	IniIsoPayFacDataTlv(&kIsoPayFacTlv);
	InitVisaTransSpecDataTlv(&kVisaTransSpecTlv);
	InitVisaPayFacDataTlv(&kVisaPayFacTlv);
	InitVisaVIPPufDataTlv(&kVIPufData);
	InitPufInfo (&kPufInfo);
	InitVisaElectComDataTlv(&kVisaElectComTlv);	

	/*trace_event("End  BuildAutReqFromNw(OK)", PROCESSING);*/   
	/*return (OK); */ 
	/*End NAB16102018 PROD00061852 */
	sprintf(sLine, "End  BuildAutReqFromNw(%d)", nRetFlag);
	trace_event(sLine, PROCESSING);
	return (nRetFlag);
}
