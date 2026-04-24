  /***********************************************************************
  **  nom	: event.h						**
  **  desc	: definition des evenements interceptibles par		**
  **		 PowerSEVEN						**
  **  auteur	: YO(99/07/24)						**
  **			(C) 1997-2001 : Hightech Payment Systems	**
  ***********************************************************************/
/* - Data Base Event -------------------------------------------------- */
#define	DB_FETCH_ERR			"DBEF"
#define	DB_INSERT_ERR			"DBEI"
#define	DB_UPDATE_ERR			"DBEU"
#define	DB_FATAL_ERR			"DBFE"
#define	DB_WARNING			"DBWR"

/* - Interface/Server Event ------------------------------------------- */
#define	INT_ABNORMAL_STP		"INAB"
#define	INT_DOWN			"INDW"
#define	INT_SIGN_ON			"INSN"
#define	INT_SIGN_OFF			"INSO"
#define	INT_START			"INSR"
#define	INT_STOP			"INST"
#define	INT_UP				"INUP"

/* - Terminal Event --------------------------------------------------- */
#define	TERM_ABNORMAL_CONECT		"TCCX"
#define	TERM_CASH_DISP_WARNING		"TCDW"
#define	TERM_CASH_DISP_ERR		"TCDE"
#define	TERM_CUTOFF_ADV			"TCOA"
#define	TERM_CRD_RD_WARNING		"TCRW"
#define	TERM_CRD_RD_ERR			"TCRE"
#define	TERM_ABNORMAL_DISC		"TDCX"
#define	TERM_SECUR_DEVISE_FAULT		"TDSF"
#define	TERM_FATAL_ERR			"TFAT"
#define	TERM_FILE_COLLECT		"TFCL"
#define	TERM_FILE_TRF_HOST_LST		"TFTH"
#define	TERM_FILE_TRF_PARAM		"TFTP"
#define	TERM_LOG_PRN_WARNING		"TLPW"
#define	TERM_LOG_PRN_ERR		"TLPE"
#define	TERM_OTHER_HDWR_FAULT		"TOHF"
#define	TERM_READY			"TRDY"
#define	TERM_IN_SERVICE			"TRIS"
#define	TERM_OUT_SERVICE		"TROS"
#define	TERM_SOFT_FAULT			"TSOF"
#define	TERM_TIMEOUT			"TTOD"
#define	TERM_CASH_FAULT			"TWCF"
#define	TERM_CASH_LOW			"TWCL"
#define	TERM_CONSUMER_PRN_WARNING	"TCPW"
#define	TERM_CONSUMER_PRN_ERR		"TCPE"
#define	TERM_WARNING_LEVEL_0		"TWL0"
#define	TERM_WARNING_LEVEL_1		"TWL1"
#define	TERM_WARNING_LEVEL_2		"TWL2"
#define	TERM_LCM_ERR			"TXRC"

/* - Network Event ---------------------------------------------------- */
#define	NW_ERR				"NFER"
#define	NW_UNKNOWN_CALL_REQ		"NUCR"

/* - Application Event ------------------------------------------------ */
#define	SYSTEM_ERR			"SYSE"
#define	APP_ERR_LEVEL_0			"SYU0"
#define	APP_ERR_LEVEL_1			"SYU1"
#define	APP_ERR_LEVEL_2			"SYU2"
#define	APP_ERR_LEVEL_3			"SYU3"
#define	APP_ERR_LEVEL_4			"SYU4"
#define	APP_ERR_LEVEL_5			"SYU5"


