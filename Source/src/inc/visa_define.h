#ifndef VISA_DEFINE__H
#define VISA_DEFINE__H

#define B1_F003_P1_PURCHASE				"00"	/* Goods/Service Purchase POS transaction only			*/
#define B1_F003_P1_WITHDRAWAL			"01"	/* Withdrawal/Cash Advance								*/
#define B1_F003_P1_CHECK_ACCEPTANCE		"03"	/* Check Acceptance										*/
#define B1_F003_P1_ACC_FUNDING			"10"	/* Account Funding										*/
#define B1_F003_P1_QUASICASH			"11"	/* Quasi-Cash Transaction (POS transaction only)		*/
#define B1_F003_P1_REFUND				"20"	/* Return of Goods Credit Credit Voucher or Merchandise Return Authorization (U.S. only)	*/
#define B1_F003_P1_ORIG_CREDIT			"26"	/* Original Credit										*/
#define B1_F003_P1_PP_ACTIV_N_LD		"28"	/* Prepaid Activation & Load Prepaid Load				*/
#define B1_F003_P1_BALANCE_INQ			"30"	/* Balance/Available Funds Inquiry						*/
#define B1_F003_P1_ELIGIBILITY_INQ		"39"	/* Eligibility Inquiry									*/
#define B1_F003_P1_CH_ACC_TRSF			"40"	/* Cardholder Account Transfer							*/
#define B1_F003_P1_BILL_PAYMENT			"50"	/* Bill Payment (U.S. only)								*/
#define B1_F003_P1_PAYMENT				"53"	/* Payment (U.S. only)									*/
#define B1_F003_P1_PIN_CHG_UNBLOCK		"70"	/* PIN Change/Unblock									*/
#define B1_F003_P1_PIN_UNBLOCK_PP_ACT	"72"	/* PIN Unblock Prepaid Activation						*/
#define B1_F003_P1_LEN					2


#define B1_F003_P2_UNSPECIFIED			"00"	/* Not Applicable or Not Specified						*/
#define B1_F003_P2_SAVING_ACC			"10"	/* Savings Account										*/
#define B1_F003_P2_CHECKING_ACC			"20"	/* Checking Account										*/
#define B1_F003_P2_CR_CARD_ACC			"30"	/* Credit Card Account									*/
#define B1_F003_P2_UNIVERSAL_ACC		"40"	/* Universal Account (represented by a cardholder identification number)	*/
#define B1_F003_P2_LEN					2

#define B1_F003_P3_UNSPECIFIED			"00"	/* Not Applicable or Not Specified						*/
#define B1_F003_P3_SAVING_ACC			"10"	/* Savings Account										*/
#define B1_F003_P3_CHECKING_ACC			"20"	/* Checking Account										*/
#define B1_F003_P3_CR_CARD_ACC			"30"	/* Credit Card Account									*/
#define B1_F003_P3_UNIVERSAL_ACC		"40"	/* Universal Account (represented by a cardholder identification number)	*/
#define B1_F003_P3_LEN					2
#define B1_F003_LEN					6
#define B1_F004_LEN					12
#define B1_F005_LEN					12
#define B1_F006_LEN					12
#define B1_F012_LEN					6
#define B1_F018_LEN					4

#define B1_F022_P1_CHIP				"05"	/* Integrated circuit card read; CVV or iCVV checking is possible	*/
#define B1_F022_P1_MANUAL			"01"	/* Manual key entry												*/
#define B1_F022_P1_MAG_UNR			"02"	/* Magnetic stripe read; CVV checking may not be possible	*/
#define B1_F022_P1_CL_CHIP			"07"	/* Contactless payment using VSDC chip rules	*/
#define B1_F022_P1_MAG				"90"	/* Magnetic stripe read and exact content of Track 1 or Track 2 included (CVV check is possible).*/
#define B1_F022_P1_CL_MAG			"91"	/* Contactless payment using magnetic stripe data rules */
#define B1_F022_P1_LEN				2



#define B1_F025_NORMAL_TRN			"00"	/* Normal transaction of this type								*/
#define B1_F025_CH_NP				"01"	/* Cardholder not present										*/
#define B1_F025_CP_PIN				"02"	/* Card and cardholder present, PIN entered						*/
#define B1_F025_MER_SUSPICIOUS		"03"	/* Merchant suspicious of transaction or card					*/
#define B1_F025_CH_P_CRD_NP			"05"	/* Cardholder present, card not present							*/
#define B1_F025_COMPLETION			"06"	/* Completion advice											*/
#define B1_F025_MOTO_RECURRING		"08"	/* Mail/phone order/recurring transaction						*/
#define B1_F025_ID_VERIFIED			"10"	/* Customer identity verified (used for Check Acceptance only)	*/
#define B1_F025_ACC_VERIF			"51"	/* Request for account number verification without authorization*/
#define B1_F025_PUB_ECOM			"59"	/* E-commerce request through public network					*/
#define B1_F025_CRD_P_MG_UNREAD		"71"	/* Card present, magnetic stripe cannot be read (key-entered) U.S. only	*/
#define B1_F025_LEN					2

