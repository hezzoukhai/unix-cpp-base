  /***********************************************************************
  **  nom	: vhsm_int_pin.c					**
  **  desc	: translation du PIN a partir d'une TPK vers la LMK	**
  **  auteur	: Younes OUADI (18 Avr 1998)				**
  **				(c) Hightech Payment Systems		**
  ***********************************************************************/
#include <stdio.h>

#include <define.h>
#include <hsm_inc.h>

#undef _FOR_INT_USE_ 
#undef _FOR_EXT_USE_ 
                     
#define _FOR_EXT_USE_
#include <libvhsm.h> 
#undef _FOR_EXT_USE_ 

  /***********************************************************************
  **  nom	: check_jc_cmd						**
  **  desc	: verifie la syntaxe et extrait la contenue de la	**
  **		commande JC						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: tpk_lmk	-> la TPK sous la LMK			**
  **		  pin_block_tpk	-> le PIN BLOCK sous la TPK		**
  **		  format	-> format du PIN BLOCK			**
  **		  strpad	-> la chaine de remplissage pour le	**
  **		PIN BLOCK			 			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (18 Avr 1998)				**
  ***********************************************************************/
check_jc_cmd(cmd, cmd_len, tpk_lmk, pin_block_tpk, format, strpad)
unsigned char	cmd[];
int		cmd_len;
unsigned char	tpk_lmk[];
unsigned char	pin_block_tpk[];
char		format[];
char		strpad[];
{
int		offset;

char        detail[256];

sprintf(detail, "Start check_jc_cmd(%d, %.36s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  VERIFICATION DE LA SYNTAXE   */
if(cmd_len != 48){
    trace_event("LANGUEUR DE LA COMMANDE JC INCORRECTE", DATA);
    trace_event("End   check_jc_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  INITIALISATION DES BUFFERS  */
offset = 0;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(tpk_lmk,		cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pin_block_tpk,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(format,		cmd + offset,	2);

offset += 2;
memcpy(strpad,		cmd + offset,	12);

sprintf(detail, "End   check_jc_cmd(%.16s, %.16s, %.2s, %.1s)", tpk_lmk, pin_block_tpk, format, strpad);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_jc_cmd					**
  **  desc	: traitement de la commande JC par VHSM			**
  **  entree	: tpk_lmk	-> la TPK sous la LMK			**
  **		  pin_block_tpk	-> le PIN BLOCK sous la TPK		**
  **		  format	-> le format du PIN BLOCK		**
  **		  strpad	-> la chaine de remplissage pour le	**
  **		PIN BLOCK			 			**
  **  sortie	: error_code	-> code error				**
  **		  pin_lmk	-> le PIN sous la LMK			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (18 Avr 1998)				**
  ***********************************************************************/
process_jc_cmd(tpk_lmk, pin_block_tpk, format, strpad, error_code, pin_lmk)
unsigned char	tpk_lmk[];
unsigned char	pin_block_tpk[];
char		format[];
char		strpad[];
char		error_code[];
unsigned char	pin_lmk[];
{
unsigned char	lmk      [SIZE_KEY];
unsigned char	tpk      [SIZE_KEY];
unsigned char	pin      [SIZE_KEY];
unsigned char	pin_block[SIZE_KEY];
unsigned char	strtmp   [2*SIZE_KEY];

char        detail[256];
int		cpt;

sprintf(detail, "Start process_jc_cmd(%.16s, %.16s, %.2s, %.12s)", tpk_lmk, pin_block_tpk, format, strpad);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(lmk,		0, sizeof(lmk));
memset(tpk,		0, sizeof(tpk));
memset(pin, 		0, sizeof(pin));
memset(pin_block,	0, sizeof(pin_block));
memset(strtmp,		0, sizeof(strtmp));

/*  CHARGEMENT DE LA PAIRE LMK  */
get_lmk(0, lmk);

/*  DECRYPTAGE DE LA TPK  */
DES(-1, 'H', tpk_lmk, lmk, tpk);

/*  DECRYPTAGE DU PIN BLOCK  */
DES(-1, 'H', pin_block_tpk, tpk, pin_block);

/*  EXTRACTION DU PIN  */
fct_pin_block(-1, format, pin, strpad, pin_block);

/*  CRYPTAGE DU PIN SOUS LA LMK  */
DES( 1, 'H', pin, lmk, pin_lmk);

/* ERROR CODE  */
memcpy(error_code, NO_ERROR, 2);

sprintf(detail, "End   process_jc_cmd(%.2s, %.16s)", error_code, pin_lmk);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_jd_resp						**
  **  desc	: reponse du VHSM a la commande JC			**
  **  entree	: error_code	-> code erreur				**
  **		  pin_lmk	-> le PIN sous la LMK			**
  **  sortie	: resp		-> reponse a la commande JC		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (18 Avr 1998)				**
  ***********************************************************************/
build_jd_resp(error_code, pin_lmk, resp, resp_len)
char		error_code[];
unsigned char	pin_lmk[];
unsigned char	resp[];
int		*resp_len;
{
char        detail[256];

sprintf(detail, "Start build_jd_resp((%.2s, %.16s)", error_code, pin_lmk);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
*resp_len = 0;

/*  CONSTRUCTION DE LA REPONSE  */
memcpy(resp + *resp_len, "JD", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, pin_lmk, SIZE_KEY);
*resp_len += SIZE_KEY;

sprintf(detail, "End   build_jd_resp(%d, %.20s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_jc						**
  **  desc	: traitement de la commande JC par VHSM			**
  **  entree	: cmd		-> commande JC				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande JC		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (18 Avr 1998)				**
  ***********************************************************************/
v_command_jc(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int		cmd_len;
unsigned char		resp[];
int		*resp_len;
{
unsigned char	tpk_lmk        [SIZE_KEY];
unsigned char	pin_block_tpk  [SIZE_KEY];
char		format         [       2];
char		strpad         [SIZE_KEY];
char		error_code     [       2];
unsigned char	pin_lmk        [SIZE_KEY];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_jc(%d, %.36s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(tpk_lmk,        0, sizeof(tpk_lmk));
memset(pin_block_tpk,  0, sizeof(pin_block_tpk));
memset(format,         0, sizeof(format));
memset(strpad,         0, sizeof(strpad));
memset(error_code,     0, sizeof(error_code));
memset(pin_lmk,        0, sizeof(pin_lmk));

/*  VERIFICATION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_jc_cmd(cmd,     cmd_len,
		      tpk_lmk, pin_block_tpk, format, strpad);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  EXTRACTION DU PIN  */
    retour = process_jc_cmd( tpk_lmk,    pin_block_tpk, format, strpad,
				error_code, pin_lmk);
    if(retour != OK){
	trace_event("End   v_command_jc(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE HH  */
retour = build_jd_resp( error_code, pin_lmk,
			resp,       resp_len);
if(retour != OK){
    trace_event("End   v_command_jc(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_jc(%d, %20s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}



