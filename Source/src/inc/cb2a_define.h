#ifndef HPS_CB2A__H__
#define HPS_CB2A__H__

#define CB2A_F031_NB_FIELDS				12
#define CB2A_F031_BUF_LEN				300
#define CB2A_F044_NB_FIELDS				15
#define CB2A_F044_BUF_LEN				128
#define CB2A_F046_NB_FIELDS				17
#define CB2A_F046_BUF_LEN				300
#define CB2A_F047_NB_FIELDS				19
#define CB2A_F047_BUF_LEN				300
#define CB2A_F058_NB_FIELDS				41
#define CB2A_F058_BUF_LEN				300
#define CB2A_F072_NB_FIELDS				49
#define CB2A_F072_BUF_LEN				4096
#define CB2A_F118_NB_FIELDS				5
#define CB2A_F118_BUF_LEN				1024

#define CB2A_AUT_F044_NB_FIELDS			11
#define CB2A_AUT_F044_BUF_LEN			300
#define CB2A_AUT_F047_NB_FIELDS			8
#define CB2A_AUT_F047_BUF_LEN			300
#define CB2A_AUT_F059_NB_FIELDS			33
#define CB2A_AUT_F059_BUF_LEN			300
#define CB2A_AUT_F117_NB_FIELDS			33
#define CB2A_AUT_F117_BUF_LEN			300

#define CB2A_AUT_F055_NB_FIELDS			18

#define MSG_TYPE_UNDEF			0
#define MSG_TYPE_REQ			1
#define MSG_TYPE_RESP			2


#define OR_CB2A_TIMEOUT					10
#define OR_CB2A_DISCONECT				13
#define OR_CB2A_INTERUP					15				 

#define CB2A_MSG_PROCESSED			100
#define CB2A_MSG_NOT_PROCESSED		101

#define INC_NO_INCIDENT				0
#define INC_NO_INCIDENT_EOT			1
#define INC_MSG_DATA_ERROR			2
#define INC_UNEXPECTED_MSG_NBR		3
#define INC_NB_MSG_EXCEEDED			4
#define INC_PROCESSING_ERROR		5
#define INC_EARLY_RECV_ACK			6
#define INC_RETRANSMISSION			7



/* Field presence flags */
#define FP_UNDEF				0
#define FP_MANDATORY			1
#define FP_CONDITIONAL			2
#define FP_OPTIONAL				3
#define FP_FORBIDDEN			4

/* Field origin flags */
#define FO_UNDEF				0
#define FO_REQ_MSG				1
#define FO_INIT_REQ_MSG			3
#define FO_INIT_REP_MSG			4
#define FO_GENERATED			5

/* macros to retreive presence/origin and to build a field indicator from presence/indicator */
#define FIELD_PRESENCE(c)		((c)&0x0F)
#define FIELD_ORIGIN(c)			(((c)&0xF0)>>4)
#define FIELD_MAP(p,o)			(((o)<<4)|(p))

#define B_UN					FIELD_MAP(FP_UNDEF,FO_UNDEF)
#define B_Z						FIELD_MAP(FP_FORBIDDEN,FO_UNDEF)

#define B_X						FIELD_MAP(FP_MANDATORY,FO_GENERATED)
#define B_XQ					FIELD_MAP(FP_MANDATORY,FO_REQ_MSG)
#define B_XQI					FIELD_MAP(FP_MANDATORY,FO_INIT_REQ_MSG)
#define B_XRI					FIELD_MAP(FP_MANDATORY,FO_INIT_REP_MSG)

#define B_C						FIELD_MAP(FP_CONDITIONAL,FO_GENERATED)
#define B_CQ					FIELD_MAP(FP_CONDITIONAL,FO_REQ_MSG)
#define B_CQI					FIELD_MAP(FP_CONDITIONAL,FO_INIT_REQ_MSG)
#define B_CRI					FIELD_MAP(FP_CONDITIONAL,FO_INIT_REP_MSG)

#define B_F						FIELD_MAP(FP_OPTIONAL,FO_GENERATED)
#define B_FQ					FIELD_MAP(FP_OPTIONAL,FO_REQ_MSG)
#define B_FQI					FIELD_MAP(FP_OPTIONAL,FO_INIT_REQ_MSG)
#define B_FRI					FIELD_MAP(FP_OPTIONAL,FO_INIT_REP_MSG)



/*Fields fomats */
#define F_C						0			/* Composed field, custom check */
#define F_N						1
#define F_AN					2
#define F_ANS					3
#define F_B						4
#define F_Z						5
#define F_UNDEF					9			/* Field undefined in CB2A */


#define TLP_STEP_INIT			0
#define TLP_STEP_TRANSFER		1
#define TLP_STEP_ACTIVATION		2
#define TLP_STEP_NO_TLP			9


#define OK_NO_DATA_TYPE 	111
/*
#define NO_ETAT_FONCTIONNEL 	0
#define ETAT_FONCT_BEFORE_TLP 	1
#define ETAT_FONCT_AFTER_TLP 	2
#define ETAT_FONCT_REASK_TLP 	3
*/
#define OPS_ACQUIRER_ID	"46321816558"
/******************************************************************************\
*
*		TLC-TLP
*
\******************************************************************************/
/* F024: Function codes values */

#define CB2A_F024_ADD_RECORD			"301"		/* Enregistrement à ajouter */
#define CB2A_F024_DEL_RECORD			"303"		/* Enregistrement à effacer */
#define CB2A_F024_REPLACE_FILE			"306"		/* Fichier à remplacer */
#define CB2A_F024_DELETE_FILE			"308"		/* Fichier à supprimer */
#define CB2A_F024_MODIFY_FILE			"360"		/* Fichier à modifier */
#define CB2A_F024_ACTIVATE_FILE			"361"		/* Activation positive de fichier */
#define CB2A_F024_N_ACTIVATE_FILE		"363"		/* Activation négative de fichier */
#define CB2A_F024_ACC_FUNC_ST_PARAM		"670"		/* Paramètres de l'état fonctionnel du système d’acceptation */
#define CB2A_F024_ACC_REF_DATA			"671"		/* Données de référence du système d'acceptation */
#define CB2A_F024_PA_REF_DATA			"672"		/* Données de référence du point d'acceptation */
#define CB2A_F024_FUNC_ST_ACK_NOTIF		"680"		/* Notification de prise en compte de l’état fonctionnel */
#define CB2A_F024_PROTO_INC_NOTIF		"681"		/* Notification d’incident protocole */
#define CB2A_F024_ECHO_TEST				"831"		/* Test d'écho */
#define CB2A_F024_DLG_RIGHT_PROPOSAL	"851"		/* Proposition du droit de parole */
#define CB2A_F024_CLOSE_SRV				"860"		/* Fermeture de dialogue ou fichier */
#define CB2A_F024_OPEN_DLG_SRV_TLC		"862"		/* Ouverture de dialogue + service télécollecte */
#define CB2A_F024_OPEN_DLG_SRV_TLP		"863"		/* Ouverture de dialogue + service téléparamétrage */
#define CB2A_F024_OPEN_SRV_TLC			"865"		/* Ouverture de service télécollecte */
#define CB2A_F024_OPEN_SRV_TLP			"866"		/* Ouverture de service téléparamétrage */
#define CB2A_F024_LEN					3


