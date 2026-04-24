/********************************************************************************/
/*  (c) Hightech Payment Systems 2015                                          **/
/* Module                                                                      **/
/*      c2p_verification.c                                                     **/
/*                                                                             **/
/* Description                                                                 **/
/*      Verification tools pour C2P                                            **/
/********************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		12-11-2015   		Mouloud OUSSYMOUH    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <p7_common.h>
#include <define.h>
#include <iso8583.h>
#include <hsm_c2p_define.h>
#include <hsm_c2p.h>
#include <hsm_inc.h>
#include <queue.h>
#include <hsm_c2p_tag_def.h>
#include <stdio.h>
#include <resources.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <tlv_autho.h>
#include <string.h>

/******************************************************************************/
typedef struct s_response
{
   char *reponse;
   char *description;
} t_response;
/******************************************************************************/

t_response hsm_response[] = {
                { "0000" , "Traitement réussi."  },
                { "0001" , "Tag ‘XX’ non autorisé"  },
                { "0002" , "Tag ‘XX’ absent"  },
                { "0003" , "La longueur correspondant au tag ‘XX’ n’est pas autorisée."  },
                { "0004" , "La valeur correspondant au tag ‘XX’ n’est pas autorisée."  },
                { "0005" , "Le TLV correspondant au tag ‘XX’ est tronqué"  },
                { "1000" , "Mémoire insuffisante      "  },
                { "1001" , "DRBG Hors Service         "  },
                { "2000" , "Clé absente en C2P        "  },
                { "2001" , "Type de clé incorrect     "  },
                { "2002" , "Usage incorrect de la clé "  },
                { "2003" , "Erreur de propriétaire    "  },
                { "2004" , "Erreur de KCV             "  },
                { "2005" , "Propriétaire absent       "  },
                { "2006" , "Clé faible ou non impaire "  },
                { "2007" , "Valeur interdite          "  },
                { "2008" , "Clé présente en C2P"  },
                { "2009" , "Longueur  incorrecte                 "  },
                { "200A" , "Clé de stockage non définie          "  },
                { "200B" , "Erreur d’identifiant complémentaire  "  },
                { "200C" , "Erreur de numéro de clé              "  },
                { "200D" , "Clé inactive                         "  },
                { "200E" , "Portée non autorisée                 "  },
                { "200F" , "Classe non autorisée                 "  },
                { "2010" , "Accès interdit                       "  },
                { "2011" , "Niveau de sécurité insuffisant       "  },
                { "2012" , "Clé maître LMK absente               "  },
                { "2013" , "Clé non soumise à activation         "  },
                { "3000" , "AC calculé différent de l’AC reçu    "  },
                { "3001" , "DAC calculé différent de DAC reçu    "  },
                { "3002" , "IDN calculé différent de l’IDN reçu  "  },
                { "3003" , "Code faux                            "  },
                { "3004" , "Erreur de format du PIN Block        "  },
                { "3006" , "Sceau faux                           "  },
                { "3007" , "Authentifiant B0’ faux               "  },
                { "3008" , "" },
                { "3009" , "Erreur de CVx    "  },
                { "300A" , "Erreur sur la longueur du modulo                  "  },
                { "300B" , "Erreur sur la valeur de l’exposant public         "  },
                { "300C" , "Erreur sur le déchiffrement de la clé privée .    "  },
                { "300D" , "Erreur de cohérence d’un bi-clé                   "  },
                { "300E" , "Longueur incorrecte sur la donnée signée.         "  },
                { "300F" , "Signature incorrecte"  },
                { "3011" , "Erreur de déchiffrement DES                       "  },
                { "3012" , "Erreur de longueur de MAC                         "  },
                { "3013" , "Erreur d’ouverture de signature                   "  },
                { "3014" , "Erreur de récupération de chaîne intermédiaire    "  },
                { "3015" , "Erreur de récupération de la terminaison          "  },
                { "3016" , "Erreur de récupération du message                 "  },
                { "3017" , "Time out                                          "  },
                { "3018" , "Annulation                                        "  },
                { "3019" , "Erreur de liaison entre C2P et le lecteur externe "  },
                { "301A" , "Carte muette                                      "  },
                { "301B" , "Carte bloquée                                     "  },
                { "301C" , "Carte non supportée                               "  },
                { "301D" , "Erreur de présentation de PIN                     "  },
                { "301E" , "Erreur de sélection du fichier de l’application   "  },
                { "301F" , "Erreur de sélection du fichier élémentaire        "  },
                { "3020" , "Erreur de création de fichier                   "  },
                { "3021" , "Erreur de lecture de fichier                    "  },
                { "3022" , "Erreur d’écriture de fichier                    "  },
                { "3023" , "Erreur de modification du PIN                   "  },
                { "3024" , "Erreur de format du fichier                     "  },
                { "3025" , "Longueur de clé incohérente                     "  },
                { "3026" , "Numéros d’identification différents             "  },
                { "3027" , "Nombre de blocs différent du nombre attendu     "  },
                { "3028" , "Mode de décomposition différent du mode attendu "  },
                { "3029" , "Ordre d’introduction non respecté               "  },
                { "302A" , "Support non autorisé                            "  },
                { "302B" , "Numéro de C2P incorrect                         "  },
                { "302C" , "Erreur de contrôle de la clé d’authentification "  },
                { "302D" , "Clé d’authentification inconnue                 "  },
                { "302E" , "Erreur de contrôle de la clé d’authentification et code correct"  },
                { "302F" , "Erreur de contrôle de la clé d’authentification et code faux"  },
                { "3030" , "Erreur de format de donnée d’authentification"  },
                { "3031" , "Erreur de longueur du code                   "  },
                { "3032" , "Erreur de format de sortie                   "  },
                { "3033" , "Erreur de vérification du NA1 d’un ticket d’authentification"  },
                { "3034" , "Numéro de serveur erroné                          "  },
                { "3035" , "Priorité erronée                                  "  },
                { "3036" , "Compteur erroné                                   "  },
                { "3037" , "Méthode de décomposition non autorisée            "  },
                { "3038" , "Erreur de format des données de calcul de HMAC SPA"  },
                { "3039" , "Longueur du code différente de celle attendue     "  },
                { "303A" , "Format du PAN American Express erroné             "  },
                { "303B" , "Code de Sécurité Carte faux                       "  },
                { "303C" , "Erreur de longueur des données d’entrée du"  },
                { "303D" , "Erreur de déchiffrement des données à reporter    "  },
                { "303E" , "Erreur de récupération de la longueur du message  "  },
                { "303F" , "Type de clé de transport erroné                   "  },
                { "3040" , "Nombre de clés de transport non supporté          "  },
                { "3041" , "Type de clé MONEO erroné                          "  },
                { "3042" , "Format de l'identifiant de clé erroné             "  },
                { "3043" , "Clé de transport différente                       "  },
                { "3044" , "Format de table de décimalisation  erroné         "  },
                { "3045" , "Masque d'AC trop court                                 "  },
                { "3046" , "Type de KCV non supporté                               "  },
                { "3047" , "Méthode de padding non supportée                       "  },
                { "3048" , "Longueur des données incorrecte                        "  },
                { "3049" , "Format de codage erroné                                "  },
                { "3049" , "OTP Faux                                               "  },
                { "304A" , "Erreur de format de la piste ISO1                      "  },
                { "304B" , "Fichier existe déjà                                    "  },
                { "304C" , "Classe de clé incohérente                              "  },
                { "304D" , "Nombre de bits non autorisé                            "  },
                { "304E" , "Challenge non initialisé                               "  },
                { "304F" , "Longueur incorrecte de la clé carte                    "  },
                { "3050" , "Format de bloc de clé TR31 non supporté                "  },
                { "3051" , "Composante  SHAMIR dupliquée                           "  },
                { "3052" , ""  },
                { "3053" , "Erreur de Format de bloc de clé TR31                   "  },
                { "3054" , "CRC Faux                                               "  },
                { "3055" , "Erreur de format des informations de compte déchiffrées"  },
                { "3056" , "Erreur de cohérence des paramètres de domaine des courbes elliptiques"  },
                { "3057" , "Erreur d'authentification du porteur de secret "  },
                { "3058" , "Permission refusée au porteur                  "  },
                { "3059" , "Erreur de format du secret BANCOMAT            "  },
                { "305A" , "Certificat non valide                          "  }
};

/******************************************************************************/
/******************************************************************************/
int TextError (char *err_code, char *sMsgErr){
   int i;
   char ligne[MAX_LINE_TRC];

   for (i=0; i<(sizeof(hsm_response)/sizeof(hsm_response[0])); i++)
      if (!memcmp(hsm_response[i].reponse, err_code-2, 4) ){
         sprintf ( ligne , "=> %s <=", hsm_response [i].description );
         trace_event ( ligne , PROCESSING );
                 sprintf(sMsgErr,"%s",hsm_response [i].description );
                 return(OK);
      }
         sprintf ( sMsgErr , "=>UNKNOWN ERROR CODE %s <=", err_code-2);
                 return(OK);
}
/* PROD00030793 */

/******************************************************************************/


int     check_cvv_data  (
							int nIndexCtx,
							char *Pan, 
							char *CvkA, 
							char *CvkB, 
							char *expiry_date, 
							char *service_code, 
							char *cvv,
							char *sHsmResultCode
							)

