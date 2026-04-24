#ifndef DCISC_DEFINE__H
#define DCISC_DEFINE__H



#define DC_F002_MAX_LEN                  19
/* Processing code */
#define DC_F003_P1_PURCHASE				"00"	/* Goods and services									*/
#define DC_F003_P1_CASH					"01"	/* Withdrawal/Cash advance								*/
#define DC_F003_P1_CR_ACC_VERIF			"18"	/* Card Account Verficiation Request					*/
#define DC_F003_P1_REFUND		        "20"	/* Refund (purchase return)					            */ /*  HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 3.4.1*/
#define DC_F003_P1_ADJUSTEMENT			"22"	/* Adjustement											*/
#define DC_F003_P1_BALANCE_INQ			"31"	/* Balance inquiry										*/
#define DC_F003_PIN_CHNAGE              "98"    /* PIN change											*/
#define DC_F003_PIN_UNBLOCK             "99"    /* PIN unblock											*/
#define DC_F003_P1_LEN					  2


#define DC_F003_P2_UNSPECIFIED			"00"	/* Default or Unspecified								*/
#define DC_F003_P2_CR_CARD_ACC			"30"	/* Credit facility										*/
#define DC_F003_P2_LEN				      2

#define DC_F003_P3_UNSPECIFIED			"00"	/* Default or Unspecified								*/
#define DC_F003_P3_LEN					  2

#define DC_F003_LEN						  6
#define DC_F004_LEN					     12
#define DC_F006_LEN					     12
#define DC_F007_LEN					     10
#define DC_F011_LEN					      6
#define DC_F012_LEN                      12
#define DC_F013_LEN                       4
#define DC_F014_LEN                       4

/* Point of service data code */
/*Position 1: Card Data Input Capability*/
#define DC_F022_P1_UNSPECIFIED			 '0'			/*		Unknown								*/
#define DC_F022_P1_MANUAL				 '1'			/*		Manual, no terminal							*/
#define DC_F022_P1_MAG_READ				 '2'			/*		Magnetic stripe read						*/
#define DC_F022_P1_BAR_CODE				 '3'			/*		Bar code									*/
#define DC_F022_P1_OCR					 '4'			/*		OCR											*/
#define DC_F022_P1_ICC_MAG				 '5'			/*		ICC					*/
#define DC_F022_P1_PAN_ENTRY			 '6'			/*		PAN key entry								*/
#define DC_F022_P1_RES_ISO_USE		     '7'			/*		Reserved for ISO use				*/
#define DC_F022_P1_CONTACTLESS			 '8'			/*		Contactless	*/
#define DC_F022_P1_HYBRID				 '9'			/*		Hybrid terminal					*/

/*	Position 2: Cardholder Authentication Capability		*/

#define DC_F022_P2_NO_AUTH				 '0'			/*		No electroinc authentication							*/
#define DC_F022_P2_PIN					 '1'			/*		PIN											*/
#define DC_F022_P2_SIG_ANALYSIS			 '2'			/*		Electronic signature analysis						*/
#define DC_F022_P2_BIOMETRICS			 '3'			/*		Biometrics authentication							*/
#define DC_F022_P2_BIOGRAPHIC		     '4'			/*		BIOGRAPHIC											*/
#define DC_F022_P2_ELECTRO_AUTH			 '5'			/*		Electronic authentication inoperative						*/
#define DC_F022_P2_Other				 '6'			/*		Other 						*/
#define DC_F022_P2_RESERV_ISO_USE		 '7'			/*   	Reserved for ISO use											*/

/*	Position 3: Card Capture Capability					*/

#define DC_F022_P3_NONE			  		 '0'			/*		None											*/
#define DC_F022_P3_Capture				 '1'			/*		Capture											*/

/*	Position 4: Operating Environment						*/
#define DC_F022_P4_NO_TERM					'0'		/*		No terminal used							*/
#define DC_F022_P4_ONPREM_CA_ATT			'1'		/*		On premises of card acceptor, Attended		*/
#define DC_F022_P4_ONPREM_CA_UNATT			'2'		/*		On premises of card acceptor, Unattended	*/
#define DC_F022_P4_OFFPREM_CA_ATT			'3'		/*		Off premises of card acceptor, Attended		*/
#define DC_F022_P4_OFPREM_CA_UNATT			'4'		/*		Off premises of card acceptor, Unattended	*/
#define DC_F022_P4_ONPREM_CH_UNATT			'5'		/*		On premises of cardholder, Unattended		*/
#define DC_F022_P4_UNKWNON					'9'		/*		Operating environment unknown										*/

