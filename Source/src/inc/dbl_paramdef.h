#ifndef HPS_DBLPARAMDEF_H
#define HPS_DBLPARAMDEF_H
/*
	- type de ticket et journal imprimable --------------
*/
#define NP_WITHDRAWAL_TICKET_ENG			"001"
#define NP_STATEMENT_TICKET_ENG				"002"
#define NP_PICKUP_TICKET_ENG				"003"
#define NP_BALANCE_TICKET_ENG				"004"
#define NP_CHEQUE_BOOK_TICKET_ENG			"005"
#define NP_CUTOFF_JOURNAL_ENG				"006"
#define NP_WITHDRAWAL_JOURNAL_ENG			"007"
#define NP_BALANCE_JOURNAL_ENG				"008"
#define NP_STATEMENT_JOURNAL_ENG			"009"
#define NP_CHEQUE_BOOK_JOURNAL_ENG			"010"
#define NP_AUTOBANK_WITHDRAWAL_TICKET_ENG		"011"
#define NP_AUTOBANK_BALANCE_TICKET_ENG			"012"
#define NP_AUTOBANK_SHORT_STAT_TICHET_ENG		"013"
#define NP_BILL_PAYMENT_JOURNAL_ENG                     "014"
#define NP_FUNDS_TRANSFER_TICKET_ENG			"015"
#define NP_LOAN_BALANCE_TICKET_ENG			"016"
#define NP_SHORT_STATEMENT_TICKET_ENG			"017"
#define NP_SHORT_STATEMENT_JOURNAL_ENG			"018"
#define NP_FUNDS_TRANSFER_JOURNAL_ENG			"019"
#define NP_CASH_DEPOSIT_TICKET_ENG			"020"
#define NP_CHEQUE_DEPOSIT_TICKET_ENG			"021"
#define NP_DOCUMENT_DEPOSIT_TICKET_ENG			"022"
#define NP_CASH_DEPOSIT_JOURNAL_ENG			"023"
#define NP_CHEQUE_DEPOSIT_JOURNAL_ENG			"024"
#define NP_DOCUMENT_DEPOSIT_JOURNAL_ENG			"025"
#define NP_CASH_DEPOSIT_ENVELOPE_ENG			"026"
#define NP_CHEQUE_DEPOSIT_ENVELOPE_ENG			"027"
#define NP_DOCUMENT_DEPOSIT_ENVELOPE_ENG		"028"
#define NP_PICKUP_JOURNAL_ENG				"029"
#define NP_MULT_CURR_WITHDRAWAL_TICKET_ENG		"030"
#define NP_CASH_FORGET_JOURNAL_ENG		        "031"
#define NP_PICKUP_TIME_OUT_JOURNAL_ENG			"032"
#define NP_BALANCE_TICKET_OFF_US_ENG			"033"
#define NP_WITHDRAWAL_TICKET_OFF_US_ENG			"034"
#define NP_BILL_PAYMENT_TICKET_OFF_US_ENG               "038"
#define NP_PIN_CHANGE_TICKET_ENG			"037"
/*BEGIN AHM 31/08/2006*/
#define NP_MIX_DEPOSIT_TICKET_ENG                       "039"
#define NP_MIX_DEPOSIT_ENVELOPE_ENG                     "043"
#define NP_MIX_DEPOSIT_JOURNAL_ENG                      "044"
/*END AHM*/
#define NP_BILL_PAYMENT_TICKET_ENG                      "040"
#define NP_FIRST_LOAN_ENG				"041"
#define NP_SECOND_LOAN_ENG				"042"
#define NP_THIRD_LOAN_ENG				"043"
#define NP_FOURTH_LOAN_ENG				"044"
#define NP_MESSAGING_TICKET_ENG				"045"
#define NP_LOAN_INQUIRY_TICKET_ENG			"035"
#define NP_CASH_RETRACTED_TICKET_ENG			"036"
/*Fin modif FB 05-09-99*/
/*Debut modif FB 28-02-00 */
#define NP_TRANS_DECLINED_TICKET_ENG                    "046"
#define NP_TRANS_CANCELED_ENG                           "047"
#define NP_TRANS_CANCELED_OFF_US_ENG                    "247"
/*ADDV2*/
#define NP_INSUFF_FUNDS_TICKET_ENG				"011"
#define NP_INSUFF_FUNDS_NAT_TICKET_ENG			"012"
#define NP_INSUFF_FUNDS_INT_TICKET_ENG			"013"

