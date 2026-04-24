#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tools.h>
#include <define.h>


#include <pulse/iso_pulse.h>
#include <pulse/pulse_privfields.h>



int GetPulseField(int field_no, TSPulseInfo * msgInfo, char *data, int *nLength)
{
	int length;
	int nBytes;
	int nFldType, nFldLength;
	char szPulseFldLen[4];	
	char szAscFldLen[4];
	char  sLine[MAX_LINE_TRC];

	memset( szPulseFldLen, '\0', 4);	
	memset( szAscFldLen, '\0', 4);	

	if (CheckBit (msgInfo->sBitMap, field_no))
	{
		nFldType   =  GetPulseFieldType  (field_no);
		nFldLength =  GetPulseFieldLength(field_no);

		switch (nFldType)
		{

		case PULSE_VAR_BITMAP :
			memcpy(szPulseFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
			EbcdicToAscii(szPulseFldLen, szAscFldLen, 2);
			length = atoi(szAscFldLen);

			if( length > nFldLength )
			{
				sprintf(sLine,"Invalid length (%d)(Max:%d) for field %d",length, nFldLength, field_no);
				trace_event(sLine,ERROR);
				return ECHEC;
			}

			memcpy(data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2,length);
			*nLength = length;
			break;

		case PULSE_LL_BIN :
			memcpy(szPulseFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 2);
			EbcdicToAscii(szPulseFldLen, szAscFldLen, 2);
			length = atoi(szAscFldLen);

			if( length > nFldLength )
			{
				sprintf(sLine,"Invalid length (%d)(Max:%d) for field %d",length, nFldLength, field_no);
				trace_event(sLine,ERROR);
				return ECHEC;
			}

			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+2, length);
			*nLength = length;
			break;

		case PULSE_LLL_BIN :
			memcpy(szPulseFldLen, msgInfo->sData + msgInfo->nFieldPos[field_no], 3);
			EbcdicToAscii(szPulseFldLen, szAscFldLen, 3);
			length = atoi(szAscFldLen);

			if( length > nFldLength )
			{
				sprintf(sLine,"Invalid length (%d)(Max:%d) for field %d",length, nFldLength, field_no);
				trace_event(sLine,ERROR);
				return ECHEC;
			}

			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no]+3, length);
			*nLength = length;
			break;

		case PULSE_FIX_BIN   :
			length = nFldLength;
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
			data[length] = '\0';
			*nLength = length;
			break;

		case PULSE_FIX_ALPHA :
			length = nFldLength;
			EbcdicToAscii( msgInfo->sData + msgInfo->nFieldPos[field_no], data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case PULSE_FIX_HEX :                           
			length = nFldLength;
			nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case PULSE_LL_HEX :                           
			memcpy( szPulseFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
			EbcdicToAscii(szPulseFldLen, szAscFldLen, 2);
			length = atoi(szAscFldLen);

			if( length > nFldLength )
			{
				sprintf(sLine,"Invalid length (%d)(Max:%d) for field %d",length, nFldLength, field_no);
				trace_event(sLine,ERROR);
				return ECHEC;
			}

			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+2, data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case PULSE_LLL_HEX :                           
			memcpy ( szPulseFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			EbcdicToAscii(szPulseFldLen, szAscFldLen, 3);
			length = atoi(szAscFldLen);

			if( length > nFldLength )
			{
				sprintf(sLine,"Invalid length (%d)(Max:%d) for field %d",length, nFldLength, field_no);
				trace_event(sLine,ERROR);
				return ECHEC;
			}

			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case PULSE_LL_ALPHA : 
			memcpy ( szPulseFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 2);
			EbcdicToAscii(szPulseFldLen, szAscFldLen, 2);
			length = atoi(szAscFldLen);

			if( length > nFldLength )
			{
				sprintf(sLine,"Invalid length (%d)(Max:%d) for field %d",length, nFldLength, field_no);
				trace_event(sLine,ERROR);
				return ECHEC;
			}

			EbcdicToAscii ( msgInfo->sData+msgInfo->nFieldPos[field_no]+2, data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case PULSE_LLL_ALPHA : 
			memcpy ( szPulseFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			EbcdicToAscii(szPulseFldLen, szAscFldLen, 3);
			length = atoi(szAscFldLen);

			if( length > nFldLength )
			{
				sprintf(sLine,"Invalid length (%d)(Max:%d) for field %d",length, nFldLength, field_no);
				trace_event(sLine,ERROR);
				return ECHEC;
			}

			EbcdicToAscii ( msgInfo->sData+msgInfo->nFieldPos[field_no]+3, data, length);
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

int AddPulseField(int field_no, TSPulseInfo *msgInfo, char *data, int nLength)
{
	int nBytes  = 0;
	int nFldType, nFldLength;
	char szPulseFldLen[4];	
	char szEbcFldLen[4];
	char  sLine[MAX_LINE_TRC];
	int i;

	memset( szPulseFldLen, '\0', 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
	{
		return(ECHEC);
	}
	nFldType   =  GetPulseFieldType  (field_no);
	nFldLength =  GetPulseFieldLength(field_no);
	if( nLength > nFldLength )
	{
		sprintf(sLine,"Add: Invalid length (%d)(Max:%d) for field %d",nLength, nFldLength, field_no);
		trace_event(sLine,ERROR);
		return ECHEC;
	}

	switch (nFldType)
	{

	case PULSE_VAR_BITMAP :
		sprintf ( szPulseFldLen, "%.3d", nLength);	
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 3);

		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
		msgInfo->nLength += 3;

		/*
		memcpy( (unsigned char*) msgInfo->sData + msgInfo->nLength, (unsigned char*) data, nLength);
		nBytes = nLength;
		*/
		AsciiToEbcdic( (unsigned char*) data, (unsigned char*) msgInfo->sData+msgInfo->nLength, nLength);
		nBytes = nLength;
		break;

	case PULSE_FIX_ALPHA :
		if ( nFldLength != nLength)
			return(ECHEC);
		AsciiToEbcdic( (unsigned char*) data, (unsigned char*) msgInfo->sData+msgInfo->nLength, nLength);
		nBytes = nLength;
		break;

	case PULSE_LL_ALPHA :
		sprintf ( szPulseFldLen, "%.2d", nLength);	
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 2);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
		msgInfo->nLength += 2;
		AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
		nBytes = nLength;
		break;

	case PULSE_LLL_ALPHA :
		sprintf ( szPulseFldLen, "%.3d", nLength);	
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 3);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
		msgInfo->nLength += 3;
		AsciiToEbcdic( data, msgInfo->sData+msgInfo->nLength, nLength);
		nBytes = nLength;
		break;

	case PULSE_FIX_HEX :
		if ( nFldLength != nLength)
			return(ECHEC);
		nBytes = AsciiToHex((unsigned char*)data, (unsigned char*) msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case PULSE_LL_HEX :
		sprintf ( szPulseFldLen, "%.2d", nLength);	
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 2);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
		msgInfo->nLength += 2;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case PULSE_LLL_HEX :
		sprintf ( szPulseFldLen, "%.3d", nLength);	
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 3);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
		msgInfo->nLength += 3;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case PULSE_FIX_BIN :
		if ( nFldLength != nLength)
			return(ECHEC);
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case PULSE_LL_BIN :
		sprintf ( szPulseFldLen, "%.2d", nLength);	
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 2);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 2);
		msgInfo->nLength += 2;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	case PULSE_LLL_BIN :
		sprintf ( szPulseFldLen, "%.3d", nLength);	
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 3);
		memcpy( msgInfo->sData+msgInfo->nLength, szEbcFldLen, 3);
		msgInfo->nLength += 3;
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;

	default :
		return(ECHEC);
	}
	msgInfo->nLength += nBytes;
	SetBit(msgInfo->sBitMap, field_no+1);
	for (i=field_no; i< MAX_PULSE_FIELDS; i++)
		msgInfo->nFieldPos[i+1] = msgInfo->nLength;
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);
	return(SUCCES);
}

