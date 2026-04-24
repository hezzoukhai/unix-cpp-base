#ifndef MDS_DEFINE___H
#define MDS_DEFINE___H


#define		MDS_F003_PC_PURCHASE			"00" /* Purchase						*/
#define		MDS_F003_PC_WITHDRAWAL			"01" /* Withdrawal						*/
#define		MDS_F003_PC_DEBIT_ADJ			"02" /* Debit Adjustment				*/
#define		MDS_F003_PC_PURCHASE_CB			"09" /* Purchase with Cash Back			*/
#define		MDS_F003_PC_CASH_ADV			"17" /* Cash Disbursement				*/
#define		MDS_F003_PC_REFUND				"20" /* Purchase Return/Refund			*/
#define		MDS_F003_PC_DEPOSIT				"21" /* Deposit							*/
#define		MDS_F003_PC_CHECK_DEP_GUARANTY	"23" /* Check Deposit Guarantee			*/
#define		MDS_F003_PC_PAYMENT				"28" /* Payment Transaction				*/
#define		MDS_F003_PC_BAL_INQ				"30" /* Balance Inquiry					*/
#define		MDS_F003_PC_ACC_TRANSFER		"40" /* Account Transfer				*/
#define		MDS_F003_PC_PIN_UNBLOCK			"91" /* PIN Unblock						*/
#define		MDS_F003_PC_PIN_CHANGE			"92" /* PIN Change						*/
#define		MDS_F003_PC_LEN					2	


/*Start IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/
#define		MDS_F048_SE22_SF01_LOW_RISK_MER_IND			"01"
#define		MDS_F048_SE22_SF02_SINGLE_TAP_IND			"02"
#define		MDS_F048_SE22_SF03_RESPONSE_PIN_REQ			"03"
#define		MDS_F048_SE22_SF04_ISS_PIN_REQ_SIN_TAP		"04"
#define		MDS_F048_SE22_SF05_CH_MER_INIT_IND			"05" /*ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define		MDS_F048_SE22_SF06_MSG_FORMAT_VERSION_CODE  "06" /*PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/
#define 	MDS_F048_SE22_LEN					/*4*/    6  /*ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/ /*PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/
#define		MDS_F048_SE22_SF01_MER_INIT_TRAN			"01"
#define		MDS_F048_SE22_SF01_ACQ_LOW_FRAUD_TRA		"02"
#define		MDS_F048_SE22_SF01_RECURRING_PYMT			"03"
#define		MDS_F048_SE22_SF01_LOW_VAL_PYMT				"04"
#define		MDS_F048_SE22_SF01_SCA_DELEGATION			"05"
#define		MDS_F048_SE22_SF01_SCP						"06"
#define		MDS_F048_SE22_SF01_AUTH_OUTAGE_EXCEPTION	"07" /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4040*/
/*Start IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/

/*Start ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/

#define	 MDS_F048_SE22_SF05_CIT_CREDENTIAL			"C101"
#define	 MDS_F048_SE22_SF05_CIT_STANDING            "C102"
#define	 MDS_F048_SE22_SF05_CIT_SUBSCRIPTION        "C103"
#define	 MDS_F048_SE22_SF05_CIT_INSTALLMENT         "C104"
#define	 MDS_F048_SE22_SF05_MIT_UNSCHEDULED         "M101"
#define	 MDS_F048_SE22_SF05_MIT_STANDING_ORDER      "M102"
#define	 MDS_F048_SE22_SF05_MIT_SUBSCRIPTION        "M103"
#define	 MDS_F048_SE22_SF05_MIT_INSTALLMENT         "M104"
#define	 MDS_F048_SE22_SF05_MIT_PARTIAL_SHIPMENT    "M205"
#define	 MDS_F048_SE22_SF05_MIT_DELAYED_CHARGE      "M206"
#define	 MDS_F048_SE22_SF05_MIT_NO_SHOW_CHARGE      "M207"
#define	 MDS_F048_SE22_SF05_MIT_RESUBMISSION        "M208"
#define  MDS_F048_SE22_SF05_LEN			             4	

/*End ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/

/* START PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/
#define     MDS_F048_SE22_SF06_LEN						          1
#define	    MDS_MSG_FORMAT_VERSION_CODE_PRE		                "A"
#define	    MDS_MSG_FORMAT_VERSION_CODE_POST		            "B"
/* END PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/

/*Start ACH20191219*/
#define		MDS_F048_T33_T01_ACC_NO_IND			"01"
#define		MDS_F048_T33_T02_ACC_NO				"02"
#define		MDS_F048_T33_T03_EXP_DT				"03"
#define		MDS_F048_T33_T05_TOKEN_ASS_LVL		"05"
#define		MDS_F048_T33_T06_TOKEN_REQ_ID		"06"
#define		MDS_F048_T33_T08_STORAGE_TECH		"08"   

