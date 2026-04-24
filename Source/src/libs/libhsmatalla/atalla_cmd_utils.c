#include <p7_common.h>
#include <iso8583.h>
#include <hsm_atalla_define.h>
#include <hsm_atalla.h>

/******************************************************************************/
#undef NOT_PRESENT	/* need to be removed from define ***/
#undef PRESENT

enum {
	NOT_PRESENT = 0,
	PRESENT		= 1,
	REPEATED	= 2,
};
/******************************************************************************/
void ResetAtallaCmdInfo(TSAtallaCmd* pkAtallaCmd)
{
	pkAtallaCmd->nLength=0;
	memset(pkAtallaCmd->sData,'\0',sizeof(pkAtallaCmd->sData));
	memset(pkAtallaCmd->sCmdTag,'\0',sizeof(pkAtallaCmd->sCmdTag));
	memset(pkAtallaCmd->nPresent,0,sizeof(pkAtallaCmd->nPresent));
	memset(pkAtallaCmd->nPosTlv,0,sizeof(pkAtallaCmd->nPosTlv));
	pkAtallaCmd->nCmdTagLen=0;
	
}

/******************************************************************************/
static int GetFieldIdIndice(E_ATALLA_FIELD_ID nFieldId, const TSAtallaCmd* pkAtallaCmd)
{

	int		i;
	int		nLocFieldId;
	
	for( i=0; i < pkAtallaCmd->pkLayout->nNbFields; i++ )
	{
		nLocFieldId = pkAtallaCmd->pkLayout->tab_fields[i].nFieldId;
		if( nLocFieldId ==  nFieldId)
		{
			return i;
		}
	}
	return -1;
}

/******************************************************************************/
int GetFieldIdLength(E_ATALLA_FIELD_ID nFieldId, const TSAtallaCmd* pkAtallaCmd)
{
	return pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.nLength;
}

/******************************************************************************/
int  AnalyseAtallaCmdInfo(const char *buffer_in, int nLength, TSAtallaCmd* pkAtallaCmd)
{
	const unsigned char  *sPtr;
	int		nCmdTagLen;
	const char		*sTag;
	int i,j=0;
	int nIndice;



	nIndice = pkAtallaCmd->pkLayout->eCmdCode;
	sTag = pkAtallaCmd->pkCmdTagProperties->tabFieldsInfo[nIndice].szTag;
	nCmdTagLen = strlen(sTag);
	
	if (memcmp(sTag, buffer_in + 1, nCmdTagLen) == 0)
		memcpy(pkAtallaCmd->sCmdTag,buffer_in + 1,nCmdTagLen);
	else
		return NOK;


	pkAtallaCmd->nLength = nLength;
	sPtr = buffer_in + nCmdTagLen +1;
	memcpy(pkAtallaCmd->sData,sPtr ,strlen(sPtr)-2);

	for (i=0; i<strlen(sPtr); i++)
	{
		if (j < pkAtallaCmd->pkLayout->nNbFields)
		{
			if (sPtr[i] == '#')
			{
				pkAtallaCmd->nPosTlv [ j ] = i+1;
				pkAtallaCmd->nPresent[ j ] = PRESENT;
				j++;
			}
		}
	}



	return OK;
}

/******************************************************************************/
int  GetAtallaCmdInfoField (E_ATALLA_FIELD_ID nFieldId, const TSAtallaCmd* pkAtallaCmd, char *data, int* nLength)
{
	int		nIndice;
		const char	*sPtr;
		int		i=1,j;
		int		nLen;


		if ( (nIndice = GetFieldIdIndice(nFieldId,pkAtallaCmd)) < 0 )
			return(NOK);


		if (pkAtallaCmd->nPresent[nIndice] == NOT_PRESENT)
			return(NOK);

		if( pkAtallaCmd->nPresent[nIndice] == PRESENT )
		{
			sPtr =  pkAtallaCmd->sData + pkAtallaCmd->nPosTlv[nIndice];
		}

	    else
			return(NOK);

		if (nIndice == pkAtallaCmd->pkLayout->nNbFields -1)
			nLen = strlen(sPtr);
		else
		{
			for (i = 1; i<pkAtallaCmd->pkLayout->nNbFields - nIndice;)
			{
				if (pkAtallaCmd->nPresent[nIndice+i] == 1)
				{
					nLen = pkAtallaCmd->nPosTlv[nIndice+i] - pkAtallaCmd->nPosTlv[nIndice] - 1;
					break;
				}
				else if ( nIndice+i == pkAtallaCmd->pkLayout->nNbFields -1)
				{
					nLen = strlen(sPtr);
					break;
				}
				else
				{
					i++;
				}
			}

		}

		memcpy(data, sPtr, nLen);
		data[nLen] = '\0';
		(*nLength) = nLen;


		return(SUCCESS);
}

