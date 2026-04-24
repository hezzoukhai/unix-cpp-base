/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      auth_utils.c                                                           */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Utilities for autorisation : Local autorisation/ Stand In Autoris...*/
/*         Check data integrity of TLV data                                    */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   12/05/2002 -         V 2.0.1                              */
/*        Add a call of PutEvent during the check of the integrity of the TLV  */
/*        data see (HYJCC120502)                                               */
/*******************************************************************************/


#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <sys/time.h>
#include <dump.h>
#include <list_thr.h>
#include <hsm_inc.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_autho.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <resources.h>
#include <service.h>
#include <event_auth.h>
#include <maj_card_activity.h>
#include <p7_sid.h>

/*************************************************************************************/
/*************************************************************************************/


int CheckReqTlvFields ( int nIndiceCtx, char *sTlvBuffer )
{
	char sMcc        		[ 4 + 1 ];
	char sPCode      		[ 30 ];
	char sIsoPosData 		[ 12 + 1 ];
	char sTrackII 			[ 256 ];
	char sExpiryDate 		[ 4 + 1 ];
	char sTrackIIExpDate	[ 4 + 1 ];
	char sPan				[ 24 + 1 ];
	char sCvv1				[ 3 + 1 ];
	char sPvv				[ 4 + 1 ];
	char sServiceCode		[ 3 + 1 ];
	char sCardPan			[ 24 + 1 ];
	char sLine       		[ MAX_LINE_TRC ];
	char sPosCond			[12 + 1]; 
	char sPinData			[256 + 1];
	char sWork       		[256 + 1];
	char sRecurringFlag		[24];
	char sCardLessType		[24];
	char tmpField 			[BUF_LG_MAX];
	char slTransAmount 		[24];
	TSTlvPrivate    		sTlvPrivateInfo;
	TSTlvPrivate    		TlvInfo;

	int  nLength;

	trace_event("Start CheckReqTlvFields()",PROCESSING);


	memset (  sPCode      	, 0 , sizeof ( sPCode ));
	memset (  sMcc       	, 0 , sizeof ( sMcc ));
	memset (  sIsoPosData 	, 0 , sizeof ( sIsoPosData ));
	memset (  sTrackII 		, 0 , sizeof ( sTrackII ));
	memset (  sExpiryDate 	, 0 , sizeof ( sExpiryDate ));
	memset (  sTrackIIExpDate 	, 0 , sizeof ( sTrackIIExpDate ));
	memset (  sPan 		, 0 , sizeof ( sPan ));
	memset (  sCardPan 		, 0 , sizeof ( sCardPan ));
	memset (  sCvv1 		, 0 , sizeof ( sCvv1 ));
	memset (  sPvv 		, 0 , sizeof ( sPvv ));
	memset (  sServiceCode 	, 0 , sizeof ( sServiceCode ));
	memset ( sPosCond       , 0, sizeof(sPosCond));
	memset ( sRecurringFlag       , 0, sizeof(sRecurringFlag));
	memset ( sCardLessType       , 0, sizeof(sCardLessType));
	memset ( slTransAmount, 0, sizeof (slTransAmount));

	InitTlvPrivate(&TlvInfo);
	InitTlvPrivate(&sTlvPrivateInfo);


	/*AMER20171123 PROD00049633*/
	if (GetTlvBuffer(AUTO_ADD_DATA_PRIVATE, sTlvBuffer, tmpField, &nLength) == SUCCESS) 
	{
		AnalyseTlvPrivate(tmpField, nLength, &TlvInfo);
	}

	if ( GetTlvBuffer ( AUTO_TRACK2_DATA , sTlvBuffer , sTrackII , &nLength ) == SUCCES)
	{
		GetTrack2Data(sTrackII,sPan,sTrackIIExpDate,sServiceCode,sPvv,sCvv1);
	}

	GetTlvPrivate(RECURRING_FLAG_TAG, &TlvInfo, sRecurringFlag, &nLength);
	GetTlvPrivate(CARDLESS_TYPE, &TlvInfo, sCardLessType, &nLength);

	GetTlvBuffer ( AUTO_MERCHANT_TYPE , sTlvBuffer , sMcc , &nLength ); 
	GetTlvBuffer ( AUTO_POS_DATA      , sTlvBuffer , sIsoPosData , &nLength );
	GetTlvBuffer ( AUTO_PROCESSING_CODE      , sTlvBuffer , sPCode , &nLength ); 


	if ( GetTlvBuffer ( AUTO_PAN , sTlvBuffer , sCardPan , &nLength ) == SUCCES && 
		strlen(sPan) > 0 )
	{
		if (strcmp(sCardPan,sPan) != 0)
		{
			PutEvent( sTlvBuffer , INVALID_ISO2, "Card Number In TrackII Diff Than Field 2"); 
			PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_TRANSACTION_ACTION , 3);
			trace_event("Check PAN  == TRACK II PAN  (NOK)",TRACE);
			trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
			return (NOK);
		}
	}



	if ( GetTlvBuffer ( AUTO_EXPIRY_DATE , sTlvBuffer , sExpiryDate , &nLength ) == SUCCES &&
		strlen(sTrackIIExpDate) > 0 )
	{
		if (memcmp(sExpiryDate,sTrackIIExpDate,4) != 0)
		{
			PutEvent( sTlvBuffer, INVALID_ISO2, "Expiry Date In TrackII Diff Than Field 14");
			PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_TRANSACTION_ACTION , 3);
			trace_event("Check EXP DATE == TRACK II EXP DATE  (NOK)",TRACE);
			trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
			return (NOK);
		}

	}



	if ( GetTlvBuffer ( AUTO_NETWORK_POS_DATA      , sTlvBuffer , tmpField , &nLength ) == SUCCES)
	{
		AnalyseTlvPrivate ( tmpField , nLength , &sTlvPrivateInfo);

		if ( GetTlvPrivate(POS_CONDITION_CODE_TAG ,&sTlvPrivateInfo,sPosCond , &nLength ) == SUCCES )
		{
			if ( memcmp(sPosCond ,P7_POS_CONDCD_MER_SUSP, 2   ) == 0 )  /* Merchant Suspicious */
			{
				PutEvent( sTlvBuffer, INVALID_MERCHANT, "Merchant Suspicious");
				PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, DO_NOT_HONOR , 3);
				trace_event("Check  NETWORK POS DATA (NOK)",TRACE);
				trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
				return (NOK);
			}

			/*EBETODO: should be checked below with F022 instead of pos cond code
			*
			*/
			if ( memcmp(sPosCond , P7_POS_CONDCD_CH_NOT_PRESENT, 2   ) == 0 )  /* Customer Not Present */
			{
				/*AMER20171123 PROD00049633: Reject authorization just if RECURRING_FLAG_TAG is not present 
				* or its value is different from the values listed below*/
				if (	sRecurringFlag[0] != P7_F048_P71_RECURRING &&
					sRecurringFlag[0] != P7_F048_P71_INSTALLMENT &&
					sRecurringFlag[0] != P7_F048_P71_CRED_ON_FILE
					)	
				{
					PutEvent( sTlvBuffer, INVALID_MERCHANT, "Customer Not Present Suspicious");
					PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, REFER_TO_ISSUER_ACTION , 3);
					trace_event("Check  NETWORK POS DATA (NOK)",TRACE);
					trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
					return (NOK);
				}
			}
		}
	}


	/* Cardholder present And Card Not Present*/
	if ( sIsoPosData [ 4 ] == P7_F022_P5_CH_PRES && 
		sIsoPosData [ 5 ] == P7_F022_P6_CARD_NPRES  &&  
		sIsoPosData [ 6 ] != P7_F022_P7_MANUAL &&  
		sIsoPosData [ 6 ] != P7_F022_P7_PAN_NETRY && /*NAK 31012017: KEYED POS*/
		sIsoPosData [ 6 ] != P7_F022_P7_CRED_ON_FILE)  /*NAB 20181704 PROD00055584: customer initiated trx using credentials on file*/
	{
		sprintf(sLine,"Pos data %s", sIsoPosData);
		trace_event(sLine,TRACE);
		PutEvent( sTlvBuffer, INVALID_MERCHANT, "Pos Data Invalid:Cardholder present and Card Not present"); 
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, REFER_TO_ISSUER_ACTION , 3);
		trace_event("Check  POS DATA (NOK)",TRACE);
		trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
		return (NOK);

	}

	 /* Cardholder Authentication Method != PIN */
	if (
		memcmp ( sMcc , MCC_AUTOMATED_CASH , 4 ) == 0 &&
		( 	sIsoPosData [ 7 ] != P7_F022_P8_PIN    && 		/*AMER20190313 PROD00066569: Enh5.00 1050: || -> &&*/
			GetTlvBuffer ( AUTO_PIN_DATA      , sTlvBuffer , sPinData , &nLength ) != SUCCES) 
		)
	{
		if( memcmp (sPCode + 6, P7_F003_DEPOSIT,2) != 0 && 
			memcmp (sPCode + 6, P7_F003_DOCUMENT_DEPOSIT,2) != 0 && 
			memcmp (sPCode + 6, P7_F003_CHECK_DEPOSIT,2) != 0  )
		{
			PutEvent( sTlvBuffer, INVALID_MERCHANT, "Pos Data Invalid:MCC 6011 and PIN Metchod Diff than PIN "); 
			PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_MERCHANT_ACTION , 3);
			trace_event("Check MCC == POS DATA (NOK)",TRACE);
			trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
			return (NOK);
		}
	}

	/* Processing Code != Cash Advnace   */
	if ( memcmp ( sMcc , MCC_MANUAL_CASH , 4 ) == 0 &&
		memcmp (sPCode + 6, P7_F003_CASH_ADVANCE,2) != 0 && 
		memcmp (sPCode + 6, P7_F003_WITHDRAWAL,2) != 0 &&
		memcmp (sPCode + 6, P7_F003_CARD_RETURNED,2) != 0 /* MTR12062018 - Card return can be sent from a Manual Cash MCC */
		)  
	{
		PutEvent( sTlvBuffer, INVALID_MERCHANT, "Mcc 6010 and Processing Code != Cash Adavnce 17/01"); 
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_MERCHANT_ACTION , 3);
		trace_event(" 1 Check MCC == PROCESSING CODE (NOK)",TRACE);
		trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
		return (NOK);
	}

	/* Processing Code = Withdrawal   */
	if (
		memcmp ( sMcc , MCC_AUTOMATED_CASH , 4 ) !=0 &&
		memcmp ( sMcc , MCC_MANUAL_CASH , 4 ) !=0 &&
		memcmp (sPCode + 6, P7_F003_WITHDRAWAL,2) == 0    
		)  
	{
		PutEvent( sTlvBuffer, INVALID_MERCHANT, "Mcc != 6010 and 6011 and Processing Code = 01"); 
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_MERCHANT_ACTION, 3);
		trace_event("Check MCC == PROCESSING CODE (NOK)",TRACE);
		trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
		return (NOK);
	}

	/* Processing Code = Cash Advance   */
	if ( memcmp ( sMcc , MCC_MANUAL_CASH , 4 ) !=0 &&
		 memcmp (sPCode + 6, P7_F003_CASH_ADVANCE,2) == 0    
		)  
	{
		PutEvent( sTlvBuffer, INVALID_MERCHANT, "Mcc != 6010 and Processing Code = 17"); 
		PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_MERCHANT_ACTION , 3);
		trace_event(" 2 Check MCC == PROCESSING CODE (NOK)",TRACE);
		trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
		return (NOK);
	}

	/* PLUTONL-2492 */
	GetTlvBuffer(AUTO_AMOUNT_TRANSACTION, sTlvBuffer, slTransAmount, &nLength);	
	if(memcmp(sPCode + 6, P7_F003_WITHDRAWAL, P7_F003_P1_LEN) == 0 && atoi(slTransAmount) == 0)
	{
		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , INVALID_SERVICE_SETUP  , 3);
		trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
		return (NOK);
	}


	memset (  sPan      	, 0 , sizeof ( sPan ));
	memset (  sCardPan      , 0 , sizeof ( sCardPan ));
	memset (  sTrackII      , 0 , sizeof ( sTrackII )); 
	memset (  sPvv      	, 0 , sizeof ( sPvv )); 
	memset (  sCvv1      	, 0 , sizeof ( sCvv1 )); 
	memset( sPinData, 0, sizeof(sPinData));
	InitTlvPrivate(&sTlvPrivateInfo);
	InitTlvPrivate(&TlvInfo);

	trace_event("End   CheckReqTlvFields(OK)",PROCESSING);

	return ( OK );
}


