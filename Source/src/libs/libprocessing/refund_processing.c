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
int RefundRequestProcessing	(	int nIndiceCtx,
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
    char   		sSecurityResults[ 11 ];
	char            tmp_buffer [ LG_MAX ];
	TSService	rService;
	char		slCardNumber[     22 + 1 ];
	char		slProcCode	[      6 + 1 ];
	char		slFuncCode	[      3 + 1 ];
	char		sLine		[MAX_LINE_TRC];
    int     	nLength;
	int 		nEventCode =  OK;
	int 		nRetCode;
	char 		sEventCode  [ 3 + 1 ];
	char 		sPrimResource  [ 6 + 1 ];
	char 		sSecResource   [ 6 + 1 ];
	/*long 		nPid;*/
	pid_t   nPid; /**MKB160408*/
	char 		sPid           [ 8 + 1 ];
	char 		sInternalStan  [ 6 + 1 ];
	char 		cOnOffMode;
    char    	sAlternAmount [ LG_MAX + 1 ];
    char    	sTlvProcCode [ LG_MAX + 1 ];
    char    	sIsoProcCode [ LG_MAX + 1 ];
	
    trace_event("Start RefundRequestProcessing()",PROCESSING);
/******************************************************************************/
/* 					Initialisation			*/
/******************************************************************************/

	memset(sSecurData 			, 0, sizeof(sSecurData));
	memset(slCardNumber			, 0, sizeof(slCardNumber));
	memset(slProcCode			, 0, sizeof(slProcCode));
	memset(slFuncCode			, 0, sizeof(slFuncCode));
	memset((char *)&rService    , 0, sizeof(rService));
	memset(sEventCode           , 0, sizeof(sEventCode));
	memset(sPrimResource     	, 0, sizeof(sPrimResource));
	memset(sSecResource   		, 0, sizeof(sSecResource));
	memset(sPid                 , 0, sizeof(sPid));
    memset(sSecurityResults , 0,  sizeof ( sSecurityResults ));
	memset(sInternalStan        , 0, sizeof(sInternalStan));
    memset(sAlternAmount , 0,  sizeof ( sAlternAmount ));


/******************************************************************************/
/*  Formatter le buffer TLV A partir du message Iso                           */
/******************************************************************************/
	trace_event("Start RefundRequestProcessing()",PROCESSING);
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

/******************************************************************************/
/* 		Load Issuer Data Info Including Routing Info                    */
/******************************************************************************/


    PrintTlvBuffer ( sTlvBuffer );
	nRetCode = LoadIssuerData (  nIndiceCtx          ,
                        		 sTlvBuffer          ,
				   				 InternationalRoutCheck ,
				      			 sPrimResource , 
				   				 sSecResource
                   				);

    sprintf (  sLine , "Primary Resource    ...%.6s ", sPrimResource );
    trace_event( sLine , PROCESSING );
    sprintf (  sLine , "Secondarry Resource ...%.6s ", sSecResource  );
    trace_event( sLine , PROCESSING );
    if ( nRetCode != OK )
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
                                        sMyResource , sPrimResource , 'N' ));
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
              sprintf ( sEventCode , "%03d", nEventCode );
	          PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode ,     3);
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
                                        sMyResource , sPrimResource , 'N' ));
	}


/******************************************************************************/
    if( GetTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,sEventCode,&nLength) != SUCCES)
         return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
                                           sMyResource , sPrimResource , 'N' ));
/******************************************************************************/
/* 				          le Traitement StandIn Or Routage Card Range         */
/******************************************************************************/
/******************************************************************************/
   if ( memcmp ( sPrimResource , STAND_IN_RESOURCE , 6 ) == 0 || memcmp ( sEventCode , INTERNATIONAL_ROUTING_EVENT , 3 ) != 0 ) 
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
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , IsoInfo , 
                                sMyResource , sPrimResource , 'N' ));

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

  }
  else
  {
    /* Traitement International  */
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
   }
/* Recuperation Numero Authorisation */
/* Insertion Autho Activity */
/* Pas De Mise A jour Card Activity */
/* Pas De Mise A jour  Balance */

    nRetCode = RefundProcessing( nIndiceCtx, sTlvBuffer , IsoInfo );
	trace_event("End   RefundRequestProcessing(OK)",PROCESSING);
   return ( OK );
}
