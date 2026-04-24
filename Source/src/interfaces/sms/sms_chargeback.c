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
/*********************                   FCT_CHGBK_ADV_FROM_NW ()                       **********/    
/****************  Processes a chargeback message (0422) received from SMS          **************/
/*************************************************************************************************/
int FCT_CHGBK_ADV_FROM_NW (int            P_nIndexCtx, 
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



	trace_event ("Start FCT_CHGBK_ADV_FROM_NW()", PROCESSING);

	/***** Initialisation des structures et variables de travail *******/
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
		ChgBkResponseToNw ("96", P_psSmsInfo);
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
		ChgBkResponseToNw ("96", P_psSmsInfo);
		return (NOK);
	}

	/***********************************************************************/
	/******** Traitement pour l'insertion dans la table AUTHO_ACTIVITY *****/
	/***********************************************************************/

	/****** Conversion du message SMS 0422 en format ISO PowerCARD   *******/

	ChargebackAdvToIso(P_nIndexCtx, P_psSmsInfo, &sIsoInfo);

	dump_p7_struct (&sIsoInfo);

	/* Conversion a partir du format IsoInfo au format TlvInfo */
	nRetCode = IsoToTlv( P_nIndexCtx, &sIsoInfo, &sInternalTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert ISO message %04d to Tlv format", sIsoInfo.nMsgType);
		trace_event (sLine, PROCESSING);
		ChgBkResponseToNw ("96", P_psSmsInfo);
		return (NOK);
	}

	/******** Construction de la chaine TLV ***********/
	PutTlvAutho  ( AUTO_CUR_TABLE_INDICATOR, &sInternalTlvInfo, "MSC", 3);
	PutTlvAutho  ( AUTO_CAPTURE_CODE, &sInternalTlvInfo, ResourceId, 6);
	PutTlvAutho  ( AUTO_ROUTING_CODE, &sInternalTlvInfo, "000000", 6);

	PutTlvAutho  ( AUTO_NETWORK_CODE, &sInternalTlvInfo, "01", 2);/*VISA*/
	PutTlvAutho ( AUTO_ISSUING_BANK , &sInternalTlvInfo, AcquirerBank , ACQUIRER_BANK_LEN);
	PutTlvAutho ( AUTO_ACQUIRER_BANK, &sInternalTlvInfo, AcquirerBank , ACQUIRER_BANK_LEN);

	/* 48 in TLV format. */
