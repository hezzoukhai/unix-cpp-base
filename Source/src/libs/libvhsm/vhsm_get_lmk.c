  /***********************************************************************
  **  nom	: v_hsm_get_lmk.c					**
  **  desc	: retourne la LMK					**
  **  auteur	: Younes OUADI (03 Avr 1998)				**
  **					(c) Hightech Payment Systems	**
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
  **  nom	: get_lmk						**
  **  desc	: retourne la LMK					**
  **  entree	: p_pair_number	-> numero de la paire LMK		**
  **  sortie	: p_lmk	-> la LMK					**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **  auteur	: Younes OUADI (03 Avr 1998)				**
  ***********************************************************************/
get_lmk(p_pair_number, p_lmk)
int			p_pair_number;
unsigned char		p_lmk[];
{
unsigned char	*v_lmk[]={	(unsigned char *)"0101010101010101",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637",
				(unsigned char *)"3031323334353637"
			   };

char detail[256];

sprintf(detail, "Start get_lmk(%d)", p_pair_number);
trace_event(detail, PROCESSING);

if((p_pair_number / 2) != 0){
    trace_event("NUMERO DE LA PAIRE LMK ERRORNE", DATA);
    trace_event("End   get_lmk(NOK)", PROCESSING);
    return (NOK);
}

memcpy(p_lmk, v_lmk[p_pair_number / 2], SIZE_KEY);

trace_event("End   get_lmk(OK)", PROCESSING);
return (OK);
}

