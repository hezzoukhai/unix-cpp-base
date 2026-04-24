#ifndef PWC_HSID_DEFINE__H
#define PWC_HSID_DEFINE__H


#define		HSID_LENGTH_LEN					4
#define		HSID_HDR_LEN					11
#define		HSID_MSG_TYPE_LEN				4




#define HSID_F003_P1_LEN					2
#define HSID_F003_P2_LEN					2
#define HSID_F003_P3_LEN					2
#define HSID_F003_LEN 						6


#define HSID_F004_LEN 		12
#define HSID_F005_LEN 		12
#define HSID_F006_LEN 		12
#define HSID_F007_LEN 		10
#define HSID_F008_LEN 		8
#define HSID_F009_LEN 		8
#define HSID_F010_LEN		8
#define HSID_F011_LEN 		6
#define HSID_F012_LEN 		12
#define HSID_F013_LEN 		4
#define HSID_F014_LEN 		4
#define HSID_F015_LEN 		6
#define HSID_F016_LEN 		4
#define HSID_F017_LEN 		4
#define HSID_F018_LEN 		4
#define HSID_F019_LEN 		3
#define HSID_F020_LEN 		3
#define HSID_F021_LEN 		3
#define HSID_F022_LEN 		12
#define HSID_F023_LEN 		3
#define HSID_F026_LEN 		4
#define HSID_F027_LEN 		1
#define HSID_F028_LEN 		6
#define HSID_F029_LEN 		3
#define HSID_F030_LEN 		24
#define HSID_F037_LEN 		12
#define HSID_F038_LEN 		6
#define HSID_F040_LEN 		3
#define HSID_F041_LEN 		8
#define HSID_F042_LEN 		15
#define HSID_F049_LEN 		3
#define HSID_F050_LEN 		3
#define HSID_F051_LEN 		3
#define HSID_F052_LEN 		16
#define HSID_F057_LEN 		3
#define HSID_F064_LEN 		8
#define HSID_F065_LEN 		0
#define HSID_F067_LEN 		2
#define HSID_F068_LEN 		3
#define HSID_F069_LEN 		3
#define HSID_F070_LEN 		3
#define HSID_F071_LEN 		8
#define HSID_F073_LEN 		6
#define HSID_F074_LEN 		10
#define HSID_F075_LEN 		10
#define HSID_F076_LEN 		10
#define HSID_F077_LEN 		10
#define HSID_F078_LEN 		10
#define HSID_F079_LEN 		10
#define HSID_F080_LEN 		10
#define HSID_F081_LEN 		10
#define HSID_F082_LEN 		10
#define HSID_F083_LEN 		10
#define HSID_F084_LEN 		10
#define HSID_F085_LEN 		10
#define HSID_F086_LEN 		16
#define HSID_F087_LEN 		16
#define HSID_F088_LEN 		16
#define HSID_F089_LEN 		16
#define HSID_F090_LEN 		35
#define HSID_F091_LEN 		3
#define HSID_F092_LEN 		3
#define HSID_F097_LEN 		17
#define HSID_F098_LEN 		25
#define HSID_F105_LEN 		16
#define HSID_F106_LEN 		16
#define HSID_F107_LEN 		10
#define HSID_F108_LEN 		10
#define HSID_F128_LEN 		8




