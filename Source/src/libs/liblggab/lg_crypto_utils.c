#include <logigab.h>



/******************************************************************************/
#undef NOT_PRESENT	/* need to be removed from define */
#undef PRESENT

enum {
	NOT_PRESENT = 0,
	PRESENT		= 1,
	REPEATED	= 2,
};

/******************************************************************************/


/******************************************************************************/
static int GetTagIndice(const char* szTag, const TSLGCryptoInfo* pkLGCryptoInfo)
{

	int		i;
	int		nFieldId;
	
	for( i=0; i < pkLGCryptoInfo->pkTemplateLayout->nNbFields; i++ )
	{
		nFieldId = pkLGCryptoInfo->pkTemplateLayout->tab_fields[i];
		if( strcmp(pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].szTag,szTag) == 0 )
		{
			return i;
		}
	}
	return -1;
}
/******************************************************************************/
static int GetFieldIdIndice(E_LG_CRPT_FIELDS nFieldId, const TSLGCryptoInfo* pkLGCryptoInfo)
{

	int		i;
	int		nLocFieldId;
	
	for( i=0; i < pkLGCryptoInfo->pkTemplateLayout->nNbFields; i++ )
	{
		nLocFieldId = pkLGCryptoInfo->pkTemplateLayout->tab_fields[i];
		if( nLocFieldId ==  nFieldId)
		{
			return i;
		}
	}
	return -1;
}

/******************************************************************************/
static int GetFieldIdLength(E_LG_CRPT_FIELDS nFieldId, const TSLGCryptoInfo* pkLGCryptoInfo)
{
	return pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.nLength;
}
/******************************************************************************/
static void AdjustLGCryptoInfo(TSLGCryptoInfo* pkLGCryptoInfo, int nPos, int nLength)
{
	int i;

	for (i = 0; i < pkLGCryptoInfo->pkTemplateLayout->nNbFields; i++)
	{
		if( pkLGCryptoInfo->nPresent[i] == PRESENT && pkLGCryptoInfo->nPosTlv[i] > nPos)
		{
			pkLGCryptoInfo->nPosTlv[i]  += nLength;
		}
	}


}
/******************************************************************************/
 int AdjustLGCryptoMsg(TSLGCryptoMsg* 	cryptoMsgIn,TSLGCryptoMsg* 	cryptoMsgOut)
{

	TSLGCryptoInfo  cryptoInfo;

	char			sLine[MAX_LINE_TRC];

    int				nLength;
    int				i;
	int				nResult;
	char			sData[512];

	TSLGCryptoInfo  cryptoEltsCryptoInfo;
	TSLGCryptoInfo  cryptoParams1Info;
	TSLGCryptoInfo  cryptoParams2Info;
	TSLGCryptoInfo  cryptoParams3Info;
	TSLGCryptoInfo  cryptoParams4Info;
	TSLGCryptoInfo  cryptoParams5Info;



	InitLGCryptoInfo(&cryptoEltsCryptoInfo,LG_TMPLT_LAY_CRYPTO_ELTS);
	InitLGCryptoInfo(&cryptoParams1Info,LG_TMPLT_LAY_PARAM_1);
	InitLGCryptoInfo(&cryptoParams2Info,LG_TMPLT_LAY_PARAM_2);
	InitLGCryptoInfo(&cryptoParams3Info,LG_TMPLT_LAY_PARAM_3);
	InitLGCryptoInfo(&cryptoParams4Info,LG_TMPLT_LAY_PARAM_4);
	InitLGCryptoInfo(&cryptoParams5Info,LG_TMPLT_LAY_PARAM_5);

	for( i=0; i < LG_TMPLT_LAY_CRYPTO_ELTS;i++)
	{
		if( cryptoMsgIn->nPresent[i] == PRESENT )
		{
			if( GetLGCryptoInfo(cryptoMsgIn->tabCryptoInfo[i].pkTemplateLayout->template_code,cryptoMsgIn,&cryptoInfo) ==SUCCESS)
			{
				AddLGCryptoInfo(cryptoMsgIn->tabCryptoInfo[i].pkTemplateLayout->template_code, cryptoMsgOut,&cryptoInfo);
			}
		}

	}

	if( GetLGCryptoInfo(LG_TMPLT_CRYPTO_ELTS,cryptoMsgIn,&cryptoEltsCryptoInfo) ==SUCCESS)
	{
		sprintf(sData,"%s","0002");

		PutLGCryptoInfoField(LG_CRPT_FLD_ENC_ALGO,&cryptoEltsCryptoInfo,sData,4);
		AddLGCryptoInfo(LG_TMPLT_CRYPTO_ELTS, cryptoMsgOut,&cryptoEltsCryptoInfo);

		sprintf(sData,"%s","0001");
		AddLGCryptoInfoField(LG_CRPT_FLD_HASH_ALGO,&cryptoParams1Info,sData,4);
		sprintf(sData,"%s","0001");
		AddLGCryptoInfoField(LG_CRPT_FLD_MASK,&cryptoParams1Info,sData,4);
		sprintf(sData,"%s","00");
		AddLGCryptoInfoField(LG_CRPT_FLD_P_PARAM_IND,&cryptoParams1Info,sData,2);

		/*AddLGCryptoInfo(LG_TMPLT_PARAM_1, cryptoMsgOut,&cryptoParams1Info);*/

		sprintf(sData,"%s","0001");
		AddLGCryptoInfoField(LG_CRPT_FLD_HASH_ALGO,&cryptoParams3Info,sData,4);
		AddLGCryptoInfo(LG_TMPLT_PARAM_3, cryptoMsgOut,&cryptoParams3Info);

	    memset(sData,'\0',sizeof(sData));
		AddLGCryptoInfoField(LG_CRPT_FLD_P,&cryptoParams4Info,sData,0);
		/*AddLGCryptoInfo(LG_TMPLT_PARAM_4, cryptoMsgOut,&cryptoParams4Info);*/
	}

	trace_event("End AdjustLGCryptoMsg()",PROCESSING);

    return OK;
}