#define CB2A_F025_RECOVERY				"8022"		/* Reprise suite à incident */
#define CB2A_F025_INIT_POINT_ACCEPTOR	"8013"		/* Première initialisation système d’acceptation	*/
#define CB2A_F025_INIT_PA_HW_CHG		"8017"		/* Première initialisation du système d'acceptation, remplacement de matériel	*/
#define CB2A_F025_LEN					4


/* Valeurs utilisées dans un message demande:	*/
#define CB2A_F026_NO_ACK 			'0' 		/* Pas d'acquittement demandé				*/
#define CB2A_F026_ACK				'1' 		/* Acquittement demandé						*/
#define CB2A_F026_ACK_EOT			'2' 		/* Acquittement demandé, fin du transfert	*/
/* Valeurs utilisées dans un message réponse : */
#define CB2A_F026_ACK_OK			'3' 		/* Acquittement positif						*/
#define CB2A_F026_ACK_OK_EOT		'4' 		/* Acquittement positif, fin du transfert	*/
#define CB2A_F026_ACK_NEG_RETRY		'7' 		/* Acquittement négatif, demande de répétition	*/
#define CB2A_F026_ACKNEG_NO_RETRY	'8' 		/* Acquittement négatif, sans répétition demandée	*/
#define CB2A_F026_ACK_NEG_EOT		'9' 		/* Acquittement négatif, arrêt du transfert	*/
#define CB2A_F026_LEN				6


#define CB2A_F070_TOTALS_OK				'0'		/* Réconcilié, totaux corrects		*/
#define CB2A_F070_TOTALS_NOK			'1'		/* Réconcilié, totaux non corrects	*/
#define CB2A_F070_TOTALS_NA				'3'		/* Totaux non valable				*/

#define CB2A_F070_SIGN_ON 				"001" 		/* Ouverture de dialogue (sign-on)			*/
#define CB2A_F070_SIGN_OFF				"002" 		/* Fermeture de dialogue (sign-off)			*/
#define CB2A_F070_ECHO					"301" 		/* Test d'écho								*/
#define CB2A_F070_LEN					3


#define CB2A_F031_TAG_PRINTING		"01" 	/* Impression									*/
#define CB2A_F031_TAG_DISPLAY		"02" 	/* Affichage									*/
#define CB2A_F031_TAG_P_D			"03" 	/* Impression et affichage						*/
#define CB2A_F031_TAG_CH_P			"04" 	/* Impression pour le porteur uniquement		*/
#define CB2A_F031_TAG_CH_D			"05" 	/* Affichage pour le porteur uniquement			*/
#define CB2A_F031_TAG_CH_D_P		"06" 	/* Impression et affichage pour le porteur		*/
#define CB2A_F031_TAG_MC_P			"07" 	/* Impression pour l’accepteur uniquement		*/
#define CB2A_F031_TAG_MC_D			"08" 	/* Affichage pour l’accepteur uniquement		*/
#define CB2A_F031_TAG_MC_D_P		"09" 	/* Impression et affichage pour l’accepteur		*/
#define CB2A_F031_TAG_CH_MC_P		"0A" 	/* Impression, Accepteur et porteur				*/
#define CB2A_F031_TAG_CH_MC_D		"0B" 	/* Affichage, Accepteur et porteur				*/
#define CB2A_F031_TAG_CH_MC_P_D		"0C" 	/* Impression et affichage, Accepteur et porteur	*/



#define	CB2A_F039_FORMAT_ERROR				"1004"
#define CB2A_F039_REJECT					"1000"
#define CB2A_F039_LEN						4


#define CB2A_F044_TAG_FIELD_ERROR			"AA"			/* Champ erroné												*/
#define CB2A_F044_TAG_SECURITY_ERROR		"AB"			/* Erreur de sécurité										*/
#define CB2A_F044_TAG_FIELD_CONVERSION		"AC"			/* Conversion de champ										*/
#define CB2A_F044_TAG_RESPONSE_REASON		"AD"			/* Précision de la cause du refus ou de l’accord			*/
#define CB2A_F044_TAG_DLG_OPEN_RESP_CODE 	"AE"			/* Code réponse de l'ouverture de dialogue et service		*/
#define CB2A_F044_TAG_CALL_SRV_CODE 		"AF"			/* Code activation appel ou service							*/
#define CB2A_F044_TAG_TR_MNG_CODE 			"AH"			/* Code gestion de transfert								*/
#define CB2A_F044_TAG_DLG_MNG_CODE 			"AI"			/* Code gestion de dialogue									*/
#define CB2A_F044_TAG_INC_MNG_CODE 			"AJ"			/* Code gestion d’incident									*/
#define CB2A_F044_TAG_MAX_AMOUNT			"BA"			/* Montant maximum pour lequel une autorisation pourra être délivrée */
#define CB2A_F044_TAG_PHONE_NBR				"BB"			/* Numéro de téléphone										*/
#define CB2A_F044_TAG_MSG_TO_TRX_SENDER		"BC"			/* Message à destination de l’initiateur de la transaction	*/
#define CB2A_F044_TAG_ISS_CALL_ACQ_MNG 		"BE"			/* Appel émetteur géré par l'acquéreur						*/
#define CB2A_F044_TAG_MAG_CRYPTO_CTRL		"CA"			/* Informations relatives au traitement du cryptogramme données piste ou équivalentes */
#define CB2A_F044_TAG_ICC_CRYPTO_CTRL		"CB"			/* Informations relatives au contrôle du cryptogramme application puce */


#define CB2A_F044_AJ_SYNC_LAST_EXCHG		'1'				/* Synchronisation sur dernier échange			*/
#define CB2A_F044_AJ_SYNC_CLOSED_DLG		'2'				/* Synchronisation en état « dialogue fermé »	*/
#define CB2A_F044_AJ_INC_TNR				"01"			/* Incident timer (Expiration du TNR)			*/
#define CB2A_F044_AJ_INC_TGR				"02"			/* Incident timer (Expiration du TGR)			*/
#define CB2A_F044_AJ_INC_TSI				"03"			/* Incident timer (Expiration du TSI)			*/
#define CB2A_F044_AJ_INC_TSM				"04"			/* Incident timer (Expiration du TSM)			*/
#define CB2A_F044_AJ_CIN_ERROR				"11"			/* Erreur de cinématique						*/
#define CB2A_F044_AJ_MSG_ERROR				"12"			/* Erreur de syntaxe/sémantique					*/
#define CB2A_F044_AJ_INC_TRSF				"20"			/* Incident en phase de transfert				*/


