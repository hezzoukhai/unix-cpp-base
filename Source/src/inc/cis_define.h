#ifndef CIS__DEFINE__H
#define CIS__DEFINE__H



#define		CIS_F002_MAX_LEN	19

#define		CIS_F003_PC_PURCHASE			"00" /* Purchase						*/
#define		CIS_F003_PC_WITHDRAWAL			"01" /* Withdrawal						*/
#define		CIS_F003_PC_DEBIT_ADJ			"02" /* Debit Adjustment				*/
#define		CIS_F003_PC_PURCHASE_CB			"09" /* Purchase with Cash Back			*/
#define		CIS_F003_PC_ACC_FUNDING			"10" /* Visa Only. Account Funding		*/
#define		CIS_F003_PC_CASH_ADV			"17" /* Cash Disbursement				*/
#define		CIS_F003_PC_SCRIPT_ISSUE		"18" /* Scrip Issue						*/
#define		CIS_F003_PC_REFUND				"20" /* Purchase Return/Refund			*/
#define		CIS_F003_PC_DEPOSIT				"21" /* Deposit							*/
#define		CIS_F003_PC_CREDIT_ADJ			"22" /* Credit Adjustment				*/
#define		CIS_F003_PC_CHECK_DEP_GUARANTY	"23" /* Check Deposit Guarantee			*/
#define		CIS_F003_PC_CHECK_DEPOSIT		"24" /* Check Deposit					*/
#define		CIS_F003_PC_PAYMENT				"28" /* Payment Transaction				*/
#define		CIS_F003_PC_BAL_INQ				"30" /* Balance Inquiry					*/
#define		CIS_F003_PC_ACC_TRANSFER		"40" /* Account Transfer				*/
#define		CIS_F003_PC_PIN_UNBLOCK			"91" /* PIN Unblock						*/
#define		CIS_F003_PC_PIN_CHANGE			"92" /* PIN Change						*/
#define		CIS_F003_PC_COF_TOKEN_PROC		"93" /* Card on File Token Processing	*/
#define		CIS_F003_PC_LEN					2	

#define		CIS_F003_LEN		6
#define		CIS_F004_LEN		12
#define		CIS_F005_LEN		12
#define		CIS_F006_LEN		12
#define		CIS_F007_LEN		10
#define		CIS_F008_LEN		8
#define		CIS_F009_LEN		8
#define		CIS_F010_LEN		8
#define		CIS_F011_LEN		6
#define		CIS_F012_LEN		6
#define		CIS_F013_LEN		4
#define		CIS_F014_LEN		4
#define		CIS_F015_LEN		4
#define		CIS_F016_LEN		4
#define		CIS_F017_LEN		4
#define		CIS_F018_LEN		4
#define		CIS_F019_LEN		3
#define		CIS_F020_LEN		3
#define		CIS_F021_LEN		3


#define CIS_F022_P1_UNKNOWN					"00"	/* PAN entry mode unknown				*/
#define CIS_F022_P1_MANUAL					"01"	/* PAN manual entry						*/
#define CIS_F022_P1_MAG						"02"	/* PAN auto-entry via magnetic stripe track data is not required.	*/
#define CIS_F022_P1_BC_READER				"03"	/* PAN auto-entry via bar code reader								*/
#define CIS_F022_P1_OCR						"04"	/* PAN auto-entry via optical character reader (OCR)				*/
#define CIS_F022_P1_CHIP					"05"	/* PAN auto-entry via chip											*/
#define CIS_F022_P1_CHIP_PAYPASS			"06"	/* PAN auto-entry via chip PayPass Mapping Service applied			*/
#define CIS_F022_P1_CONTACTLESS				"07"	/* PAN auto-entry via contactless M/Chip							*/
#define CIS_F022_P1_CL_PAYPASS				"08"	/* PAN auto-entry via contactless M/Chip PayPass Mapping Service applied	*/
#define CIS_F022_P1_ECOM_DSRP_ICC			"09"	/* PAN/Token entry via electronic commerce containing DSRP cryptogram in DE 55	*/
#define CIS_F022_P1_COF						"10"	/* Credential on File	*/
#define CIS_F022_P1_HYBRID					"79"	/* A hybrid terminal												*/
#define CIS_F022_P1_FALLBACK				"80"
#define CIS_F022_P1_ECOM_DSRP_UCAF			"81"	/* PAN/Token entry via electronic commerce with optional Identity Check-AAV or DSRP cryptogram in UCAF */
#define CIS_F022_P1_PAN_AUTO_ENTRY_SERVER	"82"	/* PAN Auto Entry via Server (issuer, acquirer, or third party vendor system) */
#define CIS_F022_P1_MAGNETIC				"90"	/* PAN auto-entry via magnetic stripe—the full track data has been read */
#define CIS_F022_P1_CL_MAG					"91"	/* PAN auto-entry via contactless magnetic stripe				*/
#define CIS_F022_P1_LEN						2



#define CIS_F022_P2_UNKNOWN					'0'		/* Unspecified or unknown								*/
#define CIS_F022_P2_HAS_PIN_ENTRY			'1'		/* Terminal has PIN entry capability					*/
#define CIS_F022_P2_NO_PIN_ENTRY			'2'		/* Terminal does not have PIN entry capability			*/
#define CIS_F022_P2_HAS_PE_NOOP				'8'		/* Terminal has PIN entry capability but PIN pad is not currently operative	*/
#define CIS_F022_P2_MPOS_SOFT_PIN                         '3'             /* mPOS Software-based PIN Entry Capability*//*ZKO:PROD00058820:ENH_OCT_2018_VISA_MCI*/



