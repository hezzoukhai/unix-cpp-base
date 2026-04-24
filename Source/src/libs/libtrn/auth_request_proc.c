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

int AuthRequestStandinProc(int nIndiceCtx, char*	sTlvBuffer)
{

	char			sFunctionCode[3 + 1];
	int				nLength;
	int				nIsFinancial = OK;
	char			sAlternAmount[512];
	char			slProcCode[64 + 1];
	char 			szCaptureCode[RESOURCE_ID_LEN + 1];
	char 			sActionCode[3 + 1];
	int				nRetCode;

	trace_event("Start AuthRequestStandinProc()", PROCESSING);

	memset(szCaptureCode, 0, sizeof (szCaptureCode));
	memset(sAlternAmount, 0, sizeof (sAlternAmount));
	memset(slProcCode, 0, sizeof(slProcCode));
	memset(sActionCode, 0, sizeof(sActionCode));

	/*YDA20171124*/
	/*PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, STAND_IN_RESOURCE, RESOURCE_ID_LEN);*/

	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, sFunctionCode, &nLength);
	GetTlvBuffer(AUTO_PROCESSING_CODE, sTlvBuffer, slProcCode, &nLength);

	if( memcmp(sFunctionCode, P7_F024_ACC_VERIF, P7_F024_LEN) == 0 )
	{
		nIsFinancial = NOK;
	}

	/*YDA20171006 SEE PROD00047328*/
	if (CheckCrossStandInFlags(nIndiceCtx, sTlvBuffer, nIsFinancial,sAlternAmount,szCaptureCode) == OK)
	{
		/******************************************************************************/
		/* 	    Check Account                                                         */
		/******************************************************************************/
		if (nIsFinancial == OK && slProcCode[6] != '2')
		{
			if(CheckAccAvailable(nIndiceCtx, sTlvBuffer) == MALFUNCTION)
			{
				trace_event("End   AuthRequestStandinProc(MALFUNCTION)", PROCESSING);
				return NOK;
			}
		}
		else
		{
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);
			
			GetCurrBalance(nIndiceCtx, sTlvBuffer, "Y");	/*AMER20211015 PLUTONL-3590*/		
		}
		
		
	}	
	
	/*AMER20200419 PLUTONL-1663: F039 not sent in the advice,
	and in case the balance is insufficient, we sent an advice to the host
	even if the auth is declined, because F039 was absent*/
	nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
	if ( nRetCode != OK )                                                                                                         
	{
		trace_event(">>> Error on AuthRequestStandinProc GetActionCode()",ERROR);
	}
	else
	{
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode,3);	
	}
	
	/*
	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);
	nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
	*/

	trace_event("End   AuthRequestStandinProc(OK)", PROCESSING);
	return nRetCode;
}
/******************************************************************************/
/******************************************************************************/
int AuthRequestProc(int nIndiceCtx, char*	sTlvBuffer)
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
	char            sAddAuthInfo[P7_F048_P64_LEN + 1];
	char			sTrnData[33];
    TlvInfo			kIsoMoneyTransfTlv;/*ICH20190525 ctrl payment type */
    TSTlvPrivate    TlvPrivateInfo; /*ICH20190525*/
	int				IsIssAvail = OK;

	trace_event("Start AuthRequestProc()", PROCESSING);
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
	memset(sAddAuthInfo, '0', sizeof(sAddAuthInfo));
	memset(sTrnData, 0, sizeof(sTrnData));

	InitTlvPrivate(&TlvNetPOSData);
	/******************************************************************************/
	/*  Recherche si Authorisation est Duplicate                                  */
	/******************************************************************************/

	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);
	GetTlvBuffer(AUTO_PROCESSING_CODE, sTlvBuffer, slProcCode, &nLength);
	if ( GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE, sTlvBuffer, sTmpData ,  &nLength) == SUCCESS )
    {
		InitTlvPrivate (&TlvPrivateInfo); 
		AnalyseTlvPrivate (sTmpData, nLength , &TlvPrivateInfo);
	}
	GetTlvPrivate( TRANSACTION_IDENTIFIER , &TlvPrivateInfo, sTrnData, &nLength);
	GetTlvPrivate( AUTH_ADD_INFO , &TlvPrivateInfo, sAddAuthInfo, &nLength);
	
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
			/*memset(sTmpData,' ',P7_F038_LEN);
			PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, sTmpData, P7_F038_LEN);*/ /*PLUTONL-5253 AHA20221012*/
			PutTlvBuffer(AUTO_ACTION_FLAG, sTlvBuffer, "D", 1);/*IBO20231115 PLUTONL-7395*/
			trace_event("End AuthRequestProc(OK): Duplicate authorization", PROCESSING);
			return (OK);
		}

		if( sOrigAuthResult[0] == AST_REP_AUTH )
		{
			if( sAuthFlag[0] != 'R' )
			{
				nRetCode = GetAuthRepeatAction(nIndiceCtx,sTlvBuffer,cRepeatAction);
				if( cRepeatAction[0] == RP_ACT_REPLY )
				{
					trace_event("End AuthRequestProc(OK): Reply Response", PROCESSING);
					return (OK);
				}
				if( cRepeatAction[0] == RP_ACT_DECLINE )
				{
					PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, P7_F039_DECLINE, P7_F039_LEN);
					/*memset(sTmpData,' ',P7_F038_LEN);
					PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, sTmpData, P7_F038_LEN);*/
					PutTlvBuffer(AUTO_ACTION_FLAG, sTlvBuffer, "D", 1);/*IBO20231115 PLUTONL-7395*/
					trace_event("End AuthRequestProc(OK): Declined", PROCESSING);
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

	if ( sOrigAuthResult[0] != AST_INC_AUTH
		 && memcmp(slProcCode + 6, P7_F003_PURCHASE, P7_F003_P1_LEN) == 0
		 && strlen(sTrnData) > 0
		 && sAddAuthInfo[P7_F048_P64_REF_ORIGINAL_AUTH_OSFT] == P7_F048_P64_P9_REF_ORIGINAL_AUTH)
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, NO_ORIGINAL_TRX, 3); 
		return (AuthLocal(nIndiceCtx, sTlvBuffer));
	}

	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if (CheckReqTlvFields(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));


	
	#ifdef FRAUD_MOD
		nRetCode = FrdCheckControl(nIndiceCtx,sTlvBuffer);
		
		if( nRetCode != OK )
		{
			return (AuthLocal(nIndiceCtx, sTlvBuffer));
		}
	#endif

	
	if (GetTlvBuffer(AUTO_NETWORK_POS_DATA, sTlvBuffer, sTmpData, &nLength)== SUCCES)
	{
		AnalyseTlvPrivate(sTmpData, nLength, &TlvNetPOSData);
	}

	/* --> Network specific: to be reviewed */
	if ( GetTlvPrivate(POS_CONDITION_CODE_TAG, &TlvNetPOSData, PosCond, &nLength) == SUCCES )
	{
		if( memcmp(PosCond,"51",2) == 0 ) 
		{
			PutTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, P7_F024_ACC_VERIF, P7_F024_LEN);
			nIsFinancial = NOK;
		}
	}
	else if( memcmp(slFuncCode, P7_F024_ACC_VERIF, P7_F024_LEN)==0 || memcmp(slFuncCode, P7_F024_ADDRESS_VERIF, P7_F024_LEN) == 0)  
	{
		nIsFinancial=NOK;
	}

	GetTlvBuffer(AUTO_AMOUNT_TRANSACTION, sTlvBuffer, slTransAmount, &nLength);

	if(memcmp(slFuncCode, P7_F024_ADDRESS_VERIF, P7_F024_LEN) != 0 
		&& memcmp(slFuncCode, P7_F024_RECURRING_MODIF, P7_F024_LEN) != 0  /*ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
		&& memcmp(slFuncCode, P7_F024_RECURRING_CANCEL, P7_F024_LEN) != 0 /*ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
		&& atoi(slTransAmount) == 0) /*AMER20180117 PROD00051816*/
	{
		PutTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, P7_F024_ACC_VERIF, P7_F024_LEN);
		nIsFinancial = NOK;
	}

	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);
	GetTlvBuffer(AUTO_PAN, sTlvBuffer, slCardNumber, &nLength);
	GetTlvBuffer(AUTO_MSG_TYPE, sTlvBuffer, rService.szMti, &nLength);
	GetTlvBuffer(AUTO_CAPTURE_CODE, sTlvBuffer, szCaptureCode, &nLength);

	if( slProcCode[6] == '2' )
	{
		cTrxSign = 'C';
	}
	else
	{
		cTrxSign = 'D';
	}


	if( nIsFinancial == NOK )
	{
		PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"MSC",3);
	}
	else if(	memcmp(slFuncCode,P7_F024_PRE_AUTH, P7_F024_LEN) == 0 ||
		memcmp(slFuncCode,P7_F024_SUPP_AMNT_ESTMD, P7_F024_LEN) == 0)
	{
		PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"PRE",3);
		/*if (ControlPreAuth(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));Commented by CFE PROD00054653*/
	}

	strcpy(sOnlineOffline, "OFF");

	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);

	nRetCode = GetAuthRouting(	nIndiceCtx,
		sTlvBuffer,
		sPrimResource,
		sSecResource);
	if (nRetCode != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));

	/*Start CFE PROD00054653 must be Called after GetAuthRouting Where the Origine_Code Is Calculated In Case Of International Routing*/
	if( memcmp(slFuncCode,P7_F024_PRE_AUTH, P7_F024_LEN) == 0 ||
		memcmp(slFuncCode,P7_F024_SUPP_AMNT_ESTMD, P7_F024_LEN) == 0)
	{
		if (ControlPreAuth(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer));
	}
	/* End CFE PROD00054653*/

	if (GetTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sRoutingEventCode, &nLength) != SUCCES)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));

	GetTlvBuffer(AUTO_ROUTING_RES_INFO_1, sTlvBuffer, sRoutingInfo_1, &nLength);
	GetTlvBuffer(AUTO_ROUTING_RES_INFO_2, sTlvBuffer, sRoutingInfo_2, &nLength);

	sprintf(sLine, "Routing=[%s][%s,%s] ", sRoutingEventCode,sRoutingInfo_1,sRoutingInfo_2 ); 
	trace_event(sLine, TRACE);

	PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource, RESOURCE_ID_LEN);
	/*Need to keep National/International event
	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);

	*/

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
/*End ICH20190525*/

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


	/*AMER20200418: PLUTONL-1663: Moved below
	if (memcmp(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) == 0 )
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
		nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
		trace_event("End AuthRequestProc(OK): Issuer not available", PROCESSING);
		return(nRetCode);
	}
	*/
	
	GetTlvBuffer(AUTO_ORIGIN_CODE, sTlvBuffer, sOriginCode, &nLength);

	/* a dynamic service which may route needs to fill below fields */
	PutTlvBuffer(AUTO_SERV_STANDIN_ENTRY, sTlvBuffer, SERV_STANDIN_PROC, strlen(SERV_STANDIN_PROC));
	PutTlvBuffer(AUTO_SERV_ROUTING_REPLY_ENTRY, sTlvBuffer, SERV_ON_RESP_PROC, strlen(SERV_ON_RESP_PROC));
	PutTlvBuffer(AUTO_SERV_ROUTING_TIMEOUT_ENTRY, sTlvBuffer, SERV_ON_TIMEOUT_PROC, strlen(SERV_ON_TIMEOUT_PROC));

	if (memcmp(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) == 0)	/*AMER20200418: PLUTONL-1663*/
	{
		IsIssAvail = NOK;
	}
	else
	{
		if ( memcmp(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) != 0 )
		{
			if( cTrxSign != 'C' && (sOriginCode[0] == '0' || sOriginCode[0] == '1' || sOriginCode[0] == '2') )
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
						nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
						trace_event("End AuthRequestProc(OK): Card In Saf(Issuer not avail)", PROCESSING);
						return(nRetCode);
					}
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
	}

	if( sOriginCode[0] != '0' && sOriginCode[0] != '1' && sOriginCode[0] != '2' )
	{
		if (PreControl_ACQ(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer));

		/*HAL18082020 PLUTONL-2181*/
		if( IsIssAvail == NOK )		
		{
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
			nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
			trace_event("End AuthRequestProc(OK): Issuer not available", PROCESSING);
			return(nRetCode);
		}
		/*HAL18082020 PLUTONL-2181*/
	}


	GetResInfoData(RES_INF_RES_TP,sExternalRoutingInfo,sResType,&nLength);

	if (memcmp(sRoutingEventCode, INTERNATIONAL_ROUTING_EVENT, 3) == 0 ||
		sResType[0] == RES_TYPE_INT_NET || sResType[0] == RES_TYPE_NAT_SWITCH)
	{
		if( nIsFinancial == OK )
		{
			nRetCode = AuthAmountMng(	nIndiceCtx,sTlvBuffer);
			if (nRetCode != OK)
				return (AuthLocal(nIndiceCtx, sTlvBuffer));
		}
		
		if( IsIssAvail == NOK )	/*AMER20200418: PLUTONL-1663*/		
		{
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
			nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
			trace_event("End AuthRequestProc(OK): Issuer not available", PROCESSING);
			return(nRetCode);
		}

		return (AuthRouting(nIndiceCtx, sTlvBuffer,sExternalRoutingInfo, SERV_ON_RESP_PROC,SERV_ON_TIMEOUT_PROC));
	}



	if (memcmp(sExternalResource, STAND_IN_RESOURCE, 6) == 0  
		|| memcmp(sExternalResource, NO_ROUTING_RESOURCE, 6) == 0)	/*AMER20200418: PLUTONL-1663*/
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
	if( nIsFinancial == OK )
	{
		nRetCode = AuthAmountMng(nIndiceCtx,sTlvBuffer);
		if (nRetCode != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer));
	}



	PrintTlvBuffer(sTlvBuffer);

	/******************************************************************************/
	/* 	    PreControl Procedure                                                  */
	/******************************************************************************/
	if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));


	if( IsIssAvail == NOK )	/*AMER20200418: PLUTONL-1663*/
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
		nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
		trace_event("End AuthRequestProc(OK): Issuer not available", PROCESSING);
		return(nRetCode);
	}
	
	/******************************************************************************/
	/* 	    Security Verfication                                                  */
	/******************************************************************************/
	if (CheckAuthSecurity(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));



	/******************************************************************************/
	/* 	    Check Limits                                                          */
	/******************************************************************************/
	if( nIsFinancial == OK )
	{
		if (CheckLimits(nIndiceCtx, sOnlineOffline, sTlvBuffer, sAlternAmount) != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer));

		/*Start YDA20170906*/
		/*Start NAB16042020 PLUTONL-1441*/
		/*GetTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sResId,&nLength);*/

		GetTlvPrivate(HPS_HOST_SCENARIO_CODE,&TlvPrivateInfo,sTmpData,&nLength);
		GetTlvBuffer (  AUTO_SECURITY_VER_RESULTS , sTlvBuffer, sSecurityResults , &nLength );

		if(	memcmp(sExternalResource, STAND_IN_RESOURCE, RESOURCE_CODE_LEN) != 0 && 
			/*memcmp(sResId, STAND_IN_RESOURCE, RESOURCE_CODE_LEN) == 0 */
			memcmp(sTmpData, "H", 1) == 0 && sSecurityResults[SEC_FLAG_CHK_LIMITS] == '0' &&
			cTrxSign == 'D')
		{
			memcpy(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
			sprintf(sLine, "Auth routed to StandIn, because host scenario processing flag = 'H'");
			trace_event(sLine, PROCESSING);
		}
		/*End NAB16042020 PLUTONL-1441*/
		/*End YDA20170906*/

	}


	
	/******************************************************************************/
	/* 	    Check FallBack														  */
	/******************************************************************************/
	/*No fallback check for voice auths*/
	if( memcmp(szCaptureCode,DSCREEN_RES_CODE,RESOURCE_CODE_LEN) != 0 )
	{
         /*Start HAL PROD00073823 20202201 Enh Q2.2020 Art 2.2*/  	   
        if( memcmp(slProcCode + 6,P7_F003_REFUND, P7_F003_P1_LEN) != 0 )
	    {
		    if (CheckFallBack(nIndiceCtx, sOnlineOffline, sTlvBuffer) != OK)
			    return (AuthLocal(nIndiceCtx, sTlvBuffer));
	    } 
         /*End HAL  PROD00073823 20202201 Enh Q2.2020 Art 2.2*/
	}


	if( nIsFinancial == OK ) 
	{
		if (GenAuthoFees(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthLocal(nIndiceCtx, sTlvBuffer));
	}

	#ifdef FRAUD_MOD
		nRetCode = FrdCheckResponse(nIndiceCtx, sTlvBuffer );
		if( nRetCode != OK )
		{
			return (AuthLocal(nIndiceCtx, sTlvBuffer));
		}
	#endif

	if (memcmp(sExternalResource, STAND_IN_RESOURCE, 6) == 0)
	{
		if (nIsFinancial == OK && cTrxSign=='D')
		{
			nRetCode = CheckAccAvailable(nIndiceCtx, sTlvBuffer);
			/*ZKO16112018: PROD00069031*/
			/* CFE we should not put the event here, because it overwrites the correct value set in PLSQL, in case of cashback authorizations
			if( nRetCode == OK )
			{
				GetTlvBuffer (  AUTO_SECURITY_VER_RESULTS , sTlvBuffer, sSecurityResults , &nLength );
				if( sSecurityResults[SEC_FLAG_CHK_LIMITS] == '1' )
				{
					PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, PARTIAL_AUTHORIZATION, 3);
				}
				else
				{
					PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, "000", 3);
				}
			}
			*/

			if( nRetCode != OK && memcmp(sPrimResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) == 0 )
			{
				nRetCode = CheckPartialAccAvailable(nIndiceCtx, sTlvBuffer);
			}

		}
		else
		{
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);
			nRetCode = GetAuthNumber(nIndiceCtx, sTlvBuffer);
			
			nRetCode = GetCurrBalance(nIndiceCtx, sTlvBuffer, "Y");	/*AMER20211015 PLUTONL-3590*/		


		}

		
		/* START CFE PIN UNBLOCK REQUEST */
		if (memcmp(slProcCode + 6, P7_F003_PIN_UNBLOCK, P7_F003_P1_LEN) == 0)
		{
			if (GenPU_IssScript(nIndiceCtx, sTlvBuffer) != OK)
			{
				/*PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, SYSTEM_MALFUNCTION, 3);NAB19032020 PLUTONL-1413*/ 
				PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, EVENT_SYSTEM_MALFUNCTION, 3);
				return (AuthLocal(nIndiceCtx, sTlvBuffer));
			}
		}

		nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);
	}
	else
	{

		/*PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);*/
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_ROUTING, 3);
		nRetCode = AuthRouting(nIndiceCtx, sTlvBuffer, sExternalRoutingInfo, SERV_ON_RESP_PROC,SERV_ON_TIMEOUT_PROC);
	}

	memset(slCardNumber, 0, sizeof(slCardNumber));


	trace_event("End AuthRequestProc(OK)", PROCESSING);
	return(nRetCode);
}

/****************************************************************************/

