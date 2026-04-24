#ifndef CBAE_DEFINE_H
#define CBAE_DEFINE_H

/*
#define OPS_BANK_CODE		"00000"
#define COUNTRY_CODE_FRANCE	"250"
*/



#define RESP_MSG_TYPE(m)	((m)&0xFFFFFFFE)+10
#define ORIG_MSG_TYPE(m)	((m)&0xFFFFFFFE)
#define REQ_MSG_TYPE(m)		(m)-10


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
#define F_ANP					6
#define F_ANLP					7
#define F_ANRP					8
#define F_UNDEF					9			/* Field undefined in CBAE */




#define INTERFACE_ISS			(1<<0)
#define INTERFACE_ACQ			(1<<1)

#define CBAE_MAX_FIELDS			128

#define MSG_TYPE_UNDEF			0
#define MSG_TYPE_REQ			1
#define MSG_TYPE_RESP			2


#define NODE_LIST_BO			'B'
#define NODE_LIST_LINE			'L'

#define NODE_TYPE_ISO			'I'
#define NODE_TYPE_CBAE		'T'


#define KEY_MNG_SELF			'S'
#define KEY_MNG_OTHER			'O'

/*#define NB_KEY_PACKS				2*/ /*EBE130617*/

#define KEY_KR_GROUP_ID			"0"
#define KEY_KR_KEY_ID			"00"


#define			HSM_FR_ZPK		'1'
#define			HSM_FR_ZAK		'2'



#define CBAE_F003_LEN				6
#define CBAE_F003_P1_LEN			2
#define CBAE_F007_LEN				10
#define CBAE_F011_LEN				6
#define CBAE_F012_LEN				6
#define CBAE_F013_LEN				4
#define CBAE_F025_LEN				2
#define CBAE_F039_LEN				2
#define CBAE_F052_LEN				16
#define CBAE_F053_LEN				10
#define CBAE_F064_LEN				8
#define CBAE_F070_LEN				3
#define CBAE_F090_LEN				42
#define CBAE_F091_LEN				1
#define CBAE_F117_LEN				255

#define CBAE_KEY_LENGTH				48

/* Below are used to declare/implement tlv library */
#define F044_NB_FIELDS				13
#define F044_BUF_LEN				124
#define F044_TAG_LEN				2


/*#define F047_NB_FIELDS				10*/
#define F047_NB_FIELDS				12		/* EBE140421 Enh4.08 */
#define F047_BUF_LEN				2048
#define F047_TAG_LEN				2
/*Start SNO310316 Enh4.10 */
 /* Field 57 Tags */
/*#define F057_NB_FIELDS				15*/ /* MTR20180410 - Enh4.12 */
#define F057_NB_FIELDS				38		 /*AMER20190228 PROD00066569: Enh5.00*/ /* NBO-HABIL501 */
#define F057_BUF_LEN				1024
#define F057_TAG_LEN				4

/*End SNO310316 */
/*#define F059_NB_FIELDS				42*/
/*#define F059_NB_FIELDS				50*/		/* EBE140421 Enh4.08 */
#define F059_NB_FIELDS				57		/* SNO150330 Enh 4.09  */ /* 57 -> 53 */
#define F059_BUF_LEN				1024
#define F059_TAG_LEN				4

/*Start PLUTONL-4280 :Enh5.03 :1355*/
#define F105_NB_FIELDS              12
#define F105_BUF_LEN                1024
#define F105_TAG_LEN                2
/*End PLUTONL-4280 :Enh5.03 :1355*/

/*Start PLUTONL-4280 :Enh5.03 :1399*/
#define F108_NB_FIELDS              1
#define F108_BUF_LEN                1024
#define F108_TAG_LEN                4
/*End PLUTONL-4280 :Enh5.03 :1399*/

/* #define F112_NB_FIELDS				3    SNO150330 Enh 4.09 --744 */
/*#define F112_NB_FIELDS				4    SNO150330 Enh 4.09 --744 */
#define F112_NB_FIELDS				7   /* MTR20180411 Enh 4.12 -- Fiche986 */
#define F112_BUF_LEN				512
#define F112_TAG_LEN				2

#define F113_NB_FIELDS				1
#define F113_BUF_LEN				512
#define F113_TAG_LEN				2

 /*#define F114_NB_FIELDS				2   SNO150330 Enh 4.09 "11431"--745 */
/*#define F114_NB_FIELDS				3    SNO150330 Enh 4.09 "11431"--745 */
#define F114_NB_FIELDS				6/*4*/   /*ASO20200602 PROD00075275: 1184 Enh5.01*//* PLUTONL-4280 :Enh5.03 :1380*/
#define F114_BUF_LEN				512
#define F114_TAG_LEN				2

/*Start SNO310316 Enh4.10 */
 /* Field 115 Tags */
#define F115_NB_FIELDS				3
#define F115_BUF_LEN				1024
#define F115_TAG_LEN				4

/*End SNO310316 */
#define F116_NB_FIELDS				1
#define F116_BUF_LEN				512
#define F116_TAG_LEN				2

#define F117_NB_FIELDS				13
#define F117_BUF_LEN				512
#define F117_TAG_LEN				2

/*AMER20190228 PROD00066569: Enh5.00 1046*/
#define F118_NB_FIELDS				48	/*ASO20210305 : Enh5.02 1272*//*PLUTONL-4280 :Enh5.03 :1383*/
#define F118_BUF_LEN				512
#define F118_TAG_LEN				4
/*Start SNO310316 Enh4.10 */
 /* Field 115 Tags */
/*#define F119_NB_FIELDS				1*/
#define F119_NB_FIELDS				30/*15*/	/*MAK20200330 PROD00074655: Enh5.01 1143 */ /* NBO-HABIL501 *//*PLUTONL-4280 :Enh5.03 :1348 -1420-1354-1363-1375-1383*/
#define F119_BUF_LEN				1024
#define F119_TAG_LEN				4
 
#define CBAE_F055_NB_SUBFIELDS		35 /* MTR Enh4.12 */


#define CBAE_F044_NB_SUBFIELDS		F044_NB_FIELDS
#define CBAE_F047_NB_SUBFIELDS		F047_NB_FIELDS
#define CBAE_F059_NB_SUBFIELDS		F059_NB_FIELDS
#define CBAE_F057_NB_SUBFIELDS		F057_NB_FIELDS
#define CBAE_F112_NB_SUBFIELDS		F112_NB_FIELDS
#define CBAE_F113_NB_SUBFIELDS		F113_NB_FIELDS
#define CBAE_F114_NB_SUBFIELDS		F114_NB_FIELDS
#define CBAE_F116_NB_SUBFIELDS		F116_NB_FIELDS
#define CBAE_F117_NB_SUBFIELDS		F117_NB_FIELDS
#define CBAE_F119_NB_SUBFIELDS		F119_NB_FIELDS	/*MAK_20200428 PROD00074908 : Enh5.01 1143 */
#define CBAE_F105_NB_SUBFIELDS		F105_NB_FIELDS  /*PLUTONL-4280 :Enh5.03 :1355*/
#define CBAE_F108_NB_SUBFIELDS		F108_NB_FIELDS  /*PLUTONL-4280 :Enh5.03 :1399*/


#define SECURITY_KEY_KR				"KR"
#define SECURITY_KEY_KT				"KT"
#define SECURITY_KEY_KS				"KS"
#define SECURITY_KEY_KC				"KC"

#define CB_KEY_STATUS_RENEWED		'R'
#define CB_KEY_STATUS_FREE			'F'


#define HSM_KEY_FLAG_ZEK			'0'
#define HSM_KEY_FLAG_ZAK			'1'

/* Field3: Processing code
*/
#define CBAE_PC_PURCHASE				"00"			/* Achat de biens ou services				*/
#define CBAE_PC_ATM_WITHDRAWAL			"01"			/* Retrait sur automate (DAB/GAB)			*/
#define CBAE_PC_QUASICASH				"11"			/* Quasi-cash								*/
#define CBAE_PC_CAPTURE_CARD			"14"			/* Capture de carte							*/
#define CBAE_PC_WITHDRAWAL				"17"			/* Retrait au guichet						*/
#define CBAE_PC_CREDIT					"20"			/* Cr�dit									*/
#define CBAE_PC_QUASICASH_RETURN		"28"			/* Retour Quasi-Cash */
#define CBAE_PC_FUNDS_CONSULT			"30"			/* Interrogation sur disponibilit� de fonds */
#define CBAE_PC_CARD_RETURN				"37"			/* Restitution de carte						*/
#define CBAE_PC_FUNDS_TRANSFER_D		"41"			/* Transfert de fonds, d�bit				*/
#define CBAE_PC_FUNDS_TRANSFER_C		"42"			/* Transfert de fonds, cr�dit				*/
#define CBAE_PC_PME						"81"			/* Autorisation de chargement PME			*/

