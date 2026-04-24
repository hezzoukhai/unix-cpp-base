#include <define.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <iso_hps.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <list_thr.h>
#include <security_data.h>
#include <queue.h>
#include <hsm_inc.h>
#include <resources.h>
#include <p7_define.h>
#include <p7_sid.h>
#include <p7_common.h>
#include <p7_globals.h>
#include <iso8583.h>
#include <tlv_fields.h>
#include <tlv_privfields.h>
#include <msg_flow_mapping.h>
#include <int1_struct.h>
#include <int1.h>

/*************************************************************************/
/* FCT_AUT_REQ_TO_NW()                                                 */
/*																		*/
/*************************************************************************/

int INT1_FCT_AUT_REQ_TO_NW(int nIndexCtx, MsgInfo * pkMsgInfo, TSIsoInfo * IsoInfo)
{
	
	TSNodeData      Node;
	int             result;
	int             retour;
	char            TrxType[3];
	char            tmpField[256];
	char            sBuffer[1024];
	int             tmpFieldLen;
	int             Length;
	char            sPurgeTime[10];
	char            sLine[MAX_LINE_TRC];


	trace_event("Start  INT1_FCT_AUT_REQ_TO_NW()", PROCESSING);

	memset(sLine     ,'\0',sizeof(sLine     ));
	memset(tmpField  ,'\0',sizeof(tmpField  ));
	memset(sBuffer   ,'\0',sizeof(sBuffer   ));
	memset(sPurgeTime,'\0',sizeof(sPurgeTime));

	dump_p7_struct(IsoInfo);

	MsgFwdReqRetrieved(IsoInfo->msgId);	

	memset(sBuffer, 0, sizeof(sBuffer));

	/* Insert original ISO message in list for later retrieval */
	InitNodeData(&Node);
	GetIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen);
	GetSecurityData(PURGE_TIME, tmpField, sPurgeTime, &tmpFieldLen);

	Node.tPurgeTime = atol(sPurgeTime);


	/*GetIsoField(ISO_AUDIT_NBR, IsoInfo, tmpField, &tmpFieldLen);*/ 
	/*AMER20180126 PROD00052530: Save external stan instead of internal stan in node key*/
	GetMsgField(ISO_AUDIT_NBR, pkMsgInfo, tmpField, &tmpFieldLen);
	/*sprintf(Node.szFstKey,"%.4d",IsoInfo->nMsgType);*/
	sprintf(Node.szFstKey,"%.4d", pkMsgInfo->nMsgType); /*PROD00057912 20180706: Use network message type instead of iso message type, because POS auth MTI = 1200 but AMEX_GNS POS auth MTI = 1100*/
	memcpy(Node.szFstKey + 4, tmpField, 6);

	GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen);
	memcpy(Node.szFstKey + 4 + 6, tmpField, tmpFieldLen); 

	if(debug_option == 1)
	{
		sprintf(sLine, "==> InsertKey = [%.32s] ", Node.szFstKey);
		trace_event(sLine, PROCESSING);
	}


	Length = IsoBuildMsg(Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	Node.msgId = IsoInfo->msgId;

	retour = InsertRequest('B', &Node);
	if (retour == EEXIST)
		pkMsgInfo->nMsgType++;
	else if (retour != OK) 
	{
		trace_event("INT1_FCT_AUT_REQ_TO_NW(): InsertRequest() failed", PROCESSING);
		return (NOK);
	}

	/* send request to network */
	Length = BuildMsgInfoBuffer(sBuffer, pkMsgInfo);
	
	dump_buffer(sBuffer, Length, 'A', 'L', 'O');

	DumpMsgInfo(pkMsgInfo);


	if (WriteLineMsg(nConnId,sBuffer,Length)) 
	{
		trace_event("INT1_FCT_AUT_REQ_TO_NW(): WriteLineMsg() failed", PROCESSING);
		return (NOK);
	}

	MsgFwdReqSent(IsoInfo->msgId);

	trace_event("End  INT1_FCT_AUT_REQ_TO_NW(OK)", PROCESSING);

	return (OK);
}
