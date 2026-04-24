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

int 
	GenerateTermPinKey(
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


	sprintf(detail, "Start GenerateTermPinKey(%.15s,%.6s)", pTermNbr,sAcqBank);
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
	lRet = GetTermMasterKey(nIndexCtx, pTermNbr,sAcqBank, lTmkLmk, lTmkZmk);
	if (lRet != OK) 
	{
		trace_event("Err. In GetTermMasterKey()", ERROR);
		trace_event("End   GenerateTermPinKey(NOK)", PROCESSING);
		return (lRet);
	}

	if( lTmkLmk[0] != 'S' )
	{
		trace_event("Invalid TMK Key scheme", ERROR);
		trace_event("End   GenerateTermPinKey(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,lTmkLmk + 1 + 14,2);

	/*
	* - preparation des buffers pour HSM
	* ----------------------------------- 
	*/
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
		trace_event("End   GenerateTermPinKey(NOK)", PROCESSING);
		return (lRet);
	}
	/*
	* - demande de generation de la TPK/TMK
	* -------------------------------- 
	*/
	length = strlen((char *) lHSMBufferIn);
	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((lRet != OK)) 
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateTermPinKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	} 

	lRet = HsmQuery(nIndexCtx, nPid, lHSMBufferIn, lHSMBufferOut);
	if (lRet == NOT_AVAILABLE) 
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateTermPinKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (lRet == HSM_MALFUNCTION) {
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   GenerateTermPinKey(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}
	/*
	* - recuperation des donnes du buffers HSM
	* ----------------------------- 
	*/

	lRet = command_A1(lHSMBufferOut, '1',pTpkLmk,pTpkTmk, szCheckValue);
	if (lRet != OK) {
		trace_event("Err. In command_A1()", ERROR);
		trace_event("End   GenerateTermPinKey(NOK)", PROCESSING);
		return (lRet);
	}


	memset(lHSMBufferOut, 0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn, 0, sizeof(lHSMBufferIn));
	memset(lTmkLmk, 0, sizeof(lTmkLmk));
	memset(lTmkZmk, 0, sizeof(lTmkZmk));

	if (debug_option == 1)
		sprintf(detail, "End   GenerateTermPinKey(%.32s,%.32s,%s)", pTpkLmk, pTpkTmk,szCheckValue);
	else
		sprintf(detail, "End   GenerateTermPinKey(%lu,%s)", strlen(pTpkTmk),szCheckValue);

	trace_event(detail, PROCESSING);
	return (OK);
}

/*ZKO13092018: rename function from double dukpt to TranslatePinFromDukptToZPK and add outletNbr for getting bank*/
int  TranslatePinFromDukptToZPK (int nIndiceCtx, 
	char *sTerminal,
	char *sOutlet, 
	char *sResourceId, 
	char *sPan, 
	char *sPin, 
	char *KSN, 
	char *BDK, 
	char *DKSN, 
	char *sTranskey, 
	char *sNewPinLength, 
	char *sNewPinBlock, 
	char *sNewFormatCode )
{
	int     retour;
	char    sZPK       [256];
	char    BuffSnd    [512];
	char    BuffRcv    [512];
	char    sLine[MAX_LINE_TRC];
	char    sHsmResultCode[5];
	pid_t   nPid;

	memset(sHsmResultCode , 0 , 5 );
	memset(BuffSnd, 0, sizeof(BuffSnd) );
	memset(BuffRcv, 0, sizeof(BuffRcv) );


	memset(sZPK , 0 , sizeof(sZPK) );
	memset(sHsmResultCode , 0 , 5 );

	if (debug_option == 1)
		sprintf(sLine, "Start TranslatePinFromDukptToZPK(%.20s,%.15s,%.16s,%.32s)",BDK , sTerminal, KSN, DKSN);
	else
		sprintf(sLine, "Start TranslatePinFromDukptToZPK(%.15s)",sTerminal);

	trace_event(sLine, PROCESSING);


	/** Premiere derivation **/

	/* Retrieving the ZPK */
	if ( GetTerminalZPK(nIndiceCtx, sTerminal , sOutlet, sResourceId, sZPK) != OK )
	{
		trace_event("End   GetTerminalZPK(NOK)Failed",PROCESSING);
		return(NOK);
	}


	memcpy(sTranskey, sZPK, strlen(sZPK));

	command_G0(BuffSnd, BDK, sZPK,DKSN, KSN, sPin, "01","01",sPan);   




	/** Retrieving the HSM interface PID **/
	if ( CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid) != OK )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   TranslatePinFromDukptToZPK(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}


	retour = HsmQueryBin(nIndiceCtx,nPid,BuffSnd, strlen(BuffSnd), BuffRcv);
	if (retour == NOT_AVAILABLE)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM NOT AVAILABLE", ERROR);
		trace_event("End   TranslatePinFromDukptToZPK(NOT_AVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{		memcpy ( sHsmResultCode , "UNVL", 4 );
	trace_event("HSM MALFUNCTION DETECTED", ERROR);
	trace_event("End   TranslatePinFromDukptToZPK(MALFUNCTION)",PROCESSING);
	return(HSM_UNAVAILABLE);
	}

	retour = command_G1 ( BuffRcv , sHsmResultCode, sNewPinLength, sNewPinBlock, sNewFormatCode );
	if ( retour != OK )
	{
		trace_event("HSM Abnormal  DETECTED", ERROR);
		trace_event("End   TranslatePinFromDukptToZPK(CRYPTOGRAPHIC_ERROR)",PROCESSING);
		return(CRYPTOGRAPHIC_ERROR);
	}

	trace_event("End   TranslatePinFromDukptToZPK(OK)", PROCESSING);
	return(OK);
}


/*ZKO:EndPROD00059856*/