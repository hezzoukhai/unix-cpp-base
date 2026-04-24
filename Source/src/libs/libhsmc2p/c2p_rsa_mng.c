/***********************************************************************************/
/*  (c) Hightech Payment Systems 2015                                          *****/
/* Module                                                                      *****/
/*      c2p_rsa_mng.c                                                          *****/
/*                                                                             *****/
/* Description                                                                 *****/
/*      RSA KEYs Management pour C2P                                          *****/
/***********************************************************************************/
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

void BuildPublicKey     (	
                            char *sPubKeyModulo,
							char *sPubKeyExponent,
							char *sPublicKeyBloc
						)
{
	int				nLenToPut;
	char			sLenNew[6];
	unsigned char	sLen[6];
	int				nNewLenLen;
	int             nLength;
        char				sLine[MAX_LINE_TRC];

	memset(sLenNew, 0, sizeof( sLenNew ) );
	
	trace_event("Start BuildPublicKey()",PROCESSING);
	
	sprintf(sLine, "sPubKeyModulo               : %s", sPubKeyModulo);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "sPubKeyExponent             : %s", sPubKeyExponent);
	trace_event(sLine, PROCESSING);
	
	trace_event("- RSA Public Key Build ---------------", PROCESSING);

	nLength   = strlen(sPubKeyModulo) / 2;
	nLenToPut = nLength;
	if ( nLength <= 127)
	{
		nNewLenLen = 1;
		sLen[0]=(unsigned char)nLenToPut;
	}
	else if ( nLenToPut <= 255)
	{
		nNewLenLen = 2;
		sLen[0]=0x81;
		sLen[1]=((unsigned char)nLenToPut);
	}
	else
	{
		nNewLenLen = 3;
		sLen[0]=0x82;
		sLen[1]=((unsigned char)nLenToPut/256);
		sLen[2]=((unsigned char)nLenToPut%256);
	}
	HexToAscii( sLen, sLenNew, 2*nNewLenLen);

	memcpy(sPublicKeyBloc , C2P_RSA_PUB_KEY_MOD, 2);
	memcpy(sPublicKeyBloc + 2 , sLenNew, 2*nNewLenLen);
	memcpy(sPublicKeyBloc + 2 + 2*nNewLenLen , sPubKeyModulo, 2*nLength);
	memcpy(sPublicKeyBloc + 2 + 2*nNewLenLen + 2*nLength, C2P_RSA_PUB_KEY_EXP, 2);
	memcpy(sPublicKeyBloc + 2 + 2*nNewLenLen + 2*nLength + 2 , "03", 2);
	memcpy(sPublicKeyBloc + 2 + 2*nNewLenLen + 2*nLength + 2 + 2 , sPubKeyExponent, 6);
}

