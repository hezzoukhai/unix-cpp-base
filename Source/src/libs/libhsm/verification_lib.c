#include <stdio.h>
#include <hsm_inc.h>
#include <resources.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>

/*---------------------------------------------------------------------------------------------------------------------------------
 Id                 Date                Author                          Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK-HF         25-08-2003          Mah.Karroum & Hafida Fouta        Medification de la fonction check_pin_data 
---------------------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************/
/* Fonction check_cvv_data.                                                   */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le cvv recu.               */
/******************************************************************************/

int     check_cvv_data  (

                            int nIndiceCtx,

                            char *Pan, 

							char *CvkA, 

							char *CvkB, 

							char *expiry_date, 

							char *service_code, 

							char *cvv,

							char *sHsmResultCode

						)

{

   unsigned char  BuffSnd   [256];

   unsigned char  BuffRcv   [256];

   int      retour;

   pid_t    nPid;

   char sLine[MAX_LINE_TRC];



   trace_event("Start check_cvv_data()",PROCESSING);



   memset(BuffSnd, 0, sizeof( BuffSnd ) );

   memset(BuffRcv, 0, sizeof( BuffRcv ) );



   sprintf(sLine, "PAN                         : %s", Pan);

   trace_event(sLine, PROCESSING);

   sprintf(sLine, "SERVICE CODE                : %.3s", service_code);

   trace_event(sLine, PROCESSING);

   sprintf(sLine, "EXPIRY DATE                 : %.4s", expiry_date);

   trace_event(sLine, PROCESSING);

   sprintf(sLine, "CVKA                        : %.16s", CvkA);

   trace_event(sLine, PROCESSING);

   sprintf(sLine, "CVKB                        : %.16s", CvkB);

   trace_event(sLine, PROCESSING);

   sprintf(sLine, "CVV                         : %.3s", cvv);

   trace_event(sLine, PROCESSING);

   trace_event("- CVV Verification ---------------", PROCESSING);



   command_CY(BuffSnd, Pan, service_code, expiry_date, CvkA, CvkB, cvv);

 

   retour = CheckResource ( nIndiceCtx, HSM_VERIF , &nPid );

   if ( ( retour != OK ) )

   {

      memcpy ( sHsmResultCode , "UNVL", 4 );

      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);

      trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);

      return(HSM_UNAVAILABLE);

   }



   retour = HsmQuery( nIndiceCtx, nPid , BuffSnd, BuffRcv );

   if ( retour == NOT_AVAILABLE )

   {

      memcpy ( sHsmResultCode , "UNVL", 4 );

      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);

      trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);

      return(HSM_UNAVAILABLE);

   }



   if (retour == HSM_MALFUNCTION)

   {

      memcpy ( sHsmResultCode , "UNVL", 4 );

      trace_event("HSM MALFUNCTION DETECTED",ERROR);

      trace_event("End   check_cvv_data(MALFUNCTION)",PROCESSING);

      return(HSM_MALFUNCTION);

   }



   retour = command_CZ(BuffRcv,sHsmResultCode);

   if ( retour != OK )

   {

      switch ( retour )

      {

         case VERIFICATION_FAILED :

      		trace_event("End   check_cvv_data(VERIFICATION_FAILED)",PROCESSING);

      		return(VERIFICATION_FAILED);



         case ERROR_COMMAND :

      		trace_event("End check_cvv_data(ERROR_COMMAND)",PROCESSING);

      		return(HSM_MALFUNCTION);



         default :

      		trace_event("End   check_cvv_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);

      		return(CRYPTOGRAPHIC_ERROR);

      }

   }



   trace_event("End   check_cvv_data(OK)",PROCESSING);

   return(OK);

}



/******************************************************************************/

/* Fonction check_pvv_data.                                                   */

/* -----------------------                                                    */

/* Description : Fonction qui verifie le pvv recu.               */

/******************************************************************************/



int check_pvv_data ( int nIndiceCtx,

					char *Pan, 

					char *PvkA, 

					char *PvkB,

					char *PinBlock, 

					char *format, 

					char *trans_key, 

					char *pvv, 

					char mode,

					char *sHsmResultCode )

