#include <define.h>
#include <iso_hps.h>
#include <iso_cps.h>
#include <iso_visa.h>
#include <visa_param.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <visa_define.h>
#include <tlv_private.h>
#include <security_data.h>
#include <list_thr.h>
#include <queue.h>
#include <visa_puf.h>
#include <visa_puf_fields.h>
#include <tlv_fields.h>
#include <iso8583.h>
#include <nw_raw_msg.h>

static int             VisaEchoedField_0400[] =
{
	0,				/* BIT_MAP2                       */
	1,				/* CARD_NBR                       */
	1,				/* PROC_CODE                      */
	1,				/* TRANS_AMOUNT                   */
	0,				/* STLMNT_AMOUNT                  */
	0,				/* CARDHOLDER BILLING AMOUNT      */
	1,				/* XMIT_TIME                      */
	0,				/* CARDHOLDER BILLING FEE         */
	0,				/* CONVRSN_RATE_STLMNT            */
	0,				/* CONVRSN_RATE_CARD_HLDR_BILLING */

	1,				/* AUDIT_NBR             */
	0,				/* TRANS_TIME            */
	0,				/* TRANS_DATE            */
	0,				/* EXPIRY_DATE           */
	/*0*/ 1,		/* STLMNT_DATE           *//*NAB04032021 PLUTONL-2813*/
	0,				/* CNVRSN_DATE           */
	0,				/* CAPTURE_DATE          */
	0,				/* MERCHANT_TYPE         */
	1,				/* ACQR_COUNTRY          */
	1,				/* PAN_COUNTRY           */

	0,				/* FORWD_COUNTRY                     */
	0,				/* POS_ENTRY_MODE                    */
	0,				/* CARD SEQUENCE NUMBER              */
	0,				/* NETWORK INTERNATIONAL IDENTIFIER  */
	1,				/* POS_CONDITION                     */
	0,				/* POS_PIN_CAPTURE                   */
	0,				/* AUTHORIZATION ID RESPONSE LENGTH  */
	0,				/* TRANS_FEE                         */
	0,				/* AMOUNT SETTLEMENT FEE             */
	0,				/* AMOUNT TRANSACTION PROCESSING FEE */

	0,				/* AMOUNT SETLMNT PROCESSING FEE     */
	1,				/* ACQR_ID                           */
	0,				/* FORWD_ID                          */
	0,				/* PAN_EXTENDED                      */
	0,				/* T2_DATA                           */
	0,				/* TRACK 3 DATA                      */
	1,				/* REFERENCE_NBR                     */
	0,				/* AUTHOR_ID                         */
	0,				/* RESPONSE_CODE                     */
	0,				/* SERVICE RESTRICTION CODE          */

	1,				/* TERMINAL_NBR                      */
	1,				/* OUTLET_NBR                        */
	0,				/* TERMINAL_ADR                      */
	1,				/* ADTNL_RESP_DATA                   *//* must be echoed *//* OBE20061222 */
	0,				/* T1_DATA                           */
	0,				/* ADDITRIONAL DATA ISO              */
	0,				/* ADDITIONAL DATA NATIONAL          */
	0,				/* ADTNL_DATA_PRIV                   */
	1,				/* TRANS_CRNCY                       */
	0,				/* STLMNT_CRNCY                      */

	0,				/* BILLING_CRNCY         */
	0,				/* PIN_DATA              */
	0,				/* SECURITY_DATA         */
	0,				/* ADTNL_AMOUNTS         */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */
	0,				/* Reserved National     */
	0,				/* Reserved National     */
	0,				/* GEOGR_DATA            */
	0,				/* ADTNL_POS_INFO        */

	0,				/* OTHER_AMOUNTS         */
	1,				/* CPS_FIELDS            *//*OAD130411 ENH April 2011*/
	1,				/* PRIV_USE_FIELDS       *//*SEW280208 ENH April 2008*/
	0,				/* MAC1                  */
	0,				/* */
	0,				/* STLMNT_CODE           */
	0,				/* Extended Payment Code */
	0,				/* RECEIVER_COUNTRY      */
	0,				/* STLMNT_INST_COUNTRY   */
	0,				/* NW_MNGMT_INFO         */

	0,				/* Message Number              */
	0,				/* Message number Last         */
	0,				/* ACTION_DATE                 */
	0,				/* CR_NBR                      */
	0,				/* CR_REV_NBR                  */
	0,				/* DB_NBR                      */
	0,				/* DB_REV_NBR                  */
	0,				/* Transfer Number             */
	0,				/* Transfer Reversal Number    */
	0,				/* Inquiries Number            */

	0,				/* Authorizations Number          */
	0,				/* Credit, Processing Fee Amount  */
	0,				/* Credit, Transaction Fee Amount */
	0,				/* Debit, Processing Fee Amount   */
	0,				/* Debit, Transaction Fee Amount  */
	0,				/* CR_AMOUNT                      */
	0,				/* CR_REV_AMOUNT                  */
	0,				/* DB_AMOUNT                      */
	0,				/* DB_REV_AMOUNT                  */
	0,				/* ORIG_DATA                      */

	0,				/* FILE_UPDATE           */
	0,				/* FILE_SECURITY         */
	0,				/* Response Indicator    */
	0,				/* Service Indicator     */
	1,				/* REPLACE_AMOUNTS       */
	0,				/* MSG_SECURITY          */
	0,				/* NET_AMOUNT            */
	0,				/* Payee                 */
	0,				/* STLMNT_INSTITN        */
	0,				/* RECEIVER_ID           */

	0,				/* FILE_NAME             */
	0,				/* ACCOUNT_ID1           */
	0,				/* ACCOUNT_ID2           */
	0,				/* Transaction Description   */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */
	0,				/* Reserved Iso          */

	0,				/* Reserved Iso          */
	0,				/* Reserved National     */
	0,				/* Reserved National     */
	0,				/* Reserved National     */
	0,				/* ADTNL_TRACE_DATA      */
	0,				/* Reserved National     */
	0,				/* Reserved National     */
	0,				/* Intra Country Data    */
	0,				/* Reserved National     */
	0,				/* Original Message Type Id  */

	0,				/* Issuing Institution Id        */
	0,				/* Remaining Open to Use         */
	0,				/* ADR_VERIF_DATA                */
	0,				/* Free Form Text - Japan        */
	0,				/* SUPPORT_INFO                  */
	0,				/* Reserved Private              */
	0,				/* File Record Action and Data   */
	0,				/* MAC2                          */
	0,				/* Field 129                     */
	0,				/* Terminal Capability Profile   */

	0,				/* Terminal Verification Results */
	0,				/* Unpredictable Number          */
	0,				/* Terminal Serial Number        */
	0,				/* Visa Discretionary Data       */
	0,				/* Issuer Discretionary Data     */
	0,				/* Issuer Discretionary Data     */
	0,				/* Application Trans. Counter    */
	0,				/* Application Inter. Profile    */
	0,				/* ARPC + Response Code          */
	0,				/* Field 140                     */

	0,				/* Field 141                     */
	0,				/* Issuer Script                 */
	0,				/* Issuer Script Results         */
	0,				/* Cryptogram Trans Type         */
	0,				/* Terminal Country Code         */
	0,				/* Terminal Trans  Date          */
	0,				/* Cryptogram Amount             */
	0,				/* Cryptogram Currency Code      */
	0,				/* Cryptogram Cash Back          */
	0,				/* Cardholder Card Scheme Crypto. */

	0,				/* Field 151             */
	0,				/* Field 152             */
	0,				/* Field 153             */
	0,				/* Field 154             */
	0,				/* Field 155             */
	0,				/* Field 156             */
	0,				/* Field 157             */
	0,				/* Field 158             */
	0,				/* Field 159             */
	0,				/* Field 160             */

	0,				/* Field 161             */
	0,				/* Field 162             */
	0,				/* Field 163             */
	0,				/* Field 164             */
	0,				/* Field 165             */
	0,				/* Field 166             */
	0,				/* Field 167             */
	0,				/* Field 168             */
	0,				/* Field 169             */
	0,				/* Field 170             */

	0,				/* Field 171             */
	0,				/* Field 172             */
	0,				/* Field 173             */
	0,				/* Field 174             */
	0,				/* Field 175             */
	0,				/* Field 176             */
	0,				/* Field 177             */
	0,				/* Field 178             */
	0,				/* Field 179             */
	0,				/* Field 180             */

	0,				/* Field 181             */
	0,				/* Field 182             */
	0,				/* Field 183             */
	0,				/* Field 184             */
	0,				/* Field 185             */
	0,				/* Field 186             */
	0,				/* Field 187             */
	0,				/* Field 188             */
	0,				/* Field 189             */
	0,				/* Field 190             */

	0,				/* Field 191             */
	0,				/* Message Authentication Code   */
};


