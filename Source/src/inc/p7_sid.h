#ifndef P7__SID__H
#define P7__SID__H

#define P7_F002_MAX_LEN					19


/* Processing codes */
#define P7_F003_PURCHASE				"00"		/* Purchases & Services							*/
#define P7_F003_WITHDRAWAL				"01"		/* Withdrawal									*/
#define P7_F003_ADJ_DEBIT				"02"		/* Debit adjustment								*/
#define P7_F003_CHECK_GUARANTEE			"03"		/* Check guarantee								*/
#define P7_F003_CHECK_VERIF				"04"		/* Check verification							*/
#define P7_F003_EUROCHECK				"05"		/* Eurocheck									*/
#define P7_F003_TRAVELER_CHECK			"06"		/* Traveler.s check								*/
#define P7_F003_LETTER_CREDIT			"07"		/* Letter of credit								*/
#define P7_F003_FUNDS_WIRE				"08"		/* Funds wire									*/
#define P7_F003_CASHBACK				"09"		/* Cash back									*/
#define P7_F003_ACC_FUNDING				"10"		/* Account funding								*/
#define P7_F003_QUASICASH				"11"		/* Quasi cash									*/
#define P7_F003_CARD_CAPTURE			"14"		/* Card capture									*/
#define P7_F003_CASH_ADVANCE			"17"		/* Cash advance									*/
#define P7_F003_DOCUMENT_DEPOSIT		"18"		/* Document deposit								*/
#define P7_F003_FEE_COLLECTION			"19"		/* Fee collection  debit						*/
#define P7_F003_REFUND					"20"		/* Refund (purchase return)						*/
#define P7_F003_DEPOSIT					"21"		/* Deposit										*/
#define P7_F003_ADJ_CREDIT				"22"		/* Credit adjustment							*/
#define P7_F003_CHECK_DEPO_GUAR			"23"		/* Check deposit guarantee						*/
#define P7_F003_CHECK_DEPOSIT			"24"		/* Check deposit								*/
#define P7_F003_PAYMENT					"28"		/* Payment(credit)								*/
#define P7_F003_FUNDS_DISBURS			"29"		/* Funds disbursement							*/
#define P7_F003_AUTHENTICATION			"30"		/* Authentication request						*/
#define P7_F003_BAL_INQ					"31"		/* Balance inquiry								*/
/*#define P7_F003_ADDR_VERIF				"32"		/* Address verification							*/ /*PROD00069892: This code is used for Dcc inquiry*/
#define P7_F003_DCC_INQUIRY				"32"		/* Dcc Inquiry									*//*ZKO:16092018*/
#define P7_F003_CARD_RETURNED			"33" 		/* Card Return									*/
#define P7_F003_RIB						"34"		/* RIB Request									*/
#define P7_F003_ATM_STATEMENT			"36"		/* ATM Statement??								*/
#define P7_F003_STAT_REQ				"37"		/* Statement request							*/
#define P7_F003_SHORT_STAT_REQ			"38"		/* Short Statement request						*/
#define P7_F003_CHECK_BOOK_REQ			"39"		/* Check book request							*/
#define P7_F003_ACC_TRANSFER			"40"		/* Account transfer								*/
#define P7_F003_CR_PAY					"42"		/* Credit Payment								*/
#define P7_F003_BILL_PAYMENT			"50"		/* Bill payment									*/
#define P7_F003_ACC_VERIF				"58"		/* Account Verification							*/
#define P7_F003_CHARITY_DON				"80"		/* Charity donation								*/
#define P7_F003_AUTH_FEES				"88"		/* Auth fees									*/
#define P7_F003_FEES_INQ				"89"		/* Fees inquiry									*/
#define P7_F003_ECOM_CH_CERT_REQ		"90"		/* E-commerce cardholder certificate request	*/
#define P7_F003_PIN_CHANGE				"91"		/* PIN change request							*/
#define P7_F003_LOAN_REQ				"92"		/* Loan request									*/
#define P7_F003_LOAN_REFUND				"93"		/* Loan refund									*/
#define P7_F003_REDEMPTION				"94"		/* Redemption									*/
#define P7_F003_TOPUP					"95"		/* Load/Topup: Ref: Check POS server			*/
#define P7_F003_CONV_RATE_REQ			"96"		/* Conversion rates request						*/
#define P7_F003_TOKEN_REQ				"97"		/* Token request								*/
#define P7_F003_PIN_UNBLOCK				"98"		/* PIN Unblock/Card activation					*/
#define P7_F003_P1_LEN					2
#define P7_F003_P2_LEN					2
#define P7_F003_P3_LEN					2
#define P7_F003_LEN						6

#define P7_F003_P2_NOT_SPEC                            "00"            /* not specified      */
#define P7_F003_P2_SAVING_ACC                          "10"            /* Savings Account */
#define P7_F003_P2_CHECKING_ACC                        "20"            /* Checking Account     */
#define P7_F003_P2_CR_CRD_ACC                          "30"            /* credit card account    */
#define P7_F003_P2_UNIVERSAL_ACC                       "40"            /* Universal Account     */
#define P7_F003_P2_INVESTMENT_ACC                      "50"            /* Investment account    */

#define P7_F003_P3_DEFAULT                             "00"            /* Default               */



/* Function codes */

#define P7_F004_LEN						12
#define P7_F005_LEN						12
#define P7_F006_LEN						12
#define P7_F007_LEN						10
#define P7_F008_LEN						8	/* not in p7 spec, is used? */
#define P7_F009_LEN						8
#define P7_F010_LEN						8
#define P7_F011_LEN						6
#define P7_F012_LEN						12
#define P7_F013_LEN						4
#define P7_F014_LEN						4
#define P7_F015_LEN						6	/* 6 or 4?? */
#define P7_F016_LEN						4
#define P7_F017_LEN						4
#define P7_F018_LEN						4
#define P7_F019_LEN						3
#define P7_F020_LEN						3
#define P7_F021_LEN						3


/*Position 1: Card Data Entry Capability*/
#define P7_F022_P1_UNSPECIFIED			'0'			/*		Not specified								*/
#define P7_F022_P1_MANUAL				'1'			/*		Manual, no terminal							*/
#define P7_F022_P1_MAG_READ				'2'			/*		Magnetic stripe read						*/
#define P7_F022_P1_BAR_CODE				'3'			/*		Bar code									*/
#define P7_F022_P1_OCR					'4'			/*		OCR											*/
#define P7_F022_P1_ICC_MAG				'5'			/*		ICC and magnetic stripe						*/
#define P7_F022_P1_PAN_ENTRY			'6'			/*		PAN key entry								*/
#define P7_F022_P1_MAG_PAN_ENTRY		'7'			/*		PAN key entry & magnetic stripe				*/
#define P7_F022_P1_ICC_MAG_PAN_ENTRY	'8'			/*		PAN key entry, magnetic stripe, & EMV-compatible ICC reader		*/
#define P7_F022_P1_ICC					'9'			/*		EMV-compatible ICC reader					*/
#define P7_F022_P1_PAN_AUTO_ENTRY_CL_MAG		'A'	/*		PAN auto-entry via contactless magnetic stripe	*/
#define P7_F022_P1_PAN_AUTO_ENTRY_CL_ICC_MAG	'M'	/*		PAN auto-entry via contactless ICC and contactless magnetic stripe transactions	*/

