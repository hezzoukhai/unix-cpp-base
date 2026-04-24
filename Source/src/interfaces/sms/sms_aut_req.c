#include <iso_hps.h>
#include <iso_sms.h>
#include <iso_ictag.h>
#include <iso_cps.h>
#include <iso_puf.h>
#include <tlv_private.h>
#include <tlv_autho.h>
#include <sms_puf_fields.h> /* AMA */
#include <sms_param.h>
#include <sms_fields.h>
#include <define.h>
#include <list_thr.h>
#include <queue.h>
#include <struct_line.h>
#include <hsm_inc.h>
#include <security_data.h>
#include <sms_ictag.h>
#include <p7_sid.h>
/*#include <visa_define.h>*/
#include <p7_define.h>
#include <sms_define.h>
#include <p7_common.h>
#include <p7_globals.h>
#include <iso8583.h>
#include <sms_puf.h> /* AMA */
#include <nw_raw_msg.h>
#include <msg_flow_mapping.h>
/*************************************************************************/
/* FCT_AUT_REQ_FROM_NW()                                               */
/* */
/* Processes a SMS authorization request (message 0200)              */
/* It calls the function to convert the message into an ISO format     */
/* structure, it then calls the pre-control module, then depending     */
/* on the action code it either sends the response message to VISA     */
/* or passes the request on to the server fo authorization.            */
/* Input:  SmsInfo: received message                                  */
/* nIndexCtx: Oracle context                                   */
/*************************************************************************/
int
	FCT_AUT_REQ_FROM_NW(int nIndexCtx, TSSmsInfo * SmsInfo)
{
	TSSmsInfo      NewSmsInfo;
	TSIsoInfo       IsoInfo;
	char            sTlvBuffer[LG_MAX];
	char            ProcessingCode[6 + 1];
	int             Length, result;
	char			sAutTabInd[3 + 1];


	trace_event("Start  FCT_AUT_REQ_FROM_NW()", PROCESSING);

	memset (sAutTabInd, '\0', sizeof(sAutTabInd));

	result = BuildAutReqFromNw ( nIndexCtx , SmsInfo, &IsoInfo);
	if(result != OK)
	{
		AutReplyToNw (nIndexCtx, &IsoInfo, SmsInfo);
		trace_event ("End  FCT_AUT_REQ_FROM_NW(NOK)", PROCESSING);
		return (NOK);
	}

	/*IsoInfo.msgId = MsgIsoReqReceived(ResourceId,&IsoInfo);*/
	IsoInfo.msgId = SmsInfo->msgId;
	trace_message(PROCESSING, "[SWMON1]: IsoInfo.msgId (%lu)",IsoInfo.msgId );


	/* Call the pre-control module */
	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	GetIsoField(ISO_PROC_CODE, &IsoInfo, ProcessingCode, &Length);
	/* Start ENH 12.2 SLA 130912 Fast Funds */
	result = GetTableInd( &IsoInfo, sAutTabInd);/*SNO120416*/
	result = ServiceProcessing(		nIndexCtx,
		ResourceId,
		/*"ASM",*/
		sAutTabInd,/*SNO120416*/
		AcquirerBank,
		"Y",
		"G",
		"N",
		sTlvBuffer,
		&IsoInfo);
	
	/*Start IBO20200517 RAWMSG*/
	/*if (g_sCaptureMsg[NW_INC_REQ] == 'Y')
	{
		LogVisaMessage(nIndexCtx, SmsInfo, &IsoInfo, INC_REQ);
	}*/
	/*End IBO20200517 RAWMSG*/

	trace_message(PROCESSING, "[SWMON2]: IsoInfo.msgId (%lu)", IsoInfo.msgId );


	switch (result)
	{
	case ROUTAGE:
		/* forward request for authorization */
		if (ForwardAutRequest(nIndexCtx, &IsoInfo, sTlvBuffer, SmsInfo) != OK)
		{
			/* respond to VISA that issuer was not available */
			PutIsoField(ISO_RESPONSE_CODE, &IsoInfo, ISSUER_UNAVAILABLE_ACTION, 3);
			AutReplyToNw(nIndexCtx, &IsoInfo, SmsInfo);
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

		trace_message(PROCESSING, "[SWMON3]: IsoInfo.msgId (%lu)", IsoInfo.msgId );
		if (AutReplyToNw(nIndexCtx, &IsoInfo, SmsInfo) != OK) {
			trace_event("End  FCT_AUT_REQ_FROM_NW(NOK)", PROCESSING);
			return (NOK);
		}
		break;

	case SEVERE_ERROR:	/* shutdown interface */
		AutReplyToNw(nIndexCtx, &IsoInfo, SmsInfo);
		/*shutdown_resource(nIndexCtx, ResourceId);*//*PLUTONL-4242 ZKO 27012022*/
		trace_event("End  FCT_AUT_REQ_FROM_NW(SEVERE_ERROR)", PROCESSING);
		return (NOK);
		break;
	}

	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	InitIsoInfo(&IsoInfo);
	trace_event("End  FCT_AUT_REQ_FROM_NW(OK)", PROCESSING);
	return (OK);

}
/****************************************************************************/
/* Forwards an authorization request to the issuer interface.              */
/****************************************************************************/
int 
	ForwardAutRequest(int nIndexCtx, TSIsoInfo * IsoInfo, char *sTlvBuffer, TSSmsInfo * SmsInfo)
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
		GetCaptureIntPurgeTime(&SmsInfo->kCtime, &sAcqPurgeTimeVal);
		GetRoutingIntPurgeTime(&SmsInfo->kCtime, &sIssPurgeTimeVal);
	}

	/*sAcqPurgeTime -= 1;
	sIssPurgeTime -= 1;

	Node.tPurgeTime = sAcqPurgeTime;*/

	Node.tPurgeTime = sAcqPurgeTimeVal.tv_sec;
	Node.stPurgeTime.tv_sec = sAcqPurgeTimeVal.tv_sec;
	Node.stPurgeTime.tv_usec = sAcqPurgeTimeVal.tv_usec;

	GetSmsField(SMS_AUDIT_NBR, SmsInfo, tmpField, &tmpFieldLen);
	sprintf(Node.szFstKey, "%04d", SmsInfo->nMsgType);
	memcpy(Node.szFstKey + 4, tmpField, 6);

	memset(tmpField, 0, sizeof(tmpField));
	tmpFieldLen = 0;

	GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen);
	memcpy(Node.szFstKey + 4 + 6, tmpField, tmpFieldLen);


	memcpy(Node.sFstBuf, (char *) &(SmsInfo->sHeader), SMS_HDR_LEN);
	memset(sBuffer, 0, sizeof(sBuffer));
	Length = SmsBuildMsg(sBuffer, SmsInfo);
	memcpy(Node.sFstBuf + SMS_HDR_LEN, sBuffer, Length);

	Node.nFstBufLen = Length + SMS_HDR_LEN;	


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
	BalMessage.sHeader.nCommand = 1200;
	/*SKARROUMI PLUTONL-4900*/
	if( nIsExternal == OK )
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

