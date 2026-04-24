#include <p7_common.h>
#include <iso8583.h>
#include <hsm_c2p_define.h>
#include <hsm_c2p.h>

/******************************************************************************/
#undef NOT_PRESENT	/* need to be removed from define ***/
#undef PRESENT

enum {
	NOT_PRESENT = 0,
	PRESENT		= 1,
	REPEATED	= 2,
};
/******************************************************************************/
void ResetC2PCmdInfo(TSC2PCmd* pkC2PCmd)
{
	pkC2PCmd->nLength=0;
	pkC2PCmd->nNextFreeRFPos=0;
	memset(pkC2PCmd->sData,'\0',sizeof(pkC2PCmd->sData));
	memset(pkC2PCmd->sCmdTag,'\0',sizeof(pkC2PCmd->sCmdTag));
	memset(pkC2PCmd->nPresent,0,sizeof(pkC2PCmd->nPresent));
	memset(pkC2PCmd->nPosTlv,0,sizeof(pkC2PCmd->nPosTlv));
	memset(pkC2PCmd->nRepeatedFieldPos,0,sizeof(pkC2PCmd->nRepeatedFieldPos));
	pkC2PCmd->nCmdTagLen=0;
	
}
/******************************************************************************/
static int GetTagIndice(const char* szTag, const TSC2PCmd* pkC2PCmd)
{

	int		i;
	int		nFieldId;
	
	for( i=0; i < pkC2PCmd->pkLayout->nNbFields; i++ )
	{
		nFieldId = pkC2PCmd->pkLayout->tab_fields[i].nFieldId;
		if( strcmp(pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].szTag,szTag) == 0 )
		{
			return i;
		}
	}
	return -1;
}
/******************************************************************************/
static int GetFieldIdIndice(E_C2P_FIELD_ID nFieldId, const TSC2PCmd* pkC2PCmd)
{

	int		i;
	int		nLocFieldId;
	
	for( i=0; i < pkC2PCmd->pkLayout->nNbFields; i++ )
	{
		nLocFieldId = pkC2PCmd->pkLayout->tab_fields[i].nFieldId;
		if( nLocFieldId ==  nFieldId)
		{
			return i;
		}
	}
	return -1;
}

/******************************************************************************/
int GetFieldIdLength(E_C2P_FIELD_ID nFieldId, const TSC2PCmd* pkC2PCmd)
{
	return pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.nLength;
}
/******************************************************************************/
static void AdjustC2PCmdInfo(TSC2PCmd* pkC2PCmd, int nPos, int nLength)
{
	int i;

	for (i = 0; i < pkC2PCmd->pkLayout->nNbFields; i++)
	{
		if( pkC2PCmd->nPresent[i] == PRESENT && pkC2PCmd->nPosTlv[i] > nPos)
		{
			pkC2PCmd->nPosTlv[i]  += nLength;
		}
	}

	for( i = 0; i < MAX_NB_MULTIFIELDS; i++ )
	{
		if( pkC2PCmd->nRepeatedFieldPos[i][0]  > nPos)
		{
			pkC2PCmd->nRepeatedFieldPos [i][0]  += nLength;
		}
	}

}