/*	Position 2: Cardholder Authentication Capability		*/

#define P7_F022_P2_NO_AUTH		'0'					/*		No authentication							*/
#define P7_F022_P2_PIN			'1'					/*		PIN											*/
#define P7_F022_P2_SIG_VERIF	'2'					/*		Signature verification						*/
#define P7_F022_P2_OTHER		'9'					/*		Other										*/

/*	Position 3: Card Retention Capability					*/

#define P7_F022_P3_NO			'0'					/*		No											*/
#define P7_F022_P3_YES			'1'					/*		Yes											*/
#define P7_F022_P3_UNKNOWN		'9'					/*		Unknown										*/


/*	Position 4: Operating Environment						*/
#define P7_F022_P4_NO_TERM					'0'		/*		No terminal used							*/
#define P7_F022_P4_ONPREM_CA_ATT			'1'		/*		On premises of card acceptor, Attended		*/
#define P7_F022_P4_ONPREM_CA_UNATT			'2'		/*		On premises of card acceptor, Unattended	*/
#define P7_F022_P4_OFFPREM_CA_ATT			'3'		/*		Off premises of card acceptor, Attended		*/
#define P7_F022_P4_OFPREM_CA_UNATT			'4'		/*		Off premises of card acceptor, Unattended	*/
#define P7_F022_P4_ONPREM_CH_UNATT			'5'		/*		On premises of card owner, Unattended		*/
#define P7_F022_P4_UNKWNON					'9'		/*		Unknown										*/
#define P7_F022_P4_CAT_LVL_1_UNATT			'S'		/*		CAT level1, unattended						*/
#define P7_F022_P4_CAT_LVL_2_UNATT			'T'		/*		CAT level2, unattended						*/
#define P7_F022_P4_CAT_LVL_3_UNATT			'U'		/*		CAT level3, unattended						*/
#define P7_F022_P4_CAT_LVL_4_UNATT			'V'		/*		CAT level4, unattended						*/


/*		Position 5: Cardholder Presence Indicator	*/
#define P7_F022_P5_CH_PRES					'0'		/*		Cardholder present							*/
#define P7_F022_P5_CH_NPRES					'1'		/*		Cardholder not present						*/
#define P7_F022_P5_CH_NPRES_MO				'2'		/*		Cardholder not present, mail order			*/
#define P7_F022_P5_CH_NPRES_TO				'3'		/*		Cardholder not present, phone order			*/
#define P7_F022_P5_CH_NPRES_AUTHORIZED		'4'		/*		Cardholder not present, authorized			*/
#define P7_F022_P5_E_ORDER					'5'		/*		Electronic order							*/

/*		Position 6: Card Presence Indicator			*/
#define P7_F022_P6_CARD_NPRES				'0'		/*		Card not present							*/
#define P7_F022_P6_CARD_PRES				'1'		/*		Card present								*/

/*		Position 7: Card Data Input Mode			*/
#define P7_F022_P7_LEN					1
#define P7_F022_P7_UNSPECIFIED				'0'		/*		Unspecified										*/
#define P7_F022_P7_MANUAL					'1'		/*		Manual, no terminal								*/
#define P7_F022_P7_MAG						'2'		/*		Magnetic stripe read ( track2 dominant ).		*/
#define P7_F022_P7_BARCODE					'3'		/*		Barcode											*/
#define P7_F022_P7_OCR						'4'		/*		OCR												*/
#define P7_F022_P7_ICC						'5'		/*		ICC, CVV reliable								*/
#define P7_F022_P7_PAN_NETRY				'6'		/*		PAN key entry									*/
#define P7_F022_P7_TRACK1_READ				'7'		/*		Track I read									*/
#define P7_F022_P7_FALLBACK					'8'		/*		mode used as fallback to PAN auto entry via ICC when the IC could not be used	*/
#define P7_F022_P7_ECOM						'9'		/*		E-Commerce										*/
#define P7_F022_P7_CRED_ON_FILE				'F'		/*		Credentials on file								*/ 
#define P7_F022_P7_ICC_CVV_UNREL			'J'		/*		ICC, CVV unreliable								*/
#define P7_F022_P7_PAN_ENTRY_MOBILE_APP		'K'		/*		PAN auto entry via mobile application			*/
/*EBE161028: Labels redefined
#define P7_F022_P7_ECOM_NO_CH_CERT			'S'		*//*		E-commerce, SET without cardholder certificate	*/
/*#define P7_F022_P7_ECOM_CH_CERT				'T'		*//*		E-commerce, SET with cardholder certificate		*/

#define P7_F022_P7_ECOM_NON_AUTH_ATTEMPT	'S'		/*		E-commerce, Non-athenticated, attempt			*/
#define P7_F022_P7_ECOM_AUTHENTICATED		'T'		/*		E-commerce, Authenticated						*/
#define P7_F022_P7_ECOM_UNSECURE			'U'		/*		E-commerce, no security							*/
#define P7_F022_P7_ECOM_SEC_CHAN			'V'		/*		E-commerce, channel encryption					*/
#define P7_F022_P7_TRACK_UNALT				'W'		/*		Unaltered track provided						*/
#define P7_F022_P7_PAN_AUTO_ENTRY_CL_MAG	'A'		/*		PAN auto-entry via contactless magnetic stripe		*/
#define P7_F022_P7_PAN_AUTO_ENTRY_CL_ICC	'M'		/*		PAN auto-entry via contactless M/Chip				*/
#define P7_F022_P7_PAN_ENTRY_ECOM			'R'		/*		PAN entry via electronic commerce, including remote chip		*/
#define P7_F022_P7_PAN_AUTO_ENTRY_SERVER	'X'		/*		PAN auto entry via server (issuer, acquirer, or third party vendor system)	*/
#define P7_F022_P7_M_ECOMMERCE           	'K'		/*		PAN auto-entry via mobile application	*/ /* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 3.4.2*/


/*		Position 8: Cardholder Authentication Method		*/
#define P7_F022_P8_LEN					1
#define P7_F022_P8_NOT_AUTH					'0'		/*		Not authenticated								*/
#define P7_F022_P8_PIN						'1'		/*		PIN												*/
#define P7_F022_P8_ELEC_SIG_VERIF			'2'		/*		Electronic signature verification				*/
#define P7_F022_P8_BIOMETRICS				'3'		/*		Biometrics										*/
#define P7_F022_P8_BIOGRAPHY				'4'		/*		Biography										*/
#define P7_F022_P8_MAN_SIG_VERIF			'5'		/*		Manual signature verification					*/
#define P7_F022_P8_OTHER_MAN_VERIF			'6'		/*		Other manual verification (e.g. driver's license)	*/

/*		Position 9: Cardholder Authentication Entity		*/
#define P7_F022_P9_NOT_AUTH					'0'		/*		Not authenticated								*/
#define P7_F022_P9_ICC						'1'		/*		ICC												*/
#define P7_F022_P9_CA_CENTER_TERM			'2'		/*		Card acceptor center or terminal				*/
#define P7_F022_P9_ACQ_SCHEME				'3'		/*		Card acquiring center, or payment scheme		*/
#define P7_F022_P9_MERCHANT					'4'		/*		Merchant										*/
#define P7_F022_P9_OTHER					'5'		/*		Other											*/
#define P7_F022_P9_UNKOWN					'9'		/*		Unknown											*/

