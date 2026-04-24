/********************************************************************************/
/*  (c) Hightech Payment Systems 2015                                          **/
/* Module                                                                      **/
/*      c2p_ersb_mng.c                                                         **/
/*                                                                             **/
/* Description                                                                 **/
/*      ERSB KEYs Management pour C2P                                          **/
/********************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		12-01-2016   		Mouloud OUSSYMOUH    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <hsm_inc.h>
#include <iso_hps.h>
#include <tlv_autho.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <define.h>
#include <queue.h>
#include <p7_common.h>
#include <iso8583.h>
#include <hsm_c2p_define.h>
#include <hsm_c2p.h>
#include <hsm_c2p_tag_def.h>
#include <resources.h>
#include <cbae.h>

/******************************************************************************/
/* Function GenerateKeyKT                                                     */
/* -------------------                                                        */
/* Tirage de Clé de Transport (KTK/KME)	     	      			              */
/* MOU 12/01/2016															  */
/*                                                                            */
/******************************************************************************/
int GenerateKeyKT    (	int 		nIndexCtx,
						char* 		sResourceId,
						char*		szZmkLmk,
						char*		szKtkLmk,
						char*		szKtkZmk,
						char*		szCheckValue
						)
{
	char    lHSMBufferOut   [LG_MAX];
	char    lHSMBufferIn    [LG_MAX];
	char    lErrCode        [LG_MAX];
	int		result;
	pid_t   nPid;
	TSC2PCmd			    kC2PCmd;
	int					    nLength;
	int					    nLengthSnd;
	int					    nLengthRcv;
	char				    sReturnCode[32];
	char				    sComplInfo[32];
	char				    sKeyRaw[LG_MAX];
    char				sIdKey[LG_MAX];
	char				sKeyUsage[4];	
	char				sWrapKey[LG_MAX];
	int					nResult;

	trace_event("Start GenerateKeyKT()",PROCESSING);
	
	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
    memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	memset(lErrCode,			0, sizeof(lErrCode));
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sKeyUsage, 0, sizeof( sKeyUsage ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_KEY_E9);

	memcpy(sIdKey,"02",2);
	memcpy(sIdKey+2,szZmkLmk+56,36);
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
	
	memcpy(sIdKey,szZmkLmk+12,38);
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
    memset(sKeyRaw, 0, sizeof( sKeyRaw ) );


	sprintf(sKeyUsage,"%4s","FFBC");
	nLength = strlen(sKeyUsage);
	nLength = AsciiToHex(sKeyUsage,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_USAGE, &kC2PCmd, sKeyRaw, nLength);

	nLength = strlen(szZmkLmk);
	nLength = AsciiToHex(szZmkLmk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (lHSMBufferIn, &kC2PCmd);

 	result = CheckHsmResource ( nIndexCtx, HSM_VERIF_MODE , &nPid );
	if ( ( result != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE(CheckResource)", ERROR);
		trace_event("End   GenerateKeyKT(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}
	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, lHSMBufferIn, nLengthSnd, lHSMBufferOut,&nLengthRcv );
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
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_KEY_FF09);

	AnalyseC2PCmdInfo(lHSMBufferOut, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, szKtkLmk, 2*nLength);

		 GetC2PCmdInfoField (C2P_FN_KEY_KEV, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, szKtkZmk, 2*nLength);
	}
	else
	{	
		 trace_event("End   GenerateKeyKT(NOK)",PROCESSING); 
		 return(NOK);
	}
	/*result = command_A1(lHSMBufferOut, '1',szKtkLmk,szKtkZmk, szCheckValue);*/
	result=Verif_Sym_Key(	nIndexCtx,
					        szKtkLmk,
						    szCheckValue
		   	            );
	
    if(result != OK)
    {
        trace_event("Err. In Verif_Sym_Key()", ERROR);
        trace_event("End   GenerateKeyKT(NOK)", PROCESSING);
        return (result);
    }

	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
    memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	trace_message(PROCESSING,"KCV: [%s]",szCheckValue);
	trace_event("End GenerateKeyKT(OK)",PROCESSING);
	return OK;
}

/******************************************************************************/
/* Function GenerateKeyKS                                                     */
/* -------------------                                                        */
/* Tirage de Clé de scellement	(KSC/KTK)     	      			                      */
/* MOU 12/01/2016															  */
/*                                                                            */
/******************************************************************************/
int GenerateKeyKS    (	int 		nIndexCtx,
						char* 		sResourceId,
						char*		szZmkLmk,
						char*		szKtkLmk,
						char*		szKtkZmk,
						char*		szCheckValue
						)
{
	char    lHSMBufferOut   [LG_MAX];
	char    lHSMBufferIn    [LG_MAX];
	char    lErrCode        [LG_MAX];
	int		result;
	pid_t   nPid;
	TSC2PCmd			    kC2PCmd;
	int					    nLength;
	int					    nLengthSnd;
	int					    nLengthRcv;
	char				    sReturnCode[32];
	char				    sComplInfo[32];
	char				    sKeyRaw[256];
    char				sIdKey[38];
	char				sKeyUsage[4];	
	char				sWrapKey[32];
	int					nResult;

	trace_event("Start GenerateKeyKS()",PROCESSING);
	
	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
    memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	memset(lErrCode,			0, sizeof(lErrCode));
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sKeyUsage, 0, sizeof( sKeyUsage ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_KEY_E9);

	memcpy(sIdKey,"27",2);
	memcpy(sIdKey+2,szZmkLmk+56,36);
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
	
	memcpy(sIdKey,szZmkLmk+12,38);
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
    memset(sKeyRaw, 0, sizeof( sKeyRaw ) );


	sprintf(sKeyUsage,"%4s","FFFF");
	nLength = strlen(sKeyUsage);
	nLength = AsciiToHex(sKeyUsage,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_USAGE, &kC2PCmd, sKeyRaw, nLength);

	nLength = strlen(szZmkLmk);
	nLength = AsciiToHex(szZmkLmk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (lHSMBufferIn, &kC2PCmd);

 	result = CheckHsmResource ( nIndexCtx, HSM_VERIF_MODE , &nPid );
	if ( ( result != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE(CheckResource)", ERROR);
		trace_event("End   GenerateKeyKS(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}


	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, lHSMBufferIn, nLengthSnd, lHSMBufferOut,&nLengthRcv );
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
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_KEY_FF09);

	AnalyseC2PCmdInfo(lHSMBufferOut, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, szKtkLmk, 2*nLength);

		 GetC2PCmdInfoField (C2P_FN_KEY_KEV, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, szKtkZmk, 2*nLength);
	}
	else
	{	
		 trace_event("End   GenerateKeyKS(NOK)",PROCESSING); 
		 return(NOK);
	}
	/*result = command_A1(lHSMBufferOut, '1',szKtkLmk,szKtkZmk, szCheckValue);*/
	result=Verif_Sym_Key(	nIndexCtx,
					        szKtkLmk,
						    szCheckValue
		   	            );
	
    if(result != OK)
    {
        trace_event("Err. In Verif_Sym_Key()", ERROR);
        trace_event("End   GenerateKeyKS(NOK)", PROCESSING);
        return (result);
    }

	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
    memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	trace_message(PROCESSING,"KCV: [%s]",szCheckValue);
	trace_event("End GenerateKeyKS(OK)",PROCESSING);
	return OK;
}

/******************************************************************************/
/* Function GenerateKeyKC                                                     */
/* -------------------                                                        */
/* Tirage de Clé de chiffrement	(KT/KTK)     	      			              */
/* MOU 12/01/2016															  */
/*                                                                            */
/******************************************************************************/
int GenerateKeyKC    (	int 		nIndexCtx,
						char* 		sResourceId,
						char*		szZmkLmk,
						char*		szKtkLmk,
						char*		szKtkZmk,
						char*		szCheckValue
						)
{
	char    lHSMBufferOut   [LG_MAX];
	char    lHSMBufferIn    [LG_MAX];
	char    lErrCode        [LG_MAX];
	int		result;
	pid_t   nPid;
	TSC2PCmd			    kC2PCmd;
	int					    nLength;
	int					    nLengthSnd;
	int					    nLengthRcv;
	char				    sReturnCode[32];
	char				    sComplInfo[32];
	char				    sKeyRaw[256];
    char				sIdKey[38];
	char				sKeyUsage[4];	
	char				sWrapKey[32];
	int					nResult;

	trace_event("Start GenerateKeyKC()",PROCESSING);
	
	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
    memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	memset(lErrCode,			0, sizeof(lErrCode));
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sKeyUsage, 0, sizeof( sKeyUsage ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_KEY_E9);
	
	memcpy(sIdKey,"26",2);
	memcpy(sIdKey+2,szZmkLmk+56,36);
	
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
	
	memcpy(sIdKey,szZmkLmk+12,38);
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
    memset(sKeyRaw, 0, sizeof( sKeyRaw ) );


	sprintf(sKeyUsage,"%4s","FFFF");
	nLength = strlen(sKeyUsage);
	nLength = AsciiToHex(sKeyUsage,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_USAGE, &kC2PCmd, sKeyRaw, nLength);

	nLength = strlen(szZmkLmk);
	nLength = AsciiToHex(szZmkLmk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (lHSMBufferIn, &kC2PCmd);

 	result = CheckHsmResource ( nIndexCtx, HSM_VERIF_MODE , &nPid );
	if ( ( result != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE(CheckResource)", ERROR);
		trace_event("End   GenerateKeyKC(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}


	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, lHSMBufferIn, nLengthSnd, lHSMBufferOut,&nLengthRcv );
	if ( result == NOT_AVAILABLE )
	{
		trace_event("HSM FOR GENERATION   NOT AVAILABLE(HsmQuery)", ERROR);
		trace_event("End   GenerateKeyKC(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}
	else if (result == HSM_MALFUNCTION)
	{
		trace_event("HSM GENERATION  MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateKeyKC(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_MALFUNCTION);
	}
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_KEY_FF09);

	AnalyseC2PCmdInfo(lHSMBufferOut, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, szKtkLmk, 2*nLength);

		 GetC2PCmdInfoField (C2P_FN_KEY_KEV, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, szKtkZmk, 2*nLength);
	}
	else
	{	
		 trace_event("End   GenerateKeyKC(NOK)",PROCESSING); 
		 return(NOK);
	}
	/*result = command_A1(lHSMBufferOut, '1',szKtkLmk,szKtkZmk, szCheckValue);*/
	result=Verif_Sym_Key(	nIndexCtx,
					        szKtkLmk,
						    szCheckValue
		   	            );
	
    if(result != OK)
    {
        trace_event("Err. In Verif_Sym_Key()", ERROR);
        trace_event("End   GenerateKeyKC(NOK)", PROCESSING);
        return (result);
    }

	memset(lHSMBufferOut,       0, sizeof(lHSMBufferOut));
    memset(lHSMBufferIn,        0, sizeof(lHSMBufferIn) );
	trace_message(PROCESSING,"KCV: [%s]",szCheckValue);
	trace_event("End GenerateKeyKC(OK)",PROCESSING);
	return OK;
}
/********************************************************************************/
/* Fonction KCKSLmkToKtk                                                        */
/* ----------------------                                                       */
/* Description : Cette fonction effectue la translation des clés KC/KS sous LMK */
/* vers KTK                                                                     */
/*                                                                              */
/********************************************************************************/
int  KCKSLmkToKtk(	int		nIndexCtx,
					char	mode,
					char	*kcksLmk, 
					char	*ktkLmk, 
					char	*kcksKtk,
					char	*check_in)
{
/*-----Déclaration des variables-----*/		
    unsigned char		BuffSnd   [C2P_BUF_MAX_LEN];
	unsigned char		BuffRcv   [C2P_BUF_MAX_LEN];
	int					nResult;
	pid_t				nPid;
	char				sLine[MAX_LINE_TRC];
	TSC2PCmd			kC2PCmd;
	int					nLength;
	int					nLengthSnd;
	int					nLengthRcv;
	char				sReturnCode[32];
	char				sComplInfo[32];
	char				sIdKey[38];
	char				*sData;
	char				sKeyRaw[256];
	char				sWrapKey[256];
 /*-----Initialisation des variables-----*/

	trace_event("Start KCKSLmkToKtk()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sData, 0, sizeof( sData ) );
	

	trace_event("- Key Translate KS/KC Under KTK ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_EXPORT_KEY_FF20);
	
	nLength = strlen(kcksLmk);
	nLength = AsciiToHex(kcksLmk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);	
	
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	
	nLength = strlen(ktkLmk);
	nLength = AsciiToHex(ktkLmk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   KCKSLmkToKtk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   KCKSLmkToKtk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   KCKSLmkToKtk(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_EXPORT_KEY_FF30);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	
   if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_KEY_KEV, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, kcksKtk, 2*nLength);

		 GetC2PCmdInfoField (C2P_FN_KEY_KCV, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, check_in, 2*nLength);
	}
	else
	{	
		 trace_event("End   KCKSLmkToKtk(NOK)",PROCESSING); 
		 return(NOK);
	}
	
	trace_event("End   KCKSLmkToKtk(OK)",PROCESSING);
	return(OK);
}
int  ZakZmkToLmk(	int		nIndexCtx,
					char	*zmkLmk, 
					char	*zakZmk, 
					char	*check_in, 
					char	*zakLmk, 
					char	*check_out)
{
/*-----Déclaration des variables-----*/		
    unsigned char		BuffSnd   [C2P_BUF_MAX_LEN];
	unsigned char		BuffRcv   [C2P_BUF_MAX_LEN];
	int					nResult;
	pid_t				nPid;
	char				sLine[MAX_LINE_TRC];
	TSC2PCmd			kC2PCmd;
	int					nLength;
	int					nLengthSnd;
	int					nLengthRcv;
	char				sReturnCode[32];
	char				sComplInfo[32];
	char				sIdKey[256];
	char				sData[256];
	char				sKeyRaw[LG_MAX];
	char				sWrapKey[256];
 /*-----Initialisation des variables-----*/

	trace_event("Start ZakZmkToLmk()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sData, 0, sizeof( sData ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );
	
	trace_event("- ZAK Key Translation from ZMK to LMK ... ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_IMPORT_KEY_EB);
	
	nLength = strlen(zmkLmk);
	nLength = AsciiToHex(zmkLmk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	
	memcpy(sIdKey,"27",2);
	memcpy(sIdKey+2,zmkLmk+56,36);
	
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);	

	sprintf(sData,"%4s","FFFF");
	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_USAGE, &kC2PCmd, sKeyRaw, nLength);
	
	/*nLength = strlen(check_in);
	nLength = AsciiToHex(check_in,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_KCV, &kC2PCmd, sKeyRaw, nLength);*/
	
	nLength = strlen(zakZmk);
	nLength = AsciiToHex(zakZmk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_KEV, &kC2PCmd, sKeyRaw, nLength);
	
	memcpy(sIdKey,zmkLmk+12,38);
	
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);	

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   ZakZmkToLmk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   ZakZmkToLmk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   ZakZmkToLmk(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_IMPORT_KEY_FF0B);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

   if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	   GetC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sWrapKey, &nLength);
       HexToAscii( sWrapKey, zakLmk, 2*nLength);

	   memcpy(check_out,zakLmk+112,6);

	}
	else
	{	
		 trace_event("End   ZakZmkToLmk(NOK)",PROCESSING); 
		 return(NOK);
	}
	
	trace_event("End   ZakZmkToLmk(OK)",PROCESSING);
	return(OK);

}
int  KCKSKtkToLmk(	int		nIndexCtx,
					char	mode,
					char	*ktkLmk, 
					char	*kcksKtk, 
					char	*check_in, 
					char	*kcksLmk, 
					char	*check_out)
{
/*-----Déclaration des variables-----*/		
    unsigned char		BuffSnd   [C2P_BUF_MAX_LEN];
	unsigned char		BuffRcv   [C2P_BUF_MAX_LEN];
	int					nResult;
	pid_t				nPid;
	char				sLine[MAX_LINE_TRC];
	TSC2PCmd			kC2PCmd;
	int					nLength;
	int					nLengthSnd;
	int					nLengthRcv;
	char				sReturnCode[32];
	char				sComplInfo[32];
	char				sIdKey[LG_MAX];
	char				sData[256];
	char				sKeyRaw[LG_MAX];
	char				sWrapKey[LG_MAX];
 /*-----Initialisation des variables-----*/

	trace_event("Start KCKSKtkToLmk()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sData, 0, sizeof( sData ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );
	
	trace_event("- KC/KS Key Translation from ZMK to LMK---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_IMPORT_KEY_EB);
	
	nLength = strlen(ktkLmk);
	nLength = AsciiToHex(ktkLmk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	sData[0]=mode;
	
	if (memcmp(sData,"1",1)==0)
		memcpy(sIdKey,"26",2); 
    else 
		memcpy(sIdKey,"27",2); 

	memcpy(sIdKey+2,ktkLmk+56,36);
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);	

	sprintf(sData,"%4s","FFFF");
	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_USAGE, &kC2PCmd, sKeyRaw, nLength);
	
	/*nLength = strlen(check_in);
	nLength = AsciiToHex(check_in,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_KCV, &kC2PCmd, sKeyRaw, nLength);*/
	
	nLength = strlen(kcksKtk);
	nLength = AsciiToHex(kcksKtk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_KEV, &kC2PCmd, sKeyRaw, nLength);
	
    memcpy(sIdKey,ktkLmk+12,38);
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   KCKSKtkToLmk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   KCKSKtkToLmk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   KCKSKtkToLmk(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_IMPORT_KEY_FF0B);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

    if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	   GetC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sWrapKey, &nLength);
       HexToAscii( sWrapKey, kcksLmk, 2*nLength);
       memcpy(check_out,kcksLmk+112,6);

	}
	else
	{	
		 trace_event("End   KCKSKtkToLmk(NOK)",PROCESSING); 
		 return(NOK);
	}
	
	trace_event("End   KCKSKtkToLmk(OK)",PROCESSING);
	return(OK);

}
int  KtkZmkToLmk(	int		nIndexCtx,
					char	*zmkLmk, 
					char	*ktkZmk, 
					char	*check_in, 
					char	*ktkLmk, 
					char	*check_out)
{
/*-----Déclaration des variables-----*/		
    unsigned char		BuffSnd   [C2P_BUF_MAX_LEN];
	unsigned char		BuffRcv   [C2P_BUF_MAX_LEN];
	int					nResult;
	pid_t				nPid;
	char				sLine[MAX_LINE_TRC];
	TSC2PCmd			kC2PCmd;
	int					nLength;
	int					nLengthSnd;
	int					nLengthRcv;
	char				sReturnCode[32];
	char				sComplInfo[32];
	char				sIdKey[LG_MAX];
	char				sData[256];
	char				sKeyRaw[LG_MAX];
	char				sWrapKey[LG_MAX];
 /*-----Initialisation des variables-----*/

	trace_event("Start KtkZmkToLmk()..",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sData, 0, sizeof( sData ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );
	
	trace_event("- KTK Key Translation from ZMK to LMK---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_IMPORT_KEY_EB);
	
	nLength = strlen(zmkLmk);
	nLength = AsciiToHex(zmkLmk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	memcpy(sIdKey,"02",2);
	memcpy(sIdKey+2,zmkLmk+56,36);
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);	

	sprintf(sData,"%4s","FFFF");
	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_USAGE, &kC2PCmd, sKeyRaw, nLength);
	
	/*nLength = strlen(check_in);
	nLength = AsciiToHex(check_in,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_KCV, &kC2PCmd, sKeyRaw, nLength);*/
	
	nLength = strlen(ktkZmk);
	nLength = AsciiToHex(ktkZmk,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_KEV, &kC2PCmd, sKeyRaw, nLength);
	
    memcpy(sIdKey,zmkLmk+12,38);
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);	

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   KtkZmkToLmk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   KtkZmkToLmk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   KtkZmkToLmk(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_IMPORT_KEY_FF0B);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

    if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	    GetC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sWrapKey, &nLength);
        HexToAscii( sWrapKey, ktkLmk, 2*nLength);
        memcpy(check_out,ktkLmk+112,6);
	}
	else
	{	
		 trace_event("End   KtkZmkToLmk(NOK)",PROCESSING); 
		 return(NOK);
	}
	
	trace_event("End   KtkZmkToLmk(OK)",PROCESSING);
	return(OK);

}
int     gen_msg_auth_code(
						int nIndiceCtx,
						unsigned char	format, 
						unsigned char	*algorithm, 
						unsigned char	padMethod,
						unsigned char	*keyTpe,
						unsigned char	*takzak_key,
						unsigned char	*mac_data,
						unsigned char	*message_mac,
						int				lg_message_mac,
						char			*sHsmResultCode)
{
	unsigned char		BuffSnd   [C2P_BUF_MAX_LEN];
	unsigned char		BuffRcv   [C2P_BUF_MAX_LEN];
	int					nResult;
	pid_t				nPid;
	char				sLine[MAX_LINE_TRC];
	char				sData[256];
	char				sKeyRaw[LG_MAX];
	TSC2PCmd			kC2PCmd;
	int					nLength;
	int					nLengthSnd;
	int					nLengthRcv;
	char				sReturnCode[32];
	char				sComplInfo[32];
	char                sWrapKey[256];

	trace_event("Start gen_msg_auth_code()",PROCESSING);

	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sData, 0, sizeof( sData ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );

	trace_event("- Msg Auth Code Generation ...---------------", PROCESSING);
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_HASH_F8);

	sData[0]='0';sData[1]=padMethod;
	
	AddC2PCmdInfoField (C2P_FN_PAD_ALGO_ID, &kC2PCmd, sData, GetFieldIdLength(C2P_FN_PAD_ALGO_ID,&kC2PCmd));
		
	nLength = strlen(takzak_key);
	nLength = AsciiToHex(takzak_key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	/*nLength = strlen(message_mac);
	nLength = AsciiToHex(message_mac,sKeyRaw,nLength);*/
	AddC2PCmdInfoField (C2P_FN_MAC_DATA, &kC2PCmd, message_mac, lg_message_mac);

    /*AddC2PCmdInfoField (C2P_FN_MAC_ALGO_ID_2, &kC2PCmd, algorithm, GetFieldIdLength(C2P_FN_MAC_ALGO_ID_2,&kC2PCmd)); ADD MOU20160512 BNT7**/
    
    /*sprintf(sData,"%d",lg_message_mac);*/
	sprintf(sData,"%d","64");/*SNO260216*/

	AddC2PCmdInfoField (C2P_FN_MAC_LEN, &kC2PCmd, sData, GetFieldIdLength(C2P_FN_MAC_LEN,&kC2PCmd));

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   gen_msg_auth_code(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndiceCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   gen_msg_auth_code(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   gen_msg_auth_code(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_HASH_FF18);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

    if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_MAC, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, mac_data, 2*nLength);

	}
	else
	{	
		 trace_event("End   gen_msg_auth_code(NOK)",PROCESSING); 
		 return(NOK);
	}
	trace_event("End   gen_msg_auth_code(OK)",PROCESSING);
	return(OK);
}
int verify_msg_auth_code(
						    int				nIndiceCtx,
						    unsigned char	format, 
						    unsigned char   *algorithm, 
						    unsigned char	padMethod,
						    unsigned char	*keyTpe,
						    unsigned char	*takzak_key,
						    unsigned char	*mac_data,
						    unsigned char	*message_mac,
						    int				lg_message_mac,
						    char			*sHsmResultCode
						)
{
	unsigned char		BuffSnd   [C2P_BUF_MAX_LEN];
	unsigned char		BuffRcv   [C2P_BUF_MAX_LEN];
	int					nResult;
	pid_t				nPid;
	char				sLine[MAX_LINE_TRC];
	char				sData[256];
	char				sKeyRaw[LG_MAX];
	TSC2PCmd			kC2PCmd;
	int					nLength;
	int					nLengthSnd;
	int					nLengthRcv;
	char				sReturnCode[32];
	char				sComplInfo[32];


	trace_event("Start verify_msg_auth_code()",PROCESSING);

	
	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(sData, 0, sizeof( sData ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	
	trace_event("- MSG AUTH CODE Verification .---------------", PROCESSING);

	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_HASH_F9);
	
	sData[0]='0';sData[1]=padMethod;
	AddC2PCmdInfoField (C2P_FN_PAD_ALGO_ID, &kC2PCmd, sData, GetFieldIdLength(C2P_FN_PAD_ALGO_ID,&kC2PCmd));

	nLength = strlen(takzak_key);
	nLength = AsciiToHex(takzak_key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	/*nLength = strlen(message_mac);
	nLength = AsciiToHex(message_mac,sKeyRaw,nLength);*/
	AddC2PCmdInfoField (C2P_FN_MAC_DATA, &kC2PCmd, message_mac, lg_message_mac);

	/*AddC2PCmdInfoField (C2P_FN_MAC_ALGO_ID_2, &kC2PCmd, algorithm, GetFieldIdLength(C2P_FN_MAC_ALGO_ID_2,&kC2PCmd)); ADD MOU20160512 BNT7*/

	/*sprintf(sData,"%d",lg_message_mac); SNO240216*/
	sprintf(sData, "%d","64");

    AddC2PCmdInfoField (C2P_FN_MAC_LEN, &kC2PCmd, sData, GetFieldIdLength(C2P_FN_MAC_LEN,&kC2PCmd));	
	
	nLength = strlen(mac_data);
	nLength = AsciiToHex(mac_data,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_MAC, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);

	nResult = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   verify_msg_auth_code(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndiceCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   verify_msg_auth_code(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   verify_msg_auth_code(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_HASH_FF19);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);
	if ( memcmp(sReturnCode, "0000", 4) != 0 )
	{	
		 trace_event("End   verify_msg_auth_code(NOK)",PROCESSING); 
		 return(NOK);
	}
	
	trace_event("End   verify_msg_auth_code(OK)",PROCESSING);
	return(OK);
}
