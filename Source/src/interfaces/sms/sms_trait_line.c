#include <stdio.h>

#include <define.h>
#include <struct_line.h>
#include <iso_sms.h>
#include <resources.h>
#include <sms_param.h>

void   LineProcMsg(int nIndexCtx, TSLineExchange *sLineMsg)
{
	TSSmsInfo  SmsInfo;
	char       sLine	[MAX_LINE_TRC];
	char       tmpField	[BUF_LG_MAX];
	int        tmpFieldLen;
	int        result;
	char       sMsgType[4+1];/*PLUTONL-3762*/

	sprintf(sLine, "Start LineProcMSg(%d)", nIndexCtx);
	trace_event(sLine, PROCESSING);

	memset (tmpField,      0,  sizeof(tmpField));
	InitSmsInfo( &SmsInfo);
	AnalyseSms( sLineMsg->sBuffer, &SmsInfo);

	SmsInfo.kCtime = sLineMsg->Stime;
	if ( SmsInfo.sHeader.bHeaderFlag & 0x80 )
	{
		/*** Rejected Message to be processed ****/
		trace_event("--------- Message Rejected by VAP ---------", PROCESSING );
		trace_event("-------------------------------------------", PROCESSING );
		RejectedMessage (nIndexCtx, &SmsInfo, SmsInfo.sHeader.abRejectCode);
		return;
	}

	if ( SmsInfo.sHeader.abMsgFlags[2] & 0x80 )
	{
		/*** Returned Message to be processed : Destination unavailable ****/
		trace_event("Message Returned : Destination Unavailable", PROCESSING);
		ReturnedMessage (&SmsInfo);
		return;
	}

	/* Run CheckFields before processing message */

	sprintf(sLine," Message Type : %d",SmsInfo.nMsgType);
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

	memset (sMsgType, 0, sizeof(sMsgType));
	sprintf(sMsgType, "%04d", SmsInfo.nMsgType);
	if (sMsgType[MSG_TYPE_CLASS] != MSG_TYPE_P2_NET  && (memcmp(sMsgType, "1700", 4) != 0))
	{
		if( sMsgType[MSG_TYPE_FUNCTION] == MSG_TYPE_P3_REQ || sMsgType[MSG_TYPE_FUNCTION] == MSG_TYPE_P3_ADV )
		{
			SmsInfo.msgId = MsgIsoLineReqReceived(ResourceId,&(sLineMsg->Stime));
			trace_message(PROCESSING, "[SWMON1]: SmsInfo.msgId (%lu)",SmsInfo.msgId );
		}
	}
	switch (SmsInfo.nMsgType)
	{ 
	case 800 : /*  Network Management Request       */
		FCT_NW_MNG_REQ_FROM_NW (nIndexCtx, &SmsInfo);
		break;

	case 810 :
		FCT_NW_MNG_REP_FROM_NW (nIndexCtx, &SmsInfo);
		break;

	case 1700:  /* ShutDown Message To Line Threads */
		FCT_SHUTDOWN_LINE (nIndexCtx);
		break;

	case 200 : 
	case 201 : 
		FCT_AUT_REQ_FROM_NW (nIndexCtx, &SmsInfo);
		break;
		
	case 210 : /*     Authorization Response        */
		FCT_AUT_REP_FROM_NW (nIndexCtx, &SmsInfo);
		break;

	case 120:		/* Authorization Advice */
	case 121:		/* Authorization Advice */
	case 220:		
	case 221:
		FCT_AUT_ADV_FROM_NW(nIndexCtx, &SmsInfo);
		break;

	case 230 : /*  Authorization Advice Response    */
		FCT_ADV_REP_FROM_NW (nIndexCtx, &SmsInfo);

	case 312:		/* File Update Response (Issuer) */
		FCT_UPD_REP_FROM_NW(nIndexCtx, &SmsInfo);
		break;
	/*Start ICH17012018*/
	case 282 : /*  Dispute Response Financial Status Advice a/ Dispute Response Financial Reversal Status Advice  */
			GetSmsField (SMS_POS_CONDITION, &SmsInfo, tmpField, &tmpFieldLen);
			if(memcmp(tmpField, "13", 2) == 0)
			{
				FCT_DISPUTE_RESP_ADV_FROM_NW (nIndexCtx, &SmsInfo);
			}
			break;
	/*End ICH17012018*/
	case 400:		/* Reversal Messages */
	case 401:		/* Reversal Messages */
		result = FCT_REV_REQ_FROM_NW(nIndexCtx, &SmsInfo);
		break;

	case 410:		/* Reversal Response */
		result = FCT_REV_REP_FROM_NW(nIndexCtx, &SmsInfo);
		break;

	case 420 :
	case 421: 				
		FCT_REV_ADV_FROM_NW(nIndexCtx, &SmsInfo);
		break;   

	case 422 : /*  ChargeBack Advice or ChargeBack Reversal or Fees Collection or Message text */
		/*  It will follow the same processing */
		/* message type depends on POS Condition Code */
		GetSmsField(SMS_POS_CONDITION, &SmsInfo,tmpField, &tmpFieldLen);
		switch (atoi (tmpField))
		{
		case 17 : /* ChargeBack */
			FCT_CHGBK_ADV_FROM_NW (nIndexCtx, &SmsInfo);
			break;

		case 54 : /* ChargeBack Reversal */
			FCT_CHGBK_REV_FROM_NW (nIndexCtx, &SmsInfo);
			break;

		default:  /*  message type depends on Processing Code    */
			GetSmsField(SMS_PROC_CODE, &SmsInfo, tmpField, &tmpFieldLen);
			if ( !memcmp(tmpField, "19", 2) || !memcmp (tmpField, "29", 2) )
				FCT_FEE_ADV_FROM_NW (nIndexCtx, &SmsInfo);
			else
				trace_event ("Sms Interface Can Not process this message" , PROCESSING);
			break;
		}
		break;

	case 430 : /*   Reversal Advice Response       */
		FCT_REV_REP_FROM_NW (nIndexCtx, &SmsInfo);
		break;

	case 500 :
	case 520 :
	case 521 : /*   Reconciliation Advice   */
		FCT_REC_ADV_FROM_NW (nIndexCtx, &SmsInfo);
		break;

	case 600 : /*  Administrative Text Message Request */
	case 620 : /*  Administrative Text Message Advice  */
		FCT_TXT_MSG_FROM_NW (nIndexCtx, &SmsInfo);
		break;

	case 610 :
	case 630 : /*  Administrative Text Message Response  */
		FCT_OUTG_REP_FROM_NW (nIndexCtx, &SmsInfo);
		break;
	}

	InitSmsInfo( &SmsInfo);   
}
