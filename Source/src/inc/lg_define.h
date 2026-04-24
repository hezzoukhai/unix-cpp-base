#ifndef LOGIGAB_DEFINE___H
#define LOGIGAB_DEFINE___H

/* LISTE DES MESSAGES		*/
#define LG_FC_WAG_PROD						'A' /* Bloc wagons produits */
#define LG_FC_WAG_COM						'B' /* Bloc wagons commercial */
#define LG_FC_MIDDLE_REQ					'C' /* Requête intermédiaire et réponse à la requête */
#define LG_FC_INQ_REQ						'D' /* Requête interrogation et réponse à la requête */
#define LG_FC_OP_REPORT						'E' /* Compte rendu d’opération et réponse au compte rendu */
#define LG_FC_REENTRY_RESP					'F' /* Réponse à la requête de réentrée */
#define LG_FC_REMOTE_EXP_CMD_REPORT			'G' /* Compte rendu d’opération d’exploitation télécommandée et réponse au compte rendu */
#define LG_FC_LOCAL_EXP_CMD_REPORT			'H' /* Compte rendu d’opération d’exploitation locale et réponse au compte rendu */
#define LG_FC_INIT_TRN						'I' /* Requête de début de transaction client */
#define LG_FC_END_OF_TRN					'J' /* Requête de fin de transaction client */
#define LG_FC_SALES_COND					'K' /* Requête demande des conditions de vente et réponse à la requête */
#define LG_FC_EXCP_MSG						'L' /* Message exceptionnel */
#define LG_FC_PRINTER_PG_TRNSF_REQ			'M' /* Requête Transfert de pages pour imprimante large ou laser */
#define LG_FC_SCREEN_PG_TRNSF_REQ			'N' /* Requête Transfert de pages Ecran */
#define LG_FC_MONEO_CTX						'Q' /* Requête demande de contexte Monéo et réponse à la requête */
#define LG_FC_TOPUP_AUTH					'R' /* Requête demande d’autorisation de chargement et réponse à la requête */
#define LG_FC_CRYPTO_DL_REQ_2				'S' /* Requête demande de chargement des clés avec nouvelle cryptographie */
#define LG_FC_PERIODIC_MSG					'T' /* Requête périodique et réponse à la requête */
#define LG_FC_TERM_SPEC						'U' /* Requête caractéristiques de l’automate et réponse à la requête */
#define LG_FC_INIT_REQ						'V' /* Requête initiale et réponse à la requête */
#define LG_FC_AUTH_REQ						'W' /* Requête demande d’autorisation et réponse à la requête */
#define LG_FC_KEYS_DL						'X' /* Requête demande de chargement des clés et réponse à la requête */
#define LG_FC_ILS_REQ						'Y' /* Requête ILS et réponse à la requête */
#define LG_FC_TERM_SPEC_PARAM				'Z' /* Requête paramètres spécifiques de l’automate et réponse à la requête */
#define LG_FC_PRD_WAG_2						'0' /* Bloc wagons produits 2 */
#define LG_FC_PERSO_FILE_DL					'1' /* Requête de téléchargement des fichiers de la personnalisation */
#define LG_FC_WAG_MONEO						'2' /* Bloc Wagon Monéo */
#define LG_FC_WAG_EMV						'5' /* Bloc wagon EMV */
#define LG_FC_WAG_DEPOSIT_REPORT			'6' /* Bloc wagon compte rendu dépôt valorisé en liasse */
#define LG_FC_WAG_CASST						'7' /* Bloc wagon cassette */
#define LG_FC_MULTI_SALES_COND_INQ			'7' /* Requête demande des conditions de vente multiple et réponse à la requête */
#define LG_FC_MSG_ACK_2						'8' /* Acquittement des messages émis sur le raccordement secondaire */
#define LG_FC_EJ_TRNSF_REQ					'9' /* Requête transfert du journal électronique */
#define LG_FC_BAL_INQ						'>' /* Requête demande de solde et réponse à la requête */
#define LG_FC_AUTH_REQ_F_CRNCY				'!' /* Requête demande d’autorisation en devises et réponse à la requête */
#define LG_FC_INIT_TLP						'=' /* Requête début du télé paramétrage */
#define LG_FC_TBL_TRNSF						'?' /* Requête transfert de table */
#define LG_FC_END_OF_TLP					';' /* Requête fin du Télé Paramétrage */
#define LG_FC_LEN							1


