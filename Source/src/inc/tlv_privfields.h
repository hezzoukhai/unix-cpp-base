/****************************************************************
*   Modification spec Avril 2006                                *
*   Elyajizi Mehdi 17/02/06                                     *
*   Ref : ME170206 :Chip enhancements.
*   YK  : YK19052006 Visa & MasterCard E-commerce
****************************************************************/

#ifndef HPS_TLV_PRIVFIELDS_H
#define HPS_TLV_PRIVFIELDS_H

/*--------------------------------------------------------------------------*/
/*  PROCESSING CODE TAGS													*/
/*  UNDER: AUTO_PROC_CODE     	      										*/
/*--------------------------------------------------------------------------*/
#define PROC_TRANS_TYPE                 "T01"
#define PROC_ACCOUNT_FROM               "T02"
#define PROC_ACCOUNT_TO                 "T03"

/*--------------------------------------------------------------------------*/
/*  PRIVATE DATA TAGS  44	     											*/
/*  UNDER: PRIVATE_DATA_44     	      										*/
/*--------------------------------------------------------------------------*/
#define BAL_TYPE_TAG               	    "A09"
/*--------------------------------------------------------------------------*/
/*  PRIVATE DATA TAGS  48	     											*/
/*  UNDER: AUTO_PRIVATE_DATA  	      										*/
/*--------------------------------------------------------------------------*/
#define FILE_UPDATE_ERR_CODE_TAG        "P01"
#define MESSAGE_TEXT_TAG             	"P02"
#define HPS_LIMITS_CRNCY_TAG 			"P03"
#define HPS_LIMITS_AMOUNT_TAG 			"P04"
#define HPS_HOST_SCENARIO_CODE  		"P05"
#define CHECK_SET_ON_OFF_LIMIT			"P06"
#define SPEC_LIMIT_ID      				"P07"
#define ORIG_BILLING_INFO      			"P08"
#define ACQUIRER_BANK_TAG				"P09"
#define HPS_ACQUIRER_BANK_TAG           ACQUIRER_BANK_TAG
#define ENCR_PIN_BLOCK_KEY_TAG 		    "P10"
/*#define KEY_EXCHANGE_BLOC_DATA_TAG 	    "P11"*//*EBE: not on internal ISS*/
#define ORIGINAL_TRX_ID					"P11"	/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
#define PRIS_INTERNAL_ID_TAG           	"P12"
#define EXTERNAL_STAN_TAG              	"P13"
#define EVENT_ACTION					"P14"
#define PVT_DATA_TAG               	    "P15"	/* This tag is activitaed to be reserved now is not yet filled by interfaces */
#define PVV_OFFSET_TAG               	"P16"
#define	RRN_RETREVIAL_TAG				"P17"
#define PAYMENT_ACC_REFERENCE			"P18"	/*AMER20170302: PROD00038916 MCI Enh17.Q2*/
#define MVV_TAG               		    "P19"
#define ICVV_RESULT_CODE_TAG			"P20"
#define MERCHANT_ADDITIONAL_DATA 		"P21"	 /* AHA20161214  PROD00036000*/
#define DYNAMIC_NBR_RESULT_CODE_TAG		"P22"   /* RFU */
#define DAC_RESULT_CODE_TAG				"P23"   /* RFU */
#define CMC7_LINE_TAG					"P24"
#define CRYPT_RESULT_CODE_TAG			"P25"
#define AUC_RESULT_CODE_TAG				"P26"	 /*** AHA20161214 ***/   /* RFU */
#define PIN_RESULT_CODE_TAG				"P27"			/*AMER20170801 PROD00042216: P28 turned into P27*/
#define EXT_SECURITY_VER_RESULTS_TAG	"P28"			/*AMER20170801: PROD00042216*/
#define SECURITY_VER_RESULTS_TAG        "P29"                                              


#define CHIP_AUTH_ADD_INFO				"P30"
/*
#define ERRONEOUS_FIELD					"P30"
#define FIELD_CONVERSION				"P31"
*/
#define MCI_DEVICE_TYPE_IND             "P31"
#define MCI_ASSIGNED_ID                 "P32"
#define MCI_PAN_MAPPING_INFO            "P33"
#define MCI_ON_BEHALF_SERV              "P34"
#define MCI_TRACEID						"P35"

#define ISSUER_BANK						"P36"
/*
#define CH_BILLING_ADDR					"P37"
#define CH_BILLING_ADDR_ZIPCODE			"P38"
*/

#define EXT_SECURITY_RESULTS_DTL		"P38" /*AMER20190313 PROD00066569: CBAE Enh5.00 989*/

#define MERCHANT_BANK_CODE				"P39"


#define E_COMMERCE_CERTIFICATE_TAG 	    "P40"
#define MERCHANT_ACRONYM                "P41" /*ICH Added for HSID*/

#define SECURITY_LEVEL_IND_TAG 		    "P42"
#define UCAF_TAG 		    	        "P43"
#define EXTERNAL_CRYPT_RESULTS     	    "P44"
#define EXTERNAL_PROCESS_INDICATOR      "P45"

#define ACQUIRER_NETWORK_CODE			"P46"	
#define ROUTING_NETWORK_CODE			"P47"
#define TCC_TAG                 	    "P48"
#define	KSN_DESC_TAG					"P49"
#define	KSN_TAG							"P50"
#define AAV_TAG							"P51"


#define DIGITAL_PAYMENT_DATA            "P52"    
#define ATM_DEPOSIT_NOTES               "P53"    

#define ECOM_AAV_TAG					AAV_TAG
#define AAV_RESULT						"P54"
#define TOKEN_DATA						"P55"
#define DCC_DATA						"P56"
#define AFD_REF_NBR                     "P57"/*ICH2017 for AFD*/
#define ACC_PT_CARD_PRD_ID				"P58"
#define OPERATION_CODE					"P59"

#define CAVV_RESULT_CODE_TAG	        AAV_RESULT
#define CAVV_TAG            			AAV_TAG

#define WATCH_LIST_SCORING_RES			"P60"
#define POS_DATA_EXT_TAG				"P61"

/*EBE130517: Evertec */

#define FEES_AMOUNT						"P62"
#define AUTHORIZED_AMOUNT				"P63"
/*
#define LEGAL_TIPS_AMOUNT				"P63"
*/
#define AUTH_ADD_INFO					"P64"
#define PAYMENT_AUTH_INFO				"P65"
#define ECOM_AUTH_INFO					"P66"

#define AUTHORIZATION_IDENTIFIER		"P67"
#define TRANSACTION_IDENTIFIER			"P68"	/* any transaction identifier e.g TID for Visa CPS, trace ID for MCI...etc */

#define ACTUAL_OUTLET_NUMBER_TAG        "P69"
#define TERM_INFO_DATA					"P70"

#define RECURRING_FLAG_TAG              "P71"   /*IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
/*#define CREDNTL_ON_FILE_TYPE            "P71" */     
#define FEES_AMOUNTS_DTL				"P72"

#define FEES_ID_INFO					"P73"

/*Start SLA051012 ENH12.2 Vme Trx */
#define ADD_AUTH_METHOD_TAG                 "P73"
#define ADD_AUTH_RES_CODE_TAG               "P74"
#define ACCOUNT_RESULT_TAG                  "P75"   /*"P72"  waiting confirmation*/
/*End SLA051012 ENH12.2 Vme Trx */

#define MC_ELEC_ACCPT_IND_TAG 		    "P76"
#define TRANS_XID_TAG                   "P77"
#define TRANS_STAIN_TAG                 "P78"
#define SETTLEMENT_DATE_TAG             "P79"

#define DINERS_NET_REF_ID               "P80"
#define CARD_ISS_DATA    		        "P81"
#define ADDRESS_VERIF_REQ_TYPE_TAG 	    "P82"
#define ADDRESS_VERIF_REP_TYPE_TAG 	    "P83"
#define CVM_DATA						"P84"
#define EXTERNAL_RESP_SOURCE            "P85"
#define FRD_CTRL_RESULT_TAG				"P86"
#define CVV_RESULT_CODE_TAG             "P87"
#define CVV2_RESULT_CODE_TAG            "P88"
#define CVC1_TRACK_EDIT_ERROR_TAG 	    "P89"
#define FRAUD_SCORE						"P90"
#define AMEX_TRX_ID              		"P91"		/*AMER20170919 PROD00046952: Added for AMEX F31 (Acquirer Reference Data)*/
#define CVC2_DATA_TAG                   "P92"
#define HSM_RESULT_CODE_TAG            	"P93"
#define SECONDARY_RESOURCE_TAG          "P94"
#define NETWORK_CODE					"P95"
#define MALFUNCTION_STIP_TAG            "P96"
#define NAT_INTERNAT_FLAG               "P97"
#define HPS_MARKET_SPECIFIC				"P98"
#define POSTILION_ROUTING_INFO			"R01" /* MRY HFC 050723 */
#define HPS_DURATION_TAG				"P99"
#define DYNAMIC_AUTHENTICATION			"K14"



