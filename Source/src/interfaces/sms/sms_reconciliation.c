#include <define.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <resources.h>
#include <sms_var.h>
#include <sms_puf.h>
#include <sms_param.h>

#include <iso_hps.h>
#include <iso_sms.h>
#include <iso_cps.h>
#include <sms_fields.h>
#include <cps_fields.h>
#include <iso_fields.h>

#include <tlv_private.h>
#include <tlv_autho.h>

#include <list_thr.h>
#include <queue.h>
#include <security_data.h>
#include <hsm_inc.h>

/*************************************************************************************************/
/*********************                   FCT_REC_ADV_FROM_NW ()                         **********/
/*******  Processes a Reconciliation Advice Message (0500) or (0520) received from SMS  **********/
/*************************************************************************************************/
int FCT_REC_ADV_FROM_NW (int            P_nIndexCtx,
	TSSmsInfo      *P_psSmsInfo)
{
	TSIsoInfo               sIsoInfo;
	TSTlvAutho              sInternalTlvInfo;
	char                    sInternalTlvBuff[ LG_MAX ];
	char                    tmpField        [ BUF_LG_MAX ];
	char                    sLine           [ MAX_LINE_TRC ];
	int                     tmpFieldLen;
	int                     nRetCode;

	trace_event ("Start FCT_REC_ADV_FROM_NW()", PROCESSING);

	/***** Initialisation des structures et variables de travail *******/
	memset(tmpField,                0, sizeof (tmpField ));
	memset(sInternalTlvBuff,        0, sizeof (sInternalTlvBuff ));

	InitIsoInfo ( &sIsoInfo);
	InitTlvAutho( &sInternalTlvInfo);

	/****** Conversion du message SMS en format ISO PowerCARD   *******/
	ReconciliationAdvToIso(P_nIndexCtx, P_psSmsInfo, &sIsoInfo);

	dump_p7_struct (&sIsoInfo);

	/* Conversion a partir du format IsoInfo au format TlvInfo */
	nRetCode = IsoToTlv( P_nIndexCtx, &sIsoInfo, &sInternalTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert the ISO message %04d into the Tlv format", sIsoInfo.nMsgType);
		trace_event (sLine, PROCESSING);
		return (NOK);
	}

	PutTlvAutho  ( AUTO_CAPTURE_CODE, &sInternalTlvInfo, ResourceId, 6);
	PutTlvAutho  ( AUTO_ACQUIRER_BANK, &sInternalTlvInfo, AcquirerBank, 6);
	PutTlvAutho  ( AUTO_RESPONSE_CODE, &sInternalTlvInfo, "500", 3);

	AuthoBuildTlv ( sInternalTlvBuff, &sInternalTlvInfo);

	/*** Insertion dans la table ***/
	nRetCode = InsertSettlement( P_nIndexCtx, sInternalTlvBuff, "ACQ");
	if ( nRetCode != OK )
	{
		trace_event  ( "ERROR IN ONLINE_NETWORK_SETTLEMENT INSERT !!!", PROCESSING );
		return (NOK);
	}

	ReconcResponseToNw ("9", P_psSmsInfo);

	trace_event ("End FCT_REC_ADV_FROM_NW()", PROCESSING);
	return (OK);
}

