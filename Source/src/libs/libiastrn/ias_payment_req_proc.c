#include <iso_hps.h>
#include <maj_card_activity.h>
#include <tlv_fields.h>
#include <p7_services.h>
#include <resources.h>

int IASPaymentReqProc(int nIndiceCtx, char*	sTlvBuffer)
{
	int			nRetCode;
	int			nLength;

	trace_event("Start IASPaymentReqProc()",PROCESSING);

	nRetCode = CheckAccAvailable(nIndiceCtx, sTlvBuffer);

	trace_event("End   IASPaymentReqProc(OK)",PROCESSING);
	return nRetCode;
}

