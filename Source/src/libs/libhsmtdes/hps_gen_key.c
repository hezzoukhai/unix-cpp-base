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

/*
int	GeneratePosIsoBDKKey(int nIndexCtx,char *pResourceId, char *pTermNbr, char *pBdkLmk, char *pBdkZmk, char *pCheckValue)
{
	char	lHSMBufferOut	[LG_MAX];
	char	lHSMBufferIn	[LG_MAX];


	char	sZmK		[ 256];

	int		lRet;
	pid_t   nPid;
	char    detail		[MAX_LINE_TRC];
	char    lErrCode		[BUF_LG_MAX];

	sprintf(detail, "Start GeneratePosIsoBDKKey(%.15s)", pTermNbr);
	trace_event(detail, PROCESSING);

	
	memset(lHSMBufferOut,	0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,	0, sizeof(lHSMBufferIn)	);
	memset(sZmK,			0, sizeof(sZmK)	);

	memset(lErrCode,		0, sizeof(lErrCode)	);

	
	lRet = GetZMK ( nIndexCtx , pResourceId, sZmK);
	if(lRet != OK)
	{
		trace_event("Err. In GetZMK()", ERROR);
		trace_event("End   GeneratePosIsoBDKKey(NOK)", PROCESSING);
		return (lRet);
	}	

	
	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( lRet != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End    generation de la BDK/LMK(HSM_UNAVAILABLE)",PROCESSING);
		trace_event("End   GeneratePosIsoBDKKey(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}


	
	lRet = command_BI(lHSMBufferIn);
	if(lRet != OK)
	{
		trace_event("Err. In command_BI()", ERROR);
		trace_event("End   GeneratePosIsoBDKKey(NOK)", PROCESSING);
		return (lRet);
	}

	


	lRet = HsmQuery(nIndexCtx, nPid , lHSMBufferIn, lHSMBufferOut );
	if ( lRet == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GeneratePosIsoBDKKey(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (lRet == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   GeneratePosIsoBDKKey(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	

	lRet = command_BJ(lHSMBufferOut, lErrCode, pBdkLmk);
	if(lRet != OK)
	{
		trace_event("Err. In command_BJ()", ERROR);
		trace_event("End   GeneratePosIsoBDKKey(NOK)", PROCESSING);
		return (lRet);
	}

	
	
	lRet = command_DY(lHSMBufferIn, sZmK, pBdkLmk );
	if(lRet != OK)
	{
		trace_event("Err. In command_DY()", ERROR);
		trace_event("End   GeneratePosIsoBDKKey(NOK)", PROCESSING);
		return (lRet);
	}

	


	lRet = HsmQuery(nIndexCtx, nPid , lHSMBufferIn, lHSMBufferOut );
	if ( lRet == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GeneratePosIsoBDKKey(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (lRet == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   GeneratePosIsoBDKKey(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	

	lRet = command_DZ(lHSMBufferOut, pBdkZmk,pCheckValue, lErrCode );
	if(lRet != OK)
	{
		trace_event("Err. In command_DZ()", ERROR);
		trace_event("End   GeneratePosIsoBDKKey(NOK)", PROCESSING);
		return (lRet);
	}


	
	
	memset(lHSMBufferOut,	0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,	0, sizeof(lHSMBufferIn)	);
	memset(sZmK,			0, sizeof(sZmK)	);

	
	if (debug_option == 1)
	{
		sprintf(detail, "End   GeneratePosIsoBDKKey(%.16s,%.16s)", pBdkLmk, pBdkZmk);
		trace_event(detail, PROCESSING);
	}
	else
		trace_event("End   GeneratePosIsoBDKKey()", PROCESSING);
	return (OK);
}
*/
/* PROD00065699: use A0 instead of IA*/
int GenerateZPK(		int 		nIndexCtx,
	char		sDesOption,
	char*		szZmkLmk,
	char*		szZpkLmk,
	char*		szZpkZmk,
	char*		szCheckValue,
	char*       keySchemeZmk, /* AHA20220928 PLUTONL-4980*/
	char*       KeyBlockVersionId /* AHA20220927 PLUTONL-4980*/
	)
{
	char    lHSMBufferOut   [BUF_LG_MAX];
	char    lHSMBufferIn    [BUF_LG_MAX];
	char	sLMKId[8];
	char    lErrCode        [256];
	int		result;
	pid_t   nPid;

	trace_event("Start GenerateZPK()",PROCESSING);

	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	memset(lErrCode,			0, sizeof(lErrCode));
	memset(sLMKId, 0, sizeof(sLMKId));

	/*if(sDesOption == DES_OPTION_TIRPLE)
	{
		result = command_IA_TDES(lHSMBufferIn,szZmkLmk);
		if(result != OK)
		{
			trace_event("End   GenerateZPK(NOK): command_IA_TDES() failed", ERROR);
			return (result);
		}
	}

	else
	{
		result = command_IA(lHSMBufferIn,szZmkLmk);
		if(result != OK)
		{
			trace_event("End   GenerateZPK(NOK): command_IA_TDES() failed", ERROR);
			return (result);
		}
	}
	*/

	if( szZmkLmk[0] != 'S' )
	{
		trace_event("Invalid ZMK Key scheme", ERROR);
		trace_event("End   GenerateZPK(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,szZmkLmk + 1 + 14,2);


	/*result = command_A0(lHSMBufferIn,'1',THALES_KEY_TP_ZPK,'0',szZmkLmk ,KEY_LEN_DOUBLE);*/
	/*result = command_A0(	lHSMBufferIn,
						'1',
						"FFF",
						THALES_KU_ZPK,
						THALES_ALGO_DES_2,
						THALES_MU_NO_RESTRICT,
						THALES_VN_00,
						THALES_EX_S,
						sLMKId,
						'0',
						szZmkLmk,
						KEY_LEN_DOUBLE);*/
	result = command_A0(	lHSMBufferIn,
						'1',
						"FFF",
						THALES_KU_ZPK,
						THALES_ALGO_DES_2,
						THALES_MU_NO_RESTRICT,
						THALES_VN_00,
						THALES_EX_S,
						sLMKId,
						'0',
						szZmkLmk,
						KEY_LEN_DOUBLE,
						keySchemeZmk,/* AHA20220927 PLUTONL-4980*/
						KeyBlockVersionId/* AHA20220927 PLUTONL-4980*/
						);

	if (result != OK)
	{
		trace_event("Err. In command_A0()", ERROR);
		trace_event("End   GenerateZPK(NOK)", PROCESSING);
		return (result);
	}


	result = CheckHsmResource ( nIndexCtx, HSM_VERIF_MODE , &nPid );
	if ( ( result != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE(CheckResource)", ERROR);
		trace_event("End   GenerateZPK(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	result = HsmQuery( nIndexCtx,  nPid , lHSMBufferIn, lHSMBufferOut );
	if ( result == NOT_AVAILABLE )
	{
		trace_event("HSM FOR GENERATION   NOT AVAILABLE(HsmQuery)", ERROR);
		trace_event("End   GenerateZPK(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}
	else if (result == HSM_MALFUNCTION)
	{
		trace_event("HSM GENERATION  MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateZPK(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	/*result = command_IB(lHSMBufferOut, szZpkZmk,szZpkLmk, szCheckValue, lErrCode);*/
	result = command_A1(lHSMBufferOut, '1',szZpkLmk,szZpkZmk, szCheckValue);
	if(result != OK)
	{
		trace_event("Err. In command_IB()", ERROR);
		trace_event("End   GenerateZPK(NOK)", PROCESSING);
		return (result);
	}

	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	trace_event("End GenerateZPK(OK)",PROCESSING);
	return OK;
}


int GenerateResourceZPK(	int 		nIndexCtx,
	char		sDesOption,/*ICH20161227*/
	char*		sResourceId,
	char*		szZpkLmk,
	char*		szZpkZmk,
	char*		szCheckValue,
	char*      keySchemeZmk, /* AHA20220928 PLUTONL-4980*/
	char*       KeyBlockVersionId /* AHA20220927 PLUTONL-4980*/
	)
{

	char    szZmkLmk        [256];
	int		result;


	trace_event("Start GenerateResourceZPK()",PROCESSING);

	memset(szZmkLmk,			0, sizeof(szZmkLmk));

	result = GetZMK(nIndexCtx,sResourceId , szZmkLmk);
	if(result != OK)
	{
		trace_event("Err. In GetZMK()", ERROR);
		trace_event("End   GenerateResourceZPK(NOK)", PROCESSING);
		return (result);
	}
	/*start  AHA20220929 PLUTONL-4980*/
	result = GenerateZPK(	nIndexCtx,
		sDesOption,/*ICH20161227*/
		szZmkLmk,
		szZpkLmk,
		szZpkZmk,
		szCheckValue,
		keySchemeZmk,
		KeyBlockVersionId
		);
	/*end  AHA20220929 PLUTONL-4980*/
	if(result != OK)
	{
		trace_event("Err. In GenerateZPK()", ERROR);
		trace_event("End   GenerateResourceZPK(NOK)", PROCESSING);
		return (result);
	}

	trace_event("End   GenerateResourceZPK(OK)",PROCESSING);
	return OK;
}


int     GenerateResourceTAK(
	int     nIndexCtx,
	char	sDesOption,/*ICH20161227*/
	char    *pResourceId,
	char    *pTakLmk,
	char    *pTakZmk,
	char    *pCheckValue)
{
	char    lHSMBufferOut   [BUF_LG_MAX];
	char    lHSMBufferIn    [BUF_LG_MAX];
	char	sLMKId[8];
	char 	sKeyLen	;
	char    lZmkLmk         [ 256 + 1];
	int     lRet;
	pid_t   nPid;
	char    sLine          [MAX_LINE_TRC];
	char    lErrCode        [BUF_LG_MAX];

	sprintf(sLine, "Start GenerateResourceTAK(%.6s)", pResourceId);
	trace_event(sLine, PROCESSING);

	/* - Memset des buffers ------------------------------------------------- */
	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	memset(lZmkLmk,             0, sizeof(lZmkLmk)      );
	memset(lErrCode,            0, sizeof(lErrCode)     );
	memset(sLMKId, 0, sizeof(sLMKId));


	/* - Recuperation de la cle ZMK/LMK Pour la translation du TAK ---------- */
	lRet = GetZMK(nIndexCtx,pResourceId , lZmkLmk);
	if(lRet != OK)
	{
		trace_event("Err. In GetZMK()", ERROR);
		trace_event("End   GenerateResourceTAK(NOK)", PROCESSING);
		return (lRet);
	}

	if( lZmkLmk[0] != 'S' )
	{
		trace_event("Invalid ZMK Key scheme", ERROR);
		trace_event("End   GenerateZPK(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,lZmkLmk + 1 + 14,2);

	/* - Generation TAK  /ZMK                ----------------------------------- */

	sKeyLen =  KEY_LEN_DOUBLE;

	if( sDesOption == DES_OPTION_SIMPLE)
	{
		sKeyLen =  KEY_LEN_SIMPLE ;
	}

	/*lRet = command_A0(lHSMBufferIn,'1',THALES_KEY_TP_TAK,'0',lZmkLmk ,sKeyLen);*/
	lRet = command_A0(	lHSMBufferIn,
						'1',
						"FFF",
						ISO9797_1_ALGO3_MAC_KEY,
						THALES_ALGO_DES_2,
						THALES_MU_NO_RESTRICT,
						THALES_VN_00,
						THALES_EX_S,
						sLMKId,
						'0',
						lZmkLmk,
						KEY_LEN_DOUBLE,
						"X",/*AHA20220928 PLUTONL-4980*/
						"" /*AHA20220928 PLUTONL-4980*/
						);
	if (lRet != OK) {
		trace_event("Err. In command_A0()", ERROR);
		trace_event("End   GenerateResourceTAK(NOK)", PROCESSING);
		return (lRet);
	}

	/* - Demande de generation de la TAK/ZMK -------------------------------- */

	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( lRet != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE(CheckResource)", ERROR);
		trace_event("End   GenerateResourceTAK(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	lRet = HsmQuery( nIndexCtx,  nPid , lHSMBufferIn, lHSMBufferOut );
	if ( lRet == NOT_AVAILABLE )
	{
		trace_event("HSM FOR GENERATION   NOT AVAILABLE(HsmQuery)", ERROR);
		trace_event("End   GenerateResourceTAK(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (lRet == HSM_MALFUNCTION)
	{
		trace_event("HSM GENERATION  MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateResourceTAK(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	lRet = command_A1(lHSMBufferOut, '1',pTakLmk,pTakZmk, pCheckValue);
	if (lRet != OK) {
		trace_event("Err. In command_A1()", ERROR);
		trace_event("End   GenerateMACKey(NOK)", PROCESSING);
		return (lRet);
	}


	memset(lHSMBufferOut, 0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn, 0, sizeof(lHSMBufferIn));
	memset(lZmkLmk, 0, sizeof(lZmkLmk));


	if (debug_option == 1)
		sprintf(sLine, "End   GenerateResourceTAK(%.32s,%.32s,%s)", pTakLmk, pTakZmk,pCheckValue);
	else
		sprintf(sLine, "End   GenerateResourceTAK(%lu,%s)",strlen( pTakZmk),pCheckValue);
	trace_event(sLine, PROCESSING);

	return (OK);
}


int GenerateKeyKS(		int 		nIndexCtx,
	char* 		sResourceId,
	char*		szZmkLmk,
	char*		szZakLmk,
	char*		szZakZmk,
	char*		szCheckValue
	)
{
	char    lHSMBufferOut   [BUF_LG_MAX];
	char    lHSMBufferIn    [BUF_LG_MAX];
	char    lErrCode        [256];
	char	sLMKId[8];
	int		result;
	pid_t   nPid;

	trace_event("Start GenerateKeyKS()",PROCESSING);

	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	memset(lErrCode,			0, sizeof(lErrCode));
	memset(sLMKId, 0, sizeof(sLMKId));

	if( szZmkLmk[0] != 'S' )
	{
		trace_event("Invalid ZMK Key scheme", ERROR);
		trace_event("End   GenerateKeyKS(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,szZmkLmk + 1 + 14,2);

	/*result = command_FI(lHSMBufferIn,szZmkLmk,HSM_KEY_FLAG_ZAK);*/
	/*result = command_A0(lHSMBufferIn,'1',THALES_KEY_TP_ZAK,'0',szZmkLmk);*/
	result = command_A0(	lHSMBufferIn,
						'1',
						"FFF",
						ISO9797_1_ALGO3_MAC_KEY,
						THALES_ALGO_DES_2,
						THALES_MU_NO_RESTRICT,
						THALES_VN_00,
						THALES_EX_S,
						sLMKId,
						'0',
						szZmkLmk,
						KEY_LEN_DOUBLE,
						"X",/*AHA20220928 PLUTONL-4980*/
						"" /*AHA20220928 PLUTONL-4980*/
						);
	if(result != OK)
	{
		trace_event("End   GenerateKeyKS(NOK): command_FI() failed", ERROR);
		return (result);
	}

	result = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( result != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE(CheckResource)", ERROR);
		trace_event("End   GenerateKeyKS(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}


	result = HsmQuery( nIndexCtx,  nPid , lHSMBufferIn, lHSMBufferOut );
	if ( result == NOT_AVAILABLE )
	{
		trace_event("HSM FOR GENERATION   NOT AVAILABLE(HsmQuery)", ERROR);
		trace_event("End   GenerateKeyKS(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}
	else if (result == HSM_MALFUNCTION)
	{
		trace_event("HSM GENERATION  MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateKeyKS(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	/*result = command_FJ(lHSMBufferOut, szZakZmk,szZakLmk, szCheckValue, lErrCode);*/
	result = command_A1(lHSMBufferOut, '1',szZakLmk,szZakZmk, szCheckValue);
	if(result != OK)
	{
		trace_event("Err. In command_FJ()", ERROR);
		trace_event("End   GenerateKeyKS(NOK)", PROCESSING);
		return (result);
	}

	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	trace_message(PROCESSING,"KCV: [%s]",szCheckValue);
	trace_event("End GenerateKeyKS(OK)",PROCESSING);
	return OK;
}
/*****************************************************************************/
int GenerateKeyKT(		int 		nIndexCtx,
	char* 		sResourceId,
	char*		szZmkLmk,
	char*		szKtkLmk,
	char*		szKtkZmk,
	char*		szCheckValue
	)
{
	char    lHSMBufferOut   [BUF_LG_MAX];
	char    lHSMBufferIn    [BUF_LG_MAX];
	char    lErrCode        [256];
	char	sLMKId[8];
	int		result;
	pid_t   nPid;

	trace_event("Start GenerateKeyKT()",PROCESSING);

	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	memset(lErrCode,			0, sizeof(lErrCode));
	memset(sLMKId, 0, sizeof(sLMKId));

	if( szZmkLmk[0] != 'S' )
	{
		trace_event("Invalid ZMK Key scheme", ERROR);
		trace_event("End   GenerateKeyKT(NOK)", PROCESSING);
		return (NOK);
	}

	memcpy(sLMKId,szZmkLmk + 1 + 14,2);

	/*result = command_A0(lHSMBufferIn,'1',"700",'0',szZmkLmk,KEY_LEN_DOUBLE);*/
	result = command_A0(	lHSMBufferIn,
						'1',
						"FFF",
						THALES_KU_TMK,
						THALES_ALGO_DES_2,
						THALES_MU_NO_RESTRICT,
						THALES_VN_00,
						THALES_EX_S,
						sLMKId,
						'0',
						szZmkLmk,
						KEY_LEN_DOUBLE,
						"X",/*AHA20220928 PLUTONL-4980*/
						"" /*AHA20220928 PLUTONL-4980*/
						);
	if(result != OK)
	{
		trace_event("End   GenerateKeyKT(NOK): command_A0() failed", ERROR);
		return (result);
	}

	result = CheckHsmResource ( nIndexCtx, HSM_VERIF_MODE , &nPid );
	if ( ( result != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE(CheckResource)", ERROR);
		trace_event("End   GenerateKeyKT(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}


	result = HsmQuery( nIndexCtx,  nPid , lHSMBufferIn, lHSMBufferOut );
	if ( result == NOT_AVAILABLE )
	{
		trace_event("HSM FOR GENERATION   NOT AVAILABLE(HsmQuery)", ERROR);
		trace_event("End   GenerateKeyKT(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}
	else if (result == HSM_MALFUNCTION)
	{
		trace_event("HSM GENERATION  MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateKeyKT(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	result = command_A1(lHSMBufferOut, '1',szKtkLmk,szKtkZmk, szCheckValue);
	if(result != OK)
	{
		trace_event("Err. In command_A1()", ERROR);
		trace_event("End   GenerateKeyKT(NOK)", PROCESSING);
		return (result);
	}

	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	trace_message(PROCESSING,"KCV: [%s]",szCheckValue);
	trace_event("End GenerateKeyKT(OK)",PROCESSING);
	return OK;
}
/*****************************************************************************/
int GenerateKeyKC(		int 		nIndexCtx,
						char* 		sResourceId,
						char*		szZmkLmk,
						char*		szZpkLmk,
						char*		szZpkZmk,
						char*		szCheckValue
						)
{

	return GenerateZPK(	nIndexCtx,
		DES_OPTION_TIRPLE,
		szZmkLmk,
		szZpkLmk,
		szZpkZmk,
		szCheckValue,
		'X',/*AHA20220930 PLUTONL-4980*/
		" " /*AHA20220930 PLUTONL-4980*/
		);

}

int GenerateKeyCheckValue(int nIndexCtx,char *pKeyLmk,char *pCheckValue)
{
	char            lHSMBufferOut[BUF_LG_MAX];
	char            lHSMBufferIn[BUF_LG_MAX];
	int             length;

	int             lRet;
	pid_t           nPid;
	char            detail[MAX_LINE_TRC];


	sprintf(detail, "Start GenerateKeyCheckValue");
	trace_event(detail, PROCESSING);


	/*
	* - memset des buffers
	* -------------------------------------------------
	*/
	memset(lHSMBufferOut, 0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,  0, sizeof(lHSMBufferIn));
	memset(detail,        0, sizeof(detail));


	/*
	* - preparation des buffers pour HSM
	* -----------------------------------
	*/
	length = command_BU(lHSMBufferIn,"02",'1', pKeyLmk, "");

	/*
	* - demande de generation de la TMK/TMK
	* --------------------------------
	*/

	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((lRet != OK)) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)", PROCESSING);
		trace_event("End   GenerateKeyCheckValue(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	lRet = HsmQuery(nIndexCtx, nPid, lHSMBufferIn, lHSMBufferOut);
	if (lRet == NOT_AVAILABLE) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateKeyCheckValue(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (lRet == HSM_MALFUNCTION) {
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   GenerateKeyCheckValue(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	lRet = command_BV(lHSMBufferOut, pCheckValue);
	if (lRet != OK) {
		trace_event("Err. In command_BV()", ERROR);
		trace_event("End   GenerateKeyCheckValue(NOK)", PROCESSING);
		return (lRet);
	}

	sprintf(detail, "End   GenerateKeyCheckValue(%.16s)", pCheckValue);
	trace_event(detail, PROCESSING);
	return (OK);
}

int ExportKeyUnderRSA(
	int              nIndexCtx,
	unsigned char    *pRsaKey,
	char             *pDesKey,
	char			 *pKeyType,
	char             *pCheckValue16,
	unsigned char    *pExportedKey,
	int              *pExportedKeyLen)
{
	unsigned char   lHSMBufferOut        [LG_MAX ];
	unsigned char   lHSMBufferIn         [LG_MAX ];
	char            Encryption_Identifier[  2 + 1];
	char            Pad_Mode_Identifier  [  2 + 1];
	char            Hash_Function        [  2 + 1];
	char            Key_Type             [  4 + 1];
	char            DES_Key_Flag         [  1 + 1];
	char            Initialization_value [ 16 + 1];
	char            Encrypted_Key_Length [  4 + 1];
	char            Signature_Indicator  [  1 + 1];
	char            Signature_Length     [  4 + 1];
	char            Signature            [512 + 1];
	char            sRsaKey              [1024+ 1];
	char            sErrCode             [ 10 + 1];
	int             length;
	int             lRet;
	pid_t           nPid;
	char            detail[LG_MAX];


	/*
	* - memset des buffers
	* -------------------------------------------------
	*/
	memset(lHSMBufferOut,              0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,               0, sizeof(lHSMBufferIn));
	memset(detail,                     0, sizeof(detail));
	memset(Encryption_Identifier,      0, sizeof(Encryption_Identifier));
	memset(Pad_Mode_Identifier,        0, sizeof(Pad_Mode_Identifier));
	memset(Key_Type,        		       0, sizeof(Key_Type));
	memset(DES_Key_Flag,        	     0, sizeof(DES_Key_Flag));
	memset(Initialization_value,       0, sizeof(Initialization_value));
	memset(Encrypted_Key_Length,       0, sizeof(Encrypted_Key_Length));
	memset(Signature_Indicator,        0, sizeof(Signature_Indicator));
	memset(Signature_Length,           0, sizeof(Signature_Length));
	memset(Signature,                  0, sizeof(Signature));
	memset(sErrCode,                   0, sizeof(sErrCode));
	memset(sRsaKey,                    0, sizeof(sRsaKey));
	memset(Hash_Function,              0, sizeof(Hash_Function));


	sprintf(detail, "Start ExportKeyUnderRSA()");
	trace_event(detail, PROCESSING);

	/*
	* - preparation des variables
	* -----------------------------------
	*/
	memcpy(Encryption_Identifier,   HSM_ENC_ID_RSA,   2);    /* RSA */
	memcpy(Pad_Mode_Identifier,     HSM_PAD_MODE_PKCS1_1_5,   2);    /* PKCS#1 */
	memcpy(Hash_Function,           HSM_HASH_ID_SHA_1,   2);    /* SHA-1 */
	if( memcmp(pKeyType,THALES_KEY_TP_TMK,3) == 0 )
	{
		memcpy(Key_Type,    		        "1400", 4);
	}
	else
	{
		memcpy(Key_Type,    		        "0000", 4);
	}

	memcpy(DES_Key_Flag,    		    "1",    1);    /* double length */

	/*
	* - preparation des buffers pour HSM
	* -----------------------------------
	*/
	length = command_GK(	lHSMBufferIn ,
		Encryption_Identifier              , /* 2N used to encrypt the key */
		Pad_Mode_Identifier                , /* 2N 01 = PKCS#1, 02 = OAEP */
		""                                 , /* MGF */
		Hash_Function      				   , /* 2N 01 = SHA-1 */
		""                                 , /* 2N present if OAEP mode */
		""                                 , /* 2N present if OAEP mode */
		Key_Type                           , /* 4N used to indicate LMK pair and variant used to encrypt the key */
		Signature_Indicator                , /* 1A = present if sig fields are present */
		""                                 , /* 2A Identifier of the hash algorithm  */
		""                                 , /* 2A 01 = PKCS#1 Identifier of the signature algorithm*/
		""                                 , /* 2N only present if Signature_Identifier present  */
		""                                 , /* 4N default 0000*/
		""                                 , /* nB if Signature_Identifier present */
		""                                 , /* 4N default 0000 */
		""                                 , /* nB if Signature_Identifier present  */
		""                                 , /* 2N location of the private key if flag = 99 use private key provided with command */
		""                                 , /* 4N only present if flag = 99*/
		""                                 , /* nB encrypted using LMK pair 34-35 only present if flag = 99*/
		DES_Key_Flag                       , /* 1N  0 : single, 1 : double, 2 : triple*/
		pDesKey		                       , /* Only present when exporting a DES key (Key Type <> 3401) */
		pCheckValue16                      , /* 16H Check value on DES key (Key Type <> 3401)*/
		""                                 , /* 2N Only present if importing an HMAC key (i.e. Key Type = 3401). */
		""                                 , /* 4N Only present if importing an HMAC key (i.e. Key Type = 3401). */
		""                                 , /* NB if Key Type = 3401*/
		""                                 , /* 4B */
		pRsaKey		                       , /* nB DER encoded in ASN.1 format */
		""                                 , /* nB Optional*/
		"03"                               , /* 2N 01 = Key Block format, 02 = Key Block Template, 03 = Unformatted Key Bloc ... */
		""                                 , /* 4N */
		""                                 , /* nH Key data and Check Value data (if present) zero filled  if Key_Block_Type= 02*/
		""                                 , /*  if Key_Block_Type = 02 */
		""                                 , /* 2N 0..8 if Key Block Type = 02*/
		""                                  /* 4N if Key Block Type = 02*/
		);

	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((lRet != OK)) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   ExportKeyUnderRSA(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	lRet = HsmQueryBin(nIndexCtx, nPid, lHSMBufferIn, length, lHSMBufferOut);
	if (lRet == NOT_AVAILABLE) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   ExportKeyUnderRSA(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (lRet == HSM_MALFUNCTION) {
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   ExportKeyUnderRSA(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	lRet = command_GL(  lHSMBufferOut,
		Key_Type                           , /* 4N used to indicate LMK pair and variant used to encrypt the key */
		Initialization_value               , /* 16H if Key Type <> 3401*/
		Encrypted_Key_Length               , /* 4N */
		pExportedKey                       , /* nB */
		Signature_Indicator                , /* = */
		Signature_Length                   , /* 4N Only present when the Signature Indicator is present*/
		Signature                          , /* nB Only present when the Signature Indicator is present*/
		sErrCode
		);

	if (lRet != OK) {
		trace_event("Err. In command_GL()", ERROR);
		trace_event("End   ExportKeyUnderRSA(NOK)", PROCESSING);
		return (lRet);
	}

	*pExportedKeyLen = atoi(Encrypted_Key_Length);

	sprintf(detail, "End   ExportKeyUnderRSA(%.32s,%.32s)", pRsaKey, pDesKey);
	trace_event(detail, PROCESSING);
	return (OK);
}

int GenerateSignatureBlock(
	int              nIndexCtx,
	unsigned char    *pMessageData,
	int              pMessageDataLen,
	char             *pPrivKey,
	unsigned char    *pSignature,
	int              *pSignatureLen)
{
	unsigned char   lHSMBufferOut[LG_MAX];
	unsigned char   lHSMBufferIn[LG_MAX];
	char            sHashIdentifier     [2 +1];
	char            sSignatureIdentifier[2 +1];
	char            sPadModeIdentifier  [2 +1];
	char            sDataLength         [4 +1];
	char            sSecretKeyLen       [4 +1];
	unsigned char   sRSAImpEpp          [1024 +1];
	unsigned char   sRSAEpp             [1024 +1];
	int             length;
	int							lTmpLen;
	int             lRet;
	int             nRSAEPPLen;
	pid_t           nPid;
	char            detail[MAX_LINE_TRC];


	/*
	* - memset des buffers
	* -------------------------------------------------
	*/
	memset(lHSMBufferOut,         0, sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,          0, sizeof(lHSMBufferIn));
	memset(sHashIdentifier,       0, sizeof(sHashIdentifier     ));
	memset(sSignatureIdentifier,  0, sizeof(sSignatureIdentifier));
	memset(sPadModeIdentifier,    0, sizeof(sPadModeIdentifier));
	memset(sDataLength,           0, sizeof(sDataLength));
	memset(sSecretKeyLen,         0, sizeof(sSecretKeyLen));
	memset(detail,                0, sizeof(detail));
	memset(sRSAImpEpp,            0, sizeof(sRSAImpEpp));
	memset(sRSAEpp,               0, sizeof(sRSAEpp));

	sprintf(detail, "Start GenerateSignatureBlock(%.30s)", pMessageData);
	trace_event(detail, PROCESSING);

	/*
	* - preparation des parametres
	* -----------------------------------
	*/
	memcpy (sHashIdentifier,      HSM_HASH_ID_SHA_1, 2);		/* '01' : SHA-1         */
	memcpy (sSignatureIdentifier, HSM_SIG_ID_RSA, 2);			/* '01' : RSA           */
	memcpy (sPadModeIdentifier,   HSM_PAD_MODE_PKCS1_1_5, 2);  /* '01' : PKCS#1 v1.5   */


	/*
	Note: When a public key is transmitted from NDC to Central, only
	256 bytes of the modulus are sent. The bytes describing the
	containing structure and the exponent of the key never change, so
	they are not transmitted. The complete key structure shown below
	must be reconstituted before the signing process.
	3082010A- Tag: structure, length 266 bytes
	02820101- Tag: integer, length 257 bytes
	00- Leading 00 ensures modulus is positive
	MM..MM- 256 byte modulus
	0203- Tag: integer, length 3 bytes
	010001- Exponent (always 65537)
	*/



	/*memcpy (lRSA, "3082010A0282010100", 18);
	memcpy (lRSA + 18, pRSAIMPEPP, lTmpLen);
	memcpy (lRSA + 18 + lTmpLen, "0203010001", 10);
	lTmpLen += (18 + 10);*/


	/*ZKO03042019 Start*/
		/*lTmpLen = strlen(pPrivKey);
		AsciiToHex(pPrivKey, sRSAImpEpp, lTmpLen);
		sprintf(sSecretKeyLen,  "%04d", lTmpLen/2);*/
	/*ZKO03042019 End*/

	sprintf(sDataLength,    "%04d", pMessageDataLen);
	dump_buffer(pMessageData, atoi(sDataLength),'A','X','X');

	/*
	* - preparation des buffers pour HSM
	* -----------------------------------
	*/
	length = command_EW  (lHSMBufferIn,
		sHashIdentifier,
		sSignatureIdentifier,
		sPadModeIdentifier,
		pMessageData,
		pMessageDataLen,
		"99",
		/*sSecretKeyLen,*/
		pPrivKey/*sRSAImpEpp*/
		);

	lRet = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((lRet != OK)) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateSignatureBlock(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	lRet = HsmQueryBin(nIndexCtx, nPid, lHSMBufferIn, length, lHSMBufferOut);
	if (lRet == NOT_AVAILABLE) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateSignatureBlock(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (lRet == HSM_MALFUNCTION) {
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   GenerateSignatureBlock(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	lRet = command_EX(lHSMBufferOut, pSignature, pSignatureLen);
	if (lRet != OK) {
		trace_event("Err. In command_EX()", ERROR);
		return (lRet);
	}

	sprintf(detail, "End   GenerateSignatureBlock(%.16s)", pSignature);
	trace_event(detail, PROCESSING);
	return (OK);
}

int import_rsa_public_key (int nIndexCtx, char *pRsaKey, char *sRSAKeyBlock, char *pMac)
{
	char                    sLine[MAX_LINE_TRC];
	unsigned char           lHSMBufferOut[LG_MAX];
	unsigned char           lHSMBufferIn[LG_MAX];
	unsigned char           sNCRPublicKey[LG_MAX];
	int                     nReturn, nLength;
	pid_t                   nPid;


	memset(sNCRPublicKey,   0,    sizeof(sNCRPublicKey));
	memset(lHSMBufferOut,   0,    sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,    0,    sizeof(lHSMBufferIn));

	sprintf(sLine, "Start import_rsa_public_key()");
	trace_event(sLine, TRACE);

	nLength = command_EO(lHSMBufferIn, pRsaKey, "02"); /* '02' : DER encoding for ASN.1 Public Key */

	nReturn = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((nReturn != OK)) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   import_rsa_public_key(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	nReturn = HsmQueryBin( nIndexCtx, nPid , lHSMBufferIn, nLength, lHSMBufferOut );
	if (nReturn == NOT_AVAILABLE) {
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   import_rsa_public_key(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (nReturn == HSM_MALFUNCTION) {
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   import_rsa_public_key(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	nReturn = command_EP(lHSMBufferOut, sRSAKeyBlock, pMac);
	if (nReturn != OK) {
		trace_event("Err. In command_EP()", ERROR);
		trace_event("End   import_rsa_public_key(NOK)", PROCESSING);
		return (nReturn);
	}

	sprintf(sLine, "End   import_rsa_public_key(%d)", nReturn);
	trace_event(sLine, PROCESSING);
	return (OK);

}

int verify_rsa_signature (int nIndexCtx, char *sPublicKey, char *pData, char *pSignature)
{
	char                    sLine[MAX_LINE_TRC];
	unsigned char           lHSMBufferOut[BUF_LG_MAX];
	unsigned char           lHSMBufferIn[LG_MAX];
	int                     nReturn, nLength;
	pid_t                   nPid;


	memset(lHSMBufferOut,   0,    sizeof(lHSMBufferOut));
	memset(lHSMBufferIn,    0,    sizeof(lHSMBufferIn));

	sprintf(sLine, "Start verify_rsa_signature(%.12s,%.12s,%.12s)",sPublicKey,pData,pSignature);
	trace_event(sLine, TRACE);


	nLength = command_EY(	lHSMBufferIn,
		HSM_HASH_ID_SHA_1,
		HSM_SIG_ID_RSA,
		HSM_PAD_MODE_PKCS1_1_5,
		pData,
		pSignature,
		sPublicKey);

	nReturn = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((nReturn != OK))
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   verify_rsa_signature(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	nReturn = HsmQueryBin( nIndexCtx, nPid , lHSMBufferIn, nLength, lHSMBufferOut );
	if (nReturn == NOT_AVAILABLE)
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   verify_rsa_signature(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (nReturn == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   verify_rsa_signature(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	nReturn = command_EZ(lHSMBufferOut);
	if (nReturn != OK)
	{
		trace_event("Err. In command_EZ()", ERROR);
		trace_event("End   verify_rsa_signature(NOK)", PROCESSING);
		return (nReturn);
	}

	sprintf(sLine, "End   verify_rsa_signature(%d)", nReturn);
	trace_event(sLine, PROCESSING);
	return (OK);
}



