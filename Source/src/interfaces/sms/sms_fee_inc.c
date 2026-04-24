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

#include <sms_fields.h>
#include <cps_fields.h>
#include <iso_fields.h>

#include <iso_hps.h>
#include <iso_sms.h>
#include <tlv_private.h>
#include <tlv_autho.h>
#include <list_thr.h>
#include <queue.h>
#include <security_data.h>
#include <hsm_inc.h>


/***********************************************************************/
/***                   FCT_FEE_ADV_FROM_NW ()                        ***/
/***  Processes a fee collection message (0422) received from SMS    ***/
/***********************************************************************/
int FCT_FEE_ADV_FROM_NW (int            P_nIndexCtx, 
	TSSmsInfo      *P_psSmsInfo)
{
	TSIsoInfo               sIsoInfo;
	TSTlvAutho              sInternalTlvInfo;
	TSTlvAutho              sIncomingTlvInfo;
	TSTlvPrivate            TlvInfo;
	char                    tmpField        [ LG_MAX ];
	char                    sInternalTlvBuff[ LG_MAX ];
	char                    sIncomingTlvBuff[ LG_MAX ];
	char                    sLine           [ MAX_LINE_TRC ];
	int                     tmpFieldLen;
	int                     nRetCode;
	char 					sPrimResource[RESOURCE_ID_LEN + 1];
	char 					sSecResource[ RESOURCE_ID_LEN + 1 ];
    char                    sStan [6 + 1 ];
    char                    sPan [24 + 1 ];

	trace_event ("Start FCT_FEE_ADV_FROM_NW()", PROCESSING);

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
	/** Conversion a partir du format SmsInfo au format TlvInfo en conservant le contenu du champ 48 **/
	nRetCode = SmsToTlv( P_nIndexCtx, P_psSmsInfo, &sIncomingTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert the ISO message %04d into the Tlv format", sIsoInfo.nMsgType);
		trace_event (sLine, PROCESSING);
		FeesResponseToNw ("96", P_psSmsInfo);
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
		FeesResponseToNw ("96", P_psSmsInfo);
		return (NOK);
	}

	/***********************************************************************/
	/******** Traitement pour l'insertion dans la table AUTHO_ACTIVITY *****/
	/***********************************************************************/

	/****** Conversion du message SMS 0422 en format ISO PowerCARD   *******/

	IncomingFeesToIso(P_nIndexCtx, P_psSmsInfo, &sIsoInfo);

	dump_p7_struct (&sIsoInfo);

	/* Conversion a partir du format IsoInfo au format TlvInfo */
	nRetCode = IsoToTlv( P_nIndexCtx, &sIsoInfo, &sInternalTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert the ISO message %04d into the Tlv format", sIsoInfo.nMsgType);
		trace_event (sLine, PROCESSING);
		FeesResponseToNw ("96", P_psSmsInfo);
		return (NOK);
	}

	/******** Construction de la chaine TLV ***********/
	PutTlvAutho  ( AUTO_CUR_TABLE_INDICATOR, &sInternalTlvInfo, "MSC", 3);
	PutTlvAutho  ( AUTO_ROUTING_CODE, &sInternalTlvInfo, "000000", 6);
	PutTlvAutho  ( AUTO_CAPTURE_CODE, &sInternalTlvInfo, ResourceId, 6);
	PutTlvAutho  ( AUTO_NETWORK_CODE, &sInternalTlvInfo, "01", 2);/*VISA*/
	PutTlvAutho ( AUTO_ISSUING_BANK , &sInternalTlvInfo, AcquirerBank , ACQUIRER_BANK_LEN);
	PutTlvAutho ( AUTO_ACQUIRER_BANK, &sInternalTlvInfo, AcquirerBank , ACQUIRER_BANK_LEN);
	/* 48 in TLV format. */

	/*ICH17012018*/
	/*InitTlvPrivate (&TlvInfo);
	AddTlvPrivate (HPS_ACQUIRER_BANK_TAG , &TlvInfo, AcquirerBank, 6);
	if (GetSmsField (SMS_AUDIT_NBR, P_psSmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddTlvPrivate (EXTERNAL_STAN_TAG , &TlvInfo, tmpField, 6);
	if (GetSmsField (SMS_ADTNL_RESP_DATA, P_psSmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddTlvPrivate (CVV_RESULT_CODE_TAG, &TlvInfo, tmpField+4, 1);

	tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
	if (tmpFieldLen > 0)
		PutTlvAutho  ( AUTO_ADD_DATA_PRIVATE, &sInternalTlvInfo, tmpField, tmpFieldLen);*/

	AuthoBuildTlv ( sInternalTlvBuff, &sInternalTlvInfo);

	/********* Insertion dans Autho Activity **********/
	nRetCode = InsertionAuthoActivity( P_nIndexCtx, sInternalTlvBuff);
	if ( nRetCode != OK )
	{
		trace_event  ( "SEVERE ERROR !!!!!", PROCESSING );
		trace_event  ( "Error On Insert Into Autho Activity Table ", PROCESSING );
		FeesResponseToNw ("96", P_psSmsInfo);
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
                                    "1722",
                                     'A',
                                     sStan,
                                     ResourceId,
                                     sPrimResource,
                                     0,
                                     "000",
                                     sInternalTlvBuff,
                                     sPan);
	}

	nRetCode = FeesResponseToNw ("00", P_psSmsInfo);
	if ( nRetCode == OK )
	{
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sInternalTlvBuff , "000" , 3);
		trace_event  ( "--------- UPDATE AUTHO ACTIVITY TABLE ----------", PROCESSING );
		UpdateAuthoActivity ( P_nIndexCtx, sInternalTlvBuff);
	}

	trace_event ("End FCT_FEE_ADV_FROM_NW()", PROCESSING);
	return (OK);
}
/*************************************************************************/
/*  IncomingFeesToIso()                                                  */
/*                                                                       */
/*  Converts an issuer-generated SMS fees collection message to ISO.     */
/*  Params: SmsInfo: received SMS message                                */
/*          IsoInfo: structure to contain ISO message                    */
/*************************************************************************/
int IncomingFeesToIso(  int nIndexCtx,
	TSSmsInfo   *SmsInfo,
	TSIsoInfo   *IsoInfo)
{

	TSPufInfo   PufInfo;
	TSCpsInfo   CpsInfo;
	TSTlvPrivate   TlvInfo;

	char    tmpField          [BUF_LG_MAX];
	char    tmpField1         [256];
	char    sPid              [ 8 + 1];
	char    ProcCode          [7];           
	char    yymmdd            [8];
	char    sIntMonth         [3];
	char    sExtMonth         [3];
	char    sIntDate          [5];
	char    sIntYear          [3];
	char    MerchantType      [5];           
	char    szProcessingCode  [3];
	int     tmpFieldLen;
	int     tmpFieldLen1;
	int     nYear;
	int     nSettlYear;
	char    sLine           [ MAX_LINE_TRC ];
	char    szGMTDate[12 + 1];      /*AMER 20160719*/
	char    szTrxDateTime[12 + 1];  /*AMER 20160719*/
	char    sStlmntDate[6 + 1];

	trace_event("Start IncomingFeesToIso()", PROCESSING);

	/***** Initialisation des variables de travail *****/
	memset (yymmdd,           0, sizeof(yymmdd));
	memset (tmpField,         0, sizeof(tmpField));
	memset (tmpField1,        0, sizeof(tmpField1));
	memset (sIntYear,         0, sizeof (sIntYear));
	memset (sIntMonth,        0, sizeof (sIntMonth));
	memset (sExtMonth,        0, sizeof (sExtMonth));
	memset (sIntDate,         0, sizeof (sIntDate));
	memset (ProcCode,         0, sizeof (ProcCode));
	memset (szProcessingCode, 0, sizeof (szProcessingCode));
	memset (MerchantType,     0, sizeof (MerchantType));
	memset(szGMTDate,'\0',sizeof(szGMTDate));      		/*AMER 20160719*/
	memset(szTrxDateTime,'\0',sizeof(szTrxDateTime));       /*AMER 20160719*/
	memset(sStlmntDate,'\0',sizeof(sStlmntDate));
	

	current_gmt_date (szGMTDate);   /*AMER 20160719*/
	
	/*IsoInfo->nMsgType = 1422;ICH why 1722*/
	IsoInfo->nMsgType = 1722;

	/* Field 2: Primary Account Number */
	if (GetSmsField (SMS_CARD_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_CARD_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 3: Processing Code */
	GetSmsField (SMS_PROC_CODE, SmsInfo, ProcCode, &tmpFieldLen);
	GetSmsField (SMS_MERCHANT_TYPE, SmsInfo, MerchantType, &tmpFieldLen);
	SmsToIsoProcessingCode (ProcCode, MerchantType, tmpField);
	AddIsoField (ISO_PROC_CODE, IsoInfo, tmpField, 6);
	memcpy (szProcessingCode, ProcCode, 2);

	/* Field 4: Transaction Amount */
	GetSmsField (SMS_TRANS_AMOUNT, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_TRANS_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 5: Settlement Amount */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_STLMNT_AMOUNT, IsoInfo, tmpField, tmpFieldLen);

	/* Field 7: Transmission Date and Time */
	GetSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_XMIT_TIME, IsoInfo, tmpField, tmpFieldLen);

	/* Field 9: Settlement Conversion Rate */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		GetSmsField (SMS_CNVRSN_RATE, SmsInfo, tmpField, &tmpFieldLen);
		AddIsoField (ISO_STLMNT_CNVRSN_RATE, IsoInfo, tmpField, tmpFieldLen);
	}

	/* Field 11: Trace Audit Number */
	GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_AUDIT_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 12: Transaction Date & Time */
	current_date_yymmdd(yymmdd);
	/* Advice does not contain TransDate therefore we force XmitDateTime */

	/*memcpy (sIntYear, yymmdd, 2);
	memcpy (sIntDate, yymmdd+2, 4);
	memset (tmpField,  0, sizeof (tmpField));
	GetSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, &tmpFieldLen);
	if ( (memcmp(sIntDate,"0101", 4) == 0) && (memcmp(tmpField,"1231", 4) == 0))
		nYear = (atoi(sIntYear) - 1)%100 ;
	else
	{
		if ( (memcmp(sIntDate,"1231", 4) == 0) && (memcmp(tmpField,"0101", 4) == 0))
			nYear = (atoi(sIntYear) + 1)%100;
		else
			nYear = atoi(sIntYear);
	}

	memset  (tmpField1,   0, sizeof (tmpField1));
	sprintf (tmpField1, "%02d", nYear);

	GetSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, &tmpFieldLen1);
	memcpy (tmpField1 + 2, tmpField, 10);
	AddIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField1, 12);*/
	
	/*AMER 20160719 Start: PROD00029786*/
		GetSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, &tmpFieldLen1);
        	memcpy(szTrxDateTime + 2,tmpField,4);
		memcpy(szTrxDateTime + 6,tmpField+4,6);
		SetLocalYear(szTrxDateTime,szGMTDate);
		AddIsoField (ISO_TRANS_DATE_TIME, IsoInfo, szTrxDateTime, 12);
    	/*AMER 20160719 End*/

	/* Field 15: Settlement Date */
	if(GetSmsField (SMS_STLMNT_DATE, SmsInfo, sStlmntDate + 2, &tmpFieldLen)== SUCCESS)
	{
		SetLocalYear(sStlmntDate,szGMTDate);
		InsertIsoField (ISO_STLMNT_DATE, IsoInfo, sStlmntDate, 6);
	}

