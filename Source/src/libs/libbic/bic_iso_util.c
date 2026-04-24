#include <stdio.h>
#include <stdlib.h>

#include <iso_bic.h>
#include <tools.h>
#include <define.h>

/*************************************************************************/
int GetBicIccField(int field_no, TSBicIccInfo * msgInfo, char *data, int *nLength)
{
	int length;
	int nBytes;
	char szBicFldLen[4];	

	memset( szBicFldLen, 0, 4);	
	if (CheckBit (msgInfo->sBitMap, field_no))
	{
		switch (GetBicIccFieldType(field_no))
		{

		case BIC_TAG_FIX_BIN   :
		case BIC_TAG_FIX_ALPHA :
			length = GetBicIccFieldLength(field_no);
			memcpy( data, msgInfo->sData + msgInfo->nFieldPos[field_no], length);
			data[length] = '\0';
			*nLength = length;
			break;

		case BIC_TAG_FIX_HEX :                           
			length = GetBicIccFieldLength(field_no);
			nBytes = HexToAscii( msgInfo->sData+msgInfo->nFieldPos[field_no], 
				data, length);
			data[length] = '\0';
			*nLength = length;
			break;

		case BIC_TAG_VAR_HEX :                           
			memcpy ( szBicFldLen, msgInfo->sData+msgInfo->nFieldPos[field_no], 3);
			length = atoi(szBicFldLen);
			nBytes = HexToAscii(msgInfo->sData+msgInfo->nFieldPos[field_no]+3, 
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

int AddBicIccField(int field_no, TSBicIccInfo *msgInfo, char *data, int nLength)
{
	int nBytes  = 0;
	int i;
	char szBicFldLen[4];	
	char  sLine [MAX_LINE_TRC];

	memset( szBicFldLen, 0, 4);	
	if ( CheckBit( msgInfo->sBitMap, field_no))
		return(ECHEC);

	msgInfo->nFieldPos[field_no] = msgInfo->nLength;
	switch (GetBicIccFieldType(field_no))
	{
	case BIC_TAG_FIX_ALPHA :
		if ( GetBicIccFieldLength(field_no) != nLength)
			return(ECHEC);
		memcpy( msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;


	case BIC_TAG_FIX_HEX :
		if ( GetBicIccFieldLength(field_no) != nLength)
			return(ECHEC);
		nBytes = AsciiToHex(data, msgInfo->sData+msgInfo->nLength, nLength);
		break;

	case BIC_TAG_VAR_HEX :
		sprintf ( szBicFldLen, "%.2d", nLength);	
		memcpy( msgInfo->sData+msgInfo->nLength, szBicFldLen, 2);
		msgInfo->nLength += 2;
		nBytes = AsciiToHex( data, msgInfo->sData+msgInfo->nLength, nLength);
		SetBit(msgInfo->sBitMap, field_no+2);
		break;


	case BIC_TAG_FIX_BIN :
		if ( GetBicIccFieldLength(field_no) != nLength)
			return(ECHEC);
		memcpy(msgInfo->sData+msgInfo->nLength, data, nLength);
		nBytes = nLength;
		break;



	default :
		return(ECHEC);
	}
	msgInfo->nLength += nBytes;
	for (i=field_no; i< BIC_MAX_ICC_TAG; i++)
		msgInfo->nFieldPos[i+1] = msgInfo->nLength;
	SetBit(msgInfo->sBitMap, field_no+1);
	return(SUCCES);
}


int BicBuildIccMsg (char * buffer_snd, TSBicIccInfo *msgInfo)
{
	char sLine     [MAX_LINE_TRC];
	char sDest     [16+1];
	char sData     [LG_MAX];
	int  nBytes;
	int  iMapCtr;
	int  nLength;
	int  i,j,k;


	sprintf(sLine,"Start BicBuildIccMsg msgInfo->nLength(%d)", msgInfo->nLength);
	trace_event(sLine, PROCESSING);

	memset (sDest,    0, sizeof (sDest)); 
	HexToAscii(msgInfo->sBitMap, sDest, 4);

	memcpy( buffer_snd, sDest, 4);   
	memcpy( buffer_snd + 4  , msgInfo->sData, msgInfo->nLength); 



	return( 4 + msgInfo->nLength);
}
