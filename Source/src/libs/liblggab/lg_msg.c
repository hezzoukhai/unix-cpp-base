#include <logigab.h>

#define MSG_HDR_OFST						0

#define DEF_LGM_HEADER_NB_FIELDS			12
#define DEF_LGM_HEADER_SIZE					48



/*TODO: replace numbers by labels */

TSLGMsgLayout	DEF_LGM_HEADER = {
	LGM_HEADER,DEF_LGM_HEADER_NB_FIELDS,DEF_LGM_HEADER_SIZE,'X',
	{
		{LG_INST_RESERVED_ZONE,	0,0}, /* Zone réservée établissement  	*/
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  				*/
		{LG_MSG_NBR,			0,0}, /* Numéro de message  			*/
		{LG_TERMINAL_NO,		0,0}, /* Numéro de l'automate  			*/
		{LG_DATE,				0,0}, /* Date (AAMMJJ)  				*/
		{LG_TIME,				0,0}, /* Heure (HHMMSS)  				*/
		{LG_OP_NO,				0,0}, /* Numéro d'opération bancaire  	*/
		{LG_ACC_BATCH_REF,		0,0}, /* Référence arrêté comptable  	*/
		{LG_REENTRY_IND,		0,0}, /* Indicateur de Réentrée  		*/
		{LG_LAST_MSG_IND,		0,0}, /* Indicateur dernier message  	*/
		{LG_FORMAT_IND,			0,0}, /* Indicateur de format  			*/
		{LG_RELEASE_1,			0,0}, /* Release 1 						*/

	}

};

#define DEF_LGM_PERIODIC_REQ_NB_FIELDS		43 + 1 + 1
#define DEF_LGM_PERIODIC_REQ_SIZE			300 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_PERIODIC_REQ = {
	LGM_PERIODIC_REQ,DEF_LGM_PERIODIC_REQ_NB_FIELDS,DEF_LGM_PERIODIC_REQ_SIZE,'X',
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_RES_STATE_CURR,			0,0}, /* Etat des ressources									*/
		{LG_RES_STATE_PREV,			0,0}, /* Etat précédent des ressources							*/
		{LG_TERM_STATE_CURR,		0,0}, /* Etat actuel de l'automate								*/
		{LG_TERM_STATE_PREV,		0,0}, /* Etat précédent de l'automate							*/
		{LG_SOFT_VER_NO,			0,0}, /* Numéro de version logiciel actif						*/
		{LG_SOFT_REL_NO,			0,0}, /* Numéro de release logiciel actif						*/
		{LG_PARAM_FILE_VER_CURR,	0,0}, /* Numéro de version des fichiers paramètres actifs		*/
		{LG_DL_SOFT_VER_NO,			0,0}, /* Numéro de version du logiciel téléchargé				*/
		{LG_DL_SOFT_REL_NO,			0,0}, /* Numéro de release du logiciel téléchargé				*/
		{LG_DL_FILE_PARAM_VER_NO,	0,0}, /* Numéro de version des fichiers paramètres téléchargés ou stockés  */
		{LG_P_CRNCY_TH_BAL,			0,0}, /* Encaisse coffre théorique de la devise principale		*/
		{LG_P_CRNCY_DISP_BAL,		0,0}, /* Encaisse coffre disponible de la devise principale		*/
		{LG_NB_ADV_NACK,			0,0}, /* Comptes rendus non-acquittés ou non-transmis			*/
		{LG_LAST_JE_REF,			0,0}, /* Référence du dernier fichier JE sauvegardé				*/
		{LG_NB_JE_NSAVE,			0,0}, /* Nombre de fichiers JE non sauvegardés					*/
		{LG_HD_AVAIL_SIZE,			0,0}, /* Place disque disponible (k octets)						*/
		{LG_CB_READER_ID,			0,0}, /* Identifiant lecteur coupleur agrée CB					*/
		{LG_MAINTAIN_REQ,			0,0}, /* Demande de maintenance préventive ou curative			*/
		{LG_REQ_RES_NO,				0,0}, /* Numéro de la ressource faisant l’objet d’une demande	*/
		{LG_MAINTAIN_REQ_LABEL,		0,0}, /* Libellé correspondant à la demande de maintenance		*/
		{LG_PRINCIPAL_CRNCY,		0,0}, /* Code devise de la devise principale					*/
		{LG_SPEC_ATM_PARAM_VER_NO,	0,0}, /* Numéro de version des paramètres spécifiques de l’automate  */
		{LG_GRAPHIC_F_VER_NO,		0,0}, /* Numéro de version des fonds graphiques					*/
		{LG_PRINTER_BINS_STATE,		0,0}, /* Etats des bacs (imprimante large ou laser)				*/
		{LG_PIN_IND,				0,0}, /* Indicateur de saisie du code confidentiel				*/
		{LG_CHIP_MNG_REQ_STATE,		0,0}, /* Etat voulu par le gestionnaire pour le traitement du microcircuit  */
		{LG_MAG_CHIP_TERM_CAP,		0,0}, /* Capacité matériel de l’automate à traiter le piste ISO2 et le microcircuit  */
		{LG_TERM_CAPTURE_CAP,		0,0}, /* Capacité matériel de l’automate à capturer les cartes  */
		{LG_TERM_PIN_CTRL_CAP,		0,0}, /* Capacité matériel de l’automate à contrôler le code confidentiel  */
		{LG_BIN_TBL_VER_NO,			0,0}, /* Numéro de version de la table des Bins active			*/
		{LG_AID_TBL_VER_NO,			0,0}, /* Numéro de version de la table des AID EMV active		*/
		{LG_DENIAL_TAC_VER_NO,		0,0}, /* Numéro de version de la table des TAC Denial EMV active  */
		{LG_RSA_KEYS_TBL_VER_NO,	0,0}, /* Numéro de version de la table des clés RSA active		*/
		{LG_CRYPTO_MOD_TYPE,		0,0}, /* Type de module cryptographique utilisé					*/
		{LG_SCD_USAGE_MODE_TERM,	0,0}, /* Modes d’utilisations SCD								*/
		{LG_MANAGED_ENC_FMT,		0,0}, /* Format de chiffrement géré								*/
		{LG_MNG_ROOT_KEY_VER_NO,	0,0}, /* Numéro de version de la clé maître gestionnaire		*/
		{LG_TERM_ROOT_KEY_VER_NO,	0,0}, /* Numéro de version de la clé maître automate			*/
		{LG_VISUALLY_IMPAIRED_TIMEOUT,	0,0}, /* Seuil time out mal voyant							*/
		{LG_CASST_STATE_CHG,		0,0}, /* Changement d’état des cassettes physiques				*/
		{LG_T3_SUSP_CR_MODE,		0,0}, /* Mode crédit en compte des medias de type 3 ou douteux  */
		{LG_DL_OTHER_CV_IND,		0,0}, /* Indicateur de téléchargement autre CV					*/
		{LG_SPEC_FILLER,			0,15}, /* LG_SPEC_FILLER										*/
		{LG_ADD_DATA,				0,0}, /* Additional data */

	}

};

#define DEF_LGM_CASST_WAG_NB_FIELDS		5
#define DEF_LGM_CASST_WAG_SIZE			7

TSLGMsgLayout	DEF_LGM_CASST_WAG = {
	LGM_CASST_WAG,DEF_LGM_CASST_WAG_NB_FIELDS,DEF_LGM_CASST_WAG_SIZE,'X',
	{
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_WAG_TYPE_CODE,		0,0}, /* Code type de Wagon  */
		{LG_CHG_ST_NB_CASST,	0,0}, /* Nombre de cassettes ayant changé d’état  */
		{LG_DATA_FIELD_LEN,		0,0}, /* Longueur du champ de données  */
		{LG_ADD_DATA,			0,0}, /* Cassettes  */

	}

};

#define DEF_LGM_CASST_NB_FIELDS		7
#define DEF_LGM_CASST_WAG_SIZE		8

TSLGMsgLayout	DEF_LGM_CASST = {
	LGM_CASST,DEF_LGM_CASST_NB_FIELDS,DEF_LGM_CASST_WAG_SIZE,'X',
	{
		{LG_CASST_ORDER_NO,		0,0}, /* Numéro d’ordre de la cassette  */
		{LG_CASST_NO,			0,0}, /* Numéro de la cassette  */
		{LG_CASST_TYPE,			0,0}, /* Type de la cassette  */
		{LG_MEDIA_TYPE,			0,0}, /* Type de media de la cassette  */
		{LG_CASST_CURR_STATE,	0,0}, /* Etat actuel de la cassette  */
		{LG_CASST_PREV_STATE,	0,0}, /* Etat précédent de la cassette  */
		{LG_SPEC_FILLER,		0,1}, /* LG_SPEC_FILLER */

	}

};

#define DEF_LGM_PERIODIC_RESP_NB_FIELDS		12 + 1
#define DEF_LGM_PERIODIC_RESP_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_PERIODIC_RESP = {
	LGM_PERIODIC_RESP,DEF_LGM_PERIODIC_RESP_NB_FIELDS,DEF_LGM_PERIODIC_RESP_SIZE,LG_FC_PERIODIC_MSG,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_ACTION_CODE,			0,0}, /* Code action  */
		{LG_PARAM_FILE_VER_TRNSF,	0,0}, /* Numéro de version des fichiers paramètres transférés  */
		{LG_EXP_CMD_OP_CD,			0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_CMD_DATA,			0,0}, /* Données de l’opération d'exploitation télécommandée  */
		{LG_SCD_USAGE_MODE_MNG,		0,0}, /* Modes d’utilisations SCD  */
		{LG_SPEC_FILLER,			0,7}, /* SPEC_FILLER */
		{LG_NEIGHBOR_TERM_1_STATE,	0,0}, /* Etat de l’automate voisin 1  */
		{LG_NEIGHBOR_TERM_2_STATE,	0,0}, /* Etat de l’automate voisin 2  */
		{LG_ACQ_ID,					0,0}, /* Identifiant de l’organisme acquéreur  */
		{LG_SPEC_FILLER,			0,19}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_MSG_ACK_CHANNEL_2_NB_FIELDS		12
#define DEF_LGM_MSG_ACK_CHANNEL_2_SIZE			48

TSLGMsgLayout	DEF_LGM_MSG_ACK_CHANNEL_2 = {
	LGM_MSG_ACK_CHANNEL_2,DEF_LGM_MSG_ACK_CHANNEL_2_NB_FIELDS,DEF_LGM_MSG_ACK_CHANNEL_2_SIZE,LG_FC_MSG_ACK_2,
	{
		{LG_INST_RESERVED_ZONE,		0,0}, /* Zone réservée établissement  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_MSG_NBR,				0,0}, /* Numéro de message  */
		{LG_TERMINAL_NO,			0,0}, /* Numéro de l'automate  */
		{LG_DATE,					0,0}, /* Date (AAMMJJ)  */
		{LG_TIME,					0,0}, /* Heure (HHMMSS)  */
		{LG_OP_NO,					0,0}, /* Numéro d'opération bancaire  */
		{LG_ACC_BATCH_REF,			0,0}, /* Référence arrêté comptable  */
		{LG_REENTRY_IND,			0,0}, /* Indicateur de Réentrée  */
		{LG_LAST_MSG_IND,			0,0}, /* Indicateur dernier message  */
		{LG_FORMAT_IND,				0,0}, /* Indicateur de format  */
		{LG_RELEASE_1,				0,0}, /* Release 1 */


	}

};

#define DEF_LGM_EXCEPTION_MSG_NB_FIELDS		15
#define DEF_LGM_EXCEPTION_MSG_SIZE			112

TSLGMsgLayout	DEF_LGM_EXCEPTION_MSG = {
	LGM_EXCEPTION_MSG,DEF_LGM_EXCEPTION_MSG_NB_FIELDS,DEF_LGM_EXCEPTION_MSG_SIZE,LG_FC_EXCP_MSG,
	{
		{LG_SPEC_FILLER,		0,8}, /* SPEC_FILLER */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_SPEC_FILLER,		0,4}, /* SPEC_FILLER */
		{LG_TERMINAL_NO,		0,0}, /* Numéro de l'automate  */
		{LG_SPEC_FILLER,		0,6}, /* SPEC_FILLER */
		{LG_SPEC_FILLER,		0,6}, /* SPEC_FILLER */
		{LG_SPEC_FILLER,		0,6}, /* SPEC_FILLER */
		{LG_SPEC_FILLER,		0,5}, /* SPEC_FILLER */
		{LG_SPEC_FILLER,		0,1}, /* SPEC_FILLER */
		{LG_SPEC_FILLER,		0,1}, /* SPEC_FILLER */
		{LG_SPEC_FILLER,		0,1}, /* SPEC_FILLER */
		{LG_SPEC_FILLER,		0,1}, /* SPEC_FILLER */
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_SPEC_FILLER,		0,62}, /* SPEC_FILLER */


	}

};


#define DEF_LGM_REENTRY_RESP_NB_FIELDS		7 + 1
#define DEF_LGM_REENTRY_RESP_SIZE			112 - MSG_HDR_OFST


TSLGMsgLayout	DEF_LGM_REENTRY_RESP = {
	LGM_REENTRY_RESP,DEF_LGM_REENTRY_RESP_NB_FIELDS,DEF_LGM_REENTRY_RESP_SIZE,LG_FC_REENTRY_RESP,
	{
		{LG_MSG_HEADER,		0,0}, /* Header */
		{LG_SEPERATOR,		0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,	0,0}, /* Code fonction  */
		{LG_ACTION_CODE,	0,0}, /* Code action  */
		{LG_SPEC_FILLER,	0,4}, /* SPEC_FILLER */
		{LG_EXP_CMD_OP_CD,	0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_CMD_DATA,	0,0}, /* Données de l’opération d'exploitation télécommandée  */
		{LG_SPEC_FILLER,	0,43}, /* SPEC_FILLER */
	}

};

#define DEF_LGM_KEYS_DL_REQ_1_NB_FIELDS		4 + 1
#define DEF_LGM_KEYS_DL_REQ_1_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_KEYS_DL_REQ_1 = {
	LGM_KEYS_DL_REQ_1,DEF_LGM_KEYS_DL_REQ_1_NB_FIELDS,DEF_LGM_KEYS_DL_REQ_1_SIZE,LG_FC_KEYS_DL,
	{
		{LG_MSG_HEADER,		0,0}, /* Header */
		{LG_CONS_STATUS,	0,0}, /* Status constructeur  */
		{LG_TERM_STATE_VAR,	0,0}, /* Variable d'état automate  */
		{LG_KEY_TYPE_RECV,	0,0}, /* Type de clé reçu  */
		{LG_SPEC_FILLER,	0,61}, /* SPEC_FILLER */


	}

};

#define DEF_LGM_KEYS_DL_RESP_1_NB_FIELDS	11 + 1
#define DEF_LGM_KEYS_DL_RESP_1_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_KEYS_DL_RESP_1 = {
	LGM_KEYS_DL_RESP_1,DEF_LGM_KEYS_DL_RESP_1_NB_FIELDS,DEF_LGM_KEYS_DL_RESP_1_SIZE,LG_FC_KEYS_DL,
	{
		{LG_MSG_HEADER,		0,0}, /* Header */
		{LG_SEPERATOR,		0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,	0,0}, /* Code fonction  */
		{LG_KEY_TYPE_SENT,	0,0}, /* Type de clé envoyé  */
		{LG_KEY_IND,		0,0}, /* Indice de clé  */
		{LG_ENV,			0,0}, /* Environnement  */
		{LG_KEY_ENC,		0,0}, /* Clé chiffré  */
		{LG_KEY_SENT_IND,	0,0}, /* Indicateur clé transmise  */
		{LG_ACTION_CODE,	0,0}, /* Code action  */
		{LG_EXP_CMD_OP_CD,	0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_CMD_DATA,	0,0}, /* Données de l’opération d'exploitation télécommandée  */
		{LG_SPEC_FILLER,	0,26}, /* SPEC_FILLER */
	}

};

#define DEF_LGM_PARAM_FILE_DL_REQ_NB_FIELDS		7 + 1
#define DEF_LGM_PARAM_FILE_DL_REQ_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_PARAM_FILE_DL_REQ = {
	LGM_PARAM_FILE_DL_REQ,DEF_LGM_PARAM_FILE_DL_REQ_NB_FIELDS,DEF_LGM_PARAM_FILE_DL_REQ_SIZE,LG_FC_PERSO_FILE_DL,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_PARAM_FILE_VER_CURR,	0,0}, /* Numéro de version des fichiers paramètres actifs  */
		{LG_PARAM_FILE_VER_TRNSF,	0,0}, /* Numéro de version des fichiers paramètres transférés  */
		{LG_LAST_DATA_BLOCK_RECV,	0,0}, /* Dernier numéro de bloc reçu  */
		{LG_FILE_TRSFT_REPORT_CD,	0,0}, /* Code compte-rendu de transfert de fichier  */
		{LG_RECV_MSG_MAX_SIZE,		0,0}, /* Taille maximum des messages en réception  */
		{LG_TRNSF_PARAM_FILE_CD,	0,0}, /* Code fichier paramètre en cours de transfert  */
		{LG_SPEC_FILLER,			0,45}, /* SPEC_FILLER */
	}

};

