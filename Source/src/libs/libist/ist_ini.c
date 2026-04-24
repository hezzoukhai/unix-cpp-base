#include <stdio.h>
#include <stdlib.h>
#include <iso_ist.h>
#include <define.h>


fld_info_t    tab_IstFldInfo[] =
{
   { IST_FIX_BITMAP,                      0},	/* BIT_MAP2			                  */
   { IST_VAR_ALPHA,	                     19},   /* CARD_NBR 			              */
   { IST_FIX_ALPHA,	                      6},	/* PROC_CODE			              */
   { IST_FIX_ALPHA,	                     12},	/* TRANS_AMOUNT			              */
   { IST_FIX_ALPHA,	                     12},	/* STLMNT_AMOUNT		              */
   { IST_FIX_ALPHA,	                     12},	/* CARDHOLDER BILLING AMOUNT	      */
   { IST_FIX_ALPHA,	                     10},	/* XMIT_TIME			              */
   { IST_FIX_ALPHA,	                      8},	/* CARDHOLDER BILLING FEE	          */
   { IST_FIX_ALPHA,	                      8},	/* CONVRSN_RATE_STLMNT		          */
   { IST_FIX_ALPHA,	                      8},	/* CONVRSN_RATE_CARD_HLDR_BILLING     */ /* 10 */
                                             
   { IST_FIX_ALPHA,	                      6},	/* AUDIT_NBR			              */
   { IST_FIX_ALPHA,	                      6},	/* TRANS_TIME			              */
   { IST_FIX_ALPHA,	                      4},	/* TRANS_DATE			              */
   { IST_FIX_ALPHA,	                      4},	/* EXPIRY_DATE			              */
   { IST_FIX_ALPHA,	                      4},	/* STLMNT_DATE			              */
   { IST_FIX_ALPHA,	                      4},	/* CNVRSN_DATE			              */
   { IST_FIX_ALPHA,	                      4},	/* CAPTURE_DATE			              */
   { IST_FIX_ALPHA,	                      4},	/* MERCHANT_TYPE		              */
   { IST_FIX_ALPHA,	                      3},	/* ACQR_COUNTRY			              */
   { IST_FIX_ALPHA,	                      3},	/* PAN_COUNTRY			              */ /* 20 */
                                             
   { IST_FIX_ALPHA,	                      3},	/* FORWD_COUNTRY		              */
   { IST_FIX_ALPHA,	                      3},	/* POS_ENTRY_MODE		              */
   { IST_FIX_ALPHA,	                      3},	/* CARD SEQUENCE NUMBER		          */
   { IST_FIX_ALPHA,	                      3},	/* NETWORK INTERNATIONAL IDENTIFIER   */
   { IST_FIX_ALPHA,	                      2},	/* POS_CONDITION		              */
   { IST_FIX_ALPHA,	                      2},	/* POS_PIN_CAPTURE		              */
   { IST_FIX_ALPHA,	                      1},	/* AUTHORIZATION ID RESPONSE LENGHT   */
   { IST_FIX_ALPHA,	                      9},	/* TRANS_FEE			              */
   { IST_FIX_ALPHA,	                      9},	/* AMOUNT SETTLEMENT FEE	          */
   { IST_FIX_ALPHA,	                      9},	/* AMOUNT TRANSACTION PROCESSING FEE  */ /* 30 */
	                                              
   { IST_FIX_ALPHA,	                      9},	/* AMOUNT SETLMNT PROCESSING FEE      */
   { IST_VAR_ALPHA,	                     11},	/* ACQR_ID			                  */
   { IST_VAR_ALPHA,	                     11},	/* FORWD_ID			                  */
   { NOT_USED,	                         28},	/* PAN_EXTENDED			              */
   { IST_VAR_ALPHA,	                     37},	/* T2_DATA			                  */
   { IST_VAR_ALPHA,	                    104},	/* TRACK 3 DATA			              */
   { IST_FIX_ALPHA,	                     12},	/* REFERENCE_NBR		              */
   { IST_FIX_ALPHA,	                      6},	/* AUTHOR_ID			              */
   { IST_FIX_ALPHA,	                      2},	/* RESPONSE_CODE		              */
   { IST_FIX_ALPHA,	                      3},	/* SERVICE RESTRICTION CODE	          */ /* 40 */

   { IST_FIX_ALPHA,	                      8},	/* TERMINAL_NBR			              */
   { IST_FIX_ALPHA,	                     15},	/* OUTLET_NBR			              */
   { IST_FIX_ALPHA,	                     40},	/* TERMINAL_ADR			              */
   { IST_VAR_ALPHA,	                     25},	/* ADTNL_RESP_DATA		              */
   { IST_VAR_ALPHA,	                     77},	/* T1_DATA			                  */
   { IST_VAR_ALPHA,	                    255},	/* ADDITRIONAL DATA ISO		          */
   { IST_VAR_HEX,		                255},	/* ADDITRIONAL DATA NATIONAL	      */
   { IST_VAR_ALPHA,	                    255},	/* ADTNL_DATA_PRIV		              */
   { IST_FIX_ALPHA,	                      3},	/* TRANS_CRNCY			              */
   { IST_FIX_ALPHA,	                      3},	/* STLMNT_CRNCY			              */   /* 50 */

   { IST_FIX_ALPHA,	                      3},	/* BILLING_CRNCY		              */
   { IST_FIX_HEX,	                     16},	/* PIN_DATA			                  */
   { IST_FIX_ALPHA,	                     16},	/* SECURITY_DATA		              */
   { IST_LLL_ALPHA,	                    120},	/* ADTNL_AMOUNTS		              */
   { IST_LLL_BIN,		                512},	/* ICC INFORMATION                    */
   { IST_VAR_ALPHA,	                    255},	/* RESERVED ISO			              */
   { IST_FIX_ALPHA,	                      6},	/* RESERVED NATIONAL                  */				
   { IST_VAR_ALPHA,	                    255},	/* RESERVED NATIONAL		          */
   { IST_VAR_ALPHA,	                     14},	/* GEOGR_DATA			              */
   { IST_VAR_SBCD,	                      6},	/* ADTNL_POS_INFO		              */ /* 60 */
	                                       
   { IST_VAR_SBCD,	                     36},	/* OTHER_AMOUNTS		              */
   { IST_VAR_BITMAP,                     59},	/* CPS_FIELDS			              */
   { IST_VAR_BITMAP,                    255},	/* PRIV_USE_FIELDS		              */
   { IST_FIX_BIN,	                      8},	/* MAC1				                  */
   { NOT_USED,	                          0},	/* 				                      */
   { IST_FIX_ALPHA,	                      1},	/* STLMNT_CODE			              */
   { IST_FIX_ALPHA,	                      1},	/* EXTENDED PAYMENT CODE	          */
   { IST_FIX_ALPHA,	                      3},	/* RECEIVER_COUNTRY		              */
   { IST_FIX_ALPHA,	                      3},	/* STLMNT_INST_COUNTRY		          */
   { IST_FIX_ALPHA,	                      3},	/* NW_MNGMT_INFO		              */ /* 70 */

   { IST_FIX_ALPHA,	                      4},	/* MESSAGE NUMBER		              */
   { IST_FIX_ALPHA,	                      4},	/* MESSAGE NUMBER LAST		          */
   { IST_FIX_ALPHA,	                      6},	/* ACTION_DATE			              */
   { IST_FIX_ALPHA,	                     10},	/* CR_NBR			                  */
   { IST_FIX_ALPHA,	                     10},	/* CR_REV_NBR			              */
   { IST_FIX_ALPHA,	                     10},	/* DB_NBR			                  */
   { IST_FIX_ALPHA,	                     10},	/* DB_REV_NBR			              */
   { IST_FIX_ALPHA,	                     10},	/* TRANSFER NUMBER		              */
   { IST_FIX_ALPHA,	                     10},	/* TRANSFER REVERSAL NUMBER	          */
   { IST_FIX_ALPHA,	                     10},	/* INQUIRIES NUMBER		              */ /* 80 */
                                             
   { IST_FIX_ALPHA,	                     10},	/* AUTHORIZATIONS NUMBER	          */
   { IST_FIX_ALPHA,	                     12},	/* CREDIT, PROCESSING FEE AMOUNT      */
   { IST_FIX_ALPHA,	                     12},	/* CREDIT, TRANSACTION FEE AMOUNT     */
   { IST_FIX_ALPHA,	                     12},	/* DEBIT, PROCESSING FEE AMOUNT	      */
   { IST_FIX_ALPHA,	                     12},	/* DEBIT, TRANSACTION FEE AMOUNT      */
   { IST_FIX_ALPHA,	                     16},	/* CR_AMOUNT			              */
   { IST_FIX_ALPHA,	                     16},	/* CR_REV_AMOUNT		              */
   { IST_FIX_ALPHA,	                     16},	/* DB_AMOUNT			              */
   { IST_FIX_ALPHA,	                     16},	/* DB_REV_AMOUNT		              */
   { IST_FIX_ALPHA,	                     42},	/* ORIG_DATA			              */ /* 90 */

   { IST_FIX_ALPHA,	                      1},	/* FILE_UPDATE			              */
   { IST_FIX_ALPHA,	                      2},	/* FILE_SECURITY		              */
   { IST_FIX_ALPHA,	                      5},	/* RESPONSE INDICATOR		          */
   { IST_FIX_ALPHA,	                      7},	/* SERVICE INDICATOR		          */
   { IST_FIX_ALPHA,	                     42},	/* REPLACE_AMOUNTS		              */
   { IST_FIX_BIN,	                      8},	/* MSG_SECURITY			              */
   { IST_FIX_ALPHA,	                     17},	/* NET_AMOUNT			              */
   { NOT_USED,	                          0},	/*      			                  */
   { IST_VAR_ALPHA,	                     11},	/* STLMNT_INSTITN		              */
   { IST_VAR_ALPHA,	                     11},	/* RECEIVER_ID			              */ /* 100 */
                                        
   { IST_VAR_ALPHA,	                     17},	/* FILE_NAME			              */
   { IST_VAR_ALPHA,	                     28},	/* ACCOUNT_ID1			              */
   { IST_VAR_ALPHA,	                     28},	/* ACCOUNT_ID2			              */ /* 103 */
   { IST_VAR_ALPHA,	                    100},	/* TRANSACTION DESCRIPTION	          */
   { NOT_USED,	                          0},	/* RESERVED ISO			              */
   { NOT_USED,	                          0},	/* RESERVED ISO			              */
   { NOT_USED,	                          0},	/* RESERVED ISO			              */
   { NOT_USED,	                          0},	/* RESERVED ISO			              */
   { NOT_USED,	                          0},	/* RESERVED ISO			              */
   { NOT_USED,	                          0},	/* RESERVED ISO			              */
                                          
   { NOT_USED,	                          0},	/* RESERVED ISO			              */
   { NOT_USED,	                          0},	/* RESERVED NATIONAL		          */
   { NOT_USED,	                          0},	/* RESERVED NATIONAL		          */
   { NOT_USED,	                          0},	/* RESERVED NATIONAL		          */
   { IST_VAR_ALPHA,	                     24},	/* ADTNL_TRACE_DATA		              */
   { NOT_USED,	                          0},	/* RESERVED NATIONAL		          */
   { NOT_USED,	                          0},	/* RESERVED NATIONAL		          */
   { NOT_USED,	                          0},	/* INTRA COUNTRY DATA		          */
   { NOT_USED,	                          0},	/* RESERVED NATIONAL		          */
   { IST_VAR_ALPHA,	                      4},	/* ORIGINAL MESSAGE TYPE ID	          */

   { NOT_USED,	                          0},	/* ISSUING INSTITUTION ID	          */
   { IST_VAR_ALPHA,	                    999},	/* REMAINING OPEN TO USE	          */
   { IST_VAR_ALPHA,	                    999},	/* ADR_VERIF_DATA		              */
   { NOT_USED,	                          0},	/* FREE FORM TEXT - JAPAN	          */
   { IST_VAR_ALPHA,	                    255},	/* SUPPORT_INFO			              */
   { IST_VAR_BITMAP,	                255},   /* PRIVATE USE FIELD                  */
   { IST_VAR_ALPHA,	                    255},	/* FILE RECORD ACTION AND DATA	      */
   { IST_FIX_BIN,	                      8},	/* MAC2				                  */
   { NOT_USED,	                          0},	/* FIELD 129			              */
   { IST_FIX_HEX,	                      6},	/* TERMINAL CAPABILITY PROFILE        */ /* 130 */

   { IST_FIX_ALPHA,	                     10},	/* TERMINAL VERIFICATION RESULTS      */
   { IST_FIX_ALPHA,	                      8},	/* UNPREDICTABLE NUMBER               */
   { IST_FIX_ALPHA,	                     16},	/* TERMINAL SERIAL NUMBER             */
   { IST_VAR_BIN,	                     16},	/* DISCRETIONARY DATA                 */
   { IST_VAR_ALPHA,	                     39},	/* ISSUER DISCRETIONARY DATA          */
   { IST_FIX_HEX,	                     16},	/* ISSUER DISCRETIONARY DATA          */
   { IST_FIX_HEX,	                      4},	/* APPLICATION TRANS. COUNTER         */
   { IST_FIX_HEX,	                      4},	/* APPLICATION INTER. PROFILE         */
   { IST_FIX_HEX,	                     20},	/* ARPC + RESPONSE CODE               */
   { NOT_USED,	                          0},	/* FIELD 140			              */

   { NOT_USED,	                          0},	/* FIELD 141			              */
   { IST_VAR_HEX,	                    510},	/* ISSUER SCRIPT                      */
   { IST_VAR_HEX,	                     40},	/* ISSUER SCRIPT RESULTS              */
   { IST_FIX_ALPHA,	                      2},	/* CRYPTOGRAM TRANS TYPE              */
   { IST_FIX_ALPHA,	                      4},	/* TERMINAL COUNTRY CODE              */
   { IST_FIX_ALPHA,	                      6},	/* TERMINAL TRANS  DATE               */
   { IST_FIX_ALPHA,	                     12},	/* CRYPTOGRAM AMOUNT                  */
   { IST_FIX_ALPHA,	                      4},	/* CRYPTOGRAM CURRENCY CODE           */
   { IST_FIX_ALPHA,	                     12},	/* CRYPTOGRAM CASH BACK               */
   { IST_FIX_HEX,	                     16},	/* CARDHOLDER CARD SCHEME CRYPTO.     */

   { NOT_USED,	                          0},	/* FIELD 151			              */
   { NOT_USED,	                          0},	/* FIELD 152			              */
   { NOT_USED,	                          0},	/* FIELD 153			              */
   { NOT_USED,	                          0},	/* FIELD 154			              */
   { NOT_USED,	                          0},	/* FIELD 155			              */
   { NOT_USED,	                          0},	/* FIELD 156			              */
   { NOT_USED,	                          0},	/* FIELD 157			              */
   { NOT_USED,	                          0},	/* FIELD 158			              */
   { NOT_USED,	                          0},	/* FIELD 159			              */
   { NOT_USED,	                          0},	/* FIELD 160			              */
                                                                                      
   { NOT_USED,	                          0},	/* FIELD 161			              */
   { NOT_USED,	                          0},	/* FIELD 162			              */
   { NOT_USED,	                          0},	/* FIELD 163			              */
   { NOT_USED,	                          0},	/* FIELD 164			              */
   { NOT_USED,	                          0},	/* FIELD 165			              */
   { NOT_USED,	                          0},	/* FIELD 166			              */
   { NOT_USED,	                          0},	/* FIELD 167			              */
   { NOT_USED,	                          0},	/* FIELD 168			              */
   { NOT_USED,	                          0},	/* FIELD 169			              */
   { NOT_USED,	                          0},	/* FIELD 170			              */
                                                                                      
   { NOT_USED,	                          0},	/* FIELD 171			              */
   { NOT_USED,	                          0},	/* FIELD 172			              */
   { NOT_USED,	                          0},	/* FIELD 173			              */
   { NOT_USED,	                          0},	/* FIELD 174			              */
   { NOT_USED,	                          0},	/* FIELD 175			              */
   { NOT_USED,	                          0},	/* FIELD 176			              */
   { NOT_USED,	                          0},	/* FIELD 177			              */
   { NOT_USED,	                          0},	/* FIELD 178			              */
   { NOT_USED,	                          0},	/* FIELD 179			              */
   { NOT_USED,	                          0},	/* FIELD 180			              */
                                                                                      
   { NOT_USED,	                          0},	/* FIELD 181			              */
   { NOT_USED,	                          0},	/* FIELD 182			              */
   { NOT_USED,	                          0},	/* FIELD 183			              */
   { NOT_USED,	                          0},	/* FIELD 184			              */
   { NOT_USED,	                          0},	/* FIELD 185			              */
   { NOT_USED,	                          0},	/* FIELD 186			              */
   { NOT_USED,	                          0},	/* FIELD 187			              */
   { NOT_USED,	                          0},	/* FIELD 188			              */
   { NOT_USED,	                          0},	/* FIELD 189			              */
   { NOT_USED,	                          0},	/* FIELD 190			              */
                                                                                      
   { NOT_USED,	                          0},	/* FIELD 191			              */
   { IST_FIX_BIN,	                      8},	/* MESSAGE AUTHENTICATION CODE        */
};
tag_info_t  tab_IstTagInfo[] =
{
   { "7100",IST_TAG_VAR_HEX ,   0},/* ISSUER SCRIPT TEMPLATE 1              */
   { "7200",IST_TAG_VAR_HEX ,   0},/* ISSUER SCRIPT TEMPLATE 2              */
   { "8600",IST_TAG_VAR_BCD ,   0},/* ISSUER SCRIPT COMMAND                 */
   { "8200",IST_TAG_VAR_HEX ,   0},/* APPLICATION INTERCHANGE PROFILE       */
   { "8400",IST_TAG_VAR_BCD ,   0},/* DEDICATED FILE NAME                   */
   { "4F00",IST_TAG_VAR_BCD ,   0},/* ICC APPLICATION ID                    */
   { "8A00",IST_TAG_VAR_HEX ,   0},/* AUTHORIZATION RESPONSE CODE           */
   { "9500",IST_TAG_VAR_HEX ,   0},/* TERMINAL VERIFICATION RESULTS         */
   { "9100",IST_TAG_VAR_HEX ,   0},/* TERMINAL VERIFICATION RESULTS         */
   { "9A00",IST_TAG_VAR_BCD ,   0},/* TRANSACTION DATE                      */
   { "9C00",IST_TAG_VAR_BCD ,   0},/* TRANSACTION TYPE                      */
   { "5F2A",IST_TAG_VAR_BCD ,   0},/* TRANSACTION CURRENCY CODE             */
   { "5F34",IST_TAG_VAR_BCD ,   0},/* PAN SEQUENCE NUMNBER                  */
   { "9F02",IST_TAG_VAR_BCD ,   0},/* TRANSACTION AMOUNT                    */
   { "9F03",IST_TAG_VAR_BCD ,   0},/* OTHER AMOUNT                          */
   { "9F09",IST_TAG_VAR_BCD ,   0},/* TERMINAL APPLICATION VERSION NUMBER   */
   { "9F10",IST_TAG_VAR_HEX ,   0},/*  ISSUER APPLICATION DATA              */
   { "9F1A",IST_TAG_VAR_BCD ,   0},/* TERMINAL COUNTRY CODE                 */
   { "9F1E",IST_TAG_VAR_HEX ,   0},/* INTERFACE DEVICE SERIAL NUMBER        */
   { "9F26",IST_TAG_VAR_HEX ,   0},/* APPLICATION CRYPTOGRAM                */
   { "9F27",IST_TAG_VAR_BCD ,   0},/* CRYPTOGRAM INFORMATION DATA           */
   { "9F33",IST_TAG_VAR_HEX ,   0},/* TERMINAL CAPABILITIES                 */
   { "9F34",IST_TAG_VAR_HEX ,   0},/* CARDHOLDER VERIFICATION METHOD        */
   { "9F35",IST_TAG_VAR_BCD ,   0},/* TERMINAL TYPE                         */
   { "9F36",IST_TAG_VAR_HEX ,   0},/* APPLICATION TRANSACTION COUNTER       */
   { "9F37",IST_TAG_VAR_HEX ,   0},/* UNPREDICTABLE NUMBER                  */
   { "9F41",IST_TAG_VAR_BCD ,   0},/* TRANSACTION SEQUENCE NUMBER           */
   { "9F53",IST_TAG_VAR_BCD ,   0},/* TRANSACTION CATEGORY CODE             */
   { "9100",IST_TAG_VAR_HEX ,   0},/* ISSUER AUTHENTICATION DATA            */
   { "9F54",IST_TAG_VAR_HEX ,   0},/* APPLICATION CRYPTOGRAM  ARPC          */
   { "5A00",IST_TAG_VAR_ALPHA,  0},/* APPLICATION PAN						*/
   { "9F06",IST_TAG_VAR_HEX,    0},/* APPLICATION IDENTIFIER   			    */
   { "9F12",IST_TAG_VAR_ALPHA,  0},/* APPLICATION PREFERED NAME			    */
   { "5700",IST_TAG_VAR_HEX,    0},/* APPLICATION TRACK2                    */
   { "8C00",IST_TAG_VAR_ALPHA , 0},/* SECURITY ATTRIBUTES                   */  
   { "5000",IST_TAG_VAR_ALPHA , 0},/* APPLICATION LABEL                     */

};
void  InitIstInfo(TSIstInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_IST_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < IST_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_IST_DATA_LEN);
   memset(( char *) & (msgInfo->sHeader), '\0', IST_HEADER_LEN);
}

int GetIstFieldType(int field_no)
{
   return(tab_IstFldInfo[field_no].type);
}

int GetIstFieldLength(int field_no)
{
   return(tab_IstFldInfo[field_no].length);
}


int GetIstTagIndice ( char *szTagName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_IstTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_IstTagInfo[nIndice].sTagName, 4) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetIstTagType ( int nIndice )
{
   return ( tab_IstTagInfo[nIndice].nType);
}
int GetIstTagLength ( int nIndice )
{
   return ( tab_IstTagInfo[nIndice].nLength);
}
void  InitIstIcTag  (TSTagIst *msgInfo)
{
   int i;

   for (i = 0; i < MAX_IST_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', MAX_IST_CHIP_LEN);
}
