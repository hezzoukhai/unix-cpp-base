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
#include <iso8583.h>

/******************************************************************************/
#define SERV_ON_TIMEOUT_PROC		"OnTimeoutServiceRequest"
#define SERV_ON_RESP_PROC			"AuthReplyProc"
#define SERV_STANDIN_PROC			"AuthRequestStandinProc"
/******************************************************************************/


/******************************************************************************/
int DigitizationRequest(int nIndiceCtx, char*	sTlvBuffer)
{

	TSService		rService;
	char			slCardNumber[24 + 1];
	char			slProcCode[64 + 1];
	char			slFuncCode[3 + 1];
	char 			sPrimResource[6 + 1];
	char 			sSecResource   [ RESOURCE_ID_LEN + 1 ];
	char			sLine[MAX_LINE_TRC];
	
	int     		nLength;
	int 			nEventCode = OK;
	int 			nRetCode;
	int				nIsFinancial = OK;
	char 			sEventCode[3 + 1];
	char 			sRoutingEventCode[3 + 1];
	char 			szCaptureCode[6 + 1];
	char			sOnlineOffline[8];
	char			sExternalResource[RESOURCE_ID_LEN + 1];
	char			sTmpData[LG_MAX];
	char			PosCond[3];
	char			slTransAmount[24];
	char			sRoutingInfo_1			[RES_INFO_LEN + 1];
	char			sRoutingInfo_2			[RES_INFO_LEN + 1];
	char			sExternalRoutingInfo	[RES_INFO_LEN + 1];
	char			cRepeatAction[4];
	char			sAuthFlag[4];
	char			sOrigAuthResult[4];
	char			sSecurityResults[ SEC_FLAG_LEN + 1 ];
	char			sResId[RESOURCE_CODE_LEN + 1];/*YDA20170906*/
	int				nPartialAuthInd = NOK;
	char			sOriginCode[4];
    

	trace_event("Start DigitizationRequest()", PROCESSING);
	/******************************************************************************/
	/* 					Initialisation			*/
	/******************************************************************************/

	memset(slCardNumber, 0, sizeof(slCardNumber));
	memset(slProcCode, 0, sizeof(slProcCode));
	memset(slFuncCode, 0, sizeof(slFuncCode));
	memset((char *)&rService, 0, sizeof(rService));
	memset(sEventCode, 0, sizeof(sEventCode));
	memset(szCaptureCode, 0, sizeof (szCaptureCode));
	memset(sOnlineOffline, 0, sizeof (sOnlineOffline));
	memset(sPrimResource, 0, sizeof (sPrimResource));
	memset(sSecResource, 0, sizeof (sSecResource));
	memset(sExternalResource, 0, sizeof (sExternalResource));
	memset(sRoutingEventCode, 0, sizeof (sRoutingEventCode));
	memset(sTmpData, 0, sizeof (sTmpData));
	memset(PosCond, 0, sizeof (PosCond));
	memset(slTransAmount, 0, sizeof (slTransAmount));
	memset(sRoutingInfo_1, 0, sizeof (sRoutingInfo_1));
	memset(sRoutingInfo_2, 0, sizeof (sRoutingInfo_2));
	memset(sExternalRoutingInfo, 0, sizeof (sExternalRoutingInfo));
	memset(sAuthFlag, 0, sizeof (sAuthFlag));
	memset(sOrigAuthResult, 0, sizeof (sOrigAuthResult));
	memset(sSecurityResults, 0, sizeof (sSecurityResults));
	memset(sResId, 0, sizeof (sResId));

	/******************************************************************************/
	/*  Recherche si Authorisation est Duplicate                                  */
	/******************************************************************************/

	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);

	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if (CheckReqTlvFields(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));



	
	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);
	GetTlvBuffer(AUTO_PAN, sTlvBuffer, slCardNumber, &nLength);
	GetTlvBuffer(AUTO_PROCESSING_CODE, sTlvBuffer, slProcCode, &nLength);
	GetTlvBuffer(AUTO_MSG_TYPE, sTlvBuffer, rService.szMti, &nLength);
	GetTlvBuffer(AUTO_CAPTURE_CODE, sTlvBuffer, szCaptureCode, &nLength);
	PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR, sTlvBuffer, "MSC", 3);

	strcpy(sOnlineOffline, "OFF");

	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);

	nRetCode = GetAuthRouting(	nIndiceCtx,
		sTlvBuffer,
		sPrimResource,
		sSecResource);
	if (nRetCode != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));


	if (GetTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sRoutingEventCode, &nLength) != SUCCES)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));

	GetTlvBuffer(AUTO_ROUTING_RES_INFO_1, sTlvBuffer, sRoutingInfo_1, &nLength);
	GetTlvBuffer(AUTO_ROUTING_RES_INFO_2, sTlvBuffer, sRoutingInfo_2, &nLength);

	sprintf(sLine, "Routing=[%s][%s,%s] ", sRoutingEventCode,sRoutingInfo_1,sRoutingInfo_2 ); 
	trace_event(sLine, TRACE);

	PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource, RESOURCE_ID_LEN);

	/******************************************************************************/
	/* 				          Controle du service /r ressource   */
	/******************************************************************************/

	memcpy(rService.szProcCode, slProcCode + 6, 2);
	memcpy(rService.szSrcAcc, slProcCode + 6 + 2 + 6, 2);
	memcpy(rService.szDestAcc, slProcCode + 6 + 2 + 6 + 2 + 6, 2);
	memcpy(rService.szFctCode, slFuncCode, 3);


	nEventCode = CheckResourceServices(nIndiceCtx, szCaptureCode, &rService);
	if (nEventCode != OK)
	{
		sprintf(sLine, "Err CheckResourceServices, EventCode= [%d] ", nEventCode); trace_event(sLine, PROCESSING);
		sprintf(sEventCode, "%03d", nEventCode);
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode, 3);
		return (AuthLocal(nIndiceCtx, sTlvBuffer));
	}

	/* Find out which resource will be used to authorize */
	memcpy(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN);
	if (memcmp(sPrimResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) == 0)
	{
		memcpy(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
	}
	else if (memcmp(sPrimResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) != 0)
	{

		nRetCode = CheckResourceLive(sRoutingInfo_1);
		if (nRetCode == OK)
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
			nRetCode = CheckResourceLive(sRoutingInfo_2);
			if (nRetCode == OK)
			{
				memcpy(sExternalResource, sSecResource, RESOURCE_ID_LEN);
				memcpy(sExternalRoutingInfo,sRoutingInfo_2,RES_INFO_LEN);
			}
		}
	}



	if (memcmp(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) == 0 )
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
		nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
		trace_event("End DigitizationRequest(OK): Issuer not available", PROCESSING);
		return(nRetCode);
	}

	/* a dynamic service which may route needs to fill below fields */
	PutTlvBuffer(AUTO_SERV_STANDIN_ENTRY, sTlvBuffer, SERV_STANDIN_PROC, strlen(SERV_STANDIN_PROC));
	PutTlvBuffer(AUTO_SERV_ROUTING_REPLY_ENTRY, sTlvBuffer, SERV_ON_RESP_PROC, strlen(SERV_ON_RESP_PROC));
	PutTlvBuffer(AUTO_SERV_ROUTING_TIMEOUT_ENTRY, sTlvBuffer, SERV_ON_TIMEOUT_PROC, strlen(SERV_ON_TIMEOUT_PROC));



	if (memcmp(sExternalResource, STAND_IN_RESOURCE, 6) == 0)
		strcpy(sOnlineOffline, "OFF");
	else
		strcpy(sOnlineOffline, "ON");


	/******************************************************************************/
	/* 	    Lecture des flags de securite                                         */
	/******************************************************************************/
	if (memcmp(slFuncCode, P7_F024_TER, P7_F024_LEN) == 0 ||
		memcmp(slFuncCode, P7_F024_TAR, P7_F024_LEN) == 0)
	{
		if (GetSecurityFlags(nIndiceCtx, sTlvBuffer, sOnlineOffline) != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer));
		
		if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer));

		if (CheckAuthSecurity(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer));
		
	}



	/******************************************************************************/
	/* 	    Product level digitization eligibility                                */
	/******************************************************************************/
	if (memcmp(slFuncCode, P7_F024_TER, P7_F024_LEN) == 0)
	{
		if (CheckProductDigitizationEligibility(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer)); 
	}

	/******************************************************************************/
	/* 	    Evaluate digitization matrix                                          */
	/******************************************************************************/
	if (memcmp(slFuncCode, P7_F024_TAR, P7_F024_LEN) == 0)
	{
		if (EvaluateDigitizationMatrix(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer)); 
	}

	/******************************************************************************/
	/* 	   Send activation code                                           */
	/******************************************************************************/
	if (memcmp(slFuncCode, P7_F024_ACN, P7_F024_LEN) == 0)
	{
		 SendActivationCode(nIndiceCtx, sTlvBuffer);
	}


	/******************************************************************************/
	/* 	   Complete tokenization                                           */
	/******************************************************************************/
	if (memcmp(slFuncCode, P7_F024_TCN, P7_F024_LEN) == 0)
	{
		 CompleteTokenization(nIndiceCtx, sTlvBuffer);
	}

	/******************************************************************************/
	/* 	   Process tokenization event notification                                          */
	/******************************************************************************/
	if (memcmp(slFuncCode, P7_F024_TVN, P7_F024_LEN) == 0)
	{
		 ProcessTokenizationEventNotif(nIndiceCtx, sTlvBuffer);
	}

	PrintTlvBuffer(sTlvBuffer);


	if (memcmp(sExternalResource, STAND_IN_RESOURCE, 6) == 0)
	{
		if (GetTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sRoutingEventCode, &nLength) != SUCCES
			|| memcmp(sRoutingEventCode, NO_REASON_TO_DECLINE, 3) != 0 )
		{
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);
		}
		nRetCode = GetAuthNumber(nIndiceCtx, sTlvBuffer);
		nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
	}
	else
	{

		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);
		nRetCode = AuthRouting(nIndiceCtx, sTlvBuffer, sExternalRoutingInfo, SERV_ON_RESP_PROC,SERV_ON_TIMEOUT_PROC);
	}

	memset(slCardNumber, 0, sizeof(slCardNumber));


	trace_event("End DigitizationRequest(OK)", PROCESSING);
	return(nRetCode);
}

/****************************************************************************/
