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

/*static char gs_zmk_key_scheme = 'X';*/ /* AHA20221007 PLUTONL-4980*/
/******************************************************************************/
/* Fonction format_card_from_pan.                                             */
/* -------------------                                                        */
/* -----   Numero de Carte sans le dernier digit Luhn Key                     */
/******************************************************************************/
int format_card_from_pan(char *card,char *pan)
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
/* Tableau de descritions des reponses HSM                                    */
/* -------------------                                                        */
/******************************************************************************/
typedef struct s_response {
	char           *reponse;
	char           *description;
}               t_response;
/******************************************************************************/

t_response      hsm_response[] = {
	{"00", "Operation Successful.........."},
	{"01", "Verification Failure........."},
	{"04", "Invalid Key Type Code........"},
	{"05", "Invalid Key Length flag......"},
	{"10", "Source Key Parity Error......"},
	{"11", "Destination Key Parity Error."},
	{"12", "Contents Of User Storage Empty."},
	{"13", "Master Key Parity Error......"},
	{"14", "PIN encrypted under (02,03) Invalid."},
	{"15", "Invalid Input Data..........."},
	{"16", "Console Or Printer Not Ready."},
	{"17", "HSM Not In Authorized State.."},
	{"18", "Document Format Not Loaded..."},
	{"19", "Specified Diebold Table is Invalid."},
	{"20", "PIN Block Does Not Contain Valid Values."},
	{"21", "Invalid Indexd Value, or overflow condition."},
	{"22", "Invalid Account Number......."},
	{"23", "Invalid Pin Block Format Code."},
	{"24", "PIN is Fewer than 4 or more than 12"},
	{"25", "Decimalization Table Error...."},
	{"30", "Invalid Reference Number......"},
	{"31", "Insufficient Solicitation Entries for Batch"},
	{"33", "LMK Key Change Storage Corrupted."},
	{"40", "Invalid Firmware Checksum...."},
	{"41", "Internal Hardware/Software Error."},
	{"42", "DES Failure ..................."},
	{"80", "Data Length Error, Less or Greather than Expected"},
	{"90", "Data Parity Error In Request Message"}
};

/******************************************************************************/
/******************************************************************************/
int 
	TextError(char *err_code, char *sMsgErr)
{
	int             i;
	char            ligne[MAX_LINE_TRC];

	for (i = 0; i < (sizeof(hsm_response) / sizeof(hsm_response[0])); i++)
	{
		if (!memcmp(hsm_response[i].reponse, err_code, 2)) {
			sprintf(ligne, "=> %s <=", hsm_response[i].description);
			trace_event(ligne, PROCESSING);
			sprintf(sMsgErr, "%s", hsm_response[i].description);
			return (OK);
		}
	}
	sprintf(sMsgErr, "=>UNKNOWN ERROR CODE %s <=", err_code);
	return (OK);
}





