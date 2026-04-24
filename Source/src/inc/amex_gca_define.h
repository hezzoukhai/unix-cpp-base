#ifndef HPS_AMEX_GCA_DEFINE_H
#define HPS_AMEX_GCA_DEFINE_H

/*-- Fields length*/
#define AMEX_F003_LEN           				  6
#define AMEX_F004_LEN           				  12
#define AMEX_F007_LEN           				  10
#define AMEX_F011_LEN           				  6
#define AMEX_F012_LEN           				  12
#define AMEX_F013_LEN           				  4
#define AMEX_F014_LEN           				  4
#define AMEX_F019_LEN           				  3
#define AMEX_F022_LEN           				  12
#define AMEX_F024_LEN							  3
#define AMEX_F025_LEN           				  4
#define AMEX_F026_LEN           				  4
#define AMEX_F027_LEN           				  1
#define AMEX_F030_LEN           				  24
#define AMEX_F054_ACCT_TYPE_LEN                   2
#define AMEX_F054_AMNT_TYPE_LEN                   2
#define AMEX_F054_CCY_LEN                         3
#define AMEX_F054_SIGN_LEN                        1
#define AMEX_F054_AMNT_LEN                        12
#define AMEX_F054_LEN                       	  20



/*-- F3: Processing code list*/
/*1100*/
#define AMEX_F003_CARD_AUTH_REQ 			"004000"	/*Card Authorization Request*/
#define AMEX_F003_AAV_AND_AUTH		        "004800"	/*Combination Automated Address Verification (AAV) and Authorization*/
#define AMEX_F003_AMEX_EM_CHECK_CASHING		"034000"	/*AMEX Emergency Check Cashing									*/
#define AMEX_F003_AMEX_TRAV_CHEQUE_ENCASH	"064000"	/*AMEX Travelers Cheque Encashment                              */
#define AMEX_F003_AAV_ONLY					"174800"    /*Transaction for Automated Address Verification (AAV) Only     */
#define AMEX_F003_ZVAV_OR_ZVAV_CID			"334000" 	/*ZVAV or ZVAV with Keyed 4 Digit CID							*/
#define AMEX_F003_ZVAV_AAV_OR_ZVAV_AAV_CID	"334800" 	/*ZVAV with AAV or ZVAV with AAV and Keyed 4 Digit CID			*/
/*1220*/
#define AMEX_F003_AUTH_ADJUSTEMENT			"220000"	/*Authorization Adjustment                                      */


/*1420*/
#define AMEX_F003_SYSTEM_GENERATED_REV		"004000"	/*Card Reversal Advice — System Generated Reversal              */
#define AMEX_F003_MERCHANT_INITIATED_REV	"024000"	/*Merchant Initiated Reversal                                   */


/*1804*/
#define AMEX_F003_ECHO_MESSAGE				"000000"	/*System Audit Control/Echo Message "Are you there?"            */

/*-- F24: Function code list*/
#define AMEX_F024_AUTH_RREQUEST				"100"
#define AMEX_F024_PARTIAL_AUTH				"181"
#define AMEX_F024_AUTH_WITH_BALANCE_RETURN	"182"
#define AMEX_F024_SIGNON   					"801" 
#define AMEX_F024_SIGNOFF				   	"802" 
#define AMEX_F024_DYNAMIC_KEY_EXCHANGE   	"811" 
#define AMEX_F024_ECHO_MESSAGE           	"831" 	


/*-- F26: Function code list*/
#define AMEX_F027_AUTHOR_ID_LENGTH			  '6'


/*-- F34: PRIMARY ACCOUNT NUMBER, EXTENDED*/
/*PAN response codes*/
#define	AMEX_F034_PAN_RETURNED				  'Y'	 
#define	AMEX_F034_LAST_4_DIGITS_OF_PAN		  'F'
#define	AMEX_F034_PAN_EXP_DT				  'E'


/*-- F44: ADDITIONAL RESPONSE DATA*/
/*CID/4DBC/4CSC response code*/
#define AMEX_F044_4CSC_OK              		 'Y'	/*4CSC matched*/
#define AMEX_F044_4CSC_NOK               	 'N'	/*4CSC did not match*/
#define AMEX_F044_4CSC_NO_CHECK 			 'U'	/*4CSC was not checked*/

