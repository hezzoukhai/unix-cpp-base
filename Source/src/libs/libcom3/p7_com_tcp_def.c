#include <p7_common.h>
#include <p7_globals.h>
#include <p7_types.h>
#include <p7_com_prop.h>
#include <p7_com_tcp.h>

/******************************************************************************/
int TCP_ReadLineMsg (TSTCPComInfo* pkTCPComInfo, int nConnIndex, char *sBuffer, int* nLength)
{

	char		sBuffRcv [ LG_MAX ];
	char		szFieldData[256];
	int			nLenMsg =0, nRead=0;
	int			nFd;
	int			nMul=1;
	int			i;
	int			nIndex,nClientIndex;
	char		szLength[24];
	int			nLengthLen;
	int			nTotalLengthLen;
	int			nLengthOffset;
	int			nFstBlkSize;
	int			nToRead;
	int			nLenAdj;
	char*		sPtr;
	
	
	memset (szFieldData, '\0', sizeof(szFieldData));
	memset (szLength, '\0', sizeof(szLength));

	nIndex = 0;

	if( pkTCPComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nClientIndex = nConnIndex;
		
	}
	else
	{
		nClientIndex = 0;
	}

	nFd = pkTCPComInfo->tab_conn[nClientIndex].nFd;

	nLengthLen = pkTCPComInfo->ComInfoProperties->nLengthLen;
	nTotalLengthLen = pkTCPComInfo->ComInfoProperties->nTotalLengthLen;
	nLengthOffset = pkTCPComInfo->ComInfoProperties->nLengthOffset;
	nFstBlkSize = nLengthOffset + nTotalLengthLen;
	


	do
    {
		nRead = recv (nFd, szFieldData, nFstBlkSize , MSG_PEEK);
    }   while ((nRead != nFstBlkSize) && (nRead > 0));

    if (nRead < 0)
	{
		trace_message(P7_TL_ERROR,"recv_nok_hdr: errno: %u, %s",errno, strerror(errno));
		return NOK;
	}

    if (nRead == 0)
	{
		trace_message(P7_TL_ERROR,"disconnected: errno: %u, %s",errno, strerror(errno));
		return NOK;
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


    

	/*nRead = recv (nFd, sBuffRcv, nLenMsg + nTotalLengthLen, MSG_WAITALL);*/
	nRead = recv (nFd, sBuffRcv, nToRead, MSG_WAITALL);
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
	
	dump_buffer((unsigned char *) sBuffRcv, nRead, 'A', 'L', 'I');


	if( nLengthOffset > 0 )
	{
		memcpy ( sBuffer, sBuffRcv , nRead);
		(*nLength) = nRead;
	}
	else
	{
		/*EBE170410: we should remove the nTotalLengthLen not the adj
		memcpy ( sBuffer, sBuffRcv +  nTotalLengthLen  , nRead - nLenAdj);*/
		memcpy ( sBuffer, sBuffRcv +  nTotalLengthLen  , nRead - nTotalLengthLen);
		(*nLength) = nRead - nLenAdj;
	}

	return OK;

}
/******************************************************************************/
int TCP_WriteLineMsg(TSTCPComInfo* pkTCPComInfo, int nConnIndex, const char *sBuffer, int nLength)
{
    char		sBuffSnd[ LG_MAX ];
	char		szFieldData[256];
	char		szLengthStr[256];
    int			nResult;
    int			i;
	int			nFd;
	int			nIndex,nClientIndex;
	int			nEffectiveLength;
	int			nDivFactor;
	int			nLengthLen;
	int			nTotalLengthLen;
	int			nLengthOffset;
	int			nFstBlkSize;
	int			nLenAdj;
	char*		sPtr;
	
    

	if( nLength > sizeof(sBuffSnd) - 64)
	{
		return NOK;
	}
	
	nIndex = 0;
	if( pkTCPComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nClientIndex = nConnIndex;
	}
	else
	{
		nClientIndex = 0;
	}

	nFd= pkTCPComInfo->tab_conn[nClientIndex].nFd;

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
		nResult = write (nFd, sBuffSnd, nLength);
	}
	else
	{
		dump_buffer(sBuffSnd, nLength + nTotalLengthLen, 'A', 'L', 'O');
		nResult = write ( nFd, sBuffSnd, nLength + nTotalLengthLen);	
	}
    if (nResult <= 0)
	{
         trace_message(P7_TL_ERROR, "write error %d:%s", errno, strerror(errno));
	}
	
    return OK;

}
/******************************************************************************/

