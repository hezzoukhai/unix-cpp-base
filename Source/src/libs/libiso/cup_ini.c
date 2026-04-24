#include <stdio.h>
#include <stdlib.h>
#include <iso_cup.h>
#include <header_cup.h>
#include <define.h>
#include <iso8583.h>
#include <cup_define.h>


static fld_info_t    tab_CupFldInfo[] =
{
	{ CUP_FIX_BITMAP,	  0},	/* BIT_MAP2			 */
	{ CUP_LL_ALPHA,	 	 19},   /* CARD_NBR 			 */
	{ CUP_FIX_ALPHA,	  6},	/* PROC_CODE			 */
	{ CUP_FIX_ALPHA,	 12},	/* TRANS_AMOUNT			 */
	{ CUP_FIX_ALPHA,	 12},	/* STLMNT_AMOUNT		 */
	{ CUP_FIX_ALPHA,	 12},	/* Cardholder Billing Amount	 */
	{ CUP_FIX_ALPHA,	 10},	/* XMIT_TIME			 */
	{ CUP_FIX_ALPHA,	  8},	/* Cardholder Billing Fee	 */
	{ CUP_FIX_ALPHA,	  8},	/* CONVRSN_RATE_STLMNT		 */
	{ CUP_FIX_ALPHA,	  8},	/* CONVRSN_RATE_CARD_HLDR_BILLING */

	{ CUP_FIX_ALPHA,	  6},	/* AUDIT_NBR			 */
	{ CUP_FIX_ALPHA,	  6},	/* TRANS_TIME			 */
	{ CUP_FIX_ALPHA,	  4},	/* TRANS_DATE			 */
	{ CUP_FIX_ALPHA,	  4},	/* EXPIRY_DATE			 */
	{ CUP_FIX_ALPHA,	  4},	/* STLMNT_DATE			 */
	{ CUP_FIX_ALPHA,	  4},	/* CNVRSN_DATE			 */
	{ CUP_FIX_ALPHA,	  4},	/* CAPTURE_DATE			 */
	{ CUP_FIX_ALPHA,	  4},	/* MERCHANT_TYPE		 */
	{ CUP_FIX_ALPHA,	  3},	/* ACQR_COUNTRY			 */
	{ CUP_FIX_ALPHA,	  3},	/* PAN_COUNTRY			 */

	{ CUP_FIX_ALPHA,	  3},	/* FORWD_COUNTRY		 */
	{ CUP_FIX_ALPHA,	  3},	/* POS_ENTRY_MODE		 */
	{ CUP_FIX_ALPHA,	  3},	/* Card Sequence Number		 */
	{ CUP_FIX_ALPHA,	  3},	/* Network International Identifier  */
	{ CUP_FIX_ALPHA,	  2},	/* POS_CONDITION		 */
	{ CUP_FIX_ALPHA,	  2},	/* POS_PIN_CAPTURE		 */
	{ CUP_FIX_ALPHA,	  1},	/* Authorization Id Response Lenght */
	{ CUP_FIX_ALPHA,	  9},	/* TRANS_FEE			 */
	{ CUP_FIX_ALPHA,	  9},	/* Amount Settlement Fee	 */
	{ CUP_FIX_ALPHA,	  9},	/* Amount Transaction Processing Fee */

	{ CUP_FIX_ALPHA,	  9},	/* Amount Setlmnt Processing Fee */
	{ CUP_LL_ALPHA,	 	 11},	/* ACQR_ID			 */
	{ CUP_LL_ALPHA,	 	 11},	/* FORWD_ID			 */
	{ CUP_FIX_ALPHA,	 28},	/* PAN_EXTENDED			 */
	{ CUP_LL_ALPHA,	 	 37},	/* T2_DATA			 */
	{ CUP_LLL_ALPHA,	104},	/* Track 3 Data			 */
	{ CUP_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
	{ CUP_FIX_ALPHA,	  6},	/* AUTHOR_ID			 */
	{ CUP_FIX_ALPHA,	  2},	/* RESPONSE_CODE		 */
	{ CUP_FIX_ALPHA,	  3},	/* Service Restriction Code	 */

	{ CUP_FIX_ALPHA,	  8},	/* TERMINAL_NBR			 */
	{ CUP_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
	{ CUP_FIX_ALPHA,	 40},	/* TERMINAL_ADR			 */
	{ CUP_LL_ALPHA,	 	 25},	/* ADTNL_RESP_DATA		 */
	{ CUP_LL_ALPHA,	 	 79},	/* T1_DATA			 */
	{ CUP_LLL_ALPHA,	255},	/* Additrional Data Cup		 */
	{ CUP_LLL_ALPHA,	255},	/* Additrional Data National	 */
	{ CUP_LLL_ALPHA,	512},	/* ADTNL_DATA_PRIV		 */
	{ CUP_FIX_ALPHA,	  3},	/* TRANS_CRNCY			 */
	{ CUP_FIX_ALPHA,	  3},	/* STLMNT_CRNCY			 */

	{ CUP_FIX_ALPHA,	  3},	/* BILLING_CRNCY		 */
	{ CUP_FIX_HEX,	 	 16},	/* PIN_DATA			 */
	{ CUP_FIX_ALPHA,	 16},	/* SECURITY_DATA		 */
	{ CUP_LLL_ALPHA,	 40},	/* ADTNL_AMOUNTS		 */
	{ CUP_LLL_ALPHA,	255},	/* IC Card Data                  */
	{ CUP_LLL_ALPHA,	255},	/* Reserved Cup			 */
	{ CUP_LLL_ALPHA,	100},	/* Additional Data Private	 */
	{ CUP_LLL_ALPHA,	600},	/* Reserved National		 */
	{ CUP_LLL_ALPHA,	600},	/* Detailed Inquiry Data1	 */
	{ CUP_LLL_ALPHA,	100},	/* Self-Defined Field		 */

	{ CUP_LLL_ALPHA,	200},	/* Cardholder Authentication Information		 */
	{ CUP_LLL_ALPHA, 	200},	/* Switching Data (Not used)*/
	{ CUP_LLL_ALPHA, 	512},	/* Financial Network Data		 */
	{ CUP_FIX_HEX,	  	 16},	/* MAC1				 */
	{ NOT_USED,	  	  0},	/* 				 */
	{ CUP_FIX_ALPHA,	  1},	/* STLMNT_CODE			 */
	{ CUP_FIX_ALPHA,	  1},	/* Extended Payment Code	*/
	{ CUP_FIX_ALPHA,	  3},	/* RECEIVER_COUNTRY		 */
	{ CUP_FIX_ALPHA,	  3},	/* STLMNT_INST_COUNTRY		 */
	{ CUP_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */

	{ CUP_FIX_ALPHA,	  4},	/* Message Number		 */
	{ CUP_FIX_ALPHA,	  4},	/* Message number Last		 */
	{ CUP_FIX_ALPHA,	  6},	/* ACTION_DATE			 */
	{ NOT_USED,	  	  0},	/* CR_NBR			 */
	{ NOT_USED,	  	  0},	/* CR_REV_NBR			 */
	{ NOT_USED,	  	  0},	/* DB_NBR			 */
	{ NOT_USED,	  	  0},	/* DB_REV_NBR			 */
	{ CUP_FIX_ALPHA,	 10},	/* Transfer Number		 */
	{ CUP_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
	{ CUP_FIX_ALPHA,	 10},	/* Inquiries Number		 */

	{ CUP_FIX_ALPHA,	 10},	/* Authorizations Number	 */
	{ CUP_FIX_ALPHA,	 12},	/* Credit, Processing Fee Amount */
	{ CUP_FIX_ALPHA,	 12},	/* Credit, Transaction Fee Amount */
	{ CUP_FIX_ALPHA,	 12},	/* Debit, Processing Fee Amount	 */
	{ CUP_FIX_ALPHA,	 12},	/* Debit, Transaction Fee Amount */
	{ CUP_FIX_ALPHA,	 16},	/* CR_AMOUNT			 */
	{ CUP_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		 */
	{ CUP_FIX_ALPHA,	 16},	/* DB_AMOUNT			 */
	{ CUP_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		 */
	{ CUP_FIX_ALPHA,	 42},	/* ORIG_DATA			 */

	{ CUP_FIX_ALPHA,	  1},	/* FILE_UPDATE			 */
	{ CUP_FIX_ALPHA,	  2},	/* FILE_SECURITY		 */
	{ CUP_FIX_ALPHA,	  5},	/* Response Indicator		 */
	{ CUP_FIX_ALPHA,	  7},	/* Service Indicator		 */
	{ CUP_FIX_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
	{ CUP_FIX_HEX,	  	 16},	/* MSG_SECURITY			 */
	{ CUP_FIX_ALPHA,	 17},	/* NET_AMOUNT			 */
	{ NOT_USED,	  	  0},		/*      			 */
	{ CUP_LLL_ALPHA,	 11},	/* STLMNT_INSTITN		 */
	{ CUP_LL_ALPHA,	 	 11},	/* RECEIVER_ID			 */

	{ CUP_LLL_ALPHA,	 17},	/* FILE_NAME			 */
	{ CUP_LL_ALPHA,		 28},	/* ACCOUNT_ID1			 */
	{ CUP_LLL_ALPHA,	 28},	/* ACCOUNT_ID2			 */ /* 103 */
	{ CUP_LLL_BIN,	512},	/* Transaction and Industry Application Information 	 */
	{ NOT_USED,	  	  0},	/* Reserved Cup			 */
	{ NOT_USED,	  	  0},	/* Reserved Cup			 */
	{ NOT_USED,	  	  0},	/* Reserved Cup			 */
	{ NOT_USED,	  	  0},	/* Reserved Cup			 */
	{ NOT_USED,	  	  0},	/* Reserved Cup			 */
	{ NOT_USED,	  	  0},	/* Reserved Cup			 */

	{ NOT_USED,	  	  0},	/* Reserved Cup			 */
	{ NOT_USED,	  	  0},	/* Reserved National		 */
	{ NOT_USED,	  	  0},	/* Reserved National		 */
	{ NOT_USED,	  	  0},	/* Reserved National		 */
	{ CUP_LLL_ALPHA,	 24},	/* ADTNL_TRACE_DATA		 */
        { NOT_USED,               0},   /* Reserved National             */
	{ CUP_LLL_BIN,	  	256},	/* National and Regional Information		 */
	{ NOT_USED,	  	  0},	/* Intra Country Data		 */
	{ NOT_USED,	  	  0},	/* Reserved National		 */
	{ CUP_LLL_ALPHA,	  4},	/* ORIGINAL MESSAGE TYPE ID	 */

	{ CUP_LLL_ALPHA,	 100},	/* CUPS Reserved	 */
	{ CUP_LLL_ALPHA,	 100},	/* Acquirer Institution Reserved	 */
	{ CUP_LLL_ALPHA,	 100},	/* Issuer Institution Reserved	 */
	{ CUP_LLL_ALPHA,	 100},	/* Free Form Text - Japan	 */
	{ CUP_LLL_BIN/*CUP_LLL_ALPHA*/,	 256},	/* Security and Risk Assessment Information		F125 *//*NAB*/
	{ CUP_LLL_ALPHA,	 255},   /* CUP PRIVATE USE FIELD */
	{ CUP_LLL_ALPHA,	 255},	/* FILE RECORD ACTION AND DATA	 */
	{ CUP_FIX_HEX,	  	  16},	/* MAC2				 */
	{ CUP_FIX_ALPHA,  	  8},	/* Field 129			 */
	{ CUP_FIX_ALPHA,	  8},	/* Terminal Capability Profile   */ /* 130 */

	{ CUP_FIX_ALPHA,	  8},	/* Terminal Verification Results */
	{ CUP_FIX_ALPHA,	  8},	/* Unpredictable Number          */
	{ CUP_FIX_ALPHA,	  16},	/* Terminal Serial Number        */
	{ CUP_LLL_BIN,	 	  16},	/* Cup Discretionary Data       */
	{ CUP_LLL_ALPHA,	  39},	/* Issuer Discretionary Data     */
	{ CUP_FIX_HEX,	 	  16},	/* Issuer Discretionary Data     */
	{ CUP_FIX_HEX,	  	   4},	/* Application Trans. Counter    */
	{ CUP_FIX_HEX,	  	   4},	/* Application Inter. Profile    */
	{ CUP_FIX_HEX,	 	  20},	/* ARPC + Response Code          */
	{ NOT_USED,	  	   0},	/* Field 140			 */

	{ NOT_USED,	  	   0},	/* Field 141			 */
	{ CUP_LLL_HEX,		 510},	/* Issuer Script                 */
	{ CUP_LLL_HEX,	 	  40},	/* Issuer Script Results         */
	{ CUP_FIX_ALPHA,	  2},	/* Cryptogram Trans Type         */
	{ CUP_FIX_ALPHA,	  4},	/* Terminal Country Code         */
	{ CUP_FIX_ALPHA,	  6},	/* Terminal Trans  Date          */
	{ CUP_FIX_ALPHA,	 12},	/* Cryptogram Amount             */
	{ CUP_FIX_ALPHA,	  4},	/* Cryptogram Currency Code      */
	{ CUP_FIX_ALPHA,	 12},	/* Cryptogram Cash Back          */
	{ CUP_FIX_HEX,	 	 16},	/* Cardholder Card Scheme Crypto.*/

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
	{ CUP_FIX_BIN,	  8},	/* Message Authentication Code   */
};

static tag_info_t  tab_CupTagInfo[] =
{
	{ "7100",CUP_TAG_VAR_HEX , 128},/* Issuer Script Template 1              */
	{ "7200",CUP_TAG_VAR_HEX , 128},/* Issuer Script Template 2              */
	{ "9100",CUP_TAG_VAR_HEX , 16},/* Issuer Authentication Data            */
	{ "8600",CUP_TAG_VAR_HEX , 0},/* Issuer Script command                 */
	{ "8200",CUP_TAG_VAR_HEX , 2},/* Application Interchange Profile       */
	{ "8400",CUP_TAG_VAR_HEX , 16},/* Dedicated File Name                   */
	{ "4F00",CUP_TAG_VAR_HEX , 0},/* ICC Application Id                    */
	{ "8A00",CUP_TAG_VAR_HEX , 0},/* Authorization Response Code           */
	{ "9500",CUP_TAG_VAR_HEX , 5},/* Terminal Verification Results         */
	{ "9A00",CUP_TAG_VAR_HEX , 6},/* Transaction Date                      */
	{ "9C00",CUP_TAG_VAR_HEX , 2},/* Transaction TYPE                      */
	{ "5F2A",CUP_TAG_VAR_HEX , 3},/* Transaction Currency Code             */
	{ "5F34",CUP_TAG_VAR_HEX , 0},/* PAN Sequence Numnber                  */
	{ "9F02",CUP_TAG_VAR_HEX , 12},/* Transaction Amount                    */
	{ "9F03",CUP_TAG_VAR_HEX , 12},/* Other Amount                          */
	{ "9F09",CUP_TAG_VAR_HEX , 2},/* Terminal Application Version Number   */
	{ "9F10",CUP_TAG_VAR_HEX , 32},/*  Issuer Application Data              */
	{ "9F1A",CUP_TAG_VAR_HEX , 3},/* Terminal Country Code                 */
	{ "9F1E",CUP_TAG_VAR_ALPHA , 8},/* Interface Device Serial Number        */
	{ "9F26",CUP_TAG_VAR_HEX , 8},/* Application cryptogram                */
	{ "9F27",CUP_TAG_VAR_HEX , 1},/* Cryptogram Information Data           */
	{ "9F33",CUP_TAG_VAR_HEX , 3},/* Terminal Capabilities                 */
	{ "9F34",CUP_TAG_VAR_HEX , 3},/* Cardholder Verification Method        */
	{ "9F35",CUP_TAG_VAR_HEX , 2},/* Terminal Type                         */
	{ "9F36",CUP_TAG_VAR_HEX , 2},/* Application Transaction Counter       */
	{ "9F37",CUP_TAG_VAR_HEX , 4},/* Unpredictable Number                  */
	{ "9F41",CUP_TAG_VAR_HEX , 4},/* Transaction Sequence Number           */
	{ "9F53",CUP_TAG_VAR_HEX , 0},/* Transaction Category Code             */
	{ "9F54",CUP_TAG_VAR_HEX , 0},/* Application cryptogram  ARPC          */
	{ "5A00",CUP_TAG_VAR_ALPHA , 0},/* Application PAN                     */
	{ "9F06",CUP_TAG_VAR_HEX , 0},  /* Application Identifier              */
	{ "9F12",CUP_TAG_VAR_ALPHA , 0},/* Application Prefered Name           */
	{ "5700",CUP_TAG_VAR_HEX , 0}, /* Application Track2                   */
	{ "8C00",CUP_TAG_VAR_ALPHA , 0},/* Security Attributes                 */
	{ "5000",CUP_TAG_VAR_ALPHA , 0}, /*  Application Label                 */
	{ "DF31",CUP_TAG_VAR_HEX , 21}, /*  Issuer Script Results             */
	{ "9F74",CUP_TAG_VAR_HEX , 6}, /*  Issuer Authorization Code - Electronic Cash             */
	{ "9F63",CUP_TAG_VAR_HEX , 16}, /*  Card  Product  Identification Information             */

};

/******************************************************************************/
static const TlvInfoProperties	CupSecRiskAssessDataProp = {
2,3,TLV_LT_ASCII,TLV_LT_ASCII,CUP_F125_NB_FIELDS,
{
{CUP_F125_CA_COMP_ACC_IND, FT_STRUCT_TLV,FF_ALPHA_ANS,AF_ASCII,FL_LL,LU_BYTE,60,PP_PRINT,"NULL","COMPROMISED_ACCOUNT_INDICATOR","NULL",NOT_REPEATABLE}, 
{CUP_F125_QR_PAYMENT, FT_STRUCT_TLV,FF_BIN,AF_ASCII,FL_LLL,LU_BYTE,120,PP_PRINT,"NULL","QRC_BASED_PAYMENT","NULL",NOT_REPEATABLE},  
{CUP_F125_CV_CUST_VERIF, FT_STRUCT_TLV,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,8,PP_PRINT,"NULL","CONSUMER_VERIFICATION_INFORMATION","NULL",NOT_REPEATABLE},
{CUP_F125_IP_INNOV_PAY, FT_STRUCT_TLV,FF_ALPHA_ANS,AF_ASCII,FL_LLL,LU_BYTE,138,PP_PRINT,"NULL","INNOVATIVE_PAYMENT","NULL",NOT_REPEATABLE},
{CUP_F125_LR_LOW_RISK_ID, FT_STRUCT_TLV,FF_ALPHA_ANS,AF_ASCII,FL_LL,LU_BYTE,23,PP_PRINT,"NULL","LOW_RISK_IDENTIFIER","NULL",NOT_REPEATABLE},
{CUP_F125_TF_AUTH_TRS_ID, FT_STRUCT_TLV,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,43,PP_PRINT,"NULL","AUTHENTICATION_TRANSACTION_IDENTIFIER","NULL",NOT_REPEATABLE},
{CUP_F125_QP_UPI_APP_IND, FT_STRUCT_TLV,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,5,PP_PRINT,"NULL","UNIONPAY_APPLICATION_INDICATOR","NULL",NOT_REPEATABLE},
{CUP_F125_TP_TRANS_PROD_INF, FT_STRUCT_TLV,FF_ALPHA_ANS,AF_ASCII,FL_FIX,LU_BYTE,6,PP_PRINT,"NULL","TRANSACTION PRODUCT INFORMATION","NULL",NOT_REPEATABLE},

},
"CupSecRiskAssessDataProp"
};
/******************************************************************************/
void  InitCupIcTag  (TSTagCup * msgInfo)
{
	int i;

	for (i = 0; i < MAX_CUP_CHIP_TAG; i++)
	{
		msgInfo->nPosTag [i] = 0;
		msgInfo->nPresent[i] = NOT_PRESENT;
	}
	msgInfo->nLength = 0;
	memset(msgInfo->sChipData,   '\0', sizeof(msgInfo->sChipData));
	return;
}

void  InitCupInfo(TSCupInfo *msgInfo)
{
	int i;

	for (i=0; i < MAX_CUP_FIELDS+1; i++)
		msgInfo->nFieldPos[i] = 0;
	for (i=0; i < CUP_BITMAP_LEN; i++)
		msgInfo->sBitMap[i] = 0x00;
	msgInfo->nLength = 0;
	msgInfo->nMsgType = 0;

	memset(msgInfo->sData,   '\0', sizeof(msgInfo->sData));
	/*memset(msgInfo->msgUUID,   '\0', sizeof(msgInfo->msgUUID));*/
	memset(( char *) & (msgInfo->sHeader), '\0', sizeof(TSCupHeader));
	msgInfo->msgId = 0;/*PLUTONL-3762*/
	return;
}

int GetCupFieldType(int field_no)
{
	return(tab_CupFldInfo[field_no].type);
}

int GetCupFieldLength(int field_no)
{
	return(tab_CupFldInfo[field_no].length);
}

int GetCupTagIndice ( char * szTagName)
{
	int nIndice, nSize;

	nSize = sizeof(tab_CupTagInfo) /sizeof(tag_info_t);
	nIndice = 0;

	do
	{
		if (memcmp(szTagName, tab_CupTagInfo[nIndice].sTagName, 4) == 0)
		{
			return(nIndice);
		}
		nIndice++;
	}while (nIndice < nSize);
	return(-1);
}

int GetCupTagType ( int nIndice )
{
	return ( tab_CupTagInfo[nIndice].nType);
}

int GetCupTagLength ( int nIndice )
{
	return ( tab_CupTagInfo[nIndice].nLength);
}
/******************************************************************************/
void InitSecRiskAssess(TlvInfo* pkTlvInfo)		
{
	pkTlvInfo->pkProperties = &CupSecRiskAssessDataProp;
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
