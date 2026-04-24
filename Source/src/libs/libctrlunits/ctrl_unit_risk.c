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
/******************************************************************************/
typedef enum {

CTRL_U_CARD_STATUS		= 1,
CTRL_U_CARD_DELIVERY	,
CTRL_U_CARD_ACTIVATION	,
CTRL_U_STOPLIST			,
CTRL_U_PTC				,
CTRL_U_CVV2_COUNTER		,
CTRL_U_EXP_DT_COUNTER	,
CTRL_U_DOMAIN_USAGE		,
CTRL_U_AVS				,
CTRL_U_EXPIRY_DATE		,
CTRL_U_ATC				,
CTRL_U_SERVICE_CODE		,
CTRL_U_SERVICE_SETUP	,
CTRL_U_TVR_CVR			,
	
} E_RSK_CTRL_UNIT;


/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlCardStatus(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlCardStatus()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_CARD_STATUS);
	if( nResult == OK )
	{
		trace_event("End   CtrlCardStatus(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlCardStatus(NOK)", PROCESSING);
	}

	return nResult;
}

int CtrlCardDelivery(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlCardDelivery()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_CARD_DELIVERY);
	if( nResult == OK )
	{
		trace_event("End   CtrlCardDelivery(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlCardDelivery(NOK)", PROCESSING);
	}

	return nResult;
}


int CtrlCardActivation(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlCardActivation()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_CARD_ACTIVATION);
	if( nResult == OK )
	{
		trace_event("End   CtrlCardActivation(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlCardActivation(NOK)", PROCESSING);
	}

	return nResult;
}
int CtrlStopLists(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlStopLists()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_STOPLIST);
	if( nResult == OK )
	{
		trace_event("End   CtrlStopLists(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlStopLists(NOK)", PROCESSING);
	}

	return nResult;
}

int CtrlPTC(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlPTC()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_PTC);
	if( nResult == OK )
	{
		trace_event("End   CtrlPTC(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlPTC(NOK)", PROCESSING);
	}

	return nResult;
}

int CtrlCVV2TC(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlCVV2TC()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_CVV2_COUNTER);
	if( nResult == OK )
	{
		trace_event("End   CtrlCVV2TC(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlCVV2TC(NOK)", PROCESSING);
	}

	return nResult;
}

int CtrlExpDtTC(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlExpDtTC()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_EXP_DT_COUNTER);
	if( nResult == OK )
	{
		trace_event("End   CtrlExpDtTC(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlExpDtTC(NOK)", PROCESSING);
	}

	return nResult;
}

int CtrlDomainUsage(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlDomainUsage()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_DOMAIN_USAGE);
	if( nResult == OK )
	{
		trace_event("End   CtrlDomainUsage(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlDomainUsage(NOK)", PROCESSING);
	}

	return nResult;
}

int CtrlAVS(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlAVS()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_AVS);
	if( nResult == OK )
	{
		trace_event("End   CtrlAVS(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlAVS(NOK)", PROCESSING);
	}

	return nResult;
}

int CtrlExpiryDate(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlExpiryDate()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_EXPIRY_DATE);
	if( nResult == OK )
	{
		trace_event("End   CtrlExpiryDate(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlExpiryDate(NOK)", PROCESSING);
	}

	return nResult;
}

int CtrlATC(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlATC()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_ATC);
	if( nResult == OK )
	{
		trace_event("End   CtrlATC(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlATC(NOK)", PROCESSING);
	}

	return nResult;
}
int CtrlServiceCode(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlServiceCode()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_SERVICE_CODE);
	if( nResult == OK )
	{
		trace_event("End   CtrlServiceCode(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlServiceCode(NOK)", PROCESSING);
	}

	return nResult;
}
int CtrlServiceSetup(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlServiceSetup()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_SERVICE_SETUP);
	if( nResult == OK )
	{
		trace_event("End   CtrlServiceSetup(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlServiceSetup(NOK)", PROCESSING);
	}

	return nResult;
}
int CtrlTVRCVR(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlTVRCVR()", PROCESSING);

	nResult = ControlRiskUnit(nIndexCtx,sTlvBuffer,CTRL_U_TVR_CVR);
	if( nResult == OK )
	{
		trace_event("End   CtrlTVRCVR(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlTVRCVR(NOK)", PROCESSING);
	}

	return nResult;
}

int CtrlRisk(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlRisk()", PROCESSING);

	nResult = ControlRisk(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End   CtrlRisk(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlRisk(NOK)", PROCESSING);
	}

	return nResult;
}

int CtrlRiskOffus(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlRiskOffus()", PROCESSING);

	nResult = ControlRiskOffus(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End   CtrlRiskOffus(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlRiskOffus(NOK)", PROCESSING);
	}

	return nResult;
}


int CtrlPANLuhnKey(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;

	trace_event("Start CtrlPANLuhnKey()", PROCESSING);

	nResult = ControlLuhnKey(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End   CtrlPANLuhnKey(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   CtrlPANLuhnKey(NOK)", PROCESSING);
	}

	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlFraudRules(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;

	trace_event("Start CtrlFraudRules()", PROCESSING);

	nResult = FraudControl(nIndexCtx, sTlvBuffer);
	
	trace_event("End CtrlFraudRules(OK)", PROCESSING);
	return nResult;
}




