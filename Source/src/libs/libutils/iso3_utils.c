#include <stdio.h>
#include <stdlib.h>
#include <iso8583.h>


int GetComposedFieldDataOffset(	int 			nFieldNo, 
								E_FIELD_TYPE 	eFieldType, 
								char* 			sPropertiesName, 
								E_ISO8583_PROTO eProtocol,
								int				nOrigDataLength,
								int* 			pnDataOffset, 
								int* 			pnDataLength)
{

	(*pnDataOffset) 	= 0;
	(*pnDataLength) 	= nOrigDataLength;
	switch(eProtocol)
	{
		case ISO8583_MCI_CIS:
		case ISO8583_MCI_MDS:
			if( nFieldNo == STD_ISO_ADTNL_DATA_PRIV )
			{
				(*pnDataOffset) 	= 1;
				(*pnDataLength) 	= nOrigDataLength - (*pnDataOffset);
				return OK;
			}
			break;
		case ISO8583_VISA_BASE1:
		case ISO8583_VISA_SMS:
			if( nFieldNo == STD_ISO_ICC_DATA )
			{
				(*pnDataOffset) 	= 3;
				(*pnDataLength) 	= nOrigDataLength - (*pnDataOffset);
			}
			break;
		default:
			break;
	
	}
	
	
	return OK;
}


int MaskTlvInfo(E_ISO8583_PROTO eProtocol, TlvInfo*		pkTlvInfo)
{

	int 		j;
	char    	tmpField[BUF_LG_MAX];
	int			nLength;
	int			nResult;
	
	memset (tmpField, '\0', sizeof(tmpField));

	
	for( j=0; j < pkTlvInfo->pkProperties->nNbFields; j++)
	{
		if (pkTlvInfo->pkProperties->tabFieldsInfo[j].field_info.nPrintPolicy != PP_ENCRYPT
				&& pkTlvInfo->pkProperties->tabFieldsInfo[j].field_info.nPrintPolicy != PP_HIDE)
		{
			continue;
		}

		if( GetTlvInfoField(pkTlvInfo->pkProperties->tabFieldsInfo[j].szTag,pkTlvInfo,tmpField,&nLength) != SUCCESS )
		{
			continue;
		}
		
		if (pkTlvInfo->pkProperties->tabFieldsInfo[j].field_info.eFieldType == FT_SINGLE)
		{
			MaskSingleField(	pkTlvInfo->pkProperties->tabFieldsInfo[j].field_info.eFormatType,
								tmpField,
								nLength);

			PutTlvInfoField (pkTlvInfo->pkProperties->tabFieldsInfo[j].szTag, pkTlvInfo, tmpField, nLength);
		}
		else 
		{
			nResult = MaskComposedField(	j, 
											pkTlvInfo->pkProperties->tabFieldsInfo[j].field_info.eFieldType, 
											pkTlvInfo->pkProperties->tabFieldsInfo[j].field_info.szPropertiesName,
											eProtocol,
											tmpField, 
											nLength);
			PutTlvInfoField(pkTlvInfo->pkProperties->tabFieldsInfo[j].szTag, pkTlvInfo, tmpField, nLength);/*PLUTONL-3131*/
		}
				
	}

	
	return OK;

}

