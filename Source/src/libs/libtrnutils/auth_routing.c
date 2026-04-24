/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      auth_utils.c                                                           */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Utilities for autorisation : Local autorisation/ Stand In Autoris...*/
/*         Check data integrity of TLV data                                    */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   12/05/2002 -         V 2.0.1                              */
/*        Add a call of PutEvent during the check of the integrity of the TLV  */
/*        data see (HYJCC120502)                                               */
/*******************************************************************************/

#include <stdio.h>
#include <define.h>
#include <iso_hps.h>
#include <sys/time.h>
#include <dump.h>
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
int AuthRouting( int nIndiceCtx,char *sTlvBuffer, char* sRoutingInfo, char* OnRespProc, char* OnTimeoutProc )
{

	TSTlvAutho			sTlvInfo;
	char	sActionCode		[ P7_F039_LEN + 1 ];
	char	sLine			[ MAX_LINE_TRC];
	char	sPid			[ PROC_PID_LEN + 1 ];
	char	sSecurData		[ P7_F053_MAX_LEN + 1 ];
	char	sResourceId		[RESOURCE_ID_LEN + 1];
	char	sNodeId			[NODE_ID_LEN + 1];
	
	
	int		nRetCode;
	int		nLength;
	
	
	

	trace_event ( "Start AuthRouting()"  , PROCESSING );

	memset ( sActionCode , 0 , sizeof ( sActionCode ));
	
	memset  ( sPid , 0 , sizeof ( sPid ));
	
	
	memset ( sSecurData , 0 , sizeof ( sSecurData ));
	memset ( sResourceId , 0 , sizeof ( sResourceId ));
	memset ( sNodeId , 0 , sizeof ( sNodeId ));

	
	GetResInfoData(RES_INF_RES_NODE,sRoutingInfo,sNodeId,&nLength);
	GetResInfoData(RES_INF_RES_ID,sRoutingInfo,sResourceId,&nLength);
	GetResInfoData(RES_INF_RES_PID,sRoutingInfo,sPid,&nLength);
	



	PutTlvBuffer ( AUTO_ROUTING_CODE ,  sTlvBuffer,sResourceId,RESOURCE_ID_LEN);
	PutTlvBuffer ( AUTO_AUTHO_FLAG   ,  sTlvBuffer, "R" ,  1);


	nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{
		trace_event ( "Can Not Write In Authorisation table !!!", FATAL );
		PutTlvBuffer( AUTO_CUR_TABLE_INDICATOR , sTlvBuffer, "SAF" ,  3);
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		if ( nRetCode != OK )
			trace_event("Can Not Write In Authorisation SAF table !!!", FATAL);

		PutTlvBuffer ( AUTO_EVENT_CODE , sTlvBuffer , MALFUNCTION_EVENT , 3);
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
		if ( nRetCode != OK )                                                                                                         
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
		else
			PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
		nRetCode = SEVERE_ERROR;
		trace_event ( "End   AuthRouting(SEVERE_ERROR)", PROCESSING ); 
		return ( nRetCode );
	}


	GetTlvBuffer(AUTO_SECURITY_DATA,sTlvBuffer,sSecurData,&nLength);
	
	PutSecurityData(SOURCE_NODE_ID,sSecurData,NodeId,NODE_ID_LEN);/*ICH20170322 for bridge R Some interfaces still don't fill the source node id by themselves*/	
	PutSecurityData(DESTINATION_PID,sSecurData, sPid,SOURCE_PID_LEN);
	PutSecurityData(DESTINATION_NODE_ID,sSecurData,sNodeId,NODE_ID_LEN);
	PutSecurityData(DESTINATION_RESOURCE,sSecurData,sResourceId,SOURCE_RESOURCE_LEN);
	PutTlvBuffer(AUTO_SECURITY_DATA,sTlvBuffer,sSecurData,strlen(sSecurData));
	

	if( OnTimeoutProc && strlen(OnTimeoutProc) > 0 )
		PutTlvBuffer(AUTO_SERV_ROUTING_TIMEOUT_ENTRY,sTlvBuffer,OnTimeoutProc,strlen(OnTimeoutProc));
	if( OnRespProc && strlen(OnRespProc) > 0 )
		PutTlvBuffer(AUTO_SERV_ROUTING_REPLY_ENTRY,sTlvBuffer,OnRespProc,strlen(OnRespProc));

	nRetCode = ROUTAGE;


	trace_event ( "End   AuthRouting()", PROCESSING ); 
	return ( nRetCode );
}


