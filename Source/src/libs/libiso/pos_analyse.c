#include <stdio.h>
#include <stdlib.h>
#include <iso_pos.h>
#include <tools.h>


int   AnalysePos(char * buffer_rec, TSPosInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 0;
   int  nBytes  = 0;
   int  nRcvHdrLen = 0;
   char szPosMsgType[5];	
   char szPosFldLen[4];	
   char szBcdFldLen[4];	
   char *sPtr;
   int j;

   InitPosInfo(msgInfo);
   sPtr = buffer_rec;


   memset(szPosMsgType, '\0',  sizeof(szPosMsgType));
   memcpy ( (char *) &(msgInfo->sHeader ), sPtr, sizeof(TSPosHeader));
   sPtr += sizeof(TSPosHeader);

   memcpy(szPosMsgType, sPtr,  4);
   msgInfo->nMsgType = atoi(szPosMsgType);
   sPtr += 4;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
  
   
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);

     
   memcpy( msgInfo->sData, sPtr, MAX_POS_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      memset (szPosFldLen, '\0' ,sizeof(szPosFldLen));	
      memset (szBcdFldLen, '\0' ,sizeof(szBcdFldLen));

      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetPosFieldType(i) )
         {
         case POS_FIX_BIN   :
         case POS_FIX_ALPHA :
            nBytes = GetPosFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POS_FIX_HEX :                           
            nDigits = GetPosFieldLength(i);
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POS_LL_HEX :                           
            memcpy (szBcdFldLen, sPtr ,1);
            nDigits = BcdToAscii(szBcdFldLen, szPosFldLen, 2);
            nDigits = atoi(szPosFldLen);
            nBytes = (nDigits + 1)/2 + 1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POS_LLL_HEX :                           
            memcpy (szBcdFldLen, sPtr ,2);
            nDigits = BcdToAscii(szBcdFldLen, szPosFldLen, 3);
            nDigits = atoi(szPosFldLen);
            nBytes = ( nDigits + 1)/2 +2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POS_LL_BIN : 
            memcpy (szBcdFldLen, sPtr ,1);
            nDigits = BcdToAscii(szBcdFldLen, szPosFldLen, 2);
            nBytes  = atoi(szPosFldLen);
            nBytes +=1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POS_LLL_BIN : 
            memcpy (szBcdFldLen, sPtr ,2);
            nDigits = BcdToAscii(szBcdFldLen, szPosFldLen, 3);
            nBytes  = atoi(szPosFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POS_LL_ALPHA : 
            memcpy (szBcdFldLen, sPtr ,1);
            nDigits = BcdToAscii(szBcdFldLen, szPosFldLen, 2);
            nBytes  = atoi(szPosFldLen);
            nBytes +=1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case POS_LLL_ALPHA : 
            memcpy (szBcdFldLen, sPtr ,2);
            nDigits = BcdToAscii(szBcdFldLen, szPosFldLen, 3);
            nBytes  = atoi(szPosFldLen);
            nBytes +=2; 
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
   for (i = iMapCtr*64; i< MAX_POS_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];

   return ( OK );
}

int AnalysePosIc(char * buffer, int nLength, TSTagPos * tagInfo)
{
   char  *sPtr;
   int   offset;
   int   nTagLength, nBytes;
   int   nTagIndice;
   int   nLenTag;
   char  szTagName [ 5 ];

   InitPosIcTag(tagInfo);

   offset = 0;
   tagInfo->nLength = nLength;
   memcpy (tagInfo->sChipData,  buffer,  nLength);

   sPtr = buffer;

   while (offset < nLength)
   {
      memset( szTagName, '0', 4);
      if ( ((*sPtr) & 0x1F ) == 0x1F)
      {
         nBytes  = HexToAscii( sPtr, szTagName, 4);
         nLenTag = 2;
      }
      else
      {
         nBytes  = HexToAscii( sPtr, szTagName, 2);
         nLenTag = 1;
      }

      if ( nTagIndice = GetPosTagIndice( szTagName) >= 0)
      {
	 tagInfo->nPosTag [ nTagIndice ] = offset;
	 tagInfo->nPresent[ nTagIndice ] = PRESENT;
      }

      nTagLength =  nLenTag + 1 + (*(sPtr + nLenTag));
      offset += nTagLength;
      sPtr   += nTagLength;
   }
   
   return(OK);
}