int InsertPulseField(int field_no, TSPulseInfo *msgInfo, char *data, int nLength)
{
	int  nFldType, nFldLength;
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i;
	char szPulseData [999];	
	char szPulseFldLen[4];	
	char szEbcFldLen[4];
	char  sLine[MAX_LINE_TRC];

	memset( szPulseFldLen, '\0', 4);	
	memset( szEbcFldLen, '\0', 4);	
	memset(szPulseData, 0, sizeof(szPulseData));

	if ( CheckBit( msgInfo->sBitMap, field_no))
	{
		trace_event(" CheckBit Error", PROCESSING);
		return(ECHEC);
	}
	nFldType   =  GetPulseFieldType  (field_no);
	nFldLength =  GetPulseFieldLength(field_no);

	if( nLength > nFldLength )
	{
		sprintf(sLine,"Ins: Invalid length (%d)(Max:%d) for field %d",nLength, nFldLength, field_no);
		trace_event(sLine,ERROR);
		return ECHEC;
	}


	switch (nFldType)
	{

	case PULSE_VAR_BITMAP :
		trace_event( "InsertPulseField, PULSE_VAR_BITMAP", PROCESSING);

		*(szPulseData) = nLength;
		nBytes = 1;
		memcpy( szPulseData+1, data, nLength);
		nBytes += nLength;
		break;

	case PULSE_FIX_ALPHA :
		if ( nFldLength != nLength)
			return(ECHEC);
		nBytes = nLength;
		AsciiToEbcdic (data, szPulseData, nBytes);
		break;

	case PULSE_LL_ALPHA :
		sprintf ( szPulseFldLen, "%.2d", nLength);	
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 2);
		memcpy( szPulseData,        szEbcFldLen, 2);
		nBytes=2;
		AsciiToEbcdic( data,        szPulseData+2, nLength);
		nBytes += nLength;
		break;

	case PULSE_LLL_ALPHA :
		sprintf ( szPulseFldLen, "%.3d", nLength);	
		nBytes = 3;
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 3);
		memcpy( szPulseData, szEbcFldLen, 3);
		AsciiToEbcdic( data,        szPulseData+3, nLength);
		nBytes += nLength;
		break;

	case PULSE_FIX_HEX :
		if ( nFldLength != nLength)
			return(ECHEC);
		nBytes = AsciiToHex((unsigned char*) data,(unsigned char*) szPulseData, nLength);
		break;

	case PULSE_LL_HEX :
		sprintf ( szPulseFldLen, "%.2d", nLength);	
		nBytes1 = 2;
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 2);
		memcpy( szPulseData, szEbcFldLen, nBytes1);
		nBytes = AsciiToHex( data, szPulseData+2, nLength);
		nBytes += nBytes1;
		break;

	case PULSE_LLL_HEX :
		sprintf ( szPulseFldLen, "%.3d", nLength);	
		nBytes1 = 3;
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 3);
		memcpy( szPulseData, szEbcFldLen, 3);
		nBytes = AsciiToHex( data, szPulseData+3, nLength);
		nBytes += nBytes1;
		break;

	case PULSE_FIX_BIN :
		if ( nFldLength != nLength)
			return(ECHEC);
		nBytes = nLength;
		memcpy( szPulseData, data, nBytes);
		break;

	case PULSE_LL_BIN :
		sprintf ( szPulseFldLen, "%.2d", nLength);	
		nBytes = 2;
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 2);
		memcpy( szPulseData, szEbcFldLen, nBytes);
		memcpy( szPulseData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	case PULSE_LLL_BIN :
		sprintf ( szPulseFldLen, "%.3d", nLength);	
		nBytes = 3;
		AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 3);
		memcpy( szPulseData, szEbcFldLen, nBytes);
		memcpy( szPulseData+nBytes, data, nLength);
		nBytes += nLength;
		break;

	default :
		return(ECHEC);
	}
	for (i = MAX_PULSE_FIELDS-1; i > field_no; i--)
	{
		memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes,
			msgInfo->sData+msgInfo->nFieldPos[i],
			msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
		msgInfo->nFieldPos[i+1] += nBytes;
	}
	memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPulseData, nBytes);
	msgInfo->nFieldPos[field_no+1] += nBytes;
	msgInfo->nLength += nBytes;
	SetBit(msgInfo->sBitMap, field_no+1);
	if ( (field_no >64) && (field_no < 128))
		SetBit(msgInfo->sBitMap, 1);
	return(SUCCES);
}

