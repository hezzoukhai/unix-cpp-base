#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <sys/time.h>
#include <define.h>
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


int AutomaticReversalProc   (		int  nIndiceCtx,    char *reversal_mode ,
	char *sCaptureCode, char *sRoutingCode , 
	char *sAuthoTable,  char* sTlvBufferIn, char  *sReasonCode )
{

	TSTlvPrivate    TlvPrivateInfo;
	char            sLine            [MAX_LINE_TRC];
	char            sDate            [ 24 + 1 ];
	char            sEventCode       [ 3 + 1 ];
	char            sMti			[ 4 + 1 ];
	char			sAcquirerId    [ 11+1 ] ;
	char			sAcquirerIdLen [  2+1 ] ;	
	char			ReversalXmit [ 10 + 1 ];
	char			ReversalStan [  6 + 1 ];
	char			sStan [  6 + 1 ];
	char			OriginalDataElements [  256 + 1 ];

	char			tmpField [ BUF_LG_MAX ];
	char			sTlvBuffer[ LG_MAX ];
	int				Offset,nMti;
	int				nOrigMti;
	int             nLength;
	int				nAcquirerIdLen;

	trace_event("Start AutomaticReversalProc()",PROCESSING);


	/******************************************************************************/
	/*                         Initialisation                                     */
	/******************************************************************************/

	memset(sEventCode           , 0, sizeof(sEventCode));
	memset(ReversalXmit         , 0, sizeof(ReversalXmit));
	memset(ReversalStan         , 0, sizeof(ReversalStan));
	memset(sAcquirerIdLen       , 0, sizeof(sAcquirerIdLen));
	memset(OriginalDataElements , 0, sizeof(OriginalDataElements));
	memset( sLine                ,0, sizeof ( sLine ));
	memset(sDate                 ,0, sizeof ( sDate ));
	memset(sMti                  ,0, sizeof ( sMti ));
	memset(sAcquirerId           ,0, sizeof ( sAcquirerId ));
	memset ( sStan               , 0 , sizeof ( sStan ));
	memset (tmpField             , 0 , sizeof ( tmpField ));
	memset (sTlvBuffer             , 0 , sizeof ( sTlvBuffer ));

	InitTlvPrivate(&TlvPrivateInfo); 

	strncpy(sTlvBuffer,sTlvBufferIn, sizeof(sTlvBuffer) - 1);

	GetTlvBuffer ( AUTO_TRANS_DATE_TIME, sTlvBuffer, ReversalXmit ,  &nLength);
	GetTlvBuffer ( AUTO_ACQ_INST_IDENT_CODE, sTlvBuffer, sAcquirerId ,  &nAcquirerIdLen);

	if ( GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE, sTlvBuffer, tmpField ,  &nLength) == SUCCESS )
	{
		memset (  sStan  , 0 , sizeof ( sStan ));
		AnalyseTlvPrivate (tmpField, nLength , &TlvPrivateInfo);
		GetTlvPrivate (EXTERNAL_STAN_TAG , &TlvPrivateInfo, sStan, &nLength);
	}
	else
		GetTlvBuffer ( AUTO_INTERNAL_STAN, sTlvBuffer, sStan ,  &nLength);

	nOrigMti = 0;
	if ( GetTlvBuffer ( AUTO_MSG_TYPE, sTlvBuffer, tmpField ,  &nLength) == SUCCESS )
		nOrigMti = atoi(tmpField);

	if ( ( nOrigMti == 1110 ) || ( nOrigMti == 1100 )) 
		nMti = 1100;
	else if ( ( nOrigMti == 1210 ) || ( nOrigMti == 1200 )) 
		nMti = 1200;
	else 
	{
		trace_event ("Can Not Reverse such Request",ERROR);
		return (NOK);
	}

	PutTlvBuffer ( AUTO_FUNCTION_CODE, sTlvBuffer, P7_F024_FULL_REVERSAL , P7_F024_LEN );/*NAB02062021 PLUTONL-3190*/
	PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE, sTlvBuffer, sReasonCode ,  4);
	


	memset  ( OriginalDataElements , 0 , sizeof ( OriginalDataElements ));
	memset  ( OriginalDataElements , '0' , 35 );
	memset  ( sMti , 0 , sizeof ( sMti ));
	sprintf ( sMti , "%4d", nMti );
	Offset = 0;
	memcpy  ( OriginalDataElements + Offset , sMti, 4); 			
	Offset += 4 ;

	/*Original Stan */
	memcpy  ( OriginalDataElements  + Offset , sStan    , 6); Offset += 6;
	/* Original Xmit */
	memcpy  ( OriginalDataElements + Offset , "00" , 2 ); Offset += 2;
	memcpy  ( OriginalDataElements + Offset , ReversalXmit , 10 ); Offset += 10;
	/* Original Acquirer Id */
	sprintf ( sAcquirerIdLen , "%02d" , nAcquirerIdLen );
	memcpy  ( OriginalDataElements + Offset , sAcquirerIdLen, 2 ); Offset += 2;
	memcpy  ( OriginalDataElements + Offset + 11 - nAcquirerIdLen , sAcquirerId , nAcquirerIdLen ); Offset += 11;
	memcpy  ( OriginalDataElements + Offset,  "0000000000000", 13); Offset += 13; 
	sprintf (sLine, "ISO Original Data = %s", OriginalDataElements);
	trace_event(sLine,TRACE);


	PutTlvBuffer ( AUTO_ORIGINAL_DATA, sTlvBuffer, OriginalDataElements ,  Offset);
	PutTlvBuffer ( AUTO_MSG_TYPE, sTlvBuffer, "1420" ,  4);
	PutTlvBuffer ( AUTO_CUR_TABLE_INDICATOR, sTlvBuffer, sAuthoTable ,  3);

	/*YDA20171208*/
	/*if ( memcmp ( reversal_mode  , "C" , 1 ) == 0 )
	PutTlvBuffer   ( AUTO_CAPTURE_CODE , sTlvBuffer, sCaptureCode ,    6);
	else
	{
	PutTlvBuffer   ( AUTO_ROUTING_CODE  , sTlvBuffer, sRoutingCode ,    6);
	PutTlvBuffer   ( AUTO_INTERNAL_STAN , sTlvBuffer, sStan        ,    6);
	}*/
	PutTlvBuffer   ( AUTO_CAPTURE_CODE , sTlvBuffer, sCaptureCode ,    6);
	PutTlvBuffer   ( AUTO_ROUTING_CODE  , sTlvBuffer, sRoutingCode ,    6);

	GetInternalStan ( nIndiceCtx , sStan );
	current_gmt_date (sDate);

	PutTlvBuffer ( AUTO_REVERSAL_STAN , sTlvBuffer, sStan ,  6 );
	PutTlvBuffer ( AUTO_REVERSAL_DATE , sTlvBuffer, sDate + 2 ,  10 );
	PutTlvBuffer ( AUTO_RESPONSE_CODE , sTlvBuffer, REVERSAL_COMPLETED_ACTION , 3 );
	PutTlvBuffer( AUTO_INTERNAL_STAN , sTlvBuffer, sStan , 6 );


	PrintTlvBuffer ( sTlvBuffer );

	trace_event("before InsertionSaf",TRACE);


	if ( memcmp ( sRoutingCode , STAND_IN_RESOURCE , 6 ) != 0 )
		InsertionSaf ( nIndiceCtx , sTlvBuffer, sCaptureCode , sRoutingCode );

	trace_event("End   AutomaticReversalProc()",PROCESSING);
	return ( OK );
}



