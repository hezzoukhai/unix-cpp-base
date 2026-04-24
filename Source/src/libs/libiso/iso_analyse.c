#include <stdio.h>
#include <stdlib.h>
#include <iso_hps.h>
#include <tools.h>

int   AnalyseIso(char * buffer_rec, TSIsoInfo *msgInfo)
{
	int  i, nDigits = 0;
	int  iMapCtr = 0;
	int  nBytes  = 0;
	int  nRcvHdrLen = 0;
	char szIsoMsgType[5];	
	char szIsoFldLen[4];	
	char *sPtr;

	InitIsoInfo(msgInfo);
	sPtr = buffer_rec;


	memset(szIsoMsgType, '\0',  5);
	memcpy(szIsoMsgType, sPtr,  4);
	msgInfo->nMsgType = atoi(szIsoMsgType);
	sPtr += 4;

	iMapCtr = 1;
	while ( ( sPtr[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 2) )
		++iMapCtr;
	memcpy( msgInfo->sBitMap, sPtr, iMapCtr*8);
	sPtr += (iMapCtr * 8);

	/* memcpy( msgInfo->sData, sPtr, MAX_ISO_DATA_LEN); */
	memcpy( msgInfo->sData, sPtr, MAX_ISO_DATA_LEN - ((iMapCtr*8) + 4 ));

	for (i = 0; i < iMapCtr * 64; ++i)
	{
		memset (szIsoFldLen, '\0' ,4);	
		if ( CheckBit( msgInfo->sBitMap, i))
		{
			switch ( GetIsoFieldType(i) )
			{
			case ISO_FIX_BIN   :
			case ISO_FIX_ALPHA :
				nBytes = GetIsoFieldLength(i);
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case ISO_FIX_HEX :                           
				nDigits = GetIsoFieldLength(i);
				nBytes = ( nDigits + 1)/2;
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case ISO_LL_HEX :                           
				memcpy (szIsoFldLen, sPtr ,2);
				nDigits = atoi(szIsoFldLen);
				nBytes = (nDigits + 1)/2 + 2; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case ISO_LLL_HEX :                           
				memcpy (szIsoFldLen, sPtr ,3);
				nDigits = atoi(szIsoFldLen);
				nBytes = ( nDigits + 1)/2 +3; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case ISO_LL_BIN : 
				memcpy (szIsoFldLen, sPtr ,2);
				nBytes  = atoi(szIsoFldLen);
				nBytes +=2; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case ISO_LLL_BIN : 
				memcpy (szIsoFldLen, sPtr ,3);
				nBytes  = atoi(szIsoFldLen);
				nBytes +=3; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case ISO_LL_ALPHA : 
				memcpy (szIsoFldLen, sPtr ,2);
				nBytes  = atoi(szIsoFldLen);
				nBytes +=2; 
				msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[i] + nBytes;
				break;

			case ISO_LLL_ALPHA : 
				memcpy (szIsoFldLen, sPtr ,3);
				nBytes  = atoi(szIsoFldLen);
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
	for (i = iMapCtr*64; i< MAX_ISO_FIELDS; i++)
		msgInfo->nFieldPos[i+1] = msgInfo->nFieldPos[iMapCtr*64];

	/*SKO290509*/ return(0);
}

int AnalyseIsoIc(char * buffer, int nLength, TSTagIso * tagInfo)
{
	char  *sPtr;
	int   offset;
	int   nTagLength, nBytes;
	int   nTagIndice;
	int   nLenTag;
	char  szTagName [ 5 ];
	char  sLine[MAX_LINE_TRC];

	memset(szTagName, 0, sizeof(szTagName)); /*MKB 07/09/06 - Added by YK16062006*/

	InitIsoIcTag(tagInfo);

	offset = 0;
	tagInfo->nLength = nLength;
	memcpy (tagInfo->sChipData,  buffer,  nLength);

	sPtr = buffer;


	while (offset < nLength)
	{
		memset( szTagName, '0', 4);
		szTagName[4] = '\0';


		/* if ( ((*sPtr) & 0x1F ) == 0x1F) * MMJ050109 */
		if (((unsigned char) ((*sPtr) & 0x1F )) == (unsigned char) 0x1F)      	
		{
			nBytes = HexToAscii( sPtr, szTagName, 4);
			nLenTag = 2;
		}
		else
		{
			nBytes = HexToAscii( sPtr, szTagName, 2);
			nLenTag = 1;
		}


		nTagIndice = GetIsoTagIndice( szTagName);
		if ( nTagIndice >= 0)
		{
			tagInfo->nPosTag [ nTagIndice ] = offset;
			tagInfo->nPresent[ nTagIndice ] = PRESENT;
		}

		nTagLength =  nLenTag + 1 + ((unsigned char)(*(sPtr + nLenTag))); /* YHM/MKB 20170418_PROD00041342 */	
		offset += nTagLength;
		sPtr   += nTagLength;
	}


	return OK;
}
