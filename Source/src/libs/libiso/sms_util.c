#include <stdio.h>
#include <stdlib.h>

#include <iso_sms.h>
#include <header_sms.h>
#include <tools.h>
#include <define.h>


int GetSmsField(int field_no, TSSmsInfo *msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;

   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      switch (GetSmsFieldType(field_no))
      {
      case SMS_VAR_BIN :
      case SMS_VAR_BITMAP :
         length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
         memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no]+1,length);
         *nLength = length;
         break;
	  case SMS_VAR2_BIN    :   /*IBO20190723 PROD00070403 Enh19.Q4 Art 9.1.3*/                        
			length = ((unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]))*256) + (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]+1));
			memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no]+VAR2_LEN_LEN,length);
			*nLength = length;
            break;
      case SMS_FIX_BIN :
         length = GetSmsFieldLength(field_no);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         *nLength = length;
         break;

      case SMS_FIX_HEX :                           
         length = GetSmsFieldLength(field_no);
         nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                             data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case SMS_VAR_HEX :                           
         length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case SMS_FIX_BCD :                           
         length = GetSmsFieldLength(field_no);
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case SMS_VAR_BCD :                           
         length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case SMS_VAR_SHEX :                           

         length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            data, 2*length); 
         data[2*length] = '\0';
         *nLength = 2*length;
         break;

      case SMS_VAR_SBCD :                           
         length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            data, 2*length); 
         data[2*length] = '\0';
         *nLength = 2*length;
         break;

      case SMS_FIX_ALPHA :                           
         length = GetSmsFieldLength(field_no);
         EbcdicToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case SMS_VAR_ALPHA :                           
         length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
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