#define ATM_LANG_REQ                    "B01"    
#define CARDLESS_TYPE                   "B02"        /* 0 - Not Cardless, 1- Card less with account Entry, 2 - Card less with card Entry */
#define AUTH_REQUESTED_AMOUNT			"B03"		/* Iso Format */

#define P7_F048_CLT_NOT_CRDL			'0'
#define P7_F048_CLT_CRDL_ACC_ENT		'1'
#define P7_F048_CLT_CRDL_PAN_ENT		'2'


#define NET_POS_ENTRY_MODE              "022"           /*AMER20171218 PROD00050359*/
#define ORIGINAL_FUNCTION_CODE          "024"           /*AEA20210712 Enh21.Q4 AN4779*/
#define NET_REFERENCE_NUMBER            "037"  /*HAL 20201303 Enh Q2.2020 Art 1.1 */

#define ISS_AUTH_OPT					"O01" /* CARD_MANAGED(1) + CREDIT_BAL_OPT(1) VEL_LIMIT_CHK_OPT(1)*/
#define LIMIT_ID_INFO					"O02"
#define SPEC_LIMIT_ID_INFO				"O03"


#define ROUTING_SEQUENCE_ID					"Y04"   /*tag for the seq of the selected route*//* HAL04092020 PLUTONL-2197*/
#define C_MSG_FLOW_ID                       "Y10"      /*SKARROUMI PLUTONL-4900*/
#define BR_ROUTING_PATH                      "K21"       /*SKA20220822 PLUTONL-5086 */
#define ROUTING_PATH                    "K22"       /*SKA20220922 PLUTONL-5106*/

#define	CB_DATA								"G01"

#define SMS_CHG_DATA      	  	 	   		"K01"	/*ICH Added for HSID*/
#define PARTIAL_AMOUNT_TYPE					"K02"	/*AMER20170308: PROD00039552*/
#define ROUTING_CODE						"K03"	/*YDA20171113 (PROD00048309)*/
#define SMS_VCR_DATA        	    		"K04"	/*ICH17012018*/
#define	RESUBMISSION_COUNTER				"K05"	/* MTR20180410 CBAE Enh4.12 */
#define DIR_SERV_TRANS_ID					"K06"   /* Directory Server Transaction ID*/
#define CASH_BACK_AMNT_IN_LIMIT_CRY			"K07"
#define CASH_BACK_LIMIT_ID					"K08"   
/*ZKO29122021 Start PLUTONL-3345*
#define AUTH_EXEMPTION_IND					"K09"   /*Authorization exemption indicator*/ 	/*AMER20190314 PROD00066569: CBAE Enh5.00 1075*/
#define MOB_PAY_SOLUTION_ID					"K10"   /*Mobile payment solution identifier*/ 	/*AMER20190425 PROD00066569: CBAE Enh5.00 1050*/
#define FRAUD_CASE_SEQ_ID					"K11"  
#define FLEET_CTRL_FLAGS					"K12" 
#define FLEET_CTRL_RES						"K13"  
#define DYNAMIC_AUTHENTICATION				"K14"  
#define CORPORATE_ID						"K15"  
#define AUTH_MSG_UUID						"K16"  
#define ACS_TRANS_ID						"K17"   /*ACS Transaction ID*/ 	/*AMER20190314 PROD00066569: CBAE Enh5.00 1073*/
#define NET_ISS_SETTL_INFO				    "K18"  
#define NET_ACQ_SETTL_INFO				 	"K19"  

#define	CUP_DS_EXPIRY_DATE					"U01"   /*DS TRANSACTION ID (Expiry date)*/

/*Start AMER20210402 PLUTONL-2947*/
#define PRIV_ORIG_DATA_ELMNT				"K20"	
#define P7_K20_ORIG_DATA_STRATEGIES			"001"	
#define P7_K20_ORIG_DATA_ELMNT				"002"

#define P7_K20_002_CARD_NUMBER		     	"002"
#define P7_K20_007_ORIG_XMIT_TIME			"007"
#define P7_K20_011_ORIG_STAN				"011"
#define P7_K20_032_ORIG_ACQ_ID 				"032"
#define P7_K20_038_AUTHOR_ID 				"038"
#define P7_K20_042_CARD_ACCEPT_ID 			"042"
#define P7_K20_200_ORIG_BANK_REF		    "200"
/*End AMER20210402 PLUTONL-2947*/

#define SOURCE_POS_ENTRY_MODE               "K30"   /*Pos entry mode of the channel who captured the message*/ /*AMER20210925 PLUTONL-3607*/
/*Start PLUTONL-4280 :Enh5.03 :1348*/
#define REATTEMPT_FROZEN_PERIOD             "K40"   /*Reattempt frozen period*/
#define REATTEMPT_CONDITIONS                "K41"   /*Reattempt conditions*/
/*End PLUTONL-4280 :Enh5.03 :1348*/

/*Start IBO20221121 PLUTONL-5416*/
#define ORIGIN_CODE                			"K50"   /*Origine Code*/
#define BANK_XMIT_TIME_TAG                	"K51"	/*BANK_XMIT_TIME*/
/*End IBO20221121 PLUTONL-5416*/

#define INTERNAL_DATA                       "K99"   /*INTERNAL DATA for velocity CNP (SID)*//*NAB02042021 PLUTONL-2974*/

/*HAL 20202003 ENHS 2020Q2 ART 9.3.3*/
#define P7_F048_EXMP_RES_TRA                     0
#define P7_F048_EXMP_RES_LOW_VAL                 1
#define P7_F048_EXMP_RES_TRST_MER                2
#define P7_F048_EXMP_RES_DELG_AUTH               3	/*AEA20210712 Enh21.Q4 Art9.3.3*/
#define P7_F048_EXMP_RES_SCP                     4 /*PLUTONL-6119 Enh 2023Q2 Visa 9.3.1 AHA20230403*/

/*Start ACH20191021  PLUTONL-500 */
#define P7_F048_P38_NETWORK				    0

#define P7_F048_P38_VISA_CHK_CVV1		    1
#define P7_F048_P38_VISA_CHK_ICVV       	2		
#define P7_F048_P38_VISA_CHK_DCVV       	3		
#define P7_F048_P38_VISA_CHK_CVV2       	4
#define P7_F048_P38_VISA_CHK_DCVV2       	5
#define P7_F048_P38_VISA_CHK_ARQC       	6
#define P7_F048_P38_VISA_CHK_AVS        	7
#define P7_F048_P38_VISA_CHK_CAVV         	8
#define P7_F048_P38_VISA_LEN				9

#define P7_F048_P38_MCI_CHK_CVC1		    1
#define P7_F048_P38_MCI_CHK_PIN             2		
#define P7_F048_P38_MCI_CHK_ARQC            4		
#define P7_F048_P38_MCI_CHK_CVC3            6
#define P7_F048_P38_MCI_CHK_AAV             7
#define P7_F048_P38_MCI_CHK_MDES_ARQC       8
#define P7_F048_P38_MCI_CHK_MDES_ATC        9
#define P7_F048_P38_MCI_CHK_MDES_CVC3       10
#define P7_F048_P38_MCI_CHK_ISS_AVS         11
#define P7_F048_P38_MCI_CHK_ISS_CVC2        12
#define P7_F048_P38_MCI_CHK_ISS_ICVV        13
#define P7_F048_P38_MCI_CHK_ISS_CVC1        14
#define P7_F048_P38_MCI_CHK_ISS_CVC3        15
#define P7_F048_P38_MCI_CHK_DTVC			16 /* ZKO 22072022 PLUTONL-4902 ENH 22.Q4 MCI AN 6039*/
#define P7_F048_P38_MCI_LEN				    17/*16-->17*//*ZKO 22072022 PLUTONL-4902 ENH 22.Q4 MCI AN 6039*/
#define P7_F048_P38_LEN                     32


/*End ACH20191021  PLUTONL-500 */

/*Start AMER20210927 PLUTONL-3612*/
#define P7_F048_P38_CUP_CHK_CVV1		1
#define P7_F048_P38_CUP_CHK_ICVV       	2		
#define P7_F048_P38_CUP_CHK_CVC3       	3		
#define P7_F048_P38_CUP_CHK_CVV2       	4
#define P7_F048_P38_CUP_CHK_ARQC       	5
#define P7_F048_P38_CUP_CHK_PVV       	6
#define P7_F048_P38_CUP_LEN				7

#define P7_F048_P38_JCB_CHK_CVV2		1
#define P7_F048_P38_JCB_CHK_AVS       	2		
#define P7_F048_P38_JCB_LEN				3
/*End AMER20210927 PLUTONL-3612*/


#define P7_F048_P56_DCC_APP_FLAG_LEN		1
#define P7_F048_P56_RATE_LEN				10
#define P7_F048_P56_RATE_DATE_LEN			4
#define P7_F048_P56_DCC_AMOUNT_LEN			12
#define P7_F048_P56_DCC_CCY_LEN				3

