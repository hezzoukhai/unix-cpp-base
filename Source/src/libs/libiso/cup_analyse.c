#include <stdio.h>
#include <stdlib.h>
#include <iso_cup.h>
#include <tools.h>

int   AnalyseCup ( unsigned char * buffer_rec, TSCupInfo *msgInfo)
{
   int  	    i, nDigits = 0;
   int  	    iMapCtr = 0;
   int  	    nBytes  = 0;
   int  	    nRcvHdrLen = CUP_HDR_LEN;
   char 	    szCupMsgType[5];		
   char 	    szCupFldLen[4];	
   unsigned 	char *sPtr;

   memset (szCupMsgType, 0, sizeof(szCupMsgType));
   memset (szCupFldLen,  0, sizeof(szCupFldLen));
	
   InitCupInfo(msgInfo);
   sPtr = buffer_rec;

   nRcvHdrLen = (int)*sPtr;
   memcpy((char *)&(msgInfo->sHeader), sPtr, nRcvHdrLen);
  
   sPtr += nRcvHdrLen;

   memcpy(szCupMsgType, sPtr, 4);
   msgInfo->nMsgType = atoi(szCupMsgType);
   sPtr += 4;

   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80)   && (iMapCtr < 3)   )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);

   memcpy (msgInfo->sData, sPtr, MAX_CUP_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      memset (szCupFldLen, '\0' ,4);	
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetCupFieldType(i) )
         {
         case CUP_FIX_BIN   :
         case CUP_FIX_ALPHA :
            nBytes = GetCupFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case CUP_FIX_HEX :                           
            nDigits = GetCupFieldLength(i);
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case CUP_LL_HEX :                           
            memcpy (szCupFldLen, sPtr ,2);
            nDigits = atoi(szCupFldLen);
            nBytes = (nDigits + 1)/2 + 2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case CUP_LLL_HEX :                           
            memcpy (szCupFldLen, sPtr ,3);
            nDigits = atoi(szCupFldLen);
            nBytes = ( nDigits + 1)/2 +3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case CUP_LL_BIN : 
            memcpy (szCupFldLen, sPtr ,2);
            nBytes  = atoi(szCupFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case CUP_LLL_BIN : 
            memcpy (szCupFldLen, sPtr ,3);
            nBytes  = atoi(szCupFldLen);
            nBytes +=3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case CUP_LL_ALPHA : 
            memcpy (szCupFldLen, sPtr ,2);
            nBytes  = atoi(szCupFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case CUP_LLL_ALPHA : 
            memcpy (szCupFldLen, sPtr ,3);
            nBytes  = atoi(szCupFldLen);
            nBytes +=3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case CUP_CD_ALPHA :
			memset (szCupFldLen, 0, strlen(szCupFldLen));
			memcpy (szCupFldLen, sPtr, 1);
			if ( szCupFldLen[0] == 'C' || szCupFldLen[0] == 'D')
			{
					nBytes = GetCupFieldLength(i);
					msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;		
			}
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
   for (i = iMapCtr*64; i< MAX_CUP_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
	  
return (OK);
}

int AnalyseCupIc(char * buffer, int nLength, TSTagCup * tagInfo)
{
   char  *sPtr;
   int   offset;
   int   nTagLength, nBytes;
   int   nTagIndice;
   int   nLenTag;
   char  szTagName [ 5 ];
   

   memset(szTagName, 0, sizeof(szTagName));
   

   InitCupIcTag(tagInfo);

   offset = 0;
   tagInfo->nLength = nLength;
   memcpy (tagInfo->sChipData,  buffer,  nLength);

   sPtr = buffer;

   trace_event("START AnalyseCupIc()", PROCESSING);

   while (offset < nLength)
   {
      memset( szTagName, '0', 4);
      szTagName[4] = '\0';


      if ( ((*sPtr) & 0x1F ) == 0x1F)
      {
         nBytes = HexToAscii( sPtr, szTagName, 4);
         nLenTag = 2;
      }
      else
      {
         nBytes = HexToAscii( sPtr, szTagName, 2);
         nLenTag = 1;
      }

      nTagIndice = GetCupTagIndice( szTagName);
      if ( nTagIndice >= 0)
      {
	 	tagInfo->nPosTag [ nTagIndice ] = offset;
	 	tagInfo->nPresent[ nTagIndice ] = PRESENT;
      }

      nTagLength =  nLenTag + 1 + (*(sPtr + nLenTag));
      offset += nTagLength;
      sPtr   += nTagLength;
   }

   trace_event("END   AnalyseCupIc()", PROCESSING);
   return(OK);
}
