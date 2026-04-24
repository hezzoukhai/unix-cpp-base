#include <stdio.h>
#include <stdlib.h>
#include <mds_define.h>
#include <iso_mds.h>
#include <tools.h>
#include <define.h>
#include <p7_define.h>
#include <p7_sid.h>


int GetMdsField(int field_no, TSMdsInfo * msgInfo, char *data, int *nLength)
{
	int length;
	int nBytes;
	int nFldType, nFldLength;
	char szMdsFldLen[4];	
	char szAscFldLen[4];	

	memset( szMdsFldLen, '\0', 4);	
	memset( szAscFldLen, '\0', 4);	
	if (CheckBit (msgInfo->sBitMap, field_no))
	{
		/* if  ( (msgInfo->nMsgType == 1304) ||
		(msgInfo->nMsgType == 1314) )
		{
		nFldType   =  GetMds93FieldType  (field_no);
		nFldLength =  GetMds93FieldLength(field_no);
		}
		else
		{ */
		nFldType   =  GetMdsFieldType  (field_no);
		nFldLength =  GetMdsFieldLength(field_no);

		/* } */

		switch (nFldType)
		{
		case MDS_LL_BIN :
			memcpy(szMdsFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
			EbcdicToAscii(szMdsFldLen, szAscFldLen, 2);
			length = atoi(szAscFldLen);
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
			*nLength = length;
			break;

		case MDS_LLL_BIN :
			memcpy(szMdsFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
			EbcdicToAscii(szMdsFldLen, szAscFldLen, 3);
			length = atoi(szAscFldLen);
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
			*nLength = length;
			break;

		case MDS_FIX_BIN   :
			length = nFldLength;
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
			data[length] = '\0';
			*nLength = length;
			break;

		case MDS_FIX_ALPHA :
			length = nFldLength;
			EbcdicToAscii( msgInfo->sData + msgInfo->nFieldPos[field_no], 
				data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case MDS_FIX_HEX :                           
			length = nFldLength;
			nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
				data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case MDS_LL_HEX :                           
			memcpy( szMdsFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
			EbcdicToAscii(szMdsFldLen, szAscFldLen, 2);
			length = atoi(szAscFldLen);
			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case MDS_LLL_HEX :                           
			memcpy ( szMdsFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			EbcdicToAscii(szMdsFldLen, szAscFldLen, 3);
			length = atoi(szAscFldLen);
			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case MDS_LL_ALPHA : 
			memcpy ( szMdsFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
			EbcdicToAscii(szMdsFldLen, szAscFldLen, 2);
			length = atoi(szAscFldLen);
			EbcdicToAscii ( msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case MDS_LLL_ALPHA : 
			memcpy ( szMdsFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			EbcdicToAscii(szMdsFldLen, szAscFldLen, 3);
			length = atoi(szAscFldLen);
			EbcdicToAscii ( msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
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

int AddMdsField(int field_no, TSMdsInfo *msgInfo, char *data, int nLength)
{
	int nBytes,i  = 0;
	int nFldType, nFldLength;
	char szMdsFldLen[4];	
	char szEbcFldLen[4];	

	memset( szMdsFldLen, '\0', 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	msgInfo->nFieldPos[field_no] = msgInfo->nLength;
	if  ( (msgInfo->nMsgType == 1304) ||
		(msgInfo->nMsgType == 1314) )
	{
		nFldType   =  GetMds93FieldType  (field_no);
		nFldLength =  GetMds93FieldLength(field_no);
	}
	else
	{
		nFldType   =  GetMdsFieldType  (field_no);
		nFldLength =  GetMdsFieldLength(field_no);
	}

	switch (nFldType)
	{
	case MDS_FIX_ALPHA :
		if ( nFldLength != nLength)
			return(ECHEC);
		AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
		nBytes = nLength;
		break;

	case MDS_LL_ALPHA :
		sprintf ( szMdsFldLen, "%.2d", nLength);	
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 2);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
		msgInfo->nLength += 2;
		AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
		nBytes = nLength;
		break;

	case MDS_LLL_ALPHA :
		sprintf ( szMdsFldLen, "%.3d", nLength);	
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 3);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
		msgInfo->nLength += 3;
		AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
		nBytes = nLength;
		break;

	case MDS_FIX_HEX :
		if ( nFldLength != nLength)
			return(ECHEC);
		nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case MDS_LL_HEX :
		sprintf ( szMdsFldLen, "%.2d", nLength);	
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 2);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
		msgInfo->nLength += 2;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case MDS_LLL_HEX :
		sprintf ( szMdsFldLen, "%.3d", nLength);	
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 3);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
		msgInfo->nLength += 3;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case MDS_FIX_BIN :
		if ( nFldLength != nLength)
			return(ECHEC);
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case MDS_LL_BIN :
		sprintf ( szMdsFldLen, "%.2d", nLength);	
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 2);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
		msgInfo->nLength += 2;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case MDS_LLL_BIN :
		sprintf ( szMdsFldLen, "%.3d", nLength);	
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 3);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
		msgInfo->nLength += 3;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	default :
		return(ECHEC);
	}
	msgInfo->nLength += nBytes;
	/*Start HAL23062020 PLUTONL-1847*/
	for (i=field_no; i< MAX_MDS_FIELDS; i++)
       msgInfo->nFieldPos[i+1] = msgInfo->nLength;
	/*End HAL23062020 PLUTONL-1847*/
	SetBit(msgInfo->sBitMap, field_no+1);
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);
	return(SUCCES);
}

int InsertMdsField(int field_no, TSMdsInfo *msgInfo, char *data, int nLength)
{
	int  nFldType, nFldLength;
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i;
	char szMdsData [999];	
	char szMdsFldLen[4];	
	char szEbcFldLen[4];	

	memset( szMdsFldLen, '\0', 4);	
	memset( szEbcFldLen, '\0', 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	if  ( (msgInfo->nMsgType == 1304) ||
		(msgInfo->nMsgType == 1314) )
	{
		nFldType   =  GetMds93FieldType  (field_no);
		nFldLength =  GetMds93FieldLength(field_no);
	}
	else
	{
		nFldType   =  GetMdsFieldType  (field_no);
		nFldLength =  GetMdsFieldLength(field_no);
	}

	switch (nFldType)
	{
	case MDS_FIX_ALPHA :
		if ( nFldLength != nLength)
			return(ECHEC);
		nBytes = nLength;
		AsciiToEbcdic (data, szMdsData, nBytes);
		break;

	case MDS_LL_ALPHA :
		sprintf ( szMdsFldLen, "%.2d", nLength);	
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 2);
		memcpy( szMdsData,        szEbcFldLen, 2);
		nBytes=2;
		AsciiToEbcdic( data,        szMdsData+2, nLength);
		nBytes += nLength;
		break;

	case MDS_LLL_ALPHA :
		sprintf ( szMdsFldLen, "%.3d", nLength);	
		nBytes = 3;
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 3);
		memcpy( szMdsData, szEbcFldLen, 3);
		AsciiToEbcdic( data,        szMdsData+3, nLength);
		nBytes += nLength;
		break;

	case MDS_FIX_HEX :
		if ( nFldLength != nLength)
			return(ECHEC);
		nBytes = AsciiToHex(data, szMdsData, nLength);
		break;

	case MDS_LL_HEX :
		sprintf ( szMdsFldLen, "%.2d", nLength);	
		nBytes1 = 2;
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 2);
		memcpy( szMdsData, szEbcFldLen, nBytes1);
		nBytes = AsciiToHex( data, szMdsData+2, nLength);
		nBytes += nBytes1;
		break;

	case MDS_LLL_HEX :
		sprintf ( szMdsFldLen, "%.3d", nLength);	
		nBytes1 = 3;
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 3);
		memcpy( szMdsData, szEbcFldLen, 3);
		nBytes = AsciiToHex( data, szMdsData+3, nLength);
		nBytes += nBytes1;
		break;

	case MDS_FIX_BIN :
		if ( nFldLength != nLength)
			return(ECHEC);
		nBytes = nLength;
		memcpy( szMdsData, data, nBytes);
		break;

	case MDS_LL_BIN :
		sprintf ( szMdsFldLen, "%.2d", nLength);	
		nBytes = 2;
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 2);
		memcpy( szMdsData, szEbcFldLen, nBytes);
		memcpy( szMdsData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	case MDS_LLL_BIN :
		sprintf ( szMdsFldLen, "%.3d", nLength);	
		nBytes = 3;
		AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 3);
		memcpy( szMdsData, szEbcFldLen, nBytes);
		memcpy( szMdsData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	default :
		return(ECHEC);
	}
	for (i = MAX_MDS_FIELDS-1; i > field_no; i--)
	{
		memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
			msgInfo->sData+msgInfo->nFieldPos[i],
			msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
		msgInfo->nFieldPos[i+1] += nBytes;
	}
	memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szMdsData, nBytes);
	msgInfo->nFieldPos[field_no+1] += nBytes;
	msgInfo->nLength += nBytes;
	SetBit(msgInfo->sBitMap, field_no+1);
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);
	return(SUCCES);
}

int PutMdsField(int field_no, TSMdsInfo *msgInfo, char *data, int nLength)
{
	int  nFldType, nFldLength;
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i, nOldFldLen ,nOldLen;/*PLUTONL-1847*/
	char szMdsData [999];	
	char szMdsFldLen[4];	
	char szEbcFldLen[4];	
	char szWork    [LG_MAX];/*HAL23062020 PLUTONL-1847*/	

	memset( szMdsFldLen, '\0', 4);	
	memset( szEbcFldLen, '\0', 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
	{
		if  ( (msgInfo->nMsgType == 1304) ||
			(msgInfo->nMsgType == 1314) )
		{
			nFldType   =  GetMds93FieldType  (field_no);
			nFldLength =  GetMds93FieldLength(field_no);
		}
		else
		{
			nFldType   =  GetMdsFieldType  (field_no);
			nFldLength =  GetMdsFieldLength(field_no);
		}

		switch (nFldType)
		{
		case MDS_FIX_ALPHA :
			if ( nFldLength != nLength)
				return(ECHEC);
			nBytes = nLength;
			AsciiToEbcdic (data, szMdsData, nBytes);
			break;

		case MDS_LL_ALPHA :
			sprintf ( szMdsFldLen, "%.2d", nLength);	
			AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 2);
			memcpy( szMdsData,        szEbcFldLen, 2);
			nBytes=2;
			AsciiToEbcdic( data,        szMdsData+2, nLength);
			nBytes += nLength;
			break;

		case MDS_LLL_ALPHA :
			sprintf ( szMdsFldLen, "%.3d", nLength);	
			nBytes = 3;
			AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 3);
			memcpy( szMdsData, szEbcFldLen, 3);
			AsciiToEbcdic( data,        szMdsData+3, nLength);
			nBytes += nLength;
			break;

		case MDS_FIX_HEX :
			if ( nFldLength != nLength)
				return(ECHEC);
			nBytes = AsciiToHex(data, szMdsData, nLength);
			break;

		case MDS_LL_HEX :
			sprintf ( szMdsFldLen, "%.2d", nLength);	
			nBytes1 = 2;
			AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 2);
			memcpy( szMdsData, szEbcFldLen, nBytes1);
			nBytes = AsciiToHex( data, szMdsData+2, nLength);
			nBytes += nBytes1;
			break;

		case MDS_LLL_HEX :
			sprintf ( szMdsFldLen, "%.3d", nLength);	
			nBytes1 = 3;
			AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 3);
			memcpy( szMdsData, szEbcFldLen, 3);
			nBytes = AsciiToHex( data, szMdsData+3, nLength);
			nBytes += nBytes1;
			break;

		case MDS_FIX_BIN :
			if ( nFldLength != nLength)
				return(ECHEC);
			nBytes = nLength;
			memcpy( szMdsData, data, nBytes);
			break;

		case MDS_LL_BIN :
			sprintf ( szMdsFldLen, "%.2d", nLength);	
			nBytes = 2;
			AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 2);
			memcpy( szMdsData, szEbcFldLen, nBytes);
			memcpy( szMdsData+nBytes, data, nLength);
			nBytes += nLength;
			break;

		case MDS_LLL_BIN :
			sprintf ( szMdsFldLen, "%.3d", nLength);	
			nBytes = 3;
			AsciiToEbcdic ( szMdsFldLen, szEbcFldLen, 3);
			memcpy( szMdsData, szEbcFldLen, nBytes);
			memcpy( szMdsData+nBytes, data, nLength);
			nBytes += nLength;
			break;

		default :
			return(ECHEC);
		}

		/*Start HAL23062020 PLUTONL-1847*/
		nOldFldLen = msgInfo->nFieldPos[field_no +1] - msgInfo->nFieldPos[field_no];
		memset(szWork, 0, sizeof(szWork));
		nOldLen =  msgInfo->nLength - msgInfo->nFieldPos[field_no+1];
		memcpy(szWork, msgInfo->sData + msgInfo->nFieldPos[field_no+1], nOldLen);

		for (i = MAX_MDS_FIELDS-1; i > field_no; i--)
		{

		 msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
		}
		memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szMdsData, nBytes);
		msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
		msgInfo->nLength += nBytes-nOldFldLen;
		SetBit(msgInfo->sBitMap, field_no+1);
		memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
		if ( (field_no >64) && (field_no < 128))
			SetBit(msgInfo->sBitMap, 1);
		return(SUCCES);
	}
		   
	else
		return ( InsertMdsField (field_no, msgInfo, data, nLength));

		/*nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
			msgInfo->nFieldPos[field_no];
		for (i = MAX_MDS_FIELDS-1; i > field_no; i--)
		{
			memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
				msgInfo->sData+msgInfo->nFieldPos[i],
				msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
			msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
		}
		memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szMdsData, nBytes);
		msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
		msgInfo->nLength += nBytes-nOldFldLen;
		SetBit(msgInfo->sBitMap, field_no+1);
		if ( (field_no >64) && (field_no < 128))
			SetBit(msgInfo->sBitMap, 1);
		return(SUCCES);
	}
	else
		return (InsertMdsField(field_no, msgInfo, data, nLength));*/
	/*End HAL23062020 PLUTONL-1847*/

}

