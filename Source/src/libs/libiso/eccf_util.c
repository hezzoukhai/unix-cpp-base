#include <stdio.h>
#include <stdlib.h>

#include <iso_eccf.h>
#include <tools.h>
#include <define.h>


int GetEccfField(int field_no, TSEccfInfo * msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
   int nFldType, nFldLength;
   char szEccfFldLen[4];	
   char szAscFldLen[4];	

   memset( szEccfFldLen, '\0', 4);	
   memset( szAscFldLen, '\0', 4);	
   if (CheckBit (msgInfo->sBitMap, field_no))
   {
         nFldType   =  GetEccfFieldType  (field_no);
         nFldLength =  GetEccfFieldLength(field_no);

      switch (nFldType)
      {
      case ECCF_LL_BIN :
         memcpy(szEccfFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
         EbcdicToAscii(szEccfFldLen, szAscFldLen, 2);
         length = atoi(szAscFldLen);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
         *nLength = length;
         break;

      case ECCF_LLL_BIN :
         memcpy(szEccfFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
         EbcdicToAscii(szEccfFldLen, szAscFldLen, 3);
         length = atoi(szAscFldLen);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
         *nLength = length;
         break;

      case ECCF_FIX_BIN   :
         length = nFldLength;
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         data[length] = '\0';
         *nLength = length;
         break;
         
      case ECCF_FIX_ALPHA :
         length = nFldLength;
         EbcdicToAscii( msgInfo->sData + msgInfo->nFieldPos[field_no], 
                        data, length);
         data[length] = '\0';
         *nLength = length;
         break;
         
      case ECCF_FIX_HEX :                           
         length = nFldLength;
         nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                           data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case ECCF_LL_HEX :                           
         memcpy( szEccfFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         EbcdicToAscii(szEccfFldLen, szAscFldLen, 2);
         length = atoi(szAscFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
                                                              data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case ECCF_LLL_HEX :                           
         memcpy ( szEccfFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         EbcdicToAscii(szEccfFldLen, szAscFldLen, 3);
         length = atoi(szAscFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
                                                              data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case ECCF_LL_ALPHA : 
         memcpy ( szEccfFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         EbcdicToAscii(szEccfFldLen, szAscFldLen, 2);
         length = atoi(szAscFldLen);
         EbcdicToAscii ( msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
                                                                 data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case ECCF_LLL_ALPHA : 
         memcpy ( szEccfFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         EbcdicToAscii(szEccfFldLen, szAscFldLen, 3);
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

int AddEccfField(int field_no, TSEccfInfo * msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int            i;
   int nFldType, nFldLength;
   char szEccfFldLen[4];	
   char szEbcFldLen[4];	
   char LineBuffer [80];


   memset( szEbcFldLen, '\0', 4);	
   memset( szEccfFldLen, '\0', 4);	

   sprintf(LineBuffer, "BIT : %d : Contenu : %.25s", field_no, data);
   trace_event(LineBuffer, PROCESSING);

   if ( !CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);


   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
      nFldType   =  GetEccfFieldType  (field_no);
      nFldLength =  GetEccfFieldLength(field_no);


   switch (nFldType)
   {
   case ECCF_FIX_ALPHA :
      if ( nFldLength != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);


      nBytes = nLength;
      break;

   case ECCF_LL_ALPHA :
      sprintf ( szEccfFldLen, "%.2d", nLength);	
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 2);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
      msgInfo->nLength += 2;
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case ECCF_LLL_ALPHA :
      sprintf ( szEccfFldLen, "%.3d", nLength);	
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 3);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
      msgInfo->nLength += 3;
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;

   case ECCF_FIX_HEX :
      if ( nFldLength != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case ECCF_LL_HEX :
      sprintf ( szEccfFldLen, "%.2d", nLength);	
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 2);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
      msgInfo->nLength += 2;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case ECCF_LLL_HEX :
      sprintf ( szEccfFldLen, "%.3d", nLength);	
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 3);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
      msgInfo->nLength += 3;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case ECCF_FIX_BIN :
      if ( nFldLength != nLength)
         return(ECHEC);
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case ECCF_LL_BIN :
      sprintf ( szEccfFldLen, "%.2d", nLength);	
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 2);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
      msgInfo->nLength += 2;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case ECCF_LLL_BIN :
      sprintf ( szEccfFldLen, "%.3d", nLength);	
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 3);
      memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
      msgInfo->nLength += 3;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   msgInfo->nLength += nBytes;
/*baz  09/04/2001
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
*/
   return(SUCCES);
}

int InsertEccfField(int field_no, TSEccfInfo *msgInfo, char *data, int nLength)
{
   int  nFldType, nFldLength;
   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i;
   char szEccfData [999];	
   char szEccfFldLen[4];	
   char szEbcFldLen[4];	

   memset( szEccfFldLen, '\0', 4);	
   memset( szEbcFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

      nFldType   =  GetEccfFieldType  (field_no);
      nFldLength =  GetEccfFieldLength(field_no);

   switch (nFldType)
   {
   case ECCF_FIX_ALPHA :
      if ( nFldLength != nLength)
         return(ECHEC);
      nBytes = nLength;
      AsciiToEbcdic (data, szEccfData, nBytes);
      break;

   case ECCF_LL_ALPHA :
      sprintf ( szEccfFldLen, "%.2d", nLength);	
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 2);
      memcpy( szEccfData,        szEbcFldLen, 2);
      nBytes=2;
      AsciiToEbcdic( data,        szEccfData+2, nLength);
      nBytes += nLength;
      break;

   case ECCF_LLL_ALPHA :
      sprintf ( szEccfFldLen, "%.3d", nLength);	
      nBytes = 3;
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 3);
      memcpy( szEccfData, szEbcFldLen, 3);
      AsciiToEbcdic( data,        szEccfData+3, nLength);
      nBytes += nLength;
      break;

   case ECCF_FIX_HEX :
      if ( nFldLength != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, szEccfData, nLength);
      break;

   case ECCF_LL_HEX :
      sprintf ( szEccfFldLen, "%.2d", nLength);	
      nBytes1 = 2;
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 2);
      memcpy( szEccfData, szEbcFldLen, nBytes1);
      nBytes = AsciiToHex( data, szEccfData+2, nLength);
      nBytes += nBytes1;
      break;

   case ECCF_LLL_HEX :
      sprintf ( szEccfFldLen, "%.3d", nLength);	
      nBytes1 = 3;
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 3);
      memcpy( szEccfData, szEbcFldLen, 3);
      nBytes = AsciiToHex( data, szEccfData+3, nLength);
      nBytes += nBytes1;
      break;

   case ECCF_FIX_BIN :
      if ( nFldLength != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy( szEccfData, data, nBytes);
      break;

   case ECCF_LL_BIN :
      sprintf ( szEccfFldLen, "%.2d", nLength);	
      nBytes = 2;
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 2);
      memcpy( szEccfData, szEbcFldLen, nBytes);
      memcpy( szEccfData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case ECCF_LLL_BIN :
      sprintf ( szEccfFldLen, "%.3d", nLength);	
      nBytes = 3;
      AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 3);
      memcpy( szEccfData, szEbcFldLen, nBytes);
      memcpy( szEccfData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
   for (i = MAX_ECCF_FIELDS-1; i > field_no; i--)
   {
      memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
              msgInfo->sData+msgInfo->nFieldPos[i],
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      msgInfo->nFieldPos[i+1] += nBytes;
   }
   memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szEccfData, nBytes);
   msgInfo->nFieldPos[field_no+1] += nBytes;
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   return(SUCCES);
}

int PutEccfField(int field_no, TSEccfInfo *msgInfo, char *data, int nLength)
{
   int  nFldType, nFldLength;
   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i, nOldFldLen;
   char szEccfData [999];	
   char szEccfFldLen[4];	
   char szEbcFldLen[4];	

   memset( szEccfFldLen, '\0', 4);	
   memset( szEbcFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
         nFldType   =  GetEccfFieldType  (field_no);
         nFldLength =  GetEccfFieldLength(field_no);

      switch (nFldType)
      {
      case ECCF_FIX_ALPHA :
         if ( nFldLength != nLength)
            return(ECHEC);
         nBytes = nLength;
         AsciiToEbcdic (data, szEccfData, nBytes);
         break;

      case ECCF_LL_ALPHA :
         sprintf ( szEccfFldLen, "%.2d", nLength);	
         AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 2);
         memcpy( szEccfData,        szEbcFldLen, 2);
         nBytes=2;
         AsciiToEbcdic( data,        szEccfData+2, nLength);
         nBytes += nLength;
         break;

      case ECCF_LLL_ALPHA :
         sprintf ( szEccfFldLen, "%.3d", nLength);	
         nBytes = 3;
         AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 3);
         memcpy( szEccfData, szEbcFldLen, 3);
         AsciiToEbcdic( data,        szEccfData+3, nLength);
         nBytes += nLength;
         break;

      case ECCF_FIX_HEX :
         if ( nFldLength != nLength)
            return(ECHEC);
         nBytes = AsciiToHex(data, szEccfData, nLength);
         break;

      case ECCF_LL_HEX :
         sprintf ( szEccfFldLen, "%.2d", nLength);	
         nBytes1 = 2;
         AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 2);
         memcpy( szEccfData, szEbcFldLen, nBytes1);
         nBytes = AsciiToHex( data, szEccfData+2, nLength);
         nBytes += nBytes1;
         break;

      case ECCF_LLL_HEX :
         sprintf ( szEccfFldLen, "%.3d", nLength);	
         nBytes1 = 3;
         AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 3);
         memcpy( szEccfData, szEbcFldLen, 3);
         nBytes = AsciiToHex( data, szEccfData+3, nLength);
         nBytes += nBytes1;
         break;

      case ECCF_FIX_BIN :
         if ( nFldLength != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy( szEccfData, data, nBytes);
         break;

      case ECCF_LL_BIN :
         sprintf ( szEccfFldLen, "%.2d", nLength);	
         nBytes = 2;
         AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 2);
         memcpy( szEccfData, szEbcFldLen, nBytes);
         memcpy( szEccfData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case ECCF_LLL_BIN :
         sprintf ( szEccfFldLen, "%.3d", nLength);	
         nBytes = 3;
         AsciiToEbcdic ( szEccfFldLen, szEbcFldLen, 3);
         memcpy( szEccfData, szEbcFldLen, nBytes);
         memcpy( szEccfData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      default :
         return(ECHEC);
      }
      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      for (i = MAX_ECCF_FIELDS-1; i > field_no; i--)
      {
         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szEccfData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return(SUCCES);
   }
   else
      return (InsertEccfField(field_no, msgInfo, data, nLength));
}

int EccfBuildMsg (char * buffer_snd, TSEccfInfo *msgInfo)
{
   char szMsgType[5];
   char szEbMsgType[5];
   int  nBytes;
   int  iMapCtr;

   sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
   AsciiToEbcdic (szMsgType,szEbMsgType ,4 );
   memcpy( buffer_snd, szEbMsgType, 4);

/*
   iMapCtr = 1;
   while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr; 
   memcpy( buffer_snd+4, msgInfo->sBitMap, iMapCtr*8);
   memcpy( buffer_snd+4 + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
*/

   memcpy( buffer_snd+4, msgInfo->sBitMap, 16);
   memcpy( buffer_snd+4+16, msgInfo->sData, msgInfo->nLength);
/* debut modif M.T 291101*/
   /*return( 4 + (iMapCtr*8) + msgInfo->nLength);*/
   return( 4 + 16 + msgInfo->nLength);
/*end modif M.T 291101*/
}

/****************************************************************************/
/* La longueur d'un TAG dans le champ 55 est toujours code sur un caractere */
/****************************************************************************/
int GetEccfIcTag (char  *tag_name, TSTagEccf *tagInfo, char *data, int *length)
{
   int nIndice ;
   int nLenTag ;
   int nLength ;
   int nDigits;

   if ( (nIndice = GetEccfTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   if ( ((*(tagInfo->sChipData + tagInfo->nPosTag[nIndice])) & 0x1F) == 0x1F)
      nLenTag = 2;
   else
      nLenTag = 1;
   *length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag ));
   nLength = GetEccfTagLength ( nIndice );
   switch (GetEccfTagType(nIndice) )
   {
   case ECCF_TAG_FIX_BIN :
   case ECCF_TAG_FIX_ALPHA :
      memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
                   *length);
      break;

   case ECCF_TAG_VAR_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;

   case ECCF_TAG_VAR_HEX :
      nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;

   case ECCF_TAG_FIX_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1,  
                            data, 
                            nLength);
      *length = nLength;
      break;

   case ECCF_TAG_FIX_HEX :
      nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;

   case ECCF_TAG_VAR_BIN :
   case ECCF_TAG_VAR_ALPHA :
      memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
                    *length);
      break;

   }
   return(SUCCES);
}

int AddEccfIcTag (char  *tag_name, TSTagEccf *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;

   trace_event("Start AddEccfIcTag", PROCESSING);

   if ( (nIndice = GetEccfTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);
   if ( ((*(tagInfo->sChipData + tagInfo->nLength)) & 0x1F) == 0x1F)
      nLenTag = 2;
   else 
      nLenTag = 1;
   switch (GetEccfTagType(nIndice) )
   {
   case ECCF_TAG_FIX_BIN :
   case ECCF_TAG_FIX_ALPHA :
      if ( GetEccfTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case ECCF_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case ECCF_TAG_FIX_BCD :
      if ( GetEccfTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case ECCF_TAG_VAR_BIN :
   case ECCF_TAG_VAR_ALPHA :
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

int PutEccfIcTag (char  *tag_name, TSTagEccf *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   int nPutPos, nTagPresent = NOT_PRESENT;
   char szLastPart  [ MAX_ECCF_CHIP_LEN ];
   int nOldTagLen, i;

   memset (szLastPart, '\0', sizeof(szLastPart));
   if ( (nIndice = GetEccfTagIndice(tag_name)) == -1)
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

   if ( ((*(tagInfo->sChipData + nPutPos)) & 0x1F) == 0x1F)
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

   switch (GetEccfTagType(nIndice) )
   {
   case ECCF_TAG_FIX_BIN :
   case ECCF_TAG_FIX_ALPHA :
      if ( GetEccfTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case ECCF_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case ECCF_TAG_FIX_BCD :
      if ( GetEccfTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case ECCF_TAG_VAR_BIN :
   case ECCF_TAG_VAR_ALPHA :
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
      for (i = nIndice + 1; i < MAX_ECCF_CHIP_TAG; i++)
      {
         if (tagInfo->nPresent[i] == PRESENT)
            tagInfo->nPosTag[i] += nDigits - nOldTagLen;
      }
   }
   return(SUCCES);
}

int EccfBuildIcFld (char * buffer_snd, TSTagEccf *tagInfo)
{
   memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
   return (tagInfo->nLength);
}

/****************************************************************************/
/* Le champ 48                                                              */
/****************************************************************************/
int GetEccfPds (char  *pds_name, TSPdsEccf *pdsInfo, char *data, int *length)
{
   int nIndice;
   int nPosLen; /* Position de la longueur */
   int nPosPds; /* Position du PDS         */
   int nPdsLen; /* Longueur du PDS et de la longueur */
   int nPdsLength;
   char szAscPdsLen [4];
   char szEbcPdsLen [4];
   int nPdsType;

   memset (szAscPdsLen, '\0', 4);

      nPosLen = 0;
      nPosPds = 2;
      nPdsLen = 2;
      if ( (nIndice = GetEccfPdsIndice(pds_name)) == -1)
         return(ECHEC);
      nPdsType    = GetEccfPdsType(nIndice) ;
      nPdsLength  = GetEccfPdsLength(nIndice); 
      /* EBE140423 sprintf ( szAscPdsLen, "%.2d", (int)(length+2));*/
	  sprintf ( szAscPdsLen, "%.2d", (int)((*length)+2));

   if (pdsInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   memcpy ( szEbcPdsLen, 
            pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice]+nPosLen,
            nPdsLen);
   EbcdicToAscii (szEbcPdsLen, szAscPdsLen, nPdsLen);
   *length = atoi(szAscPdsLen);
   switch ( nPdsType )
   {
   case ECCF_PDS_FIX_ALPHA :
      EbcdicToAscii( pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] 
                                       + nPdsLen + nPdsLen, 
                     data,
                     *length);
      break;

   case ECCF_PDS_VAR_ALPHA :
      EbcdicToAscii( pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] 
                                       + nPdsLen + nPdsLen, 
                     data,
                     *length);
      break;

   }
   return(SUCCES);
}

int AddEccfPds (char  *pds_name, TSPdsEccf *pdsInfo, char *data, int length)
{
   int nIndice;
   int nPosLen; /* Position de la longueur */
   int nPosPds; /* Position du PDS         */
   int nPdsLen; /* Longueur du PDS et de la longueur */
   int nPdsLength;
   char szAscPdsLen [4];
   int nPdsType;

   memset (szAscPdsLen, '\0', 4);

      nPosLen = 0;
      nPosPds = 2;
      nPdsLen = 2;
      if ( (nIndice = GetEccfPdsIndice(pds_name)) == -1)
         return(ECHEC);
      nPdsType    = GetEccfPdsType(nIndice) ;
      nPdsLength  = GetEccfPdsLength(nIndice); 
      sprintf ( szAscPdsLen, "%.2d", length+2);

   
   if (pdsInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   switch ( nPdsType )
   {
   case ECCF_PDS_FIX_ALPHA :
      if ( nPdsLength != length)
         return(ECHEC);
      AsciiToEbcdic( data, 
                     pdsInfo->sPdsData + pdsInfo->nLength +(2*nPdsLen), 
                     length);
      break;

   case ECCF_PDS_VAR_ALPHA :
      AsciiToEbcdic( data, 
                     pdsInfo->sPdsData + pdsInfo->nLength +(2*nPdsLen), 
                     length);
      break;

   }

   AsciiToEbcdic (szAscPdsLen,
                  pdsInfo->sPdsData + pdsInfo->nLength + nPosLen,
                  nPdsLen 
                 );
   AsciiToEbcdic (pds_name,
                  pdsInfo->sPdsData + pdsInfo->nLength + nPosPds,
                  nPdsLen 
                 );
   pdsInfo->nPosPds [ nIndice ] = pdsInfo->nLength;
   pdsInfo->nPresent[ nIndice ] = PRESENT;
   pdsInfo->nLength +=length + nPdsLen + nPdsLen;
   return(SUCCES);
}

int PutEccfPds (char  *pds_name, TSPdsEccf *pdsInfo, char *data, int length)
{
   int nIndice;
   int nPosLen; /* Position de la longueur */
   int nPosPds; /* Position du PDS         */
   int nPdsLen; /* Longueur du PDS et de la longueur */
   int nPdsLength;
   char szAscPdsLen [4];
   char szOldPdsLen [4];
   char szLastPart  [ MAX_ECCF_PDS_LEN ];
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
      if ( (nIndice = GetEccfPdsIndice(pds_name)) == -1)
         return(ECHEC);
      nPdsType    = GetEccfPdsType(nIndice) ;
      nPdsLength  = GetEccfPdsLength(nIndice); 
      sprintf ( szAscPdsLen, "%.2d", length+2);

   
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
   case ECCF_PDS_FIX_ALPHA :
      if ( nPdsLength != length)
         return(ECHEC);
      AsciiToEbcdic( data, 
                     pdsInfo->sPdsData + nPutPos +(2*nPdsLen), 
                     length);
      break;

   case ECCF_PDS_VAR_ALPHA :
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
      for ( i = nIndice+1; i < MAX_ECCF_PDS; i++)
      {
         if ( pdsInfo->nPresent[i] == PRESENT)
            pdsInfo->nPosPds[i] += length - nOldPdsLen;
      }
   }
   return(SUCCES);
}

int EccfBuildPds (char * buffer_snd, TSPdsEccf *pdsInfo)
{
   memcpy (buffer_snd, pdsInfo->sPdsData, pdsInfo->nLength);
   return(pdsInfo->nLength);
}

/******************************************************************************/
/* Fonction Hexa_To_Binaire                     .                             */
/* ---------------------                                                      */
/******************************************************************************/
void    Hexa_To_Binaire(target, source)
char    *target;
char    *source;
{
   int    i;
   unsigned char   a,b,c;

   for(i = 0; i < 8; i++)
   {
      a = *(source + 2 * i);
      b = *(source + 2 * i + 1);
      c = 0x00;

      if (a == 0x41) a = 0x0A;
      else
         if (a == 0x42) a = 0x0B;
         else
            if (a == 0x43) a = 0x0C;
            else
               if (a == 0x44) a = 0x0D;
               else
                  if (a == 0x45) a = 0x0E;
                  else
                     if (a == 0x46) a = 0x0F;

      if (b == 0x41) b = 0x0A;
      else
         if (b == 0x42) b = 0x0B;
         else
            if (b == 0x43) b = 0x0C;
            else
               if (b == 0x44) b = 0x0D;
               else
                  if (b == 0x45) b = 0x0E;
                  else
                     if (b == 0x46) b = 0x0F;

      c = (unsigned char) ((a << 4) & 0xF0) | (b & 0x0F);

      *(target +  i)  = c;
   }

   return;
}