#define		CIS_F022_LEN		3
#define		CIS_F023_LEN		3
#define		CIS_F024_LEN		3
#define		CIS_F025_LEN		2
#define		CIS_F026_LEN		2
#define		CIS_F027_LEN		1
#define		CIS_F028_LEN		9
#define		CIS_F029_LEN		9
#define		CIS_F030_LEN		9
#define		CIS_F031_LEN		9
#define		CIS_F032_MAX_LEN	6
#define		CIS_F033_MAX_LEN	6
#define		CIS_F034_MAX_LEN	28
#define		CIS_F035_MAX_LEN	37
#define		CIS_F036_MAX_LEN	104
#define		CIS_F037_LEN		12
#define		CIS_F038_LEN		6



#define CIS_F039_APPROVAL			"00"	/* Approved or completed successfully					*/
#define CIS_F039_REF_TO_ISS			"01"	/* Refer to card issuer									*/
#define CIS_F039_INV_MER			"03"	/* Invalid merchant										*/
#define CIS_F039_CAPTURE_CARD		"04"	/* Capture card											*/
#define CIS_F039_DECLINE			"05"	/* Do not honor											*/
#define CIS_F039_APPROVAL_W_ID		"08"	/* Honor with ID										*/
#define CIS_F039_PARTIAL_APPROVAL	"10"	/* Partial Approval										*/
#define CIS_F039_INV_TRN			"12"	/* Invalid transaction									*/
#define CIS_F039_INV_AMOUNT			"13"	/* Invalid amount										*/
#define CIS_F039_INV_CRD_NBR		"14"	/* Invalid card number									*/
#define CIS_F039_INV_ISS			"15"	/* Invalid issuer										*/
#define CIS_F039_FMT_ERROR			"30"	/* Format error											*/
#define CIS_F039_LOST_CARD			"41"	/* Lost card											*/
#define CIS_F039_STOLEN_CARD		"43"	/* Stolen card											*/
#define CIS_F039_INSUF_FUNDS		"51"	/* Insufficient funds/over credit limit					*/
#define CIS_F039_EXPIRED_CARD		"54"	/* Expired card											*/
#define CIS_F039_INV_PIN			"55"	/* Invalid PIN											*/
#define CIS_F039_TRN_NOPERM_CH		"57"	/* Transaction not permitted to issuer/cardholder		*/
#define CIS_F039_TRN_NOPERM_TERM	"58"	/* Transaction not permitted to acquirer/terminal		*/
#define CIS_F039_WTDWL_LMT_EXC		"61"	/* Exceeds withdrawal amount limit						*/
#define CIS_F039_RESTRICTED_CARD	"62"	/* Restricted card										*/
#define CIS_F039_SEC_VIOLATION		"63"	/* Security violation									*/
#define CIS_F039_ISS_TIMEOUT		"82"	/* Timeout at issuer									*/
#define CIS_F039_NOT_DEC_AMNT_ZERO	"85"	/* Not declined Valid for all zero amount transactions.	*/
#define CIS_F039_PIN_VAL_NP			"86"	/* PIN Validation not possible							*/
#define CIS_F039_NO_CB				"87"	/* Purchase Amount Only, No Cash Back Allowed			*/
#define CIS_F039_CRYPTO_FAILURE		"88"	/* Cryptographic failure								*/
#define CIS_F039_ISS_UNV			"91"	/* Authorization Platform or issuer system inoperative	*/
#define CIS_F039_NOABLE_ROUTE		"92"	/* Unable to route transaction							*/
#define CIS_F039_DUP_TRANSM			"94"	/* Duplicate transmission detected						*/
#define CIS_F039_SYS_ERROR			"96"	/* System error											*/
#define CIS_F039_LEN				2


#define		CIS_F040_LEN		3
#define		CIS_F041_LEN		8
#define		CIS_F042_LEN		15
#define		CIS_F043_LEN		40
#define		CIS_F044_MAX_LEN	25
#define		CIS_F045_MAX_LEN	76
#define		CIS_F046_MAX_LEN	999
#define		CIS_F047_MAX_LEN	999


/*Start IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/
#define		CIS_F048_SE22_SF01_LOW_RISK_MER_IND			"01"
#define		CIS_F048_SE22_SF02_SINGLE_TAP_IND			"02"
#define		CIS_F048_SE22_SF03_RESPONSE_PIN_REQ			"03"
#define		CIS_F048_SE22_SF04_ISS_PIN_REQ_SIN_TAP		"04"
#define		CIS_F048_SE22_SF05_CH_MER_INIT_IND			"05" /*ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define	    CIS_F048_SE22_SF06_MSG_FORMAT_VERSION_CODE	"06"  /*PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/
#define 	CIS_F048_SE22_LEN						/*4*/ 6	/*ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/ /* START PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	/*change 5 to 6 */
#define		CIS_F048_SE22_SF01_MER_INIT_TRAN			"01"
#define		CIS_F048_SE22_SF01_ACQ_LOW_FRAUD_TRA		"02"
#define		CIS_F048_SE22_SF01_RECURRING_PYMT			"03"
#define		CIS_F048_SE22_SF01_LOW_VAL_PYMT				"04"
#define		CIS_F048_SE22_SF01_SCA_DELEGATION			"05"
#define		CIS_F048_SE22_SF01_SCP						"06"
#define		CIS_F048_SE22_SF01_AUTH_OUTAGE_EXCEPTION	"07" /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4040*/

