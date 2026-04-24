#ifndef JCB_DEFINE_H
#define JCB_DEFINE_H

/* =============================================== *
 * 		  GENERAL VARIABLES		   *		
 * =============================================== */

#define JCB_MSG_TYPE_1100		       1100
#define JCB_MSG_TYPE_1101		       1101			
#define JCB_MSG_TYPE_1110                      1110
#define JCB_MSG_TYPE_1210                      1210
#define JCB_MSG_TYPE_1700		       1700	
#define	JCB_MSG_TYPE_100			100
#define JCB_MSG_TYPE_101			101 
#define JCB_MSG_TYPE_110			110
#define JCB_MSG_TYPE_130			130
#define JCB_MSG_TYPE_302			302
#define JCB_MSG_TYPE_630			630
#define JCB_MSG_TYPE_800                        800
#define JCB_MSG_TYPE_810			810

#define JCB_F038_LEN				  6
#define JCB_F039_LEN				  2

#define JCB_F039_SYSTEM_MALFUNX			"96"	
#define JCB_F039_DIRECTION_PR			"PR"
#define	JCB_F039_DIRECTION_RP			"RP"

#define JCB_UNKNOWN_COUNTRY		       "900"

/* -------------- DEFAULT VALUES --------------- */
#define JCB_F026_VDEFAULT                       "04"
#define JCB_F038_VDEFAULT                   "000000"
#define JCB_F063_VDEFAULT	   	  "SCPPPSEX"

/* ---------------   LENGTHS   ----------------- */
#define JCB_F061_LEN				  6
#define JCB_F003_LEN				  6
#define JCB_F003_P1_LEN				  2
#define JCB_F003_P2_LEN                           2
#define JCB_F003_P3_LEN                           2
#define JCB_F004_LEN                             12
#define JCB_BANK_CODE_LEN			  6
#define JCB_CURRENCY_CODE_LEN			  3
#define JCB_COUNTRY_CODE_LEN			  3
#define JCB_F007_LEN				 10
#define	JCB_F011_LEN				  6
#define JCB_F012_LEN                              6
#define JCB_F013_LEN                              4
#define JCB_F014_LEN				  4
#define JCB_F018_LEN				  4
#define JCB_F020_LEN				  3
#define JCB_F022_LEN				  4
#define JCB_F022_P1_LEN				  2
#define JCB_F022_P2_LEN				  1
#define JCB_F022_P3_LEN				  1
#define JCB_F023_LEN				  4
#define JCB_F025_LEN                              2
#define JCB_F026_LEN				  2
#define JCB_F037_LEN				 12
#define JCB_F041_LEN				  8
#define JCB_F042_LEN				 15
#define JCB_F048_TAG02_LEN			 47
#define JCB_F048_TAG02_XID_LEN			 20
#define JCB_F048_TAG02_CAVV_LEN			 20
#define JCB_F048_TAG02_ECI_LEN			  1
#define JCB_F049_LEN				  3
#define JCB_F052_LEN				 16
#define JCB_F053_LEN				 16
#define JCB_F063_LEN				  8
#define JCB_F070_LEN				  3
#define JCB_F101_LEN				  4


/* =============================================== *
 *                JCB DATA ELEMENTS                *
 * =============================================== */

/* -------- FIELD 003 : Processing code -------- */
#define JCB_F003_P1_PURCHASE			"00"	/* Purchase */
#define JCB_F003_P1_WITHDRAWAL			"01"	/* Withdrawal / Cash Advance including Manual Cash Advance */
#define JCB_F003_P1_BALANCE_INQUERY		"30"	/* Balance Inquiry */

#define JCB_F003_P2_NO_ACCOUNT			"00"	/* No account specified */
#define JCB_F003_P2_SAVING_ACC			"10"	/* Saving Account */
#define JCB_F003_P2_CHECKING_ACC		"20"	/* Checking Account */
#define JCB_F003_P2_CREDIT_ACC			"30"	/* Credit Account */

