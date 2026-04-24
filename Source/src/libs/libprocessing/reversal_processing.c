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


int ReversalProcessing   (  int nIndiceCtx,
                            char *sMyResource, 
                            char *sAuthoTable, 
                            TSIsoInfo *IsoInfo, 
							char  *sBillCalculation )

{

    char            sRoutingCode[ 6 + 1 ];
    char            sTlvBuffer[ LG_MAX + 1 ];
    TSTlvAutho      sTlvInfo;
    TSTlvPrivate    TlvPrivateInfo;
    char            sLine            [MAX_LINE_TRC];
    int             nLength;
    int             nEventCode   =  OK;
    int             nRetCode;
    char            sEventCode           [ 3 + 1 ];
    char            sData                [ LG_MAX + 1 ];
    char sActionCode [ 3 + 1 ];
    char sSecurData [ LG_MAX ];
    char ReversalXmit [ 10 + 1 ];
    char ReversalStan [  6 + 1 ];
    char OriginalDataElements [  35 + 1 ];
    char sSecondaryResource [ 6 + 1 ];
    char sAcqLength [ 2 + 1 ];
    char OrigAmounts [ 12 + 1 ];
    char tmpField [ LG_MAX ];
   trace_event("Start ReversalProcessing()",PROCESSING);
 
/******************************************************************************/
/*                         Initialisation                                     */
/******************************************************************************/
   memset(sEventCode           , 0, sizeof(sEventCode));
   memset(sRoutingCode         , 0, sizeof(sRoutingCode));
   memset(ReversalXmit         , 0, sizeof(ReversalXmit));
   memset(ReversalStan         , 0, sizeof(ReversalStan));
   memset(OriginalDataElements , 0, sizeof(OriginalDataElements));

   InitTlvAutho(&sTlvInfo);
   memset( sTlvBuffer          , 0, sizeof ( sTlvBuffer ));
   IsoToTlv      ( nIndiceCtx , IsoInfo , &sTlvInfo );

/******************************************************************************/
/*  Formatter le buffer TLV A partir du message Iso                           */
/******************************************************************************/
   
   if ( GetIsoField ( ISO_ORIG_DATA , IsoInfo , OriginalDataElements, &nLength ) != SUCCES )
   {
        PutIsoField( ISO_RESPONSE_CODE, IsoInfo , MALFUNCTION_ACTION , 3);
        trace_event ( "ERROR OriginalDataElements Missing ..", ERROR );
   		trace_event("End   ReversalProcessing(NOK)",PROCESSING);
        return ( NOK );
   }
   
   if ( GetIsoField ( ISO_AUDIT_NBR ,IsoInfo, ReversalStan , &nLength ) != SUCCES )
   {
        PutIsoField( ISO_RESPONSE_CODE, IsoInfo , MALFUNCTION_ACTION , 3);
        trace_event ( "ERROR Stan Missing ..", ERROR );
   		trace_event("End   ReversalProcessing(NOK)",PROCESSING);
        return ( NOK );
   }

   if ( GetIsoField(ISO_XMIT_TIME,IsoInfo,ReversalXmit,&nLength) != SUCCES )
   {
        PutIsoField( ISO_RESPONSE_CODE, IsoInfo , MALFUNCTION_ACTION , 3);
        trace_event ( "ERROR Xmit Time Missing ..", ERROR );
   		trace_event("End   ReversalProcessing(NOK)",PROCESSING);
        return ( NOK );
   }

   PutTlvAutho   ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable ,     3);
   PutTlvAutho   ( AUTO_CAPTURE_CODE        , &sTlvInfo, sMyResource ,     6);
   PutTlvAutho   ( AUTO_REVERSAL_STAN, &sTlvInfo, ReversalStan ,  6);
   PutTlvAutho   ( AUTO_REVERSAL_DATE, &sTlvInfo, ReversalXmit , 10); 

   memset ( sData , 0 , sizeof ( sData ));
   memcpy ( sData , OriginalDataElements + 4 , 6 );

   memset  ( tmpField , 0 , sizeof ( tmpField ));
   if(memcmp(sData, "000000", 6) != 0)
   {
   		InitTlvPrivate (&TlvPrivateInfo);
       	AddTlvPrivate (EXTERNAL_STAN_TAG , &TlvPrivateInfo, sData, 6);
   		nLength = PrivateBuildTlv (tmpField, &TlvPrivateInfo);
   		PutTlvAutho   ( AUTO_ADD_DATA_PRIVATE , &sTlvInfo, tmpField , nLength);
   }
   memset( sTlvBuffer          , 0, sizeof ( sTlvBuffer ));
   AuthoBuildTlv ( sTlvBuffer, &sTlvInfo);
   PrintTlvBuffer(sTlvBuffer);

   nRetCode = ReversalProcedure(  	nIndiceCtx , 
									NORMAL_MODE , sBillCalculation,
									sTlvBuffer , 
									sMyResource , sRoutingCode );
   PrintTlvBuffer (  sTlvBuffer );

   if ( nRetCode != OK )
   {
	 
      nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
     if ( nRetCode != OK )
               PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
     else
               PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);

      PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"MSC",3);
      PutTlvBuffer(AUTO_ROUTING_CODE ,sTlvBuffer,STAND_IN_RESOURCE ,6);

      InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );


      InitTlvAutho    ( &sTlvInfo );
      AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
      TlvToIso        ( nIndiceCtx , &sTlvInfo , IsoInfo , REVERSAL );
   	  trace_event("End   ReversalProcessing(NOK)",PROCESSING);
      return ( NOK );
   }

   PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,REVERSAL_COMPLETED_ACTION,3);
   UpdateSettlement( nIndiceCtx , sTlvBuffer);


   PrintTlvBuffer ( sTlvBuffer );
   if ( memcmp ( sRoutingCode , STAND_IN_RESOURCE , 6 ) != 0 )
        InsertionSaf ( nIndiceCtx , sTlvBuffer, sMyResource , sRoutingCode );

   InitTlvAutho    ( &sTlvInfo );
   AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
   TlvToIso        ( nIndiceCtx , &sTlvInfo , IsoInfo , REVERSAL );
   dump_p7_struct(IsoInfo);



	/*EBE140727: Log this reversal */
	PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"MSC",3);
	PutTlvBuffer(AUTO_ROUTING_CODE ,sTlvBuffer,STAND_IN_RESOURCE ,6);
	InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );

   	trace_event("End   ReversalProcessing(OK)",PROCESSING);
    return ( OK );
}


