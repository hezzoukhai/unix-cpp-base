#include <stdio.h>
#include <stdlib.h>

#include <iso_hps.h>
#include <tools.h>
#include <define.h>
#include <iso_bic.h>


int GetIsoTokenField(int field_no, TSIsoInfo * msgInfo, char *data, int *nLength, char *token)
{
	int length;
	int nBytes;
	char szIsoFldLen[4];	
	char sLine [MAX_LINE_TRC];

	memset( szIsoFldLen, '\0', 4);	
	if (CheckBit (msgInfo->sBitMap, field_no))
	{
		switch (GetIsoTokenFieldType(field_no, token))
		{
		case ISO_LL_BIN :
			memcpy( szIsoFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
			length = atoi(szIsoFldLen);
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
			*nLength = length;
			break;

		case ISO_LLL_BIN :
			memcpy( szIsoFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
			length = atoi(szIsoFldLen);
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
			*nLength = length;
			break;

		case ISO_FIX_BIN   :
		case ISO_FIX_ALPHA :
			length = GetIsoTokenFieldLength(field_no, token);
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
			data[length] = '\0';
			*nLength = length;
			break;

		case ISO_FIX_HEX :                           
			length = GetIsoTokenFieldLength(field_no, token);
			nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
				data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case ISO_LL_HEX :                           
			memcpy( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
			length = atoi(szIsoFldLen);
			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case ISO_LLL_HEX :                           
			memcpy ( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			length = atoi(szIsoFldLen);
			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case ISO_LL_ALPHA : 
			memcpy ( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
			length = atoi(szIsoFldLen);

			memcpy ( data, msgInfo->sData+msgInfo->nFieldPos[field_no]+2, length); 

			data[length] = '\0';
			*nLength = length;
			break;

		case ISO_LLL_ALPHA : 
			memcpy ( szIsoFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			length = atoi(szIsoFldLen);
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

/* Nabil Abouloula 17112009 Ajout parametre set pour le bitmap (1 , 0) */
int AddIsoTokenField(int field_no, TSIsoInfo *msgInfo, char *data, int nLength, char *token,int set)
{
	int nBytes  = 0;
	int i;
	char szIsoFldLen[5];	
	char sLine [MAX_LINE_TRC];

	sprintf(sLine,"Start Adding Token %s|%d",token,field_no);
	trace_event(sLine, TRACE);
	memset( szIsoFldLen, '\0', 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	msgInfo->nFieldPos[field_no] = msgInfo->nLength;
	switch (GetIsoTokenFieldType(field_no, token))
	{
	case ISO_FIX_ALPHA :
		if ( GetIsoTokenFieldLength(field_no, token) != nLength)
			return(ECHEC);
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case ISO_LL_ALPHA :
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		/*memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 2);
		msgInfo->nLength += 2;*/
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case ISO_LLL_ALPHA :
		sprintf ( szIsoFldLen, "%.3d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 3);
		msgInfo->nLength += 3;
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case BIC_LLLL_ALPHA :
		sprintf ( szIsoFldLen, "%.4d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 4);
		msgInfo->nLength += 4;
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case ISO_FIX_HEX :
		if ( GetIsoTokenFieldLength(field_no, token) != nLength)
			return(ECHEC);
		nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case ISO_LL_HEX :
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 2);
		msgInfo->nLength += 2;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case ISO_LLL_HEX :
		sprintf ( szIsoFldLen, "%.3d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 3);
		msgInfo->nLength += 3;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case ISO_FIX_BIN :
		if ( GetIsoTokenFieldLength(field_no, token) != nLength)
			return(ECHEC);
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case ISO_LL_BIN :
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szIsoFldLen, 2);
		msgInfo->nLength += 2;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case ISO_LLL_BIN :
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
	if(set) /* Nabil Abouloula 17112009 */
		SetBit(msgInfo->sBitMap, field_no+1);
	/*
	if ( (field_no >64) && (field_no < 128))
	SetBit(msgInfo->sBitMap, 1);**/

	/*
	sprintf(sLine,"Field \t Bitmap");
	trace_event(sLine, TRACE);
	sprintf(sLine,"%d \t %d",field_no,);
	trace_event(sLine, TRACE);
	*/

	return(SUCCES);
}

int InsertIsoTokenField(int field_no, TSIsoInfo *msgInfo, char *data, int nLength, char *token)
{
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i;
	char szIsoData [999];	
	char szIsoFldLen[4];	
	int  nOldFldLen,nOldLen;
	char szWork    [LG_MAX];	

	memset( szIsoFldLen, '\0', 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	switch (GetIsoTokenFieldType(field_no, token))
	{
	case ISO_FIX_ALPHA :
		if ( GetIsoTokenFieldLength(field_no, token) != nLength)
			return(ECHEC);
		nBytes = nLength;
		memcpy (szIsoData, data, nBytes);
		break;

	case ISO_LL_ALPHA :
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		memcpy( szIsoData,        szIsoFldLen, 2);
		nBytes=2;
		memcpy( szIsoData+2, data,        nLength);
		nBytes += nLength;
		break;

	case ISO_LLL_ALPHA :
		sprintf ( szIsoFldLen, "%.3d", nLength);	
		nBytes = 3;
		memcpy( szIsoData, szIsoFldLen, 3);
		memcpy( szIsoData+3, data,        nLength);
		nBytes += nLength;
		break;

	case BIC_LLLL_ALPHA :
		sprintf ( szIsoFldLen, "%.4d", nLength);	
		nBytes = 3;
		memcpy( szIsoData, szIsoFldLen, 4);
		memcpy( szIsoData+3, data,        nLength);
		nBytes += nLength;
		break;

	case ISO_FIX_HEX :
		if ( GetIsoTokenFieldLength(field_no, token) != nLength)
			return(ECHEC);
		nBytes = AsciiToHex(data, szIsoData, nLength);
		break;

	case ISO_LL_HEX :
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		nBytes1 = 2;
		memcpy( szIsoData, szIsoFldLen, nBytes1);
		nBytes = AsciiToHex( data, szIsoData+2, nLength);
		nBytes += nBytes1;
		break;

	case ISO_LLL_HEX :
		sprintf ( szIsoFldLen, "%.3d", nLength);	
		nBytes1 = 3;
		memcpy( szIsoData, szIsoFldLen, 3);
		nBytes = AsciiToHex( data, szIsoData+3, nLength);
		nBytes += nBytes1;
		break;

	case ISO_FIX_BIN :
		if ( GetIsoTokenFieldLength(field_no, token) != nLength)
			return(ECHEC);
		nBytes = nLength;
		memcpy( szIsoData, data, nBytes);
		break;

	case ISO_LL_BIN :
		sprintf ( szIsoFldLen, "%.2d", nLength);	
		nBytes = 2;
		memcpy( szIsoData, szIsoFldLen, nBytes);
		memcpy( szIsoData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	case ISO_LLL_BIN :
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
	return(SUCCES);
}

int PutIsoTokenField(int field_no, TSIsoInfo *msgInfo, char *data, int nLength , char *token)
{
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i, nOldFldLen,nOldLen;
	char szIsoData [999];	
	char szWork    [LG_MAX];	
	char szIsoFldLen[4];	

	memset( szIsoFldLen, '\0', 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
	{
		switch (GetIsoTokenFieldType(field_no, token))
		{
		case ISO_FIX_ALPHA :
			if ( GetIsoTokenFieldLength(field_no, token) != nLength)
				return(ECHEC);
			nBytes = nLength;
			memcpy (szIsoData, data, nBytes);
			break;

		case ISO_LL_ALPHA :
			sprintf ( szIsoFldLen, "%.2d", nLength);	
			memcpy( szIsoData,        szIsoFldLen, 2);
			nBytes=2;
			memcpy( szIsoData+2, data,        nLength);
			nBytes += nLength;
			break;

		case ISO_LLL_ALPHA :
			sprintf ( szIsoFldLen, "%.3d", nLength);	
			nBytes = 3;
			memcpy( szIsoData, szIsoFldLen, 3);
			memcpy( szIsoData+3, data,        nLength);
			nBytes += nLength;
			break;

		case BIC_LLLL_ALPHA :
			sprintf ( szIsoFldLen, "%.4d", nLength);	
			nBytes = 3;
			memcpy( szIsoData, szIsoFldLen, 4);
			memcpy( szIsoData+3, data,        nLength);
			nBytes += nLength;
			break;

		case ISO_FIX_HEX :
			if ( GetIsoTokenFieldLength(field_no, token) != nLength)
				return(ECHEC);
			nBytes = AsciiToHex(data, szIsoData, nLength);
			break;

		case ISO_LL_HEX :
			sprintf ( szIsoFldLen, "%.2d", nLength);	
			nBytes1 = 2;
			memcpy( szIsoData, szIsoFldLen, nBytes1);
			nBytes = AsciiToHex( data, szIsoData+2, nLength);
			nBytes += nBytes1;
			break;

		case ISO_LLL_HEX :
			sprintf ( szIsoFldLen, "%.3d", nLength);	
			nBytes1 = 3;
			memcpy( szIsoData, szIsoFldLen, 3);
			nBytes = AsciiToHex( data, szIsoData+3, nLength);
			nBytes += nBytes1;
			break;

		case ISO_FIX_BIN :
			if ( GetIsoTokenFieldLength(field_no, token) != nLength)
				return(ECHEC);
			nBytes = nLength;
			memcpy( szIsoData, data, nBytes);
			break;

		case ISO_LL_BIN :
			sprintf ( szIsoFldLen, "%.2d", nLength);	
			nBytes = 2;
			memcpy( szIsoData, szIsoFldLen, nBytes);
			memcpy( szIsoData+nBytes, data, nLength);
			nBytes += nLength;
			break;

		case ISO_LLL_BIN :
			sprintf ( szIsoFldLen, "%.3d", nLength);	
			nBytes = 3;
			memcpy( szIsoData, szIsoFldLen, nBytes);
			memcpy( szIsoData+nBytes, data, nLength);
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
		return(SUCCES);
	}
	else
		return ( InsertIsoField (field_no, msgInfo, data, nLength));

}

int IsoTokenBuildMsg (char * buffer_snd, TSIsoInfo *msgInfo)
{
	char szBitMap[ 16 + 1];
	int  nBytes;
	int  iMapCtr;
	int i;
	char sLine [MAX_LINE_TRC];


	iMapCtr = 1;
	while ( ( msgInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
		++iMapCtr;

	HexToAscii(msgInfo->sBitMap, szBitMap, 16);

	memcpy( buffer_snd, szBitMap, 4);
	memcpy( buffer_snd+4 , msgInfo->sData, msgInfo->nLength);
	return( 4 + msgInfo->nLength);
}

