#include <stdio.h>
#include <stdlib.h>

#include <iso_jcc.h>
#include <tools.h>
#include <define.h>


int GetJccField(int field_no, TSJccInfo * msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
   char szJccFldLen[4];	
   char szBcdFldLen[4];	

   memset( szJccFldLen, '\0', 4);	
   memset( szBcdFldLen, '\0', 4);	
   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      switch (GetJccFieldType(field_no))
      {
      case JCC_LL_BIN :
         length = *( msgInfo->sData + msgInfo->nFieldPos[field_no] );
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+1, length);
         *nLength = length;
         break;

      case JCC_LLL_BIN :
         length = *( msgInfo->sData + msgInfo->nFieldPos[field_no] ) *256 +
                  *( msgInfo->sData + msgInfo->nFieldPos[field_no] +1 );
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
         *nLength = length;
         break;

      case JCC_FIX_BIN   :
         length = GetJccFieldLength(field_no);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         data[length] = '\0';
         *nLength = length;
         break;
         
      case JCC_FIX_ALPHA :
         length = GetJccFieldLength(field_no);
         EbcdicToAscii( msgInfo->sData + msgInfo->nFieldPos[field_no], 
                        data, length);
         data[length] = '\0';
         *nLength = length;
         break;
         
      case JCC_FIX_BCD :                           
         length = GetJccFieldLength(field_no);
         nBytes = BcdToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                           data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case JCC_LL_BCD :                           
         length = *(  msgInfo->sData+msgInfo->nFieldPos[field_no] );
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                              data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case JCC_LLL_BCD :                           
         length = * ( msgInfo->sData+msgInfo->nFieldPos[field_no] ) * 256 +
                  * ( msgInfo->sData+msgInfo->nFieldPos[field_no] + 1);
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
                                                              data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case JCC_FIX_HEX :                           
         length = GetJccFieldLength(field_no);
         nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                           data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case JCC_LL_HEX :                           
         length = *( msgInfo->sData+msgInfo->nFieldPos[field_no] );
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                              data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case JCC_LLL_HEX :                           
         length = *( msgInfo->sData+msgInfo->nFieldPos[field_no])*256 +
                  *( msgInfo->sData+msgInfo->nFieldPos[field_no]+1);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
                                                              data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case JCC_LL_ALPHA : 
         length = * (  msgInfo->sData+msgInfo->nFieldPos[field_no] );
         EbcdicToAscii ( msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                         data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case JCC_LLL_ALPHA : 
         length = *( msgInfo->sData+msgInfo->nFieldPos[field_no] )*256 +
                  *( msgInfo->sData+msgInfo->nFieldPos[field_no] +1 );
         EbcdicToAscii ( msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
                  data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      default :
         return(ECHEC);
         break;
         
      }
      return(SUCCES);
   }
   return(ECHEC);
}

