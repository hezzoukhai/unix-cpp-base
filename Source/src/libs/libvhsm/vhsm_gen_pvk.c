  /***********************************************************************
  **  nom	: vhsm_gen_zmk.c					**
  **  desc	: crefhion des paires zmk				**
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
  **  nom	: check_cmd_fg						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande GG						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (06 Avr 1998)				**
  ***********************************************************************/
check_cmd_fg( cmd, cmd_len , zmk )
unsigned char	cmd[];
int	cmd_len;
char	*zmk;
{
trace_event("Start check_cmd_fg", PROCESSING);
/*
if(cmd_len != 18){
    trace_event("LANGUEUR INCORRECTE", DATA);
    trace_event("End   check_cmd_fg(NOK)", PROCESSING);
    return (NOK);
}
*/

trace_event("End   check_cmd_fg(OK)", PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_resp_fh						**
  **  desc	: reponse du VHSM a la commande GG			**
  **  entree	: error_code	-> code erreur				**
  ***********************************************************************/
build_resp_fh(error_code, pvka_lmk, pvkb_lmk, pvka_zmk, pvkb_zmk, cva , cvb , resp, resp_len)
unsigned char		error_code[];
unsigned char		pvka_lmk[];
unsigned char		pvkb_lmk[];
unsigned char		pvka_zmk[];
unsigned char		pvkb_zmk[];
unsigned char		cva[];
unsigned char		cvb[];
unsigned char		resp[];
int		*resp_len;
{
trace_event("Start build_resp_fh()", PROCESSING);

memset(resp, 0, sizeof(resp));
*resp_len = 0;

memcpy(resp + *resp_len, "FH", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, pvka_lmk , SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, pvkb_lmk , SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, pvka_zmk , SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, pvkb_zmk , SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, cva, SIZE_KEY);
*resp_len += SIZE_KEY;

memcpy(resp + *resp_len, cvb, SIZE_KEY);
*resp_len += SIZE_KEY;

trace_event("End   build_resp_fh(OK)", PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: cmd_fg_processing					**
  **  desc	: traitement de la commande AS par VHSM			**
  ***********************************************************************/
cmd_fg_processing( zmk_lmk, error_code, pvka_lmk, pvkb_lmk, pvka_zmk, pvkb_zmk , cva, cvb )
unsigned char		zmk_lmk[];
unsigned char		error_code[];
unsigned char		pvka_lmk[];
unsigned char		pvkb_lmk[];
unsigned char		pvka_zmk[];
unsigned char		pvkb_zmk[];
unsigned char		cva[];
unsigned char		cvb[];
{
unsigned char		lmk     [SIZE_KEY];
unsigned char		zmk     [SIZE_KEY];
unsigned char		pvk     [SIZE_KEY];
unsigned char		pvk_lmk [SIZE_KEY];
unsigned char		pvk_zmk [SIZE_KEY];
unsigned char		check   [SIZE_KEY];
unsigned char		strtmp  [SIZE_KEY];
int			retour;

trace_event("Start cmd_fg_processing()", PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(zmk, 	0, 	SIZE_KEY);
memset(lmk, 	0, 	SIZE_KEY);
memset(pvk, 	0, 	SIZE_KEY);
memset(pvk_lmk,	0, 	SIZE_KEY);
memset(pvk_zmk,	0, 	SIZE_KEY);
memset(check,   0, 	SIZE_KEY);
memset(strtmp, 	0, 	SIZE_KEY);

/*  CHARGEMENT DE LA LMK  */
retour = get_lmk(0, lmk);
if(retour != OK){
    memcpy(error_code, LMK_ERROR, 2);
    trace_event("End   cmd_fg_processing(NOK)", PROCESSING);
    return (NOK);
}

/*  GENERATION DE LA PVK  */
retour = get_random_key(pvk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_fg_processing(NOK)", PROCESSING);
    return (NOK);
}

/*  CRYPTAGE DE LA pvk SOUS LA LMK  */
retour = DES( 1, 'H', pvk, lmk, pvk_lmk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_fg_processing(NOK)", PROCESSING);
    return (NOK);
}

/*  DECRYPTAGE DE LA ZMK */
DES(-1, 'H', zmk_lmk, lmk, zmk);

/*  CRYPTAGE DE LA pvk SOUS LA ZMK  */
retour = DES( 1, 'H', pvk, zmk, pvk_zmk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_fg_processing(NOK)", PROCESSING);
    return (NOK);
}

memcpy ( pvka_lmk , pvk_lmk , SIZE_KEY );
memcpy ( pvka_zmk , pvk_zmk , SIZE_KEY );

/*  LA CHECK VALUE  */
memset(strtmp, '0', sizeof(strtmp));
DES(1, 'H', strtmp, pvk, cva );


/*  GENERATION DE LA PVK  */
retour = get_random_key(pvk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_fg_processing(NOK)", PROCESSING);
    return (NOK);
}

/*  CRYPTAGE DE LA pvk SOUS LA LMK  */
retour = DES( 1, 'H', pvk, lmk, pvk_lmk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_fg_processing(NOK)", PROCESSING);
    return (NOK);
}

/*  DECRYPTAGE DE LA ZMK */
DES(-1, 'H', zmk_lmk, lmk, zmk);

/*  CRYPTAGE DE LA pvk SOUS LA ZMK  */
retour = DES( 1, 'H', pvk, zmk, pvk_zmk);
if(retour != OK){
    memcpy(error_code, VHSM_ERROR, 2);
    trace_event("End   cmd_fg_processing(NOK)", PROCESSING);
    return (NOK);
}


memcpy ( pvkb_lmk , pvk_lmk , SIZE_KEY );
memcpy ( pvkb_zmk , pvk_zmk , SIZE_KEY );

/*  LA CHECK VALUE  */
memset(strtmp, '0', sizeof(strtmp));
DES(1, 'H', strtmp, pvk, cvb );

/* ERROR CODE  */
memcpy(error_code, NO_ERROR, 2);

trace_event("End   cmd_fg_processing(OK)", PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_fh						**
  ***********************************************************************/
v_command_fg(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int		cmd_len;
unsigned char		resp[];
int		*resp_len;
{
unsigned char		error_code  [       2];
unsigned char		zmk_lmk     [SIZE_KEY];
unsigned char		pvka_lmk    [SIZE_KEY];
unsigned char		pvkb_lmk    [SIZE_KEY];
unsigned char		pvka_zmk    [SIZE_KEY];
unsigned char		pvkb_zmk    [SIZE_KEY];
unsigned char		cva         [SIZE_KEY];
unsigned char		cvb         [SIZE_KEY];

int		retour;

trace_event("Start v_command_fg()", PROCESSING);

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_cmd_fg(cmd, cmd_len, zmk_lmk );
if(retour != OK){
    trace_event("ERROR DE SYNTAXE DE LA COMMANDE GG", PROCESSING);
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  CREATION ET CRYPTAGE, SOUS LA LMK, DE LA ZMK */
    retour = cmd_fg_processing( zmk_lmk ,
                                error_code,
				pvka_lmk,
				pvkb_lmk,
				pvka_zmk,
				pvkb_zmk,
				cva,
				cvb
                                );
    if(retour != OK){
	trace_event("End   v_command_fg(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE FH  */
retour = build_resp_fh( error_code, pvka_lmk, pvkb_lmk, pvka_zmk, pvkb_zmk, cva , cvb , resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_fg(NOK)", PROCESSING);
    return (NOK);
}

trace_event("End   v_command_fg(OK)", PROCESSING);
return (OK);
}

