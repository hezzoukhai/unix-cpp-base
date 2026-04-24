#include <stdio.h>
#include <hsm_inc.h>
#include <stdlib.h>
#include <string.h>

#include <iso8583.h>
#include <hsm_atalla_define.h>
#include <hsm_atalla.h>
#include <define.h>
#include <security_flags.h>





/******************************************************************************/
/* Tableau de descritions des reponses HSM                                    */
/* -------------------                                                        */
/******************************************************************************/
typedef struct s_response {
	char           *reponse;
	char           *description;
}               t_response;
/******************************************************************************/

t_response      hsm_response[] = {
	{"00", "Response to test message....."},
	{"01", "Length out of range.........."},
	{"02", "Invalid character............"},
	{"03", "Value out of range..........."},
	{"04", "Invalid number of parameters."},
	{"05", "Parity error................."},
	{"06", "Key usage error.............."},
	{"07", "Key usage error.............."},
	{"08", "Execution error.............."},
	{"09", "Execution error.............."},
	{"10", "Key Length error............."},
	{"11", "Printing error..............."},
	{"12", "Marker string not found......"},
	{"20", "Serial number set, cannot modify it."},
	{"21", "The HSM not in a Security Association, or serial number is not present."},
	{"22", "Non-existent command or option."},
	{"23", "Invalid command or option...."},
	{"24", "Incorrect challenge.........."},
	{"25", "Incorrect Acknowledgment....."},
	{"26", "Duplicate command or option.."},
	{"27", "No challenge to verify, a command 109 has been received without a prior command 108"},
	{"28", "The configuration string in command 108 is too long."},
	{"29", "Unable to allocate memory for the configuration string."},
	{"41", "ASRM timed out waiting for the response from the HSM."},
	{"73", "Header mismatch.............."},
	{"92", "Autokey error................"},
	{"93", "Factory keys already generated"},
	{"94", "No factory keys generated...."}
};

/******************************************************************************/
/******************************************************************************/
int
	TextError(char *sData)
{
	int             i;
	char            ligne[MAX_LINE_TRC];
	char			err_code[3];
	char			sFieldErr[3];

	memset(err_code,'\0',sizeof(err_code));
	memset(sFieldErr,'\0',sizeof(sFieldErr));

	memcpy(err_code, sData, 2);
	memcpy(sFieldErr, sData + 2, 2);

	for (i = 0; i < (sizeof(hsm_response) / sizeof(hsm_response[0])); i++)
	{
		if (!memcmp(hsm_response[i].reponse, err_code, 2)) {
			sprintf(ligne, "=> %s <= , first field found to be in error [%s]", hsm_response[i].description,sFieldErr);
			trace_event(ligne, PROCESSING);
			return (OK);
		}
	}
	sprintf(ligne, "=>UNKNOWN ERROR CODE %s <=", err_code);
	trace_event(ligne, PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction format_card_from_pan.                                             */
/* -------------------                                                        */
/* -----   Numero de Carte sans le dernier digit Luhn Key                     */
/******************************************************************************/
int format_card_from_pan(char *card,
						char *pan)
{
	char            V_card_number[13];

	memset(V_card_number, 0, sizeof(V_card_number));
	memset(V_card_number, '0', 12);

	if (strlen(pan) <= 12)
	{
		memcpy(V_card_number + 12 - strlen(pan) + 1,pan,strlen(pan) - 1);
	}
	else
	{
		memcpy(V_card_number, pan + (strlen(pan) - 13), 12);
	}

	memcpy(card, V_card_number, 12);
	return (OK);
}
/******************************************************************************/
/* Fonction translate_pin_block_format.                                             */
/* -------------------                                                        */
/* -----   Numero de Carte sans le dernier digit Luhn Key                     */
/******************************************************************************/
int translate_pin_block_format(char *sAtallaPBFormat,
								char *sThalesPBFormat)
{

	char	format[2+1];

	memset(format, '\0', sizeof(format));

	memcpy(format,sThalesPBFormat,strlen(sThalesPBFormat));

	switch (atoi(format)) {
		case 1:
			memcpy(sAtallaPBFormat, "1", 1);
			break;
		case 2:
			memcpy(sAtallaPBFormat, "3", 1);
			break;
		case 5:
			memcpy(sAtallaPBFormat, "C", 1);
			break;
		case 34:
			memcpy(sAtallaPBFormat, "6", 1);
			break;
		case 47:
			memcpy(sAtallaPBFormat, "8", 1);
			break;
		default:
			memcpy(sAtallaPBFormat, "1", 1);
			break;
	}

	return OK;
}
/******************************************************************************/
/******************************************************************************/

int    command_10(	char		*buffer_transmit ,
	int*		nLengthSnd,
	char*       KeyUsage,
	char*       zmkLmk,
	char*       keyLen
	)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;

	trace_event("Start command_10()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_GENERATE_KEY_10);

	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, KeyUsage, strlen(KeyUsage));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_EXCHANGE_KEY, &kAtallaCmd, zmkLmk, strlen(zmkLmk));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_LENGTH, &kAtallaCmd, keyLen, strlen(keyLen));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_10(OK)", PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/

int    command_20(	char *buffer_received,
	int nLengthRcv,
	char *keyLmk,
	char *keyZmk,
	char *check_value
	)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;

	trace_event("Start command_20()",PROCESSING);

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_20(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_GENERATE_KEY_20);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_WORKING_KEY_MFK, &kAtallaCmd, keyLmk, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_WORKING_KEY_KEK, &kAtallaCmd, keyZmk, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_CHECK_DIGITS, &kAtallaCmd, check_value, &nLength);


	trace_event("End   command_20(OK)",PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/

int    command_13(	char		*buffer_transmit ,
	int*		nLengthSnd,
	char*       KeyUsage,
	char*       zmkLmk,
	char*       keyZmk
	)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;

	trace_event("Start command_13()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_IMPORT_KEY_13);

	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, KeyUsage, strlen(KeyUsage));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_EXCHANGE_KEY, &kAtallaCmd, zmkLmk, strlen(zmkLmk));
	AddAtallaCmdInfoField (ATALLA_FN_WORKING_KEY_KEK, &kAtallaCmd, keyZmk, strlen(keyZmk));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_13(OK)", PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/
int    command_23(	char *buffer_received,
	int nLengthRcv,
	char *keyLmk,
	char *check_value
	)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;

	trace_event("Start command_23()",PROCESSING);

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_23(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_IMPORT_KEY_23);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_WORKING_KEY_MFK, &kAtallaCmd, keyLmk, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_CHECK_DIGITS, &kAtallaCmd, check_value, &nLength);


	trace_event("End   command_23(OK)",PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/

int    command_1A(	char		*buffer_transmit ,
	int*		nLengthSnd,
	char*       KeyUsage,
	char*       zmkLmk,
	char*       keyLmk
	)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;

	trace_event("Start command_1A()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_EXPORT_KEY_1A);

	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, KeyUsage, strlen(KeyUsage));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_EXCHANGE_KEY, &kAtallaCmd, zmkLmk, strlen(zmkLmk));
	AddAtallaCmdInfoField (ATALLA_FN_WORKING_KEY_MFK, &kAtallaCmd, keyLmk, strlen(keyLmk));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_1A(OK)", PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/