#define DEF_LGM_PARAM_FILE_DL_RESP_NB_FIELDS		9 + 1
#define DEF_LGM_PARAM_FILE_DL_RESP_SIZE				65 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_PARAM_FILE_DL_RESP = {
	LGM_PARAM_FILE_DL_RESP,DEF_LGM_PARAM_FILE_DL_RESP_NB_FIELDS,DEF_LGM_PARAM_FILE_DL_RESP_SIZE,LG_FC_PERSO_FILE_DL,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_ACTION_CODE,			0,0}, /* Code action  */
		{LG_TRNSF_PARAM_FILE_CD,	0,0}, /* Code fichier paramètre en cours de transfert  */
		{LG_PARAM_FILE_VER_TRNSF,	0,0}, /* Numéro de version des fichiers paramètres transférés  */
		{LG_BLOCK_NO,				0,0}, /* Numéro de bloc  */
		{LG_ACK_IMM_DIFF,			0,0}, /* Prise en compte immédiate ou différée  */
		{LG_DATA_LEN,				0,0}, /* Longueur du champ de données  */
		{LG_ADD_DATA,				0,0}, /* Longueur du champ de données  */
	}

};

#define DEF_LGM_TERM_SPEC_REQ_NB_FIELDS		2 + 1
#define DEF_LGM_TERM_SPEC_REQ_SIZE			229 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_TERM_SPEC_REQ = {
	LGM_TERM_SPEC_REQ,DEF_LGM_TERM_SPEC_REQ_NB_FIELDS,DEF_LGM_TERM_SPEC_REQ_SIZE,LG_FC_TERM_SPEC,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_TERM_LAST_MSG_IND,		0,0}, /* Indicateur dernier message automate  */
		{LG_ADD_DATA,				0,180},
	}

};



#define DEF_LGM_TERM_SPEC_RESP_NB_FIELDS		6 + 1
#define DEF_LGM_TERM_SPEC_RESP_SIZE				112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_TERM_SPEC_RESP = {
	LGM_TERM_SPEC_RESP,DEF_LGM_TERM_SPEC_RESP_NB_FIELDS,DEF_LGM_TERM_SPEC_RESP_SIZE,LG_FC_TERM_SPEC,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_ACTION_CODE,		0,0}, /* Code action  */
		{LG_EXP_CMD_OP_CD,		0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_CMD_DATA,		0,0}, /* Données de l’opération d'exploitation télécommandée  */
		{LG_SPEC_FILLER,		0,47}, /* SPEC_FILLER */
	}

};


#define DEF_LGM_TERM_SPEC_PARAM_REQ_NB_FIELDS	2 + 1
#define DEF_LGM_TERM_SPEC_PARAM_REQ_SIZE		112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_TERM_SPEC_PARAM_REQ = {
	LGM_TERM_SPEC_PARAM_REQ,DEF_LGM_TERM_SPEC_PARAM_REQ_NB_FIELDS,DEF_LGM_TERM_SPEC_PARAM_REQ_SIZE,LG_FC_TERM_SPEC_PARAM,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SPEC_ATM_PARAM_VER_NO,	0,0}, /* Numéro de version des paramètres spécifiques de l’automate  */
		{LG_SPEC_FILLER,			0,60}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_TERM_SPEC_PARAM_RESP_NB_FIELDS	7 + 1
#define DEF_LGM_TERM_SPEC_PARAM_RESP_SIZE		512 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_TERM_SPEC_PARAM_RESP = {
	LGM_TERM_SPEC_PARAM_RESP,DEF_LGM_TERM_SPEC_PARAM_RESP_NB_FIELDS,DEF_LGM_TERM_SPEC_PARAM_RESP_SIZE,LG_FC_TERM_SPEC_PARAM,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_ACTION_CODE,			0,0}, /* Code action  */
		{LG_EXP_CMD_OP_CD,			0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_CMD_DATA,			0,0}, /* Données de l’opération d'exploitation télécommandée  */
		{LG_SPEC_ATM_PARAM_VER_NO,	0,0}, /* Numéro de version des paramètres spécifiques de l’automate  */
		{LG_ADD_DATA,				0, 443},

	}

};

#define DEF_LGM_TRANSFER_REQ_NB_FIELDS	5 + 1
#define DEF_LGM_TRANSFER_REQ_SIZE		63 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_TRANSFER_REQ = {
	LGM_TRANSFER_REQ,DEF_LGM_TRANSFER_REQ_NB_FIELDS,DEF_LGM_TRANSFER_REQ_SIZE,LG_FC_EJ_TRNSF_REQ,
	{
		{LG_MSG_HEADER,					0,0}, /* Header */
		{LG_CURR_TRSNF_FILE_REF,		0,0}, /* Référence du fichier en cours de transfert  */
		{LG_CURR_TRSNF_FILE_LINE_NO,	0,0}, /* Numéro de la ligne dans le fichier en cours de transfert  */
		{LG_RECORD_SIZE,				0,150}, /* Taille de l’enregistrement  */
		{LG_MSG_NB_RECORD,				0,0}, /* Nombre d’enregistrements transféré dans le message  */
		{LG_TRSNF_FILE_DATA,			0,0}, /* Données du fichier en cours de transfert  */ /*EBETODO*/


	}

};

#define DEF_LGM_EXP_CMD_REPORT_RMT_1_NB_FIELDS	5 + 1
#define DEF_LGM_EXP_CMD_REPORT_RMT_1_SIZE		250 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_EXP_CMD_REPORT_RMT_1 = {
	LGM_EXP_CMD_REPORT_RMT_1,DEF_LGM_EXP_CMD_REPORT_RMT_1_NB_FIELDS,DEF_LGM_EXP_CMD_REPORT_RMT_1_SIZE,LG_FC_REMOTE_EXP_CMD_REPORT,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_EXP_CMD_OP_CD,		0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_REPORT_CODE,	0,0}, /* Code compte rendu d'exploitation  */
		{LG_EXP_OP_RESULT,		0,150}, /* Résultat de l'opération d'exploitation  */
		{LG_PRINCIPAL_CRNCY,	0,0}, /* Code devise de la devise principale  */
		{LG_SPEC_FILLER,		0,45}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_EXP_CMD_REPORT_RMT_2_NB_FIELDS	5 + 1
#define DEF_LGM_EXP_CMD_REPORT_RMT_2_SIZE		250 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_EXP_CMD_REPORT_RMT_2 = {
	LGM_EXP_CMD_REPORT_RMT_2,DEF_LGM_EXP_CMD_REPORT_RMT_2_NB_FIELDS,DEF_LGM_EXP_CMD_REPORT_RMT_2_SIZE,LG_FC_REMOTE_EXP_CMD_REPORT,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_EXP_CMD_OP_CD,		0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_REPORT_CODE,	0,0}, /* Code compte rendu d'exploitation  */
		{LG_EXP_OP_RESULT,		0,50}, /* Résultat de l'opération d'exploitation  */
		{LG_PRINCIPAL_CRNCY,	0,0}, /* Code devise de la devise principale  */
		{LG_SPEC_FILLER,		0,145}, /* SPEC_FILLER */
	}

};

#define DEF_LGM_EXP_CMD_REPORT_RMT_RESP_NB_FIELDS	6 + 1
#define DEF_LGM_EXP_CMD_REPORT_RMT_RESP_SIZE		112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_EXP_CMD_REPORT_RMT_RESP = {
	LGM_EXP_CMD_REPORT_RMT_RESP,DEF_LGM_EXP_CMD_REPORT_RMT_RESP_NB_FIELDS,DEF_LGM_EXP_CMD_REPORT_RMT_RESP_SIZE,LG_FC_REMOTE_EXP_CMD_REPORT,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_EXP_CMD_OP_CD,		0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_CMD_DATA,		0,0}, /* Données de l’opération d'exploitation télécommandée  */
		{LG_ACTION_CODE,		0,0}, /* Code action  */
		{LG_SPEC_FILLER,		0,47}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_INIT_TLP_REQ_NB_FIELDS		5 + 1
#define DEF_LGM_INIT_TLP_REQ_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_INIT_TLP_REQ = {
	LGM_INIT_TLP_REQ,DEF_LGM_INIT_TLP_REQ_NB_FIELDS,DEF_LGM_INIT_TLP_REQ_SIZE,LG_FC_INIT_TLP,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_BIN_TBL_VER_NO,			0,0}, /* Numéro de version de la table des Bins active  */
		{LG_AID_TBL_VER_NO,			0,0}, /* Numéro de version de la table des AID EMV active  */
		{LG_DENIAL_TAC_VER_NO,		0,0}, /* Numéro de version de la table des TAC Denial EMV active  */
		{LG_RSA_KEYS_TBL_VER_NO,	0,0}, /* Numéro de version de la table des clés RSA active  */
		{LG_SPEC_FILLER,			0,48}, /* SPEC_FILLER */

	}

};

/*#define DEF_LGM_INIT_TLP_RESP_NB_FIELDS		31 + 1
#define DEF_LGM_INIT_TLP_RESP_SIZE			200 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_INIT_TLP_RESP = {
	LGM_INIT_TLP_RESP,DEF_LGM_INIT_TLP_RESP_NB_FIELDS,DEF_LGM_INIT_TLP_RESP_SIZE, LG_FC_INIT_TLP,
	{
		{LG_MSG_HEADER,						0,0},  Header
		{LG_SEPERATOR,						0,0},  Séparateur
		{LG_FUNCTION_CODE,					0,0},  Code fonction
		{LG_NB_TABLES_TO_TRNSFT,			0,0},  Nombre de tables à transférer
		{LG_MNG_TBL_ID_TO_TRNSFT,			0,0},  Identifiant de la table gestionnaire à transférer
		{LG_TABLE_TO_TRNSFT_MNG_VER_NO,		0,0},  Numéro de version gestionnaire de la table à transférer
		{LG_TABLE_TO_TRNSFT_SIZE,			0,0},  Taille totale de la table à transférer
		{LG_MNG_TBL_ID_TO_TRNSFT,			0,0},  Identifiant de la table gestionnaire à transférer
		{LG_TABLE_TO_TRNSFT_MNG_VER_NO,		0,0},  Numéro de version gestionnaire de la table à transférer
		{LG_TABLE_TO_TRNSFT_SIZE,			0,0},  Taille totale de la table à transférer
		{LG_MNG_TBL_ID_TO_TRNSFT,			0,0},  Identifiant de la table gestionnaire à transférer
		{LG_TABLE_TO_TRNSFT_MNG_VER_NO,		0,0},  Numéro de version gestionnaire de la table à transférer
		{LG_TABLE_TO_TRNSFT_SIZE,			0,0},  Taille totale de la table à transférer
		{LG_MNG_TBL_ID_TO_TRNSFT,			0,0},  Identifiant de la table gestionnaire à transférer
		{LG_TABLE_TO_TRNSFT_MNG_VER_NO,		0,0},  Numéro de version gestionnaire de la table à transférer
		{LG_TABLE_TO_TRNSFT_SIZE,			0,0},  Taille totale de la table à transférer
		{LG_MNG_TBL_ID_TO_TRNSFT,			0,0},  Identifiant de la table gestionnaire à transférer
		{LG_TABLE_TO_TRNSFT_MNG_VER_NO,		0,0},  Numéro de version gestionnaire de la table à transférer
		{LG_TABLE_TO_TRNSFT_SIZE,			0,0},  Taille totale de la table à transférer
		{LG_MNG_TBL_ID_TO_TRNSFT,			0,0},  Identifiant de la table gestionnaire à transférer
		{LG_TABLE_TO_TRNSFT_MNG_VER_NO,		0,0},  Numéro de version gestionnaire de la table à transférer
		{LG_TABLE_TO_TRNSFT_SIZE,			0,0},  Taille totale de la table à transférer
		{LG_MNG_TBL_ID_TO_TRNSFT,			0,0},  Identifiant de la table gestionnaire à transférer
		{LG_TABLE_TO_TRNSFT_MNG_VER_NO,		0,0},  Numéro de version gestionnaire de la table à transférer
		{LG_TABLE_TO_TRNSFT_SIZE,			0,0},  Taille totale de la table à transférer
		{LG_MNG_TBL_ID_TO_TRNSFT,			0,0},  Identifiant de la table gestionnaire à transférer
		{LG_TABLE_TO_TRNSFT_MNG_VER_NO,		0,0},  Numéro de version gestionnaire de la table à transférer
		{LG_TABLE_TO_TRNSFT_SIZE,			0,0},  Taille totale de la table à transférer
		{LG_MNG_TBL_ID_TO_TRNSFT,			0,0},  Identifiant de la table gestionnaire à transférer
		{LG_TABLE_TO_TRNSFT_MNG_VER_NO,		0,0},  Numéro de version gestionnaire de la table à transférer
		{LG_TABLE_TO_TRNSFT_SIZE,			0,0},  Taille totale de la table à transférer
		{LG_SPEC_FILLER,					0,31},  SPEC_FILLER

	}

};*/

#define DEF_LGM_INIT_TLP_RESP_NB_FIELDS		5 + 1
#define DEF_LGM_INIT_TLP_RESP_SIZE			200 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_INIT_TLP_RESP = {
	LGM_INIT_TLP_RESP,DEF_LGM_INIT_TLP_RESP_NB_FIELDS,DEF_LGM_INIT_TLP_RESP_SIZE, LG_FC_INIT_TLP,
	{
		{LG_MSG_HEADER,						0,0}, /* Header */
		{LG_SEPERATOR,						0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,					0,0}, /* Code fonction  */
		{LG_NB_TABLES_TO_TRNSFT,			0,0}, /* Nombre de tables à transférer  */
		{LG_ADD_DATA,						0,117}, /* Add DATA  */
		{LG_SPEC_FILLER,					0,31}, /* SPEC_FILLER */

	}

};
#define DEF_LGM_TBL_TRNSF_REQ_NB_FIELDS		5 + 1
#define DEF_LGM_TBL_TRNSF_REQ_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_TBL_TRNSF_REQ = {
	LGM_TBL_TRNSF_REQ,DEF_LGM_TBL_TRNSF_REQ_NB_FIELDS,DEF_LGM_TBL_TRNSF_REQ_SIZE,LG_FC_TBL_TRNSF,
	{
		{LG_MSG_HEADER,					0,0}, /* Header */
		{LG_TRSNF_TBL_TERM_ID,			0,0}, /* Identifiant automate de la table en cours de transfert  */
		{LG_TRSNF_TBL_TERM_VER_NO,		0,0}, /* Numéro de version automate de la table en cours de transfert  */
		{LG_LAST_TERM_RECV_BLOCK_NO,	0,0}, /* Numéro du dernier bloc reçu par l’automate  */
		{LG_RECV_MSG_MAX_SIZE,			0,0}, /* Taille maximum des messages en réception  */
		{LG_SPEC_FILLER,				0,48}, /* SPEC_FILLER */
	}

};

#define DEF_LGM_TBL_TRNSF_RESP_NB_FIELDS	7 + 1
#define DEF_LGM_TBL_TRNSF_RESP_SIZE			66 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_TBL_TRNSF_RESP = {
	LGM_TBL_TRNSF_RESP,DEF_LGM_TBL_TRNSF_RESP_NB_FIELDS,DEF_LGM_TBL_TRNSF_RESP_SIZE,LG_FC_TBL_TRNSF,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_TRSNF_TBL_MNG_ID,		0,0}, /* Identifiant gestionnaire de la table en cours de transfert  */
		{LG_TRSNF_TBL_MNG_VER_NO,	0,0}, /* Numéro de version gestionnaire de la table en cours de transfert  */
		{LG_CURR_TRSNF_BLOCK_NO,	0,0}, /* Numéro de bloc de données en cours de transfert  */
		{LG_DATA_LEN,				0,0}, /* Longueur du champ de données  */
		{LG_ADD_DATA,				0,0}, /* Additional data */
	}

};


#define DEF_LGM_END_TLP_REQ_NB_FIELDS		7 + 1
#define DEF_LGM_END_TLP_REQ_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_END_TLP_REQ = {
	LGM_END_TLP_REQ,DEF_LGM_END_TLP_REQ_NB_FIELDS,DEF_LGM_END_TLP_REQ_SIZE, LG_FC_END_OF_TLP,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_TP_REPORT_CODE,			0,0}, /* Code compte rendu de Télé Paramétrage  */
		{LG_ERR_TBL_ID,				0,0}, /* Identifiant de la table en erreur  */
		{LG_BIN_TBL_VER_NO,			0,0}, /* Numéro de version de la table des Bins active  */
		{LG_AID_TBL_VER_NO,			0,0}, /* Numéro de version de la table des AID EMV active  */
		{LG_DENIAL_TAC_VER_NO,		0,0}, /* Numéro de version de la table des TAC Denial EMV active  */
		{LG_RSA_KEYS_TBL_VER_NO,	0,0}, /* Numéro de version de la table des clés RSA active  */
		{LG_SPEC_FILLER,			0,44}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_END_TLP_RESP_NB_FIELDS		6 + 1
