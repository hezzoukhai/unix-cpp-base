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


int BicIssuerDelegationProcessing       (       int nIndiceCtx,
	char *sMyResource,
	char *sAuthoTable,
	char *sAcquirerBank,
	char *InternationalRoutCheck,
	char *sServGatewayFlag,
	char *sBillCalculFlag,
	char *sTlvBuffer ,
	TSIsoInfo *IsoInfo
	)
{

	TSResources     sResource;
	TSTlvAutho      sTlvInfo;
	char            sSecurData [ LG_MAX ];
	char                sSecurityResults[ 11 ];
	char            tmp_buffer [ LG_MAX ];
	TSService       rService;
	char            slCardNumber[     22 + 1 ];
	char            slProcCode      [      6 + 1 ];
	char            slFuncCode      [      3 + 1 ];
	char            sLine           [MAX_LINE_TRC];
	int         nLength;
	int             nEventCode =  OK;
	int             nRetCode;
	char            sEventCode  [ 3 + 1 ];
	char            sPrimResource  [ 6 + 1 ];
	char            sSecResource   [ 6 + 1 ];
	pid_t           nPid;
	char            sPid           [ 6 + 1 ];
	char            sInternalStan  [ 6 + 1 ];
	char            cOnOffMode;
	char        sAlternAmount [ LG_MAX + 1 ];
	char        sTlvProcCode [ LG_MAX + 1 ];
	char        sIsoProcCode [ LG_MAX + 1 ];

	/*YK130104*/
	char v_action[3 + 1];
	char v_routing_code[6 + 1];
	/*YK130104*/
	int Ctrl;
	char vActionCode[3 + 1];

	trace_event("Start BicIssuerDelegationProcessing()",PROCESSING);
	/******************************************************************************/
	/*                                      Initialisation                  */
	/******************************************************************************/

	memset(v_action                         , 0, sizeof(v_action));
	memset(sSecurData                       , 0, sizeof(sSecurData));
	memset(slCardNumber                     , 0, sizeof(slCardNumber));
	memset(slProcCode                       , 0, sizeof(slProcCode));
	memset(slFuncCode                       , 0, sizeof(slFuncCode));
	memset((char *)&rService    , 0, sizeof(rService));
	memset(sEventCode           , 0, sizeof(sEventCode));
	memset(sPrimResource            , 0, sizeof(sPrimResource));
	memset(sSecResource             , 0, sizeof(sSecResource));
	memset(sPid                 , 0, sizeof(sPid));
	memset(sSecurityResults , 0,  sizeof ( sSecurityResults ));
	memset(sInternalStan        , 0, sizeof(sInternalStan));
	memset(sAlternAmount , 0,  sizeof ( sAlternAmount ));


	/******************************************************************************/
	/*  Formatter le buffer TLV A partir du message Iso                           */
	/******************************************************************************/
	InitTlvAutho(&sTlvInfo);
	nRetCode = IsoToTlv ( nIndiceCtx , IsoInfo , &sTlvInfo );
	if ( nRetCode != OK )
	{
		trace_event ( "IsoToTlv Error ...", ERROR );
		PutIsoField ( ISO_RESPONSE_CODE , IsoInfo , MALFUNCTION_ACTION , 3 );
		trace_event("End   BicDelegationProcessing(NOK IsoToTlv)",PROCESSING);
		return ( NOK );
	}

	PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable ,     3);
	PutTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sMyResource ,     6);
	PutTlvAutho ( AUTO_ACQUIRER_BANK , &sTlvInfo , sAcquirerBank ,     6);
	PutTlvAutho ( AUTO_INTER_ROUT_CHECK,&sTlvInfo,InternationalRoutCheck,1);
	PutTlvAutho ( AUTO_ROUTING_CODE, &sTlvInfo , STAND_IN_RESOURCE ,  6);
	PutTlvAutho ( AUTO_AUTHOR_SRC  , &sTlvInfo, "E0" , 2);
	PutTlvAutho ( AUTO_MESSAGE_REASON_CODE , &sTlvInfo, "6001", 4 );

	AuthoBuildTlv( sTlvBuffer, &sTlvInfo);
	PrintTlvBuffer ( sTlvBuffer );

	/******************************************************************************/
	/*  Recherche si Authorisation est Duplicate                                  */
	/******************************************************************************/
	nRetCode = GetOriginalDataElements(  nIndiceCtx , CAPTURE_MODE ,
		sTlvBuffer ,
		sPrimResource );

	if ( nRetCode == OK )
	{
		PrintTlvBuffer ( sTlvBuffer );
		trace_event ( "Found a Duplicate Request. ", PROCESSING );
		/* In case of Duplicate, over-write ACTION_CODE & AUTHORIZATION_CODE */
		PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, "308", 3);
		PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, "      ", 6);
		/* In case of Duplicate, over-write ACTION_CODE & AUTHORIZATION_CODE */
		InitTlvAutho    ( &sTlvInfo );
		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
		trace_event("End   BicDelegationProcessing(DUPLICATE TRX)",PROCESSING);
		return ( OK );

	}

	/******************************************************************************/
	/*          Controle des champs  TLV                                              */
	/******************************************************************************/
	if ( CheckReqTlvFields ( nIndiceCtx, sTlvBuffer ) != OK )
	{
		PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, "909", 3);
		InitTlvAutho    ( &sTlvInfo );
		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
		trace_event("End   BicIssuerDelegationProcessing(ChekcTlv)",PROCESSING);
		return ( NOK );
	}

	/******************************************************************************/
	/*              Load Issuer Data Info Including Routing Info                    */
	/******************************************************************************/


	PrintTlvBuffer ( sTlvBuffer );
	nRetCode = LoadIssuerData (  nIndiceCtx          ,
		sTlvBuffer          ,
		InternationalRoutCheck ,
		sPrimResource ,
		sSecResource
		);
	PrintTlvBuffer ( sTlvBuffer );

	sprintf (  sLine , "Primary Resource    ...%.6s ", sPrimResource );
	trace_event( sLine , PROCESSING );
	sprintf (  sLine , "Secondarry Resource ...%.6s ", sSecResource  );
	trace_event( sLine , PROCESSING );
	if ( nRetCode != OK )
	{
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
		if ( nRetCode != OK )
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
		else
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
		InitTlvAutho    ( &sTlvInfo );
		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
		trace_event("End   BicIssuerDelegationProcessing(Err LoadIssuerData)",PROCESSING);
		return(NOK);
	}

	PutTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource ,  6);

	/****************************************************************************/

	/******************************************************************************/
	/*                                        Controle du service /r ressource   */
	/******************************************************************************/
	GetIsoField (ISO_CARD_NBR ,     IsoInfo, slCardNumber ,&nLength);
	GetIsoField (ISO_PROC_CODE,     IsoInfo, slProcCode   ,&nLength);
	GetIsoField (ISO_FUNCTION_CODE, IsoInfo, slFuncCode   ,&nLength);
	sprintf(rService.szMti, "%04d", IsoInfo->nMsgType);

	nLength = 0;
	memcpy(rService.szProcCode, slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szSrcAcc,       slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szDestAcc,      slProcCode + nLength, 2);
	memcpy(rService.szFctCode , slFuncCode , 3);

	nEventCode = CheckResourceServices ( nIndiceCtx, sMyResource, &rService );
	if (nEventCode != OK)
	{
		sprintf ( sEventCode , "%03d", nEventCode );
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode ,     3);
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
		if ( nRetCode != OK )
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
		else
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
		InitTlvAutho    ( &sTlvInfo );
		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
		trace_event("End   BicIssuerDelegationProcessing(Err CheckResource)",PROCESSING);
		return(NOK);
	}

	/******************************************************************************/
	/*                                        Delegation    SMT                                   */
	/******************************************************************************/
	/* if (memcmp(sPrimResource, sMyResource , 6) != 0) */ /* Routage vers la SMT */
	if (memcmp(sSecResource, "150000" , 6) == 0) /* Routage vers la SMT */
	{
		sprintf(sLine," Routage vers %s", sSecResource);
		trace_event(sLine,TRACE);
		/******************************************************************************/
		/*          Lecture des flags de securite                                         */
		/******************************************************************************/
		if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "ON" ) != OK )
		{
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
			if ( nRetCode != OK )
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
			else
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event("End   BicIssuerDelegationProcessing(Err GetSecurityFlags)",PROCESSING);
			return(NOK);
		}

		/******************************************************************************/
		/*          PreControl Procedure                                                  */
		/******************************************************************************/
		if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
		{
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
			if ( nRetCode != OK )
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
			else
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event("End   BicIssuerDelegationProcessing(Err PreControl)",PROCESSING);
			return(NOK);
		}

		/******************************************************************************/
		/*          Security Verfication                                                  */
		/******************************************************************************/
		if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
		{
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
			if ( nRetCode != OK )
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
			else
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event("End   BicIssuerDelegationProcessing(Err CheckSecurity)",PROCESSING);
			return(NOK);
		}

		/******************************************************************************/
		/*  Amount Bill/Settlement Management                                         */
		/******************************************************************************/
		PrintTlvBuffer ( sTlvBuffer );
		nRetCode = AmountMng (    nIndiceCtx          ,
			sServGatewayFlag    ,
			sBillCalculFlag     ,
			sTlvBuffer
			);
		if ( nRetCode != OK )
		{
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
			if ( nRetCode != OK )
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
			else
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event ( "Retour AmountMng NOK ", PROCESSING );
			trace_event("End   BicIssuerDelegationProcessing(Err AmntMng)",PROCESSING);
			return(NOK);
		}

		PrintTlvBuffer ( sTlvBuffer );

		/******************************************************************************/
		/*          Check Limits                                                          */
		/******************************************************************************/
		if ( CheckLimits(  nIndiceCtx, "ON", sTlvBuffer, sAlternAmount ) != OK )
		{
			/*YK01062007: pour ne pas envoyer la reponse reçu de la SMC*/
			char sActionCode[3 + 1];
			memset(sActionCode, 0,sizeof(sActionCode));
			memcpy(sActionCode, "999", 3);
			if (GetIsoField (ISO_RESPONSE_CODE,     IsoInfo, sActionCode   ,&nLength) == SUCCESS)
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer, "116",3);
			else
			{
				nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
				if ( nRetCode != OK )
					PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
				else
					PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			}
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event("End   BicIssuerDelegationProcessing(Err CheckLimits)",PROCESSING);
			return(NOK);
		}

		nRetCode = CheckResource ( nIndiceCtx, sSecResource , &nPid );
		if ( ( nRetCode == OK ) && ( kill ( nPid , 0 ) == 0  || !memcmp(ResourceId,VRS_RESOURCE,6) ) )
			/* Le check sur la resource VRS est ajoute car ca appartient a un autre user */
		{
			memset  ( sPid , 0 , sizeof ( sPid ));
			sprintf ( sPid , "%06d",  nPid );
			memset(sSecurData, 0, sizeof(sSecurData));
			GetTlvBuffer   (AUTO_SECURITY_DATA,sTlvBuffer,sSecurData,&nLength);
			PutSecurityData(DESTINATION_PID,sSecurData, sPid,6);
			PutSecurityData(DESTINATION_RESOURCE,sSecurData,sSecResource,6);
			PutTlvBuffer(AUTO_SECURITY_DATA,sTlvBuffer,sSecurData,nLength);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo , REQUEST );
			/* PutTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, sSecResource ,  6); */
			trace_event ( "Primary Ressource Up ", PROCESSING );
			trace_event("End   BicIssuerDelegationProcessing(ROUTAGE)",PROCESSING);
			return(ROUTAGE);

		}
		else
		{
			trace_event ( "Primary Ressource Down ", PROCESSING );
			PutTlvBuffer (AUTO_EVENT_CODE    ,sTlvBuffer , ISSUER_NOT_AVAILABLE_EVENT,3);
			PutTlvBuffer (AUTO_RESPONSE_CODE ,sTlvBuffer, ISSUER_UNAVAILABLE_ACTION ,3);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event("End   BicIssuerDelegationProcessing(PAS ROUTAGE)",PROCESSING);
			return(NOK);
		}
	}

	if ( memcmp ( sSecResource , NO_ROUTING_RESOURCE , 6 ) == 0 )
	{
		PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT,3);
		PutTlvBuffer (AUTO_RESPONSE_CODE ,sTlvBuffer, ISSUER_UNAVAILABLE_ACTION ,3);
		InitTlvAutho    ( &sTlvInfo );
		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
		trace_event("End   BicIssuerDelegationProcessing(Err CheckResource)",PROCESSING);
		return(NOK);
	}

	/******************************************************************************/
	/*                                        le Traitement StandIn                               */
	/******************************************************************************/

	if ( memcmp ( sSecResource , STAND_IN_RESOURCE , 6 ) == 0 )
	{
		trace_event ( "Sec : STAND_IN AUTHORISATION PROCESSING", PROCESSING );
		/******************************************************************************/
		/*          Lecture des flags de securite                                         */
		/******************************************************************************/
		if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "OFF" ) != OK )
		{
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
			if ( nRetCode != OK )
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
			else
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event("End   BicIssuerDelegationProcessing(Err GetSecurity)",PROCESSING);
			return(NOK);
		}

		/******************************************************************************/
		/*          PreControl Procedure                                                  */
		/******************************************************************************/
		if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
		{
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
			if ( nRetCode != OK )
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
			else
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event("End   BicIssuerDelegationProcessing(Err PreControl)",PROCESSING);
			return(NOK);
		}

		/******************************************************************************/
		/*          Security Verfication                                                  */
		/******************************************************************************/
		if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
		{
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
			if ( nRetCode != OK )
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
			else
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event("End   BicIssuerDelegationProcessing(Err CheckSecurity)",PROCESSING);
			return(NOK);
		}

		/******************************************************************************/
		/*  Amount Bill/Settlement Management                                         */
		/******************************************************************************/
		nRetCode = AmountMng (    nIndiceCtx          ,
			sServGatewayFlag    ,
			sBillCalculFlag     ,
			sTlvBuffer
			);
		if ( nRetCode != OK )
		{
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
			if ( nRetCode != OK )
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
			else
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event ( "Retour AmountMng NOK ", PROCESSING );
			trace_event("End   BicIssuerDelegationProcessing(Err AmntMng)",PROCESSING);
			return(NOK);
		}

		/******************************************************************************/
		/*          Check Limits                                                          */
		/******************************************************************************/
		if ( CheckLimits(  nIndiceCtx, "OFF", sTlvBuffer, sAlternAmount ) != OK )
		{
			/*YK01062007: pour ne pas envoyer la reponse reçu de la SMC*/
			char sActionCode[3 + 1];
			memset(sActionCode, 0,sizeof(sActionCode));
			memcpy(sActionCode, "999", 3);
			trace_event("check old response",PROCESSING);

			if (GetIsoField (ISO_RESPONSE_CODE,     IsoInfo, sActionCode   ,&nLength) == SUCCESS)
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer, "116",3);
			else
			{

				nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
				if ( nRetCode != OK )
					PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
				else
					PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			}
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event("End   BicIssuerDelegationProcessing(Err CheckLimits)",PROCESSING);
			return(NOK);
		}

		/******************************************************************************/
		/*          Check Account                                                         */
		/******************************************************************************/
		if ( CheckAccAvailable   ( nIndiceCtx, sTlvBuffer ) != OK )
		{
			/* PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,MALFUNCTION_EVENT,3); */
			nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
			if ( nRetCode != OK )
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
			else
				PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);
			InitTlvAutho    ( &sTlvInfo );
			AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
			TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
			trace_event("End   BicIssuerDelegationProcessing(Err CheckAvailable)",PROCESSING);
			return(NOK);
		}


		InitTlvAutho    ( &sTlvInfo );
		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );

		PrintTlvBuffer ( sTlvBuffer );
		trace_event("End   BicIssuerDelegationProcessing(OK )",PROCESSING);
		return(OK);
	}
	else
	{
		PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT,3);
		PutTlvBuffer (AUTO_RESPONSE_CODE ,sTlvBuffer, ISSUER_UNAVAILABLE_ACTION ,3);
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , v_action );
		if ( nRetCode != OK )
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
		else
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,v_action ,3);

		InitTlvAutho    ( &sTlvInfo );
		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
		trace_event("End   BicIssuerDelegationProcessing(ISSUER NOT AVAILABLE )",PROCESSING);
		return(NOK);
	}
}


