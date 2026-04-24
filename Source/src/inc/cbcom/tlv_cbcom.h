#ifndef TLV_CBCOM_H
#define TLV_CBCOM_H


/**************************************/
																				 
#define   CBCOM_TLV_FIX           0
#define   CBCOM_TLV_VAR           1
/**************************************/

#define	 MAX_CBCOM_CHANNELS		2
#define  MAX_CBCOM_TLV			256
#define  MAX_CBCOM_TLV_LEN		4098
#define  REJ_IPDU_MAX_SIZE		40

#define T_IPDU_DE				0xC1		/* Transfert de données					*/
#define T_IPDU_CN				0xC2		/* Demande de connexion					*/
#define T_IPDU_AC				0xC3		/* Acceptation de connexion				*/
#define T_IPDU_AB				0xC4		/* Coupure de connexion					*/
#define T_IPDU_TD				0xC5		/* Test de disponibilité de connexion	*/
#define T_IPDU_RJ				0xC6		/* Rejet d’IPDU							*/
#define T_IPDU_TN				0xC7		/* Terminaison normale					*/
#define T_IPDU_RD				0xC8		/* Réponse Test de disponibilité		*/



#define PI01_RESPONSE_CODE			0x01	/* Code retour pseudo-session												*/
#define PI04_CARD_ACCEPTOR_SESSION_ID	0x04 /* Identifiant pseudo-session de l'accepteur de carte						*/
#define PI05_PROTOCOL_VERSION		0x05	/* Numéro de version du protocole CBcom										*/
#define PI06_MAX_IPDU_LEN			0x06	/* Valeur en octet de la longueur maximale d’IPDU							*/
#define PI07_APDU_LEN				0x07	/* Valeur en octet de la longueur du champ de données transportées (APDU)	*/
#define PI08_DATA_CODING_FORMAT		0x08	/* Format de codage des données transportées								*/
#define PI10_COMMUNITY_ID			0x0A	/* Identifiant de la communauté												*/
#define PI15_TOTAL_DATA_LEN			0x0F	/* Longueur totale des données à transférer									*/
#define PI16_TNR					0x10	/* Valeur du timer de non réponse (TNR)										*/
#define PI17_TSI					0x11	/* Valeur du timer de surveillance d’inactivité (TSI)						*/
#define PI18_TMA					0x12	/* Valeur du timer de maintient d’activité (TMA)							*/
#define PI19_APP_PROTOCOL_ID		0x13	/* Identifiant du protocole applicatif										*/
#define PI20_CBCOM_PROTOCOL_VERSION	0x14	/* Version du protocole CBcom												*/
#define PI21_APP_PROTOCOL_VERSION	0x15	/* Version du protocole applicatif											*/
#define PI22_SERVICE_CLASS			0x16	/* Classe de services														*/
#define PI23_LINK_CHANNEL_ID		0x17	/* Identifiant du canal de raccordement										*/
#define PI24_DATA_IPDU_ID			0x18	/* Identifiant d'IPDU de données											*/
#define PI25_UA						0x19	/* Identifiant de l'application utilisatrice								*/
#define PI26_CONNECTED_CBCOM_ENTITY	0x1A	/* Identifiant de l'entité CBcom connectée									*/
#define PI27_TRANSFER_MODE			0x1B	/* Mode de transfert														*/
#define PI29_FLOW_TYPE				0x1D	/* Type de flux																*/
#define PI30_WINDOW_SIZE			0x1E	/* Taille fenêtre d'anticipation											*/