#define DEF_LGM_END_TLP_RESP_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_END_TLP_RESP = {
	LGM_END_TLP_RESP,DEF_LGM_END_TLP_RESP_NB_FIELDS,DEF_LGM_END_TLP_RESP_SIZE,LG_FC_END_OF_TLP,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_ACTION_CODE,		0,0}, /* Code action  */
		{LG_EXP_CMD_OP_CD,		0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_CMD_DATA,		0,0}, /* Données de l’opération d'exploitation télécommandée  */
		{LG_SPEC_FILLER,		0,47}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_KEYS_DL_REQ_2_NB_FIELDS		13 + 1
#define DEF_LGM_KEYS_DL_REQ_2_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_KEYS_DL_REQ_2 = {
	LGM_KEYS_DL_REQ_2,DEF_LGM_KEYS_DL_REQ_2_NB_FIELDS,DEF_LGM_KEYS_DL_REQ_2_SIZE,LG_FC_CRYPTO_DL_REQ_2,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_TERM_STATE_CURR,		0,0}, /* Etat actuel de l'automate  */
		{LG_TERM_STATE_VAR,			0,0}, /* Variable d'état automate  */
		{LG_CRYPTO_MOD_TYPE,		0,0}, /* Type de module cryptographique utilisé  */
		{LG_KEY_REQ_REASON_CODE,	0,0}, /* Motif de la demande des clés  */
		{LG_SEQUENCE_NO,			0,0}, /* Numéro de séquence  */
		{LG_SCD_USAGE_MODE_TERM,	0,0}, /* Modes d’utilisations SCD  */
		{LG_MANAGED_ENC_FMT,		0,0}, /* Format de chiffrement géré  */
		{LG_MNG_ROOT_KEY_VER_NO,	0,0}, /* Numéro de version de la clé maître gestionnaire  */
		{LG_TERM_ROOT_KEY_VER_NO,	0,0}, /* Numéro de version de la clé maître automate  */
		{LG_SPEC_FILLER,			0,42}, /* SPEC_FILLER */
		{LG_KEY_DATA_BLOCK_LEN,		0,0}, /* Longueur du bloc de données Clés  */
		{LG_KEY_DATA_BLOCK,			0,0}, /* Bloc de données Clés  */

	}

};

#define DEF_LGM_KEYS_DL_RESP_2_NB_FIELDS		11 + 1 + 1
#define DEF_LGM_KEYS_DL_RESP_2_SIZE				112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_KEYS_DL_RESP_2 = {
	LGM_KEYS_DL_RESP_2,DEF_LGM_KEYS_DL_RESP_2_NB_FIELDS,DEF_LGM_KEYS_DL_RESP_2_SIZE,LG_FC_CRYPTO_DL_REQ_2,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_ACTION_CODE,			0,0}, /* Code action  */
		{LG_EXP_CMD_OP_CD,			0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_CMD_DATA,			0,0}, /* Données de l’opération d'exploitation télécommandée  */
		{LG_KEY_REQ_REASON_CODE,	0,0}, /* Motif de la demande des clés  */
		{LG_SEQUENCE_NO,			0,0}, /* Numéro de séquence  */
		{LG_SCD_USAGE_MODE_MNG,		0,0}, /* Modes d’utilisations SCD  */
		{LG_MNG_STATE_VAR,			0,0}, /* Variable d’état gestionnaire  */
		{LG_SPEC_FILLER,			0,37}, /* SPEC_FILLER */
		{LG_KEY_DATA_BLOCK_LEN,		0,0}, /* Longueur du bloc de données Clés  */
		{LG_KEY_DATA_BLOCK,			0,0}, /* Bloc de données Clés  */

	}

};

#define DEF_LGM_EXP_CMD_REPORT_MAN_1_NB_FIELDS		6 + 1
#define DEF_LGM_EXP_CMD_REPORT_MAN_1_SIZE			250 - MSG_HDR_OFST


TSLGMsgLayout	DEF_LGM_EXP_CMD_REPORT_MAN_1 = {
	LGM_EXP_CMD_REPORT_MAN_1,DEF_LGM_EXP_CMD_REPORT_MAN_1_NB_FIELDS,DEF_LGM_EXP_CMD_REPORT_MAN_1_SIZE,LG_FC_LOCAL_EXP_CMD_REPORT,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_LOCAL_EXP_OP_CD,		0,0}, /* Code opération d'exploitation locale  */
		{LG_EXP_REPORT_CODE,		0,0}, /* Code compte rendu d'exploitation  */
		{LG_EXP_OP_RESULT,			0,150}, /* Résultat de l'opération d'exploitation  */
		{LG_EXP_MODE,				0,0}, /* Mode exploitant  */
		{LG_PRINCIPAL_CRNCY,		0,0}, /* Code devise de la devise principale  */
		{LG_SPEC_FILLER,			0,43}, /* SPEC_FILLER */
	}
};


#define DEF_LGM_EXP_CMD_REPORT_MAN_2_NB_FIELDS		8 + 1
#define DEF_LGM_EXP_CMD_REPORT_MAN_2_SIZE			250 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_EXP_CMD_REPORT_MAN_2 = {
	LGM_EXP_CMD_REPORT_MAN_2,DEF_LGM_EXP_CMD_REPORT_MAN_2_NB_FIELDS,DEF_LGM_EXP_CMD_REPORT_MAN_2_SIZE,LG_FC_LOCAL_EXP_CMD_REPORT,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_LOCAL_EXP_OP_CD,		0,0}, /* Code opération d'exploitation locale  */
		{LG_EXP_REPORT_CODE,		0,0}, /* Code compte rendu d'exploitation  */
		{LG_EXP_OP_RESULT,			0,50}, /* Résultat de l'opération d'exploitation  */
		{LG_AMNT_OUT_TH_CRNY_1,		0,0}, /* Encaisse Out théorique dans la devise 1 (Devise principale)  */
		{LG_EXP_MODE,				0,0}, /* Mode exploitant  */
		{LG_PRINCIPAL_CRNCY,		0,0}, /* Code devise de la devise principale  */
		{LG_AVAIL_BAL_CRNCY_1,		0,0}, /* Encaisse Out disponible dans la devise 1(devise principale)  */
		{LG_SPEC_FILLER,			0,123}, /* SPEC_FILLER */
	}
};

#define DEF_LGM_EXP_CMD_REPORT_MAN_RESP_NB_FIELDS	4 + 1
#define DEF_LGM_EXP_CMD_REPORT_MAN_RESP_SIZE		112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_EXP_CMD_REPORT_MAN_RESP = {
	LGM_EXP_CMD_REPORT_MAN_RESP,DEF_LGM_EXP_CMD_REPORT_MAN_RESP_NB_FIELDS,DEF_LGM_EXP_CMD_REPORT_MAN_RESP_SIZE,LG_FC_LOCAL_EXP_CMD_REPORT,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_LOCAL_EXP_OP_CD,	0,0}, /* Code opération d'exploitation locale  */
		{LG_SPEC_FILLER,		0,60}, /* SPEC_FILLER */
	}

};

#define DEF_LGM_INIT_TRN_REQ_NB_FIELDS		3 + 1
#define DEF_LGM_INIT_TRN_REQ_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_INIT_TRN_REQ = {
	LGM_INIT_TRN_REQ,DEF_LGM_INIT_TRN_REQ_NB_FIELDS,DEF_LGM_INIT_TRN_REQ_SIZE,LG_FC_INIT_TRN,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_ISO2_CONTENT,		0,0}, /* Contenu ISO2  */
		{LG_CH_NAME,			0,0}, /* Nom du porteur  */
		{LG_SPEC_FILLER,		0,1}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_INITIAL_REQ_NB_FIELDS		21 + 1
#define DEF_LGM_INITIAL_REQ_SIZE			154 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_INITIAL_REQ = {
	LGM_INITIAL_REQ,DEF_LGM_INITIAL_REQ_NB_FIELDS,DEF_LGM_INITIAL_REQ_SIZE,LG_FC_INIT_REQ,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_ISO2_CONTENT,		0,0}, /* Contenu ISO2  */
		{LG_PIN_LEN,			0,0}, /* Longueur du code confidentiel  */
		{LG_PIN_BLOCK,			0,0}, /* Code chiffré  */
		{LG_TEMP_CARD_IND,		0,0}, /* Indicateur carte temporisée  */
		{LG_B0_PROC_CTX,		0,0}, /* Contexte traitement puce B0’  */
		{LG_B0_AUTH_ITEM,		0,0}, /* Elément d'authentification B0’  */
		{LG_BO_CALC_DATA,		0,0}, /* Données du calcul B0’  */
		{LG_B0_FINAL_STATE_W,	0,0}, /* Mot d'état final B0’  */
		{LG_BO_PROC_RES,		0,0}, /* Résultat traitement microcircuit B0’  */
		{LG_OP_PROC_MODE,		0,0}, /* Mode de traitement de l’opération  */
		{LG_SPEC_FILLER,		0,1}, /* SPEC_FILLER */
		{LG_PIN_ENTRY_CODE,		0,0}, /* Code saisie du code confidentiel  */
		{LG_PRINCIPAL_CRNCY,	0,0}, /* Code devise de la devise principale  */
		{LG_EMV_PROC_RESULT,	0,0}, /* Résultat du traitement du microcircuit EMV  */
		{LG_EMV_WAG_IND,		0,0}, /* Indicateur wagon EMV  */
		{LG_SCD_USAGE_MODE_TERM,0,0}, /* Modes d’utilisations SCD  */
		{LG_SPEC_FILLER,		0,1}, /* SPEC_FILLER */
		{LG_MAC_TYPE,			0,0}, /* Type de scellement exécuté  */
		{LG_MAC_LEN,			0,0}, /* Longueur du sceau  */
		{LG_MAC,				0,0}, /* Sceau du message  */
		{LG_ADD_DATA,			0,0}, /*Un Wagon EMV */
	}

};

#define DEF_LGM_INITIAL_REQ_RESP_NB_FIELDS		17 + 1
#define DEF_LGM_INITIAL_REQ_RESP_SIZE			158 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_INITIAL_REQ_RESP = {
	LGM_INITIAL_REQ_RESP,DEF_LGM_INITIAL_REQ_RESP_NB_FIELDS,DEF_LGM_INITIAL_REQ_RESP_SIZE,LG_FC_INIT_REQ,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_PROC_CODE,			0,0}, /* Code traitement  */
		{LG_REASON_CODE,		0,0}, /* Code motif  */
		{LG_SPEC_WELCOME,		0,0}, /* Accueil personnalisé  */
		{LG_SERVER_MENU,		0,0}, /* Menu serveur  */
		{LG_NB_AVAIL_COM_WAG,	0,0}, /* Nombre de wagons commerciaux disponibles  */
		{LG_NB_AVAIL_PRD_WAG,	0,0}, /* Nombre de wagons produits disponibles  */
		{LG_WTHDRAWAL_FLAG,		0,0}, /* Droit au retrait  */
		{LG_SPEC_FAST_CASH,		0,0}, /* Retrait rapide personnalisé  */
		{LG_B0_BLOCK_CHIP,		0,0}, /* Demande de blocage puce B0’  */
		{LG_EMV_WAG_IND,		0,0}, /* Indicateur wagon EMV  */
		{LG_SPEC_FILLER,		0,13}, /* SPEC_FILLER */
		{LG_MAC_TYPE,			0,0}, /* Type de scellement exécuté  */
		{LG_MAC_LEN,			0,0}, /* Longueur du sceau  */
		{LG_MAC,				0,0}, /* Sceau du message  */
		{LG_ADD_DATA,			0,0}, /* Un ou des Wagons produits + Un Wagon EMV */

	}

};

#define DEF_LGM_MIDDLE_REQ_NB_FIELDS	8 + 1
#define DEF_LGM_MIDDLE_REQ_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_MIDDLE_REQ = {
	LGM_MIDDLE_REQ,DEF_LGM_MIDDLE_REQ_NB_FIELDS,DEF_LGM_MIDDLE_REQ_SIZE,LG_FC_MIDDLE_REQ,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_CARD_ID,			0,0}, /* Identifiant carte  */
		{LG_REQ_WAG_TYPE,		0,0}, /* Type de wagons souhaités  */
		{LG_SPEC_FILLER,		0,1}, /* SPEC_FILLER */
		{LG_NB_PRD_WAG_RECV,	0,0}, /* Nombre de wagons produits déjà reçus  */
		{LG_NB_COM_WAG_RECV,	0,0}, /* Nombre de wagons commerciaux déjà reçus  */
		{LG_NB_AVAIL_COM_WAG,	0,0}, /* Nombre de wagons commerciaux disponibles  */
		{LG_NB_AVAIL_PRD_WAG,	0,0}, /* Nombre de wagons produits disponibles  */
		{LG_SPEC_FILLER,		0,39}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_MIDDLE_REQ_RESP_NB_FIELDS		0
#define DEF_LGM_MIDDLE_REQ_RESP_SIZE			0

TSLGMsgLayout	DEF_LGM_MIDDLE_REQ_RESP = {
	LGM_MIDDLE_REQ_RESP,DEF_LGM_MIDDLE_REQ_RESP_NB_FIELDS,DEF_LGM_MIDDLE_REQ_RESP_SIZE,LG_FC_MIDDLE_REQ/*,
	{
		
	}*/

};

#define DEF_LGM_WAG_PROD_1_NB_FIELDS	18
#define DEF_LGM_WAG_PROD_1_SIZE			100

TSLGMsgLayout	DEF_LGM_WAG_PROD_1 = {
	LGM_WAG_PROD_1,DEF_LGM_WAG_PROD_1_NB_FIELDS,DEF_LGM_WAG_PROD_1_SIZE,LG_FC_WAG_PROD,
	{
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_NB_AVAIL_PRD_WAG,		0,0}, /* Nombre de wagons produits disponibles  */
		{LG_NB_AVAIL_COM_WAG,		0,0}, /* Nombre de wagons commerciaux disponibles  */
		{LG_LAST_PRD_WAG_IND,		0,0}, /* Indicateur dernier wagon produit  */
		{LG_PRODUCT_NO,				0,0}, /* Numéro du produit  */
		{LG_PRODUCT_LABEL_CODE,		0,0}, /* Code libellé produit  */
		{LG_DC_CR_CODE,				0,0}, /* Code débit crédit  */
		{LG_BALANCE_1,				0,0}, /* Solde 1 signé  */
		{LG_PRODUCT_CURRENCY,		0,0}, /* Code devise du produit  */
		{LG_AUTH_PRD_OP,			0,0}, /* Opérations autorisées pour le produit  */
		{LG_LAST_PRD_WAG_IND,		0,0}, /* Indicateur dernier wagon produit  */
		{LG_PRODUCT_NO,				0,0}, /* Numéro du produit  */
		{LG_PRODUCT_LABEL_CODE,		0,0}, /* Code libellé produit  */
		{LG_DC_CR_CODE,				0,0}, /* Code débit crédit  */
		{LG_BALANCE_1,				0,0}, /* Solde 1 signé  */
		{LG_PRODUCT_CURRENCY,		0,0}, /* Code devise du produit  */
		{LG_AUTH_PRD_OP,			0,0}, /* Opérations autorisées pour le produit  */

	}

};

#define DEF_LGM_WAG_PROD_2_NB_FIELDS		24
#define DEF_LGM_WAG_PROD_2_SIZE				166

