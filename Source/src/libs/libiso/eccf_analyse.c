#include <stdio.h>
#include <stdlib.h>
#include <iso_eccf.h>
#include <tools.h>

int   AnalyseEccf(char * buffer_rec, TSEccfInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 0;
   int  nBytes  = 0;
   int  nFldType  , nFldLength;
   int  nRcvHdrLen = 0;
   char szEccfMsgType[5];	
   char szEccfFldLen[4];	
   char szAscFldLen[4];	
   char *sPtr;

   InitEccfInfo(msgInfo);
   sPtr = buffer_rec;


/****
   memset(szEccfMsgType, '\0',  5);
   EbcdicToAscii(sPtr, szEccfMsgType,  4);
   msgInfo->nMsgType = atoi(szEccfMsgType);
   sPtr += 4;
****/
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy( msgInfo->sData, sPtr, MAX_ECCF_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      memset (szEccfFldLen, '\0' ,4);	
      memset (szAscFldLen, '\0' ,4);	
      if ( CheckBit( msgInfo->sBitMap, i))
      {
            nFldType   =  GetEccfFieldType(i);
            nFldLength =  GetEccfFieldLength(i);

         switch ( nFldType )
         {
         case ECCF_FIX_BIN   :
         case ECCF_FIX_ALPHA :
            nBytes = nFldLength;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case ECCF_FIX_HEX :                           
            nDigits = nFldLength;
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case ECCF_LL_HEX :                           
            memcpy (szEccfFldLen, sPtr ,2);
            EbcdicToAscii(szEccfFldLen, szAscFldLen, 2);
            nDigits = atoi(szAscFldLen);
            nBytes = (nDigits + 1)/2 + 2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case ECCF_LLL_HEX :                           
            memcpy (szEccfFldLen, sPtr ,3);
            EbcdicToAscii(szEccfFldLen, szAscFldLen, 3);
            nDigits = atoi(szAscFldLen);
            nBytes = ( nDigits + 1)/2 +3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case ECCF_LL_BIN : 
            memcpy (szEccfFldLen, sPtr ,2);
            EbcdicToAscii(szEccfFldLen, szAscFldLen, 2);
            nBytes  = atoi(szAscFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case ECCF_LLL_BIN : 
            memcpy (szEccfFldLen, sPtr ,3);
            EbcdicToAscii(szEccfFldLen, szAscFldLen, 3);
            nBytes  = atoi(szAscFldLen);
            nBytes +=3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case ECCF_LL_ALPHA : 
            memcpy (szEccfFldLen, sPtr ,2);
            EbcdicToAscii(szEccfFldLen, szAscFldLen, 2);
            nBytes  = atoi(szAscFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case ECCF_LLL_ALPHA : 
            memcpy (szEccfFldLen, sPtr ,3);
            EbcdicToAscii(szEccfFldLen, szAscFldLen, 3);
            nBytes  = atoi(szAscFldLen);
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
   for (i = iMapCtr*64; i< MAX_ECCF_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
	  /*SKO290509*/ return(0);
}

int AnalyseEccfIc(char * buffer, int nLength, TSTagEccf * tagInfo)
{
   char  *sPtr;
   int   offset;
   int   nTagLength, nBytes;
   int   nTagIndice;
   int   nLenTag; /* Length of the PDS TAG subelement */
   char  szTagName [ 5 ];

   InitEccfIcTag(tagInfo);

   offset = 0;
   tagInfo->nLength = nLength;
   memcpy (tagInfo->sChipData,  buffer,  nLength);

   sPtr = buffer;

   while (offset < nLength)
   {
      memset( szTagName, '0', 4);
      if ( ((*sPtr) & 0x1F) == 0x1F)
      {
         nBytes = HexToAscii( sPtr, szTagName, 4);
         nLenTag = 2;
      }
      else
      {
         nBytes = HexToAscii( sPtr, szTagName, 2);
         nLenTag = 1;
      }
      if (memcmp(szTagName, "0000", 4) == 0)
         break;
      if ( (nTagIndice = GetEccfTagIndice( szTagName)) >= 0)
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

int AnalyseEccfPds(char *buffer, int nLength, TSPdsEccf *pdsInfo)
{
   char  *sPtr;
   int   offset;
   int   nPdsLength, nBytes;
   int   nPdsIndice;
   int   nLenPds; /* Length of the PDS TAG subelement */
   char  szPdsName [ 5 ];
   char  szPdsValueLen [ 5 ];
   int   nPdsValueLen ;

   InitEccfPds(pdsInfo);

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
         nPdsIndice = GetEccfPdsIndice( szPdsName); 

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