/*		Position 5: Cardholder Presence	*/
#define DC_F022_P5_CH_PRES					'0'		/*		Cardholder present							*/
#define DC_F022_P5_CH_NPRES					'1'		/*		Cardholder not present						*/
#define DC_F022_P5_CH_NPRES_MO				'2'		/*		Cardholder not present, mail order			*/
#define DC_F022_P5_CH_NPRES_TO				'3'		/*		Cardholder not present, telephone			*/
#define DC_F022_P5_CH_NPRES_STANDIN		    '4'		/*		Cardholder not present, standing authorization			*/
#define DC_F022_P5_CH_NPRES_IN			    '9'		/*		Cardholder not present, Internet							*/

/*		Position 6: Card Presence 		*/
#define DC_F022_P6_CARD_NPRES				'0'		/*		Card not present							*/
#define DC_F022_P6_CARD_PRES				'1'		/*		Card present								*/
#define DC_F022_P6_CARD_PRES_UNNK			'8'		/*		Unknown whether card present								*/

/*		Position 7: Card Data Input Mode			*/
#define DC_F022_DC_UNSPECIFIED				'0'		/*		Unspecified										*/
#define DC_F022_DC_MANUAL					'1'		/*		Manual, no terminal								*/
#define DC_F022_DC_MAG						'2'		/*		Magnetic stripe read 		*/
#define DC_F022_DC_BARCODE					'3'		/*		Bar code											*/
#define DC_F022_DC_OCR						'4'		/*		Optical Optical Character Recognition (OCR)												*/
#define DC_F022_DC_ICC						'5'		/*		Integrated Circuit Card (ICC)								*/
#define DC_F022_DC_KEY_ENTRY				'6'		/*		Key entered									*/
#define DC_F022_DC_FALLBACK					'9'		/*		PAN captured from magnetic stripe of an ICC card “Chip Fallback”										*/
#define DC_F022_DC_RFID_CHIP				'S'		/*		    RFID—Chip Card Data	*/
#define DC_F022_DC_RFID_MAG					'T'		/*		    E-commerce, SET with cardholder certificate		*/
#define DC_F022_DC_CONTACTLESS				'U'		/*		    Contactless Interface Change to ICC contact */
#define DC_F022_DC_M_COMMERCE    			'V'		/*		    PAN auto-entry via mobile application  */ 	/* START HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 3.4.2*/

/*		Position 8: Cardholder Authentication Method		*/
#define DC_F022_P8_NOT_AUTH					'0'		/*		Not authenticated								*/
#define DC_F022_P8_PIN						'1'		/*		PIN												*/
#define DC_F022_P8_ELEC_SIG_ANALYSIS		'2'		/*		Electronic signature analysis				*/
#define DC_F022_P8_BIOMETRICS				'3'		/*		Biometrics										*/
#define DC_F022_P8_BIOGRAPHIC				'4'		/*		Biographic										*/
#define DC_F022_P8_MAN_SIG_VERIF			'5'		/*		Manual signature verification					*/
#define DC_F022_P8_OTHER_MAN_VERIF			'6'		/*		Other manual verification (e.g. driver’s license)	*/
#define DC_F022_P8_NO_SECURITY				'9'		/*		No security concerns	*/
#define DC_F022_P8_SUSPEC_FRAUD				'S'		/*		Suspected fraud	*/

/*		Position 9: Cardholder Authentication Entity		*/
#define DC_F022_P9_NOT_AUTH					'0'		/*		Not authenticated								*/
#define DC_F022_P9_ICC						'1'		/*		ICC												*/
#define DC_F022_P9_CA_DEVICE				'2'		/*		Card Acceptor Device (CAD)				*/
#define DC_F022_P9_AUTH_AGENT				'3'		/*		Authorizing agent (identified in authorizing agent institution identification code)		*/
#define DC_F022_P9_MERCHANT					'4'		/*		By merchant										*/
#define DC_F022_P9_OTHER					'5'		/*		Other											*/

