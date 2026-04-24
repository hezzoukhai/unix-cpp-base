#include <stdio.h>
#include <stdlib.h>
#include <iso_bic.h>
#include <define.h>


static fld_info_t    tab_BicFldInfo[] =
{
	{ BIC_FIX_ALPHA,  	  		16},	/* BIT_MAP2			 */
	{ BIC_LL_ALPHA,	 		19},   /* CARD_NBR 			 */
	{ BIC_FIX_ALPHA,	  	6},	/* PROC_CODE			 */
	{ BIC_FIX_ALPHA,	 	12},	/* TRANS_AMOUNT			 */
	{ BIC_FIX_ALPHA,	 	12},	/* STLMNT_AMOUNT		 */
	{ BIC_FIX_ALPHA,	 	12},	/* Cardholder Billing Amount	 */
	{ BIC_FIX_ALPHA,	 	10},	/* XMIT_TIME			 */
	{ NOT_USED,  	  	8},	/* Cardholder Billing Fee	 */
	{ NOT_USED,  	  	8},	/* CONVRSN_RATE_STLMNT		 */
	{ NOT_USED,  	  	8},	/* CONVRSN_RATE_CARD_HLDR_BILLING*/

	{ BIC_FIX_ALPHA,	  	6},	/* AUDIT_NBR			 */
	{ BIC_FIX_ALPHA,	  	6},	/* TRANS_DATE_TIME               */
	{ BIC_FIX_ALPHA,	  	4},	/* TRANS_DATE			 */
	{ BIC_FIX_ALPHA,	  	4},	/* EXPIRY_DATE			 */
	{ BIC_FIX_ALPHA,	  4},	/* STLMNT_DATE			 */
	{ NOT_USED,  	  4},	/* CNVRSN_DATE			 */
	{ BIC_FIX_ALPHA,	  4},	/* CAPTURE_DATE			 */
	{ BIC_FIX_ALPHA,	  4},	/* MERCHANT_TYPE		 */
	{ NOT_USED,   	  3},	/* ACQR_COUNTRY			 */
	{ NOT_USED,  	  3},	/* PAN_COUNTRY			 */

	{ NOT_USED,  	  3},	/* FORWD_COUNTRY		 */
	{ BIC_FIX_ALPHA,       3},	/* POS_ENTRY_MODE		 */
	{ BIC_FIX_ALPHA,	  3},	/* Card Sequence Number		 */
	{ NOT_USED,  	  3},	/* Network International Identifier */
	{ BIC_FIX_ALPHA,	  2},	/* POS_CONDITION		 */
	{ BIC_FIX_ALPHA,  	  2},	/* POS PIN Capture Code         */
	{ BIC_FIX_ALPHA,	  1},	/* Authorization Id Response Lenght */
	{ BIC_FIX_ALPHA,	  9},	/* TRANS_FEE                     */
	{ NOT_USED,   	  9},	/* Amount Settlement Fee         */
	{ NOT_USED,            9},	/* Amount Transaction Processing Fee */

	{ NOT_USED,  	  9},	/* Amount Setlmnt Processing Fee */
	{ BIC_LL_ALPHA,	 11},	/* ACQR_ID			 */
	{ BIC_LL_ALPHA,	 11},	/* FORWD_ID			 */
	{ NOT_USED,  	 28},	/* PAN_EXTENDED			 */
	{ BIC_LL_ALPHA,	 37},	/* T2_DATA			 */
	{ BIC_LLL_ALPHA,	104},	/* T3_DATA                     */
	{ BIC_FIX_ALPHA,	 12},	/* REFERENCE_NBR		 */
	{ BIC_FIX_ALPHA,	  6},	/* AUTHOR_ID_RESPONSE		 */
	{ BIC_FIX_ALPHA,	  2},	/* RESPONSE CODE  		 */
	{ NOT_USED,  	  3},	/* Service Restriction Code	 */

	{ BIC_FIX_ALPHA,	 16},	/* TERMINAL_NBR			 */
	{ BIC_FIX_ALPHA,	 15},	/* OUTLET_NBR			 */
	{ BIC_FIX_ALPHA,	 40},	/* TERMINAL_ADR			 */
	{ BIC_FIX_ALPHA,	 27},	/* ADTNL_RESP_DATA               */
	{ BIC_LL_ALPHA,	 76},	/* T1_DATA			 */
	{ NOT_USED,  	999},	/* Additrional Data Bic          */
	{ NOT_USED,  	999},	/* Additrional Data National     */
	{ BIC_LLL_ALPHA,	30},	/* ADTNL_DATA_PRIV              */
	{ BIC_FIX_ALPHA,	  3},	/* TRANS_CRNCY_CODE		 */
	{ BIC_FIX_ALPHA,	  3},	/* STLMNT_CRNCY_CODE		 */

	{ NOT_USED,  	  3},	/* BILLING_CRNCY_CODE		 */
	/* { BIC_FIX_HEX,	 16},	* PIN_DATA			 */
	{ BIC_FIX_ALPHA,	 16},	/* PIN_DATA			 */
	{ BIC_FIX_ALPHA,	 16},	/* SECURITY_DATA		 */
	{ BIC_FIX_ALPHA,	 15},	/* ADTNL_AMOUNTS		 */
	{ BIC_LLL_BIN,	258},	/* ICC DATA                      *//**RHA ICC DATA MNG**/
	{ NOT_USED,	999},	/* NW Reserved                         */
	{ NOT_USED,  999},	/* Reserved National                    */
	{ NOT_USED,	999},	/* Reserved National                   */
	{ NOT_USED,  999},	/* Reserved National                   */
	/*{ BIC_FIX_ALPHA,	 15},   Terminal Data                */
	{ BIC_LLL_ALPHA,      19},  /* Terminal Data                */

	{ BIC_LLL_ALPHA,     22},     /* Card ISS Catego Resp Code Data  */       
	/*{ BIC_LL_ALPHA,     18},	 Card ISS Catego Resp Code Data  */
	{ BIC_FIX_ALPHA, 	13},	/* Postal Code                     */
	{ BIC_LLL_ALPHA,    600},	/* POS DATA, PIN Offset            */
	{ BIC_FIX_ALPHA,     16},	/* Primary MAC1                    */
	{ NOT_USED,	         0},	/* Extended BitMap                 */
	{ BIC_FIX_ALPHA,	  1},	/* STLMNT_CODE			 */
	{ NOT_USED,  	  2},	/* Extended Payment Code	 */
	{ NOT_USED,  	  3},	/* RECEIVER_COUNTRY		 */
	{ NOT_USED,   	  3},	/* STLMNT_INST_COUNTRY		 */
	{ BIC_FIX_ALPHA,	  3},	/* NW_MNGMT_INFO		 */

	{ NOT_USED,  	  4},	/* Message Number                                                   --Modif Rabie --  */
	{ NOT_USED,            4},	/* Message Number Last                                              --Modif Rabie -- */
	{ NOT_USED,  	  6},	/* ACTION_DATE			 */
	{ BIC_FIX_ALPHA,	 10},	/* CR_NBR			 */
	{ BIC_FIX_ALPHA,	 10},	/* CR_REV_NBR			 */
	{ BIC_FIX_ALPHA,	 10},	/* DB_NBR			 */
	{ BIC_FIX_ALPHA,	 10},	/* DB_REV_NBR			 */
	{ BIC_FIX_ALPHA,	 10},	/* Transfer Number		 */
	{ BIC_FIX_ALPHA,	 10},	/* Transfer Reversal Number	 */
	{ BIC_FIX_ALPHA,	 10},	/* Inquiries Number		 */

	{ BIC_FIX_ALPHA,	 10},	/* Authorizations Number	 */
	{ NOT_USED,  	 12},	/* Credit, Processing Fee Amount                                    --Modif Rabie --  */
	{ NOT_USED,  	 12},	/* Transaction Fee Amount Credit     */
	{ NOT_USED,  	 12},	/* Processing Fee Amount Debits       */
	{ NOT_USED,  	 12},	/* Debit, Transaction Fee Amount      */
	{ BIC_FIX_ALPHA,	 16},	/* CR_AMOUNT			 */
	{ BIC_FIX_ALPHA,	 16},	/* CR_REV_AMOUNT		 */
	{ BIC_FIX_ALPHA,	 16},	/* DB_AMOUNT			 */
	{ BIC_FIX_ALPHA,	 16},	/* DB_REV_AMOUNT		 */
	{ BIC_FIX_ALPHA,	 42},	/* ORIG_DATA			 */

	{ NOT_USED,	          1},	/* FILE_UPDATE_CODE              */
	{ NOT_USED,	          2},	/* FILE_SECURITY_CODE            */
	{ NOT_USED,	          5},	/* RESPONSE_INDICATOR            */
	{ NOT_USED,   	  7},	/* SERVICE_INDICATOR                 */
	{ BIC_FIX_ALPHA,	 42},	/* REPLACE_AMOUNTS		 */
	{ NOT_USED,  	 16},	/* Message Security Code             */
	{ BIC_FIX_ALPHA,	 17},	/* NET_STLMENT_AMOUNT			 */
	{ NOT_USED,  	 25},	/* Payee                             */
	{ BIC_FIX_ALPHA,	 11},	/* STLMNT_INSTITN_IDENTIF_CODE   */
	{ BIC_LL_ALPHA,	 11},	/* RECEIVER_ID			 */

	{ NOT_USED,  	 4},	/* FILE_NAME                         */
	{ BIC_LL_ALPHA,	 28},	/* ACCOUNT_ID1			 */
	{ BIC_LL_ALPHA,	 28},	/* ACCOUNT_ID2			 */
	{ NOT_USED,  	 999},	/* Transaction Description           */
	{ NOT_USED,  	 999},	/* Reserved Bic                      */
	{ NOT_USED,  	 999},	/* Reserved Bic                      */
	{ NOT_USED,  	 999},	/* Reserved Bic                      */
	{ NOT_USED,  	 999},	/* Reserved Bic                      */
	{ NOT_USED,  	 999},	/* Reserved Bic                      */
	{ NOT_USED,  	 999},	/* Reserved Bic                      */

	{ NOT_USED,  	 999},	/* Reserved Bic                      */
	{ NOT_USED,           999},	/* Reserved National             */
	{ NOT_USED, 		  999},	/* National Reserved                 */
	{ NOT_USED,            999}, /* National Reserved             */
	{ NOT_USED,            999}, /* National Reserved             */
	{ NOT_USED,            999}, /* National Reserved             */
	{ NOT_USED,            999}, /* National Reserved             */
	{ NOT_USED,            999}, /* National Reserved             */
	{ NOT_USED,            999}, /* National Reserved chapm 119     */
	{ BIC_LLL_ALPHA,       36},  /* Terminal Adress-Branch-Region */

	{ BIC_LLL_ALPHA,	  23},	/* Authorization Indicators   */
	{ BIC_FIX_ALPHA,	  14},	/* Card_Iss_Ident_Code       */
	{ BIC_LLL_ALPHA,	 553},	/* Cryptographic Service Message  */
	{ BIC_LLL_ALPHA,	  12},	/* Setlment Record 2     */
	{ BIC_LLL_ALPHA,       375},   /* Statment Print Data   */
	{ BIC_LLL_BIN,	  800},	/* ATM Additional Data         */
	{ BIC_LLL_ALPHA,	  200},	/* POS User Data               */
	{ BIC_FIX_ALPHA,       16},  /* Secondary Message Authentication Code    */

};