/*-- F39: Action code list*/
#define AMEX_F039_PARTIAL_APPROVAL			"002"
#define AMEX_F039_REQUEST_ACCEPTED			"800" /*  FZL20221018 PLUTONL-4979*/
#define AMEX_F039_FUNCTION_NOT_SUPPORTED    "115" /*  FZL20221018 PLUTONL-4979*/

/*-- F47: ADDITIONAL DATA - NATIONAL*/
/*ITD: Internet Telephone Data*/
#define	AMEX_F047_ITD_PRIM_ID						"AX"
#define	AMEX_F047_ITD_PRIM_ID_LEN					2
#define	AMEX_F047_ITD_SECOND_ID						"ITD"
#define	AMEX_F047_ITD_SECOND_ID_LEN					3
#define	AMEX_F047_ITD_CE_ID							"CE "	
#define	AMEX_F047_ITD_CE_ID_LEN						3
#define	AMEX_F047_ITD_CE_VLI_LEN					2
#define	AMEX_F047_ITD_CE_LEN						60 
#define	AMEX_F047_ITD_CH_ID							"CH "	
#define	AMEX_F047_ITD_CH_ID_LEN						3
#define	AMEX_F047_ITD_CH_VLI_LEN					2
#define	AMEX_F047_ITD_CH_LEN						60
#define	AMEX_F047_ITD_HBT_ID						"HBT"	
#define	AMEX_F047_ITD_HBT_ID_LEN					3
#define	AMEX_F047_ITD_HBT_VLI_LEN					2
#define	AMEX_F047_ITD_HBT_LEN						60
#define	AMEX_F047_ITD_STC_ID						"STC"	
#define	AMEX_F047_ITD_STC_ID_LEN					3
#define	AMEX_F047_ITD_STC_ID_VLI					"03"
#define	AMEX_F047_ITD_STC_ID_VLI_LEN				2
#define	AMEX_F047_ITD_STC_LEN						3
#define	AMEX_F047_ITD_SM_ID							"SM "	
#define	AMEX_F047_ITD_SM_ID_LEN						3
#define	AMEX_F047_ITD_SM_ID_VLI						"02"
#define	AMEX_F047_ITD_SM_ID_VLI_LEN					2
#define	AMEX_F047_ITD_SM_LEN						2
#define	AMEX_F047_ITD_SM_SAME_DAY					"01"
#define	AMEX_F047_ITD_SM_OVERNIGHT					"02"
#define	AMEX_F047_ITD_SM_2_3_DAYS					"03"
#define	AMEX_F047_ITD_SM_4_DAYS_OR_MORE				"04"
#define	AMEX_F047_ITD_SM_ELECTRONIC_DEL				"05"
#define	AMEX_F047_ITD_SM_SHIP_TO_STORE				"06"
#define	AMEX_F047_ITD_MPS_ID						"MPS "	
#define	AMEX_F047_ITD_MPS_ID_LEN					3
#define	AMEX_F047_ITD_MPS_VLI_LEN					2
#define	AMEX_F047_ITD_MPS_MAX_LEN					15
#define	AMEX_F047_ITD_CUSTOMER_IP_LEN				15
#define	AMEX_F047_ITD_CUSTOMER_ANI_LEN				10
#define	AMEX_F047_ITD_CUSTOMER_II_DIGITS_LEN		2


/*Goods Sold data*/
#define	AMEX_F047_GS_PRIM_ID				"AX"
#define	AMEX_F047_GS_PRIM_ID_LEN			2
#define	AMEX_F047_GS_SECOND_ID				"CPD"
#define	AMEX_F047_GS_SECOND_ID_LEN			3
#define	AMEX_F047_GS_VERSION_NB				"01"	
#define	AMEX_F047_GS_VERSION_NB_LEN			2
#define	AMEX_F047_GS_ID						"GS "	
#define	AMEX_F047_GS_ID_LEN					3
#define	AMEX_F047_GS_VLI_LEN	 			2
#define	AMEX_F047_GS_PRODUCT_CODE_LEN	 	4
#define	AMEX_F047_GS_GIFT_CARD	 			"1000"
		
