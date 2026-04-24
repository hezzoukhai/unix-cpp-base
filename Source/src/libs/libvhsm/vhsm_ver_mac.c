  /***********************************************************************
  **  nom	: vhsm_ver_mac.c					**
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
  **  nom	: check_mc_cmd						**
  **  desc	: verifie la syntaxe et extrait le contenu de la	**
  **		commande MA						**
  **  entree	: cmd		-> commande				**
  **		      cmd_len	-> langueur de la commande		**
  ***********************************************************************/
check_mc_cmd(cmd, cmd_len, tak_lmk, mac, data_len, data)
unsigned char	cmd[];
int	        cmd_len;
unsigned char	tak_lmk[];
unsigned char	mac[];
int	       *data_len;
unsigned char	data[];
{
int		offset;

char detail[256];

sprintf(detail, "Start check_mc_cmd(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
offset = 2;

/*  EXTRACTION DES CHAMPS  */
memcpy(tak_lmk,	cmd + offset,	SIZE_KEY);
offset += SIZE_KEY;

memcpy(mac,	cmd + offset,	8);
offset += 8;

*data_len = cmd_len - offset;
memcpy(data, 	cmd + offset, 	*data_len);

sprintf(detail, "End   check_mc_cmd(%.16s, %.8s, %d, %.40s)", tak_lmk, mac, *data_len, data);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: process_mc_cmd					**
  **  desc	: traitement de la commande EE par VHSM			**
  ***********************************************************************/
process_mc_cmd(tak_lmk, mac, data_len, data, error_code)
unsigned char	tak_lmk[];
unsigned char	mac[];
int             data_len;
unsigned char	data[];
unsigned char	error_code[];
{
unsigned char	calc_mac  [9];

char            detail[256];
int		offset, cpt;

sprintf(detail, "Start process_mc_cmd(%.16s, %.8s, %d, %.40s)", tak_lmk, mac, data_len, data);
trace_event(detail, PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(calc_mac,	0, sizeof(calc_mac) );

/* CALCUL DU MAC */
process_ma_cmd(tak_lmk, data_len, data, error_code, calc_mac);

if (memcmp(error_code, NO_ERROR, 2) == 0)
   if (memcmp(mac, calc_mac, 8) != 0)
      memcpy(error_code, HSM_VERIFICATION_FAILED, 2);

sprintf(detail, "End   process_mc_cmd(%.2s)", error_code);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: build_md_resp						**
  **  desc	: reponse du VHSM a la commande EF			**
  ***********************************************************************/
build_md_resp(error_code, resp, resp_len)
unsigned char		error_code[];
unsigned char		resp[];
int			*resp_len;
{
char detail[256];

sprintf(detail, "Start build_md_resp(%.2s)", error_code);
trace_event(detail, PROCESSING);

memset(resp, 0, sizeof(resp));
*resp_len = 0;

memcpy(resp + *resp_len, "MD", 2);
*resp_len += 2;

memcpy(resp + *resp_len, error_code, 2);
*resp_len += 2;

sprintf(detail, "End   build_md_resp(%d, %.4s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: v_command_mc						**
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
v_command_mc(cmd, cmd_len, resp, resp_len)
unsigned char		cmd[];
int			cmd_len;
unsigned char		resp[];
int			*resp_len;
{
unsigned char	tak_lmk     [SIZE_KEY];
unsigned char	mac         [       9];
unsigned char	data        [    1024];
unsigned char	error_code  [       2];
int             data_len;

char        detail[256];
int		retour;

sprintf(detail, "Start v_command_mc(%d, %.40s)", cmd_len, cmd);
trace_event(detail, PROCESSING);

memset(tak_lmk,     0, sizeof(tak_lmk));
memset(mac,         0, sizeof(mac));
memset(data,        0, sizeof(data));
memset(error_code,  0, sizeof(error_code));

/*  VERIFICTAION DE LA SYNTAXE DE LA COMMANDE  */

retour = check_mc_cmd(cmd, cmd_len, tak_lmk, mac, &data_len, data);
if(retour != OK){
    memcpy(error_code, ERROR_IN_INPUT_DATA, 2);
}
else{
    /*  GENERATION DU PVV */
    retour = process_mc_cmd( tak_lmk,
                             mac,
                             data_len,
                             data,
                             error_code);
    if(retour != OK){
	trace_event("End   v_command_mc(NOK)", PROCESSING);
	return (NOK);
    }
}

/*  CONSTRUCTION DE LA REPONSE CX  */
retour = build_md_resp(error_code, resp, resp_len);
if(retour != OK){
    trace_event("End   v_command_mc(NOK)", PROCESSING);
    return (NOK);
}

sprintf(detail, "End   v_command_mc(%d, %.10s)", *resp_len, resp);
trace_event(detail, PROCESSING);
return (OK);
}