void  InitBicInfo(TSBicInfo *msgInfo)
{
	int i;

	for (i=0; i < MAX_BIC_FIELDS+1; i++)
		msgInfo->nFieldPos[i] = 0;
	for (i=0; i < BIC_BITMAP_LEN; i++)
		msgInfo->sBitMap[i] = 0x00;
	msgInfo->nLength = 0;
	msgInfo->nMsgType = 0;
	memset(msgInfo->sData,   0, sizeof(msgInfo->sData));
}

int GetBicFieldType(int field_no)
{
	return(tab_BicFldInfo[field_no].type);
}

int GetBicFieldLength(int field_no)
{
	return(tab_BicFldInfo[field_no].length);
}

/**Start RHA310309 Adding ICC DATA MANGMENT**/
static tag_info_t  tab_BicTagInfo[] = 
{ 
	{ "7100",  BIC_TAG_VAR_HEX,   126},/* Issuer Script Template 1            */
	{ "7200",  BIC_TAG_VAR_HEX,   126},/* Issuer Script Template 2            */
	{ "8200",  BIC_TAG_VAR_HEX,   2},/* Application Interchange Profile       */
	{ "8400",  BIC_TAG_VAR_HEX,   16},/* Dedicated File Name                   */
	{ "9100",  BIC_TAG_VAR_HEX,   16},/* Issuer Authentication Data            */
	{ "9500",  BIC_TAG_VAR_HEX,   5},/* Terminal Verification Results         */
	{ "9A00",  BIC_TAG_VAR_HEX,   3},/* Transaction Date                      */
	{ "9C00",  BIC_TAG_VAR_HEX,   2},/* Transaction TYPE                      */
	{ "5F2A",  BIC_TAG_VAR_HEX,   2},/* Transaction Currency Code             */
	{ "9F02",  BIC_TAG_VAR_HEX,   6},/* Transaction Amount                    */
	{ "9F03",  BIC_TAG_VAR_HEX,  6},/* Other Amount                          */
	{ "9F09",  BIC_TAG_VAR_HEX,   2},/* Terminal Application Version Number   */
	{ "9F10",  BIC_TAG_VAR_HEX,   32},/*  Issuer Application Data             */
	{ "9F11",  BIC_TAG_VAR_BIN,   2},/*  Issuer Application Data Len             */
	{ "9F1A",  BIC_TAG_VAR_HEX,   2},/* Terminal Country Code                 */
	{ "9F1E",  BIC_TAG_VAR_ALPHA, 8},/* Interface Device Serial Number        */
	{ "9F26",  BIC_TAG_VAR_HEX,   8},/* Application cryptogram                */
	{ "9F27",  BIC_TAG_VAR_HEX,   1},/* Cryptogram Information Data           */
	{ "9F33",  BIC_TAG_VAR_HEX,   3},/* Terminal Capabilities                 */
	/**/
	{ "8A00",  BIC_TAG_VAR_ALPHA, 2},/* Authorization Response Code           */
	{ "9F34",  BIC_TAG_VAR_HEX,   3},/* Cardholder Verification Method        */
	{ "9F35",  BIC_TAG_VAR_HEX,   2},/* Terminal Type                         */
	{ "9F36",  BIC_TAG_VAR_HEX,   2},/* Application Transaction Counter       */
	{ "9F37",  BIC_TAG_VAR_HEX,   4},/* Unpredictable Number                  */
	{ "9F39",  BIC_TAG_VAR_HEX,   2},/* POS ENTRY MODE                  */
	/**/
	{ "9F41",  BIC_TAG_VAR_HEX,   0},/* Transaction Sequence Number           */
	{ "9F53",  BIC_TAG_VAR_HEX,   1},/* Transaction Category Code             */


};


void  InitBicIcTag  (TSTagBic * msgInfo)
{
	int i;

	for (i = 0; i < MAX_BIC_CHIP_TAG; i++)
	{
		msgInfo->nPosTag [i] = 0;
		msgInfo->nPresent[i] = NOT_PRESENT;
	}
	msgInfo->nLength = 0;
	memset(msgInfo->sChipData,   '\0', MAX_BIC_CHIP_LEN);
}

int GetBicTagIndice ( char * szTagName)
{
	int nIndice, nSize;

	nSize = sizeof(tab_BicTagInfo) /sizeof(tag_info_t);
	nIndice = 0;

	do
	{
		if (memcmp(szTagName, tab_BicTagInfo[nIndice].sTagName, 4) == 0)
			return(nIndice);
		nIndice++;
	}while (nIndice < nSize);
	return(-1);
}

int GetBicTagType ( int nIndice )
{
	return ( tab_BicTagInfo[nIndice].nType);
}

int GetBicTagLength ( int nIndice )
{
	return ( tab_BicTagInfo[nIndice].nLength);
}