{
	unsigned char		BuffSnd   [C2P_BUF_MAX_LEN];
	unsigned char		BuffRcv   [C2P_BUF_MAX_LEN];
	int					nResult;
	pid_t				nPid;
	char				sLine[MAX_LINE_TRC];
	char				sTrack2[64];
	char				sTrack2_raw[64];
	char				sData[256];
	char				sKeyRaw[32];
	TSC2PCmd			kC2PCmd;
	int					nLength;
	int					nLengthSnd;
	int					nLengthRcv;
	char				sReturnCode[32];
	char				sComplInfo[32];


	trace_event("Start check_cvv_data()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	
	sprintf(sLine, "PAN                         : %s", Pan);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "SERVICE CODE                : %.3s", service_code);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "EXPIRY DATE                 : %.4s", expiry_date);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "KCVX                        : %.32s", CvkA);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "CVV                         : %.3s", cvv);
	trace_event(sLine, PROCESSING);

	trace_event("- CVV Verification ---------------", PROCESSING);
	

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_CVX_FD);

	AddC2PCmdInfoField (C2P_FN_CVX_GEN_ALGO, &kC2PCmd, C2P_DF31_FMT_VISA, GetFieldIdLength(C2P_FN_CVX_GEN_ALGO,&kC2PCmd));

	sprintf(sTrack2, "0%.16sD%.4s%.3s10000%.3s00000",Pan, expiry_date, service_code, cvv);
	nLength = strlen(sTrack2);
	nLength = AsciiToHex(sTrack2,sTrack2_raw,nLength);
	AddC2PCmdInfoField (C2P_FN_ISO_TRACK_2, &kC2PCmd, sTrack2_raw, nLength);

	nLength = strlen(CvkA);
	nLength = AsciiToHex(CvkA,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	AddC2PCmdInfoField (C2P_FN_TRACK2_CTRL_VALUE_OFST, &kC2PCmd, C2P_DF09_TRACK2_CTRL_VALUE_OFST, GetFieldIdLength(C2P_FN_TRACK2_CTRL_VALUE_OFST,&kC2PCmd));

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_cvv_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_CVX_FF1D);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	/*GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);ICH05062016*/
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sHsmResultCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sHsmResultCode, "0000", 4) != 0 )
		{
	   	trace_event("End   check_cvv_data(VERIFICATION_FAILED)",PROCESSING); 
	  	return(VERIFICATION_FAILED);    /*NAK 160616   return(NOK);*/
	}

	trace_event("End   check_cvv_data(OK)",PROCESSING);
	return(OK);
}


/******************************************************************************/
/* Fonction check_aav_data.                                                  */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le AAV recu.                           */
/******************************************************************************/
int     check_aav_data  (
							int nIndexCtx,
							char *Pan,  
              int  nCvki,/*ICH20200310*/										
							char *aav_data_b64,
							char *sHsmResultCode
							)

{

  unsigned char	BuffSnd   [C2P_BUF_MAX_LEN];
  unsigned char	BuffRcv   [C2P_BUF_MAX_LEN];

  int           nResult;
  pid_t         nPid;
  char          sLine[MAX_LINE_TRC];
  char          sData[256];
  char          sKeyRaw[32];
  TSC2PCmd      kC2PCmd;
  char          sSCODE_raw[4];	
  int           nLength;
  int           nLengthSnd;
  int           nLengthRcv;
  char          sReturnCode[32];
  char          sComplInfo[32];
  char          CvkA[ 154];
  char          aav_key[500];
  char          expiry_date[4 + 1]; 
  char          service_code[3 + 1];
  char          aav_data[256];
  char          aav_value[4];
  char          tsn[16];
  char          auth_method[16];
  char          control_byte[16];
  char          key_id[16];
  char          str_temp[16];
  int           int_temp, i;
  int           retour;
  char          sAAV_raw[4];
	
	trace_event("Start check_aav_data()",PROCESSING);

  memset(BuffSnd      , 0 , sizeof( BuffSnd ) );
  memset(BuffRcv      , 0 , sizeof( BuffRcv ) );
  memset(sSCODE_raw   , 0 , sizeof( sSCODE_raw ) );	       
  memset(sReturnCode  , 0 , sizeof( sReturnCode ) );
  memset(sComplInfo   , 0 , sizeof( sComplInfo ) );
  memset(aav_data     , 0 , sizeof(aav_data));  
  memset(aav_key      , 0 , sizeof(aav_key));
  memset(aav_value    , 0 , sizeof(aav_value));
  memset(tsn          , 0 , sizeof(tsn));
  memset(auth_method  , 0 , sizeof(auth_method));
  memset(control_byte , 0 , sizeof(control_byte));
  memset(key_id       , 0 , sizeof(key_id));
  memset(service_code , 0 , sizeof(service_code));
  memset(CvkA         , 0 , sizeof(CvkA));
  memset(str_temp     , 0 , sizeof(str_temp));
	

  retour = Base64_Decode(nIndexCtx, aav_data_b64, aav_data);

/*CONTROL BYTE*/

  memcpy(str_temp, aav_data, 2);
  int_temp = strtol(str_temp, NULL, 16);
  sprintf(str_temp, "%d", int_temp);

  memcpy(control_byte, "00", 2);

	if (strlen(str_temp) > 2)
		memcpy(control_byte, str_temp + 1, 2);
	else
		memcpy(control_byte + (2 - strlen(str_temp)), str_temp, strlen(str_temp));

	/*AUTHENTICATION METHOD*/
	 memcpy(auth_method, aav_data + 20, 1);

	/*BIN Key Id*/
	memcpy(key_id, aav_data + 21, 1);

	/*TRANSACTION SEQUENCE NUMBER*/

	memcpy(str_temp, aav_data + 26, 4);  /*OK*/ 
	int_temp = strtol(str_temp, NULL, 16);

	sprintf(str_temp, "%d", int_temp);

	memcpy(tsn, "0000", 4);

	if (strlen(str_temp) > 4)
		memcpy(tsn, str_temp + 1, 4);
	else
		memcpy(tsn + (4 - strlen(str_temp)), str_temp, strlen(str_temp));

	/*UCAF/AAV*/
	  memcpy(aav_value, aav_data + 31, 3);

	/*AAV KEY */
	retour = Get_UCAF_Key(nIndexCtx, Pan, key_id, aav_key);
	if ((retour != OK))
	{
		memcpy(sHsmResultCode, "UNVL", 4);
		return(HSM_UNAVAILABLE);
	}



  memcpy(CvkA, aav_key, sizeof(CvkA));      	
  memcpy(service_code, auth_method, 1);
  memcpy(service_code + 1, control_byte, 2); 
  memcpy(expiry_date , tsn, 4);
  service_code[3] = 'F';
  aav_value[3]    = 'F'; 
 
 
  if (debug_option == 1)
  {	
    sprintf(sLine, "PAN                         : %s", Pan);
    trace_event(sLine, PROCESSING);
    sprintf(sLine, "SERVICE CODE                : %.3s", service_code);
    trace_event(sLine, PROCESSING);

    sprintf(sLine, "EXPIRY DATE                 : %.4s", expiry_date);
    trace_event(sLine, PROCESSING);

    sprintf(sLine, "KAAV                        : %s", CvkA);
    trace_event(sLine, PROCESSING);

    sprintf(sLine, "AAV                         : %.3s", aav_value);
    trace_event(sLine, PROCESSING);
  }
  else
  {
    trace_event("- AAV Verification ---------------", PROCESSING);
  }

	
  nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_CVX2_FE);

  AddC2PCmdInfoField (C2P_FN_CVX_GEN_ALGO, &kC2PCmd, C2P_DF31_FMT_VISA, GetFieldIdLength(C2P_FN_CVX_GEN_ALGO,&kC2PCmd));
  AddC2PCmdInfoField (C2P_FN_PAN, &kC2PCmd, Pan,  strlen(Pan)/2);
  AddC2PCmdInfoField (C2P_FN_EXPIRY_DATE, &kC2PCmd, expiry_date, GetFieldIdLength(C2P_FN_EXPIRY_DATE,&kC2PCmd));
	
  
  nLength = strlen(service_code);
  nLength = AsciiToHex(service_code,sSCODE_raw,nLength);
  AddC2PCmdInfoField (C2P_FN_SERVICE_CODE, &kC2PCmd, sSCODE_raw, GetFieldIdLength(C2P_FN_SERVICE_CODE,&kC2PCmd));


  nLength = strlen(CvkA);
  nLength = AsciiToHex(CvkA,sKeyRaw,nLength);
  AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
   
                        
  nLength = strlen(aav_value);                 
  nLength = AsciiToHex(aav_value,sAAV_raw,nLength);
  AddC2PCmdInfoField (C2P_FN_CVX2, &kC2PCmd, sAAV_raw, GetFieldIdLength(C2P_FN_CVX2,&kC2PCmd));
  
  DumpC2PCmdInfo(&kC2PCmd);
  nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);
  nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_aav_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_aav_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_aav_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
  nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_CVX_FF1D);

  AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
		 
  GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sHsmResultCode, &nLength);
  GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);
 
	if ( memcmp(sHsmResultCode, "0000", 4) != 0 )
		{  

	      trace_event("End   check_aav_data(VERIFICATION_FAILED)",PROCESSING); 
		    return(VERIFICATION_FAILED);
 	  }

  trace_event("End   check_aav_data(OK)",PROCESSING);
	return(OK);	 
 }


/******************************************************************************/
/* Fonction check_cavv_data.                                                  */
/* -----------------------                                                    */
/* Description : Fonction qui verifie le cavv recu.                           */
/******************************************************************************/
int     check_cavv_data  (
              int nIndexCtx,
              char *Pan,  
              char *cavv_data,
              char *sHsmResultCode
              )

