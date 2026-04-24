#include <stdio.h>
#include <stdlib.h>
#include <iso_cps.h>
#include <tools.h>

int   AnalyseCps    (char *buffer_rec, TSCpsInfo *cpsInfo)
{
   int  i, nDigits = 0;
   int  nBytes  = 0;
   char sWork[256];
   char *sPtr;

   InitCpsInfo(cpsInfo);
   sPtr = buffer_rec;
   
   memcpy( cpsInfo->sBitMap, sPtr, 8);
   sPtr += 8;
   memcpy (cpsInfo->sData, sPtr, MAX_CPS_DATA_LEN);

   for (i = 0; i < 64; ++i)
   {
      if ( CheckBit( cpsInfo->sBitMap, i))
      {
         switch ( GetCpsFieldType(i) )
         {
         case CPS_FIX_BCD :                           
            nDigits = GetCpsFieldLength(i);
            nBytes = (nDigits +1) / 2;
            cpsInfo->nFieldPos[i+1] = cpsInfo->nFieldPos[i] + nBytes ;
            break;

         case CPS_VAR_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits +1) / 2 +1;
            cpsInfo->nFieldPos[i+1] = cpsInfo->nFieldPos[i] + nBytes ;
            break;

         case CPS_FIX_ALPHA :                           
            nBytes = GetCpsFieldLength(i);
            cpsInfo->nFieldPos[i+1] = cpsInfo->nFieldPos[i] + nBytes ;
            break;

         case CPS_VAR_ALPHA:                           
            nBytes = *sPtr;					
            nBytes++;
            cpsInfo->nFieldPos[i+1] = cpsInfo->nFieldPos[i] + nBytes ;
            break;

         case CPS_FIX_BIN :                           
            nBytes = GetCpsFieldLength(i);
            cpsInfo->nFieldPos[i+1] = cpsInfo->nFieldPos[i] + nBytes ;
            break;

         default :
            nBytes =0;
            cpsInfo->nFieldPos[i+1] = cpsInfo->nFieldPos[i];
            break;
			
         } 
         sPtr += nBytes;
      } 
      else
         cpsInfo->nFieldPos[i+1] = cpsInfo->nFieldPos[i];
   } 
   cpsInfo->nLength = cpsInfo->nFieldPos[64];
   
   /*SKO290509*/ return(0);
}