/*		Position 10: Rewrite Capability						*/
#define P7_F022_P10_UNSPECIFIED				'0'		/*		Unspecified										*/
#define P7_F022_P10_NONE					'1'		/*		None											*/
#define P7_F022_P10_TRACK3_REWRITE			'2'		/*		Track III rewrite capability					*/
#define P7_F022_P10_ICC_REWRITE				'3'		/*		ICC rewriting capability						*/

/*		Position 11: Terminal Output Capability				*/
#define P7_F022_P11_UNSPECIFIED				'0'		/*		Unspecified										*/
#define P7_F022_P11_NONE					'1'		/*		None											*/
#define P7_F022_P11_PRINTING_CAP			'2'		/*		Printing capability								*/
#define P7_F022_P11_DISPLAY_CAP				'3'		/*		Display capability								*/
#define P7_F022_P11_BOTH					'4'		/*		Both											*/

/*		Position 12: PIN Entry Capability					*/
#define P7_F022_P12_NOPIN_CAPTURE			'0'		/*		No PIN capture capability						*/
#define P7_F022_P12_UNKOWN					'1'		/*		Device PIN capture capability unknown			*/
#define P7_F022_P12_MPOS_SOFT_PIN           '3'     /*		mPOS Software-based PIN Entry Capability*//*ZKO:PROD00058820:ENH_OCT_2018_VISA_MCI*/
#define P7_F022_P12_4_DIG					'4'		/*		Four characters									*/
#define P7_F022_P12_5_DIG					'5'		/*		Five characters									*/
#define P7_F022_P12_6_DIG					'6'		/*		Six characters									*/
#define P7_F022_P12_7_DIG					'7'		/*		Seven characters								*/
#define P7_F022_P12_8_DIG					'8'		/*		Eight characters								*/
#define P7_F022_P12_9_DIG					'9'		/*		Nine characters									*/
#define P7_F022_P12_10_DIG					'A'		/*		Ten characters									*/
#define P7_F022_P12_11_DIG					'B'		/*		Eleven characters								*/
#define P7_F022_P12_12_DIG					'C'		/*		Twelve characters								*/
#define P7_F022_P12_PINPAD_DOWN				'S'		/*		PIN pad down									*/
#define P7_F022_P12_PIN_TERM_VERIF			'T'		/*		PIN verified by terminal device					*/



#define P7_F022_LEN						12








#define P7_F023_LEN						3

#define P7_F024_INIT_REQUEST			"100"			/* Initial request - Accurate			*/
#define P7_F024_PRE_AUTH				"101"			/* Pre authorization					*/
#define P7_F024_REPL_AUTH_AMNT_ACC 	    "102"			/* Replacement authorization - amount accurate*/ /* ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define P7_F024_RESUBMISSION_AMNT_ACC	"104"			/* Resubmission - amount accurate*/ /* ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define P7_F024_SUPP_AMNT				"106"			/* Supplementary amount - Accurate: Incremental					*/
#define P7_F024_SUPP_AMNT_ESTMD			"107"			/* Supplementary amount - Estimated: Pre-auth incremental		*/
#define P7_F024_BAL_REQUEST				"108"			/* Balance request						*/
#define P7_F024_LATE_OPERATION          "165"           /* late Operation - Op�ration tardive */ /*MTR Enh4.12 */
#define P7_F024_TER			            "170"		    /* Tokenization Eligibility Request (TER)	*/
#define P7_F024_TAR			            "171"		    /* Tokenization Authorization Request (TAR)	*/
#define P7_F024_ACN			            "172"		    /* Activation Code Notification (ACN)	    */
#define P7_F024_TCN			            "173"		    /* Tokenization Complete Notification (TCN)	*/
#define P7_F024_TVN			            "174"		    /* Tokenization Event Notification (TVN)	*/
#define P7_F024_RECURRING_MODIF         "178"		    /* Recurring Modification	*/  /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
#define P7_F024_RECURRING_CANCEL        "179"		    /* Recurring Cancellation	*/  /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
#define P7_F024_ACC_VERIF				"181"			/* Account verification					*/
#define P7_F024_ADDRESS_VERIF			"182"			/* Address verification					*/	/*AMER20180110 PROD00051816*/
#define P7_F024_DELAYED_CHARGES			"183"			/* Delayed charges*/ 						/* ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define P7_F024_NO_SHOW					"184"			/* No show*/ 								/* ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define P7_F024_DEFERRED_AUTH			"185"			/* Deferred Authorization				*/  /*IBO20190822 PROD00071113*/
#define P7_F024_FINAL_AMOUNT			"190"			/* Initial request - Final amount		*/

#define P7_F024_INI_REQUEST				"200"			/* Initial request/ Processed off line	*/
#define P7_F024_AUTH_PREV				"201"			/* Authorized previously				*/
#define P7_F024_AUTH_PREV_AMNT_DIFF		"202"			/* Authorized previously - Amount differs*/
#define P7_F024_AUTH_FITST_PRE			"205"			/* 1st presentement*/
#define P7_F024_AUTH_SECOND_PRE			"206"			/* 2er presentement*/

#define P7_F024_TOPUP_FROM_CH_ACC		"221"			/* Topup from cardholder account: Ref: Check EMV_OfflineLimitTopUpProcessing	*/
#define P7_F024_BAL_INQUIRY				"281"			/* Balance Inquiry						*/

#define P7_F024_ADD_RECORD				"301"			/* Add record							*/
#define P7_F024_CHG_RECORD				"302"			/* Change record (partial update)		*/
#define P7_F024_DEL_RECORD				"303"			/* Delete record						*/
#define P7_F024_FULL_CHG_RECORD			"304"			/* Change record (full update)			*/
#define P7_F024_INQ_RECORD				"305"			/* Inquiry								*/

#define P7_F024_FULL_REVERSAL			"400"			/* Full reversal						*/
#define P7_F024_PARTIAL_REVERSAL		"402"			/* Partial reversal						*/


#define P7_F024_STOP_INTERFACE			"600"			/* Stop interface					*/
#define P7_F024_PARAM_REFRESH			"601"			/* Parameters refresh				*/
#define P7_F024_PING_RESOURCE			"602"			/* Ping (resource status)			*/
#define P7_F024_SIGNON_BIN				"603"			/* Sign-on (BIN)					*/
#define P7_F024_SIGNOFF_BIN				"604"			/* Sign-off (BIN)					*/
#define P7_F024_GRP_SIGNON				"605"			/* Group sign-on					*/
#define P7_F024_GRP_SIGNOFF				"606"			/* Group sign-off					*/
#define P7_F024_START_SAF				"607"			/* Start SAF						*/
#define P7_F024_END_SAF					"608"			/* End SAF							*/
#define P7_F024_ECHO_TEST				"609"			/* Echo test						*/
#define P7_F024_LINE_DOWN_IND			"610"			/* Line Down Indicator				*/
#define P7_F024_LINE_UP_IND				"611"			/* Line Up Indicator				*/
#define P7_F024_START_INTERFACE			"612"			/* Start interface					*/
#define	P7_F024_KEY_REQUEST				"615"			/* Key Request						*/
#define P7_F024_STATISTICS				"620"			/* Statistics						*/
#define	P7_F024_MAC_KEY_REQUEST			"651"			/* MAC Key Request						*/



