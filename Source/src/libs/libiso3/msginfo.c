#include <p7_common.h>
#include <p7_types.h>
#include <iso8583.h>

/******************************************************************************/

/*TODO: Support multiple network protocol at the same time */
extern const MsgInfoProperties*const  GetNetworkMsgProperties();
extern const MsgInfoProperties*const  GetSwitchMsgProperties();
/******************************************************************************/
/******************************************************************************/
static void		
InitMsgInfo(MsgInfo* pkMsgInfo)
{
	pkMsgInfo->nMsgType = 0;
	pkMsgInfo->nHeaderLen = 0;
	memset(pkMsgInfo->sHeader,0,sizeof(pkMsgInfo->sHeader));
	ResetBitMappedInfo(&pkMsgInfo->kData);
}
/******************************************************************************/
void		
InitMsgInfoFromProp(const char* szPropertiesName, MsgInfo* pkMsgInfo)
{
	InitMsgInfo(pkMsgInfo);
	pkMsgInfo->pkProperties = GetMsgInfoProperties(szPropertiesName);
	pkMsgInfo->kData.pkProperties = &pkMsgInfo->pkProperties->kDataPorperties;
}
/******************************************************************************/
void		
InitNetworkMsgInfo(MsgInfo* pkMsgInfo)
{
	InitMsgInfo(pkMsgInfo);
	pkMsgInfo->pkProperties = GetNetworkMsgProperties();
	pkMsgInfo->kData.pkProperties = &pkMsgInfo->pkProperties->kDataPorperties;
}
/******************************************************************************/
void		
InitSwitchMsgInfo(MsgInfo* pkMsgInfo)
{
	InitMsgInfo(pkMsgInfo);
	pkMsgInfo->pkProperties = GetSwitchMsgProperties();
}
/******************************************************************************/
static int		
GetMsgType(const MsgInfo* pkMsgInfo, const unsigned char* data)
{

	char	szMsgType[MAX_MSG_TYPE_LEN + 1];
	int		nMsgType = 0;

	memset(szMsgType, '\0', sizeof(szMsgType) );
	
	switch(pkMsgInfo->pkProperties->eMsgTypeType)
	{
	case FF_BCD:
		BcdToAscii((char*)data, szMsgType, pkMsgInfo->pkProperties->nMsgTypeLen);
		nMsgType = atoi(szMsgType);
		break;
	case FF_ALPHA_N:
		if( pkMsgInfo->pkProperties->eMsgTypeAlphaFormat == AF_ASCII )
			memcpy(szMsgType, data,  pkMsgInfo->pkProperties->nMsgTypeLen);
		else
			EbcdicToAscii((char*)data, szMsgType, pkMsgInfo->pkProperties->nMsgTypeLen);
			
		nMsgType = atoi(szMsgType);

		break;
	default:
		break;
	}

	return nMsgType;
}
/*****************************************************************************/
int		
AnalyseMsg(const char* buffer, MsgInfo* pkMsgInfo)
{
	int		nBytes  = 0;
	int		i = 0;
	int		iMapCtr = 0;
	const char	*sPtr;
	int		nMsgTypeByteLen;

	InitMsgInfo(pkMsgInfo);

	sPtr = buffer;
	
	/*TODO: How to get received header len if it's dynamic as cbcom?*/
	if( pkMsgInfo->pkProperties->eHeaderType == E_FIXED_LENGTH )
	{
		pkMsgInfo->nHeaderLen = pkMsgInfo->pkProperties->nHeaderLen;
	}

	if( pkMsgInfo->nHeaderLen > 0 )
	{
	memcpy(pkMsgInfo->sHeader, sPtr, pkMsgInfo->nHeaderLen);
	sPtr += pkMsgInfo->nHeaderLen;
	}
	
	pkMsgInfo->nMsgType = GetMsgType(pkMsgInfo,sPtr);
	
	if( pkMsgInfo->pkProperties->eMsgTypeType == FF_BCD )
	{
		nMsgTypeByteLen = (pkMsgInfo->pkProperties->nMsgTypeLen + 1)/2;
	}
	else
	{
		nMsgTypeByteLen = pkMsgInfo->pkProperties->nMsgTypeLen;
	}
			
	sPtr+=nMsgTypeByteLen;

	ResetBitMappedInfo(&pkMsgInfo->kData);
	AnalyseBitMappedInfo(sPtr,MAX_STRUCT_DATA_LEN,&pkMsgInfo->kData);


	return OK;
}
/******************************************************************************/
P7_INLINE int		
GetMsgField(int nFieldNo, const MsgInfo* pkMsgInfo, char *data, int* nLength)
{
	return GetBitMappedInfoField(nFieldNo,&pkMsgInfo->kData,data,nLength);
}
/******************************************************************************/
P7_INLINE int		
AddMsgField(int nFieldNo, MsgInfo* pkMsgInfo, const char *data, int nLength)
{
	return AddBitMappedInfoField(nFieldNo,&pkMsgInfo->kData,data,nLength);
}
/******************************************************************************/
P7_INLINE int
PutMsgField(int nFieldNo, MsgInfo* pkMsgInfo, const char *data, int nLength)
{
	return PutBitMappedInfoField(nFieldNo,&pkMsgInfo->kData,data,nLength);
}
/******************************************************************************/
int		
BuildMsgInfoBuffer (char * buffer, const MsgInfo* pkMsgInfo)
{
	char	szMsgType[MAX_MSG_TYPE_LEN + 1];
	int		offset = 0;
	int		nMsgTypeByteLen;

	memset(szMsgType,'\0',sizeof(szMsgType));


	/*TODO: How to get received header len if it's dynamic as cbcom?*/
	if( pkMsgInfo->pkProperties->eHeaderType == E_FIXED_LENGTH )
	{
		memcpy(buffer + offset, pkMsgInfo->sHeader, pkMsgInfo->nHeaderLen);
		offset += pkMsgInfo->nHeaderLen;
	}

	

	sprintf(szMsgType, "%.*d", pkMsgInfo->pkProperties->nMsgTypeLen, pkMsgInfo->nMsgType);
	switch(pkMsgInfo->pkProperties->eMsgTypeType)
	{
	case FF_BCD:
		AsciiToBcd(szMsgType, buffer + offset, pkMsgInfo->pkProperties->nMsgTypeLen*2);
		break;
	case FF_ALPHA_N:
		if( pkMsgInfo->pkProperties->eMsgTypeAlphaFormat == AF_ASCII )
			memcpy(buffer + offset, szMsgType,  pkMsgInfo->pkProperties->nMsgTypeLen);
		else
			AsciiToEbcdic(szMsgType, buffer + offset, pkMsgInfo->pkProperties->nMsgTypeLen);
		break;
	default:
		break;
	}

	if( pkMsgInfo->pkProperties->eMsgTypeType == FF_BCD )
	{
		nMsgTypeByteLen = (pkMsgInfo->pkProperties->nMsgTypeLen + 1)/2;
	}
	else
	{
		nMsgTypeByteLen = pkMsgInfo->pkProperties->nMsgTypeLen;
	}

	offset+=nMsgTypeByteLen;

	offset+= BuildBitMappedInfoBuffer(buffer + offset,&pkMsgInfo->kData);
	return(offset);

}
/******************************************************************************/
void	
DumpMsgInfo(const MsgInfo* pkMsgInfo)
{

	if( is_trace_level_on(P7_TL_TRACE) != OK)
		return;

	trace_event("------------------------------------",P7_TL_TRACE);
	trace_message(P7_TL_TRACE,"- M.T.I      : %.*d" , pkMsgInfo->pkProperties->nMsgTypeLen, pkMsgInfo->nMsgType);
	trace_event("------------------------------------",P7_TL_TRACE);
	DumpBitMappedInfo(&pkMsgInfo->kData);
	if(debug_option == 1)
		DumpFileBitMappedInfo(&pkMsgInfo->kData, pkMsgInfo->nMsgType);
	trace_event("------------------------------------",P7_TL_TRACE);
}
/******************************************************************************/
void		
InitMsgInfoProperties(MsgInfoProperties* pkMsgInfoProperties)
{
	
	pkMsgInfoProperties->nProtocolId=0;
	pkMsgInfoProperties->eMsgTypeType=FF_ALPHA_A;
	pkMsgInfoProperties->eMsgTypeAlphaFormat = AF_ASCII;
	pkMsgInfoProperties->nMsgTypeLen=0;
	pkMsgInfoProperties->nHeaderLen=0;
	pkMsgInfoProperties->eHeaderType=E_FIXED_LENGTH;
	pkMsgInfoProperties->eHeaderFormat=FF_ALPHA_ANS;
	memset(pkMsgInfoProperties->szProtocolName,'\0',sizeof(pkMsgInfoProperties->szProtocolName));
	memset(pkMsgInfoProperties->szProtocolPrefix,'\0',sizeof(pkMsgInfoProperties->szProtocolPrefix));
	memset(pkMsgInfoProperties->szPropertiesName,'\0',sizeof(pkMsgInfoProperties->szPropertiesName));
	InitBitMappedInfoProperties(&pkMsgInfoProperties->kDataPorperties);
}
/******************************************************************************/

