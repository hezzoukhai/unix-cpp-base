  /***********************************************************************
  **  nom	: vhsm_util.c						**
  **  desc	: utilitaires pour le VHSM				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  **				(c) Hightech Payment Systems		**
  ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <hsm_inc.h>


#include <define.h>
#include <thr_hsm.h>
#include <queue.h>
#include <errno.h>

#undef _FOR_INT_USE_ 
#undef _FOR_EXT_USE_ 
                     
#define _FOR_EXT_USE_
#include <libvhsm.h> 
#undef _FOR_EXT_USE_ 

static          used = 0;

  /***********************************************************************
  **  nom	: fct_pin_block						**
  **  desc	: fonction de manipulation du PIN et du PIN BLOCK	**
  **  entree	: format	-> format du PIN BLOCK			**
  **		  funct		-> fonction remplie : 			**
  **				 + Du PIN vers le PIN BLOCK : funct = 1	**
  **				 + Du PIN BLOCK vers le PIN : funct = -1**
  **		  pin		-> le PIN				**
  **		  pan		-> les 12 caracteres de droite sans la	**
  **		cle luhn						**
  **		  pin_block	-> le PIN BLOCK				**
  **  sortie	: pin ou pin_block					**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (18 Avr 1998)				**
  ***********************************************************************/
