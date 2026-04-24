/*******************************************************************************/
/* (c) Hightech Payment Systems 2001                                          */
/* NAME                                                                       */
/* hsm_commands.c                                                         */
/* */
/* FUNCTION                                                                    */
/* Function of hsm commands                                            */
/* NOTES                                                                       */
/* */
/* MODIFIED                                                                    */
/* H.YOUSFI   12/05/2002 -         V 2.0.1                              */
/* The buffer constructed in the command CD was incorrect               */
/* See HYJCC120502                                                      */
/* H.YOUSFI   10/10/2003 -         V 2.0.2							   */
/* I.HASSANI  10/10/2003 _																	   */
/* Ajout des commandes de gestion du MAC                               */
/*******************************************************************************/

#include <stdio.h>
#include <hsm_inc.h>
#include <stdlib.h>
#include <string.h>



/******************************************************************************/
/* Fonction command_AE.                                                       */
/* -------------------                                                        */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater TPK encryptee sous LMK vers TMK       */
/******************************************************************************/
int 
	command_AE(char *buffer_transmit,
	char *tmk_lmk,
	char *tpk_lmk
	)
{

	trace_event("Debut command_AE()", PROCESSING);

	memcpy(buffer_transmit, "AE", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, tmk_lmk, strlen(tmk_lmk));
	buffer_transmit += strlen(tmk_lmk);
	memcpy(buffer_transmit, tpk_lmk, strlen(tpk_lmk));

	trace_event("Fin   command_AE()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_AF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM TPK sous TMK ainsi que le code d'erreur.                 */
/* Entree      : Buffer recu du HSM                                           */
/******************************************************************************/
int 
	command_AF(char *buffer_received,
	char *tpk_tmk,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_AF()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "AF", 2) != 0)
		return (ERROR_COMMAND);

	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0)
		return (CHECK_ERROR);
	buffer_received += 4;

	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(tpk_tmk, buffer_received, 2 * SIZE_KEY);
		break;
		tpk_tmk[2 * SIZE_KEY] = '\0';
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(tpk_tmk, buffer_received, 3 * SIZE_KEY);
		break;
		tpk_tmk[3 * SIZE_KEY] = '\0';
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(tpk_tmk, buffer_received, SIZE_KEY);
		break;
		tpk_tmk[SIZE_KEY] = '\0';
	}

	trace_event("End   command_AF(OK)", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_AG.                                                       	 */
/* -------------------                                                        	 */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre	 
*/
/* au HSM pour translater TAK encryptee sous LMK vers TMK         */
/******************************************************************************/
int 
	command_AG(char *buffer_transmit,
	char *tmk_lmk,
	char *tak_lmk
	)
{

	trace_event("Debut command_AG()", PROCESSING);

	memcpy(buffer_transmit, "AG", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, tmk_lmk, strlen(tmk_lmk));
	buffer_transmit += strlen(tmk_lmk);
	memcpy(buffer_transmit, tak_lmk, strlen(tak_lmk));

	trace_event("Fin   command_AG()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_AH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM TAK sous TMK ainsi que le code d'erreur.                 */
/******************************************************************************/
int 
	command_AH(char *buffer_received,
	char *tak_tmk,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_AH()", PROCESSING);

	memset(Command, 0, 3);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "AH", 2) != 0)
		return (ERROR_COMMAND);

	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0)
		return (CHECK_ERROR);
	buffer_received += 4;

	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(tak_tmk, buffer_received, 2 * SIZE_KEY);
		break;
		tak_tmk[2 * SIZE_KEY] = '\0';
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(tak_tmk, buffer_received, 3 * SIZE_KEY);
		break;
		tak_tmk[3 * SIZE_KEY] = '\0';
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(tak_tmk, buffer_received, SIZE_KEY);
		break;
		tak_tmk[SIZE_KEY] = '\0';
	}


	trace_event("End   command_AH(OK)", PROCESSING);
	return (OK);
}

/*******************************************************************************/
/* Fonction command_AS.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour generer la paire CVK sous LMK                     */
/*******************************************************************************/
int 
	command_AS(char *buffer_transmit)
{
	trace_event("Debut command_AS()", PROCESSING);
	memcpy(buffer_transmit, "AS", 2);
	trace_event("Fin   command_AS()", PROCESSING);

	return (OK);
}