#define P7_F024_SIGNON					"801"			/* Sign-on (BIN)						*/
#define P7_F024_SIGNOFF					"802"			/* Sign-off (BIN)						*/
#define P7_F024_ECHO					"803"			/* Echo test							*/
#define P7_F024_KEY_CHG					"811"			/* Key change							*/
#define P7_F024_KEY_VERIFY				"812"			/* Key verification						*/
#define P7_F024_KEY_REPEAT				"813"			/* Key repeat							*/
#define P7_F024_RECON_REQ				"821"			/* Reconciliation request				*/
#define P7_F024_INT_RECON_REQ			"822"			/* Intermediary Reconciliation request	*/
#define P7_F024_INIT_REQ				"880"			/* Initialization request				*/
#define P7_F024_MAC_KEY_CHG				"899"			/* MAC Key Change						*/

#define P7_F024_N_STOP_INTERFACE		600			/* Stop interface					*/
#define P7_F024_N_PARAM_REFRESH			601			/* Parameters refresh				*/
#define P7_F024_N_PING_RESOURCE			602			/* Ping (resource status)			*/
#define P7_F024_N_SIGNON_BIN			603			/* Sign-on (BIN)					*/
#define P7_F024_N_SIGNOFF_BIN			604			/* Sign-off (BIN)					*/
#define P7_F024_N_GRP_SIGNON			605			/* Group sign-on					*/
#define P7_F024_N_GRP_SIGNOFF			606			/* Group sign-off					*/
#define P7_F024_N_START_SAF				607			/* Start SAF						*/
#define P7_F024_N_END_SAF				608			/* End SAF							*/
#define P7_F024_N_ECHO_TEST				609			/* Echo test						*/
#define P7_F024_N_LINE_DOWN_IND			610			/* Line Down Indicator				*/
#define P7_F024_N_LINE_UP_IND			611			/* Line Up Indicator				*/
#define P7_F024_N_START_INTERFACE		612			/* Start interface					*/
#define P7_F024_N_KEY_REQUEST           615         /* Key Request                      */
#define P7_F024_N_STATISTICS			620			/* Statistics						*/
#define P7_F024_KEY_EXCHANGE			650			/*AMEX GCA*/
#define  P7_F024_MAC_KEY_EXCHANGE       651 /*MAC Key Exchange*//*ZKO29102018*/
#define P7_F024_N_ACTION				700			/* Action							*/
#define p7_F024_N_DISABLE_MODE_PCI		801			/* DESABLE MODE PCI					*/

#define P7_F024_LEN						3

#define P7_F025_ISS_TIMEOUT					"1002"		/* Decline, Issuer Timeout								*/
#define P7_F025_ISS_NOT_AVAIL				"1004"		/* Decline, Issuer Not Available						*/
#define P7_F025_CIF_DEC						"1005"		/* Decline, Card found in SAF							*/
#define P7_F025_TO_STANDIN					"1010"		/* Standin on behalf of issuer because of timeout		*/
#define P7_F025_MF_STANDIN					"1011"		/* Standin on behalf of issuer because of malfunction	*/
#define P7_F025_NA_STANDIN					"1012"		/* Standin on behalf of issuer because of issuer not available */
#define P7_F025_STANDIN_PAR					"1013"		/* Standin on behalf of issuer because issuer parameters */
#define P7_F025_CIS_STANDIN					"1014"		/* Standin on behalf of issuer because Card in SAF */
#define P7_F025_PIN_TRSLT_ERR				"1225"		/* Decline, Pin translation error						*/
#define P7_F025_AGGREGATED_TRX				"1679"		/* Provision pour montants agreges						*/

#define P7_F025_CRD_CAP_TO_CND				"4019"		/* Card retained / Cardholder time out / Cash not dispensed */
#define P7_F025_REV_NOT_SPECIFIED           "4001"  	/*Not specified*/
#define P7_F025_REV_MALFUNCTION				"4002"  	/*Malfunction*/
#define P7_F025_RESP_RECV_LATE				"4006"
#define P7_F025_UNABLE_TO_DELIVER_MSG		"4013"      /* Cannot deliver message to point of sale */
#define P7_F025_LEN						4

#define P7_F026_LEN						4
#define P7_F027_LEN						1
#define P7_F028_LEN						6
#define P7_F029_LEN						3				/* not in p7 spec, is used? */
#define P7_F030_LEN						36				/* not in p7 spec, is used? */
#define P7_F031_MAX_LEN					16				/* used for ATM pin change */
#define P7_F032_MAX_LEN					11
#define P7_F033_MAX_LEN					11
#define P7_F034_MAX_LEN					28
#define P7_F035_MAX_LEN					37
#define P7_F036_MAX_LEN					104
#define P7_F037_LEN						12
#define P7_F038_LEN						6


