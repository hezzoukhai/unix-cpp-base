#include <stdio.h>
#include <stdlib.h>

#include <iso_hps.h>
#include <tools.h>
#include <define.h>


int GetIsoField(int field_no, TSIsoInfo * msgInfo, char *data, int *nLength)
{
	int		length;
	int		nBytes;
	char	szIsoFldLen[4];	
	char	sLine [MAX_LINE_TRC];
	int		nMaxLength;

	*nLength = 0;
	memset( szIsoFldLen, '\0', sizeof(szIsoFldLen));	
	if (!CheckBit (msgInfo->sBitMap, field_no))
	{
		return(ECHEC);
	}

	nMaxLength = GetIsoFieldLength(field_no);

	switch (GetIsoFieldType(field_no))
	{
	case ISO_LL_BIN :
		memcpy( szIsoFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
		length = atoi(szIsoFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetIsoField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
		*nLength = length;
		break;

	case ISO_LLL_BIN :
		memcpy( szIsoFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
		length = atoi(szIsoFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetIsoField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
		*nLength = length;
		break;

	case ISO_FIX_BIN   :
	case ISO_FIX_ALPHA :
		memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], nMaxLength);
		data[nMaxLength] = '\0';
		*nLength = nMaxLength;
		break;

	case ISO_FIX_HEX :
		nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], data, nMaxLength);
		data[nMaxLength] = '\0';
		*nLength = nMaxLength;
		break;

	case ISO_LL_HEX :                           
		memcpy( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
		length = atoi(szIsoFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetIsoField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
			data, length);
		data[length] = '\0';
		*nLength = length;
		break;

	case ISO_LLL_HEX :                           
		memcpy ( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
		length = atoi(szIsoFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetIsoField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
			data, length); 
		data[length] = '\0';
		*nLength = length;
		break;

	case ISO_LL_ALPHA : 
		memcpy ( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
		length = atoi(szIsoFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetIsoField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length); 
		data[length] = '\0';
		*nLength = length;
		break;

	case ISO_LLL_ALPHA : 
		memcpy ( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
		length = atoi(szIsoFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetIsoField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
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

int AddIsoField(int field_no, TSIsoInfo *msgInfo, char *data, int nLength)
{
	int nBytes  = 0;
	int i;
	char szIsoFldLen[5];	
	char sLine [MAX_LINE_TRC];
	int		nMaxLength;


	memset( szIsoFldLen, '\0', sizeof(szIsoFldLen));

	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	if( nLength == 0 )
	{
		sprintf(sLine,"AddIsoField(%d) invalid length is 0: %d ",field_no,nLength);
		trace_event(sLine,ERROR);
		return(ECHEC);
	}

	nMaxLength = GetIsoFieldLength(field_no);


	msgInfo->nFieldPos[field_no] = msgInfo->nLength;
	switch (GetIsoFieldType(field_no))
	{
	case ISO_FIX_ALPHA :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"AddIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case ISO_LL_ALPHA :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 2);
		msgInfo->nLength += 2;
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case ISO_LLL_ALPHA :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szIsoFldLen, "%.3d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 3);
		msgInfo->nLength += 3;
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case ISO_FIX_HEX :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"AddIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case ISO_LL_HEX :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 2);
		msgInfo->nLength += 2;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case ISO_LLL_HEX :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szIsoFldLen, "%.3d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 3);
		msgInfo->nLength += 3;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case ISO_FIX_BIN :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"AddIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case ISO_LL_BIN :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 2);
		msgInfo->nLength += 2;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case ISO_LLL_BIN :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
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

	for (i=field_no; i< MAX_ISO_FIELDS; i++)
		msgInfo->nFieldPos[i+1] = msgInfo->nLength;

	SetBit(msgInfo->sBitMap, field_no+1);

	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);

	return(SUCCES);
}

int InsertIsoField(int field_no, TSIsoInfo *msgInfo, char *data, int nLength)
{
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i;
	char szIsoData [999];	
	char szIsoFldLen[4];	
	int  nOldFldLen,nOldLen;
	char szWork    [LG_MAX];	
	int		nMaxLength;
	char	sLine[MAX_LINE_TRC];


	memset( szIsoFldLen, '\0', sizeof(szIsoFldLen));

	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);


	if( nLength == 0 )
	{
		sprintf(sLine,"InsertIsoField(%d) invalid length is 0: %d ",field_no,nLength);
		trace_event(sLine,ERROR);
		return(ECHEC);
	}

	nMaxLength = GetIsoFieldLength(field_no);

	switch (GetIsoFieldType(field_no))
	{
	case ISO_FIX_ALPHA :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"InsertIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = nLength;
		memcpy (szIsoData, data, nBytes);
		break;

	case ISO_LL_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		memcpy( szIsoData,        szIsoFldLen, 2);
		nBytes=2;
		memcpy( szIsoData+2, data,        nLength);
		nBytes += nLength;
		break;

	case ISO_LLL_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szIsoFldLen, "%.3d", nLength);	
		nBytes = 3;
		memcpy( szIsoData, szIsoFldLen, 3);
		memcpy( szIsoData+3, data,        nLength);
		nBytes += nLength;
		break;

	case ISO_FIX_HEX :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"InsertIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = AsciiToHex(data, szIsoData, nLength);
		break;

	case ISO_LL_HEX :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		nBytes1 = 2;
		memcpy( szIsoData, szIsoFldLen, nBytes1);
		nBytes = AsciiToHex( data, szIsoData+2, nLength);
		nBytes += nBytes1;
		break;

	case ISO_LLL_HEX :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szIsoFldLen, "%.3d", nLength);	
		nBytes1 = 3;
		memcpy( szIsoData, szIsoFldLen, 3);
		nBytes = AsciiToHex( data, szIsoData+3, nLength);
		nBytes += nBytes1;
		break;

	case ISO_FIX_BIN :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"InsertIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = nLength;
		memcpy( szIsoData, data, nBytes);
		break;

	case ISO_LL_BIN :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		nBytes = 2;
		memcpy( szIsoData, szIsoFldLen, nBytes);
		memcpy( szIsoData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	case ISO_LLL_BIN :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
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

	for (i = MAX_ISO_FIELDS-1; i > field_no; i--)
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

	memset(szIsoData, 0, sizeof(szIsoData));
	memset(szWork, 0, sizeof(szWork));

	return(SUCCES);
}

int PutIsoField(int field_no, TSIsoInfo *msgInfo, char *data, int nLength)
{
	int		nBytes   = 0;
	int		nBytes1  = 0;
	int		i, nOldFldLen,nOldLen;
	char	szIsoData [999];	
	char	szWork    [LG_MAX];	
	char	szIsoFldLen[4];	
	int		nMaxLength;
	int		nFieldExist = 0;
	char	sLine[MAX_LINE_TRC];

	memset( szIsoFldLen, '\0', sizeof(szIsoFldLen));

	nFieldExist = CheckBit( msgInfo->sBitMap, field_no);	


	if ( nLength > 0  && !nFieldExist/*CheckBit( msgInfo->sBitMap, field_no)*/)
	{
		return ( InsertIsoField (field_no, msgInfo, data, nLength));
	}
	else if( nLength == 0 && !nFieldExist)
	{
		return(SUCCES);
	}
	else if( nLength == 0 && nFieldExist)
	{
		/*sprintf(sLine,"PutIsoField(%d) invalid length is 0: %d ",field_no,nLength);
		trace_event(sLine,ERROR);
		return(ECHEC);*/
		nBytes = 0;
	}
	else /*nLength > 0 && nFieldExist*/ /*IBO20221227 PLUTONL-5603*/
	{

		nMaxLength = GetIsoFieldLength(field_no);
	
		switch (GetIsoFieldType(field_no))
		{
		case ISO_FIX_ALPHA :
			if ( nMaxLength != nLength)
			{
				sprintf(sLine,"PutIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);		
				return(ECHEC);
			}
			nBytes = nLength;
			memcpy (szIsoData, data, nBytes);
			break;
	
		case ISO_LL_ALPHA :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);		
				return(ECHEC);
			}
			sprintf ( szIsoFldLen, "%.2d", nLength);	
			memcpy( szIsoData,        szIsoFldLen, 2);
			nBytes=2;
			memcpy( szIsoData+2, data,        nLength);
			nBytes += nLength;
			break;
	
		case ISO_LLL_ALPHA :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);		
				return(ECHEC);
			}
			sprintf ( szIsoFldLen, "%.3d", nLength);	
			nBytes = 3;
			memcpy( szIsoData, szIsoFldLen, 3);
			memcpy( szIsoData+3, data,        nLength);
			nBytes += nLength;
			break;
	
		case ISO_FIX_HEX :
			if ( nMaxLength != nLength)
			{
				sprintf(sLine,"PutIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);		
				return(ECHEC);
			}
			nBytes = AsciiToHex(data, szIsoData, nLength);
			break;
	
		case ISO_LL_HEX :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);		
				return(ECHEC);
			}
			sprintf ( szIsoFldLen, "%.2d", nLength);	
			nBytes1 = 2;
			memcpy( szIsoData, szIsoFldLen, nBytes1);
			nBytes = AsciiToHex( data, szIsoData+2, nLength);
			nBytes += nBytes1;
			break;
	
		case ISO_LLL_HEX :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);		
				return(ECHEC);
			}
			sprintf ( szIsoFldLen, "%.3d", nLength);	
			nBytes1 = 3;
			memcpy( szIsoData, szIsoFldLen, 3);
			nBytes = AsciiToHex( data, szIsoData+3, nLength);
			nBytes += nBytes1;
			break;
	
		case ISO_FIX_BIN :
			if ( nMaxLength != nLength)
			{
				sprintf(sLine,"PutIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			nBytes = nLength;
			memcpy( szIsoData, data, nBytes);
			break;
	
		case ISO_LL_BIN :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);		
				return(ECHEC);
			}
			sprintf ( szIsoFldLen, "%.2d", nLength);	
			nBytes = 2;
			memcpy( szIsoData, szIsoFldLen, nBytes);
			memcpy( szIsoData+nBytes, data, nLength);
			nBytes += nLength;
			break;
	
		case ISO_LLL_BIN :
			if ( nLength > nMaxLength )
			{
				sprintf(sLine,"PutIsoField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
				trace_event(sLine,ERROR);		
				return(ECHEC);
			}
			sprintf ( szIsoFldLen, "%.3d", nLength);	
			nBytes = 3;
			memcpy( szIsoData, szIsoFldLen, nBytes);
			memcpy( szIsoData+nBytes, data, nLength);
			nBytes += nLength;
			break;
	
		default :
			return(ECHEC);
		}
	}

	nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
		msgInfo->nFieldPos[field_no];
	memset(szWork, 0, sizeof(szWork));
	nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
	memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

	for (i = MAX_ISO_FIELDS-1; i > field_no; i--)
	{
		msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
	}
	if(nBytes > 0)
	{
		memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szIsoData, nBytes);
		SetBit(msgInfo->sBitMap, field_no+1);
	}
	else
	{
		ClearBit(msgInfo->sBitMap, field_no+1);
	}
	msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
	msgInfo->nLength += nBytes-nOldFldLen;
	memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);

	memset(szIsoData, 0, sizeof(szIsoData));
	memset(szWork, 0, sizeof(szWork));

	return(SUCCES);	

}

