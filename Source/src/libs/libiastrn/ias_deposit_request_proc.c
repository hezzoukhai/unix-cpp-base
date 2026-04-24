#include <iso_hps.h>
#include <maj_card_activity.h>
#include <tlv_fields.h>
#include <p7_services.h>
#include <resources.h>

int IASDepositReqProc(int nIndiceCtx, char*	sTlvBuffer)
{
	
	int			nRetCode;
	int			nLength;

	trace_event("Start IASDepositReqProc()",PROCESSING);

	nRetCode = Deposit_Inquiry(nIndiceCtx, sTlvBuffer);

	trace_event("End   IASDepositReqProc(OK)",PROCESSING);
	return nRetCode;
}

