#include <stdio.h>
#include <hsm_inc.h>
#include <resources.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <stdlib.h>
#include <security_data.h>
#include <security_flags.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <tlv_autho.h>
#include <math.h>
#include <openssl/sha.h>
#include <stdint.h> 
#include <inttypes.h>


#define MAX_ISS_SCRIPT				3

/*---------------------------------------------------------------------------------------------------------------------------------
Id                 Date                Author                          Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK-HF         25-08-2003          Mah.Karroum & Hafida Fouta        Medification de la fonction check_pin_data
---------------------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************/
/* Fonction check_cvv_data.                                                   */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le cvv recu.               */
/******************************************************************************/

int
	check_cvv_data(
	int nIndiceCtx,
	char *Pan,
	char *CvkA,
	char *CvkB,
	char *expiry_date,
	char *service_code,
	char *cvv,
	char *sHsmResultCode
	)
{
	unsigned char   BuffSnd[BUF_LG_MAX];
	unsigned char   BuffRcv[256];
	int             retour;
	pid_t           nPid;
	char            sLine[MAX_LINE_TRC];

	trace_event("Start check_cvv_data()", PROCESSING);

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));

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
	retour = command_CY(BuffSnd, Pan, service_code, expiry_date, CvkA, CvkB, cvv);

	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ((retour != OK)) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	retour = HsmQuery(nIndiceCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (retour == HSM_MALFUNCTION) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_cvv_data(MALFUNCTION)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	retour = command_CZ(BuffRcv, sHsmResultCode);
	if (retour != OK)
	{
		switch (retour) 
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

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));
	trace_event("End   check_cvv_data(OK)", PROCESSING);
	return (OK);
}

int  check_amx_csc_data(
	int nIndiceCtx,
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
	unsigned char   BuffSnd[BUF_LG_MAX];
	unsigned char   BuffRcv[256];
	char			sLocCSC[24];
	char			sCsck[SIZE_KEY_TDES + 1];
	char			sCsc_value[64];
	char			sKeyInd[3];
	int             retour;
	pid_t           nPid;
	char            sLine[MAX_LINE_TRC];
	char            sExpDate[5];
	char            sScode[4];

	trace_event("Start check_amx_csc_data()", PROCESSING);

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));
	memset(sLocCSC,0,sizeof(sLocCSC));
	memset(sCsck,0,sizeof(sCsck));
	memset(sCsc_value,0,sizeof(sCsc_value));
	memset(sExpDate,'\0',sizeof(sExpDate));
	memset(sScode,'\0',sizeof(sScode));
	memset(sKeyInd,'\0',sizeof(sKeyInd));

	if(flag == AMX_AEVV )
	{
		memcpy(sScode, nCSC_field + 1, 1);
		memcpy(sScode + 1, nCSC_field + 2, 2);
		memcpy(sExpDate, nCSC_field + 10, 4);
		memcpy(sCsc_value, nCSC_field + 7, 3);
		
		/*Start AMER20211013 PLUTONL-3573*/
		memcpy(sKeyInd, nCSC_field + 4, 2);	
		
		retour = Get_CAVV_Key(nIndiceCtx, Pan, sKeyInd, CvkA, CvkB); 

		if ((retour != OK))
		{
			memcpy(sHsmResultCode, "UNVL", 4);
			return(HSM_UNAVAILABLE);
		}
		/*End AMER20211013 PLUTONL-3573*/
	}	
	else
	{
		memcpy(sScode , service_code , 3);
		memcpy(sExpDate, expiry_date , 4);	
		memcpy(sCsc_value, nCSC_field,strlen(nCSC_field));
	}
	
	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %s", Pan);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "CSC TYPE                : %d", nCSCType);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "EXPIRY DATE                 : %.4s", expiry_date);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "Service Code          : %.3s",service_code ); /*NAK CSC V2*/
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "CVKA                        : %.48s", CvkA);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "CVKB                        : %.48s", CvkB);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "CSC FIELD                        : %s", nCSC_field);
		trace_event(sLine, PROCESSING);
	
		if(flag == AMX_AEVV )
		{
			sprintf(sLine, "Key indicator       : %s", sKeyInd);
			trace_event(sLine, PROCESSING);
		}
		
		trace_event("- CVV Verification ---------------", PROCESSING);

	}
	
	/*AMER201211012 PLUTONL-3573: Don't check the AEVV if it's an attempt*/
	if( memcmp(sKeyInd, "98", 2) == 0 || memcmp(sKeyInd, "99", 2) == 0)
	{
		trace_event("AEVV generated by the network: Not to be checked", PROCESSING);
		trace_event("End check_amx_csc_data(NOK)", PROCESSING);
		return NOK;
	}

	memset(sLocCSC,'F',12);
	if( nCSCType == AMX_CSC5 )
	{
		memcpy(sLocCSC + 0, sCsc_value, 5);		
	}
	else if( nCSCType == AMX_CSC4 )
	{
		memcpy(sLocCSC + 5, sCsc_value, 4);

	}
	else
	{
		memcpy(sLocCSC + 9, sCsc_value, 3);

	}

	if( CvkA[0] == 'S' || CvkA[0] == 'U')
	{
		memcpy(sCsck,CvkA,strlen(CvkA));
	}
	else
	{
		memcpy(sCsck,CvkA,16);
		memcpy(sCsck + 16,CvkB,16);
	}

	retour = command_RY(BuffSnd, Pan, sExpDate,sScode, sCsck, sLocCSC,flag);

	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ((retour != OK)) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_amx_csc_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	retour = HsmQuery(nIndiceCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_amx_csc_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (retour == HSM_MALFUNCTION) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_amx_csc_data(MALFUNCTION)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	retour = command_RZ(BuffRcv, sHsmResultCode);
	if (retour != OK)
	{
		switch (retour) 
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

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));
	trace_event("End   check_amx_csc_data(OK)", PROCESSING);
	return (OK);
}