/* LES OPERATIONS D'EXPLOITATION LOCALE	 */
#define LO_UPD_DATE							"01" /* Mise à jour de la date */
#define LO_UPD_TIME							"02" /* Mise à jour de l'heure */
#define LO_INIT_EJ_PRINTER					"03" /* Initialisation imprimante journal */
#define LO_INIT_TICKET_PRINTER				"05" /* Initialisation de l'imprimante ticket */
#define LO_INIT_CARD_READER					"06" /* Initialisation du lecteur de cartes */
#define LO_INIT_MANIP						"07" /* Initialisation du manipulateur */
#define LO_INIT_DEPOSIT						"08" /* Initialisation du dépôt */
#define LO_INIT_FRONT_EVENTS				"09" /* Initialisation des événements façades */
#define LO_PHY_CASST_STATE					"10" /* Etat d’une cassette physique */
#define LO_LOG_RES_STATE					"11" /* Etat d’une ressource logique */
#define LO_REBOOT_TERM						"12" /* Arrêt relance de l’automate */
#define LO_EDIT_TOTALS						"13" /* Edition et transmission de totaux */
#define LO_RESOURCE_ACTIVATION				"14" /* Activation d’une ressource */
#define LO_RESOURCE_DEACTIVATION			"15" /* Désactivation d’une ressource */
#define LO_EDIT_COUNTERS					"17" /* Edition et transmission de compteurs */
#define LO_TICKET_CUT						"18" /* Coupure du ticket */
#define LO_INIT_MOD_CHECK					"19" /* Initialisation du module de remise de chèque */
#define LO_INIT_KEYS						"20" /* Initialisation des clés */
#define LO_CHECK_CASST_PARAM_K7IN_K7MIXT	"22" /* Consultation du paramétrage des cassettes de type K7In ou K7Mixte */
#define LO_PRINT_RES_STATE					"23" /* Impression de l’état des ressources */
#define LO_INIT_MOD_AUTH					"24" /* Initialisation du module d’authentification */
#define LO_PRINT_EJ							"25" /* Impression du journal électronique */
#define LO_INVALIDATE_WITHDRAWAL_OP			"26" /* Invalidation des opérations de retrait */
#define LO_VALIDATE_WITHDRAWAL_OP			"27" /* Validation des opérations de retrait */
#define LO_PARAM_FILE_LOADING				"28" /* Chargement des fichiers paramètres */
#define LO_PARAM_FILE_RETREIVAL				"29" /* Récupération des fichiers paramètres */
#define LO_INIT_ENCR						"30" /* Initialisation de l'encrypteur */
#define LO_CASST_LOCK						"31" /* Verrouillage des cassettes */
#define LO_CASST_UNLOCK						"32" /* Déverrouillage des cassettes */
#define LO_INIT_PRINTER						"33" /* Initialisation de l’imprimante large */
#define LO_IMG_COPY							"34" /* Copie des images graphiques */
#define LO_BOX_OPEN							"35" /* Ouverture du coffre */
#define LO_BOX_CLOSE						"36" /* Fermeture du coffre */
#define LO_INIT_SCREEN						"37" /* Initialisation de l’écran tactile */
#define LO_FILE_PARAM_SAVING				"38" /* Stockage des fichiers paramètres */
#define LO_FILE_PARAM_ACTIVATION			"39" /* Prise en compte des fichiers paramètres stockés */
#define LO_CASST_PARAM_K7OUT_K7MIXT			"41" /* Paramétrage des cassettes de type K7Out ou K7Mixte */
#define LO_CHECK_CASST_PARAM_K7OUT_K7MIXT	"42" /* Consultation du paramétrage des cassettes de type K7Out ou K7Mixte */
#define LO_F_CRNCY_AUT						"43" /* Autorisation des devises */
#define LO_NOTES_TYPE_AUT					"44" /* Autorisation des types de billets */
#define LO_LOCAL_EXP_ENTRY_1				"46" /* Entrée en exploitation locale (1er compte rendu) */
#define LO_UNLOAD_K7OUT_K7MIXT				"50" /* Déchargement de cassette de Type K7Out ou K7Mixte */
#define LO_ACC_CASST_K7IN					"55" /* Arrêté comptable des cassettes de type K7In */
#define LO_LOG_RES_PARAM					"56" /* Paramétrage d’une ressource logique */
#define LO_ACC_BOX_CLOSURE					"57" /* Clôture comptable coffre */
#define LO_DIAG_CODE_CHAINING_3				"58" /* Chaîne des codes dialogue 3 */
#define LO_DIAG_CODE_CHAINING_4				"59" /* Chaîne des codes dialogue 4 */
#define LO_GET_PRINT_EJ_OP_REPORT			"60" /* Consultation / impression des comptes rendus d'opération bancaire du Journal Electronique */
#define LO_PWD_ENTRY						"61" /* Saisie du mot de passe */
#define LO_END_OF_MNG_INT					"62" /* Fin d'intervention gestionnaire */
#define LO_END_OF_SAV_INT					"63" /* Fin d'intervention SAV */
#define LO_SOFT_ACTIVATION					"64" /* Mise en service d'un logiciel téléchargé */
#define LO_PARAM_K7IN_K7MIXT				"65" /* Paramétrage des cassettes de type K7In ou K7Mixte */
#define LO_UNLOAD_K7IN						"66" /* Déchargement des cassettes de dépôt d’argent de type K7In */
#define LO_DEP_RES_RECON					"67" /* Réconciliation des données sensibles d’une ressource logique de dépôt valorisé */
#define LO_CHECK_TOTAL_VALUE				"68" /* Consultation de la valeur d'un total */
#define LO_CHECK_COUNTER_VALUE				"69" /* Consultation de la valeur d'un compteur */
#define LO_ACC_BOX_REF_UPD					"70" /* Mise à jour de la référence arrêté comptable coffre */
#define LO_ACC_BOX							"71" /* Arrêté comptable coffre */
#define LO_CLEAR_REJECT_COUNTERS			"74" /* Effacement des compteurs de rejet */
#define LO_ONL_SPY_ACTIVATION				"75" /* Activation de l'espion de ligne */
#define LO_ONL_SPY_DEACTIVATION				"76" /* Désactivation de l'espion de ligne */
#define LO_CONS_SPEC						"77" /* Spécifique constructeurs */
#define LO_COLD_START						"78" /* Démarrage à froid */
#define LO_CHECK_EJ							"79" /* Consultation du journal électronique */
#define LO_CTRL_BAL_K7OUT_K7MIXT			"80" /* Contrôle du solde en cassette de type K7Out ou K7Mixte */
#define LO_ADD_NOTES_K7OUT_K7MIXT			"82" /* Ajout de billets en cassette de type K7Out ou K7Mixte */
#define LO_REM_NOTES_K7OUT_K7MIXT			"83" /* Retrait de billets en cassette de type K7Out ou K7Mixte */
#define LO_LOAD_K7OUT_K7MIXT				"86" /* Chargement cassette de type K7Out ou K7Mixte */
#define LO_DEP_RES_ST_CHECK					"87" /* Consultation des cassettes d’une ressource logique de dépôt valorisé */
#define LO_FORCE_BAL_OUT					"88" /* Forçage d'encaisse out */
#define LO_CHG_PARAM_TEMP_CRD				"89" /* Modification des paramètres du frein cartes temporisées */
#define LO_PRINT_CAPTURED_CARDS				"90" /* Impression des cartes capturées */
#define LO_PRINT_OP_PENDING_CTRL			"91" /* Impression des opérations sous réserve de contrôle */
#define LO_DIAG_CODE_CHAINING_1				"92" /* Chaîne des codes dialogue 1 */
#define LO_DIAG_CODE_CHAINING_2				"93" /* Chaîne des codes dialogue 2 */
#define LO_LAMP_TURN_OFF					"94" /* Extinction des voyants et relais */
#define LO_DEP_RES_DATA_EDIT				"95" /* Edition des données sensibles d’une ressource logique de dépôt valorisé */
#define LO_EJ_PARTIAL_COPY					"96" /* Copie partielle du Journal Electronique sur disquette */
#define LO_PRINTER_FUND_COPY				"97" /* Copie des fonds pour imprimante large ou laser */
#define LO_START_TERMINAL					"98" /* Mise en service de l'automate */
#define LO_LOCAL_EXP_EXIT_1					"99" /* Sortie d’exploitation locale (1er compte rendu) */