/******************************************************************************/
/* Function Gen_RSA_Key                                                       */
/* -------------------                                                        */
/* G�n�ration cl� RSA	     	      			                              */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Gen_RSA_Key(	int   nIndexCtx,
		   			char *sRSA_Key_ID,
					char *pModuloLen,
					char *sPubKeyExp,
					char *EncryptKeyID,
					char *PrivKeyFMT,
					char *JetonAsymKey
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
	char                            rPubKeyExp[128];
	char                            sData[256];
 /*-----Initialisation des variables-----*/

	trace_event("Start Gen_RSA_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sKeyUsage, 0, sizeof( sKeyUsage ) );
	memset(rPubKeyExp, 0, sizeof( rPubKeyExp ) );
	memset(sData, 0, sizeof( sData ) );
	sprintf(sLine, "sRSA_Key_ID         : %s", sRSA_Key_ID);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "pModuloLen          : %s", pModuloLen);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "sPubKeyExp          : %s", sPubKeyExp);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "EncryptKeyID        : %s", EncryptKeyID);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "EncryptKeyID        : %s", EncryptKeyID);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "PrivKeyFMT        : %s", PrivKeyFMT);
	trace_event(sLine, PROCESSING);

	trace_event("- RSA Key Generation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_RSA_F0);
	
	nLength = strlen(sRSA_Key_ID);
	nLength = AsciiToHex(sRSA_Key_ID,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
	
	AddC2PCmdInfoField (C2P_FN_RSA_KEY_MOD_LEN, &kC2PCmd, pModuloLen, GetFieldIdLength(C2P_FN_RSA_KEY_MOD_LEN,&kC2PCmd));

	nLength = AsciiToBinC2P(sPubKeyExp, rPubKeyExp);

	nLength = strlen(rPubKeyExp);
	nLength = AsciiToHex(rPubKeyExp,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_RSA_PUB_KEY_EXP, &kC2PCmd, sKeyRaw, nLength);
	
	
	nLength = strlen(EncryptKeyID);
	nLength = AsciiToHex(EncryptKeyID,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
	

	AddC2PCmdInfoField (C2P_FN_RSA_PRIV_KEY_FMT_ID, &kC2PCmd, PrivKeyFMT, GetFieldIdLength(C2P_FN_RSA_PRIV_KEY_FMT_ID,&kC2PCmd));
	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Gen_RSA_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Gen_RSA_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Gen_RSA_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_RSA_FF10);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_PRIV_KEY_BLOC, &kC2PCmd, JetonAsymKey, &nLength);
	}
	else
	{
		trace_event("End   Gen_RSA_Key(NOK)",PROCESSING);
	     return(NOK);
	}
	
	trace_event("End   Gen_RSA_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function Import_RSA_Key                                                       */
/* -------------------                                                        */
/* Import d�une cl� sur une cl� RSA	     	      			                          */
/* MOU 16/11/2015																  */
/*                                                                            */
/******************************************************************************/
int Import_RSA_Key(	int   nIndexCtx,
		   			char *sRSA_Pub_Key,
					char *sIdKey,
					char *SecKeyRSA,
					char *StorageKeyId,
					char *sJetonImpKey,
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
	char                sData[256];
 /*-----Initialisation des variables-----*/

	trace_event("Start Import_RSA_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sData, 0, sizeof( sData ) );
	
	
	sprintf(sLine, "sRSA_Pub_Key         : %s", sRSA_Pub_Key);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "sIdKey          : %s", sIdKey);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "SecKeyRSA          : %s", SecKeyRSA);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "StorageKeyId        : %s", StorageKeyId);
	trace_event(sLine, PROCESSING);
	

	trace_event("- RSA Key Importation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_IMP_SEC_KEY_RSA_FF8123);
	
	nLength = strlen(sRSA_Pub_Key);
	nLength = AsciiToHex(sRSA_Pub_Key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_PRIV_KEY_BLOC, &kC2PCmd, sKeyRaw, nLength);	

	nLength = strlen(sIdKey);
	nLength = AsciiToHex(sIdKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
	
	sprintf(sData,"%4s","0000");
	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_USAGE, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(SecKeyRSA);
	nLength = AsciiToHex(SecKeyRSA,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_RSA_PUB_KEY_ENC_KEY, &kC2PCmd, sKeyRaw, nLength);
	
	
	nLength = strlen(StorageKeyId);
	nLength = AsciiToHex(StorageKeyId,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Import_RSA_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Import_RSA_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Import_RSA_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_IMP_SEC_KEY_RSA_FF8133);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sJetonImpKey, &nLength);
	}
	else
	{
		trace_event("End   Import_RSA_Key(NOK)",PROCESSING);
		return(NOK);
	}
	
	trace_event("End   Import_RSA_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function Export_RSA_Key                                                    */
/* -------------------                                                        */
/* Export d�une cl� sur une cl� RSA 	     	      			              */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Export_RSA_Key(	int   nIndexCtx,
					char *sKeyToExp,
					char *PubKey,
					char *sSecKey,
					char *sKcv,
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
	char                sData[256];
 /*-----Initialisation des variables-----*/

	trace_event("Start Export_RSA_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sData, 0, sizeof( sData ) );
	
	
	sprintf(sLine, "sKeyToExp         : %s", sKeyToExp);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "PubKey          : %s", PubKey);
	trace_event(sLine, PROCESSING);


	trace_event("- RSA Key Importation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_EXP_SEC_KEY_RSA_FF8124);
	
	nLength = strlen(sKeyToExp);
	nLength = AsciiToHex(sKeyToExp,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);	

	nLength = strlen(PubKey);
	nLength = AsciiToHex(PubKey,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_CERT_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Export_RSA_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Export_RSA_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Export_RSA_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_EXP_SEC_KEY_RSA_FF8134);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_RSA_PUB_KEY_ENC_KEY, &kC2PCmd, sSecKey, &nLength);
		 GetC2PCmdInfoField (C2P_FN_KEY_KCV, &kC2PCmd, sKcv, &nLength);
	}
	else
	{
		trace_event("End   Export_RSA_Key(NOK)",PROCESSING);
		return(NOK);
	}
	trace_event("End   Export_RSA_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function Encrypt_RSA_Key                                                       */
/* -------------------                                                        */
/* Crypter avec une cl� publique       	     	      			                          */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Encrypt_RSA_Key(	int   nIndexCtx,
		   			    char *sRSAModulo,
						char *sRSAExponent,
					    char *sDataENC,
					    char *sEncDataRSA,
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
	char				sKeyRaw[LG_MAX];
	char                sData[256];
	char                sRSACert[LG_MAX];
 /*-----Initialisation des variables-----*/

	trace_event("Start Encrypt_RSA_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sData, 0, sizeof( sData ) );
	memset(sRSACert, 0, sizeof( sRSACert ) );
	
	sprintf(sLine, "sRSAModulo         : %s", sRSAModulo);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "sDataENC          : %s", sDataENC);
	trace_event(sLine, PROCESSING);
	

	trace_event("- RSA Key encryption ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_ENCRYPT_RSA_FF4C);

	AddC2PCmdInfoField (C2P_FN_RSA_ENC_ALGO_ID, &kC2PCmd, "03", GetFieldIdLength(C2P_FN_RSA_ENC_ALGO_ID,&kC2PCmd));
	
	BuildPublicKey(sRSAModulo,sRSAExponent,sRSACert);
	
	nLength = strlen(sRSACert);
	nLength = AsciiToHex(sRSACert,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_RSA_PUB_KEY_BLOC, &kC2PCmd, sKeyRaw, nLength);	
	nLength = strlen(sDataENC);
	nLength = AsciiToHex(sDataENC,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_RSA_ENC_DATA_IN, &kC2PCmd, sKeyRaw, nLength);		

	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Encrypt_RSA_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Encrypt_RSA_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Encrypt_RSA_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_ENCRYPT_RSA_FF5C);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_RSA_ENC_DATA_OUT, &kC2PCmd, sEncDataRSA, &nLength);
	}
	else
	{
		trace_event("End   Encrypt_RSA_Key(NOK)",PROCESSING);
		return(NOK);
	}
	trace_event("End   Encrypt_RSA_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function Decrypt_RSA_Key                                                       */
/* -------------------                                                        */
/* Decrypter avec une cl� priv�e	     	      			                          */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Decrypt_RSA_Key(	int   nIndexCtx,
		   			    char *ssPrivKeyBloc,
					    char *sEncryptData,
					    char *sDecryptDataRSA,
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
	char                sData[256];
 /*-----Initialisation des variables-----*/

	trace_event("Start Decrypt_RSA_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sData, 0, sizeof( sData ) );
	
	
	sprintf(sLine, "ssPrivKeyBloc         : %s", ssPrivKeyBloc);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "sEncryptData          : %s", sEncryptData);
	trace_event(sLine, PROCESSING);
	

	trace_event("- RSA Key Importation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_DECRYPT_RSA_FF8201);
	
	AddC2PCmdInfoField (C2P_FN_RSA_ENC_ALGO_ID, &kC2PCmd, "03", GetFieldIdLength(C2P_FN_RSA_ENC_ALGO_ID,&kC2PCmd));
	
	nLength = strlen(ssPrivKeyBloc);
	nLength = AsciiToHex(ssPrivKeyBloc,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_PRIV_KEY_BLOC, &kC2PCmd, sKeyRaw, nLength);	

	nLength = strlen(sEncryptData);
	nLength = AsciiToHex(sEncryptData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_RSA_ENC_DATA_OUT, &kC2PCmd, sKeyRaw, nLength);		

	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Decrypt_RSA_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Decrypt_RSA_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Decrypt_RSA_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_DECRYPT_RSA_FF8211);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_RSA_ENC_DATA_IN, &kC2PCmd, sDecryptDataRSA, &nLength);
	}
	else
	{
		trace_event("End   Decrypt_RSA_Key(NOK)",PROCESSING);
		return(NOK);
	}
	trace_event("End   Decrypt_RSA_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function Decrypt_Asym_Key                                                       */
/* -------------------                                                        */
/* Commande de transchiffrement d�une cl� asym�trique                         */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Decrypt_Asym_Key(	int   nIndexCtx,
		   			    char *PrivKeyBlock,
						char *EncryptKeyID,
						char *DecryptedAsymKey
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
	char                            rPubKeyExp[128];
	char                            sData[256];
 /*-----Initialisation des variables-----*/

	trace_event("Start Decrypt_Asym_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sKeyUsage, 0, sizeof( sKeyUsage ) );
	memset(rPubKeyExp, 0, sizeof( rPubKeyExp ) );
	memset(sData, 0, sizeof( sData ) );
	sprintf(sLine, "PrivKeyBlock         : %s", PrivKeyBlock);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "EncryptKeyID          : %s", EncryptKeyID);
	trace_event(sLine, PROCESSING);

	trace_event("- Asymetric Key Decryption ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_DECRYPT_ASYM_KEY_FF6B);
	
	nLength = strlen(PrivKeyBlock);
	nLength = AsciiToHex(PrivKeyBlock,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_PRIV_KEY_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	AddC2PCmdInfoField (C2P_FN_RSA_PRIV_KEY_FMT_ID, &kC2PCmd, "02", GetFieldIdLength(C2P_FN_RSA_PRIV_KEY_FMT_ID,&kC2PCmd));
	
	AddC2PCmdInfoField (C2P_FN_ENC_KEY_ALGO_ID, &kC2PCmd, "04", GetFieldIdLength(C2P_FN_ENC_KEY_ALGO_ID,&kC2PCmd));
	
	/*sprintf(sData,"%4s","0000");
	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_USAGE, &kC2PCmd, sKeyRaw, nLength);
	
	AddC2PCmdInfoField (C2P_FN_TOKEN_RANGE, &kC2PCmd, "03", GetFieldIdLength(C2P_FN_TOKEN_RANGE,&kC2PCmd));
	
	AddC2PCmdInfoField (C2P_FN_KEY_EXP_DATE, &kC2PCmd, "00000000", GetFieldIdLength(C2P_FN_KEY_EXP_DATE,&kC2PCmd));
	
	AddC2PCmdInfoField (C2P_FN_KEY_ACTIVATION_DATE, &kC2PCmd, "00000000", GetFieldIdLength(C2P_FN_KEY_ACTIVATION_DATE,&kC2PCmd));
	
	AddC2PCmdInfoField (C2P_FN_GROUP_ID, &kC2PCmd, "0190", GetFieldIdLength(C2P_FN_GROUP_ID,&kC2PCmd)); ADD MOU20160512 BNT7*/
	
	nLength = strlen(EncryptKeyID);
	nLength = AsciiToHex(EncryptKeyID,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);

	/*AddC2PCmdInfoField (C2P_FN_DISTR_ACC_FLAG, &kC2PCmd, "00", GetFieldIdLength(C2P_FN_DISTR_ACC_FLAG,&kC2PCmd)); ADD MOU20160512 BNT7*/
	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Decrypt_Asym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Decrypt_Asym_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Decrypt_Asym_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_DECRYPT_ASYM_KEY_FF7B);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_PRIV_KEY_BLOC, &kC2PCmd, DecryptedAsymKey, &nLength);
	}
	else
	{
		trace_event("End   Decrypt_Asym_Key(NOK)",PROCESSING);
	     return(NOK);
	}
	
	trace_event("End   Decrypt_Asym_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function Ext_Public_Key                                                    */
/* -------------------                                                        */
/* Commande d�externalisation d�une cl� publique	                          */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int Ext_Public_Key(	    int   nIndexCtx,
		   			    char *sAsymKeyID,
						char *FingerPrintFMT,
						char *PubKeyRSA,
						char *CalculatedHash
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
	char				sKeyRaw[4096];
	char                            rPubKeyExp[128];
	char                            sData[256];
 /*-----Initialisation des variables-----*/

	trace_event("Start Ext_Public_Key()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sIdKey, 0, sizeof( sIdKey ) );
	memset(sKeyUsage, 0, sizeof( sKeyUsage ) );
	memset(rPubKeyExp, 0, sizeof( rPubKeyExp ) );
	memset(sData, 0, sizeof( sData ) );
	
	sprintf(sLine, "sAsymKeyID         : %s", sAsymKeyID);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "FingerPrintFMT          : %s", FingerPrintFMT);
	trace_event(sLine, PROCESSING);

	trace_event("- Asymetric Key Decryption ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_EXT_PUBLIC_KEY_F1);
	
	nLength = strlen(sAsymKeyID);
	nLength = AsciiToHex(sAsymKeyID,sKeyRaw,nLength);
	/*AddC2PCmdInfoField (C2P_FN_PRIV_KEY_BLOC, &kC2PCmd, sKeyRaw, nLength); ADD MOU20160512 BNT7*/
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);
	
	AddC2PCmdInfoField (C2P_FN_RSA_PUB_KEY_EXT_FMT_ID, &kC2PCmd, "01", GetFieldIdLength(C2P_FN_RSA_PUB_KEY_EXT_FMT_ID,&kC2PCmd));
	
	AddC2PCmdInfoField (C2P_FN_PUB_KEY_FINGERPRINT_REQ, &kC2PCmd, FingerPrintFMT, GetFieldIdLength(C2P_FN_PUB_KEY_FINGERPRINT_REQ,&kC2PCmd));
	
	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Ext_Public_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Ext_Public_Key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Ext_Public_Key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_EXT_PUBLIC_KEY_FF11);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_RSA_PUB_KEY_BLOC, &kC2PCmd, PubKeyRSA, &nLength);
		 GetC2PCmdInfoField (C2P_FN_PUB_KEY_HASH, &kC2PCmd, CalculatedHash, &nLength);
	}
	else
	{
		trace_event("End   Ext_Public_Key(NOK)",PROCESSING);
	     return(NOK);
	}

	
	trace_event("End   Ext_Public_Key(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function RSA_Sig_Verification                                                       */
/* -------------------                                                        */
/* V�rification d�une signature par une cl� publique                          */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int RSA_Sig_Verification    (	        int   nIndexCtx,
                                        char *SigAlgoID,
										char *HashAlgoID,
										char *sRSAModulo,
										char *sRSAExponent,
										char *sSignatureData,
										char *sClearData,/*ICH19122015*/
										char *sSigDataOut,
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
	char				sKeyRaw[LG_MAX];
	char                sData[256];
	char                sWrapSig[LG_MAX];
	char                sRSACert[LG_MAX];

 /*-----Initialisation des variables-----*/

	trace_event("Start RSA_Sig_Verification()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sData, 0, sizeof( sData ) );
	memset(sWrapSig, 0, sizeof( sWrapSig ) );
	memset(sRSACert, 0, sizeof( sRSACert ) );
	
	sprintf(sLine, "sRSAModulo         : %s", sRSAModulo);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "sSignatureData          : %s", sSignatureData);
	trace_event(sLine, PROCESSING);

	/*Start ICH19122015*/
	sprintf(sLine, "sClearData          : %s", sClearData);
	trace_event(sLine, PROCESSING);
	/*End ICH19122015*/
	trace_event("- RSA Signature Verification ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_SIG_F6);
	
	AddC2PCmdInfoField (C2P_FN_SIG_VERIF_ALGO_ID, &kC2PCmd, SigAlgoID, GetFieldIdLength(C2P_FN_SIG_VERIF_ALGO_ID,&kC2PCmd));
	/*if (memcmp(SigAlgoID,"01",2)==0)
	AddC2PCmdInfoField (C2P_FN_HASH_ALGO, &kC2PCmd, HashAlgoID, GetFieldIdLength(C2P_FN_HASH_ALGO,&kC2PCmd)); ADD MOU20160512 BNT7*/
	
	BuildPublicKey(sRSAModulo,sRSAExponent,sRSACert);
	
	sprintf(sLine, "sRSACert         : %s", sRSACert);
	trace_event(sLine, PROCESSING);
	
	nLength = strlen(sRSACert);
	nLength = AsciiToHex(sRSACert,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_RSA_PUB_KEY_BLOC, &kC2PCmd, sKeyRaw, nLength);	

	/*Start ICH19122015*/
	nLength = strlen(sClearData);
	if(nLength>0)
	{
		nLength = AsciiToHex(sClearData,sKeyRaw,nLength);
		AddC2PCmdInfoField (C2P_FN_TRANS_DATA, &kC2PCmd, sKeyRaw, nLength);
	}

	/*End ICH19122015*/

	nLength = strlen(sSignatureData);
	nLength = AsciiToHex(sSignatureData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SIGNATURE, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   RSA_Sig_Verification(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   RSA_Sig_Verification(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   RSA_Sig_Verification(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_SIG_FF16);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_SIG_DATA, &kC2PCmd, sWrapSig, &nLength);
		 HexToAscii( sWrapSig, sSigDataOut, 2*nLength);
	}
	else
	{
		trace_event("End   RSA_Sig_Verification(NOK)",PROCESSING);
		return(NOK);
	}
	
	trace_event("End   RSA_Sig_Verification(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function RSA_Sig_Calculation                                               */
/* -------------------                                                        */
/* Signer des donn�es  par une cl� priv�e	     	      			          */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int RSA_Sig_Calculation(	int   nIndexCtx,
                            char *SigAlgoID,
							char *HashAlgoID,
							char *sRSAKeyID,
							char *sSignatureData,
							char *sSignatureOut,
							char *sComplSigOut,
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
	char				sKeyRaw[LG_MAX];
	char                sData[256];
	char                sWrapSigCompl[LG_MAX];
	char                sWrapSig[LG_MAX];
 /*-----Initialisation des variables-----*/

	trace_event("Start RSA_Sig_Calculation()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sData, 0, sizeof( sData ) );
	memset(sWrapSigCompl, 0, sizeof( sWrapSigCompl ) );
	memset(sWrapSig, 0, sizeof( sWrapSig ) );
	
	sprintf(sLine, "SigAlgoID         : %s", SigAlgoID);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "sRSAKeyID         : %s", sRSAKeyID);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "sSignatureData          : %s", sSignatureData);
	trace_event(sLine, PROCESSING);
	
	trace_event("- RSA Signature Calculation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_SIG_F5);
	
	AddC2PCmdInfoField (C2P_FN_SIG_ALGO_ID, &kC2PCmd, SigAlgoID, GetFieldIdLength(C2P_FN_SIG_ALGO_ID,&kC2PCmd));
	/*if (memcmp(SigAlgoID,"01",2)==0)
	AddC2PCmdInfoField (C2P_FN_HASH_ALGO, &kC2PCmd, HashAlgoID, GetFieldIdLength(C2P_FN_HASH_ALGO,&kC2PCmd)); ADD MOU20160512 BNT7*/
	
	
	nLength = strlen(sRSAKeyID);
	nLength = AsciiToHex(sRSAKeyID,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_KEY_ID, &kC2PCmd, sKeyRaw, nLength);	

	nLength = strlen(sSignatureData);
	nLength = AsciiToHex(sSignatureData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SIG_DATA, &kC2PCmd, sKeyRaw, nLength);		

	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   RSA_Sig_Calculation(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   RSA_Sig_Calculation(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   RSA_Sig_Calculation(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_SIG_FF15);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_SIGNATURE, &kC2PCmd, sWrapSig, &nLength);
		 HexToAscii( sWrapSig, sSignatureOut, 2*nLength);
		 GetC2PCmdInfoField (C2P_FN_TRANS_DATA, &kC2PCmd, sWrapSigCompl, &nLength);
		 HexToAscii( sWrapSigCompl, sComplSigOut, 2*nLength);
	}
	else
	{
		trace_event("End   RSA_Sig_Calculation(NOK)",PROCESSING);
		return(NOK);
	}

	trace_event("End   RSA_Sig_Calculation(OK)",PROCESSING);
	return(OK);

}
/******************************************************************************/
/* Function FingerPrint_Calcul                                                */
/* -------------------                                                        */
/* Commande de calcul d�empreinte par l�algorithme SHA-1	     	      	  */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int FingerPrint_Calcul(	int   nIndexCtx,
                            char *HashAlgoID,
							char *sFPrintDataIN,
							char *sFPrintDataOUT,
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
	char				sKeyRaw[LG_MAX];
	char                sData[256];
	char                sWrapSigCompl[LG_MAX];
	char                sWrapSig[LG_MAX];
 /*-----Initialisation des variables-----*/

	trace_event("Start FingerPrint_Calcul()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sKeyRaw, 0, sizeof( sKeyRaw ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sData, 0, sizeof( sData ) );
	memset(sWrapSigCompl, 0, sizeof( sWrapSigCompl ) );
	memset(sWrapSig, 0, sizeof( sWrapSig ) );
	
	sprintf(sLine, "HashAlgoID         : %s", HashAlgoID);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "sFPrintDataIN         : %s", sFPrintDataIN);
	trace_event(sLine, PROCESSING);
	
	trace_event("- Finger Print Calculation ---------------", PROCESSING);
	
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_FINGERPRINT_SHA_1_F7);
	
	nLength = strlen(sFPrintDataIN);
	nLength = AsciiToHex(sFPrintDataIN,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_FINGERPRINT_GEN_DATA, &kC2PCmd, sKeyRaw, nLength);

	/*AddC2PCmdInfoField (C2P_FN_HASH_ALGO, &kC2PCmd, HashAlgoID,GetFieldIdLength(C2P_FN_HASH_ALGO,&kC2PCmd));		ADD MOU20160512 BNT7**/

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   FingerPrint_Calcul(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   FingerPrint_Calcul(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   FingerPrint_Calcul(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_FINGERPRINT_SHA_1_FF17);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_FINGERPRINT, &kC2PCmd, sWrapSig, &nLength);
		 HexToAscii( sWrapSig, sFPrintDataOUT, 2*nLength);
	}
	else
	{
		trace_event("End   FingerPrint_Calcul(NOK)",PROCESSING);
		return(NOK);
	}

	trace_event("End   FingerPrint_Calcul(OK)",PROCESSING);
	return(OK);

}

int AsciiToBinC2P(const unsigned char *sSrc, unsigned char *sDst){

    long int binaryNumber,hexadecimalNumber=0,j=1,remainder;
    int  a[20]; 
	int  sStr[20];
    int  dec,i=0;
	char buffer[100];
	char cc[4];
    char sentence[100];
	
    memset(buffer, 0, sizeof( buffer ) );
	
    sscanf(sSrc,"%d",&dec); 
    while(dec>0) 
    { 
         a[i]=dec%2; 
         i++; 
         dec=dec/2;
    }
    int k=i-1;
	while (k>=0)
	{
	sprintf(cc,"%d",a[k]);
	memcpy(buffer+strlen(buffer),cc,1);
	k--;
	}
	
    sscanf(buffer,"%ld",&binaryNumber);
	
    while(binaryNumber!=0)
	{
    remainder=binaryNumber%10;
    hexadecimalNumber=hexadecimalNumber+remainder*j;
    j=j*2;
    binaryNumber=binaryNumber/10;
    }

    sprintf(sDst,"%lX",hexadecimalNumber);
    return 0;

}   
   

int verify_rsa_signature (int nIndexCtx, char *sPublicKey, char *pData, char *pSignature)
{
	return NOK;
}

int import_rsa_public_key (int nIndexCtx, char *pRsaKey, char *pMac)
{
	return NOK;
}

int GenerateKeyCheckValue(int nIndexCtx,char *pKeyLmk,char *pCheckValue)
{
	return NOK;
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
	return NOK;
}


int GenerateSignatureBlock(
	int              nIndexCtx,
	unsigned char    *pMessageData,
	int              pMessageDataLen,
	char             *pPrivKey,
	unsigned char    *pSignature,
	int              *pSignatureLen)
	{
		return NOK;

	}