/*	InitTlvPrivate (&TlvInfo);
	AddTlvPrivate (HPS_ACQUIRER_BANK_TAG , &TlvInfo, AcquirerBank, 6);
	if (GetSmsField (SMS_AUDIT_NBR, P_psSmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddTlvPrivate (EXTERNAL_STAN_TAG , &TlvInfo, tmpField, 6);
	if (GetSmsField (SMS_ADTNL_RESP_DATA, P_psSmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddTlvPrivate (CVV_RESULT_CODE_TAG, &TlvInfo, tmpField+4, 1);

	tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
	if (tmpFieldLen > 0)
		PutTlvAutho  ( AUTO_ADD_DATA_PRIVATE, &sInternalTlvInfo, tmpField, tmpFieldLen);
*/
	AuthoBuildTlv ( sInternalTlvBuff, &sInternalTlvInfo);

	/********* Insertion dans Autho Activity **********/
	nRetCode = InsertionAuthoActivity( P_nIndexCtx, sInternalTlvBuff);
	if ( nRetCode != OK )
	{
		trace_event  ( "SEVERE ERROR !!!!!", PROCESSING );
		trace_event  ( "Error On Insert into Autho Activity Table !! ", PROCESSING );
		ChgBkResponseToNw ("00", P_psSmsInfo);
		trace_event ("END FCT_CHGBK_ADV_FROM_NW(NOK)", PROCESSING);
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
                                    "1422",
                                     'A',
                                     sStan,
                                     ResourceId,
                                     sPrimResource,
                                     0,
                                     "000",
                                     sInternalTlvBuff,
                                     sPan);
	}

	nRetCode = ChgBkResponseToNw ("00", P_psSmsInfo);
	if ( nRetCode == OK )
	{
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sInternalTlvBuff , "000" , 3);
		UpdateAuthoActivity ( P_nIndexCtx, sInternalTlvBuff);
	}

	trace_event ("End FCT_CHGBK_ADV_FROM_NW()", PROCESSING);
	return (OK);
}
/************************************************************************************************/
/*****************                   FCT_CHGBK_REV_FROM_NW ()                      **************/
/**********  Processes a chargeback reversal message (0422) received from SMS        ************/
/************************************************************************************************/
int FCT_CHGBK_REV_FROM_NW (int          P_nIndexCtx, 
	TSSmsInfo   *P_psSmsInfo)
{
	TSIsoInfo               sIsoInfo;
	TSTlvAutho              sInternalTlvInfo;
	TSTlvAutho              sIncomingTlvInfo;
	TSTlvPrivate            TlvInfo;
	char                    tmpField        [ BUF_LG_MAX ];
	char                    sInternalTlvBuff[ BUF_LG_MAX ];
	char                    sIncomingTlvBuff[ BUF_LG_MAX ];
	char                    sLine		[ MAX_LINE_TRC ]; 
	int                     nRetCode;
	int                     tmpFieldLen;

	trace_event ("Start FCT_CHGBK_REV_FROM_NW()", PROCESSING);

	/***** Initialisation des structures et variables de travail *******/
	memset(tmpField,                0, sizeof (tmpField ));
	memset(sInternalTlvBuff,        0, sizeof (sInternalTlvBuff ));
	memset(sIncomingTlvBuff,        0, sizeof (sIncomingTlvBuff ));

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
		ChgBkResponseToNw ("96", P_psSmsInfo);
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
		ChgBkResponseToNw ("96", P_psSmsInfo);
		return (NOK);
	}

	/***********************************************************************/
	/******** Traitement pour l'insertion dans la table AUTHO_ACTIVITY *****/                                                  
	/***********************************************************************/

	/****** Conversion du message SMS 0422 en format ISO PowerCARD   *******/

	ChargebackAdvToIso(P_nIndexCtx, P_psSmsInfo, &sIsoInfo);

	dump_p7_struct (&sIsoInfo);

	/* Conversion a partir du format IsoInfo au format TlvInfo */
	nRetCode = IsoToTlv( P_nIndexCtx, &sIsoInfo, &sInternalTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert ISO message %04d to Tlv format", sIsoInfo.nMsgType);
		trace_event (sLine, PROCESSING);
		ChgBkResponseToNw ("96", P_psSmsInfo);
		return (NOK); 
	}

	/******** Construction de la chaine TLV ***********/
	PutTlvAutho  ( AUTO_CUR_TABLE_INDICATOR, &sInternalTlvInfo, "MSC", 3);
	PutTlvAutho  ( AUTO_ROUTING_CODE, &sInternalTlvInfo, "000000", 6);
	PutTlvAutho  ( AUTO_CAPTURE_CODE, &sInternalTlvInfo, ResourceId, 6);

	/* 48 in TLV format. */
	/*InitTlvPrivate (&TlvInfo);
	AddTlvPrivate (HPS_ACQUIRER_BANK_TAG , &TlvInfo, AcquirerBank, 6);
	if (GetSmsField (SMS_AUDIT_NBR, P_psSmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddTlvPrivate (EXTERNAL_STAN_TAG , &TlvInfo, tmpField, 6);
	if (GetSmsField (SMS_ADTNL_RESP_DATA, P_psSmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddTlvPrivate (CVV_RESULT_CODE_TAG, &TlvInfo, tmpField+4, 1);

	tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
	if (tmpFieldLen > 0)
		PutTlvAutho  ( AUTO_ADD_DATA_PRIVATE, &sInternalTlvInfo, tmpField, tmpFieldLen);
*/
	AuthoBuildTlv ( sInternalTlvBuff, &sInternalTlvInfo);

	/********* Insertion dans Autho Activity **********/
	/* Duplicate issue*/
    /*NAB20182004 PROD00055720 */
	nRetCode = InsertionAuthoActivity( P_nIndexCtx, sInternalTlvBuff);
	if ( nRetCode != OK )
	{
		trace_event  ( "SEVERE ERROR !!!!!", PROCESSING );
		trace_event  ( "Error On Insert Into Autho Activity Table !!! ", PROCESSING );
		ChgBkResponseToNw ("96", P_psSmsInfo);
		return (NOK);
	}
	

	nRetCode = ChgBkResponseToNw ("00", P_psSmsInfo);
	if ( nRetCode == OK )
	{
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sInternalTlvBuff , "000" , 3);
		UpdateAuthoActivity ( P_nIndexCtx, sInternalTlvBuff);
	}

	trace_event ("End FCT_CHGBK_REV_FROM_NW(OK)", PROCESSING);
	return (OK);
}

