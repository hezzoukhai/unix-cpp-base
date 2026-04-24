#include <stdio.h>
#include <string.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_cps.h>
#include <iso_visa.h>
#include <visa_param.h>
#include <tlv_private.h>
#include <security_data.h>
#include <list_thr.h>
#include <queue.h>
#include <msg_flow_mapping.h>
#include <tlv_fields.h>
#include <iso8583.h>
#include <nw_raw_msg.h>

/****************************************************************************/
/*  FCT_AUT_REQ_102_FROM_NW()                                                   */
/*                                                                          */
/*  Processes a reversal request (0400) received from VISA.                 */
/****************************************************************************/
int  FCT_AUT_REQ_102_FROM_NW (int  nIndexCtx, TSVisaInfo  *VisaInfo)
{
	TSVisaInfo  NewVisaInfo;
	TSIsoInfo   IsoInfo;
	int         result;
	int         Length;
	char        sBuffer[LG_MAX];
	char        sPid[ 8 + 1];
	char		sTlvBuffer[LG_MAX];


	trace_event ("Start  FCT_AUT_REQ_102_FROM_NW()", PROCESSING);

	memset (sBuffer, 0, sizeof(sBuffer));

	/* Convert message to ISO format */
	result = AtmConfReqFromNw (nIndexCtx, VisaInfo, &IsoInfo);

	IsoInfo.msgId = MsgIsoReqReceived(ResourceId,&IsoInfo);

#ifdef V2_SERVICES
	result = ReversalProcessing (nIndexCtx, ResourceId, "ADM" , &IsoInfo , "N" );
#else
	result = ServiceProcessing(		nIndexCtx,
									ResourceId,
									"ADM",
									AcquirerBank,
									"Y",
									"G",
									"N",
									sTlvBuffer, 
									&IsoInfo);
#endif

	/*Start IBO20200517 RAWMSG*/
	if (g_sCaptureMsg[NW_INC_REQ] == 'Y')
	{
		LogVisaMessage(nIndexCtx, VisaInfo, &IsoInfo, INC_REQ);
	}
	/*End IBO20200517 RAWMSG*/

	switch (result)
	{

	case NOK: /* the interface itself responds */
	case  OK: /* the interface itself responds */ /* No Response To Be Sent For 102 Messages */
		break;
	
	case SEVERE_ERROR : /*shutdown_resource( nIndexCtx , ResourceId );/*PLUTONL-4242 ZKO  27012022*/
		break;
	} 

	MsgIsDone(IsoInfo.msgId);

	trace_event ("End  FCT_AUT_REQ_102_FROM_NW(OK)", PROCESSING);
	return (OK);
}