#define JCB_F003_P3_NO_ACCOUNT			"00"	/* No account specified/ Not applicable */


/* -------- FIELD 022 : POS entry mode   -------  */
#define JCB_F022_P1_UNKNOWN			"00"	/* Unknown */
#define JCB_F022_P1_MANUAL_ENTRY		"01"	/* Manual entry (typing) */
#define JBC_F022_P1_MAGNETIC_STRIPE		"02"	/* Magnetic stripe reading */
#define JCB_F022_P1_CHIP_READING		"05"	/* Chip reading */
#define JCB_F022_P1_CONTACTLESS_EMV		"07"	/* Contactless EMV mode */
#define JCB_F022_P1_ECOMMERCE	        	"81"	/* Electronic Commerce */
#define JCB_F022_P1_CONTACTLESS_STRIPE    	"91"	/* Contactless magnetic stripe mode */
#define JCB_F022_P1_MAG_STRIPE_READING      	"97"	/* Magnetic stripe reading */

#define JCB_F022_P2_UNKNOWN			 "0"	/* Unknown */
#define JBC_F022_P2_TER_ACCEPT_PIN_OK		 "1"	/* Terminal can accept PINs */
#define JCB_F022_P2_TER_ACCEPT_PIN_NOK	 	 "2"	/* Terminal cannot accept PINs */

#define JCB_F022_P3_RESERVED			 "0"	/* Reserved  */

/* ------ FIELD 025 : POS condition code  ------  */
#define JCB_F025_NORMAL_PRESENTMENT	 	"00"	/* Normal presentment */
#define JCB_F025_CARDHLDR_NOT_PRESENT       	"01"	/* Cardholder not present */
#define JCB_F025_UNATTENDED_TERMINAL		"02"	/* Unattended terminal able to retain card */
#define JCB_F025_CARDHLDR_OK_CARD_NOK		"05"	/* Cardholder present, card not present */
#define JCB_F025_PREAUTHO_REQ			"06"	/* Pre-authorized request */
#define JCB_F025_TEL_DEVICE_REQ			"07"	/* Telephone device request */
#define JCB_F025_MAIL_TEL_ORDER			"08"	/* Mail and/or telephone order */
#define JCB_F025_PRESENTATION_ITEM		"13"	/* Representation of item */
#define JCB_F025_CARDHLD_TERMINAL		"15"	/* Cardholder (home) terminal */
#define JCB_F025_UNATTENDED_TERMINAL_CARD	"27"	/* Unattended terminal unable to retain card */


