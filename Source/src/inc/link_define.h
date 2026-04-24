#ifndef HPS_LINK_DEFINE_H
#define HPS_LINK_DEFINE_H





#define	C_LINK_PASSWD			"        "

/* ---- Processing Code ---- */
#define LINK_F003_P1_PURCHASE				"00"	/* Purchase & services  */
#define LINK_F003_P1_WITHDRAWAL				"01"	/* Cash Withdrawal  */
#define LINK_F003_P1_BALANCE_INQ			"31"	/* Balance inquiry  */
#define LINK_F003_P1_PIN_UNBLOCK			"91"	/* PIN Unblock  */
#define LINK_F003_P1_PIN_CHANGE				"90"	/* PIN Change  */
#define LINK_F003_P1_COUNT_BALANCE_INQ      "32"    /* Counter Balance inquiry*/

#define LINK_F003_P2_DEF						"00"	/* DEFAULT         */
#define LINK_F003_P2_SAV						"10"	/* SAV         */
#define LINK_F003_P2_CHK						"20"	/* CHK         */
#define LINK_F003_P2_CRD						"30"	/* CRD         */

#define LINK_F003_P3_DEF						"00"	/* DEFAULT         */
#define LINK_F003_P3_SAV						"10"	/* SAV         */
#define LINK_F003_P3_CHK						"20"	/* CHK        */
#define LINK_F003_P3_CRD						"30"	/* CRD         */
#define LINK_F003_P3_UNIVERSAL					"40"	/* Universal   -> uppercase*/


/* ---------------   LENGTHS   ----------------- */
#define LINK_F003_LEN		6
#define LINK_F003_P1_LEN		2
#define LINK_F003_P2_LEN		2
#define LINK_F003_P3_LEN		2
#define LINK_F142_SE22_LEN	3


#define	LINK_F123_LEN									15
#define LINK_F123_PRODUCT_TYPE 							"001"
#define LINK_F123_PRODUCT_ID 							"002"
#define LINK_F123_PRODUCT_DESCRIPTION 					"003"
#define LINK_F123_NEW_PIN  								"013"
#define LINK_F123_ISSUER_SCRIPT_RESULTS 				"014"
#define LINK_F123_BILATERAL_DISCRETIONARY_DATA 			"018"
#define LINK_F123_FCD_COMMISSION 						"027"
#define LINK_F123_SENDER_PROXY 							"041"
#define LINK_F123_RECIPIENT_PROXY 						"042"
#define LINK_F123_TRANSACTION_REFERENCE 				"043"
#define LINK_F123_SENDER_SORT_CODE 						"044"
#define LINK_F123_SENDER_ACCOUNT 						"045"
#define LINK_F123_PAYMENT_REFERENCE 					"046"
#define LINK_F123_RECEIVER_SORT_CODE 					"053"
#define LINK_F123_RECEIVER_ACCOUNT 						"054"

#define LINK_F125_LEN		38
#define LINK_F125_PIN_KEY		"001" /*AHA20220930 PLUTONL-4980*/
#define LINK_F125_KEY_DATA		"01" /*AHA20220930 PLUTONL-4980*/
#define LINK_F125_CHECK_VALUE	"02" /*AHA20220930 PLUTONL-4980*/
#define LINK_F125_NB_FIELDS		    1/*AHA20220930 PLUTONL-4980*/
#define LINK_F125_001_NB_FIELDS		2/*AHA20220930 PLUTONL-4980*/

#define	LINK_F60_NB_FIELDS			4
#define LINK_F060_SF02_REV_REASON		0
#define LINK_F060_SF03_OTHER_SERV		1
#define LINK_F060_SF04_STATUS_LAST_CHIP	2
#define LINK_F060_SF05_CID				3

#endif
