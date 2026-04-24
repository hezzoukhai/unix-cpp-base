#include <stdio.h>
#include <stdlib.h>
#include <iso_mds.h>
#include <define.h>
#include <mds_define.h>
#include <iso8583.h>


fld_info_t    tab_MdsFldInfo[] =
{
   { NOT_USED,  	  0},	/* BIT_MAP1			 */
   { MDS_LL_ALPHA,	 19},   /* CARD_NBR 			 */
   { MDS_FIX_ALPHA,	  6},	/* PROC_CODE			 */
   { MDS_FIX_ALPHA,	 12},	/* TRANS_AMOUNT			 */
   { MDS_FIX_ALPHA,	 12},	/* STLMNT_AMOUNT		 */
   { MDS_FIX_ALPHA,	 12},	/* Cardholder Billing Amount	 */
   { MDS_FIX_ALPHA,	 10},	/* XMIT_TIME			 */
   { MDS_FIX_ALPHA,	  8},	/* Cardholder Billing Fee	 */
   { MDS_FIX_ALPHA,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { MDS_FIX_ALPHA,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { MDS_FIX_ALPHA,	  6},	/* AUDIT_NBR			 */
   { MDS_FIX_ALPHA,	  6},	/* TRANS_TIME                    */
   { MDS_FIX_ALPHA,	  4},	/* TRANS_DATE			 */
   { MDS_FIX_ALPHA,	  4},	/* EXPIRY_DATE			 */
   { MDS_FIX_ALPHA,	  4},	/* STLMNT_DATE			 */
   { MDS_FIX_ALPHA,	  4},	/* CNVRSN_DATE			 */
   { MDS_FIX_ALPHA,	  4},	/* CAPTURE_DATE			 */
   { MDS_FIX_ALPHA,	  4},	/* MERCHANT_TYPE		 */
   { MDS_FIX_ALPHA,	  3},	/* ACQR_COUNTRY			 */
   { MDS_FIX_ALPHA,	  3},	/* PAN_COUNTRY			 */

   { MDS_FIX_ALPHA,	  3},	/* FORWD_COUNTRY		 */
   { MDS_FIX_ALPHA,	  3},	/* POS_ENTRY_MODE		 */
   { MDS_FIX_ALPHA,	  3},	/* Card Sequence Number		 */
   { MDS_FIX_ALPHA,	  3},	/* Function Code                 */
   { MDS_FIX_ALPHA,	  2},	/* POS_CONDITION		 */
   { MDS_FIX_ALPHA,	  2},	/* MERCHANT TYPE  		 */
   { MDS_FIX_ALPHA,	  1},	/* Authorization Id Response Lenght */
   { MDS_FIX_ALPHA,	  9},	/* TRANS_FEE			 */
   { MDS_FIX_ALPHA,	  9},	/* Amount Settlement Fee	 */
   { MDS_FIX_ALPHA,	  9},	/* Amount Transaction Processing Fee */
	
   { MDS_FIX_ALPHA,	  9},	/* Amount Setlmnt Processing Fee */
   { MDS_LL_ALPHA,	 11},	/* ACQR_ID			 */
   { MDS_LL_ALPHA,	 11},	/* FORWD_ID			 */
   { MDS_LL_ALPHA,	 28},	/* PAN_EXTENDED			 */
   { MDS_LL_ALPHA,	 37},	/* T2_DATA			 */
   { MDS_LLL_ALPHA,	  4},	/* T1_DATA                       */
   { MDS_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { MDS_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { MDS_FIX_ALPHA,	  2},	/* ACTION CODE  		 */
   { MDS_FIX_ALPHA,	  3},	/* Service Restriction Code	 */

   { MDS_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { MDS_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { MDS_FIX_ALPHA,	 40},	/* TERMINAL_ADR			 */
   { MDS_LL_ALPHA,	 25},	/* ADTNL_RESP_DATA		 */
   { MDS_LL_ALPHA,	 77},	/* T1_DATA			 */
   { MDS_LLL_ALPHA,	255},	/* Additrional Data Mds		 */
   { MDS_LLL_ALPHA,	255},	/* Additrional Data National	 */
   /** { MDS_LLL_BIN,	999},**/	/* ADTNL_DATA_PRIV		 */
   { MDS_LLL_ALPHA,	999},	/* ADTNL_DATA_PRIV		 */
   { MDS_FIX_ALPHA,	  3},	/* TRANS_CRNCY			 */
   { MDS_FIX_ALPHA,	  3},	/* STLMNT_CRNCY			 */

   { MDS_FIX_ALPHA,	  3},	/* BILLING_CRNCY		 */
   { MDS_FIX_HEX,	 16},	/* PIN_DATA			 */
   { MDS_FIX_ALPHA,	 16},	/* SECURITY_DATA		 */
   { MDS_LLL_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { MDS_LLL_BIN,	255},	/* ICC Information               */
   { MDS_LLL_ALPHA,	255},	/* Orig Data Elements            */
   { MDS_LLL_ALPHA,	  3},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	255},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	 14},	/* GEOGR_DATA			 */
   { MDS_LLL_ALPHA,	  100},	/* ADTNL_POS_INFO		 */  /* CFE_18062020*/
	
   { MDS_LLL_ALPHA,	 36},	/* OTHER_AMOUNTS		 */
   { MDS_LLL_ALPHA, 	999},	/* Redemption Number		 */
   { MDS_LLL_ALPHA, 	999},	/* Versions       		 */
   { NOT_USED,	  	  8},	/* MAC1				 */
   { NOT_USED,	  	  0},	/* 				 */
   { MDS_FIX_ALPHA,	  1},	/* STLMNT_CODE			 */
   { MDS_FIX_ALPHA,	  2},	/* Extended Payment Code	*/
   { MDS_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		 */
   { MDS_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		 */
   { MDS_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */

   { MDS_FIX_ALPHA,	  4},	/* Message Number		 */
   { MDS_FIX_ALPHA,	  4},	/* Data Record        		 */
   { MDS_FIX_ALPHA,	  6},	/* ACTION_DATE			 */
   { MDS_FIX_ALPHA,	 10},	/* CR_NBR			 */
   { MDS_FIX_ALPHA,	 10},	/* CR_REV_NBR			 */
   { MDS_FIX_ALPHA,	 10},	/* DB_NBR			 */
   { MDS_FIX_ALPHA,	 10},	/* DB_REV_NBR			 */
   { MDS_FIX_ALPHA,	 10},	/* Transfer Number		 */
   { MDS_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
   { MDS_FIX_ALPHA,	 10},	/* Inquiries Number		 */

   { MDS_FIX_ALPHA,	 10},	/* Authorizations Number	 */
   { MDS_FIX_ALPHA,	 12},	/* Credit, Processing Fee Amount */
   { MDS_FIX_ALPHA,	 12},	/* Payment Number                 */
   { MDS_FIX_ALPHA,	 12},	/* Payment Reversal Number     	 */
   { MDS_FIX_ALPHA,	 12},	/* Debit, Transaction Fee Amount */
   { MDS_FIX_ALPHA,	 16},	/* CR_AMOUNT			 */
   { MDS_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		 */
   { MDS_FIX_ALPHA,	 16},	/* DB_AMOUNT			 */
   { MDS_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		 */
   { MDS_FIX_ALPHA,	 42},	/* ORIG_DATA			 */

   { MDS_FIX_ALPHA,	  1},	/* FILE_UPDATE			 */
   { MDS_FIX_ALPHA,	  2},	/* FILE_SECURITY		 */
   { MDS_FIX_ALPHA,	  5},	/* Destination Inst ID Code      */
   { MDS_FIX_ALPHA,	  7},	/* Source Inst Id Code           */
   { MDS_FIX_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
   { MDS_FIX_BIN  ,	 8},	/* Additional Key Mngmt data    KKKK */ 
   { MDS_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { MDS_FIX_ALPHA,	 25},	/* Payee			 */
   { MDS_LL_ALPHA,	 11},	/* STLMNT_INSTITN		 */
   { MDS_LL_ALPHA,	 11},	/* RECEIVER_ID			 */

   { MDS_LL_ALPHA,	 17},	/* FILE_NAME			 */
   { MDS_LL_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { MDS_LL_ALPHA,	 28},	/* ACCOUNT_ID2			 */
   { MDS_LLL_ALPHA,	 999},	/* Digital Payment Data	 */
   { MDS_LLL_ALPHA,	 16},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	 16},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	 10},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	 MDS_F108_MAX_LEN},	/* MDS_MONEYSEND_REF_DATA			 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved Mds			 */

   { MDS_LLL_ALPHA,	  0},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	  0},	/* Intra Country Data		 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	  0},	/* Original Message Type Id	 */

   { MDS_LLL_ALPHA,	  0},	/* Issuing Institution Id	 */
   { MDS_LLL_ALPHA,	  0},	/* Remaining Open to Use	 */
   { MDS_LLL_ALPHA,	 29},	/* ADR_VERIF_DATA		 */
   { MDS_LLL_ALPHA,	  255},	/* Free Form Text - Japan	 */
   { /*MDS_LLL_ALPHA*/MDS_FIX_HEX,	/*255*/16},	/* SUPPORT_INFO			 *//*NAB11022020*/ /*PLUTONL-1082*/
   { MDS_LLL_ALPHA,	  0},	/* Reserved Private		 */
   { MDS_LLL_ALPHA,	  0},	/* File Record Action and Data	 */
   { NOT_USED,	     	  8},	/* MAC2				 */

};

fld_info_t    tab_Mds93FldInfo[] =
{
   { NOT_USED,  	  0},	/* BIT_MAP1			 */
   { MDS_LL_ALPHA,	 19},   /* CARD_NBR 			 */
   { MDS_FIX_ALPHA,	  6},	/* PROC_CODE			 */
   { MDS_FIX_ALPHA,	 12},	/* TRANS_AMOUNT			 */
   { MDS_FIX_ALPHA,	 12},	/* STLMNT_AMOUNT		 */
   { MDS_FIX_ALPHA,	 12},	/* Cardholder Billing Amount	 */
   { MDS_FIX_ALPHA,	 10},	/* XMIT_TIME			 */
   { MDS_FIX_ALPHA,	  8},	/* Cardholder Billing Fee	 */
   { MDS_FIX_ALPHA,	  8},	/* CONVRSN_RATE_STLMNT		 */
   { MDS_FIX_ALPHA,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

   { MDS_FIX_ALPHA,	  6},	/* AUDIT_NBR			 */
   { MDS_FIX_ALPHA,	 12},	/* TRANS_TIME                    */
   { MDS_FIX_ALPHA,	  4},	/* TRANS_DATE			 */
   { MDS_FIX_ALPHA,	  4},	/* EXPIRY_DATE			 */
   { MDS_FIX_ALPHA,	  4},	/* STLMNT_DATE			 */
   { MDS_FIX_ALPHA,	  4},	/* CNVRSN_DATE			 */
   { MDS_FIX_ALPHA,	  4},	/* CAPTURE_DATE			 */
   { MDS_FIX_ALPHA,	  4},	/* MERCHANT_TYPE		 */
   { MDS_FIX_ALPHA,	  3},	/* ACQR_COUNTRY			 */
   { MDS_FIX_ALPHA,	  3},	/* PAN_COUNTRY			 */

   { MDS_FIX_ALPHA,	  3},	/* FORWD_COUNTRY		 */
   { MDS_FIX_ALPHA,	  3},	/* POS_ENTRY_MODE		 */
   { MDS_FIX_ALPHA,	  3},	/* Card Sequence Number		 */
   { MDS_FIX_ALPHA,	  3},	/* Function Code                 */
   { MDS_FIX_ALPHA,	  4},	/* POS_CONDITION		 */
   { MDS_FIX_ALPHA,	  2},	/* MERCHANT TYPE  		 */
   { MDS_FIX_ALPHA,	  1},	/* Authorization Id Response Lenght */
   { MDS_FIX_ALPHA,	  9},	/* TRANS_FEE			 */
   { MDS_FIX_ALPHA,	  9},	/* Amount Settlement Fee	 */
   { MDS_FIX_ALPHA,	  9},	/* Amount Transaction Processing Fee */
	
   { MDS_FIX_ALPHA,	  9},	/* Amount Setlmnt Processing Fee */
   { MDS_LL_ALPHA,	 11},	/* ACQR_ID			 */
   { MDS_LL_ALPHA,	 11},	/* FORWD_ID			 */
   { MDS_LL_ALPHA,	 28},	/* PAN_EXTENDED			 */
   { MDS_LL_ALPHA,	 37},	/* T2_DATA			 */
   { MDS_LLL_ALPHA,	  4},	/* T1_DATA                       */
   { MDS_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
   { MDS_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
   { MDS_FIX_ALPHA,	  3},	/* ACTION CODE  		 */
   { MDS_FIX_ALPHA,	  3},	/* Service Restriction Code	 */

   { MDS_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
   { MDS_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
   { MDS_FIX_ALPHA,	 40},	/* TERMINAL_ADR			 */
   { MDS_LL_ALPHA,	 25},	/* ADTNL_RESP_DATA		 */
   { MDS_LL_ALPHA,	 77},	/* T1_DATA			 */
   { MDS_LLL_ALPHA,	255},	/* Additrional Data Mds		 */
   { MDS_LLL_ALPHA,	255},	/* Additrional Data National	 */
   { MDS_LLL_BIN,	999},	/* ADTNL_DATA_PRIV		 */
   { MDS_FIX_ALPHA,	  3},	/* TRANS_CRNCY			 */
   { MDS_FIX_ALPHA,	  3},	/* STLMNT_CRNCY			 */

   { MDS_FIX_ALPHA,	  3},	/* BILLING_CRNCY		 */
   { MDS_FIX_HEX,	 16},	/* PIN_DATA			 */
   { MDS_FIX_ALPHA,	 16},	/* SECURITY_DATA		 */
   { MDS_LLL_ALPHA,	120},	/* ADTNL_AMOUNTS		 */
   { MDS_LLL_BIN,	255},	/* ICC Information               */
   { MDS_LLL_ALPHA,	255},	/* Orig Data Elements            */
   { MDS_LLL_ALPHA,	  3},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	255},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	 14},	/* GEOGR_DATA			 */
   { MDS_LLL_ALPHA,	 100},	/* ADTNL_POS_INFO		 */ /* CFE_18062020*/
	
   { MDS_LLL_ALPHA,	 36},	/* OTHER_AMOUNTS		 */
   { MDS_LLL_ALPHA, 	999},	/* Redemption Number		 */
   { MDS_LLL_ALPHA, 	999},	/* Versions       		 */
   { NOT_USED,	  	  8},	/* MAC1				 */
   { NOT_USED,	  	  0},	/* 				 */
   { MDS_FIX_ALPHA,	  1},	/* STLMNT_CODE			 */
   { MDS_FIX_ALPHA,	  2},	/* Extended Payment Code	*/
   { MDS_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		 */
   { MDS_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		 */
   { MDS_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */

   { MDS_FIX_ALPHA,	  4},	/* Message Number		 */
   { MDS_FIX_ALPHA,	  4},	/* Data Record        		 */
   { MDS_FIX_ALPHA,	  6},	/* ACTION_DATE			 */
   { MDS_FIX_ALPHA,	 10},	/* CR_NBR			 */
   { MDS_FIX_ALPHA,	 10},	/* CR_REV_NBR			 */
   { MDS_FIX_ALPHA,	 10},	/* DB_NBR			 */
   { MDS_FIX_ALPHA,	 10},	/* DB_REV_NBR			 */
   { MDS_FIX_ALPHA,	 10},	/* Transfer Number		 */
   { MDS_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
   { MDS_FIX_ALPHA,	 10},	/* Inquiries Number		 */

   { MDS_FIX_ALPHA,	 10},	/* Authorizations Number	 */
   { MDS_FIX_ALPHA,	 12},	/* Credit, Processing Fee Amount */
   { MDS_FIX_ALPHA,	 12},	/* Payment Number                 */
   { MDS_FIX_ALPHA,	 12},	/* Payment Reversal Number     	 */
   { MDS_FIX_ALPHA,	 12},	/* Debit, Transaction Fee Amount */
   { MDS_FIX_ALPHA,	 16},	/* CR_AMOUNT			 */
   { MDS_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		 */
   { MDS_FIX_ALPHA,	 16},	/* DB_AMOUNT			 */
   { MDS_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		 */
   { MDS_FIX_ALPHA,	 42},	/* ORIG_DATA			 */

   { MDS_FIX_ALPHA,	  1},	/* FILE_UPDATE			 */
   { MDS_FIX_ALPHA,	  2},	/* FILE_SECURITY		 */
   { MDS_FIX_ALPHA,	  5},	/* Destination Inst ID Code      */
   { MDS_LL_ALPHA,	  7},	/* Source Inst Id Code           */
   { MDS_FIX_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
   { MDS_FIX_HEX  ,	 16},	/* Additional Key Mngmt data     */
   { MDS_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
   { MDS_FIX_ALPHA,	 25},	/* Payee			 */
   { MDS_LL_ALPHA,	 11},	/* STLMNT_INSTITN		 */
   { MDS_LL_ALPHA,	 11},	/* RECEIVER_ID			 */

   { MDS_LL_ALPHA,	 17},	/* FILE_NAME			 */
   { MDS_LL_ALPHA,	 28},	/* ACCOUNT_ID1			 */
   { MDS_LL_ALPHA,	 28},	/* ACCOUNT_ID2			 */
   { MDS_LLL_ALPHA,	 16},	/* Transaction Description	 */
   { MDS_LLL_ALPHA,	 16},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	 16},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	 10},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	 10},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved Mds			 */

   { MDS_LLL_ALPHA,	  0},	/* Reserved Mds			 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	  0},	/* Intra Country Data		 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved National		 */
   { MDS_LLL_ALPHA,	  0},	/* Original Message Type Id	 */

   { MDS_LLL_ALPHA,	  0},	/* Issuing Institution Id	 */
   { MDS_LLL_ALPHA,	  0},	/* Remaining Open to Use	 */
   { MDS_LLL_ALPHA,	 29},	/* ADR_VERIF_DATA		 */
   { MDS_LLL_ALPHA,	  0},	/* Free Form Text - Japan	 */
   { MDS_LLL_ALPHA,	255},	/* SUPPORT_INFO			 */
   { MDS_LLL_ALPHA,	  0},	/* Reserved Private		 */
   { MDS_LLL_ALPHA,	  0},	/* File Record Action and Data	 */
   { NOT_USED,	     	  8},	/* MAC2				 */

};

tag_info_t  tab_MdsTagInfo[] = 
{
   { "7100",  MDS_TAG_VAR_HEX,   0},/* Issuer Script Template 1              */
   { "7200",  MDS_TAG_VAR_HEX,   0},/* Issuer Script Template 2              */
   { "8200",  MDS_TAG_VAR_HEX,   2},/* Application Interchange Profile       */
   { "8400",  MDS_TAG_VAR_HEX/*MDS_TAG_VAR_BCD*/,   0},/* Dedicated File Name                   *//*NAB09062021 PLUTONL-3207*/
   { "9100",  MDS_TAG_VAR_HEX,   0},/* Issuer Authentication Data            */
   { "9500",  MDS_TAG_VAR_BCD,   5},/* Terminal Verification Results         */
   { "9A00",  MDS_TAG_VAR_BCD,   6},/* Transaction Date                      */
   { "9C00",  MDS_TAG_VAR_BCD,   2},/* Transaction TYPE                      */
   { "5F2A",  MDS_TAG_VAR_BCD,   3},/* Transaction Currency Code             */
   { "9F02",  MDS_TAG_VAR_BCD,  12},/* Transaction Amount                    */
   { "9F03",  MDS_TAG_VAR_BCD,  12},/* Other Amount                          */
   { "9F09",  MDS_TAG_VAR_BCD,   2},/* Terminal Application Version Number   */
   { "9F10",  MDS_TAG_VAR_HEX,   0},/*  Issuer Application Data              */
   { "9F1A",  MDS_TAG_VAR_BCD,   3},/* Terminal Country Code                 */
   { "9F1E",  MDS_TAG_VAR_HEX/*MDS_TAG_VAR_ALPHA*/, 0/*8*/},/* Interface Device Serial Number        *//*NAB09062021 PLUTONL-3207*/
   { "9F26",  MDS_TAG_VAR_HEX,   8},/* Application cryptogram                */
   { "9F27",  MDS_TAG_VAR_BCD,   1},/* Cryptogram Information Data           */
   { "9F33",  MDS_TAG_VAR_HEX,   3},/* Terminal Capabilities                 */
   { "9F34",  MDS_TAG_VAR_BCD,   3},/* Cardholder Verification Method        */
   { "9F35",  MDS_TAG_VAR_BCD,   2},/* Terminal Type                         */
   { "9F36",  MDS_TAG_VAR_HEX,   2},/* Application Transaction Counter       */
   { "9F37",  MDS_TAG_VAR_HEX,   4},/* Unpredictable Number                  */
   { "9F41",  MDS_TAG_VAR_BCD,   0},/* Transaction Sequence Number           */
   { "9F53",  MDS_TAG_VAR_BCD, 1},/* Transaction Category Code             */
};

pds_info_t  tab_MdsPdsInfo[] = 
{
   { "01",  MDS_PDS_FIX_ALPHA,  16},/* DES HMK (PBK)                     */
   { "02",  MDS_PDS_FIX_ALPHA,   2},/* Traffic Flags                     */
   { "03",  MDS_PDS_FIX_ALPHA,   2},/* Version Number                    */
   { "04",  MDS_PDS_FIX_ALPHA,   2},/* SIGN-OFF Reason Code              */
   { "05",  MDS_PDS_VAR_ALPHA,  96},/* Rejected Message                  */
   { "06",  MDS_PDS_FIX_ALPHA,   2},/* Reject Reason Code                */
   { "07",  MDS_PDS_FIX_ALPHA,   2},/* Test Production Flags             */
   { "08",  MDS_PDS_FIX_ALPHA,   2},/* PIN Block Format Code             */
   { "21",  MDS_PDS_VAR_ALPHA,   1},/* Acceptance Data            */
   { "20",  MDS_PDS_FIX_ALPHA,   1},/* Cardholder Verification Method    */
   { "22",  MDS_PDS_VAR_ALPHA,  21},/*Multi-Purpose Merchant Indicator*/ 	/*IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/	
   { "23",  MDS_PDS_FIX_ALPHA,   2},/* Payment Initiation Channel        */     /*ACH20200103 */
   { "26",  MDS_PDS_FIX_ALPHA,   3},/* Wallet Program Data        		  */    /*ACH20200103 */
   { "27",  MDS_PDS_VAR_ALPHA,   99},/* Transaction Analysis        	  */    /*ACH20200103 */
   { "30",  MDS_PDS_VAR_ALPHA,   64},/* Token Transaction Identifier	  */    /*ACH20200103 */
   { "33",  MDS_PDS_VAR_ALPHA,   6},/* MasterCard PAN Mapping Info       */     /*ACH20200103 */
   { "40",  MDS_PDS_VAR_ALPHA,  40},/* Certificate Serial Number         */
   { "42",  MDS_PDS_FIX_ALPHA,   19/*3*/},/* Security Level Indicator    */ /*AMER20171003*/
   { "43",  MDS_PDS_VAR_ALPHA,  32},/* UCAF                              */
   { "56",  MDS_PDS_FIX_ALPHA,  6}, /* Security Services                */ /* PLUTONL-5548  AN6773  Enh 2023Q2 AHA20221221*/
   { "61",  MDS_PDS_VAR_ALPHA,   5},/* Mehdi Elyajizi Enh72 : PosData Extention : Terminal acceptance for purchase only amount */   
   { "63",  MDS_PDS_VAR_ALPHA,  15},/* TRACEID                           */
   { "66",  MDS_PDS_VAR_ALPHA,  45},/* Authentication Data                */
   { "67",  MDS_PDS_VAR_ALPHA,  100},/* UCAF                              */
   { "71",  MDS_PDS_VAR_ALPHA,   6},/* MasterCard On-behalf Services     */    /* ACH20191126  PLUTONL-500 */
   { "74",  MDS_PDS_VAR_ALPHA,  30},/* MasterCard Additional Processing Information */    /*AMER20210925 PLUTONL-3612*/
   { "75",  MDS_PDS_VAR_ALPHA,  32},/* Fraud Assessment Prediction Information */  /*AMER20170306: Enh17.Q2*/
   { "77",  MDS_PDS_FIX_ALPHA,  3},/* UCAF                              */
   { "83",  MDS_PDS_FIX_ALPHA,   1},/* Address Verification Service Response */ /* ACH20191126  PLUTONL-500 */
   { "87",  MDS_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Invalid       */
   { "88",  MDS_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Status Change */
   { "89",  MDS_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Track Edit    */
   { "92",  MDS_PDS_FIX_ALPHA,   3},/* Card Verification Code 2          */
   { "95",  MDS_PDS_FIX_ALPHA,   6},/* Mastercard Promotion Code         */
};

pds93_info_t  tab_MdsPds93Info[] = 
{
   { "005",  MDS_PDS_VAR_ALPHA,  16},/* Message Error Indicator           */
   { "601",  MDS_PDS_FIX_ALPHA,   1},/* Entity Selector                   */
   { "602",  MDS_PDS_VAR_ALPHA,   1},/* Regional Listing Data             */
   { "603",  MDS_PDS_FIX_ALPHA,   6},/* Chargeback Effective Date         */
   { "605",  MDS_PDS_FIX_ALPHA,   6},/* Last Update Date                  */
   { "606",  MDS_PDS_FIX_ALPHA,   2},/* Service ID                        */
   { "608",  MDS_PDS_FIX_ALPHA,   4},/* Listing Region                    */
};

/*Start IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/
static const TlvInfoProperties	MDSMultiPurposeMerDataProp = {
2,2,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F048_SE22_LEN,
{
{MDS_F048_SE22_SF01_LOW_RISK_MER_IND,		FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	2,	PP_PRINT,"NULL","LOW_RISK_MER_IND","NULL",NOT_REPEATABLE},
{MDS_F048_SE22_SF02_SINGLE_TAP_IND,			FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	1,	PP_PRINT,"NULL","SINGLE_TAP_IND","NULL",NOT_REPEATABLE},
{MDS_F048_SE22_SF03_RESPONSE_PIN_REQ,		FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	1,	PP_PRINT,"NULL","RESPONSE_PIN_REQ","NULL",NOT_REPEATABLE},
{MDS_F048_SE22_SF04_ISS_PIN_REQ_SIN_TAP,	FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	1,	PP_PRINT,"NULL","ISS_PIN_REQ_SIN_TAP","NULL",NOT_REPEATABLE},
{MDS_F048_SE22_SF05_CH_MER_INIT_IND,		FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	MDS_F048_SE22_SF05_LEN,	PP_PRINT,"NULL","CH_MER_INIT_IND","NULL",NOT_REPEATABLE},/*Start ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
{MDS_F048_SE22_SF06_MSG_FORMAT_VERSION_CODE,		FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	MDS_F048_SE22_SF06_LEN,	PP_PRINT,"NULL","MSG_FORMAT_VERSION_CODE","NULL",NOT_REPEATABLE},/*Start ZKO 03022022 PLUTONL-4015 ENH 22Q2 MC AN5524*/
},
"MDSMultiPurposeMerDataProp"
};
/*End IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/


/*Start IBO20190808 PROD00070934 Enh19.Q4 AN 2628*/
static const TlvInfoProperties	MdsDigitalPaymentDataProp = {
3,3,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F104_NB_FIELDS,
{
{MDS_F104_DIGITAL_PAYMENT_CRYPTO, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,28,PP_PRINT,"NULL","DIGITAL_PAYMENT_CRYPTO","NULL",NOT_REPEATABLE},
/*Start HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/ 
{MDS_F104_DGITAL_PAYEMENT_ESTIMATED_AMOUNT, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_FIX,LU_BYTE,12,PP_PRINT,"NULL","DGITAL_PAYEMENT_ESTIMATED_AMOUNT","NULL",NOT_REPEATABLE},
{MDS_F104_DGITAL_PAYEMENT_REMOTE_COMERCE_ACCEPTOR, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,150,PP_PRINT,"NULL","DGITAL_PAYEMENT_REMOTE_COMERCE_ACCEPTOR","NULL",NOT_REPEATABLE},
/*End HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/ 
},
"MdsDigitalPaymentDataProp"
};
/*End IBO20190808 PROD00070934 Enh19.Q4 AN 2628*/

/*Start IBO20190910 PROD00071519 Enh19.Q4 AN2615*/

static const TlvInfoProperties	MdsMoneySendDataProp = {
2,3,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F108_NB_FIELDS,
{
{MDS_F108_RECIPIENT_DATA, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,358,PP_PRINT,"NULL","RECIPIENT_DATA","NULL",NOT_REPEATABLE}, /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
{MDS_F108_SENDER_DATA, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,358,PP_PRINT,"NULL","SENDER_DATA","NULL",NOT_REPEATABLE},       /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
{MDS_F108_MONEYSEND_TRANSACTION_DATA, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,132,PP_PRINT,"NULL","MONEYSEND_TRANSACTION_DATA","NULL",NOT_REPEATABLE},
{MDS_F108_MONEYSEND_LANGUAGE_DESCRIPTION, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,61,PP_PRINT,"NULL","MONEYSEND_LANGUAGE_DESCRIPTION","NULL",NOT_REPEATABLE},
},
"MdsMoneySendDataProp"
};

static const TlvInfoProperties	MdsMoneySendDataT01Prop = {
2,2,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F108T01_NB_FIELDS,
{
{MDS_F108T01_RECIPIENT_FIRST_NAME, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,/*FL_LLL*/FL_LL,LU_BYTE,35,PP_PRINT,"NULL","RECIPIENT_FIRST_NAME","NULL",NOT_REPEATABLE},/*PLUTONL-10023 Enh 2023Q4 GLB 10432 change FL_LLL to FL_LL*/
{MDS_F108T01_RECIPIENT_MIDDLE_NAME, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LL/*FL_FIX*/,LU_BYTE,35/*1*/,PP_PRINT,"NULL","RECIPIENT_MIDDLE_NAME","NULL",NOT_REPEATABLE},/*PLUTONL-6505 -Enh 2023Q4 NAB :AN 7402*//*PLUTONL-9852 -Enh 2025Q2 NAB GLB 10432*/
{MDS_F108T01_RECIPIENT_LAST_NAME, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,/*FL_LLL*/FL_LL,LU_BYTE,35,PP_PRINT,"NULL","RECIPIENT_LAST_NAME","NULL",NOT_REPEATABLE},/*PLUTONL-10023 Enh 2023Q4 GLB 10432 change FL_LLL to FL_LL*/
{MDS_F108T01_RECIPIENT_STREET_ADDR, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,50,PP_PRINT,"NULL","RECIPIENT_STREET_ADDR","NULL",NOT_REPEATABLE},
{MDS_F108T01_RECIPIENT_CITY, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,25,PP_PRINT,"NULL","RECIPIENT_CITY","NULL",NOT_REPEATABLE},
{MDS_F108T01_RECIPIENT_STATE_PROVINCE_CODE, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,3,PP_PRINT,"NULL","RECIPIENT_STATE_PROVINCE_CODE","NULL",NOT_REPEATABLE},
{MDS_F108T01_RECIPIENT_COUNTRY, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","RECIPIENT_COUNTRY","NULL",NOT_REPEATABLE},
{MDS_F108T01_RECIPIENT_POSTAL_CODE, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,10,PP_PRINT,"NULL","RECIPIENT_POSTAL_CODE","NULL",NOT_REPEATABLE},
{MDS_F108T01_RECIPIENT_PHONE_NUMBER, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_LLL,LU_BYTE,20,PP_PRINT,"NULL","RECIPIENT_PHONE_NUMBER","NULL",NOT_REPEATABLE},
{MDS_F108T01_RECIPIENT_DOB, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_FIX,LU_BYTE,8,PP_PRINT,"NULL","RECIPIENT_DOB","NULL",NOT_REPEATABLE},
{MDS_F108T01_RECIPIENT_ACCOUNT_NUMBER, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,50,PP_PRINT,"NULL","RECIPIENT_ACCOUNT_NUMBER","NULL",NOT_REPEATABLE}, /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
{MDS_F108T01_RECIPIENT_ACCOUNT_TYPE, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_LLL,LU_BYTE,2,PP_PRINT,"NULL","RECIPIENT_ACCOUNT_TYPE","NULL",NOT_REPEATABLE},        /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
},
"MdsMoneySendDataT01Prop"
};

static const TlvInfoProperties	MdsMoneySendDataT02Prop = {
2,2,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F108T02_NB_FIELDS,
{
{MDS_F108T02_SENDER_FIRST_NAME, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,/*FL_LLL*/FL_LL,LU_BYTE,35,PP_PRINT,"NULL","SENDER_FIRST_NAME","NULL",NOT_REPEATABLE},/*PLUTONL-10023 Enh 2023Q4 GLB 10432 change FL_LLL to FL_LL*/
{MDS_F108T02_SENDER_MIDDLE_NAME, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LL/*FL_FIX*/,LU_BYTE,35/*1*/,PP_PRINT,"NULL","SENDER_MIDDLE_NAME","NULL",NOT_REPEATABLE},/*PLUTONL-6505 -Enh 2023Q4 NAB :AN 7402*//*PLUTONL-9852 -Enh 2025Q2 NAB GLB 10432*/
{MDS_F108T02_SENDER_LAST_NAME, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,/*FL_LLL*/FL_LL,LU_BYTE,35,PP_PRINT,"NULL","SENDER_LAST_NAME","NULL",NOT_REPEATABLE},/*PLUTONL-10023 Enh 2023Q4 GLB 10432 change FL_LLL to FL_LL*/
{MDS_F108T02_SENDER_STREET_ADDR, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,50,PP_PRINT,"NULL","SENDER_STREET_ADDR","NULL",NOT_REPEATABLE},
{MDS_F108T02_SENDER_CITY, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,25,PP_PRINT,"NULL","SENDER_CITY","NULL",NOT_REPEATABLE},
{MDS_F108T02_SENDER_STATE_PROVINCE_CODE, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,3,PP_PRINT,"NULL","SENDER_STATE_PROVINCE_CODE","NULL",NOT_REPEATABLE},
{MDS_F108T02_SENDER_COUNTRY, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","SENDER_COUNTRY","NULL",NOT_REPEATABLE},
{MDS_F108T02_SENDER_POSTAL_CODE, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,10,PP_PRINT,"NULL","SENDER_POSTAL_CODE","NULL",NOT_REPEATABLE},
{MDS_F108T02_SENDER_PHONE_NUMBER, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_LLL,LU_BYTE,20,PP_PRINT,"NULL","SENDER_PHONE_NUMBER","NULL",NOT_REPEATABLE},
{MDS_F108T02_SENDER_DOB, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_FIX,LU_BYTE,8,PP_PRINT,"NULL","SENDER_DOB","NULL",NOT_REPEATABLE},
{MDS_F108T02_SENDER_ACCOUNT_NUMBER, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,50,PP_PRINT,"NULL","SENDER_ACCOUNT_NUMBER","NULL",NOT_REPEATABLE},/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
{MDS_F108T02_SENDER_ACCOUNT_TYPE, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_LLL,LU_BYTE,2,PP_PRINT,"NULL","SENDER_ACCOUNT_TYPE","NULL",NOT_REPEATABLE},       /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022*/
},
"MdsMoneySendDataT02Prop"
};


static const TlvInfoProperties	MdsMoneySendDataT03Prop = {
2,2,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F108T03_NB_FIELDS,
{
{MDS_F108T03_UNIQUE_TRANSACTION_REFERENCE, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,19,PP_PRINT,"NULL","UNIQUE_TRANSACTION_REFERENCE","NULL",NOT_REPEATABLE},
{MDS_F108T03_ADDITIONAL_MESSAGE, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,65,PP_PRINT,"NULL","ADDITIONAL_MESSAGE","NULL",NOT_REPEATABLE},
{MDS_F108T03_FUNDING_SOURCE, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","FUNDING_SOURCE","NULL",NOT_REPEATABLE},
{MDS_F108T03_PARTICIPANT_ID, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,30,PP_PRINT,"NULL","PARTICIPANT_ID","NULL",NOT_REPEATABLE},
{MDS_F108T03_TRANSACTION_PURPOSE, FT_SINGLE,FF_ALPHA_N,AF_ASCII,FL_FIX,LU_BYTE,2,PP_PRINT,"NULL","TRANSACTION_PURPOSE","NULL",NOT_REPEATABLE},	/*AEA20210712 Enh21.Q4 AN4972*/
},
"MdsMoneySendDataT03Prop"
};


static const TlvInfoProperties	MdsMoneySendDataT04Prop = {
2,2,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F108T04_NB_FIELDS,
{
{MDS_F108T04_LANGUAGE_IDENTIFICATION, FT_SINGLE,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","LANGUAGE_IDENTIFICATION","NULL",NOT_REPEATABLE},
{MDS_F108T04_LANGUAGE_DATA, FT_SINGLE,FF_BIN,AF_ASCII,FL_LLL,LU_BYTE,50,PP_PRINT,"NULL","LANGUAGE_DATA","NULL",NOT_REPEATABLE},
},
"MdsMoneySendDataT04Prop"
};
/*End IBO20190910 PROD00071519 Enh19.Q4 AN2615*/

/*Start ACH20191219*/
static const TlvInfoProperties	MDSPanMappingDataProp = {
2,2,TLV_LT_ASCII,TLV_LT_ASCII,6,
{
{MDS_F048_T33_T01_ACC_NO_IND,	FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	1,	PP_PRINT,"NULL","ACC_NO_IND","NULL",NOT_REPEATABLE},
{MDS_F048_T33_T02_ACC_NO,		FT_SINGLE,	FF_ALPHA_N,		AF_ASCII,FL_FIX,LU_BYTE,	19,	PP_PRINT,"NULL","ACC_NO","NULL",NOT_REPEATABLE},
{MDS_F048_T33_T03_EXP_DT,		FT_SINGLE,	FF_ALPHA_N,		AF_ASCII,FL_FIX,LU_BYTE,	4,	PP_PRINT,"NULL","EXP_DT","NULL",NOT_REPEATABLE},
{MDS_F048_T33_T05_TOKEN_ASS_LVL, FT_SINGLE,	FF_ALPHA_N,		AF_ASCII,FL_FIX,LU_BYTE,	2,	PP_PRINT,"NULL","TOKEN_ASS_LVL","NULL",NOT_REPEATABLE},
{MDS_F048_T33_T06_TOKEN_REQ_ID, FT_SINGLE,	FF_ALPHA_N,		AF_ASCII,FL_FIX,LU_BYTE,	11,	PP_PRINT,"NULL","TOKEN_REQ_ID","NULL",NOT_REPEATABLE},
{MDS_F048_T33_T08_STORAGE_TECH, FT_SINGLE,  FF_ALPHA_AN,    AF_ASCII,FL_FIX,LU_BYTE,    2,  PP_PRINT,"NULL","STORAGE_TECH","NULL",NOT_REPEATABLE},  
},
"MDSPanMappingDataProp"
};
/*Start ACH20191219*/

/* start AHA20220829 PLUTONL-4980*/
/*start cis 110*/
static const TlvInfoProperties	MdsAdditionalDataProp = {
2,3,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F110_NB_FIELDS,
{
{MDS_AD_KEY_BLOCK_PROTECT, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,80,PP_HIDE,"NULL","KEY_BLOCK_PROTECTION_KEY","NULL",NOT_REPEATABLE},
{MDS_AD_KEY_CHECK_VALUE, FT_SINGLE,FF_ALPHA_AN,AF_ASCII,FL_FIX,LU_BYTE,6,PP_PRINT,"NULL","KEY_CHECK_VALUE","NULL",NOT_REPEATABLE},
},
"MdsAdditionalDataProp"
};
/* end cis 110 */
/*end AHA20220829 PLUTONL-4980*/

/*START PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	

static const TlvInfoProperties	MdsAdditionalRecordData = {
3,3,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F122_NB_FIELDS,
{
{MDS_F122_SE_001_ADD_ACPT_DATA,			         FT_STRUCT_TLV,	FF_ALPHA_ANS,	AF_ASCII,FL_LLL,LU_BYTE,	491,	PP_PRINT,"NULL","ADD_ACPT_DATA","NULL",NOT_REPEATABLE},
},
"MdsAdditionalRecordData"
};

static const TlvInfoProperties	MdsAdditionalAcceptorData = {
3,3,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F122T001_001_NB_FIELDS,
{
{MDS_F122_ACPT_STREET_ADDR,			         FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	48,	PP_PRINT,"NULL","ACCEPTOR_STREET_ADDRESS","NULL",NOT_REPEATABLE},
{MDS_F122_ACPT_COUNTRY_SUB_CODE,			      FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	3,	PP_PRINT,"NULL","ACCEPTOR_COUNTRY_SUBDIVISION_CODE","NULL",NOT_REPEATABLE},
{MDS_F122_ACPT_URL_ADDR,			            FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	255,	PP_PRINT,"NULL","ACCEPTOR_URL_ADDRESS,","NULL",NOT_REPEATABLE},
{MDS_F122_ACPT_CUSTOMER_SVC_PHONE_NBR,			FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	16,	PP_PRINT,"NULL","ACCEPTOR_CUSTOMER_SERVICE_PHONE_NUMBER","NULL",NOT_REPEATABLE},
{MDS_F122_ACPT_PHONE_NBR,			            FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	16,	PP_PRINT,"NULL","ACCEPTOR_PHONE_NUMBER","NULL",NOT_REPEATABLE},
{MDS_F122_ACPT_ADD_CONTACT_INFO,			      FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	25,	PP_PRINT,"NULL","ACCEPTOR_ADDITIONAL_CONTACT_INFORMATION","NULL",NOT_REPEATABLE},
{MDS_F122_ACPT_TAX_ID,			               FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	21,	PP_PRINT,"NULL","ACCEPTOR_TAX_ID","NULL",NOT_REPEATABLE},
{MDS_F122_PARTNER_ID_CODE,			            FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	6,	PP_PRINT,"NULL","PARTNER_ID_CODE","NULL",NOT_REPEATABLE},
{MDS_F122_SVC_LOC_CITY_NAME,			         FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	13,	PP_PRINT,"NULL","SERVICE_LOCATION_CITY_NAME","NULL",NOT_REPEATABLE},
{MDS_F122_SVC_LOC_COUNTRY_SUB_CODE,		      FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	3,	PP_PRINT,"NULL","SERVICE_LOCATION_COUNTRY_SUBDIVISION_CODE","NULL",NOT_REPEATABLE},
{MDS_F122_SVC_LOC_COUNTRY_CODE,			      FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	3,	PP_PRINT,"NULL","SERVICE_LOCATION_COUNTRY_CODE","NULL",NOT_REPEATABLE},
{MDS_F122_SVC_LOC_POSTAL_CODE,			      FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_LLL,LU_BYTE,	10,	PP_PRINT,"NULL","SERVICE_LOCATION_POSTAL_CODE","NULL",NOT_REPEATABLE},
},
"MdsAdditionalAcceptorData"
};


/*END PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	

/*Start PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	
static const TlvInfoProperties	MdsAdditionalMerchantDataProp = {
2,2,TLV_LT_ASCII,TLV_LT_ASCII,MDS_F048_SE37_NB_FIELDS,
{
{MDS_F048_SE37_SF01_PAYFAC_ID,			FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	11,	PP_PRINT,"NULL","PAYFAC_ID","NULL",NOT_REPEATABLE},
{MDS_F048_SE37_SF02_ISO_ID,				FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	11,	PP_PRINT,"NULL","ISO_ID","NULL",NOT_REPEATABLE},
{MDS_F048_SE37_SF03_SUB_MER_ID,			FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	15,	PP_PRINT,"NULL","SUB_MER_ID","NULL",NOT_REPEATABLE},
{MDS_F048_SE37_SF04_ORIGINE_COUNTRY,	FT_SINGLE,	FF_ALPHA_AN,	AF_ASCII,FL_FIX,LU_BYTE,	3,	PP_PRINT,"NULL","ORIGINE_COUNTRY","NULL",NOT_REPEATABLE},
},
"MdsAdditionalMerchantDataProp"
};
/*END PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	

void  InitMdsIcTag  (TSTagMds * msgInfo)
{
   int i;

   for (i = 0; i < MAX_MDS_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', MAX_MDS_CHIP_LEN);
}

void  InitMdsPds  (TSPdsMds * pdsInfo)
{
   int i;

   for (i = 0; i < MAX_MDS_PDS; i++)
   {
      pdsInfo->nPosPds [i] = 0;
      pdsInfo->nPresent[i] = NOT_PRESENT;
   }
   pdsInfo->nLength  = 0;
   pdsInfo->nMsgType = 0;
   memset(pdsInfo->sPdsData,   '\0', MAX_MDS_PDS_LEN);
}

void  InitMdsInfo(TSMdsInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_MDS_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < MDS_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_MDS_DATA_LEN);
}

int GetMdsFieldType(int field_no)
{
   return(tab_MdsFldInfo[field_no].type);
}

int GetMdsFieldLength(int field_no)
{
   return(tab_MdsFldInfo[field_no].length);
}

int GetMds93FieldType(int field_no)
{
   return(tab_Mds93FldInfo[field_no].type);
}

int GetMds93FieldLength(int field_no)
{
   return(tab_Mds93FldInfo[field_no].length);
}

int GetMdsTagIndice ( char * szTagName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_MdsTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_MdsTagInfo[nIndice].sTagName, 4) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetMdsTagType ( int nIndice )
{
   return ( tab_MdsTagInfo[nIndice].nType);
}

int GetMdsTagLength ( int nIndice )
{
   return ( tab_MdsTagInfo[nIndice].nLength);
}

int GetMdsPdsIndice ( char * szPdsName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_MdsPdsInfo) /sizeof(pds_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szPdsName, tab_MdsPdsInfo[nIndice].sPdsName, 2) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetMdsPdsType ( int nIndice )
{
   return ( tab_MdsPdsInfo[nIndice].nType);
}

int GetMdsPdsLength ( int nIndice )
{
   return ( tab_MdsPdsInfo[nIndice].nLength);
}

int GetMdsPds93Indice ( char * szPdsName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_MdsPds93Info) /sizeof(pds93_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szPdsName, tab_MdsPds93Info[nIndice].sPdsName, 3) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetMdsPds93Type ( int nIndice )
{
   return ( tab_MdsPds93Info[nIndice].nType);
}

int GetMdsPds93Length ( int nIndice )
{
   return ( tab_MdsPds93Info[nIndice].nLength);
}

/******************************************************************************/
void InitMdsPanMappingDataTlvInfo(TlvInfo* pkTlvInfo) /*ACH20191219*/
{
	pkTlvInfo->pkProperties = &MDSPanMappingDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
void InitMdsMultiPurposeMerDataTlvInfo(TlvInfo* pkTlvInfo)    /*IBO20190806 PROD00070896 Enh19.Q3 AN 2609*/
{
	pkTlvInfo->pkProperties = &MDSMultiPurposeMerDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
void InitMdsDigitalPaymentDataTlvInfo(TlvInfo* pkTlvInfo)    /*IBO20190808 PROD00070934 Enh19.Q4 AN 2628*/
{
	pkTlvInfo->pkProperties = &MdsDigitalPaymentDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/*Start IBO20190910 PROD00071519 Enh19.Q4 AN2615*/
/******************************************************************************/
void InitMdsMoneySendDataTlv(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &MdsMoneySendDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
void InitMdsMSRecipientDataTlv(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &MdsMoneySendDataT01Prop;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
void InitMdsMSSenderDataTlv(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &MdsMoneySendDataT02Prop;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
void InitMdsMSTransDataTlv(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &MdsMoneySendDataT03Prop;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
void InitMdsMSLangDataTlv(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &MdsMoneySendDataT04Prop;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
/*End IBO20190910 PROD00071519 Enh19.Q4 AN2615*/
/******************************************************************************/

/* start AHA20220829 PLUTONL-4980*/
void InitMdsAdditionalDataTlv(TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = &MdsAdditionalDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/* end AHA20220829 PLUTONL-4980*/

/******************************************************************************/
void InitMdsAdditionalAcceptorData(TlvInfo* pkTlvInfo) /* PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	  
{
	pkTlvInfo->pkProperties = &MdsAdditionalAcceptorData;
	ResetTlvInfo(pkTlvInfo);
} 

/******************************************************************************/
void InitMdsAdditionalRecordData(TlvInfo* pkTlvInfo) /* PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	  
{
	pkTlvInfo->pkProperties = &MdsAdditionalRecordData;
	ResetTlvInfo(pkTlvInfo);
} 
/******************************************************************************/

void InitMdsAddMerchantDataTlvInfo(TlvInfo* pkTlvInfo) /*Start PLUTONL-5548   AN6022  Enh 2023Q2 AHA20221221*/	   
{
	pkTlvInfo->pkProperties = &MdsAdditionalMerchantDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
/* end AHA20220829 PLUTONL-4980*/