int BuildAutReqFromNw(int nIndexCtx,
	TSSmsInfo * SmsInfo,
	TSIsoInfo * IsoInfo)
{
	TSTlvPrivate    kTlvInfo;
	TSCpsInfo       CpsInfo;
	char            tmpField[256];
	char            tmpField1[256];
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
	char            AddPos[12];
	char            sIsoProcCode[7];
	char            sPid[8 + 1];
	char            sLine[MAX_LINE_TRC];
	char            sRespCode[3 + 1];
	char            sVisaRespCode[2 + 1];
	int             field_61_presence = 0;
	char            IwkLmk[256 + 1];
	char 		  sCAVV[128];
	char 		  sCAVV_Com[128];
	char 		  sXID [128];
	char			sTrxAmount[32];
	char			sBillingAmount[32];
	char			sSettlementAmount[32];
	char			sTrxCurrency[4];
	char			sBillingCurrency[4];
	char                   tmpField2 [256 +1];
	char                   tmpField3 [256 +1];
	char		szGMTDate[12 + 1];	/*AMER 20160719*/
	char		szTrxDateTime[12 + 1];  /*AMER 20160719*/
	char			sAuthId[64];
	char			sTrnId[64];
	char			sBAI[12];
	int             nLength;
	int             i;
	TSPufInfo		PufInfo;
	TlvInfo			kSmsTransSpecTlv;
	TlvInfo			kIsoMoneyTransfTlv;
	TlvInfo			kIsoPayFacTlv;
	TlvInfo			kSmsPayFacTlv;
	TlvInfo			kSmsTlvInfo;   			/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17 and 9.1.4*/
	char                    sSmsReasonCode[4 + 1]; /* AMA */
	char                    sFunctionCode[3 + 1];  /* AMA */
	TSPufInfo               kPufInfo;               /* AMA */	
	char            	sPufData[256];	/* AMA */
	char            sAddAuthInfo[3 + 1];
	char 			sIsoTermAddr[40+1];
	TSSmsPufInfo   SmspufInfo;
	char			sEcomAuthInfo[P7_F048_P66_LEN + 1];
	int				nEcomInfoLen = 0;
	TlvInfo			kSmsElectComTlv;
	char			AciCode[4]; /* AMA */  
	TlvInfo			kSmsSuppInfTlv; 
	TlvInfo         isoDigitizationDataTlv;      /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	TlvInfo         isoSaleInfoDataPropTlv;      /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	char			RecurringFlag[1 + 1];
	int				tagPresence = 0;			/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	int				nExempIndLen = 0;
	int				nExempResLen = 0;
	char            nEcommerceDatalen =0;/*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	char            EcommerceData [P7_F048_P66_LEN  +1];/*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	TlvInfo    		isoExemption;       /*HAL  20201803 Enh Q2.2020 ART 9.3.3*/
	TlvInfo			isoEcomAuthentifData;
	TlvInfo			smsAddVerifMappingData;  	/*HAL2402 PLUTONL-4391*/
	int 			nLenZipCode;
	TlvInfo			kSmsVerifDataTlv;  		/*HAL2402 PLUTONL-4391*/
	TlvInfo			smsPanMappingData;  	/*HAL2402 PLUTONL-4391*/
	TlvInfo    		kTokenDataIso;    		/*HAL2402 PLUTONL-4391*/
	TSTagIso		IsoIccInfo;                           /* NAB12032020*/
	char         	FieldData[256];                      /* NAB12032020*/
	int             FieldLen;                            /* NAB12032020*/
	char            sTokenResponseVisa[1 + 1];  /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	int nFlag;/*PLUTONL-3326*/
	char			sOtherAmnt[32];    /*NAB11032020*/
	char			sTrxOtherAmnt[32]; /*NAB11032020*/
	char			sAfdRefNbr[32+1];       /*AMER20170313*/
	char			sAuthCycleCode[12];
    char			sAvsData[51 + 1];/*ACH20191010*/
	char			sAvsRequest[1+1];
	char          	sIsoPosMode[12 + 1]; 						/*ACH 20191107*/
	char 			sPSD2AuthExempInd[16 + 1];
	char 			sPosExtData[ P7_F048_P61_LEN+1];/*PLUTONL-3326*/
	char 			sPSD2AuthExempRes[3 + 1];
	char			sInitParty[2]; /*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	BitMappedInfo	kVIPufData;
	TlvInfo         kSmsAppIdentTlv; /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/

	TlvInfo			kIsoAcqTlv;	/*PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
	TlvInfo			kSmsCustmRelatdDataTlv; /*PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
    TlvInfo			kSmsAccOwnerDataTlv; /*PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
	TlvInfo			k3DSData;    /*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
	BerInfo			kSmsBerInfo; /*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/

	trace_event("Start  BuildAutReqFromNw()", PROCESSING);

	memset(tmpField, 0, sizeof(tmpField));
	memset(tmpField1, 0, sizeof(tmpField1));
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
	memset(szGMTDate,'\0',sizeof(szGMTDate));		/*AMER 20160719*/
	memset(szTrxDateTime,'\0',sizeof(szTrxDateTime));	/*AMER 20160719*/

	memset (sTrxAmount,      	 0, sizeof(sTrxAmount));
	memset (sBillingAmount,      0, sizeof(sBillingAmount));
	memset (sTrxCurrency,		0, sizeof(sTrxCurrency));
	memset (sBillingCurrency,   0, sizeof(sBillingCurrency));
	memset (sAuthId,   0, sizeof(sAuthId));
	memset (sTrnId,   0, sizeof(sTrnId));
	memset (sBAI,   0, sizeof(sBAI));
	memset(sEcomAuthInfo, '\0', sizeof(sEcomAuthInfo));
	memset(sEcomAuthInfo, ' ', P7_F048_P66_LEN);
	memset(AciCode, 0, sizeof(AciCode)); /* AMA */
	memset (RecurringFlag,  0, sizeof(RecurringFlag));
	memset(EcommerceData, ' ', sizeof(EcommerceData));/*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	memset (FieldData,           0, sizeof(FieldData));/*NAB11032020*/
	memset(sTokenResponseVisa, 0, sizeof(sTokenResponseVisa));    /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	memset (sOtherAmnt,      	 0, sizeof(sOtherAmnt));/*NAB11032020*/
	memset (sTrxOtherAmnt,      	 0, sizeof(sTrxOtherAmnt));/*NAB11032020*/
	memset (sAfdRefNbr,  0, sizeof(sAfdRefNbr));	/*AMER20170313*/
    memset (sAvsRequest,  0, sizeof(sAvsRequest));
	memset (sIsoPosMode,   0, sizeof(sIsoPosMode)); 			/*ACH 20191107*/
 	memset(sPSD2AuthExempInd, '0', sizeof(sPSD2AuthExempInd)); /*HAL 20201803 ENHS 2020Q2 ART 9.3.3*/ /*AEA20210712 Enh21.Q4*/
	memset(sPosExtData  ,' ',sizeof(sPosExtData ));/*PLUTONL-3326*/
	memset(sPSD2AuthExempRes, ' ', sizeof(sPSD2AuthExempRes)); /*HAL 20201803 ENHS 2020Q2 ART 9.3.3*/
	memset(sInitParty, 0, sizeof(sInitParty));     /*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/

	/* Initialize ISO info structure */
	InitIsoInfo(IsoInfo);
	InitSmsPufInfo(&SmspufInfo); /* SMS DEFECT */
	InitCpsInfo(&CpsInfo);
	IniIsoMoneyTransDataTlv(&kIsoMoneyTransfTlv);
	IniIsoPayFacDataTlv(&kIsoPayFacTlv);
	InitSmsTransSpecDataTlv(&kSmsTransSpecTlv);
	InitSmsElectComDataTlv(&kSmsElectComTlv);			/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	InitSmsSupportInfDataTlv(&kSmsSuppInfTlv);		/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	IniDigitDataTlv(&isoDigitizationDataTlv);   /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	IniSaleInfoDataTlv(&isoSaleInfoDataPropTlv);	/*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	InitSmsVerifDataTlvInfo(&kSmsVerifDataTlv);			/*HAL2402 PLUTONL-4391*/
	InitSmsTokenDataTlvInfo(&smsPanMappingData);	/*HAL2402 PLUTONL-4391*/
	IniTknDataTlv(&kTokenDataIso);					/*HAL2402 PLUTONL-4391*/
	InitSmsVIPPufDataTlv(&kVIPufData);
	InitSmsPayFacDataTlv(&kSmsPayFacTlv);
	InitPufInfo (&kPufInfo);
	InitSmsAddrVerifDataTlvInfo(&smsAddVerifMappingData);
	InitSmsAppIdentDataTlv(&kSmsAppIdentTlv);  /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	InitInfo3DSDataTlv(&k3DSData);             /*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
	InitAcqDataTlv(&kIsoAcqTlv); /*PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
	InitEcomAuthentificationInfoDataTlv(&isoEcomAuthentifData);  /*PLUTONL-8809 AMANS Enh2024Q4 VISA 3.2*/ 

	current_gmt_date (szGMTDate);	/*AMER 20160719*/
	/* Message Type is 1100 */
	switch (SmsInfo->nMsgType)
	{
	case 201:
		IsoInfo->nMsgType = 1201;break;
	case 220:
		IsoInfo->nMsgType = 1220;break;
	case 221:
		IsoInfo->nMsgType = 1221;break;
	default:
		IsoInfo->nMsgType = 1200;break;
	}

	if (GetSmsField(SMS_T2_DATA, SmsInfo, T2_Data, &tmpFieldLen) == SUCCESS)
	{
		GetTrack2Data(T2_Data, Pan, ExpiryDate, ServiceCode, PVV, CVV);
	}
	/*Start IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
	if (GetSmsField(SMS_PAN_EXTENDED /*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsElectComTlv);
	}
	/*End IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/

	/*	if (GetSmsField(SMS_PRIV_USE_FIELD, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
	AnalyseVisaPuf(tmpField, &VisapufInfo);
	}*//*SNOAVOIR*/

	/* AMA */	
	if (GetSmsField (SMS_PRIV_USE_FIELD , SmsInfo,tmpField,&tmpFieldLen) == SUCCES)
	{
		AnalyseSmsPuf    (  tmpField  , &SmspufInfo);
		/*AddTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  ,sPufData, tmpFieldLen );*/
	}
	/* End AMA */

	if (GetSmsField (SMS_CPS_FIELDS, SmsInfo, tmpField, &tmpFieldLen)== SUCCESS)
	{
		AnalyseCps     (tmpField, &CpsInfo);
	}

	if (GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &tmpFieldLen)== SUCCESS)
	{
		AnalyseBitMappedInfo(tmpField, tmpFieldLen, &kVIPufData);
	}
	/*memcpy(sAuthId , ResourceId, 2);*/


	/* Field 2: Primary Account Number */
	if (GetSmsField(SMS_CARD_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
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
	GetSmsField(SMS_PROC_CODE, SmsInfo, ProcCode, &tmpFieldLen);
	GetSmsField(SMS_MERCHANT_TYPE, SmsInfo, MerchantType, &tmpFieldLen);
	SmsToIsoProcessingCode(ProcCode, MerchantType, sIsoProcCode);
	AddIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, 6);

	if (memcmp (sIsoProcCode ,P7_F003_BAL_INQ, P7_F003_P1_LEN) != 0)
	{
		/*NAB11032020 put the cash back processing code if the trx is cash back --SIA*/
		if ((memcmp (sIsoProcCode ,P7_F003_PURCHASE, P7_F003_P1_LEN) == 0)
			&&(GetSmsField (SMS_OTHER_AMOUNTS, SmsInfo,sOtherAmnt, &tmpFieldLen) == SUCCES) )
		{
			memcpy(sTrxOtherAmnt, sOtherAmnt,12);

			if(atoi(sTrxOtherAmnt) > 0)
			{
				memcpy(sIsoProcCode,P7_F003_CASHBACK,P7_F003_P1_LEN);
				PutIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, 6);
			}
		}
		/* Field 4: Transaction Amount */
		if (GetSmsField(SMS_TRANS_AMOUNT, SmsInfo, sTrxAmount, &tmpFieldLen) == SUCCESS)
			AddIsoField(ISO_TRANS_AMOUNT, IsoInfo, sTrxAmount, 12);
		else if (memcmp(sIsoProcCode,P7_F003_PIN_UNBLOCK, P7_F003_P1_LEN ) == 0
			|| memcmp(sIsoProcCode,P7_F003_PIN_CHANGE, P7_F003_P1_LEN) == 0)
		{
			AddIsoField(ISO_TRANS_AMOUNT, IsoInfo,   "000000000000", 12);
			PutIsoField(ISO_BILLING_AMOUNT, IsoInfo, "000000000000", 12);
		}

		if( ( memcmp(sIsoProcCode,P7_F003_PIN_UNBLOCK, P7_F003_P1_LEN) == 0 && atoi(sTrxAmount) > 0) || 
		memcmp(ProcCode,SMS_F003_P1_PP_ACTIV_N_LD, SMS_F003_P1_LEN) == 0 )
		{
			memcpy(sIsoProcCode,P7_F003_PAYMENT,P7_F003_P1_LEN);
			PutIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, 6);
			AddTlvInfoField(P7_F048S20_PAYMENT_TYPE, &kIsoMoneyTransfTlv, PAY_TYPE_PP_INIT_LD, PAY_TYPE_LEN);
		}

			/* Field 5: Settlement Amount */
		if (GetSmsField(SMS_BILLING_AMOUNT, SmsInfo, sBillingAmount, &tmpFieldLen) == SUCCESS)
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

	/* Field 7: Transmission Date and Time */
	GetSmsField(SMS_XMIT_TIME, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField(ISO_XMIT_TIME, IsoInfo, tmpField, 10);

	/*Start SNO210316 */
	/* Field 9: Settlement Billing Conversion Rate */
	if (GetSmsField(SMS_CNVRSN_RATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_STLMNT_CNVRSN_RATE, IsoInfo, tmpField, 8);
	/*End SNO210316 */

	/* Field 10: Cardholder Billing Conversion Rate */
	if (GetSmsField(SMS_BILLING_CNVRSN_RATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_BILLING_CNVRSN_RATE, IsoInfo, tmpField, 8);

	/* Field 11: System Trace Audit Number */
	GetInternalStan(nIndexCtx, tmpField);
	AddIsoField(ISO_AUDIT_NBR, IsoInfo, tmpField, 6);

	/* (12) Trans Date & Time */
	current_date_yymmdd(tmpField2);
	if ((GetSmsField(SMS_TRANS_DATE, SmsInfo, szTrxDateTime + 2 /*tmpField*/, &tmpFieldLen) == SUCCESS) && /*AMER 20160719*/
		(GetSmsField(SMS_TRANS_TIME, SmsInfo, tmpField1, &tmpFieldLen) == SUCCESS))
	{
		/*memcpy(tmpField2 + 2, tmpField, 4);
		memcpy(tmpField2 + 6, tmpField1, 6);
		AddIsoField(ISO_TRANS_DATE_TIME, IsoInfo, tmpField2, 12);*/

		/*AMER 20160719 Start: PROD00029786*/
		SetLocalYear(szTrxDateTime,szGMTDate);
		memcpy(szTrxDateTime + 6,tmpField1,6);
		AddIsoField (ISO_TRANS_DATE_TIME, IsoInfo, szTrxDateTime, 12);
		/*AMER 20160719 End*/
	}
	else
	{
		current_hour(tmpField3);
		memcpy(tmpField2 + 6, tmpField3, 6);
		AddIsoField(ISO_TRANS_DATE_TIME, IsoInfo, tmpField2, 12);
	}

	/* Field 14: Expiry Date */
	if (GetSmsField(SMS_EXPIRY_DATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_EXPIRY_DATE, IsoInfo, tmpField, 4);
	else if (strlen(ExpiryDate) > 0)
		AddIsoField(ISO_EXPIRY_DATE, IsoInfo, ExpiryDate, 4);
	/*Start SNO210316 */
	/* Field 15: Settlement Billing Conversion Rate */
	/*current_date_yymmdd(tmpField2);
	if (GetSmsField(SMS_STLMNT_DATE, SmsInfo, tmpField2+ 2, &tmpFieldLen) == SUCCESS){
		AddIsoField(ISO_STLMNT_DATE, IsoInfo, tmpField2, 6);
	}*/
	/*End SNO210316 */
	/* Field 18: Merchant Type */
	AddIsoField(ISO_MERCHANT_TYPE, IsoInfo, MerchantType, 4);

	/* Field 19: Acquiring Institution Country Code */
	GetSmsField(SMS_ACQR_COUNTRY, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField(ISO_ACQR_COUNTRY, IsoInfo, tmpField, 3);

	/* Field 20: PAN Country Code */
	if (GetSmsField(SMS_PAN_COUNTRY, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, 3);

	/* Field 22: POS Entry Mode */
	memset(PosMode, 0, sizeof(PosMode));
	memset(PosCond, 0, sizeof(PosCond));
	memset(AddPos, 0, sizeof(AddPos));

	GetSmsField(SMS_POS_ENTRY_MODE, SmsInfo, PosMode, &tmpFieldLen);
	GetSmsField(SMS_POS_CONDITION, SmsInfo, PosCond, &tmpFieldLen);
	GetSmsField(SMS_ADTNL_POS_INFO, SmsInfo, AddPos, &AddPosLen);

	SmsToIsoPosMode(MerchantType, PosMode, PosCond, AddPos, tmpField); /*IBO20221025 PLUTONL-5272*/
	AddIsoField(ISO_POS_DATA, IsoInfo, tmpField, 12);
	/* Reajustement Processing Code En Cas De Trx E-Comm */



	/* Field 23: Card Seq Number           */
	if (GetSmsField(SMS_CARD_SEQUENCE_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_CARD_SEQUENCE_NBR, IsoInfo, tmpField, tmpFieldLen);


	/* Field 24: Function Code */
	if (memcmp(ProcCode, SMS_F003_P1_BALANCE_INQ, SMS_F003_P1_LEN) == 0)	/* balance inquiry */
		/*	AddIsoField(ISO_FUNCTION_CODE, IsoInfo, P7_F024_BAL_REQUEST, P7_F024_LEN); commented by AMA */
		memcpy(sFunctionCode,P7_F024_BAL_REQUEST, P7_F024_LEN);

	else if( memcmp(PosCond,SMS_F025_COMPLETION,SMS_F025_LEN) == 0 )
	{
		memcpy(sFunctionCode,P7_F024_AUTH_PREV_AMNT_DIFF, P7_F024_LEN);
	}
	else if( memcmp(PosCond,SMS_F025_ACC_VERIF,SMS_F025_LEN) == 0 )	/*Set function code for non-fin auth*/
	{
		memcpy(sFunctionCode,P7_F024_ACC_VERIF, P7_F024_LEN);
	}
	else if( memcmp(PosCond,SMS_F025_NORMAL_TRN,SMS_F025_LEN) == 0 &&
		GetBitMappedInfoField (SMS_F063_PREAUTH_TIME_LIMIT, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
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
		memcpy(sFunctionCode,P7_F024_INIT_REQUEST, P7_F024_LEN)
		/*	AddIsoField(ISO_FUNCTION_CODE, IsoInfo, P7_F024_INIT_REQUEST, P7_F024_LEN) commented by AMA */;
	}

	/* AMA  Field 25 :Reason Code*/
	InitPufInfo (&PufInfo);
	GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &tmpFieldLen);
	AnalysePuf (tmpField, &PufInfo);
	if (GetPufSubField (PUF_MSG_REASON, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		memset (tmpField1, 0, sizeof (tmpField1));
		SmsToIsoReasonCode (tmpField, tmpField1);
		if(memcmp(tmpField1,MSG_REASON_INC_AUTH,4) == 0)
		{
			memcpy(sFunctionCode,P7_F024_SUPP_AMNT, P7_F024_LEN);	
		}

		/*Start IBO20190823 PROD00071113 Enh19.Q4 Art 2.1*/
		if(memcmp(tmpField1,MSG_REASON_DEFERRED_AUTH,4) == 0)
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

    AddIsoField(ISO_FUNCTION_CODE, IsoInfo,sFunctionCode,P7_F024_LEN);


	if( strlen(tmpField1) > 0 )
	{
		AddIsoField (ISO_MSG_REASON, IsoInfo, tmpField1, P7_F025_LEN);

	}
	/* END AMA */

	/* Field 27: Authorization Code Length - Always = 6 */
	AddIsoField(ISO_AUTHOR_ID_LENGTH, IsoInfo, "6", 1);

	/* Field 32: Acquiring Institution Identification Code */
	GetSmsField(SMS_ACQR_ID, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField(ISO_ACQR_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 33: Forwarding Institution Id */
	if (GetSmsField(SMS_FORWD_ID, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_FORWD_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 35: Track II Data */
	if (GetSmsField(SMS_T2_DATA, SmsInfo, T2_Data, &tmpFieldLen) == SUCCESS)
	{
		AddIsoField(ISO_T2_DATA, IsoInfo, T2_Data, tmpFieldLen);
		GetTrack2Data(T2_Data, Pan, ExpiryDate, ServiceCode, PVV, CVV);
	}
	/* Field 37: Retrieval Reference Number */
	GetSmsField(SMS_REFERENCE_NBR, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField(ISO_REFERENCE_NBR, IsoInfo, tmpField, 12);


	/*memcpy(sAuthId + 2 + P7_F011_LEN, tmpField, P7_F037_LEN);*/

	/* Field 38: Auth Number */
	if (GetSmsField(SMS_AUTHOR_ID, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_AUTHOR_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 39: Response Code */
	if (GetSmsField(SMS_RESPONSE_CODE, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		TranslateRespCode(nIndexCtx, ResourceId, tmpField, "RP", sRespCode);
		AddIsoField(ISO_RESPONSE_CODE, IsoInfo, sRespCode, 3);
	}
	/* Field 40: Service Restriction Code */
	/*	if (GetSmsField(SMS_SERVICE_CODE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	AddIsoField(ISO_SERVICE_CODE, IsoInfo, tmpField, 3);
	else
	{*//*SNOAVOIR The Single Message System does not use this data element */
	if (strlen(ServiceCode) > 0)	/* extract service code from trackII */
		AddIsoField(ISO_SERVICE_CODE, IsoInfo, ServiceCode, 3);
	else  if (GetSmsField(SMS_T1_DATA, SmsInfo, T1_Data, &tmpFieldLen) == SUCCESS)
	{
		GetTrack1Data(T1_Data, Pan, ExpiryDate, ServiceCode, PVV, CVV);
		AddIsoField(ISO_SERVICE_CODE, IsoInfo, ServiceCode, 3);
	}
	/*	}*/

	/* Field 41: Terminal Identification */
	if (GetSmsField(SMS_TERMINAL_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_TERMINAL_NBR, IsoInfo, tmpField, 8);

	/* Field 42: Card Acceptor Identification Code */
	if (GetSmsField(SMS_OUTLET_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_OUTLET_NBR, IsoInfo, tmpField, 15);

	/* Field 43: Card Acceptor Location */
	if (GetSmsField(SMS_TERMINAL_ADR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		/*PLUTONL-3660*/
		SmsToIsoAcceptorName(nIndexCtx ,tmpField,sIsoTermAddr);
		AddIsoField(ISO_TERMINAL_ADR, IsoInfo, sIsoTermAddr, tmpFieldLen);
	}

	/* Field 45: TrackI Data */
	if (GetSmsField(SMS_T1_DATA, SmsInfo, T1_Data, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_T1_DATA, IsoInfo, T1_Data, strlen(T1_Data));


	/* Field 48: Additional Private Data, it contains VISA fields 44 & */
	/* 48 in TLV format. */
	InitTlvPrivate(&kTlvInfo);
	AddTlvPrivate(HPS_ACQUIRER_BANK_TAG, &kTlvInfo, AcquirerBank, 6);

	if (GetSmsField(SMS_SUPPORT_INFO, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsSuppInfTlv);

			if( GetTlvInfoField (SMS_F125_ADD_ORIGINAL_DATA, &kSmsSuppInfTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				InitSmsAddOriginalDataTlv(&kSmsTlvInfo);	
				AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsTlvInfo);

				if( GetTlvInfoField (SMS_F125T03_ORIGINAL_TRA_ID, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
				{
					AddTlvPrivate(ORIGINAL_TRX_ID, &kTlvInfo, tmpField, tmpFieldLen);	
				}
			}
			/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
			if( GetTlvInfoField (SMS_F125_TOKEN_DEVICE, &kSmsSuppInfTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				InitTokenDevice(&kSmsTlvInfo);
				AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsTlvInfo);
				if( GetTlvInfoField (SMS_F125T01_IP_ADDR, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
				{
					PutTlvInfoField(P7_F048S22_CUSTOMER_IP, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);
			    }
			}
			if( GetTlvInfoField (SMS_F125_WALLET_PROVIDER, &kSmsSuppInfTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				InitSmsWalletProviderDataTlv(&kSmsTlvInfo);
				AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsTlvInfo);
				if( GetTlvInfoField (SMS_F125T02_WP_DEVICE_SCORE, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S25_WP_DEVICE_SCORE, &isoDigitizationDataTlv, tmpField, tmpFieldLen);
				}

				if( GetTlvInfoField (SMS_F125T02_WP_ACCOUNT_SCORE, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
				{
					AddTlvInfoField(P7_F048S25_WP_ACCOUNT_SCORE, &isoDigitizationDataTlv, tmpField, tmpFieldLen);
				}
			}
			/*End PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
	}
	if(GetSmsField(SMS_AUDIT_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddTlvPrivate(EXTERNAL_STAN_TAG, &kTlvInfo, tmpField, 6);

		/*AMER20170313 Start*/
	if ( memcmp(MerchantType,MCC_AFD,P7_F018_LEN) == 0 || memcmp(MerchantType,MCC_SERVICE_STATIONS,P7_F018_LEN) == 0)	/*AMER20171128 PROD00049633: Add Service stations MCC*/
	{
		if (GetSmsField(SMS_REFERENCE_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 6.1.2*/
		{
			memcpy(sAfdRefNbr,tmpField,tmpFieldLen);
			AddTlvPrivate(AFD_REF_NBR, &kTlvInfo, sAfdRefNbr, tmpFieldLen);
		}
	}
	/*AMER20170313 End*/

	if (nEcomInfoLen > 0)	/*IBO20221025 PLUTONL-5272*/
	{
		AddTlvPrivate(ECOM_AUTH_INFO, &kTlvInfo, sEcomAuthInfo, nEcomInfoLen);
	}

	/*memcpy(sAuthId + 2, tmpField, P7_F011_LEN);*/
	SmsBuildAuthId(SmsInfo,sAuthId);
	AddTlvPrivate(AUTHORIZATION_IDENTIFIER, &kTlvInfo, sAuthId, strlen(sAuthId));
	if (GetCpsSubField(CPS_TRANS_ID, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		strncpy(sTrnId,tmpField,sizeof(sTrnId));
		AddTlvPrivate(TRANSACTION_IDENTIFIER, &kTlvInfo, sTrnId, strlen(sTrnId));
	}


	/* IWK */
	if (GetSmsField(SMS_PIN_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		memset(IwkLmk, 0, sizeof(IwkLmk));
		if (GetIWK(nIndexCtx, ResourceId, IwkLmk) == OK)
			AddTlvPrivate(ENCR_PIN_BLOCK_KEY_TAG, &kTlvInfo, IwkLmk, strlen(IwkLmk));
	}


	memset(sPufData, 0, sizeof(sPufData));
	if (GetSmsPufSubField(SPUF_CVV2, &SmspufInfo, sPufData, &nLength) == SUCCES &&
	sPufData[0] == SMS_F126_P10_CVV2_P)
	{
		AddTlvPrivate(CVC2_DATA_TAG, &kTlvInfo, sPufData + 3, 3);
	}

	/*if ( GetVisaPufSubField(VPUF_RECURRING , &VisapufInfo, sPufData, &nLength ) == SUCCES )
	{
	AddTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , sPufData , nLength );
	}

	*//*SNOAVOIR*/

	/* RECURRING AMA */	

	if (GetSmsPufSubField(SPUF_POS_ENVIRONMENT, &SmspufInfo, sPufData, &nLength) == SUCCES)
	{
		if (GetTlvInfoField (SMS_F34_ACCEPTANCE_ENV_ADD_DATA, &kSmsElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			InitSmsAccEnvAddDataTlv(&kSmsTlvInfo);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsTlvInfo);
			GetTlvInfoField (SMS_F34T02_INITIATING_PARTY_ID, &kSmsTlvInfo, sInitParty, &tmpFieldLen);
		}

		if(sPufData[0] == SMS_F126_P13_RECURRING)
		{
			RecurringFlag[0] = P7_F048_P71_RECURRING;
			PutTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , RecurringFlag , 1);
			if (sInitParty[0] != SMS_F048_P64_P4_MIT)
			{
				sAddAuthInfo[P7_F048_P64_CRDNTL_REASON] = P7_F048_P64_P7_RECURRING;
				tagPresence = 1;
			}

		}
		else if(sPufData[0] == SMS_F126_P13_INSTALLMENT)
		{
			RecurringFlag[0] = P7_F048_P71_INSTALLMENT;
			PutTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , RecurringFlag , 1);
			if (sInitParty[0] != SMS_F048_P64_P4_MIT)
			{
				sAddAuthInfo[P7_F048_P64_CRDNTL_REASON] = P7_F048_P64_P7_INSTALLMENT;
				tagPresence = 1;
			}

		}
		else if(sPufData[0] == SMS_F126_P13_CREDENTIAL)
		{
			RecurringFlag[0] = P7_F048_P71_CRED_ON_FILE;
			PutTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , RecurringFlag , 1);
		}

	}

	if (GetSmsPufSubField(SPUF_CAVV, &SmspufInfo, sCAVV, &tmpFieldLen)== SUCCESS)
	{
		AddTlvPrivate (CAVV_TAG, &kTlvInfo, sCAVV, strlen(sCAVV));
	}

	/*if (strlen (AddPos) > 11)
	{			
		if ( memcmp(AddPos + 10,"02",2) == 0 )
			PutTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , "R" , 1 ); 
	}*/

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

		/*Start AMER20190219 PROD00065991: Enh19.Q2 Art2.1*/
	if(memcmp(MerchantType, MCC_QUASI_CASH_MER, SMS_F018_LEN) == 0)
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
	if (GetSmsField(SMS_PAN_EXTENDED /*PLUTONL-5548 Enh 2023Q2  Sms 1.2 AHA20221221*/, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitExempAuthInfoDataTlv(&isoExemption);
		/*AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsElectComTlv);*/   /*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/

		if( GetTlvInfoField (SMS_F34_ACCEPTANCE_ENV_ADD_DATA, &kSmsElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			InitSmsAccEnvAddDataTlv(&kSmsTlvInfo);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsTlvInfo);

			if( GetTlvInfoField (SMS_F34T02_INITIATING_PARTY_ID, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
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
			if( GetTlvInfoField (SMS_F34T02_AUTH_OUTAGE_INDICATOR, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				sPSD2AuthExempInd[P7_F048_S27_001_AUT_OUTAGE_EXCEPTION] = tmpField[0];
				nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_AUT_OUTAGE_EXCEPTION +1);
			}
			/*End AEA20210712 Enh21.Q4 Art9.1.2*/

			/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 SMS Art2.2*/
			if( GetTlvInfoField (SMS_F34T02_CRDNTL_MER_RELATIONSHIP_IND, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				sAddAuthInfo[P7_F048_P64_CRDNTL_MER_RELATIONSHIP_IND] = tmpField[0];
				tagPresence = 1;
			}
			/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 SMS Art2.2*/

			/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
			if( GetTlvInfoField (SMS_F34T02_ACQ_NAME, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048_S32_ACQ_NAME, &kIsoAcqTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F34T02_ACQ_STREET_ADDR, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048_S32_ACQ_STREET_ADDR, &kIsoAcqTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F34T02_ACQ_CITY_NAME, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048_S32_ACQ_CITY_NAME, &kIsoAcqTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F34T02_ACQ_STATE_CODE, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048_S32_ACQ_STATE_CODE, &kIsoAcqTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F34T02_ACQ_POSTAL_CODE, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048_S32_ACQ_POSTAL_CODE, &kIsoAcqTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F34T02_ACQ_COUNTRY_CODE, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048_S32_ACQ_COUNTRY_CODE, &kIsoAcqTlv, tmpField, tmpFieldLen);
			}

			tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kIsoAcqTlv);
			if( tmpFieldLen > 0 )
			{
				AddTlvPrivate(ACQUIRER_INFO, &kTlvInfo, tmpField, tmpFieldLen);
			}

			if( GetTlvInfoField (SMS_F34T02_ACC_STREET_ADDR, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_ACC_STREET_ADDR, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F34T02_PAYFAC_STREET_ADDR, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_STREET_ADDR, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F34T02_PAYFAC_CITY_NAME, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_CITY_NAME, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F34T02_PAYFAC_STATE_CODE, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_STATE_CODE, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F34T02_PAYFAC_POSTAL_CODE, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_POSTAL_CODE, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F34T02_PAYFAC_CTRY_CODE, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_CTRY_CODE, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
		}

		/*Start HAL PROD00073823 20202801 Enh Q2.2020 Art 3.10*/		
		if( GetTlvInfoField (SMS_F34_AUTHEN_DATA, &kSmsElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			InitSmsEcomAddDataTlv(&kSmsTlvInfo);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsTlvInfo);

			if( GetTlvInfoField (SMS_F34T01_PROTOCOL_VERSION_NUMBER, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
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
				  
				nEcomInfoLen=MAX(nEcomInfoLen,1+1); /*PLUTONL-5548 Enh 2023Q1  Sms 1.2 AHA20221221*/
			}
			/*START PLUTONL-5548 Enh 2023Q1  Sms 1.2 AHA20221221*/
			if( GetTlvInfoField (SMS_F34T01_AUTHENTICATION_PROGRAM, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				if (memcmp(tmpField,SMS_DAF,2)==0 )
				{
					sEcomAuthInfo[P7_F048_P66_AUTHEN_FRMWRK_OFST] = P7_F048_P66_VISA_DAF;
				}
				else
				{
					sEcomAuthInfo[P7_F048_P66_AUTHEN_FRMWRK_OFST] = P7_F048_P66_AF_UNSPECIFED;
				}
				
				nEcomInfoLen = MAX(nEcomInfoLen,P7_F048_P66_AUTHEN_FRMWRK_OFST+1);
			}
			/*END PLUTONL-5548 Enh 2023Q1  Sms 1.2 AHA20221221*/	
			/*Start PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
			if( GetTlvInfoField (SMS_F34T01_AUTHEN_DATA_QUALITY_INDC, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
               	AddTlvInfoField (P7_F048S26_AUTHEN_DATA_QUALITY_INDC , &isoEcomAuthentifData, tmpField, tmpFieldLen);
			}
			/*End PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 		
		}
		/*End HAL PROD00073823 20202801 Enh Q2.2020 Art 3.10*/

		if( GetTlvInfoField (SMS_F34_EU_PSD2_SCA_CSC, &kSmsElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			/*InitExempAuthInfoDataTlv(&isoExemption); 	*/
			InitSmsEuPsd2ScaCsc(&kSmsTlvInfo);
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsTlvInfo);
			
			/*Start ZKO16082019 PROD00070403 Enh19.Q4 Art 9.1.2*/				
			if( GetTlvInfoField (SMS_F34T4A_DELG_AUTH, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
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
			if( GetTlvInfoField (SMS_F34T4A_REASON_HONOR_EXEM, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField (P7_F048_S27_REASON_HONOR_EXEM , &isoExemption, tmpField, tmpFieldLen);

			}

			if( GetTlvInfoField (SMS_F34T4A_TRA_SCORE, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField (P7_F048_S27_TRA_SCORE , &isoExemption, tmpField, tmpFieldLen);

			}
			
			if( GetTlvInfoField (SMS_F34T4A_TRST_MER_EXEM, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
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

			if( GetTlvInfoField (SMS_F34T4A_LOW_VAL_EXEM, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				if (tmpField[0] == '1')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_LOW_VAL_EXEM] = '1';	/*AEA20210712 Enh21.Q4*/
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_LOW_VAL_EXEM +1);	/*AEA20210712 Enh21.Q4*/
				}
				/*Start PLUTONL-5548  Enh 2023Q2  Sms 9.3.1 AHA20221221*/
				else if (tmpField[0] == '2' || tmpField[0] == '3')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_LOW_VAL_EXEM] = '1';
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_LOW_VAL_EXEM +1);

					sPSD2AuthExempRes[P7_F048_S27_001_LOW_VAL_EXEM] = tmpField[0];
					nExempResLen = MAX(nExempResLen,P7_F048_S27_001_LOW_VAL_EXEM +1);
				}
				/*End PLUTONL-5548  Enh 2023Q2  Sms 9.3.1 AHA20221221*/
			}
			if( GetTlvInfoField (SMS_F34T4A_SPC, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
			{
				if (tmpField[0] == '1')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_SCP] = '1';	/*AEA20210712 Enh21.Q4*/
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_SCP +1);	/*AEA20210712 Enh21.Q4*/
				}
				/*Start PLUTONL-5548  Enh 2023Q2  Sms 9.3.1 AHA20221221*/
				else if (tmpField[0] == '2' || tmpField[0] == '3')
				{
					sPSD2AuthExempInd[P7_F048_S27_001_SCP] = '1';
					nExempIndLen = MAX(nExempIndLen,P7_F048_S27_001_SCP +1);

					sPSD2AuthExempRes[P7_F048_S27_001_SCP] = tmpField[0];
					nExempResLen = MAX(nExempResLen,P7_F048_S27_001_SCP +1);
				}
				/*End PLUTONL-5548  Enh 2023Q2  Sms 9.3.1 AHA20221221*/
			}
			if( GetTlvInfoField (SMS_F34T4A_TRA, &kSmsTlvInfo, tmpField, &tmpFieldLen) == SUCCESS )
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

		 /*Start NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
		 if( GetTlvInfoField (SMS_F34_SUPP_DATA, &kSmsElectComTlv, tmpField, &tmpFieldLen) == SUCCESS )
		 {   
			 /*InitEcomAuthentificationInfoDataTlv(&isoEcomAuthentifData);*/  /*PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
			 InitSmsSupplementalData(&kSmsBerInfo);
			 /*AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsBerInfo);*/
			 AnalyseBerInfo(tmpField, tmpFieldLen, &kSmsBerInfo);
				  
			 if( GetBerInfoField (SMS_F34T56_SESSION_ID, &kSmsBerInfo, tmpField, &tmpFieldLen) == SUCCESS )
				 AddTlvInfoField (P7_F048S26_ECOM_AUT_DATA_SESSION_ID , &isoEcomAuthentifData, tmpField, tmpFieldLen);
			 /*Start PLUTONL-5548  Enh 2023Q2  Sms 3.2 AHA20221221*/
			 if(GetBerInfoField (SMS_F34T56_CAVV_VERSION_NUMBER, &kSmsBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			 {
				 memcpy(EcommerceData,tmpField,P7_F063_N23_CAVV_VERSION_NBR_LEN);
				 nEcommerceDatalen=P7_F063_N23_CAVV_VERSION_NBR_LEN;
			 }
			 if( GetBerInfoField (SMS_F34T56_CAVV_TYPE, &kSmsBerInfo, tmpField, &tmpFieldLen) == SUCCESS )
			 {
				 memcpy(EcommerceData+P7_F063_N23_CAVV_TYPE_OFST,tmpField,P7_F063_N23_CAVV_TYPE_LEN);
				 nEcommerceDatalen=P7_F063_N23_CAVV_TYPE_OFST+P7_F063_N23_CAVV_TYPE_LEN;
			 }
			 if(GetBerInfoField (SMS_F34T56_IP_ADDR_VELOCITY_COUNT, &kSmsBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			 {
				 memcpy(EcommerceData+P7_F063_N23_IP_ADDR_VELC_CNT_OFST,tmpField,P7_F063_N23_IP_ADDR_VELC_CNT_LEN);
				 nEcommerceDatalen=P7_F063_N23_IP_ADDR_VELC_CNT_OFST+P7_F063_N23_IP_ADDR_VELC_CNT_LEN;
			 }
			 if(GetBerInfoField (SMS_F34T56_DEVICE_ID_VELOCITY_COUNT, &kSmsBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			 {
				 memcpy(EcommerceData+P7_F063_N23_DEVICE_ID_VELC_CNT_OFST,tmpField,P7_F063_N23_DEVICE_ID_VELC_CNT_LEN);
				 nEcommerceDatalen=P7_F063_N23_DEVICE_ID_VELC_CNT_OFST+P7_F063_N23_DEVICE_ID_VELC_CNT_LEN;
			 }
			 if(GetBerInfoField (SMS_F34T56_VISA_RISK_AUTHEN_SCORE, &kSmsBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			 {
				 memcpy(EcommerceData+P7_F063_N23_VISA_RISK_BASED_AUTN_SCORE_OFST,tmpField,P7_F063_N23_VISA_RISK_BASED_AUTN_SCORE_OFST);
				 nEcommerceDatalen=P7_F063_N23_VISA_RISK_BASED_AUTN_SCORE_OFST+P7_F063_N23_VISA_RISK_BASED_AUTN_SCORE_LEN;
			 }
			 if(GetBerInfoField (SMS_F34T56_THIRD_PARTY_ID, &kSmsBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			 {
				 AddTlvInfoField (P7_F048_S29_THIRD_PARTY_ID ,&k3DSData, tmpField, tmpFieldLen);
			 }
			 if(GetBerInfoField (SMS_F34T56_THIRD_PARTY_RISK_SCORE, &kSmsBerInfo, tmpField, &tmpFieldLen) == SUCCESS)
			 {
				 AddTlvInfoField (P7_F048_S29_THIRD_PARTY_RISK_SCORE ,&k3DSData, tmpField, tmpFieldLen);
			 }
			 /*End NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
			

			/*End PLUTONL-5548  Enh 2023Q2  Sms 3.2 AHA20221221*/
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
		
		tmpFieldLen = BuildTlvInfoBuffer (tmpField, &k3DSData);/*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
		if( tmpFieldLen > 0 )
			AddTlvPrivate(DATA_3DS, &kTlvInfo, tmpField, tmpFieldLen);
	}

	/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
    if( GetSmsField(SMS_I_ORIG_DATA , SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{				
		InitSmsCustmRelatdDataTlv(&kSmsCustmRelatdDataTlv);
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsCustmRelatdDataTlv);

		InitSmsAccOwnerDataTlv(&kSmsAccOwnerDataTlv);

		if( GetTlvInfoField (SMS_F056_ACCOUNT_OWNER_DATA, &kSmsCustmRelatdDataTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{

			AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsAccOwnerDataTlv);

			if( GetTlvInfoField (SMS_F056T05_ACC_OWNER_NATIONALITY, &kSmsAccOwnerDataTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048S22_ACC_OWNER_NATIONALITY, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F056T05_ACC_OWNER_CTRY_OF_BIRTH, &kSmsAccOwnerDataTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048S22_ACC_OWNER_CTRY_OF_BIRTH, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);
			}	
			if( GetTlvInfoField (SMS_F056T05_ACC_OWNER_OCCUPATION, &kSmsAccOwnerDataTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048S22_ACC_OWNER_OCCUPATION, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);
			}
			if( GetTlvInfoField (SMS_F056T05_ACC_OWNER_DATE_OF_BIRTH, &kSmsAccOwnerDataTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048S22_ACC_OWNER_DATE_OF_BIRTH, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);
			}	
			if( GetTlvInfoField (SMS_F056T05_ACC_OWNER_EMAIL_ADDS, &kSmsAccOwnerDataTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048S22_ACC_OWNER_EMAIL_ADDS, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);
			}
		}
			
	}
	/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/

	if(nFlag=1)
	{
		PutTlvPrivate ( POS_DATA_EXT_TAG , &kTlvInfo  , sPosExtData,P7_F048_P61_LEN );
	}
	/*PLUTONL-3326*/

	if (tagPresence == 1)
	{	
		AddTlvPrivate(AUTH_ADD_INFO, &kTlvInfo, sAddAuthInfo, P7_F048_P64_LEN);
	}
	/*PLUTONL-3326*/

	/*if(AddPos[9] == '2')
	{
		PutTlvPrivate ( RECURRING_FLAG_TAG , &kTlvInfo  , "R" , 1 );
	}*/

	if (GetSmsField(SMS_ADTNL_RESP_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AddTlvPrivate(CVV_RESULT_CODE_TAG, &kTlvInfo, tmpField + 4, 1);
		/*Start ACH20191210  PLUTONL-500 */ 
		GetIsoField (ISO_POS_DATA, IsoInfo, sIsoPosMode, &tmpFieldLen);
		int T2DataPresenceFlag= GetSmsField(SMS_T2_DATA, SmsInfo, T2_Data, &tmpFieldLen);
		int IccDataPresenceFlag= GetSmsField(SMS_ICC_DATA, SmsInfo, tmpField1, &tmpFieldLen);
		SmsToIsoAdtnlRespData(tmpField, T2DataPresenceFlag, IccDataPresenceFlag, sIsoPosMode, 0, &kTlvInfo);	
		/*End ACH20191210  PLUTONL-500 */
	}

	/*AMER20190527 PROD00068232: TODO put the correct mapping*/
	/*if (GetSmsField(SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddTlvPrivate(ADDRESS_VERIF_REQ_TYPE_TAG, &kTlvInfo, tmpField + 1, tmpFieldLen - 1);*/


	if ( GetSmsPufSubField(SPUF_ACCOUNT_RESULT , &SmspufInfo, sPufData, &nLength ) == SUCCES )
	{
	AddTlvPrivate ( ACCOUNT_RESULT_TAG , &kTlvInfo  , sPufData , nLength );
	}

		/*Start ICH See PROD00039494*/
	if ( GetSmsPufSubField(SPUF_DCC_INDICATOR , &SmspufInfo, sPufData, &nLength ) == SUCCES )
	{
		AddTlvPrivate ( DCC_DATA , &kTlvInfo  , sPufData , 1 );
	}
	/*End ICH See PROD00039494*/

	if ( GetSmsPufSubField(SPUF_3DS_INDICATOR , &SmspufInfo, sPufData, &nLength ) == SUCCES )
	{
	AddTlvPrivate ( ADD_AUTH_METHOD_TAG , &kTlvInfo  , sPufData , nLength );
	}

	if (GetCpsSubField (CPS_MVV , &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AddTlvPrivate (MVV_TAG, &kTlvInfo, tmpField,tmpFieldLen);

	}

	/*Start AMER20190219 PROD00065991: Enh19.Q2 Art2.4*/
	if (GetSmsField(SMS_TRANS_DESC, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsTransSpecTlv);
		
		if( GetTlvInfoField (SMS_F104_PAY_FAC_DATA, &kSmsTransSpecTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsPayFacTlv);
			
			if( GetTlvInfoField (SMS_F104T56_PAY_FAC_ID, &kSmsPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_PAYFAC_ID, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			
			if( GetTlvInfoField (SMS_F104T56_SUB_MER_ID, &kSmsPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				AddTlvInfoField(P7_F048P21_SUB_MER_ID, &kIsoPayFacTlv, tmpField, tmpFieldLen);
			}
			
			if( GetTlvInfoField (SMS_F104T56_ISO_ID, &kSmsPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
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

	/*End AMER20190219 PROD00065991: Enh19.Q2*/
	
	if( memcmp(sIsoProcCode,P7_F003_PAYMENT,P7_F003_P1_LEN) == 0 )
	{
		SmsToIsoMoneyTransData(SmsInfo,&kIsoMoneyTransfTlv,sBAI);
		if (GetSmsField(SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS &&
			memcmp(tmpField,"OCT",3) == 0 )
		{
			AddTlvPrivate (WATCH_LIST_SCORING_RES, &kTlvInfo, tmpField+7,3);
		}
		if( memcmp(sBAI,SMS_F104T57_AA,SMS_F104T57_LEN) == 0 || memcmp(sBAI,SMS_F104T57_PP,SMS_F104T57_LEN) == 0 )
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

	/**/
	if (GetSmsField(SMS_ADR_VERIF_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsVerifDataTlv);

		if (GetTlvInfoField(SMS_F123_ADDR_VERIF_DATA, &kSmsVerifDataTlv, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AnalyseTlvInfo(tmpField, tmpFieldLen, &smsAddVerifMappingData);
			
			sAvsRequest[0] = P7_F048_P82_ADR_VERIF_REQ;
			AddTlvPrivate(ADDRESS_VERIF_REQ_TYPE_TAG, &kTlvInfo, sAvsRequest, 1);
			
			memset(sAvsData, ' ', sizeof(sAvsData));
			sAvsData[0] = '1';

			if (GetTlvInfoField(SMS_F123_T66_POSTAL_CD, &smsAddVerifMappingData, tmpField, &tmpFieldLen) == SUCCESS)
			{
				nLenZipCode = tmpFieldLen > 9 ? 9 : tmpFieldLen;
				memcpy(sAvsData + P7_F048_S21_FORMAT_LEN, tmpField, nLenZipCode);
				tagPresence = 1;
			}

			if (GetTlvInfoField(SMS_F123_T66_STREE_ADDR, &smsAddVerifMappingData, tmpField, &tmpFieldLen) == SUCCESS)
			{
				memcpy(sAvsData + P7_F048_S21_FORMAT_LEN + P7_F048_S21_ZIP_CODE_LEN, tmpField, tmpFieldLen);
				tagPresence = 1;
			}
			/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
			if (GetTlvInfoField(SMS_F123_T66_CH_SHIP_HASH, &smsAddVerifMappingData, tmpField, &tmpFieldLen) == SUCCESS)
			{
				AddTlvInfoField(P7_F048S22_CH_SHIP_HASH, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);	
			}
			/*End PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/

			if (tagPresence == 1)
			{
				AddTlvPrivate(ADRESS_VERIFICATION_DATA, &kTlvInfo, sAvsData, P7_F048_S21_FORMAT_LEN + P7_F048_S21_ZIP_CODE_LEN + P7_F048_S21_ADR_LEN);
			}
		}

		if (GetTlvInfoField(SMS_F123_TOKEN_DATA, &kSmsVerifDataTlv, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AnalyseTlvInfo(tmpField, tmpFieldLen, &smsPanMappingData);

			if (GetTlvInfoField(SMS_F123_T68_TOKEN, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TOKEN, &kTokenDataIso, tmpField, tmpFieldLen);
			
			if (GetTlvInfoField(SMS_F123_T68_TKN_ASS_LVL, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_ASSURANCE_LVL, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_REQ_ID, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_REQ_ID, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_RANGE, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_PAN, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_REFERENCE_ID, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_PAN_REF_ID, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_EXPIRATION_DATE, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_EXP_DT, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_TYPE, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_TYPE, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_STATUS, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_STATUS, &kTokenDataIso, tmpField, tmpFieldLen);
			/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/
			if (GetTlvInfoField(SMS_F123_T68_AUTO_FILL_IND, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_AUTO_FILL, &kTokenDataIso, tmpField, tmpFieldLen);
			/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/

			/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
			if (GetTlvInfoField(SMS_F123_T68_TKN_AUTHENTICATION_AMNT, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
			{
				AddTlvInfoField(P7_F048S22_PURCHASE_AMOUNT, &isoSaleInfoDataPropTlv, tmpField, tmpFieldLen);
			}
			/*End PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
		}
	}
	/**/

	/*Start IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kIsoPayFacTlv);
	if( tmpFieldLen > 0 )
	{
		AddTlvPrivate(MERCHANT_ADDITIONAL_DATA, &kTlvInfo, tmpField, tmpFieldLen);
	}	
	/*End IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/

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
	GetSmsField(SMS_TRANS_CRNCY, SmsInfo, sTrxCurrency, &tmpFieldLen);
	AddIsoField(ISO_TRANS_CRNCY, IsoInfo, sTrxCurrency, 3);

	/* Field 50: Settlement Currency Code */
	if (GetSmsField(SMS_BILLING_CRNCY, SmsInfo, sBillingCurrency, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_STLMNT_CRNCY, IsoInfo, sBillingCurrency, 3);
	else if( strlen(sTrxCurrency) > 0 )
		AddIsoField(ISO_STLMNT_CRNCY, IsoInfo, sTrxCurrency, 3);

	/* Field 51: Cardholder Billing Currency Code */
	if (strlen(sBillingCurrency) > 0)
		AddIsoField(ISO_BILLING_CRNCY, IsoInfo, sBillingCurrency, 3);
	else if( strlen(sTrxCurrency) > 0 )
		AddIsoField(ISO_BILLING_CRNCY, IsoInfo, sTrxCurrency, 3);

	/* Field 52: Personal Identification Number Data */
	if (GetSmsField(SMS_PIN_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_PIN_DATA, IsoInfo, tmpField, 16);

	/* Field 53: Security Data */
	memset(tmpField1, 0, sizeof(tmpField1));
	if (GetSmsField(SMS_PIN_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
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

	/*: AMA (54) Additional Amount :*/
	memset(tmpField1, 0, sizeof(tmpField1));
	SmsToIsoAddAmount(SmsInfo,tmpField1);
	if ( strlen(tmpField1) > 0 )
		AddIsoField(ISO_ADTNL_AMOUNTS, IsoInfo, tmpField1, strlen(tmpField1));
	/*: END-AMA :*/

	/* Field 55: Chip Data */

	if (GetSmsField(SMS_ICC_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{

		dump_buffer(tmpField, tmpFieldLen, 'A', 'X', 'X');
		TSTagSms      SmsIccInfo;
		char           sIccBuffer[BUF_LG_MAX];		
		
		InitSmsIcTag(&SmsIccInfo);
		InitIsoIcTag(&IsoIccInfo);
		memset(sIccBuffer, 0, sizeof(sIccBuffer));

		AnalyseSmsIc  ( tmpField, tmpFieldLen , &SmsIccInfo );

		if(GetSmsIcTag  (SMS_TAG_SEC_PIN_BLOCK , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			PutIsoField(ISO_ACQR_REFERENCE_DATA, IsoInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_APP_INTER_PROFILE , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_APP_INTER_PROFILE, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_DED_FILE_NAME , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_DED_FILE_NAME, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_TVR , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TVR, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_TRANS_DATE , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TRANS_DATE, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_TRANS_TYPE , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TRANS_TYPE, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_TRANS_CUR_CODE , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TRANS_CUR_CODE, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_TRANS_AMOUNT , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TRANS_AMOUNT, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_OTHER_AMOUNT , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_OTHER_AMOUNT, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_ISS_APP_DATA , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_ISS_APP_DATA, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_TERM_COUNTRY_CODE , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TERM_COUNTRY_CODE, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_APP_CRYPTOGRAM , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_APP_CRYPTOGRAM, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_TERM_CAP , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_TERM_CAP, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_ATC , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_ATC, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_UNPRED_NUMBER , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_UNPRED_NUMBER, &IsoIccInfo, FieldData, FieldLen);
		if(GetSmsIcTag  (SMS_TAG_FFI , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag(ISO_TAG_FFI, &IsoIccInfo, FieldData, FieldLen);

		tmpFieldLen = IsoBuildIcFld(sIccBuffer, &IsoIccInfo);
		if (tmpFieldLen > 0)
			AddIsoField(ISO_ICC_DATA, IsoInfo, sIccBuffer, tmpFieldLen);
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
	if (GetSmsField(SMS_CPS_FIELDS, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
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
		if (tmpFieldLen > 0)
			AddIsoField(ISO_CPS_FIELDS, IsoInfo, tmpField, tmpFieldLen);
	}

	InitTlvPrivate(&kTlvInfo);


	if (SmsInfo->nMsgType == 220 || SmsInfo->nMsgType == 221)
	{
      	if ( GetBitMappedInfoField (SMS_F063_STIP_SWITCH_RSN_CD, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
        {	
		    AddTlvPrivate(NET_STIP_REASON_CODE_TAG, &kTlvInfo, tmpField, tmpFieldLen);
	    }

		/*Start HAL PROD00073823 20200402 Enh Q2.2020 Art 1.1*/
    	if ( GetBitMappedInfoField (SMS_F063_MSG_RSN_CODE, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
        {
	        if( memcmp(tmpField,MSG_REASON_COMPLETION_ADVICE,SMS_F063_RSN_CD_LEN) == 0 )
	        {
				        
		        memcpy(sFunctionCode,P7_F024_AUTH_PREV_AMNT_DIFF, P7_F024_LEN);
		        PutIsoField(ISO_FUNCTION_CODE, IsoInfo, sFunctionCode,P7_F024_LEN);    
	        }
		}
		/*Start HAL PROD00073823 20200402 Enh Q2.2020 Art 1.1*/
	}

	if ( GetBitMappedInfoField (SMS_F063_CHARGEBACK_REDUCTION, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
	{	
		AddTlvPrivate(NET_CHARGEBACK_FLAGS_TAG, &kTlvInfo, tmpField, tmpFieldLen);
	}

	if ( GetBitMappedInfoField (SMS_F063_ACQ_BUSINESS_ID, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
	{	
		AddTlvPrivate(NET_VISA_ABI_TAG, &kTlvInfo, tmpField, tmpFieldLen);
	}

	if ( GetBitMappedInfoField (SMS_F063_FRAUD_DATA, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
	{	
		AddTlvPrivate(NET_FRAUD_DATA_TAG, &kTlvInfo, tmpField, tmpFieldLen);
	}
	if ( GetBitMappedInfoField (SMS_F063_REIMBURSEMENT_ATTRIBUTE, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
	{	
		AddTlvPrivate(NET_REIMB_ATTR_TAG, &kTlvInfo, tmpField, tmpFieldLen);
	}
	if ( GetBitMappedInfoField (SMS_F063_SHARING_GROUP_CODE, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
	{	
		AddTlvPrivate(NET_SHARE_GRP_CODE_TAG, &kTlvInfo, tmpField, tmpFieldLen);
	}
	if ( GetBitMappedInfoField (SMS_F063_DECIMAL_POSITIONS_INDICATOR, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
	{	
		AddTlvPrivate(NET_DECIMAL_POS_TAG, &kTlvInfo, tmpField, tmpFieldLen);
	}

	if ( GetBitMappedInfoField (SMS_F063_ISSUER_CURRENCY_CONVERSTION_DATA, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
	{	
		AddTlvPrivate(NET_ISS_CUR_CONV_DATA_TAG, &kTlvInfo, tmpField, tmpFieldLen);
	}

	if( memcmp(sIsoProcCode,P7_F003_PAYMENT,P7_F003_P1_LEN) == 0 )
	{
		if (GetSmsField(SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS &&
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
	if (GetSmsPufSubField(SPUF_VPUF_MER_ID, &SmspufInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		AddTlvPrivate (NET_VISA_MERC_ID, &kTlvInfo, tmpField,tmpFieldLen);
	}
	/*Start HAL 20201803 ENHS 2020Q2 ART 9.3.3*/

	/*Start IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	if (GetSmsField(SMS_ADTNL_RESP_DATA, SmsInfo, FieldData, &FieldLen) == SUCCES)
	{
		sTokenResponseVisa[0]=FieldData[2];
		AddTlvPrivate (NET_TOKEN_PROGAM, &kTlvInfo, sTokenResponseVisa ,1);
	}
	/*End IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/

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

	/*End PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/

	/*Start PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	if( GetTlvInfoField (SMS_F104_BUSINESS_APP_ID, &kSmsTransSpecTlv, tmpField, &tmpFieldLen) == SUCCESS )
	{ 
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsAppIdentTlv);
		memset(tmpField, '\0', sizeof(tmpField));
		if( GetTlvInfoField (SMS_F104_T57_MAX_PROC_DATE, &kSmsAppIdentTlv, tmpField, &tmpFieldLen) == SUCCESS )
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

	if (!memcmp(sRespCode,BAD_CVV_ACTION, 3))
	{
		/*InsertErrorMsg( IsoInfo ,BAD_CVV_MSG , strlen(BAD_CVV_MSG));*/ 
		PutIsoField(ISO_RESPONSE_CODE, IsoInfo, P7_F039_DECLINE, P7_F039_LEN );
		AuthorLog(	nIndexCtx ,
			IsoInfo ,
			ResourceId ,
			STAND_IN_RESOURCE ,
			"ASM"
			);
		trace_event ("End  BuildAutReqFromNw(NOK)", PROCESSING);
		return (NOK);
	}
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
	InitSmsPufInfo(&SmspufInfo);
	InitCpsInfo(&CpsInfo);
	IniIsoMoneyTransDataTlv(&kIsoMoneyTransfTlv);
	IniIsoPayFacDataTlv(&kIsoPayFacTlv);
	InitSmsTransSpecDataTlv(&kSmsTransSpecTlv);
	InitSmsPayFacDataTlv(&kSmsPayFacTlv);
	InitSmsVIPPufDataTlv(&kVIPufData);
	InitPufInfo (&kPufInfo);
	InitSmsElectComDataTlv(&kSmsElectComTlv);

	trace_event("End  BuildAutReqFromNw(OK)", PROCESSING);
	return (OK);
}
/**********************************************************************/
/*  FCT_AUT_REQ_TO_NW()                                               */
/*                                                                    */
/*  Send a Financial Authorization Request message to SMS.            */
/**********************************************************************/
int  FCT_AUT_REQ_TO_NW (int nIndexCtx, TSIsoInfo *IsoInfo)
{
	TSSmsInfo		SmsInfo;
	TSNodeData		Node;
	TSTlvAutho		sInternalTlvInfo;
	char   			FieldData       [BUF_LG_MAX];
	char 			sBuffer         [BUF_LG_MAX];
	char			sLine           [MAX_LINE_TRC];
	char			sPurgeTime[10 + 1];
	int				FieldLen;
	int				Length;
	int				retour;
	time_t			PurgeTime;
    /*20201301 PLUTON-779*/
	char            sPurgeTimeMS[4];
    struct timeval	sCreationTime;
	int				nPurgeTimeMS;
	long 			nSec;
	int 			nUsec;
	char            tmpField[256];
	/*20201301 PLUTON-779*/
	char            sActionCode[4];/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
	char 			sSecurityData [128 + 1];

	trace_event("Start  FCT_AUT_REQ_TO_NW()", PROCESSING);

	dump_p7_struct (IsoInfo);

	MsgFwdReqRetrieved(IsoInfo->msgId);

	/***** Initialisation des variables et structures de travail *****/
	memset (FieldData,              0, sizeof (FieldData));
	memset (sBuffer,                0, sizeof (sBuffer));
	memset (sLine,                  0, sizeof (sLine));
	memset (sPurgeTime,             0, sizeof (sPurgeTime));
    memset (sActionCode,            0, sizeof (sActionCode));/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
	memset (sSecurityData, 0, sizeof(sSecurityData));


	/***** Insert original Iso Message In List for later retrieval  ***/
	InitNodeData (&Node);
	GetIsoField (ISO_SECURITY_DATA, IsoInfo, sSecurityData, &FieldLen);
	GetSecurityData(PURGE_TIME,sSecurityData,sPurgeTime, &FieldLen);

	/*Node.tPurgeTime = atol(sPurgeTime);*/ 	/*20201301 PLUTON-779*/

	/*20201301 PLUTON-779*/
	nSec = atol(sPurgeTime);
	
	/*GetSecurityData(PURGE_TIME_MS, tmpField, sPurgeTimeMS, &Length); */
	GetSecurityData(PURGE_TIME_MS, sSecurityData, sPurgeTimeMS, &FieldLen);
	nPurgeTimeMS = atoi(sPurgeTimeMS);
	nUsec = nPurgeTimeMS* 1000;
	Node.tPurgeTime = nSec;
	Node.stPurgeTime.tv_sec = nSec;
	Node.stPurgeTime.tv_usec = nUsec;
	/*20201301 PLUTON-779*/

	retour = BuildAutReqToNw (nIndexCtx,&SmsInfo,IsoInfo);
	/*** Auto-reponse generee en cas de probleme de conversion ***/
	if (retour != OK)
	{
		/*IsoInfo->nMsgType += 10;
		HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11
		memcpy(sActionCode,"909",3);
		GetIsoField (ISO_RESPONSE_CODE, IsoInfo, sActionCode, &FieldLen);

		ReplyIsoMsgToBal (nBalMsg, IsoInfo, "", sActionCode);
		MsgFwdRepReplied(IsoInfo->msgId);*/
		trace_event("FCT_AUT_REQ_TO_NW(): Convertion failed", PROCESSING);
		return (NOK); 
		/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/	
	}

	/*GetIsoField (ISO_AUDIT_NBR, IsoInfo, FieldData, &FieldLen);*//*  we should insert in NODE the external stan *//*NAB25042019 Certif SG*/
	GetSmsField(SMS_AUDIT_NBR, &SmsInfo, FieldData, &FieldLen);
	memcpy (Node.szFstKey, "1200", 4);
	memcpy (Node.szFstKey + 4,  FieldData, 6);

	GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &FieldLen);
	memcpy(Node.szFstKey + 4 + 6, tmpField, FieldLen);

	/*AMER 20160727 Start: PADSS Fix*/
	if(debug_option == 1)
	{
		sprintf(sLine, "==> InsertKey = [%.32s] ", Node.szFstKey);
		trace_event(sLine, PROCESSING);
	}
	/*AMER 20160727 End: PADSS Fix*/

	Length = IsoBuildMsg(Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;
	Node.msgId = IsoInfo->msgId;

	retour = InsertRequest('B', &Node);
	if (retour == EEXIST)
		SmsInfo.nMsgType = 201;
	else if (retour != OK)
	{
		trace_event("FCT_AUT_REQ_TO_NW(): InsertRequest() failed", PROCESSING);
		return (NOK);
	}
	/* send request to network */
	Length = SmsBuildMsg(sBuffer + SMS_HDR_LEN, &SmsInfo);
	SmsBuildHeader(REQUEST , &(SmsInfo.sHeader), NULL, Length, StationId);
	memcpy(sBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
	Length += SMS_HDR_LEN;

	/*Start IBO20200527 RAWMSG*/
	/*if (g_sCaptureMsg[NW_OUTG_REQ] == 'Y')
	{
		LogVisaMessage(nIndexCtx, &SmsInfo, IsoInfo, OUTG_REQ);
	}*/
	/*End IBO20200527 RAWMSG*/

	dump_buffer(sBuffer, Length, 'A', 'L', 'O');

	DumpSms(sBuffer);


	if (WriteLineMsg(sBuffer, Length) != OK)
	{
		trace_event ("FCT_AUT_REQ_TO_NW(NOK): WriteLineMsg() failed.", ERROR);
		FetchRequest('B', &Node);
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}

	MsgFwdReqSent(IsoInfo->msgId);

	memset(FieldData, 0, sizeof(FieldData));
	memset(sBuffer, 0, sizeof(sBuffer));
	memset(sLine, 0, sizeof(sLine));
    InitSmsInfo(&SmsInfo);
	InitNodeData (&Node);

	trace_event ("End FCT_AUT_REQ_TO_NW (OK)", PROCESSING);

	return (OK);
}

/*******************************************************************/
/*  BuildAutReqToNw()                                              */
/*                                                                 */
/*  Prepares a 200 message to be sent to SMS                       */
/*                                                                 */
/*  Input: IsoInfo: ISO-format message to be converted             */
/*         SmsInfo: structure to contain SMS message               */
/*         CtxIndex: pointer to Oracle context                     */
/*                                                                 */
/*******************************************************************/
int BuildAutReqToNw ( int        nIndexCtx,
	TSSmsInfo  *SmsInfo,
	TSIsoInfo  *IsoInfo)
{
	char         sDate[5];
	int          TranslStat = 0;
	char         sLine[MAX_LINE_TRC];
	char         FieldData[256];
	char         sCpsTlvData[512];   
	char         tmpField[BUF_LG_MAX];
	char         tmpField1[1024];
	char         sIccBuffer[1024];   
	int    		tmpFieldLen;
	int    		tmpFieldLen1;
	int    		tmpFieldLen2;
	int    		tmpFieldLen3;
	char   		sAbrevBankName[16];
	char   		sMcc[5];
	char   		CVV[4];
	char   		PVV[6];
	char   		T2_Data[38];
	char   		CardNumber[20];
	char   		XmitTime[10 + 1]; /* YHM20170425 */
	char   		AuditNbr[6 + 1]; /* YHM20170425 */
	int    		nRetCode;
	char   		ExpiryDate[5];
	char   		ServiceCode[4];
	char   		MerchantType[5];
	char		EmvCountryCode [4 + 1];  
	char		CC[256];   
	char   		detail[256];
	char   		isoProcCode[7];
	char   		isoPosMode[12 + 1];
	char   		TransKey  [256];
	char   		Awk [256];
	char   		PinData [16 + 1];
	char   		PinFormat [ 2 + 1];
	char   		security_format [ 2 + 1];
	char   		mode;
	int    		i;
	TSCpsInfo		CpsInfo;
	TSPufInfo		PufInfo;
	TSTlvPrivate	kTlvInfo;
	int    			IsChipTrx;
	TSTagIso		IsoIccInfo;
	TSTagSms		SmsIccInfo;
	int				FieldLen; 

	char          TrxAmount   [32];
	char          TrxCurrency [6];
	char          Outlet      [32];
	char          CalcFee     [32];
	char          IsoCountryCode[ 3+1 ];
	char		  szAcquirerBank[6 + 1];
	/*START NAB26042019*/
	char            sRefNbr[12 + 1];
	char            szWork[256 + 1];
	char            szDate[12 + 1];
	char            sData[12 + 1];
	char            exStan[6 + 1];
	/*END NAB26042019*/
	TlvInfo			kIsoPayFacTlv;
	TlvInfo			kSmsPayFacTlv;	
	TlvInfo			kSmsTransSpecTlv;
	TlvInfo			kSmsRecurringPayTlv;	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	TlvInfo			kIsoRecurringPayTlv;	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	char			szAcquirerId[12 + 1];
	char			sAdtnlInfo[12+1];
	char            sAddAuthInfo[P7_F048_P64_LEN + 1];
	char 			sQC_CrptCrcy[1+1];
	TlvInfo			kSmsElectComTlv;   		/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	TlvInfo			kSmsSuppInfTlv;   		/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	TlvInfo			kSmsTlvInfo;   			/*IBO20190729 PROD00070403 Enh19.Q4 Art 2.17 and 9.1.4*/
	TlvInfo 		kSmsAccEnvAddDataInfo;	/*AEA20210712 Enh21.Q4 Art9.1.2*/
	TlvInfo 		kTlvData;
	char 			sAuthExcem[1+1];        /*IBO20190725 PROD00070403 Enh19.Q4 Art 9.1.3*/
	char 			sTrustMer[1+1];         /*HAL 20201903 ENH 2020Q2 ART 9.3.3*/
	char            sVisaPufData[512];       /*HAL 20201903 ENH 2020Q2 ART 9.3.3*/
	TSSmsPufInfo    SmspufInfo;
  	TSTlvPrivate    kNwTlvData;
	/*Start HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
	int 			nTrxAmount ; 
	int 			trxFee;
	/*End HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
	char			RecurringFlag[2];	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	char         	sFunctionCode[4];	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	int				nNbrTags = 0;		/*ZKO 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	BerInfo			kSmsBerInfoSuppDta; /*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/

	trace_event("Start  BuildAutReqToNw()", PROCESSING);

	/* Initialize SMS info structure */

	memset (sCpsTlvData,     0, sizeof(sCpsTlvData));
	memset (tmpField1,       0, sizeof(tmpField1));
	memset (tmpField,        0, sizeof(tmpField));
	memset (sAbrevBankName,  0, sizeof(sAbrevBankName));
	memset (sMcc,            0, sizeof(sMcc));
	memset (PinData,         0, sizeof(PinData));
	memset (PinFormat,       0, sizeof(PinFormat));
	memset (TransKey,        0, sizeof(TransKey));
	memset (Awk,             0, sizeof(Awk));
	memset (CardNumber,      0, sizeof(CardNumber));
	memset (PVV,             0, sizeof(PVV));
	memset (CVV,             0, sizeof(CVV));
	memset (T2_Data,         0, sizeof(T2_Data));
	memset (isoPosMode,      0, sizeof(isoPosMode));
	memset (isoProcCode,     0, sizeof(isoProcCode));
	memset (ExpiryDate,      0, sizeof(ExpiryDate));
	memset (ServiceCode,     0, sizeof(ServiceCode));
	memset (MerchantType,    0, sizeof(MerchantType));
	memset (security_format, 0, sizeof(security_format));
	memset (sDate,           0, sizeof(sDate)); 
	memset (TrxAmount     ,    0, sizeof(TrxAmount));
	memset (TrxCurrency   ,    0, sizeof(TrxCurrency));
	memset (Outlet        ,    0, sizeof(Outlet));
	memset (CalcFee       ,    0, sizeof(CalcFee));
	memset (IsoCountryCode,    0, sizeof(IsoCountryCode));
	memset (szAcquirerBank,    0, sizeof(szAcquirerBank));
	memset (szAcquirerId,    0, sizeof(szAcquirerId));
	memset (XmitTime      ,  0,  sizeof(XmitTime)); /* YHM20170425 */
	memset (AuditNbr      ,  0, sizeof(AuditNbr)); /* YHM20170425 */
	memset (sAdtnlInfo, 0, sizeof(sAdtnlInfo));
	memset (sAddAuthInfo, 0, sizeof(sAddAuthInfo));
	memset (sQC_CrptCrcy, 0, sizeof(sQC_CrptCrcy));
	memset(sAuthExcem, '\0', sizeof(sAuthExcem));
	memset(RecurringFlag, 0, sizeof(RecurringFlag));	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	memset(sFunctionCode, 0, sizeof(sFunctionCode));	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	InitTlvPrivate (&kTlvInfo);
	IniIsoPayFacDataTlv(&kIsoPayFacTlv);
	InitSmsTransSpecDataTlv(&kSmsTransSpecTlv);
	InitSmsPayFacDataTlv(&kSmsPayFacTlv);
	InitSmsElectComDataTlv(&kSmsElectComTlv);		/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
	InitSmsSupportInfDataTlv(&kSmsSuppInfTlv);		/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	InitSmsRecurringPayDataTlv(&kSmsRecurringPayTlv);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	IniIsoInstallmentLoanTlv(&kIsoRecurringPayTlv);		/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/


	InitSmsInfo (SmsInfo);
	IsChipTrx = 0;

	if (GetIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvPrivate (tmpField, tmpFieldLen, &kTlvInfo);
	}
	else
	{
		IsoInfo->nMsgType += 10;
		ReplyIsoMsgToBal(nBalMsg, IsoInfo, "", CRYPTOGRAPHIC_ERROR_ACTION);
		trace_event("End  BuildAutReqToNw F48 Not Present(NOK)", PROCESSING);
		return (NOK);
	}
	/* Message Type is 0200 */
	SmsInfo->nMsgType = 200;

	/* Field 2: Primary Account Number */
	if (GetIsoField (ISO_CARD_NBR, IsoInfo, CardNumber, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_CARD_NBR, SmsInfo, CardNumber, tmpFieldLen);

	/* Field 3: Processing Code */
	memset(sMcc, 0, sizeof(sMcc));
	memcpy(sMcc,"0000",4);
	GetIsoField (ISO_PROC_CODE, IsoInfo, isoProcCode, &tmpFieldLen);
	GetIsoField (ISO_MERCHANT_TYPE, IsoInfo, sMcc, &tmpFieldLen);
	IsoToSmsProcessingCode (isoProcCode, tmpField, sMcc);
	AddSmsField (SMS_PROC_CODE, SmsInfo, tmpField, 6);

	/* Field 4: Transaction Amount */
	if (GetIsoField (ISO_TRANS_AMOUNT, IsoInfo, TrxAmount, &tmpFieldLen) == SUCCESS)
	{
		if ((memcmp (isoProcCode, "31", 2) != 0 ) && (memcmp (isoProcCode, "91", 2) != 0 ) && (memcmp (isoProcCode, "98", 2) != 0 ))
			/*NAB28052019 SG Certif : we shouldn't send the field 4 in case of Pin Change/Unblock msg*/
			{

			AddSmsField (SMS_TRANS_AMOUNT, SmsInfo, TrxAmount, 12);
	}
	}

	/* Field 7: Transmission Date and Time */
	GetIsoField (ISO_XMIT_TIME, IsoInfo, XmitTime, &tmpFieldLen);
	AddSmsField (SMS_XMIT_TIME, SmsInfo, XmitTime, 10);


	/* Field 11: System Trace Audit Number */
	/*START NAB25042019 Certif SG SMS Should sent the external stan instead of internal stan*/ 
	/* Field 11: System Trace Audit Number */
	/*GetIsoField (ISO_AUDIT_NBR, IsoInfo, AuditNbr, &tmpFieldLen);
	AddSmsField (SMS_AUDIT_NBR, SmsInfo, AuditNbr, 6); */
	if(GetTlvPrivate (EXTERNAL_STAN_TAG, &kTlvInfo, exStan, &tmpFieldLen) == SUCCESS)
	{
		AddSmsField (SMS_AUDIT_NBR, SmsInfo, exStan, 6); 
		
	}
	else
	{
	GetIsoField (ISO_AUDIT_NBR, IsoInfo, AuditNbr, &tmpFieldLen);
	AddSmsField (SMS_AUDIT_NBR, SmsInfo, AuditNbr, 6); 
	}
	/*END NAB25042019*/


	/* Fields 12 & 13: Local Transaction Time & Date */
	GetIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_TIME, SmsInfo, tmpField + 6, 6);
	AddSmsField (SMS_TRANS_DATE, SmsInfo, tmpField + 2, 4);

	/* Field 14: Expiry Date */
	if (GetIsoField (ISO_EXPIRY_DATE, IsoInfo,tmpField,&tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_EXPIRY_DATE, SmsInfo, tmpField, 4);
	else if (GetIsoField(ISO_T2_DATA, IsoInfo, T2_Data, &tmpFieldLen) == SUCCESS)
	{
		GetTrack2Data (T2_Data, CardNumber, ExpiryDate, ServiceCode, PVV, CVV);
		AddSmsField (SMS_EXPIRY_DATE, SmsInfo, ExpiryDate, 4);
	}

	/* Field 18: Merchant Type */
	GetIsoField (ISO_MERCHANT_TYPE, IsoInfo, MerchantType /*tmpField*/, &tmpFieldLen);
	AddSmsField (SMS_MERCHANT_TYPE, SmsInfo, MerchantType /*tmpField*/, 4);

	/* Field 19: Acquiring Institution Country Code */
	GetIsoField (ISO_ACQR_COUNTRY, IsoInfo, IsoCountryCode, &tmpFieldLen);
	/*AddSmsField (SMS_ACQR_COUNTRY, SmsInfo, CountryCode, 3);NAB10052019 SG Certif le champs 19 doit pas se recuperer de la table p7_sms_resource_param*/
	AddSmsField (SMS_ACQR_COUNTRY, SmsInfo, IsoCountryCode, 3);
	

	/* Field 20: Pan Country Code  */
	if (!memcmp(CardNumber, "59", 2))
	{
		GetIsoField (ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen);
		AddSmsField (SMS_PAN_COUNTRY, SmsInfo, tmpField, tmpFieldLen);
	}

	/* Field 21: Forwarding Institution Country Code */
	if (GetIsoField (ISO_FORWD_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_FORWD_COUNTRY, SmsInfo, tmpField, 4);

	/* Field 22: POS Entry Mode */
	GetIsoField (ISO_POS_DATA, IsoInfo, isoPosMode, &tmpFieldLen);
	IsoToSmsPosMode (isoPosMode, tmpField);

	if (GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField1, &tmpFieldLen) == SUCCESS)
	{
		memcpy (tmpField,"05",2);   
		AddSmsField (SMS_POS_ENTRY_MODE, SmsInfo, tmpField, 4);
	}
	else
	{
		AddSmsField (SMS_POS_ENTRY_MODE, SmsInfo, tmpField, 4);
	}

	/* Field 23 : CARD SEQUENCE NUMBER FOR VSDC USAGE*/ /* Nabil 170306 */
	if (GetIsoField (ISO_CARD_SEQUENCE_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_CARD_SEQUENCE_NBR, SmsInfo, tmpField, 3);

	/* Field 25: POS Condition Code, normal transaction */
	AddSmsField (SMS_POS_CONDITION, SmsInfo, "00", 2);

	/* Start ENH VISA 200904 HBU */
	/* (28) Amount Transaction Fee */  
	if  ((GetIsoField (ISO_FEES_AMOUNT, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS))
	    {		
		   if( (memcmp (isoProcCode, "31", 2)!=0) && (memcmp (isoProcCode, "40", 2)!=0)
	        && (memcmp (isoProcCode, "91", 2)!=0)
		    && (memcmp (isoProcCode, "98", 2)!=0))
		/*NAB28052019 SG Certif : we shouldn't send the field 4 in case of Pin Change/Unblock msg*/
	{
		AddSmsField (SMS_TRANS_FEE, SmsInfo, tmpField, 9);
	}
		}
	else if((memcmp (isoProcCode, "91", 2)!=0)&& (memcmp (isoProcCode, "98", 2)!=0))
	{
		AddSmsField(SMS_TRANS_FEE, SmsInfo, "D00000000", 9);
	}

	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
	if (GetSmsField (SMS_TRANS_FEE, SmsInfo, tmpField,&tmpFieldLen) == SUCCESS)
	{
		nTrxAmount  = atoi(TrxAmount);
		trxFee 		=	atoi(tmpField +1);

		if ( trxFee >= nTrxAmount && nTrxAmount > 0 ) /*IBO20201120 PLUTONL-2520*/
		{
			PutIsoField(ISO_RESPONSE_CODE, IsoInfo, INVALID_AMOUNT_ACTION , P7_F039_LEN);
			trace_event("End  BuildAutReqToNw 028 >= F004 Not Present(NOK)", PROCESSING);
			return (NOK);
		}	
	}
	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/

	/* Field 32: Acquiring Institution Identification Code */
	/*EBE
	AddSmsField (SMS_ACQR_ID, SmsInfo, AcquirerId, strlen(AcquirerId));*/
	if (GetTlvPrivate(ACQUIRER_BANK_TAG, &kTlvInfo, szAcquirerBank , &FieldLen) != SUCCESS)
	{
		trace_event("End BuildAutReqToNw (NOK) : ERROR Getting Acquirer bank",ERROR);
		return(NOK);
	}
	nRetCode = GetAquirerId( nIndexCtx, szAcquirerBank, NET_VISA, szAcquirerId)  ;
	if (nRetCode != OK)
	{
		sprintf(sLine,"End BuildAutReqToNw (NOK) : ERROR Getting AquirerId for bank[%s]",szAcquirerBank);
		trace_event(sLine,ERROR);
		return(NOK);
	}
	AddSmsField (SMS_ACQR_ID, SmsInfo, szAcquirerId, strlen(szAcquirerId));

	/* PROD00051898: acquirer id would be schemes acquirer id instead of bank code*/
	PutIsoField (ISO_ACQR_ID, IsoInfo, szAcquirerId, strlen(szAcquirerId));		/*PRODXX: Keep the acquirer id sent to VISA */

	InitSmsAccEnvAddDataTlv(&kSmsAccEnvAddDataInfo);	/*AEA20210712 Enh21.Q4 Art9.1.2*/
	/*Start IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
		if (GetTlvPrivate(AUTH_ADD_INFO, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
		{
			if (tmpField[P7_F048_P64_INITIATING_PARTY] == P7_F048_P64_P4_MER_INIT)
			{
				sAuthExcem[0]=tmpField[P7_F048_P64_INITIATING_PARTY];
				AddTlvInfoField(SMS_F34T02_INITIATING_PARTY_ID, &kSmsAccEnvAddDataInfo, sAuthExcem, 1); /*AEA20210712 Enh21.Q4 Art9.1.2*/
			}

		}
		/*End IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
		
	InitSmsEuPsd2ScaCsc(&kSmsTlvInfo);
	
	/*Start ZKO16082019 PROD00070403 Enh19.Q4 Art 9.1.2*/
     if (GetTlvPrivate( ECOM_AUTH_INFO, &kTlvInfo, tmpField, &tmpFieldLen) == SUCCESS)
     {
		if(tmpField[P7_F048_P66_DELG_AUTH_OFST] == '1')
			AddTlvInfoField(SMS_F34T4A_DELG_AUTH, &kSmsTlvInfo, tmpField + P7_F048_P66_DELG_AUTH_OFST, 1);	 /* ELKAM 10-12-2021 PLUTONL-3947 */
     }
	/*End ZKO16082019 PROD00070403 Enh19.Q4 Art 9.1.2*/

	/*Start HAL 20201903 Enh20.Q2 Art 9.3.3*/
    if (GetTlvPrivate( PSD2_DATA, &kTlvInfo, tmpField, &tmpFieldLen) == SUCCESS)
    {
		InitExempAuthInfoDataTlv(&kTlvData);
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kTlvData);

		if( GetTlvInfoField (P7_F048_S27_AUTH_EXEMPTION_INDIC, &kTlvData, tmpField, &tmpFieldLen) == SUCCESS )
		{
			if(tmpField[P7_F048_S27_001_TRST_MER_EXEM] == '1')
			{
				sAuthExcem[0]= tmpField[P7_F048_S27_001_TRST_MER_EXEM];
				AddTlvInfoField(SMS_F34T4A_TRST_MER_EXEM, &kSmsTlvInfo, sAuthExcem, 1);	/* 20202003 Enh20.Q2 Art 9.3.3*/
			}
	
			if(tmpField[P7_F048_S27_001_TRA] == '1')
			{
				sAuthExcem[0]= tmpField[P7_F048_S27_001_TRA];
				AddTlvInfoField(SMS_F34T4A_TRA, &kSmsTlvInfo, sAuthExcem, 1);	/* 20202003 Enh20.Q2 Art 9.3.4*/
		    }

			/*Start AEA20210712 Enh21.Q4 Art9.1.2*/
			if(tmpField[P7_F048_S27_001_AUT_OUTAGE_EXCEPTION] == '0' || tmpField[P7_F048_S27_001_AUT_OUTAGE_EXCEPTION] == '1')
			{
				sAuthExcem[0]=tmpField[P7_F048_S27_001_AUT_OUTAGE_EXCEPTION];
				AddTlvInfoField(SMS_F34T02_AUTH_OUTAGE_INDICATOR, &kSmsAccEnvAddDataInfo, sAuthExcem, 1);
			}
			/*End AEA20210712 Enh21.Q4 Art9.1.2*/
			
			if(tmpField[P7_F048_S27_001_LOW_VAL_EXEM] == '1')
			{
				sAuthExcem[0]=tmpField[P7_F048_S27_001_LOW_VAL_EXEM];
				AddTlvInfoField(SMS_F34T4A_LOW_VAL_EXEM, &kSmsTlvInfo, sAuthExcem, 1);
			}
			if(tmpField[P7_F048_S27_001_SCP] == '1')
			{
				sAuthExcem[0]=tmpField[P7_F048_S27_001_SCP];
				AddTlvInfoField(SMS_F34T4A_SPC, &kSmsTlvInfo, sAuthExcem, 1);
			}
		}
    }

		
	/*End HAL 20201903 Enh20.Q2 Art 9.3.3*/
	
	/*ZKO29122021 Start PLUTONL-3345

	/*Start IBO20190719 PROD00070403 Enh19.Q4 Art 9.1.3*
	if (GetTlvPrivate(AUTH_EXEMPTION_IND, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		/*InitSmsEuPsd2ScaCsc(&kSmsTlvInfo);*
		/*if(tmpField[P7_F048_K09_TRST_MER_EXEM] == '1')
		{
			sAuthExcem[0]=tmpField[P7_F048_K09_TRST_MER_EXEM];
			AddTlvInfoField(SMS_F34T4A_TRST_MER_EXEM, &kSmsTlvInfo, sAuthExcem, 1);
		}*
		if(tmpField[P7_F048_K09_LOW_VAL_EXEM] == '1')
		{
			sAuthExcem[0]=tmpField[P7_F048_K09_LOW_VAL_EXEM];
			AddTlvInfoField(SMS_F34T4A_LOW_VAL_EXEM, &kSmsTlvInfo, sAuthExcem, 1);
		}
		if(tmpField[P7_F048_K09_SCP] == '1')
		{
			sAuthExcem[0]=tmpField[P7_F048_K09_SCP];
			AddTlvInfoField(SMS_F34T4A_SPC, &kSmsTlvInfo, sAuthExcem, 1);
		}
		/*if(tmpField[P7_F048_K09_TRA] == '1')
		{
			sAuthExcem[0]=tmpField[P7_F048_K09_TRA];
			AddTlvInfoField(SMS_F34T4A_TRA, &kSmsTlvInfo, sAuthExcem, 1);
		}*
	}
	ZKO29122021 End PLUTONL-3345
	*/
	
	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kSmsTlvInfo);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(SMS_F34_EU_PSD2_SCA_CSC, &kSmsElectComTlv, tmpField, tmpFieldLen);
	}
	InitSmsSupplementalData(&kSmsBerInfoSuppDta);/*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/

	if (GetTlvPrivate( ECOM_AUTHENTIF_DATA, &kTlvInfo, tmpField, &tmpFieldLen) == SUCCESS)	/* ELKAM 10-12-2021 PLUTONL-3947 */
    {       
		/*InitSmsSupplementalData(&kSmsTlvInfo);*/
		InitEcomAuthentificationInfoDataTlv(&kTlvData);
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kTlvData);

		    if( GetTlvInfoField (P7_F048S26_ECOM_AUT_DATA_SESSION_ID, &kTlvData, tmpField, &tmpFieldLen) == SUCCESS )
		    {
			    AddTlvInfoField(SMS_F34T56_SESSION_ID, &kSmsBerInfoSuppDta, tmpField, tmpFieldLen);
		    }
    }
	
	/*Start NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
	if (GetTlvPrivate(DATA_3DS, &kTlvInfo, tmpField, &tmpFieldLen) == SUCCESS)
    {
		InitInfo3DSDataTlv(&kTlvData);
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kTlvData);

		if( GetTlvInfoField (P7_F048_S29_THIRD_PARTY_ID, &kTlvData, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddBerInfoField(SMS_F34T56_THIRD_PARTY_ID, &kSmsBerInfoSuppDta, tmpField, tmpFieldLen);	
		}
		if( GetTlvInfoField (P7_F048_S29_THIRD_PARTY_RISK_SCORE, &kTlvData, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddBerInfoField(SMS_F34T56_THIRD_PARTY_RISK_SCORE, &kSmsBerInfoSuppDta, tmpField, tmpFieldLen);	
		}
	}
  	/*tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kSmsTlvInfo);*/
	tmpFieldLen = BuildBerInfoBuffer (tmpField, &kSmsBerInfoSuppDta);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(SMS_F34_SUPP_DATA, &kSmsElectComTlv, tmpField, tmpFieldLen);
	}
    /*End NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
	
  	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kSmsTlvInfo);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(SMS_F34_SUPP_DATA, &kSmsElectComTlv, tmpField, tmpFieldLen);
	}
	
	/*Start AEA20210712 Enh21.Q4 Art9.1.2*/
	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kSmsAccEnvAddDataInfo);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(SMS_F34_ACCEPTANCE_ENV_ADD_DATA, &kSmsElectComTlv, tmpField, tmpFieldLen);
	}
	/*End AEA20210712 Enh21.Q4 Art9.1.2*/

	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kSmsElectComTlv);
	if (tmpFieldLen > 0)
		AddSmsField(SMS_PAN_EXTENDED, SmsInfo, tmpField, tmpFieldLen);
	
	/*End IBO20190719 PROD00070403 Enh19.Q4 Art 9.1.3*/
	/* Field 35: Track II Data */
	if (GetIsoField (ISO_T2_DATA, IsoInfo, T2_Data, &tmpFieldLen) == SUCCESS)
	{	
		replace(T2_Data,'=','D',tmpFieldLen);
		AddSmsField (SMS_T2_DATA, SmsInfo, T2_Data, tmpFieldLen);
	}

	/* Field 37: Retrieval Reference Number */
	/*START NAB26042019*/ /*Certif SG :Il faut construir le champs 37 pr qu'il puissent prendre en consideration le champs11 external dans les 6 derbiers caracters*/
	/*GetIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, 12);*/
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
	sprintf(sLine, "Date %s,JulianDate : %s|%d|%s|%s|", szDate, szWork, nRetCode, tmpField, exStan);
	trace_event(sLine, PROCESSING);
	sRefNbr[0] = szDate[7];
	memcpy(sRefNbr + 1, szWork, 3);
	memcpy(sRefNbr + 4, tmpField + 4, 2);
	memcpy(sRefNbr + 6, exStan, 6);
	PutSmsField(SMS_REFERENCE_NBR, SmsInfo, sRefNbr, 12);
	/*END NAB26042019*/

	/* Field 41: Terminal Identification */
	GetIsoField (ISO_TERMINAL_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TERMINAL_NBR, SmsInfo, tmpField, 8);

	/* Field 42: Card Acceptor Identification Code */
	GetIsoField (ISO_OUTLET_NBR, IsoInfo, Outlet, &tmpFieldLen);
	GetAbrevBankName (nIndexCtx, AcquirerBank, sAbrevBankName);
	AddSmsField (SMS_OUTLET_NBR, SmsInfo, sAbrevBankName, 15);

	/* Field 43: Card Acceptor Location */
	/* START NAB10052019 Certif SG */
	/*memset (tmpField1,       ' ', sizeof(tmpField1));
	GetIsoField (ISO_TERMINAL_ADR, IsoInfo, tmpField, &tmpFieldLen);*/

	/*if (tmpFieldLen > 25)
	{
	memcpy (tmpField1, tmpField, 24);
	memcpy (tmpField1 + 25, tmpField + 26, 13);
	}*/
	/*if (tmpFieldLen > 37)
	{
		memcpy (tmpField1, tmpField, 23);
		memcpy (tmpField1 + 25, tmpField + 23, 13);
	}
	else
	{
		memcpy (tmpField1, tmpField, tmpFieldLen);
		memcpy (tmpField1 + 25, BankLocation, strlen(BankLocation));
	}*/
	
	/*memcpy (tmpField1 + 38, AlphaCode2, 2);
	AddSmsField (SMS_TERMINAL_ADR, SmsInfo, tmpField1, 40);*/
	if (GetIsoField(ISO_TERMINAL_ADR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		replace(tmpField, '>', ' ', tmpFieldLen);
		replace(tmpField, '_', ' ', tmpFieldLen);
		AddSmsField(SMS_TERMINAL_ADR, SmsInfo, tmpField, 40);
	}
/*END NAB10052019*/

	/* Field 49: Transaction Currency Code */
	GetIsoField (ISO_TRANS_CRNCY, IsoInfo, TrxCurrency, &tmpFieldLen);
	AddSmsField (SMS_TRANS_CRNCY, SmsInfo, TrxCurrency, 3);

	/* Field 52: Personal Identification Number Data */
	if(GetIsoField (ISO_PIN_DATA, IsoInfo, PinData, &tmpFieldLen) == SUCCESS)
	{

		if(GetTlvPrivate (ENCR_PIN_BLOCK_KEY_TAG, &kTlvInfo, TransKey, &tmpFieldLen) != SUCCESS)
		{
			IsoInfo->nMsgType += 10;
			ReplyIsoMsgToBal(nBalMsg, IsoInfo, "", CRYPTOGRAPHIC_ERROR_ACTION);
			trace_event("End  BuildAutReqToNw TPK/ZPK Not Present(NOK)", PROCESSING);
			return (NOK);
		}
		if (GetAWK (nIndexCtx, ResourceId, Awk) != OK )
		{
			trace_event("GetAWK Failed ",PROCESSING);
			IsoInfo->nMsgType += 10;
			ReplyIsoMsgToBal(nBalMsg, IsoInfo, "", CRYPTOGRAPHIC_ERROR_ACTION);
			trace_event("End  BuildAutReqToNw ZPK EPS ERROR(NOK)", PROCESSING);
			return (NOK);
		}

		GetIsoField (ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen);
		GetSecurityData( SECURITY_FORMAT, tmpField , security_format, &tmpFieldLen);
		GetSecurityData( PIN_BLOCK_FORMAT, tmpField, PinFormat, &tmpFieldLen);

		if (debug_option == 1)
			sprintf(sLine, "TransKey =[%s]",TransKey);
		else
			sprintf(sLine, "TransKey =[Len %lu]",strlen(TransKey));
		trace_event(sLine, PROCESSING);

		if ( memcmp ( security_format  , "01" , 2 ) == 0 )
			mode = TERMINAL_MODE;
		else
			mode = SCHEME_MODE;

		if (pin_translate (nIndexCtx, CardNumber, TransKey, Awk, PinData, PinFormat, "01", mode) != OK)
		{
			trace_event ("pin_translate Failed", PROCESSING); 
			IsoInfo->nMsgType += 10;
			ReplyIsoMsgToBal (nBalMsg, IsoInfo, 0, CRYPTOGRAPHIC_ERROR_ACTION);
			trace_event("End  BuildAutReqToNw Translation EPS Error(NOK)", PROCESSING);
			return (NOK); 
		}
		AddSmsField (SMS_PIN_DATA, SmsInfo, PinData, 16);


		/* Field 53: Security Data */
		memset (tmpField, 0, sizeof(tmpField));
		memcpy (tmpField, "2001010100000000", 16);
		memset (tmpField1, 0, sizeof(tmpField1));

		GetIsoField (ISO_SECURITY_DATA, IsoInfo, tmpField1, &tmpFieldLen); 
		IsoToSmsSecData (tmpField1, tmpField);
		AddSmsField (SMS_SECURITY_DATA, SmsInfo, tmpField, 16);

	}


	/** Field 55: ICC Data **/
	/*************DREL**********************************************************/
	/*-------------------------------------------------------------------------*/
	/* ICC Data: to be read from ISO field 55 **********************************/
	/*-------------------------------------------------------------------------*/

	if (GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		IsChipTrx = 1;

		InitIsoIcTag ( &IsoIccInfo );
		AnalyseIsoIc  ( tmpField, tmpFieldLen , &IsoIccInfo );
		InitSmsIcTag ( &SmsIccInfo );
		memset(FieldData, 0, sizeof(FieldData));

		if(GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &IsoIccInfo, FieldData, &FieldLen ) == SUCCES )
		{    
			AddSmsIcTag (SMS_TAG_APP_CRYPTOGRAM, &SmsIccInfo, FieldData,  FieldLen );
		}

		/** TAG:9F27   Ommit sur la demande de VISA  ** ** DREL 14052006 ***/


		if (GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag(SMS_TAG_ISS_APP_DATA , &SmsIccInfo, FieldData,  FieldLen );


		if (GetIsoIcTag (ISO_TAG_UNPRED_NUMBER , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_UNPRED_NUMBER , &SmsIccInfo, FieldData,  FieldLen );


		if (GetIsoIcTag (ISO_TAG_ATC , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_ATC , &SmsIccInfo, FieldData,  FieldLen );


		if (GetIsoIcTag (ISO_TAG_TVR , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_TVR , &SmsIccInfo, FieldData,  FieldLen );


		if (GetIsoIcTag (ISO_TAG_TRANS_DATE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_TRANS_DATE , &SmsIccInfo, FieldData,  FieldLen );


		if (GetIsoIcTag (ISO_TAG_TRANS_TYPE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_TRANS_TYPE , &SmsIccInfo, FieldData,  FieldLen );


		/** TAG 9F02 must have the same value as FIELD 04  sur la demande de VISA **/

		if (GetIsoIcTag (ISO_TAG_TRANS_AMOUNT , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_TRANS_AMOUNT , &SmsIccInfo, FieldData,  FieldLen);

		if (GetIsoIcTag (ISO_TAG_TRANS_CUR_CODE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_TRANS_CUR_CODE , &SmsIccInfo, FieldData,  FieldLen );


		if (GetIsoIcTag (ISO_TAG_APP_INTER_PROFILE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_APP_INTER_PROFILE , &SmsIccInfo, FieldData,  FieldLen );

		/** FIELD 9F1A must have the same value as FIELD19=CountryCode  :  sur la demande de VISA  **/

		if (GetIsoIcTag (ISO_TAG_TERM_COUNTRY_CODE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES ) 
			AddSmsIcTag (SMS_TAG_TERM_COUNTRY_CODE , &SmsIccInfo,FieldData , FieldLen);


		/* BURGAN_19 */ /* BURGAN ONSITE CERTIF MKB20171024, TAG should not be sent in case of Acq ATM  ADVT  
		if ( GetIsoIcTag (ISO_TAG_OTHER_AMOUNT , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_OTHER_AMOUNT , &SmsIccInfo, FieldData,  FieldLen );                  

		if ( GetIsoIcTag (ISO_TAG_CVM , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_CVM , &SmsIccInfo, FieldData,  FieldLen );

		if ( GetIsoIcTag (ISO_TAG_TRANS_SEQ_NUM , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_TRANS_SEQ_NUM , &SmsIccInfo, FieldData,  FieldLen );

		*/
		if ( GetIsoIcTag (ISO_TAG_TERM_CAP , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_TERM_CAP , &SmsIccInfo, FieldData,  FieldLen );



		/** TAG:9F35 Ommit sur la demande de VISA **/

/*		if ( GetIsoIcTag (ISO_TAG_IFD_SERIAL_NUM , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_IFD_SERIAL_NUM , &SmsIccInfo, FieldData,  FieldLen );
*/
		if ( GetIsoIcTag (ISO_TAG_TRANS_CATEG_CODE , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_TRANS_CATEG_CODE , &SmsIccInfo, FieldData,  FieldLen );


		if ( GetIsoIcTag (ISO_TAG_DED_FILE_NAME , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_DED_FILE_NAME , &SmsIccInfo, FieldData,  FieldLen );


		if ( GetIsoIcTag (ISO_TAG_TERM_APP_VER_NUM , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_TERM_APP_VER_NUM , &SmsIccInfo, FieldData,  FieldLen );

		/*Start AMER20180307 PROD00054205: Enh18.Q2 Art 3.6*/
		if ( GetIsoIcTag (ISO_TAG_FFI , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_FFI , &SmsIccInfo, FieldData,  FieldLen );
		/*End AMER20180307 PROD00054205: Enh18.Q2 Art 3.6*/
		
		/*Start AMER20180307 PROD00054205: Enh18.Q2 Art 8.1.1*/
		if (GetIsoField(ISO_ACQR_REFERENCE_DATA, IsoInfo, PinData, &FieldLen) == SUCCESS)
		{
			if(GetTlvPrivate (ENCR_PIN_BLOCK_KEY_TAG, &kTlvInfo, TransKey, &tmpFieldLen) != SUCCESS)
			{
				IsoInfo->nMsgType += 10;
				ReplyIsoMsgToBal(nBalMsg, IsoInfo, "", CRYPTOGRAPHIC_ERROR_ACTION);
				trace_event("End  BuildAutReqToNw TPK/ZPK Not Present(NOK)", PROCESSING);
				return (NOK);
			}
			if (GetAWK (nIndexCtx, ResourceId, Awk) != OK )
			{
				trace_event("GetAWK Failed ",PROCESSING);
				IsoInfo->nMsgType += 10;
				ReplyIsoMsgToBal(nBalMsg, IsoInfo, "", CRYPTOGRAPHIC_ERROR_ACTION);
				trace_event("End  BuildAutReqToNw ZPK EPS ERROR(NOK)", PROCESSING);
				return (NOK);
			}

			GetIsoField (ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen);
			GetSecurityData( SECURITY_FORMAT, tmpField , security_format, &tmpFieldLen);
			GetSecurityData( PIN_BLOCK_FORMAT, tmpField, PinFormat, &tmpFieldLen);

			if (debug_option == 1)
				sprintf(sLine, "TransKey =[%s]",TransKey);
			else
				sprintf(sLine, "TransKey =[Len %lu]",strlen(TransKey));
			trace_event(sLine, PROCESSING);

			if ( memcmp ( security_format  , "01" , 2 ) == 0 )
				mode = TERMINAL_MODE;
			else
				mode = SCHEME_MODE;

			if (pin_translate (nIndexCtx, CardNumber, TransKey, Awk, PinData, PinFormat, "01", mode) != OK)
			{
				trace_event ("secondary pin translation Failed", PROCESSING); 
				IsoInfo->nMsgType += 10;
				ReplyIsoMsgToBal (nBalMsg, IsoInfo, 0, CRYPTOGRAPHIC_ERROR_ACTION);
				trace_event("End  BuildAutReqToNw Translation EPS Error(NOK)", PROCESSING);
				return (NOK); 
			}
			
			AddSmsIcTag (SMS_TAG_SEC_PIN_BLOCK, &SmsIccInfo, PinData, 16);
		}
		/*End AMER20180307 PROD00054205: Enh18.Q2 Art 8.1.1*/
		
		FieldLen = SmsBuildIcFld ( sIccBuffer, &SmsIccInfo);


		memset(tmpField1, 0, sizeof(tmpField1));
		tmpField1[0] = 0x01;
		tmpField1[1] = 0x00;
		tmpField1[2] = FieldLen ; 
		memcpy(tmpField1+3, sIccBuffer, FieldLen);

		AddSmsField (SMS_ICC_DATA, SmsInfo, tmpField1, FieldLen+3);

		InitIsoIcTag(&IsoIccInfo);
		InitSmsIcTag(&SmsIccInfo);
		memset(FieldData, 0, sizeof(FieldData));
	}
		
	/*AMER20190329 PROD00067537: Enh19.Q2 Art2.1*/
	GetTlvPrivate( AUTH_ADD_INFO , &kTlvInfo, sAddAuthInfo, &tmpFieldLen);
	
	if(memcmp(isoProcCode, P7_F003_QUASICASH, P7_F003_P1_LEN) == 0)
	{
		if(sAddAuthInfo[P7_F048_P64_CRYPTO_CURR_OFST] == '1')
			sQC_CrptCrcy[0] = 'C';	/*Purchase of cryptcurrecny*/
		else
			sQC_CrptCrcy[0] = 'Q';  /*Quasi-cash transaction*/
	}
	else
		sQC_CrptCrcy[0] = 'N';
	/*End AMER20190329 PROD00067537: Enh19.Q2 Art2.1*/
	
	memset(tmpField1, 0, sizeof(tmpField1));
	memcpy(tmpField1, "25", 2);

	if (IsChipTrx == 0)
	{	/*      AddSmsField (SMS_ADTNL_POS_INFO, SmsInfo, "22", 2); */ /* BURGAN_20  */
		/* AddSmsField (SMS_ADTNL_POS_INFO, SmsInfo, "25", 2); */ /* AHA20171019 BURGAN SITE fallbak case */ 
		if (isoPosMode[6]=='8')
		{
			/*AddSmsField (SMS_ADTNL_POS_INFO, SmsInfo, "2520", 4);*/
			tmpField1[2] = '2';	
		}
		else 
			/*AddSmsField (SMS_ADTNL_POS_INFO, SmsInfo, "25", 2);*/
			tmpField1[2] = '0';
		
		if(sQC_CrptCrcy[0] == 'C')	/*AMER20190329 PROD00067537: Enh19.Q2 Art2.1*/
			tmpField1[3] = '7';
		else if(sQC_CrptCrcy[0] == 'Q')
			tmpField1[3] = '8';
		else
			tmpField1[3] = '0';

	}
	else
	{
		/*memset(tmpField1, 0, sizeof(tmpField1));
		tmpField1 [0] = '2'; 
		tmpField1 [1] = '5';*/ 
		tmpField1 [2] = '0';
		
		if(sQC_CrptCrcy[0] == 'C')	/*AMER20190329 PROD00067537: Enh19.Q2 Art2.1*/
			tmpField1[3] = '7';
		else if(sQC_CrptCrcy[0] == 'Q')
			tmpField1[3] = '8';
		else
			tmpField1[3] = '0';
		
		/*memcpy(tmpField1 +3, "000", 3);*/ 
		memcpy(tmpField1 + 4, "00", 2);
		tmpField1 [6] = '1'; 
		tmpField1 [7] = '0'; 
		tmpField1 [8] = '\0'; 
		/*AddSmsField (SMS_ADTNL_POS_INFO,SmsInfo, tmpField1,strlen(tmpField1));*/
	}

	AddSmsField (SMS_ADTNL_POS_INFO,SmsInfo, tmpField1,strlen(tmpField1));

	InitCpsInfo (&CpsInfo);
	if (!memcmp(MerchantType, "6011", 4)) 
	{
		AddCpsSubField(CPS_ACI, &CpsInfo, "Y", 1);
	}
	if (GetTlvPrivate(MVV_TAG, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{	
		AddCpsSubField(CPS_MVV, &CpsInfo, tmpField, tmpFieldLen);	
	}

	tmpFieldLen = CpsBuildField (tmpField, &CpsInfo);
	if (tmpFieldLen > 8)
		AddSmsField (SMS_CPS_FIELDS, SmsInfo, tmpField, tmpFieldLen);	
		


	/* Field 63: SMS Private Use Field */
	InitPufInfo (&PufInfo);
	AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0000", 4); /* Priority Routing */
	/*Start IBO20190823 PROD00071113 Enh19.Q4 Art 2.1*/
	if (GetIsoField (ISO_FUNCTION_CODE, IsoInfo,sFunctionCode,&tmpFieldLen) == SUCCESS) /*IZL*/
	{
		if(memcmp(sFunctionCode, P7_F024_DEFERRED_AUTH, tmpFieldLen) == 0)
		AddPufSubField (PUF_MSG_REASON, &PufInfo, MSG_REASON_DEFERRED_AUTH, 4);
	}
	/*End IBO20190823 PROD00071113 Enh19.Q4 Art 2.1*/
	tmpFieldLen = PufBuildField (tmpField, &PufInfo);                          
	AddSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, tmpFieldLen);
	
	/*Start AMER20190219 PROD00065991: VISA Enh19.Q2 Art2.4*/
	if (GetTlvPrivate(MERCHANT_ADDITIONAL_DATA, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kIsoPayFacTlv);
		
		if( GetTlvInfoField (P7_F048P21_PAYFAC_ID, &kIsoPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T56_PAY_FAC_ID, &kSmsPayFacTlv, tmpField, tmpFieldLen);
		}
		
		if( GetTlvInfoField (P7_F048P21_SUB_MER_ID, &kIsoPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			PutTlvInfoField(SMS_F104T56_SUB_MER_ID, &kSmsPayFacTlv, tmpField, tmpFieldLen);
		}
		
		if( GetTlvInfoField (P7_F048P21_ISO_ID, &kIsoPayFacTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			PutTlvInfoField(SMS_F104T56_ISO_ID, &kSmsPayFacTlv, tmpField, tmpFieldLen);
		}
			
		tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kSmsPayFacTlv);
		if( tmpFieldLen > 0 )
		{
			AddTlvInfoField(SMS_F104_PAY_FAC_DATA, &kSmsTransSpecTlv, tmpField, tmpFieldLen);
		}
		
		/*tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kSmsTransSpecTlv);
		if (tmpFieldLen > 0)
			AddSmsField(SMS_TRANS_DESC, SmsInfo, tmpField, tmpFieldLen);*/
	}
	/*End AMER20190219 PROD00065991: VISA Enh19.Q2 Art2.4*/

	/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if (GetTlvPrivate(P7_F048_LOAN_DATA, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kIsoRecurringPayTlv);

		if( GetTlvInfoField (P7_F048S24_NBR_INSTALMENTS, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T18_NBR_RECURRING_PAY, &kSmsRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_FREQ_RECUR_PAY, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T18_FREQ_RECURRING_PAY, &kSmsRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_REGISTRATION_REF_NBR, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T18_REGISTR_REF_NBR, &kSmsRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_MONTHLY_RPYMNT_AMNT, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T18_MAX_RECURRING_PAY_AMNT, &kSmsRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}
		/*Start FZL 03102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/
		if( GetTlvInfoField (P7_F048S24_VALIDATION_IND, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T18_VALIDATION_IND, &kSmsRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}
		/*End FZL 03102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/ 
	}
    GetTlvPrivate( RECURRING_FLAG_TAG , &kTlvInfo, RecurringFlag, &tmpFieldLen);/* FZL 11102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/
	if(memcmp(sFunctionCode, P7_F024_RECURRING_MODIF, P7_F024_LEN) == 0 && atoi(TrxAmount) == 0)
	{
		AddTlvInfoField(SMS_F104T18_RECURRING_PAY_TYPE, &kSmsRecurringPayTlv, SMS_F104T18_T80_MODIFICATION, SMS_F104T18_RECURRING_PAY_TYPE_LEN);
		nNbrTags++;
	}
	else if(memcmp(sFunctionCode, P7_F024_RECURRING_CANCEL, P7_F024_LEN) == 0 && atoi(TrxAmount) == 0)
	{
		AddTlvInfoField(SMS_F104T18_RECURRING_PAY_TYPE, &kSmsRecurringPayTlv, SMS_F104T18_T80_CANCELLATION, SMS_F104T18_RECURRING_PAY_TYPE_LEN);
		nNbrTags++;
	}
	else if (strlen(RecurringFlag) > 0
		&&(RecurringFlag[0] ==P7_F048_P71_CRED_ON_FILE
		||RecurringFlag[0]==P7_F048_P71_INSTALLMENT
		||RecurringFlag[0]==P7_F048_P71_RECURRING
		||RecurringFlag[0]==P7_F048_P71_STANDING))
	{
		if(strlen(sAddAuthInfo) > P7_F048_P64_CRDNTL_REASON
			&& sAddAuthInfo[P7_F048_P64_CRDNTL_REASON] != P7_F048_P64_P7_UNSPECIFIED)
			{
				AddTlvInfoField(SMS_F104T18_RECURRING_PAY_TYPE, &kSmsRecurringPayTlv, SMS_F104T18_T80_REGISTRATION_FIRST_TRANS, SMS_F104T18_RECURRING_PAY_TYPE_LEN);
				nNbrTags++;
			}
			else
			{
				AddTlvInfoField(SMS_F104T18_RECURRING_PAY_TYPE, &kSmsRecurringPayTlv, SMS_F104T18_T80_SUBSEQ_TRANS, SMS_F104T18_RECURRING_PAY_TYPE_LEN);
				nNbrTags++;
			}
	}

	if (strlen(RecurringFlag) > 0
		&&(RecurringFlag[0]==P7_F048_P71_RECURRING
		||RecurringFlag[0]==P7_F048_P71_INSTALLMENT))
	{
		AddTlvInfoField(SMS_F104T18_PAY_AMNT_IND_PER_TRANS, &kSmsRecurringPayTlv, SMS_F104T18_T81_FIX_AMNT_IND_PER_TRANS, SMS_F104T18_PAY_AMNT_IND_PER_TRANS_LEN);
		nNbrTags++;
	}
	else if(strlen(RecurringFlag) > 0
		&&(RecurringFlag[0]==P7_F048_P71_STANDING
		||RecurringFlag[0]==P7_F048_P71_CRED_ON_FILE))
	{
		AddTlvInfoField(SMS_F104T18_PAY_AMNT_IND_PER_TRANS, &kSmsRecurringPayTlv, SMS_F104T18_T81_RECUR_PAY_MAX_AMNT, SMS_F104T18_PAY_AMNT_IND_PER_TRANS_LEN);
		nNbrTags++;
	}

	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kSmsRecurringPayTlv);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(SMS_F104_RECURRING_PAY, &kSmsTransSpecTlv, tmpField, tmpFieldLen);
	}

	if(strlen(RecurringFlag) > 0
		&& nNbrTags != SMS_F104_T18_NB_FIELDS /*- 1*/  /*FZL 03102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/
		&& GetIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS
		&& memcmp(tmpField,"356", 3) == 0) /*Indian Cards*/
	{
		trace_message(P7_TL_ERROR, "Tags missing in TLV Field 104, Usage 2, Dataset ID 18 for Indian Card");
		trace_message(P7_TL_PROCESSING, "End  BuildAutReqToNw ERROR(NOK)");
		return (NOK);
	}
	/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kSmsTransSpecTlv);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if (tmpFieldLen > 0)
		AddSmsField(SMS_TRANS_DESC, SmsInfo, tmpField, tmpFieldLen);

	if (memcmp (isoProcCode, "01", 2)== 0)
	{ 	
		GetIsoField (ISO_OUTLET_NBR, IsoInfo, Outlet, &tmpFieldLen);
		if(GetTlvPrivate (ACQUIRER_BANK_TAG, &kTlvInfo, TransKey, &tmpFieldLen) != SUCCESS)
		{
			IsoInfo->nMsgType += 10;
			ReplyIsoMsgToBal(nBalMsg, IsoInfo, "", FIELD_CONTROL_ERROR_ACTION);
			trace_event("End  BuildAutReqToNw Acquirer bank Not Present(NOK)", PROCESSING);
			return (NOK);
		}

		if (GetNnsCalculatedFee (nIndexCtx, CardNumber,TrxAmount,TrxCurrency,szAcquirerBank, CalcFee) == OK )
		{
			AddSmsField (SMS_SETT_SERVICE_DATA, SmsInfo, CalcFee, strlen(CalcFee));
		}
	}
	
	/*Field 125 : Supporting Information*/
	/*Start IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	if (GetTlvPrivate(ORIGINAL_TRX_ID, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		if(memcmp (tmpField,"0100000000000000",tmpFieldLen) != 0)
		{
			InitSmsAddOriginalDataTlv(&kSmsTlvInfo);
			AddTlvInfoField(SMS_F125T03_ORIGINAL_TRA_ID, &kSmsTlvInfo, tmpField, tmpFieldLen);
			tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kSmsTlvInfo);
			if( tmpFieldLen > 0 )
			{
				AddTlvInfoField(SMS_F125_ADD_ORIGINAL_DATA, &kSmsSuppInfTlv, tmpField, tmpFieldLen);
			}
			tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kSmsSuppInfTlv);
			if (tmpFieldLen > 0)
				AddSmsField(SMS_SUPPORT_INFO, SmsInfo, tmpField, tmpFieldLen);
		}
		
		
	}
	/*End IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	
	/*Start HAL 20201903 ENHS 2020Q2 ART 9.3.3*/
	if (GetIsoField(ISO_NETWORK_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitTlvPrivate(&kNwTlvData);
		AnalyseTlvPrivate(tmpField,tmpFieldLen,&kNwTlvData);
		
		InitSmsPufInfo(&SmspufInfo); 

		if(GetTlvPrivate( NET_VISA_MERC_ID , &kNwTlvData, tmpField, &tmpFieldLen) == SUCCESS)
			AddSmsPufSubField(SPUF_VPUF_MER_ID, &SmspufInfo , tmpField, tmpFieldLen);

		FieldLen = SmsPufBuildField(tmpField,&SmspufInfo);
		
		if (FieldLen > 0)
			AddSmsField (SMS_PRIV_USE_FIELD , SmsInfo, tmpField, FieldLen);
	}
    /*End HAL 20201903 ENHS 2020Q2 ART 9.3.3*/ 

	memset(sCpsTlvData, 0, sizeof(sCpsTlvData));
	memset(sIccBuffer, 0, sizeof(sIccBuffer));
	memset(tmpField1, 0, sizeof(tmpField1));
	memset(tmpField, 0, sizeof(tmpField));
	memset(FieldData, 0, sizeof(FieldData));
	memset(PinData, 0, sizeof(PinData));
	memset(PinFormat, 0, sizeof(PinFormat));
	memset(TransKey, 0, sizeof(TransKey));
	memset(Awk, 0, sizeof(Awk));
	memset(CardNumber, 0, sizeof(CardNumber));
	memset(PVV, 0, sizeof(PVV));
	memset(CVV, 0, sizeof(CVV));
	memset(T2_Data, 0, sizeof(T2_Data));
	memset(ExpiryDate, 0, sizeof(ExpiryDate));

	InitTlvPrivate (&kTlvInfo);
	IniIsoPayFacDataTlv(&kIsoPayFacTlv);
	InitSmsTransSpecDataTlv(&kSmsTransSpecTlv);
	InitSmsPayFacDataTlv(&kSmsPayFacTlv);
	InitSmsElectComDataTlv(&kSmsElectComTlv);		
	InitSmsSupportInfDataTlv(&kSmsSuppInfTlv);
	InitIsoIcTag ( &IsoIccInfo );
	InitSmsIcTag ( &SmsIccInfo );
	InitCpsInfo (&CpsInfo);	
	InitPufInfo (&PufInfo);	
	InitSmsAddOriginalDataTlv(&kSmsTlvInfo);
	
	trace_event("End  BuildAutReqToNw(OK)", PROCESSING);
	return(OK);

}