#define NP_TRANS_DECLINED_NAT_TICKET_ENG		"048"
#define NP_TRANS_DECLINED_INT_TICKET_ENG		"049"
/*ADDV2*/
/*Fin   modif FB 28-02-00*/


#define NP_WITHDRAWAL_TICKET_AR				"051"
#define NP_STATEMENT_TICKET_AR				"052"
#define NP_PICKUP_TICKET_AR				"053"
#define NP_BALANCE_TICKET_AR				"054"
#define NP_CHEQUE_BOOK_TICKET_AR			"055"
#define NP_MULT_CURR_WITHDRAWAL_TICKET_AR		"056"
#define NP_BALANCE_TICKET_OFF_US_AR			"057"
#define NP_WITHDRAWAL_TICKET_OFF_US_AR			"058"
#define NP_BILL_PAYMENT_TICKET_OFF_US_AR                "059"
#define NP_AUTOBANK_WITHDRAWAL_TICKET_AR		"061"
#define NP_AUTOBANK_BALANCE_TICKET_AR			"062"
#define NP_AUTOBANK_SHORT_STAT_TICHET_AR		"063"
#define NP_FUNDS_TRANSFER_TICKET_AR			"065"
#define NP_LOAN_BALANCE_TICKET_AR			"066"
#define NP_SHORT_STATEMENT_TICKET_AR			"067"
#define NP_CASH_DEPOSIT_TICKET_AR			"070"
#define NP_CHEQUE_DEPOSIT_TICKET_AR			"071"
#define NP_DOCUMENT_DEPOSIT_TICKET_AR			"072"
#define NP_LOAN_INQUIRY_TICKET_AR			"085"
#define NP_PIN_CHANGE_TICKET_AR 			"087"
#define NP_BILL_PAYMENT_TICKET_AR                               "088"
#define NP_FIRST_LOAN_AR				"091"
#define NP_SECOND_LOAN_AR				"092"
#define NP_THIRD_LOAN_AR				"093"
#define NP_FOURTH_LOAN_AR				"094"
#define NP_MESSAGING_TICKET_AR				"095"
/*Fin modif FB 05-09-99*/

/*Debut modif FB 28-02-00 */
#define NP_TRANS_DECLINED_TICKET_AR                     "096"
#define NP_TRANS_CANCELED_AR                            "097"
#define NP_TRANS_CANCELED_OFF_US_AR                     "297"
/*ADDV2*/
#define NP_INSUFF_FUNDS_TICKET_AR			"061"
#define NP_INSUFF_FUNDS_NAT_TICKET_AR			"062"
#define NP_INSUFF_FUNDS_INT_TICKET_AR			"063"

/*BEGIN AHM 31/08/2006*/
#define NP_MIX_DEPOSIT_TICKET_AR                        "089"
/*END AHM*/

/*BEGIN AHM 29/08/2005*/
#define NP_ISS_RESP_CODE                		"90"
#define NP_ISS_RESP_CODE_LEN            		3
#define CHARACTER_SET_ARAB				'E'
/*END AHM*/

#define NP_TRANS_DECLINED_NAT_TICKET_AR			"098"
#define NP_TRANS_DECLINED_INT_TICKET_AR			"099"
/*ADDV2*/
/*Fin   modif FB 28-02-00*/