/*Mode de lecture du syst�me d'acceptation(Point of service entry mode)*/
#define CBAE_F022_P3_MPOS_SOFT_PIN			'3'			/* Capacit� de la solution mPOS � saisir le code confidentiel (r�serv� incoming)									*/


/* Field Conditions de la transaction au point de service (Point of service condition code) */

#define CBAE_F025_NORMAL_CONDITION			"00"		/* Conditions normales									*/
#define CBAE_F025_CARDHOLDER_NOT_PRESENT	"01"		/* Client non pr�sent									*/
#define CBAE_F025_FREE_TERM_CAPTURE_CARD	"02"		/* Terminal libre-service pouvant conserver la carte	*/
#define CBAE_F025_FRAUD_SUSP_BY_MER			"03"		/* Suspicion de fraude de la part du commer�ant			*/
#define CBAE_F025_CH_P_CARD_NP				"05"		/* Client pr�sent, carte non pr�sente(only international trx) */
#define CBAE_F025_PHONE_REQUEST				"07"		/* Demande par t�l�phone (via centre d�appels)			*/
#define CBAE_F025_NON_LOCAL_PURCHASE		"08"		/* Achat par correspondance ou t�l�phone				*/
#define CBAE_F025_CH_ID_VERIFIED			"10"		/* Identit� du client v�rifi�e							*/
#define CBAE_F025_FRAUD_SUSPICION			"11"		/* Suspicion de fraude									*/
#define CBAE_F025_SECURITY_REASON			"12"		/* Raisons de s�curit�									*/
#define CBAE_F025_HOME_TERMINAL				"15"		/* Terminal � domicile									*/
#define CBAE_F025_FREE_TERM_NCAPTURE_CARD	"27"		/* Terminal libre service n�ayant pas la capacit� de conserver la carte */


/*33 Diff�r� de recouvrement*/
#define CBAE_PCD_RECOVER				"33"

/**********************************************************/

/*Field 39 */

#define CBAE_F039_APPROVED					"00"			/* transaction approuv�e ou trait�e avec succ�s		*/
#define CBAE_F039_DO_NOT_HONOR				"05"			/* ne pas honorer									*/
#define CBAE_F039_APPROVE_WITH_ID			"08"			/* approuver apr�s identification					*/
#define CBAE_F039_PARTIAL_APPROVAL			"10"			/* transaction approuv�e partiellement				*/
#define CBAE_F039_INVALID_TRX				"12"			/* transaction invalide								*/
#define CBAE_F039_INVALID_AMOUNT			"13"			/* montant invalide									*/
#define CBAE_F039_SECURITY_ERROR			"20"			/* r�ponse erron�e (erreur dans le domaine serveur) */
#define CBAE_F039_TRX_NOT_EXECUTED			"21"			/* transaction non ex�cut�e							*/
#define CBAE_F039_RECORD_NOT_FOUND			"25"			/* impossible de localiser l�enregistrement dans le fichier */
#define CBAE_F039_FORMAT_ERROR				"30"			/* erreur de format									*/
#define CBAE_F039_FUNCTION_NOT_AVAILABLE	"40"			/* fonction demand�e non prise en charge			*/
#define CBAE_F039_UNRESPECTED_SEC_RULES		"63"			/* r�gles de s�curit� non respect�es */
#define CBAE_F039_ISSUER_UNAVAILABLE		"91"			/* �metteur de cartes inaccessible					*/
#define CBAE_F039_MALFUNCTION				"96"			/* mauvais fonctionnement du syst�me, pas de reroutage demand� */
#define CBAE_F039_TIMEOUT					"97"			/* �ch�ance de la temporisation de surveillance globale */
#define CBAE_F039_SRV_UNVAILABLE			"98"			/* serveur indisponible reroutage r�seau demand�	*/
#define CBAE_F039_REVOC_AUTHO_ORDER			"R1"			/* revocation paiement recurrent pour le commercant */
#define CBAE_F039_REVOC_ALL_AUTHO_ORDER			"R3"			/* revocation tous paiements recurrents */
#define CBAE_F039_CONTACT_FALLBACK          "A0"            /* A0 repli en mode contact */ 
#define CBAE_F039_VADS_FALLBACK             "A1"            /* A1 repli VADS */

/**********************************************************/

#define CBAE_F044_FIELD_ERROR			"AA"			/* Champ erron�												*/
#define CBAE_F044_SECURITY_ERROR		"AB"			/* Erreur de s�curit�										*/
#define CBAE_F044_FIELD_CONVERSION		"AC"			/* Conversion de champ										*/
#define CBAE_F044_RESPONSE_REASON		"AD"			/* Pr�cision de la cause du refus ou de l�accord			*/
#define CBAE_F044_NW_ADD_SERVICE		"AE"			/* Compte rendu de service suppl�mentaire rendu � l�acqu�reur par le r�seau */
#define CBAE_F044_MAX_AMOUNT			"BA"			/* Montant maximum pour lequel une autorisation pourra �tre d�livr�e */
#define CBAE_F044_PHONE_NBR				"BB"			/* Num�ro de t�l�phone										*/
#define CBAE_F044_MSG_TO_TRX_SENDER		"BC"			/* Message � destination de l�initiateur de la transaction	*/
#define CBAE_F044_NW_DEVICE_ID			"BD"			/* Identifiant de l��quipement R�seau						*/
#define CBAE_F044_MIN_AMOUNT			"BF"			/* Montant minimum pour lequel une autorisation pourra �tre d�livr�e */
#define CBAE_F044_MAG_CRYPTO_CTRL		"CA"			/* Informations relatives au traitement du cryptogramme donn�es piste ou �quivalentes */
#define CBAE_F044_ICC_CRYPTO_CTRL		"CB"			/* Informations relatives au contr�le du cryptogramme application puce */
#define CBAE_F044_CARDHOLDER_ADD_CTRL	"CC"			/* Informations relatives au contr�le de l'adress porteur *//*SNO300315 Enh4.10*/

/*CBAE_F044_FIELD_ERROR*/
#define CBAE_F044_FE_VALUE_ERROR		"1"				/* Erreur de valeur											*/
#define CBAE_F044_FE_FORMAT_ERROR		"2"				/* Erreur de format											*/
#define CBAE_F044_FE_MISSING_FIELD		"3"				/* Absence injustifi�e d�un champ							*/
#define CBAE_F044_FE_NW_REPORTED_ERROR	"4"				/* Erreur de format ou de correspondance d�tect�e par un r�seau distant */
#define CBAE_F044_FE_FIELD_EXISTS		"5"				/* Pr�sence � tort d�un champ interdit						*/

/*CBAE_F044_SECURITY_ERROR*/
#define CBAE_F044_SE_KEY_NOT_EXISTS			"01"		/* cl� utilis�e inexistante									*/
#define CBAE_F044_FE_INTEGRITY_ERROR		"02"		/* erreur d�int�grit� (champ 64 ou 128 incorrect)			*/
#define CBAE_F044_FE_KEYS_NBR_OVERVLOW		"03"		/* chargement de cl� impossible � nombre maximal de cl�s atteint */
#define CBAE_F044_FE_KEY_EXISTS				"04"		/* cr�ation d�une cl� d�j� existante						*/
#define CBAE_F044_FE_NW_SECURITY_ERROR		"05"		/* erreur de s�curit� dans le domaine r�seau (r�serv� au R�seau) */
#define CBAE_F044_FE_ERROR_DECRYPT			"06"		/* erreur de vraisemblance du champ apr�s d�chiffrement (due � une cl� erron�e) */