/*End IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/

/*Start ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/

#define	 CIS_F048_SE22_SF05_CIT_CREDENTIAL			"C101"
#define	 CIS_F048_SE22_SF05_CIT_STANDING            "C102"
#define	 CIS_F048_SE22_SF05_CIT_SUBSCRIPTION        "C103"
#define	 CIS_F048_SE22_SF05_CIT_INSTALLMENT         "C104"
#define	 CIS_F048_SE22_SF05_MIT_UNSCHEDULED         "M101"
#define	 CIS_F048_SE22_SF05_MIT_STANDING_ORDER      "M102"
#define	 CIS_F048_SE22_SF05_MIT_SUBSCRIPTION        "M103"
#define	 CIS_F048_SE22_SF05_MIT_INSTALLMENT         "M104"
#define	 CIS_F048_SE22_SF05_MIT_PARTIAL_SHIPMENT    "M205"
#define	 CIS_F048_SE22_SF05_MIT_DELAYED_CHARGE      "M206"
#define	 CIS_F048_SE22_SF05_MIT_NO_SHOW_CHARGE      "M207"
#define	 CIS_F048_SE22_SF05_MIT_RESUBMISSION        "M208"
#define  CIS_F048_SE22_SF05_LEN						  4

/*End ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/

/* START PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/
#define  CIS_F048_SE22_SF06_LEN						         1
#define	 CIS_MSG_FORMAT_VERSION_CODE_PRE		            "A"
#define	 CIS_MSG_FORMAT_VERSION_CODE_POST		            "B"
/* END PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/

/*Start HAL25022021 PLUTONL-2632 ART 4224 2021.Q2 */
#define CIS_F048_SE37_SF01_PAYFAC_ID			        "01"  	/** Payment facilitator ID	**/
#define CIS_F048_SE37_SF02_ISO_ID   			        "02"	/** Independent sales organization ID **/
#define CIS_F048_SE37_SF03_SUB_MER_ID 			        "03"	/** Sub merchant ID	 **/
#define CIS_F048_SE37_SF04_ORIGINE_COUNTRY  			"04"  	/** Merchant origine’s country **/ 
#define CIS_F048_SE37_NB_FIELDS							 4
/*End HAL25022021 PLUTONL-2632 ART 4224 2021.Q2 */

/*Start HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/ 
#define		CIS_F048_SE71_SF01_MC_DIG_ENABL_SERV_DIG_PAY_DATA_VALID		"54"
/*End HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/ 

/*Start HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 4221*/ 
#define		CIS_F048_SE71_SF01_MER_VAL_SERV		"55" 
/*End HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 4221*/  

#define		CIS_F048_T33_T01_ACC_NO_IND			"01"
#define		CIS_F048_T33_T02_ACC_NO				"02"
#define		CIS_F048_T33_T03_EXP_DT				"03"
#define		CIS_F048_T33_T04_PRODUCT_CD			"04"
#define		CIS_F048_T33_T05_TOKEN_ASS_LVL		"05"
#define		CIS_F048_T33_T06_TOKEN_REQ_ID		"06"
#define		CIS_F048_T33_T07_PAN_RANGE			"07"
#define		CIS_F048_T33_T08_STORAGE_TECH		"08"         /*ACH20190910 PROD00070986*/

#define		CIS_F048_T33_T01_MDES_DEV_TKN			'C'		/* MasterCard Digital Enablement Service Device token			*/
#define		CIS_F048_T33_T01_EMB_ISS_PAN			'E'		/* Embossed Account Number Provided by Issuer					*/
#define		CIS_F048_T33_T01_MDES_COF				'F'		/* MasterCard Digital Enablement Service Card on File Token		*/
#define		CIS_F048_T33_T01_LPO_CRD				'L'		/* Pay with Rewards Loyalty Program Operator [LPO] card			*/
#define		CIS_F048_T33_T01_PAN					'M'		/* Primary Account Number										*/
#define		CIS_F048_T33_T01_CL_ACC_NO				'P'		/* Contactless Account Number									*/
#define		CIS_F048_T33_T01_REWARDS_CRD			'R'		/* Pay with Rewards card										*/
#define		CIS_F048_T33_T01_VC						'V'		/* Virtual Card Number											*/

#define		CIS_F048_SE42_SF3_MISSING_UCAF			'0'		/* Missing Universal Cardholder Authentication Field (UCAF)	    */	/*AMER20170928 PROD00047410 Enh17.Q4*/
#define		CIS_F048_SE42_SF3_INVALID_UCAF			'1'		/* Invalid Universal Cardholder Authentication Field (UCAF)	    */	/*AMER20170928 PROD00047410 Enh17.Q4*/
#define     CIS_F048_SE42_SF3_INVALID_SLI           '2'     /* Invalid Security Level Indicator                             */  /*ELKAM20230202 ENH_2023Q2 AN 7090 */

#define CIS_F048_P21_SF1_LEN                    1
#define CIS_F048_P21_SF1_OFST                  4
#define CIS_F048_P21_SF1_MPOS_TERM            '0'  /*Dedicated mPOS Terminal with PCI compliant dongle*/
#define CIS_F048_P21_SF1_SHELF_MDEV           '1'  /*Off the Shelf Mobile Device*/


