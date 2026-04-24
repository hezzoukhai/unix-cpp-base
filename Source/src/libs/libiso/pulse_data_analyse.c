#include <stdio.h>
#include <stdlib.h>
#include <pulse/pulse_privfields.h>
#include <tools.h>

/* --> Not used anywhere!*/
int   AnalysePd    (char *buffer_rec, TSPdInfo *pdInfo)
{
   int  i, nDigits = 0;
   int  nBytes  = 0;
   char sWork[256];
   char *sPtr;

   InitPulseDataInfo(pdInfo);
   sPtr = buffer_rec;
   
   memcpy( pdInfo->sBitMap, sPtr, 8);
   sPtr += 8;
   memcpy (pdInfo->sData, sPtr, MAX_PD_DATA_LEN);

   for (i = 0; i < 64; ++i)
   {
      if ( CheckBit( pdInfo->sBitMap, i))
      {
         switch ( GetPdFieldType(i) )
         {
         case PD_FIX_BCD :                           
            nDigits = GetPdFieldLength(i);
            nBytes = (nDigits +1) / 2;
            pdInfo->nFieldPos[i+1] = pdInfo->nFieldPos[i] + nBytes ;
            break;

         case PD_VAR_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits +1) / 2 +1;
            pdInfo->nFieldPos[i+1] = pdInfo->nFieldPos[i] + nBytes ;
            break;

         case PD_FIX_ALPHA :                           
            nBytes = GetPdFieldLength(i);
            pdInfo->nFieldPos[i+1] = pdInfo->nFieldPos[i] + nBytes ;
            break;

         case PD_VAR_ALPHA:                           
            nBytes = *sPtr;					
            nBytes++;
            pdInfo->nFieldPos[i+1] = pdInfo->nFieldPos[i] + nBytes ;
            break;

         case PD_FIX_BIN :                           
            nBytes = GetPdFieldLength(i);
            pdInfo->nFieldPos[i+1] = pdInfo->nFieldPos[i] + nBytes ;
            break;

         default :
            nBytes =0;
            pdInfo->nFieldPos[i+1] = pdInfo->nFieldPos[i];
            break;
			
         } 
         sPtr += nBytes;
      } 
      else
         pdInfo->nFieldPos[i+1] = pdInfo->nFieldPos[i];
   } 
   pdInfo->nLength = pdInfo->nFieldPos[64];
}


