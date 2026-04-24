#include <stdio.h>
#include <stdlib.h>

#include <iso_pos.h>
#include <tools.h>
#include <define.h>



int GetPosField(int field_no, TSPosInfo * msgInfo, char *data, int *nLength)
{
	int		length;
	int		nBytes;
	char	szPosFldLen[4];	
	char	szBcdFldLen[4];	
	int		nMaxLength;
	char	sLine[MAX_LINE_TRC];


	memset( szPosFldLen, '\0', sizeof(szPosFldLen));	
	memset( szBcdFldLen, '\0', sizeof(szBcdFldLen));	


	if (!CheckBit (msgInfo->sBitMap, field_no))
	{
		return(ECHEC);
	}

	nMaxLength = GetPosFieldLength(field_no);


	switch (GetPosFieldType(field_no))
	{
	case POS_LL_BIN :
		memcpy( szBcdFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 1);
		nBytes = BcdToAscii( szBcdFldLen, szPosFldLen, 2);
		length = atoi(szPosFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetPosField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+1, length);
		*nLength = length;
		break;

	case POS_LLL_BIN :
		memcpy( szBcdFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
		nBytes = BcdToAscii( szBcdFldLen, szPosFldLen, 3);
		length = atoi(szPosFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetPosField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
		*nLength = length;
		break;

	case POS_FIX_BIN   :
	case POS_FIX_ALPHA :
		memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], nMaxLength);
		data[nMaxLength] = '\0';
		*nLength = nMaxLength;
		break;

	case POS_FIX_HEX :                           
		nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], data, nMaxLength);
		data[nMaxLength] = '\0';
		*nLength = nMaxLength;
		break;

	case POS_LL_HEX :                           
		memcpy( szBcdFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 1);
		nBytes = BcdToAscii( szBcdFldLen, szPosFldLen, 2);
		length = atoi(szPosFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetPosField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, data, length); 
		data[length] = '\0';
		*nLength = length;
		break;

	case POS_LLL_HEX :                           
		memcpy ( szBcdFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
		nBytes = BcdToAscii( szBcdFldLen, szPosFldLen, 3);
		length = atoi(szPosFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetPosField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, data, length); 
		data[length] = '\0';
		*nLength = length;
		break;

	case POS_LL_ALPHA : 
		memcpy ( szBcdFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 1);
		nBytes = BcdToAscii( szBcdFldLen, szPosFldLen, 2);
		length = atoi(szPosFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetPosField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+1, length); 
		data[length] = '\0';
		*nLength = length;
		break;

	case POS_LLL_ALPHA : 
		memcpy ( szBcdFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
		nBytes = BcdToAscii( szBcdFldLen, szPosFldLen, 3);
		length = atoi(szPosFldLen);
		if( length > nMaxLength )
		{
			sprintf(sLine,"GetPosField(%d) invalid length Max: %d VS found %d",field_no,nMaxLength,length);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length); 
		data[length] = '\0';
		*nLength = length;
		break;

	default :
		return(ECHEC);
		break;

	}
	return(SUCCES);

	
}

