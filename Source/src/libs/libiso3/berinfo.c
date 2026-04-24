#include <p7_common.h>
#include <iso8583.h>
/******************************************************************************/
#undef NOT_PRESENT	/* need to be removed from define */
#undef PRESENT

enum {
	NOT_PRESENT = 0,
	PRESENT		= 1,
	REPEATED	= 2,
};
/******************************************************************************/
void ResetBerInfo(BerInfo* pkBerInfo)
{
	pkBerInfo->nLength=0;
	pkBerInfo->nNextFreeRFPos=0;
	memset(pkBerInfo->sData,'\0',sizeof(pkBerInfo->sData));
	memset(pkBerInfo->nPresent,0,sizeof(pkBerInfo->nPresent));
	memset(pkBerInfo->nPosTlv,0,sizeof(pkBerInfo->nPosTlv));
	memset(pkBerInfo->nRepeatedFieldPos,0,sizeof(pkBerInfo->nRepeatedFieldPos));
}
/******************************************************************************/
void InitBerInfo(const char* szPropertiesName, BerInfo* pkBerInfo)
{
	pkBerInfo->pkProperties = GetBerInfoProperties(szPropertiesName);
	ResetBerInfo(pkBerInfo);

}
/******************************************************************************/
static int GetTagIndice(const char* szTag, const BerInfo* pkBerInfo)
{

	int		i;
	for( i=0; i < pkBerInfo->pkProperties->nNbFields; i++ )
	{
		if( strcmp(pkBerInfo->pkProperties->tabFieldsInfo[i].szTag,szTag) == 0 )
		{
			return i;
		}
	}
	return -1;
}
/******************************************************************************/
static void AdjustBerInfo(BerInfo* pkBerInfo, int nPos, int nLength)
{
	int i;

	for (i = 0; i < pkBerInfo->pkProperties->nNbFields; i++)
	{
		if( pkBerInfo->nPresent[i] == PRESENT && pkBerInfo->nPosTlv[i] > nPos)
		{
			pkBerInfo->nPosTlv[i]  += nLength;
		}
	}

	for( i = 0; i < MAX_NB_MULTIFIELDS; i++ )
	{
		if( pkBerInfo->nRepeatedFieldPos[i][0]  > nPos)
		{
			pkBerInfo->nRepeatedFieldPos [i][0]  += nLength;
		}
	}

}
/******************************************************************************/
int  AnalyseBerInfo(const char *buffer_in, int nLength, BerInfo* pkBerInfo)
{
	const unsigned char  *sPtr;
	int		offset;
	int		nTlvLength, nBytes;
	int		nTlvIndice;
	char	szTlvName [ ISO3_MAX_TLV_TAG_LEN + 1 ];
	char	szTlvValueLen [ MAX_TLV_LENGTH_LEN + 4 ];
	int		i;
	int		nLenTag;
	int		nLenLen;
	
	offset = 0;

	if( nLength > sizeof(pkBerInfo->sData))
		return NOK;

	pkBerInfo->nLength = nLength;
	memcpy(pkBerInfo->sData,buffer_in,nLength);

	sPtr = buffer_in;

	while (offset < pkBerInfo->nLength)
	{
		memset( szTlvName,     '\0', sizeof(szTlvName));
		memset( szTlvValueLen,  0,  sizeof(szTlvValueLen));
		if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
		{
			HexToAscii( sPtr, szTlvName,   4 );
			nLenTag=2;
		}
		else
		{
			HexToAscii( sPtr, szTlvName,   2 );
			nLenTag=1;
		}

		if (((unsigned char) ((*(sPtr+nLenTag)) & 0x80 )) == (unsigned char) 0x80)
		{
			nLenLen = 2;
			nBytes = ((unsigned char)(*(sPtr+nLenTag)))*256 + ((unsigned char)(*(sPtr+nLenTag + 1)));
		}
		else
		{
			nLenLen = 1;
			nBytes = ((unsigned char)(*(sPtr+nLenTag)));
		}
		
		nTlvIndice   = GetTagIndice(szTlvName,pkBerInfo);

		if ( nTlvIndice >= 0)
		{
			if( nBytes > pkBerInfo->pkProperties->tabFieldsInfo[nTlvIndice].field_info.nLength )
				return(NOK);
			switch(pkBerInfo->nPresent[ nTlvIndice ])
			{
			case NOT_PRESENT:
				pkBerInfo->nPosTlv [ nTlvIndice ] = offset;
				pkBerInfo->nPresent[ nTlvIndice ] = PRESENT;
				break;
			case PRESENT:
				/* if the field is already present, we flag it as "REPEATED". nPosTlv[indice] will contain the first occurence index
				* of this field. this index is on nRepeatedFieldPos[][] array.
				nRepeatedFieldPos[][0] contains the offset of the field
				nRepeatedFieldPos[][1] contains the index of the next occurence or NOT_PRESENT if no there's no other occurence*/
				pkBerInfo->nPresent[ nTlvIndice ] = REPEATED;
				pkBerInfo->nPosTlv [ nTlvIndice ] = pkBerInfo->nNextFreeRFPos;

				pkBerInfo->nRepeatedFieldPos[pkBerInfo->nNextFreeRFPos][0] = pkBerInfo->nPosTlv [ nTlvIndice ];
				pkBerInfo->nRepeatedFieldPos[pkBerInfo->nNextFreeRFPos][1] = ++pkBerInfo->nNextFreeRFPos;
				pkBerInfo->nRepeatedFieldPos[pkBerInfo->nNextFreeRFPos][0] = offset;
				pkBerInfo->nRepeatedFieldPos[pkBerInfo->nNextFreeRFPos][1] = NOT_PRESENT;
				pkBerInfo->nNextFreeRFPos++;
				break;
			case REPEATED:
				/* for repeated fields, we start with index of the first occurence and we search for the last occurence
				* then we add this new occurence*/
				i = pkBerInfo->nPosTlv [ nTlvIndice ];
				while( pkBerInfo->nRepeatedFieldPos[i][1] != NOT_PRESENT)
				{
					i = pkBerInfo->nRepeatedFieldPos[i][1];
				}
				pkBerInfo->nRepeatedFieldPos[i][1] = pkBerInfo->nNextFreeRFPos;
				pkBerInfo->nRepeatedFieldPos[pkBerInfo->nNextFreeRFPos][0] = offset;
				pkBerInfo->nRepeatedFieldPos[pkBerInfo->nNextFreeRFPos][1] = NOT_PRESENT;
				pkBerInfo->nNextFreeRFPos++;
				break;
			}
		}
		else
		{
		
		}
		nTlvLength = nLenTag + nLenLen +  nBytes;
		offset += nTlvLength;
		sPtr   += nTlvLength;
	}

	return OK;
}
/******************************************************************************/
int  GetBerInfoNextField (const char *tlv_name, const BerInfo* pkBerInfo, char *data, int* nLength, int nIter)
{
	int		nIndice;
	char	szAscTlvLen [MAX_TLV_LENGTH_LEN + 1];
	const char	*sPtr;
	int		i,j;
	int		nLenTag;
	int		nLenLen;
	int		nBytes;

	memset (szAscTlvLen, 0, sizeof(szAscTlvLen));

	if ( (nIndice = GetTagIndice(tlv_name,pkBerInfo)) < 0 )
		return(NOK);

 
	if (pkBerInfo->nPresent[nIndice] == NOT_PRESENT)
		return(NOK);

	if( pkBerInfo->nPresent[nIndice] == PRESENT )
	{
		sPtr =  pkBerInfo->sData + pkBerInfo->nPosTlv[nIndice];
	}
	else if( pkBerInfo->nPresent[nIndice] == REPEATED )
	{
		i = pkBerInfo->nPosTlv[nIndice];
		j = 0;
		while( j < nIter && pkBerInfo->nRepeatedFieldPos[i][1] != NOT_PRESENT ) 
		{
			i = pkBerInfo->nRepeatedFieldPos[i][1];
			j++;
		}
		if( j != nIter )
			return NOK;
		sPtr =  pkBerInfo->sData + pkBerInfo->nRepeatedFieldPos[i][0];
	}


	if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
	{
		nLenTag=2;
	}
	else
	{
		nLenTag=1;
	}

	if (((unsigned char) ((*(sPtr+nLenTag)) & 0x80 )) == (unsigned char) 0x80)
	{
		nLenLen = 2;
		nBytes = ((unsigned char)(*(sPtr+nLenTag))&0x7F)*256 + ((unsigned char)(*(sPtr+nLenTag + 1))&0x7F);
	}
	else
	{
		nLenLen = 1;
		nBytes = ((unsigned char)(*(sPtr+nLenTag))&0x7F);
	}

	(*nLength) = nBytes;

	switch( pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eFormatType)
	{
	case FF_ALPHA_A		:
	case FF_ALPHA_N		:
	case FF_ALPHA_AN	:
	case FF_ALPHA_ANS	:
	case FF_ALPHA_NS	:
	case FF_Z			:
	case FF_BIN			:
		if( pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy( data, sPtr + nLenTag +  nLenLen,(*nLength));	
		}
		else if( pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_EBCDIC )
		{
			EbcdicToAscii( sPtr + nLenTag +  nLenLen,  data, (*nLength));
		}
		
		break;
	case FF_HEX:
		HexToAscii( sPtr + nLenTag +  nLenLen, data, (*nLength)*2);
		(*nLength) = (*nLength)*2;
		break;
	case FF_BCD:
		BcdToAscii( sPtr + nLenTag +  nLenLen, data, (*nLength)*2);
		(*nLength) = (*nLength)*2;
		break;
	}
	
	data[(*nLength)] = '\0';

	return(SUCCESS);

}
/******************************************************************************/
int  GetBerInfoField (const char *tlv_name, const BerInfo* pkBerInfo, char *data, int* nLength)
{
	return GetBerInfoNextField(tlv_name,pkBerInfo,data,nLength,0);
}
/******************************************************************************/
int  AddBerInfoField (const char *tlv_name, BerInfo* pkBerInfo, const char *data, int nLength)
{
	int				nIndice;
	char			szAscTlvLen [MAX_TLV_LENGTH_LEN + 1];
	char			szAscTlvLen_add [MAX_TLV_LENGTH_LEN + 1];
	unsigned char	sLen[4];
	char			*sPtr;
	int				nBytes;
	int				i;
	int				nLenTag;
	int				nLenLen;
	int				nCurrRFPos;
	int				nCurrPos;
	int				nFieldMaxLength;
	E_FIELD_LENGTH	eLengthType;
	E_LENGTH_UNIT	eLengthUnit;
	E_FIELD_FORMAT	eFormatType;
	int				nLenToPut;

	/*TODO: dans tous les add, si avec ce champ on va depasser sizeof(data) alors return error*/
	memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));
	memset (szAscTlvLen_add, '\0', sizeof(szAscTlvLen_add));

	if ( nLength == 0 )
		return(ERROR);

	if ( (nIndice = GetTagIndice(tlv_name,pkBerInfo)) < 0 )
		return(ERROR);


	if (pkBerInfo->nPresent[nIndice] == PRESENT && 
		pkBerInfo->pkProperties->tabFieldsInfo[nIndice].eIsRepeatable != IS_REPEATABLE )
		return(ERROR);


	sPtr = pkBerInfo->sData + pkBerInfo->nLength;
	AsciiToHex(tlv_name, sPtr, 4 );

	if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
	{
		nLenTag=2;
	}
	else
	{
		nLenTag=1;
	}


	nFieldMaxLength = pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.nLength;
	eLengthType		= pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eLengthType;
	eLengthUnit		= pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eLengthUnit;
	eFormatType		= pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eFormatType;

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
	
	if ( nLenToPut <= 127)
	{
		nLenLen = 1;
		sLen[0]=(unsigned char)nLenToPut;
	}
	else
	{
		nLenLen = 2;
		sLen[0]=((unsigned char)nLenToPut/256)|0x80;
		sLen[1]=((unsigned char)nLenToPut%256)&0x7F;
	}

	memcpy(pkBerInfo->sData + pkBerInfo->nLength + nLenTag , sLen, nLenLen);
	

	switch(  pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eFormatType)
	{
	case FF_ALPHA_A		:
	case FF_ALPHA_N		:
	case FF_ALPHA_AN	:
	case FF_ALPHA_ANS	:
	case FF_ALPHA_NS	:
	case FF_Z			:
		if( pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy(pkBerInfo->sData + pkBerInfo->nLength + nLenTag + nLenLen, data, nLength);
		}
		else if( pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_EBCDIC )
		{
			AsciiToEbcdic ( data, 
							pkBerInfo->sData + pkBerInfo->nLength + nLenTag + nLenLen,
							nLength);
		}
		nBytes = nLength;
		break;
	case FF_BIN			:
		memcpy(pkBerInfo->sData + pkBerInfo->nLength + nLenTag + nLenLen, data, nLength);
		nBytes = nLength;
		break;
	case FF_HEX:
		nBytes = AsciiToHex( data , pkBerInfo->sData + pkBerInfo->nLength + nLenTag + nLenLen, nLength);
		break;
	case FF_BCD:
		nBytes = AsciiToBcd( data,  pkBerInfo->sData + pkBerInfo->nLength + nLenTag + nLenLen, nLength);
		break;
	}

	

	switch( pkBerInfo->nPresent[ nIndice ])
	{
	case NOT_PRESENT:
		pkBerInfo->nPosTlv [ nIndice ] = pkBerInfo->nLength;
		pkBerInfo->nPresent[ nIndice ] = PRESENT;
		break;
	case PRESENT:
		pkBerInfo->nPresent[ nIndice ] = REPEATED;
		nCurrRFPos	= pkBerInfo->nNextFreeRFPos;
		nCurrPos	= pkBerInfo->nPosTlv [ nIndice ];
		pkBerInfo->nPosTlv [ nIndice ] = nCurrRFPos;
		pkBerInfo->nRepeatedFieldPos[nCurrRFPos][0] = nCurrPos;
		pkBerInfo->nRepeatedFieldPos[nCurrRFPos][1] = ++pkBerInfo->nNextFreeRFPos;
		pkBerInfo->nRepeatedFieldPos[pkBerInfo->nNextFreeRFPos][0] = pkBerInfo->nLength;
		pkBerInfo->nRepeatedFieldPos[pkBerInfo->nNextFreeRFPos][1] = NOT_PRESENT;
		pkBerInfo->nNextFreeRFPos++;
		break;
	case REPEATED:
		i = pkBerInfo->nPosTlv [ nIndice ];
		while( pkBerInfo->nRepeatedFieldPos[i][1] != NOT_PRESENT)
		{
			i = pkBerInfo->nRepeatedFieldPos[i][1];
		}
		pkBerInfo->nRepeatedFieldPos[i][1] = pkBerInfo->nNextFreeRFPos;
		pkBerInfo->nRepeatedFieldPos[pkBerInfo->nNextFreeRFPos][0] = pkBerInfo->nLength;
		pkBerInfo->nRepeatedFieldPos[pkBerInfo->nNextFreeRFPos][1] = NOT_PRESENT;
		pkBerInfo->nNextFreeRFPos++;
		break;
	}

	pkBerInfo->nLength += nBytes + nLenTag + nLenLen;
	return OK;

}
/******************************************************************************/
int  PutBerInfoField (const char *tlv_name, BerInfo* pkBerInfo, const char *data, int nLength)
{
	int				nIndice;
	int				nOldTlvLen;
	int				nBytes;

	char			szAscTlvLen [ MAX_TLV_LENGTH_LEN + 1 ];
	char			szAscTlvLen_add [ MAX_TLV_LENGTH_LEN + 1 ];
	char			szOldTlvLen [ MAX_TLV_LENGTH_LEN + 1];
	char			szLastPart  [ MAX_STRUCT_DATA_LEN ];
	unsigned char	sLen[4];

	int				nLenTag;
	int				nOldLenLen;
	int				nNewLenLen;
	int				nLastPartLen;
	char*			sPtr;
	int				nFieldMaxLength;
	E_FIELD_LENGTH	eLengthType;
	E_LENGTH_UNIT	eLengthUnit;
	E_FIELD_FORMAT	eFormatType;
	int				nLenToPut;


	memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));
	memset (szAscTlvLen_add, '\0', sizeof(szAscTlvLen_add));
	memset (szOldTlvLen, '\0', sizeof(szOldTlvLen));

	if ( nLength == 0 )
		return(ERROR);

	if ( (nIndice =GetTagIndice(tlv_name,pkBerInfo)) < 0 )
	{
		return(ERROR);
	}


	memset (szLastPart,  '\0', sizeof(szLastPart));


	if (pkBerInfo->nPresent[nIndice] != PRESENT  || pkBerInfo->pkProperties->tabFieldsInfo[nIndice].eIsRepeatable == IS_REPEATABLE )
	{

		return AddBerInfoField(tlv_name,pkBerInfo,data,nLength);
	}
		

	sPtr =  pkBerInfo->sData + pkBerInfo->nPosTlv[nIndice];

	if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
	{
		nLenTag=2;
	}
	else
	{
		nLenTag=1;
	}

	if (((unsigned char) ((*(sPtr+nLenTag)) & 0x80 )) == (unsigned char) 0x80)
	{
		nOldLenLen = 2;
		nOldTlvLen = ((unsigned char)(*(sPtr+nLenTag))&0x7F)*256 + ((unsigned char)(*(sPtr+nLenTag + 1))&0x7F);
	}
	else
	{
		nOldLenLen = 1;
		nOldTlvLen = ((unsigned char)(*(sPtr+nLenTag))&0x7F);
	}


	nFieldMaxLength = pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.nLength;
	eLengthType		= pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eLengthType;
	eLengthUnit		= pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eLengthUnit;
	eFormatType		= pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eFormatType;

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

	nLastPartLen = pkBerInfo->nLength - pkBerInfo->nPosTlv[nIndice] - nLenTag - nOldLenLen - nOldTlvLen;

	memcpy( szLastPart,
		sPtr + nLenTag + nOldLenLen + nOldTlvLen,
		nLastPartLen);

		
	if ( nLength <= 127)
	{
		nNewLenLen = 1;
		sLen[0]=(unsigned char)nLenToPut;
	}
	else
	{
		nNewLenLen = 2;
		sLen[0]=((unsigned char)nLenToPut/256)|0x80;
		sLen[1]=((unsigned char)nLenToPut%256)&0x7F;
	}

	memcpy(sPtr + nLenTag , sLen, nNewLenLen);

	switch( pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eFormatType)
	{
	case FF_ALPHA_A		:
	case FF_ALPHA_N		:
	case FF_ALPHA_AN	:
	case FF_ALPHA_ANS	:
	case FF_ALPHA_NS	:
	case FF_Z			:
		if( pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy( sPtr + nLenTag + nNewLenLen,data,nLength);
		}
		else if( pkBerInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_EBCDIC )
		{
			AsciiToEbcdic ( data, 
							sPtr + nLenTag + nNewLenLen,
							nLength);
		}
		nBytes = nLength;
		break;
	case FF_BIN:
		memcpy( sPtr + nLenTag + nNewLenLen,data,nLength);
		nBytes = nLength;
		break;
	case FF_HEX:
		AsciiToHex( data , sPtr + nLenTag +  nNewLenLen, nLength);
		nBytes = nLength/2;
		break;
	case FF_BCD:
		AsciiToBcd( data, sPtr + nLenTag +  nNewLenLen, nLength);
		nBytes = nLength/2;
		break;
	}
		

	memcpy(sPtr + nLenTag + nNewLenLen + nBytes,
		szLastPart,
		nLastPartLen);

	pkBerInfo->nLength += nBytes - nOldTlvLen;
	AdjustBerInfo( pkBerInfo, pkBerInfo->nPosTlv[nIndice],nBytes - nOldTlvLen);
	return(SUCCESS);

}
/******************************************************************************/
int  BuildBerInfoBuffer (char * buffer_out, BerInfo* pkBerInfo)
{
	memcpy ( buffer_out, pkBerInfo->sData , pkBerInfo->nLength);
	buffer_out[pkBerInfo->nLength ] = '\0';
	return pkBerInfo->nLength;
}
/******************************************************************************/
void DumpBerInfo(const BerInfo* pkBerInfo)
{
	char	sLine[MAX_LINE_TRC];
	int		nOffset = 0;
	int		i;
	int		nLen;
	int		nIter;
	/*int		nRepeatableFieldEnd;*/
	char	szField[MAX_FIELD_LEN];
	char	szFieldAdd[MAX_FIELD_LEN];
	
	

	if( is_trace_level_on(P7_TL_TRACE) != OK )
		return;

	memset(sLine,'\0',sizeof(sLine));
	memset(szField,'\0',sizeof(szField));
	/*nRepeatableFieldEnd = OK;*/
	nIter = 0;
	for( i=0; i < pkBerInfo->pkProperties->nNbFields;)
	{
		/* If is a repeatable field, flag it so that we keep iterating on it until there's no other value */
		if( pkBerInfo->pkProperties->tabFieldsInfo[i].eIsRepeatable == IS_REPEATABLE )
		{
			if( GetBerInfoNextField(pkBerInfo->pkProperties->tabFieldsInfo[i].szTag,pkBerInfo,szField,&nLen,nIter) != SUCCESS )
			{
				i++;
				nIter = 0;
				continue;
			}
			nIter++;

		}
		else
		{
			if( GetBerInfoField(pkBerInfo->pkProperties->tabFieldsInfo[i].szTag,pkBerInfo,szField,&nLen) != SUCCESS )
			{
				i++;
				continue;
			}
		}

		if( pkBerInfo->pkProperties->tabFieldsInfo[i].field_info.eFormatType != FF_BIN )
		{
			trace_message(P7_TL_TRACE,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLen ,szField );
		}
		else
		{
			nLen = HexToAscii(szField, szFieldAdd, nLen*2);
			szFieldAdd[nLen]='\0';/*PLUTONL-5279 AHA20221021*/
			trace_message(P7_TL_TRACE,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLen ,szFieldAdd );
		}
		/*
		switch(pkBerInfo->pkProperties->tabFieldsInfo[i].field_info.eFieldType)
		{

		case FT_SINGLE:
			
			if( pkBerInfo->pkProperties->tabFieldsInfo[i].field_info.eFormatType != FF_BIN )
			{
				trace_message(P7_TL_TRACE,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLen ,szField );
			}
			else
			{

			}
			break;
		case FT_STRUCT_TLV:
			{
				TlvInfo	kLocTlvInfo;
				InitTlvInfo(pkBerInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocTlvInfo);
				AnalyseTlvInfo(szField,nLen,&kLocTlvInfo);
				DumpTlvInfo(&kLocTlvInfo);
			}
			break;
		case FT_STRUCT_STATIC:
			{
				StaticInfo	kLocStaticInfo;
				InitStaticInfo(pkBerInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocStaticInfo);
				AnalyseStaticInfo(szField,nLen,&kLocStaticInfo);
				DumpStaticInfo(&kLocStaticInfo);
			}
			break;
		case FT_STRUCT_BER:
			{
				BerInfo	kLocBerInfo;
				InitBerInfo(pkBerInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocBerInfo);
				AnalyseBerInfo(szField,nLen,&kLocBerInfo);
				DumpBerInfo(&kLocBerInfo);
			}
			break;
		case FT_STRUCT_BITMAPPED:
			{
				BitMappedInfo	kLocBitMappedInfo;
				InitBitMappedInfo(pkBerInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocBitMappedInfo);
				AnalyseBitMappedInfo(szField,nLen,&kLocBitMappedInfo);
				DumpBitMappedInfo(&kLocBitMappedInfo);
			}
			break;
		default:
			trace_message(E_WARNING,"Unknown Field type: %u",kMsgInfo.pkMsgInfoProperties->tabFieldsInfo[i].eFieldType);
			break;
		}*/

		if( pkBerInfo->pkProperties->tabFieldsInfo[i].eIsRepeatable != IS_REPEATABLE )
		{
			i++;
		}

	}
}
/******************************************************************************/
void InitBerInfoProperties(BerInfoProperties* pkBerInfoProperties)
{
	
	ber_field_t		stFieldInfo;
	int					i;
	int					nSize;

	pkBerInfoProperties->nNbFields = 0 ;
	memset(pkBerInfoProperties->szPropertiesName,'\0',sizeof(pkBerInfoProperties->szPropertiesName));
	InitFieldInfo(&stFieldInfo.field_info);
	stFieldInfo.eIsRepeatable = NOT_REPEATABLE;
	memset(stFieldInfo.szTag,'\0',sizeof(stFieldInfo.szTag));

	nSize = sizeof(pkBerInfoProperties->tabFieldsInfo)/sizeof(pkBerInfoProperties->tabFieldsInfo[0]);
	for( i=0; i < nSize; i++ )
	{
		pkBerInfoProperties->tabFieldsInfo[i]=stFieldInfo;
	}


}
/******************************************************************************/