{
  unsigned char  BuffSnd   [C2P_BUF_MAX_LEN];
  unsigned char  BuffRcv   [C2P_BUF_MAX_LEN];
  int       nResult;
  pid_t     nPid;
  char      sLine[MAX_LINE_TRC];
  char      sTrack2[64];
  char      sCAVV_raw[4];
  char      sSCODE_raw[4];
  char      sData[256];
  char      sKeyRaw[32];
  TSC2PCmd  kC2PCmd;
  int       nLength;
  int       nLengthSnd;
  int       nLengthRcv;
  char      sReturnCode[32];
  char      sComplInfo[32];
  char      cavv_value[4];
  char      atn[5];
  char      autho_result[2];
  char      second_factor[3];
  char      key_indicator[3];
  char      CvkA[154];
  char      CvkB[16 + 1];
  char      expiry_date[4 + 1]; 
  char      service_code[4];
  int       retour;
 
  
  trace_event("Start check_cavv_data()",PROCESSING);
        

  memset(BuffSnd       , 0 , sizeof( BuffSnd ) );
  memset(BuffRcv       , 0 , sizeof( BuffRcv ) );
  memset(sReturnCode   , 0 , sizeof( sReturnCode ) );
  memset(sComplInfo    , 0 , sizeof( sComplInfo ) );
  memset(sCAVV_raw     , 0 , sizeof(sCAVV_raw));
  memset(sSCODE_raw    , 0 , sizeof(sSCODE_raw));
  memset(cavv_value    , 0 , sizeof(cavv_value));
  memset(atn           , 0 , sizeof(atn));
  memset(autho_result  , 0 , sizeof(autho_result));
  memset(second_factor , 0 , sizeof(second_factor));
  memset(service_code  , 0 , sizeof(service_code));
  memset(CvkA          , 0 , sizeof(CvkA));
  memset(CvkB          , 0 , sizeof(CvkB));
  memset(key_indicator , 0 , sizeof(key_indicator));
  memset(expiry_date   , 0 , sizeof(expiry_date));
  memset(service_code  , 0 , sizeof(service_code));

 
  memcpy(autho_result, cavv_data + 1, 1);   
  memcpy(second_factor, cavv_data + 2, 2);
  memcpy(key_indicator, cavv_data + 4, 2);
  memcpy(cavv_value, cavv_data + 7, 3);
  memcpy(atn, cavv_data + 10, 4);


  retour = Get_CAVV_Key(nIndexCtx, Pan, key_indicator, CvkA, CvkB); 

  if ((retour != OK))
  {
    memcpy(sHsmResultCode, "UNVL", 4);
    return(HSM_UNAVAILABLE);
  }
  
  memcpy(service_code, autho_result, 1);
  memcpy(service_code + 1, second_factor, 2);
  memcpy(expiry_date , atn, 4);
  service_code[3] = 'F';
  cavv_value[3] = 'F';
      
  if (debug_option == 1)
  {  
    sprintf(sLine, "PAN                         : %s", Pan);
    trace_event(sLine, PROCESSING);

    sprintf(sLine, "SERVICE CODE                : %.3s", service_code);
    trace_event(sLine, PROCESSING);

    sprintf(sLine, "EXPIRY DATE                 : %.4s", expiry_date);
    trace_event(sLine, PROCESSING);

    sprintf(sLine, "KAAV                        : %s", CvkA);
    trace_event(sLine, PROCESSING);

    sprintf(sLine, "CAVV                         : %.3s", cavv_value);
    trace_event(sLine, PROCESSING);
  }
   else
  {
    trace_event("- CAVV Verification ---------------", PROCESSING);
  }

    
  nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_CVX2_FE);

  AddC2PCmdInfoField (C2P_FN_CVX_GEN_ALGO, &kC2PCmd, C2P_DF31_FMT_VISA, GetFieldIdLength(C2P_FN_CVX_GEN_ALGO,&kC2PCmd));

  AddC2PCmdInfoField (C2P_FN_PAN, &kC2PCmd, Pan,  strlen(Pan)/2);

  AddC2PCmdInfoField (C2P_FN_EXPIRY_DATE, &kC2PCmd, expiry_date, GetFieldIdLength(C2P_FN_EXPIRY_DATE,&kC2PCmd));


  
  nLength = strlen(service_code);
  nLength = AsciiToHex(service_code,sSCODE_raw,nLength);
  AddC2PCmdInfoField (C2P_FN_SERVICE_CODE, &kC2PCmd, sSCODE_raw, GetFieldIdLength(C2P_FN_SERVICE_CODE,&kC2PCmd));
  
  nLength = strlen(CvkA);
  nLength = AsciiToHex(CvkA,sKeyRaw,nLength);
  AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
   
                          
  nLength = strlen(cavv_value);                 
  nLength = AsciiToHex(cavv_value,sCAVV_raw,nLength);
  AddC2PCmdInfoField (C2P_FN_CVX2, &kC2PCmd, sCAVV_raw, GetFieldIdLength(C2P_FN_CVX2,&kC2PCmd));
  
  DumpC2PCmdInfo(&kC2PCmd);
  nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


  nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
  if ( ( nResult != OK ) )
  {
    memcpy ( sHsmResultCode , "UNVL", 4 );
    trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
    trace_event("End   check_cavv_data(HSM_UNAVAILABLE)",PROCESSING);
    return(HSM_UNAVAILABLE);
  }

  nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
  if ( nResult == NOT_AVAILABLE )
  {
    memcpy ( sHsmResultCode , "UNVL", 4 );
    trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
    trace_event("End   check_cavv_data(HSM_UNAVAILABLE)",PROCESSING);
    return(HSM_UNAVAILABLE);
  }

  if (nResult == HSM_MALFUNCTION)
  {
    memcpy ( sHsmResultCode , "UNVL", 4 );
    trace_event("HSM MALFUNCTION DETECTED",ERROR);
    trace_event("End   check_cavv_data(MALFUNCTION)",PROCESSING);
    return(HSM_MALFUNCTION);
  }

  
  nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_CVX_FF1D);

  AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
  
  GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sHsmResultCode, &nLength);
  GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);
 
  if ( memcmp(sHsmResultCode, "0000", 4) != 0 )
    {  
     sprintf(sLine, "HsmResultCode : %s ", sHsmResultCode);
     trace_event(sLine, PROCESSING);
     trace_event("End   check_cavv_data(VERIFICATION_FAILED)",PROCESSING); 
    return(VERIFICATION_FAILED);
   }

  trace_event("End   check_cavv_data(OK)",PROCESSING);
  return(OK);  
 }


int     check_cvv2_data  (
							int nIndexCtx,
							char *Pan, 
							char *CvkA, 
							char *CvkB, 
							char *expiry_date, 
							char *service_code, 
							char *cvv2,
							char *sHsmResultCode
							)

{
	unsigned char		BuffSnd   [C2P_BUF_MAX_LEN];
	unsigned char		BuffRcv   [C2P_BUF_MAX_LEN];
	int					nResult;
	pid_t				nPid;
	char				sLine[MAX_LINE_TRC];
	char				sTrack2[64];
	char				sTrack2_raw[64];
	char				sData[256];
	char				sKeyRaw[32];
	TSC2PCmd			kC2PCmd;
	int					nLength;
	int					nLengthSnd;
	int					nLengthRcv;
	char				sReturnCode[32];
	char				sComplInfo[32];


	trace_event("Start check_cvv2_data()",PROCESSING);


	memset(BuffSnd, 0, sizeof( BuffSnd ) );
	memset(BuffRcv, 0, sizeof( BuffRcv ) );
	
	memset(sReturnCode, 0, sizeof( sReturnCode ) );
	memset(sComplInfo, 0, sizeof( sComplInfo ) );
	
	sprintf(sLine, "PAN                         : %s", Pan);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "SERVICE CODE                : %.3s", service_code);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "EXPIRY DATE                 : %.4s", expiry_date);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "KCVX                        : %.32s", CvkA);
	trace_event(sLine, PROCESSING);

	sprintf(sLine, "CVV                         : %.3s", cvv2);
	trace_event(sLine, PROCESSING);

	trace_event("- CVV Verification ---------------", PROCESSING);
	

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_CVX2_FE);

	AddC2PCmdInfoField (C2P_FN_CVX_GEN_ALGO, &kC2PCmd, C2P_DF31_FMT_VISA, GetFieldIdLength(C2P_FN_CVX_GEN_ALGO,&kC2PCmd));

	AddC2PCmdInfoField (C2P_FN_PAN, &kC2PCmd, Pan, GetFieldIdLength(C2P_FN_PAN,&kC2PCmd));

    AddC2PCmdInfoField (C2P_FN_EXPIRY_DATE, &kC2PCmd, expiry_date, GetFieldIdLength(C2P_FN_EXPIRY_DATE,&kC2PCmd));
	
	AddC2PCmdInfoField (C2P_FN_SERVICE_CODE, &kC2PCmd, service_code, GetFieldIdLength(C2P_FN_SERVICE_CODE,&kC2PCmd));

	nLength = strlen(CvkA);
	nLength = AsciiToHex(CvkA,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	AddC2PCmdInfoField (C2P_FN_CVX2, &kC2PCmd, cvv2, GetFieldIdLength(C2P_FN_CVX2,&kC2PCmd));
	
	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndexCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv2_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndexCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv2_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_cvv2_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_CVX_FF1D);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	/*GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);ICH05062016*/
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sHsmResultCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);

	if ( memcmp(sHsmResultCode, "0000", 4) != 0 )
		{
	   	trace_event("End   check_cvv2_data(NOK)",PROCESSING); 
		return(NOK);
	}

	trace_event("End   check_cvv2_data(OK)",PROCESSING);
	return(OK);
}