#define NP_WITHDRAWAL_TICKET_FR				"101"
#define NP_STATEMENT_TICKET_FR				"102"
#define NP_PICKUP_TICKET_FR				"103"
#define NP_BALANCE_TICKET_FR				"104"
#define NP_CHEQUE_BOOK_TICKET_FR			"105"
#define NP_CUTOFF_JOURNAL_FR				"106"
#define NP_WITHDRAWAL_JOURNAL_FR			"107"
#define NP_BALANCE_JOURNAL_FR				"108"
#define NP_STATEMENT_JOURNAL_FR				"109"
#define NP_CHEQUE_BOOK_JOURNAL_FR			"110"
#define NP_AUTOBANK_WITHDRAWAL_TICKET_FR		"111"
#define NP_AUTOBANK_BALANCE_TICKET_FR			"112"
#define NP_AUTOBANK_SHORT_STAT_TICHET_FR		"113"
#define NP_FUNDS_TRANSFER_TICKET_FR			"115"
#define NP_LOAN_BALANCE_TICKET_FR			"116"
#define NP_SHORT_STATEMENT_TICKET_FR			"117"
#define NP_SHORT_STATEMENT_JOURNAL_FR			"118"
#define NP_FUNDS_TRANSFER_JOURNAL_FR			"119"
#define NP_CASH_DEPOSIT_TICKET_FR			"120"
#define NP_CHEQUE_DEPOSIT_TICKET_FR			"121"
#define NP_DOCUMENT_DEPOSIT_TICKET_FR			"122"
#define NP_CASH_DEPOSIT_JOURNAL_FR			"123"
#define NP_CHEQUE_DEPOSIT_JOURNAL_FR			"124"
#define NP_DOCUMENT_DEPOSIT_JOURNAL_FR			"125"
#define NP_CASH_DEPOSIT_ENVELOPE_FR			"126"
#define NP_CHEQUE_DEPOSIT_ENVELOPE_FR			"127"
#define NP_DOCUMENT_DEPOSIT_ENVELOPE_FR			"128"
#define NP_PICKUP_JOURNAL_FR				"129"
#define NP_MULT_CURR_WITHDRAWAL_TICKET_FR		"130"
#define NP_CASH_FORGET_JOURNAL_FR		        "131"
#define NP_PICKUP_TIME_OUT_JOURNAL_FR			"132"
#define NP_BALANCE_TICKET_OFF_US_FR			"133"
#define NP_WITHDRAWAL_TICKET_OFF_US_FR			"134"
#define NP_LOAN_INQUIRY_TICKET_FR			"135"
#define NP_PIN_CHANGE_TICKET_FR 			"137"
#define NP_BILL_PAYMENT_TICKET_FR                               "140"
#define NP_BILL_PAYMENT_TICKET_OFF_US_FR                "138"
#define NP_FIRST_LOAN_FR				"141"
#define NP_SECOND_LOAN_FR				"142"
#define NP_THIRD_LOAN_FR				"143"
#define NP_FOURTH_LOAN_FR				"144"
#define NP_MESSAGING_TICKET_FR				"145"
#define NP_TRANS_DECLINED_TICKET_FR                     "146"
#define NP_TRANS_CANCELED_FR                            "147"
#define NP_INSUFF_FUNDS_TICKET_FR				"111"
#define NP_INSUFF_FUNDS_NAT_TICKET_FR			"112"
#define NP_INSUFF_FUNDS_INT_TICKET_FR			"113"

#define NP_TRANS_DECLINED_NAT_TICKET_FR			"148"
#define NP_TRANS_DECLINED_INT_TICKET_FR			"149"
/*ADDV2*/
/*Fin   modif FB 28-02-00*/

