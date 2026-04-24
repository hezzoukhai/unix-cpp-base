
/*
+------------------------------------------------------------------------------+
|                                  HPS                                         |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | autho_loan_utils.c                                         |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     |                                                            |
+-----------------+------------------------------------------------------------+
| Programme       | shell                                                      |
+-----------------+---------------------------------+---------+----------------+
| Aplication      | Powercard                       | Module  | Front End      |
+-----------------+-----------------------------+---+---------+-----+----------+
| Created by      |  youssef KHALOUFI       | Date de Creation  | 25/10/2002   |
+-----------------+-----------------------------+-------------------+----------+
| Lst. mod.       |  El-Hadj OUADIE              26/12/02 10:14:58             |
+-----------------+------------------------------------------------------------+
| Public          |                                                            |
| Objects         |                                                            |
| of the source   |                                                            |
|                 |                                                            |
+-----------------+------------------------------------------------------------+
| Dependances     |                                                            |
+-----------------+------------------------------------------------------------+
*/
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

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
int  AuthorLoanLocal( int nIndiceCtx, char *sTlvBuffer , TSIsoInfo *sIsoMsg , 
                   char *sMyResource , char *sPrimResource , char SafIndicator )
{
   TSTlvAutho      sTlvInfo;
   char sActionCode [ 3 + 1 ];
   char  sAmount   [ 18+1];
   char  sCurrency [  3+1];
   char sEventCode  [ 3 + 1 ];
   int  nRetCode;
   int  nLength;
   
   memset ( sActionCode , 0, sizeof ( sActionCode ));
   memset ( sEventCode  , 0, sizeof ( sEventCode  ));
   memset (  sAmount    , 0, sizeof ( sAmount ));
   memset (  sCurrency  , 0, sizeof ( sCurrency ));
   
   PutTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, STAND_IN_RESOURCE ,  6);
   PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "P" ,  1);
   PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);

   nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
     if ( nRetCode != OK )                                                                                                         
               PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
     else
   			   PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);

   nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
   if ( nRetCode != OK )
   {
     trace_event ( "Can Not Write In Authorisation table !!!", FATAL );

     PutTlvBuffer( AUTO_CUR_TABLE_INDICATOR , sTlvBuffer, "SAF" ,  3);
     nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
     if ( nRetCode != OK )
         trace_event("Can Not Write In Authorisation SAF table !!!", FATAL);

     PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
     PutTlvBuffer ( AUTO_RESPONSE_CODE ,sTlvBuffer  , MALFUNCTION_ACTION , 3);
     nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
     if ( nRetCode != OK )                                                                                                         
               PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
     else
     			PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
     nRetCode = SEVERE_ERROR;
   }
   
  if ( memcmp ( sActionCode , APPROVED_ACTION , 3 ) != 0 )
  {
         if ( memcmp ( sActionCode , INVALID_PIN_ACTION , 3 ) != 0 )
              nRetCode = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
         else
              nRetCode = UpdateCardActivity( nIndiceCtx , PIN_ERROR , sTlvBuffer );
  }
  else
   nRetCode = UpdateCardActivity  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );

   if ( nRetCode != OK )
   {
     trace_event ( "Can Not Update Card Activity  !!!", FATAL );
     /* What is the action to do */
    }

  if ( memcmp ( sActionCode , APPROVED_ACTION , 3 ) == 0 )
  {
   
	nRetCode = SetLoanDataToPos  ( nIndiceCtx , sTlvBuffer );
    if ( nRetCode != OK ) 
  	{
		trace_event ( "Can Not Set Data to pos  !!!", FATAL ); 
    	/* What is the action to do */
	}
	nRetCode = UpdateLoan  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
	if ( nRetCode != OK )
	{
     trace_event ( "Can Not Update Loan  !!!", FATAL );
     /* What is the action to do */
    }
  }

    fprintf ( stderr,"\n Apres Card Activity SSSSSSSS \n");
    PrintTlvBuffer ( sTlvBuffer );
              
   if ( SafIndicator == 'S' )
        InsertionSaf ( nIndiceCtx , sTlvBuffer, sMyResource , sPrimResource);

   InitTlvAutho    ( &sTlvInfo );
   AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
   TlvToIso        ( nIndiceCtx , &sTlvInfo   , sIsoMsg  , RESPONSE );
   trace_event 	   ( "Autho Local End () ", PROCESSING ); 
   return ( nRetCode );
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
int  AuthorLoanUpdate( int nIndiceCtx, char *sTlvBuffer , TSIsoInfo *sIsoMsg , 
                   char *sMyResource , char *sMsgReason, char SafIndicator )
{
   TSTlvAutho      sTlvInfo;
   char sActionCode [ 3 + 1 ];
   char  sAmount   [ 18+1];
   char  sCurrency [  3+1];
   char sEventCode  [ 3 + 1 ];
   char sPrimResource [ 6 + 1 ];
   int  nRetCode;
   int  nLength;
   
   memset ( sPrimResource , 0 , sizeof ( sPrimResource ));
   memset ( sActionCode , 0, sizeof ( sActionCode ));
   memset ( sEventCode  , 0, sizeof ( sEventCode  ));
   memset ( sAmount    , 0, sizeof ( sAmount ));
   memset ( sCurrency  , 0, sizeof ( sCurrency ));
   
   PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "T" ,  1);
   PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);
   GetTlvBuffer ( AUTO_ROUTING_CODE  , sTlvBuffer, sPrimResource , &nLength );
   PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, sMsgReason , 4 );

   nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
     if ( nRetCode != OK )                                                                                                         
               PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
     else
   			   PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);

   nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
   if ( nRetCode != OK )
   {
     trace_event ( "Can Not Write In Authorisation table !!!", FATAL );

     PutTlvBuffer( AUTO_CUR_TABLE_INDICATOR , sTlvBuffer, "SAF" ,  3);
     nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
     if ( nRetCode != OK )
         trace_event("Can Not Write In Authorisation SAF table !!!", FATAL);

     PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
     PutTlvBuffer ( AUTO_RESPONSE_CODE ,sTlvBuffer  , MALFUNCTION_ACTION , 3);
     nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
     if ( nRetCode != OK )                                                                                                         
               PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
     else
     			PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
     nRetCode = SEVERE_ERROR;
   }
   
  if ( memcmp ( sActionCode , APPROVED_ACTION , 3 ) != 0 )
  {
         if ( memcmp ( sActionCode , INVALID_PIN_ACTION , 3 ) != 0 )
              nRetCode = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
         else
              nRetCode = UpdateCardActivity( nIndiceCtx , PIN_ERROR , sTlvBuffer );
  }
  else
   nRetCode = UpdateCardActivity  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
   if ( nRetCode != OK )
   {
     trace_event ( "Can Not Update Card Activity  !!!", FATAL );
     /* What is the action to do */
    }

  if ( memcmp ( sActionCode , APPROVED_ACTION , 3 ) != 0 )
  {
   
	nRetCode = SetLoanDataToPos  ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK ) 
  	{
     	trace_event ( "Can Not Set Data to pos  !!!", FATAL ); 
    	/* What is the action to do */
    }
	nRetCode = UpdateLoan  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
	if ( nRetCode != OK )
	{
     trace_event ( "Can Not Update Loan  !!!", FATAL );
     /* What is the action to do */
    }
  }

                 
   if ( SafIndicator == 'S' )
        InsertionSaf ( nIndiceCtx , sTlvBuffer, sMyResource , sPrimResource);

   InitTlvAutho    ( &sTlvInfo );
   AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
   TlvToIso        ( nIndiceCtx , &sTlvInfo   , sIsoMsg  , RESPONSE );
   trace_event 	   ( "Autho Local Update () ", PROCESSING ); 
   return ( nRetCode );
}