/*Subelement 66.Authentication Data*/

#define CIS_F048_P66_LEN		        45  /*Authentication Data*/

/*Program Protocol*/
#define CIS_F048_P66_SF1	              "01"
#define CIS_F048_P66_SF1_LEN                    1
#define CIS_F048_P66_SF1_LENG                  "01"  
#define CIS_F048_P66_SF1_OFST                   4
/*#define CIS_F048_P66_SF1_3DSEC_1_0             '1' */                               /*3D Secure Version 1.0 [3DS 1.0]*/  
/*#define CIS_F048_P66_SF1_3DSEC_2_0             '2' */ /*EMV 3-D c [3DS 2.0] */      /* OBSOLETE ELKAM20230202 ENH_2023Q2 AN 7090 */

/* START ELKAM20230202 ENH_2023Q2 AN 7090 */
#define CIS_F048_P66_SF1_3DSEC_2_1           	'1'  
#define CIS_F048_P66_SF1_3DSEC_2_2           	'2'  
#define CIS_F048_P66_SF1_3DSEC_2_3           	'3'  
#define CIS_F048_P66_SF1_3DSEC_2_4           	'4'  
#define CIS_F048_P66_SF1_3DSEC_2_5           	'5'  
#define CIS_F048_P66_SF1_3DSEC_2_6           	'6'  
#define CIS_F048_P66_SF1_3DSEC_2_7           	'7'  
#define CIS_F048_P66_SF1_3DSEC_2_8           	'8'  
#define CIS_F048_P66_SF1_3DSEC_2_9           	'9'
/* END ELKAM20230202 ENH_2023Q2 AN 7090 */

/*Directory Server Transaction ID*/
#define CIS_F048_P66_SF2                      "02"
#define CIS_F048_P66_SF2_LEN                   36
#define CIS_F048_P66_SF2_LENG                  "36"
#define CIS_F048_P66_SF2_OFST                   4





#define CIS_F048_P77_P2P				"C01" /* Person-to-Person						*/
#define CIS_F048_P77_MCI_REBATE			"C02" /* MasterCard rebate						*/
#define CIS_F048_P77_RPWR				"C03" /* rePower Load Value						*/
#define CIS_F048_P77_GAMING_REPAY		"C04" /* Gaming Re-pay							*/
#define CIS_F048_P77_OTHER_PAY			"C05" /* Payment Transaction for a reason other than those defined in values C01 C04	*/
#define CIS_F048_P77_CR_CRD_PAY			"C06" /* Payment of a credit card balance with cash or check	*/
#define CIS_F048_P77_MON_SND_P2P		"C07" /* MoneySend Person-to-Person				*/
#define CIS_F048_P77_CARD_ACTV			"C09" /* Card Activation						*/
#define CIS_F048_P77_MON_SND_A2A		"C52" /* MoneySend Account-to-Account Transfers	*/
#define CIS_F048_P77_MON_SND_AGT_CSH	"C53" /* MoneySend Agent Cash Out				*/
#define CIS_F048_P77_MON_SND_CR_CRD_PAY "C54" /* MoneySend Credit Card Bill Payment		*/
#define CIS_F048_P77_MON_SND_B2C		"C55" /* MoneySend Business to Consumer Disbursement	*/
#define CIS_F048_P77_MON_SND_G2C		"C56" /* MoneySend Government/Non-Government Organization to Consumer Disbursement		*/
#define CIS_F048_P77_MS_ACQ_MER_SETTL	"C57" /* MoneySend Acquirer Merchant Settlement */
#define CIS_F048_P77_MON_SND_B2B		"C65" /* MoneySend Business to Business Transfer*//* IBO20190910 PROD00071519 Enh19.Q4 AN2615*/
#define CIS_F048_P77_IP_P2P				"C67" /* Inter Platform Person-to-Person		*/
#define CIS_F048_P77_LEN				3


#define CIS_F048_P87_INV_CVC1			'Y'		/* Invalid CVC 1 (only if DE 35 (Track 2 Data) or DE 45 (Track 1 Data) is 
												* present in the Authorization Request/0100 message.)
												*/
#define CIS_F048_P87_CVC2_OK			'M'		/* Valid CVC 2 (match)											*/
#define CIS_F048_P87_CVC2_INV			'N'		/* Invalid CVC 2 (non-match)									*/
#define CIS_F048_P87_CVC2_NOPROC		'P'		/* CVC 2 not processed (issuer temporarily unavailable)			*/
#define CIS_F048_P87_CVC2_NOVERIF		'U'		/* CVC 2 Unverified MasterCard Use Only							*/
#define CIS_F048_P87_CVC3_INV_UN_LEN	'E'		/* Length of unpredictable number was not a valid length	*/
#define CIS_F048_P87_CVC3_NOPROC		'P'		/* Unable to process										*/
#define CIS_F048_P87_CVC3_INV			'Y'		/* Invalid													*/

#define CIS_F124_ISS_PRDCT_CONFIG_ID	10
#define CIS_F124_PAN_SEQ_NBR			3