/******************************************************************************/
int  AnalyseLGCryptoMsg(const char *buffer_in, int nLength, TSLGCryptoMsg* pkLGCryptoMsg)
{

	TSLGCryptoInfo  pkLGCryptoInfo;
	const unsigned char  *sPtr;
	int		offset;
	int		nTlvLength, nBytes;
	int		nTlvIndice;
	char	szTlvName [ MAX_TLV_TAG_LEN + 1 ];
	char	szTlvValueLen [ MAX_TLV_LENGTH_LEN + 4 ];
	int		i;
	int		nLenTag;
	int		nLenLen;
	int		nTemplateLenLen;
	int		nFieldMaxLen;
	int		nFieldId;
	int		nTemplateId;
	int		nFields;


	offset = 0;
	nFields= 0;

	sPtr = buffer_in;

	InitLGCryptoMsg(pkLGCryptoMsg);

	pkLGCryptoMsg->nLength=nLength;
	memcpy(pkLGCryptoMsg->sData,buffer_in ,nLength);

	while (offset < nLength )
	{
		memset( szTlvName,     '\0', sizeof(szTlvName));
		memset( szTlvValueLen,  0,  sizeof(szTlvValueLen));

		if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
		{
			nLenTag=2;
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

		nTlvIndice = GetLGCryptoTemplateId(szTlvName);

		if ( nTlvIndice >= 0)
		{

			AnalyseLGCryptoInfo(sPtr,&pkLGCryptoInfo);

			pkLGCryptoMsg->nPosTlv [ nTlvIndice ] = offset;
			pkLGCryptoMsg->nPresent[ nTlvIndice ] = PRESENT;
			pkLGCryptoMsg->tabCryptoInfo[ nTlvIndice]=pkLGCryptoInfo;
			nFields++;


		}
		else
		{
			/*Trace*/
		}
		nTlvLength = nLenTag + nLenLen +  nBytes;
		offset += nTlvLength;
		sPtr   += nTlvLength;
	}

	pkLGCryptoMsg->nNbFields=nFields;

	return OK;
}

int  AnalyseLGCryptoInfo(const char *buffer_in, TSLGCryptoInfo* pkLGCryptoInfo)
{
	const unsigned char  *sPtr;
	int		offset;
	int		nTlvLength, nBytes;
	int		nTlvIndice;
	char	szTlvName [ MAX_TLV_TAG_LEN + 1 ];
	char	szTlvValueLen [ MAX_TLV_LENGTH_LEN + 4 ];
	int		i;
	int		nLenTag;
	int		nLenLen;
	int		nTemplateLenLen;
	int		nFieldMaxLen;
	int		nFieldId;
	int		nTemplateId;
	int		nLength;

	offset = 0;

	HexToAscii(buffer_in,szTlvName,2);
	szTlvName[2]='\0';

	nTemplateId = GetLGCryptoTemplateId(szTlvName);
	if( nTemplateId < 0 )
		return NOK;

	InitLGCryptoInfo(pkLGCryptoInfo,nTemplateId);

	if (((unsigned char) ((*(buffer_in + 1)) & 0x80 )) == (unsigned char) 0x80)
	{
		nTemplateLenLen = (unsigned char)(*(buffer_in + 1)&0x7F);
		if( nTemplateLenLen == 1 )
		{
			nLength = (unsigned char)(*(buffer_in + 1 + 1));
		}
		else
		{
			nLength = (unsigned char)(*(buffer_in + 1 + 1))*256 + (unsigned char)(*(buffer_in + 1 + 2));
		}
		nTemplateLenLen++;
	}
	else
	{
		nTemplateLenLen = 1;
		nLength = ((unsigned char)(*(buffer_in + 1))&0x7F);
	}

	if( nLength > sizeof(pkLGCryptoInfo->sData))
		return NOK;

	pkLGCryptoInfo->nLength = nLength;
	sPtr = buffer_in + 1 + nTemplateLenLen;
	memcpy(pkLGCryptoInfo->sData,sPtr ,nLength);


	while (offset < pkLGCryptoInfo->nLength)
	{
		memset( szTlvName,     '\0', sizeof(szTlvName));
		memset( szTlvValueLen,  0,  sizeof(szTlvValueLen));

		if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
		{
			nLenTag=2;
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

		nTlvIndice   = GetTagIndice(szTlvName,pkLGCryptoInfo);

		if ( nTlvIndice >= 0)
		{
			nFieldId = pkLGCryptoInfo->pkTemplateLayout->tab_fields[nTlvIndice];
			if( nBytes > pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.nLength )
				return(NOK);
			pkLGCryptoInfo->nPosTlv [ nTlvIndice ] = offset;
			pkLGCryptoInfo->nPresent[ nTlvIndice ] = PRESENT;
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
/******************************************************************************/
int  GetLGCryptoInfo(char* template_code,const TSLGCryptoMsg* pkLGCryptoMsg, TSLGCryptoInfo* pkLGCryptoInfo)
{
	int		nIndice;

	if ( (nIndice = GetLGCryptoTemplateId(template_code)) < 0 )
			return(NOK);

	if (pkLGCryptoMsg->nPresent[nIndice] == NOT_PRESENT)
		return(NOK);

	(*pkLGCryptoInfo) = pkLGCryptoMsg->tabCryptoInfo[nIndice];

	return(SUCCESS);

}

/******************************************************************************/
int  GetLGCryptoInfoField (E_LG_CRPT_FIELDS nFieldId, const TSLGCryptoInfo* pkLGCryptoInfo, char *data, int* nLength)
{
	int		nIndice;
	char	szAscTlvLen [MAX_TLV_LENGTH_LEN + 1];
	const char	*sPtr;
	int		i,j;
	int		nLenTag;
	int		nLenLen;
	int		nBytes;

	memset (szAscTlvLen, 0, sizeof(szAscTlvLen));
	
	if ( (nIndice = GetFieldIdIndice(nFieldId,pkLGCryptoInfo)) < 0 )
		return(NOK);

 
	if (pkLGCryptoInfo->nPresent[nIndice] == NOT_PRESENT)
		return(NOK);

	if( pkLGCryptoInfo->nPresent[nIndice] == PRESENT )
	{
		sPtr =  pkLGCryptoInfo->sData + pkLGCryptoInfo->nPosTlv[nIndice];
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

	
	switch( pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eFormatType)
	{
	case FF_ALPHA_A		:
	case FF_ALPHA_N		:
	case FF_ALPHA_AN	:
	case FF_ALPHA_ANS	:
	case FF_ALPHA_NS	:
	case FF_Z			:
	case FF_BIN			:
		if( pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy( data, sPtr + nLenTag +  nLenLen,(*nLength));	
		}
		else if( pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_EBCDIC )
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
int  AddLGCryptoInfo(char* template_code, TSLGCryptoMsg* pkLGCryptoMsg,const TSLGCryptoInfo* pkLGCryptoInfo)
{
	int				nIndice;
	char			*sPtr;
	unsigned char	sLen[4];
	int				nLenLen;
	int				nLenToPut;
	int				nLenTagAscii;
	int				nLenTag;

	if ( (nIndice = GetLGCryptoTemplateId(template_code)) < 0 )
			return(ERROR);

	/*if (pkLGCryptoMsg->nPresent[nIndice] == PRESENT  )
		return(ERROR);*/

	sPtr = pkLGCryptoMsg->sData + pkLGCryptoMsg->nLength;

	nLenTagAscii = strlen(template_code);
	AsciiToHex(template_code, sPtr, nLenTagAscii );

	if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
	{
		nLenTag=2;
	}
	else
	{
		nLenTag=1;
	}


	nLenToPut = pkLGCryptoInfo->nLength;

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

	memcpy(sPtr + nLenTag , sLen, nLenLen);
	memcpy( sPtr + nLenTag+ nLenLen , pkLGCryptoInfo->sData, pkLGCryptoInfo->nLength);

	pkLGCryptoMsg->nPresent[nIndice]=PRESENT;
	pkLGCryptoMsg->nNbFields += 1;
	pkLGCryptoMsg->nPosTlv [ nIndice ] = pkLGCryptoMsg->nLength;
	pkLGCryptoMsg->nLength += pkLGCryptoInfo->nLength + nLenTag+ nLenLen;

    pkLGCryptoMsg->tabCryptoInfo[nIndice] = *pkLGCryptoInfo;

	return(SUCCESS);

}
/******************************************************************************/
int  AddLGCryptoInfoField (E_LG_CRPT_FIELDS nFieldId, TSLGCryptoInfo* pkLGCryptoInfo, const char *data, int nLength)
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

	
	if ( (nIndice = GetFieldIdIndice(nFieldId,pkLGCryptoInfo)) < 0 )
		return(ERROR);


	if (pkLGCryptoInfo->nPresent[nIndice] == PRESENT  )
		return(ERROR);


	sPtr = pkLGCryptoInfo->sData + pkLGCryptoInfo->nLength;
	sTag = pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].szTag;
	nLenTagAscii = strlen(sTag);
	AsciiToHex(sTag, sPtr, nLenTagAscii );

	
	if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)
	{
		nLenTag=2;
	}
	else
	{
		nLenTag=1;
	}


	nFieldMaxLength = pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.nLength;
	eLengthType		= pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eLengthType;
	eLengthUnit		= pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eLengthUnit;
	eFormatType		= pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eFormatType;

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


	memcpy(pkLGCryptoInfo->sData + pkLGCryptoInfo->nLength + nLenTag , sLen, nLenLen);
	
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
			if( pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_ASCII )
			{
				memcpy(pkLGCryptoInfo->sData + pkLGCryptoInfo->nLength + nLenTag + nLenLen, data, nLength);
			}
			else if( pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_EBCDIC )
			{
				AsciiToEbcdic ( data, 
								pkLGCryptoInfo->sData + pkLGCryptoInfo->nLength + nLenTag + nLenLen,
								nLength);
			}
			nBytes = nLength;
			break;
		case FF_BIN			:
			memcpy(pkLGCryptoInfo->sData + pkLGCryptoInfo->nLength + nLenTag + nLenLen, data, nLength);
			nBytes = nLength;
			break;
		case FF_HEX:
			nBytes = AsciiToHex( data , pkLGCryptoInfo->sData + pkLGCryptoInfo->nLength + nLenTag + nLenLen, nLength);
			break;
		case FF_BCD:
			nBytes = AsciiToBcd( data,  pkLGCryptoInfo->sData + pkLGCryptoInfo->nLength + nLenTag + nLenLen, nLength);
			break;
		case FF_CBCD:
			nBytes = AsciiToCbcd( data,  pkLGCryptoInfo->sData + pkLGCryptoInfo->nLength + nLenTag + nLenLen, nLength);
			break;
		}
	}
	

	switch( pkLGCryptoInfo->nPresent[ nIndice ])
	{
	case NOT_PRESENT:
		pkLGCryptoInfo->nPosTlv [ nIndice ] = pkLGCryptoInfo->nLength;
		pkLGCryptoInfo->nPresent[ nIndice ] = PRESENT;
		break;
	}

	pkLGCryptoInfo->nLength += nBytes + nLenTag + nLenLen;
	return OK;

}
/******************************************************************************/
int  PutLGCryptoInfo(char* template_code, TSLGCryptoMsg* pkLGCryptoMsg,const TSLGCryptoInfo* pkLGCryptoInfo)
{
	int		nIndice;
	char	*sPtr;

	if ( (nIndice = GetLGCryptoTemplateId(template_code)) < 0 )
			return(ERROR);

	if (pkLGCryptoMsg->nPresent[nIndice] != PRESENT )
	{

		return AddLGCryptoInfo(template_code,pkLGCryptoMsg,pkLGCryptoInfo);
	}

	sPtr =  pkLGCryptoMsg->sData + pkLGCryptoMsg->nPosTlv[nIndice];


	return(SUCCESS);

}
/******************************************************************************/
int  PutLGCryptoInfoField (E_LG_CRPT_FIELDS nFieldId, TSLGCryptoInfo* pkLGCryptoInfo, const char *data, int nLength)
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



	if ( (nIndice =GetFieldIdIndice(nFieldId,pkLGCryptoInfo)) < 0 )
	{
		return(ERROR);
	}

	

	memset (szLastPart,  '\0', sizeof(szLastPart));


	if (pkLGCryptoInfo->nPresent[nIndice] != PRESENT )
	{

		return AddLGCryptoInfoField(nFieldId,pkLGCryptoInfo,data,nLength);
	}
		

	sPtr =  pkLGCryptoInfo->sData + pkLGCryptoInfo->nPosTlv[nIndice];

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

	


	nFieldMaxLength = pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.nLength;
	eLengthType		= pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eLengthType;
	eLengthUnit		= pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eLengthUnit;
	eFormatType		= pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eFormatType;

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

	nLastPartLen = pkLGCryptoInfo->nLength - pkLGCryptoInfo->nPosTlv[nIndice] - nLenTag - nOldLenLen - nOldTlvLen;

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
		switch( pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eFormatType)
		{
		case FF_ALPHA_A		:
		case FF_ALPHA_N		:
		case FF_ALPHA_AN	:
		case FF_ALPHA_ANS	:
		case FF_ALPHA_NS	:
		case FF_Z			:
			if( pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_ASCII )
			{
				memcpy( sPtr + nLenTag + nNewLenLen,data,nLength);
			}
			else if( pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_EBCDIC )
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

	pkLGCryptoInfo->nLength += nBytes - nOldTlvLen;
	AdjustLGCryptoInfo( pkLGCryptoInfo, pkLGCryptoInfo->nPosTlv[nIndice],nBytes - nOldTlvLen);
	return(SUCCESS);

}
/******************************************************************************/
static int  SetTemplateCode (char * buffer_out, TSLGCryptoInfo* pkLGCryptoInfo)
{
	
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
	

	
	memset (szAscTlvLen, '\0', sizeof(szAscTlvLen));
	memset (szAscTlvLen_add, '\0', sizeof(szAscTlvLen_add));

	
	sTag = pkLGCryptoInfo->pkTemplateLayout->template_code;
	nLenTagAscii = strlen(sTag);
	AsciiToHex(sTag, buffer_out, nLenTagAscii );

	nLenTag=1;

	
	nLenToPut = pkLGCryptoInfo->nLength;

	
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
		sLen[1]=((unsigned char)nLenToPut/256);
		sLen[2]=((unsigned char)nLenToPut%256);
	}

	memcpy(buffer_out + 1 , sLen, nLenLen);
	return 1 + nLenLen;
	
}
/******************************************************************************/
int  BuildLGCryptoInfoBuffer (char * buffer_out, TSLGCryptoInfo* pkLGCryptoInfo)
{
	int		nLength;
	int		nOffset;
	int		nHdrLen;

	nHdrLen = SetTemplateCode(buffer_out, pkLGCryptoInfo);
	memcpy ( buffer_out + nHdrLen, pkLGCryptoInfo->sData , pkLGCryptoInfo->nLength);
	buffer_out[pkLGCryptoInfo->nLength + nHdrLen] = '\0';
	return pkLGCryptoInfo->nLength + nHdrLen;
}
/******************************************************************************/
void DumpLGCryptoMsg(const TSLGCryptoMsg* pkLGCryptoMsg)
{

    trace_event("Start DumpLGCryptoMsg ()",PROCESSING);

	if( is_trace_level_on(P7_TL_TRACE) != OK )
		return;

	char	sLine[MAX_LINE_TRC];
	int		i;

	for( i=0; i < LG_TMPLT_LAY_QTY;i++)
	{

		if( pkLGCryptoMsg->nPresent[i] == PRESENT )
		{
			trace_message(P7_TL_TRACE,"TEMPLATE: '%.2s' : (%s)" , pkLGCryptoMsg->tabCryptoInfo[i].pkTemplateLayout->template_code,tab_templatesDesc[pkLGCryptoMsg->tabCryptoInfo[i].pkTemplateLayout->eTemplate].szLabel);

			DumpLGCryptoInfo(&pkLGCryptoMsg->tabCryptoInfo[i]);
		}
	}

	trace_event("End DumpLGCryptoMsg ()",PROCESSING);
}
/******************************************************************************/
void DumpLGCryptoInfo(const TSLGCryptoInfo* pkLGCryptoInfo)
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
	for( i=0; i < pkLGCryptoInfo->pkTemplateLayout->nNbFields;)
	{
		nFieldId = pkLGCryptoInfo->pkTemplateLayout->tab_fields[i];
		/* If is a repeatable field, flag it so that we keep iterating on it until there's no other value */
		
		if( GetLGCryptoInfoField(nFieldId,pkLGCryptoInfo,szField,&nLen) != SUCCESS )
		{
			i++;
			continue;
		}
		

		if( pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].field_info.eFormatType != FF_BIN )
		{
			trace_message(P7_TL_TRACE,"\t- FLD (%.6s) : (%03d) : [%s] " , pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].szTag , nLen ,szField );
		}
		else
		{
			nLen = HexToAscii(szField, szFieldAdd, nLen*2);
			trace_message(P7_TL_TRACE,"\t- FLD (%.6s) : (%03d) : [%s] " , pkLGCryptoInfo->pkTagProperties->tabFieldsInfo[nFieldId].szTag , nLen ,szFieldAdd );
		}
		i++;
	}


}
/******************************************************************************/