{

   int         retour;

   pid_t    nPid;



   unsigned char   BuffSnd  [256];

   unsigned char   BuffRcv  [256];



   char sLine[MAX_LINE_TRC];



   memset(BuffSnd, 0, sizeof( BuffSnd ) );

   memset(BuffRcv, 0, sizeof( BuffRcv ) );



   trace_event("Start check_pvv_data()",PROCESSING);


   /*
   sprintf(sLine, "PAN                         : %.12s", Pan + (strlen(Pan) - 13));

   trace_event(sLine, PROCESSING);

   sprintf(sLine, "PVKA                        : %.16s", PvkA);

   trace_event(sLine, PROCESSING);

   sprintf(sLine, "PVKB                        : %.16s", PvkB);

   trace_event(sLine, PROCESSING);

   sprintf(sLine, "PVV                         : %.5s", pvv);

   trace_event(sLine, PROCESSING);

   sprintf(sLine, "PIN BLOCK                   : %.16s", PinBlock);

   trace_event(sLine, PROCESSING);

   sprintf(sLine, "Format                      : %.2s", format);

   trace_event(sLine, PROCESSING);



   if ( mode == TERMINAL_MODE )

      sprintf(sLine, "TAK/TPK                     : %.16s", trans_key );

   else

      sprintf(sLine, "ZPK                         : %.16s", trans_key );

   trace_event(sLine, PROCESSING);
   */ /** HM - 09022010 - PA_DSS **/


   trace_event("- Start PVV Verification ---------------", PROCESSING);



   if ( mode == TERMINAL_MODE )

      command_DC(BuffSnd,trans_key,PvkA,PvkB,PinBlock,format,Pan,pvv);

   else

      command_EC(BuffSnd,trans_key,PvkA,PvkB,PinBlock,format,Pan,pvv);



   retour = CheckResource ( nIndiceCtx, HSM_VERIF , &nPid );

   if ( ( retour != OK ) )

   {

      memcpy ( sHsmResultCode , "UNVL", 4 );

      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);

      trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);

      return(HSM_UNAVAILABLE);

   }

   retour = HsmQuery(nIndiceCtx, nPid,BuffSnd, BuffRcv);

   if (retour == NOT_AVAILABLE)

   {

      memcpy ( sHsmResultCode , "UNVL", 4 );

      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);

      trace_event("End   check_pvv_data(HSM_UNAVAILABLE)",PROCESSING);

      return(HSM_UNAVAILABLE);

   }

   if (retour == HSM_MALFUNCTION)

   {

      memcpy ( sHsmResultCode , "UNVL", 4 );

      trace_event("HSM MALFUNCTION DETECTED",ERROR);

      trace_event("End   check_pvv_data(HSM_UNAVAILABLE)",PROCESSING);

      return(HSM_UNAVAILABLE);

   }



   if ( mode == TERMINAL_MODE )

      retour = command_DD(BuffRcv,sHsmResultCode);

   else

      retour = command_ED(BuffRcv,sHsmResultCode);



   if ( retour != OK )

   {

      switch ( retour )

      {

         case VERIFICATION_FAILED :

      		trace_event("End   check_pvv_data(VERIFICATION_FAILED)",PROCESSING);

      		return(VERIFICATION_FAILED);



         case ERROR_COMMAND :

      		trace_event("End   check_pvv_data(ERROR_COMMAND)",PROCESSING);

      		return(HSM_MALFUNCTION);



         default :

      		trace_event("End   check_pvv_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);

      		return(CRYPTOGRAPHIC_ERROR);

      }

   }



   trace_event("End   check_pvv_data(OK)",PROCESSING);

   return(OK);

}



/******************************************************************************/
/* Fonction check_pin_data.                                                   */
/* -----------------------   -                                                */
/* Description : Fonction qui verifie le pin recu.               */
/* M.A.J	   : M.K-H.F correction de l'appel de HsmQuery		 */
/******************************************************************************/

