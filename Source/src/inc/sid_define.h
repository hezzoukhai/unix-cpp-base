#ifndef PWC_SID_DEFINE__H
#define PWC_SID_DEFINE__H


#define		SID_LENGTH_LEN				4
#define		SID_HDR_LEN					11
#define		SID_MSG_TYPE_LEN			4




#define SID_F003_P1_LEN					2
#define SID_F003_P2_LEN					2
#define SID_F003_P3_LEN					2
#define SID_F003_LEN 					6


#define SID_F004_LEN 		12
#define SID_F005_LEN 		12
#define SID_F006_LEN 		12
#define SID_F007_LEN 		10
#define SID_F008_LEN 		8
#define SID_F009_LEN 		8
#define SID_F010_LEN		8
#define SID_F011_LEN 		6
#define SID_F012_LEN 		12
#define SID_F013_LEN 		4
#define SID_F014_LEN 		4
#define SID_F015_LEN 		6
#define SID_F016_LEN 		4
#define SID_F017_LEN 		4
#define SID_F018_LEN 		4
#define SID_F019_LEN 		3
#define SID_F020_LEN 		3
#define SID_F021_LEN 		3
#define SID_F022_LEN 		12
#define SID_F023_LEN 		3
#define SID_F026_LEN 		4
#define SID_F027_LEN 		1
#define SID_F028_LEN 		6
#define SID_F029_LEN 		3
#define SID_F030_LEN 		24
#define SID_F037_LEN 		12
#define SID_F038_LEN 		6
#define SID_F040_LEN 		3
#define SID_F041_LEN 		8
#define SID_F042_LEN 		15
#define SID_F049_LEN 		3
#define SID_F050_LEN 		3
#define SID_F051_LEN 		3
#define SID_F052_LEN 		16
#define SID_F057_LEN 		3
#define SID_F064_LEN 		8
#define SID_F065_LEN 		0
#define SID_F067_LEN 		2
#define SID_F068_LEN 		3
#define SID_F069_LEN 		3
#define SID_F070_LEN 		3
#define SID_F071_LEN 		8
#define SID_F073_LEN 		6
#define SID_F074_LEN 		10
#define SID_F075_LEN 		10
#define SID_F076_LEN 		10
#define SID_F077_LEN 		10
#define SID_F078_LEN 		10
#define SID_F079_LEN 		10
#define SID_F080_LEN 		10
#define SID_F081_LEN 		10
#define SID_F082_LEN 		10
#define SID_F083_LEN 		10
#define SID_F084_LEN 		10
#define SID_F085_LEN 		10
#define SID_F086_LEN 		16
#define SID_F087_LEN 		16
#define SID_F088_LEN 		16
#define SID_F089_LEN 		16
#define SID_F090_LEN 		35
#define SID_F091_LEN 		3
#define SID_F092_LEN 		3
#define SID_F097_LEN 		17
#define SID_F098_LEN 		25
#define SID_F105_LEN 		16
#define SID_F106_LEN 		16
#define SID_F107_LEN 		10
#define SID_F108_LEN 		10
#define SID_F128_LEN 		8




