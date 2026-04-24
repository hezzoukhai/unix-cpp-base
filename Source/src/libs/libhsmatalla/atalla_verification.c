#include <p7_common.h>
#include <define.h>
#include <iso8583.h>
#include <hsm_atalla_define.h>
#include <hsm_atalla.h>
#include <hsm_inc.h>
#include <queue.h>
#include <stdio.h>
#include <resources.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <tlv_autho.h>
#include <string.h>
#include <p7_define.h>
#include <security_flags.h>
#include <p7_sid.h>

#define MAX_ISS_SCRIPT				3


int     check_cvv_data  (
							int nIndexCtx,
							char *Pan, 
							char *CvkA, 
							char *CvkB, 
							char *expiry_date, 
							char *service_code, 
							char *cvv,
							char *sHsmResultCode
							)

{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];

	trace_event("Start check_cvv_data()", PROCESSING);

	memset(BuffRcv,       		'\0', sizeof(BuffRcv));
	memset(BuffSnd,        		'\0', sizeof(BuffSnd) );
	memset(sLine,        		'\0', sizeof(sLine) );

	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %s", Pan);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "SERVICE CODE                : %.3s", service_code);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "EXPIRY DATE                 : %.4s", expiry_date);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "CVKA                        : %.48s", CvkA);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "CVKB                        : %.48s", CvkB);
		trace_event(sLine, PROCESSING);

		if (memcmp(service_code, "999", 3) == 0)
			sprintf(sLine, "ICVV                        : %.3s", cvv);
		else
			sprintf(sLine, "CVV1                        : %.3s", cvv);

		trace_event(sLine, PROCESSING);
		trace_event("- CVV Verification ---------------", PROCESSING);
	}

	nResult = command_5E(BuffSnd,
						&nLengthSnd,
						"3",
						"",
						Pan,
						service_code,
						expiry_date,
						CvkA,
						CvkB,
						cvv
						);
	if (nResult != OK)
	{
		trace_event("Err. In command_5E()", ERROR);
		trace_event("End   check_cvv_data(NOK)", PROCESSING);
		return (nResult);
	}
	trace_event(BuffSnd, PROCESSING);

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_cvv_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	nResult = command_6E(BuffRcv,
						nLengthRcv,
						sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   check_cvv_data(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End check_cvv_data(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   check_cvv_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}

	memset(BuffRcv,      '\0', sizeof(BuffRcv));
	memset(BuffSnd,      '\0', sizeof(BuffSnd) );
	memset(sLine,        '\0', sizeof(sLine) );

	trace_event("End   check_cvv_data(OK)", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction check_aav_data.                                                  */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le AAV recu.                           */
/******************************************************************************/
int     check_aav_data  (int nIndexCtx,
						char *Pan,
						/*int  nCvki,*/
						char *DSTransactionID,
						char *TxCurr,
						char *TxAmount,
						char *MerchantName,
						char  sProgramProtocol,
						char *aav_data_b64,
						char *sHsmResultCode
						)

{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];
	char	aav_data[256];
	char 	aav_value[16];
	char 	tsn[16];
	char 	auth_method[16];
	char 	control_byte[16];
	char	aav_key[SIZE_KEY_TDES + 1];
	char	key_id[16];
	char	service_code[16];
	char	str_temp[16];
	char	CvkA[SIZE_KEY_TDES + 1];
	char	CvkB[SIZE_KEY_TDES + 1];
	int		int_temp, i;

	memset(sLine, 			'\0', sizeof(sLine));
	memset(BuffSnd, 		'\0', sizeof(BuffSnd));
	memset(BuffRcv, 		'\0', sizeof(BuffRcv));
	memset(aav_data, 		'\0', sizeof(aav_data));
	memset(aav_value, 		'\0', sizeof(aav_value));
	memset(tsn, 			'\0', sizeof(tsn));
	memset(auth_method, 	'\0', sizeof(auth_method));
	memset(control_byte, 	'\0', sizeof(control_byte));
	memset(aav_key, 		'\0', sizeof(aav_key));
	memset(key_id, 			'\0', sizeof(key_id));
	memset(service_code, 	'\0', sizeof(service_code));
	memset(CvkA, 			'\0', sizeof(CvkA));
	memset(CvkB, 			'\0', sizeof(CvkB));
	memset(str_temp, 		'\0', sizeof(str_temp));


	trace_event("Start check_aav_data()", PROCESSING);


	nResult = Base64_Decode(nIndexCtx, aav_data_b64, aav_data);

	/*CONTROL BYTE*/
	/*NAK-START SIB ECOM MNG 06032017*/
	/*Issuers should not perform SecureCode validation on static AAVs in DE 48,
	*        subelement 43 nor on attempts AAV generated by MasterCard */
	if(aav_data[1] == '6' || memcmp(aav_data_b64, "kL", 2) == 0 || memcmp(aav_data_b64, "kM", 2) == 0 ||	/*AMER20180324 PROD00054205: MCI Enh18.Q2 AN1085: Added the check of kL and kM for attempt CAVV generated with SPA2 algorithm*/
	memcmp(aav_data_b64, "kE", 2) == 0 || memcmp(aav_data_b64, "kF", 2) == 0 || memcmp(aav_data_b64, "kN", 2) == 0)	/*AMER20190219 PROD00065991: MCI Enh19.Q2 AN2113*/
	{
		trace_event("AAV GENERATED BY NETWORK(NOK)", PROCESSING);
		trace_event("End check_aav_data(NOK)", PROCESSING);
		return NOK;
	}
	/*NAK-END SIB ECOM MNG 06032017*/

	/*PROD00073147 Start SPA2*/

	if(sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_0  || aav_data_b64[0] == 'k'){

		nResult = MC_Spa2_AAV_implementation(nIndexCtx, aav_data, Pan, /*nCvki,*/DSTransactionID, TxCurr, TxAmount, MerchantName, sHsmResultCode);/*ASO20200403*/

		if(nResult == OK)
		{
			trace_event("End   check_aav_data(OK)", PROCESSING);
			return(OK);
		}
		else
		{
			switch (nResult)
			{
				case VERIFICATION_FAILED:
					trace_event("End   check_aav_data(VERIFICATION_FAILED)", PROCESSING);
					return(VERIFICATION_FAILED);

				case ERROR_COMMAND:
					trace_event("End check_aav_data(ERROR_COMMAND)", PROCESSING);
					return(HSM_MALFUNCTION);

				default:
					trace_event("End   check_aav_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
					return(CRYPTOGRAPHIC_ERROR);
			}
		}

	}

	/*PROD00073147 END*/

	memcpy(str_temp, aav_data, 2);
	int_temp = strtol(str_temp, NULL, 16);
	sprintf(str_temp, "%d", int_temp);

	memcpy(control_byte, "00", 2);

	if (strlen(str_temp) > 2)
		memcpy(control_byte, str_temp + 1, 2);
	else
		memcpy(control_byte + (2 - strlen(str_temp)), str_temp, strlen(str_temp));

	/*AUTHENTICATION METHOD*/
	memcpy(auth_method, aav_data + 20, 1);

	/*BIN Key Id*/
	memcpy(key_id, aav_data + 21, 1);


	/*TRANSACTION SEQUENCE NUMBER*/

	memcpy(str_temp, aav_data + 26, 4);
	int_temp = strtol(str_temp, NULL, 16);

	sprintf(str_temp, "%d", int_temp);

	memcpy(tsn, "0000", 4);

	if (strlen(str_temp) > 4)
		memcpy(tsn, str_temp + 1, 4);
	else
		memcpy(tsn + (4 - strlen(str_temp)), str_temp, strlen(str_temp));

	/*UCAF/AAV*/
	memcpy(aav_value, aav_data + 31, 3);


	/*AAV KEY */
	nResult = Get_UCAF_Key(nIndexCtx, Pan,/*nCvki,*/ key_id, aav_key);
	if ((nResult != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		return(HSM_UNAVAILABLE);
	}


	memcpy(CvkA,aav_key,strlen(aav_key));





	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %s", Pan);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "AUTHENTICATION METHOD       : %.1s", auth_method);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "CONTROL BYTE                : %.2s", control_byte);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "TRANSACTION SEQUENCE NUMBER : %.4s", tsn);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "AAV_KEY                     : %.32s", aav_key);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "UCAF/AAV                    : %.3s", aav_value);
		trace_event(sLine, PROCESSING);
		trace_event("- UCAF / AAV Verification ---------------", PROCESSING);
	}
	else
		trace_event("- UCAF / AAV Verification ---------------", PROCESSING);

	memcpy(service_code, auth_method, 1);
	memcpy(service_code + 1, control_byte, 2);


	nResult = command_5E(BuffSnd,
						&nLengthSnd,
						"3",
						"",
						Pan,
						service_code,
						tsn,
						CvkA,
						CvkB,
						aav_value
						);
	if (nResult != OK)
	{
		trace_event("Err. In command_5E()", ERROR);
		trace_event("End   check_cvv_data(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_cvv_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	nResult = command_6E(BuffRcv,
						nLengthRcv,
						sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   check_cvv_data(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End check_cvv_data(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   check_cvv_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}



	/* PCI */
	memset(sLine, '\0', sizeof(sLine));
	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(aav_data, '\0', sizeof(aav_data));
	memset(aav_value, '\0', sizeof(aav_value));
	memset(tsn, '\0', sizeof(tsn));
	memset(auth_method, '\0', sizeof(auth_method));
	memset(control_byte, '\0', sizeof(control_byte));
	memset(aav_key, '\0', sizeof(aav_key));
	memset(key_id, '\0', sizeof(key_id));
	memset(service_code, '\0', sizeof(service_code));
	memset(CvkA, '\0', sizeof(CvkA));
	memset(CvkB, '\0', sizeof(CvkB));
	memset(str_temp, '\0', sizeof(str_temp));

	trace_event("End   check_aav_data(OK)", PROCESSING);
	return(OK);
 }


/******************************************************************************/
/* Fonction check_cavv_data.                                                  */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le cavv recu.                           */
/******************************************************************************/
int     check_cavv_data  (
              int nIndexCtx,
              char *Pan,  
              char *cavv_data,
              char *sHsmResultCode
              )

{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char 	sLine[MAX_LINE_TRC];
	char	cavv_value[4];
	char 	atn[5];
	char 	autho_result[2];
	char 	second_factor[3];
	char	cavv_key[SIZE_KEY_TDES + 1];
	char	service_code[4];
	char	CvkA[SIZE_KEY_TDES + 1];
	char	CvkB[SIZE_KEY_TDES + 1];
	char	key_indicator[3];

	memset(sLine, '\0', sizeof(sLine));
	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(cavv_value, '\0', sizeof(cavv_value));
	memset(atn, '\0', sizeof(atn));
	memset(autho_result, '\0', sizeof(autho_result));
	memset(second_factor, '\0', sizeof(second_factor));
	memset(cavv_key, '\0', sizeof(cavv_key));
	memset(service_code, '\0', sizeof(service_code));
	memset(CvkA, '\0', sizeof(CvkA));
	memset(CvkB, '\0', sizeof(CvkB));
	memset(key_indicator, '\0', sizeof(key_indicator));



	trace_event("Start check_cavv_data()", PROCESSING);



	memcpy(autho_result, cavv_data + 1, 1);
	memcpy(second_factor, cavv_data + 2, 2);
	memcpy(key_indicator, cavv_data + 4, 2);
	memcpy(cavv_value, cavv_data + 7, 3);
	memcpy(atn, cavv_data + 10, 4);

	/*Start AMER20180324 PROD00054205: VISA Enh18.Q2 Art 1.1: Don't check the CAVV if it's an attempt*/
	if( (autho_result[0] == '7' || autho_result[0] == '8') &&
		(memcmp(key_indicator, "10", 2) == 0  || memcmp(key_indicator, "11", 2) == 0))
	{
		trace_event("CAVV GENERATED BY NETWORK: Not to be checked", PROCESSING);
		trace_event("End check_cavv_data(NOK)", PROCESSING);
		return NOK;
	}
	/*End AMER20180324 PROD00054205: VISA Enh18.Q2 Art 1.1: Don't check the CAVV if it's an attempt*/

	nResult = Get_CAVV_Key(nIndexCtx, Pan, key_indicator, CvkA, CvkB);

	if ((nResult != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		return(HSM_UNAVAILABLE);
	}


	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %s", Pan);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "RESULT CODE                 : %.1s", autho_result);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "SECOND FACTOR               : %.2s", second_factor);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "ATN                         : %.4s", atn);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "CAVV_KEY                    : %.48s||%.48s", CvkA, CvkB);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "CAVV                        : %.3s", cavv_value);
		trace_event(sLine, PROCESSING);
		trace_event("- CAVV Verification ---------------", PROCESSING);
	}
	else
		trace_event("- CAVV Verification ---------------", PROCESSING);

	memcpy(service_code, autho_result, 1);
	memcpy(service_code + 1, second_factor, 2);

	nResult = command_5E(BuffSnd,
						&nLengthSnd,
						"3",
						"",
						Pan,
						service_code,
						atn,
						CvkA,
						CvkB,
						cavv_value
						);
	if (nResult != OK)
	{
		trace_event("Err. In command_5E()", ERROR);
		trace_event("End   check_cavv_data(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cavv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cavv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_cavv_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}
	nResult = command_6E(BuffRcv,
						nLengthRcv,
						sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   check_cavv_data(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End check_cavv_data(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   check_cavv_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}

	memset(BuffRcv,      '\0', sizeof(BuffRcv));
	memset(BuffSnd,      '\0', sizeof(BuffSnd) );
	memset(sLine,        '\0', sizeof(sLine) );
	memset(cavv_value, 0, sizeof(cavv_value));
	memset(atn, 0, sizeof(atn));
	memset(autho_result, 0, sizeof(autho_result));
	memset(second_factor, 0, sizeof(second_factor));
	memset(cavv_key, 0, sizeof(cavv_key));
	memset(service_code, 0, sizeof(service_code));
	memset(CvkA, 0, sizeof(CvkA));
	memset(CvkB, 0, sizeof(CvkB));
	memset(key_indicator, 0, sizeof(key_indicator));

	trace_event("End   check_cavv_data(OK)", PROCESSING);
	return(OK);
}


int check_dcvv_data(
	int nIndexCtx,
	char *sMcvkc,
	char *sPan,
	char *sExpiryDate,
	char *sServiceCode,
	char *sAtc,
	char *sDcvv,
	char *sHsmResultCode
	)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];
	char	Atc[8+1];

	trace_event("Start check_dcvv_data()", PROCESSING);

	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(sLine,   '\0', sizeof(sLine));
	memset(Atc,   '\0', sizeof(Atc));

	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %s", sPan);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "EXPIRY DATE        			: %s", sExpiryDate);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "SERVICE CODE       			: %s", sServiceCode);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "MKCVKC KEY       			: %s", sMcvkc);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "ATC        					: %s", sAtc);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "DCVV        				: %s", sDcvv);
		trace_event(sLine, PROCESSING);
	}

	sprintf(Atc , "%d", atoi(sAtc));
	nResult = command_357(BuffSnd,
						&nLengthSnd,
						sMcvkc,
						sPan,
						"00",
						sExpiryDate,
						sServiceCode,
						Atc,
						sDcvv
						);
	if (nResult != OK)
	{
		trace_event("Err. In command_357()", ERROR);
		trace_event("End   check_dcvv_data(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_dcvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_dcvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_dcvv_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_457(BuffRcv,
						nLengthRcv,
						sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   check_dcvv_data(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End check_dcvv_data(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   check_dcvv_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}

	/* PCI */
	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(sLine,   '\0', sizeof(sLine));
	trace_event("End   check_dcvv_data(OK)", PROCESSING);
	return (OK);
}

int check_cvc3_data(
	int  nIndexCtx,
	char *sMkcvc3,
	char *sPan,
	char *sPanSeqNo,
	char *sExpiryDate,
	char *sServiceCode,
	char *sAtc,
	char *sDcvv,
	char *sIvcvc3,
	int  nDataLen,
	char *sData,
	char *sUn,
	char *sCvc3,
	char *sHsmResultCode
	)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];
	char	sDataHex[160+1];
	char	sDataPadded[1024];
	char	Atc[4+1];

	trace_event("Start check_cvc3_data()", PROCESSING);

	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(sLine, 	'\0', sizeof(sLine));
	memset(sDataHex, 	'\0', sizeof(sDataHex));
	memset(sDataPadded, '\0', sizeof(sDataPadded));
	memset(Atc, '\0', sizeof(Atc));

	HexToAscii(sData, sDataHex, nDataLen *2);

	if (strlen(sDataHex) % 16 == 0)
	{
		memset(sDataPadded, '0', strlen(sDataHex) + 16);
		memcpy(sDataPadded, sDataHex, strlen(sDataHex));
		memcpy(sDataPadded + strlen(sDataHex), "80", 2);
	}
	else
	{
		memcpy(sDataHex + strlen(sDataHex), "80", 2);
		if (strlen(sDataHex) % 16 != 0)
		{
			nResult = strlen(sDataHex) % 16;
			memset(sDataPadded, '0', strlen(sDataHex) + 16 - nResult);
			memcpy(sDataPadded , sDataHex, strlen(sDataHex));
		}

	}
	sprintf(Atc,"%04X",atoi(sAtc));

	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                   : %s", sPan);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "PAN  SEQUENCE NUMBER  : %s", sPanSeqNo);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "MKCVC3 KEY            : %s", sMkcvc3);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "ATC					  : %s", Atc);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "DATA				  : %s", sDataPadded);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "UN					  : %s", sUn);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "CVC3				  : %s", sCvc3);
		trace_event(sLine, PROCESSING);
	}


	nResult = command_359(BuffSnd,
						&nLengthSnd,
						sMkcvc3,
						sPan,
						"00",
						sDataPadded,
						sUn,
						Atc,
						sCvc3+1
						);
	if (nResult != OK)
	{
		trace_event("Err. In command_359()", ERROR);
		trace_event("End   check_cvc3_data(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvc3_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvc3_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_cvc3_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_459(BuffRcv,
						nLengthRcv,
						sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   check_cvc3_data(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End check_cvc3_data(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   check_cvc3_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}

	/* PCI */
	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));
	memset(sLine,   '\0', sizeof(sLine));
	trace_event("End   check_cvc3_data(OK)", PROCESSING);
	return (OK);
}

int  check_amx_csc_data(
	int nIndexCtx,
	char *Pan,
	char *CvkA,
	char *CvkB,
	char *expiry_date,
	char *service_code,
	int		nCSCType,
	char *nCSC_field,
	int  flag,
	char *sHsmResultCode
	)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];
	char	sDataCsc[32 + 1];
	char	sExpiryDate[4 + 1];
	char 	sCSC5[5 + 1];
	char 	sCSC4[4 + 1];
	char 	sCSC3[3 + 1];
	char    sExpDate[5];
	char   	sScode[4];

	trace_event("Start check_amx_csc_data()", PROCESSING);

	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sDataCsc,   	'\0', sizeof(sDataCsc));
	memset(sCSC5,   	'\0', sizeof(sCSC5));
	memset(sCSC4,   	'\0', sizeof(sCSC4));
	memset(sCSC3,   	'\0', sizeof(sCSC3));
	memset(sExpiryDate, '\0', sizeof(sExpiryDate));
	memset(sExpDate, '\0', sizeof(sExpDate));
	memset(sScode, '\0', sizeof(sScode));


	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %s", Pan);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "CSC TYPE                : %d", nCSCType);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "EXPIRY DATE                 : %.4s", expiry_date);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "Service Code          : %.3s",service_code );
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "CVKA                        : %.48s", CvkA);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "CVKB                        : %.48s", CvkB);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "CSC FIELD                        : %s", nCSC_field);

		trace_event(sLine, PROCESSING);
		trace_event("- CVV Verification ---------------", PROCESSING);

	}

	if(flag == AMX_AEVV )
	{
		memcpy(sScode, nCSC_field + 1, 1);
		memcpy(sScode + 1, nCSC_field + 2, 2);
		memcpy(sExpDate, nCSC_field + 10, 4);

		memset(sDataCsc, '0', sizeof(sDataCsc) - 1);
		if (memcmp(Pan, "37", 2)== 0)
		{
			memcpy(sDataCsc , sExpDate , 4);
			memcpy(sDataCsc +4 , Pan + 2 , 12);
		}
		else
		{
			memcpy(sDataCsc , Pan + 2 , 12);
			memcpy(sDataCsc + 12 , sExpDate , 4);
		}
		memcpy(sDataCsc +16 , sScode , 3);
	}
	else if (flag == AMX_ALGO_V2)
	{
		memset(sDataCsc, '0', sizeof(sDataCsc) - 1);
		if (memcmp(Pan, "37", 2)== 0)
		{
			memcpy(sDataCsc , expiry_date , 4);
			memcpy(sDataCsc +4 , Pan + 2 , 12);
		}
		else
		{
			memcpy(sDataCsc , Pan + 2 , 12);
			memcpy(sDataCsc + 12 , expiry_date , 4);
		}
		memcpy(sDataCsc +16 , service_code , 3);
	}
	else
	{
		memcpy(sExpiryDate, expiry_date, 4);
		memcpy(sDataCsc , Pan , strlen(Pan));

	}

	if (nCSCType == AMX_CSC5)
	{
		memcpy(sCSC5, nCSC_field, 5);
	}
	else if (nCSCType == AMX_CSC4)
	{
		memcpy(sCSC4, nCSC_field, 4);
	}
	else
	{
		if (flag == AMX_AEVV)
			memcpy(sCSC3, nCSC_field + 7, 3);
		else
			memcpy(sCSC3, nCSC_field, 3);
	}

	nResult = command_35A(BuffSnd,
						&nLengthSnd,
						sDataCsc,
						sExpiryDate,
						CvkA,
						sCSC5,
						sCSC4,
						sCSC3
						);
	if (nResult != OK)
	{
		trace_event("Err. In command_35A()", ERROR);
		trace_event("End   check_amx_csc_data(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_amx_csc_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_amx_csc_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_amx_csc_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_45A(BuffRcv,
						nLengthRcv,
						nCSCType,
						sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   check_amx_csc_data(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End check_amx_csc_data(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   check_amx_csc_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}


	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sDataCsc,   	'\0', sizeof(sDataCsc));
	memset(sCSC5,   	'\0', sizeof(sCSC5));
	memset(sCSC4,   	'\0', sizeof(sCSC4));
	memset(sCSC3,   	'\0', sizeof(sCSC3));

	trace_event("End   check_amx_csc_data(OK)", PROCESSING);
	return (OK);
}

int check_offset_data(int nIndexCtx,
	char *Pan, char *PvkA,
	char *PinBlock, char *format, char *trans_key,
	char *offset, char mode,
	char *sHsmResultCode, int PinLength)
{
	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char    sLine[MAX_LINE_TRC];
	char	sDecTable[SIZE_KEY+1];
	char	sPan[12 + 1];
	char	sFormat[1 + 1];
	char	sPanPadded[16+1];
	char	sPinLength[1+1];
	char	sOffset[12+1];

	trace_event("Start check_offset_data()", PROCESSING);

	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sDecTable,   '\0', sizeof(sDecTable));
	memset(sPan,   		'\0', sizeof(sPan));
	memset(sFormat,   	'\0', sizeof(sFormat));
	memset(sPanPadded,  '\0', sizeof(sPanPadded));
	memset(sPinLength,  '\0', sizeof(sPinLength));
	memset(sOffset,  	'\0', sizeof(sOffset));

	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %.12s", Pan + (strlen(Pan) - 13));
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "PVKA                        : %s", PvkA);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "OFFSET                      : %.12s", offset);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "PIN BLOCK                   : %.16s", PinBlock);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "Format                      : %.2s", format);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "PIN Length                  : %02d", PinLength);
		trace_event(sLine, PROCESSING);

		if (mode == TERMINAL_MODE)
			sprintf(sLine, "TAK/TPK                     : %s", trans_key);
		else
			sprintf(sLine, "ZPK                         : %s", trans_key);
		trace_event(sLine, PROCESSING);
	}

	GetHsmDecTable(sDecTable);

	sprintf(sPinLength, "%X",PinLength);
	format_card_from_pan(sPan, Pan);
	memcpy(sOffset, offset, PinLength);

	if (strlen(sPan) < 16)
	{
		memset(sPanPadded, 'D',sizeof(sPanPadded) -1);
		memcpy(sPanPadded, sPan, strlen(sPan));
	}

	translate_pin_block_format(sFormat,format);

	nResult = command_ibm_32(BuffSnd ,
							&nLengthSnd,
							sFormat,
							PinBlock,
							trans_key,
							sDecTable,
							sOffset,
							sPan,
							sPinLength,
							"F",
							PvkA,
							sPan
						);

	if (nResult != OK)
	{
		trace_event("Err. In command_ibm_32()", ERROR);
		trace_event("End   check_offset_data(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_offset_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_offset_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_offset_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_ibm_42(BuffRcv,
						nLengthRcv,
						sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   check_offset_data(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End check_offset_data(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   check_offset_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}


	/* PCI */
	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sDecTable,   '\0', sizeof(sDecTable));
	memset(sPan,   		'\0', sizeof(sPan));
	memset(sFormat,   	'\0', sizeof(sFormat));
	memset(sPanPadded,  '\0', sizeof(sPanPadded));

	trace_event("End   check_offset_data(OK)", PROCESSING);
	return (OK);
}

int check_pvv_data(int nIndexCtx,
	char *Pan,
	char *PvkA,
	char *PvkB,
	char *PinBlock,
	char *format,
	char *trans_key,
	char *pvv,
	char mode,
	char *sHsmResultCode)
{

	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char	sPan[12 + 1];
	char	sFormat[1 + 1];
	char    sPvki[1 + 1];
	char    sLine[MAX_LINE_TRC];

	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sPan,   		'\0', sizeof(sPan));
	memset(sFormat,   	'\0', sizeof(sFormat));
	memset(sPvki,   	'\0', sizeof(sPvki));

	trace_event("Start check_pvv_data()", PROCESSING);

	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %.12s", Pan + (strlen(Pan) - 13));
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "PVKA                        : %s", PvkA);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "PVKB                        : %.48s", PvkB);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "PVV                         : %.5s", pvv);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "PIN BLOCK                   : %.16s", PinBlock);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "Format                      : %.2s", format);
		trace_event(sLine, PROCESSING);

		if (mode == TERMINAL_MODE)
			sprintf(sLine, "TAK/TPK                     : %s", trans_key);
		else
			sprintf(sLine, "ZPK                         : %s", trans_key);

		trace_event(sLine, PROCESSING);
	}
	trace_event("- Start PVV Verification ---------------", PROCESSING);

	format_card_from_pan(sPan, Pan);

	translate_pin_block_format(sFormat,format);

	memcpy(sPvki, pvv, 1);

	nResult = command_visa_32(BuffSnd ,
							&nLengthSnd,
							sFormat,
							PinBlock,
							trans_key,
							PvkA,
							"",
							pvv +1,
							sPvki,
							sPan +1,
							sPan
						);

	if (nResult != OK)
	{
		trace_event("Err. In command_visa_32()", ERROR);
		trace_event("End   check_pvv_data(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_pvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_pvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_pvv_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_visa_42(BuffRcv,
						nLengthRcv,
						sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   check_pvv_data(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End 	check_pvv_data(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   check_pvv_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}


	/* PCI */
	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sPan,   		'\0', sizeof(sPan));
	memset(sFormat,   	'\0', sizeof(sFormat));


	trace_event("End   check_pvv_data(OK)", PROCESSING);
	return (OK);
}
/*************************************************************************************************/
/*************************************************************************************************/
int
	check_crpt_data(
	int nIndexCtx,
	char *sTlvBuffer,
	char *sHsmResultCode
	)
{

	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	pid_t   nPid;
	int		nResult;
	int 	nLengthSnd,nLengthRcv;
	char   	sPan[22 + 1];
	char   	sSeqNumber[8 + 1];
	char   	sAtc[4 + 1];
	char   	sUn[8 + 1];
	char   	sDerivationType[1 + 1];
	char	sDiversificationData[16+1];

	char   sArqc[16 + 1];
	char   sArpc[16 + 1];
	char   sAC_Key[SIZE_KEY_TDES + 1];
	char   sArc[256];

	char   sCsu[8 + 1];
	char   sArcCsu[8 + 1];

	char	sNetworkCode[2 + 1];
	char    sIAD[512 +1];

	char 	arpc_mthd;
	char	crd_key_drv_mthd;
	char 	sk_drv_mthd;
	char 	sCVN[2 + 1];

	TSTlvPrivate    tlvInfo;

	char            szWork[512];
	char            sIccData[512];
	char            sTmpField[512];
	char            sLine[MAX_LINE_TRC];
	char            sIad[64 + 1];
	char			sData[1024];
	char			sDataHex[1024];
	char            sEmvCardInfoData[128];

	int             nRetCode = 0;
	int             nLenWork = 0;
	int             nLenData = 0;
	int             nLength = 0;
	int             nTmpLen = 0;
	int             nIccLen = 0;
	int             nCvrLen = 0;
	int				nPanLength = 0; /*SKA06042023 - PLUTONL-6022*/
	char			sDataPadded[1024];






	trace_event("Start check_crpt_data()", PROCESSING);

	InitTlvPrivate(&tlvInfo);
	memset(sAtc, 0, sizeof(sAtc));
	memset(sUn, 0, sizeof(sUn));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sArqc, 0, sizeof(sArqc));
	memset(sArpc, 0, sizeof(sArpc));
	memset(sData, 0, sizeof(sData));
	memset(sAC_Key, 0, sizeof(sAC_Key));
	memset(szWork, 0, sizeof(szWork));
	memset(sTmpField, 0, sizeof(sTmpField));
	memset(sArc, 0, sizeof(sArc));
	memset(sArpc, 0, sizeof(sArpc));
	memset(sCsu, 0, sizeof(sCsu));
	memset(sArcCsu, 0, sizeof(sArcCsu));
	memset(sPan, 0, sizeof(sPan));
	memset(sSeqNumber, 0, sizeof(sSeqNumber));
	memset(sIccData, 0, sizeof(sIccData));
	memset(sIad, 0, sizeof(sIad));
	memset(sIAD, 0, sizeof(sIAD));
	memset(sEmvCardInfoData, 0, sizeof(sEmvCardInfoData));
	memset(sNetworkCode, 0, sizeof(sNetworkCode));
	memset(sCVN, 0, sizeof(sCVN));
	memset(sDerivationType, '\0', sizeof(sDerivationType));
	memset(sDiversificationData, '\0', sizeof(sDiversificationData));
	memset(sDataHex, '\0', sizeof(sDataHex));
	memset(sDataPadded, '\0', sizeof(sDataPadded));
	nLenData = 0;



	if (GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sIccData, &nIccLen) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-PAN", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	nLenWork = AnalyseTlvPrivate(sIccData, nIccLen, &tlvInfo);


	if (GetTlvBuffer(AUTO_PAN, sTlvBuffer, sPan, &nLenData) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-PAN", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	nPanLength = strlen(sPan);

	if (GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER, sTlvBuffer, sSeqNumber, &nLenData) != SUCCES)
	{
		memcpy(sSeqNumber, "000", 3);
	}


	if (GetTlvBuffer(AUTO_EMV_AC_KEY, sTlvBuffer, sAC_Key, &nLenData) != SUCCES)
	{
		memcpy(sHsmResultCode, "-DMK", 4);
		trace_event("INVALID KEY DMK ", ERROR);
		trace_event("END   check_crpt_data(KEY_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}



	if (GetTlvPrivate(CHIP_ISS_APPLI_DATA_TAG, &tlvInfo, sIAD, &nLenWork) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-IAD", 4);
		trace_event("INVALID EMV DATA ", ERROR);
		trace_event("END   check_crpt_data(IAD UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	GetTlvBuffer(AUTO_NETWORK_CODE, sTlvBuffer, sNetworkCode, &nLenWork);


	if (GetTlvBuffer(AUTO_EMV_CARD_INFO_PARAM, sTlvBuffer, sEmvCardInfoData, &nLenWork) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-PAN", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_EMV_CARD_INFO_PARAM) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	nRetCode = get_emv_param(sNetworkCode, sIAD, sEmvCardInfoData, sCVN, nPanLength); 
	if( nRetCode != OK )
	{
		memcpy(sHsmResultCode, "-PAN", 4);
		trace_event("ERROR WhiLe get_emv_param() ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}


	arpc_mthd			= sEmvCardInfoData[EMV_OFST_ARPC_MTHD];
	crd_key_drv_mthd	= sEmvCardInfoData[EMV_OFST_MKD];
	sk_drv_mthd			= sEmvCardInfoData[EMV_OFST_SKD];

	if (GetTlvPrivate(CHIP_ATC_TAG, &tlvInfo, sAtc, &nLenWork) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-ATC", 4);
		trace_event("ATC UNAVAILABLE", ERROR);
		trace_event("END   check_crpt_data(ATC UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}


	if( crd_key_drv_mthd == EMV_MKD_A )
	{
		switch( sk_drv_mthd  )
		{
		case EMV_SKD_CSK:		sDerivationType[0] = EMV_ATALLA_SKD_CSK;
								memset(sDiversificationData, '0', 16);
								memcpy(sDiversificationData, sAtc,4);
								break;
		case EMV_SKD_MCI_SKD:	sDerivationType[0] = EMV_ATALLA_SKD_MCI;
								memset(sDiversificationData, '0', 16);
								memcpy(sDiversificationData, sAtc,4);
								break;
		case EMV_SKD_UPI_SKD:	sDerivationType[0] = EMV_ATALLA_SKD_UPI;
								memcpy(sDiversificationData, sAtc,4);
								break;
		case EMV_SKD_NONE:		sDerivationType[0] = EMV_ATALLA_SKD_VISA;  break;

		default:				sDerivationType[0] = '?'; break;
		}
	}
	else
	{
		sDerivationType[0] = '?';
	}

	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %s", sPan);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "SEQUENCE NUMBER             : %s", sSeqNumber);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "AC KEY IS                   : %s", sAC_Key);
		trace_event(sLine, PROCESSING);
	}
	sprintf(sLine, "PARAMETERS		: %s|",sCVN);
	trace_event(sLine, PROCESSING);


	if( sDerivationType[0] == '?' )
	{
		memcpy(sHsmResultCode, "-SKD", 4);
		trace_event("ERROR ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	nRetCode = build_crpt_data_dflt(nIndexCtx, sTlvBuffer,sCVN,sData,&nLenData,sHsmResultCode);

	if( nRetCode != OK )
	{
		memcpy(sHsmResultCode, "-DAT", 4);
		trace_event("ERROR WhiLe build_crpt_data_dflt() ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	if (GetTlvPrivate(CHIP_CALC_ARC, &tlvInfo, sArcCsu, &nLenWork) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-ARC", 4);
		trace_event("ERROR CALC ARC DATA ", ERROR);
		trace_event("END   check_crpt_data(IAD UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	PutTlvPrivate(CHIP_ORIG_ARC , &tlvInfo, sArcCsu, nLenWork);

	if (arpc_mthd == EMV_ARPC_MTHD_1 )
	{
		PutTlvPrivate (CHIP_AUTHOR_RESP_CODE, &tlvInfo, sArcCsu, 4);
		AsciiToHex(sArcCsu, sArc, 4);

		sprintf(sLine, "ARC		: |%0.4s", sArcCsu);
		trace_event(sLine, PROCESSING);
	}
	else
	{
		PutTlvPrivate (CHIP_AUTHOR_RESP_CODE, &tlvInfo, "3030", 4);
		AsciiToHex(sArcCsu, sCsu, 8);
		sprintf(sLine, "CSU		: |%0.8s", sArcCsu);
		trace_event(sLine, PROCESSING);
	}


	if (GetTlvPrivate(CHIP_APP_CRYPTOGRAM_TAG, &tlvInfo, sArqc, &nLenWork) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-ARQ", 4);
		trace_event("ARQC UNAVAILABLE ", ERROR);
		trace_event("END   check_crpt_data(ARQC UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}



	if (GetTlvPrivate(CHIP_UNPRED_NUMBER_TAG, &tlvInfo, sUn, &nLenWork) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-UNP", 4);
		trace_event("UNPRED NUMBER UNAVAILABLE", ERROR);
		trace_event("END   check_crpt_data(UNPRED NUMBER UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	HexToAscii(sData, sDataHex, nLenData *2);

	if (sDerivationType[0] ==  EMV_ATALLA_SKD_VISA)
	{
		if(strlen(sDataHex) % 8 == 0)
		{
			memset(sDataPadded, '0', strlen(sDataHex) + 16);
			memcpy(sDataPadded, sDataHex, strlen(sDataHex));
		}
		else
		{
			nResult = strlen(sDataHex) % 8;
			memset(sDataPadded, '0', strlen(sDataHex) + 8 - nResult);
			memcpy(sDataPadded, sDataHex, strlen(sDataHex));
		}
	}
	else
	{
		/*if (strlen(sDataHex) % 8 == 0)
		{
			memset(sDataPadded, '0', strlen(sDataHex) + 16);
			memcpy(sDataPadded, sDataHex, strlen(sDataHex));
			memcpy(sDataPadded + strlen(sDataHex), "80", 2);
		}
		else
		{*/
			nResult = strlen(sDataHex) % 16;
			memset(sDataPadded, '0', strlen(sDataHex) + 16 - nResult);
			memcpy(sDataPadded , sDataHex, strlen(sDataHex));
			memcpy(sDataPadded + strlen(sDataHex), "80", 2);
		/*}*/
	}

	nResult = command_350(BuffSnd,
						 &nLengthSnd,
						 sDerivationType,
						 sAC_Key,
						 sPan,
						 sSeqNumber+1,
						 sDiversificationData,
						 sArqc,
						 sDataPadded,
						 sArcCsu,
						 "",
						 "",
						 "",
						 ""
						 );



	if (nResult != OK)
	{
		trace_event("Err. In command_350()", ERROR);
		trace_event("End   check_crpt_data(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_crpt_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_crpt_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_crpt_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_450(BuffRcv,
						  nLengthRcv,
						  sArpc,
						  sHsmResultCode);
	if(nResult != OK)
	{
		switch (nResult)
		{
			case VERIFICATION_FAILED:
				trace_event("End   check_crpt_data(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);
			case ERROR_COMMAND:
				trace_event("End 	check_crpt_data(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   check_crpt_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
		}
	}


	memcpy(szWork, sArpc,16);


	if(arpc_mthd == EMV_ARPC_MTHD_1)
	{
		sprintf(szWork + 16, "%02X%02X", sArc[0], sArc[1]);
	}
	else
	{
		sprintf(szWork + 16, "%02X%02X%02X%02X", sCsu[0], sCsu[1], sCsu[2], sCsu[3]);
	}


	sprintf(sLine, "==> Issuer Authen Data       : [%s]", szWork);
	trace_event(sLine, PROCESSING);


	PutTlvPrivate(CHIP_ISS_AUTHEN_DATA_TAG, &tlvInfo, szWork, strlen(szWork));


	PutTlvPrivate (CHIP_ISS_ARPC_RESP_CODE_TAG, &tlvInfo, szWork, 16);


	nIccLen = PrivateBuildTlv(sIccData, &tlvInfo);
	PutTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sIccData, nIccLen);


	InitTlvPrivate(&tlvInfo);
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sAC_Key, 0, sizeof(sAC_Key));
	memset(szWork, 0, sizeof(szWork));
	memset(sTmpField, 0, sizeof(sTmpField));
	memset(sPan, 0, sizeof(sPan));
	memset(sIccData, 0, sizeof(sIccData));
	trace_event("End   check_crpt_data(OK)", PROCESSING);
	return (OK);
}

int		pinChangeOffset(int		nIndexCtx,
						char	*sDerivationType,
						char	*sKeyENC,
						char	*sKeyMAC,
						char	*sKeyAC,
						char	*sPan,
						char	*sSeqNumber,
						char	*sDiversificationData,
						char	*sAppData,
						char	*sPvk,
						char	*sOffset,
						char	*sPinEnc,
						char	*sMac,
						char	*sHsmResultCode)
{

	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	int		nResult;
	pid_t   nPid;
	int 	nLengthSnd,nLengthRcv;
	char	sDecTable[SIZE_KEY+1];
	char    sLine[MAX_LINE_TRC];


	memset(BuffSnd, 	'\0', sizeof(BuffSnd));
	memset(BuffRcv, 	'\0', sizeof(BuffRcv));
	memset(sLine,   	'\0', sizeof(sLine));
	memset(sDecTable,   '\0', sizeof(sDecTable));

	GetHsmDecTable(sDecTable);

	nResult = command_3FC(BuffSnd,
						  &nLengthSnd,
						  sDerivationType,
						  sKeyENC,
						  sKeyMAC,
						  sKeyAC,
						  sPan,
						  sSeqNumber+1,
						  sDiversificationData,
						  sAppData,
						  sPvk,
						  sDecTable,
						  sOffset,
						  sPan+2,
						  "F");

	if (nResult != OK)
	{
		trace_event("Err. In command_3FC()", ERROR);
		trace_event("End   pinChangeOffset(NOK)", PROCESSING);
		return (nResult);
	}

	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( nResult != OK )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   pinChangeOffset(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   pinChangeOffset(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   pinChangeOffset(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nResult = command_4FC(BuffRcv,
						nLengthRcv,
						sPinEnc,
						sMac,
						sHsmResultCode);

	if(nResult != OK)
	{
		trace_event("Err. In command_4FC()", ERROR);
		trace_event("End   pinChangeOffset(NOK)", PROCESSING);
		return (nResult);
	}

	return (OK);
}

int     gen_iss_script_data(
	int        nIndexCtx,
	char      *sTlvBuffer,
	char      *sHsmResultCode
	)

{

	char    BuffRcv   		[BUF_LG_MAX];
	char    BuffSnd   		[BUF_LG_MAX];
	pid_t   nPid;
	int		nResult;
	int 	nLengthSnd,nLengthRcv;
	char	sDiversificationData[16+1];
	unsigned char  sPan[22 + 1];
	unsigned char  sSeqNumber[8 + 1];
	unsigned char  sFormatedPan[8 + 1];
	unsigned char  sAtc[2 + 1];
	unsigned char  sUn[4 + 1];
	unsigned char  sPlainTextData[1024];
	int            nPlainTextLen;
	int            nCipherTextLen;
	unsigned char  sMAC[32];
	unsigned char  sMACStr[32];
	unsigned char  sSecureMsgData[1024];
	unsigned char  sIssCmdData[1024];
	unsigned char  sIssScriptData[1024];
	unsigned char  sAuthDeclFlag[8];
	unsigned char  sTag71[256];
	unsigned char  sTag72[256];
	int            nSecureMsgLen;
	char  			sArqc[16 + 1];
	unsigned char  sSMI_Key[256];
	unsigned char  sSMC_Key[256];
	unsigned char  sAC_Key[256];
	unsigned char  sPVK_Key[256];
	unsigned char  sPVKA[256]; /*ZKO28032019*/
	char           sPinPek[64];
	char           sPek[256];
	char           sSrcPBFrmt[8];
	char 			sTransDate[16 + 1];
	char 			sRefNbr[16 + 1];
	char           sCvr[64 + 1];
	char           v_icc_application_index[4 + 1];
	char           v_issuing_bank_code[6 + 1];
	char			sCardPinOffset[16 + 1];

	TSTlvPrivate	tlvInfo;


	char           szWork[1024];
	char           sTmpField[BUF_LG_MAX];
	char			sIsAuthenticated[4];


	char           sLine[MAX_LINE_TRC];
	char           sPan12     [12 + 1];
	char           sPinBlkBCD [8 + 1];
	char			sActionFlag[4];
	char			sIAD[128];
	char			sScriptCode[2 + 1];
	char			sSecurityData[P7_F053_MAX_LEN + 1];
	char			sSecurityVerifResult[32 + 1];
	char 			sCardSeqNbr[3+1];

	int      nRetCode = 0;
	int      nLenWork = 0;
	int      nLenData = 0;
	int      nLength = 0;
	int      nTmpLen = 0;
	int		nTotalOffset;
	int		nIsPINPresent;
	int      nSequence = 0;
	char     sAppCode[4 + 1];
	char	sOriginCode[4];
	char	encryption_mode;
	char	sSecurityFormat[2 + 1];
	char	sNetworkCode[2 + 1];
	char   	sDerivationType[1 + 1];
	char	sMacLen[1+1];
	char	sDataPadded[2048];

	trace_event("Start gen_iss_script_data()", PROCESSING);

	memset(BuffSnd, '\0', sizeof(BuffSnd));
	memset(BuffRcv, '\0', sizeof(BuffRcv));

	memset(sFormatedPan, 0, sizeof(sFormatedPan));
	memset(sPlainTextData, 0, sizeof(sPlainTextData));
	memset(sSecureMsgData, 0, sizeof(sSecureMsgData));
	memset(sIssScriptData, 0, sizeof(sIssScriptData));
	memset(sIssCmdData, 0, sizeof(sIssCmdData));
	memset(sMAC, 0, sizeof(sMAC));
	memset(sArqc, 0, sizeof(sArqc));
	memset(sSMI_Key, 0, sizeof(sSMI_Key));
	memset(sSMC_Key, 0, sizeof(sSMC_Key));
	memset(sAC_Key, 0, sizeof(sAC_Key));
	memset(sPVK_Key, 0, sizeof(sPVK_Key));
	memset(szWork, 0, sizeof(szWork));
	memset(sTmpField, 0, sizeof(sTmpField));
	memset(sPinPek, '\0', sizeof(sPinPek));
	memset(sPek, 0, sizeof(sPek));
	memset(sSrcPBFrmt, 0, sizeof(sSrcPBFrmt));
	memset(sTag71, 0, sizeof(sTag71));
	memset(sTag72, 0, sizeof(sTag72));
	memset(sCvr, 0, sizeof(sCvr));
	memset(v_icc_application_index, 0, sizeof(v_icc_application_index));
	memset(v_issuing_bank_code, 0, sizeof(v_issuing_bank_code));
	memset(sTransDate, 0, sizeof(sTransDate));
	memset(sRefNbr, 0, sizeof(sRefNbr));
	memset(sPan, 0, sizeof(sPan));
	memset(sSeqNumber, 0, sizeof(sSeqNumber));
	memset(sAtc, 0, sizeof(sAtc));
	memset(sUn, 0, sizeof(sUn));
	memset(sMACStr, 0, sizeof(sMACStr));
	memset(sLine, 0, sizeof(sLine));
	memset(sPan12     , 0, sizeof(sPan12     ));
	memset(sPinBlkBCD , 0, sizeof(sPinBlkBCD ));
	memset(sAppCode , 0, sizeof(sAppCode ));
	memset(sOriginCode , 0, sizeof(sOriginCode ));
	memset(sActionFlag , 0, sizeof(sActionFlag ));
	memset(sSecurityFormat , 0, sizeof(sSecurityFormat ));
	memset(sNetworkCode , 0, sizeof(sNetworkCode ));
	memset(sIAD , 0, sizeof(sIAD ));
	memset(sScriptCode , 0, sizeof(sScriptCode ));
	memset(sSecurityData , 0, sizeof(sSecurityData ));
	memset(sIsAuthenticated , 0, sizeof(sIsAuthenticated ));
	memset(sSecurityVerifResult , 0, sizeof(sSecurityVerifResult ));
	memset(sCardPinOffset , 0, sizeof(sCardPinOffset ));
	memset(sPVKA , 0, sizeof(sPVKA ));
	memset(sCardSeqNbr, '\0', sizeof(sCardSeqNbr));
	memset(sDerivationType, '\0', sizeof(sDerivationType));
	memset(sMacLen, '\0', sizeof(sMacLen));
	memset(sDataPadded, '\0', sizeof(sDataPadded));
	InitTlvPrivate(&tlvInfo);



	GetTlvBuffer(AUTO_ORIGIN_CODE, sTlvBuffer, sOriginCode, &nLenData);
	if( sOriginCode[0] != '0' && sOriginCode[0] != '1' && sOriginCode[0] != '2' )
	{
		trace_event("End   gen_iss_script_data(). offus card", PROCESSING);
		return(OK);
	}

	GetTlvBuffer(AUTO_NETWORK_CODE, sTlvBuffer, sNetworkCode, &nLenWork);
	GetTlvBuffer(AUTO_ACTION_FLAG, sTlvBuffer, sActionFlag, &nLenData);


	if (GetTlvBuffer(AUTO_PAN, sTlvBuffer, sPan, &nLenData) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-PAN", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
		trace_event("END   gen_iss_script_data(NOK)", PROCESSING);
		return(HSM_MALFUNCTION);
	}


	if (GetTlvBuffer(AUTO_RRN, sTlvBuffer, sRefNbr, &nLenData) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-PCD", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(Reference Number) ", ERROR);
		trace_event("END   gen_iss_script_data(NOK)", PROCESSING);
		return(HSM_MALFUNCTION);
	}


	if (GetTlvBuffer(AUTO_TRANS_DATE_TIME, sTlvBuffer, sTransDate, &nLenData) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-PCD", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(TRANS_DATE_TIME) ", ERROR);
		trace_event("END   gen_iss_script_data(NOK)", PROCESSING);
		return(HSM_MALFUNCTION);
	}


	if (GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER, sTlvBuffer, sSeqNumber, &nLenData) != SUCCES)
	{
		trace_event("DE23 MISSING.  USE THE ZERO VALUE", FATAL);
		sprintf(sSeqNumber, "%03d", 0);
	}


	if (GetTlvBuffer(AUTO_ISSUING_BANK, sTlvBuffer, v_issuing_bank_code, &nLenWork) != SUCCES)
	{
		memcpy(sHsmResultCode, "-ISS", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_ISSUING_BANK) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	if (GetTlvBuffer(ICC_APPLICATION_INDX, sTlvBuffer, v_icc_application_index, &nLenWork) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-IAI", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(ICC_APPLICATION_INDX) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}


	if (GetTlvBuffer(AUTO_SECURITY_VER_RESULTS, sTlvBuffer, sSecurityVerifResult, &nLenData) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-F48", 4);
		trace_event("END   gen_iss_script_data(NOK) <ERROR ON GetTlvBuffer(AUTO_SECURITY_VER_RESULTS)>", PROCESSING);
		return(HSM_MALFUNCTION);
	}


	nIsPINPresent = OK;
	if (GetTlvBuffer(AUTO_PIN_DATA, sTlvBuffer, sTmpField, &nLenData) != SUCCESS)
	{
		nIsPINPresent = NOK;
	}

	sIsAuthenticated[0] = 'N';
	if( sSecurityVerifResult[SEC_FLAG_CHK_ARQC] == '0' &&
	(sSecurityVerifResult[SEC_FLAG_CHK_PIN] == '0' ||
		(sSecurityVerifResult[SEC_FLAG_CHK_PIN] == '4' && nIsPINPresent == NOK ) ))
	{
		sIsAuthenticated[0] = 'Y';
	}

	/*EBETODO: a chip card doing a mag author, will get "error"?*/
	if (GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sTmpField, &nLenWork) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-ICC", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_ICC_DATA) ", ERROR);
		trace_event("END   gen_iss_script_data(NOK)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nLenWork = AnalyseTlvPrivate(sTmpField, nLenWork, &tlvInfo);

	GetTlvPrivate(CHIP_ISS_APPLI_DATA_TAG, &tlvInfo, sIAD, &nLenWork);

	if (GetTlvPrivate(CHIP_CVR, &tlvInfo, sCvr, &nLenWork) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-CVR", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(CHIP_CVR) ", ERROR);
		trace_event("END   gen_iss_script_data(NOK)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	GetTlvBuffer(AUTO_EMV_CARD_INFO_PARAM, sTlvBuffer, szWork, &nLenWork);


	if( memcmp(szWork,"VSDC",4) == 0 )
	{
		sDerivationType[0] = '1';
		/*
		The issuer host can determine the format of the Issuer Application Data as follows:
		- IAD FORMAT 0/1/3: The first byte of the IAD has the value '06'.
		- IAD FORMAT 2: The first byte of the IAD has the value '1F', and the first nibble of the third byte
		has the value '2'
		*/
		/*
		CVR Byte 4 bit 8 will indicate that the application is supporting EMV command session keys
		*/
	}
	else if( memcmp(szWork,"AX42",5) == 0 )
	{
		sDerivationType[0] = '1';
	}
	else
	{
		sDerivationType[0] = '0';
	}



	memcpy(sAppCode,szWork,4);




	nRetCode = Update_SentIssScripts(nIndexCtx, sPan, sCvr,sAppCode);






	if (GetTlvPrivate(CHIP_ATC_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
	{
		memcpy(sAtc, szWork, nLenWork);
	}
	else
	{
		memset(sAtc,'0',4);
	}


	if (GetTlvPrivate(CHIP_APP_CRYPTOGRAM_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
	{
		memcpy(sArqc, szWork, nLenWork);
	}


	if (GetTlvPrivate(CHIP_UNPRED_NUMBER_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
	{
		memcpy(sUn, szWork, nLenWork);
	}


	nRetCode = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if (OK != nRetCode)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   gen_iss_script_data(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}


	nSequence = 0;
	nTotalOffset = 0;
	while (1)
	{

		memset(szWork, 0, sizeof(szWork));
		memset(sIssCmdData, 0, sizeof(sIssCmdData));
		memset(sAuthDeclFlag, 0, sizeof(sAuthDeclFlag));
		memset(BuffSnd, 0, sizeof(BuffSnd));
		memset(BuffRcv, 0, sizeof(BuffRcv));
		memset(sPlainTextData, 0, sizeof(sPlainTextData));
		memset(sPan12     , 0, sizeof(sPan12     ));
		memset(sPinBlkBCD , 0, sizeof(sPinBlkBCD ));
		memset(sScriptCode , 0, sizeof(sScriptCode ));

		nPlainTextLen = 0;


		nRetCode = Get_IssScriptData(nIndexCtx, sPan, sIssCmdData, &nSequence, sIsAuthenticated, sTransDate, sRefNbr, sScriptCode);
		if (nRetCode != OK)
		{
			trace_event("==> Get_IssScriptData(!OK).  BREAK", PROCESSING);
			break;
		}


		sprintf(sLine, "ISSUER COMMAND DATA IS       =[%s|%s]", sScriptCode,sIssCmdData);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "sIsAuthenticated             =[%s]", sIsAuthenticated);
		trace_event(sLine, PROCESSING);

		if( nSequence == 1 )
		{
			nRetCode = Get_EMV_Key(nIndexCtx, sPan, "SMI", sSMI_Key);
			if (nRetCode != OK)
			{
				memcpy(sHsmResultCode, "-SMI", 4);
				trace_event("INVALID KEY DMK ", ERROR);
				trace_event("END   gen_iss_script_data(KEY_UNAVAILABLE)", PROCESSING);
				return(HSM_MALFUNCTION);
			}
		}

		/** Diversification Data **/
		if ('0' == sDerivationType[0]) /** MCI **/
		{
			memcpy(sDiversificationData, sArqc, 16);
		}
		else
		{
			memcpy(sDiversificationData , sAtc, 4);
		}

		if ('0' == sDerivationType[0] && 1 < nSequence)
			FctIncrimentRandBy1(sArqc, sArqc);

		/*Data to be maced:
		* CLA, INS, P1, P2, Lc(including the MAC length)
		* ATC
		* ARQC
		* Plaintext/Enciphered data
		*/
		nPlainTextLen = 0;
		memcpy(sPlainTextData + nPlainTextLen, sIssCmdData, 10); nPlainTextLen += 10;
		memcpy(sPlainTextData + nPlainTextLen, sArqc, 16); nPlainTextLen += 16;

		if( memcmp(sScriptCode, "PC", 2) == 0 )
		{

			nRetCode = GetIssuerZPK(nIndexCtx, sPan, sPek );
			if( nRetCode != OK)
			{
				memcpy(sHsmResultCode, "-ZPK", 4);
				trace_event("END   gen_iss_script_data(NOK) <ERROR ON GetIssuerZPK>", PROCESSING);
				return(HSM_MALFUNCTION);
			}

			GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER, sTlvBuffer, sCardSeqNbr, &nLength);

			nRetCode = GetCardPinOffset(nIndexCtx, sPan, sCardSeqNbr, sCardPinOffset);
			if( nRetCode != OK)
			{
				memcpy(sHsmResultCode, "OFST", 4);
				trace_event("END   gen_iss_script_data(NOK) <ERROR ON GetCardPinOffset>", PROCESSING);
				return(HSM_MALFUNCTION);
			}


			nRetCode = GetTlvBuffer(AUTO_PVK_V1,sTlvBuffer,sPVK_Key,&nLength);
			if ( nRetCode != OK )
			{

				nRetCode = GetPVKPinKey ( nIndexCtx, sPan  , sPVKA  , sPVK_Key ); /** MAW20030515 **/
				if ( nRetCode != OK )
				{
					memcpy(sHsmResultCode, "PVK", 4);
					trace_event("END   gen_iss_script_data(NOK) <ERROR ON Get PVK>", PROCESSING);
					return(HSM_MALFUNCTION);
				}

			}


			nRetCode = Get_EMV_Key(nIndexCtx, sPan, "SMC", sSMC_Key);
			if (nRetCode != OK)
			{
				memcpy(sHsmResultCode, "-SMC", 4);
				trace_event("END   gen_iss_script_data(KEY_UNAVAILABLE) <ERROR ON Get_EMV_Key(ENC)>", PROCESSING);
				return(HSM_MALFUNCTION);
			}


			nRetCode = Get_EMV_Key(nIndexCtx, sPan, "AC", sAC_Key);
			if (nRetCode != OK)
			{
				memcpy(sHsmResultCode, "-AC_", 4);
				trace_event("END   gen_iss_script_data(KEY_UNAVAILABLE) <ERROR ON Get_EMV_Key(AC)>", PROCESSING);
				return(HSM_MALFUNCTION);
			}


			nRetCode = pinChangeOffset(nIndexCtx,
										sDerivationType,
										sSMC_Key,
										sSMI_Key,
										sAC_Key,
										sPan,
										sCardSeqNbr,
										sDiversificationData,
										sPlainTextData,
										sPVK_Key,
										sCardPinOffset,
										sPinPek,
										sMACStr,
										sHsmResultCode);


			if (nRetCode != OK)
			{
				memcpy(sHsmResultCode, "-PIN", 4);
				trace_event("END   gen_iss_script_data(pinChangeOffset(NOK))", PROCESSING);
				return(HSM_MALFUNCTION);
			}

		}
		else
		{
			/*Command Data if any*/
			if (strlen(sIssCmdData) > 10)
			{
				memcpy(sPlainTextData + nPlainTextLen, sIssCmdData + 10, strlen(sIssCmdData) - 10);
				nPlainTextLen += strlen(sIssCmdData) - 10;
			}
			if (strlen(sPlainTextData) % 16 == 0)
			{
				memcpy(sDataPadded , sPlainTextData, strlen(sPlainTextData));
			}
			else
			{
				nResult = strlen(sPlainTextData) % 16;
				memset(sDataPadded, '0', strlen(sPlainTextData) + 16 - nResult);
				memcpy(sDataPadded , sPlainTextData, strlen(sPlainTextData));
				memcpy(sDataPadded + strlen(sPlainTextData), "80", 2);
			}
			nResult = command_352(BuffSnd,
							  	  &nLengthSnd,
								  sDerivationType,
								  sSMI_Key,
								  sPan,
								  sSeqNumber+1,
								  sDiversificationData,
								  "3", /*64-bit MAC*/
								  "",
								  sDataPadded,
								  "",
								  "",
								  "");

			if (nResult != OK)
			{
				trace_event("Err. In command_352()", ERROR);
				trace_event("End   gen_iss_script_data(NOK)", PROCESSING);
				return (nResult);
			}

			nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
			if ( nResult != OK )
			{
				memcpy(sHsmResultCode, "UNVL", 4);
				trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
				trace_event("End   gen_iss_script_data(HSM_UNAVAILABLE)",PROCESSING);
				return(HSM_UNAVAILABLE);
			}

			nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
			if ( nResult == NOT_AVAILABLE )
			{
				memcpy(sHsmResultCode, "UNVL", 4);
				trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
				trace_event("End   gen_iss_script_data(HSM_UNAVAILABLE)",PROCESSING);
				return(HSM_UNAVAILABLE);
			}

			if (nResult == HSM_MALFUNCTION)
			{
				memcpy(sHsmResultCode, "UNVL", 4);
				trace_event("HSM MALFUNCTION DETECTED",ERROR);
				trace_event("End   gen_iss_script_data(MALFUNCTION)",PROCESSING);
				return(HSM_MALFUNCTION);
			}

			nResult = command_452(BuffRcv,
								nLengthRcv,
								sMacLen,
								sMACStr,
								sHsmResultCode);

			if(nResult != OK)
			{
				trace_event("Err. In command_452()", ERROR);
				trace_event("End   gen_iss_script_data(NOK)", PROCESSING);
				return (nResult);
			}

			trim(sMACStr,strlen(sMACStr));
		}

		memset(sTmpField, 0, sizeof(sTmpField));
		memset(sIssScriptData, 0, sizeof(sIssScriptData));



		if (debug_option == 1)
		{
			sprintf(sLine, "==> SECURE MESSAGING MAC       =[%s]", sMACStr);
			trace_event(sLine, PROCESSING);
		}



		nLength = 0;
		sprintf(sIssScriptData, "86%02lX", ((strlen(sIssCmdData) + strlen(sMACStr)) / 2) + nSecureMsgLen); nLength+=4;
		memcpy(sIssScriptData + nLength, sIssCmdData, strlen(sIssCmdData)); nLength+=strlen(sIssCmdData);

		memcpy(sIssScriptData + nLength, sMACStr, strlen(sMACStr)); nLength+=strlen(sMACStr);



		memcpy(sTag72 + nTotalOffset, sIssScriptData, nLength);
		nTotalOffset+=nLength;

		if( nSequence >= MAX_ISS_SCRIPT)
		{
			break;
		}
	}

	if (strlen(sTag71) == 0 && strlen(sTag72) == 0)
	{
		memset(BuffSnd, 0, sizeof(BuffSnd));
		memset(BuffRcv, 0, sizeof(BuffRcv));
		memset(sFormatedPan, 0, sizeof(sFormatedPan));
		memset(sPlainTextData, 0, sizeof(sPlainTextData));
		memset(sSecureMsgData, 0, sizeof(sSecureMsgData));
		memset(sIssScriptData, 0, sizeof(sIssScriptData));
		memset(sIssCmdData, 0, sizeof(sIssCmdData));
		memset(sSMI_Key, 0, sizeof(sSMI_Key));
		memset(sSMC_Key, 0, sizeof(sSMC_Key));
		memset(sAC_Key, 0, sizeof(sAC_Key));
		memset(sPVK_Key, 0, sizeof(sPVK_Key));
		memset(sCardPinOffset, 0, sizeof(sCardPinOffset));
		memset(szWork, 0, sizeof(szWork));
		memset(sTmpField, 0, sizeof(sTmpField));
		memset(sPinPek, 0, sizeof(sPinPek));
		memset(sPek, 0, sizeof(sPek));
		memset(sSrcPBFrmt, 0, sizeof(sSrcPBFrmt));
		memset(sPan, 0, sizeof(sPan));
		memset(sPan12, 0, sizeof(sPan12));
		memset(sPinBlkBCD, 0, sizeof(sPinBlkBCD));
		InitTlvPrivate(&tlvInfo);
		trace_event("End   gen_iss_script_data(NOK). NO ISSUER SCRIPT TO SEND", PROCESSING);
		return(NOK);
	}




	if (strlen(sTag71) > 0 )
	{
		nLenWork = 0;
		memcpy(szWork + nLenWork, "9F180441424344", 14); nLenWork += 14;
		memcpy(szWork + nLenWork, sTag71, strlen(sTag71)); nLenWork += strlen(sTag71);
		PutTlvPrivate(CHIP_ISS_SCRIPT_1, &tlvInfo, szWork, nLenWork);
	}
	if (strlen(sTag72) > 0 )
	{
		nLenWork = 0;
		memcpy(szWork + nLenWork, "9F180441424344", 14); nLenWork += 14;
		memcpy(szWork + nLenWork, sTag72, strlen(sTag72)); nLenWork += strlen(sTag72);
		PutTlvPrivate(CHIP_ISS_SCRIPT_2, &tlvInfo, szWork, nLenWork);

	}

	nLenWork = PrivateBuildTlv(sTmpField, &tlvInfo);
	if ( nLenWork > 0 )
	{
		PutTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sTmpField, nLenWork);
	}


	/* PCI */
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sFormatedPan, 0, sizeof(sFormatedPan));
	memset(sPlainTextData, 0, sizeof(sPlainTextData));
	memset(sSecureMsgData, 0, sizeof(sSecureMsgData));
	memset(sIssScriptData, 0, sizeof(sIssScriptData));
	memset(sIssCmdData, 0, sizeof(sIssCmdData));
	memset(sSMI_Key, 0, sizeof(sSMI_Key));
	memset(sSMC_Key, 0, sizeof(sSMC_Key));
	memset(sAC_Key, 0, sizeof(sAC_Key));
	memset(sPVK_Key, 0, sizeof(sPVK_Key));
	memset(sCardPinOffset, 0, sizeof(sCardPinOffset));
	memset(szWork, 0, sizeof(szWork));
	memset(sTmpField, 0, sizeof(sTmpField));
	memset(sPinPek, 0, sizeof(sPinPek));
	memset(sPek, 0, sizeof(sPek));
	memset(sSrcPBFrmt, 0, sizeof(sSrcPBFrmt));
	memset(sTag71, 0, sizeof(sTag71));
	memset(sTag72, 0, sizeof(sTag72));
	memset(sPan, 0, sizeof(sPan));
	memset(sPan12     , 0, sizeof(sPan12     ));
	memset(sPinBlkBCD , 0, sizeof(sPinBlkBCD ));
	InitTlvPrivate(&tlvInfo);
	trace_event("End   gen_iss_script_data(OK)", PROCESSING);
	return(OK);
}


/*Start PROD00073147*/

int		MC_Spa2_AAV_implementation(int nIndiceCtx,
	char * aav_data,
	char *Pan,/* NAB25062020 PLUTONL-2053*/
	/*int  nCvki,*//*ASO20200403*/
	char *DSTransactionID,
	char *TxCurr,
	char *TxAmount,
	char *MerchantName,
	char *sHsmResultCode){


	unsigned char  BuffSnd   [1024];
	unsigned char  buffer_received   [1024];
	unsigned char    CompressedAmount[4 + 1];
	unsigned char    MerchantNameHash[64 + 1];
	char 	sIAVMerchant[8 + 1];
	int     offset = 0;
	char	strPadd [20 + 1];
	unsigned char	sHMAC_Key [SIZE_KEY_TDES + 1];
	unsigned char   sHMAC[64 + 1];
	int     retour;
	char	err_code[4 + 1];
	unsigned char	sMsgData[256 + 1];
	pid_t           nPid;
	int 			nLength;

	char				key_id[16];/*ASO20200403*/


	trace_event("Start   MC_Spa2_AAV_implementation()",PROCESSING);

	memset(BuffSnd               , 0, sizeof(BuffSnd));
	memset(strPadd               , 0, sizeof(strPadd));
	memset(CompressedAmount               , 0, sizeof(CompressedAmount));
	memset(MerchantNameHash               , 0, sizeof(MerchantNameHash));
	memset(sMsgData               , 0, sizeof(sMsgData));
	memset(sHMAC_Key               , 0, sizeof(sHMAC_Key));
	memset(sHMAC               , 0, sizeof(sHMAC));
	memset(err_code            , 0, sizeof(err_code));
	memset(sIAVMerchant        , 0, sizeof(sIAVMerchant));
	memset(buffer_received     , 0, sizeof(buffer_received));
	memset(key_id, 0, sizeof(key_id)); /*ASO20200403*/


	CompressAmount(TxAmount, CompressedAmount);
	Sha256_ALgo(MerchantName, MerchantNameHash);

	memcpy(strPadd, "FFFFFFFFFFFFFFFFFFFF", 20);


	/*IAV := MS( 4, HMAC-SHA256( ACSkey, PAN | DS Transaction ID | Purchase Currency | Transaction Amount | Hash of Merchant Name )*/

	/*sMsgData := PAN (F right padded ) | DS Transaction ID | Purchase Currency | Transaction Amount | Hash of Merchant Name */

	memcpy(sMsgData, Pan, strlen(Pan));
	offset = offset + strlen(Pan);

	memcpy(sMsgData + offset, strPadd, 20 -  strlen(Pan));
	offset = 20;


	if(strlen(DSTransactionID) > 0)
	{
		memcpy(sMsgData + offset, DSTransactionID, strlen(DSTransactionID));
		offset = offset + strlen(DSTransactionID);
	}

	if(strlen(TxCurr) > 0)
	{
		sprintf(sMsgData + offset, "%04d", atoi(TxCurr));
		offset = offset + 4;
	}
	if(strlen(CompressedAmount) > 0)
	{
		memcpy(sMsgData + offset,  CompressedAmount, strlen(CompressedAmount));
		offset = offset + 4;
	}
	if(strlen(MerchantNameHash) > 0)
	{
		memcpy(sMsgData + offset,  MerchantNameHash, 8);
		offset = offset + 8;
	}


	memcpy(sIAVMerchant, aav_data + 4, 8);

	memcpy(key_id, aav_data + 21, 1); 	/*ASO20200403*/

	/*HMAC KEY */
	retour = Get_UCAF_Key(nIndiceCtx, Pan,/*nCvki,*/ key_id, sHMAC_Key);	/*ASO20200403*/
	if ((retour != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		return(HSM_UNAVAILABLE);
	}

	retour = generate_mac_with_hmac(nIndiceCtx,
									sHMAC_Key,
									"",
									"2",
									sMsgData,
									sHMAC,
									sHsmResultCode);

	if (retour != OK)
	{
		trace_event("End MC_Spa2_AAV_implementation(ERROR_COMMAND)", PROCESSING);
		return(HSM_MALFUNCTION);
	}
	else
	{

		if(memcmp(sHMAC, sIAVMerchant, 8) == 0)
		{
			trace_event("End   MC_Spa2_AAV_implementation(OK)", PROCESSING);
			return(OK);
		}
		else
		{
			trace_event("End   MC_Spa2_AAV_implementation(VERIFICATION_FAILED)", PROCESSING);
			return(VERIFICATION_FAILED);
		}

	}


}
