#ifndef PWC_ATM_DBL__H
#define PWC_ATM_DBL__H


/* Evenement pour la table des evenements */
#include	<string.h>
#include	<stdlib.h>

/* Type d'evenement */
#define COMMUNICATION           "COM"
#define APPLICATION		"APP"
#define SECURITE		"SEC"
#define GAB      		"GAB"
#define SERVEUR                 "SER" 
#define AVIS			"AVI"

/* Reason code */
#define FERMETURE		"FER_"
#define DEMARRAGE		"DEM_"
#define ARRET			"ARR_"
#define COMPTABLE		"COM_"
#define TRANSFERT		"TRA_"
#define ONLINE                  "ONL_"
#define OFFLINE			"OFF_"
#define ERREUR_TRAIT		"ERR_"
#define CASSETTE_STATUS		"CAS_"

#define EN_COURS		"1"
#define EFFECTUE                "0"

#define DOWN			"1"
#define UP   			"0" 

#define		OK		0
#define		NOK		-1
#define         separator       '='
#define         separatord      'D'
#define 	DELIM		";"


/* ENGINE STATUS AND EVENT  */

/** Status & event for IB **/

#define		AUT_REQ_TO_NW 	  	200
#define		AUT_REP_FROM_NW	  	201
#define		AUT_REQ_FROM_NW	  	202
#define		AUT_REP_TO_NW     	203
#define		BAL_REQ_FROM_NW     	209
#define         ATM_CONF_FROM_NW        210 
/* Debut : Y. OUADI (05 Dec 1998)                   **
** Motif : ajout d'evenemt pour le serveur NDC      */
#define		BAL_REP_TO_NW     	211
#define		STAT_REP_TO_NW     	212
#define		FUNDS_REP_TO_NW     	213
#define		STAT_REQ_TO_NW     	7009
#define		FUNDS_REQ_TO_NW   	7015

#define		POWER_UP		400
#define		SUPER_ENTRY		401
#define		SUPER_EXIT		402
/* Fin   : Y. OUADI (05 Dec 1998)                   */

#define         MAJ_SUPER_STATUS        403 /* Ajout par Y.k le 18-02-00 */

#define		REJ_REQ_TO_NW     	204
#define		REJ_REP_FROM_NW   	205
#define 	REJ_REQ_FROM_NW   	206
#define		REJ_REP_TO_NW	  	207
#define		ISS_REJ_REQ_FROM_NW 	208

#define		AUT_GAB_REQ	 	66 
#define		AUT_HPS_RESP	   	67  
#define		REJ_GAB_REQ		68
#define		REJ_HPS_RESP    	69

#define		REJ_IB_REP	 	74 
#define		GAB_RELEVE	 	75 


#define		TRANS_GAB_REQ    	70
#define		AUT_IB_REP	 	71 
#define		GAB_1304               	72
#define		GAB_1314               	73

#define		GAB_ADM	       		814

/* TRACE LEVEL */
#define		FATAL   	0
#define		ERROR		1
#define		DATA 		2
#define		NOACTION	3
#define		INFO	    3
#define		TRACE      	4
#define		PROCESSING 	5
#define		STREAM    	6


/* MESSAGES TYPES */
#define		INFO	        0
#define		MESSAGE         1

#define		NETWORK              2
#define		QUEUE                3
#define		LOCAL_PROCESSING     5
#define		PRE_AUTH_PROCESSING 40

#define		HEADER_SIZE     4
#define		BIT_MAP_SIZE    8
#define		BIT_MAP2_SIZE   16

#define		BIT_MAP_SIZE_KNET    16
#define		BIT_MAP2_SIZE_KNET   32


/* MESSAGES TYPES CIS */
#define         AUTH_REQ_MSG    "1100"
#define         AUTH_REP_MSG    "1110"
#define         CANL_REQ_MSG    "1422"
#define         CANL_REP_MSG    "1432"

#define		AUT_REQ       	"0100"
#define		AUT_REP       	"0110"
#define		AUT_ADVICE_REP 	"0130"
#define		AUT_ACQ		"0180"
#define		ISS_MAJ_REQ    	"0302"
#define		ACQ_MAJ_REQ    	"0300"

#define		AUT_REQ_FROM_CIS 	100 
#define		AUT_REP_FROM_CIS 	110
#define		AUT_ADV_FROM_CIS 	120
#define		AUT_NEGATIVE_ACK 	190
#define	        FILE_UPDATE_REQ    	300
#define		FILE_UPDATE_REP    	310


