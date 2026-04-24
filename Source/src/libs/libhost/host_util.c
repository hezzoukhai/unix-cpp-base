/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      bank_util.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      BANK Tools Management                                        */
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
#include <define.h>


int GetBankField(int field_no, TSBankInfo * msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
   char szBankFldLen[4];	

   memset( szBankFldLen, '\0', 4);	
   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      switch (GetBankFieldType(field_no))
      {
      case BANK_LL_BIN :
         memcpy( szBankFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
         length = atoi(szBankFldLen);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
         *nLength = length;
         break;

      case BANK_LLL_BIN :
         memcpy( szBankFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
         length = atoi(szBankFldLen);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
         *nLength = length;
         break;

      case BANK_FIX_BIN   :
      case BANK_FIX_ALPHA :
         length = GetBankFieldLength(field_no);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         data[length] = '\0';
         *nLength = length;
         break;
         
      case BANK_FIX_HEX :                           
         length = GetBankFieldLength(field_no);
         nBytes = HexToAscii( (unsigned char*)msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                           (unsigned char*)data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case BANK_LL_HEX :                           
         memcpy( szBankFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         length = atoi(szBankFldLen);
         nBytes = HexToAscii((unsigned char*)msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
                                                              (unsigned char*)data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case BANK_LLL_HEX :                           
         memcpy ( szBankFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         length = atoi(szBankFldLen);
         nBytes = HexToAscii((unsigned char*)msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
                                                              (unsigned char*)data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case BANK_LL_ALPHA : 
         memcpy ( szBankFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         length = atoi(szBankFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case BANK_LLL_ALPHA : 
         memcpy ( szBankFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         length = atoi(szBankFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+3, length); 
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

int AddBankField(int field_no, TSBankInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int i;
   char szBankFldLen[4];	

   memset( szBankFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
   switch (GetBankFieldType(field_no))
   {
   case BANK_FIX_ALPHA :
      if ( GetBankFieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case BANK_LL_ALPHA :
      sprintf ( szBankFldLen, "%.2d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szBankFldLen, 2);
      msgInfo->nLength += 2;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case BANK_LLL_ALPHA :
      sprintf ( szBankFldLen, "%.3d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szBankFldLen, 3);
      msgInfo->nLength += 3;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case BANK_FIX_HEX :
      if ( GetBankFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex((unsigned char*)data, (unsigned char*)msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case BANK_LL_HEX :
      sprintf ( szBankFldLen, "%.2d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szBankFldLen, 2);
      msgInfo->nLength += 2;
      nBytes = AsciiToHex( (unsigned char*)data, (unsigned char*)msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case BANK_LLL_HEX :
      sprintf ( szBankFldLen, "%.3d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szBankFldLen, 3);
      msgInfo->nLength += 3;
      nBytes = AsciiToHex((unsigned char*) data, (unsigned char*)msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case BANK_FIX_BIN :
      if ( GetBankFieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case BANK_LL_BIN :
      sprintf ( szBankFldLen, "%.2d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szBankFldLen, 2);
      msgInfo->nLength += 2;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case BANK_LLL_BIN :
      sprintf ( szBankFldLen, "%.3d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szBankFldLen, 3);
      msgInfo->nLength += 3;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   msgInfo->nLength += nBytes;
   for (i=field_no; i< MAX_BANK_FIELDS; i++)
       msgInfo->nFieldPos[i+1] = msgInfo->nLength;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   return(SUCCES);
}

int InsertBankField(int field_no, TSBankInfo *msgInfo, char *data, int nLength)
{
   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i;
   char szBankData [999];	
   char sLine [MAX_LINE_TRC];	
   char szBankFldLen[4];	

   int  nOldFldLen,nOldLen;
   char szWork    [LG_MAX];	

   memset( szBankFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   switch (GetBankFieldType(field_no))
   {
   case BANK_FIX_ALPHA :
      if ( GetBankFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy (szBankData, data, nBytes);
      break;

   case BANK_LL_ALPHA :
      sprintf ( szBankFldLen, "%.2d", nLength);	
      memcpy( szBankData,        szBankFldLen, 2);
      nBytes=2;
      memcpy( szBankData+2, data,        nLength);
      nBytes += nLength;
      break;

   case BANK_LLL_ALPHA :
      fprintf(stderr, " |%s|", data);
      sprintf ( szBankFldLen, "%.3d", nLength);	
      nBytes = 3;
      memcpy( szBankData, szBankFldLen, 3);
      memcpy( szBankData+3, data,        nLength);
      nBytes += nLength;
      break;

   case BANK_FIX_HEX :
      if ( GetBankFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex((unsigned char*)data, (unsigned char*)szBankData, nLength);
      break;

   case BANK_LL_HEX :
      sprintf ( szBankFldLen, "%.2d", nLength);	
      nBytes1 = 2;
      memcpy( szBankData, szBankFldLen, nBytes1);
      nBytes = AsciiToHex((unsigned char*) data,(unsigned char*) szBankData+2, nLength);
      nBytes += nBytes1;
      break;

   case BANK_LLL_HEX :
      sprintf ( szBankFldLen, "%.3d", nLength);	
      nBytes1 = 3;
      memcpy( szBankData, szBankFldLen, 3);
      nBytes = AsciiToHex((unsigned char*) data,(unsigned char*) szBankData+3, nLength);
      nBytes += nBytes1;
      break;

   case BANK_FIX_BIN :
      if ( GetBankFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy( szBankData, data, nBytes);
      break;

   case BANK_LL_BIN :
      sprintf ( szBankFldLen, "%.2d", nLength);	
      nBytes = 2;
      memcpy( szBankData, szBankFldLen, nBytes);
      memcpy( szBankData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case BANK_LLL_BIN :
      sprintf ( szBankFldLen, "%.3d", nLength);	
      nBytes = 3;
      memcpy( szBankData, szBankFldLen, nBytes);
      memcpy( szBankData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
      nOldFldLen = 0;
      memset(szWork, 0, sizeof(szWork));
      nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
      memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

      for (i = MAX_BANK_FIELDS-1; i > field_no; i--)
      {
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szBankData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return(SUCCES);
}

int PutBankField(int field_no, TSBankInfo *msgInfo, char *data, int nLength)
{
   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i, nOldFldLen,nOldLen;
   char szBankData [999];	
   char szWork    [LG_MAX];	
   char szBankFldLen[4];	

   memset( szBankFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      switch (GetBankFieldType(field_no))
      {
      case BANK_FIX_ALPHA :
         if ( GetBankFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy (szBankData, data, nBytes);
         break;

      case BANK_LL_ALPHA :
         sprintf ( szBankFldLen, "%.2d", nLength);	
         memcpy( szBankData,        szBankFldLen, 2);
         nBytes=2;
         memcpy( szBankData+2, data,        nLength);
         nBytes += nLength;
         break;

      case BANK_LLL_ALPHA :
         sprintf ( szBankFldLen, "%.3d", nLength);	
         nBytes = 3;
         memcpy( szBankData, szBankFldLen, 3);
         memcpy( szBankData+3, data,        nLength);
         nBytes += nLength;
         break;

      case BANK_FIX_HEX :
         if ( GetBankFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToHex((unsigned char*)data, (unsigned char*)szBankData, nLength);
         break;

      case BANK_LL_HEX :
         sprintf ( szBankFldLen, "%.2d", nLength);	
         nBytes1 = 2;
         memcpy( szBankData, szBankFldLen, nBytes1);
         nBytes = AsciiToHex((unsigned char*) data,(unsigned char*) szBankData+2, nLength);
         nBytes += nBytes1;
         break;

      case BANK_LLL_HEX :
         sprintf ( szBankFldLen, "%.3d", nLength);	
         nBytes1 = 3;
         memcpy( szBankData, szBankFldLen, 3);
         nBytes = AsciiToHex((unsigned char*) data, (unsigned char*)szBankData+3, nLength);
         nBytes += nBytes1;
         break;

      case BANK_FIX_BIN :
         if ( GetBankFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy( szBankData, data, nBytes);
         break;

      case BANK_LL_BIN :
         sprintf ( szBankFldLen, "%.2d", nLength);	
         nBytes = 2;
         memcpy( szBankData, szBankFldLen, nBytes);
         memcpy( szBankData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case BANK_LLL_BIN :
         sprintf ( szBankFldLen, "%.3d", nLength);	
         nBytes = 3;
         memcpy( szBankData, szBankFldLen, nBytes);
         memcpy( szBankData+nBytes, data, nLength);
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

      for (i = MAX_BANK_FIELDS-1; i > field_no; i--)
      {
/*         memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
                 msgInfo->sData+msgInfo->nFieldPos[i],
                 msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
*/
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szBankData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return(SUCCES);
   }
   else
      return ( InsertBankField (field_no, msgInfo, data, nLength));

}

int BankBuildMsg (char * buffer_snd, TSBankInfo *msgInfo)
{
   char szMsgType[5];
   int  nBytes;
   int  iMapCtr;

   sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
   memcpy( buffer_snd, szMsgType, 4);

   iMapCtr = 1;
   while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
      ++iMapCtr;
   memcpy( buffer_snd+4, msgInfo->sBitMap, iMapCtr*8);
   memcpy( buffer_snd+4 + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
   return( 4 + (iMapCtr*8) + msgInfo->nLength);
}

/****************************************************************************/
/**   Gestion Du Champ 55 (EMV)                                           ***/
/****************************************************************************/
int GetBankIcTag (char  *tag_name, TSTagBank *tagInfo, char *data, int *length)
{
   int  nIndice ;
   int  nLenTag ;
   int  nDigits;
   char sLine [MAX_LINE_TRC];

   if ( (nIndice = GetBankTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice])) & 0x1F == 0x1F)
      nLenTag = 2;
   else
      nLenTag = 1;
   *length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag));
   switch (GetBankTagType(nIndice) )
   {
   case BANK_TAG_FIX_BIN :
   case BANK_TAG_FIX_ALPHA :
      memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
                   *length);
      break;

   case BANK_TAG_VAR_HEX :
      nDigits = HexToAscii((unsigned char*) tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            (unsigned char*)data, 
                            (*length)*2);
      *length = (*length)*2;
      break;
   case BANK_TAG_VAR_BCD :
      nDigits = BcdToAscii((unsigned char*) tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            (unsigned char*)data, 
                            (*length)*2);
      *length = (*length)*2;
      break;

   case BANK_TAG_FIX_BCD :
      nDigits = BcdToAscii((unsigned char*) tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
							(unsigned char*)data,
                            (*length)*2);
      *length = (*length)*2;
      break;

   case BANK_TAG_VAR_BIN :
   case BANK_TAG_VAR_ALPHA :
      memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
                    *length);
      break;

   }
   return(SUCCES);
}

int AddBankIcTag (char  *tag_name, TSTagBank *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;

   if ( (nIndice = GetBankTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   AsciiToHex((unsigned char*)tag_name, (unsigned char*)tagInfo->sChipData + tagInfo->nLength, 4);
   if ( (*(tagInfo->sChipData + tagInfo->nLength)) & 0x1F == 0x1F)
      nLenTag = 2;
   else 
      nLenTag = 1;
   switch (GetBankTagType(nIndice) )
   {
   case BANK_TAG_FIX_BIN :
   case BANK_TAG_FIX_ALPHA :
      if ( GetBankTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case BANK_TAG_VAR_HEX :
      nDigits = AsciiToHex((unsigned char*) data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case BANK_TAG_VAR_BCD :
      nDigits = AsciiToBcd((unsigned char*) data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case BANK_TAG_FIX_BCD :
      if ( GetBankTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd((unsigned char*) data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case BANK_TAG_VAR_BIN :
   case BANK_TAG_VAR_ALPHA :
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

int PutBankIcTag (char  *tag_name, TSTagBank *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   int nPutPos, nTagPresent = NOT_PRESENT;
   char szLastPart  [ MAX_BANK_CHIP_LEN ];
   int nOldTagLen, i;

   if ( (nIndice = GetBankTagIndice(tag_name)) == -1)
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
      AsciiToHex((unsigned char*)tag_name, (unsigned char*)tagInfo->sChipData + tagInfo->nLength, 4);

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

   switch (GetBankTagType(nIndice) )
   {
   case BANK_TAG_FIX_BIN :
   case BANK_TAG_FIX_ALPHA :
      if ( GetBankTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case BANK_TAG_VAR_HEX :
      nDigits = AsciiToHex( (unsigned char*)data, 
                            (unsigned char*)tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case BANK_TAG_VAR_BCD :
      nDigits = AsciiToBcd( (unsigned char*)data, 
                            (unsigned char*)tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case BANK_TAG_FIX_BCD :
      if ( GetBankTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( (unsigned char*)data, 
                            (unsigned char*)tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case BANK_TAG_VAR_BIN :
   case BANK_TAG_VAR_ALPHA :
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
      for (i = nIndice + 1; i < MAX_BANK_CHIP_TAG; i++)
      {
         if (tagInfo->nPresent[i] == PRESENT)
            tagInfo->nPosTag[i] += nDigits - nOldTagLen;
      }
   }
   return(SUCCES);
}

int BankBuildIcFld (char * buffer_snd, TSTagBank *tagInfo)
{
   memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
   return(tagInfo->nLength);
}
int BankBuildHeader(char*   pNewHeader,int sLen , TSBankInfo   *BankInfo)
{ 
       char            FieldData[LG_MAX];  
       int             FieldLen;  
       sprintf(pNewHeader, "%04d",sLen + 11 );
        memcpy(pNewHeader + 4,"ISO51000",8);   
      if(GetBankField (BANK_ADTNL_RESP_DATA, BankInfo, FieldData, &FieldLen) == SUCCESS )  
         {   
             memcpy(pNewHeader + 12,FieldData,3);
         }
        else
             memcpy(pNewHeader + 12,"000",3);
			 
			 return(0);
}