/*		Position 10: Card Data Output Capability						*/
#define DC_F022_P10_UNKNOWN					'0'		/*		Unknown										*/
#define DC_F022_P10_NONE					'1'		/*		None											*/
#define DC_F022_P10_MAG						'2'		/*		Magnetic stripe write  */
#define DC_F022_P10_ICC						'3'		/*		Integrated Circuit Card (ICC)						*/
#define DC_F022_P10_CONTACTLESS				'8'		/*		Contactless						*/
#define DC_F022_P10_HYBRID					'9'		/*	Hybrid						*/

/*		Position 11: Terminal Output Capability				*/
#define DC_F022_P11_UNKNOWN					'0'		/*		UNKNOWN											*/
#define DC_F022_P11_NONE					'1'		/*		None											*/
#define DC_F022_P11_PRINTING_CAP			'2'		/*		Printing capability								*/
#define DC_F022_P11_DISPLAY_CAP				'3'		/*		Display capability								*/
#define DC_F022_P11_PRINTING_DISPLAY		'4'		/*		Printing and display											*/

/*		Position 12: PIN Capture Capability					*/
#define DC_F022_P12_NOPIN_CAPTURE			'0'		/*		No PIN capture capability						*/
#define DC_F022_P12_PIN_CAPT_UNNKOWN		'1'		/*		Device PIN capture capability unknown			*/
#define DC_F022_P12_4_DIG					'4'		/*		Four characters									*/
#define DC_F022_P12_5_DIG					'5'		/*		Five characters									*/
#define DC_F022_P12_6_DIG					'6'		/*		Six characters									*/
#define DC_F022_P12_7_DIG					'7'		/*		Seven characters								*/
#define DC_F022_P12_8_DIG					'8'		/*		Eight characters								*/
#define DC_F022_P12_9_DIG					'9'		/*		Nine characters									*/
#define DC_F022_P12_10_DIG					'A'		/*		Ten characters									*/
#define DC_F022_P12_11_DIG					'B'		/*		Eleven characters								*/
#define DC_F022_P12_12_DIG					'C'		/*		Twelve characters								*/
#define DC_F022_LEN                      	12

#define DC_F023_LEN                      	12

#define DC_F024_ORIG_REQUEST			  "100"			/* Original authorization						*/
#define DC_F024_INQ						  "108"			/* Inquiry					*/
#define DC_F024_ADD_RECORD				  "301"			/* Add record							*/
#define DC_F024_CHG_RECORD				  "302"			/* Change record 	*/
#define DC_F024_DEL_RECORD				  "303"			/* Delete record						*/
#define DC_F024_REP_RECORD			      "304"			/* Replace record			*/
#define DC_F024_INQ_RECORD				  "305"			/* Inquiry on record								*/

#define DC_F024_FULL_REVERSAL_TRX_NOT_COMPLETE_APP			  "400"			/* Full reversal, transaction did not complete as approved						*/
#define DC_F024_PARTIAL_REVERSAL		  "401"			/* Partial reversal, transaction did not complete for full amount						*/
#define DC_F024_FULL_REVERSAL_TRX_NOT_COMPLETE_ACQ			  "440"			/* Full reversal, transaction did not complete at Acquirer						*/

#define DC_F024_GENERAL_COMM			  "640"			/* Genaral communication						*/

#define DC_F024_SIGNON				      "801"			/* Sign-on request						*/
#define DC_F024_SIGNOFF					  "802"			/* Sign-off request						*/
#define DC_F024_KEY_CHG					  "811"			/* Key change							*/
#define DC_F024_KEY_TEST				  "814"			/* Key test							*/
#define DC_F024_ECHO_TEST				  "831"			/* System to system echo test */

/* CFE_18092018 */
#define DC_F025_REV_OF_REQ				  "4005"       /* Reversal of Request (issued by) */
#define DC_F025_FINAL_AMN_ADV             "0073"       /* Final Amount Advice */ /* ELKAM20230217 PLUTONL-5914 */

