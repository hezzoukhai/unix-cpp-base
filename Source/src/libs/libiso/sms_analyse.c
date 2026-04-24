#include <stdio.h>
#include <stdlib.h>
#include <iso_sms.h>
#include <tools.h>

int   AnalyseSms    (char *buffer_rec, TSSmsInfo *msgInfo)
{
   int  i, nDigits = 0;
   int  iMapCtr = 0;
   int  nBytes  = 0;
   int  nRcvHdrLen = 0;
   char sWork[256];
   char szSmsMsgType[5];	
   char *sPtr;
   char *sDataPtr;

   InitSmsInfo(msgInfo);
   sPtr = buffer_rec;

   nRcvHdrLen = (int)*sPtr;                
   memcpy((char *)&(msgInfo->sHeader), sPtr, nRcvHdrLen);
   
   if (msgInfo->sHeader.bHeaderFlag & 0x80) 
      nRcvHdrLen += 22;                
   sPtr += nRcvHdrLen;

   BcdToAscii(sPtr, szSmsMsgType, 4); 
   szSmsMsgType[4]='\0';	
   msgInfo->nMsgType = atoi(szSmsMsgType);
   sPtr += 2;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80)   && (iMapCtr < 3)   )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy (msgInfo->sData, sPtr, MAX_SMS_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetSmsFieldType(i) )
         {
         case SMS_FIX_BCD :                           
            nDigits = GetSmsFieldLength(i);
            nBytes = (nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_FIX_HEX :                           
            nDigits = GetSmsFieldLength(i);
            nBytes = (nDigits +1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_VAR_HEX :                           
            nDigits = (unsigned char)(*sPtr);
            nBytes = (nDigits + 1)/2 +1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_VAR_BCD :                           
            nDigits = (unsigned char)(*sPtr);
            nBytes = (nDigits +1)/2 +1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_VAR_SHEX :                           
            nDigits = (unsigned char)(*sPtr);
            nBytes = nDigits+1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_VAR_SBCD :                           
            nDigits = (unsigned char)(*sPtr);
            nBytes = nDigits+1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_FIX_ALPHA :                           
            nBytes = GetSmsFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_VAR_ALPHA:
            nBytes = (unsigned char)(*sPtr);
            nBytes++;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_FIX_BIN :                           
            nBytes = GetSmsFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case SMS_VAR_BIN    :                           
         case SMS_VAR_BITMAP :                           
            nBytes = (unsigned char)(*sPtr);
            nBytes++;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;
			
		case SMS_VAR2_BIN    :   /*IBO20190724 PROD00070403 Enh19.Q4 Art 9.1.3*/                        
			nBytes = ((unsigned char)(*sPtr)*256) + (unsigned char)(*(sPtr+1)) ;
			nBytes+= VAR2_LEN_LEN;
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
   for (i = iMapCtr*64; i< MAX_SMS_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
	  /*SKO290509*/ return(0);
}

/*Start MKB221208 CCD*/
int AnalyseSmsIc(char * buffer, int nLength, TSTagSms * tagInfo)
{
   char  *sPtr;
   int   offset;
   int   nTagLength, nBytes;
   int   nTagIndice;
   int   nLenTag;
   char  szTagName [ 5 ];


   InitSmsIcTag(tagInfo);

   offset = 0;
   tagInfo->nLength = nLength - 3;
   memcpy (tagInfo->sChipData,  buffer + 3,  nLength - 3);

   sPtr = buffer + 3;

	
   /* while (offset < nLength + 3)  * BURGAN ONSITE CERTIF AHA20171003 */  /* BURGAN_32 */
   while (offset < nLength -3 )
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


      nTagIndice = GetSmsTagIndice( szTagName);
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
/*End MKB221208 CCD*/
