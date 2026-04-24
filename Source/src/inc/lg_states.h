#ifndef LOGIGAB_STATTES__H
#define LOGIGAB_STATTES__H


#define LG_STATE_1_OK							0 /* Correctement terminée */
#define LG_STATE_1_RESERVED_POWER				4 /* Opération sous réserve de contrôle à la suite d'unecoupure secteur */
#define LG_STATE_1_FORGOTTEN_CARD				12 /* Carte oubliée */
#define LG_STATE_1_BLOCKED_CARD					42 /* Carte bloquée dans lecteur ou capturée*/

#define LG_STATE_1_RESERVED_CTRL				45 /* Opération sous réserve de contrôle*/
#define LG_STATE_1_RUF							49 /* RUF */
#define LG_STATE_1_NO_TICKET					50 /* PAS DE BILLET INTRODUIT PAR LE CLIENT*/
#define LG_STATE_1_S_CAPTURED_CARD				53 /* Début de capture de carte*/
#define LG_STATE_1_E_CAPTURED_CARD				70 /* Fin de capture de carte*/
#define LG_STATE_1_TIME_OUT						99 /* Pas de réponse du porteur lors de la deuxième demande*/

/*  --------------Status -------------------------------------------------- */
#define	STATUS_OK								0   /*OK*/
#define	STATUS_WARNING							1	/*Warning*/
#define	STATUS_NOK								2
#define	STATUS_ERR								3
#define	STATUS_FATAL							4
#define	STATUS_PARTIAL							5	/*Reversal Partial*/

/*  --------------Response code -------------------------------------------------- */

#define APPROVED								0
#define APPROVED_TRACK_UPD						4
#define EXPIRED_CARD							101
#define PICK_UP_CARD							200
#define PIN_TRIES_EXCEED						206
#define SPECIAL_COND							207
#define LOST_CARD								208
#define STOLEN_CARD								209
#define SUSPECTED_FRAUD							210
#define CUTTOF_IN_PROGRESS						800





/*  --------------LOGIGAB EVENTS -------------------------------------------------- */

#define	ATM_TERMINAL_OS						"TROS"
#define ATM_CARD_READER_DEV_FAULT			"6100"
#define ATM_MANIPULATOR_DEV_FAULT			"6100"
#define ATM_NIGHT_SAFE_MOD_DEV_FAULT		"6102"
#define ATM_JOURNAL_PRINTER_DEV_FAULT		"6103"
#define ATM_RECEIPT_PRINTER_DEV_FAULT		"6104"
#define ATM_DOOR_ACCES_DEV_FAULT			"6105"
#define ATM_CHIP_CARD_READER_DEV_FAULT		"6106"
#define ATM_SCANNER_CHEQ_DEV_FAULT			"6107"
#define ATM_DISK_DEV_FAULT					"6108"
#define ATM_ENCRYPTOR_DEV_FAULT				"6109"
#define ATM_80_COL_PRINTER_DEV_FAULT		"6010"
#define ATM_TOUCH_SCREEN_DEV_FAULT			"6011"
#define ATM_WITHDRAWAL_DEV_FAULT			"6012"
#define ATM_RECYCLING_DEV_FAULT				"6013"
#define ATM_VALUED_DEP_2_DEV_FAULT			"6014"
#define ATM_VALUED_DEP_3_DEV_FAULT			"6015"
#define ATM_VALUED_DEP_4a_DEV_FAULT			"6016"
#define ATM_VALUED_DEP_4b_DEV_FAULT			"6017"
#define ATM_VALUED_DEP_4_DEV_FAULT			"6018"
#define ATM_VALUED_DEPT_ALL_DEV_FAULT		"6019"
#define ATM_ENVELOPE_DEP_DEV_FAULT			"6020"
#define ATM_SCANNER_DEP_DEV_FAULT			"6021"
#define ATM_NIGHT_DEP_DEV_FAULT				"6022"
#define ATM_NOTE_ATHEN_DEV_FAULT			"6023"
#define ATM_SCREEN_OPTEVA_DEV_FAULT			"6024"
#define ATM_NIGHT_DEP_MOD_DEV_FAULT			"6025"
#define ATM_DEFAULT_EVENT					"6103"

/*  --------------LOGIGAB STATE 1 -------------------------------------------------- */