#define P7_F039_APPROVAL				"000"		/* Approval									*/
#define P7_F039_APPROVAL_W_ID			"001"		/* Approuved with ID						*/
#define P7_F039_PARTIAL_APPROVAL		"002"		/* Partial Approval							*/
#define P7_F039_APPROVAL_VIP			"003"		/* Approuved VIP							*/
#define P7_F039_APPROVAL_T3_UPD			"004"		/* Approuved, Track III updated				*/
#define P7_F039_APPROVAL_CHIP_UPD		"007"		/* Approuved update chip					*/
#define P7_F039_NO_CB					"010"		/* Purchase Amount Only, No Cash Back Allowed*/   /*NAB24032020*/
#define P7_F039_NO_RSN_TO_DEC			"085"		/* No reason to declined					*/
#define P7_F039_DECLINE					"100"		/* Rejected									*/
#define P7_F039_EXPIRED_CARD			"101"		/* Expired card								*/
#define P7_F039_SUSPECTED_FRAUD			"102"		/* Suspected fraud							*/
#define P7_F039_PRIVATE_CARD			"104" 		/* Private card								*/
#define P7_F039_PIN_TRIES_EXC			"106"		/* PIN entry tries exceeded					*/
#define P7_F039_REF_TO_ISS				"107"		/* Refer to card issuer						*/
#define P7_F039_REF_TO_ISS_SC			"108"		/* Refer to issuer; special condit			*/
#define P7_F039_INV_MER					"109"		/* Invalid Merchant							*/
#define P7_F039_INV_AMNT				"110"		/* Invalid amount							*/
#define P7_F039_INV_CARD_NBR			"111"		/* Invalid card number						*/
#define P7_F039_PIN_REQUIRED			"112"		/* PIN element required						*/
#define P7_F039_UNKOWN_CARD				"114"		/* Unknown card								*/
#define P7_F039_FCT_NA					"115"		/* Function not available					*/
#define P7_F039_UNSUF_FUNDS				"116"		/* No sufficient funds						*/
#define P7_F039_PIN_ERROR				"117"		/* Incorrect PIN							*/
#define P7_F039_NO_SUCH_CARD			"118"		/* No such card								*/
#define P7_F039_TRN_NOPERM_CH			"119"		/* Cardholder trx not permitted				*/
#define P7_F039_TNR_NOPERM_TERM			"120"		/* Terminal trx not permitted				*/
#define P7_F039_WTDWL_LMT_EXC			"121"		/* Exceeds withdrawal limit					*/
#define P7_F039_SEC_VIOLATION			"122"		/* Security violation						*/
#define P7_F039_WTDWL_FREQ_EXC			"123"		/* Withdrawal frequency exceeded			*/
#define P7_F039_CARD_NOT_IN_SRV			"125"		/* Card not in service						*/
#define P7_F039_PIN_FMT_ERR				"126"		/* Wrong PIN format							*/
#define P7_F039_PIN_LEN_ERR				"127"		/* Error PIN length							*/
#define P7_F039_CRYPTO_ERR				"128"		/* Cryptographic error						*/
#define P7_F039_FRAUD_SUSPECTED			"129"		/* Suspected fraud							*/
#define P7_F039_NO_CR_ACC				"180"		/* No credit account						*/
#define P7_F039_NO_CHEQ_ACC				"181"		/* No cheque account						*/
#define P7_F039_NO_SAVING_ACC			"182"		/* No saving Account						*/
#define P7_F039_CVV_ERROR				"183"		/* Bad CVV									*/
#define P7_F039_INV_DATE				"184"		/* Invalid date								*/
#define P7_F039_NOSUP_SERV_CODE			"185"		/* Service code not supported				*/
#define P7_F039_DECLINE_T3_UPD			"199"		/* Refused with Track III updating			*/
#define P7_F039_DECLINE_PICKUP			"200"		/* Rejected, Pick up card					*/
#define P7_F039_EXPIRED_PICKUP			"201"		/* Expired card, Pick up					*/
#define P7_F039_FRAUDULENT_PICKUP		"202"		/* Fraudulent, Pick up						*/
#define P7_F039_PRIV_CARD_PICKUP		"204"		/* Private card, Pick up					*/
#define P7_F039_CALL_ACQ_PICKUP			"205"		/* Call security acquirer, pick up			*/
#define P7_F039_PIN_TRIES_EXC_PICKUP	"206"		/* PIN entry tries exceeded					*/
#define P7_F039_SPEC_COND_PICKUP		"207"		/* Special conditions, Pick up				*/
#define P7_F039_LOST_CARD_PICKUP		"208"		/* Lost card, Pick up						*/
#define P7_F039_STOLEN_CARD_PICKUP		"209"		/* Stolen card, Pick up						*/
#define P7_F039_SUSP_FRD_PICKUP			"210"		/* Suspected fraud, Pick up					*/
#define P7_F039_ALTERN_AMNT_CANCELED	"280"		/* Alternate Amount cancelled				*/
#define P7_F039_PICKUP					"299"		/* Pick up									*/
#define P7_F039_PROC_SUCCESS			"300"		/* Successfully processed					*/
#define P7_F039_NOT_SUPP_ADDR			"301"		/* Not supported by the addressee			*/
#define P7_F039_REC_NOTFOUND			"302"		/* Record not found  in the file			*/
#define P7_F039_DUP_RECORD				"303"		/* Duplicate record.						*/
#define P7_F039_FLD_CTRL_ERR			"304"		/* Fields control error						*/
#define P7_F039_LOCK_FILE				"305"		/* Locked table								*/
#define P7_F039_UNSUCCESS				"306"		/* Unsuccessful								*/
#define P7_F039_FMT_ERROR				"307"		/* Format error								*/
#define P7_F039_DUP_REC					"308"		/* Duplicate record.						*/
#define P7_F039_UNKNOWN_FILE			"309"		/* Unknown file								*/
/*#define P7_F039_REC_NOTFOUND			"381"	*/	/* Record not found							*/
#define P7_F039_REV_ACCEPTED			"480"		/* Reversal accepted						*/
#define P7_F039_INV_REV					"481"		/* Invalid reversal							*/
#define P7_F039_ORIG_TRN_REV			"482"		/* Original Transaction Already Rev			*/
#define P7_F039_RECON_REFUSED			"501"		/* Reconciliation Not Accepted				*/
#define P7_F039_CNT_NA					"503"		/* Counters not available					*/
#define P7_F039_RECON_DONE				"581"		/* Reconciliation done						*/
#define P7_F039_CUTOFF_INPROG			"800"		/* Cut-off in progress						*/
#define P7_F039_CONN_REFUSED			"880"		/* Connection not accepted					*/
#define P7_F039_DL_HOT_LIST				"881"		/* Download Hot list						*/
#define P7_F039_INV_TRN					"902"		/* Invalid transaction						*/
#define P7_F039_NOSUCH_ISS				"908"		/* Not such issuer							*/
#define P7_F039_SYS_FAULT				"909"		/* System fault								*/
#define P7_F039_TIMEOUT					"911"		/* Time out									*/
#define P7_F039_ISS_UNV					"912"		/* Issuer not available						*/
#define P7_F039_NO_CONN					"992"		/* No Connection							*/
#define P7_F039_PIN_VERIF_FAULT			"993"		/* Pin verification fault					*/
#define P7_F039_TRN_PROC_FAULT			"994"		/* Transaction processing fault				*/
#define P7_F039_SERVER_PROC_FAULT		"995"		/* Server processing fault					*/

#define P7_F039_LEN						3
#define P7_F040_LEN						3
#define P7_F041_LEN						8
#define P7_F042_LEN						15
#define P7_F043_MAX_LEN					99
#define P7_F044_MAX_LEN					999		/* 99 to 999 */
#define P7_F045_MAX_LEN					77		/* 76 to 77 */
#define P7_F046_MAX_LEN					255				/* not in p7 spec, is used? */
#define P7_F047_MAX_LEN					255				/* not in p7 spec, is used? */
#define P7_F048_MAX_LEN					999

#define   P7_F048_VCR_USAGE    				'9'  /*ICH17012018 To indicate dispute Usage*/

#define P7_POS_CONDCD_NORMAL			"00"		/* normal  presentment	*/
#define P7_POS_CONDCD_CH_NOT_PRESENT	"01"		/* customer  not  present */
#define P7_POS_CONDCD_MER_SUSP			"03"		/* merchant  suspicious */



