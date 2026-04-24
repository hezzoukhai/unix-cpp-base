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
#define SERV_STANDIN_PROC			""
/******************************************************************************/


/******************************************************************************/
int DepositRequestProc(int nIndiceCtx, char*	sTlvBuffer)
{

	TSService		rService;
	char			slCardNumber[24 + 1];
	char			slProcCode[64 + 1];
	char			slFuncCode[3 + 1];
	char 			sPrimResource[6 + 1];
	char 			sSecResource   [ 6 + 1 ];
	char			sAlternAmount[512];
	char			sLine[MAX_LINE_TRC];
	int     		nLength;
	int 			nEventCode = OK;
	int 			nRetCode;
	char 			sEventCode[3 + 1];
	char 			sRoutingEventCode[3 + 1];
	char 			szCaptureCode[6 + 1];
	char			sOnlineOffline[8];
	char			sExternalResource[6 + 1];
	char			sTmpData[LG_MAX];
	char			sRoutingInfo_1			[RES_INFO_LEN + 1];
	char			sRoutingInfo_2			[RES_INFO_LEN + 1];
	char			sExternalRoutingInfo	[RES_INFO_LEN + 1];
	pid_t			nPid;
	char			sResType[4 + 1];
	char			sResId[RESOURCE_CODE_LEN + 1];/*YDA20170906*/
	char			sAuthFlag[4];/*MHA20191226*/
	char			sOrigAuthResult[4];/*MHA20191226*/

	trace_event("Start DepositRequestProc()", PROCESSING);
	/******************************************************************************/
	/* 					Initialisation			*/
	/******************************************************************************/

	memset(slCardNumber, 0, sizeof(slCardNumber));
	memset(slProcCode, 0, sizeof(slProcCode));
	memset(slFuncCode, 0, sizeof(slFuncCode));
	memset((char *)&rService, 0, sizeof(rService));
	memset(sEventCode, 0, sizeof(sEventCode));
	memset(szCaptureCode, 0, sizeof (szCaptureCode));
	memset(sAlternAmount, 0, sizeof (sAlternAmount));
	memset(sOnlineOffline, 0, sizeof (sOnlineOffline));
	memset(sPrimResource, 0, sizeof (sPrimResource));
	memset(sSecResource, 0, sizeof (sSecResource));
	memset(sExternalResource, 0, sizeof (sExternalResource));
	memset(sRoutingEventCode, 0, sizeof (sRoutingEventCode));
	memset(sTmpData, 0, sizeof (sTmpData));
	memset(sRoutingInfo_1, 0, sizeof (sRoutingInfo_1));
	memset(sRoutingInfo_2, 0, sizeof (sRoutingInfo_2));
	memset(sExternalRoutingInfo, 0, sizeof (sExternalRoutingInfo));
	memset(sResType, 0, sizeof (sResType));
	memset(sResId, 0, sizeof (sResId));/*YDA20170906*/
	memset(sAuthFlag, 0, sizeof (sAuthFlag)); /*MHA20191226*/
	memset(sOrigAuthResult, 0, sizeof (sOrigAuthResult)); /*MHA20191226*/

	/******************************************************************************/
	/*  Recherche si Authorisation est Duplicate                                  */
	/******************************************************************************/
	/*MHA20191226*/
/* 	nRetCode = GetOriginalDataElements	(nIndiceCtx,
										CAPTURE_MODE,
										sTlvBuffer,
	 									sPrimResource);
*/
	
		nRetCode = GetOriginalAuthData	(	nIndiceCtx,
											sTlvBuffer,
											sAuthFlag,
											sOrigAuthResult);
	/*MHA20191226*/
	if (nRetCode == OK)
	{
		trace_event("Found a Duplicate Request. ", PROCESSING);
		PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, P7_F039_DUP_REC, P7_F039_LEN);
		memset(sTmpData,' ',P7_F038_LEN);
		PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, sTmpData, P7_F038_LEN);
		return (OK);
	}

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

	PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource, 6);
 /*Need to keep National/International event	
	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);
*/
	/* Note: should confirm that it's an ICC card instead of presence of ICC data*/
#ifdef IS_GEN_SCRIPT
	if (GetTlvBuffer(AUTO_ICC_DATA, sTlvBuffer, sTmpData, &nLength) == SUCCESS &&
		memcmp(sRoutingEventCode, INTERNATIONAL_ROUTING_EVENT,3) != 0)
	{
		nRetCode = gen_iss_script_data(nIndiceCtx, sTlvBuffer, sTmpData);
	}