#define MDS_F124_ISS_PRDCT_CONFIG_ID	10
#define MDS_F124_PAN_SEQ_NBR			3
/*End ACH20191219*/

/* start AHA20220829 PLUTONL-4980*/
#define MDS_F110_NB_FIELDS              2   
#define MDS_AD_KEY_BLOCK_PROTECT        "09"
#define MDS_AD_KEY_CHECK_VALUE          "10"
/* end AHA20220829 PLUTONL-4980*/

#define POS_DATA_EXT_LENGTH				5 /*PLUTONL-5307 AHA20221018*/
/* Storage Technology values*/
#define MDS_F048_SE33_SF08_SERVER		"03" 	/* Server */
#define MDS_F048_SE33_SF08_TEE 			"04" 	/* Trusted Execution Environment (TEE) */
#define MDS_F048_SE33_SF08_SE			"05"	/* Secure Element (SE) */

/* Wallet Program Data values*/
#define MDS_F048_SE26_SF01_APPLE_PAY 		"103"
#define MDS_F048_SE26_SF01_GOOGLE_PAY 		"216"
#define MDS_F048_SE26_SF01_SAMSUNG_PAY 		"217"
#define MDS_F048_SE26_SF01_MER_TOK_PROGRAM 	"327"	/*Merchant tokenization program*/	/*PLUTONL-2222*/





#define MDS_F022_P1_UNKNOWN					"00"	/* PAN entry mode unknown				*/
#define MDS_F022_P1_MANUAL					"01"	/* PAN manual entry						*/
#define MDS_F022_P1_MAG						"02"	/* PAN auto-entry via magnetic stripe track data is not required.	*/
#define MDS_F022_P1_BC_READER				"03"	/* PAN auto-entry via bar code reader								*/
#define MDS_F022_P1_OCR						"04"	/* PAN auto-entry via optical character reader (OCR)				*/
#define MDS_F022_P1_CHIP					"05"	/* PAN auto-entry via chip											*/
#define MDS_F022_P1_CHIP_PAYPASS			"06"	/* PAN auto-entry via chip PayPass Mapping Service applied			*/
#define MDS_F022_P1_CONTACTLESS				"07"	/* PAN auto-entry via contactless M/Chip							*/
#define MDS_F022_P1_CL_PAYPASS				"08"	/* PAN auto-entry via contactless M/Chip PayPass Mapping Service applied	*/
#define MDS_F022_P1_ECOM_DSRP_ICC			"09"	/* PAN/Token entry via electronic commerce containing DSRP cryptogram in DE 55	*/
#define MDS_F022_P1_COF						"10"	/* Credential on File	*/
#define MDS_F022_P1_HYBRID					"79"	/* A hybrid terminal												*/
#define MDS_F022_P1_FALLBACK				"80"
#define MDS_F022_P1_ECOM_DSRP_UCAF			"81"	/* PAN/Token entry via electronic commerce with optional Identity Check-AAV or DSRP cryptogram in UCAF */
#define MDS_F022_P1_PAN_AUTO_ENTRY_SERVER	"82"	/* PAN Auto Entry via Server (issuer, acquirer, or third party vendor system) */
#define MDS_F022_P1_MAGNETIC				"90"	/* PAN auto-entry via magnetic stripe—the full track data has been read */
#define MDS_F022_P1_CL_MAG					"91"	/* PAN auto-entry via contactless magnetic stripe				*/
#define MDS_F022_P1_LEN						 2

