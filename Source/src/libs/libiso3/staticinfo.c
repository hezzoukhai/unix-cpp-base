#include <p7_common.h>
#include <p7_globals.h>
#include <p7_types.h>
#include <iso8583.h>

/******************************************************************************/

/******************************************************************************/
void ResetStaticInfo(StaticInfo* kStaticInfo)
{
	kStaticInfo->nLength = 0;
	memset(kStaticInfo->sData,'\0',sizeof(kStaticInfo->sData));
}
/******************************************************************************/
void InitStaticInfo(const char* szPropertiesName, StaticInfo* kStaticInfo)
{
	kStaticInfo->pkProperties = GetStaticInfoProperties(szPropertiesName);
	ResetStaticInfo(kStaticInfo);
}
/******************************************************************************/
int  AnalyseStaticInfo(const char *buffer_in, int nLength, StaticInfo* kStaticInfo)
{
	ResetStaticInfo(kStaticInfo);

	if( nLength > sizeof(kStaticInfo->sData))
		return NOK;

	kStaticInfo->nLength = nLength;
	memcpy(kStaticInfo->sData,buffer_in,nLength);
	return OK;
}
/******************************************************************************/
int  GetStaticInfoField (int nPos, const StaticInfo* kStaticInfo, char *data, int* nLength)
{
	int		i;
	int		nIndex=-1;
	for(i=0; i < kStaticInfo->pkProperties->nNbFields; i++)
	{
		if( kStaticInfo->pkProperties->tabFieldsInfo[i].nPos ==  nPos)
		{
			nIndex = i;
			break;
		}
	}
	if( nIndex < 0 )
	{
		(*nLength) = 0;
		data[(*nLength)]='\0';
		return NOK;
	}

	(*nLength) = kStaticInfo->pkProperties->tabFieldsInfo[nIndex].field_info.nLength;
	/*
	memcpy(	data,
			kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos,
			(*nLength));
			*/
	switch( kStaticInfo->pkProperties->tabFieldsInfo[nIndex].field_info.eFormatType)
	{
	case FF_ALPHA_A		:
	case FF_ALPHA_N		:
	case FF_ALPHA_AN	:
	case FF_ALPHA_ANS	:
	case FF_ALPHA_NS	:
	case FF_Z			:
		if( kStaticInfo->pkProperties->tabFieldsInfo[nIndex].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy(	data,
					kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos,
					(*nLength));
			
		}
		else if( kStaticInfo->pkProperties->tabFieldsInfo[nIndex].field_info.eAlphaFormat == AF_EBCDIC )
		{
			EbcdicToAscii( kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos,  data, (*nLength));
		}
		break;
	case FF_BIN:
		memcpy( data, kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos,(*nLength));
		break;
	case FF_HEX:
		HexToAscii( kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos , data, (*nLength)*2);
		(*nLength) = (*nLength)*2;
		break;
	case FF_BCD:
		BcdToAscii( kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos, data, (*nLength)*2);
		(*nLength) = (*nLength)*2;
		break;
	}

	return OK;

}
/******************************************************************************/
int  PutStaticInfoField (int nPos, StaticInfo* kStaticInfo, const char *data, int nLength)
{
	int		i;
	int		nIndex=-1;
	int		nBytes;

	for(i=0; i < kStaticInfo->pkProperties->nNbFields; i++)
	{
		if( kStaticInfo->pkProperties->tabFieldsInfo[i].nPos ==  nPos)
		{
			nIndex = i;
			break;
		}
	}
	if( nIndex < 0 )
	{
		return NOK;
	}
	if( nLength != kStaticInfo->pkProperties->tabFieldsInfo[nIndex].field_info.nLength)
	{
		return NOK;
	}

	/*
	memcpy(	kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos,
			data,
			nLength);*/

	switch(  kStaticInfo->pkProperties->tabFieldsInfo[nIndex].field_info.eFormatType)
	{
	case FF_ALPHA_A		:
	case FF_ALPHA_N		:
	case FF_ALPHA_AN	:
	case FF_ALPHA_ANS	:
	case FF_ALPHA_NS	:
	case FF_Z			:
		if( kStaticInfo->pkProperties->tabFieldsInfo[nIndex].field_info.eAlphaFormat == AF_ASCII )
		{
			memcpy(	kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos,
					data,
					nLength);
		}
		else if( kStaticInfo->pkProperties->tabFieldsInfo[nIndex].field_info.eAlphaFormat == AF_EBCDIC )
		{
			AsciiToEbcdic ( data, 
							kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos,
							nLength);
		}
		nBytes = nLength;
		break;
	case FF_BIN:
		memcpy(kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos, data, nLength);
		nBytes = nLength;
		break;
	case FF_HEX:
		AsciiToHex( data , kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos, nLength);
		nBytes = nLength/2;
		break;
	case FF_BCD:
		AsciiToBcd( data,  kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[nIndex].nPos, nLength);
		nBytes = nLength/2;
		break;
	}

	return OK;
}