#define DC_F44_CVV1_OK    				  "001"
#define DC_F44_CVV1_NOK 				  "002" 
#define DC_F44_CVV1_ABS 				  "003"
#define DC_F44_CVV2_OK 				      "006"
#define DC_F44_CVV2_NOK					  "007"		
#define DC_F44_CVV1_OK_CVV2_OK			  "010"
#define DC_F44_CVV1_NOK_CVV2_NOK 		  "011"
#define DC_F44_CVV1_OK_CVV2_NOK 		  "012"
#define DC_F44_CVV1_NOK_CVV2_OK     	  "013"
#define DC_F44_CVV1_ABS_CVV2_OK   		  "014"
#define DC_F44_CVV1_ABS_CVV2_NOK  		  "015"
#define DC_F44_DCVV_OK 				      "016"
#define DC_F44_DCVV_NOK 				  "017"
#define DC_F44_DCVV_UNBABLE				  "018"
#define DC_F44_CVV_NOT_CHECKED 			  "   "
#define DC_F44_CAVV_AUTHEN_OK    		  "01"  		
#define DC_F44_CAVV_AUTHEN_NOK     		  "02" 
#define DC_F44_CAVV_ATTEMPT_OK    	  	  "03"  		
#define DC_F44_CAVV_ATTEMPT_NOK     	  "04" 
#define DC_F44_CAVV_NOK_ISS_NOT_PART  	  "05" 
#define DC_F44_CAVV_UNABLE_AUTH			  "06" 
#define DC_F44_ARQC_OK			  		  "01" 
#define DC_F44_ARQC_NOK			          "02" 
#define DC_F44_UNABLE			          "03" 




#define DC_F024_LEN                	      	 3
#define DC_F025_LEN						  	 4
#define DC_F026_LEN						  	 4
#define DC_F030_LEN						  	24
#define DC_F032_MAX_LEN                   	11
#define DC_F033_MAX_LEN                   	11
#define DC_F035_MAX_LEN                   	37
#define DC_F037_LEN                       	12
#define DC_F038_LEN                       	 6