/*Start IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/
#define VISA_F34_NB_FIELDS			             8   /*7*/ /*6*//*4*/ /*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/ /*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
#define B1_F34_ACCEPTANCE_ENV_ADD_DATA			"02"
#define B1_F34_EU_PSD2_SCA_CSC					"4A" 	/*EU PSD2 Strong Consumer Authentication and Common and Secure Communication*/
#define B1_F34_AUTHEN_DATA			            "01"
#define B1_F34_SUPP_DATA		 			    "56"	
#define B1_F34_DEVICE_PLATFORM_DATA		 		"06"  /*Device Platform Data*//*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
#define B1_F34_DEVICE_COMMON_DATA		 		"07"  /*Device Common Data*//*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/	
#define VISA_F34_T02_NB_FIELDS					15			/*2 ---> 3 IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/	/*AEA20210712 Enh21.Q4 Art9.1.2*/		/*3 ---> 10 SDR20230714 PLUTONL-6643 Enh23Q4 VISA Art 3.5*/ /*PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
#define B1_F34T02_INITIATING_PARTY_ID			"80"
#define B1_F34T02_AUTH_OUTAGE_INDICATOR         "87"	/*AEA20210712 Enh21.Q4 Art9.1.2*/
/*End IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.4*/

/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/
#define B1_F34T02_CRDNTL_MER_RELATIONSHIP_IND    "88"


#define B1_F34T02_T88_NO_LINK					'0'		     /*No link*/
#define B1_F34T02_T88_LINKED				    '1'		     /*Linked*/
/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/

/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
#define	  B1_F34T02_PAYFAC_STREET_ADDR	        "D1"
#define	  B1_F34T02_PAYFAC_CITY_NAME	        "D2"
#define	  B1_F34T02_PAYFAC_STATE_CODE	        "D3"
#define	  B1_F34T02_PAYFAC_POSTAL_CODE	        "D4"
#define	  B1_F34T02_PAYFAC_CTRY_CODE	        "D5"
/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/

/*Session id HAL 20202003 ENHS 2020Q2 ART 9.3.4*/
#define VISA_F34_T56_NB_FIELDS                   11/*9*/ /*7*/ /*6*/  /*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*//*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*//*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
#define B1_F34T56_SESSION_ID		 			"81"

#define B1_F34T56_CAVV_TYPE		 			    "9F29"	 /*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
#define B1_F34T56_CAVV_VERSION_NUMBER           "9F28"   /*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
#define B1_F34T56_VISA_RISK_AUTHEN_SCORE        "9F22"   /*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
#define B1_F34T56_DEVICE_ID_VELOCITY_COUNT      "9F21"   /*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
#define B1_F34T56_IP_ADDR_VELOCITY_COUNT        "9F20"   /*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
#define B1_F34T56_VCAS_SCORE                    "DF1F"   /*VCAS Score*//*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
#define B1_F34T56_THIRD_PARTY_ID                "80"    /*Third Party Identification*//*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
#define B1_F34T56_THIRD_PARTY_RISK_SCORE        "DF22"  /*Third Party Risk Score*//*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
/*Start IBO20190719 PROD00070403 Enh19.Q4 Art 9.1.3*/
#define VISA_F34_T4A_NB_FIELDS		7
#define B1_F34T4A_TRST_MER_EXEM		 			"84"	/*Trusted Merchant Exemption                                                */
#define B1_F34T4A_LOW_VAL_EXEM					"87"    /*Low Value Exemption                                                       */
#define B1_F34T4A_SPC							"88"	/*Secure Corporate Payment                                                  */
#define B1_F34T4A_TRA							"89"	/*Transaction Risk Analysis                                                 */
#define B1_F34T4A_DELG_AUTH						"8A"	/*Delegated Authentication Program                                          */
#define B1_F34T4A_REASON_HONOR_EXEM 			"8C"	/*HAL 20201803 ENHS 2020Q2 ART 9.3.3                                        */
#define B1_F34T4A_TRA_SCORE          			"C0"	/*HAL  ENHS 2020Q2 ART 9.3.4                                        */

/*End IBO20190718 PROD00070403 Enh19.Q4 Art 9.1.3*/

/*Start HAL PROD00073823 20202401 Enh Q2.2020 Art 3.10*/
#define VISA_F34_T01_NB_FIELDS					6/*8*/ /*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
#define B1_F34T01_PROTOCOL_VERSION_NUMBER	    "86"
/*End HAL PROD00073823 20202401 Enh Q2.2020 Art 3.10*/

#define B1_F34T01_AUTHEN_DATA_QUALITY_INDC	     "C1" /*PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
#define B1_F34T01_BROWSER_IP_ADDRESS        	 "89" /*DS Browser IP Address*//*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
#define B1_F34T01_3DS_APP_IP_ADDRESS             "92" /*3DS App IP Address*//*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
#define B1_F34T01_SHIPPING_INDICATOR             "93" /*Shipping Indicators*//*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/
#define B1_F34T02_CRDNTL_MER_RELATIONSHIP_IND    "88"


#define B1_F34T02_T88_NO_LINK					'0'		     /*No link*/
#define B1_F34T02_T88_LINKED				    '1'		     /*Linked*/
/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/

/*start SDR20230714 PLUTONL-6643 Enh23Q4 VISA Art 3.5*/
#define B1_F34T02_ACC_STREET_ADDR	        	"C0"
#define	B1_F34T02_ACQ_NAME		        		"C1"
#define	B1_F34T02_ACQ_STREET_ADDR	        	"C2"
#define B1_F34T02_ACQ_CITY_NAME	        		"C3"
#define B1_F34T02_ACQ_STATE_CODE	        	"C4"
#define B1_F34T02_ACQ_POSTAL_CODE         		"C5"
#define B1_F34T02_ACQ_COUNTRY_CODE        		"C6"
/*end SDR20230714 PLUTONL-6643 Enh23Q4 VISA Art 3.5*/

