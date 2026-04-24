/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      hps_gen_key.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      Key Generation Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <hsm_inc.h>
#include <hsm_param.h>

  /***********************************************************************
  **  nom	: GenerateTPKKey()					**
  **  desc	: generation d'une TPK cryptee sous la TMK		**
  **  entree	: pTermNbr	-> numero de terminal			**
  **  sortie	: pTPKKey	-> la TPK/TMK				**
  **  retour	: OK	-> normal					**
  **  auteur	: Younes OUADI (07 Avr 1999)				**
  ***********************************************************************/
int	GenerateTPKKey(nIndexCtx,pTermNbr, pTpkLmk, pTpkTmk)
int		nIndexCtx;
char	*pTermNbr;
char	*pTpkLmk;
char	*pTpkTmk;
{
char	lHSMBufferOut	[LG_MAX];
char	lHSMBufferIn	[LG_MAX];
int     length;

char	lTmkLmk		[ 256];
char	lTmkZmk		[ 256];
int		lRet;
pid_t   nPid;
char    detail		[LG_MAX];
char    lErrCode		[LG_MAX];

sprintf(detail, "Start GenerateTPKKey(%.15s)", pTermNbr);
trace_event(detail, PROCESSING);

    /* - memset des buffers ------------------------------------------------- */
    memset(lHSMBufferOut,	0, sizeof(lHSMBufferOut));
    memset(lHSMBufferIn,	0, sizeof(lHSMBufferIn)	);
    memset(lTmkLmk,			0, sizeof(lTmkLmk)	);
    memset(lTmkZmk,			0, sizeof(lTmkZmk)	);
    memset(lErrCode,		0, sizeof(lErrCode)	);

    /* - recuperation de la cle TMK/LMK ------------------------------------- */
    lRet = GetTMK(nIndexCtx,pTermNbr, lTmkLmk, lTmkZmk);
    if(lRet != OK)
    {
        trace_event("Err. In GetTMK()", ERROR);
        trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
        return (lRet);
    }

    /* - preparation des buffers pour HSM ----------------------------------- */
    lRet = command_HC(lHSMBufferIn, lTmkLmk);
    if(lRet != OK)
    {
        trace_event("Err. In command_HC()", ERROR);
        trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
        return (lRet);
    }

    /* - demande de generation de la TPK/TMK -------------------------------- */
   length = strlen((char*)lHSMBufferIn);
   lRet = CheckResource ( nIndexCtx, HSM_VERIF , &nPid );
   if ( ( lRet != OK ) )
   {
      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
      trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
      trace_event("End   GenerateTPKKey(HSM_UNAVAILABLE)", PROCESSING);
      return(HSM_UNAVAILABLE);
   }
   lRet = HsmQuery(nIndexCtx, nPid , lHSMBufferIn, lHSMBufferOut );
   if ( lRet == NOT_AVAILABLE )
   {
      trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
      trace_event("End   GenerateTPKKey(HSM_UNAVAILABLE)", PROCESSING);
      return(HSM_UNAVAILABLE);
   }

   if (lRet == HSM_MALFUNCTION)
   {
      trace_event("HSM MALFUNCTION DETECTED",ERROR);
      trace_event("End   GenerateTPKKey(HSM_UNAVAILABLE)", PROCESSING);
      return(HSM_MALFUNCTION);
   }

    /* - recuperation des donnes du buffers HSM ----------------------------- */
	
    lRet = command_HD(lHSMBufferOut, pTpkTmk, pTpkLmk, lErrCode);
    if(lRet != OK)
    {
        trace_event("Err. In command_HC()", ERROR);
        trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
        return (lRet);
    }

/*sprintf(detail, "End   GenerateTPKKey(%.32s,%.32s)", pTpkLmk, pTpkTmk);
trace_event(detail, PROCESSING);*/ /** HM - 09022010 - PA_DSS **/
trace_event("End   GenerateTPKKey(OK)", PROCESSING);
return (OK);
}

