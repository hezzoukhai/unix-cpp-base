#ifndef HPS_COMMAND_H
#define HPS_COMMAND_H

/* Definition des commandes au niveau messages NDC */
/* Message Class                                   */
/* -------------                                   */

#define 	TERMINAL_COMMAND						'1'
#define 	DATA_COMMAND							'3'
#define 	TRANSACTION_REPLY						'4'

#define 	UNSOLICITED_MESSAGE						'1'
#define 	SOLICITED_MESSAGE						'2'


/* Message SubClass                                */
/* ----------------                                */

#define 	CUSTOMISATION_DATA						'1'
#define 	INTERACTIVE_TRANSACTION_RESPONSE		'2'
#define 	ENCRYPTION_KEY_INFO						'3'
#define     EXT_ENCRYPTION_KEY_INFO                 '4'


#define 	TRANSACTION_REQUEST                     '1'
#define 	STATUS_MESSAGE                          '2'


#define     FINANCIAL_REQUEST                       'I'
#define 	SOLDE_REQUEST                           'H'
#define 	CHEQUE_REQUEST                          'F'
#define 	STATEMENT_REQUEST                       'G'
#define 	TRANSFERT_REQUEST                       'A'


/* Message Identifier                              */
/* ------------------                              */

#define 	SCREEN_DATA_LOAD  						'1'
#define 	STATE_TABLE_LOAD  						'2'
#define 	CONFIG_PARAMETER_LOAD 					'3'
#define 	DIEBOLD_PIN_INFORMATION_LOAD			'4'
#define 	FIT_DATA_LOAD        					'5'
#define 	CONFIG_IDENTIFIER_LOAD 					'6'
#define 	ENHANCED_CONFIGURATION_LOAD    			'A'
#define 	MAC_SELECTION_LOAD						'B'
#define 	DATE_AND_TIME_LOAD						'C'
#define 	INIT_EKC                				'F'


/* Definition des ordres donnes au terminal         */
/* Dans un message Terminal Command                 */

#define		START_UP								'1'
#define		OUT_OF_SERVICE							'2'
#define		SEND_CONFIGURATION_IDENTIFER 			'3'
#define		SEND_SUPPLY_COUNTER						'4'
#define		SEND_TALLY_INFORMATION					'5'	/*Not supported */
#define		SEND_ERROR_LOG_INFORMATION				'6'	/*Not supported */
#define		SEND_CONFIG_INFORMATION					'7'
#define		SEND_DATE_TIME							'8'
#define		OPEN_VANDAL_DOOR						'9'
#define		CLOSE_VANDAL_DOOR						':'

/* Command modifier on SEND_CONFIG_INFORMATION */
#define		SEND_HW_CONFIG_ONLY						'1'
#define		SEND_SUPPLIES_DATA						'2'
#define		SEND_FITNESS_DATA						'3'
#define		SEND_TAMPER_SENSOR_ONLY					'4'
#define		SEND_SOFT_ID_ONLY						'5'
#define		SEND_ENH_CONFIG_ONLY					'6'
#define		SEND_LOC_CONFIG_OPT						'7'
#define		SEND_BNA_NOTE_DEF_DATA					'8' /**MKB070509** Cash Deposit**/
#define     SEND_BNA_WN_NOTE_DATA_R                 'r' /** TBO04012011 WN r Cash Deposit */
#define     SEND_BNA_WN_NOTE_DATA_Q                 'q' /** TBO04012011 WN r Cash Deposit */



#define		TMK_UNDER_ZMK							'1'
#define		TPK_UNDER_TMK							'2'
#define		MAC_UNDER_TMK							'5'  /* MCK30012014 */




/* Function Identifiers                             */

#define		DEPOSIT_AND_PRINT						'1'
#define		DISPENSE_AND_PRINT						'2'
#define		DISPLAY_AND_PRINT       				'3' 
#define		PRINT_IMMEDIATE    						'4'
#define		NEXT_STATE_AND_PRINT					'5'
#define		SAFE_DEPOSIT_AND_PRINT					'6'
#define		EJECT_AND_DISPENSE_AND_PRINT 			'A' /* Card before Cash */
#define 	PARALLEL_DISPENSE       				'B'
#define		EJECT_DOC_FROM_ADM      				'E'
#define		OPEN_VALIDATION_PRINTER					'O'
#define		PRINT_STATEMENT_AND_WAIT    			'P'
#define		PRINT_STATEMENT_AND_NEXT_STAT			'Q'
#define		CASH_DOC_PRINT_AND_RETAIN				'R'



