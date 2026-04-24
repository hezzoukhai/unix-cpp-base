#ifndef BASE24_DEFINE_H
#define BASE24_DEFINE_H


#define USE_SYSLOG

#if defined(USE_SYSLOG)
#	define	p7_setlogmask		setlogmask
#	define	p7_syslog			syslog
#else
#	define	p7_setlogmask	
#	define	p7_syslog		
#endif


/************************ Syslog Interface Error ******************************/
#define P7_ERR_0100			"PWC-0100: [Interface Stop]: Interface has been stopped"
#define P7_ERR_0200			"PWC-0200: [Unprocessed message]: Message has not been processed because of an internal error"
#define P7_ERR_0201			"PWC-0201: [Communication Error]: Cannot read message from line"
#define P7_ERR_0202			"PWC-0202: [Communication Error]: Cannot read message from IPC"
#define P7_ERR_0203			"PWC-0203: [Communication Error]: Cannot send message on communication line"
#define P7_ERR_0204			"PWC-0204: [Communication Error]: Cannot send message on IPC"
#define P7_ERR_0900			"PWC-0900: [Error]: Interface received an unexpected Halt signal"
/******************************************************************************/




#define FIELD_MAX_LEN               1024

#define DISCONNECT          99

#define B24_MSG_LEN_LEN			2

#define RESP_MSG_TYPE(m)	((m)&0xFFFFFFFE)+10

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
#define F_UNDEF					9			/* Field undefined in CBAE */


#define MSG_TYPE_UNDEF			0
#define MSG_TYPE_REQ			1
#define MSG_TYPE_RESP			2


#define NODE_LIST_BAL			'B'
#define NODE_LIST_LINE			'L'


#define MAX_CHANNELS			12

/** Base24 Header values **/
/* Product indicator */
#define B24_PRODUCT_IND_BASE        "00"
#define B24_PRODUCT_IND_ATM         "01"
#define B24_PRODUCT_IND_POS         "02"
#define B24_PRODUCT_IND_HOST        "08"
#define B24_PRODUCT_IND_LEN         2

#define B24_PRODUCT_IND_BASE_T      0
#define B24_PRODUCT_IND_ATM_T       1
#define B24_PRODUCT_IND_POS_T       2
#define B24_PRODUCT_IND_HOST_T      3
#define B24_PRODUCT_IND_QTY         4


/* Release */
#define B24_CURR_RELEASE            "60"
#define B24_RELEASE_LEN             2

/* Status */
#define B24_H_STATUS_DFLT               "000"   /* Default value                */
#define B24_H_STATUS_KEY_SYNC_ERROR     "196"   /* Key synchronization error    */
#define B24_H_STATUS_MAC_ERROR          "197"   /* Invalid MAC error            */
#define B24_H_STATUS_SEC_OP_FAILURE     "198"   /* Security operation failed    */
#define B24_H_STATUS_SEC_DEV_FAILURE    "199"   /* Security device failure      */
#define B24_H_STATUS_LEN                3

/* Originator and Responder code */
#define B24_H_ENT_CD_UNKNOWN            '0'     /* Undetermined                     */
#define B24_H_ENT_CD_PROC_CTRL_DEV      '1'     /* Device controlled by a process   */
#define B24_H_ENT_CD_PROC_HNDL_DEV      '2'     /* Device handler process           */
#define B24_H_ENT_CD_AUTH_PROCESS       '3'     /* Authorization process            */
#define B24_H_ENT_CD_HOST_INT           '4'     /* Host interface process           */
#define B24_H_ENT_CD_HOST               '5'     /* Host                             */
#define B24_H_ENT_CD_INTER_INT          '6'     /* Interchange interface process or remote banking standard unit interface process  */
#define B24_H_ENT_CD_INTERCHANGE        '7'     /* Interchange or remote banking endpoint device    */
#define B24_H_ENT_CD_HOST_MAINT         '8'     /* From host maintenance process    */
#define B24_H_ENT_CD_BILLPAY_SRV        '9'     /* Billpay server process           */

#if defined(B24_NCCC)
#	define B24_H_ENT_CD_DFLT_ORIGINATOR		B24_H_ENT_CD_HOST
#	define B24_H_ENT_CD_DFLT_RESPONDER		B24_H_ENT_CD_INTERCHANGE
#else
#	define B24_H_ENT_CD_DFLT_ORIGINATOR		B24_H_ENT_CD_HOST
#	define B24_H_ENT_CD_DFLT_RESPONDER		B24_H_ENT_CD_HOST
#endif


