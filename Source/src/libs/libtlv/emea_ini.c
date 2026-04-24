#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tlv_emea.h>
#include <define.h>


tlv_info_t  tab_TlvEmeaInfo[] = 
{
   { "002", EMEA_TLV_VAR, 0 }, /*               */
   { "003", EMEA_TLV_VAR, 0 }, /*               */
   { "004", EMEA_TLV_VAR, 0 }, /*               */
   { "005", EMEA_TLV_VAR, 0 }, /*               */
   { "006", EMEA_TLV_VAR, 0 }, /*               */
   { "007", EMEA_TLV_VAR, 0 }, /*               */
   { "008", EMEA_TLV_VAR, 0 }, /*               */
   { "009", EMEA_TLV_VAR, 0 }, /*               */
   { "010", EMEA_TLV_VAR, 0 }, /*               */
   { "011", EMEA_TLV_VAR, 0 }, /*               */
   { "012", EMEA_TLV_VAR, 0 }, /*               */
   { "013", EMEA_TLV_VAR, 0 }, /*               */
   { "014", EMEA_TLV_VAR, 0 }, /*               */
   { "015", EMEA_TLV_VAR, 0 }, /*               */
   { "016", EMEA_TLV_VAR, 0 }, /*               */
   { "017", EMEA_TLV_VAR, 0 }, /*               */
   { "018", EMEA_TLV_VAR, 0 }, /*               */
   { "019", EMEA_TLV_VAR, 0 }, /*               */
   { "020", EMEA_TLV_VAR, 0 }, /*               */
   { "021", EMEA_TLV_VAR, 0 }, /*               */
   { "022", EMEA_TLV_VAR, 0 }, /*               */
   { "023", EMEA_TLV_VAR, 0 }, /*               */
   { "024", EMEA_TLV_VAR, 0 }, /*               */
   { "025", EMEA_TLV_VAR, 0 }, /*               */
   { "026", EMEA_TLV_VAR, 0 }, /*               */
   { "027", EMEA_TLV_VAR, 0 }, /*               */
   { "028", EMEA_TLV_VAR, 0 }, /*               */
   { "029", EMEA_TLV_VAR, 0 }, /*               */
   { "030", EMEA_TLV_VAR, 0 }, /*               */
   { "031", EMEA_TLV_VAR, 0 }, /*               */
   { "032", EMEA_TLV_VAR, 0 }, /*               */
   { "033", EMEA_TLV_VAR, 0 }, /*               */
   { "034", EMEA_TLV_VAR, 0 }, /*               */
   { "035", EMEA_TLV_VAR, 0 }, /*               */
   { "036", EMEA_TLV_VAR, 0 }, /*               */
   { "037", EMEA_TLV_VAR, 0 }, /*               */
   { "038", EMEA_TLV_VAR, 0 }, /*               */
   { "039", EMEA_TLV_VAR, 0 }, /*               */
   { "040", EMEA_TLV_VAR, 0 }, /*               */
   { "041", EMEA_TLV_VAR, 0 }, /*               */
   { "042", EMEA_TLV_VAR, 0 }, /*               */
   { "043", EMEA_TLV_VAR, 0 }, /*               */
   { "044", EMEA_TLV_VAR, 0 }, /*               */
   { "045", EMEA_TLV_VAR, 0 }, /*               */
   { "046", EMEA_TLV_VAR, 0 }, /*               */
   { "047", EMEA_TLV_VAR, 0 }, /*               */
   { "048", EMEA_TLV_VAR, 0 }, /*               */
   { "049", EMEA_TLV_VAR, 0 }, /*               */
   { "050", EMEA_TLV_VAR, 0 }, /*               */
   { "051", EMEA_TLV_VAR, 0 }, /*               */
   { "052", EMEA_TLV_VAR, 0 }, /*               */
   { "053", EMEA_TLV_VAR, 0 }, /*               */
   { "054", EMEA_TLV_VAR, 0 }, /*               */
   { "055", EMEA_TLV_VAR, 0 }, /*               */
};

int  GetTlvBuffLength( TSTlvEmea *tlvInfo)
{
    return(tlvInfo->nLength);
}

int  SetTlvBuffLength( TSTlvEmea *tlvInfo, int nLength)
{
    tlvInfo->nLength = nLength;
	 /*SKO290509*/ return(OK);
}

void  InitTlvEmea  (TSTlvEmea * tlvInfo)
{
   int i;

   for (i = 0; i < MAX_EMEA_TLV; i++)
   {
      tlvInfo->nPosTlv [i] = 0;
      tlvInfo->nPresent[i] = NOT_PRESENT;
   }
   tlvInfo->nLength  = 0;
   memset(tlvInfo->sTlvData,   '\0', MAX_EMEA_TLV_LEN);
}

void AdjustTlvEmea (TSTlvEmea * tlvInfo, int nIndice, int nLength)
{
   int i;

   for (i = nIndice; i < MAX_EMEA_TLV; i++)
   {
      if ( tlvInfo->nPresent[i] == PRESENT)
         tlvInfo->nPosTlv [i]  += nLength;
   }
}

int GetTlvEmeaIndice ( char * szTlvName)
{
   int nIndice, nSize;

   nSize = sizeof(tab_TlvEmeaInfo) /sizeof(tlv_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTlvName, tab_TlvEmeaInfo[nIndice].sTlvName, 3) == 0)
         return(nIndice);
      nIndice++;
   }while (nIndice < nSize);
   return(-1);
}

int GetTlvEmeaType ( int nIndice )
{
   return ( tab_TlvEmeaInfo[nIndice].nType);
}

int GetTlvEmeaLength ( int nIndice )
{
   return ( tab_TlvEmeaInfo[nIndice].nLength);
}

