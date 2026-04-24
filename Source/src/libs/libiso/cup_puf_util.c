#include <stdio.h>
#include <stdlib.h>

#include <cup_puf.h>
#include <tools.h>
#include <define.h>

int GetCupPufSubField(int field_no, TSCupPufInfo *pufInfo, char *data, int *nLength)
{
   int length;
   int nBytes;

   if (CheckBit (pufInfo->sBitMap, field_no))
   {
      switch (GetCupPufFieldType(field_no))
      {
      case CUP_PUF_FIX_BIN :
			 length = GetCupPufFieldLength(field_no);
			 memcpy( data, pufInfo->sData + pufInfo->nFieldPos[field_no], length);
			 *nLength = length;
         break;

      case CUP_PUF_FIX_HEX :                           
			 length = GetCupPufFieldLength(field_no);
			 nBytes = HexToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no], data, length); 
			 data[length] = '\0';
			 *nLength = length;
         break;

      case CUP_PUF_FIX_BCD :                           
			 length = GetCupPufFieldLength(field_no);
			 nBytes = BcdToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no], data, length); 
			 data[length] = '\0';
			 *nLength = length;
         break;

      case CUP_PUF_VAR_BCD :                           
			 length = *(pufInfo->sData + pufInfo->nFieldPos[field_no]);
			 nBytes = BcdToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no]+1, data, length); 
			 data[length] = '\0';
			 *nLength = length;
         break;

      case CUP_PUF_FIX_ALPHA :                           
			 length = GetCupPufFieldLength(field_no);
			 EbcdicToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no], data, length); 
			 data[length] = '\0';
			 *nLength = length;
         break;

      case CUP_PUF_VAR_ALPHA :                           
			 length = *(pufInfo->sData + pufInfo->nFieldPos[field_no]);
			 EbcdicToAscii(pufInfo->sData+pufInfo->nFieldPos[field_no]+1, data, length); 
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

int AddCupPufSubField(int field_no, TSCupPufInfo *pufInfo, char *data, int nLength)
{
   int nBytes  = 0, nMaxLength;
   char sLine[MAX_LINE_TRC];

   memset (sLine, 0, sizeof(sLine));

   if ( CheckBit( pufInfo->sBitMap, field_no))
      return(ECHEC);
	  
	  
   pufInfo->nFieldPos[field_no] = pufInfo->nLength;
   nMaxLength = GetCupPufFieldLength(field_no);
   
   switch (GetCupPufFieldType(field_no))
   {
	case CUP_PUF_FIX_BIN :
		if ( nLength != nMaxLength )
		{
			sprintf(sLine,"AddCupPufSubField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = nLength;
		memcpy( pufInfo->sData+pufInfo->nLength, data, nBytes);
	break;

	case CUP_PUF_FIX_BCD :
		if ( nLength != nMaxLength )
		{
			sprintf(sLine,"AddCupPufSubField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = AsciiToBcd( data, pufInfo->sData+pufInfo->nLength, nLength);
	break;

	case CUP_PUF_FIX_HEX :
		if ( nLength != nMaxLength )
		{
			sprintf(sLine,"AddCupPufSubField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = AsciiToHex( data, pufInfo->sData+pufInfo->nLength, nLength);
	break;

	case CUP_PUF_FIX_ALPHA :
		if ( nLength != nMaxLength )
		{
			sprintf(sLine,"AddCupPufSubField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		AsciiToEbcdic( data, pufInfo->sData+pufInfo->nLength, nLength);
		nBytes = nLength;
	break;

	case CUP_PUF_VAR_BCD :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"AddCupPufSubField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		*(pufInfo->sData + pufInfo->nLength) = nLength;
		pufInfo->nLength++;
		nBytes = AsciiToBcd( data, pufInfo->sData+pufInfo->nLength, nLength);
	break;

	case CUP_PUF_VAR_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"AddCupPufSubField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
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

int CupPufBuildField (char * buffer_puf, TSCupPufInfo *pufInfo)
{
   memcpy( buffer_puf, pufInfo->sBitMap, CUP_PUF_BITMAP_LEN);
   memcpy( buffer_puf + CUP_PUF_BITMAP_LEN , pufInfo->sData, pufInfo->nLength);
   return( CUP_PUF_BITMAP_LEN + pufInfo->nLength);
}

