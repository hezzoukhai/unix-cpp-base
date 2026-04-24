#include <stdio.h>
#include <stdlib.h>
#include <iso_eccf.h>
#include <define.h>


fld_info_t    tab_EccfFldInfo[] =
{
  { NOT_USED,     8},	/* 001: Primary/Secondary Bit Map                 */
  { ECCF_LL_ALPHA, 	 19},	/* 002: Primary Account Number (PAN)              */
  { ECCF_FIX_ALPHA,	  6},	/* 003: Processing Code                           */
  { ECCF_FIX_ALPHA,	 12},	/* 004: Amount, Transaction                       */
  { ECCF_FIX_ALPHA,	 12},	/* 005: Amount, Reconciliation                    */
  { ECCF_FIX_ALPHA,	 12},	/* 006: Amount, Cardholder                        */

  { NOT_USED,	      0},	/* 007: Not Used                                  */
  { NOT_USED,	      0},	/* 008: Not Used                                  */

  { ECCF_FIX_ALPHA,	  8},	/* 009: Conversion Rate, Reconciliation           */
  { ECCF_FIX_ALPHA,	  8},	/* 010: Conversion Rate, Cardholder Billing       */
  { ECCF_FIX_ALPHA,	  6},	/* 011: System Trace Audit Number                 */
  { ECCF_FIX_ALPHA,	 12},	/* 012: Date & Time, Local Transaction            */

  { NOT_USED,	      0},	/* 013: Not Used                                  */

  { ECCF_FIX_ALPHA,	  4},	/* 014: Date, Expiration (YYMM)                   */
  { ECCF_FIX_ALPHA,	  6},	/* 015: Date, Settlement (YYMMDD)                 */
  { ECCF_FIX_ALPHA,	  4},	/* 016: Date, Conversion (MMDD)                   */

  { NOT_USED,	      0},	/* 017: Not Used                                  */
  { NOT_USED,	      0},	/* 018: Not Used                                  */
  { NOT_USED,	      0},	/* 019: Not Used                                  */
  { NOT_USED,	      0},	/* 020: Not Used                                  */
  { NOT_USED,	      0},	/* 021: Not Used                                  */

  { ECCF_FIX_ALPHA,	 12},	/* 022: Point of Service Data Code                */
  { ECCF_FIX_ALPHA,	  3},	/* 023: Card Sequence Number                      */
  { ECCF_FIX_ALPHA,	  3},	/* 024: Function Code                             */
  { ECCF_FIX_ALPHA,	  4},	/* 025: Message Reason Code                       */
  { ECCF_FIX_ALPHA,	  4},	/* 026: Card Acceptor Business Code               */

  { NOT_USED,	      0},	/* 027: Not Used                                  */

  { ECCF_FIX_ALPHA,	  6},	/* 028: Date, Reconciliation (YYMMDD)             */
  { ECCF_FIX_ALPHA,	  3},	/* 029: Reconciliation Indicator                  */
  { ECCF_FIX_ALPHA,	 24},	/* 030: Amounts, Original                         */
  { ECCF_LL_ALPHA,	 23},	/* 031: Acquiring Reference Number                */
  { ECCF_LL_ALPHA,	 11},	/* 032: Acquiring Institution Identification Code */
  { ECCF_LL_ALPHA,	  9},	/* 033: Forwarding Institution Identif.Code - FID */

  { NOT_USED,	      0},	/* 034: Not Used                                  */
  { NOT_USED,	      0},	/* 035: Not Used                                  */
  { NOT_USED,	      0},	/* 036: Not Used                                  */

  { ECCF_FIX_ALPHA,	 12},	/* 037: Retrieval Reference Number                */
  { ECCF_FIX_ALPHA,	  6},	/* 038: Approval Code                             */
  { ECCF_FIX_ALPHA,	  3},	/* 039: Action Code                               */

  { NOT_USED,	      0},	/* 040: Not Used                                  */

  { ECCF_FIX_ALPHA,	  8},	/* 041: Card Acceptor Terminal Identification     */
  { ECCF_FIX_ALPHA,  15},	/* 042: Card Acceptor Identification Code         */
  { ECCF_LL_ALPHA,	 54},	/* 043: Card Acceptor Name & Location             */

  { NOT_USED,	      0},	/* 044: Not Used                                  */
  { NOT_USED,	      0},	/* 045: Not Used                                  */

  { ECCF_LLL_ALPHA,	 62},	/* 046: Amount, Fees                              */

  { NOT_USED,	      0},	/* 047: Not Used                                  */
  { NOT_USED,	      0},	/* 048: Not Used                                  */

  { ECCF_FIX_ALPHA,	  3},	/* 049: Currency Code, Transaction                */
  { ECCF_FIX_ALPHA,	  3},	/* 050: Currency Code, Reconciliation             */
  { ECCF_FIX_ALPHA,	  3},	/* 051: Currency Code, Cardholder Billing         */

  { NOT_USED,	      0},	/* 052: Not Used                                  */
  { NOT_USED,	      0},	/* 053: Not Used                                  */
  { NOT_USED,	      0},	/* 054: Not Used                                  */

  { ECCF_LLL_BIN,   255},	/* 055: ICC System Related Data                   */
  { ECCF_LL_ALPHA,   35},	/* 056: Original Data Elements                    */

  { NOT_USED,	      0},	/* 057: Not Used                                  */
  { NOT_USED,	      0},	/* 058: Not Used                                  */
  { NOT_USED,	      0},	/* 059: Not Used                                  */
  { NOT_USED,	      0},	/* 060: Not Used                                  */
  { NOT_USED,	      0},	/* 061: Not Used                                  */
  { NOT_USED,	      0},	/* 062: Not Used                                  */
  { NOT_USED,	      0},	/* 063: Not Used                                  */
  { NOT_USED,	      0},	/* 064: Not Used                                  */
  { NOT_USED,	      0},	/* 065: Not Used                                  */
  { NOT_USED,	      0},	/* 066: Not Used                                  */
  { NOT_USED,	      0},	/* 067: Not Used                                  */
  { NOT_USED,	      0},	/* 068: Not Used                                  */
  { NOT_USED,	      0},	/* 069: Not Used                                  */
  { NOT_USED,	      0},	/* 070: Not Used                                  */

  { ECCF_FIX_ALPHA,	  8},	/* 071: Message Number                            */
  { ECCF_LLL_ALPHA,	999},	/* 072: Data Record                               */

  { NOT_USED,	      0},	/* 073: Not Used                                  */

  { ECCF_FIX_ALPHA,	 10},	/* 074: Credits, Number                           */
  { ECCF_FIX_ALPHA,	 10},	/* 075: Credits, Reversal Number                  */
  { ECCF_FIX_ALPHA,	 10},	/* 076: Debits, Number                            */
  { ECCF_FIX_ALPHA,	 10},	/* 077: Debits, Reversal Number                   */

  { NOT_USED,	      0},	/* 078: Not Used                                  */
  { NOT_USED,	      0},	/* 079: Not Used                                  */
  { NOT_USED,	      0},	/* 080: Not Used                                  */
  { NOT_USED,	      0},	/* 081: Not Used                                  */
  { NOT_USED,	      0},	/* 082: Not Used                                  */
  { NOT_USED,	      0},	/* 083: Not Used                                  */
  { NOT_USED,	      0},	/* 084: Not Used                                  */

  { ECCF_FIX_ALPHA,	 10},	/* 085: Fee Collection, Number                    */
  { ECCF_FIX_ALPHA,	 16},	/* 086: Credits, Amount                           */
  { ECCF_FIX_ALPHA,	 16},	/* 087: Credits, Reversal Amount                  */
  { ECCF_FIX_ALPHA,	 16},	/* 088: Debits, Amount                            */
  { ECCF_FIX_ALPHA,	 16},	/* 089: Debits, Reversal Amount                   */

  { NOT_USED,	      0},	/* 090: Not Used                                  */
  { NOT_USED,	      0},	/* 091: Not Used                                  */
  { NOT_USED,	      0},	/* 092: Not Used                                  */

  { ECCF_LL_ALPHA, 	 11},	/* 093: Trans. Destination Instit. Identif. Code  */
  { ECCF_LL_ALPHA,   11},	/* 094: Trans. Originator Instit. Identif. Code   */
  { ECCF_LL_ALPHA,   10},	/* 095: Card Issuer Reference data	              */

  { NOT_USED,	      0},	/* 096: Not Used                                  */

  { ECCF_FIX_ALPHA,  17},	/* 097: Amount, Net Reconciliation                */

  { NOT_USED,	      0},	/* 098: Not Used                                  */
  { NOT_USED,	      0},	/* 099: Not Used                                  */

  { ECCF_LL_ALPHA,	  9},	/* 100: Receiving Institution Identif. Code - RID */

  { NOT_USED,	      0},	/* 101: Not Used                                  */
  { NOT_USED,	      0},	/* 102: Not Used                                  */
  { NOT_USED,	      0},	/* 103: Not Used                                  */

  { ECCF_LLL_ALPHA,	  4},	/* 104: Transaction Description                   */
  { ECCF_FIX_ALPHA,	 16},	/* 105: Credits, Chargeback Amount                */
  { ECCF_FIX_ALPHA,	 16},	/* 106: Debits, Chargeback Amount                 */
  { ECCF_FIX_ALPHA,	 10},	/* 107: Credits, Chargeback Number                */
  { ECCF_FIX_ALPHA,	 10},	/* 108: Debits, Chargeback Number                 */
  { ECCF_LL_ALPHA, 	 72},	/* 109: Credits, Fee Amounts                      */
  { ECCF_LL_ALPHA,   72},	/* 110: Debits, Fee Amounts                       */

  { NOT_USED,	      0},	/* 111: Not Used                                  */
  { NOT_USED,	      0},	/* 112: Not Used                                  */
  { NOT_USED,	      0},	/* 113: Not Used                                  */
  { NOT_USED,	      0},	/* 114: Not Used                                  */
  { NOT_USED,	      0},	/* 115: Not Used                                  */
  { NOT_USED,	      0},	/* 116: Not Used                                  */
  { NOT_USED,	      0},	/* 117: Not Used                                  */
  { NOT_USED,	      0},	/* 118: Not Used                                  */
  { NOT_USED,	      0},	/* 119: Not Used                                  */
  { NOT_USED,	      0},	/* 120: Not Used                                  */
  { NOT_USED,	      0},	/* 121: Not Used                                  */
  { NOT_USED,	      0},	/* 122: Not Used                                  */

  { ECCF_LLL_ALPHA,	999},	/* 123: Miscellaneous Data                        */

  { NOT_USED,	      0},	/* 124: Not Used                                  */

  { ECCF_LLL_ALPHA,	125},	/* 125: Eurocheque Private Data                   */
};