#endif

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

	if (memcmp(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) == 0)
	{

		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
		nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
		trace_event("End DepositRequestProc(OK): Issuer not available", PROCESSING);
		return(nRetCode);
	}
	
	if ( memcmp(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) != 0 )
	{

		if (CardInSaf(nIndiceCtx, slCardNumber) == OK)
		{
			/*PutTlvBuffer(AUTO_MESSAGE_REASON_CODE, sTlvBuffer, "1005", 4);*/
			if (debug_option == 1)
			{
				sprintf(sLine, "Card [%s] Found In Saf File ", slCardNumber); trace_event(sLine, PROCESSING);
			}
			/* we can't forward this request, check if we can process it in standin mode*/			
			if (memcmp(sSecResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) == 0)
			{
				/* YDA20180103 : Set reason code to P7_F025_CIS_STANDIN instead of 1005 */
				PutTlvBuffer(AUTO_MESSAGE_REASON_CODE, sTlvBuffer, P7_F025_CIS_STANDIN, 4);
				memcpy(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
			}
			else
			{
				PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
				nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
				trace_event("End DepositRequestProc(OK)", PROCESSING);
				return(nRetCode);
			}
		}

	}


	/*PROD00025759*/
	GetResInfoData(RES_INF_RES_TP,sExternalRoutingInfo,sResType,&nLength);

	if (memcmp(sRoutingEventCode, INTERNATIONAL_ROUTING_EVENT, 3) == 0	||
		sResType[0] == RES_TYPE_INT_NET || sResType[0] == RES_TYPE_NAT_SWITCH)
	{
		return (AuthRouting(nIndiceCtx, sTlvBuffer,sExternalRoutingInfo, SERV_ON_RESP_PROC,SERV_ON_TIMEOUT_PROC));

	}


	if (memcmp(sExternalResource, STAND_IN_RESOURCE, 6) == 0)
		strcpy(sOnlineOffline, "OFF");
	else
		strcpy(sOnlineOffline, "ON");

	/******************************************************************************/
	/* 	    Lecture des flags de securite                                         */
	/******************************************************************************/
	if (GetSecurityFlags(nIndiceCtx, sTlvBuffer, sOnlineOffline) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));

	/******************************************************************************/
	/*  Amount Bill/Settlement Management                                         */
	/******************************************************************************/
	/*Start IZL 20042022: PLUTONL-4225*/
	if (AuthAmountMng(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));
	/*End IZL 20042022: PLUTONL-4225*/
	
	/******************************************************************************/
	/* 	    PreControl Procedure                                                  */
	/******************************************************************************/
	if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));

	/******************************************************************************/
	/* 	    Security Verfication                                                  */
	/******************************************************************************/
	if (CheckAuthSecurity(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));


	if (CheckLimits(nIndiceCtx, sOnlineOffline, sTlvBuffer, sAlternAmount) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));

	/*Start YDA20170906*/
	GetTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sResId,&nLength);
	if(memcmp(sExternalResource, STAND_IN_RESOURCE, RESOURCE_CODE_LEN) != 0 && memcmp(sResId, STAND_IN_RESOURCE, RESOURCE_CODE_LEN) == 0)
	{
		memcpy(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
		sprintf(sLine, "Auth routed to StandIn, because host scenario processing flag = 'H'");
		trace_event(sLine, PROCESSING);
	}
	/*End YDA20170906*/

	PrintTlvBuffer(sTlvBuffer);


	if (memcmp(sExternalResource, STAND_IN_RESOURCE, 6) == 0)
	{

		if (Deposit_Inquiry(nIndiceCtx, sTlvBuffer) != OK)
		{
			return (AuthLocal(nIndiceCtx, sTlvBuffer));
		}
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);
		nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
	}
	else
	{

		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);
		nRetCode = AuthRouting(nIndiceCtx, sTlvBuffer,sExternalRoutingInfo, SERV_ON_RESP_PROC,SERV_ON_TIMEOUT_PROC);

	}




	memset(slCardNumber, 0, sizeof(slCardNumber));
	memset(slCardNumber, 0, sizeof(slCardNumber));
	memset(slProcCode, 0, sizeof(slProcCode));
	memset(slFuncCode, 0, sizeof(slFuncCode));
	memset((char *)&rService, 0, sizeof(rService));
	memset(sEventCode, 0, sizeof(sEventCode));
	memset(sPrimResource, 0, sizeof(sPrimResource));
	memset(sAlternAmount, 0, sizeof (sAlternAmount));


	trace_event("End DepositRequestProc(OK)", PROCESSING);
	return(nRetCode);
}
