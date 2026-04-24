/*
-----------------------------------------------------------------------------------------------------------------------------------------
-- Description : 
	-------------------------------------------------------------------------------------------------------------------------------------
	
-----------------------------------------------------------------------------------------------------------------------------------------
-- Modification History : 
	-------------------------------------------------------------------------------------------------------------------------------------
	-- Version    Date          	Author       		Jalon                     Activity                          Comment
	-------------------------------------------------------------------------------------------------------------------------------------
	-- V3.0.0     29/12/2014    	HPS                                                                           	Initial version
-----------------------------------------------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <iso_uaeswitch.h>
#include <define.h>


fld_info_t    tab_UaeSwitchFldInfo[] =
{
   { UAESWITCH_FIX_BITMAP,  0},	/* BIT_MAP2			 */
   { UAESWITCH_VAR_BCD,	 19},   /* CARD_NBR 			 */
   { UAESWITCH_FIX_BCD,	  6},	/* PROC_CODE			 */
   { UAESWITCH_FIX_BCD,	 12},	/* TRANS_AMOUNT			 */
   { UAESWITCH_FIX_BCD,	 12},	/* STLMNT_AMOUNT		 */
   { UAESWITCH_FIX_BCD,	 12},	/* Cardholder Billing Amount	 */
   { UAESWITCH_FIX_BCD,	 10},	/* XMIT_TIME			 */
   { UAESWITCH_FIX_BCD,	  8},	/* Cardholder Billing Fee	 */
   { UAESWITCH_FIX_BCD,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { UAESWITCH_FIX_BCD,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */ /* 10 */

   { UAESWITCH_FIX_BCD,	  6},	/* AUDIT_NBR			 */
   { UAESWITCH_FIX_BCD,	  6},	/* TRANS_TIME			 */
   { UAESWITCH_FIX_BCD,	  4},	/* TRANS_DATE			 */
   { UAESWITCH_FIX_BCD,	  4},	/* EXPIRY_DATE			 */
   { UAESWITCH_FIX_BCD,	  4},	/* STLMNT_DATE			 */
   { UAESWITCH_FIX_BCD,	  4},	/* CNVRSN_DATE			 */
   { UAESWITCH_FIX_BCD,	  4},	/* CAPTURE_DATE			 */
   { UAESWITCH_FIX_BCD,	  4},	/* MERCHANT_TYPE		 */
   { UAESWITCH_FIX_BCD,	  3},	/* ACQR_COUNTRY			 */
   { UAESWITCH_FIX_BCD,	  3},	/* PAN_COUNTRY			 */ /* 20 */

   { UAESWITCH_FIX_BCD,	  3},	/* FORWD_COUNTRY		 */
   { UAESWITCH_FIX_BCD,	  3},	/* POS_ENTRY_MODE		 */
   { UAESWITCH_FIX_BCD,	  3},	/* Card Sequence Number		 */
   { UAESWITCH_FIX_BCD,	  3},	/* Network International Identifier  */
   { UAESWITCH_FIX_BCD,	  2},	/* POS_CONDITION		 */
   { UAESWITCH_FIX_BCD,	  2},	/* POS_PIN_CAPTURE		 */
   { UAESWITCH_FIX_BCD,	  1},	/* Authorization Id Response Lenght */
   { UAESWITCH_FIX_ALPHA,	  9},	/* TRANS_FEE			 */
   { UAESWITCH_FIX_ALPHA,	  9},	/* Amount Settlement Fee	 */
   { UAESWITCH_FIX_ALPHA,	  9},	/* Amount Transaction Processing Fee */ /* 30 */
	
   { UAESWITCH_FIX_ALPHA,	  9},	/* Amount Setlmnt Processing Fee */
   { UAESWITCH_VAR_BCD,	 11},	/* ACQR_ID			 */
   { UAESWITCH_VAR_BCD,	 11},	/* FORWD_ID			 */
   { UAESWITCH_VAR_BCD,	 28},	/* PAN_EXTENDED			 */
   { UAESWITCH_VAR_HEX,	 37},	/* T2_DATA			 */
   { UAESWITCH_VAR_BCD,	104},	/* Track 3 Data			 */
   { UAESWITCH_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { UAESWITCH_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { UAESWITCH_FIX_ALPHA,	  2},	/* RESPONSE_CODE		 */
   { UAESWITCH_FIX_ALPHA,	  3},	/* Service Restriction Code	 */ /* 40 */

   { UAESWITCH_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { UAESWITCH_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { UAESWITCH_FIX_ALPHA,	 40},	/* TERMINAL_ADR			 */
   { UAESWITCH_VAR_ALPHA,	 25},	/* ADTNL_RESP_DATA		 */
   { UAESWITCH_VAR_ALPHA,	 77},	/* T1_DATA			 */
   { UAESWITCH_VAR_ALPHA,	255},	/* Additrional Data Iso		 */
   { UAESWITCH_VAR_HEX,		255},	/* Additrional Data National	 */
   { UAESWITCH_VAR_ALPHA,	255},	/* ADTNL_DATA_PRIV		 */
   { UAESWITCH_FIX_BCD,	  3},	/* TRANS_CRNCY			 */
   { UAESWITCH_FIX_BCD,	  3},	/* STLMNT_CRNCY			 */   /* 50 */

   { UAESWITCH_FIX_BCD,	  3},	/* BILLING_CRNCY		 */
   { UAESWITCH_FIX_HEX,	 16},	/* PIN_DATA			 */
   { UAESWITCH_FIX_BCD,	 16},	/* SECURITY_DATA		 */
   { UAESWITCH_VAR_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { UAESWITCH_VAR_BIN,		512},	/* ICC Information               */   		/*EMV UPGRADE OBE05082009*/
   /***{ UAESWITCH_VAR_ALPHA,	255},***/	/* Reserved Iso			 */
   { UAESWITCH_VAR_ALPHA,	255},	/* Reserved Iso			 */
   { UAESWITCH_FIX_ALPHA,	6},	/* Reserved National*/				/*EMV UPGRADE OBE05082009*/
   /*** { UAESWITCH_VAR_ALPHA,	255},***/	/* Reserved National		 */
   { UAESWITCH_VAR_ALPHA,	255},	/* Reserved National		 */
   { UAESWITCH_VAR_ALPHA,	 14},	/* GEOGR_DATA			 */
   { UAESWITCH_VAR_SBCD,	  6},	/* ADTNL_POS_INFO		 */ /* 60 */
	
   { UAESWITCH_VAR_SBCD,	 36},	/* OTHER_AMOUNTS		 */
   { UAESWITCH_VAR_BITMAP, 59},	/* CPS_FIELDS			 */
   { UAESWITCH_VAR_BITMAP, 255},	/* PRIV_USE_FIELDS		 */
   { UAESWITCH_FIX_BIN,	  8},	/* MAC1				 */
   { NOT_USED,	  0},	/* 				 */
   { UAESWITCH_FIX_BCD,	  1},	/* STLMNT_CODE			 */
   { UAESWITCH_FIX_BCD,	  1},	/* Extended Payment Code	*/
   { UAESWITCH_FIX_BCD,	  3},	/* RECEIVER_COUNTRY		 */
   { UAESWITCH_FIX_BCD,	  3},	/* STLMNT_INST_COUNTRY		 */
   { UAESWITCH_FIX_BCD,	  3},	/* NW_MNGMT_INFO		 */ /* 70 */

   { UAESWITCH_FIX_BCD,	  4},	/* Message Number		 */
   { UAESWITCH_FIX_BCD,	  4},	/* Message number Last		 */
   { UAESWITCH_FIX_BCD,	  6},	/* ACTION_DATE			 */
   { UAESWITCH_FIX_BCD,	  10},	/* CR_NBR			 */
   { UAESWITCH_FIX_BCD,	  10},	/* CR_REV_NBR			 */
   { UAESWITCH_FIX_BCD,	  10},	/* DB_NBR			 */
   { UAESWITCH_FIX_BCD,	  10},	/* DB_REV_NBR			 */
   { UAESWITCH_FIX_BCD,	 10},	/* Transfer Number		 */
   { UAESWITCH_FIX_BCD,	 10},	/* Transfer Reversal Number	 */
   { UAESWITCH_FIX_BCD,	 10},	/* Inquiries Number		 */ /* 80 */

   { UAESWITCH_FIX_BCD,	 10},	/* Authorizations Number	 */
   { UAESWITCH_FIX_BCD,	 12},	/* Credit, Processing Fee Amount */
   { UAESWITCH_FIX_BCD,	 12},	/* Credit, Transaction Fee Amount */
   { UAESWITCH_FIX_BCD,	 12},	/* Debit, Processing Fee Amount	 */
   { UAESWITCH_FIX_BCD,	 12},	/* Debit, Transaction Fee Amount */
   { UAESWITCH_FIX_BCD,	 16},	/* CR_AMOUNT			 */
   { UAESWITCH_FIX_BCD,	 16},	/* CR_REV_AMOUNT		 */
   { UAESWITCH_FIX_BCD,	 16},	/* DB_AMOUNT			 */
   { UAESWITCH_FIX_BCD,	 16},	/* DB_REV_AMOUNT		 */
   { UAESWITCH_FIX_BCD,	 42},	/* ORIG_DATA			 */ /* 90 */

   { UAESWITCH_FIX_ALPHA,	  1},	/* FILE_UPDATE			 */
   { UAESWITCH_FIX_ALPHA,	  2},	/* FILE_SECURITY		 */
   { UAESWITCH_FIX_ALPHA,	  5},	/* Response Indicator		 */
   { UAESWITCH_FIX_ALPHA,	  7},	/* Service Indicator		 */
   { UAESWITCH_FIX_BCD,	 42},	/* REPLACE_AMOUNTS		 */
   { UAESWITCH_FIX_BIN,	  8},	/* MSG_SECURITY			 */
   { UAESWITCH_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { NOT_USED,	  0},		/*      			 */
   { UAESWITCH_VAR_BCD,	 11},	/* STLMNT_INSTITN		 */
   { UAESWITCH_VAR_BCD,	 11},	/* RECEIVER_ID			 */ /* 100 */

   { UAESWITCH_VAR_ALPHA,	 17},	/* FILE_NAME			 */
   { UAESWITCH_VAR_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { UAESWITCH_VAR_ALPHA,	 28},	/* ACCOUNT_ID2			 */ /* 103 */
   { UAESWITCH_VAR_ALPHA,	100},	/* Transaction Description	 */
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
   { UAESWITCH_VAR_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { NOT_USED,	  0},	/* Intra Country Data		 */
   { NOT_USED,	  0},	/* Reserved National		 */
   { UAESWITCH_VAR_BCD,	  4},	/* ORIGINAL MESSAGE TYPE ID	 */

   { NOT_USED,	  0},	/* Issuing Institution Id	 */
   { UAESWITCH_VAR_ALPHA,	  999},	/* Remaining Open to Use	 */
   { UAESWITCH_VAR_ALPHA,	 999},	/* ADR_VERIF_DATA		 */
   { NOT_USED,	  0},	/* Free Form Text - Japan	 */
   { UAESWITCH_VAR_ALPHA,	255},	/* SUPPORT_INFO			 */
   /* { NOT_USED,	  0} ,*/	/* Reserved Private		 */
   { UAESWITCH_VAR_BITMAP,	 255},   /* UAESWITCH PRIVATE USE FIELD */
   { UAESWITCH_VAR_ALPHA,	  255},	/* FILE RECORD ACTION AND DATA	 */
   { UAESWITCH_FIX_BIN,	  8},	/* MAC2				 */
   { NOT_USED,	  0},	/* Field 129			 */
   { UAESWITCH_FIX_HEX,	  6},	/* Terminal Capability Profile   */ /* 130 */

   { UAESWITCH_FIX_BCD,	  10},	/* Terminal Verification Results */
   { UAESWITCH_FIX_BCD,	  8	},	/* Unpredictable Number          */
   { UAESWITCH_FIX_BCD,	  16},	/* Terminal Serial Number        */
   { UAESWITCH_VAR_BIN,	 16},	/* UaeSwitch Discretionary Data       */
   { UAESWITCH_VAR_ALPHA,	 39},	/* Issuer Discretionary Data     */
   { UAESWITCH_FIX_HEX,	 16},	/* Issuer Discretionary Data     */
   { UAESWITCH_FIX_HEX,	  4},	/* Application Trans. Counter    */
   { UAESWITCH_FIX_HEX,	  4},	/* Application Inter. Profile    */
   { UAESWITCH_FIX_HEX,	 20},	/* ARPC + Response Code          */
   { NOT_USED,	  0},	/* Field 140			 */

   { NOT_USED,	  0},	/* Field 141			 */
   { UAESWITCH_VAR_HEX,	510},	/* Issuer Script                 */
   { UAESWITCH_VAR_HEX,	 40},	/* Issuer Script Results         */
   { UAESWITCH_FIX_BCD,	  2},	/* Cryptogram Trans Type         */
   { UAESWITCH_FIX_BCD,	  4},	/* Terminal Country Code         */
   { UAESWITCH_FIX_BCD,	  6},	/* Terminal Trans  Date          */
   { UAESWITCH_FIX_BCD,	 12},	/* Cryptogram Amount             */
   { UAESWITCH_FIX_BCD,	  4},	/* Cryptogram Currency Code      */
   { UAESWITCH_FIX_BCD,	 12},	/* Cryptogram Cash Back          */
   { UAESWITCH_FIX_HEX,	 16},	/* Cardholder Card Scheme Crypto.*/

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
   { UAESWITCH_FIX_BIN,	  8},	/* Message Authentication Code   */
};
tag_info_t  tab_UaeSwitchTagInfo[] =
{
   { "7100",UAESWITCH_TAG_VAR_HEX , 0},/* Issuer Script Template 1              */
   { "7200",UAESWITCH_TAG_VAR_HEX , 0},/* Issuer Script Template 2              */
   { "8600",UAESWITCH_TAG_VAR_BCD , 0},/* Issuer Script command                 */
   { "8200",UAESWITCH_TAG_VAR_HEX , 0},/* Application Interchange Profile       */
   { "8400",UAESWITCH_TAG_VAR_BCD , 0},/* Dedicated File Name                   */
   { "4F00",UAESWITCH_TAG_VAR_BCD , 0},/* ICC Application Id                    */
   { "8A00",UAESWITCH_TAG_VAR_HEX , 0},/* Authorization Response Code           */
   { "9500",UAESWITCH_TAG_VAR_HEX , 0},/* Terminal Verification Results         */
   { "9100",UAESWITCH_TAG_VAR_HEX , 0},/* Terminal Verification Results         */
   { "9A00",UAESWITCH_TAG_VAR_BCD , 0},/* Transaction Date                      */
   { "9C00",UAESWITCH_TAG_VAR_BCD , 0},/* Transaction TYPE                      */
   { "5F2A",UAESWITCH_TAG_VAR_BCD , 0},/* Transaction Currency Code             */
   { "5F34",UAESWITCH_TAG_VAR_BCD , 0},/* PAN Sequence Numnber                  */
   { "9F02",UAESWITCH_TAG_VAR_BCD , 0},/* Transaction Amount                    */
   { "9F03",UAESWITCH_TAG_VAR_BCD , 0},/* Other Amount                          */
   { "9F09",UAESWITCH_TAG_VAR_BCD , 0},/* Terminal Application Version Number   */
   { "9F10",UAESWITCH_TAG_VAR_HEX , 0},/*  Issuer Application Data              */
   { "9F1A",UAESWITCH_TAG_VAR_BCD , 0},/* Terminal Country Code                 */
   { "9F1E",UAESWITCH_TAG_VAR_HEX , 0},/* Interface Device Serial Number        */
   { "9F26",UAESWITCH_TAG_VAR_HEX , 0},/* Application cryptogram                */
   { "9F27",UAESWITCH_TAG_VAR_BCD , 0},/* Cryptogram Information Data           */
   { "9F33",UAESWITCH_TAG_VAR_HEX , 0},/* Terminal Capabilities                 */
   { "9F34",UAESWITCH_TAG_VAR_HEX , 0},/* Cardholder Verification Method        */
   { "9F35",UAESWITCH_TAG_VAR_BCD , 0},/* Terminal Type                         */
   { "9F36",UAESWITCH_TAG_VAR_HEX , 0},/* Application Transaction Counter       */
   { "9F37",UAESWITCH_TAG_VAR_HEX , 0},/* Unpredictable Number                  */
   { "9F41",UAESWITCH_TAG_VAR_BCD , 0},/* Transaction Sequence Number           */
   { "9F53",UAESWITCH_TAG_VAR_BCD , 0},/* Transaction Category Code             */
   { "9100",UAESWITCH_TAG_VAR_HEX , 0},/* Issuer Authentication Data            */
   { "9F54",UAESWITCH_TAG_VAR_HEX , 0},/* Application cryptogram  ARPC          */
   { "5A00",UAESWITCH_TAG_VAR_ALPHA , 0},/* Application PAN							  */
   { "9F06",UAESWITCH_TAG_VAR_HEX , 0},  /* Application Identifier   			  */
   { "9F12",UAESWITCH_TAG_VAR_ALPHA , 0},/* Application Prefered Name			  */
   { "5700",UAESWITCH_TAG_VAR_HEX , 0}, /* Application Track2   *****************/
   { "8C00",UAESWITCH_TAG_VAR_ALPHA , 0},/* Security Attributes *****************/  
   { "5000",UAESWITCH_TAG_VAR_ALPHA , 0}, /*  Application Label ****************/

}; /***EMV UPGRADE OBE05082009***/
void  InitUaeSwitchInfo(TSUaeSwitchInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_UAESWITCH_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < UAESWITCH_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_UAESWITCH_DATA_LEN);
   memset(( char *) & (msgInfo->sHeader), '\0', UAESWITCH_HEADER_LEN);
}

int GetUaeSwitchFieldType(int field_no)
{
   return(tab_UaeSwitchFldInfo[field_no].type);
}

int GetUaeSwitchFieldLength(int field_no)
{
   return(tab_UaeSwitchFldInfo[field_no].length);
}


int GetUaeSwitchTagIndice ( char *szTagName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_UaeSwitchTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_UaeSwitchTagInfo[nIndice].sTagName, 4) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetUaeSwitchTagType ( int nIndice )
{
   return ( tab_UaeSwitchTagInfo[nIndice].nType);
}
int GetUaeSwitchTagLength ( int nIndice )
{
   return ( tab_UaeSwitchTagInfo[nIndice].nLength);
}
void  InitUaeSwitchIcTag  (TSTagUaeSwitch *msgInfo)
{
   int i;

   for (i = 0; i < MAX_UAESWITCH_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', MAX_UAESWITCH_CHIP_LEN);
}
