#include <stdio.h>
#include <stdlib.h>
#include <iso_euro.h>
#include <define.h>


fld_info_t    tab_EuroFldInfo[] =
{
   { NOT_USED,  	  0},	/* BIT_MAP1			 */
   { EURO_LL_ALPHA,	 19},   /* CARD_NBR 			 */
   { EURO_FIX_ALPHA,	  6},	/* PROC_CODE			 */
   { EURO_FIX_ALPHA,	 12},	/* TRANS_AMOUNT			 */
   { EURO_FIX_ALPHA,	 12},	/* STLMNT_AMOUNT		 */
   { EURO_FIX_ALPHA,	 12},	/* Cardholder Billing Amount	 */
   { EURO_FIX_ALPHA,	 10},	/* XMIT_TIME			 */
   { EURO_FIX_ALPHA,	  8},	/* Cardholder Billing Fee	 */
   { EURO_FIX_ALPHA,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { EURO_FIX_ALPHA,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { EURO_FIX_ALPHA,	  6},	/* AUDIT_NBR			 */
   { EURO_FIX_ALPHA,	  6},	/* TRANS_TIME                    */
   { EURO_FIX_ALPHA,	  4},	/* TRANS_DATE			 */
   { EURO_FIX_ALPHA,	  4},	/* EXPIRY_DATE			 */
   { EURO_FIX_ALPHA,	  4},	/* STLMNT_DATE			 */
   { EURO_FIX_ALPHA,	  4},	/* CNVRSN_DATE			 */
   { EURO_FIX_ALPHA,	  4},	/* CAPTURE_DATE			 */
   { EURO_FIX_ALPHA,	  4},	/* MERCHANT_TYPE		 */
   { EURO_FIX_ALPHA,	  3},	/* ACQR_COUNTRY			 */
   { EURO_FIX_ALPHA,	  3},	/* PAN_COUNTRY			 */

   { EURO_FIX_ALPHA,	  3},	/* FORWD_COUNTRY		 */
   { EURO_FIX_ALPHA,	  3},	/* POS_ENTRY_MODE		 */
   { EURO_FIX_ALPHA,	  3},	/* Card Sequence Number		 */
   { EURO_FIX_ALPHA,	  3},	/* Function Code                 */
   { EURO_FIX_ALPHA,	  2},	/* POS_CONDITION		 */
   { EURO_FIX_ALPHA,	  2},	/* MERCHANT TYPE  		 */
   { EURO_FIX_ALPHA,	  1},	/* Authorization Id Response Lenght */
   { EURO_FIX_ALPHA,	  9},	/* TRANS_FEE			 */
   { EURO_FIX_ALPHA,	  9},	/* Amount Settlement Fee	 */
   { EURO_FIX_ALPHA,	  9},	/* Amount Transaction Processing Fee */
	
   { EURO_FIX_ALPHA,	  9},	/* Amount Setlmnt Processing Fee */
   { EURO_LL_ALPHA,	 11},	/* ACQR_ID			 */
   { EURO_LL_ALPHA,	 11},	/* FORWD_ID			 */
   { EURO_LL_ALPHA,	 28},	/* PAN_EXTENDED			 */
   { EURO_LL_ALPHA,	 37},	/* T2_DATA			 */
   { EURO_LLL_ALPHA,	  4},	/* T1_DATA                       */
   { EURO_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { EURO_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { EURO_FIX_ALPHA,	  2},	/* ACTION CODE  		 */
   { EURO_FIX_ALPHA,	  3},	/* Service Restriction Code	 */

   { EURO_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { EURO_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { EURO_FIX_ALPHA,	 40},	/* TERMINAL_ADR			 */
   { EURO_LL_ALPHA,	 25},	/* ADTNL_RESP_DATA		 */
   { EURO_LL_ALPHA,	 77},	/* T1_DATA			 */
   { EURO_LLL_ALPHA,	255},	/* Additrional Data Euro		 */
   { EURO_LLL_ALPHA,	255},	/* Additrional Data National	 */
   { EURO_LLL_BIN,	999},	/* ADTNL_DATA_PRIV		 */
   { EURO_FIX_ALPHA,	  3},	/* TRANS_CRNCY			 */
   { EURO_FIX_ALPHA,	  3},	/* STLMNT_CRNCY			 */

   { EURO_FIX_ALPHA,	  3},	/* BILLING_CRNCY		 */
   { EURO_FIX_HEX,	 16},	/* PIN_DATA			 */
   { EURO_FIX_ALPHA,	 16},	/* SECURITY_DATA		 */
   { EURO_LLL_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { EURO_LLL_BIN,	255},	/* ICC Information               */
   { EURO_LLL_ALPHA,	255},	/* Orig Data Elements            */
   { EURO_LLL_ALPHA,	  3},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	255},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	 14},	/* GEOGR_DATA			 */
   { EURO_LLL_ALPHA,	  6},	/* ADTNL_POS_INFO		 */
	
   { EURO_LLL_ALPHA,	 36},	/* OTHER_AMOUNTS		 */
   { EURO_LLL_ALPHA, 	999},	/* Redemption Number		 */
   { EURO_LLL_ALPHA, 	999},	/* Versions       		 */
   { NOT_USED,	  	  8},	/* MAC1				 */
   { NOT_USED,	  	  0},	/* 				 */
   { EURO_FIX_ALPHA,	  1},	/* STLMNT_CODE			 */
   { EURO_FIX_ALPHA,	  2},	/* Extended Payment Code	*/
   { EURO_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		 */
   { EURO_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		 */
   { EURO_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */

   { EURO_FIX_ALPHA,	  4},	/* Message Number		 */
   { EURO_FIX_ALPHA,	  4},	/* Data Record        		 */
   { EURO_FIX_ALPHA,	  6},	/* ACTION_DATE			 */
   { EURO_FIX_ALPHA,	 10},	/* CR_NBR			 */
   { EURO_FIX_ALPHA,	 10},	/* CR_REV_NBR			 */
   { EURO_FIX_ALPHA,	 10},	/* DB_NBR			 */
   { EURO_FIX_ALPHA,	 10},	/* DB_REV_NBR			 */
   { EURO_FIX_ALPHA,	 10},	/* Transfer Number		 */
   { EURO_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
   { EURO_FIX_ALPHA,	 10},	/* Inquiries Number		 */

   { EURO_FIX_ALPHA,	 10},	/* Authorizations Number	 */
   { EURO_FIX_ALPHA,	 12},	/* Credit, Processing Fee Amount */
   { EURO_FIX_ALPHA,	 12},	/* Payment Number                 */
   { EURO_FIX_ALPHA,	 12},	/* Payment Reversal Number     	 */
   { EURO_FIX_ALPHA,	 12},	/* Debit, Transaction Fee Amount */
   { EURO_FIX_ALPHA,	 16},	/* CR_AMOUNT			 */
   { EURO_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		 */
   { EURO_FIX_ALPHA,	 16},	/* DB_AMOUNT			 */
   { EURO_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		 */
   { EURO_FIX_ALPHA,	 42},	/* ORIG_DATA			 */

   { EURO_FIX_ALPHA,	  1},	/* FILE_UPDATE			 */
   { EURO_FIX_ALPHA,	  2},	/* FILE_SECURITY		 */
   { EURO_FIX_ALPHA,	  5},	/* Destination Inst ID Code      */
   { EURO_FIX_ALPHA,	  7},	/* Source Inst Id Code           */
   { EURO_FIX_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
   { EURO_FIX_HEX  ,	 16},	/* Additional Key Mngmt data     */
   { EURO_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { EURO_FIX_ALPHA,	 25},	/* Payee			 */
   { EURO_LL_ALPHA,	 11},	/* STLMNT_INSTITN		 */
   { EURO_LL_ALPHA,	 11},	/* RECEIVER_ID			 */

   { EURO_LL_ALPHA,	 17},	/* FILE_NAME			 */
   { EURO_LL_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { EURO_LL_ALPHA,	 28},	/* ACCOUNT_ID2			 */
   { EURO_LLL_ALPHA,	 16},	/* Transaction Description	 */
   { EURO_LLL_ALPHA,	 16},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	 16},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	 10},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	 10},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved Euro			 */

   { EURO_LLL_ALPHA,	  0},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	  0},	/* Intra Country Data		 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	  0},	/* Original Message Type Id	 */

   { EURO_LLL_ALPHA,	  0},	/* Issuing Institution Id	 */
   { EURO_LLL_ALPHA,	  0},	/* Remaining Open to Use	 */
   { EURO_LLL_ALPHA,	 29},	/* ADR_VERIF_DATA		 */
   { EURO_LLL_ALPHA,	  0},	/* Free Form Text - Japan	 */
   { EURO_LLL_ALPHA,	255},	/* SUPPORT_INFO			 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved Private		 */
   { EURO_LLL_ALPHA,	  0},	/* File Record Action and Data	 */
   { NOT_USED,	     	  8},	/* MAC2				 */

};

fld_info_t    tab_Euro93FldInfo[] =
{
   { NOT_USED,  	  0},	/* BIT_MAP1			 */
   { EURO_LL_ALPHA,	 19},   /* CARD_NBR 			 */
   { EURO_FIX_ALPHA,	  6},	/* PROC_CODE			 */
   { EURO_FIX_ALPHA,	 12},	/* TRANS_AMOUNT			 */
   { EURO_FIX_ALPHA,	 12},	/* STLMNT_AMOUNT		 */
   { EURO_FIX_ALPHA,	 12},	/* Cardholder Billing Amount	 */
   { EURO_FIX_ALPHA,	 10},	/* XMIT_TIME			 */
   { EURO_FIX_ALPHA,	  8},	/* Cardholder Billing Fee	 */
   { EURO_FIX_ALPHA,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { EURO_FIX_ALPHA,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { EURO_FIX_ALPHA,	  6},	/* AUDIT_NBR			 */
   { EURO_FIX_ALPHA,	 12},	/* TRANS_TIME                    */
   { EURO_FIX_ALPHA,	  4},	/* TRANS_DATE			 */
   { EURO_FIX_ALPHA,	  4},	/* EXPIRY_DATE			 */
   { EURO_FIX_ALPHA,	  4},	/* STLMNT_DATE			 */
   { EURO_FIX_ALPHA,	  4},	/* CNVRSN_DATE			 */
   { EURO_FIX_ALPHA,	  4},	/* CAPTURE_DATE			 */
   { EURO_FIX_ALPHA,	  4},	/* MERCHANT_TYPE		 */
   { EURO_FIX_ALPHA,	  3},	/* ACQR_COUNTRY			 */
   { EURO_FIX_ALPHA,	  3},	/* PAN_COUNTRY			 */

   { EURO_FIX_ALPHA,	  3},	/* FORWD_COUNTRY		 */
   { EURO_FIX_ALPHA,	  3},	/* POS_ENTRY_MODE		 */
   { EURO_FIX_ALPHA,	  3},	/* Card Sequence Number		 */
   { EURO_FIX_ALPHA,	  3},	/* Function Code                 */
   { EURO_FIX_ALPHA,	  4},	/* POS_CONDITION		 */
   { EURO_FIX_ALPHA,	  2},	/* MERCHANT TYPE  		 */
   { EURO_FIX_ALPHA,	  1},	/* Authorization Id Response Lenght */
   { EURO_FIX_ALPHA,	  9},	/* TRANS_FEE			 */
   { EURO_FIX_ALPHA,	  9},	/* Amount Settlement Fee	 */
   { EURO_FIX_ALPHA,	  9},	/* Amount Transaction Processing Fee */
	
   { EURO_FIX_ALPHA,	  9},	/* Amount Setlmnt Processing Fee */
   { EURO_LL_ALPHA,	 11},	/* ACQR_ID			 */
   { EURO_LL_ALPHA,	 11},	/* FORWD_ID			 */
   { EURO_LL_ALPHA,	 28},	/* PAN_EXTENDED			 */
   { EURO_LL_ALPHA,	 37},	/* T2_DATA			 */
   { EURO_LLL_ALPHA,	  4},	/* T1_DATA                       */
   { EURO_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { EURO_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { EURO_FIX_ALPHA,	  3},	/* ACTION CODE  		 */
   { EURO_FIX_ALPHA,	  3},	/* Service Restriction Code	 */

   { EURO_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { EURO_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { EURO_FIX_ALPHA,	 40},	/* TERMINAL_ADR			 */
   { EURO_LL_ALPHA,	 25},	/* ADTNL_RESP_DATA		 */
   { EURO_LL_ALPHA,	 77},	/* T1_DATA			 */
   { EURO_LLL_ALPHA,	255},	/* Additrional Data Euro		 */
   { EURO_LLL_ALPHA,	255},	/* Additrional Data National	 */
   { EURO_LLL_BIN,	999},	/* ADTNL_DATA_PRIV		 */
   { EURO_FIX_ALPHA,	  3},	/* TRANS_CRNCY			 */
   { EURO_FIX_ALPHA,	  3},	/* STLMNT_CRNCY			 */

   { EURO_FIX_ALPHA,	  3},	/* BILLING_CRNCY		 */
   { EURO_FIX_HEX,	 16},	/* PIN_DATA			 */
   { EURO_FIX_ALPHA,	 16},	/* SECURITY_DATA		 */
   { EURO_LLL_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { EURO_LLL_BIN,	255},	/* ICC Information               */
   { EURO_LLL_ALPHA,	255},	/* Orig Data Elements            */
   { EURO_LLL_ALPHA,	  3},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	255},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	 14},	/* GEOGR_DATA			 */
   { EURO_LLL_ALPHA,	  6},	/* ADTNL_POS_INFO		 */
	
   { EURO_LLL_ALPHA,	 36},	/* OTHER_AMOUNTS		 */
   { EURO_LLL_ALPHA, 	999},	/* Redemption Number		 */
   { EURO_LLL_ALPHA, 	999},	/* Versions       		 */
   { NOT_USED,	  	  8},	/* MAC1				 */
   { NOT_USED,	  	  0},	/* 				 */
   { EURO_FIX_ALPHA,	  1},	/* STLMNT_CODE			 */
   { EURO_FIX_ALPHA,	  2},	/* Extended Payment Code	*/
   { EURO_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		 */
   { EURO_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		 */
   { EURO_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */

   { EURO_FIX_ALPHA,	  4},	/* Message Number		 */
   { EURO_FIX_ALPHA,	  4},	/* Data Record        		 */
   { EURO_FIX_ALPHA,	  6},	/* ACTION_DATE			 */
   { EURO_FIX_ALPHA,	 10},	/* CR_NBR			 */
   { EURO_FIX_ALPHA,	 10},	/* CR_REV_NBR			 */
   { EURO_FIX_ALPHA,	 10},	/* DB_NBR			 */
   { EURO_FIX_ALPHA,	 10},	/* DB_REV_NBR			 */
   { EURO_FIX_ALPHA,	 10},	/* Transfer Number		 */
   { EURO_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
   { EURO_FIX_ALPHA,	 10},	/* Inquiries Number		 */

   { EURO_FIX_ALPHA,	 10},	/* Authorizations Number	 */
   { EURO_FIX_ALPHA,	 12},	/* Credit, Processing Fee Amount */
   { EURO_FIX_ALPHA,	 12},	/* Payment Number                 */
   { EURO_FIX_ALPHA,	 12},	/* Payment Reversal Number     	 */
   { EURO_FIX_ALPHA,	 12},	/* Debit, Transaction Fee Amount */
   { EURO_FIX_ALPHA,	 16},	/* CR_AMOUNT			 */
   { EURO_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		 */
   { EURO_FIX_ALPHA,	 16},	/* DB_AMOUNT			 */
   { EURO_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		 */
   { EURO_FIX_ALPHA,	 42},	/* ORIG_DATA			 */

   { EURO_FIX_ALPHA,	  1},	/* FILE_UPDATE			 */
   { EURO_FIX_ALPHA,	  2},	/* FILE_SECURITY		 */
   { EURO_FIX_ALPHA,	  5},	/* Destination Inst ID Code      */
   { EURO_LL_ALPHA,	  7},	/* Source Inst Id Code           */
   { EURO_FIX_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
   { EURO_FIX_HEX  ,	 16},	/* Additional Key Mngmt data     */
   { EURO_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { EURO_FIX_ALPHA,	 25},	/* Payee			 */
   { EURO_LL_ALPHA,	 11},	/* STLMNT_INSTITN		 */
   { EURO_LL_ALPHA,	 11},	/* RECEIVER_ID			 */

   { EURO_LL_ALPHA,	 17},	/* FILE_NAME			 */
   { EURO_LL_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { EURO_LL_ALPHA,	 28},	/* ACCOUNT_ID2			 */
   { EURO_LLL_ALPHA,	 16},	/* Transaction Description	 */
   { EURO_LLL_ALPHA,	 16},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	 16},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	 10},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	 10},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved Euro			 */

   { EURO_LLL_ALPHA,	  0},	/* Reserved Euro			 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	  0},	/* Intra Country Data		 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved National		 */
   { EURO_LLL_ALPHA,	  0},	/* Original Message Type Id	 */

   { EURO_LLL_ALPHA,	  0},	/* Issuing Institution Id	 */
   { EURO_LLL_ALPHA,	  0},	/* Remaining Open to Use	 */
   { EURO_LLL_ALPHA,	 29},	/* ADR_VERIF_DATA		 */
   { EURO_LLL_ALPHA,	  0},	/* Free Form Text - Japan	 */
   { EURO_LLL_ALPHA,	255},	/* SUPPORT_INFO			 */
   { EURO_LLL_ALPHA,	  0},	/* Reserved Private		 */
   { EURO_LLL_ALPHA,	  0},	/* File Record Action and Data	 */
   { NOT_USED,	     	  8},	/* MAC2				 */

};

tag_info_t  tab_EuroTagInfo[] = 
{
   { "7100",  EURO_TAG_VAR_BCD,   0},/* Issuer Script Template 1              */
   { "7200",  EURO_TAG_VAR_BCD,   0},/* Issuer Script Template 2              */
   { "8200",  EURO_TAG_VAR_HEX,   2},/* Application Interchange Profile       */
   { "8400",  EURO_TAG_VAR_BCD,   0},/* Dedicated File Name                   */
   { "9100",  EURO_TAG_VAR_HEX,   0},/* Issuer Authentication Data            */
   { "9500",  EURO_TAG_VAR_BCD,   5},/* Terminal Verification Results         */
   { "9A00",  EURO_TAG_VAR_BCD,   6},/* Transaction Date                      */
   { "9C00",  EURO_TAG_VAR_BCD,   2},/* Transaction TYPE                      */
   { "5F2A",  EURO_TAG_VAR_BCD,   3},/* Transaction Currency Code             */
   { "9F02",  EURO_TAG_VAR_BCD,  12},/* Transaction Amount                    */
   { "9F03",  EURO_TAG_VAR_BCD,  12},/* Other Amount                          */
   { "9F09",  EURO_TAG_VAR_BCD,   2},/* Terminal Application Version Number   */
   { "9F10",  EURO_TAG_VAR_HEX,   0},/*  Issuer Application Data              */
   { "9F1A",  EURO_TAG_VAR_BCD,   3},/* Terminal Country Code                 */
   { "9F1E",  EURO_TAG_VAR_ALPHA, 8},/* Interface Device Serial Number        */
   { "9F26",  EURO_TAG_VAR_HEX,   8},/* Application cryptogram                */
   { "9F27",  EURO_TAG_VAR_BCD,   1},/* Cryptogram Information Data           */
   { "9F33",  EURO_TAG_VAR_HEX,   3},/* Terminal Capabilities                 */
   { "9F34",  EURO_TAG_VAR_BCD,   3},/* Cardholder Verification Method        */
   { "9F35",  EURO_TAG_VAR_BCD,   2},/* Terminal Type                         */
   { "9F36",  EURO_TAG_VAR_HEX,   2},/* Application Transaction Counter       */
   { "9F37",  EURO_TAG_VAR_HEX,   4},/* Unpredictable Number                  */
   { "9F41",  EURO_TAG_VAR_BCD,   0},/* Transaction Sequence Number           */
   { "9F53",  EURO_TAG_VAR_BCD, 1},/* Transaction Category Code             */
};

pds_info_t  tab_EuroPdsInfo[] = 
{
   { "01",  EURO_PDS_FIX_ALPHA,  16},/* DES HMK (PBK)                     */
   { "02",  EURO_PDS_FIX_ALPHA,   2},/* Traffic Flags                     */
   { "03",  EURO_PDS_FIX_ALPHA,   2},/* Version Number                    */
   { "04",  EURO_PDS_FIX_ALPHA,   2},/* SIGN-OFF Reason Code              */
   { "05",  EURO_PDS_VAR_ALPHA,  96},/* Rejected Message                  */
   { "06",  EURO_PDS_FIX_ALPHA,   2},/* Reject Reason Code                */
   { "07",  EURO_PDS_FIX_ALPHA,   2},/* Test Production Flags             */
   { "08",  EURO_PDS_FIX_ALPHA,   2},/* PIN Block Format Code             */
   { "20",  EURO_PDS_FIX_ALPHA,   1},/* Cardholder Verification Method    */
   { "40",  EURO_PDS_VAR_ALPHA,  40},/* Certificate Serial Number         */
   { "42",  EURO_PDS_FIX_ALPHA,   3},/* Security Level Indicator          */
   { "43",  EURO_PDS_VAR_ALPHA,  32},/* UCAF                              */
   { "87",  EURO_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Invalid       */
   { "88",  EURO_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Status Change */
   { "89",  EURO_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Track Edit    */
   { "92",  EURO_PDS_FIX_ALPHA,   3},/* Card Verification Code 2          */
};

pds93_info_t  tab_EuroPds93Info[] = 
{
   { "005",  EURO_PDS_VAR_ALPHA,  16},/* Message Error Indicator           */
   { "601",  EURO_PDS_FIX_ALPHA,   1},/* Entity Selector                   */
   { "602",  EURO_PDS_VAR_ALPHA,   1},/* Regional Listing Data             */
   { "603",  EURO_PDS_FIX_ALPHA,   6},/* Chargeback Effective Date         */
   { "605",  EURO_PDS_FIX_ALPHA,   6},/* Last Update Date                  */
   { "606",  EURO_PDS_FIX_ALPHA,   2},/* Service ID                        */
   { "608",  EURO_PDS_FIX_ALPHA,   4},/* Listing Region                    */
};

void  InitEuroIcTag  (TSTagEuro * msgInfo)
{
   int i;

   for (i = 0; i < MAX_EURO_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', MAX_EURO_CHIP_LEN);
}

void  InitEuroPds  (TSPdsEuro * pdsInfo)
{
   int i;

   for (i = 0; i < MAX_EURO_PDS; i++)
   {
      pdsInfo->nPosPds [i] = 0;
      pdsInfo->nPresent[i] = NOT_PRESENT;
   }
   pdsInfo->nLength  = 0;
   pdsInfo->nMsgType = 0;
   memset(pdsInfo->sPdsData,   '\0', MAX_EURO_PDS_LEN);
}

void  InitEuroInfo(TSEuroInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_EURO_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < EURO_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_EURO_DATA_LEN);
}

int GetEuroFieldType(int field_no)
{
   return(tab_EuroFldInfo[field_no].type);
}

int GetEuroFieldLength(int field_no)
{
   return(tab_EuroFldInfo[field_no].length);
}

int GetEuro93FieldType(int field_no)
{
   return(tab_Euro93FldInfo[field_no].type);
}

int GetEuro93FieldLength(int field_no)
{
   return(tab_Euro93FldInfo[field_no].length);
}

int GetEuroTagIndice ( char * szTagName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_EuroTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_EuroTagInfo[nIndice].sTagName, 4) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetEuroTagType ( int nIndice )
{
   return ( tab_EuroTagInfo[nIndice].nType);
}

int GetEuroTagLength ( int nIndice )
{
   return ( tab_EuroTagInfo[nIndice].nLength);
}

int GetEuroPdsIndice ( char * szPdsName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_EuroPdsInfo) /sizeof(pds_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szPdsName, tab_EuroPdsInfo[nIndice].sPdsName, 2) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetEuroPdsType ( int nIndice )
{
   return ( tab_EuroPdsInfo[nIndice].nType);
}

int GetEuroPdsLength ( int nIndice )
{
   return ( tab_EuroPdsInfo[nIndice].nLength);
}

int GetEuroPds93Indice ( char * szPdsName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_EuroPds93Info) /sizeof(pds93_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szPdsName, tab_EuroPds93Info[nIndice].sPdsName, 3) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetEuroPds93Type ( int nIndice )
{
   return ( tab_EuroPds93Info[nIndice].nType);
}

int GetEuroPds93Length ( int nIndice )
{
   return ( tab_EuroPds93Info[nIndice].nLength);
}