/* Codes comptes rendu */
#define REP_CD_UPD_DATE						"01" /* Mise à jour de la date */
#define REP_CD_UPD_TIME						"02" /* Mise à jour de l'heure */
#define REP_CD_INIT_EJ_PRINTER				"03" /* Initialisation imprimante journal */
#define REP_CD_INIT_TICKET_PRINTER			"05" /* Initialisation de l'imprimante ticket */
#define REP_CD_INIT_CARD_READER				"06" /* Initialisation du lecteur de cartes */
#define REP_CD_INIT_MANIP					"07" /* Initialisation du manipulateur */
#define REP_CD_INIT_DEPOSIT					"08" /* Initialisation du dépôt */
#define REP_CD_INIT_FRONT_EVENTS			"09" /* Initialisation des événements façades */
#define REP_CD_REBOOT_TERM					"12" /* Arrêt relance de l’automate */
#define REP_CD_EDIT_TOTALS					"13" /* Edition et transmission de totaux */
#define REP_CD_RESOURCE_ACTIVATION			"14" /* Activation d’une ressource */
#define REP_CD_RESOURCE_DEACTIVATION		"15" /* Désactivation d’une ressource */
#define REP_CD_EDIT_COUNTERS				"17" /* Edition et transmission de compteurs */
#define REP_CD_INIT_MOD_CHECK				"19" /* Initialisation du module de remise de chèque */
#define REP_CD_CHECK_CASST_PARAM_K7IN_K7MIXT "22" /* Consultation du paramétrage des cassettes de type K7In ou K7Mixte */
#define REP_CD_INIT_MOD_AUTH				"24" /* Initialisation du module d’authentification */
#define REP_CD_INVALIDATE_WITHDRAWAL_OP		"26" /* Invalidation des opérations de retrait */
#define REP_CD_VALIDATE_WITHDRAWAL_OP		"27" /* Validation des opérations de retrait */
#define REP_CD_INIT_ENCR					"30" /* Initialisation de l'encrypteur */
#define REP_CD_CASST_LOCK					"31" /* Verrouillage des cassettes */
#define REP_CD_CASST_UNLOCK					"32" /* Déverrouillage des cassettes */
#define REP_CD_INIT_PRINTER					"33" /* Initialisation de l’imprimante large */
#define REP_CD_BOX_OPEN						"35" /* Ouverture du coffre */
#define REP_CD_BOX_CLOSE					"36" /* Fermeture du coffre */
#define REP_CD_INIT_SCREEN					"37" /* Initialisation de l’écran tactile */
#define REP_CD_CASST_PARAM_K7OUT_K7MIXT		"41" /* Paramétrage des cassettes de type K7Out ou K7Mixte */
#define REP_CD_CHECK_CASST_PARAM_K7OUT_K7MIXT "42" /* Consultation du paramétrage des cassettes de type K7Out ou K7Mixte */
#define REP_CD_F_CRNCY_AUT					"43" /* Autorisation des devises */
#define REP_CD_NOTES_TYPE_AUT				"44" /* Autorisation des types de billets */
#define REP_CD_LOCAL_EXP_ENTRY_1			"46" /* Entrée en exploitation locale (1er compte rendu) */
#define REP_CD_LOCAL_EXP_ENTRY_2			"47" /* Entrée en exploitation locale (2ième compte rendu) */
#define REP_CD_UNLOAD_K7OUT_K7MIXT			"50" /* Déchargement de cassette de Type K7Out ou K7Mixte */
#define REP_CD_ACC_CASST_K7IN				"55" /* Arrêté comptable des cassettes de type K7In */
#define REP_CD_LOG_RES_PARAM				"56" /* Paramétrage d’une ressource logique */
#define REP_CD_ACC_BOX_CLOSURE_1			"71" /* Clôture comptable coffre */
#define REP_CD_ACC_BOX_CLOSURE_2			"88" /* Clôture comptable coffre */
#define REP_CD_END_OF_MNG_INT				"62" /* Fin d'intervention gestionnaire */
#define REP_CD_END_OF_SAV_INT				"63" /* Fin d'intervention SAV */
#define REP_CD_SOFT_ACTIVATION				"64" /* Mise en service d'un logiciel téléchargé */
#define REP_CD_PARAM_K7IN_K7MIXT			"65" /* Paramétrage des cassettes de type K7In ou K7Mixte */
#define REP_CD_UNLOAD_K7IN					"66" /* Déchargement des cassettes de dépôt d’argent de type K7In */
#define REP_CD_ACC_BOX						"71" /* Arrêté comptable coffre */
#define REP_CD_COLD_START					"78" /* Démarrage à froid */
#define REP_CD_CTRL_BAL_K7OUT_K7MIXT		"80" /* Contrôle du solde en cassette de type K7Out ou K7Mixte */
#define REP_CD_ADD_NOTES_K7OUT_K7MIXT		"82" /* Ajout de billets en cassette de type K7Out ou K7Mixte */
#define REP_CD_REM_NOTES_K7OUT_K7MIXT		"83" /* Retrait de billets en cassette de type K7Out ou K7Mixte */
#define REP_CD_LOAD_K7OUT_K7MIXT			"86" /* Chargement cassette de type K7Out ou K7Mixte */
#define REP_CD_FORCE_BAL_OUT				"88" /* Forçage d'encaisse out */
#define REP_CD_CHG_PARAM_TEMP_CRD			"89" /* Modification des paramètres du frein cartes temporisées */
#define REP_CD_START_TERMINAL				"98" /* Mise en service de l'automate */
#define REP_CD_LOCAL_EXP_EXIT_1				"48" /* Sortie d’exploitation locale (1er compte rendu) */
#define REP_CD_LOCAL_EXP_EXIT_2				"49" /* Sortie d’exploitation locale (2ième compte rendu) */