int check_pvv_data ( int nIndiceCtx,
                     char *Pan, 
                     char *PvkA, 
                     char *PvkB,
                     char *PinBlock, 
                     char *format, 
                     char *trans_key, 
                     char *pvv, 
                     char *mode,
                     char *sHsmResultCode )

{
   int      retour;
   pid_t    nPid;
   unsigned char    BuffSnd  [LG_MAX];
   unsigned char    BuffRcv  [LG_MAX];
   int			    nResult;
   char				sLine[MAX_LINE_TRC];
   char				sTrack2[64];
   char				sTrack2_raw[64];
   char				sData[256];
   char				sKeyRaw[32];
   char				sPinRaw[32];
   TSC2PCmd			kC2PCmd;
   int				nLength;
   int				nLengthSnd;
   int				nLengthRcv;
   char				sReturnCode[32];
   char				sComplInfo[32];
	
   memset(BuffSnd, 0, sizeof( BuffSnd ) );
   memset(BuffRcv, 0, sizeof( BuffRcv ) );
   
   memset(sReturnCode, 0, sizeof( sReturnCode ) );
   memset(sComplInfo, 0, sizeof( sComplInfo ) );

   trace_event("Start check_pvv_data()",PROCESSING);


   sprintf(sLine, "PAN                         : %s", Pan );
   trace_event(sLine, PROCESSING);

   sprintf(sLine, "KPV                         : %.48s", PvkA);
   trace_event(sLine, PROCESSING);

   sprintf(sLine, "PVV                         : %.5s", pvv);
 	/*Start ICH14062016*/
  	if(strlen(pvv)>6)
  	{
  		 sprintf(sLine, "PVVQ                         : %.13s", pvv);
  	}
   trace_event(sLine, PROCESSING);

   sprintf(sLine, "PIN BLOCK                   : %.16s", PinBlock);
   trace_event(sLine, PROCESSING);

   sprintf(sLine, "Format                      : %.2s", format);
   trace_event(sLine, PROCESSING);

   sprintf(sLine, "KT                          : %.48s", trans_key );
   trace_event(sLine, PROCESSING);

   trace_event("- Start PVV Verification ---------------", PROCESSING);

   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_PIN_FC);

	sprintf(sTrack2, "0%.16sD9912621%.5s00000000", Pan, pvv);

   	/*Start ICH14062016*/
   	if(strlen(pvv)>6)
   	{
   	   trace_event("----API ----", PROCESSING);
   	/*	sprintf(sTrack2, "0%.16sD2512703%.6s0000000", Pan, pvv);*/
   		sprintf(sTrack2, "0%.16sD%.13s0000000", Pan, pvv);/*ED+SC+KPPPPQ*/

   	}


	/*End ICH14062016*/

	nLength = strlen(sTrack2);
	nLength = AsciiToHex(sTrack2,sTrack2_raw,nLength);
	AddC2PCmdInfoField (C2P_FN_ISO_TRACK_2, &kC2PCmd, sTrack2_raw, nLength);
	
	nLength = strlen(trans_key);
	nLength = AsciiToHex(trans_key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	nLength = strlen(PinBlock);
	nLength = AsciiToHex(PinBlock,sPinRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_ENC_PIN_BLK, &kC2PCmd, sPinRaw, GetFieldIdLength(C2P_FN_ENC_PIN_BLK,&kC2PCmd));
	
	sData[0] = '0';
	AddC2PCmdInfoField (C2P_FN_PIN_BLK_FMT, &kC2PCmd, sData, GetFieldIdLength(C2P_FN_PIN_BLK_FMT,&kC2PCmd));

	nLength = strlen(PvkA);
	nLength = AsciiToHex(PvkA,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);

	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);


	nResult = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndiceCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_cvv_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

   
   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_PIN_FF1C);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	/*GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);ICH05062016*/
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sHsmResultCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);
	if ( memcmp(sHsmResultCode, "0000", 4) != 0 )
      {

		if ( memcmp(sHsmResultCode, "3003", 4) == 0 )
		{
			trace_event("End   check_pvv_data(VERIFICATION_FAILED)", PROCESSING);
			return (VERIFICATION_FAILED);
		}
		else
		{
			trace_event("End   check_pvv_data(CRYPTOGRAPHIC_ERROR)", PROCESSING);
			return (CRYPTOGRAPHIC_ERROR);
		}

      }	
/*	if ( memcmp(sReturnCode, "0000", 4) != 0 )
      {
	   	trace_event("End   check_pvv_data(NOK)",PROCESSING); 
		return(NOK);
      }*/

   trace_event("End   check_pvv_data(OK)",PROCESSING);
   return(OK);
}
/******************************************************************************/
/* Fonction check_crpt_data.                                                  */
/******************************************************************************/

int     check_crpt_data  (
                            int        nIndiceCtx,
                            char      *sTlvBuffer,
                            char      *sHsmResultCode
                        )

{

   unsigned char  BuffSnd   [LG_MAX];
   unsigned char  BuffRcv   [LG_MAX];
   unsigned char  sPan          [22+ 1];   /* Preformated Pan/Sequence Nbr */
   unsigned char  sSeqNumber    [8 + 1];   /* Preformated Pan/Sequence Nbr */

   unsigned char  sFormatedPan  [16 + 1];  /* Preformated Pan/Sequence Nbr */
   unsigned char  sAtc  	    [4 + 1];   /* Transaction counter          */
   unsigned char  sUn   	    [8 + 1];   /* Unpredictible number */
   unsigned char  sData         [2048];
   unsigned char  sCrptAmount   [12 + 1];  /* Cryptogram Amount   */
   unsigned char  sOthrAmount   [12 + 1];  /* Other Amount ou Cash back amount */
   unsigned char  sCountryCode  [4 + 1];   /* Country Code                     */
   unsigned char  sTvr          [10 + 1];  /* Terminal Verification Results    */
   unsigned char  sCrncyCode    [4 + 1];   /* Currency Code                    */
   unsigned char  sTrxDate      [6 + 1];   /* Transaction Date                 */
   unsigned char  sTrxType      [2 + 1];   /* Transaction Type                */
   unsigned char  sAip          [4 + 1];   /* Application Interchange Profile */
   unsigned char  sCvr          [8 + 1];   /* Card Verification results        */
   unsigned char  sArqc         [16 + 1];  /* ARQC                             */
   unsigned char  sArpc         [16 + 1];  /* ARPC                             */
   unsigned char  sAC_Key       [256 + 1];
   unsigned char sAC_sKcv       [16 + 1];
   unsigned char  sArc          [10  ];   
   unsigned char  cSchemeId     [2  + 1];
   unsigned char  sProCode		[32  + 1];
   unsigned char  svData        [2048];
   unsigned char  sAC_Seq       [3 + 1];

   TSTlvPrivate   tlvInfo;
   TSTagIso       IccInfo;
   TSTlvAutho     sTlvInfo;
   TSIsoInfo      IsoInfo;
   TSC2PCmd		  kC2PCmd;
   char           szWork[256];
   char			  szTlvBuffer[2048];
   char           sIccData[3000];
   char           sTmpField[200];
   char           sTmpData[200];   
   char           sLine[MAX_LINE_TRC];
   char  		  tmpField  [  999];
   int 			  nLengthIso;
   int			  nLengthSnd;
   int			  nLengthRcv;
   char			  sReturnCode[32];
   char			  sComplInfo[32];
   char			  sKeyRaw[LG_MAX];
   int            nLength = 0;
   int	          nResult;
   int            nRetCode = 0;
   int            nLenWork = 0;
   int            nLenData =0;
   int            nTmpLen = 0;
   int            nIccLen = 0;
   int            retour = 0;
   int            nlength;
   char           sIad[64 + 1];
   int            nvLenData =0;
   char            vCvr[32 + 1];
   int             nCvrLen = 0;
   char            sIAD[512 +1]; /*BJH24042012: BCP CMI CPA */
   pid_t          nPid; 
   	char            v_icc_application_index[4 + 1];
	char            v_issuing_bank_code[6 + 1];
	char            v_product_code[3 + 1];

   trace_event("Start check_crpt_data()",PROCESSING);

   memset(BuffSnd, 0, sizeof( BuffSnd ) );
   memset(BuffRcv, 0, sizeof( BuffRcv ) );

   memset(  sFormatedPan , 0, sizeof(sFormatedPan));
   memset(  sAtc  			 , 0, sizeof(sAtc));
   memset(  sUn   			 , 0, sizeof(sUn));
   memset(  sData        , 0, sizeof(sData));
   memset(  svData        , 0, sizeof(svData));
   memset(  sCrptAmount  , 0, sizeof(sCrptAmount));
   memset(  sOthrAmount  , 0, sizeof(sOthrAmount));
   memset(  sCountryCode , 0, sizeof(sCountryCode));
   memset(  sTvr         , 0, sizeof(sTvr));
   memset(  sCrncyCode   , 0, sizeof(sCrncyCode));
   memset(  sTrxDate     , 0, sizeof(sTrxDate));
   memset(  sTrxType     , 0, sizeof(sTrxType));
   memset(  sAip         , 0, sizeof(sAip));
   memset(  sCvr         , 0, sizeof(sCvr));
   memset(  sArqc        , 0, sizeof(sArqc));
   memset(  sArpc        , 0, sizeof(sArpc));
   memset(  sAC_Key      , 0, sizeof(sAC_Key));
   memset(  sAC_Seq      , 0, sizeof(sAC_Seq));
   memset(  sAC_sKcv      , 0, sizeof(sAC_sKcv));
   memset(  szWork       , 0, sizeof(szWork));
   memset(  sTmpField    , 0, sizeof(sTmpField));
   memset(  tmpField    , 0, sizeof(tmpField));
   memset(vCvr, 0, sizeof(vCvr));
   memset(  sTmpData     , 0, sizeof(sTmpData));
   memset(  sArc         , 0, sizeof(sArc));
   memset(  sArpc        , 0, sizeof(sArpc));
   memset(  sPan         , 0, sizeof(sPan));
   memset(  sSeqNumber   , 0, sizeof(sSeqNumber));
   memset(  cSchemeId    , 0, sizeof(cSchemeId));
   memset(  sLine        , 0, sizeof(sLine));
   memset(  sProCode     , 0, sizeof(sProCode));
   memset(sIad, 0, sizeof(sIad));
   memset(sIAD, 0, sizeof(sIAD));
   memset(sReturnCode, 0, sizeof( sReturnCode ) );
   memset(sComplInfo, 0, sizeof( sComplInfo ) );
   memset(v_icc_application_index, 0, sizeof(v_icc_application_index));
  memset(v_issuing_bank_code, 0, sizeof(v_issuing_bank_code));
    memset(v_product_code, 0, sizeof(v_product_code));

/*
   memset(szTlvBuffer,0,sizeof(szTlvBuffer));
   memcpy(szTlvBuffer,sTlvBuffer,strlen(sTlvBuffer));*/
   InitTlvPrivate (&tlvInfo);
   
   /*nlength =  strlen(szTlvBuffer);*/

   if ( GetTlvBuffer ( AUTO_PAN , sTlvBuffer , sPan ,&nLenData) != SUCCESS )
   {
   	trace_event("IF GetTlvBuffer",PROCESSING);
    memcpy ( sHsmResultCode , "-PAN", 4 );
    trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
    trace_event("END   check_crpt_data(NOK)",PROCESSING);
    return(HSM_MALFUNCTION);
   }



	/** Retrieveing the Card Seq Number **/
/*    sprintf(sSeqNumber,"000");*/
	
	if (GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER, sTlvBuffer, sSeqNumber, &nLenData) != SUCCES)
        {
                memcpy(sHsmResultCode, "-PAN", 4);
                trace_event("ERROR WhiLe GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER) ", ERROR);
                trace_event("END   check_crpt_data(NOK)", PROCESSING);
                return (HSM_MALFUNCTION);
        }

   
   
   /*Getting Processing code*/
   
   if (GetTlvBuffer (AUTO_PROCESSING_CODE , sTlvBuffer, tmpField, &nLenData) != SUCCES)
   
   {
    trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PROCESSING_CODE) ", ERROR);
    trace_event("END   check_crpt_data(NOK)",PROCESSING);
    return(HSM_MALFUNCTION);
   }
   memcpy(sProCode, tmpField+6, 2);
   
   trace_event("==================CRYPTOGRAM Verification==================", PROCESSING);

   /* Getting the AC key **/