fct_pin_block(funct, format, pin, strpad, pin_block)
int		funct;
char		format[];
unsigned char	pin[];
char		strpad[];
unsigned char	pin_block[];
{
unsigned char	entree1[SIZE_KEY];
unsigned char	entree2[SIZE_KEY];
unsigned char	sortie1[SIZE_KEY];

char		strtmp[10];
int		pin_len;
int		cpt;

char            detail[256];

sprintf(detail,"Start fct_pin_block(%d,%.2s,%s)",funct,format,pin_block);
trace_event(detail, PROCESSING);

if(memcmp(format, ANSI_FORMAT, 2) == 0){
    if(1 == funct){
	pin_len = strlen(pin);
	sprintf((char *)entree1, "0%d%s", pin_len, pin);
	memset(entree1+2+pin_len, 'F', 14 - pin_len);
    }
    else{
	memcpy(entree1,		pin_block,	SIZE_KEY);
    }
    memset(entree2+0,		'0',		4);
    memcpy(entree2+4, strpad, 12);

sprintf(detail, "entree1: %s", entree1);
trace_event(detail, FATAL);
sprintf(detail, "entree2: %s", entree2);
trace_event(detail, FATAL);

    x_or(entree1, entree2, sortie1);

    if(1 == funct){
	memcpy(pin_block,	sortie1,	SIZE_KEY);
    }
    else{
	if( '0' != sortie1[0]){
	    trace_event("PIN BLOCK INCORRECTE", DATA);
	    trace_event("End   fct_pin_block(NOK)", PROCESSING);
	    return (NOK);
	}
	sprintf(strtmp, "%c", sortie1[1]);
	pin_len = atoi(strtmp);
	memcpy(pin,	sortie1 + 2,	pin_len);
trace_event(pin, PROCESSING);
    }
}
else
if(memcmp(format, DIEBOLD_FORMAT, 2) == 0){
    if(1 == funct){
	memset(pin_block, 'F', 16);
	memcpy(pin_block, pin, strlen(pin));
    }
    else{
	cpt = 0;
	while(pin_block[cpt] != 'F' && cpt < 16){
	    pin[cpt] = pin_block[cpt];
	    cpt++;
	}
    }
}
else
if(memcmp(format, BULL_FORMAT, 2) == 0){
    if( 1 == funct){
	pin_len = strlen(pin);
	pin_block[0] = '1';
	sprintf((char *)pin_block, "1%d%s%s",
			   pin_len, pin, 
			   strpad + strlen(strpad) - (14 - pin_len));
    }
    else{
	if( '1' != pin_block[0]){
	    trace_event("PIN BLOCK INCORRECTE", DATA);
	    trace_event("End   fct_pin_block(NOK)", PROCESSING);
	    return (NOK);
	}
	sprintf(strtmp, "%c", pin_block[1]);
	pin_len = atoi(strtmp);
	memcpy(pin, pin_block + 2, pin_len);
    }
}
else{
    trace_event("FORMAT INCORRECTE", DATA);
    trace_event("End   fct_pin_block(NOK)", PROCESSING);
    return (OK);
}

trace_event("End   fct_pin_block(OK)", PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: get_random_key					**
  **  desc	: generation d'une cle aleatoire			**
  **  entree	: aucun							**
  **  sortie	: cle aleatoire						**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
get_random_key(key)
unsigned char	key[];
{
int		cpt;
int		retour;

trace_event("Start get_random_key()", PROCESSING);

/*  INITIALISATION DES BUFFERS  */
memset(key, 0, SIZE_KEY);

/*  INITIALISATION DU PROCESSUS DE GENERATION DES NOMBRES ALEATOIRES  */
if ( used == 0 ) {
			srand((unsigned int) time(NULL));
			used = 1;
		}

/*  GENERATION D'UN NOMBRE ALEATOIRE SUR 16 CARACTERES */
for(cpt = 0; cpt < 4; cpt ++){
    sprintf((char *)key + cpt*4, "%04d", rand() * 9999 / RAND_MAX);
}

/*  AJUSTEMENT DE LA CLE  */
retour = adjust_key(key, 16);
if(retour != OK){
    trace_event("End   get_random_key(NOK)", PROCESSING);
    return (NOK);
}

trace_event("End   get_random_key(OK)", PROCESSING);
return (OK);
}

  /***********************************************************************
  **  nom	: adjust_key						**
  **  desc	: ajustement d'une cle					**
  **  entree	: key		-> la cle a ajuster			**
  **		  key_len	-> langueur de la cle			**
  **  sortie	: la cle ajustee					**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: Younes OUADI (20 Avr 1998)				**
  ***********************************************************************/
adjust_key(key, key_len)
unsigned char	key[];
int		key_len;
{
char            detail[256];
int		cpt;

trace_event("Start adjust_key()", PROCESSING);

/*  VERIFICATION DE LA LANGUEUR  */
if((key_len % 2) == 1){
    sprintf(detail, "THE LENGHT IS NOT EVEN : %d", key_len);
    trace_event(detail, DATA);
    trace_event("End   adjust_key(NOK)", PROCESSING);
    return (NOK);
}
/*  VERIFICATION DU CONTENU  */
for(cpt = 0; cpt < key_len; cpt ++){
    if(key[cpt] < '0' || key[cpt] > 'F')
	cpt = key_len + 1;
    else
    if(key[cpt] < 'A' && key[cpt] > '9')
	cpt = key_len + 1;
}
if(key_len + 2 == cpt){
    sprintf(detail, "THE KEY IS NOT HEXADICIMEL : %.16s", key);
    trace_event(detail, DATA);
    trace_event("End   adjust_key(NOK)", PROCESSING);
    return (NOK);
}

for(cpt = 0; cpt < key_len; cpt += 2){
   if('0' == key[cpt] || '3' == key[cpt] || '5' == key[cpt] || '6' == key[cpt]
   || '9' == key[cpt] || 'A' == key[cpt] || 'C' == key[cpt] || 'F' == key[cpt]){
	if('0' == key[cpt + 1] || '6' == key[cpt + 1]
	|| 'A' == key[cpt + 1] || 'C' == key[cpt + 1])
	    key[cpt + 1] += 1;
	else
	if('3' == key[cpt + 1] || '5' == key[cpt + 1]
	|| 'F' == key[cpt + 1] || '9' == key[cpt + 1])
	    key[cpt + 1] -= 1;
   }
   else
   if('1' == key[cpt] || '2' == key[cpt] || '4' == key[cpt] || '7' == key[cpt]
   || '9' == key[cpt] || 'B' == key[cpt] || 'D' == key[cpt] || 'E' == key[cpt]){
	if('2' == key[cpt + 1] || '4' == key[cpt + 1]
	|| '8' == key[cpt + 1] || 'E' == key[cpt + 1])
	    key[cpt + 1] += 1;
	else
	if('1' == key[cpt + 1] || '7' == key[cpt + 1]
	|| 'B' == key[cpt + 1] || 'D' == key[cpt + 1])
	    key[cpt + 1] -= 1;
   }
}

trace_event("End   adjust_key(OK)", PROCESSING);
return (OK);
}
  /***********************************************************************
  **  nom	: vHsmQuery						**
  **  desc	: Send Request to the Virtual Hsm 			**
  **  entree	: nHsmPid	-> Pid of Hsm    			**
  **		  BuffSnd	-> Buffer conains the command 		**
  **		  lgBuffSnd	-> length of the Buffer  		**
  **  sortie	: BuffSnd	-> Response of the vHsm 		**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: El Mostafa ENNOUINI (20 Mar 2002)			**
  ***********************************************************************/
int vHsmQuery( int nIndiceCtx, pid_t nHsmPid , char *BuffSnd ,int lgBuffSnd , char *BuffRcv )
{
    int        BalStatus;
    TSHsmExchange   sBalMessage;

	memset( tab_HsmCtx[nIndiceCtx].szBufferBal, 0, sizeof(tab_HsmCtx[nIndiceCtx].szBufferBal));
        sBalMessage.sHeader.nIndex     = nIndiceCtx;
        sBalMessage.sHeader.tid        = (int)pthread_self();
        sBalMessage.sHeader.nCoNumber  = GetNextCoNumber(nIndiceCtx);
    
	sBalMessage.sHeader.nPartnerId = nHsmPid;
	sBalMessage.sHeader.nSourceId  = getpid();
	sBalMessage.sHeader.nCommand   = 2100;	/**VHSM_REQUEST**/
	memcpy ( sBalMessage.sBody, BuffSnd ,  lgBuffSnd );
	sBalMessage.sHeader.nLen =  lgBuffSnd;

	if ( WriteBalHsm ( nBalHsm, &sBalMessage) != OK)
	{
		trace_event("Can Not Write Message To Hsm Queue", PROCESSING);
		fprintf(stderr,"Error Write Message  = %d To Hsm Queue =%d  ", errno,nBalHsm);
		trace_event ( "End  HsmQuery ( NOK )", PROCESSING );
		return( HSM_MALFUNCTION );
	}

	memset ( (char *)&sBalMessage , 0, sizeof ( TSHsmExchange ));


	BalStatus = WaitHsmResponse(nIndiceCtx, HSM_TIME_OUT_VALUE, BuffRcv);

	if ( BalStatus  != OK ) 
	{
		trace_event ("Time Out Situation With HSM", PROCESSING);
		trace_event ( "End HsmQuery ( NOK )", PROCESSING );
		return ( NOT_AVAILABLE );
	}

	trace_event ( "End vHsmQuery ( OK )", PROCESSING );
	return ( OK );
}
