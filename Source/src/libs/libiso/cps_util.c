#include <stdio.h>
#include <stdlib.h>

#include <iso_cps.h>
#include <tools.h>
#include <define.h>

int GetCpsSubField(int field_no, TSCpsInfo *cpsInfo, char *data, int *nLength)
{
   int length;
   int nBytes;

   if (CheckBit (cpsInfo->sBitMap, field_no))
   {
      switch (GetCpsFieldType(field_no))
      {
      case CPS_FIX_BIN :
         length = GetCpsFieldLength(field_no);
         memcpy( data, cpsInfo->sData + cpsInfo->nFieldPos[field_no], length);
         *nLength = length;
         break;

      case CPS_FIX_BCD :                           
         length = GetCpsFieldLength(field_no);
         nBytes = BcdToAscii(cpsInfo->sData+cpsInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case CPS_VAR_BCD :                           
         length = *(cpsInfo->sData + cpsInfo->nFieldPos[field_no]);
         nBytes = BcdToAscii(cpsInfo->sData+cpsInfo->nFieldPos[field_no]+1, 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case CPS_FIX_ALPHA :                           
         length = GetCpsFieldLength(field_no);
         EbcdicToAscii(cpsInfo->sData+cpsInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case CPS_VAR_ALPHA :                           
         length = *(cpsInfo->sData + cpsInfo->nFieldPos[field_no]);
         EbcdicToAscii(cpsInfo->sData+cpsInfo->nFieldPos[field_no]+1, 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      default :
         return(ECHEC);
         break;
         
      }
      return(SUCCES);
   }
   return(ECHEC);
}

int AddCpsSubField(int field_no, TSCpsInfo *cpsInfo, char *data, int nLength)
{
   int nBytes  = 0;
   char sLine[MAX_LINE_TRC];

   if ( CheckBit( cpsInfo->sBitMap, field_no))
      return(ECHEC);

   cpsInfo->nFieldPos[field_no] = cpsInfo->nLength;
   switch (GetCpsFieldType(field_no))
   {
   case CPS_FIX_BIN :
      nBytes = GetCpsFieldLength(field_no);
      memcpy( cpsInfo->sData+cpsInfo->nLength, data, nBytes);
      break;

   case CPS_FIX_BCD :
      if ( GetCpsFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToBcd( data, cpsInfo->sData + cpsInfo->nLength, nLength);
      break;

   case CPS_FIX_HEX :
      if ( GetCpsFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex( data, cpsInfo->sData+cpsInfo->nLength, nLength);
      break;

   case CPS_FIX_ALPHA :
      if ( GetCpsFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, cpsInfo->sData+cpsInfo->nLength, nLength);
      /* nBytes = strlen(data); */
      nBytes = nLength; 
      break;

   case CPS_VAR_BCD :
      *(cpsInfo->sData + cpsInfo->nLength) = nLength;
      cpsInfo->nLength++;
      nBytes = AsciiToBcd( data, cpsInfo->sData+cpsInfo->nLength, nLength);
      break;

   case CPS_VAR_ALPHA :
      *(cpsInfo->sData + cpsInfo->nLength) = nLength;
      cpsInfo->nLength++;
      AsciiToEbcdic(data, cpsInfo->sData+cpsInfo->nLength, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   cpsInfo->nLength += nBytes;
   SetBit(cpsInfo->sBitMap, field_no+1);
   return(SUCCES);
}

int CpsBuildField (char * buffer_cps, TSCpsInfo *cpsInfo)
{
   memcpy( buffer_cps, cpsInfo->sBitMap, 8);
   memcpy( buffer_cps + 8, cpsInfo->sData, cpsInfo->nLength);
   return( 8 + cpsInfo->nLength);
}

