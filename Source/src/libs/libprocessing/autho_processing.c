/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      autho_processing.c                                                     */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Controle Autorisation module                                        */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   12/05/2002 -         V 2.0.1                              */
/*        Initialisation de la structure TLV buffer                            */
/*        See HYJCC120502                                                      */
/*        H.FETHI   21/10/2003 -         V 2.0.2                               */
/*        Ajout du traitement DynamicAdvice                                    */
/*        See HF 21 10 2003                                                    */
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
extern char  ResourceId         [ 6 +1];


/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les demandes d'autorisations                         */
/****************************************************************************/
/****************************************************************************/
int AuthorRequestProcessing	(	int nIndiceCtx,
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
	TSService	    rService;
	TSTlvPrivate    TlvInfo;
	pid_t 		    nPid;
	char   		    sSecurityResults[ 11 ];
	char            tmp_buffer [ LG_MAX ];
	char		    slCardNumber[     22 + 1 ];
	char		    slProcCode	[      6 + 1 ];
	char		    slFuncCode	[      3 + 1 ];
	char		    sLine		[MAX_LINE_TRC];
	char 		    sEventCode  [ 3 + 1 ];
	char 		    sPrimResource  [ 6 + 1 ];
	char 		    sSecResource   [ 6 + 1 ];
	char 		    sPid           [ 8 + 1 ];
	char 		    sInternalStan  [ 6 + 1 ];
	char 		    cOnOffMode;
	char    	    sAlternAmount [ LG_MAX + 1 ];
	char    	    sTlvProcCode [ LG_MAX + 1 ];
	char    	    sIsoProcCode [ LG_MAX + 1 ];
	char            PosCond[3];
	char	        tmpField[MAX_LINE_TRC+1];
	char		    sFunctionCode	[3 + 1];
    int	            Account_verif_flag;
	int     	    nLength;
	int 		    nEventCode =  OK;
	int 		    nRetCode;
	int             PrimResUp = NOK;
	

	trace_event("Start AuthorRequestProcessing()",PROCESSING);
	/******************************************************************************/
	/* 					Initialisation			*/
	/******************************************************************************/

	memset(slCardNumber			, '\0', sizeof(slCardNumber));
	memset(slProcCode			, '\0', sizeof(slProcCode));
	memset(slFuncCode			, '\0', sizeof(slFuncCode));
	memset((char *)&rService    , '\0', sizeof(rService));
	memset(sEventCode           , '\0', sizeof(sEventCode));
	memset(sPrimResource     	, '\0', sizeof(sPrimResource));
	memset(sSecResource   		, '\0', sizeof(sSecResource));
	memset(sPid                 , '\0', sizeof(sPid));
	memset(sSecurityResults     , '\0',  sizeof ( sSecurityResults ));
	memset(sInternalStan        , '\0', sizeof(sInternalStan));
	memset(sAlternAmount        , '\0',  sizeof ( sAlternAmount ));
	memset(tmpField             , '\0',  sizeof ( tmpField ));
	memset(sFunctionCode     	, '\0', sizeof(sFunctionCode));

	/******************************************************************************/
	/*  Formatter le buffer TLV A partir du message Iso                           */
	/******************************************************************************/
	InitTlvAutho(&sTlvInfo);
	nRetCode = IsoToTlv ( nIndiceCtx , IsoInfo , &sTlvInfo );
	if ( nRetCode != OK )
	{
		trace_event ( "IsoToTlv Error ...", ERROR );
		InsertIsoField ( ISO_RESPONSE_CODE , IsoInfo , MALFUNCTION_ACTION , 3 );
		return ( NOK );
	}
	PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable ,     3);
	PutTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sMyResource ,     6);
	PutTlvAutho ( AUTO_ACQUIRER_BANK , &sTlvInfo , sAcquirerBank ,     6);
	PutTlvAutho ( AUTO_INTER_ROUT_CHECK,&sTlvInfo,InternationalRoutCheck,1);
	PutTlvAutho ( AUTO_ROUTING_CODE, &sTlvInfo , STAND_IN_RESOURCE ,  6);
	AuthoBuildTlv( sTlvBuffer, &sTlvInfo);
	PrintTlvBuffer ( sTlvBuffer );

	GetTlvAutho ( AUTO_FUNCTION_CODE        , &sTlvInfo, sFunctionCode   , &nLength );

	/******************************************************************************/
	/*  Recherche si Authorisation est Duplicate                                  */
	/******************************************************************************/
	nRetCode = GetOriginalDataElements(  nIndiceCtx , CAPTURE_MODE ,  
		sTlvBuffer ,
		sPrimResource );
	if ( nRetCode == OK )
	{
		trace_event ( "Found a Duplicate Request. ", PROCESSING );

		/**Begin MSA03042008*/
		/* In case of Duplicate, over-write ACTION_CODE & AUTHORIZATION_CODE */
		/*PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, DUPLICATE_AUTHO, 3);*/
		PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, "308", 3); /*EBE121120*/
		PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, "      ", 6);
		/**END MSA03042008**/

		InitTlvAutho    ( &sTlvInfo );
		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
		return ( OK );
	}
	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if ( CheckReqTlvFields ( nIndiceCtx, sTlvBuffer ) != OK )
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
		sMyResource , sPrimResource , 'S' ));


	/* Start EBE131220 */	

	if( memcmp(sFunctionCode,"101",3) == 0 )

	{

		nRetCode = GetOriginalPreAuth(nIndiceCtx,sTlvBuffer);

	}

	/* End EBE131220 */


	/******************************************************************************/
	/* 		Load Issuer Data Info Including Routing Info                    */
	/******************************************************************************/

	/* BEGIN: LHI 25-05-2008 EVENT CODE MUST BE SETTED IN NOK UNTIL ALL CONTROLS ARE OK */
	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT ,     3);
	/* END LHI 21-05-2008 INIT OF EVENT CODE AND ACTION CODE */

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
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
		sMyResource , sPrimResource , 'N' ));

	PutTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource ,  6);
	/****************************************************************************/
	/* Cette Modif Etait Faite Pour Europay Pour Changer Le processing Code 
	Des Cartes Mestro En cas de Cash Advance Voir PCRD_P7_ROUTING */
	memset(sTlvProcCode, 0, sizeof(sTlvProcCode));
	memset(sIsoProcCode, 0, sizeof(sIsoProcCode));
	if(	GetTlvBuffer(AUTO_PROCESSING_CODE,sTlvBuffer,sTlvProcCode,&nLength) == SUCCES)
	{
		if (GetIsoField (ISO_PROC_CODE, 	IsoInfo, sIsoProcCode   ,&nLength) == SUCCES)
		{
			sprintf(sLine," PROCESSING CODE TLV %s", sTlvProcCode);
			trace_event(sLine,TRACE);

			sprintf(sLine," PROCESSING CODE ISO %s", sIsoProcCode);
			trace_event(sLine,TRACE);
			memcpy(sIsoProcCode, sTlvProcCode + 6, 2);
			sprintf(sLine," PROCESSING CODE ISO %s", sIsoProcCode);
			trace_event(sLine,TRACE);
			PutIsoField ( ISO_PROC_CODE , IsoInfo , sIsoProcCode , 6 );
		}
	}

	/****************************************************************************/

	/******************************************************************************/
	/* 				          Controle du service /r ressource   */
	/******************************************************************************/
	GetIsoField (ISO_CARD_NBR , 	IsoInfo, slCardNumber ,&nLength);
	GetIsoField (ISO_PROC_CODE, 	IsoInfo, slProcCode   ,&nLength);
	GetIsoField (ISO_FUNCTION_CODE, IsoInfo, slFuncCode   ,&nLength);
	sprintf(rService.szMti, "%04d", IsoInfo->nMsgType); 

	nLength = 0;
	memcpy(rService.szProcCode, slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szSrcAcc, 	slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szDestAcc, 	slProcCode + nLength, 2); 
	memcpy(rService.szFctCode , slFuncCode , 3); 

	nEventCode = CheckResourceServices ( nIndiceCtx, sMyResource, &rService );
	if (nEventCode != OK) 
	{
		sprintf ( sLine , "Err CheckResourceServices, EventCode= [%d] ", nEventCode );
		trace_event ( sLine , PROCESSING ); 

		sprintf ( sEventCode , "%03d", nEventCode );
		PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode ,     3);
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , sMyResource , sPrimResource , 'N' ));
	}

	/*Start ENH OCT 8.2 SWI020908*/
	memset ( PosCond,  0 , sizeof ( PosCond ));
	Account_verif_flag = NOK;
	if (GetIsoField(ISO_NET_POS_DATA, IsoInfo, tmpField, &nLength) == SUCCES)
	{
		InitTlvPrivate(&TlvInfo);
		AnalyseTlvPrivate(tmpField, nLength, &TlvInfo);
		if (GetTlvPrivate(POS_CONDITION_CODE_TAG, &TlvInfo, PosCond, & nLength) == SUCCES) 
		{
			if(memcmp(PosCond,"51",2) == 0) Account_verif_flag = OK;
			sprintf (  sLine , "ACCOUNT VERIFICATION1 ENH8.2 ...%d ", Account_verif_flag  );
			trace_event( sLine , PROCESSING );
		}
	}
	/*End ENH OCT 8.2 SWI020908*/


	/******************************************************************************/
	/******************************************************************************/
	/* 				          le Traitement StandIn                               */
	/******************************************************************************/
	/******************************************************************************/

	if ( memcmp ( sPrimResource , STAND_IN_RESOURCE , 6 ) == 0 )
	{
		trace_event ( "Prm : STAND_IN AUTHORISATION PROCESSING", PROCESSING );
		/******************************************************************************/
		/* 	    Lecture des flags de securite                                         */
		/******************************************************************************/
		if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "OFF" ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'N' ));

		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'N' ));

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , sMyResource , sPrimResource , 'N' ));

		if(Account_verif_flag != OK)  /*ENH OCT 8.2 SWI020908*/
		{

			/******************************************************************************/
			/*  Amount Bill/Settlement Management                                         */ 
			/******************************************************************************/
			nRetCode = AmountMng (    nIndiceCtx          ,
				sServGatewayFlag	  ,
				sBillCalculFlag	  ,
				sTlvBuffer          
				);
			if ( nRetCode != OK )
			{
				trace_event ( "Retour AmountMng NOK ", PROCESSING );
				return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
					sMyResource , sPrimResource , 'N' ));
			}

			PrintTlvBuffer ( sTlvBuffer );



			/******************************************************************************/
			/* 	    Check Limits                                                          */
			/******************************************************************************/
			if ( CheckLimits(  nIndiceCtx, "OFF", sTlvBuffer, sAlternAmount ) != OK )
				return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'N' ));
			/* Mehdi Elyajizi MTP EMV FALLBACK251007*/

		}/*ENH OCT 8.2 SWI020908*/
		/******************************************************************************/
		/* 	    Check FallBack                                                      */
		/******************************************************************************/
		if(memcmp(sMyResource,"000002",6)){  /**MKB230109*** No Fall Back for voice auth **/
			if ( CheckFallBack(  nIndiceCtx, "OFF", sTlvBuffer  ) != OK )
				return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'N' ));
		}

		/* Mehdi Elyajizi MTP EMV FALLBACK251007 */

		/******************************************************************************/
		/* 	    Check Account                                                         */
		/******************************************************************************/
		if ( CheckAccAvailable   ( nIndiceCtx, sTlvBuffer ) != OK )
		{
			/* PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,MALFUNCTION_EVENT,3); */
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'N' ));
		}

		/* BEGIN: LHI 25-05-2008 EVENT CODE MUST BE SETTED IN NOK UNTIL ALL CONTROLS ARE OK */
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION ,     3);
		/* END LHI*/

		nRetCode = AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'N' ); /** PA DSS **/

		memset(slCardNumber, 0, sizeof(slCardNumber)); /**PA DSS*/
		/***PA DSS*/
		InitTlvAutho(&sTlvInfo);
		InitTlvPrivate(&TlvInfo);
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
		memset(tmpField                         , 0, sizeof(tmpField));
		/**/

		InitTlvAutho(&sTlvInfo); /** HM - PA_DSS **/
		trace_event("End AuthorRequestProcessing STAND_IN(OK)",PROCESSING);
		return(nRetCode); /**PA DSS*/

	}


	/******************************************************************************/
	/* Check If Routing is National Or International                              */
	/******************************************************************************/

	if( GetTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,sEventCode,&nLength) != SUCCES)
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
		sMyResource , sPrimResource , 'N' ));
	/******************************************************************************/
	/* PreControl To Run Before Sending  Request to HOST/BANK                     */
	/******************************************************************************/
	/*Start KS 121206 Control in case Primary resource down */


	nRetCode = CheckResource ( nIndiceCtx, sPrimResource , &nPid );
	if ( ( nRetCode == OK ) && ( kill ( nPid , 0 ) == 0  || !memcmp(ResourceId,VRS_RESOURCE,6) ) )
		PrimResUp = OK;

	/*End KS 121206 Control in case Primary resource down */

	/*if ( memcmp ( sEventCode , INTERNATIONAL_ROUTING_EVENT , 3 ) != 0 )*/
	if ( (memcmp ( sEventCode , INTERNATIONAL_ROUTING_EVENT , 3 ) != 0) &&  (PrimResUp == OK) ) /*KS 121206 Control in case Primary resource down */ {
		/******************************************************************************/
		/* 	    Lecture des flags de securite                                         */
		/******************************************************************************/
		if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "ON" ) != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'S' ));
		}

		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'S' ));
		}

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'S' ));
		}

		/******************************************************************************/
		/*  Amount Bill/Settlement Management                                         */ 
		/******************************************************************************/
		/*    PrintTlvBuffer ( sTlvBuffer );*/

		if(Account_verif_flag != OK)  /*ENH OCT 8.2 SWI020908*/
		{
			nRetCode = AmountMng (    nIndiceCtx          ,
				sServGatewayFlag	  ,
				sBillCalculFlag	  ,
				sTlvBuffer          
				);
			if ( nRetCode != OK )
			{
				PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
				trace_event ( "Retour AmountMng NOK ", PROCESSING );
				return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
					sMyResource , sPrimResource , 'S' ));
			}

			PrintTlvBuffer ( sTlvBuffer );

			/******************************************************************************/
			/* 	    Check Limits                                                          */
			/******************************************************************************/
			if ( CheckLimits(  nIndiceCtx, "ON", sTlvBuffer, sAlternAmount ) != OK )
			{
				PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
				return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
					sMyResource , sPrimResource , 'S' ));
			}
		}/*ENH OCT 8.2 SWI020908*/
		/* Mehdi Elyajizi MTP EMV FALLBACK251007 */
		/******************************************************************************/
		/*        Check FallBack                                                      */
		/******************************************************************************/
		if(memcmp(sMyResource,"000002",6)) /*YK19032009*/
			if ( CheckFallBack(  nIndiceCtx, "OFF", sTlvBuffer  ) != OK )
			{
				PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
				return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
					sMyResource , sPrimResource , 'N' ));
			}		
			/* Mehdi Elyajizi MTP EMV FALLBACK251007		*/
	}
	else if ( memcmp ( sEventCode , INTERNATIONAL_ROUTING_EVENT , 3 ) == 0 ) /*KS 121206 Control in case Primary resource down */
	{
		/******************************************************************************/
		/*  Amount Bill/Settlement Management International                           */ 
		/******************************************************************************/
		if(Account_verif_flag != OK)  /*ENH OCT 8.2 SWI020908*/  
		{  
			nRetCode = AmountMng (    nIndiceCtx          ,
				sServGatewayFlag	  ,
				sBillCalculFlag	  ,
				sTlvBuffer          
				);
			if ( nRetCode != OK )
			{
				trace_event ( "Retour AmountMng NOK ", PROCESSING );
				return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
					sMyResource , sPrimResource , 'N' ));
			}
		}/*ENH OCT 8.2 SWI020908*/

		PrintTlvBuffer ( sTlvBuffer );

	}
	/*Start KS 121206 Control in case Primary resource down */

	/*nRetCode = CheckResource ( nIndiceCtx, sPrimResource , &nPid );*/

	if ( (memcmp ( sEventCode , INTERNATIONAL_ROUTING_EVENT , 3 ) != 0) &&  (PrimResUp == NOK) )
	{
		trace_event("PrimResUp is NOK",TRACE);
		nRetCode = NOK;  
	}
	else 
		nRetCode = CheckResource ( nIndiceCtx, sPrimResource , &nPid );

	/*End KS 121206 Control in case Primary resource down */
	if ( ( nRetCode == OK ) && ( kill ( nPid , 0 ) == 0  || !memcmp(ResourceId,VRS_RESOURCE,6) ) ) 
		/* Le check sur la resource VRS est ajoute car ca appartient a un autre user */
	{
		trace_event ( "Primary Ressource Up ", PROCESSING ); 
		if ( CardInSaf ( nIndiceCtx , slCardNumber ) != OK )
		{
			/* BEGIN: LHI 25-05-2008 EVENT CODE MUST BE SETTED IN NOK UNTIL ALL CONTROLS ARE OK */
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION ,     3);

			return( AuthorRouting(nIndiceCtx,sTlvBuffer,
				sPrimResource,nPid,IsoInfo));
		}
		else
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1005" , 4 );
			/*MKB060309 PCI Norme Integration */
			if (debug_option == 1)
			{
				sprintf ( sLine , "Card [%s] Found In Saf File ", slCardNumber );
				trace_event ( sLine , PROCESSING ); 
			}
		}
	}
	else
	{
		PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1004" , 4 );
		trace_event ( "Primary Ressource Down ", PROCESSING ); 
	}


	if ( memcmp ( sSecResource , NO_ROUTING_RESOURCE , 6 ) == 0 )
	{
		PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT,3);
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'N' ));
	}

	if ( memcmp ( sSecResource , STAND_IN_RESOURCE , 6 ) == 0 )
	{
		trace_event ( "Sec : STAND_IN AUTHORISATION PROCESSING", PROCESSING );
		/******************************************************************************/
		/* 	    Lecture des flags de securite                                         */
		/******************************************************************************/
		if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "OFF" ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));

		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));

		/******************************************************************************/
		/* 	    Check Limits                                                          */
		/******************************************************************************/
		if(Account_verif_flag != OK)  /*ENH OCT 8.2 SWI020908*/
		{
			if ( CheckLimits(  nIndiceCtx, "OFF", sTlvBuffer, sAlternAmount ) != OK )
				return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'S' ));
		}/*ENH OCT 8.2 SWI020908*/
		/******************************************************************************/
		/* 	    Check Account                                                         */
		/******************************************************************************/
		if ( CheckAccAvailable   ( nIndiceCtx, sTlvBuffer ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));

		/* BEGIN: LHI 25-05-2008 EVENT CODE MUST BE SETTED IN NOK UNTIL ALL CONTROLS ARE OK */
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION ,     3);          

		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));
	}
	else
	{
		nRetCode = CheckResource ( nIndiceCtx, sSecResource , &nPid );
		if ( nRetCode != OK )
		{
			PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT,3);
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , sMyResource , sPrimResource , 'N' ));
		}
		/* BEGIN: LHI 25-05-2008 EVENT CODE MUST BE SETTED IN NOK UNTIL ALL CONTROLS ARE OK */
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION ,     3);    	
		return( AuthorRouting ( nIndiceCtx,sTlvBuffer,

			sSecResource,nPid,IsoInfo ));
	}
	return ( OK );
}

