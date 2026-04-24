#include <stdio.h>
#include <string.h>
#include <hsm_inc.h>
#include <resources.h>

/******************************************************************************/
/* Fonction verif_mac_data.                                                   */
/* -----------------------   -                                                */
/* Description : demande de veriffication de MAC                              */
/* Entree      : tak_key     -> la cle TAK                                    */
/* mac_data    -> le MAC du message recu de la BANK             */
/* Sortie      : aucune	                                                      */
/* Retour      : OK     -> mac correct                                        */
/* NOK	-> mac erronee                                        */
/* ERROR	-> erreur pendant le traitement                       */
/* Date/Auteur : 07/02/02   E.ENNOUINI										  */
/* : 10/10/2003 I.Hassani; H.Youssfi; corection des fonction mac  */
/******************************************************************************/


/***********************************************************/
int verify_msg_auth_code(
	int				nIndiceCtx,
	unsigned char	format, 
	unsigned char*	algorithm, 
	unsigned char	padMethod,
	unsigned char*	keyTpe,
	unsigned char	*takzak_key,
	unsigned char	*mac_data,
	unsigned char	*message_mac,
	int				lg_message_mac,
	char			*sHsmResultCode)
{

	unsigned char   BuffSnd[LG_MAX];
	unsigned char   szWork[LG_MAX];
	unsigned char   BuffRcv[BUF_LG_MAX];/*NAB18022019*/
	unsigned char   sVector[BUF_LG_MAX];
	char            mode;
	int             i, nLength, nLenWork = 0;
	int             retour, nOffset, nRemain, nIteration;
	pid_t           nPid;
	char            ligne[MAX_LINE_TRC];

	unsigned char   error_code[3];

	trace_event("Start verify_msg_auth_code()", PROCESSING);

	memset(szWork, 0, sizeof(szWork));
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sVector, 0, sizeof(sVector));
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
	{
		trace_event("--------------------------------------------", PROCESSING);
		sprintf(ligne, "takzak_key        : %s", takzak_key);
		trace_event(ligne, PROCESSING);

		sprintf(ligne, "mac_data          : %.16s", mac_data);
		trace_event(ligne, PROCESSING);
		trace_event("--------------------------------------------", PROCESSING);
	}

	/*if (CheckResource(nIndiceCtx, HSM_VERIF, &nPid) != OK) {*/
	if (CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid) != OK) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   verify_mac(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	} 


	
		mode='0';
		nLength = command_M8(	BuffSnd, 
			mode, 
			format,
			algorithm,
			padMethod,
			keyTpe,
			lg_message_mac, 
			takzak_key, 
			mac_data, 
			message_mac,
			sVector);

		retour = HsmQueryBin(nIndiceCtx, nPid, BuffSnd, nLength, BuffRcv);
		if (retour == NOT_AVAILABLE) {
			memcpy(sHsmResultCode, "UNVL", 4);
			trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
			trace_event("End   verify_msg_auth_code(HSM_UNAVAILABLE)", PROCESSING);
			return (HSM_UNAVAILABLE);
		}
		if (retour == HSM_MALFUNCTION) {
			memcpy(sHsmResultCode, "UNVL", 4);
			trace_event("HSM MALFUNCTION DETECTED", ERROR);
			trace_event("End   verify_msg_auth_code(MALFUNCTION)", PROCESSING);
			return (HSM_MALFUNCTION);
		}
		retour = command_M9(BuffRcv, sHsmResultCode,mode,sVector);

		if (retour != OK) {
			switch (retour) {
			case VERIFICATION_FAILED:
				trace_event("End   verify_msg_auth_code(VERIFICATION_FAILED)", PROCESSING);
				return (VERIFICATION_FAILED);

			case ERROR_COMMAND:
				trace_event("End verify_msg_auth_code(ERROR_COMMAND)", PROCESSING);
				return (HSM_MALFUNCTION);
			default:
				trace_event("End   verify_msg_auth_code(CRYPTOGRAPHIC_ERROR)",PROCESSING);
				return (CRYPTOGRAPHIC_ERROR);
			}

		}
	
			

	trace_event("End   verify_msg_auth_code(OK)", PROCESSING);
	return (OK);
}


