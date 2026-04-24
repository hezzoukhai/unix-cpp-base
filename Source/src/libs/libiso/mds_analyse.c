#include <stdio.h>
#include <stdlib.h>
#include <iso_mds.h>
#include <tools.h>


int   AnalyseMds(char * buffer_rec, TSMdsInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 0;
   int  nBytes  = 0;
   int  nFldType  , nFldLength;
   int  nRcvHdrLen = 0;
   char szMdsMsgType[5];	
   char szMdsFldLen[4];	
   char szAscFldLen[4];	
   char *sPtr;

   InitMdsInfo(msgInfo);
   sPtr = buffer_rec;


   memset(szMdsMsgType, '\0',  5);
   EbcdicToAscii(sPtr, szMdsMsgType,  4);
   msgInfo->nMsgType = atoi(szMdsMsgType);
   sPtr += 4;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy( msgInfo->sData, sPtr, MAX_MDS_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      memset (szMdsFldLen, '\0' ,4);	
      memset (szAscFldLen, '\0' ,4);	
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         if  ( (msgInfo->nMsgType == 1304) ||
               (msgInfo->nMsgType == 1314) )
         {
            nFldType   =  GetMds93FieldType(i);
            nFldLength =  GetMds93FieldLength(i);
         }
         else
         {
            nFldType   =  GetMdsFieldType(i);
            nFldLength =  GetMdsFieldLength(i);
         }

         switch ( nFldType )
         {
         case MDS_FIX_BIN   :
         case MDS_FIX_ALPHA :
            nBytes = nFldLength;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case MDS_FIX_HEX :                           
            nDigits = nFldLength;
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case MDS_LL_HEX :                           
            memcpy (szMdsFldLen, sPtr ,2);
            EbcdicToAscii(szMdsFldLen, szAscFldLen, 2);
            nDigits = atoi(szAscFldLen);
            nBytes = (nDigits + 1)/2 + 2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case MDS_LLL_HEX :                           
            memcpy (szMdsFldLen, sPtr ,3);
            EbcdicToAscii(szMdsFldLen, szAscFldLen, 3);
            nDigits = atoi(szAscFldLen);
            nBytes = ( nDigits + 1)/2 +3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case MDS_LL_BIN : 
            memcpy (szMdsFldLen, sPtr ,2);
            EbcdicToAscii(szMdsFldLen, szAscFldLen, 2);
            nBytes  = atoi(szAscFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case MDS_LLL_BIN : 
            memcpy (szMdsFldLen, sPtr ,3);
            EbcdicToAscii(szMdsFldLen, szAscFldLen, 3);
            nBytes  = atoi(szAscFldLen);
            nBytes +=3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case MDS_LL_ALPHA : 
            memcpy (szMdsFldLen, sPtr ,2);
            EbcdicToAscii(szMdsFldLen, szAscFldLen, 2);
            nBytes  = atoi(szAscFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case MDS_LLL_ALPHA : 
            memcpy (szMdsFldLen, sPtr ,3);
            EbcdicToAscii(szMdsFldLen, szAscFldLen, 3);
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
   for (i = iMapCtr*64; i< MAX_MDS_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
	  
	  /*SKO290509*/ return(0);
}

int AnalyseMdsIc(char * buffer, int nLength, TSTagMds * tagInfo)
{
   char  *sPtr;
   int   offset;
   int   nTagLength, nBytes;
   int   nTagIndice;
   int   nLenTag; /* Length of the PDS TAG subelement */
   char  szTagName [ 5 ];

   InitMdsIcTag(tagInfo);

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
      nTagIndice = GetMdsTagIndice( szTagName);
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

int AnalyseMdsPds(char *buffer, int nLength, int nMsgType, TSPdsMds *pdsInfo)
{
   char  *sPtr;
   int   offset;
   int   nPdsLength, nBytes;
   int   nPdsIndice;
   int   nLenPds; /* Length of the PDS TAG subelement */
   char  szPdsName [ 5 ];
   char  szPdsValueLen [ 5 ];
   int   nPdsValueLen ;

   InitMdsPds(pdsInfo);
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
	/*Mehdi Elyajizi *
         EbcdicToAscii( sPtr, szPdsName,  3);
         EbcdicToAscii( sPtr+3, szPdsValueLen, 3);
	*/
	 memcpy( szPdsName, sPtr, 3);
         memcpy( szPdsValueLen, sPtr + 3, 3);
      
         nPdsValueLen = atoi(szPdsValueLen);
         nLenPds = 3;
         nPdsIndice = GetMdsPds93Indice( szPdsName); 
      }
      else
      {
	/* Mehdi Elyajizi *
         EbcdicToAscii( sPtr+2, szPdsName,    2);
         EbcdicToAscii( sPtr, szPdsValueLen,  2);
	*/
         memcpy( szPdsName, sPtr, 2);
         memcpy( szPdsValueLen, sPtr + 2, 2);
         /*nPdsValueLen = atoi(szPdsValueLen)-2; */
		 nPdsValueLen = atoi(szPdsValueLen);  /*AMER20171002*/   
         nLenPds = 2;
         nPdsIndice = GetMdsPdsIndice( szPdsName); 
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

