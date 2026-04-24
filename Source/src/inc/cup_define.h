#ifndef CUP_DEFINE__H
#define CUP_DEFINE__H

#define	CUP_MSG_TYPE_LEN				 4

/* F003: Processing codes */

#define CUP_F003_P1_PURCHASE   				"00"		/*Goods and service		 */
#define CUP_F003_P1_WITHDRAWAL   			"01"		/*Cash && Cash Advance */
#define CUP_F003_P1_ADJ_DEBIT   			"02"		/*Adjustment (Debit adjustment) */
#define CUP_F003_P1_CHECK_GUARAN  			"03"		/*Cheque guarantee(funds guaranteed) */
#define CUP_F003_P1_CHECK_VERIF   			"04"		/*Cheque verification(funds available but not guaranteed) */
#define CUP_F003_P1_EURO_CHECK   			"05"		/*Euro-cheque */
#define CUP_F003_P1_TRAVEL_CHECK  			"06"		/*Traveler cheque */
#define CUP_F003_P1_LETTER_CREDIT  			"07"		/*Letter of credit */
#define CUP_F003_P1_GIRO   				"08"		/*Giro (postal banking) */
#define CUP_F003_P1_CASHBACK  				"09"		/*Goods and service with cash withdrawal transfer */
#define CUP_F003_P1_RETURNS 				"20"		/*Returns */
#define CUP_F003_P1_CANCEL                              "20"            /*Cancellation */
#define CUP_F003_P1_DEPOSIT   				"21"		/*Deposits */
#define CUP_F003_P1_ADJ_CREDIT   			"22"		/*Adjustment(Credit adjustment) */                 
#define CUP_F003_P1_CHECK_DEPO_GUAR			"23"		/*Cheque deposit guarantee */
#define CUP_F003_P1_CHECK_DEPOSIT   			"24"		/*Cheque deposit */
#define CUP_F003_P1_AVAIL_FUNDS_INQ   			"30"		/*Available funds inquiry */
#define CUP_F003_P1_BAL_INQ   				"31"		/*Balance inquiry */
#define CUP_F003_P1_ACC_VERIF   			"33"		/*Account Verification*/   /*NAB011223*/
#define CUP_F003_P1_ACC_TRANSFER   			"40"		/*Card holder accounts transfer */
#define CUP_F003_P1_PIN_CHANGE   			"70"		/*Pin change */
#define CUP_F003_P1_ESTAB_COM_REL   			"89"		/*Establish commission relationship */
#define CUP_F003_P1_REVOKE_COM_REL   			"92"		/*Revoke commission relationship */
#define CUP_F003_P1_LEN					2
        
#define CUP_F003_P2_DEFAULT				"00"		/* Default	*/
#define CUP_F003_P2_SAVING_ACC				"10"		/* Savings Account */
#define CUP_F003_P2_CHECKING_ACC			"20"		/* Checking Account	*/
#define CUP_F003_P2_CR_FACILITY				"30"		/* Credit facility	*/
#define CUP_F003_P2_UNIVERSAL_ACC			"40"		/* Universal Account	 */
#define CUP_F003_P2_INVESTMENT_ACC			"50"		/* Investment account 	 */
#define CUP_F003_P2_LEN					2

#define CUP_F003_P3_DEFAULT				"00"		/* Default		 */
#define CUP_F003_P3_LEN					2

#define	CUP_F007_LEN		           10  /*NAB09102023 PLUTONL-6818*/

/*Start PLUTONL-5687 AHA23012023*/
/*F018 Merchant Code */
#define CUP_CRYPT_TRAN    "6051"      /* CRYPTOCURRENCY TRANSACTIONS/
/*End PLUTONL-5687 AHA23012023*/

/* F022: Point of Service Entry Mode Code */

/*Position 1 and 2: PAN Entry Mode*/

#define CUP_F022_P1_UNSPECIFIED				"00"		/*Unspecified	 */
#define CUP_F022_P1_MANUAL				"01"		/*Manual */
#define CUP_F022_P1_MAG_READ				"02"		/*Magnetic strip read */
#define CUP_F022_P1_BAR_CODE				"03"		/*Bar code read */
#define CUP_F022_P1_OCR					"04"		/*OCR coding read */
#define CUP_F022_P1_ICC_READ				"05"		/*Integrated circuit card read, card data reliable */
#define CUP_F022_P1_QUICS_CL				"07"		/*qUICS Debit/Credit Integrated Circuit Card read (contactless) */
#define CUP_F022_P1_MAG_READ_REL			"90"		/*Magnetic strip data read and reliable, track 2 data must exist. */
#define CUP_F022_P1_MER_QRC_CHIP_I			"93"		/*Merchant-presented QRC, chip information included */ /*NAB09102023 PLUTONL-6818*/
#define CUP_F022_P1_MER_QRC_CHIP_E			"94"		/*Merchant-presented QRC (barcode also), chip information excluded *//*NAB09102023 PLUTONL-6818*/

#define CUP_F022_P1_ICC_UNREL				"95"		/*Integrated circuit card, card data may be unreliable. */
#define CUP_F022_P1_UICS_CL				"98"		/*Standard UICS Debit/Credit Integrated Circuit Card read (contactless) */
#define CUP_F022_P1_LEN					2	
	
/*Position 3: PIN Entry Mode*/	
	
#define CUP_F022_P2_UNSPECIFIED				'0'		/* Unknown */
#define CUP_F022_P2_PIN_INCLUDED 			'1'		/* PIN included in transaction */
#define CUP_F022_P2_PIN_EXCLUDED			'2'  		/* PIN excluded in transaction */
#define CUP_F022_P2_LEN					1	
	
	
/* F025: Point of Service Condition Code */	
	
#define CUP_F025_NORMAL   				"00"		/*Normal present */
#define CUP_F025_CH_NP    				"01"		/*Customer not present */
#define CUP_F025_UN_ATTEND_TERM   		 	"02"		/*Un-attended terminal */
#define CUP_F025_MER_SUSPICIOUS    		 	"03"		/*Suspicious merchant */
#define CUP_F025_CH_P_CRD_NP    		 	"05"		/*Customer  present,  card  not present */
#define CUP_F025_PRE_AUTH_REQ    		 	"06"		/*Pre-authorized request */
#define CUP_F025_MOTO_AUTH_PUR    		 	"08"		/*MOTO  authorization,  MOTO purchase */
#define CUP_F025_ID_VERIFIED    		 	"10"		/*Customer identity verified */
#define CUP_F025_SUSP_FRAUD    			 	"11"		/*Suspected Fraud  (Message type must be 0100 or0200) */
#define CUP_F025_SECU_REASON    		 	"12"		/*Security reason  (Message type must be 0100 or0200) */
#define CUP_F025_REPRESENTMENT    		 	"13"		/*Representment */
#define CUP_F025_CHARGEBACK    			 	"17"		/*Chargeback */
#define CUP_F025_MOTO_PRE_AUTH    		 	"18"		/*MOTO pre-authorization */
#define CUP_F025_RECURRING_AUTH    		 	"28"		/*Recurring authorization */
#define CUP_F025_SEC_CHARGEBACK   		 	"41"		/*Second Chargeback */
#define CUP_F025_PRE_AUT_E_COM    		 	"43"		/*Pre-authorization request  of e-commerce transaction */
#define CUP_F025_MANUAL_REFUND    		 	"61"		/*Manual refund */
#define CUP_F025_REFUND_CDRS    		 	"62"		/*Refund(Manual processing on CDRS) */
#define CUP_F025_INSTALL_PAY    		 	"64"		/*Installment payment, Installment payment transactions */
#define CUP_F025_PRIM_CREDIT    		 	"68"		/*Credit  Applicable to primary credit confirmation transaction */
#define CUP_F025_EXCEP_PROC    			 	"82"		/*Exceptional processing */
#define CUP_F025_CREDIT_ADJUST    		 	"83"		/*The  credit  adjustment   of  deposit advanced by issuer */
#define CUP_F025_LOAD_UNLOAD    		 	"91"		/*Load and unload of IC  card based  on UICS E-cash application Standard */
	
