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
#include <dump.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <string.h>


/******************************************************************************/
int AuthAdviceProc (	int nIndiceCtx, char *sTlvBuffer)
{


	int     		nLength;
	int 			nRetCode;
	char 			sActionCode       [ 3 + 1 ];
	char 			sAuthNum	      [ 7 + 1 ];
	char			sFunctionCode	[3 + 1];
	char			sCaptureCode	[ 6 + 1];
	char 			sPrimResource[6 + 1];
	char 			sSecResource   [ 6 + 1 ];
	char			sActionFlag[4];
	char			sLine[MAX_LINE_TRC];
	char			sRoutingCode[RESOURCE_ID_LEN + 1];
	char			sCaptureResInfo[128];
	char			sCaptureNodeId[4 + 1];
	char			cRepeatAction[4];
	char			sAuthFlag[4];
	char			sOrigAuthResult[4];
	char			sShadowAccLevel[1 + 1];
	char			sTlvPrivateBuffer[1024 + 1];
	TSTlvPrivate    sTlvPrivateInfo;

	char			sSecurityData			[P7_F053_MAX_LEN + 1];
	char			sRoutingInfo_1			[RES_INFO_LEN + 1];
	char			sPid					[PROC_PID_LEN + 1];
	char			sResNode				[10];
	char			sResID					[10];

	trace_event("Start AuthAdviceProc ()",PROCESSING);
	/******************************************************************************/
	/*               Initialisation				                                  */
	/******************************************************************************/
	memset(sActionCode     		, 0, sizeof(sActionCode));
	memset(sCaptureCode     	, 0, sizeof(sCaptureCode));
	memset(sAuthNum     		, 0, sizeof(sAuthNum));
	memset(sFunctionCode     	, 0, sizeof(sFunctionCode));
	memset(sActionFlag     		, 0, sizeof(sActionFlag));
	memset(sRoutingCode		, 0, sizeof(sRoutingCode ));
	memset(sPrimResource		, 0, sizeof(sPrimResource));
	memset(sSecResource		, 0, sizeof(sSecResource));
	memset(sCaptureResInfo		, 0, sizeof(sCaptureResInfo));
	memset(sCaptureNodeId		, 0, sizeof(sCaptureNodeId));
	memset(sAuthFlag, 0, sizeof (sAuthFlag));
	memset(sOrigAuthResult, 0, sizeof (sOrigAuthResult));
	memset(sShadowAccLevel, 0, sizeof (sShadowAccLevel));
	memset(sTlvPrivateBuffer, 0, sizeof (sTlvPrivateBuffer));
	memset(sSecurityData , 0, sizeof (sSecurityData ));
	memset(sRoutingInfo_1, 0, sizeof (sRoutingInfo_1));
	memset(sPid,'\0',sizeof(sPid));
	memset(sResNode,'\0',sizeof(sResNode));
	memset(sResID,'\0',sizeof(sResID));
	
	InitTlvPrivate (&sTlvPrivateInfo);

	GetTlvBuffer ( AUTO_FUNCTION_CODE        , sTlvBuffer, sFunctionCode   , &nLength );
	GetTlvBuffer ( AUTO_RESPONSE_CODE        , sTlvBuffer, sActionCode   , &nLength );
	GetTlvBuffer ( AUTO_CAPTURE_CODE        , sTlvBuffer, sCaptureCode   , &nLength );
	GetTlvBuffer ( AUTO_CAPTURE_RES_INFO    , sTlvBuffer, sCaptureResInfo   , &nLength );

	GetResInfoData(RES_INF_RES_NODE,sCaptureResInfo,sCaptureNodeId,&nLength);

	PutTlvBuffer( AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT ,     3);
	PutTlvBuffer( AUTO_AUTHO_FLAG, sTlvBuffer, "P" ,  1);
	PutTlvBuffer( AUTO_AUTHOR_SRC, sTlvBuffer, "E1",  2);

	GetTlvBuffer ( AUTO_SHADOW_ACT_LEVEL    , sTlvBuffer, sShadowAccLevel   , &nLength );

	PrintTlvBuffer(sTlvBuffer);

	/*Added in V3.3. should not processes advices more than once. Should reply with same response */
	/*EBE150912: Incremental auth processing
	nRetCode = GetOriginalDataElements	(nIndiceCtx,
	CAPTURE_MODE,
	sTlvBuffer,
	sPrimResource);
	if (nRetCode == OK)
	{
	PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
	nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
	trace_event("Found a Duplicate Request. ", PROCESSING);
	return (OK);
	}*/

	nRetCode = GetOriginalAuthData	(	nIndiceCtx,
		sTlvBuffer,
		sAuthFlag,
		sOrigAuthResult);
	if (nRetCode == OK)
	{
		trace_event("Found a Duplicate Request. ", PROCESSING);
		if( sOrigAuthResult[0] == AST_DUP_AUTH || sOrigAuthResult[0] == AST_REP_AUTH)
		{
			PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
			nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
			trace_event("End AuthRequestProc(OK): Duplicate authorization(log SAF)", PROCESSING);
			return (OK);
		}
	}
	else
	{
		if( sOrigAuthResult[0] == AST_INC_AUTH )
		{
			GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, sFunctionCode, &nLength);
			if( memcmp(sFunctionCode,P7_F024_PRE_AUTH,P7_F024_LEN) == 0 )
			{
				PutTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, P7_F024_SUPP_AMNT_ESTMD, P7_F024_LEN);
			}
			else
			{
				PutTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, P7_F024_SUPP_AMNT, P7_F024_LEN);
			}
		}
	}

	GetTlvBuffer ( AUTO_FUNCTION_CODE        , sTlvBuffer, sFunctionCode   , &nLength );

	/*EBE: for now we do call preauth completion from advice processing*/
	if( memcmp(sFunctionCode,P7_F024_AUTH_PREV_AMNT_DIFF,P7_F024_LEN) == 0 )
	{
		nRetCode = AuthPreauthComplProc(nIndiceCtx,sTlvBuffer);
		return nRetCode;
	}

	if(	memcmp(sFunctionCode,P7_F024_PRE_AUTH, P7_F024_LEN) == 0 ||
		memcmp(sFunctionCode,P7_F024_SUPP_AMNT_ESTMD, P7_F024_LEN) == 0)
	{
		PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"PRE",3);
		if (ControlPreAuth(nIndiceCtx, sTlvBuffer) != OK)
		{
			nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
			trace_event("End   AuthAdviceProc (NOK)",PROCESSING);
			return(NOK);
		}
	}

	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if ( CheckAdvTlvFields ( nIndiceCtx, sTlvBuffer ) != OK ||
		strlen(sActionCode) == 0 )		/* Action code should be mandatory in an advice */
	{
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   AuthAdviceProc (NOK)",PROCESSING);
		return(NOK);
	}