int CheckAdvTlvFields ( int nIndiceCtx, char *sTlvBuffer )
{
	char sMcc        	[ 4 + 1 ];
	char sPCode      	[ 30 ];
	char sIsoPosData 	[ 12 + 1 ];
	char sTrackII 	[ 256 ];
	char sExpiryDate 	[ 4 + 1 ];
	char sTrackIIExpDate[ 4 + 1 ];
	char sPan		[ 24 + 1 ];
	char sCvv1		[ 3 + 1 ];
	char sPvv		[ 4 + 1 ];
	char sServiceCode	[ 3 + 1 ];
	char sCardPan	[ 24 + 1 ];
	char sLine       	[ MAX_LINE_TRC ];
	int  nLength;

	trace_event("Start CheckAdvTlvFields()",PROCESSING);


	memset (  sPCode      	, 0 , sizeof ( sPCode ));
	memset (  sMcc       	, 0 , sizeof ( sMcc ));
	memset (  sIsoPosData 	, 0 , sizeof ( sIsoPosData ));
	memset (  sTrackII 		, 0 , sizeof ( sTrackII ));
	memset (  sExpiryDate 	, 0 , sizeof ( sExpiryDate ));
	memset (  sTrackIIExpDate 	, 0 , sizeof ( sTrackIIExpDate ));
	memset (  sPan 		, 0 , sizeof ( sPan ));
	memset (  sCvv1 		, 0 , sizeof ( sCvv1 ));
	memset (  sPvv 		, 0 , sizeof ( sPvv ));
	memset (  sServiceCode 	, 0 , sizeof ( sServiceCode ));
	memset (  sCardPan 		, 0 , sizeof ( sCardPan ));

	if ( GetTlvBuffer ( AUTO_PAN , sTlvBuffer , sCardPan , &nLength ) == SUCCES)
		if ( GetTlvBuffer ( AUTO_TRACK2_DATA , sTlvBuffer , sTrackII , &nLength ) == SUCCES)
		{
			GetTrack2Data(sTrackII,sPan,sTrackIIExpDate,sServiceCode,sPvv,sCvv1);

			/*MKB060309 PCI Norme Integration */
			if (debug_option == 1)
			{
				sprintf(sLine,"Check Crad Num   %s == TrackII Crad Num   %s",sCardPan, sPan);
				trace_event(sLine,TRACE);
			}

			if (memcmp(sCardPan,sPan,16) != 0)
			{
				PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_TRANSACTION_ACTION , 3);
				trace_event("Check PAN  == TRACK II PAN  (NOK)",TRACE);
				trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
				return (NOK);
			}
		}

		if ( GetTlvBuffer ( AUTO_EXPIRY_DATE , sTlvBuffer , sExpiryDate , &nLength ) == SUCCES)
			if ( GetTlvBuffer ( AUTO_TRACK2_DATA , sTlvBuffer , sTrackII , &nLength ) == SUCCES)
			{
				GetTrack2Data(sTrackII,sPan,sTrackIIExpDate,sServiceCode,sPvv,sCvv1);

				if (debug_option == 1)
				{
					sprintf(sLine,"Check ExpiryDate %s == TrackII ExpiryDate %s",sExpiryDate, sTrackIIExpDate);
					trace_event(sLine,TRACE);
				}
				if (memcmp(sExpiryDate,sTrackIIExpDate,4) != 0)
				{
					PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_TRANSACTION_ACTION , 3);
					trace_event("Check EXP DATE == TRACK II EXP DATE  (NOK)",TRACE);
					trace_event("End   CheckAdvTlvFields(NOK)",PROCESSING);
					return (NOK);
				}
			}

			if ( GetTlvBuffer ( AUTO_PROCESSING_CODE      , sTlvBuffer , sPCode , &nLength ) == SUCCES)
				if ( GetTlvBuffer ( AUTO_MERCHANT_TYPE , sTlvBuffer , sMcc , &nLength ) == SUCCES)
				{
					sprintf(sLine,"Check MCC %s == PROCESSING CODE  %s",sMcc, sPCode);
					trace_event(sLine,TRACE);
					if (
						( !memcmp ( sMcc , "6011" , 4 ) &&
						memcmp (sPCode + 6, "01",2) != 0    )  /* Processing Code != Withrawal   */
						)
					{
						PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_MERCHANT_ACTION , 3);
						trace_event("Check MCC == PROCESSING CODE (NOK)",TRACE);
						trace_event("End   CheckAdvTlvFields(NOK)",PROCESSING);
						return (NOK);
					}
					if (
						( !memcmp ( sMcc , "6010" , 4 ) &&
						memcmp (sPCode + 6, "17",2) != 0 && memcmp (sPCode + 6, "01",2) != 0  ) 
						&&  memcmp (sPCode + 6, P7_F003_CARD_RETURNED,2) != 0 /* MTR CBAE NON REG TESTING : L'avis de Restitution de la carte peut-etre envoye depuis un MCC=6010  */
						 /* Processing Code != Cash Advnace   */
						)
					{
						PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_MERCHANT_ACTION , 3);
						trace_event(" 3 Check MCC == PROCESSING CODE (NOK)",TRACE);
						trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
						return (NOK);
					}

					if (
						( memcmp ( sMcc , "6011" , 4 ) !=0 &&
						memcmp ( sMcc , "6010" , 4 ) !=0 &&
						!memcmp (sPCode + 6, "01",2)     )  /* Processing Code = Withdrawal   */
						)
					{
						PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_MERCHANT_ACTION , 3);
						trace_event("Check MCC == PROCESSING CODE (NOK)",TRACE);
						trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
						return (NOK);
					}
					if (
						( memcmp ( sMcc , "6010" , 4 ) !=0 &&
						!memcmp (sPCode + 6, "17",2)     )  /* Processing Code = Cash Advance   */
						)
					{
						PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, INVALID_MERCHANT_ACTION , 3);
						trace_event(" 4 Check MCC == PROCESSING CODE (NOK)",TRACE);
						trace_event("End   CheckReqTlvFields(NOK)",PROCESSING);
						return (NOK);
					}
				}

				memset (  sCardPan      , 0 , sizeof ( sCardPan ));/** PA DSS*/
				memset (  sPan      , 0 , sizeof ( sPan )); /** PA DSS*/
				memset (  sTrackII      , 0 , sizeof ( sTrackII )); /** PA DSS*/

				trace_event("End   CheckAdvTlvFields(OK)",PROCESSING);
				return ( OK );
}