#define HSID_F002_MAX_LEN 	19
#define HSID_F031_MAX_LEN 	9
#define HSID_F032_MAX_LEN 	11
#define HSID_F033_MAX_LEN 	11
#define HSID_F034_MAX_LEN 	28
#define HSID_F035_MAX_LEN 	37
#define HSID_F036_MAX_LEN 	4
#define HSID_F043_MAX_LEN 	99
#define HSID_F044_MAX_LEN 	999
#define HSID_F045_MAX_LEN 	77
#define HSID_F046_MAX_LEN 	255
#define HSID_F047_MAX_LEN 	255
#define HSID_F048_MAX_LEN 	999
#define HSID_F053_MAX_LEN 	16
#define HSID_F054_MAX_LEN 	120
#define HSID_F055_MAX_LEN 	255
#define HSID_F056_MAX_LEN 	255
#define HSID_F058_MAX_LEN 	255
#define HSID_F059_MAX_LEN 	14
#define HSID_F060_MAX_LEN 	6
#define HSID_F061_MAX_LEN 	999
#define HSID_F062_MAX_LEN 	999
#define HSID_F063_MAX_LEN 	999
#define HSID_F066_MAX_LEN 	1
#define HSID_F072_MAX_LEN 	999
#define HSID_F093_MAX_LEN 	11
#define HSID_F094_MAX_LEN 	11
#define HSID_F095_MAX_LEN 	42
#define HSID_F096_MAX_LEN 	17
#define HSID_F099_MAX_LEN 	11
#define HSID_F100_MAX_LEN 	11
#define HSID_F101_MAX_LEN 	17
#define HSID_F102_MAX_LEN 	28
#define HSID_F103_MAX_LEN 	28
#define HSID_F104_MAX_LEN 	16
#define HSID_F115_MAX_LEN 	24
#define HSID_F123_MAX_LEN 	29
#define HSID_F125_MAX_LEN 	255
#define HSID_F126_MAX_LEN 	255


/*  HEADER */
#define HSID_H_PROTOCOL_ID				"ISO"	/* Identification du protocole							*/
#define HSID_H_PWC_TYPE_ACQ_ISS			"7"		/* Header PowerCARD		Type							*/
#define HSID_H_PWC_PROROCOL_VERSION		"0100"	/* Version du protocole									*/

/* Processing codes */
#define HSID_F003_PURCHASE				"00"		/* Purchases & Services							*/
#define HSID_F003_WITHDRAWAL			"01"		/* Withdrawal									*/
#define HSID_F003_ADJ_DEBIT				"02"		/* Debit adjustment								*/
#define HSID_F003_CASHBACK				"09"		/* Cash back									*/
#define HSID_F003_ACC_FUNDING			"10"		/* Account funding								*/
#define HSID_F003_QUASICASH				"11"		/* Quasi cash									*/
#define HSID_F003_CASH_ADVANCE			"17"		/* Cash advance									*/
#define HSID_F003_FEE_COLLECTION		"19"		/* Fee collection  debit						*/
#define HSID_F003_REFUND				"20"		/* Refund (purchase return)						*/
#define HSID_F003_DEPOSIT				"21"		/* Deposit										*/
#define HSID_F003_ADJ_CREDIT			"22"		/* Credit adjustment							*/
#define HSID_F003_CHECK_DEPOSIT			"24"		/* Check deposit								*/
#define HSID_F003_PAYMENT				"28"		/* Payment(credit)								*/
#define HSID_F003_AUTHENTICATION		"30"		/* Authentication request						*/
#define HSID_F003_BAL_INQ				"31"		/* Balance inquiry								*/
#define HSID_F003_ADDR_VERIF			"32"		/* Address verification							*/
#define HSID_F003_RIB					"34"		/* RIB Request									*/
#define HSID_F003_STAT_REQ				"37"		/* Statement request							*/
#define HSID_F003_SHORT_STAT_REQ		"38"		/* Short Statement request						*/
#define HSID_F003_CHECK_BOOK_REQ		"39"		/* Check book request							*/
#define HSID_F003_ACC_TRANSFER			"40"		/* Account transfer								*/
#define HSID_F003_CR_PAY				"42"		/* Credit Payment								*/
#define HSID_F003_BILL_PAYMENT			"50"		/* Bill payment									*/
#define HSID_F003_ACC_VERIF				"58"		/* Account Verification							*/
#define HSID_F003_CHARITY_DON			"80"		/* Charity donation								*/
#define HSID_F003_TXT_MSG				"83"		/* Message Text									*/
#define HSID_F003_ECOM_CH_CERT_REQ		"90"		/* E-commerce cardholder certificate request	*/
#define HSID_F003_PIN_CHANGE			"91"		/* PIN change request							*/
#define HSID_F003_LOAN_REQ				"92"		/* Loan request									*/
#define HSID_F003_LOAN_REFUND			"93"		/* Loan refund									*/
#define HSID_F003_REDEMPTION			"94"		/* Redemption									*/
#define HSID_F003_TOPUP					"95"		/* Load/Topup: Ref: Check POS server			*/
#define HSID_F003_ATM_PURCHASE			"96"		/* ATM Purchase									*/
#define HSID_F003_TOKEN_REQ				"97"		/* Token request								*/
#define HSID_F003_PIN_UNBLOCK			"98"		/* PIN Unblock/Card activation					*/
#define HSID_F003_P1_LEN					2
#define HSID_F003_P2_LEN					2
#define HSID_F003_P3_LEN					2
#define HSID_F003_LEN						6
/* Function codes */