#define P7_F048_P56_DCC_APP_FLAG_OFST		0
#define P7_F048_P56_RATE_OFST				1
#define P7_F048_P56_RATE_DATE_OFST			11
#define P7_F048_P56_DCC_AMOUNT_OFST			15
#define P7_F048_P56_DCC_CCY_OFST			27

/*POS_DATA_EXT_TAG*/
#define P7_F048_P61_PARTIAL_APP_OFST		0
#define P7_F048_P61_PURCHASE_CB_APP_OFST	1
#define P7_F048_P61_TERM_CLASS_OFST			2
#define P7_F048_P61_RFU_4_OFST				3
#define P7_F048_P61_RFU_5_OFST				4
#define P7_F048_P61_RFU_6_OFST				5
#define P7_F048_P61_RFU_7_OFST				6
#define P7_F048_P61_RFU_8_OFST				7
#define P7_F048_P61_SINGLE_TAP_PIN_TERM_IND		19
#define P7_F048_P61_MPOS_IND_OFST			20

/*Start: ZKO: PROD00058820*/
#define P7_F048_P61_POS20_MPOS_TERM                    '1' /*Dedicated mPOS Terminal with PCI compliant dongle*/
#define P7_F048_P61_POS20_OFF_MDEVICE                  '2'  /*Off the Shelf Mobile Device*/
/*START PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
#define P7_F048_P63_RFU_5_OFST				4
/*END PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/


/*Program Protocol*/
/* START ELKAM20230202 ENH_2023Q2 AN 7090 */
#define P7_F048_P66_LEN                     21		/*AMER20190314 PROD00066569: CBAE Enh5.00 1075 (5 -> 18)*//*ZKO16082019 PROD00070403 (18 -> 20)*/ /* 20-->21 PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/
#define P7_F048_P66_POS2_3DSEC_UNKNOWN	    '0'      /*HAL PROD00073823 20202801 Enh Q2.2020 Art 3.10*/
#define P7_F048_P66_POS2_3DSEC_1_0			'1'
#define P7_F048_P66_POS2_3DSEC_2_0			'2'

#define P7_F048_P66_POS2_3DSEC_2_1          '3'
#define P7_F048_P66_POS2_3DSEC_2_2          '4'
#define P7_F048_P66_POS2_3DSEC_2_3          '5'
#define P7_F048_P66_POS2_3DSEC_2_4          '6'
#define P7_F048_P66_POS2_3DSEC_2_5          '7'
#define P7_F048_P66_POS2_3DSEC_2_6          '8'
#define P7_F048_P66_POS2_3DSEC_2_7          '9'
#define P7_F048_P66_POS2_3DSEC_2_8          'A'
#define P7_F048_P66_POS2_3DSEC_2_9          'B'

#define P7_F048_P66_POS3_HMAC               '0'
#define P7_F048_P66_POS3_CVV                '1'
#define P7_F048_P66_POS3_CVV_ATN            '2'
#define P7_F048_P66_POS3_AAV_MC             '3'
#define P7_F048_P66_POS3_GEN_BY_WLT         'W'


#define P7_F048_P66_POS5_MISSING_AAV        '0'
#define P7_F048_P66_POS5_INVALID_AAV        '1'
#define P7_F048_P66_POS5_INVALID_SLI        '2'

#define P7_F048_P66_POS1_RFU_OFST			0   /* POS 1        : RFU */
#define P7_F048_P66_PROG_PROT_OFST			1   /* POS 2        : Program Protocol */
#define P7_F048_P66_CAVV_ALGO_OFST			2   /* POS 3        : CAVV algorithm */
#define P7_F048_P66_POS4_RFU_OFST			3   /* POS 4        : RFU */
#define P7_F048_P66_DOWN_REAS_OFST			4   /* POS 5        : Downgrade reason */
#define P7_F048_P66_PAYM_SCNA_OFST			5   /* POS 6-7      : Payment scenario */
#define P7_F048_P66_AUTH_MTHO_OFST			7   /* POS 8-9      : Authentication method: Represents "authenticationMethod" 3DS Data Element in EMV 3DS specs */
#define P7_F048_P66_AUTH_TYPE_OFST			9   /* POS 10       : Authentication type */
#define P7_F048_P66_3DS_CHALLENGE_IND_OFST  10  /* POS 11-12    : Requestor challenge indicator: Represents "threeDSRequestorChallengeInd" 3DS Data Element in EMV 3DS specs */
#define P7_F048_P66_REASON_TRX_STATUS_OFST  12  /* POS 13-14    : Transaction Status Reason: Represents “transStatusReason” 3DS Data Element in EMV 3DS specs */
#define P7_F048_P66_CHALLENGE_CANCEL_OFST   14  /* POS 15-16    : Challenge Cancelation Indicator: Represents “challengeCancel” 3DS Data Element in EMV 3DS specs */
#define P7_F048_P66_3DS_SCORE_OFST          16  /* POS 17-18    : Score */
#define P7_F048_P66_POS19_RFU_OFST			18  /* POS 19       : RFU */

/*Start ZKO16082019 PROD00070403*/
#define P7_F048_P66_DELG_AUTH_OFST			19  /* POS 20       : Delegated Authentication Indicator */
/*End   ZKO16082019 PROD00070403*/
#define P7_F048_P66_AUTHEN_FRMWRK_OFST      20 /*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/
#define P7_F048_P61_LEN						21
#define P7_F048_P66_LEN                     21		/*AMER20190314 PROD00066569: CBAE Enh5.00 1075 (5 -> 18)*//*ZKO16082019 PROD00070403 (18 -> 20)*/ /* 20-->21 PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/


/*Start AMER20190314 PROD00066569: CBAE Enh5.00 1075*/
#define P7_F048_P66_PAYM_SCNA_LEN			     2 
#define P7_F048_P66_AUTH_MTHO_LEN			     2 
#define P7_F048_P66_3DS_CHALLENGE_IND_LEN		 2
#define P7_F048_P66_REASON_TRX_STATUS_LEN		 2
#define P7_F048_P66_CHALLENGE_CANCEL_LEN		 2
#define P7_F048_P66_3DS_SCORE_LEN		 	     2
/*End AMER20190314 PROD00066569: CBAE Enh5.00 1075*/
#define P7_F048_K06_LEN                      36
/*END:ZKO:PROD00058820*/

#define P7_F048_P61_LEN						21

/* END ELKAM20230202 ENH_2023Q2 AN 7090 */

/*AUTH_ADD_INFO*/
#define P7_F048_P64_TCC_OFST				0
#define P7_F048_P64_FINAL_AUTH_IND_OSFT		1
#define P7_F048_P64_CRYPTO_CURR_OFST		2		/*AMER20190219 PROD00065991: Enh19.Q2 Art2.1 (VISA)*/
#define P7_F048_P64_INITIATING_PARTY		3		/*IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
#define P7_F048_P64_PIN_REQ_SINGLE_TAP_IND	4
#define P7_F048_P64_RESP_TO_PIN_REQ_IND		5
/*#define P7_F048_P64_RFU_7_OFST			6*/    /*ZKO 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define P7_F048_P64_CRDNTL_REASON           6     /*ZKO 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define P7_F048_P64_TOKEN_AUTH_OSFT			7
#define P7_F048_P64_REF_ORIGINAL_AUTH_OSFT  8
#define P7_F048_P64_REF_PURPOSE_TRANSACTION 9	/*AEA20210712 Enh21.Q4 AN4972*/
#define P7_F048_P64_REATTEMPT_INDICATOR     10  /*PLUTONL-4280 :Enh5.03 :1348*/
#define P7_F048_P64_CRDNTL_MER_RELATIONSHIP_IND     11  /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/
#define P7_F048_P64_LEN						12	/*11-->12   IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/ /*AEA20210712 Enh21.Q4 AN4972*//*PLUTONL-4280 :Enh5.03 :1348*/


