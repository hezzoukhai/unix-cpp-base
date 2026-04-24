#include <stdio.h>
#include <stdlib.h>
#include <iso_postilion.h>
#include <iso_postilion_f127.h>
#include <iso_postilion_Emv.h>
#include <define.h>


fld_info_t    tab_PostilionFldInfo[] =
{
/** 001 **/   { POSTILION_FIX_BITMAP,      8},   /* BIT_MAP2             */
/** 002 **/   { POSTILION_LL_ALPHA,       19},   /* CARD_NBR                      */
/** 003 **/   { POSTILION_FIX_ALPHA,       6},   /* PROC_CODE                     */
/** 004 **/   { POSTILION_FIX_ALPHA,      12},   /* TRANS_AMOUNT                  */
/** 005 **/   { POSTILION_FIX_ALPHA,      12},   /* STLMNT_AMOUNT                 */   
/** 006 **/   { POSTILION_FIX_ALPHA,       0},   /* Cardholder Billing Amount     */
/** 007 **/   { POSTILION_FIX_ALPHA,      10},   /* XMIT_TIME                     */
/** 008 **/   { NOT_USED      ,            0},   /* Reserved Iso             */
/** 009 **/   { POSTILION_FIX_ALPHA,       8},   /* Conversion rate, settlement   */
/** 010 **/   { NOT_USED      ,            0},   /* Reserved Iso             */

/** 011 **/   { POSTILION_FIX_ALPHA,       6},   /* AUDIT_NBR                     */
/** 012 **/   { POSTILION_FIX_ALPHA,       6},   /* TRANS_DATE_TIME               */
/** 013 **/   { POSTILION_FIX_ALPHA,       4},   /* Date, local transaction  MMDD */
/** 014 **/   { POSTILION_FIX_ALPHA,       4},   /* EXPIRY_DATE                   */
/** 015 **/   { POSTILION_FIX_ALPHA,       4},   /* STLMNT_DATE                   */
/** 016 **/   { POSTILION_FIX_ALPHA,       4},   /* Date, conversion               */
/** 017 **/   { NOT_USED      ,            0},   /* Reserved Iso             */
/** 018 **/   { POSTILION_FIX_ALPHA,       4},   /* MERCHANT_TYPE                 */
/** 019 **/   { NOT_USED      ,            0},   /* Reserved Iso             */
/** 020 **/   { NOT_USED      ,            0},   /* Reserved Iso             */

/** 021 **/   { NOT_USED      ,            0},   /* Reserved Iso             */
/** 022 **/   { POSTILION_FIX_ALPHA,       3},   /* POS_ENTRY_MODE                */
/** 023 **/   { POSTILION_FIX_ALPHA,       3},   /* Card Sequence Number          */
/** 024 **/   { POSTILION_FIX_ALPHA,       3},   /* Function Code                 */
/** 025 **/   { POSTILION_FIX_ALPHA,       2},   /* MESSAGE REASON CODE          */
/** 026 **/   { POSTILION_FIX_ALPHA,       2},   /* Reserved Iso             */
/** 027 **/   { POSTILION_FIX_ALPHA,       1},   /* Reserved Iso             */
/** 028 **/   { POSTILION_FIX_ALPHA,       9},   /* Reserved Iso             */
/** 029 **/   { POSTILION_FIX_ALPHA,       9},   /* Reserved Iso             */
/** 030 **/   { POSTILION_FIX_ALPHA,       9},   /* Reserved Iso             */

/** 031 **/   { POSTILION_FIX_ALPHA,       9},   /* Reserved Iso             */
/** 032 **/   { POSTILION_LL_ALPHA,       11},   /* ACQR_ID                       */
/** 033 **/   { POSTILION_LL_ALPHA,       11},   /* ACQR_ID                       */
/** 034 **/   { NOT_USED      ,            0},   /* Reserved Iso             */
/** 035 **/   { POSTILION_LL_ALPHA,       37},   /* T2_DATA                       */
/** 036 **/   { NOT_USED      ,            0},   /* Reserved Iso             */
/** 037 **/   { POSTILION_FIX_ALPHA,      12},   /* REFERENCE_NBR                 */
/** 038 **/   { POSTILION_FIX_ALPHA,       6},   /* AUTHOR_ID                     */
/** 039 **/   { POSTILION_FIX_ALPHA,       2},   /* ACTION CODE                   */
/** 040 **/   { POSTILION_FIX_ALPHA,       3},   /* Reserved Iso             */

/** 041 **/   { POSTILION_FIX_ALPHA,       8},   /* TERMINAL_NBR                 */
/** 042 **/   { POSTILION_FIX_ALPHA,      15},   /* OUTLET_NBR                   */
/** 043 **/   { POSTILION_FIX_ALPHA,      40},   /* TERMINAL_ADR                 */
/** 044 **/   { POSTILION_LL_ALPHA,       25},   /* ADTNL_RESP_DATA              */
/** 045 **/   { POSTILION_LL_ALPHA      , 76},   /* T1_DATA                      */
/** 046 **/   { NOT_USED               ,   0},   /* POSTILION_FEE_AMOUNT         */
/** 047 **/   { NOT_USED      ,            0},   /* Reserved Iso                 */
/** 048 **/   { POSTILION_LLL_ALPHA,     999},   /* ADTNL_DATA_PRIV              */
/** 049 **/   { POSTILION_FIX_ALPHA,       3},   /* TRANS_CRNCY                  */
/** 050 **/   { POSTILION_FIX_ALPHA,       3},   /* STLMNT_CRNCY                 */

/** 051 **/   { NOT_USED  ,               0},    /* Reserved Iso                 */
/** 052 **/   { POSTILION_FIX_HEX,        16},   /* PIN_DATA                     */
/** 053 **/   { POSTILION_FIX_HEX,        96},   /* SECURITY_DATA                */
/** 054 **/   { POSTILION_LLL_ALPHA,     120},   /* Reserved Iso                 */
/** 055 **/   { NOT_USED      ,            0},   /* Reserved Iso                 */
/** 056 **/   { POSTILION_LLL_ALPHA,      99},   /* INITAIL_INFO_ELEMENTS        */
/** 057 **/   { POSTILION_LLL_ALPHA,      99},   /* Reserved Iso                 */
/** 058 **/   { POSTILION_LLL_ALPHA,      99},   /* Reserved Iso                 */
/** 059 **/   { POSTILION_LLL_ALPHA,      99},   /* Reserved Iso                 */
/** 060 **/   { NOT_USED      ,            0},   /* Reserved Iso                 */

/** 061 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 062 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 063 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 064 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 065 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 066 **/   { POSTILION_FIX_ALPHA,       1},    /* Reserved Iso                */
/** 067 **/   { POSTILION_FIX_ALPHA,       2},    /* Reserved Iso                */
/** 068 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 069 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 070 **/   { POSTILION_FIX_ALPHA,       3},    /* Reserved Iso                */

/** 071 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 072 **/   { NOT_USED      ,            0},    /* DATA_RECORDS                */
/** 073 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 074 **/   { POSTILION_FIX_ALPHA,      10},    /* TRS_NBRE_BILL_PAYEMENT      */
/** 075 **/   { POSTILION_FIX_ALPHA,      10},    /* TRS_BACK_NBRE_BILL_PAYEMENT */
/** 076 **/   { POSTILION_FIX_ALPHA,      10},    /* TRS_NBRE_PRE_PAYEMENT       */
/** 077 **/   { POSTILION_FIX_ALPHA,      10},    /* TRS_BACK_NBRE_PRE_PAYEMENT  */
/** 078 **/   { POSTILION_FIX_ALPHA,      10},    /* Reserved Iso                */
/** 079 **/   { POSTILION_FIX_ALPHA,      10},    /* Reserved Iso                */
/** 080 **/   { POSTILION_FIX_ALPHA,      10},    /* Reserved Iso                */

/** 081 **/   { POSTILION_FIX_ALPHA,      10},    /* Reserved Iso                */
/** 082 **/   { POSTILION_FIX_ALPHA,      12},    /* Reserved Iso                */
/** 083 **/   { POSTILION_FIX_ALPHA,      12},    /* TRS_NBRE_DEPOSIT            */
/** 084 **/   { POSTILION_FIX_ALPHA,      12},    /* TRS_BACK_NBRE_DEPOSIT       */
/** 085 **/   { POSTILION_FIX_ALPHA,      12},    /* Reserved Iso                */
/** 086 **/   { POSTILION_FIX_ALPHA,      16},    /* TRANSACTIONS_AMOUNT         */
/** 087 **/   { POSTILION_FIX_ALPHA,      16},    /* TRANSACTIONS_BACK_AMOUNT    */
/** 088 **/   { POSTILION_FIX_ALPHA,      16},    /* Reserved Iso                */
/** 089 **/   { POSTILION_FIX_ALPHA,      16},    /* Reserved Iso                */
/** 090 **/   { POSTILION_FIX_ALPHA,      42},    /* Reserved Iso                */

/** 091 **/   { POSTILION_FIX_ALPHA,       1},    /* Reserved Iso                */
/** 092 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 093 **/   { NOT_USED      ,            0},    /* INITIATOR_ORGANISM          */
/** 094 **/   { NOT_USED      ,            0},    /* DESTINATIION_ORGANISM       */
/** 095 **/   { POSTILION_FIX_ALPHA,      42},    /* Reserved Iso                */
/** 096 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 097 **/   { POSTILION_FIX_ALPHA,      17},    /* RECONCILIATION_AMOUNT       */
/** 098 **/   { POSTILION_FIX_ALPHA,      25},    /* Reserved Iso                */
/** 099 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 100 **/   { POSTILION_LL_ALPHA,       11},    /* Reserved Iso                */

/** 101 **/   { POSTILION_LL_ALPHA,       17},    /* Reserved Iso                */
/** 102 **/   { POSTILION_LL_ALPHA,       28},    /* ACCOUNT_ID1                 */
/** 103 **/   { POSTILION_LL_ALPHA,       28},    /* ACCOUNT_ID1                 */
/** 104 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 105 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 106 **/   { POSTILION_LLL_ALPHA,     999},    /* Reserved Iso                */
/** 107 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 108 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 109 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 110 **/   { NOT_USED      ,            0},    /* Reserved Iso                */

/** 111 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 112 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 113 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 114 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 115 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 116 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 117 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 118 **/   { POSTILION_LLL_ALPHA,     999},    /* Reserved Iso                */
/** 119 **/   { POSTILION_LLL_ALPHA,     999},    /* Reserved Iso                */
/** 120 **/   { NOT_USED      ,            0},    /* Reserved Iso                */

/** 121 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 122 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 123 **/   { POSTILION_LLL_ALPHA,      99},    /* Reserved Iso                */
/** 124 **/   { POSTILION_LLL_HEX   ,     40},    /* Reserved Iso                */
/** 125 **/   { POSTILION_LLL_ALPHA,      99},    /* Reserved Iso                */
/** 126 **/   { NOT_USED      ,            0},    /* Reserved Iso                */
/** 127 **/   { POSTILION_LLLLLL_ALPHA,	 99999},	/* POSTILION_PRIVATE_FIELD ABM 07022022 Add Structured Data field*/
/** 128 **/   { POSTILION_FIX_HEX ,       16},    /* MAC Extended                */
};


