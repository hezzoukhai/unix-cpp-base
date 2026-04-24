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
#include <hsm_atalla_define.h>
#include <hsm_atalla.h>
#include <resources.h>
#include <cbae.h>

/******************************************************************************/
/* Function GenerateKeyKT                                                     */
/* -------------------                                                        */
/* Tirage de Cl� de Transport (KTK/KME)	     	      			              */
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
	
	return OK;
}

/******************************************************************************/
/* Function GenerateKeyKS                                                     */
/* -------------------                                                        */
/* Tirage de Cl� de scellement	(KSC/KTK)     	      			                      */
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
	return OK;
}

/******************************************************************************/
/* Function GenerateKeyKC                                                     */
/* -------------------                                                        */
/* Tirage de Cl� de chiffrement	(KT/KTK)     	      			              */
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
		/*start  AHA20220929 PLUTONL-4980*/
    char* keySchemeZmk;
    char* KeyBlockVersionId;
	return GenerateZPK(	nIndexCtx,
						DES_OPTION_TIRPLE,
						szZmkLmk,
						szKtkLmk,
						szKtkZmk,
						szCheckValue,
                        keySchemeZmk,
                        KeyBlockVersionId);
         /*end  AHA20220929 PLUTONL-4980*/
}
/********************************************************************************/
/* Fonction KCKSLmkToKtk                                                        */
/* ----------------------                                                       */
/* Description : Cette fonction effectue la translation des cl�s KC/KS sous LMK */
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

	return(OK);
}
int  ZakZmkToLmk(	int		nIndexCtx,
					char	*zmkLmk, 
					char	*zakZmk, 
					char	*check_in, 
					char	*zakLmk, 
					char	*check_out)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;


	trace_event("Start ZakZmkToLmk()",PROCESSING);

	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );


	/*nResult = command_13(BuffSnd,
						&nLengthSnd,
						"",
						zmkLmk,
						zakZmk);
	if (nResult != OK)
	{
		trace_event("Err. In command_13()", ERROR);
		trace_event("End   ZakZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_11B(BuffSnd,
						&nLengthSnd,
						"0",
						zakZmk,
						zmkLmk);
	if (nResult != OK)
	{
		trace_event("Err. In command_11B()", ERROR);
		trace_event("End   ZakZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
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
	/*nResult = command_23(BuffRcv,
						nLengthRcv,
						zakLmk,
						check_out);
	if(nResult != OK)
	{
		trace_event("Err. In command_23()", ERROR);
		trace_event("End   ZakZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_21B(BuffRcv,
							nLengthRcv,
							zakLmk,
							check_out);
	if(nResult != OK)
	{
		trace_event("Err. In command_21B()", ERROR);
		trace_event("End   ZakZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}


	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   ZakZmkToLmk(OK)",PROCESSING);
	return OK;

}
int  KCKSKtkToLmk(	int		nIndexCtx,
					char	mode,
					char	*ktkLmk, 
					char	*kcksKtk, 
					char	*check_in, 
					char	*kcksLmk, 
					char	*check_out)
{

	return(OK);

}
int  KtkZmkToLmk(	int		nIndexCtx,
					char	*zmkLmk, 
					char	*ktkZmk, 
					char	*check_in, 
					char	*ktkLmk, 
					char	*check_out)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;


	trace_event("Start KtkZmkToLmk()",PROCESSING);

	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );


	/*nResult = command_13(BuffSnd,
						&nLengthSnd,
						"",
						zmkLmk,
						ktkZmk);
	if (nResult != OK)
	{
		trace_event("Err. In command_13()", ERROR);
		trace_event("End   KtkZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_11B(BuffSnd,
						&nLengthSnd,
						"0",
						ktkZmk,
						zmkLmk);
	if (nResult != OK)
	{
		trace_event("Err. In command_11B()", ERROR);
		trace_event("End   KtkZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
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
	/*nResult = command_23(BuffRcv,
						nLengthRcv,
						ktkLmk,
						check_out);
	if(nResult != OK)
	{
		trace_event("Err. In command_23()", ERROR);
		trace_event("End   KtkZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}*/
	nResult = command_21B(BuffRcv,
							nLengthRcv,
							ktkLmk,
							check_out);
	if(nResult != OK)
	{
		trace_event("Err. In command_21B()", ERROR);
		trace_event("End   KtkZmkToLmk(NOK)", PROCESSING);
		return (nResult);
	}

	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );

	trace_event("End   KtkZmkToLmk(OK)",PROCESSING);
	return OK;
}
/******************************************************************************/
int
	gen_msg_auth_code(
	int nIndexCtx,
	unsigned char	format,
	unsigned char*	algorithm,
	unsigned char	padMethod,
	unsigned char*	keyTpe,
	unsigned char	*takzak_key,
	unsigned char	*mac_data,
	unsigned char	*message_mac,
	int				lg_message_mac,
	char			*sHsmResultCode)
{

	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char	sDataLen[4+1];;
	char	ligne[MAX_LINE_TRC];


	memset(ligne, '\0',sizeof(ligne));
	memset(BuffSnd,'\0',sizeof(BuffSnd));
	memset(BuffRcv,'\0',sizeof(BuffRcv));
	memset(sDataLen,'\0',sizeof(sDataLen));

	sprintf(ligne, "Start gen_msg_auth_code(%d)", lg_message_mac);
	trace_event(ligne, PROCESSING);

	if (debug_option == 1)
	{
		trace_event("---------------------------------------------", PROCESSING);
		sprintf(ligne, "takzak_key            : %.16s", takzak_key);
		trace_event(ligne, PROCESSING);
		trace_event("---------------------------------------------", PROCESSING);
	}

	sprintf(sDataLen, "%d", lg_message_mac);

	nResult = command_98(BuffSnd ,
						&nLengthSnd,
						takzak_key,
						"1",/*32-bit MAC*/
						"7",/*ISO - 9797-1 Algorithm 3*/
						"",
						"B",/*Binary Data*/
						sDataLen,
						message_mac,
						""
						);

	if (nResult != OK)
	{
		trace_event("Err. In command_98()", ERROR);
		trace_event("End   gen_msg_auth_code(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   gen_msg_auth_code(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   gen_msg_auth_code(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   gen_msg_auth_code(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_A8(BuffRcv,
						nLengthRcv,
						mac_data,
						sHsmResultCode);
	if(nResult != OK)
	{
		trace_event("Err. In command_A8()", ERROR);
		trace_event("End   gen_msg_auth_code(NOK)", PROCESSING);
		return (nResult);
	}

	trim(mac_data,strlen(mac_data));

	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );


	trace_event("End gen_msg_auth_code(OK)", PROCESSING);
	return (OK);
}

