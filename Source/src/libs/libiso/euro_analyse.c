#include <stdio.h>
#include <stdlib.h>
#include <iso_euro.h>
#include <tools.h>

int   AnalyseEuro(char * buffer_rec, TSEuroInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 0;
   int  nBytes  = 0;
   int  nFldType  , nFldLength;
   int  nRcvHdrLen = 0;
   char szEuroMsgType[5];	
   char szEuroFldLen[4];	
   char szAscFldLen[4];	
   char *sPtr;

   InitEuroInfo(msgInfo);
   sPtr = buffer_rec;


   memset(szEuroMsgType, '\0',  5);
   EbcdicToAscii(sPtr, szEuroMsgType,  4);
   msgInfo->nMsgType = atoi(szEuroMsgType);
   sPtr += 4;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy( msgInfo->sData, sPtr, MAX_EURO_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      memset (szEuroFldLen, '\0' ,4);	
      memset (szAscFldLen, '\0' ,4);	
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         if  ( (msgInfo->nMsgType == 1304) ||
               (msgInfo->nMsgType == 1314) )
         {
            nFldType   =  GetEuro93FieldType(i);
            nFldLength =  GetEuro93FieldLength(i);
         }
         else
         {
            nFldType   =  GetEuroFieldType(i);
            nFldLength =  GetEuroFieldLength(i);
         }

         switch ( nFldType )
         {
         case EURO_FIX_BIN   :
         case EURO_FIX_ALPHA :
            nBytes = nFldLength;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case EURO_FIX_HEX :                           
            nDigits = nFldLength;
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case EURO_LL_HEX :                           
            memcpy (szEuroFldLen, sPtr ,2);
            EbcdicToAscii(szEuroFldLen, szAscFldLen, 2);
            nDigits = atoi(szAscFldLen);
            nBytes = (nDigits + 1)/2 + 2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case EURO_LLL_HEX :                           
            memcpy (szEuroFldLen, sPtr ,3);
            EbcdicToAscii(szEuroFldLen, szAscFldLen, 3);
            nDigits = atoi(szAscFldLen);
            nBytes = ( nDigits + 1)/2 +3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case EURO_LL_BIN : 
            memcpy (szEuroFldLen, sPtr ,2);
            EbcdicToAscii(szEuroFldLen, szAscFldLen, 2);
            nBytes  = atoi(szAscFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case EURO_LLL_BIN : 
            memcpy (szEuroFldLen, sPtr ,3);
            EbcdicToAscii(szEuroFldLen, szAscFldLen, 3);
            nBytes  = atoi(szAscFldLen);
            nBytes +=3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case EURO_LL_ALPHA : 
            memcpy (szEuroFldLen, sPtr ,2);
            EbcdicToAscii(szEuroFldLen, szAscFldLen, 2);
            nBytes  = atoi(szAscFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case EURO_LLL_ALPHA : 
            memcpy (szEuroFldLen, sPtr ,3);
            EbcdicToAscii(szEuroFldLen, szAscFldLen, 3);
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
   for (i = iMapCtr*64; i< MAX_EURO_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
	  
	  /*SKO290509*/ return(0);
}

int AnalyseEuroIc(char * buffer, int nLength, TSTagEuro * tagInfo)
{
   char  *sPtr;
   int   offset;
   int   nTagLength, nBytes;
   int   nTagIndice;
   int   nLenTag; /* Length of the PDS TAG subelement */
   char  szTagName [ 5 ];

   InitEuroIcTag(tagInfo);

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
      nTagIndice = GetEuroTagIndice( szTagName);
      if ( nTagIndice >= 0)
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

int AnalyseEuroPds(char *buffer, int nLength, int nMsgType, TSPdsEuro *pdsInfo)
{
   char  *sPtr;
   int   offset;
   int   nPdsLength, nBytes;
   int   nPdsIndice;
   int   nLenPds; /* Length of the PDS TAG subelement */
   char  szPdsName [ 5 ];
   char  szPdsValueLen [ 5 ];
   int   nPdsValueLen ;

   InitEuroPds(pdsInfo);
   pdsInfo->nMsgType = nMsgType;

   offset = 0;
   pdsInfo->nLength = nLength;
   memcpy (pdsInfo->sPdsData,  buffer,  nLength);

   sPtr = buffer;

   while (offset < nLength)
   {
      memset( szPdsName,     '0', 4);
      memset( szPdsValueLen,  0 , 4);
      if ( (nMsgType == 1304 ) ||
           (nMsgType == 1314 ) )
      {
         EbcdicToAscii( sPtr, szPdsName,  3);
         EbcdicToAscii( sPtr+3, szPdsValueLen, 3);
         nPdsValueLen = atoi(szPdsValueLen);
         nLenPds = 3;
         nPdsIndice = GetEuroPds93Indice( szPdsName); 
      }
      else
      {
         EbcdicToAscii( sPtr+2, szPdsName,    2);
         EbcdicToAscii( sPtr, szPdsValueLen,  2);
         nPdsValueLen = atoi(szPdsValueLen)-2;
         nLenPds = 2;
         nPdsIndice = GetEuroPdsIndice( szPdsName); 
      }

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

