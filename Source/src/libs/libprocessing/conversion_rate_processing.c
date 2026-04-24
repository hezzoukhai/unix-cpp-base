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

/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les demandes d'autorisations                         */
/****************************************************************************/
/****************************************************************************/
int ConversionRateProcessing	(int nIndiceCtx,
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
	char 		sPid           [ 8 + 1 ];
	char 		sInternalStan  [ 6 + 1 ];
	char 		cOnOffMode;
    	char    	sAlternAmount [ LG_MAX + 1 ];
	
    	trace_event("Start ConversionRateProcessing()",PROCESSING);
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
            InsertIsoField ( ISO_RESPONSE_CODE , sIsoMsg , MALFUNCTION_ACTION , 3 );
            return ( NOK );
    }
    PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable ,     3);
    PutTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sMyResource ,     6);
    PutTlvAutho ( AUTO_ACQUIRER_BANK , &sTlvInfo , sAcquirerBank ,     6);
    PutTlvAutho ( AUTO_INTER_ROUT_CHECK,&sTlvInfo,InternationalRoutCheck,1);
    AuthoBuildTlv( sTlvBuffer, &sTlvInfo);
    PrintTlvBuffer(sTlvBuffer);

/******************************************************************************/
/*  Recherche si Authorisation est Duplicate                                  */
/******************************************************************************/
     nRetCode = GetOriginalDataElements(  nIndiceCtx ,CAPTURE_MODE,
                                          sTlvBuffer ,
                                          sPrimResource );
     if ( nRetCode == OK )
     {
           trace_event ( "Found a Duplicate Request. ", PROCESSING );
         /**Begin MSA03042008*/
          /* In case of Duplicate, over-write ACTION_CODE & AUTHORIZATION_CODE */
           /* EBE140523 PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, DUPLICATE_AUTHO, 3);*/
		   PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, "308", 3);
           PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, "      ", 6);
         /**END MSA03042008**/
           InitTlvAutho    ( &sTlvInfo );
           AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
           TlvToIso        ( nIndiceCtx , &sTlvInfo   , sIsoMsg , RESPONSE );
           return ( OK );
     }

/******************************************************************************/
/* 	    Controle des champs  TLV                                              */
/******************************************************************************
    if ( CheckReqTlvFields ( nIndiceCtx, sTlvBuffer ) != OK )
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));
******************************************************************************/

/******************************************************************************/
/* 		Load Issuer Data Info Including Routing Info                    */
/******************************************************************************/

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
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));
   PutTlvBuffer ( AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource ,  6);
/******************************************************************************/
/*                        Controle du service /r ressource   */
/******************************************************************************/
    GetIsoField (ISO_CARD_NBR ,     sIsoMsg, slCardNumber ,&nLength);
    GetIsoField (ISO_PROC_CODE,     sIsoMsg, slProcCode   ,&nLength);
    GetIsoField (ISO_FUNCTION_CODE, sIsoMsg, slFuncCode   ,&nLength);
    sprintf(rService.szMti, "%04d", sIsoMsg->nMsgType);

    nLength = 0;
    memcpy(rService.szProcCode, slProcCode + nLength, 2); nLength += 2;
    memcpy(rService.szSrcAcc,   slProcCode + nLength, 2); nLength += 2;
    memcpy(rService.szDestAcc,  slProcCode + nLength, 2);
    memcpy(rService.szFctCode , slFuncCode , 3);

    nEventCode = CheckResourceServices ( nIndiceCtx, sMyResource, &rService );
    if (nEventCode != OK)
    {
              sprintf ( sEventCode , "%03d", nEventCode );
              PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode ,     3);
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg ,
                                        sMyResource , sPrimResource , 'N' ));
    }


/******************************************************************************/
/******************************************************************************/
/* 				          le Traitement StandIn                               */
/******************************************************************************/
/******************************************************************************/
   PrintTlvBuffer(sTlvBuffer);
   if ( memcmp ( sPrimResource , STAND_IN_RESOURCE , 6 ) == 0 )
   {
        trace_event ( "Prm : STAND_IN AUTHORISATION PROCESSING", PROCESSING );
/******************************************************************************/
/* 	    Lecture des flags de securite                                         */
/******************************************************************************/
        if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "OFF" ) != OK )
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));