/*Start PLUTONL-5548  Enh 2023Q2 Visa 1.2 AHA20221221*/
#define   B1_F34T01_AUTHENTICATION_PROGRAM    	"C0"
#define   VISA_DAF    	"01"
/*End PLUTONL-5548  Enh 2023Q2  Visa 1.2 AHA20221221*/   

/*Start AOU24082023*/
/*Start PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/
#define B1_F34_ADD_SERVICE_REQUEST_DATA			"03"
#define B1_F34_ADD_SERVICE_RESULT_DATA			"04"
/*End PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/
/*End AOU24082023*/

#define B1_F039_APPROVAL			"00"	/* Successful approval/completion or that V.I.P. PIN verification is valid */
#define B1_F039_REF_TO_ISS			"01"	/* Refer to card issuer												*/
#define B1_F039_REF_TO_ISS_SP		"02"	/* Refer to card issuer, special condition							*/
#define B1_F039_INVALID_MER			"03"	/* Invalid merchant or service provider								*/
#define B1_F039_PICKUP_CARD			"04"	/* Pickup card														*/
#define B1_F039_DECLINE				"05"	/* Do not honor														*/
#define B1_F039_ERROR				"06"	/* Error															*/
#define B1_F039_PICKUP_CARD_SP		"07"	/* Pickup card, special condition (other than lost/stolen card)		*/
#define B1_F039_PARTIAL_APPROVAL	"10"	/* Partial Approval													*/
#define B1_F039_VIP_APPROVAL		"11"	/* V.I.P. approval													*/
#define B1_F039_INVALID_TRN			"12"	/* Invalid transaction												*/
#define B1_F039_INVALID_AMNT		"13"	/* Invalid amount (currency conversion field overflow); or amount exceeds maximum for card program */
#define B1_F039_INVALID_ACC_NBR		"14"	/* Invalid account number (no such number)							*/
#define B1_F039_NO_ISSUER			"15"	/* No such issuer													*/
#define B1_F039_REENTER_TRN			"19"	/* Re-enter transaction												*/

#define B1_F039_SUSPECTED_FRAUD		"59"	/* Suspected fraud													*/
#define B1_F039_CRYPTOGRAPHIC_ERROR	"81"	/* Invalid Key/check value      FZL20220826 PLUTONL-4979            */
#define B1_F039_NO_RSN_TO_DECLINE	"85"	/*	No reason to decline a request for account number
												verification, address verification, CVV2 verification,
												or a credit voucher or merchandise return						*/
#define B1_F039_PIN_VERIF_UNVL			"86"	/* Cannot Verify PIN											*/
#define B1_F039_ISS_UNV					"91"	/* Issuer unavailable											*/
#define B1_F039_DEST_NOTFOUND			"92"	/* Destination cannot be found for routing						*/
#define B1_F039_TRN_COMPL				"93"	/* Transaction cannot be completed; violation of law			*/
#define B1_F039_SYSTEM_MALFUNCTION		"96"	/* System malfunction											*/
#define B1_F039_SURCHARGE_AMNT_NOPERM	"B1"	/* Surcharge amount not permitted on Visa cards (U.S. acquirers only)	*/
#define B1_F039_FORCE_STIP				"N0"	/* Force STIP													*/
#define B1_F039_CASH_SERVICE_UNVL		"N3"	/* Cash service not available									*/
#define B1_F039_CB_EXC_ISS_LIM			"N4"	/* Cashback request exceeds issuer limit						*/
#define B1_F039_CVV2_FAILURE			"N7"	/* Decline for CVV2 failure										*/
#define B1_F039_INV_BILL_INF			"P2"	/* Invalid biller information									*/
#define B1_F039_PIN_CHG_DEC				"P5"	/* PIN Change/Unblock request declined							*/
#define B1_F039_UNSAFE_PIN				"P6"	/* Unsafe PIN													*/
#define B1_F039_STOP_PAY_ORDER			"R0"	/* Stop Payment Order											*/
#define B1_F039_REVOC_AUTH_ORDER		"R1"	/* Revocation of Authorization Order							*/
#define B1_F039_REVOC_ALL_AUTH_ORDER	"R3"	/* Revocation of All Authorizations Order						*/
#define B1_F039_NO_ONL_DECLINE			"Z3"	/* Unable to go online; declined								*/
#define B1_F039_FWD_TO_ISS_A			"XA"	/* Forward to issuer											*/
#define B1_F039_FWD_TO_iSS_D			"XD"	/* Forward to issuer											*/
#define B1_F039_CARD_AUTH_FAILURE		"Q1"	/* Card Authentication failed									*/
#define B1_F039_LEN					2


#define B1_F044_P10_CVV2_MATCH			'M'		/* CVV2 Match													*/
#define B1_F044_P10_CVV2_NOMATCH		'N'		/* CVV2 No Match												*/
#define B1_F044_P10_CVV2_NOPROC			'P'		/* Not processed												*/
#define B1_F044_P10_CVV2_IS_ON_CRD		'S'		/* CVV2 should be on the card									*/
#define B1_F044_P10_ISS_NOT_ILL			'U'		/* Issuer does not participate in CVV2 service or 
													participates but has not provided Visa with encryption keys, or both */



#define		B1_F054_ACCT_TYPE_CREDIT_ACCT	"30" /*  Credit Card Account				*/   /*AMER20171116 PROD00049633*/
#define     B1_F054_ACCT_TYPE_UN			"00" /*Not applicable or not specified      */   /*NAB05042020 PLUTONL-1573*/
#define		B1_F054_ACCT_TYPE_LEN			2										        /*AMER20171116 PROD00049633*/