#define		RETURN_CARD				'0'
#define		RETAIN_CARD				'1'


#define		DO_NOT_PRINT				'0'
#define		PRINT_JOURNAL_ONLY			'1'
#define		PRINT_RECEIPT_ONLY			'2'
#define		PRINT_RECEIPT_AND_JOURNAL		'3'
#define		PRINT_PPD_ADM_OR_VALIDATION		'4'
#define		PRINT_PPD_ADM_AND_JOURNAL_OR_VALIDATION	'5'
#define		PRINT_STATEMENT_ONLY			'8'
#define		PRINT_ON_DEPOSIT_DOCUMENT		'9'


#define		DO_NOT_DISPLAY_CARDHOLDER_ENTRY		'0'
#define		DISPLAY_CARDHOLDER_ENTRY		'1'
#define		DISPLAY_X_FOR_CARDHOLDER_ENTRY		'2'

#define		DE_ACTIVATE_KEYS			'0'
#define		ACTIVATE_KEYS				'1'

/**FB 990615 pour BKME*********/
/*
#define		NO_CASH						 0	
#define		ERR_COMPTEUR				 1	not used
#define		USER_REQUEST				 2
#define		OUT_SERVICE					 9
#define		CLOCK_MALFUNCTION			 10 not used
#define		NOT_CONFIGURED				 11 not used
#define		READER_MALFUNCTION			 12 not used
#define		AUTRES						3	not used
#define		ANNULATION					5 not used
#define     ERREUR_MONTANT              6  not used
*/

#define	OS_RSN_CENTRAL_ORDER				0
#define	OS_RSN_NETWORK_DISCONNECT			1
#define	OS_RSN_FILE_TRX_IN_PROGRESS			2
#define	OS_RSN_ADVICE_FILE_TRX_IN_PROGRESS	3
#define	OS_RSN_STP_LST_TRX_IN_PROGRESS		4
#define	OS_RSN_KEY_TRX_IN_PROGRESS			5
#define	OS_RSN_CASSETTES_FAULT				6
#define	OS_RSN_NO_NOTES						7
#define	OS_RSN_SAFE_BOX_FAULT				8
#define	OS_RSN_KEYBORD_FAULT				10
#define	OS_RSN_SCREEN_FAULT					11
#define	OS_RSN_CARD_READER_FAULT			12
#define	OS_RSN_SECURITY_FAULT				13
#define	OS_RSN_PRN_FAULT					14
#define	OS_RSN_STORAGE_FAULT				15
#define	OS_RSN_PARAM_FAULT					16
#define	OS_RSN_ATM_MAINT					17
#define	OS_RSN_UNKOWN						24

#define		NO_CASH							OS_RSN_NO_NOTES
#define		USER_REQUEST					OS_RSN_ATM_MAINT
#define     OUT_SERVICE                     9 /*OS_RSN_CENTRAL_ORDER  Do not honor Withdrawal issue: PROD00041128*/	




#define		RETURN					 6
#define		CONTINUER				 7
#define		QUITTER  				 8
#define         ALL                                      9
#define         OTHER_ACC                                10
	
#define		PRINTER_CRITICAL			 "C"
#define		PRINTER_OUT_SERVICE  			 "O"
#define		PRINTER_IN_SERVICE  		 	 "I"

#define         ENCRYPTOR_OUT_SERVICE                    "O"
#define         ENCRYPTOR_IN_SERVICE                     "I"


#define		CARD_READER_IN_SERVICE			"I"
#define		CARD_READER_OUT_SERVICE			"O"
#define		SAFE_BOX_IN_SERVICE			"I"
#define		SAFE_BOX_CRITICAL			"O"
#define		SAFE_BOX_OUT_SERVICE			"O"