/* F026: Point of Service PIN Capture Code */	
	
#define CUP_F026_4_DIG					 "04"		/*Four characters	*/
#define CUP_F026_5_DIG					 "05"		/*Five characters	*/
#define CUP_F026_6_DIG					 "06"		/*Six characters	*/
#define CUP_F026_7_DIG					 "07"		/*Seven characters	*/
#define CUP_F026_8_DIG					 "08"		/*Eight characters	*/
#define CUP_F026_9_DIG					 "09"		/*Nine characters	*/
#define CUP_F026_10_DIG					 "10"		/*Ten characters	*/
#define CUP_F026_11_DIG					 "11"		/*Eleven characters	*/
#define CUP_F026_12_DIG					 "12"		/*Twelve characters	*/
		
	
	
	
	
/* F039: Response Code */	
	
#define CUP_F039_APPROVAL  				 "00"		/*Approved or completed successfully (Approve) */
#define CUP_F039_REF_TO_ISS   			 	 "01"		/*Refer to card Issuers (Decline) */
#define CUP_F039_INVALID_MER 			 	 "03"		/*Invalid merchant (Decline) */
#define CUP_F039_PICKUP  				 "04"		/*Pick-up (Pick-up) */
#define CUP_F039_CERTIF_FAILS  			 	 "05"		/*ID certification fails (Decline)         	 */
#define CUP_F039_APP_PART_AMNT  		 	 "10"		/*Approved for partial amount (Approve) */
#define CUP_F039_VIP_APP 				 "11"		/*VIP (Approve)        	 */
#define CUP_F039_INV_REL_TRANS  		 	 "12"		/*Invalid related transaction (Decline) */
#define CUP_F039_INVALID_AMNT 			 	 "13"		/*Invalid amount (Decline )       	 */
#define CUP_F039_INV_CRD_NBR  			 	 "14"		/*Invalid card number (no such account)  (Decline) */
#define CUP_F039_NO_SUCH_ISS  			 	 "15"		/*No such Issuers (Decline)        	 */
#define CUP_F039_APPROVAL_T3_UPD  		 	 "16"		/*Approved to update track 3 (Approve)         	 */
#define CUP_F039_CRD_NOT_INIT  			 	 "21"		/*Card not initialized (Decline )       	 */
#define CUP_F039_SUSP_MALFUNC  			 	 "22"		/*Suspected malfunction; related transaction error (Decline) */       
#define CUP_F039_UNABLE_LOC_ORIG_TRANS   		 "25"		/*Unable to locate original transaction (Decline) */
#define CUP_F039_FORMAT_ERROR  			 	 "30"		/*Format error (Decline)      	 */
#define CUP_F039_FRAUD_PICKUP  			 	 "34"		/*Fraud (Pick-up)         	 */
#define CUP_F039_PIN_TRY_LMT_EXC  		 	 "38"		/*PIN try limit exceeded (Decline) */
#define CUP_F039_FCT_NOT_SUP  			 	 "40"		/*Function requested not supported (Decline) */
#define CUP_F039_LOST_CARD_PICKUP  		 	 "41"		/*Lost card (Pick-up)         	 */
#define CUP_F039_STOLEN_CARD_PICKUP  	 	 	 "43"		/*Stolen card (Pick-up) */
#define CUP_F039_FALLBACK_NA  			 	 "45"		/*Fallback transaction is not allowed (Decline)	 */
#define CUP_F039_INSUF_BAL  			 	 "51"		/*Insufficient balance (Decline) */
#define CUP_F039_EXPIRED_CARD  			 	 "54"		/*Expired card (Decline) */
#define CUP_F039_PIN_ERROR  			 	 "55"		/*Incorrect personal identification number (Decline) */
#define CUP_F039_TRN_NOPERM_CH  		 	 "57"		/*Transaction not allowed to be processed by cardholder(Decline) */
#define CUP_F039_TNR_NOPERM_TERM  		 	 "58"		/*Transaction not allowed to be processed by terminal (Decline)    	 */
#define CUP_F039_SUSP_FRAUD  			 	 "59"		/*Suspected fraud (Decline)         	 */
#define CUP_F039_TRN_AMNT_LMT_EXC  	 		 "61"		/*Transaction amount limit exceeded (Decline) */
#define CUP_F039_RESTRECTED_CRD  		 	 "62"		/*Restricted card (Decline)         	 */
#define CUP_F039_ORIG_TRN_AMNT_ERR  	 	 	 "64"		/*Original transaction amount error (Decline) */
#define CUP_F039_WTDWL_LMT_EXC  		 	 "65"		/*Exceeds withdrawal velocity limit (Decline) */
#define CUP_F039_ISS_RESP_TIME_OUT  	 		 "68"		/*Issuer response time-out (Decline)         	 */
#define CUP_F039_PIN_TRIES_EXC  		 	 "75"		/*Allowable number of PIN tries exceeded (Decline) */
#define CUP_F039_CUTOFF_INPROG  		 	 "90"		/*Cutoff is in process (Decline )        	 */
#define CUP_F039_ISS_NOTCAP_PROC  		 	 "91"		/*Issuer not capable to process (Decline) */
#define CUP_F039_DEST_NOTFND_ROUT 		 	 "92"		/*institution  or intermediate ntw facility not found for routing (Decline)*/   
#define CUP_F039_DUPLICATED_TRN  		 	 "94"	     	/*Duplicated transaction (Decline) */
#define CUP_F039_SW_SYS_MALFUNC 		 	 "96"		/*Switch system malfunction (Decline)        	                       	 */
#define CUP_F039_TERM_NBR_NOT_LOC  		 	 "97"		/*ATM/POS terminal number cannot be located (Decline)                  	 */
#define CUP_F039_ISS_RESP_NOT_RECV  	 		 "98"		/*Issuer response not received by CUPS (Decline) */
#define CUP_F039_PIN_BLOCK_ERR  		 	 "99"		/*PIN Block Error (Decline)          	                              	 */
#define CUP_F039_MAC_FAILED  			 	 "A0"		/*MAC failed (Decline)   	                                 	 */
#define CUP_F039_SUCC_TRN_FAULT  		 	 "A2"		/*Successful transaction with fault (Approve) */
#define CUP_F039_ACC_NOT_FND  			 	 "A3"		/*Account not found in Transfer-in side  (Decline)                     	 */
#define CUP_F039_SUCC_TRN_FAULT_1 		 	 "A4"		/*Successful transaction with fault (Approve)                   	 */
#define CUP_F039_SUCC_TRN_FAULT_2 		 	 "A5"		/*Successful transaction with fault (Approve)	                       	 */
#define CUP_F039_SUCC_TRN_FAULT_3 		 	 "A6"		/*Successful transaction with fault (Approve)                         	 */
#define CUP_F039_SECU_PROC_FAIL  		 	 "A7"		/*Security processing failure                                          	 */
#define CUP_F039_NO_ARREARS  			 	 "B1"		/*No arrears (transaction receipt not printed)(Decline) */
#define CUP_F039_ILLEGAL_STATUS_ACQ  	 		 "C1"		/*Illegal Status of Acquirer (Decline)               	              	 */
#define CUP_F039_IIN_INCORRECT  		 	 "D1"		/*Incorrect IIN                                                    	 */
#define CUP_F039_DATE_ERROR 			 	 "D2"		/*Date Error                                                      	 */
#define CUP_F039_INV_FILE_TYPE  		 	 "D3"		/*Invalid file type                                                  	 */
#define CUP_F039_FILE_PROC  			 	 "D4"		/*File processed                                                    	 */
#define CUP_F039_NO_SUCH_FILE  			 	 "D5"		/*No such file                                                         	 */
#define CUP_F039_NOT_SUP_RECV  			 	 "D6"		/*Not supported by Receiver 	                                       	 */
#define CUP_F039_FILE_LOCK  			 	 "D7"		/*File locked                                                        	 */
#define CUP_F039_UNSUCCESS  			 	 "D8"		/*Unsuccessful                                                        	 */
#define CUP_F039_INC_FILE_LEN  			 	 "D9"		/*Incorrect file length                                                	 */
#define CUP_F039_FILE_DECOMP_ERR  		 	 "DA"		/*File decompression error  	 */
#define CUP_F039_FILE_NAME_ERR  		 	 "DB"		/*File name error              	 */
#define CUP_F039_FILE_CANT_BE_RECV  	 		 "DC"		/*File cannot be received            	 */
#define CUP_F039_FILE_REC_FORM_ERR  	 		 "F1"		/*File record format error */
#define CUP_F039_FILE_REC_REPEATED  	 		 "F2"		/*File record repeated */
#define CUP_F039_FILE_REC_NOTEXIST  	 		 "F3"		/*File record not existing */
#define CUP_F039_FILE_REC_ERR  			 	 "F4"		/*File record error */
#define CUP_F039_ITEMS_NOT_BANKBOOK  	 		 "N1"		/*Items not on Bankbook beyond limit, declined (Decline)      	 */
#define CUP_F039_APP_OFF_TRN  			 	 "Y1"		/*(Approve), The offline transaction is successful */
#define CUP_F039_UNABLE_ON_APP_OFF_TRN   		 "Y3"		/*(Approve), It is unable to be online. The offline transaction is successful*/
#define CUP_F039_DEC_OFF_TRN  			 	 "Z1"		/*(Decline), The offline transaction fails */
#define CUP_F039_UNABLE_ON_DEC_OFF_TRN   		 "Z3"		/*(Decline), It is unable to be online. The offline transaction fails */