#define CB2A_F044_AE_IDENT_AUTH_OK				"10"		/* Identification et authentification correctes	*/
#define CB2A_F044_AE_IDENT_OK					"11"		/* Identification correcte						*/
#define CB2A_F044_AE_AUTH_OK					"12"		/* Authentification correcte					*/
#define CB2A_F044_AE_BILL_MODE_NOK				"13"		/* Mode de facturation non correcte - facturation forcée par l’acquéreur */
#define CB2A_F044_AE_IDENT_AUTH_NOK				"20"		/* Identification et authentification incorrectes	*/
#define CB2A_F044_AE_IDENT_NOK					"21"		/* Identification incorrecte					*/
#define CB2A_F044_AE_AUTH_NOK					"22"		/* Authentification incorrecte					*/
#define CB2A_F044_AE_UNKNOWN_APP				"23"		/* Application non reconnue						*/
#define CB2A_F044_AE_UNKNOWN_FUNCTION			"25"		/* Type de fonction non reconnue sur le système acquéreur	*/
#define CB2A_F044_AE_UV_SERVICE					"30"		/* Service momentanément indisponible			*/
#define CB2A_F044_AE_OTHERS						"31"		/* Autres motifs de refus						*/

#define CB2A_F044_AF_NO_ACTIVATION			'1'				/* Aucune activation d’appel ou service demandé	*/
#define CB2A_F044_AF_ACTIVATE_TLP			'2'				/* Activer Téléparamétrage						*/
#define CB2A_F044_AF_ACTIVATE_TLC			'3'				/* Activer Télécollecte							*/
#define CB2A_F044_AF_ACTIVATE_DL			'4'				/* Activer Téléchargement						*/


#define CB2A_F044_AH_OK						"00"			/* Prise en compte correcte						*/
#define CB2A_F044_AH_CAPACITY_EXCEEDED		"01"			/* Dépassement de capacité						*/
#define CB2A_F044_AH_UNSUPPORTED_FILE_TYPE	"02"			/* Type de fichier non supporté					*/
#define CB2A_F044_AH_INCORRECT_VERSION_NO	"03"			/* Numéro de version incorrect					*/
#define CB2A_F044_AH_INVALID_FILE			"04"			/* Fichier invalide ; autres motifs				*/
#define CB2A_F044_AH_MULTIPLE_ERRORS		"12"			/* Trop d’erreurs pendant le transfert			*/
#define CB2A_F044_AH_FORMAT_ERROR			"13"			/* Erreur de format sur les enregistrements reçus			*/
#define CB2A_F044_AH_EXCEEDED_NB_MSG		"14"			/* Nombre de messages transférés supérieur à celui annoncé	*/
#define CB2A_F044_AH_MISSING_MSG			"15"			/* Nombre de messages transférés inférieur à celui annoncé	*/
#define CB2A_F044_AH_UNSUPPORTED_CHECKSUM	"20"			/* Type de calcul d’intégrité non supporté		*/
#define CB2A_F044_AH_INCORRECT_CHECKSUM		"21"			/* Valeur d’intégrité incorrecte				*/
#define CB2A_F044_AH_UNSUPPORTED_CS_VAL		"22"			/* Vérification d’intégrité non supportée		*/


#define CB2A_F046_TAG_ITP 				"DF50"		/* ITP (Identifiant de l'application terminal)				*/
#define CB2A_F046_TAG_ACC_SYS_NO 		"DF51"		/* Numéro logique du système d’acceptation					*/
#define CB2A_F046_TAG_ACC_SYS_ARCH_TYPE "DF52"		/* Type d’architecture du système d'acceptation				*/
#define CB2A_F046_TAG_APP_ACTIV_CODE 	"DF53"		/* Code activation application								*/
#define CB2A_F046_TAG_APP_STATUS 		"DF54"		/* Statut de l'application									*/
#define CB2A_F046_TAG_SA_REF_COMP_DATA 	"DF55" 		/* Compléments de données de référence du système d'acceptation		*/
#define CB2A_F046_TAG_FUNC_STATE_PARAMS "DF58"		/* Paramètres de l'état fonctionnel							*/
#define CB2A_F046_TAG_PA_NO 			"DF5B"		/* Numéro logique du point d’acceptation					*/
#define CB2A_F046_TAG_IDPA 				"DF5C"		/* IDPA														*/
#define CB2A_F046_TAG_SIRET 			"DF5D"		/* SIRET													*/
#define CB2A_F046_TAG_IDSA 				"DF5E"		/* IDSA														*/
#define CB2A_F046_TAG_ACC_CONTRACT_NO 	"DF5F"		/* Numéro de contrat accepteur								*/
#define CB2A_F046_TAG_LAST_SERVICE_DT 	"DF60"		/* Dernière date/heure de mise en oeuvre des différents services (TLC, TLP,DL)		*/
#define CB2A_F046_TAG_REMITTANCE_NO 	"DF61"		/* Code regroupement du remettant							*/
#define CB2A_F046_TAG_SA_CONN_MODE 		"DF65" 		/* Mode de raccordement courant du système d'acceptation			*/
#define CB2A_F046_TAG_ACC_SYS_REF_DATA 	"DF66"		/* Données de référence du système d’acceptation			*/
#define CB2A_F046_TAG_PA_REF_DATA 		"DF67"		/* Données de référence du point d’acceptation				*/


#define CB2A_F046_DF53_DEACTIVATE	'0'				/* Désactiver	*/
#define CB2A_F046_DF53_ACTIVATE		'1'				/* Activer		*/



#define CB2A_F047_TAG_AUT_REASON 			"01"	/* Raison de la demande d'autorisation							*/
#define CB2A_F047_TAG_TRX_ENV 				"02"	/* Environnement réglementaire / technique de la transaction	*/
#define CB2A_F047_TAG_NOTE_TYPE 			"03"	/* Type de facture /procédure									*/
#define CB2A_F047_TAG_LOCAL_TRX_DATE 		"04"	/* Date et heure locale de la demande d’autorisation			*/
#define CB2A_F047_TAG_TRX_FORCING_CODE 		"05"	/* Code forçage d’une transaction en autorisation				*/
#define CB2A_F047_TAG_SITE_TYPE 			"06"	/* Type de site													*/
#define CB2A_F047_TAG_TRX_YEAR 				"07"	/* Année de la transaction										*/
#define CB2A_F047_TAG_FAILED_TLC_TRX_NBR 	"09"	/* Nombre de transactions non abouties d'une remise				*/
#define CB2A_F047_TAG_SA_GEN_TRX_NO 		"10"	/* Numéro de la transaction généré par le système d’acceptation	*/
#define CB2A_F047_TAG_TIMESTAMPING_DATA 	"14"	/* Données d’horodatage											*/
#define CB2A_F047_TAG_CRD_APP_START_VAL_DATE 	"15"	/* Date de début de validité de l’application carte			*/
#define CB2A_F047_TAG_FUNC_PROTO_SUPPORTED 		"16"	/* Fonctions de protocole supportées						*/
#define CB2A_F047_TAG_CVV2 					"17"	/* Cryptogramme visuel											*/
#define CB2A_F047_TAG_SA_COUNTRY_CODE 		"18"	/* Code pays du système d’acceptation							*/
#define CB2A_F047_TAG_IBAN 					"19"	/* IBAN															*/
#define CB2A_F047_TAG_FILE_NO 				"24"	/* Numéro de dossier											*/
#define CB2A_F047_TAG_TECHNICAL_REJECT 		"47"	/* Rejet technique												*/
#define CB2A_F047_TAG_TASA 					"48"	/* TASA (Type d'Applicatif du Système d’Acceptation)			*/
#define CB2A_F047_TAG_ACQ_OUTSTANDING_REF 	"49"	/* Référence impayé de l'acquéreur								*/