int MdsBuildMsg (char * buffer_snd, TSMdsInfo *msgInfo)
{
	char szMsgType[5];
	int  nBytes;
	int  iMapCtr;
	char        sLine[LG_MAX];/**INSURE**/


	/*** MKB-100806 ***/
	memset(szMsgType, '0', sizeof(szMsgType));
	sprintf(szMsgType, "%.4d", msgInfo->nMsgType);
	/*****/

	/*sprintf(sLine," My trace !! szMsgType (%s) ",szMsgType);*/
	/*trace_event(sLine,TRACE);*/

	AsciiToEbcdic( szMsgType , buffer_snd , 4);
	/*memcpy(buffer_snd,szMsgType,4);*/

	/*sprintf(sLine," My trace !! my buffer (%s) ",buffer_snd);
	trace_event(sLine,TRACE);*/
	iMapCtr = 1;
	while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
		++iMapCtr;
	memcpy( buffer_snd+4, msgInfo->sBitMap, iMapCtr*8);
	memcpy( buffer_snd+4 + (iMapCtr*8), msgInfo->sData, msgInfo->nLength);
	return( 4 + (iMapCtr*8) + msgInfo->nLength);
}

/****************************************************************************/
/* La longueur d'un TAG dans le champ 55 est toujours code sur un caractere */
/****************************************************************************/
int GetMdsIcTag (char  *tag_name, TSTagMds *tagInfo, char *data, int *length)
{
	int nIndice ;
	int nLenTag ;
	int nDigits;
	char  sLine[MAX_LINE_TRC];
	int nType;

	if ( (nIndice = GetMdsTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
		return(ECHEC);

	if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]) & 0x1F) == 0x1F)
		nLenTag = 2;
	else
		nLenTag = 1;
	
	*length = (unsigned char)(*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag ));
	
	nType = GetMdsTagType(nIndice);	/*PROD00051722*/
	switch (nType )
	{
	case MDS_TAG_FIX_BIN :
	case MDS_TAG_FIX_ALPHA :
		memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
			*length);
		break;

	case MDS_TAG_VAR_HEX :
		nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
			nLenTag+1, 
			data, 
			(*length)*2);
		*length = (*length)*2;
		break;
	case MDS_TAG_VAR_BCD :
		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
			nLenTag+1, 
			data, 
			(*length)*2);
		*length = (*length)*2;
		break;

	case MDS_TAG_FIX_BCD :
		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
			nLenTag+1, 
			data,
			(*length)*2);
		*length = (*length)*2;
		break;

	case MDS_TAG_VAR_BIN :
	case MDS_TAG_VAR_ALPHA :
		memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
			*length);
		break;

	}

	/*PROD00051722: Receiving a binary value for an alpha expected field */
	if( nType != MDS_TAG_FIX_BIN &&
		nType != MDS_TAG_VAR_BIN )
	{
		data[*length]='\0';
		if( strlen(data) != *length )
		{
			return(ECHEC);
		}
	}

	return(SUCCES);
}