int IsoBuildMsg (char * buffer_snd, TSIsoInfo *msgInfo)
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
int GetIsoIcTag (char  *tag_name, TSTagIso *tagInfo, char *data, int *length)
{
	int  nIndice ;
	int  nLenTag ;
	int  nDigits;
	char sLine [MAX_LINE_TRC];

	if ( (nIndice = GetIsoTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
		return(ECHEC);

	/*if ( ((*(tagInfo->sChipData + tagInfo->nPosTag[nIndice])) & 0x1F) == 0x1F)*//*MKB 07/09/2006*/
	if ( (unsigned char)((*(tagInfo->sChipData + tagInfo->nPosTag[nIndice])) & 0x1F) == (unsigned char) 0x1F)
		nLenTag = 2;
	else
		nLenTag = 1;
	*length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag));

	switch (GetIsoTagType(nIndice) )
	{
	case ISO_TAG_FIX_BIN :
	case ISO_TAG_FIX_ALPHA :
		memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
			*length);
		break;


	case ISO_TAG_VAR_HEX :
		nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + nLenTag+1, data, (*length)*2);
		*length = (*length)*2;
		break;
	case ISO_TAG_VAR_BCD :
		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + nLenTag+1, data, (*length)*2);
		*length = (*length)*2;
		break;

	case ISO_TAG_FIX_BCD :
		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + nLenTag+1, data, (*length)*2);
		*length = (*length)*2;
		break;

	case ISO_TAG_VAR_BIN :
	case ISO_TAG_VAR_ALPHA :
		memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1, *length);
		break;

	}

	/*tag 57 may be right padded with F when length is odd */
	if( strcmp(tag_name,ISO_TAG_TRACK2) == 0 && (*length) >= 1 )
	{
		if( data[(*length) - 1] == 'F' )
		{
			data[(*length) - 1] ='\0';
			(*length) = (*length) -1;
		}
	}


	return(SUCCES);
}