/*AMER20180322 PROD00054205 Art 1.1: The values definition below are changed*/
#define         P7_F048_P54_CAVV_INVALID					    '0'  /*CAVV invalid or F022.P7 is ?S? or ?T? and CAVV is not present*/
/*#define         P7_F048_P54_CAVV_NOT_PRESENT                 	'R'*/  /*Not verified*/
#define         P7_F048_P54_CAVV_NOT_YET_VERIFIED               'R'  /*CAVV not yet verified (internal initial value)*/
/*#define         P7_F048_P54_CAVV_AUTHENTICATION_INV          	'P'*/  /*CAVV Not verified. Non-authenticated security submissions.*/
#define         P7_F048_P54_CAVV_NOT_VAL_ISS_NOT_PART           'P'  /*CAVV not verified, issuer not participating in CAVV verification*/
#define         P7_F048_P54_CAVV_AUTHEN_NOK    		     		'1'  /*CAVV failed validation-authentication*/
#define         P7_F048_P54_CAVV_AUTHEN_OK     		     		'2'  /*CAVV passed validation-authentication*/
#define         P7_F048_P54_CAVV_ATTEMP_OK     		     		'3'  /*CAVV passed validation-attempt*/
#define         P7_F048_P54_CAVV_ATTEMP_NOK    		     		'4'  /*CAVV failed validation-attempt*/	
/*AMER20170919 Start PROD00046952*/
#define 		P7_F048_P54_CAVV_NOK_ATT_ISS_NOT_PART			'7' /*CAVV Failed - Attempt, Issuer not participating, Network Key*/
#define 		P7_F048_P54_CAVV_OK_ATT_ISS_NOT_PART			'8' /*CAVV Passed - Attempt, Issuer not participating, Network Key*/
#define 		P7_F048_P54_CAVV_NOK_ATT_PART_ACS_NOT_AVAILABLE	'9' /*CAVV Failed - Attempt, Participating, Access Control Server (ACS) not available, Network Key*/
#define 		P7_F048_P54_CAVV_OK_ATT_PART_ACS_NOT_AVAILABLE	'A' /*CAVV Passed - Attempt, Participating, Access Control Server (ACS) not available, Network Key*/
/*AMER20170919 End PROD00046952*/ 
#define         P7_F048_P54_CAVV_VAL_NOT_PERF        	     	'V' /*CAVV Not verified. Non-secure submissions*/
#define         P7_F048_P54_CAVV_VALID_OK_VISA       	     	'B' /*CAVV passed validation?information only, no liability shift (Only from VISA network)*/
/*#define         P7_F048_P54_CAVV_NOT_VALIDATED_ATTE_VISA     	'O'  / *CAVV Not verified. attempt.* /
#define         P7_F048_P54_CAVV_NOT_VALIDATED_AUTH_VISA     	'N'  / *Not verified. authenticated.*/
#define         P7_F048_P54_CAVV_NOT_VALIDATED_ATTE   			'O' /*CAVV not verified-attempt. Unable to verify or issuer did not provide the CAVV result. F022.P7 is ?S? and AAV is present*/
#define         P7_F048_P54_CAVV_NOT_VALIDATED_AUTH   			'N' /*CAVV not verified-authenticated. Unable to verify or issuer did not provide the CAVV result. F022.P7 is ?T? and AAV is present*/
#define 		P7_F048_P54_CAVV_NOT_VERF_OTHER_SEC_METHD		'S' /*CAVV not verified. Other security method*/


/* Start DCC ICH See PROD00039494*/
#define P7_F048_P56_DCC_NO_PERFORMED	'0'			/* 	 No DCC performed*/
#define P7_F048_P56_DCC_APPROVED		'1'			/* 	 DCC request approved by cardholder*/
#define P7_F048_P56_DCC_DECLINED		'2'			/* 	 DCC request declined by cardholder*/
/*End  DCC ICH See PROD00039494*/



/*Start AMER20190219 PROD00065991: Enh19.Q2 Art2.1*/
/*Purchase of cryptocurrency indicator*/
#define P7_F048_P64_P3_UNSPECIFIED			'0'			/* 	 Unspecified*/
#define P7_F048_P64_P3_CRPT_CURR_PUR		'1'			/* 	 Purchase of cryptocurrency*/
/*End AMER20190219 PROD00065991: Enh19.Q2 Art2.1*/
#define P7_F048_P64_P3_HIGH_RISK_SEC		'2'	  /*IBO20190805 PROD00070403 Enh19.Q4 AN 2628*/

/*Start IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
#define P7_F048_P64_P4_UNSPECIFIED			'0'
#define P7_F048_P64_P4_MER_INIT			    '1'
#define P7_F048_P64_P4_CH_INIT				'2'
/*End IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/

/*Start ZKO 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define P7_F048_P64_P7_UNSPECIFIED              'N'     /* Unspecified/Credentials not stored */
#define P7_F048_P64_P7_UNSHEDULED               'C'     /* Unscheduled Credential on File */
#define P7_F048_P64_P7_STANDING_ORDER           'S'     /* Standing Order */
#define P7_F048_P64_P7_RECURRING                'R'     /* Recurring*/
#define P7_F048_P64_P7_INSTALLMENT              'I'     /* Installment */
#define P7_F048_P64_P7_CH_CRDTL_ONFILE          'F'     /* Cardholder Credential-on-file */
/*End ZKO 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/


/*Start AEA20210712 Enh21.Q4 AN4972*/
#define P7_F048_P64_P10_UNSPECIFIED			'0'	/*Unspecified*/
#define P7_F048_P64_P10_ORIGINAL_CARD		'1'	/*Fast refund to the original card*/
#define P7_F048_P64_P10_NEW_CARD			'2'	/*Fast refund to new card*/
/*End AEA20210712 Enh21.Q4 AN4972*/

/*Start IBO20190809 PROD00070934 Enh19.Q4 AN 2051*/
#define P7_F048_P61_P20_SINGLE_PIN_TAP_NOT_SUPP				'0'			/* 	 Single-tap pin not supported*/
#define P7_F048_P61_P20_SINGLE_PIN_TAP_SUPP					'1'			/* 	 Single-tap pin supported*/
#define P7_F048_P64_P5_NOT_REQUESTED						'0'			/* 	 Pin not requested in a single-tap mode */
#define P7_F048_P64_P5_REQUESTED							'1'			/* 	 Pin requested in a single-tap mode*/
#define P7_F048_P64_P6_UNSPECIFIED						    '0'			/* 	 Unspecified */
#define P7_F048_P64_P6_RESPONSE_TO_PIN_REQ					'1'			/* 	 Response to pin request*/
/*Start PLUTONL-4280 :Enh5.03 :1348*/
#define P7_F048_P64_P11_UNSPECIFIED				         	'0'         /*  Unspecified*/
#define P7_F048_P64_P11_INFO_NEXT_TRX					    '1'         /*  Obtain new information before the next transaction*/
#define P7_F048_P64_P11_TRY_AGAIN_LATER				        '2'         /*  Try again later*/
#define P7_F048_P64_P11_NEVER_TRY_AGIN					    '3'         /*  Never try again*/
/*End PLUTONL-4280 :Enh5.03 :1348*/

/*End IBO20190809 PROD00070934 Enh19.Q4 AN 2051*/

#define P7_F048_P64_P8_TOKEN_BASED_TRANS					'1'		     /*Token based transaction*/
#define P7_F048_P64_P9_REF_ORIGINAL_AUTH				    '1'		     /*Refer to original authorization*/

/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/
#define P7_F048_P64_P12_NO_LINK					'0'		     /*No link*/
#define P7_F048_P64_P12_LINKED				    '1'		     /*Linked*/
/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/		

/*Start: ZKO: PROD00058820*/
#define P7_F048_P61_POS21_MPOS_TERM             '1' /*Dedicated mPOS Terminal with PCI compliant dongle*/
#define P7_F048_P61_POS21_OFF_MDEVICE           '2'  /*Off the Shelf Mobile Device*/

