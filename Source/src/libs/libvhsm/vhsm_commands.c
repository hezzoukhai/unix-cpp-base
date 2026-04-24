#include <stdio.h>
#include <hsm_inc.h>
#include <libvhsm.h>

  /***********************************************************************
  **  nom	: command_UA						**
  **  desc	: construction de la commande UA par VHSM		**
  **  entree	: 
  **              pvka_lmk      -> pvka sous lmk                        **
  **              pan           -> pan                                  **
  **  sortie	: buffer	-> message construit			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **  auteur	: E ENNOUINI (14 Mar 2001)				**
  ***********************************************************************/
int    command_UA( buffer_transmit , pvka_lmk, pan )
char *buffer_transmit;
char *pvka_lmk;
char *pan;
{


	trace_event("Start command_UA()", PROCESSING);

        memcpy ( buffer_transmit , "UA" , 2);
	buffer_transmit += 2;
        memcpy ( buffer_transmit , pvka_lmk , SIZE_KEY);
	buffer_transmit +=SIZE_KEY;
	memcpy ( buffer_transmit , "04" , 2);
	buffer_transmit += 2;
        sprintf( buffer_transmit , "%12s", pan );

	trace_event("End command_UA()", PROCESSING);

	return(OK);
}

  /***********************************************************************
  **  nom	: command_UB						**
  **  desc	: traitement de la commande UA envoye VHSM		**
  **  entree	: buffer	-> message construit			**
  **  sortie	: err_code      -> error code                           **
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: E ENNOUINI (14 Mar 2001)				**
  ***********************************************************************/
int    command_UB( buffer_received , err_code, pin)
char   *buffer_received;
char   *err_code;
char   *pin;
{
char detail[80];
  
  sprintf(detail, "Start command_UB(%s)", buffer_received);
  trace_event(detail, PROCESSING);

	memcpy(err_code, buffer_received + 2, 2);

	if (memcmp(buffer_received, "UB", 2) != 0) {
     trace_event("End   command_UB(ERROR_COMMAND)", PROCESSING); return(ERROR_COMMAND);
}

	if (memcmp(err_code,NO_ERROR,2) != 0) {
	trace_event("End command_UB(CHECK_ERROR)", PROCESSING); return(CHECK_ERROR );
}

        /** memcpy(pin,buffer_received + 4 ,4); **/
        memcpy(pin,buffer_received + 4 ,16);
	
trace_event("End command_UB(OK)", PROCESSING);
	return(OK);

	return(CRYPTOGRAPHIC_ERROR);
}

  /***********************************************************************
  **  nom	: command_BE						**
  **  desc	: construction de la commande BE par VHSM		**
  **  entree	: 
  **              zmk_lmk       -> zmk  sous lmk                        **
  **              pin_zpk       -> pin  sous zpk                        **
  **              pin_format    -> pin  format                          **
  **              pan           -> pan                                  **
  **              pin_lmk       -> pin  sous lmk                        **
  **  sortie	: buffer	-> message construit			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **  auteur	: E ENNOUINI (14 Mar 2001)				**
  ***********************************************************************/
int    command_BE( buffer_transmit, zpk_lmk, pin_zpk, pin_format, pan ,pin_lmk )
char *buffer_transmit;
char *zpk_lmk;
char *pin_zpk;
char *pin_format;
char *pan;
char *pin_lmk;
{


	trace_event("Start command_BE()", PROCESSING);

        memcpy ( buffer_transmit , "BE" , 2);
	buffer_transmit += 2;
        memcpy ( buffer_transmit , zpk_lmk , SIZE_KEY);
	buffer_transmit +=SIZE_KEY;
	memcpy ( buffer_transmit , pin_zpk , SIZE_KEY);
	buffer_transmit += SIZE_KEY;
	memcpy ( buffer_transmit , pin_format , 2);
	buffer_transmit += 2;

        sprintf( buffer_transmit , "%12s", pan );
	buffer_transmit += 12;

	memcpy ( buffer_transmit , pin_lmk , SIZE_KEY);

	trace_event("End command_BE()", PROCESSING);

	return(OK);
}

  /***********************************************************************
  **  nom	: command_BF						**
  **  desc	: traitement de la commande UA envoye VHSM		**
  **  entree	: buffer	-> message construit			**
  **  sortie	: err_code      -> error code                           **
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: E ENNOUINI (14 Mar 2001)				**
  ***********************************************************************/
