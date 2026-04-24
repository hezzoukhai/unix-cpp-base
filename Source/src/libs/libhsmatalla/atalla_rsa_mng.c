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

    return (OK);

}   
   

int verify_rsa_signature (int nIndexCtx, char *sPublicKey, char *pData, char *pSignature)
{
	char    BuffRcv   		[LG_MAX];
	char    BuffSnd   		[LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];
	char	sDataLen[4+1];


	sprintf(sLine, "Start verify_rsa_signature(%.12s,%.12s,%.12s)",sPublicKey,pData,pSignature);
	trace_event(sLine, TRACE);


	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine, 		'\0', sizeof(sLine));
	memset(sDataLen, 	'\0', sizeof(sDataLen));

	sprintf(sDataLen, "%lu", strlen(pData)/2);

	nResult = command_125(BuffSnd,
						&nLengthSnd,
						"3", /*SHA1*/
						"1", /*standard PKCS#1 v1.5 signature*/
						sDataLen,
						pData,
						pSignature,
						sPublicKey
						);
	if (nResult != OK)
	{
		trace_event("Err. In command_125()", ERROR);
		trace_event("End   verify_rsa_signature(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   verify_rsa_signature(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   verify_rsa_signature(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   verify_rsa_signature(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_225(BuffRcv,
						nLengthRcv);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   verify_rsa_signature(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End verify_rsa_signature(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   verify_rsa_signature(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}

	/* PCI */
	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(sLine,   '\0', sizeof(sLine));
	trace_event("End   verify_rsa_signature(OK)", PROCESSING);
	return (OK);
}

int import_rsa_public_key (int nIndexCtx, char *pRsaKey, char *sRSAKeyBlock, char *pMac, char *pModulus, char *pCaPublicKey, char *pSignature)
{
	char    BuffRcv   		[LG_MAX];
	char    BuffSnd   		[LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];

	sprintf(sLine, "Start import_rsa_public_key()");
	trace_event(sLine, PROCESSING);

	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine, 		'\0', sizeof(sLine));

	nResult = command_123(BuffSnd,
						&nLengthSnd,
						"K", /*Key Usage*/
						"3", /*SHA1*/
						"1", /*standard PKCS#1 v1.5 signature*/
						"010001", /*Exponent*/
						pModulus,
						pRsaKey,
						pSignature,
						pCaPublicKey
						);
	if (nResult != OK)
	{
		trace_event("Err. In command_123()", ERROR);
		trace_event("End   import_rsa_public_key(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   import_rsa_public_key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   import_rsa_public_key(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   import_rsa_public_key(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_223(BuffRcv,
						nLengthRcv,
						sRSAKeyBlock);
	if(nResult != OK)
	{
		trace_event("Err. In command_223()", ERROR);
		trace_event("End   import_rsa_public_key(NOK)", PROCESSING);
		return (nResult);
	}


	/* PCI */
	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(sLine,   '\0', sizeof(sLine));


	trace_event("End   import_rsa_public_key(OK)", PROCESSING);
	return OK;
}

int GenerateKeyCheckValue(int nIndexCtx,char *pKeyLmk,char *pCheckValue)
{
	char    BuffRcv   		[LG_MAX];
	char    BuffSnd   		[LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];

	return OK; /*We dont use this fonction for HSM Atalla*/

	sprintf(sLine, "Start GenerateKeyCheckValue()");
	trace_event(sLine, PROCESSING);

	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine, 		'\0', sizeof(sLine));

	nResult = command_7E(BuffSnd,
						&nLengthSnd,
						"V", /*The Working Key encrypts all zeros. The check digits are the leftmost 6 hexadecimal digits of the result.*/
						"", /*KeyUsage must be empty*/
						pKeyLmk);
	if (nResult != OK)
	{
		trace_event("Err. In command_7E()", ERROR);
		trace_event("End   GenerateKeyCheckValue(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateKeyCheckValue(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateKeyCheckValue(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateKeyCheckValue(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_8E(BuffRcv,
						nLengthRcv,
						pCheckValue);
	if(nResult != OK)
	{
		trace_event("Err. In command_8E()", ERROR);
		trace_event("End   GenerateKeyCheckValue(NOK)", PROCESSING);
		return (nResult);
	}


	/* PCI */
	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(sLine,   '\0', sizeof(sLine));

	trace_event(sLine, PROCESSING);
	trace_event("End   GenerateKeyCheckValue(OK)", PROCESSING);
	return OK;

}

int ExportKeyUnderRSA(
	int              nIndexCtx,
	unsigned char    *pRsaKey,
	char             *pDesKey,
	char			 *pKeyType,
	char             *pCheckValue16,
	unsigned char    *pExportedKey,
	int              *pExportedKeyLen,
	char			 *sCheckValue)
{
	char    BuffRcv   		[LG_MAX];
	char    BuffSnd   		[LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];

	sprintf(sLine, "Start ExportKeyUnderRSA()");
	trace_event(sLine, PROCESSING);

	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine, 		'\0', sizeof(sLine));

	nResult = command_12F(BuffSnd,
						&nLengthSnd,
						"D", /*key-3DES (double-length) key*/
						"0",
						"",
						pRsaKey,
						""
						);
	if (nResult != OK)
	{
		trace_event("Err. In command_12F()", ERROR);
		trace_event("End   ExportKeyUnderRSA(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   ExportKeyUnderRSA(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   ExportKeyUnderRSA(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   ExportKeyUnderRSA(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_22F(BuffRcv,
						nLengthRcv,
						pDesKey,
						sCheckValue,
						pExportedKey);
	if(nResult != OK)
	{
		trace_event("Err. In command_22F()", ERROR);
		trace_event("End   ExportKeyUnderRSA(NOK)", PROCESSING);
		return (nResult);
	}

	*pExportedKeyLen = strlen(pExportedKey) /2;

	/* PCI */
	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(sLine,   '\0', sizeof(sLine));

	sprintf(sLine, "End   ExportKeyUnderRSA(%.32s,%.32s)", pRsaKey, pDesKey);
	trace_event(sLine, PROCESSING);
	trace_event("End   ExportKeyUnderRSA(OK)", PROCESSING);
	return OK;
}


int GenerateSignatureBlock(
	int              nIndexCtx,
	unsigned char    *pMessageData,
	int              pMessageDataLen,
	char             *pPrivKey,
	unsigned char    *pSignature,
	int              *pSignatureLen)
	{
	char    BuffRcv   		[LG_MAX];
	char    BuffSnd   		[LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];
	char	sMsgDataLen[5+1];

	sprintf(sLine, "Start GenerateSignatureBlock()");
	trace_event(sLine, PROCESSING);

	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine, 		'\0', sizeof(sLine));
	memset(sMsgDataLen, '\0', sizeof(sMsgDataLen));

	sprintf(sMsgDataLen, "%d", pMessageDataLen);

	nResult = command_124(BuffSnd,
						&nLengthSnd,
						"3", /*SHA1*/
						"1", /*standard PKCS#1 v1.5 signature*/
						sMsgDataLen,
						pMessageData,
						pPrivKey
						);
	if (nResult != OK)
	{
		trace_event("Err. In command_124()", ERROR);
		trace_event("End   GenerateSignatureBlock(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateSignatureBlock(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   GenerateSignatureBlock(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   GenerateSignatureBlock(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_224(BuffRcv,
						nLengthRcv,
						pSignature);
	if(nResult != OK)
	{
		trace_event("Err. In command_224()", ERROR);
		trace_event("End   GenerateSignatureBlock(NOK)", PROCESSING);
		return (nResult);
	}

	*pSignatureLen = strlen(pSignature) /2;

	/* PCI */
	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(sLine,   '\0', sizeof(sLine));


	trace_event("End   GenerateSignatureBlock(OK)", PROCESSING);
	return OK;

	}
