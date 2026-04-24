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
int BankFileUpdateProcessing(int nIndiceCtx,
	char *sMyResource,
	char *sDestResource,
	TSIsoInfo *sIsoMsg)
{

	TSResources     sResource;
	TSTlvAutho      sTlvInfo;
	TSTlvPrivate    sTlvPrivateInfo;
	char		sLine[MAX_LINE_TRC];
	int     	nLength;
	int 		nEventCode = OK;
	int 		nRetCode;
	int         nDestinationPid;

	char    	sTlvBuffer[LG_MAX + 1];
	char 		sEventCode[3 + 1];
	char    	sAlternAmount[LG_MAX + 1];
	char        sSecurityData[LG_MAX + 1];
	char        sSourcePid[8 + 1];
	char        sDestinationPid[8 + 1];
	char        DeclarationDate[11];
	char        CardNbr[20];
	char        PrivateBuffer[LG_MAX + 1];
	char        UpdateCode[2];
	char        ActionDate[7];
	char        sActionCode[3 + 1];
	char        sMsgErr[255 + 1];
	char        sReasonCode[4 + 1];


	trace_event("Start BankFileUpdateProcessing()", PROCESSING);
	/******************************************************************************/
	/* 					Initialisation			*/
	/******************************************************************************/
	InitTlvAutho(&sTlvInfo);
	memset(sTlvBuffer, 0, sizeof (sTlvBuffer));
	memset(sEventCode, 0, sizeof(sEventCode));
	memset(sActionCode, 0, sizeof(sActionCode));
	memset(sAlternAmount, 0, sizeof(sAlternAmount));
	memset(sSecurityData, 0, sizeof(sSecurityData));
	memset(sSourcePid, 0, sizeof(sSourcePid));
	memset(sDestinationPid, 0, sizeof(sDestinationPid));
	memset(DeclarationDate, 0, sizeof(DeclarationDate));
	memset(CardNbr, 0, sizeof(CardNbr));
	memset(PrivateBuffer, 0, sizeof(PrivateBuffer));
	memset(UpdateCode, 0, sizeof(UpdateCode));
	memset(ActionDate, 0, sizeof(ActionDate));
	memset(sMsgErr, 0, sizeof(sMsgErr));
	memset(sReasonCode, 0, sizeof(sReasonCode));

	/******************************************************************************/
	/*  Formatter le buffer TLV A partir du message Iso                           */
	/******************************************************************************/
	dump_p7_struct(sIsoMsg);

	memset(sReasonCode, 0, sizeof(sReasonCode));
	GetIsoField(ISO_MSG_REASON, sIsoMsg, sReasonCode, &nLength);

	IsoToTlv(nIndiceCtx, sIsoMsg, &sTlvInfo);

	PutTlvAutho(AUTO_CAPTURE_CODE, &sTlvInfo, sMyResource, 6);
	PutTlvAutho(AUTO_ROUTING_CODE, &sTlvInfo, sDestResource, 6);
	PutTlvAutho(AUTO_AUTHO_FLAG, &sTlvInfo, "P", 1);
	PutTlvAutho(AUTO_CUR_TABLE_INDICATOR, &sTlvInfo, "MSC", 3);

	/*EBE140523
	memcpy ( UpdateCode      , 0 , sizeof ( UpdateCode      ));
	memcpy ( DeclarationDate , 0 , sizeof ( DeclarationDate ));
	memcpy ( CardNbr         , 0 , sizeof ( CardNbr         ));
	*/

	GetTlvAutho(AUTO_PAN, &sTlvInfo, CardNbr, &nLength);
	GetTlvAutho(AUTO_TRANS_DATE_TIME, &sTlvInfo, DeclarationDate, &nLength);
	GetTlvAutho(AUTO_UF_ACTION_DATE, &sTlvInfo, ActionDate, &nLength);

	GetTlvAutho(AUTO_NETWORK_POS_DATA, &sTlvInfo, PrivateBuffer, &nLength);
	AnalyseTlvPrivate(PrivateBuffer, nLength, &sTlvPrivateInfo);
	if (GetTlvPrivate(FILE_UPDATE_CODE_TAG, &sTlvPrivateInfo, UpdateCode, &nLength) != SUCCES)
	{
		trace_event("Update Code Not Found", PROCESSING);
		return (NOK);
	}

	switch (atoi(UpdateCode))
	{
	case 1:  /* add new record */
		nRetCode = ProcessStopList(nIndiceCtx,
			'I',
			CardNbr,
			DeclarationDate,
			ActionDate,
			sReasonCode,
			sMsgErr,
			sActionCode
			);
		break;

	case 2:  /* update record */
		nRetCode = ProcessStopList(nIndiceCtx,
			'U',
			CardNbr,
			DeclarationDate,
			ActionDate,
			sReasonCode,
			sMsgErr,
			sActionCode
			);
		break;
	case 3:  /* Delete record */
		nRetCode = ProcessStopList(nIndiceCtx,
			'D',
			CardNbr,
			DeclarationDate,
			ActionDate,
			sReasonCode,
			sMsgErr,
			sActionCode
			);
		break;
	case 4:  /* Consult record */
		nRetCode = ProcessStopList(nIndiceCtx,
			'C',
			CardNbr,
			DeclarationDate,
			ActionDate,
			sReasonCode,
			sMsgErr,
			sActionCode
			);
		break;

	default: /* invalid code */
		memcpy(sActionCode, "306", 3);
		memcpy(sMsgErr, "Invalid Code", 12);
		nRetCode = NOK;
		break;
	}

	PutTlvAutho(AUTO_RESPONSE_CODE, &sTlvInfo, sActionCode, 3);
	PutIsoField(ISO_RESPONSE_CODE, sIsoMsg, sActionCode, 3);

	if (atoi(UpdateCode) == 4
		&& memcmp(ActionDate, "XXXXXX", 6) != 0
		&& memcmp(ActionDate, "YYYYYY", 6) != 0
		)
		PutIsoField(ISO_ACTION_DATE, sIsoMsg, ActionDate, 6);


	AuthoBuildTlv(sTlvBuffer, &sTlvInfo);
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
		return (SEVERE_ERROR);
	}

	return (OK);
}
/****************************************************************************/