/**Begin MKB290408**/
/*********Cash deposit commands*********************************/
#define         CASH_DEPO_NORMAL                      "N"
#define         CHEQ_DEPO_NORMAL                      "N"
#define         CHEQ_DEPO_IN_SERVICE                  "I"
#define         CHEQ_DEPO_OUT_SERVICE                 "O"
#define         CHEQ_DEPO_CRITICAL                    "C"
#define         CASH_DEPO_IN_SERVICE                  "I"
#define         CASH_DEPO_OUT_SERVICE                 "O"
#define         CASH_DEPO_CRITICAL                    "C"
/**End MKB290408**/


#define 	INTERACTIVE_TIME_RESPONSE	        "016"

/** DDA10042016 RKL Modifiers *********************/
#define     EEKC_EPP_SERIAL						'F'
#define     EEKC_EPP_VAR_SERIAL					'V'
#define     EID_PUB_KEY_SIG						'B'
#define     EEKC_EPP_PUB_KEY					'G'
#define     EEKC_KEY_ENTRY_MODE					'J'
#define     EEKC_RSA_AKEY						'C'
#define     EEKC_COMMS_KEY						'2'
#define		EEKC_EPP_CAP						'Q'
/** DDA10042016 RKL Information Modifiers *********************/
#define	EPP_SERIAL_NUMBER_AND_SIGNATURE			'1' 
#define	VAR_EPP_SERIAL_NUMBER_AND_SIGNATURE		'E'			/*wincor only? */
#define	EPP_PUBLIC_KEY_AND_SIGNATURE			'2' 
#define	KVV_FOR_KEY								'3' 
#define	KEYS_STATUS								'4' 
#define	KEY_LOADED								'5' 
#define	KEY_ENTRY_MODE							'6' 
#define	RSA_ENCRYPTION_KVV						'7' 
#define	ATM_RANDOM_NUMBER						'9' 
#define	ENCRYPTOR_CAPABILITIES_AND_STATE		'B' 
#define	KEY_DELETED								'C'

/********** TERMINAL Key Entry Mode *************
*  '1' = Set mode to single length without XOR  *
*  '2' = Set mode to single length with XOR     *
*  '3' = Set mode to double length with XOR     *
*  '4' = Set mode to double length restricted.  **/
#define	TERMINAL_KEY_ENTRY_MODE				'4'	


#define ATM_TERM_CMD_START_ATM				"START ATM"
#define ATM_TERM_CMD_STOP_ATM				"STOP ATM"
#define ATM_TERM_CMD_RELOAD_DATA			"RELOAD DATA"
#define ATM_TERM_CMD_SEND_CONFIG_ID			"SEND_CONFIG_ID"
#define ATM_TERM_CMD_RENEWAL				"RENEWAL"
#define ATM_TERM_CMD_SND_DT_TIME			"SEND_DT_TIME"
#define ATM_TERM_CMD_SND_SUPPLY_COUNTERS	"SUPPLY_COUNTERS"

#define ATM_TERM_CMD_SND_TALLY				"SEND_TALLY"		/* Not supported */
#define ATM_TERM_CMD_SND_ERR_LOG			"SEND_ERR_LOG"		/* Not supported */
#define ATM_TERM_CMD_SND_CONFIG_INFO		"SEND_CONFIG_INFO"


#define ATM_TERM_CMD_SND_CI_HW_CONFIG		"HW_CONFIG"
#define ATM_TERM_CMD_SND_CI_SUPPLIES_DATA	"SUPPLIES_DATA"
#define ATM_TERM_CMD_SND_CI_FITNESS_DATA	"FITNESS_DATA"
#define ATM_TERM_CMD_SND_CI_TAMPER_SENSOR	"TAMPER_SENSOR"
#define ATM_TERM_CMD_SND_CI_SOFT_ID			"SOFT_ID"
#define ATM_TERM_CMD_SND_CI_ENH_CONFIG		"ENH_CONFIG"
#define ATM_TERM_CMD_SND_CI_LOC_CONFIG_OPT	"LOC_CONF_OPT"
#define ATM_TERM_CMD_SND_CI_BNA_NOTE_DEF	"BNA_NOTE_DEF"


#endif /*** HPS_COMMAND_H  ****/
