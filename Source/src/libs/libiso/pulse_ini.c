#include <stdio.h>
#include <stdlib.h>
#include <define.h>
#include <pulse/iso_pulse.h>
#include <pulse/pulse_privfields.h>
#include <pulse/pulse_define.h>
#include <iso8583.h>

static fld_info_t    tab_PulseFldInfo[] =
{

   { NOT_USED,  	  	  0},	/* BIT_MAP1			 */
   { PULSE_LL_ALPHA,	 19},   /* CARD_NBR 			 */
   { PULSE_FIX_ALPHA,	  6},	/* PROC_CODE			 */
   { PULSE_FIX_ALPHA,	 12},	/* TRANS_AMOUNT			 */
   { PULSE_FIX_ALPHA,	 12},	/* STLMNT_AMOUNT		 */
   { PULSE_FIX_ALPHA,	 12},	/* Cardholder Billing Amount	 */
   { PULSE_FIX_ALPHA,	 10},	/* XMIT_TIME			 */
   { PULSE_FIX_ALPHA,	  8},	/* Cardholder Billing Fee	 */
   { PULSE_FIX_ALPHA,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { PULSE_FIX_ALPHA,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { PULSE_FIX_ALPHA,	  6},	/* AUDIT_NBR			 */
   { PULSE_FIX_ALPHA,	  6},	/* TRANS_TIME                    */
   { PULSE_FIX_ALPHA,	  4},	/* TRANS_DATE			 */
   { PULSE_FIX_ALPHA,	  4},	/* EXPIRY_DATE			 */
   { PULSE_FIX_ALPHA,	  4},	/* STLMNT_DATE			 */ /* 15 */
   { PULSE_FIX_ALPHA,	  4},	/* CNVRSN_DATE			 */
   { PULSE_FIX_ALPHA,	  4},	/* CAPTURE_DATE			 */
   { PULSE_FIX_ALPHA,	  4},	/* MERCHANT_TYPE		 */
   { PULSE_FIX_ALPHA,	  3},	/* ACQR_COUNTRY			 */
   { PULSE_FIX_ALPHA,	  3},	/* PAN_COUNTRY			 */

   { PULSE_FIX_ALPHA,	  3},	/* FORWD_COUNTRY		 */
   { PULSE_FIX_ALPHA,	  3},	/* POS_ENTRY_MODE		 */
   { PULSE_FIX_ALPHA,	  3},	/* Card Sequence Number		 */
   { PULSE_FIX_ALPHA,	  3},	/* Function Code                 */
   { PULSE_FIX_ALPHA,	  2},	/* POS_CONDITION		 */
   { PULSE_FIX_ALPHA,	  2},	/* MERCHANT TYPE  		 */
   { PULSE_FIX_ALPHA,	  1},	/* Authorization Id Response Lenght */
   { PULSE_FIX_ALPHA,	  9},	/* TRANS_FEE			 */
   { PULSE_FIX_ALPHA,	  9},	/* Amount Settlement Fee	 */
   { PULSE_FIX_ALPHA,	  9},	/* Amount Transaction Processing Fee */

   { PULSE_FIX_ALPHA,	  9},	/* Amount Setlmnt Processing Fee */
   { PULSE_LL_ALPHA,	 11},	/* ACQR_ID			 */
   { PULSE_LL_ALPHA,	 11},	/* FORWD_ID			 */
   { PULSE_LL_ALPHA,	 28},	/* PAN_EXTENDED			 */
   { PULSE_LL_ALPHA,	 37},	/* T2_DATA			 */
   { PULSE_LLL_ALPHA,	  4},	/* T1_DATA                       */
   { PULSE_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { PULSE_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { PULSE_FIX_ALPHA,	  2},	/* ACTION CODE  		 */
   { PULSE_FIX_ALPHA,	  3},	/* Service Restriction Code	 */

   { PULSE_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { PULSE_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { PULSE_FIX_ALPHA,	 40},	/* TERMINAL_ADR			 */
   { PULSE_LL_ALPHA,	 25},	/* ADTNL_RESP_DATA		 */
   { PULSE_LL_ALPHA,	 77},	/* T1_DATA			 */
   { PULSE_LLL_ALPHA,	255},	/* Additrional Data Pulse		 */
   { PULSE_LLL_ALPHA,	255},	/* Additrional Data National	 */
   { PULSE_LLL_ALPHA,	999},	/* ADTNL_DATA_PRIV		 */
   { PULSE_FIX_ALPHA,	  3},	/* TRANS_CRNCY			 */
   { PULSE_FIX_ALPHA,	  3},	/* STLMNT_CRNCY			 */

   { PULSE_FIX_ALPHA,	  3},	/* BILLING_CRNCY		 */
   { PULSE_FIX_ALPHA,	 16},	/* PIN_DATA			 */
   { PULSE_FIX_ALPHA,	 16},	/* SECURITY_DATA		 */
   { PULSE_LLL_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { PULSE_LLL_BIN,	255},	/* ICC Information               */ /* 55 */
   { PULSE_LLL_ALPHA,	255},	/* Orig Data Elements            */
   { PULSE_LLL_ALPHA,	  3},	/* Reserved National		 */
   { PULSE_LLL_ALPHA,	 11},	/* PULSE_AUTHOR_AGENT_ID		 */ /* 58 */
   { PULSE_LLL_ALPHA,	 17},	/* GEOGR_DATA			 */   /* 59 */
   { PULSE_LLL_ALPHA/*PULSE_VAR_BITMAP*/,	  255},	/* ADTNL_POS_INFO		 */ /* 60 */

   { PULSE_LLL_ALPHA,	 36},	/* OTHER_AMOUNTS		 */
   { PULSE_LLL_ALPHA, 	999},	/* Redemption Number		 */
   { PULSE_LLL_ALPHA/*PULSE_VAR_BITMAP*/, 	255},	/* Versions       		 */ /* 63 */
 /*  { PULSE_LLL_ALPHA,   255}, */  /*63 for test            */
   { NOT_USED,	  	  8},	/* MAC1				 */
   { NOT_USED,	  	  0},	/* 				 */
   { PULSE_FIX_ALPHA,	  1},	/* STLMNT_CODE			 */
   { PULSE_FIX_ALPHA,	  2},	/* Extended Payment Code	*/
   { PULSE_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		 */
   { PULSE_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		 */
   { PULSE_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */ /*70*/

   { PULSE_FIX_ALPHA,	  4},	/* Message Number		 */
   { PULSE_FIX_ALPHA,	  4},	/* Data Record        		 */
   { PULSE_FIX_ALPHA,	  6},	/* ACTION_DATE			 */
   { PULSE_FIX_ALPHA,	 10},	/* CR_NBR			 */
   { PULSE_FIX_ALPHA,	 10},	/* CR_REV_NBR			 */
   { PULSE_FIX_ALPHA,	 10},	/* DB_NBR			 */
   { PULSE_FIX_ALPHA,	 10},	/* DB_REV_NBR			 */
   { PULSE_FIX_ALPHA,	 10},	/* Transfer Number		 */
   { PULSE_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
   { PULSE_FIX_ALPHA,	 10},	/* Inquiries Number		 */

   { PULSE_FIX_ALPHA,	 10},	/* Authorizations Number	 */
   { PULSE_FIX_ALPHA,	 12},	/* Credit, Processing Fee Amount */
   { PULSE_FIX_ALPHA,	 12},	/* Payment Number                 */
   { PULSE_FIX_ALPHA,	 12},	/* Payment Reversal Number     	 */
   { PULSE_FIX_ALPHA,	 12},	/* Debit, Transaction Fee Amount */
   { PULSE_FIX_ALPHA,	 16},	/* CR_AMOUNT			 */
   { PULSE_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		 */
   { PULSE_FIX_ALPHA,	 16},	/* DB_AMOUNT			 */
   { PULSE_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		 */
   { PULSE_FIX_ALPHA,	 42},	/* ORIG_DATA			 */

   { PULSE_FIX_ALPHA,	  1},	/* FILE_UPDATE			 */
   { PULSE_FIX_ALPHA,	  2},	/* FILE_SECURITY		 */ /* 92 */
   { PULSE_FIX_ALPHA,	  5},	/* Destination Inst ID Code      */
   { PULSE_FIX_ALPHA,	  7},	/* Source Inst Id Code           */
   { PULSE_FIX_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */ /* 95 */
   { PULSE_FIX_ALPHA  ,	 8},	/* Additional Key Mngmt data    */  /* 96 */
   { PULSE_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { PULSE_FIX_ALPHA,	 25},	/* Payee			 */
   { PULSE_LL_ALPHA,	 11},	/* STLMNT_INSTITN		 */
   { PULSE_LL_ALPHA,	 11},	/* RECEIVER_ID			 */

   { PULSE_LL_ALPHA,	 17},	/* FILE_NAME			 */
   { PULSE_LL_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { PULSE_LL_ALPHA,	 28},	/* ACCOUNT_ID2			 */
   { PULSE_LLL_ALPHA,	 16},	/* Transaction Description	 */
   { PULSE_LLL_ALPHA,	 16},	/* Reserved Pulse			 */
   { PULSE_LLL_ALPHA,	 16},	/* Reserved Pulse			 */
   { PULSE_LLL_ALPHA,	 10},	/* Reserved Pulse			 */
   { PULSE_LLL_ALPHA,	 10},	/* Reserved Pulse			 */
   { PULSE_LLL_ALPHA,	  0},	/* Reserved Pulse			 */
   { PULSE_LLL_ALPHA,	  0},	/* Reserved Pulse			 */

   { PULSE_LLL_ALPHA,	  0},	/* Reserved Pulse			 */
   { PULSE_LLL_ALPHA,	  0},	/* Reserved National		 */
   { PULSE_LLL_ALPHA,	  0},	/* Reserved National		 */
   { PULSE_LLL_ALPHA,	  0},	/* Reserved National		 */
   { PULSE_LLL_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
   { PULSE_LLL_ALPHA,	  0},	/* Reserved National		 */
   { PULSE_LLL_ALPHA,	  0},	/* Reserved National		 */
   { PULSE_LLL_ALPHA,	  0},	/* Intra Country Data		 */
   { PULSE_LLL_ALPHA,	  0},	/* Reserved National		 */
   { PULSE_LLL_ALPHA,	  0},	/* Original Message Type Id	 */

   { PULSE_LLL_ALPHA,	  0},	/* Issuing Institution Id	 */
   { PULSE_LLL_ALPHA,	  0},	/* Remaining Open to Use	 */
   { PULSE_LLL_ALPHA,	 29},	/* ADR_VERIF_DATA		 */
   { PULSE_LLL_ALPHA,	  0},	/* Free Form Text - Japan	 */
   { PULSE_LLL_ALPHA,	255},	/* SUPPORT_INFO			 */
   { PULSE_LLL_ALPHA,	  50},	/*  PRIV_USE_DATA (switch private data)  */
   { PULSE_LLL_ALPHA,	  0},	/* File Record Action and Data	 */
   { NOT_USED,	     	  8},	/* MAC2				 */

};

static tag_info_t  tab_PulseTagInfo[] =
{
	{ "9F02",  PULSE_TAG_VAR_HEX   		,	PULSE_TAG_TRANS_AMOUNT_LEN       },/* Transaction Amount                    */
	{ "9F03",  PULSE_TAG_VAR_HEX   		,	PULSE_TAG_OTHER_AMOUNT_LEN       },/* Other Amount                          */
	{ "9F26",  PULSE_TAG_FIX_BIN   		,	PULSE_TAG_APP_CRYPTOGRAM_LEN     },/* Application cryptogram                */
	{ "9F06",  PULSE_TAG_VAR_BIN   		,	PULSE_TAG_APP_ID_LEN             },/* Application Identifier (AID)          */
	{ "8200",  PULSE_TAG_FIX_BIN   		,	PULSE_TAG_APP_INTER_PROFILE_LEN  },/* Application Interchange Profile       */
	{ "9F36",  PULSE_TAG_FIX_BIN   		,	PULSE_TAG_ATC_LEN                },/* Application Transaction Counter       */
	{ "9F07",  PULSE_TAG_FIX_BIN   		,	PULSE_TAG_APP_USAGE_CTRL_LEN     },/* Application Usage Control             */
	{ "9F27",  PULSE_TAG_FIX_BIN   		,	PULSE_TAG_CRYPTO_INFO_DATA_LEN   },/* Cryptogram Information Data           */
	{ "9F34",  PULSE_TAG_FIX_BIN   		,	PULSE_TAG_CVM_RESULTS_LEN        },/* Cardholder Verification Method        */
	{ "8400",  PULSE_TAG_VAR_BIN   		,	PULSE_TAG_DED_FILE_NAME_LEN      },/* Dedicated File Name                   */
	{ "9F1E",  PULSE_TAG_FIX_ALPHA 		,	PULSE_TAG_IFD_SERIAL_NUM_LEN     },/* Interface Device Serial Number        */
	{ "9F10",  PULSE_TAG_VAR_BIN   		,	PULSE_TAG_ISS_APP_DATA_LEN       },/* Issuer Application Data               */
	{ "9100",  PULSE_TAG_VAR_BIN   		,	PULSE_TAG_ISS_AUTHEN_DATA_LEN    },/* Issuer Authentication Data            */
	{ "9F5B",  PULSE_TAG_VAR_BIN   		,	PULSE_TAG_ISS_SCRIPT_RESULTS_LEN },/* Issuer Script Results                 */
	{ "7100",  PULSE_TAG_VAR_BIN   		,	PULSE_TAG_ISS_SCRIPT1_LEN        },/* Issuer Script Template 1              */
	{ "7200",  PULSE_TAG_VAR_BIN   		,	PULSE_TAG_ISS_SCRIPT2_LEN        },/* Issuer Script Template 2              */
	{ "9F09",  PULSE_TAG_FIX_BIN   		,	PULSE_TAG_TERM_APP_VER_NUM_LEN   },/* Terminal Application Version Number   */
	{ "9F33",  PULSE_TAG_FIX_BIN   		,	PULSE_TAG_TERM_CAP_LEN           },/* Terminal Capabilities                 */
	{ "9F1A",  PULSE_TAG_VAR_HEX   		,	PULSE_TAG_TERM_COUNTRY_CODE_LEN  },/* Terminal Country Code                 */
	{ "9F35",  PULSE_TAG_VAR_HEX   		,	PULSE_TAG_TERM_TYPE_LEN          },/* Terminal Type                         */
	{ "9500",  PULSE_TAG_FIX_BIN   		,	PULSE_TAG_TVR_LEN                },/* Terminal Verification Results         */
	{ "9F53",  PULSE_TAG_VAR_ALPHA 		,	PULSE_TAG_TRANS_CATEG_CODE_LEN   },/* Transaction Category Code             */
	{ "5F2A",  PULSE_TAG_VAR_HEX   		,	PULSE_TAG_TRANS_CUR_CODE_LEN     },/* Transaction Currency Code             */
	{ "9A00",  PULSE_TAG_VAR_HEX   		,	PULSE_TAG_TRANS_DATE_LEN         },/* Transaction Date                      */
	{ "9F41",  PULSE_TAG_VAR_HEX   		,	PULSE_TAG_TRANS_SEQ_NUM_LEN      },/* Transaction Sequence Number           */
	{ "9C00",  PULSE_TAG_VAR_HEX   		,	PULSE_TAG_TRANS_TYPE_LEN         },/* Transaction TYPE                      */
	{ "9F37",  PULSE_TAG_FIX_BIN   		,	PULSE_TAG_UNPRED_NUMBER_LEN      },/* Unpredictable Number                  */

};

static pds_info_t  tab_PulsePdsInfo[] =
{
   { "01",  PULSE_PDS_FIX_ALPHA,  16},/* DES HMK (PBK)                     */
   { "02",  PULSE_PDS_FIX_ALPHA,   2},/* Traffic Flags                     */
   { "03",  PULSE_PDS_FIX_ALPHA,   2},/* Version Number                    */
   { "04",  PULSE_PDS_FIX_ALPHA,   2},/* SIGN-OFF Reason Code              */
   { "05",  PULSE_PDS_VAR_ALPHA,  96},/* Rejected Message                  */
   { "06",  PULSE_PDS_FIX_ALPHA,   2},/* Reject Reason Code                */
   { "07",  PULSE_PDS_FIX_ALPHA,   2},/* Test Production Flags             */
   { "08",  PULSE_PDS_FIX_ALPHA,   2},/* PIN Block Format Code             */
   { "20",  PULSE_PDS_FIX_ALPHA,   1},/* Cardholder Verification Method    */
   { "40",  PULSE_PDS_VAR_ALPHA,  40},/* Certificate Serial Number         */
   { "42",  PULSE_PDS_FIX_ALPHA,   3},/* Security Level Indicator          */
   { "43",  PULSE_PDS_VAR_ALPHA,  32},/* UCAF                              */
   { "87",  PULSE_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Invalid       */
   { "88",  PULSE_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Status Change */
   { "89",  PULSE_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Track Edit    */
   { "92",  PULSE_PDS_FIX_ALPHA,   3},/* Card Verification Code 2          */
};

static fld_info_t tab_PdFldInfo[] =       /* Fields from PD Field             */
{
   {PD_FIX_ALPHA,	  8},	/* Pseudo Terminal		 		 */
   {PD_FIX_BCD,	 15},   /* Issuer Network Identification			 */
   {PD_FIX_ALPHA,	  4},	/* Acquirer Network Identification		 */
   {PD_FIX_ALPHA,	  1},	/* Processor Identification	 */
   {PD_FIX_BCD,	  2},	/*Authorisation Timeout			 */
   {PD_FIX_ALPHA,	  1},	/* Prestigious Property Indicator*/
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {PD_FIX_ALPHA,	 15},	/*  MasterCard Interchange  	 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {PD_FIX_BCD,	  10},	/*ENH 11.1 MVV				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
};

static fld_info_t tab_ReasonFldInfo[] =       /* Fields from Reason Code Field             */
{
   {PD_FIX_ALPHA,	  4},	/* Reversal Reason Code		 		 */
   {PD_FIX_ALPHA,	 2},   /* Advice Reason Code			 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*        */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
};



void  InitPulseIcTag  (TSTagPulse * msgInfo)
{
   int i;

   for (i = 0; i < MAX_PULSE_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', sizeof(msgInfo->sChipData));
}

void  InitPulsePds  (TSPdsPulse * pdsInfo)
{
   int i;

   for (i = 0; i < MAX_PULSE_PDS; i++)
   {
      pdsInfo->nPosPds [i] = 0;
      pdsInfo->nPresent[i] = NOT_PRESENT;
   }
   pdsInfo->nLength  = 0;
   pdsInfo->nMsgType = 0;
   memset(pdsInfo->sPdsData,   '\0', sizeof(pdsInfo->sPdsData));
}

void  InitPulseInfo(TSPulseInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_PULSE_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < PULSE_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', sizeof(msgInfo->sData));
}

int GetPulseFieldType(int field_no)
{
   return(tab_PulseFldInfo[field_no].type);
}

int GetPulseFieldLength(int field_no)
{
   return(tab_PulseFldInfo[field_no].length);
}


int GetPulseTagIndice ( char * szTagName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_PulseTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_PulseTagInfo[nIndice].sTagName, 4) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetPulseTagType ( int nIndice )
{
   return ( tab_PulseTagInfo[nIndice].nType);
}

int GetPulseTagLength ( int nIndice )
{
   return ( tab_PulseTagInfo[nIndice].nLength);
}

int GetPulsePdsIndice ( char * szPdsName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_PulsePdsInfo) /sizeof(pds_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szPdsName, tab_PulsePdsInfo[nIndice].sPdsName, 2) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetPulsePdsType ( int nIndice )
{
   return ( tab_PulsePdsInfo[nIndice].nType);
}

int GetPulsePdsLength ( int nIndice )
{
   return ( tab_PulsePdsInfo[nIndice].nLength);
}

int GetPdFieldType(int field_no)
{
   return(tab_PdFldInfo[field_no].type);
}

int GetPdFieldLength(int field_no)
{
   return(tab_PdFldInfo[field_no].length);
}


int GetReasonCodeFieldType(int field_no)
{
   return(tab_ReasonFldInfo[field_no].type);
}

int GetReasonCodeFieldLength(int field_no)
{
   return(tab_ReasonFldInfo[field_no].length);
}
/* Start FZL20220913 PLUTONL-4979 */

static const TlvInfoProperties	PULSEF125DataProp = {
2,3,TLV_LT_ASCII,TLV_LT_ASCII, PLS_F125_NB_FIELDS,
{
{PLS_F110T_KP_KEY_BLOCK, FT_STRUCT_TLV,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,115,PP_HIDE,"","PLS_F125_TKP_PROP","",NOT_REPEATABLE},
},
"PULSEF125DataProp"
};

/* field 125 tag KP details */

static const TlvInfoProperties	PULSEF125TKPDataProp = {
2,3,TLV_LT_ASCII,TLV_LT_ASCII,PLS_F125_TKP_NB_FIELDS,
{
{PLS_F125_TKP_KEY_DATA,  FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,96,PP_HIDE,"NULL","KEY DATA","NULL",NOT_REPEATABLE},
{PLS_F125_TKP_CHECK_VALUE,  FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,4,PP_PRINT,"NULL","CHECK VALUE","NULL",NOT_REPEATABLE},
},
"PULSEF125TKPDataProp"
};

/******************************************************************************/
void InitPulseKeyBlock(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &PULSEF125DataProp;
	ResetTlvInfo(pkTlvInfo);
}
/********************************************************************************/
void InitPulseTKPDataProp(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &PULSEF125TKPDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/* end  FZL20220913 PLUTONL-4979 */