#define SID_F002_MAX_LEN 	19
#define SID_F031_MAX_LEN 	9
#define SID_F032_MAX_LEN 	11
#define SID_F033_MAX_LEN 	11
#define SID_F034_MAX_LEN 	28
#define SID_F035_MAX_LEN 	37
#define SID_F036_MAX_LEN 	4
#define SID_F043_MAX_LEN 	99
#define SID_F044_MAX_LEN 	999
#define SID_F045_MAX_LEN 	77
#define SID_F046_MAX_LEN 	255
#define SID_F047_MAX_LEN 	255
#define SID_F048_MAX_LEN 	999
#define SID_F053_MAX_LEN 	16
#define SID_F054_MAX_LEN 	120
#define SID_F055_MAX_LEN 	255
#define SID_F056_MAX_LEN 	255
#define SID_F058_MAX_LEN 	255
#define SID_F059_MAX_LEN 	14
#define SID_F060_MAX_LEN 	6
#define SID_F061_MAX_LEN 	999
#define SID_F062_MAX_LEN 	999
#define SID_F063_MAX_LEN 	999
#define SID_F066_MAX_LEN 	1
#define SID_F072_MAX_LEN 	999
#define SID_F093_MAX_LEN 	11
#define SID_F094_MAX_LEN 	11
#define SID_F095_MAX_LEN 	42
#define SID_F096_MAX_LEN 	17
#define SID_F099_MAX_LEN 	11
#define SID_F100_MAX_LEN 	11
#define SID_F101_MAX_LEN 	17
#define SID_F102_MAX_LEN 	28
#define SID_F103_MAX_LEN 	28
#define SID_F104_MAX_LEN 	16
#define SID_F115_MAX_LEN 	24
#define SID_F123_MAX_LEN 	29
#define SID_F125_MAX_LEN 	255
#define SID_F126_MAX_LEN 	255



#define SID_F024_INIT_REQUEST			"100"			/* Initial request						*/
#define SID_F024_PRE_AUTH				"101"			/* Pre authorization					*/
#define SID_F024_SUPP_AMNT				"106"			/* Supplementary amount - Accurate		*/
#define SID_F024_SUPP_AMNT_ESTMD		"107"			/* Supplementary amount - Estimated		*/
#define SID_F024_BAL_REQUEST			"108"			/* Balance request						*/
#define SID_F024_ACC_VERIF				"181"			/* Account verification					*/

#define SID_F024_AUTH_PREV				"201"			/* Authorized previously				*/
#define SID_F024_AUTH_PREV_AMNT_DIFF	"202"			/* Authorized previously - Amount differs*/
#define SID_F024_TOPUP_FROM_CH_ACC		"221"			/* Topup from cardholder account: Ref: Check EMV_OfflineLimitTopUpProcessing	*/
#define SID_F024_BAL_INQUIRY			"281"			/* Balance Inquiry						*/

#define SID_F024_ADD_RECORD				"301"			/* Add record							*/
#define SID_F024_CHG_RECORD				"302"			/* Change record (partial update)		*/
#define SID_F024_DEL_RECORD				"303"			/* Delete record						*/
#define SID_F024_FULL_CHG_RECORD		"304"			/* Change record (full update)			*/
#define SID_F024_INQ_RECORD				"305"			/* Inquiry								*/

#define SID_F024_FULL_REVERSAL			"400"			/* Full reversal						*/
#define SID_F024_PARTIAL_REVERSAL		"402"			/* Partial reversal						*/


#define SID_F024_STOP_INTERFACE			"600"			/* Stop interface					*/
#define SID_F024_PARAM_REFRESH			"601"			/* Parameters refresh				*/
#define SID_F024_PING_RESOURCE			"602"			/* Ping (resource status)			*/
#define SID_F024_SIGNON_BIN				"603"			/* Sign-on (BIN)					*/
#define SID_F024_SIGNOFF_BIN			"604"			/* Sign-off (BIN)					*/
#define SID_F024_GRP_SIGNON				"605"			/* Group sign-on					*/
#define SID_F024_GRP_SIGNOFF			"606"			/* Group sign-off					*/
#define SID_F024_START_SAF				"607"			/* Start SAF						*/
#define SID_F024_END_SAF				"608"			/* End SAF							*/
#define SID_F024_ECHO_TEST				"609"			/* Echo test						*/
#define SID_F024_LINE_DOWN_IND			"610"			/* Line Down Indicator				*/
#define SID_F024_LINE_UP_IND			"611"			/* Line Up Indicator				*/
#define SID_F024_START_INTERFACE		"612"			/* Start interface					*/
#define SID_F024_STATISTICS				"620"			/* Statistics						*/