/* Storage Technology values*/
#define CIS_F048_SE33_SF08_SERVER		"03" 	/* Server */
#define CIS_F048_SE33_SF08_TEE 			"04" 	/* Trusted Execution Environment (TEE) */
#define CIS_F048_SE33_SF08_SE			"05"	/* Secure Element (SE) */

/* Wallet Program Data values*/
#define CIS_F048_SE26_SF01_APPLE_PAY 		"103"
#define CIS_F048_SE26_SF01_GOOGLE_PAY 		"216"
#define CIS_F048_SE26_SF01_SAMSUNG_PAY 		"217"
#define CIS_F048_SE26_SF01_MER_TOK_PROGRAM 	"327"	/*Merchant tokenization program*/	/*PLUTONL-2222*/

#define CIS_F048_SE78_LEN 						6	/*AEA20210712 Enh21.Q4 AN4779*/
#define CIS_F048_SE78_SF02_DCC_IND				1	/*AEA20210712 Enh21.Q4 AN4779*/
#define CIS_F048_SE78_SF05_MSG_REASON_CODE		4	/*AEA20210712 Enh21.Q4 AN4779*/

/* START PLUTONL-5548   AN6773  Enh 2023Q2 AHA20221221*/
#define CIS_F048_SE57_SF01_SEC_SERV_IND_LEN   3   /*Security Services Indicator*/
#define CIS_F048_SE57_SF02_SEC_SERV_DATA_LEN  3   /*Security Services Data*/
/* END PLUTONL-5548   AN6773  Enh 2023Q2 AHA20221221*/
/* ELKAM20221221 PLUTONL-5555 START */
#define CIS_F048_SE75_FRAUD_SCORE			"01"
#define CIS_F048_SE75_SCORE_REASON_CODE 	"02"
#define CIS_F048_SE75_RULES_CODE 			"03"
#define CIS_F048_SE75_RULES_REASON_CODE1 	"04"
#define CIS_F048_SE75_RULES_REASON_CODE2 	"05"
#define CIS_F048_SE75_LEN 					5
/* ELKAM20221221 PLUTONL-5555 END */

#define P7_F120_SE_01_AVS_IND_1_LEN		2
#define P7_F120_SE_01_AVS_IND_1_LEN_LEN		2
#define P7_F120_SE_01_ZIP_CODE_LEN		9
#define P7_F120_SE_01_ADR_LEN			20

#define		CIS_F048_MAX_LEN	999
#define		CIS_F049_LEN		3
#define		CIS_F050_LEN		3
#define		CIS_F051_LEN		3
#define		CIS_F052_LEN		16
#define		CIS_F053_LEN		16

#define		CIS_F054_AT_LG_BAL				"01" /* Ledger Balance						*/
#define		CIS_F054_AT_AVAILABLE			"02" /*  Available Balance					*/
#define		CIS_F054_AT_AMNT_OWING			"03" /*  Amount Owing						*/
#define		CIS_F054_AT_AMOUNT_DUE			"04" /*  Amount Due							*/
#define		CIS_F054_AT_HC_ELG_AMNT			"10" /*  Healthcare Eligibility Amount		*/
#define		CIS_F054_AT_PRSCPT_ELG_AMNT		"11" /*  Prescription Eligibility Amount	*/
#define		CIS_F054_AT_ONL_BILL_PAY_FEE	"17" /*  MasterCard Prepaid Online Bill Pay Transaction Fee Amount	*/
#define		CIS_F054_AT_CB_AMNT				"40" /*  Amount Cash Back					*/
#define		CIS_F054_AT_GRATUITY_AMNT		"44" /*  Amount Gratuity					*/
#define		CIS_F054_AT_ORIG_AMNT			"57" /*  Original Amount					*/
#define		CIS_F054_AT_DCC_ORIG_AMNT		"58" /*  Dynamic Currency Conversion (DCC) transactions	*/	/*AMER20170928 PROD00047410 Enh17.Q4*/
#define		CIS_F054_AT_LIM_MCI_IC			"59" /*  Limit/Balance available amount from MasterCard In Control 	*/
#define		CIS_F054_AT_LEN		2	



#define		CIS_F054_MAX_LEN	120
#define		CIS_F055_MAX_LEN	255
#define		CIS_F056_MAX_LEN	999
#define		CIS_F060_MAX_LEN	60


#define CIS_F061_S01_AUTH_LIFE_CYCLE_LEN		2
#define CIS_F061_S01_COUNTRY_CODE_LEN			3
#define CIS_F061_S01_POSTAL_CODE_MAX_LEN		10


#define CIS_F061_S01_TERM_ATT_OFST				0		/* POS Terminal Attendance					*/
#define CIS_F061_S01_POS_TERM_LOC_OFST			2		/* POS Terminal Location					*/
#define CIS_F061_S01_CH_PRES_OFST				3		/* POS Cardholder Presence					*/
#define CIS_F061_S01_CARD_PRES_OFST				4		/* POS Card Presence						*/
#define CIS_F061_S01_CARD_CAPTURE_CAP_OFST		5		/* POS Card Capture Capabilities			*/
#define CIS_F061_S01_TRANS_STATUS_OFST			6		/* POS Transaction Status					*/
#define CIS_F061_S01_TRANS_SECURITY_OFST		7		/* POS Transaction Security					*/
#define CIS_F061_S01_TRANS_STATUS_EXT_OFST		8		/* POS Transaction Status – Extended	    *//*PLUTONL-9852 -Enh 2025Q2 NAB GLB 10432*/				
#define CIS_F061_S01_CAT_LEVEL_OFST				9		/* Cardholder-Activated Terminal Level		*/
#define CIS_F061_S01_TERM_INP_CAP_OFST			10		/* POS Card Data Terminal Input Capability Indicator	*/
#define CIS_F061_S01_AUTH_LIFE_CYCLE_OFST		11		/* POS Authorization Life Cycle				*/
#define CIS_F061_S01_COUNTRY_CODE_OFST			13		/* POS Country Code							*/
#define CIS_F061_S01_POSTAL_CODE_OFST			16		/* POS Postal Code							*/