/*START 3D-SECURE MCK26072012*/
/******************************************************************************/
/* Fonction check_cavv_data.                                                  */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le cavv recu.                           */
/******************************************************************************/
int     check_cavv_data(
	int nIndiceCtx,
	char *Pan,
	char *cavv_data,
	char *sHsmResultCode
	)
{
	unsigned char  	BuffSnd[BUF_LG_MAX];
	unsigned char  	BuffRcv[256];
	char 						sLine[MAX_LINE_TRC];
	char 						cavv_value[4];
	char 						atn[5];
	char 						autho_result[2];
	char 						second_factor[3];
	char						cavv_key[SIZE_KEY_TDES + 1];
	char						service_code[4];
	char						CvkA[SIZE_KEY_TDES + 1];
	char						CvkB[SIZE_KEY_TDES + 1];
	char						key_indicator[3];

	int      				retour;
	pid_t    				nPid;

	memset(sLine, 0, sizeof(sLine));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(cavv_value, 0, sizeof(cavv_value));
	memset(atn, 0, sizeof(atn));
	memset(autho_result, 0, sizeof(autho_result));
	memset(second_factor, 0, sizeof(second_factor));
	memset(cavv_key, 0, sizeof(cavv_key));
	memset(service_code, 0, sizeof(service_code));
	memset(CvkA, 0, sizeof(CvkA));
	memset(CvkB, 0, sizeof(CvkB));
	memset(key_indicator, 0, sizeof(key_indicator));



	trace_event("Start check_cavv_data()", PROCESSING);

    /*START NAB10082020 PLUTONL-2170*/
	if (cavv_data[30]== '7') /*7: Version 7—CAVV with Supplemental Data*/
	{
		
		retour = check_cavv_data_u3_v7 (nIndiceCtx,
			Pan,
			cavv_data,
			sHsmResultCode
			);
	
	    return(retour);
	
	}

	/*END NAB10082020 PLUTONL-2170*/
	memcpy(autho_result, cavv_data + 1, 1);  /* MJA130211 */
	memcpy(second_factor, cavv_data + 2, 2);
	memcpy(key_indicator, cavv_data + 4, 2);
	memcpy(cavv_value, cavv_data + 7, 3);
	memcpy(atn, cavv_data + 10, 4);

	/*Start AMER20180324 PROD00054205: VISA Enh18.Q2 Art 1.1: Don't check the CAVV if it's an attempt*/
	if( ((autho_result[0] == '7' || autho_result[0] == '8') &&
		(memcmp(key_indicator, "10", 2) == 0  || memcmp(key_indicator, "11", 2) == 0)) ||
		(memcmp(key_indicator, "12", 2) == 0 || memcmp(key_indicator, "13", 2) == 0))	/*AEA20210712 Enh21.Q4 Art3.7: Don't check the CAVV if key indicator equals 12 or 13*/
	{	 
		trace_event("CAVV GENERATED BY NETWORK: Not to be checked", PROCESSING);
		trace_event("End check_cavv_data(NOK)", PROCESSING);
		return NOK;
	}
	/*End AMER20180324 PROD00054205: VISA Enh18.Q2 Art 1.1: Don't check the CAVV if it's an attempt*/

	retour = Get_CAVV_Key(nIndiceCtx, Pan, key_indicator, CvkA, CvkB); 

	if ((retour != OK))
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

	retour = command_CY(BuffSnd, Pan, service_code, atn, CvkA, CvkB, cavv_value);


	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ((retour != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cavv_data(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}


	retour = HsmQuery(nIndiceCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cavv_data(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_cavv_data(MALFUNCTION)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	retour = command_CZ(BuffRcv, sHsmResultCode);
	if (retour != OK)
	{
		switch (retour)
		{
		case VERIFICATION_FAILED:
			trace_event("End   check_cavv_data(VERIFICATION_FAILED)", PROCESSING);
			return(VERIFICATION_FAILED);

		case ERROR_COMMAND:
			trace_event("End check_cavv_data(ERROR_COMMAND)", PROCESSING);
			return(HSM_MALFUNCTION);

		default:
			trace_event("End   check_cavv_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
			return(CRYPTOGRAPHIC_ERROR);
		}
	}

	memset(sLine, 0, sizeof(sLine));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
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
/*Start NAB10082020*/

/******************************************************************************/
/* Fonction check_cavv_data_u3_v7.                                                  */
/* -----------------------                                                         */
/* Description : Fonction qui verifie le cavv recu  v7.                           */
/******************************************************************************/
int     check_cavv_data_u3_v7(
	int nIndiceCtx,
	char *Pan,
	char *cavv_data,
	char *sHsmResultCode
	)
{
	unsigned char  	BuffSnd[BUF_LG_MAX];
	unsigned char  	BuffRcv[256];
	char 						sLine[MAX_LINE_TRC];
	char 						cavv_value[4];
	char 						seed_value[5];
	char 						autho_result[2];
	char 						second_factor[3];
	char						cavv_key[SIZE_KEY_TDES + 1];
	char						service_code[4];
	char						CvkA[SIZE_KEY_TDES + 1];
	char						CvkB[SIZE_KEY_TDES + 1];
	char						key_indicator[3];
	char                        sPan_v7_Out[16+1] ;
	int      				    retour;
	pid_t    				    nPid;

	memset(sLine, 0, sizeof(sLine));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(cavv_value, 0, sizeof(cavv_value));
	memset(seed_value, 0, sizeof(seed_value));
	memset(autho_result, 0, sizeof(autho_result));
	memset(second_factor, 0, sizeof(second_factor));
	memset(cavv_key, 0, sizeof(cavv_key));
	memset(service_code, 0, sizeof(service_code));
	memset(CvkA, 0, sizeof(CvkA));
	memset(CvkB, 0, sizeof(CvkB));
	memset(key_indicator, 0, sizeof(key_indicator));
	memset(sPan_v7_Out, 0, sizeof(sPan_v7_Out));



	trace_event("Start check_cavv_data_u3_v7()", PROCESSING);
    
	
       retour = Pan_v7_conversion(Pan,cavv_data ,sPan_v7_Out);
	

	memcpy(autho_result, cavv_data + 1, 1);  /* MJA130211 */
	memcpy(second_factor, cavv_data + 2, 2);
	memcpy(key_indicator, cavv_data + 4, 2);
	memcpy(cavv_value, cavv_data + 7, 3);
	memcpy(seed_value, cavv_data + 10, 4);

	/*Start AMER20180324 PROD00054205: VISA Enh18.Q2 Art 1.1: Don't check the CAVV if it's an attempt*/
	if( (autho_result[0] == '7' || autho_result[0] == '8') && 
		(memcmp(key_indicator, "10", 2) == 0  || memcmp(key_indicator, "11", 2) == 0) ||
		(memcmp(key_indicator, "12", 2) == 0  || memcmp(key_indicator, "13", 2) == 0))	/*AMERA20211026 Enh21.Q4 Art3.7: Don't check the CAVV if key indicator equals 12 or 13*/		
	{	 
		trace_event("CAVV GENERATED BY NETWORK: Not to be checked", PROCESSING);
		trace_event("End check_cavv_data_u3_v7(NOK)", PROCESSING);
		return NOK;
	}
	/*End AMER20180324 PROD00054205: VISA Enh18.Q2 Art 1.1: Don't check the CAVV if it's an attempt*/
	retour = Get_CAVV_Key(nIndiceCtx, Pan, key_indicator, CvkA, CvkB); 

	if ((retour != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		return(HSM_UNAVAILABLE);
	}


	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %s", Pan);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "PAN V7                        : %s", sPan_v7_Out);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "RESULT CODE                 : %.1s", autho_result);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "SECOND FACTOR               : %.2s", second_factor);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "SEED VALUE                         : %.4s", seed_value);
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

	retour = command_CY(BuffSnd, sPan_v7_Out, service_code, seed_value, CvkA, CvkB, cavv_value);


	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ((retour != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cavv_data_u3_v7(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}


	retour = HsmQuery(nIndiceCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cavv_data_u3_v7(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_cavv_data_u3_v7(MALFUNCTION)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	retour = command_CZ(BuffRcv, sHsmResultCode);
	if (retour != OK)
	{
		switch (retour)
		{
		case VERIFICATION_FAILED:
			trace_event("End   check_cavv_data_u3_v7(VERIFICATION_FAILED)", PROCESSING);
			return(VERIFICATION_FAILED);

		case ERROR_COMMAND:
			trace_event("End check_cavv_data_u3_v7(ERROR_COMMAND)", PROCESSING);
			return(HSM_MALFUNCTION);

		default:
			trace_event("End   check_cavv_data_u3_v7(CRYPTOGRAPHIC_ERROR)", PROCESSING);
			return(CRYPTOGRAPHIC_ERROR);
		}
	}

	memset(sLine, 0, sizeof(sLine));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(cavv_value, 0, sizeof(cavv_value));
	memset(seed_value, 0, sizeof(seed_value));
	memset(autho_result, 0, sizeof(autho_result));
	memset(second_factor, 0, sizeof(second_factor));
	memset(cavv_key, 0, sizeof(cavv_key));
	memset(service_code, 0, sizeof(service_code));
	memset(CvkA, 0, sizeof(CvkA));
	memset(CvkB, 0, sizeof(CvkB));
	memset(key_indicator, 0, sizeof(key_indicator));

	trace_event("End   check_cavv_data_u3_v7(OK)", PROCESSING);
	return(OK);
}
/*End NAB10082020*/

/******************************************************************************/
/* Fonction check_aav_data.                                                  */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le aav recu.                           */
/******************************************************************************/

int     check_aav_data(
	int nIndiceCtx,
	char *Pan,
	/*int  nCvki,*//*ICH20200310*/
	char *DSTransactionID,
	char *TxCurr,
	char *TxAmount,
	char *MerchantName,
	char  sProgramProtocol,
	char *aav_data_b64,
	char *sHsmResultCode
	)
{
	unsigned char  	BuffSnd[BUF_LG_MAX];
	unsigned char  	BuffRcv[256];
	char 						sLine[MAX_LINE_TRC];
	char						aav_data[256];
	char 						aav_value[16];
	char 						tsn[16];
	char 						auth_method[16];
	char 						control_byte[16];
	char						aav_key[SIZE_KEY_TDES + 1];
	char						key_id[16];
	char						service_code[16];
	char						str_temp[16];
	char						CvkA[SIZE_KEY_TDES + 1];
	char						CvkB[SIZE_KEY_TDES + 1];

	int      				retour;
	int							int_temp, i;
	pid_t    				nPid;

	memset(sLine, 0, sizeof(sLine));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(aav_data, 0, sizeof(aav_data));
	memset(aav_value, 0, sizeof(aav_value));
	memset(tsn, 0, sizeof(tsn));
	memset(auth_method, 0, sizeof(auth_method));
	memset(control_byte, 0, sizeof(control_byte));
	memset(aav_key, 0, sizeof(aav_key));
	memset(key_id, 0, sizeof(key_id));
	memset(service_code, 0, sizeof(service_code));
	memset(CvkA, 0, sizeof(CvkA));
	memset(CvkB, 0, sizeof(CvkB));
	memset(str_temp, 0, sizeof(str_temp));


	trace_event("Start check_aav_data()", PROCESSING);


	retour = Base64_Decode(nIndiceCtx, aav_data_b64, aav_data);

	/*CONTROL BYTE*/
	/*NAK-START SIB ECOM MNG 06032017*/
	/*Issuers should not perform SecureCode validation on static AAVs in DE 48,
	*        subelement 43 nor on attempts AAV generated by MasterCard */

    /* START ELKAM20230202 ENH_2023Q2 AN 7090 */
    /*if(aav_data[1] == '6' || memcmp(aav_data_b64, "kL", 2) == 0 || memcmp(aav_data_b64, "kM", 2) == 0 ||	/*AMER20180324 PROD00054205: MCI Enh18.Q2 AN1085: Added the check of kL and kM for attempt CAVV generated with SPA2 algorithm* /
	memcmp(aav_data_b64, "kE", 2) == 0 || memcmp(aav_data_b64, "kF", 2) == 0 || memcmp(aav_data_b64, "kN", 2) == 0 || memcmp(aav_data_b64, "kD", 2) == 0  )	/*AMER20190219 PROD00065991: MCI Enh19.Q2 AN2113*/ /*FZL20221025 PLUTONL-5267*/
	if(		aav_data[1] == '6' 
		||	memcmp(aav_data_b64, "kL", 2) == 0 
		|| 	memcmp(aav_data_b64, "kM", 2) == 0 	 /*AMER20180324 PROD00054205: MCI Enh18.Q2 AN1085: Added the check of kL and kM for attempt CAVV generated with SPA2 algorithm*/
		|| 	memcmp(aav_data_b64, "kE", 2) == 0 
		|| 	memcmp(aav_data_b64, "kF", 2) == 0 
		|| 	memcmp(aav_data_b64, "kN", 2) == 0 
		|| 	memcmp(aav_data_b64, "kD", 2) == 0 	 /*AMER20190219 PROD00065991: MCI Enh19.Q2 AN2113*/ /*FZL20221025 PLUTONL-5267*/
		|| 	memcmp(aav_data_b64, "kX", 2) == 0   
		|| 	memcmp(aav_data_b64, "kY", 2) == 0   
		|| 	memcmp(aav_data_b64, "kZ", 2) == 0 )
    /* END ELKAM20230202 ENH_2023Q2 AN 7090 */
	{	
		trace_event("AAV GENERATED BY NETWORK(NOK)", PROCESSING);
		trace_event("End check_aav_data(NOK)", PROCESSING);
		return NOK;
	}	
	/*NAK-END SIB ECOM MNG 06032017*/


	/*PROD00073147 Start SPA2*/

	/* START ELKAM20230202 ENH_2023Q2 AN 7090 */
	/* if(sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_0  || aav_data_b64[0] == 'k'){ */
	if(		sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_0  
		||	sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_1  
		||	sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_2  
		||	sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_3  
		||	sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_4  
		||	sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_5  
		||	sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_6  
		||	sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_7  
		||	sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_8  
		||	sProgramProtocol == P7_F048_P66_POS2_3DSEC_2_9  
		||	aav_data_b64[0]  == 'k')
	{
	/* END ELKAM20230202 ENH_2023Q2 AN 7090 */
		retour = MC_Spa2_AAV_implementation(nIndiceCtx, aav_data, Pan, /*nCvki,*/DSTransactionID, TxCurr, TxAmount, MerchantName, sHsmResultCode);/*ASO20200403*/

		if(retour == OK)
		{
			trace_event("End   check_aav_data(OK)", PROCESSING);
			return(OK);
		}
		else
		{
			switch (retour)
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
	/*	memcpy(auth_method, aav_data + 21, 1);*//*MODIF NAK*/
	memcpy(auth_method, aav_data + 20, 1);

	/*BIN Key Id*/
	/*	memcpy(key_id, aav_data + 22, 1);*//*MODIF NAK*/
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
	/* 	memcpy(aav_value, aav_data + 32, 3);*//*MODIF NAK*/
	memcpy(aav_value, aav_data + 31, 3);


	/*AAV KEY */
	retour = Get_UCAF_Key(nIndiceCtx, Pan,/*nCvki,*/ key_id, aav_key);
	if ((retour != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		return(HSM_UNAVAILABLE);
	}

	/*if( CvkA[0] == 'S' || CvkA[0] == 'U')*/
	if( aav_key[0] == 'S' || aav_key[0] == 'U')	/*AMER20200819 PLUTONL-2164*/
	{
		memcpy(CvkA,aav_key,strlen(aav_key));
	}
	else
	{
		memcpy(CvkA, aav_key, 16);
		memcpy(CvkB, aav_key + 16, 16);
	}
	



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

	retour = command_CY(BuffSnd, Pan, service_code, tsn, CvkA, CvkB, aav_value);


	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ((retour != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_aav_data(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	} 


	retour = HsmQuery(nIndiceCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_aav_data(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_aav_data(MALFUNCTION)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	retour = command_CZ(BuffRcv, sHsmResultCode);
	if (retour != OK)
	{
		switch (retour)
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

	/* PCI */
	memset(sLine, 0, sizeof(sLine));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(aav_data, 0, sizeof(aav_data));
	memset(aav_value, 0, sizeof(aav_value));
	memset(tsn, 0, sizeof(tsn));
	memset(auth_method, 0, sizeof(auth_method));
	memset(control_byte, 0, sizeof(control_byte));
	memset(aav_key, 0, sizeof(aav_key));
	memset(key_id, 0, sizeof(key_id));
	memset(service_code, 0, sizeof(service_code));
	memset(CvkA, 0, sizeof(CvkA));
	memset(CvkB, 0, sizeof(CvkB));
	memset(str_temp, 0, sizeof(str_temp));
	trace_event("End   check_aav_data(OK)", PROCESSING);
	return(OK);
}


/*END   3D-SECURE MCK26072012*/

/******************************************************************************/

/* Fonction check_pvv_data.                                                   */

/* -----------------------                                                    */

/* Description : Fonction qui verifie le pvv recu.               */

/******************************************************************************/

int check_pvv_data(int nIndiceCtx,
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

	int             retour;
	pid_t           nPid;

	unsigned char   BuffSnd[BUF_LG_MAX];
	unsigned char   BuffRcv[BUF_LG_MAX];/*NAB18022019*/

	char            sLine[MAX_LINE_TRC];

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));

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

	if (mode == TERMINAL_MODE)
		retour = command_DC(BuffSnd, trans_key, PvkA, PvkB, PinBlock, format, Pan, pvv);
	else
		retour = command_EC(BuffSnd, trans_key, PvkA, PvkB, PinBlock, format, Pan, pvv);

	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ((retour != OK)) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);

	} 

	retour = HsmQuery(nIndiceCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_pvv_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (retour == HSM_MALFUNCTION) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_pvv_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (mode == TERMINAL_MODE)
		retour = command_DD(BuffRcv, sHsmResultCode);
	else
		retour = command_ED(BuffRcv, sHsmResultCode);

	if (retour != OK) {
		switch (retour) {
		case VERIFICATION_FAILED:
			trace_event("End   check_pvv_data(VERIFICATION_FAILED)", PROCESSING);
			return (VERIFICATION_FAILED);

		case ERROR_COMMAND:
			trace_event("End   check_pvv_data(ERROR_COMMAND)", PROCESSING);
			return (HSM_MALFUNCTION);

		default:
			trace_event("End   check_pvv_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
			return (CRYPTOGRAPHIC_ERROR);

		}

	}

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));

	trace_event("End   check_pvv_data(OK)", PROCESSING);
	return (OK);
}



/******************************************************************************/
/* Fonction check_offset_data.                                                */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le offset.                              */
/******************************************************************************/

int
	check_offset_data(int nIndiceCtx,
	char *Pan, char *PvkA,
	char *PinBlock, char *format, char *trans_key,
	char *offset, char mode,
	char *sHsmResultCode, int PinLength)
{
	unsigned char   BuffSnd[BUF_LG_MAX];/*NAB18022019 Key Block*/
	unsigned char   BuffRcv[BUF_LG_MAX];
	pid_t           nPid;
	int             retour;
	char            sLine[MAX_LINE_TRC];
	char			sDecTable[SIZE_KEY+1];

	trace_event("Start check_offset_data()", PROCESSING);

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));
	memset(sDecTable,   0, sizeof(sDecTable));
	
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

	/*PROD00053984*/
	GetHsmDecTable(sDecTable);

	if (mode == TERMINAL_MODE)
		retour = command_DA(BuffSnd, trans_key, PvkA, PinBlock, format, Pan, offset, PinLength,sDecTable);
	else
		retour = command_EA(BuffSnd, trans_key, PvkA, PinBlock, format, Pan, offset, PinLength,sDecTable);

	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ((retour != OK)) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_offset_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	} 

	retour = HsmQuery(nIndiceCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_offset_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (retour == HSM_MALFUNCTION) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_offset_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	if (mode == TERMINAL_MODE)
		retour = command_DB(BuffRcv, sHsmResultCode);
	else
		retour = command_EB(BuffRcv, sHsmResultCode);

	if (retour != OK) {
		switch (retour) {
		case VERIFICATION_FAILED:
			trace_event("End   check_offset_data(VERIFICATION_FAILED)", PROCESSING);
			return (VERIFICATION_FAILED);

		case ERROR_COMMAND:
			trace_event("End   check_offset_data(ERROR_COMMAND)", PROCESSING);
			return (HSM_MALFUNCTION);

		default:
			trace_event("End   check_offset_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
			return (CRYPTOGRAPHIC_ERROR);
		}
	}

	/* PCI */
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));
	trace_event("End   check_offset_data(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction check_crpt_data.                                                  */
/* -----------------------                                                    */
/******************************************************************************/
/*PROD00068067: Start*/


int
	check_crpt_data(
	int nIndexCtx,
	char *sTlvBuffer,
	char *sHsmResultCode
	)
{

	unsigned char   BuffSnd[512];
	unsigned char   BuffRcv[512];
	unsigned char   sPan[22 + 1];
	unsigned char   sSeqNumber[8 + 1];
	unsigned char   sFormatedPan[12 + 1];
	unsigned char   sAtc[2 + 1];
	unsigned char   sUn[4 + 1];


	unsigned char   sArqc[8 + 1];
	unsigned char   sArpc[8 + 1];
	unsigned char   sAC_Key[SIZE_KEY_TDES + 1];
	unsigned char   sArc[256];
	unsigned char   cSchemeId;
	unsigned char   sCsu[8 + 1];
	unsigned char   sArcCsu[8 + 1];

	char			sNetworkCode[2 + 1];
	char            sIAD[512 +1];

	char			mode_flag;
	char			arpc_mthd;
	char			crd_key_drv_mthd;
	char			sk_drv_mthd;
	char			sCVN[2 + 1];
	char			sCVNBin[1 + 1];

	TSTlvPrivate    tlvInfo;

	char            szWork[512];
	char            sIccData[512];
	char            sTmpField[512];
	char            sLine[MAX_LINE_TRC];
	char            sIad[64 + 1];
	char			sData[1024];
	char            sEmvCardInfoData[128];

	int             nRetCode = 0;
	int             nLenWork = 0;
	int             nLenData = 0;
	int             nLength = 0;
	int             nTmpLen = 0;
	int             nIccLen = 0;
	int             nCvrLen = 0;
	int				nUseNewCmd;
	int				nDoPad80;
	int				nPanLength = 0; /*SKA05042023 - PLUTONL-6022*/
	char            sPaddedPan[24 + 1]; /*SKA05042023 - PLUTONL-6022*/
	pid_t           nPid;
	

	



	trace_event("Start check_crpt_data()", PROCESSING);

	InitTlvPrivate(&tlvInfo);
	memset(sAtc, 0, sizeof(sAtc));
	memset(sUn, 0, sizeof(sUn));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sArqc, 0, sizeof(sArqc));
	memset(sArpc, 0, sizeof(sArpc));
	memset(sFormatedPan, 0, sizeof(sFormatedPan));
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
	memset(sCVNBin, 0, sizeof(sCVNBin));
	memset(sPaddedPan, '0', sizeof(sPaddedPan));
	nLenData = 0;
	nUseNewCmd = NOK;
	nDoPad80 = NOK;
	

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

	nPanLength = strlen(sPan); /*SKA05042023 - PLUTONL-6022*/

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
	AsciiToHex(sCVN,sCVNBin,2);

	if( arpc_mthd == EMV_ARPC_MTHD_2 )
	{
		mode_flag = '3';
	}
	else
	{
		mode_flag = '1';
	}


	nUseNewCmd = OK;
	cSchemeId = '?';
	if( crd_key_drv_mthd == EMV_MKD_A )
	{
		switch( sk_drv_mthd  )
		{
		case EMV_SKD_EMV2000:	cSchemeId = HTC_KW_SC_MK_A_SK_EMV2000; break;
		case EMV_SKD_CSK:		cSchemeId = HTC_KW_SC_MK_A_SK_CSK; break;
		case EMV_SKD_MCI_SKD:	cSchemeId = HTC_KQ_SC_MK_A_SK_MCI_SKD; nUseNewCmd = NOK;break;
		case EMV_SKD_UPI_SKD:	cSchemeId = HTC_KW_SC_MK_A_SK_UPI; break;
		case EMV_SKD_JCB_SKD:	cSchemeId = HTC_KW_SC_MK_A_SK_JCB; break;
		default:				cSchemeId = '?'; break;
		case EMV_SKD_NONE:
			if ( memcmp(sNetworkCode, NET_VISA, NET_LEN) == 0 || memcmp(sNetworkCode, NET_AMEX, NET_LEN) == 0)
			{
				cSchemeId = HTC_KQ_SC_MK_A_SK_NONE;
				nUseNewCmd = NOK;
			}
			else if ( memcmp(sNetworkCode, NET_JCB, NET_LEN) == 0)
			{
				cSchemeId = HTC_KW_SC_MK_A_SK_NONE;
			}
			break;
		}
	}
	else if( crd_key_drv_mthd == EMV_MKD_B )
	{
		if( sk_drv_mthd == EMV_SKD_EMV2000 )
		{
			cSchemeId = HTC_KW_SC_MK_B_SK_EMV2000;
		}
		else if( sk_drv_mthd == EMV_SKD_CSK )
		{
			cSchemeId = HTC_KW_SC_MK_B_SK_CSK;
		}
		else
		{
			cSchemeId = '?';
		}
	}
	else
	{
		cSchemeId = '?';
	}

	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %s", sPan);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "SEQUENCE NUMBER             : %s", sSeqNumber);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "AC KEY IS                   : %s", sAC_Key);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "PRE FORMATED PAN            : %s", szWork);
		trace_event(sLine, PROCESSING);
	}
	sprintf(sLine, "PARAMETERS		: |%c|%c|%s|", mode_flag,cSchemeId,sCVN);
	trace_event(sLine, PROCESSING);


	/* PreFormated Pan */
	memset(szWork, '0', 16);
	memcpy(sTmpField, sPan, strlen(sPan));
	memcpy(sTmpField + strlen(sPan), sSeqNumber + 1, 2);
	nLenWork = strlen(sTmpField);

	if (crd_key_drv_mthd == EMV_MKD_A)
	{
		nPanLength = 16;

		if (nLenWork < 16)
			memcpy(szWork + (16 - nLenWork), sTmpField, nLenWork);
		else
			memcpy(szWork, sTmpField + (nLenWork - 16), 16);

		AsciiToHex(szWork, sFormatedPan, nPanLength);	
	}
	else if  (crd_key_drv_mthd == EMV_MKD_B) /*SKA05042023 - PLUTONL-6022*/
	{
		nPanLength = nLenWork;
		if (nPanLength % 2 != 0)
		{
			memcpy(sPaddedPan + 1, sTmpField, nPanLength);
			nPanLength++;
			memcpy(sTmpField, sPaddedPan, nPanLength);
		}

		AsciiToHex(sTmpField, sFormatedPan, nPanLength);
	}

	if( cSchemeId == '?' )
	{
		memcpy(sHsmResultCode, "-SKD", 4);
		trace_event("ERROR cSchemeId", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

	
	if( memcmp(sCVN,EMV_CVN_17,EMV_CVN_LEN) == 0 )
	{
		nRetCode = build_crpt_data_17(nIndexCtx, sTlvBuffer,sData,&nLenData,sHsmResultCode);
	}
	else
	{
		nRetCode = build_crpt_data_dflt(nIndexCtx, sTlvBuffer,sCVN,sData,&nLenData,sHsmResultCode);
	}
	if( nRetCode != OK )
	{
		memcpy(sHsmResultCode, "-DAT", 4);
		trace_event("ERROR WhiLe build_crpt_data_dflt() ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

/*Start ARC Feature ICH20191010*/
	/*Done fir VISA CVN18 for now, need to add others, CCD? CPA?*/
/*	if( memcmp(sCVN,EMV_CVN_18,EMV_CVN_LEN) == 0 )
	{	
		memcpy(szWork,"00820000",8); 
		AsciiToHex(szWork, sCsu, 8);		
	}
	else if( memcmp(sNetworkCode, NET_DINERS, NET_LEN) == 0 )
	{	
		memcpy(szWork,"001A0000",8);
		AsciiToHex(szWork, sCsu, 8);		
	}
*/
/*End Feature ICH20191010*/

	/*TODO: for CCD, CPA? */
	if ( memcmp(sNetworkCode, NET_MASTERCARD, NET_LEN) == 0 ||
		 memcmp(sNetworkCode, NET_DINERS, NET_LEN) == 0 ||
		(memcmp(sNetworkCode, NET_VISA, NET_LEN) == 0 && memcmp(sCVN,EMV_CVN_18,EMV_CVN_LEN) == 0)
	   )
	{
		nDoPad80 = OK;
	}



/*Start ARC Feature ICH20191010*/
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
/*End Feature ICH20191010*/

	memset(szWork, 0, sizeof(szWork));
	if (sCVNBin[0] & 0x80 )
	{
		if (strlen(sIAD) >= 36)
		{
			/*TODO: EBE: include counters, comment until we manage the CVN. this action should be done only if the most sifnificant bit is 1
			AsciiToHex(sIAD + 20, szWork, 16);
			memcpy(sData + nLenData, szWork, 8);
			nLenData += 8;*/
		}
	}

	if( nDoPad80 == OK )
	{
		sData[nLenData++] = 0x80;
	}

	if (GetTlvPrivate(CHIP_APP_CRYPTOGRAM_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
	{
		AsciiToHex(szWork, sArqc, 16);
	}

	if (GetTlvPrivate(CHIP_ATC_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
	{
		AsciiToHex(szWork, sAtc, 4);
	}
	
	if (GetTlvPrivate(CHIP_UNPRED_NUMBER_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
	{
		AsciiToHex(szWork, sUn, 8);
	}



	if ( nUseNewCmd == OK )
		nLength = command_KW(BuffSnd, mode_flag, cSchemeId, sAC_Key, sFormatedPan, nPanLength/2, sAtc, sUn, nLenData, sData, sArqc, sArc,sCsu);
	else
		nLength = command_KQ(BuffSnd, cSchemeId, sAC_Key, sFormatedPan, sAtc, sUn, nLenData, sData, sArqc, sArc);



	nRetCode = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ((nRetCode != OK)) {

		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_crpt_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);

	} 


	nRetCode = HsmQueryBin(nIndexCtx, nPid, BuffSnd, nLength, BuffRcv);

	if (nRetCode == NOT_AVAILABLE) 
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_crpt_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);

	}
	if (nRetCode == HSM_MALFUNCTION) 
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_crpt_data(MALFUNCTION)", PROCESSING);
		return (HSM_MALFUNCTION);
	}


	if ( nUseNewCmd == OK )
		nRetCode = command_KX(BuffRcv,sHsmResultCode,sArpc);
	else
		nRetCode = command_KR(BuffRcv, sHsmResultCode, sArpc);


	if (nRetCode != OK) 
	{
		switch (nRetCode) 
		{
		case VERIFICATION_FAILED:

			trace_event("End   check_crpt_data(VERIFICATION_FAILED)", PROCESSING);
			return (VERIFICATION_FAILED);
		case ERROR_COMMAND:
			trace_event("End check_crpt_data(ERROR_COMMAND)", PROCESSING);
			return (HSM_MALFUNCTION);
		default:
			trace_event("End   check_crpt_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
			return (CRYPTOGRAPHIC_ERROR);

		}

	}

	HexToAscii(sArpc, szWork, 16);
/*Start ARC Feature ICH20191010*/
/*	if( memcmp(sNetworkCode, NET_DINERS, NET_LEN) == 0) 
	{
		sprintf(szWork + 16, "%02X%02X", sCsu[0], sCsu[1]);
	}
	if( memcmp(sNetworkCode, NET_CUP, NET_LEN) == 0) 
	{
		sprintf(szWork + 16, "%02X%02X%02X%02X", sCsu[0], sCsu[1], sCsu[2], sCsu[3]); 
	}
	else
	{
		if( arpc_mthd == EMV_ARPC_MTHD_1 )
		{
		sprintf(szWork + 16, "%02X%02X", sArc[0], sArc[1]);
	}
	}*/

	if(arpc_mthd == EMV_ARPC_MTHD_1)
	{
		sprintf(szWork + 16, "%02X%02X", sArc[0], sArc[1]);
	}
	/*NAB06042021 PLUTONL-2944*/
	/*else
	{
		
		sprintf(szWork + 16, "%02X%02X%02X%02X", sCsu[0], sCsu[1], sCsu[2], sCsu[3]);*//*for mode flag 3 ARPC(8B) = the 4-byte calculated ARPC followed by the 4-byte CSU*/
		
	/*}*/

/*End ARC Feature ICH20191010*/

	sprintf(sLine, "==> Issuer Authen Data       : [%s]", szWork);
	trace_event(sLine, PROCESSING);

	
	PutTlvPrivate(CHIP_ISS_AUTHEN_DATA_TAG, &tlvInfo, szWork, strlen(szWork));


	PutTlvPrivate (CHIP_ISS_ARPC_RESP_CODE_TAG, &tlvInfo, szWork, 16);/*ICH ARPC*/
/*Moved 
    HexToAscii(sArc, szWork, 4);
    PutTlvPrivate (CHIP_AUTHOR_RESP_CODE, &tlvInfo, szWork, 4);/*ICH sArc*/

	nIccLen = PrivateBuildTlv(sIccData, &tlvInfo);
	PutTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sIccData, nIccLen);


	InitTlvPrivate(&tlvInfo);
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sFormatedPan, 0, sizeof(sFormatedPan));
	memset(sAC_Key, 0, sizeof(sAC_Key));
	memset(szWork, 0, sizeof(szWork));
	memset(sTmpField, 0, sizeof(sTmpField));
	memset(sPan, 0, sizeof(sPan));
	memset(sIccData, 0, sizeof(sIccData));
	trace_event("End   check_crpt_data(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction check_data_authen.                                                */
/* -----------------------       																							*/
/* Author			 : Loubna Hidraoui				                                      */
/* Description : Fonction qui verifie le DAC et le Dynamic number recu.       */
/******************************************************************************/
#ifdef P7_UNSUPPORTED
int     check_data_authen(
	int        nIndexCtx,
	char      *sTlvBuffer,
	char      *sHsmResultCode,
	char      *sDacFlag
	)
{
	unsigned char  BuffSnd[512];
	unsigned char  BuffRcv[512];
	unsigned char  sPan[22 + 1];  /* Preformated Pan/Sequence Nbr */
	unsigned char  sFormatedPan[8 + 1]; /* Preformated Pan/Sequence Nbr  */
	unsigned char  sSeqNumber[8 + 1];  /* Preformated Pan/Sequence Nbr */
	unsigned char  sAtc[2 + 1]; /* Transaction counter                  */
	unsigned char  sUn[4 + 1]; /* Unpredictible number                 */
	unsigned char  sDac[2 + 1];   /* Data Authentication Code           */
	unsigned char  sDn[2 + 1]; /* Dynamic Number			 */
	unsigned char  sAC_Key[256];   /* Master key For DAC/IDN Verification */

	unsigned char  cSchemeId;
	unsigned char  sModeFlag;    /** 0 for DAC/ 1 for DN        */

	TSTlvPrivate tlvInfo;

	char           szWork[1024];
	char           sIccData[512];
	char           sTmpField[512];
	char           sLine[MAX_LINE_TRC];



	int      nRetCode = 0;
	int      nLenWork = 0;
	int      nLenData = 0;
	int      nLength = 0;
	int      nTmpLen = 0;
	int      nIccLen = 0;
	pid_t    nPid;

	trace_event("Start check_data_authen()", PROCESSING);

	InitTlvPrivate(&tlvInfo);
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));

	memset(sFormatedPan, 0, sizeof(sFormatedPan));
	memset(sAtc, 0, sizeof(sAtc));
	memset(sUn, 0, sizeof(sUn));
	memset(sAC_Key, 0, sizeof(sAC_Key));
	memset(szWork, 0, sizeof(szWork));
	memset(sTmpField, 0, sizeof(sTmpField));
	/*memset(sTmpData, 0, sizeof(sTmpData));*/
	memset(sDac, 0, sizeof(sDac));
	memset(sDn, 0, sizeof(sDn));
	memset(sIccData, 0, sizeof(sIccData));
	memset(sPan, 0, sizeof(sPan));
	memset(sSeqNumber, 0, sizeof(sSeqNumber));
	memset(sLine,      0, sizeof(sLine));

	/** Retrieveing the Card Number **/
	if (GetTlvBuffer(AUTO_PAN, sTlvBuffer, sPan, &nLenData) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-PAN", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
		trace_event("END   check_data_authen(NOK)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	/** Retrieveing the Card Seq Number **/

	if (GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER, sTlvBuffer, sSeqNumber, &nLenData) != SUCCES)
	{
		memcpy(sHsmResultCode, "-PAN", 4);
		trace_event("ERROR WhiLe GetIsoIcTag(ISO_TAG_TRANS_SEQ_NUM) ", ERROR);
		trace_event("END   check_data_authen(NOK)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                         : %s", sPan);
		trace_event(sLine, PROCESSING);

		sprintf(sLine, "SEQUENCE NUMBER             : %s", sSeqNumber);
		trace_event(sLine, PROCESSING);
	}

	trace_event("- Data Authentication Verification ---------------", PROCESSING);

	/* Getting the AC key **/
	if (memcmp(sDacFlag, "DAC", 3) == 0)
		nRetCode = Get_IM_Key(nIndexCtx, sPan, sAC_Key, "DAC");
	if (memcmp(sDacFlag, "IDN", 3) == 0)
		nRetCode = Get_IM_Key(nIndexCtx, sPan, sAC_Key, "IDN");

	if (nRetCode != OK)
	{
		memcpy(sHsmResultCode, "-DMK", 4);
		trace_event("INVALID KEY DMK ", ERROR);
		trace_event("END   check_data_authen(KEY_UNAVAILABLE)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	if (debug_option == 1)
	{
		sprintf(sLine, "AC KEY IS                  : %s", sAC_Key);
		trace_event(sLine, PROCESSING);
	}

	/* PreFormated Pan */

	memset(szWork, '0', 16);
	memcpy(sTmpField, sPan, strlen(sPan));
	memcpy(sTmpField + strlen(sPan), sSeqNumber + 1, 2);
	nLenWork = strlen(sTmpField);
	/* if (strlen(nLenWork) < 16 ) EBE140523: It was meant nLenWork*/
	if (nLenWork < 16)
		memcpy(szWork + (16 - nLenWork), sTmpField, nLenWork);
	else
		memcpy(szWork, sTmpField + (nLenWork - 16), 16);

	AsciiToHex(szWork, sFormatedPan, 16);
	if (debug_option == 1)
	{
		sprintf(sLine, "PRE FORMATED PAN            : %s", szWork);
		trace_event(sLine, PROCESSING);
	}

	/** Getting the ICC Data from TLV **/
	if (GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sIccData, &nIccLen) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-PAN", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
		trace_event("END   check_data_authen(NOK)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	nLenWork = AnalyseTlvPrivate(sIccData, nIccLen, &tlvInfo);
	/* sModeFlag */
	if (memcmp(sDacFlag, "DAC", 3) == 0)
		sModeFlag = '0'; /** DAC Verification **/
	else
		sModeFlag = '1'; /** DN Verification **/

	/* cSchemeId */
	cSchemeId = '1'; /** MasterCard **/

	/* ATC */
	memset(szWork, 0, sizeof(szWork));
	sAtc[0] = 0x30;
	sAtc[1] = 0x30;
	if (GetTlvPrivate(CHIP_ATC_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sAtc, 4);

	sprintf(sLine, "ATC                         : %s", szWork);
	trace_event(sLine, PROCESSING);

	/* UN */

	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate(CHIP_UNPRED_NUMBER_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
		AsciiToHex(szWork, sUn, 8);

	sprintf(sLine, "UN                          : %s", szWork);
	trace_event(sLine, PROCESSING);

	/* DAC */

	memset(szWork, 0, sizeof(szWork));

	/***Recuperation du contenu du Data Authentication Code from TLV **/
	sprintf(sLine, "DAC                         : %s", szWork);
	trace_event(sLine, PROCESSING);
	/* DN */
	memset(szWork, 0, sizeof(szWork));

	/***Recuperation du contenu du Dynamic Number from TLV **/
	sprintf(sLine, "IDN                        : %s", szWork);
	trace_event(sLine, PROCESSING);
	/* Send Commande */
	if (memcmp(sDacFlag, "DAC", 3) == 0)	/** DAC Verification **/
		nLength = command_KS(BuffSnd, sModeFlag, cSchemeId, sAC_Key, sFormatedPan, sAtc, sUn, sDac);
	else
		/** IDN Verification **/
		nLength = command_KS(BuffSnd, sModeFlag, cSchemeId, sAC_Key, sFormatedPan, sAtc, sUn, sDn);


	nRetCode = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if (nRetCode != OK)
	{

		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_data_authen(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	} 


	nRetCode = HsmQueryBin(nIndexCtx, nPid, BuffSnd, nLength, BuffRcv);

	if (nRetCode == NOT_AVAILABLE)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_data_authen(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nRetCode == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_data_authen(MALFUNCTION)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	memset(szWork, 0, sizeof(szWork));
	nRetCode = command_KT(BuffRcv, sHsmResultCode, szWork);

	if (nRetCode != OK)
	{
		switch (nRetCode)
		{
		case VERIFICATION_FAILED:
			trace_event("End   check_data_authen(VERIFICATION_FAILED)", PROCESSING);
			return(VERIFICATION_FAILED);
		case ERROR_COMMAND:
			trace_event("End check_data_authen(ERROR_COMMAND)", PROCESSING);
			return(HSM_MALFUNCTION);
		default:
			trace_event("End   check_data_authen(CRYPTOGRAPHIC_ERROR)", PROCESSING);
			return(CRYPTOGRAPHIC_ERROR);
		}
	}

	/* PCI */
	InitTlvPrivate(&tlvInfo);
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sFormatedPan, 0, sizeof(sFormatedPan));
	memset(sAtc, 0, sizeof(sAtc));
	memset(sUn, 0, sizeof(sUn));
	memset(sAC_Key, 0, sizeof(sAC_Key));
	memset(szWork, 0, sizeof(szWork));
	memset(sTmpField, 0, sizeof(sTmpField));
	/*memset(sTmpData, 0, sizeof(sTmpData));*/
	memset(sDac, 0, sizeof(sDac));
	memset(sDn, 0, sizeof(sDn));
	memset(sIccData, 0, sizeof(sIccData));
	memset(sPan, 0, sizeof(sPan));
	memset(sSeqNumber, 0, sizeof(sSeqNumber));
	memset(sLine,      0, sizeof(sLine));

	trace_event("End   check_data_authen(OK)", PROCESSING);
	return(OK);
}
#endif


int     gen_iss_script_data(
	int        nIndexCtx,
	char      *sTlvBuffer,
	char      *sHsmResultCode
	)

{

	unsigned char  BuffSnd[512];
	unsigned char  BuffRcv[512];
	unsigned char  sPan[22 + 1];
	unsigned char  sSeqNumber[8 + 1];
	unsigned char  sFormatedPan[8 + 1];
	unsigned char  sAtc[2 + 1];
	unsigned char  sUn[4 + 1];
	unsigned char  sIntegSessData[64];
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
	unsigned char  sArqc[8 + 1];
	unsigned char  sSMI_Key[256];
	unsigned char  sSMC_Key[256];
	unsigned char  sAC_Key[256];
	unsigned char  sPVK_Key[256];  
	unsigned char  sPVKA[256]; /*ZKO28032019*/
	unsigned char  cSchemeId;
	unsigned char  cModeFlag;
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
	char           sDestPBFrmt[8];
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
	pid_t    nPid;
	char     sAppCode[4 + 1];
	char	sOriginCode[4];
	char	encryption_mode;
	char	sSecurityFormat[2 + 1];
	char	sNetworkCode[2 + 1];
	char	sCVRBin[32];
	char	sChipIAD [64+1];
	char	sCardInfo[30+1];
	int		nDki = -1;
	int		nUseNewCmd;
	int 	nMacLength; /*ABA20241017 PLUTONL-9427 */

	trace_event("Start gen_iss_script_data()", PROCESSING);

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));

	memset(sFormatedPan, 0, sizeof(sFormatedPan));
	memset(sPlainTextData, 0, sizeof(sPlainTextData));
	memset(sSecureMsgData, 0, sizeof(sSecureMsgData));
	memset(sIssScriptData, 0, sizeof(sIssScriptData));
	memset(sIntegSessData, 0, sizeof(sIntegSessData));
	memset(sIssCmdData, 0, sizeof(sIssCmdData));
	memset(sMAC, 0, sizeof(sMAC));
	memset(sArqc, 0, sizeof(sArqc));
	memset(sSMI_Key, 0, sizeof(sSMI_Key));
	memset(sSMC_Key, 0, sizeof(sSMC_Key));
	memset(sAC_Key, 0, sizeof(sAC_Key));
	memset(sPVK_Key, 0, sizeof(sPVK_Key));
	memset(szWork, 0, sizeof(szWork));
	memset(sTmpField, 0, sizeof(sTmpField));
	memset(sPinPek, 0, sizeof(sPinPek));
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
	memset(sDestPBFrmt, 0, sizeof(sDestPBFrmt));
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
	/*ZKOStart28032019*/
	memset(sPVKA , 0, sizeof(sPVKA ));
        memset(sCVRBin , 0, sizeof(sCVRBin ));

	/*ZKOEnd*/
	memset(sCardSeqNbr, '\0', sizeof(sCardSeqNbr));		
	memset(sChipIAD, '\0', sizeof(sChipIAD));	
	memset(sCardInfo, '\0', sizeof(sCardInfo));	
	
	InitTlvPrivate(&tlvInfo);
	nUseNewCmd = NOK;

	

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
	AsciiToHex(sCvr,sCVRBin,nLenWork);

	GetTlvBuffer(AUTO_EMV_CARD_INFO_PARAM, sTlvBuffer, szWork, &nLenWork);

	
	if( memcmp(szWork,"VSDC",4) == 0 ) 
	{
		cSchemeId = '0';
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
		cSchemeId = '0';
	}
	else
	{
		cSchemeId = '1';
	}



	memcpy(sAppCode,szWork,4);

	/*ABA20241017 PLUTONL-9427 */
	if (memcmp(sAppCode, "UPI", 3) == 0) 
	{
		nMacLength = 8;
	}
	else
	{
		nMacLength = 16;
	} 



	nRetCode = Update_SentIssScripts(nIndexCtx, sPan, sCvr,sAppCode);



	/* PreFormated Pan */
	memset(sTmpField, 0, sizeof(sTmpField));
	memcpy(sTmpField, sPan, strlen(sPan));
	memcpy(sTmpField + strlen(sPan), sSeqNumber + 1, 2);
	nLenWork = strlen(sTmpField);

	memset(szWork, '0', 16);
	if (nLenWork < 16)
		memcpy(szWork + (16 - nLenWork), sTmpField, nLenWork);
	else
		memcpy(szWork, sTmpField + (nLenWork - 16), 16);

	AsciiToHex(szWork, sFormatedPan, 16);


	
	if (GetTlvPrivate(CHIP_ATC_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
	{
		AsciiToHex(szWork, sAtc, 4);
	}
	else
	{
		memset(sAtc,0,2);
	}

	
	if (GetTlvPrivate(CHIP_APP_CRYPTOGRAM_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
	{
		AsciiToHex(szWork, sArqc, 16);
	}


	if (GetTlvPrivate(CHIP_UNPRED_NUMBER_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
	{
		AsciiToHex(szWork, sUn, 8);
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
		memset(sIntegSessData, 0, sizeof(sIntegSessData));
		memset(BuffSnd, 0, sizeof(BuffSnd));
		memset(BuffRcv, 0, sizeof(BuffRcv));
		memset(sPlainTextData, 0, sizeof(sPlainTextData));
		memset(sDestPBFrmt, 0, sizeof(sDestPBFrmt));
		memset(sPan12     , 0, sizeof(sPan12     ));
		memset(sPinBlkBCD , 0, sizeof(sPinBlkBCD ));
		memset(sScriptCode , 0, sizeof(sScriptCode ));

		nPlainTextLen = 0;

		cModeFlag = '0';
		
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
		
		if (GetTlvPrivate(CHIP_ISS_APPLI_DATA_TAG, &tlvInfo, sChipIAD, &nLenData) == SUCCESS &&	/*PLUTONL-3519*/ 	
			GetTlvBuffer(AUTO_EMV_CARD_INFO_PARAM, sTlvBuffer, sCardInfo, &nLenData) == SUCCESS)
		{		
			nRetCode = Get_DKI_FROM_IAD(nIndexCtx, sChipIAD, sCardInfo, &nDki);	/*PLUTONL-3519*/ 
		}
		
		if( nSequence == 1 )
		{
			nRetCode = Get_EMV_Key(nIndexCtx, sPan, "SMI", sSMI_Key, nDki);	/*PLUTONL-3519*/ 
			if (nRetCode != OK)
			{
				memcpy(sHsmResultCode, "-SMI", 4);
				trace_event("INVALID KEY DMK ", ERROR);
				trace_event("END   gen_iss_script_data(KEY_UNAVAILABLE)", PROCESSING);
				return(HSM_MALFUNCTION);
			}
		}


		if( memcmp(sScriptCode, "PC", 2) == 0 )
		{
			cModeFlag = '4';
			memcpy(sSrcPBFrmt,"01",2);
		


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
				trace_event("END   gen_iss_script_data(NOK) <ERROR ON GetIssuerZPK>", PROCESSING);
				return(HSM_MALFUNCTION);
			}
			
			/*ZKO28032019 start*/

			/*nRetCode = GetTlvBuffer(AUTO_PVK_KEY,sTlvBuffer,sPVK_Key,&nLength);
			if( nRetCode != OK)
			{
				memcpy(sHsmResultCode, "PVK", 4);
				trace_event("END   gen_iss_script_data(NOK) <ERROR ON Get PVK>", PROCESSING);
				return(HSM_MALFUNCTION);
			}*/
			
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
			/*ZKO28032019 end*/

			nRetCode = PinOffsetToZpk(nIndexCtx, sPan, sCardPinOffset, sSrcPBFrmt, sPek, sPVK_Key, sPinPek );
			if( nRetCode != OK)
			{
				memcpy(sHsmResultCode, "-PIN", 4);
				trace_event("END   gen_iss_script_data(NOK) <ERROR ON PinOffsetToZpk>", PROCESSING);
				return(HSM_MALFUNCTION);
			}
			
			nRetCode = Get_EMV_Key(nIndexCtx, sPan, "SMC", sSMC_Key, nDki);	/*PLUTONL-3519*/ 
			if (nRetCode != OK)
			{
				memcpy(sHsmResultCode, "-SMC", 4);
				trace_event("END   gen_iss_script_data(KEY_UNAVAILABLE) <ERROR ON Get_EMV_Key(ENC)>", PROCESSING);
				return(HSM_MALFUNCTION);
			}


			nRetCode = Get_EMV_Key(nIndexCtx, sPan, "AC", sAC_Key, nDki);	/*PLUTONL-3519*/ 
			if (nRetCode != OK)
			{
				memcpy(sHsmResultCode, "-AC_", 4);
				trace_event("END   gen_iss_script_data(KEY_UNAVAILABLE) <ERROR ON Get_EMV_Key(AC)>", PROCESSING);
				return(HSM_MALFUNCTION);
			}


			/** Destination PIN Block Format **/
			sprintf(sDestPBFrmt, "41"); /** VISA Format Without Using Current Pin **/
			if (memcmp(sNetworkCode,NET_MASTERCARD,2) == 0 ) 
			{
				sprintf(sDestPBFrmt, "34"); /** Standard EMV PIN Block **/
			}
			else if (memcmp(sNetworkCode,NET_VISA,2) == 0 && nUseNewCmd == OK ) 
			{
				sprintf(sDestPBFrmt, "34"); 
			}
			
			
			/** Last 12 Chars of the Pan excluding the Check Digit **/
			sprintf(szWork, "%022d", 0);
			memcpy(szWork + 22 - strlen(sPan), sPan, strlen(sPan));
			/*sprintf(sPan12, szWork + 9);*/
			memcpy(sPan12, szWork + 9, 12); /*IBO20220816 PLUTONL-5083*/

			/** Compress The Pin Block **/
			AsciiToHex(sPinPek, sPinBlkBCD, 16);

		}
		

		if ('1' == cSchemeId && 1 < nSequence)
			FctIncrimentRandBy1(sArqc, sArqc);

		/* Issuer Command Data */
		AsciiToHex(sIssCmdData, szWork, strlen(sIssCmdData));

		/*Data to be maced: 
		* CLA, INS, P1, P2, Lc(including the MAC length)
		* ATC
		* ARQC
		* Plaintext/Enciphered data
		*/
		nPlainTextLen = 0;
		memcpy(sPlainTextData + nPlainTextLen, szWork, 5); nPlainTextLen += 5;
		memcpy(sPlainTextData + nPlainTextLen, sAtc, 2); nPlainTextLen += 2;
		memcpy(sPlainTextData + nPlainTextLen, sArqc, 8); nPlainTextLen += 8;

		/*Command Data if any*/
		if (strlen(sIssCmdData) > 10)
		{
			memcpy(sPlainTextData + nPlainTextLen, szWork + 5, (strlen(sIssCmdData) - 10) / 2); 
			nPlainTextLen += (strlen(sIssCmdData) - 10) / 2;
		}

		/* Integrity Session Data **/
		if ('1' == cSchemeId) /** MCI **/
		{
			memcpy(sIntegSessData + 0, sArqc, 8);
		}
		else
		{
			memcpy(sIntegSessData + 6, sAtc, 2);
		}

		
		if( nUseNewCmd == NOK )
		{
		nLength = command_KU(BuffSnd,
			cModeFlag,
			cSchemeId,
			sSMI_Key,
			sFormatedPan,
			sIntegSessData,
			nPlainTextLen,
			sPlainTextData,
			sSMC_Key,
			sPek,
			sIntegSessData,
			nPlainTextLen,
			8,
			sPinBlkBCD,
			sSrcPBFrmt,
			sDestPBFrmt,
			sPan12,
			sAC_Key,
			SCHEME_MODE
			);
		}
		else
		{
			nLength = command_KY(BuffSnd,
				cModeFlag,
				cSchemeId,
				sSMI_Key,
				sFormatedPan,
				sIntegSessData,
				nPlainTextLen,
				sPlainTextData,
				sSMC_Key,
				sPek,
				sIntegSessData,
				nPlainTextLen,
				8,
				sPinBlkBCD,
				sSrcPBFrmt,
				sDestPBFrmt,
				sPan12,
				sAC_Key,
				SCHEME_MODE
				);
		}
		

		

		nRetCode = HsmQueryBin(nIndexCtx, nPid, BuffSnd, nLength, BuffRcv);
		if (nRetCode == NOT_AVAILABLE)
		{
			memcpy(sHsmResultCode, "UNVL", 4);
			trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
			trace_event("End   gen_iss_script_data(HSM_UNAVAILABLE)", PROCESSING);
			return(HSM_UNAVAILABLE);
		}

		if (nRetCode == HSM_MALFUNCTION)
		{
			memcpy(sHsmResultCode, "UNVL", 4);
			trace_event("HSM MALFUNCTION DETECTED", ERROR);
			trace_event("End   gen_iss_script_data(MALFUNCTION)", PROCESSING);
			return(HSM_MALFUNCTION);
		}


		if( nUseNewCmd == NOK )
		{
		nRetCode = command_KV(BuffRcv, cModeFlag, sHsmResultCode, sMAC, &nSecureMsgLen, sSecureMsgData);
		}
		else
		{
			nRetCode = command_KZ(BuffRcv, cModeFlag, sHsmResultCode, sMAC, &nSecureMsgLen, sSecureMsgData);
		}
		if (nRetCode != OK)
		{
			switch (nRetCode)
			{
			case ERROR_COMMAND:
				trace_event("End   gen_iss_script_data(ERROR_COMMAND)", PROCESSING);
				return(HSM_MALFUNCTION);
			default:
				trace_event("End   gen_iss_script_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return(CRYPTOGRAPHIC_ERROR);
			}
		}

		/* MAC */
		
		
		memset(sTmpField, 0, sizeof(sTmpField));
		memset(sIssScriptData, 0, sizeof(sIssScriptData));

		HexToAscii(sMAC, sMACStr, /*16*/nMacLength); /*ABA20241017 PLUTONL-9427 */

		if ('0' != cModeFlag)
		{
			HexToAscii(sSecureMsgData, sTmpField, nSecureMsgLen * 2);
		}
		else
		{
			nSecureMsgLen = 0;
		}

		if (debug_option == 1)
		{
			sprintf(sLine, "==> SECURE MESSAGING MAC       =[%s]", sMACStr);
			trace_event(sLine, PROCESSING);
		}

		

		nLength = 0;
		sprintf(sIssScriptData, "86%02lX", ((strlen(sIssCmdData) + strlen(sMACStr)) / 2) + nSecureMsgLen); nLength+=4;
		memcpy(sIssScriptData + nLength, sIssCmdData, strlen(sIssCmdData)); nLength+=strlen(sIssCmdData);
		
		if( nSecureMsgLen > 0 )
		{
			memcpy(sIssScriptData + nLength, sTmpField, nSecureMsgLen * 2); nLength+=nSecureMsgLen * 2;
		}

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
		memset(sIntegSessData, 0, sizeof(sIntegSessData));
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
		memset(sDestPBFrmt, 0, sizeof(sDestPBFrmt));
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
	memset(sIntegSessData, 0, sizeof(sIntegSessData));
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
	memset(sDestPBFrmt, 0, sizeof(sDestPBFrmt));
	memset(sPan12     , 0, sizeof(sPan12     ));
	memset(sPinBlkBCD , 0, sizeof(sPinBlkBCD ));
	InitTlvPrivate(&tlvInfo);
	trace_event("End   gen_iss_script_data(OK)", PROCESSING);
	return(OK);
}


/******************************************************************************/
/* Fonction check_cvc3_data.                                                   */
/* -----------------------                                                     */
/* Description : Fonction qui verifie le cvc3                                  */
/******************************************************************************/
/* START MJA21092011  CONTACTLESS */

int check_cvc3_data(
	int nIndiceCtx,
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
	unsigned char   BuffSnd[BUF_LG_MAX];
	unsigned char   BuffRcv[256];
	int             retour;
	pid_t           nPid;
	char            sLine[MAX_LINE_TRC];
	int				nLength;

	trace_event("Start check_cvc3_data()", PROCESSING);

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));


	if (debug_option == 1)
	{
		sprintf(sLine, "PAN                   : %s", sPan);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "PAN  SEQUENCE NUMBER  : %s", sPanSeqNo);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "EXPIRY DATE           : %s", sExpiryDate);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "SERVICE CODE          : %s", sServiceCode);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "MKCVC3 KEY            : %s", sMkcvc3);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "ATC					  : %s", sAtc);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "DCVV				  : %s", sDcvv);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "IVCVC3				  : %s", sIvcvc3);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "DATA LENGHT			  : %d", nDataLen);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "DATA				  : %s", sData);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "UN					  : %s", sUn);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "CVC3				  : %s", sCvc3);
		trace_event(sLine, PROCESSING);
	}
	else
	{
		sprintf(sLine, "PAN  SEQUENCE NUMBER        : %s", sPanSeqNo);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "DATA LENGHT				    : %d", nDataLen);
		trace_event(sLine, PROCESSING);
	}


	nLength = command_PM(BuffSnd, '1', '2', sMkcvc3, sPan, sPanSeqNo, sExpiryDate, sServiceCode, sAtc, sDcvv, sIvcvc3,
		nDataLen, sData, sUn, sCvc3);

	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ((retour != OK)) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvc3_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	} 

	retour = HsmQueryBin(nIndiceCtx, nPid, BuffSnd, nLength, BuffRcv);
	if (retour == NOT_AVAILABLE) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvc3_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (retour == HSM_MALFUNCTION) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_cvc3_data(MALFUNCTION)", PROCESSING);
		return (HSM_MALFUNCTION);
	}
	retour = command_PN(BuffRcv, sHsmResultCode);
	if (retour != OK) {
		switch (retour) {
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
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));
	trace_event("End   check_cvc3_data(OK)", PROCESSING);
	return (OK);
}
/* END MJA21092011  CONTACTLESS */

/*MCK25062012*/
int check_dcvv_data(
	int nIndiceCtx,
	char *sMcvkc,
	char *sPan,
	char *sExpiryDate,
	char *sServiceCode,
	char *sAtc,
	char *sDcvv,
	char *sHsmResultCode
	)
{
	unsigned char   BuffSnd[BUF_LG_MAX];
	unsigned char   BuffRcv[256];
	int             retour;
	pid_t           nPid;
	char            sLine[MAX_LINE_TRC];

	trace_event("Start check_dcvv_data()", PROCESSING);

	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));

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

	retour = command_PM(BuffSnd, '0', '0', sMcvkc, sPan, "", sExpiryDate, sServiceCode, sAtc, sDcvv, "",
		0, "", "", "");


	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ((retour != OK)) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_dcvv_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	} 

	retour = HsmQuery(nIndiceCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_dcvv_data(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}
	if (retour == HSM_MALFUNCTION) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_dcvv_data(MALFUNCTION)", PROCESSING);
		return (HSM_MALFUNCTION);
	}
	retour = command_PN(BuffRcv, sHsmResultCode);
	if (retour != OK) {
		switch (retour) {
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
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sLine,   0, sizeof(sLine));
	trace_event("End   check_dcvv_data(OK)", PROCESSING);
	return (OK);
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
	unsigned char	sMsgData[40 + 1];
	unsigned char	sDataComp[40 + 1];
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
	memset(sDataComp        , 0, sizeof(sDataComp));
	memset(key_id, 0, sizeof(key_id)); /*ASO20200403*/
	

	CompressAmount(TxAmount, CompressedAmount); 
	/*Sha256_ALgo(MerchantName, MerchantNameHash);*/

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

	AsciiToHex(sMsgData, sDataComp, strlen(sMsgData));


	memcpy(sIAVMerchant, aav_data + 4, 8);
	
	memcpy(key_id, aav_data + 21, 1); 	/*ASO20200403*/

	/*HMAC KEY */
	retour = Get_UCAF_Key(nIndiceCtx, Pan,/*nCvki,*/ key_id, sHMAC_Key);	/*ASO20200403*/
	if ((retour != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		return(HSM_UNAVAILABLE);
	}


	nLength = command_LQ(BuffSnd,
						"FF", /*sHash_Identifier*/
						"0032", /*sHMAC_Len*/
						"04", /*sHMAC_Key_Fromat*/
						sHMAC_Key,
						strlen(sMsgData)/2, /*nMsgDataLen*/
						sDataComp
	);



	retour = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ((retour != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   MC_Spa2_AAV_implementation(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	} 

	retour = HsmQueryBin(nIndiceCtx, nPid, BuffSnd, nLength, buffer_received);
	if (retour == NOT_AVAILABLE)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   MC_Spa2_AAV_implementation(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   MC_Spa2_AAV_implementation(MALFUNCTION)", PROCESSING);
		return(HSM_MALFUNCTION);
	}



	retour = command_LR(buffer_received, sHMAC, err_code);
	if (retour != OK)
	{
		switch (retour)
		{
		case VERIFICATION_FAILED:
			trace_event("End   MC_Spa2_AAV_implementation(VERIFICATION_FAILED)", PROCESSING);
			return(VERIFICATION_FAILED);

		case ERROR_COMMAND:
			trace_event("End MC_Spa2_AAV_implementation(ERROR_COMMAND)", PROCESSING);
			return(HSM_MALFUNCTION);

		default:
			trace_event("End   MC_Spa2_AAV_implementation(CRYPTOGRAPHIC_ERROR)", PROCESSING);
			return(CRYPTOGRAPHIC_ERROR);
		}
	}
	else
	{

		HexToAscii(sHMAC, sDataComp, 8);

		if(memcmp(sDataComp, sIAVMerchant, 8) == 0)
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



/*Start NAB10082020  PLUTONL-2170*/

int Pan_v7_conversion (char* sPanIn,char *sAAV , char* sPanOut){
	
	
	char   sBuffer 			[ BUF_LG_MAX + 1];
	unsigned char szWork    [ 10 +1 ];
	unsigned char PanHash	[ 256 + 1 ];
	char   vJul_Day			[ 3 + 1 ];
	char   sCurr_jul_Day    [ 3 + 1 ];
	char   curDate			[ 10 + 1 ];
	char   curYear			[ 2 + 1 ];
	char   sYear			[ 2 + 1 ];
	char   sAmnt            [ 12 +1 ];
	char   sCurrency        [ 3 + 1 ];
	
	int   	cAmnt = 0;
	int     i,j = 0 ;
	int     var_Y ;
	int     offset = 0;

	
	
	
	memset ( sBuffer,  		0, sizeof ( sBuffer ));
	memset ( sAmnt,  	0, sizeof ( sAmnt ));
	memset ( sYear,  	0, sizeof ( sYear ));
	memset ( sCurrency,  	0, sizeof ( sCurrency ));
	memset ( vJul_Day,  	0, sizeof ( vJul_Day ));
	memset ( curDate,  	0, sizeof ( curDate ));
	memset ( curYear,  	0, sizeof ( curYear ));
	memset ( sCurr_jul_Day,  	0, sizeof ( sCurr_jul_Day ));
	memset ( PanHash,  	0, sizeof ( PanHash ));
	memset ( szWork,  	0, sizeof ( szWork ));
	

	offset = 0;
	memcpy(sBuffer,sPanIn, strlen(sPanIn));
	offset += strlen(sPanIn)-1;
		
	
	if (!memcmp(sAAV,"07",2) )  
	{
        sBuffer[offset++] = '6';
	}
	else if(!memcmp(sAAV,"03",2)) /*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
	{
		sBuffer[offset++] = '7';
	}
	else
	{
		sBuffer[offset++] = '5';
	}
		
/* TODO Check if other values are needed
	if (!memcmp(sAAV,"00",2) )  / * HAA 0102021 Management of Attempts * /
                sBuffer[offset++] = '5';
        if (!memcmp(sAAV,"08",2) || !memcmp(sAAV,"07",2))
                sBuffer[offset++] = '6';
        if (!memcmp(sAAV,"02",2) || !memcmp(sAAV,"03",2) || !memcmp(sAAV,"04",2) || !memcmp(sAAV,"05",2))
                sBuffer[offset++] = '7';

*/
	/* Authentication Amount : The purchase amount*/
	memcpy(sAmnt, sAAV + 14, 10); 
	/*Converted to Decimal*/
	AsciiToHex(sAmnt + 4, szWork, 6);	
    cAmnt = (szWork [0]*256*256)+(szWork [1]*256)+szWork [2];		
	sprintf ( sAmnt, "%012d" , cAmnt);
	strncpy(sBuffer + offset, sAmnt, 12);
	offset = offset +12;
	
	/*Authentication Currency Code*/
	memcpy(sCurrency, sAAV + 24, 3);
	strncpy(sBuffer + offset, sCurrency, 3);
	offset = offset +3;
	/*Authentication Date */
	/*should be concatended with the year /the year component of the CAVV creation date. Compare today's Julian day (authorization GMT-based) with the Authentication Date in F126.9 (right nibble of byte 14 and byte 15). If today’s Julian day is greater than or equal to the Authentication Date then use the current year. Otherwise, use the current year minus 1*/ 	
	/*Julian Day recu dans le CAVV  */
	memcpy(vJul_Day, sAAV +27, 3);
	
	/*Sysdate Julian Day*/
	current_julian_date ( sCurr_jul_Day) ;
	current_date_ddmmyy (curDate);
	memcpy(curYear, curDate + 4, 2);
	
	if (atoi(sCurr_jul_Day) >= atoi(vJul_Day))
	{		

		strncpy( sBuffer + offset, curYear, 2);
		offset = offset +2;
	}
	else
	{
		var_Y = atoi(curYear) -1;
		sprintf ( sYear, "%02d", var_Y);	
		strncpy( sBuffer + offset, sYear, 2);
		offset = offset +2;

	}

	strncpy( sBuffer + offset, vJul_Day, 3);
	Sha256_ALgo ( sBuffer, PanHash );

	/*iteration sur le Pan HACH PR AVOIR LES &- DIGIT*/

	for(i=0, j=0; i <=strlen(PanHash) && j < 16;i++)
	{
		
		if (!isdigit(PanHash[i]))
		{
			continue;
			
		}
		else
		{
			sPanOut	[j++] = PanHash[i];
			
		}
	}

	
	if ( strlen ( sPanOut ) < 16 )
	{
		for(i=0;i<= strlen(PanHash) && j < 16; i++)
		{
			if (!isdigit(PanHash[i]))
			{
				switch (PanHash[i])
				{
				 case 'A' : PanHash[i] ='0'  ; break;
				 case 'B' : PanHash[i] ='1'  ; break;
				 case 'C' : PanHash[i] ='2'  ; break;
				 case 'D' : PanHash[i] ='3'  ; break;
				 case 'E' : PanHash[i] ='4'  ; break;
				 case 'F' : PanHash[i] ='5'  ; break;
					
				}
				sPanOut[j++]=	PanHash[i];				
			}	
		}
	}
	
	sPanOut[16] = '\0';
	return OK;
}
/*END   NAB10082020  PLUTONL-2170*/


/******************************************************************************/
/* Fonction check_cup_av_data.                                                  */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le av recu For CUP.                           */
/******************************************************************************/
int     check_cup_av_data(
	int nIndiceCtx,
	char *Pan,
	char *sExpiryDate,
	char *av_data_b64,
	char *sHsmResultCode
	)
{
	unsigned char  	BuffSnd[BUF_LG_MAX];
	unsigned char  	BuffRcv[256];
	char 						sLine[MAX_LINE_TRC];
	char 						cavv_value[4];
	char 						avn[5];
	char 						sExpDate[5];
	char 						autho_result[2+1];
	char						cavv_key[SIZE_KEY_TDES + 1];
	char						service_code[4];
	char						CvkA[SIZE_KEY_TDES + 1];
	char						CvkB[SIZE_KEY_TDES + 1];
	char						key_indicator[3];
	char 		                av_data    [256];
    int   	int_temp=0 ;
	int      				retour;
	pid_t    				nPid;

	memset(sLine, 0, sizeof(sLine));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(cavv_value, 0, sizeof(cavv_value));
	memset(sExpDate, 0, sizeof(sExpDate));
	memset(avn, 0, sizeof(avn));
	memset(autho_result, 0, sizeof(autho_result));
	memset(av_data, 0, sizeof(av_data));
	memset(cavv_key, 0, sizeof(cavv_key));
	memset(service_code, 0, sizeof(service_code));
	memset(CvkA, 0, sizeof(CvkA));
	memset(CvkB, 0, sizeof(CvkB));
	memset(key_indicator, 0, sizeof(key_indicator));

	trace_event("Start check_cup_av_data()", PROCESSING);


    retour = Base64_Decode(nIndiceCtx, av_data_b64, av_data);
   
   /*Version2*/
	memcpy(autho_result , av_data , 2); /*Authentication Transaction Result*/
	memcpy(key_indicator, av_data + 4, 1);/*AV generator indicator*/
    
	/*Converted to Decimal*/
	memcpy(avn, av_data + 5, 3);
    int_temp=strtol(avn, NULL, 16);
	sprintf ( cavv_value, "%03d" , int_temp);
    

	retour = Get_CAVV_Key(nIndiceCtx, Pan, key_indicator, CvkA, CvkB); 

	if ((retour != OK))
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
		sprintf(sLine, "AVN                         : %.4s", avn);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "EXPIRY DATE                 : %.4s", sExpiryDate);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "CAVV_KEY                    : %.48s||%.48s", CvkA, CvkB);
		trace_event(sLine, PROCESSING);
		sprintf(sLine, "- CAVV Verification --------------: %.3s", cavv_value);
		trace_event(sLine, PROCESSING);
	}
	else
		trace_event("- CAVV Verification ---------------", PROCESSING);

    /*service code*/
	memcpy(service_code, autho_result+1, 1);
	memcpy(service_code+1, "0", 1);
	memcpy(service_code +2, key_indicator, 1);

	retour = command_CY(BuffSnd, Pan, service_code, sExpiryDate, CvkA, CvkB, cavv_value);

	retour = HsmQuery(nIndiceCtx, nPid, BuffSnd, BuffRcv);
	if (retour == NOT_AVAILABLE)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cup_av_data(HSM_UNAVAILABLE)", PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM MALFUNCTION DETECTED", ERROR);
		trace_event("End   check_cup_av_data(MALFUNCTION)", PROCESSING);
		return(HSM_MALFUNCTION);
	}

	retour = command_CZ(BuffRcv, sHsmResultCode);
	if (retour != OK)
	{
		switch (retour)
		{
		case VERIFICATION_FAILED:
			trace_event("End   check_cup_av_data(VERIFICATION_FAILED)", PROCESSING);
			return(VERIFICATION_FAILED);

		case ERROR_COMMAND:
			trace_event("End check_cup_av_data(ERROR_COMMAND)", PROCESSING);
			return(HSM_MALFUNCTION);

		default:
			trace_event("End   check_cup_av_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
			return(CRYPTOGRAPHIC_ERROR);
		}
	}

	memset(sLine, 0, sizeof(sLine));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(cavv_value, 0, sizeof(cavv_value));
	memset(avn, 0, sizeof(avn));
	memset(autho_result, 0, sizeof(autho_result));
	memset(cavv_key, 0, sizeof(cavv_key));
	memset(service_code, 0, sizeof(service_code));
	memset(CvkA, 0, sizeof(CvkA));
	memset(CvkB, 0, sizeof(CvkB));
	memset(key_indicator, 0, sizeof(key_indicator));

	trace_event("End   check_cup_av_data(OK)", PROCESSING);
	return(OK);
}
/********************/



/*END PROD00073147*/
