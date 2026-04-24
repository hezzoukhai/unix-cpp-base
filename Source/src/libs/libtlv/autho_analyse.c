#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <define.h>
#include <tlv_autho.h>

int AnalyseTlvAutho(char *buffer, TSTlvAutho *tlvInfo)
{
	char  *sPtr;
	int   offset;
	int   nTlvLength, nBytes;
	int   nTlvIndice;
	int   nLenTlv; 
	char  szTlvName [ 5 ];
	char  szTlvValueLen [ 5 ];
	int   nTlvValueLen ;
	char  sLLLL [ 4 + 1];
	char  sLine[MAX_LINE_TRC];
	int   nLLLL;
	int	  nRetCode;
	int   tab_nPresent  [ MAX_AUTHO_TLV ];

	memset ( sLLLL ,          	0 , sizeof ( sLLLL ) );
	memset ( szTlvName ,        0 , sizeof ( szTlvName ) );
	memset ( szTlvValueLen ,    0 , sizeof ( szTlvValueLen ) );
	memset ( (char*)&tab_nPresent , 0 , sizeof ( tab_nPresent ) );
	InitTlvAutho(tlvInfo);
	nRetCode = OK;
	
	memcpy ( sLLLL , buffer 	 , 4 );
	nLLLL   = atoi ( sLLLL );
	nLenTlv = strlen(buffer) - 4;
	if( nLenTlv != nLLLL )
	{
		sprintf(sLine,">>> Invalid TLV length %d vs strlen=%d", nLLLL,nLenTlv);
		trace_event(sLine, ERROR );
		nRetCode = NOK;
		dump_raw_tlv_buffer(buffer);
	}
	

	offset = 0;
	tlvInfo->nLength = nLLLL;
	memcpy (tlvInfo->sTlvData,  buffer + 4 ,  nLLLL);

	sPtr = buffer+4;

	while (offset < nLLLL)
	{
		
		memcpy( szTlvName,     sPtr,       3);
		memcpy( szTlvValueLen, sPtr+3,     3);
		if( !( 	(szTlvValueLen[0] >= '0' && szTlvValueLen[0] <= '9') && 
				(szTlvValueLen[1] >= '0' && szTlvValueLen[1] <= '9') && 
				(szTlvValueLen[2] >= '0' && szTlvValueLen[2] <= '9') ))
		{
			sprintf(sLine,">>> Invalid encoded tag length tag:%s, len:%s(offset:%d)", szTlvName,szTlvValueLen,offset);
			trace_event(sLine, ERROR );
			dump_raw_tlv_buffer(buffer);
			nRetCode = NOK;
			return NOK;
		}

		nTlvValueLen = atoi(szTlvValueLen);
		nTlvIndice   = GetTlvAuthoIndice( szTlvName); 

		if ( nTlvIndice >= 0)
		{
			
			if(tlvInfo->nPresent[ nTlvIndice ] == PRESENT)
			{
				sprintf(sLine,">>> Tag is present more than once tag:%s(offset:%d)", szTlvName,offset);
				trace_event(sLine, ERROR );
				dump_raw_tlv_buffer(buffer);
				nRetCode = NOK;
			}
			else
			{
				tlvInfo->nPosTlv [ nTlvIndice ] = offset;
				tlvInfo->nPresent[ nTlvIndice ] = PRESENT;
			}
		}
		else
		{
			sprintf(sLine,">>> Unkown tag:%s(offset:%d)", szTlvName,offset);
			trace_event(sLine, ERROR );
			dump_raw_tlv_buffer(buffer);
			nRetCode = NOK;
		}

		nTlvLength =  3 + 3 +  nTlvValueLen;
		offset += nTlvLength;
		sPtr   += nTlvLength;
	}
	
	
	if( offset != nLLLL )
	{
		sprintf(sLine,">>> Offset exceeded length: len: %d vs offset=%d", nLLLL,offset);
		trace_event(sLine, ERROR );
		nRetCode = NOK;
		dump_raw_tlv_buffer(buffer);
	}
	
	return(nRetCode);
}


int ValidateTlvAutho(char *buffer, TSTlvAutho *tlvInfo)
{
    return AnalyseTlvAutho(buffer,tlvInfo);
}