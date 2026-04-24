#include <stdio.h>
#include <hsm_inc.h>
#include <resources.h>


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
/* Date/Auteur : 07/02/02   E.ENNOUINI										  */	
/*			   : 10/10/2003 I.Hassani; H.Youssfi; corection des fonction mac  */							                                  
/******************************************************************************/


int    verify_mac(	
                   int nIndiceCtx,
                   unsigned char *tak_key,
                   unsigned char *mac_data,
                   unsigned char *message_mac,
                   int lg_message_mac,
                   char *sHsmResultCode )
{

	unsigned char  BuffSnd   [LG_MAX];
	unsigned char  szWork   [LG_MAX];
	unsigned char  BuffRcv   [128];
	unsigned char  sVector   [128];
    char           mode [2];
    int 	       i,nLength,nLenWork = 0;
	int      retour,nOffset,nRemain,nIteration;
	pid_t    nPid;
	char ligne[MAX_LINE_TRC];

	unsigned char	error_code             [     3];

	trace_event("Start verif_mac_data()",PROCESSING);

	memset(szWork, 0, sizeof( szWork ) );
	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	memset(sVector, 0, sizeof( sVector ) );
/*
	trace_event("--------------------------------------------", PROCESSING);
	sprintf(ligne, "tak_key            : %.16s", tak_key);
	trace_event(ligne, PROCESSING);
	sprintf(ligne, "mac_data          : %.16s", mac_data);
	trace_event(ligne, PROCESSING);
	trace_event("--------------------------------------------", PROCESSING);
*/ /** HM - 09022010 - PA_DSS **/
	if ( CheckResource ( nIndiceCtx, HSM_VERIF , &nPid ) != OK )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   generate_mac(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

    if (lg_message_mac < 800)
	{
		nLength = command_MW(BuffSnd,'0',tak_key,sVector,mac_data,message_mac,lg_message_mac);

		retour = HsmQueryBin( nIndiceCtx, nPid , BuffSnd,nLength, BuffRcv );
		if ( retour == NOT_AVAILABLE )
		{
			memcpy ( sHsmResultCode , "UNVL", 4 );
			trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
			trace_event("End   generate_mac(HSM_UNAVAILABLE)",PROCESSING);
			return(HSM_UNAVAILABLE);
		}

		if (retour == HSM_MALFUNCTION)
		{
			memcpy ( sHsmResultCode , "UNVL", 4 );
			trace_event("HSM MALFUNCTION DETECTED",ERROR);
			trace_event("End   generate_mac(MALFUNCTION)",PROCESSING);
			return(HSM_MALFUNCTION);
		}

		retour = command_MX(BuffRcv, sHsmResultCode);		
		
		if ( retour != OK )
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

	}

	else
	{
	   nOffset = 0;
	   nIteration =  lg_message_mac / 800 ;

	   for (i=0; i<= nIteration && i <= 3; i++ )
	   {

		sprintf(mode,"%d", i + 1);
		memset(BuffSnd,0,sizeof(BuffSnd));
		memset(BuffRcv,0,sizeof(BuffRcv));
		memset (szWork, 0, sizeof(szWork));
		nRemain  =  lg_message_mac - nOffset;
		if (nRemain > 800)
		{
			nLenWork =  800;
	   		nOffset  =   i * 800;
		}
		else
		{
			nLenWork =  nRemain;
			mode[0] = '3';
			i = 3;
		}

		sprintf(ligne,"Offset is %d|%d", nOffset,nLenWork);
		trace_event(ligne,TRACE);	
		memcpy(szWork, message_mac + nOffset, nLenWork );
		nOffset = ( i + 1) * 800;

		nLength = command_MW(BuffSnd,mode[0],tak_key,sVector,mac_data,szWork,nLenWork);

		retour = HsmQueryBin( nIndiceCtx, nPid , BuffSnd,nLength, BuffRcv );
		if ( retour == NOT_AVAILABLE )
		{
			memcpy ( sHsmResultCode , "UNVL", 4 );
			trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
			trace_event("End   generate_mac(HSM_UNAVAILABLE)",PROCESSING);
			return(HSM_UNAVAILABLE);
		}

		if (retour == HSM_MALFUNCTION)
		{
			memcpy ( sHsmResultCode , "UNVL", 4 );
			trace_event("HSM MALFUNCTION DETECTED",ERROR);
			trace_event("End   generate_mac(MALFUNCTION)",PROCESSING);
			return(HSM_MALFUNCTION);
		}

		retour = command_MX(BuffRcv, sHsmResultCode);


		sprintf(ligne,"The result of command_MX is %d", retour);
		trace_event(ligne,TRACE);	


		if ( retour != OK )
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
	
	   }

	}

	trace_event("End   verif_mac_data(OK)",PROCESSING);
	return(OK);
}


/******************************************************************************/
/* Fonction get_mac_data.                                                     */
/* -----------------------   -                                                */
/* Description : demande de generation du MAC			        			  */
/* Entree      : tak_key	-> la cle TAK				      				  */
/*		 		 mac_data	-> le MAC recu du GAB		  					  */
/* Sortie      : aucune							      						  */
/* Retour      : OK	-> pvv correct					      					  */
/*		 		 NOK	-> mac erronee			      						  */
/* Date/Auteur : 07/02/02 E.ENNOUINI                                          */
/* Modif  10/10/2003 I.Hassani; H.Youssfi                                     */
/******************************************************************************/
int	 generate_mac(
                   int nIndiceCtx,
                   unsigned char *tak_key,
                   unsigned char *mac_data,
                   unsigned char *message_mac,
                   int lg_message_mac,
                   char *sHsmResultCode )
{

	unsigned char  BuffSnd   [LG_MAX];
	unsigned char  BuffRcv   [LG_MAX];
	unsigned char  sVector     [LG_MAX];
	char 	       szWork     [LG_MAX];
    char           mode [2];
	int      retour;
	int      nLength,nLenWork = 0;
	pid_t    nPid;
	int      i, nOffset, nIteration, nRemain = 0;
	char ligne[MAX_LINE_TRC];

	sprintf(ligne,"Start generate_mac(%d)",lg_message_mac);
	trace_event(ligne,PROCESSING);
/*
	trace_event("---------------------------------------------", PROCESSING);
	sprintf(ligne, "tak_key            : %.16s", tak_key);
	trace_event(ligne, PROCESSING);
	trace_event("---------------------------------------------", PROCESSING);
*/ /** HM - 09022010 - PA_DSS **/
	memset(BuffSnd,0,sizeof(BuffSnd));
	memset(BuffRcv,0,sizeof(BuffRcv));
	memset(sVector,0,sizeof(sVector));
	memset(szWork ,0,sizeof(szWork));

	if ( CheckResource ( nIndiceCtx, HSM_VERIF , &nPid ) != OK )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   generate_mac(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

    if (lg_message_mac < 800)
	{
		nLength = command_MU(BuffSnd,'0',tak_key,sVector, message_mac, lg_message_mac);

		retour = HsmQueryBin( nIndiceCtx, nPid , BuffSnd,nLength, BuffRcv );

		if ( retour == NOT_AVAILABLE )
		{
			memcpy ( sHsmResultCode , "UNVL", 4 );
			trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
			trace_event("End   generate_mac(HSM_UNAVAILABLE)",PROCESSING);
			return(HSM_UNAVAILABLE);
		}

		if (retour == HSM_MALFUNCTION)
		{
			memcpy ( sHsmResultCode , "UNVL", 4 );
			trace_event("HSM MALFUNCTION DETECTED",ERROR);
			trace_event("End   generate_mac(MALFUNCTION)",PROCESSING);
			return(HSM_MALFUNCTION);
		}

		retour = command_MV(BuffRcv, sHsmResultCode, mac_data);

		if (retour != OK)
		{
			sprintf(ligne, "ERROR CODE             : %.s", sHsmResultCode);
			trace_event(ligne ,ERROR);
			trace_event("End generate_mac(ERROR)",PROCESSING);
			return(HSM_MALFUNCTION);
		}
	}
	else
	{
	   nOffset = 0;
	   nIteration =  lg_message_mac / 800   ;

	   for (i=0; i<= nIteration && i <= 3; i++ )
	   {
		sprintf(mode,"%d", i + 1);
		memset(BuffSnd,0,sizeof(BuffSnd));
		memset(BuffRcv,0,sizeof(BuffRcv));
		memset (szWork, 0, sizeof(szWork));
		nRemain  =  lg_message_mac - nOffset;
		if (nRemain > 800)
		{
			nLenWork =  800;
	   		nOffset  =   i * 800;
		}
		else
		{
			nLenWork =  nRemain;
			mode[0] = '3';
			i = 3;
		}

		sprintf(ligne,"Offset is %d|%d", nOffset,nLenWork);
		trace_event(ligne,TRACE);	
		memcpy(szWork, message_mac + nOffset, nLenWork );
		nOffset = ( i + 1) * 800;

		nLength = command_MU(BuffSnd,mode [0],tak_key,sVector, szWork, nLenWork);


		retour = HsmQueryBin( nIndiceCtx, nPid , BuffSnd,nLength, BuffRcv );
		if ( retour == NOT_AVAILABLE )
		{
			memcpy ( sHsmResultCode , "UNVL", 4 );
			trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
			trace_event("End   generate_mac(HSM_UNAVAILABLE)",PROCESSING);
			return(HSM_UNAVAILABLE);
		}

		if (retour == HSM_MALFUNCTION)
		{
			memcpy ( sHsmResultCode , "UNVL", 4 );
			trace_event("HSM MALFUNCTION DETECTED",ERROR);
			trace_event("End   generate_mac(MALFUNCTION)",PROCESSING);
			return(HSM_MALFUNCTION);
		}

		retour = command_MV(BuffRcv, sHsmResultCode, sVector);

		if (retour != OK)
		{
			sprintf(ligne, "ERROR CODE             : %.s", sHsmResultCode);
			trace_event(ligne ,ERROR);
			trace_event("End generate_mac(ERROR)",PROCESSING);
			return(HSM_MALFUNCTION);
		}
		memcpy(mac_data, sVector, 8);
	   }
	}

	trace_event("End generate_mac(OK)",PROCESSING);
	return(OK);
}
