#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <queue.h>
#include <dump.h>
#include <list_thr.h>
#include <security_data.h>

int  ReturnedMessage (TSSmsInfo  *SmsInfo)
{
	TSBalExchange   BalMessage;
	TSNodeData      Node;
	TSIsoInfo       IsoInfo;
	char            detail[80];
	char            tmpField[256];
	char            SourceId[12];
	int             tmpFieldLen;
	int             Length;

	trace_event ("Start  ReturnedMessage()", PROCESSING);

	/* fetch original message from list */
	GetSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField, &tmpFieldLen);

	sprintf (detail, "%04d", SmsInfo->nMsgType);
	InitNodeData (&Node);
	memcpy (Node.szFstKey, detail, 4);
	memcpy (Node.szFstKey + 4,  tmpField, 6);

	switch (FetchRequest ('B', &Node))
	{
	case ENOEXIST:
		trace_event ("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event ("End ReturnedMessage (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;

	}
	MsgFwdRepReceived(Node.msgId);

	InitIsoInfo (&IsoInfo);
	AnalyseIso (Node.sFstBuf, &IsoInfo);
	IsoInfo.nMsgType = SmsInfo->nMsgType + 1010;
	InsertIsoField (ISO_RESPONSE_CODE, &IsoInfo, ISSUER_UNAVAILABLE_ACTION , 3);

	/* forward it to requester */
	GetIsoField (ISO_SECURITY_DATA, &IsoInfo, tmpField, &tmpFieldLen);
	GetSecurityData(SOURCE_PID,tmpField,SourceId,&tmpFieldLen);

	BalMessage.sHeader.nPartnerId = atoi(SourceId);
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = IsoInfo.nMsgType;
	Length = IsoBuildMsg (BalMessage.sBody, &IsoInfo);
	BalMessage.sHeader.nLen = Length;
	BalMessage.sHeader.msgId = Node.msgId;

	if (WriteBalMsg (nBalMsg, &BalMessage) != OK)
	{
		trace_event ("End ReturnedMessage(): WriteBal() failed.", ERROR);
		return (NOK);
	}

	MsgFwdRepReplied(BalMessage.sHeader.msgId);
	trace_event ("End  ReturnedMessage(OK)", PROCESSING);
	return (OK);

}

