  /***********************************************************************
  **  nom	: ndcstatus.h						**
  **  desc	: traitement des status Ndc				**
  **  auteur	: YO990817						**
  **			(C) 1997-2001 : Hightech Payment Systems	**
  ***********************************************************************/

#ifndef HPS_NDCSTATUS_H
#define HPS_NDCSTATUS_H

/* - Hps Device Status -------------------------------------------------- */
#define	STATUS_OK				0
#define	STATUS_WARNING			1
#define	STATUS_NOK				2
#define	STATUS_ERR				3
#define	STATUS_FATAL			4

/* - Status Descriptor ------------------------------------------------- */
#define	SD_DEVICE_FAULT			'8'
#define	SD_READY				'9'
#define	SD_CMD_REJ				'A'
#define	SD_TRN_REP_SUCCES		'B'
#define	SD_SPEC_CMD_REJ			'C'
#define	SD_TERM_STATE			'F'

/* - Device Identifier -------------------------------------------------- */
#define	DI_CLOCK				'A'
#define	DI_POWER_FAILURE		'B'
#define	DI_CARD					'D'
#define	DI_CASH_HANDLER			'E'
#define	DI_DEPOSITORY			'F'
#define	DI_RECEIPT_PRN			'G'
#define	DI_JRN_PRN				'H'
#define	DI_NIGHT_SAFE_DPT		'K'
#define	DI_ENCRYPTOR			'L'
#define	DI_CAMERA				'M'
#define	DI_DOOR_ACCES			'N'
#define	DI_SENSORS				'P'
#define	DI_TOUCH_SCR_KEYBORD	'Q'
#define	DI_SUPER_KEYS			'R'
#define	DI_STAT_PRN				'V'
#define	DI_COIN_DISPENSER		'Y'
#define	DI_ENVELOPE_DISPENSER	'\\'
#define	DI_DOC_PROCESS			'|'
#define DI_BN_NOTE_DPT          'w'  /**SKA290408**/
#define DI_CHEQUE_DPT           'q'  /**SKA290508**/
#define DI_BNA_WN_NOTE_DPT      'r' /** TBO04012011 WN r Cash Deposit */

#define	DI_WNEMV				'e'
#define	DI_NCREMV				'c'

/* - Status Information Fields Position --------------------------------- */
#define SI_DEVICE_ID_POS			0
#define	SI_TRANS_STATUS_POS			1
#define	SI_DEVICE_STATUS_POS		1
#define	SI_ERR_SEVERITY_POS			2
#define	SI_DIAG_STATUS_POS			3
#define	SI_SUPPLIES_STATUS_POS		4

/* - Error Severity(Fitness) ------------------------------------------------------ */
#define	ES_NO_ERR				"0"
#define	ES_ROUTING_ERR			"1"		/* ROUTINE */
#define	ES_WARNING_COND			"2"
#define	ES_SUSPEND				"3"
#define	ES_FATAL				"4"

/* Device fault Error Severity */
/*
Error Severity. Contains information required to decide whether to 
shut down or continue to use the SST. Each character is always coded 
in the same way
*/
#define	NDC_DEV_ES_NO_ERR				'0'		/* No Error. Continue to use. no error has occurred. Diagnostic information follows.			*/
#define	NDC_DEV_ES_ROUTINE_ERR			'1'		/* Routine. Continue to use. a routine error has occurred and diagnostic information follows	*/

#define	NDC_DEV_ES_WARNING_COND			'2'		/* Warning. Continue to use. but it is recommended that the network operator be informed of the 
													error or exception condition. Diagnostic information follows.*/

#define	NDC_DEV_ES_SUSPEND				'3'		/* Suspend. This code indicates that the terminal will suspend transaction processing on 
													completion of the current transaction (state 000 entered). This is sent if cardholder tampering 
													is suspected. If no action is taken by Central, the terminal will attempt to go back 
													in service in five minutes. Diagnostic information follows.*/

#define	NDC_DEV_ES_FATAL				'4'		/* Fatal. This code indicates that the device is out of service and will continue to report 
													fatal error until operator intervention at the terminal. It is recommended that the terminal 
													is put out of service or that transactions involving the faulty device are not allowed.*/

/* Supplies status */
#define NDC_DEV_ST_NOT_CONFIG		'0'
#define NDC_DEV_ST_GOOD				'1'
#define NDC_DEV_ST_MEDIA_LOW		'2'
#define NDC_DEV_ST_MEDIA_OUT		'3'
#define NDC_DEV_ST_OVERFILL			'4'



/* - Error Severity ------------------------------------------------------ */
#define	CHIP_COM_ERROR			"0"
#define	EMV_LOG_ERROR			"1"
#define	EMV_DATA_ERROR			"2"

