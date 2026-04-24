/*
-----------------------------------------------------------------------------------------------------------------------------------------
-- Description : 
	-------------------------------------------------------------------------------------------------------------------------------------
	
-----------------------------------------------------------------------------------------------------------------------------------------
-- Modification History : 
	-------------------------------------------------------------------------------------------------------------------------------------
	-- Version    Date          	Author       		Jalon                     Activity                          Comment
	-------------------------------------------------------------------------------------------------------------------------------------
	-- V3.0.0     29/12/2014    	HPS                                                                           	Initial version
-----------------------------------------------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <iso_ist.h>
#include <ist_fields.h>
#include <tools.h>
#include <define.h>




typedef struct SICCTagData
{
    char  *sTagName;
    char  *sTagLabel;
} TSICCTagData;

TSICCTagData iccTagData[] ={
 {"50  "  , "APPLICATION_LABEL        ..."},
 {"57  "  , "TRACK_2_DATA             ..."},
 {"5A  "  , "APPL_PRIM_ACCOUNT        ..."},
 {"71  "  , "ISS_SCRIPT_TEMPLATE_1    ..."},
 {"72  "  , "ISS_SCRIPT_TEMPLATE_2    ..."},
 {"82  "  , "APP_INTERCH_PROFILE      ..."},
 {"84  "  , "DEDICATES_FILE_NAME      ..."},
 {"86  "  , "ISSUER_SCRIPT_COMMAND    ..."},
 {"8A  "  , "AUTH_RESPONSE_CODE       ..."},
 {"8C  "  , "CARD_RISK_MANAG_CDOL1    ..."},
 {"91  "  , "ISSUER_AUTHENT_DATA      ..."},
 {"95  "  , "TERMINAL_VERIF_RESULTS   ..."},
 {"9A  "  , "TRANSACTION_DATE         ..."},
 {"9C  "  , "TRANSACTION_TYPE         ..."},
 {"5F34"  , "APP_PAN_SEQ_NUMBER       ..."},
 {"5F2A"  , "TRANSACTION_CUR_CODE     ..."},
 {"9F02"  , "TRANSACTION_AMOUNT       ..."},
 {"9F03"  , "OTHER_AMOUNT             ..."},
 {"9F06"  , "APP_SELECTED             ..."},
 {"9F09"  , "TERM_APPL_VERSION_NBR    ..."},
 {"9F10"  , "ISSUER_APPL_DATA         ..."},
 {"9F1A"  , "TERMINAL_COUNTRY_CODE    ..."},
 {"9F1E"  , "INTER_DEVICE_SERIAL_NBR  ..."},
 {"9F26"  , "APPLICATION_CRYPTOGRAM   ..."},
 {"9F27"  , "CRYPTOGRAM_INF_DATA      ..."},
 {"9F33"  , "TERMINAL_CAPABILITIES    ..."},
 {"9F34"  , "CVM_RESULTS              ..."}, 
 {"9F35"  , "TERMINAL_TYPE            ..."},
 {"9F36"  , "APP_TRANS_COUNTER        ..."},
 {"9F37"  , "UNPREDICTABLE_NUMBER     ..."},
 {"9F41"  , "TRANS_SEQ_NUMBER         ..."},
 {"9F53"  , "TRANS_CATEGORY_CODE      ..."},
};


int GetIstField(int field_no, TSIstInfo *msgInfo, char *data, int *nLength)
{
   int length;
   int nBytes;
    char    szField[256];
    char    szBcdFldLen[4];
	char    sLine[256];

	memset (sLine, 0, sizeof(sLine));
	memset (szField, 0, sizeof(szField));
	memset (szBcdFldLen, 0, sizeof(szBcdFldLen));
	
   if (CheckBit (msgInfo->sBitMap, field_no))
   {
      switch (GetIstFieldType(field_no))
      {
      case IST_VAR_BIN :
      case IST_VAR_BITMAP :
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no]+1,length);
         *nLength = length;
         break;

      case IST_FIX_BIN :
      case IST_FIX_ALPHA :                           
         length = GetIstFieldLength(field_no);
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
         *nLength = length;	
         break;

      case IST_FIX_HEX :                           
         length = GetIstFieldLength(field_no);
         nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                             data, length);
         data[length] = '\0';
         *nLength = length;
         break;

      case IST_VAR_HEX :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;
		 
      case IST_LLL_BIN :  
         memcpy( szBcdFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no],3);	  
         length = atoi (szBcdFldLen);
		 nBytes = length;
         memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
         *nLength = length;
         data[length] = '\0';
         break;

      case IST_FIX_BCD :                           
         length = GetIstFieldLength(field_no);
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no], 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case IST_VAR_BCD :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            data, length); 
         data[length] = '\0';
         *nLength = length;
         break;

      case IST_VAR_SBCD :                           
         length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
         nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
                                                            data, 2*length); 
         data[2*length] = '\0';
         *nLength = 2*length;
         break;

	case IST_VAR_ALPHA :
	   memset(szField,         '\0', sizeof(szField));
	   memcpy ( szField, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
            length = atoi(szField);
            memcpy(data,msgInfo->sData+msgInfo->nFieldPos[field_no]+2,length);
            data[length] = '\0';
            *nLength = length;

		
            break;
	case IST_LLL_ALPHA :
	    memset(szField,         '\0', sizeof(szField));
	    memcpy ( szField, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
            length = atoi(szField);
            memcpy(data,msgInfo->sData+msgInfo->nFieldPos[field_no]+3,length);
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

int AddIstField(int field_no, TSIstInfo *msgInfo, char *data, int nLength)
{
   int i;
   
   int nBytes  = 0;
    char    szField[256];
    char    szPosFldLen[4];
   
   memset (szPosFldLen, 0 , sizeof(szPosFldLen));
   memset (szField, 	0 , sizeof(szField));

   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   msgInfo->nFieldPos[field_no] = msgInfo->nLength;
   switch (GetIstFieldType(field_no))
   {
   case IST_FIX_BIN :
   case IST_FIX_ALPHA :
      nBytes = GetIstFieldLength(field_no);
      memcpy( msgInfo->sData+msgInfo->nLength, data, nBytes);
      break;

   case IST_LLL_BIN :
      sprintf ( szPosFldLen, "%.3d", nLength);
	  memcpy( msgInfo->sData+msgInfo->nLength, szPosFldLen, 3);
	  msgInfo->nLength = msgInfo->nLength + 3;
	  memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;
	  
   case IST_VAR_BIN :
   case IST_VAR_BITMAP :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
      nBytes = nLength;
      break;

   case IST_FIX_BCD :
      if ( GetIstFieldLength(field_no) != nLength )
         return(ECHEC);
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case IST_FIX_HEX :
      if ( GetIstFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

/*
   case IST_FIX_ALPHA :
      if ( GetIstFieldLength(field_no) != nLength)
         return(ECHEC);
      AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;
*/

   case IST_VAR_HEX :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case IST_VAR_BCD :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

   case IST_VAR_SBCD :
      *(msgInfo->sData + msgInfo->nLength) = (nLength+1)/2;
      msgInfo->nLength++;
      nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
      break;

