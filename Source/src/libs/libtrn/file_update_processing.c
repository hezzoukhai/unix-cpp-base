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
#include <resources.h>
#include <event_auth.h>

/****************************************************************************/
int FileUpdateProcessing	(	int nIndiceCtx,
				                char *sMyResource,
				                char *sDestResource,
				 	            char *sAuthoTable,
				                TSIsoInfo *sIsoMsg  )
{

	TSResources     sResource;
    char    	    sTlvBuffer [ LG_MAX + 1 ];
	TSTlvAutho      sTlvInfo;
	char		sLine		[MAX_LINE_TRC];
    int     	nLength;
	int 		nEventCode =  OK;
	int 		nRetCode;
	char 		sEventCode  [ 3 + 1 ];
	char 		sActionCode  [ 3 + 1 ];
    char    	sAlternAmount [ LG_MAX + 1 ];
    char        sSecurityData[LG_MAX+1];
    char        sSourcePid[8 + 1];
    char        sDestinationPid[8 + 1];
    int         nDestinationPid;
	
    trace_event("Start FileUpdateProcessing()",PROCESSING);
/******************************************************************************/
/* 					Initialisation			*/
/******************************************************************************/

	InitTlvAutho(&sTlvInfo);
    memset( sTlvBuffer   	    , 0, sizeof ( sTlvBuffer ));

/******************************************************************************/
/*  Formatter le buffer TLV A partir du message Iso                           */
/******************************************************************************/
    IsoToTlv ( nIndiceCtx , sIsoMsg , &sTlvInfo );
    PutTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sMyResource   ,  6);
    PutTlvAutho ( AUTO_ROUTING_CODE        , &sTlvInfo, sDestResource ,  6);
    PutTlvAutho ( AUTO_AUTHO_FLAG          , &sTlvInfo, "P"  ,  1);
    PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, "MSC", 3);

   AuthoBuildTlv( sTlvBuffer, &sTlvInfo);
   nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
   if ( nRetCode != OK )
   {
     trace_event ( "Can Not Write In Authorisation table !!!", FATAL );
     PutTlvBuffer( AUTO_CUR_TABLE_INDICATOR , sTlvBuffer, "SAF" ,  3);
     nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
     if ( nRetCode != OK )
         trace_event("Can Not Write In Authorisation SAF table !!!", FATAL);

     PutTlvBuffer ( AUTO_EVENT_CODE , sTlvBuffer , MALFUNCTION_EVENT , 3);
     nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
     if ( nRetCode != OK )                                                                                                         
               PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
    else
     		   PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
     return ( SEVERE_ERROR );
   }

   return ( OK );
}
/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les reponses file update                            */
/****************************************************************************/
/****************************************************************************/

int FileUpdateReplyProcessing( int nIndiceCtx, TSIsoInfo *sIsoMsg, 
							   char *sCaptureCode, char *sRoutingCode ,
						       char  *sAuthoTable )
{
    int                           nRetCode;
    int                           length;
    char                         sTlvBuffer[ LG_MAX + 1 ];
    TSTlvAutho                   sTlvInfo;

  trace_event ( "Start FileUpdateReplyProcessing()", PROCESSING );
  memset ( sTlvBuffer , 0,  sizeof ( sTlvBuffer ));
  InitTlvAutho(&sTlvInfo);

  IsoToTlv    ( nIndiceCtx , sIsoMsg   , &sTlvInfo );
  PutTlvAutho ( AUTO_ROUTING_CODE, &sTlvInfo, sRoutingCode ,  6);
  PutTlvAutho ( AUTO_CAPTURE_CODE, &sTlvInfo, sCaptureCode,  6);
  PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable ,     3);
  PutTlvAutho ( AUTO_AUTHO_FLAG          , &sTlvInfo, "P" ,  1);
  length = AuthoBuildTlv( sTlvBuffer, &sTlvInfo);

   nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
   if ( nRetCode != OK )
   {
   		if ( nRetCode == MALFUNCTION )
        {
	      	PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
	      	InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
          	trace_event  ( "SEVERE ERROR !!!!!", PROCESSING );
          	trace_event  ( "Can Not Update Autho Activity File ", PROCESSING );
          	return ( SEVERE_ERROR );
        }
        else
        {
	      	PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
	      	InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
          	trace_event  ( "Can Not Update Autho Activity File .", PROCESSING );
          	trace_event  ( "Can Not Found Corresponding Request.", PROCESSING);
          	return ( NOK );
        }
   }
  
   trace_event ( "End FileUpdateReplyProcessing()", PROCESSING );
   return ( OK );
}

/****************************************************************************/
/****************************************************************************/
