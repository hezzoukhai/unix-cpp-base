#include <iso_hps.h>
#include <visa_param.h>
#include <tlv_private.h>
#include <define.h>
#include <queue.h>

#include <int1_struct.h>
#include <int1.h>
/*****************************************************************************/
/* Stop interface upon request from daemon screen.                          */
/*****************************************************************************/
void  INT1_FCT_STOP_INTERFACE(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	trace_event("Start  FCT_STOP_INTERFACE()", PROCESSING);

	int1_shutdown_resource(nIndexCtx, ResourceId);

	trace_event("End  FCT_STOP_INTERFACE()", PROCESSING);
}


/*****************************************************************************/
/* Refresh resource parameters upon request from daemon screen module.      */
/*****************************************************************************/
void INT1_FCT_REFRESH_PARAMS(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	trace_event("Start  FCT_REFRESH_PARAMS()", PROCESSING);

	trace_event("End  FCT_REFRESH_PARAMS()", PROCESSING);
}


/*****************************************************************************/
/* Indicate resource status to supervise resource module.                   */
/*****************************************************************************/
void INT1_FCT_REPORT_STATUS(int nIndexCtx, TSIsoInfo * IsoInfo)
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
void INT1_FCT_STATISTICS(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	trace_event("Start  FCT_STATISTICS()", PROCESSING);

	trace_event("End  FCT_STATISTICS()", PROCESSING);
}

/*****************************************************************************/
/* */
/*****************************************************************************/
void INT1_FCT_LINE_DOWN(int nIndexCtx, TSIsoInfo * IsoInfo)
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
void INT1_FCT_LINE_UP(int nIndexCtx, TSIsoInfo * IsoInfo)
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