#define MDS_F022_P2_UNKNOWN			'0'		/* Unspecified or unknown								*/
#define MDS_F022_P2_HAS_PIN_ENTRY		'1'		/* Terminal has PIN entry capability					*/
#define MDS_F022_P2_NO_PIN_ENTRY		'2'		/* Terminal does not have PIN entry capability			*/
#define MDS_F022_P2_HAS_PE_NOOP			'8'		/* Terminal has PIN entry capability but PIN pad is not currently operative	*/
#define MDS_F022_P2_MPOS_SOFT_PIN               '3'             /* mPOS Software-based PIN Entry Capability*//*ZKO:PROD00058820:ENH_OCT_2018_VISA_MCI*/


#define MDS_F048_SE42_SF3_MISSING_UCAF    '0'             /* Missing Universal Cardholder Authentication Field (UCAF)         */  /*AMER20170928 PROD00047410 Enh17.Q4*/
#define MDS_F048_SE42_SF3_INVALID_UCAF    '1'             /* Invalid Universal Cardholder Authentication Field (UCAF)         */  /*AMER20170928 PROD00047410 Enh17.Q4*/
#define MDS_F048_SE42_SF3_INVALID_SLI     '2'             /* Invalid Security Level Indicator                                 */  /*ELKAM20230202 ENH_2023Q2 AN 7090 */


#define MDS_F048_P21_SF1_LEN                    1
#define MDS_F048_P21_SF1_OFST                   4
#define MDS_F048_P21_SF1_MPOS_TERM            '0' /*Dedicated mPOS Terminal with PCI compliant dongle*/
#define MDS_F048_P21_SF1_SHELF_MDEV           '1' /*Off the Shelf Mobile Device*/

/*START PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	
#define MDS_F048_SE37_NB_FIELDS							 4
#define MDS_F048_SE37_SF01_PAYFAC_ID			        "01"  	/** Payment facilitator ID	**/
#define MDS_F048_SE37_SF02_ISO_ID   			        "02"	/** Independent sales organization ID **/
#define MDS_F048_SE37_SF03_SUB_MER_ID 			        "03"	/** Sub merchant ID	 **/
#define MDS_F048_SE37_SF04_ORIGINE_COUNTRY  			"04"  	/** Merchant origine’s country **/ 
/*END PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	

/*Subelement 66.Authentication Data*/

#define MDS_F048_P66_LEN                        45  /*Authentication Data*/

/*Program Protocol*/
#define MDS_F048_P66_SF1                       "01"
#define MDS_F048_P66_SF1_LENG                  "01"
#define MDS_F048_P66_SF1_OFST                  4
/* #define MDS_F048_P66_SF1_3DSEC_1_0             '1'
#define MDS_F048_P66_SF1_3DSEC_2_0             '2' *//* OBSOLETE ELKAM20230202 ENH_2023Q2 AN 7090 */

/* START ELKAM20230202 ENH_2023Q2 AN 7090 */
#define MDS_F048_P66_SF1_3DSEC_2_1           	'1'  
#define MDS_F048_P66_SF1_3DSEC_2_2           	'2'  
#define MDS_F048_P66_SF1_3DSEC_2_3           	'3'  
#define MDS_F048_P66_SF1_3DSEC_2_4           	'4'  
#define MDS_F048_P66_SF1_3DSEC_2_5           	'5'  
#define MDS_F048_P66_SF1_3DSEC_2_6           	'6'  
#define MDS_F048_P66_SF1_3DSEC_2_7           	'7'  
#define MDS_F048_P66_SF1_3DSEC_2_8           	'8'  
#define MDS_F048_P66_SF1_3DSEC_2_9           	'9'
/* END ELKAM20230202 ENH_2023Q2 AN 7090 */

/*Directory Server Transaction ID*/
#define MDS_F048_P66_SF2                      "02"
#define MDS_F048_P66_SF2_LENG                  "36"
#define MDS_F048_P66_SF2_LEN                    36



