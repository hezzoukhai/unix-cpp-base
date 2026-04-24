#include <stdio.h>
#include <define.h>
#include <struct_line.h>
#include <iso_visa.h>
#include <visa_param.h>

void 
LineProcMsg(int nIndexCtx, TSLineExchange * sLineMsg)
{
	TSVisaInfo      VisaInfo;
	char            sLine[MAX_LINE_TRC];
	int             result;
    MsgInfo		    kMsgInfo;   /* PLUTONL-1822*/
    char            sMsgType[4+1];/*PLUTONL-3762*/

	trace_event("LineProcMSg Begin", PROCESSING);
	InitVisaInfo(&VisaInfo);
	
	/*Start PLUTONL-1822*/
    InitNetworkMsgInfo(&kMsgInfo);

	result = CtrlMsg(sLineMsg->sBuffer,&kMsgInfo);
	if(result != OK)
	{
		sprintf(sLine,"CtrlMsg %d", result);
		trace_event(sLine, FATAL);
		return ; 
	}
	/*End PLUTONL-1822*/
	AnalyseVisa(sLineMsg->sBuffer, &VisaInfo);

	VisaInfo.kCtime = sLineMsg->Stime;

	if (VisaInfo.sHeader.bHeaderFlag & 0x80) 
	{
		/*** Rejected Message to be processed ****/
		trace_event("Message Rejected by VAP", PROCESSING);
		RejectedMessage(nIndexCtx ,&VisaInfo, VisaInfo.sHeader.abRejectCode);
		return;
	}
	if (VisaInfo.sHeader.abMsgFlags[2] & 0x80) 
	{
		/*** Returned Message : Destination unavailable ****/
		trace_event("Message Returned : Destination Unavailable", PROCESSING);
		ReturnedMessage(&VisaInfo);
		return;
	}
	sprintf(sLine, "Message Received : %d", VisaInfo.nMsgType);
	trace_event(sLine, PROCESSING);

	if (memcmp(ResourceStatus, OFF, 3) == 0) 
	{
		memcpy(ResourceStatus, SIGNON, 3);
		if (UpdateResource(nIndexCtx, ResourceId, LIVE, SIGNON) != OK)
		{
			trace_event("Error on Update resource", ERROR);
			shutdown_resource(nIndexCtx, ResourceId);
		}
	}
	/*PLUTONL-3762*/
	memset (sMsgType, 0, sizeof(sMsgType));
	sprintf(sMsgType, "%04d", VisaInfo.nMsgType);
	if (sMsgType[MSG_TYPE_CLASS] != MSG_TYPE_P2_NET  && (memcmp(sMsgType, "1700", 4) != 0))
	{
		
		if( sMsgType[MSG_TYPE_FUNCTION] == MSG_TYPE_P3_REQ || sMsgType[MSG_TYPE_FUNCTION] == MSG_TYPE_P3_ADV )
		{

			VisaInfo.msgId = MsgIsoLineReqReceived(ResourceId,&(sLineMsg->Stime));
			trace_message(PROCESSING, "[SWMON1]: VisaInfo.msgId (%lu)",VisaInfo.msgId );
		}
			
	}

	/*PLUTONL-3762*/
	switch (VisaInfo.nMsgType) {
	case 102:		/* Partial ATM Dispense  */
		result = FCT_AUT_REQ_102_FROM_NW(nIndexCtx, &VisaInfo);
		break;

	case 100:		/* Authorization Request */
	case 101:		/* Authorization Request */
		result = FCT_AUT_REQ_FROM_NW(nIndexCtx, &VisaInfo);
		break;

	case 110:		/* Authorization Response */
		result = FCT_AUT_REP_FROM_NW(nIndexCtx, &VisaInfo);
		break;

	case 120:		/* Authorization Advice */
	case 121:		/* Authorization Advice */
	case 220:		/* Preauth completion	*/
	case 221:
		result = FCT_AUT_ADV_FROM_NW(nIndexCtx, &VisaInfo);
		break;
	
	/*HAL 20201303 ENHQ2.20 completion advice Art 1.1*/
	case 130:		/* Advice Response */
	    result = FCT_ADV_REP_FROM_NW(nIndexCtx, &VisaInfo);
	    break;
    /*HAL 20201303 ENHQ2.20 completion advice Art 1.1*/	
	
	case 312:		/* File Update Response (Issuer) */
		result = FCT_UPD_REP_FROM_NW(nIndexCtx, &VisaInfo);
		break;

	case 400:		/* Reversal Messages */
	case 401:		/* Reversal Messages */
		result = FCT_REV_REQ_FROM_NW(nIndexCtx, &VisaInfo);
		break;

	case 410:		/* Reversal Response */
		result = FCT_REV_REP_FROM_NW(nIndexCtx, &VisaInfo);
		break;

	case 420:		/* Reversal Advice */
	case 421:		/* Reversal Advice */
		result = FCT_REV_ADV_FROM_NW(nIndexCtx, &VisaInfo);
		break;

	case 800:		/* Network Management */
		result = FCT_NW_MNG_REQ_FROM_NW(nIndexCtx, &VisaInfo);
		break;

	case 810:		/* Network Management Response */
		result = FCT_NW_MNG_REP_FROM_NW(nIndexCtx, &VisaInfo);
		break;

	case 1700:		/* ShutDown Message To Line Threads */
		result = FCT_SHUTDOWN_LINE(nIndexCtx);
		break;

	}
	InitVisaInfo(&VisaInfo);
}