#define DC_F039_PIN_CHANGE_SUCC			  					"000"		/* Pin change successful (EMV only) */
#define DC_F039_HONOR_WITH_ID			  					"001"	/* Honor with identification												*/
#define DC_F039_APP_BY_ISSUER			  					"081"	/* Approved by Issuer							*/
#define DC_F039_APP_BY_XPRESS			  					"082" /* Approved by Xpress (stand–in) */
#define DC_F039_APP_BY_POS				  					"083" /* Approved by POS */
#define DC_F039_OFF_APP_				  					"084" /* Off-line approved (EMV only) */
#define DC_F039_OFF_APP_UNABLE_GO_ONL	  					"085" /* Off-line approved—unable to go online (EMV only) */
#define DC_F039_CARD_VERIF_SUCCESS		  					"086" /* Card verification successful */
#define DC_F039_NO_RSN_TO_DEC			  					"087"		/* No reason to declined					*/
#define DC_F039_DECLINE					  					"100"		/* Do not honor									*/
#define DC_F039_EXPIRED_CARD			  					"101"		/* Expired card								*/
#define DC_F039_SUSPECTED_FRAUD			  					"102"		/* Suspected fraud(account not on Positive File)						*/
#define DC_F039_RESTRICTED_CARD			  					"104" 		/* Restricted card								*/
#define DC_F039_PIN_TRIES_EXC			  					"106"		/* Allowable PIN tries exceeded					*/
#define DC_F039_REF_TO_ISS				  					"107"		/* Refer to card issuer						*/
#define DC_F039_INV_MERCHANT			  					"109"		/* Invalid Merchant							*/
#define DC_F039_INV_AMNT				  					"110"		/* Invalid amount							*/
#define DC_F039_INV_CARD_NBR			  					"111"		/* Invalid card number						*/
#define DC_F039_FCT_NOT_SUPPORTED		  					"115"		/* Requested function not supported					*/
#define DC_F039_PIN_INCORRECT			  					"117"		/* Incorrect PIN							*/
#define DC_F039_CYCLE_CARD_SUSPENDED	  					"118"		/* Cycle range suspended								*/
#define DC_F039_TRN_NOPERM_CH			  					"119"		/* Transaction not permitted to Cardholder				*/
#define DC_F039_CARD_VAL_EXCEEDED		  					"122"		/* Card validity period exceeded						*/
#define DC_F039_VIOLATION_LAW			  					"124"		/* Violation of law			*/
#define DC_F039_CARD_NOT_EFFECTIVE		  					"125"		/* Card not effective						*/
#define DC_F039_SUSPEC_CONTREFEIT_CARD	  					"129"		/* Suspected counterfeit Card							*/
#define DC_F039_SEC_VIOLATION			  					"163"		/* Security violation						*/
#define DC_F039_CARDMEMBER_CONTACT_ISS	  					"180"		/* Cardmember to contact Issuer						*/
#define DC_F039_DEC_BY_POS				  					"181"		/* Decline given by POS Participant (adjustment)						*/
#define DC_F039_DEC_BY_ISS				  					"182"		/* Decline given by Issuer						*/
#define DC_F039_DEC_BY_XPRESS_NO_COMM_WITH_ISS				"184"		/* Decline given by Xpress, no communication with Issuer								*/
#define DC_F039_DEC_BY_XPRESS_CARD_LOCAL_USE_ONLY			"185"		/* decline given by Xpress, card is local use only								*/
#define DC_F039_REFER_TO_ISS_BY_ISS							"186"		/* Refer to Issuer given by Issuer								*/
#define DC_F039_REFER_TO_ISS_BY_XPRESS_STANDIN_LMT_EXCESS	"187"		/* Refer to Issuer given by Xpress, exceeded stand–in limit								*/
#define DC_F039_UNABLE_FWD_REQ								"188"		/* Xpress unable to forward request to Issuer X								*/
#define DC_F039_REFER_TO_ISS_BY_XPRESS_USE_ANOTHER_MTD		"189"		/* Refer to Issuer given by Xpress, use ANOTHER METHOD to contact Issuer								*/
#define DC_F039_DEC_BY_EXPRESS_REFERAL_STANDIN				"190"		/* Decline given by Xpress (referral stand–in)			*/
#define DC_F039_REFER_TO_ISS_BY_XPRESS_ACC_NOT_ON_POSITIVE_FILE	"191"		/* Refer to Issuer given by Xpress (account not on Positive File) */
#define DC_F039_RESTRICTED_MERCHANT							"192"		/* Restricted merchant */
#define DC_F039_PIN_CHANGE_UNBLOCK_FAILED					"194"		/* PIN change or unblock failed (EMV only) */
#define DC_F039_NEW_PIN_NOT_ACCEPTED						"195"		/* New PIN not accepted (EMV only) */
#define DC_F039_CHIP_INFO_ADIVCE							"196"		/* Chip information advice (EMV only) */
#define DC_F039_CARD_VERIF_FAILED							"197"		/* Card verification failed */
#define DC_F039_TVR_CVR_VALIDATION_FAILED					"198"		/* TVR or CVR validation failed */
#define DC_F039_DECLINE_PICKUP								"200"		/* Do not honor, Pick up card					*/
#define DC_F039_EXPIRED_PICKUP								"201"		/* Expired card, Pick up					*/
#define DC_F039_SUSPEC_FRAUD_PICKUP							"202"		/* Suspected fraud, Pick up						*/
#define DC_F039_CARD_ACCEPTOR_CONTACT_ACQ					"202"		/* Card acceptor contact Acquirer, Pick up						*/
#define DC_F039_RESTRICTED_CARD_PICKUP						"204"		/* Restricted Card, Pick up					*/
#define DC_F039_CALL_ACQ_PICKUP								"205"		/* Card acceptor call Acquirer’s security department, Pick up			*/
#define DC_F039_PIN_TRIES_EXC_PICKUP						"206"		/* Allowable PIN tries exceeded, Pick up					*/
#define DC_F039_SPEC_COND_PICKUP							"207"		/* Special conditions, Pick up				*/
#define DC_F039_LOST_CARD_PICKUP							"208"		/* Lost card, Pick up						*/
#define DC_F039_STOLEN_CARD_PICKUP							"209"		/* Stolen card, Pick up						*/
#define DC_F039_SUSP_CONTREFEIT_CARD_PICKUP					"210"		/* Suspected counterfeit Card, Pick up					*/
#define DC_F039_TEMP_LOST_CARD_PICKUP						"280"		/* Temporary status lost card, Pick up				*/
#define DC_F039_TEMP_STOLEN_CARD_PICKUP						"280"		/* Temporary status stolen card, Pick up				*/
#define DC_F039_PROC_SUCCESS								"300"		/* Successful				*/
#define DC_F039_NOT_SUPP_REC								"301"		/* Not supported by the receiver			*/
#define DC_F039_UNABLE_LOCATE_REC							"302"		/* Unable to locate record on file			*/
#define DC_F039_DUP_RECORD_REPLACE							"303"		/* Duplicate record, old record replaced						*/
#define DC_F039_DE_EDIT_ERROE								"304"		/* Data element edit error						*/
#define DC_F039_FILE_LOCKED									"305"		/* File locked out								*/
#define DC_F039_UNSUCCESS									"306"		/* Not successful								*/
#define DC_F039_FMT_ERROR									"307"		/* Format error								*/
#define DC_F039_DUP_REC_REJECT								"308"		/* Duplicate, new record rejected						*/
#define DC_F039_UNKNOWN_FILE								"309"		/* Unknown file								*/
#define DC_F039_UNSUCESS_CROSS_REF_EXISTS					"380"		/* Not successful, cross–reference exists							*/
#define DC_F039_CONTINU_DATA_NEXT_REC						"382"		/* Continuation data in next record (notifications)							*/
#define DC_F039_CARD_ON_POSITIVE_FILE						"397"		/* card on Positive File							*/
#define DC_F039_VIP_CARD_WITH_LIMITS						"399"		/* VIP card with limits							*/
#define DC_F039_REV_ACCEPTED								"400"		/* Reversal accepted						*/
#define DC_F039_FCT_CODE_NOT_ALLOWED						"480"		/* Invalid reversal							*/
#define DC_F039_ACC_NBR_INV									"481"		/* Account number invalid			*/
#define DC_F039_ACC_NBR_NOT_MODE_10							"482"		/* Account number does not mod10			*/
#define DC_F039_NO_MATCHING_CYCLE_RANGE						"483"		/* No matching cycle range exists for this account			*/
#define DC_F039_REV_INVALID_AMNT							"484"		/* No matching cycle range exists for this account			*/
#define DC_F039_ACCEPTED									"600"		/* Accepted				*/
#define DC_F039_FWD_DEST									"601"		/* 	Forward to destination Participant */
#define DC_F039_NW_REQ_ACCEPTED								"800"		/* Network request accepted					*/
#define DC_F039_UNKOWKN_DEST								"909"		/* Destination not known								*/
#define DC_F039_SYS_MALFUNCTION								"909"		/* System malfunction								*/
#define DC_F039_DEST_NOT_IN_SERVICE							"910"		/* Destination not in service									*/