TSLGMsgLayout	DEF_LGM_WAG_PROD_2 = {
	LGM_WAG_PROD_2,DEF_LGM_WAG_PROD_2_NB_FIELDS,DEF_LGM_WAG_PROD_2_SIZE,LG_FC_PRD_WAG_2,
	{
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_NB_AVAIL_PRD_WAG,		0,0}, /* Nombre de wagons produits disponibles  */
		{LG_NB_AVAIL_COM_WAG,		0,0}, /* Nombre de wagons commerciaux disponibles  */
		{LG_LAST_PRD_WAG_IND,		0,0}, /* Indicateur dernier wagon produit  */
		{LG_PRODUCT_NO,				0,0}, /* Numéro du produit  */
		{LG_PRODUCT_LABEL_CODE,		0,0}, /* Code libellé produit  */
		{LG_DC_CR_CODE,				0,0}, /* Code débit crédit  */
		{LG_BALANCE_1,				0,0}, /* Solde 1 signé  */
		{LG_BALANCE_2,				0,0}, /* Solde 2 signé  */
		{LG_PRODUCT_CURRENCY,		0,0}, /* Code devise du produit  */
		{LG_AUTH_PRD_OP,			0,0}, /* Opérations autorisées pour le produit  */
		{LG_DEPOSIT_FLOOR_LIMIT,	0,0}, /* Montant plancher déposable  */
		{LG_DEPOSIT_CEIL_LIMIT,		0,0}, /* Montant plafond déposable  */
		{LG_LAST_PRD_WAG_IND,		0,0}, /* Indicateur dernier wagon produit  */
		{LG_PRODUCT_NO,				0,0}, /* Numéro du produit  */
		{LG_PRODUCT_LABEL_CODE,		0,0}, /* Code libellé produit  */
		{LG_DC_CR_CODE,				0,0}, /* Code débit crédit  */
		{LG_BALANCE_1,				0,0}, /* Solde 1 signé  */
		{LG_BALANCE_2,				0,0}, /* Solde 2 signé  */
		{LG_PRODUCT_CURRENCY,		0,0}, /* Code devise du produit  */
		{LG_AUTH_PRD_OP,			0,0}, /* Opérations autorisées pour le produit  */
		{LG_DEPOSIT_FLOOR_LIMIT,	0,0}, /* Montant plancher déposable  */
		{LG_DEPOSIT_CEIL_LIMIT,		0,0}, /* Montant plafond déposable  */


	}

};

#define DEF_LGM_WAG_COM_NB_FIELDS		8
#define DEF_LGM_WAG_COM_SIZE			64

TSLGMsgLayout	DEF_LGM_WAG_COM = {
	LGM_WAG_COM,DEF_LGM_WAG_COM_NB_FIELDS,DEF_LGM_WAG_COM_SIZE,LG_FC_WAG_COM,
	{
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_NB_AVAIL_PRD_WAG,	0,0}, /* Nombre de wagons produits disponibles  */
		{LG_NB_AVAIL_COM_WAG,	0,0}, /* Nombre de wagons commerciaux disponibles  */
		{LG_LAST_COM_WAG_IND,	0,0}, /* Indicateur dernier wagon commercial  */
		{LG_VAL_CODE,			0,0}, /* Code validation  */
		{LG_COM_WAG_LABEL,		0,0}, /* Libellé du wagon commercial  */
		{LG_SPEC_FILLER,		0,18}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_WAG_EMV_NB_FIELDS		4
#define DEF_LGM_WAG_EMV_SIZE			6

TSLGMsgLayout	DEF_LGM_WAG_EMV = {
	LGM_WAG_EMV,DEF_LGM_WAG_EMV_NB_FIELDS,DEF_LGM_WAG_EMV_SIZE,LG_FC_WAG_EMV,
	{
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_WAG_TYPE_CODE,		0,0}, /* Code type de Wagon  */
		{LG_DATA_FIELD_LEN,		0,0}, /* Longueur du champ de données  */
		{LG_ADD_DATA,			0,0}, /* Champ de données de structure TLV « Binaire »*/
	}

};

#define DEF_LGM_INQ_REQ_NB_FIELDS		11 + 1
#define DEF_LGM_INQ_REQ_SIZE			112 - MSG_HDR_OFST


TSLGMsgLayout	DEF_LGM_INQ_REQ = {
	LGM_INQ_REQ,DEF_LGM_INQ_REQ_NB_FIELDS,DEF_LGM_INQ_REQ_SIZE,LG_FC_INQ_REQ,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_CARD_ID,				0,0}, /* Identifiant carte  */
		{LG_INQUIRY_TYPE,			0,0}, /* Type d'interrogation  */
		{LG_NB_RESP_BLOCK_RECV,		0,0}, /* Nombre de blocs réponse déjà reçus  */
		{LG_KEY_REPLIED,			0,0}, /* Clé retournée (interrogation)  */
		{LG_ACCOUNT_1,				0,0}, /* Compte 1  */
		{LG_CHOICE_1,				0,0}, /* Choix 1  */
		{LG_CHOICE_2,				0,0}, /* Choix 2  */
		{LG_CHOICE_3,				0,0}, /* Choix 3  */
		{LG_CHOICE_4,				0,0}, /* Choix 4  */
		{LG_RECV_MSG_MAX_SIZE,		0,0}, /* Taille maximum des messages en réception  */
		{LG_SPEC_FILLER,			0,10}, /* SPEC_FILLER */

	}

};


#define DEF_LGM_INQ_RESP_NB_FIELDS		4 + 1
#define DEF_LGM_INQ_RESP_SIZE			62 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_INQ_RESP = {
	LGM_INQ_RESP,DEF_LGM_INQ_RESP_NB_FIELDS,DEF_LGM_INQ_RESP_SIZE,LG_FC_INQ_REQ,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_TO_REPLY_KEY_INQ,	0,0}, /* Clé à retourner (interrogation)  */
		{LG_ADD_DATA,			0,0}, /* Additional data */


	}

};

#define DEF_LGM_INQ_RESP_BLK_NB_FIELDS		4
#define DEF_LGM_INQ_RESP_BLK_SIZE			/*6*/ 5

TSLGMsgLayout	DEF_LGM_INQ_RESP_BLK = {
	LGM_INQ_RESP_BLK,DEF_LGM_INQ_RESP_BLK_NB_FIELDS,DEF_LGM_INQ_RESP_BLK_SIZE,LG_FC_INQ_REQ,
	{
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_LAST_INQ_BLOCK_IND,		0,0}, /* Indicateur dernier bloc interrogation  */
		{LG_INQ_LABEL_LEN,			0,0}, /* Longueur du libellé interrogation  */
		{LG_INQUIRY_LABEL,			0,0}, /* Libellé interrogation  */

	}

};

#define DEF_LGM_ILS_REQ_NB_FIELDS		18 + 1
#define DEF_LGM_ILS_REQ_SIZE			128 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_ILS_REQ = {
	LGM_ILS_REQ,DEF_LGM_ILS_REQ_NB_FIELDS,DEF_LGM_ILS_REQ_SIZE,LG_FC_ILS_REQ,
	{
		{LG_MSG_HEADER,					0,0}, /* Header */
		{LG_NB_RESP_BLOCK_RECV,			0,0}, /* Nombre de blocs réponse déjà reçus  */
		{LG_REPLIED_KEY,				0,0}, /* Clé retournée  */
		{LG_OP_CODE,					0,0}, /* Code opération bancaire  */
		{LG_CARD_ID,					0,0}, /* Identifiant carte  */
		{LG_ACCOUNT_1,					0,0}, /* Compte 1  */
		{LG_CHOICE_1,					0,0}, /* Choix 1  */
		{LG_CHOICE_2,					0,0}, /* Choix 2  */
		{LG_CHOICE_3,					0,0}, /* Choix 3  */
		{LG_CHOICE_4,					0,0}, /* Choix 4  */
		{LG_RECV_MSG_MAX_SIZE,			0,0}, /* Taille maximum des messages en réception  */
		{LG_PRINTER_ID,					0,0}, /* Identification de l’imprimante large ou laser  */
		{LG_NEXT_LINE_NO_TOPRINT,		0,0}, /* Numéro de la prochaine ligne à imprimer  */
		{LG_CURR_PRINT_PAGE,			0,0}, /* Numéro de la page en cours d’impression  */
		{LG_MAX_LINE_PER_PAGE,			0,0}, /* Nombre de lignes maximum par page  */
		{LG_PAPER_FORMAT,				0,0}, /* Format du papier  */
		{LG_FIRST_HEADER_NB_LINES,		0,0}, /* Nombre de lignes constituant la première entête  */
		{LG_HEADERS_NB_LINES,			0,0}, /* Nombre de lignes constituant les entêtes suivantes  */
		{LG_SPEC_FILLER,				0,5}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_ILS_RESP_NB_FIELDS		4 + 1
#define DEF_LGM_ILS_RESP_SIZE			65 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_ILS_RESP = {
	LGM_ILS_RESP,DEF_LGM_ILS_RESP_NB_FIELDS,DEF_LGM_ILS_RESP_SIZE,LG_FC_ILS_REQ,
	{
		{LG_MSG_HEADER,		0,0}, /* Header */
		{LG_SEPERATOR,		0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,	0,0}, /* Code fonction  */
		{LG_TO_REPLY_KEY,	0,0}, /* Clé à retourner  */
		{LG_MAX_COL_NBR,	0,0}, /* Nombre de colonnes maximum par ligne  */

	}

};

#define DEF_LGM_ILS_RESP_BLK_NB_FIELDS		4
#define DEF_LGM_ILS_RESP_BLK_SIZE			8

TSLGMsgLayout	DEF_LGM_ILS_RESP_BLK = {
	LGM_ILS_RESP_BLK,DEF_LGM_ILS_RESP_BLK_NB_FIELDS,DEF_LGM_ILS_RESP_BLK_SIZE,LG_FC_ILS_REQ,
	{
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_LAST_ILS_BLOCK_IND,		0,0}, /* Indicateur dernier bloc ILS  */
		{LG_NB_LINES,				0,0}, /* Numéro de ligne  */
		{LG_ILS_DATA_LEN,			0,0}, /* Longueur des données ILS  */
	}

};



#define DEF_LGM_PRINTER_PG_TRNSF_REQ_NB_FIELDS		19 + 1
#define DEF_LGM_PRINTER_PG_TRNSF_REQ_SIZE			128 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_PRINTER_PG_TRNSF_REQ = {
	LGM_PRINTER_PG_TRNSF_REQ,DEF_LGM_PRINTER_PG_TRNSF_REQ_NB_FIELDS,DEF_LGM_PRINTER_PG_TRNSF_REQ_SIZE,LG_FC_PRINTER_PG_TRNSF_REQ,
	{
		{LG_MSG_HEADER,					0,0}, /* Header */
		{LG_NB_RESP_BLOCK_RECV,			0,0}, /* Nombre de blocs réponse déjà reçus  */
		{LG_REPLIED_KEY,				0,0}, /* Clé retournée  */
		{LG_OP_CODE,					0,0}, /* Code opération bancaire  */
		{LG_CARD_ID,					0,0}, /* Identifiant carte  */
		{LG_ACCOUNT_1,					0,0}, /* Compte 1  */
		{LG_CHOICE_1,					0,0}, /* Choix 1  */
		{LG_CHOICE_2,					0,0}, /* Choix 2  */
		{LG_CHOICE_3,					0,0}, /* Choix 3  */
		{LG_CHOICE_4,					0,0}, /* Choix 4  */
		{LG_RECV_MSG_MAX_SIZE,			0,0}, /* Taille maximum des messages en réception  */
		{LG_PRINTER_ID,					0,0}, /* Identification de l’imprimante large ou laser  */
		{LG_TRNSF_PRINT_PAGE_NO,		0,0}, /* Numéro de la page à imprimer en cours de transfert  */
		{LG_LAST_DISP_SCREEN_PAGE_NO,	0,0}, /* Numéro de la dernière page écran visualisée  */
		{LG_MAX_LINE_PER_PAGE,			0,0}, /* Nombre de lignes maximum par page  */
		{LG_PAPER_FORMAT,				0,0}, /* Format du papier  */
		{LG_FIRST_HEADER_NB_LINES,		0,0}, /* Nombre de lignes constituant la première entête  */
		{LG_HEADERS_NB_LINES,			0,0}, /* Nombre de lignes constituant les entêtes suivantes  */
		{LG_INQUIRY_TYPE,				0,0}, /* Type d'interrogation  */
		{LG_SPEC_FILLER,				0,5}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_PRINTER_PG_TRNSF_RESP_NB_FIELDS		9 + 1
#define DEF_LGM_PRINTER_PG_TRNSF_RESP_SIZE			71 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_PRINTER_PG_TRNSF_RESP = {
	LGM_PRINTER_PG_TRNSF_RESP,DEF_LGM_PRINTER_PG_TRNSF_RESP_NB_FIELDS,DEF_LGM_PRINTER_PG_TRNSF_RESP_SIZE,LG_FC_PRINTER_PG_TRNSF_REQ,
	{
		{LG_MSG_HEADER,					0,0}, /* Header */
		{LG_SEPERATOR,					0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,				0,0}, /* Code fonction  */
		{LG_TO_REPLY_KEY,				0,0}, /* Clé à retourner  */
		{LG_TRNSF_PRINT_PAGE_NO,		0,0}, /* Numéro de la page à imprimer en cours de transfert  */
		{LG_LAST_BLOCK_PRINT_PAGE_IND,	0,0}, /* Indicateur dernier bloc de la page à imprimer  */
		{LG_EDIT_FORMAT,				0,0}, /* Format de l’édition  */
		{LG_EDIT_WITH_GRAPHIC,			0,0}, /* Edition avec fond graphique  */
		{LG_DATA_BLOCK_LEN,				0,0}, /* Longueur du bloc de données  */
		{LG_DATA_BLOCK,					0,0}, /* Bloc de données  */

	}

};

#define DEF_LGM_SCREEN_PG_TRNSF_REQ_NB_FIELDS		12 + 1
#define DEF_LGM_SCREEN_PG_TRNSF_REQ_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_SCREEN_PG_TRNSF_REQ = {
	LGM_SCREEN_PG_TRNSF_REQ,DEF_LGM_SCREEN_PG_TRNSF_REQ_NB_FIELDS,DEF_LGM_SCREEN_PG_TRNSF_REQ_SIZE,LG_FC_SCREEN_PG_TRNSF_REQ,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_CARD_ID,				0,0}, /* Identifiant carte  */
		{LG_INQUIRY_TYPE,			0,0}, /* Type d'interrogation  */
		{LG_NB_RESP_BLOCK_RECV,		0,0}, /* Nombre de blocs réponse déjà reçus  */
		{LG_KEY_REPLIED,			0,0}, /* Clé retournée (interrogation)  */
		{LG_ACCOUNT_1,				0,0}, /* Compte 1  */
		{LG_CHOICE_1,				0,0}, /* Choix 1  */
		{LG_CHOICE_2,				0,0}, /* Choix 2  */
		{LG_CHOICE_3,				0,0}, /* Choix 3  */
		{LG_CHOICE_4,				0,0}, /* Choix 4  */
		{LG_RECV_MSG_MAX_SIZE,		0,0}, /* Taille maximum des messages en réception  */
		{LG_TRSNF_SCREEN_PAGE_NO,	0,0}, /* Numéro de la page écran en cours de transfert  */
		{LG_SPEC_FILLER,			0,8}, /* SPEC_FILLER */

	}

};


#define DEF_LGM_SCREEN_PG_TRNSF_RESP_NB_FIELDS		6 + 1
#define DEF_LGM_SCREEN_PG_TRNSF_RESP_SIZE			69 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_SCREEN_PG_TRNSF_RESP = {
	LGM_SCREEN_PG_TRNSF_RESP,DEF_LGM_SCREEN_PG_TRNSF_RESP_NB_FIELDS,DEF_LGM_SCREEN_PG_TRNSF_RESP_SIZE,LG_FC_SCREEN_PG_TRNSF_REQ,
	{
		{LG_MSG_HEADER,					0,0}, /* Header */
		{LG_SEPERATOR,					0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,				0,0}, /* Code fonction  */
		{LG_TO_REPLY_KEY_INQ,			0,0}, /* Clé à retourner (interrogation)  */
		{LG_TRSNF_SCREEN_PAGE_NO,		0,0}, /* Numéro de la page écran en cours de transfert  */
		{LG_TRSNF_SCREEN_LAST_BLOCK_IND,0,0}, /* Indicateur dernier bloc de la page écran  */
		{LG_DATA_BLOCK_LEN,				0,0}, /* Longueur du bloc de données  */
		{LG_DATA_BLOCK,					0,0}, /* Bloc de données  */

	}

};

#define DEF_LGM_SALES_COND_REQ_NB_FIELDS		6 + 1
#define DEF_LGM_SALES_COND_REQ_SIZE				128 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_SALES_COND_REQ = {
	LGM_SALES_COND_REQ,DEF_LGM_SALES_COND_REQ_NB_FIELDS,DEF_LGM_SALES_COND_REQ_SIZE,LG_FC_SALES_COND,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_OP_CODE,			0,0}, /* Code opération bancaire  */
		{LG_CURR_OP_CURRENCY,	0,0}, /* Code devise de l'opération en cours  */
		{LG_SPEC_FILLER,		0,2}, /* SPEC_FILLER */
		{LG_ISO2_CONTENT,		0,0}, /* Contenu ISO2  */
		{LG_PRINCIPAL_CRNCY,	0,0}, /* Code devise de la devise principale  */
		{LG_SPEC_FILLER,		0,33}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_SALES_COND_RESP_NB_FIELDS		13 + 1
