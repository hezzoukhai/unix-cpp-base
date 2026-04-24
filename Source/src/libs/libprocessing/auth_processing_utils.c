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
int  AuthorLocal( int nIndiceCtx, char *sTlvBuffer , TSIsoInfo *sIsoMsg , 
	char *sMyResource , char *sPrimResource , char SafIndicator )
{
	TSTlvAutho      		sTlvInfo;
	char sActionCode 	[ 3 + 1 ];
	char  sAmount   		[ 18+1];
	char  sCurrency 		[  3+1];
	char  sLine 			[  MAX_LINE_TRC+1];
	char  tmpField 		[  MAX_LINE_TRC+1];
	char  sMsgType  		[  4+1];
	char sEventCode  	[ 3 + 1 ];
	int  nRetCode;
	int  nLength;
        char tag_AVS[ 3];                    /* ENH 11.1 OHD10052011 */
	char szWork[256];
	struct timeval time_s_aut;
	/*Start ENH OCT 8.2 SWI020908*/
	char    PosCond[3];
	int     Account_verif_flag;
	TSTlvPrivate   TlvInfo;
        char       AddPos[26 +1];            /* ENH 13.2 OAD28082013 */
	/*End ENH OCT 8.2 SWI020908*/
	int  ret;
	char sProcCode[56];/**MKB070508**/

	memset ( sActionCode , 0, sizeof ( sActionCode ));
	memset ( sEventCode  , 0, sizeof ( sEventCode  ));
	memset (  sAmount    , 0, sizeof ( sAmount ));
	memset (  sCurrency  , 0, sizeof ( sCurrency ));
	memset (  tmpField  , 0, sizeof ( tmpField ));
	memset (  sLine  , 0, sizeof ( sLine ));
	memset (  szWork  , 0, sizeof ( szWork ));
	memset ( sProcCode  , 0, sizeof ( sProcCode )); /**MKB070508**/


	if ( GetTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, tmpField ,  &nLength) != SUCCESS) 
		PutTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, STAND_IN_RESOURCE ,  6); 
	PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "P" ,  1);
	PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);
	/*Start ENH OCT 8.2 SWI020908*/
	memset ( PosCond,  0 , sizeof ( PosCond ));
	Account_verif_flag = NOK;
	if (GetTlvBuffer (AUTO_NETWORK_POS_DATA, sTlvBuffer, tmpField, &nLength) == SUCCES)
	{
		InitTlvPrivate(&TlvInfo);
		AnalyseTlvPrivate(tmpField, nLength, &TlvInfo);
		if (GetTlvPrivate(POS_CONDITION_CODE_TAG, &TlvInfo, PosCond, &nLength) == SUCCES) 
		{
			if(memcmp(PosCond,"51",2) == 0) Account_verif_flag = OK;
			sprintf (  sLine , "ACCOUNT VERIFICATION2 ENH8.2 ...%d ", Account_verif_flag  );
			trace_event( sLine , PROCESSING );
		}
	}
	/*End ENH OCT 8.2 SWI020908*/
	nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
	if ( nRetCode != OK )                                                                                                         
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
	else
	{ /*Start ENH OCT 8.2 SWI020908*/
		if((memcmp(sActionCode, APPROVED_ACTION,3) == 0) && (Account_verif_flag == OK) ) /*SWI ENH020908*/
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,NO_REASON_TO_DECLINE ,3);
		else
			/*End ENH OCT 8.2 SWI020908*/
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);
	} 
	/* Update field 72 with the time Before autho_activity_insert Start HY 141002 */
	if(  GetTlvBuffer(AUTO_UF_FILE_RECORD,sTlvBuffer,tmpField,&nLength) == SUCCES)
	{
	   if (GetIsoField(ISO_PROC_CODE, sIsoMsg, sProcCode, &nLength) == SUCCESS)
		  if ( (memcmp(sProcCode, "95", 2) != 0) && (memcmp(sProcCode, "50", 2) != 0)) /* UBP RJE04122014 */
		  {
			  gettimeofday (&time_s_aut,NULL);
			  sprintf(szWork, "%010ld%06d",time_s_aut.tv_sec, time_s_aut.tv_usec);
			  memcpy(tmpField + 16, szWork, 16);
			  PutTlvBuffer ( AUTO_UF_FILE_RECORD , sTlvBuffer, tmpField ,strlen(tmpField)); 
		   }/* UBP RJE04122014 */
		   
	   
	}


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
	if (Account_verif_flag != OK) /*ENH OCT 8.2 SWI020908*/
	{   
		if (( memcmp ( sActionCode , APPROVED_ACTION , 3 ) != 0 )
			&& ( memcmp ( sActionCode , UPDATE_TRACK3 , 3 ) != 0 )) /*YK : 060303*/
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

		/* MKB070508 : si pin change, pas de maj de bal ou de settlement */
		if (GetIsoField(ISO_PROC_CODE, sIsoMsg, sProcCode, &nLength) == SUCCESS)
			if ((memcmp(sProcCode, "91", 2) == 0)) /* 91 => pin change */
			{
				InitTlvAutho    ( &sTlvInfo );
				AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
				TlvToIso        ( nIndiceCtx , &sTlvInfo   , sIsoMsg  , RESPONSE );
				sprintf(sLine,"End   AuthorLocal(PIN CHANGE  %d)",nRetCode);
				trace_event(sLine,PROCESSING);
				return(OK);
			}
			/* End MKB070508 : si pin change, pas de maj de bal ou de settlement */

			if ( ( ( memcmp ( sActionCode , APPROVED_ACTION , 3 ) == 0 )
				|| ( memcmp ( sActionCode , UPDATE_TRACK3 , 3 ) == 0 ) ) 
				&& (memcmp(sProcCode, "21", 2) != 0)) /*Cash deposit****/ /**MKB011209**/ 

			{
				nRetCode = UpdateBalance  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
				if ( nRetCode != OK )
				{
					trace_event ( "Can Not Update Balance  !!!", FATAL );
					/* What is the action to do */
				}
			}
			/* Mise a jour données settlement */
			if ( !memcmp ( sActionCode , APPROVED_ACTION , 3 ))
				UpdateSettlement( nIndiceCtx , sTlvBuffer);

			ret = PrintTlvBuffer ( sTlvBuffer ); /**Linux **ret** MKB100907**/

			if ( SafIndicator == 'S' )
			{
				/*        InsertionSaf ( nIndiceCtx , sTlvBuffer, sMyResource , sPrimResource); */
				/* SAF Insertion */
				/* The function SafProcessing Checks if the primary resource Accept to receive decline advices Or Approuved Advices to be sent */
				/*
				memset(sMsgType, 0, sizeof(sMsgType));
				GetTlvBuffer ( AUTO_MSG_TYPE , sTlvBuffer , sMsgType ,&nLength);
				memcpy(sMsgType + 2, "20",2);
				PutTlvBuffer(AUTO_MSG_TYPE,sTlvBuffer,sMsgType,4);
				*/
				nRetCode = AutSafProcessing(nIndiceCtx, sTlvBuffer,sMyResource,sPrimResource);
			}

	}/*ENH OCT 8.2 SWI020908*/
	InitTlvAutho    ( &sTlvInfo );
	ret = AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );  /**Linux **ret** MKB100907**/

	ret = TlvToIso( nIndiceCtx , &sTlvInfo   , sIsoMsg  , RESPONSE ); /**Linux **ret** MKB100907**/

	/** PA DSS**/
	InitTlvAutho    ( &sTlvInfo );
	InitTlvPrivate(&TlvInfo);
	memset ( sActionCode , 0, sizeof ( sActionCode ));
	memset ( sEventCode  , 0, sizeof ( sEventCode  ));
	memset (  sAmount    , 0, sizeof ( sAmount ));
	memset (  sCurrency  , 0, sizeof ( sCurrency ));
	memset (  tmpField  , 0, sizeof ( tmpField ));
	memset (  sLine  , 0, sizeof ( sLine ));
	memset (  szWork  , 0, sizeof ( szWork ));
	memset ( sProcCode  , 0, sizeof ( sProcCode ));

   /*PutTlvBuffer ( AUTO_SECURITY_VERIF_FLAGS, sTlvBuffer, "                    " ,  32); ICH25112013*/
         /* OHR19122014 BUG PROD00011746 */
