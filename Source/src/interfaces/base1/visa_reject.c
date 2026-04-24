#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_visa.h>
#include <queue.h>
#include <dump.h>
#include <list_thr.h>
#include <visa_define.h>
#include <resources.h>
#include <tlv_private.h>
#include <p7_sid.h>


int 
RejectedMessage(int nIndexCtx, TSVisaInfo * VisaInfo, int RejectCode)
{
	TSBalExchange   BalMessage;
	TSNodeData      Node;
	TSIsoInfo       IsoInfo;
	char            detail[MAX_LINE_TRC];
	char            tmpField[256];
	char            SourceId[7];
	int             tmpFieldLen;
	int             Length;
	/* START [ASO17012020] PROD00073688 */
	char			sTlvBuffer[LG_MAX];
	int             result;
	
	/* END [ASO17012020] PROD00073688 */
	
	sprintf(detail, "Start  RejectedMessage(): code %04d", RejectCode);
	trace_event(detail, PROCESSING);
	
	/* START [ASO17012020] PROD00073688 */

	sprintf(detail, "M.T.I : [%04d]", VisaInfo->nMsgType);
	trace_event(detail, PROCESSING);
	
	if(VisaInfo->nMsgType == 110)
	{
		result = ReversalReqFromNw(nIndexCtx, VisaInfo, &IsoInfo);
		
		result = ServiceProcessing(		nIndexCtx,
										ResourceId,
										"ADM",
										AcquirerBank,
										"Y",
										"G",
										"N",
										sTlvBuffer, 
										&IsoInfo);
										
		if( result != OK)
		{
			sprintf(detail, "End  RejectedMessage(NOK): ServiceProcessing failure");
			trace_event(detail, PROCESSING);
			return (NOK);
		}
	}
	else if(VisaInfo->nMsgType == 100)
	{
		GetVisaField(VISA_AUDIT_NBR, VisaInfo, tmpField, &tmpFieldLen);
	

		sprintf(detail, "%04d", VisaInfo->nMsgType);
		InitNodeData(&Node);
		memcpy(Node.szFstKey, detail, 4);
		memcpy(Node.szFstKey + 4, tmpField, 6);

		switch (FetchRequest('B', &Node)) 
		{
		case ENOEXIST:
			trace_event("FetchRequest failed (ENOEXIST)", ERROR);
			trace_event("End RejectedMessage (NOK)", ERROR);
			return (NOK);

		case ELATE:
			break;

		}
		
		InitIsoInfo(&IsoInfo);
		AnalyseIso(Node.sFstBuf, &IsoInfo);
		IsoInfo.nMsgType = VisaInfo->nMsgType + 1010;
		sprintf(tmpField, "%.3d", FIELDS_CTL_ERROR);
		InsertIsoField(ISO_RESPONSE_CODE, &IsoInfo, tmpField, 3);

		/* forward it to requester */
		GetIsoField(ISO_SECURITY_DATA, &IsoInfo, tmpField, &tmpFieldLen);
		memcpy(SourceId, tmpField + 10, 6);
		SourceId[6] = '\0';

		BalMessage.sHeader.nPartnerId = atoi(SourceId);
		BalMessage.sHeader.nSourceId = getpid();
		BalMessage.sHeader.nCommand = IsoInfo.nMsgType;
		Length = IsoBuildMsg(BalMessage.sBody, &IsoInfo);
		BalMessage.sHeader.nLen = Length;

		if (WriteBalMsg(nBalMsg, &BalMessage) != OK) 
		{
			trace_event("End RejectedMessage(): WriteBal() failed.", ERROR);
			IntWriteBalExeption(nIndexCtx,&IsoInfo); /* ELKAM PLUTONL-4061 */
			return (NOK);
		}
	}
	
	trace_event("End  RejectedMessage(OK)", PROCESSING);
	
	/* END [ASO17012020] PROD00073688 */
	return (OK);

}