#define HSID_F024_INIT_REQUEST			"100"			/* Initial request						*/
#define HSID_F024_PRE_AUTH				"101"			/* Pre authorization					*/
#define HSID_F024_SUPP_AMNT				"106"			/* Supplementary amount - Accurate		*/
#define HSID_F024_SUPP_AMNT_ESTMD		"107"			/* Supplementary amount - Estimated		*/
#define HSID_F024_BAL_REQUEST			"108"			/* Balance request						*/
#define HSID_F024_ACC_VERIF				"181"			/* Account verification					*/

#define HSID_F024_AUTH_PREV				"201"			/* Authorized previously				*/
#define HSID_F024_AUTH_PREV_AMNT_DIFF	"202"			/* Authorized previously - Amount differs*/
#define HSID_F024_AUTH_FITST_PRE		"205"			/* 1st presentement*/
#define HSID_F024_AUTH_SECOND_PRE		"206"			/* 2er presentement*/
#define HSID_F024_TOPUP_FROM_CH_ACC		"221"			/* Topup from cardholder account: Ref: Check EMV_OfflineLimitTopUpProcessing	*/
#define HSID_F024_BAL_INQUIRY			"281"			/* Balance Inquiry						*/

#define HSID_F024_ADD_RECORD			"301"			/* Add record							*/
#define HSID_F024_CHG_RECORD			"302"			/* Change record (partial update)		*/
#define HSID_F024_DEL_RECORD			"303"			/* Delete record						*/
#define HSID_F024_FULL_CHG_RECORD		"304"			/* Change record (full update)			*/
#define HSID_F024_INQ_RECORD			"305"			/* Inquiry								*/

#define HSID_F024_FULL_REVERSAL			"400"			/* Full reversal						*/
#define HSID_F024_PARTIAL_REVERSAL		"402"			/* Partial reversal						*/


#define HSID_F024_STOP_INTERFACE		"600"			/* Stop interface					*/
#define HSID_F024_PARAM_REFRESH			"601"			/* Parameters refresh				*/
#define HSID_F024_PING_RESOURCE			"602"			/* Ping (resource status)			*/
#define HSID_F024_SIGNON_BIN			"603"			/* Sign-on (BIN)					*/
#define HSID_F024_SIGNOFF_BIN			"604"			/* Sign-off (BIN)					*/
#define HSID_F024_GRP_SIGNON			"605"			/* Group sign-on					*/
#define HSID_F024_GRP_SIGNOFF			"606"			/* Group sign-off					*/
#define HSID_F024_START_SAF				"607"			/* Start SAF						*/
#define HSID_F024_END_SAF				"608"			/* End SAF							*/
#define HSID_F024_ECHO_TEST				"609"			/* Echo test						*/
#define HSID_F024_LINE_DOWN_IND			"610"			/* Line Down Indicator				*/
#define HSID_F024_LINE_UP_IND			"611"			/* Line Up Indicator				*/
#define HSID_F024_START_INTERFACE		"612"			/* Start interface					*/
#define HSID_F024_STATISTICS			"620"			/* Statistics						*/


