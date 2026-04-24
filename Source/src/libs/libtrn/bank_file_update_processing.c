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
#include <resources.h>
#include <event_auth.h>

/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les demandes d'autorisations                         */
/****************************************************************************/
/****************************************************************************/

int BankFileUpdateProcessingTlv(int nIndiceCtx,
	char *sMyResource,
	char *sDestResource,
	char *sTlvBuffer)
{

	TSResources     sResource;
	TSTlvPrivate    sTlvPrivateInfo;
	char		sLine[MAX_LINE_TRC];
	int     	nLength;
	int 		nEventCode = OK;
	int 		nRetCode;
	int         nDestinationPid;
	char 		sEventCode[3 + 1];
	char        sSourcePid[8 + 1];
	char        sDestinationPid[8 + 1];
	char        PrivateBuffer[2048 + 1];
	char        UpdateCode[2];
	char        ActionDate[7];
	char        sActionCode[3 + 1];
	char        sMsgErr[255 + 1];
	char        sWork[256];


	trace_event("Start BankFileUpdateProcessing()", PROCESSING);
	/******************************************************************************/
	/* 					Initialisation			*/
	/******************************************************************************/

	memset(sEventCode, 0, sizeof(sEventCode));
	memset(sActionCode, 0, sizeof(sActionCode));
	memset(sSourcePid, 0, sizeof(sSourcePid));
	memset(sDestinationPid, 0, sizeof(sDestinationPid));
	memset(PrivateBuffer, 0, sizeof(PrivateBuffer));
	memset(UpdateCode, 0, sizeof(UpdateCode));
	memset(ActionDate, 0, sizeof(ActionDate));
	memset(sMsgErr, 0, sizeof(sMsgErr));
	memset(sWork, 0, sizeof(sWork));/*SNO061015*/

/*ICH20170209
	nRetCode = GetAuthRouting(	nIndiceCtx,
		sTlvBuffer,
		sMyResource,
		sMyResource);
	if (nRetCode != OK)
	{
		nRetCode = InsertionAuthoActivity ( nIndiceCtx , sTlvBuffer );
		trace_event("End   AuthAdviceProc (NOK)",PROCESSING);
		return (  NOK );
	}

	
	PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, sDestResource, 6);*/

	PutTlvBuffer(AUTO_CAPTURE_CODE, sTlvBuffer, sMyResource, 6);
	PutTlvBuffer(AUTO_ROUTING_CODE, sTlvBuffer, "000000", 6);

	PutTlvBuffer(AUTO_AUTHO_FLAG, sTlvBuffer, "P", 1);
	PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR, sTlvBuffer, "MSC", 3);


	PrintTlvBuffer (  sTlvBuffer );
	nRetCode = ProcessStopList(nIndiceCtx,
		sTlvBuffer,
		sMsgErr,
		sActionCode);
	if (nRetCode != OK)
	{
		trace_event("ProcessStopList NOK", FATAL);

		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);
		nRetCode = GetActionCode(nIndiceCtx, sTlvBuffer, sActionCode);
		if (nRetCode != OK)
			PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, MALFUNCTION_ACTION, 3);
		else
			PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);

		trace_event("End BankFileUpdateProcessing(SEVERE_ERROR)", PROCESSING);
		return (SEVERE_ERROR);
	}

	PutTlvBuffer ( AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);/*ICH20170209*/

	PrintTlvBuffer (  sTlvBuffer );	
	if ( GetTlvBuffer(AUTO_ADD_DATA_PRIVATE ,sTlvBuffer,PrivateBuffer ,&nLength)==SUCCES )
	{
		InitTlvPrivate (&sTlvPrivateInfo);
		AnalyseTlvPrivate(PrivateBuffer, nLength, &sTlvPrivateInfo);
		PutTlvPrivate(EVENT_ACTION ,&sTlvPrivateInfo,sActionCode,3);
		PrivateBuildTlv(PrivateBuffer,&sTlvPrivateInfo);
		PutTlvBuffer(AUTO_ADD_DATA_PRIVATE, sTlvBuffer,PrivateBuffer, strlen(PrivateBuffer));
	}

	if (GetTlvBuffer(AUTO_MSG_TYPE, sTlvBuffer, sWork, &nLength) == OK && memcmp (sWork, "1100", 4 )== OK){
		nRetCode = GenAuthNumber(nIndiceCtx,sWork);
		PutTlvBuffer (AUTO_AUTH_NUMBER, sTlvBuffer, sWork, 6);
		PutTlvBuffer (AUTO_AUTH_NUMBER_LENGTH,sTlvBuffer, "6", 1);
	}
	PutEvent(sTlvBuffer, sActionCode, sMsgErr);
	nRetCode = InsertionAuthoActivity(nIndiceCtx, sTlvBuffer);
	if (nRetCode != OK)
	{
		trace_event("Can Not Write In Authorisation table !!!", FATAL);
		PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR, sTlvBuffer, "SAF", 3);
		nRetCode = InsertionAuthoActivity(nIndiceCtx, sTlvBuffer);
		if (nRetCode != OK)
			trace_event("Can Not Write In Authorisation SAF table !!!", FATAL);

		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);
		nRetCode = GetActionCode(nIndiceCtx, sTlvBuffer, sActionCode);
		if (nRetCode != OK)
			PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, MALFUNCTION_ACTION, 3);
		else
			PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);

		trace_event("End BankFileUpdateProcessing(SEVERE_ERROR)", PROCESSING);
		return (SEVERE_ERROR);
	}

	trace_event("End BankFileUpdateProcessing(OK)", PROCESSING);
	return (OK);
}
/****************************************************************************/
