#include <stdio.h>
#include <define.h>
#include <struct_time.h>
#include <stdio.h>
#include <queue.h>
#include <iso_hps.h>
#include <iso_fields.h>
#include <iso_sms.h>
#include <sms_fields.h>
#include <tlv_autho.h>
#include <tlv_fields.h>
#include <sms_param.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <p7_sid.h>
#include <security_data.h>
/**************************************************************************************/
int AcquirerTimeProcMsg( int nIndiceCtx, TSTimeExchange * sTimeMsg )
{
	TSIsoInfo       IsoInfo;
	TSIsoInfo       IsoRevInfo;
	char            sTlvBuffer          [ LG_MAX + 1 ];
	char            tmpField            [ LG_MAX ];
	TSTlvAutho      sTlvInfo;
	char            sLine               [MAX_LINE_TRC];
	int             nLength;
	int             tmpFieldLen;
	int             nEventCode   =  OK;
	int             nRetCode;
    TSTlvPrivate    PrivateTlvInfo;		/*AMER 20160818*/
	char            sProcCode[12 + 1];

	trace_event ( "Start AcquirerTimeProcMsg()" , PROCESSING );

	if ( sTimeMsg->sData.nFstPrvtDataLen != 0 )
	{
		trace_event ( "Private Data > 0" , PROCESSING );

		if ( memcmp ( sTimeMsg->sData.sFstPrvtData , "SAF",  sTimeMsg->sData.nFstPrvtDataLen ) == 0 )
		{
			trace_event ( "SAF Time Out Processing " , PROCESSING );
			SafTimeOutTraitement ( nIndiceCtx , sTimeMsg->sData.sSndBuf);
			return ( OK );
		}
		/*HAL26082020 PLUTONL-2185*/ 
		if ( memcmp ( sTimeMsg->sData.sFstPrvtData , "PERIODIQUE",  sTimeMsg->sData.nFstPrvtDataLen ) == 0 )
       	{
           	TimeOutSignOnResponseDetected ( nIndiceCtx , sTimeMsg->sData ); 
           	return ( OK );
      	}	
	   	/*HAL26082020 PLUTONL-2185*/ 
	}
	InitIsoInfo (&IsoInfo);
	InitIsoInfo (&IsoRevInfo);
	DumpIso(sTimeMsg->sData.sFstBuf);
	AnalyseIso  (sTimeMsg->sData.sFstBuf, &IsoInfo);
	dump_p7_struct(&IsoInfo);
	switch ( IsoInfo.nMsgType )
	{
	case 1200 :
		memset(sProcCode, 0, sizeof(sProcCode));
		GetIsoField(ISO_PROC_CODE, &IsoInfo, sProcCode, &tmpFieldLen);

		sprintf(sLine, "==> sProcCode=[%s]", sProcCode);
		trace_event(sLine, FATAL);

		IsoInfo.nMsgType = IsoInfo.nMsgType+10; /*NAB09022021 PLUTONL-2703*/
		if ( memcmp ( ReversalGeneration , "T" , 1 ) == 0 && memcmp(sProcCode, P7_F003_BAL_INQ, P7_F003_P1_LEN) != 0)
		{
			InitTlvAutho(&sTlvInfo);
			memset( sTlvBuffer   ,  0, sizeof ( sTlvBuffer ));
			BuildTimeOutReversalMsg ( &IsoInfo , &IsoRevInfo );
			IsoToTlv ( nIndiceCtx , &IsoRevInfo , &sTlvInfo );
			PutTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, ResourceId ,     6);
			PutTlvAutho ( AUTO_ROUTING_CODE        , &sTlvInfo, ResourceId ,     6);
			PutTlvAutho ( AUTO_MESSAGE_REASON_CODE , &sTlvInfo, "4200" ,         4);
			PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, "ATM" ,          3);



			/*AMER 20160818 Start:PROD00030831: the acquirer and issuer bank are needed to be filled in the SAF table*/
			InitTlvPrivate(&PrivateTlvInfo);
			if(GetIsoField(ISO_ADTNL_DATA_PRIV,&IsoInfo,tmpField,&tmpFieldLen)==SUCCESS)
			{
					AnalyseTlvPrivate(tmpField,tmpFieldLen,&PrivateTlvInfo);
				if( GetTlvPrivate(ACQUIRER_BANK_TAG,&PrivateTlvInfo,tmpField,&tmpFieldLen) == SUCCESS )
						PutTlvAutho(AUTO_ACQUIRER_BANK, &sTlvInfo, tmpField, tmpFieldLen);

					if( GetTlvPrivate(ISSUER_BANK,&PrivateTlvInfo,tmpField,&tmpFieldLen) == SUCCESS )
						PutTlvAutho(AUTO_ISSUING_BANK, &sTlvInfo, tmpField, tmpFieldLen);
			}
				/*AMER 20160818 End*/ /* BURGAN ONSITE UAT - CERTIF VISA - YHM20171030 */ 
				AuthoBuildTlv ( sTlvBuffer, &sTlvInfo);
				PrintTlvBuffer(sTlvBuffer);

			InsertionSaf ( nIndiceCtx, sTlvBuffer, ResourceId , ResourceId);
			ReplyIsoMsgToBal(nBalMsg,&IsoInfo,"",ISSUER_UNAVAILABLE_ACTION);
		}
		break;
	}

	MsgTimeout(sTimeMsg->sData.msgId, ResourceId);
	trace_event ( "End   AcquirerTimeProcMsg()" , PROCESSING );
	return ( OK );
}
/**************************************************************************************/
int IssuerTimeProcMsg( int nIndiceCtx, TSTimeExchange * sTimeMsg )
{
	char            sLine               [MAX_LINE_TRC];
	int             nLength;
	TSSmsInfo      SmsInfo;
	TSIsoInfo       IsoInfo;
	int             nRetCode;
	char            sTlvBuffer[LG_MAX];
	char       		sResource[ 6 + 1 ]; /* ELKAM PLUTONL-3516 */
	char			sStan[6 + 1];
	TSTlvAutho      sTlvInfo;


	trace_event ( "Start IssuerTimeProcMsg()" , PROCESSING );

	InitIsoInfo(&IsoInfo);
	InitSmsInfo(&SmsInfo);
	AnalyseSms(sTimeMsg->sData.sFstBuf, &SmsInfo);

	memset(sResource , 0 , sizeof ( sResource )); /* ELKAM PLUTONL-3516 */
	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	memset(sStan, 0, sizeof(sStan));
	memcpy(sTlvBuffer, sTimeMsg->sData.sSndBuf, sTimeMsg->sData.nSndBufLen);

	PrintTlvBuffer(sTlvBuffer);

	IsoInfo.nMsgType = 1200;
	BuildAutReqFromNw(nIndiceCtx, &SmsInfo, &IsoInfo);

	switch (IsoInfo.nMsgType) {
	case 1200:	
		nRetCode = OnTimeoutIsoServiceRequest(nIndiceCtx,sTimeMsg->sens,&IsoInfo,sTlvBuffer);
		if( nRetCode == ROUTAGE )
		{
			trace_event(" ROUTAGE Returned ", PROCESSING);
			/* put back our original ISO Stan nbr */
			GetTlvBuffer ( AUTO_INTERNAL_STAN , sTlvBuffer , sStan ,&nLength);
			PutIsoField(ISO_AUDIT_NBR, &IsoInfo, sStan, nLength);
			if (ForwardAutRequest(nIndiceCtx, &IsoInfo, sTlvBuffer, &SmsInfo) != OK) 
			{
				/* respond to VISA that issuer was not available */
				PutIsoField(ISO_RESPONSE_CODE, &IsoInfo, ISSUER_UNAVAILABLE_ACTION, 3);
				nRetCode = NOK;
			} 
			else 
			{	
				/* request was successfully passed on */
				trace_event("End  IssuerTimeProcMsg(OK)", PROCESSING);
				return (OK);
			}
		}
		else
		{
			InitTlvAutho ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso    ( nIndiceCtx  ,&sTlvInfo , &IsoInfo  , RESPONSE );
		}
		switch (nRetCode) {
		case OK:	/* the interface itself replies to the
				* network */
		case NOK:	/* the interface itself replies to the
				* network */
			trace_event(" Retour AuthRequProcessing :  NOK/OK ", PROCESSING);
			if (AutReplyToNw(nIndiceCtx, &IsoInfo, &SmsInfo) == OK) 
			{
				/* ELKAM PLUTONL-3516 START */
				GetTlvBuffer ( AUTO_ROUTING_CODE , sTlvBuffer, sResource , &nLength);
				AutomaticIsoReversalProc(nIndiceCtx , NORMAL_MODE , ResourceId , sResource , "ADM" , &IsoInfo , P7_F025_RESP_RECV_LATE );
				/* ELKAM PLUTONL-3516 END */

				trace_event("End  FCT_AUT_REQ_FROM_NW(OK)", PROCESSING);
				return (OK);
			}
			else 
			{
				trace_event("End  FCT_AUT_REQ_FROM_NW(NOK)", PROCESSING);
				return (NOK);
			}
			break;
		case SEVERE_ERROR:	/* shutdown interface */
			trace_event(" Retour AuthRequProcessing :  SEVERE_ERROR ", PROCESSING);
			AutReplyToNw(nIndiceCtx, &IsoInfo, &SmsInfo);
			shutdown_resource(nIndiceCtx, ResourceId);
			trace_event("End  FCT_AUT_REQ_FROM_NW(SEVERE_ERROR)", PROCESSING);
			return (NOK);
			break; 
		}
	}

	if ( sTimeMsg->sData.nFstPrvtDataLen != 0 )
	{
		trace_event ( "Private Data > 0" , PROCESSING );

		if ( memcmp ( sTimeMsg->sData.sFstPrvtData , "OUT",  sTimeMsg->sData.nFstPrvtDataLen ) == 0 )
		{
			trace_event ( "OUTGOING Time Out Processing " , PROCESSING );
			OutgoingTimeOutTraitement ( nIndiceCtx , sTimeMsg->sData.sSndBuf);
			return ( OK );
		}
	}

	trace_event ( "End   IssuerTimeProcMsg()" , PROCESSING );
	return (OK);
}