/*Start IBO20190719 PROD00070403 Enh19.Q4 Art 9.1.3*/
/*AUTH_EXEMPTION_IND*/
/*ZKO29122021 Start PLUTONL-3345: changer K09 par S27_001 */
#define P7_F048_S27_001_RFU_1_OFST				0
#define P7_F048_S27_001_RECURRING_PYMT 			1
#define P7_F048_S27_001_DELG_AUTH			    2 /*AEA20210712 Enh21.Q4 Art9.3.3*/
#define P7_F048_S27_001_AUT_OUTAGE_EXCEPTION	3 /*AEA20210712 Enh21.Q4 Art9.1.2*/
#define P7_F048_S27_001_LOW_VAL_EXEM			4
#define P7_F048_S27_001_TRA						5
#define P7_F048_S27_001_TRST_MER_EXEM			6
#define P7_F048_S27_001_SCP						7
#define P7_F048_S27_001_LEN						16
/*ZKO29122021 Start PLUTONL-3345*/
/*End IBO20190719 PROD00070403 Enh19.Q4 Art 9.1.3*/
/*Start PLUTONL-4280 :Enh5.03 :1420*/
#define P7_F048_P84_01_UNKNOWN				        0    /*Unknown*/
#define P7_F048_P84_02_NONE			    	        1    /*None*/
#define P7_F048_P84_03_SIGNATURE			        2    /*Signature*/
#define P7_F048_P84_04_ONLINE_PIN			        3    /*Online PIN*/
#define P7_F048_P84_05_OFF_PIN_PLAINTEXT	        4    /*Offline PIN Plaintext*/
#define P7_F048_P84_06_OFF_PIN_ENCIPHERED	        5    /*Offline PIN Enciphered*/
#define P7_F048_P84_07_CARDHOLDER_DEVICE_CODE	    6    /*Cardholder device code */
#define P7_F048_P84_08_FINGERPRINTE_BIOMETRIC	    7    /*Fingerprint biometric verified by cardholder device */
#define P7_F048_P84_09_CARDHOLDERE_DEVICE_PATTERN	8    /*Cardholder device pattern*/

/*End  PLUTONL-4280 :Enh5.03 :1420*/

/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
#define P7_F063_N23_CAVV_VERSION_NBR_OFST                            0
#define P7_F063_N23_CAVV_TYPE_OFST                                   2
#define P7_F063_N23_IP_ADDR_VELC_CNT_OFST                            4       /*VELOCITY_COUNT = VELC_CNT*/
#define P7_F063_N23_DEVICE_ID_VELC_CNT_OFST                          6
#define P7_F063_N23_VISA_RISK_BASED_AUTN_SCORE_OFST                  8            /*AUTHENTICATION=AUTN*/

#define P7_F063_N23_CAVV_VERSION_NBR_LEN                             2
#define P7_F063_N23_CAVV_TYPE_LEN                                    2
#define P7_F063_N23_IP_ADDR_VELC_CNT_LEN                             2
#define P7_F063_N23_DEVICE_ID_VELC_CNT_LEN                           2
#define P7_F063_N23_VISA_RISK_BASED_AUTN_SCORE_LEN                   2

/*End PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/

#define	UPDATE_FILE_DATA					"D90"
#define	FUND_TRANSFER_DATA					"D91"
#define	CB_SERVICES_DATA					"D92"
#define	CB_RESERVED_DATA					"D93"
/****************  CB2A TELECOLLECTE  *******SWI12112013*****/


/*--------------------------------------------------------------------------*/
/*  ICC DATA TAGS   														*/
/*  UNDER: AUTO_ICC_DATA  	      											*/
/*--------------------------------------------------------------------------*/
#define CHIP_APP_CRYPTOGRAM_TAG    		"C01"
#define CHIP_CRYPTO_INFO_DATA_TAG  		"C02"
#define CHIP_TRANS_CUR_CODE_TAG    		"C03"
#define CHIP_TRANS_DATE_TAG        		"C04"
#define CHIP_TRANS_TYPE_TAG        		"C05"
#define CHIP_TRANS_AMOUNT_TAG      		"C06"
#define CHIP_ATC_TAG               		"C07"
#define CHIP_UNPRED_NUMBER_TAG     		"C08"
#define CHIP_CATEGORY_CODE_TAG     		"C09"
#define CHIP_TVR_TAG               		"C10"
#define CHIP_TERM_COUNTRY_CODE_TAG 		"C11"
#define CHIP_AIP_TAG               		"C12"
#define CHIP_CVM_RESULTS_TAG       		"C13"
#define CHIP_TERM_CAPABILITY_TAG   		"C14"
#define CHIP_TERM_TYPE_TAG         		"C15"
#define CHIP_APPLI_ID_TAG          		"C16"
#define CHIP_APPLI_VER_TAG         		"C17"
#define CHIP_IFD_SERIAL_NUM_TAG    		"C18"
#define CHIP_TRX_SEQ_COUNT_TAG     		"C19"
#define CHIP_ISS_APPLI_DATA_TAG    		"C20"
#define CHIP_OTHER_AMOUNT_TAG      		"C21"
#define CHIP_DERIV_KEY_INDEX_TAG   		"C22"
#define CHIP_ISS_AUTHEN_DATA_TAG   		"C23"
#define CHIP_ISS_ARPC_RESP_CODE_TAG		"C24"
#define CHIP_ISS_SCRIPT_RESULT_TAG      "C25"
#define CHIP_APP_SELECTED_TAG			"C26"
#define CHIP_TRACK2_TAG					"C27" 
#define CHIP_PAN_TAG					"C28"
#define CHIP_APP_NAME_TAG				"C29"
#define CHIP_APP_LABEL_TAG				"C30"
#define CHIP_SECUR_ATT_TAG				"C31"
#define CHIP_ISS_SCRIPT_1               "C32"
#define CHIP_ISS_SCRIPT_2               "C33"
#define CHIP_PAN_SEQ_NBR                        "C34"


#define    CHIP_ISS_SCRIPT_CMD      	"C36"
#define    CHIP_TERM_CAP            	"C37"
#define    CHIP_APP_SEQ_NUMBER      	"C38"
#define	   CHIP_CVM_LIST				"C39"
#define	   CHIP_APPL_EFF_DATE			"C40"
#define	   CHIP_APPL_EXP_DATE			"C41"

#define CHIP_SEC_PIN_DATA               "C43"	/* ACK PIN CHANGE MARCH2015 */


#define CHIP_TAG_KERNEL_ID              "C60"
#define	CHIP_TAG_ICC_PROCESSING_RESULT	"C61"
#define	CHIP_TAG_APPL_TYPE				"C62"
#define	CHIP_TAG_RTT_RESULT				"C63"
#define	CHIP_TAG_CONTACTLESS_DEVICE		"C64"
#define	CHIP_TAG_EMV_ACQ_MODE			"C65"
#define	CHIP_TAG_CARD_AUTH_RES_CODE		"C66"



#define CHIP_TAG_TRACK_1_DATA			"C71"
#define CHIP_FFI_TAG                    "C72"
#define CHIP_AUTHOR_RESP_CODE           "C73"
#define CHIP_DED_FILE_NAME_TAG			"C80"  
#define CHIP_APP_USE_CONTROL_TAG		"C81"
#define	   CHIP_IAC_DEFAULT				"C82"
#define	   CHIP_IAC_DENIAL				"C83"
#define	   CHIP_IAC_ONLINE				"C84"

#define CHIP_CVR						"C96" /*EBE170107*/
#define CHIP_ORIG_ARC					"C97" /*SNO060116*/
#define CHIP_CALC_ARC					"C98" /*ARC Feature ICH20191010*/

#define EMV_TAG_APP_RGSTD_DAT             "C99"        /* MTR - Enh 4.11 */
#define CHIP_T2_EQUIVALENT_DATA			CHIP_TRACK2_TAG



/*--------------------------------------------------------------------------*/
/*  CPS DATA TAGS   														*/
/*  UNDER: AUTO_CPS_DATA  	      											*/
/*--------------------------------------------------------------------------*/
#define CPS_ACI_TAG                     "F01"
#define CPS_TRANS_ID_TAG                "F02"
#define CPS_VALIDATION_DATE_TAG         "F03"
#define CPS_MARKET_SPECIFIC_TAG         "F04"
#define CPS_DURATION_TAG                "F05"
#define CPS_PRESTIGIOUS_IND_TAG         "F06"
#define CPS_MCI_INTER_TAG               "F17"
#define CPS_MVV_TAG           			"F19"
#define CPS_PRODUCT_ID_TAG         		"F23"
#define CPS_SPEND_QUALIFIED_INDICATOR   "F25" /* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.2*/





/*--------------------------------------------------------------------------*/
/*  NETWORK POS DATA   														*/
/*  UNDER: AUTO_        	      											*/
/*--------------------------------------------------------------------------*/
#define POS_TRANS_STATUS                "007"   /* ENH 11.1 OHD10052011 */  
#define POS_ENTRY_MODE_TAG              "022"
#define POS_CONDITION_CODE_TAG          "025"
#define RESPONSE_CODE_TAG               "039"
#define ADTNL_POS_DATA_TAG              "060"
#define ADTNL_POS_DATA1_TAG             "061"
#define FILE_UPDATE_CODE_TAG            "091"
#define ACTION_CODE_TAG                 "127"

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#define PAY_ACTIVATIONDEPOT_TAG              "M01"
#define PAY_BILLPAYMENT_TAG                  "M02"


