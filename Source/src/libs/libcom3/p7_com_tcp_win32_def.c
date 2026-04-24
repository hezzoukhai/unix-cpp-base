#include <p7_common.h>
#include <p7_globals.h>
#include <p7_types.h>
#include <p7_com_prop.h>
#include <p7_com_tcp.h>

/*****************************************************************************/
int TCP_ReadLineMsg (TSTCPComInfo* pkTCPComInfo, int nConnIndex, char *sBuffer, int* nLength)
{
	char		sBuffRcv [ LG_MAX ];
	int			nLenMsg =0, nRead=0;
	int			nMul=1;
	int			i;
	char		szFieldData[256];
	char		szLength[128];
	int			nIndex;
	int			nLengthLen;
	int			nTotalLengthLen;
	int			nLengthOffset;
	int			nFstBlkSize;
	int			nToRead;
	int			nLenAdj;
	char*		sPtr;
	SOCKET*		pkSocket;


	memset (szFieldData, '\0', sizeof(szFieldData));
	memset (szLength, '\0', sizeof(szLength));

	nIndex = 0;
	if( pkTCPComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nIndex = nConnIndex;
	}
	else
	{
		nIndex = 0;
	}

	pkSocket = &pkTCPComInfo->tab_conn[nIndex].kSocket;


	nLengthLen = pkTCPComInfo->ComInfoProperties->nLengthLen;
	nTotalLengthLen = pkTCPComInfo->ComInfoProperties->nTotalLengthLen;
	nLengthOffset = pkTCPComInfo->ComInfoProperties->nLengthOffset;
	nFstBlkSize = nLengthOffset + nTotalLengthLen;


	do
    {
		nRead = recv (*pkSocket, szFieldData, nFstBlkSize , MSG_PEEK);

	}   while ((nRead != nFstBlkSize) && (nRead > 0));

    if (nRead < 0)
	{
		trace_message(P7_TL_FATAL,"recv_nok_hdr: errno: %u, %s",errno, strerror(errno));
		return NOK;
	}

    if (nRead == 0)
	{
		trace_message(P7_TL_FATAL,"disconnected: errno: %u, %s",errno, strerror(errno));
		return -2;
	}

	
	switch(pkTCPComInfo->ComInfoProperties->ePacketLengthType)
	{
	case CLTP_HIGH_ASCII:
		memcpy(szLength,szFieldData + nLengthOffset,nLengthLen);
		nLenMsg=atoi(szLength);
		break;
	case CLTP_LOW_ASCII:
		memcpy(szLength,szFieldData + nLengthOffset + (nTotalLengthLen - nLengthLen),nLengthLen);
		nLenMsg=atoi(szLength);
		break;
	case CLTP_HIGH_BINARY:
		nLenMsg =0;
		sPtr = szFieldData + nLengthOffset;
		for(i= nLengthLen - 1; i >= 0; i--)
		{
			nLenMsg += (unsigned char)sPtr[i]*nMul;
			nMul=nMul*256;
		}
		break;
	case CLTP_LOW_BINARY:
		nLenMsg =0;
		sPtr = szFieldData + nLengthOffset + (nTotalLengthLen - nLengthLen);
		for(i= nLengthLen - 1; i >= 0; i--)
		{
			nLenMsg += (unsigned char)sPtr[i]*nMul;
			nMul=nMul*256;
		}
		break;
	case CLTP_NO_LENGTH:
		nLenMsg =0;
		break;
	default:
		trace_message(P7_TL_ERROR,"--- Unknown Length type : %d",pkTCPComInfo->ComInfoProperties->ePacketLengthType);
		return NOK;
	}
	
	/*
	if( pkTCPComInfo->ComInfoProperties->bExcludeLength == NOK )
		nLenMsg -= nTotalLengthLen;
	*/

	/* The adjustment is how many bytes we should read next because of the "Length length"*/
	nLenAdj = 0;

	if( nLengthOffset == 0 )
	{
		if( pkTCPComInfo->ComInfoProperties->bExcludeLength == LDT_LENGTH_EXCLUDED )
		{
			nLenAdj = nTotalLengthLen;
		}
		else if( pkTCPComInfo->ComInfoProperties->bExcludeLength == LDT_LENGTH_INCLUDED )
		{
			nLenAdj = 0;
		}
	}

	nToRead = nLenMsg + nLenAdj;

	if (nToRead > LG_MAX)
	{
		trace_message(P7_TL_ERROR,"--- ReadLineMsg (ERROR) --- Msg too BIG : %d",nToRead);
		trace_message(P7_TL_ERROR,"recv_nok While Reading Message Data %d:%s", errno, strerror(errno));
		return NOK;
	}



	/*nRead = recv (*pkSocket, sBuffRcv, nLenMsg + nTotalLengthLen, MSG_WAITALL);*/
	nRead = recv (*pkSocket, sBuffRcv, nToRead, MSG_WAITALL);
    if (nRead < 0)
	{
        trace_message(P7_TL_ERROR,"recv_nok while reading message data %d:%s", errno, strerror(errno));
	}
    if (nRead == 0)
	{
        trace_message(P7_TL_ERROR, "recv_disc disconnect while reading message data %d:%s", errno, strerror(errno));
	}
	if( nRead != nToRead )
	{
		trace_message(P7_TL_ERROR, "Received less than expected %d VS %d", nRead, nToRead);
	}

	/* not always ascii, may be ebcdic */
	/*dump_buffer((unsigned char *) sBuffRcv, nLenMsg + pkTCPComInfo->ComInfoProperties->nLengthLen, 'A', 'L', 'I');*/
	
	dump_buffer(sBuffRcv, nRead, 'A', 'L', 'I');

	if( nLengthOffset > 0 )
	{
		memcpy ( sBuffer, sBuffRcv , nRead);
		(*nLength) = nRead;
	}
	else
	{
		memcpy ( sBuffer, sBuffRcv +  nTotalLengthLen  , nRead - nLenAdj);
		(*nLength) = nRead - nLenAdj;
	}

	
	trace_message(P7_TL_PROCESSING, "End   TCP_ReadLineMsg(%d)", (*nLength));
	return OK;

}
/*****************************************************************************/
int TCP_WriteLineMsg(TSTCPComInfo* pkTCPComInfo, int nConnIndex, const char *sBuffer, int nLength)
{
    char		sBuffSnd[ LG_MAX ];
	char		szFieldData[256];
	char		szLengthStr[256];
    int			nResult;
	int			i;
	int			nEffectiveLength;
	int			nDivFactor;
	int			nClientIndex;
	SOCKET*		pkSocket;
	int			nLengthLen;
	int			nTotalLengthLen;
	int			nLengthOffset;
	int			nFstBlkSize;
	char*		sPtr;
	int			nLenAdj;

	if( nLength > sizeof(sBuffSnd) - 64)
	{
		return NOK;
	}
    

	if( pkTCPComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nClientIndex = nConnIndex;
	}
	else
	{
		nClientIndex = 0;
	}

	pkSocket = &pkTCPComInfo->tab_conn[nClientIndex].kSocket;

	nLengthLen = pkTCPComInfo->ComInfoProperties->nLengthLen;
	nTotalLengthLen = pkTCPComInfo->ComInfoProperties->nTotalLengthLen;
	nLengthOffset = pkTCPComInfo->ComInfoProperties->nLengthOffset;
	nFstBlkSize = nLengthOffset + nTotalLengthLen;




	nEffectiveLength = nLength;
	/*
	if( pkTCPComInfo->ComInfoProperties->bExcludeLength == NOK )
		nEffectiveLength += nTotalLengthLen;
	*/

	/* The adjustment is how many bytes we should add to Length when sending it to partner*/
	nLenAdj = 0;
	if( nLengthOffset == 0 )
	{
		
		if( pkTCPComInfo->ComInfoProperties->bExcludeLength == LDT_LENGTH_EXCLUDED )
		{
			nLenAdj = 0;
		}
		else if( pkTCPComInfo->ComInfoProperties->bExcludeLength == LDT_LENGTH_INCLUDED )
		{
			nLenAdj = nTotalLengthLen;
		}
		nEffectiveLength += nLenAdj;
	}


	if( nLengthOffset > 0 )
	{
		memcpy (sBuffSnd , sBuffer, nLength);
	}
	else
	{
		memcpy (sBuffSnd + nTotalLengthLen , sBuffer, nLength);
	}


	switch(pkTCPComInfo->ComInfoProperties->ePacketLengthType)
	{
	case CLTP_HIGH_ASCII:
	case CLTP_LOW_ASCII:
		memset(szFieldData,'0',nTotalLengthLen);
		sprintf(szLengthStr,"%.*d",nLengthLen,nEffectiveLength);

		if( pkTCPComInfo->ComInfoProperties->ePacketLengthType == CLTP_HIGH_ASCII )
		{
			memcpy(szFieldData,szLengthStr,nLengthLen);
		}
		else
		{
			memcpy(szFieldData + (nTotalLengthLen - nLengthLen),szLengthStr,nLengthLen);
		}
		
		memcpy(sBuffSnd + nLengthOffset,szFieldData,nTotalLengthLen);
		break;
	case CLTP_HIGH_BINARY:
	case CLTP_LOW_BINARY:
		nDivFactor = pkTCPComInfo->ComInfoProperties->nDivFactor;
		for(i= 0 ; i < nLengthLen - 1 ; i++)
		{
			szFieldData [ i ] = nEffectiveLength/nDivFactor;
			nEffectiveLength = nEffectiveLength%nDivFactor;
			nDivFactor= (nDivFactor>>8);
		}
		szFieldData [ nLengthLen - 1] = nEffectiveLength % 256;
		memset(sBuffSnd + nLengthOffset,0,nTotalLengthLen);
		if( pkTCPComInfo->ComInfoProperties->ePacketLengthType == CLTP_HIGH_BINARY )
		{
			memcpy(sBuffSnd + nLengthOffset,szFieldData,nLengthLen);
		}
		else
		{
			memcpy(sBuffSnd + nLengthOffset + (nTotalLengthLen - nLengthLen),szFieldData,nLengthLen);
		}
		break;
	case CLTP_NO_LENGTH:
		break;
	default:
		trace_message(P7_TL_ERROR,"--- Unknown Length type : %d",pkTCPComInfo->ComInfoProperties->ePacketLengthType);
		return NOK;
	}

	if( nLengthOffset > 0 )
	{
		dump_buffer(sBuffSnd, nLength, 'A', 'L', 'O');
		nResult = send (*pkSocket, sBuffSnd, nLength ,0);
	}
	else
	{
		dump_buffer(sBuffSnd, nLength + nTotalLengthLen, 'A', 'L', 'O');
		nResult = send (*pkSocket, sBuffSnd, nLength + nTotalLengthLen ,0);
	}

    if (nResult <= 0)
	{
         trace_message(P7_TL_ERROR, "write error %d:%s", errno, strerror(errno));
	}
	
    return OK;

}
/******************************************************************************/

