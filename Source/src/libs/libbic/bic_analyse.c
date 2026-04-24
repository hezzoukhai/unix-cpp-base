#include <stdio.h>
#include <stdlib.h>
#include <iso_bic.h>
#include <tools.h>

int  AnalyseBic(char *buffer_rec, TSBicInfo *msgInfo)
{
	int  i,j,k;
	int  nDigits = 0;
	int  iMapCtr = 0;
	int  nBytes  = 0;
	int  nRcvHdrLen = 0;
	char szBicMsgType[5];	
	char szBicFldLen[4];	
	char *sPtr;
	char sWork      [32];
	char sLine      [MAX_LINE_TRC];

	char Binary_bit_map[16];

	memset(Binary_bit_map,    0, sizeof(Binary_bit_map));
	memset(sWork,             0, sizeof(sWork));

	InitBicInfo(msgInfo);
	sPtr = buffer_rec;

	memcpy((char *)&(msgInfo->sHeader), sPtr, BIC_HDR_LEN);
	sPtr += BIC_HDR_LEN;

	memset(szBicMsgType, 0,  5);
	memcpy(szBicMsgType, sPtr,  4);
	msgInfo->nMsgType = atoi(szBicMsgType);
	sPtr += 4;

	memcpy( sWork, sPtr, 32);

	/*EBETODO: ???*/
	if ( ((sWork[0]>= '8') && (sWork[0]<='9')) || ((sWork[0]>= 'A') && (sWork[0]<='F')) || ((sWork[0]>= 'a') && (sWork[0]<='f')) )
		AsciiToHex(sWork, Binary_bit_map,32);
	else
	{
		memset(sWork,  0, sizeof(sWork));
		memcpy( sWork, sPtr, 16);
		AsciiToHex(sWork, Binary_bit_map,16);
	}

	memcpy( msgInfo->sBitMap, Binary_bit_map, 16); 

	iMapCtr = 1;
	/** while ( ( Binary_bit_map[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) ) **/
	if ( Binary_bit_map[(iMapCtr - 1) * 8] & 0x80)
		++iMapCtr; 

	sPtr += iMapCtr*16;

	memcpy( msgInfo->sData, sPtr, MAX_BIC_DATA_LEN);


	for (i = 1; i < iMapCtr * 64; ++i)
	{
		memset (szBicFldLen, 0 ,4);	
		if ( CheckBit( msgInfo->sBitMap, i))
		{
			switch ( GetBicFieldType(i) )
			{
			case BIC_FIX_BIN   :
			case BIC_FIX_ALPHA :
				nBytes = GetBicFieldLength(i);
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BIC_FIX_HEX :                           
				nDigits = GetBicFieldLength(i);
				nBytes = ( nDigits + 1)/2;
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BIC_LL_HEX :                           
				memcpy (szBicFldLen, sPtr ,2);
				nDigits = atoi(szBicFldLen);
				nBytes = (nDigits + 1)/2 + 2; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BIC_LLL_HEX :                           
				memcpy (szBicFldLen, sPtr ,3);
				nDigits = atoi(szBicFldLen);
				nBytes = ( nDigits + 1)/2 +3; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BIC_LL_BIN : 
				memcpy (szBicFldLen, sPtr ,2);
				nBytes  = atoi(szBicFldLen);
				nBytes +=2; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BIC_LLL_BIN : 
				memcpy (szBicFldLen, sPtr ,3);
				nBytes  = atoi(szBicFldLen);
				nBytes +=3; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BIC_LL_ALPHA : 
				memcpy (szBicFldLen, sPtr ,2);
				nBytes  = atoi(szBicFldLen);
				nBytes +=2; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BIC_LLL_ALPHA : 
				memcpy (szBicFldLen, sPtr ,3);
				nBytes  = atoi(szBicFldLen);
				nBytes +=3; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case BIC_VAR_ALPHA:                           
				nBytes = *sPtr;                                     
				nBytes++;
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes ;
				break;

			default :
				nBytes =0;
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i];
				break;

			} 
			sPtr += nBytes;
		} 
		else
			msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i];
	} 
	msgInfo->nLength = msgInfo->nFieldPos[iMapCtr*64];
	for (i = iMapCtr*64; i< MAX_BIC_FIELDS; i++)
		msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
}


/** Start RHA Emv Mng **/
int AnalyseBicIc(char * buffer, int nLength, TSTagBic * tagInfo)
{
	char  *sPtr;
	int   offset;
	int   nTagLength, nBytes;
	int   nTagIndice;
	int   nLenTag;
	char  szTagName [ 5 ];


	InitBicIcTag(tagInfo);

	offset = 0;
	tagInfo->nLength = nLength;
	memcpy (tagInfo->sChipData,  buffer,  nLength);

	sPtr = buffer;

	while (offset < nLength)
	{
		memset( szTagName, '0', 4);
		if ( ((*sPtr) & 0x1F ) == 0x1F)
		{
			nBytes = HexToAscii( sPtr, szTagName, 4);
			nLenTag = 2;
		}
		else
		{
			nBytes = HexToAscii( sPtr, szTagName, 2);
			nLenTag = 1;
		}


		nTagIndice = GetBicTagIndice( szTagName);
		if ( nTagIndice >= 0)
		{
			tagInfo->nPosTag [ nTagIndice ] = offset;
			tagInfo->nPresent[ nTagIndice ] = PRESENT;
		}

		nTagLength =  nLenTag + 1 + (*(sPtr + nLenTag));
		offset += nTagLength;
		sPtr   += nTagLength;
	}
}

