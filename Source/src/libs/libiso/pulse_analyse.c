#include <stdio.h>
#include <stdlib.h>
#include <pulse/iso_pulse.h>
#include <tools.h>


int   AnalysePulse(char * buffer_rec, TSPulseInfo *msgInfo)
{
	int  i, nDigits = 0;
	int  iMapCtr = 0;
	int  nBytes  = 0;
	int  nFldType  , nFldLength;
	int  nRcvHdrLen = 0;
	char szPulseMsgType[5];	
	char szPulseFldLen[4];	
	char szAscFldLen[4];	
	char *sPtr;

	InitPulseInfo(msgInfo);
	sPtr = buffer_rec;


	memset(szPulseMsgType, '\0',  sizeof(szPulseMsgType));
	EbcdicToAscii(sPtr, szPulseMsgType,  4);
	msgInfo->nMsgType = atoi(szPulseMsgType);
	sPtr += 4;

	iMapCtr = 1;
	while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
		++iMapCtr;

	memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
	sPtr += (iMapCtr * 8);

	memcpy( msgInfo->sData, sPtr, MAX_PULSE_DATA_LEN);

	for (i = 0; i < iMapCtr * 64; ++i)
	{
		memset (szPulseFldLen, '\0' ,sizeof(szPulseFldLen));	
		memset (szAscFldLen, '\0' ,sizeof(szAscFldLen));

		if ( CheckBit( msgInfo->sBitMap, i))
		{

			nFldType   =  GetPulseFieldType(i);
			nFldLength =  GetPulseFieldLength(i);

			switch ( nFldType )
			{
			case PULSE_FIX_BIN   :
			case PULSE_FIX_ALPHA :
				nBytes = nFldLength;
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case PULSE_FIX_HEX :                           
				nDigits = nFldLength;
				nBytes = ( nDigits + 1)/2;
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case PULSE_LL_HEX :                           
				memcpy (szPulseFldLen, sPtr ,2);
				EbcdicToAscii(szPulseFldLen, szAscFldLen, 2);
				nDigits = atoi(szAscFldLen);
				nBytes = (nDigits + 1)/2 + 2; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case PULSE_LLL_HEX :                           
				memcpy (szPulseFldLen, sPtr ,3);
				EbcdicToAscii(szPulseFldLen, szAscFldLen, 3);
				nDigits = atoi(szAscFldLen);
				nBytes = ( nDigits + 1)/2 +3; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case PULSE_LL_BIN : 
				memcpy (szPulseFldLen, sPtr ,2);
				EbcdicToAscii(szPulseFldLen, szAscFldLen, 2);
				nBytes  = atoi(szAscFldLen);
				nBytes +=2; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case PULSE_LLL_BIN : 
				memcpy (szPulseFldLen, sPtr ,3);
				EbcdicToAscii(szPulseFldLen, szAscFldLen, 3);
				nBytes  = atoi(szAscFldLen);
				nBytes +=3; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case PULSE_LL_ALPHA : 
				memcpy (szPulseFldLen, sPtr ,2);
				EbcdicToAscii(szPulseFldLen, szAscFldLen, 2);
				nBytes  = atoi(szAscFldLen);
				nBytes +=2; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case PULSE_LLL_ALPHA : 
				memcpy (szPulseFldLen, sPtr ,3);
				EbcdicToAscii(szPulseFldLen, szAscFldLen, 3);
				nBytes  = atoi(szAscFldLen);
				nBytes +=3; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
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
	for (i = iMapCtr*64; i< MAX_PULSE_FIELDS; i++)
		msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];
}

int AnalysePulseIc(char * buffer, int nLength, TSTagPulse * tagInfo)
{
	char  *sPtr;
	int   offset;
	int   nTagLength, nBytes;
	int   nTagIndice;
	int   nLenTag; 
	char  szTagName [ 5 ];

	InitPulseIcTag(tagInfo);

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

		nTagIndice = GetPulseTagIndice( szTagName);
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

int AnalysePulsePds(char *buffer, int nLength, int nMsgType, TSPdsPulse *pdsInfo)
{
	char  *sPtr;
	int   offset;
	int   nPdsLength, nBytes;
	int   nPdsIndice;
	int   nLenPds; 
	char  szPdsName [ 5 ];
	char  szPdsValueLen [ 5 ];
	int   nPdsValueLen ;

	InitPulsePds(pdsInfo);
	pdsInfo->nMsgType = nMsgType;

	offset = 0;
	pdsInfo->nLength = nLength;
	memcpy (pdsInfo->sPdsData,  buffer,  nLength);

	sPtr = buffer;

	while (offset < nLength)
	{
		memset( szPdsName,     '0', 4);
		memset( szPdsValueLen,  0 , 4);

		EbcdicToAscii( sPtr+2, szPdsName,    2);
		EbcdicToAscii( sPtr, szPdsValueLen,  2);
		nPdsValueLen = atoi(szPdsValueLen)-2;
		nLenPds = 2;
		nPdsIndice = GetPulsePdsIndice( szPdsName); 


		if ( nPdsIndice >= 0)
		{
			pdsInfo->nPosPds [ nPdsIndice ] = offset;
			pdsInfo->nPresent[ nPdsIndice ] = PRESENT;
		}

		nPdsLength =  nLenPds + nLenPds +  nPdsValueLen;
		offset += nPdsLength;
		sPtr   += nPdsLength;
	}
}