#define REP_CD_N_CHECK_CASST_PARAM_K7IN_K7MIXT	22
#define REP_CD_N_EDIT_TOTALS					13
#define REP_CD_N_EDIT_COUNTERS					17
#define REP_CD_N_CHECK_CASST_PARAM_K7OUT_K7MIXT 42
#define REP_CD_N_NOTES_TYPE_AUT					44
#define REP_CD_N_LOCAL_EXP_ENTRY_1				46
#define REP_CD_N_LOCAL_EXP_ENTRY_2				47
#define REP_CD_N_LOCAL_EXP_EXIT_1				48
#define REP_CD_N_LOCAL_EXP_EXIT_2				49
#define REP_CD_N_ACC_CASST_K7IN					55
#define REP_CD_N_LOG_RES_PARAM					56
#define REP_CD_N_PARAM_K7IN_K7MIXT				65
#define REP_CD_N_UNLOAD_K7IN					66
#define REP_CD_N_ACC_BOX_CLOSURE_1				71
#define REP_CD_N_FORCE_BAL_OUT					88
#define REP_CD_N_57								57
#define REP_CD_N_67								67




/* LES OPERATIONS D'EXPLOITATION TELECOMMANDEE	*/
#define RO_PHY_CASST_STATE					"10" /* Etat d’une cassette physique */
#define RO_LOG_RES_STATE					"11" /* Etat d’une ressource logique */
#define RO_REBOOT_TERM						"12" /* Arrêt relance de l’automate */
#define RO_RESOURCE_ACTIVATION				"14" /* Activation d’une ressource */
#define RO_RESOURCE_DEACTIVATION			"15" /* Désactivation d’une ressource */
#define RO_CHECK_CASST_K7IN					"16" /* Consultation des encaisses des cassettes de type K7In */
#define RO_INIT_MOD_CHECK					"19" /* Initialisation du module de remise de chèque */
#define RO_CHECK_CASST_PARAM_K7IN_K7MIXT	"22" /* Consultation du paramétrage des cassettes de type K7In ou K7Mixte */
#define RO_INIT_MOD_AUTH					"24" /* Initialisation du module d’authentification */
#define RO_INIT_PRINTER						"29" /* Initialisation imprimante large */
#define RO_UPD_DATE							"30" /* Mise à jour de la date */
#define RO_UPD_TIME							"31" /* Mise à jour de l'heure */
#define RO_CHECK_TOTAL_VALUE				"32" /* Consultation de la valeur d'un total */
#define RO_CHECK_COUNTER_VALUE				"33" /* Consultation de la valeur d'un compteur */
#define RO_ACC_BOX							"35" /* Arrêté comptable coffre */
#define RO_START_TERMINAL					"38" /* Mise en service de l'automate */
#define RO_INIT_CARD_READER					"39" /* Initialisation du lecteur de cartes */
#define RO_INIT_DEPOSIT						"40" /* Initialisation du dépôt */
#define RO_INIT_MANIP						"41" /* Initialisation du manipulateur */
#define RO_ONL_SPY_ACTIVATION				"42" /* Activation de l'espion de ligne */
#define RO_ONL_SPY_DEACTIVATION				"43" /* Désactivation de l'espion de ligne */
#define RO_SEND_PERIODIC_MSG				"44" /* Demande d'émission d’une requête périodique */
#define RO_STOP_TERMINAL					"45" /* Mise hors service de l'automate */
#define RO_INVALIDATE_WITHDRAWAL_OP			"46" /* Invalidation des opérations de retrait */
#define RO_VALIDATE_WITHDRAWAL_OP			"47" /* Validation des opérations de retrait */
#define RO_INIT_TICKET_PRINTER				"48" /* Initialisation de l'imprimante ticket */
#define RO_INIT_FRONT_EVENTS				"49" /* Initialisation des événements façades */
#define RO_EDIT_TOTALS						"53" /* Edition et émission de totaux */
#define RO_EDIT_COUNTERS					"57" /* Edition et émission de compteurs */
#define RO_INIT_ENCR						"59" /* Initialisation de l'encrypteur */
#define RO_DL_SOFT_PARAM_FILES				"60" /* Chargement d'une version du logiciel automate et / ou des fichiers paramètres */
#define RO_INIT_EJ_PRINTER					"63" /* Initialisation de l'imprimante journal */
#define RO_SET_B0_PROC_STATE				"64" /* Activation désactivation des traitements B0’ EMV */
#define RO_ACC_K7IN_DEP_CASST				"65" /* Arrêté comptable des cassettes de type dépôt valorisé K7In */
#define RO_CHECK_CASST_K7OUT_K7MIXT			"69" /* Consultation des encaisses des cassettes de Type K7Out et K7Mixtes */
#define RO_INIT_SCREEN						"70" /* Initialisation de l'écran tactile */
#define RO_CHECK_CASST_PARAM_K7OUT_K7MIXT	"72" /* Consultation du paramétrage des cassettes de type K7Out ou K7Mixte */
#define RO_AUTO_ACC_BOX						"75" /* Arrêté comptable coffre automatique */
#define RO_LEN								2

