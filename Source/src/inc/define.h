#ifndef HPS_DEFINE_H
#define HPS_DEFINE_H

/*#define DEBUGIN */ /***variable necessaire au debogage Mehdi Elyajizii*/
/*int debug_option = 0; *//*MKB060309 PA DSS enable/disable Debugging */

/*EBE120925: This file is included in other source files, so debug_option is declared 'extern'. this variable defined in libc/dump.c */
/* EBE140421: moved to dump.h
extern int debug_option;*/

typedef unsigned long msg_id_t;

/* TBO12082014 */
extern int	g_InterfaceMode;


#define SCREEN_TIME_OUT_VALUE  60

#define P7_PAGESIZE		4096

#define OK        	    0
#define NOK       	   -1
#define TIMEOUT   	   -100
#define WAITING   	   -200
#define DUPLICATE  	   -3
#define MALFUNCTION        -2
#define SEVERE_ERROR       -9

#define VRAI        0
#define FAUX       -1


#if ( defined(LINUX) || defined(SOLARIS) ) && !defined(TRUE)
	#define TRUE        VRAI
	#define FALSE       FAUX  
#endif
/* Mehdi Elyajizi AIX already exist in AIX */	

#define SUCCESS  0
#define SUCCES   0
#define ECHEC   -1

#define BYTE unsigned char

#define    REQUEST          0
#define    RESPONSE         1
#define    CONFIRMATION     2
#define    REVERSAL         3
#define    REPEAT           4

/*******************/
#define INVALID_DEPOSIT_NUMBER            691
#define INVALID_CHEQUE_NUMBER             692

#define MAX_NUMBER_CHEQ_DEPOT                      10
#define MAX_NUMBER_DOC_DEPOT                       10
/*******************/

/*******************/
/* EBE140920
#define VERIF_MODE        0
#define PROD_MODE         1
#define HSM_BOTH_MODE		2	*//*EBE140716 */

#define HSM_VERIF_MODE		(1<<0)
#define HSM_PROD_MODE		(1<<1)
#define HSM_ISS_MODE		(1<<2)
#define HSM_ACQ_MODE		(1<<3)

#define HSM_ISS_VERIF		(HSM_ISS_MODE|HSM_VERIF_MODE)
#define HSM_ACQ_VERIF		(HSM_ACQ_MODE|HSM_VERIF_MODE)
#define HSM_ISS_ACQ_VERIF	(HSM_ISS_MODE|HSM_ACQ_MODE|HSM_VERIF_MODE)
#define HSM_ALL				(HSM_ISS_MODE|HSM_ACQ_MODE|HSM_VERIF_MODE|HSM_PROD_MODE)

/*******************************************************************/
/*  Constatntes utilisees pour differenciation des modes reversals */
/*******************************************************************/
#define    CAPTURE_MODE    "C"
#define    ROUTING_MODE    "R"
#define    NORMAL_MODE     "N"
#define	   LOCAL_MODE	   "L"	/*EBE140524: Reversal to be processed only locally and not forwarded */
/*************************************************************/

#define    PRESENT          0
#define    NOT_PRESENT     -1

#define    START            1 
#define    STOP             2 

#define     ATM_MNG_CMD         600
#define     SERVICE             32
#define     EXIT           	  	34
#define     C_CONFIG            33
#define     W_CONFIG            36

/* TRACE LEVEL */
#define		FATAL   	0
#define		ERROR		1
#define		DATA 		2
#define		NOACTION	3
#define		INFO     	3
#define		TRACE      	4
#define		PROCESSING 	5
#define		STREAM    	6
/*#define         INVALID_MAC 7*/

#define MSG_MAX_LEN     600
#define     VERIFICATION_ERROR  "01"

/****** SAF MANAGEMENT ***/
#define SAF_RESPONSE      0
#define SAF_TIMEOUT       1

/****** SIGN MANAGEMENT ***/
#define SIGN_RESPONSE      0
#define SIGN_TIMEOUT       1

/****** CUT_OVER MANAGEMENT ***/
#define CUT_OVER_RESPONSE               0
#define CUT_OVER_TIMEOUT                1
#define NORMAL                         'N' 
#define CUTOFF_TO_BE_GENERATED         'V'
#define ACQ_STLMNT_TO_BE_GENERATED     'A'  
#define ISS_STLMNT_TO_BE_GENERATED     'I'  

