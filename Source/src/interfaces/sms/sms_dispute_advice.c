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
#include <p7_sid.h>


/*************************************************************************************************/
/*********************                   FCT_DISPUTE_RESP_ADV_FROM_NW ()                       **********/
/****************  Processes a Dispute Response Financial Status Advice received from SMS          **************/
/*************************************************************************************************/
int FCT_DISPUTE_RESP_ADV_FROM_NW (int            P_nIndexCtx,
									TSSmsInfo      *P_psSmsInfo)
{
	TSIsoInfo               sIsoInfo;
	TSTlvAutho              sInternalTlvInfo;
	TSTlvAutho              sIncomingTlvInfo;
	TSTlvPrivate            TlvInfo;
	char                    tmpField        [ BUF_LG_MAX ];
	char                    sInternalTlvBuff[ BUF_LG_MAX ];
	char                    sIncomingTlvBuff[ BUF_LG_MAX ];
	char                    sLine           [ MAX_LINE_TRC ];
	int                     tmpFieldLen;
	int                     nRetCode;
	char 					sPrimResource[RESOURCE_ID_LEN + 1];
	char 					sSecResource[ RESOURCE_ID_LEN + 1 ];
    char                    sStan [6 + 1 ];
    char                    sPan [24 + 1 ];



	trace_event ("Start FCT_DISPUTE_RESP_ADV_FROM_NW()", PROCESSING);

	memset(tmpField,                0, sizeof (tmpField ));
	memset(sInternalTlvBuff,        0, sizeof (sInternalTlvBuff ));
	memset(sIncomingTlvBuff,        0, sizeof (sIncomingTlvBuff ));
	memset(sPrimResource, 			0, sizeof (sPrimResource));
	memset(sSecResource, 			0, sizeof (sSecResource));
    memset(sStan,        			0, sizeof (sStan ));
    memset(sPan,        			0, sizeof (sPan ));


	InitIsoInfo ( &sIsoInfo);
	InitTlvAutho( &sInternalTlvInfo);
	InitTlvAutho( &sIncomingTlvInfo);

	/***********************************************************************/
	/******** Traitement pour l'insertion dans la table SMS_INCOMING *******/
	/***********************************************************************/
	/** Conversion 2 a partir du format IsoInfo au format TlvInfo en conservant le contenu du champ 48 **/
	nRetCode = SmsToTlv( P_nIndexCtx, P_psSmsInfo, &sIncomingTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert ISO message %04d to Tlv format", sIsoInfo.nMsgType);
		trace_event (sLine, PROCESSING);
		DisputeRespAdviceResponseToNw ("96", P_psSmsInfo);
		return (NOK);
	}

	/******** Construction de la chaine TLV ***********/
	AuthoBuildTlv ( sIncomingTlvBuff, &sIncomingTlvInfo);

	/*** Insertion dans la table SMS Incoming ***/
	nRetCode = InsertionSmsIncoming( P_nIndexCtx, sIncomingTlvBuff);
	if ( nRetCode != OK )
	{
		trace_event  ( "SEVERE ERROR !!!!!", PROCESSING );
		trace_event  ( "Error On Insert Sms Incoming File ", PROCESSING );
		DisputeRespAdviceResponseToNw ("96", P_psSmsInfo);
		return (NOK);
	}

	/***********************************************************************/
	/******** Traitement pour l'insertion dans la table AUTHO_ACTIVITY *****/
	/***********************************************************************/

	/****** Conversion du message SMS 0422 en format ISO PowerCARD   *******/

	DisputeteAdvToIso(P_nIndexCtx, P_psSmsInfo, &sIsoInfo);

	dump_p7_struct (&sIsoInfo);

	/* Conversion a partir du format IsoInfo au format TlvInfo */
	nRetCode = IsoToTlv( P_nIndexCtx, &sIsoInfo, &sInternalTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert ISO message %04d to Tlv format", sIsoInfo.nMsgType);
		trace_event (sLine, PROCESSING);
		DisputeRespAdviceResponseToNw ("96", P_psSmsInfo);
		return (NOK);
	}

	/******** Construction de la chaine TLV ***********/
	PutTlvAutho  ( AUTO_CUR_TABLE_INDICATOR, &sInternalTlvInfo, "MSC", 3);
	PutTlvAutho  ( AUTO_CAPTURE_CODE, &sInternalTlvInfo, ResourceId, 6);
	PutTlvAutho  ( AUTO_ROUTING_CODE, &sInternalTlvInfo, "000000", 6);

	PutTlvAutho  ( AUTO_NETWORK_CODE, &sInternalTlvInfo, "01", 2);/*VISA*/
	PutTlvAutho ( AUTO_ISSUING_BANK , &sInternalTlvInfo, AcquirerBank , ACQUIRER_BANK_LEN);
	PutTlvAutho ( AUTO_ACQUIRER_BANK, &sInternalTlvInfo, AcquirerBank , ACQUIRER_BANK_LEN);

	AuthoBuildTlv ( sInternalTlvBuff, &sInternalTlvInfo);

	/********* Insertion dans Autho Activity **********/
	nRetCode = InsertionAuthoActivity( P_nIndexCtx, sInternalTlvBuff);
	if ( nRetCode != OK )
	{
		trace_event  ( "SEVERE ERROR !!!!!", PROCESSING );
		trace_event  ( "Error On Insert into Autho Activity Table !! ", PROCESSING );
		DisputeRespAdviceResponseToNw ("00", P_psSmsInfo);
		trace_event ("END FCT_DISPUTE_RESP_ADV_FROM_NW(NOK)", PROCESSING);
		return (NOK);
	}

	nRetCode = GetAcqNetSwitch(	P_nIndexCtx,
								sInternalTlvBuff,
								sPrimResource,
								sSecResource);
	if(nRetCode == OK)
	{
		PutTlvAutho  ( AUTO_ROUTING_CODE, &sInternalTlvInfo, sPrimResource, 6);
		GetIsoField (ISO_AUDIT_NBR, &sIsoInfo, sStan, &tmpFieldLen);

		if(GetIsoField (ISO_CARD_NBR, &sIsoInfo, sPan, &tmpFieldLen) != SUCCES)
			memcpy(sPan,"0000000000000000",16);

        nRetCode =  InsertSafData  ( P_nIndexCtx,
                                    "1282",
                                     'A',
                                     sStan,
                                     ResourceId,
                                     sPrimResource,
                                     0,
                                     "000",
                                     sInternalTlvBuff,
                                     sPan);
	}

	nRetCode = DisputeRespAdviceResponseToNw ("00", P_psSmsInfo);
	if ( nRetCode == OK )
	{
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sInternalTlvBuff , "000" , 3);
		UpdateAuthoActivity ( P_nIndexCtx, sInternalTlvBuff);
	}

	trace_event ("End FCT_DISPUTE_RESP_ADV_FROM_NW()", PROCESSING);
	return (OK);
}

