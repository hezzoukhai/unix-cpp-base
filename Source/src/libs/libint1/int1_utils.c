#include <stdio.h>
#include <define.h>
#include <dump.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <p7_define.h>
#include <p7_sid.h>
#include <p7_common.h>
#include <p7_globals.h>
#include <resources.h>
#include <iso8583.h>
#include <int1_struct.h>
#include <int1.h>
#include <security_data.h>


/******************************************************************************/

int Int1ReplyToNw(int nIndexCtx, TSIsoInfo* pkIsoInfo, MsgInfo* pkMsgInfo)
{
	char			sBuffer[LG_MAX];
	char			sRespCode[3 + 1];
	int				nResult;
	int				nLength;


	trace_event("Start Int1ReplyToNw()", PROCESSING);

	
	nLength = BuildMsgInfoBuffer(sBuffer, pkMsgInfo);
	

	dump_buffer(sBuffer, nLength, 'A', 'L', 'O');

	
	if (WriteLineMsg(nConnId,sBuffer,nLength)) 
	{
		trace_event("End   Int1ReplyToNw(NOK): WriteLineMsg() failed", PROCESSING);
		return (NOK);
	}

	if( GetIsoField(ISO_RESPONSE_CODE,pkIsoInfo,sRespCode,&nLength) != SUCCESS )
	{
		memcpy(sRespCode,MALFUNCTION_ACTION,3);
	}
	

	MsgRepSent(pkIsoInfo->msgId,sRespCode);

	trace_event("End   Int1ReplyToNw(OK)", PROCESSING);
	return OK;
}

/******************************************************************************/

int Int1ReplyToBal(int nIndexCtx, TSIsoInfo* pkIsoInfo)
{
	char            sSecurityData[128];
	char            SourceId[8 + 1];
	char			sLine[MAX_LINE_TRC];
	TSBalExchange   BalMessage;
	int				nResult;
	int				nLength;
	char			SourceNodeId [NODE_ID_LEN + 1];
	int 			nBal;

	trace_event("Start Int1ReplyToBal()", PROCESSING);

	
	memset(sSecurityData, 0, sizeof(sSecurityData));
	memset(SourceId, 0, sizeof(SourceId));
	memset(SourceNodeId, 0, sizeof(SourceNodeId));
	GetIsoField(ISO_SECURITY_DATA, pkIsoInfo, sSecurityData, &nLength);
	GetSecurityData(SOURCE_PID, sSecurityData, SourceId, &nLength);
	GetSecurityData(SOURCE_NODE_ID, sSecurityData, SourceNodeId, &nLength);

	sprintf(sLine, "Source Id = [%s] ", SourceId);
	trace_event(sLine, PROCESSING);
	
	if (memcmp(NodeId, SourceNodeId, NODE_ID_LEN) != 0)
	{
		BalMessage.sHeader.nPartnerId = getuid();
		nBal = nBalBrr;
	} 
	else 
	{
		BalMessage.sHeader.nPartnerId = atoi(SourceId);
		nBal = nBalMsg;
	}
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = pkIsoInfo->nMsgType;
	BalMessage.sHeader.msgId = pkIsoInfo->msgId;
	nLength = IsoBuildMsg(BalMessage.sBody, pkIsoInfo);
	BalMessage.sHeader.nLen = nLength;

	if (WriteBalMsg(nBal, &BalMessage) != OK) 
	{
		trace_event("End Int1ReplyToBal(): WriteBal() failed.", ERROR);
		return (NOK);
	}

	MsgFwdRepReplied(BalMessage.sHeader.msgId);

	trace_event("End   Int1ReplyToBal(OK)", PROCESSING);
	return OK;
}





/*****************************************************************************/
int		Int1GetFieldMapInfo(int nFieldNo, msg_map_t* msgmap, int mapsize, msg_map_t*	msgFieldInfo, int* currPosition)
{
	int i;
	for(i = (*currPosition) ; i < mapsize; i++ )
	{
		if( msgmap[i].nFieldNo == nFieldNo)
		{
			(*msgFieldInfo).nFieldNo =msgmap[i].nFieldNo;
			(*msgFieldInfo).info =msgmap[i].info;
			(*msgFieldInfo).nCondition =msgmap[i].nCondition;
			(*currPosition)=i;
			return OK;
		}
	}
	return NOK;
}