/**********************************************************************/
/**********           ChargebackAdvToIso ()                ************/
/*** Constructs an Iso message (1422) from the Sms received message ***/
/**********************************************************************/
int ChargebackAdvToIso (int         nIndexCtx,
	TSSmsInfo   *SmsInfo,
	TSIsoInfo   *IsoInfo)
{
	TSTlvPrivate   TlvInfo;
	TSSmsPufInfo   SmspufInfo;
	TSCpsInfo      CpsInfo;
	TSCpsInfo      NewCpsInfo;
	TSPufInfo      PufInfo;

	char           sCpsTlvData  [256];
	char           tmpField     [BUF_LG_MAX];
	char           tmpField1    [256];
	char           tmpField2    [12];
	char           sPufData     [BUF_LG_MAX];
	char           sPid         [ 8 + 1];
	char           sLine        [MAX_LINE_TRC];
	char           SmsOrigData  [ 42 + 1 ];
	char           yymmdd [8];
	char           hhmiss [8];
	char           szProcessingCode[3]; /* variable to save the processing code */
	char           ProcCode [7];
	char           MerchantType [5];
	char           sIntMonth    [3];
	char           sExtMonth    [3];
	char           sIntDate     [5];
	char           sIntYear     [3];
	int            nYear; 
	int            nOrigYear; 
	int            nSettlYear;
	int            FieldLen; 
	int            tmpFieldLen;
	int            tmpFieldLen1;
	int            tmpFieldLen2;
	int            nLength;
	char           szGMTDate[12 + 1];      /*AMER 20160719*/
	char           szTrxDateTime[12 + 1];  /*AMER 20160719*/
	char           sStlmntDate[6 + 1];

	trace_event ("Start  ChargebackAdvToIso()", PROCESSING);

	/***** Initialisation des variables de travail *****/
	memset (sLine,           0, sizeof (sLine));
	memset (tmpField,        0, sizeof (tmpField));
	memset (tmpField1,       0, sizeof (tmpField1));
	memset (tmpField2,       0, sizeof (tmpField2));
	memset (yymmdd,          0, sizeof (yymmdd));
	memset (hhmiss,          0, sizeof (hhmiss));
	memset (sIntYear,        0, sizeof (sIntYear));
	memset (sIntDate,        0, sizeof (sIntDate));
	memset (SmsOrigData,     0, sizeof (SmsOrigData));
	memset (MerchantType,    0, sizeof (MerchantType));
	memset (ProcCode,        0, sizeof (ProcCode));
	memset (szProcessingCode,0, sizeof (szProcessingCode));
	memset(szGMTDate,'\0',sizeof(szGMTDate));      		/*AMER 20160719*/
	memset(szTrxDateTime,'\0',sizeof(szTrxDateTime));       /*AMER 20160719*/
	memset(sStlmntDate,'\0',sizeof(sStlmntDate));

	current_gmt_date (szGMTDate);   /*AMER 20160719*/

	IsoInfo->nMsgType = 1422;

	/* Field 2 */
	GetSmsField (SMS_CARD_NBR, SmsInfo, tmpField, &FieldLen);
	InsertIsoField (ISO_CARD_NBR, IsoInfo, tmpField, FieldLen);

	/* Field 3 */
	GetSmsField (SMS_PROC_CODE, SmsInfo, ProcCode, &tmpFieldLen);
	GetSmsField (SMS_MERCHANT_TYPE, SmsInfo, MerchantType, &tmpFieldLen);
	SmsToIsoProcessingCode (ProcCode, MerchantType, tmpField);
	AddIsoField (ISO_PROC_CODE, IsoInfo, tmpField, 6);
	memcpy (szProcessingCode, ProcCode, 2);

	/* Field 4: Transaction Amount */
	GetSmsField (SMS_TRANS_AMOUNT, SmsInfo, tmpField, &FieldLen);
	InsertIsoField  (ISO_TRANS_AMOUNT, IsoInfo, tmpField, FieldLen);

	/* Field 5: Settlement Amount */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_STLMNT_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 7: Transmission Date & Time */
	GetSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, &FieldLen);
	InsertIsoField  (ISO_XMIT_TIME, IsoInfo, tmpField, 10);            

	/* Field 9: Settlement Conversion Rate */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		GetSmsField (SMS_CNVRSN_RATE, SmsInfo, tmpField, &tmpFieldLen);
		AddIsoField (ISO_STLMNT_CNVRSN_RATE, IsoInfo, tmpField, tmpFieldLen);
	}

	/* Field 11: System Trace Audit Number */
	/*GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &FieldLen);*//*NAB20182004 PROD00055720 */
	GetInternalStan ( nIndexCtx , tmpField );/*NAB20182004 PROD00055720 */
	InsertIsoField  (ISO_AUDIT_NBR, IsoInfo, tmpField, 6);            

	/* Fields 12: Transaction Local Date and Time */
	current_date_yymmdd (yymmdd);
	GetSmsField (SMS_TRANS_DATE, SmsInfo, szTrxDateTime + 2 /*tmpField*/, &FieldLen);	/*AMER 20160719*/
	/*memcpy (sIntYear, yymmdd, 2);
	memcpy (sIntDate, yymmdd+2, 4);*/


	/*AMER 20160719 Start: PROD00029786*/
	SetLocalYear(szTrxDateTime,szGMTDate);
	current_hour (hhmiss);
	memcpy (szTrxDateTime + 6, hhmiss, 6);
	InsertIsoField (ISO_TRANS_DATE_TIME, IsoInfo, szTrxDateTime, 12);
	/*AMER 20160719 End*/
