#include <stdio.h>
#include <sys/time.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <iso_ictag.h>
#include <iso_cps.h>
#include <iso_puf.h>
#include <tlv_private.h>
#include <sms_param.h>
#include <sms_fields.h>
#include <sms_var.h>
#include <define.h>
#include <list_thr.h>
#include <queue.h>
#include <struct_line.h>
#include <hsm_inc.h>
#include <security_data.h>
#include <p7_define.h>
/**********************************************************************/
/*  FCT_ADJ_ADV_TO_NW()                                               */
/*                                                                    */
/*  Send a Financial Transaction Advice message (220) to SMS.         */
/**********************************************************************/
int  FCT_ADJ_ADV_TO_NW (int nIndexCtx, TSIsoInfo *IsoInfo)
{
	TSSmsInfo    SmsInfo;
	TSNodeData   Node;
	char         sLine [MAX_LINE_TRC];
	char         FieldData[LG_MAX];
	char         sPurgeTime[10];
	char         sBuffer[LG_MAX];
	int          Length;
	int          FieldLen;

	trace_event ("Start FCT_ADJ_ADV_TO_NW()", PROCESSING);

	memset (sLine,       0, sizeof(sLine));
	memset (sBuffer,     0, sizeof(sBuffer));
	memset (FieldData,   0, sizeof(FieldData));

	InitNodeData (&Node);
	GetIsoField (ISO_SECURITY_DATA, IsoInfo, FieldData, &FieldLen);
	GetSecurityData(PURGE_TIME,FieldData,sPurgeTime, &FieldLen);
	/*memcpy(sPurgeTime, FieldData + 49, 9);
	sPurgeTime[9] = '\0';*/

	Node.tPurgeTime= atol(sPurgeTime);

	GetIsoField (ISO_AUDIT_NBR, IsoInfo, FieldData, &FieldLen);
	memcpy (Node.szFstKey, "1220", 4);
	memcpy (Node.szFstKey + 4,  FieldData, 6);

	sprintf  ( sLine , "Cle de Chainage   = %s ", Node.szFstKey );
	trace_event ( sLine , PROCESSING );

	Length = IsoBuildMsg (Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	if (InsertRequest('A', &Node) != OK)
	{
		trace_event ("FCT_ADJ_ADV_TO_NW(NOK): InsertRequest() failed.", ERROR);
		return(NOK);
	}

	BuildAdjustToNw (nIndexCtx, IsoInfo, &SmsInfo);

	Length = SmsBuildMsg(sBuffer+SMS_HDR_LEN, &SmsInfo);
	SmsBuildHeader(REQUEST , &(SmsInfo.sHeader), NULL, Length, "123456");
	memcpy(sBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
	Length += SMS_HDR_LEN;

	if (WriteLineMsg(sBuffer, Length) != OK)
	{
		trace_event ("FCT_ADJ_ADV_TO_NW(NOK): WriteLineMsg() failed.", ERROR);
		return (NOK);
	}

	trace_event ("End FCT_ADJ_ADV_TO_NW (OK)", PROCESSING);
	return (OK);
}

/**********************************************************************/
/*  FCT_ADJ_REP_FROM_NW()                                             */
/*                                                                    */
/*  Receives a Financial Transaction Advice message (230) from SMS.   */
/**********************************************************************/
int  FCT_ADJ_REP_FROM_NW (int nIndexCtx, TSSmsInfo *SmsInfo)
{
	TSNodeData      Node;
	TSIsoInfo       IsoInfo;
	TSBalExchange   BalMessage;
	char            FieldData[LG_MAX];
	int             FieldLen;
	char            SourceId[8 + 1];
	int             Length;
	char            sSecurityData [ 128 + 1 ];
	char            sLine [ MAX_LINE_TRC ];
	char			SourceNodeId [NODE_ID_LEN + 1];
	int				nBal;

	trace_event ("Start  FCT_ADJ_REP_FROM_NW()", PROCESSING);

	memset (sLine,      0,  sizeof(sLine));
	memset (FieldData,  0,  sizeof(FieldData));
	memset (SourceNodeId,  0,  sizeof(SourceNodeId));

	/* Get audit number to search for original transaction */
	GetSmsField (SMS_AUDIT_NBR, SmsInfo, FieldData, &FieldLen);


	InitNodeData (&Node);
	memcpy (Node.szFstKey, "1220", 4);
	memcpy (Node.szFstKey + 4,  FieldData, 6);

	sprintf  ( sLine , "Cle de DeChainage = %s ", Node.szFstKey );
	trace_event ( sLine , PROCESSING );

	switch (FetchRequest ('A', &Node))
	{
	case ENOEXIST:
		trace_event ("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event ("End FCT_ADJ_REP_FROM_NW (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;
	}

	trace_event ("FetchRequest Succeeded ", PROCESSING );

	/* reconstruct original ISO message */
	InitIsoInfo (&IsoInfo);
	AnalyseIso (Node.sFstBuf, &IsoInfo);

	/* add response elements to ISO message */
	BuildAdjRepFromNw ( nIndexCtx , SmsInfo, &IsoInfo);

	/* forward it to requester */

	memset (sSecurityData, 0, sizeof(sSecurityData));
	memset (SourceId,      0, sizeof(SourceId));
	GetIsoField (ISO_SECURITY_DATA, &IsoInfo, sSecurityData , &FieldLen);
	GetSecurityData(SOURCE_PID,sSecurityData, SourceId,&Length );
	GetSecurityData(SOURCE_NODE_ID,sSecurityData, SourceNodeId,&Length );


	dump_p7_struct ( &IsoInfo );

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
	BalMessage.sHeader.nSourceId  = getpid();
	BalMessage.sHeader.nCommand   = IsoInfo.nMsgType;
	Length = IsoBuildMsg (BalMessage.sBody, &IsoInfo);
	BalMessage.sHeader.nLen = Length;

	if (WriteBalMsg (nBal, &BalMessage) != OK)
	{
		trace_event ("End FCT_ADJ_REP_FROM_NW(): WriteBalMsg() failed.", ERROR);
		return (NOK);
	}

	trace_event ("End  FCT_ADJ_REP_FROM_NW(OK)", PROCESSING);

	return (OK);
}

/**********************************************************************/
/*      BuildAdjustToNw()                                             */
/*                                                                    */
/*     Prepares a 220 message to be sent to SMS                       */
/*                                                                    */
/*     Input: IsoInfo: ISO-format message to be converted             */
/*            SmsInfo: structure to contain SMS message               */
/*            nIndexCtx: pointer to Oracle context                    */
/*                                                                    */
/**********************************************************************/
int BuildAdjustToNw ( int        nIndexCtx,
	TSIsoInfo  *IsoInfo,
	TSSmsInfo  *SmsInfo)
{
	char    tmpField  [BUF_LG_MAX];
	char    tmpField1 [BUF_LG_MAX];
	char    sLine [MAX_LINE_TRC];
	int     tmpFieldLen;                 /* length of field read */
	int     nRetCode;
	char    sCardAccNameAddress [40];
	char    sOrigReference      [12 + 1];
	char    src;
	TSTlvPrivate TlvInfo;
	TSPufInfo   PufInfo;
	char		  szAcquirerBank[6 + 1];
	char			szAcquirerId[12 + 1];

	trace_event("start BuildAdjustToNw()", PROCESSING);

	/****** Initialisation des variables et des structures ******/
	memset (sLine,           0, sizeof(sLine));
	memset (tmpField,        0, sizeof(tmpField));
	memset (tmpField1,       0, sizeof(tmpField1));
	memset (sOrigReference,  0, sizeof(sOrigReference));
	memset (szAcquirerBank,    0, sizeof(szAcquirerBank));
	memset (szAcquirerId,    0, sizeof(szAcquirerId));


	InitSmsInfo (SmsInfo);

	InitTlvPrivate (&TlvInfo);

	/***** Message Type is 220 ******/
	SmsInfo->nMsgType = 220;

	/* Field 2: Primary Account Number */
	GetIsoField (ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_CARD_NBR, SmsInfo, tmpField, tmpFieldLen);

	if (debug_option == 1)
	{
		sprintf ( sLine , "Card Number : %s", tmpField);
		trace_event ( sLine , PROCESSING );
	}

	if (GetIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvPrivate (tmpField, tmpFieldLen, &TlvInfo);
	}

	/* Field 3: Processing Code */
	GetIsoField (ISO_PROC_CODE, IsoInfo, tmpField, &tmpFieldLen);
	if (!memcmp (tmpField + 2, "50", 2))    /* VIC does not accept "50" */
		memcpy (tmpField + 2, "00", 2);
	AddSmsField (SMS_PROC_CODE, SmsInfo, tmpField, tmpFieldLen);

	/* Field 4: Transaction Amount */
	GetIsoField (ISO_TRANS_AMOUNT, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_AMOUNT, SmsInfo, tmpField, tmpFieldLen);

	/* Field 5: Amount Settlment */
	if (GetIsoField (ISO_STLMNT_AMOUNT, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, tmpFieldLen);

	/* Field 7: Transmission Date & Time */
	GetIsoField (ISO_XMIT_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, tmpFieldLen);

	/* Field 11: Trace Audit Number */
	GetIsoField (ISO_AUDIT_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField + 4, 6);

	/* Field 12: Transaction Local Time */
	GetIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_TIME, SmsInfo, tmpField + 6, 6);

	/* Field 13: Transaction Local Date */
	GetIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_DATE, SmsInfo, tmpField + 2, 4);

	/* Field 18: Merchant Type */
	GetIsoField (ISO_MERCHANT_TYPE, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_MERCHANT_TYPE, SmsInfo, tmpField, tmpFieldLen);

	/* Field 19: Acquiring Institution Country Code */
	AddSmsField (SMS_ACQR_COUNTRY, SmsInfo, CountryCode, 3);

	/* Field 22: POS Entry Mode Code */
	/* 90: magnetic stripe read, 10: PIN accepted */
	AddSmsField (SMS_POS_ENTRY_MODE, SmsInfo, "9010", 4);

	/* Field 25: POS Condition Code */
	/* 00: normal transaction */
	AddSmsField (SMS_POS_CONDITION, SmsInfo, "00", 2);

	/* Field 32: Acquiring Institution Identification Code */
	/*
	AddSmsField (SMS_ACQR_ID, SmsInfo, AcquirerId, 11);
	*/

	if (GetTlvPrivate(ACQUIRER_BANK_TAG, &TlvInfo, szAcquirerBank , &tmpFieldLen) != SUCCESS)
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


	/* Field 33: Forwarding Institution Identification Code */
	if (GetIsoField (ISO_FORWD_ID, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_FORWD_ID, SmsInfo, tmpField, tmpFieldLen);


	/* Field 37: Reference Number */
	GetIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, 12);

	/*AddSmsField (SMS_REFERENCE_NBR, SmsInfo, sOrigReference, 12);*/

	/* Field 38: Authorization Identification Response */
	if (GetIsoField (ISO_AUTHOR_ID, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_AUTHOR_ID, SmsInfo, tmpField, tmpFieldLen);

	/* Field 41: Terminal Identification */
	GetIsoField (ISO_TERMINAL_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TERMINAL_NBR, SmsInfo, tmpField, tmpFieldLen);

	/* Field 42: Card Acceptor Identification Code */
	GetIsoField (ISO_OUTLET_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_OUTLET_NBR, SmsInfo, tmpField, 15);

	/* Field 43: Card Acceptor Location */
	GetIsoField (ISO_TERMINAL_ADR, IsoInfo, tmpField, &tmpFieldLen);
	if (tmpFieldLen > 26)
		memcpy (tmpField1, tmpField, 26);
	else
		memcpy (tmpField1, tmpField, tmpFieldLen);
	memcpy (tmpField1 + 25, BankLocation, 11);
	memcpy (tmpField1 + 38, AlphaCode2, 2);
	AddSmsField (SMS_TERMINAL_ADR, SmsInfo, tmpField1, 40);

	/* Field 48: Additional Private Data */
	if (GetIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, tmpFieldLen);

	/* Field 49: Transaction Currency Code */
	GetIsoField (ISO_TRANS_CRNCY, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_CRNCY, SmsInfo, tmpField, tmpFieldLen);

	/* Field 63: SMS Private Use Fields */
	InitPufInfo (&PufInfo);
	AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0004", 4);
	GetIsoField (ISO_MSG_REASON, IsoInfo, tmpField, &tmpFieldLen);
	IsoToSmsReasonCode (tmpField, tmpField1);
	AddPufSubField (PUF_MSG_REASON, &PufInfo, tmpField1, 4);
	tmpFieldLen = PufBuildField (tmpField, &PufInfo);                          
	AddSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, tmpFieldLen);
	/* End SMS Private Use Fields */

	trace_event("End BuildAdjustToNw()", PROCESSING);

	return(OK);
}

/****************************************************************************/
/*   BuildAdjRepFromNw()                                                    */
/*                                                                          */
/*   Processes au Authorization Response message (0230) received from SMS   */
/****************************************************************************/
int  BuildAdjRepFromNw ( int         nIndexCtx,
	TSSmsInfo   *SmsInfo,
	TSIsoInfo   *IsoInfo)
{
	TSTlvPrivate   TlvInfo;
	char           FieldData      [ BUF_LG_MAX ];
	char           tmpField       [ BUF_LG_MAX ]; 
	char           sRespCode      [ 3 + 1 ];
	char           sSmsRespCode   [ 2 + 1 ];
	int            FieldLen;
	int            offset;

	trace_event ("Start  BuildAdjRepFromNw()", PROCESSING);

	IsoInfo->nMsgType = 1230;

	memset (FieldData,    0, sizeof(FieldData));
	memset (tmpField,     0, sizeof(tmpField));
	memset (sRespCode,    0, sizeof(sRespCode));
	memset (sSmsRespCode, 0, sizeof(sSmsRespCode));

	/* Field 5: Settlement Amount */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &FieldLen) == SUCCESS)
		InsertIsoField (ISO_STLMNT_AMOUNT, IsoInfo, tmpField, FieldLen);

	/* Field 9: Settlement Conversion Rate */
	if (GetSmsField (SMS_CNVRSN_RATE, SmsInfo, tmpField, &FieldLen) == SUCCESS)
		InsertIsoField (ISO_STLMNT_CNVRSN_RATE, IsoInfo, tmpField, FieldLen);

	/* Field 15  Settlement Date */
	GetIsoField (ISO_TRANS_DATE_TIME, IsoInfo, FieldData, &FieldLen);
	GetSmsField (SMS_STLMNT_DATE, SmsInfo, FieldData + 2, &FieldLen);
	InsertIsoField (ISO_STLMNT_DATE, IsoInfo, FieldData, 6);

	/* Field 16: Conversion Date */
	if (GetSmsField (SMS_CNVRSN_DATE, SmsInfo, tmpField, &FieldLen) == SUCCESS)
		InsertIsoField (ISO_CNVRSN_DATE, IsoInfo, tmpField, FieldLen);

	/* Field 20 */
	if (GetSmsField (SMS_PAN_COUNTRY, SmsInfo, FieldData, &FieldLen) == SUCCES)
		InsertIsoField (ISO_PAN_COUNTRY, IsoInfo, FieldData, 3);

	/* Field 38 */
	if (GetSmsField (SMS_AUTHOR_ID, SmsInfo, FieldData, &FieldLen) == SUCCES)
		PutIsoField  (ISO_AUTHOR_ID, IsoInfo, FieldData, FieldLen);

	/* Field 39 */
	if (GetSmsField (SMS_RESPONSE_CODE, SmsInfo, sSmsRespCode, &FieldLen) == SUCCESS)
	{
		TranslateRespCode (nIndexCtx, ResourceId, sSmsRespCode, "RP", sRespCode );
		PutIsoField (ISO_RESPONSE_CODE, IsoInfo, sRespCode, 3);
	}

	/* Field 48: Additional Data Private */
	if (GetSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &FieldLen) == SUCCESS)
		AddIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, FieldLen);

	trace_event ("End  BuildAdjRepFromNw(OK)", PROCESSING);
	return (OK);
}