/*
   case IST_VAR_ALPHA :
      *(msgInfo->sData + msgInfo->nLength) = nLength;
      msgInfo->nLength++;
      AsciiToEbcdic(data, msgInfo->sData+msgInfo->nLength, nLength);
      nBytes = nLength;
      break;
*/
   case IST_VAR_ALPHA :
		memset(szField,         '\0', sizeof(szField));
		sprintf ( szField, "%.2d", nLength);
        memcpy( msgInfo->sData+msgInfo->nLength, szField, 2);
        msgInfo->nLength += 2;
        memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
        nBytes = nLength;
        break;

 case IST_LLL_ALPHA : 
		memset(szField,         '\0', sizeof(szField));
		sprintf ( szField, "%.3d", nLength);
        memcpy( msgInfo->sData+msgInfo->nLength, szField, 3);
        msgInfo->nLength += 3;
        memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
        nBytes = nLength;
        break;
			
   default :
      return(ECHEC);
   }
   msgInfo->nLength += nBytes;
   for (i=field_no; i< MAX_IST_FIELDS; i++)
       msgInfo->nFieldPos[i+1] = msgInfo->nLength;
   SetBit(msgInfo->sBitMap, field_no+1);
   /* if ( (field_no >64) / * && (field_no < 128) * /) */
   
   if ( (field_no >64) && (field_no < 128))
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int InsertIstField(int field_no, TSIstInfo *msgInfo, char *data, int nLength)
{
   int nBytes  = 0;
   int i;
   int szBcdFldLen;
   int nDigits;
   char sData [999];
   char szPosFldLen [3 +1];
   char    szField[256];

	memset (szPosFldLen, 0, sizeof (szPosFldLen));
	memset (sData,       0, sizeof (sData));
        memset (szField,     0 , sizeof(szField));
	
   if ( CheckBit( msgInfo->sBitMap, field_no))
      return(ECHEC);

   switch (GetIstFieldType(field_no))
   {
   case IST_FIX_BIN :
   case IST_FIX_ALPHA :
      nBytes = GetIstFieldLength(field_no);
      memcpy( sData, data, nBytes);
      break;


	 
   case IST_LLL_BIN :
      sprintf ( szPosFldLen, "%.3d", nLength);
      memcpy( sData , szPosFldLen, 3);
      memcpy( sData+3, data, nLength);
      nBytes = nLength +3;
      break;
	  
   case IST_VAR_BIN :
   case IST_VAR_BITMAP :
   case IST_VAR_ALPHA :
      *(sData) = nLength;
      nBytes = 1;
      memcpy( sData+1, data, nLength);
      nBytes += nLength;
      break;


   case IST_FIX_BCD :
      if ( GetIstFieldLength(field_no) != nLength )
         return(ECHEC);
      nBytes = AsciiToBcd( data, sData, nLength);
      break;

   case IST_FIX_HEX :
      if ( GetIstFieldLength(field_no) != nLength)
         return(ECHEC);
      nBytes = AsciiToHex( data, sData, nLength);
      break;

   case IST_VAR_SBCD :
      *(sData) = (nLength+1)/2;
      nBytes = AsciiToBcd( data, sData+1, nLength);
      nBytes++;
      break;

   case IST_VAR_BCD :
      *(sData) = nLength;
      nBytes = AsciiToBcd( data, sData+1, nLength);
      nBytes++;
      break;

   case IST_VAR_HEX :
      *(sData) = nLength;
      nBytes = AsciiToHex( data, sData+1, nLength);
      nBytes++;
      break;

    case IST_LLL_ALPHA : 
	sprintf ( szField, "%.3d", nLength);
        memcpy( msgInfo->sData+msgInfo->nLength, szField, 3);
        msgInfo->nLength += 3;
        memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
        nBytes = nLength;
	break;

   default :
      return(ECHEC);
   }
   for (i = MAX_IST_FIELDS-1; i > field_no; i--)
   {
      memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
              msgInfo->sData+msgInfo->nFieldPos[i],
              msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
      msgInfo->nFieldPos[i+1] += nBytes;
   }
   memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], sData, nBytes);
   msgInfo->nFieldPos[field_no+1] += nBytes;
   msgInfo->nLength += nBytes;
   SetBit(msgInfo->sBitMap, field_no+1);
   if ( (field_no >64) /* && (field_no < 128)*/)
      SetBit(msgInfo->sBitMap, 1);
   if (field_no >= 128)
      SetBit(msgInfo->sBitMap, 65);
   return(SUCCES);
}

