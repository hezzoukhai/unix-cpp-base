  /***********************************************************************
  **  nom	: vhsm_gen_pvv.c					**
  **  desc	: creation du pvv					**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  **				(c) Hightech Payment Systems		**
  ***********************************************************************/
#include <stdio.h>

#include <define.h>
#include <hsm_inc.h>
#include "des.h"

#undef _FOR_INT_USE_ 
#undef _FOR_EXT_USE_ 
                     
#define _FOR_EXT_USE_
#include <libvhsm.h>    
#undef _FOR_EXT_USE_ 

  /***********************************************************************
  **  nom	: check_ee_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande EE						**
  **  entree	: cmd		-> commande				**
  **		      cmd_len	-> langueur de la commande		**
  ***********************************************************************/
check_ee_cmd(cmd, cmd_len, pvka_lmk, pvv_offset, pin_length , pan , dec_table, pin_val_data )
unsigned char	cmd[];
int	cmd_len;
unsigned char	pvka_lmk[];
unsigned char	pvv_offset  [];
unsigned char	pin_length[];
unsigned char	pan[];
unsigned char	dec_table[];
unsigned char	pin_val_data[];
{
int		offset;

char detail[256];

sprintf(detail, "Start check_ee_cmd(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
offset = 0;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(pvka_lmk,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pvv_offset,	cmd + offset, 	12);

offset += 12;
memcpy(pin_length,		cmd + offset,	2);

offset += 2;
memcpy(pan,		cmd + offset,	12);

offset += 12;
memcpy(dec_table, 		cmd + offset, 	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pin_val_data, 		cmd + offset, 	12);

sprintf(detail, "End   check_ee_cmd(%.16s, %.12s, %.2s, %.19s, %.16s, %.12s)",
        pvka_lmk, pvv_offset, pin_length, pan, dec_table, pin_val_data);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_ee_cmd					**
  **  desc	: traitement de la commande EE par VHSM			**
  ***********************************************************************/
process_ee_cmd( pvka_lmk, pvv_offset, pin_length, pan, dec_table, pin_val_data, error_code, pin_lmk)
unsigned char		pvka_lmk[];
unsigned char		pvv_offset[];
unsigned char		pin_length[];
unsigned char		pan[];
unsigned char		dec_table[];
unsigned char		pin_val_data[];
unsigned char		error_code[];
unsigned char		pin_lmk[];
{
unsigned char	lmk      [SIZE_KEY];
unsigned char	pin      [SIZE_KEY];
unsigned char	pvka     [SIZE_KEY];
unsigned char	strtmp   [      32];
unsigned char	entree1  [      16];
unsigned char	sortie1  [      16];

char            detail[256];
int		offset, cpt1, cpt2;

sprintf(detail, "Start process_ee_cmd((%.16s, %.12s, %.2s, %.19s, %.16s, %.12s)",
        pvka_lmk, pvv_offset, pin_length, pan, dec_table, pin_val_data);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(lmk,		      0, sizeof(lmk) );
memset(pin,		      0, sizeof(pin) );
memset(pvka,		0, sizeof(pvka));
memset(error_code,	0, sizeof(error_code));
memset(strtmp,		'0', sizeof(strtmp));
memset(entree1,		0,   sizeof(entree1));
memset(sortie1,		0,   sizeof(sortie1));
offset = 0;

/*  CHARGEMENT DE LA LMK  */
get_lmk(0, lmk);

/*  DECRYPTAGE DE PVKa  */
DES(-1, 'H', pvka_lmk, lmk, pvka);

/* CHAMP CARTE */
   memset(entree1, 0, sizeof(entree1));
   memcpy(entree1, "0000", 4);
   memcpy(entree1 + 4, pan , 12);


/*  DECRYPTAGE DU PIN  */
DES(1, 'H', entree1, pvka, pin);

DES(1, 'H', pin, lmk, sortie1 );
memcpy(pin_lmk, sortie1, SIZE_KEY);

error_code[0]='0';
error_code[1]='0';

sprintf(detail, "End   process_ee_cmd(%.2s, %.16s)", error_code, pin_lmk);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_ef_resp						**
  **  desc	: reponse du VHSM a la commande EF			**
  ***********************************************************************/
build_ef_resp(error_code, pin_lmk, resp, resp_len)
unsigned char		error_code[];
unsigned char		pin_lmk[];
unsigned char		resp[];
int			*resp_len;
{
char detail[256];

sprintf(detail, "Start build_ef_resp(%.2s, %.16s)", error_code, pin_lmk);
trace_event(detail, PROCESSING);

memset(resp, 0, sizeof(resp));
*resp_len = 0;

memcpy(resp + *resp_len, "EF", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, pin_lmk, 5 );
*resp_len += 5;

sprintf(detail, "End   build_ef_resp(%d, %.20s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_ee						**
  **  desc	: traitement de la commande DG par VHSM			**
  **  entree	: cmd		-> commande DG				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande DG		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  ***********************************************************************/
v_command_ee(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int			cmd_len;
unsigned char		resp[];
int			*resp_len;
{
unsigned char	pvka_lmk    [SIZE_KEY];
unsigned char	pvv_offset    [12];
unsigned char	pin_length     [SIZE_KEY];
unsigned char	pan         [      12];
unsigned char	dec_table   [SIZE_KEY];
unsigned char	error_code  [       2];
unsigned char	pin_lmk     [       SIZE_KEY];
unsigned char	pin_val_data [      12];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_ee(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(pvka_lmk,     0, sizeof(pvka_lmk));
memset(pvv_offset,   0, sizeof(pvv_offset));
memset(pin_length,   0, sizeof(pin_length));
memset(pan,          0, sizeof(pan));
memset(dec_table,    0, sizeof(dec_table));
memset(error_code,   0, sizeof(error_code));
memset(pin_lmk,      0, sizeof(pin_lmk));
memset(pin_val_data, 0, sizeof(pin_val_data));

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */

retour = check_ee_cmd(cmd, cmd_len, pvka_lmk, pvv_offset, pin_length , pan , dec_table, pin_val_data );
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  GENERATION DU PVV */
    retour = process_ee_cmd( pvka_lmk,
				pvv_offset,
				pin_length,
				pan,
				dec_table,
				pin_val_data,
				error_code,
				pin_lmk);
    if(retour != OK){
	trace_event("End   v_command_ee(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE CX  */
retour = build_ef_resp(error_code, pin_lmk, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_ee(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_ee(%d, %.10s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: check_ua_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande EE						**
  **  entree	: cmd		-> commande				**
  **		      cmd_len	-> langueur de la commande		**
  ***********************************************************************/
check_ua_cmd(cmd, cmd_len, pvka_lmk, pin_length, pan)
unsigned char	cmd[];
int		cmd_len;
unsigned char	pvka_lmk[];
unsigned char	pin_length[];
unsigned char	pan[];
{
int		offset;

char detail[256];

sprintf(detail, "Start check_ua_cmd(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/* VALIDATION DE LA LONGUEUR DU MESSAGE */
if (36 < cmd_len) {
    trace_event("LANGUEUR DE LA COMMANDE INCORRECTE", DATA);
    trace_event("End   check_cw_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  INITIALISATION DES BUFFERS  */
offset = 0;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(pvka_lmk,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pin_length,		cmd + offset,	2);

offset += 2;
memcpy(pan,		        cmd + offset,	12);

sprintf(detail, "End   check_ua_cmd(%.16s, %.2s, %.16s)",
        pvka_lmk, pin_length, pan);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_ua_cmd					**
  **  desc	: traitement de la commande EE par VHSM			**
  ***********************************************************************/
process_ua_cmd(pvka_lmk, pin_length, pan, error_code, pin_lmk)
unsigned char		pvka_lmk[];
unsigned char		pin_length[];
unsigned char		pan[];
unsigned char		error_code[];
unsigned char		pin_lmk[];
{
unsigned char	lmk      [SIZE_KEY];
unsigned char	pin      [SIZE_KEY];
unsigned char	pvka     [SIZE_KEY];
unsigned char	strtmp   [      32];
unsigned char	entree1  [      16];
unsigned char	sortie1  [      16];

char            detail[256];
int		offset, pan_len, len, cpt;

sprintf(detail, "Start process_ua_cmd(%.16s, %.2s, %.16s)",
        pvka_lmk, pin_length, pan);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(lmk,		      0, sizeof(lmk) );
memset(pin,		      0, sizeof(pin) );
memset(pvka,		0, sizeof(pvka));
memset(strtmp,		'0', sizeof(strtmp));
memset(entree1,		0,   sizeof(entree1));
memset(sortie1,		0,   sizeof(sortie1));
offset = 0;

/*  CHARGEMENT DE LA LMK  */
get_lmk(0, lmk);

/*  DECRYPTAGE DE PVKa  */
DES(-1, 'H', pvka_lmk, lmk, pvka);

/* CHAMP CARTE */
memset(entree1, 0, sizeof(entree1));
pan_len = strlen(pan);

if ((pan_len%2) == 1) {
   memcpy(entree1, "0", 1);
   memcpy(entree1 + 1, pan, pan_len);
}
else {
   memcpy(entree1, pan, pan_len);
}

memset(sortie1, 0, sizeof(sortie1));
for (len = cpt = strlen(entree1); cpt >= 0; cpt--){
   if ((cpt%2) == 0)
      sortie1[16 - ((len - cpt)/2) - 1] =  entree1[cpt - 1] & 0x0F;
   else
      sortie1[16 - ((len - cpt)/2) - 1] |= (entree1[cpt - 1] << 4);
}

memcpy(entree1, sortie1, sizeof(sortie1));

/* ENCRYPTAGE */
memset(sortie1, 0, sizeof(sortie1));
DES(1, 'H', entree1, pvka, sortie1 );

memcpy(entree1, sortie1, sizeof(sortie1));

/* DECIMALISATION */
memset(sortie1, 0, sizeof(sortie1));
for(cpt = 0; cpt < 16; cpt ++)
   if(entree1[cpt] >= (unsigned char)'0'
   && entree1[cpt] <= (unsigned char)'9'
     )
      sortie1[cpt] = entree1[cpt];
   else
      sortie1[cpt] = entree1[cpt] - (unsigned char)('A' - '0');

/* PIN */
memset(pin, 0, 16);
/**
pin[0] = sortie1[ 1];
pin[1] = sortie1[ 6];
pin[2] = sortie1[ 8];
pin[3] = sortie1[13];
**/

/* msisdnToCode(pvka, pan+3, pin); */
sprintf(detail, "-->%s<--", pin);
trace_event(detail, FATAL);

/* ENCRYPTAG */
DES(1, 'H', pin, lmk, pin_lmk );

sprintf(error_code, "%s", NO_ERROR);

sprintf(detail, "End   process_ua_cmd(%.2s, %.16s)", error_code, pin_lmk);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_ub_resp						**
  **  desc	: reponse du VHSM a la commande EF			**
  ***********************************************************************/
build_ub_resp(error_code, pin_lmk, resp, resp_len)
unsigned char		error_code[];
unsigned char		pin_lmk[];
unsigned char		resp[];
int			*resp_len;
{
char detail[256];

sprintf(detail, "Start build_ub_resp(%.2s, %.16s)", error_code, pin_lmk);
trace_event(detail, PROCESSING);

*resp_len = 0;

memcpy(resp + *resp_len, "UB", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

if (memcmp(error_code, NO_ERROR, 2) == 0)
{
   memcpy(resp + *resp_len, pin_lmk, strlen(pin_lmk));
   *resp_len += strlen(pin_lmk);
}

sprintf(detail, "End   build_ub_resp(%d, %.20s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_ua						**
  **  desc	: traitement de la commande DG par VHSM			**
  **  entree	: cmd		-> commande DG				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande DG		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  ***********************************************************************/
v_command_ua(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int			cmd_len;
unsigned char		resp[];
int			*resp_len;
{
unsigned char	pvka_lmk    [SIZE_KEY+1];
unsigned char	pvv_offset  [      12+1];
unsigned char	pin_length  [SIZE_KEY+1];
unsigned char	pan         [      12+1];
unsigned char	error_code  [       2+1];
unsigned char	pin_lmk     [SIZE_KEY+1];
unsigned char	pin_val_data[      12+1];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_ua(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(pvka_lmk,     0, sizeof(pvka_lmk));
memset(pvv_offset,   0, sizeof(pvv_offset));
memset(pin_length,   0, sizeof(pin_length));
memset(pan,          0, sizeof(pan));
memset(error_code,   0, sizeof(error_code));
memset(pin_lmk,      0, sizeof(pin_lmk));
memset(pin_val_data, 0, sizeof(pin_val_data));

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_ua_cmd(cmd, cmd_len, pvka_lmk, pin_length, pan);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  GENERATION DU PIN */
    retour = process_ua_cmd( pvka_lmk,   pin_length, pan,
                             error_code, pin_lmk);
    if(retour != OK){
	trace_event("End   v_command_ua(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE CX  */
retour = build_ub_resp(error_code, pin_lmk, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_ua(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_ua(%d, %.10s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

