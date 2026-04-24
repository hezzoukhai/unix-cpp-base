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
#include <iso_hps.h>
#include <iso_sms.h>
#include <tlv_private.h>
#include <tlv_autho.h>
#include <list_thr.h>
#include <queue.h>
#include <security_data.h>
#include <hsm_inc.h>

/*****************************************************************************/
/***                   FCT_TXT_MSG_FROM_NW ()                      ***********/
/***  Processes an administrative text message (0600) received from SMS  *****/
/*****************************************************************************/
int FCT_TXT_MSG_FROM_NW (int            P_nIndexCtx,
	TSSmsInfo      *P_psSmsInfo)
{
	TSIsoInfo               sIsoInfo;
	TSTlvAutho              sInternalTlvInfo;
	TSTlvAutho              sIncomingTlvInfo;
	TSTlvPrivate            TlvInfo;
	char                    tmpField        [ BUF_LG_MAX ];
	char                    sInternalTlvBuff[ LG_MAX ];
	char                    sIncomingTlvBuff[ LG_MAX ];
	char                    sLine           [ MAX_LINE_TRC ];
	int                     tmpFieldLen;
	int                     nRetCode;
	char 					sPrimResource[RESOURCE_ID_LEN + 1];
	char 					sSecResource[ RESOURCE_ID_LEN + 1 ];
    char                    sStan [6 + 1 ];
    char                    sPan [24 + 1 ];

	trace_event ("Start FCT_TXT_MSG_FROM_NW ()", PROCESSING);

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
	PutSmsField (SMS_PROC_CODE, P_psSmsInfo, "830000", 6);
	nRetCode = SmsToTlv( P_nIndexCtx, P_psSmsInfo, &sIncomingTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert the ISO message %04d into the Tlv format", sIsoInfo.nMsgType);
		trace_event (sLine, PROCESSING);
		TextResponseToNw ("96", P_psSmsInfo);
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
		TextResponseToNw ("96", P_psSmsInfo);
		return (NOK);
	}

	/***********************************************************************/
	/******** Traitement pour l'insertion dans la table AUTHO_ACTIVITY *****/
	/***********************************************************************/

	/****** Conversion du message SMS 0422 en format ISO PowerCARD   *******/
	TextIncomingToIso(P_nIndexCtx, P_psSmsInfo, &sIsoInfo);

	dump_p7_struct (&sIsoInfo);

	/* Conversion a partir du format IsoInfo au format TlvInfo */
	nRetCode = IsoToTlv( P_nIndexCtx, &sIsoInfo, &sInternalTlvInfo);
	if  (nRetCode != OK)
	{
		sprintf (sLine, "Can not convert the ISO message %04d into the Tlv format", sIsoInfo.nMsgType);
		trace_event (sLine, PROCESSING);
		TextResponseToNw ("96", P_psSmsInfo);
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
	InitTlvPrivate (&TlvInfo);
	AddTlvPrivate (HPS_ACQUIRER_BANK_TAG , &TlvInfo, AcquirerBank, 6);
	if (GetSmsField (SMS_AUDIT_NBR, P_psSmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddTlvPrivate (EXTERNAL_STAN_TAG , &TlvInfo, tmpField, 6);
	if (GetSmsField (SMS_ADTNL_RESP_DATA, P_psSmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddTlvPrivate (CVV_RESULT_CODE_TAG, &TlvInfo, tmpField+4, 1);

	tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
	if (tmpFieldLen > 0)
		PutTlvAutho  ( AUTO_ADD_DATA_PRIVATE, &sInternalTlvInfo, tmpField, tmpFieldLen);

	AuthoBuildTlv( sInternalTlvBuff, &sInternalTlvInfo);

	/********* Insertion dans Autho Activity **********/
	nRetCode = InsertionAuthoActivity( P_nIndexCtx, sInternalTlvBuff);
	if ( nRetCode != OK )
	{
		trace_event  ( "SEVERE ERROR !!!!!", PROCESSING );
		trace_event  ( "Error On Insert Autho Activity File ", PROCESSING );
		TextResponseToNw ("96", P_psSmsInfo);
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
		if(GetIsoField (ISO_AUDIT_NBR, &sIsoInfo, sPan, &tmpFieldLen) != SUCCES)
			memcpy(sPan,"0000000000000000",16);

        nRetCode =  InsertSafData  ( P_nIndexCtx,
                                    "1600",
                                     'A',
                                     sStan,
                                     ResourceId,
                                     sPrimResource,
                                     0,
                                     "000",
                                     sInternalTlvBuff,
                                     sPan);
	}

	nRetCode = TextResponseToNw ("00", P_psSmsInfo);
	if ( nRetCode == OK )
	{
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sInternalTlvBuff , "000" , 3);
		UpdateAuthoActivity ( P_nIndexCtx, sInternalTlvBuff);
	}

	trace_event ("End FCT_TXT_MSG_FROM_NW()", PROCESSING);
	return (OK);
}
/***********************************************************************/
/**********           TextIncomingToIso ()                  ************/
/*** Constructs an Iso message (1422) from the Sms received message  ***/
/***********************************************************************/
int TextIncomingToIso  (int         nIndexCtx,
	TSSmsInfo   *SmsInfo,
	TSIsoInfo   *IsoInfo)
{
	TSPufInfo      PufInfo;
	TSCpsInfo      CpsInfo;
	TSTlvPrivate   TlvInfo;

	char           yymmdd       [8];
	char           sIntYear     [3];
	char           sIntMonth    [3];
	char           sExtMonth    [3];
	char           sPid         [8+1];
	char           tmpField     [BUF_LG_MAX];
	char           tmpField1    [BUF_LG_MAX];
	int            tmpFieldLen;
	int            tmpFieldLen1;
	int            nSettlYear;

	trace_event ("Start  TextIncomingToIso()", PROCESSING);

	memset (yymmdd,     0, sizeof (yymmdd));
	memset (tmpField,   0, sizeof (tmpField));
	memset (tmpField1,  0, sizeof (tmpField1));

	InitIsoInfo (IsoInfo);

	IsoInfo->nMsgType = SmsInfo->nMsgType + 1000;

	/* Field 3: Procesisng Code */
	InsertIsoField (ISO_PROC_CODE, IsoInfo, "830000", 6);

	/* Field 7: Transmission Date & Time */
	GetSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, &tmpFieldLen);
	InsertIsoField  (ISO_XMIT_TIME, IsoInfo, tmpField, tmpFieldLen);

	/* Field 11: System Trace Audit Number */
	GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &tmpFieldLen);
	InsertIsoField  (ISO_AUDIT_NBR, IsoInfo, tmpField, 6);

	/* Field 15: Settlement Date */
	memset (tmpField,     0, sizeof (tmpField));
	memset (sIntYear,     0, sizeof (sIntYear));
	memset (sIntMonth,    0, sizeof (sIntMonth));
	memset (sExtMonth,    0, sizeof (sExtMonth));

	GetSmsField (SMS_STLMNT_DATE, SmsInfo, tmpField, &tmpFieldLen);
	current_date_yymmdd (yymmdd);
	memcpy (sIntYear,   yymmdd, 2);
	memcpy (sIntMonth,  yymmdd+2, 2);
	memcpy (sExtMonth,  tmpField, 2);
	if (sExtMonth > sIntMonth  && memcmp(sExtMonth, "12", 2) == 0)
		nSettlYear = (atoi (sIntYear) - 1) %100;
	else
		nSettlYear = atoi (sIntYear);

	memset (tmpField1,       0, sizeof (tmpField1));
	sprintf(tmpField1, "%2d", nSettlYear);

	memcpy (tmpField1+2,  tmpField, 4);
	InsertIsoField (ISO_STLMNT_DATE, IsoInfo, tmpField1, 6);

	/* Field 21: Forwarding Institution Country Code */
	GetSmsField (SMS_FORWD_ID, SmsInfo, tmpField, &tmpFieldLen);
	if (!memcmp (tmpField, "59", 2))
	{
		if (GetSmsField (SMS_FORWD_COUNTRY, SmsInfo, tmpField1, &tmpFieldLen1) == SUCCES)
			InsertIsoField (ISO_FORWD_COUNTRY, IsoInfo, tmpField1, tmpFieldLen1);
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

	/* Field 33: Forwarding Institution Identification Code   */
	GetSmsField (SMS_FORWD_ID, SmsInfo, tmpField, &tmpFieldLen);
	InsertIsoField (ISO_FORWD_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 37: Retrieval Reference Number     */
	GetSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, &tmpFieldLen);
	InsertIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 41: Card Acceeptor Terminal Identification  */
	/* For Autho Activity Insertion */                    
	memset (tmpField, ' ' , sizeof (tmpField));           
	memcpy (tmpField, "00000000", 8);                     
	AddIsoField (ISO_TERMINAL_NBR, IsoInfo, tmpField, 8); 

	/* Field 43: Card Acceptor Location */
	if (GetSmsField (SMS_TERMINAL_ADR, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		InsertIsoField (ISO_TERMINAL_ADR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 48: Additinal Data Private */
	if (GetSmsField (SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		InsertIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, tmpFieldLen);

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

	/* Field 68: Receiving Institution Country Code */
	GetSmsField (SMS_RECEIVER_ID, SmsInfo, tmpField, &tmpFieldLen);
	if (!memcmp (tmpField, "59", 2))
	{
		if (GetSmsField (SMS_RECEIVER_COUNTRY, SmsInfo, tmpField1, &tmpFieldLen1) == SUCCES)
			InsertIsoField (ISO_RECEIVER_COUNTRY, IsoInfo, tmpField1, tmpFieldLen1);
	}

	/* Field 70: Network Management Identification Code */
	GetSmsField (SMS_NW_MNGMT_INFO, SmsInfo, tmpField, &tmpFieldLen);                                        
	InsertIsoField (ISO_NW_MNGMT_INFO, IsoInfo, tmpField, tmpFieldLen);   

	/* Field 100: Receiving Institution Identification Code */
	GetSmsField (SMS_RECEIVER_ID, SmsInfo, tmpField, &tmpFieldLen); 
	InsertIsoField (ISO_RECEIVER_ID, IsoInfo, tmpField, tmpFieldLen);  

	/* Field 115: Additional Trace Data */
	if (GetSmsField (SMS_ADTNL_TRACE_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)                                        
		InsertIsoField (ISO_ADTNL_TRACE_DATA, IsoInfo, tmpField, tmpFieldLen);             

	trace_event ("End TextIncomingToIso()", PROCESSING);

	return (OK);
}
/*************************************************************************/
/*        TextResponseToNw()                                             */
/*                                                                       */
/*        Builds a Text reponse SMS message to be sent.                  */
/*************************************************************************/
int TextResponseToNw  (char      *szResponseCode,
	TSSmsInfo *SmsInfo)
{
	TSSmsInfo    NewSmsInfo;
	TSPufInfo    PufInfo;
	TSPufInfo    NewPufInfo;
	char         tmpField   [BUF_LG_MAX];
	char         sSmsSndBuf [LG_MAX];
	int          FieldLen;
	int          FieldLen1;
	int          nMsgLen;
	int          nRetCode;

	trace_event("Start TextResponseToNw ()", PROCESSING);

	InitSmsInfo (&NewSmsInfo);

	memset (tmpField,    0, sizeof (tmpField));
	memset (sSmsSndBuf,  0, sizeof (sSmsSndBuf));

	NewSmsInfo.nMsgType = SmsInfo->nMsgType + 10; 

	/* Field 7: Transmission Date & Time */
	GetSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_XMIT_TIME, &NewSmsInfo, tmpField, FieldLen);

	/* Field 11: System Trace Audit Number */
	GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_AUDIT_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 15: Settlement Date */
	GetSmsField (SMS_STLMNT_DATE, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_STLMNT_DATE, &NewSmsInfo, tmpField, FieldLen);

	/* Field 37: Retrieval Reference Number     */
	GetSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_REFERENCE_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 39: Response Code */
	AddSmsField (SMS_RESPONSE_CODE, &NewSmsInfo, szResponseCode, 2);

	/* Field 41: Card Acceeptor Terminal Identification  */
	if (GetSmsField (SMS_TERMINAL_NBR, SmsInfo, tmpField, &FieldLen) == SUCCES)
		AddSmsField (SMS_TERMINAL_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 42: Card Acceptor Identification Code  */
	if (GetSmsField (SMS_OUTLET_NBR, SmsInfo, tmpField, &FieldLen) == SUCCES)
		AddSmsField (SMS_OUTLET_NBR, &NewSmsInfo, tmpField, FieldLen);

	/* Field 63: Private Use Fields */
	InitPufInfo (&PufInfo);
	InitPufInfo (&NewPufInfo);
	GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &FieldLen);
	AnalysePuf (tmpField, &PufInfo);

	GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, &FieldLen1);
	AddPufSubField (PUF_NETWORK_ID, &NewPufInfo, tmpField, FieldLen1);
	
	FieldLen1 = PufBuildField (tmpField, &NewPufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, &NewSmsInfo, tmpField, FieldLen1);

	/* Field 68: Receiving Institution Country Code */
	if (GetSmsField (SMS_FORWD_ID, SmsInfo, tmpField, &FieldLen)== SUCCESS)
	{
		if (GetSmsField (SMS_FORWD_COUNTRY, SmsInfo, tmpField, &FieldLen1) == SUCCES)
			AddSmsField (SMS_RECEIVER_COUNTRY, &NewSmsInfo, tmpField, FieldLen1);
	}

	/* Field 70: Network Management Identification Code */
	GetSmsField (SMS_NW_MNGMT_INFO, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_NW_MNGMT_INFO, &NewSmsInfo, tmpField, FieldLen);

	/* Field 100: Receiving Institution Identification Code */
	GetSmsField (SMS_RECEIVER_ID, SmsInfo, tmpField, &FieldLen);
	AddSmsField (SMS_RECEIVER_ID, &NewSmsInfo, tmpField, FieldLen);

	/* Field 115: Additional Trace Data */
	if (GetSmsField (SMS_ADTNL_TRACE_DATA, SmsInfo, tmpField, &FieldLen) == SUCCES)
		AddSmsField (SMS_ADTNL_TRACE_DATA, &NewSmsInfo, tmpField, FieldLen);

	/************** Send the request to Network  ************************/
	nMsgLen=SmsBuildMsg(sSmsSndBuf + SMS_HDR_LEN, &NewSmsInfo);
	SmsBuildHeader(RESPONSE, &(NewSmsInfo.sHeader), &(SmsInfo->sHeader), nMsgLen, StationId );
	memcpy(sSmsSndBuf, (char *) &(NewSmsInfo.sHeader), SMS_HDR_LEN);
	nMsgLen += SMS_HDR_LEN;

	DumpSms (sSmsSndBuf);

	if (WriteLineMsg(sSmsSndBuf, nMsgLen) != OK)
	{
		trace_event ("Function TextResponseToNw (): WriteLineMsg() failed", PROCESSING);
		return (NOK);
	}

	trace_event("End TextResponseToNw (OK)", PROCESSING);
	return(OK);
}

