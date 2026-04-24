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
 0002YK             16-10-2002          Youssef KHALOUFI        add LoanRequestProcessing
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
extern char  ResourceId[6 + 1];

/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les demandes d'autorisations                         */
/****************************************************************************/
/****************************************************************************/
int LoanRequestProcessing(int nIndiceCtx,
	char *sMyResource,
	char *sAuthoTable,
	char *sAcquirerBank,
	char *InternationalRoutCheck,
	char *sServGatewayFlag,
	char *sBillCalculFlag,
	char *sTlvBuffer,
	TSIsoInfo *IsoInfo)
{

	TSResources     sResource;
	TSTlvAutho      sTlvInfo;
	char            sSecurData[LG_MAX];
	char   		sSecurityResults[11];
	char            tmp_buffer[LG_MAX];
	TSService	rService;
	char		slCardNumber[22 + 1];
	char		slProcCode[6 + 1];
	char		slFuncCode[3 + 1];
	char		sLine[MAX_LINE_TRC];
	int     	nLength;
	int 		nEventCode = OK;
	int 		nRetCode;
	char 		sEventCode[3 + 1];
	char 		sPrimResource[6 + 1];
	char 		sSecResource[6 + 1];
	/*long 		nPid;*/
	pid_t   nPid; /**MKB160408*/
	char 		sPid[8 + 1];
	char 		sInternalStan[6 + 1];
	char 		cOnOffMode;
	char    	sAlternAmount[LG_MAX + 1];
	char    	sTlvProcCode[LG_MAX + 1];
	char    	sIsoProcCode[LG_MAX + 1];

	trace_event("Start LoanRequestProcessing()", PROCESSING);
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
	nRetCode = IsoToTlv(nIndiceCtx, IsoInfo, &sTlvInfo);
	if (nRetCode != OK)
	{
		trace_event("IsoToTlv Error ...", ERROR);
		InsertIsoField(ISO_RESPONSE_CODE, IsoInfo, MALFUNCTION_ACTION, 3);
		return (NOK);
	}

	PutTlvAutho(AUTO_CUR_TABLE_INDICATOR, &sTlvInfo, sAuthoTable, 3);
	PutTlvAutho(AUTO_CAPTURE_CODE, &sTlvInfo, sMyResource, 6);
	PutTlvAutho(AUTO_ACQUIRER_BANK, &sTlvInfo, sAcquirerBank, 6);
	PutTlvAutho(AUTO_INTER_ROUT_CHECK, &sTlvInfo, InternationalRoutCheck, 1);
	AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
	PrintTlvBuffer(sTlvBuffer);

	/******************************************************************************/
	/*  Recherche si Authorisation est Duplicate                                  */
	/******************************************************************************/
	nRetCode = GetOriginalDataElements(nIndiceCtx, CAPTURE_MODE,
		sTlvBuffer,
		sPrimResource);
	if (nRetCode == OK)
	{
		trace_event("Found a Duplicate Request. ", PROCESSING);
		/**Begin MSA03042008*/
		/* In case of Duplicate, over-write ACTION_CODE & AUTHORIZATION_CODE */
		/*PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, DUPLICATE_AUTHO, 3);*/ /*EBE140219*/
		PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, "308", 3);
		PutTlvBuffer(AUTO_AUTH_NUMBER, sTlvBuffer, "      ", 6);
		/**END MSA03042008**/
		InitTlvAutho(&sTlvInfo);
		AnalyseTlvAutho(sTlvBuffer, &sTlvInfo);
		TlvToIso(nIndiceCtx, &sTlvInfo, IsoInfo, RESPONSE);
		return (OK);
	}

	/******************************************************************************/
	/* 	    Controle des champs  TLV                                              */
	/******************************************************************************/
	if (CheckReqTlvFields(nIndiceCtx, sTlvBuffer) != OK)
		return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
		sMyResource, sPrimResource, 'S'));

	/******************************************************************************/
	/* 		Load Issuer Data Info Including Routing Info                    */
	/******************************************************************************/


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
		return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
		sMyResource, sPrimResource, 'N'));
	/****************************************************************************/
	/* Cette Modif Etait Faite Pour Europay Pour Changer Le processing Code
	   Des Cartes Mestro En cas de Cash Advance Voir PCRD_P7_ROUTING */
	memset(sTlvProcCode, 0, sizeof(sTlvProcCode));
	memset(sIsoProcCode, 0, sizeof(sIsoProcCode));
	if (GetTlvBuffer(AUTO_PROCESSING_CODE, sTlvBuffer, sTlvProcCode, &nLength) == SUCCES)
	{
		if (GetIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, &nLength) == SUCCES)
		{
			sprintf(sLine, " PROCESSING CODE TLV %s", sTlvProcCode);
			trace_event(sLine, TRACE);

			sprintf(sLine, " PROCESSING CODE ISO %s", sIsoProcCode);
			trace_event(sLine, TRACE);
			memcpy(sIsoProcCode, sTlvProcCode + 6, 2);
			sprintf(sLine, " PROCESSING CODE ISO %s", sIsoProcCode);
			trace_event(sLine, TRACE);
			PutIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, 6);
		}
	}

	/****************************************************************************/

	/******************************************************************************/
	/* 				          Controle du service /r ressource   */
	/******************************************************************************/
	GetIsoField(ISO_CARD_NBR, IsoInfo, slCardNumber, &nLength);
	GetIsoField(ISO_PROC_CODE, IsoInfo, slProcCode, &nLength);
	GetIsoField(ISO_FUNCTION_CODE, IsoInfo, slFuncCode, &nLength);
	sprintf(rService.szMti, "%04d", IsoInfo->nMsgType);

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
		return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));
	}


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
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if (CheckSecurity(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/*  Amount Bill/Settlement Management                                         */
		/******************************************************************************/
		nRetCode = AmountMng(nIndiceCtx,
							sServGatewayFlag,
							sBillCalculFlag,
							sTlvBuffer
							);
		if (nRetCode != OK)
		{
			trace_event("Retour AmountMng NOK ", PROCESSING);
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
				sMyResource, sPrimResource, 'N'));
		}

		PrintTlvBuffer(sTlvBuffer);
		/******************************************************************************/
		/* 	    Check Installment Amount                                                          */
		/******************************************************************************/

		if (CheckInstallmentAmount(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/* 	    Check Loan Account                                                         */
		/******************************************************************************/
		if (CheckAccLoanAvailable(nIndiceCtx, sTlvBuffer) != OK)
		{
			/* PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,MALFUNCTION_EVENT,3); */
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
				sMyResource, sPrimResource, 'N'));
		}


		return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));
	}



	/******************************************************************************/
	/* Check If Routing is National Or International                              */
	/******************************************************************************/

	if (GetTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sEventCode, &nLength) != SUCCES)
		return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
		sMyResource, sPrimResource, 'N'));
	/******************************************************************************/
	/* PreControl To Run Before Sending  Request to HOST/BANK                     */
	/******************************************************************************/

	if (memcmp(sEventCode, INTERNATIONAL_ROUTING_EVENT, 3) != 0)
	{
		/******************************************************************************/
		/* 	    Lecture des flags de securite                                         */
		/******************************************************************************/
		if (GetSecurityFlags(nIndiceCtx, sTlvBuffer, "ON") != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if (CheckSecurity(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/*  Amount Bill/Settlement Management                                         */
		/******************************************************************************/
		PrintTlvBuffer(sTlvBuffer);
		nRetCode = AmountMng(nIndiceCtx,
			sServGatewayFlag,
			sBillCalculFlag,
			sTlvBuffer
			);
		if (nRetCode != OK)
		{
			trace_event("Retour AmountMng NOK ", PROCESSING);
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
				sMyResource, sPrimResource, 'N'));
		}

		PrintTlvBuffer(sTlvBuffer);
		/******************************************************************************/
		/* 	    Check Installment Amount                                                          */
		/******************************************************************************/

		if (CheckInstallmentAmount(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));

	}
	else
	{
		/******************************************************************************/
		/*  Amount Bill/Settlement Management International                           */
		/******************************************************************************/
		nRetCode = AmountMng(nIndiceCtx,
			sServGatewayFlag,
			sBillCalculFlag,
			sTlvBuffer
			);
		if (nRetCode != OK)
		{
			trace_event("Retour AmountMng NOK ", PROCESSING);
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
				sMyResource, sPrimResource, 'N'));
		}

		PrintTlvBuffer(sTlvBuffer);

	}

	nRetCode = CheckResource(nIndiceCtx, sPrimResource, &nPid);
	if ((nRetCode == OK) && (kill(nPid, 0) == 0 || !memcmp(ResourceId, VRS_RESOURCE, 6)))
	{
		trace_event("Primary Ressource Up ", PROCESSING);
		if (CardInSaf(nIndiceCtx, slCardNumber) != OK)
		{
			return(AuthorRouting(nIndiceCtx, sTlvBuffer,
				sPrimResource, nPid, IsoInfo));
		}
		else
		{
			/*MKB060309 PCI Norme Integration */
			if (debug_option == 1)
				sprintf(sLine, "Card [%s] Found In Saf File ", slCardNumber);
			else
				sprintf(sLine, "Card Found In Saf File ");
			trace_event(sLine, PROCESSING);
		}
	}
	else
		trace_event("Primary Ressource Down ", PROCESSING);


	if (memcmp(sSecResource, NO_ROUTING_RESOURCE, 6) == 0)
	{
		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, ISSUER_NOT_AVAILABLE_EVENT, 3);
		return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'N'));
	}

	if (memcmp(sSecResource, STAND_IN_RESOURCE, 6) == 0)
	{
		trace_event("Sec : STAND_IN AUTHORISATION PROCESSING", PROCESSING);
		/******************************************************************************/
		/* 	    Lecture des flags de securite                                         */
		/******************************************************************************/
		if (GetSecurityFlags(nIndiceCtx, sTlvBuffer, "OFF") != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'S'));

		/******************************************************************************/
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'S'));

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if (CheckSecurity(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'S'));
		/******************************************************************************/
		/* 	    Check Installment Amount                                                          */
		/******************************************************************************/

		if (CheckInstallmentAmount(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'S'));

		/******************************************************************************/
		/* 	    Check Loan Account                                                         */
		/******************************************************************************/
		if (CheckAccLoanAvailable(nIndiceCtx, sTlvBuffer) != OK)
		{
			/* PutTlvBuffer(AUTO_EVENT_CODE,sTlvBuffer,MALFUNCTION_EVENT,3); */
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
				sMyResource, sPrimResource, 'S'));
		}


		return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
			sMyResource, sPrimResource, 'S'));
	}

	else
	{
		nRetCode = CheckResource(nIndiceCtx, sSecResource, &nPid);
		if (nRetCode != OK)
		{
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer,
				ISSUER_NOT_AVAILABLE_EVENT, 3);
			return (AuthorLoanLocal(nIndiceCtx, sTlvBuffer, IsoInfo,
				sMyResource, sPrimResource, 'N'));
		}
		return(AuthorRouting(nIndiceCtx, sTlvBuffer,
			sSecResource, nPid, IsoInfo));
	}
	return (OK);
}