/*
	- type de text imprimable ----------------------------
*/
#define NP_OPERATION_DATE		"01"
#define NP_BALANCE_DATE			"02"
#define NP_OPERATION_HOUR		"03"
#define NP_CARD_NBR			"04"
#define NP_SOURCE_ACCOUNT		"05"
#define NP_ATM_NUMBER			"06"
#define NP_OPERATION_NUMBER		"07"
#define NP_BALANCE			"08"
#define NP_STATEMENT			"09"
#define NP_TEXT				"10"
#define NP_RESPONSE_CODE		"11"
#define NP_KNET_RESP_CODE            "90"

#define NP_AMOUNT			"12"
#define NP_FACILITY_AMOUNT		"14"
#define NP_PAST_DUE_AMOUNT		"15"
#define NP_PENDING_AMOUNT		"16"
#define NP_UNUTILISED_AMOUNT		"17"
#define NP_CURRENT_BALANCE		"18"
/* Debut Modif FB 28-02-00 pour compatibilite SGBS/BTR */
#define NP_ACCOUNT_BALANCE		"18"
/* FIN   Modif FB 28-02-00 pour compatibilite SGBS/BTR */
#define NP_AVAILABLE_BALANCE		"19"
#define NP_LOAN_BALANCE			"20"
#define NP_TARGET_ACCOUNT		"21"
#define NP_DEPOSIT_NUMBER		"22"
#define NP_CUTOFF_DATA			"23"
#define NP_OPERATION_DATE_TIME		"24"
#define NP_BILLING_AMOUNT		"25"
#define NP_CONVERSION_RATE		"26"
#define NP_TRUNC_CARD_NBR		"27"
#define NP_AMOUNT_DEPOSIT		"28"

#define NP_NOTE_1_NBR			"29"
#define NP_NOTE_2_NBR			"30"
#define NP_NOTE_3_NBR			"31"
#define NP_NOTE_4_NBR			"32"

/* Debut Modif FB 28-02-00 BKME update */
#define	NP_LOAN_NBR_1			"33"
#define	NP_LOAN_AMT_1			"34"
#define	NP_LOAN_START_1			"35"
#define	NP_LOAN_EXPIRY_1		"36"
#define	NP_AMT_NEXT_INSTL_1		"37"
#define	NP_DATE_NEXT_INSTL_1		"38"
#define	NP_AMT_LAST_PAID_1		"39"
#define	NP_DATE_LAST_PAID_1		"40"
#define	NP_OUTSTD_INSTL_1		"41"
#define	NP_OUTSTD_BAL_1			"42"
#define	NP_PAST_DUE_INSTL_1		"43"
#define	NP_PAST_DUE_AMT_1		"44"
#define	NP_PAST_ACCR_INT_1		"45"
#define	NP_LOAN_NBR_2			"46"
#define	NP_LOAN_AMT_2			"47"
#define	NP_LOAN_START_2			"48"
#define	NP_LOAN_EXPIRY_2		"49"
#define	NP_AMT_NEXT_INSTL_2		"50"
#define	NP_DATE_NEXT_INSTL_2		"51"
#define	NP_AMT_LAST_PAID_2		"52"
#define	NP_DATE_LAST_PAID_2		"53"
#define	NP_OUTSTD_INSTL_2		"54"
#define	NP_OUTSTD_BAL_2			"55"
#define	NP_PAST_DUE_INSTL_2		"56"
#define	NP_PAST_DUE_AMT_2		"57"
#define	NP_PAST_ACCR_INT_2		"58"
#define	NP_LOAN_NBR_3			"59"
#define	NP_LOAN_AMT_3			"60"
#define	NP_LOAN_START_3			"61"
#define	NP_LOAN_EXPIRY_3		"62"
#define	NP_AMT_NEXT_INSTL_3		"63"
#define	NP_DATE_NEXT_INSTL_3		"64"
#define	NP_AMT_LAST_PAID_3		"65"
#define	NP_DATE_LAST_PAID_3		"66"
#define	NP_OUTSTD_INSTL_3		"67"
#define	NP_OUTSTD_BAL_3			"68"
#define	NP_PAST_DUE_INSTL_3		"69"
#define	NP_PAST_DUE_AMT_3		"70"
#define	NP_PAST_ACCR_INT_3		"71"
#define	NP_LOAN_NBR_4			"72"
#define	NP_LOAN_AMT_4			"73"
#define	NP_LOAN_START_4			"74"
#define	NP_LOAN_EXPIRY_4		"75"
#define	NP_AMT_NEXT_INSTL_4		"76"
#define	NP_DATE_NEXT_INSTL_4		"77"
#define	NP_AMT_LAST_PAID_4		"78"
#define	NP_DATE_LAST_PAID_4		"79"
#define	NP_OUTSTD_INSTL_4		"80"
#define	NP_OUTSTD_BAL_4			"81"
#define	NP_PAST_DUE_INSTL_4		"82"
#define	NP_PAST_DUE_AMT_4		"83"
#define	NP_PAST_ACCR_INT_4		"84"
#define	NP_ATM_NAME			"85"
#define	NP_RESPONSE_LABEL		"86"
#define	NP_MESSAGE			"87"
#define NP_TRANS_SEQ			"88"
#define NP_TRANS_NAME			"89"
#define NP_COMPANY                              "91"
#define NP_BILL_PAYMENT                 "92"