#define CB2A_F058_TAG_TVR 				"9500"		/* Terminal verification results						*/
#define CB2A_F058_TAG_TRX_TYPE 			"9C00" 		/* Transaction type										*/
#define CB2A_F058_TAG_TERM_CAPABILTIES 	"9F33"		/* Terminal capabilities								*/
#define CB2A_F058_TAG_CVMR 				"9F34"		/* Cardholder verification method results				*/
#define CB2A_F058_TAG_TERMINAL_TYPE 	"9F35"		/* Terminal type										*/
#define CB2A_F058_TAG_TERMINAL_UN 		"9F37"		/* Unpredictable number terminal						*/
#define CB2A_F058_TAG_HANDLE_TYPE 		"DF30"		/* Type de prise en compte								*/
#define CB2A_F058_TAG_STAMP 			"DF31"		/* Sceau												*/
#define CB2A_F058_TAG_STAMP_TYPE 		"DF32"		/* Type de sceau										*/
#define CB2A_F058_TAG_TAC_DEFAULT 		"FF0D"		/* Terminal action code default							*/
#define CB2A_F058_TAG_TAC_DENIAL 		"FF0E"		/* Terminal action code denial							*/
#define CB2A_F058_TAG_TAC_ONLINE 		"FF0F"		/* Terminal action code on-line							*/
#define CB2A_F058_TAG_REMITTANCE_LOC_TIME 	"FF20"	/* Heure locale de traitement de la remise				*/
#define CB2A_F058_TAG_REMITTANCE_LOC_DATE 	"FF21"	/* Date locale de traitement de la remise				*/
#define CB2A_F058_TAG_VALUE_DATE 		"FF22"		/* Date de valeur										*/
#define CB2A_F058_TAG_COM_AMOUNT 		"FF23"		/* Montant de la commission								*/
#define CB2A_F058_TAG_COM_AMOUNT_CV 	"FF24"		/* Contre valeur du montant de la commission			*/
#define CB2A_F058_TAG_NET_RECAP_AMOUNT 	"FF25"		/* Montant net récapitulatif							*/
#define CB2A_F058_TAG_NET_RECAP_AMOUNT_CV 	"FF26"	/* Contre valeur du montant net récapitulatif			*/
#define CB2A_F058_TAG_CV_CURRENCY_CODE 	"FF27"		/* Code monnaie ou devise de la contre valeur			*/
#define CB2A_F058_TAG_ACQ_COM_TOTAL_MNT "FF28"		/* Montant total des commissions dues par l'acquéreur à l'accepteur		*/
#define CB2A_F058_TAG_ACC_COM_TOTAL_MNT "FF29"		/* Montant total des commissions dues par l'accepteur à l'acquéreur		*/
#define CB2A_F058_TAG_TRX_AMOUNT_CV 	"FF30"		/* Contre valeur du montant de la transaction			*/
#define CB2A_F058_TAG_CONVERSION_RATE 	"FF31"		/* Taux de conversion									*/
#define CB2A_F058_TAG_REQ_DOC_TYPE 		"FF32"		/* Type de document demandé								*/
#define CB2A_F058_TAG_DOC_INIT_REQ_DATE "FF33"		/* Date de la demande initiale de document				*/
#define CB2A_F058_TAG_DOC_REQ_AUDIT_NO 	"FF34"		/* Numéro d'audit de la demande de document				*/
#define CB2A_F058_TAG_DD_RCVR_TRX_NBR 	"FF35"		/* Nombre de transactions de débit en différé de recouvrement	*/
#define CB2A_F058_TAG_DD_RCVR_TRX_MNT 	"FF36"		/* Montant des transactions de débit en différé de recouvrement	*/
#define CB2A_F058_TAG_OUTSTANDING_IMPUTATION 		"FF37"	/* Imputation de l'impayé						*/
#define CB2A_F058_TAG_OUTSTANDING_IMPUTATION_MNT 	"FF38" 	/*Montant de l'impayé imputé					*/
#define CB2A_F058_TAG_UI_CURRENCY_CODE 	"FF39" 		/*Code monnaie du montant de l'impayé imputé			*/
#define CB2A_F058_TAG_OUTSTANDING_CODE 	"FF40"		/* Code impayé											*/
#define CB2A_F058_TAG_CH_CUM_AMOUNT 	"FF42"		/* Montant cumulé par porteur							*/
#define CB2A_F058_TAG_AUTH_ID_LEN 		"FF43"		/* Longueur du numéro d’autorisation					*/
#define CB2A_F058_TAG_OS_I_CV_MNT 		"FF44"		/* Contre valeur du montant de l'impayé imputé			*/
#define CB2A_F058_TAG_ECOM_CRYPT 		"FF44"		/* Contre valeur du montant de l'impayé imputé			*/
#define CB2A_F058_TAG_TL_MANAGER_ID 	"FF45"		/* Identification du gestionnaire de télépaiement		*/
#define CB2A_F058_TAG_ACCEPT_CERT 		"FF46"		/* Certificat d'acceptation								*/
#define CB2A_F058_TAG_ECOM_TRX_SEC_TYPE "FF47"		/* Type de sécurisation de transaction de commerce électronique		*/
#define CB2A_F058_TAG_TRANS_STAIN        "FF49"		/* TransStain		*/
#define CB2A_F058_TAG_SUPPORT_TYPE 		"FF52"		/* Type de support										*/
#define CB2A_F058_TAG_EXT_TRANS_MNG 	"FF53"		/* Gestion étendue des transferts						*/