/*Start HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/ 
#define	MDS_F048_SE71_SF01_MC_DIG_ENABL_SERV_DIG_PAY_DATA_VALID		"54"
/*End HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/ 

/*Start HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 4221*/ 
#define		MDS_F048_SE71_SF01_MER_VAL_SERV		"55" 
/*End HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 4221*/  

#define MDS_F048_P77_P2P				"C01" /* Person-to-Person						*/
#define MDS_F048_P77_MCI_REBATE			"C02" /* MasterCard rebate						*/
#define MDS_F048_P77_RPWR				"C03" /* rePower Load Value						*/
#define MDS_F048_P77_GAMING_REPAY		"C04" /* Gaming Re-pay							*/
#define MDS_F048_P77_OTHER_PAY			"C05" /* Payment Transaction for a reason other than those defined in values C01-C04	*/
#define MDS_F048_P77_CR_CRD_PAY			"C06" /* Payment of a credit card balance with cash or check	*/
#define MDS_F048_P77_MON_SND_P2P		"C07" /* MoneySend Person-to-Person				*/
#define MDS_F048_P77_CARD_ACTV			"C09" /* Card Activation						*/
#define MDS_F048_P77_MON_SND_A2A		"C52" /* MoneySend Account-to-Account Transfers	*/
#define MDS_F048_P77_MON_SND_AGT_CSH	"C53" /* MoneySend Agent Cash Out				*/
#define MDS_F048_P77_MON_SND_CR_CRD_PAY "C54" /* MoneySend Credit Card Bill Payment		*/
#define MDS_F048_P77_MON_SND_B2C		"C55" /* MoneySend Business to Consumer Disbursement	*/
#define MDS_F048_P77_MON_SND_G2C		"C56" /* MoneySend Government/Non-Government Organization to Consumer Disbursement		*/
#define MDS_F048_P77_MON_SND_B2B		"C65" /* MoneySend Business to Business Transfer*//* IBO20190910 PROD00071519 Enh19.Q4 AN2615*/
#define MDS_F048_P77_IP_P2P				"C67" /* Inter Platform Person-to-Person		*/
#define MDS_F048_P77_LEN				3



#define		MDS_F054_AT_LG_BAL				"01" /* Ledger Balance						*/
#define		MDS_F054_AT_AVAILABLE			"02" /*  Available Balance					*/
#define		MDS_F054_AT_HC_ELG_AMNT			"10" /*  Healthcare Eligibility Amount		*/
#define		MDS_F054_AT_PRSCPT_ELG_AMNT		"11" /*  Prescription Eligibility Amount	*/
#define		MDS_F054_AT_ONL_BILL_PAY_FEE	"17" /*  MasterCard Prepaid Online Bill Pay Transaction Fee Amount	*/
#define		MDS_F054_AT_CB_AMNT				"40" /*  Amount Cash Back					*/
#define		MDS_F054_AT_GRATUITY_AMNT		"44" /*  Amount Gratuity					*/
#define		MDS_F054_AT_ORIG_AMNT			"57" /*  Original Amount					*/
#define		MDS_F054_AT_POI_AMOUNT			"58" /*	 POI Amount							*/
#define		MDS_F054_AT_LIM_MCI_IC			"59" /*  Limit/Balance available amount from MasterCard In Control	*/
#define		MDS_F054_AT_COPAY_AMOUNT		"80" /*	 Co-pay Amount						*/
#define		MDS_F054_AT_AVAILABLE_CREDIT	"90" /*	Available Credit					*/
#define		MDS_F054_AT_CREDIT_LIMIT		"91" /*	Credit Limit						*/
#define		MDS_F054_AT_LEN					2	
#define		MDS_F054_MAX_LEN				120

#define MDS_F060_PIN_TRSLT_ERR                           "294"  /* PIN translation unsuccessful */ /*AMER20160228: MCI Enh17.Q2*/