int GetMsgFieldPropName(int nFieldNo, const MsgInfo* pkMsgInfo, char *Name, int* nLength)
{
	return GetBitMappedInfoFieldPropName(nFieldNo, &pkMsgInfo->kData, Name, nLength);
}
/******************************************************************************/
int
CtrlMsg(const char* buffer, MsgInfo* pkMsgInfo)
{
	int		nBytes  = 0;
	int		nBitmapLen = 0;
	int		i = 0;
	int		iMapCtr = 0;
	const char	*sPtr;
	char	szMsgType[MAX_MSG_TYPE_LEN + 1];
	int		nMsgTypeByteLen;
	int		nRetCode = 0;
	BitMappedInfo* pkBitMappedInfo;
	int		nMaxNbFields;
	char	szFieldLen[12];
	int	    nTotalLenFlag = OK;
	int		nLength = 0, nMaxFldLength = 0, nLengthUnit = 0, nLenAdj = 0;
	char	sLine[MAX_LINE_TRC];
	char 	sData[LG_MAX];
	int 	format;
	
	return OK;		/*AMER20200617: Temporary until this function is well tested*/
	
	memset (sLine, '\0' ,sizeof(sLine));
	memset (sData, '\0' ,sizeof(sData));
	memset (szMsgType, '\0', sizeof(szMsgType) );

	sPtr = buffer;
	
	if( pkMsgInfo->pkProperties->eHeaderType == E_FIXED_LENGTH )
	{
		pkMsgInfo->nHeaderLen = pkMsgInfo->pkProperties->nHeaderLen;
	}

	if( pkMsgInfo->nHeaderLen > 0 )
	{
		memcpy(pkMsgInfo->sHeader, sPtr, pkMsgInfo->nHeaderLen);
		sPtr += pkMsgInfo->nHeaderLen;
	}

	if( pkMsgInfo->pkProperties->eMsgTypeType == FF_BCD )
	{
		nMsgTypeByteLen = (pkMsgInfo->pkProperties->nMsgTypeLen + 1)/2;
	}
	else
	{
		nMsgTypeByteLen = pkMsgInfo->pkProperties->nMsgTypeLen;
	}
	
	switch(pkMsgInfo->pkProperties->eMsgTypeType)
	{
	case FF_ALPHA_N:
		if( pkMsgInfo->pkProperties->eMsgTypeAlphaFormat == AF_ASCII )
			memcpy(szMsgType, sPtr,  pkMsgInfo->pkProperties->nMsgTypeLen);
		else
			EbcdicToAscii((char*)sPtr, szMsgType, pkMsgInfo->pkProperties->nMsgTypeLen);
	
		nRetCode = ControlDataType(szMsgType, pkMsgInfo->pkProperties->eMsgTypeType, pkMsgInfo->pkProperties->nMsgTypeLen);
	
		if(nRetCode != OK)
		{
			sprintf(sLine,"CtrlMsg (Msg type format invalid)");
			trace_event(sLine,ERROR);
			return NOK;
		}

		break;
	default:
		break;
	}
			
	sPtr+=nMsgTypeByteLen;

	pkBitMappedInfo = &pkMsgInfo->kData;
	
	nBitmapLen = pkBitMappedInfo->pkProperties->nBitmapLen;
	
	do
	{
		if( pkBitMappedInfo->pkProperties->eBitmapFmt == FF_BIN )
		{
			memcpy( pkBitMappedInfo->sBitMap + iMapCtr*nBitmapLen, sPtr, nBitmapLen);
			sPtr+=nBitmapLen;
		}
		else
		{
			AsciiToHex(sPtr,pkBitMappedInfo->sBitMap + (iMapCtr*nBitmapLen), nBitmapLen*2); 
			sPtr+=16;
		}
		++iMapCtr;
	} while(( pkBitMappedInfo->sBitMap[(iMapCtr - 1) * nBitmapLen] & 0x80)   && (iMapCtr < pkBitMappedInfo->pkProperties->nNbBitmaps));


	nMaxNbFields = (nBitmapLen*8)*iMapCtr;
	for (i = 1; i < nMaxNbFields; ++i)
	{
		memset (szFieldLen, '\0' ,sizeof(szFieldLen));
		
		nLenAdj = 0;
		
		if ( !CheckBit( pkBitMappedInfo->sBitMap, i))
		{
			continue;
		}
		
		nMaxFldLength = pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.nLength;
		
		switch ( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eLengthType )
		{
		case FL_FIX:                           
			nLengthUnit = pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.nLength;
			break;
		case FL_L:
			if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eAlphaFormat == AF_ASCII )
			{
				memcpy(szFieldLen, sPtr ,1);  
			}
			else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eAlphaFormat == AF_EBCDIC )
			{
				EbcdicToAscii(sPtr, szFieldLen, 1);
			}
			
			nRetCode = ControlDataType(szFieldLen, FF_ALPHA_N, 1);
			
			if( nRetCode != OK)
			{
				sprintf(sLine,"CtrlMsg field (%d) invalid length: Not numeric",i+1);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}

			nLengthUnit = atoi(szFieldLen);

			if( nLengthUnit > nMaxFldLength )
			{
				sprintf(sLine,"CtrlMsg field(%d) invalid length Max: %d VS found %d",i+1,nMaxFldLength,nLengthUnit);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			
			if( nTotalLenFlag == OK )
				nLenAdj=1;
			break;
		case FL_LL :                     
			if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eAlphaFormat == AF_ASCII )
			{
				memcpy(szFieldLen, sPtr ,2);
			}
			else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eAlphaFormat == AF_EBCDIC )
			{
				EbcdicToAscii(sPtr, szFieldLen, 2);
			}

			nRetCode = ControlDataType(szFieldLen, FF_ALPHA_N, 2);
			
			if( nRetCode != OK)
			{
				sprintf(sLine,"CtrlMsg field(%d) invalid length: Not numeric",i+1);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}

			nLengthUnit = atoi(szFieldLen);

			if( nLengthUnit > nMaxFldLength )
			{
				sprintf(sLine,"CtrlMsg field(%d) invalid length Max: %d VS found %d",i+1,nMaxFldLength,nLengthUnit);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			
			if( nTotalLenFlag == OK )
				nLenAdj=2;
			break;
		case FL_LLL : 
			if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eAlphaFormat == AF_ASCII )
			{
				memcpy(szFieldLen, sPtr ,3);
			}
			else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eAlphaFormat == AF_EBCDIC )
			{
				EbcdicToAscii(sPtr, szFieldLen, 3);
			}

			nRetCode = ControlDataType(szFieldLen, FF_ALPHA_N, 3);
			
			if( nRetCode != OK)
			{
				sprintf(sLine,"CtrlMsg field(%d) invalid length: Not numeric",i+1);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}

			nLengthUnit = atoi(szFieldLen); 
			
			if( nLengthUnit > nMaxFldLength )
			{
				sprintf(sLine,"CtrlMsg field(%d) invalid length Max: %d VS found %d",i+1,nMaxFldLength,nLengthUnit);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
			
			if( nTotalLenFlag == OK )
				nLenAdj=3;
			break;
		case FL_B :                           
			nLengthUnit = (unsigned char)sPtr[0]; 

           /*PLUTONL-1822*/ 
			if( nLengthUnit > nMaxFldLength )
			{
				sprintf(sLine,"CtrlMsg field(%d) invalid length Max: %d VS found %d",i,nMaxFldLength,nLengthUnit);
				trace_event(sLine,ERROR);
				return(ECHEC);
			} 
           /*PLUTONL-1822*/ 

			if( nTotalLenFlag == OK )
				nLenAdj=1;
			break;
		case FL_BB:
			nLengthUnit = ((unsigned char)sPtr[0])*256 + (unsigned char)sPtr[1];
			
			if( nLengthUnit > nMaxFldLength )
			{
				sprintf(sLine,"CtrlMsg field(%d) invalid length Max: %d VS found %d",i,nMaxFldLength,nLengthUnit);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}

			if( nTotalLenFlag == OK )
				nLenAdj=2;
			break;
		case FL_D :                           
			BcdToAscii(sPtr, szFieldLen, 2);
			nLengthUnit = atoi(szFieldLen);
			if( nTotalLenFlag == OK )
				nLenAdj=1;
			break;
		case FL_DD:
			BcdToAscii(sPtr, szFieldLen, 3);

			nRetCode = ControlDataType(szFieldLen, FF_ALPHA_N, 3);
			
			if( nRetCode != OK)
			{
				sprintf(sLine,"CtrlMsg field(%d) invalid length: Not numeric",i);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}

			nLengthUnit = atoi(szFieldLen);

			if( nLengthUnit > nMaxFldLength )
			{
				sprintf(sLine,"CtrlMsg field(%d) invalid length Max: %d VS found %d",i,nMaxFldLength,nLengthUnit);
				trace_event(sLine,ERROR);
				return(ECHEC);
			}
		
			if( nTotalLenFlag == OK )
				nLenAdj=2;
			break;
		default :
			nLengthUnit = 0;
			break;
		}
		
		if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eLengthUnit == LU_NIBBLES )
		{
			nBytes = (nLengthUnit + 1 )/2;
		}
		else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eLengthUnit == LU_BYTE )
		{
			nBytes = nLengthUnit;
		}
		
		nBytes += nLenAdj;
		
		if(nLengthUnit > 0)
		{
			format = pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eFormatType;
			
			memcpy(sData, sPtr+nLenAdj, nLengthUnit);
			
			switch (format)
			{
			case FF_ALPHA_A:
			case FF_ALPHA_N:
			case FF_ALPHA_AN:
			case FF_ALPHA_ANS :
				if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eAlphaFormat == AF_ASCII )
				{
					memcpy(sData, sPtr+nLenAdj, nLengthUnit);
					
				}
				else if( pkBitMappedInfo->pkProperties->tabFieldsInfo[i].field_info.eAlphaFormat == AF_EBCDIC )
				{
					EbcdicToAscii( sPtr+nLenAdj,  sData, nLengthUnit);
				}
				break;
			case FF_HEX :
				HexToAscii( (unsigned char*)sPtr+nLenAdj, sData, nLengthUnit);
				break;
			case FF_BCD :
				BcdToAscii( (unsigned char*)sPtr+nLenAdj, sData, nLengthUnit);
				break;
			}
			
			nRetCode = ControlDataType(sData, format, nLengthUnit);
		
			if(nRetCode != OK)
			{
				sprintf(sLine,"CtrlMsg field(%d) invalid format %d",i+1, format);
				trace_event(sLine,ERROR);
				return NOK;
			}
		}
		
		sPtr += nBytes;
	}

	return OK;
}
/*End PROD00070301*/