#define DEF_LGM_SALES_COND_RESP_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_SALES_COND_RESP = {
	LGM_SALES_COND_RESP,DEF_LGM_SALES_COND_RESP_NB_FIELDS,DEF_LGM_SALES_COND_RESP_SIZE,LG_FC_SALES_COND,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_CONV_RATE,				0,0}, /* Taux de conversion ou Cours suivant la quotité  */
		{LG_DISC_FEE_AMOUNT,		0,0}, /* Montant de la commission fixe ou frais  */
		{LG_DISC_RATE,				0,0}, /* Taux de commission  */
		{LG_DISC_FEE_FLOOR_LIMIT,	0,0}, /* Montant plancher de commission ou de frais  */
		{LG_DISC_FEE_CEIL_LIMIT,	0,0}, /* Montant plafond de commission ou de frais  */
		{LG_TVA_RATE,				0,0}, /* Taux de TVA  */
		{LG_TAX_CODE,				0,0}, /* Code taxe  */
		{LG_QUOTA,					0,0}, /* Quotité  */
		{LG_SALE_COND_RESP_CD,		0,0}, /* Code réponse conditions de vente  */
		{LG_SALE_TYPE,				0,0}, /* Type de vente  */
		{LG_SPEC_FILLER,			0,14}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_MULTI_SALES_COND_REQ_NB_FIELDS		6 + 1
#define DEF_LGM_MULTI_SALES_COND_REQ_SIZE			128 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_MULTI_SALES_COND_REQ = {
	LGM_MULTI_SALES_COND_REQ,DEF_LGM_MULTI_SALES_COND_REQ_NB_FIELDS,DEF_LGM_MULTI_SALES_COND_REQ_SIZE,LG_FC_MULTI_SALES_COND_INQ,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_OP_CODE,			0,0}, /* Code opération bancaire  */
		{LG_CURR_OP_CURRENCY,	0,0}, /* Code devise de l'opération en cours  */
		{LG_SPEC_FILLER,		0,2}, /* SPEC_FILLER */
		{LG_ISO2_CONTENT,		0,0}, /* Contenu ISO2  */
		{LG_PRINCIPAL_CRNCY,	0,0}, /* Code devise de la devise principale  */
		{LG_SPEC_FILLER,		0,33}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_MULTI_SALES_COND_RESP_NB_FIELDS		25 + 1
#define DEF_LGM_MULTI_SALES_COND_RESP_SIZE			/*112*/200 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_MULTI_SALES_COND_RESP = {
	LGM_MULTI_SALES_COND_RESP,DEF_LGM_MULTI_SALES_COND_RESP_NB_FIELDS,DEF_LGM_MULTI_SALES_COND_RESP_SIZE,LG_FC_MULTI_SALES_COND_INQ,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_MIDDLE_AMOUNT,			0,0}, /* Montant intermédiaire  */
		{LG_TOP_AMOUNT,				0,0}, /* Montant supérieur  */
		{LG_CONV_RATE,				0,0}, /* Taux de conversion ou Cours suivant la quotité  */
		{LG_DISC_FEE_AMOUNT,		0,0}, /* Montant de la commission fixe ou frais  */
		{LG_DISC_RATE,				0,0}, /* Taux de commission  */
		{LG_DISC_FEE_FLOOR_LIMIT,	0,0}, /* Montant plancher de commission ou de frais  */
		{LG_DISC_FEE_CEIL_LIMIT,	0,0}, /* Montant plafond de commission ou de frais  */
		{LG_CONV_RATE,				0,0}, /* Taux de conversion ou Cours suivant la quotité  */
		{LG_DISC_FEE_AMOUNT,		0,0}, /* Montant de la commission fixe ou frais  */
		{LG_DISC_RATE,				0,0}, /* Taux de commission  */
		{LG_DISC_FEE_FLOOR_LIMIT,	0,0}, /* Montant plancher de commission ou de frais  */
		{LG_DISC_FEE_CEIL_LIMIT,	0,0}, /* Montant plafond de commission ou de frais  */
		{LG_CONV_RATE,				0,0}, /* Taux de conversion ou Cours suivant la quotité  */
		{LG_DISC_FEE_AMOUNT,		0,0}, /* Montant de la commission fixe ou frais  */
		{LG_DISC_RATE,				0,0}, /* Taux de commission  */
		{LG_DISC_FEE_FLOOR_LIMIT,	0,0}, /* Montant plancher de commission ou de frais  */
		{LG_DISC_FEE_CEIL_LIMIT,	0,0}, /* Montant plafond de commission ou de frais  */
		{LG_TVA_RATE,				0,0}, /* Taux de TVA  */
		{LG_TAX_CODE,				0,0}, /* Code taxe  */
		{LG_QUOTA,					0,0}, /* Quotité  */
		{LG_SALE_COND_RESP_CD,		0,0}, /* Code réponse conditions de vente  */
		{LG_SALE_TYPE,				0,0}, /* Type de vente  */
		{LG_SPEC_FILLER,			0,14}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_AUTH_REQ_NB_FIELDS		27 + 1 + 1
#define DEF_LGM_AUTH_REQ_SIZE			200 /*- MSG_HDR_OFST*/

/*todo: unkwnon size*/
TSLGMsgLayout	DEF_LGM_AUTH_REQ = {
	LGM_AUTH_REQ,DEF_LGM_AUTH_REQ_NB_FIELDS,DEF_LGM_AUTH_REQ_SIZE,LG_FC_AUTH_REQ,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_OP_PROC_MODE,		0,0}, /* Mode de traitement de l’opération  */
		{LG_SPEC_FILLER,		0,1}, /* SPEC_FILLER */
		{LG_PIN_ENTRY_CODE,		0,0}, /* Code saisie du code confidentiel  */
		{LG_OP_CODE,			0,0}, /* Code opération bancaire  */
		{LG_REQUESTED_AMOUNT,	0,0}, /* Montant demandé  */
		{LG_PRINCIPAL_CRNCY,	0,0}, /* Code devise de la devise principale  */
		{LG_PIN_LEN,			0,0}, /* Longueur du code confidentiel  */
		{LG_PIN_BLOCK,			0,0}, /* Code chiffré  */
		{LG_ISO2_CONTENT,		0,0}, /* Contenu ISO2  */
		{LG_BANK_CODE,			0,0}, /* Code banque  */
		{LG_B0_PROC_CTX,		0,0}, /* Contexte traitement puce B0’  */
		{LG_B0_AUTH_ITEM,		0,0}, /* Elément d'authentification B0’  */
		{LG_BO_CALC_DATA,		0,0}, /* Données du calcul B0’  */
		{LG_B0_FINAL_STATE_W,	0,0}, /* Mot d'état final B0’  */
		{LG_BO_PROC_RES,		0,0}, /* Résultat traitement microcircuit B0’  */
		{LG_ACCOUNT_TO_DEBIT,	0,0}, /* Numéro du compte à débiter  */
		{LG_EMV_PROC_RESULT,	0,0}, /* Résultat du traitement du microcircuit EMV  */
		{LG_EMV_WAG_IND,		0,0}, /* Indicateur wagon EMV  */
		{LG_CHOICE_1,			0,0}, /* Choix 1  */
		{LG_CHOICE_2,			0,0}, /* Choix 2  */
		{LG_CHOICE_3,			0,0}, /* Choix 3  */
		{LG_CHOICE_4,			0,0}, /* Choix 4  */
		{LG_SCD_USAGE_MODE_TERM,0,0}, /* Modes d’utilisations SCD  */
		{LG_SPEC_FILLER,		0,14}, /* SPEC_FILLER */
		{LG_MAC_TYPE,			0,0}, /* Type de scellement exécuté  */
		{LG_MAC_LEN,			0,0}, /* Longueur du sceau  */
		{LG_MAC,				0,0}, /* Sceau du message  */
		{LG_ADD_DATA,			0,0}, /* Add Data  */
		
	}

};

#define DEF_LGM_AUTH_RESP_NB_FIELDS		18 + 1
#define DEF_LGM_AUTH_RESP_SIZE			158 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_AUTH_RESP = {
	LGM_AUTH_RESP,DEF_LGM_AUTH_RESP_NB_FIELDS,DEF_LGM_AUTH_RESP_SIZE,LG_FC_AUTH_REQ,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_RESPONSE_CODE,			0,0}, /* Code réponse autorisation  */
		{LG_REQUESTED_AMOUNT,		0,0}, /* Montant demandé  */
		{LG_AUTHORIZED_AMOUNT,		0,0}, /* Montant autorisé (en devise principale)  */
		{LG_AUTH_CODE,				0,0}, /* Numéro d'autorisation  */
		{LG_TRX_ADDRESS_1,			0,0}, /* Lieu de la transaction (adresse, ville)  */
		{LG_TRX_ADDRESS_2,			0,0}, /* Lieu de la transaction (pays, code postal)  */
		{LG_B0_BLOCK_CHIP,			0,0}, /* Demande de blocage puce B0’  */
		{LG_EMV_WAG_IND,			0,0}, /* Indicateur wagon EMV  */
		{LG_BAL_ACCOUNT_TO_DEBIT,	0,0}, /* Solde signé du compte à débiter (demande de solde)  */
		{LG_WITHDRAWAL_REPORT_SEND_COND,0,0}, /* Condition d’émission du compte rendu de retrait  */
		{LG_COM_TICKETS_TO_DIST,	0,0}, /* Nombre de coupons commerciaux à distribuer  */
		{LG_SPEC_FILLER,			0,4}, /* SPEC_FILLER */
		{LG_MAC_TYPE,				0,0}, /* Type de scellement exécuté  */
		{LG_MAC_LEN,				0,0}, /* Longueur du sceau  */
		{LG_MAC,					0,0}, /* Sceau du message  */
		{LG_ADD_DATA,				0,0}, /* Wagon EMV  */
	}

};
#define DEF_LGM_AUTH_REQ_F_CRNCY_NB_FIELDS			24 + 1
#define DEF_LGM_AUTH_REQ_F_CRNCY_SIZE				234 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_AUTH_REQ_F_CRNCY = {
	LGM_AUTH_REQ_F_CRNCY,DEF_LGM_AUTH_REQ_F_CRNCY_NB_FIELDS,DEF_LGM_AUTH_REQ_F_CRNCY_SIZE,LG_FC_AUTH_REQ_F_CRNCY,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_OP_PROC_MODE,		0,0}, /* Mode de traitement de l’opération  */
		{LG_SPEC_FILLER,		0,1}, /* SPEC_FILLER */
		{LG_PIN_ENTRY_CODE,		0,0}, /* Code saisie du code confidentiel  */
		{LG_OP_CODE,			0,0}, /* Code opération bancaire  */
		{LG_REQUESTED_AMOUNT,	0,0}, /* Montant demandé  */
		{LG_CURR_OP_CURRENCY,	0,0}, /* Code devise de l'opération en cours  */
		{LG_PIN_LEN,			0,0}, /* Longueur du code confidentiel  */
		{LG_PIN_BLOCK,			0,0}, /* Code chiffré  */
		{LG_ISO2_CONTENT,		0,0}, /* Contenu ISO2  */
		{LG_BANK_CODE,			0,0}, /* Code banque  */
		{LG_REQUESTED_AMNT_CRNCY,0,0}, /* Montant demandé en devise  */
		{LG_B0_PROC_CTX,		0,0}, /* Contexte traitement puce B0’  */
		{LG_B0_AUTH_ITEM,		0,0}, /* Elément d'authentification B0’  */
		{LG_BO_CALC_DATA,		0,0}, /* Données du calcul B0’  */
		{LG_B0_FINAL_STATE_W,	0,0}, /* Mot d'état final B0’  */
		{LG_BO_PROC_RES,		0,0}, /* Résultat traitement microcircuit B0’  */
		{LG_PRINCIPAL_CRNCY,	0,0}, /* Code devise de la devise principale  */
		{LG_EMV_PROC_RESULT,	0,0}, /* Résultat du traitement du microcircuit EMV  */
		{LG_EMV_WAG_IND,		0,0}, /* Indicateur wagon EMV  */
		{LG_SCD_USAGE_MODE_TERM,0,0}, /* Modes d’utilisations SCD  */
		{LG_SPEC_FILLER,		0,48}, /* SPEC_FILLER */
		{LG_MAC_TYPE,			0,0}, /* Type de scellement exécuté  */
		{LG_MAC_LEN,			0,0}, /* Longueur du sceau  */
		{LG_MAC,				0,0}, /* Sceau du message  */

	}

};


#define DEF_LGM_AUTH_RESP_F_CRNCY_NB_FIELDS			14 + 1
#define DEF_LGM_AUTH_RESP_F_CRNCY_SIZE				158 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_AUTH_RESP_F_CRNCY = {
	LGM_AUTH_RESP_F_CRNCY,DEF_LGM_AUTH_RESP_F_CRNCY_NB_FIELDS,DEF_LGM_AUTH_RESP_F_CRNCY_SIZE,LG_FC_AUTH_REQ_F_CRNCY,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_RESPONSE_CODE,		0,0}, /* Code réponse autorisation  */
		{LG_REQUESTED_AMOUNT,	0,0}, /* Montant demandé  */
		{LG_AUTHORIZED_AMOUNT,	0,0}, /* Montant autorisé (en devise principale)  */
		{LG_AUTH_CODE,			0,0}, /* Numéro d'autorisation  */
		{LG_TRX_ADDRESS_1,		0,0}, /* Lieu de la transaction (adresse, ville)  */
		{LG_TRX_ADDRESS_2,		0,0}, /* Lieu de la transaction (pays, code postal)  */
		{LG_B0_BLOCK_CHIP,		0,0}, /* Demande de blocage puce B0’  */
		{LG_EMV_WAG_IND,		0,0}, /* Indicateur wagon EMV  */
		{LG_SPEC_FILLER,		0,19}, /* SPEC_FILLER */
		{LG_MAC_TYPE,			0,0}, /* Type de scellement exécuté  */
		{LG_MAC_LEN,			0,0}, /* Longueur du sceau  */
		{LG_MAC,				0,0}, /* Sceau du message  */
	}

};

#define DEF_LGM_BAL_INQ_NB_FIELDS		13 + 1
#define DEF_LGM_BAL_INQ_SIZE			186 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_BAL_INQ = {
	LGM_BAL_INQ,DEF_LGM_BAL_INQ_NB_FIELDS,DEF_LGM_BAL_INQ_SIZE,LG_FC_BAL_INQ,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SPEC_FILLER,			0,3}, /* SPEC_FILLER */
		{LG_OP_CODE,				0,0}, /* Code opération bancaire  */
		{LG_REQUESTED_AMOUNT,		0,0}, /* Montant demandé  */
		{LG_SPEC_FILLER,			0,18}, /* SPEC_FILLER */
		{LG_ISO2_CONTENT,			0,0}, /* Contenu ISO2  */
		{LG_ACCOUNT_TO_DEBIT,		0,0}, /* Numéro du compte à débiter  */
		{LG_ACCOUNT_TO_CREDIT,		0,11}, /* Numéro du compte à créditer  */
		{LG_RIB_KEY,				0,0}, /* Clé RIB  */
		{LG_BRANCH_CODE,			0,0}, /* Code agence (ou code guichet)  */
		{LG_CR_ACC_BANK_CODE,		0,0}, /* Code banque du compte à créditer  */
		{LG_SPEC_FILLER,			0,15}, /* SPEC_FILLER */
		{LG_CURR_OP_CURRENCY,		0,0}, /* Code devise de l'opération en cours  */
		{LG_SPEC_FILLER,			0,14}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_BAL_INQ_RESP_NB_FIELDS		11 + 1
#define DEF_LGM_BAL_INQ_RESP_SIZE			158 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_BAL_INQ_RESP = {
	LGM_BAL_INQ_RESP,DEF_LGM_BAL_INQ_RESP_NB_FIELDS,DEF_LGM_BAL_INQ_RESP_SIZE,LG_FC_BAL_INQ,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_RESPONSE_CODE,			0,0}, /* Code réponse autorisation  */
		{LG_AUTHORIZED_AMOUNT,		0,0}, /* Montant autorisé (en devise principale)  */
		{LG_ACC_STATE_CODE,			0,0}, /* Code état du compte  */
		{LG_INFO_LABEL,				0,0}, /* Libellé d'information  */
		{LG_BAL_ACCOUNT_TO_DEBIT,	0,0}, /* Solde signé du compte à débiter (demande de solde)  */
		{LG_BAL_ACCOUNT_TO_CREDIT,	0,0}, /* Solde signé du compte à créditer (demande de solde)  */
		{LG_DEPOSIT_FLOOR_LIMIT,	0,0}, /* Montant plancher déposable  */
		{LG_DEPOSIT_CEIL_LIMIT,		0,0}, /* Montant plafond déposable  */
		{LG_SPEC_FILLER,			0,7}, /* SPEC_FILLER */

	}

};