int AutomaticReversalProcessing   (  int  nIndiceCtx,    char *reversal_mode ,
                            		 char *sCaptureCode, char *sRoutingCode , 
                            		 char *sAuthoTable, TSIsoInfo *IsoInfo,
							         char  *sReasonCode )
{
    char            sTlvBuffer[ LG_MAX + 1 ];
    TSTlvAutho      sTlvInfo;
    TSTlvPrivate    TlvPrivateInfo;
    char            sLine                [MAX_LINE_TRC];
    char            sData                [ 30 + 1 ];
    char            sEventCode           [ 3 + 1 ];
    char            sMti                 [ 4 + 1 ];
    char            sAcquirerId          [ 11+1 ] ;
    char            sAcquirerIdLen       [  2+1 ] ;	
    char            ReversalXmit         [ 10 + 1 ];
    char            ReversalStan         [  6 + 1 ];
    char            sStan                [  6 + 1 ];
    char            OriginalDataElements [  100 + 1 ];
    char            tmpField [ LG_MAX ];
    int             Offset,nMti;
	int             nLength;
    int             nAcquirerIdLen;

   
   /******************************************************************************/
   /*                         Initialisation                                     */
   /******************************************************************************/
   memset(sEventCode           , 0, sizeof(sEventCode));
   memset(ReversalXmit         , 0, sizeof(ReversalXmit));
   memset(ReversalStan         , 0, sizeof(ReversalStan));
   memset(sAcquirerIdLen       , 0, sizeof(sAcquirerIdLen));
   memset(OriginalDataElements , 0, sizeof(OriginalDataElements));
   memset( sTlvBuffer           ,0, sizeof ( sTlvBuffer ));
   memset( sLine                ,0, sizeof ( sLine ));
   memset(sData                 ,0, sizeof ( sData ));
   memset(sMti                  ,0, sizeof ( sMti ));
   memset(sAcquirerId           ,0, sizeof ( sAcquirerId ));
   memset ( sStan               , 0 , sizeof ( sStan ));
   memset (tmpField             , 0 , sizeof ( tmpField ));

   memset(&TlvPrivateInfo, 0, sizeof(TSTlvPrivate));   
   InitTlvAutho    ( &sTlvInfo );



    sprintf(sLine,"tart AutomaticReversalProcessing(%d)", nIndiceCtx);
   trace_event(sLine,TRACE);
   
   sprintf(sLine," Reversal Mode %s", reversal_mode);   trace_event(sLine,TRACE);
   sprintf(sLine," Capture Code %s", sCaptureCode);     trace_event(sLine,TRACE);
   sprintf(sLine," Routing Code %s", sRoutingCode); trace_event(sLine,TRACE);
   sprintf(sLine," Auth Table   %s", sAuthoTable);trace_event(sLine,TRACE);
   sprintf(sLine," Reason Code  %s", sReasonCode);trace_event(sLine,TRACE);
   
   
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

	trace_event("before ISO_MSG_REASON",TRACE);

    PutIsoField ( ISO_MSG_REASON , IsoInfo, sReasonCode , 4);

    memset  ( OriginalDataElements , 0 , sizeof ( OriginalDataElements ));
    memset  ( OriginalDataElements , '0' , 60 );
    memset  ( sMti , 0 , sizeof ( sMti ));
    sprintf ( sMti , "%4d", nMti );
    Offset = 0;
    memcpy  ( OriginalDataElements + Offset , sMti, 4); 			
    Offset += 4 ;

    /*Original Stan */
    memcpy  ( OriginalDataElements  + Offset , sStan    , 6); 		
    Offset += 6;
    
	/* Original Xmit */
    memset(tmpField,'\0',sizeof(tmpField));
	if (GetIsoField ( ISO_TRANS_DATE_TIME , IsoInfo, tmpField , &nLength)==SUCCESS )
      memcpy  ( OriginalDataElements + Offset , tmpField , 2 ); 			 
	else
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

	sprintf (sLine, "==>ISO Original Data =[%d][%s]",strlen(OriginalDataElements),OriginalDataElements);
	trace_event(sLine,TRACE);
	
	memset(tmpField,'\0',sizeof(tmpField));
	if (GetIsoField ( ISO_TRANS_DATE_TIME , IsoInfo, tmpField , &nLength)==SUCCESS )
    {
      memcpy( OriginalDataElements + Offset , tmpField , 12 );
      Offset += 12;
	}
    
    sprintf (sLine, "ISO Original Data = %s", OriginalDataElements);
	trace_event(sLine,TRACE);

    trace_event("before ISO_ORIG_DATA",TRACE);

    /*PutIsoField ( ISO_ORIG_DATA, IsoInfo, OriginalDataElements , Offset);*/
    PutIsoField ( ISO_ORIG_DATA, IsoInfo, OriginalDataElements ,strlen(OriginalDataElements));
    IsoInfo->nMsgType = 1420;

   /*InitTlvAutho(&sTlvInfo);*/
   
   IsoToTlv      ( nIndiceCtx , IsoInfo , &sTlvInfo );

   PutTlvAutho   ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable , 3);

   if ( memcmp ( reversal_mode  , "C" , 1 ) == 0 )
        PutTlvAutho   ( AUTO_CAPTURE_CODE , &sTlvInfo, sCaptureCode ,    6);
   else
   {
        PutTlvAutho   ( AUTO_ROUTING_CODE  , &sTlvInfo, sRoutingCode ,    6);
        PutTlvAutho   ( AUTO_INTERNAL_STAN , &sTlvInfo, sStan        ,    6);
   }

   GetInternalStan ( nIndiceCtx , sStan );
   PutTlvAutho ( AUTO_REVERSAL_STAN , &sTlvInfo, sStan ,  6 );
   current_gmt_date (sData);
   PutTlvAutho ( AUTO_REVERSAL_DATE , &sTlvInfo, sData + 2 ,  10 );
   PutTlvAutho ( AUTO_RESPONSE_CODE , &sTlvInfo, REVERSAL_COMPLETED_ACTION , 3 );
   
   trace_event("before AuthoBuildTlv",TRACE);

   AuthoBuildTlv ( sTlvBuffer, &sTlvInfo);
   /*
      ReversalProcedure( nIndiceCtx , 
   						 reversal_mode , "N" , 
   						 sTlvBuffer , 
   						 sCaptureCode, sRoutingCode );
   */

   PutTlvBuffer( AUTO_INTERNAL_STAN , sTlvBuffer, sStan , 6 );
   PrintTlvBuffer ( sTlvBuffer );

   trace_event("before InsertionSaf",TRACE);
   if ( memcmp ( sRoutingCode , STAND_IN_RESOURCE , 6 ) != 0 )
        InsertionSaf ( nIndiceCtx , sTlvBuffer, sCaptureCode , sRoutingCode );

   InitTlvAutho    ( &sTlvInfo );
   AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
   trace_event("before TlvToIso",TRACE);

   TlvToIso        ( nIndiceCtx , &sTlvInfo , IsoInfo , REVERSAL );
   return ( OK );
}
