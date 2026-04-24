  /***********************************************************************
  **  nom	: vhsm_dec_pin.c					**
  **  desc	: decryptage d'une cle					**
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
  **  nom	: check_zc_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenue de la	**
  **		commande ZC						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: key_lmk	-> la cle a encrypter sous la LMK	**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
check_zc_cmd(cmd, cmd_len, key_lmk)
unsigned char	cmd[];
int	cmd_len;
unsigned char	key_lmk[];
{
int		offset;

char        detail[256];

sprintf(detail, "Start check_zc_cmd(%d, %.20s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  VERIFICATION DE LA SYNTAXE  
if(cmd_len != 18){
    trace_event("LANGUEUR DE LA COMMANDE ZC INCORRECTE", DATA);
    trace_event("End   check_zc_cmd(NOK)", PROCESSING);
    return (NOK);
}
*/

/*  INITIALISATION DES BUFFERS  */
offset = 0;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(key_lmk,		cmd + offset,	SIZE_KEY);

sprintf(detail, "End   check_zc_cmd(%16s)", key_lmk);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_zc_cmd					**
  **  desc	: traitement de la commande ZC par VHSM			**
  **  entree	: key_lmk		-> la cle sous la LMK		**
  **  sortie	: error_code	-> code error				**
  **		  key		-> la cle decrypte			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
process_zc_cmd(key_lmk, error_code, key)
unsigned char	key_lmk[];
char		error_code[];
unsigned char	key[];
{
unsigned char	lmk[SIZE_KEY];

char              detail[256];

sprintf(detail, "Start process_zc_cmd(%.16s)", key_lmk);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(lmk,		0, 	SIZE_KEY);

/*  CHARGEMENT DE LA PAIRE LMK  */
get_lmk(0, lmk);

/*  DECRYPTAGE DE LA CLE  */
DES(-1, 'H', key_lmk, lmk, key);

/* ERROR CODE  */
memcpy(error_code, NO_ERROR, 2);

sprintf(detail, "End   process_zc_cmd(%.2s, %.16s)", error_code, key);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_resp_zd						**
  **  desc	: reponse du VHSM a la commande ZC			**
  **  entree	: error_code	-> code erreur				**
  **		  key		-> la cle decryptee			**
  **  sortie	: resp		-> reponse a la commande ZC		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
build_zd_resp(error_code, key, resp, resp_len)
char		error_code[];
unsigned char	key[];
unsigned char	resp[];
int		*resp_len;
{
char         detail[256];

sprintf(detail, "Start build_zd_resp(%.2s, %.16s)", error_code, key);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
*resp_len = 0;

/*  CONSTRUCTION DE LA REPONSE  */
memcpy(resp + *resp_len, "ZD", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, key, SIZE_KEY);
*resp_len += SIZE_KEY;

sprintf(detail, "End   build_zd_resp(%d, %.20s)", error_code, key);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_commcmd_zc						**
  **  desc	: traitement de la commande ZC par VHSM			**
  **  entree	: cmd		-> commande ZC				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande ZC		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
v_command_zc(cmd, cmd_len, resp, resp_len)
unsigned char	cmd[];
int		cmd_len;
unsigned char	resp[];
int		*resp_len;
{
unsigned char	key_lmk    [SIZE_KEY];
char		error_code [       2];
unsigned char	key        [SIZE_KEY];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_zc(%d, %.20s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(key_lmk,      0, sizeof(key_lmk));
memset(error_code,   0, sizeof(error_code));
memset(key,          0, sizeof(key));

/*  VERIFICATION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_zc_cmd(cmd, cmd_len, key_lmk);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  DECRYPTAGE DU PIN BLOCK */
    retour = process_zc_cmd( key_lmk,
				error_code, key);
    if(retour != OK){
	trace_event("End   v_command_zc(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE ZD  */
retour = build_zd_resp( error_code, key,
			resp,       resp_len);
if(retour != OK){
    trace_event("End   v_command_zc(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_zc(%d, %.20s)", *resp_len, resp);
trace_event(detail, PROCESSING);
}