#define CIS_F061_KEY_EXCH_REQ                                   "161"           /* Encryption key exchange request*/

#define		CIS_F061_MAX_LEN	26
#define		CIS_F062_MAX_LEN	100
#define		CIS_F063_MAX_LEN	50
#define		CIS_F064_LEN		8
#define		CIS_F065_LEN		8
#define		CIS_F066_LEN		1
#define		CIS_F067_LEN		2
#define		CIS_F068_LEN		3
#define		CIS_F069_LEN		3
#define		CIS_F070_LEN		3
#define		CIS_F071_LEN		4
#define		CIS_F072_LEN		4
#define		CIS_F073_LEN		6
#define		CIS_F074_LEN		10
#define		CIS_F075_LEN		10
#define		CIS_F076_LEN		10
#define		CIS_F077_LEN		10
#define		CIS_F078_LEN		10
#define		CIS_F079_LEN		10
#define		CIS_F080_LEN		10
#define		CIS_F081_LEN		10
#define		CIS_F082_LEN		12
#define		CIS_F083_LEN		12
#define		CIS_F084_LEN		12
#define		CIS_F085_LEN		12
#define		CIS_F086_LEN		16
#define		CIS_F087_LEN		16
#define		CIS_F088_LEN		16
#define		CIS_F089_LEN		16
#define		CIS_F090_LEN		42
#define		CIS_F091_LEN		1
#define		CIS_F092_LEN		2
#define		CIS_F093_LEN		5
#define		CIS_F094_LEN		7
#define		CIS_F095_LEN		42
#define		CIS_F096_LEN		8
#define		CIS_F097_LEN		17
#define		CIS_F098_LEN		25
#define		CIS_F099_MAX_LEN	11
#define		CIS_F100_MAX_LEN	11
#define		CIS_F101_MAX_LEN	17
#define		CIS_F102_MAX_LEN	28
#define		CIS_F103_MAX_LEN	28
#define		CIS_F104_MAX_LEN	999
#define		CIS_F108_MAX_LEN	999
#define		CIS_F112_MAX_LEN	100
#define		CIS_F120_MAX_LEN	999
#define		CIS_F121_MAX_LEN	6
#define		CIS_F122_MAX_LEN	999
#define		CIS_F123_MAX_LEN	512
#define		CIS_F124_MAX_LEN	199
#define		CIS_F125_LEN		16
#define		CIS_F126_MAX_LEN	100
#define		CIS_F127_MAX_LEN	100
#define		CIS_F128_LEN		8

/*Start IBO20190808 PROD00070934 Enh19.Q4 AN 2628*/
#define CIS_F104_NB_FIELDS			3 /*1--->3*/
#define CIS_F104_DIGITAL_PAYMENT_CRYPTO					"001"
/*Start HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/ 
#define CIS_F104_DGITAL_PAYEMENT_ESTIMATED_AMOUNT         "002"
#define CIS_F104_DGITAL_PAYEMENT_REMOTE_COMERCE_ACCEPTOR  "003"
/*End IBO20190808 PROD00070934 Enh19.Q4 AN 2628*/
/*Start PROD00025867*/
#define CIS_F108_RECIPIENT_DATA						"01"
#define CIS_F108_SENDER_DATA						"02"
#define CIS_F108_MONEYSEND_TRANSACTION_DATA			"03"
#define CIS_F108_MONEYSEND_LANGUAGE_DESCRIPTION		"04"
/*start IBE PLUTONL-6616*/
#define CIS_F108_DIGITAL_ACC_INFO 					"05"
#define CIS_F108_QR_DYNAMIC_CODE_DATA 				"06"
/*end IBE PLUTONL-6616*/
#define CIS_F108_DATA_VERIF_REQUEST	        		"07"/*PLUTONL-6505 -Enh 2023Q4 NAB :AN 7402*/ 
#define CIS_F108_DATA_VERIF_RESPONSE	        	"08"/*PLUTONL-6505 -Enh 2023Q4 NAB :AN 7402*/ 


#define CIS_F108T01_RECIPIENT_FIRST_NAME				"01"
#define CIS_F108T01_RECIPIENT_MIDDLE_NAME				"02"
#define CIS_F108T01_RECIPIENT_LAST_NAME					"03"
#define CIS_F108T01_RECIPIENT_STREET_ADDR				"04"
#define CIS_F108T01_RECIPIENT_CITY						"05"
#define CIS_F108T01_RECIPIENT_STATE_PROVINCE_CODE		"06"
#define CIS_F108T01_RECIPIENT_COUNTRY					"07"
#define CIS_F108T01_RECIPIENT_POSTAL_CODE				"08"
#define CIS_F108T01_RECIPIENT_PHONE_NUMBER				"09"
#define CIS_F108T01_RECIPIENT_DOB						"10"
#define CIS_F108T01_RECIPIENT_ACCOUNT_NUMBER			"11"
#define CIS_F108T01_RECIPIENT_ACCOUNT_TYPE				"18"/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/


