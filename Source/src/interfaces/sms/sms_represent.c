#include <define.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <resources.h>
#include <sms_var.h>
#include <sms_param.h>
#include <sms_fields.h>

#include <iso_cps.h>
#include <iso_puf.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <tlv_private.h>
#include <tlv_autho.h>

#include <list_thr.h>
#include <queue.h>
#include <security_data.h>
#include <hsm_inc.h>

/***********************************************************************/
/***                   FCT_REPR_TO_NW ()                             ***/
/***    sends a representation advice message (0220) to SMS          ***/
/***********************************************************************/
int FCT_REPR_TO_NW (int nIndexCtx, TSIsoInfo    *IsoInfo)
{
	TSTlvAutho            sInternalTlvInfo;
	TSTlvPrivate          TlvInfo;
	TSSmsInfo             SmsInfo;
	TSNodeData            Node;
	char                  FieldData        [BUF_LG_MAX];
	char                  sBuffer          [LG_MAX];
	char                  sLine            [MAX_LINE_TRC];
	char                  sInternalTlvBuff [BUF_LG_MAX ];
	char                  sPurgeTime       [10];
	int                   FieldLen;
	int                   tmpFieldLen;
	int                   Length;
	int                   nRetCode;

	trace_event ("--------------------------", PROCESSING);
	trace_event ("Start FCT_REPR_TO_NW()    ", PROCESSING);
	trace_event ("--------------------------", PROCESSING);

	/**** Initialisation des variables et structures de travail ****/
	memset (FieldData,              0,    sizeof (FieldData));
	memset (sBuffer,                0,    sizeof (sBuffer));
	memset (sInternalTlvBuff,       0,    sizeof (sInternalTlvBuff));
	memset (sPurgeTime,             0,    sizeof (sPurgeTime));
	memset (sLine,                  0,    sizeof (sLine));

	InitTlvAutho( &sInternalTlvInfo);
	InitSmsInfo (&SmsInfo);

	dump_p7_struct(IsoInfo);

	/**** Insert ISO message in list for later retrieval ****/

	InitNodeData (&Node);
	GetIsoField (ISO_SECURITY_DATA, IsoInfo, FieldData, &FieldLen);
	GetSecurityData(PURGE_TIME,FieldData,sPurgeTime,&FieldLen);

	Node.tPurgeTime= atol(sPurgeTime);

	GetIsoField (ISO_AUDIT_NBR, IsoInfo, FieldData, &FieldLen);
	memcpy (Node.szFstKey, "1220", 4);
	memcpy (Node.szFstKey + 4,  FieldData, 6);

	Length = IsoBuildMsg (Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	if (InsertRequest('B', &Node) != OK)
	{
		trace_event ("FCT_REPR_TO_NW(NOK): InsertRequest() failed.", ERROR);
		return(NOK);
	}

	ReprAdvToSms(nIndexCtx, IsoInfo, &SmsInfo); 

	/* Conversion a partir du format IsoInfo au format TlvInfo */
	nRetCode = IsoToTlv( nIndexCtx, IsoInfo, &sInternalTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert the ISO message %04d into the Tlv format", IsoInfo->nMsgType);
		trace_event (sLine, PROCESSING);
		return (NOK);
	}

	/******** Construction de la chaine TLV ***********/
	PutTlvAutho  ( AUTO_CUR_TABLE_INDICATOR, &sInternalTlvInfo, "MSC", 3);
	PutTlvAutho  ( AUTO_ROUTING_CODE, &sInternalTlvInfo, ResourceId, 6);
	PutTlvAutho  ( AUTO_CAPTURE_CODE, &sInternalTlvInfo, ResourceId, 6);

	/* 48 in TLV format. */
	InitTlvPrivate (&TlvInfo);
	AddTlvPrivate (HPS_ACQUIRER_BANK_TAG , &TlvInfo, AcquirerBank, 6);
	if (GetIsoField (ISO_AUDIT_NBR, IsoInfo, FieldData, &tmpFieldLen) == SUCCES)
		AddTlvPrivate (EXTERNAL_STAN_TAG , &TlvInfo, FieldData, 6);
	if (GetIsoField (ISO_ADTNL_RESP_DATA, IsoInfo, FieldData, &tmpFieldLen) == SUCCESS)
		AddTlvPrivate (CVV_RESULT_CODE_TAG, &TlvInfo, FieldData+4, 1);

	tmpFieldLen = PrivateBuildTlv (FieldData, &TlvInfo);
	if (tmpFieldLen > 0)
		PutTlvAutho  ( AUTO_ADD_DATA_PRIVATE, &sInternalTlvInfo, FieldData, tmpFieldLen);

	AuthoBuildTlv( sInternalTlvBuff, &sInternalTlvInfo);

	/********* Insertion dans Autho Activity **********/
	nRetCode = InsertionAuthoActivity(nIndexCtx, sInternalTlvBuff);
	if (nRetCode != OK )
	{
		trace_event  ( "Error On Insert Autho Activity File ", PROCESSING );
		return (NOK);
	}

	/************** Send the request to Network  ************************/
	Length = SmsBuildMsg(sBuffer + SMS_HDR_LEN, &SmsInfo);
	SmsBuildHeader(REQUEST , &(SmsInfo.sHeader), NULL, Length, StationId);
	memcpy(sBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
	Length += SMS_HDR_LEN;
	DumpSms(sBuffer);

	if (WriteLineMsg(sBuffer, Length) != OK)
	{
		trace_event ("FCT_REPR_TO_NW(NOK): WriteLineMsg() failed", ERROR);
		return (NOK);
	}

	
	trace_event ("End FCT_REPR_TO_NW()      ", PROCESSING);
	
	return (OK); 
}

/*********************************************************************************/
/***                   FCT_REPR_RPLY_FROM_NW ()                                ***/
/***  Receives & converts a representation response message (0230) from SMS    ***/
/*********************************************************************************/
int FCT_REPR_RPLY_FROM_NW (int         nIndiceCtx, 
	TSSmsInfo   *smsInfo)
{
	TSTlvAutho          sInternalTlvInfo;
	TSBalExchange       BalMessage;
	TSNodeData          node;
	TSIsoInfo           isoInfo;
	char                sInternalTlvBuff     [LG_MAX ];
	char                data                 [BUF_LG_MAX];
	char                srcId                [7];
	char                sLine                [MAX_LINE_TRC];
	int                 length;
	int                 nRetCode;

	
	trace_event ("Start FCT_REPR_RPLY_FROM_NW() ", PROCESSING);
	

	/**** Initialisation des variables et structures de travail ****/
	memset (data,                0,    sizeof (data)); 
	memset (sInternalTlvBuff,    0,    sizeof (sInternalTlvBuff));
	memset (srcId,               0,    sizeof (srcId));
	memset (sLine,               0,    sizeof (sLine));

	InitTlvAutho( &sInternalTlvInfo);

	InitIsoInfo (&isoInfo);

	/**** Insert ISO message in list for later retrieval ****/
	
	InitNodeData (&node);
	memcpy (node.szFstKey, "1220", 4);
	GetSmsField (SMS_AUDIT_NBR, smsInfo, data, &length);
	memcpy (node.szFstKey + 4,  data, 6);

	switch (FetchRequest ('B', &node))
	{
	case ENOEXIST:
		trace_event ("FetchRequest failed (ENOEXIST)", ERROR);
		return (NOK);

	case ELATE:
		break;
	}

	AnalyseIso (node.sFstBuf, &isoInfo);
	RepresentRespFromSms (nIndiceCtx, smsInfo, &isoInfo);

	GetIsoField (ISO_SECURITY_DATA, &isoInfo, data, &length);
	GetSecurityData(SOURCE_PID,data,srcId,&length);

	BalMessage.sHeader.nPartnerId = atoi(srcId);
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1230;
	length = IsoBuildMsg (BalMessage.sBody, &isoInfo);
	BalMessage.sHeader.nLen = length;

	if (WriteBalMsg (nBalMsg, &BalMessage) != OK)
	{
		trace_event ("FCT_REPR_RPLY_FROM_NW(NOK): WriteBalMsg() failed.", ERROR);
		return (NOK);
	}

	
	trace_event ("End FCT_REPR_RPLY_FROM_NW (OK)", PROCESSING);
	

	return (OK);
}

/**********************************************************************/
/**********              ReprAdvToSms ()                   ************/
/**  Prepares SMS Representment message 0220 to be sent to network  ***/
/**  Input: IsoInfo: structure containing ISO-format message        ***/
/**         SmsInfo: structure to contain SMS message to be sent    ***/
/**********************************************************************/
int ReprAdvToSms (int         nIndexCtx,
	TSIsoInfo   *IsoInfo,
	TSSmsInfo   *SmsInfo)
{
	TSTlvPrivate   TlvInfo;
	TSCpsInfo      CpsInfo;
	TSPufInfo      PufInfo;
	char           tmpField     [BUF_LG_MAX];
	char           tmpField1    [BUF_LG_MAX];
	char           sMcc         [4+1];
	char           sIsoProcCode [6+1];
	int            tmpFieldLen;
	int            tmpFieldLen1;

	
	trace_event ("Start  ReprAdvToSms()     ", PROCESSING);
	

	/****** Initialisation des variables de travail ****/
	
	memset (sMcc,         0, sizeof(sMcc));
	memset (sIsoProcCode, 0, sizeof(sIsoProcCode));
	memset (tmpField,     0, sizeof(tmpField));
	memset (tmpField1,    0, sizeof(tmpField1));

	InitSmsInfo (SmsInfo);

	SmsInfo->nMsgType = 0220;

	/* Field 2: Primary Account Number */
	GetIsoField (ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_CARD_NBR, SmsInfo, tmpField, tmpFieldLen);

	/* Field 3: Processing Code */
	GetIsoField (ISO_PROC_CODE, IsoInfo, sIsoProcCode, &tmpFieldLen);
	GetIsoField (ISO_MERCHANT_TYPE, IsoInfo, sMcc, &tmpFieldLen);
	IsoToSmsProcessingCode (sIsoProcCode, tmpField, sMcc);
	AddSmsField (SMS_PROC_CODE, SmsInfo, tmpField, 6);

	/* Field 4: Transaction Amount */
	GetIsoField (ISO_TRANS_AMOUNT, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_AMOUNT, SmsInfo, tmpField, 12);

	/* Field 7: Transmission Date and Time */
	GetIsoField (ISO_XMIT_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, 10);

	/* Field 11: System Trace Audit Number */
	GetIsoField (ISO_AUDIT_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, 6);

	/* Field 13: Transaction Local Date */
	GetIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_DATE, SmsInfo, tmpField + 2, 4);

	/* Field 18: Merchant Type */
	GetIsoField (ISO_MERCHANT_TYPE, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_MERCHANT_TYPE, SmsInfo, tmpField, 4);

	/* Field 19: Acquiring Institution Country Code */
	GetIsoField (ISO_ACQR_COUNTRY, IsoInfo, tmpField, &tmpFieldLen );
	AddSmsField (SMS_ACQR_COUNTRY, SmsInfo, tmpField, 3);

	/* Field 20: PAN Country Code */
	if (GetIsoField (ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_PAN_COUNTRY, SmsInfo, tmpField, 3);

	/* Field 21: Forwarding Institution Country Code */
	if (GetIsoField (ISO_FORWD_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_FORWD_COUNTRY, SmsInfo, sForwarderCountry, 3);

	/* Field 25: POS Condition Code */
	AddSmsField (SMS_POS_CONDITION, SmsInfo, "13", 2);

	/* Field 28: Transaction Fee Amount */
	if (GetIsoField (ISO_FEES_AMOUNT, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_TRANS_FEE, SmsInfo, tmpField, tmpFieldLen);

	/* Field 32: Acquiring Institution Identification Code */
	GetIsoField (ISO_ACQR_ID, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_ACQR_ID, SmsInfo, tmpField, tmpFieldLen);

	/* Field 33: Forwarding Institution Id Code */
	if (GetIsoField (ISO_FORWD_ID, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_FORWD_ID, SmsInfo, tmpField, tmpFieldLen);

	/* Field 37: Retrieval Reference Number */
	GetIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, 12);

	/* Field 41: Terminal Identification */
	GetIsoField (ISO_TERMINAL_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TERMINAL_NBR, SmsInfo, tmpField, 8);

	/* Field 42: Card Acceptor Identification Code */
	GetIsoField (ISO_OUTLET_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_OUTLET_NBR, SmsInfo, tmpField, 15);

	/* Field 43: Card Acceptor Location */
	GetIsoField (ISO_TERMINAL_ADR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TERMINAL_ADR, SmsInfo, tmpField, tmpFieldLen);

	/* Field 44: Additional Response Data */
	if (GetIsoField (ISO_ADTNL_RESP_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_ADTNL_RESP_DATA, SmsInfo, tmpField, tmpFieldLen);

	/* Field 48: Additional Data Private */
	GetIsoField (ISO_NETWORK_DATA, IsoInfo, tmpField, &tmpFieldLen);
	if (!memcmp (tmpField, "0002", 4))  /* not used for PLUS */
	{
		memset (tmpField1,      0, sizeof(tmpField1));

		memcpy (tmpField1,      "V", 1);     /* Identifier  */
		memcpy (tmpField1 + 1,   "2", 1);     /* Usage Code  */

		GetIsoField (ISO_RECORD_DATA, IsoInfo, tmpField, &tmpFieldLen);
		memcpy (tmpField1 + 2,   tmpField, tmpFieldLen);     /* Doc Indicator & Chargeback Reference & Msg Txt */
		AddSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField1, tmpFieldLen+2);
	}

	/* Field 49: Transaction Currency Code */
	GetIsoField (ISO_TRANS_CRNCY, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_CRNCY, SmsInfo, tmpField, 3);

	/* Field 62: CPS fields */
	if (GetIsoField (ISO_CPS_FIELDS, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitCpsInfo (&CpsInfo);
		AddCpsSubField (CPS_ACI, &CpsInfo, tmpField, 1);
		if (tmpFieldLen > 1)    /* we have a Transaction Id */
			AddCpsSubField (CPS_TRANS_ID, &CpsInfo, tmpField + 1, 15);

		tmpFieldLen = CpsBuildField (tmpField, &CpsInfo);
		AddSmsField (SMS_CPS_FIELDS, SmsInfo, tmpField, tmpFieldLen);
	}

	/* Field 63: SMS Private Use Field */
	InitPufInfo (&PufInfo);
	InitTlvPrivate (&TlvInfo);
	GetIsoField (ISO_NETWORK_DATA, IsoInfo, tmpField, &tmpFieldLen);
	AnalyseTlvPrivate (tmpField, tmpFieldLen, &TlvInfo);
	if (GetTlvPrivate (NET_NETWORK_ID_TAG, &TlvInfo, tmpField ,  &tmpFieldLen) == SUCCESS) 
		AddPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, 4);

	GetIsoField (ISO_MSG_REASON, IsoInfo, tmpField, &tmpFieldLen);
	AddPufSubField (PUF_MSG_REASON, &PufInfo, tmpField, 4);

	GetIsoField (ISO_NETWORK_DATA, IsoInfo, tmpField1, &tmpFieldLen);
	if (!memcmp (tmpField1, "0004", 4))  /* Only used for PLUS */
		AddPufSubField (PUF_PLUS_PMC, &PufInfo, PlusPmc, 3);

	tmpFieldLen = PufBuildField (tmpField, &PufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, tmpFieldLen);

	trace_event ("End ReprAdvToSms()", PROCESSING);

	return(OK);

}

/********************************************************************************/
/*             RepresentRespFromSms()                                           */
/*                                                                              */
/*  Puts the SMS representment response message in an ISO-format structure      */
/*  Input: SmsInfo: structure containing received SMS response                  */
/*         IsoInfo: will contain ISO-format response                            */
/********************************************************************************/
int RepresentRespFromSms(int        nIndexCtx,
	TSSmsInfo  *SmsInfo,
	TSIsoInfo  *IsoInfo)
{
	char        yymmdd        [8];
	char        tmpField      [256];
	char        tmpField1     [256];
	char        sRespCode     [3+1];
	int         tmpFieldLen;


	trace_event("Start RepresentRespFromSms()", PROCESSING);

	memset(yymmdd,     0, sizeof(yymmdd));
	memset(tmpField,   0, sizeof(tmpField));
	memset(sRespCode,  0, sizeof(sRespCode));

	current_date_yymmdd(yymmdd);

	IsoInfo->nMsgType = 1230;

	/* Field 5: Settlement Amount */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		InsertIsoField (ISO_STLMNT_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 9: Settlement Conversion Rate */
	if (GetSmsField (SMS_CNVRSN_RATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		InsertIsoField (ISO_STLMNT_CNVRSN_RATE, IsoInfo, tmpField, tmpFieldLen);

	/* Field 15: Settlement Date */
	GetSmsField (SMS_STLMNT_DATE, SmsInfo, tmpField, &tmpFieldLen);
	memcpy (tmpField1, yymmdd, 2);
	memcpy (tmpField1 + 2, tmpField, tmpFieldLen);
	InsertIsoField (ISO_STLMNT_DATE, IsoInfo, tmpField1, 6);

	/* Field 16: Conversion Date */
	if (GetSmsField (SMS_CNVRSN_DATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		InsertIsoField (ISO_CNVRSN_DATE, IsoInfo, tmpField, tmpFieldLen);

	/* Field 39: Response Code */
	if (GetSmsField (SMS_RESPONSE_CODE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		TranslateRespCode (nIndexCtx, ResourceId, tmpField, "RP", sRespCode );
		InsertIsoField (ISO_RESPONSE_CODE, IsoInfo, sRespCode, 3);
	}

	trace_event("End RepresentRespFromSms (OK)", PROCESSING);

	return(OK);
}