/* Field 43 Card Acceptor Name/Location*/
#define CUP_F043_P1_LEN					25
#define CUP_F043_P2_LEN                                 12
#define CUP_F043_P3_LEN                                 3


/*Field 48 Additional Data-Private*/
#define CUP_F048_USAGE_AA				"AA"
#define CUP_F048_ORG_FIXED_AMT				"100"
#define CUP_F048_ORG_ESTIMATED_AMT			"101"
#define CUP_F048_USAGE_LEN				2
#define CUP_F048_FUNCTION_CODE_LEN			3




	
/*Field 53 Security Related Control Information*/
/*Position 1: Key-Type*/
#define CUP_F053_P1_PIK					'1'
#define CUP_F053_P1_MAK					'2'
#define CUP_F053_P1_LEN					 1

/*Position 1: PIN format used*/
#define CUP_F053_P1_ANSIX_NO_PAN                         '1'
#define CUP_F053_P1_ANSIX_WITH_PAN                       '2'


/*Position 2: Encryption  Method Used*/
#define CUP_F053_P2_SINGLE                                 '0'
#define CUP_F053_P2_DOUBLE                                 '6'
#define CUP_F053_P2_LEN                                     1

/*Position 3: Reserved*/
#define CUP_F053_P3_RESERVED                        "00000000000000"
#define CUP_F053_P3_LEN                                     14

	
/* Field 54 : Additional Amounts */	
	
/* Position 1-2   : Account type */		
#define CUP_F054_P1_SAVING_ACC  			"10"		/*Saving account */
#define CUP_F054_P1_CHECK_ACC 				"20"		/*Checking Account */
#define CUP_F054_P1_CREDIT_CRD_ACC  			"30"		/*Credit card account */
#define CUP_F054_AT_LEN					2		
			
/* Position 3-4  : Balance type */		

#define CUP_F054_P2_LEDGER_BAL  			"01"		/*Ledger Balance */
#define CUP_F054_P2_AVAILABLE_BAL  			"02"		/*Available Balance*/
#define CUP_F054_BT_LEN                                 2
				
		
/* Position 8   : Balance symbol */		
#define CUP_F054_P3_CREDIT  				'C'			/*Credit */
#define CUP_F054_P3_DEBIT  				'D'			/*Debit */
#define CUP_F054_BS_LEN                                 1
		
	
	
/* F060: Point of Service Condition Code */	
	
/* SubField 60.1 Message Reason Code n(4) */	
	
