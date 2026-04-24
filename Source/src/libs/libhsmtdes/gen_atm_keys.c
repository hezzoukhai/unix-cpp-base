/*******************************************************************************/
/* (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/* hps_gen_key.c                                                                 */
/* */
/* Description                                                                 */
/* Key Generation Tools Management                                        */
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
int 
	GenerateTPKKey(
	int             nIndexCtx,
	char           *pTermNbr,
	char			*sAcqBank, 
	char           *pTpkLmk,
	char           *pTpkTmk,
	char		  *szCheckValue)
{
	char            lHSMBufferOut[BUF_LG_MAX];
	char            lHSMBufferIn[BUF_LG_MAX];
	int             length;

	char            lTmkLmk[256];
	char            lTmkZmk[256];
	char			sLMKId[8];
	int             lRet;
	pid_t           nPid;
	char            detail[MAX_LINE_TRC];


	sprintf(detail, "Start GenerateTPKKey(%.15s,%.6s)", pTermNbr,sAcqBank);
	trace_event(detail, PROCESSING);

	/*
	* - memset des buffers
	* ------------------------------------------------- 
	*/
	memset(lHSMBufferOut, 0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn, 0, sizeof(lHSMBufferIn));
	memset(lTmkLmk, 0, sizeof(lTmkLmk));
	memset(lTmkZmk, 0, sizeof(lTmkZmk));
	memset(sLMKId, 0, sizeof(sLMKId));

	/*
	* - recuperation de la cle TMK/LMK
	* ------------------------------------- 
	*/
	lRet = GetTMK(nIndexCtx, pTermNbr,sAcqBank, lTmkLmk, lTmkZmk);
	if (lRet != OK) 
	{
		trace_event("Err. In GetTMK()", ERROR);
		trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
		return (lRet);
	}
	
	if( lTmkLmk[0] != 'S' )
	{
		trace_event("Invalid TMK Key scheme", ERROR);
		trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,lTmkLmk + 1 + 14,2);

	/*
	* - preparation des buffers pour HSM
	* ----------------------------------- 
	*/
	/*lRet = command_HC(lHSMBufferIn, lTmkLmk);*/
	/*lRet = command_A0(lHSMBufferIn,'1',THALES_KEY_TP_TPK,'1',lTmkLmk);*/

	lRet = command_A0(	lHSMBufferIn,
						'1',
						"FFF",
						THALES_KU_TPK,
						THALES_ALGO_DES_2,
						THALES_MU_NO_RESTRICT,
						THALES_VN_00,
						THALES_EX_S,
						sLMKId,
						'1',
						lTmkLmk,
						KEY_LEN_DOUBLE,
						"X",/*AHA20220928 PLUTONL-4980*/
						"" /*AHA20220928 PLUTONL-4980*/
						);
	if (lRet != OK) 
	{
		trace_event("Err. In command_A0()", ERROR);
		trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
		return (lRet);
	}
	/*
	* - demande de generation de la TPK/TMK
	* -------------------------------- 
	*/
	length = strlen((char *) lHSMBufferIn);
	/*lRet = CheckResource(nIndexCtx, HSM_VERIF, &nPid);*/
	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((lRet != OK)) 
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)", PROCESSING);
		trace_event("End   GenerateTPKKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	} 

	lRet = HsmQuery(nIndexCtx, nPid, lHSMBufferIn, lHSMBufferOut);
	if (lRet == NOT_AVAILABLE) 
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateTPKKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (lRet == HSM_MALFUNCTION) {
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   GenerateTPKKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}
	/*
	* - recuperation des donnes du buffers HSM
	* ----------------------------- 
	*/

	/*lRet = command_HD(lHSMBufferOut, pTpkTmk, pTpkLmk, lErrCode);*/
	lRet = command_A1(lHSMBufferOut, '1',pTpkLmk,pTpkTmk, szCheckValue);
	if (lRet != OK) {
		trace_event("Err. In command_A1()", ERROR);
		trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
		return (lRet);
	}


	memset(lHSMBufferOut, 0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn, 0, sizeof(lHSMBufferIn));
	memset(lTmkLmk, 0, sizeof(lTmkLmk));
	memset(lTmkZmk, 0, sizeof(lTmkZmk));

	if (debug_option == 1)
		sprintf(detail, "End   GenerateTPKKey(%.32s,%.32s,%s)", pTpkLmk, pTpkTmk,szCheckValue);
	else
		sprintf(detail, "End   GenerateTPKKey(%lu,%s)", strlen(pTpkTmk),szCheckValue);

	trace_event(detail, PROCESSING);
	return (OK);
}
/*Renewal Keys MCK28012014*/
int 
	GenerateTMKKey(
	int             nIndexCtx,
	char           *pTermNbr,
	char			*sAcqBank, 
	char           *pTmkLmk,
	char           *pTmkTmk,
	char		  *szCheckValue)
{
	char            lHSMBufferOut[BUF_LG_MAX];
	char            lHSMBufferIn[BUF_LG_MAX];
	int             length;

	char            lTmkLmk[256];
	char            lTmkTmk[256];
	char			sLMKId[8];
	int             lRet;
	pid_t           nPid;
	char            detail[MAX_LINE_TRC];


	sprintf(detail, "Start GenerateTMKKey(%.15s,%.6s)", pTermNbr,sAcqBank);
	trace_event(detail, PROCESSING);

	/*
	* - memset des buffers
	* ------------------------------------------------- 
	*/
	memset(lHSMBufferOut, 0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn, 0, sizeof(lHSMBufferIn));
	memset(lTmkLmk, 0, sizeof(lTmkLmk));
	memset(lTmkTmk, 0, sizeof(lTmkTmk));
	memset(sLMKId, 0, sizeof(sLMKId));

	/*
	* - recuperation de la cle TMK/LMK
	* ------------------------------------- 
	*/
	lRet = GetTMK(nIndexCtx, pTermNbr,sAcqBank, lTmkLmk, lTmkTmk);
	if (lRet != OK) {
		trace_event("Err. In GetTMK()", ERROR);
		trace_event("End   GenerateTMKKey(NOK)", PROCESSING);
		return (lRet);
	}

	if( lTmkLmk[0] != 'S' )
	{
		trace_event("Invalid TMK Key scheme", ERROR);
		trace_event("End   GenerateTMKKey(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,lTmkLmk + 1 + 14,2);

	/*
	* - preparation des buffers pour HSM
	* ----------------------------------- 
	*/
	/*lRet = command_HC(lHSMBufferIn, lTmkLmk);*/
	/*lRet = command_A0(lHSMBufferIn,'1',THALES_KEY_TP_TMK,'1',lTmkLmk);*/
	lRet = command_A0(	lHSMBufferIn,
						'1',
						"FFF",
						THALES_KU_TMK,
						THALES_ALGO_DES_2,
						THALES_MU_NO_RESTRICT,
						THALES_VN_00,
						THALES_EX_S,
						sLMKId,
						'1',
						lTmkLmk,
						KEY_LEN_DOUBLE,
						"X",/*AHA20220928 PLUTONL-4980*/
						"" /*AHA20220928 PLUTONL-4980*/
						);
	if (lRet != OK) {
		trace_event("Err. In command_HC()", ERROR);
		trace_event("End   GenerateTMKKey(NOK)", PROCESSING);
		return (lRet);
	}
	/*
	* - demande de generation de la TMK/TMK
	* -------------------------------- 
	*/
	length = strlen((char *) lHSMBufferIn);
	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((lRet != OK)) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)", PROCESSING);
		trace_event("End   GenerateTMKKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	} 

	lRet = HsmQuery(nIndexCtx, nPid, lHSMBufferIn, lHSMBufferOut);
	if (lRet == NOT_AVAILABLE) 
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateTMKKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (lRet == HSM_MALFUNCTION) {
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   GenerateTMKKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	/*lRet = command_HD(lHSMBufferOut, pTmkTmk, pTmkLmk, lErrCode);*/
	lRet = command_A1(lHSMBufferOut, '1',pTmkLmk,pTmkTmk, szCheckValue);
	if (lRet != OK) {
		trace_event("Err. In command_HC()", ERROR);
		trace_event("End   GenerateTMKKey(NOK)", PROCESSING);
		return (lRet);
	}

	/*
	* - memset des buffers
	* ------------------------------------------------- 
	*/
	memset(lHSMBufferOut, 0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn, 0, sizeof(lHSMBufferIn));
	memset(lTmkLmk, 0, sizeof(lTmkLmk));
	memset(lTmkTmk, 0, sizeof(lTmkTmk));

	if (debug_option == 1)
		sprintf(detail, "End   GenerateTMKKey(%.32s,%.32s,%s)", pTmkLmk, pTmkTmk,szCheckValue);
	else
		sprintf(detail, "End   GenerateTMKKey(%lu;%s)", strlen(pTmkTmk),szCheckValue);
	trace_event(detail, PROCESSING);
	return (OK);
}

int 
	GenerateMACKey(
	int             nIndexCtx,
	char           *pTermNbr,
	char			*sAcqBank, 
	char           *pMacLmk,
	char           *pMacTmk,
	char		  *szCheckValue)
{
	char            lHSMBufferOut[BUF_LG_MAX];
	char            lHSMBufferIn[BUF_LG_MAX];
	int             length;

	char            lTmkLmk[256];
	char            lTmkTmk[256];
	char			sLMKId[8];
	int             lRet;
	pid_t           nPid;
	char            detail[MAX_LINE_TRC];


	sprintf(detail, "Start GenerateMACKey(%.15s,%.6s)", pTermNbr,sAcqBank);
	trace_event(detail, PROCESSING);

	/*
	* - memset des buffers
	* ------------------------------------------------- 
	*/
	memset(lHSMBufferOut, 0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn, 0, sizeof(lHSMBufferIn));
	memset(lTmkLmk, 0, sizeof(lTmkLmk));
	memset(lTmkTmk, 0, sizeof(lTmkTmk));
	memset(sLMKId, 0, sizeof(sLMKId));

	/*
	* - recuperation de la cle TMK/LMK
	* ------------------------------------- 
	*/
	lRet = GetTMK(nIndexCtx, pTermNbr,sAcqBank, lTmkLmk, lTmkTmk);
	if (lRet != OK) 
	{
		trace_event("Err. In GetTMK()", ERROR);
		trace_event("End   GenerateMACKey(NOK)", PROCESSING);
		return (lRet);
	}

	if( lTmkLmk[0] != 'S' )
	{
		trace_event("Invalid TMK Key scheme", ERROR);
		trace_event("End   GenerateMACKey(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,lTmkLmk + 1 + 14,2);

	/*
	* - preparation des buffers pour HSM
	* ----------------------------------- 
	*/
	/*lRet = command_HA_TDES(lHSMBufferIn, lMacLmk);*/
	/*lRet = command_A0(lHSMBufferIn,'1',THALES_KEY_TP_TAK,'1',lTmkLmk);*/
	lRet = command_A0(	lHSMBufferIn,
						'1',
						"FFF",
						ISO9797_1_ALGO3_MAC_KEY,
						THALES_ALGO_DES_2,
						THALES_MU_NO_RESTRICT,
						THALES_VN_00,
						THALES_EX_S,
						sLMKId,
						'1',
						lTmkLmk,
						KEY_LEN_DOUBLE,
						"X",/*AHA20220928 PLUTONL-4980*/
						"" /*AHA20220928 PLUTONL-4980*/
						);
	if (lRet != OK) {
		trace_event("Err. In command_HC()", ERROR);
		trace_event("End   GenerateMACKey(NOK)", PROCESSING);
		return (lRet);
	}
	/*
	* - demande de generation de la TMK/TMK
	* -------------------------------- 
	*/
	length = strlen((char *) lHSMBufferIn);
	/*lRet = CheckResource(nIndexCtx, HSM_VERIF, &nPid);*/
	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((lRet != OK)) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)", PROCESSING);
		trace_event("End   GenerateMACKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	lRet = HsmQuery(nIndexCtx, nPid, lHSMBufferIn, lHSMBufferOut);
	if (lRet == NOT_AVAILABLE) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateMACKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (lRet == HSM_MALFUNCTION) {
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   GenerateMACKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	/*lRet = command_HB(lHSMBufferOut, pMacTmk, pMacLmk, lErrCode);*/
	lRet = command_A1(lHSMBufferOut, '1',pMacLmk,pMacTmk, szCheckValue);
	if (lRet != OK) {
		trace_event("Err. In command_HC()", ERROR);
		trace_event("End   GenerateMACKey(NOK)", PROCESSING);
		return (lRet);
	}


	/*
	* - memset des buffers
	* ------------------------------------------------- 
	*/
	memset(lHSMBufferOut, 0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn, 0, sizeof(lHSMBufferIn));
	memset(lTmkLmk, 0, sizeof(lTmkLmk));
	memset(lTmkTmk, 0, sizeof(lTmkTmk));


	if (debug_option == 1)
		sprintf(detail, "End   GenerateMACKey(%.32s,%.32s,%s)", pMacLmk, pMacTmk,szCheckValue);
	else
		sprintf(detail, "End   GenerateMACKey(%lu,%s)",strlen( pMacTmk),szCheckValue);
	trace_event(detail, PROCESSING);
	return (OK);
}


