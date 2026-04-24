#include <stdio.h>
#include <stdlib.h>
#include <iso_sid.h>
#include <tools.h>

int   AnalyseHsid(char * buffer_rec, TSSidInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 0;
   int  nBytes  = 0;
   int  nRcvHdrLen = 0;
   char szIsoMsgType[5];	
   char szIsoFldLen[4];	
   char *sPtr;

   InitHsidInfo(msgInfo);
   sPtr = buffer_rec;

   sPtr += 15;

   memset(szIsoMsgType, '\0',  5);
   memcpy(szIsoMsgType, sPtr,  4);
   msgInfo->nMsgType = atoi(szIsoMsgType);
   sPtr += 4;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy( msgInfo->sData, sPtr, MAX_SID_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      memset (szIsoFldLen, '\0' ,4);	
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetHsidFieldType(i) )
         {
         case SID_FIX_BIN   :
         case SID_FIX_ALPHA :
            nBytes = GetHsidFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case SID_FIX_HEX :                           
            nDigits = GetHsidFieldLength(i);
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case SID_LL_HEX :                           
            memcpy (szIsoFldLen, sPtr ,2);
            nDigits = atoi(szIsoFldLen);
            nBytes = (nDigits + 1)/2 + 2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case SID_LLL_HEX :                           
            memcpy (szIsoFldLen, sPtr ,3);
            nDigits = atoi(szIsoFldLen);
            nBytes = ( nDigits + 1)/2 +3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case SID_LL_BIN : 
            memcpy (szIsoFldLen, sPtr ,2);
            nBytes  = atoi(szIsoFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case SID_LLL_BIN : 
            memcpy (szIsoFldLen, sPtr ,3);
            nBytes  = atoi(szIsoFldLen);
            nBytes +=3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case SID_LL_ALPHA : 
            memcpy (szIsoFldLen, sPtr ,2);
            nBytes  = atoi(szIsoFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case SID_LLL_ALPHA : 
            memcpy (szIsoFldLen, sPtr ,3);
            nBytes  = atoi(szIsoFldLen);
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
   for (i = iMapCtr*64; i< MAX_SID_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
}

int AnalyseHsidIc(char * buffer, int nLength, TSTagSid * tagInfo)
{
   char  *sPtr;
   int   offset;
   int   nTagLength, nBytes;
   int   nTagIndice;
   int   nLenTag;
   char  szTagName [ 5 ];
   char  sLine[MAX_LINE_TRC];

   InitHsidIcTag(tagInfo);

   offset = 0;
   tagInfo->nLength = nLength;
   memcpy (tagInfo->sChipData,  buffer,  nLength);

   sPtr = buffer;

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

      nTagIndice = GetHsidTagIndice( szTagName);
      if ( nTagIndice >= 0)
      {
	 	tagInfo->nPosTag [ nTagIndice ] = offset;
	 	tagInfo->nPresent[ nTagIndice ] = PRESENT;
      }

      nTagLength =  nLenTag + 1 + (*(sPtr + nLenTag));
      offset += nTagLength;
      sPtr   += nTagLength;
   }
}