tag_info_t  tab_EccfTagInfo[] = 
{
   { "7100",  ECCF_TAG_VAR_BIN,   0},/* Issuer Script Template 1              */
   { "7200",  ECCF_TAG_VAR_BIN,   0},/* Issuer Script Template 2              */
   { "8200",  ECCF_TAG_FIX_HEX,   2},/* Application Interchange Profile       */
   { "8400",  ECCF_TAG_VAR_HEX,   0},/* Dedicated File Name                   */
   { "9100",  ECCF_TAG_VAR_BIN,   0},/* Issuer Authentication Data            */
   { "9500",  ECCF_TAG_FIX_HEX,   5},/* Terminal Verification Results         */
   { "9A00",  ECCF_TAG_FIX_BCD,   6},/* Transaction Date                      */
   { "9C00",  ECCF_TAG_FIX_BCD,   2},/* Transaction TYPE                      */
   { "5F2A",  ECCF_TAG_FIX_BCD,   3},/* Transaction Currency Code             */
   { "9F02",  ECCF_TAG_FIX_BCD,  12},/* Transaction Amount                    */
   { "9F03",  ECCF_TAG_FIX_BCD,  12},/* Other Amount                          */
   { "9F09",  ECCF_TAG_FIX_HEX,   2},/* Terminal Application Version Number   */
   { "9F10",  ECCF_TAG_VAR_HEX,   0},/*  Issuer Application Data              */
   { "9F1A",  ECCF_TAG_FIX_BCD,   3},/* Terminal Country Code                 */
   { "9F1E",  ECCF_TAG_FIX_ALPHA, 8},/* Interface Device Serial Number        */
   { "9F26",  ECCF_TAG_FIX_HEX,   8},/* Application cryptogram                */
   { "9F27",  ECCF_TAG_FIX_HEX,   1},/* Cryptogram Information Data           */
   { "9F33",  ECCF_TAG_FIX_HEX,   3},/* Terminal Capabilities                 */
   { "9F34",  ECCF_TAG_FIX_HEX,   3},/* Cardholder Verification Method        */
   { "9F35",  ECCF_TAG_FIX_BCD,   2},/* Terminal Type                         */
   { "9F36",  ECCF_TAG_FIX_HEX,   2},/* Application Transaction Counter       */
   { "9F37",  ECCF_TAG_FIX_HEX,   4},/* Unpredictable Number                  */
   { "9F41",  ECCF_TAG_VAR_BCD,   0},/* Transaction Sequence Number           */
   { "9F53",  ECCF_TAG_FIX_ALPHA, 1},/* Transaction Category Code             */
};