/*CBAE_F044_MAG_CRYPTO_CTRL*/
#define CBAE_F044_MC_CTRL_NOT_VERIF			'0'			/* CVV/dCVV/iCVV/CVC/CVC3 non v�rifi�							*/
#define CBAE_F044_MC_CTRL_OK				'1'			/* CVV/dCVV/iCVV/CVC/CVC3 v�rifi� et incorrect					*/
#define CBAE_F044_MC_CTRL_INCORRECT			'2'			/* CVV/dCVV/iCVV/CVC/CVC3 v�rifi� et correct					*/
#define CBAE_F044_MC_CTRL_INVALID			'E'			/* CVC3 v�rifi� et incorrect, invalidit� de la longueur du nombre al�atoire utilis� */
/*CBAE_F044_ICC_CRYPTO_CTRL*/
#define CBAE_F044_ICC_CRYPT_NOT_VERIF		'6'			/* Application Cryptogram non v�rifi�						*/
#define CBAE_F044_ICC_CRYPT_INCORRECT		'7'			/* Application Cryptogram v�rifi� et incorrect				*/
#define CBAE_F044_ICC_CRYPT_OK				'8'			/* Application Cryptogram v�rifi� et correct				*/
#define CBAE_F044_ICC_CRYPT_NOT_SENT		'9'			/* Non transmis par l��metteur (utilisable uniquement par les passerelles internationales) */
#define CBAE_F044_ICC_CRYPT_OK_ATC_REPLY    'E'         /* Valid application cryptogram; ATC replay*//*PLUTONL-4280 :Enh5.03 :1402*/
/**********************************************************/
#define TAG_F047_POS_DATA_CODE			"03"	/* Code de donn�es du point de service								*/
#define TAG_F047_SITE_TYPE				"08"	/* Type de site														*/
#define TAG_F047_RECORD_NUMBER			"24"	/* Num�ro de dossier												*/
#define TAG_F047_CARD_READ_ADD_CAP		"30"	/* CAPACIT� ADDITIONNELLE DE LECTURE DE CARTE						*//*EBE140421: Enh4.08*/
#define TAG_F047_PA_INFO				"31"	/* INFORMATIONS SUR LE POINT D�ACCEPTATION							*//*EBE140421: Enh4.08*/
#define TAG_F047_STATE_CHG_CODE			"71"	/* Code de changement d��tat du champ 22 (Mastercard)				*/
#define TAG_F047_MAG_INTEGRITY_CHECK	"72"	/* Information sur contr�le int�grit� piste magn�tique (Mastercard)	*/
#define TAG_F047_POS_DATA_CAPACITY		"94"	/* Capacit� des donn�es au point d�acceptation						*/
/*#define TAG_F047_NETWORK_DATA			"95"*/	/* Donn�es de r�seau												*/
#define TAG_F047_UNIQUE_TRX_REF			"95"	/* R�f�rence unique de transaction*/ /*AMER20190313 PROD00066569 1064: Label changed*/
#define TAG_F047_SIRET					"96"	/* SIRET															*/
/*#define TAG_F047_GROUPING_ID			"97"	*//* Identifiant de regroupement*/ /*EBE130521: CBAE4.07: changes from 97 to 99 */
#define TAG_F047_CARD_PRODUCT_ID		"98"	/* Identifiant du produit Carte										*//*SNO040416 Enh4.10*/
#define TAG_F047_GROUPING_ID			"99"	/* Identifiant de regroupement										*/


#define F047T03_UNKNOWN					'0'			/* Inconnue															*/
#define F047T03_MANUAL_NO_TERMINAL		'1'			/* Manuel, pas de terminal											*/
#define F047T03_MAG_READER				'2'			/* Lecteur de piste magn�tique										*/
#define F047T03_IC_READER_NCIC			'3'			/* Lecteur de carte � microcircuit sans contact contexte puce EMV	*/
#define F047T03_IC_READER_NOM			'4'			/* Lecteur de carte � microcircuit sans contact contexte piste magn�tique			*/
#define F047T03_IC_READER				'5'			/* Lecteur de carte � microcircuit									*/
#define F047T03_KEYBOARD_INPUT			'6'			/* Saisie clavier													*/
#define F047T03_IC_READER_NO			'8'			/* Lecteur de carte � microcircuit contexte sans contact (valeur utilis�e par VISA � l�incoming) */

/* Start EBE140421: Enh 4.08 (560)*/
#define F047T03_05_CH_PRESENT				'0'		/* Porteur pr�sent								*/
#define F047T03_05_CH_NP_UNKNOWN_RSN		'1'		/* Porteur non pr�sent - raison inconnue		*/
#define F047T03_05_CH_NP_MO					'2'		/* Porteur non pr�sent - commande par courrier	*/
#define F047T03_05_CH_NP_TO					'3'		/* Porteur non pr�sent - commande par t�l�phone	*/
#define F047T03_05_CH_NP_RP					'4'		/* Porteur non pr�sent - Paiement r�current		*/
#define F047T03_05_CH_NP_ON					'9'		/* Porteur non pr�sent - r�seau ouvert			*/
#define F047T03_05_UN						'S'		/* Pr�sence du porteur non d�termin�e			*/

#define F047T03_06_CARD_NP					'0'		/* Carte non pr�sente							*/
#define F047T03_06_CARD_PRESENT				'1'		/* Carte pr�sente								*/
#define F047T03_06_UN						'8'		/* Pr�sence de la carte non d�termin�e			*/

#define F047T03_07_UNKNOWN					'0'		/* Mode inconnu								*/
#define F047T03_07_KEYED_IN					'1'		/* Saisie manuelle, pas de terminal			*/
#define F047T03_07_MAG						'2'		/* Lecture piste magn�tique					*/
#define F047T03_07_BAR_CODE					'3'		/* Lecture code barre						*/
#define F047T03_07_OCR						'4'		/* OCR										*/
#define F047T03_07_CHIP						'5'		/* Lecture micro-circuit					*/
#define F047T03_07_PAN_ENTRY				'6'		/* Saisie clavier							*/
#define F047T03_07_FALLBACK					'9'		/* Lecture piste magn�tique en mode d�grad� pour une carte � microcircuit	*/
#define F047T03_07_CL_CHIP					'S'		/* Lecture sans contact- donn�es microcircuit	*/
#define F047T03_07_CL_MAG					'T'		/* Lecture sans contact � donn�es piste		*/
/* End EBE140421: Enh 4.08 */

#define TAG_F047_CPI_CB					"0"
#define TAG_F047_CPI_VISA				"1"
#define TAG_F047_CPI_MC					"2"

/*Start AMER20190314 PROD00066569: Enh5.00 1052*/
#define TAG_F047_PA_MPOS_TERM              '2' 		/*Terminal mPOS d�di� avec dongle complatible PCI (avec ou sans clavier)*/
#define TAG_F047_PA_OFF_MDEVICE            '3'  		/*Appareil mobile sur �tag�re*/
/*End AMER20190314 PROD00066569: Enh5.00 1052*/

/*************************************************************/
/* Internal Fields of Security data(53) */
 
#define TAG_F053_ARCH_SECURITY				0			/* Architecture de s�curit�									*/
#define TAG_F053_SEC_DATA					2			/* Donn�es de s�curit� utilis�es							*/
#define TAG_F053_AUTH_DATA_ENC_MODE			4			/* Mode de chiffrement des donn�es d'authentification		*/
#define TAG_F053_PINKEY_ENC_MODE			5			/* Mode de chiffrement du code confidentiel ou de la cl�	*/
#define TAG_F053_PIN_FORMAT					6			/* Format du code confidentiel								*/
#define TAG_F053_PINKEY_KEY_ID				8			/* Cl� de chiffrement du code confidentiel/Cl� de chiffrement de la cl� transport�e */
#define TAG_F053_UPDATE_KEY_ID				10			/* Cl� de chiffrement � modifier							*/
#define TAG_F053_MAC_KEY					12			/* Cl� de chiffrement pour la s�curisation des messages		*/
#define TAG_F053_UPDATE_KEY_CODE			14			/* Code de mise � jour de cl�								*/
#define TAG_F053_UPDATE_KEY_TYPE			15			/* Type de cl� mise � jour									*/
#define TAG_F053_KEYS_PACK					16			/* R�f�rence du jeu de cl� utilis�							*/
#define TAG_F053_PINKEY_GROUP				17			/* R�f�rence du groupe de la cl� de chiffrement du code conf. ou de la cl� transport�e */
#define TAG_F053_UPDATE_KEY_GROUP			18			/* R�f�rence du groupe de cl� de chiffrement � modifier		*/
#define TAG_F053_MAC_GROUP					19			/* R�f�rence du groupe de la cl� de scellement				*/
 
/*************************************************************/
#define TAG_F053_AS_CBAE_ISO2				0x01		/* Organisation de cl� CBAE ISO mode 2 */