#define B24_F002_01_PURCHASE			"00"		/* Normal Purchase						*/
#define B24_F002_01_WITHDRAWAL			"01"		/* Cash withdrawal / cash advance		*/
#define B24_F002_01_ADJ_DEBIT			"02"		/* Adjustment Debit						*/
#define B24_F002_01_PURCHASE_CB			"09"		/* Debit Card Purchase with Cash Back	*/
#define B24_F002_01_REFUND				"20"		/* Returns								*/
#define B24_F002_01_DEPOSIT				"21"		/* Deposit								*/
#define B24_F002_01_BAL_INQ				"31"		/* Balance inquiry						*/
#define B24_F002_01_CH_ACC_TF			"40"		/* Cardholder account transfer			*/
#define B24_F002_01_MOTO				"80"		/* Mail Order/Telephone Order			*/
#define B24_F002_01_CARD_VERIF			"81"		/* Card Verification					*/
#define B24_F002_01_LEN					2



/* Network Management Information Code */
#define B24_F070_LOGON                  "001"   /* Log-on               */
#define B24_F070_LOGOFF                 "002"   /* Log-off              */
#define B24_F070_CHG_KEY                "161"   /* Change Key           */
#define B24_F070_NEW_KEY                "162"   /* New Key              */
#define B24_F070_REPEAT_KEY             "163"   /* Repeat Key           */
#define B24_F070_VERIFY_KEY             "164"   /* Verify Key           */
#define B24_F070_ECHO_TEST              "301"   /* Echo-test            */
#define B24_F070_CUTOVER				"201"	/* Cutover				*/
#define B24_F070_LEN                    3



#define BASE24_F003_LEN					6
#define BASE24_F012_LEN					6
#define BASE24_F013_LEN					4
#define BASE24_F022_LEN					3
#define BASE24_F025_LEN					2
#define BASE24_F026_LEN					2
#define BASE24_F043_LEN					40

#define B24_F022_01_UNSPECIFIED			"00"		/* Unspecified		*/
#define B24_F022_01_MANUALLY			"01"		/* Manually			*/
#define B24_F022_01_MAG_STRIP			"02"		/* Magnetic stripe	*/
#define B24_F022_01_EMV					"05"		/* EMV				*/
#define B24_F022_01_EMV_FALLBACK		"80"		/* EMV Fallback		*/
#define B24_F022_01_LEN					2
/*START NAB08102020 PLUTONL-2288 */
#if defined(B24_PROSA)
#define B24_F022_01_CONTACTLESS			"07"		/* Contactless, follow CHIP message format				*/
#define B24_F022_01_MC_JCB_ECOM			"81"		/* MasterCard EC or JCB EC transaction					*/
#define B24_F022_01_MAG_COMPLETE		"90"		/* Megnatic strip read, complete data transmission		*/
#define B24_F022_01_CONTACTLESS_FLW_MAG	"91"		/* Contactless, follow Megnatic strip message format	*/
#endif
/*END NAB08102020 PLUTONL-2288*/

#define B24_F022_02_UNKNOWN				'0'
#define B24_F022_02_PIN_ENTRY_CAP		'1'			/* PIN entry capability		*/
#define B24_F022_02_NO_PIN_ENTRY_CAP	'2'			/* No PIN entry capability	*/

/******************************************************************************/
#define B24_F025_NORMAL_PRESENTMENT		"00"		/* Normal presentment						*/
#define B24_F025_CUSTOMER_NOT_PRESENT	"01"		/* Customer not present						*/
#define B24_F025_MOTO					"08"		/* Mail/telephone order						*/
#define B24_F025_AVS					"51"		/* Address Verification Service only		*/
#define B24_F025_E_COM					"59"		/* NCCC: VISA/U card EC Transaction
														Evertec: EC Transaction
													*/
#define B24_F025_PRE_AUTH				"06"		/* Pre-authorization & Pre-authorization completion	*/
#define B24_F025_LEN					2
/*START NAB08102020 PLUTONL-2288 */
#if defined(B24_PROSA)

#	define B24_F025_UN_TERMINAL			"02"			/* Unattended terminal, unable to retain card	*/




/*#	define B24_F025_MCI_EC					"15"	*/	/* MasterCard EC Transaction					*/

#endif
/*END NAB08102020 PLUTONL-2288*/
/******************************************************************************/


#define B24_F091_ADD				'1'
#define B24_F091_UPD				'2'
#define B24_F091_DEL				'3'
#define B24_F091_INQ				'5'
#define B24_F091_INC                            '9'

/******************************************************************************/

#define ST_CHANNEL_OFFLINE		'D'
#define ST_CHANNEL_CONNECT		'C'
#define ST_CHANNEL_SIGN_ON		'O'