/*Start ICH20190220*/
	/** Getting the AUTO_EMV_CARD_INFO_DATA from TLV **/
	if (GetTlvBuffer(AUTO_EMV_CARD_INFO_DATA, sTlvBuffer, tmpField, &nLenData) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-AUTO_EMV_CARD_INFO_DATA", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
		trace_event("END   gen_iss_script_data(NOK)", PROCESSING);
		return(HSM_MALFUNCTION);
	}
	
	 memcpy(sAC_Seq, tmpField, 3);

	if (GetTlvBuffer(AUTO_ISSUING_BANK, sTlvBuffer, v_issuing_bank_code, &nLenData) != OK) {
		memcpy(sHsmResultCode, "-ICC", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_ISSUING_BANK) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

   /*nRetCode = Get_AC_Key(nIndiceCtx,sPan,sAC_Key);*/

	nRetCode = Get_Key(nIndiceCtx   , v_issuing_bank_code, "AC", sAC_Seq, sAC_Key, sAC_sKcv);
    if (nRetCode != OK)
    {
        memcpy(sHsmResultCode, "-AC_", 4);
        trace_event("END   gen_iss_script_data(KEY_UNAVAILABLE) <ERROR ON Get_EMV_Key(AC)>", PROCESSING);
        return(HSM_MALFUNCTION);
    }


/*End ICH20190220*/ 

	/* PreFormated Pan */

   memset(szWork, '0', 16);

   memcpy(sTmpField, sPan, strlen(sPan));

   memcpy(sTmpField + strlen(sPan), sSeqNumber+1, 2);

   nLenWork = strlen(sTmpField);

   if (nLenWork < 16 )
   memcpy(szWork + (16 - nLenWork), sTmpField , nLenWork);
   else
   memcpy(szWork, sTmpField + (nLenWork - 16), 16);


   memcpy (sFormatedPan,szWork, 16);
   
   
	/** Getting the ICC Data from TLV **/
   if ( GetTlvBuffer ( AUTO_ICC_DATA , sTlvBuffer , sIccData ,&nIccLen) != SUCCESS )
   {
      memcpy ( sHsmResultCode , "-PAN", 4 );
      trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
      trace_event("END   check_crpt_data(NOK)",PROCESSING);
      return(HSM_MALFUNCTION);
   }

   	 PrintTlvBuffer ( tlvInfo );


   	nResult=AnalyseTlvPrivate(sIccData , nIccLen,&tlvInfo);


    sprintf(sLine, "PAN                         : [%s]", sPan);
    trace_event(sLine, PROCESSING);

    sprintf(sLine, "SEQUENCE NUMBER             : [%s]", sSeqNumber);
    trace_event(sLine, PROCESSING);

   	/* cSchemeId ADD MOU060616*/
	memcpy(cSchemeId,"02",2);     /** Visa **/
	if (!GetTlvBuffer(AUTO_NETWORK_CODE, sTlvBuffer, szWork, &nLenWork) && memcmp(szWork, "01", 2))
		memcpy(cSchemeId,"01",2); /** MCI **/

    /*memcpy(cSchemeId,"02",2);*/
    sprintf(sLine, "cSchemeId                    : [%s]", cSchemeId);
   	trace_event(sLine, PROCESSING);


	/* ATC */
	memset(szWork, 0, sizeof(szWork));

	if (GetTlvPrivate (CHIP_ATC_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
    {
    memcpy ( sAtc,szWork, 4);
   	sprintf(sLine, "ATC                         : [%s]", sAtc);
   	trace_event(sLine, PROCESSING);
    }
	/* UN */

	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_UNPRED_NUMBER_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
	{   		
   	memcpy (sUn,szWork,  8);

	}
	else
   	memcpy (sUn,"00000000",  8);
   	sprintf(sLine, "UN                          : [%s]", sUn);
   	trace_event(sLine, PROCESSING);
	/* Data */
    nLenData = 0;
	/* Crpt Amount */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_AMOUNT_TAG , &tlvInfo, szWork, &nLenWork) != SUCCESS)
    sprintf(szWork,"%012d", 0);

    /*AsciiToHex (szWork, sCrptAmount, 12);*/
    memcpy (sCrptAmount,szWork,  12);

   	sprintf(sLine, "CRPT AMOUNT                 : [%s]", szWork);
   	trace_event(sLine, PROCESSING);
    memcpy(sData + nLenData, sCrptAmount, 12); nLenData += 12;


	/* Other Amount */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_OTHER_AMOUNT_TAG , &tlvInfo, szWork, &nLenWork) != SUCCESS)
    sprintf(szWork,"%012d", 0);

    /*AsciiToHex (szWork, sOthrAmount, 12);*/
	memcpy (sOthrAmount,szWork,  12);

    sprintf(sLine, "OTHER AMOUNT                : [%s]", szWork);
    trace_event(sLine, PROCESSING);
    memcpy(sData + nLenData, sOthrAmount, 12); nLenData += 12; 

	/* Country Code */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TERM_COUNTRY_CODE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   	/*AsciiToHex (szWork, sCountryCode, 4);*/
   	memcpy (sCountryCode,szWork,  4);
   	
    else
    {
       memcpy ( sHsmResultCode , "---1", 4 );
       trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TERM_COUNTRY_CODE_TAG>> NOT RECEIVED <==", ERROR);
       trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
       return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "COUNTRY CODE                 : [%s]", szWork);
   	trace_event(sLine, PROCESSING);
    memcpy(sData + nLenData, sCountryCode, 4); nLenData += 4;

    /* TVR          */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TVR_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
    memcpy (sTvr,szWork,  10);
    else
    {
    memcpy ( sHsmResultCode , "---2", 4 );
    trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TVR_TAG>> NOT RECEIVED <==", ERROR);
    trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
    return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "TVR                         : [%s]", szWork);
   	trace_event(sLine, PROCESSING);
    memcpy(sData + nLenData, sTvr, 10); nLenData += 10;

    /* CURRENCY CODE          */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_CUR_CODE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   	memcpy (sCrncyCode,szWork,  4);
    else
    {
       memcpy ( sHsmResultCode , "---3", 4 );
       trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_CUR_CODE_TAG>> NOT RECEIVED <==", ERROR);
       trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
       return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "CURRENCY CODE               : [%s]", szWork);
   	trace_event(sLine, PROCESSING);

    memcpy(sData + nLenData, sCrncyCode, 4); nLenData += 4;

    /* TRX DATE         */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_DATE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   	   memcpy (sTrxDate,szWork,  6);
    else
    {
       memcpy ( sHsmResultCode , "---4", 4 );
       trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_DATE_TAG>> NOT RECEIVED <==", ERROR);
       trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
       return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "TRANSACTION DATE            : [%s]", szWork);
   	trace_event(sLine, PROCESSING);

    memcpy(sData + nLenData, sTrxDate, 6); nLenData += 6; 

    /* TRX TYPE         */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_TYPE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   	memcpy (sTrxType,szWork,  2);
    else
    {
       memcpy ( sHsmResultCode , "---3", 4 );
       trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_TYPE_TAG>> NOT RECEIVED <==", ERROR);
       trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
       return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "TRANSACTION TYPE            : %s", szWork);
   	trace_event(sLine, PROCESSING);

    memcpy(sData + nLenData, sTrxType, 2); nLenData += 2;

	/* UN */
    memcpy(sData + nLenData, sUn, 8); 
    nLenData += 8; 

	/* AIP              */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_AIP_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   	memcpy (sAip,szWork,  4);
    else
    {
        memcpy ( sHsmResultCode , "---4", 4 );
        trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_AIP_TAG>> NOT RECEIVED <==", ERROR);
        trace_event("END   check_crpt_data(CRYPTOGRAPHIC_ERROR)",PROCESSING);
        return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "AIP                         : %s", szWork);
   	trace_event(sLine, PROCESSING);

    memcpy(sData + nLenData, sAip, 4); nLenData += 4; 

	/* ATC */
    memcpy(sData + nLenData, sAtc, 4); nLenData += 4;



    /*ICH2019 icc by prd code*/
	if (GetTlvBuffer(AUTO_PRODUCT_CODE, sTlvBuffer, v_product_code, &nLenWork) != OK)
	{
		memcpy(sHsmResultCode, "-ICC", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PRODUCT_CODE) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}
	if (GetTlvBuffer(ICC_APPLICATION_INDX, sTlvBuffer, v_icc_application_index, &nLenWork) != SUCCESS) {
		memcpy(sHsmResultCode, "-ICC", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(ICC_APPLICATION_INDX) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}
	memset(sIAD, 0, sizeof(sIAD));
	if (GetTlvPrivate(CHIP_ISS_APPLI_DATA_TAG, &tlvInfo, sIAD, &nLenWork) != SUCCESS)
		sprintf(sIAD, "%32d", 0);

    if (memcmp(cSchemeId,"01",2)==0)
	{
		/*AsciiToHex(sIAD, sIad, 64);*/
		memcpy(sData + nLenData, sIad, 32);
		nLenData += 32;	
	}	
	else
	{
	memset(vCvr, 0, sizeof(vCvr));
	nRetCode = GetCVRFromIAD(nIndiceCtx, sIAD, v_issuing_bank_code,v_product_code/*ICH2019*/, v_icc_application_index, vCvr);

	if (nRetCode != OK) {
		memcpy(sHsmResultCode, "-CVR", 4);
		trace_event("INVALID CVR ", ERROR);
		trace_event("END   check_crpt_data(KEY_UNAVAILABLE)", PROCESSING);
		return (HSM_MALFUNCTION);
	}
	
	sprintf(sLine, "CVR                         : [%s]", vCvr);
	trace_event(sLine, PROCESSING);


	nCvrLen = strlen(vCvr);

	/*AsciiToHex(vCvr, sCvr, nCvrLen);*/
	memcpy(sData + nLenData, vCvr, nCvrLen);
	nLenData += nCvrLen;

	} 

    /**** End ADD MOU06062015*****/
    /* ARQC */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_APP_CRYPTOGRAM_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   	memcpy (sArqc,szWork,  16);

   	sprintf(sLine, "ARQC                        : [%s]", sArqc);
   	trace_event(sLine, PROCESSING);
   	
   	sprintf(sArc, "3030");	
    sprintf(sLine, "Arc                          : [%s]",sArc);
    trace_event(sLine, PROCESSING);
   
    memset(BuffSnd ,0,sizeof(BuffSnd));
	
	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_AC_E2);
	
	AddC2PCmdInfoField (C2P_FN_AC_ALGO_ID, &kC2PCmd, cSchemeId, GetFieldIdLength(C2P_FN_AC_ALGO_ID,&kC2PCmd));
	
	nLength = strlen(sAC_Key);
	nLength = AsciiToHex(sAC_Key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	AddC2PCmdInfoField (C2P_FN_PAN, &kC2PCmd, sPan, strlen(sPan)/2);
	
	memset(szWork, 0, sizeof(szWork));
	memcpy(szWork,sSeqNumber+1,2);
	AddC2PCmdInfoField (C2P_FN_PAN_SEQ_NO, &kC2PCmd, szWork, GetFieldIdLength(C2P_FN_PAN_SEQ_NO,&kC2PCmd));
	
	if (memcmp(cSchemeId,"01",2)==0)
		{
		/*** Start ADD MOU060616***/
		memset(sKeyRaw ,0,sizeof(sKeyRaw));
		memcpy(svData + nvLenData, sAtc, 4);
	    nvLenData += 4;
		memcpy(svData + nvLenData, "0000", 4);
	    nvLenData += 4;
	    memcpy(svData + nvLenData, sUn, 8);
	    nvLenData += 8;

		nLength = strlen(svData);
		nLength = AsciiToHex(svData,sKeyRaw,nLength);
		AddC2PCmdInfoField (C2P_FN_SESSION_KEY_GEN_USED_NBR, &kC2PCmd, sKeyRaw, nLength);
		/*** End ADD MOU060616***/
		}
    nLength = strlen(sData);
	nLength = AsciiToHex(sData,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_AC_GEN_DATA, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(sArqc);
	nLength = AsciiToHex(sArqc,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_ARQC, &kC2PCmd, sKeyRaw, nLength);


    /*retour = BNT_command_E2 ( BuffSnd, cSchemeId, sAC_Key, sPan, sSeqNumber, sAtc, sUn, nLenData, sData, sArqc, sArc);*/
 	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);

	nResult = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndiceCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_cvv_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_VERIFY_AC_FF02);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	/*GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);ICH05062016*/
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sHsmResultCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);
	
	if ( memcmp(sHsmResultCode, "0000", 4) != 0 )
	{
	   	trace_event("End   check_crpt_data(NOK)",PROCESSING); 
		return(NOK);
	}
	
    nRetCode = calc_arpc (nIndiceCtx, sTlvBuffer, sHsmResultCode);
	if ( nRetCode != OK )
	{
		 trace_event("End   check_crpt_data(NOK) Error calcul ARPC",PROCESSING);
		 return nRetCode;
	}
    

    trace_event("End   check_crpt_data(OK)",PROCESSING);

    return(OK);
}
/******************************************************************************/
/* Fonction calc_arpc.                                                  */
/* -----------------------                                                    */
/******************************************************************************/