/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les reponses autorisations                          */
/****************************************************************************/
/****************************************************************************/

int AuthorReplyProcessing  ( int nIndiceCtx, TSIsoInfo *IsoInfo , char *sTlvBuffer )
{
	int                 nRetCode = 0;
	int                 length   = 0;
	int			nTimeLen = 0;
	int   		nLength  = 0;

	TSTlvAutho          sTlvInfo;
	TSTlvPrivate        sTlvPrivateInfo;
	struct timeval 	time_s_aut;

	char 		sActionCode    [       3 + 1];
	char  		sCaptureCode   [       6 + 1];
	char  		sProcessingCode[       6 + 1];
	char  		sRoutingCode   [       6 + 1];
	char  		TableIndicator [       3 + 1];
	char  		sIssActionCode [     256 + 1];
	char		sTimeField     [         256];
	char		tmpField       [         256];
	char		szWork         [         256];
	char  		sLine          [MAX_LINE_TRC];
	char                vAuthorSrc     [      2 + 1 ]; /** 20040303 **/

	trace_event ( "Start AuthorReplyProcessing()", PROCESSING );

	InitTlvAutho    ( &sTlvInfo ); /** MAW20050103 **/
	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );

	/*  Add By HF 21/10/2003 */
	memset ( sActionCode    , 	0, sizeof(sActionCode) ); 
	memset ( sCaptureCode   , 	0, sizeof(sCaptureCode) );
	memset ( sProcessingCode  , 	0, sizeof(sProcessingCode) ); 
	memset ( sRoutingCode   , 	0, sizeof(sRoutingCode) );
	memset ( TableIndicator , 	0, sizeof(TableIndicator) ); 
	memset ( sIssActionCode , 	0, sizeof(sIssActionCode) ); 
	memset ( sTimeField , 	0, sizeof(sTimeField) ); 
	memset ( tmpField   , 	0, sizeof(tmpField) );
	memset ( szWork   , 	        0, sizeof(szWork) );
	memset ( sLine   , 	        0, sizeof(sLine) );
	memset ( vAuthorSrc   ,       0, sizeof(vAuthorSrc) );

	/*  Add By HF 21/10/2003 */


	PrintTlvBuffer ( sTlvBuffer );

	GetTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sCaptureCode   , &nLength );
	GetTlvAutho ( AUTO_ROUTING_CODE        , &sTlvInfo, sRoutingCode   , &nLength );
	GetTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, TableIndicator , &nLength ); 
	GetTlvAutho ( AUTO_RESPONSE_CODE       , &sTlvInfo, sActionCode    , &nLength ); 

	dump_p7_struct ( IsoInfo );


	/** START 20040303 **/
	memset(vAuthorSrc, 0, sizeof(vAuthorSrc));
	if ( !memcmp(sCaptureCode, "000002", 6) )
		memcpy( vAuthorSrc, "P0" , 2);
	else
		memcpy( vAuthorSrc, "E0" , 2);
	/** END   20040303 **/


	nRetCode = IsoToTlv ( nIndiceCtx , IsoInfo , &sTlvInfo );
	if ( nRetCode != OK )
	{
		trace_event ( "IsoToTlv Error ...", ERROR );
		if ( memcmp ( sActionCode , "000" , 3 ) == 0 )
			AutomaticReversalProcessing   (   nIndiceCtx,   CAPTURE_MODE,
			sCaptureCode, sRoutingCode , 
			TableIndicator , IsoInfo ,
			"4002" );
		InsertIsoField ( ISO_RESPONSE_CODE , IsoInfo , MALFUNCTION_ACTION , 3 );
		return ( NOK );
	}
	trace_event ( "Fin IsoToTlv  ...", ERROR );

	PutTlvAutho ( AUTO_AUTHO_FLAG         , &sTlvInfo, "P" ,  1);
	/*PutTlvAutho ( AUTO_AUTHOR_SRC         , &sTlvInfo , "E0" ,  2); **/
	PutTlvAutho ( AUTO_AUTHOR_SRC         , &sTlvInfo , vAuthorSrc ,  2); /** 20040303 **/
	/* Start HYJCC120502 */

	memset(sTlvBuffer, 0, LG_MAX);

	/* End   HYJCC120502 */
	length = AuthoBuildTlv( sTlvBuffer, &sTlvInfo);
	PrintTlvBuffer ( sTlvBuffer );

	nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
	if ( nRetCode != OK )                                                                                                         
	{
		memcpy(sActionCode,MALFUNCTION_ACTION,3);
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
	}
	else
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);

	/* System Malfunction Processing */
	/**    HF 21/10/2003
	if ( GetIsoField (ISO_NET_POS_DATA, IsoInfo, szWork, &nLength ) == SUCCES )
	{
	InitTlvPrivate (&sTlvPrivateInfo);
	AnalyseTlvPrivate ( szWork , nLength , &sTlvPrivateInfo);
	memset(sIssActionCode, 0, sizeof(sIssActionCode));
	if ( GetTlvPrivate (RESPONSE_CODE_TAG, &sTlvPrivateInfo, sIssActionCode , &nLength) == SUCCES )
	{
	if(!memcmp(sIssActionCode, ISS_MALFUNCTION_ACTION, 2))
	{
	nRetCode = SystemMalfunctionProcessing( nIndiceCtx, sCaptureCode, sTlvBuffer , IsoInfo);
	StopListReplyProcessing (nIndiceCtx,sTlvBuffer); 
	Mise a Jour Point Service 
	PrintTlvBuffer ( sTlvBuffer );
	trace_event ( "End AuthorReplyProcessing(SystemMalfunctionProcessing)", PROCESSING );
	return ( nRetCode );
	}
	}
	}


	PrintTlvBuffer ( sTlvBuffer );
	FIN 21/10/2003 **/ 

	/*********************/
	/* System Malfunction Processing */

	if ( GetIsoField (ISO_NET_POS_DATA, IsoInfo, szWork, &nLength ) == SUCCES )
	{
		InitTlvPrivate (&sTlvPrivateInfo);
		AnalyseTlvPrivate ( szWork , nLength , &sTlvPrivateInfo);
		memset(sIssActionCode, 0, sizeof(sIssActionCode));
		if ( GetTlvPrivate (RESPONSE_CODE_TAG, &sTlvPrivateInfo, sIssActionCode , &nLength) == SUCCES )
		{
			/* HY 170403 
			Pour prendre en compte les reponses interface issuer ( Cas de pin translation )
			*/
			vAuthorSrc[1] = '2' ; /** 20040303 **/
			if(!memcmp(sIssActionCode, "XX", 2))
				/** PutTlvBuffer ( AUTO_AUTHOR_SRC         , sTlvBuffer, "E2" ,  2); **/
				PutTlvBuffer ( AUTO_AUTHOR_SRC         , sTlvBuffer, vAuthorSrc ,  2); /** 20040303 **/

			if(!memcmp(sIssActionCode, ISS_MALFUNCTION_ACTION, strlen(ISS_MALFUNCTION_ACTION)))
			{
				/* Secondary Resource Stand In */
				memset(szWork, 0, sizeof(szWork));
				if ( GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer , szWork ,&nLength) != SUCCES )
				{
					InitTlvAutho ( &sTlvInfo );
					AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
					TlvToIso    ( nIndiceCtx         ,&sTlvInfo , IsoInfo  , RESPONSE );
					trace_event("End   DynamicStandinProcessing(Field 48 Not Found In TLV)",FATAL);
					return ( NOK );
				}

				InitTlvPrivate(&sTlvPrivateInfo);
				AnalyseTlvPrivate( szWork ,nLength,&sTlvPrivateInfo);
				if ( GetTlvPrivate(SECONDARY_RESOURCE_TAG ,&sTlvPrivateInfo ,tmpField ,&nLength) != SUCCES )
				{
					InitTlvAutho ( &sTlvInfo );
					AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
					TlvToIso    ( nIndiceCtx         ,&sTlvInfo , IsoInfo  , RESPONSE );
					AuthorUpdate( nIndiceCtx, sTlvBuffer , IsoInfo , sCaptureCode , "1003", 'S' );
					trace_event("End   DynamicStandinProcessing(Secondary Resource Not Found)",FATAL);
					return ( NOK );
				}

				if ( memcmp ( tmpField , STAND_IN_RESOURCE , 6 ) != 0 )
				{
					PutTlvBuffer (AUTO_AUTHO_FLAG ,sTlvBuffer , "P" ,1);
					/** PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E0" , 2); **/
					vAuthorSrc[1]='0';
					PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, vAuthorSrc , 2); /** 20040303 **/
					InitTlvAutho ( &sTlvInfo );
					AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
					TlvToIso    ( nIndiceCtx        ,&sTlvInfo , IsoInfo  , RESPONSE );
					nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
					if (nRetCode != OK)
						trace_event ( "Can Not Write In Authorisation table !!!", FATAL );
					sprintf(sLine,"End   DynamicStandinProcessing(Secondary Resource %s Not StandIn)",tmpField);
					trace_event(sLine,PROCESSING);
					return(OK);
				}

				nRetCode = CheckDynamicAdvice(nIndiceCtx, sRoutingCode);
				if (nRetCode == OK)
				{
					PutTlvBuffer (AUTO_AUTHO_FLAG ,sTlvBuffer , "P" ,1);
					/** PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2); **/
					vAuthorSrc[1]='2';
					PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, vAuthorSrc , 2); /** 20040303 **/
					PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1003" , 4 );

					/* Traitement Stand In */
					/* Balance Inquiry */
					/*
					if ( GetIsoField ( ISO_PROC_CODE , IsoInfo ,  sProcessingCode , &nLength ) == SUCCESS )
					{
					if (!memcmp(sProcessingCode, "31",2))
					{
					nRetCode = BalanceProcessing ( nIndiceCtx, sCaptureCode, sTlvBuffer,IsoInfo,"1003");
					nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
					if ( nRetCode != OK )
					memcpy(sActionCode,MALFUNCTION_ACTION,3);
					PutIsoField(ISO_RESPONSE_CODE,IsoInfo,sActionCode ,3);
					sprintf(sLine,"End   DynamicStandinProcessing(Balance Inquiry %d)",nRetCode);
					trace_event(sLine,PROCESSING);
					return(nRetCode);
					}
					}
					*/

					nRetCode = AuthorProcessing ( nIndiceCtx, sCaptureCode, sTlvBuffer,IsoInfo,"1003");
					nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
					if ( nRetCode != OK )
						memcpy(sActionCode,MALFUNCTION_ACTION,3);

					PutIsoField(ISO_RESPONSE_CODE,IsoInfo,sActionCode ,3);
				}
				else
				{
					PutTlvBuffer (AUTO_AUTHO_FLAG ,sTlvBuffer , "P" ,1);
					/** PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E0" , 2); **/
					vAuthorSrc[1]='0';
					PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, vAuthorSrc , 2); /** 20040303 **/
					nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
					if ( nRetCode != OK )
					{ 	
						trace_event ( "Can Not Write In Authorisation table !!!", FATAL );

						PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
						InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
						trace_event  ( "Can Not Update Autho Activity File ", PROCESSING );
						return ( NOK );
					}
					nRetCode = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
				}
				/* SAF Insertion */
				/* The function SafProcessing Checks if the primary resource Accept to receive decline advices Or Approuved Advices to be sent */

				nRetCode = AutSafProcessing(nIndiceCtx, sTlvBuffer,sCaptureCode,sRoutingCode);
				PrintTlvBuffer ( sTlvBuffer );
				trace_event ( "End AuthorReplyProcessing(SystemMalfunctionProcessing)", PROCESSING );
				return ( nRetCode );
			}
		}
	} 
	/*********************/
	/** Start RHA03122008**/
	if(GetIsoField ( ISO_PROC_CODE , IsoInfo ,  sProcessingCode , &nLength ) == SUCCESS) {
		sprintf(sLine,"Iso Proc Code Is ===> %s", sProcessingCode);
		trace_event(sLine, TRACE);}
	/* Update field 72 with the time Before autho_activity_insert Start HY 141002 */
	if(  GetTlvBuffer(AUTO_UF_FILE_RECORD,sTlvBuffer,tmpField,&nLength) == SUCCES
		&& memcmp(sProcessingCode, "38", 2) != OK) /**RHA03122008**/
	{
		/* YOua - 09Aug2003 */
		/*
		gettimeofday (&time_s_aut,NULL);
		*/
#if defined ( __STDC__ )
#if defined ( _SVID_GETTOD )
		gettimeofday (&time_s_aut);
#else
		gettimeofday (&time_s_aut,NULL);
#endif /* _SVID_GETTOD */
		gettimeofday (&time_s_aut,NULL);
#endif /* __STDC__ */
		/* YOua - 09Aug2003 */

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
		trace_event("Error in UpdateAuthoActivity 1",TRACE);
		if ( memcmp ( sActionCode , "000" , 3 ) == 0 )
		{
			trace_event("Error in UpdateAuthoActivity 2",TRACE);
			AutomaticReversalProcessing   (  nIndiceCtx,   CAPTURE_MODE,
				sCaptureCode, sRoutingCode ,
				TableIndicator , IsoInfo,
				"4002" );



		}

		trace_event("Error in UpdateAuthoActivity 3",TRACE);
		PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
		trace_event("Error in UpdateAuthoActivity 4",TRACE);
		InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("Error in UpdateAuthoActivity 5",TRACE);

		PutTlvBuffer ( AUTO_EVENT_CODE    , sTlvBuffer , MALFUNCTION_EVENT  , 3);
		PutTlvBuffer ( AUTO_RESPONSE_CODE , sTlvBuffer , MALFUNCTION_ACTION , 3); 
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nRetCode != OK )
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
		else
			PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);

		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso    ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
		trace_event  ( "SEVERE ERROR !!!!!", PROCESSING );
		trace_event  ( "Can Not Update Autho Activity File ", PROCESSING );
		return ( NOK );
	}

	if ( memcmp ( sActionCode , APPROVED_ACTION , 3 ) != 0 )
	{
		if ( memcmp ( sActionCode , INVALID_PIN_ACTION , 3 ) != 0 )
			nRetCode = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
		else
			nRetCode = UpdateCardActivity( nIndiceCtx , PIN_ERROR , sTlvBuffer );
	}
	else
	{
		nRetCode = UpdateCardActivity  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
		if ( nRetCode != OK )
		{
			trace_event ( "Can Not Update Card Activity  !!!", FATAL );
			/* What is the action to do */
		}

		nRetCode = UpdateBalance  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
		if ( nRetCode != OK ) 
		{
			trace_event ( "Can Not Update Balance  !!!", FATAL ); 
			/* What is the action to do */
		}
		UpdateSettlement( nIndiceCtx , sTlvBuffer);
	}

	StopListReplyProcessing (nIndiceCtx,sTlvBuffer);

	/**/

	InitTlvAutho    ( &sTlvInfo );
	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	/**Debut   Ajout BELAFIA le 05092007**/
	/*TlvToIso    ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );*/
	if ( TlvToIso ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE ) != OK ) {
		trace_event ( "TlvToIso Error ...", ERROR );
		PutIsoField ( ISO_RESPONSE_CODE , IsoInfo , MALFUNCTION_ACTION , 3 );
		/**Debut Ajout BELAFIA le 05092007**/
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
		nRetCode = UpdateAuthoActivity ( nIndiceCtx , sTlvBuffer );
		if ( nRetCode != OK )
		{
			trace_event ( "Error during Update AuthoActivity", FATAL );
		}
		/**fin   Ajout BELAFIA le 05092007**/
		return (SEVERE_ERROR); 
	}
	/**Fin   Ajout BELAFIA le 05092007**/
	if ( nTimeLen > 0 )
		PutIsoField( ISO_RECORD_DATA, IsoInfo,sTimeField ,nTimeLen );

	trace_event ( "End AuthorReplyProcessing()", PROCESSING );
	return ( OK );
}