/***************************************************************************/
/* FCT_REV_REQ_TO_NW()                                                    */
/* Sends a reversal request message (0400) to VISA.                       */
/***************************************************************************/
int
	FCT_REV_REQ_TO_NW(int nIndexCtx, TSIsoInfo * IsoInfo, char *sTlvBuffer)
{
	TSBalExchange   BalMessage;
	TSVisaInfo      VisaInfo;
	TSNodeData      Node;
	int             result;
	char            tmpField[256];
	char            sLine[MAX_LINE_TRC];
	int             tmpFieldLen;
	char            DestId[12];
	char            sBuffer[LG_MAX];
	int             Length;
	struct	timeval stPurgeTime;
	char 			sSecurityData [128 + 1];



	trace_event("Start  FCT_REV_REQ_TO_NW(S)", PROCESSING);

	memset(sBuffer, 0, sizeof(sBuffer));
	memset(DestId, 0, sizeof(DestId));
	memset (sSecurityData, 0, sizeof(sSecurityData));


	/* Convert message to VISA format */
	dump_p7_struct(IsoInfo);

	result = ReversalReqToNw(nIndexCtx,IsoInfo, &VisaInfo);
	/*Start ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if(result != OK)
	{
		PutIsoField (ISO_RESPONSE_CODE, IsoInfo, MALFUNCTION_ACTION, P7_F039_LEN);
		trace_message(P7_TL_PROCESSING, "FCT_AUT_REQ_TO_NW(): Convertion failed");
        return (NOK);
	}
	/*End ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	/* Insert original ISO message in list for later retrieval */
	InitNodeData(&Node);
	GetSafPurgeTimeMS(&stPurgeTime);
	Node.stPurgeTime = stPurgeTime;
	memcpy(Node.szFstKey, "1420", 4);

	GetVisaField(VISA_AUDIT_NBR, &VisaInfo, tmpField, &tmpFieldLen);

	memcpy(Node.szFstKey + 4, tmpField, 6);
	memcpy(Node.sSndPrvtData + 0, ResourceId, 6);
	Node.nSndPrvtDataLen = 6;


	memcpy(Node.sFstPrvtData, "SAF", 3);
	Node.nFstPrvtDataLen = 3;

	Length = IsoBuildMsg(Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	Node.nSndBufLen = strlen(sTlvBuffer);
	memcpy(Node.sSndBuf, sTlvBuffer, Node.nSndBufLen);

	if (InsertRequest('B', &Node) != OK)
	{

		sprintf(tmpField, "%.3d", SYSTEM_MALFUNCTION);
		InsertIsoField(ISO_RESPONSE_CODE, IsoInfo, tmpField, 3);
		GetIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen);
		GetSecurityData( DESTINATION_PID, tmpField , DestId, &tmpFieldLen);

		BalMessage.sHeader.nPartnerId = atoi(DestId);
		BalMessage.sHeader.nSourceId = getpid();
		BalMessage.sHeader.nCommand = 1430;
		Length = IsoBuildMsg(BalMessage.sBody, IsoInfo);
		BalMessage.sHeader.nLen = Length;

		WriteBalMsg(nBalMsg, &BalMessage);

		trace_event("FCT_REV_REQ_TO_NW(NOK): InsertRequest() failure.", ERROR);
		return (NOK);
	}


	/* send to network */
	Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, &VisaInfo);
	VisaBuildHeader(REQUEST, &(VisaInfo.sHeader), NULL, Length, StationId);
	memcpy(sBuffer, (char *) &(VisaInfo.sHeader), VISA_HDR_LEN);
	Length += VISA_HDR_LEN;

	/*Start IBO20200527 RAWMSG*/
	if (g_sCaptureMsg[NW_OUTG_REQ] == 'Y')
	{
		LogVisaMessage(nIndexCtx, &VisaInfo, IsoInfo, OUTG_REQ);
	}
	/*End IBO20200527 RAWMSG*/

	DumpVisa(sBuffer);

	if (WriteLineMsg(sBuffer, Length) != OK)
	{

		FetchRequest('B', &Node);
		trace_event("FCT_REV_REQ_TO_NW(NOK): WriteLineMsg() failure.", ERROR);
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}


	trace_event("End  FCT_REV_REQ_TO_NW(OK)", PROCESSING);
	return (OK);
}
/****************************************************************************/
/* FCT_REV_REQ_FROM_NW()													*/
/*																			*/
/* Processes a reversal request (0400) received from VISA.					*/
/****************************************************************************/
int FCT_REV_REQ_FROM_NW(int nIndexCtx, TSVisaInfo * VisaInfo)
{

	TSIsoInfo       IsoInfo;
	int             result;
	char            sPid[8 + 1];
	char			sTlvBuffer[LG_MAX];



	trace_event("Start  FCT_REV_REQ_FROM_NW()", PROCESSING);


	/* Convert message to ISO format */
	result = ReversalReqFromNw(nIndexCtx, VisaInfo, &IsoInfo);

	IsoInfo.msgId = MsgIsoReqReceived(ResourceId,&IsoInfo);

	result = ServiceProcessing(		nIndexCtx,
		ResourceId,
		"ADM",
		AcquirerBank,
		"Y",
		"G",
		"N",
		sTlvBuffer,
		&IsoInfo);

	/*Start IBO20200517 RAWMSG*/
	if (g_sCaptureMsg[NW_INC_REQ] == 'Y')
	{
		LogVisaMessage(nIndexCtx, VisaInfo, &IsoInfo, INC_REQ);
	}
	/*End IBO20200517 RAWMSG*/

	switch (result) {

	case OK:		/* the interface itself responds */
		if (ReversalRepToNw(nIndexCtx, &IsoInfo, VisaInfo) != OK) {
			trace_message (P7_TL_ERROR,"Error when calling ReversalRepToNw(): [IndexCtx:%d]",nIndexCtx);
			trace_message(P7_TL_INFO,"End FCT_REV_REQ_FROM_NW(NOK)");
			return (NOK);
		}
		trace_message(P7_TL_PROCESSING,"End FCT_AUT_ADV_FROM_NW(OK): [nIndexCtx:%d]", nIndexCtx);
		break;
	case NOK:
		if (ReversalRepToNw(nIndexCtx, &IsoInfo, VisaInfo) != OK) {
			trace_message (P7_TL_ERROR,"Error when calling ReversalRepToNw(): [IndexCtx:%d]",nIndexCtx);
			trace_message(P7_TL_INFO,"End FCT_REV_REQ_FROM_NW(NOK)");
			return (NOK);
		}
		trace_message(P7_TL_PROCESSING,"End FCT_AUT_ADV_FROM_NW(NOK): [nIndexCtx:%d]", nIndexCtx);
		break;
	/*PLUTONL-4242 ZKO Start 27012022*/
	case SEVERE_ERROR:
		/*trace_event("Sever error received", ERROR);
		shutdown_resource(nIndexCtx, ResourceId);*/
		if (ReversalRepToNw(nIndexCtx, &IsoInfo, VisaInfo) != OK) {
			trace_message (P7_TL_ERROR,"Error when calling ReversalRepToNw(): [IndexCtx:%d]",nIndexCtx);
			trace_message(P7_TL_INFO,"End FCT_REV_REQ_FROM_NW(NOK)");
			return (NOK);
		}
		trace_message (P7_TL_ERROR,"Error when calling ServiceProcessing(): [IndexCtx:%d, ResourceId:%s]",nIndexCtx, ResourceId);
		trace_message(P7_TL_INFO,"End FCT_REV_REQ_FROM_NW(NOK):SEVERE_ERROR");
		return (NOK);
	/*PLUTONL-4242 ZKO End 27012022*/
	}
	return (OK);
}

