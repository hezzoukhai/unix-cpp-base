#include <define.h>
#include <iso_hps.h>
#include <tlv_autho.h>
#include <p7_sid.h>
#include <queue.h>
#include <list_thr.h>
#include <mod_fraud.h>
#include <hps_iso_8583.h>
#include <tlv_private.h>
#include <resources.h>
#include <security_flags.h>





/*********************************************************************************/

int FrdCheckInquiry(int nIndexCtx,char* sTlvBuffer)
{
	char			sFieldData[BUF_LG_MAX];
	char			sFraudParameters[FRD_PARAM_LEN + 1];
	char			sRoutingCode[RESOURCE_ID_LEN + 1];
	char			sExternalResource[RESOURCE_ID_LEN + 1];
	char			sPrimaryResource[RESOURCE_ID_LEN + 1];
	char			sTimeOutPeriodStr[FRD_TO_PER_LEN + 1];
	char			sDefaultAction[3 + 1];
	char			sRTFlag[1 + 1];
	char			sOrigRTFlag[1 + 1];
	pid_t			nDestPid;
	char			sLine[MAX_LINE_TRC];
	char			sCardNumber[24 + 1];
	char			sSecurityResults[ SEC_FLAG_LEN + 1 ];
	char			sSTAN[6 + 1];
	char			sMsgType[4 + 1];
	TSIsoInfo		IsoInfo;
	TSTlvAutho		sTlvInfo;
	TSNodeData      Node;
	TSBalExchange   BalMessage;
	TSTlvPrivate    TlvPrivData;
	int				nResult;
	int				nFieldLength;
	unsigned int	nTimeOutPeriod;
	unsigned int	nTimeOutPeriod_sec;
	unsigned int	nTimeOutPeriod_usec;
	int				nReturnCode;

	

	trace_event("Start  FrdCheckInquiry()", PROCESSING);


	memset(sFraudParameters,'\0',sizeof(sFraudParameters));
	memset(sRoutingCode,'\0',sizeof(sRoutingCode));
	memset(sPrimaryResource,'\0',sizeof(sPrimaryResource));
	memset(sExternalResource,'\0',sizeof(sExternalResource));
	memset(sTimeOutPeriodStr,'\0',sizeof(sTimeOutPeriodStr));
	memset(sDefaultAction,'\0',sizeof(sDefaultAction));
	memset(sRTFlag,'\0',sizeof(sRTFlag));
	memset(sOrigRTFlag,'\0',sizeof(sOrigRTFlag));
	memset(sCardNumber,'\0',sizeof(sCardNumber));
	memset(sSTAN,'\0',sizeof(sSTAN));
	memset(sMsgType,'\0',sizeof(sMsgType));
	memset(sSecurityResults,'\0',sizeof(sSecurityResults));
	nReturnCode = OK;
	
	
	InitIsoInfo(&IsoInfo);
	InitTlvAutho    ( &sTlvInfo );
	InitNodeData(&Node);



	if( GetTlvBuffer ( AUTO_FRAUD_PARAMS , sTlvBuffer, sFraudParameters , &nFieldLength) != SUCCESS )
	{
		trace_event("End    FrdCheckInquiry(OK): No Fraud check", PROCESSING);
		return OK;
	}


	memcpy(sRoutingCode, sFraudParameters + FRD_RT_CD_OFST, FRD_RT_CD_LEN);
	memcpy(sDefaultAction, sFraudParameters + FRD_DEF_ACTION_OFST, FRD_DEF_ACTION_LEN);
	memcpy(sRTFlag, sFraudParameters + FRD_RT_FLAG_OFST, FRD_RT_FLAG_LEN);
	memcpy(sOrigRTFlag, sFraudParameters + FRD_RT_FLAG_OFST, FRD_RT_FLAG_LEN);

	GetTlvBuffer ( AUTO_MSG_TYPE , sTlvBuffer, sMsgType , &nFieldLength);

	if( sMsgType[MSG_TYPE_FUNCTION] == MSG_TYPE_P3_ADV )
	{
		sRTFlag[0]='N';
		sOrigRTFlag[0]='N';
	}

	if( GetTlvBuffer ( AUTO_SECURITY_VER_RESULTS , sTlvBuffer, sSecurityResults , &nFieldLength) != SUCCESS )
	{
		trace_event("End    FrdCheckInquiry(OK): Missing SECURITY_VER_RESULTS", PROCESSING);
		return NOK;
	}


	memcpy(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN);
	memcpy(sPrimaryResource, sRoutingCode, RESOURCE_ID_LEN);
	
	if (memcmp(sRoutingCode, STAND_IN_RESOURCE, RESOURCE_ID_LEN) == 0)
	{
		memcpy(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
	}
	else if (memcmp(sRoutingCode, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) != 0)
	{

		memcpy(sFieldData,sFraudParameters + FRD_RT_CD_PID_OFST, FRD_RT_CD_PID_LEN);
		sFieldData[FRD_RT_CD_PID_LEN]='\0';
		nDestPid = atoi(sFieldData);
		if( kill ( nDestPid , 0 ) == 0 )
		{
			memcpy(sExternalResource, sRoutingCode, RESOURCE_ID_LEN);
		}
	}

	if (memcmp(sExternalResource, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) == 0)
	{
		memcpy(sRoutingCode, sFraudParameters + FRD_ALT_RT_CD_OFST, FRD_ALT_RT_CD_LEN);
		if (memcmp(sRoutingCode, STAND_IN_RESOURCE, RESOURCE_ID_LEN) == 0)
		{
			memcpy(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN);
		}
		else if (memcmp(sRoutingCode, NO_ROUTING_RESOURCE, RESOURCE_ID_LEN) != 0)
		{
			memcpy(sFieldData,sFraudParameters + FRD_ALT_RT_CD_PID_OFST, FRD_ALT_RT_CD_PID_LEN);
			sFieldData[FRD_ALT_RT_CD_PID_LEN]='\0';
			nDestPid = atoi(sFieldData);
			if( kill ( nDestPid , 0 ) == 0 )
			{
				memcpy(sExternalResource, sRoutingCode, RESOURCE_ID_LEN);
			}
		}
	}

	if( memcmp(sExternalResource,NO_ROUTING_RESOURCE,RESOURCE_ID_LEN) == 0 )
	{
		memcpy(sExternalResource, sPrimaryResource, RESOURCE_ID_LEN);
		trace_event(">>>> No available resource", ERROR);

		if( sRTFlag[0] == 'Y' )
		{
			/*We should at least make sure it's processed later to update counters*/
			sRTFlag[0] = 'N';
			nReturnCode = NOK;
		}
	}


	/* non real-time */
	if( sRTFlag[0] == 'N')
	{
		GetTlvBuffer ( AUTO_PAN , sTlvBuffer, sCardNumber , &nFieldLength);
		GetTlvBuffer ( AUTO_INTERNAL_STAN , sTlvBuffer, sSTAN , &nFieldLength);
		
		nResult =  InsertSafData  ( nIndexCtx,
								sMsgType,
								'A',
								sSTAN,
								ResourceId,
								sExternalResource,
								0,
								"000",
								sTlvBuffer,
								sCardNumber
								);
		if( nResult != OK )
		{
			trace_event("End    FrdCheckInquiry(NOK): Error insert saf data", ERROR);
			return NOK;
		}

		if( sOrigRTFlag[0] == 'Y' )
		{
			PutTlvBuffer (  AUTO_RESPONSE_CODE , sTlvBuffer, sDefaultAction , P7_F039_LEN );
		}		
		trace_event("End    FrdCheckInquiry(OK): NRT", PROCESSING);
		return nReturnCode;
	}


	if( memcmp(sExternalResource,STAND_IN_RESOURCE,RESOURCE_ID_LEN) == 0 )
	{
		nResult = CheckFraudRulesTlv(nIndexCtx,sTlvBuffer);		
		trace_event("End    FrdCheckInquiry(OK)", PROCESSING);		
		return nResult;
	}

	

	nResult = AnalyseTlvAutho ( sTlvBuffer , &sTlvInfo );
	nResult = TlvToIso        ( nIndexCtx , &sTlvInfo   , &IsoInfo  , REQUEST );

	/*JOU20170718: add missing fields to Iso*/
	IsoInfo.nMsgType = atoi(sMsgType);
	if ( GetTlvAutho( AUTO_INTERNAL_STAN , &sTlvInfo,sFieldData,&nFieldLength) == SUCCES )
		PutIsoField ( ISO_AUDIT_NBR , &IsoInfo , sFieldData , nFieldLength );

	if ( GetTlvAutho( AUTO_PAN , &sTlvInfo,sFieldData,&nFieldLength) == SUCCES )
		PutIsoField ( ISO_CARD_NBR , &IsoInfo , sFieldData , nFieldLength );
	/**/

	GetIsoField(ISO_AUDIT_NBR, &IsoInfo, sFieldData, &nFieldLength);
	memcpy(Node.szFstKey, sFieldData, P7_F011_LEN);

	GetIsoField(ISO_XMIT_TIME, &IsoInfo, sFieldData, &nFieldLength);
	memcpy(Node.szFstKey + P7_F011_LEN, sFieldData, nFieldLength);
	
	/*EBETODO: should we keep both iso and tlv?*/
	nFieldLength = IsoBuildMsg(sFieldData,&IsoInfo);
	memcpy(Node.sFstBuf, sFieldData, nFieldLength);
	Node.nFstBufLen = nFieldLength;

	strncpy(Node.sSndBuf, sTlvBuffer, sizeof(Node.sSndBuf) - 1);
	Node.nFstBufLen = strlen(Node.sSndBuf);


	memcpy(BalMessage.sBody, sFieldData, nFieldLength);
	BalMessage.sHeader.nLen = nFieldLength;

	memcpy(sTimeOutPeriodStr, sFraudParameters + FRD_TO_PER_OFST, FRD_TO_PER_LEN);

	nTimeOutPeriod = atoi(sTimeOutPeriodStr)*1000; /*from ms to us*/
	nTimeOutPeriod_sec = nTimeOutPeriod/1000000;
	nTimeOutPeriod_usec = nTimeOutPeriod%1000000;

	Node.nFstPrvtDataLen = nTimeOutPeriod;
	Node.nTrdPrvtDataLen=0;
	memcpy(Node.sTrdPrvtData,sDefaultAction,3);

	
	if( Node.stPurgeTime.tv_usec + nTimeOutPeriod_usec >= 1000000)
	{
		nTimeOutPeriod_usec -= 1000000;
		nTimeOutPeriod_sec++;
	}

	gettimeofday(&Node.stPurgeTime,NULL);
	Node.stPurgeTime.tv_sec += nTimeOutPeriod_sec;
	Node.stPurgeTime.tv_sec -= EPOCH_TIME_ADJUSTMENT;
	Node.stPurgeTime.tv_usec += nTimeOutPeriod_usec;

	sprintf(sLine, "==> FraudInsertkey = [%s] ", Node.szFstKey);
	trace_event(sLine, TRACE);

	if (InsertFraudRequest(&Node) != OK) 
	{
		trace_event("FrdCheckInquiry(): InsertRequest() failed", PROCESSING);
		return (NOK);
	}

	/*JOU20170711 : added this part, because nDestPid is unset.*/
	memcpy(sFieldData,sFraudParameters + FRD_RT_CD_PID_OFST, FRD_RT_CD_PID_LEN);
	sFieldData[FRD_RT_CD_PID_LEN]='\0';
	nDestPid = atoi(sFieldData);
	/*EBETODO: why unset? we did at the begining to check if resource is UP... to be checked*/
	/**/

	BalMessage.sHeader.nPartnerId = nDestPid;
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1100;
	
	if (WriteBalMsg(nBalFrd, &BalMessage) != OK) 
	{
		trace_event("FrdCheckInquiry(): WriteBalMsg() failed", PROCESSING);
		/* free request from list */

		FetchFraudRequest(&Node);
		return (NOK);
	}

	sSecurityResults[SEC_FLAG_CHK_FRAUD] = '2';
	PutTlvBuffer (  AUTO_SECURITY_VER_RESULTS , sTlvBuffer, sSecurityResults , strlen(sSecurityResults) );


	trace_event("End    FrdCheckInquiry(OK)", PROCESSING);
	return OK;
}

int FrdCheckControl(int nIndexCtx,char* sTlvBuffer)
{
	char			sLine 			[ MAX_LINE_TRC];
	char			sResponseCode	[3 + 1];
	int				nFieldLength;
	int				nReturnCode;
	int				nResult;

	trace_event("Start  FrdCheckControl()", PROCESSING);

	memset(sResponseCode,'\0',sizeof(sResponseCode));

	nResult = FrdCheckInquiry(nIndexCtx,sTlvBuffer);
	if( nResult == OK )
	{
		trace_event("End    FrdCheckControl(OK)", PROCESSING);
		return OK;
	}

	if( GetTlvBuffer ( AUTO_RESPONSE_CODE , sTlvBuffer, sResponseCode , &nFieldLength) != SUCCESS )
	{
		PutTlvBuffer (  AUTO_RESPONSE_CODE , sTlvBuffer, P7_F039_TRN_PROC_FAULT , P7_F039_LEN );
	}
	
	trace_event("End    FrdCheckControl(NOK)", PROCESSING);
	return NOK;
}

int FrdCheckResponse(int nIndexCtx, char* sTlvBuffer)
{
	TSNodeData      Node;
	int				nResult;
	char            sFieldData[BUF_LG_MAX];
	char			sSecurityResults[ SEC_FLAG_LEN + 1 ];
	char			sResponseCode[3 + 1];
	int             nFieldLength;
	char sLine 		 [ MAX_LINE_TRC];
	int				nReturnCode;
	
	trace_event("Start  FrdCheckResponse()", PROCESSING);

	memset(sSecurityResults,'\0',sizeof(sSecurityResults));
	memset(sResponseCode,'\0',sizeof(sResponseCode));
	InitNodeData(&Node);

	if( GetTlvBuffer ( AUTO_SECURITY_VER_RESULTS , sTlvBuffer, sSecurityResults , &nFieldLength) != SUCCESS )
	{
		trace_event("End    FrdCheckResponse(OK): Missing SECURITY_VER_RESULTS", PROCESSING);
		return NOK;
	}

	if( sSecurityResults[SEC_FLAG_CHK_FRAUD] != '2' )
	{
		trace_event("End    FrdCheckResponse(OK): No Pending request", PROCESSING);
		return OK;
	}

	GetTlvBuffer ( AUTO_INTERNAL_STAN , sTlvBuffer, sFieldData, &nFieldLength);
	memcpy(Node.szFstKey, sFieldData, P7_F011_LEN);

	GetTlvBuffer ( AUTO_TRANS_DATE_TIME , sTlvBuffer, sFieldData, &nFieldLength);
	memcpy(Node.szFstKey + P7_F011_LEN, sFieldData, nFieldLength);

	sprintf(sFieldData,"Fetch key=[%s]",Node.szFstKey);
	trace_event(sFieldData, PROCESSING);

	nReturnCode = OK;

	nResult = FetchFraudResponse (&Node, sResponseCode);
	if( nResult != OK )
	{
		if( strlen(sResponseCode) == 0 )
		{
			memcpy(sResponseCode,P7_F039_TRN_PROC_FAULT , P7_F039_LEN );
		}
		sSecurityResults[SEC_FLAG_CHK_FRAUD] = '3';
		nReturnCode = NOK;
	}
	else
	{
		if( memcpy(sResponseCode,"000",3) == 0 )
		{
			sSecurityResults[SEC_FLAG_CHK_FRAUD] = '0';
		}
		else
		{
			sSecurityResults[SEC_FLAG_CHK_FRAUD] = '1';
			nReturnCode = NOK;
		}
	}
	PutTlvBuffer (  AUTO_SECURITY_VER_RESULTS , sTlvBuffer, sSecurityResults , strlen(sSecurityResults) );
	PutTlvBuffer (  AUTO_RESPONSE_CODE , sTlvBuffer, sResponseCode , strlen(sResponseCode) );

	if( nReturnCode == OK )
	{
		trace_event("End    FrdCheckResponse(OK)", PROCESSING);
	}
	else
	{
		trace_event("End    FrdCheckResponse(NOK)", PROCESSING);
	}

	return nReturnCode;
}


int CheckFraudRulesIso(int nIndexCtx, TSIsoInfo*		pkIsoInfo)
{
	char                         sTlvBuffer[ LG_MAX + 1 ];
	char						sRespondeCode[P7_F039_LEN + 1];
	TSTlvAutho                   sTlvInfo;
	int							nLength;
	int							nResult;
	char sLine 					[ MAX_LINE_TRC];

	trace_event("Start  CheckFraudRulesIso()", PROCESSING);


	memset ( sTlvBuffer , 0,  sizeof ( sTlvBuffer ));
	memset ( sRespondeCode , 0,  sizeof ( sRespondeCode ));
	InitTlvAutho(&sTlvInfo);

	IsoToTlv    ( nIndexCtx , pkIsoInfo   , &sTlvInfo );

	nLength = AuthoBuildTlv( sTlvBuffer, &sTlvInfo);

	CheckFraudRulesTlv(nIndexCtx,sTlvBuffer);

	
	if( GetTlvBuffer ( AUTO_RESPONSE_CODE , sTlvBuffer, sRespondeCode ,     &nLength) == SUCCESS )
	{
		PutIsoField(ISO_RESPONSE_CODE, pkIsoInfo, sRespondeCode, nLength);
	}

	trace_event("End    CheckFraudRulesIso(OK)", PROCESSING);
	return OK;
}


int LogFrdAdvice(int nIndexCtx,TSIsoInfo	*pkIsoInfo)
{

	char                        sTlvBuffer[ LG_MAX + 1 ];
	TSTlvAutho                  sTlvInfo;
	int							nLength;
	int							nResult;

	trace_event("Start LogFrdAdvice()", PROCESSING);

	memset ( sTlvBuffer , 0,  sizeof ( sTlvBuffer ));
	InitTlvAutho(&sTlvInfo);

	IsoToTlv    ( nIndexCtx , pkIsoInfo   , &sTlvInfo );

	nLength = AuthoBuildTlv( sTlvBuffer, &sTlvInfo);

	nResult = InsertFraudAuthActivity ( nIndexCtx , sTlvBuffer );
	if ( nResult != OK )
	{
		trace_event("End   LogFrdAdvice(NOK): InsertFraudAuthActivity() failure", ERROR);
		return NOK;
	}
	

	trace_event("End   LogFrdAdvice(OK)", PROCESSING);
	return OK;
}


