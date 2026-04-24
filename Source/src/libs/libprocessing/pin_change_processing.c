/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*                                                                             */
/*                                                                             */
/* Description                                                                 */
/*                                                                             */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initial Version	
---------------------------------------------------------------------------------------------------------------------------------*/
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

#define PIN_METHODE_PVV 	'2'
#define PIN_METHODE_OFFSET  '3'

/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les demandes d'autorisations                         */
/****************************************************************************/
/****************************************************************************/
int PinChangeProcessing	(int nIndiceCtx,
				         char *sMyResource,
				 	     char *sAuthoTable, 
						 char *sAcquirerBank,
                         char *InternationalRoutCheck, 
						 char *sServGatewayFlag,
				         char *sBillCalculFlag,
                         char *sTlvBuffer , TSIsoInfo *sIsoMsg  )
{

	TSResources     sResource;
	TSTlvAutho      sTlvInfo;
	char            sSecurData 	[ 128 ];
    char   		    sSecurityResults[ 11 ];
	char            tmp_buffer 	[ 1500 ];
	TSService		rService;
	char			slCardNumber	[     22 + 1 ];
	char			slProcCode		[      6 + 1 ];
	char			slFuncCode		[      3 + 1 ];
	char			sLine			[MAX_LINE_TRC];
    int     		nLength;
	int 			nEventCode =  OK;
	int 			nRetCode;
	char 			sRetCode		[4];
	char 			sEventCode  	[ 3 + 1 ];
	char 			sPrimResource  	[ 6 + 1 ];
	char 			sSecResource   	[ 6 + 1 ];
	/*long 		nPid;*/
	pid_t   nPid; /**MKB160408*/
	char 			sPid           	[ 8 + 1 ];
	char 			sInternalStan  	[ 6 + 1 ];
	char 			cOnOffMode;
    char    		sAlternAmount [ 128+ 1 ];
	
    trace_event("Start PinChangeProcessing()",PROCESSING);

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
    memset(sSecurityResults     , 0,  sizeof ( sSecurityResults ));
	memset(sInternalStan        , 0, sizeof(sInternalStan));
    memset(sAlternAmount 		, 0,  sizeof ( sAlternAmount ));


/******************************************************************************/
/*  Formatter le buffer TLV A partir du message Iso                           */
/******************************************************************************/
	InitTlvAutho(&sTlvInfo);
    memset( sTlvBuffer   	    , 0, sizeof ( sTlvBuffer ));
    nRetCode = IsoToTlv ( nIndiceCtx , sIsoMsg , &sTlvInfo );
    if ( nRetCode != OK )
    {
            trace_event ( "IsoToTlv Error ...", ERROR );
            InsertIsoField ( ISO_RESPONSE_CODE , sIsoMsg , MALFUNCTION_ACTION , 3 );
            return ( NOK );
    }

    PutTlvAutho ( AUTO_CUR_TABLE_INDICATOR , &sTlvInfo, sAuthoTable ,     3);
    PutTlvAutho ( AUTO_CAPTURE_CODE        , &sTlvInfo, sMyResource ,     6);
    PutTlvAutho ( AUTO_ACQUIRER_BANK       , &sTlvInfo , sAcquirerBank ,     6);
    PutTlvAutho ( AUTO_INTER_ROUT_CHECK,&sTlvInfo,InternationalRoutCheck,1);
    AuthoBuildTlv( sTlvBuffer, &sTlvInfo);


/******************************************************************************/
/* 				          Controle du service /r ressource   */
/******************************************************************************/
	GetIsoField (ISO_PROC_CODE, 	sIsoMsg, slProcCode ,&nLength);
	GetIsoField (ISO_FUNCTION_CODE, sIsoMsg, slFuncCode , &nLength);
	sprintf(rService.szMti, "%04d", sIsoMsg->nMsgType); 

	nLength = 0;
	memcpy(rService.szProcCode, 	slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szSrcAcc, 	slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szDestAcc, 	slProcCode + nLength, 2); 
	memcpy(rService.szFctCode , 	slFuncCode , 3); 

	nEventCode = CheckResourceServices ( nIndiceCtx, sMyResource, &rService );
	if (nEventCode != OK) 
    {
              sprintf ( sEventCode , "%03d", nEventCode );
	          PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode ,     3);
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));
	}