/* MESSAGES TYPES MDS */
#define		TRANS_REQ       "0200"
#define		TRANS_REP       "0210"
#define		ADVICE_REQ      "0220"
#define		ADVICE_REP      "0230"
#define		TRN_NEGATIV     "0290"
#define		TRN_ACKNLG      "0280"

#define		RECONC_REP      "0530"
#define		ADV_RECONC_REP  "0532"

#define		ADMIN_ADV       "0620"
#define		ADMIN_REP       "0630"
#define		ADMIN_ADV_DPO   "0644"

#define		REVERS_REQ      "0420"
#define		REVERS_REP      "0430"
#define		REVERS_ADVICE   "0422"
#define		REVERS_ADV_REP  "0432"

#define		NW_MNG_REQ_     "0800"
#define		NW_MNG_REP_     "0810"
#define		NW_MNG_ADVICE_  "0820"
#define		NW_ADV_REP      "0830"


/* MESSAGES TYPES ISO */
#define         AUTH_GAB_REP_MSG        "1210"
#define         TRANS_GAB_REP_MSG       "1230"
#define         CANCL_REP_TO_GAB        "1430"
#define         REP_TO_GAB_ADM          "1814"
#define         MSG_1304_TO_GAB         "1304"
#define         RELEVE_REP              "1614"


/** Status & event for MCI **/

#define		TRANS_MSG_REQ	 	200 
#define		TRANS_MSG_REP	 	210 
#define		ADV_REQ_FROM_NW    	220
#define		ADVIC_MDS_REP    	230
#define		ADVIC_MDS_NA    	290

#define		ACQ_REJ_REQ		420
#define		ISS_REJ_REQ		422
#define		ACQ_REJ_REP		430

#define		ACQ_RECONC_REQ		500
#define		REP_ACQ_RECONC_REQ	510

#define		ISS_RECONC_REQ		502
#define		REP_ISS_RECONC_REQ	512

#define		ACQ_RECONC_ADV		520
#define		ACQ_RECONC_ADV_REPEAT	521
#define		REP_ACQ_RECONC_ADV	530

#define		ISS_RECONC_ADV		522
#define		ISS_RECONC_ADV_REPEAT	523
#define		ISS_ACQ_RECONC_ADV	532


#define		ADMIN_ADV_REQ		620
#define		ADMIN_ADV_REP		630
#define		ADMIN_DPO		644

#define		AUT_CIS_REP		110
#define		AUT_CIS_REQ		100

#define		PRE_REQ_FROM_NW		100

#define		NW_MNG_REQ		800
#define		REP_NW_MNG    		810
#define		NW_MNG_ADV		820
#define		NW_MNG_ADV_REPEAT	821
#define		REP_NW_MNG_ADVICE	822
#define		NW_MNG_KEY_EXCHANGE     823
#define		REP_NW_MNG_ADV_REP   	830

#define		NW_MNG_REQ_ON		8001
#define		NW_MNG_REQ_OFF		8002
#define		NW_MNG_REQ_ECH		8003
#define		NW_MNG_REQ_CUT		8004
#define		NW_MNG_REQ_CUT_REP	8005
#define		REP_NW_MNG_REQ_ON	8100
#define		REP_NW_MNG_REQ_OFF	8101
#define		SAF_SESSION_REQ		8102
#define		SAF_SESSION_REP		8103

#define		LATE_REVERSAL   	9000

#define		AUT_ISO_REQ	 	120 
#define		REJ_ISO_REQ	 	142 

#define		ISS_MAJ_FILE_REP 	312 

#define		ISO_NW_MNG_REQ 		804 

/* ISO MANAGEMENT */
#define		LLVAR	      -10
#define		LLLVAR	      -11

#define		VERSION	      110 
#define		SIZEBUF	      512 


/* QUEUES MANAGEMENT   */
#define SIZE_CONF	         128 

#define READY                   1000
#define DELIVERED               1001
#define DELIVERY_FAILED         1002
#define ACK_NOT_RECEIVED        1003
#define CONGESTION              1005
#define AVAILABLE               1006
#define NOT_AVAILABLE           1007
#define OFF_LINE                1008
#define RETRY                   1009
#define RECEIVED_FAILED         1010
#define ACK_NOT_DELIVERED       1011
#define RECEIVED                1012


/*types PICK_UP  */
#define         TIME_OUT                205
#define         CENTRAL_ORDER           206
#define         COUNTERFEIT             207
#define         AUTHENTIFICATION        208
#define         P_U_EXPIRED             209
#define         CARD_READER             210



