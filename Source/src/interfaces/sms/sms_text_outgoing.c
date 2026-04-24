#include <define.h>
#include <stdio.h>
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
/***                   FCT_MSG_REQ_TO_NW ()                          ***/
/***    sends an Administrative free text message (0600) to SMS      ***/
/***********************************************************************/
int FCT_MSG_REQ_TO_NW (int nIndexCtx, TSIsoInfo    *IsoInfo)
{
	TSTlvAutho            sInternalTlvInfo;
	TSSmsInfo             SmsInfo;
	TSNodeData            Node;
	char                  FieldData[LG_MAX];
	char                  sBuffer[LG_MAX];
	char                  sLine [MAX_LINE_TRC];
	char                  sInternalTlvBuff     [ LG_MAX ];
	char                  sPurgeTime[10];
	int                   FieldLen;
	int                   Length;
	int                   nRetCode;

	trace_event ("Start FCT_MSG_REQ_TO_NW()", PROCESSING);

	memset (sBuffer,                0,    sizeof (sBuffer));
	memset (sInternalTlvBuff,       0,    sizeof (sInternalTlvBuff));
	memset (sLine,                  0,    sizeof (sLine));

	InitTlvAutho( &sInternalTlvInfo);

	InitNodeData (&Node);
	GetIsoField (ISO_SECURITY_DATA, IsoInfo, FieldData, &FieldLen);
	GetSecurityData(PURGE_TIME,FieldData,sPurgeTime, &FieldLen);


	Node.tPurgeTime= atol(sPurgeTime);

	GetIsoField (ISO_AUDIT_NBR, IsoInfo, FieldData, &FieldLen);
	memcpy (Node.szFstKey, "1600", 4);
	memcpy (Node.szFstKey + 4,  FieldData, 6);

	Length = IsoBuildMsg (Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	
	dump_p7_struct (IsoInfo);

	if (InsertRequest('B', &Node) != OK)
	{
		trace_event ("FCT_MSG_REQ_TO_NW(NOK): InsertRequest() failed.", ERROR);
		return(NOK);
	}

	TextReqToSms( nIndexCtx , IsoInfo, &SmsInfo);

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
		trace_event ("FCT_MSG_REQ_TO_NW(NOK): WriteLineMsg() failed.", ERROR);
		return (NOK);
	}

	trace_event ("End FCT_MSG_REQ_TO_NW()", PROCESSING);

	return (OK);
}
/****************************************************************************************/
/***                      FCT_MSG_RPLY_FROM_NW ()                                     ***/
/*** Receives & converts a free administrative text message response (0610) from SMS  ***/
/****************************************************************************************/
int FCT_MSG_RPLY_FROM_NW  (int         nIndiceCtx,
	TSSmsInfo   *smsInfo)
{
	TSTlvAutho          sInternalTlvInfo;
	TSBalExchange       BalMessage;
	TSNodeData          node;
	TSIsoInfo           isoInfo;
	int                 length;
	int                 nRetCode;
	char                sInternalTlvBuff     [LG_MAX ];
	char                data[BUF_LG_MAX];
	char                srcId[12];
	char                sLine                [MAX_LINE_TRC];

	trace_event ("Start FCT_MSG_RPLY_FROM_NW()", PROCESSING);

	memset (data,                   0,    sizeof (data));
	memset (sInternalTlvBuff,       0,    sizeof (sInternalTlvBuff));
	memset (sLine,                  0,    sizeof (sLine));

	InitTlvAutho( &sInternalTlvInfo);

	GetSmsField (SMS_AUDIT_NBR, smsInfo, data, &length);

	InitNodeData (&node);
	memcpy (node.szFstKey, "0600", 4);
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

	isoInfo.nMsgType = smsInfo->nMsgType + 1000;

	TextResponseFromSms (nIndiceCtx, smsInfo, &isoInfo);

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
	GetSecurityData(SOURCE_PID,data,srcId,&length);


	BalMessage.sHeader.nPartnerId = atoi(srcId);
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1610;
	length = IsoBuildMsg (BalMessage.sBody, &isoInfo);
	BalMessage.sHeader.nLen = length;

	if (WriteBalMsg (nBalMsg, &BalMessage) != OK)
	{
		trace_event ("FCT_MSG_RPLY_FROM_NW(NOK): WriteBalMsg() failed.", ERROR);
		return (NOK);
	}

	trace_event ("End FCT_MSG_RPLY_FROM_NW (OK)", PROCESSING);
	return (OK);
}