/*BEGIN AHM 2007/02/26*/
#define NP_SIMSIM_REF                  "93"
#define NP_SIMSIM_CREDIT_BALANCE               "94"
#define NP_SIMSIM_VALID_UNTIL                  "95"
#define NP_SIMSIM_EXPIRY_DATE                  "96"
/*END AHM*/

#define NP_OPERATION_DATE_LEN		8
#define NP_BALANCE_DATE_LEN		8
#define NP_OPERATION_HOUR_LEN		8
#define NP_CARD_NBR_LEN			22
#define NP_SOURCE_ACCOUNT_LEN		20
#define NP_ATM_NUMBER_LEN		15
#define NP_OPERATION_NUMBER_LEN		6
#define NP_BALANCE_LEN			20
#define NP_STATEMENT_LEN		0
#define NP_TEXT_LEN			0
#define NP_RESPONSE_CODE_LEN		3
#define NP_KNET_RESP_CODE_LEN                3
#define NP_AMOUNT_LEN			20
#define NP_RATE_LEN			8
#define NP_FACILITY_AMOUNT_LEN		20
#define NP_PAST_DUE_AMOUNT_LEN		20
#define NP_PENDING_AMOUNT_LEN		20
#define NP_UNUTILISED_AMOUNT_LEN	20
#define NP_CURRENT_BALANCE_LEN		20
#define NP_AVAILABLE_BALANCE_LEN	20
#define NP_LOAN_BALANCE_LEN		20
#define NP_TARGET_ACCOUNT_LEN		20
#define NP_DEPOSIT_NUMBER_LEN		3
#define NP_CUTOFF_DATA_LEN		0
#define NP_OPERATION_DATE_TIME_LEN	18
#define NP_DEPOSIT_AMOUNT		20
#define NP_NOTE_1_NBR_LEN		3
#define NP_NOTE_2_NBR_LEN		3
#define NP_NOTE_3_NBR_LEN		3
#define NP_NOTE_4_NBR_LEN		3
/* Debut Modif FB 28-02-00 */

#define	NP_LOAN_NBR_LEN			7
#define	NP_LOAN_AMT_LEN			18
#define	NP_LOAN_START_LEN		8
#define	NP_LOAN_EXPIRY_LEN		8
#define	NP_AMT_NEXT_INSTL_LEN		18
#define	NP_DATE_NEXT_INSTL_LEN		8
#define	NP_AMT_LAST_PAID_LEN		18
#define	NP_DATE_LAST_PAID_LEN		8
#define	NP_OUTSTD_INSTL_LEN		4
#define	NP_OUTSTD_BAL_LEN		18
#define	NP_PAST_DUE_INSTL_LEN		3
#define	NP_PAST_DUE_AMT_LEN		18
#define	NP_PAST_ACCR_INT_LEN		18
#define	NP_ATM_NAME_LEN			30
#define	NP_RESPONSE_LABEL_LEN		40
#define	NP_MESSAGE_LEN			3
#define NP_EMV_APPL_REF_LEN             10
#define NP_EMV_ARQC_LEN                 10
#define	NP_TRANS_SEQ_LEN		6
#define	NP_TRANS_NAME_LEN		26

