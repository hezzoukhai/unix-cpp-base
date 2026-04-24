#include <stdio.h>
#include <stdlib.h>
#include <iso_hsid.h>
#include <hsid_define.h>

#include <define.h>
#include <p7_common.h>
#include <p7_globals.h>
#include <iso8583.h>


fld_info_t    tab_SidFldInfo[] =
{
   { NOT_USED,  	  0},	/* BIT_MAP1			 */
   { HSID_LL_ALPHA,	 19},   /* CARD_NBR 			 */
   { HSID_FIX_ALPHA,	  6},	/* PROC_CODE			 */
   { HSID_FIX_ALPHA,	 12},	/* TRANS_AMOUNT			 */
   { HSID_FIX_ALPHA,	 12},	/* STLMNT_AMOUNT		 */
   { HSID_FIX_ALPHA,	 12},	/* Cardholder Billing Amount	 */
   { HSID_FIX_ALPHA,	 10},	/* XMIT_TIME			 */
   { HSID_FIX_ALPHA,	  8},	/* Cardholder Billing Fee	 */
   { HSID_FIX_ALPHA,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { HSID_FIX_ALPHA,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { HSID_FIX_ALPHA,	  6},	/* AUDIT_NBR			 */
   { HSID_FIX_ALPHA,	 12},	/* TRANS_DATE_TIME               */
   { HSID_FIX_ALPHA,	  4},	/* TRANS_DATE			 */
   { HSID_FIX_ALPHA,	  4},	/* EXPIRY_DATE			 */
   { HSID_FIX_ALPHA,	  6},	/* STLMNT_DATE			 */
   { HSID_FIX_ALPHA,	  4},	/* CNVRSN_DATE			 */
   { HSID_FIX_ALPHA,	  4},	/* CAPTURE_DATE			 */
   { HSID_FIX_ALPHA,	  4},	/* MERCHANT_TYPE		 */
   { HSID_FIX_ALPHA,	  3},	/* ACQR_COUNTRY			 */
   { HSID_FIX_ALPHA,	  3},	/* PAN_COUNTRY			 */

   { HSID_FIX_ALPHA,	  3},	/* FORWD_COUNTRY		 */
   { HSID_FIX_ALPHA,	 12},	/* POS_ENTRY_MODE		 */
   { HSID_FIX_ALPHA,	  3},	/* Card Sequence Number		 */
   { HSID_FIX_ALPHA,	  3},	/* Function Code                 */
   { HSID_FIX_ALPHA,	  4},	/* POS_CONDITION		 */
   { HSID_FIX_ALPHA,	  4},	/* MERCHANT TYPE  		 */
   { HSID_FIX_ALPHA,	  1},	/* Authorization Id Response Lenght */
   { HSID_FIX_ALPHA,	  6},	/* TRANS_FEE			 */
   { HSID_FIX_ALPHA,	  3},	/* Amount Settlement Fee	 */
   { HSID_FIX_ALPHA,	 24},	/* Amount Transaction Processing Fee */

   { HSID_LL_ALPHA,	  9},	/* Amount Setlmnt Processing Fee */
   { HSID_LL_ALPHA,	 11},	/* ACQR_ID			 */
   { HSID_LL_ALPHA,	 11},	/* FORWD_ID			 */
   { HSID_LL_ALPHA,	 28},	/* PAN_EXTENDED			 */
   { HSID_LL_ALPHA,	 37},	/* T2_DATA			 */
   { HSID_LLL_ALPHA,	  4},	/* T1_DATA                       */
   { HSID_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { HSID_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { HSID_FIX_ALPHA,	  3},	/* ACTION CODE  		 */
   { HSID_FIX_ALPHA,	  3},	/* Service Restriction Code	 */

   { HSID_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { HSID_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { HSID_LL_ALPHA,	 99},	/* TERMINAL_ADR			 */
   { HSID_LLL_ALPHA,	999},	/* ADTNL_RESP_DATA		 */
   { HSID_LL_ALPHA,	 77},	/* T1_DATA			 */
   { HSID_LLL_ALPHA,	255},	/* Additrional Data Iso		 */
   { HSID_LLL_ALPHA,	255},	/* Additrional Data National	 */
   { HSID_LLL_ALPHA,	999},	/* ADTNL_DATA_PRIV		 */
   { HSID_FIX_ALPHA,	  3},	/* TRANS_CRNCY			 */
   { HSID_FIX_ALPHA,	  3},	/* STLMNT_CRNCY			 */

   { HSID_FIX_ALPHA,	  3},	/* BILLING_CRNCY		 */
   { HSID_FIX_ALPHA,	 16},	/* PIN_DATA			 */
   { HSID_LL_ALPHA,	 16},	/* SECURITY_DATA		 */
   { HSID_LLL_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { HSID_LLL_BIN,	255},	/* ICC Information               */
   { HSID_LL_ALPHA,	255},	/* Orig Data Elements            */
   { HSID_FIX_ALPHA,	  3},	/* Reserved National		 */
   { HSID_LL_ALPHA,	255},	/* Reserved National		 */
   { HSID_LLL_ALPHA,	 14},	/* GEOGR_DATA			 */
   { HSID_LLL_ALPHA,	  6},	/* ADTNL_POS_INFO		 */

   { HSID_LLL_ALPHA,	 999},	/* NETWORK POS DATA		 */
   { HSID_LLL_ALPHA, 	999},	/* Redemption Number		 */
   { HSID_LLL_ALPHA, 	999},	/* Versions       		 */
   { NOT_USED,	  8},	/* MAC1				 */
   { NOT_USED,	  0},	/* 				 */
   { HSID_LLL_ALPHA,	  1},	/* STLMNT_CODE			 */
   { HSID_FIX_ALPHA,	  2},	/* Extended Payment Code	*/
   { HSID_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		 */
   { HSID_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		 */
   { HSID_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */

   { HSID_FIX_ALPHA,	  8},	/* Message Number		 */
   { HSID_LLL_ALPHA,	999},	/* Data Record        		 */
   { HSID_FIX_ALPHA,	  6},	/* ACTION_DATE			 */
   { HSID_FIX_ALPHA,	 10},	/* CR_NBR			 */
   { HSID_FIX_ALPHA,	 10},	/* CR_REV_NBR			 */
   { HSID_FIX_ALPHA,	 10},	/* DB_NBR			 */
   { HSID_FIX_ALPHA,	 10},	/* DB_REV_NBR			 */
   { HSID_FIX_ALPHA,	 10},	/* Transfer Number		 */
   { HSID_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
   { HSID_FIX_ALPHA,	 10},	/* Inquiries Number		 */

   { HSID_FIX_ALPHA,	 10},	/* Authorizations Number	 */
   { HSID_FIX_ALPHA,	 10},	/* Credit, Processing Fee Amount */
   { HSID_FIX_ALPHA,	 10},	/* Payment Number                 */
   { HSID_FIX_ALPHA,	 10},	/* Payment Reversal Number     	 */
   { HSID_FIX_ALPHA,	 10},	/* Debit, Transaction Fee Amount */
   { HSID_FIX_ALPHA,	 16},	/* CR_AMOUNT			 */
   { HSID_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		 */
   { HSID_FIX_ALPHA,	 16},	/* DB_AMOUNT			 */
   { HSID_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		 */
   { HSID_FIX_ALPHA,	 35},	/* ORIG_DATA			 */

   { HSID_FIX_ALPHA,	  3},	/* FILE_UPDATE			 */
   { HSID_FIX_ALPHA,	  3},	/* FILE_SECURITY		 */
   { HSID_LL_ALPHA,	 11},	/* Destination Inst ID Code      */
   { HSID_LL_ALPHA,	 11},	/* Source Inst Id Code           */
   { HSID_LL_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
   { HSID_LLL_ALPHA,	 17},	/* Additional Key Mngmt data     */
   { HSID_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { HSID_FIX_ALPHA,	 25},	/* Payee			 */
   { HSID_LL_ALPHA,	 11},	/* STLMNT_INSTITN		 */
   { HSID_LL_ALPHA,	 11},	/* RECEIVER_ID			 */

   { HSID_LL_ALPHA,	 17},	/* FILE_NAME			 */
   { HSID_LL_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { HSID_LL_ALPHA,	 28},	/* ACCOUNT_ID2			 */
   { HSID_LLL_ALPHA,	 16},	/* Transaction Description	 */
   { HSID_FIX_ALPHA,	 16},	/* Reserved Iso			 */
   { HSID_FIX_ALPHA,	 16},	/* Reserved Iso			 */
   { HSID_FIX_ALPHA,	 10},	/* Reserved Iso			 */
   { HSID_FIX_ALPHA,	 10},	/* Reserved Iso			 */
   { HSID_LL_ALPHA,	  0},	/* Reserved Iso			 */
   { HSID_LL_ALPHA,	  0},	/* Reserved Iso			 */

   { HSID_LLL_ALPHA,	  0},	/* Reserved Iso			 */
   { HSID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { HSID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { HSID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { HSID_LLL_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
   { HSID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { HSID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { HSID_LLL_ALPHA,	  0},	/* Intra Country Data		 */
   { HSID_LLL_ALPHA,	  0},	/* Reserved National		 */
   { HSID_LLL_ALPHA,	  0},	/* Original Message Type Id	 */

   { HSID_LLL_ALPHA,	  0},	/* Issuing Institution Id	 */
   { HSID_LLL_ALPHA,	  0},	/* Remaining Open to Use	 */
   { HSID_LLL_ALPHA,	 29},	/* ADR_VERIF_DATA		 */
   { HSID_LLL_ALPHA,	  0},	/* Free Form Text - Japan	 */
   { HSID_LLL_ALPHA,	255},	/* SUPPORT_INFO			 */
   { HSID_LLL_ALPHA,	255},	/* Reserved Private		 */
   { HSID_LLL_ALPHA,	  0},	/* File Record Action and Data	 */
   { HSID_FIX_ALPHA,	  8},	/* MAC2				 */

};

static tag_info_t  tab_SidTagInfo[] =
{
   { "7100",HSID_TAG_VAR_BCD , 0},/* Issuer Script Template 1              */
   { "7200",HSID_TAG_VAR_BCD , 0},/* Issuer Script Template 2              */
   { "8600",HSID_TAG_VAR_BCD , 0},/* Issuer Script command                 */
   { "8200",HSID_TAG_VAR_HEX , 0},/* Application Interchange Profile       */
   { "8400",HSID_TAG_VAR_BCD , 0},/* Dedicated File Name                   */
   { "4F00",HSID_TAG_VAR_BCD , 0},/* ICC Application Id                    */
   { "8A00",HSID_TAG_VAR_BCD , 0},/* Authorization Response Code           */
   { "9500",HSID_TAG_VAR_BCD , 0},/* Terminal Verification Results         */
   { "9A00",HSID_TAG_VAR_BCD , 0},/* Transaction Date                      */
   { "9C00",HSID_TAG_VAR_BCD , 0},/* Transaction TYPE                      */
   { "5F2A",HSID_TAG_VAR_BCD , 0},/* Transaction Currency Code             */
   { "9F02",HSID_TAG_VAR_BCD , 0},/* Transaction Amount                    */
   { "9F03",HSID_TAG_VAR_BCD , 0},/* Other Amount                          */
   { "9F09",HSID_TAG_VAR_BCD , 0},/* Terminal Application Version Number   */
   { "9F10",HSID_TAG_VAR_HEX , 0},/*  Issuer Application Data              */
   { "9F1A",HSID_TAG_VAR_BCD , 0},/* Terminal Country Code                 */
   { "9F1E",HSID_TAG_VAR_ALPHA , 0},/* Interface Device Serial Number        */
   { "9F26",HSID_TAG_VAR_HEX , 0},/* Application cryptogram                */
   { "9F27",HSID_TAG_VAR_BCD , 0},/* Cryptogram Information Data           */
   { "9F33",HSID_TAG_VAR_HEX , 0},/* Terminal Capabilities                 */
   { "9F34",HSID_TAG_VAR_BCD , 0},/* Cardholder Verification Method        */
   { "9F35",HSID_TAG_VAR_BCD , 0},/* Terminal Type                         */
   { "9F36",HSID_TAG_VAR_HEX , 0},/* Application Transaction Counter       */
   { "9F37",HSID_TAG_VAR_HEX , 0},/* Unpredictable Number                  */
   { "9F41",HSID_TAG_VAR_BCD , 0},/* Transaction Sequence Number           */
   { "9F53",HSID_TAG_VAR_BCD , 0},/* Transaction Category Code             */
   { "9100",HSID_TAG_VAR_HEX , 0},/* Issuer Authentication Data            */
   { "9F54",HSID_TAG_VAR_HEX , 0},/* Application cryptogram  ARPC          */
};

void  InitHsidIcTag  (TSTagSid * msgInfo)
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

void  InitHsidInfo(TSSidInfo *msgInfo)
{
   int i;

   memset((char*)msgInfo->nFieldPos,0,sizeof(msgInfo->nFieldPos));
   memset((char*)msgInfo->sBitMap,0,sizeof(msgInfo->sBitMap));
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', sizeof(msgInfo->sData));
}

int GetHsidFieldType(int field_no)
{
   return(tab_SidFldInfo[field_no].type);
}

int GetHsidFieldLength(int field_no)
{
   return(tab_SidFldInfo[field_no].length);
}

int GetHsidTagIndice ( char * szTagName)
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

int GetHsidTagType ( int nIndice )
{
   return ( tab_SidTagInfo[nIndice].nType);
}

int GetHsidTagLength ( int nIndice )
{
   return ( tab_SidTagInfo[nIndice].nLength);
}

static const TlvInfoProperties	HsidPrivateTlvDataProp = {
3,3,TLV_LT_ASCII,TLV_LT_ASCII,HSID_F048_NB_FIELDS,
{
		{HSID_F048_P10_MAC_KEY_TAG, 1,5,0,2,0,33,0,"NULL","MAC_KEY_TAG","NULL"},
		{HSID_F048_P16_PIN_TRANSPORT_KEY_TAG, 1,5,0,2,0,33,0,"NULL","PIN_TRANSPORT_KEY_TAG","NULL"},
		{HSID_F048_P48_TCC_TAG, 1,5,0,0,0,1,0,"NULL","TCC_TAG","NULL"},
		{HSID_F048_P83_ADDRESS_VERIF_REP_TYPE_TAG, 1,5,0,2,0,2,0,"NULL","ADDRESS_VERIF_REP_TYPE_TAG","NULL"},
		{HSID_F048_P87_CVV_RESULT_CODE_TAG, 1,5,0,0,0,1,0,"NULL","CVV_RESULT_CODE_TAG","NULL"},
		{HSID_F048_P88_CRYPT_RESULT_CODE_TAG, 1,5,0,0,0,1,0,"NULL","CRYPT_RESULT_CODE_TAG","NULL"},
		{HSID_F048_P92_CVC2_DATA_TAG, 1,5,0,0,0,3,0,"NULL","CVC2_DATA_TAG","NULL"},
		{HSID_F048_P95_NETWORK_CODE_TAG, 1,5,0,0,0,2,0,"NULL","NETWORK_CODE_TAG","NULL"},
		{HSID_F048_P32_CODE_USAGE_TAG, 1,5,0,0,0,1,0,"NULL","CODE_USAGE_TAG","NULL"},
		{HSID_F048_P33_INDICATEUR_DOC_TAG, 1,5,0,0,0,1,0,"NULL","INDICATEUR_DOC_TAG","NULL"},
		{HSID_F048_P34_CHGBCK_REF_TAG, 1,5,0,0,0,6,0,"NULL","CHGBCK_REF_TAG","NULL"},
		{HSID_F048_P39_MCI_ON_BEHALF_SERV , 1,5,0,2,0,40,0,"NULL","MCI_ON_BEHALF_SERV ","NULL"},
		{HSID_F048_P40_NETWORK_ID_TAG, 1,5,0,0,0,4,0,"NULL","NETWORK_ID_TAG","NULL"},
		{HSID_F048_P51_MCI_ON_BEHALF_SERV, 1,5,0,2,0,40,0,"NULL","MCI_ON_BEHALF_SERV","NULL"},
		{HSID_F048_P52_MCI_PAN_MAPPING_INFO, 1,5,0,2,0,43,0,"NULL","MCI_PAN_MAPPING_INFO","NULL"},
		{HSID_F048_P63_MC_NETWORK_DATA_TAG, 1,5,0,0,0,12,0,"NULL","MC_NETWORK_DATA_TAG","NULL"},
		{HSID_F048_P77_TRANS_XID_TAG, 1,5,0,0,0,3,0,"NULL","TRANS_XID_TAG","NULL"},
		{HSID_F048_P61_POS_DATA_EURO_PVTAG, 1,5,0,0,0,1,0,"NULL","POS_DATA_EURO_PVTAG","NULL"},
		{HSID_F048_P82_ADDRESS_VERIF_REQ_TYPE_TAG, 1,5,0,2,0,2,0,"NULL","ADDRESS_VERIF_REQ_TYPE_TAG","NULL"},
		{HSID_F048_P90_AVS_DATA, 1,5,0,3,0,999,0,"NULL","AVS_DATA","NULL"},
		{HSID_F048_R01_CTLS_DEVICE_TYPE, 1,5,0,0,0,2,0,"NULL","CTLS_DEVICE_TYPE","NULL"},
		{HSID_F048_G03_POS_ENVIRONMENT_TAG, 1,5,0,0,0,1,0,"NULL","POS_ENVIRONMENT_TAG","NULL"},
		{HSID_F048_P22_POS_ENTRY_MODE_PVTAG, 1,5,0,0,0,4,0,"NULL","POS_ENTRY_MODE_PVTAG","NULL"},
		{HSID_F048_P25_POS_CONDITION_CODE_PVTAG, 1,5,0,0,0,2,0,"NULL","POS_CONDITION_CODE_PVTAG","NULL"},
		{HSID_F048_P60_ADTNL_POS_DATA_PVTAG, 1,5,0,2,0,99,0,"NULL","ADTNL_POS_DATA_PVTAG","NULL"},
		{HSID_F048_P20_ACRO_ATM_PURCHASE_TAG, 1,5,0,2,0,20,0,"NULL","ACRO_ATM_PURCHASE_TAG","NULL"},
		{HSID_F048_P21_MERCHANT_ADDITIONAL_DATA, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_LLL,LU_BYTE,256,PP_PRINT,"NULL","HSID_F048_P21_MERCHANT_ADDITIONAL_DATA","NULL"},	
		{HSID_F048_M32_MCI_ASSIGNED_ID, 1,5,0,2,0,6,0,"NULL","MCI_ASSIGNED_ID","NULL"},
		{HSID_F048_P31_MSG_REASON_CODE_TAG, 1,5,0,0,0,4,0,"NULL","MSG_REASON_CODE_TAG","NULL"},
		{HSID_F048_P35_SMS_MESSAGE_TEXT_TAG, 1,5,0,0,0,50,0,"NULL","SMS_MESSAGE_TEXT_TAG","NULL"},
		{HSID_F048_P58_DIGITAL_PAYMENT, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,28,PP_PRINT,"NULL","DIGITAL_PAYMENT","NULL"},
		{HSID_F048_N22_NET_VISA_EXT_STIP_REASON_CODE, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_LLL,LU_BYTE,1,PP_PRINT,"NULL","EXT_STIP_REASON_CODE","NULL"}, /* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/  
        {HSID_F048_N54_SECURITY_SERV_DATA, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_LLL,LU_BYTE,6,PP_PRINT,"NULL","SECURITY_SERV_DATA","NULL"},
		{HSID_F048_N55_FRAUD_SCORE, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_LLL,LU_BYTE,32,PP_PRINT,"NULL","HSID_F048_N55_FRAUD_SCORE","NULL"},	/*AMER20210416 PLUTONL-2985: MC Enh19.Q2 AN2122*/
		{HSID_F048_N56_MERCHANT_ADVICE_CODE, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","HSID_F048_N56_MERCHANT_ADVICE_CODE","NULL"},	/*AEA20210712 Enh21.Q4 AN4761*/
		{HSID_F048_N57_MC_TRACE_ID, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,15,PP_PRINT,"NULL","MC_TRACE_ID","NULL"},	/*AEA20210712 Enh21.Q4 AN4972*/
        {HSID_F048_S20_MONEY_TRANSFER_DATA, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,999,PP_PRINT,"NULL","HSID_F048_S20_MONEY_TRANSFER_DATA","NULL"},/*HAL PLUTONL-4154 4.2.4	AN 4972 */
		{HSID_F048_S26_ECOM_AUTHENTIF_DATA, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,999,PP_PRINT,"NULL","HSID_F048_S26_ECOM_AUTHENTIF_DATA","NULL"},/* HAL PROD00073823 20201302 Enh Q2.2020 Art 2941*/
		{HSID_F048_S27_PSD2_DATA, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,999,PP_PRINT,"NULL","HSID_F048_S27_PSD2_DATA","NULL"},/* HAL  20201803 Enh Q2.2020 Art 9.3.3*/ 
		{HSID_F048_P56_DCC_DATA, 1,5,0,0,0,1,0,"NULL","DCC_DATA","NULL"},
		/*Start AMER201211126 PLUTONL-3140*/
		{HSID_F048_P64_AUTH_ADD_INFO,FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_LLL,LU_BYTE,19,PP_PRINT,"NULL","HSID_F048_P64_AUTH_ADD_INFO","NULL"}, /*HAL PLUTONL-4154 4.2.4	AN 4972 */
        {HSID_F048_P66_ECOM_INFO,FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_LLL,LU_BYTE,19,PP_PRINT,"NULL","HSID_F048_P66_ECOM_INFO","NULL"},
		{HSID_F048_K06_DS_TRX_ID,FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,36,PP_PRINT,"NULL","HSID_F048_K06_DS_TRX_ID","NULL"},
		{HSID_F048_K17_ACS_TRX_ID,FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,36,PP_PRINT,"NULL","HSID_F048_K17_ACS_TRX_ID","NULL"},
		/*End AMER201211126 PLUTONL-3140*/
      {HSID_F048_P71_RECURRING_FLAG_TAG,FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_LLL,LU_BYTE,1,PP_PRINT,"NULL","HSID_F048_P71_RECURRING_FLAG_TAG","NULL"}, /* ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
      {HSID_F048_N58_TOKEN_PROGAM,FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","HSID_F048_N58_TOKEN_PROGAM","NULL"},   /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
      {HSID_F048_N59_MCI_MIT_CIT_INDICATOR,FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,4,PP_PRINT,"NULL","HSID_F048_N59_MCI_MIT_CIT_INDICATOR","NULL"},   /*IZL 03062022 PLUTONL-4810 ENH 22Q2 MC AN5524*/
},
"HsidPrivateTlvDataProp"
};


/******************************************************************************/

static const TlvInfoProperties	HsidCPSTlvDataProp = {
3,3,TLV_LT_ASCII,TLV_LT_ASCII,HSID_F062_NB_FIELDS,
{
		{HSID_F062_F01_CPS_ACI_TAG, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","CPS_ACI_TAG","NULL",NOT_REPEATABLE},
		{HSID_F062_F02_CPS_TRANS_ID_TAG, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,15,PP_PRINT,"NULL","CPS_TRANS_ID_TAG","NULL",NOT_REPEATABLE},
		{HSID_F062_F03_TCC_TAG, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,4,PP_PRINT,"NULL","TCC_TAG","NULL",NOT_REPEATABLE},
		{HSID_F062_F04_CPS_VALIDATION_DATE_TAG, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","CPS_VALIDATION_DATE_TAG","NULL",NOT_REPEATABLE},
		{HSID_F062_F05_CPS_MARKET_SPECIFIC_TAG, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","CPS_MARKET_SPECIFIC_TAG","NULL",NOT_REPEATABLE},
		{HSID_F062_F06_CPS_DURATION_TAG, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","CPS_DURATION_TAG","NULL",NOT_REPEATABLE},
		{HSID_F062_F19_CPS_MVV_TAG, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,10,PP_PRINT,"NULL","CPS_MVV_TAG","NULL",NOT_REPEATABLE},

},
"HsidCPSTlvDataProp"
};

/******************************************************************************/

static const TlvInfoProperties	HsidPOSDataProp = {
3,3,TLV_LT_ASCII,TLV_LT_ASCII,HSID_F061_NB_FIELDS,
{
		{HSID_F061_039_RESPONSE_CODE_TAG, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,1,PP_PRINT,"NULL","RESPONSE_CODE_TAG","NULL",NOT_REPEATABLE},
		{HSID_F061_061_ADTNL_POS_DATA1_TAG, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_LL,LU_BYTE,15,PP_PRINT,"NULL","ADTNL_POS_DATA1_TAG","NULL",NOT_REPEATABLE},

},
"HsidPOSDataProp"
};


/******************************************************************************/

void InitHsidPrivateDataTlv(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &HsidPrivateTlvDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/

/******************************************************************************/
void InitHsidCPSTlv(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &HsidCPSTlvDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
void InitHsidPOSTlv(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &HsidPOSDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/

/******************************************************************************/