/******************************************************************************/
/* 	    Controle des champs  TLV                                              */
/******************************************************************************/
    if ( CheckReqTlvFields ( nIndiceCtx, sTlvBuffer ) != OK )
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));

/******************************************************************************/
/* 		Load Issuer Data Info Including Routing Info                    */
/******************************************************************************/
    /* trace_event (  sTlvBuffer , PROCESSING );  PCI*/
    trace_event ( "AVANT Load Issuer Data", PROCESSING );
	fprintf(stderr," \n AVANT LOAD ISSUER DATA \n");
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
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));

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
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));

/******************************************************************************/
/* 	    PreControl Procedure                                                  */
/******************************************************************************/
        if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));

/******************************************************************************/
/* 	    Security Verfication                                                  */
/******************************************************************************/
        if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                sMyResource , sPrimResource , 'N' ));
    
	   nRetCode = PinChange_Inquiry(nIndiceCtx,sTlvBuffer);
	   sprintf(sRetCode,"%03d",nRetCode);
       PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,sRetCode, 3); 
      
      return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                     sMyResource , sPrimResource , 'N' ));
  }


/******************************************************************************/
/* Check If Routing is National Or International                              */
/******************************************************************************/

    if( GetTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,sEventCode,&nLength) != SUCCES)
         return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                           sMyResource , sPrimResource , 'N' ));
/******************************************************************************/
/* PreControl To Run Before Sending  Request to HOST/BANK                     */
/******************************************************************************/

   if ( memcmp ( sEventCode , INTERNATIONAL_ROUTING_EVENT , 3 ) != 0 )
   {
        trace_event ( "PreControl To Run Before Send  AUTHORISATION PROCESSING", PROCESSING );
/******************************************************************************/
/* 	    Lecture des flags de securite                                         */
/******************************************************************************/
        if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "ON" ) != OK )
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));

/******************************************************************************/
/* 	    PreControl Procedure                                                  */
/******************************************************************************/
        if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));

/******************************************************************************/
/* 	    Security Verfication                                                  */
/******************************************************************************/
        if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                sMyResource , sPrimResource , 'N' ));

   }

   nRetCode = CheckResource ( nIndiceCtx, sPrimResource , &nPid );
   if ( ( nRetCode != OK ) || ( kill ( nPid , 0 ) != 0 ))
   {
       trace_event ( "Primary Ressource Down ", PROCESSING ); 
       if ( memcmp ( sSecResource , NO_ROUTING_RESOURCE , 6 ) == 0 )
       {
            PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT,3);
        	return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                              sMyResource , sPrimResource , 'N' ));
        }

         else
         if ( memcmp ( sSecResource , STAND_IN_RESOURCE , 6 ) == 0 )
         {
           trace_event ( "Sec : STAND_IN AUTHORISATION PROCESSING", PROCESSING );
/******************************************************************************/
/* 	    Lecture des flags de securite                                         */
/******************************************************************************/
           if ( GetSecurityFlags( nIndiceCtx , sTlvBuffer , "OFF" ) != OK )
                 return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));

/******************************************************************************/
/* 	    PreControl Procedure                                                  */
/******************************************************************************/
           if ( PreControl( nIndiceCtx , sTlvBuffer ) != OK )
                return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                        sMyResource , sPrimResource , 'N' ));

/******************************************************************************/
/* 	    Security Verfication                                                  */
/******************************************************************************/
           if ( CheckSecurity  ( nIndiceCtx , sTlvBuffer ) != OK )
              return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                      sMyResource , sPrimResource , 'N' ));
                                      
          /* BEGIN: LHI 25-05-2008 EVENT CODE MUST BE SETTED IN NOK UNTIL ALL CONTROLS ARE OK */
					 	nRetCode = PinChange_Inquiry(nIndiceCtx,sTlvBuffer);
	   				sprintf(sRetCode,"%03d",nRetCode);
       			PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,sRetCode, 3); 
   					PrintTlvBuffer(sTlvBuffer);                            

          return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                     sMyResource , sPrimResource , 'N' ));
        }
        else
        {
			nRetCode = CheckResource ( nIndiceCtx, sSecResource , &nPid );
		    if ( nRetCode != OK )
   		    {
		       PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,
									ISSUER_NOT_AVAILABLE_EVENT,3);
        	    return (  AuthorLocal( nIndiceCtx, sTlvBuffer , sIsoMsg , 
                                       sMyResource , sPrimResource , 'N' ));
            }
  /* BEGIN: LHI 25-05-2008 EVENT CODE MUST BE SETTED IN NOK UNTIL ALL CONTROLS ARE OK */
   		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION ,     3);
   		PrintTlvBuffer(sTlvBuffer);     
		    return( AuthorRouting ( nIndiceCtx,sTlvBuffer,
									sSecResource,nPid,sIsoMsg ));
          }
   }
   else
   {
   trace_event ( "Primary Ressource Up ", PROCESSING ); 
   
    /* BEGIN: LHI 25-05-2008 EVENT CODE MUST BE SETTED IN NOK UNTIL ALL CONTROLS ARE OK */
   PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION ,     3);
   PrintTlvBuffer(sTlvBuffer);
   
   return( AuthorRouting(nIndiceCtx,sTlvBuffer,
									sPrimResource,nPid,sIsoMsg));
   }
   return ( OK );
}