int PutIstField(int field_no, TSIstInfo *msgInfo, char *data, int nLength)
{
   char         sData [999];	
   char         szWork    [LG_MAX];	
   char         szIsoFldLen[4];	
   char         szField[256];	
   int          nBytes   = 0;
   int          nBytes1  = 0;
   int          i, nOldFldLen,nOldLen;
   
   memset(sData,         '\0', sizeof(sData));
   memset(szWork,        '\0', sizeof(szWork));
   memset(szIsoFldLen,   '\0', sizeof(szIsoFldLen));
   memset(szField,       '\0', sizeof(szField));
   

   if ( CheckBit( msgInfo->sBitMap, field_no))
   {
      switch (GetIstFieldType(field_no))
      {
      case IST_FIX_BIN :
      case IST_FIX_ALPHA :
         nBytes = GetIstFieldLength(field_no);
         memcpy( sData, data, nBytes);
         break;

      case IST_VAR_BIN :
      case IST_VAR_BITMAP :
      case IST_VAR_ALPHA :
      case IST_LLL_ALPHA :
         *(sData) = nLength;
         nBytes = 1;
         memcpy( sData+1, data, nLength);
         nBytes += nLength;
         break;

      case IST_FIX_BCD :
         if ( GetIstFieldLength(field_no) != nLength )
            return(ECHEC);
         nBytes = AsciiToBcd( data, sData, nLength);
         break;

      case IST_FIX_HEX :
         if ( GetIstFieldLength(field_no) != nLength)
            return(ECHEC);
         nBytes = AsciiToHex( data, sData, nLength);
         break;

      case IST_VAR_SBCD :
         *(sData) = (nLength+1)/2;
         nBytes = AsciiToBcd( data, sData+1, nLength);
         nBytes++;
         break;

      case IST_VAR_BCD :
         *(sData) = nLength;
         nBytes = AsciiToBcd( data, sData+1, nLength);
         nBytes++;
         break;

      case IST_VAR_HEX :
         *(sData) = nLength;
         nBytes = AsciiToHex( data, sData+1, nLength);
         nBytes++;
         break;
/*
    case IST_LLL_ALPHA : 
	sprintf ( szField, "%.3d", nLength);
        memcpy( msgInfo->sData+msgInfo->nLength, szField, 3);
        msgInfo->nLength += 3;
        memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
        nBytes = nLength;
	break;
*/

      default :
         return(ECHEC);
      }

      nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
                   msgInfo->nFieldPos[field_no];
      memset(szWork, 0, sizeof(szWork));
      nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
      memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

      for (i = MAX_IST_FIELDS-1; i > field_no; i--)
      {
         msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
      }
      memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], sData, nBytes);
      msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
      msgInfo->nLength += nBytes-nOldFldLen;
      SetBit(msgInfo->sBitMap, field_no+1);
      memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
      if ( (field_no >64) && (field_no < 128))
         SetBit(msgInfo->sBitMap, 1);
      return(SUCCES);
   }
   else
      return ( InsertIstField (field_no, msgInfo, data, nLength));
}

