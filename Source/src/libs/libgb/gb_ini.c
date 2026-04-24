#include <stdio.h>
#include <stdlib.h>
#include <iso_gb.h>
#include <header_gb.h>
#include <define.h>
#include <iso8583.h>

static fld_info_t tab_GBFldInfo[] =
    {
        {NOT_USED,      0},           /* BIT_MAP1			 */
        {NOT_USED,      19},          /* CARD_NBR 			 */
        {NOT_USED,      6},           /* PROC_CODE			 */
        {NOT_USED,      12},          /* TRANS_AMOUNT			 */
        {NOT_USED,      12},          /* STLMNT_AMOUNT		 */
        {NOT_USED,      12},          /* Cardholder Billing Amount	 */
        {GB_FIX_ALPHA,  10},          /* XMIT_TIME			 */ /* Field 7 */
        {NOT_USED,      8},           /* Cardholder Billing Fee	 */
        {NOT_USED,      8},           /* CONVRSN_RATE_STLMNT		 */
        {NOT_USED,      8},           /* CONVRSN_RATE_CARD_HLDR_BILLING */

        {GB_FIX_ALPHA,  12},           /* AUDIT_NBR			 */ /* Field 11 */ /** HMA - 20052013 **/
        {GB_FIX_ALPHA,  14},           /* TRANS_TIME            */ /* Field 12 */
        {NOT_USED,      4},            /* TRANS_DATE			 */
        {NOT_USED,      4},            /* EXPIRY_DATE			 */
        {NOT_USED,      4},            /* STLMNT_DATE			 */
        {NOT_USED,      4},            /* CNVRSN_DATE			 */
        {NOT_USED,      4},            /* CAPTURE_DATE			 */
        {GB_LL_ALPHA,   140},          /* MESSAGE ERROR INDICATOR	 */ /* Field 18 */
        {NOT_USED,      3},            /* ACQR_COUNTRY			 */
        {NOT_USED,      3},            /* PAN_COUNTRY			 */

        {NOT_USED,      3},            /* FORWD_COUNTRY		 */
        {NOT_USED,      3},            /* POS_ENTRY_MODE		 */
        {NOT_USED,      3},            /* Card Sequence Number		 */
        {GB_FIX_ALPHA,  3},            /* Function Code                 */ /* Field 24 */
        {NOT_USED,      2},            /* POS_CONDITION		 */
        {NOT_USED,      2},            /* MERCHANT TYPE  		 */
        {NOT_USED,      1},            /* Authorization Id Response Lenght */
        {NOT_USED,      9},            /* TRANS_FEE			 */
        {NOT_USED,      9},            /* Amount Settlement Fee	 */
        {NOT_USED,      9},            /* Amount Transaction Processing Fee */

        {NOT_USED,      9},            /* Amount Setlmnt Processing Fee */
        {GB_LL_ALPHA_N, 11},           /* ACQR_ID			 */ /* Field 32 */ /** HMA - 21052013 **/
        {NOT_USED,      11},           /* FORWD_ID			 */
        {NOT_USED,      28},           /* PAN_EXTENDED			 */
        {NOT_USED,      47},           /* T2_DATA			 */
        {NOT_USED,      4},            /* T1_DATA                       */
        {NOT_USED,      12},           /* REFERENCE_NBR		 */
        {NOT_USED,      6},            /* AUTHOR_ID			 */
        {GB_FIX_ALPHA,  4},            /* ACTION CODE  		 */ /* Field 39 */
        {NOT_USED,      3},            /* Service Restriction Code	 */

        {GB_FIX_ALPHA,  16},           /* TERMINAL_NBR			 */ /* Field 41 */ /** HMA - 20052013 **/
        {GB_L_ALPHA,    35},           /* OUTLET_NBR			 */ /* Field 42 */ /** HMA - 20052013 **/
        {NOT_USED,      40},           /* TERMINAL_ADR			 */
        {NOT_USED,      25},           /* ADTNL_RESP_DATA		 */
        {NOT_USED,      77},           /* T1_DATA			 */
        {NOT_USED,      255},          /* Additrional Data Cis		 */
        {NOT_USED,      255},          /* Additrional Data National	 */
        {NOT_USED,      999},          /* ADTNL_DATA_PRIV		 */
        {NOT_USED,      3},            /* TRANS_CRNCY			 */
        {NOT_USED,      3},            /* STLMNT_CRNCY			 */

        {NOT_USED,      3},            /* BILLING_CRNCY		 */
        {NOT_USED,      16},           /* PIN_DATA			 */
        {GB_FIX_ALPHA,  16},           /* SECURITY_DATA		 */
        {NOT_USED,      120},          /* ADTNL_AMOUNTS		 */
        {NOT_USED,      255},          /* ICC Information               */
        {NOT_USED,      255},          /* Orig Data Elements            */
        {NOT_USED,      3},            /* Reserved National		 */
        {NOT_USED,      255},          /* Reserved National		 */
        {NOT_USED,      14},           /* GEOGR_DATA			 */
        {NOT_USED,      6},            /* ADTNL_POS_INFO		 */

        {NOT_USED,      36},           /* OTHER_AMOUNTS		 */ /* 61 */
        {NOT_USED,      999},          /* Redemption Number		 */
        {GB_LLLL_ALPHA, 9999},         /* DCC PRIVATE DATA ELEMENTS      	*/ /* Field 63 */
        {NOT_USED,      8},            /* MAC1				 */
};