/*¨PI 01 values */
#define PI01_RC_OK							0x00	/* Pas d'anomalie. Elle concrétise la bonne interprétation de l'IPDU */
#define PI01_RC_UNKNOWN_IPDU				0x02	/* IPDU inconnue. Le contenu du champ PGI n’est pas reconnu */
#define PI01_RC_INVALID_LGI					0x05	/* Longueur de paramètre invalide */
#define PI01_RC_PI_NOT_PERMITED				0x06	/* Paramètre interdit */
#define PI01_RC_PA_SATURATION				0x07	/* Saturation du point d’accès */
#define PI01_RC_TRX_NOT_PERMITED			0x0A	/* Transaction interdite au demandeur (code service non autorisé) */
#define PI01_RC_MOMENTARY_DISCONNECTION		0x0C	/* Coupure momentanée : Le canal de raccordement est rompu momentanément, l’utilisateur est invité à se reconnecter dès que possible. */
#define PI01_RC_ACCESS_NOT_PERMITED			0x0D	/* Accès interdit à l’application utilisatrice. L’accès à cette application est refusé sur l’ensemble du réseau. */
#define PI01_RC_SERVICE_STOPPED				0x0E	/* Service arrêté Suite à une demande de connexion */
#define PI01_RC_ACCESS_POINT_STOPPED		0x10	/* Point d’accès en arrêt bancaire */
#define PI01_RC_UNKNOWN_APDU				0x11	/* APDU inconnue */
#define PI01_RC_INCORRECT_APDU				0x12	/* APDU incorrecte */
#define PI01_RC_INCORRECT_APDU_LEN			0x13	/* Longueur APDU incorrecte */
#define PI01_RC_OUT_OF_BOUNDS_TNR			0x15	/* Valeur du timer de non-réponse (TNR) hors limite. */
#define PI01_RC_UNSUPPORTED_CBCOM_VER		0x1E	/* Version CBcom non supportée */
#define PI01_RC_UNSUPPORTED_APP_PROT_VER	0x20	/* Version de protocole Applicatif non supportée */
#define PI01_RC_INVALID_IPDU_FORMAT			0x23	/* Format IPDU invalide. L'IPDU ne peut être décodée (autre erreur de décodage) */
#define PI01_RC_UNEXCPTED_IPDU				0x24	/* Erreur de cinématique : L'IPDU reçue n'est pas compatible avec l'état de la couche CBcom. */
#define PI01_RC_MISSING_PI					0x25	/* Paramètre(s) Obligatoire(s) absent(s) */
#define PI01_RC_INVALID_PI_LEN				0x26	/* Longueur paramètre invalide. La longueur LI est nulle, différente de la longueur d'un PI de longueur fixe, ou supérieur à la longueur maximale du PI. */
#define PI01_RC_INVALID_PI_VALUE			0x27	/* Valeur paramètre invalide. La valeur du PI n'est pas licite. */
#define PI01_RC_EXPIRED_TC					0x29	/* Expiration de la temporisation de surveillance de la connexion (Tc) */
#define PI01_RC_EXPIRED_TDC					0x2A	/* Expiration de la temporisation de test de disponibilité de la connexion (Tdc) */
#define PI01_RC_OB_ANTICIP_WINDOW_SIZE		0x2B	/* Taille de la fenêtre d’anticipation hors limite */
#define PI01_RC_CHANNEL_ID_DUP_ERROR		0x2C	/* Erreur d’identification du canal de raccordement : Duplication d’identifiant de canal de raccordement. */
#define PI01_RC_UNSUPPORTED_SERVICES_CLASS	0x2D	/* Classe de services non supportée ou non autorisée */
#define PI01_RC_UNSUPPORTED_FLOW_TYPE		0x2E	/* Type de flux non supporté ou non autorisé */
#define PI01_RC_UNSUPPORTED_TRANSFER_MODE	0x2F	/* Mode de transfert non supporté ou incompatible avec la classe de services */
#define PI01_RC_UNKNOWN_UA_ID				0x30	/* Identifiant d’application utilisatrice ou d'entité CBcom connectée non reconnu */
#define PI01_RC_PERSISTANT_ERROR			0x31	/* Erreur persistante. Le nombre d’erreurs détectées sur un canal de raccordement dépasse le seuil admis. */
#define PI01_RC_UNRESPECTED_ANTICIP_WINDOW	0x32	/* Fenêtre d’anticipation non respectée */
#define PI01_RC_MULTIPLEXING_ATTEMPT		0x33	/* Tentative de multiplexage */
#define PI01_RC_CHANNEL_ID_ERROR			0x34	/* Erreur d’identification du canal de raccordement : Canal de raccordement inconnu */
#define PI01_RC_RES_LIMIT_REACHED			0x35	/* Maximum de ressources atteint. Le nombre maximum de Canaux de raccordement est atteint */
#define PI01_RC_EXPIRED_TTN					0x37	/* Expiration de la temporisation de surveillance de la terminaison normale de connexion (Ttn) */
#define PI01_RC_INVALID_BANKING_TRX			0x38	/* Transaction bancaire: Éléments du message applicatif absents ou incorrects*/
#define PI01_RC_NORMAL_TERMINATION			0x80	/* Terminaison normale. Arrêt des activités */


#define CBCOM_APP_PROTOCOL_ID_0001		"\x00\x01"
#define CBCOM_APP_PROTOCOL_VER_0406		"\x04\x06"
#define CBCOM_APP_PROTOCOL_VER_0407		"\x04\x07"
#define CBCOM_APP_PROTOCOL_VER_0408		"\x04\x08"	/*EBE140421: Enh 4.08*/
#define CBCOM_APP_PROTOCOL_VER_0409		"\x04\x09"	/*SNO010515: Enh 4.09*/
#define CBCOM_APP_PROTOCOL_VER_0410		"\x04\x10"	/*SNO0050416: Enh4.10*/  
#define CBCOM_APP_PROTOCOL_VER_0411		"\x04\x11"	/*MTR100517: Enh4.11 */
#define CBCOM_APP_PROTOCOL_VER_0412		"\x04\x12"	/*MTR260318: Enh4.12 */
#define CBCOM_APP_PROTOCOL_VER_0500		"\x05\x00"	/*AMER20190228 PROD00066569: Enh5.00*/
#define CBCOM_APP_PROTOCOL_VER_0501		"\x05\x01"	/*MAK20200323 PROD00074451: Enh5.01*/
#define CBCOM_APP_PROTOCOL_VER_0502		"\x05\x02"	/*ASO20210305 : Enh5.02*/ 
#define CBCOM_APP_PROTOCOL_VER_0503		"\x05\x03"	/*PLUTONL-4280 :Enh5.03*/