#define RO_N_LOG_RES_STATE						11
#define RO_N_CHECK_CASST_K7IN					16
#define RO_N_CHECK_CASST_PARAM_K7IN_K7MIXT		22
#define RO_N_EDIT_TOTALS						53
#define RO_N_EDIT_COUNTERS						57
#define RO_N_ACC_BOX							35
#define RO_N_AUTO_ACC_BOX						75
#define RO_N_ACC_K7IN_DEP_CASST					65
#define RO_N_CHECK_CASST_K7OUT_K7MIXT			69
#define RO_N_CHECK_CASST_PARAM_K7OUT_K7MIXT		72

/* Rubrique 00: Separateur  */
#define ATM_SERVER_SEP				'-'

/* Rubrique 9: Indicateur de Réentrée */

#define LG_F009_OTHER_MSG				'0'		/* wagon cassette absent	*/
#define LG_F009_REENTRY_MSG				'1'		/* wagon cassette présent	*/

/* Rubrique 23: Code Action */

#define LG_F023_NO_ACTION				'0' /* Pas d'action */
#define LG_F023_REENTRY					'1' /* Réentrée */
#define LG_F023_INIT_SCD				'4' /* Initialisation SCD */
#define LG_F023_PARAM_FILE_DL			'5' /* Téléchargement des fichiers paramètres */
#define LG_F023_ATM_MASTER_KEY_RENEWAL	'6' /* Renouvellement clé maître GAB en nouvelle cryptographie */
#define LG_F023_SRV_MASTER_KEY_RENEWAL	'7' /* Renouvellement clé maître GDG en nouvelle cryptographie */
#define LG_F023_KTK_KEY_RENEWAL			'8' /* Renouvellement clé de transport de clés en nouvelle cryptographie */
#define LG_F023_KT_KS_KEY_RENEWAL		'9' /* Renouvellement clé de chiffrement et clé de scellement en nouvelle cryptographie */
#define LG_F023_REMOTE_ATM_CMD			'A' /* Exécution d'une opération d'exploitation */
#define LG_F023_KEY_LOADING				'C' /* Chargement des clés (ancienne cryptographie) */
#define LG_F023_ATM_PARAM_UPD			'E' /* Mise à jour des paramètres spécifiques de l’automate */
#define LG_F023_SEND_TERM_ATTR			'U' /* Demande de transfert des caractéristiques de l’automate */
#define LG_F023_TLP_REQUEST				'D' /* Demande de Télé Paramétrage */

/* Rubrique 75: Type d'interrogation */

/*Rubrique 53: Indicateur carte temporisée*/

#define LG_F053_NORMAL_CARD				'0'		/* Carte normale	*/
#define LG_F053_TEMP_CARD				'1'		/* Carte temporisée	*/
#define LG_F053_LOCAL_CARD				'2'		/* Carte locale	*/

/* Rubrique 83: Indicateur dernier bloc interrogation */

#define LG_F083_CUR_BLK					1		/* Block current	*/
#define LG_F083_LAST_BLK				0		/* Last Block		*/

/* Rubrique 87: Code opération bancaire */

#define LG_F087_INCIDENT				"00"		/* Incident		*/
#define LG_F087_WITHDRAWAL				"01"		/* Cash withdrawal / cash advance		*/
#define LG_F087_CHEQUE_DEPOSIT			"04"		/* Cheque Deposit						*/
#define LG_F087_CASH_DEPOSIT			"08"		/* Cash Deposit							*/
#define LG_F087_CHEQUE_REQ				"51"		/* Cheque Request						*/
#define LG_F087_WITHDRAWAL_REV			"15"		/* Withdrawal reversal   			    */
#define LG_F087_DEPOSIT					"21"		/* DEPOSIT								*/
#define LG_F087_REFUND					"20"		/* Returns								*/
#define LG_F087_DEPOSIT					"21"		/* Deposit								*/
#define LG_F087_BAL_INQ					"31"		/* Balance inquiry						*/
#define LG_F087_TRANSFER_REQ			"40"		/* Transfer request




/* Rubrique 168: Type de compte-rendu d'opération bancaire */
#define LG_F168_WITHDRAWAL				'1'			/* Retrait							*/
#define LG_F168_INCID					'2'			/* Dégradé et épilogue				*/
#define LG_F168_FUND_TRNSF				'3'			/* Virement							*/
#define LG_F168_OTHER_OP				'4'			/* Autres opérations				*/
#define LG_F168_WITHDRAWAL_F_CRNCY		'6'			/* Retrait en devises				*/
#define LG_F168_DEPOSIT					'7'			/* Dépôt d’argent en liasse			*/
#define LG_F168_TOPUP_MONEO				'8'			/* Chargement Monéo					*/

/* Rubrique 189 Prise en compte immédiate ou différée */
#define LG_F189_IMMEDIATE				'0'			/* immédiate							*/
#define LG_F189_DEFERRED				'1'			/* Ddifférée			*/

#define ATM_SCREEN_SIZE						40
#define PARAM_FILE_BLOCK_SIZE				1024
#define TERM_SPEC_DATA_BLK_SIZE				180
#define TERM_SPEC_PARAM_BLK_SIZE			443
#define WAG_PROD_1_SIZE						100

#define LG_SHORT_STAT_LEN 					61

