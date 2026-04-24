#include <stdio.h>
#include <stdlib.h>
#include <iso8583.h>
#include <iso_visa.h>
#include <header_visa.h>
#include <tools.h>
#include <define.h>


int GetVisaField(int field_no, TSVisaInfo *msgInfo, /*OBE121106*/unsigned char *data, int *nLength)
{
   int length;
   int nBytes;

   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      switch (GetVisaFieldType(field_no))
      {
      case VISA_VAR_BIN :
      case VISA_VAR_BITMAP :
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no]+1,length);
         *nLength = length;
         break;
      case VISA_VAR2_BIN    :   /*IBO20190723 PROD00070403 Enh19.Q4 Art 9.1.3*/                        
			length = ((unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]))*256) + (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]+1));
			memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no]+VAR2_LEN_LEN,length);
			*nLength = length;
            break;

      case VISA_FIX_BIN :
         length = GetVisaFieldLength(field_no);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         *nLength = length;
         break;

      case VISA_FIX_HEX :                           
         length = GetVisaFieldLength(field_no);
         nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                             data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case VISA_VAR_HEX :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case VISA_FIX_BCD :                           
         length = GetVisaFieldLength(field_no);
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case VISA_VAR_BCD :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case VISA_VAR_SBCD :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            data, 2*length); 
         data[2*length] = '\0';
         *nLength = 2*length;
         break;

      case VISA_FIX_ALPHA :                           
         length = GetVisaFieldLength(field_no);
         EbcdicToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case VISA_VAR_ALPHA :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         EbcdicToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
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

