#include <stdio.h>
#include <stdlib.h>
#include <iso_puf.h>
#include <define.h>

#include <visa_define.h>
#include <sms_define.h>
#include <p7_common.h>
#include <p7_globals.h>
#include <iso8583.h>

static fld_info_t tab_PufFldInfo[] =	/* Fields from PRIV_USE_FIELDS of SMS*/
{
   {PUF_FIX_BCD,	  4},	/* NETWORK_ID			 */
   {PUF_FIX_BCD,	  4},   /* NOT APPLICABLE                */
   {PUF_FIX_BCD,	  4},	/* MSG_REASON			 */
   {PUF_FIX_BCD,	  4},	/* STIP_OR_SWITCH_REASON	 */
   {PUF_FIX_BCD,	  6},	/* PLUS_PMC			 */
   {PUF_FIX_ALPHA,	  7},	/* Chargeback Reduction/BASEII flags */
   {PUF_FIX_BIN,	  8},	/* NETWORK_FLAGS		 */
   {PUF_FIX_BCD,	  8},	/* Visa Acqr Business Id US only */
   {PUF_FIX_ALPHA,	 14},	/* Fraud Data	 		 */
   {PUF_FIX_ALPHA,	 13},	/* Gateway Merchant Data	 */

   {PUF_FIX_ALPHA,	  1},	/* Reimbursment attribute	 */
   {PUF_FIX_ALPHA,	 30},	/* Sharing Group Code		 */
   {PUF_FIX_BCD,	  6},	/* Decimal Positions Indicator   */
   {PUF_FIX_ALPHA,	 36},	/* Issuer Currency conversion Data */
   {PUF_FIX_ALPHA,	  9},   /* Settlement Amount,              */
   {PUF_FIX_BIN,	  6},   /* ImagExpress Workstation         */
   {PUF_FIX_BIN,	  1},   /* Additional Data Indicator       */
   {PUF_FIX_BIN,	  1},   /* Volume-Tier Request Indicator   */
   {PUF_FIX_ALPHA,	  3},   /* FEE Program Indicator           */ /*ADD SWI200904 VISA*/
   {NOT_USED,	  0},   /* Future Use                      */
   {NOT_USED,	  0},   /* Future Use                      */

   {NOT_USED,	  0},   /* Future Use                      */
   {NOT_USED,	  0},   /* Future Use                      */
   {NOT_USED,	  0},   /* Future Use                      */
   {NOT_USED,	  0},   /* Future Use                      */
};




