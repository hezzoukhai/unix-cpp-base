#ifndef P7_DEFINE_____H
#define P7_DEFINE_____H
#include <p7_trace.h>

#define NODE_LIST_BAL			'B'
#define NODE_LIST_LINE			'L'


/* Action flags when receiving a duplicate authorization*/
#define RP_ACT_REPLY		'R'		/* reply with same data			*/
#define RP_ACT_DECLINE		'D'		/* decline with duplicate		*/
#define RP_ACT_FORWARD		'F'		/* forward to issuer(not applicable for standin, in this case we decline)*/
/******************************************************************************/
/* event type when finding a duplicate */
#define AST_ERROR			'E'		/* Error						*/
#define AST_NEW_AUTH		'N'		/* New authorization			*/
#define AST_DUP_AUTH		'D'		/* Duplicate authorization		*/
#define AST_INC_AUTH		'I'		/* Incremental authorization	*/
#define AST_REP_AUTH		'R'		/* Repeat authorization			*/
/******************************************************************************/
/* Channel auth processing mode		*/
#define CAPM_SINGLE_MSG		'S'
#define CAPM_DUAL_MSG		'D'
#define CAPM_BOTH			'B'


#define AUTH_TBL_IND_LEN				3
#define SAF_COUNTER_LEN					8
#define AUTH_EVENT_CODE_LEN				3
#define AUTH_SEC_VERIF_RSLT_LEN			32


#define SIZE_KEY_TDES			256
#define SIZE_KEY_DES			16
#define SIZE_DOUBLE_KEY_BLOCK			73



#define RES_INF_RES_NODE_LEN            4
#define RES_INF_RES_ID_LEN              6
#define RES_INF_RES_CD_LEN              2
#define RES_INF_RES_TP_LEN              1
#define RES_INF_RES_LIVE_LEN            1
#define RES_INF_RES_PS_LEN              3
#define RES_INF_RES_PID_LEN             8

#define RES_INFO_LEN					(RES_INF_RES_NODE_LEN	+ \
										RES_INF_RES_ID_LEN		+ \
										RES_INF_RES_CD_LEN		+ \
										RES_INF_RES_TP_LEN		+ \
										RES_INF_RES_LIVE_LEN	+ \
										RES_INF_RES_PS_LEN		+ \
										RES_INF_RES_PID_LEN )
enum {
	RES_INF_RES_NODE    =0	,
	RES_INF_RES_ID      	,
	RES_INF_RES_CD      	,
	RES_INF_RES_TP      	,
	RES_INF_RES_LIVE    	,
	RES_INF_RES_PS      	,
	RES_INF_RES_PID     
};


/* Networks	*/
#define NET_LOCAL_NETWORK		"00"
#define NET_VISA				"01"
#define NET_MASTERCARD			"02"
#define NET_EUROPAY				"03"
#define NET_AMEX				"04"
#define NET_DINERS				"05"
#define NET_JCB					"06"
#define NET_CUP					"08"
#define NET_DISCOVER                            "07"
#define NET_CB					"09"
#define NET_UAESWITCH                           "21"
#define NET_UCARD				"XX"
#define NET_LEN					2

#define DINERS_NET_CODE		"D"

#define MCI_CB_ACS_ISS_NO_CHLG					"kA"	/* Transaction successfully authenticated by the ACS without challenging the cardholder */
#define MCI_CB_ACS_ISS_CHLG						"kB"	/* Transaction successfully authenticated by the ACS after challenging the cardholder	*/
#define MCI_CB_ACS_MCI_RBA						"kC"	/* Transaction successfully authenticated by Mastercard based on risk based assessment (RBA) score */
#define MCI_CB_ACS_MCI_RFU_1					"kD"	/* Future use only�no current use cases for Stand In challenge	*/
#define MCI_CB_ATT_UNABLE_TO_PROC				"kE"	/* Transaction could not be authenticated by either the ACS or Mastercard RBA. */
#define MCI_CB_ATT_RFU_1						"kF"	/* Future use only�no current use cases for Attempts challenge */

#define MCI_CB_NOT_AUTH_ACQ_EXCEP				"kN"	/* Transaction was not authenticated by ACS or Mastercard, based on Acquirer exemption 
															from strong customer authentication (SCA) under PSD2		*/