#define		B1_F054_AT_LG_BAL				"01" /*  Ledger Balance						*/
#define		B1_F054_AT_AVAILABLE			"02" /*  Available Balance					*/
#define		B1_F054_AT_AMOUNT_SURCHARGE		"42" /*  Amount Surcharge					*/
#define		B1_F054_AT_CUMUL_AMNT			"43" /*  Total cumulative amount			*/	/*AMER20170426: PROD00041736*/
#define		B1_F054_AT_MEMBER_FEE			"56" /*  Member-provided fee				*/
#define		B1_F054_AT_ORIG_AMNT			"57" /*  Original Amount					*/
#define		B1_F054_AT_VMT					"95" /*  Visa money transfer (VMT)			*/
#define		B1_F054_AT_LEN		2	

#define		B1_F060_POS8_INSTALL_PAY	    "03" /*  Installment payment			    */	/*AMER20171123: PROD00049633*/	  
#define		B1_F060_POS8_LEN 			2

/*Start IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define B1_F048_P64_P4_MIT			    '1'
#define B1_F048_P64_P4_CIT				'2'
/*End IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/

#define B1_F070_SIGNON				"071"		/* Sign-on to the Base I and V.I.P. Systems, start BASE I and V.I.P. processing	*/
#define B1_F070_SIGNOFF				"072"		/* Sign-off from the Base I and V.I.P. Systems, terminate BASE I and V.I.P. processing	*/
#define B1_F070_START_SAF			"078"		/* Start transmission of both BASE I and V.I.P. advices		*/
#define B1_F070_STOP_SAF			"079"		/* Stop transmission of both BASE I and V.I.P. advices		*/
#define B1_F070_AWK_REQUEST 		"160"       /* Request AWK  FZL20220826 PLUTONL-4979*/
#define B1_F070_IWK_REQUEST 		"161"       /* Request IWK  FZL20220826 PLUTONL-4979*/
#define B1_F070_AWK_INCOMING		"162"       /* Receive AWK  FZL20220826 PLUTONL-4979*/
#define B1_F070_IWK_INCOMING		"163"       /* Receive IWK  FZL20220826 PLUTONL-4979*/
#define B1_F070_ECHO_TEST			"301"		/* Echo test	*/
#define B1_F070_LEN					3



#define VISA_F063_NB_FIELDS				21
#define VISA_F063_RSN_CD_LEN			4                     /* HAL PROD00073823 20200402 Enh Q2.2020 Art 1.1*/

#define MSG_REASON_INC_AUTH 				"3900"  	          /* Incremental Authorization */
#define MSG_REASON_RESUBMISSION				"3901"                    /* Resubmission		     */
#define MSG_REASON_DELAY_CHARGES			"3902"                    /* Delayed Charges	     */
#define MSG_REASON_REAUTH				    "3903"                    /* Reauthorization           */
#define MSG_REASON_NO_SHOW				    "3904"                    /* No Show		     */
#define MSG_REASON_ACC_TOP_UP               "3905"                    /* Account Top Up            */
#define MSG_REASON_COMPLETION_ADVICE 	    "3906"		           /*AFD Completion Advice*/		/* HAL PROD00073823 20200402 Enh Q2.2020 Art 1.1*/
#define MSG_REASON_DEFERRED_AUTH 			"5206"			/* Deferred Authorization*/  /*IBO20190823 PROD00071113 Enh19.Q4 Art 2.1*/

#define B1_F063_NETWORK_ID				0
#define B1_F063_PREAUTH_TIME_LIMIT		1
#define B1_F063_MSG_RSN_CODE			2
#define B1_F063_STIP_SWITCH_RSN_CD		3
#define B1_F063_FEE_PROGRAM_IND			18


#define B1_F104_PURCHASE_LINE_ITEM_DATA			"02"
/*#define B1_F104_DIAL_TERMINAL_DATA				"56"*/
#define B1_F104_PAY_FAC_DATA					"56"		/*AMER20190219 PROD00065991: Enh19.Q2*/		
#define B1_F104_BUSINESS_APP_ID					"57"
#define B1_F104_HEALTHCARE_ELIGIBILITY_INQ		"58"
#define B1_F104_PROMOTION_DATA					"59"
#define B1_F104_VISA_RISK_ASSESSMENT_DATA		"5B"
#define B1_F104_COMMERCIAL_CARD_DATA			"5C"
#define B1_F104_INSTALLMENT_PAYMENT_DATA		"5D"
#define B1_F104_SENDER_DATA						"5F"
#define B1_F104_AIRLINE_DATA					"60"
#define B1_F104_CAR_RENTAL_DATA					"61"
#define B1_F104_LODGING_DATA					"62"
#define B1_F104_NON_INDUSTRY_SPECIFIC_DATA		"63"
#define B1_F104_MASTERCARD_MEMBER_DEF_DATA		"65"
#define B1_F104_AMEX_DATA						"66"
#define B1_F104_MULTIPLE_PAYMENT_FORMS			"69"
#define B1_F104_TRAVEL_TAG_DATA					"6C"
#define B1_F104_ISSUER_SUPPLIED_DATA			"6D"
#define B1_F104_LOAN_DETAILS					"6E"
#define B1_F104_FREE_FORM_DESCRIPTION_DATA		"71"
#define B1_F104_ADDITIONAL_SENDER_DATA			"71"
#define B1_F104_FREE_FORM_TEXT_ORG_CR_TRN		"71"
#define B1_F104_RECURRING_PAY					"18"	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