int LoanReplyProcessing(int nIndiceCtx, TSIsoInfo *IsoInfo, char *sTlvBuffer)
{
	int                         nRetCode;
	int                         length;
	char 						sActionCode[3 + 1];
	TSTlvAutho                  sTlvInfo;
	TSTlvPrivate                sTlvPrivateInfo;
	char  tmpField[LG_MAX];
	char  sCaptureCode[6 + 1];
	char  sRoutingCode[6 + 1];
	char  TableIndicator[3 + 1];
	int   nLength;

	trace_event("Start LoanReplyProcessing()", PROCESSING);
	AnalyseTlvAutho(sTlvBuffer, &sTlvInfo);

	PrintTlvBuffer(sTlvBuffer);

	GetTlvAutho(AUTO_CAPTURE_CODE, &sTlvInfo, sCaptureCode, &nLength);
	GetTlvAutho(AUTO_ROUTING_CODE, &sTlvInfo, sRoutingCode, &nLength);
	GetTlvAutho(AUTO_CUR_TABLE_INDICATOR, &sTlvInfo, TableIndicator, &nLength);
	GetTlvAutho(AUTO_RESPONSE_CODE, &sTlvInfo, sActionCode, &nLength);

	dump_p7_struct(IsoInfo);

	nRetCode = IsoToTlv(nIndiceCtx, IsoInfo, &sTlvInfo);
	if (nRetCode != OK)
	{
		trace_event("IsoToTlv Error ...", ERROR);
		if (memcmp(sActionCode, "000", 3) == 0)
			AutomaticReversalProcessing(nIndiceCtx, CAPTURE_MODE,
			sCaptureCode, sRoutingCode,
			TableIndicator, IsoInfo,
			"4002");
		InsertIsoField(ISO_RESPONSE_CODE, IsoInfo, MALFUNCTION_ACTION, 3);
		return (NOK);
	}
	trace_event("Fin IsoToTlv  ...", ERROR);

	PutTlvAutho(AUTO_AUTHO_FLAG, &sTlvInfo, "P", 1);
	PutTlvAutho(AUTO_AUTHOR_SRC, &sTlvInfo, "E0", 2);
	length = AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
	PrintTlvBuffer(sTlvBuffer);

	nRetCode = GetActionCode(nIndiceCtx, sTlvBuffer, sActionCode);
	if (nRetCode != OK)
		PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, MALFUNCTION_ACTION, 3);
	else
		PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);

	PrintTlvBuffer(sTlvBuffer);
	nRetCode = UpdateAuthoActivity(nIndiceCtx, sTlvBuffer);
	if (nRetCode != OK)
	{
		if (memcmp(sActionCode, "000", 3) == 0)
			AutomaticReversalProcessing(nIndiceCtx, CAPTURE_MODE,
			sCaptureCode, sRoutingCode,
			TableIndicator, IsoInfo,
			"4002");

		PutTlvBuffer(AUTO_CUR_TABLE_INDICATOR, sTlvBuffer, "SAF", 3);
		InsertionAuthoActivity(nIndiceCtx, sTlvBuffer);

		PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, MALFUNCTION_EVENT, 3);
		PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, MALFUNCTION_ACTION, 3);
		nRetCode = GetActionCode(nIndiceCtx, sTlvBuffer, sActionCode);
		if (nRetCode != OK)
			PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, MALFUNCTION_ACTION, 3);
		else
			PutTlvBuffer(AUTO_RESPONSE_CODE, sTlvBuffer, sActionCode, 3);

		AnalyseTlvAutho(sTlvBuffer, &sTlvInfo);
		TlvToIso(nIndiceCtx, &sTlvInfo, IsoInfo, RESPONSE);
		trace_event("SEVERE ERROR !!!!!", PROCESSING);
		trace_event("Can Not Update Autho Activity File ", PROCESSING);
		return (NOK);
	}

	if (memcmp(sActionCode, APPROVED_ACTION, 3) != 0)
	{
		if (memcmp(sActionCode, INVALID_PIN_ACTION, 3) != 0)
			nRetCode = UpdateCardActivity(nIndiceCtx, DECLINED_AUTH, sTlvBuffer);
		else
			nRetCode = UpdateCardActivity(nIndiceCtx, PIN_ERROR, sTlvBuffer);
	}
	else
	{
		nRetCode = UpdateCardActivity(nIndiceCtx, APPROVED_AUTH, sTlvBuffer);
		if (nRetCode != OK)
		{
			trace_event("Can Not Update Card Activity  !!!", FATAL);
			/* What is the action to do */
		}

		nRetCode = SetLoanDataToPos(nIndiceCtx, sTlvBuffer);
		if (nRetCode != OK)
		{
			trace_event("Can Not Set Data to pos  !!!", FATAL);
			/* What is the action to do */
		}
		nRetCode = UpdateLoan(nIndiceCtx, APPROVED_AUTH, sTlvBuffer);
		if (nRetCode != OK)
		{
			trace_event("Can Not Update Loan  !!!", FATAL);
			/* What is the action to do */
		}


	}

	StopListReplyProcessing(nIndiceCtx, sTlvBuffer);

	AnalyseTlvAutho(sTlvBuffer, &sTlvInfo);
	TlvToIso(nIndiceCtx, &sTlvInfo, IsoInfo, RESPONSE);
	trace_event("End LoanReplyProcessing()", PROCESSING);
	return (OK);
}
/****************************************************************************/
/****************************************************************************/
/* Fonction qui traite les demandes d'autorisations                         */
/****************************************************************************/
/****************************************************************************/
int LoanInquiryProcessing(int nIndiceCtx,
	char *sMyResource,
	char *sAuthoTable,
	char *sAcquirerBank,
	char *InternationalRoutCheck,
	char *sServGatewayFlag,
	char *sBillCalculFlag,
	char *sTlvBuffer, TSIsoInfo *sIsoMsg,
	char	Check)
{

	TSResources     sResource;
	TSTlvAutho      sTlvInfo;
	char            sSecurData[LG_MAX];
	char   		sSecurityResults[11];
	char            tmp_buffer[LG_MAX];
	TSService	rService;
	char		slCardNumber[22 + 1];
	char		slProcCode[6 + 1];
	char		slFuncCode[3 + 1];
	char		sLine[MAX_LINE_TRC];
	int     	nLength;
	int 		nEventCode = OK;
	int 		nRetCode;
	char 		sRetCode[4];
	char 		sEventCode[3 + 1];
	char 		sPrimResource[6 + 1];
	char 		sSecResource[6 + 1];
	/*long 		nPid;*/
	pid_t   nPid; /**MKB160408*/
	char 		sPid[8 + 1];
	char 		sInternalStan[6 + 1];
	char 		cOnOffMode;
	char    	sAlternAmount[LG_MAX + 1];

	trace_event("Start LoanInquiryProcessing()", PROCESSING);

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
	trace_event("AVANT Load Issuer Data", PROCESSING);
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
		/* 	    PreControl Procedure                                                  */
		/******************************************************************************/
		if (PreControl(nIndiceCtx, sTlvBuffer) != OK)
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
			sMyResource, sPrimResource, 'N'));

		/******************************************************************************/
		/* 	    Security Verfication                                                  */
		/******************************************************************************/
		if (Check == 'Y') {
			if (CheckSecurity(nIndiceCtx, sTlvBuffer) != OK)
				return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
				sMyResource, sPrimResource, 'N'));
		}

		nRetCode = Get_LoanInquiry(nIndiceCtx, sTlvBuffer);
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
			return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
				sMyResource, sPrimResource, 'N'));
		}

		else
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
			if (Check == 'Y') {
				if (CheckSecurity(nIndiceCtx, sTlvBuffer) != OK)
					return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
					sMyResource, sPrimResource, 'N'));
			}
			nRetCode = Get_LoanInquiry(nIndiceCtx, sTlvBuffer);
			sprintf(sRetCode, "%03d", nRetCode);
			PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer, sRetCode, 3);

			return (AuthorLocalAdm(nIndiceCtx, sTlvBuffer, sIsoMsg,
				sMyResource, sPrimResource, 'N'));
		}
		else
		{
			nRetCode = CheckResource(nIndiceCtx, sSecResource, &nPid);
			if (nRetCode != OK)
			{
				PutTlvBuffer(AUTO_EVENT_CODE, sTlvBuffer,
					ISSUER_NOT_AVAILABLE_EVENT, 3);
				return (AuthorLocal(nIndiceCtx, sTlvBuffer, sIsoMsg,
					sMyResource, sPrimResource, 'N'));
			}
			return(AuthorRouting(nIndiceCtx, sTlvBuffer,
				sSecResource, nPid, sIsoMsg));
		}
	}
	else
	{
		trace_event("Primary Ressource Up ", PROCESSING);
		return(AuthorRouting(nIndiceCtx, sTlvBuffer,
			sPrimResource, nPid, sIsoMsg));
	}
	return (OK);
}