int    command_2A(	char *buffer_received,
					int nLengthRcv,
					char *keyZmk,
					char *check_value,
					char *keyLmkAKB
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;

	trace_event("Start command_2A()",PROCESSING);

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_2A(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_EXPORT_KEY_2A);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_WORKING_KEY_KEK, &kAtallaCmd, keyZmk, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_CHECK_DIGITS, &kAtallaCmd, check_value, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_WORKING_KEY_MFK, &kAtallaCmd, keyLmkAKB, &nLength);


	trace_event("End   command_2A(OK)",PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_5E(	char	*buffer_transmit ,
					int 	*nLengthSnd,
					char	*cvv_algorithm,
					char    *KeyUsage,
					char 	*card_number,
					char 	*service_code,
					char 	*expiry_date,
					char 	*cvk_A,
					char 	*cvk_B,
					char 	*cvv
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;
	char				sCvvData[32+1];
	int					sOffset=0;

	trace_event("Start command_5E()", PROCESSING);

	memset(sCvvData, 	'\0', sizeof(sCvvData));

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_VERIFY_CVV_5E);

	memcpy(sCvvData, card_number, strlen(card_number));
	sOffset += strlen(card_number);
	memcpy(sCvvData + sOffset, expiry_date, 4);
	sOffset += 4;
	memcpy(sCvvData + sOffset, service_code, 3);

	AddAtallaCmdInfoField (ATALLA_FN_CVV_ALGORITHM, &kAtallaCmd, cvv_algorithm, strlen(cvv_algorithm));
	AddAtallaCmdInfoField (ATALLA_FN_CVV_KEY, &kAtallaCmd, cvk_A, strlen(cvk_A));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, KeyUsage, strlen(KeyUsage));
	AddAtallaCmdInfoField (ATALLA_FN_CVV_DATA, &kAtallaCmd, sCvvData, strlen(sCvvData));
	AddAtallaCmdInfoField (ATALLA_FN_CVV, &kAtallaCmd, cvv, strlen(cvv));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_5E(OK)", PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/
int    command_6E(	char *buffer_received,
					int nLengthRcv,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[1+1];

	trace_event("Start command_6E()",PROCESSING);

	memset(sVerFlag, '\0', sizeof(sVerFlag));

	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_6E(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_VERIFY_CVV_6E);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG, &kAtallaCmd, sVerFlag, &nLength);

	if (memcmp(sVerFlag, "Y", 1) == 0)
	{
		trace_event("End   command_6E(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_6E(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_6E(CRYPTOGRAPHIC_ERROR)",PROCESSING);
	return(CRYPTOGRAPHIC_ERROR);
}
/******************************************************************************/
/******************************************************************************/
int    command_357(	char	*buffer_transmit ,
					int 	*nLengthSnd,
					char 	*sMkCvv,
					char 	*sPan,
					char 	*sPanSeqNo,
					char 	*sExpiryDate,
					char 	*sServiceCode,
					char 	*sAtc,
					char 	*sDcvv)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_357()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_VERIFY_DCVV_357);



	AddAtallaCmdInfoField (ATALLA_FN_ISSUER_MASTER_KEY_CVV, &kAtallaCmd, sMkCvv, strlen(sMkCvv));
	AddAtallaCmdInfoField (ATALLA_FN_PAN, &kAtallaCmd, sPan, strlen(sPan));
	AddAtallaCmdInfoField (ATALLA_FN_PAN_SEQ_NUMBER, &kAtallaCmd, sPanSeqNo, strlen(sPanSeqNo));
	AddAtallaCmdInfoField (ATALLA_FN_EXPIRY_DATE, &kAtallaCmd, sExpiryDate, strlen(sExpiryDate));
	AddAtallaCmdInfoField (ATALLA_FN_SERVICE_CODE, &kAtallaCmd, sServiceCode, strlen(sServiceCode));
	AddAtallaCmdInfoField (ATALLA_FN_ATC, &kAtallaCmd, sAtc, strlen(sAtc));
	AddAtallaCmdInfoField (ATALLA_FN_DCVV, &kAtallaCmd, sDcvv, strlen(sDcvv));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_357(OK)", PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/
int    command_457(	char *buffer_received,
					int nLengthRcv,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[1+1];

	trace_event("Start command_457()",PROCESSING);

	memset(sVerFlag, '\0', sizeof(sVerFlag));

	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_457(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_VERIFY_DCVV_457);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG, &kAtallaCmd, sVerFlag, &nLength);

	if (memcmp(sVerFlag, "Y", 1) == 0)
	{
		trace_event("End   command_457(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_457(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_457(CRYPTOGRAPHIC_ERROR)",PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/******************************************************************************/
int    command_359(	char	*buffer_transmit ,
					int 	*nLengthSnd,
					char 	*sMkCvc3,
					char 	*sPan,
					char 	*sPanSeqNo,
					char 	*sTrackData,
					char 	*sUnpredictableNo,
					char 	*sAtc,
					char 	*sDcvc3)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_359()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_VERIFY_CVC3_359);



	AddAtallaCmdInfoField (ATALLA_FN_ISSUER_MASTER_KEY_CVC3, &kAtallaCmd, sMkCvc3, strlen(sMkCvc3));
	AddAtallaCmdInfoField (ATALLA_FN_PAN, &kAtallaCmd, sPan, strlen(sPan));
	AddAtallaCmdInfoField (ATALLA_FN_PAN_SEQ_NUMBER, &kAtallaCmd, sPanSeqNo, strlen(sPanSeqNo));
	AddAtallaCmdInfoField (ATALLA_FN_TRACK_1_2_DATA, &kAtallaCmd, sTrackData, strlen(sTrackData));
	AddAtallaCmdInfoField (ATALLA_FN_UNPREDICTABLE_NUMBER, &kAtallaCmd, sUnpredictableNo, strlen(sUnpredictableNo));
	AddAtallaCmdInfoField (ATALLA_FN_ATC, &kAtallaCmd, sAtc, strlen(sAtc));
	AddAtallaCmdInfoField (ATALLA_FN_DCVC3, &kAtallaCmd, sDcvc3, strlen(sDcvc3));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_359(OK)", PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/
int    command_459(	char *buffer_received,
					int nLengthRcv,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[1+1];

	trace_event("Start command_459()",PROCESSING);

	memset(sVerFlag, '\0', sizeof(sVerFlag));

	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_459(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_VERIFY_CVC3_459);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG, &kAtallaCmd, sVerFlag, &nLength);

	if (memcmp(sVerFlag, "Y", 1) == 0)
	{
		trace_event("End   command_459(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_459(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_459(CRYPTOGRAPHIC_ERROR)",PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}
/******************************************************************************/
/******************************************************************************/
int    command_35A(	char	*buffer_transmit ,
					int 	*nLengthSnd,
					char 	*sData,
					char 	*sExpiryDate,
					char 	*sCscKey,
					char 	*sCsc5,
					char 	*sCsc4,
					char 	*sCsc3)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_35A()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_VERIFY_CSC_35A);



	AddAtallaCmdInfoField (ATALLA_FN_CSC_DATA, &kAtallaCmd, sData, strlen(sData));
	AddAtallaCmdInfoField (ATALLA_FN_EXPIRY_DATE, &kAtallaCmd, sExpiryDate, strlen(sExpiryDate));
	AddAtallaCmdInfoField (ATALLA_FN_CARD_SECURITY_CODE_KEY, &kAtallaCmd, sCscKey, strlen(sCscKey));
	AddAtallaCmdInfoField (ATALLA_FN_CSC_5, &kAtallaCmd, sCsc5, strlen(sCsc5));
	AddAtallaCmdInfoField (ATALLA_FN_CSC_4, &kAtallaCmd, sCsc4, strlen(sCsc4));
	AddAtallaCmdInfoField (ATALLA_FN_CSC_3, &kAtallaCmd, sCsc3, strlen(sCsc3));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_35A(OK)", PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/
int    command_45A(	char *buffer_received,
					int nLengthRcv,
					int	nCSCType,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[1+1];

	trace_event("Start command_45A()",PROCESSING);

	memset(sVerFlag, '\0', sizeof(sVerFlag));

	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_45A(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_VERIFY_CSC_45A);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	if (nCSCType == AMX_CSC5)
	{
		GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG, &kAtallaCmd, sVerFlag, &nLength);
	}
	else if (nCSCType == AMX_CSC4)
	{
		GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG_2, &kAtallaCmd, sVerFlag, &nLength);
	}
	else
	{
		GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG_3, &kAtallaCmd, sVerFlag, &nLength);
	}


	if (memcmp(sVerFlag, "Y", 1) == 0)
	{
		trace_event("End   command_45A(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_45A(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_45A(CRYPTOGRAPHIC_ERROR)",PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/******************************************************************************/
int    command_335(	char	*buffer_transmit ,
					int 	*nLengthSnd,
					char 	*sIncKeyUsage,
					char 	*sIncFormat,
					char 	*sOutgKeyUsage,
					char 	*sOutgFormat,
					char 	*sIncKey,
					char 	*sOutgKey,
					char	*sPinBlock,
					char	*sIncPBData,
					char	*sOutgPBData
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_335()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_TRANSLATE_PIN_335);

	AddAtallaCmdInfoField (ATALLA_FN_INC_KEY_USAGE, &kAtallaCmd, sIncKeyUsage, strlen(sIncKeyUsage));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_FORMAT, &kAtallaCmd, sIncFormat, strlen(sIncFormat));
	AddAtallaCmdInfoField (ATALLA_FN_OUTG_KEY_USAGE, &kAtallaCmd, sOutgKeyUsage, strlen(sOutgKeyUsage));
	AddAtallaCmdInfoField (ATALLA_FN_OUTG_PIN_BLOCK_FORMAT, &kAtallaCmd, sOutgFormat, strlen(sOutgFormat));
	AddAtallaCmdInfoField (ATALLA_FN_INC_KEY, &kAtallaCmd, sIncKey, strlen(sIncKey));
	AddAtallaCmdInfoField (ATALLA_FN_OUTG_KEY, &kAtallaCmd, sOutgKey, strlen(sOutgKey));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK, &kAtallaCmd, sPinBlock, strlen(sPinBlock));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_DATA, &kAtallaCmd, sIncPBData, strlen(sIncPBData));
	AddAtallaCmdInfoField (ATALLA_FN_OUTG_PIN_BLOCK_DATA, &kAtallaCmd, sOutgPBData, strlen(sOutgPBData));


	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_335(OK)", PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/
