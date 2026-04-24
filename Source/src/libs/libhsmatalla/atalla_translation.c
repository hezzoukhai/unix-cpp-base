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
#include <hsm_atalla_define.h>
#include <hsm_atalla.h>
#include <resources.h>

/*****************************************************************************/
/* Fonction pin_translate                                                    */
/* ----------------------                                                    */
/* Description : Cette fonction effectue le transchiffrement du pin d'une ZPK*/
/*               vers une autre ZPK.                                         */
/*****************************************************************************/
int  pin_translate( int nIndexCtx,
					char* card,
					char* transkey,
					char* zpk,
					char* pin,
					char* format_in,
					char* format_out,
					char mode)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char	ligne[MAX_LINE_TRC];
	char	sIncFormat[1 + 1];
	char	sOutgFormat[1 + 1];
	char	sPBData[12 + 1];


	if(debug_option ==1)
		sprintf(ligne, "Start pin_translate(%.22s,%s,%s,%.16s,%.2s,%.2s,%c)",
		card, transkey, zpk, pin, format_in, format_out, mode);
	else
		sprintf(ligne, "Start pin_translate()");
	trace_event(ligne, PROCESSING);

	sprintf(ligne, "format_out:  %.2s", format_out);
	trace_event(ligne, PROCESSING);

	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );
	memset(sIncFormat,        	'\0', sizeof(sIncFormat) );
	memset(sOutgFormat,        	'\0', sizeof(sOutgFormat) );
	memset(sPBData,        		'\0', sizeof(sPBData) );



	if(debug_option ==1)
	{
		sprintf(ligne, "transkey pin_translate(%s)",transkey);
		trace_event(ligne, PROCESSING);
	}

	format_card_from_pan(sPBData, card);

	translate_pin_block_format(sIncFormat,format_in);
	translate_pin_block_format(sOutgFormat,format_out);

	nResult = command_335(BuffSnd,
						&nLengthSnd,
						"",
						sIncFormat,
						"",
						sOutgFormat,
						transkey,
						zpk,
						pin,
						sPBData,
						sPBData);
	if (nResult != OK)
	{
		trace_event("Err. In command_335()", ERROR);
		trace_event("End   pin_translate(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   pin_translate(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   pin_translate(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   pin_translate(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	nResult = command_435(BuffRcv,
						nLengthRcv,
						pin);
	if(nResult != OK)
	{
		trace_event("Err. In command_435()", ERROR);
		trace_event("End   pin_translate(NOK)", PROCESSING);
		return (nResult);
	}

	memset(BuffRcv,       	'\0', sizeof(BuffRcv));
	memset(BuffSnd,        	'\0', sizeof(BuffSnd) );
	memset(sIncFormat,      '\0', sizeof(sIncFormat) );
	memset(sOutgFormat,     '\0', sizeof(sOutgFormat) );
	memset(sPBData,        	'\0', sizeof(sPBData) );


	trace_event("End   pin_translate(OK)", PROCESSING);
	return (OK);
}




int TranslateAwkFromZmkToLmk (int nIndexCtx,char *Zmk,  char *AwkZmk,  char *AwkLmk , char *tmpFieldLen)
{
return NOK;
}

