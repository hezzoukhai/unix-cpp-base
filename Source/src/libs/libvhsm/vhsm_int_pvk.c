  /***********************************************************************
  **  nom	: vhsm_int_pvk.c					**
  **  desc	: integration des paires pvk				**
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
  **  nom	: check_fc_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande FC						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: zmk_lmk	-> la ZMK sous la LMK			**
  **		  pvk_zmk	-> la CVKa sous la ZMK			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
check_fc_cmd(cmd, cmd_len, zmk_lmk, pvk_zmk)
unsigned char	cmd[];
int	cmd_len;
unsigned char	zmk_lmk[];
unsigned char	pvk_zmk[];
{
char detail[256];

sprintf(detail, "Start check_fc_cmd(%d, %.40s)", cmd_len, cmd);
trace_event("Start check_fc_cmd()", PROCESSING);

/*  VERIFICATION DE LA SYNTAXE   */
if(cmd_len != 34){
    trace_event("LANGUEUR INCORRECTE", DATA);
    trace_event("End   check_fc_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  EXTRACTION DES CHAMPS  */
memcpy(zmk_lmk,		cmd + 2,		SIZE_KEY);
memcpy(pvk_zmk,		cmd + 2 + SIZE_KEY,	SIZE_KEY);

sprintf(detail, "End   check_fc_cmd(%.16s, %.16s)", zmk_lmk, pvk_zmk);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_fd_cmd					**
  **  desc	: traitement de la commande FC par VHSM			**
  **  entree	: zmk_lmk	-> la ZMK sous la LMK			**
  **		  pvk_zmk	-> la PVK sous la ZMK			**
  **  sortie	: error_code	-> code error				**
  **		  pvk_lmk	-> la PVK sous la LMK			**
  **		  cv		-> la check value de la PVK		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
process_fd_cmd(zmk_lmk, pvk_zmk, error_code, pvk_lmk, cv)
unsigned char		zmk_lmk[];
unsigned char		pvk_zmk[];
char			error_code[];
unsigned char		pvk_lmk[];
unsigned char		cv[];
{
unsigned char		lmk   [SIZE_KEY];
unsigned char		zmk   [SIZE_KEY];
unsigned char		pvk  [SIZE_KEY];
unsigned char		strtmp[SIZE_KEY];

char detail[256];

sprintf(detail, "Start process_fd_cmd(%.16s, %.16s)", zmk_lmk, pvk_zmk);
trace_event(detail, PROCESSING);

/* INITIALISATION DES BUFFERS  */
memset(lmk,			0,	sizeof(lmk));
memset(zmk,			0,	sizeof(zmk));
memset(pvk,			0,	sizeof(pvk));
memset(error_code,	0,	sizeof(error_code));

/*  CHARGEMENT DE LA LMK  */
get_lmk(0, lmk);

/*  DECRYPTAGE DE LA ZMK  */
DES(-1, 'H', zmk_lmk, lmk, zmk);

/*  DECRYPTAGE DE PVK  */
DES(-1, 'H', pvk_zmk, zmk, pvk);

/*  CRYPTAGE DE PVK SOUS LMK  */
DES( 1, 'H', pvk, lmk, pvk_lmk);

/*  LES CHECK VALUES  */
memset(strtmp, '0', sizeof(strtmp));
DES( 1, 'H', strtmp, pvk, cv);

/*  L'ERROR_CODE  */
memcpy(error_code, NO_ERROR, 2);

sprintf(detail, "End   process_fd_cmd(%.2s, %.16s, %.16s)", error_code, pvk_lmk, cv);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_resp_fd						**
  **  desc	: reponse du VHSM a la commande FC			**
  **  entree	: error_code	-> code erreur				**
  **		  pvk_lmk	-> la CVKa sous la LMK			**
  **		  cv		-> la check value de la PVKa		**
  **  sortie	: resp		-> reponse a la commande FC		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
build_fd_resp(error_code, pvk_lmk, cv, resp, resp_len)
char			error_code[];
unsigned char		pvk_lmk[];
unsigned char		cv[];
unsigned char		resp[];
int		*resp_len;
{
char detail[256];

sprintf(detail, "Start build_fd_resp(%.2s, %.16s, %.16s)", error_code, pvk_lmk, cv);
trace_event(detail, PROCESSING);

*resp_len = 0;

memcpy(resp + *resp_len, "FD", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, pvk_lmk, SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, cv, SIZE_KEY);
*resp_len += SIZE_KEY;

sprintf(detail, "End   build_fd_resp(%d, %.30s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_fc						**
  **  desc	: traitement de la commande FC par VHSM			**
  **  entree	: cmd		-> commande FC				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande FC		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
v_command_fc(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int			cmd_len;
unsigned char		resp[];
int			*resp_len;
{
unsigned char		zmk_lmk    [SIZE_KEY];
unsigned char		pvk_zmk   [SIZE_KEY];
char			error_code [       2];
unsigned char		pvk_lmk   [SIZE_KEY];
unsigned char		cv 	   [SIZE_KEY];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_aw(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(zmk_lmk,    0, sizeof(zmk_lmk));
memset(pvk_zmk,    0, sizeof(pvk_zmk));
memset(error_code, 0, sizeof(error_code));
memset(pvk_lmk,    0, sizeof(pvk_lmk));
memset(cv,         0, sizeof(cv));


/*  VERIFICATION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_fc_cmd(cmd, cmd_len, zmk_lmk, pvk_zmk);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  CRYPTAGE, SOUS LA LMK, DE LA PVKA ET PVKB */
    retour = process_fd_cmd( zmk_lmk,
				pvk_zmk,
				error_code,
				pvk_lmk,
				cv);
    if(retour != OK){
	trace_event("End   v_command_fc(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE FD  */
retour = build_fd_resp(error_code, pvk_lmk, cv, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_fc(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_aw(%d, %.40s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