/* ------ FIELD 039 : Response code       ------  */
#define JCB_F039_APPROVED			"00"	/* Approved or completed successfully */
#define JCB_F039_REFER_TO_ISSUER		"01"	/* Refer to Issuer */
#define JCB_F039_INVALID_MERCHANT		"03"	/* Invalid merchant */
#define JCB_F039_PICKUP                       	"04"	/* Pick-up */
#define JCB_F039_DO_NOT_HONOR			"05"	/* Do not honor */
#define JCB_F039_ERROR				"06"	/* Error */
#define JCB_F039_SPECIAL_PICKUP			"07"	/* Pick-up card, special condition, counterfeit */
#define JCB_F039_REQ_IN_PROGRESS		"09"	/* Request in progress */
#define JCB_F039_PARTIAL_APPROVAL		"10"	/* Partial Approval */
#define JCB_F039_INVALID_TRX			"12"	/* Invalid transaction (Duplicate transaction) */
#define JCB_F039_INVALID_AMOUNT			"13"	/* Invalid amount */
#define JCB_F039_INVALID_CARD_NUM		"14"	/* Invalid card number (no such number) */
#define JCB_F039_NO_ISSUER			"15"	/* No such Issuer */
#define JCB_F039_INVALID_RESP			"20"	/* Invalid response */
#define JCB_F039_FORMAT_ERROR			"30"	/* Format error */
#define JCB_F039_BANK_NOT_SUPP			"31"	/* Bank not supported by switch */
#define JCB_F039_EXPIRED_CARD			"33"	/* Expired card */
#define JCB_F039_SUSP_FRAUD			"34"	/* Suspended fraud */
#define JCB_F039_RESTRICTED_CARD		"36"	/* Restricted card */
#define JCB_F039_REQ_FX_NOT_SUPP		"40"	/* Requested function not supported */
#define JCB_F039_LOST_CARD			"41"	/* Lost card */
#define JCB_F039_NO_UNIV_ACCOUNT		"42"	/* No universal account */
#define JCB_F039_STOLEN_CARD			"43"	/* Stolen card, pick-up */
#define JCB_F039_NOT_SUFF_FUNDS			"51"	/* Not sufficient funds */
#define JCB_F039_EXPRD_CRD_OR_DATE		"54"	/* Expired card, or expiry date error */
#define JCB_F039_INCORRECT_PIN			"55"	/* Incorrect personal identification number */
#define JCB_F039_NO_CARD_RECORD			"56"	/* No card record */
#define JCB_F039_TRX_NOT_PERM_CARDHLD		"57"	/* Transaction not permitted to cardholder */
#define JCB_F039_TRX_NOT_PERM_CRD		"58"	/* Transaction not permitted to terminal */	
#define JCB_F039_SUSPECTED_FRAUD		"59"	/* Suspected fraud */
#define JCB_F039_EXD_WITH_AMNT_LIMIT		"61"	/* Exceeds withdrawal amount limit */
#define JCB_F039_RESTRICTED_CARD_2		"62"	/* Restricted card */
#define JCB_F039_SECURITY_VIOLATION		"63"	/* Security violation */
#define JCB_F039_EXD_WITH_FREQ_LIMIT		"65"	/* Exceeds withdrawal frequency limit */
#define JCB_F039_PIN_TIRES_EXCDED		"75"	/* Allowable number of PIN tries exceeded */
#define JCB_F039_INCORRECT_REVERSAL		"76"	/* Incorrect reversal (Auth-ID, STAN, AMNT)*/
#define JCB_F039_LOST_STOLEN_PICKUP		"77"	/* Lost, stolen, pick-up */
#define JCB_F039_SHOP_BLACKLISTED		"78"	/* Shop in black list */
#define JCB_F039_ACC_FLAG_FALSE			"79"	/* Account status flag false */
#define JCB_F039_PIN_CRYPTO_ERROR		"81"	/* PIN cryptographic error found */
#define JCB_F039_NOT_DECLINED			"85"	/* Not Declined */
#define JCB_F039_INCORRECT_PASSPORT		"87"	/* Incorrect passport number */
#define JCB_F039_INCORRECT_DATE_BIRTH		"88"	/* Incorrect date of birth */
#define JCB_F039_NOT_APPROVED			"89"	/* Not approved: free message */
#define JCB_F039_CUTOFF_IN_PROGRESS		"90"	/* Cutoff is in progress */
#define JCB_F039_ISSUER_SWITCH_KO		"91"	/* Issuer or switch is inoperative */
#define JCB_F039_FINANCIAL_INSTU_404		"92"	/* Financial institution cannot be found for routing */
#define JCB_F039_DUPL_TRASMISSION		"94"	/* Duplicate transmission */
#define JCB_F039_SYSTEM_MALFX			"96"	/* System malfunction */


/* - FIELD 53 : Security related control info. -- */
#define JCB_F053_P1_KEY_TYPE			"99"	/* Acquirer and Issuer working key */
#define JCB_F053_P2_PIN_ENCRY_ALGO		"01"	/* DES or Triple DES (Double length key) */
#define JCB_F053_P3_ISO9564			"01"	/* ANSI Standard (ISO 9564-1 Format 0) */
#define JCB_F053_P4_KEY_INDEX 			"00"	/* Not specified */
#define JCB_F053_P5_MAC_INDEX                   "00"    /* Not specified */
#define JCB_F053_P7_RESERVED                    "00"    /* Reserverd */


