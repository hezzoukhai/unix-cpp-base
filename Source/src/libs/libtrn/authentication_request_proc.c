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
int AuthenticationRequestProc(int nIndiceCtx, char*	sTlvBuffer)
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
	char			sExternalRoutingInfo	[RES_INFO_LEN + 1];
	char			sRoutingInfo_1			[RES_INFO_LEN + 1];
	char			sRoutingInfo_2			[RES_INFO_LEN + 1];
	pid_t			nPid;
	char			sResType[4 + 1];


	trace_event("Start AuthenticationRequestProc()", PROCESSING);
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
	memset(sExternalRoutingInfo, 0, sizeof (sExternalRoutingInfo));
	memset(sRoutingInfo_1, 0, sizeof (sRoutingInfo_1));
	memset(sRoutingInfo_2, 0, sizeof (sRoutingInfo_2));
	memset(sResType, 0, sizeof (sResType));
	
	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if (CheckReqTlvFields(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocalAdm(nIndiceCtx, sTlvBuffer));


	GetTlvBuffer(AUTO_FUNCTION_CODE, sTlvBuffer, slFuncCode, &nLength);
	GetTlvBuffer(AUTO_PAN, sTlvBuffer, slCardNumber, &nLength);
	GetTlvBuffer(AUTO_PROCESSING_CODE, sTlvBuffer, slProcCode, &nLength);
	GetTlvBuffer(AUTO_MSG_TYPE, sTlvBuffer, rService.szMti, &nLength);
	GetTlvBuffer(AUTO_CAPTURE_CODE, sTlvBuffer, szCaptureCode, &nLength);

	
	strcpy(sOnlineOffline, "OFF");

	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);
	memcpy(sPrimResource,STAND_IN_RESOURCE, RESOURCE_ID_LEN);
	memcpy(sSecResource,NO_ROUTING_RESOURCE, RESOURCE_ID_LEN);
	memcpy(sExternalResource, STAND_IN_RESOURCE, RESOURCE_ID_LEN);


	PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sPrimResource, 6);


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
		sprintf(sLine, "Err CheckResourceServices, EventCode= [%d] ", nEventCode); 
		trace_event(sLine, PROCESSING);
		sprintf(sEventCode, "%03d", nEventCode);
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode, 3);
		return (AuthLocal(nIndiceCtx, sTlvBuffer));
	}


	/******************************************************************************/
	/* 	    Lecture des flags de securite                                         */
	/******************************************************************************/
	if (GetSecurityFlags(nIndiceCtx, sTlvBuffer, sOnlineOffline) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));

	/******************************************************************************/
	/* 	    PreControl Procedure                                                  */
	/******************************************************************************/

	nRetCode = PrintTlvBuffer ( sTlvBuffer );
	if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
	{
		nRetCode = PrintTlvBuffer ( sTlvBuffer );
		return (AuthLocal(nIndiceCtx, sTlvBuffer));
	}

	/******************************************************************************/
	/* 	    Security Verfication                                                  */
	/******************************************************************************/
	if (CheckAuthSecurity(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthLocal(nIndiceCtx, sTlvBuffer));


	PrintTlvBuffer(sTlvBuffer);

	PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, APPROVED_ACTION, 3);

	nRetCode = GetAuthNumber(nIndiceCtx, sTlvBuffer);
	nRetCode = AuthLocal(nIndiceCtx, sTlvBuffer);



	memset(slCardNumber, 0, sizeof(slCardNumber));
	memset(slCardNumber, 0, sizeof(slCardNumber));
	memset(slProcCode, 0, sizeof(slProcCode));
	memset(slFuncCode, 0, sizeof(slFuncCode));
	memset((char *)&rService, 0, sizeof(rService));
	memset(sEventCode, 0, sizeof(sEventCode));
	memset(sPrimResource, 0, sizeof(sPrimResource));
	


	trace_event("End AuthenticationRequestProc(OK)", PROCESSING);
	return(nRetCode);
}
