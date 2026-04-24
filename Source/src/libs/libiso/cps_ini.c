#include <stdio.h>
#include <stdlib.h>
#include <iso_cps.h>
#include <define.h>

fld_info_t tab_CpsFldInfo[] =       /* Fields from CPS Field             */
{
   {CPS_FIX_ALPHA,	  1},	/* ACI		 		 */
   {CPS_FIX_BCD,	 15},   /* TRANS_ID			 */
   {CPS_FIX_ALPHA,	  4},	/* Validation Code		 */
   {CPS_FIX_ALPHA,	  1},	/* Market Specific Data		 */
   {CPS_FIX_BCD,	  2},	/* Duration			 */
   {CPS_FIX_ALPHA,	  1},	/* Prestigious Property Indicator*/
   {CPS_FIX_ALPHA,	  26},	/*Purchase Identifier				 */   /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art8.3.1*/	
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {CPS_FIX_ALPHA,	 15},	/*  MasterCard Interchange  	 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {CPS_FIX_BCD, 10},	/*ENH 11.1 MVV	BIS110413	 */ 
   {CPS_FIX_ALPHA, 4},  /* Online Risk Assessment Risk Score and Reason Codes "PROD00070511"*/ /*NAB21092020 PLUTONL-2274*/
   {CPS_FIX_ALPHA, 6},  /*	PRODUCT_ID */  /* DEF-19862 PLUTONL-5280 ELKAM10102022 */
   {CPS_FIX_ALPHA,  2},  /*	Program Identifier */
   {NOT_USED,	  0},	/*				 */
   {CPS_FIX_ALPHA,	  1},	/*	SPEND_QUALIFIED_IND *//* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.1*/
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */

   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
   {NOT_USED,	  0},	/*				 */
};

void  InitCpsInfo(TSCpsInfo *cpsInfo)
{
   int i;

   for (i=0; i < MAX_CPS_FIELDS+1; i++)
      cpsInfo->nFieldPos[i] = 0;
   for (i=0; i < CPS_BITMAP_LEN; i++)
      cpsInfo->sBitMap[i] = 0x00;
   cpsInfo->nLength = 0;
   memset(cpsInfo->sData, '\0', MAX_CPS_DATA_LEN);
}

int GetCpsFieldType(int field_no)
{
   return(tab_CpsFldInfo[field_no].type);
}

int GetCpsFieldLength(int field_no)
{
   return(tab_CpsFldInfo[field_no].length);
}