/* - FIELD 54 : Security related control info. -- */
#define JCB_F054_P2_LEDGER_BAL			"01"	/* Ledger Balance (Only used for Balance Inquiry */
#define JCB_F054_P2_AVAILABLE_BAL		 "2"	/* Available Balance (Only used for Balance Inquiry) */
#define JCB_F054_P2_ORGINAL_AMOUNT		"57"	/* Original Amount (Only used for Partial Approval) */

#define JCB_F054_P4_CREDIT_AMOUNT		 'C'	/* Credit amount */
#define JCB_F054_P4_DEBIT_AMOUNT	 	 'D'	/* Debit amount */

/*START HAL 20210503 FOR DBS DBSSG-8*/
#define JCB_F55_APP_CRYPTOGRAM    "9F26"
#define JCB_F55_CRYPTO_INFO_DATA  "9F27"
#define JCB_F55_ISS_APP_DATA      "9F10"
#define JCB_F55_UNPRED_NUMBER     "9F37"
#define JCB_F55_ATC               "9F36"
#define JCB_F55_TVR 			  "95"
#define JCB_F55_TRANS_DATE        "9A"
#define JCB_F55_TRANS_TYPE        "9C"
#define JCB_F55_TRANS_AMOUNT      "9F02"
#define JCB_F55_TRANS_CUR_CODE    "5F2A"
#define JCB_F55_APP_INTER_PROFILE "82"
#define JCB_F55_TERM_COUNTRY_CODE "9F1A"
#define JCB_F55_OTHER_AMOUNT      "9F03"
#define JCB_F55_TERM_CAP          "9F33"
#define JCB_F55_CVM              "9F34"
#define JCB_F55_TERM_TYPE        "9F35"
#define JCB_F55_IFD_SERIAL_NUM   "9F1E"
#define JCB_F55_DED_FILE_NAME 	 "84"
#define JCB_F55_TERM_APP_VER_NUM "9F09"
#define JCB_F55_TRANS_SEQ_NUM    "9F41"
#define JCB_F55_ISS_AUTHEN_DATA   "91"
#define JCB_F55_ISS_SCRIPT1 	 "71"
#define JCB_F55_ISS_SCRIPT2      "72"
#define JCB_F55_TRANS_CATEG_CODE "9F53"
#define JCB_F55_ICC_APPLICATION_ID "4F"



#define JCB_F55_NB_FIELDS				  24
/*END HAL 20210503 FOR DBS DBSSG-8*/
/* ------ FIELD 61 : POS Information	   ------ */
#define JCB_F061_P1_UNKNOWN			 "0"	/* Unknown */
#define JCB_F061_P1_MAIL_TEL_ORDER		 "1"	/* Mail/Telephone Order Transaction */
#define JCB_F061_P1_NONMAIL_TEL_ORDER		 "2"	/* Non-Mail/Telephone Order Transaction */

#define JCB_F061_P2_RECURRING_TRX		 "1"	/* Recurring Transaction */

#define JCB_F061_P3_PRE_AUTHO			 "1"	/* Nonrecurring Transaction */


/* -- FIELD 70 : Network management info code  -- */
#define JCB_F070_SIGN_ON	     	       "001"	/* Sign-on */
#define JCB_F070_SIGN_OFF		       "002"	/* Sign-off */
#define JCB_F070_DYNAMIC_KEY_EXCH	       "101"	/* Dynamic Key Exchange */
#define JCB_F070_ECHO_TEST		       "301"	/* Echo test */


/* ------  FIELD 101 : File Name           ------ */
#define JCB_F101_FILE_NAME                    "6332"    /* File name */

#endif /* JCB_DEFINE_H */
 