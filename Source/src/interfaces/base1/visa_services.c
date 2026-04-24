#include <iso_hps.h>
#include <visa_param.h>
#include <tlv_private.h>
#include <define.h>
#include <queue.h>
#include <visa_fields.h>
#include <visa_define.h>
#include <iso_visa.h>
#include <list_thr.h>

/*****************************************************************************/
/* Stop interface upon request from daemon screen.                          */
/*****************************************************************************/
void 
FCT_STOP_INTERFACE(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	trace_event("Start  FCT_STOP_INTERFACE()", PROCESSING);

	shutdown_resource(nIndexCtx, ResourceId);

	trace_event("End  FCT_STOP_INTERFACE()", PROCESSING);
}


/*****************************************************************************/
/* Refresh resource parameters upon request from daemon screen module.      */
/*****************************************************************************/
void 
FCT_REFRESH_PARAMS(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	trace_event("Start  FCT_REFRESH_PARAMS()", PROCESSING);

	ReloadTransactionsContainers();

	trace_event("End  FCT_REFRESH_PARAMS()", PROCESSING);
}


/*****************************************************************************/
/* Indicate resource status to supervise resource module.                   */
/*****************************************************************************/
void 
FCT_REPORT_STATUS(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	TSTlvPrivate    TlvInfo;
	char            FieldData[256];
	int             FieldLen;


	trace_event("Start  FCT_REPORT_STATUS()", PROCESSING);

	IsoInfo->nMsgType = 1616;
	InitTlvPrivate(&TlvInfo);
	AddTlvPrivate(PRIS_INTERNAL_ID_TAG, &TlvInfo, ResourceInternalId,
		      strlen(ResourceInternalId));
	FieldLen = PrivateBuildTlv(FieldData, &TlvInfo);
	PutIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, FieldLen);
	dump_p7_struct(IsoInfo);
	ReplyIsoMsgToBal(nBalAdm, IsoInfo, 0, "000");

	trace_event("End  REPORT_STATUS()", PROCESSING);
}


/*****************************************************************************/
/* Report resource statistics to supervise resource module.                 */
/*****************************************************************************/
void 
FCT_STATISTICS(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	trace_event("Start  FCT_STATISTICS()", PROCESSING);

	trace_event("End  FCT_STATISTICS()", PROCESSING);
}

/*****************************************************************************/
/* */
/*****************************************************************************/
void 
FCT_LINE_DOWN(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	trace_event("Start  FCT_LINE_DOWN()", PROCESSING);

	/*
	 * update resource activity table to indicate that resource was
	 * stopped 
	 */
	if (ResourceActivity(nIndexCtx, ResourceId, STOP) != OK) {
		fprintf(stderr, "Resources_Ativity table update failed.\n");
		return;
	}
	/* communication is down, so update resource status */
	if (UpdateResource(nIndexCtx, ResourceId, CRITICAL, OFF) != OK) {
		fprintf(stderr, "UpdateResource() failed.\n");
		return;
	}
	trace_event("End  FCT_LINE_DOWN()", PROCESSING);
}

/*****************************************************************************/
/* */
/*****************************************************************************/
void 
FCT_LINE_UP(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	trace_event("Start  FCT_LINE_UP()", PROCESSING);

	/*
	 * update resource activity table to indicate that resource was
	 * launched 
	 */
	if (ResourceActivity(nIndexCtx, ResourceId, START) != OK) {
		fprintf(stderr, "Resources_Ativity table update failed.\n");
		return;
	}
	/* communication is up, so update resource status */
	if (UpdateResource(nIndexCtx, ResourceId, LIVE, SIGNON) != OK) {
		fprintf(stderr, "UpdateResource() failed.\n");
		return;
	}
	trace_event("End  FCT_LINE_UP()", PROCESSING);
}
/*****************************************************************************/
/*                     FZL20220822 PLUTONL-4979                                                      */
/*****************************************************************************/
int  FCT_KEY_REQUEST (int nIndexCtx, TSIsoInfo * IsoInfo, char  KeyType)
{
	TSNodeData      Node;
	int             Length;
	char            sBuffer[BUF_LG_MAX];
	char            sLine[MAX_LINE_TRC];
	char            sGmtDate[32];
	struct timeval stPurgeTime;	
	char            Stan[7];
	TSVisaInfo      msgInfo;
	TSPufInfo		PufInfo;

    sprintf(sLine, "Start  FCT_KEY_REQUEST( %cWK)", KeyType);
    trace_event (sLine, PROCESSING);
	
	memset(Stan, 0, sizeof(Stan));
	memset(sGmtDate, 0, sizeof(sGmtDate));
    memset(sBuffer, 0, sizeof(sBuffer));
   	
	InitNodeData(&Node);
    
	GetSafPurgeTimeMS(&stPurgeTime);
	Node.stPurgeTime = stPurgeTime;
    
	GetInternalStan(nIndexCtx, Stan);
	memcpy(Node.szFstKey, "1606", 4);
	memcpy(Node.szFstKey + 4, Stan, 6);
  
    
	Length = IsoBuildMsg(Node.sFstBuf, IsoInfo);
	
	Node.nFstBufLen = Length;
	
	if ( InsertRequest('B', &Node) != OK ) 
	{
		sprintf(sLine, "End FCT_KEY_REQUEST(%cWK) ERROR", KeyType);
        trace_event (sLine, ERROR);
		return (NOK);
	}

	InitVisaInfo(&msgInfo);
    msgInfo.nMsgType = 800;   

	current_gmt_date(sGmtDate);

	AddVisaField(VISA_XMIT_TIME, &msgInfo, sGmtDate + 2, 10);
	
	AddVisaField(VISA_AUDIT_NBR, &msgInfo,Stan, 6);
    
	AddVisaField(VISA_FORWD_ID, &msgInfo, AcquirerId, strlen(AcquirerId));
	
    AddVisaField(VISA_SECURITY_DATA, &msgInfo,"0000000100000000", 16);

    /* field  63*/
	InitPufInfo (&PufInfo);
	AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0002", 4);
	AddPufSubField (PUF_PREAUTH_TIME_LIMIT, &PufInfo, "0002", 4);
	Length = PufBuildField (sBuffer, &PufInfo);                          
	AddVisaField (VISA_PRIV_USE_FIELDS, &msgInfo, sBuffer, Length);
	/* end field 63 */
	
	
	if ( KeyType == 'I' )
		AddVisaField(VISA_NW_MNGMT_INFO,  &msgInfo, B1_F070_IWK_REQUEST, 3);
	else
        AddVisaField(VISA_NW_MNGMT_INFO,  &msgInfo, B1_F070_AWK_REQUEST, 3);

    memset(sBuffer, 0, sizeof(sBuffer));
	Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, &msgInfo);

	VisaBuildHeader(REQUEST, &(msgInfo.sHeader), NULL, Length, StationId);
	memcpy(sBuffer, (char *) &(msgInfo.sHeader), VISA_HDR_LEN);
	Length += VISA_HDR_LEN;

   	DumpVisa(sBuffer);

	if ( WriteLineMsg(sBuffer, Length) != OK ) 
	{
		FetchRequest('B', &Node);
		sprintf(sLine, "End FCT_KEY_REQUEST(%cWK) ERROR", KeyType);
        trace_event (sLine, ERROR);
		return (NOK);
	}

   sprintf(sLine, "End FCT_KEY_REQUEST(%cWK)", KeyType);
   trace_event (sLine, PROCESSING);

   return OK; 
}