#if IS_P7_SWI
	nRetCode = GetAuthRouting(	nIndiceCtx,
		sTlvBuffer,
		sPrimResource,
		sSecResource);
	if (nRetCode != OK)
	{
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   AuthAdviceProc (NOK)",PROCESSING);
		return (  NOK );
	}
#endif

	/*EBE: not managed for now
	if( memcmp(sFunctionCode,P7_F024_PRE_AUTH,P7_F024_LEN) == 0 )
	{
	nRetCode = GetOriginalPreAuth(nIndiceCtx,sTlvBuffer);
	}
	*/

	nRetCode = AuthAmountMng ( nIndiceCtx , sTlvBuffer);
	if ( nRetCode != OK )
	{
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   AuthAdviceProc (NOK)",PROCESSING);
		return (  NOK );
	}

	if( strlen(sPrimResource) > 0 )
	{
		PutTlvBuffer ( AUTO_ROUTING_CODE , sTlvBuffer, sPrimResource , RESOURCE_ID_LEN);
		/* ELKAM START 30-11-21 PLUTONL-3914 */
		
		GetTlvBuffer(AUTO_ROUTING_RES_INFO_1, sTlvBuffer, sRoutingInfo_1, &nLength);
	
		GetResInfoData(RES_INF_RES_PID,sRoutingInfo_1,sPid,&nLength);
		GetResInfoData(RES_INF_RES_NODE,sRoutingInfo_1,sResNode,&nLength);
		GetResInfoData(RES_INF_RES_ID  ,sRoutingInfo_1,sResID,&nLength);

		GetTlvBuffer(AUTO_SECURITY_DATA,sTlvBuffer, sSecurityData, &nLength);

		PutSecurityData(DESTINATION_PID, sSecurityData, sPid, strlen(sPid));						/* DESTINATION PID  from FLD 404 to FLD 53 */
		PutSecurityData(DESTINATION_NODE_ID, sSecurityData, sResNode, strlen(sResNode));			/* DESTINATION NODE from FLD 404 to FLD 53 */
		PutSecurityData(DESTINATION_RESOURCE, sSecurityData, sResID, strlen(sResID));
		
		PutTlvBuffer ( AUTO_SECURITY_DATA , sTlvBuffer ,  sSecurityData   ,  strlen(sSecurityData));

		/* ELKAM END 30-11-21 PLUTONL-3914 */
	}

	if(	GetTlvBuffer(AUTO_AUTH_NUMBER ,sTlvBuffer,sAuthNum,&nLength) != SUCCES)
		GetAuthNumber ( nIndiceCtx , sTlvBuffer );

	/*Start IBO20220816 PLUTONL-5069*/
	nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
	if ( nRetCode != OK )
	{
		memcpy(sActionCode,MALFUNCTION_ACTION,3);
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
	}
	else
		PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);
	/*End IBO20220816 PLUTONL-5069*/

	PrintTlvBuffer(sTlvBuffer);

	/*Start ICH20170917 Add fee from advice*/
	nRetCode = GenAuthoFees(nIndiceCtx , sTlvBuffer);
	if( nRetCode != OK )
	{
		trace_event ( "Can Not Gen Autho Fees", FATAL );
	}

	if( sShadowAccLevel[0] == 'Y' )
	{
		nRetCode = GetShadowAccBalance(nIndiceCtx , sTlvBuffer);
		if( nRetCode != OK )
		{
			trace_event ( "Can Not Gen Autho Fees", FATAL );
		}
	}

	PrintTlvBuffer(sTlvBuffer);
	
	/*Start AMER20190614:  PROD00069634*/
	nRetCode=FraudUpdate( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{
		trace_event ( "Can Not log authorisation in Fraud table  !!!", FATAL );
	}
	/*End AMER20190614: PROD00069634*/


	/*End ICH20170917*/

        /*Start ICH20170509 nedd to put action flag to update OTB
        nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );

        if ( nRetCode != OK )
        {
                memcpy(sActionCode,MALFUNCTION_ACTION,3);
                PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
        }
        else
                PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode ,3);

        End ICH20170509*/

	
	/*AMER20190313 PROD00066569: Enh5.00: We need to overwrite the EVENT_ACTION because it is used by CBAE to send F39 to the network*/
	if ( GetTlvBuffer(AUTO_ADD_DATA_PRIVATE ,sTlvBuffer, sTlvPrivateBuffer ,&nLength)==SUCCES )
		AnalyseTlvPrivate(sTlvPrivateBuffer, nLength, &sTlvPrivateInfo);
	
	GetTlvBuffer ( AUTO_ACTION_FLAG, sTlvBuffer, sActionFlag, &nLength);
	if ( sActionFlag[0] == ACTION_FLAG_APPROVAL )
		PutTlvPrivate(EVENT_ACTION ,&sTlvPrivateInfo, APPROVED_ACTION,P7_F039_LEN);	
	else
		PutTlvPrivate(EVENT_ACTION ,&sTlvPrivateInfo, MALFUNCTION_EVENT,P7_F039_LEN);
	
	nLength=PrivateBuildTlv(sTlvPrivateBuffer,&sTlvPrivateInfo);
	PutTlvBuffer(AUTO_ADD_DATA_PRIVATE, sTlvBuffer, sTlvPrivateBuffer, nLength);

	PutTlvBuffer( AUTO_EVENT_CODE, sTlvBuffer, ADVICE_ACCEPTED , 3); /*HAL25032022 PLUTONL-3952*/


	nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{ 

#if IS_P7_SWI
		nRetCode = AutSafProcessing(nIndiceCtx, sTlvBuffer, sCaptureCode, sPrimResource);
#endif
		PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"SAF",3);
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		if ( nRetCode != OK )
			trace_event ( "Can Not Write In Authorization SAF Table ", PROCESSING );

		trace_event ( "Can Not Write In Authorization Table ", PROCESSING );
		return ( SEVERE_ERROR );
	}


 /*GetActionCode ICH20180525 is move it to before inert autho to be part of replicated msg with bridge*/
 
	PrintTlvBuffer(sTlvBuffer);


	if ( memcmp ( sActionCode , APPROVED_ACTION , 3 ) != 0 )
	{
		if ( memcmp ( sActionCode , INVALID_PIN_ACTION , 3 ) != 0 )
			nRetCode = UpdateCardActivity( nIndiceCtx , DECLINED_AUTH , sTlvBuffer );
		else
			nRetCode = UpdateCardActivity( nIndiceCtx , PIN_ERROR , sTlvBuffer );

		/*ICH20170917 Need to update balance in case of declined autorisation to impact OTB with online fee*/
		nRetCode = UpdateBalance  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
		if ( nRetCode != OK )
		{
			trace_event ( "Can Not Update Balance  !!!", FATAL );
			/* What is the action to do */
		}	
	}
	else
	{
		nRetCode = UpdateCardActivity  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
		if ( nRetCode != OK )
		{
			trace_event ( "Can Not Update Card Activity  !!!", FATAL );
			/* What is the action to do */
		}

		nRetCode = UpdateBalance  ( nIndiceCtx , APPROVED_AUTH , sTlvBuffer );
		if ( nRetCode != OK ) 
		{
			trace_event ( "Can Not Update Balance  !!!", FATAL ); 
			/* What is the action to do */
		}

	}


	/*TODO: missing update settl*/


	GetTlvBuffer(AUTO_FUNCTION_CODE,sTlvBuffer,sFunctionCode   , &nLength );
	/*EBE: not managed for now
	if( memcmp(sFunctionCode,P7_F024_AUTH_PREV_AMNT_DIFF,P7_F024_LEN) == 0)
	{
	nRetCode = CompletePreauth(nIndiceCtx,sTlvBuffer);
	}*/


