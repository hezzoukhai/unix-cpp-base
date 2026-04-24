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
#define FRAUD_MOD			    1
/******************************************************************************/


int DccInquiryProc (int nIndiceCtx, char*	sTlvBuffer)
{
    TSService		rService;
	char			slCardNumber[24 + 1];
	char			slProcCode[64 + 1];
	char			slFuncCode[3 + 1];
	char 			sPrimResource[6 + 1];
	char 			sSecResource   [ RESOURCE_ID_LEN + 1 ];
	char			sAlternAmount[256];
	char			sLine[MAX_LINE_TRC];
	char			cTrxSign;
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
	TSTlvPrivate    TlvNetPOSData;
	char			cRepeatAction[4];
	char			sAuthFlag[4];
	char			sOrigAuthResult[4];
	char			sSecurityResults[ SEC_FLAG_LEN + 1 ];
	char			sResType[4 + 1];
	char			sResId[RESOURCE_CODE_LEN + 1];/*YDA20170906*/
	char			sFrdCtrlResult[3 + 1];
	int				nPartialAuthInd = NOK;
	int				nWaitFrdResultFlag;
	char			sOriginCode[4];
    TlvInfo			kIsoMoneyTransfTlv;/*ICH20190525 ctrl payment type */
    TSTlvPrivate    TlvPrivateInfo; /*ICH20190525*/

    
	trace_event("Start DccInquiryProc()", PROCESSING);
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
	memset(PosCond, 0, sizeof (PosCond));
	memset(slTransAmount, 0, sizeof (slTransAmount));
	memset(sRoutingInfo_1, 0, sizeof (sRoutingInfo_1));
	memset(sRoutingInfo_2, 0, sizeof (sRoutingInfo_2));
	memset(sExternalRoutingInfo, 0, sizeof (sExternalRoutingInfo));
	memset(sAuthFlag, 0, sizeof (sAuthFlag));
	memset(sOrigAuthResult, 0, sizeof (sOrigAuthResult));
	memset(sSecurityResults, 0, sizeof (sSecurityResults));
	memset(sResType, 0, sizeof (sResType));
	memset(sFrdCtrlResult, 0, sizeof (sFrdCtrlResult));
	memset(sResId, 0, sizeof (sResId));

	InitTlvPrivate(&TlvNetPOSData);

    /******************************************************************************/
	/*  Recherche si Authorisation est Duplicate                                  */
	/******************************************************************************/

	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);

	nRetCode = GetOriginalAuthData	(	nIndiceCtx,
		sTlvBuffer,
		sAuthFlag,
		sOrigAuthResult);
	if (nRetCode == OK)
	{
		trace_event("Found a Duplicate Request. ", PROCESSING);
		if( sOrigAuthResult[0] == AST_DUP_AUTH )
		{
			PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, P7_F039_DUP_REC, P7_F039_LEN);
			memset(sTmpData,' ',P7_F038_LEN);
			PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, sTmpData, P7_F038_LEN);
			trace_event("End DccInquiryProc(OK): Duplicate authorization", PROCESSING);
			return (OK);
		}

		if( sOrigAuthResult[0] == AST_REP_AUTH )
		{
			if( sAuthFlag[0] != 'R' )
			{
				nRetCode = GetAuthRepeatAction(nIndiceCtx,sTlvBuffer,cRepeatAction);
				if( cRepeatAction[0] == RP_ACT_REPLY )
				{
					trace_event("End DccInquiryProc(OK): Reply Response", PROCESSING);
					return (OK);
				}
				if( cRepeatAction[0] == RP_ACT_DECLINE )
				{
					PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, P7_F039_DECLINE, P7_F039_LEN);
					memset(sTmpData,' ',P7_F038_LEN);
					PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, sTmpData, P7_F038_LEN);
					trace_event("End DccInquiryProc(OK): Declined", PROCESSING);
					return (OK);
				}
			}
		}
	}
	else
	{
		if( sOrigAuthResult[0] == AST_INC_AUTH )
		{
			/*GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);*/
			if( memcmp(slFuncCode,P7_F024_PRE_AUTH,P7_F024_LEN) == 0 )
			{
				PutTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, P7_F024_SUPP_AMNT_ESTMD, P7_F024_LEN);
			}
			else
			{
				PutTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, P7_F024_SUPP_AMNT, P7_F024_LEN);
			}
		}
	}

	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if (CheckReqTlvFields(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocalAdm(nIndiceCtx, sTlvBuffer));


	
	#ifdef FRAUD_MOD
		nRetCode = FrdCheckControl(nIndiceCtx,sTlvBuffer);
		
		if( nRetCode != OK )
		{
			return (AuthLocalAdm(nIndiceCtx, sTlvBuffer));
		}
	#endif


	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);
	GetTlvBuffer(AUTO_PAN, sTlvBuffer, slCardNumber, &nLength);
	GetTlvBuffer(AUTO_PROCESSING_CODE, sTlvBuffer, slProcCode, &nLength);
	GetTlvBuffer(AUTO_MSG_TYPE, sTlvBuffer, rService.szMti, &nLength);
	GetTlvBuffer(AUTO_CAPTURE_CODE, sTlvBuffer, szCaptureCode, &nLength);