static const BitMappedProperties	VisaF063DataProp = {
VISA_F063_NB_FIELDS,1,1,3,FF_BIN,
{
{B1_F063_NETWORK_ID, FT_SINGLE,FF_BCD,AF_EBCDIC,FL_FIX,LU_NIBBLES,4,PP_PRINT,"NULL","NETWORK_ID","NULL"},
{B1_F063_PREAUTH_TIME_LIMIT, FT_SINGLE,FF_BCD,AF_EBCDIC,FL_FIX,LU_NIBBLES,4,PP_PRINT,"NULL","PREAUTH_TIME_LIMIT","NULL"},
{B1_F063_MSG_RSN_CODE, FT_SINGLE,FF_BCD,AF_EBCDIC,FL_FIX,LU_NIBBLES,4,PP_PRINT,"NULL","MSG_RSN_CODE","NULL"},
{B1_F063_STIP_SWITCH_RSN_CD, FT_SINGLE,FF_BCD,AF_EBCDIC,FL_FIX,LU_NIBBLES,4,PP_PRINT,"NULL","STIP_SWITCH_RSN_CD","NULL"},
{B1_F063_FEE_PROGRAM_IND, FT_SINGLE,FF_ALPHA_AN,AF_EBCDIC,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","FEE_PROGRAM_IND","NULL"},
},
"VisaF063DataProp"
};	

void  InitPufInfo(TSPufInfo *pufInfo)
{
   int i;

   for (i=0; i < MAX_PUF_FIELDS+1; i++)
      pufInfo->nFieldPos[i] = 0;
   for (i=0; i < PUF_BITMAP_LEN; i++)
      pufInfo->sBitMap[i] = 0x00;
   pufInfo->nLength = 0;
   memset(pufInfo->sData, '\0', MAX_PUF_DATA_LEN);
}

int GetPufFieldType(int field_no)
{
   return(tab_PufFldInfo[field_no].type);
}

int GetPufFieldLength(int field_no)
{
   return(tab_PufFldInfo[field_no].length);
}

/******************************************************************************/
void InitVisaVIPPufDataTlv(BitMappedInfo* pkVIPPufInfo)
{
	pkVIPPufInfo->pkProperties = &VisaF063DataProp;
	ResetBitMappedInfo(pkVIPPufInfo);
}
/******************************************************************************/
static const BitMappedProperties	SmsF063DataProp = {
SMS_F063_NB_FIELDS,1,1,3,FF_BIN,
{
{SMS_F063_NETWORK_ID, FT_SINGLE,FF_BCD,AF_EBCDIC,FL_FIX,LU_NIBBLES,4,PP_PRINT,"NULL","NETWORK_ID","NULL"},
{SMS_F063_PREAUTH_TIME_LIMIT, FT_SINGLE,FF_BCD,AF_EBCDIC,FL_FIX,LU_NIBBLES,4,PP_PRINT,"NULL","PREAUTH_TIME_LIMIT","NULL"},
{SMS_F063_MSG_RSN_CODE, FT_SINGLE,FF_BCD,AF_EBCDIC,FL_FIX,LU_NIBBLES,4,PP_PRINT,"NULL","MSG_RSN_CODE","NULL"},
{SMS_F063_STIP_SWITCH_RSN_CD, FT_SINGLE,FF_BCD,AF_EBCDIC,FL_FIX,LU_NIBBLES,4,PP_PRINT,"NULL","STIP_SWITCH_RSN_CD","NULL"},
{SMS_F063_CHARGEBACK_REDUCTION, FT_SINGLE,FF_ALPHA_ANS,AF_EBCDIC,FL_FIX,LU_NIBBLES,14,PP_PRINT,"NULL","CHARGEBACK_REDUCTION","NULL"},
{SMS_F063_ACQ_BUSINESS_ID, FT_SINGLE,FF_BCD,AF_EBCDIC,FL_FIX,LU_NIBBLES,8,PP_PRINT,"NULL","ACQ_BUSINESS_ID","NULL"},
{SMS_F063_FRAUD_DATA, FT_SINGLE,FF_ALPHA_ANS,AF_EBCDIC,FL_FIX,LU_NIBBLES,28,PP_PRINT,"NULL","FRAUD_DATA","NULL"},
{SMS_F063_REIMBURSEMENT_ATTRIBUTE, FT_SINGLE,FF_ALPHA_ANS,AF_EBCDIC,FL_FIX,LU_NIBBLES,2,PP_PRINT,"NULL","REIMBURSEMENT_ATTRIBUTE","NULL"},
{SMS_F063_SHARING_GROUP_CODE, FT_SINGLE,FF_ALPHA_ANS,AF_EBCDIC,FL_FIX,LU_NIBBLES,60,PP_PRINT,"NULL","SHARING_GROUP_CODE","NULL"},
{SMS_F063_DECIMAL_POSITIONS_INDICATOR, FT_SINGLE,FF_BCD,AF_EBCDIC,FL_FIX,LU_NIBBLES,6,PP_PRINT,"NULL","DECIMAL_POSITIONS_INDICATOR","NULL"},
{SMS_F063_ISSUER_CURRENCY_CONVERSTION_DATA, FT_SINGLE,FF_ALPHA_ANS,AF_EBCDIC,FL_FIX,LU_NIBBLES,72,PP_PRINT,"NULL","ISSUER_CURRENCY_CONVERSTION_DATA","NULL"},
{SMS_F063_FEE_PROGRAM_IND, FT_SINGLE,FF_ALPHA_AN,AF_EBCDIC,FL_FIX,LU_BYTE,3,PP_PRINT,"NULL","FEE_PROGRAM_IND","NULL"},
},
"SmsF063DataProp"
};	


void InitSmsVIPPufDataTlv(BitMappedInfo* pkVIPPufInfo)
{
	pkVIPPufInfo->pkProperties = &SmsF063DataProp;
	ResetBitMappedInfo(pkVIPPufInfo);
}