/*Start AMER20190219 PROD00065991: Enh19.Q2 Art2.4*/	
#define B1_F104T56_PAY_FAC_ID					"01"
#define B1_F104T56_SUB_MER_ID					"02"
#define B1_F104T56_ISO_ID						"03"
/*End AMER20190219 PROD00065991: Enh19.Q2 Art2.4*/
/*#define B1_F104T56_TRANSFER_SERVC_FOR_OCT       "81"    /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
#define B1_F104T56_ACC_LEGAL_BUSINESS_NAME      "81"     /*ZKO 05042022 PLUTONL-4550 ENH 22.Q2 VISA Art3.2*/
#define B1_F104T56_PAY_FACILITATOR              "82"    /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/

#define B1_F104T57_BUSINESS_APP_ID				"01"
#define B1_F104T57_FUNDS_SOURCE					"02"
#define B1_F104T57_PAY_PURPOSE					"82"	/*AEA20210712 Enh21.Q4 Art2.9*/
/*Start PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
#define B1_F104_T57_BUSINESS_APP_ID		 		"01"
#define B1_F104_T57_SERVICE_PROC_TYPE 			"80"
#define B1_F104_T57_DEFER_OCT_REQ_TYPE 			"81"
#define B1_F104_T57_PAY_PURPOSE		 			"82"
#define B1_F104_T57_MAX_PROC_DATE				"83"    
#define B1_F104_T57_PT_SRC_FUNDS 				"02"
/*End PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/

#define B1_F104T5F_SENDER_REF_NBR				"01"
#define B1_F104T5F_SENDER_ACCOUNT_NO			"02"
#define B1_F104T5F_SENDER_NAME					"03"
#define B1_F104T5F_SENDER_STREET_ADDR			"04"
#define B1_F104T5F_SENDER_CITY_NAME				"05"
#define B1_F104T5F_SENDER_STATE					"06"
#define B1_F104T5F_SENDER_COUNTRY				"07"
#define B1_F104T5F_FUNDS_SOURCE					"08"
#define B1_F104T5F_RECIPIENT_NAME				"0A"

#define B1_F104T71_FREE_FORM_DATA				"01"


#define B1_F104T57_AA 		"AA" /* Account to account1									*/
#define B1_F104T57_BB		"BB" /* Business to business								*/
#define B1_F104T57_BI		"BI" /* Money transfer bank-initiated						*/
#define B1_F104T57_BP		"BP" /* Non-card bill payment								*/
#define B1_F104T57_CC		"CC" /* Cash claim											*/
#define B1_F104T57_CI		"CI" /* Cash in												*/
#define B1_F104T57_CO		"CO" /* Cash out											*/
#define B1_F104T57_CP		"CP" /* Card bill payment									*/
#define B1_F104T57_FD		"FD" /* Funds disbursement (general)						*/
#define B1_F104T57_GD		"GD" /* Government disbursement								*/
#define B1_F104T57_GP		"GP" /* Gambling payout (other than online gambling)		*/
#define B1_F104T57_MD		"MD" /* Merchant disbursement								*/
#define B1_F104T57_MI		"MI" /* Money transfer merchant-initiated					*/
#define B1_F104T57_OG		"OG" /* Online gambling payout								*/
#define B1_F104T57_PD		"PD" /* Payroll/pension disbursement						*/
#define B1_F104T57_PG		"PG" /* Payment to government								*/
#define B1_F104T57_PP		"PP" /* Person to person									*/
#define B1_F104T57_PS		"PS" /* Payment for goods and services (general)			*/
#define B1_F104T57_TU		"TU" /* Top-Up for enhanced prepaid loads					*/
#define B1_F104T57_LEN		2

/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
#define B1_F104T18_RECURRING_PAY_TYPE					"80"
#define B1_F104T18_PAY_AMNT_IND_PER_TRANS				"81"
#define B1_F104T18_NBR_RECURRING_PAY					"82"
#define B1_F104T18_FREQ_RECURRING_PAY					"83"
#define B1_F104T18_REGISTR_REF_NBR						"84"
#define B1_F104T18_MAX_RECURRING_PAY_AMNT				"85"
#define B1_F104T18_VALIDATION_IND						"86"


#define B1_F104T18_T80_REGISTRATION_FIRST_TRANS			"1"
#define B1_F104T18_T80_SUBSEQ_TRANS						"2"
#define B1_F104T18_T80_MODIFICATION						"3"
#define B1_F104T18_T80_CANCELLATION						"4"

#define B1_F104T18_T81_FIX_AMNT_IND_PER_TRANS			"1"
#define B1_F104T18_T81_RECUR_PAY_MAX_AMNT				"2"
/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