#define DC_F039_LEN											   3
#define DC_F040_LEN											   3
#define DC_F041_LEN											   8
#define DC_F042_LEN											   15
#define DC_F043_MAX_LEN										   99

/* Additional Response Data */
/*		Position 3-5: CVV Indicator		*/
#define DC_F044_P3_CVV1_CORRECT								"001"    /* CVV1 on stripe correct */
#define DC_F044_P3_CVV1_INCORRECT		    				"002"  /* CVV1 on stripe not correct */
#define DC_F044_P3_CVV1_MISSING		        				"003" /* CVV1 missing */
#define DC_F044_P3_CVV2_CORRECT								"006" /* CVV2 correct */
#define DC_F044_P3_CVV2_INCORRECT		    				"007"  /* CVV2 not correct */
#define DC_F044_P3_CVV1_CVV2_CORRECT						"010" /* CVV1 and CVV2 correct*/
#define DC_F044_P3_CVV1_CVV2_INCORRECT		    			"011" /* CVV1 and CVV2 incorrect*/
#define DC_F044_P3_CVV1_CORRECT_CVV2_INCORRECT		   	    "012" /* CVV1 correct and CVV2 incorrect */
#define DC_F044_P3_CVV1_INCORRECT_CVV2_CORRECT		        "013" /* CVV1 incorrect and CVV2 correct */
#define DC_F044_P3_CVV1_MISSING_CVV2_CORRECT		    	"014" /* CVV1 missing and CVV2 correct */
#define DC_F044_P3_CVV1_MISSING_CVV2_INCORRECT		   		"015" /* CVV1 missing and CVV2 incorrect */
#define DC_F044_P3_DCVV_VALID	    						"016" /* DCVV Valid */
#define DC_F044_P3_DCVV_INVALID	   						    "017" /* DCVV Invalid */
#define DC_F044_P3_DCVV_UNABLE_VALIDATE	    				"018" /* DCVV Unable to Validate */

/*		Position 6-7: CAVV Result Code		*/
#define DC_F044_P6_CAVV_PASSED_VERIF_AUTH					 "01"    /* CAVV passed verification- authentication */
#define DC_F044_P6_CAVV_FAILED_VERIF_AUTH					 "02"     /* CAVV failed verification -authentication */
#define DC_F044_P6_CAVV_PASSED_VALIDATION_ATTEMPT			 "03"  /* CAVV passed validation—attempt */
#define DC_F044_P6_CAVV_FAILED_VALIDATION_ATTEMPT			 "04"  /* CAVV failed validation—attempt */
#define DC_F044_P6_CAVV_NOT_VALIDATED						 "05"   /* CAVV not validated, issuer not participating in CAVV validation */
#define DC_F044_P6_CAVV_UNABLE_PERFORM_AUTH				     "06"	/* CAVV Unable to perform authentication */