#define MDS_F061_S01_COUNTRY_CODE_OFST       13              /*POS Country Code*/
#define MDS_F061_S01_COUNTRY_CODE_LEN		 3
#define MDS_F061_S04_CH_PRES_OFST			 3		/* POS Cardholder Presence*//* ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define MDS_F061_S07_TRANS_STATUS_OFST		 6		/* POS Transaction Status					*/
#define MDS_F061_S09_TRANS_STATUS_EXT_OFST	 8	    /* POS Transaction Status – Extended	    *//*PLUTONL-9852 -Enh 2025Q2 NAB GLB 10432*/
#define MDS_F061_S10_CAT_LEVEL_OFST				9		/* Cardholder-Activated Terminal Level		*/
#define MDS_F061_S11_TERM_INP_CAP_OFST			10		/* POS Card Data Terminal Input Capability Indicator	*/


/*Start IBO20190808 PROD00070934 Enh19.Q4 AN 2628*/
#define MDS_F104_NB_FIELDS			          3
#define MDS_F104_DIGITAL_PAYMENT_CRYPTO				    "001"
/*Start HAL PROD00073823 20201102 Enh Q2.2020 Art 2941*/ 
#define MDS_F104_DGITAL_PAYEMENT_ESTIMATED_AMOUNT                "002"
#define MDS_F104_DGITAL_PAYEMENT_REMOTE_COMERCE_ACCEPTOR         "003"
/*End HAL PROD00073823 20201102 Enh Q2.2020 Art 2941*/ 
/*End IBO20190808 PROD00070934 Enh19.Q4 AN 2628*/

/*Start IBO20190910 PROD00071519 Enh19.Q4 AN2615*/
#define MDS_F108_RECIPIENT_DATA						"01"
#define MDS_F108_SENDER_DATA						"02"
#define MDS_F108_MONEYSEND_TRANSACTION_DATA			"03"
#define MDS_F108_MONEYSEND_LANGUAGE_DESCRIPTION		"04"
#define MDS_F108_DATA_VERIF_REQUEST	        		"07"/*PLUTONL-6505 -Enh 2023Q4 NAB :AN 7402*/ 
#define MDS_F108_DATA_VERIF_RESPONSE	        	"08"/*PLUTONL-6505 -Enh 2023Q4 NAB :AN 7402*/ 

/*Start AEA20210712 Enh21.Q4 AN4972*/ /*Transaction purpose values */
#define MDS_F108SE03_SB05_ORIGINAL_CARD					"12"	/* Fast refund to original card */
#define MDS_F108SE03_SB05_NEW_CARD						"13"	/* Fast refund to new card */
/*End AEA20210712 Enh21.Q4 AN4972*/ 

#define MDS_F108T01_RECIPIENT_FIRST_NAME				"01"
#define MDS_F108T01_RECIPIENT_MIDDLE_NAME				"02"
#define MDS_F108T01_RECIPIENT_LAST_NAME					"03"
#define MDS_F108T01_RECIPIENT_STREET_ADDR				"04"
#define MDS_F108T01_RECIPIENT_CITY						"05"
#define MDS_F108T01_RECIPIENT_STATE_PROVINCE_CODE		"06"
#define MDS_F108T01_RECIPIENT_COUNTRY					"07"
#define MDS_F108T01_RECIPIENT_POSTAL_CODE				"08"
#define MDS_F108T01_RECIPIENT_PHONE_NUMBER				"09"
#define MDS_F108T01_RECIPIENT_DOB						"10"
#define MDS_F108T01_RECIPIENT_ACCOUNT_NUMBER			"11"
#define MDS_F108T01_RECIPIENT_ACCOUNT_TYPE		    	"18"/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/