/*      PutTlvBuffer ( AUTO_SECURITY_VERIF_FLAGS, sTlvBuffer, "                    " ,  32); */
        memset(szWork,' ',32);
        PutTlvBuffer ( AUTO_SECURITY_VERIF_FLAGS, sTlvBuffer, szWork ,  32);
        /* OHR19122014 BUG PROD00011746 */
	sprintf(sLine,"End   AuthorLocal(%d)",nRetCode);
	trace_event(sLine,PROCESSING);

	return ( nRetCode );
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
int  AuthorUpdate( int nIndiceCtx, char *sTlvBuffer , TSIsoInfo *sIsoMsg , 
	char *sMyResource , char *sMsgReason, char SafIndicator/* Ce flag N'est plus utilisé */ )
{
	TSTlvAutho      sTlvInfo;
	char sActionCode [ 3 + 1 ];
	char  sAmount   [ 18+1];
	char  sCurrency [  3+1];
	char sEventCode  [ 3 + 1 ];
	char sPrimResource [ 6 + 1 ];
	int  nRetCode;
	int  nLength;
	char tmpField[256];
	char szWork[256];
	struct timeval time_s_aut;
	char	sTimeField[256];
	int	nTimeLen;


	memset ( sPrimResource , 0 , sizeof ( sPrimResource ));
	memset ( sActionCode , 0, sizeof ( sActionCode ));
	memset ( sEventCode  , 0, sizeof ( sEventCode  ));
	memset ( sAmount    , 0, sizeof ( sAmount ));
	memset ( sCurrency  , 0, sizeof ( sCurrency ));
	memset (  szWork  , 0, sizeof ( szWork ));
	memset (  tmpField  , 0, sizeof ( tmpField ));
	memset (  sTimeField  , 0, sizeof ( sTimeField ));

	PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "P" ,  1);
	PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);
	GetTlvBuffer ( AUTO_ROUTING_CODE  , sTlvBuffer, sPrimResource , &nLength );
	PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, sMsgReason , 4 );

	nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
	if ( nRetCode != OK )                                                                                                         
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
	else
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);

	/* Update field 72 with the time Before autho_activity_insert Start HY 141002 */
	nTimeLen = 0;
	if(  GetTlvBuffer(AUTO_UF_FILE_RECORD,sTlvBuffer,tmpField,&nLength) == SUCCES)
	{
		gettimeofday (&time_s_aut,NULL);
		sprintf(szWork, "%010ld%06d",time_s_aut.tv_sec, time_s_aut.tv_usec);
		memcpy(tmpField + 32, szWork, 16);
		PutTlvBuffer ( AUTO_UF_FILE_RECORD , sTlvBuffer, tmpField , nLength );
		memcpy(sTimeField, tmpField,nLength);
		sTimeField[nLength] = '\0';
		nTimeLen = nLength;

	}
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
		nRetCode = UpdateBalance  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
		if ( nRetCode != OK )
		{
			trace_event ( "Can Not Update Balance  !!!", FATAL );
			/* What is the action to do */
		}
	}

	if ( !memcmp ( sActionCode , APPROVED_ACTION , 3 ))
		UpdateSettlement( nIndiceCtx , sTlvBuffer);

	/* Ne sera pas gerer  ce niveau 
	il doit etre gerer soit apres issuer time out/ System Malfunction time out ou prim resource not availables
	Voir exception_processing for time out and system malfunction and autho_processing pour l'autre cas
	if ( SafIndicator == 'S' )
	InsertionSaf ( nIndiceCtx , sTlvBuffer, sMyResource , sPrimResource);
	*/

	InitTlvAutho    ( &sTlvInfo );
	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	TlvToIso        ( nIndiceCtx , &sTlvInfo   , sIsoMsg  , RESPONSE );
	if ( nTimeLen > 0 )
		PutIsoField( ISO_RECORD_DATA, sIsoMsg,sTimeField ,nTimeLen );

	trace_event 	   ( "Autho Local Update () ", PROCESSING ); 
	return ( nRetCode );
}