int    command_435(	char *buffer_received,
					int nLengthRcv,
					char *sPinBlock
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;

	trace_event("Start command_435()",PROCESSING);



	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_435(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_TRANSLATE_PIN_435);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);


	GetAtallaCmdInfoField (ATALLA_FN_OUTG_PIN_BLOCK, &kAtallaCmd, sPinBlock, &nLength);


	trace_event("End   command_435(OK)",PROCESSING);

	return(OK);
}

/******************************************************************************/
/******************************************************************************/
int  command_ibm_32(char	*buffer_transmit ,
					int 	*nLengthSnd,
					char 	*sFormat,
					char	*sPinBlock,
					char 	*sKey,
					char 	*sDecTable,
					char 	*sOffset,
					char 	*sValidationData,
					char 	*sPinLength,
					char	*sPad,
					char	*sPvk,
					char	*sPBData
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_ibm_32()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_VERIFY_PIN_IBM_32);

	AddAtallaCmdInfoField (ATALLA_FN_PIN_METHOD, &kAtallaCmd, "2", 1);
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_FORMAT, &kAtallaCmd, sFormat, strlen(sFormat));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK, &kAtallaCmd, sPinBlock, strlen(sPinBlock));
	AddAtallaCmdInfoField (ATALLA_FN_INC_KEY, &kAtallaCmd, sKey, strlen(sKey));
	AddAtallaCmdInfoField (ATALLA_FN_CONVERSION_TABLE, &kAtallaCmd, sDecTable, strlen(sDecTable));
	AddAtallaCmdInfoField (ATALLA_FN_OFFSET, &kAtallaCmd, sOffset, strlen(sOffset));
	AddAtallaCmdInfoField (ATALLA_FN_VALIDATION_DATA, &kAtallaCmd, sValidationData, strlen(sValidationData));
	AddAtallaCmdInfoField (ATALLA_FN_PAD, &kAtallaCmd, sPad, strlen(sPad));
	AddAtallaCmdInfoField (ATALLA_FN_CHECK_LENGTH, &kAtallaCmd, sPinLength, strlen(sPinLength));
	AddAtallaCmdInfoField (ATALLA_FN_PVK, &kAtallaCmd, sPvk, strlen(sPvk));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_DATA, &kAtallaCmd, sPBData, strlen(sPBData));


	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_ibm_32(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_ibm_42(	char *buffer_received,
						int nLengthRcv,
						char *sHsmResultCode
						)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[1+1];

	trace_event("Start command_ibm_42()",PROCESSING);

	memset(sVerFlag, '\0', sizeof(sVerFlag));

	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_ibm_42(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_VERIFY_PIN_IBM_42);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG, &kAtallaCmd, sVerFlag, &nLength);

	if (memcmp(sVerFlag, "Y", 1) == 0)
	{
		trace_event("End   command_ibm_42(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_ibm_42(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_ibm_42(CRYPTOGRAPHIC_ERROR)",PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/******************************************************************************/
int  command_visa_32(char	*buffer_transmit ,
					int 	*nLengthSnd,
					char 	*sFormat,
					char	*sPinBlock,
					char 	*sKey,
					char 	*sPvk,
					char 	*sPvk2,
					char 	*sPvv,
					char 	*sPvki,
					char	*sPan,
					char	*sPBData
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_visa_32()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_VERIFY_PIN_VISA_32);

	AddAtallaCmdInfoField (ATALLA_FN_PIN_METHOD, &kAtallaCmd, "3", 1);
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_FORMAT, &kAtallaCmd, sFormat, strlen(sFormat));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK, &kAtallaCmd, sPinBlock, strlen(sPinBlock));
	AddAtallaCmdInfoField (ATALLA_FN_INC_KEY, &kAtallaCmd, sKey, strlen(sKey));
	AddAtallaCmdInfoField (ATALLA_FN_PVK, &kAtallaCmd, sPvk, strlen(sPvk));
	AddAtallaCmdInfoField (ATALLA_FN_PVK_2, &kAtallaCmd, sPvk2, strlen(sPvk2));
	AddAtallaCmdInfoField (ATALLA_FN_PVV, &kAtallaCmd, sPvv, strlen(sPvv));
	AddAtallaCmdInfoField (ATALLA_FN_PVKI, &kAtallaCmd, sPvki, strlen(sPvki));
	AddAtallaCmdInfoField (ATALLA_FN_PAN, &kAtallaCmd, sPan, strlen(sPan));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_DATA, &kAtallaCmd, sPBData, strlen(sPBData));


	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_visa_32(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_visa_42(	char *buffer_received,
						int nLengthRcv,
						char *sHsmResultCode
						)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[1+1];

	trace_event("Start command_visa_42()",PROCESSING);

	memset(sVerFlag, '\0', sizeof(sVerFlag));

	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_visa_42(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_VERIFY_PIN_VISA_42);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG, &kAtallaCmd, sVerFlag, &nLength);

	if (memcmp(sVerFlag, "Y", 1) == 0)
	{
		trace_event("End   command_visa_42(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_visa_42(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_visa_42(CRYPTOGRAPHIC_ERROR)",PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/******************************************************************************/
int  command_ibm_37(char	*buffer_transmit ,
					int 	*nLengthSnd,
					char 	*sFormat,
					char	*sPinBlock,
					char 	*sKey,
					char 	*sDecTable,
					char 	*sOffset,
					char 	*sValidationData,
					char 	*sPinLength,
					char	*sPad,
					char	*sPvk,
					char	*NewPinBlock,
					char	*sPBData
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_ibm_37()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_PIN_CHANGE_IBM_37);

	AddAtallaCmdInfoField (ATALLA_FN_PIN_METHOD, &kAtallaCmd, "2", 1);
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_FORMAT, &kAtallaCmd, sFormat, strlen(sFormat));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK, &kAtallaCmd, sPinBlock, strlen(sPinBlock));
	AddAtallaCmdInfoField (ATALLA_FN_INC_KEY, &kAtallaCmd, sKey, strlen(sKey));
	AddAtallaCmdInfoField (ATALLA_FN_CONVERSION_TABLE, &kAtallaCmd, sDecTable, strlen(sDecTable));
	AddAtallaCmdInfoField (ATALLA_FN_OFFSET, &kAtallaCmd, sOffset, strlen(sOffset));
	AddAtallaCmdInfoField (ATALLA_FN_VALIDATION_DATA, &kAtallaCmd, sValidationData, strlen(sValidationData));
	AddAtallaCmdInfoField (ATALLA_FN_PAD, &kAtallaCmd, sPad, strlen(sPad));
	AddAtallaCmdInfoField (ATALLA_FN_CHECK_LENGTH, &kAtallaCmd, sPinLength, strlen(sPinLength));
	AddAtallaCmdInfoField (ATALLA_FN_PVK, &kAtallaCmd, sPvk, strlen(sPvk));
	AddAtallaCmdInfoField (ATALLA_FN_NEW_PIN_BLOCK, &kAtallaCmd, NewPinBlock, strlen(NewPinBlock));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_DATA, &kAtallaCmd, sPBData, strlen(sPBData));


	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_ibm_37(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_ibm_47(	char *buffer_received,
						int nLengthRcv,
						char *sNewOffset,
						char *sHsmResultCode
						)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[2+1];

	trace_event("Start command_ibm_47()",PROCESSING);

	memset(sVerFlag, '\0', sizeof(sVerFlag));

	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_ibm_47(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_PIN_CHANGE_IBM_47);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG, &kAtallaCmd, sVerFlag, &nLength);

	if (memcmp(sVerFlag, "Y", 1) == 0 || memcmp(sVerFlag, "NO", 2) == 0)
	{
		GetAtallaCmdInfoField (ATALLA_FN_NEW_OFFSET, &kAtallaCmd, sNewOffset, &nLength);
		trace_event("End   command_ibm_47(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_ibm_47(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_ibm_47(CRYPTOGRAPHIC_ERROR)",PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/******************************************************************************/
int  command_visa_37(char	*buffer_transmit ,
					int 	*nLengthSnd,
					char 	*sFormat,
					char	*sPinBlock,
					char 	*sKey,
					char 	*sPvk,
					char 	*sPvk2,
					char 	*sPvv,
					char 	*sPvki,
					char	*sPan,
					char	*NewPinBlock,
					char	*sPBData
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_visa_37()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_PIN_CHANGE_VISA_37);

	AddAtallaCmdInfoField (ATALLA_FN_PIN_METHOD, &kAtallaCmd, "3", 1);
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_FORMAT, &kAtallaCmd, sFormat, strlen(sFormat));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK, &kAtallaCmd, sPinBlock, strlen(sPinBlock));
	AddAtallaCmdInfoField (ATALLA_FN_INC_KEY, &kAtallaCmd, sKey, strlen(sKey));
	AddAtallaCmdInfoField (ATALLA_FN_PVK, &kAtallaCmd, sPvk, strlen(sPvk));
	AddAtallaCmdInfoField (ATALLA_FN_PVK_2, &kAtallaCmd, sPvk2, strlen(sPvk2));
	AddAtallaCmdInfoField (ATALLA_FN_PVV, &kAtallaCmd, sPvv, strlen(sPvv));
	AddAtallaCmdInfoField (ATALLA_FN_PVKI, &kAtallaCmd, sPvki, strlen(sPvki));
	AddAtallaCmdInfoField (ATALLA_FN_PAN, &kAtallaCmd, sPan, strlen(sPan));
	AddAtallaCmdInfoField (ATALLA_FN_NEW_PIN_BLOCK, &kAtallaCmd, NewPinBlock, strlen(NewPinBlock));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_DATA, &kAtallaCmd, sPBData, strlen(sPBData));


	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_visa_37(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_visa_47(	char *buffer_received,
						int nLengthRcv,
						char *sNewPvv,
						char *sHsmResultCode
						)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[2+1];

	trace_event("Start command_visa_47()",PROCESSING);

	memset(sVerFlag, '\0', sizeof(sVerFlag));

	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_visa_47(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_PIN_CHANGE_VISA_47);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG, &kAtallaCmd, sVerFlag, &nLength);

	if (memcmp(sVerFlag, "Y", 1) == 0 || memcmp(sVerFlag, "NO", 2) == 0)
	{
		GetAtallaCmdInfoField (ATALLA_FN_NEW_PVV, &kAtallaCmd, sNewPvv, &nLength);
		trace_event("End   command_visa_47(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_visa_47(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_visa_47(CRYPTOGRAPHIC_ERROR)",PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}
/******************************************************************************/
/******************************************************************************/

int  command_350(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char 	*sDerivationType,
				char	*sKeyAc,
				char	*sPan,
				char 	*sSeqNumb,
				char 	*sDiversification,
				char 	*sArqc,
				char 	*sPaddedData,
				char 	*sArc,
				char	*sHEmvTree,
				char	*sIvEmvTree,
				char	*sIndexEmvTree,
				char	*sFrc
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_350()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_VERIFY_ARQC_350);


	AddAtallaCmdInfoField (ATALLA_FN_DERIVATION_TYPE, &kAtallaCmd, sDerivationType, strlen(sDerivationType));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_AC, &kAtallaCmd, sKeyAc, strlen(sKeyAc));
	AddAtallaCmdInfoField (ATALLA_FN_PAN, &kAtallaCmd, sPan, strlen(sPan));
	AddAtallaCmdInfoField (ATALLA_FN_PAN_SEQ_NUMBER, &kAtallaCmd, sSeqNumb, strlen(sSeqNumb));
	AddAtallaCmdInfoField (ATALLA_FN_DIVERSIFICATION_DATA, &kAtallaCmd, sDiversification, strlen(sDiversification));
	AddAtallaCmdInfoField (ATALLA_FN_ARQC, &kAtallaCmd, sArqc, strlen(sArqc));
	AddAtallaCmdInfoField (ATALLA_FN_PADDED_DATA_BLOCK, &kAtallaCmd, sPaddedData, strlen(sPaddedData));
	AddAtallaCmdInfoField (ATALLA_FN_ARC, &kAtallaCmd, sArc, strlen(sArc));

	if (strlen(sHEmvTree) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_H_EMV_TREE, &kAtallaCmd, sHEmvTree, strlen(sHEmvTree));

	if (strlen(sIvEmvTree) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_IV_EMV_TREE, &kAtallaCmd, sIvEmvTree, strlen(sIvEmvTree));

	if (strlen(sIndexEmvTree) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_INDEX_EMV_TREE, &kAtallaCmd, sIndexEmvTree, strlen(sIndexEmvTree));

	if (strlen(sFrc) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_FRC, &kAtallaCmd, sFrc, strlen(sFrc));


	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_350(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_450(	char *buffer_received,
					int nLengthRcv,
					char *sArpc,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;


	trace_event("Start command_450()",PROCESSING);


	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_450(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_VERIFY_ARQC_450);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_ARPC, &kAtallaCmd, sArpc, &nLength);

	if ( strlen(sArpc) > 0 )
	{
		trace_event("End   command_450(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_450(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_450(CRYPTOGRAPHIC_ERROR)",PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}
/******************************************************************************/
/******************************************************************************/

int  command_351(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char 	*sDerivationType,
				char	*sPinBlockFormat,
				char	*sKey,
				char	*sKeyEnc,
				char	*sKeyMac,
				char	*sKeyAc,
				char	*sNewPinBlock,
				char	*sPinIssueNum,
				char	*sPan,
				char 	*sSeqNumb,
				char 	*sDiversification,
				char 	*sAppData,
				char 	*sPinBlockData,
				char 	*sOldPinBlock,
				char	*sHEmvTree,
				char	*sIvEmvTree,
				char	*sIndexEmvTree
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;



	trace_event("Start command_351()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_EMV_PIN_CHANGE_351);


	AddAtallaCmdInfoField (ATALLA_FN_DERIVATION_TYPE, &kAtallaCmd, sDerivationType, strlen(sDerivationType));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_FORMAT, &kAtallaCmd, sPinBlockFormat, strlen(sPinBlockFormat));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, "", 0);
	AddAtallaCmdInfoField (ATALLA_FN_INC_KEY, &kAtallaCmd, sKey, strlen(sKey));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_ENC, &kAtallaCmd, sKeyEnc, strlen(sKeyEnc));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_MAC, &kAtallaCmd, sKeyMac, strlen(sKeyMac));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_AC, &kAtallaCmd, sKeyAc, strlen(sKeyAc));
	AddAtallaCmdInfoField (ATALLA_FN_NEW_PIN_BLOCK, &kAtallaCmd, sNewPinBlock, strlen(sNewPinBlock));
	AddAtallaCmdInfoField (ATALLA_FN_PIN_ISSUE_NUMBER, &kAtallaCmd, sPinIssueNum, strlen(sPinIssueNum));
	AddAtallaCmdInfoField (ATALLA_FN_PAN, &kAtallaCmd, sPan, strlen(sPan));
	AddAtallaCmdInfoField (ATALLA_FN_PAN_SEQ_NUMBER, &kAtallaCmd, sSeqNumb, strlen(sSeqNumb));
	AddAtallaCmdInfoField (ATALLA_FN_DIVERSIFICATION_DATA, &kAtallaCmd, sDiversification, strlen(sDiversification));
	AddAtallaCmdInfoField (ATALLA_FN_APPLICATION_DATA, &kAtallaCmd, sAppData, strlen(sAppData));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_DATA, &kAtallaCmd, sPinBlockData, strlen(sPinBlockData));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK, &kAtallaCmd, sOldPinBlock, strlen(sOldPinBlock));

	if (strlen(sHEmvTree) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_H_EMV_TREE, &kAtallaCmd, sHEmvTree, strlen(sHEmvTree));

	if (strlen(sIvEmvTree) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_IV_EMV_TREE, &kAtallaCmd, sIvEmvTree, strlen(sIvEmvTree));

	if (strlen(sIndexEmvTree) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_INDEX_EMV_TREE, &kAtallaCmd, sIndexEmvTree, strlen(sIndexEmvTree));



	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_351(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_451(	char *buffer_received,
					int nLengthRcv,
					char *sEncrPinBlock,
					char *sMac,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[2+1];

	trace_event("Start command_451()",PROCESSING);

	memset(sVerFlag,	'\0',sizeof(sVerFlag));


	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_451(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_EMV_PIN_CHANGE_451);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_SANITY_CHECK_IND, &kAtallaCmd, sVerFlag, &nLength);

	if ( memcmp(sVerFlag, "Y", 1) != 0 )
	{
		trace_event("End   command_451(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}
	else
	{
		GetAtallaCmdInfoField (ATALLA_FN_OUTG_PIN_BLOCK, &kAtallaCmd, sEncrPinBlock, &nLength);
		GetAtallaCmdInfoField (ATALLA_FN_MAC, &kAtallaCmd, sMac, &nLength);
		trace_event("End   command_451(OK)", PROCESSING);
		return (OK);
	}

	trace_event("End   command_451(OK)",PROCESSING);

	return(OK);
}

/******************************************************************************/
/******************************************************************************/

int  command_352(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char 	*sDerivationType,
				char	*sKeyMac,
				char	*sPan,
				char 	*sSeqNumb,
				char 	*sDiversification,
				char 	*sMacLen,
				char 	*sContInitVec,
				char 	*sPaddedData,
				char	*sHEmvTree,
				char	*sIvEmvTree,
				char	*sIndexEmvTree
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;



	trace_event("Start command_352()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_GENERATE_EMV_MAC_352);


	AddAtallaCmdInfoField (ATALLA_FN_DERIVATION_TYPE, &kAtallaCmd, sDerivationType, strlen(sDerivationType));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_MAC, &kAtallaCmd, sKeyMac, strlen(sKeyMac));
	AddAtallaCmdInfoField (ATALLA_FN_PAN, &kAtallaCmd, sPan, strlen(sPan));
	AddAtallaCmdInfoField (ATALLA_FN_PAN_SEQ_NUMBER, &kAtallaCmd, sSeqNumb, strlen(sSeqNumb));
	AddAtallaCmdInfoField (ATALLA_FN_DIVERSIFICATION_DATA, &kAtallaCmd, sDiversification, strlen(sDiversification));
	AddAtallaCmdInfoField (ATALLA_FN_MAC_LENGTH, &kAtallaCmd, sMacLen, strlen(sMacLen));
	AddAtallaCmdInfoField (ATALLA_FN_CONTINUATION_INIT_VECTOR, &kAtallaCmd, sContInitVec, strlen(sContInitVec));
	AddAtallaCmdInfoField (ATALLA_FN_PADDED_DATA_BLOCK, &kAtallaCmd, sPaddedData, strlen(sPaddedData));

	if (strlen(sHEmvTree) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_H_EMV_TREE, &kAtallaCmd, sHEmvTree, strlen(sHEmvTree));

	if (strlen(sIvEmvTree) > 0)
	AddAtallaCmdInfoField (ATALLA_FN_IV_EMV_TREE, &kAtallaCmd, sIvEmvTree, strlen(sIvEmvTree));

	if (strlen(sIndexEmvTree) > 0)
	AddAtallaCmdInfoField (ATALLA_FN_INDEX_EMV_TREE, &kAtallaCmd, sIndexEmvTree, strlen(sIndexEmvTree));



	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_352(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_452(	char *buffer_received,
					int   nLengthRcv,
					char *sMacLen,
					char *sMac,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;

	trace_event("Start command_452()",PROCESSING);


	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_452(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_GENERATE_EMV_MAC_452);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_MAC_LENGTH, &kAtallaCmd, sMacLen, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_MAC, &kAtallaCmd, sMac, &nLength);



	trace_event("End   command_452(OK)",PROCESSING);

	return(OK);
}

/******************************************************************************/
/******************************************************************************/

int  command_3FC(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char 	*sDerivationType,
				char	*sKeyEnc,
				char	*sKeyMac,
				char	*sKeyAc,
				char	*sPan,
				char 	*sSeqNumb,
				char 	*sDiversification,
				char 	*sAppData,
				char 	*sPvk,
				char 	*sDecTable,
				char	*sOffset,
				char	*sValData,
				char	*sPad
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_3FC()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_EMV_PIN_CHANGE_IBM_3FC);


	AddAtallaCmdInfoField (ATALLA_FN_DERIVATION_TYPE, &kAtallaCmd, sDerivationType, strlen(sDerivationType));
	AddAtallaCmdInfoField (ATALLA_FN_PIN_METHOD, &kAtallaCmd, "2", 1);
	AddAtallaCmdInfoField (ATALLA_FN_KEY_ENC, &kAtallaCmd, sKeyEnc, strlen(sKeyEnc));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_MAC, &kAtallaCmd, sKeyMac, strlen(sKeyMac));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_AC, &kAtallaCmd, sKeyAc, strlen(sKeyAc));
	AddAtallaCmdInfoField (ATALLA_FN_PAN, &kAtallaCmd, sPan, strlen(sPan));
	AddAtallaCmdInfoField (ATALLA_FN_PAN_SEQ_NUMBER, &kAtallaCmd, sSeqNumb, strlen(sSeqNumb));
	AddAtallaCmdInfoField (ATALLA_FN_DIVERSIFICATION_DATA, &kAtallaCmd, sDiversification, strlen(sDiversification));
	AddAtallaCmdInfoField (ATALLA_FN_APPLICATION_DATA, &kAtallaCmd, sAppData, strlen(sAppData));
	AddAtallaCmdInfoField (ATALLA_FN_PVK, &kAtallaCmd, sPvk, strlen(sPvk));
	AddAtallaCmdInfoField (ATALLA_FN_CONVERSION_TABLE, &kAtallaCmd, sDecTable, strlen(sDecTable));
	AddAtallaCmdInfoField (ATALLA_FN_OFFSET, &kAtallaCmd, sOffset, strlen(sOffset));
	AddAtallaCmdInfoField (ATALLA_FN_VALIDATION_DATA, &kAtallaCmd, sValData, strlen(sValData));
	AddAtallaCmdInfoField (ATALLA_FN_PAD, &kAtallaCmd, sPad, strlen(sPad));


	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_3FC(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_4FC(	char *buffer_received,
					int   nLengthRcv,
					char *sEncrPinBlock,
					char *sMac,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;

	trace_event("Start command_4FC()",PROCESSING);


	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_4FC(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_EMV_PIN_CHANGE_IBM_4FC);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_OUTG_PIN_BLOCK, &kAtallaCmd, sEncrPinBlock, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_MAC, &kAtallaCmd, sMac, &nLength);



	trace_event("End   command_4FC(OK)",PROCESSING);

	return(OK);
}
/******************************************************************************/
/******************************************************************************/

int  command_98(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char	*sKeyMsgAuth,
				char	*sMacLen,
				char	*sMacAlgo,
				char 	*sInitVect,
				char 	*sDataFormat,
				char 	*sDataLen,
				char 	*sData,
				char 	*sKeyUsage
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_98()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_GENERATE_MAC_98);



	AddAtallaCmdInfoField (ATALLA_FN_MSG_AUTHENTICATION_KEY, &kAtallaCmd, sKeyMsgAuth, strlen(sKeyMsgAuth));
	AddAtallaCmdInfoField (ATALLA_FN_MAC_LENGTH, &kAtallaCmd, sMacLen, strlen(sMacLen));
	AddAtallaCmdInfoField (ATALLA_FN_MAC_ALGORITHM, &kAtallaCmd, sMacAlgo, strlen(sMacAlgo));
	AddAtallaCmdInfoField (ATALLA_FN_CONTINUATION_INIT_VECTOR, &kAtallaCmd, sInitVect, strlen(sInitVect));
	AddAtallaCmdInfoField (ATALLA_FN_DATA_FORMAT, &kAtallaCmd, sDataFormat, strlen(sDataFormat));
	AddAtallaCmdInfoField (ATALLA_FN_DATA_LENGTH, &kAtallaCmd, sDataLen, strlen(sDataLen));
	AddAtallaCmdInfoField (ATALLA_FN_DATA, &kAtallaCmd, sData, strlen(sData));
	if (strlen(sKeyUsage) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, sKeyUsage, strlen(sKeyUsage));


	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_98(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_A8(	char *buffer_received,
					int   nLengthRcv,
					char *sMac,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;



	trace_event("Start command_A8()",PROCESSING);


	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_A8(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_GENERATE_MAC_A8);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_MAC, &kAtallaCmd, sMac, &nLength);


	trace_event("End   command_A8(OK)",PROCESSING);

	return(OK);
}
/******************************************************************************/
/******************************************************************************/

int  command_99(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char	*sKeyMsgAuth,
				char	*sMacLen,
				char	*sMacAlgo,
				char 	*sInitVect,
				char 	*sDataFormat,
				char 	*sDataLen,
				char 	*sData,
				char	*sMac,
				char 	*sKeyUsage
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;
	char				szMac[9+1];


	trace_event("Start command_99()", PROCESSING);

	memset(szMac, '\0', sizeof(szMac));

	memcpy(szMac, sMac, 4);
	memcpy(szMac + 4, " ",1);
	memcpy(szMac + 5, sMac+4, 4);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_VERIFY_MAC_99);


	AddAtallaCmdInfoField (ATALLA_FN_MSG_AUTHENTICATION_KEY, &kAtallaCmd, sKeyMsgAuth, strlen(sKeyMsgAuth));
	AddAtallaCmdInfoField (ATALLA_FN_MAC_ALGORITHM, &kAtallaCmd, sMacAlgo, strlen(sMacAlgo));
	AddAtallaCmdInfoField (ATALLA_FN_CONTINUATION_INIT_VECTOR, &kAtallaCmd, sInitVect, strlen(sInitVect));
	AddAtallaCmdInfoField (ATALLA_FN_MAC_LENGTH, &kAtallaCmd, sMacLen, strlen(sMacLen));
	AddAtallaCmdInfoField (ATALLA_FN_DATA_FORMAT, &kAtallaCmd, sDataFormat, strlen(sDataFormat));
	AddAtallaCmdInfoField (ATALLA_FN_DATA_LENGTH, &kAtallaCmd, sDataLen, strlen(sDataLen));
	AddAtallaCmdInfoField (ATALLA_FN_DATA, &kAtallaCmd, sData, strlen(sData));
	AddAtallaCmdInfoField (ATALLA_FN_MAC, &kAtallaCmd, szMac, strlen(szMac));
	if (strlen(sKeyUsage) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, sKeyUsage, strlen(sKeyUsage));


	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_99(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_A9(	char *buffer_received,
					int   nLengthRcv,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[1+1];


	trace_event("Start command_A9()",PROCESSING);

	memset(sVerFlag, '\0',sizeof(sVerFlag));


	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_A9(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_VERIFY_MAC_A9);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG, &kAtallaCmd, sVerFlag, &nLength);

	if (memcmp(sVerFlag, "Y", 1) == 0)
	{
		trace_event("End   command_A9(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_A9(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_A9(CRYPTOGRAPHIC_ERROR)",PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/******************************************************************************/

int  command_39B(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char	*sKeyMsgAuth,
				char	*sKeyMsgAuth2,
				char	*sMacAlgo,
				char 	*sData
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_39B()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_GENERATE_MAC_HMAC_39B);



	AddAtallaCmdInfoField (ATALLA_FN_MSG_AUTHENTICATION_KEY, &kAtallaCmd, sKeyMsgAuth, strlen(sKeyMsgAuth));
	AddAtallaCmdInfoField (ATALLA_FN_MSG_AUTHENTICATION_KEY_2, &kAtallaCmd, sKeyMsgAuth2, strlen(sKeyMsgAuth2));
	AddAtallaCmdInfoField (ATALLA_FN_MAC_ALGORITHM, &kAtallaCmd, sMacAlgo, strlen(sMacAlgo));
	AddAtallaCmdInfoField (ATALLA_FN_DATA, &kAtallaCmd, sData, strlen(sData));



	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_39B(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_49B(	char *buffer_received,
					int   nLengthRcv,
					char *sMac,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;



	trace_event("Start command_49B()",PROCESSING);


	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_49B(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_GENERATE_MAC_HMAC_49B);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_MAC, &kAtallaCmd, sMac, &nLength);


	trace_event("End   command_49B(OK)",PROCESSING);

	return(OK);
}
/******************************************************************************/
/******************************************************************************/

int  command_120(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char	*sKeyUsage,
				char	*sPublicExponent,
				char	*sModulusSize,
				char 	*sKeySlot
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_120()", PROCESSING);


	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_GENERATE_RSA_KEY_PAIR_120);

	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, sKeyUsage, strlen(sKeyUsage));
	AddAtallaCmdInfoField (ATALLA_FN_PUBLIC_EXPONENT, &kAtallaCmd, sPublicExponent, strlen(sPublicExponent));
	AddAtallaCmdInfoField (ATALLA_FN_MODULUS_SIZE, &kAtallaCmd, sModulusSize, strlen(sModulusSize));
	if (strlen(sKeySlot) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_KEY_SLOT, &kAtallaCmd, sKeySlot, strlen(sKeySlot));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;

	trace_event("End   command_120(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_220(	char *buffer_received,
					int   nLengthRcv,
					char *sRsaPubKey,
					char *sRsaPrivKey,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;


	trace_event("Start command_220()",PROCESSING);


	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_220(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_GENERATE_RSA_KEY_PAIR_220);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_RSA_PUBLIC_KEY, &kAtallaCmd, sRsaPubKey, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_RSA_PRIVATE_KEY, &kAtallaCmd, sRsaPrivKey, &nLength);


	trace_event("End   command_220(OK)",PROCESSING);

	return(OK);
}
/******************************************************************************/
/******************************************************************************/

int  command_123(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char	*sKeyUsage,
				char	*sHashAlgo,
				char	*sSignatureAlgo,
				char	*sPublicExponent,
				char	*sModulusSize,
				char	*PublicKeyData,
				char 	*sSignature,
				char	*sCaPublicKey
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_123()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_VERIF_PUB_KEY_AND_GEN_RSA_AKB_123);

	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, sKeyUsage, strlen(sKeyUsage));
	AddAtallaCmdInfoField (ATALLA_FN_HASH_ALGORITHM, &kAtallaCmd, sHashAlgo, strlen(sHashAlgo));
	AddAtallaCmdInfoField (ATALLA_FN_SIGNATURE_ALGORITHM, &kAtallaCmd, sSignatureAlgo, strlen(sSignatureAlgo));
	AddAtallaCmdInfoField (ATALLA_FN_PUBLIC_EXPONENT, &kAtallaCmd, sPublicExponent, strlen(sPublicExponent));
	AddAtallaCmdInfoField (ATALLA_FN_MODULUS_SIZE, &kAtallaCmd, sModulusSize, strlen(sModulusSize));
	AddAtallaCmdInfoField (ATALLA_FN_PUBLIC_KEY_DATA, &kAtallaCmd, PublicKeyData, strlen(PublicKeyData));
	AddAtallaCmdInfoField (ATALLA_FN_SIGNATURE, &kAtallaCmd, sSignature, strlen(sSignature));
	AddAtallaCmdInfoField (ATALLA_FN_CA_PUBLIC_KEY, &kAtallaCmd, sCaPublicKey, strlen(sCaPublicKey));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;

	trace_event("End   command_123(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_223(	char *buffer_received,
					int   nLengthRcv,
					char *sRsaPubKey
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;


	trace_event("Start command_223()",PROCESSING);



	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_223(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_VERIF_PUB_KEY_AND_GEN_RSA_AKB_223);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_RSA_PUBLIC_KEY, &kAtallaCmd, sRsaPubKey, &nLength);


	trace_event("End   command_223(OK)",PROCESSING);

	return(OK);
}
/******************************************************************************/
/******************************************************************************/

