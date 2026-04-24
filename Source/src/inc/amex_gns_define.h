#ifndef HPS_AMEX_GNS_DEFINE_H
#define HPS_AMEX_GNS_DEFINE_H

/*-- Fields length*/
#define AMEX_F003_LEN           				  6

/*-- F3: Processing code list*/
/*1100*/
#define AMEX_F003_CASH_ADVANCE				"104000"    /*Cash advance*/
#define AMEX_F003_AAV_ONLY					"174800"    /*Transaction for Automated Address Verification (AAV) Only*/
#define AMEX_F003_PIN_CHANGE				"960000"    /*Pin change*/


/*-- F44: ADDITIONAL RESPONSE DATA*/
/*CID/4DBC/4CSC response code*/
#define AMEXG_F044_4CSC_NOK                   	  'N'    /*Printed Card Security Code or OTP not matched*/
#define AMEXG_F044_4CSC_NO_CHECK                  'U'    /*Printed Card Security Code or OTP unchecked*/
#define AMEXG_F044_4CSC_OK                        'Y'    /*Printed Card Security Code or OTP matched*/

#define AMEX_TAG_FIX_HEX       0
#define AMEX_TAG_FIX_BCD       1
#define AMEX_TAG_L_HEX         2
#define AMEX_TAG_LL_HEX        3
#define AMEX_TAG_L_BIN         4


#define MAX_AMEX_CHIP_TAG       100
#define MAX_AMEX_CHIP_LEN       999 

#define AMEX_TAG_ISS_SCRIPTS         "ISSC"

#endif /** HPS_AMEX_GNS_DEFINE_H **/
