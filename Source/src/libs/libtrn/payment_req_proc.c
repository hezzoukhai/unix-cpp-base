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
#include <general_macros.h>
#include <p7_sid.h>
#include <p7_define.h>


/******************************************************************************/
#define SERV_ON_TIMEOUT_PROC		"OnTimeoutServiceRequest"
#define SERV_ON_RESP_PROC			"AuthReplyProc"
#define SERV_STANDIN_PROC			"PaymentRequestProc"
/******************************************************************************/


/******************************************************************************/
int PaymentRequestProc(int nIndiceCtx, char*	sTlvBuffer)
{
	/*JOU20170801: remove old PaymentRequestProc code, and call AuthRequestProc instead. see PROD00044784*/
	return AuthRequestProc(nIndiceCtx, sTlvBuffer);
}
