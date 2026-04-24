  /***********************************************************************
  **  nom	: vhsm_gen_cvv.c					**
  **  desc	: creation du cvv					**
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
  **  nom	: check_cw_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande CW						**
  **  entree	: cmd		-> commande				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: cvka_lmk	-> la CVKa sous la LMK			**
  **		: cvkb_lmk	-> la CVKb sous la LMK			**
  **		  pan		-> le PAN tout entier			**
  **		  expiry_date	-> date d'expiration de la carte	**
  **		  service_code	-> code service de la carte		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (15 Avr 1998)				**
  ***********************************************************************/
check_cw_cmd(cmd, cmd_len, cvka_lmk, cvkb_lmk, pan, expiry_date, service_code)
unsigned char	cmd[];
int	cmd_len;
unsigned char	cvka_lmk[];
unsigned char	cvkb_lmk[];
char		pan[];
unsigned char	expiry_date[];
unsigned char	service_code[];
{
int		i,pan_len;
int		offset;

char        detail[256];

sprintf(detail, "Start check_cw_cmd(%d, %.30s", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  VERIFICATION DE LA SYNTAXE  */
if(cmd_len <= 42){
    trace_event("LANGUEUR DE LA COMMANDE INCORRECTE", DATA);
    trace_event("End   check_cw_cmd(NOK)", PROCESSING);
    return (NOK);
}

offset = 0;

/*  CALCUL DE LA LANGUEUR DU PAN  */
pan_len = cmd_len - 42;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(cvka_lmk,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
memcpy(cvkb_lmk,	cmd + offset, 	SIZE_KEY);

offset += SIZE_KEY;
pan_len = 0;
for ( i = 0 ; i < 22 ; i ++ )
{
  if ( cmd [ offset + i ] == ';' ) break;
  pan[ i] = cmd [ offset + i ];
  pan_len ++;
}

offset += pan_len + 1;
memcpy(expiry_date,	cmd + offset,	4);

offset += 4;
memcpy(service_code,	cmd + offset,	3);

sprintf(detail, "End   check_cw_cmd(%.16s, %.16s, %.19s, %.4s, %.3s)", 
        cvka_lmk, cvkb_lmk, pan, expiry_date, service_code);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_cw_cmd					**
  **  desc	: traitement de la commande CW par VHSM			**
  **  entree	: cvka_lmk	-> la CVKa sous la LMK			**
  **		  cvkb_lmk	-> la CVKb sous la LMK			**
  **		  pan		-> le PAN tout entier			**
  **		  expiry_date	-> date d'expiration de la carte	**
  **		  service_code	-> code service de la carte		**
  **  sortie	: error_code	-> code error				**
  **		  cvv		-> la CVV				**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (15 Avr 1998)				**
  ***********************************************************************/
process_cw_cmd(cvka_lmk, cvkb_lmk, pan, expiry_date, service_code, error_code, cvv)
unsigned char		cvka_lmk[];
unsigned char		cvkb_lmk[];
char			pan[];
unsigned char		expiry_date[];
unsigned char		service_code[];
unsigned char		error_code[];
unsigned char		cvv[];
{
unsigned char	lmk      [SIZE_KEY];
unsigned char	cvka     [SIZE_KEY];
unsigned char	cvkb     [SIZE_KEY];
unsigned char	strtmp   [      32];
unsigned char	entree1  [      16];
unsigned char	entree2  [      16];
unsigned char	sortie1  [      16];
unsigned char	sortie2  [      16];

char        detail[256];
int		offset, cpt1, cpt2;

sprintf(detail, "Start process_cw_cmd((%.16s, %.16s, %.19s, %.4s, %.3s)", 
        cvka_lmk, cvkb_lmk, pan, expiry_date, service_code);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(lmk,		0, SIZE_KEY);
memset(cvka,		0, SIZE_KEY);
memset(cvkb,		0, SIZE_KEY);
memset(strtmp,		'0', sizeof(strtmp));
memset(entree1,		0,   sizeof(entree1));
memset(entree2,		0,   sizeof(entree2));
memset(sortie1,		0,   sizeof(sortie1));
memset(sortie2,		0,   sizeof(sortie2));
offset = 0;

/*  CHARGEMENT DE LA LMK  */
get_lmk(0, lmk);

/*  DECRYPTAGE DE CVKa ET CVKb   */
DES(-1, 'H', cvka_lmk, lmk, cvka);
DES(-1, 'H', cvkb_lmk, lmk, cvkb);

/*  CALCUL DU CVV  */
    /*  STEP 1  */
memcpy(strtmp + offset,	pan,		strlen(pan));
offset += strlen(pan);
memcpy(strtmp + offset,	expiry_date,	4);
offset += 4;
memcpy(strtmp + offset,	service_code,	3);

    /*  STEP 2  */
memcpy(entree1,		strtmp,		16);
memcpy(entree2,		strtmp + 16,	16);

    /*  STEP 3  */
DES( 1, 'H', entree1, cvka, sortie1);

    /*  STEP 4  */
memcpy(entree1, sortie1, 16);
x_or(entree1, entree2, sortie1);
DES( 1, 'H', sortie1, cvka, sortie2);

    /*  STEP 5  */
memcpy(entree1, sortie2, 16);
DES(-1, 'H', entree1, cvkb, sortie1);

    /*  STEP 6  */
memcpy(entree1, sortie1, 16);
DES( 1, 'H', entree1, cvka, sortie1);

    /*  STEP 7  AND STEP 9  */
memcpy(entree1, sortie1, 16);
for(cpt1 = 0, cpt2 = 0; cpt1 < 16; cpt1 ++)
    if(entree1[cpt1] >= (unsigned char)'0'
    && entree1[cpt1] <= (unsigned char)'9')
    {
	sortie1[cpt2] = entree1[cpt1];
	cpt2 ++;
    }

    /*  STEP 8   AND STEP 9  */
if (cpt2 < 2)
  for(cpt1 = 0; cpt1 < 16; cpt1 ++)
    if(entree1[cpt1] >= (unsigned char) 'A'
    && entree1[cpt1] <= (unsigned char) 'F')
    {
	sortie1[cpt2] = entree1[cpt1] - (unsigned char)('A' - '0');
	cpt2 ++;
    }

    /*  STEP 10  */
memcpy(cvv, sortie1, 3);

/*  ERROR CODE  */
memcpy(error_code, NO_ERROR, 2);

sprintf(detail,"End   process_cw_cmd(%.2s, %.3s)", error_code, cvv);
trace_event(detail, PROCESSING); /*EBE140506*/
return (OK);
}

  /***********************************************************************
  **  nom	: build_cx_resp						**
  **  desc	: reponse du VHSM a la commande CW			**
  **  entree	: error_code	-> code erreur				**
  **		  cvv		-> le CVV				**
  **  sortie	: resp		-> reponse a la commande CW		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> syntaxe errornee				**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (15 Avr 1998)				**
  ***********************************************************************/
build_cx_resp(error_code, cvv, resp, resp_len)
unsigned char		error_code[];
unsigned char		cvv[];
unsigned char		resp[];
int			*resp_len;
{
char detail[256];
int offset = 0;

sprintf(detail, "Start build_cx_resp(error_code : %s, cvv : %s )", error_code , cvv );
trace_event (detail, PROCESSING );

offset = 0;

memcpy(resp + offset, "CX", 2);
offset += 2;

memcpy(resp + offset, error_code, 2);
offset += 2;

memcpy(resp + offset, cvv, 3);
offset += 3;

*resp_len = offset;

sprintf(detail, "End   build_cx_resp(%d, %.5s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_cw						**
  **  desc	: traitement de la commande CW par VHSM			**
  **  entree	: cmd		-> commande CW				**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse a la commande CW		**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (15 Avr 1998)				**
  ***********************************************************************/
v_command_cw(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int			cmd_len;
unsigned char		resp[];
int			*resp_len;
{
unsigned char	cvka_lmk    [SIZE_KEY];
unsigned char	cvkb_lmk    [SIZE_KEY];
char		pan         [      23];
unsigned char	expiry_date [       4];
unsigned char	service_code[       3];
unsigned char	error_code  [       2+1];
unsigned char	cvv         [       3];

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_cw(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(cvka_lmk,           0, sizeof(cvka_lmk));
memset(cvkb_lmk,           0, sizeof(cvkb_lmk));
memset(cvv,                0, sizeof(cvv));
memset(pan,                0, sizeof(pan));
memset(expiry_date,        0, sizeof(expiry_date));
memset(service_code,       0, sizeof(service_code));
memset(error_code,         0, sizeof(error_code));

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */
retour = check_cw_cmd(cmd, cmd_len, cvka_lmk, cvkb_lmk, pan, expiry_date, service_code);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  GENERATION DU CVV */
    retour = process_cw_cmd( cvka_lmk,
				cvkb_lmk,
				pan,
				expiry_date,
				service_code,
				error_code,
				cvv);
    if(retour != OK){
	trace_event("End   v_command_cw(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE CX  */
retour = build_cx_resp(error_code, cvv, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_cw(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_cw(%d, %.5s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

