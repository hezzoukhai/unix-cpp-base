/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      translation_lib.c                                                      */
/*                                                                             */
/* Description                                                                 */
/*      Transalation Tools Management                                          */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <hsm_inc.h>
#include <string.h>
#include <resources.h>

/*****************************************************************************/
/* Fonction key_exchange                                                     */
/* ---------------------                                                     */
/* Description : Cette fonction effectue le transchiffrement de la cle de    */
/*               transport du Pin de la ZMK vers la LMK.                     */
/*****************************************************************************/
int  key_exchange_scheme(nIndexCtx,Zmk_Key, Zpk_Zmk, Check_Value_In , Zpk_Lmk, Check_Value_Out)
int  nIndexCtx;
char *Zmk_Key;
char *Zpk_Zmk;
char *Check_Value_In ;
char *Zpk_Lmk;
char *Check_Value_Out ;
{
  pid_t nPid;
  int	retour;
  char  BuffSnd [128];
  char  BuffRcv [128];
  char ligne[MAX_LINE_TRC];
  char sHsmResultCode[5];

  memset(sHsmResultCode , 0 , 5 );
  memset(BuffSnd, 0, sizeof(BuffSnd) );
  memset(BuffRcv, 0, sizeof(BuffRcv) );
  memset(Check_Value_Out   , 0,             16);

  /*sprintf(ligne, "Start key_exchange_scheme(%.32s, %.32s, %.16s)", Zmk_Key,Zpk_Zmk,Check_Value_In );
  trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
  trace_event("Start key_exchange_scheme()", PROCESSING);

  command_FA(BuffSnd, Zmk_Key, Zpk_Zmk);

  if ( CheckResource ( nIndexCtx, HSM_VERIF , &nPid ) != OK )
  {
         trace_event ( "Resource HSM VERIF Unavailable", ERROR );
         return ( HSM_UNAVAILABLE );
  }


  /*sprintf(ligne, " translation_lib.c -->  BuffSnd[%.100s]", BuffSnd );
  trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
 

   retour = HsmQuery(nIndexCtx,nPid,BuffSnd, BuffRcv); 
  if (retour == NOT_AVAILABLE)
  {
    trace_event("HSM NOT AVAILABLE", ERROR);
    trace_event("End   key_exchange_scheme(NOT_AVAILABLE)",PROCESSING);
    return(HSM_UNAVAILABLE);
  }

  if (retour == HSM_MALFUNCTION)
  {
        trace_event("HSM MALFUNCTION DETECTED", ERROR);
        trace_event("End   key_exchange_scheme(MALFUNCTION)",PROCESSING);
        return(HSM_UNAVAILABLE);
  }

  retour = command_FB ( BuffRcv , Zpk_Lmk , Check_Value_Out , sHsmResultCode );
  if ( retour != OK )
  {
        trace_event("HSM Abnormal  DETECTED", ERROR);
        trace_event("End   key_exchange_scheme(CRYPTOGRAPHIC_ERROR)",PROCESSING);
        return(CRYPTOGRAPHIC_ERROR);
  }
  
  if(memcmp(Check_Value_Out,Check_Value_In,strlen(Check_Value_In)) != 0)
  if ( retour != OK )
  {
        trace_event("CHECK VALUES ARE DIFFERENT", ERROR);
        trace_event("End   key_exchange_scheme(CRYPTOGRAPHIC_ERROR)",PROCESSING);
        return(CRYPTOGRAPHIC_ERROR);
  }

  trace_event("End   key_exchange_scheme(OK)",PROCESSING);
  return(OK);
}

/*****************************************************************************/
/* Fonction pin_translate                                                    */
/* ----------------------                                                    */
/* Description : Cette fonction effectue le transchiffrement du pin d'une ZPK*/
/*               vers une autre ZPK.                                         */
/*****************************************************************************/
int  pin_translate (nIndiceCtx, card, transkey, zpk, pin, format_in, format_out, mode )
int nIndiceCtx; 
char *card, *transkey, *zpk, *pin, *format_in, *format_out, mode;
{
  int	  retour;
  char    BuffSnd    [128];
  char    BuffRcv    [128];
  char    ligne[MAX_LINE_TRC];
  char    sHsmResultCode[5];
  pid_t   nPid;


  memset(sHsmResultCode , 0 , 5 );
  /*sprintf(ligne, "Start pin_translate(%.22s,%.16s,%.16s,%.16s,%.2s,%.2s,%c)",
        card, transkey, zpk, pin, format_in, format_out, mode);
   trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
   trace_event("Start pin_translate()", PROCESSING);

  sprintf(ligne, "formatou  %.2s", format_out);
   trace_event(ligne, PROCESSING);

  memset(BuffSnd, 0, sizeof(BuffSnd) );
  memset(BuffRcv, 0, sizeof(BuffRcv) );


  if ( mode == TERMINAL_MODE )
    command_CA (BuffSnd,transkey, zpk, pin, format_in, format_out, card);
    /*command_CA (BuffSnd,transkey, zpk, "1D01A422255F1E29", format_in, format_out, card);*/
  else
    command_CC (BuffSnd,transkey, zpk, pin, format_in, format_out, card);

   retour = CheckResource ( nIndiceCtx, HSM_VERIF , &nPid );
   if ( ( retour != OK ) )
   {
      trace_event("HSM FOR TRANSLATION NOT AVAILABLE", ERROR);
      trace_event("End   pin_translate(MALFUNCTION)",PROCESSING);
      return(HSM_UNAVAILABLE);
   }

    retour = HsmQuery (nIndiceCtx,nPid, BuffSnd, BuffRcv);
    if (retour == NOT_AVAILABLE)
    {
        trace_event("HSM NOT AVAILABLE", ERROR);
        trace_event("End   pin_translate()",PROCESSING);
        return(HSM_UNAVAILABLE);
   }

   if (retour == HSM_MALFUNCTION)
   {
        trace_event("HSM MALFUNCTION DETECTED", ERROR);
        trace_event("End   pin_translate()",PROCESSING);
        return(HSM_UNAVAILABLE);
   }

    if ( mode == TERMINAL_MODE )
     retour = command_CB (BuffRcv, pin , sHsmResultCode );
    else
     retour = command_CD (BuffRcv, pin , sHsmResultCode );

    if ( retour != OK )
    {
        trace_event("End   pin_translate(NOK)",PROCESSING);
        return(CRYPTOGRAPHIC_ERROR);
    }

  trace_event("End   pin_translate()", PROCESSING);
  return(OK);
}

/***********************************************************************************/
/*                                                                                 */
/***********************************************************************************/
int TranslateAwkFromZmkToLmk (int nIndexCtx,char *Zmk,  char *AwkZmk,  char *AwkLmk , char *tmpFieldLen)
{
  pid_t   nPid;
  int	retour;
  char  BuffSnd [128];
  char  BuffRcv [128];
  char ligne[MAX_LINE_TRC];
  char sHsmResultCode[5];
  char CheckValue [ 16 + 1];
  char Key        [ 16 + 1];

  memset(sHsmResultCode , 0 , 5 );
  memset(BuffSnd, 0, sizeof(BuffSnd) );
  memset(BuffRcv, 0, sizeof(BuffRcv) );
  memset(CheckValue, 0,  sizeof ( CheckValue ));
  memset(Key , 0,  sizeof ( Key ));

  /*sprintf(ligne, "Start TranslateAwkFromZmkToLmk (%.16s, %.16s)", 
		              Zmk,AwkZmk);
  trace_event(ligne, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
  trace_event("Start TranslateAwkFromZmkToLmk ()", PROCESSING);

  command_FA(BuffSnd, Zmk, AwkZmk);
  if ( CheckResource ( nIndexCtx, HSM_VERIF , &nPid ) != OK )
  {
         trace_event ( "End   TranslateAwkFromZmkToLmk(TranslateAwkFromZmkToLmk)", ERROR );
         return ( HSM_UNAVAILABLE);
   }

  retour = HsmQuery(nIndexCtx,nPid,BuffSnd, BuffRcv);
  if (retour == NOT_AVAILABLE)
  {
    trace_event("HSM NOT AVAILABLE", ERROR);
    trace_event("End   TranslateAwkFromZmkToLmk(NOT_AVAILABLE)",PROCESSING);
    return(HSM_UNAVAILABLE);
  }

  if (retour == HSM_MALFUNCTION)
  {
        trace_event("HSM MALFUNCTION DETECTED", ERROR);
        trace_event("End   TranslateAwkFromZmkToLmk(MALFUNCTION)",PROCESSING);
        return(HSM_UNAVAILABLE);
  }

  retour = command_FB ( BuffRcv , Key , CheckValue , sHsmResultCode );
  if ( retour != OK )
  {
        trace_event("HSM Abnormal  DETECTED", ERROR);
        trace_event("End   TranslateAwkFromZmkToLmk(CRYPTOGRAPHIC_ERROR)",PROCESSING);
        return(CRYPTOGRAPHIC_ERROR);
  }
  
  memcpy( AwkLmk  , Key,  strlen ( Key ));
  *tmpFieldLen = strlen ( Key );

  trace_event("End   TranslateAwkFromZmkToLmk(OK)",PROCESSING);
  return(OK);
}

/************************************************************************
  **  nom       : TpkFromZmkToLmk                                       **
  **  desc      : Traduction d'un TPK/ZMK en un TPK/LMK                 **
  **  entree    : pZmkKey       -> La clé ZMK                           **
  **              pTpkKey       -> Tpk sous ZMK                 **
  **  sortie    : Tpk  en LMK
  **
       KHALOUFI
  ************************************************************************/


int     TpkFromZmkToLmk(nIndexCtx,pZmkKey, pTpkKey, pTpkLmk)
int                             nIndexCtx;
unsigned char   pZmkKey         [];
unsigned char   pTpkKey         [];
unsigned char   pTpkLmk         [];
{
unsigned char   lHsmBuffIn      [LG_MAX];
unsigned char   lHsmBuffOut     [LG_MAX];
unsigned char   lErrCode        [     3];
char            sLine           [MAX_LINE_TRC];
pid_t                   nPid;
int                     lRet;
int                     length;

/*sprintf(sLine, "Start TpkFromZmkToLmk(%.32s,%.32s)",
               pZmkKey, pTpkKey);
trace_event(sLine, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
trace_event("Start TpkFromZmkToLmk()", PROCESSING);

    memset(lHsmBuffIn           , 0, sizeof(lHsmBuffIn) );
    memset(lHsmBuffOut          , 0, sizeof(lHsmBuffOut));
    memset(lErrCode             , 0, sizeof(lErrCode)   );

    /* - Construction du tampon ------------------------------------------ */
    command_FC(lHsmBuffIn,pZmkKey, pTpkKey);

    /* - Appel HSM ------------------------------------------------------- */
   lRet = CheckResource ( nIndexCtx, HSM_VERIF , &nPid );
   if ( ( lRet != OK ) )
   {
      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
      trace_event("End   TpkFromZmkToLmk(HSM_UNAVAILABLE)",PROCESSING);
      return(HSM_UNAVAILABLE);
   }

   lRet = HsmQuery( nIndexCtx,nPid , lHsmBuffIn, lHsmBuffOut );
if ( lRet == NOT_AVAILABLE )
   {
      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
      trace_event("End   TpkFromZmkToLmk(HSM_UNAVAILABLE)",PROCESSING);
      return(HSM_UNAVAILABLE);
   }

   if (lRet == HSM_MALFUNCTION)
   {
      trace_event("HSM MALFUNCTION DETECTED",ERROR);
      trace_event("End   TpkFromZmkToLmk(SYSTEM_MALFUNCTION)",PROCESSING);
      return(HSM_MALFUNCTION);
   }

        trace_event("End  TpkFromZmkToLmk(OK)",PROCESSING);
    /* - Extraction de la reponse --------------------------------------- */
    lRet = command_FD(lHsmBuffOut, lErrCode, pTpkLmk);
    if (memcmp(lErrCode, VERIFICATION_ERROR, 2) == 0)
    {/*YK:1208*/
     trace_event("WARNING FORCAGE PARITE",PROCESSING);

     /*  trace_event("End   TpkFromZmkToLmk(NOK)",PROCESSING);
       return(NOK);*/
    }
    if ((memcmp(lErrCode, "00", 2) != 0) && (memcmp(lErrCode, "01", 2) != 0))
    {
       trace_event("End   TpkFromZmkToLmk(SYSTEM_MALFUNCTION)",PROCESSING);
       return(SYSTEM_MALFUNCTION);
    }

/*sprintf(sLine, "End   TpkFomZmkToLmk(%.16s)", pTpkLmk);
trace_event(sLine, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
trace_event("End   TpkFomZmkToLmk(OK)", PROCESSING);
return(OK);
}