int AutomaticIsoReversalProc   (	int  nIndiceCtx,    char *reversal_mode ,
	char *sCaptureCode, char *sRoutingCode , 
	char *sAuthoTable, TSIsoInfo *IsoInfo,
	char  *sReasonCode )
{
	char            sTlvBuffer[ LG_MAX + 1 ];
	TSTlvAutho      sTlvInfo;
	TSTlvPrivate    TlvPrivateInfo;
	char            sLine            [MAX_LINE_TRC];
	char            sDate                [ 24 + 1 ];
	char            sEventCode           [ 3 + 1 ];
	char            sMti           [ 4 + 1 ];
	char			sAcquirerId    [ 11+1 ] ;
	char			sAcquirerIdLen [  2+1 ] ;	
	char			ReversalXmit [ 10 + 1 ];
	char			ReversalStan [  6 + 1 ];
	char			sStan [  6 + 1 ];
	char			OriginalDataElements [  256 + 1 ];
	char			tmpField [ LG_MAX ];
	int				Offset,nMti;
	int             nLength;
	int				nAcquirerIdLen;
	char                    szWork[6 + 1];		/*AMER 20160714*/
	int                       nLen;


	trace_event("Start AutomaticIsoReversalProc()",PROCESSING);


	/******************************************************************************/
	/*                         Initialisation                                     */
	/******************************************************************************/

	memset(sEventCode           , 0, sizeof(sEventCode));
	memset(ReversalXmit         , 0, sizeof(ReversalXmit));
	memset(ReversalStan         , 0, sizeof(ReversalStan));
	memset(sAcquirerIdLen       , 0, sizeof(sAcquirerIdLen));
	memset(OriginalDataElements , 0, sizeof(OriginalDataElements));
	memset( sTlvBuffer           ,0, sizeof ( sTlvBuffer ));
	memset(sDate                 ,0, sizeof ( sDate ));
	memset(sMti                  ,0, sizeof ( sMti ));
	memset(sAcquirerId           ,0, sizeof ( sAcquirerId ));
	memset ( sStan               , 0 , sizeof ( sStan ));
	memset (tmpField             , 0 , sizeof ( tmpField ));
	memset(szWork,'\0',sizeof(szWork));			/*AMER 20160714*/

	InitTlvAutho    ( &sTlvInfo );
    InitTlvPrivate(&TlvPrivateInfo);/*NAB*/

	sprintf(sLine," Reversal Mode %s", reversal_mode);
	trace_event(sLine,TRACE);

	sprintf(sLine," Capture Code %s", sCaptureCode);
	trace_event(sLine,TRACE);

	sprintf(sLine," Routing Code %s", sRoutingCode);
	trace_event(sLine,TRACE);

	sprintf(sLine," Auth Table   %s", sAuthoTable);
	trace_event(sLine,TRACE);

	sprintf(sLine," Reason Code  %s", sReasonCode);
	trace_event(sLine,TRACE);


	GetIsoField ( ISO_XMIT_TIME , IsoInfo, ReversalXmit , &nLength);
	GetIsoField ( ISO_ACQR_ID   , IsoInfo, sAcquirerId , &nAcquirerIdLen);
	if ( GetIsoField ( ISO_ADTNL_DATA_PRIV , IsoInfo, tmpField, &nLength) == SUCCESS )
	{
		memset (  sStan  , 0 , sizeof ( sStan ));
		AnalyseTlvPrivate (tmpField, nLength , &TlvPrivateInfo);
		GetTlvPrivate (EXTERNAL_STAN_TAG , &TlvPrivateInfo, sStan, &nLength);
	}
	else
		GetIsoField ( ISO_AUDIT_NBR   , IsoInfo, sStan , &nLength);

	if ( ( IsoInfo->nMsgType == 1110 ) ||
		( IsoInfo->nMsgType == 1100 )
		) nMti = 1100;
	else if (
		( IsoInfo->nMsgType == 1210 ) ||
		( IsoInfo->nMsgType == 1200 )
		) nMti = 1200;
	else {
		trace_event ("Can Not Reverse such Request",ERROR);
		return (NOK);
	}

	
	PutIsoField ( ISO_FUNCTION_CODE , IsoInfo, P7_F024_FULL_REVERSAL, P7_F024_LEN);/*NAB02062021 PLUTONL-3190*/
	PutIsoField ( ISO_MSG_REASON , IsoInfo, sReasonCode , 4);

	memset  ( OriginalDataElements , 0 , sizeof ( OriginalDataElements ));
	memset  ( OriginalDataElements , '0' , 35 );
	memset  ( sMti , 0 , sizeof ( sMti ));
	sprintf ( sMti , "%4d", nMti );
	Offset = 0;
	memcpy  ( OriginalDataElements + Offset , sMti, 4); 			
	Offset += 4 ;

	/*Original Stan */
	memcpy  ( OriginalDataElements  + Offset , sStan    , 6); 		
	Offset += 6;
	/* Original Xmit */
	memcpy  ( OriginalDataElements + Offset , "00" , 2 ); 			 
	Offset += 2;
	memcpy  ( OriginalDataElements + Offset , ReversalXmit , 10 );   
	Offset += 10;
	/* Original Acquirer Id */
	sprintf ( sAcquirerIdLen , "%02d" , nAcquirerIdLen );
	memcpy  ( OriginalDataElements + Offset , sAcquirerIdLen, 2 );  
	Offset += 2;
	memcpy  ( OriginalDataElements + Offset + 11 - nAcquirerIdLen , sAcquirerId , nAcquirerIdLen ); Offset += 11;
	memcpy  ( OriginalDataElements + Offset,  "0000000000000", 13); Offset += 13; 


	PutIsoField ( ISO_ORIG_DATA, IsoInfo, OriginalDataElements , Offset);
	IsoInfo->nMsgType = 1420;

	IsoToTlv      ( nIndiceCtx , IsoInfo , &sTlvInfo );

	PutTlvAutho   ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable , 3);

	
	PutTlvAutho   ( AUTO_CAPTURE_CODE , &sTlvInfo, sCaptureCode ,    6);
	PutTlvAutho   ( AUTO_ROUTING_CODE  , &sTlvInfo, sRoutingCode ,    6);


	GetInternalStan ( nIndiceCtx , sStan );
	PutTlvAutho ( AUTO_REVERSAL_STAN , &sTlvInfo, sStan ,  6 );
	current_gmt_date (sDate);
	PutTlvAutho ( AUTO_REVERSAL_DATE , &sTlvInfo, sDate + 2 ,  10 );
	PutTlvAutho ( AUTO_RESPONSE_CODE , &sTlvInfo, REVERSAL_COMPLETED_ACTION , 3 );
	/*AMER 20160714 Start*/
	if (GetTlvPrivate(ISSUER_BANK, &TlvPrivateInfo, szWork , &nLen) != SUCCESS)
	{
		trace_event("End AutomaticIsoReversalProc (NOK) : ERROR Getting ISSUER BANK ",ERROR);
		return(NOK);
	}
	PutTlvAutho(AUTO_ISSUING_BANK, &sTlvInfo, szWork, nLen);
	if (GetTlvPrivate(ACQUIRER_BANK_TAG, &TlvPrivateInfo, szWork , &nLen) != SUCCESS)
	{
		trace_event("End AutomaticIsoReversalProc (NOK) : ERROR Getting ACQUIRER BANK ",ERROR);
		return(NOK);
	}
	PutTlvAutho(AUTO_ACQUIRER_BANK, &sTlvInfo, szWork , nLen);
	/*AMER 20160714 End*/

	AuthoBuildTlv ( sTlvBuffer, &sTlvInfo);

	PutTlvBuffer( AUTO_INTERNAL_STAN , sTlvBuffer, sStan , 6 );
	PrintTlvBuffer ( sTlvBuffer );


	if ( memcmp ( sRoutingCode , STAND_IN_RESOURCE , 6 ) != 0 )
		InsertionSaf ( nIndiceCtx , sTlvBuffer, sCaptureCode , sRoutingCode );

	InitTlvAutho    ( &sTlvInfo );
	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );

	TlvToIso        ( nIndiceCtx , &sTlvInfo , IsoInfo , REVERSAL );

	trace_event("End   AutomaticIsoReversalProc(OK)",PROCESSING);
	return ( OK );
}

