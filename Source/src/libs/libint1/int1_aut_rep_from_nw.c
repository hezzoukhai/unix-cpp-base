#include <stdio.h>
#include <string.h>
#include <define.h>
#include <iso_hps.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <queue.h>
#include <security_data.h>
#include <hsm_inc.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <p7_services.h>
#include <int1.h>
#include <int1_struct.h>

/**************************************************************************/
/* FCT_AUT_REP_FROM_NW()                                                */
/* */
/**************************************************************************/
int Int1GetOriginalRequest(int nIndexCtx, MsgInfo * pkMsgInfoResp, TSIsoInfo* pkOrigIsoInfo, char* sOrigTlvBuffer)
{
	TSNodeData      Node;
	char            sFieldData[256];
	
	int             nLength;
	
	char            sLine[MAX_LINE_TRC];

	trace_event("Start  Int1GetOriginalRequest()", PROCESSING);

	/* Get audit number to search for original transaction */
	GetMsgField(ISO_AUDIT_NBR, pkMsgInfoResp, sFieldData, &nLength);
	
	sprintf(sLine, "Stan en retour = %s ", sFieldData);
	trace_event(sLine, PROCESSING);

	InitNodeData(&Node);
	sprintf(Node.szFstKey, "%.4d", pkMsgInfoResp->nMsgType - 10);
	memcpy(Node.szFstKey + 4, sFieldData, 6);

	GetMsgField(ISO_CARD_NBR, pkMsgInfoResp, sFieldData, &nLength);
	memcpy(Node.szFstKey + 4 + 6, sFieldData, nLength);



	switch (FetchRequest('B', &Node)) 
	{
	case ENOEXIST:
		trace_event("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event("End Int1GetOriginalRequest (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;
	}

	trace_event("FetchRequest Succeeded ", PROCESSING);

	

	MsgFwdRepReceived(Node.msgId);

	/* reconstruct original ISO message */
	InitIsoInfo(pkOrigIsoInfo);
	AnalyseIso(Node.sFstBuf, pkOrigIsoInfo);
	pkOrigIsoInfo->msgId = Node.msgId;

	if( Node.nSndBufLen > 0 )
	{
		memcpy(sOrigTlvBuffer,Node.sSndBuf,Node.nSndBufLen);

	}
	sOrigTlvBuffer[Node.nSndBufLen]='\0';

	dump_p7_struct(pkOrigIsoInfo);


	trace_event("End  Int1GetOriginalRequest(OK)", PROCESSING);

	return (OK);
}