/* TRAITEMENT HSM */
#define Len_HSM_HEADER        	   6
#define Len_Buffer_transmit	 256
/*BEGIN AHM 2007/02/10*/
/*#define SIZE_KEY                  16*/
/* EBE: multiple definitions... 
#define SIZE_KEY                 256 */
/*END AHM*/
#define SIZE_ERROR_CODE            2
#define SIZE_EXPIRAT_DATE	   4
#define SIZE_SERV_CODE		   3
#define SIZE_PAN		  19
#define PIN_FORMAT_ERROR          20

#define ERROR_HEADER	        2000
#define ERROR_COMMAND           2001
#define CHECK_ERROR             2002


#define ATM			1
#define POS			2
#define NET			3

/*------- KNET -------*/
#define		TPII_TRN_REP	 2200 
#define		TPII_REVERS_REP	 3200 
#define		START_HEADER	 "ISO"
#define		MSG_HEADER_SIZE   16
#define		KNET_HEADER_SIZE  9

#define		POS_KNET_HEADER  "025000077"
#define		ATM_KNET_HEADER  "015000077"

#define		NW_LOG_HEADER    "005000070"
#define		NW_ECHO_HEADER   "005000067"



/**************************************************************************/


/*Debut Modif Fb 05-09-99*/
/*BKME Update To SGBS*/
#define		LAST_PIN_RETRY          		105
#define NO_ERROR			"00"
#define VERIFICATION_ERROR		"01"
/*Fin Modif FB 05-09-99*/
/*
#define		PIN_RETRY_LIMIT_EXCEEDED          	106
*/
#define		INVALID_MERCHANT                        109
#define		RESTRICTED_CARD                         104
#define		INVALID_AMOUNT                          110
#define		PIN_ERROR                               117
#define		NOT_PERMITED_TO_TERMINAL                120
#define		CRYPTOGRAPHIC_ERROR                     128
#define		CVV_ERROR                               183

#define		FRAUD_SUSPECTED_PICK_UP_CARD	        202
#define		PICK_UP_SPECIAL_CONDITION	        207
#define		LOST_CARD_PICK_UP                       208
#define		STOLEN_CARD_PICK_UP                     209
#define		COUNTERFEIT_PICK_UP_CARD 	        210

#define		FIELD_IN_ERROR                          304

#define		INVALID_BUSINESS_DATE                   900
#define		INVALID_TRANSACTION                     902

#define		NO_ROUTING                              908
#define		SYSTEM_MALFUNCTION                      909
#define		ISSUER_NOT_AVAILABLE                    912
/* Debut ajout par Y.K le 11-09-99 : valeur utilisee par ServPOS */
#define         ISSUER_TIME_OUT                         911
/* Fin   ajout par Y.K le 11-09-99 : valeur utilisee par ServPOS */
#define		ISSUER_NOT_FOUND                        992
#define		PIN_VERIFICATION_NOT_POSSIBLE           993
#define		PROCESSING_NOT_AVAILABLE                994
#define		PROCESSING_ERROR                        995

#define		PROCESSED			        999


/*************************************************************************/
/* Ressources reason codes                                               */
/*************************************************************************/
#define		REQUEST_TO_STOP				"0000"
#define		LOCAL_NET_FAILURE			"0001"
#define		UNKOWN_ADDRESS				"0002"
#define		NETWORK_CONGESTION			"0004"
#define		REMOTE_NETWORK_FAILURE			"0005"



#define		NON_TRANSPARENT_MODE			87
#define		TRANSPARENT_MODE			88
#define		SEND_THROUGH     			89

/**********************************************************/
/* NDC DEFINE 						  */
/**********************************************************/
/* FUNCTION CODE */
#define WITHDRAWAL_ADVICE               "201"
#define AVAILABL_REQ                    "282"
#define OPEN_SESSION                    "801"
#define CUT_OFF_ACK                     "882"
#define ECHANGE_K7                      "881"
#define ON_SERVICE                      "890"
#define GAB_OUT_SERVICE                 "891"
#define START_SUPERVISOR_MODE           "892"
#define END_SUPERVISOR_MODE             "893"
#define WITHDRAWAL                      "200"
#define BALANCE_REQ                     "281"
#define CHEQUE_REQ                      "692"
#define STATEMENT_REQ                   "691"

#define CASH                            200
#define BALANCE                         281
#define STATEMENT                       691
#define CHEQUE                          692


