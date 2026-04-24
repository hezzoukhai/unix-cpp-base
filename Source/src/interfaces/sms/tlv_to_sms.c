#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <iso_cps.h>

#include <tlv_autho.h>
#include <tlv_private.h>
#include <sms_fields.h>
#include <sms_puf.h>
#include <sms_param.h>
#include <sms_define.h>

#include <security_data.h>

/************************************************************************************************/
/*   Fonction qui convertit un message Tlv en un message Sms                                    */
/*   Usage unique pour les representments, fees, text messages, et adjustments : (Outgoing)     */
/*   Ghassane Fahmi   Le 10/03/2003        - Initial Version -                                  */
/************************************************************************************************/
int TlvToSms (int nIndiceCtx, TSTlvAutho *tlvInfo, TSSmsInfo * SmsInfo)
{
	TSPufInfo      PufInfo;
	TSCpsInfo      CpsInfo;
	TSSmsInfo      SmsInfo1;
	char           sLine     [ MAX_LINE_TRC ];
	char           tmpField  [ BUF_LG_MAX ];
	char           tmpField1 [ 256 ];
	char           FieldData [ 12 ];
	char           sAbrevBankName[ 16 ];
	char           isoPosMode[12 + 1];
	char           sProcCode [ 6 + 1];
	char           NwData    [ LG_MAX ];
	char           tagName   [ 4 ];
	char           sPosCode  [ 2+1 ];
	char           sMti      [ 4+1 ];
	int            nRetCode;
	int            nLength;
	int            tmpFieldLen;
	int            tmpFieldLen1;
	int            i;
	TSTlvPrivate   TlvInfoTmp, kTlvInfo;

	trace_event ( "Start TlvToSms ()", PROCESSING );

	/*** Initialisation des variables de travail ****/
	memset ( sLine,         0 , sizeof ( sLine));
	memset ( tmpField1,     0 , sizeof ( tmpField1));
	memset ( FieldData,     0 , sizeof ( FieldData ));
	memset ( sAbrevBankName,0 , sizeof ( sAbrevBankName ));
	memset ( isoPosMode    ,0 , sizeof ( isoPosMode ));
	memset ( sPosCode      ,0 , sizeof ( sPosCode ));
	memset ( sProcCode     ,0 , sizeof ( sProcCode ));
	memset ( NwData,        0 , sizeof ( NwData ));
	memset ( tagName,       0 , sizeof ( tagName ));
	memset ( sMti,          0 , sizeof ( sMti ));

	/*************************************************************/
	/****  Message Type                                      *****/
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_MSG_TYPE, tlvInfo, sMti, &nLength) ==SUCCESS )
		SmsInfo->nMsgType = atoi(sMti);

	/*************************************************************/
	/****  Field 2 : Primary Account Number                 ******/
	/*************************************************************/
	memset ( tmpField,     0 , sizeof ( tmpField));
	if ( GetTlvAutho ( AUTO_PAN, tlvInfo, tmpField, &nLength) ==SUCCESS )
		AddSmsField ( SMS_CARD_NBR, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 3 : Processing Code                        ******/
	/*************************************************************/
	memset ( tmpField,     0 , sizeof ( tmpField));
	if ( memcmp(sMti,"0600",4) != 0 && memcmp(sMti,"0620",4) != 0)
	{
		if ( GetTlvAutho ( AUTO_PROCESSING_CODE, tlvInfo, tmpField, &nLength) ==SUCCES )
		{
			if ( GetTlvAutho ( AUTO_MERCHANT_TYPE, tlvInfo, FieldData, &nLength) ==SUCCES )
			{ 
				IsoToSmsProcessingCode ( tmpField,tmpField1,FieldData);
				memcpy(tmpField, tmpField1, 6);
			}
			AddSmsField ( SMS_PROC_CODE, SmsInfo, tmpField, 6);
			memcpy (sProcCode, tmpField, 6);
		}
	}

	/*************************************************************/
	/****  Field 4 : Transaction Amount                     ******/
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_AMOUNT_TRANSACTION, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_TRANS_AMOUNT, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 7 : Transmission Date & Time               ******/
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_TRANS_DATE_TIME, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_XMIT_TIME, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 11 : Trace Audit Number                    ******/
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_INTERNAL_STAN, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_AUDIT_NBR, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 12 : Transaction Local Time                ******/
	/*************************************************************/
	/*Start AMER20211004 PLUTONL-3579*/
	if (GetTlvAutho(AUTO_NETWORK_POS_DATA, tlvInfo, tmpField, &nLength) == SUCCES)
	{
		InitTlvPrivate(&kTlvInfo);
		AnalyseTlvPrivate(tmpField, nLength, &kTlvInfo);
	}
	
	GetTlvPrivate(POS_CONDITION_CODE_TAG, &kTlvInfo, sPosCode, &nLength);
	
	if ( GetTlvAutho ( AUTO_DATE_TIME_LOCAL_TRAN, tlvInfo, tmpField, &nLength) ==SUCCES )
	{
		if (memcmp(sPosCode, "13", 2) != 0)		/*F012 is not used in 0220 dispute response financial*/
		{
			AddSmsField ( SMS_TRANS_TIME, SmsInfo, tmpField + 6, 6);
		}
		AddSmsField ( SMS_TRANS_DATE, SmsInfo, tmpField + 2 , 4);
	}
	/*End AMER20211004 PLUTONL-3579*/

	/*************************************************************/
	/****  Field 13 : Transaction Local Date                ******/
	/*************************************************************/
	/*if ( GetTlvAutho ( AUTO_EFFECTIVE_DATE, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_TRANS_DATE, SmsInfo, tmpField, nLength);*/
	
	/*************************************************************/ 
	/****  Field 18 : Merchant Type                         ******/ 
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_MERCHANT_TYPE, tlvInfo, tmpField, &nLength) ==SUCCES )        
		AddSmsField ( SMS_MERCHANT_TYPE, SmsInfo, tmpField, nLength);

	/*************************************************************/ 
	/****  Field 19 : Acquiring Institution Country Code    ******/        
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_ACQU_INST_COUNTRY, tlvInfo, tmpField, &nLength) ==SUCCES ) 
		AddSmsField ( SMS_ACQR_COUNTRY, SmsInfo, tmpField, nLength);

	/*************************************************************/ 
	/****  Field 20 : PAN Extended Country Code             ******/                          
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_PAN_EXTENDED_COUNTRY, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_PAN_COUNTRY, SmsInfo, tmpField, nLength);

	/*************************************************************/ 
	/****  Field 21 : Forwarding Institution Country Code   ******/                          
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_FORW_INST_COUNTRY, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_FORWD_COUNTRY, SmsInfo, tmpField, nLength);

	/*************************************************************/ 
	/****  Field 22 : POS Entry Mode                        ******/                          
	/*************************************************************/
	/*if ( GetTlvAutho ( AUTO_POS_DATA, tlvInfo, isoPosMode, &nLength) ==SUCCES )
	{
		IsoToSmsPosMode (isoPosMode, tmpField);
		AddSmsField ( SMS_POS_ENTRY_MODE, SmsInfo, tmpField, nLength);
	}*/
	
	/*Start AMER20211004 PLUTONL-3579*/
	if (memcmp(sPosCode, "13", 2) != 0)		/*F022 is not used in 0220 dispute response financial*/
	{	
		if(GetTlvPrivate(POS_ENTRY_MODE_TAG, &kTlvInfo, tmpField, &nLength) == SUCCES)
		{
			AddSmsField ( SMS_POS_ENTRY_MODE, SmsInfo, tmpField, nLength);
		}
	}
	/*End AMER20211004 PLUTONL-3579*/

	/*************************************************************/ 
	/****  Field 25 : POS Condition Code                    ******/                          
	/*************************************************************/
	/*if ( GetTlvAutho ( AUTO_FUNCTION_CODE, tlvInfo, tmpField, &nLength) ==SUCCES )
	{
		AddSmsField ( SMS_POS_CONDITION, SmsInfo, tmpField, nLength);
		memcpy (sPosCode, tmpField, 2);
	}*/
	
	/*Start AMER20211004 PLUTONL-3579*/
	if (strlen(sPosCode) > 0)
	{
		AddSmsField ( SMS_POS_CONDITION, SmsInfo, sPosCode, SMS_F025_LEN);
	}
	/*End AMER20211004 PLUTONL-3579*/

	/*************************************************************/ 
	/****  Field 28 : Transaction Fees Amount               ******/                          
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_RECONCILIATION_DATE, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_TRANS_FEE, SmsInfo, tmpField, nLength);

	/*************************************************************/ 
	/****  Field 32 : Acquiring Institution Id Code         ******/                          
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_ACQ_INST_IDENT_CODE, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_ACQR_ID, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 33 : Forwarding Institution Id Code        ******/ 
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_FOR_INST_IDENT_CODE, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_FORWD_ID, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 37 : Retrieval Reference Number            ******/ 
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_RRN, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_REFERENCE_NBR, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 41 : Card Acceptor Terminal Id             ******/ 
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_CARD_ACC_TERMINAL, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_TERMINAL_NBR, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 42 : Card Acceptor Id Code                 ******/ 
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_CARD_ACC_ID, tlvInfo, tmpField, &nLength) ==SUCCES )
	{
		GetAbrevBankName (nIndiceCtx, AcquirerBank, sAbrevBankName);
		AddSmsField ( SMS_OUTLET_NBR, SmsInfo, sAbrevBankName, nLength);
	}

	/*************************************************************/
	/****  Field 43 : Card Acceptor Name/Location           ******/ 
	/*************************************************************/
	memset (tmpField1,       ' ', 64);
	if ( GetTlvAutho ( AUTO_CARD_ACC_LOCATION, tlvInfo, tmpField, &nLength) ==SUCCES )
	{
		memcpy (tmpField1, tmpField, 24);
		memcpy (tmpField1 + 25, BankLocation, strlen(BankLocation));
		memcpy (tmpField1 + 38, AlphaCode2, 2);
		AddSmsField (SMS_TERMINAL_ADR, SmsInfo, tmpField1, 40);
	}

	/*************************************************************/
	/****  Field 48 : Additional Data Private               ******/ 
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_UF_FILE_RECORD, tlvInfo, tmpField, &nLength) ==SUCCES ) /*ICH17012018*/
	/*if ( GetTlvAutho ( AUTO_ADD_DATA_PRIVATE, tlvInfo, tmpField, &nLength) ==SUCCES )*/
		AddSmsField ( SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 49 : Transaction Currency                  ******/              
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_TRAN_CURRENCY_CODE, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_TRANS_CRNCY, SmsInfo, tmpField, nLength);



	/*************************************************************/
	/****  Emv Data      55                                   ******/
	/*************************************************************/
	/*{
		TSIsoInfo   IsoInfo;
		char        IsoEmvData  [BUF_LG_MAX];
		int	        nLenEmvData;

		memset ( tmpField,     0 , sizeof ( tmpField));
		memset ( IsoEmvData,     0 , sizeof ( IsoEmvData));

		if ( GetTlvAutho ( AUTO_ICC_DATA, tlvInfo, tmpField, &nLength) ==SUCCES )
		{
			InitIsoInfo(&IsoInfo);
			IccTlvToIso(tmpField,  nLength,IsoEmvData, &nLenEmvData);
			PutIsoField ( ISO_ICC_DATA , &IsoInfo , IsoEmvData , nLenEmvData );

			if (GetIsoField (ISO_ICC_DATA, &IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
				AddSmsField (SMS_ICC_DATA, SmsInfo, tmpField, tmpFieldLen);

		}
	}*/
	

	/*************************************************************/
	/****  Field 60 : Additional POS Information            ******/              
	/*************************************************************/
	if ( !memcmp (sPosCode, "13", 2) )
		AddSmsField ( SMS_ADTNL_POS_INFO, SmsInfo, "22", 2);
	else
	{
		if (!memcmp (sProcCode, "02", 2) || !memcmp (sProcCode, "22", 2))
			AddSmsField ( SMS_ADTNL_POS_INFO, SmsInfo, "00", 2);
	}

	/*************************************************************/
	/****  Field 62 : CPS Fields                            ******/              
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_CPS_DATA, tlvInfo, tmpField, &nLength) ==SUCCES )
	{
		InitCpsInfo (&CpsInfo);

		InitTlvPrivate (&TlvInfoTmp);
		AnalyseTlvPrivate (tmpField, nLength, &TlvInfoTmp);
		if (GetTlvPrivate (CPS_ACI_TAG,&TlvInfoTmp,tmpField,&nLength) == SUCCESS)
			AddCpsSubField (CPS_ACI, &CpsInfo, tmpField, 1);

		if (GetTlvPrivate (CPS_TRANS_ID_TAG, &TlvInfoTmp, tmpField, &nLength)  == SUCCESS)
			AddCpsSubField (CPS_TRANS_ID, &CpsInfo, tmpField, 15);
		tmpFieldLen = CpsBuildField (tmpField, &CpsInfo);

		AddSmsField ( SMS_CPS_FIELDS, SmsInfo, tmpField, tmpFieldLen);
	}

	/*************************************************************/
	/****  Field 63 : Private Use Fields                    ******/              
	/*************************************************************/
	memset ( tmpField,     0 , sizeof ( tmpField));
	InitPufInfo (&PufInfo);

	if ( GetTlvAutho ( AUTO_NETWORK_DATA, tlvInfo, NwData, &nLength) ==SUCCES )
	{
		InitTlvPrivate (&TlvInfoTmp);
		AnalyseTlvPrivate (NwData, nLength, &TlvInfoTmp);

		GetTlvPrivate (NET_NETWORK_ID_TAG, &TlvInfoTmp, tmpField ,  &tmpFieldLen);


		if (tmpFieldLen == 4)
			AddPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, 4);
		else
			AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0002", 4);
	}
	else
		AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0002", 4);
            

	/*if ( GetTlvAutho ( AUTO_MESSAGE_REASON_CODE, tlvInfo, tmpField, &nLength) ==SUCCES )*/
	/*AMER20211004 PLUTONL-3579*/
	if(GetTlvPrivate (NET_MSG_REASON_CODE_TAG, &TlvInfoTmp, tmpField ,  &tmpFieldLen) == SUCCESS)
		AddPufSubField (PUF_MSG_REASON, &PufInfo, tmpField, 4);

	if ( GetTlvAutho ( AUTO_FPI, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddPufSubField (PUF_FPI, &PufInfo, tmpField, 3);

	tmpFieldLen = PufBuildField (tmpField, &PufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, tmpFieldLen);

	/*************************************************************/
	/****  Field 68 : Receiving Inst Country Code           ******/
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_RECEIV_INST_COUNTRY, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_RECEIVER_COUNTRY, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 70 : Network Mngmt Info Code               ******/
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_AUTH_INST_CODE, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_NW_MNGMT_INFO, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Field 100 : Receiving Inst Id Code               ******/
	/*************************************************************/
	if ( GetTlvAutho ( AUTO_RECEIV_INST_ID, tlvInfo, tmpField, &nLength) ==SUCCES )
		AddSmsField ( SMS_RECEIVER_ID, SmsInfo, tmpField, nLength);

	/*************************************************************/
	/****  Emv Data                                         ******/
	/*************************************************************/
	/*{
		TSIsoInfo  IsoInfo;
		char        IsoEmvData  [512];
		int	       nLenEmvData;

		memset ( tmpField,     0 , sizeof ( tmpField));
		memset ( IsoEmvData,     0 , sizeof ( IsoEmvData));

		if ( GetTlvAutho ( AUTO_ICC_DATA, tlvInfo, tmpField, &nLength) ==SUCCES )
		{
			InitIsoInfo(&IsoInfo);
			IccTlvToIso(tmpField,  nLength,IsoEmvData, &nLenEmvData);
			PutIsoField ( ISO_ICC_DATA , &IsoInfo , IsoEmvData , nLenEmvData );

			if (GetIsoField (ISO_ICC_DATA, &IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
				AddSmsField (SMS_ICC_DATA, SmsInfo, tmpField, tmpFieldLen);

		}
	}*/

	trace_event ( "End   TlvToSms ()", PROCESSING );
	return(OK);
}

