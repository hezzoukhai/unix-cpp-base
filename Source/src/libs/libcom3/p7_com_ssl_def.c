#include <p7_common.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <p7_types.h>
#include <p7_com_prop.h>
#include <p7_com_ssl.h>



/******************************************************************************/
int SSL_ReadLineMsg (TSSSLComInfo*	pkSSLComInfo, int nConnIndex, char *sBuffer, int* nLength)
{
	char		sBuffRcv[ LG_MAX ];
	char		sLine[ MAX_LINE_TRC ];
	char		szFieldData[256];
	char		szLength[128];
	int			nRead;
	int			nLenMsg;
	int			nIndex,nClientIndex;
	BIO*		conn_bio;
	int			i;
	int			nMul=1;
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
	if( pkSSLComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nClientIndex = nConnIndex;
		conn_bio = pkSSLComInfo->tab_conn[nClientIndex].conn_bio;
	}
	else
	{
		conn_bio = pkSSLComInfo->root_bio;
	}
   

	trace_event("Start ReadLineMsgSSL()", P7_TL_PROCESSING);
	
 
	

	/*TODO: TO check is it needed?*/
	if(BIO_do_handshake(conn_bio) <= 0) 
	{
		trace_event("Error in SSL handshake",P7_TL_ERROR);
		return (NOK);
	}

	nLengthLen = pkSSLComInfo->ComInfoProperties->nLengthLen;
	nTotalLengthLen = pkSSLComInfo->ComInfoProperties->nTotalLengthLen;
	nLengthOffset = pkSSLComInfo->ComInfoProperties->nLengthOffset;
	nFstBlkSize = nLengthOffset + nTotalLengthLen;
	
	do
    {
		nRead = BIO_read(conn_bio,sBuffRcv,nFstBlkSize);
    }   while ((nRead != nFstBlkSize) && (nRead > 0));

    if (nRead < 0)
	{
		trace_message(P7_TL_ERROR,"recv_nok_hdr: errno: %u, %s",errno, strerror(errno));
		return NOK;
	}

    if (nRead == 0)
	{
		trace_message(P7_TL_ERROR,"disconnected: errno: %u, %s",errno, strerror(errno));
		return NOK; /* DISCONNECT */
	}


	switch(pkSSLComInfo->ComInfoProperties->ePacketLengthType)
	{
	case CLTP_HIGH_ASCII:
		memcpy(szLength,sBuffRcv + nLengthOffset,nLengthLen);
		nLenMsg=atoi(szLength);
		break;
	case CLTP_LOW_ASCII:
		memcpy(szLength,sBuffRcv + nLengthOffset + (nTotalLengthLen - nLengthLen),nLengthLen);
		nLenMsg=atoi(szLength);
		break;
	case CLTP_HIGH_BINARY:
		nLenMsg =0;
		sPtr = sBuffRcv + nLengthOffset;
		for(i= nLengthLen - 1; i >= 0; i--)
		{
			nLenMsg += (unsigned char)sPtr[i]*nMul;
			nMul=nMul*256;
		}
		break;
	case CLTP_LOW_BINARY:
		nLenMsg =0;
		sPtr = sBuffRcv + nLengthOffset + (nTotalLengthLen - nLengthLen);
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
		trace_message(P7_TL_ERROR,"--- Unknown Length type : %d",pkSSLComInfo->ComInfoProperties->ePacketLengthType);
		return NOK;
	}

	/*
	if( pkSSLComInfo->ComInfoProperties->bExcludeLength == NOK)
		nLenMsg -= nTotalLengthLen;
	*/
	/* The adjustment is how many bytes we should read next because of the "Length length"*/
	nLenAdj = 0;
	if( nLengthOffset == 0 )
	{
		if( pkSSLComInfo->ComInfoProperties->bExcludeLength == LDT_LENGTH_EXCLUDED )
		{
			nLenAdj = 0;
		}
		else if( pkSSLComInfo->ComInfoProperties->bExcludeLength == LDT_LENGTH_INCLUDED )
		{
			nLenAdj = nTotalLengthLen;
		}
	}
	nToRead = nLenMsg - nLenAdj;
	
	if (nToRead > LG_MAX)
	{
		trace_message(P7_TL_ERROR,"--- ReadLineMsg (ERROR) --- Msg too BIG : %d",nToRead);
		trace_message(P7_TL_ERROR,"recv_nok While Reading Message Data %d:%s", errno, strerror(errno));
		return NOK;
	}


	nRead = BIO_read(conn_bio,sBuffRcv + nFstBlkSize,nToRead );


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
	
	dump_buffer((unsigned char *) sBuffRcv, nRead + nFstBlkSize, 'A', 'L', 'I');

	if( nLengthOffset > 0 )
	{
		memcpy ( sBuffer, sBuffRcv , nRead + nFstBlkSize);
		(*nLength) = nRead + nFstBlkSize;
	}
	else
	{
		memcpy ( sBuffer, sBuffRcv +  nTotalLengthLen  , nLenMsg + nLenAdj);
		(*nLength) = nLenMsg + nLenAdj;
	}

	trace_event("End ReadLineMsgSSL(OK)",P7_TL_PROCESSING);
         
	return OK;

}
/******************************************************************************/
int SSL_WriteLineMsg(TSSSLComInfo*	pkSSLComInfo, int nConnIndex, const char *sBuffer, int nLength)
{
    char		sBuffSnd[ LG_MAX ];
	char		szFieldData[256];
	char		szLengthStr[256];
    int			nResult;
	int			i;
	int			nEffectiveLength;
	char		szLengthFormatString[24];
	int			nDivFactor;
	int			nClientIndex;
	BIO*		conn_bio;
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
	
	memset (szLengthFormatString, '\0', sizeof(szLengthFormatString));

	
	if( pkSSLComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nClientIndex = nConnIndex;
		conn_bio = pkSSLComInfo->tab_conn[nClientIndex].conn_bio;
	}
	else
	{
		conn_bio = pkSSLComInfo->root_bio;
	}

	
	
	nLengthLen = pkSSLComInfo->ComInfoProperties->nLengthLen;
	nTotalLengthLen = pkSSLComInfo->ComInfoProperties->nTotalLengthLen;
	nLengthOffset = pkSSLComInfo->ComInfoProperties->nLengthOffset;
	nFstBlkSize = nLengthOffset + nTotalLengthLen;
	
	nEffectiveLength = nLength;
	
	/*
	if( pkSSLComInfo->ComInfoProperties->bExcludeLength == NOK)
		nEffectiveLength += nTotalLengthLen;
	*/
	/* The adjustment is how many bytes we should add to Length when sending it to partner*/
	
	nLenAdj = 0;
	if( nLengthOffset == 0 )
	{
		if( pkSSLComInfo->ComInfoProperties->bExcludeLength == LDT_LENGTH_EXCLUDED )
		{
			nLenAdj = 0;
		}
		else if( pkSSLComInfo->ComInfoProperties->bExcludeLength == LDT_LENGTH_INCLUDED )
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
	
	
	switch(pkSSLComInfo->ComInfoProperties->ePacketLengthType)
	{
	case CLTP_HIGH_ASCII:
	case CLTP_LOW_ASCII:
		memset(szFieldData,'0',nTotalLengthLen);
		sprintf(szLengthStr,"%.*d",nLengthLen,nEffectiveLength);

		if( pkSSLComInfo->ComInfoProperties->ePacketLengthType == CLTP_HIGH_ASCII )
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
		nDivFactor = pkSSLComInfo->ComInfoProperties->nDivFactor;
		for(i= 0 ; i < nLengthLen - 1 ; i++)
		{
			szFieldData [ i ] = nEffectiveLength/nDivFactor;
			nEffectiveLength = nEffectiveLength%nDivFactor;
			nDivFactor= (nDivFactor>>8);
		}
		szFieldData [ nLengthLen - 1] = nEffectiveLength % 256;
		memset(sBuffSnd + nLengthOffset,0,nTotalLengthLen);
		if( pkSSLComInfo->ComInfoProperties->ePacketLengthType == CLTP_HIGH_BINARY )
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
		trace_message(P7_TL_ERROR,"--- Unknown Length type : %d",pkSSLComInfo->ComInfoProperties->ePacketLengthType);
		return NOK;
	}

	if( nLengthOffset > 0 )
	{
		dump_buffer(sBuffSnd, nLength, 'A', 'L', 'O');
		nResult = BIO_write(conn_bio,sBuffSnd,nLength);
	}
	else
	{
		dump_buffer(sBuffSnd, nLength + nTotalLengthLen, 'A', 'L', 'O');
		nResult = BIO_write(conn_bio,sBuffSnd,nLength + nTotalLengthLen);	
	}
	BIO_flush(conn_bio);

    if (nResult <= 0)
	{
         trace_message(P7_TL_ERROR, "write error %d:%s", errno, strerror(errno));
	}
	
    return OK;
}
/******************************************************************************/
