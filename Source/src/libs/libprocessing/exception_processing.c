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

extern char  ResourceId         [ ];

/*********************************************************************************************************************************/
/* IssuerTimeOutProcessing                                                                                                       */
/* -----------------------                                                                                                       */
/* Description : This function process the authorisation request in stand in processing in case of the time out from issuer      */
/*               First it update the original request to be as time out request, then in case of the primary resource accepts    */
/*               to do stand in processing (Secondary resource is stand)                                                         */
/*               In case of stand in processing this function also insert in SAF table in case of decline or approuved advices   */
/*               Are checked for the destination resource                                                                        */
/* Entree	    :                                                                                                                */
/*              nIndexCtx : OracleContexte                                                                                       */
/*              sResourceId: Acquirer Resource Id                                                                                */
/*              sTlvBuffer: Original TLV data used in the on-line processing to the issuer                                       */
/*              IsoInfo   : Original ISO data used in the on-line processing to the issuer                                       */
/* Sortie	    :                                                                                                                */
/* Author/Date  : H. YOUSFI 05/06/2002                                                                                           */
/* Revision     : This Function is under validation with SystemMalfunctionProcessing To use the same stand in processing         */
/*********************************************************************************************************************************/
int IssuerTimeOutProcessing (	int nIndiceCtx, char *sMyResource,
	char *sTlvBuffer , TSIsoInfo *sIsoMsg  )
{

	TSTlvAutho     tlvInfo;
	/* TSTlvPrivate   TlvPrivInfo; */
	TSTlvPrivate    sTlvPrivateInfo;

	char            sTlvPrivateBuffer [ 999 + 1 ];
	char            tmpField [ 999 + 1 ];
	char		   sLine		[MAX_LINE_TRC];
	int     	   nLength;
	int 		   nEventCode =  OK;
	int 		   nRetCode;
	char 		   sEventCode  [ 3 + 1 ];
	char 		   sActionCode  [ 3 + 1 ];
	char 		   sPrimResource   [ 6 + 1 ];
	char 		   sSecResource    [ 6 + 1 ];
	char    	   sAlternAmount [ 64 + 1 ];
	char    	   sStan         [ 6 + 1 ];
	char    	   sMsgType      [ 4 + 1 ];

	trace_event("Start IssuerTimeOutProcessing()",PROCESSING);
	/******************************************************************************/
	/* 					Initialisation			*/
	/******************************************************************************/
	memset(sEventCode           , 0, sizeof(sEventCode));
	memset(sActionCode          , 0, sizeof(sActionCode));
	memset(sPrimResource   		, 0, sizeof(sPrimResource));
	memset(sSecResource   		, 0, sizeof(sSecResource));
	memset(sTlvPrivateBuffer    , 0, sizeof (sTlvPrivateBuffer ));

	InitTlvAutho ( &tlvInfo );
	AnalyseTlvAutho ( sTlvBuffer , &tlvInfo );

	PutTlvAutho (AUTO_EVENT_CODE    ,&tlvInfo , ISSUER_NOT_AVAILABLE_EVENT,3);
	PutTlvAutho (AUTO_RESPONSE_CODE ,&tlvInfo , ISSUER_UNAVAILABLE_ACTION ,3);
	/**********************************************************************/
	/* (61) Network Pos Data Issuer Response Code                         */

	/* Ce bout de code est en commentaire on a pas besoin
	InitTlvPrivate (&TlvPrivInfo);
	if ( GetTlvAutho ( AUTO_NETWORK_POS_DATA , &tlvInfo , sTlvPrivateBuffer ,&nLength) == SUCCES )
	AnalyseTlvPrivate ( tmpField , nLength , &TlvPrivInfo);

	PutTlvPrivate (RESPONSE_CODE_TAG, &TlvPrivInfo, "91" , 2);
	nLength = PrivateBuildTlv (tmpField, &TlvPrivInfo);
	if ( nLength > 0 )
	PutTlvAutho (AUTO_NETWORK_POS_DATA ,&tlvInfo , tmpField ,nLength);
	*/

	PutTlvAutho (AUTO_AUTHO_FLAG ,&tlvInfo , "P" ,1);
	PutTlvAutho ( AUTO_AUTHOR_SRC  , &tlvInfo, "E2" , 2);
	PutTlvAutho ( AUTO_MESSAGE_REASON_CODE , &tlvInfo, "1002" , 4 );

	memset(sTlvBuffer, 0, LG_MAX);
	AuthoBuildTlv( sTlvBuffer, &tlvInfo);

	/* Authorisation Stand In */

	if ( GetTlvAutho ( AUTO_ADD_DATA_PRIVATE , &tlvInfo , sTlvPrivateBuffer ,&nLength) != SUCCES )
	{
		TlvToIso    ( nIndiceCtx 		 ,&tlvInfo , sIsoMsg  , RESPONSE );
		trace_event("End   IssuerTimeOutProcessing(Field 48 Not Found In TLV)",FATAL);
		return ( NOK );
	}

	InitTlvPrivate(&sTlvPrivateInfo);
	AnalyseTlvPrivate( sTlvPrivateBuffer ,nLength,&sTlvPrivateInfo);
	if ( GetTlvPrivate(SECONDARY_RESOURCE_TAG ,&sTlvPrivateInfo ,sSecResource ,&nLength) != SUCCES )
	{
		TlvToIso    ( nIndiceCtx 		 ,&tlvInfo , sIsoMsg  , RESPONSE );
		memset(sTlvBuffer, 0, LG_MAX);
		AuthoBuildTlv( sTlvBuffer, &tlvInfo);
		AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg , sMyResource , "1002", 'S' );
		trace_event("End   IssuerTimeOutProcessing(Secondary Resource Not Found)",FATAL);
		return ( NOK );
	}

	if ( memcmp ( sSecResource , STAND_IN_RESOURCE , 6 ) != 0 )
	{
		TlvToIso    ( nIndiceCtx 		 ,&tlvInfo , sIsoMsg  , RESPONSE );
		memset(sTlvBuffer, 0, LG_MAX);
		AuthoBuildTlv( sTlvBuffer, &tlvInfo);
		nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
		sprintf(sLine,"End   IssuerTimeOutProcessing(Secondary Resource %s Not StandIn)",sSecResource);
		trace_event(sLine,PROCESSING);
		return(OK);
	}
	else
	{
		PrintTlvBuffer(sTlvBuffer);
		/* Traitement Stand In */
		nRetCode = AuthorProcessing ( nIndiceCtx, sMyResource, sTlvBuffer,sIsoMsg,"1002");

		/* SAF Insertion */
		/* The function SafProcessing Checks if the primary resource Accept to receive decline advices Or Approuved Advices to be sent */
		/* 
		Si la resource Primaire N'existe Pas Dans le TLV (Bug dans tous les cas) alors inserer cette transaction comme Stand in
		et il sera logue dans la table SAF et on poura l'identifié
		*/

		if ( GetTlvAutho ( AUTO_ROUTING_CODE , &tlvInfo , sPrimResource ,&nLength) != SUCCES )
		{
			memcpy(sPrimResource, STAND_IN_RESOURCE,6);
			trace_event("Primary Resource Not Found In TLV DATA",FATAL);
		}
		/*
		memset(sMsgType, 0, sizeof(sMsgType));
		GetTlvAutho ( AUTO_MSG_TYPE , &tlvInfo , sMsgType ,&nLength);
		memcpy(sMsgType + 2, "20",2);
		PutTlvBuffer(AUTO_MSG_TYPE,sTlvBuffer,sMsgType,4);
		*/
		nRetCode = AutSafProcessing(nIndiceCtx, sTlvBuffer,sMyResource,sPrimResource);
		sprintf(sLine,"End   IssuerTimeOutProcessing(StandInProcessing %d)",nRetCode);
		trace_event(sLine,PROCESSING);
		return(nRetCode);
	}

}
/*********************************************************************************************************************************/
/* SystemMalfunctionProcessing                                                                                                   */
/* -----------------------                                                                                                       */
/* Description : This function process the authorisation request in stand in processing in case of the time out from issuer      */
/*               First it update the original request to be as time out request, then in case of the primary resource accepts    */
/*               to do stand in processing (Secondary resource is stand)                                                         */
/*               In case of stand in processing this function also insert in SAF table in case of decline or approuved advices   */
/*               Are checked for the destination resource                                                                        */
/* Entree	    :                                                                                                                */
/*              nIndexCtx : OracleContexte                                                                                       */
/*              sResourceId: Acquirer Resource Id                                                                                */
/*              sTlvBuffer: Original TLV data used in the on-line processing to the issuer                                       */
/*              IsoInfo   : Original ISO data used in the on-line processing to the issuer                                       */
/* Sortie	    :                                                                                                                */
/* Author/Date  : H. YOUSFI 05/06/2002                                                                                           */
/* Revision     : This Function is under validation with SystemMalfunctionProcessing To use the same stand in processing         */
/*********************************************************************************************************************************/
int SystemMalfunctionProcessing (	int nIndiceCtx, char *sMyResource,
	char *sTlvBuffer , TSIsoInfo *sIsoMsg  )
{

	TSTlvAutho     tlvInfo;
	TSTlvPrivate    sTlvPrivateInfo;

	char            sTlvPrivateBuffer [ 999 + 1 ];
	char		   sLine		[MAX_LINE_TRC];
	int     	   nLength;
	int 		   nEventCode =  OK;
	int 		   nRetCode;
	char 		   sEventCode  [ 3 + 1 ];
	char 		   sActionCode  [ 3 + 1 ];
	char 		   sPrimResource   [ 6 + 1 ];
	char 		   sSecResource    [ 6 + 1 ];
	char    	   sAlternAmount [ 64 + 1 ];
	char    	   sStan         [ 6 + 1 ];
	char    	   sMsgType      [ 4 + 1 ];

	sprintf(sLine,"Start DynamicStandinProcessing(%s)",sMyResource);
	trace_event(sLine,PROCESSING);
	/******************************************************************************/
	/* 					Initialisation			*/
	/******************************************************************************/
	memset(sEventCode           , 0, sizeof(sEventCode));
	memset(sActionCode          , 0, sizeof(sActionCode));
	memset(sPrimResource   		, 0, sizeof(sPrimResource));
	memset(sSecResource   		, 0, sizeof(sSecResource));
	memset(sTlvPrivateBuffer    , 0, sizeof (sTlvPrivateBuffer ));

	InitTlvAutho ( &tlvInfo );
	AnalyseTlvAutho ( sTlvBuffer , &tlvInfo );

	PutTlvAutho (AUTO_EVENT_CODE    ,&tlvInfo , MALFUNCTION_EVENT ,3);
	PutTlvAutho (AUTO_RESPONSE_CODE ,&tlvInfo , MALFUNCTION_ACTION ,3);

	memset(sTlvBuffer, 0, LG_MAX);
	AuthoBuildTlv( sTlvBuffer, &tlvInfo);

	/* Authorisation Stand In */

	if ( GetTlvAutho ( AUTO_ADD_DATA_PRIVATE , &tlvInfo , sTlvPrivateBuffer ,&nLength) != SUCCES )
	{
		TlvToIso    ( nIndiceCtx 		 ,&tlvInfo , sIsoMsg  , RESPONSE );
		trace_event("End   DynamicStandinProcessing(Field 48 Not Found In TLV)",FATAL);
		return ( NOK );
	}

	InitTlvPrivate(&sTlvPrivateInfo);
	AnalyseTlvPrivate( sTlvPrivateBuffer ,nLength,&sTlvPrivateInfo);
	if ( GetTlvPrivate(SECONDARY_RESOURCE_TAG ,&sTlvPrivateInfo ,sSecResource ,&nLength) != SUCCES )
	{
		TlvToIso    ( nIndiceCtx 		 ,&tlvInfo , sIsoMsg  , RESPONSE );
		memset(sTlvBuffer, 0, LG_MAX);
		AuthoBuildTlv( sTlvBuffer, &tlvInfo);
		AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg , sMyResource , "1003", 'S' );
		trace_event("End   DynamicStandinProcessing(Secondary Resource Not Found)",FATAL);
		return ( NOK );
	}

	if ( memcmp ( sSecResource , STAND_IN_RESOURCE , 6 ) != 0 )
	{
		PutTlvAutho (AUTO_AUTHO_FLAG ,&tlvInfo , "P" ,1);
		PutTlvAutho ( AUTO_AUTHOR_SRC  , &tlvInfo, "E0" , 2);
		TlvToIso    ( nIndiceCtx 		 ,&tlvInfo , sIsoMsg  , RESPONSE );
		memset(sTlvBuffer, 0, LG_MAX);
		AuthoBuildTlv( sTlvBuffer, &tlvInfo);
		nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
		sprintf(sLine,"End   DynamicStandinProcessing(Secondary Resource %s Not StandIn)",sSecResource);
		trace_event(sLine,PROCESSING);
		return(OK);
	}
	else
	{
		PrintTlvBuffer(sTlvBuffer);
		PutTlvAutho (AUTO_AUTHO_FLAG ,&tlvInfo , "P" ,1);
		PutTlvAutho ( AUTO_AUTHOR_SRC  , &tlvInfo, "E2" , 2);
		PutTlvAutho ( AUTO_MESSAGE_REASON_CODE , &tlvInfo, "1003" , 4 );
		/* 
		Si la resource Primaire N'existe Pas Dans le TLV (Bug dans tous les cas) alors inserer cette transaction comme Stand in
		et il sera logue dans la table SAF et on poura l'identifié
		*/

		if ( GetTlvAutho ( AUTO_ROUTING_CODE , &tlvInfo , sPrimResource ,&nLength) != SUCCES )
		{
			memcpy(sPrimResource, STAND_IN_RESOURCE,6);
			trace_event("Primary Resource Not Found In TLV DATA",FATAL);
		}
		sPrimResource[6] = '\0';
		/* Traitement Stand In */
		nRetCode = CheckDynamicAdvice(nIndiceCtx, sPrimResource);
		if (nRetCode == OK)
		{
			nRetCode = AuthorProcessing ( nIndiceCtx, sMyResource, sTlvBuffer,sIsoMsg,"1003");
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
			if ( nRetCode != OK )
				memcpy(sActionCode,MALFUNCTION_ACTION,3);

			PutIsoField(ISO_RESPONSE_CODE,sIsoMsg,sActionCode ,3);
		}
		else
		{
			nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
			if ( nRetCode != OK )
			{
				PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
				InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
				trace_event  ( "Can Not Update Autho Activity File ", PROCESSING );
				return ( NOK );
			}
			nRetCode = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
		}

		/* SAF Insertion */
		/* The function SafProcessing Checks if the primary resource Accept to receive decline advices Or Approuved Advices to be sent */
		/*
		memset(sMsgType, 0, sizeof(sMsgType));
		GetTlvAutho ( AUTO_MSG_TYPE , &tlvInfo , sMsgType ,&nLength);
		memcpy(sMsgType + 2, "20",2);
		PutTlvBuffer(AUTO_MSG_TYPE,sTlvBuffer,sMsgType,4);
		*/
		nRetCode = AutSafProcessing(nIndiceCtx, sTlvBuffer,sMyResource,sPrimResource);
		InitTlvAutho ( &tlvInfo );
		AnalyseTlvAutho ( sTlvBuffer , &tlvInfo );
		sprintf(sLine,"End   DynamicStandinProcessing(StandInProcessing %d)",nRetCode);
		trace_event(sLine,PROCESSING);
		return(nRetCode);
	}

}
int AuthorProcessing(	int nIndiceCtx, char *sMyResource,
	char *sTlvBuffer , TSIsoInfo *sIsoMsg , char *sReasonCode)
{

	int nRetCode;
	char    	   sAlternAmount [ 64 + 1 ];

	trace_event("Start AuthorProcessing()",PROCESSING);

	memset(sAlternAmount, 0, sizeof(sAlternAmount));

	/******************************************************************************/
	/* 	    Lecture des flags de securite                                         */
	/******************************************************************************/
	DelTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer);
	PrintTlvBuffer(sTlvBuffer);
	if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "OFF" ) != OK )
	{
		nRetCode =   AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
			sMyResource , sReasonCode, 'S' );
		trace_event("End   AuthorProcessing(ERROR SECURITY FLAGS)",PROCESSING);
		return(nRetCode);
	}

	/******************************************************************************/
	/* 	    PreControl Procedure                                                  */
	/******************************************************************************/
	if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
	{
		nRetCode =  AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
			sMyResource , sReasonCode, 'S' );
		trace_event("End   AuthorProcessing(ERROR PRE-CONTROL)",PROCESSING);
		return(nRetCode);
	}


	/******************************************************************************/
	/* 	    Security Verfication                                                  */
	/******************************************************************************/
	if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
	{
		nRetCode =   AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
			sMyResource , sReasonCode, 'S' );
		trace_event("End   AuthorProcessing(ERROR CHECK SECURITY)",PROCESSING);
		return(nRetCode);
	}


	/******************************************************************************/
	/* 	    Check Limits                                                          */
	/******************************************************************************/
	if ( CheckLimits(  nIndiceCtx, "OFF", sTlvBuffer, sAlternAmount ) != OK )
	{
		nRetCode =  AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
			sMyResource , sReasonCode, 'S' );
		trace_event("End   AuthorProcessing(ERROR CHECK LIMITS )",PROCESSING);
		return(nRetCode);
	}

	/******************************************************************************/
	/* 	    Check Account                                                         */
	/******************************************************************************/
	if ( CheckAccAvailable   ( nIndiceCtx, sTlvBuffer ) != OK )
	{
		/* PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,MALFUNCTION_EVENT,3); */
		nRetCode =   AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
			sMyResource , sReasonCode, 'S' );
		trace_event("End   AuthorProcessing(ERROR CHECK AVAILABLE)",PROCESSING);
		return(nRetCode);
	}


	nRetCode = AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
		sMyResource , sReasonCode, 'S' );

	/******************************************************************************/
	/*  Amount Bill/Settlement Management(Already Calculated During Online Checks)*/
	/******************************************************************************/
	trace_event("End   AuthorProcessing(OK)",PROCESSING);
	return(OK);
}