#define SID_F024_SIGNON					"801"			/* Sign-on (BIN)						*/
#define SID_F024_SIGNOFF				"802"			/* Sign-off (BIN)						*/
#define SID_F024_ECHO					"803"			/* Echo test							*/
#define SID_F024_SIGNON_SAF				"804"			/* Sign-on (SAF)						*/
#define SID_F024_SIGNOFF_SAF			"805"			/* Sign-off (SAF)						*/
#define SID_F024_KEY_CHG				"811"			/* Key change							*/
#define SID_F024_KEY_VERIFY				"812"			/* Key verification						*/
#define SID_F024_KEY_REPEAT				"813"			/* Key repeat							*/
#define SID_F024_RECON_REQ				"821"			/* Reconciliation request				*/
#define SID_F024_INT_RECON_REQ			"822"			/* Intermediary Reconciliation request	*/
#define SID_F024_INIT_REQ				"880"			/* Initialization request				*/

#define SID_F024_N_SIGNON_BIN			603
#define SID_F024_N_SIGNOFF_BIN			604
#define SID_F024_N_ECHO_TEST			609


#define SID_F024_N_SIGNON				801
#define SID_F024_N_SIGNOFF				802
#define SID_F024_N_ECHO					803
#define SID_F024_N_SIGNON_SAF			804
#define SID_F024_N_SIGNOFF_SAF			805
#define SID_F024_N_KEY_CHG				811
#define SID_F024_N_KEY_VERIFY			812
#define SID_F024_N_KEY_REPEAT			813
#define SID_F024_N_RECON_REQ			821
#define SID_F024_N_INT_RECON_REQ		822
#define SID_F024_N_INIT_REQ				880

#define SID_F024_N_ZPK_KEY_CHG			811
#define SID_F024_N_TAK_KEY_CHG			899

#define SID_F024_LEN						3
#define SID_F025_LEN						4