/***********************************************************************/
/**********          ReconciliationAdvToIso ()             ************/
/****** Constructs an Iso message from the Sms received message *******/
/**********************************************************************/
int ReconciliationAdvToIso (int         nIndexCtx,
	TSSmsInfo   *SmsInfo,
	TSIsoInfo   *IsoInfo)
{
	TSTlvPrivate   TlvInfo;
	TSSmsPufInfo   SmspufInfo;
	TSCpsInfo      CpsInfo;
	TSPufInfo      PufInfo;

	char           tmpField     [BUF_LG_MAX];
	char           tmpField1    [256];
	char           sPufData     [BUF_LG_MAX];
	char           sPid         [ 8 + 1];
	char           sLine        [MAX_LINE_TRC];
	char           yymmdd [8];
	char           hhmiss [8];
	char           sIntMonth    [3];
	char           sExtMonth    [3];
	char           sIntYear     [3];
	int            nYear;
	int            nSettlYear;
	int            FieldLen;
	int            tmpFieldLen;
	int            tmpFieldLen1;
	int            nLength;

	trace_event ("Start  ReconciliationAdvToIso()", PROCESSING);

	/***** Initialisation des variables de travail *****/
	memset (tmpField,        0, sizeof (tmpField));
	memset (tmpField1,       0, sizeof (tmpField1));
	memset (yymmdd,          0, sizeof (yymmdd));
	memset (hhmiss,          0, sizeof (hhmiss));
	memset (sIntYear,        0, sizeof (sIntYear));

	IsoInfo->nMsgType = SmsInfo->nMsgType + 1000;

	/* Field 7: Transmission Date & Time */
	GetSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, &FieldLen);
	AddIsoField  (ISO_XMIT_TIME, IsoInfo, tmpField, 10);

	/* Field 11: System Trace Audit Number */
	GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &FieldLen);
	AddIsoField  (ISO_AUDIT_NBR, IsoInfo, tmpField, 6);

	/* Fields 12: Transaction Local Date and Time */
	current_date_yymmdd (yymmdd);
	memcpy (sIntYear, yymmdd, 2);
	memcpy (tmpField1, yymmdd, 6);
	current_hour (hhmiss);
	memcpy (tmpField1 + 6, hhmiss, 6);
	AddIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField1, 12);

	/* Field 15: Settlement Date */
	memset (tmpField,     0, sizeof (tmpField));
	memset (sIntMonth,    0, sizeof (sIntMonth));
	memset (sExtMonth,    0, sizeof (sExtMonth));

	GetSmsField (SMS_STLMNT_DATE, SmsInfo, tmpField, &tmpFieldLen);
	memcpy (sIntMonth,  yymmdd+2, 2);
	memcpy (sExtMonth,  tmpField, 2);
	if ( atoi(sExtMonth) > atoi(sIntMonth) )
		nSettlYear = (atoi (sIntYear) - 1) %100;
	else
		nSettlYear = atoi (sIntYear);

	memset (tmpField1,       0, sizeof (tmpField1));
	sprintf(tmpField1, "%02d", nSettlYear);


	memcpy (tmpField1+2,  tmpField, 4);
	AddIsoField (ISO_STLMNT_DATE, IsoInfo, tmpField1, 6);


	/* Field 50: Settlement Currency Code */
	if (GetSmsField (SMS_STLMNT_CRNCY, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_STLMNT_CRNCY, IsoInfo, tmpField, tmpFieldLen);
	else
		AddIsoField (ISO_STLMNT_CRNCY, IsoInfo, "840", 3);

	/* Field 53: Security Data */
	memset ( tmpField1 , 0 , sizeof ( tmpField1 ));
	if (GetSmsField (SMS_PIN_DATA, SmsInfo,tmpField, &tmpFieldLen) == SUCCESS)
	{
		PutSecurityData(SECURITY_FORMAT  , tmpField1 , "02" , 2);
		PutSecurityData(PIN_BLOCK_FORMAT , tmpField1 , "01" , 2);
	}
	else
	{
		PutSecurityData(SECURITY_FORMAT  , tmpField1 , "00" , 2);
		PutSecurityData(PIN_BLOCK_FORMAT , tmpField1 , "99" , 2);
	}

	PutSecurityData(PIN_ENCRYPTION_KEY_INDEX , tmpField1 , "000" , 3);
	PutSecurityData(MAC_KEY_INDEX , tmpField1 , "000" , 3);

	memset( sPid , 0, sizeof ( sPid ));
	sprintf (sPid , "%08d", getpid());
	PutSecurityData(SOURCE_PID , tmpField1 , sPid  , 8);
	PutSecurityData(SOURCE_RESOURCE , tmpField1 , ResourceId  , 6);
	PutSecurityData(DESTINATION_PID , tmpField1 , "00000000"  , 8);
	PutSecurityData(DESTINATION_RESOURCE , tmpField1 , "000000"  , 6);
	PutSecurityData(USER , tmpField1 , "SMS            "  , 15);
	PutSecurityData(PURGE_TIME , tmpField1 , "000000000"  , 9);

	AddIsoField (ISO_SECURITY_DATA, IsoInfo, tmpField1, strlen(tmpField1));

	/* Field 63: Private Use Fields */
	GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &tmpFieldLen); 
	InitPufInfo (&PufInfo);
	AnalysePuf (tmpField, &PufInfo);
	GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, &tmpFieldLen);
	InitTlvPrivate(&TlvInfo);
	PutTlvPrivate(NET_NETWORK_ID_TAG, &TlvInfo, tmpField, 4);
	memset (tmpField, 0, sizeof(tmpField));
	FieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
	AddIsoField (ISO_NETWORK_DATA, IsoInfo, tmpField, FieldLen);

	memset (tmpField, 0, sizeof (tmpField));
	if (GetPufSubField (PUF_DEC_POS_INDICATOR, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_RECORD_DATA, IsoInfo, tmpField, tmpFieldLen);

	/* Field 69: Settlement Inst Country Code */
	if (GetSmsField (SMS_STLMNT_INST_COUNTRY, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_STLMNT_INST_COUNTRY, IsoInfo, tmpField, tmpFieldLen);

	/* Field 70: Network Management Info Code */
	GetSmsField (SMS_NW_MNGMT_INFO, SmsInfo, tmpField, &tmpFieldLen); 
	AddIsoField (ISO_NW_MNGMT_INFO, IsoInfo, tmpField, tmpFieldLen);

	/* Field 74: Credits Nbr */
	GetSmsField (SMS_CR_NBR, SmsInfo, tmpField, &tmpFieldLen); 
	AddIsoField (ISO_CR_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 75: Credits Reversal Nbr */
	GetSmsField (SMS_CR_REV_NBR, SmsInfo, tmpField, &tmpFieldLen); 
	AddIsoField (ISO_CR_REV_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 76: Debits Nbr */
	GetSmsField (SMS_DB_NBR, SmsInfo, tmpField, &tmpFieldLen); 
	AddIsoField (ISO_DB_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 77: Debits Reversal Nbr */
	GetSmsField (SMS_DB_REV_NBR, SmsInfo, tmpField, &tmpFieldLen); 
	AddIsoField (ISO_DB_REV_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 86: Credits Amount */
	GetSmsField (SMS_CR_AMOUNT, SmsInfo, tmpField, &tmpFieldLen); 
	AddIsoField (ISO_CR_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 87: Credits Reversal Amount */
	GetSmsField (SMS_CR_REV_AMOUNT, SmsInfo, tmpField, &tmpFieldLen); 
	AddIsoField (ISO_CR_REV_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 88: Debits Amount */
	GetSmsField (SMS_DB_AMOUNT, SmsInfo, tmpField, &tmpFieldLen); 
	AddIsoField (ISO_DB_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 89: Debits Reversal Amount */
	GetSmsField (SMS_DB_REV_AMOUNT, SmsInfo, tmpField, &tmpFieldLen); 
	AddIsoField (ISO_DB_REV_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 97: Amount Net Settlement */
	if(GetSmsField (SMS_NET_AMOUNT, SmsInfo, tmpField, &tmpFieldLen)==SUCCESS) 
		AddIsoField (ISO_NET_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 99: Settlement Inst ID Code */
	if(GetSmsField (SMS_STLMNT_INSTITN, SmsInfo, tmpField, &tmpFieldLen)==SUCCESS) 
		AddIsoField (ISO_STLMNT_INSTITN, IsoInfo, tmpField, tmpFieldLen);

	trace_event("End   ReconciliationAdvToIso()", PROCESSING);
	return(OK);
}

/****************************************************************************/
/*  ReconcResponseToNw()                                                    */
/*                                                                          */
/*  Builds a Reconciliation response SMS message to be sent for Acknowledge */
/****************************************************************************/
int ReconcResponseToNw (char        *szSettlementCode,
	TSSmsInfo   *SmsInfo)
{
	TSSmsInfo    NewSmsInfo;
	TSPufInfo    PufInfo;
	TSPufInfo    NewPufInfo;
	char         tmpField   [BUF_LG_MAX];
	char         tmpField1  [BUF_LG_MAX];
	char         sSmsSndBuf [LG_MAX];
	int          FieldLen;
	int          FieldLen1;
	int          nMsgLen;
	int          nRetCode;
	int          i;

	trace_event("Start ReconcResponseToNw ()", PROCESSING);

	InitSmsInfo (&NewSmsInfo);

	memset (tmpField,    0, sizeof (tmpField));
	memset (tmpField1,   0, sizeof (tmpField1));
	memset (sSmsSndBuf,  0, sizeof (sSmsSndBuf));

	if ( SmsInfo->nMsgType == 500 || SmsInfo->nMsgType == 501)
		NewSmsInfo.nMsgType = 510;
	else
		NewSmsInfo.nMsgType = 530;

	for (i = 0; i < SMS_RESPONSE_CODE; i++)
	{
		if (GetSmsField (i, SmsInfo, tmpField, &FieldLen) == SUCCESS)
			AddSmsField (i, &NewSmsInfo,  tmpField, FieldLen);
	}
	for (i = SMS_TERMINAL_NBR; i < SMS_PRIV_USE_FIELDS; i++)
	{
		if (GetSmsField (i, SmsInfo, tmpField, &FieldLen) == SUCCESS)
			AddSmsField (i, &NewSmsInfo,  tmpField, FieldLen);
	}

	/* Field 63: Private Use Fields */
	InitPufInfo (&PufInfo);
	InitPufInfo (&NewPufInfo);
	GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &FieldLen);
	AnalysePuf (tmpField, &PufInfo);
	GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField1, &FieldLen1);

	AddPufSubField (PUF_NETWORK_ID, &NewPufInfo, tmpField1, FieldLen1);
	FieldLen1 = PufBuildField (tmpField1, &NewPufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, &NewSmsInfo, tmpField1, FieldLen1);

	AddSmsField (SMS_STLMNT_CODE, &NewSmsInfo, szSettlementCode, 1);

	/* Field 69: Settlement Inst Country Code */
	if (GetSmsField (SMS_STLMNT_INST_COUNTRY, SmsInfo, tmpField, &FieldLen) == SUCCESS)
		AddSmsField (SMS_STLMNT_INST_COUNTRY, &NewSmsInfo, tmpField, FieldLen);

	/* Field 70: Network Management Info Code */
	GetSmsField (SMS_NW_MNGMT_INFO, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_NW_MNGMT_INFO, &NewSmsInfo, tmpField, FieldLen);

	if (!memcmp (szSettlementCode, "2", 1)) /* The station does not agree on the totals */
	{
		for (i = SMS_CR_NBR; i < SMS_STLMNT_INSTITN; i++)
		{
			if (GetSmsField (i, SmsInfo, tmpField, &FieldLen) == SUCCESS)
				AddSmsField (i, &NewSmsInfo,  tmpField, FieldLen);
		}
	}

	/* Field 99: Settlement Inst ID Code */
	GetSmsField (SMS_STLMNT_INSTITN, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_STLMNT_INSTITN, &NewSmsInfo, tmpField, FieldLen);

	/************** Send the request to Network  ************************/
	nMsgLen=SmsBuildMsg(sSmsSndBuf + SMS_HDR_LEN, &NewSmsInfo);
	SmsBuildHeader(RESPONSE, &(NewSmsInfo.sHeader), &(SmsInfo->sHeader), nMsgLen, StationId );

	memcpy(sSmsSndBuf, (char *) &(NewSmsInfo.sHeader), SMS_HDR_LEN);
	nMsgLen += SMS_HDR_LEN;

	DumpSms (sSmsSndBuf);

	if (WriteLineMsg(sSmsSndBuf, nMsgLen) != OK)
	{
		trace_event ("Function ReconcResponseToNw (): WriteLineMsg() failed", PROCESSING);
		return (NOK);
	}

	trace_event ("End   ReconcResponseToNw ()", PROCESSING);

	return (OK);
}

