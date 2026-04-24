#include <define.h>
#include <stdio.h>
#include <string.h>
#include <tlv_switch_header.h>

typedef struct tlv_info_st
{
	char*   sTlvName;
	int     nLength;
	char*   description;
}tlv_info_type;


static tlv_info_type tab_TlvSwitchHeader[] =
{
	{ISW_MSG_ID, 		12,		"ISW_MSG_ID" 			},
	{ISW_CHANNEL_ID, 	2,		"MMF_TAG_MSG_TYPE" 		},
	{ISW_CONNEXION_ID, 	2,		"ISW_CONNEXION_ID" 		},
}
;


void InitTlvSwitchHeader(TSTlvSwitchHeader * tlvInfo) 
{
	int 	i = 0;
	int		n = sizeof(tlvInfo->nPosTlv)/sizeof(tlvInfo->nPosTlv[0]);
	for (i = 0; i < n; i++) 
	{
		tlvInfo->nPosTlv [i] = 0;
		tlvInfo->nPresent[i] = NOT_PRESENT;
	}
	tlvInfo->nLength = 0;
	memset(tlvInfo->sTlvData, '\0', MAX_SW_HEADER_DATA_LEN);

}

void AdjustTlvSwitchHeader (TSTlvSwitchHeader * tlvInfo, int nIndice, int nLength) 
{
   int i;
   int		n = sizeof(tlvInfo->nPosTlv)/sizeof(tlvInfo->nPosTlv[0]);
   for (i = 0; i < n; i++)
   {
       if ( tlvInfo->nPresent[i] == PRESENT && tlvInfo->nPosTlv [i] > tlvInfo->nPosTlv [nIndice] )
       {
            tlvInfo->nPosTlv [i]  += nLength;
       }
   }


}
int GetTlvSwitchHeaderIndice ( char * szTlvName) 
{
	int nIndice, nSize;
	nSize = sizeof(tab_TlvSwitchHeader)/sizeof(tab_TlvSwitchHeader[0]);
	nIndice = 0;
	do 
	{
		if (memcmp(szTlvName, tab_TlvSwitchHeader[nIndice].sTlvName, 3) == 0) 
			return(nIndice);
		nIndice++;
	}
	while (nIndice < nSize);
	return(-1);

}