#define CUP_F060_P1_TERM_PROC						"1004"		/*Terminal processed */
#define CUP_F060_P1_ICC_PROC						"1005"		/*ICC processed	 */
#define CUP_F060_P1_ICC_APPl_CNNT_PROC					"1500"		/*ICC application unable to process */
#define CUP_F060_P1_ICC_RAND_SELC					"1502"		/*ICC random selection */
#define CUP_F060_P1_TERM_RAND_SELC					"1503"		/*Terminal random selection */
#define CUP_F060_P1_TERM_CNNT_PROC					"1504"		/*Terminal not able to process ICC */
#define CUP_F060_P1_ON_FORCED_ICC					"1505"		/*On line forced by ICC */
#define CUP_F060_P1_ON_FORCED_ACCEPTOR					"1506"		/*On line forced by card acceptor */
#define CUP_F060_P1_ON_FORCED_CRD					"1507"		/*On line forced by CAD */
#define CUP_F060_P1_ON_FORCED_TERM					"1508"		/*On line forced by terminal */
#define CUP_F060_P1_ON_FORCED_ISS					"1509"		/*On line forced by card Issuer */
#define CUP_F060_P1_OVER_FLR_LMT					"1510"		/*Over floor limit */
#define CUP_F060_P1_CRD_ACCEPT_SUSP					"1511"		/*Card acceptor suspicious */
#define CUP_F060_P1_REF_CRDT_ADJST_PROC					"2000"		/*Refund or Credit Adjustment Processed */
#define CUP_F060_P1_INCORR_TRACE_NO					"2001"		/*Incorrect Trace No. in Chargeback */
#define CUP_F060_P1_NN_RECEIPT_SUP_DOC					"2002"		/*Non-Receipt of Supporting Documentation */
#define CUP_F060_P1_CORR_TRN_DATE					"2003"		/*Correct Transaction Date Provided */
#define CUP_F060_P1_CORR_MERC_LOC					"2005"		/*Correct Merchant Location Provided */
#define CUP_F060_P1_TRN_AUT_ISS						"2008"		/*Transaction Authorized by Issuer */
#define CUP_F060_P1_INV_FRST_CHARGEBACK					"2702"		/*Invalid First Chargeback */
#define CUP_F060_P1_LEG_TRN_RECEIPT					"2704"		/*Legible Transaction Receipt Provided */
#define CUP_F060_P1_ACQ_PROV_TRN_CORR					"2706"		/*Acquirer Can Prove That The Transaction Is Correct */
#define CUP_F060_P1_NNSUP_DOC_CHARGEBACK				"2707"		/*Illegible Supporting Documentation for Chargeback */
#define CUP_F060_P1_FORMAT_ERR_NN_ACTION				"4003"		/*Format error, no action taken */
#define CUP_F060_P1_COMPLETED_PART					"4004"		/*Completed partially */
#define CUP_F060_P1_ORG_AMNT_INCORR					"4005"		/*Original amount incorrect */
#define CUP_F060_P1_RESP_RECV_LATE					"4006"		/*Response received too late */
#define CUP_F060_P1_ACCEPT_CNNT_COMP_TRN				"4007"		/*Card acceptor device unable to complete transaction */
#define CUP_F060_P1_DEPOSIT_OUT_BAL					"4008"		/*Deposit out of balance */
#define CUP_F060_P1_PAYMENT_OUT_BAL					"4010"		/*Payment out of balance */
#define CUP_F060_P1_DEPST_OUT_BAL_APl_CNT				"4011"		/*Deposit out of balance/applied contents */
#define CUP_F060_P1_PYMNT_OUT_BAL_APl_CNT				"4012"		/*Payment out of balance/applied contents */
#define CUP_F060_P1_CNNT_DLVR_MSG_TOTERM				"4013"		/*Unable to deliver message to terminal */
#define CUP_F060_P1_SUSPMALFNC_CRD_RETAIN				"4014"		/*Suspected malfunction/card retained */
#define CUP_F060_P1_SUSPMALFNC_CRD_RETURN				"4015"		/*Suspected malfunction/card returned */
#define CUP_F060_P1_SUSPMALFNC_TRCK3_NUPD				"4016"		/*Suspected malfunction/track 3 not updated */
#define CUP_F060_P1_SUSPMALFNC_NOCASH_DISP				"4017"		/*Suspected malfunction/no cash dispensed */
#define CUP_F060_P1_TIME_OUT_TAK_MNY					"4018"		/*Timed-out at taking money/no cash dispensed */
#define CUP_F060_P1_TIME_OUT_TAK					"4019"		/*Timed-out at taking */
#define CUP_F060_P1_INV_RESP_NN_ACTION					"4020"		/*Invalid response, no action taken */
#define CUP_F060_P1_TIME_OUT_WAIT_RESP					"4021"		/*Timeout waiting for response	 */
#define CUP_F060_P1_GEN_REV_FULL					"4351"		/*Terminal-generated reversal (full amount) */
#define CUP_F060_P1_GEN_REV_PART					"4352"		/*Terminal-generated reversal (partial amount) */
#define CUP_F060_P1_ACQ_RECV_LATE_RESP					"4353"		/*Acquirer received late response from CUPS */
#define CUP_F060_P1_ACQ_DTCT_TIME_OUT					"4354"		/*Acquirer detected time-out */
#define CUP_F060_P1_ACQ_DTCT_INC_MAC					"4355"		/*Acquirer detected incorrect MAC from response */
#define CUP_F060_P1_ACQ_CNNT_SEND_OP					"4356"		/*Acquirer not able to send operating */
#define CUP_F060_P1_CUPS_RCV_LATE_RESP					"4360"		/*CUPS received late response from card Issuer */
#define CUP_F060_P1_CUPS_WAIT_RESP					"4361"		/*CUPS waiting for Issuer’s response until time-out */
#define CUP_F060_P1_CUPS_DTCT_INC_MAC					"4362"		/*CUPS detected incorrect MAC from Issuer’s response */
#define CUP_F060_P1_CUPS_CNNT_FRWD_RESP					"4363"		/*CUPS unable to forward Issuer’s response  to Acquirer */
#define CUP_F060_P1_CUPS_CNNT_FRWD_REQ					"4364"		/*CUPS unable to forward request to transfer-in party */
#define CUP_F060_P1_DISPENSER_OUT_NOTES					"4365"		/*Dispenser out of notes */
#define CUP_F060_P1_REFSD_BY_TRNSFRIN					"4366"		/*Refused by transfer-in	 */
#define CUP_F060_P1_NN_PART_DISBURS_CASH				"4501"		/*Non-Disbursement or Partial Disbursement of Cash at ATM */
#define CUP_F060_P1_PURCH_NNCOMPLET					"4502"		/*Purchase Not Completed */
#define CUP_F060_P1_DISPUTE_DEBITADJ					"4503"		/*Dispute on Debit Adjustment */
#define CUP_F060_P1_DISPUTE_TRN_AMNT_DIFF				"4507"		/*Cardholder Dispute-Transaction Amount Differs */
#define CUP_F060_P1_EXC_LMT_AMNT					"4508"		/*Exceeds Limited or Authorized Amount */
#define CUP_F060_P1_DUPLICATE_PROC					"4512"		/*Duplicate Processing */
#define CUP_F060_P1_FRAUD_MUL_TRN					"4514"		/*Fraudulent Multiple Transactions */
#define CUP_F060_P1_TRN_NNRECOG						"4515"		/*Transaction Not Recognized */
#define CUP_F060_P1_DECLINED_AUT					"4522"		/*Declined Authorization */
#define CUP_F060_P1_ILLEGIBLE_FULF					"4526"		/*Illegible Fulfillment */
#define CUP_F060_P1_FULFILLMENT						"4527"		/*Fulfillment */
#define CUP_F060_P1_CNCLD_PRE_AUTH					"4528"		/*Cancelled Pre-authorization */
#define CUP_F060_P1_QUEST_TRN_RECEIPT					"4531"		/*Questionable Transaction Receipt */
#define CUP_F060_P1_REFUND_NNPROC					"4532"		/*Refund Not Processed */
#define CUP_F060_P1_LATE_PRESENTMENT					"4536"		/*Late presentment */
#define CUP_F060_P1_CNCLD_TRN						"4544"		/*Cancelled Transaction */
#define CUP_F060_P1_TRN_SETT_GOODS_NNRCV				"4557"		/*Transaction was settled but goods or services was not received */
#define CUP_F060_P1_VERIF_TC_FAILS					"4558"		/*Verification for Transaction Certificate (TC) Fails */
#define CUP_F060_P1_TC_RCQ_CNNTBE_PROV					"4559"		/*Trans Certif(TC) and Relevant Calcul Data cannot be Provided */
#define CUP_F060_P1_COUNTERFEIT_CRD					"4562"		/*Counterfeit Card */
#define CUP_F060_P1_INV_REPRESENTMNET					"4570"		/*Invalid Representment */
#define CUP_F060_P1_NW_SUPP_DOC_PROV					"4571"		/*New Supporting Documentation Provided */
#define CUP_F060_P1_CHARGEBACK_RSN_ADJ					"4572"		/*Chargeback Reason Adjusted */
#define CUP_F060_P1_FEES_RFND						"4752"		/*Fees Refund for Unsuccessful Balance Inquiry */
#define CUP_F060_P1_HIGH_RSK_MER					"4802"		/*High Risk Merchant */
#define CUP_F060_P1_PROHIBITED_MER					"4803"		/*Prohibited Merchant */
#define CUP_F060_P1_PAID_OTHER_MEANS					"4806"		/*Paid by Other Means */
#define CUP_F060_P1_CHD_DENS_TRN_NNFC2FC				"4810"		/*The Cardholder  denies  the Transaction-Non-face-to-face*/
#define CUP_F060_P1_ATM_LOC_INQ						"6300"		/*ATM Location Inquiry by Cardholder */
#define CUP_F060_P1_MER_NAMORADR_INQ					"6301"		/*Merchant Name or Address Inquiry by Cardholder */
#define CUP_F060_P1_TRN_RES_INC						"6302"		/*Transaction Result Inquiry by Cardholder */
#define CUP_F060_P1_INQ_PROHB_MER					"6309"		/*Inquiry for Prohibited Merchant */
#define CUP_F060_P1_TRN_RCP_REQ_DIRCT					"6303"		/*Transaction Receipt Request by Cardholder Directly */
#define CUP_F060_P1_TRN_RCP_REQ_RESP					"6304"		/*Transaction Receipt  Req  by  Cardholder  after Responded Inq */
#define CUP_F060_P1_TRN_RCP_REQ_UNRESP					"6305"		/*Transaction Transaction  Receipt  Req  after  Un-responded Inq */
#define CUP_F060_P1_SECONDI_INQ						"6340"		/*Second Inquiry */
#define CUP_F060_P1_TRN_RCP_REQ_JUSTC					"6343"		/*Transaction Receipt Request by Justice */
#define CUP_F060_P1_TRN_RCP_REQ_FRD_TRN					"6345"		/*Transaction Receipt  Request  for  Fraudulent Transactions	 */
#define CUP_F060_P1_CUPS_PRFM_STNDIN_FEE				"8010"		/*CUPS performs stand-in fee collection or fund disbursement */
#define CUP_F060_P1_EME_CASH_CH_GOTCASH					"7525"		/*Emergency Cash Service Fee (cardholder got the cash) */
#define CUP_F060_P1_EME_CASH_CH_NNGOTCASH				"7526"		/*Emergency Cash Service Fee (cardholder did not get the cash) */
#define CUP_F060_P1_INQ_RETV_REQ					"7610"		/*Inquire Retrieval Request */
#define CUP_F060_P1_APPL_FEE_SCND					"7611"		/*Application fee for second appeal */
#define CUP_F060_P1_DISPUTED_AMNT					"7612"		/*Disputed amount */
#define CUP_F060_P1_PNALTY_LATE_PYMNT					"7613"		/*Penalty for late payment of disputed amount */
#define CUP_F060_P1_RTRN_INTERCH_FEE					"7614"		/*Return interchange fee due to incorrect region code */
#define CUP_F060_P1_ARBIT_FIL_REVW_FEE					"7620"		/*Arbitration filing/review fee */
#define CUP_F060_P1_AUTH_PROC_CHARGE					"7630"		/*Authorization processing charge */
#define CUP_F060_P1_MISC_CHARGE						"7640"		/*Misc. charges */
#define CUP_F060_P1_CHARGE_CLEAR_ISS					"7650"		/*Service charge for clearing of Issuer */
#define CUP_F060_P1_MER_RSK_VAL_FEE					"7660"		/*Merchant risk validation fee */
#define CUP_F060_P1_CHARGE_DEL_SETT					"7680"		/*Charge for delayed settlement */
#define CUP_F060_P1_ACC_FOLLOW_FEE					"7690"		/*Account follow-up fee */
#define CUP_F060_P1_VALUE_ADD_TAX					"7700"		/*Value-added tax */
#define CUP_F060_P1_FEE_NNMEMBER					"7710"		/*Register/annual fee for non-member */
#define CUP_F060_P1_CHARGE_CHARGEBACK					"7720"		/*Commission charge for chargeback */
#define CUP_F060_P1_CHARGE_REPR_APPEAL					"7730"		/*Commission charge for Representment/appeal */
#define CUP_F060_P1_NNPERFM_FEE						"7740"		/*Nonperformance fee */
#define CUP_F060_P1_BIN_COPYRIGHT_REGFEE				"7750"		/*BIN copyright and register fee */
#define CUP_F060_P1_REGFEE_GUAR_MEMBER					"7760"		/*Register fee for guaranteed member */
#define CUP_F060_P1_CHARGE_DATA_EXCH_FORM				"7770"		/*Charge for data exchange forms */
#define CUP_F060_P1_SERV_POSTPON_CHARGE					"7785"		/*Service charge/charge for postponing */
#define CUP_F060_P1_REPAYMENT						"7790"		/*Repayment */
#define CUP_F060_P1_TLGRAPH_TELEPH_CAHRGE				"7800"		/*Telegraph, telephone and cable charge */
#define CUP_F060_P1_LOST_CRD_FEE					"7810"		/*Lost card reporting fee */
#define CUP_F060_P1_MER_SERV_FEE					"7820"		/*Merchant service fee */
#define CUP_F060_P1_TECHNICAL_VIOL					"9806"		/*Technical Violation */
#define CUP_F060_P1_RSK_FEE						"9807"		/*Risk Retribution Fee */
#define CUP_F060_P1_TRN_CNCL_BY_CH					"9600"		/*Transaction Cancelled by Cardholder */
#define CUP_F060_P1_SURPLUS_TRN_AMNT					"9601"		/*Surplus of Transaction Amount Identified by Acquirer or receiver */
#define CUP_F060_P1_NNDISB_CASH_ATM					"9602"		/*Non-Disbursement of Cash at ATM */
#define CUP_F060_P1_PART_DISB_CASH_ATM					"9603"		/*Partial Disbursement of Cash at ATM */
#define CUP_F060_P1_TRN_AMNT_DIF_PURCH_ADJ				"9650"		/*Transaction Amount Differs-Purchase */
#define CUP_F060_P1_TRN_AMNT_DIF_ATM_ADJ				"9651"		/*Transaction Amount Differs-ATM */
#define CUP_F060_P1_INC_CREDIT_ADJ					"9652"		/*Incorrect Credit Adjustment */
#define CUP_F060_P1_TIME_PREAUT_EXC_ADJ					"9653"		/*Time Frame for Pre-auth Completion Exceeded (Debit Adjust Msg) */
#define CUP_F060_P1_AB_REV_TRN_ADJ					"9654"		/*Abnormal Reversal Transaction (Debit Adjustment Message) */
#define CUP_F060_P1_AB_CNCL_TRN_ADJ					"9655"		/*Abnormal Cancellation Transaction   (Debit Adjustment Message) */
#define CUP_F060_P1_TRN_AMNT_DIFPURC_MALF				"9656"		/*Transaction   Amount  Differs-Purchase  Due to System Malfunction*/
#define CUP_F060_P1_INC_ADJ_RFN_VCHR					"9657"		/*Incorrect Credit Adjustment, Manual Refund or Credit Voucher	 */
#define CUP_F060_P1_INC_CREDITADJ_DEBCRD				"9700"		/*Incorrect Credit Adjustment of Debit Card */
#define CUP_F060_P1_QUEST_FRST_CHARGEBACK				"9701"		/*Questionable First Chargeback of Debit Card */
#define CUP_F060_P1_DEFICIT_TRN_AMNT_ACQ				"9702"		/*Deficit of Transaction Amount at Acquirer */
#define CUP_F060_P1_TIME_FRAME_EXC					"9703"		/*Time Frame Exceeded */
#define CUP_F060_P1_GDRC_ANDED						"9704"		/*General Dispute Resolution Cycle (GDRC) Ended */
#define CUP_F060_P1_TRN_NNFND_UPISYS					"9705"		/*Transaction Record Not Found in UPI System */
#define CUP_F060_P1_OTHER_AGR_PYMNT					"9706"		/*Other Mutually Agreed Payment	 */
#define CUP_F060_P1_MANUAL_RFND						"9707"		/*Manual Refund	 */
#define CUP_F060_P1_TRN_AMNT_DIF_PURCH					"9720"		/*Transaction Amount Differs-Purchase(Good Faith Collection Msg) */
#define CUP_F060_P1_TRN_AMNT_DIF_ATM					"9721"		/*Transaction Amount Differs-ATM      (Good Faith Collection Msg) */
#define CUP_F060_P1_INC_REFUND						"9722"		/*Incorrect Refund */
#define CUP_F060_P1_TIME_PREAUT_EXC					"9723"		/*Time Fram for PreAut Completion Exceed(Good Faith Collection Msg)*/
#define CUP_F060_P1_ABNRM_REV_TRN					"9724"		/*Abnormal Reversal Transaction (Good Faith Collection Message) */
#define CUP_F060_P1_ABNRM_CNCL_TRN					"9725"		/*Abnormal Cancellation Transaction(Good Faith Collection Message)*/
#define CUP_F060_P1_LEN			4

