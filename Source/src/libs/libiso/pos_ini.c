#include <stdio.h>
#include <stdlib.h>
#include <iso_pos.h>
#include <define.h>
#include <posiso_define.h>


static fld_info_t    tab_PosFldInfo[] =
{
   { NOT_USED,  		0},	/* BIT_MAP1			 */
   { POS_LL_ALPHA,		POSISO_F002_MAX_LEN},   /* CARD_NBR 			 */
   { POS_FIX_ALPHA,		POSISO_F003_LEN},	/* PROC_CODE			 */
   { POS_FIX_ALPHA,		POSISO_F004_LEN},	/* TRANS_AMOUNT			 */
   { POS_FIX_ALPHA,		POSISO_F005_LEN},	/* STLMNT_AMOUNT		 */
   { NOT_USED,	 		12},	/* Cardholder Billing Amount	 */
   { POS_FIX_ALPHA,		POSISO_F007_LEN},	/* XMIT_TIME			 */
   { NOT_USED,	  		8},	/* Cardholder Billing Fee	 */
   { POS_FIX_ALPHA,		8},	/* CONVRSN_RATE_STLMNT		 */
   { NOT_USED,	  		8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { POS_FIX_ALPHA,		POSISO_F011_LEN},	/* AUDIT_NBR			 */
   { POS_FIX_ALPHA,		POSISO_F012_LEN},	/* TRANS_DATE_TIME               */
   { POS_FIX_ALPHA,	  4},	/* START_DATE			 */
   { POS_FIX_ALPHA,		POSISO_F014_LEN},	/* EXPIRY_DATE			 */
#ifdef PPWM_223
   { POS_FIX_ALPHA,	  4},	/* STLMNT_DATE			 */
#else
   { POS_FIX_ALPHA,		POSISO_F015_LEN},	/* STLMNT_DATE			 */ 
#endif
   { NOT_USED,	  	  4},	/* CNVRSN_DATE			 */
   { POS_FIX_ALPHA,		POSISO_F017_LEN},	/* CAPTURE_DATE			 */
   { NOT_USED,	          4},	/* MERCHANT_TYPE		 */
   { NOT_USED,	    	  3},	/* ACQR_COUNTRY			 */
   { NOT_USED,	          3},	/* PAN_COUNTRY			 */

   { NOT_USED,	          3},	/* FORWD_COUNTRY		 */
   { POS_FIX_ALPHA,		POSISO_F022_LEN},	/* POS_ENTRY_MODE		 */
   { POS_FIX_ALPHA,		POSISO_F023_LEN},	/* Card Sequence Number		 */
   { POS_FIX_ALPHA,		POSISO_F024_LEN},	/* Function Code                 */
   { POS_FIX_ALPHA,		POSISO_F025_LEN},	/* Message Reason Code           */
   { POS_FIX_ALPHA,		POSISO_F026_LEN},	/* MERCHANT TYPE  		 */
   { NOT_USED,		  1},	/* Authorization Id Response Lenght */
   { NOT_USED,		  6},	/* TRANS_FEE			 */
   { NOT_USED,		  3},	/* Amount Settlement Fee	 */
   /*{ NOT_USED,		 24},	/* Amount Transaction Processing Fee */
   { POS_FIX_ALPHA,      24},   /* REPLACEMENT_AMOUNTS     PLUTONL-2409*/
   { POS_FIX_HEX,        16},   /* NEW_PIN_DATA   ZKO:13092018 */
	
   /*{ NOT_USED,		  9},	/* Amount Setlmnt Processing Fee */
   { POS_LL_ALPHA,		POSISO_F032_MAX_LEN},	/* ACQR_ID			 */
   { NOT_USED,		 11},	/* FORWD_ID			 */
   { POS_LL_ALPHA,		POSISO_F034_MAX_LEN},	/* PAN_EXTENDED			 */
   { POS_LL_ALPHA,		POSISO_F035_MAX_LEN},	/* T2_DATA			 */
   { POS_LLL_ALPHA,	  4},	/* T1_DATA                       */
   { POS_FIX_ALPHA,		POSISO_F037_LEN},	/* REFERENCE_NBR		 */
   { POS_FIX_ALPHA,		POSISO_F038_LEN},	/* AUTHOR_ID			 */
   { POS_FIX_ALPHA,		POSISO_F039_LEN},	/* ACTION CODE  		 */
   { NOT_USED,		  3},	/* Service Restriction Code	 */

   { POS_FIX_ALPHA,		POSISO_F041_LEN},	/* TERMINAL_NBR			 */
   { POS_FIX_ALPHA,		POSISO_F042_LEN},	/* OUTLET_NBR			 */
   { POS_LL_ALPHA,		POSISO_F043_MAX_LEN},	/* TERMINAL_ADR			 */
   { POS_LLL_ALPHA,		POSISO_F044_MAX_LEN},	/* ADTNL_RESP_DATA		 */
   { POS_LL_ALPHA,	 77},	/* T1_DATA			 */
   { POS_LLL_ALPHA,	255},	/* Additrional Data Pos		 */
   { POS_LLL_ALPHA,	255},	/* Additrional Data National	 */
   { POS_LLL_ALPHA,		POSISO_F048_MAX_LEN},	/* ADTNL_DATA_PRIV		 */
   { POS_FIX_ALPHA,		POSISO_F049_LEN},	/* TRANS_CRNCY			 */
   { POS_FIX_ALPHA,		POSISO_F050_LEN},	/* STLMNT_CRNCY			 */

   { NOT_USED,		  3},	/* BILLING_CRNCY		 */
   { POS_FIX_HEX,		POSISO_F052_LEN},	/* PIN_DATA			 */
   { POS_LL_ALPHA,		POSISO_F053_MAX_LEN},	/* SECURITY_DATA		 */
   { POS_LLL_ALPHA,		POSISO_F054_MAX_LEN},	/* ADTNL_AMOUNTS		 */
   { POS_LLL_BIN,		POSISO_F055_MAX_LEN},	/* ICC Information               */
   { POS_LL_ALPHA,		POSISO_F056_MAX_LEN},	/* Orig Data Elements            */
   { NOT_USED,		  3},	/* Reserved National		 */
   { NOT_USED,		255},	/* Reserved National		 */
   { NOT_USED,		 14},	/* GEOGR_DATA			 */
   { POS_LLL_ALPHA,	  6},	/* ADTNL_POS_INFO		 */
	
   { POS_LLL_ALPHA,	 36},	/* OTHER_AMOUNTS		 */
   { POS_LLL_ALPHA, 	POSISO_F062_MAX_LEN},	/* Redemption Number		 */
   { POS_LLL_ALPHA, 	POSISO_F063_MAX_LEN},	/* Versions       		 */
   { POS_FIX_BIN,		POSISO_F064_LEN},	/* MAC1				 */ /*MKB 160806**/
   { NOT_USED,	  0},	/* 				 */
   { POS_LLL_ALPHA,	  1},	/* STLMNT_CODE			 */
   { POS_FIX_ALPHA,	  2},	/* Extended Payment Code	*/
   { POS_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		 */
   { POS_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		 */
   { POS_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */

   { POS_FIX_ALPHA,	  POSISO_F071_LEN},	/* Message Number		 */
   { POS_LLL_ALPHA,	POSISO_F072_MAX_LEN},	/* Data Record        		 */
   { POS_FIX_ALPHA,	  POSISO_F073_LEN},	/* ACTION_DATE			 */
   { POS_FIX_ALPHA,	 POSISO_F074_LEN},	/* CR_NBR			 */
   { POS_FIX_ALPHA,	 POSISO_F075_LEN},	/* CR_REV_NBR			 */
   { POS_FIX_ALPHA,	 POSISO_F076_LEN},	/* DB_NBR			 */
   { POS_FIX_ALPHA,	 POSISO_F077_LEN},	/* DB_REV_NBR			 */
   { POS_FIX_ALPHA,	 10},	/* Transfer Number		 */
   { POS_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
   { POS_FIX_ALPHA,	 10},	/* Inquiries Number		 */

   { POS_FIX_ALPHA,	 10},	/* Authorizations Number	 */
   { POS_FIX_ALPHA,	 10},	/* Credit, Processing Fee Amount */
   { POS_FIX_ALPHA,	 POSISO_F083_LEN},	/* Payment Number                 */
   { POS_FIX_ALPHA,	 POSISO_F084_LEN},	/* Payment Reversal Number     	 */
   { POS_FIX_ALPHA,	 10},	/* Debit, Transaction Fee Amount */
   { POS_FIX_ALPHA,	 POSISO_F086_LEN},	/* CR_AMOUNT			 */
   { POS_FIX_ALPHA,	 POSISO_F087_LEN},	/* CR_REV_AMOUNT		 */
   { POS_FIX_ALPHA,	 POSISO_F088_LEN},	/* DB_AMOUNT			 */
   { POS_FIX_ALPHA,	 POSISO_F089_LEN},	/* DB_REV_AMOUNT		 */
   { POS_FIX_ALPHA,	 10},	/* ORIG_DATA			 */

   { POS_FIX_ALPHA,	  3},	/* FILE_UPDATE			 */
   { POS_FIX_ALPHA,	  3},	/* FILE_SECURITY		 */
   { POS_LL_ALPHA,	 11},	/* Destination Inst ID Code      */
   { POS_LL_ALPHA,	 11},	/* Source Inst Id Code           */
   { POS_LL_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
   { POS_LLL_ALPHA,	 17},	/* Additional Key Mngmt data     */
   { POS_FIX_ALPHA,	 POSISO_F097_LEN},	/* NET_AMOUNT			 */
   { POS_FIX_ALPHA,	 25},	/* Payee			 */
   { POS_LL_ALPHA,	 11},	/* STLMNT_INSTITN		 */
   { POS_LL_ALPHA,	 11},	/* RECEIVER_ID			 */

   { POS_LL_ALPHA,	 POSISO_F101_MAX_LEN},	/* FILE_NAME			 */
   { POS_LL_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { POS_LL_ALPHA,	 28},	/* ACCOUNT_ID2			 */
   { POS_LLL_ALPHA,	 16},	/* Transaction Description	 */
   { POS_FIX_ALPHA,	 16},	/* Reserved Pos			 */
   { POS_FIX_ALPHA,	 16},	/* Reserved Pos			 */
   { POS_FIX_ALPHA,	 10},	/* Reserved Pos			 */
   { POS_FIX_ALPHA,	 10},	/* Reserved Pos			 */
   { POS_LL_ALPHA,	  0},	/* Reserved Pos			 */
/*#ifdef PPWM_223*/
	{ POS_LLL_BIN,	POSISO_F110_MAX_LEN},
/*#else
   { POS_LLL_ALPHA,	POSISO_F110_MAX_LEN},
#endif
   */
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
   { POS_FIX_BIN,	  POSISO_F128_LEN},	/* MAC2				 */ /*MKB 160806**/

};

static tag_info_t  tab_PosTagInfo[] = 
{
   { "7100",POS_TAG_VAR_BIN , 0},/* Issuer Script Template 1              */
   { "7200",POS_TAG_VAR_BIN , 0},/* Issuer Script Template 2              */
   { "8600",POS_TAG_VAR_BIN , 0},/* Issuer Script command                 */
   { "8200",POS_TAG_VAR_BIN , 0},/* Application Interchange Profile       */
   { "8400",POS_TAG_VAR_BIN , 0},/* Dedicated File Name                   */
   { "4F00",POS_TAG_VAR_BIN , 0},/* ICC Application Id                    */
   { "8A00",POS_TAG_VAR_BIN , 0},/* Authorization Response Code           */
   { "9100",POS_TAG_VAR_BIN , 0},/* Issuer Authentication Data            */
   { "9500",POS_TAG_VAR_BIN , 0},/* Terminal Verification Results         */
   { "9A00",POS_TAG_VAR_BIN , 0},/* Transaction Date                      */
   { "9C00",POS_TAG_VAR_BIN , 0},/* Transaction TYPE                      */
   { "5F2A",POS_TAG_VAR_BIN , 0},/* Transaction Currency Code             */
   { "9F02",POS_TAG_VAR_BIN , 0},/* Transaction Amount                    */
   { "9F03",POS_TAG_VAR_BIN , 0},/* Other Amount                          */
   { "9F09",POS_TAG_VAR_BIN , 0},/* Terminal Application Version Number   */
   { "9F10",POS_TAG_VAR_BIN , 0},/*  Issuer Application Data              */
   { "9F1A",POS_TAG_VAR_BIN , 0},/* Terminal Country Code                 */
   { "9F1E",POS_TAG_VAR_BIN , 0},/* Interface Device Serial Number        */
   { "9F26",POS_TAG_VAR_BIN , 0},/* Application cryptogram                */
   { "9F27",POS_TAG_VAR_BIN , 0},/* Cryptogram Information Data           */
   { "9F33",POS_TAG_VAR_BIN , 0},/* Terminal Capabilities                 */
   { "9F34",POS_TAG_VAR_BIN , 0},/* Cardholder Verification Method        */
   { "9F35",POS_TAG_VAR_BIN , 0},/* Terminal Type                         */
   { "9F36",POS_TAG_VAR_BIN , 0},/* Application Transaction Counter       */
   { "9F37",POS_TAG_VAR_BIN , 0},/* Unpredictable Number                  */
   { "9F41",POS_TAG_VAR_BIN , 0},/* Transaction Sequence Number           */
   { "9F53",POS_TAG_VAR_BIN , 0},/* Transaction Category Code             */
};

void  InitPosIcTag  (TSTagPos * msgInfo)
{
   int i;
   int nSize;

   nSize = sizeof(msgInfo->nPosTag)/sizeof(msgInfo->nPosTag[0]);
   for (i = 0; i < nSize; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }

   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', sizeof(msgInfo->sChipData));
}

void  InitPosInfo(TSPosInfo *msgInfo)
{
   int i;

   memset((char*)&msgInfo->nFieldPos,0,sizeof(msgInfo->nFieldPos));
   memset(msgInfo->sBitMap,0,sizeof(msgInfo->sBitMap));
   
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', sizeof(msgInfo->sData));
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

   nSize = sizeof(tab_PosTagInfo) /sizeof(tab_PosTagInfo[0]);
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


