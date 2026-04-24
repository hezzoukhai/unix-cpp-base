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

#include <p7_services.h>
#include <p7_define.h>
#include <p7_sid.h>

static int IssuerTimeOutProc(int nIndexCtx, TSIsoInfo *IsoInfo, char*	sTlvBuffer)
{

	char				sLine[MAX_LINE_TRC];
	char				sTlvPrivateBuffer [ 1024];
	char 				sSecResource    [ RESOURCE_ID_LEN + 1 ];
	TSTlvPrivate		sTlvPrivateInfo;
	int     			nLength;
	int					nRetCode;
	char				sServContainer	[CONTAINER_NAME_MAX_LEN + 1];
	char				sServReplyEntry	[SERVICE_NAME_MAX_LEN + 1];
	fnServiceReply		fnReplyEntry;

	trace_event("Start IssuerTimeOutProc()",PROCESSING);
	memset(sSecResource,'\0',sizeof(sSecResource));

	InitTlvPrivate(&sTlvPrivateInfo);


	PutTlvBuffer (AUTO_EVENT_CODE    ,sTlvBuffer , TIME_OUT_TRX,3);
	PutTlvBuffer (AUTO_RESPONSE_CODE ,sTlvBuffer , ISSUER_UNAVAILABLE_ACTION ,3);
	PutTlvBuffer (AUTO_AUTHO_FLAG ,sTlvBuffer , "P" ,1);
	PutTlvBuffer ( AUTO_AUTHOR_SRC  , sTlvBuffer, "E2" , 2);
	PutTlvBuffer ( AUTO_MESSAGE_REASON_CODE , sTlvBuffer, P7_F025_ISS_TIMEOUT , P7_F025_LEN );

	
	if( GetTlvBuffer ( AUTO_ADD_DATA_PRIVATE , sTlvBuffer , sTlvPrivateBuffer ,&nLength) == SUCCESS )
	{
		AnalyseTlvPrivate( sTlvPrivateBuffer ,nLength,&sTlvPrivateInfo);
	}

	GetTlvPrivate(SECONDARY_RESOURCE_TAG ,&sTlvPrivateInfo ,sSecResource ,&nLength);

	if ( memcmp (sSecResource , STAND_IN_RESOURCE , RESOURCE_ID_LEN ) == 0 )
	{
		nRetCode = StandinProc( nIndexCtx , sTlvBuffer, P7_F025_TO_STANDIN );
		if( nRetCode == ROUTAGE )
		{
			return(nRetCode);
		}
		if( nRetCode != OK )
		{
			nRetCode = UpdateAuthoActivity ( nIndexCtx , sTlvBuffer );
			trace_event("End   IssuerTimeOutProc(StandIn Failure)",PROCESSING);
			return(NOK);
		}
	}

	memset(sServContainer, 0, sizeof(sServContainer));
	memset(sServReplyEntry, 0, sizeof(sServReplyEntry));


	GetTlvBuffer ( AUTO_SERV_CONTAINER , sTlvBuffer , sServContainer ,&nLength);
	GetTlvBuffer ( AUTO_SERV_ROUTING_REPLY_ENTRY , sTlvBuffer , sServReplyEntry ,&nLength);
	if( strlen(sServContainer) == 0 || strlen(sServReplyEntry) == 0 )
	{
		nRetCode = UpdateAuthoActivity ( nIndexCtx , sTlvBuffer );
		trace_event("End   IssuerTimeOutProc(No Reply process)",PROCESSING);
		return OK;
	}

	fnReplyEntry = GetServiceReply(sServContainer,sServReplyEntry);
	if( fnReplyEntry == NULL )
	{
		nRetCode = UpdateAuthoActivity ( nIndexCtx , sTlvBuffer );
		trace_event("End   IssuerTimeOutProc(Inv Reply Process)",PROCESSING);
		return(OK);
	}

	nRetCode = (*fnReplyEntry)(nIndexCtx,IsoInfo, sTlvBuffer,FWD_MSG_REPLIED);


	return nRetCode;
}


static int AcquirerTimeOutProc(int nIndexCtx, TSIsoInfo *IsoInfo, char*	sTlvBuffer)
{

	trace_event("Start AcquirerTimeOutProc()",PROCESSING);

	trace_event("End   AcquirerTimeOutProc(OK)",PROCESSING);

	return OK;
}


int OnTimeoutServiceRequest(int nIndexCtx,char queue_type, char* sTlvBuffer)
{
	int				nResult;
	TSIsoInfo		kIsoInfo;

	InitIsoInfo(&kIsoInfo);
	if( queue_type == NODE_LIST_BO )
	{
		nResult = AcquirerTimeOutProc(nIndexCtx,&kIsoInfo,sTlvBuffer);
	}
	else if( queue_type == NODE_LIST_LINE )
	{
		nResult = IssuerTimeOutProc(nIndexCtx,&kIsoInfo,sTlvBuffer);
	}

	return nResult;
}

int OnTimeoutIsoServiceRequest(int nIndexCtx,char queue_type, TSIsoInfo *IsoInfo, char* sTlvBuffer)
{
	int nResult;

	if( queue_type == NODE_LIST_BO )
	{
		nResult = AcquirerTimeOutProc(nIndexCtx,IsoInfo,sTlvBuffer);
	}
	else if( queue_type == NODE_LIST_LINE )
	{
		nResult = IssuerTimeOutProc(nIndexCtx,IsoInfo,sTlvBuffer);
	}


	return nResult;
}