/**********************************************************************/
/**********           DisputeteAdvToIso ()                ************/
/*** Constructs an Iso message (0282) from the Sms received message ***/
/**********************************************************************/
int DisputeteAdvToIso (int         nIndexCtx,
	TSSmsInfo   *SmsInfo,
	TSIsoInfo   *IsoInfo)
{
	TSTlvPrivate   TlvInfo;
	TSSmsPufInfo   SmspufInfo;
	TSCpsInfo      CpsInfo;
	TSPufInfo      PufInfo;

	char           sCpsTlvData  [256];
	char           tmpField     [BUF_LG_MAX];
	char           sPufData     [BUF_LG_MAX];
	char           sPid         [ 8 + 1];
	char           sLine        [MAX_LINE_TRC];
	char           yymmdd [8];
	char           hhmiss [8];
	char           szProcessingCode[3];
	char           ProcCode [7];
	char           MerchantType [5];
	int            tmpFieldLen;
	char           szGMTDate[12 + 1];
	char           szTrxDateTime[12 + 1];
	char           sStlmntDate[6 + 1];
	char           sSecurityData[256];

	trace_event ("Start  DisputeteAdvToIso()", PROCESSING);

	memset (sLine,           0, sizeof (sLine));
	memset (tmpField,        0, sizeof (tmpField));
	memset (yymmdd,          0, sizeof (yymmdd));
	memset (hhmiss,          0, sizeof (hhmiss));
	memset (MerchantType,    0, sizeof (MerchantType));
	memset (ProcCode,        0, sizeof (ProcCode));
	memset (szProcessingCode,0, sizeof (szProcessingCode));
	memset(szGMTDate,		 0,sizeof(szGMTDate));
	memset(szTrxDateTime,    0,sizeof(szTrxDateTime));
	memset(sStlmntDate,		 0,sizeof(sStlmntDate));
	memset(sSecurityData,	 0,sizeof(sSecurityData));
	memset (sCpsTlvData,    0, sizeof(sCpsTlvData));


	IsoInfo->nMsgType = 1282;

	/* Field 2 */
	GetSmsField (SMS_CARD_NBR, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_CARD_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 3 */
	GetSmsField (SMS_PROC_CODE, SmsInfo, ProcCode, &tmpFieldLen);
	GetSmsField (SMS_MERCHANT_TYPE, SmsInfo, MerchantType, &tmpFieldLen);
	SmsToIsoProcessingCode (ProcCode, MerchantType, tmpField);
	AddIsoField (ISO_PROC_CODE, IsoInfo, tmpField, 6);
	memcpy (szProcessingCode, ProcCode, 2);

	/* Field 4: Transaction Amount */
	GetSmsField (SMS_TRANS_AMOUNT, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField  (ISO_TRANS_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 7: Transmission Date & Time */
	GetSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField  (ISO_XMIT_TIME, IsoInfo, tmpField, 10);


	/* Field 11: System Trace Audit Number */
	GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField  (ISO_AUDIT_NBR, IsoInfo, tmpField, 6);

	/* Fields 13: Date, Local transaction*/
	current_gmt_date (szGMTDate);
	current_date_yymmdd (yymmdd);
	GetSmsField (SMS_TRANS_DATE, SmsInfo, szTrxDateTime + 2 /*tmpField*/, &tmpFieldLen);
	SetLocalYear(szTrxDateTime,szGMTDate);
	current_hour (hhmiss);
    memcpy (szTrxDateTime + 6, hhmiss, 6);
    AddIsoField (ISO_TRANS_DATE_TIME, IsoInfo, szTrxDateTime, 12);

	/* Field 15: Settlement Date */
	if(GetSmsField (SMS_STLMNT_DATE, SmsInfo, sStlmntDate + 2, &tmpFieldLen)== SUCCESS)
	{
		SetLocalYear(sStlmntDate,szGMTDate);
		AddIsoField (ISO_STLMNT_DATE, IsoInfo, sStlmntDate, 6);
	}

	/* Field 18: Merchant Type */
	GetSmsField (SMS_MERCHANT_TYPE, SmsInfo, tmpField, &tmpFieldLen);
	InsertIsoField  (ISO_MERCHANT_TYPE, IsoInfo, tmpField, 4);            

	/* Field 19: Acquirer Institution Country Code */
	GetSmsField (SMS_ACQR_COUNTRY, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField  (ISO_ACQR_COUNTRY, IsoInfo, tmpField, tmpFieldLen);


	/* Field 24: Function Code */
	switch (atoi(szProcessingCode))
	{
	case 1: /* Reprsentement */
		AddIsoField (ISO_FUNCTION_CODE, IsoInfo, "451", 3);
		break;
	case 2: /* Reprsentement reversal debit */
		AddIsoField (ISO_FUNCTION_CODE, IsoInfo, "452", 3);
		break;
	case 22: /* Reprsentement reversal credit */
		AddIsoField (ISO_FUNCTION_CODE, IsoInfo, "472", 3);
		break;
	}

	/* Field 32: Acquirer Institution Identification Code   */
	GetSmsField (SMS_ACQR_ID, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_ACQR_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 37: Retrieval Reference Number     */
	GetSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 38: Authorization Identification Response     */
	if( GetSmsField (SMS_AUTHOR_ID, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	AddIsoField (ISO_AUTHOR_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 39: RResponse Code     */
	if( GetSmsField (SMS_RESPONSE_CODE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	AddIsoField (ISO_RESPONSE_CODE, IsoInfo, tmpField, tmpFieldLen);

	/* Field 41: Card Acceeptor Terminal Identification  */
	if(GetSmsField (SMS_TERMINAL_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	AddIsoField (ISO_TERMINAL_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 42: Card Acceptor Identification Code  */
	GetSmsField (SMS_OUTLET_NBR, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_OUTLET_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 43: Card Acceptor Name/Location  */
	if (GetSmsField (SMS_TERMINAL_ADR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_TERMINAL_ADR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 48: Additinal Data Private */
	InitTlvPrivate (&TlvInfo);

	if (GetSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		if(tmpField[1]== P7_F048_VCR_USAGE)
			PutTlvPrivate(SMS_VCR_DATA ,&TlvInfo,tmpField + 3,tmpFieldLen - 3 );
		else
			PutTlvPrivate(SMS_CHG_DATA ,&TlvInfo,tmpField + 1,tmpFieldLen - 1);
	}

	PutTlvPrivate (HPS_ACQUIRER_BANK_TAG , &TlvInfo, AcquirerBank, 6);

	if (GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		PutTlvPrivate (EXTERNAL_STAN_TAG , &TlvInfo, tmpField, 6);

	tmpFieldLen = PrivateBuildTlv(tmpField, &TlvInfo);
	if(tmpFieldLen >0)
		AddIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, tmpFieldLen);

	/* Field 49: Transaction Currency Code  */
	GetSmsField (SMS_TRANS_CRNCY, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_TRANS_CRNCY, IsoInfo, tmpField, tmpFieldLen);


	/* Field 53: Security Data */
	if (GetSmsField (SMS_PIN_DATA, SmsInfo,tmpField, &tmpFieldLen) == SUCCESS)
	{
		PutSecurityData(SECURITY_FORMAT  , sSecurityData , "02" , 2);
		PutSecurityData(PIN_BLOCK_FORMAT , sSecurityData , "01" , 2);
	}
	else
	{
		PutSecurityData(SECURITY_FORMAT  , sSecurityData , "00" , 2);
		PutSecurityData(PIN_BLOCK_FORMAT , sSecurityData , "99" , 2);
	}

	PutSecurityData(PIN_ENCRYPTION_KEY_INDEX , sSecurityData , "000" , 3);
	PutSecurityData(MAC_KEY_INDEX , sSecurityData , "000" , 3);

	memset( sPid , 0, sizeof ( sPid ));
	sprintf (sPid , "%08d", getpid());
	PutSecurityData(SOURCE_PID , sSecurityData , sPid  , 8);
	PutSecurityData(SOURCE_RESOURCE , sSecurityData , ResourceId  , 6);
	PutSecurityData(DESTINATION_PID , sSecurityData , "00000000"  , 8);
	PutSecurityData(DESTINATION_RESOURCE , sSecurityData , "000000"  , 6);
	PutSecurityData(USER , sSecurityData , "SMS            "  , 15);
	PutSecurityData(PURGE_TIME , sSecurityData , "000000000"  , 9);

	AddIsoField (ISO_SECURITY_DATA, IsoInfo, sSecurityData, strlen(sSecurityData));


	/* Field: 62: CPS field */
	InitCpsInfo (&CpsInfo);

	if (GetSmsField (SMS_CPS_FIELDS , SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseCps(tmpField,&CpsInfo);
		InitTlvPrivate (&TlvInfo);

		if (GetCpsSubField (CPS_TRANS_ID, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			PutTlvPrivate(CPS_TRANS_ID_TAG ,&TlvInfo, tmpField, tmpFieldLen);
			tmpFieldLen = PrivateBuildTlv(sCpsTlvData, &TlvInfo);
			AddIsoField (ISO_CPS_FIELDS, IsoInfo, sCpsTlvData, tmpFieldLen);
		}

	}

	/* Field 63: Private Use Fields */
	if (GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitPufInfo (&PufInfo);
		AnalysePuf (tmpField, &PufInfo);
		InitTlvPrivate(&TlvInfo);

		if(GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
			PutTlvPrivate(NET_NETWORK_ID_TAG, &TlvInfo, tmpField, strlen(tmpField));

		if(GetPufSubField (PUF_MSG_REASON, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
			PutTlvPrivate(NET_MSG_REASON_CODE_TAG, &TlvInfo, tmpField, strlen(tmpField));

		if(GetPufSubField (PUF_STIP_REASON, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
			PutTlvPrivate(NET_STIP_REASON_CODE_TAG, &TlvInfo, tmpField, strlen(tmpField));

		/*Start ICH17012018*/
		if(GetPufSubField (PUF_CHARGEBACK_FLAGS, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
			PutTlvPrivate(NET_CHARGEBACK_FLAGS_TAG, &TlvInfo, tmpField, strlen(tmpField));
		/*End ICH17012018*/

		tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
		
		if (tmpFieldLen > 0)
			AddIsoField (ISO_NETWORK_DATA, IsoInfo, tmpField, tmpFieldLen);
	}

	trace_event("End DisputeteAdvToIso()", PROCESSING);
	return(OK);
}
/*************************************************************************/
/*  DisputeRespAdviceResponseToNw()                                                  */
/*                                                                       */
/*  Builds a ChargeBack reponse SMS message to be sent.                  */
/*************************************************************************/
int DisputeRespAdviceResponseToNw (char        *szResponseCode,
	TSSmsInfo   *SmsInfo)
{
	TSSmsInfo    NewSmsInfo;
	TSPufInfo    PufInfo;
	TSPufInfo    NewPufInfo;
	char         tmpField   [BUF_LG_MAX];
	char         sSmsSndBuf [LG_MAX];
	int          FieldLen;
	int          nMsgLen;
	int          nRetCode;

	trace_event("Start DisputeRespAdviceResponseToNw ()", PROCESSING);

	InitSmsInfo (&NewSmsInfo);

	memset (tmpField,    0, sizeof (tmpField));
	memset (sSmsSndBuf,  0, sizeof (sSmsSndBuf));

	NewSmsInfo.nMsgType = 292;


	/* Field 3 */
	GetSmsField (SMS_PROC_CODE, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_PROC_CODE, &NewSmsInfo, tmpField, FieldLen);

	/* Field 7: Transmission Date & Time */
	GetSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_XMIT_TIME, &NewSmsInfo, tmpField, FieldLen);

	/* Field 11: System Trace Audit Number */
	GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_AUDIT_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 15: Settlement Date */
	GetSmsField (SMS_STLMNT_DATE, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_STLMNT_DATE, &NewSmsInfo, tmpField, FieldLen);

	/* Field 19: Acquirer Institution Country Code */
	GetSmsField (SMS_ACQR_COUNTRY, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_ACQR_COUNTRY, &NewSmsInfo, tmpField, FieldLen);

	/* Field 25: Pos Condition Code */
	GetSmsField (SMS_POS_CONDITION, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_POS_CONDITION, &NewSmsInfo, tmpField, FieldLen);

	/* Field 32: Acquirer Institution Identification Code   */
	GetSmsField (SMS_ACQR_ID, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_ACQR_ID, &NewSmsInfo, tmpField, FieldLen);

	/* Field 37: Retrieval Reference Number     */
	GetSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_REFERENCE_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 39: Response Code   */
	AddSmsField (SMS_RESPONSE_CODE, &NewSmsInfo, szResponseCode, 2);

	/* Field 41: Card Acceeptor Terminal Identification  */
	if(GetSmsField (SMS_TERMINAL_NBR, SmsInfo, tmpField, &FieldLen) == SUCCESS)
	AddSmsField (SMS_TERMINAL_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 42: Card Acceptor Identification Code  */
	GetSmsField (SMS_OUTLET_NBR, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_OUTLET_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 60: Additional POS Information */
	GetSmsField (SMS_ADTNL_POS_INFO, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_ADTNL_POS_INFO, &NewSmsInfo, tmpField, FieldLen);

	/* Field 63: Private Use Fields */
	InitPufInfo (&PufInfo);
	InitPufInfo (&NewPufInfo);
	GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &FieldLen);
	AnalysePuf (tmpField, &PufInfo);
	GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, &FieldLen);

	AddPufSubField (PUF_NETWORK_ID, &NewPufInfo, tmpField, FieldLen);
	FieldLen = PufBuildField (tmpField, &NewPufInfo);
	if(FieldLen > 0)
	AddSmsField (SMS_PRIV_USE_FIELDS, &NewSmsInfo, tmpField, FieldLen);



	/************** Send the request to Network  ************************/
	nMsgLen=SmsBuildMsg(sSmsSndBuf + SMS_HDR_LEN, &NewSmsInfo);
	SmsBuildHeader(RESPONSE, &(NewSmsInfo.sHeader), &(SmsInfo->sHeader), nMsgLen, StationId );

	memcpy(sSmsSndBuf, (char *) &(NewSmsInfo.sHeader), SMS_HDR_LEN);
	nMsgLen += SMS_HDR_LEN;

	DumpSms (sSmsSndBuf);

	if (WriteLineMsg(sSmsSndBuf, nMsgLen) != OK)
	{
		trace_event ("Function DisputeRespAdviceResponseToNw (): WriteLineMsg() failed", PROCESSING);
		return (NOK);
	} 

	trace_event("End DisputeRespAdviceResponseToNw (OK)", PROCESSING);
	return(OK);
}