#define MCI_CB_AUTH_RP_SS						"kO"	/* Recurring transaction and was successfully authenticated by either the ACS or Mastercard RBA */
#define MCI_CB_AUTH_FIRST_RP					"kP"	/* Recurring transaction is the first in a series and was successfully authenticated by the ACS using SCA */
/*--------------------------------------------------*/
#define MCI_CB_SHA1_ACS_ISS_NO_CHLG				"kG"
#define MCI_CB_SHA1_ACS_ISS_CHLG				"kH"
#define MCI_CB_SHA1_ACS_MCI_RBA					"kJ"
#define MCI_CB_SHA1_ACS_MCI_RFU_1				"kK"
#define MCI_CB_SHA1_ATT_UNABLE_TO_PROC			"kL"
#define MCI_CB_SHA1_ATT_RFU_1					"kM"

#define MCI_CB_LEN								2




#define MCI_3DS1_CB_ACS_AUTHENTICATED			"j"
#define MCI_3DS1_CB_ATTEMPT						"h"





#define P7_CTRL_CHECK_OK			'0'
#define P7_CTRL_CHECK_INV			'1'
#define P7_CTRL_CHECK_ERROR			'2'
#define P7_CTRL_CHECK_MALFUNCTION	'3'
#define P7_CTRL_CHECK_NOPROC		'4'

/* Transaction types */
#define TT_MICRO_PAYMENT		"0001"
#define TT_LOAN					"1001"
#define TT_INSTALLMENT			"1102"
#define TT_BONUS_PAYMENT		"2001"
#define TT_FOCE_POST			"FP00"
#define TT_SAF					"SF00"
#define TT_LEN					4


/* Ecom types */
#define ECT_NONE					"00"	/* Not secured */
#define ECT_INT_SEC_SSL				"01"	/* Secure SSL */
#define ECT_INT_SEC_OTHER			"99"	/* Secured by other means */
#define ECT_LEN						2

/* Installment indicator */
#define ISTL_IND_FEES_INCLUDED		'I'
#define ISTL_IND_PLUS_FEES			'E'

/* Installment plan types */
#define IPT_ISSUER_FIN			"01"		/* Issuer financed		*/
#define IPT_MERCAHNT_FIN		"02"		/* Merchant financed	*/
#define IPT_ACQUIRER_FIN		"03"		/* Acquirer financed	*/
#define IPT_LEN					2


/* Period types */
#define PRT_DAY					'1'
#define PRT_WEEK				'2'
#define PRT_MONTH				'3'

/* Advice type */
#define ADV_TYPE_SAF			'S'
#define ADV_TYPE_FORCE_POST		'P'



#define ACTION_FLAG_STOP_LIST	'S'
#define ACTION_FLAG_CAPTURE		'P'
#define ACTION_FLAG_CANCELED	'C'
#define ACTION_FLAG_APPROVAL	'A'


#define NODE_LIST_BO			'B'
#define NODE_LIST_BAL			'B'
#define NODE_LIST_LINE			'L'



#define PAY_TYPE_ACC_2_ACC			"AA"	/* Account to account									*/
#define PAY_TYPE_P_2_P				"PP"	/* Person to person										*/
#define PAY_TYPE_PP_INIT_LD			"PL"	/* Prepaid initial load									*/
#define PAY_TYPE_PP_RELOAD			"PR"	/* Prepaid reload										*/
#define PAY_TYPE_PP_UNLOAD			"PU"	/* Prepaid unload										*/
#define PAY_TYPE_CASH_IN			"CI"	/* Agent CashIn											*/
#define PAY_TYPE_CASH_OUT			"CO"	/* Agent CashOut										*/
#define PAY_TYPE_GAMBLING_REPAY		"GR"	/* Gambling repay										*/
#define PAY_REFUND					"RF"	/* Refund												*/
#define PAY_TYPE_MER_REB			"MR"	/* Merchant rebate										*/

#define PAY_EARNING_PAYMENT			"EP"	/* Earning payment										*/
#define PAY_PROF_SERVICES_PAYMENT_C2B	"CP"	/* Payment for goods and services(C2B)					*/
#define PAY_PROF_SERVICES_PAYMENT_B2B	"SP"	/* Payment for goods and services(B2B)					*/