/******************************************************************************/
/* Device Status Information: Card Reader/Writer: Transaction/Device Status	*/
#define DV_SI_D_TS_NO_EXCP				'0'		/* U	*/
#define DV_SI_D_TS_CRD_NOT_TAKEN		'1'		/* S/U: The CH did not take his card within the allowed time and it was captured or jammed	*/
#define DV_SI_D_TS_EJECT_FAILURE		'2'		/* S/U: The mechanism failed to eject the card, which was either captured or jammed			*/
#define DV_SI_D_TS_UPD_FAILURE			'3'		/* S/U	*/
#define DV_SI_D_TS_INV_TRACK_DATA		'4'		/* S/U	*/
#define DV_SI_D_TS_ERR_TRACK_DATA		'7'		/* U	*/

/* Device Status Information: Card Reader/Writer: Supplies Status	*/
#define DV_SI_D_SS_NO_NEW_STATE			'0'
#define DV_SI_D_SS_NO_OVERFILL			'1'		/* No overfill condition (capture bin)	*/
#define DV_SI_D_SS_OVERFILL				'4'		/* Overfill condition (capture bin)		*/

/******************************************************************************/
/* Device Status Information: Cash Handler: Transaction/Device Status	*/
#define DV_SI_E_TS_SUCCESS					'0'		/* U	*/
#define DV_SI_E_TS_SHORT_DISPENSE			'1'		/* S	*/
#define DV_SI_E_TS_NO_DISPENSE				'2'		/* S	*/
#define DV_SI_E_TS_UNKOWN_DISPENSE			'3'		/* S	*/
#define DV_SI_E_TS_NO_DISPENSE_CARD_NOT_EJ	'4'		/* S	*/
#define DV_SI_E_TS_NOTES_RETRACTED			'5'		/* S/U	*/

/* Device Status Information: Cash Handler: Supplies Status	*/
#define DV_SI_E_SS_NO_NEW_STATE			'0'
#define DV_SI_E_SS_SUFFICIENT_NOTES		'1'
#define DV_SI_E_SS_NOTES_LOW			'2'
#define DV_SI_E_SS_OUT_OF_NOTES			'3'
/* Device Status Information: Cash Handler: Supplies Status(Character 0)	*/
#define DV_SI_E_SS_0_NO_NEW_STATE			'0'
#define DV_SI_E_SS_0_NO_OVERFILL			'1'
#define DV_SI_E_SS_0_OVERFILL				'4'
/******************************************************************************/
/* Device Status Information: Receipt Printer: Transaction/Device Status	*/
#define DV_SI_G_TS_PRINT_SUCCESS			'0'		/* U	*/
#define DV_SI_G_TS_PRINT_NOT_SUCCESS		'1'		/* S/U	*/
#define DV_SI_G_TS_DEV_NOT_CONFIG			'2'		/* U	*/
#define DV_SI_G_TS_CANCEL_KEY_PRESSED		'4'		/* U	*/
#define DV_SI_G_TS_RECEIPT_RETRACTED		'5'		/* U	*/


/* Device Status Information: Receipt Printer: Supplies Status	*/
#define DV_SI_G_SS_0_SUFFICIENT_PAPER		'1'
#define DV_SI_G_SS_0_PAPER_LOW				'2'
#define DV_SI_G_SS_0_PAPER_EXHAUSTED		'3'

#define DV_SI_G_SS_1_RIBBON_OK				'1'
#define DV_SI_G_SS_1_RIBBON_REPL_WRN		'2'
#define DV_SI_G_SS_1_RIBBON_REPL_FATAL		'3'

#define DV_SI_G_SS_2_PRINT_HEAD_OK			'1'
#define DV_SI_G_SS_2_PRINT_HEAD_REPL_WRN	'2'
#define DV_SI_G_SS_2_PRINT_HEAD_REPL_FATAL	'3'

#define DV_SI_G_SS_3_KNIFE_OK				'1'
#define DV_SI_G_SS_3_KNIFE_REPL_WRN			'2'

#define DV_SI_G_SS_4_CAPTURE_BIN_OK				'1'
#define DV_SI_G_SS_4_CAPTURE_BIN_OVERFILL		'2'