int GenerateTPKKey( int nIndexCtx,
					char *pTermNbr,
					char* sAcqBank,
					char *pTpkLmk,
					char *pTpkTmk,
					char* szCheckValue)
{
	char	lTmkLmk[256];
	char	lTmkZmk[256];
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;


	trace_event("Start GenerateTPKKey()",PROCESSING);

	memset(lTmkLmk,				'\0', sizeof(lTmkLmk));
	memset(lTmkZmk,				'\0', sizeof(lTmkZmk));
	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );

	nResult = GetTMK(nIndexCtx, pTermNbr,sAcqBank, lTmkLmk, lTmkZmk);
	if (nResult != OK)
	{
		trace_event("Err. In GetTMK()", ERROR);
		trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
		return (nResult);
	}

	/*nResult = command_10(BuffSnd,
						&nLengthSnd,
						"1PUNE000",
						lTmkLmk,
						"D");
	if (nResult != OK)
	{
		trace_event("Err. In command_10()", ERROR);
		trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_1A(BuffSnd,
						&nLengthSnd,
						"0",
						lTmkLmk,
						"G1PUNN000,16");
	if (nResult != OK)
	{
		trace_event("Err. In command_1A()", ERROR);
		trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateTPKKey(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateTPKKey(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateTPKKey(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	/*nResult = command_20(BuffRcv,
						nLengthRcv,
						pTpkLmk,
						pTpkTmk,
						szCheckValue);
	if(nResult != OK)
	{
		trace_event("Err. In command_20()", ERROR);
		trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_2A(BuffRcv,
						nLengthRcv,
						pTpkTmk,
						szCheckValue,
						pTpkLmk
						);
	if(nResult != OK)
	{
		trace_event("Err. In command_2A()", ERROR);
		trace_event("End   GenerateTPKKey(NOK)", PROCESSING);
		return (nResult);
	}


	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   GenerateTPKKey(OK)",PROCESSING);
	return OK;
}


int GenerateTMKKey(int nIndexCtx,
					char *pTermNbr,
					char* sAcqBank,
					char *pTmkLmk,
					char *pTmkTmk,
					char* szCheckValue)
{
	char	lTmkLmk[256];
	char	lTmkZmk[256];
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;

	trace_event("Start GenerateTMKKey()",PROCESSING);

	memset(lTmkLmk,				'\0', sizeof(lTmkLmk));
	memset(lTmkZmk,				'\0', sizeof(lTmkZmk));
	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );

	nResult = GetTMK(nIndexCtx, pTermNbr,sAcqBank, lTmkLmk, lTmkZmk);
	if (nResult != OK)
	{
		trace_event("Err. In GetTMK()", ERROR);
		trace_event("End   GenerateTMKKey(NOK)", PROCESSING);
		return (nResult);
	}

	/*nResult = command_10(BuffSnd,
						&nLengthSnd,
						"1KDNE000",
						lTmkLmk,
						"D");
	if (nResult != OK)
	{
		trace_event("Err. In command_10()", ERROR);
		trace_event("End   GenerateTMKKey(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_1A(BuffSnd,
						&nLengthSnd,
						"0",
						lTmkLmk,
						"G1KDNN000,16");
	if (nResult != OK)
	{
		trace_event("Err. In command_1A()", ERROR);
		trace_event("End   GenerateTMKKey(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateTMKKey(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateTMKKey(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateTMKKey(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	/*nResult = command_20(BuffRcv,
						nLengthRcv,
						pTmkLmk,
						pTmkTmk,
						szCheckValue);
	if(nResult != OK)
	{
		trace_event("Err. In command_20()", ERROR);
		trace_event("End   GenerateTMKKey(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_2A(BuffRcv,
						nLengthRcv,
						pTmkTmk,
						szCheckValue,
						pTmkLmk
						);
	if(nResult != OK)
	{
		trace_event("Err. In command_2A()", ERROR);
		trace_event("End   GenerateTMKKey(NOK)", PROCESSING);
		return (nResult);
	}

	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   GenerateTMKKey(OK)",PROCESSING);
	return OK;
}

int GenerateMACKey(int nIndexCtx,
					char *pTermNbr,
					char* sAcqBank,
					char *pMacLmk,
					char *pMacTmk,
					char* szCheckValue)
{
	char	lTmkLmk[256];
	char	lTmkZmk[256];
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;


	trace_event("Start GenerateMACKey()",PROCESSING);

	memset(lTmkLmk,				'\0', sizeof(lTmkLmk));
	memset(lTmkZmk,				'\0', sizeof(lTmkZmk));
	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );

	nResult = GetTMK(nIndexCtx, pTermNbr,sAcqBank, lTmkLmk, lTmkZmk);
	if (nResult != OK)
	{
		trace_event("Err. In GetTMK()", ERROR);
		trace_event("End   GenerateMACKey(NOK)", PROCESSING);
		return (nResult);
	}

	/*nResult = command_10(BuffSnd,
						&nLengthSnd,
						"1mFNE00m",
						lTmkLmk,
						"D");
	if (nResult != OK)
	{
		trace_event("Err. In command_10()", ERROR);
		trace_event("End   GenerateMACKey(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_1A(BuffSnd,
						&nLengthSnd,
						"0",
						lTmkLmk,
						"1mFNN00m,16");
	if (nResult != OK)
	{
		trace_event("Err. In command_1A()", ERROR);
		trace_event("End   GenerateMACKey(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateMACKey(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateMACKey(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateMACKey(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	/*nResult = command_20(BuffRcv,
						nLengthRcv,
						pMacLmk,
						pMacTmk,
						szCheckValue);
	if(nResult != OK)
	{
		trace_event("Err. In command_20()", ERROR);
		trace_event("End   GenerateMACKey(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_2A(BuffRcv,
						nLengthRcv,
						pMacTmk,
						szCheckValue,
						pMacLmk
						);
	if(nResult != OK)
	{
		trace_event("Err. In command_2A()", ERROR);
		trace_event("End   GenerateMACKey(NOK)", PROCESSING);
		return (nResult);
	}

	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   GenerateMACKey(OK)",PROCESSING);
	return OK;
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
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char	sCheckValue[6 + 1];


	trace_event("Start TpkFromZmkToLmk()",PROCESSING);

	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );
	memset(sCheckValue,        	'\0', sizeof(sCheckValue) );


	/*nResult = command_13(BuffSnd,
						&nLengthSnd,
						"",
						pZmkKey,
						pTpkKey);
	if (nResult != OK)
	{
		trace_event("Err. In command_13()", ERROR);
		trace_event("End   TpkFromZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_11B(BuffSnd,
						&nLengthSnd,
						"0",
						pTpkKey,
						pZmkKey);
	if (nResult != OK)
	{
		trace_event("Err. In command_11B()", ERROR);
		trace_event("End   TpkFromZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   TpkFromZmkToLmk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   TpkFromZmkToLmk(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   TpkFromZmkToLmk(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	/*nResult = command_23(BuffRcv,
						nLengthRcv,
						pTpkLmk,
						sCheckValue);
	if(nResult != OK)
	{
		trace_event("Err. In command_23()", ERROR);
		trace_event("End   TpkFromZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_21B(BuffRcv,
						nLengthRcv,
						pTpkLmk,
						sCheckValue);
	if(nResult != OK)
	{
		trace_event("Err. In command_21B()", ERROR);
		trace_event("End   TpkFromZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}

	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   TpkFromZmkToLmk(OK)",PROCESSING);
	return OK;
}


int  FctTAKKeyExchange(
	int  nIndexCtx,
	char *Zmk_Key,
	char *Tak_Zmk,
	char *Check_Value_In,
	char *Tak_Lmk,
	char *Check_Value_Out)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;


	trace_event("Start FctTAKKeyExchange()",PROCESSING);

	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );


	/*nResult = command_13(BuffSnd,
						&nLengthSnd,
						"",
						Zmk_Key,
						Tak_Zmk);
	if (nResult != OK)
	{
		trace_event("Err. In command_13()", ERROR);
		trace_event("End   FctTAKKeyExchange(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_11B(BuffSnd,
						&nLengthSnd,
						"0",
						Tak_Zmk,
						Zmk_Key);
	if (nResult != OK)
	{
		trace_event("Err. In command_11B()", ERROR);
		trace_event("End   FctTAKKeyExchange(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   FctTAKKeyExchange(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   FctTAKKeyExchange(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   FctTAKKeyExchange(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	/*nResult = command_23(BuffRcv,
						nLengthRcv,
						Tak_Lmk,
						Check_Value_Out);
	if(nResult != OK)
	{
		trace_event("Err. In command_23()", ERROR);
		trace_event("End   FctTAKKeyExchange(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_21B(BuffRcv,
						nLengthRcv,
						Tak_Lmk,
						Check_Value_Out);
	if(nResult != OK)
	{
		trace_event("Err. In command_21B()", ERROR);
		trace_event("End   FctTAKKeyExchange(NOK)", PROCESSING);
		return (nResult);
	}

	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   FctTAKKeyExchange(OK)",PROCESSING);
	return OK;


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

	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char	ligne[MAX_LINE_TRC];

	if(debug_option ==1)
		sprintf(ligne, "Start translate_key_scheme(%s, %s, %.16s)", Zmk_Key, Zpk_Zmk, Check_Value_In);
	else
		sprintf(ligne, "Start translate_key_scheme(%.16s)",Check_Value_In);

	trace_event(ligne, PROCESSING);

	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );

	/*nResult = command_13(BuffSnd,
						&nLengthSnd,
						"",
						Zmk_Key,
						Zpk_Zmk);
	if (nResult != OK)
	{
		trace_event("Err. In command_13()", ERROR);
		trace_event("End   translate_key_scheme(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_11B(BuffSnd,
						&nLengthSnd,
						"0",
						Zpk_Zmk,
						Zmk_Key);
	if (nResult != OK)
	{
		trace_event("Err. In command_11B()", ERROR);
		trace_event("End   translate_key_scheme(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   translate_key_scheme(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   translate_key_scheme(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   translate_key_scheme(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	/*nResult = command_23(BuffRcv,
						nLengthRcv,
						Zpk_Lmk,
						Check_Value_Out);
	if(nResult != OK)
	{
		trace_event("Err. In command_23()", ERROR);
		trace_event("End   translate_key_scheme(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_21B(BuffRcv,
							nLengthRcv,
							Zpk_Lmk,
							Check_Value_Out);
	if(nResult != OK)
	{
		trace_event("Err. In command_21B()", ERROR);
		trace_event("End   translate_key_scheme(NOK)", PROCESSING);
		return (nResult);
	}

	/* à revoir
	 * if (memcmp(Check_Value_Out, Check_Value_In, strlen(Check_Value_In)) != 0)
	{
		trace_event("CHECK VALUES ARE DIFFERENT", ERROR);
		trace_event("End   translate_key_scheme(CRYPTOGRAPHIC_ERROR)", PROCESSING);
		return (CRYPTOGRAPHIC_ERROR);
	}*/

	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   TpkFromZmkToLmk(OK)",PROCESSING);
	return OK;
}