/*************************************************************************************/
/*************************************************************************************/
int  RefundProcessing( int nIndiceCtx, char *sTlvBuffer , TSIsoInfo *sIsoMsg  )
{
	TSTlvAutho      sTlvInfo;
	char sActionCode [ 3 + 1 ];
	char  sAmount   [ 18+1];
	char  sCurrency [  3+1];
	char sEventCode  [ 3 + 1 ];
	int  nRetCode;
	int  nLength;

	trace_event 	   ( "Start RefundProcessing () ", PROCESSING ); 

	memset ( sActionCode , 0, sizeof ( sActionCode ));
	memset ( sEventCode  , 0, sizeof ( sEventCode  ));
	memset (  sAmount    , 0, sizeof ( sAmount ));
	memset (  sCurrency  , 0, sizeof ( sCurrency ));

	PutTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, STAND_IN_RESOURCE ,  6);
	PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "P" ,  1);
	PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);
	PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,APPROVED_ACTION ,3);
	nRetCode = GetAuthNumber ( nIndiceCtx , sTlvBuffer );

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

	InitTlvAutho    ( &sTlvInfo );
	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	TlvToIso        ( nIndiceCtx , &sTlvInfo   , sIsoMsg  , RESPONSE );
	trace_event 	   ( "End   RefundProcessing (OK) ", PROCESSING ); 
	return ( nRetCode );
}
/******************************************************************************/
/******************************************************************************/
int  AuthorLocalAdm( int nIndiceCtx, char *sTlvBuffer , TSIsoInfo *sIsoMsg , 
	char *sMyResource , char *sPrimResource , char SafIndicator )
{
	TSTlvAutho      sTlvInfo;
	char sActionCode [ 3 + 1 ];
	char  sAmount   [ 18+1];
	char  sCurrency [  3+1];
	char sEventCode  [ 3 + 1 ];
	int  nRetCode;
	int  nLength;
	char sPCode      	[ LG_MAX ];
	trace_event("Start AuthorLocalAdm()",PROCESSING); 
	memset (  sPCode      	, 0 , sizeof ( sPCode ));
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

	if ( GetTlvBuffer ( AUTO_PROCESSING_CODE      , sTlvBuffer , sPCode , &nLength ) != SUCCES)
		trace_event("Get Processing Code Fieled ",PROCESSING);

	if ( (memcmp ( sActionCode , APPROVED_ACTION , 3 ) != 0 )
		&& ( memcmp ( sActionCode , UPDATE_TRACK3 , 3 ) != 0 ))
	{
		if ( memcmp ( sActionCode , INVALID_PIN_ACTION , 3 ) != 0 )
			nRetCode = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
		else
			nRetCode = UpdateCardActivity( nIndiceCtx , PIN_ERROR , sTlvBuffer );
	}
	else
		if ( memcmp(sPCode+6,"30",2) != 0 )
			nRetCode = UpdateCardActivity  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );

	if ( nRetCode != OK )
	{
		trace_event ( "Can Not Update Card Activity  !!!", FATAL );
		/* What is the action to do */
	}

	if ( SafIndicator == 'S' )
		InsertionSaf ( nIndiceCtx , sTlvBuffer, sMyResource , sPrimResource);

	InitTlvAutho    ( &sTlvInfo );
	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	TlvToIso        ( nIndiceCtx , &sTlvInfo   , sIsoMsg  , RESPONSE );
	trace_event 	   ( "End AuthorLocalAdm () ", PROCESSING ); 
	return ( nRetCode );
}