/*-- F55: ICC tags list*/
#define AMEX_TAG_ISS_SCRIPT1         "7100" 
#define AMEX_TAG_ISS_SCRIPT2         "7200"
#define AMEX_TAG_ISS_SCRIPT_CMD      "8600"
#define AMEX_TAG_APP_INTER_PROFILE   "8200"
#define AMEX_TAG_DED_FILE_NAME       "8400"
#define AMEX_TAG_ICC_APP_ID          "4F00"
#define AMEX_TAG_TERM_CAP_PROFILE    "9F33"
#define AMEX_TAG_AUTHOR_RESP_CODE    "8A00"
#define AMEX_TAG_ISS_AUTHEN_DATA     "9100"
#define AMEX_TAG_TVR                 "9500"
#define AMEX_TAG_ISS_SCRIPT_RESULT   "9600" 
#define AMEX_TAG_TRANS_DATE          "9A00"
#define AMEX_TAG_TRANS_TYPE          "9C00"
#define AMEX_TAG_TRANS_CUR_CODE      "5F2A"
#define AMEX_TAG_PAN_SEQ_NBR         "5F34"
#define AMEX_TAG_TRANS_AMOUNT        "9F02"
#define AMEX_TAG_OTHER_AMOUNT        "9F03"
#define AMEX_TAG_TERM_APP_VER_NUM    "9F09"
#define AMEX_TAG_ISS_APP_DATA        "9F10"
#define AMEX_TAG_TERM_COUNTRY_CODE   "9F1A"
#define AMEX_TAG_IFD_SERIAL_NUM      "9F1E"
#define AMEX_TAG_APP_CRYPTOGRAM      "9F26"
#define AMEX_TAG_CRYPTO_INFO_DATA    "9F27"
#define AMEX_TAG_TERM_CAP            "9F33"
#define AMEX_TAG_CVM                 "9F34"
#define AMEX_TAG_TERM_TYPE           "9F35"
#define AMEX_TAG_ATC                 "9F36"
#define AMEX_TAG_UNPRED_NUMBER       "9F37"
#define AMEX_TAG_TRANS_SEQ_NUM       "9F41"
#define AMEX_TAG_TRANS_CATEG_CODE    "9F53"
#define AMEX_TAG_APP_SEQ_NUMBER      "5F34"
#define AMEX_TAG_ARPC                "9F54"
#define	AMEX_TAG_APP_SELECTED	     "9F06"	
#define	AMEX_TAG_TRACK2	             "5700"
#define	AMEX_TAG_APP_PAN             "5A00"
#define	AMEX_TAG_APP_NAME            "9F12"
#define	AMEX_TAG_ICC                 "8C00"
#define	AMEX_TAG_APP_LABEL           "5000" 

#define AMEX_TAG_ISS_SCRIPTS         "ISSC"

/*-- 60: NATIONAL USE DATA*/
#define	AMEX_F060_PRIM_ID				"AX"
#define	AMEX_F060_PRIM_ID_LEN			2
#define	AMEX_F060_SECOND_ID				"AAD"
#define	AMEX_F060_SECOND_ID_LEN			3
#define	AMEX_F060_SELLER_ID_LEN			20
#define	AMEX_F060_SELLER_EMAIL_VLI_LEN	2
#define	AMEX_F060_SELLER_EMAIL_LEN	    40
#define	AMEX_F060_SELLER_PHONE_LEN		20

/*-- F61: NATIONAL USE DATA*/
/*American Express SafeKey Format*/
#define	AMEX_F061_SK_PRIM_ID				"AX"
#define	AMEX_F061_SK_PRIM_ID_LEN			2
#define	AMEX_F061_SK_SECOND_ID				"ASK"
#define	AMEX_F061_SK_SECOND_ID_LEN			3
#define	AMEX_F061_SK_ECI_LEN				2
#define	AMEX_F061_SK_AEVV_ID				"AEVV"
#define	AMEX_F061_SK_AEVV_ID_LEN			4
#define	AMEX_F061_SK_AEVV_LEN				20
#define	AMEX_F061_SK_XID_ID					"XID"
#define	AMEX_F061_SK_XID_ID_LEN				3
#define	AMEX_F061_SK_XID_LEN				20