/* End MKB12052014 SMT Integration */


/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les demandes d'autorisations                         */
/****************************************************************************/
/****************************************************************************/
int StandInAuthorisationX(	int nIndiceCtx, char *sMyResource,
	char *sTlvBuffer , TSIsoInfo *sIsoMsg  , char *autho_flag )
{

	TSTlvAutho     tlvInfo;
	char            sTlvPrivateBuffer [ LG_MAX + 1 ];
	TSTlvPrivate    sTlvPrivateInfo;
	char		   sLine		[MAX_LINE_TRC];
	int     	   nLength;
	int 		   nEventCode =  OK;
	int 		   nRetCode;
	char 		   sEventCode  [ 3 + 1 ];
	char 		   sActionCode  [ 3 + 1 ];
	char 		   sPrimResource   [ 6 + 1 ];
	char 		   sSecResource    [ 6 + 1 ];
	char    	   sAlternAmount [ LG_MAX + 1 ];
	char    	   sStan         [ 6 + 1 ];

	trace_event("Start StandInAuthorisation ()",PROCESSING);
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
	/******************************************************************************/
	/******************************************************************************/
	/* 				          le Traitement StandIn  Cas Exception                */
	/*          ( Time Out de L'Issuer ou System Malfunction de L'Issuer          */
	/******************************************************************************/
	/******************************************************************************/

	if ( GetTlvAutho ( AUTO_ROUTING_CODE , &tlvInfo , sPrimResource ,&nLength) != SUCCES )
	{
		if ( memcmp ( autho_flag , "T" , 1 ) == 0 )
		{
			PutTlvAutho (AUTO_EVENT_CODE    ,&tlvInfo , ISSUER_NOT_AVAILABLE_EVENT,3);
			PutTlvAutho (AUTO_RESPONSE_CODE ,&tlvInfo , ISSUER_UNAVAILABLE_ACTION ,3);
		}
		else
		{
			PutTlvAutho (AUTO_EVENT_CODE    ,&tlvInfo , MALFUNCTION_EVENT  ,3);
			PutTlvAutho (AUTO_RESPONSE_CODE ,&tlvInfo , MALFUNCTION_ACTION ,3);
		}
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nRetCode != OK )
			PutTlvAutho(AUTO_RESPONSE_CODE,&tlvInfo ,MALFUNCTION_ACTION,3);
		else
			PutTlvAutho(AUTO_RESPONSE_CODE,&tlvInfo ,sActionCode ,3);
		TlvToIso    ( nIndiceCtx 		 ,&tlvInfo , sIsoMsg  , RESPONSE );
		return ( OK );
	}

	if ( GetTlvAutho ( AUTO_ADD_DATA_PRIVATE , &tlvInfo , sTlvPrivateBuffer ,&nLength) != SUCCES )
	{
		if ( memcmp ( autho_flag , "T" , 1 ) == 0 )
		{
			PutTlvAutho (AUTO_EVENT_CODE    ,&tlvInfo , ISSUER_NOT_AVAILABLE_EVENT,3);
			PutTlvAutho (AUTO_RESPONSE_CODE ,&tlvInfo , ISSUER_UNAVAILABLE_ACTION ,3);
		}
		else
		{
			PutTlvAutho (AUTO_EVENT_CODE    ,&tlvInfo , MALFUNCTION_EVENT  ,3);
			PutTlvAutho (AUTO_RESPONSE_CODE ,&tlvInfo , MALFUNCTION_ACTION ,3);
		}
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nRetCode != OK )
			PutTlvAutho(AUTO_RESPONSE_CODE,&tlvInfo ,MALFUNCTION_ACTION,3);
		else
			PutTlvAutho(AUTO_RESPONSE_CODE,&tlvInfo ,sActionCode ,3);
		TlvToIso    ( nIndiceCtx 		 ,&tlvInfo , sIsoMsg  , RESPONSE );
		return ( OK );
	}

	InitTlvPrivate(&sTlvPrivateInfo);
	AnalyseTlvPrivate( sTlvPrivateBuffer ,nLength,&sTlvPrivateInfo);
	if ( GetTlvPrivate(SECONDARY_RESOURCE_TAG ,&sTlvPrivateInfo ,sSecResource ,&nLength) != SUCCES )
	{
		if ( memcmp ( autho_flag , "T" , 1 ) == 0 )
		{
			PutTlvAutho (AUTO_EVENT_CODE    ,&tlvInfo , ISSUER_NOT_AVAILABLE_EVENT,3);
			PutTlvAutho (AUTO_RESPONSE_CODE ,&tlvInfo , ISSUER_UNAVAILABLE_ACTION ,3);

		}
		else
		{
			PutTlvAutho (AUTO_EVENT_CODE    ,&tlvInfo , MALFUNCTION_EVENT  ,3);
			PutTlvAutho (AUTO_RESPONSE_CODE ,&tlvInfo , MALFUNCTION_ACTION ,3);
		}
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nRetCode != OK )
			PutTlvAutho(AUTO_RESPONSE_CODE,&tlvInfo ,MALFUNCTION_ACTION,3);
		else
			PutTlvAutho(AUTO_RESPONSE_CODE,&tlvInfo ,sActionCode ,3);
		TlvToIso    ( nIndiceCtx 		 ,&tlvInfo , sIsoMsg  , RESPONSE );
		return ( OK );
	}

	if ( memcmp ( sSecResource , STAND_IN_RESOURCE , 6 ) == 0 )
	{
		trace_event ( "Sec (Exception): STAND_IN Process", PROCESSING );
		/******************************************************************************/
		/* 	    Lecture des flags de securite                                         */
		/******************************************************************************/
		if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "OFF" ) != OK )
			return ( AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
			sMyResource , "1002", 'S' ));

		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
			return ( AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
			sMyResource , "1002", 'S' ));

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
			return ( AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
			sMyResource , "1002", 'S' ));

		/******************************************************************************/
		/* 	    Check Limits                                                          */
		/******************************************************************************/
		if ( CheckLimits(  nIndiceCtx, "OFF", sTlvBuffer, sAlternAmount ) != OK )
			return ( AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
			sMyResource , "1002", 'S' ));

		/******************************************************************************/
		/* 	    Check Account                                                         */
		/******************************************************************************/
		if ( CheckAccAvailable   ( nIndiceCtx, sTlvBuffer ) != OK )
		{
			PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,MALFUNCTION_EVENT,3);
			return ( AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
				sMyResource , "1002", 'S' ));
		}

		nRetCode = AuthorUpdate( nIndiceCtx, sTlvBuffer , sIsoMsg ,
			sMyResource , "1002", 'S' );
		return(nRetCode);
	}
	else
	{
		if ( memcmp ( autho_flag , "T" , 1 ) == 0 )
		{
			PutTlvAutho (AUTO_EVENT_CODE    ,&tlvInfo , ISSUER_NOT_AVAILABLE_EVENT,3);
			PutTlvAutho (AUTO_RESPONSE_CODE ,&tlvInfo , ISSUER_UNAVAILABLE_ACTION ,3);
			AuthoBuildTlv( sTlvBuffer, &tlvInfo);
		}
		else
		{
			PutTlvAutho (AUTO_EVENT_CODE    ,&tlvInfo , MALFUNCTION_EVENT  ,3);
			PutTlvAutho (AUTO_RESPONSE_CODE ,&tlvInfo , MALFUNCTION_ACTION ,3);
			AuthoBuildTlv( sTlvBuffer, &tlvInfo);
		}
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nRetCode != OK )
			PutTlvAutho(AUTO_RESPONSE_CODE,&tlvInfo ,MALFUNCTION_ACTION,3);
		else
			PutTlvAutho(AUTO_RESPONSE_CODE,&tlvInfo ,sActionCode ,3);
		TlvToIso    ( nIndiceCtx 		 ,&tlvInfo , sIsoMsg  , RESPONSE );

		PutTlvBuffer ( AUTO_AUTHO_FLAG  , sTlvBuffer, "T" ,  1);
		PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);
		PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1002" , 4 );
		nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   StandInAuthorisation()",PROCESSING);

		return ( OK );
	}

	return ( OK );
}


/******************************************************************************************************/