int  command_124(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char	*sHashAlgo,
				char	*sSignatureAlgo,
				char	*sDataLen,
				char	*sData,
				char	*sRsaPrivateKey
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;

	trace_event("Start command_124()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_GEN_DIGITAL_SIGNATURE_124);

	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, "0", 1);
	AddAtallaCmdInfoField (ATALLA_FN_HASH_ALGORITHM, &kAtallaCmd, sHashAlgo, strlen(sHashAlgo));
	AddAtallaCmdInfoField (ATALLA_FN_SIGNATURE_ALGORITHM, &kAtallaCmd, sSignatureAlgo, strlen(sSignatureAlgo));
	AddAtallaCmdInfoField (ATALLA_FN_DATA_LENGTH, &kAtallaCmd, sDataLen, strlen(sDataLen));
	AddAtallaCmdInfoField (ATALLA_FN_DATA, &kAtallaCmd, sData, strlen(sData));
	AddAtallaCmdInfoField (ATALLA_FN_EMPTY_FIELD, &kAtallaCmd, "", 0);
	AddAtallaCmdInfoField (ATALLA_FN_RSA_PRIVATE_KEY, &kAtallaCmd, sRsaPrivateKey, strlen(sRsaPrivateKey));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;

	trace_event("End   command_124(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_224(	char *buffer_received,
					int   nLengthRcv,
					char *sSignature
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;


	trace_event("Start command_224()",PROCESSING);



	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_224(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_GEN_DIGITAL_SIGNATURE_224);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_SIGNATURE, &kAtallaCmd, sSignature, &nLength);


	trace_event("End   command_224(OK)",PROCESSING);

	return(OK);
}
/******************************************************************************/
/******************************************************************************/

