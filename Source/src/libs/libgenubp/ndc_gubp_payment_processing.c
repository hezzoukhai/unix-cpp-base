/*NOT USED*/
#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <sys/time.h>
#include <define.h>
#include <list_thr.h>
#include <hsm_inc.h>
#include <iso_hps.h>
#include <tlv_fields.h>
#include <tlv_autho.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <resources.h>
#include <service.h>
#include <event_auth.h>
#include <maj_card_activity.h>
#include <gubp_opcode.h>
#include <thr_ndc_extern.h>

#include <ascii.h>
#include <command.h>
#include <actioncode.h>
#include <paramdef.h>
#include <langue.h>



int Ubp_Payment_Processing	(	int nIndiceCtx, char *sTlvBuffer )
{

	/* call WS route iso8583*/
	if ( UbpPayment_Serv   ( nIndiceCtx, sTlvBuffer ) != OK )
	{
		trace_event ( "Error Returned ...", ERROR );
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,"100",3);
		PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,"115",3);

		PrintTlvBuffer(sTlvBuffer);
		return NOK;
	}

	return OK;

}