/*spec BANK*/
#define BANK_CODE               "000001"

/********** APPLICATION CONECTION NAME FOR TEST ENVIRONNEMENT ********/
#define NDC_APP_TEST            "NDCTST"
#define DBL_APP_TEST            "DBLTST"
#define POS_APP_TEST            "POSTST"
#define DLT_SRV_TEST            "serv_hostst"
#define DLT_HST_TEST            "hostest"
/********* APPLICATION CONECTION NAME FOR TEST ENVIRONNEMENT *********/

/********** APPLICATION CONECTION NAME FOR PROD ENVIRONNEMENT *********/
#define NDC_APP_PROD            "NDCSERV"
#define DBL_APP_PROD            "DBLSERV"
#define POS_APP_PROD            "POSPWR"
#define DLT_SRV_PROD            "serv_host"
#define DLT_HST_PROD            "hostprod"
/********** APPLICATION CONECTION NAME FOR PROD ENVIRONNEMENT *********/


/**** PRODUCTION QUEUE ID *****************/
#define         BAL_PROD        (key_t)10
#define         BAL_VERIF       (key_t)20
#define         BAL_IPC_ONLINE  (key_t)30
#define         BAL_CIS         (key_t)40
/**** PRODUCTION QUEUE ID *****************/

/**** TEST EVIRONNEMENT QUEUE ID ***********
#define         BAL_PROD        (key_t)50
#define         BAL_VERIF       (key_t)60
#define         BAL_IPC_ONLINE  (key_t)70
#define         BAL_CIS         (key_t)80
 **** TEST EVIRONNEMENT QUEUE ID ***********/

/******
#define USER_PROD              "powercard"
#define PASS_PROD              "pcard001"
#define USER_TEST              "test"
#define PASS_TEST              "test1000"
****/

#define END_PROCESS             6000
#define DELAY                   38 /* changement du timer de 30s � 38s */ 

#define BLOCK_SIZE                      256
#define STOP_LIST                       881
#define SEND_CUT_OFF                    888
#define REBOOT_GAB                      898
#define PICK_UP_CARD                    200
#define CARD_MANIPULATE                 202
#define ALTERN_AMOUNT_CANCELED          280

#define NODATA_WAITING                  50
#define DATA_RECEIVED                   100

#define CHEQ_REQ_CANCEL                 282
#define FONDS_INSUFFISANTS              116
#define CONTROL_ZONE_ERROR              304
#define INVALID_TRANSACTION             902
#define NO_ROUTING                      908
#define SYSTEM_MALFUNCTION              909
#define ISSUER_NOT_AVAILABLE            912
#define ISSUER_NOT_FOUND                992
#define PIN_VERIFICATION_NOT_POSSIBLE   993
#define PROCESSING_NOT_AVAILABLE        994
#define SERVER_PROCESSING_ERROR         995
#define TRANSACTION_NOT_PERMITTED       120

#define END_TRANSFERT                   300
#define OK_NO_ACTION                    899
#define INSERT_BLOCK                    301

#define UPDATE_PAR                      882
#define UPDATE_APP                      883
#define UPDATE_SCR                      884
#define UPDATE_PRF                      885
#define PIN_KEY_TRANSFERT               891
#define TPK_KEY_TRANSFERT               892
#define MAC_KEY_TRANSFERT               893
#define APPROVED                        0
#define DECLINED                        100
#define PIN_RETRY_LIMIT_EXCEEDED        206
#define WRONG_AMOUNT                    110
#define PIN_ERROR                       117
#define ACTION_NOT_PERMITTED            119
#define CRYPTOGRAPHIC_ERROR             128
#define CVV_ERROR                       183



/* EMS SPECIFICATION */

#define         ADVIC_EMS_REP           230
#define         ADVIC_EMS_NA            290

/* PASSWORD     SK 12/04/98   */

#define		KEY_PASS		"A1002B0607C1806F"
#define 	PROD			"0"
#define		TEST			"1"


/****** START VARIABLES INTERFACE HOST CBK    *******************************/
/****** M.MORTADY LE 11/08/98       *******************************/
#define         CAISSE_CUT_OFF          "1824"
#define         HEADER_HOST             "ISO11000000"
#define         HEADER_HOST_SIZE        11
#define         LG_ENTETE_ATM           11
#define         HOST_TIME_OUT           290
#define         HOST_NOT_AVAILABLE      280
#define         HOST_RESPONSE           1
#define         LG_ENTETE_CONVER        65
#define         STOP_HOST               "STOP HOST"
#define         LG_ENTETE_CICS           50
#define         HT_UPDATE_FILE             300
#define         AUTH_GAB_REQ_MSG        "1200"
#define         BUFFER_TIME_OUT         "TIME OUT"

