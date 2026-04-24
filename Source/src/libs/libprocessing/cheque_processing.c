/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*                                                                             */
/*                                                                             */
/* Description                                                                 */
/*                                                                             */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		18-02-2002   		Mahjoub KARROUM    		Initial Version
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
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

/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les demandes d'autorisations                         */
/****************************************************************************/
/****************************************************************************/
int ChequeReqProcessing(int nIndiceCtx,
	char *sMyResource,
	char *sAuthoTable,
	char *sAcquirerBank,
	char *InternationalRoutCheck,
	char *sServGatewayFlag,
	char *sBillCalculFlag,
	char *sTlvBuffer, TSIsoInfo *sIsoMsg)
{

	TSResources     sResource;
	TSTlvAutho      sTlvInfo;
	char            sSecurData[LG_MAX];
	char   		    sSecurityResults[11];
	char            tmp_buffer[LG_MAX];
	TSService		rService;
	char			slCardNumber[22 + 1];
	char			slProcCode[6 + 1];
	char			slFuncCode[3 + 1];
	char			sLine[MAX_LINE_TRC];
	int     		nLength;
	int 			nEventCode = OK;
	int 			nRetCode;
	char 			sRetCode[4];
	char 			sEventCode[3 + 1];
	char 			sPrimResource[6 + 1];
	char 			sSecResource[6 + 1];
	/*long 		nPid;*/
	pid_t   nPid; /**MKB160408*/
	char 			sPid[8 + 1];
	char 			sInternalStan[6 + 1];
	char 			cOnOffMode;
	char    		sAlternAmount[LG_MAX + 1];

	trace_event("Start ChequeReqProcessing  ()", PROCESSING);

	/******************************************************************************/
	/* 					Initialisation			*/
	/******************************************************************************/

	memset(sSecurData, 0, sizeof(sSecurData));
	memset(slCardNumber, 0, sizeof(slCardNumber));
	memset(slProcCode, 0, sizeof(slProcCode));
	memset(slFuncCode, 0, sizeof(slFuncCode));
	memset((char *)&rService, 0, sizeof(rService));
	memset(sEventCode, 0, sizeof(sEventCode));
	memset(sPrimResource, 0, sizeof(sPrimResource));
	memset(sSecResource, 0, sizeof(sSecResource));
	memset(sPid, 0, sizeof(sPid));
	memset(sSecurityResults, 0, sizeof (sSecurityResults));
	memset(sInternalStan, 0, sizeof(sInternalStan));
	memset(sAlternAmount, 0, sizeof (sAlternAmount));


	/******************************************************************************/
	/*  Formatter le buffer TLV A partir du message Iso                           */
	/******************************************************************************/
	InitTlvAutho(&sTlvInfo);
	memset(sTlvBuffer, 0, sizeof (sTlvBuffer));
	nRetCode = IsoToTlv(nIndiceCtx, sIsoMsg, &sTlvInfo);
	if (nRetCode != OK)
	{
		trace_event("IsoToTlv Error ...", ERROR);
		InsertIsoField(ISO_RESPONSE_CODE, sIsoMsg, MALFUNCTION_ACTION, 3);
		return (NOK);
	}

	PutTlvAutho(AUTO_CUR_TABLE_INDICATOR, &sTlvInfo, sAuthoTable, 3);
	PutTlvAutho(AUTO_CAPTURE_CODE, &sTlvInfo, sMyResource, 6);
	PutTlvAutho(AUTO_ACQUIRER_BANK, &sTlvInfo, sAcquirerBank, 6);
	PutTlvAutho(AUTO_INTER_ROUT_CHECK, &sTlvInfo, InternationalRoutCheck, 1);
	AuthoBuildTlv(sTlvBuffer, &sTlvInfo);

	/******************************************************************************/
	/* 				          Controle du service /r ressource   */
	/******************************************************************************/
	GetIsoField(ISO_PROC_CODE, sIsoMsg, slProcCode, &nLength);
	GetIsoField(ISO_FUNCTION_CODE, sIsoMsg, slFuncCode, &nLength);
	sprintf(rService.szMti, "%04d", sIsoMsg->nMsgType);

	nLength = 0;
	memcpy(rService.szProcCode, slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szSrcAcc, slProcCode + nLength, 2); nLength += 2;
	memcpy(rService.szDestAcc, slProcCode + nLength, 2);
	memcpy(rService.szFctCode, slFuncCode, 3);

	nEventCode = CheckResourceServices(nIndiceCtx, sMyResource, &rService);
	if (nEventCode != OK)
	{
		sprintf(sEventCode, "%03d", nEventCode);
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode, 3);
		return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
			sMyResource, sPrimResource, 'N'));
	}

	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if (CheckReqTlvFields(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
		sMyResource, sPrimResource, 'N'));

	/******************************************************************************/
	/* 		Load Issuer Data Info Including Routing Info                    */
	/******************************************************************************/
	/* trace_event (  sTlvBuffer , PROCESSING );  PCI*/
	trace_event("AVANT Load Issuer Data", PROCESSING);
	PrintTlvBuffer(sTlvBuffer);

	nRetCode = LoadIssuerData(	nIndiceCtx,
		sTlvBuffer,
		InternationalRoutCheck,
		sPrimResource,
		sSecResource
		);

	sprintf(sLine, "Primary Resource    ...%.6s ", sPrimResource);
	trace_event(sLine, PROCESSING);
	sprintf(sLine, "Secondarry Resource ...%.6s ", sSecResource);
	trace_event(sLine, PROCESSING);
	if (nRetCode != OK)
		return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
		sMyResource, sPrimResource, 'N'));

	/******************************************************************************/
	/******************************************************************************/
	/* 				          le Traitement StandIn                               */
	/******************************************************************************/
	/******************************************************************************/

	if (memcmp(sPrimResource, STAND_IN_RESOURCE, 6) == 0)
	{
		trace_event("Prm : STAND_IN AUTHORISATION PROCESSING", PROCESSING);
		/******************************************************************************/
		/* 	    Lecture des flags de securite                                         */
		/******************************************************************************/
		if (GetSecurityFlags(nIndiceCtx, sTlvBuffer, "OFF") != OK)
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/* 	    Controle limite de demande de cheque                                  */
		/******************************************************************************/
		if (Cheque_Control(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
			sMyResource, sPrimResource, 'N'));
		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if (CheckSecurity(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
			sMyResource, sPrimResource, 'N'));

		nRetCode = Cheque_Inquiry(nIndiceCtx, sTlvBuffer);
		sprintf(sRetCode, "%03d", nRetCode);
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sRetCode, 3);

		return (AuthorLocalAdm(nIndiceCtx, sTlvBuffer, sIsoMsg,
			sMyResource, sPrimResource, 'N'));
	}


	/******************************************************************************/
	/* Check If Routing is National Or International                              */
	/******************************************************************************/

	if (GetTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode, &nLength) != SUCCES)
		return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
		sMyResource, sPrimResource, 'N'));
	/******************************************************************************/
	/* PreControl To Run Before Sending  Request to HOST/BANK                     */
	/******************************************************************************/

	if (memcmp(sEventCode, INTERNATIONAL_ROUTING_EVENT, 3) != 0)
	{
		trace_event("PreControl To Run Before Send  AUTHORISATION PROCESSING", PROCESSING);
		/******************************************************************************/
		/* 	    Lecture des flags de securite                                         */
		/******************************************************************************/
		if (GetSecurityFlags(nIndiceCtx, sTlvBuffer, "ON") != OK)
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if (CheckSecurity(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
			sMyResource, sPrimResource, 'N'));

	}

	nRetCode = CheckResource(nIndiceCtx, sPrimResource, &nPid);
	if ((nRetCode != OK) || (kill(nPid, 0) != 0))
	{
		trace_event("Primary Ressource Down ", PROCESSING);
		if (memcmp(sSecResource, NO_ROUTING_RESOURCE, 6) == 0)
		{
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg, sMyResource, sPrimResource, 'N'));
		}
		if (memcmp(sSecResource, STAND_IN_RESOURCE, 6) == 0)
		{
			trace_event("Sec : STAND_IN AUTHORISATION PROCESSING", PROCESSING);
			/******************************************************************************/
			/* 	    Lecture des flags de securite                                         */
			/******************************************************************************/
			if (GetSecurityFlags(nIndiceCtx, sTlvBuffer, "OFF") != OK)
				return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
				sMyResource, sPrimResource, 'N'));

			/******************************************************************************/
			/* 	    PreControl Procedure                                                  */
			/******************************************************************************/
			if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
				return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
				sMyResource, sPrimResource, 'N'));

			/******************************************************************************/
			/* 	    Security Verfication                                                  */
			/******************************************************************************/
			if (CheckSecurity(nIndiceCtx, sTlvBuffer) != OK)
				return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
				sMyResource, sPrimResource, 'N'));

			nRetCode = Cheque_Inquiry(nIndiceCtx, sTlvBuffer);
			sprintf(sRetCode, "%03d", nRetCode);
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sRetCode, 3);
			return (AuthorLocalAdm(nIndiceCtx, sTlvBuffer, sIsoMsg, sMyResource, sPrimResource, 'N'));
		}

		nRetCode = CheckResource(nIndiceCtx, sSecResource, &nPid);
		if (nRetCode != OK)
		{
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg, sMyResource, sPrimResource, 'N'));
		}
		return(AuthorRouting(nIndiceCtx, sTlvBuffer, sSecResource, nPid, sIsoMsg));
	}

	trace_event("Primary Ressource Up ", PROCESSING);
	return(AuthorRouting(nIndiceCtx, sTlvBuffer, sPrimResource, nPid, sIsoMsg));

	return (OK);
}

