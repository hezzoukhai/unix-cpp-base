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
#include <hps_iso_8583.h>
/******************************************************************************/
int InternalReversalProc(int nIndiceCtx, char*	sTlvBuffer, char Mode)
{

	char	sRoutingCode [6 + 1];
	char	ReversalXmit [ 10 + 1 ];
	char	ReversalStan [  6 + 1 ];
	char	szCaptureCode[  6 + 1 ];
	char	szOrigTableInd[3 + 1];
	char	szSecurityData[P7_F053_MAX_LEN + 1];
	int		nLength;
	char	sActionCode 		[ 3 + 1 ];
	char	sDstNodeId[NODE_ID_LEN + 1];
	char	sCaptureResInfo[128];
	char	sCaptureNodeId[4 + 1];
	char	sMTI[4 + 1];
	char	sEvent		[12];
	char	sLine[MAX_LINE_TRC];
	char	sExternalRoutingInfo[RES_INFO_LEN + 1];
	char	slProcCode[64 + 1];
	char	slCardNumber[24 + 1];
	char	sAppCode[4 + 1];
	char 	sCardSeqNbr[3+1];
	char	sData[BUF_LG_MAX];
	char	offline_pin_cap;
	int		nRetCode;
	int		nResult;
	char	sSecRoutingCode [RESOURCE_ID_LEN + 1];

	trace_event("Start InternalReversalProc()",PROCESSING);

	memset(ReversalXmit,'\0',sizeof(ReversalXmit));
	memset(ReversalStan,'\0',sizeof(ReversalStan));
	memset(szCaptureCode,'\0',sizeof(szCaptureCode));
	memset(sRoutingCode,'\0',sizeof(sRoutingCode));
	memset(sActionCode,'\0',sizeof(sActionCode));
	memset(szOrigTableInd,'\0',sizeof(szOrigTableInd));
	memset(sEvent,'\0',sizeof(sEvent));
	memset(szSecurityData,'\0',sizeof(szSecurityData));
	memset(sDstNodeId,'\0',sizeof(sDstNodeId));
	memset(sMTI,'\0',sizeof(sMTI));
	memset(sExternalRoutingInfo,'\0',sizeof(sExternalRoutingInfo));
	memset(slProcCode, '\0', sizeof(slProcCode));
	memset(slCardNumber, '\0', sizeof(slCardNumber));
	memset(sAppCode, '\0', sizeof(sAppCode));
	memset(sData, '\0', sizeof(sData));
	memset(sCardSeqNbr, '\0', sizeof(sCardSeqNbr));
	memset(sSecRoutingCode,'\0',sizeof(sSecRoutingCode));
	


	GetTlvBuffer(AUTO_TRANS_DATE_TIME,sTlvBuffer,ReversalXmit,&nLength);
	GetTlvBuffer(AUTO_INTERNAL_STAN,sTlvBuffer,ReversalStan,&nLength);
	GetTlvBuffer(AUTO_CAPTURE_CODE,sTlvBuffer,szCaptureCode,&nLength);
	GetTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,szOrigTableInd,&nLength);
	GetTlvBuffer(AUTO_PROCESSING_CODE, sTlvBuffer, slProcCode, &nLength);

	GetTlvBuffer ( AUTO_CAPTURE_RES_INFO    , sTlvBuffer, sCaptureResInfo   , &nLength );

	GetResInfoData(RES_INF_RES_NODE,sCaptureResInfo,sCaptureNodeId,&nLength);


	PutTlvBuffer(AUTO_REVERSAL_DATE, sTlvBuffer, ReversalXmit, 10);
	PutTlvBuffer(AUTO_REVERSAL_STAN, sTlvBuffer, ReversalStan, 6);

	/*Start SKA20221026 PLUTONL-5184*/
	if (Mode == 'I')
	{
		nRetCode = ReversalProcedure(nIndiceCtx , 
									NORMAL_MODE , 
									"Y",
									sTlvBuffer , 
									szCaptureCode , 
									sRoutingCode );
	
	}
	else if (Mode == 'N')
	{
		nRetCode = ReversalProcedure(nIndiceCtx , 
									NORMAL_MODE , 
									"Y",
									sTlvBuffer , 
									szCaptureCode , 
									sRoutingCode );
		if (nRetCode != OK)
		{
			trace_event("ReversalProcedure() Failed",PROCESSING);
			trace_event("The reversal is being routed",PROCESSING);

			nRetCode = GetAuthRouting(  nIndiceCtx , 
									sTlvBuffer ,
									sRoutingCode , 
									sSecRoutingCode );
		}
		
	}
	/*End SKA20221026 PLUTONL-5184*/
	PrintTlvBuffer (  sTlvBuffer );

	GetTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,sEvent,&nLength);
    GetTlvBuffer(AUTO_MSG_TYPE,sTlvBuffer,sMTI,&nLength);
	PutTlvBuffer( AUTO_AUTHO_FLAG, sTlvBuffer, "P" ,  1);/*ICH20170323 Added for bridge in order to insert into DB SAF*/

	if ( nRetCode != OK )
	{

		nResult = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );
		if ( nResult != OK )
			memcpy(sActionCode,MALFUNCTION_ACTION,P7_F039_LEN);
		else
			memcpy(sActionCode,sActionCode,P7_F039_LEN);
	}
	else
	{
/*ICH20180109 for partial reversal of preauth we convert 1420 to 1220 aaction code to be pproved */
		/*memcpy(sActionCode,P7_F039_REV_ACCEPTED,P7_F039_LEN);*/
		if( sMTI[MSG_TYPE_CLASS] == MSG_TYPE_P2_REV )
		{
			memcpy(sActionCode,P7_F039_REV_ACCEPTED,P7_F039_LEN);
		}

		else {
			memcpy(sActionCode,P7_F039_APPROVAL,P7_F039_LEN);
		}



		if( memcmp(slProcCode + 6, P7_F003_PIN_CHANGE, P7_F003_P1_LEN) == 0 )
		{
			GetTlvBuffer(AUTO_PAN, sTlvBuffer, slCardNumber, &nLength);
			GetTlvBuffer(AUTO_CARD_SEQUENCE_NUMBER, sTlvBuffer, sCardSeqNbr, &nLength);	/*AMER20191025: PLUTONL-490*/
			nRetCode = PinInfoReversal(nIndiceCtx,slCardNumber,sCardSeqNbr);
			if( nRetCode != OK )
			{
				trace_event(">> Unable to reverse PIN value", ERROR);
			}
			else
			{
				if( GetTlvBuffer(AUTO_EMV_CARD_INFO_PARAM, sTlvBuffer, sData, &nLength) == SUCCESS )
				{
					offline_pin_cap		= sData[13];
					memcpy(sAppCode,sData,4);
				}
				if( offline_pin_cap != '0')
				{
					nRetCode = GenPC_IssScript(nIndiceCtx,slCardNumber,sAppCode);
					if( nRetCode != OK )
					{
						trace_event(">> Unable to reverse PIN value", ERROR);
					}
				}
			}
		}
	}

	memcpy(sDstNodeId,NodeId,4);
	if ( memcmp ( sRoutingCode , STAND_IN_RESOURCE , 6 ) != 0 )
	{
		nRetCode = GetResourceNode(nIndiceCtx,sRoutingCode,sDstNodeId);
		if( nRetCode != OK )
		{
			sprintf(sLine,"Error on get node id [%s]",sRoutingCode);
			trace_event(sLine, ERROR);
		}
	}

	PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,sActionCode,3);
	PutTlvBuffer(AUTO_TABLE_INDICATOR,sTlvBuffer,szOrigTableInd,3);
	/*PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"MSC",3);ICH20180109*/
	if( sMTI[MSG_TYPE_CLASS] == MSG_TYPE_P2_REV )
	{
	PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR,sTlvBuffer,"MSC",3);
	}
	/*EBE151205: Not needed. a Node which did'nt capture the reversal should not forward it
	PutTlvBuffer(AUTO_ROUTING_CODE ,sTlvBuffer,STAND_IN_RESOURCE ,6);
	*/
	PutTlvBuffer(AUTO_ROUTING_CODE ,sTlvBuffer,sRoutingCode ,strlen(sRoutingCode));/*ICH used by Reversal request*/
	GetTlvBuffer(AUTO_SECURITY_DATA,sTlvBuffer,szSecurityData,&nLength);
	PutSecurityData(SOURCE_RESOURCE, szSecurityData, szCaptureCode, SOURCE_RESOURCE_LEN);
	PutSecurityData(SOURCE_NODE_ID, szSecurityData, NodeId, NODE_ID_LEN);
	PutSecurityData(DESTINATION_RESOURCE, szSecurityData, sRoutingCode, DESTINATION_RESOURCE_LEN);
	PutSecurityData(DESTINATION_NODE_ID, szSecurityData, sDstNodeId, NODE_ID_LEN);


	PutTlvBuffer(AUTO_SECURITY_DATA ,sTlvBuffer,szSecurityData ,strlen(szSecurityData));

	InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );

	if( nRetCode == OK )
	{
		UpdateSettlement( nIndiceCtx , sTlvBuffer);

		if ( memcmp ( sRoutingCode , STAND_IN_RESOURCE , 6 ) != 0 &&
			memcmp(sCaptureNodeId, NodeId, NODE_ID_LEN) == 0 )
		{

			InsertionSaf ( nIndiceCtx , sTlvBuffer, szCaptureCode , sRoutingCode );

		}
	}

	trace_message(PROCESSING, "End InternalReversalProc(%d)", nRetCode);
	return(nRetCode);
}