/*Start ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
#define B1_F104T18_RECURRING_PAY_TYPE_LEN					1
#define B1_F104T18_PAY_AMNT_IND_PER_TRANS_LEN				1
#define B1_F104T18_NBR_RECURRING_PAY_LEN					1  /* 2 -> 1 FZL 05102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17 */
#define B1_F104T18_FREQ_RECURRING_PAY_LEN					1  /* 2 -> 1 FZL 05102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17 */
#define B1_F104T18_REGISTR_REF_NBR_LEN						18 /* 35 -> 18 FZL 05102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17 */
#define B1_F104T18_MAX_RECURRING_PAY_AMNT_LEN				6  /* 12 -> 6 FZL 05102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17 */
#define B1_F104T18_VALIDATION_IND_LEN						1
/*End ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

#define VISA_F056_T05_NB_FIELDS										19 /*PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
#define	  B1_F056T05_ACC_OWNER_NATIONALITY						    "8F"
#define	  B1_F056T05_ACC_OWNER_CTRY_OF_BIRTH						"90"
#define	  B1_F056T05_ACC_OWNER_OCCUPATION						    "91"
#define	  B1_F056T05_ACC_OWNER_DATE_OF_BIRTH						"92"
#define	  B1_F056T05_ACC_OWNER_EMAIL_ADDS						    "97"
/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
#define VISA_F034_T02_NB_FIELDS					15 		/*2 >>> 9 SDR20230714 PLUTONL-6643 Enh23Q4 VISA Art 3.5*/	/*PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/		


/* start FZL 20220822 PLUTONL-4979*/
#define VISA_F110_NB_FIELDS			1
#define B1_F110T04_KEY_DATA  	  "04"

#define VISA_F110_T04_NB_FIELDS     7
#define B1_F110_T04_CONTROL       "80"
#define B1_F110_T04_KEY_IDENT     "81"
#define B1_F110_T04_ALGO          "83"
#define B1_F110_T04_KEY_PROTECT   "85"
#define B1_F110_T04_KEY_IND       "86"
#define B1_F110_T04_ENCT_DATA     "87"
#define B1_F110_T04_CHECK_VALUE   "88"
/* end FZL 20220822 PLUTONL-4979*/

/*Start PLUTONL-9204 AMANS Enh24Q4 VISA AN 1.1*/
#define VISA_F111_NB_FIELDS 				3 
#define B1_F111_RATE_INFO 					"01"
#define B1_F111_JURISD_SETTL_DATA 			"02"
#define B1_F111_RESP_CLEARING_DATA 			"56" 
#define VISA_F111_T56_NB_FIELDS              6 
#define B1_F111_T56_ACCOUNT_FUNDING_SRC      "80"
#define B1_F111_T56_APPLIED_ACI              "82"
#define B1_F111_T56_APPLIED_MARKED_SPEC_DATA "84"
#define B1_F111_T56_APPLIED_CH_ID_METHOD     "85"
#define B1_F111_T56_APPLIED_ECI 			 "86"
#define B1_F111_T56_DOWNGRADE_REASON_CODE    "88"
/*End PLUTONL-9204 AMANS Enh24Q4 VISA AN 1.1*/

#define B1_F126_P10_CVV2_NP			'0'			/* CVV2 value not provided							*/
#define B1_F126_P10_CVV2_P			'1'			/* CVV2 value is present							*/
#define B1_F126_P10_CVV2_ILGBL		'2'			/* CVV2 value is on the card but is illegible		*/
#define B1_F126_P10_NO_CVV2			'9'			/* No CVV2 value on card							*/

/*Start IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/
#define B1_F126_POS_ENV					12		/* POS environnement */

/*#define B1_F126_P13_POS_ENV			'0'		 POS environnement is not present	*/
/*#define B1_F126_P13_POS_ENV_P			'1'			 POS environnement is present		*/

#define B1_F126_P13_CREDENTIAL		   	'C'	  	
#define B1_F126_P13_RECURRING			'R'	
#define B1_F126_P13_INSTALLMENT			'I'	
/*End IZL 20220201 PLUTONL-4015 ENH 22Q2 MC AN5524*/

#define VISA_F104_NB_FIELDS			23			/*22--> 23 IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
#define VISA_F104_T56_NB_FIELDS		5			/*AMER20190219 PROD00065991: Enh19.Q2*/   /*3-->5 IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
#define VISA_F104_T57_NB_FIELDS		6			/*AEA20210712 Enh21.Q4 Art2.9*/ /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
#define VISA_F104_T5F_NB_FIELDS		9
#define VISA_F104_T71_NB_FIELDS		1
#define VISA_F104_T18_NB_FIELDS		7			/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

#define VISA_F034_T01_NB_FIELDS					6 /*PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/



#define		B1_F123_T68_PROP			"VISA_F123_T68_PROP"
#define		B1_F123_T66_PROP			"VISA_F123_T66_PROP"

#define		B1_F123_ADDR_VERIF_DATA		"66"
#define		B1_F123_TOKEN_DATA			"68"
#define     B1_F123_T66_NB_FIELDS        3  /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
#define		B1_F123_T66_POSTAL_CD		"C0"
#define		B1_F123_T66_STREE_ADDR		"CF"
#define     B1_F123_T66_CH_SHIP_HASH    "D6"     /* Cardholder Shipping Hash -->  CH_SHIP_HASH = CARDHOLDER_SHIPPING_HASH */ /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
#define		VISA_F123_T68_NB_FIELDS		10 	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/  /* PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221  change nb of fields from 9->10*/

#define		B1_F123_T68_TOKEN			"01"
#define		B1_F123_T68_TKN_ASS_LVL		"02"
#define		B1_F123_T68_TKN_REQ_ID		"03"
#define		B1_F123_T68_TKN_RANGE		"04"

/*Start HAL2402 PLUTONL-4391*/
#define		B1_F123_T68_TKN_REFERENCE_ID	"05"
#define		B1_F123_T68_TKN_EXPIRATION_DATE	"06"
#define		B1_F123_T68_TKN_TYPE			"07"
#define		B1_F123_T68_TKN_STATUS			"08"   
#define     B1_F123_T68_TKN_AUTHENTICATION_AMNT    "85"          /* Token Authentication Amount */ /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
/*End HAL2402 PLUTONL-4391*/
#define		B1_F123_T68_AUTO_FILL_IND		"0D" 	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art2.2*/