int     calc_arpc  (
                            int        nIndiceCtx,
                            char      *sTlvBuffer,
                            char      *sHsmResultCode
                        )

{

   unsigned char  BuffSnd   [LG_MAX];
   unsigned char  BuffRcv   [LG_MAX];
   unsigned char  sPan          [22+ 1];  /* Preformated Pan/Sequence Nbr */
   unsigned char  sSeqNumber    [8 + 1];  /* Preformated Pan/Sequence Nbr */
   unsigned char  sFormatedPan  [16 + 1]; /* Preformated Pan/Sequence Nbr */
   unsigned char  sAtc  	[4 + 1]; /* Transaction counter          */
   unsigned char  sUn   	[8 + 1]; /* Unpredictible number */
   unsigned char  sData         [2048];
   unsigned char  sCrptAmount   [12 + 1];  /* Cryptogram Amount   */
   unsigned char  sOthrAmount   [12 + 1];  /* Other Amount ou Cash back amount */
   unsigned char  sCountryCode  [4 + 1];  /* Country Code                     */
   unsigned char  sTvr          [10 + 1];  /* Terminal Verification Results    */
   unsigned char  sCrncyCode    [4 + 1];  /* Currency Code                    */
   unsigned char  sTrxDate      [6 + 1];  /* Transaction Date                 */
   unsigned char  sTrxType      [2 + 1];   /* Transaction Type                */
   unsigned char  sAip          [4 + 1];   /* Application Interchange Profile */
   unsigned char  sCvr          [8 + 1];  /* Card Verification results        */
   unsigned char  sArqc         [16 + 1];   /* ARQC                             */
   unsigned char  sArpc         [16 + 1];   /* ARPC                             */
   unsigned char  sAC_Key       [256 + 1];
   unsigned char  sArc          [10  ];   
   unsigned char  cSchemeId     [2  + 1];
   unsigned char  sProCode			[32  + 1];
   unsigned char  svData         [2048];
   unsigned char  sAC_sKcv       [16 + 1];
   unsigned char  sAC_Seq       [3 + 1];
   char            v_issuing_bank_code[6 + 1];

   TSC2PCmd		   kC2PCmd;
   TSTlvPrivate tlvInfo;
   TSTagIso       IccInfo;
   TSTlvAutho      sTlvInfo;
   TSIsoInfo       IsoInfo;
   char           szWork[256];
   char						szTlvBuffer[2048];
   char           sIccData[3000];
   char           sTmpField[200];
   char           sTmpData[200];   
   char           sLine[MAX_LINE_TRC];
   char  		  tmpField  [  999];
   char			  sReturnCode[32];
   char			  sComplInfo[32];
   char			  sKeyRaw[LG_MAX];
   int 			  nLengthIso;
   int      nRetCode = 0;
   int      nLenWork = 0;
   int      nLenData =0;
   int      nLength = 0;
   int      nTmpLen = 0;
   int      nIccLen = 0;
   int      retour = 0;
   int            nvLenData =0;
   int			   nLengthSnd;
   int			   nLengthRcv;
   int      nlength;
   int		nResult;
   pid_t    nPid; 

   
   trace_event("Start calc_arpc()",PROCESSING);

   memset(BuffSnd, 0, sizeof( BuffSnd ) );
   memset(BuffRcv, 0, sizeof( BuffRcv ) );

   memset(  sFormatedPan , 0, sizeof(sFormatedPan));
   memset(  sAtc  			 , 0, sizeof(sAtc));
   memset(  sUn   			 , 0, sizeof(sUn));
   memset(  sData        , 0, sizeof(sData));
   memset(  sCrptAmount  , 0, sizeof(sCrptAmount));
   memset(  sOthrAmount  , 0, sizeof(sOthrAmount));
   memset(  sCountryCode , 0, sizeof(sCountryCode));
   memset(  sTvr         , 0, sizeof(sTvr));
   memset(  sCrncyCode   , 0, sizeof(sCrncyCode));
   memset(  sTrxDate     , 0, sizeof(sTrxDate));
   memset(  sTrxType     , 0, sizeof(sTrxType));
   memset(  sAip         , 0, sizeof(sAip));
   memset(  sCvr         , 0, sizeof(sCvr));
   memset(  sArqc        , 0, sizeof(sArqc));
   memset(  sArpc        , 0, sizeof(sArpc));
   memset(  sAC_Key      , 0, sizeof(sAC_Key));
   memset(  szWork       , 0, sizeof(szWork));
   memset(  sTmpField    , 0, sizeof(sTmpField));
   memset(  tmpField    , 0, sizeof(tmpField));
   
   memset(  sTmpData     , 0, sizeof(sTmpData));
   memset(  sArc         , 0, sizeof(sArc));
   memset(  sArpc        , 0, sizeof(sArpc));
   memset(  sPan         , 0, sizeof(sPan));
   memset(  sSeqNumber   , 0, sizeof(sSeqNumber));
   memset(  cSchemeId    , 0, sizeof(cSchemeId));
   memset(  sLine        , 0, sizeof(sLine));
   memset(  sProCode     , 0, sizeof(sProCode));
   memset(sReturnCode, 0, sizeof( sReturnCode ) );
   memset(sComplInfo, 0, sizeof( sComplInfo ) );
   memset(  svData        , 0, sizeof(svData));

   memset(  sAC_sKcv        , 0, sizeof(sAC_sKcv));
   memset(  sAC_Seq        , 0, sizeof(sAC_Seq));
  memset(v_issuing_bank_code, 0, sizeof(v_issuing_bank_code));

	memset(szTlvBuffer,0,sizeof(szTlvBuffer));
	memcpy(szTlvBuffer,sTlvBuffer,strlen(sTlvBuffer));
	
	nlength =  strlen(szTlvBuffer);

   if ( GetTlvBuffer ( AUTO_PAN , szTlvBuffer , sPan ,&nLenData) != SUCCESS )
   {
   		trace_event("IF GetTlvBuffer",PROCESSING);
      memcpy ( sHsmResultCode , "-PAN", 4 );
      trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
      trace_event("END   calc_arpc(NOK)",PROCESSING);
      return(HSM_MALFUNCTION);
   }

   
  /*    sprintf(sSeqNumber,"000");*/
	
	if (GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER, sTlvBuffer, sSeqNumber, &nLenData) != SUCCES)
        {
                memcpy(sHsmResultCode, "-PAN", 4);
                trace_event("ERROR WhiLe GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER) ", ERROR);
                trace_event("END   check_crpt_data(NOK)", PROCESSING);
                return (HSM_MALFUNCTION);
        }
   
   /*Getting Processing code*/
   
   if (GetTlvBuffer (AUTO_PROCESSING_CODE , szTlvBuffer, tmpField, &nLenData) != SUCCES)
   
   {
      trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PROCESSING_CODE) ", ERROR);
      trace_event("END   calc_arpc(NOK)",PROCESSING);
      return(HSM_MALFUNCTION);
   }
      memcpy(sProCode, tmpField+6, 2);
   
   
   
      /* Getting the AC key **/