#define LG_F118_VALIDITY                                    1
#define LG_F118_EXPIRED                                     2
#define LG_F118_POWER_FAILURE                               3
#define LG_F118_RES_POWER_FAILURE                           4
#define LG_F118_OVERLIMIT_FAST_CASH                         5
#define LG_F118_OFFLINE_TRX_STOP                            6
#define LG_F118_BALANCE_CANCEL                              7
#define LG_F118_FAST_CASH_REFUSE                          	8
#define LG_F118_WITHDRAWAL_CANCEL                           9
#define LG_F118_CLIENT_CANCEL                              	10
#define LG_F118_TIMEOUT                                 	11
#define LG_F118_FORGOT_CARD                                 12
#define LG_F118_NBR_TRIES_EXCEED                            13
#define LG_F118_AMOUT_NULL                                  14
#define LG_F118_ILS_CANCEL                                  15
#define LG_F118_NO_CHOISE                                   16
#define LG_F118_CARD_READER_FAULT                           17
#define LG_F118_EMV_CARD_READER_FAULT                       18
#define LG_F118_EMV_FAULT                                   19
#define LG_F118_PIN_CANCEL                                  20
#define LG_F118_PIN_ERROR                                   21
#define LG_F118_ILS_FAULT                                   22
#define LG_F118_NOT_ACCEPTED_CARD                           23
#define LG_F118_PIN_TRIES_EXCEED                            24
#define LG_F118_EMV_CONDITIONS                              25
#define LG_F118_CHEQUE_TIMEOUT                              26
#define LG_F118_DEPOT_TIMEOUT                               27
#define LG_F118_NOTE_DIST_CANCEL                            28
#define LG_F118_NOTE_PRE_CANCEL                             29
#define LG_F118_CARD_NOT_READABLE                           30
#define LG_F118_RUF_1                                        31
#define LG_F118_NOTE_AUTH                                   32
#define LG_F118_TERMINAL_OS                                 35
#define LG_F118_INCORRECT_DATA                              36
#define LG_F118_WITHDRAWAL_OS                               37
#define LG_F118_WITHDRAWAL_NULL                             38
#define LG_F118_NO_CTRL_PIN                                 39
#define LG_F118_OP_CANCEL                                   40
#define LG_F118_NOTE_EXTRACT_FAULT                          41
#define LG_F118_BLOCKED_CARD_IN_READER                      42
#define LG_F118_BLOCKED_ENVLOPE                             43
#define LG_F118_DOCUMENT_TIMEOUT                            44
#define LG_F118_OP_RSE_CTRL                                 45
#define LG_F118_MANIPULATOR_FAULT                           46
#define LG_F118_ENCRYPTORT_FAULT                            47
#define LG_F118_NO_OPERATION_POSSIBLE                       48
#define LG_F118_RUF_2                                       49
#define LG_F118_NO_NOTE_DEPOT_OP                            50
#define LG_F118_MAC_ERROR                                   51
#define LG_F118_TICKET_FAULT                                52
#define LG_F118_CARD_PICKUP_1                               53
#define LG_F118_CARD_PICKUP_2                               54
#define LG_F118_CARD_PICKUP_3                               55
#define LG_F118_CARD_PICKUP_4                               56
#define LG_F118_CARD_PICKUP_5                               57
#define LG_F118_CARD_PICKUP_6                               58
#define LG_F118_CARD_PICKUP_7                               59
#define LG_F118_CARD_PICKUP_8                               60
#define LG_F118_CARD_PICKUP_9                               61
#define LG_F118_CARD_PICKUP_10                              62
#define LG_F118_CARD_PICKUP_11                              63
#define LG_F118_CARD_PICKUP_12                              64
#define LG_F118_CARD_PICKUP_13                              65
#define LG_F118_CARD_PICKUP_14                              66
#define LG_F118_CARD_PICKUP_15                              67
#define LG_F118_CARD_PICKUP_16                              68
#define LG_F118_CARD_PICKUP_17                              69
#define LG_F118_CARD_PICKUP_18                              70
#define LG_F118_CARD_REFUND_1                               71
#define LG_F118_CARD_REFUND_2                               72
#define LG_F118_CARD_REFUND_3                               73
#define LG_F118_CARD_REFUND_4                               74
#define LG_F118_CARD_REFUND_5                               75
#define LG_F118_CARD_REFUND_6                               76
#define LG_F118_CARD_REFUND_7                               77
#define LG_F118_CARD_REFUND_8                               78
#define LG_F118_CARD_REFUND_9                               79
#define LG_F118_CARD_REFUND_10                              80
#define LG_F118_CARD_REFUND_11                              81
#define LG_F118_CARD_REFUND_12                              82
#define LG_F118_CARD_REFUND_13                              83
#define LG_F118_CARD_REFUND_14                              84
#define LG_F118_CARD_REFUND_15                              85
#define LG_F118_CARD_REFUND_16                              86
#define LG_F118_CARD_REFUND_17                              88
#define LG_F118_CARD_REFUND_18                              89
#define LG_F118_CARD_REFUND_19                              90
#define LG_F118_CARD_APP_REFUSE                             91
#define LG_F118_OVERLIMIT                                   92
#define LG_F118_OVERLIMIT_CUMUL                             93
#define LG_F118_PRAC_FAULT                                  94
#define LG_F118_MENEO_FAULT                                 95
#define LG_F118_CANCEL_WITH_OK                              96
#define LG_F118_PRINT_ILS_FAULT                             97
#define LG_F118_CANCEL_WITH_NOK_1                           98
#define LG_F118_CANCEL_WITH_NOK_2                           99

#endif