#define FU_FN_B24_NEG_CARD_FILE				"B24_NEG_CARD_FILE"
#define FU_FN_B24_CARD_AUTH_FILE			"B24_CARD_AUTH_FILE"
#define FU_FN_VISA_EXCEPTION_FILE			"VISA_EXCEPTION_FILE"
#define FU_FN_VISA_FULL_AUTHO_FILE			"VISA_FULL_AUTHO_FILE"
#define FU_FN_VISA_RISK_LEVEL_FILE			"VISA_RISK_LEVEL_FILE"
#define FU_FN_VISA_PIN_VERIF_FILE			"VISA_PIN_VERIF_FILE"
#define FU_FN_MC_STANDIN_ACC_FILE			"MC_STANDIN_ACC_FILE"
#define FU_FN_MC_WARNING_BULLETIN_FILE		"MC_WARNING_BULLETIN_FILE"
#define FU_FN_JCB_EXCEPTION_FILE			"JCB_EXCEPTION_FILE"


#define FU_STATUS_SENT                      'S'
#define FU_STATUS_PENDING                   'X'
#define FU_STATUS_DISCARDED                 'D'
#define FU_STATUS_PROCESSED                 'P'



#define CARD_TYPE_JCB			"J"		/*JCB											*/
#define CARD_TYPE_UCARD			"NC"	/* UCARD										*/
#define CARD_TYPE_ADM			"AD"	/* Administrative (BASE24-atm only)				*/
#define CARD_TYPE_AMEX			"AX"	/* American Express credit						*/
#define CARD_TYPE_BD			"BD"	/* Business deposit (BASE24-atm and BASE24-teller only)		*/
#define CARD_TYPE_PRIVATE		"C*"	/* Private label credit (includes C, C0-C9, CA, and CC-CZ)	*/
#define CARD_TYPE_CB			"CB"	/* Carte Blanche credit							*/
#define CARD_TYPE_DEMO			"D "	/* Demonstration (BASE24-atm only)				*/
#define CARD_TYPE_DINERS		"DC"	/* Diners Club credit							*/
#define CARD_TYPE_DISCOVER		"DS"	/* Discover (Sears) credit						*/
#define CARD_TYPE_MCC			"M "	/* MasterCard credit							*/
#define CARD_TYPE_MCD			"MD"	/* MasterCard debit (See BASE24-pos note below)	*/
#define CARD_TYPE_MC_DUAL		"MM"	/* MasterCard dual (See BASE24-pos note below)	*/
#define CARD_TYPE_PROP			"P*"	/* Proprietary debit (includes P, P0-P9, and PA-PZ)			*/
#define CARD_TYPE_SPEC			"SC"	/* Special, Check (BASE24-pos only)							*/
#define CARD_TYPE_SP			"SP"	/* Special purpose (BASE24-atm Self-Service Banking Check Application only)	*/
#define CARD_TYPE_SUPER_TELLER	"ST"	/* Super teller (BASE24-atm Self-Service Banking Base Application only)		*/
#define CARD_TYPE_VISA			"V "	/* VISA credit									*/
#define CARD_TYPE_VISA_DEBIT	"VD"	/* VISA debit (See BASE24-pos note below)		*/
#define CARD_TYPE_VISA_DUAL		"VV"	/* VISA dual (See BASE24-pos note below)		*/



#define NEG_REASON_CD_ACC_OPEN				"00"	/* Account open				*/
#define NEG_REASON_CD_LOST_CARD				"01"	/* Lost card				*/
#define NEG_REASON_CD_STOLEN_CARD			"02"	/* Stolen card				*/
#define NEG_REASON_CD_REFERRAL				"03"	/* Referral					*/
#define NEG_REASON_CD_MAYBE					"04"	/* Maybe					*/
#define NEG_REASON_CD_DENIAL				"05"	/* Denial					*/
#define NEG_REASON_CD_SIG_RESTRICED			"06"	/* Signature restricted		*/
#define NEG_REASON_CD_COUNTRY_CLUB			"07"	/* Country club				*/
#define NEG_REASON_CD_ACC_EXPIRED			"08"	/* Account expired			*/
#define NEG_REASON_CD_COMMERCIAL			"09"	/* Commercial				*/
#define NEG_REASON_CD_VIP					"10"	/* VIP						*/
#define NEG_REASON_CD_ACC_CLOSED			"11"	/* Account closed			*/


/*START NAB08102020*/
#define B24_ARQC_VERIF_PASSED          '4'
#define B24_ARQC_VERIF_FAILED          '3'
#define B24_ARQC_NOT_VERIFIED          '0'
/*END  NAB08102020*/
#endif