#define SID_F039_APPROVAL				"000"		/* Approval									*/
#define SID_F039_APPROVAL_W_ID			"001"		/* Approuved with ID						*/
#define SID_F039_APPROVAL_VIP			"003"		/* Approuved VIP							*/
#define SID_F039_APPROVAL_T3_UPD		"004"		/* Approuved, Track III updated				*/
#define SID_F039_APPROVAL_CHIP_UPD		"007"		/* Approuved update chip					*/
#define SID_F039_NO_RSN_TO_DEC			"085"		/* No reason to declined					*/
#define SID_F039_DECLINE				"100"		/* Rejected									*/
#define SID_F039_EXPIRED_CARD			"101"		/* Expired card								*/
#define SID_F039_SUSPECTED_FRAUD		"102"		/* Suspected fraud							*/
#define SID_F039_PRIVATE_CARD			"104" 		/* Private card								*/
#define SID_F039_PIN_TRIES_EXC			"106"		/* PIN entry tries exceeded					*/
#define SID_F039_REF_TO_ISS				"107"		/* Refer to card issuer						*/
#define SID_F039_REF_TO_ISS_SC			"108"		/* Refer to issuer; special condit			*/
#define SID_F039_INV_MER				"109"		/* Invalid Merchant							*/
#define SID_F039_INV_AMNT				"110"		/* Invalid amount							*/
#define SID_F039_INV_CARD_NBR			"111"		/* Invalid card number						*/
#define SID_F039_PIN_REQUIRED			"112"		/* PIN element required						*/
#define SID_F039_UNKOWN_CARD			"114"		/* Unknown card								*/
#define SID_F039_FCT_NA					"115"		/* Function not available					*/
#define SID_F039_UNSUF_FUNDS			"116"		/* No sufficient funds						*/
#define SID_F039_PIN_ERROR				"117"		/* Incorrect PIN							*/
#define SID_F039_NO_SUCH_CARD			"118"		/* No such card								*/
#define SID_F039_TRN_NOPERM_CH			"119"		/* Cardholder trx not permitted				*/
#define SID_F039_TNR_NOPERM_TERM		"120"		/* Terminal trx not permitted				*/
#define SID_F039_WTDWL_LMT_EXC			"121"		/* Exceeds withdrawal limit					*/
#define SID_F039_SEC_VIOLATION			"122"		/* Security violation						*/
#define SID_F039_WTDWL_FREQ_EXC			"123"		/* Withdrawal frequency exceeded			*/
#define SID_F039_CARD_NOT_IN_SRV		"125"		/* Card not in service						*/
#define SID_F039_PIN_FMT_ERR			"126"		/* Wrong PIN format							*/
#define SID_F039_PIN_LEN_ERR			"127"		/* Error PIN length							*/
#define SID_F039_CRYPTO_ERR				"128"		/* Cryptographic error						*/
#define SID_F039_FRAUD_SUSPECTED		"129"		/* Suspected fraud							*/
#define SID_F039_NO_CR_ACC				"180"		/* No credit account						*/
#define SID_F039_NO_CHEQ_ACC			"181"		/* No cheque account						*/
#define SID_F039_NO_SAVING_ACC			"182"		/* No saving Account						*/
#define SID_F039_CVV_ERROR				"183"		/* Bad CVV									*/
#define SID_F039_INV_DATE				"184"		/* Invalid date								*/
#define SID_F039_NOSUP_SERV_CODE		"185"		/* Service code not supported				*/
#define SID_F039_DECLINE_T3_UPD			"199"		/* Refused with Track III updating			*/
#define SID_F039_DECLINE_PICKUP			"200"		/* Rejected, Pick up card					*/
#define SID_F039_EXPIRED_PICKUP			"201"		/* Expired card, Pick up					*/
#define SID_F039_FRAUDULENT_PICKUP		"202"		/* Fraudulent, Pick up						*/
#define SID_F039_PRIV_CARD_PICKUP		"204"		/* Private card, Pick up					*/
#define SID_F039_CALL_ACQ_PICKUP		"205"		/* Call security acquirer, pick up			*/
#define SID_F039_PIN_TRIES_EXC_PICKUP	"206"		/* PIN entry tries exceeded					*/
#define SID_F039_SPEC_COND_PICKUP		"207"		/* Special conditions, Pick up				*/
#define SID_F039_LOST_CARD_PICKUP		"208"		/* Lost card, Pick up						*/
#define SID_F039_STOLEN_CARD_PICKUP		"209"		/* Stolen card, Pick up						*/
#define SID_F039_SUSP_FRD_PICKUP		"210"		/* Suspected fraud, Pick up					*/
#define SID_F039_ALTERN_AMNT_CANCELED	"280"		/* Alternate Amount cancelled				*/
#define SID_F039_PICKUP					"299"		/* Pick up									*/
#define SID_F039_PROC_SUCCESS			"300"		/* Successfully processed					*/
#define SID_F039_NOT_SUPP_ADDR			"301"		/* Not supported by the addressee			*/
#define SID_F039_REC_NOTFOUND			"302"		/* Record not found  in the file			*/
#define SID_F039_DUP_RECORD				"303"		/* Duplicate record.						*/
#define SID_F039_FLD_CTRL_ERR			"304"		/* Fields control error						*/
#define SID_F039_LOCK_FILE				"305"		/* Locked table								*/
#define SID_F039_UNSUCCESS				"306"		/* Unsuccessful								*/
#define SID_F039_FMT_ERROR				"307"		/* Format error								*/
#define SID_F039_DUP_REC				"308"		/* Duplicate record.						*/
#define SID_F039_UNKNOWN_FILE			"309"		/* Unknown file								*/
/*#define SID_F039_REC_NOTFOUND			"381"	*/	/* Record not found							*/
#define SID_F039_REV_ACCEPTED			"480"		/* Reversal accepted						*/
#define SID_F039_INV_REV				"481"		/* Invalid reversal							*/
#define SID_F039_ORIG_TRN_REV			"482"		/* Original Transaction Already Rev			*/
#define SID_F039_RECON_REFUSED			"501"		/* Reconciliation Not Accepted				*/
#define SID_F039_CNT_NA					"503"		/* Counters not available					*/
#define SID_F039_RECON_DONE				"581"		/* Reconciliation done						*/
#define SID_F039_NW_REQ_ACCEPTED		"800"		/* Network management message accepted		*/
#define SID_F039_CONN_REFUSED			"880"		/* Connection not accepted					*/
#define SID_F039_DL_HOT_LIST			"881"		/* Download Hot list						*/
#define SID_F039_INV_TRN				"902"		/* Invalid transaction						*/
#define SID_F039_NOSUCH_ISS				"908"		/* Not such issuer							*/
#define SID_F039_SYS_FAULT				"909"		/* System fault								*/
#define SID_F039_TIMEOUT				"911"		/* Time out									*/
#define SID_F039_ISS_UNV				"912"		/* Issuer not available						*/
#define SID_F039_NO_CONN				"992"		/* No Connection							*/
#define SID_F039_PIN_VERIF_FAULT		"993"		/* Pin verification fault					*/
#define SID_F039_TRN_PROC_FAULT			"994"		/* Transaction processing fault				*/
#define SID_F039_SERVER_PROC_FAULT		"995"		/* Server processing fault					*/

