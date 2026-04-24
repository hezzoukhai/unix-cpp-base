#include <stdio.h>
#include <hsm_inc.h>
#include <libvhsm.h>


/******************************************************************************/
/* Fonction verif_mac_data.                                                   */
/* -----------------------   -                                                */
/* Description : demande de veriffication de MAC                              */
/* Entree      : tak_key     -> la cle TAK                                    */
/*               mac_data    -> le MAC du message recu de la BANK             */
/* Sortie      : aucune	                                                      */
/* Retour      : OK     -> mac correct                                        */
/*               NOK	-> mac erronee                                        */
/*               ERROR	-> erreur pendant le traitement                       */
/* Date/Auteur : 07/02/02 E.ENNOUINI                                          */
/******************************************************************************/
int    vhsm_generate_mac(	
                   int nIndiceCtx,
                   unsigned char *tak_key,
                   unsigned char *mac_data,
                   unsigned char *message_mac,
                   int lg_message_mac,
                   char *sHsmResultCode )
{

	unsigned char  BuffSnd   [LG_MAX];
	unsigned char  BuffRcv   [LG_MAX];
	int            lgBuffSnd;
	int      retour;
	pid_t    nPid;
	char ligne[MAX_LINE_TRC];

	trace_event("Start vhsm_generate_mac()",PROCESSING);

	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );

	trace_event("--------------------------------------------", PROCESSING);
	sprintf(ligne, "tak_key            : %.16s", tak_key);
	trace_event(ligne, PROCESSING);
	sprintf(ligne, "len_data            : %d", lg_message_mac);
	trace_event(ligne, PROCESSING);
	trace_event("--------------------------------------------", PROCESSING);

	command_MA(BuffSnd,&lgBuffSnd,lg_message_mac,tak_key,mac_data,message_mac);

	retour = CheckResource ( nIndiceCtx, VHSM_VERIF , &nPid );
	if ( retour != OK )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   vhsm_generate_mac(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	retour = vHsmQuery( nIndiceCtx, nPid , BuffSnd, lgBuffSnd, BuffRcv  );
	if ( retour == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   vhsm_generate_mac(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   vhsm_generate_mac(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	retour = command_MB(BuffRcv, sHsmResultCode, mac_data);

	if ( retour != OK )
	{
		switch ( retour )
		{
			case VERIFICATION_FAILED :
				trace_event("End   vhsm_generate_mac(VERIFICATION_FAILED)",
                                                                PROCESSING);
				return(VERIFICATION_FAILED);

			case ERROR_COMMAND :
				trace_event("End vhsm_generate_mac(ERROR_COMMAND)",PROCESSING);
				return(HSM_MALFUNCTION);

			default :
				trace_event("End   vhsm_generate_mac(CRYPTOGRAPHIC_ERROR)",
                                                                PROCESSING);
				return(CRYPTOGRAPHIC_ERROR);
		}
	}

	trace_event("End vhsm_generate_mac(OK)",PROCESSING);
	return(OK);
}

/******************************************************************************/
/* Fonction get_mac_data.                                                     */
/* -----------------------   -                                                */
/* Description : demande de generation du MAC                                 */
/* Entree      : tak_key      -> la cle TAK                                   */
/* 		 mac_data     -> le MAC recu du GAB                           */
/* Sortie      : aucune                                                       */
/* Retour      : OK	-> pvv correct                                        */
/* 		 NOK	-> mac erronee                                        */
/* Date/Auteur : 07/02/02 E.ENNOUINI                                          */
/******************************************************************************/
int    vhsm_verify_mac(	
                   int nIndiceCtx,
                   unsigned char *tak_key,
                   unsigned char *mac_data,
                   unsigned char *message_mac,
                   int lg_message_mac,
                   char *sHsmResultCode )
{

	unsigned char  BuffSnd   [LG_MAX];
	unsigned char  BuffRcv   [LG_MAX];
	int            lgBuffSnd;
	int      retour;
	pid_t    nPid;
	char ligne[MAX_LINE_TRC];

	sprintf(ligne,"Start vhsm_verify_mac(%d)",lg_message_mac);
	trace_event(ligne,PROCESSING);

	trace_event("---------------------------------------------", PROCESSING);
	sprintf(ligne, "tak_key            : %.8s", tak_key);
	trace_event(ligne, PROCESSING);
	trace_event("---------------------------------------------", PROCESSING);

	memset(BuffSnd,0,sizeof(BuffSnd));
	memset(BuffRcv,0,sizeof(BuffRcv));

	if ( CheckResource ( nIndiceCtx, VHSM_VERIF , &nPid ) != OK )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   vhsm_verify_mac(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	command_MC(BuffSnd, &lgBuffSnd, lg_message_mac, tak_key, mac_data, message_mac);

	retour = vHsmQuery( nIndiceCtx, nPid , BuffSnd, lgBuffSnd, BuffRcv  );
	if ( retour == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   vhsm_verify_mac(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   vhsm_verify_mac(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	retour = command_MD(BuffRcv, sHsmResultCode);

	if (retour != OK)
	{
		switch ( retour )
		{
			case VERIFICATION_FAILED :
				trace_event("End   verif_mac_data(VERIFICATION_FAILED)",
                                                                PROCESSING);
				return(VERIFICATION_FAILED);

			case ERROR_COMMAND :
				trace_event("End verif_mac_data(ERROR_COMMAND)",PROCESSING);
				return(HSM_MALFUNCTION);

			default :
				trace_event("End   verif_mac_data(CRYPTOGRAPHIC_ERROR)",
                                                                PROCESSING);
				return(CRYPTOGRAPHIC_ERROR);
		}
	}

	trace_event("End vhsm_generate_mac(OK)",PROCESSING);
	return(OK);
}
