/********************************************************************************/
/*  (c) Hightech Payment Systems 2015                                          **/
/* Module                                                                      **/
/*      c2p_translation.c                                                      **/
/*                                                                             **/
/* Description                                                                 **/
/*      Translation tools pour C2P                                             **/
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

/*****************************************************************************/
/* Fonction pin_translate                                                    */
/* ----------------------                                                    */
/* Description : Cette fonction effectue le transchiffrement du pin d'une ZPK*/
/*               vers une autre ZPK.                                         */
/*****************************************************************************/
int  pin_translate (int nIndiceCtx,char *card,char *transkey,char *zpk,char *pin,char *format_in,char *format_out,char mode,char *piste2 ,char *pin_out)
{
  int	  retour;
  int	  nLength;
  char    BuffSnd       [512];
  char    BuffRcv       [512];
  char    sLine[MAX_LINE_TRC];
  char    sHsmResultCode[5];
  pid_t   nPid;
  char    sPan 	       [ 22 + 1];
  char    sExpiryDate  [  4 + 1];
  char    sServiceCode [  3 + 1];
  char    sPvv         [  5 + 1]; 
  char    sCvv1        [  3 + 1];
  char	  sTrack2[64];
  char	  sTrack2_raw[64];
  char	  sData[256];
  char    sKeyRaw[32];
  TSC2PCmd		  kC2PCmd;
  int					nResult;
  int					nLengthSnd;
  int					nLengthRcv;
  char				    sReturnCode[32];
  char				    sComplInfo[32];
  char                  sWrapSig[ 64 + 1];
  
  memset(sHsmResultCode , 0 , 5 );
  sprintf(sLine, "Start pin_translate(%.22s,%.16s,%.16s,%.16s,%.2s,%.2s,%c, %.37s)",
        card, transkey, zpk, pin, format_in, format_out, mode,piste2);
  trace_event(sLine, PROCESSING);

  sprintf(sLine, "formatou  %.2s", format_out);
  trace_event(sLine, PROCESSING);
  
  memset ( BuffSnd,        0, sizeof ( BuffSnd) );
  memset ( BuffRcv,        0, sizeof ( BuffRcv) );
  memset ( sPan,  		   0, sizeof ( sPan ));
  memset ( sExpiryDate,    0, sizeof ( sExpiryDate));
  memset ( sServiceCode,   0, sizeof ( sServiceCode));
  memset ( sPvv,  		   0, sizeof ( sPvv ));
  memset ( sCvv1,  		   0, sizeof ( sCvv1));
  memset(sReturnCode, 0, sizeof( sReturnCode ) );
  memset(sComplInfo, 0, sizeof( sComplInfo ) );
  memset(sWrapSig, 0, sizeof( sWrapSig ) );
  
  nResult = InitC2PCmd(&kC2PCmd, C2P_MN_PIN_TRSLT_FB);
  
  GetTrack2Data(piste2,sPan,sExpiryDate,sServiceCode,sPvv,sCvv1);
  
  sprintf(sLine, "sPVV [%s] scvv1 [%s] servicecode [%s] sExpiryDate [%s]",sPvv, sCvv1, sServiceCode, sExpiryDate);
        
  trace_event(sLine, PROCESSING);
  
  /*BNT_command_FB (BuffSnd,transkey, zpk, pin, format_in, format_out, card, sPvv);*/
  
  	nLength = strlen(transkey);
	nLength = AsciiToHex(transkey,sKeyRaw,nLength);
    AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(zpk);
	nLength = AsciiToHex(zpk,sKeyRaw,nLength);
    AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(pin);
	nLength = AsciiToHex(pin,sKeyRaw,nLength);
  	AddC2PCmdInfoField (C2P_FN_ENC_PIN_BLK, &kC2PCmd, sKeyRaw, nLength);
	sData[0] = '0';
	AddC2PCmdInfoField (C2P_FN_DEC_MODE, &kC2PCmd, sData, GetFieldIdLength(C2P_FN_DEC_MODE,&kC2PCmd));
	
	AddC2PCmdInfoField (C2P_FN_PIN_BLK_FMT, &kC2PCmd, format_in, GetFieldIdLength(C2P_FN_PIN_BLK_FMT,&kC2PCmd));
	
	AddC2PCmdInfoField (C2P_FN_PIN_BLK_FMT, &kC2PCmd, format_out, GetFieldIdLength(C2P_FN_PIN_BLK_FMT,&kC2PCmd));
	
    sprintf(sTrack2, "0%.16sD%.4s%.3s10000%.3s00000",sPan, sExpiryDate, sServiceCode, sCvv1);
	nLength = strlen(sTrack2);
	nLength = AsciiToHex(sTrack2,sTrack2_raw,nLength);
	
	AddC2PCmdInfoField (C2P_FN_ISO_TRACK_2, &kC2PCmd, sTrack2_raw, GetFieldIdLength(C2P_FN_ISO_TRACK_2,&kC2PCmd));

 	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);

	nResult = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   pin_translate(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndiceCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   pin_translate(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   pin_translate(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_PIN_TRSLT_FF1B);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);
	if ( memcmp(sReturnCode, "0000", 4) == 0 )
	{
	     GetC2PCmdInfoField (C2P_FN_ENC_PIN_BLK, &kC2PCmd, sWrapSig, &nLength);
		 HexToAscii( sWrapSig, pin_out, 2*nLength);
	}
    trace_event("End   pin_translate()", PROCESSING);
    return(OK);
}



