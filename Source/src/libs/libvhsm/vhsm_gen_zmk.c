  /***********************************************************************
  **  nom	: vhsm_gen_zmk.c					**
  **  desc	: creation des paires zmk				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
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
  **  nom	: check_gg_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande GG						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: zmk1_lmk	-> la ZMK1 sous la LMK			**
  **		: zmk2_lmk	-> la ZMK2 sous la LMK			**
  **		: zmk3_lmk	-> la ZMK3 sous la LMK			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
check_gg_cmd(cmd, cmd_len, zmk1_lmk, zmk2_lmk, zmk3_lmk)
unsigned char	cmd[];
int	cmd_len;
unsigned char	zmk1_lmk[];
unsigned char	zmk2_lmk[];
unsigned char	zmk3_lmk[];
{
char              detail[256];

sprintf(detail, "Start check_gg_cmd(%d, %.20d)", cmd, cmd_len);
trace_event(detail, PROCESSING);

if(cmd_len != 50){
   trace_event("LANGUEUR INCORRECTE", DATA);
   trace_event("End   check_gg_cmd(NOK)", PROCESSING);
   return (NOK);
}

memcpy(zmk1_lmk, cmd + 2, 		      SIZE_KEY);
memcpy(zmk2_lmk, cmd + 2 + SIZE_KEY, 	SIZE_KEY);
memcpy(zmk3_lmk, cmd + 2 + 2*SIZE_KEY, 	SIZE_KEY);

sprintf(detail, "End   check_gg_cmd(%.16s, %.16s, %.16s)", zmk1_lmk, zmk3_lmk, zmk2_lmk);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_gg_cmd					**
  **  desc	: traitement de la commande GG par VHSM			**
  **  entree	: zmk1_lmk	-> la ZMK1 sous la LMK			**
  **		: zmk2_lmk	-> la ZMK2 sous la LMK			**
  **		: zmk3_lmk	-> la ZMK3 sous la LMK			**
  **  sortie	: error_code	-> code error				**
  **		  zmk_lmk	-> la ZMK sous la LMK			**
  **		  cv		-> la check value			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
process_gg_cmd(zmk1_lmk, zmk2_lmk, zmk3_lmk, error_code, zmk_lmk, cv)
unsigned char		zmk1_lmk[];
unsigned char		zmk2_lmk[];
unsigned char		zmk3_lmk[];
unsigned char		error_code[];
unsigned char		zmk_lmk[];
unsigned char		cv[];
{
unsigned char		lmk     [SIZE_KEY];
unsigned char		zmk     [SIZE_KEY];
unsigned char		zmk1    [SIZE_KEY];
unsigned char		zmk2    [SIZE_KEY];
unsigned char		zmk3    [SIZE_KEY];
unsigned char		strtmp  [SIZE_KEY];

char                    detail[256];
int			      retour;

sprintf(detail, "Start process_gg_cmd(%.16s, %.16s, %.16s)", zmk1_lmk, zmk3_lmk, zmk2_lmk);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(lmk, 	0, 	sizeof(lmk));
memset(zmk, 	0, 	sizeof(zmk));
memset(zmk1, 	0, 	sizeof(zmk1));
memset(zmk2, 	0, 	sizeof(zmk2));
memset(zmk3, 	0, 	sizeof(zmk3));
memset(strtmp, 	0, 	sizeof(strtmp));

/*  CHARGEMENT DE LA LMK  */
retour = get_lmk(0, lmk);
if(retour != OK){
    memcpy(error_code, LMK_ERROR, 2);
    sprintf(detail, "get_lmk() failure : %d", retour);
    trace_event(detail, ERROR);
    trace_event("End   process_gg_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  DECRYPTAGE DES ZCMK  */
DES(-1, 'H', zmk1_lmk, lmk, zmk1);
DES(-1, 'H', zmk2_lmk, lmk, zmk2);
DES(-1, 'H', zmk3_lmk, lmk, zmk3);

/*  CREATION DE LA ZMK  */
x_or(zmk1,	zmk2,	strtmp);
x_or(strtmp,	zmk3,	zmk);

/*  CRYPTAGE DE LA ZMK SOUS LA LMK  */
DES(1, 'H', zmk, lmk, zmk_lmk);

/*  LA CHECK VALUE  */
memset(strtmp, '0', sizeof(strtmp));
DES(1, 'H', strtmp, zmk, cv);

/* ERROR CODE  */
memcpy(error_code, NO_ERROR, 2);

sprintf(detail, "End   process_gg_cmd(%.2s, %.16s, %.16s", error_code, zmk_lmk, cv);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_gh_resp						**
  **  desc	: reponse du VHSM a la commande GG			**
  **  entree	: error_code	-> code erreur				**
  **		  zmk_lmk	-> la ZMK sous la LMK			**
  **		  cv		-> la check value			**
  **  sortie	: resp		-> reponse a la commande GG		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
build_gh_resp(error_code, zmk_lmk, cv, resp, resp_len)
unsigned char		error_code[];
unsigned char		zmk_lmk[];
unsigned char		cv[];
unsigned char		resp[];
int		*resp_len;
{
char detail[256];

sprintf(detail, "Start build_gh_resp(%.2s, %.16s, %.16s)", error_code, zmk_lmk, cv);
trace_event(detail, PROCESSING);

*resp_len = 0;

memcpy(resp + *resp_len, "GH", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, zmk_lmk, SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, cv, SIZE_KEY);
*resp_len += SIZE_KEY;

sprintf(detail, "End   build_gh_resp(%d, %.40s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_gg						**
  **  desc	: traitement de la commande GG par VHSM			**
  **  entree	: cmd		-> commande GG				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande GG		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
v_command_gg(cmd, cmd_len, resp, resp_len)
unsigned char           cmd[];
int                     cmd_len;
unsigned char		resp[];
int                    *resp_len;
{
unsigned char		error_code  [       2];
unsigned char		zmk_lmk     [SIZE_KEY];
unsigned char		cv          [SIZE_KEY];
unsigned char		zmk1_lmk    [SIZE_KEY];
unsigned char		zmk2_lmk    [SIZE_KEY];
unsigned char		zmk3_lmk    [SIZE_KEY];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_gg(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(error_code, 0, sizeof(error_code));
memset(zmk_lmk,    0, sizeof(zmk_lmk));
memset(cv,         0, sizeof(cv));
memset(zmk1_lmk,   0, sizeof(zmk1_lmk));
memset(zmk2_lmk,   0, sizeof(zmk2_lmk));
memset(zmk3_lmk,   0, sizeof(zmk3_lmk));

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_gg_cmd(cmd, cmd_len, zmk1_lmk, zmk2_lmk, zmk3_lmk);
if(retour != OK){
    trace_event("ERROR DE SYNTAXE DE LA COMMANDE GG", PROCESSING);
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  CREATION ET CRYPTAGE, SOUS LA LMK, DE LA ZMK */
    retour = process_gg_cmd( zmk1_lmk,
				zmk2_lmk,
				zmk3_lmk,
				error_code,
				zmk_lmk,
				cv);
    if(retour != OK){
	trace_event("End   v_command_gg(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE GH  */
retour = build_gh_resp(error_code, zmk_lmk, cv, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_gg(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_gg(%d, %.40s)", *resp_len, resp);
trace_event("End   v_command_gg(OK)", PROCESSING);
return (OK);
}

