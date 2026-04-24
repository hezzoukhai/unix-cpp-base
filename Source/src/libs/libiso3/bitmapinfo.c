#include <p7_common.h>
#include <iso8583.h>

/******************************************************************************/
void InitBitMappedInfo(const char* szPropertiesName, BitMappedInfo* pkBitMappedInfo)
{
	pkBitMappedInfo->pkProperties = GetBitMappedProperties(szPropertiesName);
	ResetBitMappedInfo(pkBitMappedInfo);
}
/******************************************************************************/
void ResetBitMappedInfo(BitMappedInfo* pkBitMappedInfo)
{
	
	pkBitMappedInfo->nLength  = 0;
	memset(pkBitMappedInfo->sBitMap,0,sizeof(pkBitMappedInfo->sBitMap));
	memset(pkBitMappedInfo->sData,0,sizeof(pkBitMappedInfo->sData));
	memset(pkBitMappedInfo->nPresent,0, sizeof(pkBitMappedInfo->nPresent));
	memset(pkBitMappedInfo->nPos,0, sizeof(pkBitMappedInfo->nPos));
}
/******************************************************************************\
* Return the field number of bytes
\******************************************************************************/
static int		
GetFieldLength(const BitMappedInfo* pkBitMappedInfo, int nFieldNo, const char* data, int nTotalLenFlag)
{
	int		nBytes  = 0;
	int		nLengthUnit =0;
	int		nLenAdj = 0;
	char	szFieldLen[12];

	memset (szFieldLen, '\0' ,sizeof(szFieldLen));	
	switch ( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eLengthType )
	{
	case FL_FIX:                           
		nLengthUnit = pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.nLength;
		break;
	case FL_L:
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy(szFieldLen, data ,1);
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			EbcdicToAscii(data, szFieldLen, 1);
		}
		nLengthUnit = atoi(szFieldLen);
		if( nTotalLenFlag == OK )
			nLenAdj=1;
		break;
	case FL_LL :                     
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy(szFieldLen, data ,2);
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			EbcdicToAscii(data, szFieldLen, 2);
		}
		nLengthUnit = atoi(szFieldLen);
		if( nTotalLenFlag == OK )
			nLenAdj=2;
		break;
	case FL_LLL : 
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy(szFieldLen, data ,3);
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			EbcdicToAscii(data, szFieldLen, 3);
		}
		nLengthUnit = atoi(szFieldLen); 
		if( nTotalLenFlag == OK )
			nLenAdj=3;
		break;
	case FL_B :                           
		nLengthUnit = (unsigned char)data[0]; 
		if( nTotalLenFlag == OK )
			nLenAdj=1;
		break;
	case FL_BB:
		nLengthUnit = ((unsigned char)data[0])*256 + (unsigned char)data[1]; 
		if( nTotalLenFlag == OK )
			nLenAdj=2;
		break;
	case FL_D :                           
		BcdToAscii(data, szFieldLen, 2);
		nLengthUnit = atoi(szFieldLen);
		if( nTotalLenFlag == OK )
			nLenAdj=1;
		break;
	case FL_DD:
		BcdToAscii(data, szFieldLen, 3);
		nLengthUnit = atoi(szFieldLen); 
		if( nTotalLenFlag == OK )
			nLenAdj=2;
		break;
	default :
		nLengthUnit = 0;
		break;
	}
	
	
	if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eLengthUnit == LU_NIBBLES )
	{
		nBytes = (nLengthUnit + 1 )/2;
	}
	else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eLengthUnit == LU_BYTE )
	{
		nBytes = nLengthUnit;
	}
	
	nBytes += nLenAdj;

	return nBytes;
	
}
/******************************************************************************\
* Return the field number of characters
\******************************************************************************/
static int		
GetFieldCharLength(const BitMappedInfo* pkBitMappedInfo, int nFieldNo, const char* data)
{
	int		nLengthUnit =0;
	char	szFieldLen[12];

	memset (szFieldLen, '\0' ,sizeof(szFieldLen));	
	switch ( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eLengthType )
	{
	case FL_FIX:
		nLengthUnit = pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.nLength;
		break;
	case FL_L:
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy(szFieldLen, data ,1);
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			EbcdicToAscii(data, szFieldLen, 1);
		}
		nLengthUnit = atoi(szFieldLen);
		break;
	case FL_LL :                     
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy(szFieldLen, data ,2);
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			EbcdicToAscii(data, szFieldLen, 2);
		}
		nLengthUnit = atoi(szFieldLen);
		break;
	case FL_LLL : 
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy(szFieldLen, data ,3);
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			EbcdicToAscii(data, szFieldLen, 3);
		}
		nLengthUnit = atoi(szFieldLen); 
		break;
	case FL_B :                           
		nLengthUnit = (unsigned char)data[0]; 
		break;
	case FL_BB:
		nLengthUnit = ((unsigned char)data[0])*256 + (unsigned char)data[1]; 
		break;
	case FL_D :                           
		BcdToAscii(data, szFieldLen, 2);
		nLengthUnit = atoi(szFieldLen); 
		break;
	case FL_DD:
		BcdToAscii(data, szFieldLen, 3);
		nLengthUnit = atoi(szFieldLen); 
		break;
	default :
		nLengthUnit = 0;
		break;
	}

	if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eLengthUnit == LU_BYTE )
	{
		switch(pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eFormatType)
		{
		case FF_HEX:
		case FF_BCD:
			nLengthUnit = nLengthUnit*2;
			break;
		case FF_X_BCD:
			P7_ASSERT(nLengthUnit > 0 );
			nLengthUnit = (nLengthUnit - 1)*2;
			break;
		default:
			break;
		}
	}

	return nLengthUnit;
}
/*****************************************************************************/
static int		
GetFieldPosition(const BitMappedInfo* pkBitMappedInfo,int nFieldNo)
{
	int		nBytes  = 0;
	
	switch ( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eLengthType )
	{
	case FL_FIX:
		return pkBitMappedInfo->nPos[nFieldNo];
	case FL_L:
	case FL_B:
	case FL_D:
		return pkBitMappedInfo->nPos[nFieldNo] + 1;
	case FL_LL :
	case FL_BB:
	case FL_DD:
		return pkBitMappedInfo->nPos[nFieldNo] + 2;
	case FL_LLL : 
		return pkBitMappedInfo->nPos[nFieldNo] + 3; 
	default :
		P7_ASSERT( 0 && "Unknown length type");
		return -1;
	}

	P7_ASSERT( 0 && "Exception ");
	return -1;
}
/******************************************************************************/
int		
RemoveBitMappedInfoField(int nFieldNo, BitMappedInfo* pkBitMappedInfo)
{
	int		nBytes;
	int		i;
	int		nFieldPos;
	char	sBuffer[4096];

	if (!CheckBit (pkBitMappedInfo->sBitMap, nFieldNo))
	{
		return OK;
	}

	nFieldPos = pkBitMappedInfo->nPos[nFieldNo];
	nBytes = GetFieldLength(pkBitMappedInfo,nFieldNo,pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo],OK);

	memcpy(sBuffer,
			pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo] + nBytes, 
			pkBitMappedInfo->nLength - nBytes);
	memcpy(pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo],
			sBuffer,
			pkBitMappedInfo->nLength - nBytes);

	pkBitMappedInfo->nPos[nFieldNo] = 0;

	for( i = 0; i < pkBitMappedInfo->pkProperties->nNbFields; i++ )
	{
		if ( pkBitMappedInfo->nPos[i] > nFieldPos )
		{
			pkBitMappedInfo->nPos[i] -= nBytes;
		}
	}

	pkBitMappedInfo->nLength -= nBytes; /*EBE150827*/
        /*ClearBit (pkBitMappedInfo->sBitMap, nFieldNo);*//*PLUTONL-3595*/
		  ClearBit2 (pkBitMappedInfo->sBitMap, nFieldNo);

	return OK;

}
/******************************************************************************/
int  AnalyseBitMappedInfo(const char *buffer_in, int nLength, BitMappedInfo* pkBitMappedInfo)
{
	int		nBytes  = 0;
	int		i = 0;
	int		iMapCtr = 0;
	int		nBitmapLen;
	int		nNbConsBitmaps;
	int		nMaxNbFields;
	const unsigned char	*sPtr;
	int		offset;
	char            sLine[200];
	

	ResetBitMappedInfo(pkBitMappedInfo);

	if( nLength > sizeof(pkBitMappedInfo->sData))
		return NOK;


	sPtr = (unsigned char*)buffer_in;
	
	
	nBitmapLen = pkBitMappedInfo->pkProperties->nBitmapLen;
	nNbConsBitmaps = pkBitMappedInfo->pkProperties->nNbConsBitmaps;


	do
	{
		if( pkBitMappedInfo->pkProperties->eBitmapFmt == FF_BIN )
		{
			memcpy( pkBitMappedInfo->sBitMap + iMapCtr*nBitmapLen, sPtr, nBitmapLen);
			sPtr+=nBitmapLen;
		}
		else
		{
			AsciiToHex(sPtr,pkBitMappedInfo->sBitMap + (iMapCtr*nBitmapLen), nBitmapLen*2); 
			sPtr+=(nBitmapLen*2);
		}
		++iMapCtr;
	} while(( pkBitMappedInfo->sBitMap[(iMapCtr - 1) * nBitmapLen] & 0x80)   && (iMapCtr < nNbConsBitmaps));

	

	offset = 0;
	memcpy( pkBitMappedInfo->sData, sPtr, nLength);

	nMaxNbFields = (nBitmapLen*8)*iMapCtr;



	/*EBE160418
	for (i = 1; i < nMaxNbFields; ++i)*/
	for (i = 0; i < nMaxNbFields; ++i)
	{
		if ( !CheckBit( pkBitMappedInfo->sBitMap, i))
		{
			continue;
		}
		nBytes  = GetFieldLength(pkBitMappedInfo,i,sPtr,OK);
		pkBitMappedInfo->nPos[i] = offset;
		offset+= nBytes;
		sPtr += nBytes;
	}

	/*EBEBITMAP*/
	/* we have an additional nested bitmap(e.g field 65)*/
	if( pkBitMappedInfo->pkProperties->nNbBitmaps > nNbConsBitmaps )
	{
		if ( CheckBit( pkBitMappedInfo->sBitMap, nBitmapLen*8) )
		{
			nMaxNbFields += (nBitmapLen*8);
		 if( pkBitMappedInfo->pkProperties->eBitmapFmt == FF_BIN )
		 {
			memcpy(	pkBitMappedInfo->sBitMap + (nBitmapLen*2), 
					pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nBitmapLen*8],
					nBitmapLen);
		}
		else
		{
			AsciiToHex(pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nBitmapLen*8],pkBitMappedInfo->sBitMap + (nBitmapLen*2),16);
		}

			for (; i < nMaxNbFields; ++i)
			{
				if ( !CheckBit( pkBitMappedInfo->sBitMap, i))
				{
					continue;
				}
				nBytes  = GetFieldLength(pkBitMappedInfo,i,sPtr,OK);
				pkBitMappedInfo->nPos[i] = offset;
				offset+= nBytes;
				sPtr += nBytes;
			}

		}
	}


	pkBitMappedInfo->nLength = offset;

	return OK;

}
/******************************************************************************/
int  GetBitMappedInfoField (int nFieldNo, const BitMappedInfo* pkBitMappedInfo, char *data, int* nLength)
{
	int		nNbChar;
	int		nFieldPos;

	if (!CheckBit (pkBitMappedInfo->sBitMap, nFieldNo))
	{
		(*nLength)=0;
		data[0] = '\0';
		return NOK;
	}

	nFieldPos = GetFieldPosition(pkBitMappedInfo,nFieldNo);
	if( nFieldPos < 0 )
	{
		(*nLength)=0;
		data[0] = '\0';
		return NOK;
	}

	nNbChar  = GetFieldCharLength(pkBitMappedInfo,nFieldNo,pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo]);

	switch (pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eFormatType)
	{
	case FF_ALPHA_A:
	case FF_ALPHA_N:
	case FF_ALPHA_AN:
	case FF_ALPHA_ANS :
	case FF_Z :/*PLUTONL-3131*/
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy( data, pkBitMappedInfo->sData + nFieldPos, nNbChar);
			
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			EbcdicToAscii( pkBitMappedInfo->sData + nFieldPos,  data, nNbChar);
		}
		(*nLength) = nNbChar;
		break;
	case FF_BIN :
		memcpy( data, pkBitMappedInfo->sData + nFieldPos, nNbChar);
		(*nLength) = nNbChar;
		break;
	case FF_HEX :
	
		HexToAscii( (unsigned char*)pkBitMappedInfo->sData + nFieldPos, data, nNbChar);
		(*nLength) = nNbChar;
		break;
	case FF_BCD :
		BcdToAscii( (unsigned char*)pkBitMappedInfo->sData + nFieldPos, data, nNbChar);
		(*nLength) = nNbChar;
		break;
	default :
		(*nLength) = 0;
		data[0] = '\0';
		return(NOK);
		break;
	}

	data[(*nLength)] = '\0';

	return OK;

}
/******************************************************************************/
int  PutBitMappedInfoField (int nFieldNo, BitMappedInfo* pkBitMappedInfo, const char *data, int nLength)
{
	int				nBytes  = 0;
	char			szIsoFldLen[12];
	char			szLen[12];
	/*char			szLocData[1024];*/
	int				nLengthLen;
	int				nFieldMaxLength;
	E_FIELD_LENGTH	eLengthType;
	E_LENGTH_UNIT	eLengthUnit;
	E_FIELD_FORMAT	eFormatType;
	int				nLenToPut;
	int				nBitmapLen;

	memset(szIsoFldLen,'\0',sizeof(szIsoFldLen));
	memset(szLen,'\0',sizeof(szLen));

	if( nLength == 0 )
	{
		return OK;
	}

	nBitmapLen = pkBitMappedInfo->pkProperties->nBitmapLen;

	if (CheckBit(pkBitMappedInfo->sBitMap, nFieldNo))
	{
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eLengthType != FL_FIX	)
		{
		

			RemoveBitMappedInfoField(nFieldNo,pkBitMappedInfo);
			pkBitMappedInfo->nPos[nFieldNo] = pkBitMappedInfo->nLength;
		}
	}
	else
	{
		pkBitMappedInfo->nPos[nFieldNo] = pkBitMappedInfo->nLength;
	}

	nFieldMaxLength = pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.nLength;
	eLengthType		= pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eLengthType;
	eLengthUnit		= pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eLengthUnit;
	eFormatType		= pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eFormatType;

	nLenToPut = nLength;

	if( eLengthUnit == LU_BYTE   )
	{
		switch(eFormatType)
		{
		case FF_HEX:
		case FF_BCD:
			nFieldMaxLength = nFieldMaxLength*2;
			nLenToPut = (nLength + 1)/2;
			break;
		case FF_X_BCD:
			P7_ASSERT(nFieldMaxLength > 0 );
			nFieldMaxLength = (nFieldMaxLength - 1)*2;
			nLenToPut = (nLength)/2 + 1;
			break;
		}
	}

	if( eLengthType == FL_FIX )
	{
		if( nLength != nFieldMaxLength )
		{
			return NOK;
		}
	}
	else
	{
		if( nLength > nFieldMaxLength )
		{
			return NOK;
		}
	}

	switch ( eLengthType )
	{
	case FL_B:
		szIsoFldLen[0]=(unsigned char)nLenToPut;
		nLengthLen = 1;
		break;
	case FL_BB:
		szIsoFldLen[0]=(unsigned char)(nLenToPut/255);
		szIsoFldLen[1]=(unsigned char)(nLenToPut%255);
		nLengthLen = 2;
		break;
	case FL_D:
		sprintf (szLen, "%.2d", nLenToPut);
		AsciiToBcd ( szLen, szIsoFldLen, 2);
		nLengthLen = 1;
		break;
	case FL_DD:
		sprintf (szLen, "%.3d", nLenToPut);
		AsciiToBcd ( szLen, szIsoFldLen, 3);
		nLengthLen = 2;
		break;
	case FL_L:
		sprintf (szLen, "%.1d", nLenToPut);
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			strncpy(szIsoFldLen,szLen,sizeof(szIsoFldLen) - 1);
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			AsciiToEbcdic ( szLen, szIsoFldLen, 1);
		}
		nLengthLen = 1;
		break;
	case FL_LL :
		sprintf (szLen, "%.2d", nLenToPut);
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			strncpy(szIsoFldLen,szLen,sizeof(szIsoFldLen) - 1);
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			AsciiToEbcdic ( szLen, szIsoFldLen, 2);
		}		
		nLengthLen = 2;
		break;
	case FL_LLL : 
		sprintf (szLen, "%.3d", nLenToPut);
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			strncpy(szIsoFldLen,szLen,sizeof(szIsoFldLen) - 1);
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			AsciiToEbcdic ( szLen, szIsoFldLen, 3);
		}
		nLengthLen = 3;
		break;
	default :
		nLengthLen = 0;
		break;
	}

	if( nLengthLen > 0 )
	{
		memcpy( pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo], szIsoFldLen, nLengthLen);
	}

	/**memcpy( pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo] + nLengthLen, szLocData, nLength);**/

	switch (pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eFormatType)
	{
	case FF_ALPHA_A :
	case FF_ALPHA_N :
	case FF_ALPHA_AN :
	case FF_ALPHA_ANS :
	case FF_Z :/*PLUTONL-3131*/
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy( pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo] + nLengthLen, data, nLength);
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eAlphaFormat == AF_EBCDIC )
		{
			AsciiToEbcdic ( data, pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo] + nLengthLen, nLength);
		}
		nBytes = nLength;
		break;
	case FF_BIN:
		memcpy( pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo] + nLengthLen, data, nLength);
		nBytes = nLength;
		break;
	case FF_HEX :
		nBytes = AsciiToHex(data, (char*)pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo] + nLengthLen, nLength);
		break;
	case FF_BCD :
		nBytes = AsciiToBcd(data, (char*)pkBitMappedInfo->sData + pkBitMappedInfo->nPos[nFieldNo] + nLengthLen, nLength);
		break;
	default :
		return(NOK);
	}


	pkBitMappedInfo->nLength += nBytes + nLengthLen;

	SetBit2(pkBitMappedInfo->sBitMap, nFieldNo);
	/*
	if ( (nFieldNo >64) && (nFieldNo < 128))
		SetBit(pkBitMappedInfo->sBitMap, 0);
		*/
	if ( ( nFieldNo >(1*nBitmapLen*8)) && (nFieldNo < (2*nBitmapLen*8)))
	{
		SetBit2(pkBitMappedInfo->sBitMap, 0);
	}
	else if( ( nFieldNo >(2*nBitmapLen*8)) && (nFieldNo < (3*nBitmapLen*8)))
	{
		SetBit2(pkBitMappedInfo->sBitMap, 0);
		if ( !CheckBit( pkBitMappedInfo->sBitMap, nBitmapLen*8) && 
			pkBitMappedInfo->pkProperties->nNbBitmaps > pkBitMappedInfo->pkProperties->nNbConsBitmaps )
		{
			/*just occupy the 8 bytes and set position of bitmap3 */
			PutBitMappedInfoField(nBitmapLen*8,pkBitMappedInfo,pkBitMappedInfo->sBitMap + (nBitmapLen*2),nBitmapLen);
		}
		SetBit2(pkBitMappedInfo->sBitMap, nBitmapLen*8);
	}

	return(OK);
}
/******************************************************************************/
int  AddBitMappedInfoField (int nFieldNo, BitMappedInfo* pkBitMappedInfo, const char *data, int nLength)
{
	if (CheckBit(pkBitMappedInfo->sBitMap, nFieldNo))
		return NOK;

	return PutBitMappedInfoField(nFieldNo,pkBitMappedInfo,data,nLength);
}
/******************************************************************************/
int  BuildBitMappedInfoBuffer (char * buffer_snd, const BitMappedInfo* pkBitMappedInfo)
{
	int		nBytes;
	int		iMapCtr;
	int		i;
	int		offset = 0;
	int		nBitmapLen;
	int		nNbBitmaps;
	int		nNbConsBitmaps;
	int		nMaxNbFields;
	char	sNullBitmap[8];
	char	sData[128];
	
	iMapCtr = 1;
	nBitmapLen = pkBitMappedInfo->pkProperties->nBitmapLen;
	nNbBitmaps		= pkBitMappedInfo->pkProperties->nNbBitmaps;
	nNbConsBitmaps	= pkBitMappedInfo->pkProperties->nNbConsBitmaps;
	/*EBEBITMAP*/
	if ( nNbBitmaps > nNbConsBitmaps	&& 
		 CheckBit( pkBitMappedInfo->sBitMap, nBitmapLen*8) )
	{
	memset(sNullBitmap,0,sizeof(sNullBitmap));
		if( memcmp(pkBitMappedInfo->sBitMap + (nBitmapLen*2),sNullBitmap,nBitmapLen) != 0 )
	{
			/*IBO*/
			if( pkBitMappedInfo->pkProperties->eBitmapFmt != FF_BIN)
			{
				memset(sData,'\0',sizeof(sData));
				HexToAscii(pkBitMappedInfo->sBitMap + (nBitmapLen*2),sData,16);
				PutBitMappedInfoField(nBitmapLen*8,pkBitMappedInfo,sData,16);
			}
			else
			{
				PutBitMappedInfoField(nBitmapLen*8,pkBitMappedInfo,pkBitMappedInfo->sBitMap + (nBitmapLen*2),nBitmapLen);
			}
			/*IBO*/
		}
	}

	


	while ( ( pkBitMappedInfo->sBitMap[(iMapCtr - 1) * nBitmapLen] & 0x80) && (iMapCtr < nNbConsBitmaps) )
		++iMapCtr;
	
	
	if( pkBitMappedInfo->pkProperties->eBitmapFmt == FF_BIN)
	{
		memcpy( buffer_snd + offset, pkBitMappedInfo->sBitMap, iMapCtr*nBitmapLen); 
		offset +=iMapCtr*nBitmapLen;
	}
	else
	{
		HexToAscii(pkBitMappedInfo->sBitMap,buffer_snd+offset,iMapCtr*nBitmapLen*2); 
		offset+=(iMapCtr*nBitmapLen*2);
	}

	/*EBEBITMAP*/
	if( nNbBitmaps > nNbConsBitmaps &&
		CheckBit( pkBitMappedInfo->sBitMap, nBitmapLen*8))
	{
		iMapCtr++;
	}


	nMaxNbFields = iMapCtr * 64;
	/*EBE160418
	for (i = 1; i < iMapCtr * 64; ++i)*/
	for (i = 0; i < nMaxNbFields; ++i)
	{
		if ( !CheckBit( pkBitMappedInfo->sBitMap, i))
		{
			continue;
		}
		nBytes = GetFieldLength(pkBitMappedInfo,i,pkBitMappedInfo->sData + pkBitMappedInfo->nPos[i],OK);
		memcpy( buffer_snd + offset,pkBitMappedInfo->sData + pkBitMappedInfo->nPos[i],nBytes); 
		offset+=nBytes;
	}
	
	return(offset);

}
/******************************************************************************/
/*Start AMER20180207 PROD00052530*/
void DumpFileBitMappedInfo(const BitMappedInfo* pkBitMappedInfo, int pMsgType)
{
	char	sLine[MAX_LINE_TRC];
	char    sLineBuffer[MAX_LINE_TRC];
	int		nOffset = 0;
	int		i, k;
	int		iMapCtr;
	int		nLen;
	char	szField[MAX_FIELD_LEN];
	char	szFieldAdd[MAX_FIELD_LEN];
	int		nBitmapLen;

	memset(sLine,'\0',sizeof(sLine));
	memset(szField,'\0',sizeof(szField));
	
	trace_event("------------------------------------",TRACE);
	sprintf(sLine, "Start DumpFileBitMappedInfo()");
	trace_event(sLine,TRACE);
	trace_event("------------------------------------",TRACE);
 	
	sprintf(sLine,"MESSAGE_ISO3_XML_FORMAT_START[%04d]" , pMsgType);
    trace_event(sLine,TRACE);
	
	sprintf(sLine,"<?xml version=\"1.0\"?>");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<BaseMessage Description=\"Iso3 base message template\">");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<MessageInfo>");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<MsgType Value=\"%04d\"/>" , pMsgType);
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"<MsgHeader Value=\"\"/>");
	trace_event(sLine,TRACE);
	
	iMapCtr = 1;
	nBitmapLen = pkBitMappedInfo->pkProperties->nBitmapLen;

	while ( ( pkBitMappedInfo->sBitMap[(iMapCtr - 1) * nBitmapLen] & 0x80)  && (iMapCtr < pkBitMappedInfo->pkProperties->nNbBitmaps) )
		++iMapCtr;

	for (i = 1; i < iMapCtr * 64; ++i)
	{
		if ( !CheckBit( pkBitMappedInfo->sBitMap, i))
		{
			continue;
		}
		if ( GetBitMappedInfoField(i,pkBitMappedInfo, szField, &nLen ) != OK )
		{
			continue;
		}
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eFormatType != FF_BIN )
		{
			sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , szField);
			k=0;  
			
			while(k < strlen(sLine)) 
			{
				memset(sLineBuffer,0,sizeof(sLineBuffer));
				if((strlen(sLine)-k)>93)
					memcpy(sLineBuffer,sLine+k,93);
				else
					memcpy(sLineBuffer,sLine+k,strlen(sLine)-k);
			
				trace_event(sLineBuffer ,TRACE);
				k+=93;
			}
		}
		else
		{
			nLen = HexToAscii(szField, szFieldAdd, nLen*2);
			szFieldAdd[nLen]='\0';/*PLUTONL-5279 AHA20221021*/
			sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , szFieldAdd);
			k=0;  
			
			while(k < strlen(sLine)) 
			{
				memset(sLineBuffer,0,sizeof(sLineBuffer));
				if((strlen(sLine)-k)>93)
					memcpy(sLineBuffer,sLine+k,93);
				else
					memcpy(sLineBuffer,sLine+k,strlen(sLine)-k);
			
				trace_event(sLineBuffer ,TRACE);
				k+=93;
			}
		}
	}
	
	sprintf(sLine,"</MessageInfo>");
	trace_event(sLine,TRACE);
	
	sprintf(sLine,"</BaseMessage>");
	trace_event(sLine,TRACE);
	
    sprintf(sLine,"MESSAGE_ISO3_XML_FORMAT_END[%04d]" , pMsgType);
	trace_event(sLine,TRACE);
	
	trace_event  ("------------------------------------",TRACE);
	sprintf(sLine, "End DumpFileBitMappedInfo()");
	trace_event  (sLine,TRACE);
}
/*End AMER20180207 PROD00052530*/
/******************************************************************************/
void DumpBitMappedInfo(const BitMappedInfo* pkBitMappedInfo)
{

	char	sLine[MAX_LINE_TRC];
	int		nOffset = 0;
	int		i;
	int		iMapCtr;
	int		nLen;
	char	szField[MAX_FIELD_LEN];
	char	szFieldAdd[MAX_FIELD_LEN];
	int		nBitmapLen;
	char 	CardTrunc[22 + 1];
	
	if( is_trace_level_on(P7_TL_TRACE) != OK)
		return;

	memset(sLine,'\0',sizeof(sLine));
	memset(szField,'\0',sizeof(szField));
	memset(CardTrunc, 0, sizeof(CardTrunc));
	
	iMapCtr = 1;
	nBitmapLen = pkBitMappedInfo->pkProperties->nBitmapLen;

	while ( ( pkBitMappedInfo->sBitMap[(iMapCtr - 1) * nBitmapLen] & 0x80)  && (iMapCtr < pkBitMappedInfo->pkProperties->nNbBitmaps) )
		++iMapCtr;

	memcpy(sLine,"- BIT MAP  : ",13); nOffset+=13;
	for (i=0; i<iMapCtr * nBitmapLen;i++)
	{
		sprintf(sLine + nOffset," %02X", (unsigned char)pkBitMappedInfo->sBitMap[i]);
		nOffset += 3;
	}
	trace_event(sLine,P7_TL_TRACE);

	trace_event("------------------------------------",P7_TL_TRACE);
	trace_event("- FLD (FIELD): LENGTH :  DATA       ",P7_TL_TRACE);
	trace_event("------------------------------------",P7_TL_TRACE);

	for (i = 1; i < iMapCtr * 64; ++i)
	{
		if ( !CheckBit( pkBitMappedInfo->sBitMap, i))
		{
			continue;
		}
		if ( GetBitMappedInfoField(i,pkBitMappedInfo, szField, &nLen ) != OK )
		{
			trace_message(P7_TL_DATA,"Field set on bitmap, GetMsgField failed: [%u]",i);
			continue;
		}
		/* Start ACH20200221 PLUTONL-1274*/

		if (debug_option == 1 || pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.nPrintPolicy == PP_PRINT)
		{
			if (pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eFormatType != FF_BIN)
			{
				trace_message(P7_TL_TRACE, "- FLD (%03d) : (%03d) : [%s] ", i + 1, nLen, szField);
			}
			else
			{
				nLen = HexToAscii(szField, szFieldAdd, nLen * 2);
				szFieldAdd[nLen]='\0';/*PLUTONL-5279 AHA20221021*/
				trace_message(P7_TL_TRACE, "- FLD (%03d) : (%03d) : [%s] ", i + 1, nLen, szFieldAdd);
			}
		}
		else
		{
			if (pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.nPrintPolicy == PP_ENCRYPT)
			{
				if (i == 1)
				{
					TrunCard(szField, CardTrunc);
					trace_message(P7_TL_TRACE, "- FLD (%03d) : (%03d) : [%s] ", i + 1, nLen, CardTrunc);
				}
				else
				{
					if (pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eFormatType != FF_BIN)
					{
						memset(szField, '*', nLen);
						trace_message(P7_TL_TRACE, "- FLD (%03d) : (%03d) : [%s] ", i + 1, nLen, szField);
					}
					else
					{
						nLen = HexToAscii(szField, szFieldAdd, nLen * 2);
						szFieldAdd[nLen]='\0';/*PLUTONL-5279 AHA20221021*/
						memset(szFieldAdd, '*', nLen);
						trace_message(P7_TL_TRACE, "- FLD (%03d) : (%03d) : [%s] ", i + 1, nLen, szFieldAdd);
					}
				}
			}
			else /* PP_HIDE */
			{
				continue;
			}
		}

		/* End ACH20200221 PLUTONL-1274*/
		
		/*
		switch( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eFieldType)
		{
		case FT_SINGLE:
			if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eFormatType != FF_BIN )
			{
				trace_message(P7_TL_TRACE,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLen ,szField );
			}
			else
			{
				nLen = HexToAscii(szField, szFieldAdd, nLen*2);
				trace_message(P7_TL_TRACE,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLen ,szFieldAdd );
			}
			break;
		case FT_STRUCT_TLV:
			{
				TlvInfo	kTlvInfo;
				InitTlvInfo(pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kTlvInfo);
				AnalyseTlvInfo(szField,nLen,&kTlvInfo);
				DumpTlvInfo(&kTlvInfo);
			}
			break;
		case FT_STRUCT_STATIC:
			{
				StaticInfo	kStaticInfo;
				InitStaticInfo(pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kStaticInfo);
				AnalyseStaticInfo(szField,nLen,&kStaticInfo);
				DumpStaticInfo(&kStaticInfo);
			}
			break;
		case FT_STRUCT_BER:
			{
				BerInfo	kBerInfo;
				InitBerInfo(pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kBerInfo);
				AnalyseBerInfo(szField,nLen,&kBerInfo);
				DumpBerInfo(&kBerInfo);
			}
			break;
		case FT_STRUCT_BITMAPPED:
			{
				BitMappedInfo	kLocBitMappedInfo;
				InitBitMappedInfo(pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocBitMappedInfo);
				AnalyseBitMappedInfo(szField,nLen,&kLocBitMappedInfo);
				DumpBitMappedInfo(&kLocBitMappedInfo);
			}
			break;
		default:
			//trace_message(E_WARNING,"Unknown Field type: %u",kMsgInfo.pkMsgInfoProperties->tabFieldsInfo[i].eFieldType);
			break;
		}
		*/
	}

}
/******************************************************************************/
void InitBitMappedInfoProperties(BitMappedProperties* pkBitMappedProperties)
{
	
	bm_field_t			stFieldInfo;
	int					i;
	int					nSize;

	pkBitMappedProperties->nNbFields = 0;
	pkBitMappedProperties->nNbBitmaps = 0;
	pkBitMappedProperties->nNbConsBitmaps = 0;
	pkBitMappedProperties->eBitmapFmt = FF_BIN;
	memset(pkBitMappedProperties->szPropertiesName,'\0',sizeof(pkBitMappedProperties->szPropertiesName));
	InitFieldInfo(&stFieldInfo.field_info);
	stFieldInfo.nFieldNo=0;
	nSize = sizeof(pkBitMappedProperties->tabFieldsInfo)/sizeof(pkBitMappedProperties->tabFieldsInfo[0]);
	for( i=0; i < nSize; i++ )
	{
		pkBitMappedProperties->tabFieldsInfo[i]=stFieldInfo;
	}
}
/******************************************************************************/
int GetBitMappedInfoFieldPropName(int nFieldNo, const BitMappedInfo* pkBitMappedInfo, char *Name, int* nLength)
{

	if( pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.eFieldType == FT_SINGLE )
	{
		return NOK;
	}

	strncpy(Name, pkBitMappedInfo->pkProperties->tabFieldsInfo[nFieldNo].field_info.szPropertiesName,MAX_PROP_NAME_LEN);
	(*nLength) = strlen(Name);
	return OK;
}

/******************************************************************************/