int MaskBerInfo(E_ISO8583_PROTO eProtocol, BerInfo*		pkBerInfo)
{
	
	int 		j;
	char    	tmpField[BUF_LG_MAX];
	int			nLength;
	int			nResult;

	for( j=0; j < pkBerInfo->pkProperties->nNbFields; j++)
	{

		if (pkBerInfo->pkProperties->tabFieldsInfo[j].field_info.nPrintPolicy != PP_ENCRYPT
				&& pkBerInfo->pkProperties->tabFieldsInfo[j].field_info.nPrintPolicy != PP_HIDE)
		{
			continue;
		}

		if( GetBerInfoField(pkBerInfo->pkProperties->tabFieldsInfo[j].szTag,pkBerInfo,tmpField,&nLength) != SUCCESS )
		{
			continue;
		}
		if (pkBerInfo->pkProperties->tabFieldsInfo[j].field_info.eFieldType == FT_SINGLE)
		{
			MaskSingleField(	pkBerInfo->pkProperties->tabFieldsInfo[j].field_info.eFormatType,
								tmpField,
								nLength);
			
			PutBerInfoField (pkBerInfo->pkProperties->tabFieldsInfo[j].szTag, pkBerInfo, tmpField, nLength);
		}
		else 
		{
			nResult = MaskComposedField(	j, 
											pkBerInfo->pkProperties->tabFieldsInfo[j].field_info.eFieldType, 
											pkBerInfo->pkProperties->tabFieldsInfo[j].field_info.szPropertiesName,
											eProtocol,
											tmpField, 
											nLength);
			PutBerInfoField (pkBerInfo->pkProperties->tabFieldsInfo[j].szTag, pkBerInfo, tmpField, nLength);/*PLUTONL-3131*/
		}

		
	}


	return OK;
}

int MaskStaticInfo(E_ISO8583_PROTO eProtocol, StaticInfo*		pkStaticInfo)
{
	
	int 		j;
	char    	tmpField[BUF_LG_MAX];
	int			nLength;
	int			nResult;
	
	for( j=0; j < pkStaticInfo->pkProperties->nNbFields; j++)
	{

		if (pkStaticInfo->pkProperties->tabFieldsInfo[j].field_info.nPrintPolicy != PP_ENCRYPT
				&& pkStaticInfo->pkProperties->tabFieldsInfo[j].field_info.nPrintPolicy != PP_HIDE)
		{
			continue;
		}

		if( GetStaticInfoField(pkStaticInfo->pkProperties->tabFieldsInfo[j].nPos,pkStaticInfo,tmpField,&nLength) != SUCCESS )
		{
			continue;
		}
		if (pkStaticInfo->pkProperties->tabFieldsInfo[j].field_info.eFieldType == FT_SINGLE)
		{
			MaskSingleField(	pkStaticInfo->pkProperties->tabFieldsInfo[j].field_info.eFormatType,
								tmpField,
								nLength);
			
			PutStaticInfoField (pkStaticInfo->pkProperties->tabFieldsInfo[j].nPos, pkStaticInfo, tmpField, nLength);
		}
		else 
		{
			nResult = MaskComposedField(	j, 
											pkStaticInfo->pkProperties->tabFieldsInfo[j].field_info.eFieldType, 
											pkStaticInfo->pkProperties->tabFieldsInfo[j].field_info.szPropertiesName,
											eProtocol,
											tmpField, 
											nLength);
			PutStaticInfoField(pkStaticInfo->pkProperties->tabFieldsInfo[j].nPos, pkStaticInfo, tmpField, nLength);/*PLUTONL-3131*/
		}

		
	}


	return OK;
}

int MaskBitMappedInfo(E_ISO8583_PROTO eProtocol, BitMappedInfo*		pkBitMappedInfo)
{
	
	int 		j;
	char    	tmpField[BUF_LG_MAX];
	int			nLength;
	int			nResult;
	
	memset (tmpField, '\0', sizeof(tmpField));
	for( j=0; j < pkBitMappedInfo->pkProperties->nNbFields; j++)
	{

		if (pkBitMappedInfo->pkProperties->tabFieldsInfo[j].field_info.nPrintPolicy != PP_ENCRYPT
				&& pkBitMappedInfo->pkProperties->tabFieldsInfo[j].field_info.nPrintPolicy != PP_HIDE)
		{
			continue;
		}

		if( GetBitMappedInfoField(pkBitMappedInfo->pkProperties->tabFieldsInfo[j].nFieldNo,pkBitMappedInfo,tmpField,&nLength) != SUCCESS )
		{
			continue;
		}
		if (pkBitMappedInfo->pkProperties->tabFieldsInfo[j].field_info.eFieldType == FT_SINGLE)
		{
			MaskSingleField(	pkBitMappedInfo->pkProperties->tabFieldsInfo[j].field_info.eFormatType,
								tmpField,
								nLength);
			PutBitMappedInfoField (pkBitMappedInfo->pkProperties->tabFieldsInfo[j].nFieldNo, pkBitMappedInfo, tmpField, nLength);
		}
		else 
		{
			nResult = MaskComposedField(	j, 
											pkBitMappedInfo->pkProperties->tabFieldsInfo[j].field_info.eFieldType, 
											pkBitMappedInfo->pkProperties->tabFieldsInfo[j].field_info.szPropertiesName,
											eProtocol,
											tmpField, 
											nLength);
			/*PLUTONL-3131*/
			PutBitMappedInfoField (pkBitMappedInfo->pkProperties->tabFieldsInfo[j].nFieldNo, pkBitMappedInfo, tmpField, nLength);
			



		}

	}


	return OK;
}



