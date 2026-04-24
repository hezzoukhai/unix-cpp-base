#include <stdio.h>
#include <stdlib.h>

#include <iso_euro.h>
#include <tools.h>
#include <define.h>



int GetEuroField(int field_no, TSEuroInfo * msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
   int nFldType, nFldLength;
   char szEuroFldLen[4];	
   char szAscFldLen[4];	

   memset( szEuroFldLen, '\0', 4);	
   memset( szAscFldLen, '\0', 4);	
   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      if  ( (msgInfo->nMsgType == 1304) ||
            (msgInfo->nMsgType == 1314) )
      {
         nFldType   =  GetEuro93FieldType  (field_no);
         nFldLength =  GetEuro93FieldLength(field_no);
      }
      else
      {
         nFldType   =  GetEuroFieldType  (field_no);
         nFldLength =  GetEuroFieldLength(field_no);
      }

      switch (nFldType)
      {
      case EURO_LL_BIN :
         memcpy(szEuroFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
         EbcdicToAscii(szEuroFldLen, szAscFldLen, 2);
         length = atoi(szAscFldLen);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
         *nLength = length;
         break;

      case EURO_LLL_BIN :
         memcpy(szEuroFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
         EbcdicToAscii(szEuroFldLen, szAscFldLen, 3);
         length = atoi(szAscFldLen);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
         *nLength = length;
         break;

      case EURO_FIX_BIN   :
         length = nFldLength;
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         data[length] = '\0';
         *nLength = length;
         break;
         
      case EURO_FIX_ALPHA :
         length = nFldLength;
         EbcdicToAscii( msgInfo->sData + msgInfo->nFieldPos[field_no], 
                        data, length);
         data[length] = '\0';
         *nLength = length;
         break;
         
      case EURO_FIX_HEX :                           
         length = nFldLength;
         nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                           data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case EURO_LL_HEX :                           
         memcpy( szEuroFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         EbcdicToAscii(szEuroFldLen, szAscFldLen, 2);
         length = atoi(szAscFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
                                                              data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case EURO_LLL_HEX :                           
         memcpy ( szEuroFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         EbcdicToAscii(szEuroFldLen, szAscFldLen, 3);
         length = atoi(szAscFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
                                                              data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case EURO_LL_ALPHA : 
         memcpy ( szEuroFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         EbcdicToAscii(szEuroFldLen, szAscFldLen, 2);
         length = atoi(szAscFldLen);
         EbcdicToAscii ( msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
                                                                 data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case EURO_LLL_ALPHA : 
         memcpy ( szEuroFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         EbcdicToAscii(szEuroFldLen, szAscFldLen, 3);
         length = atoi(szAscFldLen);
         EbcdicToAscii ( msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
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

int AddEuroField(int field_no, TSEuroInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int nFldType, nFldLength;
   char szEuroFldLen[4];	
   char szEbcFldLen[4];	

   memset( szEuroFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
   if  ( (msgInfo->nMsgType == 1304) ||
         (msgInfo->nMsgType == 1314) )
   {
      nFldType   =  GetEuro93FieldType  (field_no);
      nFldLength =  GetEuro93FieldLength(field_no);
   }
   else
   {
      nFldType   =  GetEuroFieldType  (field_no);
      nFldLength =  GetEuroFieldLength(field_no);
   }

   switch (nFldType)
   {
   case EURO_FIX_ALPHA :
      if ( nFldLength != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case EURO_LL_ALPHA :
      sprintf ( szEuroFldLen, "%.2d", nLength);	
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 2);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
      msgInfo->nLength += 2;
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case EURO_LLL_ALPHA :
      sprintf ( szEuroFldLen, "%.3d", nLength);	
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 3);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
      msgInfo->nLength += 3;
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case EURO_FIX_HEX :
      if ( nFldLength != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case EURO_LL_HEX :
      sprintf ( szEuroFldLen, "%.2d", nLength);	
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 2);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
      msgInfo->nLength += 2;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case EURO_LLL_HEX :
      sprintf ( szEuroFldLen, "%.3d", nLength);	
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 3);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
      msgInfo->nLength += 3;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case EURO_FIX_BIN :
      if ( nFldLength != nLength)
         return(ECHEC);
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case EURO_LL_BIN :
      sprintf ( szEuroFldLen, "%.2d", nLength);	
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 2);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
      msgInfo->nLength += 2;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case EURO_LLL_BIN :
      sprintf ( szEuroFldLen, "%.3d", nLength);	
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 3);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
      msgInfo->nLength += 3;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   return(SUCCES);
}

int InsertEuroField(int field_no, TSEuroInfo *msgInfo, char *data, int nLength)
{
   int  nFldType, nFldLength;
   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i;
   char szEuroData [999];	
   char szEuroFldLen[4];	
   char szEbcFldLen[4];	

   memset( szEuroFldLen, '\0', 4);	
   memset( szEbcFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   if  ( (msgInfo->nMsgType == 1304) ||
         (msgInfo->nMsgType == 1314) )
   {
      nFldType   =  GetEuro93FieldType  (field_no);
      nFldLength =  GetEuro93FieldLength(field_no);
   }
   else
   {
      nFldType   =  GetEuroFieldType  (field_no);
      nFldLength =  GetEuroFieldLength(field_no);
   }

   switch (nFldType)
   {
   case EURO_FIX_ALPHA :
      if ( nFldLength != nLength)
         return(ECHEC);
      nBytes = nLength;
      AsciiToEbcdic (data, szEuroData, nBytes);
      break;

   case EURO_LL_ALPHA :
      sprintf ( szEuroFldLen, "%.2d", nLength);	
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 2);
      memcpy( szEuroData,        szEbcFldLen, 2);
      nBytes=2;
      AsciiToEbcdic( data,        szEuroData+2, nLength);
      nBytes += nLength;
      break;

   case EURO_LLL_ALPHA :
      sprintf ( szEuroFldLen, "%.3d", nLength);	
      nBytes = 3;
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 3);
      memcpy( szEuroData, szEbcFldLen, 3);
      AsciiToEbcdic( data,        szEuroData+3, nLength);
      nBytes += nLength;
      break;

   case EURO_FIX_HEX :
      if ( nFldLength != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, szEuroData, nLength);
      break;

   case EURO_LL_HEX :
      sprintf ( szEuroFldLen, "%.2d", nLength);	
      nBytes1 = 2;
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 2);
      memcpy( szEuroData, szEbcFldLen, nBytes1);
      nBytes = AsciiToHex( data, szEuroData+2, nLength);
      nBytes += nBytes1;
      break;

   case EURO_LLL_HEX :
      sprintf ( szEuroFldLen, "%.3d", nLength);	
      nBytes1 = 3;
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 3);
      memcpy( szEuroData, szEbcFldLen, 3);
      nBytes = AsciiToHex( data, szEuroData+3, nLength);
      nBytes += nBytes1;
      break;

   case EURO_FIX_BIN :
      if ( nFldLength != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy( szEuroData, data, nBytes);
      break;

   case EURO_LL_BIN :
      sprintf ( szEuroFldLen, "%.2d", nLength);	
      nBytes = 2;
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 2);
      memcpy( szEuroData, szEbcFldLen, nBytes);
      memcpy( szEuroData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case EURO_LLL_BIN :
      sprintf ( szEuroFldLen, "%.3d", nLength);	
      nBytes = 3;
      AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 3);
      memcpy( szEuroData, szEbcFldLen, nBytes);
      memcpy( szEuroData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
   for (i = MAX_EURO_FIELDS-1; i > field_no; i--)
   {
      memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
              msgInfo->sData+msgInfo->nFieldPos[i],
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      msgInfo->nFieldPos[i+1] += nBytes;
   }
   memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szEuroData, nBytes);
   msgInfo->nFieldPos[field_no+1] += nBytes;
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   return(SUCCES);
}

int PutEuroField(int field_no, TSEuroInfo *msgInfo, char *data, int nLength)
{
   int  nFldType, nFldLength;
   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i, nOldFldLen;
   char szEuroData [999];	
   char szEuroFldLen[4];	
   char szEbcFldLen[4];	

   memset( szEuroFldLen, '\0', 4);	
   memset( szEbcFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      if  ( (msgInfo->nMsgType == 1304) ||
            (msgInfo->nMsgType == 1314) )
      {
         nFldType   =  GetEuro93FieldType  (field_no);
         nFldLength =  GetEuro93FieldLength(field_no);
      }
      else
      {
         nFldType   =  GetEuroFieldType  (field_no);
         nFldLength =  GetEuroFieldLength(field_no);
      }

      switch (nFldType)
      {
      case EURO_FIX_ALPHA :
         if ( nFldLength != nLength)
            return(ECHEC);
         nBytes = nLength;
         AsciiToEbcdic (data, szEuroData, nBytes);
         break;

      case EURO_LL_ALPHA :
         sprintf ( szEuroFldLen, "%.2d", nLength);	
         AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 2);
         memcpy( szEuroData,        szEbcFldLen, 2);
         nBytes=2;
         AsciiToEbcdic( data,        szEuroData+2, nLength);
         nBytes += nLength;
         break;

      case EURO_LLL_ALPHA :
         sprintf ( szEuroFldLen, "%.3d", nLength);	
         nBytes = 3;
         AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 3);
         memcpy( szEuroData, szEbcFldLen, 3);
         AsciiToEbcdic( data,        szEuroData+3, nLength);
         nBytes += nLength;
         break;

      case EURO_FIX_HEX :
         if ( nFldLength != nLength)
            return(ECHEC);
         nBytes = AsciiToHex(data, szEuroData, nLength);
         break;

      case EURO_LL_HEX :
         sprintf ( szEuroFldLen, "%.2d", nLength);	
         nBytes1 = 2;
         AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 2);
         memcpy( szEuroData, szEbcFldLen, nBytes1);
         nBytes = AsciiToHex( data, szEuroData+2, nLength);
         nBytes += nBytes1;
         break;

      case EURO_LLL_HEX :
         sprintf ( szEuroFldLen, "%.3d", nLength);	
         nBytes1 = 3;
         AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 3);
         memcpy( szEuroData, szEbcFldLen, 3);
         nBytes = AsciiToHex( data, szEuroData+3, nLength);
         nBytes += nBytes1;
         break;

      case EURO_FIX_BIN :
         if ( nFldLength != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy( szEuroData, data, nBytes);
         break;

      case EURO_LL_BIN :
         sprintf ( szEuroFldLen, "%.2d", nLength);	
         nBytes = 2;
         AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 2);
         memcpy( szEuroData, szEbcFldLen, nBytes);
         memcpy( szEuroData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case EURO_LLL_BIN :
         sprintf ( szEuroFldLen, "%.3d", nLength);	
         nBytes = 3;
         AsciiToEbcdic ( szEuroFldLen, szEbcFldLen, 3);
         memcpy( szEuroData, szEbcFldLen, nBytes);
         memcpy( szEuroData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      default :
         return(ECHEC);
      }
      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      for (i = MAX_EURO_FIELDS-1; i > field_no; i--)
      {
         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szEuroData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return(SUCCES);
   }
   else
      return (InsertEuroField(field_no, msgInfo, data, nLength));
}

int EuroBuildMsg (char * buffer_snd, TSEuroInfo *msgInfo)
{
   char szMsgType[5];
   int  nBytes;
   int  iMapCtr;

   sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
   AsciiToEbcdic( szMsgType , buffer_snd , 4);

   iMapCtr = 1;
   while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
   memcpy( buffer_snd+4, msgInfo->sBitMap, iMapCtr*8);
   memcpy( buffer_snd+4 + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
   return( 4 + (iMapCtr*8) + msgInfo->nLength);
}

/****************************************************************************/
/* La longueur d'un TAG dans le champ 55 est toujours code sur un caractere */
/****************************************************************************/
int GetEuroIcTag (char  *tag_name, TSTagEuro *tagInfo, char *data, int *length)
{
   int nIndice ;
   int nLenTag ;
   int nDigits;
   char  sLine[MAX_LINE_TRC];

   if ( (nIndice = GetEuroTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]) & 0x1F) == 0x1F)
      nLenTag = 2;
   else
      nLenTag = 1;
   *length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag ));


   switch (GetEuroTagType(nIndice) )
   {
   case EURO_TAG_FIX_BIN :
   case EURO_TAG_FIX_ALPHA :
      memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
                   *length);
      break;

   case EURO_TAG_VAR_HEX :
      nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;
   case EURO_TAG_VAR_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;

   case EURO_TAG_FIX_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
							data,
                            (*length)*2);
      *length = (*length)*2;
      break;

   case EURO_TAG_VAR_BIN :
   case EURO_TAG_VAR_ALPHA :
      memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
                    *length);
      break;

   }
   return(SUCCES);
}

int AddEuroIcTag (char  *tag_name, TSTagEuro *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   char sLine[MAX_LINE_TRC];

   if ( (nIndice = GetEuroTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

   if ( (*(tagInfo->sChipData + tagInfo->nLength) & 0x1F ) == 0x1F)
      nLenTag = 2;
   else 
      nLenTag = 1;

   sprintf(sLine,"TAG IS %s|%02X|%d",tag_name ,*(tagInfo->sChipData + tagInfo->nLength), nLenTag);
   trace_event(sLine,TRACE);
   switch (GetEuroTagType(nIndice) )
   {
   case EURO_TAG_FIX_BIN :
   case EURO_TAG_FIX_ALPHA :
      if ( GetEuroTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case EURO_TAG_VAR_HEX :
      nDigits = AsciiToHex( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case EURO_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case EURO_TAG_FIX_BCD :
      if ( GetEuroTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case EURO_TAG_VAR_BIN :
   case EURO_TAG_VAR_ALPHA :
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   }

   *(tagInfo->sChipData + tagInfo->nLength + nLenTag)    = nDigits;
   tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
   tagInfo->nPresent[ nIndice ] = PRESENT;
   tagInfo->nLength +=nDigits+nLenTag + 1;
   return(SUCCES);
}

int PutEuroIcTag (char  *tag_name, TSTagEuro *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   int nPutPos, nTagPresent = NOT_PRESENT;
   char szLastPart  [ MAX_EURO_CHIP_LEN ];
   int nOldTagLen, i;

   memset (szLastPart, '\0', sizeof(szLastPart));
   if ( (nIndice = GetEuroTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == PRESENT)
   {
      nPutPos = tagInfo->nPosTag [nIndice];
      nTagPresent = PRESENT;
   }
   else
   {
      nPutPos = tagInfo->nLength;
   }
   
   if ( nTagPresent != PRESENT )
      AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

   if ( (*(tagInfo->sChipData + nPutPos)) & 0x1F == 0x1F)
      nLenTag = 2;
   else 
      nLenTag = 1;

   if ( nTagPresent == PRESENT )
   {
      nOldTagLen = * ( tagInfo->sChipData + nPutPos + nLenTag );
      memcpy (szLastPart, 
              tagInfo->sChipData + nPutPos + nLenTag + nOldTagLen +1,
              tagInfo->nLength - nPutPos -nLenTag - nOldTagLen -1);
   }

   switch (GetEuroTagType(nIndice) )
   {
   case EURO_TAG_FIX_BIN :
   case EURO_TAG_FIX_ALPHA :
      if ( GetEuroTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case EURO_TAG_VAR_HEX :
      nDigits = AsciiToHex( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case EURO_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case EURO_TAG_FIX_BCD :
      if ( GetEuroTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case EURO_TAG_VAR_BIN :
   case EURO_TAG_VAR_ALPHA :
      memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
      nDigits = length;
      break;

   }

   *(tagInfo->sChipData + nPutPos + nLenTag)    = nDigits;
   if ( nTagPresent != PRESENT )
   {
      tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
      tagInfo->nPresent[ nIndice ] = PRESENT;
      tagInfo->nLength +=nDigits+nLenTag + 1;
   }
   else
   {
      memcpy (tagInfo->sChipData + nPutPos + nLenTag + nDigits +1,
              szLastPart, 
              tagInfo->nLength - nPutPos -nLenTag - nOldTagLen -1);
      tagInfo->nLength +=nDigits - nOldTagLen;
      for (i = nIndice + 1; i < MAX_EURO_CHIP_TAG; i++)
      {
         if (tagInfo->nPresent[i] == PRESENT)
            tagInfo->nPosTag[i] += nDigits - nOldTagLen;
      }
   }
   return(SUCCES);
}

int EuroBuildIcFld (char * buffer_snd, TSTagEuro *tagInfo)
{
   memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
   return(tagInfo->nLength);
}

/****************************************************************************/
/* Le champ 48                                                              */
/****************************************************************************/
int GetEuroPds (char  *pds_name, TSPdsEuro *pdsInfo, char *data, int *length)
{
   int nIndice;
   int nPosLen; /* Position de la longueur */
   int nPosPds; /* Position du PDS         */
   int nPdsLen; /* Longueur du PDS et de la longueur */
   int nPdsLength;
   int nPdsType;
   char szAscPdsLen [4];
   char szEbcPdsLen [4];
   int i;

   memset (szAscPdsLen, '\0', 4);

   if ( (pdsInfo->nMsgType == 1304) ||
        (pdsInfo->nMsgType == 1314) )
   {
      nPosLen = 3;
      nPosPds = 0;
      nPdsLen = 3;
      if ( (nIndice = GetEuroPds93Indice(pds_name)) == -1)
         return(ECHEC);
      nPdsType    = GetEuroPds93Type(nIndice) ;
      nPdsLength  = GetEuroPds93Length(nIndice); 
      /* EBE140523 sprintf ( szAscPdsLen, "%.3d", (int)length);*/
	  sprintf ( szAscPdsLen, "%.3d", (int)(*length));
   }
   else
   {
      nPosLen = 0;
      nPosPds = 2;
      nPdsLen = 2;
      if ( (nIndice = GetEuroPdsIndice(pds_name)) == -1)
         return(ECHEC);
      nPdsType    = GetEuroPdsType(nIndice) ;
      nPdsLength  = GetEuroPdsLength(nIndice); 
      /* EBE140523 sprintf ( szAscPdsLen, "%.2d", (int)(length+2));*/
	  sprintf ( szAscPdsLen, "%.2d", (int)((*length)+2));
   }

   if (pdsInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
  
   memset ( szAscPdsLen , 0, sizeof ( szAscPdsLen ));
   EbcdicToAscii ( pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice]+nPosLen,
                   szAscPdsLen, 
                   nPdsLen);
   *length = atoi(szAscPdsLen) - nPosPds;

/****
   for ( i = 0 ; i < *length ; i++ )
       data[i] = ( pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice]
                                       + nPdsLen + nPdsLen )[i];
****/



   switch ( nPdsType )
   {
   case EURO_PDS_FIX_ALPHA :
      EbcdicToAscii(  pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] + 
                      nPdsLen + nPdsLen, 
                      data , 
                      *length);
      break;

   case EURO_PDS_VAR_ALPHA :
      EbcdicToAscii( pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] +
                     nPdsLen + nPdsLen, 
                     data ,
                     *length);
      break;

   }

   return(SUCCES);
}

int AddEuroPds (char  *pds_name, TSPdsEuro *pdsInfo, char *data, int length)
{
   int i;
   int nIndice;
   int nPosLen; /* Position de la longueur */
   int nPosPds; /* Position du PDS         */
   int nPdsLen; /* Longueur du PDS et de la longueur */
   int nPdsLength;
   char szAscPdsLen [4];
   int nPdsType;

   memset (szAscPdsLen, '\0', 4);

   if ( (pdsInfo->nMsgType == 1304) ||
        (pdsInfo->nMsgType == 1314) )
   {
      nPosLen = 3;
      nPosPds = 0;
      nPdsLen = 3;
      if ( (nIndice = GetEuroPds93Indice(pds_name)) == -1)
         return(ECHEC);
      nPdsType    = GetEuroPds93Type(nIndice) ;
      nPdsLength  = GetEuroPds93Length(nIndice); 
      sprintf ( szAscPdsLen, "%.3d", length);
   }
   else
   {
      nPosLen = 0;
      nPosPds = 2;
      nPdsLen = 2;
      if ( (nIndice = GetEuroPdsIndice(pds_name)) == -1)
         return(ECHEC);
      nPdsType    = GetEuroPdsType(nIndice) ;
      nPdsLength  = GetEuroPdsLength(nIndice); 
      sprintf ( szAscPdsLen, "%.2d", length+2);
   }

   if (pdsInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
  
/****
   for ( i = 0 ; i < length ; i ++ ) 
       (pdsInfo->sPdsData + pdsInfo->nLength +(2*nPdsLen))[i] = data[i];
***/

   switch ( nPdsType )
   {
   case EURO_PDS_FIX_ALPHA :
      if ( nPdsLength != length)
         return(ECHEC);

      AsciiToEbcdic( data, 
                     pdsInfo->sPdsData + pdsInfo->nLength +(2*nPdsLen), 
                     length);
      break;

   case EURO_PDS_VAR_ALPHA :
      AsciiToEbcdic( data, 
                     pdsInfo->sPdsData + pdsInfo->nLength +(2*nPdsLen), 
                     length);
      break;

   }
   AsciiToEbcdic ( szAscPdsLen,
                   pdsInfo->sPdsData + pdsInfo->nLength + nPosLen,
                   nPdsLen 
                 );

   AsciiToEbcdic ( pds_name,
                   pdsInfo->sPdsData + pdsInfo->nLength + nPosPds,
                   nPdsLen 
                 );


   pdsInfo->nPosPds [ nIndice ] = pdsInfo->nLength;
   pdsInfo->nPresent[ nIndice ] = PRESENT;
   pdsInfo->nLength +=length + nPdsLen + nPdsLen;
   return(SUCCES);
}

int PutEuroPds (char  *pds_name, TSPdsEuro *pdsInfo, char *data, int length)
{
   int nIndice;
   int nPosLen; /* Position de la longueur */
   int nPosPds; /* Position du PDS         */
   int nPdsLen; /* Longueur du PDS et de la longueur */
   int nPdsLength;
   char szAscPdsLen [4];
   char szOldPdsLen [4];
   char szLastPart  [ MAX_EURO_PDS_LEN ];
   int nPdsType, i;
   int nPutPos, 
       nOldPdsLen, 
       nPdsPresent = NOT_PRESENT;

   memset (szAscPdsLen, '\0', 4);
   memset (szOldPdsLen, '\0', 4);
   memset (szLastPart, '\0', sizeof (szLastPart));

   if ( (pdsInfo->nMsgType == 1304) ||
        (pdsInfo->nMsgType == 1314) )
   {
      nPosLen = 3;
      nPosPds = 0;
      nPdsLen = 3;
      if ( (nIndice = GetEuroPds93Indice(pds_name)) == -1)
         return(ECHEC);
      nPdsType    = GetEuroPds93Type(nIndice) ;
      nPdsLength  = GetEuroPds93Length(nIndice); 
      sprintf ( szAscPdsLen, "%.3d", length);
   }
   else
   {
      nPosLen = 0;
      nPosPds = 2;
      nPdsLen = 2;
      if ( (nIndice = GetEuroPdsIndice(pds_name)) == -1)
         return(ECHEC);
      nPdsType    = GetEuroPdsType(nIndice) ;
      nPdsLength  = GetEuroPdsLength(nIndice); 
      sprintf ( szAscPdsLen, "%.2d", length+2);
   }

   
   if (pdsInfo->nPresent[nIndice] == PRESENT)
   {
      nPutPos = pdsInfo->nPosPds[nIndice];
      memcpy (szOldPdsLen,
              pdsInfo->sPdsData + pdsInfo->nPosPds [nIndice] + nPosLen,
              nPdsLen);
      nOldPdsLen = atoi(szOldPdsLen) - nPosPds;
      memcpy(szLastPart, 
             pdsInfo->sPdsData + nPutPos + nPdsLen + nPdsLen + nOldPdsLen,
             pdsInfo->nLength - nPutPos - nPdsLen - nPdsLen - nOldPdsLen);
   }
   else
      nPutPos = pdsInfo->nLength;
   
   switch ( nPdsType )
   {
   case EURO_PDS_FIX_ALPHA :
      if ( nPdsLength != length)
         return(ECHEC);
      AsciiToEbcdic( data, 
                     pdsInfo->sPdsData + nPutPos +(2*nPdsLen), 
                     length);
      break;

   case EURO_PDS_VAR_ALPHA :
      AsciiToEbcdic( data, 
                     pdsInfo->sPdsData + nPutPos +(2*nPdsLen), 
                     length);
      break;

   }

   AsciiToEbcdic (szAscPdsLen,
                  pdsInfo->sPdsData + nPutPos + nPosLen,
                  nPdsLen 
                 );
   if (nPdsPresent == NOT_PRESENT )
   {
      AsciiToEbcdic (pds_name,
                     pdsInfo->sPdsData + pdsInfo->nLength + nPosPds,
                     nPdsLen 
                    );
      pdsInfo->nPosPds [ nIndice ] = pdsInfo->nLength;
      pdsInfo->nPresent[ nIndice ] = PRESENT;
      pdsInfo->nLength +=length + nPdsLen + nPdsLen;
   }
   else
   {
      memcpy (pdsInfo->sPdsData + nPutPos+ nPdsLen+ nPdsLen+ length- nOldPdsLen,
              szLastPart,
              pdsInfo->nLength - nPutPos - nPdsLen - nPdsLen - nOldPdsLen);

      pdsInfo->nLength +=length - nOldPdsLen;
      for ( i = nIndice+1; i < MAX_EURO_PDS; i++)
      {
         if ( pdsInfo->nPresent[i] == PRESENT)
            pdsInfo->nPosPds[i] += length - nOldPdsLen;
      }
   }
   return(SUCCES);
}

int EuroBuildPds (char * sPdsBuffer, TSPdsEuro *pdsInfo)
{
    int i;
      fprintf ( stderr,"\n Data Avant le Build : ");
   for ( i = 0 ; i < pdsInfo->nLength ; i ++ )
      fprintf ( stderr,"%02X ", pdsInfo->sPdsData[i] );

   for ( i = 0 ; i <4 ; i++ ) sPdsBuffer [ i ] = 0x40;
   memcpy ( sPdsBuffer + 4 , pdsInfo->sPdsData , pdsInfo->nLength);

   return(pdsInfo->nLength + 4);
}

