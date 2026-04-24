#include <logigab.h>

static int GetFieldIndex(int field_no, TSLGMsgInfo* msgInfo )
{
	int	i;
	for(i=0; i < msgInfo->layout->nNbFields; i++ )
	{
		if( msgInfo->layout->tab_fields[i].nFieldNo == field_no )
			return i;
	}

	return -1;
}

static int GetMultiFieldIndex(int field_no, int nPosition, TSLGMsgInfo* msgInfo) {

	int i;
	int nIndex;
	nIndex = 1;

	for (i = 0; i < msgInfo->layout->nNbFields; i++) {
		if (msgInfo->layout->tab_fields[i].nFieldNo == field_no) {

			if (nPosition == nIndex) {
				return i;
			}

			else {
				nIndex++;

			}
		}

	}

	return -1;
}

int GetLGMacData(TSLGMsgInfo * msgInfo, char *data, int *nLength)
{
	int		nIndex;
	char	sLine[MAX_LINE_TRC];
	int		nFieldOffset;

	nIndex = GetFieldIndex(LG_MAC_TYPE,msgInfo);
	if( nIndex < 0 )
	{
		sprintf(sLine,"MAC Not supported");
		trace_event(sLine,ERROR);
		return NOK;
	}

	nFieldOffset = msgInfo->layout->tab_fields[nIndex].nOffset;
	memcpy(data,msgInfo->data,nFieldOffset);

	data[nFieldOffset+1] = '\0';

	(*nLength) = nFieldOffset;

	return SUCCESS;
}



int GetLGMsgField(int field_no, TSLGMsgInfo * msgInfo, char *data, int *nLength)
{
	int		nIndex;
	char	sLine[MAX_LINE_TRC];
	int		nFieldLength;
	int		nFieldOffset;

	nIndex = GetFieldIndex(field_no,msgInfo);
	if( nIndex < 0 )
	{
		sprintf(sLine,"Field No found %d",field_no);
		trace_event(sLine,ERROR);
		return NOK;
	}

	nFieldOffset = msgInfo->layout->tab_fields[nIndex].nOffset;
	nFieldLength = msgInfo->layout->tab_fields[nIndex].nSize;
	memcpy(data,msgInfo->data + nFieldOffset,nFieldLength);

	/*TODO: variable length fields */
	data[nFieldLength] = '\0';

	(*nLength) = nFieldLength;
	return SUCCESS;
}

int GetLGMsgMultiField(int field_no,int nPostion, TSLGMsgInfo * msgInfo, char *data, int *nLength)
{
	int		nIndex;
	char	sLine[MAX_LINE_TRC];
	int		nFieldLength;
	int		nFieldOffset;

	nIndex = GetMultiFieldIndex(field_no,nPostion,msgInfo);

	if( nIndex < 0 )
	{
		sprintf(sLine,"Field No found %d",field_no);
		trace_event(sLine,ERROR);
		return NOK;
	}

	nFieldOffset = msgInfo->layout->tab_fields[nIndex].nOffset;
	nFieldLength = msgInfo->layout->tab_fields[nIndex].nSize;
	memcpy(data,msgInfo->data + nFieldOffset,nFieldLength);

	/*TODO: variable length fields */
	data[nFieldLength] = '\0';

	(*nLength) = nFieldLength;
	return SUCCESS;
}


int GetLGMsgAddData(TSLGMsgInfo * msgInfo, char *data, int *nLength)
{
	int		nIndex;
	char	sLine[MAX_LINE_TRC];
	int		nFieldLength;
	int		nFieldOffset;

	nIndex = msgInfo->layout->nNbFields -1;

	nFieldOffset = msgInfo->layout->tab_fields[nIndex].nOffset;
	nFieldLength = msgInfo->nLength - nFieldOffset;
	memcpy(data,msgInfo->data + nFieldOffset,nFieldLength);

	data[nFieldLength] = '\0';

	(*nLength) = nFieldLength;
	return SUCCESS;
}



int AddLGMsgField(int field_no, TSLGMsgInfo *msgInfo, char *data, int nLength)
{
	int		nIndex;
	char	sLine[MAX_LINE_TRC];
	int		nFieldOffset;


	if( tab_LGFields[field_no].eFieldType == LG_UNDEF)
	{
		sprintf(sLine,"Undefined Field No %d",field_no);
		trace_event(sLine,ERROR);
		return NOK;
	}

	if( ( tab_LGFields[field_no].eFieldType == LG_ALPHA ||
		tab_LGFields[field_no].eFieldType == LG_ALPHA
		) && tab_LGFields[field_no].nFieldLength != nLength )
	{
		sprintf(sLine,"Invalid length for Field No %d( %d VS %d)",field_no,nLength,tab_LGFields[field_no].nFieldLength);
		trace_event(sLine,ERROR);
		return NOK;
	}

	if( tab_LGFields[field_no].eFieldType == LG_VAR_ALPHA &&
		tab_LGFields[field_no].nFieldLength + 1 != nLength
		&& field_no!=LG_ADD_DATA)
	{
		sprintf(sLine,"Invalid length for Field No %d( %d VS %d)",field_no,nLength,tab_LGFields[field_no].nFieldLength + 1);
		trace_event(sLine,ERROR);
		return NOK;
	}

	nIndex = GetFieldIndex(field_no,msgInfo);
	if( nIndex < 0 )
	{
		sprintf(sLine,"Field No found %d",field_no);
		trace_event(sLine,ERROR);
		return NOK;
	}

	nFieldOffset = msgInfo->layout->tab_fields[nIndex].nOffset;
	memcpy(msgInfo->data + nFieldOffset, data,nLength);

	msgInfo->nLength+=nLength;

	return SUCCESS;

}


