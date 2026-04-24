#include <stdio.h>
#include <stdlib.h>
#include <cup_puf.h>
#include <tools.h>

int   AnalyseCupPuf    (char *buffer_rec, TSCupPufInfo *pufInfo)
{
   int  i, nDigits = 0;
   int  nBytes  = 0;
   char *sPtr;

   
   InitCupPufInfo(pufInfo);
   sPtr = buffer_rec;
	
   memcpy( pufInfo->sBitMap, sPtr, CUP_PUF_BITMAP_LEN);
   sPtr += CUP_PUF_BITMAP_LEN ;
   memcpy (pufInfo->sData, sPtr, MAX_CUP_PUF_DATA_LEN);


   for (i = 0; i < 24; ++i)
   {
      if ( CheckBit( pufInfo->sBitMap, i))
      {
         switch ( GetCupPufFieldType(i) )
         {
         case CUP_PUF_FIX_HEX :                           
            nDigits = GetCupPufFieldLength(i);
            nBytes = (nDigits +1)/2;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case CUP_PUF_FIX_BCD :                           
            nDigits = GetCupPufFieldLength(i);
            nBytes = (nDigits +1)/2;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case CUP_PUF_VAR_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits+1)/2 +1; 
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case CUP_PUF_FIX_ALPHA :                           
            nBytes = GetCupPufFieldLength(i);
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case CUP_PUF_VAR_ALPHA:                           
            nBytes = *sPtr;					
            nBytes++;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case CUP_PUF_FIX_BIN :                           
            nBytes = GetCupPufFieldLength(i);
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         default :
            nBytes =0;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i];
            break;
			
         } 
         sPtr += nBytes;
      } 
      else
         pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i];
   } 
   pufInfo->nLength = pufInfo->nFieldPos[24];
   return (OK);
}

