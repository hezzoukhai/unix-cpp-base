#include <stdio.h>
#include <stdlib.h>
#include <iso_sid.h>
#include <define.h>


static fld_info_t    tab_SidFldInfo[] =
{
   { NOT_USED,  	  0},	/* BIT_MAP1			 */
   { SID_LL_ALPHA,	 19},   /* CARD_NBR 			 */
   { SID_FIX_ALPHA,	  6},	/* PROC_CODE			 */
   { SID_FIX_ALPHA,	 12},	/* TRANS_AMOUNT			 */
   { SID_FIX_ALPHA,	 12},	/* STLMNT_AMOUNT		 */
   { SID_FIX_ALPHA,	 12},	/* Cardholder Billing Amount	 */
   { SID_FIX_ALPHA,	 10},	/* XMIT_TIME			 */
   { SID_FIX_ALPHA,	  8},	/* Cardholder Billing Fee	 */
   { SID_FIX_ALPHA,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { SID_FIX_ALPHA,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { SID_FIX_ALPHA,	  6},	/* AUDIT_NBR			 */
   { SID_FIX_ALPHA,	 12},	/* TRANS_DATE_TIME               */
   { SID_FIX_ALPHA,	  4},	/* START_EXPIRY_DATE	 */
   { SID_FIX_ALPHA,	  4},	/* EXPIRY_DATE			 */
   { SID_FIX_ALPHA,	  6},	/* STLMNT_DATE			 */
   { SID_FIX_ALPHA,	  4},	/* CNVRSN_DATE			 */
   { SID_FIX_ALPHA,	  4},	/* CAPTURE_DATE			 */
   { SID_FIX_ALPHA,	  4},	/* MERCHANT_TYPE		 */
   { SID_FIX_ALPHA,	  3},	/* ACQR_COUNTRY			 */
   { SID_FIX_ALPHA,	  3},	/* PAN_COUNTRY			 */

   { SID_FIX_ALPHA,	  3},	/* FORWD_COUNTRY		 */
   { SID_FIX_ALPHA,	 12},	/* POS_ENTRY_MODE		 */
   { SID_FIX_ALPHA,	  3},	/* Card Sequence Number		 */
   { SID_FIX_ALPHA,	  3},	/* Function Code                 */
   { SID_FIX_ALPHA,	  4},	/* POS_CONDITION		 */
   { SID_FIX_ALPHA,	  4},	/* MERCHANT TYPE  		 */
   { SID_FIX_ALPHA,	  1},	/* Authorization Id Response Lenght */
   { SID_FIX_ALPHA,	  6},	/* TRANS_FEE			 */
   { SID_FIX_ALPHA,	  3},	/* Amount Settlement Fee	 */
   { SID_FIX_ALPHA,	 24},	/* Amount Transaction Processing Fee */
	
   { SID_LL_ALPHA,	  9},	/* Amount Setlmnt Processing Fee */
   { SID_LL_ALPHA,	 11},	/* ACQR_ID			 */
   { SID_LL_ALPHA,	 11},	/* FORWD_ID			 */
   { SID_LL_ALPHA,	 28},	/* PAN_EXTENDED			 */
   { SID_LL_ALPHA,	 37},	/* T2_DATA			 */
   { SID_LLL_ALPHA,	  4},	/* T1_DATA                       */
   { SID_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { SID_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { SID_FIX_ALPHA,	  3},	/* ACTION CODE  		 */
   { SID_FIX_ALPHA,	  3},	/* Service Restriction Code	 */

   { SID_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { SID_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { SID_LL_ALPHA,	 99},	/* TERMINAL_ADR			 */
   { SID_LLL_ALPHA,	999},	/* ADTNL_RESP_DATA		 */
   { SID_LL_ALPHA,	 77},	/* T1_DATA			 */
   { SID_LLL_ALPHA,	255},	/* Additrional Data Iso		 */
   { SID_LLL_ALPHA,	255},	/* Additrional Data National	 */
   { SID_LLL_ALPHA,	999},	/* ADTNL_DATA_PRIV		 */
   { SID_FIX_ALPHA,	  3},	/* TRANS_CRNCY			 */
   { SID_FIX_ALPHA,	  3},	/* STLMNT_CRNCY			 */

   { SID_FIX_ALPHA,	  3},	/* BILLING_CRNCY		 */
   { SID_FIX_HEX,	 16},	/* PIN_DATA			 */
   { SID_LL_ALPHA,	 16},	/* SECURITY_DATA		 */
   { SID_LLL_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { SID_LLL_BIN,	255},	/* ICC Information               */
   { SID_LL_ALPHA,	255},	/* Orig Data Elements            */
   { SID_FIX_ALPHA,	  3},	/* Reserved National		 */
   { SID_LL_ALPHA,	255},	/* Reserved National		 */
   { SID_LLL_ALPHA,	 14},	/* GEOGR_DATA			 */
   { SID_LLL_ALPHA,	  6},	/* ADTNL_POS_INFO		 */
	
   { SID_LLL_ALPHA,	 999},	/* NETWORK POS DATA		 */
   { SID_LLL_ALPHA, 	999},	/* Redemption Number		 */
   { SID_LLL_ALPHA, 	999},	/* Versions       		 */
   { NOT_USED,	  8},	/* MAC1				 */
   { NOT_USED,	  0},	/* 				 */
   { SID_LLL_ALPHA,	  1},	/* STLMNT_CODE			 */
   { SID_FIX_ALPHA,	  2},	/* Extended Payment Code	*/
   { SID_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		 */
   { SID_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		 */
   { SID_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */

   { SID_FIX_ALPHA,	  8},	/* Message Number		 */
   { SID_LLL_ALPHA,	999},	/* Data Record        		 */
   { SID_FIX_ALPHA,	  6},	/* ACTION_DATE			 */
   { SID_FIX_ALPHA,	 10},	/* CR_NBR			 */
   { SID_FIX_ALPHA,	 10},	/* CR_REV_NBR			 */
   { SID_FIX_ALPHA,	 10},	/* DB_NBR			 */
   { SID_FIX_ALPHA,	 10},	/* DB_REV_NBR			 */
   { SID_FIX_ALPHA,	 10},	/* Transfer Number		 */
   { SID_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
   { SID_FIX_ALPHA,	 10},	/* Inquiries Number		 */

   { SID_FIX_ALPHA,	 10},	/* Authorizations Number	 */
   { SID_FIX_ALPHA,	 10},	/* Credit, Processing Fee Amount */
   { SID_FIX_ALPHA,	 10},	/* Payment Number                 */
   { SID_FIX_ALPHA,	 10},	/* Payment Reversal Number     	 */
   { SID_FIX_ALPHA,	 10},	/* Debit, Transaction Fee Amount */
   { SID_FIX_ALPHA,	 16},	/* CR_AMOUNT			 */
   { SID_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		 */
   { SID_FIX_ALPHA,	 16},	/* DB_AMOUNT			 */
   { SID_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		 */
   { SID_FIX_ALPHA,	 35},	/* ORIG_DATA			 */

   { SID_FIX_ALPHA,	  3},	/* FILE_UPDATE			 */
   { SID_FIX_ALPHA,	  3},	/* FILE_SECURITY		 */
   { SID_LL_ALPHA,	 11},	/* Destination Inst ID Code      */
   { SID_LL_ALPHA,	 11},	/* Source Inst Id Code           */
   { SID_LL_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
   { SID_LLL_ALPHA,	 17},	/* Additional Key Mngmt data     */
   { SID_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { SID_FIX_ALPHA,	 25},	/* Payee			 */
   { SID_LL_ALPHA,	 11},	/* STLMNT_INSTITN		 */
   { SID_LL_ALPHA,	 11},	/* RECEIVER_ID			 */

   { SID_LL_ALPHA,	 17},	/* FILE_NAME			 */
   { SID_LL_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { SID_LL_ALPHA,	 28},	/* ACCOUNT_ID2			 */
   { SID_LLL_ALPHA,	 16},	/* Transaction Description	 */
   { SID_FIX_ALPHA,	 16},	/* Reserved Iso			 */
   { SID_FIX_ALPHA,	 16},	/* Reserved Iso			 */
   { SID_FIX_ALPHA,	 10},	/* Reserved Iso			 */
   { SID_FIX_ALPHA,	 10},	/* Reserved Iso			 */
   { SID_LL_ALPHA,	  0},	/* Reserved Iso			 */
   { SID_LL_ALPHA,	  0},	/* Reserved Iso			 */

   { SID_LLL_ALPHA,	  0},	/* Reserved Iso			 */
   { SID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { SID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { SID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { SID_LLL_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
   { SID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { SID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { SID_LLL_ALPHA,	  0},	/* Intra Country Data		 */
   { SID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { SID_LLL_ALPHA,	  0},	/* Original Message Type Id	 */

   { SID_LLL_ALPHA,	  0},	/* Issuing Institution Id	 */
   { SID_LLL_ALPHA,	  0},	/* Remaining Open to Use	 */
   { SID_LLL_ALPHA,	 29},	/* ADR_VERIF_DATA		 */
   { SID_LLL_ALPHA,	  0},	/* Free Form Text - Japan	 */
   { SID_LLL_ALPHA,	255},	/* SUPPORT_INFO			 */
   { SID_LLL_ALPHA,	255},	/* Reserved Private		 */
   { SID_LLL_ALPHA,	  0},	/* File Record Action and Data	 */
   { SID_FIX_ALPHA,	  8},	/* MAC2				 */

};

static tag_info_t  tab_SidTagInfo[] = 
{
   { "7100",SID_TAG_VAR_BCD , 0},/* Issuer Script Template 1              */
   { "7200",SID_TAG_VAR_BCD , 0},/* Issuer Script Template 2              */
   { "8600",SID_TAG_VAR_BCD , 0},/* Issuer Script command                 */
   { "8200",SID_TAG_VAR_HEX , 0},/* Application Interchange Profile       */
   { "8400",SID_TAG_VAR_BCD , 0},/* Dedicated File Name                   */
   { "4F00",SID_TAG_VAR_BCD , 0},/* ICC Application Id                    */
   { "8A00",SID_TAG_VAR_BCD , 0},/* Authorization Response Code           */
   { "9500",SID_TAG_VAR_BCD , 0},/* Terminal Verification Results         */
   { "9A00",SID_TAG_VAR_BCD , 0},/* Transaction Date                      */
   { "9C00",SID_TAG_VAR_BCD , 0},/* Transaction TYPE                      */
   { "5F2A",SID_TAG_VAR_BCD , 0},/* Transaction Currency Code             */
   { "9F02",SID_TAG_VAR_BCD , 0},/* Transaction Amount                    */
   { "9F03",SID_TAG_VAR_BCD , 0},/* Other Amount                          */
   { "9F09",SID_TAG_VAR_BCD , 0},/* Terminal Application Version Number   */
   { "9F10",SID_TAG_VAR_HEX , 0},/*  Issuer Application Data              */
   { "9F1A",SID_TAG_VAR_BCD , 0},/* Terminal Country Code                 */
   { "9F1E",SID_TAG_VAR_ALPHA , 0},/* Interface Device Serial Number        */
   { "9F26",SID_TAG_VAR_HEX , 0},/* Application cryptogram                */
   { "9F27",SID_TAG_VAR_BCD , 0},/* Cryptogram Information Data           */
   { "9F33",SID_TAG_VAR_HEX , 0},/* Terminal Capabilities                 */
   { "9F34",SID_TAG_VAR_BCD , 0},/* Cardholder Verification Method        */
   { "9F35",SID_TAG_VAR_BCD , 0},/* Terminal Type                         */
   { "9F36",SID_TAG_VAR_HEX , 0},/* Application Transaction Counter       */
   { "9F37",SID_TAG_VAR_HEX , 0},/* Unpredictable Number                  */
   { "9F41",SID_TAG_VAR_BCD , 0},/* Transaction Sequence Number           */
   { "9F53",SID_TAG_VAR_BCD , 0},/* Transaction Category Code             */
   { "9100",SID_TAG_VAR_HEX , 0},/* Issuer Authentication Data            */
   { "9F54",SID_TAG_VAR_HEX , 0},/* Application cryptogram  ARPC          */
};

void  InitSidIcTag  (TSTagSid * msgInfo)
{
   int	i;
   int	nSize;

   nSize = sizeof(msgInfo->nPosTag)/sizeof(msgInfo->nPosTag[0]);
   memset((char*)msgInfo->nPosTag,0,sizeof(msgInfo->nPosTag));
   for (i = 0; i < nSize; i++)
   {
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', sizeof(msgInfo->sChipData));
}

void  InitSidInfo(TSSidInfo *msgInfo)
{
   int i;

   memset((char*)msgInfo->nFieldPos,0,sizeof(msgInfo->nFieldPos));
   memset((char*)msgInfo->sBitMap,0,sizeof(msgInfo->sBitMap));
   memset(msgInfo->sHeader,   '\0', sizeof(msgInfo->sHeader));/*NAB0204*/
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', sizeof(msgInfo->sData));
}

int GetSidFieldType(int field_no)
{
   return(tab_SidFldInfo[field_no].type);
}

int GetSidFieldLength(int field_no)
{
   return(tab_SidFldInfo[field_no].length);
}

int GetSidTagIndice ( char * szTagName)
{
   int nIndice, nSize;
   char sLine[MAX_LINE_TRC];
   

   nSize = sizeof(tab_SidTagInfo) /sizeof(tab_SidTagInfo[0]);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_SidTagInfo[nIndice].sTagName, 4) == 0)
	  {
         	return(nIndice);
      }
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetSidTagType ( int nIndice )
{
   return ( tab_SidTagInfo[nIndice].nType);
}

int GetSidTagLength ( int nIndice )
{
   return ( tab_SidTagInfo[nIndice].nLength);
}

