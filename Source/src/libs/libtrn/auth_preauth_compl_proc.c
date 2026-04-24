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
int AuthPreauthComplProc (	int nIndiceCtx, char *sTlvBuffer)
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
	char			sDstNodeId[NODE_ID_LEN + 1];
	char			szSecurityData[P7_F053_MAX_LEN + 1];
	/*char			szCaptureCode[  6 + 1 ];*/

	trace_event("Start AuthPreauthComplProc ()",PROCESSING);
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
	memset(sDstNodeId,'\0',sizeof(sDstNodeId));
	memset(szSecurityData,'\0',sizeof(szSecurityData));
	/*memset(szCaptureCode,'\0',sizeof(szCaptureCode));*/


	GetTlvBuffer ( AUTO_FUNCTION_CODE        , sTlvBuffer, sFunctionCode   , &nLength );
	GetTlvBuffer ( AUTO_RESPONSE_CODE        , sTlvBuffer, sActionCode   , &nLength );
	GetTlvBuffer ( AUTO_CAPTURE_CODE        , sTlvBuffer, sCaptureCode   , &nLength );
	GetTlvBuffer ( AUTO_CAPTURE_RES_INFO    , sTlvBuffer, sCaptureResInfo   , &nLength );

	GetResInfoData(RES_INF_RES_NODE,sCaptureResInfo,sCaptureNodeId,&nLength);

	PutTlvBuffer( AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT ,     3);
	PutTlvBuffer( AUTO_AUTHO_FLAG, sTlvBuffer, "P" ,  1);
	PutTlvBuffer( AUTO_AUTHOR_SRC, sTlvBuffer, "E1",  2);


	PrintTlvBuffer(sTlvBuffer);

	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if ( CheckAdvTlvFields ( nIndiceCtx, sTlvBuffer ) != OK )
	{
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   AuthPreauthComplProc (NOK)",PROCESSING);
		return(NOK);
	}

	/* PROD00053516 GetAuthRouting replaced by getting routing from original preauthorisation */
/*
#if IS_P7_SWI
        nRetCode = GetAuthRouting(      nIndiceCtx,
                                                        sTlvBuffer,
                                                        sPrimResource,
                                                        sSecResource);
        if (nRetCode != OK)
        {
                nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
                trace_event("End   AuthPreauthComplProc (NOK)",PROCESSING);
                return (NOK);
        }
#endif

	if( strlen(sPrimResource) > 0 )
        {
                PutTlvBuffer ( AUTO_ROUTING_CODE , sTlvBuffer, sPrimResource , RESOURCE_ID_LEN);
        }
*/
	/* PROD00069031: will be done by called pkg, need to load account before calling amount mng
	nRetCode = AuthAmountMng ( nIndiceCtx , sTlvBuffer);
	if ( nRetCode != OK )
	{
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   AuthPreauthComplProc (NOK)",PROCESSING);
		return (  NOK );
	}
	*/

	
	if(	GetTlvBuffer(AUTO_AUTH_NUMBER ,sTlvBuffer,sAuthNum,&nLength) != SUCCES)
		GetAuthNumber ( nIndiceCtx , sTlvBuffer );


	nRetCode = CompletePreauth(nIndiceCtx , sTlvBuffer);
	if ( nRetCode != OK )
	{

		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   AuthPreauthComplProc (NOK)",PROCESSING);
		return (  NOK );
	}

	/*GetTlvBuffer(AUTO_ROUTING_CODE ,sTlvBuffer,sRoutingCode,&nLength);*/

	GetTlvBuffer(AUTO_ROUTING_CODE ,sTlvBuffer,sPrimResource,&nLength);/* PROD00053516 */
	
	PrintTlvBuffer(sTlvBuffer);

	memcpy(sDstNodeId,NodeId,4);

	if ( memcmp ( sPrimResource /*sRoutingCode*/ , STAND_IN_RESOURCE , 6 ) != 0 )
	{
		nRetCode = GetResourceNode(nIndiceCtx,sPrimResource /*sRoutingCode*/,sDstNodeId);
		if( nRetCode != OK )
		{
			sprintf(sLine,"Error on get node id [%s]",/*sRoutingCode*/ sPrimResource);
			trace_event(sLine, ERROR);
		}
	}

	GetTlvBuffer(AUTO_SECURITY_DATA,sTlvBuffer,szSecurityData,&nLength);
	PutSecurityData(SOURCE_RESOURCE, szSecurityData, sCaptureCode /*szCaptureCode*/, SOURCE_RESOURCE_LEN);
	PutSecurityData(SOURCE_NODE_ID, szSecurityData, NodeId, NODE_ID_LEN);
	PutSecurityData(DESTINATION_RESOURCE, szSecurityData, sPrimResource /*sRoutingCode*/, DESTINATION_RESOURCE_LEN);
	PutSecurityData(DESTINATION_NODE_ID, szSecurityData, sDstNodeId, NODE_ID_LEN);


	PutTlvBuffer(AUTO_SECURITY_DATA ,sTlvBuffer,szSecurityData ,strlen(szSecurityData));
	PutTlvBuffer( AUTO_EVENT_CODE, sTlvBuffer, P7_F039_APPROVAL , 3);
	InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );

	if( nRetCode == OK )
	{
		UpdateSettlement( nIndiceCtx , sTlvBuffer);

#if IS_P7_SWI
		if ( memcmp ( sPrimResource /*sRoutingCode*/ , STAND_IN_RESOURCE , 6 ) != 0 &&
			memcmp(sCaptureNodeId, NodeId, NODE_ID_LEN) == 0 )
		{

			InsertionSaf ( nIndiceCtx , sTlvBuffer, sCaptureCode /*szCaptureCode*/ , sPrimResource /*sRoutingCode*/ );

		}
#endif

	}

	trace_event("End   AuthPreauthComplProc (OK)",PROCESSING);
	return ( OK );
}


