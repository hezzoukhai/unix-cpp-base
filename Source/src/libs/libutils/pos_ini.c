#include <stdio.h>
#include <stdlib.h>
#include <iso_pos.h>
#include <define.h>


fld_info_t    tab_PosFldInfo[] =
{
   { NOT_USED,  	  0},	/* BIT_MAP1			 */
   { POS_LL_ALPHA,	 19},   /* CARD_NBR 			 */
   { POS_FIX_ALPHA,	  6},	/* PROC_CODE			 */
   { POS_FIX_ALPHA,	 12},	/* TRANS_AMOUNT			 */
   { POS_FIX_ALPHA,	 12},	/* STLMNT_AMOUNT		 */
   { NOT_USED,	 	 12},	/* Cardholder Billing Amount	 */
   { POS_FIX_ALPHA,	 10},	/* XMIT_TIME			 */
   { NOT_USED,	  	  8},	/* Cardholder Billing Fee	 */
   { POS_FIX_ALPHA,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { NOT_USED,	  	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { POS_FIX_ALPHA,	  6},	/* AUDIT_NBR			 */
   { POS_FIX_ALPHA,	 12},	/* TRANS_DATE_TIME               */
   { POS_FIX_ALPHA,	  4},	/* START_DATE			 */
   { POS_FIX_ALPHA,	  4},	/* EXPIRY_DATE			 */
   { POS_FIX_ALPHA,	  6},	/* STLMNT_DATE			 */
   { NOT_USED,	  	  4},	/* CNVRSN_DATE			 */
   { POS_FIX_ALPHA,	  4},	/* CAPTURE_DATE			 */
   { NOT_USED,	          4},	/* MERCHANT_TYPE		 */
   { NOT_USED,	    	  3},	/* ACQR_COUNTRY			 */
   { NOT_USED,	          3},	/* PAN_COUNTRY			 */

   { NOT_USED,	          3},	/* FORWD_COUNTRY		 */
   { POS_FIX_ALPHA,	 12},	/* POS_ENTRY_MODE		 */
   { POS_FIX_ALPHA,	  3},	/* Card Sequence Number		 */
   { POS_FIX_ALPHA,	  3},	/* Function Code                 */
   { POS_FIX_ALPHA,	  4},	/* Message Reason Code           */
   { POS_FIX_ALPHA,	  4},	/* MERCHANT TYPE  		 */
   { NOT_USED,		  1},	/* Authorization Id Response Lenght */
   { NOT_USED,		  6},	/* TRANS_FEE			 */
   { NOT_USED,		  3},	/* Amount Settlement Fee	 */
   { POS_FIX_ALPHA,	 12},	/* Amount Transaction Processing Fee */
	
   { NOT_USED,		  9},	/* Amount Setlmnt Processing Fee */
   { POS_LL_ALPHA,	 11},	/* ACQR_ID			 */
   { NOT_USED,		 11},	/* FORWD_ID			 */
   { POS_LL_ALPHA,	 28},	/* PAN_EXTENDED			 */
   { POS_LL_ALPHA,	 37},	/* T2_DATA			 */
   { POS_LLL_ALPHA,	  4},	/* T1_DATA                       */
   { POS_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { POS_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { POS_FIX_ALPHA,	  3},	/* ACTION CODE  		 */
   { NOT_USED,		  3},	/* Service Restriction Code	 */

   /*Numero de terminal remis encore une fois sur 8*/
   { POS_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { POS_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { POS_LL_ALPHA,	 99},	/* TERMINAL_ADR			 */
   /* ihi 22032003 correction au niveau de la specification du champ 
      44 pour le mettre comme la spec le precise LLLVAR*/	
   { POS_LLL_ALPHA,	999},	/* ADTNL_RESP_DATA		 */
   { POS_LL_ALPHA,	 77},	/* T1_DATA			 */
   { POS_LLL_ALPHA,	255},	/* Additrional Data Pos		 */
   { POS_LLL_ALPHA,	255},	/* Additrional Data National	 */
   { POS_LLL_ALPHA,	999},	/* ADTNL_DATA_PRIV		 */
   { POS_FIX_ALPHA,	  3},	/* TRANS_CRNCY			 */
   { POS_FIX_ALPHA,	  3},	/* STLMNT_CRNCY			 */

   { NOT_USED,		  3},	/* BILLING_CRNCY		 */
   { POS_FIX_HEX,	 16},	/* PIN_DATA			 */
   { POS_LL_ALPHA,	 18},	/* SECURITY_DATA		 */
   { POS_LLL_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { POS_LLL_BIN,	255},	/* ICC Information               */
   { POS_LL_ALPHA,	255},	/* Orig Data Elements            */
   { NOT_USED,		  3},	/* Reserved National		 */
   { NOT_USED,		255},	/* Reserved National		 */
   { NOT_USED,		 14},	/* GEOGR_DATA			 */
   { POS_LLL_ALPHA,	  6},	/* ADTNL_POS_INFO		 */
	
   { POS_LLL_ALPHA,	 36},	/* OTHER_AMOUNTS		 */
   { POS_LLL_ALPHA, 999}, /*REDUMPTION NUMBER */
   { POS_LLL_ALPHA, 	999},	/* Versions       		 */
   { POS_FIX_BIN,	  8},	/* MAC1				 */
   { NOT_USED,	  0},	/* 				 */
   { POS_LLL_ALPHA,	  1},	/* STLMNT_CODE			 */
   { POS_FIX_ALPHA,	  2},	/* Extended Payment Code	*/
   { POS_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		 */
   { POS_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		 */
   { POS_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */

   { POS_FIX_ALPHA,	  8},	/* Message Number		 */
   { POS_LLL_ALPHA,	999},	/* Data Record        		 */
   { POS_FIX_ALPHA,	  6},	/* ACTION_DATE			 */
   { POS_FIX_ALPHA,	 10},	/* CR_NBR			 */
   { POS_FIX_ALPHA,	 10},	/* CR_REV_NBR			 */
   { POS_FIX_ALPHA,	 10},	/* DB_NBR			 */
   { POS_FIX_ALPHA,	 10},	/* DB_REV_NBR			 */
   { POS_FIX_ALPHA,	 10},	/* Transfer Number		 */
   { POS_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
   { POS_FIX_ALPHA,	 10},	/* Inquiries Number		 */

   { POS_FIX_ALPHA,	 10},	/* Authorizations Number	 */
   { POS_FIX_ALPHA,	 10},	/* Credit, Processing Fee Amount */
   { POS_FIX_ALPHA,	 10},	/* Payment Number                 */
   { POS_FIX_ALPHA,	 10},	/* Payment Reversal Number     	 */
   { POS_FIX_ALPHA,	 10},	/* Debit, Transaction Fee Amount */
   { POS_FIX_ALPHA,	 16},	/* CR_AMOUNT			 */
   { POS_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		 */
   { POS_FIX_ALPHA,	 16},	/* DB_AMOUNT			 */
   { POS_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		 */
   { POS_FIX_ALPHA,	 10},	/* ORIG_DATA			 */

   { POS_FIX_ALPHA,	  3},	/* FILE_UPDATE			 */
   { POS_FIX_ALPHA,	  3},	/* FILE_SECURITY		 */
   { POS_LL_ALPHA,	 11},	/* Destination Inst ID Code      */
   { POS_LL_ALPHA,	 11},	/* Source Inst Id Code           */
   { POS_LL_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
   { POS_LLL_ALPHA,	 17},	/* Additional Key Mngmt data     */
   { POS_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { POS_FIX_ALPHA,	 25},	/* Payee			 */
   { POS_LL_ALPHA,	 11},	/* STLMNT_INSTITN		 */
   { POS_LL_ALPHA,	 11},	/* RECEIVER_ID			 */

   { POS_LL_ALPHA,	 17},	/* FILE_NAME			 */
   { POS_LL_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { POS_LL_ALPHA,	 28},	/* ACCOUNT_ID2			 */
   { POS_LLL_ALPHA,	 16},	/* Transaction Description	 */
   { POS_FIX_ALPHA,	 16},	/* Reserved Pos			 */
   { POS_FIX_ALPHA,	 16},	/* Reserved Pos			 */
   { POS_FIX_ALPHA,	 10},	/* Reserved Pos			 */
   { POS_FIX_ALPHA,	 10},	/* Reserved Pos			 */
   { POS_LL_ALPHA,	  0},	/* Reserved Pos			 */
   { POS_LLL_BIN, 999},	/* Reserved Pos			 */ /*180204 HKETTANI Field 110 used for chip data during EOD Download Trxs */

   { POS_LLL_ALPHA,	  0},	/* Reserved Pos			 */
   { POS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { POS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { POS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { POS_LLL_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
   { POS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { POS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { POS_LLL_ALPHA,	  0},	/* Intra Country Data		 */
   { POS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { POS_LLL_ALPHA,	  0},	/* Original Message Type Id	 */

   { POS_LLL_ALPHA,	  0},	/* Issuing Institution Id	 */
   { POS_LLL_ALPHA,	  0},	/* Remaining Open to Use	 */
   { POS_LLL_ALPHA,	 29},	/* ADR_VERIF_DATA		 */
   { POS_LLL_ALPHA,	  0},	/* Free Form Text - Japan	 */
   { POS_LLL_ALPHA,	255},	/* SUPPORT_INFO			 */
   { POS_LLL_ALPHA,	  0},	/* Reserved Private		 */
   { POS_LLL_ALPHA,	  0},	/* File Record Action and Data	 */
   { POS_FIX_BIN,	  8},	/* MAC2				 */

};
/* 21122004 */
tag_info_t  tab_PosTagInfo[] =
{
   { "7100",POS_TAG_VAR_BCD , 0},/* Issuer Script Template 1              */
   { "7200",POS_TAG_VAR_BCD , 0},/* Issuer Script Template 2              */
   { "8600",POS_TAG_VAR_BCD , 0},/* Issuer Script command                 */
   { "8200",POS_TAG_VAR_HEX , 0},/* Application Interchange Profile       */
   { "8400",POS_TAG_VAR_BCD , 0},/* Dedicated File Name                   */
   { "4F00",POS_TAG_VAR_BCD , 0},/* ICC Application Id                    */
   { "8A00",POS_TAG_VAR_HEX , 0},/* Authorization Response Code           */
   { "9500",POS_TAG_VAR_HEX , 0},/* Terminal Verification Results         */
   { "9A00",POS_TAG_VAR_BCD , 0},/* Transaction Date                      */
   { "9C00",POS_TAG_VAR_BCD , 0},/* Transaction TYPE                      */
   { "5F2A",POS_TAG_VAR_BCD , 0},/* Transaction Currency Code             */
   { "9F02",POS_TAG_VAR_BCD , 0},/* Transaction Amount                    */
   { "9F03",POS_TAG_VAR_BCD , 0},/* Other Amount                          */
   { "9F09",POS_TAG_VAR_BCD , 0},/* Terminal Application Version Number   */
   { "9F10",POS_TAG_VAR_HEX , 0},/*  Issuer Application Data              */
   { "9F1A",POS_TAG_VAR_BCD , 0},/* Terminal Country Code                 */
   { "9F1E",POS_TAG_VAR_ALPHA , 0},/* Interface Device Serial Number        */
   { "9F26",POS_TAG_VAR_HEX , 0},/* Application cryptogram                */
   { "9F27",POS_TAG_VAR_BCD , 0},/* Cryptogram Information Data           */
   { "9F33",POS_TAG_VAR_HEX , 0},/* Terminal Capabilities                 */
   { "9F34",POS_TAG_VAR_HEX , 0},/* Cardholder Verification Method        */
   { "9F35",POS_TAG_VAR_BCD , 0},/* Terminal Type                         */
   { "9F36",POS_TAG_VAR_HEX , 0},/* Application Transaction Counter       */
   { "9F37",POS_TAG_VAR_HEX , 0},/* Unpredictable Number                  */
   { "9F41",POS_TAG_VAR_BCD , 0},/* Transaction Sequence Number           */
   { "9F53",POS_TAG_VAR_BCD , 0},/* Transaction Category Code             */
   { "9100",POS_TAG_VAR_HEX , 0},/* Issuer Authentication Data            */
   { "9F54",POS_TAG_VAR_HEX , 0},/* Application cryptogram  ARPC          */
};

void  InitPosIcTag  (TSTagPos * msgInfo)
{
   int i;

   for (i = 0; i < MAX_POS_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', MAX_POS_CHIP_LEN);
}

void  InitPosInfo(TSPosInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_POS_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < POS_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_POS_DATA_LEN);
}

int GetPosFieldType(int field_no)
{
   return(tab_PosFldInfo[field_no].type);
}

int GetPosFieldLength(int field_no)
{
   return(tab_PosFldInfo[field_no].length);
}

int GetPosTagIndice ( char * szTagName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_PosTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_PosTagInfo[nIndice].sTagName, 4) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetPosTagType ( int nIndice )
{
   return ( tab_PosTagInfo[nIndice].nType);
}

int GetPosTagLength ( int nIndice )
{
   return ( tab_PosTagInfo[nIndice].nLength);
}