int PutPulseField(int field_no, TSPulseInfo *msgInfo, char *data, int nLength)
{
	int  nFldType, nFldLength;
	int  nBytes   = 0;
	int  nBytes1  = 0;
	int  i, nOldFldLen ,nOldLen;/*PLUTONL-1847*/
	char szPulseData [999];	
	char szPulseFldLen[4];	
	char szEbcFldLen[4];
	char  sLine[MAX_LINE_TRC];
	char szWork    [LG_MAX];/*HAL23062020 PLUTONL-1847*/	

	memset( szPulseFldLen, '\0', 4);	
	memset( szEbcFldLen, '\0', 4);	
	memset (szPulseData , 0, sizeof(szPulseData));

	if ( CheckBit( msgInfo->sBitMap, field_no))
	{
		nFldType   =  GetPulseFieldType  (field_no);
		nFldLength =  GetPulseFieldLength(field_no);

		if( nLength > nFldLength )
		{
			sprintf(sLine,"Put: Invalid length (%d)(Max:%d) for field %d",nLength, nFldLength, field_no);
			trace_event(sLine,ERROR);
			return ECHEC;
		}

		switch (nFldType)
		{
		case PULSE_VAR_BITMAP :
			trace_event( "PutPulseField, PULSE_VAR_BITMAP", PROCESSING);

			*(szPulseData) = nLength;
			nBytes = 1;
			memcpy( szPulseData+1, data, nLength);
			nBytes += nLength;
			break;

		case PULSE_FIX_ALPHA :
			if ( nFldLength != nLength)
				return(ECHEC);
			nBytes = nLength;
			AsciiToEbcdic (data, szPulseData, nBytes);
			break;

		case PULSE_LL_ALPHA :
			sprintf ( szPulseFldLen, "%.2d", nLength);	
			AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 2);
			memcpy( szPulseData,        szEbcFldLen, 2);
			nBytes=2;
			AsciiToEbcdic( data,        szPulseData+2, nLength);
			nBytes += nLength;
			break;

		case PULSE_LLL_ALPHA :
			sprintf ( szPulseFldLen, "%.3d", nLength);	
			nBytes = 3;
			AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 3);
			memcpy( szPulseData, szEbcFldLen, 3);
			AsciiToEbcdic( data,        szPulseData+3, nLength);
			nBytes += nLength;
			break;

		case PULSE_FIX_HEX :
			if ( nFldLength != nLength)
				return(ECHEC);
			nBytes = AsciiToHex((unsigned char*)data, (unsigned char*)szPulseData, nLength);
			break;

		case PULSE_LL_HEX :
			sprintf ( szPulseFldLen, "%.2d", nLength);	
			nBytes1 = 2;
			AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 2);
			memcpy( szPulseData, szEbcFldLen, nBytes1);
			nBytes = AsciiToHex( data, szPulseData+2, nLength);
			nBytes += nBytes1;
			break;

		case PULSE_LLL_HEX :
			sprintf ( szPulseFldLen, "%.3d", nLength);	
			nBytes1 = 3;
			AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 3);
			memcpy( szPulseData, szEbcFldLen, 3);
			nBytes = AsciiToHex( data, szPulseData+3, nLength);
			nBytes += nBytes1;
			break;

		case PULSE_FIX_BIN :
			if ( nFldLength != nLength)
				return(ECHEC);
			nBytes = nLength;
			memcpy( szPulseData, data, nBytes);
			break;

		case PULSE_LL_BIN :
			sprintf ( szPulseFldLen, "%.2d", nLength);	
			nBytes = 2;
			AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 2);
			memcpy( szPulseData, szEbcFldLen, nBytes);
			memcpy( szPulseData+nBytes, data, nLength);
			nBytes += nLength;
			break;

		case PULSE_LLL_BIN :
			sprintf ( szPulseFldLen, "%.3d", nLength);	
			nBytes = 3;
			AsciiToEbcdic ( szPulseFldLen, szEbcFldLen, 3);
			memcpy( szPulseData, szEbcFldLen, nBytes);
			memcpy( szPulseData+nBytes, data, nLength);
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

		for (i = MAX_PULSE_FIELDS-1; i > field_no; i--)
		{

		 msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
		}
		memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPulseData, nBytes);
		msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
		msgInfo->nLength += nBytes-nOldFldLen;
		SetBit(msgInfo->sBitMap, field_no+1);
		memcpy(msgInfo->sData + msgInfo->nFieldPos[field_no + 1], szWork, nOldLen);
		if ( (field_no >64) && (field_no < 128))
			SetBit(msgInfo->sBitMap, 1);
		return(SUCCES);
	}
		   
	else
		return ( InsertPulseField (field_no, msgInfo, data, nLength));

		/*nOldFldLen = msgInfo->nFieldPos[field_no +1] - 
			msgInfo->nFieldPos[field_no];
		for (i = MAX_PULSE_FIELDS-1; i > field_no; i--)
		{
			memcpy (msgInfo->sData+msgInfo->nFieldPos[i]+nBytes-nOldFldLen,
				msgInfo->sData+msgInfo->nFieldPos[i],
				msgInfo->nFieldPos[i+1] - msgInfo->nFieldPos[i] );
			msgInfo->nFieldPos[i+1] += nBytes-nOldFldLen;
		}
		memcpy(msgInfo->sData+msgInfo->nFieldPos[field_no], szPulseData, nBytes);
		msgInfo->nFieldPos[field_no+1] += nBytes-nOldFldLen;
		msgInfo->nLength += nBytes-nOldFldLen;
		SetBit(msgInfo->sBitMap, field_no+1);
		if ( (field_no >64) && (field_no < 128))
			SetBit(msgInfo->sBitMap, 1);
		return(SUCCES);
	}
	else
	{
		if (( field_no == 62) || ( field_no == 63))
			trace_event( "on est dans le Insert", PROCESSING);

		return (InsertPulseField(field_no, msgInfo, data, nLength));
	}*/
		/*End HAL23062020 PLUTONL-1847*/

}

