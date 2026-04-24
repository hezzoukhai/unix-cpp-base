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
#include <p7_services.h>
#include <hps_iso_8583.h>
/******************************************************************************\
* Return: NOK when duplicate or repetition
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlDuplicateAuthorization(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	
	char			sAuthFlag[4];
	char			sOrigAuthResult[4];
	char			cRepeatAction[4];
	char			sTmpData[256];
	int				nResult;
	int				nLength;

	trace_event("Start CtrlDuplicateAuthorization()", PROCESSING);


	memset(sAuthFlag,'\0',sizeof(sAuthFlag));
	memset(sOrigAuthResult,'\0',sizeof(sOrigAuthResult));
	memset(cRepeatAction,'\0',sizeof(cRepeatAction));
	memset(sTmpData,'\0',sizeof(sTmpData));
	
	
	nResult = GetOriginalAuthData	(	nIndexCtx,
										sTlvBuffer,
										sAuthFlag,
										sOrigAuthResult);
	if (nResult == OK)
	{
		trace_event("Found a Duplicate Request. ", PROCESSING);
		if( sOrigAuthResult[0] == AST_DUP_AUTH )
		{
			PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, P7_F039_DUP_REC, P7_F039_LEN);
			memset(sTmpData,' ',P7_F038_LEN);
			PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, sTmpData, P7_F038_LEN);
			trace_event("End AuthRequestProc(OK): Duplicate authorization", PROCESSING);
			return (NOK);
		}

		if( sOrigAuthResult[0] == AST_REP_AUTH )
		{
			if( sAuthFlag[0] != 'R' )
			{
				nResult = GetAuthRepeatAction(nIndexCtx,sTlvBuffer,cRepeatAction);
				if( cRepeatAction[0] == RP_ACT_REPLY )
				{
					trace_event("End AuthRequestProc(OK): Reply Response", PROCESSING);
					return (NOK);
				}
				if( cRepeatAction[0] == RP_ACT_DECLINE )
				{
					PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, P7_F039_DECLINE, P7_F039_LEN);
					memset(sTmpData,' ',P7_F038_LEN);
					PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, sTmpData, P7_F038_LEN);
					trace_event("End AuthRequestProc(OK): Declined", PROCESSING);
					return (NOK);
				}
			}
		}
	}
	else
	{
		if( sOrigAuthResult[0] == AST_INC_AUTH )
		{
			PutTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, P7_F024_SUPP_AMNT_ESTMD, P7_F024_LEN);
		}
	}


	trace_event("End CtrlDuplicateAuthorization(OK)", PROCESSING);
	return OK;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlAuthorRequestData(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;

	trace_event("Start CtrlAuthorRequestData()", PROCESSING);

	nResult = CheckReqTlvFields(nIndexCtx,sTlvBuffer);

	trace_event("End CtrlAuthorRequestData(OK)", PROCESSING);
	return nResult;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int GetAuthorRoutingChannel(int nIndexCtx, char*	sTlvBuffer, char routing_var_opt, char* sAddInfo)
{
	char			sLine					[MAX_LINE_TRC];
	char 			sPrimResource			[RESOURCE_ID_LEN + 1];
	char 			sSecResource			[RESOURCE_ID_LEN + 1];
	char			sExternalResource		[RESOURCE_ID_LEN + 1];
	char			sRoutingInfo_1			[RES_INFO_LEN + 1];
	char			sRoutingInfo_2			[RES_INFO_LEN + 1];
	char			sExternalRoutingInfo	[RES_INFO_LEN + 1];
	char			slCardNumber[24 + 1];
	int				nResult;
	int				nLength;

	trace_event("Start GetAuthorRoutingChannel()", PROCESSING);

	
	memset(sPrimResource,'\0',sizeof(sPrimResource));
	memset(sSecResource,'\0',sizeof(sSecResource));
	memset(sExternalResource,'\0',sizeof(sExternalResource));
	memset(sRoutingInfo_1,'\0',sizeof(sRoutingInfo_1));
	memset(sRoutingInfo_2,'\0',sizeof(sRoutingInfo_2));
	memset(sExternalRoutingInfo,'\0',sizeof(sExternalRoutingInfo));
	memset(slCardNumber,'\0',sizeof(slCardNumber));
	
	
	nResult = GetAuthRoutingChannel(	nIndexCtx,
										routing_var_opt,
										sTlvBuffer,
										sPrimResource,
										sSecResource);


	if( nResult != OK )
	{
		trace_event("End GetAuthorRoutingChannel(NOK)", PROCESSING);
		return NOK;
	}

	PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource, RESOURCE_ID_LEN);

	GetTlvBuffer(AUTO_ROUTING_RES_INFO_1, sTlvBuffer, sRoutingInfo_1, &nLength);
	GetTlvBuffer(AUTO_ROUTING_RES_INFO_2, sTlvBuffer, sRoutingInfo_2, &nLength);


	memcpy(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN);
	if (memcmp(sPrimResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) == 0)
	{
		memcpy(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
	}
	else if (memcmp(sPrimResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) != 0)
	{

		nResult = CheckResourceLive(sRoutingInfo_1);
		if (nResult == OK)
		{
			memcpy(sExternalResource, sPrimResource, RESOURCE_ID_LEN);
			memcpy(sExternalRoutingInfo,sRoutingInfo_1,RES_INFO_LEN);
		}
	}

	/* if we didn't find an available resource*/
	if (memcmp(sExternalResource, NO_ROUTING_RESOURCE, 6) == 0)
	{
		if (memcmp(sSecResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) == 0)
		{
			memcpy(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
		}
		else if (memcmp(sSecResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) != 0)
		{
			nResult = CheckResourceLive(sRoutingInfo_2);
			if (nResult == OK)
			{
				memcpy(sExternalResource, sSecResource, RESOURCE_ID_LEN);
				memcpy(sExternalRoutingInfo,sRoutingInfo_2,RES_INFO_LEN);
			}
		}
	}

	if (memcmp(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) == 0 )
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
		trace_event("End GetAuthorRoutingChannel(NOK): Issuer not available", PROCESSING);
		return(NOK);
	}


	if ( memcmp(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) != 0 )
	{
		GetTlvBuffer(AUTO_PAN, sTlvBuffer, slCardNumber, &nLength);

		if (CardInSaf(nIndexCtx, slCardNumber) == OK)
		{

			if (debug_option == 1)
			{
				sprintf(sLine, "Card [%s] Found In Saf File ", slCardNumber); trace_event(sLine, PROCESSING);
			}
			/* we can't forward this request, check if we can process it in standin mode*/			
			if (memcmp(sSecResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) == 0)
			{
				/* YDA20171130 : Set reason code to P7_F025_CIS_STANDIN instead of 1005 */
				PutTlvBuffer(AUTO_MESSAGE_REASON_CODE, sTlvBuffer, P7_F025_CIS_STANDIN, P7_F025_LEN);
				memcpy(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
			}
			else
			{
				PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
				trace_event("End GetAuthorRoutingChannel(OK): Card In Saf(Issuer not avail)", PROCESSING);
				return(NOK);
			}
		}

	}
	else
	{
		if (memcmp(sPrimResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) != 0)
		{
			PutTlvBuffer(AUTO_MESSAGE_REASON_CODE, sTlvBuffer, P7_F025_NA_STANDIN, P7_F025_LEN);
		}
	}

	PutTlvBuffer(AUTO_SEL_ROUTING_CODE, sTlvBuffer, sExternalResource, RESOURCE_ID_LEN);
  
  	if(strlen(sExternalRoutingInfo) != 0){
		PutTlvBuffer(AUTO_SEL_ROUTING_INFO, sTlvBuffer, sExternalRoutingInfo, RES_INFO_LEN);
    }

	if (memcmp(sExternalResource, STAND_IN_RESOURCE, 6) != 0)
	{
		PutTlvBuffer(AUTO_ON_OFF_ROUTING, sTlvBuffer, "ON", 2);
		if (GetSecurityFlags(nIndexCtx, sTlvBuffer, "ON") != OK)
		{
			return NOK;
		}
	}
	
	nResult = GetSecurityChecksParams(nIndexCtx,sTlvBuffer);
	if( nResult != OK )
	{
		PrintTlvBuffer(sTlvBuffer);
		return ( NOK );
	}



	trace_event("End   GetAuthorRoutingChannel(*OK*)", PROCESSING);
	return OK;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int GetAuthorRouting(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	int nResult;

	nResult =  GetAuthorRoutingChannel(nIndexCtx, sTlvBuffer, 'M', sAddInfo);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int GetIssuerBinAuthorRouting(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	int nResult;

	nResult =  GetAuthorRoutingChannel(nIndexCtx, sTlvBuffer, 'I', sAddInfo);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int GetBinAuthorRouting(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	int nResult;

	nResult =  GetAuthorRoutingChannel(nIndexCtx, sTlvBuffer, 'B', sAddInfo);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int GetTokenRouting(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	char			sLine					[MAX_LINE_TRC];
	char 			sPrimResource			[RESOURCE_ID_LEN + 1];
	char 			sSecResource			[RESOURCE_ID_LEN + 1];
	char			sExternalResource		[RESOURCE_ID_LEN + 1];
	char			sRoutingInfo_1			[RES_INFO_LEN + 1];
	char			sRoutingInfo_2			[RES_INFO_LEN + 1];
	char			sExternalRoutingInfo	[RES_INFO_LEN + 1];
	char			slCardNumber[24 + 1];
	int				nResult;
	int				nLength;

	trace_event("Start GetTokenRouting()", PROCESSING);

	
	memset(sPrimResource,'\0',sizeof(sPrimResource));
	memset(sSecResource,'\0',sizeof(sSecResource));
	memset(sExternalResource,'\0',sizeof(sExternalResource));
	memset(sRoutingInfo_1,'\0',sizeof(sRoutingInfo_1));
	memset(sRoutingInfo_2,'\0',sizeof(sRoutingInfo_2));
	memset(sExternalRoutingInfo,'\0',sizeof(sExternalRoutingInfo));
	memset(slCardNumber,'\0',sizeof(slCardNumber));
	
	
	nResult = GetAuthRouting(	nIndexCtx,
								sTlvBuffer,
								sPrimResource,
								sSecResource);
	if( nResult != OK )
	{
		trace_event("End GetTokenRouting(NOK): GetAuthRouting() failure", PROCESSING);
		return NOK;
	}

	PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource, RESOURCE_ID_LEN);

	GetTlvBuffer(AUTO_ROUTING_RES_INFO_1, sTlvBuffer, sRoutingInfo_1, &nLength);
	GetTlvBuffer(AUTO_ROUTING_RES_INFO_2, sTlvBuffer, sRoutingInfo_2, &nLength);


	memcpy(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN);
	if (memcmp(sPrimResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) != 0)
	{

		nResult = CheckResourceLive(sRoutingInfo_1);
		if (nResult == OK)
		{
			memcpy(sExternalResource, sPrimResource, RESOURCE_ID_LEN);
			memcpy(sExternalRoutingInfo,sRoutingInfo_1,RES_INFO_LEN);
		}
	}

	/* if we didn't find an available resource*/
	if (memcmp(sExternalResource, NO_ROUTING_RESOURCE, 6) == 0)
	{
		if (memcmp(sSecResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) != 0)
		{
			nResult = CheckResourceLive(sRoutingInfo_2);
			if (nResult == OK)
			{
				memcpy(sExternalResource, sSecResource, RESOURCE_ID_LEN);
				memcpy(sExternalRoutingInfo,sRoutingInfo_2,RES_INFO_LEN);
			}
		}
	}

	if (memcmp(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) == 0 )
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
		trace_event("End GetTokenRouting(NOK): Issuer not available", PROCESSING);
		return(NOK);
	}


	PutTlvBuffer(AUTO_SEL_ROUTING_CODE, sTlvBuffer, sExternalResource, RESOURCE_ID_LEN);
  
  	if(strlen(sExternalRoutingInfo) != 0)
	{
		PutTlvBuffer(AUTO_SEL_ROUTING_INFO, sTlvBuffer, sExternalRoutingInfo, RES_INFO_LEN);
    }


	trace_event("End GetTokenRouting(OK)", PROCESSING);
	return OK;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlCaptureChannelService(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	int				nResult;
	int				nLength;
	int				nEventCode;
	char			szCaptureCode[RESOURCE_ID_LEN + 1];
	char 			sEventCode[3 + 1];
	char			slProcCode[64 + 1];
	char			slFuncCode[3 + 1];
	char			sLine[MAX_LINE_TRC];
	TSService		rService;

	trace_event("Start CtrlCaptureChannelService()", PROCESSING);

	memset((char *)&rService, '\0', sizeof(rService));
	memset(sEventCode, '\0', sizeof(sEventCode));
	memset(szCaptureCode, '\0', sizeof (szCaptureCode));
	memset(slProcCode, '\0', sizeof (slProcCode));
	memset(slFuncCode, '\0', sizeof (slFuncCode));

	
	
	
	GetTlvBuffer(AUTO_PROCESSING_CODE, sTlvBuffer, slProcCode, &nLength);
	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);
	GetTlvBuffer(AUTO_CAPTURE_CODE, sTlvBuffer, szCaptureCode, &nLength);

	memcpy(rService.szProcCode, slProcCode + 6, 2);
	memcpy(rService.szSrcAcc, slProcCode + 6 + 2 + 6, 2);
	memcpy(rService.szDestAcc, slProcCode + 6 + 2 + 6 + 2 + 6, 2);
	memcpy(rService.szFctCode, slFuncCode, 3);

	nEventCode = CheckResourceServices(nIndexCtx, szCaptureCode, &rService);
	if (nEventCode != OK)
	{
		sprintf(sLine, "Err CheckResourceServices, EventCode= [%d] ", nEventCode); trace_event(sLine, PROCESSING);
		sprintf(sEventCode, "%03d", nEventCode);
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode, 3);
		return(NOK);
	}

	trace_event("End CtrlCaptureChannelService(OK)", PROCESSING);
	return OK;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlRoutingChannelService(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{
	int				nResult;
	int				nLength;
	int				nEventCode;
	char			szRoutingCode[RESOURCE_ID_LEN + 1];
	char 			sEventCode[3 + 1];
	char			slProcCode[64 + 1];
	char			slFuncCode[3 + 1];
	char			sLine[MAX_LINE_TRC];
	TSService		rService;

	trace_event("Start CtrlRoutingChannelService()", PROCESSING);

	memset((char *)&rService, '\0', sizeof(rService));
	memset(sEventCode, '\0', sizeof(sEventCode));
	memset(szRoutingCode, '\0', sizeof (szRoutingCode));
	memset(slProcCode, '\0', sizeof (slProcCode));
	memset(slFuncCode, '\0', sizeof (slFuncCode));
	
	
	
	GetTlvBuffer(AUTO_PROCESSING_CODE, sTlvBuffer, slProcCode, &nLength);
	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);
	GetTlvBuffer(AUTO_SEL_ROUTING_CODE, sTlvBuffer, szRoutingCode, &nLength);

	memcpy(rService.szProcCode, slProcCode + 6, 2);
	memcpy(rService.szSrcAcc, slProcCode + 6 + 2 + 6, 2);
	memcpy(rService.szDestAcc, slProcCode + 6 + 2 + 6 + 2 + 6, 2);
	memcpy(rService.szFctCode, slFuncCode, 3);

	nEventCode = CheckResourceServices(nIndexCtx, szRoutingCode, &rService);
	if (nEventCode != OK)
	{
		sprintf(sLine, "Err CheckResourceServices, EventCode= [%d] ", nEventCode); trace_event(sLine, PROCESSING);
		sprintf(sEventCode, "%03d", nEventCode);
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode, 3);
		return(NOK);
	}

	trace_event("End CtrlRoutingChannelService(OK)", PROCESSING);
	return OK;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlPreAuthRequest(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;

	trace_event("Start CtrlPreAuthRequest()", PROCESSING);

	nResult = ControlPreAuth(nIndexCtx,sTlvBuffer);

	trace_event("End CtrlPreAuthRequest(OK)", PROCESSING);
	return nResult;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int AuthorizationRouting(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;
	char			sExternalRoutingInfo	[RES_INFO_LEN + 1];


	trace_event("Start AuthorizationRouting()", PROCESSING);

	memset(sExternalRoutingInfo, '\0', sizeof (sExternalRoutingInfo));
	
	GetTlvBuffer(AUTO_SEL_ROUTING_INFO, sTlvBuffer, sExternalRoutingInfo, &nLength);


	nResult = AuthRouting(nIndexCtx, sTlvBuffer,sExternalRoutingInfo, 0,0);

	trace_event("End AuthorizationRouting(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlSecurity(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;

	trace_event("Start CtrlSecurity()", PROCESSING);

	

	nResult = CheckAuthSecurity(nIndexCtx, sTlvBuffer);

	trace_event("End CtrlSecurity(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlSpendingLimits(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;
	char			sOnlineOffline[8];
	char			sAlternAmount[256];

	trace_event("Start CtrlSpendingLimits()", PROCESSING);

	memset(sOnlineOffline, '\0', sizeof (sOnlineOffline));
	memset(sAlternAmount, '\0', sizeof (sAlternAmount));
	
	GetTlvBuffer(AUTO_ON_OFF_ROUTING, sTlvBuffer, sOnlineOffline, &nLength);


	nResult = CheckVelocityLimits(nIndexCtx, sOnlineOffline, sTlvBuffer, sAlternAmount);

	trace_event("End CtrlSpendingLimits(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlFallback(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;
	
	trace_event("Start CtrlFallback()", PROCESSING);

	nResult = CheckFallBack(nIndexCtx, "", sTlvBuffer);

	trace_event("End CtrlFallback(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int GenerateAuthorFees(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;
	
	trace_event("Start GenerateAuthorFees()", PROCESSING);

	nResult = GenAuthoFees(nIndexCtx, sTlvBuffer);

	trace_event("End GenerateAuthorFees(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlAccAvailable(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;
	
	trace_event("Start CtrlAccAvailable()", PROCESSING);

	nResult = CheckAccAvailable(nIndexCtx, sTlvBuffer);

	trace_event("End CtrlAccAvailable(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int AuthorAmountsConversion(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;

	trace_event("Start AuthorAmountsConversion()", PROCESSING);


	nResult = AuthAmountMng(nIndexCtx, sTlvBuffer);

	trace_event("End AuthorAmountsConversion(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int GenerateAuthNumber(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;

	trace_event("Start GenerateAuthNumber()", PROCESSING);

	

	nResult = GetAuthNumber(nIndexCtx, sTlvBuffer);
	if( nResult == OK )
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);
	}

	trace_event("End GenerateAuthNumber(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CtrlAuthorEvents(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;

	trace_event("Start CtrlAuthorEvents()", PROCESSING);

	

	nResult = ControlEvents(nIndexCtx, sTlvBuffer);
	

	trace_event("End CtrlAuthorEvents(OK)", PROCESSING);
	return nResult;
}
/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int CheckPanInSaf(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;

	trace_event("Start CheckPanInSaf()", PROCESSING);

	

	nResult = CardInSaf(nIndexCtx, sTlvBuffer);
	

	trace_event("End CheckPanInSaf(OK)", PROCESSING);
	return nResult;
}

/******************************************************************************\
* Return: 
* AddInfo:
*
*
*
\******************************************************************************/
int DumpAuthorDtl(int nIndexCtx, char*	sTlvBuffer, char* sAddInfo)
{

	int				nResult;
	int				nLength;

	trace_event("Start DumpAuthorDtl()", PROCESSING);

	

	nResult = PrintTlvBuffer(sTlvBuffer);
	

	trace_event("End DumpAuthorDtl(OK)", PROCESSING);
	return nResult;
}