int PinChange_Inquiry ( int nIndexCtx, char *sTlvBuffer  )
{

	TSTlvAutho      sTlvInfo;
	TSTlvPrivate    sTlvPrivateInfo;
    char    		sSecurityData       [ 128 + 1 ];
    char    		lNewPinOff 			[ 5 ];
    /* Mehdi Elyajizi PCI Pin Change */
    char    		lNewPinPvv 			[ 5 ];
    char    		lOldPinPvv 			[ 5 ]; 
    /* Mehdi Elyajizi PCI Pin Change */
    int     		nLength;
    int     		lRet = NOK;
    
    char cvv1_flag = 'Y';
    char cvv2_flag = 'Y';
    char pin_flag  = 'Y';
    char pin_method;
    char encryption_mode  ;
    char pin_method_flag = PVV_METHOD;
    char crpt_flag = 'Y';

    char cvv1_result = '4';
    char cvv2_result = '4';
    char pin_result  = '4';
    char crpt_result = '4';

    char   sTrack1Data 	[ 77 + 1];
    char   sTrack2Data 	[ 40 + 1];
    char   sPan 	    [ 22 + 1];
    char   sPinBlock    [ 16 + 1];
    char   sExpiryDate  [  4 + 1];
    char   sExpirySwapped  [  4 + 1];
    char   sServiceCode [  3 + 1];
    char   pvki;
    char   sPvv         [  5 + 1];
	char   sVisaOffset  [  5 + 1];/*DP_FCIB_20120215_1600_1_AER_PIN_OFFSET*/
    char   sCvv1        [  3 + 1];
    char   sCvv2        [  3 + 1];
    char   sSecurityFlags  [ 20 + 1 ];
    char   sSecurityResults[ 20 + 1 ];
    char   sHsmResultCode  [ 4 + 1 ];

    char   sCVKA  	  [ SIZE_KEY + 1];
    char   sCVKB  	  [ SIZE_KEY + 1];
    char   sPVKA  	  [ SIZE_KEY + 1];
    char   sPVKB  	  [ SIZE_KEY + 1];
    char   sTransKey  [ SIZE_KEY + 1];
    char   sPIN_KEY   [ SIZE_KEY + 1];
    char   sSecurityFormat  [ 2 + 1];
    char   sPinBlockFormat  [ 2 + 1];
    char   lPvki[1 + 1];/* MCK300408*/
    char   sLine [ MAX_LINE_TRC ];
    int result;
    char            tPvkiPVV     [  5 + 1];
    char            tOffset      [ 12 + 1];
	char			sReelPvv		[ 5 ];
    int             tPinLength; 
	

/************************************************************************/
/* INITIALISATION DES VARIABLES                                         */
/************************************************************************/
    memset ( sTrack1Data, 	0, sizeof ( sTrack1Data ));
    memset ( sTrack2Data, 	0, sizeof ( sTrack2Data ));
    memset ( sPan,  		0, sizeof ( sPan ));
    memset ( sPinBlock,     0, sizeof ( sPinBlock ));
    memset ( sSecurityData, 0, sizeof ( sSecurityData ));
    memset ( sExpiryDate,  	0, sizeof ( sExpiryDate));
    memset ( sExpirySwapped,  	0, sizeof ( sExpirySwapped));
    memset ( sServiceCode,  0, sizeof ( sServiceCode));
    memset ( sPvv,  		0, sizeof ( sPvv ));
    memset ( sCvv1,  		0, sizeof ( sCvv1));
    memset ( sCvv2,  		0, sizeof ( sCvv2));
    memset ( sSecurityFlags   , 0,  sizeof (sSecurityFlags));
    memset ( sSecurityResults , 0,  sizeof ( sSecurityResults));
    memset ( sHsmResultCode   , 0, sizeof ( sHsmResultCode ));
    memset ( sSecurityFormat  , 0, sizeof ( sSecurityFormat ));
    memset ( sPinBlockFormat  , 0, sizeof ( sPinBlockFormat ));

    memset ( sCVKA  , 0, sizeof ( sCVKA ));
    memset ( sCVKB  , 0, sizeof ( sCVKB ));
    memset ( sPVKA  , 0, sizeof ( sPVKA ));
    memset ( sPVKB  , 0, sizeof ( sPVKB ));
    memset ( sTransKey  , 0, sizeof ( sPVKB ));
    memset ( sPIN_KEY   , 0, sizeof ( sPVKB ));
    memset ( lPvki   , 0, sizeof ( lPvki ));/* MCK300408*/
	memset ( sReelPvv   , 0, sizeof ( sReelPvv ));
	
    /* Mehdi Elyajizi PCI Pin Change */
		memset ( lNewPinPvv,  		0, sizeof ( lNewPinPvv ));
		memset ( lOldPinPvv,  		0, sizeof ( lOldPinPvv ));
    /* Mehdi Elyajizi PCI Pin Change */
    
    InitTlvAutho	(&sTlvInfo);
    AnalyseTlvAutho (sTlvBuffer , &sTlvInfo);


/*****************************************************************************/
/*         extracting the SECURITY_VERIF_FLAG Tag'Value                      */
/*         If  Daes not Exist take the default values                        */
/*         means YES for all verif and PVV_METHOD for PIN_VERIF_METHOD       */
/*****************************************************************************/
    if ( GetTlvAutho (  AUTO_SECURITY_VERIF_FLAGS  , &sTlvInfo,
						sSecurityFlags   		   , &nLength ) != SUCCES )
    {
	   sprintf(sLine,"Security Flags Empty.");
       trace_event(sLine,PROCESSING);
       return ( SYSTEM_MALFUNCTION );
    }
    else
    {
	   sprintf(sLine,"SECURITY FLAGS RECEIVED");
       trace_event(sLine,PROCESSING);
	   sprintf(sLine,"________ _____ ________");
       trace_event(sLine,PROCESSING);
	   sprintf(sLine,"CVV1 FLAG : %c", sSecurityFlags[0] );
       trace_event(sLine,PROCESSING);
	   sprintf(sLine,"CVC2 FLAG : %c", sSecurityFlags[1] );
       trace_event(sLine,PROCESSING);
	   sprintf(sLine,"PIN  FLAG : %c", sSecurityFlags[2] );
       trace_event(sLine,PROCESSING);
	   sprintf(sLine,"PIN  METD : %c", sSecurityFlags[3] );
       trace_event(sLine,PROCESSING);
	   sprintf(sLine,"CRPT FLAG : %c", sSecurityFlags[4] );
       trace_event(sLine,PROCESSING);

       cvv1_flag 	= sSecurityFlags[0];
       cvv2_flag 	= sSecurityFlags[1];
       pin_flag  	= sSecurityFlags[2];
       pin_method  	= sSecurityFlags[3];
       crpt_flag  	= sSecurityFlags[4];
    }

    if ( GetTlvAutho (  AUTO_SECURITY_VER_RESULTS , &sTlvInfo,
						sSecurityResults , &nLength ) != SUCCES )
    {
	   sprintf(sLine,"Security Results Empty.");
       trace_event(sLine,PROCESSING);
       return ( SYSTEM_MALFUNCTION );
    }

/*****************************************************************************/
/* Read TRACK Data                                                           */
/*****************************************************************************/
    if(GetTlvAutho(AUTO_TRACK2_DATA,&sTlvInfo,sTrack2Data,&nLength)== SUCCES)
         GetTrack2Data(sTrack2Data,sPan,sExpiryDate,sServiceCode,sPvv,sCvv1);
    else
    if(GetTlvAutho(AUTO_TRACK1_DATA,&sTlvInfo,sTrack1Data,&nLength) == SUCCES)
         GetTrack1Data(sTrack1Data,sPan,sExpiryDate,sServiceCode,sPvv,sCvv1);
    else 
    {
		GetTlvAutho(AUTO_PAN,&sTlvInfo,sPan,&nLength);
		GetTlvAutho(AUTO_EXPIRY_DATE,&sTlvInfo,sExpiryDate,&nLength); 
		
		if (debug_option ==1)
		{
			fprintf ( stderr,"\n PPPPPAn [%s] ", sPan );			
			fprintf ( stderr,"\n EXXXXPd [%s] ", sExpiryDate);
		}

    }

/*---------------------------------------------------------------------------*/
/*        PIN CHANGE METHODE PVV                                             */
/*---------------------------------------------------------------------------*/
   /**Start MKB200309**/
   memset(lNewPinOff, 0, sizeof(lNewPinOff));
   /*result = get_pin_info ( nIndexCtx, sPan, tPvkiPVV, tOffset, &tPinLength);*/
   /*result = get_pin_info ( nIndexCtx, sPan, tPvkiPVV, sVisaOffset,tOffset, &tPinLength);MCK20150801*/
   result = get_pin_info ( nIndexCtx, sPan, tPvkiPVV, sVisaOffset,tOffset,sReelPvv, &tPinLength);
   if ( 0 == tPinLength)
      	tPinLength = 4;
   /**End MKB200309**/

   if (pin_method  	== PIN_METHODE_PVV )
   {
	/**Start MKB200309**/
	/*SKO PCI */
	sprintf(sLine,"pin_method : PIN_METHODE_PVV" );
    trace_event(sLine,PROCESSING);
	
	char sIccData [LG_MAX], sHsmResult[256];
   	char  sPinOffset[16+1];/*sPvv[16+1],AER 14022012 DP_FCIB_20120215_1600_1_AER_PIN_OFFSET*/ 

      	memset(sIccData, 0, sizeof(sIccData));
      	memset(sHsmResult, 0, sizeof(sHsmResult));
      	/*memset(sPvv, 0, sizeof(sPvv));AER 14022012 DP_FCIB_20120215_1600_1_AER_PIN_OFFSET*/ 
      	memset(sPinOffset, 0, sizeof(sPinOffset));
	/***End MKB200309**/

   	memcpy(lOldPinPvv, sPvv + 1, 4);

	lRet = ComputePvv(nIndexCtx,&sTlvInfo ,lNewPinOff,sPvv[0]); /**MCK280408**/

	if (lRet == OK ){
			/*SKO PCI */
			sprintf(sLine,"After ComputePvv " );
            trace_event(sLine,PROCESSING);
			
         	lPvki[0] = sPvv[0];
      	 	lRet = PutPvv(nIndexCtx,sPan, lOldPinPvv, lNewPinOff,lPvki);

		/**Start MKB200309**/
		/**if	(  OK == GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sIccData, &nLength ) 
			 && OK == (lRet = ComputePinOffset(nIndexCtx,sTlvBuffer,lNewPinOff))  SMA20150624**/
		     if	( OK == GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sIccData, &nLength ) 
			   && OK == (lRet = ComputePinOffset(nIndexCtx,&sTlvInfo,lNewPinOff,lNewPinOff[0],4)))
              		/** PIN Change and PIN Re-Printing **/
            	{
               		if ( OK == (lRet))/* = UpdatePinChangeInfo(nIndexCtx,sPan, pvki, sPvv, sPinOffset)) )*/
                  		lRet = gen_iss_script_data(nIndexCtx, sTlvBuffer, sHsmResult) ;
            	}
		/***End MKB200309**/
	}
   }
/*---------------------------------------------------------------------------*/
/*        PIN CHANGE METHODE OFFSET                                          */
/*---------------------------------------------------------------------------*/
   if (pin_method  	== PIN_METHODE_OFFSET )
   {
	sprintf(sLine,"pin_method : PIN_METHODE_OFFSET" );
    trace_event(sLine,PROCESSING);
 		/*lRet = ComputePinOffset(nIndexCtx,sTlvBuffer,lNewPinOff);AER 15022012 Wrong parameters passed DP_FCIB_20120215_1600_1_AER_PIN_OFFSET*/
		lRet = ComputePinOffset(nIndexCtx,&sTlvInfo,lNewPinOff,sPvv[0],4);
 	
    		if (lRet == OK )
    			lRet = PutPinOffset(nIndexCtx,sPan,lNewPinOff);
   }
			/*SKO PCI */
			sprintf(sLine,"Retour Pin_change : [%d] ", lRet);
            trace_event(sLine,PROCESSING);
  return ( lRet );
}