/*****************************************************************************************/
/******************            TextReqToSms ()                            ****************/
/***    Prepares SMS Administrative Text message 0600 to be sent to network           ***/
/***             Input: IsoInfo: structure containing ISO-format message              ***/
/***                    SmsInfo: structure to contain SMS message to be sent          ***/
/****************************************************************************************/
int TextReqToSms (int        nIndiceCtx,
	TSIsoInfo  *IsoInfo,
	TSSmsInfo  *SmsInfo)
{
	TSPufInfo  PufInfo;
	TSTlvPrivate   TlvInfo;
	char       tmpField   [LG_MAX];
	int        tmpFieldLen;
	int        tmpFieldLen1;

	trace_event("Start TextReqToSms()", PROCESSING);

	memset (tmpField,    0, sizeof(tmpField));
	

	InitSmsInfo (SmsInfo);

	SmsInfo->nMsgType = IsoInfo->nMsgType - 1000;


	/* Field 7: Transmission Date & Time */
	GetIsoField (ISO_XMIT_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, 10);

	/* Field 11: System Trace Audit Number */
	GetIsoField (ISO_AUDIT_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, 6);

	/* Field 21: Forwarding Institution Country Code */
	GetIsoField (ISO_FORWD_ID, IsoInfo, tmpField, &tmpFieldLen);
	if (!memcmp (tmpField, "59", 2))
	{
		if (GetIsoField (ISO_FORWD_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddSmsField (SMS_FORWD_COUNTRY, SmsInfo, tmpField, tmpFieldLen);
	}

	/* Field 33: Forwarding Institution Id Code  */
	GetIsoField (ISO_FORWD_ID, IsoInfo, tmpField, &tmpFieldLen);                                                                
	AddSmsField (SMS_FORWD_ID, SmsInfo, tmpField, tmpFieldLen);

	/* Field 37: Retrieval Reference Number  */
	GetIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen); 
	AddSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, tmpFieldLen);

	/* Field 48: Additinal Data Private */
	GetIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, tmpFieldLen);

	/* Field 63: SMS Private Use Field */
	InitPufInfo (&PufInfo);
	GetIsoField (ISO_NETWORK_DATA, IsoInfo, tmpField, &tmpFieldLen);
	InitTlvPrivate(&TlvInfo);
	AnalyseTlvPrivate (tmpField, tmpFieldLen, &TlvInfo);
	if (GetTlvPrivate (NET_NETWORK_ID_TAG, &TlvInfo, tmpField ,  &tmpFieldLen) == SUCCESS) 
		AddPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, 4);

	tmpFieldLen = PufBuildField (tmpField, &PufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, tmpFieldLen);

	/* Field 68: Receiving Institution Country Code */
	GetIsoField (ISO_RECEIVER_ID, IsoInfo, tmpField, &tmpFieldLen);
	if (!memcmp (tmpField, "59", 2))
	{
		if (GetIsoField (ISO_RECEIVER_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddSmsField (SMS_RECEIVER_COUNTRY, SmsInfo, tmpField, tmpFieldLen); 
	}

	/* Field 70: Network Management Identification Code */
	GetIsoField (ISO_NW_MNGMT_INFO, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_NW_MNGMT_INFO, SmsInfo, tmpField, tmpFieldLen);

	/* Field 100: Receiving Institution Identification Code */
	GetIsoField (ISO_RECEIVER_ID, IsoInfo, tmpField, &tmpFieldLen);                                                            
	AddSmsField (SMS_RECEIVER_ID, SmsInfo, tmpField, tmpFieldLen);      

	trace_event ("End TextReqToSms()", PROCESSING);

	return (OK);

}

/********************************************************************************/
/*              TextResponseFromSms()                                           */
/*                                                                              */
/*  Puts the SMS text response message in an ISO-format structure               */
/*  Input: SmsInfo: structure containing received SMS response                  */
/*         IsoInfo: will contain ISO-format response                            */
/********************************************************************************/
int TextResponseFromSms (int        nIndexCtx,
	TSSmsInfo  *SmsInfo,
	TSIsoInfo  *IsoInfo)
{
	char        yymmdd        [8];
	char        tmpField      [256];
	char        tmpField1     [256];
	char        sRespCode     [3+1];
	int         tmpFieldLen;


	trace_event("Start TextResponseFromSms()", PROCESSING);

	memset(yymmdd,     0, sizeof(yymmdd));
	memset(tmpField,   0, sizeof(tmpField));
	memset(sRespCode,  0, sizeof(sRespCode));

	current_date_yymmdd(yymmdd);

	/* Field 15: Settlement Date */
	GetSmsField (SMS_STLMNT_DATE, SmsInfo, tmpField, &tmpFieldLen);
	memcpy (tmpField1, yymmdd, 2);
	memcpy (tmpField1 + 2, tmpField, tmpFieldLen);
	InsertIsoField (ISO_STLMNT_DATE, IsoInfo, tmpField1, 6);

	/* Field 39: Response Code */
	if (GetSmsField (SMS_RESPONSE_CODE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		TranslateRespCode (nIndexCtx, ResourceId, tmpField, "RP", sRespCode );
		InsertIsoField (ISO_RESPONSE_CODE, IsoInfo, sRespCode, 3);
	}

	trace_event("End TextResponseFromSms (OK)", PROCESSING);

	return(OK);

}
