/******************************************************************************/
/* Fonction command_CA.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de translater un PIN encrypte sous */
/* une cle TPK vers une cle ZPK.                                */
/******************************************************************************/
int 
	command_CA(char *buffer_transmit,
	char *Tpk,
	char *Zpk,
	char *PinTpk,
	char *format_in,
	char *format_out,
	char *Pan
	)
{
	char            Card[13];

	trace_event("Start command_CA()", PROCESSING);

	memset(Card, 0, 13);

	/*Command Code*/
	memcpy(buffer_transmit, "CA", 2);
	buffer_transmit += 2;

	/*Source TPK*/
	memcpy(buffer_transmit, Tpk, strlen(Tpk));
	buffer_transmit += strlen(Tpk);

	/*Destination Key*/
	memcpy(buffer_transmit, Zpk, strlen(Zpk));
	buffer_transmit += strlen(Zpk);

	/*Maximum PIN Length*/
	memcpy(buffer_transmit, "12", 2);
	buffer_transmit += 2;

	/*Source PIN Block*/
	memcpy(buffer_transmit, PinTpk, SIZE_KEY);
	buffer_transmit += SIZE_KEY;

	/*Source PIN Block Format Code*/
	memcpy(buffer_transmit, format_in, 2);
	buffer_transmit += 2;

	/*Destination PIN Block Format Code*/
	memcpy(buffer_transmit, format_out, 2);
	buffer_transmit += 2;

	/*PAN*/
	format_card_from_pan(Card, Pan);
	memcpy(buffer_transmit, Card, 12);

	trace_event("End   command_CA()", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_CB.                                                       */
/* -------------------                                                        */
/*
* Description : Cette fonction a pour but de recuperer le PIN translate
* d'une 
*/
/* cle TPK vers une cle ZPK.                                    */
/******************************************************************************/
int 
	command_CB(char *buffer_received,
	char *PinZpk,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_CB()", PROCESSING);

	memset(PinZpk, 0, SIZE_KEY);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "CB", 2) != 0)
		return (ERROR_COMMAND);

	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0)
		return (CHECK_ERROR);
	buffer_received += 6;

	memcpy(PinZpk, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;

	trace_event("End   command_CB()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_CC.                                                       */
/* -------------------                                                        */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour translater le Pin d'une ZPK vers une autre ZPK   */
/******************************************************************************/
int 
	command_CC(char *buffer_transmit,
	char *zpk_in,
	char *zpk_out,
	char *pin,
	char *format_in,
	char *format_out,
	char *pan
	)
{
	char            card[13];
	char            sLine[MAX_LINE_TRC];
	int             offset;

	trace_event("Start command_CC()", PROCESSING);

	if (debug_option == 1)
	{
		sprintf(sLine, "==> zpk_in     =[%s]", zpk_in);
		trace_event(sLine, FATAL);
		sprintf(sLine, "==> zpk_out    =[%s]", zpk_out);
		trace_event(sLine, FATAL);
		sprintf(sLine, "==> pin        =[%s]", pin);
		trace_event(sLine, FATAL);
		sprintf(sLine, "==> format_in  =[%s]", format_in);
		trace_event(sLine, FATAL);
		sprintf(sLine, "==> format_out =[%s]", format_out);
		trace_event(sLine, FATAL);
		sprintf(sLine, "==> pan        =[%s]", pan);
		trace_event(sLine, FATAL);
	}
	offset = 0;
	memset(card, 0, 13);

	/*Command Code*/
	memcpy(buffer_transmit + offset, "CC", 2);
	offset += 2;

	/*Source ZPK*/
	memcpy(buffer_transmit + offset, zpk_in, strlen(zpk_in));
	offset += strlen(zpk_in);

	/*Destination ZPK*/
	memcpy(buffer_transmit + offset, zpk_out, strlen(zpk_out));
	offset += strlen(zpk_out);
    
	/*Maximum PIN Length*/
	memcpy(buffer_transmit + offset, "12", 2);
	offset += 2;

	/*Source PIN block */
	memcpy(buffer_transmit + offset, pin, SIZE_KEY);
	offset += SIZE_KEY;

	/*Source PIN block format */
	memcpy(buffer_transmit + offset, format_in, 2);
	offset += 2;

	/*Destination PIN block format*/
	memcpy(buffer_transmit + offset, format_out, 2);
	offset += 2;

	/*PAN/Token*/
	format_card_from_pan(card, pan);
	memcpy(buffer_transmit + offset, card, 12);
	offset += 12;
	
	
	trace_event("End   command_CC()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_CD.                                                       */
/* -------------------                                                        */
/*
* Description : Cette fonction a pour but de recuperer le PIN translate
* d'une 
*/
/* cle ZPK vers une autre cle ZPK.                              */
/******************************************************************************/
int 
	command_CD(char *buffer_received,
	char *PinZpk,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_CD()", PROCESSING);

	memset(PinZpk, 0, SIZE_KEY);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "CD", 2) != 0)
		return (ERROR_COMMAND);
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0)
		return (CHECK_ERROR);
	buffer_received += 6;

	memcpy(PinZpk, buffer_received, SIZE_KEY);

	trace_event("End   command_CD()", PROCESSING);
	return (OK);
}

/*******************************************************************************/
/* Fonction command_CW.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour generer la CVV a encoder sur la carte             */
/*******************************************************************************/
int 
	command_CW(char *buffer_transmit,
	char *cvka_lmk,
	char *cvkb_lmk,
	char *pan,
	char *expirat_date,
	char *serv_code
	)
{
	char            ligne[MAX_LINE_TRC];



	if (debug_option == 1)
		sprintf(ligne, "Start  command_CW(%s,%s,%s)", pan, expirat_date, serv_code);
	else
		sprintf(ligne, "Start command_CW()");
	trace_event(ligne, PROCESSING);

	/*Command Code*/
	memcpy(buffer_transmit, "CW", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, cvka_lmk, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, cvkb_lmk, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, pan, strlen(pan));
	buffer_transmit += strlen(pan);

	/*Delimiter*/
	memcpy(buffer_transmit, ";", 1);
	buffer_transmit += 1;

	/*Expiration date*/
	memcpy(buffer_transmit, expirat_date, 4);
	buffer_transmit += 4;

	/*Service Code*/
	memcpy(buffer_transmit, serv_code, 3);

	trace_event("End   command_CW()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_CX.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM la CVV et le code erreur associe                         */
/******************************************************************************/
int 
	command_CX(char *buffer_received,
	char *cvv,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_CX()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "CX", 2) != 0)
		return (ERROR_COMMAND);

	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0)
		return (CHECK_ERROR);
	buffer_received += 4;

	memcpy(cvv, buffer_received, 3);

	trace_event("End   command_CX()", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_CY                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer a envoyer    */
/* au HSM pour la verification du CVV.                          */
/******************************************************************************/
int 
	command_CY(char *buffer_transmit,
	char *card_number,
	char *service_code,
	char *expiry_date,
	char *cvk_A,
	char *cvk_B,
	char *cvv
	)
{
	char            ligne[MAX_LINE_TRC];
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "Start command_CY(%.22s,%.3s,%.4s,%.3s)",
		card_number, service_code, expiry_date, cvv);
	else
		sprintf(ligne, "Start command_CY()");

	trace_event(ligne, PROCESSING);

	memcpy(buffer_transmit, "CY", 2);
	buffer_transmit += 2;

	memcpy(buffer_transmit, cvk_A, strlen(cvk_A));
	buffer_transmit += strlen(cvk_A);
	if( cvk_A[0] != 'S' && cvk_A[0] != 'U')
	{
		memcpy(buffer_transmit, cvk_B, SIZE_KEY);
		buffer_transmit += SIZE_KEY;
	}
	
	memcpy(buffer_transmit, cvv, 3);
	buffer_transmit += 3;

	/*Primary account number (nN)*/
	memcpy(buffer_transmit, card_number, strlen(card_number));
	buffer_transmit += strlen(card_number);

	/*Delimiter (1A)*/
	memcpy(buffer_transmit, ";", 1);
	buffer_transmit += 1;

	/*Expiration date (4N)*/ /*ZKO why it is divided into two*????*/
	memcpy(buffer_transmit, expiry_date + 0, 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, expiry_date + 2, 2);
	buffer_transmit += 2;

	/*Service code (3N)*/
	memcpy(buffer_transmit, service_code, 3);
	buffer_transmit += 3;

	trace_event("End   command_CY(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_CZ.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de donner le resultat de la        */
/* verification CVV                                             */
/******************************************************************************/
int 
	command_CZ(char *buffer_received,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_CZ()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "CZ", 2) != 0) {
		trace_event("End   command_CZ(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) == 0) {
		trace_event("End   command_CZ(OK)", PROCESSING);
		return (OK);
	}
	if (memcmp(err_code + 2, "01", 2) == 0) {
		trace_event("End   command_CZ(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}
	trace_event("End   command_CZ(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);


}
/******************************************************************************/
/* Fonction command_RY                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer a envoyer    */
/*               au HSM pour la verification du CSC.                          */
/* Author : Nizar LYAZIDI                                                      */
/******************************************************************************/

int command_RY ( char *buffer_transmit, 
	char *card_number, 
	char *expiry_date, 
	char *service_code,	
	char *csck,
	char *csc  , 
	int  flag
	)

{

	char ligne[MAX_LINE_TRC];

	int nb=0;
	int i=0;


	if (debug_option == 1)
	{
		if(flag==0)
			sprintf(ligne, "Start  command_RY(%s,%.22s,%.4s,%s)", csck, card_number, expiry_date, csc );
		else
			sprintf(ligne, "Start  command_RY(%s,%.22s,%.4s,%.3s,%s)", csck, card_number, expiry_date,service_code, csc );	

		trace_event(ligne, PROCESSING);
	}
	else
	{
		sprintf(ligne, "Start command_RY()");
		trace_event(ligne, PROCESSING);	 
	}


	memcpy(buffer_transmit , "RY", 2);         

	buffer_transmit  += 2;

	memcpy(buffer_transmit , "4" , 1);         

	buffer_transmit  += 1;
	
	if(flag==0) memcpy(buffer_transmit , "0" , 1);  
	if(flag==2) memcpy(buffer_transmit , "2" , 1);         
	if(flag==3) memcpy(buffer_transmit , "3" , 1);

	buffer_transmit  += 1;

	/*
	memcpy(buffer_transmit , csck, 2 * SIZE_KEY); 
	buffer_transmit += 2 * SIZE_KEY;
	*/
	memcpy(buffer_transmit , csck, strlen(csck)); 
	buffer_transmit += strlen(csck);

	memcpy(buffer_transmit , card_number, strlen(card_number));
	buffer_transmit+=strlen(card_number);

	if(strlen(card_number) < 19)
	{
		nb = 19 - strlen(card_number);
		for (i=0; i<nb; i++ )
		{
			memcpy(buffer_transmit , "0" , 1);         
			buffer_transmit  += 1;
		}
	}



	memcpy(buffer_transmit , expiry_date + 0, 4); 
	buffer_transmit += 4;
	

	if(flag != 0)
	{	   
		memcpy(buffer_transmit , service_code , 3);
		buffer_transmit += 3;
	}	    

	if(flag == 3)
	{	   
		memcpy(buffer_transmit , csc+9, 3);    
		buffer_transmit += 3;
	}
	else
	{
		memcpy(buffer_transmit , csc, strlen(csc));    
		buffer_transmit += strlen(csc);
	}



	trace_event("End   command_RY(OK)",PROCESSING);

	return(OK);

}

/******************************************************************************/
/* Fonction command_RZ.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de donner le resultat de la        */
/*               verification CSC                                             */
/* Author : Nizar LYAZIDI                                                      */
/******************************************************************************/

int    command_RZ( char *buffer_received , char *err_code)

{

	char Command[2 + 1];
	char sLine[MAX_LINE_TRC];



	trace_event("Start command_RZ()",PROCESSING);



	memset(Command , 0, 2);      
	memcpy(Command , buffer_received, 2);      
	memcpy(err_code, buffer_received, 8);   

	if (memcmp(Command, "RZ", 2) != 0)
	{
		trace_event("End   command_RZ(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	TextError( err_code + 2,sLine);

	if (memcmp(err_code + 2, "00", 2) == 0)  /* 00 for no error, 4 for Mode , 1 : 5 digit not presenet, 0 : 4 digit pass, 1 : 3 digit not present, See HSM doc*/
	{
		trace_event("End   command_RZ(OK)",PROCESSING);
		return(OK);
	}

	if (memcmp(err_code + 2, "01", 2) == 0) 
	{
		trace_event("End   command_RZ(VERIFICATION_FAILED)",PROCESSING);
		return(VERIFICATION_FAILED);
	}

	trace_event("End   command_RZ(CRYPTOGRAPHIC_ERROR)",PROCESSING);
	return(CRYPTOGRAPHIC_ERROR); 

}

/*******************************************************************************/
/* Fonction command_EC.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour verifier le PVV                                   */
/*******************************************************************************/
int 
	command_EC(char *buffer_transmit,
	char *zpk,
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

	trace_event("Start command_EC()", PROCESSING);

	memset(card, 0, sizeof(card));
	memset(pvki, 0, sizeof(pvki));
	memcpy(pvki, pvv, 1);

	memcpy(buffer_transmit, "EC", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, zpk, strlen(zpk));
	buffer_transmit += strlen(zpk);

	memcpy(buffer_transmit, pvka, strlen(pvka));
	buffer_transmit += strlen(pvka);
	if( pvka[0] != 'S' && pvka[0] != 'U')
	{
		memcpy(buffer_transmit, pvkb, SIZE_KEY);
		buffer_transmit += SIZE_KEY;
	}

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

	trace_event("Fin   command_EC(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_ED.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recupere le buffer              */
/* resultat de verif PVV                                        */
/******************************************************************************/
int 
	command_ED(char *buffer_received,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_ED()", PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "ED", 2) != 0) {
		trace_event("End   command_ED(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) == 0) {
		trace_event("End   command_ED(OK)", PROCESSING);
		return (OK);
	}
	if (memcmp(err_code + 2, "01", 2) == 0) {
		trace_event("End   command_ED(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}
	trace_event("End   command_ED(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/* Fonction command_DC                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer a envoyer    */
/* au HSM pour la verification du PVV.                          */
/******************************************************************************/
int 
	command_DC(char *buffer_transmit,
	char *tpk_key,
	char *pvka_key,
	char *pvkb_key,
	char *pin_block,
	char *format,
	char *pan,
	char *pvv
	)
{
	char            pvki[2];


	trace_event("Start command_DC()", PROCESSING);

	memset(pvki, 0, 2);
	memcpy(pvki, pvv, 1);

	/*Command Code (2A)*/
	memcpy(buffer_transmit, "DC", 2);
	buffer_transmit += 2;

	/*TPK ('S' + n A)*/
	memcpy(buffer_transmit, tpk_key, strlen(tpk_key));
	buffer_transmit += strlen(tpk_key);

	memcpy(buffer_transmit, pvka_key, strlen(pvka_key));
	buffer_transmit += strlen(pvka_key);

	if( pvka_key[0] != 'S' && pvka_key[0] != 'U')
	{
		memcpy(buffer_transmit, pvkb_key, SIZE_KEY);
		buffer_transmit += SIZE_KEY;
	}


	/*PIN Block (16 or 32H)*/
	memcpy(buffer_transmit, pin_block, SIZE_KEY);
	buffer_transmit += SIZE_KEY;

	/*PIN Block Format Code*/
	memcpy(buffer_transmit, format, 2);
	buffer_transmit += 2;

	/*PAN( 12N)*/
	memcpy(buffer_transmit, pan + strlen(pan) - 13, 12);
	buffer_transmit += 12;

	/*PVKI (1N)*/
	memcpy(buffer_transmit, pvki, 1);
	buffer_transmit += 1;

	/*PVV (4N)*/
	memcpy(buffer_transmit, pvv + 1, 4);
	buffer_transmit += 4;

	trace_event("End   command_DC()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_DD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de donner le resultat de la        */
/* verification PVV                                             */
/******************************************************************************/
int 
	command_DD(char *buffer_received,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_DD()", PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "DD", 2) != 0) {
		trace_event("End   command_DD(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) == 0) {
		trace_event("End   command_DD(OK)", PROCESSING);
		return (OK);
	}
	if (memcmp(err_code + 2, "01", 2) == 0) {
		trace_event("End   command_DD(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}
	trace_event("End   command_DD(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);
}



/******************************************************************************/
/* Fonction command_DE                                                        */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de generer un IBM PIN offset       */
/******************************************************************************/

int 
	command_DE(char *buffer_transmit,
	char *Pvka,
	char *PinLmk,
	char *Pan,
	char *sDecTable
	)
{

	char            Card[13];

	trace_event("Start command_DE()", PROCESSING);

	memset(Card, 0, 13);
	memcpy(buffer_transmit, "DE", 2);
	buffer_transmit += 2;
	
	memcpy(buffer_transmit, Pvka, strlen(Pvka));/*NAB18022019 Key Block*/
	buffer_transmit += strlen(Pvka);
	
	memcpy(buffer_transmit, PinLmk, strlen(PinLmk));
	buffer_transmit += strlen(PinLmk);
	memcpy(buffer_transmit, "04", 2);
	buffer_transmit += 2;

	format_card_from_pan(Card, Pan);
	memcpy(buffer_transmit, Card, 12);
	buffer_transmit += 12;
	/*PROD00053984
	memcpy(buffer_transmit, Validation_data, SIZE_KEY);*/
	memcpy(buffer_transmit, sDecTable, SIZE_KEY);
	buffer_transmit += SIZE_KEY;

	memcpy(buffer_transmit, Pan, 10);
	buffer_transmit += 10;
	memcpy(buffer_transmit, "N", 1);
	buffer_transmit += 1;
	memcpy(buffer_transmit, Pan + (strlen(Pan) - 1), 1);
	buffer_transmit += 1;

	trace_event("End   command_DE(OK)", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_DF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM l'offset.                                                  */
/******************************************************************************/
int 
	command_DF(char *buffer_received,
	char *offset,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_DF()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "DF", 2) != 0) {
		trace_event("End   command_DF(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	/*if (memcmp(err_code + 2, "00", 2) != 0) {*/
	if (memcmp(err_code + 2, "00", 2) != 0 && memcmp(err_code + 2, "02", 2) != 0) {
		trace_event("End   command_DF(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	memcpy(offset, buffer_received, 12);
	trace_event("End   command_DF(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_DG.                                                       */
/* -------------------                                                        */
/* Description :  formatter un buffer pour demande de generation PVV          */
/******************************************************************************/
int 
	command_DG(char *buffer_transmit,
	char *pvka,
	char *pvkb,
	char *pin,
	char *Pan,
	char *pvki
	)
{
	char            Card[13];
	memset(Card, 0, 13);

	trace_event("Start   command_DG()", PROCESSING);

	memcpy(buffer_transmit, "DG", 2);
	buffer_transmit += 2;
	
	memcpy(buffer_transmit, pvka, strlen(pvka));/*NAB18022019 Key Block*/
	buffer_transmit += strlen(pvka);
	if( pvka[0] != 'S' && pvka[0] != 'U')
	{
		memcpy(buffer_transmit, pvkb, SIZE_KEY);
		buffer_transmit += SIZE_KEY;
	}
	
	/*PROD00053984: support of pin length > 4
	memcpy(buffer_transmit, pin, PIN_LMK_LENGTH);
	buffer_transmit += PIN_LMK_LENGTH;*/
	memcpy(buffer_transmit, pin, strlen(pin));
	buffer_transmit += strlen(pin);
	format_card_from_pan(Card, Pan);
	memcpy(buffer_transmit, Card, 12);
	buffer_transmit += 12;
	memcpy(buffer_transmit, pvki, 1);
	buffer_transmit += 1;

	trace_event("End    command_DG()", PROCESSING);
	return (OK);
}



/******************************************************************************/
/* Fonction command_DH.                                                       */
/* -------------------                                                        */
/* recuperer le PVV                                       */
/******************************************************************************/
int 
	command_DH(
	char           *buffer_received,
	char           *pvv,
	char           *err_code)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_DH()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "DH", 2) != 0) {
		trace_event("End   command_DH(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_DH(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	memcpy(pvv, buffer_received, 4);

	trace_event("End   command_DH(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_EA                                                         */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de verifier un PIN methode IBM     */
/******************************************************************************/
int 
	command_EA(char *buffer_transmit,
	char *Zpk,
	char *Pvka,
	char *PinZpk,
	char *format,
	char *Pan,
	char *offset,
	int  PinLength,
	char *sDecTable
	)
{

	char            Card[13];
	char            vPinLen[2 + 1];

	trace_event("Start command_EA()", PROCESSING);

	memset(Card, 0, 13);
	memcpy(buffer_transmit, "EA", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, Zpk, strlen(Zpk));
	buffer_transmit += strlen(Zpk);

	memcpy(buffer_transmit, Pvka, strlen(Pvka));
	buffer_transmit += strlen(Pvka);
	
	memcpy(buffer_transmit, "12", 2);
	buffer_transmit += 2;
	
	memcpy(buffer_transmit, PinZpk, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	
	memcpy(buffer_transmit, format, 2);
	buffer_transmit += 2;
	
	/*memcpy(buffer_transmit, "04", 2);*/
	sprintf(vPinLen, "%02d", PinLength);
	memcpy(buffer_transmit, vPinLen, 2);
	buffer_transmit += 2;

	format_card_from_pan(Card, Pan);
	memcpy(buffer_transmit, Card, 12);
	buffer_transmit += 12;

	/*PROD00053984
	memcpy(buffer_transmit, Validation_data, SIZE_KEY);*/
	memcpy(buffer_transmit, sDecTable, SIZE_KEY);

	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, Pan, 10);
	buffer_transmit += 10;
	memcpy(buffer_transmit, "N", 1);
	buffer_transmit += 1;
	memcpy(buffer_transmit, Pan + (strlen(Pan) - 1), 1);
	buffer_transmit += 1;
	memcpy(buffer_transmit, offset, 4);
	buffer_transmit += 4;
	memcpy(buffer_transmit, "FFFFFFFF", 8);
	buffer_transmit += 8;

	trace_event("Start command_EA(OK)", PROCESSING);
	return (OK);
}
/******************************************************************************/
/* Fonction command_EB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de donner le resultat de la        */
/* verification du PIN par la methode IBM                       */
/******************************************************************************/
int 
	command_EB(char *buffer_received,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_EB()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "EB", 2) != 0) {
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) == 0 || memcmp(err_code + 2, "02", 2) == 0) { /*ZKO PROD00072366*/
		trace_event("End   command_EB(OK)", PROCESSING);
		return (OK);
	}
	if (memcmp(err_code + 2, "01", 2) == 0) {
		trace_event("End   command_EB(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}
	trace_event("End   command_EB(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);
}

/*******************************************************************************/
/* Fonction command_EE.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour generer le PIN                                    */
/*******************************************************************************/
int 
	command_EE(char *buffer_transmit,
	char *pvk_lmk,
	char *offset,
	char *pan,
	char *dec_table
	)
{
	char            Card[13];
	char            vPinLen[2 + 1];
	char			sLine[MAX_LINE_TRC];
	int				PinLength;

	memset(Card, 0, sizeof(Card));

	if( debug_option == 1 )
	{

		sprintf(sLine,"Start command_EE(%s,%s,%s,%s)", pan,offset,pvk_lmk,dec_table);
		trace_event(sLine, PROCESSING);
	}
	else
	{
		trace_event("Start command_EE()", PROCESSING);	
	}


	PinLength = strlen(offset);
	if( PinLength > 12 )
	{
		trace_event("End   command_EE(NOK): Invalid PIN Length", PROCESSING);
		return NOK;
	}

	memcpy(buffer_transmit, "EE", 2);
	buffer_transmit += 2;

	memcpy(buffer_transmit, pvk_lmk, strlen(pvk_lmk));
	buffer_transmit += strlen(pvk_lmk);

	/*memcpy(buffer_transmit, "FFFFFFFF04", 10);
	buffer_transmit += 10;*/

	memcpy(buffer_transmit, offset, PinLength);
	if( PinLength < 12 )
	{
		memset(buffer_transmit + PinLength,'F',12 - PinLength);
	}
	buffer_transmit += 12;

	sprintf(vPinLen, "%02d", PinLength);
	memcpy(buffer_transmit, vPinLen, 2);
	buffer_transmit+=2;


	format_card_from_pan(Card, pan);
	memcpy(buffer_transmit, Card, 12);
	buffer_transmit += 12;

	/*PROD00053984
	memcpy(buffer_transmit, Validation_data, SIZE_KEY);*/
	memcpy(buffer_transmit, dec_table, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, pan, 10);
	buffer_transmit += 10;
	memcpy(buffer_transmit, "N", 1);
	buffer_transmit += 1;
	memcpy(buffer_transmit, pan + (strlen(pan) - 1), 1);
	buffer_transmit += 1;

	trace_event("End   command_EE(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_EF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM le PIN et le code erreur associe               */
/******************************************************************************/
int 
	command_EF(char *buffer_received,
	char *pin,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_EF()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "EF", 2) != 0) {
		trace_event("End   command_EF(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_EF(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	/*PROD00053984: support of pin length > 4
	memset(pin, 0, PIN_LMK_LENGTH);
	memcpy(pin, buffer_received, PIN_LMK_LENGTH);
	*/

	strncpy(pin, buffer_received, PIN_LMK_MAX_LEN);

	trace_event("End   command_EF(OK)", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_EF_35.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM le PIN et le code erreur associe               */
/******************************************************************************/
int 
	command_EF_35(char *buffer_received,
	char *pin,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_EF_35()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if ((memcmp(Command, "EF", 2) != 0)) {
		trace_event("End   command_EF(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0 && (memcmp(err_code + 2, "02", 2) != 0)) {
		trace_event("End   command_EF(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	/*PROD00053984: support of pin length > 4
	memset(pin, 0, PIN_LMK_LENGTH);
	memcpy(pin, buffer_received, PIN_LMK_LENGTH);
	*/

	strncpy(pin, buffer_received, PIN_LMK_MAX_LEN);

	trace_event("End   command_EF_35(OK)", PROCESSING);
	return (OK);
}

/*******************************************************************************/
/* Fonction command_PE.                                                        */
/* -------------------                                                         */
/*
* Description : Cette fonction a pour but de preparer le buffer a
* transmettre 
*/
/* au HSM pour imprimer le PIN                                   */
/*******************************************************************************/
int 
	command_PE(char *buffer_transmit,
	char *pan,
	char *pin,
	char *field0,
	char *field1,
	char *field2,
	char *field3,
	char *field4,
	char *field5,
	char *field6
	)
{
	char            card[13];
	memset(card, 0, 13);

	trace_event("Start command_PE()", PROCESSING);

	memcpy(buffer_transmit, "PE", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, "C", 1);
	buffer_transmit += 1;
	format_card_from_pan(card, pan);
	memcpy(buffer_transmit, card, 12);
	buffer_transmit += 12;
	/*
	memcpy(buffer_transmit, pin, PIN_LMK_LENGTH);
	buffer_transmit += PIN_LMK_LENGTH;
	*/
	memcpy(buffer_transmit, pin, strlen(pin));
	buffer_transmit += strlen(pin);

	memcpy(buffer_transmit, field0, strlen(field0));
	buffer_transmit += strlen(field0);

	memcpy(buffer_transmit, ";", 1);
	buffer_transmit += 1;

	memcpy(buffer_transmit, field1, strlen(field1));
	buffer_transmit += strlen(field1);

	memcpy(buffer_transmit, ";", 1);
	buffer_transmit += 1;

	memcpy(buffer_transmit, field2, strlen(field2));
	buffer_transmit += strlen(field2);

	memcpy(buffer_transmit, ";", 1);
	buffer_transmit += 1;

	memcpy(buffer_transmit, field3, strlen(field3));
	buffer_transmit += strlen(field3);

	memcpy(buffer_transmit, ";", 1);
	buffer_transmit += 1;

	memcpy(buffer_transmit, field4, strlen(field4));
	buffer_transmit += strlen(field4);

	memcpy(buffer_transmit, ";", 1);
	buffer_transmit += 1;

	memcpy(buffer_transmit, field5, strlen(field5));
	buffer_transmit += strlen(field5);

	memcpy(buffer_transmit, ";", 1);
	buffer_transmit += 1;

	memcpy(buffer_transmit, field6, strlen(field6));
	buffer_transmit += strlen(field6);

	trace_event("End   command_PE(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_PZ.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de controler le message recu suite */
/* l'impression du PIN. Le PF n'est pas remonter par le serv.  */
/******************************************************************************/
int 
	command_PZ(char *buffer_received,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_PZ()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "PZ", 2) != 0) {
		trace_event("End   command_PZ(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_PZ(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	trace_event("End   command_PZ(OK)", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_JC.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de translater un PIN sous TPK vers */
/* PIN sous LMK.                                                */
/******************************************************************************/
int 
	command_JC(char *buffer_transmit,
	char *Tpk,
	char *PinBlock,
	char *format,
	char *Pan
	)
{
	char            Card[13];
	char            ligne[MAX_LINE_TRC];
	/*SKO290509*/ char           CardTrunc[22 + 1];
	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(Card, 0, 13);
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "Start command_JC(%.16s,%.2s,%.22s)", PinBlock, format, Pan);
	else{
		TrunCard(Pan, CardTrunc);
		sprintf(ligne, "Start command_JC(%.2s,%.22s)", format, CardTrunc);
	}
	trace_event(ligne, PROCESSING);
	memcpy(buffer_transmit, "JC", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, Tpk, strlen(Tpk));
	buffer_transmit += strlen(Tpk);
	memcpy(buffer_transmit, PinBlock, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, format, 2);
	buffer_transmit += 2;

	format_card_from_pan(Card, Pan);
	memcpy(buffer_transmit, Card, 12);
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "End   command_JC(%.60s)", buffer_transmit);
	else
		sprintf(ligne, "End   command_JC(OK)");
	trace_event(ligne, PROCESSING);

	return (OK);
}


/******************************************************************************/
/* Fonction command_JD.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but d'extraire un PIN sous LMK apres   */
/* translation.                                                */
/******************************************************************************/
int 
	command_JD(char *buffer_received,
	char *PinLmk,
	char *err_code
	)
{
	char            ligne[MAX_LINE_TRC];
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	memset(Command, 0, 3);
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "Start command_JD(%.40s)", buffer_received);
	else
		sprintf(ligne, "Start command_JD()");
	trace_event(ligne, PROCESSING);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received + 2, 2);
	if (memcmp(Command, "JD", 2) != 0) {
		trace_event("End   command_JD(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code, sLine);
	if (memcmp(err_code, "00", 2) != 0) {
		trace_event("End   command_JD(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	/*memcpy(PinLmk, buffer_received, SIZE_KEY);*/
	memcpy(PinLmk, buffer_received, strlen(buffer_received));
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "End   command_JD(%.2s,%.16s)", err_code, PinLmk);
	else
		sprintf(ligne, "End   command_JD(OK)");
	trace_event(ligne, PROCESSING);

	return (OK);
}

/******************************************************************************/
/* Fonction command_JE.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer pour         */
/* translater un PIN sous ZPK vers un PIN sous LMK              */
/******************************************************************************/
int 
	command_JE(char *buffer_transmit,
	char *Zpk,
	char *PinBlock,
	char *format,
	char *Pan
	)
{
	char            Card[13];
	char            ligne[MAX_LINE_TRC];


	if (debug_option == 1)
		sprintf(ligne, "Start command_JE(%s,%.2s,%.22s)", PinBlock, format, Pan);
	else 
	{
		sprintf(ligne, "Start command_JE(%.2s)", format);
	}
	trace_event(ligne, PROCESSING);

	memcpy(buffer_transmit, "JE", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, Zpk, strlen(Zpk));
	buffer_transmit += strlen(Zpk);
	memcpy(buffer_transmit, PinBlock, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, format, 2);
	buffer_transmit += 2;

	format_card_from_pan(Card, Pan);
	memcpy(buffer_transmit, Card, 12);

	trace_event("End   command_JE()", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_JF.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer le PIN sous LMK       */
/******************************************************************************/
int 
	command_JF(char *buffer_received,
	char *pin_lmk,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_JF()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received+2, 2);/*AER23022015 Fix PIN CHANGE ISSUE*/

	if (memcmp(Command, "JF", 2) != 0) {
		trace_event("End   command_JF(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code, sLine);
	if (memcmp(err_code, "00", 2) != 0) {
		trace_event("End   command_JF(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	/*buffer_received += 4;
	memcpy(pin_lmk, buffer_received, SIZE_KEY);i*/

	memcpy(pin_lmk, buffer_received+4,strlen(buffer_received)-4);/*AER23022015 Fix PIN CHANGE ISSUE*/

	trace_event("End   command_JF(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_JG.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de preparer le buffer pour         */
/* translater un PIN sous LMK vers un PIN sous ZPK              */
/******************************************************************************/
int 
	command_JG(char *buffer_transmit,
	char *Zpk,
	char *PinLmk,
	char *format,
	char *Pan
	)
{
	char            Card[13];
	char            ligne[MAX_LINE_TRC];

	memset(Card, 0, sizeof(Card));

	if (debug_option == 1)
		sprintf(ligne, "Start command_JG(%.16s,%.2s,%.22s)", PinLmk, format, Card);
	else
		sprintf(ligne, "Start command_JG()");

	trace_event(ligne, PROCESSING);

	memcpy(buffer_transmit, "JG", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, Zpk, strlen(Zpk));
	buffer_transmit += strlen(Zpk);
	memcpy(buffer_transmit, format, 2);
	buffer_transmit += 2;
	format_card_from_pan(Card, Pan);
	memcpy(buffer_transmit, Card, 12);
	buffer_transmit += 12;
	memcpy(buffer_transmit, PinLmk, strlen(PinLmk));
	buffer_transmit += strlen(PinLmk);

	trace_event("End   command_JG()", PROCESSING);

	return (OK);
}


/******************************************************************************/
/* Fonction command_JH.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer le PINBlock sous ZMK  */
/******************************************************************************/
int 
	command_JH(char *buffer_received,
	char *PinBlock,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_JH()", PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received + 2, 2);

	if (memcmp(Command, "JH", 2) != 0) 
	{
		trace_event("End   command_JH(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}

	TextError(err_code, sLine);

	if (memcmp(err_code, "00", 2) != 0) 
	{
		trace_event("End   command_JH(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}

	buffer_received += 4;

	memcpy(PinBlock, buffer_received, SIZE_KEY);

	trace_event("End   command_JH(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_NC.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de tester le fonctionnament HSM    */
/******************************************************************************/
int 
	command_NC(char *buffer_transmit)
{
	trace_event("Start command_NC()", PROCESSING);

	memcpy(buffer_transmit, "NC", 2);

	trace_event("End   command_NC()", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_ND.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM le resultat du test de fonctionnement.                   */
/******************************************************************************/
int 
	command_ND(char *buffer_received,
	char *lmk_check,
	char *err_code
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_ND()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "ND", 2) != 0) {
		trace_event("End   command_ND(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0) {
		trace_event("End   command_ND(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	memcpy(lmk_check, buffer_received, SIZE_KEY);
	buffer_received += SIZE_KEY;

	trace_event("End   command_ND(OK)", PROCESSING);
	return (OK);
}

/******************************************************************************/
/* Fonction command_DA                                                         */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de verifier un PIN methode IBM     */
/******************************************************************************/
int  command_DA(
	char           *buffer_transmit,
	char *Zpk,char  *Pvka, char *PinZpk, char *format,char  *Pan,char  *offset,
	int  PinLength,char *sDecTable)	
{

	char            Card[13];
	char            vPinLen[2 + 1];	/** MAW20030525 **/

	memset(Card, 0, 13);
	trace_event("Start command_DA", PROCESSING);

	memcpy(buffer_transmit, "DA", 2);
	buffer_transmit += 2;

	memcpy(buffer_transmit, Zpk, strlen(Zpk));
	buffer_transmit += strlen(Zpk);

	memcpy(buffer_transmit, Pvka, strlen(Pvka));
	buffer_transmit += strlen(Pvka);

	memcpy(buffer_transmit, "12", 2);
	buffer_transmit += 2;

	memcpy(buffer_transmit, PinZpk, SIZE_KEY);
	buffer_transmit += SIZE_KEY;

	memcpy(buffer_transmit, format, 2);
	buffer_transmit += 2;
	
	/** memcpy( buffer_transmit, "04", 2);         **/
	sprintf(vPinLen, "%02d", PinLength);
	memcpy(buffer_transmit, vPinLen, 2);
	buffer_transmit += 2;
	

	format_card_from_pan(Card, Pan);
	memcpy(buffer_transmit, Card, 12);
	buffer_transmit += 12;

	/*PROD00053984
	memcpy(buffer_transmit, Validation_data, SIZE_KEY);*/
	memcpy(buffer_transmit, sDecTable, SIZE_KEY);
	buffer_transmit += SIZE_KEY;

	memcpy(buffer_transmit, Pan, 10);
	buffer_transmit += 10;
	memcpy(buffer_transmit, "N", 1);
	buffer_transmit += 1;
	memcpy(buffer_transmit, Pan + (strlen(Pan) - 1), 1);
	buffer_transmit += 1;
	/** START MAW20030525 **/
	/**
	memcpy( buffer_transmit, offset, 4);
	buffer_transmit +=4;
	**/
	memcpy(buffer_transmit, offset, strlen(offset));
	buffer_transmit += strlen(offset);

	/**
	memcpy( buffer_transmit, "FFFFFFFF", 8);
	buffer_transmit +=8;
	**/
	memcpy(buffer_transmit, "FFFFFFFFFFFF", 12 - strlen(offset));
	buffer_transmit += 12 - strlen(offset);
	/** END   MAW20030525 **/

	trace_event("End command_DA()", PROCESSING);
	return (OK);
}
/******************************************************************************/
/* Fonction command_DB.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de donner le resultat de la        */
/* verification du PIN par la methode IBM                       */
/******************************************************************************/
int command_DB(char           *buffer_received,char           *err_code)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "DB", 2) != 0)
		return (ERROR_COMMAND);

	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) == 0 || memcmp(err_code + 2, "02", 2) == 0) /* ZKO PROD00072366*/
		return (OK);
	if (memcmp(err_code + 2, "01", 2) == 0)
		return (VERIFICATION_FAILED);
	return (CRYPTOGRAPHIC_ERROR);
}

/***********************************************************************
**  NAME  : command_KQ
**  DESC  : ARQC Verification and ARPC Generation
**  INPUT : 
**  OUTPUT: 
**  AUTHOR: H.YOUSFI 11/Nov/2003
***********************************************************************/
int 
	command_KQ(char *sHsmBuffer,
	char cSchemeId,
	char *sDmk,
	char *sPanAndSeq,
	char *sAtc,
	char *sUn,
	int nDataLen,
	char *sData,
	char *sArqc,
	char *sArc
	)
{

	int             offset = 0;
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_KQ()", PROCESSING);

	/*Command Code (2A)*/
	memcpy(sHsmBuffer + offset, "KQ", 2);
	offset += 2;

	/*Mode Flag  (1N)*/
	if( nDataLen > 0 )
	{
		sHsmBuffer[offset] = '1';	/** Mode Flag: 1 = Perform ARQC Verification and ARPC generation **/
	}
	else
	{
		sHsmBuffer[offset] = '2'; /** Mode Flag: 2 = Perform only ARPC generation **/
	}


	offset += 1;


	/*Scheme I*D (1N)*/
	sHsmBuffer[offset] = cSchemeId;
	offset += 1;

	/*AC 'S' + nA*/
	memcpy(sHsmBuffer + offset, sDmk, strlen(sDmk));
	offset += strlen(sDmk);

	/*PAN (8B)*/
	memcpy(sHsmBuffer + offset, sPanAndSeq, 8);
	offset += 8;

	/*ATC (2B)*/
	memcpy(sHsmBuffer + offset, sAtc, 2);
	offset += 2;

	/*Unpredictable Number 4B*/
	memcpy(sHsmBuffer + offset, sUn, 4);
	offset += 4;


	/*Transaction Data*/
	if(  sHsmBuffer[2] == '0' || sHsmBuffer[2] == '1' )
	{

		sprintf(sHsmBuffer + offset, "%02X", nDataLen);
		offset += 2;

		memcpy(sHsmBuffer + offset, sData, nDataLen);
		offset += nDataLen;

		memcpy(sHsmBuffer + offset, ";", 1);
		offset += 1;
	}
	/*ARQC (8B)*/
	memcpy(sHsmBuffer + offset, sArqc, 8);
	offset += 8;

	/*ARC (2B)*/
	memcpy(sHsmBuffer + offset, sArc, 2);
	offset += 2;

	sprintf(sLine, "End command_KQ(%d)", offset);
	trace_event(sLine, PROCESSING);
	return (offset);
}
/******************************************************************************/
/* Fonction command_KR.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recupere le buffer              */
/* resultat de verif ARQC                                       */
/******************************************************************************/
int 
	command_KR(char *sBuffRcv,
	char *sErrCode,
	char *sArpc
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_KR()", PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command, sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp(Command, "KR", 2) != 0) {
		trace_event("End   command_ED(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(sErrCode + 2, sLine);
	if (memcmp(sErrCode + 2, "00", 2) == 0) {
		memcpy(sArpc, sBuffRcv + 4, 8);

		trace_event("End   command_KR(OK)", PROCESSING);
		return (OK);
	}
	if (memcmp(sErrCode + 2, "01", 2) == 0) {
		memcpy(sArpc, sBuffRcv + 4, 8);
		trace_event("End   command_KR(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}
	trace_event("End   command_KR(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/* Function command_KW.                                                       */
/* -------------------                                                        */
/* ARQC (or TC/ACC) Verification and/or ARPC Generation (EMV2000)              */
/******************************************************************************/
/*PROD00068163: Start*/
int 
	command_KW(char *sHsmBuffer,
	char mode_flag,
	char cSchemeId,
	char *sDmk,
	char *sPanAndSeq,
	int nPanLength, /*SKA05042023 - PLUTONL-6022*/
	char *sAtc,
	char *sUn,
	int nDataLen,
	char *sData,
	char *sArqc,
	char *sArc,
	char *sCsu 
	)
{

	int             offset = 0;
	char            sLine[MAX_LINE_TRC];
	char			sPanLength[2 + 1];
	

	trace_event("Start command_KW()", PROCESSING);


	memset(sPanLength, '\0', sizeof(sPanLength));

	memcpy(sHsmBuffer + offset, "KW", 2);
	offset += 2;

	sHsmBuffer[offset++] = mode_flag;
	sHsmBuffer[offset++] = cSchemeId;

    
	memcpy(sHsmBuffer + offset, sDmk, strlen(sDmk));
	offset += strlen(sDmk);

	sprintf(sPanLength, "%.2d", nPanLength);
 
	if(  cSchemeId == '0' || cSchemeId == '1')
	{
		memcpy(sHsmBuffer + offset, "0000000000000000", 16);
		offset += 16;
	}
	
	if(  cSchemeId == '1' || cSchemeId == '3' || cSchemeId == '6' || cSchemeId == '7' || cSchemeId == '9')
	{
		memcpy(sHsmBuffer + offset, sPanLength, 2);
		offset += 2;

		memcpy(sHsmBuffer + offset, sPanAndSeq, nPanLength);
		offset += nPanLength;

		sHsmBuffer[offset++] = ';';
	}
	else
	{
		memcpy(sHsmBuffer + offset, sPanAndSeq, 8);
		offset += 8;
	}

	
	if(  cSchemeId == '0' || cSchemeId == '1')
	{
		sHsmBuffer[offset++] = '1';

	}
	
	switch(cSchemeId)
	{
	case '0': 
	case '1': 
	case '2':
	case '3': 
	case '5':
	case '6':
	case '7':
	case '9': 
	case 'B': 
	case 'C':
		memcpy(sHsmBuffer + offset, sAtc, 2);
		offset += 2;
		break;
	}

	if( cSchemeId == 'C' && (mode_flag == '0' || mode_flag == '1') )
	{
		sHsmBuffer[offset++] = '0';	
	}


	if(  mode_flag == '0' || mode_flag == '1' || mode_flag == '3' || mode_flag == '5')
	{
		sprintf(sHsmBuffer + offset, "%02X", nDataLen);
		offset += 2;

		memcpy(sHsmBuffer + offset, sData, nDataLen);
		offset += nDataLen;

		sHsmBuffer[offset++] = ';';
	}

	memcpy(sHsmBuffer + offset, sArqc, 8);
	offset += 8;

	if( mode_flag == '1' || mode_flag == '2' )
	{
		memcpy(sHsmBuffer + offset, sArc, 2);
		offset += 2;
	}

	if(  mode_flag == '3' || mode_flag == '4' || mode_flag == '5' || mode_flag == '6')
	{
		memcpy(sHsmBuffer + offset, sCsu, 4);
		offset += 4;	

		sHsmBuffer[offset++] = '0';
	}



	sprintf(sLine, "End command_KW(%d)", offset);
	trace_event(sLine, PROCESSING);
	return (offset);
}
/*PROD00068163: End*/
/******************************************************************************/
/* Fonction command_KX.                                                       */
/* -------------------                                                        */
/******************************************************************************/
int 
	command_KX(char *sBuffRcv,
	char *sErrCode,
	char *sArpc
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_KX()", PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command, sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp(Command, "KX", 2) != 0) {
		trace_event("End   command_KX(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(sErrCode + 2, sLine);
	if (memcmp(sErrCode + 2, "00", 2) == 0) {
		 memcpy(sArpc, sBuffRcv + 4, 8); /*BJH24042012: BCP CMI CPA */
		/*memcpy(sArpc, sBuffRcv + 4, 4);*//*NAB06042021 PLUTONL-2944*/

		trace_event("End   command_KX(OK)", PROCESSING);
		return (OK);
	}
	if (memcmp(sErrCode + 2, "01", 2) == 0) {
		 memcpy(sArpc, sBuffRcv + 4, 8);/* BJH24042012: BCP CMI CPA */
		/*memcpy(sArpc, sBuffRcv + 4, 4);*//*NAB06042021 PLUTONL-2944*/
		trace_event("End   command_KX(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}
	trace_event("End   command_KX(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);
}


/******************************************************************************/
/* Function command_KU.                                                       */
/* -------------------                                                        */
/******************************************************************************/
int command_KU  (  char *sHsmBuffer,
	char cModeFlag,
	char cSchemeId,
	char *sSMI,
	char *sPanAndSeq,
	char *sIntegSessData,
	int  nPlainTextLen,
	char *sPlainTextData,
	char *sSMC,
	char *sTK,
	char *sConfSessData,
	int  nOffset,
	int  nCipherTextLen,
	char *sCipherTextData,
	char *sSrcPBFrmt,
	char *sDestPBFrmt,
	char *sPan12LastDigits,
	char *sAC,
	char encryption_mode
	)
{

	int     offset = 0;
	char    sLine[MAX_LINE_TRC];

	trace_event("Start command_KU()", PROCESSING);

	/*Command Code*/
	memcpy(sHsmBuffer + offset, "KU",   2);
	offset += 2;

	/*Mode Flag*/
	sHsmBuffer[offset++] = cModeFlag;

	/*Scheme ID*/
	sHsmBuffer[offset++] = cSchemeId ;

	/*SMI*/
	memcpy(sHsmBuffer + offset, sSMI,   strlen(sSMI));
	offset += strlen(sSMI);

	/*PAN/PAN Sequence No*/
	memcpy(sHsmBuffer + offset, sPanAndSeq,   8);
	offset += 8;

	/*Integrity Session Data*/
	memcpy(sHsmBuffer + offset, sIntegSessData,   8);
	offset += 8;

	/*Plaintext Message Data Length*/
	sprintf(sHsmBuffer + offset , "%04X", nPlainTextLen);
	offset += 4;

	/*Plaintext Message */
	memcpy(sHsmBuffer + offset, sPlainTextData,   nPlainTextLen);
	offset += nPlainTextLen;

	sHsmBuffer[offset++] = ';';
	if ( cModeFlag == '4' ) /** PIN Change With Different Key **/
	{
		memcpy(sHsmBuffer + offset, sSMC,   strlen(sSMC));
		offset += strlen(sSMC);

		memcpy(sHsmBuffer + offset, sConfSessData,   8);
		offset += 8;

		sprintf(sHsmBuffer + offset , "%04X", nOffset);
		offset += 4;

		sprintf(sHsmBuffer + offset , "%04X", nCipherTextLen);
		offset += 4;

		memcpy(sHsmBuffer + offset, sCipherTextData,   nCipherTextLen);
		offset += nCipherTextLen;

		/*PROD00053984
		memcpy(sHsmBuffer + offset, ";1",   2); / ** PIN Change uses aLways a TPK on PowerCARD ** /
		offset += 2;
		*/
		sHsmBuffer[offset++] =';';
		if(sTK[0] == 'S')/*ZKO28032019*/
		{
			
			sHsmBuffer[offset++] ='F';
		
		}
		else
		{
			if( encryption_mode == TERMINAL_MODE )
			{
				sHsmBuffer[offset++] ='1';
			}
			else
			{
				sHsmBuffer[offset++] ='0';
			}				
		}


		memcpy(sHsmBuffer + offset, sTK,   strlen(sTK));
		offset += strlen(sTK);

		memcpy(sHsmBuffer + offset, sSrcPBFrmt,   2);
		offset += 2;

		memcpy(sHsmBuffer + offset, sDestPBFrmt,  2);
		offset += 2;

		memcpy(sHsmBuffer + offset, sPan12LastDigits,   12);
		offset += 12;

		if ( 0==memcmp(sDestPBFrmt,"41",2) || 0==memcmp(sDestPBFrmt,"42",2) )
		{
			memcpy(sHsmBuffer + offset, sAC,   strlen(sAC));
			offset += strlen(sAC);
		}

	}

	sprintf(sLine, "End command_KU(%d)", offset);
	trace_event(sLine,  PROCESSING);
	return (offset);
}
/******************************************************************************/
/* Fonction command_KV.                                                       */
/* -------------------                                                        */
/******************************************************************************/
int    command_KV( char *sBuffRcv,
	char cModeFlag,
	char *sErrCode,
	char *sMAC,
	int  *nSecureMsgLen,
	char *sSecureMsgData
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];
	char sTmpData [ LG_MAX ];
	unsigned char sSecDataLen[16];

	trace_event("Start command_KV()",PROCESSING);

	memset(Command, 0, sizeof(Command));
	memset(sTmpData, 0, sizeof(sTmpData));
	memset(sSecDataLen, 0, sizeof(sSecDataLen));

	memcpy(Command , sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp( Command , "KV", 2) != 0)
	{
		trace_event("End   command_KV(ERROR_COMMAND)",PROCESSING);
		return ( ERROR_COMMAND );
	}

	TextError( sErrCode + 2,sLine);
	if (memcmp(sErrCode + 2, "00", 2) == 0)
	{
		memcpy(sMAC, sBuffRcv + 4, 8);

		if( '0' != cModeFlag )
		{
			memcpy(sTmpData, sBuffRcv + 4+8, 4);

			AsciiToHex(sTmpData, sSecDataLen, 4);
			*nSecureMsgLen = (sSecDataLen[0]*256) + sSecDataLen[1];
			memcpy(sSecureMsgData, sBuffRcv + 4+8+4, *nSecureMsgLen);
		}

		trace_event("End   command_KV(OK)",PROCESSING);
		return ( OK );
	}

	trace_event("End   command_KV(CRYPTOGRAPHIC_ERROR)",PROCESSING);
	return(CRYPTOGRAPHIC_ERROR);
}

/******************************************************************************/
/* Fonction command_KY.                                                       */
/* -------------------                                                        */
/*                                                                            */
/******************************************************************************/
int command_KY  (  char *sHsmBuffer,
	char cModeFlag,
	char cSchemeId,
	char *sSMI,
	char *sPanAndSeq,
	char *sIntegSessData,
	int  nPlainTextLen,
	char *sPlainTextData,
	char *sSMC,
	char *sTK,
	char *sConfSessData,
	int  nOffset,
	int  nCipherTextLen,
	char *sCipherTextData,
	char *sSrcPBFrmt,
	char *sDestPBFrmt,
	char *sPan12LastDigits,
	char *sAC,
	char encryption_mode
	)
{

	int     offset = 0;
	char    sLine[MAX_LINE_TRC];

	trace_event("Start command_KY()", PROCESSING);


	memcpy(sHsmBuffer + offset, "KY",   2);
	offset += 2;

	sHsmBuffer[offset] = cModeFlag;

	offset += 1;

	sHsmBuffer[offset] = cSchemeId ;
	offset += 1;

	if (debug_option ==1)
	{
		sprintf(sLine, "==> sSMI=[%s]", sSMI); trace_event(sLine, FATAL);
	}
	memcpy(sHsmBuffer + offset, sSMI,   strlen(sSMI));
	offset += strlen(sSMI);

	memcpy(sHsmBuffer + offset, sPanAndSeq,   8);
	offset += 8;

	memcpy(sHsmBuffer + offset, sIntegSessData,   8);
	offset += 8;

	sprintf(sHsmBuffer + offset , "%04X", nPlainTextLen);
	offset += 4;

	memcpy(sHsmBuffer + offset, sPlainTextData,   nPlainTextLen);
	offset += nPlainTextLen;

	memcpy(sHsmBuffer + offset, ";",   1);
	offset += 1;

	/*if ( cModeFlag == '4' ) ** PIN Change With Different Key ** BJH24042012: BCP CMI CPA */
	if ( (cModeFlag == '2') || (cModeFlag == '4') ) /** PIN Change With Different Key **/
	{
		memcpy(sHsmBuffer + offset, sSMC,   strlen(sSMC));
		offset += strlen(sSMC);

		sprintf(sHsmBuffer + offset , "%04X", nOffset);
		offset += 4;

		sprintf(sHsmBuffer + offset , "%04X", nCipherTextLen);
		offset += 4;

		memcpy(sHsmBuffer + offset, sCipherTextData,   nCipherTextLen);
		offset += nCipherTextLen;

		/* memcpy(sHsmBuffer + offset, ";1",   2); ** PIN Change uses aLways a TPK on PowerCARD ** BJH24042012: BCP CMI CPA */
		memcpy(sHsmBuffer + offset, ";",   1);
		offset += 1;
	}

	if ( cModeFlag == '4' ) /** PIN Change With Different Key **/
	{
		/*
		memcpy(sHsmBuffer + offset, "1",   1); / ** PIN Change uses aLways a TPK on PowerCARD ** /
		offset += 1;	
		*/
		/*PROD00053984 */
		if( encryption_mode == TERMINAL_MODE )
		{
			sHsmBuffer[offset++] ='1';
		}
		else
		{
			sHsmBuffer[offset++] ='0';
		}

		memcpy(sHsmBuffer + offset, sTK,   strlen(sTK));
		offset += strlen(sTK);

		memcpy(sHsmBuffer + offset, sSrcPBFrmt,   2);
		offset += 2;

		memcpy(sHsmBuffer + offset, sDestPBFrmt,  2);
		offset += 2;

		memcpy(sHsmBuffer + offset, sPan12LastDigits,   12);
		offset += 12;

		if ( 0==memcmp(sDestPBFrmt,"41",2) || 0==memcmp(sDestPBFrmt,"42",2) )
		{
			memcpy(sHsmBuffer + offset, sAC,   strlen(sAC));
			offset += strlen(sAC);
		}

	}

	sprintf(sLine, "End command_KY(%d)", offset);
	trace_event(sLine,  PROCESSING);
	return (offset);
}
/******************************************************************************/
/* Fonction command_KZ.                                                       */
/* -------------------                                                        */
/* B.JABALLAH 24/04/2012             BJH24042012                              */
/******************************************************************************/
int    command_KZ( char *sBuffRcv,
	char cModeFlag,
	char *sErrCode,
	char *sMAC,
	int  *nSecureMsgLen,
	char *sSecureMsgData
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];
	char sTmpData [ LG_MAX ];
	unsigned char sSecDataLen[16];

	trace_event("Start command_KZ()",PROCESSING);

	memset(Command, 0, sizeof(Command));
	memset(sTmpData, 0, sizeof(sTmpData));
	memset(sSecDataLen, 0, sizeof(sSecDataLen));

	memcpy(Command , sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp( Command , "KZ", 2) != 0)
	{
		trace_event("End   command_KZ(ERROR_COMMAND)",PROCESSING);
		return ( ERROR_COMMAND );
	}

	TextError( sErrCode + 2,sLine);
	if (memcmp(sErrCode + 2, "00", 2) == 0)
	{
		memcpy(sMAC, sBuffRcv + 4, 8);


		if( '5' != cModeFlag )
		{
			memcpy(sTmpData, sBuffRcv + 4+8, 4);

			AsciiToHex(sTmpData, sSecDataLen, 4);
			*nSecureMsgLen = (sSecDataLen[0]*256) + sSecDataLen[1];
			memcpy(sSecureMsgData, sBuffRcv + 4+8+4, *nSecureMsgLen);
		}

		trace_event("End   command_KZ(OK)",PROCESSING);
		return ( OK );
	}

	trace_event("End   command_KZ(CRYPTOGRAPHIC_ERROR)",PROCESSING);
	return(CRYPTOGRAPHIC_ERROR);
}

/*EBE12115 */
int    command_M6(	unsigned char  *buffer_transmit ,
	unsigned char	mode, 
	unsigned char	format, 
	unsigned char*	algorithm, 
	unsigned char	padMethod,
	unsigned char*	keyTpe,
	int				data_len, 
	unsigned char*	takzak_lmk, 
	unsigned char*	data,
	char*			interValue)
{
	int offset=0;

	trace_event("Start command_M6()", PROCESSING);

	/*Command Code*/
	memcpy ( buffer_transmit , "M6" , 2);
	offset += 2;

	/*Mode Flag */
	buffer_transmit[offset++]=mode;
	/*
	buffer_transmit[offset++]='2';
	memcpy ( buffer_transmit + offset , "03" , 2);
	offset += 2;
	buffer_transmit[offset++]='2';
	memcpy ( buffer_transmit + offset, "008" , 3);
	offset += 3;
	*/
	buffer_transmit[offset++]=format;
	memcpy ( buffer_transmit + offset , algorithm , 2);
	offset += 2;
	buffer_transmit[offset++]=padMethod;
	if( takzak_lmk[0] != 'S' )
	{
		memcpy ( buffer_transmit + offset, keyTpe , 3);
	}
	else
	{
		memcpy ( buffer_transmit + offset, "FFF" , 3);
	}
	offset += 3;

	/*Key*/
	memcpy ( buffer_transmit + offset , takzak_lmk , strlen(takzak_lmk));
	offset +=strlen(takzak_lmk);

	/*intermediate IV*/
	if( mode != '0' && mode != '1')
	{
		memcpy ( buffer_transmit + offset , interValue , 16);
		offset +=16;
	}

	sprintf(buffer_transmit+offset,"%04X",data_len);
	offset += 4;


	memcpy(buffer_transmit + offset, data, data_len);
	offset += data_len;

	trace_event("End command_M6()", PROCESSING);
	return (offset);
}

/************************************************************************/
int    command_M7(	char   *buffer_received , 
	char*	err_code, 
	char*	macData,
	char	mode,
	char*	interValue)
{

	int offset=0;
	trace_event("Start command_M7()", PROCESSING);

	memcpy(err_code, buffer_received + 2, 2);

	if (memcmp(buffer_received, "M7", 2) != 0) return(ERROR_COMMAND);

	if (memcmp(err_code , "00", 2) != 0) return ( CRYPTOGRAPHIC_ERROR );

	offset = 4;
	if( mode == '1' || mode == '2' )
	{
		memcpy(interValue, buffer_received + offset , 16);
		offset +=16;
	}
	/*offset+=4;*/ /*HSM doesn't return the Length field */
	if( memcmp(buffer_received + offset,"0008",4) == 0 )
	{
		offset+=4;
	}
	memcpy(macData, buffer_received + offset , 8);
	trace_event("End command_M7()", PROCESSING);

	return(OK);
}
/************************************************************************/
int    command_M8(	unsigned char  *buffer_transmit ,
	unsigned char	mode, 
	unsigned char	format, 
	unsigned char*	algorithm, 
	unsigned char	padMethod,
	unsigned char*	keyTpe,
	int				data_len, 
	unsigned char*	takzak_lmk, 
	unsigned char*	mac, 
	unsigned char*	data,
	unsigned char*	interValue)
{
	int offset=0;

	trace_event("Start command_M8()", PROCESSING);

	memcpy ( buffer_transmit , "M8" , 2);
	offset += 2;
	buffer_transmit[offset++]=mode;
	/*
	buffer_transmit[offset++]='2';
	memcpy ( buffer_transmit + offset , "03" , 2);
	offset += 2;
	buffer_transmit[offset++]='2';
	memcpy ( buffer_transmit + offset, "008" , 3);
	offset += 3;
	*/
	buffer_transmit[offset++]=format;
	memcpy ( buffer_transmit + offset , algorithm , 2);
	offset += 2;
	buffer_transmit[offset++]=padMethod;
	/*NAB18022019 Key Block*/
    if( takzak_lmk[0] != 'S' ) 
	{
		memcpy ( buffer_transmit + offset, keyTpe , 3);
		
	}
	else
	{
	   memcpy ( buffer_transmit + offset, "FFF" , 3);
	}
	offset += 3;
	memcpy ( buffer_transmit + offset , takzak_lmk , strlen(takzak_lmk));
	offset +=strlen(takzak_lmk);

	if( mode != '0' && mode != '1')
	{
		memcpy ( buffer_transmit + offset , interValue , 16);
		offset +=16;
	}

	sprintf(buffer_transmit+offset,"%04X",data_len);
	offset += 4;


	memcpy(buffer_transmit + offset, data, data_len);
	offset += data_len;

	memcpy ( buffer_transmit + offset , mac , 8);
	offset += 8;

	trace_event("End command_M8()", PROCESSING);
	return (offset);
}

/************************************************************************/
int    command_M9(	char*	buffer_received , 
	char*	err_code,
	char	mode,
	char*	interValue)
{

	trace_event("Start command_M9()", PROCESSING);

	memcpy(err_code, buffer_received + 2, 2);

	if (memcmp(buffer_received, "M9", 2) != 0) return(ERROR_COMMAND);

	if (memcmp(err_code , "00", 2) != 0) return ( CRYPTOGRAPHIC_ERROR );


	if( mode == '1' || mode == '2')
	{
		memcpy ( interValue , buffer_received + 4, 16);
	}

	trace_event("End command_M9()", PROCESSING);

	return(OK);
}
/*****************************************************************************\
* A0 - To generate a key and optionally encrypt key under ZMK for transmission
\*****************************************************************************/
int    command_A0(	char		*buffer_transmit ,
	char		mode,
	char*		keyType,
	
	char*       KeyUsage,
	char*       Algorithm,
	char*       ModeOfUse,
	char*       KeyVersionNumber,
	char*       Exportability,
	char*		lmk_id,

	char		zmk_flag,
	char*       zmkLmk,
	char        keyLen,
	char*       keySchemeZmk, /* AHA20220927 PLUTONL-4980*/
	char*       KeyBlockVersionId /* AHA20220927 PLUTONL-4980*/

	)
{


	trace_event("Start command_A0()", PROCESSING);

	memcpy(buffer_transmit, "A0", 2);           
	buffer_transmit +=2;
	memcpy(buffer_transmit, &mode,1);           
	buffer_transmit +=1;

	memcpy(buffer_transmit, keyType, 3); 
	buffer_transmit +=3;

	if( strcmp(keyType,"FFF") != 0 )
	{
		memcpy(buffer_transmit, "U",1);
		if(keyLen == KEY_LEN_SIMPLE)
		{
			memcpy(buffer_transmit, "Z",1);
		}
		buffer_transmit +=1;
		memcpy(buffer_transmit, ";",1);
		buffer_transmit +=1;
		memcpy(buffer_transmit, &zmk_flag,1);
		buffer_transmit +=1;
		memcpy(buffer_transmit, zmkLmk, strlen(zmkLmk)); 
		buffer_transmit +=strlen(zmkLmk);
		memcpy(buffer_transmit, "X",1);
		if(keyLen == KEY_LEN_SIMPLE)
		{
			memcpy(buffer_transmit, "Z",1);
		}
		buffer_transmit +=1;
	}
	else
	{	
		/* start AHA20220927 PLUTONL-4980*/
		if( keySchemeZmk[0] == 'R')
		{
			memcpy(buffer_transmit, "S",1);
			buffer_transmit +=1;
			memcpy(buffer_transmit, zmkLmk, strlen(zmkLmk)); 
			buffer_transmit +=strlen(zmkLmk);
			memcpy(buffer_transmit, "R",1);
			buffer_transmit +=1;
			/*LMK Delimiter*/
			memcpy(buffer_transmit , "%", 1);
			buffer_transmit += 1;/*NAB18022019 Key Block*/
			/*LMK Identifier*/
			memcpy(buffer_transmit, lmk_id, 2);/*NAB18022019 Key Block*/
			buffer_transmit += 2;

			/*Delimiter*/
			memcpy(buffer_transmit, "#", 1);
			buffer_transmit += 1;

			/*Key Usage  */
			memcpy(buffer_transmit, KeyUsage, 2);
			buffer_transmit += 2;

			/*Algorithm*/
			memcpy(buffer_transmit, Algorithm, 2);
			buffer_transmit += 2;

			/*Mode Of Use*/
			memcpy(buffer_transmit, ModeOfUse, 1);
			buffer_transmit += 1;

			/*Key Version Number*/
			memcpy(buffer_transmit, KeyVersionNumber, 2);
			buffer_transmit += 2;

			/*Exportability* E : maybe exported in a trusted key block ou N :no export?*/
			memcpy(buffer_transmit, Exportability ,1 );
			buffer_transmit += 1;

			/*Number of optional blocks*/
			memcpy(buffer_transmit , "00", 2);
			buffer_transmit += 2;

			/*Delimiter */
			memcpy(buffer_transmit , "&", 1);
			buffer_transmit += 1;

			/*Mode of Use */
			memcpy(buffer_transmit , "N", 1);
			buffer_transmit += 1;
			/*Delimiter */
			memcpy(buffer_transmit , "!", 1);
			buffer_transmit += 1;

			/*key block version id */
			memcpy(buffer_transmit , KeyBlockVersionId, 1);
			buffer_transmit += 1;
		}
		else if (keySchemeZmk[0]== 'X')
		{
			memcpy(buffer_transmit, "S",1);
			buffer_transmit +=1;
			memcpy(buffer_transmit, zmkLmk, strlen(zmkLmk)); 
			buffer_transmit +=strlen(zmkLmk);
			memcpy(buffer_transmit, "X",1);
			buffer_transmit +=1;
			/*LMK Delimiter*/
			memcpy(buffer_transmit , "%", 1);
			buffer_transmit += 1;/*NAB18022019 Key Block*/
			/*LMK Identifier*/
			memcpy(buffer_transmit, lmk_id, 2);/*NAB18022019 Key Block*/
			buffer_transmit += 2;

			/*Delimiter*/
			memcpy(buffer_transmit, "#", 1);
			buffer_transmit += 1;

			/*Key Usage  */
			memcpy(buffer_transmit, KeyUsage, 2);
			buffer_transmit += 2;

			/*Algorithm*/
			memcpy(buffer_transmit, Algorithm, 2);
			buffer_transmit += 2;

			/*Mode Of Use*/
			memcpy(buffer_transmit, ModeOfUse, 1);
			buffer_transmit += 1;

			/*Key Version Number*/
			memcpy(buffer_transmit, KeyVersionNumber, 2);
			buffer_transmit += 2;

			/*Exportability* E : maybe exported in a trusted key block ou N :no export?*/
			memcpy(buffer_transmit, Exportability ,1 );
			buffer_transmit += 1;

			/*Number of optional blocks*/
			memcpy(buffer_transmit , "00", 2);
			buffer_transmit += 2;

		}
		else
		{
			/*ERROR*/
			trace_event("Error In key Scheme ZMK", ERROR);
			trace_event("Err. In command_A0(NOK)", ERROR);
		    return(ERROR_COMMAND);
		}
		/* end AHA20220927 PLUTONL-4980*/
	}

	trace_event("End   command_A0(OK)", PROCESSING);
	return(OK);
}

int    command_A1(	char *buffer_received,
	char mode,
	char *keyLmk,
	char *keyZmk, 
	char *check_value 
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];
	int		nKeyLen;
	char	sKeyLenStr[32];
	char    zmk_key_scheme;/* AHA20220927 PLUTONL-4980*/

	trace_event("Start command_A1()",PROCESSING);


	if (memcmp(buffer_received, "A1", 2) != 0) 
	{
		trace_event("End   command_A1(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	TextError( buffer_received + 2,sLine);
	if (memcmp(buffer_received + 2 , "00", 2) != 0) 
	{
		trace_event("End   command_A1(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
	}

	buffer_received += 4;
	switch (buffer_received[0])
	{
	case 'U' :
	case 'X' :
		memcpy(keyLmk, buffer_received,  2 * SIZE_KEY + 1); 
		buffer_received +=( 2 * SIZE_KEY + 1);
		keyLmk [ 2 * SIZE_KEY + 1] = '\0';
		if( mode == '1' )
		{
			memcpy(keyZmk, buffer_received,  2 * SIZE_KEY + 1); 
			buffer_received +=( 2 * SIZE_KEY + 1);
			keyZmk [ 2 * SIZE_KEY + 1] = '\0';
		}
		break;
	case 'T' :
	case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
		memcpy(keyLmk, buffer_received,  3 * SIZE_KEY + 1); 
		buffer_received +=( 3 * SIZE_KEY + 1);
		keyLmk [ 3 * SIZE_KEY + 1] = '\0'; 
		if( mode == '1' )
		{
			memcpy(keyZmk, buffer_received,  3 * SIZE_KEY + 1); 
			buffer_received +=( 3 * SIZE_KEY + 1);
			keyZmk [ 3 * SIZE_KEY + 1] = '\0';
		}
		break;
	case 'S':
		trace_event("Keyblock Encryption Length",TRACE);
		memcpy(sKeyLenStr,buffer_received + 2,4);
		nKeyLen = atoi(sKeyLenStr) + 1;
		memcpy(keyLmk, buffer_received,nKeyLen);
		keyLmk [nKeyLen] = '\0';
		buffer_received += nKeyLen;
		if( mode == '1' )
		{  /*start AHA20220927 PLUTONL-4980*/
			zmk_key_scheme=buffer_received[0];
			if( zmk_key_scheme == 'R' )
			{
				memcpy(sKeyLenStr,buffer_received + 2,4);
				nKeyLen = atoi(sKeyLenStr) + 1;
				memcpy(keyZmk, buffer_received,  nKeyLen); 
				buffer_received +=(nKeyLen);
				keyZmk [nKeyLen] = '\0';
			}
			else if (zmk_key_scheme=='X')
			{
				memcpy(keyZmk, buffer_received,  2 * SIZE_KEY + 1);
				buffer_received +=( 2 * SIZE_KEY + 1);
				keyZmk [ 2 * SIZE_KEY + 1] = '\0';
			}
			else
			{
				trace_event("Error In Key Scheme ZMK",ERROR);
				trace_event("End   command_A1(ERROR_COMMAND)",PROCESSING);
				return(ERROR_COMMAND);
			}
			/* end AHA20220927 PLUTONL-4980*/
		}
		break;
	default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
		memcpy(keyLmk, buffer_received, SIZE_KEY); 
		buffer_received +=SIZE_KEY;
		keyLmk [SIZE_KEY] = '\0';
		if( mode == '1' )
		{
			memcpy(keyZmk, buffer_received,  SIZE_KEY); 
			buffer_received +=SIZE_KEY;
			keyZmk [SIZE_KEY] = '\0';
		}
	}

	memcpy(check_value, buffer_received, 6);
	check_value[6]='\0';

	trace_event("End   command_A1(OK)",PROCESSING);
	return(OK);
}

/*****************************************************************************\
* A6 - To import a key encrypted under a ZMK
\*****************************************************************************/
int    command_A6(	char		*buffer_transmit ,
	char*		keyType,
	char*		zmkLmk,
	char*		keyZmk,

	char*       KeyUsage,
	/*char*       Algorithm,*//*NAB 18022019 Key Block*/
	char*       ModeOfUse,
	char*       KeyVersionNumber,
	char*       Exportability,
	char*		lmk_id,

	char*		attala_var
	)
{

	int		nLength;
	char	sLine[MAX_LINE_TRC];
	int     offset;

	trace_event("Start command_A6()", PROCESSING);

	offset = 0;

	memcpy(buffer_transmit + offset, "A6", 2);           
	offset +=2;

	memcpy(buffer_transmit + offset, keyType, 3); 
	offset +=3;
	
	memcpy(buffer_transmit + offset, zmkLmk, strlen(zmkLmk)); 
	offset +=strlen(zmkLmk);
	
	if ( strlen( keyZmk  ) == 2*SIZE_KEY)
	{
		memcpy(buffer_transmit + offset,"X" ,1 );
		offset +=1;
	}
	memcpy(buffer_transmit + offset, keyZmk, strlen(keyZmk)); 
	offset +=strlen(keyZmk);
	
	if( strcmp(keyType,"FFF") != 0 )
	{
		memcpy(buffer_transmit + offset, "U",1);           
		offset +=1;
	}
	else
	{
		memcpy(buffer_transmit + offset, "S",1);
		offset +=1;
	}

	if( attala_var && strlen(attala_var) > 0 )
	{
		nLength = strlen(attala_var);
		if( nLength > 2 )
		{
			nLength = 2;
		}
		memcpy(buffer_transmit + offset, attala_var,nLength);           
		offset +=nLength;
	}

	if( strcmp(keyType,"FFF") == 0 )
	{
		/*LMK Delimiter*/
		memcpy(buffer_transmit + offset, "%", 1);
		offset += 1;/*NAB18022019 Key Block*/
		
		/*LMK Identifier*/
		memcpy(buffer_transmit + offset, lmk_id, 2);
		offset += 2;
	
		/*Delimiter*/
		memcpy(buffer_transmit + offset, "#", 1);
		offset += 1;
		/*Key Usage  */
		memcpy(buffer_transmit + offset, KeyUsage, 2);
		offset += 2;
				
		if ( keyZmk[0]=='R' )  /* FZL 20220822 PLUTONL-4979*/
		{
			memcpy(buffer_transmit + offset, "00", 2);
			offset += 2; 	
		}
		else 
		{
			/*Mode of Use*/
			memcpy(buffer_transmit + offset, ModeOfUse, 1);
			offset += 1;
			/*Key Version Number*/
			memcpy(buffer_transmit + offset, KeyVersionNumber, 2);
			offset += 2;
			/*Exportability*/
			memcpy(buffer_transmit + offset, Exportability, 1);
			offset += 1;
			/*Number of optional blocks*/
			memcpy(buffer_transmit + offset, "00", 2);
			offset += 2;
		}

	}

	trace_event("End   command_A6(OK)", PROCESSING);
	return(OK);
}

int    command_A7(	char *buffer_received,
	char *keyLmk,
	char *check_value 
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];
	int		nKeyLen;
	char	sKeyLenStr[32];


	trace_event("Start command_A7()",PROCESSING);

        memset(sKeyLenStr,    0, sizeof(sKeyLenStr));

	if (memcmp(buffer_received, "A7", 2) != 0) 
	{
		trace_event("End   command_A7(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	TextError( buffer_received + 2,sLine);
	if (memcmp(buffer_received + 2 , "00", 2) != 0) 
	{
		trace_event("End   command_A7(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
	}

	buffer_received += 4;
	switch (buffer_received[0])
	{
	case 'U' :
	case 'X' :
		memcpy(keyLmk, buffer_received,  2 * SIZE_KEY + 1); 
		buffer_received +=( 2 * SIZE_KEY + 1);
		keyLmk [ 2 * SIZE_KEY + 1] = '\0';
		break;
	case 'T' :
	case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
		memcpy(keyLmk, buffer_received,  3 * SIZE_KEY + 1); 
		buffer_received +=( 3 * SIZE_KEY + 1);
		keyLmk [ 3 * SIZE_KEY + 1] = '\0'; 
		break;
	case 'S':
		trace_event("Keyblock Encryption Length",TRACE);
		memcpy(sKeyLenStr,buffer_received + 2,4);
		nKeyLen = atoi(sKeyLenStr) + 1;
		memcpy(keyLmk, buffer_received,nKeyLen);
		keyLmk [nKeyLen] = '\0';
		buffer_received += nKeyLen;
		break;

	
	default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
		memcpy(keyLmk, buffer_received, SIZE_KEY); 
		buffer_received +=SIZE_KEY;
		keyLmk [SIZE_KEY] = '\0';
	} 

    memcpy(check_value, buffer_received, 6);
	check_value[6]='\0';
	trace_event("End   command_A7(OK)",PROCESSING);
	return(OK);
}
/*****************************************************************************\
* SU - Import ZPK or ZAK under KTK
\*****************************************************************************/
int    command_SU(	char		*buffer_transmit ,
	char		mode,
	char*		keyKtk,
	char*		ktkLmk
	)
{


	trace_event("Start command_SU()", PROCESSING);

	memcpy(buffer_transmit, "SU", 2);           
	buffer_transmit +=2;
	memcpy(buffer_transmit, &mode,1);           
	buffer_transmit +=1;
	memcpy(buffer_transmit, keyKtk, strlen(keyKtk)); 
	buffer_transmit +=strlen(keyKtk);
	memcpy(buffer_transmit, ktkLmk, strlen(ktkLmk)); 
	buffer_transmit +=strlen(ktkLmk);

	trace_event("End   command_SU(OK)", PROCESSING);
	return(OK);
}

int    command_SV( char *buffer_received, 
	char *keyLmk,
	char *check_value , 
	char *err_code 
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];

	trace_event("Start command_SV()",PROCESSING);

	memset(Command,    0, 3);
	memset(keyLmk,    0, SIZE_KEY);
	memset(check_value,0, SIZE_KEY);

	memcpy(Command , buffer_received, 2);      
	memcpy(err_code, buffer_received, 4);   
	if (memcmp(Command, "SV", 2) != 0) 
	{
		trace_event("End   command_SV(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	TextError( err_code + 2,sLine);
	if (memcmp(err_code + 2 , "00", 2) != 0) 
	{
		trace_event("End   command_SV(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
	}

	buffer_received += 4;
	switch (buffer_received[0])
	{
	case 'U' :
	case 'X' :
		memcpy(keyLmk, buffer_received,  2 * SIZE_KEY + 1); 
		buffer_received +=( 2 * SIZE_KEY + 1);
		keyLmk [ 2 * SIZE_KEY + 1] = '\0'; 
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	case 'T' :
	case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
		memcpy(keyLmk, buffer_received,  3 * SIZE_KEY + 1); 
		buffer_received +=( 3 * SIZE_KEY + 1);
		keyLmk [ 3 * SIZE_KEY + 1] = '\0'; 
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
		memcpy(keyLmk, buffer_received, SIZE_KEY); 
		buffer_received +=SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
		keyLmk [SIZE_KEY] = '\0';
	}

	trace_event("End   command_SV(OK)",PROCESSING);
	return(OK);
}
/*****************************************************************************\
* SW - Export ZPK or ZAK under KTK
\*****************************************************************************/
int    command_SW(	char		*buffer_transmit ,
	char		mode,
	char*		keyLmk,
	char*		ktkLmk
	)
{


	trace_event("Start command_SW()", PROCESSING);

	memcpy(buffer_transmit, "SW", 2);           
	buffer_transmit +=2;
	memcpy(buffer_transmit, &mode,1);           
	buffer_transmit +=1;
	memcpy(buffer_transmit, keyLmk, strlen(keyLmk)); 
	buffer_transmit +=strlen(keyLmk);
	memcpy(buffer_transmit, ktkLmk, strlen(ktkLmk)); 
	buffer_transmit +=strlen(ktkLmk);
	buffer_transmit[0] ='X';

	trace_event("End   command_SW(OK)", PROCESSING);
	return(OK);
}

int    command_SX( char *buffer_received, 
	char *keyKtk,
	char *check_value , 
	char *err_code 
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];

	trace_event("Start command_SX()",PROCESSING);

	memset(Command,    0, 3);
	memset(keyKtk,    0, SIZE_KEY);
	memset(check_value,0, SIZE_KEY);

	memcpy(Command , buffer_received, 2);      
	memcpy(err_code, buffer_received, 4);   
	if (memcmp(Command, "SX", 2) != 0) 
	{
		trace_event("End   command_SX(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	TextError( err_code + 2,sLine);
	if (memcmp(err_code + 2 , "00", 2) != 0) 
	{
		trace_event("End   command_SX(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
	}

	buffer_received += 4;
	switch (buffer_received[0])
	{
	case 'U' :
	case 'X' :
		memcpy(keyKtk, buffer_received,  2 * SIZE_KEY + 1); 
		buffer_received +=( 2 * SIZE_KEY + 1);
		keyKtk [ 2 * SIZE_KEY + 1] = '\0'; 
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	case 'T' :
	case 'Y' :
		trace_event("Triple DES Encryption Triple Length",TRACE);
		memcpy(keyKtk, buffer_received,  3 * SIZE_KEY + 1); 
		buffer_received +=( 3 * SIZE_KEY + 1);
		keyKtk [ 3 * SIZE_KEY + 1] = '\0'; 
		memcpy(check_value, buffer_received, SIZE_KEY);
		break;
	default  : 
		trace_event("Single DES Encryption Single Length",TRACE);
		memcpy(keyKtk, buffer_received, SIZE_KEY); 
		buffer_received +=SIZE_KEY;
		memcpy(check_value, buffer_received, SIZE_KEY);
		keyKtk [SIZE_KEY] = '\0';
	}

	trace_event("End   command_SX(OK)",PROCESSING);
	return(OK);
}


/* START MJA21092011 CONTACTLESS  */
/***********************************************************************
**  NAME  : command_PM
**  DESC  : CVC3 Verification
***********************************************************************/

int command_PM (char *sHsmBuffer,
	char cSchemeId,
	char cVersion,
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
	char *sCvc3)
{

	int             offset = 0;
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_PM()", PROCESSING);

	memcpy(sHsmBuffer + offset, "PM", 2); offset += 2;

	sprintf(sHsmBuffer + offset, "%c", cSchemeId); offset += 1;

	sprintf(sHsmBuffer + offset, "%c", cVersion); offset += 1;

	memcpy(sHsmBuffer + offset, sMkcvc3, strlen(sMkcvc3)); offset += strlen(sMkcvc3);

	memcpy(sHsmBuffer + offset, "A", 1); offset += 1;

	memcpy(sHsmBuffer + offset, sPan, strlen(sPan)); offset += strlen(sPan);

	memcpy(sHsmBuffer + offset, ";", 1); offset += 1;

	switch(cSchemeId)
	{
	case '0':
		memcpy(sHsmBuffer + offset, sExpiryDate, 4); offset += 4;
		memcpy(sHsmBuffer + offset, sServiceCode, 3); offset += 3;
		memcpy(sHsmBuffer + offset, sAtc, 6); offset += 6;
		memcpy(sHsmBuffer + offset, sDcvv, 3); offset += 3;
		break;

	case '1':
		if (cVersion=='1' || cVersion == '2')
		{
			if(sPanSeqNo != NULL)
			{
				memcpy(sHsmBuffer + offset, sPanSeqNo + 1, 2); offset += 2;
			}
			else
			{
				memcpy(sHsmBuffer + offset, "00", 2);
				offset += 2;
			}
		}
		if (cVersion=='1' || cVersion == '0')
		{
			memcpy(sHsmBuffer + offset, sIvcvc3, 5); offset += 5;
		}

		if (cVersion == '2')
		{
			sprintf(sHsmBuffer + offset, "%03d", nDataLen); offset += 3;

			memcpy(sHsmBuffer + offset, sData, nDataLen); offset += nDataLen;
			memcpy(sHsmBuffer + offset, sUn, 8); offset += 8;
			memcpy(sHsmBuffer + offset, sAtc, 5); offset += 5;
			memcpy(sHsmBuffer + offset, sCvc3, 5); offset += 5;
		}
		break;
	}

	if (debug_option == 1)
		sprintf(sLine, "End command_PM(%d), HsmBuffer (%s) ", offset, sHsmBuffer);
	else
		sprintf(sLine, "End command_PM(offset %d) ", offset);
	trace_event(sLine, PROCESSING);
	return (offset);
}
/******************************************************************************/
/* Fonction command_PN.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recupere le buffer              */
/* resultat de verif CVC3                                       */
/******************************************************************************/
int
	command_PN(char *sBuffRcv,
	char *sErrCode
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_PN()", PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command, sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp(Command, "PN", 2) != 0) 
	{
		trace_event("End   command_PN(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}

	TextError(sErrCode + 2, sLine);
	if (memcmp(sErrCode + 2, "00", 2) == 0) 
	{
		trace_event("End   command_PN(OK)", PROCESSING);
		return (OK);
	}
	if (memcmp(sErrCode + 2, "01", 2) == 0) 
	{
		trace_event("End   command_PN(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_PN(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);
}
/* END MJA21092011 CONTACTLESS  */




/***********************************************************************
***  NAME  : command_JS
***  DESC  : ARQC Verification and/or ARPC Generation (CUP)
***  INPUT : 
***  OUTPUT: 
***  13/06/2017
************************************************************************/
int 
	command_JS(char *sHsmBuffer,
	char *sDmk,
	char *sPanAndSeq,
	char *sAtc,
	int nDataLen,
	char *sData,
	char *sArqc,
	char *sArc
	)
{

	int             offset = 0;
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_JS()", PROCESSING);

	/*Command Code*/
	memcpy(sHsmBuffer + offset, "JS", 2);
	offset += 2;

	/*Mode Flag*/

	if( nDataLen > 0 )
	{
		sHsmBuffer[offset] = '1';	/** Mode Flag: 1 = Perform ARQC Verification and ARPC generation **/
	}
	else
	{
		sHsmBuffer[offset] = '2'; /** Mode Flag: 2 = Perform only ARPC generation **/
	}


	offset += 1;

	/*Scheme ID*/

	sHsmBuffer[offset] = '1';
	offset += 1;

	/*MK-AC(LMK)*/
	memcpy(sHsmBuffer + offset, sDmk, strlen(sDmk));
	offset += strlen(sDmk);

	/*PAN/PAN Sequence No*/
	memcpy(sHsmBuffer + offset, sPanAndSeq, 8);
	offset += 8;

	/*ATC*/
	memcpy(sHsmBuffer + offset, sAtc, 2);
	offset += 2;

	/*Padding Flag*/
	sHsmBuffer[offset] = '0';
	offset += 1;


	if(  sHsmBuffer[2] == '0' || sHsmBuffer[2] == '1' )
	{
		/*Transaction Data Length*/
		sprintf(sHsmBuffer + offset, "%02X", nDataLen);
		offset += 2;

		/*Transaction Data*/
		memcpy(sHsmBuffer + offset, sData, nDataLen);
		offset += nDataLen;

		/*Delimiter*/
		memcpy(sHsmBuffer + offset, ";", 1);
		offset += 1;
	}

	/*ARQC*/
	memcpy(sHsmBuffer + offset, sArqc, 8);
	offset += 8;
	if(  sHsmBuffer[2] == '1' || sHsmBuffer[2] == '2' )
	{
		memcpy(sHsmBuffer + offset, sArc, 2);
		offset += 2;
	}

	sprintf(sLine, "End command_JS(%d)", offset);
	trace_event(sLine, PROCESSING);
	return (offset);
}
/******************************************************************************/
/* Fonction command_JT.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recupere le buffer              */
/* resultat de verif ARQC                                       */
/******************************************************************************/
int 
	command_JT(char *sBuffRcv,
	char *sErrCode,
	char *sArpc
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_JT()", PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command, sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp(Command, "JT", 2) != 0) {
		trace_event("End   command_JT(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(sErrCode + 2, sLine);
	if (memcmp(sErrCode + 2, "00", 2) == 0) {
		memcpy(sArpc, sBuffRcv + 4, 8);

		trace_event("End   command_JT(OK)", PROCESSING);
		return (OK);
	}
	if (memcmp(sErrCode + 2, "01", 2) == 0) {
		memcpy(sArpc, sBuffRcv + 4, 8);
		trace_event("End   command_JT(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}
	trace_event("End   command_JT(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);
}
/*zkorachi: TODO Command_JU()*/

/***********************************************************************
**  NAME  : command_BU
**  DESC  : Generate a Key Check Value
***********************************************************************/

int command_BU (char *sHsmBuffer,
	char *KeyTypeCode,
	char cKeyLengthFlag,
	char *sKey,
	char *sKeyType
	)
{

	int             offset = 0;
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_BU()", PROCESSING);

	memcpy(sHsmBuffer + offset, "BU", 2); offset += 2;
	if( sKey[0] == 'S' )
	{
		memcpy(sHsmBuffer + offset, "FFF", 3); offset += 3;
		memcpy(sHsmBuffer + offset, sKey, strlen(sKey)); 
		offset += strlen(sKey); 
		memcpy(sHsmBuffer + offset, ";FFF", 4); /*ZKO020142019*/
		offset += 4; 
	}
	else
	{
		memcpy(sHsmBuffer + offset, KeyTypeCode, 2); offset += 2;
		sprintf(sHsmBuffer + offset, "%c", cKeyLengthFlag); offset += 1;
		memcpy(sHsmBuffer + offset, sKey, strlen(sKey)); offset += strlen(sKey);

		if (strlen(sKeyType) == 3)
		{
			memcpy(sHsmBuffer + offset, ";", 1); offset += 1;
			memcpy(sHsmBuffer + offset, sKeyType, 3); offset += 3;
			memcpy(sHsmBuffer + offset, ";", 1); offset += 1;
			memcpy(sHsmBuffer + offset, "0", 1); offset += 1;
			memcpy(sHsmBuffer + offset, "0", 1); offset += 1;
			memcpy(sHsmBuffer + offset, "0", 1); offset += 1; /*  backwards compatible and produces a 16 hex KCV*/
		}
	}


	sprintf(sLine, "End command_BU(%d), HsmBuffer (%s) ", offset, sHsmBuffer);
	trace_event(sLine, PROCESSING);
	return (offset);
}

/******************************************************************************/
/* Fonction command_BV.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recupere le buffer              */
/* resultat de BU					                                         */
/******************************************************************************/
int
	command_BV(char *sBuffRcv,
	char *sKeyCheckValue
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];
	char            sErrCode[256 +1 ];

	trace_event("Start command_BV()", PROCESSING);

	memset(Command,  0, sizeof(Command));
	memset(sErrCode, 0, sizeof(sErrCode));
	memset(sLine,    0, sizeof(sLine));

	memcpy(Command, sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);

	if (memcmp(Command, "BV", 2) != 0) 
	{
		trace_event("End   command_BV(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}

	TextError(sErrCode + 2, sLine);

	if (memcmp(sErrCode + 2, "00", 2) != 0) 
	{
		trace_event("End   command_BV(NOK)", PROCESSING);
		return (NOK);
	}

	if (strlen(sBuffRcv) -4 >= 16)
		memcpy (sKeyCheckValue, sBuffRcv + 4, 16);
	else
		memcpy (sKeyCheckValue, sBuffRcv + 4, strlen(sBuffRcv) -4);

	trace_event("End   command_BV(OK)", PROCESSING);
	return (OK);
}


/*****************************************************************************\
* EY - Validate a signature on a message using a public key.
\*****************************************************************************/
int    command_EY(      char            *buffer_transmit ,
	char			*hash_id,
	char			*sig_id,
	char			*pad_mode_id,
	char            *pData,
	char            *pSignature,
	char            *pPublicKey
	)
{


	int           nKeySize, nOffset;
	char          sKeySizeStr[4+1];


	nOffset = 0;
	memset(sKeySizeStr,  0, sizeof(sKeySizeStr));

	trace_event("Start command_EY()", PROCESSING);

	memcpy(buffer_transmit, "EY", 2);
	nOffset +=2;

	memcpy(buffer_transmit + nOffset, hash_id, 2);
	nOffset +=2;

	memcpy(buffer_transmit + nOffset, sig_id, 2);
	nOffset +=2;

	memcpy(buffer_transmit + nOffset, pad_mode_id, 2);
	nOffset +=2;

	nKeySize = strlen(pSignature);
	nKeySize = AsciiToHex(pSignature, buffer_transmit + nOffset + 4, nKeySize);

	sprintf(sKeySizeStr, "%04d", nKeySize);
	memcpy(buffer_transmit + nOffset, sKeySizeStr, 4);
	nOffset += 4;
	nOffset += nKeySize;

	buffer_transmit[nOffset++]=';';


	nKeySize = strlen(pData);
	nKeySize = AsciiToHex(pData, buffer_transmit + nOffset + 4, nKeySize);
	sprintf(sKeySizeStr, "%04d", nKeySize);
	memcpy(buffer_transmit + nOffset, sKeySizeStr, 4);
	nOffset += 4;
	nOffset += nKeySize;


	buffer_transmit[nOffset++]=';';

	nKeySize = strlen(pPublicKey);
	
	if(pPublicKey[0]== 'S')/*ZKO28032019*/
	{	
		memcpy(buffer_transmit + nOffset, pPublicKey, 1 + 16);
		nOffset += 17;
		nKeySize = AsciiToHex(pPublicKey + 17, buffer_transmit + nOffset, nKeySize - 17);
	}
	else
	{   
		nKeySize = AsciiToHex(pPublicKey, buffer_transmit + nOffset, nKeySize);
		
	}
	
	nOffset += nKeySize;

	trace_event("End   command_EY(OK)", PROCESSING);
	return(nOffset);
}


int    command_EZ(      char *buffer_received)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];

	trace_event("Start command_EZ()",PROCESSING);

	if (memcmp(buffer_received, "EZ", 2) != 0)
	{
		trace_event("End   command_EZ(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	if (memcmp(buffer_received + 2, "00", 2) == 0) 
	{
		trace_event("End   command_EZ(OK)", PROCESSING);
		return (OK);
	}

	if( memcmp(buffer_received + 2, "01", 2) == 0 || memcmp(buffer_received + 2, "02", 2) == 0) 
	{
		trace_event("End   command_EZ(VERIFICATION_FAILED)", PROCESSING);
		return (VERIFICATION_FAILED);
	}

	trace_event("End   command_EZ(CRYPTOGRAPHIC_ERROR)", PROCESSING);
	return (CRYPTOGRAPHIC_ERROR);


}

/***********************************************************************
**  NAME  : command_EW
**  DESC  : Generate a Signature
***********************************************************************/
int           command_EW (unsigned char    *sHsmBuffer,
	char             *sHashIdentifier,
	char             *sSignatureIdentifier,
	char             *sPadModeIdentifier,
	unsigned char    *sMessageData,
	int              nMessageDataLen,
	char             *sSecretKeyFlag,
	/*char             *sSecretKeyLen,
	unsigned char    *sSecretKey*/
	 char    		 *sSecretKey
	
	)
{

	int             offset = 0, nKeyLen;
	char            sLine[MAX_LINE_TRC];
	char			sDataLength[4 + 1];

	trace_event("Start command_EW()", PROCESSING);

	memcpy(sHsmBuffer + offset, "EW", 2); offset += 2;

	memcpy(sHsmBuffer + offset, sHashIdentifier, 2);      offset += 2;
	memcpy(sHsmBuffer + offset, sSignatureIdentifier, 2); offset += 2;
	memcpy(sHsmBuffer + offset, sPadModeIdentifier, 2);   offset += 2;

	sprintf(sDataLength,    "%04d", nMessageDataLen);

	memcpy(sHsmBuffer + offset, sDataLength, 4); 					          offset += 4;
	memcpy(sHsmBuffer + offset, sMessageData, nMessageDataLen);				  offset += nMessageDataLen;
	memcpy(sHsmBuffer + offset, ";", 1);                                      offset += 1;
	memcpy(sHsmBuffer + offset, sSecretKeyFlag, 2);                           offset += 2;
	

	if (memcmp(sSecretKeyFlag, "99", 2) == 0)
	{
		nKeyLen = strlen(sSecretKey);
		
		if( sSecretKey[0] == 'S' )
		{
			memcpy(sHsmBuffer + offset, "FFFF", 4);               
			offset += 4;
			memcpy(sHsmBuffer + offset, sSecretKey, 1 + 16);
			offset += 17;
			nKeyLen = AsciiToHex(sSecretKey + 17, sHsmBuffer + offset, nKeyLen - 17);
			offset += nKeyLen;
		}
		else
		{
			sprintf(sHsmBuffer + offset, "%4d", nKeyLen/2);               
			offset += 4;
			AsciiToHex(sSecretKey, sHsmBuffer + offset, nKeyLen);
			offset += nKeyLen/2;
		}
	}


	sprintf(sLine, "End command_EW(%d)", offset);
	trace_event(sLine, PROCESSING);
	return (offset);
}

/******************************************************************************/
/* Fonction command_EX.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recupere le buffer              */
/* resultat de EW					                                         */
/******************************************************************************/
int command_EX(unsigned char *sBuffRcv, unsigned char *sSignature, int  *nSignatureLen)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];
	char            sErrCode[10 +1 ];
	char            sSignatureLen[4 +1 ];

	trace_event("Start command_EX()", PROCESSING);

	memset(Command,  0, sizeof(Command));
	memset(sErrCode, 0, sizeof(sErrCode));
	memset(sSignatureLen, 0, sizeof(sSignatureLen));
	memset(sLine,    0, sizeof(sLine));

	memcpy(Command, sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);

	if (memcmp(Command, "EX", 2) != 0) 
	{
		trace_event("End   command_EX(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}

	TextError(sErrCode + 2, sLine);

	if (memcmp(sErrCode + 2, "00", 2) != 0) 
	{
		sprintf(sLine, "HSM Error: %.30s", sLine);
		trace_event(sLine, TRACE);		 
		trace_event("End   command_EX(NOK)", PROCESSING);
		return (NOK);
	}


	memcpy (sSignatureLen, sBuffRcv + 4, 4);
	*nSignatureLen = atoi (sSignatureLen);


	memcpy (sSignature, sBuffRcv + 8, *nSignatureLen);

	trace_event("End   command_EX(OK)", PROCESSING);
	return (OK);
}

/*****************************************************************************\
* GK - Export  Key under RSA Public Key
\*****************************************************************************/
int    command_GK(	unsigned char *buffer_transmit                    ,
	char          *Encryption_Identifier              , /* 2N used to encrypt the key */     
	char          *Pad_Mode_Identifier                , /* 2N 01 = PKCS#1, 02 = OAEP*/
	char          *Mask_Generation_Function           , /* 2N present if OAEP mode*/
	char          *MGF_Hash_Function                  , /* 2N 01 = SHA-1 */
	char          *OAEP_Encoding_Parameters_Length    , /* 2N present if OAEP mode */
	char          *OAEP_Encoding_Parameters           , /* 2N present if OAEP mode */
	char          *Key_Type                           , /* 4N used to indicate LMK pair and variant used to encrypt the key */
	char          *Signature_Indicator                , /* 1A = present if sig fields are present */
	char          *Signature_Hash_Identifier          , /* 2A Identifier of the hash algorithm  */
	char          *Signature_Identifier               , /* 2A 01 = PKCS#1 Identifier of the signature algorithm*/
	char          *Signature_Pad_Mode_Identifier      , /* 2N only present if Signature_Identifier present  */
	char          *Header_Data_Block_Length           , /* 4N default 0000*/
	char          *Header_Data_Block				  , /* nB if Signature_Identifier present */
	char          *Footer_Data_Block_Length           , /* 4N default 0000 */
	char          *Footer_Data_Block                  , /* nB if Signature_Identifier present  */
	char          *Private_Key_Flag                   , /* 2N location of the private key if flag = 99 use private key provided with command */
	char          *Private_Key_Length                 , /* 4N only present if flag = 99*/
	char          *Private_Key                        , /* nB encrypted using LMK pair 34-35 only present if flag = 99*/
	char          *DES_Key_Flag                       , /* 1N  0 : single, 1 : double, 2 : triple*/
	char          *DES_Key_LMK                        , /* Only present when exporting a DES key (Key Type <> 3401) */
	char          *Check_Value                        , /* 16H Check value on DES key (Key Type <> 3401)*/
	char          *HMAC_Key_Format                    , /* 2N Only present if importing an HMAC key (i.e. Key Type = 3401). */
	char          *HMAC_Key_Length                    , /* 4N Only present if importing an HMAC key (i.e. Key Type = 3401). */
	char          *HMAC_Key_LMK                       , /* NB if Key Type = 3401*/
	char          *MAC                                , /* 4B */
	char          *Public_Key                         , /* nB DER encoded in ASN.1 format */
	char          *Authentication_Data                , /* nB Optional*/
	char          *Key_Block_Type                     , /* 2N 01 = Key Block format, 02 = Key Block Template, 03 = Unformatted Key Block, ...  */
	char          *Key_Block_Template_Length          , /* 4N */
	char          *Key_Block_Template                 , /* nH Key data and Check Value data (if present) zero filled  if Key_Block_Type= 02*/
	char          *Key_Offset                         , /*  if Key_Block_Type = 02 */
	char          *Check_Value_Length                 , /* 2N 0..8 if Key Block Type = 02*/
	char          *Check_Value_Offset                  /* 4N if Key Block Type = 02*/
	)
{
	int               nOffset = 0, Public_Key_Len = 0;
	/*char              Delimiter [1+ 1]; */


	/*memset (Delimiter,           0, sizeof (Delimiter));*/



	trace_event("Start command_GK()", PROCESSING);

	/*
	- sample
	GK 01 01 1404 1

	U6CB03686CA61EB6F24DD9839EC458ADE C3F15BB36A6DFF7A
	<C501FE2D><30450240A

	1D8B0C7D2C52393825A8223C820AE0D130DE3EB8BF96819225C3848><40D788A493B1C2E12619223

	070EAC7CE3A68A35C3A06796C3478E66C9B06C318><70C0F4F7020103>;01

	Command Response:

	GL00****************0064<************************************************************

	********************************************************************>
	*/

	memcpy(buffer_transmit, "GK", 2);           
	nOffset +=2;

	memcpy(buffer_transmit + nOffset, Encryption_Identifier, 2);
	nOffset += 2;

	memcpy(buffer_transmit + nOffset, Pad_Mode_Identifier, 2);
	nOffset += 2;

	/* --------------------------------------------------------------------------------------------------------- */
	if (memcmp (Pad_Mode_Identifier, HSM_PAD_MODE_PKCS1_2_2, 2) ==0 )
	{
		memcpy(buffer_transmit + nOffset, Mask_Generation_Function, strlen(Mask_Generation_Function));
		nOffset += strlen(Mask_Generation_Function);

		memcpy(buffer_transmit + nOffset, MGF_Hash_Function, strlen(MGF_Hash_Function));
		nOffset += strlen(MGF_Hash_Function);

		memcpy(buffer_transmit + nOffset, OAEP_Encoding_Parameters_Length, strlen(OAEP_Encoding_Parameters_Length));
		nOffset += strlen(OAEP_Encoding_Parameters_Length);

		memcpy(buffer_transmit + nOffset, OAEP_Encoding_Parameters, strlen(OAEP_Encoding_Parameters));
		nOffset += strlen(OAEP_Encoding_Parameters);

		buffer_transmit[nOffset++]=';';
	}

	/* --------------------------------------------------------------------------------------------------------- */
	if( DES_Key_LMK[0] == 'S' )
	{
		memcpy(buffer_transmit + nOffset, "FFFF", 4);
		nOffset +=4;
	}
	else
	{
		memcpy(buffer_transmit + nOffset, Key_Type, strlen(Key_Type));
		nOffset +=strlen(Key_Type);
	}

	/* --------------------------------------------------------------------------------------------------------- */
	if (   
		strlen(Signature_Hash_Identifier    ) > 0 && 
		strlen(Signature_Identifier         ) > 0 && 
		strlen(Signature_Pad_Mode_Identifier) > 0
		)
	{


		buffer_transmit[nOffset++]='=';

		memcpy(buffer_transmit + nOffset, Signature_Hash_Identifier          	, strlen(Signature_Hash_Identifier          ));
		nOffset +=strlen(Signature_Hash_Identifier          );

		memcpy(buffer_transmit + nOffset, Signature_Identifier               	, strlen(Signature_Identifier               ));
		nOffset +=strlen(Signature_Identifier               );

		memcpy(buffer_transmit + nOffset, Signature_Pad_Mode_Identifier      	, strlen(Signature_Pad_Mode_Identifier      ));
		nOffset +=strlen(Signature_Pad_Mode_Identifier      );


		memcpy(buffer_transmit + nOffset, Header_Data_Block_Length           	, strlen(Header_Data_Block_Length           ));
		nOffset +=strlen(Header_Data_Block_Length           );

		if (memcmp (Header_Data_Block_Length, "0000", 4) != 0)
		{
			memcpy(buffer_transmit + nOffset, Header_Data_Block				    , strlen(Header_Data_Block				    ));
			nOffset +=strlen(Header_Data_Block				    );
		}

		buffer_transmit[nOffset++]=';';

		memcpy(buffer_transmit + nOffset, Footer_Data_Block_Length           	, strlen(Footer_Data_Block_Length           ));
		nOffset +=strlen(Footer_Data_Block_Length           );

		if (memcmp (Footer_Data_Block_Length, "0000", 4) != 0)
		{
			memcpy(buffer_transmit + nOffset, Footer_Data_Block                  	, strlen(Footer_Data_Block                  ));
			nOffset +=strlen(Footer_Data_Block                  );
		}


		/* --------------------------------------------------------------------------------------------------------- */

		buffer_transmit[nOffset++]=';';

		memcpy(buffer_transmit + nOffset, Private_Key_Flag                   	, strlen(Private_Key_Flag                   ));
		nOffset +=strlen(Private_Key_Flag                   );

		/* --------------------------------------------------------------------------------------------------------- */

		if (memcmp( Private_Key_Flag, "99", 2 ) == 0)
		{
			if( Private_Key[0] == 'S' )
			{
				memcpy(buffer_transmit + nOffset, "FFFF", 4);
				nOffset +=4;
			}
			else
			{
				memcpy(buffer_transmit + nOffset, Private_Key_Length                 	, strlen(Private_Key_Length                 ));
				nOffset +=strlen(Private_Key_Length                 );
			}

			memcpy(buffer_transmit + nOffset, Private_Key                        	, strlen(Private_Key                        ));
			nOffset +=strlen(Private_Key                        );
		}

		buffer_transmit[nOffset++]=';';
	}



	/* --------------------------------------------------------------------------------------------------------- */

	if (memcmp (Key_Type, "3401", 4) != 0)
	{	
		if( DES_Key_LMK[0] == 'S' )
		{
			buffer_transmit[nOffset++] = 'F';
		}
		else
		{
			memcpy(buffer_transmit + nOffset, DES_Key_Flag                       	, strlen(DES_Key_Flag                       ));
			nOffset +=strlen(DES_Key_Flag                       );
		}

		memcpy(buffer_transmit + nOffset, DES_Key_LMK                        	, strlen(DES_Key_LMK                        ));
		nOffset +=strlen(DES_Key_LMK                        );

		memcpy(buffer_transmit + nOffset, Check_Value                        	, strlen(Check_Value                        ));
		nOffset +=strlen(Check_Value                        );
	}
	else
	{
		if( HMAC_Key_LMK[0] == 'S' )
		{
			memcpy(buffer_transmit + nOffset, "04", 2);
			nOffset +=2;

			memcpy(buffer_transmit + nOffset, "FFFF", 4);
			nOffset +=4;
		}
		else
		{
			memcpy(buffer_transmit + nOffset, HMAC_Key_Format                    	, strlen(HMAC_Key_Format                    ));
			nOffset +=strlen(HMAC_Key_Format                    );

			memcpy(buffer_transmit + nOffset, HMAC_Key_Length                    	, strlen(HMAC_Key_Length                    ));
			nOffset +=strlen(HMAC_Key_Length                    );
		}

		memcpy(buffer_transmit + nOffset, HMAC_Key_LMK                       	, strlen(HMAC_Key_LMK                       ));
		nOffset +=strlen(HMAC_Key_LMK                       );

		buffer_transmit[nOffset++]=';';
	}

	/* --------------------------------------------------------------------------------------------------------- */




	
	
	/*ZKO020142019 Start*/
	Public_Key_Len = strlen(Public_Key);
	
	if( Public_Key[0] == 'S' )
	{
		memcpy(buffer_transmit + nOffset, Public_Key, 1 + 16);
		nOffset += 17;
		AsciiToHex(Public_Key + 17, buffer_transmit + nOffset, Public_Key_Len - 17);
		nOffset += (Public_Key_Len - 17)/2;
	}
	else
	{
		
		AsciiToHex(Public_Key, buffer_transmit + nOffset, Public_Key_Len);
		nOffset += (Public_Key_Len/2);		
	}
	


    /*ZKO020142019 End*/

	/* --------------------------------------------------------------------------------------------------------- */

	if (strlen(Authentication_Data ) > 0)
	{
		memcpy(buffer_transmit + nOffset, Authentication_Data                	, strlen(Authentication_Data                ));
		nOffset +=strlen(Authentication_Data                );
	}

	/* --------------------------------------------------------------------------------------------------------- */

	if (strlen(Key_Block_Type) > 0) 
	{	
		buffer_transmit[nOffset++]=';';


		memcpy(buffer_transmit + nOffset, Key_Block_Type                     	, strlen(Key_Block_Type                     ));
		nOffset +=strlen(Key_Block_Type                     );
	}

	/* --------------------------------------------------------------------------------------------------------- */

	if (memcmp(Key_Block_Type, "02",2 ) == 0)
	{
		memcpy(buffer_transmit + nOffset, Key_Block_Template_Length          	, strlen(Key_Block_Template_Length          ));
		nOffset +=strlen(Key_Block_Template_Length          );

		memcpy(buffer_transmit + nOffset, Key_Block_Template                 	, strlen(Key_Block_Template                 ));
		nOffset +=strlen(Key_Block_Template                 );

		buffer_transmit[nOffset++]=';';

		memcpy(buffer_transmit + nOffset, Key_Offset                         	, strlen(Key_Offset                         ));
		nOffset +=strlen(Key_Offset                         );
	}

	/* --------------------------------------------------------------------------------------------------------- */

	if (memcmp (Key_Type, "3401", 4) != 0 && memcmp(Key_Block_Type, "02",2 ) == 0)
	{
		memcpy(buffer_transmit + nOffset, Check_Value_Length                 	, strlen(Check_Value_Length                 ));
		nOffset +=strlen(Check_Value_Length                 );

		memcpy(buffer_transmit + nOffset, Check_Value_Offset                 	, strlen(Check_Value_Offset                 ));
		nOffset +=strlen(Check_Value_Offset                 );
	}

	/* -------------------------------------------   End   ------------------------------------------------------ */

	trace_event("End   command_GK(OK)", PROCESSING);
	return(nOffset);
}

/******************************************************************************/
/* Fonction command_GL.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recupere le buffer              */
/* resultat de Export  Key under RSA Public Key                               */
/******************************************************************************/
int            command_GL( unsigned char    *sBuffRcv,
	char             *Key_Type                           , /* 4N used to indicate LMK pair and variant used to encrypt the key */
	char             *Initialization_value               , /* 16H if Key Type <> 3401*/
	char             *Encrypted_Key_Length               , /* 4N */
	unsigned char    *Encrypted_Key                      , /* nB */
	char             *Signature_Indicator                , /* = */
	char             *Signature_Length                   , /* 4N Only present when the Signature Indicator is present*/
	char             *Signature                          , /* nB Only present when the Signature Indicator is present*/
	char             *sErrCode
	)
{
	char            Command[3];
	char            sLine[MAX_LINE_TRC];
	int             offset = 0;

	trace_event("Start command_GL()", PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command, sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);

	if (memcmp(Command, "GL", 2) != 0) 
	{
		trace_event("End   command_GL(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}

	TextError(sErrCode + 2, sLine);
	if (memcmp(sErrCode + 2, "00", 2) != 0) 
	{
		trace_event("End   command_GL(ERROR)", PROCESSING);
		return (ERROR);
	}
	offset = 4;
	if (memcmp (Key_Type, "3401", 4) != 0 && memcmp (Key_Type, "1400", 4) != 0)
	{
		memcpy (Initialization_value, sBuffRcv + offset, 16);
		offset += 16;
	}

	memcpy (Encrypted_Key_Length, sBuffRcv + offset , 4);
	offset += 4;

	memcpy (Encrypted_Key , sBuffRcv + offset , atoi(Encrypted_Key_Length));
	offset += atoi(Encrypted_Key_Length);

	if (	offset  + 4 < strlen((char *) sBuffRcv) && 
		memcmp (Signature_Indicator, "=", 1 ) == 0 
		)
	{
		memcpy (Signature_Length, sBuffRcv + offset , 4);
		offset += 4;

		if (atoi(Signature_Length) > 0)
			memcpy (Signature , sBuffRcv + offset , atoi(Signature_Length));
	}

	trace_event("End   command_GL(OK)", PROCESSING);
	return (OK);
}

/*****************************************************************************\
* EO - Import a Public Key by either generating a MAC on it, using the LMK or 
* by creating a keyblock.
\*****************************************************************************/
int    command_EO(char *buffer_transmit , char		*rsakey,char    *rsaencoding)
{
	char    RSA_Public_Key[LG_MAX]; 
	int     nKeyLen;
	int     nOffset = 0;

	memset(RSA_Public_Key, 0, sizeof(RSA_Public_Key));

	trace_event("Start command_EO()", PROCESSING);

	memcpy(buffer_transmit, "EO", 2);           
	nOffset +=2;

	memcpy(buffer_transmit + nOffset, rsaencoding, strlen(rsaencoding));
	nOffset +=2;

	nKeyLen = strlen(rsakey);
	AsciiToHex(rsakey, RSA_Public_Key, nKeyLen);
	memcpy(buffer_transmit + nOffset, RSA_Public_Key, nKeyLen/2);
	nOffset += (nKeyLen/2);
	
	memcpy(buffer_transmit + nOffset, "~%01#N00S00", 11);/*ZKO01042019*/
	nOffset += 11;
	

	trace_event("End   command_EO(OK)", PROCESSING);
	return(nOffset);
}

int    command_EP(	char *buffer_received, char *sRSAKey, char *mac)
{
	unsigned      char mac_from_buff[4 + 1];
	char          sLine[MAX_LINE_TRC];
	char          sAsciiHex[2 + 1], sKeyLen [4 + 1];
	int           nCtr, i, nKeyLen;

	memset(mac_from_buff, 0, sizeof(mac_from_buff));
	memset(sAsciiHex,     0, sizeof(sAsciiHex));
	memset(sKeyLen,     0, sizeof(sKeyLen));

	trace_event("Start command_EP()",PROCESSING);

	if (memcmp(buffer_received, "EP", 2) != 0) 
	{
		trace_event("End   command_EP(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	TextError( buffer_received + 2,sLine);
	if (memcmp(buffer_received + 2 , "00", 2) != 0) 
	{
		trace_event("End   command_EP(CHECK_ERROR)",PROCESSING);
		return(CRYPTOGRAPHIC_ERROR );
	}
	
	/*ZKO Start 28032019*/
	if(buffer_received[4] =='S')
	{   
		memcpy(sRSAKey, buffer_received + 4, 17);
		memcpy(sKeyLen,buffer_received + 6, 4);
		nKeyLen = atoi(sKeyLen);
		HexToAscii(buffer_received + 21 , sRSAKey + 17, (nKeyLen - 16) * 2);
		
		trace_event("End   command_EP(OK)",PROCESSING);
		return(OK);

	}
	/*ZKO END 28032019*/

	memcpy(mac_from_buff, buffer_received + 4, 4);

	/*EBETODO*/
	nCtr = 0;
	for (i = 0; i < 4; i++)
	{
		memset(sAsciiHex, 0, sizeof(sAsciiHex));
		sprintf(sAsciiHex, "%02X", mac_from_buff[i]);
		memcpy(mac + nCtr, sAsciiHex, 2);
		nCtr += 2;
	}

	sprintf(sLine, "mac: (%lu)%s", strlen(mac), mac);
	trace_event(sLine, TRACE);

	trace_event("End   command_EP(OK)",PROCESSING);
	return(OK);
}


/*ZKO:StartPROD00059856*/
/*Translate a PIN from *BDK Encryption to Interchange Key Encryption*/

int command_G0  (  char *sHsmBuffer,
	char *sBdk,
	char *sZpk,
	char *sKsnDesc,
	char *sKsn,
	char *sPinBlock,
	char *sFormatIN,
	char *sFormatOut,
	char *sPan
	)
{

	int     offset = 0;
	char    sLine[MAX_LINE_TRC];
	char    sCard[13];

	memset(sLine, 0, sizeof(sLine));
	memset(sCard, 0, sizeof(sCard));

	trace_event("Start command_G0()", PROCESSING);

	memcpy(sHsmBuffer + offset, "G0",   2);
	offset += 2;   

	sprintf(sLine, "sBdk(%s)",sBdk);
	trace_event(sLine,  PROCESSING);

	memcpy(sHsmBuffer + offset, sBdk,   strlen(sBdk));
	offset += strlen(sBdk);

	sprintf(sLine, "sZpk(%s)",sZpk);
	trace_event(sLine,  PROCESSING);
	memcpy(sHsmBuffer + offset, sZpk,   strlen(sZpk));
	offset += strlen(sZpk);

	sprintf(sLine, "sKsnDesc(%s)",sKsnDesc);
	trace_event(sLine,  PROCESSING);
	memcpy(sHsmBuffer + offset, sKsnDesc,   strlen(sKsnDesc));
	offset += strlen(sKsnDesc);

	sprintf(sLine, "sKsn(%s)",sKsn);
	trace_event(sLine,  PROCESSING);
	memcpy(sHsmBuffer + offset, sKsn,   strlen(sKsn));
	offset += strlen(sKsn);

	sprintf(sLine, "sPinBlock(%s)",sPinBlock);
	trace_event(sLine,  PROCESSING);
	memcpy(sHsmBuffer + offset, sPinBlock,   strlen(sPinBlock));
	offset += strlen(sPinBlock);  

	sprintf(sLine, "sFormatIN(%s)",sFormatIN);
	trace_event(sLine,  PROCESSING);    
	memcpy(sHsmBuffer + offset, sFormatIN,   strlen(sFormatIN));
	offset += strlen(sFormatIN); 

	sprintf(sLine, "sFormatOut(%s)",sFormatOut);
	trace_event(sLine,  PROCESSING);  
	memcpy(sHsmBuffer + offset, sFormatOut,   strlen(sFormatOut));
	offset += strlen(sFormatOut);  

	sprintf(sLine, "sPan(%s)",sPan);
	trace_event(sLine,  PROCESSING);  
	format_card_from_pan ( sCard , sPan );
	memcpy(sHsmBuffer + offset, sCard ,12); 
	offset += 12;

	sprintf(sLine, "End command_G02(%s,%s,%s)",sPinBlock,sFormatOut,sCard);
	trace_event(sLine,  PROCESSING);

	sprintf(sLine, "End command_G0(%d)", offset);
	trace_event(sLine,  PROCESSING);
	return (offset);
}

int    command_G1( char *sBuffRcv,
	char *sErrCode,
	char *sPinLen,
	char *sPinBlock,
	char *sFormatCode
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];

	trace_event("Start command_G1()",PROCESSING);

	memset(Command, 0, sizeof(Command));

	memcpy(Command , sBuffRcv, 2);
	memcpy(sErrCode, sBuffRcv, 4);
	if (memcmp( Command , "G1", 2) != 0)
	{
		trace_event("End   command_G1(ERROR_COMMAND)",PROCESSING);
		return ( ERROR_COMMAND );
	}

	TextError( sErrCode + 2,sLine);
	if (memcmp(sErrCode + 2, "00", 2) == 0)
	{
		memcpy(sPinLen,     sBuffRcv + 4, 2);
		memcpy(sPinBlock,   sBuffRcv + 6, 16);
		memcpy(sFormatCode, sBuffRcv + 22, 2);

		trace_event("End   command_G1(OK)",PROCESSING);
		return ( OK );
	}
	else
	{

		trace_event("End   command_G1(VERIFICATION_FAILED)",PROCESSING);
		return ( VERIFICATION_FAILED );
	}
	trace_event("End   command_G1(CRYPTOGRAPHIC_ERROR)",PROCESSING);
	return(CRYPTOGRAPHIC_ERROR);
}
/*ZKO:ENDPROD00059856*/




/*****************************************************************************\
* A8 - To export a key 
\*****************************************************************************/
int    command_A8(	char		*buffer_transmit ,
	char*		keyType,
	char*       zmkLmk,
	char*        keyLmk
	)
{


		trace_event("Start command_A8()", PROCESSING);

		memcpy(buffer_transmit, "A8", 2);           
		buffer_transmit +=2;

		memcpy(buffer_transmit, keyType, 3); 
		buffer_transmit +=3;


		if(strcmp(keyType,THALES_KEY_TP_TAK)==0)
		{
			memcpy(buffer_transmit, ";1", 2); 
			buffer_transmit +=2;
		}
		if(strcmp(keyType,THALES_KEY_TP_TMK)==0)
		{
			memcpy(buffer_transmit, ";0", 2); 
			buffer_transmit +=2;
		}

		memcpy(buffer_transmit, zmkLmk, strlen(zmkLmk)); 
		buffer_transmit +=strlen(zmkLmk);
		
		
		memcpy(buffer_transmit, keyLmk, strlen(keyLmk)); 
		buffer_transmit +=strlen(keyLmk);

		/*Delimiter*/
		if(strlen(keyLmk) > 16)
		{
			memcpy(buffer_transmit, "X", 1); /*ZKOTODO:to be changed to support other schemes*/
			buffer_transmit += 1;
		}

	

	trace_event("End   command_A8(OK)", PROCESSING);
	return(OK);
}

int    command_A9(	char *buffer_received,
					char *keyZmk, 
					char *check_value 
	)
{
	char Command[3];
	char sLine[MAX_LINE_TRC];
	int		nKeyLen;
	char	sKeyLenStr[32];

	trace_event("Start command_A9()",PROCESSING);


	if (memcmp(buffer_received, "A9", 2) != 0) 
	{
		trace_event("End   command_A9(ERROR_COMMAND)",PROCESSING);
		return(ERROR_COMMAND);
	}

	TextError( buffer_received + 2,sLine);
	if (memcmp(buffer_received + 2 , "00", 2) != 0) 
	{
		trace_event("End   command_A9(CHECK_ERROR)",PROCESSING);
		return(CHECK_ERROR );
	}

	buffer_received += 4;
	switch (buffer_received[0])
	{
		case 'U' :
		case 'X' :
			memcpy(keyZmk, buffer_received,  2 * SIZE_KEY + 1); 
			buffer_received +=( 2 * SIZE_KEY + 1);
			keyZmk [ 2 * SIZE_KEY + 1] = '\0';
			break;
		case 'T' :
		case 'Y' :
			trace_event("Triple DES Encryption Triple Length",TRACE);
			memcpy(keyZmk, buffer_received,  3 * SIZE_KEY + 1); 
			buffer_received +=( 3 * SIZE_KEY + 1);
			keyZmk [ 3 * SIZE_KEY + 1] = '\0'; 
			break;
		case 'S':
			trace_event("Keyblock Encryption Length",TRACE);
			memcpy(sKeyLenStr,buffer_received + 2,4);
			nKeyLen = atoi(sKeyLenStr) + 1;
			memcpy(keyZmk, buffer_received,nKeyLen);
			keyZmk [nKeyLen] = '\0';
			buffer_received += nKeyLen;
			break;
		default  : 
			trace_event("Single DES Encryption Single Length",TRACE);
			memcpy(keyZmk, buffer_received, SIZE_KEY); 
			buffer_received +=SIZE_KEY;
			keyZmk [SIZE_KEY] = '\0';

	}

	memcpy(check_value, buffer_received, 6);
	check_value[6]='\0';

	trace_event("End   command_A9(OK)",PROCESSING);
	return(OK);
}

/* Mehdi Elyajizi 16092006 emv control verification flags*/
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




/* Start ZKO 16102019  PROD00072366 implementation of  FW and BK for PCI compliance*/

/******************************************************************************/
/* Fonction command_FW.                                                       */
/* -------------------                                                        */
/* Description : Generate an ABA PVV (of a customer selected PIN)			  */
/*                                                                            */
/******************************************************************************/
int 
	command_FW(char *buffer_transmit,
	char *Tpk_Zpk,
	char *Pvk,
	char *PinBlock,
	char *format,
	char *Pan,
	char *PvkI
	)
{
	char            Card[13];
	char            ligne[MAX_LINE_TRC];
	/*SKO290509*/ char           CardTrunc[22 + 1];
	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(Card, 0, 13);
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "Start command_FW(%.16s,%.2s,%.22s)", PinBlock, format, Pan);
	else{
		TrunCard(Pan, CardTrunc);
		sprintf(ligne, "Start command_FW(%.2s,%.22s)", format, CardTrunc);
	}
	trace_event(ligne, PROCESSING);
	memcpy(buffer_transmit, "FW", 2);
	buffer_transmit += 2;
	memcpy(buffer_transmit, "FFF", 3);
	buffer_transmit += 3;
	memcpy(buffer_transmit, Tpk_Zpk, strlen(Tpk_Zpk));
	buffer_transmit += strlen(Tpk_Zpk);
	memcpy(buffer_transmit, Pvk, strlen(Pvk));
	buffer_transmit += strlen(Pvk);
	memcpy(buffer_transmit, PinBlock, SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy(buffer_transmit, format, 2);
	buffer_transmit += 2;
	format_card_from_pan(Card, Pan);
	memcpy(buffer_transmit, Card, 12);
	buffer_transmit += 12;
	memcpy(buffer_transmit, PvkI, 1);
	buffer_transmit += 1;
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "End   command_FW(%.60s)", buffer_transmit);
	else
		sprintf(ligne, "End   command_FW(OK)");
	trace_event(ligne, PROCESSING);

	return (OK);
}


/******************************************************************************/
/* Fonction command_FX.                                                       */
/* -------------------                                                        */
/*                               								             */
/******************************************************************************/
int 
	command_FX(char *buffer_received,
	char *Pvv,
	char *err_code
	)
{
	char            ligne[MAX_LINE_TRC];
	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	memset(Command, 0, 3);
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "Start command_FX(%.40s)", buffer_received);
	else
		sprintf(ligne, "Start command_FX()");
	trace_event(ligne, PROCESSING);

	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received + 2, 2);
	if (memcmp(Command, "FX", 2) != 0) {
		trace_event("End   command_FX(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code, sLine);
	if (memcmp(err_code, "00", 2) != 0) {
		trace_event("End   command_FX(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	memcpy(Pvv, buffer_received, 4);
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
		sprintf(ligne, "End   command_FX(%.2s,%.16s)", err_code, Pvv);
	else
		sprintf(ligne, "End   command_FX(OK)");
	trace_event(ligne, PROCESSING);

	return (OK);
}


/******************************************************************************/
/* Fonction command_BK                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de generer un IBM PIN offset       */
/******************************************************************************/

int 
	command_BK(char *buffer_transmit,
	char *Pvk,
	char *Tpk_Zpk,
	char *PinBlock,
	char *format,
	char *Pan,
	char *sDecTable
	)
{

	char            Card[13];

	trace_event("Start command_BK()", PROCESSING);

	memset(Card, 0, 13);
	memcpy(buffer_transmit, "BK", 2);
	buffer_transmit += 2;

	memcpy(buffer_transmit, "FFF", 3);
	buffer_transmit += 3;

	memcpy(buffer_transmit, Tpk_Zpk, strlen(Tpk_Zpk));/*NAB18022019 Key Block*/
	buffer_transmit += strlen(Tpk_Zpk);
	
	memcpy(buffer_transmit, Pvk, strlen(Pvk));/*NAB18022019 Key Block*/
	buffer_transmit += strlen(Pvk);

	memcpy(buffer_transmit, PinBlock, SIZE_KEY);
	buffer_transmit += SIZE_KEY;

	memcpy(buffer_transmit, format, 2);
	buffer_transmit += 2;
	
	memcpy(buffer_transmit, "04", 2);
	buffer_transmit += 2;

	format_card_from_pan(Card, Pan);
	memcpy(buffer_transmit, Card, 12);
	buffer_transmit += 12;
	/*PROD00053984
	memcpy(buffer_transmit, Validation_data, SIZE_KEY);*/
	memcpy(buffer_transmit, sDecTable, SIZE_KEY);
	buffer_transmit += SIZE_KEY;

	memcpy(buffer_transmit, Pan, 10);
	buffer_transmit += 10;
	memcpy(buffer_transmit, "N", 1);
	buffer_transmit += 1;
	memcpy(buffer_transmit, Pan + (strlen(Pan) - 1), 1);
	buffer_transmit += 1;

	trace_event("End   command_BK(OK)", PROCESSING);
	return (OK);
}


/******************************************************************************/
/* Fonction command_BL.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM l'offset.                                                  */
/******************************************************************************/
int 
	command_BL(char *buffer_received,
	char *offset,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_BL()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);
	if (memcmp(Command, "BL", 2) != 0) {
		trace_event("End   command_BL(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);
	}
	TextError(err_code + 2, sLine);
	if (memcmp(err_code + 2, "00", 2) != 0 && memcmp(err_code + 2, "02", 2) != 0) {
		trace_event("End   command_BL(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);
	}
	buffer_received += 4;

	memcpy(offset, buffer_received, 12);
	trace_event("End   command_BL(OK)", PROCESSING);
	return (OK);
}

/* End ZKO 16102019  PROD00072366 implementation of  FW and BK for PCI compliance*/




/*PROD00073147 - Mastercard SPA2 algorithm Implementation*/

/******************************************************************************/
/* Fonction command_LQ                                                      */
/* -------------------                                                        */
/* Description : Generate an HMAC on a Block of Data       */
/******************************************************************************/

int 
	command_LQ(char *buffer_transmit,
	char *Hash_Identifier,
	char *HMAC_Len,
	char *HMAC_Key_Fromat,
	char *HMAC_Key,
	int   nMsgDataLen,
	char *MsgData
	)
{

	char            Card[13];
	int				offset = 0;

	trace_event("Start command_LQ()", PROCESSING);

	memset(Card, 0, 13);
	memcpy(buffer_transmit, "LQ", 2);
	offset = offset + 2;

	memcpy(buffer_transmit + offset, Hash_Identifier, 2);
	offset = offset + 2;

	memcpy(buffer_transmit + offset, HMAC_Len, 4);
	offset = offset + 4;	

	memcpy(buffer_transmit + offset, HMAC_Key_Fromat, 2);
	offset = offset + 2;

	memcpy(buffer_transmit + offset, "FFFF", 4);
	offset = offset + 4;

	memcpy(buffer_transmit + offset, HMAC_Key, strlen(HMAC_Key));
	offset = offset + strlen(HMAC_Key);

	sprintf(buffer_transmit + offset, "%05d", nMsgDataLen);
	offset = offset + 5;

	memcpy(buffer_transmit + offset, MsgData, nMsgDataLen);
	offset = offset + nMsgDataLen;
	
	trace_event("End   command_LQ(OK)", PROCESSING);
	
	return (offset);
}


/******************************************************************************/
/* Fonction command_BL.                                                       */
/* -------------------                                                        */
/* Description : Cette fonction a pour but de recuperer du buffer recu du     */
/* HSM l'offset.                                                  */
/******************************************************************************/
int 
	command_LR(char *buffer_received,
	char *sHMAC,
	char *err_code
	)
{

	char            Command[3];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start command_LR()", PROCESSING);

	memset(Command, 0, 3);
	memcpy(Command, buffer_received, 2);
	memcpy(err_code, buffer_received, 4);

	if (memcmp(Command, "LR", 2) != 0) {

		trace_event("End   command_LR(ERROR_COMMAND)", PROCESSING);
		return (ERROR_COMMAND);

	}
	TextError(err_code + 2, sLine);

	if (memcmp(err_code + 2, "00", 2) != 0 ) {

		trace_event("End   command_LR(CHECK_ERROR)", PROCESSING);
		return (CHECK_ERROR);

	}
	buffer_received += 8;

	memcpy(sHMAC, buffer_received, 4);
	trace_event("End   command_LR(OK)", PROCESSING);
	return (OK);
}

/* End ZKO 16102019  PROD00072366 implementation of  FW and BK for PCI compliance*/