#define SID_F039_LEN					3

#define SID_F048_P88_CVV2_NO_CHECK       ' '
#define SID_F048_P88_CVV2_NOK            '1'
#define SID_F048_P88_CVV2_OK             '2'

#define SID_REQ_TYPE_SCREEN				"SCREEN"
#define SID_REQ_TYPE_SAF				"SAF"
#define SID_REQ_TYPE_ZPKEXCHANGE		"ZPKEXCHANGE"
#define SID_REQ_TYPE_TAKEXCHANGE		"TAKEXCHANGE"
#define SID_REQ_TYPE_SIGNOFFSAF			"SIGNOFFSAF"
#define SID_REQ_TYPE_PERIODIQUE			"PERIODIQUE"

#define MAC_MNG_FLAG_Y  				'Y'
#define MAC_MNG_FLAG_N  				'N'


#define DYNAMIC_ADVICE_FLAG_Y  			'Y'
#define DYNAMIC_ADVICE_FLAG_N  			'N'

/*NAB06 FIELD 48 PLUTONL-3131*/
#define SID_F48_NB_FIELDS			  59
#define SID_F55_NB_FIELDS			  31


#define SID_F48_FILE_UP_ERR_CODE     "P01"
#define SID_F48_MSG_TEXT     		 "P02"
#define SID_F48_KEY_TAG     		 "P10"
#define SID_F48_ORIG_TRX_ID     	 "P11"
#define SID_F48_NEW_PIN_DATA     	 "P16"
#define SID_F48_REF_NUM     		 "P17"
#define SID_F48_PAR     		 	 "P18"
#define SID_F48_VISA_MERCHANT     	 "P19"
#define SID_F48_ICVV_RESULT     	 "P20"
#define SID_F48_MERC_ADDTL_DATA      "P21"
#define SID_F48_ARQC_RESULT      	 "P25"
#define SID_F48_PIN_RESULT      	 "P27"
#define SID_F48_CHIP_ADDTL      	 "P30"
#define SID_F48_MERCHANT_ID      	 "P32"
#define SID_F48_OBS      	 		 "P34"
#define SID_F48_TRACE_ID      	 	 "P35"
#define SID_F48_MERCHANT_ACCNUM      "P41"
#define SID_F48_MC_UCAF    			 "P43"
#define SID_F48_ROUTING_NW_CODE    	 "P47"
#define SID_F48_KSN_DESCRIPTION    	 "P49"
#define SID_F48_KSN   	             "P50"
#define SID_F48_AAV   	             "P51"
#define SID_F48_DIGITAL_PAYMT_DATA   "P52"
#define SID_F48_ECOM_CRPT_RESULT     "P54"
#define SID_F48_TOKEN_DATA           "P55"
#define SID_F48_DCC_DATA             "P56"
#define SID_F48_AFD_REF_NUM          "P57"
#define SID_F48_ACCPT_POINT_ID       "P58"
#define SID_F48_SCORING_RESULT       "P60"
#define SID_F48_ADD_POS_DATA         "P61"
#define SID_F48_AUTH_ADTNL_INFO      "P64"
#define SID_F48_ECOM_AUTH_INFO       "P66"
#define SID_F48_AUTHORISATION_ID     "P67"
#define SID_F48_TRANSACTION_ID       "P68"
#define SID_F48_TERMINAL_INFO        "P70"
#define SID_F48_RECCURING_IND        "P71"
#define SID_F48_XID                  "P77"
#define SID_F48_DINERS_NW_REF_ID     "P80"
#define SID_F48_ADD_VERIF_REQUEST    "P82"
#define SID_F48_ADD_VERIF_RESPONSE   "P83"
#define SID_F48_CVV_RESULT           "P87"
#define SID_F48_EXT_RESP_CODE        "P88"
#define SID_F48_FRAUD_SCORE          "P90"
#define SID_F48_AMEX_TRX_ID          "P91"
#define SID_F48_CVC2                 "P92"
#define SID_F48_NETWORK_CODE         "P95"
#define SID_F48_MONEY_TRANSFER_DATA  "S20"
#define SID_F48_ADDRESS_DATA         "S21"
#define SID_F48_SALE_INFO            "S22"
#define SID_F48_SALE_ITEMS_DATA      "S23"
#define SID_F48_ECOM_AUTH_DATA       "S26"
#define SID_F48_PSD2                 "S27"
#define SID_F48_SMS_CHARGEBACK_DATA  "K01"
#define SID_F48_SERVER_TRANS_ID      "K06"
#define SID_F48_ACS_TRANS_ID         "K08"
#define SID_F48_AUTH_EXMP_IND        "K09"
#define SID_F48_MOBILE_SOLUTION_ID   "K10" 
#define SID_F48_NETWORK_MERCHANT_ID  "N21"
#define SID_F48_SECURITY_ADTNL_DATA  "N52" 

