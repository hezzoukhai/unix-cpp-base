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

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int StopListFileUpdate(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	char        	sMsgErr[255 + 1];
	char        	sActionCode[3 + 1];
	
	
	trace_event("Start StopListFileUpdate()", PROCESSING);

	memset(sMsgErr, '\0', sizeof (sMsgErr));
	memset(sActionCode, '\0', sizeof (sActionCode));
	
	nResult = ProcessStopList(nIndexCtx,sTlvBuffer,sMsgErr,sActionCode);
	if( nResult == OK )
	{
		trace_event("End   StopListFileUpdate(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   StopListFileUpdate(NOK)", PROCESSING);
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
int GetAccountBalance(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	
	
	
	trace_event("Start GetAccountBalance()", PROCESSING);

	nResult = BalanceInquiry(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End   GetAccountBalance(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   GetAccountBalance(NOK)", PROCESSING);
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
int ActivatePAN(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	
	
	
	trace_event("Start ActivatePAN()", PROCESSING);

	nResult = Activate_Card(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End   ActivatePAN(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   ActivatePAN(NOK)", PROCESSING);
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
int RegisterChequeRequest(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	
	
	
	trace_event("Start RegisterChequeRequest()", PROCESSING);

	nResult = Cheque_Inquiry(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End   RegisterChequeRequest(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   RegisterChequeRequest(NOK)", PROCESSING);
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
int RegisterDepositRequest(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	
	
	
	trace_event("Start RegisterDepositRequest()", PROCESSING);

	nResult = Deposit_Inquiry(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End   RegisterDepositRequest(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   RegisterDepositRequest(NOK)", PROCESSING);
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
int RegisterPinChange(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	
	
	
	trace_event("Start RegisterPinChange()", PROCESSING);

	nResult = PinChange_Inquiry(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End   RegisterPinChange(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   RegisterPinChange(NOK)", PROCESSING);
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
int PINChangeRevert(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;
	char			slCardNumber[24 + 1];
	
	
	trace_event("Start PINChangeRevert()", PROCESSING);

	memset(slCardNumber,'\0',sizeof(slCardNumber));

	GetTlvBuffer(AUTO_PAN, sTlvBuffer, slCardNumber, &nLength);

	nResult = PinInfoReversal(nIndexCtx,sTlvBuffer);

	memset(slCardNumber,'\0',sizeof(slCardNumber));
	if( nResult == OK )
	{
		trace_event("End   PINChangeRevert(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   PINChangeRevert(NOK)", PROCESSING);
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
int RegisterStatementRequest(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	char        	sMsgErr[255 + 1];
	char        	sActionCode[3 + 1];
	
	
	trace_event("Start RegisterStatementRequest()", PROCESSING);

	memset(sMsgErr, '\0', sizeof (sMsgErr));
	memset(sActionCode, '\0', sizeof (sActionCode));
	
	nResult = Statement_Request(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End   RegisterStatementRequest(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   RegisterStatementRequest(NOK)", PROCESSING);
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
int GetRIB(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	

	trace_event("Start GetRIB()", PROCESSING);

	nResult = Rib_Inquiry(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End   GetRIB(OK)", PROCESSING);
	}
	else
	{
		trace_event("End   GetRIB(NOK)", PROCESSING);
	}

	return nResult;
}