#define HSID_F024_SIGNON				"801"			/* Sign-on (BIN)						*/
#define HSID_F024_SIGNOFF				"802"			/* Sign-off (BIN)						*/
#define HSID_F024_ECHO					"803"			/* Echo test							*/
#define HSID_F024_SIGNON_SAF			"804"			/* Sign-on (SAF)						*/
#define HSID_F024_SIGNOFF_SAF			"805"			/* Sign-off (SAF)						*/
#define HSID_F024_KEY_CHG				"811"			/* Key change							*/
#define HSID_F024_KEY_VERIFY			"812"			/* Key verification						*/
#define HSID_F024_KEY_REPEAT			"813"			/* Key repeat							*/
#define HSID_F024_RECON_REQ				"821"			/* Reconciliation request				*/
#define HSID_F024_INT_RECON_REQ			"822"			/* Intermediary Reconciliation request	*/
#define HSID_F024_INIT_REQ				"880"			/* Initialization request				*/

#define HSID_F024_N_SIGNON_BIN			603
#define HSID_F024_N_SIGNOFF_BIN			604
#define HSID_F024_N_ECHO_TEST			609
#define HSID_F_N_DISABLE_MODE_PCI		801			/* DESABLE MODE PCI					*/


#define HSID_F024_N_SIGNON				801
#define HSID_F024_N_SIGNOFF				802
#define HSID_F024_N_ECHO				803
#define HSID_F024_N_SIGNON_SAF			804
#define HSID_F024_N_SIGNOFF_SAF			805
#define HSID_F024_N_ZPK_KEY_CHG			811
#define HSID_F024_N_KEY_VERIFY			812
#define HSID_F024_N_KEY_REPEAT			813
#define HSID_F024_N_RECON_REQ			821
#define HSID_F024_N_INT_RECON_REQ		822
#define HSID_F024_N_INIT_REQ			880
#define HSID_F024_N_TAK_KEY_CHG			899


#define HSID_F024_LEN					3
#define HSID_F025_LEN					4

#define HSID_F038_DEFAULT_ID			"099999"		/* Default ID							*/
#define HSID_F038_AUTH_ID_NULL			"000000"		/* Default ID							*/