/*** LIST MANAGEMENT ****/
/**** Defined In errno.h
#define  EEXIST          -10
#define  ENOMEM          -12
****/
#define  ENOEXIST        -11
#define  ELATE           -12

/***** MAX LENGTH ****/
#define  LG_MAX				4096
#define  LG_MAX_POSTILION	50000  /* MRY HFC 050723 */
#define  BULK_LG_MAX		50000
#define  BULK_C_LG_MAX		4096
#define  MAX_LINE_TRC		 512
#define  TLV_MAX_LENGTH		3096            /*AMER 20160721: 2000 turned into 3096*/
#define	 TLV_BUF_LG_MAX		4096
#define	 BUF_LG_MAX			1516
#define  MAX_FIELD_LENGTH	1024	/*AMER 20160701*/

#define  MAX_CMD_LEN		128		/*Start ICH20191110*/

#define	 NOT_AVAILABLE	    1007 
#define  ERROR_COMMAND      2001
#define  CHECK_ERROR        2002

/****THREAD ****/
#define MAX_WAIT_REQUEST      500
/*#define MAX_WAIT_REQUEST      5000*/
/*#define RESERVED_THREAD        20*/	/* EBE140107*/
#define RESERVED_THREAD        40
#define HSID_PROC_LINE        105 /*SKA20221117 PLUTONL-5393*/
#define HSID_PROC_BANK        355 /*SKA20221117 PLUTONL-5393*/
#define MAX_BAL_THREAD        256
#define MAX_THREAD            512

#define MAIN_THREAD_ID          0
#define BAL_THREAD_ID           1
#define LINE_THREAD_ID          2
#define SIG_THREAD_ID           3
#define ADM_THREAD_ID           4
#define TIME_THREAD_ID          5
#define SAF_THREAD_ID           6
#define CUTOFF_THREAD_ID        7
#define CTL_THREAD_ID           8
#define PRIS_THREAD_ID          9
#define IPC_THREAD_ID          10
#define UFS_THREAD_ID          11
#define DBTS_THREAD_ID         12
#define SIGNIN_THREAD_ID       13
#define STLMNT_THREAD_ID       14
#define HSM_THREAD_ID          9
#define HSM_PROD_THREAD_ID		10
#define HSM_VERIF_THREAD_ID		11
#define POLLING_THREAD_ID     15 /*ADD KS 041111 Polling et 150906*/
#define NOTIF_THREAD_ID       16 /*ADD MKB/YAB Notification mudule 22112011*/
#define CBCOM_TIME_THREAD_ID        17 /*ADD SWI pour le traitement protocole CBAE IPDU 19122012*/

#define FRAUD_PROC_THREAD_ID			9
#define FRAUD_TO_THREAD_ID             10

#define ORCH_THREAD_ID         25	/*AA : orchestrator*/
#define SWMON_RECON_THREAD_ID  26	/*BRS Reconciliation*/
#define BRIDGE_R_THREAD_ID		27 	/*SKARROUMI PLUTONL-4900*/
/*Start EBE130925:SWMON*/
#define SYSMON_PURGE_ID			18
#define SYSMON_PROC_ID			19
#define SWMON_PURGE_THREAD_ID	20
#define SWMON_PROC_THREAD_ID	21
#define SYSMON_PRIS_PROC_ID		22
/*End EBE130925:SWMON*/
#define FU_THREAD_ID			23		/* File update thread */

#define SWMON_NTWRAWMSG_THREAD_ID	24  /*IBO20200508*/

#define DE_MAX_THREAD 			40		/*AMER20210915 PLUTONL-3545: Daemon event max thread*/			

#define MAX_LENGTH_ORA_ERROR_LIST	100 /* ELKAM16092022 PLUTONL-5169 */

/** Ajout G.F le 04/03/03 pour le traitement SMS Outgoing **/
#define OUTG_THREAD_ID         500
/** Fin   G.F le 04/03/03 pour le traitement SMS Outgoing **/

#define SUPERVISE_TIME_OUT      5