int AuthorLog ( int nIndiceCtx, TSIsoInfo *sIsoMsg, 
	char *sCaptureCode, char *sRoutingCode ,
	char  *sAuthoTable  )
{
	int                           nRetCode;
	int                           length;
	char                         sTlvBuffer[ LG_MAX + 1 ];
	TSTlvAutho                   sTlvInfo;

	trace_event ( "Start AuthorLogInsertion()", PROCESSING );
	memset ( sTlvBuffer , 0,  sizeof ( sTlvBuffer ));
	InitTlvAutho(&sTlvInfo);

	IsoToTlv    ( nIndiceCtx , sIsoMsg   , &sTlvInfo );
	PutTlvAutho ( AUTO_ROUTING_CODE, &sTlvInfo, sRoutingCode ,  6);
	PutTlvAutho ( AUTO_CAPTURE_CODE, &sTlvInfo, sCaptureCode,  6);
	PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable ,     3);
	PutTlvAutho ( AUTO_AUTHO_FLAG          , &sTlvInfo,  "P" ,  1);
	PutTlvAutho ( AUTO_AUTHOR_SRC  , &sTlvInfo , "E2" ,  2);
	PutTlvAutho ( AUTO_NODE_ID, &sTlvInfo , NodeId ,  NODE_ID_LEN);
	PutTlvAutho ( AUTO_DBSERV_ID, &sTlvInfo , g_LocDBServ ,  DBINST_LEN);
	PutTlvAutho ( AUTO_CAPTURE_RES_INFO, &sTlvInfo , NodeId ,  NODE_ID_LEN);/*YDA20181207 see(PROD00063910)*/
        PutTlvAutho ( AUTO_SWI_DBSERV_ID, &sTlvInfo , g_LocDBServ ,  DBINST_LEN);/*YDA20181207 see(PROD00063910)*/

	length = AuthoBuildTlv( sTlvBuffer, &sTlvInfo);

	nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{
		PutTlvBuffer ( AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		memset ( sTlvBuffer , 0,  sizeof ( sTlvBuffer ));/*NAK PADSS*/
		if ( nRetCode != OK )
		{
			trace_event  ( "SEVERE ERROR !!!!!", PROCESSING );
			trace_event  ( "Can Not Insert Autho Activity File ", PROCESSING );
			return ( SEVERE_ERROR );
		}
	}
	memset ( sTlvBuffer , 0,  sizeof ( sTlvBuffer ));/*NAK PADSS*/
	trace_event ( "End AuthorLogInsertion()", PROCESSING );
	return ( OK );
}