int    check_pin_data(int 	nIndiceCtx, 
					  char	*Pan, 
					  char	*pin_tpk, 	
					  char	*pin_block_format, 
					  char	*pin_key, 
					  char	*tpk_key , 
					  char	*sHsmResultCode )
{
   pid_t   nPid;
   unsigned char   error_code     [ 3];
   unsigned char   BuffSnd        [128];
   unsigned char   BuffRcv        [128];
   unsigned char   pin_block_clair_calcul [16];
   unsigned char   pin_clair_recu      [16];
   unsigned char   pin_tpk_expand  [17];

   char     sLine[MAX_LINE_TRC];
   int      retour;

   sprintf(sLine, "Start   check_pin_data(%d, %.19s, %.16s, %.2s, %.16s, %.16s)",
                  nIndiceCtx, Pan, pin_tpk, pin_block_format, pin_key, tpk_key);
   trace_event(sLine, PROCESSING);

   memset(BuffSnd               , 0, sizeof(BuffSnd) );
   memset(BuffRcv               , 0, sizeof(BuffRcv) );
   memset(pin_tpk_expand    	, 0, sizeof(pin_tpk_expand) );
   memset(error_code            , 0, sizeof(error_code) );
   memset(pin_clair_recu        , 0, sizeof(pin_clair_recu) );
   memset(pin_block_clair_calcul, 0, sizeof(pin_block_clair_calcul) );

   memcpy(pin_tpk_expand, pin_tpk, 16);

   command_HG(BuffSnd, tpk_key, pin_tpk_expand);

   retour = CheckResource ( nIndiceCtx, HSM_VERIF , &nPid );
   if (  retour != OK  )
   {
      memcpy ( sHsmResultCode , "UNVL", 4 );
      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
      trace_event("End   check_pin_data(HSM_UNAVAILABLE)",PROCESSING);
      return(HSM_UNAVAILABLE);
   }

   retour = HsmQuery(nIndiceCtx,nPid,BuffSnd, BuffRcv);
   if(retour == NOT_AVAILABLE)
   {
      memcpy ( sHsmResultCode , "UNVL", 4 );
      trace_event("HSM NOT AVAILABLE.",PROCESSING);
      trace_event("End   check_pin_data(HSM_UNAVAILABLE)",PROCESSING);
      return(HSM_UNAVAILABLE);
   }

   if(retour == HSM_MALFUNCTION)
   {
      memcpy ( sHsmResultCode , "UNVL", 4 );
      trace_event("MALFUNCTION ERROR", ERROR);
      trace_event("End   check_pin_data(HSM_UNAVAILABLE)",PROCESSING);
      return(HSM_UNAVAILABLE);
   }

   retour = command_HH(BuffRcv, pin_clair_recu , sHsmResultCode );
   if ( retour != OK )
   {
      switch ( retour )
      {
         case ERROR_COMMAND :
                trace_event("End   check_pin_data(ERROR_COMMAND)",PROCESSING);
                return(HSM_MALFUNCTION);

         default :
                trace_event("End   check_pin_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
                return(CRYPTOGRAPHIC_ERROR);
      }
   }


   /** calc_pin_block(nIndiceCtx, Pan, pin_key, pin_block_format, pin_block_clair_calcul); **/
   calc_pin_block(nIndiceCtx, Pan, pin_key, pin_block_clair_calcul); /** MAW20030922 **/

   if (memcmp(pin_clair_recu, pin_block_clair_calcul, 16) == 0)
   {
      trace_event("End   check_pin_data(OK)",PROCESSING);
      return(OK);
   }
   else
   {
      trace_event("End   check_pin_data(NOK)",PROCESSING);
      return(VERIFICATION_FAILED);
   }

   trace_event("End   check_pin_data(OK)",PROCESSING);
   return(OK);
}

/******************************************************************************/
/* Fonction check_offset_data.                                                */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le offset.                              */
/******************************************************************************/

int check_offset_data ( int nIndiceCtx,
					    char *Pan, char *PvkA,
					    char *PinBlock, char *format, char *trans_key, 
					    char *offset , char mode,
					    char *sHsmResultCode , int PinLength)
{
   unsigned char   BuffSnd  [128];
   unsigned char   BuffRcv  [128];
   pid_t    nPid;
   int         retour;
   char sLine[MAX_LINE_TRC];

   trace_event("Start check_offset_data()",PROCESSING);

   memset(BuffSnd, 0, sizeof( BuffSnd ) );
   memset(BuffRcv, 0, sizeof( BuffRcv ) );

   sprintf(sLine, "PAN                         : %.12s", Pan + (strlen(Pan) - 13));
   trace_event(sLine, PROCESSING);
   sprintf(sLine, "PVKA                        : %.16s", PvkA);
   trace_event(sLine, PROCESSING);
   sprintf(sLine, "OFFSET                      : %.12s", offset);
   trace_event(sLine, PROCESSING);
   sprintf(sLine, "PIN BLOCK                   : %.16s", PinBlock);
   trace_event(sLine, PROCESSING);
   sprintf(sLine, "Format                      : %.2s", format);
   trace_event(sLine, PROCESSING);
   sprintf(sLine, "PIN Length                  : %02d", PinLength);
   trace_event(sLine, PROCESSING);

   if ( mode == TERMINAL_MODE )
      sprintf(sLine, "TAK/TPK                     : %.16s", trans_key );
   else
      sprintf(sLine, "ZPK                         : %.16s", trans_key );
   trace_event(sLine, PROCESSING);

   if ( mode == TERMINAL_MODE )
      command_DA(BuffSnd, trans_key, PvkA, PinBlock, format, Pan, offset, PinLength);
   else
      command_EA(BuffSnd, trans_key, PvkA, PinBlock, format, Pan, offset, PinLength);

   retour = CheckResource ( nIndiceCtx, HSM_VERIF , &nPid );
   if ( ( retour != OK ) )
   {
      memcpy ( sHsmResultCode , "UNVL", 4 );
      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
      trace_event("End   check_offset_data(HSM_UNAVAILABLE)",PROCESSING);
      return(HSM_UNAVAILABLE);
   }
   retour = HsmQuery(nIndiceCtx,nPid,BuffSnd, BuffRcv);
   if (retour == NOT_AVAILABLE)
   {
      memcpy ( sHsmResultCode , "UNVL", 4 );
      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
      trace_event("End   check_offset_data(HSM_UNAVAILABLE)",PROCESSING);
      return(HSM_UNAVAILABLE);
   }
   if (retour == HSM_MALFUNCTION)
   {
      memcpy ( sHsmResultCode , "UNVL", 4 );
      trace_event("HSM MALFUNCTION DETECTED",ERROR);
      trace_event("End   check_offset_data(HSM_UNAVAILABLE)",PROCESSING);
      return(HSM_UNAVAILABLE);
   }

   if ( mode == TERMINAL_MODE )
      retour = command_DB(BuffRcv,sHsmResultCode);
   else
      retour = command_EB(BuffRcv,sHsmResultCode);

   if ( retour != OK )
   {
      switch ( retour )
      {
         case VERIFICATION_FAILED :
      		trace_event("End   check_offset_data(VERIFICATION_FAILED)",PROCESSING);
      		return(VERIFICATION_FAILED);

         case ERROR_COMMAND :
      		trace_event("End   check_offset_data(ERROR_COMMAND)",PROCESSING);
      		return(HSM_MALFUNCTION);

         default :
      		trace_event("End   check_offset_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
      		return(CRYPTOGRAPHIC_ERROR);
      }
   }

   trace_event("End   check_offset_data(OK)",PROCESSING);
   return(OK);
}

/** START MAW20050914 **/
/******************************************************************************/
/* Fonction check_crpt_data.                                                  */
/* -----------------------                                                    */
/******************************************************************************/

int     check_crpt_data  (
                            int        nIndexCtx,
                            char      *sTlvBuffer,
                            char      *sHsmResultCode
                        )

{

   unsigned char  BuffSnd   [LG_MAX];
   unsigned char  BuffRcv   [LG_MAX];
   unsigned char  sPan          [22+ 1];  /* Preformated Pan/Sequence Nbr */
   unsigned char  sSeqNumber    [8 + 1];  /* Preformated Pan/Sequence Nbr */

   unsigned char  sFormatedPan  [8 + 1]; /* Preformated Pan/Sequence Nbr */
   unsigned char  sAtc  	[2 + 1]; /* Transaction counter          */
   unsigned char  sUn   	[4 + 1]; /* Unpredictible number */
   unsigned char  sData         [LG_MAX]; /* Data */
   unsigned char  sCrptAmount   [6 + 1];  /* Cryptogram Amount   */
   unsigned char  sOthrAmount   [6 + 1];  /* Other Amount ou Cash back amount */
   unsigned char  sCountryCode  [2 + 1];  /* Country Code                     */
   unsigned char  sTvr          [5 + 1];  /* Terminal Verification Results    */
   unsigned char  sCrncyCode    [2 + 1];  /* Currency Code                    */
   unsigned char  sTrxDate      [3 + 1];  /* Transaction Date                 */
   unsigned char  sTrxType      [1 + 1];   /* Transaction Type                */
   unsigned char  sAip          [2 + 1];   /* Application Interchange Profile */
   unsigned char  sCvr          [4 + 1];  /* Card Verification results        */
   unsigned char  sArqc         [8 + 1];   /* ARQC                             */
   unsigned char  sArpc         [8 + 1];   /* ARPC                             */
   unsigned char  sAC_Key       [256  ];   /* Master key For authentification Crtyptogram   */
   unsigned char  sArc          [256  ];   
   unsigned char  cSchemeId            ;

   TSTlvPrivate tlvInfo;

   char           szWork[LG_MAX];
   char           sIccData[LG_MAX];
   char           sTmpField[LG_MAX];
   char           sTmpData[LG_MAX];
   char           sLine[MAX_LINE_TRC];
   TSTagIso       IccInfo;

   int      nRetCode = 0;
   int      nLenWork = 0;
   int      nLenData = 0;
   int      nLength = 0;
   int      nTmpLen = 0;
   int      nIccLen = 0;
   pid_t    nPid;



   trace_event("Start check_crpt_data()",PROCESSING);

   memset(BuffSnd, 0, sizeof( BuffSnd ) );
   memset(BuffRcv, 0, sizeof( BuffRcv ) );

   memset(  sFormatedPan  , 0, sizeof(sFormatedPan));
   memset(  sAtc  	, 0, sizeof(sAtc));
   memset(  sUn   	, 0, sizeof(sUn));
   memset(  sData        , 0, sizeof(sData));
   memset(  sCrptAmount  , 0, sizeof(sCrptAmount));
   memset(  sOthrAmount  , 0, sizeof(sOthrAmount));
   memset(  sCountryCode , 0, sizeof(sCountryCode));
   memset(  sTvr         , 0, sizeof(sTvr));
   memset(  sCrncyCode   , 0, sizeof(sCrncyCode));
   memset(  sTrxDate     , 0, sizeof(sTrxDate));
   memset(  sTrxType     , 0, sizeof(sTrxType));
   memset(  sAip         , 0, sizeof(sAip));
   memset(  sCvr         , 0, sizeof(sCvr));
   memset(  sArqc        , 0, sizeof(sArqc));
   memset(  sArpc        , 0, sizeof(sArpc));
   memset(  sAC_Key      , 0, sizeof(sAC_Key));
   memset(  szWork       , 0, sizeof(szWork));
   memset(  sTmpField    , 0, sizeof(sTmpField));
   memset(  sTmpField    , 0, sizeof(sTmpField));
   memset(  sTmpData      , 0, sizeof(sTmpData));
   memset(  sArc          , 0, sizeof(sArc));
   memset(  sArpc         , 0, sizeof(sArpc));

/** Retrieveing the Card Number **/
   if ( GetTlvBuffer ( AUTO_PAN , sTlvBuffer , sPan ,&nLenData) != SUCCESS )
   {
      memcpy ( sHsmResultCode , "-PAN", 4 );
      trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
      trace_event("END   check_crpt_data(NOK)",PROCESSING);
      return(HSM_MALFUNCTION);
   }

/** Retrieveing the Card Seq Number **/
   memset(sSeqNumber, 0, sizeof(sSeqNumber));
   if (GetTlvBuffer (AUTO_CARD_SEQUENCE_NUMBER , sTlvBuffer, sSeqNumber, &nLenData) != SUCCES)
   {
	memcpy(sSeqNumber, "000", 3);
 trace_event("END   check_crpt_data(NOKremplacer par 000 pour test:)",PROCESSING);
/*
      memcpy ( sHsmResultCode , "-PAN", 4 );
      trace_event("ERROR WhiLe GetIsoIcTag(ISO_TAG_TRANS_SEQ_NUM) ", ERROR);
      trace_event("END   check_crpt_data(NOK)",PROCESSING);
      return(HSM_MALFUNCTION);	*/
   }



   sprintf(sLine, "PAN                         : %s", sPan);
   trace_event(sLine, PROCESSING);

   sprintf(sLine, "SEQUENCE NUMBER             : %s", sSeqNumber);
   trace_event(sLine, PROCESSING);


   trace_event("- CRYPTOGRAM Verification ---------------", PROCESSING);

/* Getting the AC key **/

   nRetCode = Get_AC_Key(nIndexCtx,sPan,sAC_Key);

    if(nRetCode != OK)
    {
        memcpy ( sHsmResultCode , "-DMK", 4 );
        trace_event("INVALID KEY DMK ", ERROR);
        trace_event("END   check_crpt_data(KEY_UNAVAILABLE)",PROCESSING);
        return(HSM_MALFUNCTION);
    }
   sprintf(sLine, "AC KEY IS                  : %s", sAC_Key);
   trace_event(sLine, PROCESSING);


/* PreFormated Pan */

   memset(szWork, '0', 16);
   memcpy(sTmpField, sPan, strlen(sPan));
   memcpy(sTmpField + strlen(sPan), sSeqNumber+1, 2);
   nLenWork = strlen(sTmpField);
   /* if (strlen(nLenWork) < 16 ) EBE140523: It was meant nLenWork*/
   if( nLenWork < 16 )
   	memcpy(szWork + (16 - nLenWork), sTmpField , nLenWork);
    else
   	memcpy(szWork, sTmpField + (nLenWork - 16), 16);

   AsciiToHex (szWork, sFormatedPan, 16);
   sprintf(sLine, "PRE FORMATED PAN            : %s", szWork);
   trace_event(sLine, PROCESSING);

/** Getting the ICC Data from TLV **/
   if ( GetTlvBuffer ( AUTO_ICC_DATA , sTlvBuffer , sIccData ,&nIccLen) != SUCCESS )
   {
      memcpy ( sHsmResultCode , "-PAN", 4 );
      trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
      trace_event("END   check_crpt_data(NOK)",PROCESSING);
      return(HSM_MALFUNCTION);
   }
	InitTlvPrivate (&tlvInfo);
	AnalyseTlvPrivate(sIccData , nIccLen,&tlvInfo);

/* cSchemeId */
   cSchemeId = '0'; /** Visa **/
   if ( !GetTlvBuffer ( AUTO_NETWORK_CODE , sTlvBuffer , szWork ,&nLenWork) && memcmp(szWork, "01", 2) )
      cSchemeId = '1'; /** MC **/

/* ATC */
	memset(szWork, 0, sizeof(szWork));
        sAtc[0] = 0x30;
        sAtc[1] = 0x30;
	if (GetTlvPrivate (CHIP_ATC_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
   		AsciiToHex (szWork, sAtc, 4);

   	sprintf(sLine, "ATC                         : %s", szWork);
   	trace_event(sLine, PROCESSING);

/* UN */

	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_UNPRED_NUMBER_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
   		AsciiToHex (szWork, sUn, 8);

   	sprintf(sLine, "UN                          : %s", szWork);
   	trace_event(sLine, PROCESSING);

/* Data */
      nLenData = 0;
/* Crpt Amount */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_AMOUNT_TAG , &tlvInfo, szWork, &nLenWork) != SUCCESS)
           sprintf(szWork,"%012d", 0);

        AsciiToHex (szWork, sCrptAmount, 12);
         

   	sprintf(sLine, "CRPT AMOUNT                 : %s", szWork);
   	trace_event(sLine, PROCESSING);
        memcpy(sData + nLenData, sCrptAmount, 6); nLenData += 6;


/* Other Amount */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_OTHER_AMOUNT_TAG , &tlvInfo, szWork, &nLenWork) != SUCCESS)
           sprintf(szWork,"%012d", 0);

        AsciiToHex (szWork, sOthrAmount, 12);

        sprintf(sLine, "OTHER AMOUNT                 : %s", szWork);
        trace_event(sLine, PROCESSING);
        memcpy(sData + nLenData, sOthrAmount, 6); nLenData += 6; 

/* Country Code */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TERM_COUNTRY_CODE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   		AsciiToHex (szWork, sCountryCode, 4);
        else
        {
           memcpy ( sHsmResultCode , "---1", 4 );
           trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TERM_COUNTRY_CODE_TAG>> NOT RECEIVED <==", ERROR);
           trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
           return(CRYPTOGRAPHIC_ERROR);
        }

   	sprintf(sLine, "COUNTRY CODE                 : %s", szWork);
   	trace_event(sLine, PROCESSING);
        memcpy(sData + nLenData, sCountryCode, 2); nLenData += 2;


/* TVR          */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TVR_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   		AsciiToHex (szWork, sTvr, 10);
        else
        {
           memcpy ( sHsmResultCode , "---2", 4 );
           trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TVR_TAG>> NOT RECEIVED <==", ERROR);
           trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
           return(CRYPTOGRAPHIC_ERROR);
        }

   	sprintf(sLine, "TVR                          : %s", szWork);
   	trace_event(sLine, PROCESSING);
        memcpy(sData + nLenData, sTvr, 5); nLenData += 5;

/* CURRENCY CODE          */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_CUR_CODE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   		AsciiToHex (szWork, sCrncyCode, 4);
        else
        {
           memcpy ( sHsmResultCode , "---3", 4 );
           trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_CUR_CODE_TAG>> NOT RECEIVED <==", ERROR);
           trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
           return(CRYPTOGRAPHIC_ERROR);
        }

   	sprintf(sLine, "CURRENCY CODE                : %s", szWork);
   	trace_event(sLine, PROCESSING);

        memcpy(sData + nLenData, sCrncyCode, 2); nLenData += 2;

/* TRX DATE         */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_DATE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   		AsciiToHex (szWork, sTrxDate, 6);
        else
        {
           memcpy ( sHsmResultCode , "---4", 4 );
           trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_DATE_TAG>> NOT RECEIVED <==", ERROR);
           trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
           return(CRYPTOGRAPHIC_ERROR);
        }

   	sprintf(sLine, "TRANSACTION DATE             : %s", szWork);
   	trace_event(sLine, PROCESSING);

        memcpy(sData + nLenData, sTrxDate, 3); nLenData += 3; 

/* TRX TYPE         */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_TYPE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   		AsciiToHex (szWork, sTrxType, 2);
        else
        {
           memcpy ( sHsmResultCode , "---3", 4 );
           trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_TYPE_TAG>> NOT RECEIVED <==", ERROR);
           trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
           return(CRYPTOGRAPHIC_ERROR);
        }

   	sprintf(sLine, "TRANSACTION TYPE             : %s", szWork);
   	trace_event(sLine, PROCESSING);

        memcpy(sData + nLenData, sTrxType, 1); nLenData += 1;

/* UN */
        memcpy(sData + nLenData, sUn, 4); nLenData += 4; 

/* AIP              */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_AIP_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   		AsciiToHex (szWork, sAip, 4);
        else
        {
           memcpy ( sHsmResultCode , "---4", 4 );
           trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_AIP_TAG>> NOT RECEIVED <==", ERROR);
           trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
           return(CRYPTOGRAPHIC_ERROR);
        }

   	sprintf(sLine, "AIP                          : %s", szWork);
   	trace_event(sLine, PROCESSING);

        memcpy(sData + nLenData, sAip, 2); nLenData += 2; 

/* ATC */
        memcpy(sData + nLenData, sAtc, 2); nLenData += 2;

/* CVR              */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_ISS_APPLI_DATA_TAG , &tlvInfo, szWork, &nLenWork) != SUCCESS)
           sprintf(szWork, "%32d", 0);


        if ( '1' == cSchemeId ) /** MCI **/
        {
   	   sprintf(sLine, "CVR                          : %.8s", szWork+4);
           AsciiToHex (szWork+4, sCvr, 8); /** MC M-Chip Lite **/
           memcpy(sData + nLenData, sCvr, 4); nLenData += 4;
        }
        else /** VISA **/
        {
   	   sprintf(sLine, "CVR                          : %.8s", szWork+6);
           AsciiToHex (szWork+6, sCvr, 8); /** VSDC **/
           memcpy(sData + nLenData, sCvr, 4); nLenData += 4;
        }

   	trace_event(sLine, PROCESSING);