int    command_BF( buffer_received , err_code)
char   *buffer_received;
char   *err_code;
{

   memcpy(err_code, buffer_received + 2, 2);

   if (memcmp(buffer_received, "BF", 2) != 0) return(ERROR_COMMAND);

   if (memcmp(err_code , "00", 2) == 0) return ( OK );
   return(CRYPTOGRAPHIC_ERROR);
}

  /***********************************************************************
  **  nom	: command_MA						**
  **  desc	: construction de la commande MA par VHSM		**
  **  entree	: 
  **              pvka_lmk      -> ptmk sous lmk                        **
  **              pan           -> pan                                  **
  **  sortie	: buffer	-> message construit			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **  auteur	: E ENNOUINI (14 Mar 2001)				**
  ***********************************************************************/

int    command_MA( buffer_transmit , buffer_transmit_len , data_len, tak_lmk, mac, data)
unsigned char  *buffer_transmit;
int            *buffer_transmit_len;
int	        data_len;
unsigned char	tak_lmk[];
unsigned char	mac[];
unsigned char	data[];
{

	int index = 0 ;
	trace_event("Start command_MA()", PROCESSING);

        memcpy ( buffer_transmit + index , "MA" , 2);
	index += 2;
        memcpy ( buffer_transmit + index , tak_lmk , SIZE_KEY);
	index +=SIZE_KEY;
	memcpy(buffer_transmit + index, data, data_len);
	index +=data_len;

	*buffer_transmit_len = index ;
	trace_event("End command_MA()", PROCESSING);
	return (OK);
}

  /***********************************************************************
  **  nom	: command_MB						**
  **  desc	: traitement de la commande MA envoye VHSM		**
  **  entree	: buffer	-> message construit			**
  **  sortie	: err_code      -> error code                           **
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: E ENNOUINI (14 Mar 2001)				**
  ***********************************************************************/
int    command_MB( buffer_received , err_code, mac)
char   *buffer_received;
char   *err_code;
char   *mac;
{

	trace_event("Start command_MB()", PROCESSING);

   	memcpy(err_code, buffer_received + 2, 2);

	if (memcmp(buffer_received, "MB", 2) != 0) return(ERROR_COMMAND);

	if (memcmp(err_code , "00", 2) == 0) 
	{
 		memcpy(mac,buffer_received + 4 ,8);
		return ( OK );
	}

	trace_event("End command_MB()", PROCESSING);

	return(CRYPTOGRAPHIC_ERROR);
}
  /***********************************************************************
  **  nom	: command_MC						**
  **  desc	: construction de la commande MC par VHSM		**
  **  entree	: 
  **              pvka_lmk      -> ptmk sous lmk                        **
  **              pan           -> pan                                  **
  **  sortie	: buffer	-> message construit			**
  **  retour	: OK	-> fonction aboutie avec succes			**
  **  auteur	: E ENNOUINI (14 Mar 2001)				**
  ***********************************************************************/

int    command_MC( buffer_transmit, buffer_transmit_len , data_len, tak_lmk, mac, data)
unsigned char  *buffer_transmit;
int             *buffer_transmit_len;
int	        data_len;
unsigned char	tak_lmk[];
unsigned char	mac[];
unsigned char	data[];
{

	int index = 0 ;
	char ldata_len [5];

	trace_event("Start command_MC()", PROCESSING);

	memset(ldata_len,0,5);

        memcpy ( buffer_transmit , "MC" , 2);
	index += 2;
        memcpy ( buffer_transmit + index , tak_lmk , SIZE_KEY);
	index +=SIZE_KEY;
	memcpy ( buffer_transmit + index , mac , 8);
	index += 8;
	sprintf(ldata_len,"%d",data_len);
/****
	memcpy(buffer_transmit + index ,ldata_len,strlen(ldata_len));
	index += strlen(ldata_len);
*****/

	memcpy(buffer_transmit + index , data, data_len);
	index += data_len;

	*buffer_transmit_len = index ;

	trace_event("End command_MC()", PROCESSING);
	return (OK);
}

  /***********************************************************************
  **  nom	: command_MD						**
  **  desc	: traitement de la commande MD envoye VHSM		**
  **  entree	: buffer	-> message construit			**
  **  sortie	: err_code      -> error code                           **
  **  retour	: OK	-> fonction aboutie avec succes			**
  **		  NOK	-> fonction aboutie avec echec			**
  **		  ERROR	-> fonction non aboutie				**
  **  auteur	: E ENNOUINI (14 Mar 2001)				**
  ***********************************************************************/
int    command_MD( buffer_received , err_code )
char   *buffer_received;
char   *err_code;
{

	trace_event("Start command_MD()", PROCESSING);

   	memcpy(err_code, buffer_received + 2, 2);

	if (memcmp(buffer_received, "MD", 2) != 0) return(ERROR_COMMAND);
	if (memcmp(err_code,NO_ERROR,2) != 0) return(CHECK_ERROR );
	return(OK);

	trace_event("End command_MD()", PROCESSING);

}
