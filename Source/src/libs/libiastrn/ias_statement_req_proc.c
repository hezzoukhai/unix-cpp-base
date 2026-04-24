#include <iso_hps.h>
#include <maj_card_activity.h>
#include <tlv_fields.h>
#include <p7_services.h>
#include <resources.h>

int IASStatementReqProc(int nIndiceCtx, char*	sTlvBuffer)
{
	
	int			nRetCode;
	int			nLength;

	trace_event("Start IASStatementReqProc()",PROCESSING);

#if IS_P7_IBE 
	nRetCode = Statement_Inquiry(nIndiceCtx, sTlvBuffer);
#endif
	
	trace_event("End   IASStatementReqProc(OK)",PROCESSING);
	return nRetCode;
}