#define CB2A_FILE_ID_CURRENCY				"01"	/* Monnaie ou devise								*/
#define CB2A_FILE_ID_CH_MSG 				"02"	/* Message porteur									*/
#define CB2A_FILE_ID_ACC_MSG 				"03"	/* Message Accepteur								*/
#define CB2A_FILE_ID_ACC_PARAM 				"04"	/* Paramètres accepteur								*/
#define CB2A_FILE_ID_CH_NOTE_EDIT_PARAM 	"05"	/* Edition ticket porteur							*/
#define CB2A_FILE_ID_REPORT_NOTE_EDIT_PARAM "06"	/* Edition ticket compte rendu						*/
#define CB2A_FILE_ID_APP_PARAM 				"07"	/* Paramètres applicatifs							*/
#define CB2A_FILE_ID_CALL_PARAM 			"08"	/* Appel											*/
#define CB2A_FILE_ID_EMV_AID_DATA_LIST 		"09"	/* Liste des données spécifiques EMV par AID		*/
#define CB2A_FILE_ID_CB_PUB_AUTH_KEY 		"10"	/* Liste des clés publiques d’authentification CB	*/
#define CB2A_FILE_ID_EMV_DOL 				"11"	/* Liste des DOL EMV								*/
#define CB2A_FILE_ID_ACQ_RISK 				"12"	/* Risque acquéreur									*/
#define CB2A_FILE_ID_CH_CTRL_LIST 			"13"	/* Liste de contrôle de numéros de carte porteur	*/
#define CB2A_FILE_ID_CB_APPROVED_BIN_LIST 	"14"	/* Liste de BINs agrées CB							*/
#define CB2A_FILE_ID_EMV_TAC_LIST 			"16"	/* Liste des TAC EMV								*/
#define CB2A_FILE_ID_OTHER_CURRENCY 		"17"	/* Autre monnaie ou devise							*/
#define CB2A_FILE_ID_GMT_TIMESTAMPING_PARAM "18"	/* Paramètres d’horodatage GMT						*/
#define CB2A_FILE_ID_EMV_PUB_AUTH_KEY_LIST 	"19"	/* Liste des clés publiques d'authentification EMV	*/
#define CB2A_FILE_ID_EMV_AID_LIST 			"20"	/* Liste des AID EMV								*/
#define CB2A_FILE_ID_TELECOM_TLC 			"21"	/* Télécommunication télécollecte					*/
#define CB2A_FILE_ID_TELECOM_TLP 			"22"	/* Télécommunication téléparamétrage				*/
#define CB2A_FILE_ID_TELECOM_AUT 			"23"	/* Télécommunication autorisation					*/
#define CB2A_FILE_ID_TELECOM_DL_ENV_1 		"25"	/* Télécommunication téléchargement env. 1			*/
#define CB2A_FILE_ID_TELECOM_DL_ENV_2 		"26"	/* Télécommunication téléchargement env. 2			*/
#define CB2A_FILE_ID_EMV_RND_CALL_TABLE 	"27"	/* Table d’appel aléatoire EMV						*/
#define CB2A_FILE_ID_ACC_PS_ID 				"28"	/* Identifiant pseudo-session de l’accepteur de carte	*/
#define CB2A_FILE_ID_EMV_COMP_TLC_DATA 		"29"	/* Liste des données complémentaires EMV pour télécollecte	*/
#define CB2A_FILE_ID_ACC_PS_ID_ADD			"32" 	/* Identifiant pseudo-session étendu de l'accepteur de carte 	*/
#define CB2A_FILE_ID_LEN					2


#define CB2A_FILE_ID_NO_NO_CURRENCY				1
#define CB2A_FILE_ID_NO_CH_MSG 					2
#define CB2A_FILE_ID_NO_ACC_MSG 				3
#define CB2A_FILE_ID_NO_ACC_PARAM 				4
#define CB2A_FILE_ID_NO_CH_NOTE_EDIT_PARAM 		5
#define CB2A_FILE_ID_NO_REPORT_NOTE_EDIT_PARAM 	6
#define CB2A_FILE_ID_NO_APP_PARAM 				7
#define CB2A_FILE_ID_NO_CALL_PARAM 				8
#define CB2A_FILE_ID_NO_EMV_AID_DATA_LIST 		9
#define CB2A_FILE_ID_NO_CB_PUB_AUTH_KEY 		10
#define CB2A_FILE_ID_NO_EMV_DOL 				11
#define CB2A_FILE_ID_NO_ACQ_RISK 				12
#define CB2A_FILE_ID_NO_CH_CTRL_LIST 			13
#define CB2A_FILE_ID_NO_CB_APPROVED_BIN_LIST 	14
#define CB2A_FILE_ID_NO_EMV_TAC_LIST 			16
#define CB2A_FILE_ID_NO_OTHER_CURRENCY 			17
#define CB2A_FILE_ID_NO_GMT_TIMESTAMPING_PARAM 	18
#define CB2A_FILE_ID_NO_EMV_PUB_AUTH_KEY_LIST 	19
#define CB2A_FILE_ID_NO_EMV_AID_LIST 			20
#define CB2A_FILE_ID_NO_TELECOM_TLC 			21
#define CB2A_FILE_ID_NO_TELECOM_TLP 			22
#define CB2A_FILE_ID_NO_TELECOM_AUT 			23
#define CB2A_FILE_ID_NO_TELECOM_DL_ENV_1 		25
#define CB2A_FILE_ID_NO_TELECOM_DL_ENV_2 		26
#define CB2A_FILE_ID_NO_EMV_RND_CALL_TABLE 		27
#define CB2A_FILE_ID_NO_ACC_PS_ID 				28
#define CB2A_FILE_ID_NO_EMV_COMP_TLC_DATA 		29
#define CB2A_FILE_ID_NO_ACC_PS_ID_ADD			32
#define CB2A_FILE_ID_LAST						CB2A_FILE_ID_NO_ACC_PS_ID_ADD