/***************************************************************************************/
void   TimeProcMsg(int nIndiceCtx, TSTimeExchange *sTimeMsg)
{

	trace_event ( "Time Out Detected ....", PROCESSING );

	switch( sTimeMsg->sens)
	{
	case 'B' :
		AcquirerTimeProcMsg( nIndiceCtx, sTimeMsg);
		break;

	case 'L' :
		IssuerTimeProcMsg( nIndiceCtx, sTimeMsg);
		break;

	case 'S' :
		FCT_SHUTDOWN_TIME( nIndiceCtx );
		break;

	default :
		break;
	}
}

/************************************************************************************/
int BuildTimeOutReversalMsg ( TSIsoInfo *IsoInfo , TSIsoInfo * IsoRevInfo )
{
    TSTlvPrivate    TlvInfo;
	char          OrigDataElements [ LG_MAX ];
	char          tmpField         [ LG_MAX ];
	int           tmpFieldLen;
	int           nPos;
    char            exStan[6 + 1];              /*NAB05092018*/
	dump_p7_struct(IsoInfo);

	IsoRevInfo->nMsgType = 1420;
	/*START NAB05092018*/
	InitTlvPrivate(&TlvInfo);
	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		AnalyseTlvPrivate(tmpField, tmpFieldLen, &TlvInfo);
	}
	/*END NAB05092018*/

	/*---------------------------------------------------------------------------------*/
	/* (2) Primary Account Number */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField (ISO_CARD_NBR, IsoInfo   , tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField  (ISO_CARD_NBR, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (3) Processing Code */
	/*---------------------------------------------------------------------------------*/
	if ( GetIsoField (ISO_PROC_CODE, IsoInfo,    tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField (ISO_PROC_CODE, IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (4): Transaction Amount */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField (ISO_TRANS_AMOUNT, IsoInfo,tmpField,&tmpFieldLen) == SUCCES)
		AddIsoField (ISO_TRANS_AMOUNT , IsoRevInfo, tmpField, tmpFieldLen);

	/*---------------------------------------------------------------------------------*/
	/* (7) Transaction Date/Time */
	/*---------------------------------------------------------------------------------*/
	current_gmt_date (tmpField);
	AddIsoField (ISO_XMIT_TIME, IsoRevInfo, tmpField + 2, 10);

	/*---------------------------------------------------------------------------------*/
	/* (11) Trace Audit Number */
	/*---------------------------------------------------------------------------------*/
	if ( GetIsoField (ISO_AUDIT_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCES )
		AddIsoField (ISO_AUDIT_NBR , IsoRevInfo, tmpField, tmpFieldLen );


	/*---------------------------------------------------------------------------------*/
	/* (12) & (13) Local Transaction Time & Date */
	/*---------------------------------------------------------------------------------*/
	if ( GetIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField , &tmpFieldLen) == SUCCES )
		AddIsoField (ISO_TRANS_DATE_TIME , IsoRevInfo, tmpField, tmpFieldLen );
	
    /*NAB31032021 PLUTONL-2868*/
	/* Start LHOU11032021 HPSP-351*/
	/* (14) Expiry Date */
	/*---------------------------------------------------------------------------------*/
	if ( GetIsoField (ISO_EXPIRY_DATE, IsoInfo, tmpField , &tmpFieldLen) == SUCCES )
		AddIsoField (ISO_EXPIRY_DATE , IsoRevInfo, tmpField, tmpFieldLen );
	/* End LHOU11032021*/
	
	/*---------------------------------------------------------------------------------*/
	/* (18) Merchant Type                 */
	/*---------------------------------------------------------------------------------*/
	if ( GetIsoField (ISO_MERCHANT_TYPE, IsoInfo, tmpField , &tmpFieldLen) == SUCCES )
		AddIsoField (ISO_MERCHANT_TYPE , IsoRevInfo, tmpField, tmpFieldLen );

	/*---------------------------------------------------------------------------------*/
	/* (19) Acquiring Country Code        */
	/*---------------------------------------------------------------------------------*/
	if ( GetIsoField (ISO_ACQR_COUNTRY , IsoInfo, tmpField , &tmpFieldLen) == SUCCES )
		AddIsoField (ISO_ACQR_COUNTRY , IsoRevInfo, tmpField, tmpFieldLen );


	/*---------------------------------------------------------------------------------*/
	/* (20): PAN Country Code */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField (ISO_PAN_COUNTRY, IsoInfo,tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField (ISO_PAN_COUNTRY , IsoRevInfo, tmpField, tmpFieldLen );

	/*---------------------------------------------------------------------------------*/
	/* (22) POS Entry Mode */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField (ISO_POS_DATA, IsoInfo,tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField (ISO_POS_DATA , IsoRevInfo, tmpField, tmpFieldLen );

	/*---------------------------------------------------------------------------------*/
	/* (32) Acquiring Institution Id Code */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField (ISO_ACQR_ID, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField (ISO_ACQR_ID, IsoRevInfo, tmpField, tmpFieldLen );

	/*---------------------------------------------------------------------------------*/
	/* (33) Forwarding Institution Id */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField (ISO_FORWD_ID, IsoInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField (ISO_FORWD_ID, IsoRevInfo, tmpField, tmpFieldLen );
	else
		AddIsoField (ISO_FORWD_ID, IsoRevInfo, tmpField, tmpFieldLen ); /* BURGAN ONSITE UAT VISA CERTIF YHM20171030*/ /* BURGAN_21 */

	/*---------------------------------------------------------------------------------*/
	/* (37) Reference Number */
	/*---------------------------------------------------------------------------------*/
	GetIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_REFERENCE_NBR , IsoRevInfo, tmpField, tmpFieldLen );

	/*---------------------------------------------------------------------------------*/
	/* (38) Auth Code          */
	/*---------------------------------------------------------------------------------*/
	AddIsoField (ISO_AUTHOR_ID , IsoRevInfo, "A99999", 6); /* BURGAN ONSITE UAT - VISA CERTIF YHM20171031 - REMOVED AS WE DON'T RECEIVE A RESPONSE */
	/* BURGAN_22 */
	/*---------------------------------------------------------------------------------*/
	/* (39) Response Code Code */
	/*---------------------------------------------------------------------------------*/
	AddIsoField (ISO_RESPONSE_CODE , IsoRevInfo, "911", 3);


	/*---------------------------------------------------------------------------------*/
	/* (41) Card Acceptor Terminal Id */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField (ISO_TERMINAL_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_TERMINAL_NBR , IsoRevInfo, tmpField, tmpFieldLen );

	/*---------------------------------------------------------------------------------*/
	/* (42) Card Acceptor Name/Location */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField (ISO_OUTLET_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_OUTLET_NBR , IsoRevInfo, tmpField, tmpFieldLen );

	/*---------------------------------------------------------------------------------*/
	/* (43) Card Acceptor Name/Location */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField (ISO_TERMINAL_ADR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_TERMINAL_ADR , IsoRevInfo, tmpField, tmpFieldLen );
	/*Start SNO070116 */
	/*---------------------------------------------------------------------------------*/
	/* (48) ADDITIONNAL DATA */
	/*---------------------------------------------------------------------------------*/
	if (GetIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField (ISO_ADTNL_DATA_PRIV , IsoRevInfo, tmpField, tmpFieldLen );
	/*End SNO070116 */		
	/*---------------------------------------------------------------------------------*/
	/* (49) Transaction Currency Code */
	/*---------------------------------------------------------------------------------*/
	GetIsoField (ISO_TRANS_CRNCY, IsoInfo, tmpField, &tmpFieldLen);
	AddIsoField (ISO_TRANS_CRNCY, IsoRevInfo, tmpField, 3);

	/*---------------------------------------------------------------------------------*/
	/* (90) Original Data Elements  */
	/*---------------------------------------------------------------------------------*/
	nPos = 0;
	memset ( OrigDataElements ,  0 , sizeof(OrigDataElements) );
	memcpy ( OrigDataElements +  nPos , "1200" , 4 ); nPos += 4;

	/*NAB05092018 getting external stan instead of internal stan PROD00060083*/
	 if (GetTlvPrivate( EXTERNAL_STAN_TAG , &TlvInfo,exStan, &tmpFieldLen) == SUCCES)
		{
		/*GetIsoField (ISO_AUDIT_NBR,   IsoInfo, tmpField     , &tmpFieldLen);
		memcpy ( OrigDataElements +  nPos , tmpField , 6);*/
		memcpy(OrigDataElements + nPos, exStan, 6);		
		}
	else{
		 memcpy ( OrigDataElements +  nPos , "000000" , 6);
		}	
	nPos += 6;
	
	memcpy ( OrigDataElements +  nPos , "00" , 2); nPos += 2;

	GetIsoField (ISO_XMIT_TIME,   IsoInfo, tmpField , &tmpFieldLen);
	memcpy ( OrigDataElements + nPos , tmpField , 10  ); nPos += 10;

	/* GetIsoField (ISO_ACQR_ID, IsoInfo, tmpField    , &tmpFieldLen); */
	sprintf ( OrigDataElements + nPos , "%02lu" , strlen(AcquirerId)  ); nPos += 2;
	sprintf ( OrigDataElements + nPos , "%011s",AcquirerId );     nPos += 11;
	memcpy(OrigDataElements + nPos, "0000000000000", 13); nPos += 13;


	AddIsoField (ISO_ORIG_DATA , IsoRevInfo, OrigDataElements , nPos);

	dump_p7_struct(IsoRevInfo);

	trace_event ("End  BuildTimeOutReversalMsg (OK)", PROCESSING);
	return (OK);
}




