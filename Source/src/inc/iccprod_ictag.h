/*******************************************************************************/
/*  (c) Hightech Payment Systems 2005                                          */
/*  NAME                                                                       */
/*      iccprod_ictag.h                                                        */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Management emv fields that have tlv format (file card production)   */
/* NOTES                                                                       */
/*                                                                             */
/* INITIAL VERSION                                                             */
/*        Y.LAMRANI   25/01/2005 -         V 2.2.0                             */
/*******************************************************************************/

#ifndef HPS_ICCPROD_ICTAG_H
#define HPS_ICCPROD_ICTAG_H

#define    ICC_TAG_TRACK_2_DATA             "5700"
#define    ICC_TAG_CARDHOLDER_NAME          "5F20"
#define    ICC_TAG_TRACK_1_DATA             "9F1F"
#define    ICC_TAG_APPL_EXP_DATE            "5F24"
#define    ICC_TAG_APPL_PRIM_ACCOUNT        "5A00"
#define    ICC_TAG_APPL_VERSION_NBR         "9F08"
#define    ICC_TAG_APPL_CURR_CODE_1         "9F42"
#define    ICC_TAG_APPL_EFFECT_DATE         "5F25"
#define    ICC_TAG_APPL_PAN_SEQ_NBR         "5F34"
#define    ICC_TAG_APPL_USAGE_CTRL          "9F07"
#define    ICC_TAG_ISS_COUNTRY_CODE         "5F28"
#define    ICC_TAG_ISS_COUNTRY_CODE1        "9F57"
#define    ICC_TAG_APPL_SERVICE_CODE        "5F30"
#define    ICC_TAG_APPL_CURRENCY_EXP        "9F44"
#define    ICC_TAG_DDOL                     "9F49"
#define    ICC_TAG_ISS_CODE_TABLE_IDX       "9F11"
#define    ICC_TAG_LANGUAGE_PREFERENCE      "5F2D"
#define    ICC_TAG_APPLICATION_ID           "4F00"
#define    ICC_TAG_APPLICATION_LABEL        "5000"
#define    ICC_TAG_APPL_PRIORITY_IND        "8700"
#define    ICC_TAG_APPL_PREFERRED_NAME      "9F12"
#define    ICC_TAG_APPL_INTERCHG_PRFIL      "8200"
#define    ICC_TAG_APPL_CURR_CODE_2         "9F51"
#define    ICC_TAG_APPL_DEFAULT_ACTION      "9F52"
#define    ICC_TAG_APPL_TRX_COUNTER         "9F36"
#define    ICC_TAG_CONSEC_TRX_LIM_INT_T     "9F53"
#define    ICC_TAG_CONSEC_TRX_LIM_INT_C     "9F72"
#define    ICC_TAG_CUMUL_TRX_AMT_LIM_TOT    "9F54"
#define    ICC_TAG_CUMUL_TRX_AMT_UPLIM_TOT  "9F5C"
#define    ICC_TAG_GEOGRAPHIC_INDICATOR     "9F55"
/*#define    ICC_TAG_ISSUER_APPL_DATA         "9F10"*/
#define    ICC_TAG_ISSUER_AUTHENT_IND       "9F56"
#define    ICC_TAG_LOWER_CONSEC_OFF_LIM     "9F58"
#define    ICC_TAG_LOWER_OFFLINE_LIM        "9F14"
#define    ICC_TAG_UPPER_CONSEC_OFFLIM      "9F59"
#define    ICC_TAG_UPPER_CONSEC_OFFLIM1     "9F23"
#define    ICC_TAG_LAST_ONLINE_ATC_RGSTR    "9F13"
#define    ICC_TAG_PIN_TRY_COUNTER          "9F17"
#define    ICC_TAG_SPECIFIC_DATA            "FF60"
/**/
#define    ICC_TAG_KEK_KEY                  "FF22"
#define    ICC_TAG_KCV_KEK                  "FF29"
#define    ICC_TAG_APPLICATION_CTRL         "D500"
#define    ICC_TAG_N_DOM_CTRL_FACT_EXP      "CE00"
/**/
#define    ICC_TAG_LOWER_CUM_OFF_TRX_AMT    "CA00"
#define    ICC_TAG_UPPER_CUM_OFF_TRX_AMT    "CB00"
#define    ICC_TAG_CARD_TVR_ACTION_CODE     "C600"
#define    ICC_TAG_CARDISS_ACTCOD_OFLIN     "C400"
#define    ICC_TAG_CARDISS_ACTCOD_ONLIN     "C500"
#define    ICC_TAG_CARDISS_ACTCOD_DECLN     "C300"
#define    ICC_TAG_PEK_KEY                  "FF23"
#define    ICC_TAG_CARD_RISK_MANAG_CDOL1    "8C00"
#define    ICC_TAG_CARD_RISK_MANAG_CDOL2    "8D00"
#define    ICC_TAG_CA_PUBLIC_KEY_INDEX      "8F00"
#define    ICC_TAG_ISS_PUBLIC_KEY_CERTIF    "9000"
#define    ICC_TAG_PUBLIC_KEY_CERTIF        "9F46"
#define    ICC_TAG_ISS_PUBLIC_KEY_EXP       "9F32"
#define    ICC_TAG_PUBLIC_KEY_EXP           "9F47"
#define    ICC_TAG_SIGNED_SAD               "9300"
/**/
#define    ICC_TAG_CHIP_DYNAMIC_DATA        "9F4B"
/**/
#define    ICC_TAG_ISS_PUBLIC_KEY_REM       "9200"
#define    ICC_TAG_PUBLIC_KEY_REM           "9F48"
/**/
#define    ICC_TAG_AFL                      "9400"
/**/
#define    ICC_TAG_STATDATA_AUTF_TAGLIST    "9F4A"
#define    ICC_TAG_CVM_LIST                 "8E00"
#define    ICC_TAG_ISS_ACTCOD_DEFAULT       "9F0D"
#define    ICC_TAG_ISS_ACTCOD_DENIAL        "9F0E"
#define    ICC_TAG_ISS_ACTCOD_ONLINE        "9F0F"
/**/
#define    ICC_TAG_CRYPTOGRAM_DATA          "9F10"
#define    ICC_TAG_PIN_BLOCK                "9F62"
/**/
#define    ICC_TAG_KEY BUFFER               "9F60"

#endif /** HPS_ICCPROD_ICTAG_H **/