/******************************************************************************/
int  AddStaticInfoField (int nPos, StaticInfo* kStaticInfo, const char *data, int nLength)
{
	return PutStaticInfoField(nPos,kStaticInfo,data,nLength);
}
/******************************************************************************/
int  BuildStaticInfoBuffer (char * buffer_snd, StaticInfo* kStaticInfo)
{
	memcpy(buffer_snd,kStaticInfo->sData,kStaticInfo->nLength);
	return OK;
}
/******************************************************************************/
void DumpStaticInfo(const StaticInfo* kStaticInfo)
{
	int			i;
	char		data[1024];
	char		dataAdd[1024];
	int			nLen;

	for(i=0; i < kStaticInfo->pkProperties->nNbFields; i++ )
	{
		nLen = kStaticInfo->pkProperties->tabFieldsInfo[i].field_info.nLength;
		memcpy(	data,
				kStaticInfo->sData + kStaticInfo->pkProperties->tabFieldsInfo[i].nPos,
				nLen);
		data[nLen]='\0';
		if( kStaticInfo->pkProperties->tabFieldsInfo[i].field_info.eFormatType != FF_BIN )
		{
			trace_message(P7_TL_TRACE,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLen ,data );
		}
		else
		{
			nLen = HexToAscii(data, dataAdd, nLen*2);
			dataAdd[nLen]='\0';/*PLUTONL-5279 AHA20221021*/
			trace_message(P7_TL_TRACE,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLen ,dataAdd );
		}
		/*
		switch( kStaticInfo->pkProperties->tabFieldsInfo[i].field_info.eFieldType)
		{
		case FT_SINGLE:
			trace_message(P7_TL_TRACE,"- FLD (%16s) : (%03d) : [%s] " , kStaticInfo->pkProperties->tabFieldsInfo[i].field_info.szLabel , nLen ,data );
			break;
		case FT_STRUCT_TLV:
			{
				TlvInfo	kLocTlvInfo;
				InitTlvInfo(kStaticInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocTlvInfo);
				AnalyseTlvInfo(data,nLen,&kLocTlvInfo);
				DumpTlvInfo(&kLocTlvInfo);
			}
			break;
		case FT_STRUCT_STATIC:
			{
				StaticInfo	kLocStaticInfo;
				InitStaticInfo(kStaticInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocStaticInfo);
				AnalyseStaticInfo(data,nLen,&kLocStaticInfo);
				DumpStaticInfo(&kLocStaticInfo);
			}
			break;
		case FT_STRUCT_BER:
			{
				BerInfo	kLocBerInfo;
				InitBerInfo(kStaticInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocBerInfo);
				AnalyseBerInfo(data,nLen,&kLocBerInfo);
				DumpBerInfo(&kLocBerInfo);
			}
			break;
		case FT_STRUCT_BITMAPPED:
			{
				BitMappedInfo	kLocBitMappedInfo;
				InitBitMappedInfo(kStaticInfo->pkProperties->tabFieldsInfo[i].field_info.szPropertiesName,&kLocBitMappedInfo);
				AnalyseBitMappedInfo(data,nLen,&kLocBitMappedInfo);
				DumpBitMappedInfo(&kLocBitMappedInfo);
			}
			break;
		default:
			trace_message(E_WARNING,"Unknown Field type: %u",kMsgInfo.pkMsgInfoProperties->tabFieldsInfo[i].eFieldType);
			break;
		}*/

	}
}
/******************************************************************************/
void InitStaticInfoProperties(StaticInfoProperties* kStaticInfoProperties)
{
	
	static_field_t		stFieldInfo;
	int					i;
	int					nSize;
	kStaticInfoProperties->nNbFields = 0 ;
	
	stFieldInfo.nPos=0;
	InitFieldInfo(&stFieldInfo.field_info);
	memset(kStaticInfoProperties->szPropertiesName,'\0',sizeof(kStaticInfoProperties->szPropertiesName));
	nSize = sizeof(kStaticInfoProperties->tabFieldsInfo)/sizeof(kStaticInfoProperties->tabFieldsInfo[0]);
	for( i=0; i < nSize; i++ )
	{
		kStaticInfoProperties->tabFieldsInfo[i]=stFieldInfo;
	}
}
/******************************************************************************/