int AddPosField(int field_no, TSPosInfo *msgInfo, char *data, int nLength)
{
	int nBytes,i   = 0;
	int nDigits  = 0;
	char szPosFldLen[4];	
	char szBcdFldLen[4];	
	int		nMaxLength;
	char	sLine[MAX_LINE_TRC];

	memset( szPosFldLen, '\0', sizeof(szPosFldLen));	
	memset( szBcdFldLen, '\0', sizeof(szBcdFldLen));	


	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);


	nMaxLength = GetPosFieldLength(field_no);


	msgInfo->nFieldPos[field_no] = msgInfo->nLength;


	switch (GetPosFieldType(field_no))
	{
	case POS_FIX_ALPHA :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"AddPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case POS_LL_ALPHA :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		sprintf ( szPosFldLen, "%.2d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 2);
		memcpy( msgInfo->sData+msgInfo->nLength, szBcdFldLen, 1);
		msgInfo->nLength += 1;
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case POS_LLL_ALPHA :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		sprintf ( szPosFldLen, "%.3d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 3);
		memcpy( msgInfo->sData+msgInfo->nLength, szBcdFldLen, 2);
		msgInfo->nLength += 2;
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case POS_FIX_HEX :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"AddPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case POS_LL_HEX :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		sprintf ( szPosFldLen, "%.2d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 2);
		memcpy( msgInfo->sData+msgInfo->nLength, szBcdFldLen, 1);
		msgInfo->nLength += 1;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case POS_LLL_HEX :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szPosFldLen, "%.3d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 3);
		memcpy( msgInfo->sData+msgInfo->nLength, szBcdFldLen, 2);
		msgInfo->nLength += 2;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case POS_FIX_BIN :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"AddPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case POS_LL_BIN :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		sprintf ( szPosFldLen, "%.2d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 2);
		memcpy( msgInfo->sData+msgInfo->nLength, szBcdFldLen, 1);
		msgInfo->nLength += 1;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case POS_LLL_BIN :
		if( nLength > nMaxLength )
		{
			sprintf(sLine,"AddPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		sprintf ( szPosFldLen, "%.3d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 3);
		memcpy( msgInfo->sData+msgInfo->nLength, szBcdFldLen, 2);
		msgInfo->nLength += 2;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	default :
		return(ECHEC);
	}

	msgInfo->nLength += nBytes;
	/*Start HAL23062020 PLUTONL-1847*/
	for (i=field_no; i< MAX_POS_FIELDS; i++)
       msgInfo->nFieldPos[i+1] = msgInfo->nLength;
	/*End HAL23062020 PLUTONL-1847*/
	SetBit(msgInfo->sBitMap, field_no+1);
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);
	return(SUCCES);

}