int  command_125(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char	*sHashAlgo,
				char	*sSignatureAlgo,
				char	*sDataLen,
				char	*sData,
				char	*sSignature,
				char	*sRsaPublicKey
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;

	trace_event("Start command_125()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_VERIF_DIGITAL_SIGNATURE_125);

	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, "0", 1);
	AddAtallaCmdInfoField (ATALLA_FN_HASH_ALGORITHM, &kAtallaCmd, sHashAlgo, strlen(sHashAlgo));
	AddAtallaCmdInfoField (ATALLA_FN_SIGNATURE_ALGORITHM, &kAtallaCmd, sSignatureAlgo, strlen(sSignatureAlgo));
	AddAtallaCmdInfoField (ATALLA_FN_DATA_LENGTH, &kAtallaCmd, sDataLen, strlen(sDataLen));
	AddAtallaCmdInfoField (ATALLA_FN_DATA, &kAtallaCmd, sData, strlen(sData));
	AddAtallaCmdInfoField (ATALLA_FN_EMPTY_FIELD, &kAtallaCmd, "", 0);
	AddAtallaCmdInfoField (ATALLA_FN_SIGNATURE, &kAtallaCmd, sSignature, strlen(sSignature));
	AddAtallaCmdInfoField (ATALLA_FN_RSA_PUBLIC_KEY, &kAtallaCmd, sRsaPublicKey, strlen(sRsaPublicKey));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;

	trace_event("End   command_125(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_225(	char *buffer_received,
					int   nLengthRcv
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;
	char				sVerFlag[1+1];


	trace_event("Start command_225()",PROCESSING);

	memset(sVerFlag, '\0', sizeof(sVerFlag));


	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_225(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_VERIF_DIGITAL_SIGNATURE_225);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_VERIFICATION_FLAG, &kAtallaCmd, sVerFlag, &nLength);

	if (memcmp(sVerFlag, "Y", 1) == 0)
	{
		trace_event("End   command_225(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End   command_225(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_225(CRYPTOGRAPHIC_ERROR)",PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}
/******************************************************************************/
/******************************************************************************/

int  command_126(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char	*sFlag,
				char	*sHashAlgo,
				char	*sEncoding,
				char	*sDataLen,
				char	*sData,
				char	*sInterResult
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;

	trace_event("Start command_126()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_GEN_MESSAGE_DIGEST_126);

	AddAtallaCmdInfoField (ATALLA_FN_FLAG, &kAtallaCmd, sFlag, strlen(sFlag));
	AddAtallaCmdInfoField (ATALLA_FN_HASH_ALGORITHM, &kAtallaCmd, sHashAlgo, strlen(sHashAlgo));
	AddAtallaCmdInfoField (ATALLA_FN_ENCODING, &kAtallaCmd, sEncoding, strlen(sEncoding));
	AddAtallaCmdInfoField (ATALLA_FN_DATA_LENGTH, &kAtallaCmd, sDataLen, strlen(sDataLen));
	AddAtallaCmdInfoField (ATALLA_FN_DATA, &kAtallaCmd, sData, strlen(sData));
	AddAtallaCmdInfoField (ATALLA_FN_INTERMEDIATE_RESULT, &kAtallaCmd, sInterResult, strlen(sInterResult));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;

	trace_event("End   command_126(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_226(	char *buffer_received,
					int   nLengthRcv,
					char *sMsgDigest,
					char *sHsmResultCode
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;


	trace_event("Start command_226()",PROCESSING);


	memcpy(sHsmResultCode, buffer_received +4,4); /*Need Information*/

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_226(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_GEN_MESSAGE_DIGEST_226);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_MESSAGE_DIGEST, &kAtallaCmd, sMsgDigest, &nLength);


	trace_event("End   command_226(OK)",PROCESSING);

	return(OK);
}
/******************************************************************************/
/******************************************************************************/

int  command_12F(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char	*sKeyLen,
				char	*sDataLen,
				char	*sData,
				char	*sRsaPublicKey,
				char	*sTmkHeader
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;

	trace_event("Start command_12F()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_GEN_TMK_AND_ENCRYPT_PK_12F);

	AddAtallaCmdInfoField (ATALLA_FN_KEY_LENGTH, &kAtallaCmd, sKeyLen, strlen(sKeyLen));
	AddAtallaCmdInfoField (ATALLA_FN_DATA_LENGTH, &kAtallaCmd, sDataLen, strlen(sDataLen));
	AddAtallaCmdInfoField (ATALLA_FN_DATA, &kAtallaCmd, sData, strlen(sData));
	AddAtallaCmdInfoField (ATALLA_FN_RSA_PUBLIC_KEY, &kAtallaCmd, sRsaPublicKey, strlen(sRsaPublicKey));
	if (strlen(sTmkHeader) > 0)
		AddAtallaCmdInfoField (ATALLA_FN_MASTER_KEY_HEADER, &kAtallaCmd, sTmkHeader, strlen(sTmkHeader));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;

	trace_event("End   command_12F(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_22F(	char *buffer_received,
					int   nLengthRcv,
					char *sTmk,
					char *sCheckValue,
					char *sTmkPublicKey
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;


	trace_event("Start command_22F()",PROCESSING);



	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_22F(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_GEN_TMK_AND_ENCRYPT_PK_22F);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_ATM_MASTER_KEY, &kAtallaCmd, sTmk, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_CHECK_DIGITS, &kAtallaCmd, sCheckValue, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_ATM_MASTER_KEY_UNDER_PK, &kAtallaCmd, sTmkPublicKey, &nLength);


	trace_event("End   command_22F(OK)",PROCESSING);

	return(OK);
}
/******************************************************************************/
/******************************************************************************/

int  command_7E(char	*buffer_transmit ,
				int 	*nLengthSnd,
				char	*sMethod,
				char	*sKeyUsage,
				char	*sKeyLmk
				)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;

	trace_event("Start command_7E()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_GEN_CHECK_DIGITS_7E);

	AddAtallaCmdInfoField (ATALLA_FN_METHOD, &kAtallaCmd, sMethod, strlen(sMethod));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, sKeyUsage, strlen(sKeyUsage));
	AddAtallaCmdInfoField (ATALLA_FN_WORKING_KEY_MFK, &kAtallaCmd, sKeyLmk, strlen(sKeyLmk));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;

	trace_event("End   command_7E(OK)", PROCESSING);
	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_8E(	char *buffer_received,
					int   nLengthRcv,
					char *sCheckValue
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;


	trace_event("Start command_8E()",PROCESSING);



	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_8E(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_GEN_CHECK_DIGITS_8E);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_CHECK_DIGITS, &kAtallaCmd, sCheckValue, &nLength);


	trace_event("End   command_8E(OK)",PROCESSING);

	return(OK);
}
/******************************************************************************/
/******************************************************************************/
int    command_3E(	char	*buffer_transmit ,
					int 	*nLengthSnd,
					char 	*sIncFormat,
					char 	*sOutgFormat,
					char 	*sIncKey,
					char	*sIncKeyMng,
					char	*sIncKeyKsn,
					char 	*sOutgKey,
					char	*sOutgKeyMng,
					char	*sOutgKeyKsn,
					char	*sPinBlock,
					char	*sIncPBData,
					char	*sOutgPBData
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;


	trace_event("Start command_3E()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_TRANSLATE_ISO_PIN_3E);

	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_FORMAT, &kAtallaCmd, sIncFormat, strlen(sIncFormat));
	AddAtallaCmdInfoField (ATALLA_FN_OUTG_PIN_BLOCK_FORMAT, &kAtallaCmd, sOutgFormat, strlen(sOutgFormat));
	AddAtallaCmdInfoField (ATALLA_FN_INC_KEY, &kAtallaCmd, sIncKey, strlen(sIncKey));
	AddAtallaCmdInfoField (ATALLA_FN_INC_KEY_MNG, &kAtallaCmd, sIncKeyMng, strlen(sIncKeyMng));
	AddAtallaCmdInfoField (ATALLA_FN_INC_KEY_KSN, &kAtallaCmd, sIncKeyKsn, strlen(sIncKeyKsn));
	AddAtallaCmdInfoField (ATALLA_FN_OUTG_KEY, &kAtallaCmd, sOutgKey, strlen(sOutgKey));
	AddAtallaCmdInfoField (ATALLA_FN_OUTG_KEY_MNG, &kAtallaCmd, sOutgKeyMng, strlen(sOutgKeyMng));
	AddAtallaCmdInfoField (ATALLA_FN_OUTG_KEY_KSN, &kAtallaCmd, sOutgKeyKsn, strlen(sOutgKeyKsn));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK, &kAtallaCmd, sPinBlock, strlen(sPinBlock));
	AddAtallaCmdInfoField (ATALLA_FN_INC_PIN_BLOCK_DATA, &kAtallaCmd, sIncPBData, strlen(sIncPBData));
	AddAtallaCmdInfoField (ATALLA_FN_OUTG_PIN_BLOCK_DATA, &kAtallaCmd, sOutgPBData, strlen(sOutgPBData));


	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_3E(OK)", PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/
int    command_4E(	char *buffer_received,
					int nLengthRcv,
					char *sPinBlock
					)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;

	trace_event("Start command_4E()",PROCESSING);



	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_4E(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_TRANSLATE_ISO_PIN_4E);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);


	GetAtallaCmdInfoField (ATALLA_FN_OUTG_PIN_BLOCK, &kAtallaCmd, sPinBlock, &nLength);


	trace_event("End   command_4E(OK)",PROCESSING);

	return(OK);
}
/******************************************************************************/
/******************************************************************************/

int    command_11B(	char		*buffer_transmit ,
	int*		nLengthSnd,
	char*       KeyUsage,
	char*       keyZmk,
	char*       zmkLmk
	)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int 				nLength;

	trace_event("Start command_11B()", PROCESSING);

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_REQ_IMPORT_KEY_NAKB_11B);

	AddAtallaCmdInfoField (ATALLA_FN_KEY_USAGE, &kAtallaCmd, KeyUsage, strlen(KeyUsage));
	AddAtallaCmdInfoField (ATALLA_FN_WORKING_KEY_KEK, &kAtallaCmd, keyZmk, strlen(keyZmk));
	AddAtallaCmdInfoField (ATALLA_FN_KEY_EXCHANGE_KEY, &kAtallaCmd, zmkLmk, strlen(zmkLmk));

	DumpAtallaCmdInfo(&kAtallaCmd);
	nLength= BuildAtallaCmdInfoBuffer (buffer_transmit, &kAtallaCmd);
	(*nLengthSnd) = nLength;



	trace_event("End   command_11B(OK)", PROCESSING);
	return(OK);
}

/******************************************************************************/
/******************************************************************************/
int    command_21B(	char *buffer_received,
	int nLengthRcv,
	char *keyLmk,
	char *check_value
	)
{
	int					nResult;
	TSAtallaCmd			kAtallaCmd;
	int					nLength;

	trace_event("Start command_21B()",PROCESSING);

	if (memcmp(buffer_received + 1 , "00", 2) == 0)
	{
		TextError(buffer_received + 4);
		trace_event("End   command_21B(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR);
	}

	nResult = InitAtallaCmd(&kAtallaCmd, ATALLA_MN_RESP_IMPORT_KEY_NAKB_21B);
	AnalyseAtallaCmdInfo(buffer_received, nLengthRcv, &kAtallaCmd);

	GetAtallaCmdInfoField (ATALLA_FN_WORKING_KEY_MFK, &kAtallaCmd, keyLmk, &nLength);
	GetAtallaCmdInfoField (ATALLA_FN_CHECK_DIGITS, &kAtallaCmd, check_value, &nLength);


	trace_event("End   command_21B(OK)",PROCESSING);
	return(OK);
}