int PulseBuildMsg (char * buffer_snd, TSPulseInfo *msgInfo)
{
	char 	szMsgType[5];
	int  	nBytes;
	int  	iMapCtr;
	char    sLine[MAX_LINE_TRC];

	memset( sLine, 0, sizeof(sLine));

	sprintf(szMsgType, "%.4d", msgInfo->nMsgType);

	szMsgType[4] = '\0';

	sprintf(sLine," My trace !! szMsgType (%s) ",szMsgType);
	trace_event(sLine,TRACE);

	AsciiToEbcdic( szMsgType , buffer_snd , 4);

	sprintf(sLine," My trace !! my buffer (%s) ",buffer_snd);
	trace_event(sLine,TRACE);


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
int GetPulseIcTag (char  *tag_name, TSTagPulse *tagInfo, char *data, int *length)
{
	int nIndice ;
	int nLenTag ;
	int nDigits;
	char  sLine[MAX_LINE_TRC];

	if ( (nIndice = GetPulseTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == NOT_PRESENT)
		return(ECHEC);

	if ( (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]) & 0x1F) == 0x1F)
		nLenTag = 2;
	else
		nLenTag = 1;
	*length = (*(tagInfo->sChipData + tagInfo->nPosTag[nIndice]+nLenTag ));


	switch (GetPulseTagType(nIndice) )
	{
	case PULSE_TAG_FIX_BIN :
	case PULSE_TAG_FIX_ALPHA :
		memcpy( data,tagInfo->sChipData + tagInfo->nPosTag[nIndice] +nLenTag + 1,
			*length);
		break;

	case PULSE_TAG_VAR_HEX :
		nDigits = HexToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
			nLenTag+1, 
			data, 
			(*length)*2);
		*length = (*length)*2;
		break;
	case PULSE_TAG_VAR_BCD :
		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
			nLenTag+1, 
			data, 
			(*length)*2);
		*length = (*length)*2;
		break;

	case PULSE_TAG_FIX_BCD :
		nDigits = BcdToAscii( tagInfo->sChipData + tagInfo->nPosTag[nIndice] + 
			nLenTag+1, 
			data,
			(*length)*2);
		*length = (*length)*2;
		break;

	case PULSE_TAG_VAR_BIN :
	case PULSE_TAG_VAR_ALPHA :
		memcpy( data, tagInfo->sChipData +tagInfo->nPosTag[nIndice] +nLenTag + 1,
			*length);
		break;

	}
	return(SUCCES);
}