/******************************************************************************/

int ReversalBridgeProc(int nIndiceCtx, char*	sTlvBuffer)
{

        int                     nRetCode;

        trace_event("Start ReversalBridgeProc ()",PROCESSING);

        PrintTlvBuffer(sTlvBuffer);

        nRetCode=BridgeRevAdvice(nIndiceCtx,sTlvBuffer);

        PrintTlvBuffer(sTlvBuffer);

        trace_event("End   ReversalBridgeProc (OK)",PROCESSING);

        return nRetCode;
}

/*Start SKA20221026 PLUTONL-5184*/
int NationalReversalProc(int nIndiceCtx, char*	sTlvBuffer)
{
	int 	nLength, nResult;
	char	sSettlmentCurrency [3 + 1];
	char	sTransactionCurrency [3 + 1];
	char	sData [BUF_LG_MAX];

	memset(sSettlmentCurrency, '\0', sizeof(sSettlmentCurrency));
	memset(sTransactionCurrency,'\0',sizeof(sTransactionCurrency));
	memset(sData,'\0',sizeof(sData));

	if (GetTlvBuffer(AUTO_AMOUNT_SETTLEMENT,sTlvBuffer,sData,&nLength) != SUCCESS 
		&& GetTlvBuffer(AUTO_AMOUNT_TRANSACTION,sTlvBuffer,sData,&nLength) == SUCCESS)
	{
		PutTlvBuffer(AUTO_AMOUNT_SETTLEMENT, sTlvBuffer, sData, 12);
	}

	if (GetTlvBuffer(AUTO_SETL_CURRENCY_CODE,sTlvBuffer,sSettlmentCurrency,&nLength) != SUCCESS 
		&& GetTlvBuffer(AUTO_TRAN_CURRENCY_CODE,sTlvBuffer,sSettlmentCurrency,&nLength) == SUCCESS)
	{
		PutTlvBuffer(AUTO_SETL_CURRENCY_CODE, sTlvBuffer, sSettlmentCurrency, 3);
	}

	nResult = ReversalAuthAmountMng(nIndiceCtx, sTlvBuffer);
	if (nResult != OK)
	{
		trace_event("End NationalReversalProc (NOK): ReversalAuthAmountMng Failed",PROCESSING);
		return NOK;
	}

	nResult = InternalReversalProc(nIndiceCtx, sTlvBuffer, 'N');
	if (nResult != OK)
	{
		trace_event("End NationalReversalProc (NOK): InternalReversalProc Failed",PROCESSING);
		return NOK;
	}

	trace_event("End NationalReversalProc(OK)", PROCESSING);
	return OK;

}

int ReversalProc(int nIndiceCtx, char*	sTlvBuffer)
{
	int nResult;

	nResult = InternalReversalProc(nIndiceCtx, sTlvBuffer, 'I');
	if (nResult != OK)
	{
		trace_event("End ReversalProc (NOK): InternalReversalProc Failed",PROCESSING);
		return NOK;
	}

	trace_event("End ReversalProc(OK)", PROCESSING);
	return OK;
}
/*End SKA20221026 PLUTONL-5184*/