int	export_key(
	int             nIndexCtx,
	char           *transKeyLmk,
	char           *keyLmk,
	char		   *keyType,
	char           *KeyZmk,
	char           *Check_Value_Out)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char	sKeyLmkAkb[75];


	trace_event("Start export_key()",PROCESSING);

	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );
	memset(sKeyLmkAkb,        	'\0', sizeof(sKeyLmkAkb) );

	nResult = command_1A(BuffSnd,
						&nLengthSnd,
						"0",
						transKeyLmk,
						keyLmk);
	if (nResult != OK)
	{
		trace_event("Err. In command_1A()", ERROR);
		trace_event("End   export_key(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   export_key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   export_key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   export_key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	nResult = command_2A(BuffRcv,
						nLengthRcv,
						KeyZmk,
						Check_Value_Out,
						sKeyLmkAkb);
	if(nResult != OK)
	{
		trace_event("Err. In command_2A()", ERROR);
		trace_event("End   export_key(NOK)", PROCESSING);
		return (nResult);
	}

	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   export_key(OK)",PROCESSING);
	return OK;


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
	return translate_key_scheme(nIndexCtx,Zmk_Key,Zpk_Zmk,Check_Value_In,Zpk_Lmk,Check_Value_Out,"");
}


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
	return OK;
}
