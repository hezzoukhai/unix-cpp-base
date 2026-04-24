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

/***************************************************************
  **  nom   : CardLessAccountVerifProcessing	 		**
  **  desc   : Fonction to verify the Account number          	**
  **  entree   : aucune                  			**
  **  sortie   : aucune                  			**
  **  retour   : OK si c'est OK               		  	**
  **        		 NOK dans le cas contraire      	**
  **	Author 	 : L.HIDRAOUI					**
  **************************************************************/
  
int CardLessAccountVerifProcessing	(	int nIndiceCtx,
				        char *sMyResource,
				        char *sAuthoTable,
                                	char *sAcquirerBank,
                                	char *InternationalRoutCheck, 
					char *sServGatewayFlag,
				        char *sBillCalculFlag,
                                        char *sTlvBuffer ,
                                        TSIsoInfo *sIsoMsg  )
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
	pid_t 		nPid;
	char 		sPid           [ 6 + 1 ];
	char 		sInternalStan  [ 6 + 1 ];
	char 		cOnOffMode;
        char    	sAlternAmount [ LG_MAX + 1 ];
	
    trace_event("Start CardLessAccountVerifProcessing()",PROCESSING);
/******************************************************************************/
/* 					Initialisation			*/
/******************************************************************************/

	memset(sSecurData 			, 0, sizeof(sSecurData));
	memset(slCardNumber			, 0, sizeof(slCardNumber));
	memset(slProcCode			, 0, sizeof(slProcCode));
	memset(slFuncCode			, 0, sizeof(slFuncCode));
	memset((char *)&rService                , 0, sizeof(rService));
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
    InitTlvAutho(&sTlvInfo);
    memset( sTlvBuffer   	    , 0, sizeof ( sTlvBuffer ));
    memset( sTlvBuffer   	    , 0, LG_MAX );
    nRetCode = IsoToTlv ( nIndiceCtx , sIsoMsg , &sTlvInfo );
    if ( nRetCode != OK )
    {
            trace_event ( "IsoToTlv Error ...", ERROR );
            PutIsoField ( ISO_RESPONSE_CODE , sIsoMsg , MALFUNCTION_ACTION , 3 );
            return ( NOK );
    }
    PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable ,     3);
    PutTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sMyResource ,     6);
    PutTlvAutho ( AUTO_ACQUIRER_BANK , &sTlvInfo , sAcquirerBank ,     6);
    PutTlvAutho ( AUTO_INTER_ROUT_CHECK,&sTlvInfo,InternationalRoutCheck,1);
    PutTlvAutho ( AUTO_ROUTING_CODE         , &sTlvInfo , STAND_IN_RESOURCE ,  6);
    
    AuthoBuildTlv( sTlvBuffer, &sTlvInfo);
    PrintTlvBuffer(sTlvBuffer);


	nRetCode = LoadIssuerData (   nIndiceCtx          ,
                        	      sTlvBuffer      ,
				      InternationalRoutCheck ,
				      sPrimResource , 
				      sSecResource
                   		   );

    sprintf (  sLine , "Primary Resource    ...%.6s ", sPrimResource );
    trace_event( sLine , PROCESSING );
    sprintf (  sLine , "Secondarry Resource ...%.6s ", sSecResource  );
    trace_event( sLine , PROCESSING );
    if ( nRetCode != OK )
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));
   PutTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource ,  6);
   

    GetIsoField (ISO_CARD_NBR ,     sIsoMsg, slCardNumber ,&nLength);
    GetIsoField (ISO_PROC_CODE,     sIsoMsg, slProcCode   ,&nLength);
    GetIsoField (ISO_FUNCTION_CODE, sIsoMsg, slFuncCode   ,&nLength);
    sprintf(rService.szMti, "%04d", sIsoMsg->nMsgType);
		
    PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, "000" ,     3);
    
    PrintTlvBuffer(sTlvBuffer);
/*
	if ( CheckCardInquiry   ( nIndiceCtx, sTlvBuffer ) != OK )
      	 {
      		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, 909 ,     3);
              	return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , sMyResource , sPrimResource , 'N' ));
       	 }
*/        	 
   	return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , sMyResource , sPrimResource , 'N' ));

   	return ( OK );
}