#define         STAT_CHQ_REQ            "1604"
#define         MSG_1314_TO_HOST         "1314" 

/****** END INTERFACE HOST CBK      *******************************/

/****** DEFINE FOR VISA BASEI       ******************************/
#define         BASE1_QUERY              700

/* ISO MANAGEMENT */
#define         VAR             -20
#define         VARB            -21
#define         LLVAR           -10
#define         LLLVAR          -11

#define         OPTIONAL        2
#define         ZEROWISE        3
#define         GENERATION      4


/* VISA SPECIFICATION */

#define VIP_REVERS_REP                  "0410"
#define VIP_REVERS_REQ                  "0400"
#define VIP_ADMIN_REQ                   "0600"
#define VARIABLE                        1
#define FIXED_LG                        0


#define         VIP_HEADER_SIZE         22
#define         SOURCE_STATION          999999


#define         SI_MODE_ENTRY           8104
#define         SI_MODE_ENTRY_REP       8105
#define         SI_MODE_EXIT            8106
#define         SI_MODE_EXIT_REP        8107
#define         PROMPT_ADVICE           8108
#define         PROMPT_ADVICE_REP       8109
#define         SAF_SESSION_END         8110
#define         SAF_SESSION_END_REP     8111

#define         AUT_ADV_TO_NW            120
#define         ADM_REQ_FROM_NW         0600


/* Y.O : le 14 Dec 1998 */
#define	STANDIN				0
#define	NO_STANDIN			1
/* Y.O : le 14 Dec 1998 */

/* M.M : le 27/01/99 pour Interface SMT */

#define        LATE_REVERS_MSG                 1421
#define        ADV_REP_FROM_NW                 130
#define        ADV_REP_TO_NW                   131
#define        REP_NW_MNG_REQ_ECH              301
#define        NW_KEY_CHG_REP                  102
#define        PARTNER_ID_LG                   10

/* M.M : le 27/01/99 pour Interface SMT */

#define INVALID_DEPOSIT_NUMBER            691
#define INVALID_CHEQUE_NUMBER             692

/** DEBUT AJOUT M.MORTADY LE 15/06/99 DATE_DEMARRAGE DDMMYYYY **/
#define DATE_DEMARRAGE             	10071999
/** FIN   AJOUT M.MORTADY LE 15/06/99 DATE_DEMARRAGE DDMMYYYY **/

/** DEBUT AJOUT Y.K LE 04/08/00 : modif timer de visa **/
/* #define         TIMER_VISA              20 */
#define         TIMER_VISA              35
/** Fin   AJOUT Y.K LE 04/08/00 : modif timer de visa **/

#define		TIMED_AUT_REP_FROM_NW	8201

/* Debut Ajout par Y.K le 23-11-99 : gestion du 0800 pour Dde de reconciliation VISA */
#define		NW_MNG_REQ_REC       	8270
#define		SIGN_ON_ADV           	8078
#define		SIGN_ON_REQ          	8071
#define		SIGN_OFF_REQ          	8072
/* Fin   Ajout par Y.K le 23-11-99 : gestion du 0800 pour Dde de reconciliation VISA */

/* Debut Modif par Y.K le 24-11-99 : Ajout de la gestion du message 0500 SMS */
#define		TOTAL_RECONC_MSG	5272
/* Fin   Modif par Y.K le 24-11-99 : Ajout de la gestion du message 0500 SMS */

/* Debut Modif par Y.K le 24-11-99 : Ajout de la gestion du message 0810 response au Dde de Reconciliation */
#define		REP_NW_MNG_RECONC       8280
/* Fin   Modif par Y.K le 24-11-99 : Ajout de la gestion du message 0810 response au Dde de Reconciliation */


/* SPEC SGBS  Ajou par Y.K le 14-03-00 */
# define        SHORT_STAT_ROUTING      -1
# define        BALANCE_ROUTING         -1
# define        CHEQUE_ROUTING          -1

# define        SHORT_STAT_STANDIN       0
# define        BALANCE_STANDIN          0
# define        CHEQUE_STANDIN           0
/* SPEC SGBS */

/* Debut MDS */
#define  ECHO_REQ_TO_NW              7015
#define  ECHO_TST_REP                2700
#define  VOID                        2701
/* Fin   MDS */

#endif