/* SubField 60.2 Additional Point Of Service Information ans11 */

#define CUP_F060_P2_LEN			11

/* SF60.2.1 Account Holder Type (1 byte) */

#define CUP_F060_P2_1_INDV_CRD_ACC 					'0'			/*Individual card account */
#define CUP_F060_P2_1_INDV_NNCRD_ACC 					'1'			/*Individual non-card account */
#define CUP_F060_P2_1_CORP_NNCRD_ACC 					'2'			/*Corporate non-card account */
#define CUP_F060_P2_1_CORP_CRD_ACC 					'3'			/*Corporate card account */
#define CUP_F060_P2_1_UNKNOWN 						'9'			/*Unknown or irrelevant Account Holder Type */
#define CUP_F060_P2_1_LEN						1

/* SubF60.2.2 Terminal Entry Capability (1 byte)	Terminal Entry Capability (1 byte) */

#define CUP_F060_P2_2_UNKNOWN   					'0'			/*Unknown                */
#define CUP_F060_P2_2_MAG_READ_CAP  					'2'			/*Magnetic stripe read capability */
#define CUP_F060_P2_2_CNTICC_MAG   					'5'			/*Contact IC+magnetic read. Cannot read contactless IC card*/
#define CUP_F060_P2_2_LESSICC_CNTICC_MAG   				'6'			/*Contactless IC + Contact IC+ magnetic read*/
#define CUP_F060_P2_2_LEN						 1