/*Start ICH20190220*/
	/** Getting the AUTO_EMV_CARD_INFO_DATA from TLV **/
	if (GetTlvBuffer(AUTO_EMV_CARD_INFO_DATA, sTlvBuffer, tmpField, &nLenData) != SUCCESS)
	{
		memcpy(sHsmResultCode, "-AUTO_EMV_CARD_INFO_DATA", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_PAN) ", ERROR);
		trace_event("END   gen_iss_script_data(NOK)", PROCESSING);
		return(HSM_MALFUNCTION);
	}
	
	 memcpy(sAC_Seq, tmpField, 3);

	if (GetTlvBuffer(AUTO_ISSUING_BANK, sTlvBuffer, v_issuing_bank_code, &nLenData) != OK) {
		memcpy(sHsmResultCode, "-ICC", 4);
		trace_event("ERROR WhiLe GetTlvBuffer(AUTO_ISSUING_BANK) ", ERROR);
		trace_event("END   check_crpt_data(NOK)", PROCESSING);
		return (HSM_MALFUNCTION);
	}

   /*nRetCode = Get_AC_Key(nIndiceCtx,sPan,sAC_Key);*/

	nRetCode = Get_Key(nIndiceCtx   , v_issuing_bank_code, "AC", sAC_Seq, sAC_Key, sAC_sKcv);
    if (nRetCode != OK)
    {
        memcpy(sHsmResultCode, "-AC_", 4);
        trace_event("END   gen_iss_script_data(KEY_UNAVAILABLE) <ERROR ON Get_EMV_Key(AC)>", PROCESSING);
        return(HSM_MALFUNCTION);
    }