#if IS_P7_SWI
	/* check if the destination resource would like to be informed about approvals/declines*/
	if( memcmp(sPrimResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN) != 0 &&
		memcmp(sCaptureNodeId, NodeId, NODE_ID_LEN) == 0 )
	{
		PutTlvBuffer( AUTO_EVENT_CODE, sTlvBuffer, P7_F039_APPROVAL , 3);
		nRetCode = AutSafProcessing(nIndiceCtx, sTlvBuffer, sCaptureCode, sPrimResource);
	}
#endif

	trace_event("End   AuthAdviceProc (OK)",PROCESSING);
	return ( OK );
}
/******************************************************************************/
int AuthBridgeAdviceProc (	int nIndiceCtx, char *sTlvBuffer)
{

	int 			nRetCode;
	int     		nLength;
	char			sTlvPrivateBuffer[1024 + 1];
	TSTlvPrivate    sTlvPrivateInfo;

	trace_event("Start AuthBridgeAdviceProc ()",PROCESSING);
	
	memset(sTlvPrivateBuffer, 0, sizeof (sTlvPrivateBuffer));
	InitTlvPrivate (&sTlvPrivateInfo);

	PrintTlvBuffer(sTlvBuffer);

	nRetCode = BridgeAuthAdvice(nIndiceCtx, sTlvBuffer);

	if ( nRetCode != OK ) 
	{
		if ( GetTlvBuffer(AUTO_ADD_DATA_PRIVATE , sTlvBuffer, sTlvPrivateBuffer ,&nLength) == SUCCES )
			AnalyseTlvPrivate(sTlvPrivateBuffer, nLength, &sTlvPrivateInfo);

		PutTlvPrivate(EVENT_ACTION , &sTlvPrivateInfo, ISS_MALFUNCTION_ACTION, P7_F039_LEN);

		nLength= PrivateBuildTlv(sTlvPrivateBuffer, &sTlvPrivateInfo);
		PutTlvBuffer(AUTO_ADD_DATA_PRIVATE, sTlvBuffer, sTlvPrivateBuffer, nLength);
	}
	PrintTlvBuffer(sTlvBuffer);

	trace_event("End   AuthBridgeAdviceProc (OK)",PROCESSING);

	return nRetCode;
}