/*Start AMER20190314 PROD00066569: CBAE Enh5.00*/
/*1075*/
#define P7_F048_P66_3DS_AUTH_TYPE_CH			'1'	/*Challenge*/ 
#define P7_F048_P66_3DS_AUTH_TYPE_FR			'2'	/*Frictionless*/

/*766*/
#define P7_F048_P66_CAVV_ALG_HMAC			'0'	/*HMAC (SET  TransStain)*/
#define P7_F048_P66_CAVV_ALG_CVV			'1'	/*HMAC (SET  TransStain)*/
#define P7_F048_P66_CAVV_ALG_CVV_WITH_ATN	'2'	/*HMAC (SET  TransStain)*/
#define P7_F048_P66_CAVV_ALG_AAV_MC			'3'	/*HMAC (SET  TransStain)*/
#define P7_F048_P66_CAVV_ALG_CRPT_WALLET	'W'	/*HMAC (SET  TransStain)*/

/*766*/
/*End AMER20190314 PROD00066569: CBAE Enh5.00*/

/*Start IBO20221102 PLUTONL-5272*/
#define P7_F048_P66_POS1_ECOM						'9'		/*		E-Commerce										*/
#define P7_F048_P66_POS1_ECOM_NON_AUTH_ATTEMPT		'S'		/*		E-commerce, Non-athenticated, attempt			*/
#define P7_F048_P66_POS1_ECOM_AUTHENTICATED			'T'		/*		E-commerce, Authenticated						*/
#define P7_F048_P66_POS1_ECOM_UNSECURE				'U'		/*		E-commerce, no security							*/
#define P7_F048_P66_POS1_ECOM_SEC_CHAN				'V'		/*		E-commerce, channel encryption					*/
/*End IBO20221102 PLUTONL-5272*/

/*Program Protocol*/
#define P7_F048_P66_POS2_3DSEC_1_0			'1'
#define P7_F048_P66_POS2_3DSEC_2_0			'2'
/*END:ZKO:PROD00058820*/
/*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/
#define   P7_F048_P66_AF_UNSPECIFED          '0'        /* Authentication framework */
#define   P7_F048_P66_VISA_DAF               '1'        /* VISA Digital authentication framework */
/*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/

/* START PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/
#define	 P7_F048P21_MCI_MSG_FORMAT_VERSION_CODE_Y		                "Y"
#define	 P7_F048P21_MCI_MSG_FORMAT_VERSION_CODE_N		                "N"
/* END PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/
/*HAL PROD00073823 20202801 Enh Q2.2020 Art 2941*/ 
#define P7_F048_P38_MCI_CHK_ESTIMATED_AMNT        16
#define P7_F048_P38_MCI_CHK_REMOTE_COMMERCE_ID    17
/*HAL PROD00073823 20202801 Enh Q2.2020 Art 2941*/ 

/*Start HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 4221*/
#define P7_F048_P38_MCI_CHK_MER_POS_DATA          19 
/*End HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 4221*/

/*Start AMER20171123: PROD00049633*/
#define P7_F048_P71_RECURRING			'R'			/* Recurring transaction */
#define P7_F048_P71_INSTALLMENT			'I'			/* Installment payment */
#define P7_F048_P71_CRED_ON_FILE		'C'			/* Credentials on file */
/*End AMER20171123: PROD00049633*/
#define P7_F048_P71_STANDING            'S'         /*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/

#define P7_F048_P82_ADR_VERIF_REQ		'R'			/* Address verification requested */	/*AMER20180109 PROD00050359*/

#define P7_F048_S21_FORMAT_LEN				1
#define P7_F048_S21_ZIP_CODE_LEN			10
#define P7_F048_S21_ADR_LEN			        40

#define P7_F048S23_004_GIFT_CARD			"01000"	 

#define P7_F048_K02_TRX_AMNT_TYPE			'T'		/* Transaction amount type */
#define P7_F048_K02_BILL_AMNT_TYPE			'B'		/* Billing amount type */

#define P7_F049_LEN						3
#define P7_F050_LEN						3
#define P7_F051_LEN						3
#define P7_F052_LEN						16	/* 8 bin */
#define P7_F053_MAX_LEN					99

/*Positions 1-2: Security format (Pin Block encryption method)*/
#define P7_F053_P1_NO_ENC					"00"
#define P7_F053_P1_TPK						"01"
#define P7_F053_P1_ZPK						"02"
#define P7_F053_P1_LEN                                         2
/*Positions 3-4: Pin block format*/
#define P7_F053_P2_FORMAT01					"01"
#define P7_F053_P2_NO_PIN                                        "99"
#define P7_F053_P2_LEN						2
/* Positions 5-7: Pin encryption key index */
#define P7_F053_P3_RESERVED					"000"
#define P7_F053_P3_LEN						3
/* Positions 8-10: Mac key index */
#define P7_F053_P4_RESERVED					"000"
#define P7_F053_P4_LEN						3
/* Positions 11-16: Source PID */
#define P7_F053_P5_LEN						8
/* Positions 17-22: Source resource */
#define P7_F053_P6_LEN						6
/* Positions 23-28: Destination PID */
#define P7_F053_P7_LEN						8
/* Positions 29-34: Destination resource */
#define P7_F053_P8_LEN						6
/* Positions 35-49: User */
#define P7_F053_P9_LEN						15
/* Positions 50-58: PurgeTime */
#define P7_F053_P10_LEN						9
/* Positions 59-62: Source node ID */
#define P7_F053_P11_LEN						4
/* Positions 63-66: Destination node ID */
#define P7_F053_P12_LEN						4

#define P7_F054_ACCT_TYPE_LEN			2
#define P7_F054_AMNT_TYPE_LEN			2
#define P7_F054_CCY_LEN					3
#define P7_F054_SIGN_LEN				1
#define P7_F054_AMNT_LEN				12

#define P7_F054_MAX_LEN					120

#define		P7_F054_ACCT_TYPE_SA			"90"	/* Shadow account						*/	/*AMER20171116 PROD00049633*/

#define		P7_F054_AT_LG_BAL				"01"	/* Ledger Balance						*/
#define		P7_F054_AT_AVAILABLE			"02"	/*  Available Balance					*/
#define		P7_F054_AT_AMNT_OWING			"03"	/*  Amount Owing						*/
#define		P7_F054_AT_AMOUNT_DUE			"04"	/*  Amount Due							*/
#define		P7_F054_AT_CB_AMNT				"40"	/*  Amount Cash Back					*/
#define		P7_F054_AT_AMNT_GRATUITY		"44"	/*  Amount, Gratuity					*/
#define		P7_F054_AT_ORIG_AMNT			"57"	/*  Original Amount						*/
#define		P7_F054_AT_DCC_ORIG_AMNT		"58"	/*  DCC Original Amount					*/	/*AMER20170928 PROD00047410 Enh17.Q4*/
#define		P7_F054_AT_UNSPECIFIED			"99"	/*  Unspecified							*/
#define		P7_F054_AT_LEN		2	