#define CB2A_F072_TAG_CURRENCY 					"DF01"	/* Paramètres de la monnaie ou devise					*/
#define CB2A_F072_TAG_CH_MSG_PARAM 				"DF02"	/* Paramètres messages porteur							*/
#define CB2A_F072_TAG_ACC_MSG_PARAM 			"DF03"	/* Paramètres messages accepteur						*/
#define CB2A_F072_TAG_ACC_PARAM 				"DF04"	/* Paramètres accepteur									*/
#define CB2A_F072_TAG_CH_NOTE_EDIT_PARAM 		"DF05"	/* Paramètres d'édition du ticket porteur				*/
#define CB2A_F072_TAG_REPORT_NOTE_EDIT_PARAM 	"DF06"	/* Paramètres d'édition du ticket compte-rendu			*/
#define CB2A_F072_TAG_APP_PARAM 				"DF07"	/* Paramètres applicatifs								*/
#define CB2A_F072_TAG_CALL_PARAM 				"DF08"	/* Paramètres d'appel									*/
#define CB2A_F072_TAG_GMT_STAMPING_PARAM 		"DF09"	/* Paramètres d’horodatage GMT							*/
#define CB2A_F072_TAG_CB_PUB_KEY_LIST_PARAM 	"DF0A"	/* Paramètres liste des clés publiques CB				*/
#define CB2A_F072_TAG_OTHER_CURRENCY_PARAM 		"DF0B"	/* Paramètres autre monnaie ou devise					*/
#define CB2A_F072_TAG_ACQ_RISK_PARAM 			"DF0C"	/* Paramètres risques acquéreur/code numérique de la monnaie ou devise	*/
#define CB2A_F072_TAG_TELECOM_TLC_PARAM 		"DF10"	/* Paramètres télécommunication télécollecte			*/
#define CB2A_F072_TAG_TELECOM_TLP_PARAM 		"DF11"	/* Paramètres télécommunication téléparamétrage			*/
#define CB2A_F072_TAG_TELECOM_AUT_PARAM 		"DF12"	/* Paramètres télécommunication autorisation			*/
#define CB2A_F072_TAG_TELECOM_WRAP_DL_PARAM_1 	"DF14"	/* Paramètres télécommunication téléchargement enveloppe 1	*/
#define CB2A_F072_TAG_TELECOM_WRAP_DL_PARAM_2 	"DF15"	/* Paramètres télécommunication téléchargement enveloppe 2	*/
#define CB2A_F072_TAG_EMV_PUB_KEY_LIST_PARAM 	"DF16"	/* Paramètres liste des clés publiques d'authentification EMV	*/
#define CB2A_F072_TAG_EMV_AID_LIST_PARAM 		"DF17"	/* Paramètres liste des AID EMV							*/
#define CB2A_F072_TAG_EMV_TAC_LIST 				"DF18"	/* Liste des TAC EMV									*/
#define CB2A_F072_TAG_EMV_AID_DATA_LIST 		"DF19"	/* Liste des données spécifiques EMV par AID			*/
#define CB2A_F072_TAG_EMV_DFLT_DOL 				"DF1A"	/* DOL EMV par défaut pour une application carte EMV	*/
#define CB2A_F072_TAG_EMV_RND_CALL_PARAM 		"DF1B"	/* Paramètres d’appel aléatoire EMV						*/
#define CB2A_F072_TAG_ACC_PS_ID 				"DF1C"	/* Identifiant pseudo-session de l’accepteur de carte	*/
#define CB2A_F072_TAG_CARD_CTRL_LIST_PARAM 		"DF1D"	/* Paramètres liste de contrôle des numéros de carte porteur	*/
#define CB2A_F072_TAG_CB_APPROVED_BIN_PARAM 	"DF1E"	/* Paramètres liste des BIN agréés CB							*/
#define CB2A_F072_TAG_EMV_COMP_TLC_DATA 		"DF1F"	/* Liste des données complémentaires EMV pour télécollecte		*/
#define CB2A_F072_TAG_PA_BILLING_MODE 			"DF20"	/* Paramètres accepteur / Mode facturation télécom				*/
#define CB2A_F072_TAG_PA_ACTIVATION_MODE 		"DF21"	/* Paramètres accepteur / Code activation mode appelé			*/
#define CB2A_F072_TAG_PA_SIRET 					"DF22"	/* Paramètres accepteur / SIRET							*/
#define CB2A_F072_TAG_PA_SITE_TYPE 				"DF23"	/* Paramètres accepteur / Type de site					*/
#define CB2A_F072_TAG_OPERATING_CALL_PARAM 		"DF24"	/* Paramètres d’appel / Fonctionnement					*/
#define CB2A_F072_TAG_CB_RSA_KEY_MODULO 		"DF25"	/* Paramètres liste des clés publiques CB / Modulo clé RSA		*/
#define CB2A_F072_TAG_PQ_MIN_AMOUNT 			"DF26"	/* Paramètres risque acquéreur / Montant minimum accepté		*/
#define CB2A_F072_TAG_PQ_MAX_AMOUNT 			"DF27"	/* Paramètres risque acquéreur / Montant maximum accepté		*/
#define CB2A_F072_TAG_PRQ_DA_AMOUNT 			"DF28"	/* Paramètres risque acquéreur / Montant utilisé de double authentification porteur	*/
#define CB2A_F072_TAG_PRQ_COM_SRV_VALUE 		"DF2A"	/* Paramètres risque acquéreur / Valeur du service commercial	*/
#define CB2A_F072_TAG_PRQ_AUT_AMOUNT 			"DF2B"	/* Paramètres risque acquéreur / Montant d’autorisation			*/
#define CB2A_F072_TAG_DLP1_ACCESS_CTRL 			"DF2C"	/* Paramètres téléchargement enveloppe 1 / */
														/* Elément de contrôle d’accès au système acquéreur de téléchargement	*/
#define CB2A_F072_TAG_DLP1_SW_REF 				"DF2D"	/* Paramètres téléchargement enveloppe 1 / Référence du logiciel à télécharger		*/
#define CB2A_F072_TAG_DLP2_SA_NO 				"DF2E"	/* Paramètres téléchargement enveloppe 2 / Paramètre numéro logique du système d’acceptation	*/
#define CB2A_F072_TAG_EMV_PUB_KEY 				"DF2F"	/* Paramètres liste clés publiques d’authentification EMV / Modulo clé RSA			*/
#define CB2A_F072_TAG_PRQ_MAX_INT_AMNT 			"DF3A"	/* Paramètre risque acquéreur/ Montant maximum accepté pour une carte étrangère sur automate	*/
#define CB2A_F072_TAG_ACQ_TELECOM_PARAM 		"DF41" 	/* Paramètres de télécommunication acquéreur X		*/
#define CB2A_F072_TAG_CALL_PARAM_ADD			"DF43" 	/* Paramètres d'appel								*/
#define CB2A_F072_TAG_WRAP_DL_PARAM_2 			"DF44" 	/* Paramètres de téléchargement enveloppe 2			*/
#define CB2A_F072_TAG_ACC_PS_ID_ADD 			"DF45"	/* Identifiant pseudo-session étendu de l'accepteur de carte	*/






#define CB2A_F118_TAG_NORMALIZED_ADDR 	"01"	/* Adresse normalisée					*/
#define CB2A_F118_TAG_PHONE_NUM 		"02"	/* Numéro de téléphone					*/
#define CB2A_F118_TAG_FAX_NUM 			"03"	/* Numéro de fax						*/
#define CB2A_F118_TAG_E_ADDR 			"04"	/* Adresse électronique					*/
#define CB2A_F118_TAG_DOC_REQ_REASON 	"05"	/* Motif de la demande de document		*/

/******************************************************************************\
*
*		Autorisation
*
\******************************************************************************/

#define CB2A_PC_PURCHASE				"00"			/* Achat de biens ou services				*/
#define CB2A_PC_ATM_WITHDRAWAL			"10"			/* Retrait sur automate (DAB/GAB)			*/
#define CB2A_PC_QUASICASH				"11"			/* Quasi-cash								*/
#define CB2A_PC_CAPTURE_CARD			"14"			/* Capture de carte							*/
#define CB2A_PC_WITHDRAWAL				"17"			/* Retrait au guichet						*/
#define CB2A_PC_CREDIT					"20"			/* Crédit									*/
#define CB2A_PC_QUASICASH_REFUND		"28"			/* Retour Quasi-cash						*/
#define CB2A_PC_FUNDS_CONSULT			"30"			/* Interrogation sur disponibilité de fonds */
#define CB2A_PC_CARD_RETURN				"37"			/* Restitution de carte						*/
#define CB2A_PC_PME						"81"			/* Autorisation de chargement PME			*/


