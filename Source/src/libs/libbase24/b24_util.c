/*#include <base24.h>*/
#include <stdarg.h>
#include <base24/iso_base24.h>
#include <base24/base24_define.h>
#include <define.h>
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
int GetBase24Field(int field_no, TSBase24Info *msgInfo, char *data, int *nLength)
{
    int     length;
    int     nBytes;
    char    szField[256];


    if (CheckBit (msgInfo->sBitMap, field_no))
    {

        memset(szField,'\0',sizeof(szField));

        switch (GetBase24FieldType(field_no,msgInfo->nProductIndicator))
        {
        case BASE24_FIX_BIN :
        case BASE24_FIX_BITMAP:
            length = GetBase24FieldLength(field_no,msgInfo->nProductIndicator);
            memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
            *nLength = length;
            break;

        case BASE24_FIX_HEX :
            length = GetBase24FieldLength(field_no,msgInfo->nProductIndicator);
            nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no],
                                                             data, length);
            data[length] = '\0';
            *nLength = length;
            break;

        case BASE24_FIX_BCD :
            length = GetBase24FieldLength(field_no,msgInfo->nProductIndicator);
            nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no],
                                                            data, length);
            data[length] = '\0';
            *nLength = length;
            break;

        case BASE24_FIX_ALPHA :
            length = GetBase24FieldLength(field_no,msgInfo->nProductIndicator);
            memcpy(data,msgInfo->sData+msgInfo->nFieldPos[field_no],length);
            data[length] = '\0';
            *nLength = length;
            break;

        case BASE24_VAR_BIN :
        case BASE24_VAR_BITMAP :
            length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < length )
                return ECHEC;
            memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no]+1,length);
            *nLength = length;
            break;

        case BASE24_VAR_HEX :
            length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < (length*2) )
                return ECHEC;
            nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1,
                                                            data, length);
            data[length] = '\0';
            *nLength = length;
            break;

        case BASE24_VAR_BCD :
            length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < (length*2) )
                return ECHEC;
            nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1,
                                                            data, length);
            data[length] = '\0';
            *nLength = length;
            break;

        case BASE24_VAR_SBCD :
            length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < (length*2) )
                return ECHEC;
            nBytes = BcdToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1,
                                                        data, 2*length);
            data[2*length] = '\0';
            *nLength = 2*length;
            break;

        case BASE24_VAR_ALPHA :
            length = (unsigned char)(*(msgInfo->sData + msgInfo->nFieldPos[field_no]));
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < length )
                return ECHEC;
            memcpy(data,msgInfo->sData+msgInfo->nFieldPos[field_no]+1,length);
            data[length] = '\0';
            *nLength = length;
            break;
        case BASE24_LL_ALPHA :
            memcpy ( szField, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
            length = atoi(szField);
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < length )
                return ECHEC;
            memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length);
            data[length] = '\0';
            *nLength = length;
            break;

        case BASE24_LLL_ALPHA :
            memcpy ( szField, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
            length = atoi(szField);
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < length )
                return ECHEC;
            memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+3, length);
            data[length] = '\0';
            *nLength = length;
            break;
        case BASE24_LL_HEX :
            memcpy( szField, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
            length = atoi(szField);
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < (length*2) )
                return ECHEC;
            nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2,
                                                              data, length);
            data[length] = '\0';
            *nLength = length;
            break;

        case BASE24_LLL_HEX :
            memcpy ( szField, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
            length = atoi(szField);
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < (length*2) )
                return ECHEC;
            nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3,
                                                              data, length);
            data[length] = '\0';
            *nLength = length;
            break;

        case BASE24_LL_BIN :
            memcpy( szField, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
            length = atoi(szField);
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < length )
                return ECHEC;
            memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
            *nLength = length;
            break;

        case BASE24_LLL_BIN :
            memcpy( szField, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
            length = atoi(szField);
            if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < length )
                return ECHEC;
            memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
            *nLength = length;
            break;

        default :
            return(ECHEC);
            break;
      }

	  if( (*nLength) == 0 )
		  return(ECHEC);

      return(SUCCES);
   }
   return(ECHEC);
}
/******************************************************************************/
int AddBase24Field(int field_no, TSBase24Info *msgInfo, char *data, int nLength)
{

    int i,nBytes  = 0;
    char    szField[256];


    if ( CheckBit( msgInfo->sBitMap, field_no))
        return(ECHEC);

	if( nLength == 0 )
		return OK;

    if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < nLength )
        return ECHEC;

    msgInfo->nFieldPos[field_no] = msgInfo->nLength;

    switch (GetBase24FieldType(field_no,msgInfo->nProductIndicator))
    {
    case BASE24_FIX_BIN :
    case BASE24_FIX_BITMAP :
        nBytes = GetBase24FieldLength(field_no,msgInfo->nProductIndicator);
        memcpy( msgInfo->sData+msgInfo->nLength, data, nBytes);
        break;

    case BASE24_FIX_BCD :
        if ( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) != nLength )
            return(ECHEC);
        nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
        break;

    case BASE24_FIX_HEX :
        if ( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) != nLength)
            return(ECHEC);
        nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
        break;

    case BASE24_FIX_ALPHA :
        if ( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) != nLength)
            return(ECHEC);
        memcpy(msgInfo->sData+msgInfo->nLength,data,nLength);
        nBytes = nLength;
        break;

    case BASE24_VAR_BIN :
    case BASE24_VAR_BITMAP :
        *(msgInfo->sData + msgInfo->nLength) = nLength;
        msgInfo->nLength++;
        memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
        nBytes = nLength;
        break;

    case BASE24_VAR_HEX :
        *(msgInfo->sData + msgInfo->nLength) = nLength;
        msgInfo->nLength++;
        nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
        break;

    case BASE24_VAR_BCD :
        *(msgInfo->sData + msgInfo->nLength) = nLength;
        msgInfo->nLength++;
        nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
        break;

    case BASE24_VAR_SBCD :
        *(msgInfo->sData + msgInfo->nLength) = (nLength+1)/2;
        msgInfo->nLength++;
        nBytes = AsciiToBcd( data, msgInfo->sData+msgInfo->nLength, nLength);
        break;

    case BASE24_VAR_ALPHA :
        *(msgInfo->sData + msgInfo->nLength) = nLength;
        msgInfo->nLength++;
        memcpy(msgInfo->sData+msgInfo->nLength,data,nLength);
        nBytes = nLength;
        break;
    case BASE24_LL_ALPHA :
    case BASE24_LL_BIN :
        sprintf ( szField, "%.2d", nLength);
        memcpy( msgInfo->sData+msgInfo->nLength, szField, 2);
        msgInfo->nLength += 2;
        memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
        nBytes = nLength;
        break;

    case BASE24_LLL_ALPHA :
    case BASE24_LLL_BIN :
        sprintf ( szField, "%.3d", nLength);
        memcpy( msgInfo->sData+msgInfo->nLength, szField, 3);
        msgInfo->nLength += 3;
        memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
        nBytes = nLength;
        break;
    case BASE24_LL_HEX :
        sprintf ( szField, "%.2d", nLength);
        memcpy( msgInfo->sData+msgInfo->nLength, szField, 2);
        msgInfo->nLength += 2;
        nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
        break;

    case BASE24_LLL_HEX :
        sprintf ( szField, "%.3d", nLength);
        memcpy( msgInfo->sData+msgInfo->nLength, szField, 3);
        msgInfo->nLength += 3;
        nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
        break;

    default :
        return(ECHEC);
    }

    msgInfo->nLength += nBytes;
    for (i=field_no; i< MAX_BASE24_FIELDS; i++)
        msgInfo->nFieldPos[i+1] = msgInfo->nLength;

    SetBit(msgInfo->sBitMap, field_no+1);

    if ( (field_no >64) /* && (field_no < 128) */)
        SetBit(msgInfo->sBitMap, 1);
    if (field_no >= 128)
        SetBit(msgInfo->sBitMap, 65);
    return(SUCCES);
}
/******************************************************************************/
int InsertBase24Field(int field_no, TSBase24Info *msgInfo, char *data, int nLength)
{
    int nBytes  = 0;
    int i;
    char szFieldData    [FIELD_MAX_LEN];
    int  nOldLen;
    char szWork   [LG_MAX];

    if ( CheckBit( msgInfo->sBitMap, field_no))
        return(ECHEC);

    return PutBase24Field(field_no,msgInfo,data,nLength);
}
/******************************************************************************/
int PutBase24Field(int field_no, TSBase24Info *msgInfo, char *data, int nLength)
{

    int     nBytes   = 0;
    int     nBytes1  = 0;
    int     i, nOldFldLen,nOldLen;
    char    szWork    [LG_MAX];
    char    szFieldLen[4];
    char    szFieldData[FIELD_MAX_LEN];

	if( nLength == 0 )
		return OK;

    if( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) < nLength )
        return ECHEC;

    memset(szFieldLen,'\0',sizeof(szFieldLen));
    memset(szFieldData,'\0',sizeof(szFieldData));
    memset(szWork,'\0',sizeof(szWork));

    switch (GetBase24FieldType(field_no,msgInfo->nProductIndicator))
    {
    case BASE24_FIX_BIN :
        nBytes = GetBase24FieldLength(field_no,msgInfo->nProductIndicator);
        memcpy( szFieldData, data, nBytes);
        break;

    case BASE24_VAR_BIN :
    case BASE24_VAR_BITMAP :
        *(szFieldData) = nLength;
        nBytes = 1;
        memcpy( szFieldData+1, data, nLength);
        nBytes += nLength;
        break;

    case BASE24_FIX_BCD :
        if ( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) != nLength )
            return(ECHEC);
        nBytes = AsciiToBcd( data, szFieldData, nLength);
        break;

    case BASE24_FIX_HEX :
        if ( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) != nLength)
            return(ECHEC);
        nBytes = AsciiToHex( data, szFieldData, nLength);
        break;

    case BASE24_FIX_ALPHA :
        if ( GetBase24FieldLength(field_no,msgInfo->nProductIndicator) != nLength)
            return(ECHEC);
        memcpy(szFieldData,data,nLength);
        nBytes = nLength;
        break;

    case BASE24_VAR_SBCD :
        *(szFieldData) = (nLength+1)/2;
        nBytes = AsciiToBcd( data, szFieldData+1, nLength);
        nBytes++;
        break;

    case BASE24_VAR_BCD :
        *(szFieldData) = nLength;
        nBytes = AsciiToBcd( data, szFieldData+1, nLength);
        nBytes++;
        break;

    case BASE24_VAR_HEX :
        *(szFieldData) = nLength;
        nBytes = AsciiToHex( data, szFieldData+1, nLength);
        nBytes++;
        break;

    case BASE24_VAR_ALPHA :
        *(szFieldData) = nLength;
        nBytes =1;
        memcpy(szFieldData + 1,data,nLength);
        nBytes += nLength;
        break;
    case BASE24_LL_ALPHA :
    case BASE24_LL_BIN :
        sprintf ( szFieldLen, "%.2d", nLength);
        memcpy( szFieldData,        szFieldLen, 2);
        nBytes=2;
        memcpy( szFieldData+2, data,        nLength);
        nBytes += nLength;
        break;

    case BASE24_LLL_ALPHA :
    case BASE24_LLL_BIN :
        sprintf ( szFieldLen, "%.3d", nLength);
        nBytes = 3;
        memcpy( szFieldData, szFieldLen, 3);
        memcpy( szFieldData+3, data,        nLength);
        nBytes += nLength;
        break;
    case BASE24_LL_HEX :
        sprintf ( szFieldLen, "%.2d", nLength);
        nBytes1 = 2;
        memcpy( szFieldData, szFieldLen, nBytes1);
        nBytes = AsciiToHex( data, szFieldData+2, nLength);
        nBytes += nBytes1;
        break;

    case BASE24_LLL_HEX :
        sprintf ( szFieldLen, "%.3d", nLength);
        nBytes1 = 3;
        memcpy( szFieldData, szFieldLen, 3);
        nBytes = AsciiToHex( data, szFieldData+3, nLength);
        nBytes += nBytes1;
        break;

    default :
        return(ECHEC);
    }

    nOldFldLen = msgInfo->nFieldPos[field_no +1] -
           msgInfo->nFieldPos[field_no];
    memset(szWork, 0, sizeof(szWork));
    nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
    memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

    for (i = MAX_BASE24_FIELDS-1; i > field_no; i--)
    {
        msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
    }

    memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szFieldData, nBytes);
    
	msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
    msgInfo->nLength += nBytes-nOldFldLen;
    SetBit(msgInfo->sBitMap, field_no+1);
    memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
    if ( (field_no >64) && (field_no < 128))
        SetBit(msgInfo->sBitMap, 1);
    return(SUCCES);

}
/******************************************************************************/
/*
int Base24BuildMsg (char * buffer_snd, TSBase24Info *msgInfo)
{
    int  offset = 0;

    memcpy(buffer_snd + offset, msgInfo->sHeader.sProductIndicator,2); offset+=2;
    memcpy(buffer_snd + offset, msgInfo->sHeader.sReleaseNumber,2); offset+=2;
    memcpy(buffer_snd + offset, msgInfo->sHeader.sStatus,3); offset+=3;
    buffer_snd[offset++] = msgInfo->sHeader.sOriginatorCode[0];
    buffer_snd[offset++] = msgInfo->sHeader.sResponderCode[0];
    return BASE24_HEADER_LEN;
}
*/
/******************************************************************************/
int Base24BuildMsg (char * buffer_snd, TSBase24Info *msgInfo)
{
   char szMsgType[5];
   int  nBytes;
   int  offset;
   int  iMapCtr;

   offset = 0;
   memcpy(buffer_snd + offset,"ISO",3);offset+=3;
   memcpy(buffer_snd + offset,(char*)&msgInfo->sHeader,BASE24_HEADER_LEN);
   offset +=BASE24_HEADER_LEN;

   memset(szMsgType, '\0', sizeof(szMsgType));
   sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
   memcpy( buffer_snd + offset,szMsgType, 4);
   offset += 4 ;

   iMapCtr = 1;
   while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 2)  )
      ++iMapCtr;
   /*
   memcpy( buffer_snd+offset, msgInfo->sBitMap, iMapCtr*8);
   memcpy( buffer_snd+offset + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
   return( offset + (iMapCtr*8) + msgInfo->nLength);
   */
   HexToAscii(msgInfo->sBitMap,buffer_snd+offset,iMapCtr*8*2); offset+=(iMapCtr*8*2);
   memcpy( buffer_snd+offset, msgInfo->sData, msgInfo->nLength);offset+=msgInfo->nLength;
   return offset;

}
/******************************************************************************/