/******************************************************************************/
/* 	    PreControl Procedure                                                  */
/******************************************************************************/
        if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));

/******************************************************************************/
/* 	    Security Verfication                                                  */
/******************************************************************************/
        if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                sMyResource , sPrimResource , 'N' ));

/******************************************************************************/
/*      Conversion Rate Processing                                            */
/******************************************************************************/
        PrintTlvBuffer(sTlvBuffer);
        if ( ConvRateInquiry   ( nIndiceCtx, sTlvBuffer ) != OK )
        {
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg ,
                                     sMyResource , sPrimResource , 'N' ));
         }

        PrintTlvBuffer(sTlvBuffer);

      return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                     sMyResource , sPrimResource , 'N' ));

  }


/******************************************************************************/
/* Check If Routing is National Or International                              */
/******************************************************************************/

    if( GetTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,sEventCode,&nLength) != SUCCES)
         return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                           sMyResource , sPrimResource , 'N' ));
/******************************************************************************/
/* PreControl To Run Before Sending  Request to HOST/BANK                     */
/******************************************************************************/

   if ( memcmp ( sEventCode , INTERNATIONAL_ROUTING_EVENT , 3 ) != 0 )
   {
/******************************************************************************/
/* 	    Lecture des flags de securite                                         */
/******************************************************************************/
        if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "ON" ) != OK )
		{
              PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'S' ));
		}

/******************************************************************************/
/* 	    PreControl Procedure                                                  */
/******************************************************************************/
        if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
		{
              PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'S' ));
		}

/******************************************************************************/
/* 	    Security Verfication                                                  */
/******************************************************************************/
        if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
		{
              PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1006" , 4 );
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                sMyResource , sPrimResource , 'S' ));
		}

   }

   nRetCode = CheckResource ( nIndiceCtx, sPrimResource , &nPid );
   if ( ( nRetCode != OK ) || ( kill ( nPid , 0 ) != 0 ))
   {
       trace_event ( "Primary Ressource Down ", PROCESSING ); 
       if ( memcmp ( sSecResource , NO_ROUTING_RESOURCE , 6 ) == 0 )
       {
			PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1004" , 4 );
            PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT,3);
        	return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                              sMyResource , sPrimResource , 'N' ));
        }

         else
         if ( memcmp ( sSecResource , STAND_IN_RESOURCE , 6 ) == 0 )
         {
		   PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1004" , 4 );
           trace_event ( "Sec : STAND_IN AUTHORISATION PROCESSING", PROCESSING );
/******************************************************************************/
/* 	    Lecture des flags de securite                                         */
/******************************************************************************/
           if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "OFF" ) != OK )
                 return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'S' ));

/******************************************************************************/
/* 	    PreControl Procedure                                                  */
/******************************************************************************/
           if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
                return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'S' ));

/******************************************************************************/
/* 	    Security Verfication                                                  */
/******************************************************************************/
           if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                      sMyResource , sPrimResource , 'S' ));

/******************************************************************************/
/*      Conversion Rate Processing                                            */
/******************************************************************************/
        PrintTlvBuffer(sTlvBuffer);
        if ( ConvRateInquiry   ( nIndiceCtx, sTlvBuffer ) != OK )
        {
              return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg ,
                                     sMyResource , sPrimResource , 'S' ));
         }
        PrintTlvBuffer(sTlvBuffer);

          return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                     sMyResource , sPrimResource , 'S' ));
        }
        else
        {
			nRetCode = CheckResource ( nIndiceCtx, sSecResource , &nPid );
		    if ( nRetCode != OK )
   		    {
               PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, "1004" , 4 );
		       PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,
									ISSUER_NOT_AVAILABLE_EVENT,3);
        	    return (  AuthorLocalAdm( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                       sMyResource , sPrimResource , 'S' ));
            }
		    return( AuthorRouting ( nIndiceCtx,sTlvBuffer,
									sSecResource,nPid,sIsoMsg ));
          }
   }
   else
   {
   trace_event ( "Primary Ressource Up ", PROCESSING ); 
   return( AuthorRouting(nIndiceCtx,sTlvBuffer,
									sPrimResource,nPid,sIsoMsg));
   }
   return ( OK );
}

