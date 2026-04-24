/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      rkl_ini.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      RKL Init Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <iso_rkl.h>
/*#include <header_rkl.h>*/
#include <define.h>


fld_info_t    tab_RklFldInfo[] =
{
   { RKL_FIX_BITMAP,  0},	/* BIT_MAP2			 */
   { RKL_VAR_BCD,	 19},   /* CARD_NBR 			 */
   { RKL_FIX_BCD,	  6},	/* PROC_CODE			 */
   { RKL_FIX_BCD,	 12},	/* TRANS_AMOUNT			 */
   { RKL_FIX_BCD,	 12},	/* STLMNT_AMOUNT		 */
   { RKL_FIX_BCD,	 12},	/* Cardholder Billing Amount	 */
   { RKL_FIX_BCD,	 10},	/* XMIT_TIME			 */
   { RKL_FIX_BCD,	  8},	/* Cardholder Billing Fee	 */
   { RKL_FIX_BCD,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { RKL_FIX_BCD,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { RKL_FIX_BCD,	  6},	/* AUDIT_NBR			 */
   { RKL_FIX_BCD,	  6},	/* TRANS_TIME			 */
   { RKL_FIX_BCD,	  4},	/* TRANS_DATE			 */
   { RKL_FIX_BCD,	  4},	/* EXPIRY_DATE			 */
   { RKL_FIX_BCD,	  4},	/* STLMNT_DATE			 */
   { RKL_FIX_BCD,	  4},	/* CNVRSN_DATE			 */
   { RKL_FIX_BCD,	  4},	/* CAPTURE_DATE			 */
   { RKL_FIX_BCD,	  4},	/* MERCHANT_TYPE		 */
   { RKL_FIX_BCD,	  3},	/* ACQR_COUNTRY			 */
   { RKL_FIX_BCD,	  3},	/* PAN_COUNTRY			 */

   { RKL_FIX_BCD,	  3},	/* FORWD_COUNTRY		 */
   { RKL_FIX_BCD,	  4},	/* POS_ENTRY_MODE		 */
   { RKL_FIX_BCD,	  3},	/* Card Sequence Number		 */
   { RKL_FIX_BCD,	  3},	/* Network International Identifier  */
   { RKL_FIX_BCD,	  2},	/* POS_CONDITION		 */
   { RKL_FIX_BCD,	  2},	/* POS_PIN_CAPTURE		 */
   { RKL_FIX_BCD,	  1},	/* Authorization Id Response Lenght */
   { RKL_FIX_ALPHA,	  9},	/* TRANS_FEE			 */
   { RKL_FIX_ALPHA,	  9},	/* Amount Settlement Fee	 */
   { RKL_FIX_ALPHA,	  9},	/* Amount Transaction Processing Fee */
	
   { RKL_FIX_ALPHA,	  9},	/* Amount Setlmnt Processing Fee */
   { RKL_VAR_BCD,	 11},	/* ACQR_ID			 */
   { RKL_VAR_BCD,	 11},	/* FORWD_ID			 */
   { RKL_VAR_BCD,	 28},	/* PAN_EXTENDED			 */
   /*** { RKL_VAR_BCD,	 37},***/	/* T2_DATA			 */
   { RKL_VAR_HEX,	 37},	/* T2_DATA			 */
   { RKL_VAR_BCD,	104},	/* Track 3 Data			 */
   { RKL_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { RKL_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { RKL_FIX_ALPHA,	  2},	/* RESPONSE_CODE		 */
   { RKL_FIX_ALPHA,	  3},	/* Service Restriction Code	 */

   { RKL_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { RKL_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { RKL_FIX_ALPHA,	 40},	/* TERMINAL_ADR			 */
   { RKL_VAR_ALPHA,	 25},	/* ADTNL_RESP_DATA		 */
   { RKL_VAR_ALPHA,	 77},	/* T1_DATA			 */
   { RKL_VAR_ALPHA,	255},	/* Additrional Data Iso		 */
   { RKL_VAR_ALPHA,	255},	/* Additrional Data National	 */
   { RKL_VAR_ALPHA,	255},	/* ADTNL_DATA_PRIV		 */
   { RKL_FIX_BCD,	  3},	/* TRANS_CRNCY			 */
   { RKL_FIX_BCD,	  3},	/* STLMNT_CRNCY			 */

   { RKL_FIX_BCD,	  3},	/* BILLING_CRNCY		 */
   { RKL_FIX_HEX,	 16},	/* PIN_DATA			 */
   { RKL_FIX_BCD,	 16},	/* SECURITY_DATA		 */
   { RKL_VAR_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { RKL_VAR_ALPHA,	255},	/* Reserved Iso			 */
   { RKL_VAR_ALPHA,	255},	/* Reserved Iso			 */
   { RKL_VAR_ALPHA,	255},	/* Reserved National		 */
   { RKL_VAR_ALPHA,	255},	/* Reserved National		 */
   { RKL_VAR_ALPHA,	 14},	/* GEOGR_DATA			 */
   { RKL_VAR_SBCD,	  6},	/* ADTNL_POS_INFO		 */
	
   { RKL_VAR_SBCD,	 36},	/* OTHER_AMOUNTS		 */
   { RKL_FIX_ALPHA, 59},	/* CPS_FIELDS			 */
   { RKL_VAR_BITMAP, 255},	/* PRIV_USE_FIELDS		 */
   { RKL_FIX_BIN,	  8},	/* MAC1				 */
   { NOT_USED,	  0},	/* 				 */
   { RKL_FIX_BCD,	  1},	/* STLMNT_CODE			 */
   { RKL_FIX_BCD,	  1},	/* Extended Payment Code	*/
   { RKL_FIX_BCD,	  3},	/* RECEIVER_COUNTRY		 */
   { RKL_FIX_BCD,	  3},	/* STLMNT_INST_COUNTRY		 */
   { RKL_FIX_BCD,	  3},	/* NW_MNGMT_INFO		 */

   { RKL_FIX_BCD,	  4},	/* Message Number		 */
   { RKL_FIX_BCD,	  4},	/* Message number Last		 */
   { RKL_FIX_BCD,	  6},	/* ACTION_DATE			 */
   { NOT_USED,	  0},	/* CR_NBR			 */
   { NOT_USED,	  0},	/* CR_REV_NBR			 */
   { NOT_USED,	  0},	/* DB_NBR			 */
   { NOT_USED,	  0},	/* DB_REV_NBR			 */
   { RKL_FIX_BCD,	 10},	/* Transfer Number		 */
   { RKL_FIX_BCD,	 10},	/* Transfer Reversal Number	 */
   { RKL_FIX_BCD,	 10},	/* Inquiries Number		 */

   { RKL_FIX_BCD,	 10},	/* Authorizations Number	 */
   { RKL_FIX_BCD,	 12},	/* Credit, Processing Fee Amount */
   { RKL_FIX_BCD,	 12},	/* Credit, Transaction Fee Amount */
   { RKL_FIX_BCD,	 12},	/* Debit, Processing Fee Amount	 */
   { RKL_FIX_BCD,	 12},	/* Debit, Transaction Fee Amount */
   { RKL_FIX_BCD,	 16},	/* CR_AMOUNT			 */
   { RKL_FIX_BCD,	 16},	/* CR_REV_AMOUNT		 */
   { RKL_FIX_BCD,	 16},	/* DB_AMOUNT			 */
   { RKL_FIX_BCD,	 16},	/* DB_REV_AMOUNT		 */
   { RKL_FIX_BCD,	 42},	/* ORIG_DATA			 */

   { RKL_FIX_ALPHA,	  1},	/* FILE_UPDATE			 */
   { RKL_FIX_ALPHA,	  2},	/* FILE_SECURITY		 */
   { RKL_FIX_ALPHA,	  5},	/* Response Indicator		 */
   { RKL_FIX_ALPHA,	  7},	/* Service Indicator		 */
   { RKL_FIX_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
   { RKL_FIX_BIN,	  8},	/* MSG_SECURITY			 */
   { RKL_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { NOT_USED,	  0},		/*      			 */
   { RKL_VAR_BCD,	 11},	/* STLMNT_INSTITN		 */
   { RKL_VAR_BCD,	 11},	/* RECEIVER_ID			 */

   { RKL_VAR_ALPHA,	 17},	/* FILE_NAME			 */
   { RKL_VAR_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { RKL_VAR_ALPHA,	 28},	/* ACCOUNT_ID2			 */ /* 103 */
   { RKL_VAR_ALPHA,	100},	/* Transaction Description	 */
   { NOT_USED,	  0},	/* Reserved Iso			 */
   { NOT_USED,	  0},	/* Reserved Iso			 */
   { NOT_USED,	  0},	/* Reserved Iso			 */
   { NOT_USED,	  0},	/* Reserved Iso			 */
   { NOT_USED,	  0},	/* Reserved Iso			 */
   { NOT_USED,	  0},	/* Reserved Iso			 */

   { NOT_USED,	  0},	/* Reserved Iso			 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { RKL_VAR_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Intra Country Data		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { RKL_VAR_BCD,	  4},	/* ORIGINAL MESSAGE TYPE ID	 */

   { NOT_USED,	  0},	/* Issuing Institution Id	 */
   { NOT_USED,	  0},	/* Remaining Open to Use	 */
   { RKL_VAR_ALPHA,	 29},	/* ADR_VERIF_DATA		 */
   { NOT_USED,	  0},	/* Free Form Text - Japan	 */
   { RKL_VAR_ALPHA,	255},	/* SUPPORT_INFO			 */
   /* { NOT_USED,	  0} ,*/	/* Reserved Private		 */
   { RKL_VAR_BITMAP,	 255},   /* VISA PRIVATE USE FIELD */
   { RKL_VAR_ALPHA,	  255},	/* FILE RECORD ACTION AND DATA	 */
   { RKL_FIX_BIN,	  8},	/* MAC2				 */
   { NOT_USED,	  0},	/* Field 129			 */
   { RKL_FIX_HEX,	  6},	/* Terminal Capability Profile   */ /* 130 */

   { RKL_FIX_BCD,	  10},	/* Terminal Verification Results */
   { RKL_FIX_BCD,	  8	},	/* Unpredictable Number          */
   { RKL_FIX_BCD,	  16},	/* Terminal Serial Number        */
   { RKL_VAR_BIN,	 16},	/* Rkl Discretionary Data       */
   { RKL_VAR_ALPHA,	 39},	/* Issuer Discretionary Data     */
   { RKL_FIX_HEX,	 16},	/* Issuer Discretionary Data     */
   { RKL_FIX_HEX,	  4},	/* Application Trans. Counter    */
   { RKL_FIX_HEX,	  4},	/* Application Inter. Profile    */
   { RKL_FIX_HEX,	 20},	/* ARPC + Response Code          */
   { NOT_USED,	  0},	/* Field 140			 */

   { NOT_USED,	  0},	/* Field 141			 */
   { RKL_VAR_HEX,	510},	/* Issuer Script                 */
   { RKL_VAR_HEX,	 40},	/* Issuer Script Results         */
   { RKL_FIX_BCD,	  2},	/* Cryptogram Trans Type         */
   { RKL_FIX_BCD,	  4},	/* Terminal Country Code         */
   { RKL_FIX_BCD,	  6},	/* Terminal Trans  Date          */
   { RKL_FIX_BCD,	 12},	/* Cryptogram Amount             */
   { RKL_FIX_BCD,	  4},	/* Cryptogram Currency Code      */
   { RKL_FIX_BCD,	 12},	/* Cryptogram Cash Back          */
   { RKL_FIX_HEX,	 16},	/* Cardholder Card Scheme Crypto.*/

   { NOT_USED,	  0},	/* Field 151			 */
   { NOT_USED,	  0},	/* Field 152			 */
   { NOT_USED,	  0},	/* Field 153			 */
   { NOT_USED,	  0},	/* Field 154			 */
   { NOT_USED,	  0},	/* Field 155			 */
   { NOT_USED,	  0},	/* Field 156			 */
   { NOT_USED,	  0},	/* Field 157			 */
   { NOT_USED,	  0},	/* Field 158			 */
   { NOT_USED,	  0},	/* Field 159			 */
   { NOT_USED,	  0},	/* Field 160			 */

   { NOT_USED,	  0},	/* Field 161			 */
   { NOT_USED,	  0},	/* Field 162			 */
   { NOT_USED,	  0},	/* Field 163			 */
   { NOT_USED,	  0},	/* Field 164			 */
   { NOT_USED,	  0},	/* Field 165			 */
   { NOT_USED,	  0},	/* Field 166			 */
   { NOT_USED,	  0},	/* Field 167			 */
   { NOT_USED,	  0},	/* Field 168			 */
   { NOT_USED,	  0},	/* Field 169			 */
   { NOT_USED,	  0},	/* Field 170			 */

   { NOT_USED,	  0},	/* Field 171			 */
   { NOT_USED,	  0},	/* Field 172			 */
   { NOT_USED,	  0},	/* Field 173			 */
   { NOT_USED,	  0},	/* Field 174			 */
   { NOT_USED,	  0},	/* Field 175			 */
   { NOT_USED,	  0},	/* Field 176			 */
   { NOT_USED,	  0},	/* Field 177			 */
   { NOT_USED,	  0},	/* Field 178			 */
   { NOT_USED,	  0},	/* Field 179			 */
   { NOT_USED,	  0},	/* Field 180			 */

   { NOT_USED,	  0},	/* Field 181			 */
   { NOT_USED,	  0},	/* Field 182			 */
   { NOT_USED,	  0},	/* Field 183			 */
   { NOT_USED,	  0},	/* Field 184			 */
   { NOT_USED,	  0},	/* Field 185			 */
   { NOT_USED,	  0},	/* Field 186			 */
   { NOT_USED,	  0},	/* Field 187			 */
   { NOT_USED,	  0},	/* Field 188			 */
   { NOT_USED,	  0},	/* Field 189			 */
   { NOT_USED,	  0},	/* Field 190			 */

   { NOT_USED,	  0},	/* Field 191			 */
   { RKL_FIX_BIN,	  8},	/* Message Authentication Code   */
};

void  InitRklInfo(TSRklInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_RKL_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < RKL_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_RKL_DATA_LEN);
   /*memset(( char *) & (msgInfo->sHeader), '\0', sizeof(TSRklHeader));*/
}

int GetRklFieldType(int field_no)
{
   return(tab_RklFldInfo[field_no].type);
}

int GetRklFieldLength(int field_no)
{
   return(tab_RklFldInfo[field_no].length);
}