#define TAG_F053_SD_NO_ZONE					0			/* aucune zone chiffr�e n'est utilis�e						*/
#define TAG_F053_SD_PRIV_ZONE				(1<<0)		/* utilisation d'une zone chiffr�e priv�e (champ 48 des messages d'�change de cl�s) */
#define TAG_F053_SD_PIN_BLOCK				(1<<1)		/* utilisation du champ 52 (code confidentiel chiffr�)		*/
#define TAG_F053_SD_MAC						(1<<2)		/* utilisation du champ 64 ou 128 (donn�es d'authentification) */

#define TAG_F053_ADEM_NONE					0x00
#define TAG_F053_ADEM_ISO_9797				0x03

#define TAG_F053_PKEM_NONE					0
#define TAG_F053_PKEM_TDES					2
#define TAG_F053_PKEM_RFU					3

#define TAG_F053_PF_NONE					0
#define TAG_F053_PF_ISO_9564				0x01

#define CBAE_F053_UKC_NONE					0			/* transaction diff�rente de mise � jour de cl� */
#define CBAE_F053_UKC_ADD_KEY				1			/* ajouter une cl� */
#define CBAE_F053_UKC_UPDATE_KEY			2			/* modifier une cl� */


#define CBAE_F053_TUKT_OTHER			0			/* transaction autre que mise � jour de cl�				*/
#define CBAE_F053_TUKT_KT				1			/* cl� de transport de cl� (champ 48 des messages de mise � jour de cl�) */
#define CBAE_F053_TUKT_KC				2			/* cl� de chiffrement du code confidentiel (champ 52)	*/
#define CBAE_F053_TUKT_KS				4			/* cl� de calcul de la donn�e d'authentification (champ 64 ou 128) */
#define CBAE_F053_TUKT_ALL				7			/* toutes les cl�s										*/
#define CBAE_F053_TUKT_KT_UNDER_KR		8			/* cl� de transport chiffr�e par KR						*/
#define CBAE_F053_TUKT_KS_UNDER_KR		9			/* cl� de scellement chiffr�e par KR					*/
/*************************************************************/
/*Start SNO310316 Enh4.10*/
 /* Field 57 Tags */

#define CBAE_TAG_F057_APPLI_SELECT						"0002"		/* Indicateur s�lection application         */
#define CBAE_TAG_F057_CHOSEN_BRAND						"0003"		/* Marque choisie							*/
/* #define CBAE_TAG_F057_ZIP_CODE					"0005"	*//* Code postal du porteur *//* MTR-Enh4.11*/
#define	CBAE_TAG_F057_CARD_PRODUCT_CODE					"0005" 		/* Code produit carte SA *//* MTR-Enh4.11*/
#define CBAE_TAG_F057_CARDHOLDER_ADDRESS				"0006"		/* Adresse du porteur       				*/
#define CBAE_TAG_F057_ON_BEHALF_SERVICES				"0007"		/* On Behalf Services       				*/

/*End SNO310316 */
#define CBAE_TAG_F057_ZIP_CODE						"0008"		/* Code postal du porteur */ /* MTR-Enh4.11*/
#define CBAE_TAG_F057_DELIVERY_ADDRESS					"0009"		/* Adresse de livraison	*/
#define CBAE_TAG_F057_IP_ADDRESS								"0010"		/* Adresse IP	*/
#define CBAE_TAG_F057_ITEMS_NUMBER							"0011"		/* Nombre d'articles	*/
/*#define CBAE_TAG_F057_SOLUTION_IDENTIFIER				"0012"*/	/* Identifiant de solution de paiement mobile	*/
#define CBAE_TAG_F057_WALLET_IDENTIFIER				    "0012"      /*Wallet Identifier*/ /*PLUTONL-4280 :Enh5.03 :1413*/
#define CBAE_TAG_F057_TRANSACTION_TYPOLOGY			"0013"		/* Typologie de transaction       	*/
#define CBAE_TAG_F057_PROOF_TYPE        				"0014"		/* Type de preuve                   */
#define CBAE_TAG_F057_EC_IND_EXT        				"0015"		/* Indicateur de commerce �lectronique �tendu */
#define CBAE_TAG_F057_ORGINAL_TRX_ID                    "0016"      /* IDENTIFIANT DE LA TRANSACTION D’ORIGINE */
#define CBAE_TAG_F057_CRYPTO_TIME                       "0017"      /* Date et heure GMT de saisie du cryptogramme */
#define CBAE_TAG_F057_CRYPTO_IND                        "0018"      /* Date et heure GMT de saisie du cryptogramme */
#define CBAE_TAG_F057_SERIAL_NUMBER                     "0019"      /* Num�ro de s�rie */
#define CBAE_TAG_F057_RESUBMISSION_COUNTER              "0020"	    /* Compteur de reemission */
/*Start AMER20190314 PROD00066569: Enh5.00*/
/*1053*/
#define CBAE_TAG_F057_3DS_VERSION       			"0022"	    /* Version majeure du protocole 3DS 	  */
/*1073*/
#define CBAE_TAG_F057_CONTAINER_UUID       			"0023"	    /* Container UUID 	  					  */
/*1070*/
#define CBAE_TAG_F057_ISO              				"0024"	    /* Organisation de vente ind�pendante 	  */
#define CBAE_TAG_F057_PAY_FAC_ID              		"0025"	    /* Identifiant du facilitateur de paiement*/ 
#define CBAE_TAG_F057_MARKET_PLACE_ID              	"0026"	    /* Identifiant du market place			  */ 
#define CBAE_TAG_F057_MERCHANT_ID              		"0027"	    /* Identifiant du commer�ant final 		  */
/*1075*/
#define CBAE_TAG_F057_PAYMENT_SCENARIO              "0028"	    /* Cas de paiement 		  				  */
#define CBAE_TAG_F057_ACTION_CARD_ON_FILE           "0029"	    /* Action card-on-file	  				  */
#define CBAE_TAG_F057_TERM_NUMBER    	            "0031"	    /* Num�ro d��ch�ance	  				  */
#define CBAE_TAG_F057_TERM_TOTAL_NBR                "0032"	    /* Nombre total d'�ch�ances	  			  */
#define CBAE_TAG_F057_AUTH_EXEMPTION_IND            "0033"	    /* Indicateur d�exemption	  			  */
/*936*/
#define CBAE_TAG_F057_RISK_ANALYSIS_IND       		"0034"	    /* Indicateurs d�analyse de risques	  	  */
#define CBAE_TAG_F057_ACC_DATA_COMP_INFO       		"0035"	    /* Informations de compromission de donn�es de compte*/
/*1073*/
#define CBAE_TAG_F057_MERCHANT_NAME       			"0036"	    /* Nom du commer�ant 	  			      */
#define CBAE_TAG_F057_AUTH_DATE_TIME  				"0037"	    /* Date de l�authentification			  */
#define CBAE_TAG_F057_PURCHASE_AMOUNT      			"0038"	    /* Montant pr�sent� � l�authentification  */
/*989*/
#define CBAE_TAG_F057_AUTHENTICATION_IND   			"0039"	    /* Indicateur d'authentification  */
/*1062*/

#define CBAE_TAG_F057_PAYMENT_VALIDITY_DATE			"0045"		/* MAK_20200413 PROD00074712: Enh5.01 1140*/

#define CBAE_TAG_F057_PAY_ACC_REF              		"0056"	    /* Payment account reference */
/* YEL PROD00073659 */
#define CBAE_TAG_F057_DATE_CREAT_PAR                     "0057"      /* Date create PAR  (Payment account reference) */
/*1053*/
#define CBAE_TAG_F057_APPLE_PAY						"00"		/* Apple pay */	
#define CBAE_TAG_F057_SAMSUNG_PAY					"01"		/* Samsung pay */	
#define CBAE_TAG_F057_ANDROID_PAY					"02"		/* Android pay */	
#define CBAE_TAG_F057_GARMIN_PAY					"03"		/* Garmin  Pay */           /*PLUTONL-4280 :Enh5.03 :1413*/
#define CBAE_TAG_F057_MERCHANT_TOKENISATION		    "10"		/* Merchant Tokenisation *//*PLUTONL-4280 :Enh5.03 :1413*/
/*1053*/
#define CBAE_TAG_F057_CONTAINER_UUID_DS					'1'			/* DS Transaction ID */		
#define CBAE_TAG_F057_CONTAINER_UUID_ACS				'2'	    	/* ACS Transaction ID */	
																		   