/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les demandes d'advices                               */
/****************************************************************************/
/****************************************************************************/
int AuthorAdviceProcessing (	int nIndiceCtx,
	char *sMyResource, char *sAuthoTable,
	char *sAcquirerBank,
	char *sServGatewayFlag,
	char *sIntRoutCheck,
	/* Debut Ajout H.YOUSFI 151002 */
	char *sTlvBuffer,
	/* End   Ajout H.YOUSFI 151002 */
	TSIsoInfo *IsoInfo  )
{
	TSResources    sResource;
	/* char    	   sTlvBuffer   	  [ LG_MAX + 1 ]; */
	TSTlvAutho     sTlvInfo;
	char		sLine				[MAX_LINE_TRC];
	int     	nLength;
	int 		nEventCode   =  OK;
	int 		nRetCode;
	char 		sDestResource     [ 6 + 1 ];
	char 		sSescondaryResource [ 6 + 1 ];
	char 		sActionCode       [ 3 + 1 ];
	char 		sAuthNum	      [ 7 + 1 ];
	char 		sInternalStan  [ 6 + 1 ];
	char 		sPrimResource  [ 6 + 1 ];
	char 		sReasonCode  [ 4 + 1 ];
	char            vAuthorSrc     [ 2 + 1]; /** 20040303 **/
	char  		sCaptureCode   [  6 + 1]; /** 20040303 **/
	char		sFunctionCode	[3 + 1]; /* EBE131220 */

	trace_event("Start AuthorAdviceProcessing ()",PROCESSING);
	/******************************************************************************/
	/*               Initialisation				                                  */
	/******************************************************************************/
	memset(sDestResource        , 0, sizeof(sDestResource));
	InitTlvAutho(&sTlvInfo);
	/* memset( sTlvBuffer   	    , 0, sizeof ( sTlvBuffer )); */
	memset(sPrimResource     	, 0, sizeof(sPrimResource));
	memset(sFunctionCode     	, 0, sizeof(sFunctionCode));

	/******************************************************************************/
	/*  Formatter le buffer TLV A partir du message Iso                           */
	/******************************************************************************/
	nRetCode = IsoToTlv ( nIndiceCtx , IsoInfo , &sTlvInfo );
	if ( nRetCode != OK )
	{
		trace_event ( "IsoToTlv Error ...", ERROR );
		return ( NOK );
	}

	/** START 20040303 **/
	memset ( sCaptureCode   ,     0, sizeof(sCaptureCode) );
	GetTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sCaptureCode   , &nLength );
	GetTlvAutho ( AUTO_FUNCTION_CODE        , &sTlvInfo, sFunctionCode   , &nLength );


	memset(vAuthorSrc, 0, sizeof(vAuthorSrc));
	if ( !memcmp(sCaptureCode, "000002", 6) )
		memcpy( vAuthorSrc, "P0" , 2);
	else
		memcpy( vAuthorSrc, "E0" , 2);
	/** END   20040303 **/

	PutTlvAutho  ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable   , 3);
	PutTlvAutho  ( AUTO_CAPTURE_CODE        , &sTlvInfo, sMyResource   , 6);
	PutTlvAutho  ( AUTO_ACQUIRER_BANK       , &sTlvInfo , sAcquirerBank ,6);
	PutTlvAutho  ( AUTO_AUTHO_FLAG          , &sTlvInfo, "P" ,  1);
	/** PutTlvAutho  ( AUTO_AUTHOR_SRC  , &sTlvInfo , "E1" ,  2); **/
	vAuthorSrc [1] = '1';
	PutTlvAutho  ( AUTO_AUTHOR_SRC  , &sTlvInfo , vAuthorSrc ,  2); /** 20040303 **/
	AuthoBuildTlv ( sTlvBuffer, &sTlvInfo);
	PrintTlvBuffer(sTlvBuffer);

	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if ( CheckAdvTlvFields ( nIndiceCtx, sTlvBuffer ) != OK )
	{
		PutTlvBuffer ( AUTO_ROUTING_CODE , sTlvBuffer, STAND_IN_RESOURCE , 6);
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   AuthorAdviceProcessing (NOK)",PROCESSING);
		return(NOK);
	}

	/* Start EBE131220 */	
	if( memcmp(sFunctionCode,"101",3) == 0 )
	{
		nRetCode = GetOriginalPreAuth(nIndiceCtx,sTlvBuffer);
	}
	/* End EBE131220 */


	nRetCode = LoadIssuerData (  nIndiceCtx    ,
		sTlvBuffer    ,
		sIntRoutCheck ,
		sDestResource , 
		sSescondaryResource
		);

	PrintTlvBuffer(sTlvBuffer);

	if ( nRetCode != OK )
	{
		trace_event ( "Could Not Found Destination ", PROCESSING );
		PutTlvBuffer  ( AUTO_ROUTING_CODE        ,sTlvBuffer, STAND_IN_RESOURCE   , 6);
		PutTlvBuffer  ( AUTO_RESPONSE_CODE         ,sTlvBuffer, MALFUNCTION_ACTION , 3);
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   AuthorAdviceProcessing (NOK)",PROCESSING);
		return ( NOK );
	}

	nRetCode = AmountMng (    nIndiceCtx          ,
		sServGatewayFlag    ,
		"N"     ,                                                                                
		sTlvBuffer
		);

	if ( nRetCode != OK )
	{
		trace_event ( "Retour AmountMng NOK ", PROCESSING );
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   AuthorAdviceProcessing (NOK)",PROCESSING);
		return (  NOK );
	}

	PutTlvBuffer ( AUTO_ROUTING_CODE , sTlvBuffer, sDestResource , 6);
	if(	GetTlvBuffer(AUTO_AUTH_NUMBER ,sTlvBuffer,sAuthNum,&nLength) != SUCCES)
		GetAuthNumber ( nIndiceCtx , sTlvBuffer );

	/** debut 18032004 ihi */
	/* OBE23092007 the action in the authorization table should not be overwritten by an approved value
	PutTlvBuffer  ( AUTO_ORIG_ACTION_CODE	, sTlvBuffer, "000"  ,  3);
	PutTlvBuffer  ( AUTO_RESPONSE_CODE       , sTlvBuffer, "000" ,  3);*/
	PrintTlvBuffer(sTlvBuffer);
	/** end 18032004 ihi */

	nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{ 
		InsertionSaf ( nIndiceCtx, sTlvBuffer, sMyResource , sDestResource );
		PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		if ( nRetCode != OK )
			trace_event ( "Can Not Write In Authorization SAF Table ", PROCESSING );

		trace_event ( "Can Not Write In Authorization Table ", PROCESSING );
		return ( SEVERE_ERROR );
	}


	if (GetIsoField(ISO_RESPONSE_CODE,IsoInfo,sActionCode,&nLength) == SUCCES )
	{
		if ( memcmp ( sActionCode , APPROVED_ACTION , 3 ) != 0 )
		{
			if ( memcmp ( sActionCode , INVALID_PIN_ACTION , 3 ) != 0 )
				nRetCode = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
			else
				nRetCode = UpdateCardActivity( nIndiceCtx , PIN_ERROR , sTlvBuffer );
		}
		else
		{
			nRetCode = UpdateCardActivity  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
			if ( nRetCode != OK )
			{
				trace_event ( "Can Not Update Card Activity  !!!", FATAL );
				/* What is the action to do */
			}
			nRetCode = UpdateBalance  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
			if ( nRetCode != OK ) 
			{
				trace_event ( "Can Not Update Balance  !!!", FATAL ); 
				/* What is the action to do */
			}
			UpdateSettlement( nIndiceCtx , sTlvBuffer);
		}
	}

	/* Saf Insertion */
	/*
	if ( memcmp ( sDestResource , STAND_IN_RESOURCE , 6 ) != 0 )
	InsertionSaf ( nIndiceCtx, sTlvBuffer, sMyResource , sDestResource );
	*/

	/* Start EBE131220 */
	GetTlvBuffer(AUTO_FUNCTION_CODE,sTlvBuffer,sFunctionCode   , &nLength );
	if( memcmp(sFunctionCode,"201",3) == 0 ||
		memcmp(sFunctionCode,"202",3) == 0)
	{
		nRetCode = CompletePreauth(nIndiceCtx,sTlvBuffer);
	}
	/* End EBE131220 */


	memset(sReasonCode, 0, sizeof(sReasonCode));
	if(	GetTlvBuffer(AUTO_MESSAGE_REASON_CODE ,sTlvBuffer,sReasonCode,&nLength) == SUCCES &&
		!memcmp(sReasonCode,"7001",4)
		) 
	{
		memcpy(sReasonCode, "7001",4);
		sprintf(sLine,"PAS D4INSERTION SAF ");
		trace_event(sLine,PROCESSING);
	}
	else
	{
		nRetCode = AutSafProcessing(nIndiceCtx, sTlvBuffer,sMyResource,sDestResource);
		sprintf(sLine,"AutSafProcessing(%d)",nRetCode);
		trace_event(sLine,PROCESSING);
	}



	InitTlvAutho    ( &sTlvInfo );
	AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
	trace_event("End   AuthorAdviceProcessing (OK)",PROCESSING);
	return ( OK );
}