/*End ICH20190220*/ 
      

   /* PreFormated Pan */

    memset(szWork, '0', 16);

    memcpy(sTmpField, sPan, strlen(sPan));

    memcpy(sTmpField + strlen(sPan), sSeqNumber+1, 2);

    nLenWork = strlen(sTmpField);

    if (nLenWork < 16 )
   	memcpy(szWork + (16 - nLenWork), sTmpField , nLenWork);
    else
   	memcpy(szWork, sTmpField + (nLenWork - 16), 16);


    memcpy (sFormatedPan,szWork, 16);
    sprintf(sLine, "PRE FORMATED PAN            : %s", szWork);
   
    /** Getting the ICC Data from TLV **/
    if ( GetTlvBuffer ( AUTO_ICC_DATA , szTlvBuffer , sIccData ,&nIccLen) != SUCCESS )
    {
       memcpy ( sHsmResultCode , "-PAN", 4 );
       trace_event("ERROR WhiLe GetTlvBuffer(AUTO_ICC_DATA) ", ERROR);
       trace_event("END   calc_arpc(NOK)",PROCESSING);
       return(HSM_MALFUNCTION);
    }

	InitTlvPrivate (&tlvInfo);
	AnalyseTlvPrivate(sIccData , nIccLen,&tlvInfo);

    sprintf(sLine, "PAN                         : %s", sPan);
    trace_event(sLine, PROCESSING);

    sprintf(sLine, "SEQUENCE NUMBER             : %s", sSeqNumber);
    trace_event(sLine, PROCESSING);

  	/* cSchemeId ADD MOU060616*/
	memcpy(cSchemeId,"02",2);     /** Visa **/
	if (!GetTlvBuffer(AUTO_NETWORK_CODE, sTlvBuffer, szWork, &nLenWork) && memcmp(szWork, "01", 2))
		memcpy(cSchemeId,"01",2); /** MCI **/
    /* cSchemeId */

   /* memcpy(cSchemeId,"02",2);*/
    sprintf(sLine, " cSchemeId                    : %s", cSchemeId);
   	trace_event(sLine, PROCESSING);

    /* ATC */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_ATC_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
    {
    memcpy ( sAtc,szWork, 4);
   	sprintf(sLine, "ATC                         : %s", sAtc);
   	trace_event(sLine, PROCESSING);
    }
	
    /* UN */

	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_UNPRED_NUMBER_TAG, &tlvInfo, szWork, &nLenWork) == SUCCESS)
    {   		
    memcpy (sUn,szWork,  8);
    }
    else
   	memcpy (sUn,"00000000",  8);
	
   	sprintf(sLine, "UN                          : %s", sUn);
   	trace_event(sLine, PROCESSING);
	
    /* Data */
    nLenData = 0;
	
    /* Crpt Amount */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_AMOUNT_TAG , &tlvInfo, szWork, &nLenWork) != SUCCESS)
    sprintf(szWork,"%012d", 0);

    memcpy (sCrptAmount,szWork,  12);

   	sprintf(sLine, "CRPT AMOUNT                 : %s", szWork);
   	trace_event(sLine, PROCESSING);
	
    memcpy(sData + nLenData, sCrptAmount, 12); nLenData += 12;

	/* Other Amount */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_OTHER_AMOUNT_TAG , &tlvInfo, szWork, &nLenWork) != SUCCESS)
    sprintf(szWork,"%012d", 0);

    /*AsciiToHex (szWork, sOthrAmount, 12);*/
    memcpy (sOthrAmount,szWork,  12);
    sprintf(sLine, "OTHER AMOUNT                 : %s", szWork);
    trace_event(sLine, PROCESSING);
    memcpy(sData + nLenData, sOthrAmount, 12); nLenData += 12; 

    /* Country Code */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TERM_COUNTRY_CODE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   	memcpy (sCountryCode,szWork,  4);
    else
    {
    memcpy ( sHsmResultCode , "---1", 4 );
    trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TERM_COUNTRY_CODE_TAG>> NOT RECEIVED <==", ERROR);
    trace_event("END   calc_arpc(CRYPTOGRAPHIC_ERROR)",PROCESSING);
    return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "COUNTRY CODE                 : %s", szWork);
   	trace_event(sLine, PROCESSING);
    memcpy(sData + nLenData, sCountryCode, 4); nLenData += 4;

    /* TVR          */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TVR_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   	memcpy (sTvr,szWork,  10);
    else
    {
    memcpy ( sHsmResultCode , "---2", 4 );
    trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TVR_TAG>> NOT RECEIVED <==", ERROR);
    trace_event("END   calc_arpc(CRYPTOGRAPHIC_ERROR)",PROCESSING);
    return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "TVR                          : %s", szWork);
   	trace_event(sLine, PROCESSING);
    memcpy(sData + nLenData, sTvr, 10); nLenData += 10;

	/* CURRENCY CODE          */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_CUR_CODE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   		memcpy (sCrncyCode,szWork,  4);
    else
    {
        memcpy ( sHsmResultCode , "---3", 4 );
        trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_CUR_CODE_TAG>> NOT RECEIVED <==", ERROR);
        trace_event("END   calc_arpc(CRYPTOGRAPHIC_ERROR)",PROCESSING);
        return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "CURRENCY CODE                : %s", szWork);
   	trace_event(sLine, PROCESSING);

    memcpy(sData + nLenData, sCrncyCode, 4); nLenData += 4;

    /* TRX DATE         */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_DATE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   	   memcpy (sTrxDate,szWork,  6);
    else
    {
       memcpy ( sHsmResultCode , "---4", 4 );
       trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_DATE_TAG>> NOT RECEIVED <==", ERROR);
       trace_event("END   calc_arpc(CRYPTOGRAPHIC_ERROR)",PROCESSING);
       return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "TRANSACTION DATE             : %s", szWork);
   	trace_event(sLine, PROCESSING);

    memcpy(sData + nLenData, sTrxDate, 6); nLenData += 6; 

    /* TRX TYPE         */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_TRANS_TYPE_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
    memcpy (sTrxType,szWork,  2);  		
    else
    {
    memcpy ( sHsmResultCode , "---3", 4 );
    trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_TRANS_TYPE_TAG>> NOT RECEIVED <==", ERROR);
    trace_event("END   calc_arpc(CRYPTOGRAPHIC_ERROR)",PROCESSING);
    return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "TRANSACTION TYPE             : %s", szWork);
   	trace_event(sLine, PROCESSING);

    memcpy(sData + nLenData, sTrxType, 2); nLenData += 2;

    /* UN */
    memcpy(sData + nLenData, sUn, 8); 
    nLenData += 8; 

    /* AIP              */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_AIP_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
   	   memcpy (sAip,szWork,  4);
    else
    {
       memcpy ( sHsmResultCode , "---4", 4 );
       trace_event("==> CANNOT CHECK CRYPTOGRAM.  <<CHIP_AIP_TAG>> NOT RECEIVED <==", ERROR);
       trace_event("END   calc_arpc(CRYPTOGRAPHIC_ERROR)",PROCESSING);
       return(CRYPTOGRAPHIC_ERROR);
    }

   	sprintf(sLine, "AIP                          : %s", szWork);
   	trace_event(sLine, PROCESSING);

    memcpy(sData + nLenData, sAip, 4); nLenData += 4; 

    /* ATC */
    memcpy(sData + nLenData, sAtc, 4); nLenData += 4;

    /* CVR              */
	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_ISS_APPLI_DATA_TAG , &tlvInfo, szWork, &nLenWork) != SUCCESS)
    sprintf(szWork, "%32d", 0);

   	sprintf(sLine, "CVR                          : %.8s", szWork+6);
    memcpy (sCvr,szWork+6,  8);
    memcpy(sData + nLenData, sCvr, 8); nLenData += 8;

   	sprintf(sLine, "CVR                          : %s", sCvr);
   	trace_event(sLine, PROCESSING);
	
    /* ARQC */

	memset(szWork, 0, sizeof(szWork));
	if (GetTlvPrivate (CHIP_APP_CRYPTOGRAM_TAG , &tlvInfo, szWork, &nLenWork) == SUCCESS)
    memcpy (sArqc,szWork,  16);

   	sprintf(sLine, "ARQC                         : %s", sArqc);
   	trace_event(sLine, PROCESSING);
  	memset(szWork, 0, sizeof(szWork)); 	

    memcpy(sArc,"3030",4); /* JGH for test */
   
    sprintf(sLine, "ARC..........................: %s",sArc);
    trace_event(sLine, PROCESSING);
     
    memset(BuffSnd ,0,sizeof(BuffSnd));

	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_ARPC_E3);
	
	AddC2PCmdInfoField (C2P_FN_ARPC_GEN_ALGOID, &kC2PCmd, cSchemeId, GetFieldIdLength(C2P_FN_ARPC_GEN_ALGOID,&kC2PCmd));
	
	nLength = strlen(sAC_Key);
	nLength = AsciiToHex(sAC_Key,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_SYM_KEY_TKN_BLOC, &kC2PCmd, sKeyRaw, nLength);
	
	AddC2PCmdInfoField (C2P_FN_PAN, &kC2PCmd, sPan, strlen(sPan)/2);
	
	memset(szWork, 0, sizeof(szWork));
	memcpy(szWork,sSeqNumber+1,2);
	AddC2PCmdInfoField (C2P_FN_PAN_SEQ_NO, &kC2PCmd, szWork, GetFieldIdLength(C2P_FN_PAN_SEQ_NO,&kC2PCmd));
	


	nLength = strlen(sArqc);
	nLength = AsciiToHex(sArqc,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_ARQC, &kC2PCmd, sKeyRaw, nLength);
	
	nLength = strlen(sArc);
	nLength = AsciiToHex(sArc,sKeyRaw,nLength);
	AddC2PCmdInfoField (C2P_FN_ARC, &kC2PCmd, sKeyRaw, nLength);

 	DumpC2PCmdInfo(&kC2PCmd);
	nLengthSnd = BuildC2PCmdInfoBuffer (BuffSnd, &kC2PCmd);

	nResult = CheckHsmResource(nIndiceCtx, HSM_VERIF_MODE, &nPid);
	if ( ( nResult != OK ) )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	nResult = HsmRequest( nIndiceCtx, nPid ,nBalHsmVerf, BuffSnd, nLengthSnd, BuffRcv,&nLengthRcv );
	if ( nResult == NOT_AVAILABLE )
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM FOR VERIFICATION NOT AVAILABLE", ERROR);
		trace_event("End   check_cvv_data(HSM_UNAVAILABLE)",PROCESSING);
		return(HSM_UNAVAILABLE);
	}

	if (nResult == HSM_MALFUNCTION)
	{
		memcpy ( sHsmResultCode , "UNVL", 4 );
		trace_event("HSM MALFUNCTION DETECTED",ERROR);
		trace_event("End   check_cvv_data(MALFUNCTION)",PROCESSING);
		return(HSM_MALFUNCTION);
	}

   	nResult = InitC2PCmd(&kC2PCmd, C2P_MN_GEN_ARPC_FF03);

	AnalyseC2PCmdInfo(BuffRcv, &kC2PCmd);
	
	/*GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sReturnCode, &nLength);ICH05062016*/
	GetC2PCmdInfoField (C2P_FN_RETURN_CODE, &kC2PCmd, sHsmResultCode, &nLength);
	GetC2PCmdInfoField (C2P_FN_COMPL_INFO, &kC2PCmd, sComplInfo, &nLength);
	
    if ( memcmp(sHsmResultCode, "0000", 4) == 0 )
       {
	    memset(szWork, 0, sizeof(szWork));
		 
		GetC2PCmdInfoField (C2P_FN_ARPC, &kC2PCmd, szWork, &nLength);
		HexToAscii( szWork, sArpc, 2*nLength);
       }
    else
       {
	   	trace_event("End   calc_arpc(NOK)",PROCESSING); 
		return(NOK);
       }
	
    /* ARPC and ARC */
    memset(szWork, 0, sizeof(szWork));	
    memset(sTmpField, 0, sizeof(sTmpField));	
   	memcpy (szWork, sArpc, 16);
    memcpy(szWork + 16, sArc, 4); 

    sprintf(sLine, "==> Issuer Authen Data       : [%s]", szWork);
   	trace_event(sLine, PROCESSING);

	PutTlvPrivate (CHIP_ISS_AUTHEN_DATA_TAG, &tlvInfo, szWork, 20);

	nIccLen = PrivateBuildTlv (sIccData, &tlvInfo);

    PutTlvBuffer ( AUTO_ICC_DATA, sTlvBuffer, sIccData, nIccLen);

    trace_event("End   calc_arpc(OK)",PROCESSING);

    return(OK);
}


int     gen_iss_script_data(
	int        nIndexCtx,
	char      *sTlvBuffer,
	char      *sHsmResultCode
	)

{
	return(OK);

}

