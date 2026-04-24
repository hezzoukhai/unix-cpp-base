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
  **  nom	: check_be_cmd						**
  **  desc	: verifie la syntaxe et extrait la contenue de la	**
  **		commande JC						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: zpk_lmk	-> la TPK sous la LMK			**
  **		  pin_block_zpk	-> le PIN BLOCK sous la TPK		**
  **		  format	-> format du PIN BLOCK			**
  **		  strpad	-> la chaine de remplissage pour le	**
  **		PIN BLOCK			 			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (18 Avr 1998)				**
  ***********************************************************************/
check_be_cmd(cmd, cmd_len, zpk_lmk, pin_block_zpk, format, pan, lcl_pin_lmk)
unsigned char	cmd[];
int		cmd_len;
unsigned char	zpk_lmk[];
unsigned char	pin_block_zpk[];
char		format[];
char		pan[];
unsigned char	lcl_pin_lmk[];
{
int		offset;

char        detail[256];

sprintf(detail, "Start check_be_cmd(%d, %.36s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  VERIFICATION DE LA SYNTAXE   */
if(cmd_len < 64){
    trace_event("LANGUEUR DE LA COMMANDE JC INCORRECTE", DATA);
    trace_event("End   check_be_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  INITIALISATION DES BUFFERS  */
offset = 0;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(zpk_lmk,		cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pin_block_zpk,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(format,		cmd + offset,	2);

offset += 2;
memcpy(pan,		cmd + offset,	12);

offset += 12;
memcpy(lcl_pin_lmk,	cmd + offset,	16);

sprintf(detail, "End   check_be_cmd(%.16s, %.16s, %.2s, %.12s, %.16s)", zpk_lmk, pin_block_zpk, format, pan, lcl_pin_lmk);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_be_cmd					**
  **  desc	: traitement de la commande JC par VHSM			**
  **  entree	: zpk_lmk	-> la TPK sous la LMK			**
  **		  pin_block_zpk	-> le PIN BLOCK sous la TPK		**
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
process_be_cmd(zpk_lmk, pin_block_zpk, format, pan, lcl_pin_lmk, error_code)
unsigned char	zpk_lmk[];
unsigned char	pin_block_zpk[];
char		format[];
char		pan[];
unsigned char	lcl_pin_lmk[];
char		error_code[];
{
unsigned char	lmk      [SIZE_KEY];
unsigned char	zpk      [SIZE_KEY];
unsigned char	pin      [SIZE_KEY];
unsigned char	calc_pin_lmk[SIZE_KEY];
unsigned char	pin_block[SIZE_KEY];
unsigned char	strtmp   [2*SIZE_KEY];

char        detail[256];
int		cpt;

sprintf(detail, "Start process_be_cmd((%.16s, %.16s, %.2s, %.12s, %.16s)",
        zpk_lmk, pin_block_zpk, format, pan, lcl_pin_lmk);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(calc_pin_lmk,	0, sizeof(calc_pin_lmk));

/* CALCUL DU PIN */
process_jc_cmd(zpk_lmk, pin_block_zpk, format, pan, error_code, calc_pin_lmk);

/* ERROR CODE  */
if (memcmp(lcl_pin_lmk, calc_pin_lmk, SIZE_KEY) != 0)
   memcpy(error_code, HSM_VERIFICATION_FAILED, 2);
else
   memcpy(error_code, NO_ERROR, 2);

sprintf(detail, "End   process_be_cmd(%.2s)", error_code);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_bf_resp						**
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
build_bf_resp(error_code, resp, resp_len)
char		error_code[];
unsigned char	resp[];
int		*resp_len;
{
char        detail[256];

sprintf(detail, "Start build_bf_resp((%.2s)", error_code);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
*resp_len = 0;

/*  CONSTRUCTION DE LA REPONSE  */
memcpy(resp + *resp_len, "BF", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

sprintf(detail, "End   build_bf_resp(%d, %.20s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_be						**
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
v_command_be(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int		cmd_len;
unsigned char		resp[];
int		*resp_len;
{
unsigned char	zpk_lmk        [SIZE_KEY+1];
unsigned char	pin_block_zpk  [SIZE_KEY+1];
char		format         [       2+1];
char		pan            [SIZE_KEY+1];
unsigned char	lcl_pin_lmk    [SIZE_KEY+1];
char		error_code     [       2+1];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_be(%d, %.36s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(zpk_lmk,        0, sizeof(zpk_lmk));
memset(pin_block_zpk,  0, sizeof(pin_block_zpk));
memset(format,         0, sizeof(format));
memset(pan,            0, sizeof(pan));
memset(lcl_pin_lmk,    0, sizeof(lcl_pin_lmk));
memset(error_code,     0, sizeof(error_code));

/*  VERIFICATION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_be_cmd(cmd,     cmd_len,
                      zpk_lmk, pin_block_zpk, format, pan, lcl_pin_lmk);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  EXTRACTION DU PIN  */
    retour = process_be_cmd(zpk_lmk, pin_block_zpk, format, pan, lcl_pin_lmk,
                            error_code);
    if(retour != OK){
	trace_event("End   v_command_be(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE HH  */
retour = build_bf_resp( error_code,
			resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_be(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_be(%d, %20s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}