int AddMdsIcTag (char  *tag_name, TSTagMds *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;
	char sLine[MAX_LINE_TRC];

	if ( (nIndice = GetMdsTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == PRESENT)
		return(ECHEC);

	AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

	if ( (*(tagInfo->sChipData + tagInfo->nLength) & 0x1F ) == 0x1F)
		nLenTag = 2;
	else 
		nLenTag = 1;

	if(debug_option ==1 ) 
	{   
		sprintf(sLine,"TAG IS %s|%02X|%d",tag_name ,*(tagInfo->sChipData + tagInfo->nLength), nLenTag);
		trace_event(sLine,TRACE);
	}

	switch (GetMdsTagType(nIndice) )
	{
	case MDS_TAG_FIX_BIN :
	case MDS_TAG_FIX_ALPHA :
		if ( GetMdsTagLength(nIndice) != length)
			return(ECHEC);
		memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
		nDigits = length;
		break;

	case MDS_TAG_VAR_HEX :
		nDigits = AsciiToHex( data, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case MDS_TAG_VAR_BCD :
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case MDS_TAG_FIX_BCD :
		if ( GetMdsTagLength(nIndice) != length)
			return(ECHEC);
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case MDS_TAG_VAR_BIN :
	case MDS_TAG_VAR_ALPHA :
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

int PutMdsIcTag (char  *tag_name, TSTagMds *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;
	int nPutPos, nTagPresent = NOT_PRESENT;
	char szLastPart  [ MAX_MDS_CHIP_LEN ];
	int nOldTagLen, i;

	memset (szLastPart, '\0', sizeof(szLastPart));
	if ( (nIndice = GetMdsTagIndice(tag_name)) == -1)
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

	switch (GetMdsTagType(nIndice) )
	{
	case MDS_TAG_FIX_BIN :
	case MDS_TAG_FIX_ALPHA :
		if ( GetMdsTagLength(nIndice) != length)
			return(ECHEC);
		memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
		nDigits = length;
		break;

	case MDS_TAG_VAR_HEX :
		nDigits = AsciiToHex( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case MDS_TAG_VAR_BCD :
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case MDS_TAG_FIX_BCD :
		if ( GetMdsTagLength(nIndice) != length)
			return(ECHEC);
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case MDS_TAG_VAR_BIN :
	case MDS_TAG_VAR_ALPHA :
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
		for (i = nIndice + 1; i < MAX_MDS_CHIP_TAG; i++)
		{
			if (tagInfo->nPresent[i] == PRESENT)
				tagInfo->nPosTag[i] += nDigits - nOldTagLen;
		}
	}
	return(SUCCES);
}

int MdsBuildIcFld (char * buffer_snd, TSTagMds *tagInfo)
{
	memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
	return(tagInfo->nLength);
}

/****************************************************************************/
/* Le champ 48                                                              */
/****************************************************************************/
int GetMdsPds (char  *pds_name, TSPdsMds *pdsInfo, char *data, int *length)
{
	int nIndice;
	int nPosLen; /* Position de la longueur */
	int nPosPds; /* Position du PDS         */
	int nPdsLen; /* Longueur du PDS et de la longueur */
	int nPdsLength;
	int nPdsType;
	char szAscPdsLen [4];
	char szEbcPdsLen [4];
	int i;

	memset (szAscPdsLen, '\0', 4);

	if ( (pdsInfo->nMsgType == 1304) ||
		(pdsInfo->nMsgType == 1314) )
	{
		nPosLen = 3;
		nPosPds = 0;
		nPdsLen = 3;
		if ( (nIndice = GetMdsPds93Indice(pds_name)) == -1)
			return(ECHEC);
		nPdsType    = GetMdsPds93Type(nIndice) ;
		nPdsLength  = GetMdsPds93Length(nIndice); 
		/* EBE140523 sprintf ( szAscPdsLen, "%.3d", (int)length);*/
		sprintf ( szAscPdsLen, "%.3d", (int)(*length));
	}
	else
	{
		nPosLen = 0;
		nPosPds = 2;
		nPdsLen = 2;
		if ( (nIndice = GetMdsPdsIndice(pds_name)) == -1)
			return(ECHEC);
		nPdsType    = GetMdsPdsType(nIndice) ;
		nPdsLength  = GetMdsPdsLength(nIndice); 
		/*sprintf ( szAscPdsLen, "%.2d", length+2);*/ /* Mehdi Elyajizi */
	}

	if (pdsInfo->nPresent[nIndice] == NOT_PRESENT)
		return(ECHEC);

	/***Begin Mehdi Elyajizi

	memset ( szAscPdsLen , 0, sizeof ( szAscPdsLen ));
	EbcdicToAscii ( pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice]+nPosLen,
	szAscPdsLen, 
	nPdsLen);
	*length = atoi(szAscPdsLen) - nPosPds;

	switch ( nPdsType )
	{
	case MDS_PDS_FIX_ALPHA :
	EbcdicToAscii(  pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] + 
	nPdsLen + nPdsLen, 
	data , 
	*length);
	break;

	case MDS_PDS_VAR_ALPHA :
	EbcdicToAscii( pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] +
	nPdsLen + nPdsLen, 
	data ,
	*length);
	break;

	}
	Mehdi Elyajizi ***/

	/**length = nPdsLength;
	memcpy( data , pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] + 
		nPdsLen + nPdsLen,
		*length);*/
	
	memset ( szAscPdsLen , 0, sizeof ( szAscPdsLen ));

        memcpy ( szAscPdsLen, pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice]+nPosPds, nPdsLen);

        *length = atoi(szAscPdsLen) ;
	memcpy ( data, pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice]+nPosPds+nPdsLen, *length );
	data[*length] = '\0';

	return(SUCCES);
}
/* Mehdi Elyajizi Enh 72 PDS management */
int AddMdsPds (char  *pds_name, TSPdsMds *pdsInfo, char *data, int length)
{
	int i;
	int nIndice;
	int nPosLen; /* Position de la longueur */
	int nPosPds; /* Position du PDS         */
	int nPdsLen; /* Longueur du PDS et de la longueur */
	int nPdsLength;
	char szAscPdsLen [4];
	int nPdsType;
	char sLine[LG_MAX + 1];

	memset (szAscPdsLen, '\0', 4);
	memset(sLine, 0, sizeof(sLine));

	if ( (pdsInfo->nMsgType == 1304) ||
		(pdsInfo->nMsgType == 1314) )
	{
		nPosLen = 3;
		nPosPds = 0;
		nPdsLen = 3;
		if ( (nIndice = GetMdsPds93Indice(pds_name)) == -1)
			return(ECHEC);
		nPdsType    = GetMdsPds93Type(nIndice) ;
		nPdsLength  = GetMdsPds93Length(nIndice); 
		sprintf ( szAscPdsLen, "%.3d", length);
	}
	else
	{
		nPosLen = 0;
		nPosPds = 2;
		nPdsLen = 2;
		if ( (nIndice = GetMdsPdsIndice(pds_name)) == -1)
			return(ECHEC);
		nPdsType    = GetMdsPdsType(nIndice) ;
		nPdsLength  = GetMdsPdsLength(nIndice); 
		/* sprintf ( szAscPdsLen, "%.2d", length+2);*/
		sprintf ( szAscPdsLen, "%.2d", length);
	}
	if (pdsInfo->nPresent[nIndice] == PRESENT)
	{
		sprintf(sLine, "==> AddMDSsPds(ERROR. Tag [%.2s] Already PRESENT)", pds_name);
		trace_event(sLine, FATAL);
		return(ECHEC);
	}

	sprintf(szAscPdsLen,"%02d", length);


	memcpy(pdsInfo->sPdsData + pdsInfo->nLength , pds_name, 2);
	memcpy(pdsInfo->sPdsData + pdsInfo->nLength + nPosPds, szAscPdsLen, 2);
	memcpy(pdsInfo->sPdsData + pdsInfo->nLength + nPosPds + nPdsLen, data, length);

	pdsInfo->nPosPds [ nIndice ] = pdsInfo->nLength;
	pdsInfo->nPresent[ nIndice ] = PRESENT;
	pdsInfo->nLength += nPosPds + nPdsLen + length;

	return(SUCCES);
}
/* Mehdi Elyajizi*/

int AddMdsPds_old_mehdi (char  *pds_name, TSPdsMds *pdsInfo, char *data, int length)
{
	int i;
	int nIndice;
	int nPosLen; /* Position de la longueur */
	int nPosPds; /* Position du PDS         */
	int nPdsLen; /* Longueur du PDS et de la longueur */
	int nPdsLength;
	char szAscPdsLen [4];
	int nPdsType;

	memset (szAscPdsLen, '\0', 4);

	if ( (pdsInfo->nMsgType == 1304) ||
		(pdsInfo->nMsgType == 1314) )
	{
		nPosLen = 3;
		nPosPds = 0;
		nPdsLen = 3;
		if ( (nIndice = GetMdsPds93Indice(pds_name)) == -1)
			return(ECHEC);
		nPdsType    = GetMdsPds93Type(nIndice) ;
		nPdsLength  = GetMdsPds93Length(nIndice); 
		sprintf ( szAscPdsLen, "%.3d", length);
	}
	else
	{
		nPosLen = 0;
		nPosPds = 2;
		nPdsLen = 2;
		if ( (nIndice = GetMdsPdsIndice(pds_name)) == -1)
			return(ECHEC);
		nPdsType    = GetMdsPdsType(nIndice) ;
		nPdsLength  = GetMdsPdsLength(nIndice); 
		sprintf ( szAscPdsLen, "%.2d", length+2);
	}

	if (pdsInfo->nPresent[nIndice] == PRESENT)
		return(ECHEC);

	/****
	for ( i = 0 ; i < length ; i ++ ) 
	(pdsInfo->sPdsData + pdsInfo->nLength +(2*nPdsLen))[i] = data[i];
	***/

	switch ( nPdsType )
	{
	case MDS_PDS_FIX_ALPHA :
		if ( nPdsLength != length)
			return(ECHEC);

		AsciiToEbcdic( data, 
			pdsInfo->sPdsData + pdsInfo->nLength +(2*nPdsLen), 
			length);
		break;

	case MDS_PDS_VAR_ALPHA :
		AsciiToEbcdic( data, 
			pdsInfo->sPdsData + pdsInfo->nLength +(2*nPdsLen), 
			length);
		break;

	}
	AsciiToEbcdic ( szAscPdsLen,
		pdsInfo->sPdsData + pdsInfo->nLength + nPosLen,
		nPdsLen 
		);

	AsciiToEbcdic ( pds_name,
		pdsInfo->sPdsData + pdsInfo->nLength + nPosPds,
		nPdsLen 
		);


	pdsInfo->nPosPds [ nIndice ] = pdsInfo->nLength;
	pdsInfo->nPresent[ nIndice ] = PRESENT;
	pdsInfo->nLength +=length + nPdsLen + nPdsLen;
	return(SUCCES);
}

int PutMdsPds (char  *pds_name, TSPdsMds *pdsInfo, char *data, int length)
{
	int nIndice;
	int nPosLen; /* Position de la longueur */
	int nPosPds; /* Position du PDS         */
	int nPdsLen; /* Longueur du PDS et de la longueur */
	int nPdsLength;
	char szAscPdsLen [4];
	char szOldPdsLen [4];
	char szLastPart  [ MAX_MDS_PDS_LEN ];
	int nPdsType, i;
	int nPutPos, 
		nOldPdsLen, 
		nPdsPresent = NOT_PRESENT;

	memset (szAscPdsLen, '\0', 4);
	memset (szOldPdsLen, '\0', 4);
	memset (szLastPart, '\0', sizeof (szLastPart));

	if ( (pdsInfo->nMsgType == 1304) ||
		(pdsInfo->nMsgType == 1314) )
	{
		nPosLen = 3;
		nPosPds = 0;
		nPdsLen = 3;
		if ( (nIndice = GetMdsPds93Indice(pds_name)) == -1)
			return(ECHEC);
		nPdsType    = GetMdsPds93Type(nIndice) ;
		nPdsLength  = GetMdsPds93Length(nIndice); 
		sprintf ( szAscPdsLen, "%.3d", length);
	}
	else
	{
		nPosLen = 0;
		nPosPds = 2;
		nPdsLen = 2;
		if ( (nIndice = GetMdsPdsIndice(pds_name)) == -1)
			return(ECHEC);
		nPdsType    = GetMdsPdsType(nIndice) ;
		nPdsLength  = GetMdsPdsLength(nIndice); 
		sprintf ( szAscPdsLen, "%.2d", length+2);
	}

	/* Start PLUTONL-5812 AN6022  Enh 2023Q2  FZL20230202*/
	if (pdsInfo->nPresent[nIndice] == PRESENT)
	{
		nPutPos = pdsInfo->nPosPds[nIndice];
		memcpy (szOldPdsLen,
			pdsInfo->sPdsData + pdsInfo->nPosPds [nIndice] + nPosPds,
			nPdsLen);
		nOldPdsLen = atoi(szOldPdsLen) ;
		memcpy(szLastPart, 
			pdsInfo->sPdsData + nPutPos + nPosPds + nPdsLen + nOldPdsLen,
			pdsInfo->nLength - nPutPos - nPosPds - nPdsLen - nOldPdsLen);
	}
	else
		nPutPos = pdsInfo->nLength;

	sprintf(szAscPdsLen, "%02d", length);
	memcpy(pdsInfo->sPdsData + nPutPos, pds_name, nPosPds);
	memcpy(pdsInfo->sPdsData + nPutPos + nPosPds , szAscPdsLen, nPdsLen);
	memcpy(pdsInfo->sPdsData + nPutPos + nPosPds + nPdsLen, data, length);


	if (nPdsPresent == NOT_PRESENT )
	{

		pdsInfo->nPosPds [ nIndice ] = pdsInfo->nLength;
		pdsInfo->nPresent[ nIndice ] = PRESENT;
		pdsInfo->nLength +=length + nPdsLen + nPdsLen;
	}
	else
	{
		memcpy (pdsInfo->sPdsData + nPutPos+ nPosPds+ nPdsLen + length,
			szLastPart,
			pdsInfo->nLength - nPutPos - nPosLen - nPdsLen - nOldPdsLen);

		pdsInfo->nLength +=length - nOldPdsLen;
		for ( i = nIndice+1; i < MAX_MDS_PDS; i++)
		{
			if ( pdsInfo->nPresent[i] == PRESENT)
				pdsInfo->nPosPds[i] += length - nOldPdsLen;
		}
	}
	/* End PLUTONL-5812 AN6022  Enh 2023Q2  FZL20230202*/
	return(SUCCES);
}

int MdsBuildPds (char * sPdsBuffer, TSPdsMds *pdsInfo)
{
	/*int i;
	fprintf ( stderr,"\n Data Avant le Build : ");
	for ( i = 0 ; i < pdsInfo->nLength ; i ++ )
		fprintf ( stderr,"%02X ", pdsInfo->sPdsData[i] );

	for ( i = 0 ; i <4 ; i++ ) sPdsBuffer [ i ] = 0x40;
	memcpy ( sPdsBuffer + 4 , pdsInfo->sPdsData , pdsInfo->nLength);

	return(pdsInfo->nLength + 4);*/
	
	memcpy ( sPdsBuffer , pdsInfo->sPdsData , pdsInfo->nLength);
	return(pdsInfo->nLength );
}


/**************************************************************************/
/*   MdsToIsoPosMode							*/
/*                                                                        */
/*   Converts MDS POS Entry Mode data (field 22) into an ISO format code */
/*   Input:  MDS fields 18, 22, 25, & 60                                 */
/*   Output: ISO field 22                                                 */
/**************************************************************************/
void   MdsToIsoPosMode (char  *Merchant,
	char  *MdsPosMode,
	char  *MdsPosCond,
	char  *MdsAddPos,
	char  *IsoPosMode)
{

	trace_event("Start MdsToIsoPosMode()", PROCESSING);

	/* Card Data Entry Capability - Position 1 */
	/*if (memcmp(MdsAddPos + 1, "0", 1) == 0) memcpy (IsoPosMode,  "0", 1);
	else
		if (memcmp(MdsAddPos + 1, "1", 1) == 0) memcpy (IsoPosMode , "1" , 1 );
		else
			if (memcmp(MdsAddPos + 1, "2", 1) == 0) memcpy (IsoPosMode , "2" , 1 );
			else
				if (memcmp(MdsAddPos + 1, "3", 1) == 0) memcpy (IsoPosMode , "3" , 1 );
				else
					if (memcmp(MdsAddPos + 1, "4", 1) == 0) memcpy (IsoPosMode , "4" , 1 );
					else
						if (memcmp(MdsAddPos + 1, "5", 1) == 0) memcpy (IsoPosMode , "9" , 1 );
						else
							if (memcmp(MdsAddPos + 1, "9", 1) == 0) memcpy (IsoPosMode , "1" , 1 );
							else
								memcpy (IsoPosMode , "0" , 1 );*/
	
	if (memcmp(MdsAddPos + 10, "0", 1) == 0) memcpy (IsoPosMode,  "0", 1);
	else if (memcmp(MdsAddPos + 10, "1", 1) == 0) memcpy (IsoPosMode , "1" , 1 );
	else if (memcmp(MdsAddPos + 10, "2", 1) == 0) memcpy (IsoPosMode , "2" , 1 );
	else if (memcmp(MdsAddPos + 10, "3", 1) == 0) memcpy (IsoPosMode , "M" , 1 );
	else if (memcmp(MdsAddPos + 10, "4", 1) == 0) memcpy (IsoPosMode , "A" , 1 );
	else if (memcmp(MdsAddPos + 10, "5", 1) == 0) memcpy (IsoPosMode , "5" , 1 );
	else if (memcmp(MdsAddPos + 10, "6", 1) == 0) memcpy (IsoPosMode , "6" , 1 );
	else if (memcmp(MdsAddPos + 10, "7", 1) == 0) memcpy (IsoPosMode , "7" , 1 );
	else if (memcmp(MdsAddPos + 10, "8", 1) == 0) memcpy (IsoPosMode , "8" , 1 );
	else if (memcmp(MdsAddPos + 10, "9", 1) == 0) memcpy (IsoPosMode , "9" , 1 );

	/* Cardholder Authentication Capability - Position 2 */
	/*if (memcmp(MdsPosCond, "57", 1) == 0) memcpy (( IsoPosMode + 1) , "1" , 1);
	else
		memcpy ((IsoPosMode + 1) , "0" , 1);*/

	if (memcmp(MdsPosCond + 2, "2", 1) == 0) memcpy (( IsoPosMode + 1) , "0" , 1);
   	else if (memcmp(MdsPosCond + 2, "1", 1) == 0) memcpy (( IsoPosMode + 1) , "1" , 1);
   	else if (memcmp(MdsPosCond + 2, "0", 1) == 0) memcpy (( IsoPosMode + 1) , "0" , 1);
   	else if (memcmp(MdsPosCond + 2, "8", 1) == 0) memcpy (( IsoPosMode + 1) , "0" , 1);
   	else
		memcpy (( IsoPosMode + 1) , "0" , 1);

	/* Card Retention capability - Position 3 */
	/*if (memcmp(Merchant, "6011", 4) == 0) memcpy (( IsoPosMode + 2) , "1" , 1);
	else
		memcpy ((IsoPosMode + 2) , "0" , 1);*/

	IsoPosMode[2] = MdsAddPos[5];

	/* Operating Environment - Position 4 */
	/*memcpy  ((IsoPosMode + 3) , "0" , 1);*/
	
	if (memcmp(MdsAddPos + 2, "3", 1) == 0) 		memcpy (IsoPosMode + 3, "0" , 1 );
   	else if (memcmp(MdsAddPos + 2, "0", 1) == 0) 	memcpy (IsoPosMode + 3, "1" , 1 );
   	else if (memcmp(MdsAddPos + 2, "1", 1) == 0)   memcpy (IsoPosMode + 3, "3" , 1 );
   	else if (memcmp(MdsAddPos + 2, "2", 1) == 0)   memcpy (IsoPosMode + 3, "3" , 1 );
   	else memcpy  ((IsoPosMode + 3) , "9" , 1);

	/* Cardholder Presence Indicator - Position 5 */
	/*if (memcmp(MdsPosCond, "01", 2) == 0) memcpy (( IsoPosMode + 4) , "1" , 1);
	else
		if (memcmp(MdsPosCond, "05", 2) == 0) memcpy (( IsoPosMode + 4) , "0" , 1);
		else
			if (memcmp(MdsPosCond, "08", 2) == 0) memcpy (( IsoPosMode + 4) , "2" , 1);
			else
				if (memcmp(MdsPosCond, "51", 2) == 0) memcpy (( IsoPosMode + 4) , "4" , 1);
				else
					if (memcmp(MdsPosCond, "55", 2) == 0) memcpy (( IsoPosMode + 4) , "3" , 1);
					else
						memcpy ((IsoPosMode + 4) , "0" , 1);*/
					
	if (memcmp(MdsAddPos  + 3 , "0", 1) == 0)  	 memcpy((IsoPosMode + 4), "0", 1); /** Cardholder Present                   **/
	else if (memcmp(MdsAddPos  + 3 , "1", 1) == 0)  memcpy((IsoPosMode + 4), "1", 1); /** Cardholder Not Present, UNspecified  **/
	else if (memcmp(MdsAddPos  + 3 , "2", 1) == 0)  memcpy((IsoPosMode + 4), "2", 1); /** Cardholder Not Present, Mail/Facsimile order                 **/
	else if (memcmp(MdsAddPos  + 3 , "3", 1) == 0)  memcpy((IsoPosMode + 4), "3", 1); /** Cardholder Not Present, Phone ARU order                      **/
	else if (memcmp(MdsAddPos  + 3 , "4", 1) == 0)  memcpy((IsoPosMode + 4), "4", 1); /** Cardholder Not Present, standing order / recuring order      **/
	else if (memcmp(MdsAddPos  + 3 , "5", 1) == 0)  memcpy((IsoPosMode + 4), "5", 1); /** Cardholder Not Present, Electronic Order (Home PC, Internet , mobile Pda)**/
	else  memcpy((IsoPosMode + 4), "0", 1);

	/* Card Presence Indicator - Position 6 */
	/*if (memcmp(MdsPosCond, "00", 1) == 0) memcpy (( IsoPosMode + 5) , "1" , 1);
	else
		if (memcmp(MdsPosCond, "02", 1) == 0) memcpy (( IsoPosMode + 5) , "1" , 1);
		else
			if (memcmp(MdsPosCond, "03", 1) == 0) memcpy (( IsoPosMode + 5) , "1" , 1);
			else
				if (memcmp(MdsPosCond, "10", 1) == 0) memcpy (( IsoPosMode + 5) , "1" , 1);
				else
					if (memcmp(MdsPosCond, "57", 1) == 0) memcpy (( IsoPosMode + 5) , "1" , 1);
					else
						if (memcmp(MdsPosCond, "71", 1) == 0) memcpy (( IsoPosMode + 5) , "1" , 1);
						else
							memcpy ((IsoPosMode + 5) , "0" , 1 );*/
	
	if (memcmp(MdsAddPos + 4, "1", 1) == 0) 
		memcpy (( IsoPosMode + 5) , "0" , 1);
   	else if (memcmp(MdsAddPos + 4, "0", 1) == 0) 
		memcpy (( IsoPosMode + 5) , "1" , 1);
   	else
		memcpy ((IsoPosMode + 5) , "1" , 1 );

	/* Card Data Input Mode - Position 7 */
	if (memcmp(MdsPosMode, "00", 2) == 0) 
		memcpy (( IsoPosMode + 6) , "0" , 1);
	else if (memcmp(MdsPosMode, "01", 2) == 0 || memcmp(MdsPosMode, "79", 2) == 0) memcpy (( IsoPosMode + 6) , "1" , 1);
	else if (memcmp(MdsPosMode, "02", 2) == 0) memcpy (( IsoPosMode + 6) , "2" , 1);
	else if (memcmp(MdsPosMode, "03", 2) == 0) memcpy (( IsoPosMode + 6) , "3" , 1);
	else if (memcmp(MdsPosMode, "04", 2) == 0) memcpy (( IsoPosMode + 6) , "4" , 1);
	else if (memcmp(MdsPosMode, "05", 2) == 0) memcpy (( IsoPosMode + 6) , "5" , 1);
	/*else if (memcmp(MdsPosMode, "06", 2) == 0) memcpy (( IsoPosMode + 6) , "7" , 1);*/ /*AMER20190830 PROD00071322: This value is no more used*/
	else if (memcmp(MdsPosMode, "07", 2) == 0) memcpy (( IsoPosMode + 6) , "M" , 1);
	else if (memcmp(MdsPosMode, "10", 2) == 0) memcpy (( IsoPosMode + 6) , "F" , 1);
	else if (memcmp(MdsPosMode, "80", 2) == 0) memcpy (( IsoPosMode + 6) , "8" , 1);
	else if (memcmp(MdsPosMode, "81", 2) == 0) memcpy (( IsoPosMode + 6) , "9" , 1);
	else if (memcmp(MdsPosMode, "82", 2) == 0) memcpy (( IsoPosMode + 6) , "X" , 1);
	else if (memcmp(MdsPosMode, "90", 2) == 0) memcpy (( IsoPosMode + 6) , /*"A"*/ "2" , 1); /*NAB04022020*/
	else if (memcmp(MdsPosMode, "91", 2) == 0) memcpy (( IsoPosMode + 6) , "A" , 1);
	else if (memcmp(MdsPosMode, "95", 2) == 0) memcpy (( IsoPosMode + 6) , "J" , 1);
	else memcpy ((IsoPosMode + 6) , "0" , 1);

	/* Cardholder Authentication Method - Position 8 */
	memcpy ((IsoPosMode + 7) , "0" , 1);

	/* Cardholder Authentication Entity - Position 9 */
	memcpy ((IsoPosMode + 8) , "0" , 1);

	/* Rewrite capability - Position 10 */
	memcpy ((IsoPosMode + 9) , "0" , 1);

	/* Terminal Output capability - Position 11 */
	memcpy ((IsoPosMode + 10) , "0" , 1);

	/* PIN Entry capability - Position 12 */
	/*ZKO:start*/
	/*if (memcmp(MdsPosMode + 2, "2", 2) == 0) memcpy ((IsoPosMode + 11) ,"0", 1);
	else
		if (memcmp(MdsPosMode + 2, "8", 2) == 0) memcpy ((IsoPosMode + 11) ,"S", 1);
		else
			memcpy ((IsoPosMode + 11) , "1" , 1);

        memcpy ((IsoPosMode + 11) , "1" , 1);
	*/
	

	switch(MdsPosMode[2])
	{
	case MDS_F022_P2_NO_PIN_ENTRY:	IsoPosMode[11]=P7_F022_P12_NOPIN_CAPTURE;break;
	case MDS_F022_P2_HAS_PE_NOOP:	IsoPosMode[11]=P7_F022_P12_PINPAD_DOWN;break;
	case MDS_F022_P2_HAS_PIN_ENTRY: IsoPosMode[11]=P7_F022_P12_12_DIG;break;	
	case MDS_F022_P2_MPOS_SOFT_PIN: IsoPosMode[11]=P7_F022_P12_MPOS_SOFT_PIN;break;
	default: IsoPosMode[11]=P7_F022_P12_UNKOWN;break;
	}


	/*ZKO:end*/

	


	trace_event("End   MdsToIsoPosMode(OK)", PROCESSING);

	return;
}



/*************************************************************************/
/*   MdsToIsoPosData()                                                  */
/*************************************************************************/
void MdsToIsoPosData (  char *MdsPosMode ,
	char *MdsPosData ,
	char *PreAuthIndicator ,
	char *PreAuthValue ,
	char *IsoPosData ,
	TSMdsInfo  *MdsInfo )
{

	char    sLine [ MAX_LINE_TRC ];
	char           T_Data[77];
	char          tmpField[256];
	int           tmpFieldLen;
	TSPdsMds     sPdsInfo;


	trace_event("Start MdsToIsoPosData()", PROCESSING);

	sprintf  ( sLine , "MdsPosData = [%s] ", MdsPosData );
	trace_event ( sLine , PROCESSING );

	memcpy ( IsoPosData , "000000000000" , 12 );

	/*-----------------------------------------------------------------------*/
	/* SubField 1 : Card Data Entry  Capability                              */
	/*-----------------------------------------------------------------------*/
	switch (MdsPosData[10])
	{
	case   '0'  : IsoPosData[0] = '0';           break;
	case   '1'  : IsoPosData[0] = '1';           break;
	case   '2'  : IsoPosData[0] = '2';           break;
	case   '3'  : IsoPosData[0] = '3';           break;
	case   '4'  : IsoPosData[0] = '4';           break;
	case   '5'  : IsoPosData[0] = '5';           break;
	case   '6'  : IsoPosData[0] = '6';           break;
	case   '7'  : IsoPosData[0] = '7';           break;
	case   '8'  : IsoPosData[0] = '8';           break;
	case   '9'  : IsoPosData[0] = '9';           break;
	default     : IsoPosData[0] = '0';           break;
	}
	
	/*-----------------------------------------------------------------------*/
	/* SubField 2 : Cardholder Auth Capabilities                             */
	/*-----------------------------------------------------------------------*/
	if ( memcmp ( MdsPosMode + 2 ,  "1" , 2 ) == 0 )
		IsoPosData[1] = '1';
	else
		if ( memcmp ( MdsPosMode + 2 ,  "2" , 2 ) == 0 )
			IsoPosData[1] = '2';
		else
			IsoPosData[1] = '0';

	
	/*-----------------------------------------------------------------------*/
	/* SubField 3 : Card Retention Capability                                */
	/*-----------------------------------------------------------------------*/
	switch (MdsPosData[5])
	{
	case   '0'  : IsoPosData[2] = '0';           break;
	case   '1'  : IsoPosData[2] = '1';           break;
	default     : IsoPosData[2] = '0';           break;
	}
	
	/*-----------------------------------------------------------------------*/
	/* SubField 4 : Operating Environnement                                  */
	/*-----------------------------------------------------------------------*/
	if ( MdsPosData[9] == '0' )
	{
		switch (MdsPosData[0]) /* <== Operating Environnement */
		{
		case   '0'  :
			if ( MdsPosData [2] == '0' )         IsoPosData[3] = '1';
			else if ( MdsPosData [2] == '1' )   IsoPosData[3] = '3';
			else                                 IsoPosData[3] = '9';
			break;

		case   '1'  :
			if ( MdsPosData [2] == '0' )      IsoPosData[3] = '2';
			else if ( MdsPosData [2] == '1' ) IsoPosData[3] = '4';
			else if ( MdsPosData [2] == '2' ) IsoPosData[3] = '5';
			else                               IsoPosData[3] = '9';
			break; /*IBO20230217 PLUTONL-5917*/

		default    :                                       IsoPosData[3] = '9';
			break;
		}
	}
	else
	{
		switch ( MdsPosData[9] )
		{
		case   '1'  : IsoPosData[3] = 'S';           break;
		case   '2'  : IsoPosData[3] = 'T';           break;
		case   '3'  : IsoPosData[3] = 'U';           break;
		case   '4'  : IsoPosData[3] = 'V';           break;
		default     : IsoPosData[3] = '9';           break;
		}
	}
	
	/*-----------------------------------------------------------------------*/
	/* SubField 5 : Cardholder Presence   Indicator                          */
	/*-----------------------------------------------------------------------*/
	switch (MdsPosData[3])
	{
	case   '0'  : IsoPosData[4] = '0';           break;
	case   '1'  : IsoPosData[4] = '1';           break;
	case   '2'  : IsoPosData[4] = '2';           break;
	case   '3'  : IsoPosData[4] = '3';           break;
	case   '4'  : IsoPosData[4] = '4';           break;
	case   '5'  : IsoPosData[4] = '5';           break;
	default     : IsoPosData[4] = '1';           break;
	}
	
	/*-----------------------------------------------------------------------*/
	/* SubField 6 : Card  Presence   Indicator                               */
	/*-----------------------------------------------------------------------*/
	switch (MdsPosData[4])
	{
	case   '0'  : IsoPosData[5] = '1';           break;
	case   '1'  : IsoPosData[5] = '0';           break;
	default     : IsoPosData[5] = '1';           break;
	}
	
	/*-----------------------------------------------------------------------*/
	/* SubField 7 : Card Data Input Mode                                     */
	/*-----------------------------------------------------------------------*/
	if ( memcmp ( MdsPosMode ,  "00" , 2 ) == 0 )
		IsoPosData [ 6 ] = '0';
	else if ( memcmp ( MdsPosMode ,  "01" , 2 ) == 0 )
		IsoPosData [ 6 ] = '1';
	else if ( memcmp ( MdsPosMode ,  "02" , 2 ) == 0 )
		IsoPosData [ 6 ] = '2';
	else if ( memcmp ( MdsPosMode ,  "03" , 2 ) == 0 )
		IsoPosData [ 6 ] = '3';
	else if ( memcmp ( MdsPosMode ,  "04" , 2 ) == 0 )
		IsoPosData [ 6 ] = '4';
	else if ( memcmp ( MdsPosMode ,  "05" , 2 ) == 0 )
		IsoPosData [ 6 ] = '5';
	else if ( memcmp ( MdsPosMode ,  "07" , 2 ) == 0 )	/*/*AMER20210927 PLUTONL-3612*/
		IsoPosData [ 6 ] = P7_F022_P7_PAN_AUTO_ENTRY_CL_ICC;
	else if (memcmp(MdsPosMode, "10", 2) == 0) 		/*AMER20180308 PROD00054205: Enh 18.Q2 AN1103*/	
		IsoPosData[6] = P7_F022_P7_CRED_ON_FILE;
	else if ( memcmp ( MdsPosMode ,  "80" , 2 ) == 0 )
		IsoPosData [ 6 ] = '8';
	else if ( memcmp ( MdsPosMode ,  "90" , 2 ) == 0 )
		IsoPosData [ 6 ] = '2';
	
	if(GetMdsField(MDS_T1_DATA,MdsInfo, T_Data, &tmpFieldLen) ==SUCCES)
		IsoPosData [ 6 ] = '7';
	
	if ( memcmp ( MdsPosMode ,  "81" , 2 ) == 0 )
		IsoPosData [ 6 ] = '9';

	

	/* IBO20230224 PLUTONL-5942
	 *
	 if (GetMdsField (MDS_ADTNL_DATA_PRIV, MdsInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		AnalyseMdsPds (tmpField + 1, tmpFieldLen - 1, MdsInfo->nMsgType , &sPdsInfo);/*IBO20230217 PLUTONL-5917* /
		/*InitMdsPds (&sPdsInfo);* /
		if ( GetMdsPds ( MDS_PDS_SECURITY_IND , &sPdsInfo  , tmpField, &tmpFieldLen ) == SUCCES )
		{
			if ( memcmp ( tmpField , "00" , 2 ) == 0 )
				IsoPosData [ 6 ] = '9';
			else
				if ( memcmp ( tmpField , "05" , 2 ) == 0 )
					IsoPosData [ 6 ] = 'T';
				else
					if ( memcmp ( tmpField , "06" , 2 ) == 0 )
						IsoPosData [ 6 ] = 'S';
					else
						if ( memcmp ( tmpField , "07" , 2 ) == 0 )
							IsoPosData [ 6 ] = 'V';
						else
							if ( memcmp ( tmpField , "08" , 2 ) == 0 )
								IsoPosData [ 6 ] = 'U';
							else
								IsoPosData [ 6 ] = '9';
		}
	}*/
	
	/*-----------------------------------------------------------------------*/
	/* SubField 8 : Cardholder Authentication Method                         */
	/*-----------------------------------------------------------------------*/
	if ( GetMdsField (MDS_PIN_DATA, MdsInfo, tmpField, &tmpFieldLen) == SUCCES)
		IsoPosData [ 7 ] = '1';
	else
		if ( IsoPosData [6] == 'T' )
			IsoPosData [ 7 ] = '2';
		else
			IsoPosData [ 7 ] = '0';
	
	/*-----------------------------------------------------------------------*/
	/* SubField 9 : Cardholder Authentication Entity                         */
	/*-----------------------------------------------------------------------*/
	IsoPosData [ 8 ] = '0';

	/*-----------------------------------------------------------------------*/
	/* SubField 10 : Rewrite Capability                                      */
	/*-----------------------------------------------------------------------*/
	IsoPosData [ 9 ] = '0';

	/*-----------------------------------------------------------------------*/
	/* SubField 11 : Terminal Output Capability                               */
	/*-----------------------------------------------------------------------*/
	IsoPosData [ 10 ] = '0';

	/*-----------------------------------------------------------------------*/
	/* SubField 12 : Pin Entry Capability                                    */
	/*-----------------------------------------------------------------------*/

	/*if ( memcmp ( MdsPosMode + 2 ,  "0" , 1 ) == 0 )
		IsoPosData [ 11 ] = '1';
	else
		if ( memcmp ( MdsPosMode + 2 ,  "2" , 1 ) == 0 )
			IsoPosData [ 11 ] = '0';
		else
			IsoPosData [ 11 ] = '4';
	*/
	/*ZKO:start*/ 
	switch(MdsPosMode[2])
        {
        case MDS_F022_P2_NO_PIN_ENTRY:  IsoPosData[11]=P7_F022_P12_NOPIN_CAPTURE;break;
        case MDS_F022_P2_HAS_PE_NOOP:   IsoPosData[11]=P7_F022_P12_PINPAD_DOWN;break;
        case MDS_F022_P2_HAS_PIN_ENTRY: IsoPosData[11]=P7_F022_P12_12_DIG;break;
        case MDS_F022_P2_MPOS_SOFT_PIN: IsoPosData[11]=P7_F022_P12_MPOS_SOFT_PIN;break;/*ENH:Q18.4*/
        default: IsoPosData[11]=P7_F022_P12_UNKOWN;break;
        }


        /*ZKO:end*/
 

	
	switch (MdsPosData[4])
	{

	case '4'  :
		memcpy ( PreAuthIndicator , "Y"  , 1 );
		memcpy ( PreAuthValue     , MdsPosData + 11 , 2 );
		break;

	default   :
		memcpy ( PreAuthIndicator , "N"  , 1 );
		memcpy ( PreAuthValue     , "00" , 2 );
		break;
	}
	
	
	trace_event("End MdsToIsoPosData(OK)", PROCESSING);
	return;
}


/*************************************************************************/
/*   IsoToMdsPosData()                                                   */
/*************************************************************************/
void  IsoToMdsPosData ( char  *IsoMode,
	char  *MdsPosData,
	char  PreAuthIndicator ,
	char *PreAuthTime ,
	char *TerminalCountryCode )
{

	char    sLine [ MAX_LINE_TRC ];


	trace_event("Start IsoToMdsPosData()", PROCESSING);
	
	sprintf  ( sLine , "IsoMode = %s ", IsoMode );
	trace_event ( sLine , PROCESSING );

	/*-----------------------------------------------------------------------*/
	/* SubField 1 : Pos-terminal attendance Indicator                        */
	/*-----------------------------------------------------------------------*/
	switch (IsoMode[3]) /* <== Operating Environnement */
	{
	case   '0'  : MdsPosData[0] = '2';           break;

	case   '1'  :
	case   '3'  : MdsPosData[0] = '0';           break;
	case   '4'  :    /* MKB 29032015 Certif NIV & MTIP & Online MDS */
		if(IsoMode[6]=='5')
			MdsPosData[0] = '1';           
		else
			MdsPosData[0] = '0';           
		break;

	default     : MdsPosData[0] = '1';           break;
	}

	/*-----------------------------------------------------------------------*/
	/* SubField 2 : RFU                                                      */
	/*-----------------------------------------------------------------------*/
	MdsPosData[1] = '0';

	/*-----------------------------------------------------------------------*/
	/* SubField 3 : Pos-terminal Location   Indicator                        */
	/*-----------------------------------------------------------------------*/
	
	switch (IsoMode[3]) /* <== Operating Environnement */
	{
	case   '9'  :
	case   '0'  : MdsPosData[2] = '3';           break;

	case   '1'  :
	case   '2'  : MdsPosData[2] = '0';           break;

	case   '3'  : MdsPosData[2] = '1';           break; /* MKB 29032015 Certif NIV & MTIP & Online MDS */
	case   '4'  : MdsPosData[2] = '1';           break; /* MKB 29032015 Certif NIV & MTIP & Online MDS */

	case   '5'  : MdsPosData[2] = '2';           break;
	case   'T'  : MdsPosData[2] = '0';           break;

	default     : MdsPosData[2] = '1';           break;
	}
	

	/*-----------------------------------------------------------------------*/
	/* SubField 4 : Pos-Cardholder Presence   Indicator                      */
	/*-----------------------------------------------------------------------*/
	switch (IsoMode[4]) /* <== Cardholder Presence   Indicator            */
	{
	case   '0'  : MdsPosData[3] = '0';           break;
	case   '1'  : MdsPosData[3] = '1';           break;
	case   '2'  : MdsPosData[3] = '2';           break;
	case   '3'  : MdsPosData[3] = '3';           break;
	case   '4'  : MdsPosData[3] = '4';           break;
	case   '5'  : MdsPosData[3] = '5';           break;

	default     : MdsPosData[3] = '1';           break;
	}

	/*-----------------------------------------------------------------------*/
	/* SubField 5 : Pos-Card       Presence   Indicator                      */
	/*-----------------------------------------------------------------------*/
	switch (IsoMode[5]) /* <== Card  Presence   Indicator */
	{
	case   '0'  : MdsPosData[4] = '1';           break;
	case   '1'  : MdsPosData[4] = '0';           break;

	default     : MdsPosData[4] = '1';           break;
	}

	/*-----------------------------------------------------------------------*/
	/* SubField 6 : Pos-Card Capture Capabilities                            */
	/*-----------------------------------------------------------------------*/
	switch (IsoMode[2]) /* <== Card Retention Capability */
	{
	case   '0'  : MdsPosData[5] = '0';           break;
	case   '1'  : MdsPosData[5] = '1';           break;
	default     : MdsPosData[5] = '0';           break;
	}

	/*-----------------------------------------------------------------------*/
	/* SubField 7 : Pos-Transaction Status Indicator                         */
	/*-----------------------------------------------------------------------*/
	if ( PreAuthIndicator  == 'Y' )
		MdsPosData[6] = '4';
	else
		MdsPosData[6] = '0';

	/*-----------------------------------------------------------------------*/
	/* SubField 8 : Pos-Transaction Security indicator                       */
	/*-----------------------------------------------------------------------*/
	MdsPosData[7] = '0';

	/*-----------------------------------------------------------------------*/
	/* SubField 9 : RFU                                                      */
	/*-----------------------------------------------------------------------*/
	MdsPosData[8] = '0';

	/*-----------------------------------------------------------------------*/
	/* SubField 10 : CAT-Level Indicator                                      */
	/*-----------------------------------------------------------------------*/
	switch (IsoMode[3]) /* <== Operating Environnement */
	{
	case   'S'  : MdsPosData[9] = '1';           break;
	case   'T'  : MdsPosData[9] = '2';           break;
	case   'U'  : MdsPosData[9] = '3';           break;
	case   'V'  : MdsPosData[9] = '4';           break;
	case   '3'  : MdsPosData[9] = '2';           break;/*MDS Cert*/
	default     : MdsPosData[9] = '1';           break; /* 0 MKB 29032015 Certif NIV & MTIP & Online MDS */
	}
	if (IsoMode[6] == '9') /* E-Commerce */
		MdsPosData[9] = '6';
	if (IsoMode[6] == 'V') /* E-Commerce */
		MdsPosData[9] = '6';
	if (IsoMode[6] == 'U') /* E-Commerce */
		MdsPosData[9] = '6';
	if (IsoMode[6] == 'T') /* E-Commerce */
		MdsPosData[9] = '6';
	if (IsoMode[6] == 'S') /* E-Commerce */
		MdsPosData[9] = '6';

	/*-----------------------------------------------------------------------*/
	/* SubField 11 : POS Card Data Terminal Input Capability                  */
	/*-----------------------------------------------------------------------*/
	switch (IsoMode[0]) /* <== Operating Environnement */
	{
	case   '0'  : MdsPosData[10] = '0';           break;
	case   '1'  : MdsPosData[10] = '1';           break;
	case   '2'  : MdsPosData[10] = '2';           break;
	case   '3'  : MdsPosData[10] = '3';           break;
	case   '4'  : MdsPosData[10] = '4';           break;
	case   '5'  : MdsPosData[10] = '5';           break;
	case   '6'  : MdsPosData[10] = '6';           break;
	case   '7'  : MdsPosData[10] = '7';           break;
	case   '8'  : MdsPosData[10] = '8';           break;
	case   '9'  : MdsPosData[10] = '9';           break;
	default     : MdsPosData[10] = '0';           break;
	}

	

	if ( '5' == IsoMode[6]  || '8' == IsoMode[6]) /* MKB 29032015 Certif NIV & MTIP & Online MDS */
		MdsPosData[10] = '5';

	

	/*-----------------------------------------------------------------------*/
	/* SubField 12 : POS Authorisation Life Cycle                             */
	/*-----------------------------------------------------------------------*/
	if ( PreAuthIndicator  == 'Y' )
		memcpy ( MdsPosData + 11 , PreAuthTime , 2 );
	else
		/*memcpy ( MdsPosData + 11 , "00" , 2 );*/
		memcpy ( MdsPosData + 11 , "01" , 2 );/* MDS Cert by AA*/

	/*-----------------------------------------------------------------------*/
	/* SubField 13 : POS Country  Code                                        */
	/*-----------------------------------------------------------------------*/
	memcpy ( MdsPosData + 13 , TerminalCountryCode , 3 );

	/*-----------------------------------------------------------------------*/
	/* SubField 14 : POS Postal   Code                                        */
	/*-----------------------------------------------------------------------*/
	memcpy ( MdsPosData + 16 , "0000000000" , 10  );

	MdsPosData[26] = 0;

	

	trace_event("End IsoToMdsPosMode(OK)", PROCESSING);
	return;
}
/*************************************************************************/
/*   IsoToMdsProcessingCode()                                            */
/*                                                                       */
/*   Function to convert ISO processing code to MDS processing code      */
/*                                                                       */
/*   Input:  IsoCode: ISO processing code                                */
/*   Output: MdsCode: MDS processing code                                */
/*************************************************************************/
void  IsoToMdsProcessingCode ( char  *IsoCode,
	char  *MdsCode,
	char  *sMcc)
{
	char    v_process_code    [7];
	char    v_posit1          [3];
	char    v_posit2          [3];
	char    v_posit3          [3];
	char    sLine             [MAX_LINE_TRC];

	int     v_trans_type;
	int     v_account_from;
	int     v_account_to;
	int     nMcc = 0;

	sprintf(sLine, "Start IsoToMdsProcessingCode(%6s)", IsoCode);
	trace_event(sLine, PROCESSING);

	memset(v_process_code, 0, sizeof(v_process_code));
	memset(v_posit1,       0, sizeof(v_posit1));
	memset(v_posit2,       0, sizeof(v_posit2));
	memset(v_posit3,       0, sizeof(v_posit3));

	v_trans_type   = 0;
	v_account_from = 0;
	v_account_to   = 0;

	memcpy(v_posit1, IsoCode,     2);
	memcpy(v_posit2, IsoCode + 2, 2);
	memcpy(v_posit3, IsoCode + 4, 2);

	v_trans_type   = atoi(v_posit1);
	v_account_from = atoi(v_posit2);
	v_account_to   = atoi(v_posit3);

	switch(v_trans_type)
	{
	case   1  : memcpy(v_process_code, "01", 2); break;
	case   2  : memcpy(v_process_code, "02", 2); break;
	case   3  : memcpy(v_process_code, "03", 2); break;
	case   9  : memcpy(v_process_code, "00", 2); break;    /* Mehdi Elyajizi Enh7.2 Purchase with cash back */    
	case  11  : memcpy(v_process_code, "11", 2); break;
	case  17  : memcpy(v_process_code, "01", 2); break;
	case  18  : memcpy(v_process_code, "40", 2); break;
	case  19  : memcpy(v_process_code, "19", 2); break;
	case  20  : memcpy(v_process_code, "20", 2); break;		/*IBO20190805 PROD00070403 Enh19.Q4 AN 2643*/
	case  22  : memcpy(v_process_code, "22", 2); break;
	case  29  : memcpy(v_process_code, "29", 2); break;
	case  31  : memcpy(v_process_code, "30", 2); break;
	case  40  : memcpy(v_process_code, "40", 2); break;
	case  91  : memcpy(v_process_code, "92", 2); break;
	default   : memcpy(v_process_code, "00", 2);
		trace_event("Invalid processing code", ERROR);
	}

	switch(v_account_from)
	{

	case    0  : memcpy(v_process_code + 2, "00", 2); break;
	case   10  : memcpy(v_process_code + 2, "10", 2); break;
	case   20  : memcpy(v_process_code + 2, "20", 2); break;
	case   30  : memcpy(v_process_code + 2, "30", 2); break;
	default    : memcpy(v_process_code + 2, "00", 2);
		trace_event("Invalid processing code", ERROR);
	}

	switch(v_account_to)
	{

	case    0  : memcpy(v_process_code + 4, "00", 2); break;
	case   10  : memcpy(v_process_code + 4, "10", 2); break;
	case   20  : memcpy(v_process_code + 4, "20", 2); break;
	case   30  : memcpy(v_process_code + 4, "30", 2); break;
	default    : memcpy(v_process_code + 4, "00", 2);
		trace_event("Invalid processing code", ERROR);
	}
	nMcc = atoi(sMcc);
	switch(nMcc)
	{

	case    6051  : memcpy(v_process_code , "11", 2); break;
	case    7995  : memcpy(v_process_code , "11", 2); break;
	case    4829  : memcpy(v_process_code , "11", 2); break;
	}

	memcpy(MdsCode, v_process_code, 6);
	MdsCode[6] = '\0';

	trace_event("End  IsoToMdsProcessingCode(OK)", PROCESSING);
	return;

}



/*******************************************************************************************/
/*				*/
/*	BuildPosMode		*/
/*				*/
/**********************************************************************************************/

void  BuildPosMode ( char *IsoPosMode , char *mdsPosMode)
{
	char    sLine             [MAX_LINE_TRC];
	
	trace_event("Start  BuildPosMode", PROCESSING);
	/*-----------------------------------------------------------------------*/
	/* Card Data Input Mode                                                  */
	/*-----------------------------------------------------------------------*/
	switch (IsoPosMode[6]) /* <== Card Data Input Mode */
	{
	case   '0'  : memcpy ( mdsPosMode ,  "00" , 2 );  break;
	case   '1'  : memcpy ( mdsPosMode ,  "01" , 2 );  break;
		/* case   '2'  : memcpy ( mdsPosMode ,  "02" , 2 );  break; */
	case   '2'  : memcpy ( mdsPosMode ,  "90" , 2 );  break;
	case   '3'  : memcpy ( mdsPosMode ,  "03" , 2 );  break;
	case   '4'  : memcpy ( mdsPosMode ,  "04" , 2 );  break;
	case   '5'  : memcpy ( mdsPosMode ,  "05" , 2 );  break;
	case   '6'  : memcpy ( mdsPosMode ,  "01" , 2 );  break;

	case   '8'  : memcpy ( mdsPosMode ,  "80" , 2 );  break;
	case   'F'  : memcpy(  mdsPosMode,   "10",  2 );  break;	/*AMER20180308 PROD00054205: Enh 18.Q2 AN1103*/
	case   '9'  :
	case   'S'  :
	case   'T'  :
	case   'U'  :
	case   'V'  : memcpy ( mdsPosMode ,  "81" , 2 );  break;
	case   'W'  : memcpy ( mdsPosMode ,  "90" , 2 );  break;

	default     : memcpy ( mdsPosMode ,  "00" , 2 );  break;
	}

	/*-----------------------------------------------------------------------*/
	/* Pin Entry Capability                                                  */
	/*-----------------------------------------------------------------------*/
	switch (IsoPosMode[11]) /* <== Pin Entry Capability */
	{
		/*case   '0'  : memcpy ( mdsPosMode + 2 ,  "2" , 1 );  break; MDS Precert */
	case   '0'  : memcpy ( mdsPosMode + 2 ,  "1" , 1 );  break;
	case   '1'  : memcpy ( mdsPosMode + 2 ,  "0" , 1 );  break;
	default     : memcpy ( mdsPosMode + 2 ,  "1" , 1 );  break;
	}
	
	sprintf  ( sLine , "mdsPosMode = %s ", mdsPosMode );
	trace_event ( sLine , PROCESSING );
	
	trace_event("End  BuildPosMode", PROCESSING);

	return;
}

int GetMdsTlv(char *pTag, char * pTlvBuffer, int pTlvLen, char *pData, int *pLength)
{
	int length, ptr;
	int nBytes;
	int nFldType, nFldLength;
	char szMdsFldLen[4];
	char szAscFldLen[4];

	/** trace_event("Start GetMdsTlv()", PROCESSING);  **/

	length = 0;
	ptr = 0;
	while ( ptr < pTlvLen)
	{
		if ( !memcmp(pTag, pTlvBuffer + ptr, 2) )   
		{
			memcpy(szAscFldLen, pTlvBuffer + ptr + 2, 2);
			*pLength = atoi(szAscFldLen);

			memcpy( pData, pTlvBuffer + ptr + 2 + 2, *pLength);
			/** trace_event("End   GetMdsTlv(OK)", PROCESSING);  **/
			return(SUCCES);
		}

		memcpy(szAscFldLen, pTlvBuffer + ptr + 2, 2);
		ptr += 2 + 2 + atoi(szAscFldLen);
	}

	/** trace_event("End   GetMdsTlv(NOK)", PROCESSING);  **/
	return(ECHEC);

}