/***************************************************************************************/
/* Start ENH 12.2 SLA 130912 Fast Funds */
/*****************************************************************************************/
/* Author_OctProc                                                            */
/*****************************************************************************************/
int Author_OctProc ( int        P_nIndexCtx,
	char       *P_sMyResource,
	char       *P_sAuthoTable,
	char       *P_sAcquirerBank,
	char       *P_sInternationalRoutCheck,
	char       *P_sServGatewayFlag,
	char       *P_sBillCalculFlag,
	char       *P_sTlvBuffer,
	TSIsoInfo  *P_psIsoInfo)
{
	TSService       rService;
	TSTlvAutho      sTlvInfo;
	char            sPrimResource  [ 6 + 1 ];
	char            sSecResource   [ 6 + 1 ];
	char            slCardNumber   [ 22 + 1 ];
	char            slProcCode     [ 6 + 1 ];
	char            slFuncCode     [ 3 + 1 ];
	char            sEventCode     [ 3 + 1 ];
	char            sLine          [ MAX_LINE_TRC ];
	char            tmpField       [ LG_MAX ];
	pid_t           nPid;
	int             nLength;
	int             Length;
	int             nEventCode =  OK;
	int             nRetCode;
	char            sSecVerifFlag [ 20 + 1 ];
	char    	     sAlternAmount [ LG_MAX + 1 ];
	trace_event ("Start Author_OctProc()", PROCESSING);
	/************** Initialisation des structures et variables de travail ***************/
	memset((char *)&rService,     0, sizeof(rService));
	memset (sPrimResource,        0, sizeof(sPrimResource));
	memset (sSecVerifFlag,        0, sizeof(sSecVerifFlag));
	memset (sSecResource,         0, sizeof(sSecResource));
	memset (slCardNumber,         0, sizeof(slCardNumber));
	memset (slProcCode,           0, sizeof(slProcCode));
	memset (slFuncCode,           0, sizeof(slFuncCode));
	memset (sEventCode,           0, sizeof(sEventCode));
	memset (sLine,                0, sizeof(sLine));
	memset (tmpField,             0, sizeof(tmpField));
	memset((char *)&nPid,     0, sizeof(nPid));
	/********** Formattage du buffer TLV A partir du message Iso *************/
	InitTlvAutho(&sTlvInfo);
	nRetCode = IsoToTlv ( P_nIndexCtx, P_psIsoInfo , &sTlvInfo );
	if ( nRetCode != OK )
	{
		trace_event ( "IsoToTlv Error ...", ERROR );
		InsertIsoField ( ISO_RESPONSE_CODE, P_psIsoInfo, MALFUNCTION_ACTION, 3 );
		trace_event ("End   Author_OctProc(NOK)", PROCESSING);
		return ( NOK );
	}
	PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR, &sTlvInfo, P_sAuthoTable,              3);
	PutTlvAutho ( AUTO_CAPTURE_CODE,        &sTlvInfo, P_sMyResource,              6);
	PutTlvAutho ( AUTO_ACQUIRER_BANK,       &sTlvInfo, P_sAcquirerBank,            6);
	PutTlvAutho ( AUTO_INTER_ROUT_CHECK,    &sTlvInfo, P_sInternationalRoutCheck,  1);
	PutTlvAutho ( AUTO_ROUTING_CODE,        &sTlvInfo, STAND_IN_RESOURCE,          6); 
	/** ********************************************** **/
	AuthoBuildTlv (P_sTlvBuffer, &sTlvInfo);
	PrintTlvBuffer (P_sTlvBuffer);
	/*************** Recherche si Authorisation est Duplicatee ****************/
	nRetCode = GetOriginalDataElements ( P_nIndexCtx,
		CAPTURE_MODE,
		P_sTlvBuffer,
		sPrimResource);
	if ( nRetCode == OK )
	{
		trace_event ( "Found a Duplicate Request. ", PROCESSING );
		/* ajouter le code reponse "100" a P_psIsoInfo */
		PutIsoField (ISO_RESPONSE_CODE, P_psIsoInfo, "100", 3);
		trace_event ("End   Author_OctProc(NOK)", PROCESSING);
		return ( NOK );
	}
	/*************** Controle des champs TLV ****************/
	memset (tmpField,             0, sizeof(tmpField));
	if( GetIsoField (ISO_TRANS_AMOUNT,      P_psIsoInfo, tmpField, &nLength) == SUCCESS)
	{
		if(atoi(tmpField ) == 0)
		{
			PutTlvBuffer ( AUTO_RESPONSE_CODE, P_sTlvBuffer, "110", 3);
			OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
				P_sMyResource, sPrimResource, 'S');
			return(NOK);
		}
	}
	else
	{
		PutTlvBuffer ( AUTO_RESPONSE_CODE, P_sTlvBuffer, "110", 3);
		OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
			P_sMyResource, sPrimResource, 'S');
		return(NOK);
	}		 
	PrintTlvBuffer ( P_sTlvBuffer );
	/************ Load Issuer Data Info Including Routing Info *************/
	nRetCode = LoadIssuerData ( P_nIndexCtx,
		P_sTlvBuffer,
		P_sInternationalRoutCheck,
		sPrimResource,
		sSecResource);

	PrintTlvBuffer ( P_sTlvBuffer );
	sprintf ( sLine, "Primary Resource    ...%.6s ", sPrimResource);
	trace_event( sLine, PROCESSING );
	sprintf ( sLine, "Secondary Resource  ...%.6s ", sSecResource);
	trace_event( sLine , PROCESSING );
	if ( nRetCode != OK )
	{
		OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
			P_sMyResource, sPrimResource, 'N');
		return(NOK);
	}
	PutTlvBuffer ( AUTO_ROUTING_CODE, P_sTlvBuffer, sPrimResource, 6);
	/************** Controle du service /r ressource *****************/
	GetIsoField (ISO_CARD_NBR,      P_psIsoInfo, slCardNumber, &nLength);
	GetIsoField (ISO_PROC_CODE,     P_psIsoInfo, slProcCode,   &nLength);
	GetIsoField (ISO_FUNCTION_CODE, P_psIsoInfo, slFuncCode,   &nLength);
	sprintf(rService.szMti, "%04d", P_psIsoInfo->nMsgType);
	nLength = 0;
	memcpy(rService.szProcCode, slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szSrcAcc,   slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szDestAcc,  slProcCode + nLength, 2);
	memcpy(rService.szFctCode , slFuncCode, 3);
	nEventCode = CheckResourceServices ( P_nIndexCtx, P_sMyResource, &rService );
	if (nEventCode != OK)
	{
		sprintf ( sEventCode , "%03d", nEventCode );
		PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE, P_sTlvBuffer, "1006", 4 );
		PutTlvBuffer(AUTO_EVENT_CODE, P_sTlvBuffer, sEventCode, 3);
		OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
			P_sMyResource, sPrimResource, 'N' );
		return(NOK);
	}
	/*PutTlvBuffer ( AUTO_ISO3_SMART_OPTION ,      P_sTlvBuffer , "N",  1);
	PutTlvBuffer ( AUTO_ISO3_SMART_PROC_MODE ,   P_sTlvBuffer , "ON-LINE",  7);
	PutTlvBuffer ( AUTO_ROT_MEM_OPTION,          P_sTlvBuffer , "N",  1);
	PutTlvBuffer ( AUTO_ROT_MEM_CTRL_LIMIT,      P_sTlvBuffer , "N",  1);
	PutTlvBuffer ( AUTO_ROT_MEM_CTRL_AVAILABLE,   P_sTlvBuffer , "N",  1);
	PutTlvBuffer ( AUTO_ROT_MEM_SCANF_ISO2ISO3,  P_sTlvBuffer , "N",  1);
	PutTlvBuffer ( AUTO_ROT_MEM_LAST_USAGE_DATE, P_sTlvBuffer , "N",  1);
	PutTlvBuffer ( AUTO_BAL_LEVEL, P_sTlvBuffer , "Y",  1);*/
	/************************ Processing with Primary Resource ***************************/
	/******** 1- Stand In ********IBO20100602 TO BE CHECKED **/
	if ( memcmp ( sPrimResource, STAND_IN_RESOURCE, 6 ) == 0 )
	{
		trace_event ( "Primary Resource: STAND_IN AUTHORISATION PROCESSING", PROCESSING );

		/********* Lecture des flags de securite ************/
		if ( GetSecurityFlags(P_nIndexCtx, P_sTlvBuffer, "OFF") != OK )
		{
			OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
				P_sMyResource , sPrimResource, 'N');
			return(NOK);
		}
		/************ Check OCT Acceptance *************/
		nRetCode = CheckOctAcceptance ( P_nIndexCtx,
			P_sTlvBuffer,
			slCardNumber
			);
		PrintTlvBuffer ( P_sTlvBuffer );
		if ( nRetCode != OK )
		{
			OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
				P_sMyResource, sPrimResource, 'N');
			return(NOK);
		} 
		/******************************************************************************/
		/*          PreControl Procedure                                                  */
		/******************************************************************************/
		if ( PreControl( P_nIndexCtx , P_sTlvBuffer ) != OK )
		{
			OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer , P_psIsoInfo , P_sMyResource , sPrimResource
				, 'N' );
			return(NOK);
		}
		/**** Amount Bill/Settlement Management ****/
		nRetCode = AmountMng ( P_nIndexCtx,
			P_sServGatewayFlag,
			P_sBillCalculFlag,
			P_sTlvBuffer);
		if ( nRetCode != OK )
		{
			trace_event ( "Retour AmountMng NOK ", PROCESSING );
			OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
				P_sMyResource, sPrimResource, 'N' );
			return(NOK);
		}

		if ( CheckLimits(  P_nIndexCtx, "OFF", P_sTlvBuffer, sAlternAmount ) != OK )
			return (  OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer , P_psIsoInfo , 
			P_sMyResource , sPrimResource , 'N' ));

		/* if ( PrepaidCheckAccount   ( P_nIndexCtx, P_sTlvBuffer ) != OK )*/
		if ( OctCheckAccount   ( P_nIndexCtx, P_sTlvBuffer ) != OK )  /* SLA020113 Add Controls to debit cards */
		{
			return (  OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer , P_psIsoInfo , 
				P_sMyResource , sPrimResource , 'N' ));
		}

		PutTlvBuffer ( AUTO_RESPONSE_CODE ,P_sTlvBuffer  , APPROVED_ACTION , 3);
		PrintTlvBuffer ( P_sTlvBuffer );
		return ( OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer , P_psIsoInfo , P_sMyResource , sPrimResource
			, 'N' ));
	}
	/**** 2- National or International Routage *****/
	if( GetTlvBuffer(AUTO_EVENT_CODE, P_sTlvBuffer, sEventCode,&nLength) != SUCCES)
		return ( OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
		P_sMyResource, sPrimResource, 'N' ));

	if ( memcmp ( sEventCode, INTERNATIONAL_ROUTING_EVENT, 3) != 0 )
	{
		/********* Lecture des flags de securite ********/
		if ( GetSecurityFlags( P_nIndexCtx, P_sTlvBuffer, "ON" ) != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE, P_sTlvBuffer, "1006", 4);
			return ( OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
				P_sMyResource, sPrimResource, 'N'));
		}   
		/******************************************************************************/
		/*          PreControl Procedure                                                  */
		/******************************************************************************/
		PutTlvBuffer ( AUTO_ROT_MEM_SCANF_ISO2ISO3,  P_sTlvBuffer , "N",  1);
		if ( PreControl( P_nIndexCtx , P_sTlvBuffer ) != OK )
		{
			OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer , P_psIsoInfo , P_sMyResource , sPrimResource
				, 'N' );
			return(NOK);
		}
		/******************************************************************************/
		/*          Security Verfication                                                  */
		/******************************************************************************/
		nRetCode = CheckOctAcceptance ( P_nIndexCtx,
			P_sTlvBuffer,
			slCardNumber
			);

		PrintTlvBuffer ( P_sTlvBuffer );

		if ( nRetCode != OK )
		{
			OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
				P_sMyResource, sPrimResource, 'N');
			return(NOK);
		} 
		/*** Amount Bill/Settlement Management National ***/
		PrintTlvBuffer ( P_sTlvBuffer);
		nRetCode = AmountMng ( P_nIndexCtx,
			P_sServGatewayFlag,
			P_sBillCalculFlag,
			P_sTlvBuffer);

		if ( nRetCode != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE, P_sTlvBuffer, "1006" , 4 );
			trace_event ( "Retour AmountMng NOK ", PROCESSING );
			OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
				P_sMyResource, sPrimResource, 'N');
			return(NOK);
		}
		/******************************************************************************/
		/* 	    Check Limits                                                          */
		/******************************************************************************/
		if ( CheckLimits(  P_nIndexCtx, "ON", P_sTlvBuffer, sAlternAmount ) != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , P_sTlvBuffer, "1006" , 4 );
			return (  OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer , P_psIsoInfo , 
				P_sMyResource , sPrimResource , 'S' ));
		}
		PrintTlvBuffer ( P_sTlvBuffer);		  
	}
	else
	{
		/*** Amount Bill/Settlement Management International ***/
		nRetCode = AmountMng ( P_nIndexCtx,
			P_sServGatewayFlag,
			P_sBillCalculFlag,
			P_sTlvBuffer);
		if ( nRetCode != OK )
		{
			trace_event ( "Retour AmountMng NOK ", PROCESSING );
			OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
				P_sMyResource, sPrimResource, 'N');
			return(NOK);
		}
		PrintTlvBuffer ( P_sTlvBuffer);
	}
	nRetCode = CheckResource ( P_nIndexCtx, sPrimResource, &nPid );
	if ((nRetCode == OK) && (kill(nPid, 0) == 0  || !memcmp(ResourceId, VRS_RESOURCE,6)))
		/* Le check sur la resource VRS est ajoute car ca appartient a un autre user */
	{
		trace_event ( "Primary Ressource Up ", PROCESSING );
		if ( CardInSaf ( P_nIndexCtx, slCardNumber) != OK )
		{
			return ( AuthorRouting ( P_nIndexCtx, P_sTlvBuffer,
				sPrimResource, nPid, P_psIsoInfo));
		}
		else
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE, P_sTlvBuffer, "1005", 4 );
			if(debug_option == 1 )
				sprintf ( sLine, "Card [%s] Found In Saf File ", slCardNumber);
			else
				sprintf ( sLine, "Card [**********] Found In Saf File ");
			trace_event ( sLine, PROCESSING );
		}
	}
	else
	{
		PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE, P_sTlvBuffer, "1004", 4 );
		trace_event ( "Primary Ressource Down ", PROCESSING );
	}
	/************************ Processing with Secondary Resource ***************************/
	if ( memcmp ( sSecResource, NO_ROUTING_RESOURCE, 6 ) == 0 )
	{
		PutTlvBuffer(AUTO_EVENT_CODE, P_sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
		OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
			P_sMyResource, sPrimResource, 'N');
		return(NOK);
	}
	if ( memcmp ( sSecResource, STAND_IN_RESOURCE, 6 ) == 0 )
	{
		trace_event ( "Secondary Resource : STAND_IN AUTHORISATION PROCESSING", PROCESSING );
		/******** Lecture des flags de securite ******/
		if ( GetSecurityFlags( P_nIndexCtx, P_sTlvBuffer, "OFF" ) != OK )
		{
			OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
				P_sMyResource, sPrimResource, 'N');
			return(NOK);
		}
		return ( OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
			P_sMyResource, sPrimResource, 'S'));
	}
	else
	{
		nRetCode = CheckResource ( P_nIndexCtx, sSecResource , &nPid );
		if ( nRetCode != OK )
		{
			PutTlvBuffer(AUTO_EVENT_CODE, P_sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
			return ( OctAuthorLocal( P_nIndexCtx, P_sTlvBuffer, P_psIsoInfo,
				P_sMyResource , sPrimResource , 'N' ));
		}
		return ( AuthorRouting ( P_nIndexCtx, P_sTlvBuffer,
			sSecResource, nPid, P_psIsoInfo));
	}

	PutTlvBuffer ( AUTO_AUTH_NUMBER,          P_sTlvBuffer , "000001",  6);
	trace_event ("End   Author_OctProc(OK)", PROCESSING);
	return ( OK );
}
/* End ENH 12.2 SLA 130912 Fast Funds */
/***************************************************************************************/
/***************************************************************************************/


