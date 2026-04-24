  /***********************************************************************
  **  nom	: vhsm.c						**
  **  desc	: fonctions de communication avec l'HSM virtuel		**
  **  auteur	: Younes OUADI (01 Avr 1998)				**
  **				(c) Hightech Payment Systems		**
  ***********************************************************************/
#include <stdio.h>

#include <define.h>
#include <hsm_inc.h>

#undef _FOR_INT_USE_
#undef _FOR_EXT_USE_

#define _FOR_INT_USE_
#include <libvhsm.h>
#undef _FOR_INT_USE_

int v_void(char *cmd, int cmd_len, char *resp, int *resp_len)
{
   trace_event("Start v_void()", PROCESSING);

   trace_event("End   v_void()", PROCESSING);
   return(OK);
}


  /***********************************************************************
  **  nom	: call_virtual_hsm					**
  **  desc	: commande pour l'HSM virtuel				**
  **  entree	: cmd		-> commande pour l'HSM			**
  **		  cmd_len	-> langueur de la commande		**
  **  sortie	: resp		-> reponse de l'HSM			**
  **		  resp_len	-> langueur de la reponse		**
  **  retour	: OK		-> si l'HSM a accepte la commande	**
  **		  NOK		-> l'HSM n'a pas accepte la commande	**
  **		  ERROR		-> fonction non aboutie			**
  **  auteur	: Younes OUADI (01 Avr 1998)				**
  ***********************************************************************/
int call_virtual_hsm(cmd, cmd_len, resp, resp_len)
unsigned char	cmd[];
int		cmd_len;
unsigned char	resp[];
int		*resp_len;
{
int		cpt;
char		detail[256];
int		retour;

sprintf(detail, "Start call_virtual_hsm(%.2s,%d)", cmd, cmd_len);
trace_event(detail, PROCESSING);

cpt = 0;
while(memcmp(sHsmCmd[cpt].command, "EOT", 3) != 0)
{
   if(memcmp(sHsmCmd[cpt].command, cmd, 2) == 0)
   {
      retour = sHsmCmd[cpt].v_command_xx(cmd, cmd_len, resp, resp_len);
      break;
   }
   cpt++;
}

if(memcmp(sHsmCmd[cpt].command, "EOT", 3) == 0)
{
    sprintf(detail, "COMMANDE INCONNUE : %.2s", cmd);
    trace_event(detail, TRACE);

    trace_event("End   call_virtual_hsm(ERROR)", PROCESSING);
    return (ERROR);
}


sprintf(detail, "End   call_virtual_hsm(%d)", retour);
trace_event(detail, PROCESSING);
return (retour);
}