/*--------------------------------------------------------------------------*/
/*  NETWORK DATA TAGS   													*/
/*  UNDER: AUTO_NETWORK_DATA  	   											*/
/*--------------------------------------------------------------------------*/
#define NET_BANKNET_DATA_TAG            "N00"
#define NET_NETWORK_ID_TAG              "N01"
#define NET_REFERENCE_ID_TAG            "N02"
#define NET_MSG_REASON_CODE_TAG         "N03"
#define NET_STIP_REASON_CODE_TAG        "N04"
#define NET_CHARGEBACK_FLAGS_TAG        "N06"
#define NET_NET_PARTI_FLAGS_TAG         "N07"
#define NET_VISA_ABI_TAG                "N08"
#define NET_FRAUD_DATA_TAG              "N09"
#define NET_REIMB_ATTR_TAG              "N11"
#define NET_SHARE_GRP_CODE_TAG          "N12"
#define NET_DECIMAL_POS_TAG             "N13"
#define NET_ISS_CUR_CONV_DATA_TAG       "N14"
#define NET_STLMNT_AMOUT_TAG            "N15"
#define NET_IEW_SBA_TAG                 "N16"
#define NET_ADTL_DATA_IND_TAG           "N17"
#define NET_VOL_REQ_IND_TAG             "N18"
#define NET_VISA_ACT_CHK_RES			"N20"
#define NET_VISA_MERC_ID			    "N21"	/*HAL  20201803 Enh Q2.2020 Art 9.3.3*/
#define NET_VISA_EXT_STIP_REASON_CODE   "N22"	/* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/  
#define NET_VISA_ECOM_DATA              "N23"   /*PLUTONL-4280 :Enh5.03 :1372*/
#define NET_VISA_WATCH_LIST_RC          "N24"   /*PLUTONL-4280 :Enh5.03 :1383*/
#define NET_VISA_OVERALL_ASSESSMENT_SECURE_ECOM  "N25"   /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.1*/	
#define NET_VISA_PURCHASE_IDENTIFIER             "N26"   /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art8.3.1*/
#define NET_MCI_TRX_INT_CLASS			"N50"
/*AMER20190313 PROD00066569: CBAE Enh5.00 936*/
#define NET_MCI_ACC_DT_COMP_INFO		"N51"
#define NET_MCI_SEC_SERV_ADD_DATA_ISS	"N52"
#define NET_MCI_TRX_ANALYSIS		    "N53"   /*ACH20190910 PROD00070986*/
#define NET_MCI_TKN_TRX_ID	            "N54"   /*ACH20190910 PROD00070986*/
#define NET_MCI_MERCHANT_ADVICE_CODE	"N55"	/*AEA20210712 Enh21.Q4 AN4761*/
#define NET_TOKEN_PROGAM                "N56"   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
#define MCI_MIT_CIT_INDICATOR           "N58"  /*IZL 03062022 PLUTONL-4810 ENH 22Q2 MC AN5524*/
#define NET_MCI_SEC_SERV_IND            "N59"  /* PLUTONL-5548   AN6773  Enh 2023Q2 AHA20221221*/ /*Security Service Indicator*/
#define NET_MCI_SEC_SERV_DATA           "N60"  /* PLUTONL-5548   AN6773  Enh 2023Q2 AHA20221221*/  /*Security Service Data*/
#define NET_MAX_PROC_DATE_TAG           "N62" /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/


/*--------------------------------------------------------------------------*/
/*  Data Sets  Priv data         			    							*/
/*--------------------------------------------------------------------------*/
/* START MTR 4.11 PROD00042228 extends PROD00043022*/
/*EBETODO: To be changed*/ /*ZKO2092018:S30-S39 reserved for industry data*/
#define MONEY_TRANSFER_DATA					"S20"
#define ADRESS_VERIFICATION_DATA			"S21"

/*Start AMER20170919 PROD00046952*/
#define SALE_INFO							"S22"
#define SALE_ITEMS							"S23"
/*End AMER20170919 PROD00046952*/

#define SOLUTION_IDENTIFIER_DATA			"S22"
#define TRANSACTION_TYPOLOGY_DATA			"S23"
#define P7_F048_LOAN_DATA					"S24"
#define DIGITIZATION_DATA					"S25"
#define ECOM_AUTHENTIF_DATA					"S26" /*HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/ 
#define PSD2_DATA							"S27" /*HAL  20201803 Enh Q2.2020 Art 9.3.3*/ 
#define INIT_TRX_ECOM_DATA		            "S28" /*MAK30032020 PROD00074655: Enh5.01*/
#define DATA_3DS		                    "S29"  /*PLUTONL-4280 :Enh5.03 :1372*/
#define P7_F048_FLEET_DATA					"S30"
#define FOREIGN_RATE_INFO					"S31"








/*--------------------------------------------------------------------------*/
/*  ADDITIONAL POS  DATA TAGS   			    							*/
/*--------------------------------------------------------------------------*/
#define CARD_AUTHEN_RELIABIL_IND       "L01"
#define CHIP_CONDITION_CODE            "L02"
#define CCPS_TRANSACTION_IND           "L03"




/******************************************************************************\
* 
*
\******************************************************************************/
#define CAPTURE_CODE					"I00"
#define RES_AUTH_PROC_MODE				"I01"
#define TRX_TYPE						"I02"
#define MERCHANT_ID						"I03"
#define STORE_ID						"I04"
#define EDC_TRX_CODE					"I05"
#define BILL_REF_ID						"I06"
#define GOODS_ID						"I07"
#define PERSONAL_ID						"I08"
#define PERSONAL_PIN					"I09"
#define ECOM_TYPE_IND					"I10"
#define ORDER_NO						"I11"

#define INSTLMNT_IND					"I30"
#define INSTLMNT_RESP_CODE				"I31"
#define INSTLMNT_NUM_PERIOD				"I32"
#define INSTLMNT_PERIOD_UNIT			"I33"
#define INSTLMNT_DOWN_PAYMENT			"I34"
#define INSTLMNT_PAYMENT				"I35"
#define INSTLMNT_GRACE_PERIOD			"I36"
#define INSTLMNT_GRACE_UNIT				"I37"
#define INSTLMNT_PLAN_ID				"I38"
#define INSTLMNT_PLAN_TYPE				"I39"
#define INSTLMNT_FEE					"I40"


#define LOY_TRX_IND						"I50"
#define LOY_RESP_CODE					"I51"
#define LOY_REDEMPTION_POINTS			"I52"
#define LOY_BALANCE						"I53"
#define LOY_PAID_AMOUNT					"I54"


#define LOCAL_MERCHANT_ID				"I70"
#define NEW_PIN_FORMAT					"I71"
#define NEW_PIN_OFFSET					"I72"
#define NEW_PIN_1						"I73"
#define NEW_PIN_2						"I74"
#define NEW_PIN_LEN						"I75"


#define ADVICE_TYPE						"I80"

/*--------------------------------------------------------------------------*/
/* UTILITY PAYMENT DATA TAGS                                               */
/*------------------------------------------------------------------------*/
#define   UTILITY_PAY_ID                "U01"
#define   UTILITY_ACC_ID                "U02"
#define   UTILITY_BILL_ID               "U03"

#define   BASE24_DE048                        "B48"
#define   BASE24_DE060                        "B60"


#define TRANS_DESC_TAG                 "T05"/* SWI06032015 ENH15.1*/


/*EBETODO: details on user specifics need to be defined*/
/*
#define MSISDN_TAG               	    "P17"
#define NAME_SURNAME_TAG                "P19"
#define WU_FIRST_NAME_TAG      			"W01"
#define WU_LAST_NAME_TAG      			"W02"
#define WU_MTCN_TAG           			"W03"
#define WU_TRX_ID_TAG      				"W04"
#define WU_REEL_AMOUNT_TAG     			"W05"
#define WU_PICKUP_REF_TAG  				"W06"
#define WU_TRX_DIGEST_TAG  				"W07"
*/
/*SNO151115 Tags for Tags G01 CB_DATA*/


#define CB_FIELD_ERROR					"100"		/* CB2A Aut/CB2A TLC/CBAE 44.AA			*/
#define CB_FIELD_CONV					"101"		/* CB2A Aut/CB2A TLC/CBAE 44.AC			*/
#define CB_CVV_CTRL_RESULT				"102"		/* CB2A Aut/CB2A TLC/CBAE 44.CA			*/
#define CB_REASON_CODE					"103"		/* CB2A/CBAE 59.101, CB2A TLC 47.01		*/
#define CB_TRX_ENV						"104"		/* CB2A/CBAE 59.200, CB2A TLC 47.02		*/
#define CB_INVOICE_TYPE					"105"		/* CB2A TLC 47.03						*/
#define CB_AUT_FORCING_CD				"106"		/* CB2A TLC 47.05						*/
#define CB_SA_GEN_TRX_NO				"107"		/* CB2A TLC 47.10						*/
#define CB_TRX_ID						"108"		/* CB2A TLC 47.24						*/

#define CB_SIRET_NUMBER					"200"		/* CB2A/CBAE 47.96, CB2A TLC 46.DF5D	*/
#define CB_SITE_TYPE					"201"		/* CB2A/CBAE 47.08, CB2A TLC 47.06		*/
#define CB_ACCEPTOR_CONTRACT_NO			"202"		/* CB2A/CBAE 59.202, CB2A TLC 46.DF5F	*/