/* SubF60.2.3 Chip Condition Code (1 byte)	Chip Condition Code (1 byte) */

#define CUP_F060_P2_3_NOT_APPL		 				'0'			/*Not applicable; subsequent subfields are present */
#define CUP_F060_P2_3_DRCT_SWIP_MAG					'1'			/*direct magnetic stripe swiping without inserting for chip*/
#define CUP_F060_P2_3_FALLBACK	 					'2'			/*fallback transaction happened*/
#define CUP_F060_P2_3_LEN						 1

/*Reserved*/
#define	CUP_F060_P2_4_RESERVED						'0'
#define CUP_F060_P2_4_LEN						 1

/* SubF60.2.5 Terminal Type (2 byte) */

#define CUP_F060_P2_5_UNKNOWN    					"00"		/*Unknown              	 */
#define CUP_F060_P2_5_ATM_CDM     					"01"		/*ATM、CDM              	 */
#define CUP_F060_P2_5_POS    						"03"		/*POS                  	 */
#define CUP_F060_P2_5_MLTMED_TERM    					"05"		/*Multimedia terminal  	 */
#define CUP_F060_P2_5_COUNTER     					"06"		/*Counter              	 */
#define CUP_F060_P2_5_PC     						"07"		/*PC                   	 */
#define CUP_F060_P2_5_MOBILE_PHONE    					"08"		/*Mobile phone         	 */
#define CUP_F060_P2_5_I_FIX_PHONE    					"09"		/*I type fixed phone   	 */
#define CUP_F060_P2_5_WIRLSS_POS     					"11"		/*Wireless POS         	 */
#define CUP_F060_P2_5_CDRS     						"12"		/*CDRS                 	 */
#define CUP_F060_P2_5_MER_SYS    					"14"		/*Merchant’s System    	 */
#define CUP_F060_P2_5_SETUP_BOX     					"16"		/*Set Up Box           	 */
#define CUP_F060_P2_5_II_FIX_PHONE    					"17"		/*II type fixed phone  	 */
#define CUP_F060_P2_5_BATCH_FILE_PROC     				"20"		/*Batch File Processing	 */
#define CUP_F060_P2_5_MPOS     						"23"		/*mPOS 					 */
#define CUP_F060_P2_5_LEN						2

/* SubF60.2.6 Signature-Only Indicator/Receiver’s Currency Indicator (1 byte) */

#define CUP_F060_P2_6_NNSIGNATURE    					'0'		/*Non Signature-based Credit Card Network */
#define CUP_F060_P2_6_SIGNATURE    					'1'		/*Signature-based Credit Card Network */
#define CUP_F060_P2_6_REM_CURR_IS_RCV_CURR     				'0'		/*The Remittee's acc curry is the primary curry of recv's acc*/
#define CUP_F060_P2_6_REM_CURR_IS_SNDR_CURR    				'2'		/*The Remittee's account currency is the original currency of sendr*/ 
#define CUP_F060_P2_6_LEN		1


/* SubF60.2.7 IC Card Authentication Reliability Identifier (1 byte) */

#define CUP_F060_P2_7_FILL 						'0'		/*Fill for fld 60, if pos 8 or subsequent positions are present. */
#define CUP_F060_P2_7_ACQ_CRD_AUT_REL 					'1'		/*Acquirer indicates that Card Authentication may not be reliable. */
#define CUP_F060_P2_7_ACQ_INACTIVE					'2'		/*Switch Center indicates acquirer inactive for Card Authentication*/
#define CUP_F060_P2_7_ISS_INACTIVE 					'3'		/*Switch Center indicates issuer inactive for Card Authentication. */
#define CUP_F060_P2_7_TRN_TKN_SERV_UP 					'4'		/*transaction has used Token Service provided by UnionPay.*/
#define CUP_F060_P2_7_LEN							1 



/* SubF60.2.8 Ecommerce Identifier (ECI) (2 byte) */

#define CUP_F060_P2_8_NOT_APPLICABLE    			"00"		/*Not applicable,               */
#define CUP_F060_P2_8_UP_SAFE_ENTRY_MODE    			"01"		/*UP safe entry mode auth, and cardholder security inform input success */
#define CUP_F060_P2_8_CER_ISS_SAA_DRCT_AUT   			"03"		/*Conduct the certif of Iss SAA direct authentication authorization*/
#define CUP_F060_P2_8_AUT_ISS_SA_DRCT_VRF    			"05"		/*Conduct the authentication of Issuer SA direct status verification*/
#define CUP_F060_P2_8_ISS_DRCT_VRF    				"06"		/*Tried to conduct the issuer direct status verification */
#define CUP_F060_P2_8_FAIL_CUPSCRE_SECUTECH    			"07"		/*Failed CUPSecure safe authentication. adopt the secu techno of channel */
#define CUP_F060_P2_8_FAIL_CUPSCRE_NNSECUTECH    		"08"		/*Failed CUPSecure safe authentication. not adopt the secu techno encrypt*/
#define CUP_F060_P2_8_ISS_AUTH_MODE    				"09"		/*Issuer Authentication Mode in card-no-present self-service transactions. */
#define CUP_F060_P2_8_ISS_NNAUTH_MODE   			"10"		/*Issuer Non-Authentication Mode in card-no-present self-service transact */
#define CUP_F060_P2_8_LEN							2 