/******************************************************************************/
int  AnalyseC2PCmdInfo(const char *buffer_in, TSC2PCmd* pkC2PCmd)
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
	int		nCmdTagLen;
	int		nCmdLenLen;
	int		nFieldMaxLen;
	int		nFieldId;
	
	int		nLength;
	

	offset = 0;

	
	memcpy(pkC2PCmd->sCmdTag,buffer_in,sizeof(pkC2PCmd->sCmdTag));
	if (((unsigned char) ((*pkC2PCmd->sCmdTag) & 0x1F )) == (unsigned char) 0x1F)
	{
		nCmdTagLen=2;
		if (((unsigned char) ((*(pkC2PCmd->sCmdTag+1)) & 0x80 )) == (unsigned char) 0x80)
		{
			nCmdTagLen=3;
		}
	}
	else
	{
		nCmdTagLen=1;
	}

	if (((unsigned char) ((*(pkC2PCmd->sCmdTag + nCmdTagLen)) & 0x80 )) == (unsigned char) 0x80)
	{
		nCmdLenLen = (unsigned char)(*(pkC2PCmd->sCmdTag + nCmdTagLen)&0x7F);
		if( nCmdLenLen == 1 )
		{
			nLength = (unsigned char)(*(pkC2PCmd->sCmdTag + nCmdTagLen + 1));
		}
		else
		{
			nLength = (unsigned char)(*(pkC2PCmd->sCmdTag + nCmdTagLen + 1))*256 + (unsigned char)(*(pkC2PCmd->sCmdTag + nCmdTagLen + 2));
		}
		nCmdLenLen++;
	}
	else
	{
		nCmdLenLen = 1;
		nLength = ((unsigned char)(*(pkC2PCmd->sCmdTag + nCmdTagLen))&0x7F);
	}



	/* ADD TEST if( nLength > sizeof(pkC2PCmd->sData))*/
	/*	return NOK;*/

	pkC2PCmd->nLength = nLength;
	sPtr = buffer_in + nCmdTagLen + nCmdLenLen;
	memcpy(pkC2PCmd->sData,sPtr ,nLength);

	

	while (offset < pkC2PCmd->nLength)
	{
		memset( szTlvName,     '\0', sizeof(szTlvName));
		memset( szTlvValueLen,  0,  sizeof(szTlvValueLen));
		
		if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
		{
			nLenTag=2;
			if (((unsigned char) ((*(sPtr+1)) & 0x80 )) == (unsigned char) 0x80)
			{
				nLenTag=3;
			}
		}
		else
		{
			nLenTag=1;
		}
		HexToAscii( sPtr, szTlvName,   nLenTag*2 );

		if (((unsigned char) ((*(sPtr+nLenTag)) & 0x80 )) == (unsigned char) 0x80)
		{
			nLenLen = (unsigned char)(*(sPtr+nLenTag)&0x7F);
			if( nLenLen == 1 )
			{
				nBytes = (unsigned char)(*(sPtr+nLenTag + 1));
			}
			else
			{
				nBytes = (unsigned char)(*(sPtr+nLenTag + 1))*256 + (unsigned char)(*(sPtr+nLenTag + 2));
			}
			nLenLen++;
		}
		else
		{
			nLenLen = 1;
			nBytes = ((unsigned char)(*(sPtr+nLenTag))&0x7F);
		}
		
		nTlvIndice   = GetTagIndice(szTlvName,pkC2PCmd);

		if ( nTlvIndice >= 0)
		{
			nFieldId = pkC2PCmd->pkLayout->tab_fields[nTlvIndice].nFieldId;
			if( nBytes > pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.nLength )
				return(NOK);
			switch(pkC2PCmd->nPresent[ nTlvIndice ])
			{
			case NOT_PRESENT:
				pkC2PCmd->nPosTlv [ nTlvIndice ] = offset;
				pkC2PCmd->nPresent[ nTlvIndice ] = PRESENT;
				break;
			case PRESENT:
				/* if the field is already present, we flag it as "REPEATED". nPosTlv[indice] will contain the first occurence index
				* of this field. this index is on nRepeatedFieldPos[][] array.
				nRepeatedFieldPos[][0] contains the offset of the field
				nRepeatedFieldPos[][1] contains the index of the next occurence or NOT_PRESENT if no there's no other occurence*/
				pkC2PCmd->nPresent[ nTlvIndice ] = REPEATED;
				pkC2PCmd->nPosTlv [ nTlvIndice ] = pkC2PCmd->nNextFreeRFPos;

				pkC2PCmd->nRepeatedFieldPos[pkC2PCmd->nNextFreeRFPos][0] = pkC2PCmd->nPosTlv [ nTlvIndice ];
				pkC2PCmd->nRepeatedFieldPos[pkC2PCmd->nNextFreeRFPos][1] = ++pkC2PCmd->nNextFreeRFPos;
				pkC2PCmd->nRepeatedFieldPos[pkC2PCmd->nNextFreeRFPos][0] = offset;
				pkC2PCmd->nRepeatedFieldPos[pkC2PCmd->nNextFreeRFPos][1] = NOT_PRESENT;
				pkC2PCmd->nNextFreeRFPos++;
				break;
			case REPEATED:
				/* for repeated fields, we start with index of the first occurence and we search for the last occurence
				* then we add this new occurence*/
				i = pkC2PCmd->nPosTlv [ nTlvIndice ];
				while( pkC2PCmd->nRepeatedFieldPos[i][1] != NOT_PRESENT)
				{
					i = pkC2PCmd->nRepeatedFieldPos[i][1];
				}
				pkC2PCmd->nRepeatedFieldPos[i][1] = pkC2PCmd->nNextFreeRFPos;
				pkC2PCmd->nRepeatedFieldPos[pkC2PCmd->nNextFreeRFPos][0] = offset;
				pkC2PCmd->nRepeatedFieldPos[pkC2PCmd->nNextFreeRFPos][1] = NOT_PRESENT;
				pkC2PCmd->nNextFreeRFPos++;
				break;
			}
		}
		else
		{
		     /*Trace*/
		}
		nTlvLength = nLenTag + nLenLen +  nBytes;
		offset += nTlvLength;
		sPtr   += nTlvLength;
	}

	return OK;
}
/******************************************************************************/
int  GetC2PCmdInfoNextField (E_C2P_FIELD_ID nFieldId, const TSC2PCmd* pkC2PCmd, char *data, int* nLength, int nIter)
{
	int		nIndice;
	char	szAscTlvLen [MAX_TLV_LENGTH_LEN + 1];
	const char	*sPtr;
	int		i,j;
	int		nLenTag;
	int		nLenLen;
	int		nBytes;

	memset (szAscTlvLen, 0, sizeof(szAscTlvLen));
	
	if ( (nIndice = GetFieldIdIndice(nFieldId,pkC2PCmd)) < 0 )
		return(NOK);

 
	if (pkC2PCmd->nPresent[nIndice] == NOT_PRESENT)
		return(NOK);

	if( pkC2PCmd->nPresent[nIndice] == PRESENT )
	{
		sPtr =  pkC2PCmd->sData + pkC2PCmd->nPosTlv[nIndice];
	}
	else if( pkC2PCmd->nPresent[nIndice] == REPEATED )
	{
		i = pkC2PCmd->nPosTlv[nIndice];
		j = 0;
		while( j < nIter && pkC2PCmd->nRepeatedFieldPos[i][1] != NOT_PRESENT ) 
		{
			i = pkC2PCmd->nRepeatedFieldPos[i][1];
			j++;
		}
		if( j != nIter )
			return NOK;
		sPtr =  pkC2PCmd->sData + pkC2PCmd->nRepeatedFieldPos[i][0];
	}
    else
		return;

	if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
	{
		nLenTag=2;
		if (((unsigned char) ((*(sPtr+1)) & 0x80 )) == (unsigned char) 0x80)
		{
			nLenTag=3;
		}
	}
	else
	{
		nLenTag=1;
	}

	if (((unsigned char) ((*(sPtr+nLenTag)) & 0x80 )) == (unsigned char) 0x80)
	{
		nLenLen = (unsigned char)(*(sPtr+nLenTag)&0x7F);
		if( nLenLen == 1 )
		{
			nBytes = (unsigned char)(*(sPtr+nLenTag + 1));
		}
		else
		{
			nBytes = (unsigned char)(*(sPtr+nLenTag + 1))*256 + (unsigned char)(*(sPtr+nLenTag + 2));
		}
		nLenLen++;
	}
	else
	{
		nLenLen = 1;
		nBytes = ((unsigned char)(*(sPtr+nLenTag))&0x7F);
	}

	(*nLength) = nBytes;

	
	switch( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eFormatType)
	{
	case FF_ALPHA_A		:
	case FF_ALPHA_N		:
	case FF_ALPHA_AN	:
	case FF_ALPHA_ANS	:
	case FF_ALPHA_NS	:
	case FF_Z			:
	case FF_BIN			:
		if( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy( data, sPtr + nLenTag +  nLenLen,(*nLength));	
		}
		else if( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_EBCDIC )
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
	case FF_CBCD:
		CbcdToAscii( sPtr + nLenTag +  nLenLen, data, (*nLength)*2);
		(*nLength) = (*nLength)*2;
		break;
	}
	
	data[(*nLength)] = '\0';

	return(SUCCESS);

}
/******************************************************************************/
int  GetC2PCmdInfoField (E_C2P_FIELD_ID nFieldId, const TSC2PCmd* pkC2PCmd, char *data, int* nLength)
{
	return GetC2PCmdInfoNextField(nFieldId,pkC2PCmd,data,nLength,0);
}
/******************************************************************************/
int  AddC2PCmdInfoField (E_C2P_FIELD_ID nFieldId, TSC2PCmd* pkC2PCmd, const char *data, int nLength)
{
	int				nIndice;
	char			szAscTlvLen [MAX_TLV_LENGTH_LEN + 1];
	char			szAscTlvLen_add [MAX_TLV_LENGTH_LEN + 1];
	unsigned char	sLen[4];
	char			*sPtr;
	char			*sTag;
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
	int				nLenTagAscii;

	/*TODO: dans tous les add, si avec ce champ on va depasser sizeof(data) alors return error*/
	memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));
	memset (szAscTlvLen_add, '\0', sizeof(szAscTlvLen_add));

	
	if ( (nIndice = GetFieldIdIndice(nFieldId,pkC2PCmd)) < 0 )
		return(ERROR);

	if (pkC2PCmd->nPresent[nIndice] == PRESENT && 
		pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].eIsRepeatable != IS_REPEATABLE )
		return(ERROR);


	sPtr = pkC2PCmd->sData + pkC2PCmd->nLength;
	sTag = pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].szTag;
	nLenTagAscii = strlen(sTag);
	AsciiToHex(sTag, sPtr, nLenTagAscii );

	if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
	{
		nLenTag=2;
		if (((unsigned char) ((*(sPtr+1)) & 0x80 )) == (unsigned char) 0x80)
		{
			nLenTag=3;
		}
	}
	else
	{
		nLenTag=1;
	}
	
	nFieldMaxLength = pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.nLength;
	eLengthType		= pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eLengthType;
	eLengthUnit		= pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eLengthUnit;
	eFormatType		= pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eFormatType;

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
	else if ( nLenToPut <= 255)
	{
		nLenLen = 2;
		sLen[0]=0x81;
		sLen[1]=((unsigned char)nLenToPut);
	}
	else
	{
		nLenLen = 3;
		sLen[0]=0x82;
		sLen[1]=(unsigned char)(nLenToPut/256);
		sLen[2]=(unsigned char)(nLenToPut%256);
	}

	memcpy(pkC2PCmd->sData + pkC2PCmd->nLength + nLenTag , sLen, nLenLen);
	
	nBytes = 0;
	if( nLength > 0 )
	{
		switch(  eFormatType)
		{
		case FF_ALPHA_A		:
		case FF_ALPHA_N		:
		case FF_ALPHA_AN	:
		case FF_ALPHA_ANS	:
		case FF_ALPHA_NS	:
		case FF_Z			:
			if( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_ASCII )
			{
				memcpy(pkC2PCmd->sData + pkC2PCmd->nLength + nLenTag + nLenLen, data, nLength);
			}
			else if( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_EBCDIC )
			{
				AsciiToEbcdic ( data, 
								pkC2PCmd->sData + pkC2PCmd->nLength + nLenTag + nLenLen,
								nLength);
			}
			nBytes = nLength;
			break;
		case FF_BIN			:
			memcpy(pkC2PCmd->sData + pkC2PCmd->nLength + nLenTag + nLenLen, data, nLength);
			nBytes = nLength;
			break;
		case FF_HEX:
		
			nBytes = AsciiToHex( data , pkC2PCmd->sData + pkC2PCmd->nLength + nLenTag + nLenLen, 2*nLength);

			break;
		case FF_BCD:
			nBytes = AsciiToBcd( data,  pkC2PCmd->sData + pkC2PCmd->nLength + nLenTag + nLenLen, /*nLengthADD MOU20160401*/2*nLength);
			break;
		case FF_CBCD:
			nBytes = AsciiToCbcd( data,  pkC2PCmd->sData + pkC2PCmd->nLength + nLenTag + nLenLen, /*nLength ADD TEST MOU*/nLength*2);
			break;
		}
	}

	switch( pkC2PCmd->nPresent[ nIndice ])
	{
	case NOT_PRESENT:
		pkC2PCmd->nPosTlv [ nIndice ] = pkC2PCmd->nLength;
		pkC2PCmd->nPresent[ nIndice ] = PRESENT;
		break;
	case PRESENT:
		pkC2PCmd->nPresent[ nIndice ] = REPEATED;
		nCurrRFPos	= pkC2PCmd->nNextFreeRFPos;
		nCurrPos	= pkC2PCmd->nPosTlv [ nIndice ];
		pkC2PCmd->nPosTlv [ nIndice ] = nCurrRFPos;
		pkC2PCmd->nRepeatedFieldPos[nCurrRFPos][0] = nCurrPos;
		pkC2PCmd->nRepeatedFieldPos[nCurrRFPos][1] = ++pkC2PCmd->nNextFreeRFPos;
		pkC2PCmd->nRepeatedFieldPos[pkC2PCmd->nNextFreeRFPos][0] = pkC2PCmd->nLength;
		pkC2PCmd->nRepeatedFieldPos[pkC2PCmd->nNextFreeRFPos][1] = NOT_PRESENT;
		pkC2PCmd->nNextFreeRFPos++;
		break;
	case REPEATED:
		i = pkC2PCmd->nPosTlv [ nIndice ];
		while( pkC2PCmd->nRepeatedFieldPos[i][1] != NOT_PRESENT)
		{
			i = pkC2PCmd->nRepeatedFieldPos[i][1];
		}
		pkC2PCmd->nRepeatedFieldPos[i][1] = pkC2PCmd->nNextFreeRFPos;
		pkC2PCmd->nRepeatedFieldPos[pkC2PCmd->nNextFreeRFPos][0] = pkC2PCmd->nLength;
		pkC2PCmd->nRepeatedFieldPos[pkC2PCmd->nNextFreeRFPos][1] = NOT_PRESENT;
		pkC2PCmd->nNextFreeRFPos++;
		break;
	}

	pkC2PCmd->nLength += nBytes + nLenTag + nLenLen;
	
	return OK;

}
/******************************************************************************/
int  PutC2PCmdInfoField (E_C2P_FIELD_ID nFieldId, TSC2PCmd* pkC2PCmd, const char *data, int nLength)
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



	if ( (nIndice =GetFieldIdIndice(nFieldId,pkC2PCmd)) < 0 )
	{
		return(ERROR);
	}

	

	memset (szLastPart,  '\0', sizeof(szLastPart));


	if (pkC2PCmd->nPresent[nIndice] != PRESENT  || pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].eIsRepeatable == IS_REPEATABLE )
	{

		return AddC2PCmdInfoField(nFieldId,pkC2PCmd,data,nLength);
	}
		

	sPtr =  pkC2PCmd->sData + pkC2PCmd->nPosTlv[nIndice];

	if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
	{
		nLenTag=2;
		if (((unsigned char) ((*(sPtr+1)) & 0x80 )) == (unsigned char) 0x80)
		{
			nLenTag=3;
		}
	}
	else
	{
		nLenTag=1;
	}

	if (((unsigned char) ((*(sPtr+nLenTag)) & 0x80 )) == (unsigned char) 0x80)
	{
		
		nOldLenLen = (unsigned char)(*(sPtr+nLenTag)&0x7F);
		if( nOldLenLen == 1 )
		{
			nOldTlvLen = (unsigned char)(*(sPtr+nLenTag + 1));
		}
		else
		{
			nOldTlvLen = (unsigned char)(*(sPtr+nLenTag + 1))*256 + (unsigned char)(*(sPtr+nLenTag + 2));
		}
		nOldLenLen++;
	}
	else
	{
		nOldLenLen = 1;
		nOldTlvLen = ((unsigned char)(*(sPtr+nLenTag))&0x7F);
	}


	nFieldMaxLength = pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.nLength;
	eLengthType		= pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eLengthType;
	eLengthUnit		= pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eLengthUnit;
	eFormatType		= pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eFormatType;

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

	nLastPartLen = pkC2PCmd->nLength - pkC2PCmd->nPosTlv[nIndice] - nLenTag - nOldLenLen - nOldTlvLen;

	memcpy( szLastPart,
		sPtr + nLenTag + nOldLenLen + nOldTlvLen,
		nLastPartLen);

		
	if ( nLength <= 127)
	{
		nNewLenLen = 1;
		sLen[0]=(unsigned char)nLenToPut;
	}
	else if ( nLenToPut <= 255)
	{
		nNewLenLen = 2;
		sLen[0]=0x81;
		sLen[1]=((unsigned char)nLenToPut);
	}
	else
	{
		nNewLenLen = 3;
		sLen[0]=0x82;
		sLen[1]=((unsigned char)nLenToPut/256);
		sLen[2]=((unsigned char)nLenToPut%256);
	}

	memcpy(sPtr + nLenTag , sLen, nNewLenLen);

	nBytes = 0;
	if( nLength > 0 )
	{
		switch( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eFormatType)
		{
		case FF_ALPHA_A		:
		case FF_ALPHA_N		:
		case FF_ALPHA_AN	:
		case FF_ALPHA_ANS	:
		case FF_ALPHA_NS	:
		case FF_Z			:
			if( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_ASCII )
			{
				memcpy( sPtr + nLenTag + nNewLenLen,data,nLength);
			}
			else if( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_EBCDIC )
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
		case FF_CBCD:
			AsciiToCbcd( data, sPtr + nLenTag +  nNewLenLen, nLength);
			nBytes = nLength/2;
			break;
		}
	}

	memcpy(sPtr + nLenTag + nNewLenLen + nBytes,
		szLastPart,
		nLastPartLen);

	pkC2PCmd->nLength += nBytes - nOldTlvLen;
	AdjustC2PCmdInfo( pkC2PCmd, pkC2PCmd->nPosTlv[nIndice],nBytes - nOldTlvLen);
	return(SUCCESS);

}
/******************************************************************************/
/******************************************************************************/
int  SetCmdTag (char * buffer_out, TSC2PCmd* pkC2PCmd)
{
	int				nIndice;
	char			szAscTlvLen [MAX_TLV_LENGTH_LEN + 1];
	char			szAscTlvLen_add [MAX_TLV_LENGTH_LEN + 1];
	unsigned char	sLen[4];
	char			*sPtr;
	const char		*sTag;
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
	int				nLenTagAscii;
	

	/*TODO: dans tous les add, si avec ce champ on va depasser sizeof(data) alors return error*/
	memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));
	memset (szAscTlvLen_add, '\0', sizeof(szAscTlvLen_add));

	nIndice = pkC2PCmd->pkLayout->eCmdCode;
	
	

	sPtr = pkC2PCmd->sCmdTag;
	sTag = pkC2PCmd->pkCmdTagProperties->tabFieldsInfo[nIndice].szTag;
	nLenTagAscii = strlen(sTag);
	AsciiToHex(sTag, sPtr, nLenTagAscii );

	
	if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
	{
		nLenTag=2;
		if (((unsigned char) ((*(sPtr+1)) & 0x80 )) == (unsigned char) 0x80)
		{
			nLenTag=3;
		}
	}
	else
	{
		nLenTag=1;
	}

	

	nFieldMaxLength = pkC2PCmd->pkCmdTagProperties->tabFieldsInfo[nIndice].field_info.nLength;
	eLengthType		= pkC2PCmd->pkCmdTagProperties->tabFieldsInfo[nIndice].field_info.eLengthType;
	eLengthUnit		= pkC2PCmd->pkCmdTagProperties->tabFieldsInfo[nIndice].field_info.eLengthUnit;
	eFormatType		= pkC2PCmd->pkCmdTagProperties->tabFieldsInfo[nIndice].field_info.eFormatType;

	
	nLenToPut = pkC2PCmd->nLength;

	if( eLengthUnit == LU_BYTE   )
	{
		switch(eFormatType)
		{
		case FF_HEX:
		case FF_BCD:
			nFieldMaxLength = nFieldMaxLength*2;
			nLenToPut = (pkC2PCmd->nLength + 1)/2;
			break;
		case FF_X_BCD:
			nFieldMaxLength = (nFieldMaxLength - 1)*2;
			nLenToPut = (pkC2PCmd->nLength)/2 + 1;
			break;
		}
	}

	if( pkC2PCmd->nLength > nFieldMaxLength )
	{
		return NOK;
	}

	
	
	if ( nLenToPut <= 127)
	{
		nLenLen = 1;
		sLen[0]=(unsigned char)nLenToPut;
	}
	else if ( nLenToPut <= 255)
	{
		nLenLen = 2;
		sLen[0]=0x81;
		sLen[1]=((unsigned char)nLenToPut);
	}
	else
	{
		nLenLen = 3;
		sLen[0]=0x82;
		sLen[1]=(unsigned char)(nLenToPut/256);
		sLen[2]=(unsigned char)(nLenToPut%256);
	}

	memcpy(pkC2PCmd->sCmdTag + nLenTag , sLen, nLenLen);
	pkC2PCmd->nCmdTagLen = nLenTag + nLenLen;
	return pkC2PCmd->nCmdTagLen;
	
}
/******************************************************************************/
/******************************************************************************/

