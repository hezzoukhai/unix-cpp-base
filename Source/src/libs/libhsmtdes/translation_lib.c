/*******************************************************************************/
/* (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/* translation_lib.c                                                      */
/* */
/* Description                                                                 */
/* Transalation Tools Management                                          */
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
#include <p7_define.h>

/*****************************************************************************/
/* Fonction key_exchange                                                     */
/* ---------------------                                                     */
/* Description : Cette fonction effectue le transchiffrement de la cle de    */
/* transport du Pin de la ZMK vers la LMK.                     */
/*****************************************************************************/
int 
	translate_key_scheme(
	int             nIndexCtx,
	char           *Zmk_Key,
	char           *Zpk_Zmk,
	char           *Check_Value_In,
	char           *Zpk_Lmk,
	char           *Check_Value_Out,
	char		   *sAttalaVar)
{
	pid_t           nPid;
	int             retour;
	char            BuffSnd[BUF_LG_MAX];
	char            BuffRcv[BUF_LG_MAX];
	char            ligne[MAX_LINE_TRC];
	char            sHsmResultCode[5];
	char			sLMKId[8];

	memset(sHsmResultCode, 0, sizeof(sHsmResultCode));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(Check_Value_Out, 0, 16);
	memset(sLMKId, 0, sizeof(sLMKId));


	if(debug_option ==1)
		sprintf(ligne, "Start translate_key_scheme(%s, %s, %.16s)", Zmk_Key, Zpk_Zmk, Check_Value_In);
	else
		sprintf(ligne, "Start translate_key_scheme(%.16s)",Check_Value_In);
	trace_event(ligne, PROCESSING);

	if( Zmk_Key[0] != 'S' )
	{
		trace_event("Invalid ZMK Key scheme", ERROR);
		trace_event("End   translate_key_scheme(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,Zmk_Key + 1 + 14,2);

	/*command_A6(BuffSnd, THALES_KEY_TP_ZPK, Zmk_Key, Zpk_Zmk,sAttalaVar);*/

	command_A6(	BuffSnd ,
				"FFF",
				Zmk_Key,
				Zpk_Zmk,
				THALES_KU_ZPK,
				/*THALES_ALGO_DES_2,*//*NAB180022019 Key Block*/
				THALES_MU_NO_RESTRICT,
				THALES_VN_00,
				THALES_EX_S,
				sLMKId,
				sAttalaVar
				);

	if(debug_option ==1)
	{
		sprintf(ligne, "BuffSnd[%.100s]", BuffSnd);
		trace_event(ligne, PROCESSING);
	}


	if (CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid) != OK) 
	{
		trace_event("Resource HSM VERIF Unavailable", ERROR);
		return (HSM_UNAVAILABLE);
	} 


	retour = HsmQuery(nIndexCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE) 
	{
		trace_event("HSM NOT AVAILABLE", ERROR);
		trace_event("End   translate_key_scheme(NOT_AVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION) 
	{
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   translate_key_scheme(MALFUNCTION)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}


	retour = command_A7 ( BuffRcv , Zpk_Lmk, Check_Value_Out ); 
	if (retour != OK) {
		trace_event("HSM Abnormal  DETECTED", ERROR);
		trace_event("End   translate_key_scheme(CRYPTOGRAPHIC_ERROR)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}

	if (memcmp(Check_Value_Out, Check_Value_In, strlen(Check_Value_In)) != 0)
	{
		trace_event("CHECK VALUES ARE DIFFERENT", ERROR);
		trace_event("End   translate_key_scheme(CRYPTOGRAPHIC_ERROR)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}

	trace_event("End   translate_key_scheme(OK)", PROCESSING);
	return (OK);
}

int 
	key_exchange_scheme(
	int             nIndexCtx,
	char           *Zmk_Key,
	char           *Zpk_Zmk,
	char           *Check_Value_In,
	char           *Zpk_Lmk,
	char           *Check_Value_Out)
{
	/* Temporary solution in order to compile */
	return translate_key_scheme(nIndexCtx,Zmk_Key,Zpk_Zmk,Check_Value_In,Zpk_Lmk,Check_Value_Out,""); 
}


/*****************************************************************************/
/* Fonction key_exchange                                                     */
/* ---------------------                                                     */
/* Description : Cette fonction effectue le transchiffrement de la cle de    */
/* transport du Pin de la ZMK vers la LMK.                     */
/*****************************************************************************/
int 
	key_exchange_scheme2(
	int             nIndexCtx,
	char           *Zmk_Key,
	char           *Zpk_Zmk,
	char           *Check_Value_In,
	char           *Zpk_Lmk,
	char           *Check_Value_Out)
{

	return translate_key_scheme(nIndexCtx,Zmk_Key,Zpk_Zmk,Check_Value_In,Zpk_Lmk,Check_Value_Out,"");
}

/*****************************************************************************/
/* Fonction pin_translate                                                    */
/* ----------------------                                                    */
/* Description : Cette fonction effectue le transchiffrement du pin d'une ZPK */
/* vers une autre ZPK.                                         */
/*****************************************************************************/
int  pin_translate( int nIndiceCtx, char* card, char* transkey, char* zpk, char* pin, char* format_in, char* format_out, char mode)
{
	int             retour;
	char            BuffSnd[BUF_LG_MAX];
	char            BuffRcv[BUF_LG_MAX];/*NAB18022019 Key Block*/
	char            ligne[MAX_LINE_TRC];
	char            sHsmResultCode[5];
	pid_t           nPid;


	memset(sHsmResultCode, 0, 5);
	/* Mehdi Elyajizi PCI */
	if(debug_option ==1)
		/*sprintf(ligne, "Start pin_translate(%.22s,%.16s,%.16s,%.16s,%.2s,%.2s,%c)",
		card, transkey, zpk, pin, format_in, format_out, mode);*/
		sprintf(ligne, "Start pin_translate(%.22s,%s,%s,%.16s,%.2s,%.2s,%c)",
		card, transkey, zpk, pin, format_in, format_out, mode);/*NAB180022019 Key Block*/
	else
		sprintf(ligne, "Start pin_translate()");
	trace_event(ligne, PROCESSING);

	/* Mehdi Elyajizi PCI */
	sprintf(ligne, "formatou  %.2s", format_out);
	trace_event(ligne, PROCESSING);

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));


	if (mode == TERMINAL_MODE)
		command_CA(BuffSnd, transkey, zpk, pin, format_in, format_out, card);
	else
		command_CC(BuffSnd, transkey, zpk, pin, format_in, format_out, card);

	if(debug_option ==1)
	{
		sprintf(ligne, "transkey pin_translate(%s)",transkey);
		trace_event(ligne, PROCESSING);
	}

	/* 	retour = CheckResource(nIndiceCtx, HSM_VERIF, &nPid);*/
	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);

	if ((retour != OK)) {
		trace_event("HSM FOR TRANSLATION NOT AVAILABLE", ERROR);
		trace_event("End   pin_translate(MALFUNCTION)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}


	retour = HsmQuery(nIndiceCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE) {
		trace_event("HSM NOT AVAILABLE", ERROR);
		trace_event("End   pin_translate()", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (retour == HSM_MALFUNCTION) {
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   pin_translate()", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (mode == TERMINAL_MODE)
		retour = command_CB(BuffRcv, pin, sHsmResultCode);
	else
		retour = command_CD(BuffRcv, pin, sHsmResultCode);

	if (retour != OK) 
	{
		trace_event("End   pin_translate(NOK)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}
	
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	trace_event("End   pin_translate(OK)", PROCESSING);
	return (OK);
}

/***********************************************************************************/
/* */
/***********************************************************************************/
/* PROD00065699: not used
int 
TranslateAwkFromZmkToLmk(int nIndexCtx, char *Zmk, char *AwkZmk, char *AwkLmk, char *tmpFieldLen)
{
pid_t           nPid;
int             retour;
char            BuffSnd[128];
char            BuffRcv[128];
char            ligne[MAX_LINE_TRC];
char            sHsmResultCode[5];
char            CheckValue[16 + 1];
char            Key[256 + 1];

memset(sHsmResultCode, 0, 5);
memset(BuffSnd, 0, sizeof(BuffSnd));
memset(BuffRcv, 0, sizeof(BuffRcv));
memset(CheckValue, 0, sizeof(CheckValue));
memset(Key, 0, sizeof(Key));

if(debug_option ==1)
sprintf(ligne, "Start TranslateAwkFromZmkToLmk (%.16s, %.16s)",Zmk, AwkZmk);
else
sprintf(ligne, "Start TranslateAwkFromZmkToLmk ()");
trace_event(ligne, PROCESSING);

command_FA(BuffSnd, Zmk, AwkZmk);


if (CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid) != OK) {
trace_event("End   TranslateAwkFromZmkToLmk(TranslateAwkFromZmkToLmk)", ERROR);
return (HSM_UNAVAILABLE);
} 

retour = HsmQuery(nIndexCtx, nPid, BuffSnd, BuffRcv);
if (retour == NOT_AVAILABLE) {
trace_event("HSM NOT AVAILABLE", ERROR);
trace_event("End   TranslateAwkFromZmkToLmk(NOT_AVAILABLE)", PROCESSING);
return (HSM_UNAVAILABLE);
}
if (retour == HSM_MALFUNCTION) {
trace_event("HSM MALFUNCTION DETECTED", ERROR);
trace_event("End   TranslateAwkFromZmkToLmk(MALFUNCTION)", PROCESSING);
return (HSM_UNAVAILABLE);
}
retour = command_FB(BuffRcv, Key, CheckValue, sHsmResultCode);
if (retour != OK) {
trace_event("HSM Abnormal  DETECTED", ERROR);
trace_event("End   TranslateAwkFromZmkToLmk(CRYPTOGRAPHIC_ERROR)", PROCESSING);
return (CRYPTOGRAPHIC_ERROR);
}

memcpy(AwkLmk, Key, strlen(Key));
*tmpFieldLen = strlen(Key);

trace_event("End   TranslateAwkFromZmkToLmk(OK)", PROCESSING);
return (OK);
}
*/
/************************************************************************
**  nom       : TpkFromZmkToLmk                                       **
**  desc      : Traduction d'un TPK/ZMK en un TPK/LMK                 **
**  entree    : pZmkKey       -> La cl� ZMK                           **
**              pTpkKey       -> Tpk sous ZMK                 **
**  sortie    : Tpk  en LMK
**
KHALOUFI
************************************************************************/


int 
	TpkFromZmkToLmk(
	int             nIndexCtx,
	unsigned char   pZmkKey[],
	unsigned char   pTpkKey[],
	unsigned char   pTpkLmk[])
{

	unsigned char   lHsmBuffIn[BUF_LG_MAX];
	unsigned char   lHsmBuffOut[BUF_LG_MAX];

	char			check_out[7];
	unsigned char   lErrCode[3];
	char            sLine[MAX_LINE_TRC];
	char			sLMKId[8];
	char			sKeyType[3+1];
	pid_t           nPid;
	int             lRet;
	int             length;
	


	
	if(debug_option ==1)
		sprintf(sLine, "Start TpkFromZmkToLmk(%.32s,%.32s)",  pZmkKey, pTpkKey);
	else
		sprintf(sLine, "Start TpkFromZmkToLmk()");
	trace_event(sLine, PROCESSING);
	


	memset(lHsmBuffIn, 0, sizeof(lHsmBuffIn));
	memset(lHsmBuffOut, 0, sizeof(lHsmBuffOut));
	memset(lErrCode, 0, sizeof(lErrCode));
	memset(sKeyType, 0, sizeof(sKeyType));
	memset(sLMKId, 0, sizeof(sLMKId));
	


	/*if( pZmkKey[0] != 'S' )
	{
		trace_event("Invalid ZMK Key scheme", ERROR);
		trace_event("End   TpkFromZmkToLmk(NOK)", PROCESSING);
		return (NOK);
	}*/

	if(pZmkKey[0] == 'S' )
	{
		memcpy(sLMKId, pZmkKey + 1 + 14,2);
		memcpy(sKeyType, "FFF",3);
		
	}
	else
	{
		memcpy(sKeyType, THALES_KEY_TP_TPK,3);
	}
	
    

	/*
	* - Construction du tampon
	* ------------------------------------------ 
	*/
	/*command_A6(lHsmBuffIn, "002", pZmkKey, pTpkKey,"");*/
	command_A6(	lHsmBuffIn ,
				sKeyType,
				pZmkKey,
				pTpkKey,
				THALES_KU_TPK,
				/*THALES_ALGO_DES_2,*//*NAB180022019 Key Block*/
				THALES_MU_NO_RESTRICT,
				THALES_VN_00,
				THALES_EX_S,
				sLMKId,
				""
				);

	/*
	* - Appel HSM
	* ------------------------------------------------------- 
	*/
	/* 	lRet = CheckResource(nIndexCtx, HSM_VERIF, &nPid);*/
	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((lRet != OK)) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   TpkFromZmkToLmk(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	} 

	lRet = HsmQuery(nIndexCtx, nPid, lHsmBuffIn, lHsmBuffOut);
	if (lRet == NOT_AVAILABLE) 
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   TpkFromZmkToLmk(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	if (lRet == HSM_MALFUNCTION) 
	{
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   TpkFromZmkToLmk(SYSTEM_MALFUNCTION)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	/* - Extraction de la reponse --------------------------------------- */

	lRet=command_A7 ( lHsmBuffOut , pTpkLmk, check_out );
	if ( lRet != OK )
	{
		trace_event("HSM Abnormal  DETECTED", ERROR);
		trace_event("End   TpkZmkToLmk(CRYPTOGRAPHIC_ERROR)",PROCESSING);
		return(CRYPTOGRAPHIC_ERROR);
	}


	/* Mehdi Elyajizi PCI */
	if(debug_option ==1)
		sprintf(sLine, "End   TpkFromZmkToLmk(%s)", pTpkLmk);
	else
		sprintf(sLine, "End   TpkFromZmkToLmk(OK)");
	trace_event(sLine, PROCESSING);
	/* Mehdi Elyajizi PCI */

	return (OK);
}
/** START MAW20030704 **/
/*****************************************************************************/
/* Fonction FctTAKKeyExchange                                              */
/* ---------------------                                                     */
/*****************************************************************************/
int  FctTAKKeyExchange(
	int  nIndexCtx,
	char *Zmk_Key,
	char *Tak_Zmk,
	char *Check_Value_In,
	char *Tak_Lmk,
	char *Check_Value_Out)
{
	pid_t nPid;
	int   retour;
	char  BuffSnd [BUF_LG_MAX];/*NAB180022019 Key Block length of buffer!!*/
	char  BuffSnd1 [BUF_LG_MAX];
	char  BuffRcv [BUF_LG_MAX];
	char ligne[MAX_LINE_TRC];
	char sHsmResultCode[5];
	char			sLMKId[8];

	memset(sHsmResultCode , 0 , sizeof(sHsmResultCode) );
	memset(BuffSnd, 0, sizeof(BuffSnd) );
	memset(BuffSnd1, 0, sizeof(BuffSnd1) );
	memset(BuffRcv, 0, sizeof(BuffRcv) );
	memset(sLMKId, 0, sizeof(sLMKId));
	memset(Check_Value_Out   , 0,             16);

	if (debug_option == 1)
		sprintf(ligne, "Start FctTAKKeyExchange(%s, %s, %.16s)",
						Zmk_Key,Tak_Zmk,Check_Value_In );
	else
		sprintf(ligne, "Start FctTAKKeyExchange(%.16s)",Check_Value_In );
	trace_event(ligne, PROCESSING);

	if( Zmk_Key[0] != 'S' )
	{
		trace_event("Invalid ZMK Key scheme", ERROR);
		trace_event("End   FctTAKKeyExchange(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,Zmk_Key + 1 + 14,2);

	/*command_A6(BuffSnd1, THALES_KEY_TP_TAK, Zmk_Key, Tak_Zmk,"");*/

	command_A6(	/*BuffSnd*/BuffSnd1 ,
				"FFF",
				Zmk_Key,
				Tak_Zmk,
				ISO9797_1_ALGO3_MAC_KEY,
				/*THALES_ALGO_DES_2,NAB18022019 Key Block*/
				THALES_MU_NO_RESTRICT,
				THALES_VN_00,
				THALES_EX_S,
				sLMKId,
				""
				);

	strcpy(BuffSnd, BuffSnd1);
	/*   if ( CheckResource ( nIndexCtx, HSM_VERIF , &nPid ) != OK )*/
	if (CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid) != OK)
	{
		trace_event ( "Resource HSM VERIF Unavailable", ERROR );
		return ( HSM_UNAVAILABLE );
	} 

	if (debug_option == 1)
	{
		sprintf(ligne, " BuffSnd[%.80s]", BuffSnd );
		trace_event(ligne, PROCESSING);
	}


	retour = HsmQueryBin(nIndexCtx,nPid,BuffSnd, strlen(BuffSnd), BuffRcv);
	if (retour == NOT_AVAILABLE)
	{
		trace_event("HSM NOT AVAILABLE", ERROR);
		trace_event("End   FctTAKKeyExchange(NOT_AVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   FctTAKKeyExchange(MALFUNCTION)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (debug_option == 1)
	{
		sprintf(ligne, "==> BuffRcv=[%.80s]", BuffRcv );
		trace_event(ligne, PROCESSING);
	}
	retour=command_A7 (   BuffRcv , Tak_Lmk, Check_Value_Out );
	if ( retour != OK )
	{
		trace_event("HSM Abnormal  DETECTED", ERROR);
		trace_event("End   FctTAKKeyExchange(CRYPTOGRAPHIC_ERROR)",PROCESSING);
		return(CRYPTOGRAPHIC_ERROR);
	}

	if (debug_option == 1)
	{
		sprintf(ligne, "==> Tak_Lmk=[%s]", Tak_Lmk );
		trace_event(ligne, PROCESSING);
	}
	sprintf(ligne, "==> Check_Value_Out=[%s]", Check_Value_Out );
	trace_event(ligne, PROCESSING);



	trace_event("End   FctTAKKeyExchange(OK)",PROCESSING);
	return(OK);
}

/*
int   ConvTakToTpk (
int             nIndiceCtx,
unsigned char * pResourceId,
unsigned char * pTerminalNbr,
unsigned char * pInTakLmk,
unsigned char * pOutTpkLmk,
unsigned char * pOutTpkZmk)
{


unsigned char   lErrCode        [     3];
unsigned char     vZmkLmk [ 256 + 1 ];
unsigned char     vTakZmk [ 256 + 1 ];
unsigned char     vTpkLmk [ 256 + 1 ];
unsigned char     vTpkZmk [ 256 + 1 ];
int      ret_val;
char      vHsmBufferOut [ 256 ];
char      vHsmBufferIn  [ 256 ];
char      vHsmErrorCode [ 8 ];
int      vLen;
char     sLine[MAX_LINE_TRC];
int      retour;
pid_t                   nPid;
char     checkvalue    [16 + 1];

if (debug_option == 1)
sprintf(sLine, "START ConvTakToTpk ([%.6s],[%.15s],[%.32s])", pResourceId, pTerminalNbr, pInTakLmk);
else
sprintf(sLine, "START ConvTakToTpk ([%.6s],[%.15s])", pResourceId, pTerminalNbr);
trace_event(sLine, PROCESSING);

memset(vZmkLmk, 0, sizeof(vZmkLmk));
memset(vTakZmk, 0, sizeof(vTakZmk));
memset(vTpkLmk, 0, sizeof(vTpkLmk));
memset(vTpkZmk, 0, sizeof(vTpkZmk));
memset(vHsmBufferOut, 0, sizeof(vHsmBufferOut));
memset(vHsmBufferIn, 0, sizeof(vHsmBufferIn));
memset(vHsmErrorCode, 0, sizeof(vHsmErrorCode));
memset(sLine, 0, sizeof(sLine));
memset(&nPid, 0, sizeof(&nPid));
memset(checkvalue, 0, sizeof(checkvalue));
memset(lErrCode, 0, sizeof(lErrCode));


ret_val = GetZMK(nIndiceCtx, pResourceId, vZmkLmk);
if ( OK != ret_val )
{
trace_event("END   ConvTakToTpk (NOK)", PROCESSING);
return(NOK);
}


if (debug_option == 1)
{
sprintf(sLine, "ZMK/LMK: [%.32s] TAK/LMK: [%.32s]",vZmkLmk, pInTakLmk);
trace_event(sLine, PROCESSING);
}

ret_val = command_MG(vHsmBufferIn, vZmkLmk, pInTakLmk);
if ( OK != ret_val )
{
trace_event("END   ConvTakToTpk(NOK .command_MG)",PROCESSING);
return(NOK);
}

ret_val = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
if ( ( ret_val != OK ) )
{
trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
trace_event("End   PinFromTpkToLmk(HSM_UNAVAILABLE)",PROCESSING);
return(HSM_UNAVAILABLE);
} 

retour = HsmQuery(nIndiceCtx, nPid, vHsmBufferIn, vHsmBufferOut);
if (retour == NOT_AVAILABLE)
{
trace_event("HSM NOT AVAILABLE", ERROR);
trace_event("End   ConvTakToTpk(HSM NOT_AVAILABLE)",PROCESSING);
return(HSM_UNAVAILABLE);
}

if (retour == HSM_MALFUNCTION)
{
trace_event("HSM MALFUNCTION DETECTED", ERROR);
trace_event("End   ConvTakToTpk(MALFUNCTION)",PROCESSING);
return(HSM_UNAVAILABLE);
}

ret_val = command_MH(vHsmBufferOut, vTakZmk, checkvalue, vHsmErrorCode);
if ( OK != ret_val )
{
trace_event("END   ConvTakToTpk(NOK .command_MH)",PROCESSING);
return(NOK);
}



memset(vHsmBufferOut, 0, sizeof(vHsmBufferOut));
memset(vHsmBufferIn, 0, sizeof(vHsmBufferIn));
memset(vHsmErrorCode, 0, sizeof(vHsmErrorCode));

memcpy( vTpkZmk, vTakZmk, sizeof(vTakZmk));

if (debug_option == 1)
{
sprintf(sLine, "vTakZmk=[%s])", vTakZmk);
trace_event(sLine, PROCESSING);

sprintf(sLine, "vTpkZmk=[%s])", vTpkZmk);
trace_event(sLine, PROCESSING);   
}

ret_val = command_FC(vHsmBufferIn, vZmkLmk, vTpkZmk);
if ( OK != ret_val )
{
trace_event("END   ConvTakToTpk(NOK .command_FC)",PROCESSING);
return(NOK);
}

if (debug_option == 1)
{
sprintf(sLine, "vHsmBufferIn=[%s])", vHsmBufferIn);
trace_event(sLine, PROCESSING);
}
retour = HsmQuery(nIndiceCtx, nPid, vHsmBufferIn, vHsmBufferOut);
if (retour == NOT_AVAILABLE)
{
trace_event("HSM NOT AVAILABLE", ERROR);
trace_event("End   ConvTakToTpk(HSM NOT_AVAILABLE)",PROCESSING);
return(HSM_UNAVAILABLE);
}
if (retour == HSM_MALFUNCTION)
{
trace_event("HSM MALFUNCTION DETECTED", ERROR);
trace_event("End   ConvTakToTpk(MALFUNCTION)",PROCESSING);
return(HSM_UNAVAILABLE);
}

memset(checkvalue,  0,  sizeof(checkvalue));


ret_val = command_FD(vHsmBufferOut, lErrCode, vTpkLmk);
if ( OK != ret_val )
{
trace_event("END   ConvTakToTpk(NOK .command_MH)",PROCESSING);
return(NOK);
}


memcpy(pOutTpkLmk, vTpkLmk, strlen(vTpkLmk));
memcpy(pOutTpkZmk, vTpkZmk, strlen(vTpkZmk));

if (debug_option == 1)  
sprintf(sLine, "END   ConvTakToTpk(OK [%.32s].[%.32s])", pOutTpkLmk, pOutTpkZmk);
else
sprintf(sLine, "END   ConvTakToTpk(OK)");
trace_event(sLine, PROCESSING);
return(OK);

}
*/

/* EBE130115 */
int  ZakZmkToLmk(	int		nIndexCtx,
	char	*zmkLmk, 
	char	*zakZmk, 
	char	*check_in, 
	char	*zakLmk, 
	char	*check_out)
{
	

	pid_t	nPid;
	int		retour;
	char	BuffSnd [512];
	char	BuffRcv [512];
	char	ligne[MAX_LINE_TRC];
	char	sHsmResultCode[5];
	char	sLMKId[8];

	trace_event("Start ZakZmkToLmk()",PROCESSING);


	memset(sHsmResultCode , 0 , sizeof(sHsmResultCode) );
	memset(BuffSnd, 0, sizeof(BuffSnd) );
	memset(BuffRcv, 0, sizeof(BuffRcv) );
	memset(check_out   , 0,             16);
	memset(sLMKId, 0, sizeof(sLMKId));

	/*command_FK(BuffSnd, zmkLmk, zakZmk);*/
	/*command_A6(BuffSnd, THALES_KEY_TP_ZAK, zmkLmk, zakZmk,"");*/

	if( zmkLmk[0] != 'S' )
	{
		trace_event("Invalid ZMK Key scheme", ERROR);
		trace_event("End   ZakZmkToLmk(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,zmkLmk + 1 + 14,2);

	command_A6(	BuffSnd ,
				"FFF",
				zmkLmk,
				zakZmk,
				ISO9797_1_ALGO3_MAC_KEY,
				THALES_ALGO_DES_2,
				THALES_MU_NO_RESTRICT,
				THALES_VN_00,
				THALES_EX_S,
				sLMKId,
				""
				);

	if (CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid) != OK)
	{
		trace_event ( "Resource HSM VERIF Unavailable", ERROR );
		return ( HSM_UNAVAILABLE );
	}


	if (debug_option == 1)
	{
		sprintf(ligne, " BuffSnd[%.80s]", BuffSnd );
		trace_event(ligne, PROCESSING);
	}


	retour = HsmQueryBin(nIndexCtx,nPid,BuffSnd, strlen(BuffSnd), BuffRcv);
	if (retour == NOT_AVAILABLE)
	{
		trace_event("HSM NOT AVAILABLE", ERROR);
		trace_event("End   ZakZmkToLmk(NOT_AVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   ZakZmkToLmk(MALFUNCTION)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (debug_option == 1)
	{
		sprintf(ligne, "==> BuffRcv=[%.80s]", BuffRcv );
		trace_event(ligne, PROCESSING);
	}


	/*retour = command_FL ( BuffRcv , zakLmk , check_out , sHsmResultCode );*/
	retour=command_A7 (   BuffRcv , zakLmk, check_out );
	if ( retour != OK )
	{
		trace_event("HSM Abnormal  DETECTED", ERROR);
		trace_event("End   ZakZmkToLmk(CRYPTOGRAPHIC_ERROR)",PROCESSING);
		return(CRYPTOGRAPHIC_ERROR);
	}

	if (debug_option == 1)
	{
		sprintf(ligne, "==> Tak_Lmk=[%s]", zakLmk );
		trace_event(ligne, PROCESSING);
	}

	sprintf(ligne, "==> Check_Value_Out=[%s]", check_out );
	trace_event(ligne, PROCESSING);

	trace_event("End ZakZmkToLmk(OK)",PROCESSING);
	return OK;
}

/*****************************************************************************/
int  KCKSKtkToLmk(	int		nIndexCtx,
	char	mode,
	char	*ktkLmk, 
	char	*kcksKtk, 
	char	*check_in, 
	char	*kcksLmk, 
	char	*check_out)
{
	trace_event("Start KCKSKtkToLmk()",PROCESSING);

	pid_t	nPid;
	int		retour;
	char	BuffSnd [512];
	char	BuffRcv [512];
	char	ligne[MAX_LINE_TRC];
	char	sHsmResultCode[5];

	memset(sHsmResultCode , 0 , 5 );
	memset(BuffSnd, 0, sizeof(BuffSnd) );
	memset(BuffRcv, 0, sizeof(BuffRcv) );
	memset(check_out   , 0,             16);

	command_SU(BuffSnd, mode,kcksKtk, ktkLmk);

	/* 	if ( CheckResource ( nIndexCtx, HSM_VERIF , &nPid ) != OK  ) TBO12082014*/
	if (CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid) != OK)
	{
		trace_event ( "Resource HSM VERIF Unavailable", ERROR );
		return ( HSM_UNAVAILABLE );
	}

	if (debug_option == 1)
	{
		sprintf(ligne, " BuffSnd[%.80s]", BuffSnd );
		trace_event(ligne, PROCESSING);
	}

	retour = HsmQueryBin(nIndexCtx,nPid,BuffSnd, strlen(BuffSnd), BuffRcv);
	if (retour == NOT_AVAILABLE)
	{
		trace_event("HSM NOT AVAILABLE", ERROR);
		trace_event("End   KCKSKtkToLmk(NOT_AVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   KCKSKtkToLmk(MALFUNCTION)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (debug_option == 1)
	{
		sprintf(ligne, "==> BuffRcv=[%.80s]", BuffRcv );
		trace_event(ligne, PROCESSING);
	}

	retour = command_SV ( BuffRcv , kcksLmk , check_out , sHsmResultCode );
	if ( retour != OK )
	{
		trace_event("HSM Abnormal  DETECTED", ERROR);
		trace_event("End   ZakKtkToLmk(CRYPTOGRAPHIC_ERROR)",PROCESSING);
		return(CRYPTOGRAPHIC_ERROR);
	}

	if (debug_option == 1)
	{
		sprintf(ligne, "==> kcksLmk=[%s]", kcksLmk );
		trace_event(ligne, PROCESSING);
	}
	sprintf(ligne, "==> Check_Value_Out=[%s]", check_out );
	trace_event(ligne, PROCESSING);

	trace_event("End KCKSKtkToLmk(OK)",PROCESSING);
	return OK;
}
/*****************************************************************************/
int  KCKSLmkToKtk(	int		nIndexCtx,
	char	mode,
	char	*kcksLmk, 
	char	*ktkLmk, 
	char	*kcksKtk,
	char	*check_in)
{
	trace_event("Start KCKSLmkToKtk()",PROCESSING);

	pid_t	nPid;
	int		retour;
	char	BuffSnd [512];
	char	BuffRcv [512];
	char	ligne[MAX_LINE_TRC];
	char	sHsmResultCode[5];
	char	check_out [512];

	memset(sHsmResultCode , 0 , 5 );
	memset(BuffSnd, 0, sizeof(BuffSnd) );
	memset(BuffRcv, 0, sizeof(BuffRcv) );
	memset(check_out   , 0,             sizeof(check_out));

	command_SW(BuffSnd, mode,kcksLmk, ktkLmk);

	/* 	if ( CheckResource ( nIndexCtx, HSM_VERIF , &nPid ) != OK  ) TBO12082014*/
	if (CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid) != OK)
	{
		trace_event ( "Resource HSM VERIF Unavailable", ERROR );
		return ( HSM_UNAVAILABLE );
	}


	if (debug_option == 1)
	{
		sprintf(ligne, " BuffSnd[%.80s]", BuffSnd );
		trace_event(ligne, PROCESSING);
	}

	retour = HsmQueryBin(nIndexCtx,nPid,BuffSnd, strlen(BuffSnd), BuffRcv);
	if (retour == NOT_AVAILABLE)
	{
		trace_event("HSM NOT AVAILABLE", ERROR);
		trace_event("End   KCKSLmkToKtk(NOT_AVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   KCKSLmkToKtk(MALFUNCTION)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (debug_option == 1)
	{
		sprintf(ligne, "==> BuffRcv=[%.80s]", BuffRcv );
		trace_event(ligne, PROCESSING);
	}


	retour = command_SX ( BuffRcv , kcksKtk , check_out , sHsmResultCode );
	if ( retour != OK )
	{
		trace_event("HSM Abnormal  DETECTED", ERROR);
		trace_event("End   KCKSLmkToKtk(CRYPTOGRAPHIC_ERROR)",PROCESSING);
		return(CRYPTOGRAPHIC_ERROR);
	}

	if (debug_option == 1)
	{
		sprintf(ligne, "==> kcksKtk=[%s]", kcksKtk );
		trace_event(ligne, PROCESSING);
	}
	sprintf(ligne, "==> Check_Value_Out=[%s]", check_out );
	trace_event(ligne, PROCESSING);

	trace_event("End KCKSLmkToKtk(OK)",PROCESSING);
	return OK;
}
/*****************************************************************************/
int  KtkZmkToLmk(	int		nIndexCtx,
	char	*zmkLmk, 
	char	*ktkZmk, 
	char	*check_in, 
	char	*ktkLmk, 
	char	*check_out)
{
	

	pid_t	nPid;
	int		retour;
	char	BuffSnd [512];
	char	BuffRcv [512];
	char	ligne[MAX_LINE_TRC];
	char	sHsmResultCode[5];
	char	sLMKId[8];

	trace_event("Start KtkZmkToLmk()",PROCESSING);


	memset(sHsmResultCode , 0 , sizeof(sHsmResultCode) );
	memset(BuffSnd, 0, sizeof(BuffSnd) );
	memset(BuffRcv, 0, sizeof(BuffRcv) );
	memset(sLMKId, 0, sizeof(sLMKId));
	memset(check_out   , 0,             16);

	if( zmkLmk[0] != 'S' )
	{
		trace_event("Invalid ZMK Key scheme", ERROR);
		trace_event("End   KtkZmkToLmk(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,zmkLmk + 1 + 14,2);

	command_A6(	BuffSnd ,
				"FFF",
				zmkLmk,
				ktkZmk,
				THALES_KU_TMK,
				THALES_ALGO_DES_2,
				THALES_MU_NO_RESTRICT,
				THALES_VN_00,
				THALES_EX_S,
				sLMKId,
				""
				);

	/*command_A6(BuffSnd, "700", zmkLmk, ktkZmk,"");*/

	if (CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid) != OK)
	{
		trace_event ( "Resource HSM VERIF Unavailable", ERROR );
		return ( HSM_UNAVAILABLE );
	}

	if (debug_option == 1)
	{
		sprintf(ligne, " BuffSnd[%.80s]", BuffSnd );
		trace_event(ligne, PROCESSING);
	}

	retour = HsmQueryBin(nIndexCtx,nPid,BuffSnd, strlen(BuffSnd), BuffRcv);
	if (retour == NOT_AVAILABLE)
	{
		trace_event("HSM NOT AVAILABLE", ERROR);
		trace_event("End   KtkZmkToLmk(NOT_AVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   KtkZmkToLmk(MALFUNCTION)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (debug_option == 1)
	{
		sprintf(ligne, "==> BuffRcv=[%.80s]", BuffRcv );
		trace_event(ligne, PROCESSING);
	}

	retour = command_A7 ( BuffRcv , ktkLmk , check_out );
	if ( retour != OK )
	{
		trace_event("HSM Abnormal  DETECTED", ERROR);
		trace_event("End   KtkZmkToLmk(CRYPTOGRAPHIC_ERROR)",PROCESSING);
		return(CRYPTOGRAPHIC_ERROR);
	}

	if (debug_option == 1)
	{
		sprintf(ligne, "==> ktkLmk=[%s]", ktkLmk );
		trace_event(ligne, PROCESSING);
	}

	sprintf(ligne, "==> Check_Value_Out=[%s]", check_out );
	trace_event(ligne, PROCESSING);

	trace_event("End KtkZmkToLmk(OK)",PROCESSING);
	return OK;
}


/* EBE140417: Bring from libprocessing */
/************************************************************************
**  nom       : PinFromTpkToLmk                                       **
**  desc      : Traduction d'un PIN/TPK en un PIN/LMK                 **
**  entree    : pTpkKey	-> La cl� TPK                          	**
**              pPinBlk	-> PIN Block sous Tpk			**
**              pPinBlkFmt    -> Format du PIN			**
**              pCardNbr      -> Num�ro de carte			**
**  sortie    : PIN Block en LMK                                      **
************************************************************************/
int	PinFromTpkToLmk(
	int				nIndexCtx,
	unsigned char	pTpkKey		[],
	unsigned char	pPinBlk		[],
	unsigned char	pPinBlkFmt	[],
	unsigned char	pCardNbr	[],
	unsigned char	pPinLmk		[])
{
	unsigned char	lHsmBuffIn	[LG_MAX];
	unsigned char	lHsmBuffOut	[LG_MAX];
	unsigned char	lErrCode	[     3];
	char 		sLine		[MAX_LINE_TRC];
	pid_t    		nPid;
	int 			lRet;
	int 			length;


	if (debug_option == 1)
	{
		sprintf(sLine, "Start PinFromTpkToLmk(%.16s,%.16s,%.2s,%.22s)",
			pTpkKey, pPinBlk, pPinBlkFmt, pCardNbr);
		trace_event(sLine, PROCESSING);
	}
	else
		trace_event("Start PinFromTpkToLmk()", PROCESSING);

	memset(lHsmBuffIn		, 0, sizeof(lHsmBuffIn)	);
	memset(lHsmBuffOut		, 0, sizeof(lHsmBuffOut));
	memset(lErrCode		, 0, sizeof(lErrCode)	);

	/* - Construction du tampon ------------------------------------------ */
	command_JC(lHsmBuffIn,pTpkKey, pPinBlk, pPinBlkFmt, pCardNbr);

	/* - Appel HSM ------------------------------------------------------- */
	/*    lRet = CheckResource ( nIndexCtx, HSM_VERIF , &nPid );*/
	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( lRet != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   PinFromTpkToLmk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}


	lRet = HsmQuery( nIndexCtx,nPid , lHsmBuffIn, lHsmBuffOut );
	if ( lRet == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   PinFromTpkToLmk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (lRet == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   PinFromTpkToLmk(SYSTEM_MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}


	/* - Extraction de la reponse --------------------------------------- */
	/*lRet = command_JD(lHsmBuffOut, lErrCode, pPinLmk); MCK300408*/
	lRet = command_JD(lHsmBuffOut, pPinLmk , lErrCode);
	if (memcmp(lErrCode, VERIFICATION_ERROR, 2) == 0)
	{
		trace_event("End   PinFromTpkToLmk(NOK)",PROCESSING);
		return(NOK);
	}
	if (memcmp(lErrCode, "00", 2) != 0)
	{
		trace_event("End   PinFromTpkToLmk(SYSTEM_MALFUNCTION)",PROCESSING);
		return(SYSTEM_MALFUNCTION);
	}


	if (debug_option == 1)
	{
		sprintf(sLine, "End   PinFromTpkToLmk(%.16s)", pPinLmk);
		trace_event(sLine, PROCESSING);
	}
	else
		trace_event("End   PinFromTpkToLmk(OK)", PROCESSING);
	return(OK);
}

/***********************************************************************
**  nom       : PinFromZpkToLmk                                       **
**  desc      : Traduction d'un PIN/ZPK en un PIN/LMK                 **
**  entree    : pZpkKey	-> La cl� TPK                          	**
**              pPinBlk	-> PIN Block sous Zpk			**
**              pPinBlkFmt    -> Format du PIN			**
**              pCardNbr      -> Num�ro de carte			**
**  sortie    : PIN Block en LMK                                      **
***********************************************************************/
int	PinFromZpkToLmk(
	int				nIndexCtx,
	unsigned char	pZpkKey		[],
	unsigned char	pPinBlk		[],
	unsigned char	pPinBlkFmt	[],
	unsigned char	pCardNbr	[],
	unsigned char	pPinLmk		[])
{


	unsigned char	lHsmBuffIn	[LG_MAX];
	unsigned char	lHsmBuffOut	[LG_MAX];
	unsigned char	lErrCode	[     3];
	char 		sLine		[MAX_LINE_TRC];
	pid_t    		nPid;
	int 			lRet;
	int				length;

	if (debug_option == 1)
	{
		sprintf(sLine, "Start PinFromZpkToLmk(%s,%.16s,%.2s,%.22s)",
			pZpkKey, pPinBlk, pPinBlkFmt, pCardNbr);
		trace_event(sLine, PROCESSING);
	}
	else
		trace_event("Start PinFromZpkToLmk()", PROCESSING);

	memset(lHsmBuffIn		, 0, sizeof(lHsmBuffIn)	);
	memset(lHsmBuffOut		, 0, sizeof(lHsmBuffOut));
	memset(lErrCode		, 0, sizeof(lErrCode)	);

	/* - Construction du tampon ------------------------------------------ */
	command_JE(lHsmBuffIn,pZpkKey, pPinBlk, pPinBlkFmt, pCardNbr);

	/* - Appel HSM ------------------------------------------------------- */
	/*    if ( CheckResource ( nIndexCtx, HSM_VERIF , &nPid ) != OK )*/
	if (CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid) != OK)
	{
		trace_event ( "Resource HSM Unavailable", ERROR );
		trace_event("End   PinFromZpkToLmk(HSM_UNAVIALABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	} 


	lRet = HsmQuery(nIndexCtx, nPid , lHsmBuffIn, lHsmBuffOut );
	if ( lRet == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   PinFromZpkToLmk(HSM_UNAVIALABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (lRet == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   PinFromZpkToLmk(SYSTEM_MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	trace_event("End  PinFromZpkToLmk(OK)",PROCESSING);


	/* - Extraction de la reponse --------------------------------------- */
	/*lRet = command_JF(lHsmBuffOut, lErrCode, pPinLmk);*/
	lRet = command_JF(lHsmBuffOut,pPinLmk, lErrCode);/*AER23022015 Fix PIN CHANGE ISSUE*/

	if (memcmp(lErrCode, VERIFICATION_ERROR, 2) == 0)
	{
		trace_event("End   PinFromZpkToLmk(NOK)",PROCESSING);
		return(NOK);
	}
	if (memcmp(lErrCode, "00", 2) != 0)
	{
		trace_event("End   PinFromZpkToLmk(SYSTEM_MALFUNCTION)",PROCESSING);
		return(SYSTEM_MALFUNCTION);
	}


	if (debug_option == 1)
	{
		sprintf(sLine, "End   PinFomZpkToLmk(%.16s)", pPinLmk);
		trace_event(sLine, PROCESSING);
	}
	else
		trace_event("End   PinFomZpkToLmk(OK)", PROCESSING);
	return(OK);
}

/***********************************************************************
**  nom       : PinFromTpkToZpk                                       **
**  desc      : Traduction d'un PIN/TPK en un PIN/ZPK                 **
**  entree    : pTpkKey	-> La cl� TPK                          	**
**		  pZpkKey	-> la cle ZPK				**
**              pPinTpk	-> PIN Block sous Zpk			**
**              pPinBlkFmt    -> Format du PIN			**
**              pCardNbr      -> Num�ro de carte			**
**  sortie    : PIN Block en Zpk                                      **
***********************************************************************/
int	PinFromTpkToZpk(
	int				nIndexCtx,
	unsigned char	pTpkKey		[],
	unsigned char	pZpkKey		[],
	unsigned char	pPinTpk		[],
	unsigned char	pPinBlkFmt	[],
	unsigned char	pCardNbr	[],
	unsigned char	pPinZpk		[])
{


	unsigned char	lHsmBuffIn	[LG_MAX];
	unsigned char	lHsmBuffOut	[LG_MAX];
	unsigned char	lErrCode	[     3];
	char 		sLine		[MAX_LINE_TRC];
	pid_t    		nPid;
	int 			lRet;
	int             length;

	if (debug_option == 1)
	{
		sprintf(sLine, "Start PinFromTpkToZpk(%.16s,%.16s,%.16s,%.2s,%.22s)",
			pTpkKey, pZpkKey, pPinTpk, pPinBlkFmt, pCardNbr);
		trace_event(sLine, PROCESSING);
	}
	else
		trace_event("Start PinFromTpkToZpk()", PROCESSING);

	memset(lHsmBuffIn		, 0, sizeof(lHsmBuffIn)	);
	memset(lHsmBuffOut		, 0, sizeof(lHsmBuffOut));
	memset(lErrCode		, 0, sizeof(lErrCode)	);

	/* - Construction du tampon ------------------------------------------ */
	command_CA(lHsmBuffIn, pTpkKey, pZpkKey, pPinTpk, pPinBlkFmt, pCardNbr);

	/* - Appel HSM ------------------------------------------------------- */
	/*    lRet = CheckResource ( nIndexCtx, HSM_VERIF , &nPid );*/
	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( lRet != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   PinFromTpkToZmk(HSM_UNAVIALABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}



	lRet = HsmQuery(nIndexCtx, nPid , lHsmBuffIn, lHsmBuffOut );
	if ( lRet == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   PinFromTpkToZmk(HSM_UNAVIALABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (lRet == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   PinFromTpkToZmk(SYSTEM_MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}


	/* - Extraction de la reponse --------------------------------------- */
	if ( command_CB(lHsmBuffOut, lErrCode, pPinZpk) != OK )
	{
		trace_event("End   PinFromZpkToZpk(NOK)",PROCESSING);
		return(NOK);
	}
	if (memcmp(lErrCode, "00", 2) != 0)
	{
		trace_event("End   PinFromZpkToZpk(SYSTEM_MALFUNCTION)",PROCESSING);
		return(SYSTEM_MALFUNCTION);
	}

	if (debug_option == 1)
	{
		sprintf(sLine, "End   PinFomTpkToZpk(%.16s)", pPinZpk);
		trace_event(sLine, PROCESSING);
	}
	else
		trace_event("End   PinFomTpkToZpk()", PROCESSING);
	return(OK);
} 


int PinOffsetToZpk(int nIndexCtx, char* sPan, char* sCardPinOffset, char* sPBFrmt, char* sZPK, char* sPVK, char* sPinPek )
{

	char			sHsmBuffIn	[BUF_LG_MAX];
	char			sHsmBuffOut	[BUF_LG_MAX];
	char			sErrCode	[     3];
	char 			sLine		[MAX_LINE_TRC];
	char    		sPvka       [SIZE_KEY_TDES + 1];
	char			sHsmResultCode[12];
	char			pin_lmk[16 + 1];
	char			sDecTable[32];
	pid_t    		nPid;
	int 			lRet;
	int             length;


	trace_event("Start PinOffsetToZpk()", PROCESSING);

	memset(sHsmBuffIn		, 0, sizeof(sHsmBuffIn)	);
	memset(sHsmBuffOut		, 0, sizeof(sHsmBuffOut));
	memset(sErrCode			, 0, sizeof(sErrCode)	);
	memset(sPvka			, 0, sizeof(sPvka)	);
	memset(sHsmResultCode	, 0, sizeof(sHsmResultCode)	);
	memset(pin_lmk	, 0, sizeof(pin_lmk)	);
	memset(sDecTable	, 0, sizeof(sDecTable)	);

	strncpy(sPvka, sPVK, sizeof(sPvka) - 1);

	GetHsmDecTable(sDecTable);

	command_EE(sHsmBuffIn, sPvka, sCardPinOffset, sPan, sDecTable);

	if (CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid) != OK) 
	{
		trace_event("Resource HSM PROD Unavailable", ERROR);
		return NOK;
	}

	if (HsmQuery(nIndexCtx, nPid, sHsmBuffIn, sHsmBuffOut) != OK) 
	{
		trace_event("HSM Communication Malfunction", ERROR);
		return NOK;
	}
	/*if (command_EF(sHsmBuffOut, pin_lmk, sHsmResultCode) != OK) */
	if (command_EF_35(sHsmBuffOut, pin_lmk, sHsmResultCode) != OK) /*PA16082023 to accept EF02 as success response*/
	{
		trace_event("HSM Processing Malfunction", ERROR);
		return NOK;
	}

	memset(sHsmBuffIn		, 0, sizeof(sHsmBuffIn)	);
	memset(sHsmBuffOut		, 0, sizeof(sHsmBuffOut));

	command_JG(sHsmBuffIn,sZPK,pin_lmk,sPBFrmt,sPan);

	if (HsmQuery(nIndexCtx, nPid, sHsmBuffIn, sHsmBuffOut) != OK) 
	{
		trace_event("HSM Communication Malfunction", ERROR);
		return NOK;
	}

	if (command_JH(sHsmBuffOut, sPinPek, sHsmResultCode) != OK) 
	{
		trace_event("HSM Processing Malfunction", ERROR);
		return NOK;
	}


	memset(pin_lmk	, 0, sizeof(pin_lmk)	);

	trace_event("End   PinOffsetToZpk(OK)", PROCESSING);
	return OK;

}



/*ZKO Start 04122020*/

int	export_key(
	int             nIndexCtx,
	char           *transKeyLmk,
	char           *keyLmk,
	char		   *keyType,
	char           *KeyZmk,
	char           *Check_Value_Out)
{


	pid_t           nPid;
	int             retour;
	char            BuffSnd[BUF_LG_MAX];
	char            BuffRcv[BUF_LG_MAX];
	char            ligne[MAX_LINE_TRC];


	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(Check_Value_Out, 0, 16);
	

	command_A8(BuffSnd, keyType, transKeyLmk, keyLmk);
		
	if(debug_option == 1)
	{
		sprintf(ligne, "BuffSnd[%.100s]", BuffSnd);
		trace_event(ligne, PROCESSING);
	}
		
	if (CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid) != OK) 
	{
		trace_event("Resource HSM VERIF Unavailable", ERROR);
		return (HSM_UNAVAILABLE);
	} 


	retour = HsmQuery(nIndexCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE) 
	{
		trace_event("HSM NOT AVAILABLE", ERROR);
		trace_event("End   export_key(NOT_AVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION) 
	{
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   export_key(MALFUNCTION)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	retour = command_A9 ( BuffRcv , KeyZmk, Check_Value_Out ); 
	if (retour != OK) 
	{
		trace_event("HSM Abnormal  DETECTED", ERROR);
		trace_event("End   export_key(CRYPTOGRAPHIC_ERROR)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}
		

		
}
/*ZKO End 04122020*/




