#ifndef HPS_JCC_ICTAG_H
#define HPS_JCC_ICTAG_H

#define    JCB_TAG_ISS_SCRIPT1         "7100" 
#define    JCB_TAG_ISS_SCRIPT2         "7200"
#define    JCB_TAG_ISS_SCRIPT_CMD      "8600"
#define    JCB_TAG_APP_INTER_PROFILE   "8200"
#define    JCB_TAG_DED_FILE_NAME       "8400"
#define    JCB_TAG_ICC_APP_ID          "4F00"
#define    JCB_TAG_TERM_CAP_PROFILE    "9F33"
#define    JCB_TAG_AUTHOR_RESP_CODE    "8A00"
#define    JCB_TAG_ISS_AUTHEN_DATA     "9100"
#define    JCB_TAG_TVR                 "9500"
#define    JCB_TAG_TRANS_DATE          "9A00"
#define    JCB_TAG_TRANS_TYPE          "9C00"
#define    JCB_TAG_TRANS_CUR_CODE      "5F2A"
#define    JCB_TAG_TRANS_AMOUNT        "9F02"
#define    JCB_TAG_OTHER_AMOUNT        "9F03"
#define    JCB_TAG_TERM_APP_VER_NUM    "9F09"
#define    JCB_TAG_ISS_APP_DATA        "9F10"
#define    JCB_TAG_TERM_COUNTRY_CODE   "9F1A"
#define    JCB_TAG_IFD_SERIAL_NUM      "9F1E"
#define    JCB_TAG_APP_CRYPTOGRAM      "9F26"
#define    JCB_TAG_CRYPTO_INFO_DATA    "9F27"
#define    JCB_TAG_TERM_CAP            "9F33"
#define    JCB_TAG_CVM                 "9F34"
#define    JCB_TAG_TERM_TYPE           "9F35"
#define    JCB_TAG_ATC                 "9F36"
#define    JCB_TAG_UNPRED_NUMBER       "9F37"
#define    JCB_TAG_TRANS_SEQ_NUM       "9F41"
#define    JCB_TAG_TRANS_CATEG_CODE    "9F53"
#define    JCB_TAG_DED_FILE_NAME       "8400"
#define    JCB_TAG_SEC_PIN_DATA        "C000"
#define    JCB_TAG_FFI                 "9F6E"



/*Start MUW 26082021*/

#define MANDATORY                     1
#define CONDITIONAL                   2
#define OPTIONAL                      3
#define DATA_NOT_USED                 4


typedef struct s_jcb_tag
{
   char *sTagName;
   char *sTagDesc;
   int  nPresenceInd;
} jcb_tag_t;

jcb_tag_t  tjcb_tag_100[] = 
{
   { JCB_TAG_APP_CRYPTOGRAM    ,"JCB_TAG_APP_CRYPTOGRAM    ", MANDATORY    },/* Application cryptogram          */
   { JCB_TAG_CRYPTO_INFO_DATA  ,"JCB_TAG_CRYPTO_INFO_DATA  ", MANDATORY    },/* Issuer Application Data         */
   { JCB_TAG_ISS_APP_DATA      ,"JCB_TAG_ISS_APP_DATA      ", MANDATORY    },/* Issuer Application Data (IAD)   */
   { JCB_TAG_UNPRED_NUMBER     ,"JCB_TAG_UNPRED_NUMBER     ", MANDATORY    },/* Unpredictable Number            */
   { JCB_TAG_ATC               ,"JCB_TAG_ATC               ", MANDATORY    },/* Application Transaction Counter */
   { JCB_TAG_TVR               ,"JCB_TAG_TVR               ", MANDATORY    },/* Terminal Verification Results   */
   { JCB_TAG_TRANS_DATE        ,"JCB_TAG_TRANS_DATE        ", MANDATORY    },/* Transaction Date                */
   { JCB_TAG_TRANS_TYPE        ,"JCB_TAG_TRANS_TYPE        ", MANDATORY    },/* Transaction TYPE                */
   { JCB_TAG_TRANS_AMOUNT      ,"JCB_TAG_TRANS_AMOUNT      ", MANDATORY    },/* Transaction Amount              */
   { JCB_TAG_TRANS_CUR_CODE    ,"JCB_TAG_TRANS_CUR_CODE    ", MANDATORY    },/* Transaction Currency Code       */
   { JCB_TAG_APP_INTER_PROFILE ,"JCB_TAG_APP_INTER_PROFILE ", MANDATORY    },/* Application Interchange Profile */
   { JCB_TAG_TERM_COUNTRY_CODE ,"JCB_TAG_TERM_COUNTRY_CODE ", MANDATORY    },/* Terminal Country Code           */
   { JCB_TAG_OTHER_AMOUNT      ,"JCB_TAG_OTHER_AMOUNT      ", MANDATORY    },/* Other Amount                    */
   { JCB_TAG_CVM               ,"JCB_TAG_CVM               ", CONDITIONAL  },/* Cardholder Verification Method  */
   { JCB_TAG_TERM_TYPE         ,"JCB_TAG_TERM_TYPE         ", CONDITIONAL  },/* Terminal Type                   */
   { JCB_TAG_TERM_APP_VER_NUM  ,"JCB_TAG_TERM_APP_VER_NUM  ", CONDITIONAL  },/* Terminal App. Version Number    */
   { JCB_TAG_TERM_CAP          ,"JCB_TAG_TERM_CAP          ", DATA_NOT_USED  },  /* JG NA Terminal Capabilities           */
   { JCB_TAG_IFD_SERIAL_NUM    ,"JCB_TAG_IFD_SERIAL_NUM    ", CONDITIONAL  },/* Interface Device Serial Number  */
   { JCB_TAG_ICC_APP_ID        ,"JCB_TAG_ICC_APP_ID        ", CONDITIONAL  },/* ICC Application ID              */
   { JCB_TAG_TRANS_SEQ_NUM     ,"JCB_TAG_TRANS_SEQ_NUM     ", CONDITIONAL  },/* Transaction Sequence Counter    */
   { JCB_TAG_DED_FILE_NAME     ,"JCB_TAG_DED_FILE_NAME     ", CONDITIONAL  },/* Dedicated File Name             */
};

/*End MUW 26082021*/

#endif /** HPS_JCC_ICTAG_H **/