/*Start ICH17012018
	if ( GetSmsField (SMS_STLMNT_DATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		memcpy (sIntMonth,  yymmdd+2, 2);
		memcpy (sExtMonth,  tmpField, 2);
		if (sExtMonth > sIntMonth)
			nSettlYear = (atoi (sIntYear) - 1)%100;
		else
			nSettlYear = atoi (sIntYear);

		memset (tmpField1,       0, sizeof (tmpField1));
		sprintf(tmpField1, "%02d", nSettlYear);

		memcpy (tmpField1+2,  tmpField, 4);

		AddIsoField (ISO_STLMNT_DATE, IsoInfo, tmpField1, 6);
	}*/


	/* Field 16: Conversion Date */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		GetSmsField (SMS_CNVRSN_DATE, SmsInfo, tmpField, &tmpFieldLen);
		AddIsoField (ISO_CNVRSN_DATE, IsoInfo, tmpField, tmpFieldLen);
	}

	/* Field 19: Acquiring Institution Country Code */
	if (GetSmsField (SMS_ACQR_COUNTRY, SmsInfo, tmpField, &tmpFieldLen)== SUCCESS)
	AddIsoField (ISO_ACQR_COUNTRY, IsoInfo, tmpField, tmpFieldLen);

	/* Field 21: Forwarding Institution Country Code */
	if (GetSmsField (SMS_FORWD_COUNTRY, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_FORWD_COUNTRY, IsoInfo, tmpField, tmpFieldLen);

	/* Field 24: Function Code */
	switch (atoi(szProcessingCode))
	{
	case 19:  /* Fees collection */
		AddIsoField (ISO_FUNCTION_CODE, IsoInfo, "469", 3);
		break;
	case 29:  /* Funds disbursment */
		AddIsoField (ISO_FUNCTION_CODE, IsoInfo, "479", 3);
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

	/* Field 32: Acquiring Institution Identification Code */
	GetSmsField (SMS_ACQR_ID, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_ACQR_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 33: Forwarding Institution Identification */
	GetSmsField (SMS_FORWD_ID, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_FORWD_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 37: Retrieval Reference Number */
	GetSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 41: Card Acceptor Term Id */
	/* For Autho Activity Insertion */
	memset (tmpField, ' ' , sizeof (tmpField));
	memcpy (tmpField, "00000000", 8);
	AddIsoField (ISO_TERMINAL_NBR, IsoInfo, tmpField, 8);

	/* Field 48: Additional Data Private */
	/*if (GetSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, tmpFieldLen);*/

/*Start ICH17012018*/
	InitTlvPrivate (&TlvInfo);

	if (GetSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		if(memcmp(tmpField,"VCR",3) == 0)
		{
			PutTlvPrivate(SMS_VCR_DATA ,&TlvInfo,tmpField + 4,tmpFieldLen - 4);
		}
		else
		{
			PutTlvPrivate(MESSAGE_TEXT_TAG ,&TlvInfo,tmpField,tmpFieldLen );/*Old Value*/
		}
	}

	PutTlvPrivate (HPS_ACQUIRER_BANK_TAG , &TlvInfo, AcquirerBank, 6);

	if (GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		PutTlvPrivate (EXTERNAL_STAN_TAG , &TlvInfo, tmpField, 6);

	if (GetSmsField (SMS_ADTNL_RESP_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		PutTlvPrivate (CVV_RESULT_CODE_TAG, &TlvInfo, tmpField+4, 1);

	tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
	if (tmpFieldLen > 0)
		AddIsoField  ( ISO_ADTNL_DATA_PRIV, IsoInfo ,tmpField, tmpFieldLen);

/*End ICH17012018*/


	/* Field 49: Transaction Currency Code */
	GetSmsField (SMS_TRANS_CRNCY, SmsInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_TRANS_CRNCY, IsoInfo, tmpField, tmpFieldLen);

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

	/* Field: 62: CPS field */
	InitCpsInfo (&CpsInfo);
	if (GetSmsField (SMS_CPS_FIELDS, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseCps (tmpField, &CpsInfo);
		if (GetCpsSubField (CPS_ACI, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			if (GetCpsSubField (CPS_TRANS_ID, &CpsInfo, tmpField1, &tmpFieldLen1) == SUCCESS)
			{
				memcpy (tmpField + 1, tmpField1, tmpFieldLen1);
				AddIsoField (ISO_CPS_FIELDS, IsoInfo, tmpField, tmpFieldLen1 + 1);
			}
			else
				AddIsoField (ISO_CPS_FIELDS, IsoInfo, tmpField, 1);
		}
	}

	/* Field 63: PUF fields */
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

	trace_event("End IncomingFeesToIso()", PROCESSING);

	return(OK);
}

/*************************************************************************/
/*  FeesResponseToNw()                                                   */
/*                                                                       */
/*  Builds a Fees Collection reponse SMS message to be sent.             */
/*************************************************************************/
int FeesResponseToNw  (char      *szResponseCode,
	TSSmsInfo *SmsInfo)
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

	trace_event("Start FeesResponseToNw ()", PROCESSING);

	InitSmsInfo (&NewSmsInfo);

	memset (tmpField,    0, sizeof (tmpField));
	memset (tmpField1,   0, sizeof (tmpField1));
	memset (sSmsSndBuf,  0, sizeof (sSmsSndBuf));

	NewSmsInfo.nMsgType = 432;

	/* Field 2 */
	if (GetSmsField (SMS_CARD_NBR, SmsInfo, tmpField, &FieldLen) == SUCCESS)
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
	if (GetSmsField (SMS_STLMNT_DATE, SmsInfo, tmpField, &FieldLen) == SUCCESS)
		AddSmsField (SMS_STLMNT_DATE, &NewSmsInfo, tmpField, FieldLen);

	/* Field 19: Acquiring Institution Country Code */
	GetSmsField (SMS_ACQR_COUNTRY, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_ACQR_COUNTRY, &NewSmsInfo, tmpField, FieldLen);

	/* Field 32: Acquirer Institution Identification Code   */
	GetSmsField (SMS_ACQR_ID, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_ACQR_ID, &NewSmsInfo, tmpField, FieldLen);

	/* Field 33: Forwarding Institution Identification Code   */
	GetSmsField (SMS_FORWD_ID, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_FORWD_ID, &NewSmsInfo, tmpField, FieldLen);

	/* Field 37: Retrieval Reference Number     */
	GetSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_REFERENCE_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 39: Response Code   */
	AddSmsField (SMS_RESPONSE_CODE, &NewSmsInfo, szResponseCode, 2);

	/* Field 63: Private Use Fields */
	InitPufInfo (&PufInfo);
	InitPufInfo (&NewPufInfo);
	GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &FieldLen);
	AnalysePuf (tmpField, &PufInfo);
	GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField1, &FieldLen1);

	AddPufSubField (PUF_NETWORK_ID, &NewPufInfo, tmpField1, FieldLen1);
	FieldLen1 = PufBuildField (tmpField1, &NewPufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, &NewSmsInfo, tmpField1, FieldLen1);

	/************** Send the request to Network  ************************/
	nMsgLen=SmsBuildMsg(sSmsSndBuf + SMS_HDR_LEN, &NewSmsInfo);
	SmsBuildHeader(RESPONSE, &(NewSmsInfo.sHeader), &(SmsInfo->sHeader), nMsgLen, StationId );
	memcpy(sSmsSndBuf, (char *) &(NewSmsInfo.sHeader), SMS_HDR_LEN);
	nMsgLen += SMS_HDR_LEN;

	DumpSms (sSmsSndBuf);

	if (WriteLineMsg(sSmsSndBuf, nMsgLen) != OK)
	{
		trace_event ("Function FeesResponseToNw (): WriteLineMsg() failed", PROCESSING);
		return (NOK);
	}

	trace_event("End FeesResponseToNw (OK)", PROCESSING);
	return(OK);
}