#define CB_SA_ARCH						"210"		/* CB2A TLC 46.DF52						*/
#define CB_APP_STATUS					"211"		/* CB2A TLC 46.DF54						*/
#define CB_TASA							"212"		/* CB2A 59.20B, CB2A TLC 47.48			*/
#define CB_IDPA							"213"		/* CB2A 47.97, CB2A TLC 46.DF5C			*/
#define CB_IDSA							"214"		/* CB2A 47.A0, CB2A TLC 46.DF5E			*/
#define CB_PA_LOGICAL_NO				"215"		/* CB2A/CBAE 59.204, CB2A TLC 46.DF5B	*/
#define CB_SA_LOGICAL_NO				"216"		/* CB2A/CBAE 59.203, CB2A TLC 46.DF51	*/
#define CB_ITP_PA						"217"		/* CBAE 59.215, (Not in CB2A)			*/
#define CB_ITP_SA						"218"		/* CB2A/CBAE 59.201, CB2A TLC 46.DF50	*/

#define CB_TAC_DEFAULT					"300"		/* CB2A TLC 58.FF0D						*/
#define CB_TAC_DENIAL					"301"		/* CB2A TLC 58.FF0E						*/
#define CB_TAC_ONLINE					"302"		/* CB2A TLC 58.FF0F						*/
#define CB_ACCEPT_CERT					"303"		/* CB2A TLC 58.FF46						*/
#define CB_CARD_APP_TYPE				"304"		/* CB2A TLC 55.DF73						*/



#define P7_F048S20_PAYMENT_TYPE					"001"
#define P7_F048S20_FUNDS_SOURCE					"002"
#define P7_F048S20_SENDER_REF_NBR				"003" /*MTR20180412 - Enh CBAE4.12 name changed*/
#define P7_F048S20_TRX_REF_NBR				    "003" 
#define P7_F048S20_SENDER_ACCOUNT_NO			"004"
#define P7_F048S20_SENDER_NAME					"005"
#define P7_F048S20_SENDER_STREET_ADDR			"006"
#define P7_F048S20_SENDER_CITY_NAME				"007"
#define P7_F048S20_SENDER_STATE					"008"
#define P7_F048S20_SENDER_COUNTRY				"009"
#define P7_F048S20_SENDER_PHONE_NBR				"010"
#define P7_F048S20_SENDER_DOB					"011"
#define P7_F048S20_MESSAGE						"012"
#define P7_F048S20_RECIPIENT_NAME				"013"

/*PROD00025867*/
#define P7_F048S20_SENDER_FIRST_NAME			"014"
#define P7_F048S20_SENDER_MIDDLE_NAME			"015"
#define P7_F048S20_SENDER_LAST_NAME				"016"
#define P7_F048S20_SENDER_POSTAL_CODE			"017"
#define P7_F048S20_RECIPIENT_FIRST_NAME			"018"
#define P7_F048S20_RECIPIENT_MIDDLE_NAME		"019"
#define P7_F048S20_RECIPIENT_LAST_NAME			"020"
#define P7_F048S20_RECIPIENT_STREET_ADDR		"021"
#define P7_F048S20_RECIPIENT_CITY_NAME			"022"
#define P7_F048S20_RECIPIENT_STATE				"023"
#define P7_F048S20_RECIPIENT_COUNTRY			"024"
#define P7_F048S20_RECIPIENT_POSTAL_CODE		"025"
#define P7_F048S20_RECIPIENT_PHONE_NUMBER		"026" 
#define P7_F048S20_RECIPIENT_DOB				"027"
#define P7_F048S20_RECIPIENT_ACCOUNT_NUMBER		"028"
#define P7_F048S20_SENDER_ID					"029"
#define P7_F048S20_LANGUAGE_ID					"030"
#define P7_F048S20_ADD_SENDER_DATA				"031"
#define P7_F048S20_IBAN         				"033"
#define P7_F048S20_SENDER_BIC         			"034"	/*AMER20190228 PROD00066569: Enh5.0 1046*/
#define P7_F048S20_RECIPIENT_BIC			    "035"   /*AMER20190228 PROD00066569: Enh5.0 1046*/
#define P7_F048S20_RECIPIENT_ACCOUNT_TYPE       "036"   /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
#define P7_F048S20_SENDER_ACCOUNT_TYPE	        "037"   /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
#define P7_F048S20_PAYMENT_PURPOSE	            "038"	/*AEA20210712 Enh21.Q4 Art2.9*/
#define P7_F048S20_SENDER_ID_TYPE_CODE          "040"   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
#define P7_F048S20_SENDER_ID_SUBTYPE_CODE       "041"   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
#define P7_F048S20_SENDER_ID_ISS_CNTRY_CODE     "042"   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
#define P7_F048S20_RECEIVER_ID_TYPE_CODE        "043"   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
#define P7_F048S20_RECEIVER_ID_SUBTYPE_CODE     "044"   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
#define P7_F048S20_RECEIVER_ID                  "045"   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
#define P7_F048S20_RECEIVER_ID_ISS_COUNTRY_CODE "046"   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
#define P7_F048S20_DATA_VERIFICATION_TYPE       "047"   /*PLUTONL-6505 -Enh 2023Q4 NAB :AN 7402*/


/*ZKOStart20092018:FleetData*/

/* 
#define P7_F048S30_INVOICE_NBR			"001"
#define P7_F048S30_MILEAGE				"002"
#define P7_F048S30_PRODUCT_CODE			"003"
#define P7_F048S30_VEHICLE_REG_NBR		"004"
#define P7_F048S30_FUEL_QUANTITY		"005"
#define P7_F048S30_FUEL_AMOUNT			"006"
#define P7_F048S30_OIL_QUANTITY			"007"
#define P7_F048S30_OIL_AMOUNT			"008"
#define P7_F048S30_TYRES_QUANTITY		"009"
#define P7_F048S30_TYRES_AMOUNT			"010"
#define P7_F048S30_BATTERIES_AMOUNT		"011"
#define P7_F048S30_SUNDRY_AMOUNT		"012"
#define P7_F048S30_GARAGE_AMOUNT		"013"
*/
/*ZKOEnd20092018:FleetData*/



/*ZKOStart25092018: Installment Loan*/
#define P7_F048S24_MONTHLY_RPYMNT_AMNT			"001"
#define P7_F048S24_INTEREST_RATE				"002"
#define P7_F048S24_FIRST_RPYMNT_DATE			"003"
#define P7_F048S24_NBR_INSTALMENTS				"004"
/*ZKOEnd25092018: Installment Loan*/
#define P7_F048S24_FREQ_RECUR_PAY				"007"   /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/   
#define P7_F048S24_REGISTRATION_REF_NBR			"008"   /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
#define P7_F048S24_VALIDATION_IND               "009"   /*Visa only*/ /*IZL 26092022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/




/*** subfield of field F048.P21 additionnal Merchant data ***/
#define P7_F048P21_PAYFAC_ID					"001"  	/** PAYMENT_FACILITATOR_ID **/
#define P7_F048P21_ISO_ID        				"002"	/** INDEPENDENT_SALESORGANIZATION_ID **/
#define P7_F048P21_SUB_MER_ID                   "003" 	/** SUB_MERCHANT_ID **/
#define P7_F048P21_MARKET_PLACE_ID              "004" 	/** Market place id **/ /*AMER20190228 PROD00066569: Enh5.00 1070*/
#define P7_F048P21_MERCHANT_ID              	"005"   /** Merchant id **/
#define P7_F048P21_MERCHANT_ORIGINE_COUNTRY     "006"   /* HAL25022021 PLUTONL-2632 ART 4224 2021.Q2 */
/*#define	P7_F048P21_TRANSF_SERVC_PROVID_NAME     "007"	/*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/	
#define	P7_F048P21_ACC_LEGAL_BUSINESS_NAME      "007"	/*ZKO 05042022 PLUTONL-4550 ENH 22.Q2 VISA Art3.2*/																		   
#define P7_F048P21_PAY_FACILITATOR_NAME         "008"   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
/* START PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/
#define P7_F048P21_MCI_MSG_FORMAT_VERSION_CODE	  "009"
#define P7_F048P21_ACC_STREET_ADDR				  "010"
#define P7_F048P21_ACC_CTRY_SUB_CODE			  "011"
#define P7_F048P21_ACC_URL_ADDR				      "012"
#define P7_F048P21_ACC_CUSTOMER_SERVICE_PHONE_NBR "013"
#define P7_F048P21_ACC_PHONE_NBR				  "014"
#define P7_F048P21_ACC_ADD_CONTACT_INFO			  "015"
#define P7_F048P21_ACC_TAX_ID					  "016"
#define P7_F048P21_ACC_PARTNER_ID				  "017"
#define P7_F048P21_ACC_POSTAL_CODE				  "018"
/* END PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/


/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
#define	  P7_F048P21_PAYFAC_STREET_ADDR	        "023"
#define	  P7_F048P21_PAYFAC_CITY_NAME	        "024"
#define	  P7_F048P21_PAYFAC_STATE_CODE	        "025"
#define	  P7_F048P21_PAYFAC_POSTAL_CODE	        "026"
#define	  P7_F048P21_PAYFAC_CTRY_CODE	        "027"
/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/


/* F048.P55: Token data */
#define P7_F048P55_TOKEN						"T01"
#define P7_F048P55_TKN_REF_ID					"T02"
#define P7_F048P55_TKN_REQ_ID					"T03"
#define P7_F048P55_TKN_ASSURANCE_LVL			"T04"
#define P7_F048P55_TKN_STATUS					"T05"
#define P7_F048P55_TKN_EXP_DT					"T06"
#define P7_F048P55_TKN_TYPE						"T07"
#define P7_F048P55_TKN_DEVICE_TYPE				"T08"
#define P7_F048P55_TKN_PAN						"T10"
#define P7_F048P55_TKN_PAN_SEQ_NBR				"T11"
#define P7_F048P55_TKN_PAN_STATUS 				"T12"
#define P7_F048P55_TKN_PAN_EXP_DT 				"T13"
#define P7_F048P55_TKN_PAN_REF_ID				"T14"
#define P7_F048P55_TKN_PAN_ISSUER_ID 			"T30"
#define P7_F048P55_AUTO_FILL         			"T31"   /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/	