/********************************************************/
	strcpy(sOnlineOffline, "OFF");

	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);

	nRetCode = GetAuthRouting (	nIndiceCtx,
                                sTlvBuffer,
                                sPrimResource,
                                sSecResource
                              );
	if (nRetCode != OK)
		return (AuthLocalAdm(nIndiceCtx, sTlvBuffer));

    
	if (GetTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sRoutingEventCode, &nLength) != SUCCES)
		return (AuthLocalAdm (nIndiceCtx, sTlvBuffer));

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

/*Start ICH20190525 add payment type to service ctrl (S20_001) */
	if(memcmp(rService.szProcCode, P7_F003_PAYMENT ,P7_F003_P1_LEN) == 0 )
	{
		if ( GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE, sTlvBuffer, sTmpData ,  &nLength) == SUCCESS )
      	{
			InitTlvPrivate (&TlvPrivateInfo); 
			AnalyseTlvPrivate (sTmpData, nLength , &TlvPrivateInfo);

			if(GetTlvPrivate(MONEY_TRANSFER_DATA,&TlvPrivateInfo,sTmpData,&nLength) == SUCCESS )
			{	
				IniIsoMoneyTransDataTlv(&kIsoMoneyTransfTlv);/*ICH20190525*/
  				AnalyseTlvInfo(sTmpData, nLength,&kIsoMoneyTransfTlv);
				if(GetTlvInfoField (P7_F048S20_PAYMENT_TYPE , &kIsoMoneyTransfTlv, sTmpData, &nLength) == SUCCESS )
				{
					memcpy(rService.sZPaymType, sTmpData, nLength);
				}
			}
		}
	}
/*End ICH20190525*/

	nEventCode = CheckResourceServices(nIndiceCtx, szCaptureCode, &rService);
	if (nEventCode != OK)
	{
		sprintf(sLine, "Err CheckResourceServices, EventCode= [%d] ", nEventCode); trace_event(sLine, PROCESSING);
		sprintf(sEventCode, "%03d", nEventCode);
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode, 3);
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



	if (memcmp(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) == 0 )
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
		nRetCode = AuthLocalAdm(nIndiceCtx, sTlvBuffer);
		trace_event("End DccInquiryProc(OK): Issuer not available", PROCESSING);
		return(nRetCode);
	}

	/* a dynamic service which may route needs to fill below fields */
	PutTlvBuffer(AUTO_SERV_STANDIN_ENTRY, sTlvBuffer, SERV_STANDIN_PROC, strlen(SERV_STANDIN_PROC));
	PutTlvBuffer(AUTO_SERV_ROUTING_REPLY_ENTRY, sTlvBuffer, SERV_ON_RESP_PROC, strlen(SERV_ON_RESP_PROC));
	PutTlvBuffer(AUTO_SERV_ROUTING_TIMEOUT_ENTRY, sTlvBuffer, SERV_ON_TIMEOUT_PROC, strlen(SERV_ON_TIMEOUT_PROC));

	if ( memcmp(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) != 0 )
	{
		if (CardInSaf(nIndiceCtx, slCardNumber) == OK)
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
				nRetCode = AuthLocalAdm(nIndiceCtx, sTlvBuffer);
				trace_event("End DccInquiryProc(OK): Card In Saf(Issuer not avail)", PROCESSING);
				return(nRetCode);
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

	GetTlvBuffer(AUTO_ORIGIN_CODE, sTlvBuffer, sOriginCode, &nLength);
	if( sOriginCode[0] != '0' && sOriginCode[0] != '1' && sOriginCode[0] != '2' )
	{
		if (PreControl_ACQ(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthLocalAdm(nIndiceCtx, sTlvBuffer));
	}

	GetResInfoData(RES_INF_RES_TP,sExternalRoutingInfo,sResType,&nLength);

	nRetCode = AuthAmountMng(	nIndiceCtx,sTlvBuffer);
	if (nRetCode != OK)
	{
		return (AuthLocalAdm(nIndiceCtx, sTlvBuffer));
	}
	return (AuthRouting(nIndiceCtx, sTlvBuffer,sExternalRoutingInfo, SERV_ON_RESP_PROC,SERV_ON_TIMEOUT_PROC));

}