/***********************************************************/
int verify_msg_auth_code(
	int				nIndexCtx,
	unsigned char	format,
	unsigned char*	algorithm,
	unsigned char	padMethod,
	unsigned char*	keyTpe,
	unsigned char	*takzak_key,
	unsigned char	*mac_data,
	unsigned char	*message_mac,
	int				lg_message_mac,
	char			*sHsmResultCode)
{

	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char	sDataLen[4+1];;
	char	ligne[MAX_LINE_TRC];




	trace_event("Start verify_msg_auth_code()", PROCESSING);

	memset(ligne, '\0',sizeof(ligne));
	memset(BuffSnd,'\0',sizeof(BuffSnd));
	memset(BuffRcv,'\0',sizeof(BuffRcv));
	memset(sDataLen,'\0',sizeof(sDataLen));


	if (debug_option == 1)
	{
		trace_event("--------------------------------------------", PROCESSING);
		sprintf(ligne, "takzak_key        : %s", takzak_key);
		trace_event(ligne, PROCESSING);

		sprintf(ligne, "mac_data          : %.16s", mac_data);
		trace_event(ligne, PROCESSING);
		trace_event("--------------------------------------------", PROCESSING);
	}

	sprintf(sDataLen, "%d", lg_message_mac);

	nResult = command_99(BuffSnd ,
						&nLengthSnd,
						takzak_key,
						"1",/*32-bit MAC*/
						"7",/*ISO - 9797-1 Algorithm 3*/
						"",
						"B",/*Binary Data*/
						sDataLen,
						message_mac,
						mac_data,
						""
						);

	if (nResult != OK)
	{
		trace_event("Err. In command_99()", ERROR);
		trace_event("End   verify_msg_auth_code(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   verify_msg_auth_code(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   verify_msg_auth_code(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   verify_msg_auth_code(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_A9(BuffRcv,
						nLengthRcv,
						sHsmResultCode);

	if (nResult != OK)
	{
		switch (nResult) {
		case VERIFICATION_FAILED:
			trace_event("End   verify_msg_auth_code(VERIFICATION_FAILED)", PROCESSING);
			return (VERIFICATION_FAILED);

		case ERROR_COMMAND:
			trace_event("End verify_msg_auth_code(ERROR_COMMAND)", PROCESSING);
			return (HSM_MALFUNCTION);
		default:
			trace_event("End   verify_msg_auth_code(CRYPTOGRAPHIC_ERROR)",PROCESSING);
			return (CRYPTOGRAPHIC_ERROR);
		}

	}

	trace_event("End   verify_msg_auth_code(OK)", PROCESSING);
	return (OK);
}
/***********************************************************/

int
	verify_mac(
	int nIndiceCtx,
	unsigned char *tak_key,
	unsigned char *mac_data,
	unsigned char *message_mac,
	int lg_message_mac,
	char *sHsmResultCode)
{

	return verify_msg_auth_code(	nIndiceCtx,
                                    HSM_FORMAT_BINARY,
                                    HSM_MAC_SIZE_8HEX_ALGO_ISO_9797_3,
                                    HSM_PAD_MTHD_ISO_9797_1,
                                    HSM_KEY_TAK,
                                    tak_key,
                                    mac_data,
                                    message_mac,
                                    lg_message_mac,
									sHsmResultCode ) ;
}

/***********************************************************/
int
	generate_mac(
	int				nIndiceCtx,
	unsigned char	*tak_key,
	unsigned char	*mac_data,
	unsigned char	*message_mac,
	int				lg_message_mac,
	char			*sHsmResultCode)
{

	return gen_msg_auth_code(     nIndiceCtx,
                                HSM_FORMAT_BINARY,
                                HSM_MAC_SIZE_8HEX_ALGO_ISO_9797_3,
                                HSM_PAD_MTHD_ISO_9797_1,
                                HSM_KEY_TAK,
                                tak_key,
                                mac_data,
                                message_mac,
                                lg_message_mac,
                                sHsmResultCode ) ;
}

/***********************************************************/

int
	generate_mac_with_hmac(
	int				nIndexCtx,
	unsigned char	*tak_key,
	unsigned char	*tak_key2,
	unsigned char*	algorithm,
	unsigned char	*message_mac,
	unsigned char	*mac_data,
	char			*sHsmResultCode

)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char	sDataLen[4+1];;
	char	ligne[MAX_LINE_TRC];


	memset(ligne, '\0',sizeof(ligne));
	memset(BuffSnd,'\0',sizeof(BuffSnd));
	memset(BuffRcv,'\0',sizeof(BuffRcv));


	trace_event("Start generate_mac_with_hmac()", PROCESSING);

	if (debug_option == 1)
	{
		trace_event("---------------------------------------------", PROCESSING);
		sprintf(ligne, "takzak_key            : %.16s", tak_key);
		trace_event(ligne, PROCESSING);
		sprintf(ligne, "takzak_key2            : %.16s", tak_key2);
		trace_event(ligne, PROCESSING);
		trace_event("---------------------------------------------", PROCESSING);
	}


	nResult = command_39B(BuffSnd ,
						&nLengthSnd,
						tak_key,
						tak_key2,
						algorithm,
						message_mac
						);

	if (nResult != OK)
	{
		trace_event("Err. In command_39B()", ERROR);
		trace_event("End   generate_mac_with_hmac(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   generate_mac_with_hmac(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   generate_mac_with_hmac(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   generate_mac_with_hmac(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_49B(BuffRcv,
						nLengthRcv,
						mac_data,
						sHsmResultCode);
	if(nResult != OK)
	{
		trace_event("Err. In command_49B()", ERROR);
		trace_event("End   generate_mac_with_hmac(NOK)", PROCESSING);
		return (nResult);
	}


	memset(BuffRcv,       '\0', sizeof(BuffRcv));
	memset(BuffSnd,        '\0', sizeof(BuffSnd) );


	trace_event("End generate_mac_with_hmac(OK)", PROCESSING);
	return (OK);

}

