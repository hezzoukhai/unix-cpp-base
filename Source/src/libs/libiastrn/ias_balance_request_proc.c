#include <iso_hps.h>
#include <maj_card_activity.h>
#include <tlv_fields.h>
#include <p7_services.h>
#include <resources.h>

int IASBalReqProc(int nIndiceCtx, char*	sTlvBuffer)
{
	
	int			nRetCode;
	int			nLength;

	trace_event("Start IASBalReqProc()",PROCESSING);

	nRetCode = BalanceInquiry(nIndiceCtx, sTlvBuffer);

	trace_event("End   IASBalReqProc(OK)",PROCESSING);
	return nRetCode;
}

