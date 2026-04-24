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
/* INT1_FCT_AUT_REP_TO_NW()                                               */
/*																		  */
/**************************************************************************/
int INT1_FCT_AUT_REP_TO_NW(int nIndexCtx, TSIsoInfo * IsoInfo, MsgInfo * pkMsgInfo, char* sTlvBuffer)
{
	MsgInfo			OrigMsgInfo;
	TSNodeData      Node;
	TSTlvPrivate    TlvInfo;
	char            sStan[6 + 1];
	char            FieldData[BUF_LG_MAX];
	int             FieldLen;
	int             response;
	char            sDestinationPid[10];
	char            sSecurityData[128 + 1];
	char            sDestResource[6 + 1];
	int             i, nLength;
	char            sLine[MAX_LINE_TRC];

	trace_event("Start INT1_FCT_AUT_REP_TO_NW()", PROCESSING);
	
	memset(sLine          ,'\0',sizeof(sLine          ));
    memset(sStan          ,'\0',sizeof(sStan          ));
    memset(FieldData      ,'\0',sizeof(FieldData      ));
    memset(sSecurityData  ,'\0',sizeof(sSecurityData  ));
    memset(sDestResource  ,'\0',sizeof(sDestResource  ));

	InitNetworkMsgInfo(&OrigMsgInfo);

	MsgRepRetrieved(IsoInfo->msgId);
	
	dump_p7_struct(IsoInfo);


	GetIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, &FieldLen);
	GetSecurityData(DESTINATION_RESOURCE, sSecurityData, sDestResource, &FieldLen);


	/* Get audit number to search for original transaction */
	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, &FieldLen) == SUCCESS) 
	{

		AnalyseTlvPrivate(FieldData, FieldLen, &TlvInfo);
		if (GetTlvPrivate(EXTERNAL_STAN_TAG, &TlvInfo, sStan, &FieldLen) != SUCCESS) 
		{
			trace_event("End FCT_AUT_REP_TO_NW (NOK)", ERROR);
			return (NOK);
		}
		sprintf(sLine, ">>> STAN = %.6s", sStan);
		trace_event(sLine, PROCESSING);
	}

	
	InitNodeData(&Node);
	sprintf(Node.szFstKey, "%04d", IsoInfo->nMsgType - 10);
	memcpy(Node.szFstKey + 4, sStan, 6);

	GetIsoField(ISO_CARD_NBR, IsoInfo, FieldData, &FieldLen);
	memcpy(Node.szFstKey + 4 + 6, FieldData, FieldLen);

	sprintf(sLine, "==> Fetch Key =[%.10s***]", Node.szFstKey);
	trace_event(sLine, FATAL);

	switch (FetchRequest('L', &Node)) 
	{
	case ENOEXIST:
		trace_event("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event("End INT1_FCT_AUT_REP_TO_NW (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;
	}

	trace_event("FetchRequest SUCCES OK", PROCESSING);

	nLength = AnalyseMsg(Node.sFstBuf, pkMsgInfo);

	
	memcpy(sTlvBuffer, Node.sSndBuf, Node.nSndBufLen);

	/* update authorizations table */
	response = ServiceReplyProcessing(nIndexCtx, IsoInfo, sTlvBuffer,FWD_MSG_REPLIED);
	if( response == ROUTAGE )
	{
		if (Int1ForwardAutRequest(nIndexCtx, IsoInfo, sTlvBuffer, pkMsgInfo) != OK) 
		{
			/* respond to NW that issuer was not available */
			PutIsoField(ISO_RESPONSE_CODE, IsoInfo, ISSUER_UNAVAILABLE_ACTION, 3);
			trace_event("End  INT1_FCT_AUT_REP_TO_NW(OK)", PROCESSING);
			return (I1RET_PROC_ERR);
		} 
		else 
		{	
			/* request was successfully passed on to issuer interface */
			trace_event(" ROUTAGE Returned ", PROCESSING);
			trace_event("End  INT1_FCT_AUT_REP_TO_NW(OK): nothing to be done", PROCESSING);
			return I1RET_ROUTED;
		}
	}


	trace_event("End  INT1_FCT_AUT_REP_TO_NW(OK): nothing to be done", PROCESSING);

	return I1RET_PROCESSED;

}
