#include <stdio.h>
#include <stdlib.h>
#include <iso_hps.h>
#include <iso_bic.h>
#include <define.h>


static fld_info_t    tab_IsoB2FldInfo[] =
{
	{ ISO_FIX_ALPHA,                   4},     /* USER-FLD1                     */
	{ ISO_FIX_ALPHA,                   2},             /* CRYPTO-INFO-DATA                      */
	{ ISO_FIX_ALPHA,                   10},         /* TVR                   */
	{ ISO_FIX_ALPHA,                   16},             /* ARQC          */
	{ ISO_FIX_ALPHA,                   12},             /* AMT-AUTH      */
	{ ISO_FIX_ALPHA,                   12},             /*AMT-OTHER      */
	{ ISO_FIX_ALPHA,                   4},             /* AIP   */
	{ ISO_FIX_ALPHA,                   4},             /* ATC   */

	{ ISO_FIX_ALPHA,                   3},             /*TERM-CNTRY-CDE*/
	{ ISO_FIX_ALPHA,                   3},             /* TRAN-CRNCY-CDE     */
	{ ISO_FIX_ALPHA,                   6},             /* TRAN-DAT                */
	{ ISO_FIX_ALPHA,                   2},             /* TRAN-TYPE             */
	{ ISO_FIX_ALPHA,                   8},             /* UNPREDICT-NUM      */
	{ NOT_USED,     	              0},             /* ISS-APPL-DATA        */			
	{ NOT_USED,     	              0},             /* ISS-APPL-DATA        */			
	/* { BIC_LLLL_ALPHA,                  64},              ISS-APPL-DATA        */			
	{ ISO_FIX_ALPHA,                  68},             /* ISS-APPL-DATA        */			

};

static fld_info_t    tab_IsoB3FldInfo[] =
{
	{ ISO_FIX_ALPHA,                   8},    /* TERM-SERL-NUM */
	{ ISO_FIX_ALPHA,                   8},    /* EMV-TERM-CAP */
	{ ISO_FIX_ALPHA,                   4},   /* USER-FLD1 */
	{ ISO_FIX_ALPHA,                   8},   /* USER-FLD2 */
	{ ISO_FIX_ALPHA,                   2},   /* EMV-TERM-TYPE */
	{ ISO_FIX_ALPHA,                   4},  /* APPL-VER-NUM */
	{ ISO_FIX_ALPHA,                   6}, /* CVM-RSLTS */
	/*  { BIC_LLLL_ALPHA,                 36},  DF-NAME */ 
	{ ISO_FIX_ALPHA,                 36},  /* DF-NAME */   /***OTA26062012*/

};




void  InitIsoTokenInfo(TSIsoInfo *msgInfo)
{
	int i;

	for (i=0; i < MAX_ISO_FIELDS+1; i++)
		msgInfo->nFieldPos[i] = 0;
	for (i=0; i < ISO_BITMAP_LEN; i++)
		msgInfo->sBitMap[i] = 0x00;
	msgInfo->nLength = 0;
	msgInfo->nMsgType = 0;
	memset(msgInfo->sData,   '\0', MAX_ISO_DATA_LEN);
}

int GetIsoTokenFieldType(int field_no, char *token)
{
	if(!memcmp(token, "B2", 2))
		return(tab_IsoB2FldInfo[field_no].type);
	else if (!memcmp(token, "B3", 2))
		return(tab_IsoB3FldInfo[field_no].type);
}

int GetIsoTokenFieldLength(int field_no, char *token)
{
	if(!memcmp(token, "B2", 2))
		return(tab_IsoB2FldInfo[field_no].length);
	else if (!memcmp(token, "B3", 2))
		return(tab_IsoB3FldInfo[field_no].length);
}

