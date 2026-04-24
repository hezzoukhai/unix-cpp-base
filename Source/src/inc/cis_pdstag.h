#ifndef HPS_CIS_PDSTAG_H
#define HPS_CIS_PDSTAG_H

#define CIS_PDS_DES                      "01"
#define CIS_PDS_FLAGS                    "02"
#define CIS_PDS_VER_NBR                  "03"
#define CIS_PDS_SIGN_OFF_REASON          "04"
#define CIS_PDS_REJ_MSG                  "05"
#define CIS_PDS_REJ_REASON               "06"
#define CIS_PDS_TEST_PROD                "07"
#define CIS_PDS_PINBLOCK_FORMAT          "08"
#define CIS_PDS_KEY_EXT_DATA             "11"
#define CIS_PDS_ADV_DATE_TIME            "15"  /*NAB22072020 PLUTONL-2141*/
#define CIS_PDS_CVM                      "20"
#define CIS_PDS_ACCEPTANCE_DATA          "21"
#define CIS_MULTI_PUR_MER_IND      	     "22"	/*IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/
#define CIS_PDS_DEVICE_TYPE_IND          "23"
#define CIS_PDS_WALLET_PROG_DATA         "26"   /*ACH20190910 PROD00070986*/
#define CIS_PDS_TRX_ANALYSIS             "27"   /*ACH20190910 PROD00070986*/
#define CIS_PDS_TKN_TRX_ID               "30"   /*ACH20190910 PROD00070986*/
#define CIS_PDS_MCI_ASSIGNED_ID          "32"
#define CIS_PDS_PAN_MAPPING_INFO         "33"
#define CIS_PDS_MVV                      "36"   /*SBE17112016 PROD00035095*/
#define CIS_PDS_ADDIT_MERCHANT_DATA      "37"   /* AHA20161214  PROD00036000*/
#define CIS_PDS_NBR                      "38"
#define CIS_PDS_CERT_SERIAL_NBR          "40"
#define CIS_PDS_PRODUCT_ID		 "46"
#define CIS_PDS_SECURITY_IND             "42"
#define CIS_PDS_UCAF                     "43"
#define CIS_PDS_SECURITY_SERV_DATA       "56"   /*AMER20191023 PROD00072515: MC Enh19.Q2 AN2122*/
#define CIS_PDS_SECURITY_SERV_ADD_DATA_ACQ       "57"   /*PLUTONL-5548   AN6773  Enh 2023Q2 AHA20221221*/ /*Security Services Additional Data for Acquirers*/
#define CIS_PDS_POS_DATA_EXT             "61"
#define CIS_PDS_TRACEID                  "63"
#define CIS_PDS_AUTHENTICATION_DATA      "66"
#define CIS_PDS_MONEY_SEND_INFO		 "67"
#define CIS_PDS_ON_BEHALF_SERV           "71"
#define CIS_PDS_ADD_PROC_INFO		 "74"
#define CIS_PDS_FRAUD_SCORING            "75"	/*AMER20160306: Enh17.Q2*/	
#define CIS_PDS_MC_ELEC_ACCPT_IND        "76"
#define CIS_PDS_PAYMENT_TRANS            "77"
#define CIS_PDS_PAYMENT_SERVICE_IND      "78"/* DCC ICH See PROD00039494*/
#define CIS_PDS_PIN_SERVICE_CODE         "80"
#define CIS_PDS_AVS                      "82"
#define CIS_PDS_AVS_RESPONSE             "83"
#define CIS_PDS_MERCHANT_ADV_CODE        "84"	/*AMER20211026 Enh21.Q4 AN4761*/
#define CIS_PDS_VISA_RESPONSE            "84"
#define CIS_PDS_CVC_RESULT		 "87"
#define CIS_PDS_CVC_CHANGE               "88"
#define CIS_PDS_CVC_ERROR                "89"
#define CIS_PDS_CPS_ACI			 "90"
#define CIS_PDS_CPS_TRANS_ID		 "91"
#define CIS_PDS_CVC2                     "92"
#define CIS_PROMOTION_CODE               "95"
#define CIS_PDS_MARKET_SPEC_DATA	 "96"
/*START NAB30092020 PLUTONL-2303*/
typedef struct t_pds_cis
{
   int  presence;
   char *value;
} t_pds_cis;
/*END NAB30092020 PLUTONL-2303*/
#endif /** HPS_CIS_PDSTAG_H **/
