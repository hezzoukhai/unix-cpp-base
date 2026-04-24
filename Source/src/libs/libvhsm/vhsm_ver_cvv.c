  /***********************************************************************
  **  nom	: vhsm_ver_cvv.c					**
  **  desc	: verification du cvv					**
  **  auteur	: Younes OUADI (15 Avr 1998)				**
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
  **  nom	: check_cy_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande CY						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: cvka_lmk	-> la CVKa sous la LMK			**
  **		: cvkb_lmk	-> la CVKb sous la LMK			**
  **		  cvv		-> le CVV a verifier			**
  **		  pan		-> le PAN tout entier			**
  **		  expiry_date	-> date d'expiration de la carte	**
  **		  service_code	-> code service de la carte		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (15 Avr 1998)				**
  ***********************************************************************/
check_cy_cmd(cmd, cmd_len, cvka_lmk, cvkb_lmk, cvv, pan, expiry_date, service_code)
unsigned char	cmd[];
int	cmd_len;
unsigned char	cvka_lmk[];
unsigned char	cvkb_lmk[];
unsigned char	cvv[];
char		pan[];
char		expiry_date[];
char		service_code[];
{
int		pan_len;
int		offset;

char detail[256];

sprintf(detail, "Start check_cy_cmd(%d, %.20s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  VERIFICATION DE LA SYNTAXE */
if(cmd_len <= 45){
    trace_event("LANGUEUR DE LA COMMANDE INCORRECTE", DATA);
    trace_event("End   check_cy_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  INITIALISATION DES BUFFERS  */
offset = 0;

/*  CALCUL DE LA LANGUEUR DU PAN  */
pan_len = cmd_len - 45;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(cvka_lmk,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(cvkb_lmk,	cmd + offset, 	SIZE_KEY);

offset += SIZE_KEY;
memcpy(cvv,		cmd + offset, 	3);

offset += 3;
memcpy(pan,		cmd + offset,	pan_len);

offset += pan_len + 1;
memcpy(expiry_date,	cmd + offset,	4);

offset += 4;
memcpy(service_code,	cmd + offset,	3);

sprintf(detail, "End   check_cy_cmd(%.16s, %.16s, %.19s, %.3s, %.4s, %.3s)",
                cvka_lmk, cvkb_lmk, cvv, pan, expiry_date, service_code);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_cy_cmd					**
  **  desc	: traitement de la commande CY par VHSM			**
  **  entree	: cvka_lmk	-> la CVKa sous la LMK			**
  **		: cvkb_lmk	-> la CVKb sous la LMK			**
  **		  cvv		-> le CVV a verifier			**
  **		  pan		-> le PAN tout entier			**
  **		  expiry_date	-> date d'expiration de la carte	**
  **		  service_code	-> code service de la carte		**
  **  sortie	: error_code	-> code error				**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (15 Avr 1998)				**
  ***********************************************************************/
process_cy_cmd(cvka_lmk, cvkb_lmk, cvv, pan, expiry_date, service_code, error_code)
unsigned char		cvka_lmk[];
unsigned char		cvkb_lmk[];
unsigned char		cvv[];
char			pan[];
char			expiry_date[];
char			service_code[];
char			error_code[];
{
unsigned char	cvv_gen[SIZE_KEY];	/* CVV genere */

char        detail[256];
int		retour;

sprintf(detail, "Start process_cy_cmd(%.16s, %.16s, %.19s, %.3s, %.4s, %.3s)",
                cvka_lmk, cvkb_lmk, cvv, pan, expiry_date, service_code);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(cvv_gen, 	0, 3);
memcpy(error_code,	NO_ERROR, 2);

/*  GENERATION DU CVV  */
retour = process_cw_cmd(cvka_lmk, cvkb_lmk,
			   pan, expiry_date, service_code,
			   error_code, cvv_gen);
if(retour != OK){
    memcpy(error_code, HSM_VERIFICATION_FAILED, 2);
}
else  /*  COMPARAISON DES DEUX CVV  */
if(memcmp(cvv, cvv_gen, 3) != 0){
    memcpy(error_code, HSM_VERIFICATION_FAILED, 2);
}

sprintf(detail, "End   process_cy_cmd(%.2s)", error_code);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_cz_resp						**
  **  desc	: reponse du VHSM a la commande CY			**
  **  entree	: error_code	-> code erreur				**
  **  sortie	: resp		-> reponse a la commande CY		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (15 Avr 1998)				**
  ***********************************************************************/
build_cz_resp(error_code, resp, resp_len)
char			error_code[];
unsigned char		resp[];
int			*resp_len;
{
char detail[256];

sprintf(detail, "Start build_cz_resp(%.2s)", error_code);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
*resp_len = 0;

memcpy(resp + *resp_len, "CZ", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

sprintf(detail, "End   build_resp_at(%d, %.4s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_cy						**
  **  desc	: traitement de la commande CY par VHSM			**
  **  entree	: cmd		-> commande CY				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande CY		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (15 Avr 1998)				**
  ***********************************************************************/
v_command_cy(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int			cmd_len;
unsigned char		resp[];
int			*resp_len;
{
unsigned char		cvka_lmk    [SIZE_KEY];
unsigned char		cvkb_lmk    [SIZE_KEY];
unsigned char		cvv         [SIZE_KEY];
char			pan         [      22];
char			expiry_date [       4];
char			service_code[       3];
char			error_code  [       2];

char            detail[256];
int		retour;

sprintf(detail, "Start v_command_cy(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(cvka_lmk,           0, sizeof(cvka_lmk));
memset(cvkb_lmk,           0, sizeof(cvkb_lmk));
memset(cvv,                0, sizeof(cvv));
memset(pan,                0, sizeof(pan));
memset(expiry_date,        0, sizeof(expiry_date));
memset(service_code,       0, sizeof(service_code));
memset(error_code,         0, sizeof(error_code));

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_cy_cmd(cmd,      cmd_len,
		      cvka_lmk, cvkb_lmk, cvv, pan, expiry_date, service_code);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  VERIFICATION DU CVV */
    retour = process_cy_cmd( cvka_lmk,
				cvkb_lmk,
				cvv,
				pan,
				expiry_date,
				service_code,
				error_code);
    if(retour != OK){
	trace_event("End   v_command_cy(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE CZ  */
retour = build_cz_resp(error_code, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_cy(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_cy(%d, %.4s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