/*Start ICH17012018*/
	/* Field 15: Settlement Date */
	/*memset (tmpField,     0, sizeof (tmpField));
	memset (sIntMonth,    0, sizeof (sIntMonth));
	memset (sExtMonth,    0, sizeof (sExtMonth));*/

	if(GetSmsField (SMS_STLMNT_DATE, SmsInfo, sStlmntDate + 2, &tmpFieldLen)== SUCCESS)
	{
		SetLocalYear(sStlmntDate,szGMTDate);
		InsertIsoField (ISO_STLMNT_DATE, IsoInfo, sStlmntDate, 6);
	}

	/*memcpy (sIntMonth,  yymmdd+2, 2);
	memcpy (sExtMonth,  tmpField, 2);
	if (atoi(sExtMonth) > atoi(sIntMonth))
		nSettlYear = (atoi (sIntYear) - 1) %100;
	else
		nSettlYear = atoi (sIntYear);

	memset (tmpField1,       0, sizeof (tmpField1));
	sprintf(tmpField1, "%02d", nSettlYear);

	memcpy (tmpField1+2,  tmpField, 4)

	InsertIsoField (ISO_STLMNT_DATE, IsoInfo, tmpField1, 6);*/
/*End ICH17012018*/

	/* Field 16: Conversion Date */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		GetSmsField (SMS_CNVRSN_DATE, SmsInfo, tmpField, &tmpFieldLen);
		AddIsoField (ISO_CNVRSN_DATE, IsoInfo, tmpField, tmpFieldLen);
	}

	/* Field 18: Merchant Type */
	GetSmsField (SMS_MERCHANT_TYPE, SmsInfo, tmpField, &FieldLen);
	InsertIsoField  (ISO_MERCHANT_TYPE, IsoInfo, tmpField, 4);            

	/* Field 19: Acquirer Institution Country Code */
	GetSmsField (SMS_ACQR_COUNTRY, SmsInfo, tmpField, &FieldLen);
	InsertIsoField  (ISO_ACQR_COUNTRY, IsoInfo, tmpField, FieldLen);    

	/* Field 20: PAN Country Code if applicable */
	GetSmsField (SMS_CARD_NBR, SmsInfo, tmpField, &tmpFieldLen);
	if (!memcmp (tmpField, "59", 2))
	{
		if (GetSmsField (SMS_PAN_COUNTRY, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddIsoField (ISO_PAN_COUNTRY, IsoInfo, tmpField, tmpFieldLen);
	}

	/* Field 21: Forwarding Institution Country Code */
	GetSmsField (SMS_FORWD_COUNTRY, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_FORWD_COUNTRY, IsoInfo, tmpField, 3);

	/* Field 24: Function Code */
	switch (atoi(szProcessingCode))
	{
	case 1: /* ChargeBack */
		AddIsoField (ISO_FUNCTION_CODE, IsoInfo, "451", 3);
		break;
	case 2: /* ChargeBack reversal debit */
		AddIsoField (ISO_FUNCTION_CODE, IsoInfo, "452", 3);
		break;
	case 22: /* ChargeBack reversal credit */
		AddIsoField (ISO_FUNCTION_CODE, IsoInfo, "472", 3);
		break;
	}

	/* Field 25: Message Reason Code */
	InitPufInfo (&PufInfo);
	GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &tmpFieldLen);
	AnalysePuf (tmpField, &PufInfo);
	if (GetPufSubField (PUF_MSG_REASON, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		memset (tmpField1, 0, sizeof (tmpField1));
		SmsToIsoReasonCode (tmpField, tmpField1);
		AddIsoField (ISO_MSG_REASON, IsoInfo, tmpField1, tmpFieldLen);
	}

	/* Field 28: Transaction Fee */
	if (GetSmsField (SMS_TRANS_FEE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		InsertIsoField (ISO_FEES_AMOUNT, IsoInfo, tmpField+1, tmpFieldLen-1);            

	/* Field 32: Acquirer Institution Identification Code   */
	GetSmsField (SMS_ACQR_ID, SmsInfo, tmpField, &FieldLen);
	InsertIsoField (ISO_ACQR_ID, IsoInfo, tmpField, FieldLen);

	/* Field 37: Retrieval Reference Number     */
	GetSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, &FieldLen);
	InsertIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, FieldLen);

	/* Field 41: Card Acceeptor Terminal Identification  */
	GetSmsField (SMS_TERMINAL_NBR, SmsInfo, tmpField, &FieldLen);
	InsertIsoField (ISO_TERMINAL_NBR, IsoInfo, tmpField, FieldLen);

	/* Field 42: Card Acceptor Identification Code  */
	GetSmsField (SMS_OUTLET_NBR, SmsInfo, tmpField, &FieldLen);
	InsertIsoField (ISO_OUTLET_NBR, IsoInfo, tmpField, FieldLen);

	/* Field 43: Card Acceptor Name/Location  */
	if (GetSmsField (SMS_TERMINAL_ADR, SmsInfo, tmpField, &FieldLen) == SUCCESS)
		InsertIsoField (ISO_TERMINAL_ADR, IsoInfo, tmpField, FieldLen);

	/* Field 44: Additional Response Data */
	if (GetSmsField (SMS_ADTNL_RESP_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		InsertIsoField (ISO_ADTNL_RESP_DATA, IsoInfo, tmpField, tmpFieldLen);

	/* Field 48: Additinal Data Private */
	/*if (GetSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, tmpFieldLen);*/

/*Start ICH17012018*/
	InitTlvPrivate (&TlvInfo);

	if (GetSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{

		if(tmpField[1]== P7_F048_VCR_USAGE)
			PutTlvPrivate(SMS_VCR_DATA ,&TlvInfo,tmpField + 3,tmpFieldLen - 3);
		else
			PutTlvPrivate(SMS_CHG_DATA ,&TlvInfo,tmpField + 1,tmpFieldLen - 1);
	}

	PutTlvPrivate (HPS_ACQUIRER_BANK_TAG , &TlvInfo, AcquirerBank, 6);

	if (GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		PutTlvPrivate (EXTERNAL_STAN_TAG , &TlvInfo, tmpField, 6);

	if (GetSmsField (SMS_ADTNL_RESP_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		PutTlvPrivate (CVV_RESULT_CODE_TAG, &TlvInfo, tmpField+4, 1);

	tmpFieldLen = PrivateBuildTlv(tmpField, &TlvInfo);

	if(tmpFieldLen >0)
		AddIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, tmpFieldLen);

/*End ICH17012018*/

	/* Field 49: Transaction Currency Code  */
	GetSmsField (SMS_TRANS_CRNCY, SmsInfo, tmpField, &FieldLen);
	InsertIsoField (ISO_TRANS_CRNCY, IsoInfo, tmpField, FieldLen);

	/* Field 50: Settlement Currency Code */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		GetSmsField (SMS_STLMNT_CRNCY, SmsInfo, tmpField, &tmpFieldLen);
		AddIsoField (ISO_STLMNT_CRNCY, IsoInfo, tmpField, tmpFieldLen);
	}

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

	/* Field 56: Original Data Elements   */
	if (GetSmsField (SMS_ORIG_DATA, SmsInfo, SmsOrigData, &FieldLen) == SUCCESS)
	{
		memset (tmpField1,  0, sizeof (tmpField1));
		memset (tmpField2,  0, sizeof (tmpField2));
		memset (sIntYear,   0, sizeof (sIntYear));
		memset (sIntMonth,  0, sizeof (sIntMonth));
		memset (sExtMonth,  0, sizeof (sExtMonth));

		memcpy (tmpField2, SmsOrigData, 4);
		sprintf(tmpField1, "%04d", atoi(tmpField2) + 1000);  /* Message Number */
		memcpy (tmpField1 +  4, SmsOrigData + 4, 6);         /* Audit Number */
		current_date_yymmdd (yymmdd);
		memcpy (sIntYear,   yymmdd, 2);
		memcpy (sIntMonth,  yymmdd+2, 2);
		memcpy (sExtMonth,  SmsOrigData+10, 2);

		if (atoi(sExtMonth) > atoi(sIntMonth))
			nOrigYear = (atoi (sIntYear) - 1) %100;
		else
			nOrigYear = atoi (sIntYear);
		sprintf (tmpField1 +10, "%02d", nOrigYear);

		memcpy (tmpField1 + 12, SmsOrigData + 10, 10);       /* Xmit Date & Time */
		memcpy (tmpField1 + 22, SmsOrigData + 20, 11);       /* Orig Acqr Id */
		memcpy (tmpField1 + 33, SmsOrigData + 31, 11);       /* Orig Forwd Id */

		InsertIsoField (ISO_ORIG_DATA, IsoInfo, tmpField1, 44);
	}

	/* Field: 62: CPS field */
	InitCpsInfo (&CpsInfo);
	memset (sCpsTlvData,    0, sizeof(sCpsTlvData));
	memset (tmpField,       0, sizeof(tmpField));
	memset (tmpField1,      0, sizeof(tmpField1));

	if (GetSmsField (SMS_CPS_FIELDS , SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseCps(tmpField,&CpsInfo);
		InitTlvPrivate (&TlvInfo);
		if (GetCpsSubField (CPS_ACI, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			PutTlvPrivate(CPS_ACI_TAG ,&TlvInfo,tmpField,1);
			if (GetCpsSubField (CPS_TRANS_ID, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
			{
				PutTlvPrivate(CPS_TRANS_ID_TAG ,&TlvInfo, tmpField, tmpFieldLen);
				tmpFieldLen = PrivateBuildTlv(sCpsTlvData, &TlvInfo);
				AddIsoField (ISO_CPS_FIELDS, IsoInfo, sCpsTlvData, tmpFieldLen);
			}
			else
			{
				tmpFieldLen = PrivateBuildTlv(sCpsTlvData, &TlvInfo);
				AddIsoField (ISO_CPS_FIELDS, IsoInfo, sCpsTlvData, tmpFieldLen);
			}
		}
	}

	/* Field 63: Private Use Fields */
	if (GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitPufInfo (&PufInfo);
		AnalysePuf (tmpField, &PufInfo);
		GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, &tmpFieldLen);
		
		InitTlvPrivate(&TlvInfo);
		PutTlvPrivate(NET_NETWORK_ID_TAG, &TlvInfo, tmpField, strlen(tmpField));
		memset (tmpField, 0, sizeof(tmpField));
		
		tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
		
		if (tmpFieldLen > 0)
			AddIsoField (ISO_NETWORK_DATA, IsoInfo, tmpField, tmpFieldLen);

		memset (tmpField,  0, sizeof (tmpField)); 
		memset (tmpField1, 0, sizeof (tmpField1)); 
		if (GetPufSubField (PUF_STIP_REASON, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			memcpy (tmpField1, tmpField, 4);
			if (GetPufSubField (PUF_PLUS_PMC, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
			{
				memcpy (tmpField1+4, tmpField, 6);
				AddIsoField (ISO_RECORD_DATA, IsoInfo, tmpField1, 10);
			}
			else
				AddIsoField (ISO_RECORD_DATA, IsoInfo, tmpField1, 4);
		}
	}

	trace_event("End ChargebackAdvToIso()", PROCESSING);
	return(OK);
}

/*************************************************************************/
/*  ChgBkResponseToNw()                                                  */
/*                                                                       */
/*  Builds a ChargeBack reponse SMS message to be sent.                  */
/*************************************************************************/
int ChgBkResponseToNw (char        *szResponseCode,
	TSSmsInfo   *SmsInfo)
{
	TSSmsInfo    NewSmsInfo;
	TSPufInfo    PufInfo;
	TSPufInfo    NewPufInfo;
	char         tmpField   [BUF_LG_MAX];
	char         tmpField1  [256];
	char         sSmsSndBuf [LG_MAX];
	int          FieldLen;
	int          FieldLen1; 
	int          nMsgLen;
	int          nRetCode;
	int          i;

	trace_event("Start ChgBkResponseToNw ()", PROCESSING);

	InitSmsInfo (&NewSmsInfo);

	memset (tmpField,    0, sizeof (tmpField));
	memset (tmpField1,   0, sizeof (tmpField1));
	memset (sSmsSndBuf,  0, sizeof (sSmsSndBuf));

	NewSmsInfo.nMsgType = 432;

	/* Field 2 */
	GetSmsField (SMS_CARD_NBR, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_CARD_NBR, &NewSmsInfo, tmpField, FieldLen);

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
	GetSmsField (SMS_TERMINAL_NBR, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_TERMINAL_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 42: Card Acceptor Identification Code  */
	GetSmsField (SMS_OUTLET_NBR, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_OUTLET_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 63: Private Use Fields */
	InitPufInfo (&PufInfo);
	InitPufInfo (&NewPufInfo);
	GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &FieldLen);
	AnalysePuf (tmpField, &PufInfo);
	GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField1, &FieldLen1);

	AddPufSubField (PUF_NETWORK_ID, &NewPufInfo, tmpField1, FieldLen1);
	FieldLen1 = PufBuildField (tmpField1, &NewPufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, &NewSmsInfo, tmpField1, FieldLen1);

	/* Field 90: Original Data Elements   */
	GetSmsField (SMS_ORIG_DATA, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_ORIG_DATA, &NewSmsInfo, tmpField, FieldLen);

	/************** Send the request to Network  ************************/
	nMsgLen=SmsBuildMsg(sSmsSndBuf + SMS_HDR_LEN, &NewSmsInfo);
	SmsBuildHeader(RESPONSE, &(NewSmsInfo.sHeader), &(SmsInfo->sHeader), nMsgLen, StationId );

	memcpy(sSmsSndBuf, (char *) &(NewSmsInfo.sHeader), SMS_HDR_LEN);
	nMsgLen += SMS_HDR_LEN;

	DumpSms (sSmsSndBuf);

	if (WriteLineMsg(sSmsSndBuf, nMsgLen) != OK)
	{
		trace_event ("Function ChgBkResponseToNw (): WriteLineMsg() failed", PROCESSING);
		return (NOK);
	} 

	trace_event("End ChgBkResponseToNw (OK)", PROCESSING);
	return(OK);
}