/* Debut Modif FB 28-02-00 pour compatibilite SGBS/BTR */
#define NP_ACCOUNT_BALANCE_LEN		18
/* FIN   Modif FB 28-02-00 pour compatibilite SGBS/BTR */
/* - Position des champs loan dans RECORD DATA ------------------------- */
#define	LN_ACC_TYPE_LEN			2
#define	LN_ACC_CUR_LEN			3
#define	LN_ACC_NBR_LEN			24
#define	LN_LOAN_NBR_LEN			7
#define	LN_LOAN_AMT_LEN			15
#define	LN_LOAN_START_LEN		6
#define	LN_LOAN_EXPIRY_LEN		6
#define	LN_AMT_NEXT_INSTL_LEN		15
#define	LN_DATE_NEXT_INSTL_LEN		6
#define	LN_AMT_LAST_PAID_LEN		15
#define	LN_DATE_LAST_PAID_LEN		6
#define	LN_OUTSTD_INSTL_LEN		4
#define	LN_OUTSTD_BAL_LEN		15
#define	LN_PAST_DUE_INSTL_LEN		3
#define	LN_PAST_DUE_AMT_LEN		15
#define	LN_PAST_ACCR_INT_LEN		15

/*BEGIN AHM 2007/02/26*/
#define NP_SIMSIM_REF_LEN               12
#define NP_SIMSIM_CREDIT_BALANCE_LEN    12
#define NP_SIMSIM_VALID_UNTIL_LEN       6
#define NP_SIMSIM_EXPIRY_DATE_LEN       6
/*END AHM*/

#define	NP_ACC_TYPE_OFF		(0)
#define	NP_ACC_CUR_OFF		(NP_ACC_TYPE_OFF+LN_ACC_TYPE_LEN)
#define	NP_ACC_NBR_OFF		(NP_ACC_CUR_OFF+LN_ACC_CUR_LEN)
#define	NP_LOAN_NBR_OFF		(NP_ACC_NBR_OFF+LN_ACC_NBR_LEN)
#define	NP_LOAN_AMT_OFF		(NP_LOAN_NBR_OFF+LN_LOAN_NBR_LEN)
#define	NP_LOAN_START_OFF	(NP_LOAN_AMT_OFF+LN_LOAN_AMT_LEN)
#define	NP_LOAN_EXPIRY_OFF	(NP_LOAN_START_OFF+LN_LOAN_START_LEN)
#define	NP_AMT_NEXT_INSTL_OFF	(NP_LOAN_EXPIRY_OFF+LN_LOAN_EXPIRY_LEN)
#define	NP_DATE_NEXT_INSTL_OFF	(NP_AMT_NEXT_INSTL_OFF+LN_AMT_NEXT_INSTL_LEN)
#define	NP_AMT_LAST_PAID_OFF	(NP_DATE_NEXT_INSTL_OFF+LN_DATE_NEXT_INSTL_LEN)
#define	NP_DATE_LAST_PAID_OFF	(NP_AMT_LAST_PAID_OFF+LN_AMT_LAST_PAID_LEN)
#define	NP_OUTSTD_INSTL_OFF	(NP_DATE_LAST_PAID_OFF+LN_DATE_LAST_PAID_LEN)
#define	NP_OUTSTD_BAL_OFF	(NP_OUTSTD_INSTL_OFF+LN_OUTSTD_INSTL_LEN)
#define	NP_PAST_DUE_INSTL_OFF	(NP_OUTSTD_BAL_OFF+LN_OUTSTD_BAL_LEN)
#define	NP_PAST_DUE_AMT_OFF	(NP_PAST_DUE_INSTL_OFF+LN_PAST_DUE_INSTL_LEN)
#define	NP_PAST_ACCR_INT_OFF	(NP_PAST_DUE_AMT_OFF+LN_PAST_DUE_AMT_LEN)