/* F048.S22 */
#define P7_F048S22_BILLING_POSTAL_CODE			"001"
#define P7_F048S22_BILLING_ADDRESS				"002"
#define P7_F048S22_BILLING_FIRST_NAME			"003"
#define P7_F048S22_BILLING_LAST_NAME			"004"
#define P7_F048S22_BILLING_PHONE_NBR			"005"
#define P7_F048S22_BILLING_COUNTRY_CODE			"006"
#define P7_F048S22_SHIPPING_METHOD				"007"
#define P7_F048S22_SHIP_TO_POSTAL_CODE			"008"
#define P7_F048S22_SHIP_TO_ADDRESS				"009"
#define P7_F048S22_SHIP_TO_FIRST_NAME			"010"
#define P7_F048S22_SHIP_TO_LAST_NAME			"011"
#define P7_F048S22_SHIP_TO_PHONE_NUMBER			"012"
#define P7_F048S22_SHIP_TO_COUNTRY_CODE			"013"
#define P7_F048S22_CUSTOMER_EMAIL				"014"
#define P7_F048S22_CUSTOMER_IP					"015"
#define P7_F048S22_CUSTOMER_HTTP_BROWSER_TYPE	"016"
#define P7_F048S22_MERCHANT_HOSTNAME			"020"
#define P7_F048S22_MERCHANT_ANI					"021"
#define P7_F048S22_MERCHANT_II_DIGITS			"022"
#define P7_F048S22_MERCHANT_NAME				"023"
#define P7_F048S22_PURCHASE_AMOUNT				"024"
#define P7_F048S22_AUTH_DATE_TIME				"025"
#define P7_F048S22_CH_SHIP_HASH				    "026" /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/

#define P7_F048S22_ACC_OWNER_FIRST_NAME         "027" /*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/
#define P7_F048S22_ACC_OWNER_MIDDLE_NAME        "028" /*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/
#define P7_F048S22_ACC_OWNER_LAST_NAME          "029" /*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/
#define P7_F048S22_ANI_REQUEST_INDIC            "030" /*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/
#define P7_F048S22_NAME_MATCHING_RES            "031" /*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/


/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
#define	  P7_F048S22_ACC_OWNER_NATIONALITY      "032"
#define	  P7_F048S22_ACC_OWNER_CTRY_OF_BIRTH	"033"
#define	  P7_F048S22_ACC_OWNER_OCCUPATION	    "034"
#define	  P7_F048S22_ACC_OWNER_DATE_OF_BIRTH	"035"
#define	  P7_F048S22_ACC_OWNER_EMAIL_ADDS	    "036"
/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/

/* F048.S23: Encoding of auth field is:  Len + TLV || Len + TLV...etc*/
#define P7_F048S23_ITEM_IDENTIFICATION			"001"
#define P7_F048S23_PRODUCT_CODE					"002"
#define P7_F048S23_ADD_PRODUCT_CODE				"003"
#define P7_F048S23_INT_PRODUCT_CODE				"004"
#define P7_F048S23_UNIT_OF_MEASURE				"005"
#define P7_F048S23_PRODUCT_QUANTITY				"006"
#define P7_F048S23_UNIT_PRICE					"007"
#define P7_F048S23_PRODUCT_AMOUNT				"009"
#define P7_F048S23_VALUE_ADDED_TAX				"011"
#define P7_F048S23_TAX_TYPE						"012"
#define P7_F048S23_PRODUCT_DESCRIPTION			"013"
#define P7_F048S23_DELIVERY_LOCATION			"014"
#define P7_F048S23_DELIVERY_SERVICE				"015"



/* F048.S25: Digitization data */
#define P7_F048S25_PAN_SOURCE					"D01"
#define P7_F048S25_PAY_APP_INSTANCE_ID			"D02"
#define P7_F048S25_NBR_ACTIVE_TKNS				"D03"
#define P7_F048S25_TKN_TYPE					    "D06"
#define P7_F048S25_DEVICE_SRC_IP_ADR			"D07"
#define P7_F048S25_WP_TKN_RECMD				    "D08"
#define P7_F048S25_WP_TKN_RECMD_VERSION			"D09"
#define P7_F048S25_WP_DEVICE_SCORE			    "D10"
#define P7_F048S25_WP_ACCOUNT_SCORE			    "D11"
#define P7_F048S25_DEVICE_LOCATION 			    "D13"
#define P7_F048S25_MOB_NBR_LAST_DIGITS		    "D14"
#define P7_F048S25_ACTIVATION_CODE           	"D15"
#define P7_F048S25_ACTIVATION_CODE_EXP_DT 	    "D16"
#define P7_F048S25_CSMR_ACTIV_METHD_PREF 	    "D17"
#define P7_F048S25_DEVICE_NAME 	                "D20"
#define P7_F048S25_FINAL_TKN_DECISION        	"D21"
#define P7_F048S25_FINAL_TKN_DECISION_IND 	    "D22"
#define P7_F048S25_TC_IDENTIFIER 	            "D23"
#define P7_F048S25_TC_DT 	                    "D24"
#define P7_F048S25_TKN_UNQ_REF 	                "D26"
#define P7_F048S25_PAN_UNQ_REF 	                "D27"
#define P7_F048S25_TKN_EVENT_IND 	            "D28"
#define P7_F048S25_TKN_EVENT_RSN_CODE 	        "D29"
#define P7_F048S25_EVENT_REQUESTOR 	            "D30"
#define P7_F048S25_TKN_DATA_ACTV_MTHD 	        "D32"

/*Start HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/ 
#define P7_F048S26_ECOM_AUT_DATA_ESTIMATED_AMOUNT    "001"
#define P7_F048S26_ECOM_AUT_DATA_MERCHANT_ACRONYM    "002"
#define P7_F048S26_ECOM_AUT_DATA_DATE_TIME           "003"  /*ASO20200603 PROD00075275: Enh5.01 1140*/
#define P7_F048S26_ECOM_AUT_DATA_SESSION_ID          "004"  /*HAL  20201803 Enh Q2.2020 Art 9.3.4*/
#define P7_F048S26_AUTHEN_DATA_QUALITY_INDC          "010" /*PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 