fld_info_t    tab_PostilionF127FldInfo[] =
{
/** 001 **/   { NOT_USED                 ,      0},   /* Not Used                      */
/** 002 **/   { POSTILION_F127_LL_ALPHA  ,     32},   /* Switch Key                    */
/** 003 **/   { POSTILION_F127_FIX_ALPHA,      48},   /* Routing Information           */
/** 004 **/   { POSTILION_F127_FIX_ALPHA,      22},   /* POS Data                      */
/** 005 **/   { POSTILION_F127_FIX_ALPHA,      73},   /* Service Station Data          */
/** 006 **/   { POSTILION_F127_FIX_ALPHA,       2},   /* Authorization Profile         */
/** 007 **/   { POSTILION_F127_LL_ALPHA,       55},   /* Check Data                    */
/** 008 **/   { POSTILION_F127_LLL_ALPHA,     128},   /* Retention Data                */
/** 009 **/   { POSTILION_F127_LLL_ALPHA,     999},   /* Additional Node Data          */
/** 010 **/   { POSTILION_F127_FIX_ALPHA,       3},   /* CVV2                          */
/** 011 **/   { POSTILION_F127_LL_ALPHA,       32},   /* Switch Key                    */
/** 012 **/   { POSTILION_F127_LL_ALPHA,       25},   /* Terminal Owner                */
/** 013 **/   { POSTILION_F127_FIX_ALPHA,      17},   /* POS Geographic Data           */
/** 014 **/   { POSTILION_F127_FIX_ALPHA,       8},   /* Sponsor Bank                  */
/** 015 **/   { POSTILION_F127_LL_ALPHA,       29},   /* Address Verification Data     */
/** 016 **/   { POSTILION_F127_FIX_ALPHA,       1},   /* Address Verification Result   */
/** 017 **/   { POSTILION_F127_LL_ALPHA,       50},   /* Cardholder Information        */
/** 018 **/   { POSTILION_F127_LL_ALPHA,       50},   /* Validation Data               */
/** 019 **/   { POSTILION_F127_FIX_ALPHA,      45},   /* Payee Bank Account            */
/** 020 **/   { POSTILION_F127_FIX_ALPHA,       8},   /* Authorizer Date Settlement    */
/** 021 **/   { POSTILION_F127_LL_ALPHA,       12},   /* Record Identification         */
/** 022 **/   { POSTILION_F127_LLLLL_ALPHA, 99999},   /* Structured Data               */ 
/** 023 **/   { POSTILION_F127_FIX_ALPHA,     253},   /* Payee Name and Address        */
/** 024 **/   { POSTILION_F127_LL_ALPHA,       24},   /* Payer Account                 */
/** 025 **/   { POSTILION_F127_LLLL_ALPHA,   8000},   /* ICC Data                      */
/** 026 **/   { POSTILION_F127_LL_ALPHA,       20},   /* Original Node                 */
/** 027 **/   { POSTILION_F127_FIX_ALPHA,       1},   /* Card Verification Result      */
/** 028 **/   { POSTILION_F127_FIX_ALPHA,       4},   /* Amex Card Identification (CID)*/
/** 029 **/   { POSTILION_F127_FIX_BIN,        40},   /* 3D Secure Data                */
/** 030 **/   { POSTILION_F127_FIX_ALPHA,       1},   /* 3D Secure Result              */
/** 031 **/   { POSTILION_F127_LL_ALPHA ,      11},   /* Issuer Network ID             */
/** 032 **/   { POSTILION_F127_LL_BIN,         33},   /* UCAF data                     */
};