#define MDS_F108T02_SENDER_FIRST_NAME					"01"
#define MDS_F108T02_SENDER_MIDDLE_NAME					"02"
#define MDS_F108T02_SENDER_LAST_NAME					"03"
#define MDS_F108T02_SENDER_STREET_ADDR					"04"
#define MDS_F108T02_SENDER_CITY							"05"
#define MDS_F108T02_SENDER_STATE_PROVINCE_CODE			"06"
#define MDS_F108T02_SENDER_COUNTRY						"07"
#define MDS_F108T02_SENDER_POSTAL_CODE					"08"
#define MDS_F108T02_SENDER_PHONE_NUMBER					"09"
#define MDS_F108T02_SENDER_DOB							"10"
#define MDS_F108T02_SENDER_ACCOUNT_NUMBER				"11"
#define MDS_F108T02_SENDER_ACCOUNT_TYPE				    "18"/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/


#define MDS_F108T03_UNIQUE_TRANSACTION_REFERENCE		"01"
#define MDS_F108T03_ADDITIONAL_MESSAGE					"02"
#define MDS_F108T03_FUNDING_SOURCE						"03"
#define MDS_F108T03_PARTICIPANT_ID						"04"
#define MDS_F108T03_TRANSACTION_PURPOSE					"05"	/*AEA20210712 Enh21.Q4 AN4972*/


#define MDS_F108T04_LANGUAGE_IDENTIFICATION				"01"
#define MDS_F108T04_LANGUAGE_DATA						"02"

#define MDS_F108_T07_DATA_VERIF_TYPE	        		"01"/*PLUTONL-6505 -Enh 2023Q4 NAB :AN 7402*/

#define MDS_F108_NB_FIELDS			4
#define MDS_F108T01_NB_FIELDS		12/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
#define MDS_F108T02_NB_FIELDS		12/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
#define MDS_F108T03_NB_FIELDS		5 /*AEA20210712 Enh21.Q4 AN4972*/
#define MDS_F108T04_NB_FIELDS		2
#define	MDS_F108_MAX_LEN	999

/*End IBO20190910 PROD00071519 Enh19.Q4 AN2615*/


#define		MDS_PROCESSOR_ID            	"M01"
#define		MDS_ACQUIRER_ID             	"M02"
#define		MDS_ICA_ID                  	"M03"
#define		MDS_COUNTRY                 	"M04"
#define		MDS_ALPHA_CNTR_3            	"M05"
#define		MDS_ALPHA_CNTR_2            	"M06"
#define		MDS_BANK_LOCATION           	"M07"


#define MDS_F048_P87_CVC2_OK			'M'		/* Valid CVC 2 (match)											*/
#define MDS_F048_P87_CVC2_INV			'N'		/* Invalid CVC 2 (non-match)									*/
#define MDS_F048_P87_CVC2_NOPROC		'P'		/* CVC 2 not processed (issuer temporarily unavailable)			*/
#define MDS_F048_P87_CVC2_NOVERIF		'U'		/* CVC 2 Unverified MasterCard Use Only							*/

/*START PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	
#define MDS_F122_NB_FIELDS		        1
#define MDS_F122T001_001_NB_FIELDS		12

#define MDS_F122_SE_001_ADD_ACPT_DATA                 "001"

#define MDS_F122_ACPT_STREET_ADDR                     "001"
#define MDS_F122_ACPT_COUNTRY_SUB_CODE                "002"
#define MDS_F122_ACPT_URL_ADDR                        "003"
#define MDS_F122_ACPT_CUSTOMER_SVC_PHONE_NBR          "004"
#define MDS_F122_ACPT_PHONE_NBR                       "005"
#define MDS_F122_ACPT_ADD_CONTACT_INFO                "006"
#define MDS_F122_ACPT_TAX_ID                          "007"
#define MDS_F122_PARTNER_ID_CODE                      "008"
#define MDS_F122_SVC_LOC_CITY_NAME                    "009"
#define MDS_F122_SVC_LOC_COUNTRY_SUB_CODE             "010"
#define MDS_F122_SVC_LOC_COUNTRY_CODE                 "011"
#define MDS_F122_SVC_LOC_POSTAL_CODE                  "012"
/*END PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	

/*Start ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define MDS_CIT					"C1"
#define MDS_MIT_RECURRING		"M1"
#define MDS_MIT_INDUSTRY		"M2"

#define MDS_F061_SF4_STANDING_RECURRING 	'4'

/*End ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/

#endif


