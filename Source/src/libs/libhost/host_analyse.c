/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      bank_analyse.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      BANK Analyse Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <iso_bank.h>
#include <tools.h>

int   AnalyseBank(char * buffer_rec, TSBankInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 0;
   int  nBytes  = 0;
   int  nRcvHdrLen = 0;
   char szBankMsgType[5];	
   char szBankFldLen[4];	
   char *sPtr;

   InitBankInfo(msgInfo);
   sPtr = buffer_rec;
   memcpy((char *)&(msgInfo->sHeader), sPtr, BANK_HDR_LEN);
   sPtr += BANK_HDR_LEN;


   memset(szBankMsgType, '\0',  5);
   memcpy(szBankMsgType, sPtr,  4);
   msgInfo->nMsgType = atoi(szBankMsgType);
   sPtr += 4;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy( msgInfo->sData, sPtr, MAX_BANK_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      memset (szBankFldLen, '\0' ,4);	
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetBankFieldType(i) )
         {
         case BANK_FIX_BIN   :
         case BANK_FIX_ALPHA :
            nBytes = GetBankFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case BANK_FIX_HEX :                           
            nDigits = GetBankFieldLength(i);
            nBytes = ( nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case BANK_LL_HEX :                           
            memcpy (szBankFldLen, sPtr ,2);
            nDigits = atoi(szBankFldLen);
            nBytes = (nDigits + 1)/2 + 2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case BANK_LLL_HEX :                           
            memcpy (szBankFldLen, sPtr ,3);
            nDigits = atoi(szBankFldLen);
            nBytes = ( nDigits + 1)/2 +3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case BANK_LL_BIN : 
            memcpy (szBankFldLen, sPtr ,2);
            nBytes  = atoi(szBankFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case BANK_LLL_BIN : 
            memcpy (szBankFldLen, sPtr ,3);
            nBytes  = atoi(szBankFldLen);
            nBytes +=3; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case BANK_LL_ALPHA : 
            memcpy (szBankFldLen, sPtr ,2);
            nBytes  = atoi(szBankFldLen);
            nBytes +=2; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
            break;

         case BANK_LLL_ALPHA : 
            memcpy (szBankFldLen, sPtr ,3);
            nBytes  = atoi(szBankFldLen);
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
   for (i = iMapCtr*64; i< MAX_BANK_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
   return(OK);
}

int AnalyseBankIc(char * buffer, int nLength, TSTagBank * tagInfo)
{
   char  *sPtr;
   int   offset;
   int   nTagLength, nBytes;
   int   nTagIndice;
   int   nLenTag;
   char  szTagName [ 5 ];
   char  sLine[MAX_LINE_TRC];

   InitBankIcTag(tagInfo);

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
         nBytes = HexToAscii( (unsigned char*)sPtr, (unsigned char*)szTagName, 4);
         nLenTag = 2;
      }
      else
      {
         nBytes = HexToAscii( (unsigned char*)sPtr, (unsigned char*)szTagName, 2);
         nLenTag = 1;
      }

      nTagIndice = GetBankTagIndice( szTagName);
      if ( nTagIndice >= 0)
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
