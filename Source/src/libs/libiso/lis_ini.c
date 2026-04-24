#include <stdio.h>
#include <stdlib.h>
#include <iso_eccf.h>
#include <define.h>

tag_info_t  tab_LisTagInfo[] = 
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

pds_info_t  tab_LisPdsInfo[] = 
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
   { "87",  ECCF_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Invalid       */
   { "88",  ECCF_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Status Change */
   { "89",  ECCF_PDS_FIX_ALPHA,   1},/* Magnetic Stripe CVC Track Edit    */
   { "92",  ECCF_PDS_FIX_ALPHA,   3},/* Card Verification Code 2          */
};

void  InitLisIcTag  (TSTagEccf * msgInfo)
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

void  InitLisPds  (TSPdsEccf * pdsInfo)
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

void  InitLisInfo(TSEccfInfo *msgInfo)
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

int GetLisFieldType(int field_no)
{
   return(tab_LisFldInfo[field_no].type);
}

int GetLisFieldLength(int field_no)
{
   return(tab_EccfFldInfo[field_no].length);
}

int GetLisTagIndice ( char * szTagName)
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

int GetLisTagType ( int nIndice )
{
   return ( tab_EccfTagInfo[nIndice].nType);
}

int GetLisTagLength ( int nIndice )
{
   return ( tab_EccfTagInfo[nIndice].nLength);
}

int GetLisPdsIndice ( char * szPdsName)
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

int GetLisPdsType ( int nIndice )
{
   return ( tab_EccfPdsInfo[nIndice].nType);
}

int GetLisPdsLength ( int nIndice )
{
   return ( tab_EccfPdsInfo[nIndice].nLength);
}


