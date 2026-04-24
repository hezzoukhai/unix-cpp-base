#include <stdio.h>
#include <stdlib.h>
#include <iso_bic.h>
#include <define.h>


fld_info_t    tab_BicIccFldInfo[] =
{
	{ BIC_TAG_FIX_ALPHA,	       	2},   	/* USER-FLD1			 */
	{ BIC_TAG_FIX_HEX,	  		1},		/* CRYPTO-INFO-DATA			 */
	{ BIC_TAG_FIX_HEX,	 	    5},		/* TVR			 */
	{ BIC_TAG_FIX_HEX,	 		8},		/* ARQC		 */
	{ BIC_TAG_FIX_HEX,	 		6},		/* AMT-AUTH	 */
	{ BIC_TAG_FIX_HEX,	 		6},		/*AMT-OTHER	 */
	{ BIC_TAG_FIX_HEX,  	  		2},		/* AIP	 */
	{ BIC_TAG_FIX_HEX,  	  		2},		/* ATC	 */
	{ BIC_TAG_FIX_HEX,  	  		2},		/*TERM-CNTRY-CDE*/

	{ BIC_TAG_FIX_HEX,	 	 	2},		/* TRAN-CRNCY-CDE     */
	{ BIC_TAG_FIX_HEX,	 	 	3},		/* TRAN-DAT                */
	{ BIC_TAG_FIX_HEX,	 	 	1},		/* TRAN-TYPE		 */
	{ BIC_TAG_FIX_HEX,	 	 	4},		/* UNPREDICT-NUM      */
	{ BIC_TAG_VAR_HEX,	 	   32}		/* ISS-APPL-DATA        */


};


void  InitBicIccInfo(TSBicIccInfo *msgInfo)
{
	int i;

	for (i=0; i < BIC_MAX_ICC_TAG+1; i++)
		msgInfo->nFieldPos[i] = 0;
	for (i=0; i < BIC_ICC_BITMAP_LEN; i++)
		msgInfo->sBitMap[i] = 0x00;
	msgInfo->nLength = 0;
	msgInfo->nMsgType = 0;
	memset(msgInfo->sData,   0, MAX_BIC_DATA_LEN);
}

int GetBicIccFieldType(int field_no)
{
	return(tab_BicIccFldInfo[field_no].type);
}

int GetBicIccFieldLength(int field_no)
{
	return(tab_BicIccFldInfo[field_no].length);
}