/*#define CBCOM_CURR_APP_PROTOCOL_VER	CBCOM_APP_PROTOCOL_VER_0407*/
/*#define CBCOM_CURR_APP_PROTOCOL_VER		CBCOM_APP_PROTOCOL_VER_0408 EBE140421: Enh 4.08*/
/*#define CBCOM_CURR_APP_PROTOCOL_VER		CBCOM_APP_PROTOCOL_VER_0409 SNO010515: Enh 4.09*/ 
/*#define CBCOM_CURR_APP_PROTOCOL_VER		CBCOM_APP_PROTOCOL_VER_0410 SNO0050416: Enh4.10*/ 
/*#define CBCOM_CURR_APP_PROTOCOL_VER       CBCOM_APP_PROTOCOL_VER_0411 MTR100517 Enh4.11 */
/*#define CBCOM_CURR_APP_PROTOCOL_VER         CBCOM_APP_PROTOCOL_VER_0412 MTR260318 Enh4.12 */
/*#define CBCOM_CURR_APP_PROTOCOL_VER         CBCOM_APP_PROTOCOL_VER_0500 AMER20190228 PROD00066569: Enh5.00*/
/*#define CBCOM_CURR_APP_PROTOCOL_VER         CBCOM_APP_PROTOCOL_VER_0501 MAK20200323 PROD00074451: Enh5.01*/
/*#define CBCOM_CURR_APP_PROTOCOL_VER         CBCOM_APP_PROTOCOL_VER_0502*/ /*ASO20210305 : Enh5.02*/
#define CBCOM_CURR_APP_PROTOCOL_VER           CBCOM_APP_PROTOCOL_VER_0503  /*PLUTONL-4280 :Enh5.03*/
#define CBCOM_PROTOCOL_VER_0104			"\x14"

#define PI08_DCF_ASCII				"\x01"
#define PI10_CI_CB					"CB      "

#define PI22_SC_ADM					(1<<0)
#define PI22_SC_WITHDRAWAL			(1<<1)
#define PI22_SC_PAYMENTS			(1<<2)
#define PI22_SC_STOP_LIST			(1<<3)
#define PI22_SC_CERTIFICATE			(1<<4)
#define PI22_SC_CARD_MNG			(1<<5)
#define PI22_SC_PME					(1<<6)



#define PI27_TM_TRANSACTIONAL		"\x01"

#define PI29_FT_PRODUCTION			"\x01"
#define PI29_FT_CERTIFICATION		"\x02"
#define PI29_FT_TEST				"\x03"


#define PI01_LEN					1
#define PI06_LEN					2
#define PI08_LEN					1
#define PI10_LEN					8
#define PI16_LEN					2
#define PI17_LEN					2
#define PI18_LEN					2
#define PI19_LEN					2
#define PI20_LEN					1
#define PI21_LEN					2
#define PI22_LEN					4
#define PI23_LEN					2
#define PI25_LEN					12
#define PI26_LEN					12
#define PI27_LEN					1
#define PI29_LEN					1
#define PI30_LEN					1


typedef struct STlvCBCom
{
   int  nPresent  [ MAX_CBCOM_TLV ];
   int  nPosTlv   [ MAX_CBCOM_TLV ];
   int  nLength;
   char sTlvData  [ MAX_CBCOM_TLV_LEN ];
} TSTlvCBCom;


typedef struct
{
   char    sTlvName;
   int     nType;
   int     nLength;
} cbcom_tlv_info_t;


typedef struct
{
   unsigned char nResultCode;
   char*		szDescription;
} cbcom_result_code;


void InitTlvCBCom(TSTlvCBCom * tlvInfo);
int  AnalyseTlvCBCom(char *buffer,TSTlvCBCom *tlvInfo);
int  GetTlvCBCom (char tlv_name, TSTlvCBCom *tlvInfo,char *data, int *length);
int  AddTlvCBCom (char tlv_name, TSTlvCBCom *tlvInfo, char *data, int length);
int  PutTlvCBCom (char tlv_name, TSTlvCBCom *tlvInfo, char *data, int length);
int  CBComBuildTlv (char * buffer_snd, TSTlvCBCom *tlvInfo);
int PrintTlvCBCom( TSTlvCBCom *tlvInfo );
int  PrintTlvBufferCBCom ( char *sTlvBuffer );

#endif
