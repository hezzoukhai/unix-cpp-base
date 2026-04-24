#include <stdio.h>
#include <define.h>
#include <p7_sid.h>
#include <queue.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <list_thr.h>
#include <line_com.h>
#include <visa_param.h>
#include <tlv_autho.h>

void BalProcMsg(int nIndexCtx, TSBalExchange * sBalMsg)
{
	TSIsoInfo       IsoInfo;
	TSTlvAutho		sTlvInfo;
	char            FieldData[256];
	int             FieldLen;
	int             result;


	InitIsoInfo(&IsoInfo);
	AnalyseIso(sBalMsg->sBody, &IsoInfo);


	IsoInfo.msgId = sBalMsg->sHeader.msgId;
	dump_p7_struct(&IsoInfo);

	switch (IsoInfo.nMsgType) {
	case 1100:		/* Authorization Request to VISA */
	case 1200:		/* Authorization Request to VISA */
	case 1101:
		if (ResourceStatusSignOn(ResourceStatus) != OK) 
		{
			IsoInfo.nMsgType = 1110;
			ReplyIsoMsgToBal(nBalMsg, &IsoInfo, 0, P7_F039_ISS_UNV);
			MsgFwdRepReplied(sBalMsg->sHeader.msgId);
		} 
		else
		{
			MsgBalFwdReqRetrieved(IsoInfo.msgId,&(sBalMsg->StimeB));/*PLUTONL-3762*/
			FCT_AUT_REQ_TO_NW(nIndexCtx, &IsoInfo);
		}
		break;

	case 1110:		/* Authorization Response to VISA */
	case 1111:
	
		MsgBalRepRetrieved(IsoInfo.msgId,&(sBalMsg->StimeB));/*PLUTONL-3762*/
		FCT_AUT_REP_TO_NW(nIndexCtx, &IsoInfo);
		break;

	case 1304:		/* File Update Request (Issuer) */
		if (ResourceStatusSignOn(ResourceStatus) != OK) 
		{
			IsoInfo.nMsgType = 1314;
			ReplyIsoMsgToBal(nBalMsg, &IsoInfo, 0, P7_F039_ISS_UNV);
			MsgFwdRepReplied(sBalMsg->sHeader.msgId);
		} 
		else
		{
			FCT_UPD_REQ_TO_NW(nIndexCtx, &IsoInfo,"N");/*ICH20191018 CIS/BASE1 Excp file mngt*/;
		}
		break;

	case 1606:		/* Service Message */
		if( GetIsoField(ISO_FUNCTION_CODE, &IsoInfo, FieldData, &FieldLen) != SUCCESS )
		{
			trace_event ( "Function code missing" , PROCESSING );
			return;
		}
		switch (atoi(FieldData)) {
		case P7_F024_N_STOP_INTERFACE:	/* Stop interface */
			break;
		case P7_F024_N_PARAM_REFRESH:	/* Refresh parameters */
			break;
		case P7_F024_N_PING_RESOURCE:	/* Indicate resource status */
			break;
		case P7_F024_N_STATISTICS:	/* Stats */
			break;
		default:	/* Network Management Message */
			FCT_NW_MNG_REQ_TO_NW(nIndexCtx, &IsoInfo);
			break;
		}
	case 1700:		/* ShutDown Message To Line Threads */
		result = FCT_SHUTDOWN_BAL(nIndexCtx);
		break;

	}

	InitIsoInfo(&IsoInfo);
}
