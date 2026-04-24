#ifndef HPS_MDS_PDSTAG_H
#define HPS_MDS_PDSTAG_H

#define MDS_PDS_DES                      "01"
#define MDS_PDS_FLAGS                    "02"
#define MDS_PDS_VER_NBR                  "03"
#define MDS_PDS_SIGN_OFF_REASON          "04"
#define MDS_PDS_REJ_MSG                  "05"
#define MDS_PDS_REJ_REASON               "06"
#define MDS_PDS_TEST_PROD                "07"
#define MDS_PDS_PINBLOCK_FORMAT          "08"
#define MDS_PDS_ACCEPTANCE_DATA          "21"
#define MDS_PDS_CVM                      "20"
#define MDS_MULTI_PUR_MER_IND      	     "22"	/*IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/
#define MDS_PDS_DEVICE_TYPE_IND          "23"   /*ACH20191219 */
#define MDS_PDS_WALLET_PROG_DATA         "26"   /*ACH20191219 */
#define MDS_PDS_TRX_ANALYSIS             "27"   /*ACH20191219 */
#define MDS_PDS_TKN_TRX_ID               "30"   /*ACH20191219 */
#define MDS_PDS_PAN_MAPPING_INFO         "33"   /*ACH20191219*/
#define MDS_PDS_ADDIT_MERCHANT_DATA      "37"   /*PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	
#define MDS_PDS_CERT_SERIAL_NBR          "40"
#define MDS_PDS_SECURITY_IND             "42"
#define MDS_PDS_UCAF                     "43" /*ZKO 26072022 PLUTONL-4902 ENH 22.Q4 MCI AN 6039*/
#define MDS_PDS_POS_DATA_EXT             "61" /* Mehdi Elyajizi Enh7.2 Purchase with Cash Back */
#define MDS_PDS_TRACEID                  "63"
#define MDS_PDS_AUTHENTICATION_DATA      "66"
#define MDS_PDS_MONEY_SEND_INFO	         "67"
#define MDS_PDS_ON_BEHALF_SERV           "71"
#define MDS_PDS_ADD_PROC_INFO            "74"	/*AMER20210925 PLUTONL-3612*/
#define MDS_PDS_FRAUD_DATA            	 "75"   /*AMER20160306: Enh17.Q2*/
#define MDS_PDS_PAYMENT_TRANS			 "77"
#define MDS_PDS_PIN_SERVICE_CODE         "80"	/*AMER20170228: Enh 17.Q2*/
#define MDS_PDS_AVS_RESPONSE             "83"  /*ACH 20191119*/
#define MDS_PDS_MERCHANT_ADV_CODE        "84"	/*AEA20210712 Enh21.Q4 AN4761*/
#define MDS_PDS_CVC_INVALID              "87"
#define MDS_PDS_CVC_CHANGE               "88"
#define MDS_PDS_CVC_ERROR                "89"
#define MDS_PDS_CVC2                     "92"
#define MDS_PROMOTION_CODE               "95"

/******* PDS des Messages File Update ******/
#define MDS_PDS_MSG_ERR_IND    	"005"
#define MDS_PDS_ENTITY_SELECTOR    "601"
#define MDS_PDS_REGION_LIST_DATA   "602"
#define MDS_PDS_CHARG_EFF_DATE    	"603"
#define MDS_PDS_LAST_UPDATE_DATE   "605"
#define MDS_PDS_SERVICE_ID    		"606"
#define MDS_PDS_LIST_REG    		"608"
/*START NAB30092020 PLUTONL-2303*/
typedef struct t_pds_mds
{
   int  presence;
   char *value;
} t_pds_mds;
/*END NAB30092020 PLUTONL-2303*/

#endif /** HPS_MDS_PDSTAG_H **/