#define	  PG_EBCDIC             		"EBCDIC_MODE" /*NAB26032020*/
#define	  PG_ECOM_CAVV          		"ECOM_CAVV" /*NAB07042020 PLUTONL-1477*/
#define	  PG_AUTH_REPLY_BACK_PERIOD		"AUTH_REPLY_BACK_PERIOD" /*ZKO10032022 PLUTONL-4411*/

/*****************************************/
#define   INVALID_AMOUNT        110
#define   FIELDS_CTL_ERROR      304 
#define   INVALID_TRANSACTION   902
#define   SYSTEM_MALFUNCTION    909
#define   INVALID_VALUE         910
#define   PROCESSING_ERROR      995
#define   DUPLICATE_AUTHO       308 /**MKB040408**/

/*  Issuer Response Codes */
#define   ISS_MALFUNCTION_ACTION             "909"

/*  ISO Response Codes */
#define   APPROVED_ACTION                  "000"
#define   PARTIAL_AUTHORIZATION_ACTION     "002"
#define   PIN_NOT_CHANGED    			   "039"	/*AMER20210809 PLUTONL-3173 Enh21.Q4 MC AN2921*/
#define   INVALID_PIN_ACTION               "117"
#define   TRX_NOT_PERMITTED_TO_CARDHOLDER  "119"	/*AMER20210804: Enh21.Q4 MC AN4972*/
#define   INVALID_AMOUNT_ACTION            "110"	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
#define   CRYPTOGRAPHIC_ERROR_ACTION       "128"
#define   ISSUER_UNAVAILABLE_ACTION        "912"
#define   MALFUNCTION_ACTION               "909"
#define   REVERSAL_COMPLETED_ACTION        "480"
#define   REVERSAL_NO_ACTION      		   "481"
#define   INCONSISTENT_REVERSAL_ACTION     "483"
#define   INVALID_MERCHANT_ACTION		   "109"
#define   POS_UPLOAD_NOK_ACTION		 	   "306"
#define   POS_UPLOAD_OK_ACTION		 	   "300"
#define   DO_NOT_HONOR           	 	   "100"
#define   UPDATE_TRACK3                    "004"
#define   ALTERN_AMOUNT                    "001"
#define   FUNCT_NOT_AVIALABL               "115"
#define   INVALID_TRANSACTION_ACTION 	   "902"
#define   REFER_TO_ISSUER_ACTION  		   "108" 
#define   FIELD_CONTROL_ERROR_ACTION 	   "304"
#define   NO_REASON_TO_DECLINE             "085" /*ENH OCT 8.2 SWI100908*/
#define   PIN_INFORMATION_REQUIRED         "112"
#define   PIN_INFORMATION_INCORRECT        "117"
#define   CONTACTLESS_LIMIT_EXCEEDED       "186" /*MTR20180409 - Enh CBAE4.12 */
#define   PURCHASE_ONLY_APPROVED           "500"  /* Mehdi Elyajizi Purchase with cash back */
#define   PARTIAL_APPR_NOT_ALLOWED         "501"	/* Mehdi Elyajizi Purchase with cash back */
                                           
/*OBE08022007*/
#define 	BAD_CVV_ACTION 									"183"
/*OBE08022007*/

/* BAD CVV MESSAGE OBE08022007*/
#define 	BAD_CVV_MSG 	"BAD CVV/iCVV CONTROLED BY VISA"
/* BAD CVV MESSAGE OBE08022007*/



/* RESOURCE_LIVE & RESOURCE_PROCESSING_STEP */
#define OFF       "OFF"
#define OPEN      "OPN"
#define CONNECT   "CNT"
#define SAF       "SAF"
#define SIGNON    "ONL"
#define SIGNOFF   "OFL"
#define STOPI     "STP"  /*ICH20170706 (PROD00044554)  Internal use,not appear in GU,is used to avoid reconnect interface in case of shutdown signal*/

/*
#define LIVE       "Y"
#define CRITICAL  "C"
#define NOTLIVE   "N"
*/
#define LIVE      "YYY"
#define CRITICAL  "CCC"
#define NOTLIVE   "NNN"

