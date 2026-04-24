#include <stdio.h>
#include <stdlib.h>

#include <iso_bic.h>
#include <tools.h>
#include <define.h>

/******************************************************************************/
int GetBicField(int field_no, TSBicInfo * msgInfo, char *data, int *nLength)
{
	int length;
	int nBytes;
	char szBicFldLen[4];	

	memset( szBicFldLen, 0, 4);	
	if (CheckBit (msgInfo->sBitMap, field_no))
	{
		switch (GetBicFieldType(field_no))
		{
		case BIC_LL_BIN :
			memcpy( szBicFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
			length = atoi(szBicFldLen);
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
			*nLength = length;
			break;

		case BIC_LLL_BIN :
			memcpy( szBicFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
			length = atoi(szBicFldLen);
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
			*nLength = length;
			break;

		case BIC_FIX_BIN   :
		case BIC_FIX_ALPHA :
			length = GetBicFieldLength(field_no);
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
			data[length] = '\0';
			*nLength = length;
			break;

		case BIC_FIX_HEX :                           
			length = GetBicFieldLength(field_no);
			nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
				data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case BIC_LL_HEX :                           
			memcpy( szBicFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
			length = atoi(szBicFldLen);
			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case BIC_LLL_HEX :                           
			memcpy ( szBicFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			length = atoi(szBicFldLen);
			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case BIC_LL_ALPHA : 
			memcpy ( szBicFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
			length = atoi(szBicFldLen);
			memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case BIC_LLL_ALPHA : 
			memcpy ( szBicFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			length = atoi(szBicFldLen);
			memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+3, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case BIC_VAR_ALPHA :                           
			length = *(msgInfo->sData + msgInfo->nFieldPos[field_no]);
			EbcdicToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+1, 
				data, length); 
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

int AddBicField(int field_no, TSBicInfo *msgInfo, char *data, int nLength)
{
	int nBytes  = 0;
	int i;
	char szBicFldLen[4];	
	char  sLine [MAX_LINE_TRC];

	memset( szBicFldLen, 0, 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	msgInfo->nFieldPos[field_no] = msgInfo->nLength;
	switch (GetBicFieldType(field_no))
	{
	case BIC_FIX_ALPHA :
		if ( GetBicFieldLength(field_no) != nLength)
			return(ECHEC);
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case BIC_LL_ALPHA :
		sprintf ( szBicFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szBicFldLen, 2);
		msgInfo->nLength += 2;
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case BIC_LLL_ALPHA :
		sprintf ( szBicFldLen, "%.3d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szBicFldLen, 3);
		msgInfo->nLength += 3;
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case BIC_FIX_HEX :
		if ( GetBicFieldLength(field_no) != nLength)
			return(ECHEC);
		nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case BIC_LL_HEX :
		sprintf ( szBicFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szBicFldLen, 2);
		msgInfo->nLength += 2;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case BIC_LLL_HEX :
		sprintf ( szBicFldLen, "%.3d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szBicFldLen, 3);
		msgInfo->nLength += 3;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case BIC_FIX_BIN :
		if ( GetBicFieldLength(field_no) != nLength)
			return(ECHEC);
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case BIC_LL_BIN :
		sprintf ( szBicFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szBicFldLen, 2);
		msgInfo->nLength += 2;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case BIC_LLL_BIN :
		sprintf ( szBicFldLen, "%.3d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szBicFldLen, 3);
		msgInfo->nLength += 3;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case BIC_VAR_ALPHA :
		*(msgInfo->sData + msgInfo->nLength) = nLength;
		msgInfo->nLength++;
		AsciiToEbcdic(data, msgInfo->sData+msgInfo->nLength, nLength);
		nBytes = nLength;
		break;

	default :
		return(ECHEC);
	}
	msgInfo->nLength += nBytes;
	for (i=field_no; i< MAX_BIC_FIELDS; i++)
		msgInfo->nFieldPos[i+1] = msgInfo->nLength;
	SetBit(msgInfo->sBitMap, field_no+1);
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);
	return(SUCCES);
}

int InsertBicField(int field_no, TSBicInfo *msgInfo, char *data, int nLength)
{
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i;
	char szBicData [999];	
	char szBicFldLen[4];	
	int  nOldFldLen,nOldLen;
	char szWork    [LG_MAX];	

	memset( szBicFldLen, 0, 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	switch (GetBicFieldType(field_no))
	{
	case BIC_FIX_ALPHA :
		if ( GetBicFieldLength(field_no) != nLength)
			return(ECHEC);
		nBytes = nLength;
		memcpy (szBicData, data, nBytes);
		break;

	case BIC_LL_ALPHA :
		sprintf ( szBicFldLen, "%.2d", nLength);	
		memcpy( szBicData,        szBicFldLen, 2);
		nBytes=2;
		memcpy( szBicData+2, data,        nLength);
		nBytes += nLength;
		break;

	case BIC_LLL_ALPHA :
		sprintf ( szBicFldLen, "%.3d", nLength);	
		nBytes = 3;
		memcpy( szBicData, szBicFldLen, 3);
		memcpy( szBicData+3, data,        nLength);
		nBytes += nLength;
		break;

	case BIC_FIX_HEX :
		if ( GetBicFieldLength(field_no) != nLength)
			return(ECHEC);
		nBytes = AsciiToHex(data, szBicData, nLength);
		break;

	case BIC_LL_HEX :
		sprintf ( szBicFldLen, "%.2d", nLength);	
		nBytes1 = 2;
		memcpy( szBicData, szBicFldLen, nBytes1);
		nBytes = AsciiToHex( data, szBicData+2, nLength);
		nBytes += nBytes1;
		break;

	case BIC_LLL_HEX :
		sprintf ( szBicFldLen, "%.3d", nLength);	
		nBytes1 = 3;
		memcpy( szBicData, szBicFldLen, 3);
		nBytes = AsciiToHex( data, szBicData+3, nLength);
		nBytes += nBytes1;
		break;

	case BIC_FIX_BIN :
		if ( GetBicFieldLength(field_no) != nLength)
			return(ECHEC);
		nBytes = nLength;
		memcpy( szBicData, data, nBytes);
		break;

	case BIC_LL_BIN :
		sprintf ( szBicFldLen, "%.2d", nLength);	
		nBytes = 2;
		memcpy( szBicData, szBicFldLen, nBytes);
		memcpy( szBicData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	case BIC_LLL_BIN :
		sprintf ( szBicFldLen, "%.3d", nLength);	
		nBytes = 3;
		memcpy( szBicData, szBicFldLen, nBytes);
		memcpy( szBicData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	case BIC_VAR_ALPHA :
		*(szBicData) = nLength;
		nBytes =1;
		AsciiToEbcdic(data, szBicData+1, nLength);
		nBytes += nLength;
		break;

	default :
		return(ECHEC);
	}
	nOldFldLen = 0;
	memset(szWork, 0, sizeof(szWork));
	nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
	memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

	for (i = MAX_BIC_FIELDS-1; i > field_no; i--)
		msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;

	memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szBicData, nBytes);
	msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
	msgInfo->nLength += nBytes-nOldFldLen;
	SetBit(msgInfo->sBitMap, field_no+1);
	memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);
	return(SUCCES);
}

int PutBicField(int field_no, TSBicInfo *msgInfo, char *data, int nLength)
{
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i, nOldFldLen,nOldLen;
	char szBicData [999];	
	char szWork    [LG_MAX];	
	char szBicFldLen[4];	

	memset( szBicFldLen, '\0', 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
	{
		switch (GetBicFieldType(field_no))
		{
		case BIC_FIX_ALPHA :
			if ( GetBicFieldLength(field_no) != nLength)
				return(ECHEC);
			nBytes = nLength;
			memcpy (szBicData, data, nBytes);
			break;

		case BIC_LL_ALPHA :
			sprintf ( szBicFldLen, "%.2d", nLength);	
			memcpy( szBicData,        szBicFldLen, 2);
			nBytes=2;
			memcpy( szBicData+2, data,        nLength);
			nBytes += nLength;
			break;

		case BIC_LLL_ALPHA :
			sprintf ( szBicFldLen, "%.3d", nLength);	
			nBytes = 3;
			memcpy( szBicData, szBicFldLen, 3);
			memcpy( szBicData+3, data,        nLength);
			nBytes += nLength;
			break;

		case BIC_FIX_HEX :
			if ( GetBicFieldLength(field_no) != nLength)
				return(ECHEC);
			nBytes = AsciiToHex(data, szBicData, nLength);
			break;

		case BIC_LL_HEX :
			sprintf ( szBicFldLen, "%.2d", nLength);	
			nBytes1 = 2;
			memcpy( szBicData, szBicFldLen, nBytes1);
			nBytes = AsciiToHex( data, szBicData+2, nLength);
			nBytes += nBytes1;
			break;

		case BIC_LLL_HEX :
			sprintf ( szBicFldLen, "%.3d", nLength);	
			nBytes1 = 3;
			memcpy( szBicData, szBicFldLen, 3);
			nBytes = AsciiToHex( data, szBicData+3, nLength);
			nBytes += nBytes1;
			break;

		case BIC_FIX_BIN :
			if ( GetBicFieldLength(field_no) != nLength)
				return(ECHEC);
			nBytes = nLength;
			memcpy( szBicData, data, nBytes);
			break;

		case BIC_LL_BIN :
			sprintf ( szBicFldLen, "%.2d", nLength);	
			nBytes = 2;
			memcpy( szBicData, szBicFldLen, nBytes);
			memcpy( szBicData+nBytes, data, nLength);
			nBytes += nLength;
			break;

		case BIC_LLL_BIN :
			sprintf ( szBicFldLen, "%.3d", nLength);	
			nBytes = 3;
			memcpy( szBicData, szBicFldLen, nBytes);
			memcpy( szBicData+nBytes, data, nLength);
			nBytes += nLength;
			break;

		case BIC_VAR_ALPHA :
			*(szBicData) = nLength;
			nBytes =1;
			AsciiToEbcdic(data, szBicData+1, nLength);
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

		for (i = MAX_BIC_FIELDS-1; i > field_no; i--)
		{
			msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
		}
		memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szBicData, nBytes);
		msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
		msgInfo->nLength += nBytes-nOldFldLen;
		SetBit(msgInfo->sBitMap, field_no+1);
		memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
		if ( (field_no >64) && (field_no < 128))
			SetBit(msgInfo->sBitMap, 1);
		return(SUCCES);
	}
	else
		return ( InsertBicField (field_no, msgInfo, data, nLength));

}

int BicBuildMsg (char * buffer_snd, TSBicInfo *msgInfo)
{
	char szMsgType [5];
	char sLine     [MAX_LINE_TRC];
	char sDest     [16+1];
	char sSecDest  [16+1];
	char sSecBitMap[16];
	char sData     [LG_MAX];
	int  nBytes;
	int  iMapCtr;
	int  nLength;
	int  i,j,k;
	int  c=0;

	sprintf(szMsgType, "%.4d", msgInfo->nMsgType);

	sprintf(sLine,"Start BicBuildMsg msgInfo->nLength(%d)", msgInfo->nLength);
	trace_event(sLine, PROCESSING);
	/*  Header */

	/*???*/
	/*memcpy (buffer_snd, "ISO016000077", 12);*/

	memcpy (buffer_snd , szMsgType, 4);


	SetBit(msgInfo->sBitMap, 1); /* B24 exige le bitmap secondaire même vide */
	memset (sDest,    0, sizeof (sDest)); 
	HexToAscii(msgInfo->sBitMap, sDest, 16);

	memcpy( buffer_snd+4, sDest, 16);    

	memset (sSecBitMap, 0, sizeof (sSecBitMap));
	for (j=0; j < BIC_BITMAP_LEN; j++)
		sSecBitMap[j] = 0x00;

	k=0; 
	for (i=64; i< MAX_BIC_FIELDS; i++)
	{
		memset (sData,      0, sizeof (sData)); 
		if (GetBicField(i, msgInfo, sData, &nLength) == SUCCESS)
		{
			SetBit(sSecBitMap, i-63);
			k++;
		}
	}

	c = 0 ;
	if ( k>0 ) 
	{
		c = 16;
		memset (sSecDest,    0, sizeof (sSecDest)); 
		HexToAscii(sSecBitMap, sSecDest, 16);

		memcpy( buffer_snd+4 + 16, sSecDest, 16);
	}
	else
	{
		/* B24 exige le secondary bitmap même vide  */
		c = 16;
		memset (sSecDest,    0, sizeof (sSecDest)); 
		HexToAscii(sSecBitMap, sSecDest, 16);
		SetBit(msgInfo->sBitMap, 1);
		memset( buffer_snd+4 + 16, '0', 16);
	}
	memcpy( buffer_snd + 4  +16+c, msgInfo->sData, msgInfo->nLength);

	return( 4 +16+c + msgInfo->nLength);
}

/*???*/
void BicBuildHeader(int             nReqResp,
					TSBicHeader*   pNewHeader,
					TSBicHeader*   pIncomingHeader,
					char*           pBicProduct)
{
	if (REQUEST == nReqResp)
	{
		memset( (char*)pNewHeader, 0, sizeof(TSBicHeader));
		memcpy(pNewHeader->bicStart          , "ISO",3);
		memcpy(pNewHeader->bicProduct        , pBicProduct,2);
		memcpy(pNewHeader->bicVersion        , "60" ,2);
		memcpy(pNewHeader->bicStatus         , "000",3);
		/* NAB01092014  suite à la demande de la SMT
		pNewHeader->bicOriginatorCode  = '7';
		pNewHeader->bicResponderCode   = '7';  */
		pNewHeader->bicOriginatorCode  = '6';
		pNewHeader->bicResponderCode   = '0';  
		/* pNewHeader->bicSimFlag   = '0';  / NAB21082014 pour simulateur */
	}
	else /* This is a response */
	{
		memcpy((char*)pNewHeader, (char*)pIncomingHeader, sizeof(TSBicHeader));
		memcpy(pNewHeader->bicStatus         , "000",3); 
		pNewHeader->bicResponderCode   = '7';
		/* pNewHeader->bicSimFlag   = '0';  / NAB21082014 pour simulateur */
	}
}


void BicHeaderToBuffer ( TSBicInfo *BicInfo , char *BicHeaderBuffer )
{
	memcpy ( BicHeaderBuffer  ,   BicInfo->sHeader.bicStart        , 3 );
	memcpy ( BicHeaderBuffer+3,   BicInfo->sHeader.bicProduct      , 2 );
	memcpy ( BicHeaderBuffer+5,   BicInfo->sHeader.bicVersion      , 2 );
	memcpy ( BicHeaderBuffer+7,   BicInfo->sHeader.bicStatus       , 3 );
	BicHeaderBuffer [ 10 ] =   BicInfo->sHeader.bicOriginatorCode;
	BicHeaderBuffer [ 11 ] =   BicInfo->sHeader.bicResponderCode;
}
/**Start RHA Icc Mng**/
int GetBicIcTag (char  *tag_name, TSTagBic *tagInfo, char *data, int *length)
{
	int nIndice ;
	int nLenTag ;
	int nDigits;
	char  sLine[MAX_LINE_TRC];

	if ( (nIndice = GetBicTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
		return(ECHEC);

	if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]) & 0x1F) == 0x1F)
		nLenTag = 2;
	else
		nLenTag = 1;
	*length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag ));


	switch (GetBicTagType(nIndice) )
	{
	case BIC_TAG_FIX_BIN :
	case BIC_TAG_FIX_ALPHA :
		memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
			*length);
		break;

	case BIC_TAG_VAR_HEX :
		nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] +
			nLenTag+1,
			data,
			(*length)*2);
		*length = (*length)*2;
		break;
	case BIC_TAG_VAR_BCD :
		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] +
			nLenTag+1,
			data,
			(*length)*2);
		*length = (*length)*2;
		break;

	case BIC_TAG_FIX_BCD :
		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] +
			nLenTag+1,
			data,
			(*length)*2);
		*length = (*length)*2;
		break;

	case BIC_TAG_VAR_BIN :
	case BIC_TAG_VAR_ALPHA :
		memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
			*length);
		break;

	}
	return(SUCCES);
}

int AddBicIcTag (char  *tag_name, TSTagBic *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;
	char sLine[MAX_LINE_TRC];

	if ( (nIndice = GetBicTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == PRESENT)
		return(ECHEC);

	/*AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

	if ( (*(tagInfo->sChipData + tagInfo->nLength) & 0x1F ) == 0x1F)
	nLenTag = 2;
	else
	nLenTag = 1;*/

	/* sprintf(sLine,"TAG IS %s|%02X|%d",tag_name ,*(tagInfo->sChipData + tagInfo->nLength), nLenTag);
	trace_event(sLine,TRACE);*/
	switch (GetBicTagType(nIndice) )
	{
	case BIC_TAG_FIX_BIN :
	case BIC_TAG_FIX_ALPHA :
		if ( GetBicTagLength(nIndice) != length)
			return(ECHEC);
		memcpy( tagInfo->sChipData + tagInfo->nLength , data, length);

		break;

	case BIC_TAG_VAR_HEX :
		nDigits = AsciiToHex( data,
			tagInfo->sChipData + tagInfo->nLength ,
			length);

		break;

	case BIC_TAG_VAR_BCD :
		nDigits = AsciiToBcd( data,
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
			length);

		break;

	case BIC_TAG_FIX_BCD :
		if ( GetBicTagLength(nIndice) != length)
			return(ECHEC);
		nDigits = AsciiToBcd( data,
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1,
			length);

		break;

	case BIC_TAG_VAR_BIN :
	case BIC_TAG_VAR_ALPHA :
		memcpy( tagInfo->sChipData + tagInfo->nLength , data, length);
		nDigits = length;

		break;

	}

	/**(tagInfo->sChipData + tagInfo->nLength + nLenTag)    = nDigits;*/
	tagInfo->nPosTag [ nIndice ] = tagInfo->nLength;
	tagInfo->nPresent[ nIndice ] = PRESENT;
	tagInfo->nLength +=nDigits ;

	return(SUCCES);
}

int PutBicIcTag (char  *tag_name, TSTagBic *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;
	int nPutPos, nTagPresent = NOT_PRESENT;
	char szLastPart  [ MAX_BIC_CHIP_LEN ];
	int nOldTagLen, i;

	memset (szLastPart, '\0', sizeof(szLastPart));
	if ( (nIndice = GetBicTagIndice(tag_name)) == -1)
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

	switch (GetBicTagType(nIndice) )
	{
	case BIC_TAG_FIX_BIN :
	case BIC_TAG_FIX_ALPHA :
		if ( GetBicTagLength(nIndice) != length)
			return(ECHEC);
		memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
		nDigits = length;
		break;

	case BIC_TAG_VAR_HEX :
		nDigits = AsciiToHex( data,
			tagInfo->sChipData + nPutPos + nLenTag+1,
			length);
		break;

	case BIC_TAG_VAR_BCD :
		nDigits = AsciiToBcd( data,
			tagInfo->sChipData + nPutPos + nLenTag+1,
			length);
		break;

	case BIC_TAG_FIX_BCD :
		if ( GetBicTagLength(nIndice) != length)
			return(ECHEC);
		nDigits = AsciiToBcd( data,
			tagInfo->sChipData + nPutPos + nLenTag+1,
			length);
		break;

	case BIC_TAG_VAR_BIN :
	case BIC_TAG_VAR_ALPHA :
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
		for (i = nIndice + 1; i < MAX_BIC_CHIP_TAG; i++)
		{
			if (tagInfo->nPresent[i] == PRESENT)
				tagInfo->nPosTag[i] += nDigits - nOldTagLen;
		}
	}
	return(SUCCES);
}

int BicBuildIcFld (char * buffer_snd, TSTagBic *tagInfo)
{
	unsigned char sBitMap[2 + 1];
	memset (sBitMap, 0,sizeof(sBitMap));
	buffer_snd[0] = 0x7F;
	buffer_snd[1] = 0xFF;

	memcpy (buffer_snd + 2, tagInfo->sChipData , tagInfo->nLength);

	return(tagInfo->nLength + 2);

}
/** End RHA**/


/* NAB 11042012  */
int str_istr (const char *cs, const char *ct)
{
	int index = -1;

	if (cs != NULL && ct != NULL)
	{
		char *ptr_pos = NULL;

		ptr_pos = strstr (cs, ct);
		if (ptr_pos != NULL)
		{
			index = ptr_pos - cs;
		}
	}
	return index;
}

/**********************************************************************************/
/* Start NAB01122014 Fonction pour éliminer les caractères spéciaux d'une chaine **/
/* Wording	: Chaine à nettoyer 						***/
/* Lenght 	: Longueur de la chaine						***/
/* c		: caractère de remplacemet					***/
/* Return 	: Nombre de caratères remplacés					***/
/**********************************************************************************/
int RegExpression(char * Wording, int Lenght, char c)
{

	int i = 0 , j = 0 ;
	int charval;
	for(i = 0 ; i < Lenght ; i ++)
	{
		charval = Wording [i];
		if (charval < 48) 
		{
			Wording [i] = c ;
			j++;
		}

		if ( (charval > 57) && (charval < 65) ) 
		{
			Wording [i] = c ;
			j++;
		}

		if ( (charval > 90) && (charval < 97) ) 
		{
			Wording [i] = c ;
			j++;
		}

		if (charval > 122) 
		{
			Wording [i] = c ;
			j++;
		}

	}
	return(j);
}

/* End NAB01122014 ***/
