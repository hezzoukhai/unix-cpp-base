#include <p7_common.h>
#include <p7_globals.h>
#include <p7_types.h>
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
void InitTlvInfo(const char* szPropertiesName, TlvInfo* pkTlvInfo)
{
	pkTlvInfo->pkProperties = GetTlvInfoProperties(szPropertiesName);
	ResetTlvInfo(pkTlvInfo);
}
/******************************************************************************/
void ResetTlvInfo(TlvInfo* kTlvInfo)
{
	kTlvInfo->nLength=0;
	kTlvInfo->nNextFreeRFPos=0;
	memset(kTlvInfo->sData,'\0',sizeof(kTlvInfo->sData));
	memset(kTlvInfo->nPresent,0,sizeof(kTlvInfo->nPresent));
	memset(kTlvInfo->nPosTlv,0,sizeof(kTlvInfo->nPosTlv));
	memset(kTlvInfo->nRepeatedFieldPos,0,sizeof(kTlvInfo->nRepeatedFieldPos));
}
/******************************************************************************/
static int GetTagIndice(const char* szTag, const TlvInfo* pkTlvInfo)
{

	int		i;
	for( i=0; i < pkTlvInfo->pkProperties->nNbFields; i++ )
	{
		if( strcmp(pkTlvInfo->pkProperties->tabFieldsInfo[i].szTag,szTag) == 0 )
		{
			return i;
		}
	}
	return -1;

}
/******************************************************************************/
static int ValidateTlvInfo(const TlvInfo* pkTlvInfo)
{
	int		nIndice, nSize;
	char	data[MAX_FIELD_LEN + 1];
	int		nResult;
	int		nLen;
	int		nIter;
	

	nSize = pkTlvInfo->pkProperties->nNbFields;
	nIndice = 0;
	do
	{
		if (pkTlvInfo->nPresent[nIndice] == NOT_PRESENT)
		{
			nIndice++;
			continue;
		}

		memset(data, '\0', sizeof(data));

		if (pkTlvInfo->nPresent[nIndice] == PRESENT || pkTlvInfo->nPresent[nIndice] == REPEATED)
		{
			nResult = GetTlvInfoField(pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].szTag,pkTlvInfo, data , &nLen);
			if (nResult != SUCCESS)
			{
				/*TODO: error not found*/
				return(NOK);
			}
			if(nLen != strlen(data))
			{
				/*TODO: error
				sprintf(sLine,"End   Validate|Length Error(NOK|%s|%d|%d|%d)",pkSubFieldInfo->u.sTag,nLen,strlen(data),nIndice);
				trace_event(sLine,PROCESSING);
				dump_buffer(data, length,'A','X','X');
				dump_buffer(tlvInfo.sTlvData, tlvInfo.nLength,'A','X','X');*/
				return(NOK);
			}
			if(  pkTlvInfo->nPresent[nIndice] == REPEATED )
			{
				nIter = 0;
				do 
				{
					nResult = GetTlvInfoNextField(pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].szTag,pkTlvInfo, data , &nLen,nIter);
					if( nResult == SUCCESS &&
						nLen != strlen(data))
					{
						/*TODO: error
						sprintf(sLine,"End   Validate|Length Error(NOK|%s|%d|%d|%d)",pkSubFieldInfo->u.sTag,nLen,strlen(data),nIndice);
						trace_event(sLine,PROCESSING);
						dump_buffer(data, length,'A','X','X');
						dump_buffer(tlvInfo.sTlvData, tlvInfo.nLength,'A','X','X');*/
						return(NOK);
					}
					nIter++;
				} while(nResult == SUCCESS);
			}
		}
	}while (nIndice < nSize);
	
	return(OK);
}
/******************************************************************************/
static void AdjustTlvInfo(TlvInfo* pkTlvInfo, int nPos, int nLength)
{
	int i;

	for (i = 0; i < pkTlvInfo->pkProperties->nNbFields; i++)
	{
		if( pkTlvInfo->nPresent[i] == PRESENT && pkTlvInfo->nPosTlv[i] > nPos)
		{
			pkTlvInfo->nPosTlv[i]  += nLength;
		}
	}

	for( i = 0; i < MAX_NB_MULTIFIELDS; i++ )
	{
		if( pkTlvInfo->nRepeatedFieldPos[i][0]  > nPos)
		{
			pkTlvInfo->nRepeatedFieldPos [i][0]  += nLength;
		}
	}

}
/******************************************************************************/
int  AnalyseTlvInfo(const char *buffer_in, int nLength, TlvInfo* pkTlvInfo)
{

	const unsigned char  *sPtr;
	int		offset;
	int		nTlvLength;
	int		nTlvIndice;
	char	szTlvName [ ISO3_MAX_TLV_TAG_LEN + 1 ];
	char	szTlvValueLen [ MAX_TLV_LENGTH_LEN + 4 ];
	int		nTlvValueLen ;
	int		mul,value,i;
	
	offset = 0;

	if( nLength > sizeof(pkTlvInfo->sData))
		return NOK;

	pkTlvInfo->nLength = nLength;
	memcpy(pkTlvInfo->sData,buffer_in,nLength);

	sPtr = buffer_in;

	while (offset < pkTlvInfo->nLength)
	{
		memset( szTlvName,     '\0', sizeof(szTlvName));
		memset( szTlvValueLen,  0,  sizeof(szTlvValueLen));
		if(  pkTlvInfo->pkProperties->eTagType == TLV_LT_BIN )
		{
			HexToAscii( (char*)sPtr, szTlvName,   pkTlvInfo->pkProperties->nTagLen*2 );
		}
		else
		{
			memcpy( szTlvName,     sPtr,    pkTlvInfo->pkProperties->nTagLen );
		}

		
		if ( pkTlvInfo->pkProperties->eLengthType == TLV_LT_BIN )
		{
			mul=1;
			nTlvValueLen=0;
			for( i=pkTlvInfo->pkProperties->nLengthLen - 1; i >= 0; i-- )
			{
				value = (unsigned char)(*(sPtr + pkTlvInfo->pkProperties->nTagLen + i) );
				value = value*mul;
				mul= mul*256;
				nTlvValueLen += value;
			}
		}
		else
		{
			memcpy( szTlvValueLen, sPtr + pkTlvInfo->pkProperties->nTagLen,     pkTlvInfo->pkProperties->nLengthLen);
			nTlvValueLen = atoi(szTlvValueLen);
		}
		
		nTlvIndice   = GetTagIndice( szTlvName,pkTlvInfo);

		if ( nTlvIndice >= 0)
		{
			if( nTlvValueLen > pkTlvInfo->pkProperties->tabFieldsInfo[nTlvIndice].field_info.nLength )
				return(NOK);
			switch(pkTlvInfo->nPresent[ nTlvIndice ])
			{
			case NOT_PRESENT:
				pkTlvInfo->nPosTlv [ nTlvIndice ] = offset;
				pkTlvInfo->nPresent[ nTlvIndice ] = PRESENT;
				break;
			case PRESENT:
				/* if the field is already present, we flag it as "REPEATED". nPosTlv[indice] will contain the first occurence index
				* of this field. this index is on nRepeatedFieldPos[][] array.
				nRepeatedFieldPos[][0] contains the offset of the field
				nRepeatedFieldPos[][1] contains the index of the next occurence or NOT_PRESENT if no there's no other occurence*/
				pkTlvInfo->nPresent[ nTlvIndice ] = REPEATED;
				pkTlvInfo->nPosTlv [ nTlvIndice ] = pkTlvInfo->nNextFreeRFPos;

				pkTlvInfo->nRepeatedFieldPos[pkTlvInfo->nNextFreeRFPos][0] = pkTlvInfo->nPosTlv [ nTlvIndice ];
				pkTlvInfo->nRepeatedFieldPos[pkTlvInfo->nNextFreeRFPos][1] = ++pkTlvInfo->nNextFreeRFPos;
				pkTlvInfo->nRepeatedFieldPos[pkTlvInfo->nNextFreeRFPos][0] = offset;
				pkTlvInfo->nRepeatedFieldPos[pkTlvInfo->nNextFreeRFPos][1] = NOT_PRESENT;
				pkTlvInfo->nNextFreeRFPos++;
				break;
			case REPEATED:
				/* for repeated fields, we start with index of the first occurence and we search for the last occurence
				* then we add this new occurence*/
				i = pkTlvInfo->nPosTlv [ nTlvIndice ];
				while( pkTlvInfo->nRepeatedFieldPos[i][1] != NOT_PRESENT)
				{
					i = pkTlvInfo->nRepeatedFieldPos[i][1];
				}
				pkTlvInfo->nRepeatedFieldPos[i][1] = pkTlvInfo->nNextFreeRFPos;
				pkTlvInfo->nRepeatedFieldPos[pkTlvInfo->nNextFreeRFPos][0] = offset;
				pkTlvInfo->nRepeatedFieldPos[pkTlvInfo->nNextFreeRFPos][1] = NOT_PRESENT;
				pkTlvInfo->nNextFreeRFPos++;
				break;
			}
		}
		else
		{
			
		}

		nTlvLength = pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen +  nTlvValueLen;
		offset += nTlvLength;
		sPtr   += nTlvLength;
	}

	return OK;
}
/******************************************************************************/
int  GetTlvInfoNextField (const char *tlv_name, const TlvInfo* pkTlvInfo, char *data, int* nLength, int nIter)
{
	int		nIndice;
	char	szAscTlvLen [MAX_TLV_LENGTH_LEN + 1];
	const char	*sPtr;
	int		i,j;
	int		mul,value;

	memset (szAscTlvLen, 0, sizeof(szAscTlvLen));

	if ( (nIndice = GetTagIndice(tlv_name,pkTlvInfo)) < 0 )
		return(NOK);

 
	if (pkTlvInfo->nPresent[nIndice] == NOT_PRESENT)
		return(NOK);


	if( pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].eIsRepeatable == NOT_REPEATABLE )
	{
		if( pkTlvInfo->nPresent[nIndice] == PRESENT )
		{
			sPtr =  pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen;
		}
	}
	else
	{
		if( pkTlvInfo->nPresent[nIndice] == PRESENT )
		{
			if( nIter ==  0 )
			{
				sPtr =  pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen;
			}
			else
			{
				return(NOK);
			}
		}
		else if( pkTlvInfo->nPresent[nIndice] == REPEATED )
		{
			i = pkTlvInfo->nPosTlv[nIndice];
			j = 0;
			while( j < nIter && pkTlvInfo->nRepeatedFieldPos[i][1] != NOT_PRESENT ) 
			{
				i = pkTlvInfo->nRepeatedFieldPos[i][1];
				j++;
			}
			if( j != nIter )
				return NOK;
			sPtr =  pkTlvInfo->sData + pkTlvInfo->nRepeatedFieldPos[i][0] + pkTlvInfo->pkProperties->nTagLen;
		}

	}
	

	if(  pkTlvInfo->pkProperties->eTagType == TLV_LT_BIN )
	{
		(*nLength) = 0;
		mul=1;
		for( i=pkTlvInfo->pkProperties->nLengthLen - 1; i >= 0; i-- )
		{
			value = (unsigned char)(*(sPtr+ i) );
			value = value*mul;
			mul= mul*256;
			(*nLength) += value;
		}
	}
	else
	{
		memcpy ( szAscTlvLen, sPtr, pkTlvInfo->pkProperties->nLengthLen);
		(*nLength) = atoi(szAscTlvLen);
	}

	switch( pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eFormatType)
	{
	case FF_ALPHA_A		:
	case FF_ALPHA_N		:
	case FF_ALPHA_AN	:
	case FF_ALPHA_ANS	:
	case FF_ALPHA_NS	:
	case FF_Z			:
		if( pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy( data, sPtr + pkTlvInfo->pkProperties->nLengthLen,(*nLength));
			
		}
		else if( pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_EBCDIC )
		{
			EbcdicToAscii( sPtr + pkTlvInfo->pkProperties->nLengthLen,  data, (*nLength));
		}
		break;
	case FF_BIN:
		memcpy( data, sPtr + pkTlvInfo->pkProperties->nLengthLen,(*nLength));
		break;
	case FF_HEX:
		HexToAscii( sPtr + pkTlvInfo->pkProperties->nLengthLen , data, (*nLength)*2);
		(*nLength) = (*nLength)*2;
		break;
	case FF_BCD:
		BcdToAscii( sPtr + pkTlvInfo->pkProperties->nLengthLen, data, (*nLength)*2);
		(*nLength) = (*nLength)*2;
		break;
	}
	
	data[(*nLength)] = '\0';

	return(SUCCESS);
}
/******************************************************************************/
int  GetTlvInfoField (const char *tlv_name, const TlvInfo* kTlvInfo, char *data, int* nLength)
{
	return GetTlvInfoNextField(tlv_name,kTlvInfo,data,nLength,0);
}
/******************************************************************************/
int  AddTlvInfoField (const char *tlv_name, TlvInfo* pkTlvInfo, const char *data, int nLength)
{
	int		nIndice;
	char	szAscTlvLen [MAX_TLV_LENGTH_LEN + 1];
	char	szAscTlvLen_add [MAX_TLV_LENGTH_LEN + 1];
	int		nBytes;
	int		i;
	int		nCurrRFPos;
	int				nFieldMaxLength;
	E_FIELD_LENGTH	eLengthType;
	E_LENGTH_UNIT	eLengthUnit;
	E_FIELD_FORMAT	eFormatType;
	int				nLenToPut;
	int		nCurrPos;


	memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));
	memset (szAscTlvLen_add, '\0', sizeof(szAscTlvLen_add));

	if ( nLength == 0 )
		return(ERROR);

	if ( (nIndice = GetTagIndice(tlv_name,pkTlvInfo)) < 0)
		return(ERROR);


	if (pkTlvInfo->nPresent[nIndice] == PRESENT && 
		pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].eIsRepeatable != IS_REPEATABLE )
		return(ERROR);


	/***
	if( nLength > pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.nLength )
		return(ERROR);
	***/

	nFieldMaxLength = pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.nLength;
	eLengthType		= pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eLengthType;
	eLengthUnit		= pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eLengthUnit;
	eFormatType		= pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eFormatType;

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
	
	if( pkTlvInfo->pkProperties->eTagType == TLV_LT_ASCII )
	{
		memcpy (pkTlvInfo->sData + pkTlvInfo->nLength    , tlv_name,    pkTlvInfo->pkProperties->nTagLen);
	}
	else
	{
		AsciiToHex(tlv_name, pkTlvInfo->sData + pkTlvInfo->nLength, pkTlvInfo->pkProperties->nTagLen*2 );
	}

	switch(  pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eFormatType)
	{
	case FF_ALPHA_A		:
	case FF_ALPHA_N		:
	case FF_ALPHA_AN	:
	case FF_ALPHA_ANS	:
	case FF_ALPHA_NS	:
	case FF_Z			:
		if( pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy(pkTlvInfo->sData + pkTlvInfo->nLength + pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen, data, nLength);
		}
		else if( pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_EBCDIC )
		{
			AsciiToEbcdic ( data, 
							pkTlvInfo->sData + pkTlvInfo->nLength + pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen,
							nLength);
		}
		nBytes = nLength;
		break;
	case FF_BIN:
		memcpy(pkTlvInfo->sData + pkTlvInfo->nLength + pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen, data, nLength);
		nBytes = nLength;
		break;
	case FF_HEX:
		AsciiToHex( data , pkTlvInfo->sData + pkTlvInfo->nLength + pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen, nLength);
		nBytes = nLenToPut;
		break;
	case FF_BCD:
		AsciiToBcd( data,  pkTlvInfo->sData + pkTlvInfo->nLength + pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen, nLength);
		nBytes = nLenToPut;
		break;
	}


	switch( pkTlvInfo->nPresent[ nIndice ])
	{
	case NOT_PRESENT:
		pkTlvInfo->nPosTlv [ nIndice ] = pkTlvInfo->nLength;
		pkTlvInfo->nPresent[ nIndice ] = PRESENT;
		break;
	case PRESENT:
		pkTlvInfo->nPresent[ nIndice ] = REPEATED;

		nCurrRFPos	= pkTlvInfo->nNextFreeRFPos;
		nCurrPos	= pkTlvInfo->nPosTlv [ nIndice ];
		pkTlvInfo->nPosTlv [ nIndice ] = nCurrRFPos;
		pkTlvInfo->nRepeatedFieldPos[nCurrRFPos][0] = nCurrPos;
		pkTlvInfo->nRepeatedFieldPos[nCurrRFPos][1] = ++pkTlvInfo->nNextFreeRFPos;
		pkTlvInfo->nRepeatedFieldPos[pkTlvInfo->nNextFreeRFPos][0] = pkTlvInfo->nLength;
		pkTlvInfo->nRepeatedFieldPos[pkTlvInfo->nNextFreeRFPos][1] = NOT_PRESENT;
		pkTlvInfo->nNextFreeRFPos++;
		break;
	case REPEATED:
		i = pkTlvInfo->nPosTlv [ nIndice ];
		while( pkTlvInfo->nRepeatedFieldPos[i][1] != NOT_PRESENT)
		{
			i = pkTlvInfo->nRepeatedFieldPos[i][1];
		}
		pkTlvInfo->nRepeatedFieldPos[i][1] = pkTlvInfo->nNextFreeRFPos;
		pkTlvInfo->nRepeatedFieldPos[pkTlvInfo->nNextFreeRFPos][0] = pkTlvInfo->nLength;
		pkTlvInfo->nRepeatedFieldPos[pkTlvInfo->nNextFreeRFPos][1] = NOT_PRESENT;
		pkTlvInfo->nNextFreeRFPos++;
		break;
	}

	
	if( pkTlvInfo->pkProperties->eLengthType == TLV_LT_ASCII )
	{
		sprintf ( szAscTlvLen, "%.*d", pkTlvInfo->pkProperties->nLengthLen,nBytes);
		memcpy (pkTlvInfo->sData  + pkTlvInfo->nLength + pkTlvInfo->pkProperties->nTagLen, szAscTlvLen, pkTlvInfo->pkProperties->nLengthLen);
	}
	else
	{  
		/*sprintf ( szAscTlvLen, "%.*d", pkTlvInfo->pkProperties->nLengthLen*2,nBytes);*/

		sprintf ( szAscTlvLen, "%.*X", pkTlvInfo->pkProperties->nLengthLen*2,nBytes);
		AsciiToHex(szAscTlvLen, szAscTlvLen_add, pkTlvInfo->pkProperties->nLengthLen*2  );
		
		memcpy(pkTlvInfo->sData  + pkTlvInfo->nLength + pkTlvInfo->pkProperties->nTagLen ,szAscTlvLen_add,pkTlvInfo->pkProperties->nLengthLen);
	}

	 pkTlvInfo->nLength += nBytes + pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen;

	return(SUCCESS);
}
/******************************************************************************/
int  PutTlvInfoField (const char *tlv_name, TlvInfo* pkTlvInfo, const char *data, int nLength)
{
	int nIndice;
	int nOldTlvLen;
	int	nBytes;
	int nLastPartLen;
	char szAscTlvLen [ MAX_TLV_LENGTH_LEN + 1 ];
	char szAscTlvLen_add [ MAX_TLV_LENGTH_LEN + 1 ];
	char szOldTlvLen [ MAX_TLV_LENGTH_LEN + 1];
	char szLastPart  [ MAX_STRUCT_DATA_LEN ];
	
	int mul,value,i;


	memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));
	memset (szAscTlvLen_add, '\0', sizeof(szAscTlvLen_add));
	memset (szOldTlvLen, '\0', sizeof(szOldTlvLen));

	if ( nLength == 0 )
		return(ERROR);

	if ( (nIndice =GetTagIndice(tlv_name,pkTlvInfo)) < 0)
	{
		return(ERROR);
	}


	if( nLength > pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.nLength )
		return(ERROR);

	memset (szLastPart,  '\0', sizeof(szLastPart));


	if (pkTlvInfo->nPresent[nIndice] == PRESENT  && pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].eIsRepeatable != IS_REPEATABLE )
	{
		
		if( pkTlvInfo->pkProperties->eLengthType == TLV_LT_ASCII )
		{
			memcpy( szOldTlvLen,
				pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen,
				pkTlvInfo->pkProperties->nLengthLen);
			nOldTlvLen = atoi(szOldTlvLen);
		}
		else
		{
			mul=1;
			nOldTlvLen = 0;
			for( i=pkTlvInfo->pkProperties->nLengthLen - 1; i >= 0; i-- )
			{
				value = (unsigned char)(*(pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen+ i) );
				value = value*mul;
				mul= mul*256;
				nOldTlvLen += value;
			}
		}

		nLastPartLen = pkTlvInfo->nLength - pkTlvInfo->nPosTlv[nIndice] - pkTlvInfo->pkProperties->nTagLen - pkTlvInfo->pkProperties->nLengthLen - nOldTlvLen;
		memcpy( szLastPart,
			pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen + nOldTlvLen,
			nLastPartLen);

		switch( pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eFormatType)
		{
		case FF_ALPHA_A		:
		case FF_ALPHA_N		:
		case FF_ALPHA_AN	:
		case FF_ALPHA_ANS	:
		case FF_ALPHA_NS	:
		case FF_Z			:
			if( pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_ASCII )
			{
				memcpy( pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen,
						data,
						nLength);
			}
			else if( pkTlvInfo->pkProperties->tabFieldsInfo[nIndice].field_info.eAlphaFormat == AF_EBCDIC )
			{
				AsciiToEbcdic ( data, 
								pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen,
								nLength);
			}
			nBytes = nLength;
			break;
		case FF_BIN			:
			memcpy( pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen,
				data,
				nLength);
			nBytes = nLength;
			break; /* PLUTONL-5283 FZL20221013*/
		case FF_HEX:
			AsciiToHex( data , (char*)pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen +  pkTlvInfo->pkProperties->nLengthLen, nLength);
			nBytes = nLength/2;
			break;
		case FF_BCD:
			AsciiToBcd( data, (char*)pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen +  pkTlvInfo->pkProperties->nLengthLen, nLength);
			nBytes = nLength/2;
			break;
		}
		
		
		if( pkTlvInfo->pkProperties->eLengthType == TLV_LT_ASCII )
		{
			sprintf ( szAscTlvLen, "%.*d", pkTlvInfo->pkProperties->nLengthLen, nBytes);	
			memcpy (pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen,
				szAscTlvLen,
				pkTlvInfo->pkProperties->nLengthLen);
		}
		else
		{
			sprintf ( szAscTlvLen, "%.*d", pkTlvInfo->pkProperties->nLengthLen*2, nBytes);
			AsciiToHex(szAscTlvLen, szAscTlvLen_add, pkTlvInfo->pkProperties->nLengthLen*2  );
			memcpy(pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] + pkTlvInfo->pkProperties->nTagLen,szAscTlvLen_add,pkTlvInfo->pkProperties->nLengthLen);
		}

		memcpy(pkTlvInfo->sData + pkTlvInfo->nPosTlv[nIndice] +  pkTlvInfo->pkProperties->nTagLen + pkTlvInfo->pkProperties->nLengthLen + nBytes,
			szLastPart,
			nLastPartLen);

		pkTlvInfo->nLength += nBytes - nOldTlvLen;
		AdjustTlvInfo( pkTlvInfo, pkTlvInfo->nPosTlv[nIndice], nBytes - nOldTlvLen);
		return(SUCCESS);
	}
	
	return AddTlvInfoField(tlv_name,pkTlvInfo,data,nLength);
}
/******************************************************************************/
int  BuildTlvInfoBuffer (char * buffer_out, TlvInfo* pkTlvInfo)
{
	memcpy ( buffer_out, pkTlvInfo->sData , pkTlvInfo->nLength);
	buffer_out[pkTlvInfo->nLength ] = '\0';
	return pkTlvInfo->nLength;
}
/******************************************************************************/
void DumpTlvInfo(const TlvInfo* pkTlvInfo)
{
	char	sLine[MAX_LINE_TRC];
	int		nOffset = 0;
	int		i;
	int		nLen;
	int		nIter;
	/*int		nRepeatableFieldEnd;*/
	char	szField[MAX_FIELD_LEN];
	char	szFieldAdd[MAX_FIELD_LEN];
	

	if( is_trace_level_on(P7_TL_TRACE) != OK)
		return;

	memset(sLine,'\0',sizeof(sLine));
	memset(szField,'\0',sizeof(szField));
	/*nRepeatableFieldEnd = OK;*/
	nIter = 0;
	for( i=0; i < pkTlvInfo->pkProperties->nNbFields;)
	{
		/* If is a repeatable field, flag it so that we keep iterating on it until there's no other value */
		if( pkTlvInfo->pkProperties->tabFieldsInfo[i].eIsRepeatable == IS_REPEATABLE )
		{
			/*if( nRepeatableFieldEnd == NOK )*/
			{
				if( GetTlvInfoNextField(pkTlvInfo->pkProperties->tabFieldsInfo[i].szTag,pkTlvInfo,szField,&nLen,nIter) != SUCCESS )
				{
					i++;
					nIter = 0;
					/*nRepeatableFieldEnd = OK;*/
					continue;
				}
				nIter++;
			}
			/*
			nIter = 1;
			nRepeatableFieldEnd = NOK;*/
		}
		else
		{
			if( GetTlvInfoField(pkTlvInfo->pkProperties->tabFieldsInfo[i].szTag,pkTlvInfo,szField,&nLen) != SUCCESS )
			{
				i++;
				continue;
			}
		}

		
		
		
		if( pkTlvInfo->pkProperties->tabFieldsInfo[i].field_info.eFormatType != FF_BIN )
		{
			trace_message(P7_TL_TRACE,"			- TAG (%s) : (%03d) : [%s] " , pkTlvInfo->pkProperties->tabFieldsInfo[i].szTag , nLen ,szField );
		}
		else
		{
			nLen = HexToAscii(szField, szFieldAdd, nLen*2);
			szFieldAdd[nLen]='\0';/*PLUTONL-5279 AHA20221021*/
			trace_message(P7_TL_TRACE,"			- TAG (%s) : (%03d) : [%s] " , pkTlvInfo->pkProperties->tabFieldsInfo[i].szTag , nLen ,szFieldAdd );
		}
		/*
		switch(pkTlvInfo->pkProperties->tabFieldsInfo[i].field_info.eFieldType)
		{

		case FT_SINGLE:
			if( pkTlvInfo->pkProperties->tabFieldsInfo[i].field_info.eFormatType != FF_BIN )
			{
				trace_message(P7_TL_TRACE,"- TAG (%s) : (%03d) : [%s] " , pkTlvInfo->pkProperties->tabFieldsInfo[i].szTag , nLen ,szField );
			}
			else
			{

			}

			break;
		case FT_STRUCT_TLV:
			{
				
				TlvInfo	kLocTlvInfo;
				trace_message(P7_TL_TRACE,"- TAG (%s) : (%03d): Dump " , pkTlvInfo->pkProperties->tabFieldsInfo[i].szTag , nLen ,szField );
				trace_event("=================",P7_TL_TRACE);
				InitTlvInfo(pkTlvInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocTlvInfo);
				AnalyseTlvInfo(szField,nLen,&kLocTlvInfo);
				DumpTlvInfo(&kLocTlvInfo);
				trace_event("=================",P7_TL_TRACE);
			}
			break;
		case FT_STRUCT_STATIC:
			{
				StaticInfo	kLocStaticInfo;
				InitStaticInfo(pkTlvInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocStaticInfo);
				AnalyseStaticInfo(szField,nLen,&kLocStaticInfo);
				DumpStaticInfo(&kLocStaticInfo);
			}
			break;
		case FT_STRUCT_BER:
			{
				BerInfo	kLocBerInfo;
				InitBerInfo(pkTlvInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocBerInfo);
				AnalyseBerInfo(szField,nLen,&kLocBerInfo);
				DumpBerInfo(&kLocBerInfo);
			}
			break;
		case FT_STRUCT_BITMAPPED:
			{
				BitMappedInfo	kLocBitMappedInfo;
				InitBitMappedInfo(pkTlvInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocBitMappedInfo);
				AnalyseBitMappedInfo(szField,nLen,&kLocBitMappedInfo);
				DumpBitMappedInfo(&kLocBitMappedInfo);
			}
			break;
		default:
			trace_message(E_WARNING,"Unknown Field type: %u",kMsgInfo.pkMsgInfoProperties->tabFieldsInfo[i].eFieldType);
			break;
		}*/

		if( pkTlvInfo->pkProperties->tabFieldsInfo[i].eIsRepeatable != IS_REPEATABLE )
		{
			i++;
		}

	}
}
/******************************************************************************/
void InitTlvInfoProperties(TlvInfoProperties* pkTlvInfoProperties)
{
	
	tlv_field_t			stFieldInfo;
	int					i;
	int					nSize;

	pkTlvInfoProperties->nTagLen = 0;
	pkTlvInfoProperties->nLengthLen = 0;
	pkTlvInfoProperties->eLengthType = TLV_LT_ASCII;
	pkTlvInfoProperties->eTagType = TLV_LT_ASCII;
	pkTlvInfoProperties->nNbFields = 0 ;
	memset(pkTlvInfoProperties->szPropertiesName,'\0',sizeof(pkTlvInfoProperties->szPropertiesName));

	memset(stFieldInfo.szTag,'\0',sizeof(stFieldInfo.szTag));
	InitFieldInfo(&stFieldInfo.field_info);
	stFieldInfo.eIsRepeatable = NOT_REPEATABLE;
	

	nSize = sizeof(pkTlvInfoProperties->tabFieldsInfo)/sizeof(pkTlvInfoProperties->tabFieldsInfo[0]);
	for( i=0; i < nSize; i++ )
		pkTlvInfoProperties->tabFieldsInfo[i]=stFieldInfo;

}
/******************************************************************************/