/* DEFINITION DES TYPES D'AUTORISATION */
#define STAND_IN   		1
#define STAND_IN_OK_THEN_HOST   2
#define STAND_IN_NOK_THEN_HOST  3
#define STIP       		4
#define ROUTAGE    				5

/* Definition Parametre Connection */
#define        PROD                    "0"
#define        KEY_PASS                "A1002B0607C1806F"

/* TRANSACTION TYPES */
#define  INQUIRY       0
#define  AUT_DEBIT     1
#define  AUT_CREDIT    2
#define  REV_DEBIT    10
#define  REV_CREDIT   20

/*** ATM CONST ****/
/* Events */
#define		TRANSACTION_REQ_RECEIVED	1
#define		TRANSACTION_RESP_RECEIVED	2
#define		TRANSACTION_TIMEOUT			3
#define     SOLICITED					34
#define     UNSOLICITED					35
#define     POWER_UP					400
#define     SUPER_ENTRY					401
#define     SUPER_EXIT					402
#define     POWER_UP_RECEIVED			400
#define     SUPER_ENTRY_RECEIVED		401
#define     SUPER_EXIT_RECEIVED			402

/* Type d'evenement */
#define COMMUNICATION           "COM"
#define APPLICATION             "APP"
#define SECURITE                "SEC"
#define GAB                     "GAB"
#define SERVEUR                 "SER"
#define AVIS                    "AVI"

/* Reason code */
#define FERMETURE               "FER_"
#define DEMARRAGE               "DEM_"
#define ARRET                   "ARR_"
#define COMPTABLE               "COM_"
#define TRANSFERT               "TRA_"
#define ONLINE                  "ONL_"
#define OFFLINE                 "OFF_"
#define ERREUR_TRAIT            "ERR_"
#define CASSETTE_STATUS         "CAS_"

#define EN_COURS                "1"
#define EFFECTUE                "0"

#define DOWN                    "1"
#define UP                      "0"

#define COM_UP                 0
#define COM_DOWN               1

/* Resource Activity Counters */
#define  ECHO_TEST_NBR           1
#define  APPROVAL_NBR            2
#define  DECLINE_NBR             3
#define  CAPTURE_NBR             4
#define  WRONG_PIN_NBR           5
#define  REVERSAL_NBR            6
#define  CHARGEBACK_NBR          7
#define  TIME_OUT_NBR            8
#define  ACQ_TRX_NBR             9
#define  ISS_TRX_NBR            10

/* ISO Message Type */

#define AUT_REQUEST 		"1100"
#define AUT_REQUEST_REP 	"1110"
#define FINANCIAL_REQ		"1200"
#define FINANCIAL_ADV		"1220"
#define FILE_UPD_REQ		"1304"
#define REVERSAL_ADV		"1420"
#define RECONCIL_ADV_ACQ	"1520"
#define RECONCIL_ADV_ISS	"1522"
#define NET_MGT_REQ			"1804"
#define FINANCIAL_ADV_REP	"1230"
#define FINANCIAL_ADV_REP	"1230"
#define FILE_UPD_ADV		"1324"
#define FILE_UPD_ADV_REP	"1334"
#define REVERSAL_ADV_REP	"1430"


/**** VRS CONSTANTES ***/
#define  CR_LF_FLAG           0
#define  SEND_LINE_FLAG       1
#define  MAX_WAIT_LINE        20  /*ADD KS 041111 Polling et 150906*/ 
#define  POLL_INTERVALL       10  /*ADD KS 041111 Polling et 150906*/ 
/**** VRS CONSTANTES ***/

/**** SERVERS STATUS ***/
#define TO_BE_STOPPED          1
#define ALIVE                  2

#define FONDS_INSUFFISANTS 		116

#define DELIVERED               1001
#define DELAY_HSM               3
#define RECEIVED                1012

#define MODIF_PIN      		"304" 
#define  OFFS_OFFSET             49




#define TR_CONFIRMATION                           13
/****SITE REGION : IF EUROPE THEN E ELSE N(NON E)****/
#define SITE_REG          "E"   /*  Process Site Region */

#define PROC_PID_LEN			8
/*
#define MAX_DB_INACTIV_PERIOD				60			
#define MAX_DB_CONN_RENEW_WAIT_PERIOD		2
*/
/*EBE140421*/
#undef P7_DEF_SYM
#if defined(P7_IMPORT_SYM)
#	define P7_DEF_SYM	extern
#else
#	define P7_DEF_SYM
#endif

