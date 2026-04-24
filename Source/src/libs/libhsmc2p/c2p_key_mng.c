/********************************************************************************/
/*  (c) Hightech Payment Systems 2015                                          **/
/* Module                                                                      **/
/*      c2p_key_mng.c                                                          **/
/*                                                                             **/
/* Description                                                                 **/
/*      KEYs  Management pour C2P                                              **/
/********************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		12-11-2015   		Mouloud OUSSYMOUH    		Initiale Version	
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
/******************************************************************************/
/* Function Gen_Sym_Key                                                       */
/* -------------------                                                        */
/* Tirage de cl� sym�trique	     	      			                          */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Gen_Sym_Key(	int   nIndexCtx,
		   			char *sKDK_Key,
					char *sTK,
		   			char *sRSAModulo,
				    char *sRSAExponent,
					char *DecAlgoID,
					char *DecAlgoParam,/*ICH26052016*/
					char *EncryptDESKey,
					char *KeyIdentData,
					char *sKEY_Jeton,
					char *sKeyUnderRSA,
					char *sCheckValue,
					char *sHsmResultCode
		   	    )
{
/*-----D�claration des variables-----*/		
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
	char				sKeyUsage[4];
	char				sKeyRaw[32];
	char				sWrapKey[32];
	char                            sRSACert[LG_MAX];
 /*-----Initialisation des variables-----*/

	trace_event("Start Gen_Sym_Key()",PROCESSING);

	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sKeyUsage, 0, sizeof( sKeyUsage ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );
	memset(sRSACert, 0, sizeof( sRSACert ) );
	
	sprintf(sLine, "sKDK_Key          : %s", sKDK_Key);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "TK                : %.2s", sTK);
	trace_event(sLine, PROCESSING);


	trace_event("- Key Generation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_KEY_E9);
	
	/*sprintf(sIdKey, "%2s%10s%24s%2s",sTK, "1122334499", "000000000000000000000000","01");*/
	sprintf(sIdKey, "%2s%36s",sTK, sKDK_Key+2);
	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(sKDK_Key);
	nLength = AsciiToHex(sKDK_Key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
    memset(sKeyRaw, 0, sizeof( sKeyRaw ) );


	sprintf(sKeyUsage,"%4s","FFFF");
	nLength = strlen(sKeyUsage);
	nLength = AsciiToHex(sKeyUsage,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_USAGE, &kC2PCmd, sKeyRaw, nLength);
	if (strlen(sRSAModulo) > 0)
	{

		BuildPublicKey(sRSAModulo,sRSAExponent,sRSACert);
	
		nLength = strlen(sRSACert);
		nLength = AsciiToHex(sRSACert,sKeyRaw,nLength);
		AddC2PCmdInfoField (C2P_FN_RSA_PUB_KEY_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
		AddC2PCmdInfoField (C2P_FN_DES_KEY_ENC_RSA_ALGO_ID, &kC2PCmd, DecAlgoID, GetFieldIdLength(C2P_FN_DES_KEY_ENC_RSA_ALGO_ID,&kC2PCmd));



	}
	
	if (strlen(EncryptDESKey) > 0)
	{
	nLength = strlen(EncryptDESKey);
	nLength = AsciiToHex(EncryptDESKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	}
	
	if (strlen(KeyIdentData) > 0)
	{
    nLength = strlen(KeyIdentData);
	nLength = AsciiToHex(KeyIdentData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_IDENT_DATA, &kC2PCmd, sKeyRaw, nLength);
	}
	
	/*Start ICH26052016*/
	if (strlen(DecAlgoParam) > 0)
	{
	    nLength = strlen(DecAlgoParam);
		nLength = AsciiToHex(DecAlgoParam,sKeyRaw,nLength);
		AddC2PCmdInfoField (C2P_FN_OAEP_ENC_PARAM, &kC2PCmd, sKeyRaw, nLength);

	}
	/*End ICH26052016*/

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Gen_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Gen_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Gen_Sym_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_KEY_FF09);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, sKEY_Jeton, 2*nLength);
		 memcpy(sCheckValue,sKEY_Jeton+112,6);
	     trace_event("--------------START TOKEN KEY DES ---------------", PROCESSING);
	     dump_buffer((unsigned char *) sWrapKey, nLength, 'E', 'L', 'O');
	     trace_event("--------------END  TOKEN KEY DES ---------------", PROCESSING);

	 	if (strlen(EncryptDESKey) > 0)
	 	{
		     GetC2PCmdInfoField (C2P_FN_KEY_KEV, &kC2PCmd, sWrapKey, &nLength);
		     HexToAscii( sWrapKey, sKeyUnderRSA, 2*nLength);
	 	}

		if (strlen(sRSAModulo) > 0)
		{
		     GetC2PCmdInfoField (C2P_FN_RSA_PUB_KEY_ENC_KEY, &kC2PCmd, sWrapKey, &nLength);
		     HexToAscii( sWrapKey, sKeyUnderRSA, 2*nLength);
		}

	     trace_event("--------------START KEY UNDER RSA ---------------", PROCESSING);
	     dump_buffer((unsigned char *) sWrapKey, nLength, 'E', 'L', 'O');
	     trace_event("--------------END  KEY UNDER RSA  ---------------", PROCESSING);
	}
	trace_event("End   Gen_Sym_Key(OK)",PROCESSING);
	return(OK);

}
/*******************************************************************************/
/* Function Save_Sym_Key                                                       */
/* -------------------                                                         */
/* Imposition de cl� sym�trique	     	      			                       */
/* MOU 16/11/2015															   */
/*                                                                             */
/*******************************************************************************/
int Save_Sym_Key  (	    int   nIndexCtx,
					    char *sKEY_Jeton,
					    char *sHsmResultCode
		   	          )
{
/*-----D�claration des variables-----*/		
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
	char				sKeyRaw[32];
	char				sData[256];
 /*-----Initialisation des variables-----*/

	trace_event("Start Save_Sym_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sData, 0, sizeof( sData ) );

	sprintf(sLine, "sKEY_Jeton          : %s", sKEY_Jeton);
	trace_event(sLine, PROCESSING);

	
	trace_event("- Key Generation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_SAVE_KEY_EA);
	
	nLength = strlen(sKEY_Jeton);
	nLength = AsciiToHex(sKEY_Jeton,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	/*sData[0] = '3';
	AddC2PCmdInfoField (C2P_FN_TOKEN_RANGE, &kC2PCmd, sData, GetFieldIdLength(C2P_FN_TOKEN_RANGE,&kC2PCmd)); ADD MOU20150512*/

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Save_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Save_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Save_Sym_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_SAVE_KEY_FF0A);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	trace_event("End   Save_Sym_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function Del_Sym_Key                                                       */
/* -------------------                                                        */
/* Suppression d'une cl� sym�trique	     	      			                          */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Del_Sym_Key(	int   nIndexCtx,
					    char *sKEY_Id,
					    char *sHsmResultCode
		   	          )
{
/*-----D�claration des variables-----*/		
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
	char				sKeyRaw[32];
 /*-----Initialisation des variables-----*/

	trace_event("Start Del_Sym_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );

	sprintf(sLine, "sKEY_Id          : %s", sKEY_Id);
	trace_event(sLine, PROCESSING);

	
	trace_event("- Key Generation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_DEL_KEY_EC);
	
	nLength = strlen(sKEY_Id);
	nLength = AsciiToHex(sKEY_Id,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Del_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Del_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Del_Sym_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_DEL_KEY_FF0C);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	trace_event("End   Del_Sym_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function Verif_Sym_Key                                                       */
/* -------------------                                                        */
/* V�rification de cl�	     	      			                          */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Verif_Sym_Key(	int   nIndexCtx,
					    char *sKEY_Jeton,
					    char *CheckValue
		   	          )
{
/*-----D�claration des variables-----*/		
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
	char				sKeyRaw[256];
	char				sWrapKey[256];
 /*-----Initialisation des variables-----*/

	trace_event("Start Verif_Sym_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );

	sprintf(sLine, "sKEY_Jeton          : %s", sKEY_Jeton);
	trace_event(sLine, PROCESSING);

	
	trace_event("- Key Generation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_KEY_ED);
	
	nLength = strlen(sKEY_Jeton);
	nLength = AsciiToHex(sKEY_Jeton,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Verif_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Verif_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Verif_Sym_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_KEY_FF0D);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);
	
	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
		 GetC2PCmdInfoField (C2P_FN_KEY_KCV, &kC2PCmd, sWrapKey, &nLength);
         HexToAscii( sWrapKey, CheckValue, 2*nLength);
	}
	else
	trace_event("End   Verif_Sym_Key(NOK)",PROCESSING);	
	
	trace_event("End   Verif_Sym_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function Imp_Sym_Key                                                       */
/* -------------------                                                        */
/* Importation d'une cl� sym�trique	     	      			                          */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Imp_Sym_Key(	int   nIndexCtx,
		   			char *pKDK,
                    char *pKEK,
                    char *pKeyUnderKEK,
                    char *pKeyType,
                    char *pKcv,
					char *sKEY_Jeton,
					char *sHsmResultCode
		   	    )
{
/*-----D�claration des variables-----*/		
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
	char				sData[256];
	char				sKeyRaw[32];
 /*-----Initialisation des variables-----*/

	trace_event("Start Imp_Sym_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sData, 0, sizeof( sData ) );
	
	sprintf(sLine, "pKDK          : %s", pKDK);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "pKEK                : %s", pKEK);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "pKeyUnderKEK                : %s", pKeyUnderKEK);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "pKeyType                : %s", pKeyType);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "pKcv                : %s", pKcv);
	trace_event(sLine, PROCESSING);


	trace_event("- Key Importation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_IMPORT_KEY_EB);
	
	nLength = strlen(pKEK);
	nLength = AsciiToHex(pKEK,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);	
	
    memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	
	sprintf(sIdKey,"%2s%36s",pKeyType,pKDK+2);

	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);

	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	
	sprintf(sData,"%4s","0000");
	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_USAGE, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(pKcv);
	nLength = AsciiToHex(pKcv,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_KCV, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(pKeyUnderKEK);
	nLength = AsciiToHex(pKeyUnderKEK,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_KEV, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(pKDK);
	nLength = AsciiToHex(pKDK,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Imp_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Imp_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Imp_Sym_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_IMPORT_KEY_FF0B);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKEY_Jeton, &nLength);
	}
	trace_event("End   Imp_Sym_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function Exp_Sym_Key                                                       */
/* -------------------                                                        */
/* Exportation d�une cl� sym�trique 	      			                      */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Exp_Sym_Key(	int   nIndexCtx,
		   			char *pKEK,
                    char *pKeyUnderKDK,
                    char *pKeyUnderKEK,
                    char *pKcv,
					char *sHsmResultCode
		   	    )
{
/*-----D�claration des variables-----*/		
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
	char				sKeyRaw[32];
 /*-----Initialisation des variables-----*/

	trace_event("Start Exp_Sym_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sData, 0, sizeof( sData ) );
	
	sprintf(sLine, "pKeyUnderKDK          : %s", pKeyUnderKDK);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "pKEK                : %s", pKEK);
	trace_event(sLine, PROCESSING);

	trace_event("- Key Exportation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_EXPORT_KEY_FF20);
	
	nLength = strlen(pKeyUnderKDK);
	nLength = AsciiToHex(pKeyUnderKDK,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);	
	
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	
	nLength = strlen(pKEK);
	nLength = AsciiToHex(pKEK,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Exp_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Exp_Sym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Exp_Sym_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_EXPORT_KEY_FF30);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_KEY_KEV, &kC2PCmd, pKeyUnderKEK, &nLength);
		 GetC2PCmdInfoField (C2P_FN_KEY_KCV, &kC2PCmd, pKcv, &nLength);
	}
	trace_event("End   Exp_Sym_Key(OK)",PROCESSING);
	return(OK);

}



int GenerateResourceZPK(int nIndexCtx,char sDesOption,char* sResourceId,char* szZpkLmk,char* szZpkZmk,char* szCheckValue,char* keySchemeZmk,char* KeyBlockVersionId) /* AHA20220930 PLUTONL-4980*/
{


	trace_event("Start GenerateResourceZPK()",PROCESSING);



	trace_event("End   GenerateResourceZPK(OK)",PROCESSING);
	return OK;
}

int     GenerateResourceTAK(int nIndexCtx,char sDesOption,char *pResourceId,char *pTakLmk,char *pTakZmk,char *pCheckValue)
{


	trace_event("End   GenerateResourceTAK(OK)", PROCESSING);
	return (OK);
}