/******************************************************************************/
/* Fonction command_AT.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM la paire CVK sous LMK, la Check_value ainsi que          */
/* le code d'erreur.                                            */
/******************************************************************************/
int 
	command_AT(char *buffer_received,
	char *cvka_lmk,
	char *cvkb_lmk,
	char *check_valuea,
	char *check_valueb,
	char *err_code)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_AT()", PROCESSING);

	memset(Command, 0, 3);

	memset(cvka_lmk, 0, SIZE_KEY);
	memset(cvkb_lmk, 0, SIZE_KEY);
	memset(check_valuea, 0, SIZE_KEY);
	memset(check_valuea, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "AT", 2) != 0)
		return (ERROR_COMMAND);

	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0)
		return (CHECK_ERROR);
	buffer_received += 4;

	memcpy(cvka_lmk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(cvkb_lmk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(check_valuea, buffer_received, SIZE_KEY);
	buffer_received += 6;
	memcpy(check_valueb, buffer_received, SIZE_KEY);

	trace_event("End   command_AT()", PROCESSING);
	return (OK);
}

/*******************************************************************************/
/* Fonction command_AU.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater la paire CVK sous LMK vers une paire   */
/* sous ZMK   						                           */
/*******************************************************************************/
int 
	command_AU(char *buffer_transmit,
	char *zmk_lmk,
	char *cvka_lmk,
	char *cvkb_lmk)
{
	trace_event("Debut command_AU()", PROCESSING);

	memcpy(buffer_transmit, "AU", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk_lmk, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, cvka_lmk, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, cvkb_lmk, SIZE_KEY);

	trace_event("Fin   command_AU()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_AV.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM la paire CVK sous ZMK, la Check value de chaque cle de   */
/* la pair et le code d'erreur.                                 */
/******************************************************************************/
int 
	command_AV(char *buffer_received,
	char *cvka_zmk,
	char *cvkb_zmk,
	char *check_valuea,
	char *check_valueb,
	char *err_code)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_AV()", PROCESSING);

	memset(cvka_zmk, 0, SIZE_KEY);
	memset(cvkb_zmk, 0, SIZE_KEY);
	memset(check_valuea, 0, SIZE_KEY);
	memset(check_valueb, 0, SIZE_KEY);

	memset(Command, 0, 3);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "AV", 2) != 0)
		return (ERROR_COMMAND);

	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0)
		return (CHECK_ERROR);
	buffer_received += 4;

	memcpy(cvka_zmk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(cvkb_zmk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(check_valuea, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(check_valueb, buffer_received, SIZE_KEY);

	trace_event("End   command_AV()", PROCESSING);
	return (OK);
}


/*******************************************************************************/
/* Fonction command_AW.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater la paire CVK sous ZMK vers une paire   */
/* sous LMK                                                      */
/*******************************************************************************/
int 
	command_AW(char *buffer_transmit,
	char *zmk_lmk,
	char *cvka_zmk,
	char *cvkb_zmk
	)
{
	trace_event("Start command_AW()", PROCESSING);

	memcpy(buffer_transmit, "AW", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk_lmk, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, cvka_zmk, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, cvkb_zmk, SIZE_KEY);

	trace_event("End   command_AW()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_AX.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM la paire CVK sous LMK, la Check value de chaque cle de   */
/* la paire et le code d'erreur.                                */
/******************************************************************************/
int 
	command_AX(char *buffer_received,
	char *cvka_lmk,
	char *cvkb_lmk,
	char *check_valuea,
	char *check_valueb,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_AX()", PROCESSING);

	memset(Command, 0, 3);

	memset(cvka_lmk, 0, SIZE_KEY);
	memset(cvkb_lmk, 0, SIZE_KEY);
	memset(check_valuea, 0, SIZE_KEY);
	memset(check_valueb, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "AX", 2) != 0)
		return (ERROR_COMMAND);

	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0)
		return (CHECK_ERROR);
	buffer_received += 4;

	memcpy(cvka_lmk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(cvkb_lmk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(check_valuea, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(check_valueb, buffer_received, SIZE_KEY);

	trace_event("End   command_AX()", PROCESSING);
	return (OK);
}

/*******************************************************************************/
/* Fonction command_FE.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater PVK, TPK ou TMK encryptee sous LMK     */
/* vers ZMK                                                      */
/*******************************************************************************/
int 
	command_FE(char *buffer_transmit,
	char *zmk_lmk,
	char *key_lmk
	)
{
	trace_event("Start command_FE()", PROCESSING);

	memcpy(buffer_transmit, "FE", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));
	buffer_transmit += strlen(zmk_lmk);
	memcpy(buffer_transmit, key_lmk, strlen(key_lmk));
	buffer_transmit += strlen(key_lmk);

	trace_event("End   command_FE()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_FF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM TMK, TPK ou PVK sous ZMK, la Check_value ainsi que le    */
/* code d'erreur. 				                              */
/******************************************************************************/
int 
	command_FF(char *buffer_received,
	char *key_zmk,
	char *check_value,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_FF()", PROCESSING);

	memset(Command, 0, 3);
	memset(check_value, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "FF", 2) != 0) {
		trace_event("End   command_FF(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_FF(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(key_zmk, buffer_received, 2 * SIZE_KEY);
		buffer_received += (2 * SIZE_KEY);
		memcpy(check_value, buffer_received, SIZE_KEY);
		key_zmk[2 * SIZE_KEY] = '\0';
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(key_zmk, buffer_received, 3 * SIZE_KEY);
		buffer_received += (3 * SIZE_KEY);
		memcpy(check_value, buffer_received, SIZE_KEY);
		key_zmk[3 * SIZE_KEY] = '\0';
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(key_zmk, buffer_received, SIZE_KEY);
		buffer_received += SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
		key_zmk[SIZE_KEY] = '\0';
	}

	trace_event("End   command_FF(OK)", PROCESSING);
	return (OK);
}


/*******************************************************************************/
/* Fonction command_FM.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater ZAK encryptee sous LMK vers ZMK        */
/*******************************************************************************/
int 
	command_FM(char *buffer_transmit,
	char *zmk_lmk,
	char *zak_lmk
	)
{
	trace_event("Start command_FM()", PROCESSING);

	memcpy(buffer_transmit, "FM", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, "1", 1);
	buffer_transmit += 1;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));
	buffer_transmit += strlen(zmk_lmk);
	memcpy(buffer_transmit, zak_lmk, strlen(zak_lmk));

	trace_event("End   command_FM()", PROCESSING);

	return (OK);
}

/******************************************************************************/
/* Fonction command_FN.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM ZAK sous ZMK et le code d'erreur.                        */
/******************************************************************************/
int 
	command_FN(char *buffer_received,
	char *zak_zmk,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_FN()", PROCESSING);

	memset(Command, 0, 3);
	memset(zak_zmk, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "FN", 2) != 0) {
		trace_event("End   command_FN(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_FN(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(zak_zmk, buffer_received, 2 * SIZE_KEY);
		buffer_received += (2 * SIZE_KEY);
		zak_zmk[2 * SIZE_KEY] = '\0';
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(zak_zmk, buffer_received, 3 * SIZE_KEY);
		buffer_received += (3 * SIZE_KEY);
		zak_zmk[3 * SIZE_KEY] = '\0';
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(zak_zmk, buffer_received, SIZE_KEY);
		buffer_received += SIZE_KEY;
		zak_zmk[SIZE_KEY] = '\0';
	}

	return (OK);
}

/*******************************************************************************/
/* Fonction command_GC.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater ZPK encryptee sous LMK vers ZPK        */
/* encrypte sous ZMK    						                   */
/*******************************************************************************/
int 
	command_GC(char *buffer_transmit,
	char *zmk_lmk,
	char *zpk_lmk
	)
{
	trace_event("Start command_GC()", PROCESSING);

	memcpy(buffer_transmit, "GC", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));
	buffer_transmit += strlen(zmk_lmk);
	memcpy(buffer_transmit, zpk_lmk, strlen(zpk_lmk));
	buffer_transmit += strlen(zpk_lmk);

	trace_event("End   command_GC()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_GD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM ZPK sous ZMK, la Check_value ainsi que le code d'erreur. */
/******************************************************************************/
int 
	command_GD(char *buffer_received,
	char *zpk_zmk,
	char *check_value,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_GD()", PROCESSING);

	memset(Command, 0, 3);
	memset(zpk_zmk, 0, SIZE_KEY);
	memset(check_value, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "GD", 2) != 0) {
		trace_event("End   command_GD(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_GD(ERROR_COMMAND)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;
	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(zpk_zmk, buffer_received, 2 * SIZE_KEY);
		zpk_zmk[2 * SIZE_KEY] = '\0';
		buffer_received += (2 * SIZE_KEY);
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(zpk_zmk, buffer_received, 3 * SIZE_KEY);
		zpk_zmk[3 * SIZE_KEY] = '\0';
		buffer_received += (3 * SIZE_KEY);
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(zpk_zmk, buffer_received, SIZE_KEY);
		zpk_zmk[SIZE_KEY] = '\0';
		buffer_received += SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
	}

	trace_event("End   command_GD(OK)", PROCESSING);
	return (OK);
}

/*******************************************************************************/
/* Fonction command_GG.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour generer la ZMK a partir des 3 composantes ZMK1,   */
/* ZMK2, ZMK3. 							                       */
/******************************************************************************/
int 
	command_GG(char *buffer_transmit,
	char *zmk1_lmk,
	char *zmk2_lmk,
	char *zmk3_lmk
	)
{
	trace_event("Start command_GG()", PROCESSING);

	memcpy(buffer_transmit, "GG", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk1_lmk, strlen(zmk1_lmk));
	buffer_transmit += strlen(zmk1_lmk);
	memcpy(buffer_transmit, zmk2_lmk, strlen(zmk2_lmk));
	buffer_transmit += strlen(zmk2_lmk);
	memcpy(buffer_transmit, zmk3_lmk, strlen(zmk3_lmk));
	buffer_transmit += strlen(zmk3_lmk);

	trace_event("End   command_GG()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_GH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM la ZMK sous LMK ainsi que le code d'erreur.              */
/******************************************************************************/
int 
	command_GH(char *buffer_received,
	char *zmk_lmk,
	char *check_value,
	char *err_code
	)
{


	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_GH()", PROCESSING);

	memset(Command, 0, 3);
	memset(zmk_lmk, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "GH", 2) != 0) {

		trace_event("End   command_GH(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_GH(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(zmk_lmk, buffer_received, 2 * SIZE_KEY);
		zmk_lmk[2 * SIZE_KEY] = '\0';
		buffer_received += (2 * SIZE_KEY);
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(zmk_lmk, buffer_received, 3 * SIZE_KEY);
		zmk_lmk[3 * SIZE_KEY] = '\0';
		buffer_received += (3 * SIZE_KEY);
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);

		if (strlen(buffer_received) > (2 * SIZE_KEY)) {	/* ZMK used as double
														* length */
			memcpy(zmk_lmk, buffer_received, 2 * SIZE_KEY);
			zmk_lmk[2 * SIZE_KEY] = '\0';
			buffer_received += (2 * SIZE_KEY);
			memcpy(check_value, buffer_received, SIZE_KEY);
		} else {
			memcpy(zmk_lmk, buffer_received, SIZE_KEY);
			zmk_lmk[SIZE_KEY] = '\0';
			buffer_received += SIZE_KEY;
			memcpy(check_value, buffer_received, SIZE_KEY);
		}
	}

	trace_event("End   command_GH(OK)", PROCESSING);
	return (OK);
}



/*******************************************************************************/
/* Fonction command_OE.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour generer la TMK et la TPK d'une maniere aleatoire  */
/*******************************************************************************/
int 
	command_OE(char *buffer_transmit)
{

	trace_event("Start command_OE()", PROCESSING);

	memcpy(buffer_transmit, "OE", 2);
	buffer_transmit += 2;

	trace_event("End   command_OE()", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_OF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM la TMK ou la TPK sous LMK, la Check_value ainsi que      */
/* le code d'erreur.                                            */
/******************************************************************************/
int 
	command_OF(char *buffer_received,
	char *key_lmk,
	char *check_value,
	char *err_code)
{


	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_OF()", PROCESSING);

	memset(Command, 0, 3);
	memset(key_lmk, 0, SIZE_KEY);
	memset(check_value, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "OE", 2) != 0) {
		trace_event("End   command_OF(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_OF(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;
	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(key_lmk, buffer_received, 2 * SIZE_KEY);
		key_lmk[2 * SIZE_KEY] = '\0';
		buffer_received += (2 * SIZE_KEY);
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(key_lmk, buffer_received, 3 * SIZE_KEY);
		key_lmk[3 * SIZE_KEY] = '\0';
		buffer_received += (3 * SIZE_KEY);
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(key_lmk, buffer_received, SIZE_KEY);
		key_lmk[SIZE_KEY] = '\0';
		buffer_received += SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
	}


	trace_event("End   command_OF(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_HE                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM le pin block encrypte sous une tak             .         */
/******************************************************************************/
int 
	command_HE(char *buffer_transmit,
	char *cle,
	char *data
	)
{

	trace_event("Start command_HE()", PROCESSING);

	memcpy(buffer_transmit, "HE", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, cle, strlen(cle));
	buffer_transmit += strlen(cle);
	memcpy(buffer_transmit, data, SIZE_KEY);

	trace_event("End   command_HE()", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_HF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM le pin block encrypte sous une tak             .         */
/******************************************************************************/
int 
	command_HF(char *buffer_received,
	char *pin,
	char *err_code
	)
{


	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_HF()", PROCESSING);

	memset(Command, 0, 3);
	memset(pin, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "HF", 2) != 0) {
		trace_event("End   command_HF(ERROR_COMMND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_HF(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	memcpy(pin, buffer_received, SIZE_KEY);
	trace_event("End   command_HF(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_HG                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM le pin block decrypte sous une tak             .         */
/******************************************************************************/
int 
	command_HG(char *buffer_transmit,
	char *cle,
	char *data
	)
{
	trace_event("Start command_HG()", PROCESSING);

	memcpy(buffer_transmit, "HG", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, cle, strlen(cle));
	buffer_transmit += strlen(cle);
	memcpy(buffer_transmit, data, SIZE_KEY);

	trace_event("End   command_HG()", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_HH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM le pin block decrypt sous une tak             .         */
/******************************************************************************/
int 
	command_HH(char *buffer_received,
	char *pin,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_HH()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "HH", 2) != 0) {
		trace_event("Start command_HH(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("Start command_HH(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	memcpy(pin, buffer_received, SIZE_KEY);

	trace_event("End   command_HH(OK)", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_HC                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de generer randomly une cle et     */
/* l'encrypter sous LMK paire 14,15                             */
/******************************************************************************/
/*
* int  command_HC( char *buffer_transmit, char *key ) { trace_event("Start
* command_HC()", PROCESSING); 
*
* memcpy(buffer_transmit, "HC", 2);         buffer_transmit += 2;
* memcpy(buffer_transmit , key, strlen(key));  buffer_transmit +=
* strlen(key); 
*
* trace_event("End   command_HC(OK)", PROCESSING); return(OK); } 
*/
/*
* int  command_HC( char *buffer_transmit, char *key ) { trace_event("Start
* command_HC()", PROCESSING); 
*
* memcpy(buffer_transmit, "HCU", 3); buffer_transmit += 3;
* memcpy(buffer_transmit , key, strlen(key)); buffer_transmit +=
* strlen(key); 
*
* trace_event("End   command_HC(OK)", PROCESSING); return(OK); } 
*/

int 
	command_HC(char *buffer_transmit,
	char *key
	)
{
	char            sLine[MAX_LINE_TRC];
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(sLine, "Start command_HC(%s)", key);
	else
		sprintf(sLine, "Start command_HC()");
	trace_event(sLine, PROCESSING);	

	memcpy(buffer_transmit, "HC", 2);
	buffer_transmit += 2;
	if (strlen(key) > 16) {
		/* NBD28112008 Start
		emcpy(buffer_transmit, "U", 1);
		buffer_transmit += 1;
		NBD28112008 End */
		memcpy(buffer_transmit, key, strlen(key));
		buffer_transmit += strlen(key);
		memcpy(buffer_transmit, ";XU0", 4);
		buffer_transmit += 4;
	} else {
		memcpy(buffer_transmit, key, strlen(key));
		buffer_transmit += strlen(key);
	}


	trace_event("End   command_HC(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_HD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer la cle sous LMK       */
/******************************************************************************/
int 
	command_HD(char *buffer_received,
	char *key_key,
	char *key_lmk,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_HD()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "HD", 2) != 0) {
		trace_event("End   command_HD(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_HD(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;
	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(key_key, buffer_received, (2 * SIZE_KEY) + 1);
		key_key[(2 * SIZE_KEY) + 1] = '\0';
		buffer_received += (2 * SIZE_KEY);
		buffer_received += 1;
		memcpy(key_lmk, buffer_received, (2 * SIZE_KEY) + 1);
		key_lmk[(2 * SIZE_KEY) + 1] = '\0';
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(key_key, buffer_received, (3 * SIZE_KEY) + 1);
		key_key[(3 * SIZE_KEY) + 1] = '\0';
		buffer_received += 1;
		buffer_received += (3 * SIZE_KEY);
		memcpy(key_lmk, buffer_received, (3 * SIZE_KEY) + 1);
		key_lmk[(3 * SIZE_KEY) + 1] = '\0';
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(key_key, buffer_received, SIZE_KEY);
		key_key[SIZE_KEY] = '\0';
		buffer_received += SIZE_KEY;
		memcpy(key_lmk, buffer_received, SIZE_KEY);
		key_lmk[SIZE_KEY] = '\0';
	}


	trace_event("End   command_HD(OK)", PROCESSING);
	return (OK);
}

/*******************************************************************************/
/* Fonction command_FG.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour generer la paire PVK sous LMK                     */
/*******************************************************************************/
int 
	command_FG(char *buffer_transmit,
	char *zmk
	)
{
	trace_event("Debut command_FG()", PROCESSING);

	memcpy(buffer_transmit, "FG", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk, strlen(zmk));

	trace_event("Fin   command_FG(OK)", PROCESSING);

	return (OK);
}

/******************************************************************************/
/* Fonction command_FH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM la paire PVK sous LMK, la Check_value ainsi que          */
/* le code d'erreur.			                                  */
/******************************************************************************/
int 
	command_FH(char *buffer_received,
	char *pvka_lmk,
	char *pvkb_lmk,
	char *pvka_zmk,
	char *pvkb_zmk,
	char *check_valuea,
	char *check_valueb,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_FH()", PROCESSING);

	memset(Command, 0, 3);
	memset(pvka_lmk, 0, SIZE_KEY);
	memset(pvkb_lmk, 0, SIZE_KEY);
	memset(pvka_zmk, 0, SIZE_KEY);
	memset(pvkb_zmk, 0, SIZE_KEY);
	memset(check_valuea, 0, SIZE_KEY);
	memset(check_valueb, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "FH", 2) != 0) {
		trace_event("End   command_FH(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_FH(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	memcpy(pvka_lmk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(pvkb_lmk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(pvka_zmk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(pvkb_zmk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(check_valuea, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;
	memcpy(check_valueb, buffer_received, SIZE_KEY);

	trace_event("End   command_FH(OK)", PROCESSING);
	return (OK);
}

/*******************************************************************************/
/* Fonction command_MI.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater TAK encryptee sous ZMK vers LMK        */
/*******************************************************************************/
int 
	command_MI(char *buffer_transmit,
	char *zmk,
	char *tak_zmk
	)
{
	trace_event("Start command_MI()", PROCESSING);

	memcpy(buffer_transmit, "MI", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk, strlen(zmk));
	buffer_transmit += strlen(zmk);
	if ( strlen( tak_zmk ) != SIZE_KEY) /* ICH For HSID */
	{
		memcpy(buffer_transmit,"X" ,1 );
		buffer_transmit +=1;
	}
	memcpy(buffer_transmit, tak_zmk, strlen(tak_zmk));

	trace_event("End   command_MI(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_MJ.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM TAK sous LMK                                             */
/******************************************************************************/
int 
	command_MJ(char *buffer_received,
	char *tak_lmk,
	char *check_value,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_MJ()", PROCESSING);

	memset(Command, 0, 3);
	memset(tak_lmk, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "MJ", 2) != 0) {
		trace_event("End   command_MJ(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_MJ(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;
	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(tak_lmk, buffer_received, 2 * SIZE_KEY + 1) ;
		buffer_received += (2 * SIZE_KEY) + 1;
		memcpy(check_value, buffer_received, SIZE_KEY);
		tak_lmk[2 * SIZE_KEY + 1] = '\0';
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(tak_lmk, buffer_received, 3 * SIZE_KEY);
		buffer_received += (3 * SIZE_KEY);
		memcpy(check_value, buffer_received, 3 * SIZE_KEY);
		tak_lmk[3 * SIZE_KEY] = '\0';
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(tak_lmk, buffer_received, SIZE_KEY);
		buffer_received += SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
		tak_lmk[SIZE_KEY] = '\0';
	}

	trace_event("End   command_MJ(OK)", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_BA.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but d'encrypter un PIN clair sous LMK  */
/******************************************************************************/
int 
	command_BA(char *buffer_transmit,
	char *pin,
	char *Pan
	)
{
	char            card[13];
	memset(card, 0, 13);

	trace_event("Start command_BA()", PROCESSING);

	memcpy(buffer_transmit, "BA", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, pin, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	format_card_from_pan(card, Pan);
	memcpy(buffer_transmit, card, 12);

	trace_event("End   command_BA(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_BB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM PIN sous LMK                                             */
/******************************************************************************/
int 
	command_BB(char *buffer_received,
	char *pin_lmk,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_BB()", PROCESSING);

	memset(Command, 0, 3);
	memset(pin_lmk, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "BB", 2) != 0) {
		trace_event("End   command_BB(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_BB(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	memcpy(pin_lmk, buffer_received, SIZE_KEY);

	trace_event("End   command_BB(OK)", PROCESSING);
	return (OK);
}

/***********************************************************************
**  nom   : command_MW                                                **
**  desc  : demande de verification du  mac d'un messsage             **
**  entree: tak_key   : cle d authentication du message               **
**          mac_data  : donnees du MAC                                **
**  sortie: Buffer-> Message a envoyer  au HSM                        **
**  retour: OK    -> fonction aboutie avec succes                     **
**          NOK   -> fonction aboutie avec echec                      **
**          ERROR -> fonction non aboutie                             **
**  auteur: E. ENNOUINI (06 Fev 2002)                                 **
***********************************************************************/
int command_MW(
	unsigned char   Buffer[],
	char            mode,
	unsigned char   tak_key[],
	unsigned char   sVect[],
	unsigned char   mac_data[],
	unsigned char   message_mac[],
	int             lg_message_mac)
{

	int             offset = 0;
	char            ligne[MAX_LINE_TRC];

	trace_event("Start command_MW()", PROCESSING);

	memcpy(Buffer + offset, "MW", 2);
	offset += 2;

	Buffer[offset++] = mode;

	memcpy(Buffer + offset, tak_key, strlen(tak_key));
	offset += strlen(tak_key);

	memcpy(Buffer + offset, mac_data, 8);
	offset += 8;

	sprintf(ligne, "Inside command_MW the length is %d", lg_message_mac);
	trace_event(ligne, PROCESSING);

	sprintf(Buffer + offset, "%03X", lg_message_mac);
	offset += 3;

	memcpy(Buffer + offset, message_mac, lg_message_mac);
	offset += lg_message_mac;

	/** necessaire ??? Buffer[offset]=0;*/
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "End command_MW(%02d)%s", offset, Buffer);
	else
		sprintf(ligne, "End command_MW(OK)");
	trace_event(ligne, PROCESSING);	

	return (offset);
}

/***********************************************************************
**  nom   : command_MX                                                **
**  desc  : recuperation du reponse du HSM  pour une demande de       ** 
**          verification du MAC                                       **
**  entree: Buffer_received-> le buffer recu                          **
**  sortie: error_code    -> code erreur de la commande               **
**  retour: OK    -> fonction aboutie avec succes                     **
**          NOK   -> fonction aboutie avec echec                      **
**  auteur: E. ENNOUINI (07 Fev 2002)                                 **
***********************************************************************/
int command_MX(unsigned char   Buffer_received[],unsigned char   error_code[])
{


	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_MX()", PROCESSING);

	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
	{
		sprintf(sLine, "Buffer_received=[%s]", Buffer_received);
		trace_event(sLine, PROCESSING);
	}
	memset(Command, 0, sizeof(Command));
	memcpy(Command, Buffer_received, 2);
	/** END   MAW20030328 **/

	memcpy(error_code, Buffer_received + 2, 2);

	if (memcmp(Command, "MX", 2) != 0)
		return (ERROR_COMMAND);

	/** if (memcmp(error_code + 2, "00", 2) == 0) return ( OK ); **/
	if (memcmp(error_code, "00", 2) == 0)
	{
		trace_event("End   command_MX(OK)", PROCESSING);
		return (OK);	/** MAW20030328 **/
	}

	/** if (memcmp(error_code + 2, "01", 2) == 0) return ( VERIFICATION_FAILED ); **/
	if (memcmp(error_code, "01", 2) == 0)
		return (VERIFICATION_FAILED);	/** MAW20030328 **/

	trace_event("End   command_MX(ERROR)", PROCESSING);

	return (CRYPTOGRAPHIC_ERROR);
}


/*****************************************************************************\
* TA - Verify an Interchange PIN Using the VISA Method
\*****************************************************************************/
int command_TA(char *buffer_transmit,
	char *zpkLmk,
	char *pvka,
	char *pvkb,
	char *pin_block,
	char *format,
	char *pan,
	char *pvv
	)
{
	char            card[13];
	char            pvki[2];

	trace_event("Start command_TA()", PROCESSING);

	memset(card, 0, sizeof(card));
	memset(pvki, 0, sizeof(pvki));
	memcpy(pvki, pvv, 1);

	memcpy(buffer_transmit, "TA", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zpkLmk, strlen(zpkLmk));
	buffer_transmit += strlen(zpkLmk);
	memcpy(buffer_transmit, pvka, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, pvkb, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, pin_block, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, format, 2);
	buffer_transmit += 2;
	format_card_from_pan(card, pan);
	memcpy(buffer_transmit, card, 12);
	buffer_transmit += 12;
	memcpy(buffer_transmit, pvki, 1);
	buffer_transmit += 1;
	memcpy(buffer_transmit, pvv + 1, 4);
	buffer_transmit += 4;

	trace_event("Fin   command_TA(OK)", PROCESSING);
	return (OK);
}

int 
	command_TB(char *buffer_received,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_TB()", PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "TB", 2) != 0) {
		trace_event("End   command_TB(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) == 0) {
		trace_event("End   command_TB(OK)", PROCESSING);
		return (OK);
	}
	if (memcmp(err_code + 2, "01", 2) == 0) {
		trace_event("End   command_TB(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}
	trace_event("End   command_TB(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/* Fonction command_MG.                                                       */
/* -------------------                                                        */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater TAK encryptee sous LMK vers ZMK         */
/******************************************************************************/
int 
	command_MG(char *buffer_transmit,
	char *zmk,
	char *tak_lmk
	)
{
	trace_event("Start command_MG()", PROCESSING);

	memcpy(buffer_transmit, "MG", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk, strlen(zmk));
	buffer_transmit += strlen(zmk);
	memcpy(buffer_transmit, tak_lmk, strlen(tak_lmk));

	trace_event("End   command_MG(OK)", PROCESSING);
	return (OK);
}

/***********************************************************/
int 
	command_MG_TDES(char *buffer_transmit,
	char *zmk,
	char *tak_lmk
	)
{
	trace_event("Start command_MG_TDES()", PROCESSING);

	memcpy(buffer_transmit, "MG", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk, strlen(zmk));
	buffer_transmit += strlen(zmk);
	memcpy(buffer_transmit, tak_lmk, strlen(tak_lmk));
	buffer_transmit += strlen(tak_lmk);
	/* NAB16112011
	memcpy(buffer_transmit, ";XU0", 4);  */
	memcpy(buffer_transmit, ";X00", 4);
	buffer_transmit += 4;

	trace_event("End   command_MG_TDES(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_MH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM TAK sous ZMK                                             */
/******************************************************************************/
int 
	command_MH(char *buffer_received,
	char *tak_zmk,
	char *check_value,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_MH()", PROCESSING);

	memset(Command, 0, 3);
	memset(tak_zmk, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "MH", 2) != 0) {
		trace_event("End   command_MH(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_MH(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;
	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(tak_zmk, buffer_received, 2 * SIZE_KEY+1);
		buffer_received += (2 * SIZE_KEY+1);
		memcpy(check_value, buffer_received, 2 * SIZE_KEY);
		tak_zmk[2 * SIZE_KEY+1] = '\0';
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(tak_zmk, buffer_received, 3 * SIZE_KEY);
		buffer_received += (3 * SIZE_KEY);
		memcpy(check_value, buffer_received, 3 * SIZE_KEY);
		tak_zmk[3 * SIZE_KEY] = '\0';
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(tak_zmk, buffer_received, SIZE_KEY);
		buffer_received += SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
		tak_zmk[SIZE_KEY] = '\0';
	}


	trace_event("End   command_MH(OK)", PROCESSING);
	return (OK);
}

/***********************************************************************
**  nom   : command_MU                                                **
**  desc  : demande de generation du MAC d'un message                 **
**  entree: tak_key    : cle d authentication du message              **
**          message_mac  :  message to be MACed                       **
**          lg_message_mac  :  length of the  message to be MACed     **
**  sortie: Buffer-> a envoyer au module de securite                  **
**  retour: OK    -> fonction aboutie avec succes                     **
**          NOK   -> fonction aboutie avec echec                      **
**          ERROR -> fonction non aboutie                             **
**  auteur: M. ENNOUINI (07 Avr 2002)                                 **
***********************************************************************/
int 
	command_MU_DES(Buffer, mode, tak_key, sVect, message_mac, lg_message_mac)
	unsigned char   Buffer[];
char            mode;
unsigned char   tak_key[];
unsigned char   sVect[];
unsigned char   message_mac[];
int             lg_message_mac;
{

	int             offset = 0;
	char            ligne[MAX_LINE_TRC];

	trace_event("Start command_MU()", PROCESSING);

	memcpy(Buffer + offset, "MU", 2);
	offset += 2;

	Buffer[offset++] = mode;

	memcpy(Buffer + offset, tak_key, SIZE_KEY);
	offset += SIZE_KEY;

	if (mode != '0' && mode != '1') {
		memcpy(Buffer + offset, sVect, SIZE_KEY); 
		offset += SIZE_KEY;
	}
	sprintf(Buffer + offset, "%03X", lg_message_mac);
	offset += 3;

	memcpy(Buffer + offset, message_mac, lg_message_mac);
	offset += lg_message_mac;

	sprintf(ligne, "End command_MU(%02d)", offset);
	trace_event(ligne, PROCESSING);
	return (offset);
}
/****************************************************************************/
int 
	command_MU(Buffer, mode, tak_key, sVect, message_mac, lg_message_mac)
	unsigned char   Buffer[];
char            mode;
unsigned char   tak_key[];
unsigned char   sVect[];
unsigned char   message_mac[];
int             lg_message_mac;
{

	int             offset = 0;
	char            ligne[MAX_LINE_TRC];

	trace_event("Start command_MU()", PROCESSING);

	memcpy(Buffer + offset, "MU", 2);
	offset += 2;

	Buffer[offset++] = mode;
	memcpy(Buffer + offset, tak_key, SIZE_KEY * 2 );
	offset += SIZE_KEY * 2 ;
	if (mode != '0' && mode != '1') {
		memcpy(Buffer + offset, sVect, SIZE_KEY);
		offset += SIZE_KEY;
	}
	sprintf(Buffer + offset, "%03X", lg_message_mac);
	offset += 3;

	memcpy(Buffer + offset, message_mac, lg_message_mac);
	offset += lg_message_mac;

	sprintf(ligne, "End command_MU(%02d)", offset);
	trace_event(ligne, PROCESSING);
	return (offset);
}
/***********************************************************************
**  nom   : command_MV                                                **
**  desc  : extraction des champs du buffer recu du module de securite**
**  entree: Buffer_received-> le buffer recu                          **
**  sortie: error_code    -> code erreur de la commande               **
**  retour: OK    -> fonction aboutie avec succes                     **
**          NOK   -> fonction aboutie avec echec                      **
**          ERROR -> fonction non aboutie                             **
**  auteur: M. ENNOUINI (06 Avr 2001)                                 **
***********************************************************************/
command_MV(Buffer_received, error_code, mac_data)
	unsigned char   Buffer_received[];
unsigned char   error_code[];
unsigned char   mac_data[];
{

	char            Command[3];
	int             offset = 2;

	trace_event("Start command_MV()", PROCESSING);

	memcpy(Command, Buffer_received, 2);

	memcpy(error_code, Buffer_received + offset, 2);
	offset += 2;

	if (memcmp(Command, "MV", 2) != 0)
		return (ERROR_COMMAND);

	if (memcmp(error_code, "00", 2) != 0)
		return (CHECK_ERROR);
	else
		/* memcpy(mac_data,Buffer_received + offset ,8); */
		memcpy(mac_data, Buffer_received + offset, strlen(Buffer_received + offset));

	trace_event("End   command_MV(OK)", PROCESSING);

	return (OK);
}
int 
	command_KU_OLD(char *sHsmBuffer,
	char cModeFlag,
	char cSchemeId,
	char *sSMI,
	char *sPanAndSeq,
	char *sIntegSessData,
	int nPlainTextLen,
	char *sPlainTextData,
	char *sSMC,
	char *sTK,
	char *sConfSessData,
	int nOffset,
	int nCipherTextLen,
	char *sCipherTextData
	)
{

	int             offset = 0;
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_KU_OLD()", PROCESSING);

	trace_event("KU", PROCESSING);
	memcpy(sHsmBuffer + offset, "KU", 2);
	offset += 2;

	sHsmBuffer[offset] = cModeFlag;

	offset += 1;

	sHsmBuffer[offset] = cSchemeId;
	offset += 1;
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
	{
		sprintf(sLine, "==> sSMI=[%s]", sSMI);
		trace_event(sLine, FATAL);
	}
	memcpy(sHsmBuffer + offset, sSMI, strlen(sSMI));
	offset += strlen(sSMI);

	memcpy(sHsmBuffer + offset, sPanAndSeq, 8);
	offset += 8;

	memcpy(sHsmBuffer + offset, sIntegSessData, 8);
	offset += 8;

	sprintf(sHsmBuffer + offset, "%04X", nPlainTextLen);
	offset += 4;

	memcpy(sHsmBuffer + offset, sPlainTextData, nPlainTextLen);
	offset += nPlainTextLen;

	memcpy(sHsmBuffer + offset, ";", 1);
	offset += 1;

	if (cModeFlag != '0') {
		memcpy(sHsmBuffer + offset, sSMC, strlen(sSMC));
		offset += strlen(sSMC);

		memcpy(sHsmBuffer + offset, sTK, strlen(sTK));
		offset += strlen(sSMC);

		memcpy(sHsmBuffer + offset, sConfSessData, 8);
		offset += 8;

		sprintf(sHsmBuffer + offset, "%04X", nOffset);
		offset += 4;

		sprintf(sHsmBuffer + offset, "%04X", nCipherTextLen);
		offset += 2;

		memcpy(sHsmBuffer + offset, sCipherTextData, nCipherTextLen);
		offset += nCipherTextLen;

	}
	sprintf(sLine, "End command_KU_OLD(%d)", offset);
	trace_event(sLine, PROCESSING);
	return (offset);
}
/******************************************************************************/
/* Fonction command_KV_OLD.                                                       */
/* -------------------                                                        */
/******************************************************************************/
int 
	command_KV_OLD(char *sBuffRcv,
	char cModeFlag,
	char *sErrCode,
	char *sMAC,
	int *nSecureMsgLen,
	char *sSecureMsgData
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];
	char            sLine2[MAX_LINE_TRC];
	char            sTmpData[LG_MAX];

	trace_event("Start command_KV_OLD()", PROCESSING);

	memset(Command, 0, sizeof(Command));
	memset(sTmpData, 0, sizeof(sTmpData));

	memcpy(Command, sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp(Command, "KV", 2) != 0) {
		trace_event("End   command_KV_OLD(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(sErrCode + 2, sLine);

	/***/

	sprintf(sLine2, "sErrCode=[%s]. sLine=[%s]", sErrCode, sLine);
	trace_event(sLine2, PROCESSING);

	/****/

	if (memcmp(sErrCode + 2, "00", 2) == 0) {
		memcpy(sMAC, sBuffRcv + 4, 8);

		if ('0' != cModeFlag) {
			memcpy(sTmpData, sBuffRcv + 4 + 8, 4);
			*nSecureMsgLen = atoi(sTmpData);
			memcpy(sSecureMsgData, sBuffRcv + 4 + 8 + 4, (*nSecureMsgLen));
		}
		trace_event("End   command_KV_OLD(OK)", PROCESSING);
		return (OK);
	}
	trace_event("End   command_KV_OLD(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);
}
/* End Mehdi Elyajizi EMV issuer SCRIPT */

/*Translate a PIN from *BDK Encryption to Interchange Key Encryption*/
int command_CI  (  char *sHsmBuffer,
	char *sBdk,
	char *sZpk,
	char *sKsnDesc,
	char *sKsn,
	char *sPinBlock,
	char *sFormatOut,
	char *sPan
	)
{

	int     offset = 0;
	char    sLine[MAX_LINE_TRC];
	char    sCard[13];

	memset(sLine, 0, sizeof(sLine));
	memset(sCard, 0, sizeof(sCard));

	trace_event("Start command_CI()", PROCESSING);

	memcpy(sHsmBuffer + offset, "CI",   2);
	offset += 2;   

	memcpy(sHsmBuffer + offset, sBdk,   strlen(sBdk));
	offset += strlen(sBdk);

	memcpy(sHsmBuffer + offset, sZpk,   strlen(sZpk));
	offset += strlen(sZpk);

	memcpy(sHsmBuffer + offset, sKsnDesc,   strlen(sKsnDesc));
	offset += strlen(sKsnDesc);

	memcpy(sHsmBuffer + offset, sKsn,   strlen(sKsn));
	offset += strlen(sKsn);

	memcpy(sHsmBuffer + offset, sPinBlock,   strlen(sPinBlock));
	offset += strlen(sPinBlock);  

	memcpy(sHsmBuffer + offset, sFormatOut,   strlen(sFormatOut));
	offset += strlen(sFormatOut);  

	format_card_from_pan ( sCard , sPan );
	memcpy(sHsmBuffer, sCard ,12); 

	sprintf(sLine, "End command_CI(%d)", offset);
	trace_event(sLine,  PROCESSING);
	return (offset);
}

int    command_CJ( char *sBuffRcv,
	char *sErrCode,
	char *sPinLen,
	char *sPinBlock,
	char *sFormatCode
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];

	trace_event("Start command_CJ()",PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command , sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp( Command , "CJ", 2) != 0)
	{
		trace_event("End   command_CJ(ERROR_COMMAND)",PROCESSING);
		return ( ERROR_COMMAND );
	}

	TextError( sErrCode + 2,sLine);
	if (memcmp(sErrCode + 2, "00", 2) == 0)
	{
		memcpy(sPinLen,     sBuffRcv + 4, 2);
		memcpy(sPinBlock,   sBuffRcv + 6, 16);
		memcpy(sFormatCode, sBuffRcv + 22, 2);

		trace_event("End   command_CJ(OK)",PROCESSING);
		return ( OK );
	}
	else
	{

		trace_event("End   command_CJ(VERIFICATION_FAILED)",PROCESSING);
		return ( VERIFICATION_FAILED );
	}
	trace_event("End   command_CJ(CRYPTOGRAPHIC_ERROR)",PROCESSING);
	return(CRYPTOGRAPHIC_ERROR);
}


/*generate a BDK/LMK key */
int command_BI  (  char *sHsmBuffer    )
{

	int     offset = 0;
	char    sLine[MAX_LINE_TRC];    

	memset(sLine, 0, sizeof(sLine));


	trace_event("Start command_BI()", PROCESSING);

	memcpy(sHsmBuffer + offset, "BI",   2);
	offset += 2; 

	memcpy(sHsmBuffer , ";XU0", 4);  
	sHsmBuffer += 4;   

	sprintf(sLine, "End command_BI(%d)", offset);
	trace_event(sLine,  PROCESSING);
	return (offset);
}

int    command_BJ( char *sBuffRcv,
	char *sErrCode,
	char *sBdkLmk                  
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];


	memset(Command, 0, sizeof(Command));
	memset(sLine,   0, sizeof(sLine));

	trace_event("Start command_BJ()",PROCESSING);



	memcpy(Command , sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp( Command , "BJ", 2) != 0)
	{
		trace_event("End   command_BJ(ERROR_COMMAND)",PROCESSING);
		return ( ERROR_COMMAND );
	}

	TextError( sErrCode + 2,sLine);
	if (memcmp(sErrCode + 2, "00", 2) == 0)
	{

		switch (sBuffRcv[4])
		{
		case 'U' :
		case 'X' :					
			trace_event("Triple DES Encryption Double Length",TRACE);
			memcpy(sBdkLmk, sBuffRcv+4, 2 * SIZE_KEY +1); 
			sBdkLmk [2 * SIZE_KEY+1] = '\0';			   		
			break;
		case 'T' :
		case 'Y' :
			trace_event("Triple DES Encryption Triple Length",TRACE);
			memcpy(sBdkLmk, sBuffRcv+4, 3 * SIZE_KEY +1); 
			sBdkLmk [3 * SIZE_KEY] = '\0';

			break;
		default  : 
			trace_event("Single DES Encryption Single Length",TRACE);
			memcpy(sBdkLmk, sBuffRcv+4, SIZE_KEY);
			sBdkLmk [SIZE_KEY] = '\0';			   	
		}           
		trace_event("End   command_BJ(OK)",PROCESSING);
		return ( OK );
	}
	if (memcmp(sErrCode + 2, "01", 2) == 0)
	{
		trace_event("End   command_BJ(VERIFICATION_FAILED)",PROCESSING);
		return ( VERIFICATION_FAILED );
	}
	trace_event("End   command_BJ(CRYPTOGRAPHIC_ERROR)",PROCESSING);
	return(CRYPTOGRAPHIC_ERROR);
}


/*translate a BDK/LMK key to BDK/ZMK*/
int command_DY  (  char *sHsmBuffer,
	char *sZmk,
	char *sBdk
	)
{

	int     offset = 0;
	char    sLine[MAX_LINE_TRC];    

	memset(sLine, 0, sizeof(sLine));


	trace_event("Start command_DY()", PROCESSING);

	memcpy(sHsmBuffer + offset, "DY",   2);
	offset += 2; 


	memcpy(sHsmBuffer + offset, sZmk,   strlen(sZmk));
	offset += strlen(sZmk); 

	memcpy(sHsmBuffer + offset, sBdk,   strlen(sBdk));
	offset += strlen(sBdk);


	/*
	memcpy(buffer_transmit , ";XU0", 4);  
	buffer_transmit += 4;
	*/   

	sprintf(sLine, "End command_DY(%d)", offset);
	trace_event(sLine,  PROCESSING);
	return (offset);
}

int    command_DZ( char *buffer_received, 
	char *sBdk_zmk, 
	char *check_value , 
	char *err_code 
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];

	trace_event("Start command_DZ()",PROCESSING);

	memset(sBdk_zmk    ,         0, SIZE_KEY * 3);
	memset(Command    ,         0, 3);
	memset(check_value,         0, SIZE_KEY);

	memcpy(Command , buffer_received, 2);      
	memcpy(err_code, buffer_received, 4);   
	/* memcpy(err_code, buffer_received+ 2, 2); */


	if (memcmp(Command, "DZ", 2) != 0) 
	{
		trace_event("End   command_DY(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	/* trace_event("WARNING !! : bEFORE CALLING TEXTERROR",PROCESSING); */
	TextError( err_code + 2,sLine);

	if (memcmp(err_code+2  , "01", 2) == 0 )
	{
		trace_event("WARNING !! : ODD PARITY ENFORCED",PROCESSING);
	}
	else
	{       
		if ( (memcmp(err_code + 2 , "00", 2) != 0) && (memcmp(err_code + 2 , "01", 2) != 0)  ) 

		{
			trace_event("End   command_DZ(CHECK_ERROR)",PROCESSING);
			return(CHECK_ERROR );
		}
	}

	buffer_received += 4;
	switch (buffer_received[0])
	{
	case 'U' :
	case 'X' :
		trace_event("Triple DES Encryption Double Length",TRACE);
		memcpy(sBdk_zmk, buffer_received,  2 * SIZE_KEY + 1);      
		buffer_received +=( 2 * SIZE_KEY + 1);
		memcpy(check_value, buffer_received,  SIZE_KEY);
		sBdk_zmk [ 2 * SIZE_KEY + 1] = '\0';
		break;
	case 'T' :
	case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
		memcpy(sBdk_zmk, buffer_received, 3 * SIZE_KEY + 1);      
		buffer_received +=( 3 * SIZE_KEY + 1);
		memcpy(check_value, buffer_received,   SIZE_KEY);
		sBdk_zmk [ 3 * SIZE_KEY + 1] = '\0';
		break;
	default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
		memcpy(sBdk_zmk, buffer_received, SIZE_KEY);      
		buffer_received +=SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
		sBdk_zmk [SIZE_KEY] = '\0';
	}

	trace_event("End   command_DZ(OK)",PROCESSING);
	return(OK);
}

int command_CM( char *buffer_transmit, 
	char *bdk_key,                
	char *pvka_key, 
	char *pvkb_key,
	char *ksn_desc,
	char *ksn, 
	char *pin_block, 
	char *pan, 
	char *pvv
	)
{
	char pvki[2];


	trace_event("Start command_CM()", PROCESSING);

	memset ( pvki , 0, 2 );
	memcpy ( pvki , pvv, 1 );
	memcpy( buffer_transmit ,"CM",   2);        
	buffer_transmit += 2;
	memcpy( buffer_transmit ,bdk_key,   strlen(bdk_key)); 
	buffer_transmit += strlen(bdk_key);
	memcpy( buffer_transmit ,pvka_key,   SIZE_KEY); 
	buffer_transmit += SIZE_KEY;
	memcpy( buffer_transmit ,pvkb_key,   SIZE_KEY); 
	buffer_transmit += SIZE_KEY;
	memcpy( buffer_transmit ,ksn_desc,   strlen(ksn_desc)); 
	buffer_transmit += strlen(ksn_desc);
	memcpy( buffer_transmit ,ksn,   strlen(ksn)); 
	buffer_transmit += strlen(ksn);   
	memcpy( buffer_transmit ,pin_block,   SIZE_KEY); 
	buffer_transmit += SIZE_KEY;  
	memcpy( buffer_transmit ,pan + strlen(pan) - 13 ,       12);        
	buffer_transmit += 12;
	memcpy ( buffer_transmit,pvki ,      1);        
	buffer_transmit += 1;
	memcpy( buffer_transmit ,pvv+1,      4);        
	buffer_transmit += 4;

	trace_event("End   command_CM()", PROCESSING);
	return (OK);
}

int    command_CN( char *buffer_received , 
	char *err_code 
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];

	trace_event( "Start command_CN()",PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command , buffer_received, 2);      
	memcpy(err_code, buffer_received, 4);   
	if (memcmp(Command, "CN", 2) != 0) 
	{
		trace_event( "End   command_CN(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	TextError( err_code + 2,sLine);
	if (memcmp(err_code + 2, "00", 2) == 0) 
	{
		trace_event( "End   command_CN(OK)",PROCESSING);
		return ( OK );
	}
	if (memcmp(err_code + 2, "01", 2) == 0)
	{
		trace_event( "End   command_CN(VERIFICATION_FAILED)",PROCESSING);
		return ( VERIFICATION_FAILED );
	}
	trace_event( "End   command_CN(CRYPTOGRAPHIC_ERROR)",PROCESSING);
	return(CRYPTOGRAPHIC_ERROR);
}
/*YK2007:DUKPT*/


/***********************************************************************
**  NAME  : command_KS
**  DESC  : Data Authentication Code Verification
**  INPUT :
**  OUTPUT:
**  AUTHOR:
***********************************************************************/
int command_KS  (  char *sHsmBuffer,
	char sModeFlag,
	char cSchemeId,
	char *sDmk,
	char *sPanAndSeq,
	char *sAtc,
	char *sUn,
	char *sData
	)
{

	int     offset = 0;
	char    sLine[MAX_LINE_TRC];

	trace_event("Start command_KS()", PROCESSING);


	memcpy(sHsmBuffer + offset, "KS",   2);
	offset += 2;

	memcpy(sHsmBuffer + offset, &sModeFlag,   1);
	offset += 1;

	sHsmBuffer[offset] = cSchemeId ;
	offset += 1;


	memcpy(sHsmBuffer + offset, sDmk,   strlen(sDmk));
	offset += strlen(sDmk);

	memcpy(sHsmBuffer + offset, sPanAndSeq,   8);
	offset += 8;


	memcpy(sHsmBuffer + offset, sData,   8);
	offset += 8;

	/*SKO290509*/    if(sModeFlag == 1)
	{
		memcpy(sHsmBuffer + offset, sAtc,   2);
		offset += 2;

		memcpy(sHsmBuffer + offset, sUn,   4);
		offset += 4;
	}

	sprintf(sLine, "End command_KS(%d)", offset);
	trace_event(sLine,  PROCESSING);
	return (offset);
}
/******************************************************************************/
/* Fonction command_KT.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recupere le buffer              */
/*               resultat de verif du DAC/IDN                                 */
/******************************************************************************/
int    command_KT( char *sBuffRcv,
	char *sErrCode,
	char *sData
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];

	trace_event("Start command_KT()",PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command , sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp( Command , "KT", 2) != 0)
	{
		trace_event("End   command_KT(ERROR_COMMAND)",PROCESSING);
		return ( ERROR_COMMAND );
	}

	TextError( sErrCode + 2,sLine);
	if (memcmp(sErrCode + 2, "01", 2) == 0)
	{
		memcpy(sData, sBuffRcv + 4, 8);
		trace_event("End   command_KT(VERIFICATION_FAILED)",PROCESSING);
		return ( VERIFICATION_FAILED );
	}
	if (memcmp(sErrCode + 2, "00", 2) == 0)
	{
		trace_event("End   command_KT(OK)",PROCESSING);
		return ( OK );
	}
	trace_event("End   command_KT(CRYPTOGRAPHIC_ERROR)",PROCESSING);
	return(CRYPTOGRAPHIC_ERROR);
}
/* Mehdi Elyajizi */


/*******************************************************************************/
/* Fonction command_FA.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater ZPK encryptee sous ZMK vers LMK        */
/******************************************************************************/
int 
	command_FA(char *buffer_transmit,
	char *zmk_lmk,
	char *zpk_zmk
	)
{
	char            ligne[MAX_LINE_TRC];
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "Start command_FA(%.48s, .48%s)", zmk_lmk, zpk_zmk);
	else
		sprintf(ligne, "Start command_FA()");

	trace_event(ligne, PROCESSING);

	memcpy(buffer_transmit, "FA", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));
	buffer_transmit += strlen(zmk_lmk);


	if (32 == strlen(zpk_zmk)) {
		memcpy(buffer_transmit, "X", 1);
		buffer_transmit += 1;
	}
	if (48 == strlen(zpk_zmk)) {
		memcpy(buffer_transmit, "T", 1);
		buffer_transmit += 1;
	}
	memcpy(buffer_transmit, zpk_zmk, strlen(zpk_zmk));
	buffer_transmit += strlen(zpk_zmk);

	trace_event("End   command_FA(OK)", PROCESSING);
	return (OK);
}


/*******************************************************************************/
/* Fonction command_FA_U.                                                        */
/* -------------------                                                         */
/*
*  * Description : Cette fonction a pour but de preparer le buffer a
*   * transmettre 
*    */
/* au HSM pour translater ZPK encryptee sous ZMK vers LMK        */
/******************************************************************************/
int 
	command_FA_U(char *buffer_transmit,
	char *zmk_lmk,
	char *zpk_zmk
	)
{
	char            ligne[MAX_LINE_TRC];
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "Start command_FA_U(%.48s, .48%s)", zmk_lmk, zpk_zmk);
	else
		sprintf(ligne, "Start command_FA_U()");

	trace_event(ligne, PROCESSING);

	memcpy(buffer_transmit, "FA", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));
	buffer_transmit += strlen(zmk_lmk);


	if (32 == strlen(zpk_zmk)) {
		memcpy(buffer_transmit, "U", 1);  
		buffer_transmit += 1;
	}
	if (48 == strlen(zpk_zmk)) {
		memcpy(buffer_transmit, "T", 1);
		buffer_transmit += 1;
	}
	memcpy(buffer_transmit, zpk_zmk, strlen(zpk_zmk));
	buffer_transmit += strlen(zpk_zmk);

	trace_event("End   command_FA_U(OK)", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_FB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM ZPK sous LMK, la Check_value ainsi que le code d'erreur. */
/******************************************************************************/
int 
	command_FB(char *buffer_received,
	char *zpk_lmk,
	char *check_value,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_FB()", PROCESSING);

	memset(zpk_lmk, 0, SIZE_KEY * 3);
	memset(Command, 0, 3);
	memset(check_value, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	/* memcpy(err_code, buffer_received+ 2, 2); */


	if (memcmp(Command, "FB", 2) != 0) {
		trace_event("End   command_FB(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	/* trace_event("WARNING !! : bEFORE CALLING TEXTERROR",PROCESSING); */
	TextError(err_code + 2, sLine);

	if (memcmp(err_code + 2, "01", 2) == 0) {
		trace_event("WARNING !! : ODD PARITY ENFORCED", PROCESSING);
	} else {
		/* GF */
		if ((memcmp(err_code + 2, "00", 2) != 0) && (memcmp(err_code + 2, "01", 2) != 0))
			/* GF */
		{
			trace_event("End   command_FB(CHECK_ERROR)", PROCESSING);
			return (CHECK_ERROR);
		}
	}

	buffer_received += 4;
	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(zpk_lmk, buffer_received, 2 * SIZE_KEY + 1);
		buffer_received += (2 * SIZE_KEY + 1);
		memcpy(check_value, buffer_received, SIZE_KEY);
		zpk_lmk[2 * SIZE_KEY + 1] = '\0';
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(zpk_lmk, buffer_received, 3 * SIZE_KEY + 1);
		buffer_received += (3 * SIZE_KEY + 1);
		memcpy(check_value, buffer_received, SIZE_KEY);
		zpk_lmk[3 * SIZE_KEY + 1] = '\0';
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(zpk_lmk, buffer_received, SIZE_KEY);
		buffer_received += SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
		zpk_lmk[SIZE_KEY] = '\0';
	}

	trace_event("End   command_FB(OK)", PROCESSING);
	return (OK);
}



/*******************************************************************************/
/* Fonction command_FC.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater PVK/TPK/TMK encryptee sous ZMK         */
/* vers LMK  							                           */
/*******************************************************************************/
int 
	command_FC(char *buffer_transmit,
	char *zmk_lmk,
	char *key_zmk
	)
{
	trace_event("Start command_FC()", PROCESSING);

	memcpy(buffer_transmit, "FC", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));
	buffer_transmit += strlen(zmk_lmk);
	memcpy(buffer_transmit, key_zmk, strlen(key_zmk));
	buffer_transmit += strlen(key_zmk);
	/*EBE160819: To be confirmed with a thales box
	memcpy(buffer_transmit,";0U", 3);
	buffer_transmit += 3;*/

	/*
	* JUST FOR TEST
	* memcpy(buffer_transmit,"FCU1065E88F7103933E825028454A3786D5X093B5FF
	* 4BC7FB0009BCF689D2136385E",68); 
	*/

	trace_event("End   command_FC(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_FD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM TPK ou PVK sous LMK, la Check_value ainsi que le code    */
/* d'erreur.                                                    */
/******************************************************************************/
/**
int    command_FD( char *buffer_received , 
char *key_lmk, 
char *check_value , 
char *err_code 
)
**/
int 
	command_FD(char *buffer_received, char *err_code, char *key_lmk)
{				/** MAW20030725 **/
	char            Command[3];
	char            sLine[MAX_LINE_TRC];
	char            check_value[16 + 1];

	trace_event("Start command_FD()", PROCESSING);
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
	{
		sprintf(sLine, "buffer_received=[%s]", buffer_received);
		trace_event(sLine, FATAL);
	}
	memset(Command, 0, 3);
	memset(key_lmk, 0, SIZE_KEY);
	memset(check_value, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	/** memcpy(err_code, buffer_received, 4);    **/
	memcpy(err_code, buffer_received + 2, 2);

	if (memcmp(Command, "FD", 2) != 0) {
		trace_event("End   command_FD(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	/** if (memcmp(err_code + 2 , "00", 2) != 0)  **/
	/* start YK1208 */
	if (memcmp(err_code, "01", 2) == 0) {
		trace_event("WARNING FORCAGE PARITE", PROCESSING);

	} else
		/* end YK1208 */
		if (memcmp(err_code, "00", 2) != 0) {
			trace_event("End   command_FD(CHECK_ERROR)", PROCESSING);
			return (CHECK_ERROR);
		}
		buffer_received += 4;

		switch (buffer_received[0]) {
		case 'U':
		case 'X':
			trace_event("Triple DES Encryption Double Length", TRACE);
			memcpy(key_lmk, buffer_received, (2 * SIZE_KEY) + 1);
			buffer_received += (2 * SIZE_KEY) + 1;
			memcpy(check_value, buffer_received, SIZE_KEY);
			key_lmk[(2 * SIZE_KEY) + 1] = '\0';
			break;
		case 'T':
		case 'Y':
			trace_event("Triple DES Encryption Triple Length", TRACE);
			memcpy(key_lmk, buffer_received, (3 * SIZE_KEY) + 1);
			buffer_received += (3 * SIZE_KEY) + 1;
			memcpy(check_value, buffer_received, SIZE_KEY);
			key_lmk[(3 * SIZE_KEY) + 1] = '\0';
			break;
		default:
			trace_event("Single DES Encryption Single Length", TRACE);
			memcpy(key_lmk, buffer_received, SIZE_KEY);
			buffer_received += SIZE_KEY;
			memcpy(check_value, buffer_received, SIZE_KEY);
			key_lmk[SIZE_KEY] = '\0';
		}
		/*MKB060309 PCI Norme Integration */
		if (debug_option == 1)
		{
			sprintf(sLine, "==> key_lmk=[%s]", key_lmk);
			trace_event(sLine, FATAL);
		}
		/* Mehdi Elyajizi PCI */
		trace_event("End   command_FD(OK)", PROCESSING);
		return (OK);
}


/********************************************************************************/
/* Fonction command_FI.                                                         */
/* -------------------                                                          */
/* Description : Cette fonction a pour but de preparer le buffer a transmettre  */
/*               au HSM pour generer la ZAK a partir d'une ZMK                  */
/* Entree      : ZMK sous LMK                                                   */
/********************************************************************************/
int    command_FI( char		*buffer_transmit , 
	char		*zmk_lmk,
	char		keyFlag
	)
{


	trace_event("Start command_FI()", PROCESSING);

	memcpy(buffer_transmit, "FI", 2);           
	buffer_transmit +=2;
	memcpy(buffer_transmit, &keyFlag,1);           
	buffer_transmit +=1;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk)); 
	buffer_transmit +=strlen(zmk_lmk);

	memcpy(buffer_transmit, ";XU0", 4);

	trace_event("End   command_FI(OK)", PROCESSING);
	return(OK);
}


/******************************************************************************/
/* Fonction command_FJ.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/*               HSM la ZAK sous LMK, ZAK sous ZMK, la Check_value ainsi      */
/*               que le code d'erreur.                                        */
/******************************************************************************/
int    command_FJ( char *buffer_received, 
	char *zak_zmk,
	char *zak_lmk,
	char *check_value , 
	char *err_code 
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];

	trace_event("Start command_FJ()",PROCESSING);

	memset(Command,    0, 3);
	memset(zak_lmk,    0, SIZE_KEY);
	memset(zak_zmk,    0, SIZE_KEY);
	memset(check_value,0, SIZE_KEY);

	memcpy(Command , buffer_received, 2);      
	memcpy(err_code, buffer_received, 4);   
	if (memcmp(Command, "FJ", 2) != 0) 
	{
		trace_event("End   command_FJ(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	TextError( err_code + 2,sLine);
	if (memcmp(err_code + 2 , "00", 2) != 0) 
	{
		trace_event("End   command_FJ(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
	}

	buffer_received += 4;
	switch (buffer_received[0])
	{
	case 'U' :
	case 'X' :
		memcpy(zak_zmk, buffer_received,  2 * SIZE_KEY + 1); 
		buffer_received +=( 2 * SIZE_KEY + 1);
		memcpy(zak_lmk, buffer_received, 2 * SIZE_KEY + 1); 
		buffer_received += (2 * SIZE_KEY) + 1;
		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_zmk [ 2 * SIZE_KEY + 1] = '\0'; 
		zak_lmk [ 2 * SIZE_KEY + 1] = '\0'; 
		break;
	case 'T' :
	case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
		memcpy(zak_zmk, buffer_received,  3 * SIZE_KEY + 1); 
		buffer_received +=( 3 * SIZE_KEY + 1);
		memcpy(zak_lmk, buffer_received, 3 * SIZE_KEY + 1); 
		buffer_received += (3 * SIZE_KEY + 1);
		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_zmk [ 3 * SIZE_KEY + 1] = '\0'; 
		zak_lmk [ 3 * SIZE_KEY + 1] = '\0'; 
		break;
	default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
		memcpy(zak_zmk, buffer_received, SIZE_KEY); 
		buffer_received +=SIZE_KEY;
		memcpy(zak_lmk, buffer_received, SIZE_KEY); 
		buffer_received +=SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_zmk [SIZE_KEY] = '\0';
	}

	trace_event("End   command_FJ(OK)",PROCESSING);
	return(OK);
}

/******************************************************************************/
/* Fonction command_FK.                                                       */
/* -------------------                                                        */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater ZAK encryptee sous ZMK vers LMK         */
/******************************************************************************/
int 
	command_FK(char *buffer_transmit,
	char *zmk_lmk,
	char *zak_zmk
	)
{

	trace_event("Start command_FK()", PROCESSING);

	memcpy(buffer_transmit, "FK", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, "1", 1);
	buffer_transmit += 1;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));
	buffer_transmit += strlen(zmk_lmk);
	memcpy(buffer_transmit, zak_zmk, strlen(zak_zmk));
	buffer_transmit += strlen(zak_zmk);

	trace_event("End   command_FK(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_FL.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM ZAK sous LMK, Check_value ainsi que le code d'erreur.    */
/******************************************************************************/
int 
	command_FL(char *buffer_received,
	char *zak_lmk,
	char *check_value,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_FL()", PROCESSING);

	memset(Command, 0, 3);

	memset(zak_lmk, 0, SIZE_KEY);
	memset(check_value, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "FL", 2) != 0) {
		trace_event("End   command_FL(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_FL(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;
	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(zak_lmk, buffer_received, 2 * SIZE_KEY + 1);
		buffer_received += (2 * SIZE_KEY) + 1;
		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_lmk[2 * SIZE_KEY + 1] = '\0';
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(zak_lmk, buffer_received, 3 * SIZE_KEY);
		buffer_received += (3 * SIZE_KEY);
		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_lmk[3 * SIZE_KEY] = '\0';
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(zak_lmk, buffer_received, SIZE_KEY);
		buffer_received += SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
		zak_lmk[SIZE_KEY] = '\0';
	}


	trace_event("End   command_FL(OK)", PROCESSING);
	return (OK);
}


/*******************************************************************************/
/* Fonction command_HA.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour generer la TAK a partir d'une TMK                 */
/*******************************************************************************/
int 
	command_HA(char *buffer_transmit,
	char *tmk_lmk
	)
{
	trace_event("Start command_HA()", PROCESSING);

	memcpy(buffer_transmit, "HA", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, tmk_lmk, strlen(tmk_lmk));
	buffer_transmit += strlen(tmk_lmk);

	trace_event("End   command_HA(OK)", PROCESSING);
	return (OK);
}

/* IB08052006, to generate TAK double lenght */
int 
	command_HA_TDES(char *buffer_transmit,
	char *zmk_lmk
	)
{
	trace_event("Start command_HA_TDES()", PROCESSING);
	/* command_HA(buffer_transmit, zmk_lmk); */


	memcpy(buffer_transmit, "HA", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));
	buffer_transmit += strlen(zmk_lmk);


	memcpy(buffer_transmit, ";XU0", 4);
	buffer_transmit += 4;



	trace_event("End   command_HA_TDES()", PROCESSING);

	return (OK);
}
/* IB08052006, to generate ZPK double lenght */


/******************************************************************************/
/* Fonction command_HB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM la TAK sous TMK, TAK sous LMK, la Check_value ainsi que  */
/* le code d'erreur.                                            */
/******************************************************************************/
int 
	command_HB(char *buffer_received,
	char *tak_tmk,
	char *tak_lmk,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_HB()", PROCESSING);

	memset(Command, 0, 3);
	memset(tak_lmk, 0, SIZE_KEY);
	memset(tak_tmk, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "HB", 2) != 0) {
		trace_event("End   command_HB(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_HB(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;
	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		/* IB09052006 */
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(tak_tmk, buffer_received, 2 * SIZE_KEY + 1);
		tak_tmk[2 * SIZE_KEY + 1] = '\0';
		buffer_received += (2 * SIZE_KEY) + 1;
		memcpy(tak_lmk, buffer_received, 2 * SIZE_KEY + 1);
		buffer_received += (2 * SIZE_KEY) + 1;
		tak_lmk[2 * SIZE_KEY + 1] = '\0';
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(tak_tmk, buffer_received, 3 * SIZE_KEY);
		tak_tmk[3 * SIZE_KEY] = '\0';
		buffer_received += (3 * SIZE_KEY);
		memcpy(tak_lmk, buffer_received, 3 * SIZE_KEY);
		buffer_received += (3 * SIZE_KEY);
		tak_lmk[3 * SIZE_KEY] = '\0';
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(tak_tmk, buffer_received, SIZE_KEY);
		tak_tmk[SIZE_KEY] = '\0';
		buffer_received += SIZE_KEY;
		memcpy(tak_lmk, buffer_received, SIZE_KEY);
		buffer_received += SIZE_KEY;
		tak_lmk[SIZE_KEY] = '\0';
	}


	trace_event("End   command_HB(OK)", PROCESSING);
	return (OK);
}

/*******************************************************************************/
/* Fonction command_IA.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour generer la ZPK a partir d'une ZMK                 */
/*******************************************************************************/
int 
	command_IA(char *buffer_transmit,
	char *zmk_lmk
	)
{
	trace_event("Start command_IA()", PROCESSING);

	memcpy(buffer_transmit, "IA", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));
	buffer_transmit += strlen(zmk_lmk);

	trace_event("End   command_IA()", PROCESSING);

	return (OK);
}

/* YK29122005, to generate ZPK double lenght */
int 
	command_IA_TDES(char *buffer_transmit,
	char *zmk_lmk
	)
{
	trace_event("Start command_IA_TDES()", PROCESSING);

	memcpy(buffer_transmit, "IA", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zmk_lmk, strlen(zmk_lmk));
	buffer_transmit += strlen(zmk_lmk);


	memcpy(buffer_transmit, ";XU0", 4);
	buffer_transmit += 4;



	trace_event("End   command_IA_TDES()", PROCESSING);

	return (OK);
}
/* YK29122005, to generate ZPK double lenght */

/******************************************************************************/
/* Fonction command_IB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM la ZPK sous ZMK,ZPK sous LMK, la Check_value ainsi que   */
/* le code d'erreur.                                            */
/******************************************************************************/
int 
	command_IB(char *buffer_received,
	char *zpk_zmk,
	char *zpk_lmk,
	char *check_value,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_IB()", PROCESSING);

	memset(Command, 0, 3);
	memset(zpk_lmk, 0, SIZE_KEY);
	memset(zpk_zmk, 0, SIZE_KEY);
	memset(check_value, 0, SIZE_KEY);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "IB", 2) != 0) {
		trace_event("End   command_IB(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_IB(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;
	switch (buffer_received[0]) {
	case 'U':
	case 'X':
		/* YK30122005, to save ZPK double lenght */
		trace_event("Triple DES Encryption Double Length", TRACE);
		memcpy(zpk_zmk, buffer_received, (2 * SIZE_KEY) + 1);
		zpk_zmk[(2 * SIZE_KEY) + 1] = '\0';
		buffer_received += (2 * SIZE_KEY) + 1;
		memcpy(zpk_lmk, buffer_received, (2 * SIZE_KEY) + 1);
		buffer_received += (2 * SIZE_KEY) + 1;
		zpk_lmk[(2 * SIZE_KEY) + 1] = '\0';
		/* YK30122005, to save ZPK double lenght */
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	case 'T':
	case 'Y':
		trace_event("Triple DES Encryption Triple Length", TRACE);
		memcpy(zpk_zmk, buffer_received, 3 * SIZE_KEY);
		zpk_zmk[3 * SIZE_KEY] = '\0';
		buffer_received += (3 * SIZE_KEY);
		memcpy(zpk_lmk, buffer_received, 3 * SIZE_KEY);
		buffer_received += (3 * SIZE_KEY);
		zpk_lmk[3 * SIZE_KEY] = '\0';
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	default:
		trace_event("Single DES Encryption Single Length", TRACE);
		memcpy(zpk_zmk, buffer_received, SIZE_KEY);
		zpk_zmk[SIZE_KEY] = '\0';
		buffer_received += SIZE_KEY;
		memcpy(zpk_lmk, buffer_received, SIZE_KEY);
		buffer_received += SIZE_KEY;
		zpk_lmk[SIZE_KEY] = '\0';
		memcpy(check_value, buffer_received, SIZE_KEY);
	}


	trace_event("End   command_IB(OK)", PROCESSING);
	return (OK);
}

/***********************************************************************
**  nom       : command_MA                                            **
**  desc      : construction de la commande MA par VHSM               **
**  entree    :
**              pvka_lmk      -> ptmk sous lmk                        **
**              pan           -> pan                                  **
**  sortie    : buffer        -> message construit                    **
**  retour    : OK    -> fonction aboutie avec succes                 **
**  auteur    : E ENNOUINI (14 Mar 2001)                              **
***********************************************************************/

int    command_MA( buffer_transmit , data_len, tak_lmk, data)
	unsigned char  *buffer_transmit;
int             data_len;
unsigned char   tak_lmk[];
unsigned char   data[];
{
	int offset=0;

	trace_event("Start command_MA()", PROCESSING);

	memcpy ( buffer_transmit , "MA" , 2);
	offset += 2;
	memcpy ( buffer_transmit + offset, tak_lmk , strlen(tak_lmk));
	offset += strlen(tak_lmk);
	/*
	memcpy ( buffer_transmit , "MK" , 2);
	offset += 2;
	memcpy ( buffer_transmit + offset, tak_lmk + 1, strlen(tak_lmk) - 1);
	offset += strlen(tak_lmk) - 1;
	sprintf(buffer_transmit+offset,"%03X",data_len);
	offset += 3;
	*/

	memcpy(buffer_transmit+offset, data, data_len);
	offset += data_len;

	trace_event("End command_MA()", PROCESSING);
	return (offset);
}

/***********************************************************************
**  nom       : command_MB                                            **
**  desc      : traitement de la commande MA envoye VHSM              **
**  entree    : buffer        -> message construit                    **
**  sortie    : err_code      -> error code                           **
**  retour    : OK    -> fonction aboutie avec succes                 **
**              NOK   -> fonction aboutie avec echec                  **
**              ERROR -> fonction non aboutie                         **
**  auteur    : E ENNOUINI (14 Mar 2001)                              **
***********************************************************************/
int    command_MB( buffer_received , err_code, mac)
	char   *buffer_received;
char   *err_code;
char   *mac;
{

	trace_event("Start command_MB()", PROCESSING);

	memcpy(err_code, buffer_received + 2, 2);

	/*if (memcmp(buffer_received, "MB", 2) != 0) return(ERROR_COMMAND);*/

	if (memcmp(err_code , "00", 2) == 0)
	{
		memcpy(mac,buffer_received + 4,8);
		return ( OK );
	}

	trace_event("End command_MB()", PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}


int    command_MC( 
	unsigned char  *buffer_transmit,
	int             data_len,
	unsigned char   tak_lmk[],
	unsigned char   mac[],
	unsigned char   data[])
{
	int offset=0;

	trace_event("Start command_MC()", PROCESSING);

	memcpy ( buffer_transmit , "MC" , 2);
	offset += 2;
	memcpy ( buffer_transmit + offset , tak_lmk , strlen(tak_lmk));
	offset +=strlen(tak_lmk);
	/*
	memcpy ( buffer_transmit , "MM" , 2);offset += 2;
	memcpy ( buffer_transmit + offset , tak_lmk + 1 , strlen(tak_lmk) - 1);
	offset +=strlen(tak_lmk) - 1;
	sprintf(buffer_transmit+offset,"%03X",data_len);
	offset += 3;
	*/


	memcpy ( buffer_transmit + offset , mac , 8);
	offset += 8;
	memcpy(buffer_transmit + offset, data, data_len);
	offset += data_len;

	trace_event("End command_MC()", PROCESSING);
	return (offset);
}

/***********************************************************************
**  nom       : command_MD                                            **
**  desc      : traitement de la commande MD envoye VHSM              **
**  entree    : buffer        -> message construit                    **
**  sortie    : err_code      -> error code                           **
**  retour    : OK    -> fonction aboutie avec succes                 **
**              NOK   -> fonction aboutie avec echec                  **
**              ERROR -> fonction non aboutie                         **
**  auteur    : E ENNOUINI (14 Mar 2001)                              **
***********************************************************************/
int    command_MD( buffer_received , err_code)
	char   *buffer_received;
char   *err_code;
{

	trace_event("Start command_MD()", PROCESSING);

	memcpy(err_code, buffer_received + 2, 2);

	/*if (memcmp(buffer_received, "MD", 2) != 0) return(ERROR_COMMAND);*/

	if (memcmp(err_code , "00", 2) == 0) return ( OK );

	trace_event("End command_MD()", PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}


	