#define HSID_F039_APPROVAL				"000"		/* Approval									*/
#define HSID_F039_APPROVAL_W_ID			"001"		/* Approuved with ID						*/
#define HSID_F039_APPROVAL_VIP			"003"		/* Approuved VIP							*/
#define HSID_F039_APPROVAL_T3_UPD		"004"		/* Approuved, Track III updated				*/
#define HSID_F039_APPROVAL_CHIP_UPD		"007"		/* Approuved update chip					*/
#define HSID_F039_NO_RSN_TO_DEC			"085"		/* No reason to declined					*/
#define HSID_F039_DECLINE				"100"		/* Rejected									*/
#define HSID_F039_EXPIRED_CARD			"101"		/* Expired card								*/
#define HSID_F039_SUSPECTED_FRAUD		"102"		/* Suspected fraud							*/
#define HSID_F039_PRIVATE_CARD			"104" 		/* Private card								*/
#define HSID_F039_PIN_TRIES_EXC			"106"		/* PIN entry tries exceeded					*/
#define HSID_F039_REF_TO_ISS			"107"		/* Refer to card issuer						*/
#define HSID_F039_REF_TO_ISS_SC			"108"		/* Refer to issuer; special condit			*/
#define HSID_F039_INV_MER				"109"		/* Invalid Merchant							*/
#define HSID_F039_INV_AMNT				"110"		/* Invalid amount							*/
#define HSID_F039_INV_CARD_NBR			"111"		/* Invalid card number						*/
#define HSID_F039_PIN_REQUIRED			"112"		/* PIN element required						*/
#define HSID_F039_UNKOWN_CARD			"114"		/* Unknown card								*/
#define HSID_F039_FCT_NA				"115"		/* Function not available					*/
#define HSID_F039_UNSUF_FUNDS			"116"		/* No sufficient funds						*/
#define HSID_F039_PIN_ERROR				"117"		/* Incorrect PIN							*/
#define HSID_F039_NO_SUCH_CARD			"118"		/* No such card								*/
#define HSID_F039_TRN_NOPERM_CH			"119"		/* Cardholder trx not permitted				*/
#define HSID_F039_TNR_NOPERM_TERM		"120"		/* Terminal trx not permitted				*/
#define HSID_F039_WTDWL_LMT_EXC			"121"		/* Exceeds withdrawal limit					*/
#define HSID_F039_SEC_VIOLATION			"122"		/* Security violation						*/
#define HSID_F039_WTDWL_FREQ_EXC		"123"		/* Withdrawal frequency exceeded			*/
#define HSID_F039_CARD_NOT_IN_SRV		"125"		/* Card not in service						*/
#define HSID_F039_PIN_FMT_ERR			"126"		/* Wrong PIN format							*/
#define HSID_F039_PIN_LEN_ERR			"127"		/* Error PIN length							*/
#define HSID_F039_CRYPTO_ERR			"128"		/* Cryptographic error						*/
#define HSID_F039_FRAUD_SUSPECTED		"129"		/* Suspected fraud							*/
#define HSID_F039_NO_CR_ACC				"180"		/* No credit account						*/
#define HSID_F039_NO_CHEQ_ACC			"181"		/* No cheque account						*/
#define HSID_F039_NO_SAVING_ACC			"182"		/* No saving Account						*/
#define HSID_F039_CVV_ERROR				"183"		/* Bad CVV									*/
#define HSID_F039_INV_DATE				"184"		/* Invalid date								*/
#define HSID_F039_NOSUP_SERV_CODE		"185"		/* Service code not supported				*/
#define HSID_F039_DECLINE_T3_UPD		"199"		/* Refused with Track III updating			*/
#define HSID_F039_DECLINE_PICKUP		"200"		/* Rejected, Pick up card					*/
#define HSID_F039_EXPIRED_PICKUP		"201"		/* Expired card, Pick up					*/
#define HSID_F039_FRAUDULENT_PICKUP		"202"		/* Fraudulent, Pick up						*/
#define HSID_F039_PRIV_CARD_PICKUP		"204"		/* Private card, Pick up					*/
#define HSID_F039_CALL_ACQ_PICKUP		"205"		/* Call security acquirer, pick up			*/
#define HSID_F039_PIN_TRIES_EXC_PICKUP	"206"		/* PIN entry tries exceeded					*/
#define HSID_F039_SPEC_COND_PICKUP		"207"		/* Special conditions, Pick up				*/
#define HSID_F039_LOST_CARD_PICKUP		"208"		/* Lost card, Pick up						*/
#define HSID_F039_STOLEN_CARD_PICKUP	"209"		/* Stolen card, Pick up						*/
#define HSID_F039_SUSP_FRD_PICKUP		"210"		/* Suspected fraud, Pick up					*/
#define HSID_F039_ALTERN_AMNT_CANCELED	"280"		/* Alternate Amount cancelled				*/
#define HSID_F039_PICKUP				"299"		/* Pick up									*/
#define HSID_F039_PROC_SUCCESS			"300"		/* Successfully processed					*/
#define HSID_F039_NOT_SUPP_ADDR			"301"		/* Not supported by the addressee			*/
#define HSID_F039_REC_NOTFOUND			"302"		/* Record not found  in the file			*/
#define HSID_F039_DUP_RECORD			"303"		/* Duplicate record.						*/
#define HSID_F039_FLD_CTRL_ERR			"304"		/* Fields control error						*/
#define HSID_F039_LOCK_FILE				"305"		/* Locked table								*/
#define HSID_F039_UNSUCCESS				"306"		/* Unsuccessful								*/
#define HSID_F039_FMT_ERROR				"307"		/* Format error								*/
#define HSID_F039_DUP_REC				"308"		/* Duplicate record.						*/
#define HSID_F039_UNKNOWN_FILE			"309"		/* Unknown file								*/
/*#define HSID_F039_REC_NOTFOUND			"381"	*/	/* Record not found							*/
#define HSID_F039_REV_ACCEPTED			"480"		/* Reversal accepted						*/
#define HSID_F039_INV_REV				"481"		/* Invalid reversal							*/
#define HSID_F039_ORIG_TRN_REV			"482"		/* Original Transaction Already Rev			*/
#define HSID_F039_RECON_REFUSED			"501"		/* Reconciliation Not Accepted				*/
#define HSID_F039_CNT_NA				"503"		/* Counters not available					*/
#define HSID_F039_RECON_DONE			"581"		/* Reconciliation done						*/
#define HSID_F039_NW_REQ_ACCEPTED		"800"		/* Network management message accepted		*/
#define HSID_F039_CONN_REFUSED			"880"		/* Connection not accepted					*/
#define HSID_F039_DL_HOT_LIST			"881"		/* Download Hot list						*/
#define HSID_F039_INV_TRN				"902"		/* Invalid transaction						*/
#define HSID_F039_NOSUCH_ISS			"908"		/* Not such issuer							*/
#define HSID_F039_SYS_FAULT				"909"		/* System fault								*/
#define HSID_F039_TIMEOUT				"911"		/* Time out									*/
#define HSID_F039_ISS_UNV				"912"		/* Issuer not available						*/
#define HSID_F039_NO_CONN				"992"		/* No Connection							*/
#define HSID_F039_PIN_VERIF_FAULT		"993"		/* Pin verification fault					*/
#define HSID_F039_TRN_PROC_FAULT		"994"		/* Transaction processing fault				*/
#define HSID_F039_SERVER_PROC_FAULT		"995"		/* Server processing fault					*/

