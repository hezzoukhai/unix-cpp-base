#include <iso_hps.h>
#include <maj_card_activity.h>
#include <tlv_fields.h>
#include <p7_services.h>
#include <resources.h>

int IASChequeReqProc(int nIndiceCtx, char*	sTlvBuffer)
{
	
	int			nRetCode;
	int			nLength;

	trace_event("Start IASChequeReqProc()",PROCESSING);

	nRetCode = Cheque_Inquiry(nIndiceCtx, sTlvBuffer);

	trace_event("End   IASChequeReqProc(OK)",PROCESSING);
	return nRetCode;
}