int AddVisaField(int field_no, TSVisaInfo *msgInfo, char *data, int nLength)
{
   int i,nBytes  = 0;
   char  sLine[MAX_LINE_TRC];
   char sLength[2+1];
   
	
   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
   switch (GetVisaFieldType(field_no))
   {
   case VISA_FIX_BIN :
      nBytes = GetVisaFieldLength(field_no);
      memcpy( msgInfo->sData+msgInfo->nLength, data, nBytes);
      break;

   case VISA_VAR_BIN :
   case VISA_VAR_BITMAP :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case VISA_VAR2_BIN :	/*IBO20190724 PROD00070403 Enh19.Q4 Art 9.1.3*/
      /**(msgInfo->sData + msgInfo->nLength) = nLength;*/
	  memset(sLength,'\0',sizeof(sLength));
	  sLength [ 0 ] = nLength / 256;
	  sLength [ 1 ] = nLength % 256; 
	  memcpy (msgInfo->sData + msgInfo->nLength, sLength, 2);
      msgInfo->nLength+=VAR2_LEN_LEN;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;
			
   case VISA_FIX_BCD :
      if ( GetVisaFieldLength(field_no) != nLength )
         return(ECHEC);
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case VISA_FIX_HEX :
      if ( GetVisaFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case VISA_FIX_ALPHA :
      if ( GetVisaFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case VISA_VAR_HEX :
		if(debug_option == 1 )
		{
		  sprintf(sLine,"VISA_VAR_HEX %s|%d",data,nLength);
		  trace_event(sLine,TRACE);
		}
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case VISA_VAR_BCD :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case VISA_VAR_SBCD :
      *(msgInfo->sData + msgInfo->nLength) = (nLength+1)/2;
      msgInfo->nLength++;
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case VISA_VAR_ALPHA :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      AsciiToEbcdic(data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   msgInfo->nLength += nBytes;
   for (i=field_no; i< MAX_VISA_FIELDS; i++)
       msgInfo->nFieldPos[i+1] = msgInfo->nLength;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) /* && (field_no < 128) */)
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int InsertVisaField(int field_no, TSVisaInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int i;
   char szVisaData [999];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   switch (GetVisaFieldType(field_no))
   {
   case VISA_FIX_BIN :
      nBytes = GetVisaFieldLength(field_no);
      memcpy( szVisaData, data, nBytes);
      break;

   case VISA_VAR_BIN :
   case VISA_VAR_BITMAP :
      *(szVisaData) = nLength;
      nBytes = 1;
      memcpy( szVisaData+1, data, nLength);
      nBytes += nLength;
      break;

   case VISA_FIX_BCD :
      if ( GetVisaFieldLength(field_no) != nLength )
         return(ECHEC);
      nBytes = AsciiToBcd( data, szVisaData, nLength);
      break;

   case VISA_FIX_HEX :
      if ( GetVisaFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex( data, szVisaData, nLength);
      break;

   case VISA_FIX_ALPHA :
      if ( GetVisaFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, szVisaData, nLength);
      nBytes = nLength;
      break;

   case VISA_VAR_SBCD :
      *(szVisaData) = (nLength+1)/2;
      nBytes = AsciiToBcd( data, szVisaData+1, nLength);
      nBytes++;
      break;

   case VISA_VAR_BCD :
      *(szVisaData) = nLength;
      nBytes = AsciiToBcd( data, szVisaData+1, nLength);
      nBytes++;
      break;

   case VISA_VAR_HEX :
      *(szVisaData) = nLength;
      nBytes = AsciiToHex( data, szVisaData+1, nLength);
      nBytes++;
      break;

   case VISA_VAR_ALPHA :
      *(szVisaData) = nLength;
      nBytes =1;
      AsciiToEbcdic(data, szVisaData+1, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
   for (i = MAX_VISA_FIELDS-1; i > field_no; i--)
   {
      memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
              msgInfo->sData+msgInfo->nFieldPos[i],
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      msgInfo->nFieldPos[i+1] += nBytes;
   }
   memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szVisaData, nBytes);
   msgInfo->nFieldPos[field_no+1] += nBytes;
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) /* && (field_no < 128)*/)
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int PutVisaField(int field_no, TSVisaInfo *msgInfo, char *data, int nLength)
{

   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i, nOldFldLen,nOldLen;
   char szVisaData [999];	
   char szWork    [LG_MAX];	
   char szIsoFldLen[4];	

   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      switch (GetVisaFieldType(field_no))
      {
      case VISA_FIX_BIN :
         nBytes = GetVisaFieldLength(field_no);
         memcpy( szVisaData, data, nBytes);
         break;

      case VISA_VAR_BIN :
      case VISA_VAR_BITMAP :
         *(szVisaData) = nLength;
         nBytes = 1;
         memcpy( szVisaData+1, data, nLength);
         nBytes += nLength;
         break;

      case VISA_FIX_BCD :
         if ( GetVisaFieldLength(field_no) != nLength )
            return(ECHEC);
         nBytes = AsciiToBcd( data, szVisaData, nLength);
         break;

      case VISA_FIX_HEX :
         if ( GetVisaFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToHex( data, szVisaData, nLength);
         break;

      case VISA_FIX_ALPHA :
         if ( GetVisaFieldLength(field_no) != nLength)
            return(ECHEC);
         AsciiToEbcdic( data, szVisaData, nLength);
         nBytes = nLength;
         break;

      case VISA_VAR_SBCD :
         *(szVisaData) = (nLength+1)/2;
         nBytes = AsciiToBcd( data, szVisaData+1, nLength);
         nBytes++;
         break;

      case VISA_VAR_BCD :
         *(szVisaData) = nLength;
         nBytes = AsciiToBcd( data, szVisaData+1, nLength);
         nBytes++;
         break;

      case VISA_VAR_HEX :
         *(szVisaData) = nLength;
         nBytes = AsciiToHex( data, szVisaData+1, nLength);
         nBytes++;
         break;

      case VISA_VAR_ALPHA :
         *(szVisaData) = nLength;
         nBytes =1;
         AsciiToEbcdic(data, szVisaData+1, nLength);
         nBytes += nLength;
         break;

      default :
         return(ECHEC);
      }

      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      memset(szWork, 0, sizeof(szWork));
      nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
      memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

      for (i = MAX_VISA_FIELDS-1; i > field_no; i--)
      {
/*         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
*/
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szVisaData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return(SUCCES);
   }
   else
      return ( InsertVisaField (field_no, msgInfo, data, nLength));
/*
      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      for (i = MAX_VISA_FIELDS-1; i > field_no; i--)
      {
         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szVisaData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      if (field_no >= 128)
         SetBit(msgInfo->sBitMap, 65);
      return(SUCCES);
   }
   else
      return(InsertVisaField(field_no, msgInfo, data, nLength));
*/
}

int VisaBuildMsg (char * buffer_snd, TSVisaInfo *msgInfo)
{
   char szMsgType[5];
   int  nBytes;
   int  iMapCtr;

   memset(szMsgType, 0, 5);
   sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
   fprintf(stderr, "MsgType : %.4s\n", szMsgType);
   nBytes = AsciiToBcd( szMsgType, buffer_snd, 4);

   iMapCtr = 1;
   while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) /* && (iMapCtr < 2) */ )
      ++iMapCtr;
   memcpy( buffer_snd+2, msgInfo->sBitMap, iMapCtr*8);
   memcpy( buffer_snd+2 + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
   return( 2 + (iMapCtr*8) + msgInfo->nLength);
}

void VisaBuildHeader(int      nReqResp, 
                    TSVisaHeader*   pNewHeader, 
                    TSVisaHeader*   pIncomingHeader, 
                    int nVisaMsgLength,
                    char * szStationId)
{
   if (REQUEST == nReqResp)
   {
      memset( (char*)pNewHeader, 0, sizeof(TSVisaHeader));
      AsciiToBcd(szStationId, pNewHeader->abSrcStationId, 6);
       memset( pNewHeader->abBaseIIFlags , 0, 2);/*SWI200904 ENH VISA April 2009*/
   }
   else /* This is a response */
   {
      memcpy((char*)pNewHeader, (char*)pIncomingHeader, sizeof(TSVisaHeader));
      memcpy(pNewHeader->abSrcStationId, pIncomingHeader->abDestStationId, 3);
      memcpy(pNewHeader->abDestStationId, pIncomingHeader->abSrcStationId, 3);
      memcpy(pNewHeader->abBaseIIFlags , pIncomingHeader->abBaseIIFlags, 2); /*SWI200904 ENH VISA April 2009*/
   }

   pNewHeader->bHeaderLength = VISA_HDR_LEN;
/*memset( pNewHeader->abBaseIIFlags , 0, 2); SWI200904 ENH VISA April 2009*/
   pNewHeader->bHeaderFlag   = 0x01;
   pNewHeader->bTextFormat   = 0x02;
   pNewHeader->abMsgLength[0] =  (nVisaMsgLength + VISA_HDR_LEN) / 256;
   pNewHeader->abMsgLength[1] =  (nVisaMsgLength + VISA_HDR_LEN) % 256;
}

void VisaHeaderToBuffer ( TSVisaInfo *VisaInfo , char *VisaHeaderBuffer )
{
    VisaHeaderBuffer [ 0 ] =   VisaInfo->sHeader.bHeaderLength;
    VisaHeaderBuffer [ 1 ] =   VisaInfo->sHeader.bHeaderFlag;
    VisaHeaderBuffer [ 2 ] =   VisaInfo->sHeader.bTextFormat;
    memcpy ( VisaHeaderBuffer  + 2 ,   VisaInfo->sHeader.abMsgLength      , 2 );
    memcpy ( VisaHeaderBuffer  + 4 ,   VisaInfo->sHeader.abDestStationId  , 3 );
    memcpy ( VisaHeaderBuffer  + 7 ,   VisaInfo->sHeader.abSrcStationId   , 3 );
    VisaHeaderBuffer [ 10 ] =   VisaInfo->sHeader.bEchoInfo;
    memcpy ( VisaHeaderBuffer + 11  , VisaInfo->sHeader.abBaseIIFlags   , 3 );
    memcpy ( VisaHeaderBuffer + 14  , VisaInfo->sHeader.abMsgFlags      , 3 ); 
    VisaHeaderBuffer [ 17 ] =   VisaInfo->sHeader.bBatchNumber;
    memcpy ( VisaHeaderBuffer  + 18 , VisaInfo->sHeader.abReserved    ,  3);
    VisaHeaderBuffer [ 21 ] =   VisaInfo->sHeader.bUserInfo ;
}

/*Start MKB221208 CCD*/
int GetVisaIcTag (char  *tag_name, TSTagVisa *tagInfo, char *data, int *length)
{
   int nIndice ;
   int nLenTag ;
   int nDigits;
   char  sLine[MAX_LINE_TRC];

   if ( (nIndice = GetVisaTagIndice(tag_name)) == -1)
      return(ECHEC);

   if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);

   if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]) & 0x1F) == 0x1F)
      nLenTag = 2;
   else
      nLenTag = 1;
   *length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag ));


   switch (GetVisaTagType(nIndice) )
   {
   case VISA_TAG_FIX_BIN :
   case VISA_TAG_FIX_ALPHA :
      memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
                   *length);
      break;

   case VISA_TAG_VAR_HEX :
      nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] +
                                               nLenTag+1,
                            data,
                            (*length)*2);
      *length = (*length)*2;
      break;
   case VISA_TAG_VAR_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] +
                                               nLenTag+1,
                            data,
                            (*length)*2);
      *length = (*length)*2;
      break;

   case VISA_TAG_FIX_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] +
                                               nLenTag+1,
                                                        data,
                            (*length)*2);
      *length = (*length)*2;
      break;

   case VISA_TAG_VAR_BIN :
   case VISA_TAG_VAR_ALPHA :
      memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
                    *length);
      break;

   }
   return(SUCCES);
}