int InsertPosField(int field_no, TSPosInfo *msgInfo, char *data, int nLength)
{
	int		nBytes    = 0;
	int		nDigits   = 0;
	int		nBytes1   = 0;
	int		i;
	char	szPosData [BUF_LG_MAX];	
	char	szPosFldLen[4];	
	char	szBcdFldLen[4];	
	int		nMaxLength;
	char	sLine[MAX_LINE_TRC];

	memset( szPosFldLen, '\0', sizeof(szPosFldLen));	
	memset( szBcdFldLen, '\0', sizeof(szBcdFldLen));


	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	nMaxLength = GetPosFieldLength(field_no);


	switch (GetPosFieldType(field_no))
	{
	case POS_FIX_ALPHA :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"InsertPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = nLength;
		memcpy (szPosData, data, nBytes);
		break;

	case POS_LL_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		sprintf ( szPosFldLen, "%.2d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 2);
		memcpy( szPosData,        szBcdFldLen, 1);
		nBytes=1;
		memcpy( szPosData+1, data,        nLength);
		nBytes += nLength;
		break;

	case POS_LLL_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		sprintf ( szPosFldLen, "%.3d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 3);
		nBytes = 2;
		memcpy( szPosData, szBcdFldLen, 2);
		memcpy( szPosData+2, data,        nLength);
		nBytes += nLength;
		break;

	case POS_FIX_HEX :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"InsertPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = AsciiToHex(data, szPosData, nLength);
		break;

	case POS_LL_HEX :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szPosFldLen, "%.2d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 2);
		nBytes1 = 1;
		memcpy( szPosData, szBcdFldLen, nBytes1);
		nBytes = AsciiToHex( data, szPosData+1, nLength);
		nBytes += nBytes1;
		break;

	case POS_LLL_HEX :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szPosFldLen, "%.3d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 3);
		nBytes1 = 2;
		memcpy( szPosData, szBcdFldLen, 2);
		nBytes = AsciiToHex( data, szPosData+2, nLength);
		nBytes += nBytes1;
		break;

	case POS_FIX_BIN :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"InsertPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = nLength;
		memcpy( szPosData, data, nBytes);
		break;

	case POS_LL_BIN :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szPosFldLen, "%.2d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 2);
		nBytes = 1;
		memcpy( szPosData, szBcdFldLen, nBytes);
		memcpy( szPosData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	case POS_LLL_BIN :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"InsertPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szPosFldLen, "%.3d", nLength);	
		nDigits = AsciiToBcd(szPosFldLen, szBcdFldLen, 3);
		nBytes = 2;
		memcpy( szPosData, szBcdFldLen, nBytes);
		memcpy( szPosData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	default :
		return(ECHEC);
	}

	for (i = MAX_POS_FIELDS-1; i > field_no; i--)
	{
		memcpy (msgInfo->sData + msgInfo->nFieldPos[i]+nBytes,
				msgInfo->sData + msgInfo->nFieldPos[i],
				msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
		msgInfo->nFieldPos[i+1] += nBytes;
	}

	memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPosData, nBytes);
	msgInfo->nFieldPos[field_no+1] += nBytes;
	msgInfo->nLength += nBytes;
	SetBit(msgInfo->sBitMap, field_no+1);

	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);
	return(SUCCES);
}

int PutPosField(int field_no, TSPosInfo *msgInfo, char *data, int nLength)
{
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i, nOldFldLen,nOldLen;
	char szPosData [999];	
	char szPosFldLen[4];	
	char szWork    [LG_MAX];
	char szBcdFldLen[4];	
	int		nMaxLength;
	char	sLine[MAX_LINE_TRC];


	memset( szPosFldLen, '\0', sizeof(szPosFldLen));	
	memset( szBcdFldLen, '\0', sizeof(szPosFldLen));	

	nMaxLength = GetPosFieldLength(field_no);


	if ( !CheckBit( msgInfo->sBitMap, field_no))
	{
		return ( InsertPosField (field_no, msgInfo, data, nLength));
	}

	switch (GetPosFieldType(field_no))
	{
	case POS_FIX_ALPHA :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"PutPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = nLength;
		memcpy (szPosData, data, nBytes);
		break;

	case POS_LL_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"PutPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		sprintf ( szPosFldLen, "%.2d", nLength);	
		nBytes = AsciiToBcd(szPosFldLen, szBcdFldLen, 2);
		memcpy( szPosData, szBcdFldLen, 1);
		nBytes = 1;
		memcpy( szPosData+1, data,        nLength);
		nBytes += nLength;
		break;
		

	case POS_LLL_ALPHA :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"PutPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		sprintf ( szPosFldLen, "%.3d", nLength);	
		nBytes = 3;
		memcpy( szPosData, szPosFldLen, 3);
		memcpy( szPosData+3, data,        nLength);
		nBytes += nLength;
		break;

	case POS_FIX_HEX :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"PutPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}

		nBytes = AsciiToHex(data, szPosData, nLength);
		break;

	case POS_LL_HEX :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"PutPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szPosFldLen, "%.2d", nLength);	
		nBytes1 = 2;
		memcpy( szPosData, szPosFldLen, nBytes1);
		nBytes = AsciiToHex( data, szPosData+2, nLength);
		nBytes += nBytes1;
		break;

	case POS_LLL_HEX :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"PutPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szPosFldLen, "%.3d", nLength);	
		nBytes1 = 3;
		memcpy( szPosData, szPosFldLen, 3);
		nBytes = AsciiToHex( data, szPosData+3, nLength);
		nBytes += nBytes1;
		break;

	case POS_FIX_BIN :
		if ( nMaxLength != nLength)
		{
			sprintf(sLine,"PutPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		nBytes = nLength;
		memcpy( szPosData, data, nBytes);
		break;

	case POS_LL_BIN :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"PutPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szPosFldLen, "%.2d", nLength);	
		nBytes = 2;
		memcpy( szPosData, szPosFldLen, nBytes);
		memcpy( szPosData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	case POS_LLL_BIN :
		if ( nLength > nMaxLength )
		{
			sprintf(sLine,"PutPosField(%d) invalid length Max: %d VS %d",field_no,nMaxLength,nLength);
			trace_event(sLine,ERROR);
			return(ECHEC);
		}
		sprintf ( szPosFldLen, "%.3d", nLength);	
		nBytes = 3;
		memcpy( szPosData, szPosFldLen, nBytes);
		memcpy( szPosData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	default :
		return(ECHEC);
	}

	nOldFldLen = msgInfo->nFieldPos[field_no +1] - msgInfo->nFieldPos[field_no];
	memset(szWork, 0, sizeof(szWork));
	nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
	memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

	for (i = MAX_POS_FIELDS-1; i > field_no; i--)
	{
		msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
	}

	memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPosData, nBytes);
	msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
	msgInfo->nLength += nBytes-nOldFldLen;
	SetBit(msgInfo->sBitMap, field_no+1);
	memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);

	memset( szPosData, '\0', sizeof(szPosData));

	return(SUCCES);


}

int PosBuildMsg (char * buffer_snd, TSPosInfo *msgInfo)
{
	char szMsgType[5];
	char szMsgLen [5];
	int  nBytes;
	int  iMapCtr;
	int  nLenHead = sizeof(TSPosHeader);


	sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
	memcpy( buffer_snd+nLenHead, szMsgType, 4);

	iMapCtr = 1;
	while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
		++iMapCtr;

	sprintf(szMsgLen , "%.4d", msgInfo->nLength + 4 + nLenHead + (iMapCtr*8));
	memcpy(msgInfo->sHeader.sMsgLen, szMsgLen, 4);

	memcpy(buffer_snd, (char *)&(msgInfo->sHeader), nLenHead);

	memcpy( buffer_snd+4+nLenHead, msgInfo->sBitMap, iMapCtr*8);
	memcpy( buffer_snd+4 + nLenHead+ (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
	return( 4 + (iMapCtr*8) + nLenHead + msgInfo->nLength);
}

/****************************************************************************/
/*** Gestion du Champs 55 (EMV)                                            **/
/****************************************************************************/
int GetPosIcTag (char  *tag_name, TSTagPos *tagInfo, char *data, int *length)
{
	int nIndice ;
	int nLenTag ;
	int nDigits;

	if ( (nIndice = GetPosTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
		return(ECHEC);

	if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice])) & 0x1F == 0x1F)
		nLenTag = 2;
	else
		nLenTag = 1;
	*length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag + 1));
	switch (GetPosTagType(nIndice) )
	{
	case POS_TAG_FIX_BIN :
	case POS_TAG_FIX_ALPHA :
		memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
			*length);
		break;

	case POS_TAG_VAR_BCD :
		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
			nLenTag+1, 
			data, 
			(*length)*2);
		*length = (*length)*2;
		break;

	case POS_TAG_FIX_BCD :
		nDigits = BcdToAscii( data, 
			tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
			nLenTag+1, 
			(*length)*2);
		*length = (*length)*2;
		break;

	case POS_TAG_VAR_BIN :
	case POS_TAG_VAR_ALPHA :
		memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
			*length);
		break;

	}
	return(SUCCES);
}