/* SubF60.2.9 Interactive Mode Identifier (1 byte) */


#define CUP_F060_P2_9_DEFAULT     				 '0'		/*Default */
#define CUP_F060_P2_9_INTERNET     				 '1'		/*Internet */
#define CUP_F060_P2_9_TEXT_MSG      				 '2'		/*Text Message */
#define CUP_F060_P2_9_VOICE     				 '3'		/*Voice */
#define CUP_F060_P2_9_LEN					 1 




/* Field 60.3 Additional Transaction Information ans..15 (LLVAR) */

/*F60.3.1: Special Pricing Type (Currently not used)*/

#define	CUP_F060_P3_1_NOTUSED					"00"
#define CUP_F060_P3_1_LEN					 2

/*F60.3.2: Special Pricing Level (Currently not used) */	

#define CUP_F060_P3_2_NOTUSED                                   '0'
#define CUP_F060_P3_2_LEN                                        1

 	
/* SubF60.3.3: Minor Unit of Transaction Currency	 */

#define CUP_F060_P3_3_MINOR_UNIT_0   				"a00"		/*0 Minor Unit (currently, only used in Vietnam Dong Transaction) */
#define CUP_F060_P3_3_MINOR_UNIT_1 				"100"		/*1 Minor Unit */
#define CUP_F060_P3_3_MINOR_UNIT_2  				"200"		/*2 Minor Unit (currently, only used in Indonesia Rupiah Transaction) */
#define CUP_F060_P3_3_MINOR_UNIT_3  				"300"		/*3 Minor Unit */
#define CUP_F060_P3_3_DEFAULT  					"000"		/*Default value	 */
#define CUP_F060_P3_3_LEN					3

/* SubF60.3.4:Partial Approval Indicator */
	
#define CUP_F060_P3_4_NOT_SUPPORTED 				'0'			/*Not supported */
#define CUP_F060_P3_4_SUPPORTED 				'1'			/*Supported */
#define CUP_F060_P3_4_LEN					 1

/* SubF60.3.5: Transaction Initiation Mode */
	
#define CUP_F060_P3_5_UNKNOWN 					'0'		/*Unknown      */
#define CUP_F060_P3_5_ATTENDED 					'1'		/*Attended(initiated on attended terminals. Ex: merchant acceptor; counter)*/
#define CUP_F060_P3_5_UNATTENDED 				'2'		/*Unattended ( initiated by cardholders at unattended terminals) */
#define CUP_F060_P3_5_AGENT 					'3'		/*Agent (transactions initiated by merchant on cardholder’s behalf*/
#define CUP_F060_P3_5_BATCH_AGENT 				'4'		/*Batch Agent  */
#define CUP_F060_P3_5_LEN					1

/* SubF60.3.6: Transaction Medium */	

#define CUP_F060_P3_6_UNKNOWN    				'0'			/*Unknown */
#define CUP_F060_P3_6_MAG_TRN   				'1'			/*Magnetic stripe card transaction */
#define CUP_F060_P3_6_ICC_TRN  					'2'			/*Chip card transaction via chip */
#define CUP_F060_P3_6_FALLBACK  				'3'			/*Magstripe transaction of Chip and Magstripe hybrid card */
#define CUP_F060_P3_6_VIRTL_CRD_TRN   				'4'			/*Virtual card transaction */
#define CUP_F060_P3_6_PURE_CHAR_TRN   				'5'			/*Pure characters transaction */
#define CUP_F060_P3_6_BIO_TRAIT_TRN   				'6'			/*Biolgoical traits transaction */
#define CUP_F060_P3_6_CRD_NN_PRESNT   				'7'			/*Card no present transaction */
#define CUP_F060_P3_6_LEN		1

/* SubF60.3.7: IC Card Application Type */	
#define CUP_F060_P3_7_UNKNOWN 					'0'			/*Unknown                                            */
#define CUP_F060_P3_7_DEBIT_CREDIT_APPL 			'1'			/*debit/credit application (Contact/Contactless)     */
#define CUP_F060_P3_7_EMV_STNRD_APPL 				'2'			/*EMV standard debit/credit application.             */
#define CUP_F060_P3_7_ELECTR_CASH_APPL 				'3'			/*electronic cash application (contact/ contactless) */
#define CUP_F060_P3_7_LEN					 1

/* SubF60.3.8: Account Attribute */ 	
#define CUP_F060_P3_8_UNKNOWN  					"00"		/*Unknown */
#define CUP_F060_P3_8_DEBIT_ACC  				"01"		/*Debit account */
#define CUP_F060_P3_8_CREDIT_ACC 				"02"		/*Credit account */
#define CUP_F060_P3_8_QUASICREDIT  				"03"		/*Quasi-credit account */
#define CUP_F060_P3_8_CREDIT_DEBIT  				"04"		/*Credit/Debit account */
#define CUP_F060_P3_8_PREPAID_ACC  				"05"		/*Prepaid account */
#define CUP_F060_P3_8_LEN		2

/* SubF60.3.9: Card level */  	
#define CUP_F060_P3_9_UNKNOWN 					'0'			/*Unknown             */
#define CUP_F060_P3_9_UP_CLASSIC 				'1'			/*UnionPay Classic    */
#define CUP_F060_P3_9_UP_GOLD 					'3'			/*UnionPay Gold       */
#define CUP_F060_P3_9_UP_PLATINUM 				'4'			/*UnionPay Platinum   */
#define CUP_F060_P3_9_UP_DIAMOND				'5'			/*UnionPay Diamond    */
#define CUP_F060_P3_9_LEN		1

/* SubF60.3.10 : Card Product */ 	
#define CUP_F060_P3_10_UNKNOWN  				"00"		/*Unknown */
#define CUP_F060_P3_10_OFFICIAL_CRD  				"01"		/*Official Card */
#define CUP_F060_P3_10_FEE_PYMNT_CRD 				"02"		/*Fee Payment Card */
#define CUP_F060_P3_10_AIRLINE_CRD  				"03"		/*Airline Card */
#define CUP_F060_P3_10_STUDENT_CRD  				"04"		/*Student Card */
#define CUP_F060_P3_10_SOCIAL_SECU_CRD 				"05"		/*Social Security Card */
#define CUP_F060_P3_10_HEALTH_CRD  				"06"		/*Health Card */
#define CUP_F060_P3_10_FUNONG_CRD  				"07"		/*FuNong Card */
#define CUP_F060_P3_10_MILITARY_CRD  				"08"		/*Military Card */
#define CUP_F060_P3_10_FAMILY_CRD				"09"		/*Family Card */
#define CUP_F060_P3_10_SETTLEMNT_CRD  				"10"		/*Settlement Card */
#define CUP_F060_P3_10_TRAVEL_CRD  				"40"		/*Travel Card */
#define CUP_F060_P3_10_CORPORATE_CRD  				"80"		/*Corporate Card */
#define CUP_F060_P3_10_BUSNSS_TRAVEL_CRD  			"81"		/*Business Travel Card */
#define CUP_F060_P3_10_PURCHASE_CRD  				"82"		/*Purchase Card */
#define CUP_F060_P3_10_LEN		2

/* Field 61 : Cardholder Authentication Information */

/* SubField 61.1 ID Number (ans22)*/

#define CUP_F061_P1_LEN              22