/* Rubrique 33: Variable d'état automate */
#define LG_F033_KEY_DL_OK					'0'	/* Traitement et/ou chargement clé correct */
#define LG_F033_KEY_DL_NOK					'1'	/* Chargement clé incorrect */
#define LG_F033_SECURITY_MOD_MISSING		'2' /* Module de sécurité absent */
#define LG_F033_SERVER_MSG_NOK				'3' /* Message GDG incorrect (code action, rubrique, template, N° de séquence) */
#define LG_F033_SERVER_SIG_NOK				'4' /* Signature GDG incorrecte (nouvelle cryptographie uniquement) */
#define LG_F033_TPK_NOK						'5' /* Vérification KT incorrecte (nouvelle cryptographie uniquement) */
#define LG_F033_TAK_NOK						'6' /* Vérification KSC incorrecte (nouvelle cryptographie uniquement) */
#define LG_F033_SERVER_KCV_NOK				'7' /* Empreinte GDG incorrecte */
#define LG_F033_OTHERS						'9' /* Autres */

/* 54 Code traitement */
#define LG_F054_ACCEPTED_CARD				'0'	/* Carte acceptée  */
#define LG_F054_CONFISCATE_CARD				'2'	/* Carte à confisquer */
#define LG_F054_REFUSED_CARD				'4' /* Carte à refuser  */
#define LG_F054_RETRY_PIN					'6' /* Code confidentiel à recomposer*/
#define LG_F054_CAPTTURED_CARD				'7' /* Capture de la carte et fermeture de l'automate*/
#define LG_F054_LOAD_KEYS					'8' /* Restitution de la carte, et rechargement des clés */



/* Rubrique: Demande de blocage puce B0’ */
#define LG_F151_BLOCK_CHIP_N				"00"		/* Ne pas bloquer la puce */
#define LG_F151_BLOCK_CHIP_Y				"01"		/* Bloquer la puce */

/* Rubrique 249: Mode de traitement de l’opération */
#define LG_F249_MODE_MAG				'1'		/* La transaction s’effectue avec les données de la piste IS02 */
#define LG_F249_MODE_CHIP				'2'		/* La transaction s’effectue avec les données de l’application carte EMV */

/* Rubrique: 251 Indicateur wagon EMV */
#define LG_F251_EMV_DATA_PRESENT		'1'		/* Wagon EMV présent dans le message */
#define LG_F251_EMV_DATA_NOT_PRESENT	'0'		/* Wagon EMV absent du message */


/* Rubrique 264: Code type de Wagon*/

#define LG_F264_WAG_MONEO				'2'		/* Wagon de données Monéo	*/
#define LG_F264_WAG_EMV					'5'		/* Wagon de données EMV		*/
#define LG_F264_WAM_DEPOSIT_ADV			'6'		/* Wagon de données compte rendu de dépôt en liasse		*/
#define LG_F264_CASST					'7'		/* Wagon cassettes			*/


/* Rubrique 269: Changement d’état des cassettes physiques*/
#define LG_F269_CASTT_NOT_PRESENT			'0'		/* wagon cassette absent	*/
#define LG_F269_CASTT_PRESENT				'1'		/* wagon cassette présent	*/

/* Rubrique 270: Type de module cryptographique utilisé */
#define LG_F270_NONE					'0'	/* Aucun (ILS) */
#define LG_F270_NEW_CRYPTO				'1' /* Module gérant la nouvelle cryptographie */
#define LG_F270_OLD_CRYPTO				'2' /* Module gérant l’ancienne cryptographie */
#define LG_F270_BOTH					'3' /* Module gérant les deux architectures */


/* Rubrique 271: Motif de la demande des clés */
#define LG_F271_INIT_TERM				'0'		/* Initialisation automate	*/
#define LG_F271_INIT_SCD				'5'		/* Initialisation SCD		*/
#define LG_F271_MK_ATM_RENEWAL			'6'		/* Renouvellement clé maître GAB en nouvelle cryptographie	*/
#define LG_F271_MK_SERVER_RENEWAL		'7'		/* Renouvellement clé maître GDG en nouvelle cryptographie	*/
#define LG_F271_TMK_RENEWAL				'8'		/* Renouvellement clé de transport de clés en nouvelle cryptographie	*/
#define LG_F271_TPK_TAK_RENEWAL			'9'		/* Renouvellement clé de chiffrement et clé de scellement en nouvelle cryptographie	*/

/* Rubrique 273: Modes d’utilisations SCD */
/** Position 1 : Mode de scellement des données **/
#define LG_F273_T1_NONE				'0'	/* Pas de scellement (valeur par défaut et applicable lors du démarrage à froid) */
#define LG_F273_T1_MAC_S			'1' /* Scellement simple */
#define LG_F273_T1_MAC_C			'2' /* Scellement chaîné */

/** Position 2 : Format de chiffrement du PINBlock **/
#define LG_F273_T2_NONE				'0' /* Aucun */
#define LG_F273_T2_ISO_0			'1' /* Format ISO 0 */
#define LG_F273_T2_ISO_3			'2' /* Format ISO 3 */

/** Position 3 : Architecture cryptographique utilisée **/
#define LG_F273_T3_INITIAL			'0' /* Premier pilotage lors de la 1ère connexion de l’automate */
#define LG_F273_T3_OLD_CRYPTO		'1' /* Ancienne cryptographie */
#define LG_F273_T3_NEW_CRYPTO		'2' /* Nouvelle cryptographie */
#define LG_F273_T3_NONE				'4' /* Aucune architecture */


/* Rubrique 274: Variable d’état gestionnaire */
#define LG_F274_PROC_OK				'0'	/* Traitement correct */
#define LG_F274_TERM_MSG_NOK		'1' /* Message GAB incorrect */
#define LG_F274_TERM_SIG_NOK		'2' /* Signature GAB incorrecte */
#define LG_F274_TERM_KCV_NOK		'3' /* Empreinte GAB incorrecte */
#define LG_F274_ABORT				'4' /* Abandon de l’action */
#define LG_F274_OTHER				'9' /* Autres */