int ApproveAuth(int nIndiceCtx, char* sTlvBuffer)
{
	int		nRetCode;

	PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, APPROVED_ACTION, 3);
	nRetCode = GetAuthNumber(nIndiceCtx, sTlvBuffer);
	return OK;
}


int GetResAuthMsgMode(int nIndexCtx, char* sResourceId, TSIsoInfo*   IsoInfo, char* sProcMode)
{

	char	sMTI[4 + 1];
	char	sMcc[4 + 1];
	char	sProcCode[2 + 1];
	char	sFunctionCode[3 + 1];
	char	sSourceAcc[2 + 1];
	char	sDestAcc[2 + 1];
	char	sData[128];
	int		nResult;
	int		nLength;

	trace_event ( "Start GetResAuthMsgMode()", PROCESSING );
	memset(sMTI,'\0',sizeof(sMTI));
	memset(sMcc,'\0',sizeof(sMcc));
	memset(sProcCode,'\0',sizeof(sProcCode));
	memset(sFunctionCode,'\0',sizeof(sFunctionCode));
	memset(sSourceAcc,'\0',sizeof(sSourceAcc));
	memset(sDestAcc,'\0',sizeof(sDestAcc));

	sprintf(sMTI,"%.4d",IsoInfo->nMsgType);
	GetIsoField (ISO_PROC_CODE, IsoInfo, sData, &nLength);
	memcpy(sProcCode,sData,2);
	memcpy(sSourceAcc,sData + 2,2);
	memcpy(sDestAcc,sData+4,2);
	GetIsoField (ISO_FUNCTION_CODE, IsoInfo, sFunctionCode, &nLength);
	GetIsoField (ISO_MERCHANT_TYPE, IsoInfo, sMcc, &nLength);

	nResult = GetResAuthProcMode (	nIndexCtx, 
		sResourceId , 
		sMTI, 
		sMcc, 
		sProcCode, 
		sFunctionCode, 
		sSourceAcc, 
		sDestAcc, 
		sProcMode );
	if( nResult != OK )
	{
		trace_event ( "End   GetResAuthMsgMode(NOK): GetResAuthProcMode() failed ", PROCESSING );
		return NOK;
	}

	trace_event ( "End   GetResAuthMsgMode(OK)", PROCESSING );
	return OK;
}

