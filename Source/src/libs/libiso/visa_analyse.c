#include <stdio.h>
#include <stdlib.h>
#include <iso8583.h>
#include <iso_visa.h>
#include <tools.h>

int   AnalyseVisa    (/*OBE121106*/unsigned char *buffer_rec, TSVisaInfo *msgInfo) 

{
   int  i, nDigits = 0;
   int  iMapCtr = 0;
   int  nBytes  = 0;
   int  nRcvHdrLen = 0;
   char sWork[256];
   char szVisaMsgType[5];	
   unsigned char *sPtr;/*OBE121106*/ 
   char *sDataPtr;

   InitVisaInfo(msgInfo);
   sPtr = buffer_rec;

   nRcvHdrLen = (int)*sPtr;                
   memcpy((char *)&(msgInfo->sHeader), sPtr, nRcvHdrLen);
   
   if (msgInfo->sHeader.bHeaderFlag & 0x80) 
      nRcvHdrLen += 22;                
   sPtr += nRcvHdrLen;

   BcdToAscii(sPtr, szVisaMsgType, 4); 
   szVisaMsgType[4]='\0';	
   msgInfo->nMsgType = atoi(szVisaMsgType);
   sPtr += 2;
   
   iMapCtr = 1;
   while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80)   && (iMapCtr < 3)   )
      ++iMapCtr;
   memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
   sPtr += (iMapCtr * 8);
  
   memcpy (msgInfo->sData, sPtr, MAX_VISA_DATA_LEN);

   for (i = 0; i < iMapCtr * 64; ++i)
   {
      if ( CheckBit( msgInfo->sBitMap, i))
      {
         switch ( GetVisaFieldType(i) )
         {
         case VISA_FIX_BCD :                           
            nDigits = GetVisaFieldLength(i);
            nBytes = (nDigits + 1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_FIX_HEX :                           
            nDigits = GetVisaFieldLength(i);
            nBytes = (nDigits +1)/2;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_VAR_HEX :                           
            nDigits = *sPtr;
            nBytes = (nDigits + 1)/2 +1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_VAR_BCD :                           
            nDigits = *sPtr;
            nBytes = (nDigits +1)/2 +1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_VAR_SBCD :                           
            nDigits = *sPtr;
            nBytes = nDigits+1; 
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_FIX_ALPHA :                           
            nBytes = GetVisaFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_VAR_ALPHA:                           
            nBytes = *sPtr;					
            nBytes++;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_FIX_BIN :                           
            nBytes = GetVisaFieldLength(i);
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;

         case VISA_VAR_BIN    :                           
         case VISA_VAR_BITMAP :                           
            nBytes = *sPtr;							
            nBytes++;
            msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
            break;
		case VISA_VAR2_BIN    :   /*IBO20190723 PROD00070403 Enh19.Q4 Art 9.1.3*/                        
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
   for (i = iMapCtr*64; i< MAX_VISA_FIELDS; i++)
      msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
	  
	  /*SKO290509*/ return(0);
}


/*Start MKB221208 CCD*/
int AnalyseVisaIc(char * buffer, int nLength, TSTagVisa * tagInfo)
{
   char  *sPtr;
   int   offset;
   int   nTagLength, nBytes;
   int   nTagIndice;
   int   nLenTag;
   char  szTagName [ 5 ];


   InitVisaIcTag(tagInfo);

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


      nTagIndice = GetVisaTagIndice( szTagName);
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
