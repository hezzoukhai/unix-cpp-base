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
#include <p7_sid.h>
#include <p7_define.h>

/******************************************************************************/
#define SERV_ON_TIMEOUT_PROC		"OnTimeoutServiceRequest"
#define SERV_ON_RESP_PROC			"AuthReplyProc"
#define SERV_STANDIN_PROC1			"CheckAccountProc"
#define SERV_STANDIN_PROC2			"OnlineCashDepositProc"
#define SERV_STANDIN_PROC3			"OnlineCheckDepositProc"

/******************************************************************************/


int OnlineCheckDepositProc    (    int nIndiceCtx,
	char *sTlvBuffer  )
{

	TSService       rService;
	char            slProcCode      [      32 + 1 ];
	char            slFuncCode      [      3 + 1 ];
	char            sRoutingEventCode      [      3 + 1 ];
	char            sLine           [MAX_LINE_TRC];
	int             nLength;
	int             nEventCode =  OK;
	int             nRetCode;
	char            sEventCode     [ 3 + 1 ];
	char            sPrimResource  [ 6 + 1 ];
	char            sSecResource   [ 6 + 1 ];
	char            sInternalStan  [ 6 + 1 ];
	char            szCaptureCode  [ 6 + 1 ];
	char            sExternalResource  [ 6 + 1 ];
	char			sRoutingInfo_1			[RES_INFO_LEN + 1];
	char			sRoutingInfo_2			[RES_INFO_LEN + 1];
	char			sExternalRoutingInfo	[RES_INFO_LEN + 1];
	char			sAlternAmount	        [100 + 1];
	char			sOnlineOffline	        [100 + 1];
	char            cOnOffMode;
	char			sResType[4 + 1];
	char			sResId[RESOURCE_CODE_LEN + 1];/*YDA20170906*/

	trace_event("Start OnlineCheckDepositProc()",PROCESSING);
	/******************************************************************************/
	/*                                      Initialisation                  */
	/******************************************************************************/

	memset(slProcCode           , 0, sizeof(slProcCode));
	memset(slFuncCode           , 0, sizeof(slFuncCode));
	memset((char *)&rService    , 0, sizeof(rService));
	memset(sEventCode           , 0, sizeof(sEventCode));
	memset(sPrimResource        , 0, sizeof(sPrimResource));
	memset(sSecResource         , 0, sizeof(sSecResource));
	memset(sInternalStan        , 0, sizeof(sInternalStan));
	memset(sRoutingInfo_1       , 0, sizeof (sRoutingInfo_1));
	memset(sRoutingInfo_2       , 0, sizeof (sRoutingInfo_2));
	memset(sExternalRoutingInfo , 0, sizeof (sExternalRoutingInfo));
	memset(sLine                , 0, sizeof (sLine));
	memset(sRoutingEventCode    , 0, sizeof (sRoutingEventCode));
	memset(szCaptureCode        , 0, sizeof (szCaptureCode));
	memset(sOnlineOffline       , 0, sizeof (sOnlineOffline));
	memset(sExternalResource    , 0, sizeof (sExternalResource));
	memset(sResType, 0, sizeof (sResType));
	memset(sResId, 0, sizeof (sResId));/*YDA20170906*/

	PrintTlvBuffer(sTlvBuffer);

	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if (CheckReqTlvFields(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));

	strcpy(sOnlineOffline, "OFF");
	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);

	nRetCode = GetAuthRouting(	nIndiceCtx,
		sTlvBuffer,
		sPrimResource,
		sSecResource);

	if (nRetCode != OK)
		return (AuthLocalAdm(nIndiceCtx, sTlvBuffer));

	if (GetTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sRoutingEventCode, &nLength) != SUCCES)
		return (AuthLocalAdm(nIndiceCtx, sTlvBuffer));

	GetTlvBuffer(AUTO_ROUTING_RES_INFO_1, sTlvBuffer, sRoutingInfo_1, &nLength);
	GetTlvBuffer(AUTO_ROUTING_RES_INFO_2, sTlvBuffer, sRoutingInfo_2, &nLength);

	PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource, 6);
	/*Need to keep National/International event
	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);
	*/

	/******************************************************************************/
	/*                        Controle du service /r ressource   */
	/******************************************************************************/

	GetTlvBuffer(AUTO_PROCESSING_CODE, sTlvBuffer, slProcCode, &nLength);
	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);
	GetTlvBuffer(AUTO_MSG_TYPE, sTlvBuffer, rService.szMti, &nLength);
	GetTlvBuffer(AUTO_CAPTURE_CODE, sTlvBuffer, szCaptureCode, &nLength);




	nLength = 0;

	memcpy(rService.szProcCode, slProcCode + 6, 2);
	memcpy(rService.szSrcAcc, slProcCode + 6 + 2 + 6, 2);
	memcpy(rService.szDestAcc, slProcCode + 6 + 2 + 6 + 2 + 6, 2);
	memcpy(rService.szFctCode , slFuncCode , 3);


	nEventCode = CheckResourceServices ( nIndiceCtx, szCaptureCode, &rService );
	if (nEventCode != OK)
	{
		sprintf ( sEventCode , "%03d", nEventCode );
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode ,     3);
		return (AuthLocalAdm(nIndiceCtx, sTlvBuffer));
	}

	/******************************************************************************/
	/*  Amount Bill/Settlement Management                                         */
	/******************************************************************************/
	nEventCode = AuthAmountMng (    nIndiceCtx          ,
		sTlvBuffer
		);
	if ( nEventCode != OK )
	{
		trace_event ( "Retour AmountMng NOK ", PROCESSING );
		return (AuthLocalAdm(nIndiceCtx, sTlvBuffer));
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
		nRetCode = AuthLocalAdm(nIndiceCtx, sTlvBuffer);

		trace_event("End OnlineCheckDepositProc(): Issuer not available", PROCESSING);
		return(nRetCode);
	}

	/* a dynamic service which may route needs to fill below fields */
	PutTlvBuffer(AUTO_SERV_STANDIN_ENTRY, sTlvBuffer, SERV_STANDIN_PROC3, strlen(SERV_STANDIN_PROC3));
	PutTlvBuffer(AUTO_SERV_ROUTING_REPLY_ENTRY, sTlvBuffer, SERV_ON_RESP_PROC, strlen(SERV_ON_RESP_PROC));
	PutTlvBuffer(AUTO_SERV_ROUTING_TIMEOUT_ENTRY, sTlvBuffer, SERV_ON_TIMEOUT_PROC, strlen(SERV_ON_TIMEOUT_PROC));

	/*PROD00025759*/
	GetResInfoData(RES_INF_RES_TP,sExternalRoutingInfo,sResType,&nLength);

	if (memcmp(sRoutingEventCode, INTERNATIONAL_ROUTING_EVENT, 3) == 0 ||
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
	/* 	    PreControl Procedure                                                  */
	/******************************************************************************/
	if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));

	/******************************************************************************/
	/* 	    Security Verfication                                                  */
	/******************************************************************************/
	if (memcmp(sRoutingEventCode, INTERNATIONAL_ROUTING_EVENT, 3) != 0)
		if (CheckAuthSecurity(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer));


	PrintTlvBuffer(sTlvBuffer);

	/******************************************************************************/
	/* 	    Check Limits                                                          */
	/******************************************************************************/
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

	if (memcmp(sExternalResource, STAND_IN_RESOURCE, 6) == 0)
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);
		nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
	}
	else
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);
		nRetCode = AuthRouting(nIndiceCtx, sTlvBuffer, sExternalRoutingInfo, SERV_ON_RESP_PROC,SERV_ON_TIMEOUT_PROC);
	}


	sprintf(sLine, "End OnlineCheckDepositProc(%d)", nRetCode); 
	trace_event(sLine, PROCESSING);

	return ( nRetCode );
}
