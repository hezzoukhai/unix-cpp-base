#include <stdio.h>
#include <stdlib.h>
#include <iso_jcc.h>
#include <tools.h>

int   AnalyseJcc(char * buffer_rec, TSJccInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 0;
   int  nBytes  = 0;
   int  nFldType  , nFldLength;
   int  nRcvHdrLen = 0;
   char szJccMsgType[5];	
   char szJccFldLen[4];	
   char szAscFldLen[4];	
   char *sPtr;

   InitJccInfo(msgInfo);
   sPtr = buffer_rec;


   memset(szJccMsgType, '\0',  5);
   memcpy(szJccMsgType, sPtr,  2);
   nDigits = BcdToAscii(sPtr, szJccMsgType, 4);
   msgInfo->nMsgType = atoi(szJccMsgType);
   sPtr += 2;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy( msgInfo->sData, sPtr, MAX_JCC_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      memset (szJccFldLen, '\0' ,4);	
      memset (szAscFldLen, '\0' ,4);	
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         nFldType   =  GetJccFieldType(i);
         nFldLength =  GetJccFieldLength(i);
         switch ( nFldType )
         {
         case JCC_FIX_BIN   :
         case JCC_FIX_ALPHA :
            nBytes = nFldLength;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case JCC_FIX_BCD :                           
         case JCC_FIX_HEX :                           
            nDigits = nFldLength;
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case JCC_LL_HEX :                           
         case JCC_LL_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits + 1)/2 + 1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case JCC_LLL_HEX :                           
         case JCC_LLL_BCD :                           
            nDigits = *sPtr*256 + *(sPtr +1);
            nBytes = ( nDigits + 1)/2 +2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case JCC_LL_BIN : 
            nBytes  = *sPtr;
            nBytes +=1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case JCC_LLL_BIN : 
            nBytes  = *sPtr*256 + *(sPtr +1);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case JCC_LL_ALPHA : 
            nBytes  = *sPtr;
            nBytes +=1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case JCC_LLL_ALPHA : 
            nBytes  = *sPtr*256 + *(sPtr +1);
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
   for (i = iMapCtr*64; i< MAX_JCC_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
	  /*SKO290509*/ return(0);
}

int AnalyseJccIc(char * buffer, int nLength, TSTagJcc * tagInfo)
{
   char  *sPtr;
   int   offset;
   int   nTagLength, nBytes;
   int   nTagIndice;
   int   nLenTag; /* Length of the PDS TAG subelement */
   char  szTagName [ 5 ];

   InitJccIcTag(tagInfo);

   offset = 0;
   tagInfo->nLength = nLength;
   memcpy (tagInfo->sChipData,  buffer,  nLength);

   sPtr = buffer;

   while (offset < nLength)
   {
      memset( szTagName, '0', 4);
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
      if ( (nTagIndice = GetJccTagIndice( szTagName)) >= 0)
      {
         tagInfo->nPosTag [ nTagIndice ] = offset;
         tagInfo->nPresent[ nTagIndice ] = PRESENT;
      }

      nTagLength =  nLenTag + 1 + (*(sPtr + nLenTag));
      offset += nTagLength;
      sPtr   += nTagLength;
   }
   /*SKO290509*/ return(0);
}

int AnalyseJccPds(char *buffer, int nLength, TSPdsJcc *pdsInfo)
{
   char  *sPtr;
   int   offset;
   int   nPdsLength, nBytes;
   int   nPdsIndice;
   int   nLenPds; /* Length of the PDS TAG subelement */
   char  szPdsName [ 5 ];
   char  szPdsValueLen [ 5 ];
   int   nPdsValueLen ;

   InitJccPds(pdsInfo);

   offset = 0;
   pdsInfo->nLength = nLength;
   memcpy (pdsInfo->sPdsData,  buffer,  nLength);

   sPtr = buffer;

   while (offset < nLength)
   {
      memset( szPdsName,     '0', 4);
      memset( szPdsValueLen,  0 , 4);
         EbcdicToAscii( sPtr+2,   szPdsName,     2);
         EbcdicToAscii( sPtr,     szPdsValueLen, 2);
         nPdsValueLen = atoi(szPdsValueLen)-2;
         nLenPds = 2;
         nPdsIndice = GetJccPdsIndice( szPdsName); 

      if ( nPdsIndice >= 0)
      {
         pdsInfo->nPosPds [ nPdsIndice ] = offset;
         pdsInfo->nPresent[ nPdsIndice ] = PRESENT;
      }

      nPdsLength =  nLenPds + nLenPds +  nPdsValueLen;
      offset += nPdsLength;
      sPtr   += nPdsLength;
   }
   /*SKO290509*/ return(0);
}

