  /***********************************************************************
  **  nom	: vhsm_ver_pvv.c					**
  **  desc	: verification du pvv					**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
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
  **  nom	: check_dc_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande CG						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: tak_lmk	-> la TAK sous la LMK			**
  **		  pvka_lmk	-> la PVKa sous la LMK			**
  **		  pvkb_lmk	-> la PVKb sous la LMK			**
  **		  pin_block_tak	-> numero d'identification personel	**
  **		  format	-> format du PIN BLOCK			**
  **		  pan		-> les 12 caracteres de droite sans cle **
  **		luhn							**
  **		  pvki		-> numero de la paire PVK utulisee	**
  **		  pvv		-> le PVV a verifier			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  ***********************************************************************/
check_dc_cmd(cmd, cmd_len, tak_lmk, pvka_lmk, pvkb_lmk, pin_block_tak, format, pan, pvki, pvv)
unsigned char	cmd[];
int	cmd_len;
unsigned char	tak_lmk[];
unsigned char	pvka_lmk[];
unsigned char	pvkb_lmk[];
unsigned char	pin_block_tak[];
char		format[];
char		pan[];
unsigned char	*pvki;
unsigned char	pvv[];
{
char        detail[256];

int		offset;

sprintf(detail, "Start check_dc_cmd(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  VERIFICATION DE LA SYNTAXE  */
if(cmd_len != 85){
    trace_event("LANGUEUR DE LA COMMANDE 'DC' INCORRECTE", DATA);
    trace_event("End   check_dc_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  INITIALISATION DES BUFFERS  */
offset = 0;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(tak_lmk,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pvka_lmk,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pvkb_lmk,	cmd + offset, 	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pin_block_tak,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(format,		cmd + offset,	2);

offset += 2;
memcpy(pan,		cmd + offset,	12);

offset += 12;
memcpy(pvki,		cmd + offset,	1);

offset += 1;
memcpy(pvv,		cmd + offset,	4);

sprintf(detail, "End   check_dc_cmd(%.16s, %.16s, %.16s, %.16s, %.2s, %.19s, %.1s, %.4s)",
        tak_lmk, pvka_lmk, pvkb_lmk, pin_block_tak, format, pan, pvki, pvv);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_dc_cmd					**
  **  desc	: traitement de la commande CG par VHSM			**
  **  entree	: tak_lmk	-> la TAK sous la LMK			**
  **		  pvka_lmk	-> la PVKa sous la LMK			**
  **		: pvkb_lmk	-> la PVKb sous la LMK			**
  **		  pin_block_tak	-> le PIN BLOCK sous la TAK		**
  **		  format	-> le format di PIN BLOCK		**
  **		  pan		-> les 12 caracteres de droite sans cle **
  **		luhn							**
  **		  pvki		-> numero de la paire PVK utilisee	**
  **		  pvv		-> PVV a verifier			**
  **  sortie	: error_code	-> code error				**
  **		  pvv		-> le PVV				**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  ***********************************************************************/
process_dc_cmd(tak_lmk, pvka_lmk, pvkb_lmk, pin_block_tak, format, pan, pvki, pvv, error_code)
unsigned char		tak_lmk[];
unsigned char		pvka_lmk[];
unsigned char		pvkb_lmk[];
unsigned char		pin_block_tak[];
char			format[];
unsigned char		pan[];
unsigned char		pvki;
unsigned char		pvv[];
unsigned char		error_code[];
{
unsigned char	lmk      [SIZE_KEY];
unsigned char	pin      [SIZE_KEY];
unsigned char	pin_lmk  [SIZE_KEY];
unsigned char	pvv_gen  [SIZE_KEY];

char        detail[256];
int		retour;
/*EBE140523
sprintf(detail, "Start process_dc_cmd(%.16s, %.16s, %.16s, %.16s, %.2s, %.19s, %.1s, %.4s)",
        tak_lmk, pvka_lmk, pvkb_lmk, pin_block_tak, format, pan,(char *) pvki, pvv);
		*/
sprintf(detail, "Start process_dc_cmd(%.16s, %.16s, %.16s, %.16s, %.2s, %.19s, %c, %.4s)",
        tak_lmk, pvka_lmk, pvkb_lmk, pin_block_tak, format, pan,pvki, pvv);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(lmk,		0, sizeof(lmk)    );
memset(pin,		0, sizeof(pin)    );
memset(pin_lmk,	0, sizeof(pin_lmk));
memset(pvv_gen,	0, sizeof(pvv_gen));

memcpy(error_code,	NO_ERROR, 	2              );

/*  EXTRACTION DU PIN  */
retour = process_jc_cmd(tak_lmk, pin_block_tak, format, pan,
		  error_code, pin_lmk);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
    trace_event("End   process_dc_cmd(NOK)", PROCESSING);
    return (NOK);
}

retour = get_lmk(0, lmk);
if(retour != OK){
    memcpy(error_code, LMK_ERROR, 2);
    trace_event("End   process_dc_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  GENERATION DU PVV  */
retour = process_dg_cmd(pvka_lmk, pvkb_lmk, pin_lmk, pan, pvki,
			   error_code, pvv_gen);
if(retour != OK){
    memcpy(error_code, HSM_VERIFICATION_FAILED, 2);
}
else  /*  COMPARAISON DES DEUX PVV  */
if(memcmp(pvv_gen, (unsigned char *)pvv, 4) != 0){
    memcpy(error_code, HSM_VERIFICATION_FAILED, 2);
}

sprintf(detail, "End   process_dc_cmd(%.2s)", error_code);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_dd_resp						**
  **  desc	: reponse du VHSM a la commande CG			**
  **  entree	: error_code	-> code erreur				**
  **  sortie	: resp		-> reponse a la commande CG		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  ***********************************************************************/
build_dd_resp(error_code, resp, resp_len)
unsigned char		error_code[];
unsigned char		resp[];
int			*resp_len;
{
char               detail[256];

sprintf(detail, "Start build_dd_resp(%.2s)", error_code);
trace_event(detail, PROCESSING);

memset(resp, 0, sizeof(resp));
*resp_len = 0;

memcpy(resp + *resp_len, "DD", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

sprintf(detail, "End   build_dd_resp(%d, %.4s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_dc						**
  **  desc	: traitement de la commande DC par VHSM			**
  **  entree	: cmd		-> commande DC				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande DC		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  ***********************************************************************/
v_command_dc(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int			cmd_len;
unsigned char		resp[];
int			*resp_len;
{
unsigned char	tak_lmk      [SIZE_KEY];
unsigned char	pvka_lmk     [SIZE_KEY];
unsigned char	pvkb_lmk     [SIZE_KEY];
unsigned char	pin_block_tak[SIZE_KEY];
char		format       [       2];
char		pan          [      12];
unsigned char	pvki;
unsigned char	error_code   [       2];
unsigned char	pvv          [       4];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_dc(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(tak_lmk,       0, sizeof(tak_lmk));
memset(pvka_lmk,      0, sizeof(pvka_lmk));
memset(pvkb_lmk,      0, sizeof(pvkb_lmk));
memset(pin_block_tak, 0, sizeof(pin_block_tak));
memset(format,        0, sizeof(format));
memset(pan,           0, sizeof(pan));
memset(&pvki,         0, sizeof(pvki));
memset(error_code,    0, sizeof(error_code));
memset(pvv,           0, sizeof(pvv));

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_dc_cmd(cmd,        cmd_len,
		      tak_lmk,    pvka_lmk,
		      pvkb_lmk,   pin_block_tak,
		      format,     pan,
		      &pvki,       pvv);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  GENERATION DU PVV */
    retour = process_dc_cmd( tak_lmk,
				pvka_lmk,
				pvkb_lmk,
				pin_block_tak,
				format,
				pan,
				pvki,
				pvv,
				error_code);
    if(retour != OK){
	trace_event("End   v_command_dc(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE CX  */
retour = build_dd_resp(error_code, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_dc(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_dc(%d, %.4s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