int AddJccField(int field_no, TSJccInfo *msgInfo, char *data, int nLength)
{
   int nBytes   = 0;
   int i = 0,nDigits  = 0;
   char szJccFldLen[4];	
   char szBcdFldLen[4];	

   memset( szJccFldLen, '\0', 4);	
   memset( szBcdFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
   switch (GetJccFieldType(field_no))
   {
   case JCC_FIX_ALPHA :
      if ( GetJccFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case JCC_LL_ALPHA :
      *(msgInfo->sData+msgInfo->nLength)  = nLength;
      msgInfo->nLength += 1;
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case JCC_LLL_ALPHA :
      *( msgInfo->sData+msgInfo->nLength ) = nLength / 256;
      *( msgInfo->sData+msgInfo->nLength +1) = nLength % 256;
      msgInfo->nLength += 2;
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case JCC_FIX_BCD :
      if ( GetJccFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToBcd(data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case JCC_LL_BCD :
      *(msgInfo->sData+msgInfo->nLength)  = nLength;
      msgInfo->nLength += 1;
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case JCC_LLL_BCD :
      *( msgInfo->sData+msgInfo->nLength ) = nLength / 256;
      *( msgInfo->sData+msgInfo->nLength +1) = nLength % 256;
      msgInfo->nLength += 2;
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case JCC_FIX_HEX :
      if ( GetJccFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case JCC_LL_HEX :
      *(msgInfo->sData+msgInfo->nLength)  = nLength;
      msgInfo->nLength += 1;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case JCC_LLL_HEX :
      *( msgInfo->sData+msgInfo->nLength ) = nLength / 256;
      *( msgInfo->sData+msgInfo->nLength +1) = nLength % 256;
      msgInfo->nLength += 2;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case JCC_FIX_BIN :
      if ( GetJccFieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case JCC_LL_BIN :
      *(msgInfo->sData+msgInfo->nLength)  = nLength;
      msgInfo->nLength += 1;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case JCC_LLL_BIN :
      *( msgInfo->sData+msgInfo->nLength ) = nLength / 256;
      *( msgInfo->sData+msgInfo->nLength +1) = nLength % 256;
      msgInfo->nLength += 2;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   msgInfo->nLength += nBytes;
   for (i=field_no; i< MAX_JCC_FIELDS; i++)
       msgInfo->nFieldPos[i+1] = msgInfo->nLength;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   return(SUCCES);
}

int InsertJccField(int field_no, TSJccInfo *msgInfo, char *data, int nLength)
{
   int  nBytes    = 0;
   int  nDigits   = 0;
   int  nBytes1   = 0;
   int  i;

   int  nOldFldLen,nOldLen;

   char szJccData [999];	
   char szWork [LG_MAX];	
   char szJccFldLen[4];	
   char szBcdFldLen[4];	

   memset( szJccFldLen, '\0', 4);	
   memset( szBcdFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   switch (GetJccFieldType(field_no))
   {
   case JCC_FIX_ALPHA :
      if ( GetJccFieldLength(field_no) != nLength)
         return(ECHEC);

      nBytes = nLength;
      AsciiToEbcdic (data, szJccData, nBytes);
      break;

   case JCC_LL_ALPHA :
      *szJccData = nLength;
      nBytes=1;
      AsciiToEbcdic( data, szJccData+1,        nLength);
      nBytes += nLength;
      break;

   case JCC_LLL_ALPHA :
      *szJccData = nLength/256;
      *(szJccData+1) = nLength%256;
      nBytes = 2;
      AsciiToEbcdic( data, szJccData+2,         nLength);
      nBytes += nLength;
      break;

   case JCC_FIX_BCD :
      if ( GetJccFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToBcd(data, szJccData, nLength);
      break;

   case JCC_LL_BCD :
      *szJccData = nLength;
      nBytes1 = 1;
      nBytes = AsciiToBcd( data, szJccData+1, nLength);
      nBytes += nBytes1;
      break;

   case JCC_LLL_BCD :
      *szJccData = nLength/256;
      *(szJccData+1) = nLength%256;
      nBytes1 = 2;
      nBytes = AsciiToBcd( data, szJccData+2, nLength);
      nBytes += nBytes1;
      break;

   case JCC_FIX_HEX :
      if ( GetJccFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, szJccData, nLength);
      break;

   case JCC_LL_HEX :
      *szJccData = nLength;
      nBytes1 = 1;
      nBytes = AsciiToHex( data, szJccData+1, nLength);
      nBytes += nBytes1;
      break;

   case JCC_LLL_HEX :
      *szJccData = nLength/256;
      *(szJccData+1) = nLength%256;
      nBytes1 = 2;
      nBytes = AsciiToHex( data, szJccData+2, nLength);
      nBytes += nBytes1;
      break;

   case JCC_FIX_BIN :
      if ( GetJccFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy( szJccData, data, nBytes);
      break;

   case JCC_LL_BIN :
      *szJccData = nLength;
      nBytes = 1;
      memcpy( szJccData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case JCC_LLL_BIN :
      *szJccData = nLength/256;
      *(szJccData+1) = nLength%256;
      nBytes = 2;
      memcpy( szJccData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
      nOldFldLen = 0;
      memset(szWork, 0, sizeof(szWork));
      nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
      memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

      for (i = MAX_JCC_FIELDS-1; i > field_no; i--)
      {
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szJccData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
/*
   for (i = MAX_JCC_FIELDS-1; i > field_no; i--)
   {
	  memset(szWork, 0, sizeof(szWork));
      memcpy (szWork,
              msgInfo->sData+msgInfo->nFieldPos[i],
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
              szWork,
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      msgInfo->nFieldPos[i+1] += nBytes;
   }
   memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szJccData, nBytes);
   msgInfo->nFieldPos[field_no+1] += nBytes;
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
*/
   return(SUCCES);
}

int PutJccField(int field_no, TSJccInfo *msgInfo, char *data, int nLength)
{
   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i, nOldFldLen,nOldLen;
   char szJccData [999];	
   char szJccFldLen[4];	
   char szWork    [LG_MAX];


   memset( szJccFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      switch (GetJccFieldType(field_no))
      {
      case JCC_FIX_ALPHA :
         if ( GetJccFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         AsciiToEbcdic (data, szJccData, nBytes);
         break;

      case JCC_LL_ALPHA :
         *szJccData = nLength;
         nBytes=1;
         AsciiToEbcdic( data, szJccData+1,        nLength);
         nBytes += nLength;
         break;

      case JCC_LLL_ALPHA :
         *szJccData = nLength/256;
         *(szJccData +1)= nLength%256;
         nBytes = 2;
         AsciiToEbcdic( data, szJccData+2,        nLength);
         nBytes += nLength;
         break;

      case JCC_FIX_BCD :
         if ( GetJccFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToBcd(data, szJccData, nLength);
         break;

      case JCC_LL_BCD :
         *szJccData = nLength;
         nBytes1 = 1;
         nBytes = AsciiToBcd( data, szJccData+1, nLength);
         nBytes += nBytes1;
         break;

      case JCC_LLL_BCD :
         *szJccData = nLength/256;
         *(szJccData +1)= nLength%256;
         nBytes1 = 2;
         nBytes = AsciiToBcd( data, szJccData+2, nLength);
         nBytes += nBytes1;
         break;

      case JCC_FIX_HEX :
         if ( GetJccFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToHex(data, szJccData, nLength);
         break;

      case JCC_LL_HEX :
         *szJccData = nLength;
         nBytes1 = 1;
         nBytes = AsciiToHex( data, szJccData+1, nLength);
         nBytes += nBytes1;
         break;

      case JCC_LLL_HEX :
         *szJccData = nLength/256;
         *(szJccData +1)= nLength%256;
         nBytes1 = 2;
         nBytes = AsciiToHex( data, szJccData+2, nLength);
         nBytes += nBytes1;
         break;

      case JCC_FIX_BIN :
         if ( GetJccFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy( szJccData, data, nBytes);
         break;

      case JCC_LL_BIN :
         *szJccData = nLength;
         nBytes = 1;
         memcpy( szJccData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case JCC_LLL_BIN :
         *szJccData = nLength/256;
         *(szJccData +1)= nLength%256;
         nBytes = 2;
         memcpy( szJccData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      default :
         return(ECHEC);
      }
/*
      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      for (i = MAX_JCC_FIELDS-1; i > field_no; i--)
      {
         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szJccData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return(SUCCES);
*/
      nOldFldLen = msgInfo->nFieldPos[field_no +1] -
                   msgInfo->nFieldPos[field_no];
      memset(szWork, 0, sizeof(szWork));
      nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
      memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

      for (i = MAX_JCC_FIELDS-1; i > field_no; i--)
      {
/*         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
*/
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szJccData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return(SUCCES);
   }
   else
      return ( InsertJccField (field_no, msgInfo, data, nLength));

}

int JccBuildMsg (char * buffer_snd, TSJccInfo *msgInfo)
{
   char szMsgType[5];
   int  nBytes;
   int  iMapCtr;

   sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
   AsciiToBcd( szMsgType, buffer_snd,  4);

   iMapCtr = 1;
   while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
   memcpy( buffer_snd+2, msgInfo->sBitMap, iMapCtr*8);
   memcpy( buffer_snd+2 + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
   return( 2 + (iMapCtr*8) + msgInfo->nLength);
}

/****************************************************************************/
/*** Gestion du Champs 55 (EMV)                                            **/
/****************************************************************************/
int GetJccIcTag (char  *tag_name, TSTagJcc *tagInfo, char *data, int *length)
{
   int nIndice ;
   int nLenTag ;
   int nDigits;

   if ( (nIndice = GetJccTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]) & 0x1F) == 0x1F) /*YK24022006*/
      nLenTag = 2;
   else
      nLenTag = 1;
   /* *length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag + 1));*/
   
   *length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag));/*YK24022006*/
   
   switch (GetJccTagType(nIndice) )
   {
   case JCC_TAG_FIX_BIN :
   case JCC_TAG_FIX_ALPHA :
      memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
                   *length);
      break;

   case JCC_TAG_VAR_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;

   case JCC_TAG_FIX_BCD :
      nDigits = BcdToAscii( data, 
                            tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            (*length)*2);
      *length = (*length)*2;
      break;

   case JCC_TAG_VAR_BIN :
   case JCC_TAG_VAR_ALPHA :
      memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
                    *length);
      break;
   /*YK220206:EMV*/
   case JCC_TAG_VAR_HEX :
      nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;
   /*YK220206:EMV*/
   }
   return(SUCCES);
}

int AddJccIcTag (char  *tag_name, TSTagJcc *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;

   if ( (nIndice = GetJccTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);
   if ( (*(tagInfo->sChipData + tagInfo->nLength) & 0x1F) == 0x1F)/*YK24022006*/
      nLenTag = 2;
   else 
      nLenTag = 1;
   switch (GetJccTagType(nIndice) )
   {
   case JCC_TAG_FIX_BIN :
   case JCC_TAG_FIX_ALPHA :
      if ( GetJccTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case JCC_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case JCC_TAG_FIX_BCD :
      if ( GetJccTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case JCC_TAG_VAR_BIN :
   case JCC_TAG_VAR_ALPHA :
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   /*YK220206:EMV*/
   case JCC_TAG_VAR_HEX :
      nDigits = AsciiToHex( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;
   /*YK220206:EMV*/

   }

   *(tagInfo->sChipData + tagInfo->nLength + nLenTag)    = nDigits;
   tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
   tagInfo->nPresent[ nIndice ] = PRESENT;
   tagInfo->nLength +=nDigits+nLenTag + 1;
   return(SUCCES);
}

int PutJccIcTag (char  *tag_name, TSTagJcc *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   int nPutJcc, nTagPresent = NOT_PRESENT;
   char szLastPart  [ MAX_JCC_CHIP_LEN ];
   int nOldTagLen, i;

   if ( (nIndice = GetJccTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == PRESENT)
   {
      nPutJcc = tagInfo->nPosTag [nIndice];
      nTagPresent = PRESENT;
   }
   else
   {
      nPutJcc = tagInfo->nLength;
   }
   
   if ( nTagPresent != PRESENT )
      AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

      if ( (*(tagInfo->sChipData + nPutJcc) & 0x1F) == 0x1F)/*YK24022006*/
      nLenTag = 2;
   else 
      nLenTag = 1;

   if ( nTagPresent == PRESENT )
   {
      nOldTagLen = * ( tagInfo->sChipData + nPutJcc + nLenTag );
      memcpy (szLastPart, 
              tagInfo->sChipData + nPutJcc + nLenTag + nOldTagLen +1,
              tagInfo->nLength - nPutJcc -nLenTag - nOldTagLen -1);
   }

   switch (GetJccTagType(nIndice) )
   {
   case JCC_TAG_FIX_BIN :
   case JCC_TAG_FIX_ALPHA :
      if ( GetJccTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + nPutJcc +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case JCC_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + nPutJcc + nLenTag+1, 
                            length);
      break;

   case JCC_TAG_FIX_BCD :
      if ( GetJccTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + nPutJcc + nLenTag+1, 
                            length);
      break;

   case JCC_TAG_VAR_BIN :
   case JCC_TAG_VAR_ALPHA :
      memcpy( tagInfo->sChipData + nPutJcc +nLenTag + 1, data, length);
      nDigits = length;
      break;
   /*YK220206:EMV*/   
   case JCC_TAG_VAR_HEX :
      nDigits = AsciiToHex( data, 
                            tagInfo->sChipData + nPutJcc + nLenTag+1, 
                            length);
                            
      /*
      sprintf(sLine, "tag_name(%s)nLenTag(%d)nDigits(%d)tagInfo->nLength(%d)",tag_name,nLenTag,nDigits,tagInfo->nLength);
      trace_event(sLine, PROCESSING);
      */
      break;
  /*YK220206:EMV*/
   }

   *(tagInfo->sChipData + nPutJcc + nLenTag)    = nDigits;
   if ( nTagPresent != PRESENT )
   {
      tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
      tagInfo->nPresent[ nIndice ] = PRESENT;
      tagInfo->nLength +=nDigits+nLenTag + 1;
   }
   else
   {
      memcpy (tagInfo->sChipData + nPutJcc + nLenTag + nDigits +1,
              szLastPart, 
              tagInfo->nLength - nPutJcc -nLenTag - nOldTagLen -1);
      tagInfo->nLength +=nDigits - nOldTagLen;
      for (i = nIndice + 1; i < MAX_JCC_CHIP_TAG; i++)
      {
         if (tagInfo->nPresent[i] == PRESENT)
            tagInfo->nPosTag[i] += nDigits - nOldTagLen;
      }
   }
   return(SUCCES);
}

int JccBuildIcFld (char * buffer_snd, TSTagJcc *tagInfo)
{
   memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
   return(tagInfo->nLength);
}

/* --- BEGIN LHI 14-12-2007 --- */

/* CHAMP 48 */

int PutJccPds (char  *pds_name, TSPdsJcc *pdsInfo, char *data, int length)
{
   int nIndice;
   int nPosLen; /* Position de la longueur */
   int nPosPds; /* Position du PDS         */
   int nPdsLen; /* Longueur du PDS et de la longueur */
   int nPdsLength;
   char szAscPdsLen [4];
   char szOldPdsLen [4];
   char szLastPart  [ 999 ];
   int nPdsType, i;
   int nPutPos, 
       nOldPdsLen, 
       nPdsPresent = NOT_PRESENT;

   memset (szAscPdsLen, '\0', 4);
   memset (szOldPdsLen, '\0', 4);
   memset (szLastPart, '\0', sizeof (szLastPart));

      nPosLen = 0;
      nPosPds = 2;
      nPdsLen = 2;
      if ( (nIndice = GetJccPdsIndice(pds_name)) == -1)
         return(ECHEC);
      nPdsType    = GetJccPdsType(nIndice) ;
      nPdsLength  = GetJccPdsLength(nIndice); 
      sprintf ( szAscPdsLen, "%.2d", length+2);
 
   
   if (pdsInfo->nPresent[nIndice] == PRESENT)
   {
      nPutPos = pdsInfo->nPosPds[nIndice];
      memcpy (szOldPdsLen,
              pdsInfo->sPdsData + pdsInfo->nPosPds [nIndice] + nPosPds,
              nPdsLen);
      nOldPdsLen = atoi(szOldPdsLen) ;
      memcpy(szLastPart, 
             pdsInfo->sPdsData + nPutPos + nPosPds + nPdsLen + nOldPdsLen,
             pdsInfo->nLength - nPutPos - nPosPds - nPdsLen - nOldPdsLen);
   }
   else
      nPutPos = pdsInfo->nLength;
   

   sprintf(szAscPdsLen, "%02d", length);
   memcpy(pdsInfo->sPdsData + nPutPos, pds_name, nPosPds);
   memcpy(pdsInfo->sPdsData + nPutPos + nPosPds , szAscPdsLen, nPdsLen);
   memcpy(pdsInfo->sPdsData + nPutPos + nPosPds + nPdsLen, data, length);


   if (nPdsPresent == NOT_PRESENT )
   {
      pdsInfo->nPosPds [ nIndice ] = pdsInfo->nLength;
      pdsInfo->nPresent[ nIndice ] = PRESENT;
      pdsInfo->nLength +=length + nPdsLen + nPdsLen;
   }
   else
   {
      memcpy (pdsInfo->sPdsData + nPutPos+ nPosPds+ nPdsLen + length,
              szLastPart,
              pdsInfo->nLength - nPutPos - nPosLen - nPdsLen - nOldPdsLen);

      pdsInfo->nLength +=length - nOldPdsLen;
      for ( i = nIndice+1; i < MAX_JCC_PDS; i++)
      {
         if ( pdsInfo->nPresent[i] == PRESENT)
            pdsInfo->nPosPds[i] += length - nOldPdsLen;
      }
   }
   return(SUCCES);
}
int JccBuildPds (char * sPdsBuffer, TSPdsJcc *pdsInfo)
{
    int i;
      fprintf ( stderr,"\n Data Avant le Build : ");
   for ( i = 0 ; i < pdsInfo->nLength ; i ++ )
      fprintf ( stderr,"%02X ", pdsInfo->sPdsData[i] );

   memcpy ( sPdsBuffer , pdsInfo->sPdsData , pdsInfo->nLength);

   return(pdsInfo->nLength );

}

/* BEGIN TBO01082014 */
int GetJccPds (char  *pds_name, TSPdsJcc *pdsInfo , char *data, int *length)
{
   int nIndice;
   int nPdsLength;
   char szAscPdsLen [4];
   int nPdsType;

   memset (szAscPdsLen, '\0', 4);

   if ( (nIndice = GetJccPdsIndice(pds_name)) == -1)
      return(ECHEC);
   nPdsType    = GetJccPdsType(nIndice) ;
   nPdsLength  = GetJccPdsLength(nIndice);

   if (pdsInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);

   memcpy ( szAscPdsLen,
            pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] + 2,
            2);
   *length = atoi(szAscPdsLen);

   switch ( nPdsType )
   {
   case JCC_PDS_FIX_ALPHA :
   case JCC_PDS_VAR_ALPHA :
      memcpy( data,
              pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] + 2 + 2,
              *length);
      break;

   }
   return(SUCCES);
}
/* End TBO01082014 */