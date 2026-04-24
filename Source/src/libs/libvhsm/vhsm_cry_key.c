  /***********************************************************************
  **  nom	: vhsm_cry_pin.c					**
  **  desc	: cryptage d'une cle					**
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
  **  nom	: check_za_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenue de la	**
  **		commande ZA						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: key		-> la cle a encrypter sous la LMK	**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
check_za_cmd(cmd, cmd_len, key)
unsigned char	cmd[];
int	cmd_len;
unsigned char	key[];
{
int		offset;

char detail[256];

sprintf(detail, "Start check_za_cmd(%d, %.20s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  VERIFICATION DE LA SYNTAXE  */
if(cmd_len != 18){
    trace_event("LANGUEUR DE LA COMMANDE ZA INCORRECTE", DATA);
    trace_event("End   check_za_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  INITIALISATION DES BUFFERS  */
offset = 0;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(key,		cmd + offset,	SIZE_KEY);

sprintf(detail, "End   check_za_cmd(%.16s)", key);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_za_cmd					**
  **  desc	: traitement de la commande ZA par VHSM			**
  **  entree	: key		-> la cle a encrypter			**
  **  sortie	: error_code	-> code error				**
  **		  key_lmk	-> la cle sous la LMK			**
  **		  cv		-> la CHECK VALUE			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
process_za_cmd(key, error_code, key_lmk, cv)
unsigned char	key[];
char		error_code[];
unsigned char	key_lmk[];
unsigned char	cv[];
{
unsigned char	lmk[SIZE_KEY];
unsigned char	strtmp[SIZE_KEY];

char        detail[256];
int		retour;

sprintf(detail, "Start process_za_cmd(%.16s)", key);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(lmk,		0, 	SIZE_KEY);
memset(strtmp,		'0',	SIZE_KEY);

/*  CHARGEMENT DE LA PAIRE LMK  */
retour = get_lmk(0, lmk);
if(retour != OK){
    memcpy(error_code, LMK_ERROR, 2);
    trace_event("End   cmd_za_processoing(NOK)", PROCESSING);
    return (NOK);
}
/*  CRYPTAGE DE LA CLE  */
DES( 1, 'H', key, lmk, key_lmk);

/*  LA CHECK VALUE  */
DES( 1, 'H', strtmp, key, cv);

/* ERROR CODE  */
memcpy(error_code, NO_ERROR, 2);

sprintf(detail, "End   process_za_cmd(%.2s, %.16s, %.16s)", error_code, key_lmk, cv);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_resp_zb						**
  **  desc	: reponse du VHSM a la commande ZA			**
  **  entree	: error_code	-> code erreur				**
  **		  key_lmk	-> la cle sous la LMK			**
  **		  cv		-> la CHECK VALUE			**
  **  sortie	: resp		-> reponse a la commande ZA		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
build_za_resp(error_code, key_lmk, cv, resp, resp_len)
char		error_code[];
unsigned char	key_lmk[];
unsigned char	cv[];
unsigned char	resp[];
int		*resp_len;
{
char detail[256];

/*trace_event(detail, "Start build_za_resp(%.2, %.16s, %.16s)", error_code, key_lmk, cv); EBE140506*/
sprintf(detail, "Start build_za_resp(%.2s, %.16s, %.16s)", error_code, key_lmk, cv);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
*resp_len = 0;

/*  CONSTRUCTION DE LA REPONSE  */
memcpy(resp + *resp_len, "ZB", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, key_lmk, SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, cv, SIZE_KEY);
*resp_len += SIZE_KEY;

sprintf(detail, "End   build_za_resp(%d, %.30s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_za						**
  **  desc	: traitement de la commande ZA par VHSM			**
  **  entree	: cmd		-> commande ZA				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande ZA		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
v_command_za(cmd, cmd_len, resp, resp_len)
unsigned char	cmd[];
int		cmd_len;
unsigned char	resp[];
int		*resp_len;
{
unsigned char	key        [SIZE_KEY];
char		error_code [       2];
unsigned char	key_lmk    [SIZE_KEY];
unsigned char	cv         [SIZE_KEY];

char            detail[256];
int		retour;

sprintf(detail, "Start v_command_za(%d, %.20s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(key,          0, sizeof(key));
memset(error_code,   0, sizeof(error_code));
memset(key_lmk,      0, sizeof(key_lmk));
memset(cv,           0, sizeof(cv));

/*  VERIFICATION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_za_cmd(cmd,     cmd_len,
		      key);
if(retour != OK){
    trace_event("ERROR DE SYNTAXE DE LA COMMANDE ZA", PROCESSING);
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  DECRYPTAGE DU PIN BLOCK */
    retour = process_za_cmd( key,
				error_code, key_lmk, cv);
    if(retour != OK){
	trace_event("End   v_command_za(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE HH  */
retour = build_za_resp( error_code, key_lmk, cv,
			resp,       resp_len);
if(retour != OK){
    trace_event("End   v_command_za(NOK)", PROCESSING);
    return (NOK);
}

/*EBE140523 sprintf(detail, "End   v_command_za(%d, %.30s)",(int) resp_len, resp);*/
sprintf(detail, "End   v_command_za(%d, %.30s)",(int) (*resp_len), resp);
trace_event(detail, PROCESSING);
return (OK);
}