#define P7_F055_MAX_LEN					299		/* 255 to 299 */
#define P7_F056_MAX_LEN					60
#define P7_F057_LEN						3				/* not in p7 spec, is used? */
#define P7_F058_MAX_LEN					255				/* not in p7 spec, is used? */
#define P7_F059_MAX_LEN					14				/* not in p7 spec, is used? */

#define ERT_PROXIMITY_PAYMENT			"10" /*  Paiement de proximite */
#define ERT_CNP_PAYMENT 				"20" /* paiement a distance non specifie */
#define ERT_PHONE_ORDER					"21" /* telephone */
#define ERT_MAIL_ORDER					"22" /* correspondance */
#define ERT_ECOM 						"24" /* reseaux ouverts */
#define ERT_ECOM_RECURRING_PAYMENT 		"27" /* Paiement recurrent issu de commande sur reseaux ouverts	** */
#define ERT_RECURRING_PAYMENT 			"28" /* Paiement recurrent issu d'autre moyen de commande		** */

#define ERT_ATM 						"70" /* retrait sur automate(pas cb2a) */
#define ERT_CASH_ADVANCE 				"75" /* retrait guichet */
#define ERT_QUASI_CASH 					"60" /* quasi-cash cas standard */

#define ERT_TERMINAL_PAYMENT			"40" /* Paiement sur automate */
#define ERT_TERM_LVL1_CLASS1 			"41" /* Paiement par automate de classe 1  niveau 1 : ADM */
#define ERT_TERM_LVL1_CLASS21 			"42" /* Paiement par automate de classe 2.1  niveau 1 : ADM */
#define ERT_TERM_LVL1 					"43" /* Paiement sur automate  authentification porteur obligatoire */
#define ERT_TERM_LVL2_CLASS1 			"45" /* Paiement par automate de classe 1  niveau 2 : SST */
#define ERT_TERM_LVL2_CLASS21 			"46" /* Paiement par automate de classe 2.1  niveau 2 : SST */
#define ERT_TERM_LVL2_CLASS22 			"47" /* Paiement par automate de classe 2.2  niveau 2 : SST */
#define ERT_TERM_LVL3_CLASS1 			"49" /* Paiement par automate de classe 1  niveau 3 : LAT */


 
#define ERT_TERM_LVL3_CLASS21 			"50" /* Paiement par automate de classe 2.1  niveau 3 : LAT */
#define ERT_TERM_LVL3_CLASS22 			"51" /* Paiement par automate de classe 2.2  niveau 3 : LAT */
#define ERT_PRE_AUTH 					"80" /* Pre-autorisation	*/

/*Start AMER20190517 : Cbae 5.00 PROD00067907 1050*/
#define APPLE_PAY_SOL 					"000" 
#define SAMSUNG_PAY_SOL 				"001" 
#define GOOGLE_PAY_SOL 					"002" 								 						   
/*End AMER20190517 : Cbae 5.00 PROD00067907 1050*/

#define P7_F060_MAX_LEN					999
#define P7_F060_MAX_LEN					999
#define P7_F061_MAX_LEN					999
#define P7_F062_MAX_LEN					999
#define P7_F063_MAX_LEN					999
#define P7_F064_LEN						16		/* 8 bin */
#define P7_F065_LEN						0				/* not in p7 spec, is used? */
#define P7_F066_LEN						1				/* not in p7 spec, is used? */
#define P7_F067_LEN						2				/* not in p7 spec, is used? */
#define P7_F068_LEN						3				/* not in p7 spec, is used? */
#define P7_F069_LEN						3				/* not in p7 spec, is used? */
#define P7_F070_LEN						3
#define P7_F071_LEN						8
#define P7_F072_MAX_LEN					999
#define P7_F073_LEN						6
#define P7_F074_LEN						10
#define P7_F075_LEN						10
#define P7_F076_LEN						10
#define P7_F077_LEN						10
#define P7_F078_LEN						10				/* not in p7 spec, is used? */
#define P7_F079_LEN						10				/* not in p7 spec, is used? */
#define P7_F080_LEN						10				/* not in p7 spec, is used? */
#define P7_F081_LEN						10				/* not in p7 spec, is used? */
#define P7_F082_LEN						10				/* not in p7 spec, is used? */
#define P7_F083_LEN						10
#define P7_F084_LEN						10
#define P7_F085_LEN						10				/* not in p7 spec, is used? */
#define P7_F086_LEN						16
#define P7_F087_LEN						16
#define P7_F088_LEN						16
#define P7_F089_LEN						16
#define P7_F090_LEN						35				/* not in p7 spec, is used? */

#define P7_F090_P1_LEN         4
#define P7_F090_P2_LEN         6
#define P7_F090_P3_LEN         12
#define P7_F090_P4_LEN         11


#define P7_F091_LEN						3				/* not in p7 spec, is used? */
#define P7_F092_LEN						3				/* not in p7 spec, is used? */
#define P7_F093_MAX_LEN					11
#define P7_F094_MAX_LEN					11
#define P7_F095_MAX_LEN					42				/* not in p7 spec, is used? */
#define P7_F096_MAX_LEN					999
#define P7_F097_LEN						17
#define P7_F098_LEN						25				/* not in p7 spec, is used? */
#define P7_F099_MAX_LEN					11
#define P7_F100_MAX_LEN					11				/* not in p7 spec, is used? */
#define P7_F101_MAX_LEN					17
#define P7_F102_MAX_LEN					28
#define P7_F103_MAX_LEN					28
#define P7_F104_MAX_LEN					16				/* not in p7 spec, is used? */
#define P7_F105_LEN						16
#define P7_F106_LEN						16
#define P7_F107_LEN						16
#define P7_F108_LEN						16
#define P7_F109_MAX_LEN					0				/* not in p7 spec, is used? */
#define P7_F110_MAX_LEN					100				/* used for ATM response Label */
#define P7_F111_MAX_LEN					0				/* not in p7 spec, is used? */
#define P7_F112_MAX_LEN					256				/* not in p7 spec, is used? */
#define P7_F113_MAX_LEN					100				/* used in ATM              */
#define P7_F114_MAX_LEN					100				/* used in ATM              */
#define P7_F115_MAX_LEN					24				/* not in p7 spec, is used? */
#define P7_F116_MAX_LEN					999
#define P7_F117_MAX_LEN					0				/* not in p7 spec, is used? */
#define P7_F118_MAX_LEN					0				/* not in p7 spec, is used? */
#define P7_F119_MAX_LEN					16				/* not in p7 spec, is used? */
#define P7_F120_MAX_LEN					0				/* not in p7 spec, is used? */
#define P7_F121_MAX_LEN					255				/* not in p7 spec, is used? */
#define P7_F122_MAX_LEN					255				/* not in p7 spec, is used? */
#define P7_F123_MAX_LEN					29				/* not in p7 spec, is used? */
#define P7_F124_MAX_LEN					255				/* not in p7 spec, is used? */
#define P7_F125_MAX_LEN					255				/* not in p7 spec, is used? */
#define P7_F126_MAX_LEN					309				/*SKARROUMI PLUTONL-4833*/
#define P7_F127_MAX_LEN					999
#define P7_F128_LEN						8		/* 8 bin */

#endif