/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
#define  VISA_F125_T01_NB_FIELDS                1
#define B1_F125_TOKEN_DEVICE                   "01"   
#define B1_F125_T01_IP_ADDR                    "07"            
         
/*End PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
/*Start IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
#define VISA_F125_NB_FIELDS			3	/*1 --> 2 IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.1*/ /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221 change 2->3*/
#define VISA_F125_T03_NB_FIELDS		1
#define B1_F125_ADD_ORIGINAL_DATA			"03"
#define B1_F125T03_ORIGINAL_TRA_ID			"03"
/*End IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/

/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.1*/
#define VISA_F125_T02_NB_FIELDS				 3   /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
#define B1_F125_WALLET_PROVIDER				"02"
#define B1_F125T02_WP_DEVICE_SCORE         "05" /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
#define B1_F125T02_WP_ACCOUNT_SCORE        "06" /*PLUTONL-5548  Enh 2023Q2  Visa 3.3 AHA20221221*/
#define B1_F125T02_OVERALL_ASSESSMENT		"80"
/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.1*/

#define		BASE1_STATION_ID            "B01"
#define		BASE1_ACQUIRER_ID           "B02"
#define		BASE1_COUNTRY               "B03"
#define		BASE1_ALPHA_CNTR_1          "B04"
#define		BASE1_ALPHA_CNTR_2          "B05"
#define		BASE1_BANK_LOCATION         "B06"


/*Start IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/
#define VISA_F056_NB_FIELDS			/*1*/ 4
#define VISA_F056_T01_NB_FIELDS										2
#define VISA_F056_T02_NB_FIELDS										2
#define VISA_F056_T03_NB_FIELDS										10
#define VISA_F056_T05_NB_FIELDS										19 /*PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/

#define B1_F056_ACCOUNT_DATA										"01"
#define B1_F056_CONTACT_INFO										"02"
#define B1_F056_CUSTOMER_ID_DATA									"03"
#define B1_F056_ACCOUNT_OWNER_DATA									"05" /*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*//*To push at SPX*/

#define B1_F056T01_PAY_ACCNT_REF									"01"
#define B1_F056T01_PAY_ACCNT_REF_CREATION_DATE						"02"

#define B1_F056T02_OTHER_PHONE_NBR									"83"
#define B1_F056T02_OTHER_EMAIL_ADD									"86"



#define B1_F056T03_DATA_SENDER_RECIP			"9F1F"
#define B1_F056T03_ID_TYPE_CODE					"9F20"
#define B1_F056T03_ID_SUBTYPE_CODE				"9F21"
#define B1_F056T03_ACQ_POPULATED_ID_VAL			"9F22"
#define B1_F056T03_CUSTOMER_ID_ISS_CNTRY_CODE	"9F24"

#define B1_F056T05_ACCOUNT_REF_CODE									"80"
#define B1_F056T05_ACCOUNT_OWNER_TYPE								"81"/*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*//*To push at SPX*/
#define B1_F056T05_ACCOUNT_OWNER_NAME_GIVEN							"83"/*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*//*To push at SPX*/
#define B1_F056T05_ACCOUNT_OWNER_NAME_MIDDLE						"84"/*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*//*To push at SPX*/
#define B1_F056T05_ACCOUNT_OWNER_NAME_LAST							"85"/*PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*//*To push at SPX*/
#define B1_F056T05_ACCOUNT_OWNER_ADDR_LINE1							"86"
#define B1_F056T05_ACCOUNT_OWNER_ADDR_LINE2							"87"
#define B1_F056T05_ACCOUNT_OWNER_STREET_NAME						"88"
#define B1_F056T05_ACCOUNT_OWNER_BUILDING_NBR						"89"
#define B1_F056T05_ACCOUNT_OWNER_POSTAL_CODE						"8A"
#define B1_F056T05_ACCOUNT_OWNER_CITY_NAME							"8B"
#define B1_F056T05_ACCOUNT_OWNER_COUNTRY_SUBDIV_CODE_MINOR			"8C"
#define B1_F056T05_ACCOUNT_OWNER_COUNTRY_SUBDIV_CODE_MAJOR			"8D"
#define B1_F056T05_ACCOUNT_OWNER_COUNTRY_CODE						"8E"

/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
#define	  B1_F056T05_ACC_OWNER_NATIONALITY						    "8F"
#define	  B1_F056T05_ACC_OWNER_CTRY_OF_BIRTH						"90"
#define	  B1_F056T05_ACC_OWNER_OCCUPATION						    "91"
#define	  B1_F056T05_ACC_OWNER_DATE_OF_BIRTH						"92"
#define	  B1_F056T05_ACC_OWNER_EMAIL_ADDS						    "97"
/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/

#define B1_F056T03_9F1F_05						"05"		
#define B1_F056T03_9F1F_06						"06"			

#define B1_F056T03_9F1F_LEN						2       
/*End IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art3.2*/


/*Start IZL 05052022 PLUTONL-4675*/
#define VISA_F126_NB_FIELDS  			19