#define CB2A_AUT_F039_FORMAT_ERROR				"30"			/* erreur de format									*/


#define CB2A_AUT_F044_TAG_FIELD_ERROR			"AA"			/* Champ erroné												*/
#define CB2A_AUT_F044_TAG_SECURITY_ERROR		"AB"			/* Erreur de sécurité										*/
#define CB2A_AUT_F044_TAG_FIELD_CONVERSION		"AC"			/* Conversion de champ										*/
#define CB2A_AUT_F044_TAG_RESPONSE_REASON		"AD"			/* Précision de la cause du refus ou de l’accord			*/
#define CB2A_AUT_F044_TAG_CALL_SRV_CODE 		"AF"			/* Code activation appel ou service							*/
#define CB2A_AUT_F044_TAG_MAX_AMOUNT			"BA"			/* Montant maximum pour lequel une autorisation pourra être délivrée */
#define CB2A_AUT_F044_TAG_PHONE_NBR				"BB"			/* Numéro de téléphone										*/
#define CB2A_AUT_F044_TAG_MSG_TO_TRX_SENDER		"BC"			/* Message à destination de l’initiateur de la transaction	*/
#define CB2A_AUT_F044_TAG_IB_NUMBER 			"BD"			/* Numéro d’IB												*/
#define CB2A_AUT_F044_TAG_MAG_CRYPTO_CTRL		"CA"			/* Informations relatives au traitement du cryptogramme données piste ou équivalentes */
#define CB2A_AUT_F044_TAG_ICC_CRYPTO_CTRL		"CB"			/* Informations relatives au contrôle du cryptogramme application puce */


#define CB2A_AUT_F044_TAG_FE_VALUE_ERROR		"1"				/* Erreur de valeur											*/
#define CB2A_AUT_F044_TAG_FE_FORMAT_ERROR		"2"				/* Erreur de format											*/
#define CB2A_AUT_F044_TAG_FE_MISSING_FIELD		"3"				/* Absence injustifiée d’un champ							*/


#define CB2A_AUT_F047_TAG_POS_DATA_CODE 	"03"	/* Code de données du point de service					*/
#define CB2A_AUT_F047_TAG_SITE_TYPE 		"08"	/* Type de site											*/
#define CB2A_AUT_F047_TAG_ACQ_FRD_CONV 		"20"	/* Conversion de champ par acquéreur (chp 32) ou transmetteur (chp 33)	*/
/*EBE161028: Tag 24 is not part of CB2A AUT V1.1.2
#define CB2A_AUT_F047_TAG_NUM_DOSS	 		"24"	*//* Numéro dossier	*/
#define CB2A_AUT_F047_TAG_F22_ST_CHG 		"71"	/* Code de changement d’état du champ 22 (Mastercard, EPI)				*/
#define CB2A_AUT_F047_TAG_MAG_STRIP_CTRL 	"72"	/* Information sur contrôle intégrité piste magnétique (Mastercard, EPI)	*/
#define CB2A_AUT_F047_TAG_SIRET 			"96"	/* SIRET												*/
#define CB2A_AUT_F047_TAG_IDPA 				"97"	/* IDPA													*/
#define CB2A_AUT_F047_TAG_IDSA 				"A0"	/* IDSA													*/




#define CB2A_AUT_F053_TAG_SEC_DATA					0			/* Données de sécurité utilisées							*/
#define CB2A_AUT_F053_TAG_POS_VERIF					1			/* Vérifications effectuées par le demandeur	*/
#define CB2A_AUT_F053_TAG_AUTH_DATA_ENC_MODE		2			/* Mode de chiffrement des données d'authentification		*/
#define CB2A_AUT_F053_TAG_PINKEY_ENC_MODE			3			/* Mode de chiffrement du code confidentiel ou de la clé	*/
#define CB2A_AUT_F053_TAG_PIN_FORMAT				4			/* Format du code confidentiel								*/
#define CB2A_AUT_F053_TAG_PINKEY_KEY_ID				6			/* Clé de chiffrement du code confidentiel/Clé de chiffrement de la clé transportée */
#define CB2A_AUT_F053_TAG_UPDATE_KEY_ID				8			/* Clé de chiffrement à modifier							*/
#define CB2A_AUT_F053_TAG_RESERVED_1				10
#define CB2A_AUT_F053_TAG_MAC_KEY					12			/* Clé de chiffrement pour la sécurisation des messages		*/
#define CB2A_AUT_F053_TAG_UPDATE_KEY_CODE			14			/* Code de mise à jour de clé								*/
#define CB2A_AUT_F053_TAG_UPDATE_KEY_TYPE			15			/* Type de clé mise à jour									*/


#define		CB2A_AUT_F055_TAG_TRACK2				"5700"
#define		CB2A_AUT_F055_TAG_ISS_SCRIPT1			"7100"
#define		CB2A_AUT_F055_TAG_ISS_SCRIPT2			"7200"
#define		CB2A_AUT_F055_TAG_APP_INTER_PROFILE		"8200"
#define		CB2A_AUT_F055_TAG_ISS_AUTHEN_DATA		"9100"
#define		CB2A_AUT_F055_TAG_TVR					"9500"
#define		CB2A_AUT_F055_TAG_TRANS_TYPE			"9C00"
#define		CB2A_AUT_F055_TAG_APP_EXPIRY_DATE		"5F24"
#define		CB2A_AUT_F055_TAG_OTHER_AMOUNT			"9F03"
#define		CB2A_AUT_F055_TAG_APP_SELECTED			"9F06"
#define		CB2A_AUT_F055_TAG_ISS_APP_DATA			"9F10"
#define		CB2A_AUT_F055_TAG_APP_CRYPTOGRAM		"9F26"
#define		CB2A_AUT_F055_TAG_CRYPTO_INFO_DATA		"9F27"
#define		CB2A_AUT_F055_TAG_TERM_CAP_PROFILE		"9F33"
#define		CB2A_AUT_F055_TAG_ATC					"9F36"
#define		CB2A_AUT_F055_TAG_UNPRED_NUMBER			"9F37"
#define		CB2A_AUT_F055_TAG_PROC_RESULTS			"DF80"
#define		CB2A_AUT_F055_TAG_CARD_APP_TYPE			"DF81"