int GetTlvSwitchHeaderLength ( int nIndice ) 
{
	return ( tab_TlvSwitchHeader[nIndice].nLength);

}
const char* GetTlvSwitchHeaderName ( int nIndice ) 
{
	return ( tab_TlvSwitchHeader[nIndice].sTlvName);

}
int ValidateTlvSwitchHeaderData (char *pTlvBuffer) 
{
	int nIndice, nSize,length,nRetCode;
	char sLine[MAX_LINE_TRC];
	char tlv_name[256];
	char data[LG_MAX];
	char sLLLL [ 4 + 1];
	int nLLLL;
	TSTlvSwitchHeader tlvInfo;
	
	trace_event("Start ValidateTlvSwitchHeaderData()",PROCESSING);
	
	InitTlvSwitchHeader ( &tlvInfo );
	memset ( sLLLL , 0 , sizeof ( sLLLL ) );
	memcpy ( sLLLL , pTlvBuffer , 4 );
	nLLLL = atoi ( sLLLL );
	
	AnalyseTlvSwitchHeader ( pTlvBuffer + 4 , &tlvInfo );
	nSize = sizeof(tab_TlvSwitchHeader)/sizeof(tab_TlvSwitchHeader[0]);
	nIndice = 0;
	do 
	{
		if (tlvInfo.nPresent[nIndice] == PRESENT) 
		{
			memset(tlv_name, 0, sizeof(tlv_name));
			memcpy(tlv_name, tab_TlvSwitchHeader[nIndice].sTlvName, 3);
			memset(data, 0, sizeof(data));
			nRetCode = GetTlvSwitchHeader (tlv_name, &tlvInfo, data , &length);
			if (nRetCode != SUCCESS) 
			{
				sprintf(sLine,"End   ValidateTlvSwitchHeaderData|Presence Error(NOK|%s)",tlv_name);
				trace_event(sLine,PROCESSING);
				return(NOK);

			}
			if(length != strlen(data)) 
			{
				sprintf(sLine,"End   ValidateTlvSwitchHeaderData|Length Error(NOK|%s|%d|%lu|%d)",tlv_name,length,strlen(data),nIndice);
				trace_event(sLine,PROCESSING);
				dump_buffer(data, length,'A','X','X');
				dump_buffer(tlvInfo.sTlvData, tlvInfo.nLength,'A','X','X');
				return(NOK);

			}

		}
		nIndice ++;

	}
	while (nIndice < nSize);
	sprintf(sLine,"End  ValidateTlvSwitchHeaderData(OK)");
	trace_event(sLine,PROCESSING);
	return(OK);

}
int GetTlvSwitchHeader (char *tlv_name, TSTlvSwitchHeader *tlvInfo, char *data , int *length) 
{
	int nIndice;
	int nTlvLength;
	char szAscTlvLen [3 + 4];
	
	memset (szAscTlvLen, 0, sizeof(szAscTlvLen));
	
	if ( (nIndice = GetTlvSwitchHeaderIndice(tlv_name)) == -1) return(ECHEC);
	
	
	nTlvLength = GetTlvSwitchHeaderLength(nIndice);
	
	if (tlvInfo->nPresent[nIndice] == NOT_PRESENT) return(ECHEC);
	
	memcpy ( szAscTlvLen, tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3, 3);
	*length = atoi(szAscTlvLen);
	
	
	memcpy( data, tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3, *length);
	data[*length] = '\0';
	return(SUCCES);


}
int AddTlvSwitchHeader (char *tlv_name, TSTlvSwitchHeader *tlvInfo, char *data, int length) 
{
	int nIndice;
	int nTlvLength;
	char szAscTlvLen [3 + 4];
	
	memset (szAscTlvLen, 0, sizeof(szAscTlvLen));
	
	if ( length == 0 ) return(ECHEC);
	if ( (nIndice = GetTlvSwitchHeaderIndice(tlv_name)) == -1) return(ECHEC);
	
	
	nTlvLength = GetTlvSwitchHeaderLength(nIndice);
	
	
	sprintf ( szAscTlvLen, "%.3d", length);
	
	if (tlvInfo->nPresent[nIndice] == PRESENT) 
		return(ECHEC);
	
	memcpy( tlvInfo->sTlvData + tlvInfo->nLength + 3 + 3, data, length);
	
	memcpy (tlvInfo->sTlvData + tlvInfo->nLength , tlv_name, 3);
	memcpy (tlvInfo->sTlvData + tlvInfo->nLength + 3, szAscTlvLen, 3);
	tlvInfo->nPosTlv [ nIndice ] = tlvInfo->nLength;
	tlvInfo->nPresent[ nIndice ] = PRESENT;
	tlvInfo->nLength +=length + 3 + 3;
	return(SUCCES);

}
int PutTlvSwitchHeader (char *tlv_name, TSTlvSwitchHeader *tlvInfo, char *data, int length) 
{
	int nIndice;
	int nTlvLength;
	int nOldTlvLen;
	
	char szAscTlvLen [ 3 + 4 ];
	char szOldTlvLen [ 3 + 4];
	char szLastPart [ MAX_SW_HEADER_DATA_LEN ];
	
	memset (szAscTlvLen, 0, sizeof(szAscTlvLen));
	memset (szOldTlvLen, 0, sizeof(szOldTlvLen));
	
	if ( length == 0 ) return(ECHEC);
	if ( (nIndice = GetTlvSwitchHeaderIndice(tlv_name)) == -1) 
	{
		return(ECHEC);

	}
	
	
	nTlvLength = GetTlvSwitchHeaderLength(nIndice);
	
	sprintf ( szAscTlvLen, "%.3d", length);
	memset (szLastPart, 0, sizeof(szLastPart));
	if (tlvInfo->nPresent[nIndice] == PRESENT) 
	{
		
		memcpy( szOldTlvLen, tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3, 3);
		nOldTlvLen = atoi(szOldTlvLen);
		memcpy( szLastPart, 
				tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3 +nOldTlvLen, 
				tlvInfo->nLength - tlvInfo->nPosTlv[nIndice]- 3 - 3 - nOldTlvLen);
				
		memcpy (tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3, szAscTlvLen, 3);
		memcpy( tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3, data, length);
		memcpy( tlvInfo->sTlvData + tlvInfo->nPosTlv[nIndice] + 3 + 3 +length, 
			szLastPart, 
			tlvInfo->nLength - tlvInfo->nPosTlv[nIndice]- 3 - 3 - nOldTlvLen);
			
		tlvInfo->nLength += length - nOldTlvLen;
		AdjustTlvSwitchHeader( tlvInfo, nIndice, length-nOldTlvLen);
		return(SUCCES);

	}

	memcpy( tlvInfo->sTlvData + tlvInfo->nLength + 3 + 3, data, length);

	memcpy (tlvInfo->sTlvData + tlvInfo->nLength , tlv_name, 3);
	memcpy (tlvInfo->sTlvData + tlvInfo->nLength + 3, szAscTlvLen, 3);
	tlvInfo->nPosTlv [ nIndice ] = tlvInfo->nLength;
	tlvInfo->nPresent[ nIndice ] = PRESENT;
	tlvInfo->nLength +=length + 3 + 3;
	return(SUCCES);

}
int BuildTlvSwitchHeader (char * buffer_snd, TSTlvSwitchHeader *tlvInfo) 
{
	char sLLLL [ 4 + 1 ];
	char buffer[LG_MAX];
	int nLen;

	memset ( sLLLL , 0 , sizeof ( sLLLL ));
	memcpy(buffer, tlvInfo->sTlvData, tlvInfo->nLength);
	nLen = tlvInfo->nLength;
	sprintf( sLLLL , "%04d" , nLen );
	memcpy ( buffer_snd + 0 , sLLLL , 4 );
	memcpy ( buffer_snd + 4 , buffer , nLen);
	buffer_snd[nLen + 4 ] = '\0';
	return(nLen + 4 );

}
int SwitchHeaderBuildTlvHeader (char * buffer_in, int nLen, char * buffer_out) 
{
	char sLLLL [ 4 + 1 ];
	memset ( sLLLL , 0 , sizeof ( sLLLL ));
	sprintf( sLLLL , "%04d" , nLen );
	memcpy ( buffer_out + 0 , sLLLL , 4 );
	memcpy ( buffer_out + 4 , buffer_in , nLen);
	buffer_out[nLen + 4 ] = '\0';

}
int AnalyseTlvSwitchHeader(char *buffer_in, TSTlvSwitchHeader *tlvInfo) 
{
	char *sPtr;
	int offset;
	int nTlvLength, nBytes;
	int nTlvIndice;
	int nLenTlv;
	char szTlvName [ 3 + 1 ];
	char szTlvValueLen [ 3 + 4 ];
	int nTlvValueLen ;
	char buffer[LG_MAX];
	int		nLLLL;
	char	sLLLL[4 + 1];
	
	memset ( buffer , '\0' , sizeof ( buffer ) );
	memset ( sLLLL , '\0' , sizeof ( sLLLL ) );
	InitTlvSwitchHeader(tlvInfo);
	offset = 0;
	memcpy(sLLLL, buffer_in, 4);

	nLLLL = atoi(sLLLL);
	
	tlvInfo->nLength = nLLLL;
	memcpy(buffer,buffer_in + offset ,nLLLL);
	memcpy (tlvInfo->sTlvData, buffer , nLLLL);
	sPtr = buffer;
	
	offset = 0;
	while (offset < nLLLL) 
	{
		memset( szTlvName, '\0', sizeof(szTlvName));
		memset( szTlvValueLen, 0, sizeof(szTlvValueLen));
		memcpy( szTlvName, sPtr, 3);
		memcpy( szTlvValueLen, sPtr+3, 3);
		nTlvValueLen = atoi(szTlvValueLen);
		nTlvIndice = GetTlvSwitchHeaderIndice( szTlvName);
		if ( nTlvIndice >= 0) 
		{
			tlvInfo->nPosTlv [ nTlvIndice ] = offset;
			tlvInfo->nPresent[ nTlvIndice ] = PRESENT;

		}
		nTlvLength = 3 + 3 + nTlvValueLen;
		offset += nTlvLength;
		sPtr += nTlvLength;

	}

}
void DumpSwitchHeader(TSTlvSwitchHeader *tlvInfo) 
{
	int i;
	char sLine[LG_MAX];
	char sWork[LG_MAX];
	int nLen;
	int		n = sizeof(tlvInfo->nPosTlv)/sizeof(tlvInfo->nPosTlv[0]);
	for( i=0; i < n; i++ ) 
	{
		if( GetTlvSwitchHeader(tab_TlvSwitchHeader[i].sTlvName,tlvInfo,sWork,&nLen) != SUCCES ) continue;
		sprintf ( sLine ,"Tag: '%.4s' :(%03d): (%.55s) : [%s]", tab_TlvSwitchHeader[i].sTlvName,nLen, tab_TlvSwitchHeader[i].description, sWork );
		trace_event(sLine,TRACE);
	}
}