static fld_info_t tab_GBFld63Info[] =
    {
        {NOT_USED,         0},      /** HMA - 20052013 **/ /*                             */
        {GB_L_ALPHA,       15},     /* Merchant ID   */ /*MERCHANT ID                  */
        {GB_FIX_ALPHA,     16},     /* Terminal ID   */ /*TERMINAL ID                  */
        {GB_LL_ALPHA_N,    11},     /* Acquerer ID   */ /** HMA - 21052013 **/ /*ACQUERER ID                  */
        {NOT_USED,         0},      /*                             */
        {GB_FIX_ALPHA_N,   16},     /** HMA - 20052013 **/ /*FOREIGN CURRENCY INFORMATION */
        {NOT_USED,         0},      /* Card Number  */ /*                             */
        {NOT_USED,         0},      /** HMA - 20052013 **/ /*                             */
        {GB_FIX_ALPHA_N,   10},     /** HMA - 20052013 **/ /*EXCHANGE RATE INFORMATION    */
        {GB_L_ALPHA,       99},     /** HMA - 20052013 **/ /*RATE REQUEST REFERENCE ID    */

        {NOT_USED,         0},      /** HMA - 20052013 **/ /*                             */
        {NOT_USED,         0},      /** HMA - 20052013 **/ /*                             */
        {NOT_USED,         0},      /** HMA - 20052013 **/ /*                             */
        {NOT_USED,         0},      /** HMA - 20052013 **/ /*                             */
        {GB_FIX_ALPHA,     3},      /* Card Type  */ /*CARD TYPE                    */
        {GB_FIX_ALPHA_N,   14},     /** HMA - 20052013 **/ /*UPDATE CALL DATE & TIME      */
        {NOT_USED,         0},      /** HMA - 20052013 **/ /*                             */
        {GB_FIX_ALPHA_N,   11},     /** HMA - 20052013 **/ /*INVERTED RATE INFORMATION    */
        {NOT_USED,         0},      /** HMA - 20052013 **/ /*                             */
        {GB_L_ALPHA,       35},     /** HMA - 20052013 **/ /*FOREIGN CURRENCY MERCHANT ID */

        {GB_FIX_ALPHA,     16},     /** HMA - 20052013 **/ /*FOREIGN CURRENCY TERMINAL ID */
        {GB_FIX_ALPHA_N,   6},      /* Till ID  */ /*TILL ID                      */
        {NOT_USED,         0},      /** HMA - 20052013 **/ /*                             */
        {GB_FIX_ALPHA,     3},      /** HMA - 20052013 **/ /*FOREIGN CURRENCY ALPHABITIC  */
        {NOT_USED,         0},      /** HMA - 20052013 **/ /*                             */
        {GB_FIX_ALPHA,     9},      /** HMA - 20052013 **/ /*MARK UP PURCENTAGE INFO      */
        {GB_FIX_ALPHA,     9},      /** HMA - 20052013 **/ /*COMMISSION PERCENTAGE INFO   */
        {GB_FIX_ALPHA,     16},     /** HMA - 20052013 **/                   /*COMMISSION VALUE INFO        */
        {GB_FIX_ALPHA,     3},       /** HMA - 20052013 **/ /*FOREIGN CURRENCY NUM & RATE  */
};

static fld_info_t tab_GBFld63InInfo[] =
    {
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {GB_L_ALPHA,    15},     /* Merchant ID   */
        {GB_FIX_ALPHA,  16},     /* Terminal ID   */
        {GB_LL_ALPHA_N, 11},     /* Acquerer ID   */
        {GB_FIX_ALPHA,  16},     /* Base Currency Information  */
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {GB_LL_ALPHA_N, 19},     /* Card Number  */
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {NOT_USED,      0},      /** HMA - 20052013 **/

        {NOT_USED,      0},      /** HMA - 20052013 **/
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {GB_FIX_ALPHA,  3},      /* Card Type  */
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {NOT_USED,      0},      /** HMA - 20052013 **/
        {NOT_USED,      0},      /** HMA - 20052013 **/

        {NOT_USED,      0},      /** HMA - 20052013 **/
        {GB_FIX_ALPHA, 6},       /* Till ID  */
};

