#include <stdio.h>
#include <string.h>
#include <define.h>
#include <iso_hps.h>
#include <tlv_private.h>
#include <security_data.h>
#include <p7_services.h>
#include <int1_struct.h>
#include <int1.h>

/****************************************************************************/
/* INT1_FCT_AUT_ADV_TO_NW()                                                  */
/****************************************************************************/
int INT1_FCT_AUT_ADV_TO_NW(int nIndexCtx, MsgInfo * pkMsgInfo, TSIsoInfo * IsoInfo,char *sTlvBuffer)
{
    TSNodeData      Node;
    int             retour;
    char            tmpField[256];
    char            sBuffer[1024];
    int             tmpFieldLen;
    int             Length;
	time_t          nPurgeTime;
    char            sLine[MAX_LINE_TRC];

    trace_event("Start  INT1_FCT_AUT_ADV_TO_NW()", PROCESSING);

    dump_p7_struct(IsoInfo);

    MsgFwdReqRetrieved(IsoInfo->msgId);

    memset(sBuffer, 0, sizeof(sBuffer));

    /* Insert original ISO message in list for later retrieval */
	InitNodeData (&Node);
	GetSafPurgeTime (&nPurgeTime);
	Node.tPurgeTime = nPurgeTime;

	GetMsgField(ISO_AUDIT_NBR, pkMsgInfo, tmpField, &tmpFieldLen);
	/*AMER20180213 PROD00052530: Use network message type instead of iso message type, because for POS refund advice the MTI = 1220 but AMEX_GNS refund advice = 1120*/
	sprintf(Node.szFstKey,"%.4d", pkMsgInfo->nMsgType); 
    memcpy(Node.szFstKey + 4, tmpField, 6);

    GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen);
    memcpy(Node.szFstKey + 4 + 6, tmpField, tmpFieldLen);
 
	memcpy(Node.sFstPrvtData, "SAF", 3);
	Node.nFstPrvtDataLen = 3;
	
	if(debug_option == 1)
	{
		sprintf(sLine, "==> InsertKey = [%.32s] ", Node.szFstKey);
		trace_event(sLine, PROCESSING);
	}

    Length = IsoBuildMsg(Node.sFstBuf, IsoInfo);
    Node.nFstBufLen = Length;

    Node.msgId = IsoInfo->msgId;
	Node.nSndBufLen = strlen(sTlvBuffer);
    memcpy(Node.sSndBuf, sTlvBuffer, Node.nSndBufLen);

    retour = InsertRequest('B', &Node);
    if (retour == EEXIST)
            pkMsgInfo->nMsgType++;
    else if (retour != OK)
    {
        trace_event("INT1_FCT_AUT_ADV_TO_NW(): InsertRequest() failed", PROCESSING);
        return (NOK);
    }

    /* send request to network */
    Length = BuildMsgInfoBuffer(sBuffer, pkMsgInfo);

    dump_buffer(sBuffer, Length, 'A', 'L', 'O');

    DumpMsgInfo(pkMsgInfo);

    if (WriteLineMsg(nConnId,sBuffer,Length))
    {
        trace_event("INT1_FCT_AUT_ADV_TO_NW(): WriteLineMsg() failed", PROCESSING);
        return (NOK);
    }

    MsgFwdReqSent(IsoInfo->msgId);

	trace_event("End  INT1_FCT_AUT_ADV_TO_NW(OK)", PROCESSING);
	return (OK);
}