/*		Position 19-20: Cryptogram Validation Result Code		*/
#define DC_F044_P19_CRYPTOGRAM_VALID					     "01"    /* Pass, Cryptogram Valid */
#define DC_F044_P19_CRYPTOGRAM_INVALID						 "02"    /* Fail, Cryptogram invalid */
#define DC_F044_P19_CRYPTOGRAM_UNABLE_VALIDATE			   	 "03"    /* Unable to validate */

/*		Position 21: TVR Check Result		*/
#define DC_F044_P21_TVR_NO_TESTED							   '0'    /* Not Tested, TVR */
#define DC_F044_P21_TVR_PASS									   '1'   /* Pass, TVR*/
#define DC_F044_P21_TVR_FAIL									   '2'   /* Fail, TVR */

/*		Position 22: CVR Check Result		*/
#define DC_F044_P22_CVR_NO_TESTED	 					       '0'  /* Not Tested, CVR */
#define DC_F044_P22_CVR_PASS			 					       '1'             /* Pass, CVR*/
#define DC_F044_P22_CVR_FAIL			 					       '2'  			 /* Fail, CVR */ 
/* ELKAM20230217 PLUTONL-5914 START*/
#define DC_F054_AT_ORIG_AMNT                                "56" 

#define DC_F062_NB_FIELDS                                   4

#define DC_F062_DF01_POS_TERM_TYPE                          "DF01"
#define DC_F062_DF03_CARD_PROD_TYPE                         "DF03"
#define DC_F062_DF04_TOK_PROV_VER_REQ                       "DF04"
#define DC_F062_DF05_CUS_MER_INIT_TRANS                     "DF05"

#define DC_F062_DF05_RE_AUTH_FULL_AMNT                      "A"
#define DC_F062_DF05_DELAYED_CARD_SALE                      "D"
#define DC_F062_DF05_RESUB_OF_CARD_SALE                     "E"
#define DC_F062_DF05_TRANSIT_AGGRE_TRANS                    "G"
#define DC_F062_DF05_INC_AUTH                               "I"
#define DC_F062_DF05_NO_SHOW_CHARGES                        "N"
#define DC_F062_DF05_PARTIAL_SPLIT_SHIPMENT                 "P"
#define DC_F062_DF05_RECURRING_PAYMENT                      "R"
#define DC_F062_DF05_INSTALLMENT_PAYMENT                    "S"
#define DC_F062_DF05_UNSCHEDULED_PAYMENT                    "U"
#define DC_F062_DF05_FINAL_AMOUNT_ADVICE                    "0"
#define DC_F062_DF05_PRE_AUT_REQ                            "4"
/* ELKAM20230217 PLUTONL-5914 END*/
                               
#define DC_F044_MAX_LEN				 					       22
#define DC_F045_MAX_LEN				 					       76
#define DC_F046_LEN					 					       11
#define DC_F048_MAX_LEN				 					      999
#define DC_F049_LEN					 					     	3
#define DC_F051_LEN					 					     	3
#define DC_F052_LEN					 					       16  --TOREV
#define DC_F054_MAX_LEN				 					      120  
#define DC_F055_MAX_LEN				 					      255
#define DC_F056_LEN					 					       33
#define DC_F058_LEN					 					       11
#define DC_F059_MAX_LEN				 					      999
#define DC_F062_MAX_LEN				 					      999
#define DC_F064_LEN					 					       16 --TOREV
#define DC_F072_MAX_LEN				 					      999
#define DC_F092_LEN					 					        3
#define DC_F093_MAX_LEN				 					       11
#define DC_F094_MAX_LEN				 					       11
#define DC_F096_MAX_LEN				 					       36 --TOREV
#define DC_F100_MAX_LEN				 					       11
#define DC_F101_MAX_LEN				 					       17
#define DC_F122_MAX_LEN				 					      999
#define DC_F123_MAX_LEN				 					       15
#define DC_F124_MAX_LEN				 					      999
#define DC_F125_MAX_LEN				 					       16  --TOREV

#define DCISC_REQ_TYPE_KEY_TEST           				"KEYTEST"  /* CFE_18092018 */

#endif

