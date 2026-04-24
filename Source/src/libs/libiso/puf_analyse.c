#include <stdio.h>
#include <stdlib.h>
#include <iso_puf.h>
#include <tools.h>

int   AnalysePuf    (char *buffer_rec, TSPufInfo *pufInfo)
{
   int  i, nDigits = 0;
   int  nBytes  = 0;
   char sWork[256];
   char *sPtr;
   char *sDataPtr;

   InitPufInfo(pufInfo);
   sPtr = buffer_rec;
   
   memcpy( pufInfo->sBitMap, sPtr, 3);
   sPtr += 3;
   memcpy (pufInfo->sData, sPtr, MAX_PUF_DATA_LEN);

   for (i = 0; i < 24; ++i)
   {
      if ( CheckBit( pufInfo->sBitMap, i))
      {
         switch ( GetPufFieldType(i) )
         {
         case PUF_FIX_BCD :                           
            nDigits = GetPufFieldLength(i);
            nBytes = (nDigits +1)/2;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case PUF_VAR_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits+1)/2 +1; 
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case PUF_FIX_ALPHA :                           
            nBytes = GetPufFieldLength(i);
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case PUF_VAR_ALPHA:                           
            nBytes = *sPtr;					
            nBytes++;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case PUF_FIX_BIN :                           
            nBytes = GetPufFieldLength(i);
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
   
   /*SKO290509*/ return(0);
}