#define HSID_F039_LEN					3

/**FILED P48**/

#define HSID_F048_P10_MAC_KEY_TAG										"P10"
#define HSID_F048_P16_PIN_TRANSPORT_KEY_TAG								"P16"
#define HSID_F048_P48_TCC_TAG                                         	"P48"
#define HSID_F048_P87_CVV_RESULT_CODE_TAG                               "P87"
#define HSID_F048_P88_CRYPT_RESULT_CODE_TAG                             "P88"
#define HSID_F048_P83_ADDRESS_VERIF_REP_TYPE_TAG						"P83"
#define HSID_F048_P92_CVC2_DATA_TAG                                     "P92"
#define HSID_F048_P95_NETWORK_CODE_TAG                                  "P95"
#define HSID_F048_P31_MSG_REASON_CODE_TAG                               "P31"
#define HSID_F048_P32_CODE_USAGE_TAG                                    "P32"
#define HSID_F048_P33_INDICATEUR_DOC_TAG                                "P33"
#define HSID_F048_P34_CHGBCK_REF_TAG                                    "P34"
#define HSID_F048_P35_SMS_MESSAGE_TEXT_TAG                              "P35"
#define HSID_F048_P39_MCI_ON_BEHALF_SERV                                "P39"
#define HSID_F048_P40_NETWORK_ID_TAG                                    "P40"
#define HSID_F048_P51_MCI_ON_BEHALF_SERV                                "P51"
#define HSID_F048_P52_MCI_PAN_MAPPING_INFO                              "P52"
#define HSID_F048_P63_MC_NETWORK_DATA_TAG                               "P63"
#define HSID_F048_P64_AUTH_ADD_INFO										"P64"	/*AEA20210712 Enh21.Q4 AN4972*/
#define HSID_F048_P71_RECURRING_FLAG_TAG                                "P71"
#define HSID_F048_P77_TRANS_XID_TAG                                     "P77"
#define HSID_F048_P61_POS_DATA_EURO_PVTAG                               "P61"
#define HSID_F048_P82_ADDRESS_VERIF_REQ_TYPE_TAG                        "P82"
#define HSID_F048_P90_AVS_DATA                                         	"P90"
#define HSID_F048_R01_CTLS_DEVICE_TYPE                                  "R01"
#define HSID_F048_G03_POS_ENVIRONMENT_TAG                               "G03"
#define HSID_F048_P22_POS_ENTRY_MODE_PVTAG                              "P22"
#define HSID_F048_P25_POS_CONDITION_CODE_PVTAG                          "P25"
#define HSID_F048_P60_ADTNL_POS_DATA_PVTAG                              "P60"
#define HSID_F048_P20_ACRO_ATM_PURCHASE_TAG                             "P20"
#define HSID_F048_P21_MERCHANT_ADDITIONAL_DATA							"P21"
#define HSID_F048_M32_MCI_ASSIGNED_ID                             		"M32"
#define HSID_F048_P56_DCC_DATA                          				"P56"
#define HSID_F048_P58_DIGITAL_PAYMENT                          			"P58"	/*AMER20191023 PROD00072515: MC Enh19.Q2 AN2620*/
#define HSID_F048_N21_NET_VISA_MERC_ID                        		    "N21"	/* HAL 20201803 Enh Q2.2020 Art 9.3.3*/ 
#define HSID_F048_N22_NET_VISA_EXT_STIP_REASON_CODE                     "N22"	/* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/  
#define HSID_F048_N54_SECURITY_SERV_DATA                          		"N54"	/*AMER20191023 PROD00072515: MC Enh19.Q2 AN2122*/
#define HSID_F048_N55_FRAUD_SCORE                          				"N55"	/*AMER20210416 PLUTONL-2985: MC Enh19.Q2 AN2122*/
#define HSID_F048_N56_MERCHANT_ADVICE_CODE								"N56"	/*AEA20210712 Enh21.Q4 AN4761*/
#define HSID_F048_N57_MC_TRACE_ID										"N57"	/*AEA20210712 Enh21.Q4 AN4972*/
#define HSID_F048_S20_MONEY_TRANSFER_DATA                               "S20"	/*AEA20210712 Enh21.Q4 AN4972*/
#define HSID_F048_S26_ECOM_AUTHENTIF_DATA                               "S26"   /* HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/
#define HSID_F048_S27_PSD2_DATA                                         "S27"   /* HAL 20201803 Enh Q2.2020 Art 9.3.3*/ 
/*Start AMER201211126 PLUTONL-3140*/
#define HSID_F048_P66_ECOM_INFO		                          			"P66"	
#define HSID_F048_K06_DS_TRX_ID		                          			"K06"	
#define HSID_F048_K17_ACS_TRX_ID		                       			"K17"
/*End AMER201211126 PLUTONL-3140*/
#define HSID_F048_N58_TOKEN_PROGAM                                      "N58"    /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
#define HSID_F048_N59_MCI_MIT_CIT_INDICATOR                             "N59"    /*IZL 03062022 PLUTONL-4810 ENH 22Q2 MC AN5524*/

