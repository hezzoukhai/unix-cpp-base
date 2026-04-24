  /***********************************************************************
  **  nom	: vhsm_gen_tmk.c					**
  **  desc	: integration de TMK sous la LMK			**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
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
  **  nom	: check_oe_cmd						**
  **  desc	: verifie la syntaxe et extrait la contenue de la	**
  **		commandeOE						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: aucune						**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
check_oe_cmd(cmd, cmd_len)
unsigned char	cmd[];
int	cmd_len;
{
char detail[256];

sprintf(detail, "Start check_oe_cmd(%d, %.2s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

if(cmd_len != 2){
    trace_event("LANGUEUR INCORRECTE DE LA COMMANDEOE", DATA);
    trace_event("End   check_oe_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  AUCUNE DONNEES A EXTRAIRE  */

sprintf(detail, "End   check_oe_cmd(OK)");
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_oe_cmd					**
  **  desc	: traitement de la commandeOE par VHSM			**
  **  entree	: aucune						**
  **  sortie	: error_code	-> code error				**
  **		  tmk_lmk	-> la TMK sous la LMK			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
process_oe_cmd(error_code, tmk_lmk)
char			error_code[];
unsigned char		tmk_lmk[];
{
unsigned char		lmk[SIZE_KEY + 1];
unsigned char		tmk[SIZE_KEY + 1];

char              detail[256];
int			retour;

sprintf(detail, "Start process_oe_cmd()");
trace_event(detail, PROCESSING);

memset(lmk,		0,	sizeof(lmk));

/*  CHARGEMENT DE LA LMK  */
retour = get_lmk(0, lmk);
if(retour != OK){
    memcpy(error_code, LMK_ERROR, 2);
    trace_event("End   cmd_eo_processing(NOK)", PROCESSING);
    return (NOK);
}

/*  GENERATION DE LA TMK  */
retour = get_random_key(tmk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_eo_processing(NOK)", PROCESSING);
    return (NOK);
}

/*  CRYPTAGE DE LA TMK SOUS LA LMK  */
retour = DES( 1, 'H', tmk, lmk, tmk_lmk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_eo_processing(NOK)", PROCESSING);
    return (NOK);
}

/*  ERROR CODE  */
memcpy(error_code, NO_ERROR, 2);

sprintf(detail, "End   process_oe_cmd(%.2s, %.16s)", error_code, tmk_lmk);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_of_resp						**
  **  desc	: reponse du VHSM a la commandeOE			**
  **  entree	: error_code	-> code erreur				**
  **		  tmk_lmk	-> la TMK sous la LMK			**
  **  sortie	: resp		-> reponse a la commandeOE		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (03 Avr 1998)				**
  ***********************************************************************/
build_of_resp(error_code, tmk_lmk, resp, resp_len)
char			error_code[];
unsigned char		tmk_lmk[];
unsigned char		resp[];
int		*resp_len;
{
char        detail[256];

sprintf(detail, "Start build_of_resp(%.2s, %.16s)", error_code, tmk_lmk);
trace_event(detail, PROCESSING);

*resp_len = 0;

memcpy(resp + *resp_len, "OF", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, tmk_lmk, SIZE_KEY);
*resp_len += SIZE_KEY;

sprintf(detail, "End   build_of_resp(%d, %.20s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_oe						**
  **  desc	: traitement de la commandeOE par VHSM			**
  **  entree	: cmd		-> commandeOE				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commandeOE		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
v_command_oe(cmd, cmd_len, resp, resp_len)
unsigned char	cmd[];
int		cmd_len;
unsigned char	resp[];
int		*resp_len;
{
unsigned char		tmk_lmk	    [SIZE_KEY];
char			error_code  [       2];

char              detail[256];
int			retour;

sprintf(detail, "Start v_command_oe(%d, %.2s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(tmk_lmk   , 0, sizeof(tmk_lmk));
memset(error_code, 0, sizeof(error_code));

/*
  VERIFICATION DE LA SYNTAXE DE LA COMMANDE  
*/
retour = check_oe_cmd(cmd, cmd_len);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*
       GENERATION DE LA TMK 
    */
    retour = process_oe_cmd( error_code, tmk_lmk);
    if(retour != OK){
	trace_event("End   v_command_oe(NOK)", PROCESSING);
	return (NOK);
    }
}

/*
  CONSTRUCTION DE LA REPONSE MJ  
*/
retour = build_of_resp(error_code, tmk_lmk, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_oe(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_oe(%d, %.20s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