#define CB2A_AUT_F059_TAG_FUNCTION_CODE 		"0100"		/* Code fonction										*/
#define CB2A_AUT_F059_TAG_REASON_CODE 			"0101"		/* Code raison du message								*/
#define CB2A_AUT_F059_TAG_TRX_YEAR 				"0102"		/* Année de la transaction								*/
#define CB2A_AUT_F059_TAG_CARD_START_VAL_DATE 	"0103"		/* Date de début de validité de la carte				*/
#define CB2A_AUT_F059_TAG_TRX_ENV 				"0200"		/* Environnement réglementaire et technique de la transaction	*/
#define CB2A_AUT_F059_TAG_ITP 					"0201"		/* ITP (Identifiant de l’application Terminal)			*/
#define CB2A_AUT_F059_TAG_ACC_CONTRACT_NO 		"0202"		/* Numéro de contrat accepteur							*/
#define CB2A_AUT_F059_TAG_ACC_SYS_NO 			"0203"		/* Numéro logique du système d'acceptation				*/
#define CB2A_AUT_F059_TAG_ACC_POINT_NO 			"0204"		/* Numéro logique du point d'acceptation				*/
#define CB2A_AUT_F059_TAG_ACC_SYS_COUNTRY_CODE 	"0205"		/* Code pays du système d'acceptation					*/
#define CB2A_AUT_F059_TAG_ACTOR_TYPE 			"0206"		/* Type d'acteur										*/
#define CB2A_AUT_F059_TAG_CH_CUM_AMOUNT 		"0207"		/* Montant cumulé par porteur							*/
#define CB2A_AUT_F059_TAG_PRE_AUT_NOTE_VAL_PERIOD 	"0208"	/* Durée de validité d'une facture préautorisée			*/
#define CB2A_AUT_F059_TAG_FLOW_CTRL_RESULT 		"0209"		/* Résultat du contrôle de flux							*/
#define CB2A_AUT_F059_TAG_B4_B0_APP_ACTION 		"020A"		/* Action sur l'application carte B4-B0'				*/
#define CB2A_AUT_F059_TAG_TASA 					"020B"		/* Type d'applicatif du système d'acceptation (TASA)	*/
#define CB2A_AUT_F059_TAG_TAD_TECH_ID 			"020C"		/* Identifiant Technique du TAD							*/
#define CB2A_AUT_F059_TAG_CVV2 					"0300"		/* Cryptogramme visuel									*/
#define CB2A_AUT_F059_TAG_CVV2_CTRL_INFO 		"0301"		/* Informations relatives au traitement du cryptogramme visuel	*/
#define CB2A_AUT_F059_TAG_XID 					"0400"		/* XID													*/
#define CB2A_AUT_F059_TAG_TRANS_STAIN 			"0401"		/*	TransStain											*/
#define CB2A_AUT_F059_TAG_MC_SET_SERIAL_NO 		"0402"		/* Numéro de série du "certificat commerçant" SET		*/
#define CB2A_AUT_F059_TAG_CH_SET_SERIAL_NO 		"0403"		/* Numéro de série du "certificat porteur" SET			*/
#define CB2A_AUT_F059_TAG_ISS_NW_ID 			"0404"		/* Identification du réseau émetteur					*/
#define CB2A_AUT_F059_TAG_CERT_TYPE 			"0405"		/* Nature du certificat à délivrer						*/
#define CB2A_AUT_F059_TAG_CERT_DELIV_TYPE 		"0406"		/* Type de délivrance de certificat						*/
#define CB2A_AUT_F059_TAG_ECOM_SEC_TYPE 		"0407"		/* Type de sécurisation de transaction de commerce électronique	*/
#define CB2A_AUT_F059_TAG_CERT_VALIDITY 		"0408"		/* Validité du certificat								*/
/*EBE161028: Fields are not part of CB2A Aut 1.1.2
#define CB2A_AUT_F059_EC_CRYPTO_INFO			"0409"		/ * Informations relatives au traitement du cryptogramme de commerce électronique * /
#define CB2A_AUT_F059_VADS_USE_RESULT			"0412"		/ * Résultat de l'utilisation de l'architecture de paiement sécurisé VADS * /
#define CB2A_AUT_F059_TRX_SECURING				"0413"		/ * Sécurisation de la transaction * /
*/

#define CB2A_AUT_F059_TAG_ACCEPT_CERT 			"0500"		/* Certificat d’acceptation								*/
#define CB2A_AUT_F059_TAG_TL_MANAGER_ID 		"0501"		/* Identification du gestionnaire de télépaiement		*/
#define CB2A_AUT_F059_TAG_VISA_CPS_ATM_ACI 		"0600"		/* VISA CPS/ATM ACI										*/
#define CB2A_AUT_F059_TAG_VISA_CPS_ATM_TI 		"0601"		/* VISA CPS/ATM TI										*/
#define CB2A_AUT_F059_TAG_VISA_CPS_ATM_VC 		"0602"		/* VISA CPS/ATM VC										*/
/*EBE161028: Fields are not part of CB2A Aut 1.1.2
#define CB2A_AUT_F059_TAG_TYPE_FACTURE          "0800"		/ * Type Facture * / 
#define CB2A_AUT_F059_TAG_SERVICE_RISQUE        "0802"		/ * SERVICE RISQUE * /
#define CB2A_AUT_F059_TAG_ID_NETWORK_AUT        "0803"		/ * IDENTIFICATION DU RESEAU INTERNATIONAL D AUTORISATION * / 
#define CB2A_AUT_F059_TAG_SERVICE_OPTI_SUPP     "0805"		/ * Services Optionnels Supporte * /
*/


#define CB2A_AUT_F117_TAG_SL_REASON				"01"			/* Raison de mise en opposition				*/
#define CB2A_AUT_F117_TAG_REC_VAL_DATE			"02"			/* Date de validité de l'enregistrement		*/
#define CB2A_AUT_F117_TAG_RIB					"03" 			/* RIB										*/
#define CB2A_AUT_F117_TAG_CH_TITLE				"04"			/* Titre du porteur							*/
#define CB2A_AUT_F117_TAG_CARD_TYPE				"05"			/* Type de carte							*/
#define CB2A_AUT_F117_TAG_DECLARED_DATE			"06"			/* Date déclarée							*/
#define CB2A_AUT_F117_TAG_DATE_TIME				"07"			/* Date et heure de prise en compte			*/
#define CB2A_AUT_F117_TAG_CH_NAME				"08"			/* Nom du porteur							*/
#define CB2A_AUT_F117_TAG_PIN_LOST				"09"			/* Perte du code confidentiel				*/
#define CB2A_AUT_F117_TAG_ORIGIN				"10"			/* Top forçage et provenance				*/
#define CB2A_AUT_F117_TAG_LOST_COUNTRY			"11"			/* Code pays de la perte ou du vol			*/
#define CB2A_AUT_F117_TAG_LOST_DEP				"12"			/* Code département de la perte ou du vol	*/
#define CB2A_AUT_F117_TAG_ACK_REF				"21"			/* Référence de prise en compte d'opposition */



#endif

