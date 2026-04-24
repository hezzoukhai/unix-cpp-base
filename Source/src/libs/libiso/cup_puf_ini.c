#include <stdio.h>
#include <stdlib.h>
#include <cup_puf.h>
#include <define.h>

static fld_info_t tab_CupPufFldInfo[] =	/* Fields from PRIV_USE_FIELDS of SMS*/
{
	{CUP_PUF_FIX_ALPHA,	  25},	/* CUSTOMUER NAME 			 */
	{CUP_PUF_FIX_ALPHA,	  57},	/* CUSTOMUER ADDRESS	 */
	{CUP_PUF_FIX_ALPHA,	  57},	/* BILLER ADDRESS   	 */
	{CUP_PUF_FIX_ALPHA,	  18},	/* BILLER PHONE NUMBER*/
	{CUP_PUF_FIX_ALPHA,	  6},	/* PROCESS BY DATE */
	{CUP_PUF_FIX_HEX  ,	  34},	/* CARDHOLDER VSEC            */
	{CUP_PUF_FIX_HEX  ,	  34},	/* MERCHANT VSEC            */
	{CUP_PUF_FIX_BCD  ,	  40},	/* TRX ID   VSEC            */
	{CUP_PUF_FIX_BCD  ,	  40},	/* TRANS ST VSEC            */
	{CUP_PUF_FIX_ALPHA,	  6},	/* CVV2               */
	{CUP_PUF_FIX_ALPHA,	  2},	/* IGOTS                */
};

void  InitCupPufInfo(TSCupPufInfo *pufInfo)
{
	int i;

	for (i=0; i < MAX_CUP_PUF_FIELDS+1; i++)
		pufInfo->nFieldPos[i] = 0;
	for (i=0; i < CUP_PUF_BITMAP_LEN; i++)
		pufInfo->sBitMap[i] = 0x00;
	pufInfo->nLength = 0;
	memset(pufInfo->sData, '\0', sizeof(pufInfo->sData));
	return;
}

int GetCupPufFieldType(int field_no)
{
	return(tab_CupPufFldInfo[field_no].type);
}

int GetCupPufFieldLength(int field_no)
{
	return(tab_CupPufFldInfo[field_no].length);
}

