/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      rkl_analyse.c                                                         */
/*                                                                             */
/* Description                                                                 */
/*      RKL Analyse Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <iso_rkl.h>
#include <tools.h>

int   AnalyseRkl    (char *buffer_rec, TSRklInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 0;
   int  nBytes  = 0;
   int  nRcvHdrLen = 0;
   char sWork[256];
   char szRklMsgType[5];	
   char *sPtr;
   char *sDataPtr;

   InitRklInfo(msgInfo);
   sPtr = buffer_rec;
/*
   nRcvHdrLen = (int)*sPtr;                
   memcpy((char *)&(msgInfo->sHeader), sPtr, nRcvHdrLen);
   
   if (msgInfo->sHeader.bHeaderFlag & 0x80) 
      nRcvHdrLen += 22;                
   sPtr += nRcvHdrLen;
*/
   BcdToAscii((unsigned char*)sPtr,(unsigned char*) szRklMsgType, 4); 
   szRklMsgType[4]='\0';	
   msgInfo->nMsgType = atoi(szRklMsgType);
   sPtr += 2;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80)   && (iMapCtr < 3)   )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy (msgInfo->sData, sPtr, MAX_RKL_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetRklFieldType(i) )
         {
         case RKL_FIX_BCD :                           
            nDigits = GetRklFieldLength(i);
            nBytes = (nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case RKL_FIX_HEX :                           
            nDigits = GetRklFieldLength(i);
            nBytes = (nDigits +1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case RKL_VAR_HEX :                           
            nDigits = *sPtr;
            nBytes = (nDigits + 1)/2 +1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case RKL_VAR_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits +1)/2 +1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case RKL_VAR_SBCD :                           
            nDigits = *sPtr;
            nBytes = nDigits+1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case RKL_FIX_ALPHA :                           
            nBytes = GetRklFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case RKL_VAR_ALPHA:                           
            nBytes = *sPtr;					
            nBytes++;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case RKL_FIX_BIN :                           
            nBytes = GetRklFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case RKL_VAR_BIN    :                           
         case RKL_VAR_BITMAP :                           
            nBytes = *sPtr;							
            nBytes++;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;
			
         default :
            nBytes =0;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i];
            break;
			
         } 
         sPtr += nBytes;
      } 
      else
         msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i];
   } 
   msgInfo->nLength = msgInfo->nFieldPos[iMapCtr*64];
   for (i = iMapCtr*64; i< MAX_RKL_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
   return(OK);
}