int AddSmsField(int field_no, TSSmsInfo *msgInfo, char *data, int nLength)
{
   int i,nBytes  = 0;
   char  sLine[MAX_LINE_TRC];
   char sLength[2+1];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
   switch (GetSmsFieldType(field_no))
   {
   case SMS_FIX_BIN :
      nBytes = GetSmsFieldLength(field_no);
      memcpy( msgInfo->sData+msgInfo->nLength, data, nBytes);
      break;

   case SMS_VAR_BIN :
   case SMS_VAR_BITMAP :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;
	  
   case SMS_VAR2_BIN :	/*IBO20190724 PROD00070403 Enh19.Q4 Art 9.1.3*/
      
	  memset(sLength,'\0',sizeof(sLength));
	  sLength [ 0 ] = nLength / 256;
	  sLength [ 1 ] = nLength % 256; 
	  memcpy (msgInfo->sData + msgInfo->nLength, sLength, 2);
      msgInfo->nLength+=VAR2_LEN_LEN;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;
	  
   case SMS_FIX_BCD :
      if ( GetSmsFieldLength(field_no) != nLength )
         return(ECHEC);
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case SMS_FIX_HEX :
      if ( GetSmsFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case SMS_FIX_ALPHA :
      if ( GetSmsFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case SMS_VAR_HEX :
/*START TED PADSS
      sprintf(sLine,"SMS_VAR_HEX %s|%d",data,nLength);
      trace_event(sLine,TRACE);
END TED PADSS*/
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case SMS_VAR_BCD :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case SMS_VAR_SHEX :
      *(msgInfo->sData + msgInfo->nLength) = (nLength+1)/2;
      msgInfo->nLength++;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case SMS_VAR_SBCD :
      *(msgInfo->sData + msgInfo->nLength) = (nLength+1)/2;
      msgInfo->nLength++;
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case SMS_VAR_ALPHA :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      AsciiToEbcdic(data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   msgInfo->nLength += nBytes;
   for (i=field_no; i< MAX_SMS_FIELDS; i++)
       msgInfo->nFieldPos[i+1] = msgInfo->nLength;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) /* && (field_no < 128) */)
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int InsertSmsField(int field_no, TSSmsInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int i;
   char szSmsData [999];
   char sLine [MAX_LINE_TRC];

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   memset(szSmsData, 0, sizeof(szSmsData));

   switch (GetSmsFieldType(field_no))
   {
   case SMS_FIX_BIN :
      nBytes = GetSmsFieldLength(field_no);
      memcpy( szSmsData, data, nBytes);
      break;

   case SMS_VAR_BIN :
   case SMS_VAR_BITMAP :
      *(szSmsData) = nLength;
      nBytes = 1;
      memcpy( szSmsData+1, data, nLength);
      nBytes += nLength;
      break;

   case SMS_FIX_BCD :
      if ( GetSmsFieldLength(field_no) != nLength )
         return(ECHEC);
      nBytes = AsciiToBcd( data, szSmsData, nLength);
      break;

   case SMS_FIX_HEX :
      if ( GetSmsFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex( data, szSmsData, nLength);
      break;

   case SMS_FIX_ALPHA :
      if ( GetSmsFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, szSmsData, nLength);
      nBytes = nLength;
      break;

   case SMS_VAR_SHEX :
      *(szSmsData) = (nLength+1)/2;
      nBytes = AsciiToHex( data, szSmsData+1, nLength);
      sprintf(sLine,"SHEXXXXXXXXX %d||%d|%02X|%02X|%02X", nBytes,nLength,szSmsData[0], szSmsData[1], szSmsData[5]);
      trace_event(sLine,TRACE);
      nBytes++;
      break;

   case SMS_VAR_SBCD :
      *(szSmsData) = (nLength+1)/2;
      nBytes = AsciiToBcd( data, szSmsData+1, nLength);
      nBytes++;
      break;

   case SMS_VAR_BCD :
      *(szSmsData) = nLength;
      nBytes = AsciiToBcd( data, szSmsData+1, nLength);
      nBytes++;
      break;

   case SMS_VAR_HEX :
      *(szSmsData) = nLength;
      nBytes = AsciiToHex( data, szSmsData+1, nLength);
      nBytes++;
      break;

   case SMS_VAR_ALPHA :
      *(szSmsData) = nLength;
      nBytes =1;
      AsciiToEbcdic(data, szSmsData+1, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
   for (i = MAX_SMS_FIELDS-1; i > field_no; i--)
   {
      memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
              msgInfo->sData+msgInfo->nFieldPos[i],
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      msgInfo->nFieldPos[i+1] += nBytes;
   }
   memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szSmsData, nBytes);
   msgInfo->nFieldPos[field_no+1] += nBytes;
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) /* && (field_no < 128)*/)
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int PutSmsField(int field_no, TSSmsInfo *msgInfo, char *data, int nLength)
{

   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i, nOldFldLen,nOldLen;
   char szSmsData [999];	
   char szWork    [LG_MAX];	
   char szIsoFldLen[4];	

   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      switch (GetSmsFieldType(field_no))
      {
      case SMS_FIX_BIN :
         nBytes = GetSmsFieldLength(field_no);
         memcpy( szSmsData, data, nBytes);
         break;

      case SMS_VAR_BIN :
      case SMS_VAR_BITMAP :
         *(szSmsData) = nLength;
         nBytes = 1;
         memcpy( szSmsData+1, data, nLength);
         nBytes += nLength;
         break;

      case SMS_FIX_BCD :
         if ( GetSmsFieldLength(field_no) != nLength )
            return(ECHEC);
         nBytes = AsciiToBcd( data, szSmsData, nLength);
         break;

      case SMS_FIX_HEX :
         if ( GetSmsFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToHex( data, szSmsData, nLength);
         break;

      case SMS_FIX_ALPHA :
         if ( GetSmsFieldLength(field_no) != nLength)
            return(ECHEC);
         AsciiToEbcdic( data, szSmsData, nLength);
         nBytes = nLength;
         break;

      case SMS_VAR_SHEX :
         *(szSmsData) = (nLength+1)/2;
         nBytes = AsciiToHex( data, szSmsData+1, nLength);
         nBytes++;
         break;

      case SMS_VAR_SBCD :
         *(szSmsData) = (nLength+1)/2;
         nBytes = AsciiToBcd( data, szSmsData+1, nLength);
         nBytes++;
         break;

      case SMS_VAR_BCD :
         *(szSmsData) = nLength;
         nBytes = AsciiToBcd( data, szSmsData+1, nLength);
         nBytes++;
         break;

      case SMS_VAR_HEX :
         *(szSmsData) = nLength;
         nBytes = AsciiToHex( data, szSmsData+1, nLength);
         nBytes++;
         break;

      case SMS_VAR_ALPHA :
         *(szSmsData) = nLength;
         nBytes =1;
         AsciiToEbcdic(data, szSmsData+1, nLength);
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

      for (i = MAX_SMS_FIELDS-1; i > field_no; i--)
      {
/*         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
*/
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szSmsData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return(SUCCES);
   }
   else
      return ( InsertSmsField (field_no, msgInfo, data, nLength));
/*
      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      for (i = MAX_SMS_FIELDS-1; i > field_no; i--)
      {
         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szSmsData, nBytes);
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
      return(InsertSmsField(field_no, msgInfo, data, nLength));
*/
}

int SmsBuildMsg (char * buffer_snd, TSSmsInfo *msgInfo)
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

void SmsBuildHeader(int      nReqResp, 
                    TSSmsHeader*   pNewHeader, 
                    TSSmsHeader*   pIncomingHeader, 
                    int nSmsMsgLength,
                    char * szStationId)
{
   char  lStationId [ 6 +1 ];/* NGA 140807*/
   if (REQUEST == nReqResp)
   {
      memcpy(lStationId, pNewHeader->abSrcStationId, 6);/* NGA 140807*/
      memset( (char*)pNewHeader, 0, sizeof(TSSmsHeader));
     memcpy(pNewHeader->abDestStationId, lStationId,6);/* NGA 140807*/
      AsciiToBcd(szStationId, pNewHeader->abSrcStationId, 6);
   }
   else /* This is a response */
   {
      memcpy((char*)pNewHeader, (char*)pIncomingHeader, sizeof(TSSmsHeader));
      memcpy(pNewHeader->abSrcStationId, pIncomingHeader->abDestStationId, 3);
      memcpy(pNewHeader->abDestStationId, pIncomingHeader->abSrcStationId, 3);
      memcpy(pNewHeader->abBaseIIFlags, pIncomingHeader->abBaseIIFlags, 2); /* ENH VISA 200904 HBU */
   }

   pNewHeader->bHeaderLength = SMS_HDR_LEN;

   /* memset( pNewHeader->abBaseIIFlags , 0, 2);  ENH VISA 200904 HBU */
   pNewHeader->bHeaderFlag   = 0x01;
   pNewHeader->bTextFormat   = 0x02;
   pNewHeader->abMsgLength[0] =  (nSmsMsgLength + SMS_HDR_LEN) / 256;
   pNewHeader->abMsgLength[1] =  (nSmsMsgLength + SMS_HDR_LEN) % 256;
}

void SmsHeaderToBuffer ( TSSmsInfo *SmsInfo , char *SmsHeaderBuffer )
{
    SmsHeaderBuffer [ 0 ] =   SmsInfo->sHeader.bHeaderLength;
    SmsHeaderBuffer [ 1 ] =   SmsInfo->sHeader.bHeaderFlag;
    SmsHeaderBuffer [ 2 ] =   SmsInfo->sHeader.bTextFormat;
    memcpy ( SmsHeaderBuffer  + 2 ,   SmsInfo->sHeader.abMsgLength      , 2 );
    memcpy ( SmsHeaderBuffer  + 4 ,   SmsInfo->sHeader.abDestStationId  , 3 );
    memcpy ( SmsHeaderBuffer  + 7 ,   SmsInfo->sHeader.abSrcStationId   , 3 );
    SmsHeaderBuffer [ 10 ] =   SmsInfo->sHeader.bEchoInfo;
    memcpy ( SmsHeaderBuffer + 11  , SmsInfo->sHeader.abBaseIIFlags   , 3 );
    memcpy ( SmsHeaderBuffer + 14  , SmsInfo->sHeader.abMsgFlags      , 3 ); 
    SmsHeaderBuffer [ 17 ] =   SmsInfo->sHeader.bBatchNumber;
    memcpy ( SmsHeaderBuffer  + 18 , SmsInfo->sHeader.abReserved    ,  3);
    SmsHeaderBuffer [ 21 ] =   SmsInfo->sHeader.bUserInfo ;
}


int AddSmsIcTag (char  *tag_name, TSTagSms *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   char sLine[MAX_LINE_TRC];

trace_event("Start AddSmsIcTag",PROCESSING);
   if ( (nIndice = GetSmsTagIndice(tag_name)) == -1)
      return(ECHEC);

sprintf(sLine,"nIndice = %d",nIndice);
trace_event(sLine,PROCESSING);

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
			sprintf(sLine,"TAG IS %s|****************|%d",tag_name , nLenTag);
		else
			sprintf(sLine,"TAG IS %s|%02X|%d",tag_name ,*(tagInfo->sChipData + tagInfo->nLength), nLenTag);
	}
	trace_event(sLine,TRACE);
	
   switch (GetSmsTagType(nIndice) )
   {
   case SMS_TAG_FIX_BIN :
   case SMS_TAG_FIX_ALPHA :
      if ( GetSmsTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case SMS_TAG_VAR_HEX :
      nDigits = AsciiToHex( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
                            length);
      break;
case SMS_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
                            length);
      break;

   case SMS_TAG_FIX_BCD :
      if ( GetSmsTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
                            length);
      break;

   case SMS_TAG_VAR_BIN :
   case SMS_TAG_VAR_ALPHA :
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   }

  *(tagInfo->sChipData + tagInfo->nLength + nLenTag)    = nDigits;
   tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;

  tagInfo->nPresent[ nIndice ] = PRESENT;
   tagInfo->nLength +=nDigits+nLenTag + 1;
   return(SUCCES);
trace_event("End AddSmsIcTag",PROCESSING);
}

int SmsBuildIcFld (char * buffer_snd, TSTagSms *tagInfo)
{
   memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
   return(tagInfo->nLength);
}

int GetSmsIcTag (char  *tag_name, TSTagSms *tagInfo, char *data, int *length)
{
   int nIndice ;
   int nLenTag ;
   int nDigits;
   char  sLine[MAX_LINE_TRC];

   if ( (nIndice = GetSmsTagIndice(tag_name)) == -1)
      return(ECHEC);

   if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);

   if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]) & 0x1F) == 0x1F)
      nLenTag = 2;
   else
      nLenTag = 1;
   *length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag ));


   switch (GetSmsTagType(nIndice) )
   {
   case SMS_TAG_FIX_BIN :
   case SMS_TAG_FIX_ALPHA :
      memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
                   *length);
      break;

   case SMS_TAG_VAR_HEX :
      nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] +
                                               nLenTag+1,
                            data,
                            (*length)*2);
      *length = (*length)*2;
      break;
   case SMS_TAG_VAR_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] +
                                               nLenTag+1,
                            data,
                            (*length)*2);
      *length = (*length)*2;
      break;

   case SMS_TAG_FIX_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] +
                                               nLenTag+1,
                                                        data,
                            (*length)*2);
      *length = (*length)*2;
      break;

   case SMS_TAG_VAR_BIN :
   case SMS_TAG_VAR_ALPHA :
      memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
                    *length);
      break;

   }
   return(OK);
}