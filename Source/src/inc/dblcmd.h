/* Definition des commandes au niveau messages DIEBOLD */
/* Message Class                                   */
/* -------------                                   */
#ifndef HPS_DBLCMD_H
#define HPS_DBLCMD_H

#define 	TERMINAL_COMMAND			'1'
#define 	DATA_COMMAND				'3'
#define 	TRANSACTION_REPLY			'4'
#define		ENH_DATA_COMMAND                        '5'

#define 	UNSOLICITED_MESSAGE			'1'
#define 	SOLICITED_MESSAGE			'2'


/* Message SubClass                                */
/* ----------------                                */

#define 	CUSTOMISATION_DATA			'1'
#define 	INTERACTIVE_TRANSACTION_RESPONSE	'2'
#define 	ENCRYPTION_KEY_INFO			'3'

#define 	TRANSACTION_REQUEST                     '1'
#define 	STATUS_MESSAGE                          '2'

/***** Ajouter par S. AFELLAY *******/
#define         FINANCIAL_REQUEST                       'I'
#define 	SOLDE_REQUEST                           'H'
#define 	CHEQUE_REQUEST                          'F'
#define 	STATEMENT_REQUEST                       'G'
#define 	TRANSFERT_REQUEST                       'A'

/* Message Identifier                              */
/* ------------------                              */

#define 	SCREEN_DATA_LOAD  			'1'
#define 	STATE_TABLE_LOAD  			'2'
#define 	CONFIG_PARAMETER_LOAD 			'3'
#define 	DIEBOLD_PIN_INFORMATION_LOAD		'4'
#define 	FIT_DATA_LOAD        			'5'
#define 	CONFIG_IDENTIFIER_LOAD 			'6'
#define 	ENHANCED_CONFIGURATION_LOAD    		'A'
#define 	MAC_SELECTION_LOAD			'B'
#define 	DATE_AND_TIME_LOAD			'3'
#define 	INIT_EKC                		'F'


/* Definition des ordres donnes au terminal         */
/* Dans un message Operational Command              */

#define		START_UP				'1'
#define		OUT_OF_SERVICE				'2'
#define		SEND_CONFIG_INFO			'3'
#define		CLOSE_VANDAL_ALLOW			'4'
#define		CLOSE_VANDAL_DISALLOW			'5'
#define		AUTHO_UNLOCK_CHEST_DOOR			'6'
#define		NOT_AUTHO_UNLOCK_CHEST_DOOR		'7'
#define		ENH_MONOCHROME_GRAPHICS			'8'
#define		SEND_SUPPLY_COUNTER			'9'
#define		SEND_CLEAR_SUPPLY_CTR			':'
#define		SEND_ENH_SUPPLY_CTR			';'
#define		SEND_CLEAR_ENH_SUPPLY_CTR		'<'
#define		RUN_SELF_TEST				'='
#define		COND_OUT_OF_SERVICE			'>'


#define		TMK_UNDER_ZMK				'1'
#define		TPK_UNDER_TMK				'2'



/* Function Identifiers                             */

#define		RUN_DUMP				'Q'
#define		EJECT_CARD				'E'
#define		DEPOSIT_AND_PRINT			'1'
#define		DISPENSE_AND_PRINT			'2'
#define		DISPLAY_AND_PRINT       		'3' 
#define		PRINT_IMMEDIATE    			'4'
#define		NEXT_STATE_AND_PRINT			'5'
#define		SAFE_DEPOSIT_AND_PRINT			'6'
#define		EJECT_AND_DISPENSE_AND_PRINT 		'A' /* Card before Cash */
#define 	PARALLEL_DISPENSE       		'B'
#define		EJECT_DOC_FROM_ADM      		'E'
#define		OPEN_VALIDATION_PRINTER			'O'
#define		PRINT_STATEMENT_AND_WAIT    		'P'
#define		PRINT_STATEMENT_AND_NEXT_STAT		'Q'
#define		CASH_DOC_PRINT_AND_RETAIN		'R'



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


#define		USER_REQUEST				 2
#define		ERR_COMPTEUR				 1
#define		NO_CASH					 0	
#define		OUT_SERVICE				 9	
#define		READER_MALFUNCTION			 12              
#define		CLOCK_MALFUNCTION			 10
#define		NOT_CONFIGURED				 11

/* Debut Ajout par Y.K le 19-02-00 */
#define         UNKOWN_FAULT                             99
#define         SAFE_BOX_FAULT                           98
#define         PRNT_FAULT                               97
/* Fin   Ajout par Y.K le 19-02-00 */

#define		AUTRES					 3	
/*#define		CONFIRMATION				 4*/	
#define		ANNULATION				 5
#define         ERREUR_MONTANT                           6 


#define		CONTINUER				 7
#define		QUITTER  				 8
	
/*BEGIN AHM 02/08/2005*/
/*#define		PRINTER_CRITICAL			 "1"
#define		PRINTER_IN_SERVICE  		 	 "0"
#define		PRINTER_OUT_SERVICE  			 "2"
*/
#define		PRINTER_CRITICAL			 "C"
#define		PRINTER_IN_SERVICE  		 	 "I"
#define		PRINTER_OUT_SERVICE  			 "O"
/*END AHM*/
#define		RETURN					 6

/*BEGIN Adil HMAMI*/
#define         SEND_CONFIG_INFO                        '3'
#define         SEND_CLEAR_SUPPLY_CTR                   ':'
#define         SEND_CLEAR_ENH_SUPPLY_CTR               '<'
#define         SEND_ENH_SUPPLY_CTR                     ';'
#define         ENCRYPTOR_IN_SERVICE                    "I"
#define		INTERACTIVE_TIME_RESPONSE               "040"
/*END Adil HMAMI*/

#endif