/* ELKAM PLUTONL-3835 START */
/* Function to process a reversal as it is from Network */

int GenIsoRevProc   (int  nIndiceCtx, char *sAuthoTable, TSIsoInfo *IsoInfo, char  *sReasonCode )
{
	char            sTlvBuffer[ LG_MAX + 1 ];
	TSTlvAutho      sTlvInfo;
	TSTlvPrivate    TlvPrivateInfo;
	char            sLine            [MAX_LINE_TRC];
	char            sDate                [ 24 + 1 ];
	char            sEventCode           [ 3 + 1 ];
	char            sMti           [ 4 + 1 ];
	char			sAcquirerId    [ 11+1 ] ;
	char			sAcquirerIdLen [  2+1 ] ;	
	char			ReversalXmit [ 10 + 1 ];
	char			ReversalStan [  6 + 1 ];
	char			sStan [  6 + 1 ];
	char			OriginalDataElements [  256 + 1 ];
	char			tmpField [ LG_MAX ];
	int				Offset,nMti;
	int             nLength;
	int				nAcquirerIdLen;
	char            szWork[6 + 1];		
	int             nLen;
	int				result;
	char       		sCaptureCode[ 6 + 1 ];
	char       		sRoutingCode[ 6 + 1 ]; 
	char            sSecurityData[128];


	trace_event("Start GenIsoRevProc()",PROCESSING);


	/******************************************************************************/
	/*                         Initialisation                                     */
	/******************************************************************************/

	memset(sEventCode           , 0, sizeof(sEventCode));
	memset(ReversalXmit         , 0, sizeof(ReversalXmit));
	memset(ReversalStan         , 0, sizeof(ReversalStan));
	memset(sAcquirerIdLen       , 0, sizeof(sAcquirerIdLen));
	memset(OriginalDataElements , 0, sizeof(OriginalDataElements));
	memset( sTlvBuffer           ,0, sizeof ( sTlvBuffer ));
	memset(sDate                 ,0, sizeof ( sDate ));
	memset(sMti                  ,0, sizeof ( sMti ));
	memset(sAcquirerId           ,0, sizeof ( sAcquirerId ));
	memset ( sStan               , 0 , sizeof ( sStan ));
	memset (tmpField             , 0 , sizeof ( tmpField ));
	memset(szWork,'\0',sizeof(szWork));			/*AMER 20160714*/
	memset(sSecurityData, 0, sizeof(sSecurityData));
	memset(sCaptureCode , 0 , sizeof ( sCaptureCode )); 
	memset(sRoutingCode , 0 , sizeof ( sRoutingCode ));

	InitTlvAutho    ( &sTlvInfo );
    InitTlvPrivate(&TlvPrivateInfo);/*NAB*/

	if (GetIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, &nLength) == SUCCESS)
	{
		GetSecurityData(SOURCE_RESOURCE, sSecurityData, sCaptureCode, &nLength);
		GetSecurityData(DESTINATION_RESOURCE, sSecurityData, sRoutingCode, &nLength);
	}

	sprintf(sLine," Capture Code %s", sCaptureCode);
	trace_event(sLine,TRACE);

	sprintf(sLine," Routing Code %s", sRoutingCode);
	trace_event(sLine,TRACE);

	sprintf(sLine," Auth Table   %s", sAuthoTable);
	trace_event(sLine,TRACE);

	sprintf(sLine," Reason Code  %s", sReasonCode);
	trace_event(sLine,TRACE);


	GetIsoField ( ISO_XMIT_TIME , IsoInfo, ReversalXmit , &nLength);
	GetIsoField ( ISO_ACQR_ID   , IsoInfo, sAcquirerId , &nAcquirerIdLen);
	if ( GetIsoField ( ISO_ADTNL_DATA_PRIV , IsoInfo, tmpField, &nLength) == SUCCESS )
	{
		memset (  sStan  , 0 , sizeof ( sStan ));
		AnalyseTlvPrivate (tmpField, nLength , &TlvPrivateInfo);
		GetTlvPrivate (EXTERNAL_STAN_TAG , &TlvPrivateInfo, sStan, &nLength);
	}
	else
		GetIsoField ( ISO_AUDIT_NBR   , IsoInfo, sStan , &nLength);

	if ( ( IsoInfo->nMsgType == 1110 ) ||
		( IsoInfo->nMsgType == 1100 )
		) nMti = 1100;
	else if (
		( IsoInfo->nMsgType == 1210 ) ||
		( IsoInfo->nMsgType == 1200 )
		) nMti = 1200;
	else {
		trace_event ("Can Not Reverse such Request",ERROR);
		return (NOK);
	}

	
	PutIsoField ( ISO_FUNCTION_CODE , IsoInfo, P7_F024_FULL_REVERSAL, P7_F024_LEN);/*NAB02062021 PLUTONL-3190*/
	PutIsoField ( ISO_MSG_REASON , IsoInfo, sReasonCode , 4);

	memset  ( OriginalDataElements , 0 , sizeof ( OriginalDataElements ));
	memset  ( OriginalDataElements , '0' , 35 );
	memset  ( sMti , 0 , sizeof ( sMti ));
	sprintf ( sMti , "%4d", nMti );
	Offset = 0;
	memcpy  ( OriginalDataElements + Offset , sMti, 4); 			
	Offset += 4 ;

	/*Original Stan */
	memcpy  ( OriginalDataElements  + Offset , sStan    , 6); 		
	Offset += 6;
	/* Original Xmit */
	memcpy  ( OriginalDataElements + Offset , "YY" , 2 ); 			 
	Offset += 2;
	memcpy  ( OriginalDataElements + Offset , ReversalXmit , 10 );   
	Offset += 10;
	/* Original Acquirer Id */
	sprintf ( sAcquirerIdLen , "%02d" , nAcquirerIdLen );
	memcpy  ( OriginalDataElements + Offset , sAcquirerIdLen, 2 );  
	Offset += 2;
	memcpy  ( OriginalDataElements + Offset + 11 - nAcquirerIdLen , sAcquirerId , nAcquirerIdLen ); Offset += 11;
	memcpy  ( OriginalDataElements + Offset,  "0000000000000", 13); Offset += 13; 


	PutIsoField ( ISO_ORIG_DATA, IsoInfo, OriginalDataElements , Offset);
	IsoInfo->nMsgType = 1420;

	GetInternalStan ( nIndiceCtx , sStan );
	PutIsoField( ISO_AUDIT_NBR , IsoInfo, sStan , 6 );

	IsoInfo->msgId = 0;

	result = ServiceProcessing(		nIndiceCtx,
									ResourceId,
									sAuthoTable,
									AcquirerBank,
									"Y",
									"G",
									"N",
									sTlvBuffer, 
									IsoInfo);

	if( result == NOK)
		{
			sprintf(sLine, "End  GenIsoRevProc(NOK): ServiceProcessing failure");
			trace_event(sLine, PROCESSING);
			return (NOK);
		}

	trace_event("End   GenIsoRevProc(OK)",PROCESSING);
	return ( OK );
}

/* ELKAM PLUTONL-3835 END */