int MaskSingleField(E_FIELD_FORMAT	eFormatType, char* sData, int nLength)
{
	
	switch (eFormatType)
	{
	case FF_ALPHA_A:
	case FF_ALPHA_AN:
	case FF_ALPHA_ANS :
		memset(sData, '*', nLength);
		break;
	case FF_ALPHA_N:
		memset(sData, '0', nLength);
		break;
	case FF_HEX:
		memset(sData, 'F', nLength);
		break;
	case FF_BIN:
		memset(sData, '\xFF', nLength);
		break;
	default:
		memset(sData, '*', nLength);
		break;
	}
	return OK;
}

int MaskComposedField(int nFieldNo, E_FIELD_TYPE eFieldType, char * sPropertiesName,E_ISO8583_PROTO eProtocol, char* sData, int nLength)
{
	TlvInfo			kTlvInfo;
	BerInfo			kBerInfo;
	StaticInfo		kStaticInfo;
	BitMappedInfo	kBitMappedInfo;
	int				nDataLength;
	int				nDataOffset;
	int				nLocLen;
	int				nResult;

	
	nResult = GetComposedFieldDataOffset(	nFieldNo, 
											eFieldType, 
											sPropertiesName, 
											eProtocol,
											nLength,
											&nDataOffset, 
											&nDataLength);
											
	
								
	switch( eFieldType )
	{
		case FT_STRUCT_TLV:
			InitTlvInfo(sPropertiesName,&kTlvInfo);
			AnalyseTlvInfo(sData + nDataOffset,nDataLength,&kTlvInfo);
			MaskTlvInfo(eProtocol,&kTlvInfo);
			nLocLen = BuildTlvInfoBuffer (sData + nDataOffset, &kTlvInfo);
			break;
		case FT_STRUCT_BER:
			InitBerInfo(sPropertiesName,&kBerInfo);
			AnalyseBerInfo(sData + nDataOffset,nDataLength,&kBerInfo);
			MaskBerInfo(eProtocol,&kBerInfo);
			nLocLen = BuildBerInfoBuffer (sData + nDataOffset, &kBerInfo);
			break;
		case FT_STRUCT_STATIC:
			InitStaticInfo(sPropertiesName, &kStaticInfo);
			AnalyseStaticInfo(sData + nDataOffset,nDataLength,&kStaticInfo);
			MaskStaticInfo(eProtocol,&kStaticInfo);
			nLocLen = BuildStaticInfoBuffer (sData + nDataOffset, &kStaticInfo);
			break;
		case FT_STRUCT_BITMAPPED:
			InitBitMappedInfo(sPropertiesName, &kBitMappedInfo);
			AnalyseBitMappedInfo(sData + nDataOffset,nDataLength,&kBitMappedInfo);
			MaskBitMappedInfo(eProtocol,&kBitMappedInfo);
			nLocLen = BuildBitMappedInfoBuffer (sData + nDataOffset, &kBitMappedInfo);
			break;
		default:
			return NOK;
	}
	
		
	return OK;
}


