/*
-----------------------------------------------------------------------------------------------------------------------------------------
-- Description : 
	-------------------------------------------------------------------------------------------------------------------------------------
	
-----------------------------------------------------------------------------------------------------------------------------------------
-- Modification History : 
	-------------------------------------------------------------------------------------------------------------------------------------
	-- Version    Date          	Author       		Jalon                     Activity                          Comment
	-------------------------------------------------------------------------------------------------------------------------------------
	-- V3.0.0     29/12/2014    	HPS                                                                           	Initial version
-----------------------------------------------------------------------------------------------------------------------------------------
*/


#include <stdio.h>
#include <stdlib.h>
#include <iso_uaeswitch.h>
#include <tools.h>

int   AnalyseUaeSwitch    (char *buffer_rec, TSUaeSwitchInfo *msgInfo)
{
   unsigned char   *sPtr;
   char            *sDataPtr;
   char            sWork[256];
   char            szUaeSwitchMsgType[5];	
   int             i; 
   int             nDigits = 0;
   int             iMapCtr = 0;
   int             nBytes  = 0;
   int             nRcvHdrLen = 0;
   
   InitUaeSwitchInfo(msgInfo);
   sPtr = buffer_rec;

   nRcvHdrLen = UAESWITCH_HEADER_LEN ;
   memcpy((char *)&(msgInfo->sHeader), sPtr, nRcvHdrLen);
   sPtr += nRcvHdrLen;

   BcdToAscii(sPtr, szUaeSwitchMsgType, 4); 
   szUaeSwitchMsgType[4]='\0';	
   msgInfo->nMsgType = atoi(szUaeSwitchMsgType);
   sPtr += 2;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80)   && (iMapCtr < 3)   )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy (msgInfo->sData, sPtr, MAX_UAESWITCH_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetUaeSwitchFieldType(i) )
         {
         case UAESWITCH_FIX_BCD :                           
            nDigits = GetUaeSwitchFieldLength(i);
            nBytes = (nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case UAESWITCH_FIX_HEX :                           
            nDigits = GetUaeSwitchFieldLength(i);
            nBytes = (nDigits +1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case UAESWITCH_VAR_HEX :                           
            nDigits = *sPtr;
            nBytes = (nDigits + 1)/2 +1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case UAESWITCH_VAR_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits +1)/2 +1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case UAESWITCH_VAR_SBCD :                           
            nDigits = *sPtr;
            nBytes = nDigits+1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case UAESWITCH_FIX_ALPHA :                           
            nBytes = GetUaeSwitchFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case UAESWITCH_VAR_ALPHA:                           
            nBytes = *sPtr;					
            nBytes++;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case UAESWITCH_FIX_BIN :                           
            nBytes = GetUaeSwitchFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case UAESWITCH_VAR_BIN    :                           
         case UAESWITCH_VAR_BITMAP :                           
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
   for (i = iMapCtr*64; i< MAX_UAESWITCH_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
}

