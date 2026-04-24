#include <stdio.h>
#include <stdlib.h>
#include <visa_puf.h>
#include <define.h>

fld_info_t tab_VisaPufFldInfo[] =	/* Fields from PRIV_USE_FIELDS of SMS*/
{
   {VISA_PUF_FIX_ALPHA,	  25},	/* CUSTOMUER NAME 			 */
   {VISA_PUF_FIX_ALPHA,	  57},	/* CUSTOMUER ADDRESS	 */
   {VISA_PUF_FIX_ALPHA,	  57},	/* BILLER ADDRESS   	 */
   {VISA_PUF_FIX_ALPHA,	  18},	/* BILLER PHONE NUMBER*/
   {VISA_PUF_FIX_ALPHA,	  8},	   /* VPUF_MER_ID */
   {VISA_PUF_FIX_HEX  ,	  34},	/* CARDHOLDER VSEC            */
   {VISA_PUF_FIX_HEX  ,	  34},	/* MERCHANT VSEC            */
   {VISA_PUF_FIX_BCD  ,	  40},	/* TRX ID   VSEC            */
   {VISA_PUF_FIX_HEX  ,	  40},	/* TRANS ST VSEC            */ /*PLUTONL-3114 */
   {VISA_PUF_FIX_ALPHA,	  6},	/* CVV2               */
   {VISA_PUF_FIX_ALPHA,	  2},	/* IGOTS                */
   {VISA_PUF_FIX_ALPHA,   24},/*SERVICE INDICATORS, NOT USED*/
   {VISA_PUF_FIX_ALPHA,   1}, /*RECURRING*/

   {VISA_PUF_FIX_ALPHA,   2},/*SERVICE INDICATORS, NOT USED*/
   {VISA_PUF_FIX_ALPHA,   2},/*VPUF_POS_ENV*/
   {VISA_PUF_FIX_ALPHA,   2}, /*VPUF_PAY_GAR_OPT*/
   {VISA_PUF_FIX_ALPHA,   2},/*VPUF_UCAF_COL*/
   {VISA_PUF_FIX_ALPHA,   2}, /*VPUF_UCAF_FIELD*/
   {VISA_PUF_FIX_ALPHA,   1}, /* DCC ICH See PROD00039494*/

   /* End SLA 080212 Recurring Payment */
};

void  InitVisaPufInfo(TSVisaPufInfo *pufInfo)
{
   int i;

   for (i=0; i < MAX_VISA_PUF_FIELDS+1; i++)
      pufInfo->nFieldPos[i] = 0;
   for (i=0; i < VISA_PUF_BITMAP_LEN; i++)
      pufInfo->sBitMap[i] = 0x00;
   pufInfo->nLength = 0;
   memset(pufInfo->sData, '\0', MAX_VISA_PUF_DATA_LEN);
}

int GetVisaPufFieldType(int field_no)
{
   return(tab_VisaPufFldInfo[field_no].type);
}

int GetVisaPufFieldLength(int field_no)
{
   return(tab_VisaPufFldInfo[field_no].length);
}