#define	NP_LOAN_DATA_LEN		157

/*
	Short Statement Constantes
*/
/*#define	NP_SHORT_STAT_LEN		61*/
#define	NP_SHORT_STAT_LEN		25

/*#define	NP_DATE_POS			(0)
#define	NP_LABELE_POS			(0+6)
#define	NP_AMT_POS			(0+6+20)
#define	NP_AMT_SIGN_POS			(0+6+20+15)
#define	NP_BAL_POS			(0+6+20+15+1)
#define	NP_BAL_SIGN_POS			(0+6+20+15+1+15)
*/

#define	NP_DATE_POS			(0)
#define	NP_LABELE_POS			(0+6)
#define	NP_AMT_POS			(0+6+3)
#define	NP_AMT_SIGN_POS			(0+6+3+12)
#define	NP_BAL_POS			(0+6+3+12+1)
#define	NP_BAL_SIGN_POS			(0+6+3+12+1+15)

/*#define	NP_DATE_LEN			6
#define	NP_LABEL_LEN			20
#define	NP_AMT_LEN			15
#define	NP_AMT_SIGN_LEN			1
#define	NP_BAL_LEN			15
#define	NP_BAL_SIGN_LEN			1
*/

#define	NP_DATE_LEN			6
#define	NP_LABEL_LEN			3
#define	NP_AMT_LEN			12
#define	NP_AMT_SIGN_LEN			1

#define	NP_BAL_LEN			15
#define	NP_BAL_SIGN_LEN			1

/* Fin Modif FB 28-02-00 */



#define MAXLEN			20000

typedef struct dbl_param_t
  {	int	len; 
	char	data[MAXLEN];
  } dbl_param;

/*Debut modif FB 05-09-99 */
/*Compatibilité BKME*/
#define	NP_LOAN_INQUIRY_JOURNAL				"300"
#define	NP_PIN_CHANGE_JOURNAL				"301"
#define	NP_MESSAGING_JOURNAL				"302"
#define NP_AUTHEN_JOURNAL                               "303"
/*Fin modif FB 05-09-99*/


#define NP_MOBILE_NUMBER_LEN          6
#define MSISDN_TAG                    "P17"
#define NP_MOBILE_NBR                 "93"
#define NP_AUTHOR_NBR                 "94"

#define NP_LABEL_POS                    (0+6)

/*BEGIN AHM 15/09/2005 ajout de Ticket Payment Carte de credit     */
#define NP_CR_PAYMENT_TICKET_ENG                          "321"
#define NP_CR_PAYMENT_TICKET_AR                          "371"
#define NP_CR_PAYMENT_TICKET_FR                          "421"
#define NP_CR_PAYMENT_JRN_ENG                             "322"

#define NP_CHEQUE_BOOK_TICKET_MAIL_ENG                  "205"
#define NP_CHEQUE_BOOK_TICKET_MAIL_AR                   "255"
#define NP_CHEQUE_BOOK_TICKET_MAIL_FR                   "305"
/*END AHM*/

/*BEGIN AHM 2007/05/03 Admin Card*/
#define NP_ADMIN_CARD_BALANCING				"97"
#define NP_ADMIN_CARD_BALANCING_ENG                     "900"
#define NP_ADMIN_CARD_BALANCING_JOURNAL_ENG             "950"

#define NP_EMV_APPL_REF                                 "98"
#define NP_EMV_ARQC                                     "99"
/*END AHM*/


#endif /*** HPS_DBLPARAMDEF_H ***/