/*End HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/ 

/* F048.S30: Fleet data */
#define P7_F048S30_PURCHASE_TYPE                "001"
#define P7_F048S30_SERVICE_TYPE                 "002"
#define P7_F048S30_FUEL_TYPE                    "003"
#define P7_F048S30_UNIT_MEASURE                 "004"
#define P7_F048S30_QUANTITY                     "005"
#define P7_F048S30_UNIT_COST                    "006"
#define P7_F048S30_GROSS_FUEL_PRICE             "007"
#define P7_F048S30_NET_FUEL_PRICE               "008"
#define P7_F048S30_GROSS_NON_FUEL_PRICE         "009"
#define P7_F048S30_NET_NON_FUEL_PRICE           "010"
#define P7_F048S30_ODOMETER                     "011"
#define P7_F048S30_VAT_TAX_RATE                 "012"
#define P7_F048S30_MISC_FUEL_TAX_EX_STATUS      "013"
#define P7_F048S30_MISC_FUEL_TAX                "014"
#define P7_F048S30_MISC_NON_FUEL_TAX_EX_STATUS  "015"
#define P7_F048S30_MISC_NON_FUEL_TAX            "016"
#define P7_F048S30_LOCAL_TAX_INCLUDED           "017"
#define P7_F048S30_LOCAL_TAX                    "018"
#define P7_F048S30_NATIONAL_TAX_INCLUDED        "019"
#define P7_F048S30_NATIONAL_TAX                 "020"
#define P7_F048S30_OTHER_TAX                    "021"
#define P7_F048S30_MER_VAT_REGISTRATION         "022"
#define P7_F048S30_CUSTOMER_VAT_REG_NUMBER      "023"
#define P7_F048S30_CUSTOMER_REFERENCE_NUMBER    "024"
#define P7_F048S30_MESSAGE_IDENTIFIER           "025"
#define P7_F048S30_ADDITIONAL_DATA_INDICATOR    "026"
#define P7_F048S30_SUMMARY_COMMODITY_CODE       "027"
#define P7_F048S30_NON_FUEL_PRODUCT_CODE_1      "028"
#define P7_F048S30_NON_FUEL_PRODUCT_CODE_2      "029"
#define P7_F048S30_NON_FUEL_PRODUCT_CODE_3      "030"
#define P7_F048S30_NON_FUEL_PRODUCT_CODE_4      "031"
#define P7_F048S30_NON_FUEL_PRODUCT_CODE_5      "032"
#define P7_F048S30_NON_FUEL_PRODUCT_CODE_6      "033"
#define P7_F048S30_NON_FUEL_PRODUCT_CODE_7      "034"
#define P7_F048S30_NON_FUEL_PRODUCT_CODE_8      "035"
#define P7_F048S30_FUEL_BRAND                   "036"
#define P7_F048S30_FUEL_TRANS_VAL_RESULTS       "037"
#define P7_F048S30_FUEL_ACCEPTANCE_MODE         "038"
#define P7_F048S30_DRIVER_ID                    "039"
#define P7_F048S30_JOB_NUMBER                   "040"
#define P7_F048S30_FLEET_NUMBER                 "041"
#define P7_F048S30_VRN                          "042"
#define P7_F048S30_PRODUCT_QUALIFIER            "043"
#define P7_F048S30_EXPANDED_FUEL_TYPE           "044"

/*Start HAL  20201803 Enh Q2.2020 Art 9.3.3*/ 
#define P7_F048_S27_AUTH_EXEMPTION_INDIC    "001"
#define P7_F048_S27_AUTH_EXEMPTION_RES      "002"
#define P7_F048_S27_REASON_HONOR_EXEM       "003"
#define P7_F048_S27_TRA_SCORE               "004"

#define	P7_F048_S27T001_EXEMP_IND_LEN		16
#define	P7_F048_S27T002_EXEMP_RSLT_LEN		8 /* PLUTONL-5283 FZL20221013*/

#define	P7_F048_S27T002_TRA_OFST			0
#define	P7_F048_S27T002_LVA_OFST			1
#define	P7_F048_S27T002_TRUSTED_MER_OFST	2
#define	P7_F048_S27T002_DELEG_AUTH_OFST		3
#define	P7_F048_S27T002_SCP_IND_OFST	    4 /* PLUTONL-5283 FZL20221013*/
/*HAL  20201803 Enh Q2.2020 Art 9.3.3*/ 



#define P7_F048_S28_3DS_PROTOCOL_VERSION               	"001"
#define P7_F048_S28_ACS_TRX_ID               			"002"
#define P7_F048_S28_DS_TRX_ID              				"003"
#define P7_F048_S28_MERCHANT_NAME               		"004"
#define P7_F048_S28_AUTHENTICATION_DATE               	"005"
#define P7_F048_S28_AUTHENTICATION_AMOUNT               "006"
#define P7_F048_S28_ECOM_TRX_SECURITY_TYPE              "007"
#define P7_F048_S28_CARDHOLDER_AUTHENTICATION_METHOD    "008"
#define P7_F048_S28_ECOM_CRYPTOGRAM_CALCULATION_METHOD  "009"
#define P7_F048_S28_RESULT_SECURED_REMOTE_PAYMENT		"010"
#define P7_F048_S28_RESULT_SECURED_REMOTE_PAYMENT_EXT	"011"
#define P7_F048_S28_AUTHENTICATION_VALUE				"012"

/*Start PLUTONL-4280 :Enh5.03 :1372*/
#define P7_F048_S29_3DS_Browser_IP_Address               "001"
#define P7_F048_S29_3DS_APP_IP_Address                   "002"
#define P7_F048_S29_SHIPPING_INDICATOR                   "003"
#define P7_F048_S29_DEVICE_ID                            "004"
#define P7_F048_S29_DEVICE_TYPE                          "005"
#define P7_F048_S29_SDK_APP_ID                           "006"
/*End Start PLUTONL-4280 :Enh5.03 :1372*/
#define P7_F048_S29_THIRD_PARTY_ID                       "010" /*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
#define P7_F048_S29_THIRD_PARTY_RISK_SCORE               "011" /*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/

/*START ASO20210419 : Enh5.02 1288*/
#define P7_F048_S31_PERS_FX_EXCH_RATE_PROVIDER				"001"
#define P7_F048_S31_PERS_FX_RATE_TABLE_ID				    "002"
#define P7_F048_S31_PERS_FX_EXCH_RATE         				"003"
#define P7_F048_S31_PERS_FX_ELIGIBILITY_INDICATOR			"004"
#define P7_F048_S31_ECB_FX_EXCH_RATE_PROVIDER				"005"
#define P7_F048_S31_ECB_FX_RATE_TABLE_ID				    "006"
#define P7_F048_S31_ECB_FX_EXCH_RATE        				"007"
/*END ASO20210419 : Enh5.02 1288*/
/*Start PLUTONL-4280 :Enh5.03 :1362*/
/*#define P7_F048_S31_ECB_FX_EXCH_RATE        				"201"*/
#define P7_F048_S31_ECB_FX_RATE_DIFF_SIGN        			"201"
#define P7_F048_S31_ECB_FX_EXCH_PERC_DIFF        			"202"
/*End PLUTONL-4280 :Enh5.03 :1362*/


/* Token types */
#define F048P55_T07_SE 	                        "02"    /*      Secure element      */
#define F048P55_T07_TEE 	                    "04"    /*          TEE             */
#define F048P55_T07_REM_SEC_LOC  	            "05"    /* Remote Secure location   */

/* Mobile payment solutions */
#define F048K10_APPLE_PAY 	                    "000" 
#define F048K10_SAMSUNG_PAY 	                "001" 
#define F048K10_GOOGLE_PAY 	                    "002" 
#define F048K10_MERCHANT_PROGRAM                "003" 
#define F048K10_OTHER_WALLET                    "111" 


/* MCI DE 124 fields len */
#define MCI_F124_MSG_TYPE_LEN			2	
#define MCI_F124_CORRELATION_ID_LEN		14	
#define MCI_F124_PAN_SOURCE_LEN			1
#define MCI_F124_PAY_APP_INST_ID_LEN	48	
#define MCI_F124_NBR_ACTIVE_TKNS_LEN	2	
#define MCI_F124_WP_ACC_ID_HASH_LEN		64	
#define MCI_F124_CARDHLDR_NAME_LEN		27	
#define MCI_F124_TKN_TYPE_LEN			1
#define MCI_F124_DEVICE_SRC_IP_AD_LEN	12	
#define MCI_F124_WP_TKN_RECMD_LEN		1
#define MCI_F124_WP_TKN_RECMD_VERS_LEN	2
#define MCI_F124_WP_DEVICE_SCORE_LEN	1
#define MCI_F124_WP_ACCOUNT_SCORE_LEN	1
#define MCI_F124_WP_RECM_REAS_CD_LEN	6
#define MCI_F124_DEVICE_LOCT_LEN		9
#define MCI_F124_MOB_NBR_LAST_DIG_LEN	4
#define MCI_F124_ACTIV_CODE_LEN			8
#define MCI_F124_ACTIV_CODE_EXP_DT_LEN	10
#define MCI_F124_ISS_PRD_CONF_ID_LEN	10
#define MCI_F124_CONSUMER_LANG_LEN		2
#define MCI_F124_DEVICE_NAME_LEN		20
#define MCI_F124_FNL_TKN_DECISION_LEN	1
#define MCI_F124_FNL_TKN_DECS_IND_LEN	1
#define MCI_F124_TC_IDENTIFIER_LEN		32
#define MCI_F124_TC_DT_LEN				10
#define MCI_F124_NBR_ACTV_ATTEMPTS_LEN	1
#define MCI_F124_TKN_UNQ_REF_LEN		48
#define MCI_F124_PAN_UNQ_REF_LEN		48
#define MCI_F124_TKN_EVENT_IND_LEN		1
#define MCI_F124_TKN_EVENT_RSN_CD_LEN	2
#define MCI_F124_EVENT_REQUESTOR_LEN	1
#define MCI_F124_ISS_PRDCT_CONFIG_ID	10
#define MCI_F124_PAN_SEQ_NBR			3

#endif /* HPS_TLV_PRIVFIELDS_H */

