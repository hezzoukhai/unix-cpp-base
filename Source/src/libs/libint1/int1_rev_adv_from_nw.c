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
/* INT1_FCT_REV_ADV_FROM_NW()                                                  */
/****************************************************************************/
int INT1_FCT_REV_ADV_FROM_NW(int nIndexCtx, MsgInfo * pkMsgInfo, TSIsoInfo*  pkIsoInfo, char* sTblInd)
{
	MsgInfo			kNewMsgInfo;
	int             result;
	int             Length;
	char            sBuffer[1024];
	char            sPid[8 + 1];
	char			sTlvBuffer[LG_MAX];
	


	trace_event("Start  INT1_FCT_REV_ADV_FROM_NW()", PROCESSING);

	memset(sBuffer, 0, sizeof(sBuffer));
	
	

	pkIsoInfo->msgId = MsgIsoReqReceived(ResourceId,pkIsoInfo);
	result = ServiceProcessing(		nIndexCtx,
									ResourceId,
									sTblInd,
									AcquirerBank,
									"Y",
									"G",
									"N",
									sTlvBuffer, 
									pkIsoInfo);

	switch (result) {

	case NOK:      /* the interface itself responds */
	case OK:      /* the interface itself responds */

		trace_event("End  INT1_FCT_REV_ADV_FROM_NW(OK)", PROCESSING);
		return I1RET_PROCESSED;
		break;

	case SEVERE_ERROR:
		int1_shutdown_resource(nIndexCtx, ResourceId);
		break;
	}


	trace_event("End  INT1_FCT_REV_ADV_FROM_NW(OK)", PROCESSING);
	return (OK);
}


int INT1_FCT_REV_REP_FROM_NW(int nIndexCtx, MsgInfo * pkMsgInfo, TSIsoInfo*  pkIsoInfo, char* sTblInd)
{

}
