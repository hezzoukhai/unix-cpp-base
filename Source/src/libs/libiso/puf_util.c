#include <stdio.h>
#include <stdlib.h>

#include <iso_puf.h>
#include <tools.h>
#include <define.h>

int GetPufSubField(int field_no, TSPufInfo *pufInfo, char *data, int *nLength)
{
   int length;
   int nBytes;

   if (CheckBit (pufInfo->sBitMap, field_no))
   {
      switch (GetPufFieldType(field_no))
      {
      case PUF_FIX_BIN :
         length = GetPufFieldLength(field_no);
         memcpy( data, pufInfo->sData + pufInfo->nFieldPos[field_no], length);
         *nLength = length;
         break;

      case PUF_FIX_BCD :                           
         length = GetPufFieldLength(field_no);
         nBytes = BcdToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case PUF_VAR_BCD :                           
         length = *(pufInfo->sData + pufInfo->nFieldPos[field_no]);
         nBytes = BcdToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no]+1, 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case PUF_FIX_ALPHA :                           
         length = GetPufFieldLength(field_no);
         EbcdicToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case PUF_VAR_ALPHA :                           
         length = *(pufInfo->sData + pufInfo->nFieldPos[field_no]);
         EbcdicToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no]+1, 
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

int AddPufSubField(int field_no, TSPufInfo *pufInfo, char *data, int nLength)
{
   int nBytes  = 0;

   if ( CheckBit( pufInfo->sBitMap, field_no))
      return(ECHEC);

   pufInfo->nFieldPos[field_no] = pufInfo->nLength;
   switch (GetPufFieldType(field_no))
   {
   case PUF_FIX_BIN :
      nBytes = GetPufFieldLength(field_no);
      memcpy( pufInfo->sData+pufInfo->nLength, data, nBytes);
      break;

   case PUF_FIX_BCD :
      if ( GetPufFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToBcd( data, pufInfo->sData+pufInfo->nLength, nLength);
      break;

   case PUF_FIX_HEX :
      if ( GetPufFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex( data, pufInfo->sData+pufInfo->nLength, nLength);
      break;

   case PUF_FIX_ALPHA :
      if ( GetPufFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, pufInfo->sData+pufInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case PUF_VAR_BCD :
      *(pufInfo->sData + pufInfo->nLength) = nLength;
      pufInfo->nLength++;
      nBytes = AsciiToBcd( data, pufInfo->sData+pufInfo->nLength, nLength);
      break;

   case PUF_VAR_ALPHA :
      *(pufInfo->sData + pufInfo->nLength) = nLength;
      pufInfo->nLength++;
      AsciiToEbcdic(data, pufInfo->sData+pufInfo->nLength, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   pufInfo->nLength += nBytes;
   SetBit(pufInfo->sBitMap, field_no+1);
   return(SUCCES);
}

int PufBuildField (char * buffer_puf, TSPufInfo *pufInfo)
{
   memcpy( buffer_puf, pufInfo->sBitMap, 3);
   memcpy( buffer_puf + 3, pufInfo->sData, pufInfo->nLength);
   return( 3 + pufInfo->nLength);
}