#define HSID_F048_NB_FIELDS		47 /*46-> 47 IZL 03062022 PLUTONL-4810 ENH 22Q2 MC AN5524*/ /*45-> 46*//*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/ /*44-> 45/* ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/ /* 30-->34 HAL PROD00073823 20200402 Enh Q2.2020 */ /* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1 to 35*/  /*42-->44 IZL 20220121 PLUTONL-4015 ENH 22Q2 MC AN5520*/  
#define HSID_F048_P20_LEN		20

#define HSID_F048_P32_LEN		1
#define HSID_F048_P33_LEN		1
#define HSID_F048_P34_LEN		6

/**KEY EXCHANG P48**/


/*Start IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define HSID_F048_P64_P7_UNSPECIFIED                'N'    /* Unspecified/Credentials not stored */
#define HSID_F048_P64_P7_UNSHEDULED                 'C'    /* Unscheduled Credential on File */
#define HSID_F048_P64_P7_STANDING_ORDER             'S'    /* Standing Order */
#define HSID_F048_P64_P7_RECURRING                  'R'    /* Recurring*/
#define HSID_F048_P64_P7_INSTALLMENT                'I'    /* Installment */
#define HSID_F048_P64_P7_CH_CRDTL_ONFILE 		    'F'    /* Cardholder Credential-on-file */
/*End IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/


#define HSID_F048_P87_CVV_CVV2_NO_CHECK									' '
#define HSID_F048_P87_CVV_CVV2_NOK										'1'
#define HSID_F048_P87_CVV_CVV2_OK										'2'
#define HSID_F048_P87_CVV3_INVALID_LEN									'E'
#define HSID_F048_P87_CVV3_NO_VALIDATION								'P'
#define HSID_F048_P87_CVV3_INVALID										'Y'

#define HSID_F048_P87_LEN												1

#define HSID_F061_039_RESPONSE_CODE_TAG                                  "039"
#define HSID_F061_061_ADTNL_POS_DATA1_TAG                                "061"

#define HSID_F061_NB_FIELDS		2


#define HSID_F062_F01_CPS_ACI_TAG                                         "F01"
#define HSID_F062_F02_CPS_TRANS_ID_TAG                                    "F02"
#define HSID_F062_F03_TCC_TAG                                         	  "F03"
#define HSID_F062_F04_CPS_VALIDATION_DATE_TAG                             "F04"
#define HSID_F062_F05_CPS_MARKET_SPECIFIC_TAG                             "F05"
#define HSID_F062_F06_CPS_DURATION_TAG                                    "F06"
#define HSID_F062_F19_CPS_MVV_TAG                                   	  "F19"


#define HSID_F062_F04_LOCATION          'A'
#define HSID_F062_F04_HOTEL             'H'
#define HSID_F062_F04_OTHERS            'N'

#define HSID_F062_F04_LEN            1


#define HSID_F062_NB_FIELDS		6
/**ISO INTERNE*/
#define F048_P87_CVV_CVV2_NO_CHECK										'4'
#define F048_P87_CVV_CVV2_NOK											'2'
#define F048_P87_CVV_CVV2_OK											'0'
#define F048_P87_CVV3_INVALID_LEN										'E'
#define F048_P87_CVV3_NO_VALIDATION										'P'
#define F048_P87_CVV3_INVALID											'Y'

