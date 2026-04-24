#include <stdio.h>
#include <stdlib.h>
#include <visa_puf.h>
#include <tools.h>

int   AnalyseVisaPuf    (char *buffer_rec, TSVisaPufInfo *pufInfo)
{
   int  i, nDigits = 0;
   int  nBytes  = 0;
   char sWork[256];
   char *sPtr;
   char *sDataPtr;

   InitVisaPufInfo(pufInfo);
   sPtr = buffer_rec;
	
   memcpy( pufInfo->sBitMap, sPtr, VISA_PUF_BITMAP_LEN);
   sPtr += VISA_PUF_BITMAP_LEN ;
   memcpy (pufInfo->sData, sPtr, MAX_VISA_PUF_DATA_LEN);


   for (i = 0; i < 24; ++i)
   {
      if ( CheckBit( pufInfo->sBitMap, i))
      {
         switch ( GetVisaPufFieldType(i) )
         {
         case VISA_PUF_FIX_HEX :                           
            nDigits = GetVisaPufFieldLength(i);
            nBytes = (nDigits +1)/2;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_PUF_FIX_BCD :                           
            nDigits = GetVisaPufFieldLength(i);
            nBytes = (nDigits +1)/2;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_PUF_VAR_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits+1)/2 +1; 
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_PUF_FIX_ALPHA :                           
            nBytes = GetVisaPufFieldLength(i);
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_PUF_VAR_ALPHA:                           
            nBytes = *sPtr;					
            nBytes++;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_PUF_FIX_BIN :                           
            nBytes = GetVisaPufFieldLength(i);
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