/*EBE140421: include dump.h here to avoid modifying all sources */
#include <stdio.h>
#include <string.h>
#include <dump.h>

#include <p7_config.h>
#if defined(FO_SWI_SA_CORE) || defined(FO_SWI_ST_CORE) || defined(FO_SWI_PD_CORE)
#define IS_P7_SWI	1
#else
#define IS_P7_SWI	0
#endif

#if defined(FO_ATM_SA_CORE) || defined(FO_ATM_ST_CORE) || defined(FO_ATM_PD_CORE)
#define IS_P7_ATM		1
#else
#define IS_P7_ATM		0
#endif

#if defined(FO_POS_SA_CORE) || defined(FO_POS_ST_CORE) || defined(FO_POS_PD_CORE)
#define IS_P7_POS		1
#else
#define IS_P7_POS		0
#endif

#if defined(FO_ISS_SA_CORE) || defined(FO_ISS_ST_CORE)
#define IS_P7_IAS		1
#else
#define IS_P7_IAS		0
#endif

#if defined(BO_ISS_ST_CORE) 
#define IS_P7_IBE               1
#else
#define IS_P7_IBE               0
#endif

/*ICH20180321 to active active/active archi*/

#if defined(FO_ACT_ARCH)
#define IS_P7_AAA              	1
#else
#define IS_P7_AAA               0
#endif


/*--AER15012015i Special MCC ----*/

/*-- MCC LIST----*/
#define MCC_AUTOMATED_CASH				"6011"
#define MCC_MANUAL_CASH					"6010"
#define MCC_TELCOM_SERVICE				"4814"
#define MCC_MONEY_TRANSFER				"4829"	/*AMER20210804 */
#define MCC_QUASI_CASH_MER				"6051"
#define MCC_QUASI_CASH_FIN_INST			"6050"
#define MCC_BETTING_CASINO_GAMBLING		"7995"
/*Start AMER20210119 PLUTONL-2657: Enh20.Q3 MC AN3405*/
#define MCC_GOV_OWNED_LOTTERY           "7800"
#define MCC_INTERNET_GAMBLING           "7801"
#define MCC_GOV_HOSRSE_DOG_RACING       "7802"
#define MCC_GOV_OWNED_LOTTERY_SPEC_CTRY         "9406"
/*End AMER20210119 PLUTONL-2657: Enh20.Q3 MC AN3405*/
#define MCC_AFD     					"5542"
#define MCC_SERVICE_STATIONS     		"5541"		/*AMER20171128 PROD00049633*/
#define MCC_TRSFT_M_AGENT				"6529"		/* Transfert via application Agent (mAgent) */
#define MCC_TRSFT_M_INDIVIDUAL			"0600"
#define MCC_HIGH_RISK_SECURITIES      	"6211"			/*IBO20190805 PROD00070403 Enh19.Q4 AN 2628*/

/*-- PROCESSING CODE LIST---*/
#define PC_PURCHASE                "00"
#define PC_WITHDRAWAL              "01"
#define PC_BALANCE_INQUIRY         "31"
#define PC_SHORT_STATEMENT         "38"
#define PC_PIN_CHANGE              "91"

/*-- Countries list --*/
#define USA               	  "840"
#define CANADA                "124"

/*----------YES OR NO FLAGS ---------*/
#define YES_FLAG                         1
#define NO_FLAG                          0


#define	LINE_DISCONECT				13
#define	LINE_INTR					14
#define	LINE_TIMEOUT				15



