#include <stdio.h>
#include <stdlib.h>
#include <iso_jcc.h>
#include <define.h>


fld_info_t    tab_JccFldInfo[] =
{
   { NOT_USED,  	  0},	/* BIT_MAP1			 */
   { JCC_LL_BCD,	 19},   /* CARD_NBR 			 */
   { JCC_FIX_BCD,	  6},	/* PROC_CODE			 */
   { JCC_FIX_BCD,	 12},	/* TRANS_AMOUNT			 */
   { JCC_FIX_BCD,	 12},	/* STLMNT_AMOUNT		 */
   { JCC_FIX_BCD,	 12},	/* Cardholder Billing Amount	 */
   { JCC_FIX_BCD,	 10},	/* XMIT_TIME			 */
   { NOT_USED,	  8},	/* Cardholder Billing Fee	 */
   { NOT_USED,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { JCC_FIX_BCD,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { JCC_FIX_BCD,	  6},	/* AUDIT_NBR			 */
   { JCC_FIX_BCD,	  6},	/* TRANS_DATE_TIME               */
   { JCC_FIX_BCD,	  4},	/* TRANS_DATE			 */
   { JCC_FIX_BCD,	  4},	/* EXPIRY_DATE			 */
   { JCC_FIX_BCD,	  4},	/* STLMNT_DATE			 */
   /*YK26042006*/
   /*{ NOT_USED,	  4},*/	/* CNVRSN_DATE			 */
   { JCC_FIX_BCD,	  4},	/* CNVRSN_DATE			 */
   /*YK26042006*/
   { JCC_FIX_BCD,	  4},	/* CAPTURE_DATE			 */
   { JCC_FIX_BCD,	  4},	/* MERCHANT_TYPE		 */
   { JCC_FIX_BCD,	  3},	/* ACQR_COUNTRY			 */
   { NOT_USED,	  3},	/* PAN_COUNTRY			 */

   { NOT_USED,	  3},	/* FORWD_COUNTRY		 */
   { JCC_FIX_BCD,	  3},	/* POS_ENTRY_MODE		 */
   { JCC_FIX_BCD,	  3},	/* Card Sequence Number		 */
   { NOT_USED,	  3},	/* Function Code                 */
   { JCC_FIX_BCD,	  2},	/* POS_CONDITION		 */
   { JCC_FIX_BCD,	  2},	/* MERCHANT TYPE  		 */
   { NOT_USED,	  1},	/* Authorization Id Response Lenght */
   { NOT_USED,	  6},	/* TRANS_FEE			 */
   { NOT_USED,	  3},	/* Amount Settlement Fee	 */
   { NOT_USED,	 24},	/* Amount Transaction Processing Fee */
	
   { NOT_USED,	  9},	/* Amount Setlmnt Processing Fee */
   { JCC_LL_BCD,	 11},	/* ACQR_ID			 */
   { NOT_USED,	 11},	/* FORWD_ID			 */
   { NOT_USED,	 28},	/* PAN_EXTENDED			 */
   /*** { JCC_LL_BCD,	 37},***/	/* T2_DATA			 */
   { JCC_LL_HEX,	 37},	/* T2_DATA			 */
   { NOT_USED,	  4},	/* T1_DATA                       */
   { JCC_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { JCC_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { JCC_FIX_ALPHA,	  2},	/* ACTION CODE  		 */
   { NOT_USED,	  3},	/* Service Restriction Code	 */

   { JCC_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { JCC_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { JCC_FIX_ALPHA,	 40},	/* TERMINAL_ADR			 */
   { NOT_USED,	 25},	/* ADTNL_RESP_DATA		 */
   { JCC_LL_ALPHA,	 77},	/* T1_DATA			 */
   { NOT_USED,	999},	/* Additrional Data Jcc		 */
   { NOT_USED,	255},	/* Additrional Data National	 */
   { JCC_LLL_ALPHA,	999},	/* ADTNL_DATA_PRIV		 */
   { JCC_FIX_BCD,	  3},	/* TRANS_CRNCY			 */
   { JCC_FIX_BCD,	  3},	/* STLMNT_CRNCY			 */

   { JCC_FIX_BCD,	  3},	/* BILLING_CRNCY		 */
   { JCC_FIX_HEX,	 16},	/* PIN_DATA			 */
   { JCC_FIX_BCD,	 16},	/* SECURITY_DATA		 */
   { JCC_LL_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { JCC_LLL_BIN,	999},	/* ICC Information               *//*YK220206:EMV*/
   { NOT_USED,	255},	/* Orig Data Elements            */
   { NOT_USED,	  3},	/* Reserved National		 */
   { NOT_USED,	255},	/* Reserved National		 */
   { JCC_LL_ALPHA,	 10},	/* GEOGR_DATA			 */
   { JCC_LL_ALPHA,	999},	/* ADTNL_POS_INFO		 */
	
   { JCC_LL_ALPHA,	 26},	/* OTHER_AMOUNTS		 */
   { NOT_USED,	999},	/* Redemption Number		 */
   { NOT_USED, 	999},	/* Versions       		 */
   { JCC_FIX_BIN,	  8},	/* MAC1				 */
   { JCC_FIX_BCD,	  1},	/* 				 */
   { JCC_FIX_BCD,	  1},	/* STLMNT_CODE			 */
   { NOT_USED,	  2},	/* Extended Payment Code	*/
   { NOT_USED,	  3},	/* RECEIVER_COUNTRY		 */
   { NOT_USED,	  3},	/* STLMNT_INST_COUNTRY		 */
   { JCC_FIX_BCD,	  3},	/* NW_MNGMT_INFO		 */

   { NOT_USED,	  8},	/* Message Number		 */
   { NOT_USED,	999},	/* Data Record        		 */
   { JCC_FIX_BCD,	  6},	/* ACTION_DATE			 */
   { JCC_FIX_BCD,	 10},	/* CR_NBR			 */
   { JCC_FIX_BCD,	 10},	/* CR_REV_NBR			 */
   { JCC_FIX_BCD,	 10},	/* DB_NBR			 */
   { JCC_FIX_BCD,	 10},	/* DB_REV_NBR			 */
   { NOT_USED,	 10},	/* Transfer Number		 */
   { NOT_USED,	 10},	/* Transfer Reversal Number	 */
   { JCC_FIX_BCD,	 10},	/* Inquiries Number		 */

   { JCC_FIX_BCD,	 10},	/* Authorizations Number	 */
   { NOT_USED,	 10},	/* Credit, Processing Fee Amount */
   { NOT_USED,	 10},	/* Payment Number                 */
   { NOT_USED,	 10},	/* Payment Reversal Number     	 */
   { NOT_USED,	 10},	/* Debit, Transaction Fee Amount */
   { JCC_FIX_BCD,	 16},	/* CR_AMOUNT			 */
   { JCC_FIX_BCD,	 16},	/* CR_REV_AMOUNT		 */
   { JCC_FIX_BCD,	 16},	/* DB_AMOUNT			 */
   { JCC_FIX_BCD,	 16},	/* DB_REV_AMOUNT		 */
   { JCC_FIX_BCD,	 42},	/* ORIG_DATA			 */

   { JCC_FIX_ALPHA,	  1},	/* FILE_UPDATE			 */
   { NOT_USED,	  3},	/* FILE_SECURITY		 */
   { NOT_USED,	 11},	/* Destination Inst ID Code      */
   { NOT_USED,	 11},	/* Source Inst Id Code           */
   { JCC_FIX_BCD,	 42},	/* REPLACE_AMOUNTS		 */
   { NOT_USED,	 17},	/* Additional Key Mngmt data     */
   { JCC_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { NOT_USED,	 25},	/* Payee			 */
   { JCC_LL_BCD,	 11},	/* STLMNT_INSTITN		 */
   { JCC_LL_BCD,	 11},	/* RECEIVER_ID			 */

   { JCC_LL_ALPHA,	 17},	/* FILE_NAME			 */
   { NOT_USED,	 28},	/* ACCOUNT_ID1			 */
   { NOT_USED,	 28},	/* ACCOUNT_ID2			 */
   { JCC_LL_HEX, 255},/* ENH15.1 SWI24022015 { NOT_USED,	 16},*/	/* Transaction Description	 */
   { NOT_USED,	 16},	/* Reserved Jcc			 */
   { NOT_USED,	 16},	/* Reserved Jcc			 */
   { NOT_USED,	 10},	/* Reserved Jcc			 */
   { NOT_USED,	 10},	/* Reserved Jcc			 */
   { NOT_USED,	  0},	/* Reserved Jcc			 */
   { NOT_USED,	  0},	/* Reserved Jcc			 */

   { NOT_USED,	  0},	/* Reserved Jcc			 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	 24},	/* ADTNL_TRACE_DATA		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Intra Country Data		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Original Message Type Id	 */

   { NOT_USED,	  0},	/* Issuing Institution Id	 */
   { NOT_USED,	  0},	/* Remaining Open to Use	 */
   { NOT_USED,	 29},	/* ADR_VERIF_DATA		 */
   { NOT_USED,	  0},	/* Free Form Text - Japan	 */
   { NOT_USED,	255},	/* SUPPORT_INFO			 */
   { JCC_LL_BIN,	 255},   /* JCC PRIVATE USE FIELD */ /*YK March 2006 Visa e-commerce*/
   { JCC_LL_ALPHA,	  0},	/* File Record Action and Data	 */
   { JCC_FIX_BIN,	  8},	/* MAC2				 */
};

tag_info_t  tab_JccTagInfo[] = 
{
   { "7100",JCC_TAG_VAR_HEX , 0},/* Issuer Script Template 1              */
   { "7200",JCC_TAG_VAR_HEX , 0},/* Issuer Script Template 2              */
   { "8200",JCC_TAG_VAR_HEX , 0},/* Application Interchange Profile       */
   { "8400",JCC_TAG_VAR_HEX , 0},/* Dedicated File Name                   */
   { "8600",JCC_TAG_VAR_HEX , 0},/* Issuer Script command                 */
   { "8A00",JCC_TAG_VAR_HEX , 0},/* Authorization Response Code           */
   { "4F00",JCC_TAG_VAR_HEX , 0},/* ICC Application Id                    */
   { "9100",JCC_TAG_VAR_HEX , 0},/* Issuer Authentication Data            */
   { "9500",JCC_TAG_VAR_HEX , 0},/* Terminal Verification Results         */
   { "9A00",JCC_TAG_VAR_HEX , 0},/* Transaction Date                      */
   { "9C00",JCC_TAG_VAR_HEX , 0},/* Transaction TYPE                      */
   { "5F2A",JCC_TAG_VAR_HEX , 0},/* Transaction Currency Code             */
   { "5F34",JCC_TAG_VAR_HEX , 0},/* PAN Sequence Numnber                  */ /*YK220206:EMV*/
   { "9F02",JCC_TAG_VAR_HEX , 0},/* Transaction Amount                    */
   { "9F03",JCC_TAG_VAR_HEX , 0},/* Other Amount                          */
   { "9F09",JCC_TAG_VAR_HEX , 0},/* Terminal Application Version Number   */
   { "9F10",JCC_TAG_VAR_HEX , 0},/*  Issuer Application Data              */
   { "9F1A",JCC_TAG_VAR_HEX , 0},/* Terminal Country Code                 */
   /*{ "9F1E",JCC_TAG_VAR_HEX , 0},  ACK APRIL 2015 *//* Interface Device Serial Number        */
   { "9F1E",JCC_TAG_VAR_ALPHA , 0},/* Interface Device Serial Number        */
   { "9F26",JCC_TAG_VAR_HEX , 0},/* Application cryptogram                */
   { "9F27",JCC_TAG_VAR_HEX , 0},/* Cryptogram Information Data           */
   { "9F33",JCC_TAG_VAR_HEX , 0},/* Terminal Capabilities                 */
   { "9F34",JCC_TAG_VAR_HEX , 0},/* Cardholder Verification Method        */
   { "9F35",JCC_TAG_VAR_HEX , 0},/* Terminal Type                         */
   { "9F36",JCC_TAG_VAR_HEX , 0},/* Application Transaction Counter       */
   { "9F37",JCC_TAG_VAR_HEX , 0},/* Unpredictable Number                  */
   { "9F41",JCC_TAG_VAR_HEX , 0},/* Transaction Sequence Number           */
   { "9F53",JCC_TAG_VAR_HEX , 0},/* Transaction Category Code             */
   { "9F6E",JCC_TAG_VAR_HEX , 0},/* Transaction Description TAG           */ /*SWI06032015 ENH15.1*/
   { "C000",JCC_TAG_VAR_HEX , 0},/* New Pin                               */
};

pds_info_t  tab_JccPdsInfo[] = 
{
   { "20",  JCC_PDS_FIX_ALPHA,   1},/* Cardholder Verification Method    */
   { "40",  JCC_PDS_FIX_ALPHA,  40},/* Certificate Serial Number         */
   { "42",  JCC_PDS_FIX_ALPHA,   2},/* Security Level Indicator          */
   { "89",  JCC_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Track Edit    */
   { "92",  JCC_PDS_FIX_ALPHA,   3},/* Card Verification Code 2          */
   { "93",  JCC_PDS_FIX_ALPHA,  20},/* SET unique Transaction ID         */
   { "94",  JCC_PDS_FIX_ALPHA,  20},/* TransStain                        */
   { "95",  JCC_PDS_FIX_ALPHA,   1},/* Card Authentication Results Code  */ /* TBO31072014 */
};

void  InitJccIcTag  (TSTagJcc * msgInfo)
{
   int i;

   for (i = 0; i < MAX_JCC_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', MAX_JCC_CHIP_LEN);
}

void  InitJccInfo(TSJccInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_JCC_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < JCC_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_JCC_DATA_LEN);
}

void  InitJccPds  (TSPdsJcc * pdsInfo)
{
   int i;

   for (i = 0; i < MAX_JCC_PDS; i++)
   {
      pdsInfo->nPosPds [i] = 0;
      pdsInfo->nPresent[i] = NOT_PRESENT;
   }
   pdsInfo->nLength  = 0;
   memset(pdsInfo->sPdsData,   '\0', MAX_JCC_PDS_LEN);
}

int GetJccFieldType(int field_no)
{
   return(tab_JccFldInfo[field_no].type);
}

int GetJccFieldLength(int field_no)
{
   return(tab_JccFldInfo[field_no].length);
}

int GetJccTagIndice ( char * szTagName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_JccTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   { 
      if (memcmp(szTagName, tab_JccTagInfo[nIndice].sTagName, 4) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetJccTagType ( int nIndice )
{
   return ( tab_JccTagInfo[nIndice].nType);
}

int GetJccTagLength ( int nIndice )
{
   return ( tab_JccTagInfo[nIndice].nLength);
}

int GetJccPdsIndice ( char * szPdsName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_JccPdsInfo) /sizeof(pds_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szPdsName, tab_JccPdsInfo[nIndice].sPdsName, 2) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetJccPdsType ( int nIndice )
{
   return ( tab_JccPdsInfo[nIndice].nType);
}

int GetJccPdsLength ( int nIndice )
{
   return ( tab_JccPdsInfo[nIndice].nLength);
}