#define HSID_REQ_TYPE_SCREEN			"SCREEN"
#define HSID_REQ_TYPE_SAF				"SAF"
#define HSID_REQ_TYPE_ZPKEXCHANGE		"ZPKEXCHANGE"
#define HSID_REQ_TYPE_TAKEXCHANGE		"TAKEXCHANGE"
#define HSID_REQ_TYPE_SIGNOFFSAF		"SIGNOFFSAF"
#define HSID_REQ_TYPE_PERIODIQUE		"PERIODIQUE"

#define HSID_H_ERR_POS_NOERROR			"000"

#define HSID_MR_LATE_RESPONSE			"4006"
#define HSID_MR_INVALID_RESPONSE		"4020"
#define HSID_MR_TIME_OUT				"4200"
#define HSID_MR_NOT_SPECIFIED			"4001"


#define RESOURCE_ID_IB 					"110101"
#define RESOURCE_SMS_DFLT 				"110003"

#define MAC_MNG_FLAG_Y  				'Y'
#define MAC_MNG_FLAG_N  				'N'

#define V_OUTLETVERIF_FLAG_Y  			'Y'
#define V_OUTLETVERIF_FLAG_N  			'N'


#define E_COMMERCE_MNGT_Y  				'Y'
#define E_COMMERCE_MNGT_N  				'N'


#define DYNAMIC_ADVICE_FLAG_Y  			'Y'
#define DYNAMIC_ADVICE_FLAG_N  			'N'

#define SIGNOFF_SAF_MANAGED 			'M'
#define SIGNOFF_SAF_NOT_MANAGED  		'N'

#define MCC_AUTOMATED_CASH_CODE     6011
#define MCC_TELCOM_SERVICE_CODE     4814

#define ADD_RECORD  					"1"
#define CHG_RECORD  					"2"
#define DEL_RECORD  					"3"
#define CONS_RECORD  					"4"

#define UPD_CODE_LEN  					1

#define STLMNT_MATCHED  				"M"
#define STLMNT_UNMATCHED  				"N"

#define MTI_LEN							4

#endif