int AddPosIcTag (char  *tag_name, TSTagPos *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;

	if ( (nIndice = GetPosTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == PRESENT)
		return(ECHEC);

	AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);
	if ( (*(tagInfo->sChipData + tagInfo->nLength)) & 0x1F == 0x1F)
		nLenTag = 2;
	else 
		nLenTag = 1;
	switch (GetPosTagType(nIndice) )
	{
	case POS_TAG_FIX_BIN :
	case POS_TAG_FIX_ALPHA :
		if ( GetPosTagLength(nIndice) != length)
			return(ECHEC);
		memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
		nDigits = length;
		break;

	case POS_TAG_VAR_BCD :
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case POS_TAG_FIX_BCD :
		if ( GetPosTagLength(nIndice) != length)
			return(ECHEC);
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case POS_TAG_VAR_BIN :
	case POS_TAG_VAR_ALPHA :
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

int PutPosIcTag (char  *tag_name, TSTagPos *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;
	int nPutPos, nTagPresent = NOT_PRESENT;
	char szLastPart  [ MAX_POS_CHIP_LEN ];
	int nOldTagLen, i;

	if ( (nIndice = GetPosTagIndice(tag_name)) == -1)
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

	switch (GetPosTagType(nIndice) )
	{
	case POS_TAG_FIX_BIN :
	case POS_TAG_FIX_ALPHA :
		if ( GetPosTagLength(nIndice) != length)
			return(ECHEC);
		memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
		nDigits = length;
		break;

	case POS_TAG_VAR_BCD :
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case POS_TAG_FIX_BCD :
		if ( GetPosTagLength(nIndice) != length)
			return(ECHEC);
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case POS_TAG_VAR_BIN :
	case POS_TAG_VAR_ALPHA :
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
		for (i = nIndice + 1; i < MAX_POS_CHIP_TAG; i++)
		{
			if (tagInfo->nPresent[i] == PRESENT)
				tagInfo->nPosTag[i] += nDigits - nOldTagLen;
		}
	}
	return(SUCCES);
}

int PosBuildIcFld (char * buffer_snd, TSTagPos *tagInfo)
{
	memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
	return(tagInfo->nLength);
}