int AddIsoIcTag (char  *tag_name, TSTagIso *tagInfo, char *data, int length)
{
	int		nIndice;
	int		nLenTag;
	int		nLength;
	int		nDigits;
	char	sLocData[BUF_LG_MAX];

	if ( (nIndice = GetIsoTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == PRESENT)
		return(ECHEC);

	if( length > BUF_LG_MAX - 1 )
		return(ECHEC);

	memcpy(sLocData,data,sizeof(sLocData) - 1);

	/*tag 57 should be right padded with F when length is odd */
	if( strcmp(tag_name,ISO_TAG_TRACK2) == 0 && (length&0x1) == 1 )
	{
		sLocData[length++]='F';
	}


	AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

	/*if ( (*(tagInfo->sChipData + tagInfo->nLength)) & 0x1F == 0x1F)*/ /**MKB 07092006**/
	if ( (unsigned char) ((*(tagInfo->sChipData + tagInfo->nLength)) & 0x1F) == (unsigned char)0x1F)
		nLenTag = 2;
	else 
		nLenTag = 1;

	switch (GetIsoTagType(nIndice) )
	{
	case ISO_TAG_FIX_BIN :
	case ISO_TAG_FIX_ALPHA :
		if ( GetIsoTagLength(nIndice) != length)
			return(ECHEC);
		memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, sLocData, length);
		nDigits = length;
		break;

	case ISO_TAG_VAR_HEX :
		nDigits = AsciiToHex( sLocData, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case ISO_TAG_VAR_BCD :
		nDigits = AsciiToBcd( sLocData, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case ISO_TAG_FIX_BCD :
		if ( GetIsoTagLength(nIndice) != length)
			return(ECHEC);
		nDigits = AsciiToBcd( sLocData, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case ISO_TAG_VAR_BIN :
	case ISO_TAG_VAR_ALPHA :
		memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, sLocData, length);
		nDigits = length;
		break;

	}

	*(tagInfo->sChipData + tagInfo->nLength + nLenTag)    = nDigits;
	tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
	tagInfo->nPresent[ nIndice ] = PRESENT;
	tagInfo->nLength +=nDigits+nLenTag + 1;
	return(SUCCES);
}

int PutIsoIcTag (char  *tag_name, TSTagIso *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;
	int nPutPos, nTagPresent = NOT_PRESENT;
	char szLastPart  [ MAX_ISO_CHIP_LEN ];
	int nOldTagLen, i;
	char	sLocData[BUF_LG_MAX];
	
	
	
	if ( (nIndice = GetIsoTagIndice(tag_name)) == -1)
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

	
	if( length > BUF_LG_MAX - 1 )
		return(ECHEC);

	memcpy(sLocData,data,sizeof(sLocData) - 1);

	/*tag 57 should be right padded with F when length is odd */
	if( strcmp(tag_name,ISO_TAG_TRACK2) == 0 && (length&0x1) == 1 )
	{
		sLocData[length++]='F';
	}

	
	if ( nTagPresent != PRESENT )
		AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

	/*if ( (*(tagInfo->sChipData + nPutPos)) & 0x1F == 0x1F)*/ /*LHI 25 08 2006 **/
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

	switch (GetIsoTagType(nIndice) )
	{
	case ISO_TAG_FIX_BIN :
	case ISO_TAG_FIX_ALPHA :
		if ( GetIsoTagLength(nIndice) != length)
			return(ECHEC);
		memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, sLocData, length);
		nDigits = length;
		break;

	case ISO_TAG_VAR_HEX :
		nDigits = AsciiToHex( sLocData, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case ISO_TAG_VAR_BCD :
		nDigits = AsciiToBcd( sLocData, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case ISO_TAG_FIX_BCD :
		if ( GetIsoTagLength(nIndice) != length)
			return(ECHEC);
		nDigits = AsciiToBcd( sLocData, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case ISO_TAG_VAR_BIN :
	case ISO_TAG_VAR_ALPHA :
		memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, sLocData, length);
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
		for (i = nIndice + 1; i < MAX_ISO_CHIP_TAG; i++)
		{
			if (tagInfo->nPresent[i] == PRESENT)
				tagInfo->nPosTag[i] += nDigits - nOldTagLen;
		}
	}
	return(SUCCES);
}

int IsoBuildIcFld (char * buffer_snd, TSTagIso *tagInfo)
{
	memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
	return(tagInfo->nLength);
}