int TranslateAwkFromZmkToLmk (int nIndexCtx,char *Zmk,  char *AwkZmk,  char *AwkLmk , char *tmpFieldLen)
{
return NOK;
}

int  key_exchange_scheme(
						int  nIndexCtx,
						char *Zmk_Key,
						char *Zpk_Zmk,
						char *Check_Value_In,
						char *Zpk_Lmk,
						char *Check_Value_Out)
{

return NOK;
}


int 
verify_mac(
	   int nIndiceCtx,
	   unsigned char *tak_key,
	   unsigned char *mac_data,
	   unsigned char *message_mac,
	   int lg_message_mac,
	   char *sHsmResultCode)
{
	   return NOK;
}



int 
generate_mac(
	     int nIndiceCtx,
	     unsigned char *tak_key,
	     unsigned char *mac_data,
	     unsigned char *message_mac,
	     int lg_message_mac,
	     char *sHsmResultCode)
{

	return NOK;
}



int GenerateTPKKey( int nIndexCtx,char *pTermNbr,char* sAcqBank, char *pTpkLmk,char *pTpkTmk, char* szCheckValue)
{
	return NOK;
}


int GenerateTMKKey(int nIndexCtx,char *pTermNbr,char* sAcqBank, char *pTmkLmk,char *pTmkTmk, char* szCheckValue)
{
	return NOK;
}

int GenerateMACKey(int nIndexCtx,char *pTermNbr,char* sAcqBank, char *pMacLmk,char *pMacTmk, char* szCheckValue)
{
	return NOK;
}

int	GeneratePosIsoBDKKey(int nIndexCtx,char *pResourceId, char *pTermNbr, char *pBdkLmk, char *pBdkZmk, char *pCheckValue)
{
	return NOK;
}

int 
TpkFromZmkToLmk(
	int             nIndexCtx,
	unsigned char   pZmkKey[],
	unsigned char   pTpkKey[],
	unsigned char   pTpkLmk[])
{
	return NOK;
}


int  FctTAKKeyExchange(
	int  nIndexCtx,
	char *Zmk_Key,
	char *Tak_Zmk,
	char *Check_Value_In,
	char *Tak_Lmk,
	char *Check_Value_Out)
{
	return NOK;
}



int	PinFromZpkToLmk(
	int				nIndexCtx,
	unsigned char	pZpkKey		[],
	unsigned char	pPinBlk		[],
	unsigned char	pPinBlkFmt	[],
	unsigned char	pCardNbr	[],
	unsigned char	pPinLmk		[])
{
	return NOK;

}