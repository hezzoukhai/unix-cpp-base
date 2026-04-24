  /***********************************************************************
  **  nom	: vhsm_int_cvk.c					**
  **  desc	: integration des paires cvk				**
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
  **  nom	: check_aw_cmd						**
  **  desc	: verifie la syntaxe et extrait la contenue de la	**
  **		commande AW						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: zmk_lmk	-> la ZMK sous la LMK			**
  **		  cvka_zmk	-> la CVKa sous la ZMK			**
  **		  cvkb_zmk	-> la CVKb sous la ZMK			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
check_aw_cmd(cmd, cmd_len, zmk_lmk, cvka_zmk, cvkb_zmk)
unsigned char	cmd[];
int		cmd_len;
unsigned char	zmk_lmk[];
unsigned char	cvka_zmk[];
unsigned char	cvkb_zmk[];
{
char detail[256];

sprintf(detail, "Start check_aw_cmd(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  VERIFICATION DE LA SYNTAXE   */
if(cmd_len != 50){
    trace_event("LANGUEUR INCORRECTE", DATA);
    trace_event("End   check_aw_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  EXTRACTION DES CHAMPS  */
memcpy(zmk_lmk,	cmd + 2,			SIZE_KEY);
memcpy(cvka_zmk,	cmd + 2 + SIZE_KEY,	SIZE_KEY);
memcpy(cvkb_zmk,	cmd + 2 + 2*SIZE_KEY,	SIZE_KEY);

sprintf(detail, "zmk_lmk : %s", zmk_lmk);
trace_event(detail, PROCESSING);
sprintf(detail, "cvka_zmk : %s", cvka_zmk);
trace_event(detail, PROCESSING);
sprintf(detail, "cvkb_zmk : %s", cvkb_zmk);
trace_event(detail, PROCESSING);

sprintf(detail, "End   check_aw_cmd(%.16s, %.16s, %.16s)", zmk_lmk, cvka_zmk, cvkb_zmk);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_ax_cmd					**
  **  desc	: traitement de la commande AW par VHSM			**
  **  entree	: zmk_lmk	-> la ZMK sous la LMK			**
  **		  cvka_zmk	-> la CVKa sous la ZMK			**
  **		  cvkb_zmk	-> la CVKb sous la ZMK			**
  **  sortie	: error_code	-> code error				**
  **		  cvka_lmk	-> la CVKa sous la LMK			**
  **		  cvkb_lmk	-> la CVKb sous la LMK			**
  **		  cva		-> la check value de la CVKa		**
  **		  cvb		-> la check value de la CVKb		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
process_ax_cmd(zmk_lmk, cvka_zmk, cvkb_zmk, error_code, cvka_lmk, cvkb_lmk, cva, cvb)
unsigned char		zmk_lmk[];
unsigned char		cvka_zmk[];
unsigned char		cvkb_zmk[];
unsigned char		error_code[];
unsigned char		cvka_lmk[];
unsigned char		cvkb_lmk[];
unsigned char		cva[];
unsigned char		cvb[];
{
unsigned char		lmk   [SIZE_KEY];
unsigned char		zmk   [SIZE_KEY];
unsigned char		cvka  [SIZE_KEY];
unsigned char		cvkb  [SIZE_KEY];
unsigned char		strtmp[SIZE_KEY];

char                    detail[256];

sprintf(detail, "Start process_ax_cmd(%.16s, %.16s, %.16s)", zmk_lmk, cvka_zmk, cvkb_zmk);
trace_event(detail, PROCESSING);

/* INITIALISATION DES BUFFERS  */
memset(lmk,		0, sizeof(lmk));
memset(zmk,		0, sizeof(zmk));
memset(cvka,	0, sizeof(cvka));
memset(cvkb,	0, sizeof(cvkb));
memset(strtmp,	0, sizeof(strtmp));

/*  CHARGEMENT DE LA LMK  */
get_lmk(0, lmk);

/*  DECRYPTAGE DE LA ZMK  */
DES(-1, 'H', zmk_lmk, lmk, zmk);

/*  DECRYPTAGE DE CVKa ET CVKb  */
DES(-1, 'H', cvka_zmk, zmk, cvka);
DES(-1, 'H', cvkb_zmk, zmk, cvkb);

/*  CRYPTAGE DE CVKa ET CVKb SOUS LMK  */
DES( 1, 'H', cvka, lmk, cvka_lmk);
DES( 1, 'H', cvkb, lmk, cvkb_lmk);

/*  LES CHECK VALUES  */
memset(strtmp, '0', sizeof(strtmp));
DES( 1, 'H', strtmp, cvka, cva);
DES( 1, 'H', strtmp, cvkb, cvb);

/*  L'ERROR_CODE  */
memcpy(error_code, NO_ERROR, 2);

sprintf(detail, "End   process_ax_cmd(%.2s, %.16s, , %.16s, %.16s, %.16s)",
        error_code, cvka_lmk, cvkb_lmk, cva, cvb);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_ax_resp						**
  **  desc	: reponse du VHSM a la commande AW			**
  **  entree	: error_code	-> code erreur				**
  **		  cvka_lmk	-> la CVKa sous la LMK			**
  **		  cvkb_lmk	-> la CVKb sous la LMK			**
  **		  cva		-> la check value de la CVKa		**
  **		  cvb		-> la check value de la CVKb		**
  **  sortie	: resp		-> reponse a la commande AW		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
build_ax_resp(error_code, cvka_lmk, cvkb_lmk, cva, cvb, resp, resp_len)
unsigned char		error_code[];
unsigned char		cvka_lmk[];
unsigned char		cvkb_lmk[];
unsigned char		cva[];
unsigned char		cvb[];
unsigned char		resp[];
int			*resp_len;
{
char detail[256];

sprintf(detail, "End   build_ax_resp(%.2s, %.16s, , %.16s, %.16s, %.16s)",
        error_code, cvka_lmk, cvkb_lmk, cva, cvb);
trace_event(detail, PROCESSING);

*resp_len = 0;

memcpy(resp + *resp_len, "AX", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, cvka_lmk, SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, cvkb_lmk, SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, cva, 6);
*resp_len += 6;

memcpy(resp + *resp_len, cvb, 6);
*resp_len += 6;

sprintf(detail, "End   build_ax_resp(%d, %.40s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_aw						**
  **  desc	: traitement de la commande AW par VHSM			**
  **  entree	: cmd		-> commande AW				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande AW		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
v_command_aw(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int		cmd_len;
unsigned char		resp[];
int		*resp_len;
{
unsigned char		zmk_lmk    [SIZE_KEY + 1];
unsigned char		cvka_zmk   [SIZE_KEY + 1];
unsigned char		cvkb_zmk   [SIZE_KEY + 1];
char                    error_code [       2 + 1];
unsigned char		cvka_lmk   [SIZE_KEY + 1];
unsigned char		cvkb_lmk   [SIZE_KEY + 1];
unsigned char		cva 	   [SIZE_KEY + 1];
unsigned char		cvb 	   [SIZE_KEY + 1];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_aw(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(zmk_lmk,    0, sizeof(zmk_lmk));
memset(cvka_lmk,   0, sizeof(cvka_lmk));
memset(cvkb_lmk,   0, sizeof(cvkb_lmk));
memset(error_code, 0, sizeof(error_code));
memset(cvka_zmk,   0, sizeof(cvka_zmk));
memset(cvkb_zmk,   0, sizeof(cvkb_zmk));
memset(cva,        0, sizeof(cva));
memset(cvb,        0, sizeof(cvb));

/*  VERIFICATION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_aw_cmd(cmd, cmd_len, zmk_lmk, cvka_zmk, cvkb_zmk);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  CRYPTAGE, SOUS LA LMK, DE LA CVKA ET CVKB */
    retour = process_ax_cmd( zmk_lmk,
				cvka_zmk,
				cvkb_zmk,
				error_code,
				cvka_lmk,
				cvkb_lmk,
				cva,
				cvb);
    if(retour != OK){
	trace_event("End   v_command_aw(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE AX  */
retour = build_ax_resp(error_code, cvka_lmk, cvkb_lmk, cva, cvb, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_aw(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_aw(%d, %.40s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