int AddPulseIcTag (char  *tag_name, TSTagPulse *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;
	char sLine[MAX_LINE_TRC];

	if ( (nIndice = GetPulseTagIndice(tag_name)) == -1)
		return(ECHEC);

	if (tagInfo->nPresent[nIndice] == PRESENT)
		return(ECHEC);

	AsciiToHex(tag_name, tagInfo->sChipData + tagInfo->nLength, 4);

	if ( (*(tagInfo->sChipData + tagInfo->nLength) & 0x1F ) == 0x1F)
		nLenTag = 2;
	else 
		nLenTag = 1;

	switch (GetPulseTagType(nIndice) )
	{
	case PULSE_TAG_FIX_BIN :
	case PULSE_TAG_FIX_ALPHA :
		if ( GetPulseTagLength(nIndice) != length)
			return(ECHEC);
		memcpy( tagInfo->sChipData + tagInfo->nLength +nLenTag + 1, data, length);
		nDigits = length;
		break;

	case PULSE_TAG_VAR_HEX :
		nDigits = AsciiToHex( data, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case PULSE_TAG_VAR_BCD :
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case PULSE_TAG_FIX_BCD :
		if ( GetPulseTagLength(nIndice) != length)
			return(ECHEC);
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + tagInfo->nLength + nLenTag+1, 
			length);
		break;

	case PULSE_TAG_VAR_BIN :
	case PULSE_TAG_VAR_ALPHA :
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

int PutPulseIcTag (char  *tag_name, TSTagPulse *tagInfo, char *data, int length)
{
	int nIndice;
	int nLenTag;
	int nLength;
	int nDigits;
	int nPutPos, nTagPresent = NOT_PRESENT;
	char szLastPart  [ MAX_PULSE_CHIP_LEN ];
	int nOldTagLen, i;

	memset (szLastPart, '\0', sizeof(szLastPart));
	if ( (nIndice = GetPulseTagIndice(tag_name)) == -1)
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

	switch (GetPulseTagType(nIndice) )
	{
	case PULSE_TAG_FIX_BIN :
	case PULSE_TAG_FIX_ALPHA :
		if ( GetPulseTagLength(nIndice) != length)
			return(ECHEC);
		memcpy( tagInfo->sChipData + nPutPos +nLenTag + 1, data, length);
		nDigits = length;
		break;

	case PULSE_TAG_VAR_HEX :
		nDigits = AsciiToHex( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case PULSE_TAG_VAR_BCD :
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case PULSE_TAG_FIX_BCD :
		if ( GetPulseTagLength(nIndice) != length)
			return(ECHEC);
		nDigits = AsciiToBcd( data, 
			tagInfo->sChipData + nPutPos + nLenTag+1, 
			length);
		break;

	case PULSE_TAG_VAR_BIN :
	case PULSE_TAG_VAR_ALPHA :
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
		for (i = nIndice + 1; i < MAX_PULSE_CHIP_TAG; i++)
		{
			if (tagInfo->nPresent[i] == PRESENT)
				tagInfo->nPosTag[i] += nDigits - nOldTagLen;
		}
	}
	return(SUCCES);
}

int PulseBuildIcFld (char * buffer_snd, TSTagPulse *tagInfo)
{
	memcpy (buffer_snd, tagInfo->sChipData, tagInfo->nLength);
	return(tagInfo->nLength);
}

/****************************************************************************/
/* Le champ 48                                                              */
/****************************************************************************/
int GetPulsePds (char  *pds_name, TSPdsPulse *pdsInfo, char *data, int *length)
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

	nPosLen = 0;
	nPosPds = 2;
	nPdsLen = 2;
	if ( (nIndice = GetPulsePdsIndice(pds_name)) == -1)
		return(ECHEC);
	nPdsType    = GetPulsePdsType(nIndice) ;
	nPdsLength  = GetPulsePdsLength(nIndice); 
	sprintf ( szAscPdsLen, "%.2d", length+2);

	if (pdsInfo->nPresent[nIndice] == NOT_PRESENT)
		return(ECHEC);

	memset ( szAscPdsLen , 0, sizeof ( szAscPdsLen ));
	EbcdicToAscii ( pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice]+nPosLen,
		szAscPdsLen, 
		nPdsLen);
	*length = atoi(szAscPdsLen) - nPosPds;

	switch ( nPdsType )
	{
	case PULSE_PDS_FIX_ALPHA :
		EbcdicToAscii(  pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] + 
			nPdsLen + nPdsLen, 
			data , 
			*length);
		break;

	case PULSE_PDS_VAR_ALPHA :
		EbcdicToAscii( pdsInfo->sPdsData + pdsInfo->nPosPds[nIndice] +
			nPdsLen + nPdsLen, 
			data ,
			*length);
		break;

	}

	return(SUCCES);
}