/******************************************************************************/
int ForwardToBackEndIAS( int nIndiceCtx,char *sTlvBuffer, char* OnRespProc, char* OnTimeoutProc )
{
	/*TSNodeData			Node;*/
	TSTlvAutho			sTlvInfo;
	char				sActionCode		[ P7_F039_LEN + 1 ];
	char				sLine			[ MAX_LINE_TRC];
	char				sPid			[ PROC_PID_LEN + 1 ];
	char				sSecurData		[ P7_F053_MAX_LEN + 1 ];
	char				sSrcResourceId		[RESOURCE_ID_LEN + 1];
	pid_t				nSrcPid;
	char				sDstResourceId		[RESOURCE_ID_LEN + 1];
	char				sDstNodeId			[NODE_ID_LEN + 1];
	pid_t				nDstPid;
	
	int					nRetCode;
	int					nLength;
	
	
	

	trace_event ( "Start ForwardToBackEndIAS()"  , PROCESSING );

	memset ( sActionCode , 0 , sizeof ( sActionCode ));
	memset  ( sPid , 0 , sizeof ( sPid ));
	memset ( sSecurData , 0 , sizeof ( sSecurData ));
	memset ( sSrcResourceId , 0 , sizeof ( sSrcResourceId ));
	memset ( sDstResourceId , 0 , sizeof ( sDstResourceId ));
	memset ( sDstNodeId , 0 , sizeof ( sDstNodeId ));
	/*InitNodeData(&Node);*/


	nRetCode = GetBEBridgeResourceId(nIndiceCtx,NodeId,sSrcResourceId,&nSrcPid,
												sDstNodeId,sDstResourceId,&nDstPid);
	if( nRetCode != OK )
	{
		PutTlvBuffer ( AUTO_EVENT_CODE , sTlvBuffer , MALFUNCTION_EVENT , 3);
		trace_event ( "End   ForwardToBackEndIAS(NOK): GetBEBridgeResourceId() failed"  , ERROR );
		return NOK;
	}
	

	PutTlvBuffer ( AUTO_ROUTING_CODE ,  sTlvBuffer,sDstResourceId,RESOURCE_ID_LEN);
	PutTlvBuffer ( AUTO_AUTHO_FLAG   ,  sTlvBuffer, "R" ,  1);


	nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
	if ( nRetCode != OK )
	{
		trace_event ( "Can Not Write In Authorisation table !!!", FATAL );
		PutTlvBuffer( AUTO_CUR_TABLE_INDICATOR , sTlvBuffer, "SAF" ,  3);
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		if ( nRetCode != OK )
			trace_event("Can Not Write In Authorisation SAF table !!!", FATAL);

		PutTlvBuffer ( AUTO_EVENT_CODE , sTlvBuffer , MALFUNCTION_EVENT , 3);
		nRetCode = GetActionCode( nIndiceCtx , sTlvBuffer , sActionCode );                                                            
		if ( nRetCode != OK )                                                                                                         
			PutTlvBuffer(AUTO_RESPONSE_CODE,sTlvBuffer,MALFUNCTION_ACTION,3);
		else
			PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);
		nRetCode = SEVERE_ERROR;
		trace_event ( "End   AuthRouting(SEVERE_ERROR)", PROCESSING ); 
		return ( nRetCode );
	}


	GetTlvBuffer(AUTO_SECURITY_DATA,sTlvBuffer,sSecurData,&nLength);
	sprintf(sPid,"%.*d",PROC_PID_LEN,nSrcPid);

	PutSecurityData(DESTINATION_PID,sSecurData, sPid,SOURCE_PID_LEN);
	PutSecurityData(DESTINATION_NODE_ID,sSecurData,sDstNodeId,NODE_ID_LEN);
        PutSecurityData(DESTINATION_RESOURCE,sSecurData,sSrcResourceId,SOURCE_RESOURCE_LEN);

	PutTlvBuffer(AUTO_SECURITY_DATA,sTlvBuffer,sSecurData,strlen(sSecurData));

	

	if( OnTimeoutProc && strlen(OnTimeoutProc) > 0 )
		PutTlvBuffer(AUTO_SERV_ROUTING_TIMEOUT_ENTRY,sTlvBuffer,OnTimeoutProc,strlen(OnTimeoutProc));
	if( OnRespProc && strlen(OnRespProc) > 0 )
		PutTlvBuffer(AUTO_SERV_ROUTING_REPLY_ENTRY,sTlvBuffer,OnRespProc,strlen(OnRespProc));

	nRetCode = ROUTAGE;


	trace_event ( "End   ForwardToBackEndIAS()", PROCESSING ); 
	return ( nRetCode );
}
