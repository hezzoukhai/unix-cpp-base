/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      bank_ini.c                                                             */
/*                                                                             */
/* Description                                                                 */
/*      BANK Init Tools Management                                             */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <iso_bank.h>
#include <define.h>


fld_info_t    tab_BankFldInfo[] =
{
   { NOT_USED,  	      0},	/* BIT_MAP1			                   */
   { BANK_LL_ALPHA,	     19},   /* CARD_NBR 			               */
   { BANK_FIX_ALPHA,	  6},	/* PROC_CODE			               */
   { BANK_FIX_ALPHA,	 12},	/* TRANS_AMOUNT			               */
   { BANK_FIX_ALPHA,	 12},	/* STLMNT_AMOUNT		               */
   { BANK_FIX_ALPHA,	 12},	/* CARDHOLDER BILLING AMOUNT	       */
   { BANK_FIX_ALPHA,	 10},	/* XMIT_TIME			               */
   { BANK_FIX_ALPHA,	  8},	/* CARDHOLDER BILLING FEE	           */
   { BANK_FIX_ALPHA,	  8},	/* CONVRSN_RATE_STLMNT		           */
   { BANK_FIX_ALPHA,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING      */ /*10*/
                                                                       
   { BANK_FIX_ALPHA,	  6},	/* AUDIT_NBR			               */
   { BANK_FIX_ALPHA,	 12},	/* TRANS_DATE_TIME                     */
   { BANK_FIX_ALPHA,	  4},	/* TRANS_DATE			               */
   { BANK_FIX_ALPHA,	  4},	/* EXPIRY_DATE			               */
   { BANK_FIX_ALPHA,	  4},	/* STLMNT_DATE			               */
   { BANK_FIX_ALPHA,	  4},	/* CNVRSN_DATE			               */
   { BANK_FIX_ALPHA,	  4},	/* CAPTURE_DATE			               */
   { BANK_FIX_ALPHA,	  4},	/* MERCHANT_TYPE		               */
   { BANK_FIX_ALPHA,	  3},	/* ACQR_COUNTRY			               */
   { BANK_FIX_ALPHA,	  3},	/* PAN_COUNTRY			               */ /*20*/
                                                                       
   { BANK_FIX_ALPHA,	  3},	/* FORWD_COUNTRY		               */
   { BANK_FIX_ALPHA,	 12},	/* POS_ENTRY_MODE		               */
   { BANK_FIX_ALPHA,	  3},	/* CARD SEQUENCE NUMBER		           */
   { BANK_FIX_ALPHA,	  3},	/* FUNCTION CODE                       */
   { BANK_FIX_ALPHA,	  4},	/* POS_CONDITION		               */
   { BANK_FIX_ALPHA,	  4},	/* MERCHANT TYPE  		               */
   { BANK_FIX_ALPHA,	  1},	/* AUTHORIZATION ID RESPONSE LENGHT    */
   { BANK_FIX_ALPHA,	  6},	/* TRANS_FEE			               */
   { BANK_FIX_ALPHA,	  3},	/* AMOUNT SETTLEMENT FEE	           */
   { BANK_FIX_ALPHA,	 24},	/* AMOUNT TRANSACTION PROCESSING FEE   */ /*30*/
	
   { BANK_LL_ALPHA,	      9},	/* AMOUNT SETLMNT PROCESSING FEE       */
   { BANK_LL_ALPHA,	     11},	/* ACQR_ID			                   */
   { BANK_LL_ALPHA,	     11},	/* FORWD_ID			                   */
   { BANK_LL_ALPHA,	     28},	/* PAN_EXTENDED			               */
   { BANK_LL_ALPHA,	     37},	/* T2_DATA			                   */
   { BANK_LLL_ALPHA,	  4},	/* T1_DATA                             */
   { BANK_FIX_ALPHA,	 12},	/* REFERENCE_NBR		               */
   { BANK_FIX_ALPHA,	  6},	/* AUTHOR_ID			               */
   { BANK_FIX_ALPHA,	  3},	/* ACTION CODE  		               */
   { BANK_FIX_ALPHA,	  3},	/* SERVICE RESTRICTION CODE	           */ /*40*/

   { BANK_FIX_ALPHA,	  8},	/* TERMINAL_NBR			               */
   { BANK_FIX_ALPHA,	 15},	/* OUTLET_NBR			               */
   { BANK_LL_ALPHA,	     99},	/* TERMINAL_ADR			               */
   { BANK_LLL_ALPHA,	999},	/* ADTNL_RESP_DATA		               */
   { BANK_LL_ALPHA,	     77},	/* T1_DATA			                   */
   { BANK_LLL_ALPHA,	255},	/* ADDITRIONAL DATA BANK		       */
   { BANK_LLL_ALPHA,	255},	/* ADDITRIONAL DATA NATIONAL	       */
   { BANK_LLL_ALPHA,	999},	/* ADTNL_DATA_PRIV		               */
   { BANK_FIX_ALPHA,	  3},	/* TRANS_CRNCY			               */
   { BANK_FIX_ALPHA,	  3},	/* STLMNT_CRNCY			               */ /*50*/

   { BANK_FIX_ALPHA,	  3},	/* BILLING_CRNCY		               */
   { BANK_FIX_ALPHA,	 16},	/* PIN_DATA			                   */
   { BANK_LL_ALPHA,	     16},	/* SECURITY_DATA		               */
   { BANK_LLL_ALPHA,	120},	/* ADTNL_AMOUNTS		               */
   { BANK_LLL_BIN,	    255},	/* ICC INFORMATION                     */
   { BANK_LL_ALPHA,	    255},	/* ORIG DATA ELEMENTS                  */
   { BANK_FIX_ALPHA,	  3},	/* RESERVED NATIONAL		           */
   { BANK_LL_ALPHA,	    255},	/* RESERVED NATIONAL		           */
   { BANK_LLL_ALPHA,	 14},	/* GEOGR_DATA			               */
   { BANK_LLL_ALPHA,	  6},	/* ADTNL_POS_INFO		               */ /*60*/
	
   { BANK_LLL_ALPHA,	999},	/* NETWORK POS DATA		               */
   { BANK_LLL_ALPHA, 	999},	/* REDEMPTION NUMBER		           */
   { BANK_LLL_ALPHA, 	999},	/* VERSIONS       		               */
   { NOT_USED,	          8},	/* MAC1				                   */
   { NOT_USED,	          0},	/* 				                       */
   { BANK_LLL_ALPHA,	  1},	/* STLMNT_CODE			               */
   { BANK_FIX_ALPHA,	  2},	/* EXTENDED PAYMENT CODE	           */
   { BANK_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		               */
   { BANK_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		           */
   { BANK_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		               */ /*70*/

   { BANK_FIX_ALPHA,	  8},	/* MESSAGE NUMBER		               */
   { BANK_LLL_ALPHA,	999},	/* DATA RECORD        		           */
   { BANK_FIX_ALPHA,	  6},	/* ACTION_DATE			               */
   { BANK_FIX_ALPHA,	 10},	/* CR_NBR			                   */
   { BANK_FIX_ALPHA,	 10},	/* CR_REV_NBR			               */
   { BANK_FIX_ALPHA,	 10},	/* DB_NBR			                   */
   { BANK_FIX_ALPHA,	 10},	/* DB_REV_NBR			               */
   { BANK_FIX_ALPHA,	 10},	/* TRANSFER NUMBER		               */
   { BANK_FIX_ALPHA,	 10},	/* TRANSFER REVERSAL NUMBER	           */
   { BANK_FIX_ALPHA,	 10},	/* INQUIRIES NUMBER		               */ /*80*/

   { BANK_FIX_ALPHA,	 10},	/* AUTHORIZATIONS NUMBER	           */
   { BANK_FIX_ALPHA,	 10},	/* CREDIT, PROCESSING FEE AMOUNT       */
   { BANK_FIX_ALPHA,	 10},	/* PAYMENT NUMBER                      */
   { BANK_FIX_ALPHA,	 10},	/* PAYMENT REVERSAL NUMBER     	       */
   { BANK_FIX_ALPHA,	 10},	/* DEBIT, TRANSACTION FEE AMOUNT       */
   { BANK_FIX_ALPHA,	 16},	/* CR_AMOUNT			               */
   { BANK_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		               */
   { BANK_FIX_ALPHA,	 16},	/* DB_AMOUNT			               */
   { BANK_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		               */
   { BANK_FIX_ALPHA,	 35},	/* ORIG_DATA			               */ /*90*/
                                                                       
   { BANK_FIX_ALPHA,	  3},	/* FILE_UPDATE			               */
   { BANK_FIX_ALPHA,	  3},	/* FILE_SECURITY		               */
   { BANK_LL_ALPHA,	     11},	/* DESTINATION INST ID CODE            */
   { BANK_LL_ALPHA,	     11},	/* SOURCE INST ID CODE                 */
   { BANK_FIX_ALPHA,	 42},	/* REPLACE_AMOUNTS		               */
   { BANK_LLL_ALPHA,	 17},	/* ADDITIONAL KEY MNGMT DATA           */
   { BANK_FIX_ALPHA,	 17},	/* NET_AMOUNT		                   */
   { BANK_FIX_ALPHA,	 25},	/* PAYEE			                   */
   { BANK_LL_ALPHA,	     11},	/* STLMNT_INSTITN		               */
   { BANK_LL_ALPHA,	     11},	/* RECEIVER_ID			               */ /*100*/
                                                                       
   { BANK_LL_ALPHA,	     17},	/* FILE_NAME			               */
   { BANK_LL_ALPHA,	     28},	/* ACCOUNT_ID1			               */
   { BANK_LL_ALPHA,	     28},	/* ACCOUNT_ID2			               */
   { BANK_LLL_ALPHA,	 16},	/* TRANSACTION DESCRIPTION	           */
   { BANK_FIX_ALPHA,	 16},	/* RESERVED BANK			           */
   { BANK_FIX_ALPHA,	 16},	/* RESERVED BANK			           */
   { BANK_FIX_ALPHA,	 10},	/* RESERVED BANK			           */
   { BANK_FIX_ALPHA,	 10},	/* RESERVED BANK			           */
   { BANK_LL_ALPHA,	      0},	/* RESERVED BANK			           */
   { BANK_LL_ALPHA,	      0},	/* RESERVED BANK			           */ /*110*/
                                                                       
   { BANK_LLL_ALPHA,	  0},	/* RESERVED BANK			           */
   { BANK_LLL_ALPHA,	  0},	/* RESERVED NATIONAL		           */
   { BANK_LLL_ALPHA,	  0},	/* RESERVED NATIONAL		           */
   { BANK_LLL_ALPHA,	  0},	/* RESERVED NATIONAL		           */
   { BANK_LLL_ALPHA,	 24},	/* ADTNL_TRACE_DATA		               */
   { BANK_LLL_ALPHA,	  0},	/* RESERVED NATIONAL		           */
   { BANK_LLL_ALPHA,	  0},	/* RESERVED NATIONAL		           */
   { BANK_LLL_ALPHA,	  0},	/* INTRA COUNTRY DATA		           */
   { BANK_LLL_ALPHA,	  0},	/* RESERVED NATIONAL		           */
   { BANK_LLL_ALPHA,	  0},	/* ORIGINAL MESSAGE TYPE ID	           */ /*120*/

   { BANK_LLL_ALPHA,	  0},	/* ISSUING INSTITUTION ID	           */
   { BANK_LLL_ALPHA,	  0},	/* REMAINING OPEN TO USE	           */
   { BANK_LLL_ALPHA,	 29},	/* ADR_VERIF_DATA		               */
   { BANK_LLL_ALPHA,	  0},	/* FREE FORM TEXT - JAPAN	           */
   { BANK_LLL_ALPHA,	255},	/* SUPPORT_INFO			               */
   { BANK_LLL_ALPHA,	255},	/* RESERVED PRIVATE		               */
   { BANK_LLL_ALPHA,	  0},	/* FILE RECORD ACTION AND DATA	       */
   { NOT_USED,	          8},	/* MAC2				                   */

};

tag_info_t  tab_BankTagInfo[] = 
{
   { "7100",BANK_TAG_VAR_BCD   , 0},/* Issuer Script Template 1              */
   { "7200",BANK_TAG_VAR_BCD   , 0},/* Issuer Script Template 2              */
   { "8600",BANK_TAG_VAR_BCD   , 0},/* Issuer Script command                 */
   { "8200",BANK_TAG_VAR_HEX   , 0},/* Application Interchange Profile       */
   { "8400",BANK_TAG_VAR_BCD   , 0},/* Dedicated File Name                   */
   { "4F00",BANK_TAG_VAR_BCD   , 0},/* ICC Application Id                    */
   { "8A00",BANK_TAG_VAR_BCD   , 0},/* Authorization Response Code           */
   { "9500",BANK_TAG_VAR_BCD   , 0},/* Terminal Verification Results         */
   { "9A00",BANK_TAG_VAR_BCD   , 0},/* Transaction Date                      */
   { "9C00",BANK_TAG_VAR_BCD   , 0},/* Transaction TYPE                      */
   { "5F2A",BANK_TAG_VAR_BCD   , 0},/* Transaction Currency Code             */
   { "9F02",BANK_TAG_VAR_BCD   , 0},/* Transaction Amount                    */
   { "9F03",BANK_TAG_VAR_BCD   , 0},/* Other Amount                          */
   { "9F09",BANK_TAG_VAR_BCD   , 0},/* Terminal Application Version Number   */
   { "9F10",BANK_TAG_VAR_HEX   , 0},/*  Issuer Application Data              */
   { "9F1A",BANK_TAG_VAR_BCD   , 0},/* Terminal Country Code                 */
   { "9F1E",BANK_TAG_VAR_ALPHA , 0},/* Interface Device Serial Number        */
   { "9F26",BANK_TAG_VAR_HEX   , 0},/* Application cryptogram                */
   { "9F27",BANK_TAG_VAR_BCD   , 0},/* Cryptogram Information Data           */
   { "9F33",BANK_TAG_VAR_HEX   , 0},/* Terminal Capabilities                 */
   { "9F34",BANK_TAG_VAR_BCD   , 0},/* Cardholder Verification Method        */
   { "9F35",BANK_TAG_VAR_BCD   , 0},/* Terminal Type                         */
   { "9F36",BANK_TAG_VAR_HEX   , 0},/* Application Transaction Counter       */
   { "9F37",BANK_TAG_VAR_HEX   , 0},/* Unpredictable Number                  */
   { "9F41",BANK_TAG_VAR_BCD   , 0},/* Transaction Sequence Number           */
   { "9F53",BANK_TAG_VAR_BCD   , 0},/* Transaction Category Code             */
   { "9100",BANK_TAG_VAR_HEX   , 0},/* Issuer Authentication Data            */
   { "9F54",BANK_TAG_VAR_HEX   , 0},/* Application cryptogram  ARPC          */
};

void  InitBankIcTag  (TSTagBank * msgInfo)
{
   int i;

   for (i = 0; i < MAX_BANK_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', MAX_BANK_CHIP_LEN);
}

void  InitBankInfo(TSBankInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_BANK_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < BANK_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_BANK_DATA_LEN);
}

int GetBankFieldType(int field_no)
{
   return(tab_BankFldInfo[field_no].type);
}

int GetBankFieldLength(int field_no)
{
   return(tab_BankFldInfo[field_no].length);
}

int GetBankTagIndice ( char * szTagName)
{
   int nIndice, nSize;
   char sLine[MAX_LINE_TRC];
   

   nSize = sizeof(tab_BankTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_BankTagInfo[nIndice].sTagName, 4) == 0)
	  {
         	return(nIndice);
      }
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}


int GetBankTagType ( int nIndice )
{
   return ( tab_BankTagInfo[nIndice].nType);
}

int GetBankTagLength ( int nIndice )
{
   return ( tab_BankTagInfo[nIndice].nLength);
}