#define DEF_LGM_MONEO_CTX_REQ_NB_FIELDS		3 + 1
#define DEF_LGM_MONEO_CTX_REQ_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_MONEO_CTX_REQ = {
	LGM_MONEO_CTX_REQ,DEF_LGM_MONEO_CTX_REQ_NB_FIELDS,DEF_LGM_MONEO_CTX_REQ_SIZE,LG_FC_MONEO_CTX,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_ISO2_CONTENT,		0,0}, /* Contenu ISO2  */
		{LG_CH_NO,				0,0}, /* Numéro de porteur issu de l’application  */
		{LG_SPEC_FILLER,		0,8}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_MONEO_CTX_REQ_RESP_NB_FIELDS		7 + 1
#define DEF_LGM_MONEO_CTX_REQ_RESP_SIZE				112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_MONEO_CTX_REQ_RESP = {
	LGM_MONEO_CTX_REQ_RESP,DEF_LGM_MONEO_CTX_REQ_RESP_NB_FIELDS,DEF_LGM_MONEO_CTX_REQ_RESP_SIZE,LG_FC_MONEO_CTX,
	{
		{LG_MSG_HEADER,			0,0}, /* Header */
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,		0,0}, /* Code fonction  */
		{LG_TSEQ,				0,0}, /* TSEQ (N° séquence reçu lors de la demande de contexte)  */
		{LG_TERMINAL_ID,		0,0}, /* Terminal_Id (N° terminal reçu lors de la demande de contexte)  */
		{LG_SAM,				0,0}, /* N° SAM  */
		{LG_SERVER_RESP_CODE,	0,0}, /* Code réponse GDG  */
		{LG_SPEC_FILLER,		0,27}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_TOPUP_REQ_NB_FIELDS		12 + 1
#define DEF_LGM_TOPUP_REQ_SIZE			200 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_TOPUP_REQ = {
	LGM_TOPUP_REQ,DEF_LGM_TOPUP_REQ_NB_FIELDS,DEF_LGM_TOPUP_REQ_SIZE,LG_FC_TOPUP_AUTH,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_CURR_OP_CURRENCY,		0,0}, /* Code devise de l'opération en cours  */
		{LG_DOWNLOAD_TYPE,			0,0}, /* Type de chargement  */
		{LG_TSEQ,					0,0}, /* TSEQ (N° séquence reçu lors de la demande de contexte)  */
		{LG_TERMINAL_ID,			0,0}, /* Terminal_Id (N° terminal reçu lors de la demande de contexte)  */
		{LG_RESPONSE_CODE,			0,0}, /* Code réponse autorisation  */
		{LG_AUTH_CODE,				0,0}, /* Numéro d'autorisation  */
		{LG_EMV_WAG_IND,			0,0}, /* Indicateur wagon EMV  */
		{LG_CH_NO,					0,0}, /* Numéro de porteur issu de l’application  */
		{LG_AMONT_ENTERED,			0,0}, /* Montant du chargement composé  */
		{LG_SPEC_FILLER,			0,11}, /* SPEC_FILLER */
		{LG_ADD_CARD_DATA_LEN,		0,0}, /* Longueur des données complémentaires issues de la carte  */
		{LG_ADD_CARD_DATA,			0,79}, /* Bloc des données complémentaires issues de la carte  */

	}

};

#define DEF_LGM_TOPUP_RESP_NB_FIELDS	10 + 1
#define DEF_LGM_TOPUP_RESP_SIZE			112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_TOPUP_RESP = {
	LGM_TOPUP_RESP,DEF_LGM_TOPUP_RESP_NB_FIELDS,DEF_LGM_TOPUP_RESP_SIZE,LG_FC_TOPUP_AUTH,
	{
		{LG_MSG_HEADER,						0,0}, /* Header */
		{LG_SEPERATOR,						0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,					0,0}, /* Code fonction  */
		{LG_PME_AUTH_NO,					0,0}, /* Numéro d’autorisation PME  */
		{LG_DOWNLOAD_TYPE,					0,0}, /* Type de chargement  */
		{LG_PME_EXPIRY_DATE_TOPUP_RESP,		0,0}, /* Date d’échéance du PME, en réponse à la demande de chargement  */
		{LG_PME_MNG_HOUR_TOPUP_RESP,		0,0}, /* Heure du gestionnaire service PME, en réponse à la demande de chargement  */
		{LG_PME_MNG_DATE_TOPUP_RESP,		0,0}, /* Date du gestionnaire service PME, en réponse à la demande de chargement  */
		{LG_MNG_RESP_CODE_TOPUP_RESP,		0,0}, /* Code réponse gestionnaire, en réponse à la demande de chargement  */
		{LG_MONERO_WAG_IND,					0,0}, /* Indicateur wagon Monéo  */
		{LG_SPEC_FILLER,					0,37}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_WAG_MONEO_NB_FIELDS		3
#define DEF_LGM_WAG_MONEO_SIZE			5

TSLGMsgLayout	DEF_LGM_WAG_MONEO = {
	LGM_WAG_MONEO,DEF_LGM_WAG_MONEO_NB_FIELDS,DEF_LGM_WAG_MONEO_SIZE,LG_FC_WAG_MONEO,
	{
		{LG_SEPERATOR,					0,0}, /* Séparateur  */
		{LG_WAG_TYPE_CODE,				0,0}, /* Code type de Wagon  */
		{LG_ADD_DATA_LEN_TOPUP_RESP,	0,0}, /* Longueur des données complémentaires en réponse à la demande de chargement */

	}

};

#define DEF_LGM_WITHDRAWAL_ADVICE_NB_FIELDS			41 + 1
#define DEF_LGM_WITHDRAWAL_ADVICE_SIZE				250 - MSG_HDR_OFST

/*todo: unkwnon size*/
TSLGMsgLayout	DEF_LGM_WITHDRAWAL_ADVICE = {
	LGM_WITHDRAWAL_ADVICE,DEF_LGM_WITHDRAWAL_ADVICE_NB_FIELDS,DEF_LGM_WITHDRAWAL_ADVICE_SIZE,' ',
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_OP_CD_REPORT_TYPE,		0,0}, /* Type de compte-rendu d'opération bancaire  */
		{LG_OP_CODE,				0,0}, /* Code opération bancaire  */
		{LG_STATE_1_IND,			0,0}, /* Indicateur ETAT1  */
		{LG_STATE_2_IND,			0,0}, /* Indicateur ETAT2  */
		{LG_STATE_3_IND,			0,0}, /* Indicateur ETAT3  */
		{LG_ISO2_CONTENT,			0,0}, /* Contenu ISO2  */
		{LG_ACCOUNT_TO_DEBIT,		0,0}, /* Numéro du compte à débiter  */
		{LG_CURR_OP_CURRENCY,		0,0}, /* Code devise de l'opération en cours  */
		{LG_AUTHORIZED_AMOUNT,		0,0}, /* Montant autorisé (en devise principale)  */
		{LG_AUTH_CODE,				0,0}, /* Numéro d'autorisation  */
		{LG_OP_PROC_MODE,			0,0}, /* Mode de traitement de l’opération  */
		{LG_SPEC_FILLER,			0,1}, /* SPEC_FILLER */
		{LG_DISTRIBUTED_AMOUNT,		0,0}, /* Montant distribué  */
		{LG_BANK_CODE,				0,0}, /* Code banque  */
		{LG_AMNT_OUT_TH_CRNY_1,		0,0}, /* Encaisse Out théorique dans la devise 1 (Devise principale)  */
		{LG_REVERSAL_NOTIF,			0,0}, /* Notification de redressement  */
		{LG_K7OUT_O1_NB_DIST,		0,0}, /* Nombre de medias distribués en cassette de type K7Out ordre 1  */
		{LG_K7OUT_O2_NB_DIST,		0,0}, /* Nombre de medias distribués en cassette de type K7Out ordre 2  */
		{LG_K7OUT_O3_NB_DIST,		0,0}, /* Nombre de medias distribués en cassette de type K7Out ordre 3  */
		{LG_K7OUT_O4_NB_DIST,		0,0}, /* Nombre de medias distribués en cassette de type K7Out ordre 4  */
		{LG_B0_BLOCK_CHIP_ATM_REQ,	0,0}, /* Blocage puce B0’ demandé au GAB  */
		{LG_K7OUT_O1_DEN,			0,0}, /* Dénomination cassette de type K7Out ordre 1  */
		{LG_K7OUT_O2_DEN,			0,0}, /* Dénomination cassette de type K7Out ordre 2  */
		{LG_K7OUT_O3_DEN,			0,0}, /* Dénomination cassette de type K7Out ordre 3  */
		{LG_K7OUT_O4_DEN,			0,0}, /* Dénomination cassette de type K7Out ordre 4  */
		{LG_AVAIL_BAL_CRNCY_1,		0,0}, /* Encaisse Out disponible dans la devise 1(devise principale)  */
		{LG_K7MIXT_O1_NB_DIST,		0,0}, /* Nombre de medias distribués en cassette de type K7Mixte ordre 1  */
		{LG_K7MIXT_O2_NB_DIST,		0,0}, /* Nombre de medias distribués en cassette de type K7Mixte ordre 2  */
		{LG_K7MIXT_O3_NB_DIST,		0,0}, /* Nombre de medias distribués en cassette de type K7Mixte ordre 3  */
		{LG_K7MIXT_O4_NB_DIST,		0,0}, /* Nombre de medias distribués en cassette de type K7Mixte ordre 4  */
		{LG_K7MIXT_O1_DEN,			0,0}, /* Dénomination cassette de type K7Mixte ordre 1  */
		{LG_K7MIXT_O2_DEN,			0,0}, /* Dénomination cassette de type K7Mixte ordre 2 */
		{LG_K7MIXT_O3_DEN,			0,0}, /* Dénomination cassette de type K7Mixte ordre 3  */
		{LG_K7MIXT_O4_DEN,			0,0}, /* Dénomination cassette de type K7Mixte ordre 4  */
		{LG_EMV_PROC_RESULT,		0,0}, /* Résultat du traitement du microcircuit EMV  */
		{LG_EMV_WAG_IND,			0,0}, /* Indicateur wagon EMV  */
		{LG_SCD_USAGE_MODE_TERM,	0,0}, /* Modes d’utilisations SCD  */
		{LG_COM_TICKETS_DIST,		0,0}, /* Nombre de coupons commerciaux distribués  */
		{LG_MAC_TYPE,				0,0}, /* Type de scellement exécuté  */
		{LG_MAC_LEN,				0,0}, /* Longueur du sceau  */
		{LG_MAC,					0,0}, /* Sceau du message  */

	}
};

#define DEF_LGM_DEGRADE_NB_FIELDS		12 + 1
#define DEF_LGM_DEGRADE_SIZE			250 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_DEGRADE = {
	LGM_DEGRADE,DEF_LGM_DEGRADE_NB_FIELDS,DEF_LGM_DEGRADE_SIZE,' ',
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_OP_CD_REPORT_TYPE,		0,0}, /* Type de compte-rendu d'opération bancaire  */
		{LG_OP_CODE,				0,0}, /* Code opération bancaire  */
		{LG_STATE_1_IND,			0,0}, /* Indicateur ETAT1  */
		{LG_STATE_2_IND,			0,0}, /* Indicateur ETAT2  */
		{LG_STATE_3_IND,			0,0}, /* Indicateur ETAT3  */
		{LG_ISO2_CONTENT,			0,0}, /* Contenu ISO2  */
		{LG_TEMP_CARD_IND,			0,0}, /* Indicateur carte temporisée  */
		{LG_B0_BLOCK_CHIP_ATM_REQ,	0,0}, /* Blocage puce B0’ demandé au GAB  */
		{LG_EMV_PROC_RESULT,		0,0}, /* Résultat du traitement du microcircuit EMV  */
		{LG_EMV_WAG_IND,			0,0}, /* Indicateur wagon EMV  */
		{LG_CH_NO,					0,0}, /* Numéro de porteur issu de l’application  */
		{LG_SPEC_FILLER,			0,131}, /* SPEC_FILLER */
	}

};


#define DEF_LGM_FUND_TRNSF_ADVICE_NB_FIELDS		12 + 1
#define DEF_LGM_FUND_TRNSF_ADVICE_SIZE			250 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_FUND_TRNSF_ADVICE = {
	LGM_FUND_TRNSF_ADVICE,DEF_LGM_FUND_TRNSF_ADVICE_NB_FIELDS,DEF_LGM_FUND_TRNSF_ADVICE_SIZE,' ',
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_OP_CD_REPORT_TYPE,		0,0}, /* Type de compte-rendu d'opération bancaire  */
		{LG_OP_CODE,				0,0}, /* Code opération bancaire  */
		{LG_STATE_1_IND,			0,0}, /* Indicateur ETAT1  */
		{LG_STATE_2_IND,			0,0}, /* Indicateur ETAT2  */
		{LG_STATE_3_IND,			0,0}, /* Indicateur ETAT3  */
		{LG_ISO2_CONTENT,			0,0}, /* Contenu ISO2  */
		{LG_ACCOUNT_TO_DEBIT,		0,0}, /* Numéro du compte à débiter  */
		{LG_ACCOUNT_TO_CREDIT,		0,23}, /* Numéro du compte à créditer  */
		{LG_OP_AMOUNT,				0,0}, /* Montant de l’opération  */
		{LG_SPEC_FILLER,			0,2}, /* SPEC_FILLER */
		{LG_CURR_OP_CURRENCY,		0,0}, /* Code devise de l'opération en cours  */
		{LG_SPEC_FILLER,			0,105}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_OTHER_OP_ADVICE_NB_FIELDS	23 + 1
#define DEF_LGM_OTHER_OP_ADVICE_SIZE		250 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_OTHER_OP_ADVICE = {
	LGM_OTHER_OP_ADVICE,DEF_LGM_OTHER_OP_ADVICE_NB_FIELDS,DEF_LGM_OTHER_OP_ADVICE_SIZE,' ',
	{
		{LG_MSG_HEADER,					0,0}, /* Header */
		{LG_OP_CD_REPORT_TYPE,			0,0}, /* Type de compte-rendu d'opération bancaire  */
		{LG_OP_CODE,					0,0}, /* Code opération bancaire  */
		{LG_STATE_1_IND,				0,0}, /* Indicateur ETAT1  */
		{LG_STATE_2_IND,				0,0}, /* Indicateur ETAT2  */
		{LG_STATE_3_IND,				0,0}, /* Indicateur ETAT3  */
		{LG_ISO2_CONTENT,				0,0}, /* Contenu ISO2  */
		{LG_OP_AMOUNT,					0,0}, /* Montant de l’opération  */
		{LG_SELECTED_ACC_NO,			0,0}, /* Numéro du compte choisi  */
		{LG_DEPOSIT_NO,					0,0}, /* Numéro de dépôt  */
		{LG_CHOICE_1,					0,0}, /* Choix 1  */
		{LG_CHOICE_2,					0,0}, /* Choix 2  */
		{LG_CHOICE_3,					0,0}, /* Choix 3  */
		{LG_CHOICE_4,					0,0}, /* Choix 4  */
		{LG_CHECK_CMC7_NO,				0,0}, /* Numéro CMC7 du chèque  */
		{LG_CHECK_INTERBANK_CMC7_CD,	0,0}, /* Code interbancaire CMC7 du chèque  */
		{LG_CHECK_CMC7_ACC_NO,			0,0}, /* Numéro de compte CMC7 du chèque  */
		{LG_AMOUNT_SCANNED,				0,0}, /* Montant lu par le scanner  */
		{LG_CHECK_REMITTANCE_NO,		0,0}, /* Numéro de la remise de chèques  */
		{LG_REMITTANCE_NB_CHECK,		0,0}, /* Nombre de chèques de la remise  */
		{LG_REMITTANCE_FINAL_AMOUNT,	0,0}, /* Montant final de la remise  */
		{LG_SPEC_FILLER,				0,2}, /* SPEC_FILLER */
		{LG_CURR_OP_CURRENCY,			0,0}, /* Code devise de l'opération en cours  */
		{LG_SPEC_FILLER,				0,59}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_WTHDRL_ADVICE_F_CRNCY_NB_FIELDS		33 + 1
#define DEF_LGM_WTHDRL_ADVICE_F_CRNCY_SIZE			250 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_WTHDRL_ADVICE_F_CRNCY = {
	LGM_WTHDRL_ADVICE_F_CRNCY,DEF_LGM_WTHDRL_ADVICE_F_CRNCY_NB_FIELDS,DEF_LGM_WTHDRL_ADVICE_F_CRNCY_SIZE,' ',
	{
		{LG_MSG_HEADER,					0,0}, /* Header */
		{LG_OP_CD_REPORT_TYPE,			0,0}, /* Type de compte-rendu d'opération bancaire  */
		{LG_OP_CODE,					0,0}, /* Code opération bancaire  */
		{LG_STATE_1_IND,				0,0}, /* Indicateur ETAT1  */
		{LG_STATE_2_IND,				0,0}, /* Indicateur ETAT2  */
		{LG_STATE_3_IND,				0,0}, /* Indicateur ETAT3  */
		{LG_ISO2_CONTENT,				0,0}, /* Contenu ISO2  */
		{LG_CURR_OP_CURRENCY,			0,0}, /* Code devise de l'opération en cours  */
		{LG_AUTHORIZED_AMOUNT,			0,0}, /* Montant autorisé (en devise principale)  */
		{LG_AUTH_CODE,					0,0}, /* Numéro d'autorisation  */
		{LG_OP_PROC_MODE,				0,0}, /* Mode de traitement de l’opération  */
		{LG_SPEC_FILLER,				0,1}, /* SPEC_FILLER */
		{LG_DISTR_AMNT_P_CRNCY,			0,0}, /* Montant distribué (en devise principale)  */
		{LG_DISTR_AMNT_F_CRNCY,			0,0}, /* Montant distribué (en devise étrangère)  */
		{LG_DISC_AMNT_HT,				0,0}, /* Montant de la commission HT  */
		{LG_AVAIL_CRNY,					0,0}, /* Encaisse Out disponible de la devise  */
		{LG_REVERSAL_NOTIF,				0,0}, /* Notification de redressement  */
		{LG_K7OUT_O1_NB_DIST,			0,0}, /* Nombre de medias distribués en cassette de type K7Out ordre 1  */
		{LG_K7OUT_O2_NB_DIST,			0,0}, /* Nombre de medias distribués en cassette de type K7Out ordre 2  */
		{LG_K7OUT_O3_NB_DIST,			0,0}, /* Nombre de medias distribués en cassette de type K7Out ordre 3  */
		{LG_K7OUT_O4_NB_DIST,			0,0}, /* Nombre de medias distribués en cassette de type K7Out ordre 4  */
		{LG_B0_BLOCK_CHIP_ATM_REQ,		0,0}, /* Blocage puce B0’ demandé au GAB  */
		{LG_PRINCIPAL_CRNCY,			0,0}, /* Code devise de la devise principale  */
		{LG_K7OUT_O1_DEN,				0,0}, /* Dénomination cassette de type K7Out ordre 1  */
		{LG_K7OUT_O2_DEN,				0,0}, /* Dénomination cassette de type K7Out ordre 2  */
		{LG_K7OUT_O3_DEN,				0,0}, /* Dénomination cassette de type K7Out ordre 3  */
		{LG_K7OUT_O4_DEN,				0,0}, /* Dénomination cassette de type K7Out ordre 4  */
		{LG_EMV_PROC_RESULT,			0,0}, /* Résultat du traitement du microcircuit EMV  */
		{LG_EMV_WAG_IND,				0,0}, /* Indicateur wagon EMV  */
		{LG_SCD_USAGE_MODE_TERM,		0,0}, /* Modes d’utilisations SCD  */
		{LG_SPEC_FILLER,				0,41}, /* SPEC_FILLER */
		{LG_MAC_TYPE,					0,0}, /* Type de scellement exécuté  */
		{LG_MAC_LEN,					0,0}, /* Longueur du sceau  */
		{LG_MAC,						0,0}, /* Sceau du message  */

	}

};