pds_info_t  tab_EccfPdsInfo[] = 
{
   { "01",  ECCF_PDS_FIX_ALPHA,  16},/* DES HMK (PBK)                     */
   { "02",  ECCF_PDS_FIX_ALPHA,   2},/* Traffic Flags                     */
   { "03",  ECCF_PDS_FIX_ALPHA,   3},/* Version Number                    */
   { "04",  ECCF_PDS_FIX_ALPHA,   2},/* SIGN-OFF Reason Code              */
   { "05",  ECCF_PDS_FIX_ALPHA,  96},/* Rejected Message                  */
   { "06",  ECCF_PDS_FIX_ALPHA,   2},/* Reject Reason Code                */
   { "07",  ECCF_PDS_FIX_ALPHA,   2},/* Test Production Flags             */
   { "08",  ECCF_PDS_FIX_ALPHA,   2},/* PIN Block Format Code             */
   { "20",  ECCF_PDS_FIX_ALPHA,   1},/* Cardholder Verification Method    */
   { "40",  ECCF_PDS_VAR_ALPHA,  40},/* Certificate Serial Number         */
   { "42",  ECCF_PDS_FIX_ALPHA,   2},/* Security Level Indicator          */
   { "43",  ECCF_PDS_FIX_ALPHA,   32},/* Security Level Indicator          */
   { "87",  ECCF_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Invalid       */
   { "88",  ECCF_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Status Change */
   { "89",  ECCF_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Track Edit    */
   { "92",  ECCF_PDS_FIX_ALPHA,   3},/* Card Verification Code 2          */
};

void  InitEccfIcTag  (TSTagEccf * msgInfo)
{
   int i;

   for (i = 0; i < MAX_ECCF_CHIP_TAG; i++)
   {
      msgInfo->nPosTag [i] = 0;
      msgInfo->nPresent[i] = NOT_PRESENT;
   }
   msgInfo->nLength = 0;
   memset(msgInfo->sChipData,   '\0', MAX_ECCF_CHIP_LEN);
}

void  InitEccfPds  (TSPdsEccf * pdsInfo)
{
   int i;

   for (i = 0; i < MAX_ECCF_PDS; i++)
   {
      pdsInfo->nPosPds [i] = 0;
      pdsInfo->nPresent[i] = NOT_PRESENT;
   }
   pdsInfo->nLength  = 0;
   memset(pdsInfo->sPdsData,   '\0', MAX_ECCF_PDS_LEN);
}

void  InitEccfInfo(TSEccfInfo *msgInfo)
{
   int i;

   for (i=0; i < MAX_ECCF_FIELDS+1; i++)
      msgInfo->nFieldPos[i] = 0;
   for (i=0; i < ECCF_BITMAP_LEN; i++)
      msgInfo->sBitMap[i] = 0x00;
   msgInfo->nLength = 0;
   msgInfo->nMsgType = 0;
   memset(msgInfo->sData,   '\0', MAX_ECCF_DATA_LEN);
}

int GetEccfFieldType(int field_no)
{
   return(tab_EccfFldInfo[field_no].type);
}

int GetEccfFieldLength(int field_no)
{
   return(tab_EccfFldInfo[field_no].length);
}

int GetEccfTagIndice ( char * szTagName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_EccfTagInfo) /sizeof(tag_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTagName, tab_EccfTagInfo[nIndice].sTagName, 4) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetEccfTagType ( int nIndice )
{
   return ( tab_EccfTagInfo[nIndice].nType);
}

int GetEccfTagLength ( int nIndice )
{
   return ( tab_EccfTagInfo[nIndice].nLength);
}

int GetEccfPdsIndice ( char * szPdsName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_EccfPdsInfo) /sizeof(pds_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szPdsName, tab_EccfPdsInfo[nIndice].sPdsName, 2) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetEccfPdsType ( int nIndice )
{
   return ( tab_EccfPdsInfo[nIndice].nType);
}

int GetEccfPdsLength ( int nIndice )
{
   return ( tab_EccfPdsInfo[nIndice].nLength);
}