#define B1_BIT_F126_Reserved1 0
#define B1_BIT_F126_Reserved2 1
#define B1_BIT_F126_Reserved3 2
#define B1_BIT_F126_Reserved4 3
#define B1_BIT_F126_VISA_MERCH_ID 4
#define B1_BIT_F126_CH_CERT_SERIAL_NO 5
#define B1_BIT_F126_MER_CERT_SERIAL_NO 6
#define B1_BIT_F126_TRN_ID_XID 7
#define B1_BIT_F126_CAVV_DATA 8
#define B1_BIT_F126_CVV2_AUTH_DATA 9
#define B1_BIT_F126_Reserved5 10
#define B1_BIT_F126_SERVICE_IND 11
#define B1_BIT_F126_POS_ENV 12
#define B1_BIT_F126_Reserved6 13
#define B1_BIT_F126_MCI_UCAF_COLL_IND 14
#define B1_BIT_F126_MCI_UCAF_FIELD 15
#define B1_BIT_F126_Reserved7 16
#define B1_BIT_F126_AGENT_UNIQE_ACC_RES 17
#define B1_BIT_F126_DCC_IND 18
/*End IZL 05052022 PLUTONL-4675*/
/*F034_T01*/
#define VISA_F034_T01_NB_FIELDS					6 /*PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/
#define B1_F034_T01_3DS_PROTO_VERSION_NUMBER 	"86"
#define B1_F034_T01_3DS_BROWSER_IP 				"89"
#define B1_F034_T01_3DS_APP_IP 					"92"
#define B1_F034_T01_SHIPPING_INDICATOR 			"93"
#define B1_F034_T01_AUTHENTICATION_PROGRAM 		"C0"

#define VISA_F034_T02_NB_FIELDS					15 		/*2 >>> 9 SDR20230714 PLUTONL-6643 Enh23Q4 VISA Art 3.5*/	/*PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/		
#define B1_F034_T02_INITIATING_PARTY_IND 		"80"
#define B1_F034_T02_ACCEPTANCE_ENV_AUTH_OUT_IND "87"

/*F034_T4A*/
#define VISA_F034_T4A_NB_FIELDS					7
#define B1_F034T4A_TRST_MER_EXEM_IND 			"84"
#define B1_F034T4A_LOW_VAL_EXEM_IND 			"87"
#define B1_F034T4A_SCP_IND 						"88"
#define B1_F034T4A_TRA_EXEM_IND 				"89"
#define B1_F034T4A_DELEGATED_AUTH_IND 			"8A"
#define B1_F034T4A_REASONS_NOT_HONORING_EXEM 	"8C"
#define B1_F034T4A_TRA_SCORE 					"C0"

/*F034_T56*/
#define VISA_F034_T56_NB_FIELDS					12/*10*//*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
#define B1_F034_T56_CONSUMER_DEVICE_IP 			"9F1F" /*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
#define B1_F034_T56_IP_VELOCITY_COUNT 			"9F20"
#define B1_F034_T56_DEVICE_ID_VELOCITY 			"9F21"
#define B1_F034_T56_VISA_RISK_AUTH_SCORE 		"9F22"
#define B1_F034_T56_CAVV_VERSION_NBR 			"9F28"
#define B1_F034_T56_CAVV_TYPE 					"9F29"
#define B1_F034_T56_VCAS_SCORE 					"DF1F" /*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
#define B1_F034_T56_CYBS_DM_RESULT 				"DF20"
#define B1_F034_T56_SECURE_REMOTE_COMMERCE 		"DF21" /*NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
#define B1_F034_T56_SESSION_ID 					"81"

/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/
#define	  B1_F34T02_PAYFAC_STREET_ADDR	        "D1"
#define	  B1_F34T02_PAYFAC_CITY_NAME	        "D2"
#define	  B1_F34T02_PAYFAC_STATE_CODE	        "D3"
#define	  B1_F34T02_PAYFAC_POSTAL_CODE	        "D4"
#define	  B1_F34T02_PAYFAC_CTRY_CODE	        "D5"
/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2 20240702*/

/*Start PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/	
#define   VISA_F34_T03_NB_FIELDS				             1			
#define   B1_F34T03_ACC_NAME_REQUEST		 		        "C0"

#define VISA_F34_T04_NB_FIELDS				               	5			

#define   B1_F34T04_ACC_NAME_REQ_RES		 				"C0"
#define	  B1_F34T04_FULL_NAME_MATCHING_RES		 			"C4"
#define	  B1_F34T04_LAST_NAME_MATCHING_RES		 			"C8"
#define	  B1_F34T04_MIDDLE_NAME_MATCHING_RES		 		"C9"
#define	  B1_F34T04_FIRST_NAME_MATCHING_RES		 	     	"CA"

/*Start NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
#define   VISA_F34_T06_NB_FIELDS				             2	

#define   B1_F34T06_PLATFORM_DEVICE_ID                      "86"     /*Device ID (Platform)*/
#define   B1_F34T06_PLATFORM_DEVICE_TYPE                    "87"     /*Device Type (Platform)*/

#define   VISA_F34_T07_NB_FIELDS				             1	

#define   B1_F34T07_SDK_APP_ID                              "8D"        /*SDK App ID*/
/*End NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/

#define   B1_ACC_NAME_REQUEST_INFO							'Y'        /* Y (Yes) to notify the issuer  that acquirer is requesting for account name information  */
#define	  B1_ACC_NAME_REQUEST_VERIF							'I'        /* I (Issuer) to notify the issuer that acquirer is requesting for account name verification */
	  
#define	  B1_NAME_VALIDATION_MATCH	                    	"01"
#define	  B1_NAME_VALIDATION_PARTIAL_MATCH	            	"50"
#define	  B1_NAME_VALIDATION_NO_MATCH	                	"99"

/*End PLUTONL-6505 -Enh 2023Q4 NAB AN 1.1*/	

#endif

