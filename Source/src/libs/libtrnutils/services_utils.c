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
#include <signal.h>
#include <general_macros.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <p7_services.h>



int GetAuthRepeatAction(int nIndiceCtx,char* sTlvBuffer, char* cAuthAction)
{
	char		cAppRepeatOpt = RP_ACT_REPLY;		/* for a repeat of an approved auth*/
	char		cDecRepeatOpt = RP_ACT_FORWARD;		/* for a repeat of a declined auth*/
	int			nRepeatPeriod = 10;					/* if exceeded, a repeat is considered as duplicate */
	char		sOrigMsgType[4 + 1];
	char		sFieldData[256];
	char		sActionCode[3 + 1];
	char		sRoutingCode[6 + 1];
	char		sTableInd[3 + 1];
	int			nLength;
	char		sLine[MAX_LINE_TRC];
	char		sActionFlag[1 + 1];




	trace_event("Start GetAuthRepeatAction()", PROCESSING);

	memset(sOrigMsgType,'\0',sizeof(sOrigMsgType));
	memset(sActionCode,'\0',sizeof(sActionCode));
	memset(sRoutingCode,'\0',sizeof(sRoutingCode));
	memset(sActionFlag,'\0',sizeof(sActionFlag));	/*20200701 PLUTONL-1968*/

	cAuthAction[0] = RP_ACT_DECLINE;


	GetTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, &nLength);

	/*20200701 PLUTONL-1968*/
	GetTlvBuffer (AUTO_ACTION_FLAG, sTlvBuffer, sActionFlag ,  &nLength);

	if(sActionFlag[0] == ACTION_FLAG_APPROVAL)	/*20200701 PLUTONL-1968*/
	{
		cAuthAction[0] = cAppRepeatOpt;
	}
	else
	{
		cAuthAction[0] = cDecRepeatOpt;
	}

	GetTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sRoutingCode, &nLength);

	if(memcmp(sRoutingCode,STAND_IN_RESOURCE,RESOURCE_ID_LEN) == 0 &&
		cAuthAction[0] == RP_ACT_FORWARD )
	{
		cAuthAction[0] = RP_ACT_DECLINE;
	}

	sprintf(sLine,"End   GetAuthRepeatAction(OK): (%c)",cAuthAction[0]);
	trace_event(sLine, PROCESSING);
	return OK;

}

