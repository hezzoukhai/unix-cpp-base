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
  **  nom	: check_ma_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande MA						**
  **  entree	: cmd		-> commande				**
  **		      cmd_len	-> langueur de la commande		**
  ***********************************************************************/
check_ma_cmd(cmd, cmd_len, tak_lmk, data_len, data)
unsigned char	cmd[];
int	        cmd_len;
unsigned char	tak_lmk[];
int	       *data_len;
unsigned char	data[];
{
int		offset;

char detail[256];

sprintf(detail, "Start check_ma_cmd(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
offset = 0;

/*  EXTRACTION DES CHAMPS  */
offset += 2;
memcpy(tak_lmk,	cmd + offset,	SIZE_KEY);

offset += SIZE_KEY;
*data_len = cmd_len - offset;
memcpy(data, 	cmd + offset, 	*data_len);

sprintf(detail, "End   check_ma_cmd(%.16s, %d, %.40s)", tak_lmk, *data_len, data);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_ma_cmd					**
  **  desc	: traitement de la commande EE par VHSM			**
  ***********************************************************************/
process_ma_cmd(tak_lmk, data_len, data, error_code, mac)
unsigned char	tak_lmk[];
int             data_len;
unsigned char	data[];
unsigned char	error_code[];
unsigned char	mac[];
{
unsigned char   expanded_data[2048];
unsigned char	lmk      [SIZE_KEY];
unsigned char	tak      [SIZE_KEY];
unsigned char	entree1  [SIZE_KEY];
unsigned char	entree2  [SIZE_KEY];
unsigned char	sortie1  [SIZE_KEY];
unsigned char	sortie2  [SIZE_KEY];

char            detail[256];
int		offset, cpt, i, j;

sprintf(detail, "Start process_ma_cmd(%.16s, %d, %.40s)", tak_lmk, data_len, data);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(expanded_data,	0, sizeof(expanded_data) );
memset(lmk,		0, sizeof(lmk) );
memset(tak,		0, sizeof(tak) );
memset(entree1,		0, sizeof(entree1));
memset(entree2,		0, sizeof(entree2));
memset(sortie1,		0, sizeof(sortie1));
memset(sortie2,		0, sizeof(sortie2));
offset = 0;

/*  CHARGEMENT DE LA LMK  */
get_lmk(0, lmk);

/*  DECRYPTAGE DE TAK  */
DES(-1, 'H', tak_lmk, lmk, tak);

memset(entree1, '0', sizeof(entree1));
memset(entree2, '0', sizeof(entree2));
memset(sortie1, 0, sizeof(sortie1));
memset(sortie2, 0, sizeof(sortie2));

/* SPEP 0 : DATA EXPAND */
for (cpt = 0; cpt < data_len; cpt++)
   sprintf(expanded_data + cpt*2, "%02X", data[cpt]);

/* STEP 1 : ENCRYPTION DES 16 PREMIERS BYTES */
memcpy(entree1, expanded_data, (data_len*2 < 16 ? data_len*2 : 16));

i = 1;

DES(1, 'H', entree1, tak, sortie1);
/**/
sprintf(detail, "%d, entree1: %.16s, sortie1: %.16s", i, entree1, sortie1);
trace_event(detail, FATAL);
/**/
memcpy(entree1, sortie1, 16);

for (cpt = 16; cpt < data_len*2; cpt += 16)
{
++i;
   memset(entree2, '0', sizeof(entree2));
   memset(sortie1, 0, sizeof(sortie1));
   memset(sortie2, 0, sizeof(sortie2));

   memcpy(entree2, expanded_data + cpt, (data_len*2 - cpt < 16 ? data_len*2 - cpt : 16));
/**/
sprintf(detail, "%d, entree1: %.16s, entree2: %.16s", i, entree1, entree2);
trace_event(detail, FATAL);
/**/

   /* STEP 2 : XOR AVEC LES 16 SUIVANT */
   x_or(entree1, entree2, sortie1);
/**/
sprintf(detail, "%d, entree1: %.16s, sortie1: %.16s", i, entree1, sortie1);
trace_event(detail, FATAL);
/**/

   /* STEP 3 : ENCRYPTION DU RESULTAT */
   DES(1, 'H', sortie1, tak, sortie2);
/**/
sprintf(detail, "%d, sortie2: %.16s", i, sortie2);
trace_event(detail, FATAL);
/**/

   memcpy(entree1, sortie2, 16);

}

/*  STEP 5 : DECIMALISATION */
/* PAS DE DECIMALIZATION ***************
memset(sortie1, 0, sizeof(sortie1));
for(cpt = 0; cpt < 16; cpt ++)
   if(entree1[cpt] >= (unsigned char)'0'
   && entree1[cpt] <= (unsigned char)'9'
     )
      sortie1[cpt] = entree1[cpt];
   else
      sortie1[cpt] = entree1[cpt] - (unsigned char)('A' - '0');

***************************************/
memcpy(sortie1, entree1, 16);

/* STEP 6 : MAC */
memcpy(mac, sortie1, 8);

sprintf(error_code, "%.2s", NO_ERROR);

sprintf(detail, "End Process_ma_cmd(%.2s, %.8s)", error_code, mac);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_mb_resp						**
  **  desc	: reponse du VHSM a la commande EF			**
  ***********************************************************************/
build_mb_resp(error_code, mac, resp, resp_len)
unsigned char		error_code[];
unsigned char		mac[];
unsigned char		resp[];
int			*resp_len;
{
char detail[256];

sprintf(detail, "Start build_mb_resp(%.2s, %.8s)", error_code, mac);
trace_event(detail, PROCESSING);

memset(resp, 0, sizeof(resp));
*resp_len = 0;

memcpy(resp + *resp_len, "MB", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

memcpy(resp + *resp_len, mac, strlen(mac));
*resp_len += strlen(mac);

sprintf(detail, "End   build_mb_resp(%d, %.20s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_ma						**
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
v_command_ma(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int			cmd_len;
unsigned char		resp[];
int			*resp_len;
{
unsigned char	tak_lmk     [SIZE_KEY];
unsigned char	data        [    1024];
unsigned char	error_code  [       2];
unsigned char	mac         [       9];
int             data_len;

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_ma(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(tak_lmk,     0, sizeof(tak_lmk));
memset(data,        0, sizeof(data));
memset(error_code,  0, sizeof(error_code));
memset(mac,         0, sizeof(mac));

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */

retour = check_ma_cmd(cmd, cmd_len, tak_lmk, &data_len, data);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  GENERATION DU PVV */
    retour = process_ma_cmd( tak_lmk,
				data_len,
				data,
				error_code,
				mac);
    if(retour != OK){
	trace_event("End   v_command_ma(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE CX  */
retour = build_mb_resp(error_code, mac, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_ma(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_ma(%d, %.10s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