/*****************************************************************************/
/*****************************************************************************/
int
	FCT_REV_REP_FROM_NW(int nIndexCtx, TSVisaInfo * VisaInfo)
{
	TSBalExchange   BalMessage;
	TSIsoInfo       IsoInfo;
	TSNodeData      Node;
	char            tmpField[256];
	int             tmpFieldLen;
	char            SourceResource[6 + 1];
	char            RespCode[3 + 1];
	char            sResource[6 + 1];
	char            sLine[MAX_LINE_TRC];
	char            sTlvBuffer[LG_MAX];
	int             Length;
	char 			sSecurityData [128 + 1];

	trace_event("Start  FCT_REV_REP_FROM_NW()", PROCESSING);
	memset(RespCode, 0, sizeof(RespCode));
	memset(tmpField, 0, sizeof(tmpField));
	memset(SourceResource, 0, sizeof(SourceResource));
	memset(sResource, 0, sizeof(sResource));
	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	memset (sSecurityData, 0, sizeof(sSecurityData));


	/* free request from list */
	GetVisaField(VISA_AUDIT_NBR, VisaInfo, tmpField, &tmpFieldLen);
	InitNodeData(&Node);
	memcpy(Node.szFstKey, "1420", 4);
	memcpy(Node.szFstKey + 4, tmpField, 6);
	sprintf(sLine, "Cle REP Is :%s", Node.szFstKey);
	trace_event(sLine, TRACE);


	switch (FetchRequest('B', &Node))
	{
	case ENOEXIST:
		trace_event("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event("End FCT_REV_ADV_REP_FROM_BANK(NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;
	}

	InitIsoInfo(&IsoInfo);
	AnalyseIso(Node.sFstBuf, &IsoInfo);

	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	memcpy(sTlvBuffer, Node.sSndBuf, Node.nSndBufLen);


	/* Add Visa Response */
	if (GetVisaField(VISA_RESPONSE_CODE, VisaInfo, RespCode, &Length) == SUCCESS)
	{
		TranslateRespCode(nIndexCtx, ResourceId, RespCode, "RP", tmpField);
		InsertIsoField(ISO_RESPONSE_CODE, &IsoInfo, tmpField, 3);
	}

	SafResponseTraitement(	nIndexCtx,
		sTlvBuffer,
		"1430",
		RespCode,
		tmpField);

	/*Start IBO20200517 RAWMSG*/
	if (g_sCaptureMsg[NW_INC_RESP] == 'Y')
	{
		LogVisaMessage(nIndexCtx, VisaInfo, &IsoInfo, INC_RESP);
	}
	/*End IBO20200517 RAWMSG*/

	trace_event("End  FCT_REV_REP_FROM_NW(OK)", PROCESSING);
	return (OK);
}


/*****************************************************************************/
/****************************************************************************/
/* ReversalReqFromNw()                                                    */
/* */
/* Converts a received reversal request message (0400) from BASEI         */
/* into an ISO-format message for processing.                             */
/* Input:  VisaInfo: received message                                     */
/* Output: IsoInfo:  generated ISO message                                */
/****************************************************************************/
int
	ReversalReqFromNw(int nIndexCtx,
	TSVisaInfo * VisaInfo,
	TSIsoInfo * IsoInfo)
{
	TSTlvPrivate    TlvInfo;
	TSCpsInfo       CpsInfo;
	char            tmpField[BUF_LG_MAX];	/** MAN_12112017  Old Value 256**/
	char            tmpField1[256];
	char            tmpField2[256];
	char            MerchantType[5];
	char            RespCode[3];
	char            ProcCode[7];
	char            PosMode[7];
	char            PosCond[3];
	char            AddPos[11];
	char            sXmitDateTime[10 + 1];
	char            sStan[6 + 1];
	char            sExternalStan[6 + 1];
	char            sAcquirerId[11 + 1];
	char            sAcquirerIdLen[2 + 1];
	int             nAcquirerIdLen;
	char            IsoOrigData[35 + 1];
	char            VisaOrigData[42 + 1];
	char            sMti[4 + 1];
	int             Offset;
	char            sPid[8 + 1];
	int             tmpFieldLen;
	char            sLine[MAX_LINE_TRC];
	int             pos_flag = 0;
	char			sAuthId[64];
	char			sTrnId[64];
	char			sTrxAmount[32];
	char			sBillingAmount[32];
	char			sTrxCurrency[4];
	char			sBillingCurrency[4];
	BitMappedInfo	kVIPufData;
	char 			sIsoTermAddr[40+1];/*PLUTONL-3660*/
	char			sEcomAuthInfo[P7_F048_P66_LEN + 1];
	int				nEcomInfoLen = 0;

	trace_event("Start  ReversalReqFromNw()", PROCESSING);

	memset(RespCode, 0, sizeof(RespCode));
	memset(tmpField, 0, sizeof(tmpField));
	memset(tmpField1, 0, sizeof(tmpField1));
	memset(tmpField2, 0, sizeof(tmpField2));
	memset(ProcCode, 0, sizeof(ProcCode));
	memset(MerchantType, 0, sizeof(MerchantType));
	memset (sAuthId,   0, sizeof(sAuthId));
	memset (sTrnId,   0, sizeof(sTrnId));

	memset (sTrxAmount,   0, sizeof(sTrxAmount));
	memset (sBillingAmount,   0, sizeof(sBillingAmount));
	memset (sTrxCurrency,   0, sizeof(sTrxCurrency));
	memset (sBillingCurrency,   0, sizeof(sBillingCurrency));
	memset (sExternalStan,   0, sizeof(sExternalStan));
	memset(sEcomAuthInfo, '\0', sizeof(sEcomAuthInfo));
	memset(sEcomAuthInfo, ' ', P7_F048_P66_LEN);


	InitIsoInfo(IsoInfo);
	InitCpsInfo(&CpsInfo);
	InitVisaVIPPufDataTlv(&kVIPufData);

	IsoInfo->nMsgType = 1420;

	if (GetVisaField(VISA_CPS_FIELDS, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseCps(tmpField, &CpsInfo);
	}

	if (GetVisaField (VISA_PRIV_USE_FIELDS, VisaInfo, tmpField, &tmpFieldLen)== SUCCESS)
	{
		AnalyseBitMappedInfo(tmpField, tmpFieldLen, &kVIPufData);
	}

	memcpy(sAuthId , ResourceId, 2);	/*AMER 20160714*/

	/* Field 2: Primary Account Number */
	if (GetVisaField(VISA_CARD_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_CARD_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 3: Processing Code */
	GetVisaField(VISA_PROC_CODE, VisaInfo, ProcCode, &tmpFieldLen);
	GetVisaField(VISA_MERCHANT_TYPE, VisaInfo, MerchantType, &tmpFieldLen);
	VisaToIsoProcessingCode(ProcCode, MerchantType, tmpField);
	AddIsoField(ISO_PROC_CODE, IsoInfo, tmpField, 6);

	/* Field 4: Transaction Amount */
	if (GetVisaField(VISA_TRANS_AMOUNT, VisaInfo, sTrxAmount, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_TRANS_AMOUNT, IsoInfo, sTrxAmount, 12);

	/* Field 6: Cardholder Billing Amount */
	if (GetVisaField(VISA_BILLING_AMOUNT, VisaInfo, sBillingAmount, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_BILLING_AMOUNT, IsoInfo, sBillingAmount, 12);
	else if( strlen(sTrxAmount) > 0 )
	{
		AddIsoField(ISO_BILLING_AMOUNT, IsoInfo, sTrxAmount, 12);
	}

	/* Field 7: Transmission Date and Time */
	if (GetVisaField(VISA_XMIT_TIME, VisaInfo, sXmitDateTime, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_XMIT_TIME, IsoInfo, sXmitDateTime, 10);

	/* Field 10: Cardholder Billing Conversion Rate */
	if (GetVisaField(VISA_BILLING_CNVRSN_RATE, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_BILLING_CNVRSN_RATE, IsoInfo, tmpField, 8);

	/* Field 11: System Trace Audit Number */
	GetInternalStan(nIndexCtx, sStan);
	AddIsoField(ISO_AUDIT_NBR, IsoInfo, sStan, 6);

	GetVisaField(VISA_AUDIT_NBR, VisaInfo, sExternalStan, &tmpFieldLen);	/*AMER20171124 PROD00049633*/

	/* Field 12: Transaction local date and time  */
	/* ELKAM20221214 PLUTONL-5363 START */
	current_gmt_date (tmpField);
	AddIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField, 12);
	/* ELKAM20221214 PLUTONL-5363 END */

	/* Field 14: Expiry Date */
	if (GetVisaField(VISA_EXPIRY_DATE, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_EXPIRY_DATE, IsoInfo, tmpField, 4);

	/* Field 18: Merchant Type */
	if(strlen(MerchantType) > 0)
	{
		AddIsoField(ISO_MERCHANT_TYPE, IsoInfo, MerchantType, 4);
	}

	/* Field 19: Acquiring Institution Country Code */
	if (GetVisaField(VISA_ACQR_COUNTRY, VisaInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		AddIsoField(ISO_ACQR_COUNTRY, IsoInfo, tmpField, 3);
	}

	/* Field 20: PAN Country Code */
	if (GetVisaField(VISA_PAN_COUNTRY, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, 3);

	/* Field 22: POS Entry Mode */
	memset(PosMode, 0, sizeof(PosMode));
	memset(PosCond, 0, sizeof(PosCond));
	memset(AddPos, 0, sizeof(AddPos));

	if (
		(GetVisaField(VISA_POS_ENTRY_MODE, VisaInfo, PosMode, &tmpFieldLen) == SUCCESS) &&
		(GetVisaField(VISA_POS_CONDITION, VisaInfo, PosCond, &tmpFieldLen) == SUCCESS) &&
		(GetVisaField(VISA_ADTNL_POS_INFO, VisaInfo, AddPos, &tmpFieldLen) == SUCCESS)
		) {

			VisaToIsoPosMode(MerchantType, PosMode, PosCond, AddPos, tmpField, sEcomAuthInfo, &nEcomInfoLen); /*IBO20221025 PLUTONL-5272*/
			AddIsoField(ISO_POS_DATA, IsoInfo, tmpField, 12);
			pos_flag = 1;
	}

	/* Field 24: Function Code */
	if (GetVisaField(VISA_REPLACE_AMOUNTS, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		/* partial reversal */
		AddIsoField(ISO_FUNCTION_CODE, IsoInfo, P7_F024_PARTIAL_REVERSAL, P7_F024_LEN);
	else
		/* full reversal */
		AddIsoField(ISO_FUNCTION_CODE, IsoInfo, P7_F024_FULL_REVERSAL, P7_F024_LEN);

	/* Field 25: Message Reason, 4001 = Unspecified */
	AddIsoField(ISO_MSG_REASON, IsoInfo, "4001", 4);

	/* Field 30: replacement trx amount + replacement billing amount */
	/* in case of a partial reversal */
	memset(tmpField, 0, sizeof(tmpField));
	memset(tmpField1, 0, sizeof(tmpField1));
	if (GetVisaField(VISA_REPLACE_AMOUNTS, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		if ((GetVisaField(VISA_OTHER_AMOUNTS, VisaInfo, tmpField1, &tmpFieldLen) == SUCCES)
			&& (tmpFieldLen == 36)
			&& (memcmp(tmpField1 + 24, "000000000000", 12) != 0)
			)
			memcpy(tmpField + 12, tmpField1 + 12 + 12, 12);
		else
			memcpy(tmpField + 12, "000000000000", 12);
		AddIsoField(ISO_ORIGINAL_AMOUNTS, IsoInfo, tmpField, 24);
	}
	/* Field 32: Acquiring Institution Identification Code */
	if (GetVisaField(VISA_ACQR_ID, VisaInfo, sAcquirerId, &nAcquirerIdLen) == SUCCES)
		AddIsoField(ISO_ACQR_ID, IsoInfo, sAcquirerId, nAcquirerIdLen);

	/* Field 33: Forwarding Institution Id */
	if (GetVisaField(VISA_FORWD_ID, VisaInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_FORWD_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 37: Retrieval Reference Number */
	if (GetVisaField(VISA_REFERENCE_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		AddIsoField(ISO_REFERENCE_NBR, IsoInfo, tmpField, 12);
		memcpy(sAuthId + 2 + P7_F011_LEN, tmpField, P7_F037_LEN);	/*AMER 20160714*/
	}

	/* Field 38: Authorization Id */
	if (GetVisaField(VISA_AUTHOR_ID, VisaInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_AUTHOR_ID, IsoInfo, tmpField, 6);

	/* Field 39: Response Code */
	if (GetVisaField(VISA_RESPONSE_CODE, VisaInfo, RespCode, &tmpFieldLen) == SUCCESS)
	{
		TranslateRespCode(nIndexCtx, ResourceId, RespCode, "RP", tmpField);
		AddIsoField(ISO_RESPONSE_CODE, IsoInfo, tmpField, P7_F039_LEN);
	}
	/* Field 40: Service Restriction Code */
	if (GetVisaField(VISA_SERVICE_CODE, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_SERVICE_CODE, IsoInfo, tmpField, 3);

	/* Field 41: Terminal Identification */
	if (GetVisaField(VISA_TERMINAL_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_TERMINAL_NBR, IsoInfo, tmpField, 8);


	/* Field 42: Card Acceptor Identification Code */
	if (GetVisaField(VISA_OUTLET_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_OUTLET_NBR, IsoInfo, tmpField, 15);


	/* Field 43: Card Acceptor Location */
	/*PLUTONL-3660*/
	/*if (GetVisaField(VISA_TERMINAL_ADR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_TERMINAL_ADR, IsoInfo, tmpField, 40);*/
	if (GetVisaField(VISA_TERMINAL_ADR, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		VisaToIsoAcceptorName(nIndexCtx ,tmpField,sIsoTermAddr);
        AddIsoField(ISO_TERMINAL_ADR, IsoInfo, sIsoTermAddr, tmpFieldLen);

	}
	/*PLUTONL-3660*/


	memset(tmpField1, 0, sizeof(tmpField1));
	memcpy(tmpField1, tmpField, strlen(tmpField));



	/* Field 48: Additional Private Data */

	InitTlvPrivate(&TlvInfo);

	AddTlvPrivate(HPS_ACQUIRER_BANK_TAG, &TlvInfo, AcquirerBank, 6);

	if (GetVisaField(VISA_AUDIT_NBR, VisaInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		AddTlvPrivate(EXTERNAL_STAN_TAG, &TlvInfo, tmpField, 6);
		/*memcpy(sAuthId + 2, tmpField, P7_F011_LEN);*/
	}

	/*AMER 20160714:PROD00029686: We build the AuthId with the stan included in DE 90 instead of DE 11*/
	if (GetVisaField(VISA_ORIG_DATA, VisaInfo, VisaOrigData, &tmpFieldLen) == SUCCES)
	{
		/* Original Stan */
		if (memcmp(VisaOrigData + 4, "000000", 6) == 0)
                        memcpy(sAuthId + 2, sExternalStan /*sStan*/, 6);		/*AMER20171124 PROD00049633: We should use external stan instead of internal stan*/
		else
			memcpy(sAuthId + 2, VisaOrigData + 4, P7_F011_LEN);
	}
	else
		memcpy(sAuthId + 2, sExternalStan, 6); /* [ASO17012020] PROD00073688 */

	/*VisaBuildAuthId(VisaInfo,sAuthId);*/	/*AMER 20160714: To don't be used anymore. We construct the AuthId above*/
	AddTlvPrivate(AUTHORIZATION_IDENTIFIER, &TlvInfo, sAuthId, strlen(sAuthId));
	if (GetCpsSubField(CPS_TRANS_ID, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		strncpy(sTrnId,tmpField,sizeof(sTrnId));
		AddTlvPrivate(TRANSACTION_IDENTIFIER, &TlvInfo, sTrnId, strlen(sTrnId));
	}

	if (GetVisaField(VISA_ADTNL_DATA_PRIV, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS) {
		/* skip the asterisk (*) */
		AddTlvPrivate(MESSAGE_TEXT_TAG, &TlvInfo, tmpField + 1, tmpFieldLen - 1);
	}

	if (nEcomInfoLen > 0)	/*IBO20221025 PLUTONL-5272*/
	{
		AddTlvPrivate(ECOM_AUTH_INFO, &TlvInfo, sEcomAuthInfo, nEcomInfoLen);
	}

	tmpFieldLen = PrivateBuildTlv(tmpField, &TlvInfo);
	if (tmpFieldLen > 0)
		AddIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, tmpFieldLen);

	/* Field 49: Transaction Currency Code */
	if (GetVisaField(VISA_TRANS_CRNCY, VisaInfo, sTrxCurrency, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_TRANS_CRNCY, IsoInfo, sTrxCurrency, 3);

	/* Field 51: Cardholder Billing Currency Code */
	if (GetVisaField(VISA_BILLING_CRNCY, VisaInfo, sBillingCurrency, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_BILLING_CRNCY, IsoInfo, sBillingCurrency, tmpFieldLen);
	else if( strlen(sTrxCurrency) > 0 )
	{
		AddIsoField(ISO_BILLING_CRNCY, IsoInfo, sTrxCurrency, 3);
	}


	/* Field 53: Security Data */
	memset(tmpField1, 0, sizeof(tmpField1));
	PutSecurityData(SECURITY_FORMAT, tmpField1, "00", 2);
	PutSecurityData(PIN_BLOCK_FORMAT, tmpField1, "99", 2);
	PutSecurityData(PIN_ENCRYPTION_KEY_INDEX, tmpField1, "000", 3);
	PutSecurityData(MAC_KEY_INDEX, tmpField1, "000", 3);
	memset(sPid, 0, sizeof(sPid));
	sprintf(sPid, "%08d", getpid());
	PutSecurityData(SOURCE_PID, tmpField1, sPid, 8);
	PutSecurityData(SOURCE_RESOURCE, tmpField1, ResourceId, 6);
	PutSecurityData(DESTINATION_PID, tmpField1, "00000000", 8);
	PutSecurityData(DESTINATION_RESOURCE, tmpField1, "000000", 6);
	PutSecurityData(USER, tmpField1, "VISA           ", 15);
	PutSecurityData(PURGE_TIME, tmpField1, "000000000", 9);
	AddIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField1, strlen(tmpField1));

	/* Field 56: Original Data */
	if (GetVisaField(VISA_ORIG_DATA, VisaInfo, VisaOrigData, &tmpFieldLen) == SUCCES)
	{
		memset(IsoOrigData, 0, sizeof(IsoOrigData));
		memset(IsoOrigData, '0', 35);

		Offset = 0;
		/* Original Type de Message */
		memset(sMti, 0, sizeof(sMti));
		memcpy(sMti, VisaOrigData, 4);
		sprintf(sMti, "%4d", atoi(sMti) + 1000);
		memcpy(IsoOrigData + Offset, sMti, 4);
		Offset += 4;

		/* Original Stan */
		if (memcmp(VisaOrigData + 4, "000000" /*"OOOOOO"*/, 6) == 0)	/*AMER20171124 PROD00049633: We should use 0 instead of O*/
			memcpy(IsoOrigData + Offset, sExternalStan /*sStan*/, 6);	/*AMER20171124 PROD00049633: We should use external stan instead of internal stan*/
		else
			memcpy(IsoOrigData + Offset, VisaOrigData + 4, 6);
		Offset += 6;

		/* Original Xmit */
		memcpy(IsoOrigData + Offset, "00", 2);
		Offset += 2;

		if (memcmp(VisaOrigData + 4 + 6, "0000000000", 10) == 0)
			memcpy(IsoOrigData + Offset, sXmitDateTime, 10);
		else
			memcpy(IsoOrigData + Offset, VisaOrigData + 4 + 6, 10);
		Offset += 10;

		/* Original Acquirer Id */
		sprintf(sAcquirerIdLen, "%02d", nAcquirerIdLen);
		memcpy(IsoOrigData + Offset, sAcquirerIdLen, 2);
		Offset += 2;

		if (memcmp(VisaOrigData + 4 + 6 + 10, "00000000000", 11) == 0)
			memcpy(IsoOrigData + Offset + 11 - nAcquirerIdLen, sAcquirerId, nAcquirerIdLen);
		else
			memcpy(IsoOrigData + Offset, VisaOrigData + 4 + 6, 11);

		AddIsoField(ISO_ORIG_DATA, IsoInfo, IsoOrigData, 35);
	}

	/* Field 61: Network POS Data, in TLV format */
	if (pos_flag != 0)
	{
		InitTlvPrivate(&TlvInfo);
		if (strlen(PosMode) > 0)
			AddTlvPrivate(POS_ENTRY_MODE_TAG, &TlvInfo, PosMode, 4);
		if (strlen(PosCond) > 0)
			AddTlvPrivate(POS_CONDITION_CODE_TAG, &TlvInfo, PosCond, 2);

		if (strlen(RespCode) > 0)
			AddTlvPrivate(RESPONSE_CODE_TAG, &TlvInfo, RespCode, 2);
		if (strlen(AddPos) > 0)
			AddTlvPrivate(ADTNL_POS_DATA_TAG, &TlvInfo, AddPos, strlen(AddPos));
		tmpFieldLen = PrivateBuildTlv(tmpField, &TlvInfo);
		AddIsoField(ISO_NET_POS_DATA, IsoInfo, tmpField, tmpFieldLen);
	}

	/* Field 62: CPS Data, in TLV format */
	if (GetVisaField(VISA_CPS_FIELDS, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitTlvPrivate(&TlvInfo);
		if (GetCpsSubField(CPS_ACI, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddTlvPrivate(CPS_ACI_TAG, &TlvInfo, tmpField, 1);

		if (GetCpsSubField(CPS_TRANS_ID, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddTlvPrivate(CPS_TRANS_ID_TAG, &TlvInfo, tmpField, 15);

		if (GetCpsSubField(CPS_VALIDATION_CODE, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddTlvPrivate(CPS_VALIDATION_DATE_TAG, &TlvInfo, tmpField, tmpFieldLen);

		if (GetCpsSubField (CPS_MVV , &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddTlvPrivate(CPS_MVV_TAG ,&TlvInfo,tmpField,tmpFieldLen);

		tmpFieldLen = PrivateBuildTlv(tmpField, &TlvInfo);
		if( tmpFieldLen > 0 )
			AddIsoField(ISO_CPS_FIELDS, IsoInfo, tmpField, tmpFieldLen);
	}

	/*Start AMER20171004 PROD00046795 Enh17.Q4*/
	InitTlvPrivate(&TlvInfo);
	if ( GetBitMappedInfoField (B1_F063_STIP_SWITCH_RSN_CD, &kVIPufData, tmpField, &tmpFieldLen) == SUCCESS)
		AddTlvPrivate(NET_STIP_REASON_CODE_TAG, &TlvInfo, tmpField, tmpFieldLen);


	tmpFieldLen = PrivateBuildTlv(tmpField, &TlvInfo);
	if( tmpFieldLen > 0 )
	{
		AddIsoField(ISO_NETWORK_DATA, IsoInfo, tmpField, tmpFieldLen);
	}
	/*End AMER20171004 PROD00046795 Enh17.Q4*/


	dump_p7_struct(IsoInfo);
	trace_event("End  ReversalReqFromNw(OK)", PROCESSING);
	return (OK);
}



/*************************************************************************/
/* ReversalReqToNw()                                                   */
/* */
/* Prepares a reversal request message (0400) to be sent to BASEI.     */
/* Input:  ISO format message                                          */
/* Output: VISA format message                                         */
/*************************************************************************/
int ReversalReqToNw(int nIndexCtx, TSIsoInfo * IsoInfo, TSVisaInfo * VisaInfo)
{
	TSCpsInfo       CpsInfo;
	TSTlvPrivate    kTlvInfo;
	TSTlvPrivate    TlvAddDataPriv;
	TSTagIso        TagIso;
	TlvInfo			kVisaRecurringPayTlv;	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	TlvInfo			kIsoRecurringPayTlv;	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	TlvInfo			kVisaTransSpecTlv;	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	char            tmpField[1024];
	char            tmpField1[1024];
	char            tmpField2[1024];
	char            sTraceAudit[12];
	int             tmpFieldLen;
	char            MerchantType[12];
	char            isoProcCode[12];
	char            isoPosMode[24];
	char            NwPosMode[5];
	char            sAcqLen[4];
	int             nAcqLen;
	int             i;
	char            sAcqId[32];
	char            sRepAmount[64];
	char            sMcc[12];
	char            sLine[MAX_LINE_TRC];
	char            sRefNbr[12 + 1];
	char            szDate[12 + 1];
	char            szWork[256 + 1];
	int             nRetCode;
	char 			OutletNumber[15 +1 ];
	char 			AquirerBin[12 +1 ];
	char          	Priv_Use_Field[255 + 1];
	char          	vBitMap[3 +1];
	char			szAcquirerBank[6 + 1];
	char 			isoFunctionCode[4];
	char			isoTrxAmnt[13];
	int				verification_only = 0;
	char			RecurringFlag[2];
	TSVisaPufInfo   VisapufInfo;/*ZKO13112018*/
	char            sVisaPufData[512];/*ZKO13112018*/
	char            sAddAuthInfo[P7_F048_P64_LEN + 1];
	char 			sPosExtData[P7_F048_P61_LEN +1];
	int				nNbrTags = 0;		/*ZKO 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	memset(tmpField, 0, sizeof(tmpField));
	memset(tmpField1, 0, sizeof(tmpField1));
	memset(tmpField, 0, sizeof(tmpField));
	memset(MerchantType, 0, sizeof(MerchantType));
	memset(isoProcCode, 0, sizeof(isoProcCode));
	memset(isoPosMode, 0, sizeof(isoPosMode));
	memset(sTraceAudit, 0, sizeof(sTraceAudit));
	memset(szAcquirerBank, 0, sizeof(szAcquirerBank));
	memset(OutletNumber, 0, sizeof(OutletNumber));/*NZE21042016*/
	memset(NwPosMode, 0, sizeof(NwPosMode));
	memset(isoFunctionCode, 0, sizeof(isoFunctionCode));
	memset(RecurringFlag, 0, sizeof(RecurringFlag));
	memset(sAddAuthInfo, 0, sizeof(sAddAuthInfo));
	memset(sPosExtData, 0, sizeof(sPosExtData));


	trace_event("Start  ReversalReqToNw()", PROCESSING);

	InitVisaInfo(VisaInfo);
	InitTlvPrivate(&TlvAddDataPriv);
	InitVisaRecurringPayDataTlv(&kVisaRecurringPayTlv);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	IniIsoInstallmentLoanTlv(&kIsoRecurringPayTlv);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	InitVisaTransSpecDataTlv(&kVisaTransSpecTlv);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		AnalyseTlvPrivate(tmpField, tmpFieldLen, &TlvAddDataPriv);
	}

	VisaInfo->nMsgType = IsoInfo->nMsgType - 1020;

	/* (2) Primary Account Number */
	if (GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_CARD_NBR, VisaInfo, tmpField, tmpFieldLen);

	/* (3) Processing Code */
	strcpy(sMcc, "0000");
	GetIsoField(ISO_PROC_CODE, IsoInfo, isoProcCode, &tmpFieldLen);
	GetIsoField(ISO_MERCHANT_TYPE, IsoInfo, sMcc, &tmpFieldLen);
	IsoToVisaProcessingCode(isoProcCode, tmpField, sMcc);
	AddVisaField(VISA_PROC_CODE, VisaInfo, tmpField, 6);

	/* (4): Transaction Amount */
	GetIsoField(ISO_TRANS_AMOUNT, IsoInfo, isoTrxAmnt /*tmpField*/, &tmpFieldLen);
	AddVisaField(VISA_TRANS_AMOUNT, VisaInfo, isoTrxAmnt /*tmpField*/, 12);

	/* (7) Transaction Date/Time */
	GetIsoField(ISO_XMIT_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddVisaField(VISA_XMIT_TIME, VisaInfo, tmpField, 10);

	/* Recuperer le champs 11 a partir des donn�es originaux  Voir Field 90 */

	memset (tmpField1, 0, sizeof(tmpField1));
	memset (tmpField2, 0, sizeof(tmpField2));
	if (GetIsoField (ISO_ORIG_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		PutVisaField (VISA_AUDIT_NBR, VisaInfo, tmpField + 4, 6);
	}
	/*(12) & (13) Time /Date Local Transaction*//*PLUTONL-3665*/
	if(GetIsoField(ISO_TRANS_DATE_TIME, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AddVisaField (VISA_TRANS_TIME, VisaInfo, tmpField+6, 6);
		AddVisaField (VISA_TRANS_DATE, VisaInfo, tmpField+2, 4);
	}
	/*PLUTONL-3665*/

	/* (14) Expiry Date */
	if (GetIsoField(ISO_EXPIRY_DATE, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_EXPIRY_DATE, VisaInfo, tmpField, 4);

	/* (18) Merchant Type */
	GetIsoField(ISO_MERCHANT_TYPE, IsoInfo, MerchantType, &tmpFieldLen);
	AddVisaField(VISA_MERCHANT_TYPE, VisaInfo, MerchantType, 4);

	/* (19) Acquiring Institution Country Code */
	GetIsoField(ISO_ACQR_COUNTRY, IsoInfo, tmpField, &tmpFieldLen);
	AddVisaField(VISA_ACQR_COUNTRY, VisaInfo, tmpField, 3);

	/* (20): PAN Country Code */
	if (GetIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_PAN_COUNTRY, VisaInfo, tmpField, 3);

	/* (22) POS Entry Mode */
	if (GetIsoField(ISO_POS_DATA, IsoInfo, isoPosMode, &tmpFieldLen) == SUCCESS)
	{
		IsoToVisaPosMode(isoPosMode, NwPosMode /*tmpField*/);
		AddVisaField(VISA_POS_ENTRY_MODE, VisaInfo, NwPosMode /*tmpField*/, 4);
	}

	/* (23) CARD SEQUENCE NUMBER */
	if (GetIsoField(ISO_CARD_SEQUENCE_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_CARD_SEQUENCE_NBR, VisaInfo, tmpField, 3);


	/* (25) POS Condition Code */
	/*IsoToVisaPosCondition(isoPosMode, MerchantType, tmpField);
	AddVisaField(VISA_POS_CONDITION, VisaInfo, B1_F025_MOTO_RECURRING, B1_F025_LEN);*/
	/*Start AMER20180309 PROD00054205 Enh18.Q2 Art 1.1*/
	GetIsoField(ISO_FUNCTION_CODE, IsoInfo, isoFunctionCode, &tmpFieldLen);
	if(memcmp(isoFunctionCode, P7_F024_ACC_VERIF, P7_F024_LEN) == 0 && atoi(isoTrxAmnt) == 0)
		verification_only = 1;
	IsoToVisaPosCondition(isoPosMode, MerchantType, verification_only, tmpField);
	AddVisaField(VISA_POS_CONDITION, VisaInfo, tmpField, B1_F025_LEN);
	/*End AMER20180309 PROD00054205 Enh18.Q2 Art 1.1*/

	/* (32) Acquiring Institution Code */
	/*EBE
	if (GetIsoField (ISO_OUTLET_NBR, IsoInfo, OutletNumber, &tmpFieldLen) == SUCCESS)
	{
	nRetCode = GetVisaAquirer( nIndexCtx, "01", OutletNumber , AquirerBin)  ;
	if (nRetCode != OK)
	{
	trace_event("End BuildAutReqToNw (NOK) : ERROR Getting AquirerBin",ERROR);
	return(NOK);
	}
	else
	{
	AddVisaField (VISA_ACQR_ID, VisaInfo, AquirerBin , strlen(AquirerBin));
	}
	}*/
	if (GetTlvPrivate(ACQUIRER_BANK_TAG, &TlvAddDataPriv, szAcquirerBank , &tmpFieldLen) != SUCCESS)
	{
		trace_event("End ReversalReqToNw (NOK) : ERROR Getting Acquirer bank",ERROR);
		return(NOK);
	}
	nRetCode = GetAquirerBin( nIndexCtx, szAcquirerBank, NET_VISA, AquirerBin)  ;
	if (nRetCode != OK)
	{
		sprintf(sLine,"End ReversalReqToNw (NOK) : ERROR Getting AquirerBin for bank[%s]",szAcquirerBank);
		trace_event(sLine,ERROR);
		return(NOK);
	}
	AddVisaField (VISA_ACQR_ID, VisaInfo, AquirerBin , strlen(AquirerBin));	


	/* (33) Forwarding Institution Code */

	/* (34) PAN Extended */
	if (GetIsoField(ISO_PAN_EXTENDED, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_PAN_EXTENDED /*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/, VisaInfo, tmpField, tmpFieldLen);

	/* (37) Reference Number */

	memset (tmpField1, 0, sizeof(tmpField1));
	memset (tmpField2, 0, sizeof(tmpField2));
	if (GetIsoField (ISO_ORIG_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		memcpy  (tmpField1, tmpField, 4);
		sprintf (tmpField2, "%04d", 100);
		memcpy  (tmpField2 + 4 , tmpField + 4, 6);
		memcpy  (tmpField2 + 10, tmpField + 12, 10);
		memset(sAcqId, 0, sizeof(sAcqId));
		memset(sAcqId, '0', 22);
		memcpy(sAcqId+(11-strlen(AcquirerId)), AcquirerId  , strlen(AcquirerId));
		memcpy  (tmpField2 + 20, sAcqId, 22);

		/* Upgrade of the Reference Number */
		memset(sRefNbr, 0, sizeof(sRefNbr));
		memset(szDate, 0, sizeof(szDate));
		memset(szWork, 0, sizeof(szWork));
		current_date_yyyymmdd(szWork);
		memcpy(tmpField, tmpField2 + 10, 10);
		memcpy(szDate , tmpField + 2, 2);
		memcpy(szDate + 2 , tmpField, 2);
		memcpy(szDate + 4 , szWork , 4);
		memset(szWork, 0, sizeof(szWork));
		nRetCode = convert_to_julian (szDate,szWork);
		sRefNbr[0] = szDate[7];
		memcpy(sRefNbr + 1, szWork, 3);
		memcpy(sRefNbr + 4, tmpField + 4, 2);
		memcpy(sRefNbr + 6, tmpField2 + 4     , 6);
		PutVisaField (VISA_REFERENCE_NBR, VisaInfo, sRefNbr, 12);
	}

	/* (38) Authorization Id Response */
	/* BURGAN ONSITE CERTIF: in case of timeout reversal, field 38 contains A99999, should not */ /* BURGAN_02 */
	GetIsoField(ISO_AUTHOR_ID, IsoInfo, tmpField, &tmpFieldLen);
	if (memcmp(tmpField, "A9", 2) !=0)
	AddVisaField(VISA_AUTHOR_ID, VisaInfo, tmpField, 6);

	/* (40) Service Restriction Code */

	/* (41) Card Acceptor Terminal Id */
	if (GetIsoField(ISO_TERMINAL_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddVisaField(VISA_TERMINAL_NBR, VisaInfo, tmpField, 8);

	/* (42) Card Acceptor Id *//*NZE21042016 Activation de r�cup�ration du champ 42*/
	if (GetIsoField (ISO_OUTLET_NBR, IsoInfo, OutletNumber, &tmpFieldLen) == SUCCESS)
	{
		AddVisaField (VISA_OUTLET_NBR, VisaInfo, OutletNumber, 15);
	}

	/* (43) Card Acceptor Name/Location */

	if (GetIsoField(ISO_TERMINAL_ADR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		replace(tmpField, '>', ' ', tmpFieldLen);
		replace(tmpField, '_', ' ', tmpFieldLen);
		AddVisaField(VISA_TERMINAL_ADR, VisaInfo, tmpField, 40);
	}
	/* Field 48: Additional Private Data */

	/* (49) Transaction Currency Code */
	GetIsoField(ISO_TRANS_CRNCY, IsoInfo, tmpField, &tmpFieldLen);
	AddVisaField(VISA_TRANS_CRNCY, VisaInfo, tmpField, 3);

	/* (55) Additional POS Info */
	/** MAN_12112017 ONSITE CERTIF No need to send ICC for Reversal
	if (GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		memset(tmpField1, 0, sizeof(tmpField1));
		tmpField1[0] = 0x01;
		tmpField1[1] = 0x00;
		tmpField1[2] = tmpFieldLen;
		memcpy(tmpField1 + 3, tmpField, tmpFieldLen);
		AddVisaField(VISA_ICC_DATA, VisaInfo, tmpField1, tmpFieldLen + 3);
	}
	** MAN_12112017 **/

	/*Start AMER20180307 PROD00054205: Enh 18.Q2 Art 3.6*/
	if (GetIsoField(ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS && memcmp(NwPosMode, "91", 2) == 0)
	{
		memset(tmpField1, 0, sizeof(tmpField1));
		tmpField1[0] = 0x01;
		tmpField1[1] = 0x00;
		tmpField1[2] = tmpFieldLen;
		memcpy(tmpField1 + 3, tmpField, tmpFieldLen);
		AddVisaField(VISA_ICC_DATA, VisaInfo, tmpField1, tmpFieldLen + 3);
	}
	/*End AMER20180307 PROD00054205: Enh 18.Q2 Art 3.6*/

	GetTlvPrivate( RECURRING_FLAG_TAG , &TlvAddDataPriv, RecurringFlag, &tmpFieldLen); /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	GetTlvPrivate( AUTH_ADD_INFO , &TlvAddDataPriv, sAddAuthInfo, &tmpFieldLen);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	/* (60) Additional POS Info */
	if (GetIsoField(ISO_POS_DATA, IsoInfo, isoPosMode, &tmpFieldLen) == SUCCESS)
	{
               memset(tmpField, 0, sizeof(tmpField));/*ABL13112018*/
		/*GetTlvPrivate( RECURRING_FLAG_TAG , &TlvInfo, RecurringFlag, &tmpFieldLen);*/
		/*GetTlvPrivate( RECURRING_FLAG_TAG , &TlvAddDataPriv, RecurringFlag, &tmpFieldLen);*/ /*ABL28112018 Crash Interface*/
		/*GetTlvPrivate( AUTH_ADD_INFO , &TlvAddDataPriv, sAddAuthInfo, &tmpFieldLen);	*/	/*AMER20190219 PROD00065991 Enh19.Q2 Art 2.1*/
		GetTlvPrivate( POS_DATA_EXT_TAG , &TlvAddDataPriv, sPosExtData, &tmpFieldLen);	/*PLUTONL-3326*/
		BuildAddPosInfo(isoPosMode, MerchantType, isoProcCode, RecurringFlag, sAddAuthInfo,sPosExtData ,tmpField, IsoInfo);	/*AMER20180309 PROD00054205 Enh18.Q2 Art 1.1: Adding RecurringFlag argument*/
		AddVisaField(VISA_ADTNL_POS_INFO, VisaInfo, tmpField, strlen(tmpField));
	}
	/* (62) CPS Fields */
	InitTlvPrivate (&kTlvInfo);	/*HAL28082020 PLUTONL-2111 ENHS 20.Q4 ART 9.1.1*/
	InitCpsInfo (&CpsInfo);	/*HAL28082020 PLUTONL-2111 ENHS 20.Q4 ART 9.1.1*/


	if (GetIsoField (ISO_CPS_FIELDS, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvPrivate (tmpField, tmpFieldLen, &kTlvInfo);
		if (GetTlvPrivate (CPS_ACI_TAG,&kTlvInfo,tmpField,&tmpFieldLen) == SUCCESS)
			AddCpsSubField (CPS_ACI, &CpsInfo, tmpField, 1);
		if (GetTlvPrivate (CPS_TRANS_ID_TAG, &kTlvInfo, tmpField, &tmpFieldLen)  == SUCCESS)
			AddCpsSubField (CPS_TRANS_ID, &CpsInfo, tmpField, 15);

	}
	/*HAL28082020 PLUTONL-2111 ENHS 20.Q4 ART 9.1.1*/
	if (GetTlvPrivate(MVV_TAG, &TlvAddDataPriv, tmpField , &tmpFieldLen) == SUCCESS)
	{
		AddCpsSubField(CPS_MVV, &CpsInfo, tmpField, 10);
	}

	tmpFieldLen = CpsBuildField (tmpField, &CpsInfo);
	if (tmpFieldLen > 8)
		AddVisaField (VISA_CPS_FIELDS, VisaInfo, tmpField, tmpFieldLen);
	/*HAL28082020 PLUTONL-2111 ENHS 20.Q4 ART 9.1.1*/

	/* (63) Visa Private use field*/
	memset(Priv_Use_Field, 0, sizeof(Priv_Use_Field));
	vBitMap[0] = 0xA0;
	vBitMap[1] = 0x00;
	vBitMap[2] = 0x00;
	memcpy(Priv_Use_Field,vBitMap,3);
	/**Network Id :0000*/
	Priv_Use_Field[3] = 0x00;
	Priv_Use_Field[4] = 0x00;
	/**Message Reason Code:2501**/
	memset(tmpField, 0, sizeof(tmpField));
	Priv_Use_Field[5] = 0x25;
	if ((GetIsoField (ISO_MSG_REASON, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) && memcmp(tmpField, "4007", 4) == 0 ) /*OTA21112018 Visa Certif MCB*/
		Priv_Use_Field[6] = 0x02;
	else
	Priv_Use_Field[6] = 0x01;
	AddVisaField(VISA_PRIV_USE_FIELDS, VisaInfo, Priv_Use_Field, 7);


	/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if (GetTlvPrivate(P7_F048_LOAN_DATA, &TlvAddDataPriv, tmpField , &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kIsoRecurringPayTlv);

		if( GetTlvInfoField (P7_F048S24_NBR_INSTALMENTS, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			PutTlvInfoField(B1_F104T18_NBR_RECURRING_PAY, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
		}

		if( GetTlvInfoField (P7_F048S24_FREQ_RECUR_PAY, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			PutTlvInfoField(B1_F104T18_FREQ_RECURRING_PAY, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
		}

		if( GetTlvInfoField (P7_F048S24_REGISTRATION_REF_NBR, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			PutTlvInfoField(B1_F104T18_REGISTR_REF_NBR, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
		}

		if( GetTlvInfoField (P7_F048S24_MONTHLY_RPYMNT_AMNT, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			PutTlvInfoField(B1_F104T18_MAX_RECURRING_PAY_AMNT, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
		}

	}
	if(memcmp(isoFunctionCode, P7_F024_RECURRING_MODIF, P7_F024_LEN) == 0 && atoi(isoTrxAmnt) == 0)
	{
		PutTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_MODIFICATION, 1);
	}
	else if(memcmp(isoFunctionCode, P7_F024_RECURRING_CANCEL, P7_F024_LEN) == 0 && atoi(isoTrxAmnt) == 0){
		PutTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_CANCELLATION, 1);
	}
	else if (strlen(RecurringFlag) > 0
		&&(RecurringFlag[0] ==P7_F048_P71_CRED_ON_FILE
		||RecurringFlag[0]==P7_F048_P71_INSTALLMENT
		||RecurringFlag[0]==P7_F048_P71_RECURRING
		||RecurringFlag[0]==P7_F048_P71_STANDING))
	{
		if(strlen(sAddAuthInfo) > P7_F048_P64_CRDNTL_REASON
			&& sAddAuthInfo[P7_F048_P64_CRDNTL_REASON] != P7_F048_P64_P7_UNSPECIFIED)
			{
				PutTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_REGISTRATION_FIRST_TRANS, 1);
			}
			else{
				PutTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_SUBSEQ_TRANS, 1);
			}
	}

	if (strlen(RecurringFlag) > 0
		&&(RecurringFlag[0]==P7_F048_P71_RECURRING
		||RecurringFlag[0]==P7_F048_P71_INSTALLMENT))
	{
		PutTlvInfoField(B1_F104T18_PAY_AMNT_IND_PER_TRANS, &kVisaRecurringPayTlv, B1_F104T18_T81_FIX_AMNT_IND_PER_TRANS, 1);
	}

	if(strlen(RecurringFlag) > 0
		&&(RecurringFlag[0]==P7_F048_P71_STANDING
		||RecurringFlag[0]==P7_F048_P71_CRED_ON_FILE))
	{
		PutTlvInfoField(B1_F104T18_PAY_AMNT_IND_PER_TRANS, &kVisaRecurringPayTlv, B1_F104T18_T81_RECUR_PAY_MAX_AMNT, 1);
	}


	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kVisaRecurringPayTlv);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(B1_F104_RECURRING_PAY, &kVisaTransSpecTlv, tmpField, tmpFieldLen);
	}

	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kVisaTransSpecTlv);
	if (tmpFieldLen > 0)
		AddVisaField(VISA_TRANS_DESC, VisaInfo, tmpField, tmpFieldLen);
	/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	/* (90) Original Data Elements */
	memset(tmpField1, 0, sizeof(tmpField1));
	memset(tmpField2, 0, sizeof(tmpField2));
	if (GetIsoField(ISO_ORIG_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		memcpy(tmpField1, tmpField, 4);
		sprintf(tmpField2, "%04d", 100);
		memcpy(tmpField2 + 4, tmpField + 4, 6);
		memcpy(tmpField2 + 10, tmpField + 12, 10);
		memset(sAcqId, 0, sizeof(sAcqId));
		memset(sAcqId, '0', 22);
		memcpy(sAcqId + (11 - strlen(AcquirerId)), AcquirerId, strlen(AcquirerId));
		memcpy(tmpField2 + 20, sAcqId, 22);
		AddVisaField(VISA_ORIG_DATA, VisaInfo, tmpField2, 42);

	}
	/* (95) Replacement Amount, if this is a partial reversal */
	memset(tmpField, 0, sizeof(tmpField));


	if (GetIsoField(ISO_ORIGINAL_AMOUNTS, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		memset(sRepAmount, 0, sizeof(sRepAmount));
		memset(sRepAmount, '0', 42);
		memcpy(sRepAmount, tmpField, 12);
		AddVisaField(VISA_REPLACE_AMOUNTS, VisaInfo, sRepAmount, 42);
	}

	/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if (GetTlvPrivate(P7_F048_LOAN_DATA, &TlvAddDataPriv, tmpField , &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kIsoRecurringPayTlv);

		if( GetTlvInfoField (P7_F048S24_NBR_INSTALMENTS, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(B1_F104T18_NBR_RECURRING_PAY, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_FREQ_RECUR_PAY, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(B1_F104T18_FREQ_RECURRING_PAY, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_REGISTRATION_REF_NBR, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(B1_F104T18_REGISTR_REF_NBR, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_MONTHLY_RPYMNT_AMNT, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(B1_F104T18_MAX_RECURRING_PAY_AMNT, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		/*Start IZL 26092022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/
		if( GetTlvInfoField (P7_F048S24_VALIDATION_IND, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(B1_F104T18_VALIDATION_IND, &kVisaRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}
		/*End IZL 26092022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/

	}
	if(memcmp(isoFunctionCode, P7_F024_RECURRING_MODIF, P7_F024_LEN) == 0 && atoi(isoTrxAmnt) == 0)
	{
		AddTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_MODIFICATION, B1_F104T18_RECURRING_PAY_TYPE_LEN);
		nNbrTags++;
	}
	else if(memcmp(isoFunctionCode, P7_F024_RECURRING_CANCEL, P7_F024_LEN) == 0 && atoi(isoTrxAmnt) == 0)
	{
		AddTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_CANCELLATION, B1_F104T18_RECURRING_PAY_TYPE_LEN);
		nNbrTags++;
	}
	else if (strlen(RecurringFlag) > 0
		&&(RecurringFlag[0] ==P7_F048_P71_CRED_ON_FILE
		||RecurringFlag[0]==P7_F048_P71_INSTALLMENT
		||RecurringFlag[0]==P7_F048_P71_RECURRING
		||RecurringFlag[0]==P7_F048_P71_STANDING))
	{
		if(strlen(sAddAuthInfo) > P7_F048_P64_CRDNTL_REASON
			&& sAddAuthInfo[P7_F048_P64_CRDNTL_REASON] != P7_F048_P64_P7_UNSPECIFIED)
			{
				AddTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_REGISTRATION_FIRST_TRANS, B1_F104T18_RECURRING_PAY_TYPE_LEN);
				nNbrTags++;
			}
			else
			{
				AddTlvInfoField(B1_F104T18_RECURRING_PAY_TYPE, &kVisaRecurringPayTlv, B1_F104T18_T80_SUBSEQ_TRANS, B1_F104T18_RECURRING_PAY_TYPE_LEN);
				nNbrTags++;
			}
	}

	if (strlen(RecurringFlag) > 0
		&&(RecurringFlag[0]==P7_F048_P71_RECURRING
		||RecurringFlag[0]==P7_F048_P71_INSTALLMENT))
	{
		AddTlvInfoField(B1_F104T18_PAY_AMNT_IND_PER_TRANS, &kVisaRecurringPayTlv, B1_F104T18_T81_FIX_AMNT_IND_PER_TRANS, B1_F104T18_PAY_AMNT_IND_PER_TRANS_LEN);
		nNbrTags++;
	}
	else if(strlen(RecurringFlag) > 0
		&&(RecurringFlag[0]==P7_F048_P71_STANDING
		||RecurringFlag[0]==P7_F048_P71_CRED_ON_FILE))
	{
		AddTlvInfoField(B1_F104T18_PAY_AMNT_IND_PER_TRANS, &kVisaRecurringPayTlv, B1_F104T18_T81_RECUR_PAY_MAX_AMNT, B1_F104T18_PAY_AMNT_IND_PER_TRANS_LEN);
		nNbrTags++;
	}


	if(strlen(RecurringFlag) > 0
		&& nNbrTags != VISA_F104_T18_NB_FIELDS /*- 1 */	/*IZL 26092022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/
		&& GetIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS
		&& memcmp(tmpField,"356", 3) == 0) /*Indian Cards*/
	{
		trace_message(P7_TL_ERROR, "Tags missing in TLV Field 104, Usage 2, Dataset ID 18 for Indian Card");
		trace_message(P7_TL_PROCESSING, "End  ReversalReqToNw ERROR(NOK)");
		return (NOK);
	}


	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kVisaRecurringPayTlv);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(B1_F104_RECURRING_PAY, &kVisaTransSpecTlv, tmpField, tmpFieldLen);
	}

	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kVisaTransSpecTlv);
	if (tmpFieldLen > 0)
		AddVisaField(VISA_TRANS_DESC, VisaInfo, tmpField, tmpFieldLen);
	/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	/* Field 126: VISA Private Use Fields */
	/*ZKO:Start 13112018*/
	if ( GetTlvPrivate( DCC_DATA , &TlvAddDataPriv,tmpField, &tmpFieldLen) == SUCCESS
			&& tmpField[0] == P7_F048_P56_DCC_APPROVED)
	{


		InitVisaPufInfo(&VisapufInfo);

		AddVisaPufSubField(VPUF_DCC_INDICATOR, &VisapufInfo , "1", 1);

		memset(sVisaPufData, 0, sizeof(sVisaPufData));

		tmpFieldLen = VisaPufBuildField(sVisaPufData,&VisapufInfo);
		if (tmpFieldLen > 0)
			AddVisaField (VISA_PRIV_USE_FIELD , VisaInfo, sVisaPufData, tmpFieldLen);

	}
	/*ZKO:End */



	trace_event("End  ReversalReqToNw(OK)", PROCESSING);
	return (OK);
}




/*************************************************************************/
/* ReversalRepToNw()                                                   */
/* */
/* Prepares an reversal response message (0410) to VISA.               */
/* Input:  ISO format message                                          */
/* Output: VISA message                                                */
/*************************************************************************/
int
	ReversalRepToNw(int nIndexCtx,
	TSIsoInfo * IsoInfo,
	TSVisaInfo * OrigVisaInfo)
{
	TSTlvPrivate    TlvInfo;
	TSVisaInfo      NewVisaInfo;
	char            tmpField[256];
	char          	tmpField1[256];
	char            isoResponse[4];
	char            FieldData[256];
	char            sBuffer[1024];
	int             tmpFieldLen;
	int				Length;
	int             i;


	trace_event("Start  ReversalRepToNw()", PROCESSING);

	InitVisaInfo(&NewVisaInfo);
	memset(sBuffer, 0, sizeof(sBuffer));


	if (OrigVisaInfo->nMsgType == 420)
		NewVisaInfo.nMsgType = 430;
	else
		NewVisaInfo.nMsgType = 410;

	for (i = 0; i < MAX_VISA_FIELDS; i++) {
		switch (i)
		{
		case VISA_PRIV_USE_FIELDS:
			if (VisaEchoedField_0400[i] == 1)
			{
				if (GetVisaField (i, OrigVisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
				{
					if((tmpField[0]&0x80)==0x80)
					{
						memset(tmpField1,0,sizeof(tmpField1));
						tmpField1[0]=0x80;
						tmpField1[1]=0x00;
						tmpField1[2]=0x00;
						memcpy(tmpField1+3 ,tmpField+3,2);
						AddVisaField (i, &NewVisaInfo, tmpField1, 5);
					}
				}
			}
			break;
		case VISA_RESPONSE_CODE:
			/*EBE150917
			if (  memcmp(SITE_REG,"E",1) == 0   )
			{
			GetIsoField (ISO_RESPONSE_CODE, IsoInfo, isoResponse, &tmpFieldLen);
			TranslateRespCode (nIndexCtx,ResourceId, isoResponse, "PR", tmpField);
			AddVisaField (VISA_RESPONSE_CODE, NewVisaInfo, tmpField, 2);
			}
			else
			AddVisaField (VISA_RESPONSE_CODE, NewVisaInfo, "00", 2);
			*/
#if VISA_EU
			GetIsoField (ISO_RESPONSE_CODE, IsoInfo, isoResponse, &tmpFieldLen);
			TranslateRespCode (nIndexCtx,ResourceId, isoResponse, "PR", tmpField);
			AddVisaField (VISA_RESPONSE_CODE, &NewVisaInfo, tmpField, 2);
#else
			AddVisaField (VISA_RESPONSE_CODE, &NewVisaInfo, "00", 2);
			memcpy(isoResponse,"000",P7_F039_LEN);
#endif
			break;
		case VISA_ADTNL_RESP_DATA:
			memset(FieldData, 0, sizeof(FieldData));
			FieldData[0] = '5';
			AddVisaField(VISA_ADTNL_RESP_DATA, &NewVisaInfo, FieldData, strlen(FieldData));
			break;
		case VISA_ORIG_DATA:
			if (GetVisaField(VISA_ORIG_DATA, OrigVisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
				AddVisaField(VISA_ORIG_DATA, &NewVisaInfo, tmpField, tmpFieldLen);
			break;
		case VISA_REPLACE_AMOUNTS:
			if (GetVisaField(VISA_REPLACE_AMOUNTS, OrigVisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
				AddVisaField(VISA_REPLACE_AMOUNTS, &NewVisaInfo, tmpField, tmpFieldLen);
			break;
		default:
			if (VisaEchoedField_0400[i] == 1)
			{
				if (GetVisaField(i, OrigVisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
					AddVisaField(i, &NewVisaInfo, tmpField, tmpFieldLen);
			}
			break;
		}
	}


	/* send response to network */
	Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, &NewVisaInfo);
	VisaBuildHeader(RESPONSE, &(NewVisaInfo.sHeader), &(OrigVisaInfo->sHeader), Length, StationId);
	memcpy(sBuffer, (char *) &(NewVisaInfo.sHeader), VISA_HDR_LEN);
	Length += VISA_HDR_LEN;

	DumpVisa(sBuffer);
	if (WriteLineMsg(sBuffer, Length))
	{
		trace_message ( P7_TL_ERROR,"Error when calling WriteLineMsg(): [Length:%d]",Length);
		trace_message(P7_TL_INFO,"End ReversalRepToNw(NOK)");
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}


	/* YDA20171225 (PROD00048309) */
	/*MsgRepSent(IsoInfo->msgId,isoResponse);*/
	MsgIsoRepSent(IsoInfo->msgId,IsoInfo);

	trace_event("End  ReversalRepToNw(OK)", PROCESSING);
	return (OK);
}



/*****************************************************************************/
/*****************************************************************************/

/* This Function is not used */

int
	ForwardRevRequest(TSIsoInfo * IsoInfo, TSVisaInfo * VisaInfo)
{
	TSBalExchange   BalMessage;
	TSNodeData      Node;
	time_t          PurgeTime;
	char            tmpField[256];
	char            SourceId[7];
	char            sBuffer[1024];
	int             tmpFieldLen;
	int             Length;


	/* Build node to insert into list */
	InitNodeData(&Node);
	GetAcquirerPurgeTime(&PurgeTime);
	Node.tPurgeTime = PurgeTime;

	GetVisaField(VISA_AUDIT_NBR, VisaInfo, tmpField, &tmpFieldLen);
	memcpy(Node.szFstKey, "0400", 4);
	memcpy(Node.szFstKey + 4, tmpField, 6);

	VisaHeaderToBuffer(VisaInfo, Node.sFstBuf);
	memset(sBuffer, 0, sizeof(sBuffer));
	Length = VisaBuildMsg(sBuffer, VisaInfo);
	memcpy(Node.sFstBuf + VISA_HDR_LEN, sBuffer, Length);

	Node.nFstBufLen = Length + (int) VisaInfo->sHeader.bHeaderLength;

	if (InsertRequest('L', &Node) != OK)
	{
		trace_event("ForwardRevRequest(): InsertRequest() failed.", ERROR);
		return (NOK);
	}
	/* Add issuer purge time to Security Data Field (53) */
	GetIssuerPurgeTime(&PurgeTime);
	GetIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen);
	sprintf(tmpField + 49, "%09ld", PurgeTime);
	PutIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField, tmpFieldLen + 9);

	/* Put message in queue */
	GetIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen);
	memcpy(SourceId, tmpField + 10, 6);
	SourceId[6] = '\0';

	BalMessage.sHeader.nPartnerId = atoi(SourceId);
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = 1420;
	Length = IsoBuildMsg(BalMessage.sBody, IsoInfo);
	BalMessage.sHeader.nLen = Length;

	if (WriteBalMsg(nBalMsg, &BalMessage) != OK)
	{
		/* free request from list */
		FetchRequest('L', &Node);

		/*
		* send reply to network to inform it of failure to process
		* request
		*/
		/* Response = Issuer Not Available */
		InsertVisaField(VISA_RESPONSE_CODE, VisaInfo, B1_F039_ISS_UNV, B1_F039_LEN);
		Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, VisaInfo);
		VisaBuildHeader(RESPONSE, &(VisaInfo->sHeader), NULL, Length, StationId);
		memcpy(sBuffer, (char *) &(VisaInfo->sHeader), VISA_HDR_LEN);
		Length += VISA_HDR_LEN;


		dump_buffer(sBuffer, Length, 'E', 'L', 'O');

		DumpVisa(sBuffer);
		if (WriteLineMsg(sBuffer, Length))
		{
			trace_event("ForwardRevRequest(): WriteLineMsg() failed.", ERROR);
			return (NOK);
		}
	}
	return (OK);
}