#define DEF_LGM_DEPOSIT_ADVICE_NB_FIELDS		31 + 1
#define DEF_LGM_DEPOSIT_ADVICE_SIZE				250 - MSG_HDR_OFST


TSLGMsgLayout	DEF_LGM_DEPOSIT_ADVICE = {
	LGM_DEPOSIT_ADVICE,DEF_LGM_DEPOSIT_ADVICE_NB_FIELDS,DEF_LGM_DEPOSIT_ADVICE_SIZE,' ',
	{
		{LG_MSG_HEADER,					0,0}, /* Header */
		{LG_OP_CD_REPORT_TYPE,			0,0}, /* Type de compte-rendu d'opération bancaire  */
		{LG_OP_CODE,					0,0}, /* Code opération bancaire  */
		{LG_STATE_1_IND,				0,0}, /* Indicateur ETAT1  */
		{LG_STATE_2_IND,				0,0}, /* Indicateur ETAT2  */
		{LG_STATE_3_IND,				0,0}, /* Indicateur ETAT3  */
		{LG_ISO2_CONTENT,				0,0}, /* Contenu ISO2  */
		{LG_ACCOUNT_TO_CREDIT,			0,11}, /* Numéro du compte à créditer  */
		{LG_MONEY_DEPOSIT_NO,			0,0}, /* Numéro du dépôt d’argent  */
		{LG_CURR_OP_CURRENCY,			0,0}, /* Code devise de l'opération en cours  */
		{LG_DEPOSIT_AMNT_OP_CRNY,		0,0}, /* Montant total déposé dans la devise de l’opération en cours  */
		{LG_DEPOSIT_AMNT_P_CRNCY,		0,0}, /* Montant total déposé dans la devise principale  */
		{LG_CONV_RATE,					0,0}, /* Taux de conversion ou Cours suivant la quotité  */
		{LG_QUOTA,						0,0}, /* Quotité  */
		{LG_SALE_TYPE,					0,0}, /* Type de vente  */
		{LG_OP_CRNCY_DEP_BAL,			0,0}, /* Encaisse In théorique du dépôt valorisé dans la devise de l’opération  */
		{LG_SPEC_FILLER,				0,10}, /* SPEC_FILLER */
		{LG_PRINCIPAL_CRNCY,			0,0}, /* Code devise de la devise principale  */
		{LG_K7MIXT_O1_DEN,				0,0}, /* Dénomination cassette de type K7Mixte ordre 1  */
		{LG_K7MIXT_O1_NB_STORED,		0,0}, /* Nombre de medias stockés en cassette de type K7Mixte ordre 1  */
		{LG_K7MIXT_O2_DEN,				0,0}, /* Dénomination cassette de type K7Mixte ordre 2 */
		{LG_K7MIXT_O2_NB_STORED,		0,0}, /* Nombre de medias stockés en cassette de type K7Mixte ordre 2  */
		{LG_K7MIXT_O3_DEN,				0,0}, /* Dénomination cassette de type K7Mixte ordre 3  */
		{LG_K7MIXT_O3_NB_STORED,		0,0}, /* Nombre de medias stockés en cassette de type K7Mixte ordre 3  */
		{LG_K7MIXT_O4_DEN,				0,0}, /* Dénomination cassette de type K7Mixte ordre 4  */
		{LG_K7MIXT_O4_NB_STORED,		0,0}, /* Nombre de medias stockés en cassette de type K7Mixte ordre 4  */
		{LG_BANK_CODE,					0,0}, /* Code banque  */
		{LG_RIB_KEY,					0,0}, /* Clé RIB  */
		{LG_BRANCH_CODE,				0,0}, /* Code agence (ou code guichet)  */
		{LG_CR_ACC_BANK_CODE,			0,0}, /* Code banque du compte à créditer  */
		{LG_BUNDLES_DEP_REPORT_WAG_IND,	0,0}, /* Indicateur wagon compte rendu dépôt en liasse  */
		{LG_SPEC_FILLER,				0,17}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_WAG_BUNDLE_DEPOSIT_NB_FIELDS		8
#define DEF_LGM_WAG_BUNDLE_DEPOSIT_SIZE				19

TSLGMsgLayout	DEF_LGM_WAG_BUNDLE_DEPOSIT = {
	LGM_WAG_BUNDLE_DEPOSIT,DEF_LGM_WAG_BUNDLE_DEPOSIT_NB_FIELDS,DEF_LGM_WAG_BUNDLE_DEPOSIT_SIZE,' ',
	{
		
		{LG_SEPERATOR,			0,0}, /* Séparateur  */
		{LG_WAG_TYPE_CODE,		0,0}, /* Code type de Wagon  */
		{LG_T3_SUSP_CR_MODE,	0,0}, /* Mode crédit en compte des medias de type 3 ou douteux  */
		{LG_TYPE_1_MEDIA_NBR,	0,0}, /* Nombre total de media de type 1  */
		{LG_NTYPE_1_MEDIA_NBR,	0,0}, /* Nombre total de media hors type 1  */
		{LG_NB_OP_BUNDLES,		0,0}, /* Nombre total de liasses de l’opération  */
		{LG_DATA_FIELD_LEN,		0,0}, /* Longueur du champ de données  */
		{LG_SPEC_FILLER,		0,2}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_BUNDLE_NB_FIELDS		6
#define DEF_LGM_BUNDLE_SIZE				25

TSLGMsgLayout	DEF_LGM_BUNDLE = {
	LGM_BUNDLE,DEF_LGM_BUNDLE_NB_FIELDS,DEF_LGM_BUNDLE_SIZE,' ',
	{
		{LG_WAD_NO,				0,0}, /* Numéro de la liasse  */
		{LG_IS1_WAD_NBR,		0,0}, /* Nombre total de media de type 1 de la liasse  */
		{LG_NO1_WAD_NBR,		0,0}, /* Nombre total de medias hors type 1 de la liasse  */
		{LG_TOTAL_WAD_AMOUNT,	0,0}, /* Montant total de la liasse  */
		{LG_NB_WAD_DEN_NBR,		0,0}, /* Nombre total de dénomination de la liasse  */
		{LG_SPEC_FILLER,		0,2}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_DEN_NB_FIELDS			6
#define DEF_LGM_DEN_SIZE				22

TSLGMsgLayout	DEF_LGM_DEN = {
	LGM_DEN,DEF_LGM_DEN_NB_FIELDS,DEF_LGM_DEN_SIZE,' ',
	{

		{LG_CURR_DEN,						0,0}, /* Code dénomination de la dénomination courante  */
		{LG_CURR_DEN_TYPE2_MEDIA_NBR,		0,0}, /* Nombre de medias de type 2 de la dénomination courante  */
		{LG_CURR_DEN_TYPE3_MEDIA_NBR,		0,0}, /* Nombre de medias de type 3 de la dénomination courante  */
		{LG_CURR_DEN_TYPE4A_MEDIA_NBR,		0,0}, /* Nombre de medias de type 4a de la dénomination courante  */
		{LG_CURR_DEN_TYPE4B_MEDIA_NBR,		0,0}, /* Nombre de medias de type 4b de la dénomination courante  */
		{LG_SPEC_FILLER,					0,4}, /* SPEC_FILLER */

	}
};

#define DEF_LGM_TOPUP_ADVICE_NB_FIELDS		18 + 1
#define DEF_LGM_TOPUP_ADVICE_SIZE			250 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_TOPUP_ADVICE = {
	LGM_TOPUP_ADVICE,DEF_LGM_TOPUP_ADVICE_NB_FIELDS,DEF_LGM_TOPUP_ADVICE_SIZE,' ',
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_OP_CD_REPORT_TYPE,		0,0}, /* Type de compte-rendu d'opération bancaire  */
		{LG_OP_CODE,				0,0}, /* Code opération bancaire  */
		{LG_STATE_1_IND,			0,0}, /* Indicateur ETAT1  */
		{LG_STATE_2_IND,			0,0}, /* Indicateur ETAT2  */
		{LG_STATE_3_IND,			0,0}, /* Indicateur ETAT3  */
		{LG_CURR_OP_CURRENCY,		0,0}, /* Code devise de l'opération en cours  */
		{LG_MONEO_CHIP_PROC_RES,	0,0}, /* Résultat traitement microcircuit Monéo  */
		{LG_DOWNLOAD_TYPE,			0,0}, /* Type de chargement  */
		{LG_TSEQ,					0,0}, /* TSEQ (N° séquence reçu lors de la demande de contexte)  */
		{LG_TERMINAL_ID,			0,0}, /* Terminal_Id (N° terminal reçu lors de la demande de contexte)  */
		{LG_CH_NO,					0,0}, /* Numéro de porteur issu de l’application  */
		{LG_AMONT_ENTERED,			0,0}, /* Montant du chargement composé  */
		{LG_PME_AUTH_NO,			0,0}, /* Numéro d’autorisation PME  */
		{LG_MNG_RESP_CODE_TOPUP_RESP,0,0}, /* Code réponse gestionnaire, en réponse à la demande de chargement  */
		{LG_SPEC_FILLER,			0,122}, /* SPEC_FILLER */
		{LG_AUTH_CODE,				0,0}, /* Numéro d'autorisation  */
		{LG_EMV_PROC_RESULT,		0,0}, /* Résultat du traitement du microcircuit EMV  */
		{LG_EMV_WAG_IND,			0,0}, /* Indicateur wagon EMV  */

	}

};


#define DEF_LGM_OP_REPORT_RESP_NB_FIELDS		7 + 1
#define DEF_LGM_OP_REPORT_RESP_SIZE				112 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_OP_REPORT_RESP = {
	LGM_OP_REPORT_RESP,DEF_LGM_OP_REPORT_RESP_NB_FIELDS,DEF_LGM_OP_REPORT_RESP_SIZE,'X',
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
		{LG_SEPERATOR,				0,0}, /* Séparateur  */
		{LG_FUNCTION_CODE,			0,0}, /* Code fonction  */
		{LG_OP_NO,					0,0}, /* Numéro d'opération bancaire  */
		{LG_ACTION_CODE,			0,0}, /* Code action  */
		{LG_EXP_CMD_OP_CD,			0,0}, /* Code de l’opération d'exploitation télécommandée  */
		{LG_EXP_CMD_DATA,			0,0}, /* Données de l’opération d'exploitation télécommandée  */
		{LG_SPEC_FILLER,			0,41}, /* SPEC_FILLER */

	}

};

#define DEF_LGM_END_OF_TRN_REQ_NB_FIELDS		0 + 1
#define DEF_LGM_END_OF_TRN_REQ_SIZE				48 - MSG_HDR_OFST

TSLGMsgLayout	DEF_LGM_END_OF_TRN_REQ = {
	LGM_END_OF_TRN_REQ,DEF_LGM_END_OF_TRN_REQ_NB_FIELDS,DEF_LGM_END_OF_TRN_REQ_SIZE,LG_FC_END_OF_TRN,
	{
		{LG_MSG_HEADER,				0,0}, /* Header */
	}

};



TSLGMsgLayout*	tab_MsgLayout[] = {


	&DEF_LGM_HEADER					,
	&DEF_LGM_PERIODIC_REQ			,
	&DEF_LGM_CASST_WAG				,
	&DEF_LGM_CASST					,
	&DEF_LGM_PERIODIC_RESP			,
	&DEF_LGM_MSG_ACK_CHANNEL_2		,
	&DEF_LGM_EXCEPTION_MSG			,
	&DEF_LGM_REENTRY_RESP			,
	&DEF_LGM_KEYS_DL_REQ_1			,
	&DEF_LGM_KEYS_DL_RESP_1			,
	&DEF_LGM_PARAM_FILE_DL_REQ		,
	&DEF_LGM_PARAM_FILE_DL_RESP		,
	&DEF_LGM_TERM_SPEC_REQ			,
	&DEF_LGM_TERM_SPEC_RESP			,
	&DEF_LGM_TERM_SPEC_PARAM_REQ		,
	&DEF_LGM_TERM_SPEC_PARAM_RESP	,
	&DEF_LGM_TRANSFER_REQ			,
	&DEF_LGM_EXP_CMD_REPORT_RMT_1	,
	&DEF_LGM_EXP_CMD_REPORT_RMT_2	,
	&DEF_LGM_EXP_CMD_REPORT_RMT_RESP	,
	&DEF_LGM_INIT_TLP_REQ			,
	&DEF_LGM_INIT_TLP_RESP			,
	&DEF_LGM_TBL_TRNSF_REQ			,
	&DEF_LGM_TBL_TRNSF_RESP			,
	&DEF_LGM_END_TLP_REQ				,
	&DEF_LGM_END_TLP_RESP			,
	&DEF_LGM_KEYS_DL_REQ_2			,
	&DEF_LGM_KEYS_DL_RESP_2			,
	&DEF_LGM_EXP_CMD_REPORT_MAN_1	,
	&DEF_LGM_EXP_CMD_REPORT_MAN_2	,
	&DEF_LGM_EXP_CMD_REPORT_MAN_RESP	,
	&DEF_LGM_INIT_TRN_REQ			,
	&DEF_LGM_INITIAL_REQ				,
	&DEF_LGM_INITIAL_REQ_RESP		,
	&DEF_LGM_MIDDLE_REQ				,
	&DEF_LGM_MIDDLE_REQ_RESP			,
	&DEF_LGM_WAG_PROD_1				,
	&DEF_LGM_WAG_PROD_2				,
	&DEF_LGM_WAG_COM					,
	&DEF_LGM_WAG_EMV					,
	&DEF_LGM_INQ_REQ					,
	&DEF_LGM_INQ_RESP				,
	&DEF_LGM_INQ_RESP_BLK			,
	&DEF_LGM_ILS_REQ					,
	&DEF_LGM_ILS_RESP				,
	&DEF_LGM_ILS_RESP_BLK			,
	&DEF_LGM_PRINTER_PG_TRNSF_REQ	,
	&DEF_LGM_PRINTER_PG_TRNSF_RESP	,
	&DEF_LGM_SCREEN_PG_TRNSF_REQ		,
	&DEF_LGM_SCREEN_PG_TRNSF_RESP	,
	&DEF_LGM_SALES_COND_REQ			,
	&DEF_LGM_SALES_COND_RESP			,
	&DEF_LGM_MULTI_SALES_COND_REQ	,
	&DEF_LGM_MULTI_SALES_COND_RESP	,
	&DEF_LGM_AUTH_REQ				,
	&DEF_LGM_AUTH_RESP				,
	&DEF_LGM_AUTH_REQ_F_CRNCY		,
	&DEF_LGM_AUTH_RESP_F_CRNCY		,
	&DEF_LGM_BAL_INQ					,
	&DEF_LGM_BAL_INQ_RESP			,
	&DEF_LGM_MONEO_CTX_REQ			,
	&DEF_LGM_MONEO_CTX_REQ_RESP		,
	&DEF_LGM_TOPUP_REQ				,
	&DEF_LGM_TOPUP_RESP				,
	&DEF_LGM_WAG_MONEO				,
	&DEF_LGM_WITHDRAWAL_ADVICE		,
	&DEF_LGM_DEGRADE					,
	&DEF_LGM_FUND_TRNSF_ADVICE		,
	&DEF_LGM_OTHER_OP_ADVICE			,
	&DEF_LGM_WTHDRL_ADVICE_F_CRNCY	,
	&DEF_LGM_DEPOSIT_ADVICE			,
	&DEF_LGM_WAG_BUNDLE_DEPOSIT		,
	&DEF_LGM_BUNDLE					,
	&DEF_LGM_DEN						,
	&DEF_LGM_TOPUP_ADVICE			,
	&DEF_LGM_OP_REPORT_RESP			,
	&DEF_LGM_END_OF_TRN_REQ			,

};


typedef struct {
	char				function_code;
	E_LG_LAYOUT_TYPE	eLayoutType;
} TSLGSrvMsg;

static TSLGSrvMsg tab_server_msg[]= {

{LG_FC_PERIODIC_MSG,			LGM_PERIODIC_RESP			},
{LG_FC_MSG_ACK_2,				LGM_MSG_ACK_CHANNEL_2		},
{LG_FC_EXCP_MSG,				LGM_EXCEPTION_MSG			},
{LG_FC_REENTRY_RESP,			LGM_REENTRY_RESP			},
{LG_FC_KEYS_DL,					LGM_KEYS_DL_RESP_1			},
{LG_FC_PERSO_FILE_DL,			LGM_PARAM_FILE_DL_RESP		},
{LG_FC_TERM_SPEC,				LGM_TERM_SPEC_RESP			},
{LG_FC_TERM_SPEC_PARAM,			LGM_TERM_SPEC_PARAM_RESP	},
{LG_FC_REMOTE_EXP_CMD_REPORT,	LGM_EXP_CMD_REPORT_RMT_RESP	},
{LG_FC_INIT_TLP,				LGM_INIT_TLP_RESP			},
{LG_FC_WAG_PROD,				LGM_TBL_TRNSF_RESP			},
{LG_FC_END_OF_TLP,				LGM_END_TLP_RESP			},
{LG_FC_CRYPTO_DL_REQ_2,			LGM_KEYS_DL_RESP_2			},
{LG_FC_LOCAL_EXP_CMD_REPORT,	LGM_EXP_CMD_REPORT_MAN_RESP	},
{LG_FC_INIT_REQ,				LGM_INITIAL_REQ_RESP		},
{LG_FC_MIDDLE_REQ,				LGM_MIDDLE_REQ_RESP			},
{LG_FC_WAG_PROD,				LGM_WAG_PROD_1				},
{LG_FC_PRD_WAG_2,				LGM_WAG_PROD_2				},
{LG_FC_WAG_COM,					LGM_WAG_COM					},
{LG_FC_INQ_REQ,					LGM_INQ_RESP				},
{LG_FC_ILS_REQ,					LGM_ILS_RESP				},
{LG_FC_PRINTER_PG_TRNSF_REQ,	LGM_PRINTER_PG_TRNSF_RESP	},
{LG_FC_SCREEN_PG_TRNSF_REQ,		LGM_SCREEN_PG_TRNSF_RESP	},
{LG_FC_SALES_COND,				LGM_SALES_COND_RESP			},
{LG_FC_WAG_CASST,				LGM_MULTI_SALES_COND_RESP	},
{LG_FC_AUTH_REQ,				LGM_AUTH_RESP				},
{LG_FC_AUTH_REQ_F_CRNCY,		LGM_AUTH_RESP_F_CRNCY		},
{LG_FC_BAL_INQ,					LGM_BAL_INQ_RESP			},
{LG_FC_MONEO_CTX,				LGM_MONEO_CTX_REQ_RESP		},
{LG_FC_TOPUP_AUTH,				LGM_TOPUP_RESP				},
{LG_FC_OP_REPORT,				LGM_OP_REPORT_RESP			},
{LG_FC_END_OF_TRN,				LGM_END_OF_TRN_REQ			},

};


void InitLGMsgLayout(TSLGMsgInfo* msgInfo, E_LG_LAYOUT_TYPE eMsgType)
{
	memset(msgInfo->data,' ',sizeof(msgInfo->data));
	msgInfo->data[sizeof(msgInfo->data) - 1 ]='\0';
	msgInfo->layout = tab_MsgLayout[eMsgType];
	msgInfo->nLength=0;
}

int InitServerLGMessage(E_SERVER_MSG eSrvMsgTp, TSLGMsgInfo* msgInfo)
{
	
	msgInfo->layout = tab_MsgLayout[tab_server_msg[eSrvMsgTp].eLayoutType];
	memset(msgInfo->data,   '\0', sizeof(msgInfo->data));
	msgInfo->nLength=0;
}

int AnalyseLGMsgHeader(char* data, TSLGMsgInfo* msgInfo)
{
	InitLGMsgLayout(msgInfo,LGM_HEADER);
	strncpy(msgInfo->data,data,LG_MSG_HEADER_LEN);
	msgInfo->data[LG_MSG_HEADER_LEN]='\0';
	msgInfo->nLength=LG_MSG_HEADER_LEN;
}

void AnalyseLGMsgLayout(char* data, int nLength, TSLGMsgInfo* msgInfo, E_LG_LAYOUT_TYPE eMsgType)
{
	memcpy(msgInfo->data,data,nLength);
	msgInfo->data[nLength]='\0';
	msgInfo->layout = tab_MsgLayout[eMsgType];
	msgInfo->nLength = nLength;
}


int AnalyseLGTermMessage(char* data, int nDataLen, TSLGMsgInfo* msgInfo)
{
	E_LG_LAYOUT_TYPE	eMsgType;
	TSLGMsgInfo			kLocMsg;
	char				function_code[4];
	char				report_code[4];
	char				op_code[4];
	int					nLength;
	char*				sPtr;
	int					nReportCode;
	int					nOpCode;
	char				report_type;



	if( nDataLen <= LG_MSG_HEADER_LEN )
	{
		return NOK;
	}

	msgInfo->nLength = nDataLen;

	AnalyseLGMsgHeader(data, &kLocMsg);


	if( GetLGMsgField(LG_FUNCTION_CODE, &kLocMsg, function_code, &nLength) != SUCCESS )
	{
		return NOK;
	}

	sPtr = data + LG_MSG_HEADER_LEN;

	switch(function_code[0])
	{

	case LG_FC_PERIODIC_MSG:
		eMsgType = LGM_PERIODIC_REQ;break;
	case LG_FC_KEYS_DL:
		eMsgType = LGM_KEYS_DL_REQ_1;break;
	case LG_FC_PERSO_FILE_DL:
		eMsgType = LGM_PARAM_FILE_DL_REQ;break;
	case LG_FC_TERM_SPEC:
		eMsgType = LGM_TERM_SPEC_REQ;break;
	case LG_FC_TERM_SPEC_PARAM:
		eMsgType = LGM_TERM_SPEC_PARAM_REQ;break;
	case LG_FC_EJ_TRNSF_REQ:
		eMsgType = LGM_TRANSFER_REQ;break;
	case LG_FC_REMOTE_EXP_CMD_REPORT:
		memcpy(op_code, sPtr , 2);op_code[2]='\0';
		nOpCode = atoi(op_code);
		switch(nOpCode)
		{
		case RO_N_LOG_RES_STATE					:
		case RO_N_CHECK_CASST_K7IN				:
		case RO_N_CHECK_CASST_PARAM_K7IN_K7MIXT	:
		case RO_N_EDIT_TOTALS					:
		case RO_N_EDIT_COUNTERS					:
		case RO_N_ACC_BOX						:
		case RO_N_AUTO_ACC_BOX					:
		case RO_N_ACC_K7IN_DEP_CASST			:
		case RO_N_CHECK_CASST_K7OUT_K7MIXT		:
		case RO_N_CHECK_CASST_PARAM_K7OUT_K7MIXT:
			eMsgType = LGM_EXP_CMD_REPORT_RMT_1;break;
		default:
			eMsgType = LGM_EXP_CMD_REPORT_RMT_2;break;
		}
		break;
	case LG_FC_INIT_TLP:
		eMsgType = LGM_INIT_TLP_REQ;break;
	case LG_FC_TBL_TRNSF:
		eMsgType = LGM_TBL_TRNSF_REQ;break;
	case LG_FC_END_OF_TLP:
		eMsgType = LGM_END_TLP_REQ;break;
	case LG_FC_CRYPTO_DL_REQ_2:
		eMsgType = LGM_KEYS_DL_REQ_2;break;
	case LG_FC_LOCAL_EXP_CMD_REPORT:
		memcpy(report_code, sPtr, 2);report_code[2]='\0';
		nReportCode = atoi(report_code);
		switch(nReportCode)
		{
			case REP_CD_N_CHECK_CASST_PARAM_K7IN_K7MIXT	:
			case REP_CD_N_EDIT_TOTALS					:
			case REP_CD_N_EDIT_COUNTERS					:
			case REP_CD_N_CHECK_CASST_PARAM_K7OUT_K7MIXT :
			case REP_CD_N_NOTES_TYPE_AUT				:
			case REP_CD_N_LOCAL_EXP_ENTRY_1				:
			case REP_CD_N_LOCAL_EXP_ENTRY_2				:
			case REP_CD_N_LOCAL_EXP_EXIT_1				:
			case REP_CD_N_LOCAL_EXP_EXIT_2				:
			case REP_CD_N_ACC_CASST_K7IN				:
			case REP_CD_N_LOG_RES_PARAM					:
			case REP_CD_N_PARAM_K7IN_K7MIXT				:
			case REP_CD_N_UNLOAD_K7IN					:
			case REP_CD_N_ACC_BOX_CLOSURE_1				:
			case REP_CD_N_FORCE_BAL_OUT					:
			case REP_CD_N_57							:
			case REP_CD_N_67							:
				eMsgType = LGM_EXP_CMD_REPORT_MAN_1;break;
			default:
				eMsgType = LGM_EXP_CMD_REPORT_MAN_2;break;
		}
		break;
	case LG_FC_INIT_TRN:
		eMsgType = LGM_INIT_TRN_REQ;break;
	case LG_FC_INIT_REQ:
		eMsgType = LGM_INITIAL_REQ;break;
	case LG_FC_MIDDLE_REQ:
		eMsgType = LGM_MIDDLE_REQ;break;
	case LG_FC_INQ_REQ:
		eMsgType = LGM_INQ_REQ;break;
	case LG_FC_ILS_REQ:
		eMsgType = LGM_ILS_REQ;break;
	case LG_FC_PRINTER_PG_TRNSF_REQ:
		eMsgType = LGM_PRINTER_PG_TRNSF_REQ;break;
	case LG_FC_SCREEN_PG_TRNSF_REQ:
		eMsgType = LGM_SCREEN_PG_TRNSF_REQ;break;
	case LG_FC_SALES_COND:
		eMsgType = LGM_SALES_COND_REQ;break;
	case LG_FC_MULTI_SALES_COND_INQ:
		eMsgType = LGM_MULTI_SALES_COND_REQ;break;
	case LG_FC_AUTH_REQ:
		eMsgType = LGM_AUTH_REQ;break;
	case LG_FC_AUTH_REQ_F_CRNCY:
		eMsgType = LGM_AUTH_REQ_F_CRNCY;break;
	case LG_FC_BAL_INQ:
		eMsgType = LGM_BAL_INQ;break;
	case LG_FC_MONEO_CTX:
		eMsgType = LGM_MONEO_CTX_REQ;break;
	case LG_FC_TOPUP_AUTH:
		eMsgType = LGM_TOPUP_REQ;break;
	case LG_FC_OP_REPORT:
		report_type = sPtr[0];
		switch(report_type)
		{
			case LG_F168_WITHDRAWAL			:
				eMsgType = LGM_WITHDRAWAL_ADVICE;break;
			case LG_F168_INCID				:
				eMsgType = LGM_DEGRADE;break;
			case LG_F168_FUND_TRNSF			:
				eMsgType = LGM_FUND_TRNSF_ADVICE;break;
			case LG_F168_OTHER_OP			:
				eMsgType = LGM_OTHER_OP_ADVICE;break;
			case LG_F168_WITHDRAWAL_F_CRNCY	:
				eMsgType = LGM_WTHDRL_ADVICE_F_CRNCY;break;
			case LG_F168_DEPOSIT				:
				eMsgType = LGM_DEPOSIT_ADVICE;break;
			case LG_F168_TOPUP_MONEO			:
				eMsgType = LGM_TOPUP_ADVICE;break;
			default:
				return NOK;
				break;
		}
		break;
	default:
		return NOK;
	}

	msgInfo->layout = tab_MsgLayout[eMsgType];
	/*
	strncpy(msgInfo->data, data, msgInfo->layout->nSize);
	msgInfo->data[msgInfo->layout->nSize]='\0';*/
	/*
	memcpy(msgInfo->header,data,LG_MSG_HEADER_LEN);
	memcpy(msgInfo->data,data + LG_MSG_HEADER_LEN, nDataLen - LG_MSG_HEADER_LEN);*/
	memcpy(msgInfo->data,data, nDataLen);
	return OK;
}



int SetupLGMsgLayouts()
{

	int		i,j;
	int		nSize;
	int		nFieldNo;
	int		nFieldLength;
	int		nOffset;
	char	sLine[MAX_LINE_TRC];
	int		nExpectedSize;
	int		nIsVariableLength;

	nSize = sizeof(tab_MsgLayout)/sizeof(tab_MsgLayout[0]);
	
	for(i = 0; i < nSize; i++ )
	{
		nOffset = 0;
		nIsVariableLength = NOK;
		fprintf(stderr,"================================================\n");
		/*if( i != LGM_AUTH_REQ && i != LGM_HEADER) continue;*/

		for(j=0; j < tab_MsgLayout[i]->nNbFields; j++ )
		{
			nFieldNo = tab_MsgLayout[i]->tab_fields[j].nFieldNo;
			if( nFieldNo != 999 )
			{
				nFieldLength = tab_LGFields[nFieldNo].nFieldLength;
				if( tab_LGFields[nFieldNo].eFieldType == LG_VAR_ALPHA )
				{
					/*
					sprintf(sLine,"ERROR: Field length is variable %d:%d(%d)",i,nFieldNo,nFieldLength);
					trace_event(sLine,ERROR);
					return NOK;*/
					fprintf(stderr,"ERROR: Field length is variable %d:%d(%d)\n",i,nFieldNo,nFieldLength);
					nFieldLength = tab_MsgLayout[i]->tab_fields[j].nSize;
					nIsVariableLength = OK;
				}
				else
				{
					tab_MsgLayout[i]->tab_fields[j].nSize = nFieldLength;
				}
			}
			else
			{
				nFieldLength = tab_MsgLayout[i]->tab_fields[j].nSize;
			}
			
			tab_MsgLayout[i]->tab_fields[j].nOffset = nOffset;
			nOffset += nFieldLength;
			fprintf(stderr,"Add %d = %d\n",nFieldLength,nOffset);
		}

		nExpectedSize = nOffset;
		/*
		if( tab_MsgLayout[i]->eLayoutType != LGM_HEADER )
		{
			nExpectedSize += LG_MSG_HEADER_LEN;
		}*/
		if( nExpectedSize != tab_MsgLayout[i]->nSize )
		{
			sprintf(sLine,"ERROR: Msg size is different than expected(%d VS %d) ,%d",nOffset,tab_MsgLayout[i]->nSize, tab_MsgLayout[i]->eLayoutType);
			trace_event(sLine,ERROR);
			fprintf(stderr,"%s\n",sLine);
			if( nIsVariableLength == NOK )
			{
				return NOK;
			}
		}

	}

}



