/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      sms_puf_ini.c                                                         */
/*                                                                             */
/* Description                                                                 */
/*      SMS PUF Init Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sms_puf.h>
#include <define.h>

fld_info_t tab_SmsPufFldInfo[] =	/* Fields from PRIV_USE_FIELDS of SMS*/
{
   {SMS_PUF_FIX_ALPHA,	  25},	/* CUSTOMUER NAME 			 */
   {SMS_PUF_FIX_ALPHA,	  57},	/* CUSTOMUER ADDRESS	 */
   {SMS_PUF_FIX_ALPHA,	  57},	/* BILLER ADDRESS   	 */
   {SMS_PUF_FIX_ALPHA,	  18},	/* BILLER PHONE NUMBER*/
   {SMS_PUF_FIX_ALPHA,	  8},	   /* VPUF_MER_ID */
   {SMS_PUF_FIX_HEX  ,	  34},	/* CARDHOLDER VSEC            */
   {SMS_PUF_FIX_HEX  ,	  34},	/* MERCHANT VSEC            */
   {SMS_PUF_FIX_BCD  ,	  40},	/* TRX ID   VSEC            */
   {SMS_PUF_FIX_BCD  ,	  40},	/* TRANS ST VSEC            */
   {SMS_PUF_FIX_ALPHA,	  6},	/* CVV2               */
   {SMS_PUF_FIX_ALPHA,	  2},	/* IGOTS                */
   {SMS_PUF_FIX_ALPHA,   24},   /*SERVICE INDICATORS, NOT USED*/
   {SMS_PUF_FIX_ALPHA,    1},   /* RECURRING       */
};

void  InitSmsPufInfo(TSSmsPufInfo *pufInfo)
{
   int i;

   for (i=0; i < MAX_SMS_PUF_FIELDS+1; i++)
      pufInfo->nFieldPos[i] = 0;
   for (i=0; i < SMS_PUF_BITMAP_LEN; i++)
      pufInfo->sBitMap[i] = 0x00;
   pufInfo->nLength = 0;
   memset(pufInfo->sData, '\0', MAX_SMS_PUF_DATA_LEN);
}

int GetSmsPufFieldType(int field_no)
{
   return(tab_SmsPufFldInfo[field_no].type);
}

int GetSmsPufFieldLength(int field_no)
{
   return(tab_SmsPufFldInfo[field_no].length);
}