/* SubField 61.2 CVN Check Result (ans1), 61.3  PVN Check Result (ans1) and SubField 61.4 Card-Not-Present check Result ans1 */	

#define CUP_F061_CHECK_RESULT_SUCC 				 '1'		/*succeeded */
#define CUP_F061_CHECK_RESULT_FAIL  				 '2'		/*failed */
#define CUP_F061_CHECK_RESULT_UNV   				 '3'		/*unverified */
#define CUP_F061_CHECK_RESULT_LEN				  1	
	
/* SubField 61.5 ARQC Authentication Result (Ans1) */	

#define CUP_F061_P5_PASSES_ARQC_AUTH   				'1'			/*Transaction passes ARQC authentication*/
#define CUP_F061_P5_FAILS_ARQC_AUTH   				'2'			/*Transaction fails ARQC authentication */
#define CUP_F061_P5_NO_ARQC_AUTH   				'3'			/*No ARQC  authentication conducted */
#define CUP_F061_P5_LEN		1


/* Field 70 : Network Management Information Code */

#define CUP_F070_SIGNS_ON      					"001"		/*Member  signs  on */
#define CUP_F070_SIGNS_OFF     					"002"		/*Member  signs  off */
#define CUP_F070_UP_RESET_KEY      				"101"		/*UnionPay resets the key (Message Type 0800/0810) */
#define CUP_F070_REQ_RESET_KEY      				"101"		/*Member requests to reset the key */
#define CUP_F070_UP_START_DATE_SW      				"201"		/*UnionPay starts date switch */
#define CUP_F070_UP_END_DATE_SW      				"202"		/*UnionPay ends date switch */
#define CUP_F070_ECHO_TEST      				"301"		/*Echo test */
#define CUP_F070_STND_AUT_ECHO_MSG      			"401"		/*Stand-in authorization Echo message. */
#define CUP_F070_STND_AUT_TRN_ACT     				"501"		/*Stand-in authorization transaction transmission activate */
#define CUP_F070_STND_AUT_ON_TRN_FINI      			"502"		/*Stand-in authorization online transmission finish/termination.*/
#define CUP_F070_LEN		3

#define CUP_F090_P1_LEN         4
#define CUP_F090_P2_LEN         6
#define CUP_F090_P3_LEN         10
#define CUP_F090_P4_LEN         11
#define CUP_F090_P5_LEN         11

/****************************************************/
/*Field 125 Security and Risk Assessment Information*/
#define CUP_F125_NB_FIELDS					8
#define CUP_F125_CA_COMP_ACC_IND 			"CA" /*Compromised Account Indicator*/
#define CUP_F125_QR_PAYMENT 					"QR" /*QRC-based Payment*/
#define CUP_F125_CV_CUST_VERIF 			    "CV" /*Consumer Verification Information*/
#define CUP_F125_IP_INNOV_PAY 				"IP" /*Innovative Payment*/
#define CUP_F125_LR_LOW_RISK_ID 	            "LR" /*Low Risk Identifier*/
#define CUP_F125_TF_AUTH_TRS_ID 		        "TF" /*Authentication Transaction Identifier*/
#define CUP_F125_QP_UPI_APP_IND 			    "QP" /*UnionPay Application Indicator*/
#define CUP_F125_TP_TRANS_PROD_INF 			"TP" /*Transaction Product Information*/


#define CUP_F002_MAX_LEN	19
#define CUP_F003_LEN		6
#define CUP_F004_LEN		12
#define CUP_F005_LEN		12
#define CUP_F006_LEN		12
#define CUP_F007_LEN		10
#define CUP_F009_LEN		8
#define CUP_F010_LEN		8
#define CUP_F011_LEN		6
#define CUP_F012_LEN		6
#define CUP_F013_LEN		4
#define CUP_F014_LEN		4
#define CUP_F015_LEN		4
#define CUP_F016_LEN		4
#define CUP_F018_LEN		4
#define CUP_F019_LEN		3
#define CUP_F022_LEN		3
#define CUP_F023_LEN		3
#define CUP_F025_LEN		2
#define CUP_F026_LEN		2
#define CUP_F028_LEN		8
#define CUP_F032_MAX_LEN	11
#define CUP_F033_MAX_LEN	11
#define CUP_F035_MAX_LEN	37
#define CUP_F036_MAX_LEN	104
#define CUP_F037_LEN		12
#define CUP_F038_LEN		6
#define CUP_F039_LEN		2
#define CUP_F041_LEN		8
#define CUP_F042_LEN		15
#define CUP_F043_LEN		40
#define CUP_F044_MAX_LEN	25
#define CUP_F045_MAX_LEN	79
#define CUP_F048_MAX_LEN	512
#define CUP_F049_LEN		3
#define CUP_F050_LEN		3
#define CUP_F051_LEN		3
#define CUP_F052_LEN		16
#define CUP_F053_LEN		16
#define CUP_F054_MAX_LEN	40
#define CUP_F055_MAX_LEN	255
#define CUP_F057_MAX_LEN	100
#define CUP_F059_MAX_LEN	600
#define CUP_F060_MAX_LEN	100
#define CUP_F061_MAX_LEN	200
#define CUP_F063_MAX_LEN	512
#define CUP_F070_LEN		3
#define CUP_F090_LEN		42
#define CUP_F096_LEN		64
#define CUP_F100_MAX_LEN	11
#define CUP_F102_MAX_LEN	28
#define CUP_F104_MAX_LEN	512
#define CUP_F117_MAX_LEN	256
#define CUP_F121_MAX_LEN	100
#define CUP_F122_MAX_LEN	100
#define CUP_F123_MAX_LEN	100
#define CUP_F125_MAX_LEN	256
#define CUP_F128_LEN		64

/*HAL 20210427 FOR DBS DBS-14*/
#define CUP_F55_APP_CRYPTOGRAM    "9F26"
#define CUP_F55_CRYPTO_INFO_DATA  "9F27"
#define CUP_F55_ISS_APP_DATA      "9F10"
#define CUP_F55_UNPRED_NUMBER     "9F37"
#define CUP_F55_ATC               "9F36"
#define CUP_F55_TVR 			  "95"
#define CUP_F55_TRANS_DATE        "9A"
#define CUP_F55_TRANS_TYPE        "9C"
#define CUP_F55_TRANS_AMOUNT      "9F02"
#define CUP_F55_TRANS_CUR_CODE    "5F2A"
#define CUP_F55_APP_INTER_PROFILE "82"
#define CUP_F55_TERM_COUNTRY_CODE "9F1A"
#define CUP_F55_OTHER_AMOUNT      "9F03"
#define CUP_F55_TERM_CAP          "9F33"
#define CUP_F55_CVM              "9F34"
#define CUP_F55_TERM_TYPE        "9F35"
#define CUP_F55_IFD_SERIAL_NUM   "9F1E"
#define CUP_F55_DED_FILE_NAME 	 "84"
#define CUP_F55_TERM_APP_VER_NUM "9F09"
#define CUP_F55_TRANS_SEQ_NUM    "9F41"
#define CUP_F55_ISS_AUTHEN_DATA   "91"
#define CUP_F55_ISS_SCRIPT1 	 "71"
#define CUP_F55_ISS_SCRIPT2      "72"
#define CUP_F55_TRANS_CATEG_CODE "9F53"

#define CUP_F55_NB_FIELDS				  24
/*HAL 20210427 FOR DBS DBS-14*/

#define  CUP_LG_MAX             1900
#define	 CUP_STATION_ID_LEN	11




#endif