/*End AMER20190314 PROD00066569: Enh5.00 1070*/		
 /* Field 59 Tags */

#define TAG_F059_FUNCTION_CODE				"0100"		/* Code fonction												*/
#define TAG_F059_MESSAGE_REASON				"0101"		/* Code raison du message										*/
#define TAG_F059_TRANSACTION_YEAR			"0102"		/* Ann�e de la transaction										*/

#define TAG_F059_TRX_ENV					"0200"		/* Environnement r�glementaire et technique de la transaction	*/
#define TAG_F059_ITP						"0201"		/* ITP (Identification d�application du Terminal)(Syst�me d�acceptation)*/
#define TAG_F059_ACCEPTOR_CONTRACT_NO		"0202"		/* Num�ro de contrat accepteur									*/
#define TAG_F059_SA_LOGICAL_NO				"0203"		/* Num�ro logique du syst�me d'acceptation						*//*EBE140421: Enh4.08*/
#define TAG_F059_PA_LOGICAL_NO				"0204"		/* Num�ro logique du point d'acceptation						*//*EBE140421: Enh4.08*/
#define TAG_F059_ACCEPT_TERM_COUNTRY		"0205"		/* Code pays du syst�me d'acceptation							*/
#define TAG_F059_CH_CUM_AMOUNT				"0207"		/* Montant cumul� par porteur									*/
#define TAG_F059_PRE_AUT_CHECK_VAL_PERIOD	"0208"		/* Dur�e de validit� d'une facture pr�-autoris�e				*/
/* START MTR-Enh4.11 */
#define TAG_F059_CLIENT_REF_1				"0210"		/* R�f�rence client 1											*/
#define TAG_F059_CLIENT_REF_2				"0211"		/* R�f�rence client 2											*/
#define TAG_F059_MARKET_NUMBER				"0212"		/* Num�ro de march�												*/
#define TAG_F059_TVA_AMOUNT					"0213"		/* Montant de TVA												*/
/* END MTR-Enh4.11 */
#define TAG_F059_PAYMENT_SERVICE_DATA		"0214"		/* Donn�es services de paiement									*/
#define TAG_F059_ITP_PA						"0215"		/* ITP du Point d�acceptation									*//*EBE140421: Enh4.08*/
	
#define TAG_F059_VISUAL_CRYPTO				"0300"		/* Cryptogramme visuel											*/
#define TAG_F059_VISUAL_CRYPT_INFO			"0301"		/* Informations relatives au traitement du cryptogramme visuel	*/
#define TAG_F059_CVD_NBR					"0302"		/* Num�ro de CVD (carte virtuelle dynamique)					*/
#define TAG_F059_CVD_EXPIRY_DATE			"0303"		/* Date d�expiration CVD (carte virtuelle dynamique)			*/


#define TAG_F059_ACC_TRX_ID					"0400"		/* Identifiant transaction fourni par l'accepteur */
#define TAG_F059_EC_CRYPTO					"0401"		/* Cryptogramme de commerce �lectronique */
/*#define TAG_F059_EC_SEC_TYPE				"0407"*/	/* Type de s�curisation de transaction de commerce �lectronique */
#define TAG_F059_EC_AUTH_TYPE               "0407"      /* Type d'authentification de transaction de commerce �lectronique *//*PLUTONL-4280 :Enh5.03 :1345*/
#define TAG_F059_EC_CRYPTO_INFO				"0409"		/* Informations relatives au traitement du cryptogramme de commerce �lectronique */
#define TAG_F059_CH_ISS_AUTH				"0410"		/* M�thode d'authentification porteur utilis�e par l'�metteur */
#define TAG_F059_EC_CRYPTO_METHOD			"0411"		/* M�thode de calcul du cryptogramme de commerce �lectronique */
#define TAG_F059_VADS_USE_RESULT			"0412"		/* R�sultat de l'utilisation de l'architecture de paiement s�curis� VADS */
/*#define TAG_F059_TRX_SECURING				"0413"*/	/* S�curisation de la transaction */
#define TAG_F059_MODIFIED_EC_AUTH_TYPE		"0413"		/* Type de s�curisation de transaction de commerce �lectronique Modifié*//*PLUTONL-4280 :Enh5.03 :1345*/ 
#define TAG_F059_EC_COMPLEMENTARY_DATA		"0414"		/* Donn�es compl�mentaires de commerce �lectronique */
#define TAG_F059_DENOM_PRTF_NUMERIQUE		"0415"		/* DENOMINATION DU PORTEFEUILLE NUMERIQUE *//*SNO150330  Enh 4.09 --751 */
#define TAG_F059_IND_COMMERCE_ELEC			"0416"		/* INDICATEUR DE COMMERCE ELECTRONIQUE *//*SNO150330  Enh 4.09 --751 */
#define TAG_F059_WALLET_COMPLEMENT_DATA		"0417"		/* DONNEES ADDTIONNELLES PORTEFEUILLES NUM�RIQUES *//*SNO150330  Enh 4.09 --712 */
#define TAG_F059_ID_WALLET					"0418"		/* IDENTIFIANT WALLET *//*SNO150330  Enh 4.09 --712 */
#define TAG_F059_3DS_EXT_RESULT  			"0419"		/* Extension du r�sultat de l'utilisation de l'architecture de paiement s�curis� */ /*AMER20190314 PROD00066569: Enh5.00 1075*/

#define TAG_F059_VISA_CPS_ACI				"0600"		/* VISA CPS/ATM ACI */
#define TAG_F059_VISA_TRX_ID				"0601"		/* VISA Transaction Identifier */
#define TAG_F059_VISA_VAL_CODE				"0602"		/* VISA Validation Code */
#define TAG_F059_VISA_AGENT_UNIQUE_ID		"0603"		/* AGENT UNIQUE ID					*//*EBE140421: Enh4.08*/
#define TAG_F059_VISA_SPEND_QUAL_IND		"0604"		/* SPEND QUALIFIED INDICATOR		*//*EBE140421: Enh4.08*/
#define TAG_F059_VISA_ACCEPT_PROP			"0605"		/* CARACT�RISTIQUES D�ACCEPTATION	*//*EBE140421: Enh4.08*/


#define TAG_F059_MC_CRP_DRV_NO				"0702"		/* MasterCard Corporate Fleet Card ID / driver number */
#define TAG_F059_MC_VEHICULE_NO				"0703"		/* MasterCard Corporate Fleet Card vehicule number */
#define TAG_F059_ADD_RESP_DATA				"0704"		/* Additional response data */
#define TAG_F059_FINAL_AUTH_IND				"0705"		/* FINAL AUTHORIZATION INDICATOR		*//*EBE140421: Enh4.08*/
#define TAG_F059_TRANSIT_TRX_TYPE_IND		"0706"		/* TRANSIT TRANSACTION TYPE INDICATOR	*//*EBE140421: Enh4.08*/
#define TAG_F059_ON_BEHALF_SERVICES		    "0707"		/* ON BEHALF SERVICES	*//*SNO150330  Enh 4.09 --750 */
#define TAG_F059_ADVICE_REASON_CODE		    "0708"		/* ADVICE REASON CODE	*//*SNO150330  Enh 4.09 --750 */
#define TAG_F059_DATA_PAYMENT_FACILITATOR	"0709"		/* Donnees facilitateur de paiement	*//*SNO150408  Enh 4.09 --771 */
#define TAG_F059_DEVICE_TYPE				"0710"		/* Device type	*//*SNO040416 Enh4.10 */

#define TAG_F059_CHECK_TYPE					"0800"		/* Type de facture / procedure */
#define TAG_F059_MERCHANT_ADVICE			"0801"		/* Avis au commercant */
#define TAG_F059_RISK_SERVICE				"0802"		/* Service Risque X */
#define TAG_F059_AUT_INT_NET_ID				"0803"		/* Identification du reseau international d'autorisation */
#define TAG_F059_ROUTING_REASON				"0804"		/* Motif du reroutage par le Reseau */
#define TAG_F059_OPTIONAL_SERVICE			"0805"		/* Services optionnels supportes (domaine acquereur) */
#define TAG_F059_STATE_CODE					"0806"		/* CODE ETAT/PROVINCE DE L'ACCEPTEUR */ /* Added on CBAE 4.07 */
#define TAG_F059_ISS_COUNTRY				"0807"		/* Code pays de l'metteur */
#define TAG_F059_MSG_ORIGIN					"0808"		/* Message Origin */ /* Added on CBAE 4.07 */

