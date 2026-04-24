  /***********************************************************************
  **  nom	: vhsm_gen_pvv.c					**
  **  desc	: creation du pvv					**
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
  **  nom	: check_dg_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande DG						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: pvka_lmk	-> la PVKa sous la LMK			**
  **		: pvkb_lmk	-> la PVKb sous la LMK			**
  **		  pin_lmk	-> le PIN sous la LMK			**
  **		  pan		-> les 12 caracteres de droite sans cle **
  **		luhn							**
  **		  pvki		-> numero de la paire PVK utulisee	**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  ***********************************************************************/
check_dg_cmd(cmd, cmd_len, pvka_lmk, pvkb_lmk, pin_lmk, pan, pvki)
unsigned char	cmd[];
int	cmd_len;
unsigned char	pvka_lmk[];
unsigned char	pvkb_lmk[];
unsigned char	pin_lmk[];
char		pan[];
unsigned char	pvki;
{
int		offset;

char detail[256];

sprintf(detail, "Start check_cmd_dg(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  VERIFICATION DE LA SYNTAXE   */
if(cmd_len != 53){
    trace_event("LANGUEUR DE LA COMMANDE DG' INCORRECTE", DATA);
    trace_event("End   check_dg_cmd(NOK)", PROCESSING);
    return (NOK);
}

/*  INITIALISATION DES BUFFERS  */
memset(pvka_lmk,	0, sizeof(pvka_lmk));
memset(pvkb_lmk,	0, sizeof(pvkb_lmk));
memset(pin_lmk,		0, sizeof(pin_lmk));
memset(pan,		0, sizeof(pan));
offset = 0;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(pvka_lmk,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pvkb_lmk,	cmd + offset, 	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pin_lmk,		cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(pan,		cmd + offset,	12);

offset += 12;
memcpy(&pvki, 		cmd + offset, 	1);

sprintf(detail, "End   check_dg_cmd(%.16s, %.16s, %.16s, %.19s, %.1s)",
        pvka_lmk, pvkb_lmk, pin_lmk, pan, &pvki);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_dg_cmd					**
  **  desc	: traitement de la commande DG par VHSM			**
  **  entree	: pvka_lmk	-> la PVKa sous la LMK			**
  **		: pvkb_lmk	-> la PVKb sous la LMK			**
  **		  pin_lmk	-> numero d'identification personel	**
  **		  pan		-> les 12 caracteres de droite sans cle **
  **		luhn							**
  **		  pvki		-> numero de la paire PVK utilisee	**
  **  sortie	: error_code	-> code error				**
  **		  pvv		-> le PVV				**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  ***********************************************************************/
process_dg_cmd(pvka_lmk, pvkb_lmk, pin_lmk, pan, pvki, error_code, pvv)
unsigned char		pvka_lmk[];
unsigned char		pvkb_lmk[];
unsigned char		pin_lmk[];
char			pan[];
unsigned char		pvki;
unsigned char		error_code[];
unsigned char		pvv[];
{
unsigned char	lmk      [SIZE_KEY];
unsigned char	pvka     [SIZE_KEY];
unsigned char	pvkb     [SIZE_KEY];
unsigned char	pin      [      16];
unsigned char	strtmp   [      32];
unsigned char	entree1  [      16];
unsigned char	sortie1  [      16];
int		offset, cpt1, cpt2;

char detail[256];

sprintf(detail, "Start process_dg_cmd(%.16s, %.16s, %.16s, %.19s, %.1s)",
        pvka_lmk, pvkb_lmk, pin_lmk, pan, &pvki);
trace_event("Start process_dg_cmd()", PROCESSING);


/*  INITIALISATION DES BUFFERS  */
memset(lmk,		0, sizeof(lmk) );
memset(pvka,		0, sizeof(pvka));
memset(pvkb,		0, sizeof(pvkb));
memset(pin,		0, sizeof(pin) );
memset(strtmp,		'0', sizeof(strtmp));
memset(entree1,		0,   sizeof(entree1));
memset(sortie1,		0,   sizeof(sortie1));
offset = 0;

/*  CHARGEMENT DE LA LMK  */
get_lmk(0, lmk);

/*  DECRYPTAGE DE PVKa ET PVKb   */
DES(-1, 'H', pvka_lmk, lmk, pvka);
DES(-1, 'H', pvkb_lmk, lmk, pvkb);

/*  DECRYPTAGE DU PIN  */
DES(-1, 'H', pin_lmk, lmk, pin);

/*  CALCUL DU PVV  */
    /*  STEP 1  */
memcpy(entree1 + offset,	pan+1,	11);
offset += 11;
entree1[offset] = pvki;
offset += 1;
memcpy(entree1 + offset,	pin,	4);

    /*  STEP 2  */
DES( 1, 'H', entree1, pvka, sortie1);

    /*  STEP 3  */
memcpy(entree1, sortie1, 16);
DES(-1, 'H', entree1, pvkb, sortie1);

    /*  STEP 4  */
memcpy(entree1, sortie1, 16);
DES( 1, 'H', entree1, pvka, sortie1);

    /*  STEP 5  */
memcpy(entree1, sortie1, 16);
for(cpt1 = 0, cpt2 = 0; cpt1 < 16; cpt1 ++)
    if(entree1[cpt1] >= (unsigned char)'0'
    && entree1[cpt1] <= (unsigned char)'9')
    {
	sortie1[cpt2] = entree1[cpt1];
	cpt2 ++;
    }

if (cpt2 < 3)
  for(cpt1 = 0; cpt1 < 16; cpt1 ++)
    if(entree1[cpt1] >= (unsigned char) 'A'
    && entree1[cpt1] <= (unsigned char) 'F')
    {
	sortie1[cpt2] = entree1[cpt1] - (unsigned char)('A' - '0');
	cpt2 ++;
    }

memcpy(pvv, sortie1, 4);

/*  ERROR CODE  */
memcpy(error_code, NO_ERROR, 2);

sprintf(detail, "End   process_dg_cmd(%.2s, %.4s)", error_code, pvv);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_dh_resp						**
  **  desc	: reponse du VHSM a la commande DG			**
  **  entree	: error_code	-> code erreur				**
  **		  pvv		-> le PVV				**
  **  sortie	: resp		-> reponse a la commande DG		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  ***********************************************************************/
build_dh_resp(error_code, pvv, resp, resp_len)
unsigned char		error_code[];
unsigned char		pvv[];
unsigned char		resp[];
int			*resp_len;
{
char detail[256];

sprintf(detail, "Start build_dh_resp(%.2s, %.4s)", error_code, pvv);
trace_event(detail, PROCESSING);

memset(resp, 0, sizeof(resp));
*resp_len = 0;

memcpy(resp + *resp_len, "DH", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, pvv, SIZE_KEY);
*resp_len += SIZE_KEY;

sprintf(detail, "End   build_dh_resp(%d, %.8s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_dg						**
  **  desc	: traitement de la commande DG par VHSM			**
  **  entree	: cmd		-> commande DG				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande DG		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (17 Avr 1998)				**
  ***********************************************************************/
v_command_dg(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int			cmd_len;
unsigned char		resp[];
int			*resp_len;
{
unsigned char	pvka_lmk    [SIZE_KEY];
unsigned char	pvkb_lmk    [SIZE_KEY];
unsigned char	pin_lmk     [SIZE_KEY];
char		pan         [      12];
unsigned char	pvki;
unsigned char	error_code  [       2];
unsigned char	pvv         [       4];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_dg(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(pvka_lmk,    0, sizeof(pvka_lmk));
memset(pvkb_lmk,    0, sizeof(pvkb_lmk));
memset(pin_lmk,     0, sizeof(pin_lmk));
memset(pan,         0, sizeof(pan));
/*EBE140523 memset(pvki,        0, sizeof(pvki));*/
memset(error_code,  0, sizeof(error_code));
memset(pvv,         0, sizeof(pvv));
pvki=0;

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_dg_cmd(cmd, cmd_len, pvka_lmk, pvkb_lmk, pin_lmk, pan, pvki);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  GENERATION DU PVV */
    retour = process_dg_cmd( pvka_lmk,
				pvkb_lmk,
				pin_lmk,
				pan,
				pvki,
				error_code,
				pvv);
    if(retour != OK){
	trace_event("End   v_command_dg(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE CX  */
retour = build_dh_resp(error_code, pvv, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_dg(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_dg(%d, %.8s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

