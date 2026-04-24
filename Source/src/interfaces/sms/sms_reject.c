#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <queue.h>
#include <dump.h>
#include <list_thr.h>
#include <security_data.h>

int  RejectedMessage (int nIndexCtx, TSSmsInfo  *SmsInfo, int          RejectCode)
{
	TSBalExchange   BalMessage;
	TSNodeData      Node;
	TSIsoInfo       IsoInfo;
	char            detail[80];
	char            tmpField[256];
	char            sTlvBuffer [LG_MAX];
	char            SourceId[9];
	int             tmpFieldLen;
	int             Length;

	memset (detail, 0, sizeof(detail));
	memset (sTlvBuffer, 0, sizeof(sTlvBuffer));
	memset (tmpField, 0, sizeof(tmpField));
	memset (SourceId, 0, sizeof(SourceId));
	sprintf (detail, "Start  RejectedMessage(): code %04d", RejectCode);
	trace_event (detail, PROCESSING);

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
		trace_event ("End RejectedMessage (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;

	}

	if(SmsInfo->nMsgType == 220 )
	{
		/* OutgoingRespReceived(); */
		memcpy ( sTlvBuffer , Node.sSndBuf, Node.nSndBufLen );

		Length = OutgoingTimeOutTraitement ( nIndexCtx , sTlvBuffer);
		trace_event ("End  RejectedMessage(OK)", PROCESSING);
		return (OK);
	} 

	if (SmsInfo->nMsgType == 600 )
	{
		memcpy ( sTlvBuffer , Node.sSndBuf, Node.nSndBufLen );

		Length = OutgoingTimeOutTraitement ( nIndexCtx , sTlvBuffer);

		trace_event ("End  RejectedMessage(OK)", PROCESSING);
		return ( OK );
	}

	InitIsoInfo (&IsoInfo);
	AnalyseIso (Node.sFstBuf, &IsoInfo);
	IsoInfo.nMsgType = SmsInfo->nMsgType + 1010;
	sprintf(tmpField, "%.3d", FIELDS_CTL_ERROR);
	InsertIsoField (ISO_RESPONSE_CODE, &IsoInfo, tmpField, 3);

	/* forward it to requester */
	GetIsoField (ISO_SECURITY_DATA, &IsoInfo, tmpField, &tmpFieldLen);
	GetSecurityData(SOURCE_PID,tmpField,SourceId,&tmpFieldLen);


	BalMessage.sHeader.nPartnerId = atoi(SourceId);
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = IsoInfo.nMsgType;
	Length = IsoBuildMsg (BalMessage.sBody, &IsoInfo);
	BalMessage.sHeader.nLen = Length;

	if (WriteBalMsg (nBalMsg, &BalMessage) != OK)
	{
		trace_event ("End RejectedMessage(): WriteBal() failed.", ERROR);
		IntWriteBalExeption(nIndexCtx,&IsoInfo); /* ELKAM PLUTONL-4061 */
		return (NOK);
	}

	trace_event ("End  RejectedMessage(OK)", PROCESSING);
	return (OK);

}