/** MTR Enh4.12 FUNCTION CODE LENGTH = 3 not 4
#define TAG_F059_FC_INIT_REQ_EXACT_AMNT			"0100"		
#define TAG_F059_FC_INIT_REQ_PRE_AMNT			"0101"		
#define TAG_F059_FC_ADD_REQ_EXACT_AMNT			"0106"		
#define TAG_F059_FC_ADD_REQ_PRE_AMNT			"0107"		
#define TAG_F059_FC_INFO_REQUEST				"0108"		
#define TAG_F059_FC_CASHBACK					"0109"		
#define TAG_F059_FC_COMPLEMENTARY_NOTE			"0163"		
#define TAG_F059_FC_NOSHOW_NOTE					"0164"		
#define TAG_F059_FC_LEN							4
*/
/* MTR Enh4.12 */
#define TAG_F059_FC_INIT_REQ_EXACT_AMNT			"100"		/* autorisation initiale � montant exact */
#define TAG_F059_FC_INIT_REQ_PRE_AMNT			"101"		/* autorisation initiale � montant estim� */
#define TAG_F059_FC_ADD_REQ_EXACT_AMNT			"106"		/* autorisation suppl�mentaire � montant exact */
#define TAG_F059_FC_ADD_REQ_PRE_AMNT			"107"		/* autorisation suppl�mentaire � montant estim� */
#define TAG_F059_FC_INFO_REQUEST				"108"		/* Demande de renseignement */
#define TAG_F059_FC_CASHBACK					"109"		/* Cashback */
#define TAG_F059_FC_COMPLEMENTARY_NOTE			"163"		/* Facture compl�mentaire */
#define TAG_F059_FC_NOSHOW_NOTE					"164"		/* Facture no-show */
#define TAG_F059_FC_LEN							3
/* MTR Enh4.12 */

#define TAG_F059_RSC_PREAUTH_COMPL				"1191"		/* Finalisation d�une transaction initiale de pr�-autorisation (Incoming)	*/
#define TAG_F059_RSC_PREAUTH_REQ				"1655"		/* Demande de pr�-autorisation												*/
#define TAG_F059_RSC_PREAUTH_FINAL_AMNT			"1662"		/* Montant de cl�ture d�une facture pr�-autoris�e.							*/
#define TAG_F059_RSC_PROV_AGGR_AMOUNTS          "1679"      /* Provision pour montants agr�g�s */ /*MTR Enh4.12 */
#define TAG_F059_RSC_LEN						4

#define TAG_F059_ECST_NOT_SECURE				"08"		/* Non s�curis�e		*/
#define TAG_F059_ECST_SECURE_OTHER				"09"		/* S�curis�e par tout autre moyen que ceux pr�cis�s par les autres valeurs */
#define TAG_F059_ECST_SECURE					"20"		/* VADS (Vente A Distance S�curis�e) */
#define TAG_F059_ECST_SECURE_MOBILE				"21"		/* VADS S�curis�e par mobile */
#define TAG_F059_ECST_LEN						2
/*Start AMER20190314 PROD00066569: Enh5.00*/
/*1075*/
#define TAG_F059_3DS_AUTH_TYPE_CH				"CH"		/* Challenge */		
#define TAG_F059_3DS_AUTH_TYPE_FR				"FR"		/* Frictionless */	
#define TAG_F059_3DS_AUTH_TYPE_LEN				2

#define TAG_F059_3DS_SCORE_LEN					2

/**800*/
#define TAG_F059_SVC_ATT_RECURRING			"00"
#define TAG_F059_SVC_ATT_PREAUTH			"02"
#define TAG_F059_SVC_ATT_ADD_PREAUTH		"03"
#define TAG_F059_SVC_ATT_AGGREG				"05"
#define TAG_F059_SVC_ATT_FIRST_RECURRING	"06"
#define TAG_F059_SVC_ATT_NEXT_RECURRING		"07"
#define TAG_F059_SVC_ATT_CRYPTO_CCY			"08"
#define TAG_F059_SVC_ATT_INSTALLMENT		"09"
#define TAG_F059_SVC_ATT_CH_STORAGE_COF		"10"
#define TAG_F059_SVC_ATT_DEBT_RECOVERY		"11"
#define TAG_F059_SVC_ATT_LEN				2

/*766*/
#define TAG_F059_CRPT_GEN_BY_WALLET_SOL		   'W'			/*Cryptogramme g�n�r� par une solution wallet*/
/*End AMER20190314 PROD00066569: Enh5.00 1075*/
/*****************************************************************************/

#define CBAE_F070_OWNER_SENDER		"101"		/* Le cr�ateur de cl�s est l'�metteur du message */
#define CBAE_F070_OWNER_RECEIVER	"140"		/* Le cr�ateur de cl�s est le r�cepteur du message */
#define CBAE_F070_ECHO				"301"		/* Test d'�cho */

/*****************************************************************************/
#define CBAE_F091_ADD_RECORD			"1"
#define CBAE_F091_CHG_RECORD			"2"
/*****************************************************************************/
/*Start PLUTONL-4280 :Enh5.03 :1355*/
/*105*/
#define TAG_F105_PAYMENT_TOKEN                   "01"    /*PAYMENT TOKEN*/
#define TAG_F105_TOKEN_EXPIRY_DATE               "02"    /*TOKEN EXPIRY DATE*/
#define TAG_F105_TOKEN_REQUESTOR_ID              "03"    /*TOKEN REQUESTOR ID*/
#define TAG_F105_TOKEN_ASSURANCE_METHOD          "04"    /*TOKEN ASSURANCE METHOD*/
#define TAG_F105_TOKEN_LOCATION                  "07"    /*TOKEN LOCATION*/
#define TAG_F105_TOKEN_STATUS                    "08"    /*TOKEN STATUS*/
#define TAG_F105_LAST_4_DIGITS_PAN               "10"    /*LAST FOUR DIGITS OF PAN*/
#define TAG_F105_PAN                             "11"    /*PAN*/
#define TAG_F105_PAN_EXPIRY_DATE                 "12"    /*PAN EXPIRY DATE*/
#define TAG_F105_ISSUER_ID                       "13"    /*ISSUER ID*/
#define TAG_F105_PAN_REFERENCE_ID                "14"    /*PAN REFERENCE ID*/
#define TAG_F105_TOKEN_REFERENCE_ID              "15"    /*TOKEN REFERENCE ID*/
/*End PLUTONL-4280 :Enh5.03 :1355*/
/*****************************************************************************/
#define TAG_F108_TOKEN_PROGRAM_INDICATOR         "0359" /*TOKEN PROGRAM INDICATOR*//* PLUTONL-4280 :Enh5.03 :1399*/

/******************************************************************************/
#define TAG_F112_TRX_ORIG_DATA			"01"			/* Donn�es d�origine de la transaction */
#define TAG_F112_TRX_COMPL_DATA			"02"			/* Donn�es compl�mentaires li�es � la transaction */
#define TAG_F112_TRX_APP_ID				"03"			/* Identifiant du type d�application � l�origine de la transaction de transfert de fonds */
#define TAG_F112_TRX_FUND_TRANSFER		"04"			/* Donn�es li�es au transfert de fonds */ /* SNO150330 Enh 4.09 --744 */