/*Macros to backup the tlvbuffer, mask sensitive data and restore the data after the DB call*/
#define P7_DB_PREPROC_TLV_BUF(a)		{																					\
										char loc_tlv_buffer_x[LG_MAX];														\
										char loc_data_x[BUF_LG_MAX];														\
										int  loc_nLength_x;																	\
										TSTlvPrivate        loc_sTlvPrivateInfo;											\
										TSTlvPrivate        loc_sOrigTlvPrivateInfo;										\
										int					loc_isF048_chg;													\
										int					loc_isF055_chg;													\
										loc_isF048_chg = 0;																	\
										loc_isF055_chg = 0;																	\
										tlvcpy(loc_tlv_buffer_x,a,strlen(a));												\
										if( GetTlvBuffer(AUTO_PIN_DATA, a, loc_data_x, &loc_nLength_x) == SUCCESS) {		\
											memset(loc_data_x,'X',loc_nLength_x);											\
											PutTlvBuffer(AUTO_PIN_DATA, a, loc_data_x, loc_nLength_x);						\
										}																					\
										if( GetTlvBuffer(AUTO_TRACK2_DATA, a, loc_data_x, &loc_nLength_x)== SUCCESS) {		\
											MaskTrack2Data(loc_data_x,"011011");											\
											PutTlvBuffer(AUTO_TRACK2_DATA, a, loc_data_x, strlen(loc_data_x));				\
										}																					\
										if( GetTlvBuffer(AUTO_ADD_DATA_PRIVATE, a, loc_data_x, &loc_nLength_x) == SUCCESS) {				\
											InitTlvPrivate(&loc_sTlvPrivateInfo);															\
											AnalyseTlvPrivate( loc_data_x ,loc_nLength_x,&loc_sTlvPrivateInfo);								\
											if(GetTlvPrivate( CVC2_DATA_TAG ,&loc_sTlvPrivateInfo ,loc_data_x ,&loc_nLength_x)== SUCCESS)	\
											{																								\
												loc_isF048_chg = 1;																			\
												memset(loc_data_x,'X',loc_nLength_x);														\
												PutTlvPrivate (CVC2_DATA_TAG, &loc_sTlvPrivateInfo, loc_data_x, loc_nLength_x);				\
											}	                                                                                            \
											if(GetTlvPrivate( PVV_OFFSET_TAG ,&loc_sTlvPrivateInfo ,loc_data_x ,&loc_nLength_x)== SUCCESS)	\
											{																								\
												loc_isF048_chg = 1;																			\
												memset(loc_data_x,'X',loc_nLength_x);														\
												PutTlvPrivate (PVV_OFFSET_TAG, &loc_sTlvPrivateInfo, loc_data_x, loc_nLength_x);			\
											}																								\
											if( loc_isF048_chg == 1 )																		\
											{																								\
												loc_nLength_x = PrivateBuildTlv (loc_data_x, &loc_sTlvPrivateInfo);							\
												PutTlvBuffer(AUTO_ADD_DATA_PRIVATE, a, loc_data_x, loc_nLength_x);							\
											}																								\
										}																									\
										if( GetTlvBuffer(AUTO_ICC_DATA, a, loc_data_x, &loc_nLength_x) == SUCCESS) {				\
											InitTlvPrivate(&loc_sTlvPrivateInfo);															\
											AnalyseTlvPrivate( loc_data_x ,loc_nLength_x,&loc_sTlvPrivateInfo);								\
											if(GetTlvPrivate( CHIP_TRACK2_TAG ,&loc_sTlvPrivateInfo ,loc_data_x ,&loc_nLength_x)== SUCCESS)	\
											{																								\
												loc_isF055_chg = 1;																			\
												memset(loc_data_x,'X',loc_nLength_x);														\
												PutTlvPrivate (CHIP_TRACK2_TAG, &loc_sTlvPrivateInfo, loc_data_x, loc_nLength_x);				\
											}																								\
											if( loc_isF055_chg == 1 )																		\
											{																								\
												loc_nLength_x = PrivateBuildTlv (loc_data_x, &loc_sTlvPrivateInfo);							\
												PutTlvBuffer(AUTO_ICC_DATA, a, loc_data_x, loc_nLength_x);							\
											}																								\
										}																					