/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/



/* End ENH 12.2 SLA 130912 Fast Funds */
/***************************************************************************************/
/******************* Start ENH Money Send HBU250810***************************/
/***************************************************************************************/
int PaymentTrsReqProcessing (   int nIndiceCtx,
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

	TSTlvAutho      sTlvInfo;
	TSService   rService;
	char        slCardNumber[     22 + 1 ];
	char        slProcCode  [      6 + 1 ];
	char        slFuncCode  [      3 + 1 ];
	char        sLine       [MAX_LINE_TRC];
	int         nLength;
	int         nEventCode =  OK;
	int         nRetCode;
	char        sEventCode  [ 3 + 1 ];
	char        sPrimResource  [ 6 + 1 ];
	char        sSecResource   [ 6 + 1 ];
	char        sTlvProcCode [ LG_MAX + 1 ];
	char        sIsoProcCode [ LG_MAX + 1 ];
	char        sSecurityLevel [ 20 + 1 ];
	pid_t 		nPid;    
	char    	sAlternAmount [ LG_MAX + 1 ];    

	trace_event("Start PaymentTrsReqProcessing()",PROCESSING);

	/******************************************************************************/
	/*                  Initialisation                                            */
	/******************************************************************************/

	memset(slCardNumber         , 0, sizeof(slCardNumber));
	memset(slProcCode           , 0, sizeof(slProcCode));
	memset(slFuncCode           , 0, sizeof(slFuncCode));
	memset((char *)&rService    , 0, sizeof(rService));
	memset(sEventCode           , 0, sizeof(sEventCode));
	memset(sPrimResource        , 0, sizeof(sPrimResource));
	memset(sSecResource         , 0, sizeof(sSecResource));
	memset(sSecurityLevel , 0,  sizeof ( sSecurityLevel )); 

	memset(sAlternAmount , 0,  sizeof ( sAlternAmount ));    

	/******************************************************************************/
	/*  Formatter le buffer TLV A partir du message Iso                           */
	/******************************************************************************/

	InitTlvAutho(&sTlvInfo);
	nRetCode = IsoToTlv ( nIndiceCtx , IsoInfo , &sTlvInfo );
	if ( nRetCode != OK )
	{
		trace_event ( "IsoToTlv Error ...", ERROR );
		InsertIsoField ( ISO_RESPONSE_CODE , IsoInfo , MALFUNCTION_ACTION , 3 );
		return ( NOK );
	}

	PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable ,     3);
	PutTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sMyResource ,     6);
	PutTlvAutho ( AUTO_ACQUIRER_BANK , &sTlvInfo , sAcquirerBank ,     6);
	PutTlvAutho ( AUTO_INTER_ROUT_CHECK,&sTlvInfo,InternationalRoutCheck,1);
	PutTlvAutho ( AUTO_ROUTING_CODE, &sTlvInfo , STAND_IN_RESOURCE ,  6);
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
		trace_event ( "Found a Duplicate Request. ", PROCESSING );
		InitTlvAutho    ( &sTlvInfo );
		AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
		TlvToIso        ( nIndiceCtx , &sTlvInfo   , IsoInfo  , RESPONSE );
		return ( OK );
	}

	/******************************************************************************/
	/*      Controle des champs  TLV                                              */
	/******************************************************************************/

	if ( CheckReqTlvFields ( nIndiceCtx, sTlvBuffer ) != OK ){
		trace_event("End PaymentTrsReqProcessing (1)", PROCESSING);
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
			sMyResource , sPrimResource , 'S' ));
	}

	/******************************************************************************/
	/*            Load Issuer Data Info Including Routing Info                    */
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

	if ( nRetCode != OK ){
		trace_event("End PaymentTrsReqProcessing (2)", PROCESSING);
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
			sMyResource , sPrimResource , 'N' ));
	}

	PutTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource ,  6);

	/*****************************************************************************/
	/*  Cette Modif Etait Faite Pour Europay Pour Changer Le processing Code     */
	/*   Des Cartes Mestro En cas de Cash Advance Voir PCRD_P7_ROUTING           */
	/*****************************************************************************/

	memset(sTlvProcCode, 0, sizeof(sTlvProcCode));
	memset(sIsoProcCode, 0, sizeof(sIsoProcCode));
	if(   GetTlvBuffer(AUTO_PROCESSING_CODE,sTlvBuffer,sTlvProcCode,&nLength) == SUCCES)
	{
		if (GetIsoField (ISO_PROC_CODE,     IsoInfo, sIsoProcCode   ,&nLength) == SUCCES)
		{
			sprintf(sLine," PROCESSING CODE TLV %s", sTlvProcCode);
			trace_event(sLine,TRACE);

			sprintf(sLine," PROCESSING CODE ISO %s", sIsoProcCode);
			trace_event(sLine,TRACE);
			memcpy(sIsoProcCode, sTlvProcCode + 6, 2);
			sprintf(sLine," PROCESSING CODE ISO %s", sIsoProcCode);
			trace_event(sLine,TRACE);
			PutIsoField ( ISO_PROC_CODE , IsoInfo , sIsoProcCode , 6 );
		}
	}

	/******************************************************************************/
	/*                        Controle du service r ressource                     */
	/******************************************************************************/

	GetIsoField (ISO_CARD_NBR ,     IsoInfo, slCardNumber ,&nLength);
	GetIsoField (ISO_PROC_CODE,     IsoInfo, slProcCode   ,&nLength);
	GetIsoField (ISO_FUNCTION_CODE, IsoInfo, slFuncCode   ,&nLength);

	sprintf(rService.szMti, "%04d", IsoInfo->nMsgType);

	nLength = 0;
	memcpy(rService.szProcCode, slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szSrcAcc,   slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szDestAcc,  slProcCode + nLength, 2);
	memcpy(rService.szFctCode , slFuncCode , 3);

	nEventCode = CheckResourceServices ( nIndiceCtx, sMyResource, &rService );
	if (nEventCode != OK)
	{
		sprintf ( sEventCode , "%03d", nEventCode );
		PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode ,     3);
		trace_event("End PaymentTrsReqProcessing (3)", PROCESSING);
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
			sMyResource , sPrimResource , 'N' ));
	}

	/******************************************************************************/
	/******************************************************************************/
	/*                        le Traitement StandIn                               */
	/******************************************************************************/
	/******************************************************************************/

	if ( memcmp ( sPrimResource , STAND_IN_RESOURCE , 6 ) == 0 )
	{
		trace_event ( "Prm : STAND_IN AUTHORISATION PROCESSING", PROCESSING );
		/******************************************************************************/
		/*      Lecture des flags de securite                                         */
		/******************************************************************************/
		if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "OFF" ) != OK ){
			trace_event("End PaymentTrsReqProcessing (4)", PROCESSING);
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
				sMyResource , sPrimResource , 'N' ));
		}

		/******************************************************************************/
		/*      PreControl Procedure                                                  */
		/******************************************************************************/

		PrintTlvBuffer ( sTlvBuffer );

		if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK ){
			trace_event("End PaymentTrsReqProcessing (5)", PROCESSING);
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
				sMyResource , sPrimResource , 'N' ));
		}

		/******************************************************************************/
		/*      Security Verification                                                  */
		/******************************************************************************/

		if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK ){
			trace_event("End PaymentTrsReqProcessing (6)", PROCESSING);
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
				sMyResource , sPrimResource , 'N' ));
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
			trace_event ( "Retour AmountMng NOK ", PROCESSING );
			trace_event("End PaymentTrsReqProcessing (7)", PROCESSING);
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
				sMyResource , sPrimResource , 'N' ));
		}

		PrintTlvBuffer ( sTlvBuffer );

		/******************************************************************************/
		/*      Check Account                                                         */
		/******************************************************************************/

		if ( CheckAccAvailable   ( nIndiceCtx, sTlvBuffer ) != OK )
		{
			trace_event("End PaymentTrsReqProcessing (8)", PROCESSING);
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
				sMyResource , sPrimResource , 'N' ));
		}

		trace_event("End PaymentTrsReqProcessing (OK)", PROCESSING);
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
			sMyResource , sPrimResource , 'N' ));
	}

	/******************************************************************************/
	/* Check If Routing is National Or International   Not Permetted For EWB      */
	/******************************************************************************/

	if( GetTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,sEventCode,&nLength) != SUCCES){
		trace_event("End PaymentTrsReqProcessing (9)", PROCESSING);
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo ,
			sMyResource , sPrimResource , 'N' ));
	}




	if ( memcmp ( sEventCode , INTERNATIONAL_ROUTING_EVENT , 3 ) != 0 )
	{
		/******************************************************************************/
		/* 	    Lecture des flags de securite                                         */
		/******************************************************************************/
		if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "ON" ) != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'S' ));
		}

		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'S' ));
		}

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'S' ));
		}

		/******************************************************************************/
		/*  Amount Bill/Settlement Management                                         */ 
		/******************************************************************************/
		PrintTlvBuffer ( sTlvBuffer );
		nRetCode = AmountMng (    nIndiceCtx          ,
			sServGatewayFlag	  ,
			sBillCalculFlag	  ,
			sTlvBuffer          
			);
		if ( nRetCode != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
			trace_event ( "Retour AmountMng NOK ", PROCESSING );
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'S' ));
		}

		PrintTlvBuffer ( sTlvBuffer );

		/******************************************************************************/
		/* 	    Check Limits                                                          */
		/******************************************************************************/
		if ( CheckLimits(  nIndiceCtx, "ON", sTlvBuffer, sAlternAmount ) != OK )
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'S' ));
		}
	}
	else
	{
		/******************************************************************************/
		/*  Amount Bill/Settlement Management International                           */ 
		/******************************************************************************/
		nRetCode = AmountMng (    nIndiceCtx          ,
			sServGatewayFlag	  ,
			sBillCalculFlag	  ,
			sTlvBuffer          
			);
		if ( nRetCode != OK )
		{
			trace_event ( "Retour AmountMng NOK ", PROCESSING );
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
				sMyResource , sPrimResource , 'N' ));
		}

		PrintTlvBuffer ( sTlvBuffer );

	}    


	nRetCode = CheckResource ( nIndiceCtx, sPrimResource , &nPid );
	if  ( (  nRetCode == OK  ) || (kill ( nPid , 0 ) != 0) )
	{
		trace_event ( "Primary Ressource Up ", PROCESSING ); 
		if ( CardInSaf ( nIndiceCtx , slCardNumber ) != OK )
		{
			return( AuthorRouting(nIndiceCtx,sTlvBuffer,
				sPrimResource,nPid,IsoInfo));
		}
		else
		{
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1005" , 4 );
		}
	}
	else
	{
		PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1004" , 4 );
		trace_event ( "Primary Ressource Down ", PROCESSING ); 
	}    

	if ( memcmp ( sSecResource , NO_ROUTING_RESOURCE , 6 ) == 0 )
	{
		PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT,3);
		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'N' ));
	}

	if ( memcmp ( sSecResource , STAND_IN_RESOURCE , 6 ) == 0 )
	{
		trace_event ( "Sec : STAND_IN AUTHORISATION PROCESSING", PROCESSING );
		/******************************************************************************/
		/* 	    Lecture des flags de securite                                         */
		/******************************************************************************/
		if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "OFF" ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));

		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));

		/******************************************************************************/
		/* 	    Check Limits                                                          */
		/******************************************************************************/
		if ( CheckLimits(  nIndiceCtx, "OFF", sTlvBuffer, sAlternAmount ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));

		/******************************************************************************/
		/* 	    Check Account                                                         */
		/******************************************************************************/
		if ( CheckAccAvailable   ( nIndiceCtx, sTlvBuffer ) != OK )
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));

		return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
			sMyResource , sPrimResource , 'S' ));
	}
	else
	{
		nRetCode = CheckResource ( nIndiceCtx, sSecResource , &nPid );
		if ( (nRetCode != OK) || (kill ( nPid , 0 ) != 0) ) /*YK 240304 : second ressource not up*/
		{
			PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT,3);
			return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , sMyResource , sPrimResource , 'N' ));
		}
		return( AuthorRouting ( nIndiceCtx,sTlvBuffer,
			sSecResource,nPid,IsoInfo ));
	}
	trace_event("End PaymentTrsReqProcessing (END OK)", PROCESSING);
	return ( OK );    
}
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/