/****************************************************************************/
/*   AtmConfReqFromNw()                                                     */
/*                                                                          */
/*   Converts a received reversal request message (0400) from BASEI         */
/*   into an ISO-format message for processing.                             */
/*   Input:  VisaInfo: received message                                     */
/*   Output: IsoInfo:  generated ISO message                                */
/****************************************************************************/
int  AtmConfReqFromNw ( int         nIndexCtx,
						TSVisaInfo  *VisaInfo, 
						TSIsoInfo   *IsoInfo)
{
	TSTlvPrivate   TlvInfo;
	char           tmpField[256];
	char           tmpField1[256];
	char           tmpField2[256];
	char           MerchantType[5];
	char           RespCode[3];
	char           ProcCode[7];
	char           PosMode[7];
	char           PosCond[3];
	char           AddPos[11];
	char	          sXmitDateTime[ 10 + 1 ];
	char	          sStan        [  6 + 1 ];
	char	          sAcquirerId  [ 11 + 1 ];
	char	          sAcquirerIdLen [ 2 + 1 ];
	int 	          nAcquirerIdLen;
	char	          IsoOrigData   [ 35 + 1 ];
	char	          VisaOrigData  [ 42 + 1 ];
	char	          sMti          [  4 + 1 ];     
	int            Offset;
	char                sPid[ 8 + 1];
	int            tmpFieldLen;
	char			sEcomAuthInfo[P7_F048_P66_LEN + 1];
	int				nEcomInfoLen = 0;


	trace_event ("Start  AtmConfReqFromNw()", PROCESSING);

	memset (RespCode,  0, sizeof(RespCode));
	memset(sEcomAuthInfo, '\0', sizeof(sEcomAuthInfo));
	memset(sEcomAuthInfo, ' ', P7_F048_P66_LEN);


	InitIsoInfo (IsoInfo);
	IsoInfo->nMsgType = 1420;

	/* Field 2: Primary Account Number */
	if (GetVisaField(VISA_CARD_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_CARD_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 3: Processing Code */
	GetVisaField (VISA_PROC_CODE, VisaInfo, ProcCode, &tmpFieldLen);
	if ( GetVisaField (VISA_MERCHANT_TYPE, VisaInfo, MerchantType, &tmpFieldLen) != SUCCESS)
		memcpy(MerchantType, "6011", 4);

	VisaToIsoProcessingCode (ProcCode, MerchantType, tmpField);
	AddIsoField (ISO_PROC_CODE, IsoInfo, tmpField, 6);

	/* Field 4: Transaction Amount */
	if (GetVisaField (VISA_TRANS_AMOUNT , VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField  (ISO_TRANS_AMOUNT, IsoInfo, tmpField, 12);

	/* Field 6: Cardholder Billing Amount */
	if ( GetVisaField ( VISA_BILLING_AMOUNT , VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_BILLING_AMOUNT , IsoInfo, tmpField, 12);


	/* Field 7: Transmission Date and Time */
	if ( GetVisaField (VISA_XMIT_TIME, VisaInfo, sXmitDateTime, &tmpFieldLen) == SUCCES )
		AddIsoField  (ISO_XMIT_TIME, IsoInfo, sXmitDateTime , 10);

	/* Field 10: Cardholder Billing Conversion Rate */
	if (GetVisaField (VISA_BILLING_CNVRSN_RATE, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_BILLING_CNVRSN_RATE, IsoInfo, tmpField, 8);

	/* Field 11: System Trace Audit Number */
	GetInternalStan ( nIndexCtx , sStan );
	AddIsoField  (  ISO_AUDIT_NBR,  IsoInfo, sStan, 6 );

	/* Field 14: Expiry Date */
	if (GetVisaField (VISA_EXPIRY_DATE, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_EXPIRY_DATE, IsoInfo, tmpField, 4);

	/* Field 18: Merchant Type */
	AddIsoField (ISO_MERCHANT_TYPE, IsoInfo, MerchantType, 4);

	/* Field 19: Acquiring Institution Country Code */
	GetVisaField (VISA_ACQR_COUNTRY, VisaInfo, tmpField, &tmpFieldLen);
	AddIsoField  (ISO_ACQR_COUNTRY, IsoInfo, tmpField, 3);

	/* Field 20: PAN Country Code */
	if (GetVisaField (VISA_PAN_COUNTRY, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_PAN_COUNTRY, IsoInfo, tmpField, 3);

	/* Field 22: POS Entry Mode */
	memset ( PosMode,  0 , sizeof ( PosMode ));
	memset ( PosCond,  0 , sizeof ( PosCond ));
	memset ( AddPos ,  0 , sizeof ( AddPos  ));

	if (
		( GetVisaField (VISA_POS_ENTRY_MODE, VisaInfo, PosMode, &tmpFieldLen)                 == SUCCESS) &&
		( GetVisaField (VISA_POS_CONDITION, VisaInfo , PosCond, &tmpFieldLen)                == SUCCESS)  &&
		( GetVisaField (VISA_ADTNL_POS_INFO, VisaInfo, AddPos, &tmpFieldLen)                 == SUCCESS)
		)
	{

		VisaToIsoPosMode (MerchantType, PosMode, PosCond, AddPos, tmpField, sEcomAuthInfo, &nEcomInfoLen); /*IBO20221025 PLUTONL-5272*/
		AddIsoField (ISO_POS_DATA, IsoInfo, tmpField, 12);
	}

	/* Field 24: Function Code */
	AddIsoField (ISO_FUNCTION_CODE, IsoInfo, "402", 3);

	/* Field 25: Message Reason, 4001 = Unspecified */
	AddIsoField (ISO_MSG_REASON , IsoInfo, "4001", 4);

	/* Field 30: replacement trx amount + replacement billing amount */ 
	/* in case of a partial reversal */
	memset (tmpField,  0 , sizeof ( tmpField ));
	if ( GetVisaField ( VISA_OTHER_AMOUNTS , VisaInfo, tmpField , &tmpFieldLen) == SUCCES )
		AddIsoField (ISO_ORIGINAL_AMOUNTS, IsoInfo, tmpField, 24);

	/* Field 32: Acquiring Institution Identification Code */
	if ( GetVisaField (VISA_ACQR_ID, VisaInfo, sAcquirerId, &nAcquirerIdLen) == SUCCES )
		AddIsoField  ( ISO_ACQR_ID,  IsoInfo, sAcquirerId, nAcquirerIdLen);

	/* Field 33: Forwarding Institution Id */
	if (GetVisaField (VISA_FORWD_ID, VisaInfo,tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField (ISO_FORWD_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 37: Retrieval Reference Number */
	GetVisaField (VISA_REFERENCE_NBR, VisaInfo, tmpField, &tmpFieldLen);
	AddIsoField  (ISO_REFERENCE_NBR, IsoInfo, tmpField, 12);

	/* Field 38: Authorization Id */
	GetVisaField (VISA_AUTHOR_ID, VisaInfo, tmpField, &tmpFieldLen);
	AddIsoField  (ISO_AUTHOR_ID, IsoInfo, tmpField, 6);

	/* Field 39: Response Code */
	if (GetVisaField (VISA_RESPONSE_CODE, VisaInfo, RespCode, &tmpFieldLen) == SUCCESS)
	{
		TranslateRespCode (nIndexCtx, ResourceId, RespCode, "RP", tmpField);  
		AddIsoField (ISO_RESPONSE_CODE, IsoInfo, tmpField, 3);
	}

	/* Field 40: Service Restriction Code */
	if (GetVisaField (VISA_SERVICE_CODE, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_SERVICE_CODE, IsoInfo, tmpField, 3);

	/* Field 41: Terminal Identification */
	if (GetVisaField (VISA_TERMINAL_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_TERMINAL_NBR, IsoInfo, tmpField, 8);

	/* Field 42: Card Acceptor Identification Code */
	if (GetVisaField (VISA_OUTLET_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_OUTLET_NBR, IsoInfo, tmpField, 15);

	/* Field 43: Card Acceptor Location */
	if (GetVisaField (VISA_TERMINAL_ADR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_TERMINAL_ADR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 48: Additional Private Data */
	InitTlvPrivate (&TlvInfo);
	AddTlvPrivate (HPS_ACQUIRER_BANK_TAG , &TlvInfo, AcquirerBank, 6);

	if( GetVisaField (VISA_AUDIT_NBR, VisaInfo,tmpField,&tmpFieldLen) == SUCCES )
		AddTlvPrivate (EXTERNAL_STAN_TAG , &TlvInfo, tmpField, 6);

	if (GetVisaField (VISA_ADTNL_DATA_PRIV, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		/* skip the asterisk (*) */
		AddTlvPrivate (MESSAGE_TEXT_TAG, &TlvInfo, tmpField + 1, tmpFieldLen - 1);
	}
	if (nEcomInfoLen > 0)	/*IBO20221025 PLUTONL-5272*/
	{
		AddTlvPrivate(ECOM_AUTH_INFO, &TlvInfo, sEcomAuthInfo, nEcomInfoLen);
	}
	tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
	if (tmpFieldLen > 0)
		AddIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, tmpFieldLen);

	/* Field 49: Transaction Currency Code */
	GetVisaField (VISA_TRANS_CRNCY, VisaInfo, tmpField, &tmpFieldLen);
	AddIsoField  (ISO_TRANS_CRNCY, IsoInfo, tmpField, 3);

	/* Field 51: Cardholder Billing Currency Code */
	if (GetVisaField (VISA_BILLING_CRNCY, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_BILLING_CRNCY, IsoInfo, tmpField, tmpFieldLen);



	/* Field 53: Security Data */
	memset ( tmpField1 , 0 , sizeof ( tmpField1 ));
	PutSecurityData(SECURITY_FORMAT  , tmpField1 , "00" , 2);
	PutSecurityData(PIN_BLOCK_FORMAT , tmpField1 , "99" , 2);
	PutSecurityData(PIN_ENCRYPTION_KEY_INDEX , tmpField1 , "000" , 3);
	PutSecurityData(MAC_KEY_INDEX , tmpField1 , "000" , 3);
	sprintf (sPid , "%08d", getpid());
	PutSecurityData(SOURCE_PID , tmpField1 , sPid  , 8);
	PutSecurityData(SOURCE_RESOURCE , tmpField1 , ResourceId  , 6);
	PutSecurityData(DESTINATION_PID , tmpField1 , "00000000"  , 8);
	PutSecurityData(DESTINATION_RESOURCE , tmpField1 , "000000"  , 6);
	PutSecurityData(USER , tmpField1 , "VISA           "  , 15);
	PutSecurityData(PURGE_TIME , tmpField1 , "000000000"  , 9);
	AddIsoField (ISO_SECURITY_DATA, IsoInfo, tmpField1, strlen(tmpField1));


	/* Field 56: Original Data */
	memset ( IsoOrigData , 0 , sizeof ( IsoOrigData ));
	memset ( IsoOrigData , '0' , 35 );

	Offset = 0;
	/* Original Type de Message */
	memset  ( sMti , 0 			 , sizeof ( sMti ));
	memcpy  ( sMti , "0100" , 4);
	memcpy  ( IsoOrigData + Offset , sMti, 4); Offset += 4 ;

	/* Original Stan */
	if( GetVisaField (VISA_AUDIT_NBR, VisaInfo,tmpField,&tmpFieldLen) == SUCCES )
		memcpy  ( IsoOrigData  + Offset , tmpField , 6);
	else
		memcpy  ( IsoOrigData  + Offset , sStan    , 6); 

	Offset += 6;


	/* Original Xmit */
	memcpy  ( IsoOrigData + Offset , "00" , 2 ); Offset += 2;

	memcpy  ( IsoOrigData + Offset , sXmitDateTime  , 10 ); Offset += 10;

	/* Original Acquirer Id */
	sprintf ( sAcquirerIdLen , "%02d" , nAcquirerIdLen );
	memcpy  ( IsoOrigData + Offset , sAcquirerIdLen, 2 ); Offset += 2;
	memcpy  ( IsoOrigData + Offset + 11 - nAcquirerIdLen , sAcquirerId , nAcquirerIdLen );
	AddIsoField ( ISO_ORIG_DATA, IsoInfo, IsoOrigData , 35);

	/* Field 61: Network POS Data, in TLV format */
	InitTlvPrivate (&TlvInfo);

	/* EBETODO: Why are they commented?
	AddTlvPrivate (POS_ENTRY_MODE_TAG, &TlvInfo, PosMode, 4);
	AddTlvPrivate (POS_CONDITION_CODE_TAG, &TlvInfo, PosCond, 2);
	*/

	if (strlen(RespCode) > 0)
		AddTlvPrivate (RESPONSE_CODE_TAG, &TlvInfo, RespCode, 2);
	/* AddTlvPrivate (ADTNL_POS_DATA_TAG, &TlvInfo, AddPos, strlen(AddPos)); */
	tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
	AddIsoField (ISO_NET_POS_DATA, IsoInfo, tmpField, tmpFieldLen);

	/* Field 62: CPS Data, in TLV format */
	if (GetVisaField (VISA_CPS_FIELDS,VisaInfo,tmpField,&tmpFieldLen) == SUCCESS)
	{
		InitTlvPrivate (&TlvInfo);
		AddTlvPrivate (CPS_ACI_TAG, &TlvInfo, tmpField, 1);
		AddTlvPrivate (CPS_TRANS_ID_TAG, &TlvInfo, tmpField + 1, 15);
		tmpFieldLen = PrivateBuildTlv (tmpField, &TlvInfo);
		AddIsoField (ISO_CPS_FIELDS, IsoInfo, tmpField, tmpFieldLen);
	}

	dump_p7_struct(IsoInfo);
	trace_event ("End  AtmConfReqFromNw(OK)", PROCESSING);
	return (OK);
}