#define P7_DB_POSTPROC_TLV_BUF(a)																										\
										if( GetTlvBuffer(AUTO_PIN_DATA, loc_tlv_buffer_x, loc_data_x, &loc_nLength_x) == SUCCESS) {		\
											PutTlvBuffer(AUTO_PIN_DATA, a, loc_data_x, loc_nLength_x);									\
										}																								\
										if( GetTlvBuffer(AUTO_TRACK2_DATA, loc_tlv_buffer_x, loc_data_x, &loc_nLength_x)== SUCCESS) {	\
											PutTlvBuffer(AUTO_TRACK2_DATA, a, loc_data_x, loc_nLength_x);								\
										}																								\
										if( loc_isF048_chg == 1 )																		\
										{																								\
											InitTlvPrivate(&loc_sTlvPrivateInfo);																\
											InitTlvPrivate(&loc_sOrigTlvPrivateInfo);															\
											if( GetTlvBuffer(AUTO_ADD_DATA_PRIVATE, loc_tlv_buffer_x, loc_data_x, &loc_nLength_x) == SUCCESS) {	\
												AnalyseTlvPrivate( loc_data_x ,loc_nLength_x,&loc_sOrigTlvPrivateInfo);							\
											}																									\
											if( GetTlvBuffer(AUTO_ADD_DATA_PRIVATE, a, loc_data_x, &loc_nLength_x) == SUCCESS) {				\
												AnalyseTlvPrivate( loc_data_x ,loc_nLength_x,&loc_sTlvPrivateInfo);								\
											}																									\
											if(GetTlvPrivate( CVC2_DATA_TAG ,&loc_sOrigTlvPrivateInfo ,loc_data_x ,&loc_nLength_x)== SUCCESS)	\
											{																									\
												PutTlvPrivate (CVC2_DATA_TAG, &loc_sTlvPrivateInfo, loc_data_x, loc_nLength_x);					\
											}	                                                                                                 \
											if(GetTlvPrivate( PVV_OFFSET_TAG ,&loc_sOrigTlvPrivateInfo ,loc_data_x ,&loc_nLength_x)== SUCCESS)	\
											{																									\
												PutTlvPrivate (PVV_OFFSET_TAG, &loc_sTlvPrivateInfo, loc_data_x, loc_nLength_x);				\
											}																									\
											loc_nLength_x = PrivateBuildTlv (loc_data_x, &loc_sTlvPrivateInfo);									\
											PutTlvBuffer(AUTO_ADD_DATA_PRIVATE, a, loc_data_x, loc_nLength_x);									\
											InitTlvPrivate(&loc_sTlvPrivateInfo);																\
											InitTlvPrivate(&loc_sOrigTlvPrivateInfo);															\
										}																										\
										if( loc_isF055_chg == 1 )																		\
										{																								\
											InitTlvPrivate(&loc_sTlvPrivateInfo);																\
											InitTlvPrivate(&loc_sOrigTlvPrivateInfo);															\
											if( GetTlvBuffer(AUTO_ICC_DATA, loc_tlv_buffer_x, loc_data_x, &loc_nLength_x) == SUCCESS) {	\
												AnalyseTlvPrivate( loc_data_x ,loc_nLength_x,&loc_sOrigTlvPrivateInfo);							\
											}																									\
											if( GetTlvBuffer(AUTO_ICC_DATA, a, loc_data_x, &loc_nLength_x) == SUCCESS) {				\
												AnalyseTlvPrivate( loc_data_x ,loc_nLength_x,&loc_sTlvPrivateInfo);								\
											}																									\
											if(GetTlvPrivate( CHIP_TRACK2_TAG ,&loc_sOrigTlvPrivateInfo ,loc_data_x ,&loc_nLength_x)== SUCCESS)	\
											{																									\
												PutTlvPrivate (CHIP_TRACK2_TAG, &loc_sTlvPrivateInfo, loc_data_x, loc_nLength_x);					\
											}																									\
											loc_nLength_x = PrivateBuildTlv (loc_data_x, &loc_sTlvPrivateInfo);									\
											PutTlvBuffer(AUTO_ICC_DATA, a, loc_data_x, loc_nLength_x);									\
											InitTlvPrivate(&loc_sTlvPrivateInfo);																\
											InitTlvPrivate(&loc_sOrigTlvPrivateInfo);															\
										}																									\
										memset(loc_tlv_buffer_x,'\0',sizeof(loc_tlv_buffer_x));													\
										memset(loc_data_x,'\0',sizeof(loc_data_x));																
										


#define P7_DB_EXCP_POSTPROC_TLV_BUF(a)			P7_DB_POSTPROC_TLV_BUF(a)																		\
												}


#endif /* HPS_DEFINE_H */