fld_info_t    tab_PostilionEmvFldInfo[] =
{
/** 001 **/   { NOT_USED,                         0},   /* Not Used                      */
/** 002 **/   { POSTILION_EMV_FIX_ALPHA,          12},   /* AMOUNT_AUTHORIZED                      */
/** 003 **/   { POSTILION_EMV_FIX_ALPHA,          12},   /* AMOUNT_OTHER           */
/** 004 **/   { POSTILION_EMV_LL_ALPHA,           32},   /* APPLICATION_IDENTIFIER                      */
/** 005 **/   { POSTILION_EMV_FIX_ALPHA,          4},   /* APPLICATION_INTERCHANGE_PROFILE          */
/** 006 **/   { POSTILION_EMV_FIX_ALPHA,          4},   /* APPLICATION_TRANSACTION_COUNTER         */
/** 007 **/   { POSTILION_EMV_FIX_ALPHA,          4},   /* APPLICATION_USAGE_CONTROL                    */
/** 008 **/   { POSTILION_EMV_FIX_ALPHA,          2},   /* AUTHORIZATION_RESPONSE_CODE                */
/** 009 **/   { POSTILION_EMV_FIX_ALPHA,          1},   /* CARD_AUTHENTICATION_RELIABILITY_INDICATOR          */
/** 010 **/   { POSTILION_EMV_FIX_ALPHA,          1},   /* CARD_AUTHENTICATION_RESULTS_CODE                          */
/** 011 **/   { POSTILION_EMV_FIX_ALPHA,          1},   /* CHIP_CONDITION_CODE                      */
/** 012 **/   { POSTILION_EMV_FIX_ALPHA,          16},   /* TCRYPTOGRAM                */
/** 013 **/   { POSTILION_EMV_FIX_ALPHA,          2},   /* CRYPTOGRAM_INFORMATION_DATA           */
/** 014 **/   { POSTILION_EMV_LLL_ALPHA,          504},   /* CVM_LIST                  */
/** 015 **/   { POSTILION_EMV_FIX_ALPHA,          6},   /* CVM_RESULTS     */
/** 016 **/   { POSTILION_EMV_FIX_ALPHA,          8},   /* INTERFACE_DEVICE_SERIAL_NUMBER   */
/** 017 **/   { POSTILION_EMV_FIX_ALPHA,          11},   /* ISSUER_ACTION_CODE        */
/** 018 **/   { POSTILION_EMV_LL_ALPHA,           64},   /* ISSUER_APPLICATION_DATA               */
/** 019 **/   { POSTILION_EMV_LLLL_ALPHA,         507},   /* ISSUER_SCRIPT_RESULTS            */
/** 020 **/   { POSTILION_EMV_FIX_ALPHA,          4},   /* TERMINAL_APPLICATION_VERSION_NUMBER    */
/** 021 **/   { POSTILION_EMV_FIX_ALPHA,          6},   /* TERMINAL_CAPABILITIES         */
/** 022 **/   { POSTILION_EMV_FIX_ALPHA,          3},   /* TERMINAL_COUNTRY_CODE               */ 
/** 023 **/   { POSTILION_EMV_FIX_ALPHA,          2},   /* TERMINAL_TYPE        */
/** 024 **/   { POSTILION_EMV_FIX_ALPHA,          10},   /* TERMINAL_VERIFICATION_RESULT                 */
/** 025 **/   { POSTILION_EMV_FIX_ALPHA,          1},   /* TRANSACTION_CATEGORY_CODE                      */
/** 026 **/   { POSTILION_EMV_FIX_ALPHA,          3},   /* TRANSACTION_CURRENCY_CODE                 */
/** 027 **/   { POSTILION_EMV_FIX_ALPHA,          6},   /* TRANSACTION_DATE      */
/** 028 **/   { POSTILION_EMV_L_ALPHA,            8},   /* TRANSACTION_SEQUENCE_COUNTER*/
/** 029 **/   { POSTILION_EMV_FIX_ALPHA,          2},   /* TRANSACTION_TYPE                */
/** 030 **/   { POSTILION_EMV_FIX_ALPHA,          8},   /* UNPREDICTABLE_NUMBER              */
/** 031 **/   { POSTILION_EMV_LL_ALPHA,          32},   /* ISSUER_AUTHENTICATION_DATA*/
/** 032 **/   { POSTILION_EMV_LLLL_ALPHA,         3354},   /* ISSUER_SCRIPT_TEMPLATE_1*/
/** 033 **/   { POSTILION_EMV_LLLL_ALPHA,         3354},   /* ISSUER_SCRIPT_TEMPLATE_2*/
};