#define PAY_TYPE_BILL_PAY			"BP"	/* Credit card Bill payment								*/
#define PAY_TYPE_PAYROLL			"PD"	/* Payroll/pension disbursement							*/
#define PAY_TYPE_GOV_2_C			"OC"	/* Government/Non-Government Organization Disbursement	*/
#define PAY_TYPE_C_2_GOV			"GC"	/* Consumer to government								*/
#define PAY_TYPE_ACQ_MER_SETTL		"MS"	/* Acquirer to merchant settlment						*/	/* PROD00025867*/
#define PAY_TYPE_MOBILE_TOPUP		"MT"	/* Mobile phone topup									*/
#define PAY_TYPE_UTILITY_PAYMENT	"UP"	/* Utility payment										*/
#define PAY_TYPE_OTHER_PAY			"OP"	/* Other Payment										*/

#define PAY_TYPE_BUSINESS_DIST		"BD"	/* Business Disbursement								*/	/* ??? */
#define	PAY_TYPE_TAX_REBATE			"TR"	/* Tax rebate											*/
#define PAY_TYPE_LEN				2




/*Start PROD00025867*/
#define FUNDS_SOURCE_CREDIT			"01"	/* Credit						*/
#define FUNDS_SOURCE_DEBIT			"02"	/* Debit						*/
#define FUNDS_SOURCE_PREPAID		"03"	/* Prepaid						*/
#define FUNDS_SOURCE_DEPOSIT_ACC	"04"	/* Deposit Account				*/
#define FUNDS_SOURCE_MOBILE_MONEY	"05"	/* Mobile Money Account			*/
#define FUNDS_SOURCE_CASH			"06"	/* Cash							*/
#define FUNDS_SOURCE_LEN			2
/*End PROD00025867*/

/* Eletronic commerce indicator */
#define P7_ECI_UNSPECIFIED					'0'
#define P7_ECI_AUTHENTICATED				'1'
#define P7_ECI_NON_AUTHEN_ATTEMPT			'2'
#define P7_ECI_NON_AUTHENTICATED			'3'
#define P7_ECI_NON_SECURE					'4'


/* Terminal info data */
#define TID_FORMAT_OFST                   		 0
#define TID_ACQ_BANK_CODE_OFST                   1
#define TID_BRANCH_CODE_OFST                     7
#define TID_COUNTRY_CODE_OFST                    13
#define TID_STATE_CODE_OFST                    	 16
#define TID_ZIP_CODE_OFST                        19
#define TID_CITY_NAME_OFST                       29
#define TID_MCC_OFST                             41




#define TID_ACQ_BANK_CODE_LEN               6
#define TID_BRANCH_CODE_LEN                 6
#define TID_MCC_LEN                         4
#define TID_CITY_NAME_LEN                   12
#define TID_ZIP_CODE_LEN                    10
#define TID_COUNTRY_CODE_LEN                3

/* Original billing info P08 */
#define OBAI_BILL_CRNCY_O               0
#define OBAI_BILL_AMOUNT_O              3
#define OBAI_CONV_RATE_O                21
#define OBAI_CONV_DATE_O                29
#define OBAI_REPL_BILL_AMOUNT_O         33

#define OBAI_BILL_CRNCY_L               3
#define OBAI_BILL_AMOUNT_L              18
#define OBAI_CONV_RATE_L                8
#define OBAI_CONV_DATE_L                4
#define OBAI_REPL_BILL_AMOUNT_L         18

/* Network settlement info K18 & K19 */

#define NET_SETL_CRNCY_O               0
#define NET_SETL_AMOUNT_O              3
#define NET_SETL_DATE_O         	   21
#define NET_SETL_CONV_RATE_O           29
#define NET_SETL_CONV_DATE_O           37


#define NET_SETL_CRNCY_L               3
#define NET_SETL_AMOUNT_L              18
#define NET_SETL_DATE_L         	   8
#define NET_SETL_CONV_RATE_L           8
#define NET_SETL_CONV_DATE_L           4
#define NET_SETL_INFO_L                41


#define DO_PRECONTROL				(1<<0)
#define DO_CHECK_SECURITY			(1<<1)
#define DO_AMOUNT_MNG				(1<<2)
#define DO_CHECK_LIMIT				(1<<3)
#define DO_CHECK_FALLBACK			(1<<4)
#define DO_CHECK_AVAILABLE			(1<<5)
#define DO_UPD_CARD_ACTIVITY		(1<<6)
#define DO_UPD_BALANCE				(1<<7)
#define DO_UPD_SETTLEMENT			(1<<8)
#define DO_SAF_PROCESSING			(1<<9)
#define DO_AUTH_APPROVAL			(1<<10)


#define P7_PRIV_TLV_DATA_LEN		(1024 + 1)




#define ISO_7813_T2_FMT			1
#define ANSI_X416_T2_FMT		2


#endif


