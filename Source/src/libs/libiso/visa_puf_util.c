#include <stdio.h>
#include <stdlib.h>

#include <visa_puf.h>
#include <tools.h>
#include <define.h>

int GetVisaPufSubField(int field_no, TSVisaPufInfo *pufInfo, char *data, int *nLength)
{
   int length;
   int nBytes;

   if (CheckBit (pufInfo->sBitMap, field_no))
   {
      switch (GetVisaPufFieldType(field_no))
      {
      case VISA_PUF_FIX_BIN :
         length = GetVisaPufFieldLength(field_no);
         memcpy( data, pufInfo->sData + pufInfo->nFieldPos[field_no], length);
         *nLength = length;
         break;

      case VISA_PUF_FIX_HEX :                           
         length = GetVisaPufFieldLength(field_no);
         nBytes = HexToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case VISA_PUF_FIX_BCD :                           
         length = GetVisaPufFieldLength(field_no);
         nBytes = BcdToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case VISA_PUF_VAR_BCD :                           
         length = *(pufInfo->sData + pufInfo->nFieldPos[field_no]);
         nBytes = BcdToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no]+1, 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case VISA_PUF_FIX_ALPHA :                           
         length = GetVisaPufFieldLength(field_no);
         EbcdicToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case VISA_PUF_VAR_ALPHA :                           
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

int AddVisaPufSubField(int field_no, TSVisaPufInfo *pufInfo, char *data, int nLength)
{
   int nBytes  = 0;
   char sLine[LG_MAX];


   if ( CheckBit( pufInfo->sBitMap, field_no))
      return(ECHEC);

   pufInfo->nFieldPos[field_no] = pufInfo->nLength;
   switch (GetVisaPufFieldType(field_no))
   {
   case VISA_PUF_FIX_BIN :
      nBytes = GetVisaPufFieldLength(field_no);
      memcpy( pufInfo->sData+pufInfo->nLength, data, nBytes);
      break;

   case VISA_PUF_FIX_BCD :
      if ( GetVisaPufFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToBcd( data, pufInfo->sData+pufInfo->nLength, nLength);
      break;

   case VISA_PUF_FIX_HEX :
      if ( GetVisaPufFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex( data, pufInfo->sData+pufInfo->nLength, nLength);
      break;

   case VISA_PUF_FIX_ALPHA :
      if ( GetVisaPufFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, pufInfo->sData+pufInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case VISA_PUF_VAR_BCD :
      *(pufInfo->sData + pufInfo->nLength) = nLength;
      pufInfo->nLength++;
      nBytes = AsciiToBcd( data, pufInfo->sData+pufInfo->nLength, nLength);
      break;

   case VISA_PUF_VAR_ALPHA :
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

int VisaPufBuildField (char * buffer_puf, TSVisaPufInfo *pufInfo)
{
   char sLine[LG_MAX];

   memcpy( buffer_puf, pufInfo->sBitMap, VISA_PUF_BITMAP_LEN);
   memcpy( buffer_puf + VISA_PUF_BITMAP_LEN , pufInfo->sData, pufInfo->nLength);
   return( VISA_PUF_BITMAP_LEN + pufInfo->nLength);
}