void  InitPostilionInfo(TSPostilionInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_POSTILION_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < POSTILION_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_POSTILION_DATA_LEN);
   memset(msgInfo->sHeader, '\0', sizeof(msgInfo->sHeader));
}

int GetPostilionFieldType(int field_no)
{
   return(tab_PostilionFldInfo[field_no].type);
}

int GetPostilionFieldLength(int field_no)
{
   return(tab_PostilionFldInfo[field_no].length);
}

/** *********************** **/
/** Postilion Field 127 Mng **/
/** *********************** **/
void  InitPostilionF127Info(TSPostilionF127Info *msgInfo)
{
   int i;

   for (i=0; i < MAX_POSTILION_F127_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < POSTILION_F127_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_POSTILION_F127_DATA_LEN);
   memset(msgInfo->sHeader, '\0', sizeof(msgInfo->sHeader));
}

int GetPostilionF127FieldType(int field_no)
{
   return(tab_PostilionF127FldInfo[field_no].type);
}

int GetPostilionF127FieldLength(int field_no)
{
   return(tab_PostilionF127FldInfo[field_no].length);
}
/** *********************** **/

/** *********************** **/
/** Postilion EMV Field Mng **/
/** *********************** **/
void  InitPostilionEmvInfo(TSPostilionEmvInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_POSTILION_EMV_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < POSTILION_EMV_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_POSTILION_EMV_DATA_LEN);
   memset(msgInfo->sHeader, '\0', sizeof(msgInfo->sHeader));
}

int GetPostilionEmvFieldType(int field_no)
{
   return(tab_PostilionEmvFldInfo[field_no].type);
}

int GetPostilionEmvFieldLength(int field_no)
{
   return(tab_PostilionEmvFldInfo[field_no].length);
}
/** *********************** **/