int AddPulsePds (char  *pds_name, TSPdsPulse *pdsInfo, char *data, int length)
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


	nPosLen = 0;
	nPosPds = 2;
	nPdsLen = 2;
	if ( (nIndice = GetPulsePdsIndice(pds_name)) == -1)
		return(ECHEC);
	nPdsType    = GetPulsePdsType(nIndice) ;
	nPdsLength  = GetPulsePdsLength(nIndice); 
	sprintf ( szAscPdsLen, "%.2d", length+2);

	if (pdsInfo->nPresent[nIndice] == PRESENT)
		return(ECHEC);


	switch ( nPdsType )
	{
	case PULSE_PDS_FIX_ALPHA :
		if ( nPdsLength != length)
			return(ECHEC);

		AsciiToEbcdic( data, 
			pdsInfo->sPdsData + pdsInfo->nLength +(2*nPdsLen), 
			length);
		break;

	case PULSE_PDS_VAR_ALPHA :
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

int PutPulsePds (char  *pds_name, TSPdsPulse *pdsInfo, char *data, int length)
{
	int nIndice;
	int nPosLen; /* Position de la longueur */
	int nPosPds; /* Position du PDS         */
	int nPdsLen; /* Longueur du PDS et de la longueur */
	int nPdsLength;
	char szAscPdsLen [4];
	char szOldPdsLen [4];
	char szLastPart  [ MAX_PULSE_PDS_LEN ];
	int nPdsType, i;
	int nPutPos, 
		nOldPdsLen, 
		nPdsPresent = NOT_PRESENT;

	memset (szAscPdsLen, '\0', 4);
	memset (szOldPdsLen, '\0', 4);
	memset (szLastPart, '\0', sizeof (szLastPart));


	nPosLen = 0;
	nPosPds = 2;
	nPdsLen = 2;
	if ( (nIndice = GetPulsePdsIndice(pds_name)) == -1)
		return(ECHEC);
	nPdsType    = GetPulsePdsType(nIndice) ;
	nPdsLength  = GetPulsePdsLength(nIndice); 
	sprintf ( szAscPdsLen, "%.2d", length+2);


	if (pdsInfo->nPresent[nIndice] == PRESENT)
	{
		nPutPos = pdsInfo->nPosPds[nIndice];
		memcpy (szOldPdsLen,
			pdsInfo->sPdsData + pdsInfo->nPosPds [nIndice] + nPosLen,
			nPdsLen);
		nOldPdsLen = atoi(szOldPdsLen) - nPosPds;
		memcpy(szLastPart, 
			pdsInfo->sPdsData + nPutPos + nPdsLen + nPdsLen + nOldPdsLen,
			pdsInfo->nLength - nPutPos - nPdsLen - nPdsLen - nOldPdsLen);
	}
	else
		nPutPos = pdsInfo->nLength;

	switch ( nPdsType )
	{
	case PULSE_PDS_FIX_ALPHA :
		if ( nPdsLength != length)
			return(ECHEC);
		AsciiToEbcdic( data, 
			pdsInfo->sPdsData + nPutPos +(2*nPdsLen), 
			length);
		break;

	case PULSE_PDS_VAR_ALPHA :
		AsciiToEbcdic( data, 
			pdsInfo->sPdsData + nPutPos +(2*nPdsLen), 
			length);
		break;

	}

	AsciiToEbcdic (szAscPdsLen,
		pdsInfo->sPdsData + nPutPos + nPosLen,
		nPdsLen 
		);
	if (nPdsPresent == NOT_PRESENT )
	{
		AsciiToEbcdic (pds_name,
			pdsInfo->sPdsData + pdsInfo->nLength + nPosPds,
			nPdsLen 
			);
		pdsInfo->nPosPds [ nIndice ] = pdsInfo->nLength;
		pdsInfo->nPresent[ nIndice ] = PRESENT;
		pdsInfo->nLength +=length + nPdsLen + nPdsLen;
	}
	else
	{
		memcpy (pdsInfo->sPdsData + nPutPos+ nPdsLen+ nPdsLen+ length- nOldPdsLen,
			szLastPart,
			pdsInfo->nLength - nPutPos - nPdsLen - nPdsLen - nOldPdsLen);

		pdsInfo->nLength +=length - nOldPdsLen;
		for ( i = nIndice+1; i < MAX_PULSE_PDS; i++)
		{
			if ( pdsInfo->nPresent[i] == PRESENT)
				pdsInfo->nPosPds[i] += length - nOldPdsLen;
		}
	}
	return(SUCCES);
}

int PulseBuildPds (char * sPdsBuffer, TSPdsPulse *pdsInfo)
{
	int i;
	fprintf ( stderr,"\n Data Avant le Build : ");
	for ( i = 0 ; i < pdsInfo->nLength ; i ++ )
		fprintf ( stderr,"%02X ", pdsInfo->sPdsData[i] );

	for ( i = 0 ; i <4 ; i++ ) sPdsBuffer [ i ] = 0x40;
	memcpy ( sPdsBuffer + 4 , pdsInfo->sPdsData , pdsInfo->nLength);

	return(pdsInfo->nLength + 4);
}




void  InitPulseDataInfo(TSPdInfo *pdInfo)
{
	int i;

	for (i=0; i < MAX_PD_FIELDS + 1; i++)
		pdInfo->nFieldPos[i] = 0;
	for (i=0; i < PD_BITMAP_LEN; i++)
		pdInfo->sBitMap[i] = 0x00;
	pdInfo->nLength = 0;
	memset(pdInfo->sData, '\0', MAX_PD_DATA_LEN);
}

int AddPdSubField(int field_no, TSPdInfo *pdInfo, char *data, int nLength)
{
	int nBytes  = 0;
	char sLine[MAX_LINE_TRC];

	if ( CheckBit( pdInfo->sBitMap, field_no))
	{
		trace_event( " AddPdSubField.. Erreur CheckBit", PROCESSING);
		return(ECHEC);
	}
	pdInfo->nFieldPos[field_no] = pdInfo->nLength;

	switch (GetPdFieldType(field_no))
	{
	case PD_FIX_BIN :
		nBytes = GetPdFieldLength(field_no);
		memcpy( pdInfo->sData + pdInfo->nLength, data, nBytes);
		break;

	case PD_FIX_BCD :
		if ( GetPdFieldLength(field_no) != nLength)
			return(ECHEC);
		nBytes = AsciiToBcd( data, pdInfo->sData + pdInfo->nLength, nLength);
		break;

	case PD_FIX_HEX :
		if ( GetPdFieldLength(field_no) != nLength)
			return(ECHEC);
		nBytes = AsciiToHex( data, pdInfo->sData + pdInfo->nLength, nLength);
		break;

	case PD_FIX_ALPHA :
		trace_event ( " PD_FIX_ALPHA ", PROCESSING);
		if ( GetPdFieldLength(field_no) != nLength)
		{
			trace_event( " Erreur ...GetPdFieldLength", PROCESSING);
			return(ECHEC);
		}
		AsciiToEbcdic( data, pdInfo->sData + pdInfo->nLength, nLength);
		nBytes = nLength;
		break;

	case PD_VAR_BCD :
		*(pdInfo->sData + pdInfo->nLength) = nLength;
		pdInfo->nLength++;
		nBytes = AsciiToBcd( data, pdInfo->sData + pdInfo->nLength, nLength);
		break;

	case PD_VAR_ALPHA :
		*(pdInfo->sData + pdInfo->nLength) = nLength;
		pdInfo->nLength++;
		AsciiToEbcdic(data, pdInfo->sData + pdInfo->nLength, nLength);
		nBytes = nLength;
		break;

	default :
		return(ECHEC);
	}
	pdInfo->nLength += nBytes;
	SetBit(pdInfo->sBitMap, field_no + 1);
	return(SUCCES);
}

int GetPdSubField(int field_no, TSPdInfo *pdInfo, char *data, int *nLength)
{
	int length;
	int nBytes;

	if (CheckBit (pdInfo->sBitMap, field_no))
	{
		switch (GetPdFieldType(field_no))
		{
		case PD_FIX_BIN :
			length = GetPdFieldLength(field_no);
			memcpy( data, pdInfo->sData + pdInfo->nFieldPos[field_no], length);
			*nLength = length;
			break;

		case PD_FIX_BCD :                           
			length = GetPdFieldLength(field_no);
			nBytes = BcdToAscii(pdInfo->sData+pdInfo->nFieldPos[field_no], 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case PD_VAR_BCD :                           
			length = *(pdInfo->sData + pdInfo->nFieldPos[field_no]);
			nBytes = BcdToAscii(pdInfo->sData+pdInfo->nFieldPos[field_no]+1, 
				data, length); 
			data[length] = '\0';
			*nLength = length;
			break;

		case PD_FIX_ALPHA :                           
			length = GetPdFieldLength(field_no);
			/*EbcdicToAscii(pdInfo->sData+pdInfo->nFieldPos[field_no], 
			data, length); 
			data[length] = '\0';
			*nLength = length;
			break;*/
			/*AsciiToEbcdic( pdInfo->sData+pdInfo->nFieldPos[field_no],
			data, length);*/
			memcpy (data, pdInfo->sData+pdInfo->nFieldPos[field_no], length);
			data[length] = '\0';
			*nLength = length;
			break;

		case PD_VAR_ALPHA :                           
			length = *(pdInfo->sData + pdInfo->nFieldPos[field_no]);
			EbcdicToAscii(pdInfo->sData+pdInfo->nFieldPos[field_no]+1, 
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

int PdBuildField (char * buffer_pd, TSPdInfo *pdInfo)
{
	char tmp [ 2 + 1];  

	memcpy( buffer_pd, "80", 2);  
	memcpy( buffer_pd + 2, pdInfo->sData, pdInfo->nLength);
	return( 2 + pdInfo->nLength);
}

int  AddReasonCodeSubField (int field_no, TSPdInfo *pdInfo, char *data, int nLength)
{
	int nBytes  = 0;
	char sLine[MAX_LINE_TRC];

	if ( CheckBit( pdInfo->sBitMap, field_no)) {
		trace_event( " AddReasonCodeSubField.. Erreur CheckBit", PROCESSING);
		return(ECHEC);
	}

	pdInfo->nFieldPos[field_no] = pdInfo->nLength;

	if (GetReasonCodeFieldLength(field_no) != nLength) {
		trace_event( " Erreur ...GetReasonCodeFieldLength", PROCESSING);
		return(ECHEC);
	}     
	switch (GetReasonCodeFieldType(field_no))
	{
	case PD_FIX_BIN :
		nBytes = GetReasonCodeFieldLength(field_no);
		memcpy( pdInfo->sData + pdInfo->nLength, data, nBytes);
		break;

	case PD_FIX_BCD :
		nBytes = AsciiToBcd( data, pdInfo->sData + pdInfo->nLength, nLength);
		break;

	case PD_FIX_HEX :
		nBytes = AsciiToHex( data, pdInfo->sData + pdInfo->nLength, nLength);
		break;

	case PD_FIX_ALPHA :
		trace_event ( " PD_FIX_ALPHA ", PROCESSING);
		AsciiToEbcdic( data, pdInfo->sData + pdInfo->nLength, nLength);
		nBytes = nLength;
		break;

	case PD_VAR_BCD :
		*(pdInfo->sData + pdInfo->nLength) = nLength;
		pdInfo->nLength++;
		nBytes = AsciiToBcd( data, pdInfo->sData + pdInfo->nLength, nLength);
		break;

	case PD_VAR_ALPHA :
		*(pdInfo->sData + pdInfo->nLength) = nLength;
		pdInfo->nLength++;
		AsciiToEbcdic(data, pdInfo->sData + pdInfo->nLength, nLength);
		nBytes = nLength;
		break;

	default :
		return(ECHEC);
	}
	pdInfo->nLength += nBytes;
	SetBit(pdInfo->sBitMap, field_no + 1);
	return(SUCCES);
}

int ReversalBuildField (char * buffer_pd, TSPdInfo *pdInfo)
{

	char tmp [ 2 + 1];

	/*YDA20180228*/
	sprintf( buffer_pd, "%02X", /*(unsigned int)*/(unsigned char *)pdInfo->sBitMap);

	memcpy( buffer_pd + 2, pdInfo->sData, pdInfo->nLength);
	return( 2 + pdInfo->nLength);

}
