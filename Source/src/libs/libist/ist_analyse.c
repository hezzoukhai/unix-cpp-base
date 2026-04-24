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
#include <iso_ist.h>
#include <tools.h>

int   AnalyseIst    (char *buffer_rec, TSIstInfo *msgInfo)
{
   unsigned char         *sPtr;
   char                  sMsgType[5]; 
   char                  *sDataPtr;
   char              	 sLine[MAX_LINE_TRC];
   int                   i; 
   int                   nDigits = 0;
   int                   iMapCtr = 0;
   int                   nBytes  = 0;
   int                   nRcvHdrLen = 0;
   char    				szField[256];
   char 		szIstFldLen[4];
   
   InitIstInfo(msgInfo);
   sPtr = buffer_rec;

   
   memset(sMsgType, '\0',  sizeof(sMsgType));
   memcpy(sMsgType, sPtr,  4);
   msgInfo->nMsgType = atoi(sMsgType);
   sPtr += 4;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80)   && (iMapCtr < 3)   )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
/*
    iMapCtr = 1;
    do
    {
          AsciiToHex(sPtr,msgInfo->sBitMap + (iMapCtr*8), 16); sPtr+=16;
          ++iMapCtr;
    } while(( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80)   && (iMapCtr < 3));
*/

  
   memcpy (msgInfo->sData, sPtr, MAX_IST_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetIstFieldType(i) )
         {
         case IST_FIX_BCD :                           
            nDigits = GetIstFieldLength(i);
            nBytes = (nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case IST_FIX_HEX :                           
            nDigits = GetIstFieldLength(i);
            nBytes = (nDigits +1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case IST_VAR_HEX :                           
            nDigits = *sPtr;
            nBytes = (nDigits + 1)/2 +1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case IST_VAR_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits +1)/2 +1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case IST_VAR_SBCD :                           
            nDigits = *sPtr;
            nBytes = nDigits+1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case IST_FIX_ALPHA :                           
            nBytes = GetIstFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case IST_VAR_ALPHA:                           
            /*nBytes = *sPtr;					
            nBytes++;*/
			memset(szField,         '\0', sizeof(szField));
			memcpy ( szField, sPtr, 2);
            nBytes = atoi(szField);
			nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case IST_FIX_BIN :                           
            nBytes = GetIstFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case IST_VAR_BIN    :                           
         case IST_VAR_BITMAP :                           
            nBytes = *sPtr;							
            nBytes++;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;
/*
	case IST_LL_ALPHA : 
	    memcpy (szIstFldLen, sPtr ,2);
	    nBytes  = atoi(szIstFldLen);
	    nBytes +=2; 
	    msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
	    break;
*/

        case IST_LLL_ALPHA : 
	    memcpy (szIstFldLen, sPtr ,3);
	    nBytes  = atoi(szIstFldLen);
	    nBytes +=3; 
	    msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
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
   for (i = iMapCtr*64; i< MAX_IST_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
	  
return OK;
}