#define TAG_F112_COUNTERPART_NAME	    "07"            /* Nom et pr�nom contrepartie */ /* MTR Enh4.12 Fiche968 */
#define TAG_F112_TRANSFER_MOTIF	        "08"            /* Libell� motif du transfert */ /* MTR Enh4.12 Fiche968 */
#define TAG_F112_TRX_IBAN   	        "10"            /* IBAN */ /* MTR Enh4.12 Fiche968 */
/*****************************************************************************/
#define TAG_F113_MOBILE_PHONE			"01"
/*****************************************************************************/
#define TAG_F114_VISA_MER_PROGRAM		"30"			/* VISA - Programme Commer�ant (SELECT MERCHANT FEE) */
#define TAG_F114_DEBT_REPAYMENT  		"31"			/* VISA - Service Debt Repayment */ /* SNO150330 Enh 4.09 --745 */
#define TAG_F114_ACTIVITY_RSLT_SERVICE  "32"			/* NBO-HABIL501 PROD00075275: 1184 Enh5.01*/
#define TAG_F114_MC_ALM					"60"			/* MASTERCARD - Programme Account Level Management (ALM) */
#define TAG_F114_FOREIGN_RATE_INFO		"33"            /*ASO20210419 : Enh5.02 1288*/
#define TAG_F114_AUTHORISATION_POSTING_CODE      "34"           /*PLUTONL-4280 :Enh5.03 :1380*/
/*****************************************************************************/
/*PLUTONL-4280 :Enh5.03 :1346*/
#define CBAE_TAG_F115_PA_NEXO/*CBAE_TAG_F115_PA_OSCAR*/					"0001"			/* Identifiant PA Nexo */ /* SNO010416 Enh4.105 */
#define CBAE_TAG_F115_SA_NEXO/*CBAE_TAG_F115_SA_OSCAR*/  				"0002"			/* Identifiant SA Nexo */ /* SNO010416 Enh4.105 */
#define CBAE_TAG_F115_CERT_NEXO/*CBAE_TAG_F115_CERT_OSCAR*/				"0003"			/* Certificat Nexo */     /* SNO010416 Enh4.105 */
/*****************************************************************************/
#define TAG_F116_SICB_MOV_PRES			"01"			/* Pr�sentateur du mouvement SICB */
/*****************************************************************************/
#define TAG_F117_SL_REASON				"01"			/* Raison de mise en opposition				*/
#define TAG_F117_REC_VAL_DATE			"02"			/* Date de validit� de l'enregistrement		*/
#define TAG_F117_CH_TITLE				"04"			/* Titre du porteur							*/
#define TAG_F117_CARD_TYPE				"05"			/* Type de carte							*/
#define TAG_F117_DECLARED_DATE			"06"			/* Date d�clar�e							*/
#define TAG_F117_DATE_TIME				"07"			/* Date et heure de prise en compte			*/
#define TAG_F117_CH_NAME				"08"			/* Nom du porteur							*/
#define TAG_F117_PIN_LOST				"09"			/* Perte du code confidentiel				*/
#define TAG_F117_ORIGIN					"10"			/* Top for�age et provenance				*/
#define TAG_F117_LOST_COUNTRY			"11"			/* Code pays de la perte ou du vol			*/
#define TAG_F117_LOST_DEP				"12"			/* Code d�partement de la perte ou du vol	*/
#define TAG_F117_FRAUD_TYPE				"13"			/* Type de fraude							*/
#define TAG_F117_IBAN					"23"			/* IBAN (International Bank Account Number) */
/*****************************************************************************/
/*Start AMER20190228 PROD00066569: Enh5.00 1046*/
#define TAG_F118_NOMENCLATURE				"0001"		
#define TAG_F118_TRX_REF_ID   			    "1000"	
#define TAG_F118_TRX_APP_ID			        "1001"	
#define TAG_F118_FUND_SOURCE			    "1002"
#define TAG_F118_FUND_PURPOSE		        "1003"
#define TAG_F118_MESSAGE				    "1004"
#define TAG_F118_CLIENT_LANG			    "1005"
#define TAG_F118_CLIENT_LANG_MSG		    "1006"
#define TAG_F118_SENDER_ID_PAR_ORG		    "2000"
#define TAG_F118_SENDER_PAN			        "2001"
#define TAG_F118_SENDER_FIRST_NAME		    "2002"
#define TAG_F118_SENDER_SECOND_NAME	        "2003"
#define TAG_F118_SENDER_LAST_NAME		    "2004"
#define TAG_F118_SENDER_ADDRESS		        "2005"
#define TAG_F118_SENDER_ZIP_CODE		    "2006"
#define TAG_F118_SENDER_CITY			    "2007"
#define TAG_F118_SENDER_STATE			    "2008"
#define TAG_F118_SENDER_COUNTRY		        "2009"
#define TAG_F118_SENDER_TELEPHONE		    "2010"
#define TAG_F118_SENDER_BIRTHDAY		    "2011"
#define TAG_F118_SENDER_BIC			        "2012"
#define TAG_F118_SENDER_IBAN			    "2013"
#define TAG_F118_SENDER_ACC_NBR		        "2014"
#define TAG_F118_SENDER_DOC_ID			    "2015"
#define TAG_F118_SENDER_ID_NUMBER		    "2016"
#define TAG_F118_SENDER_ID_COUNTRY		    "2017"
#define TAG_F118_SENDER_NATIONALITY	        "2018"
#define TAG_F118_PAYER_ACCOUNT_TYPE	        "2019" /*ASO20210305 : Enh5.02 1292*/
#define TAG_F118_RECIPIENT_PAN			    "3001"
#define TAG_F118_RECIPIENT_FIRST_NAME	    "3002"
#define TAG_F118_RECIPIENT_SECOND_NAME      "3003"
#define TAG_F118_RECIPIENT_LAST_NAME	    "3004"
#define TAG_F118_RECIPIENT_ADDRESS		    "3005"
#define TAG_F118_RECIPIENT_ZIP_CODE	        "3006"
#define TAG_F118_RECIPIENT_CITY		        "3007"
#define TAG_F118_RECIPIENT_STATE		    "3008"
#define TAG_F118_RECIPIENT_COUNTRY		    "3009"
#define TAG_F118_RECIPIENT_TELEPHONE	    "3010"
#define TAG_F118_RECIPIENT_BIRTHDAY	        "3011"
#define TAG_F118_RECIPIENT_BIC			    "3012"
#define TAG_F118_RECIPIENT_ACC_NBR		    "3014"
#define TAG_F118_RECIPIENT_DOC_ID		    "3015"
#define TAG_F118_RECIPIENT_ID_NUMBER	    "3016"
#define TAG_F118_RECIPIENT_ID_COUNTRY	    "3017"
#define TAG_F118_RECIPIENT_NATIONALITY	    "3018"
#define TAG_F118_PAYEE_ACCOUNT_TYPE	        "3019" /*ASO20210305 : Enh5.02 1292*/
#define TAG_F118_PAYER_ID_SUB_TYPE	        "2020" /*PLUTONL-4280 :Enh5.03 :1383*/
#define TAG_F118_PAYEE_ID_SUB_TYPE	        "3020" /*PLUTONL-4280 :Enh5.03 :1383*/
/*End AMER20190228 PROD00066569: Enh5.00 1046/
/*****************************************************************************/
/*Start SNO010416 Enh4.10 */
#define CBAE_TAG_F119_DATA_MONEYSEND	"0100"			/* Donn�es transfert de fonds */

#define CBAE_F119_T0100_SENDER_FIRST_NAME					0
#define CBAE_F119_T0100_SENDER_MIDDLE_NAME					35
#define CBAE_F119_T0100_SENDER_LAST_NAME					36
#define CBAE_F119_T0100_SENDER_STREET_ADDR					71
#define CBAE_F119_T0100_SENDER_CITY							121
#define CBAE_F119_T0100_SENDER_STATE_PROVINCE_CODE			146
#define CBAE_F119_T0100_SENDER_COUNTRY						149
#define CBAE_F119_T0100_SENDER_POSTAL_CODE					152
#define CBAE_F119_T0100_SENDER_ACCOUNT_NUMBER				162
#define CBAE_F119_T0100_UNIQUE_TRANSACTION_REFERENCE		172
#define CBAE_F119_T0100_FUNDING_SOURCE						191
#define CBAE_F119_T0100_LANGUAGE_IDENTIFICATION				192
#define CBAE_F119_T0100_LANGUAGE_DATA						195

/*IBO20220817 PLUTONL-5088#define CBAE_TAG_F119_DIGITAL_PAYMENT_CRYPTOGRAM	"0015"*/ /* START MAK20200323 PROD00074451: Enh5.01 1177*/ /* NBO-HABIL501 */

/* START ASO20200321 PROD00074655: Enh5.01 1143 */

