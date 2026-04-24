#include <stdio.h>
#include <stdlib.h>

#include <iso_hsid.h>
#include <hsid_param.h>
#include <hsid_define.h>
#include <tools.h>
#include <define.h>
#include <globals.h> /*HIA05052008 PCI*/


int GetHsidField(int field_no, TSSidInfo * msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
   char szIsoFldLen[4];	

   memset( szIsoFldLen, '\0', 4);	
   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      switch (GetHsidFieldType(field_no))
      {
      case HSID_LL_BIN :
         memcpy( szIsoFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
         length = atoi(szIsoFldLen);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
         *nLength = length;
         break;

      case HSID_LLL_BIN :
         memcpy( szIsoFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
         length = atoi(szIsoFldLen);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
         *nLength = length;
         break;

      case HSID_FIX_BIN   :
      case HSID_FIX_ALPHA :
         length = GetHsidFieldLength(field_no);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         data[length] = '\0';
         *nLength = length;
         break;
         
      case HSID_FIX_HEX :
         length = GetHsidFieldLength(field_no);
         nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                           data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case HSID_LL_HEX :
         memcpy( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         length = atoi(szIsoFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
                                                              data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case HSID_LLL_HEX :
         memcpy ( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         length = atoi(szIsoFldLen);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
                                                              data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case HSID_LL_ALPHA :
         memcpy ( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
         length = atoi(szIsoFldLen);
         memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case HSID_LLL_ALPHA :
         memcpy ( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
         length = atoi(szIsoFldLen);
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

int AddHsidField(int field_no, TSSidInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int i;
   char szIsoFldLen[4];	

   memset( szIsoFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
   switch (GetHsidFieldType(field_no))
   {
   case HSID_FIX_ALPHA :
      if ( GetHsidFieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case HSID_LL_ALPHA :
      sprintf ( szIsoFldLen, "%.2d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 2);
      msgInfo->nLength += 2;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case HSID_LLL_ALPHA :
      sprintf ( szIsoFldLen, "%.3d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 3);
      msgInfo->nLength += 3;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case HSID_FIX_HEX :
      if ( GetHsidFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case HSID_LL_HEX :
      sprintf ( szIsoFldLen, "%.2d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 2);
      msgInfo->nLength += 2;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case HSID_LLL_HEX :
      sprintf ( szIsoFldLen, "%.3d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 3);
      msgInfo->nLength += 3;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case HSID_FIX_BIN :
      if ( GetHsidFieldLength(field_no) != nLength)
         return(ECHEC);
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case HSID_LL_BIN :
      sprintf ( szIsoFldLen, "%.2d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 2);
      msgInfo->nLength += 2;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case HSID_LLL_BIN :
      sprintf ( szIsoFldLen, "%.3d", nLength);	
      memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 3);
      msgInfo->nLength += 3;
      memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   default :
      return(ECHEC);
   }
   msgInfo->nLength += nBytes;
   for (i=field_no; i< MAX_HSID_FIELDS; i++)
       msgInfo->nFieldPos[i+1] = msgInfo->nLength;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   return(SUCCES);
}

int InsertHsidField(int field_no, TSSidInfo *msgInfo, char *data, int nLength)
{
   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i;
   char szIsoData [999];	
   char szIsoFldLen[4];	
   int  nOldFldLen,nOldLen;
   char szWork    [LG_MAX];	


   memset( szIsoFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))   		
      return(ECHEC);

   switch (GetHsidFieldType(field_no))
   {
   case HSID_FIX_ALPHA :
      if ( GetHsidFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy (szIsoData, data, nBytes);
      break;

   case HSID_LL_ALPHA :
      sprintf ( szIsoFldLen, "%.2d", nLength);	
      memcpy( szIsoData,        szIsoFldLen, 2);
      nBytes=2;
      memcpy( szIsoData+2, data,        nLength);
      nBytes += nLength;
      break;

   case HSID_LLL_ALPHA :
      sprintf ( szIsoFldLen, "%.3d", nLength);	
      nBytes = 3;
      memcpy( szIsoData, szIsoFldLen, 3);
      memcpy( szIsoData+3, data,        nLength);
      nBytes += nLength;
      break;

   case HSID_FIX_HEX :
      if ( GetHsidFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex(data, szIsoData, nLength);
      break;

   case HSID_LL_HEX :
      sprintf ( szIsoFldLen, "%.2d", nLength);	
      nBytes1 = 2;
      memcpy( szIsoData, szIsoFldLen, nBytes1);
      nBytes = AsciiToHex( data, szIsoData+2, nLength);
      nBytes += nBytes1;
      break;

   case HSID_LLL_HEX :
      sprintf ( szIsoFldLen, "%.3d", nLength);	
      nBytes1 = 3;
      memcpy( szIsoData, szIsoFldLen, 3);
      nBytes = AsciiToHex( data, szIsoData+3, nLength);
      nBytes += nBytes1;
      break;

   case HSID_FIX_BIN :
      if ( GetHsidFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = nLength;
      memcpy( szIsoData, data, nBytes);
      break;

   case HSID_LL_BIN :
      sprintf ( szIsoFldLen, "%.2d", nLength);	
      nBytes = 2;
      memcpy( szIsoData, szIsoFldLen, nBytes);
      memcpy( szIsoData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   case HSID_LLL_BIN :
      sprintf ( szIsoFldLen, "%.3d", nLength);	
      nBytes = 3;
      memcpy( szIsoData, szIsoFldLen, nBytes);
      memcpy( szIsoData+nBytes, data, nLength);
      nBytes += nLength;
      break;

   default :
      return(ECHEC);
   }
   
  
      nOldFldLen = 0;
      memset(szWork, 0, sizeof(szWork));
      nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
      memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

      for (i = MAX_HSID_FIELDS-1; i > field_no; i--)
      {
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szIsoData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
   return(SUCCES);
}

int PutHsidField(int field_no, TSSidInfo *msgInfo, char *data, int nLength)
{
   int  nBytes   = 0;
   int  nBytes1  = 0;
   int  i, nOldFldLen,nOldLen;
   char szIsoData [999];	
   char szWork    [LG_MAX];	
   char szIsoFldLen[4];	
		
   memset( szIsoFldLen, '\0', 4);	
   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      
      switch (GetHsidFieldType(field_no))
      {
      case HSID_FIX_ALPHA :
         if ( GetHsidFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy (szIsoData, data, nBytes);
         break;

      case HSID_LL_ALPHA :
         sprintf ( szIsoFldLen, "%.2d", nLength);	
         memcpy( szIsoData,        szIsoFldLen, 2);
         nBytes=2;
         memcpy( szIsoData+2, data,        nLength);
         nBytes += nLength;
         break;

      case HSID_LLL_ALPHA :
         sprintf ( szIsoFldLen, "%.3d", nLength);	
         nBytes = 3;
         memcpy( szIsoData, szIsoFldLen, 3);
         memcpy( szIsoData+3, data,        nLength);
         nBytes += nLength;
         break;

      case HSID_FIX_HEX :
         if ( GetHsidFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToHex(data, szIsoData, nLength);
         break;

      case HSID_LL_HEX :
         sprintf ( szIsoFldLen, "%.2d", nLength);	
         nBytes1 = 2;
         memcpy( szIsoData, szIsoFldLen, nBytes1);
         nBytes = AsciiToHex( data, szIsoData+2, nLength);
         nBytes += nBytes1;
         break;

      case HSID_LLL_HEX :
         sprintf ( szIsoFldLen, "%.3d", nLength);	
         nBytes1 = 3;
         memcpy( szIsoData, szIsoFldLen, 3);
         nBytes = AsciiToHex( data, szIsoData+3, nLength);
         nBytes += nBytes1;
         break;

      case HSID_FIX_BIN :
         if ( GetHsidFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = nLength;
         memcpy( szIsoData, data, nBytes);
         break;

      case HSID_LL_BIN :
         sprintf ( szIsoFldLen, "%.2d", nLength);	
         nBytes = 2;
         memcpy( szIsoData, szIsoFldLen, nBytes);
         memcpy( szIsoData+nBytes, data, nLength);
         nBytes += nLength;
         break;

      case HSID_LLL_BIN :
         sprintf ( szIsoFldLen, "%.3d", nLength);	
         nBytes = 3;
         memcpy( szIsoData, szIsoFldLen, nBytes);
         memcpy( szIsoData+nBytes, data, nLength);
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

      for (i = MAX_HSID_FIELDS-1; i > field_no; i--)
      {
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szIsoData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return(SUCCES);
   }
   else
   {
    
      return ( InsertHsidField (field_no, msgInfo, data, nLength));
   }
}


int BuildBuffer (char * buffer_snd, TSSidInfo *msgInfo, char *BadField,int *iMapCtrOut)
{

		char	szMsgType[5];
		int		iMapCtr;
		char	lengthBuffer[5];
		char	szBadField[4];
		int		nLength;
		int		nOffset;

		memset(szMsgType ,0,sizeof(szMsgType));
		memset(lengthBuffer ,0,sizeof(lengthBuffer));
		memset(szBadField ,0,sizeof(szBadField));

		memcpy(szBadField ,BadField,3);

		nOffset = HSID_LENGTH_LEN;
		sprintf(buffer_snd + nOffset   ,"ISO70100%.3s",szBadField); nOffset+=HSID_HDR_LEN;


		sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
		memcpy( buffer_snd+nOffset, szMsgType, HSID_MSG_TYPE_LEN);nOffset+=HSID_MSG_TYPE_LEN;

		iMapCtr = 1;
		while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
			++iMapCtr;

		memcpy( buffer_snd + nOffset, msgInfo->sBitMap, iMapCtr*8); nOffset+=(iMapCtr*8);
		memcpy( buffer_snd + nOffset, msgInfo->sData, msgInfo->nLength); nOffset+=msgInfo->nLength;
		nLength = nOffset;
		sprintf(lengthBuffer,"%04d",nLength - HSID_LENGTH_LEN);
		memcpy( buffer_snd,lengthBuffer,HSID_LENGTH_LEN);

		*iMapCtrOut = iMapCtr ;

		return(nLength);
}


int HsidBuildMsg (int nIndiceCtx, char * buffer_snd, TSSidInfo *msgInfo, char *BadField)
{
   
		int		iMapCtr;
		int		nLength;
		char	sMacData[ 8 + 1 ];
		char	sLine [ MAX_LINE_TRC ];

		memset(sMacData, 0, sizeof(sMacData));

		nLength = BuildBuffer(buffer_snd,msgInfo,BadField,&iMapCtr);

		if( g_mac_mng_flag == MAC_MNG_FLAG_N )
		{
			return nLength;
		}


		if ( CalculMAC (nIndiceCtx ,
						ResourceId ,
						buffer_snd ,
						nLength ,
						iMapCtr ,
						sMacData) != OK )
		{
			trace_event("Error Calcul Mac ", ERROR );
			trace_event("==> SidBuildMsg(NOK)", ERROR );
			return nLength;
		}

        PutHsidField(HSID_MAC2,msgInfo,sMacData,8);

		nLength = BuildBuffer(buffer_snd,msgInfo,BadField,&iMapCtr);

		return(nLength);

}

/****************************************************************************/
/**   Gestion Du Champ 55 (EMV)                                           ***/
/****************************************************************************/
int GetHsidIcTag (char  *tag_name, TSTagSid *tagInfo, char *data, int *length)
{
   int  nIndice ;
   int  nLenTag ;
   int  nDigits;
   char sLine [MAX_LINE_TRC];

   if ( (nIndice = GetHsidTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
      return(ECHEC);
   
   if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice])) & 0x1F == 0x1F)
      nLenTag = 2;
   else
      nLenTag = 1;
   *length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag));
   switch (GetHsidTagType(nIndice) )
   {
   case HSID_TAG_FIX_BIN :
   case HSID_TAG_FIX_ALPHA :
      memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
                   *length);
      break;

   case HSID_TAG_VAR_HEX :
      nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;
   case HSID_TAG_VAR_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
                            data, 
                            (*length)*2);
      *length = (*length)*2;
      break;

   case HSID_TAG_FIX_BCD :
      nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
                                               nLenTag+1, 
							data,
                            (*length)*2);
      *length = (*length)*2;
      break;

   case HSID_TAG_VAR_BIN :
   case HSID_TAG_VAR_ALPHA :
      memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
                    *length);
      break;

   }
   return(SUCCES);
}

int AddHsidIcTag (char  *tag_name, TSTagSid *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;

   if ( (nIndice = GetHsidTagIndice(tag_name)) == -1)
      return(ECHEC);
   
   if (tagInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);
   
   AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);
   if ( (*(tagInfo->sChipData + tagInfo->nLength)) & 0x1F == 0x1F)
      nLenTag = 2;
   else 
      nLenTag = 1;
   switch (GetHsidTagType(nIndice) )
   {
   case HSID_TAG_FIX_BIN :
   case HSID_TAG_FIX_ALPHA :
      if ( GetHsidTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case HSID_TAG_VAR_HEX :
      nDigits = AsciiToHex( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case HSID_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case HSID_TAG_FIX_BCD :
      if ( GetHsidTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
                            length);
      break;

   case HSID_TAG_VAR_BIN :
   case HSID_TAG_VAR_ALPHA :
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

int PutHsidIcTag (char  *tag_name, TSTagSid *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   int nPutPos, nTagPresent = NOT_PRESENT;
   char szLastPart  [ MAX_HSID_CHIP_LEN ];
   int nOldTagLen, i;

   if ( (nIndice = GetHsidTagIndice(tag_name)) == -1)
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

   switch (GetHsidTagType(nIndice) )
   {
   case HSID_TAG_FIX_BIN :
   case HSID_TAG_FIX_ALPHA :
      if ( GetHsidTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case HSID_TAG_VAR_HEX :
      nDigits = AsciiToHex( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case HSID_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case HSID_TAG_FIX_BCD :
      if ( GetHsidTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data, 
                            tagInfo->sChipData + nPutPos + nLenTag+1, 
                            length);
      break;

   case HSID_TAG_VAR_BIN :
   case HSID_TAG_VAR_ALPHA :
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
      for (i = nIndice + 1; i < MAX_HSID_CHIP_TAG; i++)
      {
         if (tagInfo->nPresent[i] == PRESENT)
            tagInfo->nPosTag[i] += nDigits - nOldTagLen;
      }
   }
   return(SUCCES);
}

int HsidBuildIcFld (char * buffer_snd, TSTagSid *tagInfo)
{
   memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
   return(tagInfo->nLength);
}