/******************************************************************************/
/* Device Status Information: Journal Printer: Transaction/Device Status	*/
#define DV_SI_H_TS_PRINT_SUCCESS				'0'		/* U	*/
#define DV_SI_H_TS_PRINT_NOT_SUCCESS			'1'		/* U	*/
#define DV_SI_H_TS_DEV_NOT_CONFIG				'2'		/* U	*/
#define DV_SI_H_TS_JP_BKP_ACTIVATED				'6'		/* U	*/
#define DV_SI_H_TS_JP_BKP_N_REPRINT_FINISH		'7'		/* U	*/
#define DV_SI_H_TS_JP_BKP_N_REPRINT_START		'8'		/* U	*/
#define DV_SI_H_TS_JP_BKP_HALTED				'9'		/* U	*/
#define DV_SI_H_TS_JP_BKP_LOG_SEC_ERR			':'		/* U	*/
#define DV_SI_H_TS_JP_BKP_REPRINT_HALTED		';'		/* U	*/
#define DV_SI_H_TS_JP_BKP_TAMPER_ST_ENTER		'<'		/* U	*/

/* Device Status Information: Journal Printer: Supplies Status	*/
#define DV_SI_H_SS_0_SUFFICIENT_PAPER		'1'
#define DV_SI_H_SS_0_PAPER_LOW				'2'
#define DV_SI_H_SS_0_PAPER_EXHAUSTED		'3'

#define DV_SI_H_SS_1_RIBBON_OK				'1'
#define DV_SI_H_SS_1_RIBBON_REPL_WRN		'2'
#define DV_SI_H_SS_1_RIBBON_REPL_FATAL		'3'

#define DV_SI_H_SS_2_PRINT_HEAD_OK			'1'
#define DV_SI_H_SS_2_PRINT_HEAD_REPL_WRN	'2'
#define DV_SI_H_SS_2_PRINT_HEAD_REPL_FATAL	'3'

#define DV_SI_H_SS_3_KNIFE_OK				'1'

/******************************************************************************/
/* Device Status Information: Encryptor: Transaction/Device Status	*/

#define DV_SI_L_TS_ENCRYPTOR_ERROR			'1'		/* U	*/
#define DV_SI_L_TS_NOT_CONFIG				'2'		/* U	*/
/******************************************************************************/


/* Terminal State - Status Information Sub-Field ‘g1’*/
#define TSSI_G1_SND_CONFIG_INFO				'1'		/* received on cmd SEND_CONFIG_INFORMATION with no subcmd or sub cmd 6 */
#define TSSI_G1_SND_SUPPLY_COUNTERS			'2'		/* received on cmd SEND_SUPPLY_COUNTER				*/
#define TSSI_G1_SND_DT_TIME					'5'		/* received on cmd SEND_DATE_TIME					*/
#define TSSI_G1_SND_CONFIG_ID				'6'		/* received on cmd SEND_CONFIGURATION_IDENTIFER		*/
#define TSSI_G1_HW_CONFIG_DATA				'H'		/* received on cmd SEND_CONFIG_INFORMATION with subcmd SEND_HW_CONFIG_ONLY */
#define TSSI_G1_SND_SUPPLY_DATA				'I'		/* received on cmd SEND_CONFIG_INFORMATION with subcmd SEND_SUPPLIES_DATA */
#define TSSI_G1_SND_FITNESS_DATA			'J'		/* received on cmd SEND_CONFIG_INFORMATION with subcmd SEND_FITNESS_DATA */
#define TSSI_G1_SND_TAMPER_SENSOR_DATA		'K'		/* received on cmd SEND_CONFIG_INFORMATION with subcmd SEND_TAMPER_SENSOR_ONLY */
#define TSSI_G1_SND_SW_ID					'L'		/* received on cmd SEND_CONFIG_INFORMATION with subcmd SEND_SOFT_ID_ONLY */
#define TSSI_G1_SND_LOC_CONFIG_OPT			'M'		/* received on cmd SEND_CONFIG_INFORMATION with subcmd SEND_LOC_CONFIG_OPT */
#define TSSI_G1_SND_CASH_DEP_DEF			'N'		/* received on cmd SEND_CONFIG_INFORMATION with subcmd SEND_BNA_NOTE_DEF_DATA */




/* - Command Reject Structure ------------------------------------------ */
typedef struct SCmdRejT{
    char	*fValue;
    char	*fQualifier;
    char	*fLabel;
} SCmdRej;

/* - Specific Command Reject Structure --------------------------------- */
typedef struct SSpecCmdRejT{
    char	*fValue;
    char	*fQualifier;
    char	*fLabel;
} SSpecCmdRej;

/* - Status information --------------------------------------------------- */
typedef struct SStatusInfoT{
    char	fStatusDesc	[  2];
    char	fDeviceId	[  2];
    char	fTransStatus	[256];
    char	fErrSeverity	[256];
    char	fDiagStatus	[256];
    char	fSuppliesStatus	[512];
    char	fStatusInfo	[512];
} SStatusInfo;




#endif /*** HPS_NDCSTATUS_H ****/
