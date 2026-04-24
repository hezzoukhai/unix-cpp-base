#include <stdio.h>
#include <string.h>
#include <define.h>
#include <hsm_inc.h>
#include <resources.h>


int CheckPIN ( int  nIndexCtx ,
               char *sResourceId,
               char *sPhoneNumber,
               char *sPinFormat,
               char *sPinZmk)
{

	char pvka_key    [ 16 + 1 ];
	char pvkb_key    [ 16 + 1 ];
	char awk1_key    [ 16 + 1 ];
	char awk2_key    [ 16 + 1 ];
	char pin_lmk	 [ 16 + 1 ];
	char pvki = '1';
	unsigned char  BuffSnd   [128];
	unsigned char  BuffRcv   [128];
	pid_t    nPid;
 	char ligne[MAX_LINE_TRC];
	char sHsmResultCode [4 + 1 ];
	int  retour;

	sprintf (ligne ,"Start CheckPIN(%12s)", sPhoneNumber );
	trace_event(ligne , PROCESSING);

	memset( pvka_key  , 0, sizeof(pvka_key));
	memset( pvkb_key  , 0, sizeof(pvkb_key));
	memset( awk1_key  , 0, sizeof(awk1_key));
	memset( awk2_key  , 0, sizeof(awk2_key));
	memset( pin_lmk   , 0, sizeof(pin_lmk));

	/********Generation Du Pin **********/

	memset( BuffSnd    , 0, sizeof(BuffSnd));
	memset( BuffRcv    , 0, sizeof(BuffRcv));

	retour = GetPVK  ( nIndexCtx, sPhoneNumber,  pvki, pvka_key ,pvkb_key );

	if ( retour != OK )
	{	
		return(MALFUNCTION);
	}

	command_UA( BuffSnd , pvka_key, sPhoneNumber );

	/*retour = CheckResource ( nIndexCtx, HSM_VERIF , &nPid );*/
	retour = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( retour != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		return(HSM_UNAVAILABLE);
	} 

	retour = HsmQuery( nIndexCtx, nPid , BuffSnd, BuffRcv );
	if ( retour == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		return(HSM_MALFUNCTION);
	}

	retour = command_UB(BuffRcv,sHsmResultCode, pin_lmk);

	if ( retour != OK )
	{
		switch ( retour )
		{
			case ERROR_COMMAND :
				trace_event("End verif_mac_data(ERROR_COMMAND)",PROCESSING);
				return(HSM_MALFUNCTION);

			default :
				trace_event("End   verif_mac_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return(CRYPTOGRAPHIC_ERROR);
 		}
	}

		/********Verification Du Pin **********/

	memset( BuffSnd    , 0, sizeof(BuffSnd));
	memset( BuffRcv    , 0, sizeof(BuffRcv));

	retour = GetAWK1 ( nIndexCtx,sResourceId, awk1_key , awk2_key );

	if ( retour != OK )
	{	
		return(MALFUNCTION);
	}

	command_BE( BuffSnd, awk1_key, sPinZmk, sPinFormat,  sPhoneNumber,pin_lmk );

	/*retour = CheckResource ( nIndexCtx, HSM_VERIF , &nPid );*/
	retour = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( retour != OK ) )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		return(HSM_UNAVAILABLE);
	} 


	retour = HsmQuery( nIndexCtx, nPid , BuffSnd, BuffRcv );
	if ( retour == NOT_AVAILABLE )
	{
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		return(HSM_UNAVAILABLE);
	}

	if (retour == HSM_MALFUNCTION)
	{
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		return(HSM_MALFUNCTION);
	}

	retour = command_BF(BuffRcv,sHsmResultCode);

	if ( retour != OK )
	{
		switch ( retour )
		{
			case ERROR_COMMAND :
				trace_event("End verif_mac_data(ERROR_COMMAND)",PROCESSING);
				return(HSM_MALFUNCTION);

			default :
				trace_event("End   verif_mac_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
				return(CRYPTOGRAPHIC_ERROR);
 		}
	}

	trace_event("End  CheckPIN(OK)",PROCESSING);
	return(retour);
}