#define CIS_F108T02_SENDER_FIRST_NAME					"01"
#define CIS_F108T02_SENDER_MIDDLE_NAME					"02"
#define CIS_F108T02_SENDER_LAST_NAME					"03"
#define CIS_F108T02_SENDER_STREET_ADDR					"04"
#define CIS_F108T02_SENDER_CITY							"05"
#define CIS_F108T02_SENDER_STATE_PROVINCE_CODE			"06"
#define CIS_F108T02_SENDER_COUNTRY						"07"
#define CIS_F108T02_SENDER_POSTAL_CODE					"08"
#define CIS_F108T02_SENDER_PHONE_NUMBER					"09"
#define CIS_F108T02_SENDER_DOB							"10"
#define CIS_F108T02_SENDER_ACCOUNT_NUMBER				"11"
#define CIS_F108T02_SENDER_ACCOUNT_TYPE					"18"/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/


#define CIS_F108T03_UNIQUE_TRANSACTION_REFERENCE		"01"
#define CIS_F108T03_ADDITIONAL_MESSAGE					"02"
#define CIS_F108T03_FUNDING_SOURCE						"03"
#define CIS_F108T03_PARTICIPANT_ID						"04"
#define CIS_F108T03_TRANSACTION_PURPOSE					"05"	/*AEA20210712 Enh21.Q4 AN4972*/

/*Start AEA20210712 Enh21.Q4 AN4972*/ /*Transaction purpose values */
#define CIS_F108SE03_SB05_ORIGINAL_CARD					"12"	/* Fast refund to original card */
#define CIS_F108SE03_SB05_NEW_CARD						"13"	/* Fast refund to new card */
/*End AEA20210712 Enh21.Q4 AN4972*/ 

#define CIS_F108T04_LANGUAGE_IDENTIFICATION				"01"
#define CIS_F108T04_LANGUAGE_DATA						"02"
#define CIS_F108_T07_DATA_VERIF_TYPE	        		"01"/*PLUTONL-6505 -Enh 2023Q4 NAB :AN 7402*/


#define CIS_F108_NB_FIELDS			4
#define CIS_F108T01_NB_FIELDS		12/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
#define CIS_F108T02_NB_FIELDS		12/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
#define CIS_F108T03_NB_FIELDS		5 /*AEA20210712 Enh21.Q4 AN4972*/
#define CIS_F108T04_NB_FIELDS		2
#define CIS_F110_NB_FIELDS          2  /* AHA20220921 PLUTONL-4980*/
#define CIS_F110_AD_KEY_BLOCK_PROTECT        "09"/* AHA20220921 PLUTONL-4980*/
#define CIS_F110_AD_KEY_CHECK_VALUE          "10"/* AHA20220921 PLUTONL-4980*/
/*End PROD00025867*/


/*START PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	
#define CIS_F122_NB_FIELDS	     	1
#define CIS_F122T001_001_NB_FIELDS		12

#define CIS_F122_SE_001_ADD_ACPT_DATA                     "001"

#define CIS_F122_ACPT_STREET_ADDR                   "001"
#define CIS_F122_ACPT_COUNTRY_SUB_CODE              "002"
#define CIS_F122_ACPT_URL_ADDR                      "003"
#define CIS_F122_ACPT_CUSTOMER_SVC_PHONE_NBR        "004"
#define CIS_F122_ACPT_PHONE_NBR                     "005"
#define CIS_F122_ACPT_ADD_CONTACT_INFO              "006"
#define CIS_F122_ACPT_TAX_ID                        "007"
#define CIS_F122_PARTNER_ID_CODE                    "008"
#define CIS_F122_SVC_LOC_CITY_NAME                  "009"
#define CIS_F122_SVC_LOC_COUNTRY_SUB_CODE           "010"
#define CIS_F122_SVC_LOC_COUNTRY_CODE               "011"
#define CIS_F122_SVC_LOC_POSTAL_CODE                "012"
/*END PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	

/*Start IBO20200606 RawNtwMsg*/
#define CIS_F55_DED_FILE_NAME 	  "84"
#define CIS_F55_TERM_APP_VER_NUM "9F09"
#define CIS_F55_IFD_SERIAL_NUM   "9F1E"
#define CIS_F55_CRYPTO_INFO_DATA "9F27"
#define CIS_F55_CVM              "9F34"
#define CIS_F55_TERM_TYPE        "9F35"
#define CIS_F55_TRANS_SEQ_NUM    "9F41"
#define CIS_F55_TRANS_CATEG_CODE "9F53"
#define CIS_F55_ISS_SCRIPT1 	 "71"
#define CIS_F55_ISS_SCRIPT2      "72"
#define CIS_F55_APP_INTER_PROFILE "82"
#define CIS_F55_ISS_AUTHEN_DATA   "91"
#define CIS_F55_TVR 			  "95"
#define CIS_F55_TRANS_DATE        "9A"
#define CIS_F55_TRANS_TYPE        "9C"
#define CIS_F55_TRANS_CUR_CODE    "5F2A"
#define CIS_F55_TRANS_AMOUNT      "9F02"
#define CIS_F55_OTHER_AMOUNT      "9F03"
#define CIS_F55_ISS_APP_DATA      "9F10"
#define CIS_F55_TERM_COUNTRY_CODE "9F1A"
#define CIS_F55_APP_CRYPTOGRAM    "9F26"
#define CIS_F55_TERM_CAP          "9F33"
#define CIS_F55_ATC               "9F36"
#define CIS_F55_UNPRED_NUMBER     "9F37"

