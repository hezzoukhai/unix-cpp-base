#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <iso_cup.h>
#include <tools.h>
#include <define.h>
#include <cup_param.h>
#include <cup_define.h>


int GetCupField(int field_no, TSCupInfo * msgInfo, char *data, int *nLength)
{
	int length;
	int nBytes;
   	int nMaxLength;
	char szCupFldLen[4];	
	char sLine [MAX_LINE_TRC];

	memset( szCupFldLen, '\0', sizeof(szCupFldLen));	
	memset( sLine,       '\0', sizeof(sLine));	

	if (CheckBit (msgInfo->sBitMap, field_no))
	{
   		nMaxLength = GetCupFieldLength(field_no);
		switch (GetCupFieldType(field_no))
		{
		case CUP_LL_BIN :
			memcpy( szCupFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
			length = atoi(szCupFldLen);

			if ( length > nMaxLength )
			{
				sprintf(sLine,"GetCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,length);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}

			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
			*nLength = length;
			break;

		case CUP_LLL_BIN :
			memcpy( szCupFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
			length = atoi(szCupFldLen);

         		if ( length > nMaxLength )
         		{
            			sprintf(sLine,"GetCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,length);
            			trace_event(sLine,ERROR);
            			return(ECHEC);
         		}

			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
			*nLength = length;
			break;

		case CUP_FIX_BIN   :
		case CUP_FIX_ALPHA :
         		length = nMaxLength;
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
			data[length] = '\0';
			*nLength = length;
			break;

		case CUP_FIX_HEX :                           
         		length = nMaxLength;
			nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
				data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case CUP_LL_HEX :                           
			memcpy( szCupFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
			length = atoi(szCupFldLen);

         		if ( length > nMaxLength )
         		{
            			sprintf(sLine,"GetCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,length);
            			trace_event(sLine,ERROR);
            			return(ECHEC);
         		}

			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case CUP_LLL_HEX :                           
			memcpy ( szCupFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			length = atoi(szCupFldLen);

         		if ( length > nMaxLength )
         		{
            			sprintf(sLine,"GetCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,length);
            			trace_event(sLine,ERROR);
            			return(ECHEC);
         		}

			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case CUP_LL_ALPHA : 
			memcpy ( szCupFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
			length = atoi(szCupFldLen);

         		if ( length > nMaxLength )
         		{
            			sprintf(sLine,"GetCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,length);
            			trace_event(sLine,ERROR);
            			return(ECHEC);
         		}

			memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length); 

			data[length] = '\0';
			*nLength = length;
			break;

		case CUP_LLL_ALPHA : 
			memcpy ( szCupFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			length = atoi(szCupFldLen);

         		if ( length > nMaxLength )
         		{
            			sprintf(sLine,"GetCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,length);
            			trace_event(sLine,ERROR);
            			return(ECHEC);
         		}

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

int AddCupField(int field_no, TSCupInfo *msgInfo, char *data, int nLength)
{
	int nBytes  = 0;
	int i;
	int nMaxLength;
	char szCupFldLen[5];	
	char sLine [MAX_LINE_TRC];

	memset( szCupFldLen, '\0',  sizeof(szCupFldLen));	
	memset( sLine,       '\0',  sizeof(sLine));	

	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	msgInfo->nFieldPos[field_no] = msgInfo->nLength;
	nMaxLength = GetCupFieldLength(field_no);

	switch (GetCupFieldType(field_no))
	{
	case CUP_FIX_ALPHA :
		if ( nMaxLength!= nLength)
		{
			sprintf(sLine,"AddCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case CUP_LL_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"AddCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szCupFldLen, 2);
		msgInfo->nLength += 2;
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case CUP_LLL_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"AddCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.3d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szCupFldLen, 3);
		msgInfo->nLength += 3;
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case CUP_FIX_HEX :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"AddCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case CUP_LL_HEX :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"AddCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szCupFldLen, 2);
		msgInfo->nLength += 2;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case CUP_LLL_HEX :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"AddCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.3d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szCupFldLen, 3);
		msgInfo->nLength += 3;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case CUP_FIX_BIN :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"AddCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case CUP_LL_BIN :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"AddCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szCupFldLen, 2);
		msgInfo->nLength += 2;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case CUP_LLL_BIN :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"AddCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.3d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szCupFldLen, 3);
		msgInfo->nLength += 3;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	default :
		return(ECHEC);
	}
	msgInfo->nLength += nBytes;
	for (i=field_no; i< MAX_CUP_FIELDS; i++)
		msgInfo->nFieldPos[i+1] = msgInfo->nLength;
	SetBit(msgInfo->sBitMap, field_no+1);
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);

	return(SUCCES);
}
/* NOT USED
int InsertCupField(int field_no, TSCupInfo *msgInfo, char *data, int nLength)
{
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i,nMaxLength;
	char szCupData [999];	
	char szCupFldLen[4];	
	int  nOldFldLen,nOldLen;
	char szWork    [LG_MAX];	
	char	sLine [MAX_LINE_TRC];

	memset( szCupFldLen, '\0', sizeof(szCupFldLen));	
	memset( szCupData, '\0', sizeof(szCupData));	
	memset( szWork, '\0', sizeof(szWork));	
	memset( sLine, '\0', sizeof(sLine));	

	nMaxLength = GetCupFieldLength(field_no);

	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	switch (GetCupFieldType(field_no))
	{
	case CUP_FIX_ALPHA :
		if ( nLength != nMaxLength )
		{
			sprintf(sLine,"InsertCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = nLength;
		memcpy (szCupData, data, nBytes);
		break;

	case CUP_LL_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.2d", nLength);	
		memcpy( szCupData,        szCupFldLen, 2);
		nBytes=2;
		memcpy( szCupData+2, data,        nLength);
		nBytes += nLength;
		break;

	case CUP_LLL_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.3d", nLength);	
		nBytes = 3;
		memcpy( szCupData, szCupFldLen, 3);
		memcpy( szCupData+3, data,        nLength);
		nBytes += nLength;
		break;

	case CUP_FIX_HEX :
		if ( nLength != nMaxLength )
		{
			sprintf(sLine,"InsertCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = AsciiToHex(data, szCupData, nLength);
		break;

	case CUP_LL_HEX :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.2d", nLength);	
		nBytes1 = 2;
		memcpy( szCupData, szCupFldLen, nBytes1);
		nBytes = AsciiToHex( data, szCupData+2, nLength);
		nBytes += nBytes1;
		break;

	case CUP_LLL_HEX :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.3d", nLength);	
		nBytes1 = 3;
		memcpy( szCupData, szCupFldLen, 3);
		nBytes = AsciiToHex( data, szCupData+3, nLength);
		nBytes += nBytes1;
		break;

	case CUP_FIX_BIN :
		if ( nLength != nMaxLength )
		{
			sprintf(sLine,"InsertCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = nLength;
		memcpy( szCupData, data, nBytes);
		break;

	case CUP_LL_BIN :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.2d", nLength);	
		nBytes = 2;
		memcpy( szCupData, szCupFldLen, nBytes);
		memcpy( szCupData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	case CUP_LLL_BIN :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szCupFldLen, "%.3d", nLength);	
		nBytes = 3;
		memcpy( szCupData, szCupFldLen, nBytes);
		memcpy( szCupData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	default :
		return(ECHEC);
	}
	nOldFldLen = 0;
	memset(szWork, 0, sizeof(szWork));
	nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
	memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

	for (i = MAX_CUP_FIELDS-1; i > field_no; i--)
	{
		msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
	}
	memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szCupData, nBytes);
	msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
	msgInfo->nLength += nBytes-nOldFldLen;
	SetBit(msgInfo->sBitMap, field_no+1);
	memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);

	return(SUCCES);
}
*/

/*NOT USED
int PutCupField(int field_no, TSCupInfo *msgInfo, char *data, int nLength)
{
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i, nOldFldLen,nOldLen,nMaxLength;
	char szCupData [999];	
	char szWork    [LG_MAX];	
	char szCupFldLen[4];	
	char	sLine [MAX_LINE_TRC];

	memset( szCupFldLen, '\0', sizeof(szCupFldLen));	
	memset( sLine,       '\0', sizeof(sLine));	
	memset( szWork,      '\0', sizeof(szWork));	
	memset( szCupData,   '\0', sizeof(szCupData));

	nMaxLength = GetCupFieldLength(field_no);

	if ( CheckBit( msgInfo->sBitMap, field_no))
	{
		switch (GetCupFieldType(field_no))
		{
		case CUP_FIX_ALPHA :
			if ( nLength != nMaxLength )
			{
				sprintf(sLine,"PutCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			nBytes = nLength;
			memcpy (szCupData, data, nBytes);
			break;

		case CUP_LL_ALPHA :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			sprintf ( szCupFldLen, "%.2d", nLength);	
			memcpy( szCupData,        szCupFldLen, 2);
			nBytes=2;
			memcpy( szCupData+2, data,        nLength);
			nBytes += nLength;
			break;

		case CUP_LLL_ALPHA :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			sprintf ( szCupFldLen, "%.3d", nLength);	
			nBytes = 3;
			memcpy( szCupData, szCupFldLen, 3);
			memcpy( szCupData+3, data,        nLength);
			nBytes += nLength;
			break;

		case CUP_FIX_HEX :
			if ( nLength != nMaxLength )
			{
				sprintf(sLine,"PutCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			nBytes = AsciiToHex(data, szCupData, nLength);
			break;

		case CUP_LL_HEX :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			sprintf ( szCupFldLen, "%.2d", nLength);	
			nBytes1 = 2;
			memcpy( szCupData, szCupFldLen, nBytes1);
			nBytes = AsciiToHex( data, szCupData+2, nLength);
			nBytes += nBytes1;
			break;

		case CUP_LLL_HEX :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			sprintf ( szCupFldLen, "%.3d", nLength);	
			nBytes1 = 3;
			memcpy( szCupData, szCupFldLen, 3);
			nBytes = AsciiToHex( data, szCupData+3, nLength);
			nBytes += nBytes1;
			break;

		case CUP_FIX_BIN :
			if ( nLength != nMaxLength )
			{
				sprintf(sLine,"PutCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			nBytes = nLength;
			memcpy( szCupData, data, nBytes);
			break;

		case CUP_LL_BIN :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			sprintf ( szCupFldLen, "%.2d", nLength);	
			nBytes = 2;
			memcpy( szCupData, szCupFldLen, nBytes);
			memcpy( szCupData+nBytes, data, nLength);
			nBytes += nLength;
			break;

		case CUP_LLL_BIN :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutCupField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			sprintf ( szCupFldLen, "%.3d", nLength);	
			nBytes = 3;
			memcpy( szCupData, szCupFldLen, nBytes);
			memcpy( szCupData+nBytes, data, nLength);
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

		for (i = MAX_CUP_FIELDS-1; i > field_no; i--)
		{
			msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
		}
		memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szCupData, nBytes);
		msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
		msgInfo->nLength += nBytes-nOldFldLen;
		SetBit(msgInfo->sBitMap, field_no+1);
		memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
		if ( (field_no >64) && (field_no < 128))
			SetBit(msgInfo->sBitMap, 1);
		return(SUCCES);
	}
	else
		return ( InsertCupField (field_no, msgInfo, data, nLength));

}
*/
int CupBuildMsg (char * buffer_snd, TSCupInfo *msgInfo)
{
	char szMsgType[5];
	int  nBytes;
	int  iMapCtr;

	memset (szMsgType, 0, sizeof(szMsgType));

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
int GetCupIcTag (char  *tag_name, TSTagCup *tagInfo, char *data, int *length)/*NOT USED*/
{
	int  nIndice ;
	int  nLenTag ;
	int  nDigits;
   int	nTagMaxLength;
	char sLine [MAX_LINE_TRC];

	memset (sLine, 0, sizeof(sLine));

	if ( (nIndice = GetCupTagIndice(tag_name)) == -1)
		return(ECHEC);
	if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
		return(ECHEC);

	if ( (unsigned char)((*(tagInfo->sChipData + tagInfo->nPosTag[nIndice])) & 0x1F) == (unsigned char) 0x1F)
		nLenTag = 2;
	else
		nLenTag = 1;
	*length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag));
   nTagMaxLength = GetCupTagLength(nIndice);

	switch (GetCupTagType(nIndice) )
	{
	case CUP_TAG_FIX_BIN :
	case CUP_TAG_FIX_ALPHA :

	if ( nTagMaxLength!= *length)
	{
		sprintf(sLine,"GetCupIcTag(%s) invalid length Max: %d VS %d",tag_name,nTagMaxLength,*length);
		trace_event(sLine,ERROR);
		return(ECHEC);
	}
		memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
			*length);
		break;

	case CUP_TAG_VAR_HEX :

        if (  *length > nTagMaxLength * 2)
        {
                sprintf(sLine,"GetCupIcTag(%s) invalid length Max: %d VS %d",tag_name,nTagMaxLength,*length);
                trace_event(sLine,ERROR);
                return(ECHEC);
        }

		nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + nLenTag+1, data, (*length)*2);
		*length = (*length)*2;
		break;
	case CUP_TAG_VAR_BCD :

        if (  *length > nTagMaxLength)
        {
                sprintf(sLine,"GetCupIcTag(%s) invalid length Max: %d VS %d",tag_name,nTagMaxLength,*length);
                trace_event(sLine,ERROR);
                return(ECHEC);
        }

		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + nLenTag+1, data, (*length)*2);
		*length = (*length)*2;
		break;

	case CUP_TAG_FIX_BCD :

        if ( nTagMaxLength!= *length)
        {
                sprintf(sLine,"GetCupIcTag(%s) invalid length Max: %d VS %d",tag_name,nTagMaxLength,*length);
                trace_event(sLine,ERROR);
                return(ECHEC);
        }

		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + nLenTag+1, data, (*length)*2);
		*length = (*length)*2;
		break;

	case CUP_TAG_VAR_BIN :
	case CUP_TAG_VAR_ALPHA :

        if (  *length > nTagMaxLength)
        {
                sprintf(sLine,"GetCupIcTag(%s) invalid length Max: %d VS %d",tag_name,nTagMaxLength,*length);
                trace_event(sLine,ERROR);
                return(ECHEC);
        }

		memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1, *length);
		break;

	}
	return(SUCCES);
}

int AddCupIcTag (char  *tag_name, TSTagCup *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;
   int  nTagMaxLength;
   char sLine [MAX_LINE_TRC];

	if ( (nIndice = GetCupTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == PRESENT)
		return(ECHEC);

	AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

	if ( (unsigned char) ((*(tagInfo->sChipData + tagInfo->nLength)) & 0x1F) == (unsigned char)0x1F)
		nLenTag = 2;
	else 
		nLenTag = 1;

   	nTagMaxLength = GetCupTagLength(nIndice);
	switch (GetCupTagType(nIndice) )
	{
	case CUP_TAG_FIX_BIN :
	case CUP_TAG_FIX_ALPHA :

      		if (nTagMaxLength!= length)
      		{
			sprintf(sLine,"AddCupIcTag(%d) invalid length Max: %d VS %d",nIndice,nTagMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
      		}
		memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
		nDigits = length;
		break;

	case CUP_TAG_VAR_HEX :

      		if (length > nTagMaxLength * 2)
      		{
        		sprintf(sLine,"AddCupIcTag(%d) invalid length Max: %d VS %d",nIndice,nTagMaxLength,length);
        		trace_event(sLine,ERROR);
        		return(ECHEC);
      		}

		nDigits = AsciiToHex( data, 
		tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
		length);
		break;

	case CUP_TAG_VAR_BCD :

      		if (length > nTagMaxLength)
      		{
        		sprintf(sLine,"AddCupIcTag(%d) invalid length Max: %d VS %d",nIndice,nTagMaxLength,length);
        		trace_event(sLine,ERROR);
        		return(ECHEC);
      		}


		nDigits = AsciiToBcd( data, 
		tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
		length);
		break;

	case CUP_TAG_FIX_BCD :
      
      		if (length !=  nTagMaxLength)
      		{
        		sprintf(sLine,"AddCupIcTag(%d) invalid length Max: %d VS %d",nIndice,nTagMaxLength,length);
        		trace_event(sLine,ERROR);
			return(ECHEC);
      		}


		nDigits = AsciiToBcd( data, 
		tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
		length);
		break;

	case CUP_TAG_VAR_BIN :
	case CUP_TAG_VAR_ALPHA :

	        if (length > nTagMaxLength)
		{
        		sprintf(sLine,"AddCupIcTag(%d) invalid length Max: %d VS %d",nIndice,nTagMaxLength,length);
        		trace_event(sLine,ERROR);
        		return(ECHEC);
      		}


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
/* NOT USED
int PutCupIcTag (char  *tag_name, TSTagCup *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;
	int nPutPos, nTagPresent = NOT_PRESENT;
	char szLastPart  [ MAX_CUP_CHIP_LEN ];
	int nOldTagLen, i;
     	int  nTagMaxLength;
   	char sLine [MAX_LINE_TRC];

	memset (szLastPart, 0, sizeof(szLastPart));

	if ( (nIndice = GetCupTagIndice(tag_name)) == -1)
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

	if ( (unsigned char) ((*(tagInfo->sChipData + nPutPos)) & 0x1F) == (unsigned char) 0x1F)
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
   	nTagMaxLength = GetCupTagLength(nIndice);
	switch (GetCupTagType(nIndice) )
	{
	case CUP_TAG_FIX_BIN :
	case CUP_TAG_FIX_ALPHA :
      
  	     	if (length != nTagMaxLength)
        	{
       			sprintf(sLine,"PutCupIcTag(%d) invalid length Max: %d VS %d",nIndice,nTagMaxLength,length);
        		trace_event(sLine,ERROR);
			return(ECHEC);
      		}

		memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
		nDigits = length;
		break;

	case CUP_TAG_VAR_HEX :

      		if (length > nTagMaxLength)
      		{
        		sprintf(sLine,"PutCupIcTag(%d) invalid length Max: %d VS %d",nIndice,nTagMaxLength,length);
        		trace_event(sLine,ERROR);
        		return(ECHEC);
      		}

		nDigits = AsciiToHex( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case CUP_TAG_VAR_BCD :

	        if (length > nTagMaxLength)
      		{
        		sprintf(sLine,"PutCupIcTag(%d) invalid length Max: %d VS %d",nIndice,nTagMaxLength,length);
        		trace_event(sLine,ERROR);
        		return(ECHEC);
      		}

		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case CUP_TAG_FIX_BCD :
      
      		if (length != nTagMaxLength)
      		{
        		sprintf(sLine,"PutCupIcTag(%d) invalid length Max: %d VS %d",nIndice,nTagMaxLength,length);
        		trace_event(sLine,ERROR);
			return(ECHEC);
      		}

		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case CUP_TAG_VAR_BIN :
	case CUP_TAG_VAR_ALPHA :

      		if (length > nTagMaxLength)
      		{
        		sprintf(sLine,"PutCupIcTag(%d) invalid length Max: %d VS %d",nIndice,nTagMaxLength,length);
        		trace_event(sLine,ERROR);
        		return(ECHEC);
      		}

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
		for (i = nIndice + 1; i < MAX_CUP_CHIP_TAG; i++)
		{
			if (tagInfo->nPresent[i] == PRESENT)
				tagInfo->nPosTag[i] += nDigits - nOldTagLen;
		}
	}
	return(SUCCES);
}
*/

int CupBuildIcFld (char * buffer_snd, TSTagCup *tagInfo)
{
	memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
	return(tagInfo->nLength);
}

void CupBuildHeader(int      nReqResp,
	TSCupHeader*   pNewHeader,
	TSCupHeader*   pIncomingHeader,
	int nCupMsgLength,
	char * szStationId)
{	
	if (REQUEST == nReqResp)
   	{
      		memset((char*)pNewHeader, 0, sizeof(TSCupHeader));
        	memcpy(pNewHeader->abSrcStationId, szStationId, 6);
                memcpy(pNewHeader->abSrcStationId + 6, "     ", 5);
   	}
	
	else /* This is a response */
   	{

		memcpy((char*)pNewHeader, (char*)pIncomingHeader, sizeof(TSCupHeader));
		memcpy(pNewHeader->abSrcStationId, pIncomingHeader->abDestStationId, CUP_STATION_ID_LEN);
		memcpy(pNewHeader->abDestStationId, pIncomingHeader->abSrcStationId, CUP_STATION_ID_LEN);
	}
	pNewHeader->bHeaderLength = CUP_HDR_LEN;

	pNewHeader->bHeaderFlag   = 0x01;
	sprintf (pNewHeader->abMsgLength , "%04d" , (nCupMsgLength + CUP_HDR_LEN)) ;
}

void CupHeaderToBuffer (TSCupHeader  *sCupHeader , int nCupLenBuff)
{
	char sLine [MAX_LINE_TRC];
	char tmpField[13];

	memset(tmpField, 0, sizeof(tmpField));
	memset(sLine, 0, sizeof(sLine));

	sCupHeader->bHeaderLength = 0x2E;
	sCupHeader->bHeaderFlag = 0x02;

	sprintf(sCupHeader->abMsgLength, "%04d", nCupLenBuff );
	sprintf(sCupHeader->abDestStationId, "%s", "00010344   ");


	memcpy ( tmpField, AcquirerId, 8);
	memcpy ( tmpField + 8, "   ", 3);
	sprintf(sCupHeader->abSrcStationId, "%s",tmpField);

	sCupHeader->abReserved[0] = 0x00;
	sCupHeader->abReserved[1] = 0x00;
	sCupHeader->abReserved[2] = 0x00;
	sCupHeader->bBatchNumber = 0x00;
	memcpy(sCupHeader->abTransInfo, "00000000", 8);
	sCupHeader->bUserInfo = 0x00;

	/*sprintf(sCupHeader->abRejectCode, "%05d", 0);*/
	memcpy(sCupHeader->abRejectCode, "00000", 5);

	return;
}

