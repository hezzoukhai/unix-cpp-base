#include <iso_hps.h>
#include <maj_card_activity.h>
#include <tlv_fields.h>
#include <p7_services.h>
#include <resources.h>

int IASAuthReqProc(int nIndiceCtx, char*	sTlvBuffer)
{
	
	int			nRetCode;
	int			nLength;

	trace_event("Start IASAuthReqProc()",PROCESSING);

	PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
	


	if (CheckAccAvailable(nIndiceCtx, sTlvBuffer) != OK)
	{
		return (NOK);
	}

	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);


	nRetCode = UpdateBalance  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
	if ( nRetCode != OK )
	{
		trace_event ( "Can Not Update Balance  !!!", FATAL );
		/* What is the action to do */
	}

	trace_event("End   IASAuthReqProc(OK)",PROCESSING);
	return nRetCode;
}