int AddLGMsgMultiField(int field_no,int nPostion, TSLGMsgInfo *msgInfo, char *data, int nLength)
{
	int		nIndex;
	char	sLine[MAX_LINE_TRC];
	int		nFieldOffset;



	nIndex = GetMultiFieldIndex(field_no,nPostion,msgInfo);
	if( nIndex < 0 )
	{
		sprintf(sLine,"Field No found %d",field_no);
		trace_event(sLine,ERROR);
		return NOK;
	}

	nFieldOffset = msgInfo->layout->tab_fields[nIndex].nOffset;
	memcpy(msgInfo->data + nFieldOffset, data,nLength);

	msgInfo->nLength+=nLength;

	return SUCCESS;

}

int BuildLGMsg (char * data, TSLGMsgInfo *msgInfo)
{
	memcpy(data, msgInfo->header, LG_MSG_HEADER_LEN );
	/*TODO: variable length msgs */
	memcpy(data + LG_MSG_HEADER_LEN,msgInfo->data,msgInfo->layout->nSize);
	return SUCCESS;
}

void DumpLGMsgStruct( TSLGMsgInfo *msgInfo,E_LG_DUMP_LEVEL DumpLevel)
{
	TSLGMsgInfo		msgInfoHeader;

	char	data[BUF_LG_MAX];
	char	dataHex[BUF_LG_MAX];
	int		nLength;
	int		i;
	char*	sPtr;
	int		nFieldNo;
	char	sLine[MAX_LINE_TRC];
	char	sLineBuf[MAX_LINE_TRC];
	char	sHeader[LG_MSG_HEADER_LEN + 1];
	int		nLineLen;

	if(DumpLevel == LG_LEVEL_MSG)
	{
		trace_event("Start DumpLGMsgStruct()",PROCESSING);
	}

	sPtr = msgInfo->data;
	for(i=0; i < msgInfo->layout->nNbFields; i++ )
	{
		nLength = msgInfo->layout->tab_fields[i].nSize;
		nFieldNo = msgInfo->layout->tab_fields[i].nFieldNo;
		
		/*if( tab_LGFields[nFieldNo].eFieldType == LG_BIN )
		{
			 memcpy(data,sPtr, nLength);
			 nLength = HexToAscii(data, dataHex, nLength*2);
			 memset(data,0,sizeof(data));
			 strncpy(data,dataHex, nLength);


		}*/

		if( nFieldNo == LG_MSG_HEADER )
		{
			GetLGMsgField(LG_MSG_HEADER, msgInfo, sHeader, &nLength);
			AnalyseLGMsgHeader(sHeader, &msgInfoHeader);

		   trace_event("------------------------------------",TRACE);
		   sprintf(sLine ,"- MESSAGE :  HEADER" );
		   trace_event(sLine,TRACE);
		   trace_event("------------------------------------",TRACE);

			DumpLGMsgStruct( &msgInfoHeader,LG_LEVEL_HEADER);

		   trace_event("------------------------------------",TRACE);
		   sprintf(sLine ,"- MESSAGE :  BODY" );
		   trace_event(sLine,TRACE);
		   trace_event("------------------------------------",TRACE);
		}

		else if( nFieldNo != 999 )
		{
			strncpy(data,sPtr, nLength);
			data[nLength]='\0';

			sprintf ( sLineBuf ,"FLD: '%.3d' : (%-25s",
						nFieldNo,
						tab_LGFields[nFieldNo].szLabel);
			nLineLen = strlen(sLineBuf);
			if( nLineLen < 44 )
			{
				memset(sLineBuf + nLineLen,' ', 44 - nLineLen);
			}
			sLineBuf[44]=')';
			sLineBuf[45]='\0';
			sprintf ( sLine ,"%s (%.3d): [%s]",
						sLineBuf,
						nLength,
						data );
			trace_event(sLine,PROCESSING);
			/*fprintf(stderr,"%s\n",sLine);*/
		}

		sPtr += nLength;
	}

	if(DumpLevel == LG_LEVEL_MSG)
	{
		trace_event("End   DumpLGMsgStruct(OK)",PROCESSING);
	}


}