int AuthBridgeReconcilProc (int nIndiceCtx, char *sTlvBuffer)
{

	int 			nRetCode;
	int     		nLength;
	char			sTlvPrivateBuffer[1024 + 1];
	TSTlvPrivate    sTlvPrivateInfo;

	trace_event("Start AuthBridgeReconcilProc ()",PROCESSING);
	
	memset(sTlvPrivateBuffer, 0, sizeof (sTlvPrivateBuffer));
	InitTlvPrivate (&sTlvPrivateInfo);

	PrintTlvBuffer(sTlvBuffer);

	nRetCode = BridgeReconcilAdvice(nIndiceCtx, sTlvBuffer);

 	if ( nRetCode != OK )  
	{
		if ( GetTlvBuffer(AUTO_ADD_DATA_PRIVATE , sTlvBuffer, sTlvPrivateBuffer ,&nLength) == SUCCES )
			AnalyseTlvPrivate(sTlvPrivateBuffer, nLength, &sTlvPrivateInfo);

		PutTlvPrivate(EVENT_ACTION , &sTlvPrivateInfo, ISS_MALFUNCTION_ACTION, P7_F039_LEN);

		nLength= PrivateBuildTlv(sTlvPrivateBuffer, &sTlvPrivateInfo);
		PutTlvBuffer(AUTO_ADD_DATA_PRIVATE, sTlvBuffer, sTlvPrivateBuffer, nLength);
	}

	PrintTlvBuffer(sTlvBuffer);

	trace_event("End   AuthBridgeReconcilProc (OK)",PROCESSING);

	return nRetCode;
}