/* Rubrique 279: Format de chiffrement géré */
#define LG_F279_NONE				'0' /* Aucun (ILS) */
#define LG_F279_ISO_0				'1'	/* Format ISO 0 */
#define LG_F279_ISO_3				'2' /* Format ISO 3 */
#define LG_F279_ISO_0_ISO_3			'3' /* Format ISO 0 et Format ISO3 */

/* Les status des clés*/
#define KEY_STATUS_INIT				'0' /* Init (ILS) */
#define KEY_STATUS_SRV_MK_LOADED	'1'	/* Serveur MK chargé  ISO 0 */
#define KEY_STATUS_MK_LOADED		'2' /* MK chargé */
#define KEY_STATUS_OK				'3' /* OK*/


/* Les opérations de l'exploitation*/
#define LG_F25_START_TERM			"38" /* Mise en service de l'automate*/

/*Crypto fields */

typedef enum {
	LG_CRPT_FLD_KEY_TYPE			= 0	,
	LG_CRPT_FLD_KEY_SERIAL				,
	LG_CRPT_FLD_KEY_VERSION				,
	LG_CRPT_FLD_KEY_METHOD				,
	LG_CRPT_FLD_RSA_OUT					,
	LG_CRPT_FLD_MODULO					,
	LG_CRPT_FLD_EXPONENT				,
	LG_CRPT_FLD_MANUFAC					,
	LG_CRPT_FLD_SERIAL					,
	LG_CRPT_FLD_RAND					,
	LG_CRPT_FLD_SERVER_SIG				,
	LG_CRPT_FLD_KD_SIG					,
	LG_CRPT_FLD_KG_SIG					,
	LG_CRPT_FLD_KTK_SIG					,
	LG_CRPT_FLD_KT_KEY					,
	LG_CRPT_FLD_KSC_KEY					,
	LG_CRPT_FLD_MAC						,
	LG_CRPT_FLD_KT_VERIF_DATA			,
	LG_CRPT_FLD_KSC_VERIF_DATA			,
	LG_CRPT_FLD_PKCS_REF				,
	LG_CRPT_FLD_HASH_ALGO				,
	LG_CRPT_FLD_ENC_ALGO				,
	LG_CRPT_FLD_SIG_ALGO				,
	LG_CRPT_FLD_MASK					,
	LG_CRPT_FLD_P_PARAM_IND				,
	LG_CRPT_FLD_SALT_PARAM_IND			,
	LG_CRPT_FLD_P						,
	LG_CRPT_FLD_SALT					,

	LG_CRPT_FLD_QTY
} E_LG_CRPT_FIELDS;




#define LG_CRPT_KEY_TYPE				"FFD1"	/* Type de clé										*/
#define LG_CRPT_KEY_SERIAL				"FFD2"	/* Série de la clé									*/
#define LG_CRPT_KEY_VERSION				"FFD3"	/* Version de la clé								*/
#define LG_CRPT_KEY_METHOD				"FFD4"	/* Method											*/
#define LG_CRPT_RSA_OUT					"FFD5"	/* RSA-OUT											*/
#define LG_CRPT_MODULO					"FFD6"	/* Modulo											*/
#define LG_CRPT_EXPONENT				"FFD7"	/* Exposant											*/
#define LG_CRPT_MANUFAC					"FFD8"	/* Manufac											*/
#define LG_CRPT_SERIAL					"FFD9"	/* Serial											*/
#define LG_CRPT_RAND					"FFDA"	/* Aléa												*/
#define LG_CRPT_SERVER_SIG				"FFDB"	/* Authentification GDG (Signature GDG par KGs)		*/
#define LG_CRPT_KD_SIG					"FFDC"	/* Signature de KDp									*/
#define LG_CRPT_KG_SIG					"FFDD"	/* Signature KGp									*/
#define LG_CRPT_KTK_SIG					"FFDE"	/* Signature de KTK									*/
#define LG_CRPT_KT_KEY					"FFE0"	/* Cryptogramme de la clé KT						*/
#define LG_CRPT_KSC_KEY					"FFE1"	/* Cryptogramme de la clé KSC						*/
#define LG_CRPT_MAC						"FFE3"	/* Sceau											*/
#define LG_CRPT_KT_VERIF_DATA			"FFE4"	/* Donnée de vérification KT						*/
#define LG_CRPT_KSC_VERIF_DATA			"FFE5"	/* Donnée de vérification KSC						*/
#define LG_CRPT_PKCS_REF				"FFE6"	/* Référence norme PKCS implémentée					*/
#define LG_CRPT_HASH_ALGO				"FFE7"	/* Algorithme de haschage							*/
#define LG_CRPT_ENC_ALGO				"FFE8"	/* Algorithme de chiffrement KTK géré				*/
#define LG_CRPT_SIG_ALGO				"FFE9"	/* Algorithme de signature géré						*/
#define LG_CRPT_MASK					"FFEA"	/* Masque											*/
#define LG_CRPT_P_PARAM_IND				"FFEB"	/* Indicateur Paramètre p							*/
#define LG_CRPT_SALT_PARAM_IND			"FFEC"	/* Indicateur paramètre Salt						*/
#define LG_CRPT_P						"FFED"	/* p												*/
#define LG_CRPT_SALT					"FFEE"	/* salt												*/



typedef enum {
	LG_TMPLT_LAY_KG_ID				=0	,
	LG_TMPLT_LAY_ENC_KTK				,
	LG_TMPLT_LAY_KG_P					,
	LG_TMPLT_LAY_KD_P					,
	LG_TMPLT_LAY_SN_SCD					,
	LG_TMPLT_LAY_RAND					,
	LG_TMPLT_LAY_GDG_AUTHEN				,
	LG_TMPLT_LAY_KD_P_SIG				,
	LG_TMPLT_LAY_KG_P_SIG				,
	LG_TMPLT_LAY_KTK_SIG				,
	LG_TMPLT_LAY_ENC_KT					,
	LG_TMPLT_LAY_ENC_KSC				,
	LG_TMPLT_LAY_MAC					,
	LG_TMPLT_LAY_KD_ID					,
	LG_TMPLT_LAY_KTK_ID					,
	LG_TMPLT_LAY_CRYPTO_ELTS			,
	LG_TMPLT_LAY_PARAM_1				,
	LG_TMPLT_LAY_PARAM_2				,
	LG_TMPLT_LAY_PARAM_3				,
	LG_TMPLT_LAY_PARAM_4				,
	LG_TMPLT_LAY_PARAM_5				,

	LG_TMPLT_LAY_QTY
} E_LG_TMPLT_LAYOUT;