#define CIS_F55_NB_FIELDS				  24
#define CIS_F48_NB_FIELDS				  64

/*F48*/
#define CIS_F48_ENC_PIN_BLK_KEY "10"
#define CIS_F48_KEY_EXCH_BLK_DATA "11"
#define CIS_F48_ROUTING_IND "12"
#define CIS_F48_MCI_HOSTED_MPTP_REQ_DATA "13"
#define CIS_F48_AUTH_PLATFORM_ADV_DT_TIME "15"
#define CIS_F48_PROC_PSEUDO_ICA "16"
#define CIS_F48_SERVICE_PARAMS "18"
#define CIS_F48_CH_VERIF_MTHD "20"
#define CIS_F48_PAY_INIT_CHANNEL "23"
#define CIS_F48_MCI_CASH_PROGRAM_DATA "25"
#define CIS_F48_WALLET_PROGRAM_DATA "26"
#define CIS_F48_MCI_ASSIGNED_ID "32"
#define CIS_F48_PAN_MAPPING_FILE_INFO "33"
#define CIS_F48_DYN_CVC3_ATC_INFO "34"
#define CIS_F48_CL_NONCARD_FF_REQ_RESP "35"
#define CIS_F48_VISA_MVV "36"
#define CIS_F48_ADD_MER_DATA "37"
#define CIS_F48_ACC_CATEGORY "38"
#define CIS_F48_EXP_MON_ACC_SERV_INFO "39"
#define CIS_F48_ECOM_MER_CH_CERT_SNO "40"
#define CIS_F48_ECOM_CERT_QUAL_INFO "41"
#define CIS_F48_ECOM_IND "42"
#define CIS_F48_UCAF "43"
#define CIS_F48_VISA_3DS_XID "44"
#define CIS_F48_VISA83DS_SECT_RESP_CD "45"
#define CIS_F48_PRODUCT_ID "46"
#define CIS_F48_MCI_PAY_GW_IND "47"
#define CIS_F48_MOBILE_PROGRAM_IND "48"
#define CIS_F48_MER_ON_BEHALF_SERVICES "51"
#define CIS_F48_MER_FRAUD_SCORING_DATA "55"
#define CIS_F48_SECURITY_SERVICES_ADD_DATA_ISS "56"
#define CIS_F48_SECURITY_SERVICES_ADD_DATA_ACQ "57"
#define CIS_F48_ATM_ADD_DATA "58"
#define CIS_F48_POS_DATA_EXT_COND_CD "61"
#define CIS_F48_TRACE_ID "63"
#define CIS_F48_TRANSIT_PROGRAM "64"
#define CIS_F48_ON_BEHALF_SERVICES "71"
#define CIS_F48_ISS_CHIP_AUTH "72"
#define CIS_F48_MCI_INTERNAL_USE "73"
#define CIS_F48_ADD_PROC_INFO "74"
#define CIS_F48_FRAUD_SCORING_DATA "75"
#define CIS_F48_MCI_E_ACCEP_IND "76"
#define CIS_F48_FUNDING_PAY_TRN_TYPE_IND "77"
#define CIS_F48_PAYMENT_SERVICES_IND "78"
#define CIS_F48_CVR_TVR_ERR_RESULT "79"
#define CIS_F48_PIN_SERVICE_CODE "80"
#define CIS_F48_AVS_REQ "82"
#define CIS_F48_AVS_RESP "83"
#define CIS_F48_MER_ADVICE_CODE "84"
#define CIS_F48_ACCOUNT_STATUS "85"
#define CIS_F48_REL_PARTICIPANT_IND "86"
#define CIS_F48_CARD_VAL_CODE_RES "87"
#define CIS_F48_MAG_COMPL_STATUS_IND "88"
#define CIS_F48_MAG_COMPL_ERR_IND "89"
#define CIS_F48_LODGING_AUTO_RENT_IND "90"
#define CIS_F48_CPS_TRN_ID "91"
#define CIS_F48_CVC2 "92"
#define CIS_F48_FLEET_CARD_ID_REQ_DATA "93"
#define CIS_F48_COM_CARD_INQ_REQ "94"
#define CIS_F48_MCI_PROMO_CODE "95"
#define CIS_F48_VISA_MARKET_DATA_ID "96"
#define CIS_F48_PRESTIGIOUS_PROP_IND "97"
#define CIS_F48_MCI_CORP_FLEET_CARD_ID_DRIVER_ID "98"
#define CIS_F48_MCI_CORP_FLEET_CARD_VEHICLE_NO "99"
/*End IBO20200606 RawNtwMsg*/
#define CIS_SEC_DATA "9701010001000000"  /*PLUTONL-2389*/

/*Start ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define CIS_CIT					"C1"
#define CIS_MIT_RECURRING		"M1"
#define CIS_MIT_INDUSTRY		"M2"

#define CIS_F061_SF4_CH_NOT_PRESENT 		'3'
#define CIS_F061_SF4_STANDING_RECURRING 	'4'

/*End ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/

#endif