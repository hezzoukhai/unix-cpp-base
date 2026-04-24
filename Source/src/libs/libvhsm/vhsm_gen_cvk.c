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
  **  nom	: check_as_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande GG						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
check_as_cmd( cmd, cmd_len )
unsigned char	cmd[];
int	cmd_len;
{
char  detail[256];

sprintf(detail, "Start check_as_cmd(%d, %.2s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

if(cmd_len != 2){
    trace_event("LANGUEUR INCORRECTE", DATA);
    trace_event("End   check_as_cmd(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   check_as_cmd(OK)");
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_as_cmd					**
  **  desc	: traitement de la commande AS par VHSM			**
  ***********************************************************************/
process_as_cmd( error_code, cvka, cvkb , cva, cvb )
unsigned char		error_code[];
unsigned char		cvka[];
unsigned char		cvkb[];
unsigned char		cva[];
unsigned char		cvb[];
{
unsigned char		lmk     [SIZE_KEY];
unsigned char		cvk     [SIZE_KEY];
unsigned char		cvk_lmk [SIZE_KEY];
unsigned char		check   [SIZE_KEY];
unsigned char		strtmp  [SIZE_KEY];

char              detail[256];
int			retour;

sprintf(detail, "Start process_as_cmd()");
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(lmk, 	0, sizeof(lmk));
memset(cvk, 	0, sizeof(cvk));
memset(cvk_lmk,	0, sizeof(cvk_lmk));
memset(check,     0, sizeof(check));
memset(strtmp, 	0, sizeof(strtmp));

/*  CHARGEMENT DE LA LMK  */
retour = get_lmk(0, lmk);
if(retour != OK){
    memcpy(error_code, LMK_ERROR, 2);
    trace_event("End   process_as_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  GENERATION DE LA CVK  */
retour = get_random_key(cvk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_eo_processing(NOK)", PROCESSING);
    return (NOK);
}

/*  CRYPTAGE DE LA TMK SOUS LA LMK  */
retour = DES( 1, 'H', cvk, lmk, cvk_lmk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_eo_processing(NOK)", PROCESSING);
    return (NOK);
}

memcpy ( cvka , cvk_lmk , SIZE_KEY );

/*  LA CHECK VALUE  */
memset(strtmp, '0', sizeof(strtmp));
DES(1, 'H', strtmp, cvk, cva );

/*  GENERATION DE LA CVK  */
retour = get_random_key(cvk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_eo_processing(NOK)", PROCESSING);
    return (NOK);
}

/*  CRYPTAGE DE LA TMK SOUS LA LMK  */
retour = DES( 1, 'H', cvk, lmk, cvk_lmk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_eo_processing(NOK)", PROCESSING);
    return (NOK);
}

memcpy ( cvkb , cvk_lmk , SIZE_KEY );

/*  LA CHECK VALUE  */
memset(strtmp, '0', sizeof(strtmp));
DES(1, 'H', strtmp, cvk, cvb );

/* ERROR CODE  */
memcpy(error_code, NO_ERROR, 2);

sprintf(detail, "End   process_as_cmd(%.2s, %.16s, %.16s, %.6s, %.6s)", error_code, cvka, cvkb, cva, cvb);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_at_resp						**
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
build_at_resp(error_code, cvka, cvkb, cva , cvb , resp, resp_len)
unsigned char		error_code[];
unsigned char		cvka[];
unsigned char		cvkb[];
unsigned char		cva[];
unsigned char		cvb[];
unsigned char		resp[];
int		*resp_len;
{
char        detail[256];

sprintf(detail, "Start build_at_resp(%.2s, %.16s, %.16s, %.6s, %.6s)", error_code, cvka, cvkb, cva, cvb);
trace_event(detail, PROCESSING);

memset(resp, 0, sizeof(resp));
*resp_len = 0;

memcpy(resp + *resp_len, "AT", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, cvka, SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, cvkb, SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, cva, SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, cvb, SIZE_KEY);
*resp_len += SIZE_KEY;

/*trace_event("End   build_at_resp(%d, %.40s)", *resp_len, resp); EBE140506*/
sprintf(detail,"End   build_at_resp(%d, %.40s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_at						**
  ***********************************************************************/
v_command_as(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int		cmd_len;
unsigned char		resp[];
int		*resp_len;
{
unsigned char		error_code  [       2];
unsigned char		cvka        [SIZE_KEY];
unsigned char		cvkb        [SIZE_KEY];
unsigned char		cva        [SIZE_KEY];
unsigned char		cvb        [SIZE_KEY];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_as(%d, %.2s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(error_code, 0, sizeof(error_code));
memset(cvka,       0, sizeof(cvka));
memset(cvkb,       0, sizeof(cvkb));
memset(cva,        0, sizeof(cva));
memset(cvb,        0, sizeof(cvb));

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_as_cmd(cmd, cmd_len);
if(retour != OK){
    trace_event("ERROR DE SYNTAXE DE LA COMMANDE GG", PROCESSING);
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  CREATION ET CRYPTAGE, SOUS LA LMK, DE LA ZMK */
    retour = process_as_cmd( error_code,
				cvka,
				cvkb,
				cva,
				cvb);
    if(retour != OK){
	trace_event("End   v_command_as(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE GH  */
retour = build_at_resp( error_code, cvka, cvkb, cva , cvb , resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_as(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_as(%d, %.40s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

