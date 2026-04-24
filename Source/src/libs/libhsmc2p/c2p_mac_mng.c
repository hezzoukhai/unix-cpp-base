/********************************************************************************/
/*  (c) Hightech Payment Systems 2015                                          **/
/* Module                                                                      **/
/*      c2p_mac_mng.c                                                          **/
/*                                                                             **/
/* Description                                                                 **/
/*      MAC  Management pour C2P                                               **/
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
/* Function Seal_Calculation                                                   */
/* -------------------                                                        */
/* Calcul de sceau 	      			                                          */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int     Seal_Calculation     (
						     int nIndiceCtx,
						     char *pTAK,
                             char *pMessageData,
                             int   pMessageLen,
						     char *mac_data,
                             char *sHsmResultCode
						    )

{
	unsigned char		BuffSnd   [C2P_BUF_MAX_LEN];
	unsigned char		BuffRcv   [C2P_BUF_MAX_LEN];
	int					nResult,nLen;
	pid_t				nPid;
	char				sLine[MAX_LINE_TRC];
	char				sData[LG_MAX];
	char				sKeyRaw[LG_MAX];
	TSC2PCmd			kC2PCmd;
	int				    nLength;
	int				    nLengthSnd;
	int				    nLengthRcv;
	char				sReturnCode[32];
	char				sComplInfo[32];
    char                sWrapKey[256];

	trace_event("Start Seal_Calculation()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sWrapKey, 0, sizeof( sWrapKey ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	memset(sData, 0, sizeof( sData ) );
	
	sprintf(sLine, "pTAK                         : %s", pTAK);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "MESSAGE DATA               : %s", pMessageData);
	trace_event(sLine, PROCESSING);


	trace_event("- Seal  Calculation ---------------", PROCESSING);
	

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_HASH_F8);


	AddC2PCmdInfoField (C2P_FN_PAD_ALGO_ID, &kC2PCmd, "02", GetFieldIdLength(C2P_FN_PAD_ALGO_ID,&kC2PCmd));
		
	nLength = strlen(pTAK);
	nLength = AsciiToHex(pTAK,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
		sprintf(sLine, "Length-pMessageData               : %d", strlen(pMessageData));
	trace_event(sLine, PROCESSING);

	nLength = 2*strlen(pMessageData);
	nLen = HexToAscii(pMessageData,sData,nLength);
	
	sprintf(sLine, "sData               : %s", sData);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "Length-sData               : %d", strlen(sData));
	trace_event(sLine, PROCESSING);
	
	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_MAC_DATA, &kC2PCmd, sKeyRaw, nLength);

	memset(sData, 0, sizeof( sData ) );
	
    sprintf(sData,"%d",pMessageLen);

	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_MAC_LEN, &kC2PCmd, sKeyRaw, nLength);
	

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Seal_Calculation(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndiceCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Seal_Calculation(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Seal_Calculation(MALFUNCTION)",PROCESSING);
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
		 trace_event("End   Seal_Calculation(NOK)",PROCESSING); 
		 return(NOK);
	}
	trace_event("End   Seal_Calculation(OK)",PROCESSING);
	return(OK);
}
/******************************************************************************/
/* Function Seal_Verification                                                 */
/* -------------------                                                        */
/* Vérification de sceau	      			                                  */
/* MOU 16/11/2015															  */
/*                                                                            */
/******************************************************************************/
int     Seal_Verification    (int nIndiceCtx,
                             char *pTAK,
                             char *pMAC,
                             char *pMessageData,
                             int   pMessageLen,
                             char  *sHsmResultCode
						    )

{
	unsigned char		BuffSnd   [C2P_BUF_MAX_LEN];
	unsigned char		BuffRcv   [C2P_BUF_MAX_LEN];
	int					nResult,nLen;
	pid_t				nPid;
	char				sLine[MAX_LINE_TRC];
	char				sData[LG_MAX];
	char				sKeyRaw[LG_MAX];
	TSC2PCmd			kC2PCmd;
	int					nLength;
	int					nLengthSnd;
	int					nLengthRcv;
	char				sReturnCode[32];
	char				sComplInfo[32];


	trace_event("Start Seal_Verification()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sData, 0, sizeof( sData ) );
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	
	sprintf(sLine, "pTAK                         : %s", pTAK);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "MESSAGE DATA               : %s", pMessageData);
	trace_event(sLine, PROCESSING);


	trace_event("- Seal  Verification ---------------", PROCESSING);
	
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_HASH_F9);

	AddC2PCmdInfoField (C2P_FN_PAD_ALGO_ID, &kC2PCmd, "02", GetFieldIdLength(C2P_FN_PAD_ALGO_ID,&kC2PCmd));
		
	nLength = strlen(pTAK);
	nLength = AsciiToHex(pTAK,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	nLength = 2*strlen(pMessageData);
	nLen = HexToAscii(pMessageData,sData,nLength);
	
	sprintf(sLine, "sData               : %s", sData);
	trace_event(sLine, PROCESSING);
	
	sprintf(sLine, "Length-sData               : %d", strlen(sData));
	trace_event(sLine, PROCESSING);
	
	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_MAC_DATA, &kC2PCmd, sKeyRaw, nLength);

	
	memset(sData, 0, sizeof( sData ) );
	
    sprintf(sData,"%d",pMessageLen);

	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_MAC_LEN, &kC2PCmd, sKeyRaw, nLength);
	
	memset(sData, 0, sizeof( sData ) );
	memset(sData,'0',16);
    memcpy(sData, pMAC,8);
		
	nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_MAC, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Seal_Verification(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndiceCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   Seal_Verification(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   Seal_Verification(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_HASH_FF19);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sReturnCode, "0000", 4) != 0 )
	{	
		 trace_event("End   Seal_Verification(NOK)",PROCESSING); 
		 return(NOK);
	}
	
	trace_event("End   Seal_Verification(OK)",PROCESSING);
	return(OK);

}