/******************************************************************************/
int 
	gen_msg_auth_code(
	int nIndiceCtx,
	unsigned char	format, 
	unsigned char*	algorithm, 
	unsigned char	padMethod,
	unsigned char*	keyTpe,
	unsigned char	*takzak_key,
	unsigned char	*mac_data,
	unsigned char	*message_mac,
	int				lg_message_mac,
	char			*sHsmResultCode)
{

	unsigned char   BuffSnd[LG_MAX];
	unsigned char   BuffRcv[LG_MAX];
	unsigned char   sVector[LG_MAX];
	char            szWork[LG_MAX];
	char            mode;
	int             retour;
	int             nLength, nLenWork = 0;
	pid_t           nPid;
	int             i, nOffset, nIteration, nRemain = 0;
	char            ligne[MAX_LINE_TRC];

	/*START ADD KS 070307*/

	memset(ligne,0,sizeof(ligne));
	memset(BuffSnd,0,sizeof(BuffSnd));
	memset(BuffRcv,0,sizeof(BuffRcv));
	memset(sVector,0,sizeof(sVector));
	memset(szWork ,0,sizeof(szWork));


	nLenWork = 0;
	nRemain = 0;
	retour = -1;
	nLength = 0;
	nPid = 0;
	i = 0;
	nOffset = 0;
	nIteration =0;  


	/*END ADD KS 070307*/
	sprintf(ligne, "Start gen_msg_auth_code(%d)", lg_message_mac);
	trace_event(ligne, PROCESSING);
	/*MKB060309 PCI Norme Integration */
	if (debug_option == 1)
	{
		trace_event("---------------------------------------------", PROCESSING);
		sprintf(ligne, "takzak_key            : %.16s", takzak_key);
		trace_event(ligne, PROCESSING);
		trace_event("---------------------------------------------", PROCESSING);
	}
	memset(BuffSnd, 0, sizeof(BuffSnd));
	memset(BuffRcv, 0, sizeof(BuffRcv));
	memset(sVector, 0, sizeof(sVector));
	memset(szWork, 0, sizeof(szWork));

	/*if (CheckResource(nIndiceCtx, HSM_VERIF, &nPid) != OK) {*/
	if (CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid) != OK) {
		memcpy(sHsmResultCode, "UNVL", 4);
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   gen_msg_auth_code(HSM_UNAVAILABLE)", PROCESSING);
		return (HSM_UNAVAILABLE);
	}

	
		mode = '0';

		nLength = command_M6(	BuffSnd, 
			mode, 
			format,
			algorithm,
			padMethod,
			keyTpe,
			lg_message_mac, 
			takzak_key, 
			message_mac, 
			sVector);


		retour = HsmQueryBin(nIndiceCtx, nPid, BuffSnd, nLength, BuffRcv);

		if (retour == NOT_AVAILABLE) {
			memcpy(sHsmResultCode, "UNVL", 4);
			trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
			trace_event("End   gen_msg_auth_code(HSM_UNAVAILABLE)", PROCESSING);
			return (HSM_UNAVAILABLE);
		}
		if (retour == HSM_MALFUNCTION) {
			memcpy(sHsmResultCode, "UNVL", 4);
			trace_event("HSM MALFUNCTION DETECTED", ERROR);
			trace_event("End   gen_msg_auth_code(MALFUNCTION)", PROCESSING);
			return (HSM_MALFUNCTION);
		}

		retour = command_M7(BuffRcv, sHsmResultCode,mac_data,mode,sVector);
		if (retour != OK) {
			sprintf(ligne, "ERROR CODE             : %.s", sHsmResultCode);
			trace_event(ligne, ERROR);
			trace_event("End gen_msg_auth_code(ERROR)", PROCESSING);
			return (HSM_MALFUNCTION);
		}
	
		

	trace_event("End gen_msg_auth_code(OK)", PROCESSING);
	return (OK);
}


/* PROD00065699: modified impl*/
int 
	verify_mac(
	int nIndiceCtx,
	unsigned char *tak_key,
	unsigned char *mac_data,
	unsigned char *message_mac,
	int lg_message_mac,
	char *sHsmResultCode)
{

	return verify_msg_auth_code(	nIndiceCtx,
                                    HSM_FORMAT_BINARY,
                                    /*HSM_MAC_SIZE_8HEX_ALGO_ISO_9797_1*/ HSM_MAC_SIZE_8HEX_ALGO_ISO_9797_3,
                                    HSM_PAD_MTHD_ISO_9797_1,
                                    HSM_KEY_TAK,
                                    tak_key,
                                    mac_data,
                                    message_mac,
                                    lg_message_mac,
									sHsmResultCode ) ;
}


/******************************************************************************/
/* Fonction get_mac_data.                                                     */
/* -----------------------   -                                                */
/* Description : demande de generation du MAC			        			  */
/* Entree      : tak_key	-> la cle TAK				      				  */
/* mac_data	-> le MAC recu du GAB		  					  */
/* Sortie      : aucune							      						  */
/* Retour      : OK	-> pvv correct					      					  */
/* NOK	-> mac erronee			      						  */
/* Date/Auteur : 07/02/02 E.ENNOUINI                                          */
/* Modif  10/10/2003 I.Hassani; H.Youssfi                                     */
/******************************************************************************/
/* PROD00065699: modified impl*/
int 
	generate_mac(
	int				nIndiceCtx,
	unsigned char	*tak_key,
	unsigned char	*mac_data,
	unsigned char	*message_mac,
	int				lg_message_mac,
	char			*sHsmResultCode)
{

	return gen_msg_auth_code(     nIndiceCtx,
                                HSM_FORMAT_BINARY,
                                /*HSM_MAC_SIZE_8HEX_ALGO_ISO_9797_1*/ HSM_MAC_SIZE_8HEX_ALGO_ISO_9797_3,
                                HSM_PAD_MTHD_ISO_9797_1,
                                HSM_KEY_TAK,
                                tak_key,
                                mac_data,
                                message_mac,
                                lg_message_mac,
                                sHsmResultCode ) ;
}