/*Request*/
#define AMEX_F061_ECI_REQ_AUTH_WITH_AEVV      	 	  "05"	/*Authenticated with AEVV*/
#define AMEX_F061_ECI_REQ_ATTEMPTED_WITH_AEVV      	  "06"	/*Attempted with AEVV*/
#define AMEX_F061_ECI_REQ_NOT_AUTH      			  "07"	/*Not Authenticated*/

/*Response*/
#define	AMEX_F061_ECI_REP_AEVV_FAILED_AUTH_ISS_KEY					'1' /*AEVV Failed - Authentication, Issuer Key */
#define	AMEX_F061_ECI_REP_AEVV_PASSED_AUTH_ISS_KEY					'2' /*AEVV Passed - Authentication, Issuer Key */
#define	AMEX_F061_ECI_REP_AEVV_PASSED_ATT_ISS_KEY					'3' /*AEVV Passed - Attempt, Issuer Key */
#define AMEX_F061_ECI_REP_AEVV_FAILED_ATT_ISS_KEY					'4' /*AEVV Failed - Attempt, Issuer Key */                                                                     
#define	AMEX_F061_ECI_REP_AEVV_FAILED_ATT_ISS_NOT_PARTICIPATING		'7' /*AEVV Failed - Attempt, Issuer not participating, Network Key */
#define	AMEX_F061_ECI_REP_AEVV_PASSED_ATT_ISS_NOT_PARTICIPATING		'8' /*AEVV Passed - Attempt, Issuer not participating, Network Key  */
#define	AMEX_F061_ECI_REP_AEVV_FAILED_ATT_PART_ACS_NOT_AVAILABLE	'9' /*AEVV Failed - Attempt, Participating, Access Control Server (ACS) not available, Network Key */
#define	AMEX_F061_ECI_REP_AEVV_PASSED_ATT_PART_ACS_NOT_AVAILABLE	'A' /*AEVV Passed - Attempt, Participating, Access Control Server (ACS) not available, Network Key*/                                                                  
#define	AMEX_F061_ECI_REP_AEVV_UNCHECKED							'U' /*AEVV Unchecked */


/*-- F63: PRIVATE USE DATA*/
#define	AMEX_F063_SERVICE_ID		  			  "AX"
#define	AMEX_F063_SERVICE_ID_LEN		  		  2
#define	AMEX_F063_AAV_RTI_ID		  			  "AD"
#define	AMEX_F063_AAV_RTI_ID_LEN		  		  2
#define	AMEX_F063_TNV_EAV_RTI_ID		  		  "AE"		/*Telephone Number Verification - Email Address Verification RTI*/
#define	AMEX_F063_TNV_EAV_RTI_ID_LEN		  	  2
#define	AMEX_F063_CM_BILL_POSTAL_CODE_LEN		  9
#define	AMEX_F063_CM_BILL_ADDRESS_LEN			  20 
#define	AMEX_F063_CM_FIRST_NAME_LEN				  15
#define	AMEX_F063_CM_LAST_NAME_LEN				  30
#define	AMEX_F063_CM_BILL_PHONE_NBR_LEN	          10
#define	AMEX_F063_SHIP_TO_POSTAL_CODE_LEN 		  9
#define	AMEX_F063_SHIP_TO_ADDRESS_LEN			  50
#define	AMEX_F063_SHIP_TO_FIRST_NAME_LEN		  15
#define	AMEX_F063_SHIP_TO_LAST_NAME_LEN			  30
#define	AMEX_F063_SHIP_TO_PHONE_NBR_LEN			  10
#define	AMEX_F063_SHIP_TO_COUNTRY_CODE_LEN        3


#define AMEX_TAG_FIX_HEX       0
#define AMEX_TAG_FIX_BCD       1
#define AMEX_TAG_L_HEX         2
#define AMEX_TAG_LL_HEX        3
#define AMEX_TAG_L_BIN         4


#define MAX_AMEX_CHIP_TAG       100
#define MAX_AMEX_CHIP_LEN       999 

#define MANDATORY                1      
#define DATA_NOT_USED            4    

#define AMEX_REQ_TYPE_PERIODIQUE                 "PERIODIQUE"

#endif /** HPS_AMEX_GCA_DEFINE_H **/