#define TAG_119_TRUSTED_MERCHANT_EXEMPTION_INDICATOR            "0002"
#define TAG_119_TRA_EXEMPTION_INDICATOR                                         "0003"
#define TAG_119_REASONS_FOR_NOT_HONORING_EXEMPTIONS             "0004"
#define TAG_119_TRA_SCORE                                                                       "0005"
#define TAG_119_SESSION_ID                                                                      "0006"
#define TAG_119_3DS_INDICATOR                                                           "0007"
#define TAG_119_SERVICE_INDICATOR                                                       "0008"
#define TAG_119_MERCHANT_IDENTIFIER                                                     "0009"
#define TAG_119_DELEG_AUTH_EXEMPTION_IND                        "0017" /*NAB :A ajouter au niveau Projet:PLUTONL-4280 :Enh5.03 :1375*/
/*#define TAG_119_DIGITAL_PAYMENT_CRYPTOGRAM                                      "0015" /* START ASO20200316 PROD00074451: Enh5.01 1177*/
/*AMA_07082020 PROD00075766:Enh5.01 JuneUpdate*/
#define TAG_119_INITIAL_TRX_ECOM							"0046" /* MAK_20200413 PROD00074712: Enh5.01 1140*/
#define TAG_119_ECOM_DATA_INITIAL_TRX						"0420" /* MAK_20200413 PROD00074712: Enh5.01 1140*/
#define TAG_119_CARDHOLDER_ID_MTHD_IND						"0012" /*MAK_20200428 PROD00074844 : Enh5.01 1161*/
#define TAG_119_3D_SECURE_COMPONENTS_AVAILABILTY			"0013" /*MAK_20200428 PROD00074908 : Enh5.01 1148*/
#define TAG_119_DEBIT_UNIQUE_REF_ID			                "0047" /*ASO20210305 : Enh5.02 1257*/
#define TAG_119_PROMOTION_TYPE		                        "0100" /*ASO20210415 : Enh5.02 1277*/
#define TAG_119_PROMOTION_CODE		                        "0101" /*ASO20210415 : Enh5.02 1277*/
#define TAG_119_REATTEMPT_INDICATOR                         "0801" /*PLUTONL-4280 :Enh5.03 :1348*/
#define TAG_119_REATTEMPT_FROZEN_PERIOD                     "0802" /*PLUTONL-4280 :Enh5.03 :1348*/
#define TAG_119_REATTEMPT_CONDITIONS                        "0803" /*PLUTONL-4280 :Enh5.03 :1348*/
#define TAG_119_CARDHOLDER_VERIF_MTHD_POS                   "1022" /*PLUTONL-4280 :Enh5.03 :1420*/
#define TAG_119_CIT_MIT_INDICATORS                          "0027" /*PLUTONL-4280 :Enh5.03 :1354*/
#define TAG_119_MERCHNT_CNTRY_OF_ORIGIN                     "0374" /*PLUTONL-4280 :Enh5.03 :1363*/
/*Start PLUTONL-4280 :Enh5.03 :1372*/
#define TAG_119_3DS_APP_IP_ADDRESS                          "0375"
#define TAG_119_3DS_BROWSER_IP_ADDRESS                      "0376"
#define TAG_119_AUTHENTICATION_PROGRAM_INDIC                "0377"
#define TAG_119_EMV_SDK_DATA                                "0378"
#define TAG_119_SCHEME_AUTHENTICATION_DATA                  "0379"
/*End  PLUTONL-4280 :Enh5.03 :1372*/
/*#define TAG_119_TRANSFER_SERVICE_PROVIDER_NAME            "0018" */ /*PLUTONL-4280 :Enh5.03 :1383*/
#define TAG_119_ACCEPTOR_LEGAL_BUSINESS_NAME                "0018" /*PLUTONL-4280 :Enh5.03 :1383 MAJ */
#define TAG_119_PAYMENT_FACILITATOR_NAME                    "0019"  /*PLUTONL-4280 :Enh5.03 :1383*/
#define TAG_119_WATCH_LIST_RESULTS_CODE                     "0020"  /*PLUTONL-4280 :Enh5.03 :1383*/

/* END ASO20200321 PROD00074655: Enh5.01 1143 */
/*End SNO010416 Enh4.10 */
/*Start PLUTONL-4280 :Enh5.03 :1420*/
#define B1_F119_1022_CVM                     7
#define B1_F119_1022_OFF_PIN_ENCRYPTED       5
#define B1_F119_1022_OFF_PIN_CLEAR           4
#define B1_F119_1022_ONLINE_PIN              3
#define B1_F119_1022_SIGNATURE               2
#define B1_F119_1022_NO_CVM                  1
#define B1_F119_1022_UNKNOWN                 0
/*End PLUTONL-4280 :Enh5.03 :1420*/
/*Start PLUTONL-4280 :Enh5.03 :1354*/
#define CBAE_CIT					"C1"
#define CBAE_MIT_RECURRING		    "M1"
#define CBAE_MIT_INDUSTRY		    "M2"

#define	 CBAE_F119_CIT_CREDENTIAL		   "C101"
#define	 CBAE_F119_CIT_STANDING            "C102"
#define	 CBAE_F119_CIT_SUBSCRIPTION        "C103"
#define	 CBAE_F119_CIT_INSTALLMENT         "C104"
#define	 CBAE_F119_MIT_UNSCHEDULED         "M101"
#define	 CBAE_F119_MIT_STANDING_ORDER      "M102"
#define	 CBAE_F119_MIT_SUBSCRIPTION        "M103"
#define	 CBAE_F119_MIT_INSTALLMENT         "M104"
#define	 CBAE_F119_MIT_PARTIAL_SHIPMENT    "M205"
#define	 CBAE_F119_MIT_DELAYED_CHARGE      "M206"
#define	 CBAE_F119_MIT_NO_SHOW_CHARGE      "M207"
#define	 CBAE_F119_MIT_RESUBMISSION        "M208"
#define  CBAE_F119_CIT_MIT_LEN			    4
/*End PLUTONL-4280 :Enh5.03 :1354*/
/*Start PLUTONL-4280 :Enh5.03 :1373*/
#define OBTAIN_INFO_NEXT_TRX                  "01"
#define TRY_AGAIN_LATER                       "02"
#define NEVER_TRY_AGAIN                       "03"
#define REATTEMPT_DURATION_MAXIMUM_REATTEMPTS "072015"
/*End PLUTONL-4280 :Enh5.03 :1373*/

#define VISA_ECOM_DATA_LEN             10 /*PLUTONL-4280*/
/*Error codes */

#define ERR_KEY_NOT_FOUND				1
#define ERR_MAC_CHECK_FAILURE			2
#define ERR_UNKNOWN_KEY_TYPE			3
#define ERR_UNKNOWN_UPDATE_CODE			4
#define ERR_KEY_TRANSLATION				5

/* File update error codes */
#define ERR_UNKNOWN_REASON_CODE			200
#define ERR_OP_NOT_SUPPORTED			201
#define ERR_S_L_REC_EXISTS				202
#define ERR_S_L_REC_NOT_EXISTS			203

/* network management error codes */
#define ERR_KEY_EXISTS					300
#define ERR_KEY_NBR_OVERFLOW			301
#define ERR_KEYPACK_INVALID				302
#define ERR_KEYPACK_NOTEXIST			303
#define ERR_KEYPACK_RENEWAL				304
#define ERR_KEYPACK_VALID				305
#define ERR_KEYPACK_FROZEN				306
#define ERR_KEYPACK_EXPIRED				307



#define CBAE_FT_PT_CARTE_A_CARTE			"CC"		/* Transfert carte � carte								*/
#define CBAE_FT_PT_EPURSE_UNLOAD			"DE"		/* D�chargement de compte de monnaie �lectronique		*/
#define CBAE_FT_PT_ECO_COL_B2B				"EB"		/* Economie collaborative B2B							*/
#define CBAE_FT_PT_ECO_COL_B2C				"EC"		/* Economie collaborative B2C							*/
#define CBAE_FT_PT_SERVICES_PAYMENT			"PA"		/* Paiement de prestations professionnel vers particulier	*/
#define CBAE_FT_PT_EARNING_PAYMENT			"PG"		/* Paiement des gains									*/
#define CBAE_FT_PT_REFUND					"RA"		/* Remboursement d�achats non r�gl�s par carte			*/
#define CBAE_FT_PT_FUND_TRSF				"RE"		/* Transfert de fonds sur r�cepteur de fonds			*/


#define		ERSB_ACQUIRER_ID                   "E15"
#define		ERSB_CONNECTED_ENTITY              "E16"
#define		ERSB_USER_APPLICATION              "E17"
#define		ERSB_SERVER_ID                     "E18"
#define		ERSB_SICB_IDENTIFIER               "E19"
#define		ERSB_COUNTRY_CODE                  "E20"
#define		ERSB_BANK_LOCATION                 "E21"
#define		ERSB_NB_KEY_PACKS                  "E22"

#define TAG_F118_TRX_APP_ID_LEN			     2		


#endif

