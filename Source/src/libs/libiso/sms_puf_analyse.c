/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      sms_puf_analyse.c                                                     */
/*                                                                             */
/* Description                                                                 */
/*      SMS PUF Analyse Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sms_puf.h>
#include <tools.h>

int   AnalyseSmsPuf    (char *buffer_rec, TSSmsPufInfo *pufInfo)
{
   int  i, nDigits = 0;
   int  nBytes  = 0;
   char sWork[256];
   char *sPtr;
   char *sDataPtr;

   InitSmsPufInfo(pufInfo);
   sPtr = buffer_rec;
	
   memcpy( pufInfo->sBitMap, sPtr, SMS_PUF_BITMAP_LEN);
   sPtr += SMS_PUF_BITMAP_LEN ;
   memcpy (pufInfo->sData, sPtr, MAX_SMS_PUF_DATA_LEN);


   for (i = 0; i < 24; ++i)
   {
      if ( CheckBit( pufInfo->sBitMap, i))
      {
         switch ( GetSmsPufFieldType(i) )
         {
         case SMS_PUF_FIX_HEX :                           
            nDigits = GetSmsPufFieldLength(i);
            nBytes = (nDigits +1)/2;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_PUF_FIX_BCD :                           
            nDigits = GetSmsPufFieldLength(i);
            nBytes = (nDigits +1)/2;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_PUF_VAR_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits+1)/2 +1; 
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_PUF_FIX_ALPHA :                           
            nBytes = GetSmsPufFieldLength(i);
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_PUF_VAR_ALPHA:                           
            nBytes = *sPtr;					
            nBytes++;
            pufInfo->nFieldPos[i+1] = pufInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_PUF_FIX_BIN :                           
            nBytes = GetSmsPufFieldLength(i);
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