/**************************************************************************************/
/* Start ENH 12.2 SLA 200712 Fast Funds */ 
int  OctAuthorLocal( int nIndiceCtx, char *sTlvBuffer , TSIsoInfo *sIsoMsg ,
	char *sMyResource , char *sPrimResource , char SafIndicator )
{
	TSTlvAutho                   sTlvInfo;
	char sTlvVOutlet [ LG_MAX + 1 ];
	char sActionCode     [ 3 + 1 ];
	char  sLine                  [  MAX_LINE_TRC+1];
	char  tmpField               [  MAX_LINE_TRC+1];
	char sEventCode      [ 3 + 1 ];
	int  nRetCode;
	int  nLength;
	char szWork[256];
	struct timeval time_s_aut;

	memset ( sActionCode , 0, sizeof ( sActionCode ));
	memset ( sEventCode  , 0, sizeof ( sEventCode  ));
	memset (  tmpField  , 0, sizeof ( tmpField ));
	memset (  sLine  , 0, sizeof ( sLine ));
	memset(szWork, 0, sizeof(szWork));
	memset(sTlvVOutlet, 0, sizeof(sTlvVOutlet));
	memset ((char *)&time_s_aut , 0, sizeof ( time_s_aut ));

	if ( GetTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, tmpField ,  &nLength) != SUCCESS)
		PutTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, STAND_IN_RESOURCE ,  6);
	PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "P" ,  1);
	PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);

	nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
	if ( nRetCode != OK )
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
	else
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);
	/* Update field 72 with the time Before autho_activity_insert Start HY 141002 */
	if(  GetTlvBuffer(AUTO_UF_FILE_RECORD,sTlvBuffer,tmpField,&nLength) == SUCCES)
	{
		gettimeofday (&time_s_aut,NULL);
		sprintf(szWork, "%010ld%06ld",time_s_aut.tv_sec, time_s_aut.tv_usec);
		memcpy(tmpField + 16, szWork, 16);
		PutTlvBuffer ( AUTO_UF_FILE_RECORD , sTlvBuffer, tmpField , nLength );
	}
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
		nRetCode  = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
	else
		nRetCode = UpdateCardActivity  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );

	if ( nRetCode != OK )
	{
		trace_event ( "Can Not Update Card Activity  !!!", FATAL );
	}
	if ( memcmp ( sActionCode , APPROVED_ACTION , 3 ) == 0 )
	{
		nRetCode = OctAccountDeposit(nIndiceCtx, sTlvBuffer);
		if ( nRetCode != OK )
		{  trace_event  ( "Error On  OctAccountDeposit!! ", PROCESSING );
		return (NOK);
		}
	}   
	/* Mise a jour données settlement */
	if ( !memcmp ( sActionCode , APPROVED_ACTION , 3 ))
		UpdateSettlement( nIndiceCtx , sTlvBuffer);
	PrintTlvBuffer ( sTlvBuffer );
	if ( SafIndicator == 'S' )
	{
		nRetCode = AutSafProcessing(nIndiceCtx, sTlvBuffer,sMyResource,sPrimResource);
	}
	InitTlvAutho    ( &sTlvInfo );
	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	if (TlvToIso(nIndiceCtx , &sTlvInfo   , sIsoMsg  , RESPONSE ) != OK)
	{
		trace_event ("TlvToIso Error ...",ERROR);
		PutIsoField( ISO_RESPONSE_CODE, sIsoMsg, MALFUNCTION_ACTION , 3);
		nRetCode = SEVERE_ERROR;
	}
	sprintf(sLine,"End   OctAuthorLocal(%d)",nRetCode);
	trace_event(sLine,PROCESSING);

	return ( nRetCode );
}
/* End ENH 12.2 SLA 200712 Fast Funds */