#define LG_TMPLT_KG_ID					"01"	/* IdKG								*/
#define LG_TMPLT_ENC_KTK				"02"	/* KTK-chiffrée						*/
#define LG_TMPLT_KG_P					"03"	/* KGp								*/
#define LG_TMPLT_KD_P					"04"	/* KDp								*/
#define LG_TMPLT_SN_SCD					"05"	/* SnSCD							*/
#define LG_TMPLT_RAND					"06"	/* Aléa								*/
#define LG_TMPLT_GDG_AUTHEN				"07"	/* Authentification GDG				*/
#define LG_TMPLT_KD_P_SIG				"08"	/* Signature de KDp					*/
#define LG_TMPLT_KG_P_SIG				"09"	/* Signature de KGp					*/
#define LG_TMPLT_KTK_SIG				"0A"	/* Signature de KTK					*/
#define LG_TMPLT_ENC_KT					"0B"	/* KT-chiffrée						*/
#define LG_TMPLT_ENC_KSC				"0C"	/* KSC-chiffrée						*/
#define LG_TMPLT_MAC					"0D"	/* Sceau							*/
#define LG_TMPLT_KD_ID					"0E"	/* IdKD								*/
#define LG_TMPLT_KTK_ID					"10"	/* IdKTK							*/
#define LG_TMPLT_CRYPTO_ELTS			"12"	/* Elts_Crypto						*/
#define LG_TMPLT_PARAM_1				"13"	/* Param1							*/
#define LG_TMPLT_PARAM_2				"14"	/* Param2							*/
#define LG_TMPLT_PARAM_3				"15"	/* Param3							*/
#define LG_TMPLT_PARAM_4				"16"	/* Param4							*/
#define LG_TMPLT_PARAM_5				"17"	/* Param5							*/


#define LG_TMPLT_KG_ID_NB_FIELDS				3
#define LG_TMPLT_ENC_KTK_NB_FIELDS				5
#define LG_TMPLT_KG_P_NB_FIELDS					5
#define LG_TMPLT_KD_P_NB_FIELDS					5
#define LG_TMPLT_SN_SCD_NB_FIELDS				2
#define LG_TMPLT_RAND_NB_FIELDS					1
#define LG_TMPLT_GDG_AUTHEN_NB_FIELDS			1
#define LG_TMPLT_KD_P_SIG_NB_FIELDS				1
#define LG_TMPLT_KG_P_SIG_NB_FIELDS				1
#define LG_TMPLT_KTK_SIG_NB_FIELDS				1
#define LG_TMPLT_ENC_KT_NB_FIELDS				5
#define LG_TMPLT_ENC_KSC_NB_FIELDS				5
#define LG_TMPLT_MAC_NB_FIELDS					1
#define LG_TMPLT_KD_ID_NB_FIELDS				3
#define LG_TMPLT_KTK_ID_NB_FIELDS				3
#define LG_TMPLT_CRYPTO_ELTS_NB_FIELDS			4
#define LG_TMPLT_PARAM_1_NB_FIELDS				3
#define LG_TMPLT_PARAM_2_NB_FIELDS				3
#define LG_TMPLT_PARAM_3_NB_FIELDS				1
#define LG_TMPLT_PARAM_4_NB_FIELDS				1
#define LG_TMPLT_PARAM_5_NB_FIELDS				1


#define LG_CRYPTO_RSA_KEY_MOD_LEN				"0080"
#define LG_CRYPTO_RSA_PUB_KEY_EXP				"65537"
#define LG_CRYPTO_RSA_FMT						"01"/*Identifiant de la méthode de mise en forme des données d’un bi-clé RSA.*/

#define LG_CRYPTO_KEY_KG						"KG"
#define LG_CRYPTO_KEY_KD						"KD"
#define LG_CRYPTO_KEY_KTK						"KTK"
#define LG_CRYPTO_KEY_KT						"TPK"
#define LG_CRYPTO_KEY_KSC						"MAC"

#define LG_KEY_TYPE_MK_SRV						"01"
#define LG_KEY_TYPE_MK_TERM						"02"
#define LG_KEY_TYPE_DES_KEY						"03"

#define LG_KEY_STATUS_INITIAL					"I"
#define LG_KEY_STATUS_NORMAL					"N"
#define LG_KEY_STATUS_EXPIRED					"E"

#define LG_SIG_ALGO_PKCS						"01"
#define LG_KEY_STATUS_PSS						"05"

#define LG_KEY_METHOD_EME_OAEP					"01"
#define LG_KEY_METHOD_EME_PKCS1					"02"

#define LG_SIG_ALGO_PKCS1						"01"
#define LG_SIG_ALGO_PSS							"02"

#define LG_HASH_ALGO_SHA_1						"01"
#define LG_HASH_ALGO_SHA_256					"02"
#define LG_HASH_ALGO_SHA_384					"03"
#define LG_HASH_ALGO_SHA_512					"04"

/*Key type*/
#define LG_TK_KG                           		"31"
#define LG_TK_KTK                          		"02"
#define LG_TK_KSC                         		"27"
#define LG_TK_KT                           		"26"


#define LG_HASH_ALGO_SHA_512					"04"


#define  PADDED_DATA_LENGTH 					256




#define OR_BAL			  		9
#define OR_DISCONECT			13




#endif