int  BuildC2PCmdInfoBuffer (char * buffer_out, TSC2PCmd* pkC2PCmd)
{
	int		nLength;
	int		nOffset;

	nOffset = SetCmdTag(buffer_out, pkC2PCmd);
	memcpy ( buffer_out, pkC2PCmd->sCmdTag , nOffset);
	memcpy ( buffer_out + nOffset, pkC2PCmd->sData , pkC2PCmd->nLength);
	
	buffer_out[pkC2PCmd->nLength + pkC2PCmd->nCmdTagLen] = '\0';
	return pkC2PCmd->nLength + pkC2PCmd->nCmdTagLen;
}
/******************************************************************************/
void DumpC2PCmdInfo(const TSC2PCmd* pkC2PCmd)
{
	char	sLine[MAX_LINE_TRC];
	int		nOffset = 0;
	int		i;
	int		nLen;
	int		nIter;
	int		nFieldId;
	char	szField[MAX_FIELD_LEN];
	char	szFieldAdd[MAX_FIELD_LEN];
	
	

	if( is_trace_level_on(P7_TL_TRACE) != OK )
		return;

	memset(sLine,'\0',sizeof(sLine));
	memset(szField,'\0',sizeof(szField));

	nIter = 0;
	for( i=0; i < pkC2PCmd->pkLayout->nNbFields;)
	{
		nFieldId = pkC2PCmd->pkLayout->tab_fields[i].nFieldId;
		/* If is a repeatable field, flag it so that we keep iterating on it until there's no other value */
		if( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].eIsRepeatable == IS_REPEATABLE )
		{
			if( GetC2PCmdInfoNextField(nFieldId,pkC2PCmd,szField,&nLen,nIter) != SUCCESS )
			{
				i++;
				nIter = 0;
				continue;
			}
			i++;
			nIter++;

		}
		else
		{
			if( GetC2PCmdInfoField(nFieldId,pkC2PCmd,szField,&nLen) != SUCCESS )
			{
				i++;
				continue;
			}
		}

		if( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eFormatType != FF_BIN )
		{
			trace_message(P7_TL_TRACE,"- FLD (%.6s) : (%03d) : [%s] " , pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].szTag , nLen ,szField );
		}
		else
		{
			nLen = HexToAscii(szField, szFieldAdd, nLen*2);
			trace_message(P7_TL_TRACE,"- FLD (%.6s) : (%03d) : [%s] " , pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].szTag , nLen ,szFieldAdd );
		}

		if( pkC2PCmd->pkProperties->tabFieldsInfo[nFieldId].eIsRepeatable != IS_REPEATABLE )
		{
			i++;
		}

	}
}
/******************************************************************************/