/*NAB06 FIELD 55 PLUTONL-3131*/
#define SID_F55_ISS_SCRIPT1 	     "71"
#define SID_F55_ISS_SCRIPT2          "72"
#define SID_F55_APP_INTER_PROFILE    "82"
#define SID_F55_DED_FILE_NAME 	     "84"
#define SID_F55_ISS_SCRIPT_CMD 	     "86"
#define SID_F55_AUTH_RESP_CODE 	     "8A"
#define SID_F55_ISS_AUTHEN_DATA      "91"
#define SID_F55_TVR 			     "95"
#define SID_F55_TRANS_DATE           "9A"
#define SID_F55_TRANS_TYPE           "9C"
#define SID_F55_TRANS_CUR_CODE       "5F2A"
#define SID_F55_PAN_SEQ_NUMBER       "5F34"
#define SID_F55_APP_PROP_DATA        "9F0A"
#define SID_F55_TRANS_AMOUNT         "9F02"
#define SID_F55_OTHER_AMOUNT         "9F03"
#define SID_F55_APPLICATION          "9F06"
#define SID_F55_TERM_APP_VER_NUM     "9F09"
#define SID_F55_TAG_ISS_APP_DATA     "9F10"
#define SID_F55_TERM_COUNTRY_CODE    "9F1A"
#define SID_F55_IFD_SERIAL_NUM       "9F1E"
#define SID_F55_FFI                  "9F6E"
#define SID_F55_APP_CRYPTOGRAM       "9F26"
#define SID_F55_CRYPTO_INFO_DATA     "9F27"
#define SID_F55_TERM_CAP             "9F33"
#define SID_F55_CVM                  "9F34"
#define SID_F55_TERM_TYPE            "9F35"
#define SID_F55_ATC                  "9F36"
#define SID_F55_UNPRED_NUMBER        "9F37"
#define SID_F55_TRANS_SEQ_NUM        "9F41"
#define SID_F55_TRANS_CATEG_CODE     "9F53"
#define SID_F55_ISS_SCRIPT_RESULT    "9F5B"


#endif