/* ARQC */

	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_APP_CRYPTOGRAM_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   		AsciiToHex (szWork, sArqc, 16);

   	sprintf(sLine, "ARQC                         : %s", szWork);
   	trace_event(sLine, PROCESSING);

   memcpy(sArc,"00",2);

   if ( '1' == cSchemeId ) /** MCI **/
   {
      sData[nLenData]=0x80; nLenData += 1; /** M/Chip 2.1 **/
   }

   nLength = command_KQ(BuffSnd, cSchemeId, sAC_Key, sFormatedPan, sAtc, sUn, nLenData, sData, sArqc, sArc); 
   /** nLength = command_KW(BuffSnd, cSchemeId, sAC_Key, sFormatedPan, sAtc, sUn, nLenData, sData, sArqc, sArc); **/

 

   nRetCode = CheckResource ( nIndexCtx, HSM_VERIF , &nPid );

   if ( ( nRetCode != OK ) )

   {

      memcpy ( sHsmResultCode , "UNVL", 4 );
      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
      trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
      return(HSM_UNAVAILABLE);

   }



   nRetCode = HsmQueryBin( nIndexCtx, nPid , BuffSnd, nLength, BuffRcv );

   if ( nRetCode == NOT_AVAILABLE )

   {
      memcpy ( sHsmResultCode , "UNVL", 4 );
      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
      trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
      return(HSM_UNAVAILABLE);

   }



   if (nRetCode == HSM_MALFUNCTION)

   {
      memcpy ( sHsmResultCode , "UNVL", 4 );
      trace_event("HSM MALFUNCTION DETECTED",ERROR);
      trace_event("End   check_cvv_data(MALFUNCTION)",PROCESSING);
      return(HSM_MALFUNCTION);
   }


    nRetCode = command_KR(BuffRcv,sHsmResultCode,sArpc); 
    /** nRetCode = command_KX(BuffRcv,sHsmResultCode,sArpc); **/

   if ( nRetCode != OK )

   {
      switch ( nRetCode )

      {
         case VERIFICATION_FAILED :

      		trace_event("End   check_crpt_data(VERIFICATION_FAILED)",PROCESSING);
      		return(VERIFICATION_FAILED);
         case ERROR_COMMAND :
      		trace_event("End check_cvv_data(ERROR_COMMAND)",PROCESSING);
      		return(HSM_MALFUNCTION);
         default :
      		trace_event("End   check_cvv_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
      		return(CRYPTOGRAPHIC_ERROR);

      }

   }

/* ARPC and ARC */
        memset(szWork, 0, sizeof(szWork));	
        memset(sTmpField, 0, sizeof(sTmpField));	
   	HexToAscii (sArpc, szWork, 16);
        /** memcpy(szWork + 16, sArc, 2); **/
        sprintf(szWork + 16, "%02X%02X", sArc[0], sArc[1]);

        sprintf(sLine, "==> Issuer Authen Data       : [%s]", szWork);
   	trace_event(sLine, PROCESSING);

	/** PutTlvPrivate (CHIP_ISS_ARPC_RESP_CODE_TAG, &tlvInfo, szWork, 20); **/
	PutTlvPrivate (CHIP_ISS_AUTHEN_DATA_TAG, &tlvInfo, szWork, 20);

	nIccLen = PrivateBuildTlv (sIccData, &tlvInfo);

        PutTlvBuffer ( AUTO_ICC_DATA, sTlvBuffer, sIccData, nIccLen);


   trace_event("End   check_crpt_data(OK)",PROCESSING);

   return(OK);
}