int AddVisaIcTag (char  *tag_name, TSTagVisa *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   char sLine[MAX_LINE_TRC];

   if ( (nIndice = GetVisaTagIndice(tag_name)) == -1)
      return(ECHEC);

   if (tagInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);

   AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

   if ( (*(tagInfo->sChipData + tagInfo->nLength) & 0x1F ) == 0x1F)
      nLenTag = 2;
   else
      nLenTag = 1;

   
    if(debug_option == 1 )
		sprintf(sLine,"TAG IS %s|%02X|%d",tag_name ,*(tagInfo->sChipData + tagInfo->nLength), nLenTag);
	else
	{
		if ( memcmp(tag_name, "5F34", 4) == 0 || memcmp(tag_name, "9F27", 4) == 0 || memcmp(tag_name, "57", 2) == 0|| memcmp(tag_name, "5A", 2) == 0)
			sprintf(sLine,"TAG IS %s|***************|%d",tag_name , nLenTag);
		else
			sprintf(sLine,"TAG IS %s|%02X|%d",tag_name ,*(tagInfo->sChipData + tagInfo->nLength), nLenTag);
	}
	trace_event(sLine,TRACE);
	
   switch (GetVisaTagType(nIndice) )
   {
   case VISA_TAG_FIX_BIN :
   case VISA_TAG_FIX_ALPHA :
      if ( GetVisaTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
       break;

   case VISA_TAG_VAR_HEX :
      nDigits = AsciiToHex( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
                            length);
       
      break;

   case VISA_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
                            length);
       
      break;

   case VISA_TAG_FIX_BCD :
      if ( GetVisaTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
                            length);
       
      break;

   case VISA_TAG_VAR_BIN :
   case VISA_TAG_VAR_ALPHA :
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
int PutVisaIcTag (char  *tag_name, TSTagVisa *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   int nPutPos, nTagPresent = NOT_PRESENT;
   char szLastPart  [ MAX_VISA_CHIP_LEN ];
   int nOldTagLen, i;

   memset (szLastPart, '\0', sizeof(szLastPart));
   if ( (nIndice = GetVisaTagIndice(tag_name)) == -1)
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

   switch (GetVisaTagType(nIndice) )
   {
   case VISA_TAG_FIX_BIN :
   case VISA_TAG_FIX_ALPHA :
      if ( GetVisaTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case VISA_TAG_VAR_HEX :
      nDigits = AsciiToHex( data,
                            tagInfo->sChipData + nPutPos + nLenTag+1,
                            length);
      break;

   case VISA_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data,
                            tagInfo->sChipData + nPutPos + nLenTag+1,
                            length);
      break;

   case VISA_TAG_FIX_BCD :
      if ( GetVisaTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data,
                            tagInfo->sChipData + nPutPos + nLenTag+1,
                            length);
      break;

   case VISA_TAG_VAR_BIN :
   case VISA_TAG_VAR_ALPHA :
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
      for (i = nIndice + 1; i < MAX_VISA_CHIP_TAG; i++)
      {
         if (tagInfo->nPresent[i] == PRESENT)
   		tagInfo->nPosTag[i] += nDigits - nOldTagLen;
      }
   }
   return(SUCCES);
}

int VisaBuildIcFld (char * buffer_snd, TSTagVisa *tagInfo)
{
   char szFldLen[4];
   char szBcdFldLen[4];

   memset( szFldLen   , '\0', 4);
   memset( szBcdFldLen, '\0', 4);
   /*ADD Dataset Identifier 2b. Dataset Length. 3b*/
    buffer_snd[0] = 0x01;
        buffer_snd[1] = 0;
        buffer_snd[2] = tagInfo->nLength;
   /*ADD Dataset Identifier 2b. Dataset Length. 3b*/

        memcpy (buffer_snd  + 3 , tagInfo->sChipData, tagInfo->nLength);

    return(tagInfo->nLength + 3);
}
/*End MKB221208 CCD*/
 