/*static const TlvInfoProperties	GbAddDataProp = {
2,2,TLV_LT_ASCII,TLV_LT_ASCII,GB_F048_NB_FIELDS,
{
{DCC_FOR_CURR, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,35,PP_PRINT,"NULL","RECIPIENT_FIRST_NAME","NULL",NOT_REPEATABLE},
{DCC_FOR_AMOUNT, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","RECIPIENT_MIDDLE_NAME","NULL",NOT_REPEATABLE},
{DCC_FOR_ALPHA_CODE, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,35,PP_PRINT,"NULL","RECIPIENT_LAST_NAME","NULL",NOT_REPEATABLE},
{DCC_RATE_VALUE, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,50,PP_PRINT,"NULL","RECIPIENT_STREET_ADDR","NULL",NOT_REPEATABLE},
{DCC_INVERT_RATE, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,25,PP_PRINT,"NULL","RECIPIENT_CITY","NULL",NOT_REPEATABLE},
{DCC_MARCKUP_VALUE, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,3,PP_PRINT,"NULL","RECIPIENT_STATE_PROVINCE_CODE","NULL",NOT_REPEATABLE},
{DCC_ORIGINE_AMOUNT, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","RECIPIENT_COUNTRY","NULL",NOT_REPEATABLE},
{DCC_ORIGINE_CURR, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,10,PP_PRINT,"NULL","RECIPIENT_POSTAL_CODE","NULL",NOT_REPEATABLE},
{CARD_TYPE_TAG, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_LLL,LU_BYTE,20,PP_PRINT,"NULL","RECIPIENT_PHONE_NUMBER","NULL",NOT_REPEATABLE},
{DCC_MSG_ERROR, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_FIX,LU_BYTE,8,PP_PRINT,"NULL","RECIPIENT_DOB","NULL",NOT_REPEATABLE},
{DCC_TRX_FLAG, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,50,PP_PRINT,"NULL","RECIPIENT_ACCOUNT_NUMBER","NULL",NOT_REPEATABLE},
{DCC_MSG_NUMBER_TAG, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_LLL,LU_BYTE,2,PP_PRINT,"NULL","RECIPIENT_ACCOUNT_TYPE","NULL",NOT_REPEATABLE},		
{DCC_ORIGINAL_STAN, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_FIX,LU_BYTE,8,PP_PRINT,"NULL","RECIPIENT_DOB","NULL",NOT_REPEATABLE},
{DCC_FOR_CURR_UNIT, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,50,PP_PRINT,"NULL","RECIPIENT_ACCOUNT_NUMBER","NULL",NOT_REPEATABLE},
{DCC_RATE_UNIT, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_LLL,LU_BYTE,2,PP_PRINT,"NULL","RECIPIENT_ACCOUNT_TYPE","NULL",NOT_REPEATABLE},		
{DCC_INVERT_RATE_UNIT, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_FIX,LU_BYTE,8,PP_PRINT,"NULL","RECIPIENT_DOB","NULL",NOT_REPEATABLE},
{DCC_INVERT_DISP_UNIT, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,50,PP_PRINT,"NULL","RECIPIENT_ACCOUNT_NUMBER","NULL",NOT_REPEATABLE},
{DCC_MARCKUP_POINT, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_LLL,LU_BYTE,2,PP_PRINT,"NULL","RECIPIENT_ACCOUNT_TYPE","NULL",NOT_REPEATABLE},		
{DCC_CURRENCY_ALPHA, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_FIX,LU_BYTE,8,PP_PRINT,"NULL","RECIPIENT_DOB","NULL",NOT_REPEATABLE},
},
"GbAddDataProp"
};*/


void InitGBInfo(TSGBInfo *msgInfo)
{
   int i;

   for (i = 0; i < MAX_GB_FIELDS + 1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i = 0; i < GB_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
	memset(msgInfo->sData, '\0', sizeof(msgInfo->sData));
   memset((char *)&(msgInfo->sHeader), '\0', sizeof(TSGBHeader));
}

int GetGBFieldType(int field_no)
{
   return (tab_GBFldInfo[field_no].type);
}

int GetGBFieldLength(int field_no)
{
   return (tab_GBFldInfo[field_no].length);
}

int GetGBField63Type(int field_no)
{
   return (tab_GBFld63Info[field_no].type);
}

int GetGBField63Length(int field_no)
{
   return (tab_GBFld63Info[field_no].length);
}

int GetGBField63InType(int field_no)
{
   return (tab_GBFld63InInfo[field_no].type);
}

int GetGBField63InLength(int field_no)
{
   return (tab_GBFld63InInfo[field_no].length);
}

/******************************************************************************/
/*void InitGbAddDataTlv(TlvInfo* pkTlvInfo)
{
pkTlvInfo->pkProperties = &GbAddDataProp;
	ResetTlvInfo(pkTlvInfo);*/
/******************************************************************************/