/******************************************************************************/
int  AddAtallaCmdInfoField (E_ATALLA_FIELD_ID nFieldId, TSAtallaCmd* pkAtallaCmd, const char *data, int nLength)
{
	int				nIndice;
	int				nBytes;
	int				nFieldMaxLength;
	E_FIELD_LENGTH	eLengthType;
	E_FIELD_FORMAT	eFormatType;

	
	if ( (nIndice = GetFieldIdIndice(nFieldId,pkAtallaCmd)) < 0 )
		return(ERROR);

	if (pkAtallaCmd->nPresent[nIndice] == PRESENT)
		return(ERROR);

	nFieldMaxLength = pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.nLength;
	eLengthType		= pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eLengthType;
	eFormatType		= pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eFormatType;


	if( eLengthType == FL_FIX )
	{
		if( nLength != nFieldMaxLength )
		{
			return NOK;
		}
	}
	else
	{
		if( nLength > nFieldMaxLength )
		{
			return NOK;
		}
	}
	

	nBytes = 0;
	if( nLength >= 0 )
	{
		memcpy(pkAtallaCmd->sData + pkAtallaCmd->nLength , "#", 1);
		switch(  eFormatType)
		{
		case FF_ALPHA_A		:
		case FF_ALPHA_N		:
		case FF_ALPHA_AN	:
		case FF_ALPHA_ANS	:
		case FF_ALPHA_NS	:
		case FF_Z			:
			if( pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_ASCII )
			{
				memcpy(pkAtallaCmd->sData + pkAtallaCmd->nLength + 1 , data, nLength);
			}
			else if( pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eAlphaFormat == AF_EBCDIC )
			{
				AsciiToEbcdic ( data, 
								pkAtallaCmd->sData + pkAtallaCmd->nLength + 1,
								nLength);
			}
			nBytes = nLength;
			break;
		case FF_BIN			:
			memcpy(pkAtallaCmd->sData + pkAtallaCmd->nLength + 1 , data, nLength);
			nBytes = nLength;
			break;
		case FF_HEX:
		
			nBytes = AsciiToHex( data , pkAtallaCmd->sData + pkAtallaCmd->nLength + 1, 2*nLength);

			break;
		case FF_BCD:
			nBytes = AsciiToBcd( data,  pkAtallaCmd->sData + pkAtallaCmd->nLength + 1, 2*nLength);
			break;
		case FF_CBCD:
			nBytes = AsciiToCbcd( data,  pkAtallaCmd->sData + pkAtallaCmd->nLength + 1, nLength*2);
			break;
		}

		pkAtallaCmd->nPosTlv [ nIndice ] = pkAtallaCmd->nLength +1;
		pkAtallaCmd->nPresent[ nIndice ] = PRESENT;
		pkAtallaCmd->nLength += nBytes + 1;
	}
	


	return OK;

}

/******************************************************************************/
int  SetCmdTag (char * buffer_out, TSAtallaCmd* pkAtallaCmd)
{
	int				nIndice;
	const char		*sTag;
	int				nLenTag;
	

	nIndice = pkAtallaCmd->pkLayout->eCmdCode;
	sTag = pkAtallaCmd->pkCmdTagProperties->tabFieldsInfo[nIndice].szTag;
	nLenTag = strlen(sTag);
	memcpy(pkAtallaCmd->sCmdTag, sTag, nLenTag);
	pkAtallaCmd->nCmdTagLen = nLenTag ;

	return pkAtallaCmd->nCmdTagLen;
	
}
/******************************************************************************/
/******************************************************************************/

int  BuildAtallaCmdInfoBuffer (char * buffer_out, TSAtallaCmd* pkAtallaCmd)
{
	int		nLength;
	int		nOffset;
	int		nCmdLen;

	nOffset = SetCmdTag(buffer_out, pkAtallaCmd);
	memcpy ( buffer_out , "<" , 1);
	memcpy ( buffer_out +1, pkAtallaCmd->sCmdTag , nOffset);
	memcpy ( buffer_out + nOffset +1, pkAtallaCmd->sData , pkAtallaCmd->nLength);
	memcpy ( buffer_out + nOffset + pkAtallaCmd->nLength + 1  , "#>" , 2);
	nCmdLen = pkAtallaCmd->nLength + pkAtallaCmd->nCmdTagLen + 3;
	
	return nCmdLen;
}
/******************************************************************************/
void DumpAtallaCmdInfo(const TSAtallaCmd* pkAtallaCmd)
{
	char	sLine[MAX_LINE_TRC];
	int		nOffset = 0;
	int		i;
	int		nLen;
	int		nIter;
	int		nFieldId;
	char	szField[4000 +1];
	char	szFieldAdd[4000 +1];
	
	

	if (debug_option != 1)
		return;

	memset(sLine,'\0',sizeof(sLine));
	memset(szField,'\0',sizeof(szField));

	nIter = 0;
	for( i=0; i < pkAtallaCmd->pkLayout->nNbFields;)
	{
		nFieldId = pkAtallaCmd->pkLayout->tab_fields[i].nFieldId;

		if( GetAtallaCmdInfoField(nFieldId,pkAtallaCmd,szField,&nLen) != SUCCESS )
		{
			i++;
			continue;
		}

		if (pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.nPrintPolicy == PP_ENCRYPT
			|| pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.nPrintPolicy == PP_HIDE)
		{
			i++;
			continue;
		}

		if( pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].field_info.eFormatType != FF_BIN )
		{
			trace_message(P7_TL_TRACE,"- FLD (%.6s) : (%03d) : [%s] " , pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].szTag , nLen ,szField );
		}
		else
		{
			nLen = HexToAscii(szField, szFieldAdd, nLen*2);
			trace_message(P7_TL_TRACE,"- FLD (%.6s) : (%03d) : [%s] " , pkAtallaCmd->pkProperties->tabFieldsInfo[nFieldId].szTag , nLen ,szFieldAdd );
		}

		i++;

	}
}
/******************************************************************************/
