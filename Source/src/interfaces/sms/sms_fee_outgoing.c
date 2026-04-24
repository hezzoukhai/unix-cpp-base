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
/***                   FCT_FEE_ADV_TO_NW ()                          ***/
/***    sends a fee collection advice message (0220) to SMS          ***/
/***********************************************************************/
int FCT_FEE_ADV_TO_NW (int nIndexCtx, TSIsoInfo    *IsoInfo)
{
	TSTlvAutho            sInternalTlvInfo;
	TSSmsInfo             SmsInfo;
	TSNodeData            Node;
	char                  FieldData         [LG_MAX];
	char                  sLine             [MAX_LINE_TRC];
	char                  sInternalTlvBuff  [LG_MAX ];
	char                  sPurgeTime        [10];
	char                  sBuffer           [LG_MAX];
	int                   FieldLen;
	int                   Length;
	int                   nRetCode; 

	trace_event ("Start FCT_FEE_ADV_TO_NW()", PROCESSING);

	memset (sBuffer,                0,    sizeof(sBuffer));
	memset (sInternalTlvBuff,       0,    sizeof (sInternalTlvBuff));
	memset (sLine,                  0,    sizeof (sLine));

	InitTlvAutho( &sInternalTlvInfo);

	InitNodeData (&Node);
	GetIsoField (ISO_SECURITY_DATA, IsoInfo, FieldData, &FieldLen);
	GetSecurityData(PURGE_TIME,FieldData,sPurgeTime, &FieldLen);

	Node.tPurgeTime= atol(sPurgeTime);

	GetIsoField (ISO_AUDIT_NBR, IsoInfo, FieldData, &FieldLen);
	memcpy (Node.szFstKey, "1220", 4);
	memcpy (Node.szFstKey + 4,  FieldData, 6);

	Length = IsoBuildMsg (Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	
	dump_p7_struct (IsoInfo);

	if (InsertRequest('B', &Node) != OK)
	{
		trace_event ("FCT_FEE_ADV_TO_NW(NOK): InsertRequest() failed.", ERROR);
		return(NOK);
	}

	FeeAdvToSms( nIndexCtx , IsoInfo, &SmsInfo);

	/* Conversion a partir du format IsoInfo au format TlvInfo */
	nRetCode = IsoToTlv( nIndexCtx, IsoInfo, &sInternalTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert the ISO message %04d into the Tlv format", IsoInfo->nMsgType);
		trace_event (sLine, PROCESSING);
		return (NOK);
	}

	/******** Construction de la chaine TLV ***********/
	AuthoBuildTlv( sInternalTlvBuff, &sInternalTlvInfo);

	/********* Insertion dans Autho Activity **********/
	nRetCode = InsertionAuthoActivity(nIndexCtx, sInternalTlvBuff);
	if ( nRetCode != OK )
	{
		trace_event  ( "Error On Insert Autho Activity File ", PROCESSING );
		return (NOK);
	}


	/************** Send the request to Network  ************************/
	Length = SmsBuildMsg(sBuffer + SMS_HDR_LEN, &SmsInfo);
	SmsBuildHeader(REQUEST , &(SmsInfo.sHeader), NULL, Length, StationId);
	memcpy(sBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
	Length += SMS_HDR_LEN;

	dump_buffer(sBuffer, Length, 'E', 'L', 'O');

	DumpSms(sBuffer);

	if (WriteLineMsg(sBuffer, Length) != OK)
	{
		trace_event ("FCT_FEE_ADV_TO_NW(NOK): WriteLineMsg() failed.", ERROR);
		return (NOK);
	}

	trace_event ("End FCT_FEE_ADV_TO_NW()", PROCESSING);
	return (OK);
}

/*********************************************************************************/
/***                   FCT_FEE_RPLY_FROM_NW ()                                 ***/
/***  Receives & converts a fee Collection response message (0230) from SMS    ***/
/*********************************************************************************/
int FCT_FEE_RPLY_FROM_NW (int         nIndiceCtx,
	TSSmsInfo   *smsInfo)
{
	TSTlvAutho          sInternalTlvInfo;
	TSBalExchange       BalMessage;
	TSNodeData          node;
	TSIsoInfo           isoInfo;
	char                data                 [BUF_LG_MAX];
	char                src_pid                [7];
	char                sInternalTlvBuff     [LG_MAX ];
	char                sLine                [MAX_LINE_TRC];
	int                 length;
	int                 nRetCode;

	trace_event ("Start FCT_FEE_RPLY_FROM_NW()", PROCESSING);

	memset (data,                   0,    sizeof (data));
	memset (sInternalTlvBuff,       0,    sizeof (sInternalTlvBuff));
	memset (sLine,                  0,    sizeof (sLine));

	InitTlvAutho( &sInternalTlvInfo);

	GetSmsField (SMS_AUDIT_NBR, smsInfo, data, &length);

	InitNodeData (&node);
	memcpy (node.szFstKey, "0220", 4);
	memcpy (node.szFstKey + 4,  data, 6);

	switch (FetchRequest ('B', &node))
	{
	case ENOEXIST:
		trace_event ("FetchRequest failed (ENOEXIST)", ERROR);
		return;

	case ELATE:
		break;
	}

	AnalyseIso (node.sFstBuf, &isoInfo);

	FeesResponseFromSms (nIndiceCtx, smsInfo, &isoInfo); 

	/* Conversion a partir du format IsoInfo au format TlvInfo */
	nRetCode = IsoToTlv( nIndiceCtx, &isoInfo, &sInternalTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert the ISO message %04d into the Tlv format", isoInfo.nMsgType);
		trace_event (sLine, PROCESSING);
		return (NOK);
	}

	/******** Construction de la chaine TLV ***********/
	AuthoBuildTlv( sInternalTlvBuff, &sInternalTlvInfo);

	/********* Insertion dans Autho Activity **********/
	nRetCode = InsertionAuthoActivity(nIndiceCtx, sInternalTlvBuff);
	if ( nRetCode != OK )
	{
		trace_event  ( "Error On Insert Autho Activity File ", PROCESSING );
		return (NOK);
	}

	GetIsoField (ISO_SECURITY_DATA, &isoInfo, data, &length);
	GetSecurityData(SOURCE_PID,data,src_pid, &length);

	BalMessage.sHeader.nPartnerId = atoi(src_pid);
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1230;
	length = IsoBuildMsg (BalMessage.sBody, &isoInfo);
	BalMessage.sHeader.nLen = length;

	if (WriteBalMsg (nBalMsg, &BalMessage) != OK)
	{
		trace_event ("FCT_FEE_RPLY_FROM_NW(NOK): WriteBalMsg() failed.", ERROR);
		return (NOK);
	}

	trace_event ("End FCT_FEE_RPLY_FROM_NW (OK)", PROCESSING);

	return (OK);
}

/**********************************************************************/
/**********              FeeAdvToSms ()                    ************/
/***   Converts and sends an sms fee collection message (0220)      ***/
/**********************************************************************/
int FeeAdvToSms (int         nIndexCtx,
	TSIsoInfo   *IsoInfo,
	TSSmsInfo   *SmsInfo)
{
	TSTlvPrivate   TlvInfo;
	TSPufInfo      PufInfo;
	char           tmpField[BUF_LG_MAX];
	char           tmpField1[BUF_LG_MAX];
	int            tmpFieldLen;
	int            tmpFieldLen1;

	trace_event ("Start  FeeAdvToSms()", PROCESSING);

	memset(tmpField,      0,  sizeof(tmpField));
	memset(tmpField1,     0,  sizeof(tmpField1));
	

	InitSmsInfo (SmsInfo);

	SmsInfo->nMsgType = 0220;

	/* Field 2: Primary Account Number */
	if (GetIsoField (ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_CARD_NBR, SmsInfo, tmpField, tmpFieldLen);

	/* Field 3: Processing Code */
	GetIsoField (ISO_PROC_CODE, IsoInfo, tmpField, &tmpFieldLen);
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

	/* Field 19: Acquiring Institution Country Code */
	GetIsoField (ISO_ACQR_COUNTRY, IsoInfo, tmpField, &tmpFieldLen );
	AddSmsField (SMS_ACQR_COUNTRY, SmsInfo, tmpField, 3);

	/* Field 32: Acquiring Institution Identification Code */
	GetIsoField (ISO_ACQR_ID, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_ACQR_ID, SmsInfo, tmpField, tmpFieldLen);

	/* Field 33: Forwarding Institution Id Code */
	GetIsoField (ISO_FORWD_ID, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_FORWD_ID, SmsInfo, tmpField, tmpFieldLen);

	/* Field 37: Retrieval Reference Number */
	GetIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, 12);

	/* Field 48: Additional Data Private */
	GetIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, 4);


	/* Field 49: Transaction Currency Code */
	GetIsoField (ISO_TRANS_CRNCY, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_CRNCY, SmsInfo, tmpField, 3);

	/* Field 63: SMS Private Use Field */
	InitPufInfo (&PufInfo);
	InitTlvPrivate (&TlvInfo);
	GetIsoField (ISO_NETWORK_DATA, IsoInfo, tmpField, &tmpFieldLen);
	AnalyseTlvPrivate (tmpField, tmpFieldLen, &TlvInfo);

	if (GetTlvPrivate (NET_NETWORK_ID_TAG, &TlvInfo, tmpField ,  &tmpFieldLen) == SUCCESS) 
		AddPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, tmpFieldLen);
	
	GetIsoField (ISO_MSG_REASON, IsoInfo, tmpField, &tmpFieldLen);
	AddPufSubField (PUF_MSG_REASON, &PufInfo, tmpField, 4);

	tmpFieldLen1 = PufBuildField (tmpField1, &PufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField1, tmpFieldLen1);

	/* Field 68: Receiving Institution Country Code */
	GetIsoField (ISO_RECEIVER_ID, IsoInfo, tmpField, &tmpFieldLen);
	if (!memcmp (tmpField, "59", 2))
		if (GetIsoField (ISO_RECEIVER_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddSmsField (SMS_RECEIVER_COUNTRY, SmsInfo, tmpField, 3);

	/* Field 100: Receiving Institution Id Code */
	GetIsoField (ISO_RECEIVER_ID, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_RECEIVER_ID, SmsInfo, tmpField, tmpFieldLen);

	trace_event ("End FeeAdvToSms()", PROCESSING);

	return(OK);

}
/*************************************************************************/
/*  FeesResponseFromSms()                                                */
/*                                                                       */
/*  Converts received SMS fees collection response to ISO-format message */
/*  Input: SmsInfo: SMS received message                                 */
/*         IsoInfo: ISO message to build                                 */
/*************************************************************************/
int FeesResponseFromSms(int         nIndexCtx,
	TSSmsInfo   *SmsInfo,
	TSIsoInfo   *IsoInfo)
{
	char    yymmdd     [8];
	char    tmpField   [BUF_LG_MAX];
	char    sRespCode  [3+1]; 
	int     tmpFieldLen;
	int     tmpFieldLen1;

	trace_event("Start FeesResponseFromSms()", PROCESSING);

	memset(yymmdd,        0,  sizeof(yymmdd));
	memset(tmpField,      0,  sizeof(tmpField));
	memset(sRespCode,     0,  sizeof(sRespCode));

	IsoInfo->nMsgType = 1230;

	/* Field 5: Settlement Amount */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_STLMNT_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 9: Settlement Conversion Rate */
	if (GetSmsField (SMS_CNVRSN_RATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		InsertIsoField (ISO_STLMNT_CNVRSN_RATE, IsoInfo, tmpField, tmpFieldLen);

	/* Field 15: Settlement Date */
	if (GetSmsField (SMS_STLMNT_DATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		current_date_yymmdd(yymmdd);
		memcpy (tmpField, yymmdd, 2);
		GetSmsField (SMS_STLMNT_DATE, SmsInfo, tmpField + 2, &tmpFieldLen1);
		AddIsoField (ISO_STLMNT_DATE, IsoInfo, tmpField, 6);
	}

	/* Field 16: Conversion Date */
	if (GetSmsField (SMS_CNVRSN_DATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_CNVRSN_DATE, IsoInfo, tmpField, tmpFieldLen);

	/* Field 39: Response Code */
	if (GetSmsField (SMS_RESPONSE_CODE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		TranslateRespCode (nIndexCtx, ResourceId, tmpField, "RP", sRespCode );
		InsertIsoField (ISO_RESPONSE_CODE, IsoInfo, sRespCode, 3);
	}

	trace_event("End FeesResponseFromSms(OK)", PROCESSING);

	return(OK);
}