int BuildMsgBuffer (char * buffer_snd, TSIstInfo *msgInfo)
{
   char szMsgType[5];
   int  nBytes;
   int  offset;
   int  iMapCtr;


   offset = 0;

   memset(szMsgType, '\0', sizeof(szMsgType));
   sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
   memcpy( buffer_snd, szMsgType, 4);
   offset += 4 ;

   iMapCtr = 1;
   while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80))
      ++iMapCtr;
   memcpy( buffer_snd+offset, msgInfo->sBitMap, iMapCtr*8);
   memcpy( buffer_snd+offset + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
   return( offset + (iMapCtr*8) + msgInfo->nLength);

}



int AddIstIcTag (char  *tag_name, TSTagIst *tagInfo, char *data, int length)
{
   int nIndice;
   int nLenTag;
   int nLength;
   int nDigits;
   
   if ( (nIndice = GetIstTagIndice(tag_name)) == -1)
      return(ECHEC);

   if (tagInfo->nPresent[nIndice] == PRESENT)
      return(ECHEC);


   AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);


  
   if (  (unsigned char) (*(tagInfo->sChipData + tagInfo->nLength) & 0x1F ) ==  (unsigned char) 0x1F)
      nLenTag = 2;
   else
      nLenTag = 1;

   switch (GetIstTagType(nIndice) )
   {
   case IST_TAG_FIX_BIN :
   case IST_TAG_FIX_ALPHA :
      if ( GetIstTagLength(nIndice) != length)
         return(ECHEC);
      memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
      nDigits = length;
      break;

   case IST_TAG_VAR_HEX :
      nDigits = AsciiToHex( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
                            length);
      break;
   case IST_TAG_VAR_BCD :
      nDigits = AsciiToBcd( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
                            length);
      break;

   case IST_TAG_FIX_BCD :
      if ( GetIstTagLength(nIndice) != length)
         return(ECHEC);
      nDigits = AsciiToBcd( data,
                            tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
                            length);
      break;

   case IST_TAG_VAR_BIN :
   case IST_TAG_VAR_ALPHA :
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

int IstBuildIcFld (char * buffer_snd, TSTagIst *tagInfo)
{
   memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
   return(tagInfo->nLength);
}

int GetIccTagLabl (char * sTagName, char * sTagLabl)
{
    int i=0;
    
    for (i=0; i<(sizeof(iccTagData)/sizeof(iccTagData[0])); i++)
    {
      if (memcmp(iccTagData[i].sTagName, sTagName, strlen(sTagName)) ==0)
      {
       memcpy(sTagLabl,iccTagData[i].sTagLabel,strlen(iccTagData[i].sTagLabel));
       break;
      }
    }
    
    if (strlen(sTagLabl) ==0 )
    {
       memcpy(sTagLabl,sTagName,strlen(sTagName));
    }
    
    return(OK);
}
