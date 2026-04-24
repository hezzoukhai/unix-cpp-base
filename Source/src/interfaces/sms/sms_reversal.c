#include <stdio.h>
#include <sys/time.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <sms_var.h>
#include <sms_param.h>
#include <sms_ictag.h>
#include <iso_cps.h>
#include <iso_puf.h>
#include <tlv_private.h>
#include <tlv_autho.h>
#include <define.h>
#include <list_thr.h>
#include <queue.h>
#include <struct_line.h>
#include <p7_sid.h>
#include <sms_define.h>
#include <visa_define.h>
#include <security_data.h>
#include <p7_define.h>
#include <iso8583.h>/*ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
#include <nw_raw_msg.h>
#include <sms_puf_fields.h>
#include <sms_puf.h>


static int             SmsEchoedField_0400[] =
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
	1,				/* STLMNT_DATE           */
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


/*char *RightAdjust(char *sSrc, int nLen);*/

/****************************************************************************/
/* FCT_REV_REQ_FROM_NW()													*/
/*																			*/
/* Processes a reversal request (0400) received from SMS.					*/
/****************************************************************************/
int FCT_REV_REQ_FROM_NW(int nIndexCtx, TSSmsInfo * SmsInfo)
{
	TSSmsInfo      NewSmsInfo;
	TSIsoInfo       IsoInfo;
	int             result;
	int             Length;
	char            sBuffer[1024];
	char            sPid[8 + 1];
	char			sTlvBuffer[LG_MAX];



	trace_event("Start  FCT_REV_REQ_FROM_NW()", PROCESSING);

	memset(sBuffer, 0, sizeof(sBuffer));


	/* Convert message to ISO format */
	result = ReversalReqFromNw(nIndexCtx, SmsInfo, &IsoInfo);

	IsoInfo.msgId = MsgIsoReqReceived(ResourceId,&IsoInfo);

	result = ServiceProcessing(		nIndexCtx,
									ResourceId,
									"ASM",
									AcquirerBank,
									"Y",
									"G",
									"N",
									sTlvBuffer,
									&IsoInfo);

	/*Start IBO20200517 RAWMSG*/
	/*if (g_sCaptureMsg[NW_INC_REQ] == 'Y')
	{
		LogVisaMessage(nIndexCtx, SmsInfo, &IsoInfo, INC_REQ);
	}*/
	/*End IBO20200517 RAWMSG*/

	switch (result) {

	case OK:		/* the interface itself responds */
		if (ReversalRepToNw(nIndexCtx, &IsoInfo, SmsInfo) != OK) {
			trace_message (P7_TL_ERROR,"Error when calling ReversalRepToNw(): [IndexCtx:%d]",nIndexCtx);
			trace_message(P7_TL_INFO,"End FCT_REV_REQ_FROM_NW(NOK)");
			return (NOK);
		}
		trace_message(P7_TL_PROCESSING,"End FCT_AUT_ADV_FROM_NW(OK): [nIndexCtx:%d]", nIndexCtx);
		break;
	case NOK:
		if (ReversalRepToNw(nIndexCtx, &IsoInfo, SmsInfo) != OK) {
			trace_message (P7_TL_ERROR,"Error when calling ReversalRepToNw(): [IndexCtx:%d]",nIndexCtx);
			trace_message(P7_TL_INFO,"End FCT_REV_REQ_FROM_NW(NOK)");
			return (NOK);
		}
		trace_message(P7_TL_PROCESSING,"End FCT_AUT_ADV_FROM_NW(NOK): [nIndexCtx:%d]", nIndexCtx);
		break;

	case SEVERE_ERROR:
		if (ReversalRepToNw(nIndexCtx, &IsoInfo, SmsInfo) != OK) {
			trace_message (P7_TL_ERROR,"Error when calling ReversalRepToNw(): [IndexCtx:%d]",nIndexCtx);
			trace_message(P7_TL_INFO,"End FCT_REV_REQ_FROM_NW(NOK)");
			return (NOK);
		}
		trace_message (P7_TL_ERROR,"Error when calling ServiceProcessing(): [IndexCtx:%d, ResourceId:%s]",nIndexCtx, ResourceId);
		trace_message(P7_TL_INFO,"End FCT_REV_REQ_FROM_NW(NOK):SEVERE_ERROR");
		return (NOK);
	}

	trace_event("End   FCT_REV_REQ_FROM_NW(OK)", PROCESSING);
	return (OK);
}
/****************************************************************************/
/* ReversalReqFromNw()                                                    */
/* */
/* Converts a received reversal request message (0400) from BASEI         */
/* into an ISO-format message for processing.                             */
/* Input:  SmsInfo: received message                                     */
/* Output: IsoInfo:  generated ISO message                                */
/****************************************************************************/
int
	ReversalReqFromNw(int nIndexCtx,
	TSSmsInfo * SmsInfo,
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
	char            SmsOrigData[42 + 1];
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
	TSPufInfo		PufInfo;
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


	IsoInfo->nMsgType = 1420;

	if (GetSmsField(SMS_CPS_FIELDS, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseCps(tmpField, &CpsInfo);
	}

	InitPufInfo (&PufInfo);
	if(GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AnalysePuf (tmpField, &PufInfo);

	memcpy(sAuthId , ResourceId, 2);        /*AMER 20160816*/

	/* Field 2: Primary Account Number */
	if (GetSmsField(SMS_CARD_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_CARD_NBR, IsoInfo, tmpField, tmpFieldLen);

	/* Field 3: Processing Code */
	GetSmsField(SMS_PROC_CODE, SmsInfo, ProcCode, &tmpFieldLen);
	GetSmsField(SMS_MERCHANT_TYPE, SmsInfo, MerchantType, &tmpFieldLen);
	SmsToIsoProcessingCode(ProcCode, MerchantType, tmpField);
	AddIsoField(ISO_PROC_CODE, IsoInfo, tmpField, 6);

	/* Field 4: Transaction Amount */
	if (GetSmsField(SMS_TRANS_AMOUNT, SmsInfo, sTrxAmount, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_TRANS_AMOUNT, IsoInfo, sTrxAmount, 12);

	/* Field 6: Cardholder Billing Amount */
	if (GetSmsField(SMS_BILLING_AMOUNT, SmsInfo, sBillingAmount, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_BILLING_AMOUNT, IsoInfo, sBillingAmount, 12);
	else if( strlen(sTrxAmount) > 0 )
	{
		AddIsoField(ISO_BILLING_AMOUNT, IsoInfo, sTrxAmount, 12);
	}

	/* Field 7: Transmission Date and Time */
	if (GetSmsField(SMS_XMIT_TIME, SmsInfo, sXmitDateTime, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_XMIT_TIME, IsoInfo, sXmitDateTime, 10);

	/* Field 10: Cardholder Billing Conversion Rate */
	if (GetSmsField(SMS_BILLING_CNVRSN_RATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_BILLING_CNVRSN_RATE, IsoInfo, tmpField, 8);

	/* Field 11: System Trace Audit Number */
	GetInternalStan(nIndexCtx, sStan);
	AddIsoField(ISO_AUDIT_NBR, IsoInfo, sStan, 6);

	GetSmsField(SMS_AUDIT_NBR, SmsInfo, sExternalStan, &tmpFieldLen);	/*AMER20171124 PROD00049633*/

	/* Field 12: Transaction local date and time  */
	/* ELKAM20221214 PLUTONL-5363 START */
	current_gmt_date (tmpField);
	AddIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField, 12);
	/* ELKAM20221214 PLUTONL-5363 END */

	/* Field 14: Expiry Date */
	if (GetSmsField(SMS_EXPIRY_DATE, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_EXPIRY_DATE, IsoInfo, tmpField, 4);

	/* Field 18: Merchant Type */
	if(strlen(MerchantType) > 0)
	{
		AddIsoField(ISO_MERCHANT_TYPE, IsoInfo, MerchantType, 4);
	}

	/* Field 19: Acquiring Institution Country Code */
	if (GetSmsField(SMS_ACQR_COUNTRY, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		AddIsoField(ISO_ACQR_COUNTRY, IsoInfo, tmpField, 3);
	}

	/* Field 20: PAN Country Code */
	if (GetSmsField(SMS_PAN_COUNTRY, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, 3);

	/* Field 22: POS Entry Mode */
	memset(PosMode, 0, sizeof(PosMode));
	memset(PosCond, 0, sizeof(PosCond));
	memset(AddPos, 0, sizeof(AddPos));

	if (
		(GetSmsField(SMS_POS_ENTRY_MODE, SmsInfo, PosMode, &tmpFieldLen) == SUCCESS) &&
		(GetSmsField(SMS_POS_CONDITION, SmsInfo, PosCond, &tmpFieldLen) == SUCCESS) &&
		(GetSmsField(SMS_ADTNL_POS_INFO, SmsInfo, AddPos, &tmpFieldLen) == SUCCESS)
		) {

			SmsToIsoPosMode(MerchantType, PosMode, PosCond, AddPos, tmpField, sEcomAuthInfo, &nEcomInfoLen); /*IBO20221025 PLUTONL-5272*/
			AddIsoField(ISO_POS_DATA, IsoInfo, tmpField, 12);
			pos_flag = 1;
	}

	/* Field 24: Function Code */
	if (GetSmsField(SMS_REPLACE_AMOUNTS, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
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
	if (GetSmsField(SMS_REPLACE_AMOUNTS, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		if ((GetSmsField(SMS_OTHER_AMOUNTS, SmsInfo, tmpField1, &tmpFieldLen) == SUCCES)
			&& (tmpFieldLen == 36)
			&& (memcmp(tmpField1 + 24, "000000000000", 12) != 0)
			)
			memcpy(tmpField + 12, tmpField1 + 12 + 12, 12);
		else
			memcpy(tmpField + 12, "000000000000", 12);
		AddIsoField(ISO_ORIGINAL_AMOUNTS, IsoInfo, tmpField, 24);
	}
	/* Field 32: Acquiring Institution Identification Code */
	if (GetSmsField(SMS_ACQR_ID, SmsInfo, sAcquirerId, &nAcquirerIdLen) == SUCCES)
		AddIsoField(ISO_ACQR_ID, IsoInfo, sAcquirerId, nAcquirerIdLen);

	/* Field 33: Forwarding Institution Id */
	if (GetSmsField(SMS_FORWD_ID, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_FORWD_ID, IsoInfo, tmpField, tmpFieldLen);

	/* Field 37: Retrieval Reference Number */
	if (GetSmsField(SMS_REFERENCE_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		AddIsoField(ISO_REFERENCE_NBR, IsoInfo, tmpField, 12);
		memcpy(sAuthId + 2 + P7_F011_LEN, tmpField, P7_F037_LEN);	/*AMER 20160816*/
	}

	/* Field 38: Authorization Id */
	if (GetSmsField(SMS_AUTHOR_ID, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_AUTHOR_ID, IsoInfo, tmpField, 6);

	/* Field 39: Response Code */
	if (GetSmsField(SMS_RESPONSE_CODE, SmsInfo, RespCode, &tmpFieldLen) == SUCCESS)
	{
		TranslateRespCode(nIndexCtx, ResourceId, RespCode, "RP", tmpField);
		AddIsoField(ISO_RESPONSE_CODE, IsoInfo, tmpField, P7_F039_LEN);
	}
	/* Field 41: Terminal Identification */
	if (GetSmsField(SMS_TERMINAL_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_TERMINAL_NBR, IsoInfo, tmpField, 8);


	/* Field 42: Card Acceptor Identification Code */
	if (GetSmsField(SMS_OUTLET_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddIsoField(ISO_OUTLET_NBR, IsoInfo, tmpField, 15);


	/* Field 43: Card Acceptor Location */
	if (GetSmsField(SMS_TERMINAL_ADR, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		SmsToIsoAcceptorName(nIndexCtx ,tmpField,sIsoTermAddr);
		AddIsoField(ISO_TERMINAL_ADR, IsoInfo, sIsoTermAddr, tmpFieldLen);

	}


	memset(tmpField1, 0, sizeof(tmpField1));
	memcpy(tmpField1, tmpField, strlen(tmpField));



	/* Field 48: Additional Private Data */

	InitTlvPrivate(&TlvInfo);

	AddTlvPrivate(HPS_ACQUIRER_BANK_TAG, &TlvInfo, AcquirerBank, 6);

	if (GetSmsField(SMS_AUDIT_NBR, SmsInfo, tmpField, &tmpFieldLen) == SUCCES)
	{
		AddTlvPrivate(EXTERNAL_STAN_TAG, &TlvInfo, tmpField, 6);
		/*memcpy(sAuthId + 2, tmpField, P7_F011_LEN);*/
	}

	/*AMER 20160714:PROD00029686: We build the AuthId with the stan included in DE 90 instead of DE 11*/
        if (GetSmsField(SMS_ORIG_DATA, SmsInfo, SmsOrigData, &tmpFieldLen) == SUCCES)
        {
                /* Original Stan */
                if (memcmp(SmsOrigData + 4, "000000", 6) == 0)
                        memcpy(sAuthId + 2, sExternalStan /*sStan*/, 6);		/*AMER20171124 PROD00049633: We should use external stan instead of internal stan*/
                else
                        memcpy(sAuthId + 2, SmsOrigData + 4, P7_F011_LEN);
        }
	else
		memcpy(sAuthId + 2, sExternalStan, 6); /* [ASO17012020] PROD00073688 */

	/*SmsBuildAuthId(SmsInfo,sAuthId);*/	/*AMER 20160714: To don't be used anymore. We construct the AuthId above*/
	AddTlvPrivate(AUTHORIZATION_IDENTIFIER, &TlvInfo, sAuthId, strlen(sAuthId));
	if (GetCpsSubField(CPS_TRANS_ID, &CpsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		strncpy(sTrnId,tmpField,sizeof(sTrnId));
		AddTlvPrivate(TRANSACTION_IDENTIFIER, &TlvInfo, sTrnId, strlen(sTrnId));
	}

	if (GetSmsField(SMS_ADTNL_DATA_PRIV, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS) {
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
	if (GetSmsField(SMS_TRANS_CRNCY, SmsInfo, sTrxCurrency, &tmpFieldLen) == SUCCES)
		AddIsoField(ISO_TRANS_CRNCY, IsoInfo, sTrxCurrency, 3);

	/* Field 51: Cardholder Billing Currency Code */
	if (GetSmsField(SMS_BILLING_CRNCY, SmsInfo, sBillingCurrency, &tmpFieldLen) == SUCCESS)
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
	PutSecurityData(USER, tmpField1, "SMS           ", 15);
	PutSecurityData(PURGE_TIME, tmpField1, "000000000", 9);
	AddIsoField(ISO_SECURITY_DATA, IsoInfo, tmpField1, strlen(tmpField1));

	/* Field 56: Original Data */
	if (GetSmsField(SMS_ORIG_DATA, SmsInfo, SmsOrigData, &tmpFieldLen) == SUCCES)
	{
		memset(IsoOrigData, 0, sizeof(IsoOrigData));
		memset(IsoOrigData, '0', 35);

		Offset = 0;
		/* Original Type de Message */
		memset(sMti, 0, sizeof(sMti));
		memcpy(sMti, SmsOrigData, 4);
		sprintf(sMti, "%4d", atoi(sMti) + 1000);
		memcpy(IsoOrigData + Offset, sMti, 4);
		Offset += 4;

		/* Original Stan */
		if (memcmp(SmsOrigData + 4, "000000" /*"OOOOOO"*/, 6) == 0) 	/*AMER20171124 PROD00049633: We should use 0 instead of O*/
			memcpy(IsoOrigData + Offset, sExternalStan /*sStan*/, 6);	/*AMER20171124 PROD00049633: We should use external stan instead of internal stan*/
		else
			memcpy(IsoOrigData + Offset, SmsOrigData + 4, 6);
		Offset += 6;

		/* Original Xmit */
		memcpy(IsoOrigData + Offset, "00", 2);
		Offset += 2;

		if (memcmp(SmsOrigData + 4 + 6, "0000000000", 10) == 0)
			memcpy(IsoOrigData + Offset, sXmitDateTime, 10);
		else
			memcpy(IsoOrigData + Offset, SmsOrigData + 4 + 6, 10);
		Offset += 10;

		/* Original Acquirer Id */
		sprintf(sAcquirerIdLen, "%02d", nAcquirerIdLen);
		memcpy(IsoOrigData + Offset, sAcquirerIdLen, 2);
		Offset += 2;

		if (memcmp(SmsOrigData + 4 + 6 + 10, "00000000000", 11) == 0)
			memcpy(IsoOrigData + Offset + 11 - nAcquirerIdLen, sAcquirerId, nAcquirerIdLen);
		else
			memcpy(IsoOrigData + Offset, SmsOrigData + 4 + 6, 11);

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
	if (GetSmsField(SMS_CPS_FIELDS, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
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
	if (GetPufSubField (PUF_STIP_REASON, &PufInfo, tmpField, &tmpFieldLen) == SUCCESS)
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



/****************************************************************************/
/*  FCT_REV_ACQ_FROM_NW()                                                   */
/*                                                                          */
/*  Receives a Reversal Advice message (420) from SMS after reconnection.   */
/****************************************************************************/
/*int FCT_REV_ACQ_FROM_NW (int  nIndexCtx, TSSmsInfo  *SmsInfo)
{
	TSSmsInfo       NewSmsInfo;
	TSPufInfo    PufInfo;
	TSPufInfo    NewPufInfo;
	char         tmpField   [BUF_LG_MAX];
	char         sSmsSndBuf [LG_MAX];
	int          FieldLen;
	int          FieldLen1;
	int          nMsgLen;
	int          nRetCode;
	int    i;

	trace_event("Start FCT_REV_ACQ_FROM_NW ()", PROCESSING);

	InitSmsInfo (&NewSmsInfo);

	memset (tmpField,    0, sizeof (tmpField));
	memset (sSmsSndBuf,  0, sizeof (sSmsSndBuf));

	NewSmsInfo.nMsgType = 430;

	for( i=0; i<SMS_PRIV_USE_FIELDS;i++ )
	{
		if ( (i != SMS_TRANS_AMOUNT) &&
			(i != SMS_TRANS_TIME) &&
			(i != SMS_TRANS_DATE) &&
			(i != SMS_MERCHANT_TYPE) &&
			(i != SMS_POS_ENTRY_MODE) &&
			(i != SMS_AUTHOR_ID) &&
			(i != SMS_TERMINAL_ADR) &&
			(i != SMS_ADTNL_RESP_DATA)
			)
		{
			if ( GetSmsField (i, SmsInfo, tmpField, &FieldLen) == SUCCESS)
				AddSmsField (i, &NewSmsInfo, tmpField, FieldLen);
		}
	}

	/* Field 63: Private Use Fields *
	InitPufInfo (&PufInfo);
	InitPufInfo (&NewPufInfo);
	GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, &FieldLen);
	AnalysePuf (tmpField, &PufInfo);

	GetPufSubField (PUF_NETWORK_ID, &PufInfo, tmpField, &FieldLen1);
	AddPufSubField (PUF_NETWORK_ID, &NewPufInfo, tmpField, FieldLen1);

	FieldLen1 = PufBuildField (tmpField, &NewPufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, &NewSmsInfo, tmpField, FieldLen1);


	for( i=SMS_MAC1; i<128;i++ )
	{
		if ( GetSmsField (i, SmsInfo, tmpField, &FieldLen) == SUCCESS)
			AddSmsField (i, &NewSmsInfo, tmpField, FieldLen);
	}

	/************** Send the request to Network  ************************
	nMsgLen=SmsBuildMsg(sSmsSndBuf + SMS_HDR_LEN, &NewSmsInfo);
	SmsBuildHeader(RESPONSE, &(NewSmsInfo.sHeader), &(SmsInfo->sHeader), nMsgLen, StationId );
	memcpy(sSmsSndBuf, (char *) &(NewSmsInfo.sHeader), SMS_HDR_LEN);
	nMsgLen += SMS_HDR_LEN;

	DumpSms (sSmsSndBuf);

	dump_buffer(sSmsSndBuf, nMsgLen, 'A', 'L', 'O');

	if (WriteLineMsg(sSmsSndBuf, nMsgLen) != OK)
	{
		trace_event ("Function FCT_REV_ACQ_FROM_NW (): WriteLineMsg() failed", PROCESSING);
		return (NOK);
	}

	trace_event("End   FCT_REV_ACQ_FROM_NW (OK)", PROCESSING);
	return(OK);

}*/

/****************************************************************************/
/*   FCT_REV_ADV_TO_NW()                                                    */
/*                                                                          */
/*  Sends a Reversal Advice message (420) to SMS.                           */
/****************************************************************************/
int FCT_REV_ADV_TO_NW (int  nIndexCtx, TSIsoInfo  *IsoInfo, char *sTlvBuffer)
{
	TSSmsInfo	SmsInfo;
	TSNodeData	node;
	TSBalExchange   sBalMsg;
	char   		data      [LG_MAX];
	char   		tmpField  [LG_MAX];
	char   		sPurgeTime[10];
	char            DestId    [7];
	char 		sBuffer   [LG_MAX];
	char 		sLine     [MAX_LINE_TRC];
	int		tmpFieldLen;
	int 		nLength;
	int		length;
	int		result;
	int		nBalMsg;
	int		retour;
	struct timeval  stPurgeTime;
	char            sActionCode[4];/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/

	trace_event ("Start FCT_REV_ADV_TO_NW()", PROCESSING);

	/**** Initialisation des variables et structures de travail ****/
	memset (data ,          0,    sizeof (data));
	memset (tmpField ,      0,    sizeof (tmpField));
	memset (sPurgeTime ,    0,    sizeof (sPurgeTime));
	memset (DestId ,        0,    sizeof (DestId));
	memset (sBuffer ,       0,    sizeof (sBuffer));
	memset (sLine ,         0,    sizeof (sLine));
	memset (sActionCode,    0,    sizeof (sActionCode));/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/

	InitSmsInfo (&SmsInfo);
	InitNodeData(&node);

	dump_p7_struct(IsoInfo);

	result = ReversalAdviceToSms (nIndexCtx, &SmsInfo, IsoInfo);

	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
	/*** Auto-reponse generee en cas de probleme de conversion ***/
	if (result != OK)
	{

		IsoInfo->nMsgType += 10;
		memcpy(sActionCode,MALFUNCTION_ACTION,3);
		PutIsoField (ISO_RESPONSE_CODE, IsoInfo, sActionCode, P7_F039_LEN);/*ZKO 26072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

		MsgFwdRepReplied(IsoInfo->msgId);

		trace_event ("End   FCT_REV_ADV_TO_NW(NOK): System Malfunction", PROCESSING);
		return (NOK);
	}
	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/

	/* Insert original ISO message in list for later retrieval */
	trace_event("Start Chainage",TRACE);
	GetSafPurgeTimeMS(&stPurgeTime); /*10062020 PLUTONL-1955*/
	node.stPurgeTime= stPurgeTime ;

	GetSmsField (SMS_AUDIT_NBR, &SmsInfo, data, &length);
	memcpy (node.szFstKey, "1420", 4);
	memcpy (node.szFstKey + 4,  data, 6);

	memcpy (node.sSndPrvtData + 0 , ResourceId    , 6 );
	node.nSndPrvtDataLen = 6;

	memcpy (node.sFstPrvtData , "SAF" , 3);
	node.nFstPrvtDataLen = 3;

	nLength = IsoBuildMsg (node.sFstBuf, IsoInfo);
	node.nFstBufLen = nLength;

	node.nSndBufLen = strlen ( sTlvBuffer );
	memcpy (node.sSndBuf, sTlvBuffer , node.nSndBufLen);

	if (InsertRequest('B', &node) != OK)
	{
		/* Put message in queue */
		sprintf (tmpField, "%.3d", SYSTEM_MALFUNCTION);
		InsertIsoField (ISO_RESPONSE_CODE, IsoInfo, tmpField, 3);
		GetIsoField (ISO_SECURITY_DATA, IsoInfo, tmpField, &tmpFieldLen);
		memcpy (DestId, tmpField + 10, 6);
		DestId[6] = '\0';

		sBalMsg.sHeader.nPartnerId = atoi(DestId);
		sBalMsg.sHeader.nSourceId = getpid();
		sBalMsg.sHeader.nCommand = 1430;
		nLength = IsoBuildMsg (sBalMsg.sBody, IsoInfo);
		sBalMsg.sHeader.nLen = nLength;

		if(WriteBalMsg (nBalMsg, &sBalMsg))
		{
			trace_event ("FCT_REV_ADV_TO_NW(NOK): InsertRequest(): WriteBal Failure", ERROR);
			return (NOK) ;
		}

		trace_event ("FCT_REV_ADV_TO_NW(NOK): InsertRequest() failure", ERROR);
		return (NOK) ;
	}

	/***** Send this buffer to the Network *****/
	nLength = SmsBuildMsg (sBuffer+SMS_HDR_LEN, &SmsInfo);

	SmsBuildHeader(REQUEST, &(SmsInfo.sHeader), NULL, nLength, StationId);

	memcpy(sBuffer, (char *) &(SmsInfo.sHeader), SMS_HDR_LEN);
	nLength += SMS_HDR_LEN;


	trace_event("==> Buffer To send to SMS <==", PROCESSING);
	DumpSms(sBuffer);


	dump_buffer(sBuffer, nLength, 'A', 'L', 'O');

	if (WriteLineMsg(sBuffer, nLength) != OK)
	{
		/* free request from list */
		FetchRequest ('B', &node);
		trace_event ("End  FCT_REV_ADV_TO_NW (NOK): WriteLineMsg Failure", PROCESSING);
		return (NOK) ;
	}

	trace_event ("WriteLineMsg (OK)", PROCESSING);
	trace_event ("End  FCT_REV_ADV_TO_NW (OK)", PROCESSING);
	return (OK);
}

/***********************************************************************/
/*  FCT_REV_REP_FROM_NW()                                              */
/*                                                                     */
/*  Receives a Reversal Advice Response Message (430) from SMS.        */
/***********************************************************************/
int FCT_REV_REP_FROM_NW (int nIndexCtx, TSSmsInfo	*SmsInfo)
{
	TSIsoInfo		IsoInfo;
	TSNodeData		node;
	TSBalExchange	        BalMessage;

	char			sBuffer   [LG_MAX];
	char			tmpField  [LG_MAX];
	char			data      [LG_MAX];
	char                    RespCode  [3 + 1];
	char			sLine     [MAX_LINE_TRC];
	char                    sTlvBuffer[LG_MAX];
	char 			PartnerId [7];
	int 			Length;
	time_t 			temps;

	trace_event ("Start FCT_REV_REP_FROM_NW()", PROCESSING);

	/**** Initialisation des variables de travail ****/
	memset (sBuffer,              0, sizeof (sBuffer));
	memset (tmpField,             0, sizeof (tmpField));
	memset (RespCode,             0, sizeof (RespCode));
	memset (sTlvBuffer,           0, sizeof (sTlvBuffer));
	memset (data,                 0, sizeof (data));
	memset (PartnerId,            0, sizeof (PartnerId));

	GetSmsField (SMS_AUDIT_NBR, SmsInfo, data, &Length);

	InitNodeData (&node);
	memcpy (node.szFstKey, "1420", 4);
	memcpy (node.szFstKey + 4,  data, 6);
	sprintf(sLine,"Cle de dechainage REP Is :%s", node.szFstKey);
	trace_event(sLine,TRACE);

	switch (FetchRequest ('B', &node))
	{
	case ENOEXIST:
		trace_event ("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event ("End FCT_REV_REP_FROM_NW(NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;
	}

	InitIsoInfo (&IsoInfo);
	AnalyseIso (node.sFstBuf, &IsoInfo);
	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	memcpy ( sTlvBuffer , node.sSndBuf, node.nSndBufLen );

	/* ReversalResponseFromSms(nIndexCtx, SmsInfo, &IsoInfo); */

	/* Field 5 */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, tmpField, &Length) == SUCCES)
		InsertIsoField (ISO_STLMNT_AMOUNT, &IsoInfo, tmpField, Length);

	/**** Add Sms Response ****/
	if (GetSmsField (SMS_RESPONSE_CODE, SmsInfo, RespCode, &Length) == SUCCESS)
	{
		TranslateRespCode (nIndexCtx, ResourceId, RespCode, "RP", tmpField);
		InsertIsoField (ISO_RESPONSE_CODE, &IsoInfo, tmpField, 3);
	}

	SafResponseTraitement ( nIndexCtx,  sTlvBuffer ,
		"1430", RespCode, tmpField );

	/*Start IBO20200517 RAWMSG*/
	/*if (g_sCaptureMsg[NW_INC_RESP] == 'Y')
	{
		LogVisaMessage(nIndexCtx, SmsInfo, &IsoInfo, INC_RESP);
	}*/
	/*End IBO20200517 RAWMSG*/

	trace_event ("End FCT_REV_REP_FROM_NW (OK)", PROCESSING);
	return (OK);
}

/***************************************************************************/
/*  ReversalAdviceToSms()                                                  */
/*                                                                         */
/*  Prepares an SMS reversal advice message.                               */
/*  Params: IsoInfo: ISO-formatted message                                 */
/*          SmsInfo: SMS message to be sent                                */
/***************************************************************************/
int ReversalAdviceToSms(int  nIndexCtx, TSSmsInfo   *SmsInfo, TSIsoInfo   *IsoInfo)
{
	char    tmpField              [BUF_LG_MAX];
	char    tmpField1             [BUF_LG_MAX];
	char    tmpField2			  [1024];
	int     tmpFieldLen;
	char    CardNumber[20];
	char    sCardAccNameAddress   [40];
	char    sAbrevBankName        [16];
	char    sOrigReference        [12 + 1];
	char    isoPosMode            [12 + 1];
	char    isoProcCode           [6 + 1];
	char    sMcc                  [4 + 1];
	char    sCpsId                [1 + 1];
	char    sLine                 [MAX_LINE_TRC];
	char	sIccBuffer[BUF_LG_MAX];
	char         FieldData[256];
	int     nRetCode;
	int          FieldLen;
	int          IsChipTrx;
	char		szAcquirerBank[6 + 1];
	char		szAcquirerId[12 + 1];
	char         sFunctionCode[4];	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	TSTagSms     SmsIccInfo;
	TSTagIso      IsoIccInfo;
	TSPufInfo    PufInfo;
	TSCpsInfo    CpsInfo;
	TSTlvPrivate kTlvInfo;
	char            sRefNbr[12 + 1];
	char            szDate[12 + 1];
	char            szWork[256 + 1];
	char            sAcqId[32];
	char            exStan[6 + 1];
	/*Start HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
	char          	TrxAmount[32];
	int 			nTrxAmount ;
	int 			trxFee;
	/*End HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
	/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	TlvInfo			kSmsRecurringPayTlv;
	TlvInfo			kIsoRecurringPayTlv;
	TlvInfo			kSmsTransSpecTlv;
	char            sAddAuthInfo[P7_F048_P64_LEN + 1];
	char			RecurringFlag[2];
	int				nNbrTags = 0;		/*ZKO 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	trace_event("Start ReversalAdviceToSms()", PROCESSING);

	/****** Initialisation des variables et structures de travail *******/
	memset (tmpField,              0,     sizeof (tmpField));
	memset (tmpField1,             0,     sizeof (tmpField1));
	memset (CardNumber,            0,     sizeof (CardNumber));
	memset (sAbrevBankName,        0,     sizeof (sAbrevBankName));
	memset (sCardAccNameAddress,   0,     sizeof (sCardAccNameAddress));
	memset (isoProcCode,           0,     sizeof (isoProcCode));
	memset (isoPosMode,            0,     sizeof (isoPosMode));
	memset (sOrigReference,        0,     sizeof (sOrigReference));
	memset (sMcc,                  0,     sizeof (sMcc));
	memset (sCpsId,                0,     sizeof (sCpsId));
	memset (sIccBuffer  ,			0, sizeof(sIccBuffer));
	memset (FieldData,              0,     sizeof (FieldData));
	memset (szAcquirerBank,    0, sizeof(szAcquirerBank));
	memset (szAcquirerId,    0, sizeof(szAcquirerId));
	memset (exStan , '0', sizeof(exStan));
	memset (TrxAmount     ,    0, sizeof(TrxAmount));/*15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
	memset (sAddAuthInfo, 0, sizeof(sAddAuthInfo));	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	memset(RecurringFlag, 0, sizeof(RecurringFlag));	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	memset(sFunctionCode, 0, sizeof(sFunctionCode));	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/


	IsChipTrx = 0;

	/* Message Type is 420 */
	SmsInfo->nMsgType = 420;
	/*Start SNO070116 */
	InitTlvPrivate (&kTlvInfo);
	InitSmsRecurringPayDataTlv(&kSmsRecurringPayTlv);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	IniIsoInstallmentLoanTlv(&kIsoRecurringPayTlv);		/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	InitSmsTransSpecDataTlv(&kSmsTransSpecTlv);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	if (GetIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvPrivate (tmpField, tmpFieldLen, &kTlvInfo);
	}
	/*End SNO070116 */
	/* Field 2: Primary Account Number */
	if (GetIsoField (ISO_CARD_NBR, IsoInfo, CardNumber, &tmpFieldLen)  == SUCCESS)
		AddSmsField (SMS_CARD_NBR, SmsInfo, CardNumber, tmpFieldLen);

	/* Field 3: Processing Code */
	memcpy(sMcc,"0000",4);
	GetIsoField (ISO_PROC_CODE, IsoInfo, isoProcCode, &tmpFieldLen);
	GetIsoField (ISO_MERCHANT_TYPE, IsoInfo, sMcc, &tmpFieldLen);
	IsoToSmsProcessingCode (isoProcCode, tmpField, sMcc);
	AddSmsField (SMS_PROC_CODE, SmsInfo, tmpField, 6);

	/* Field 4: Transaction Amount */
	/*NAB21062019 Certif SG ,we should'nt send F.4 at reversal of PIN change and Unblock*/

	if((memcmp (isoProcCode, "91", 2)!=0)&&(memcmp (isoProcCode, "98", 2)!=0))
	{
		/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/ /*the value tmpField changed to TrxAmount to compare it with ISO_FEES_AMOUNT*/
		if(GetIsoField (ISO_TRANS_AMOUNT, IsoInfo, TrxAmount, &tmpFieldLen) ==SUCCESS)
		{
			AddSmsField (SMS_TRANS_AMOUNT, SmsInfo, TrxAmount, 12);
		}

	}

	/* Field 7: Transmission Date & Time */
	GetIsoField (ISO_XMIT_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_XMIT_TIME, SmsInfo, tmpField, 10);

	/* Field 11: Trace Audit Number */
	/*START NAB26042019 Certif SG: faut le recuperer depuis l origine iso (56)*/
	/*GetIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField + 6, 6);*/
	/*memset (tmpField1, 0, sizeof(tmpField1));
	memset (tmpField2, 0, sizeof(tmpField2));*/
	/*NAB02052019 concernant le revrsal de authenrification failure le 56 est remplis par le internal car on passe pas par la fonction :AUT_REVERSAL_PROCEDURE
	*/
	/*if (GetIsoField (ISO_ORIG_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AddSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField + 4, 6);
	}
	*/
	if(GetTlvPrivate (EXTERNAL_STAN_TAG, &kTlvInfo, exStan, &tmpFieldLen) == SUCCESS)
	{
			AddSmsField (SMS_AUDIT_NBR, SmsInfo, exStan, 6);

	}
	else
	{
		if (GetIsoField (ISO_ORIG_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AddSmsField (SMS_AUDIT_NBR, SmsInfo, tmpField + 4, 6);
		}
	}



	/*END NAB26042019*/
	/* Field 12: Transaction Local Time */
	GetIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_TIME, SmsInfo, tmpField + 6, 6);

	/* Field 13: Transaction Local Date */
	GetIsoField (ISO_TRANS_DATE_TIME, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_DATE, SmsInfo, tmpField + 2, 4);


    /* Field 14: Expiry Date */
	if (GetIsoField (ISO_EXPIRY_DATE, IsoInfo, tmpField, &tmpFieldLen)== SUCCESS )
	AddSmsField (SMS_EXPIRY_DATE, SmsInfo, tmpField,  4);


	/* Field 18: Merchant Type */
	GetIsoField (ISO_MERCHANT_TYPE, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_MERCHANT_TYPE, SmsInfo, tmpField, 4);

	/* Field 19: Acquiring Institution Country Code */

	/*AddSmsField (SMS_ACQR_COUNTRY, SmsInfo, CountryCode, 3);*//*NAB100052019 SG Certif */
	 GetIsoField(ISO_ACQR_COUNTRY, IsoInfo, tmpField, &tmpFieldLen);
	 AddSmsField (SMS_ACQR_COUNTRY, SmsInfo, tmpField, 3);

	/* Field 20: Pan Country Code  */
	if (!memcmp(CardNumber, "59", 2))
	{
		GetIsoField (ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen);
		AddSmsField (SMS_PAN_COUNTRY, SmsInfo, tmpField, tmpFieldLen);
	}

	/* Field 22: POS Entry Mode Code */
	/* 90: magnetic stripe read, 10: PIN accepted */
	GetIsoField (ISO_POS_DATA, IsoInfo, isoPosMode, &tmpFieldLen);
	IsoToSmsPosMode (isoPosMode, tmpField);
	AddSmsField (SMS_POS_ENTRY_MODE, SmsInfo, tmpField, 4);

	/* Field 25: POS Condition Code */
	/* 00: normal transaction */
	AddSmsField (SMS_POS_CONDITION, SmsInfo, "00", 2);


	/* Field 28: Amount Transaction Fee */
	/*NAB21062019 Certif SG ,we should'nt send F.28  at reversal of PIN change and Unblock*/


	if ( (memcmp (isoProcCode, "91", 2)!=0) && (memcmp (isoProcCode, "98", 2)!=0))
	{
		if (GetIsoField (ISO_FEES_AMOUNT, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AddSmsField (SMS_TRANS_FEE, SmsInfo, tmpField, 9);
		}
		else

			AddSmsField (SMS_TRANS_FEE, SmsInfo, "C00000000", 9);
	}

	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/
		if (GetSmsField (SMS_TRANS_FEE, SmsInfo, tmpField,&tmpFieldLen) == SUCCESS)
	{
		nTrxAmount  = atoi(TrxAmount);
		trxFee 		=	atoi(tmpField +1);

		if ( trxFee >= nTrxAmount && nTrxAmount > 0 ) /*IBO20201120 PLUTONL-2520*/
		{
			PutIsoField(ISO_RESPONSE_CODE, IsoInfo, INVALID_AMOUNT_ACTION, P7_F039_LEN);
			trace_event("End  BuildAutReqToNw 028 >= F004 Not Present(NOK)", PROCESSING);
			return (NOK);
		}
	}
	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 2.11*/

	/* Field 32: Acquiring Institution Identification Code */
	/*EBE
	AddSmsField (SMS_ACQR_ID, SmsInfo, AcquirerId, strlen (AcquirerId));*/
	if (GetTlvPrivate(ACQUIRER_BANK_TAG, &kTlvInfo, szAcquirerBank , &FieldLen) != SUCCESS)
	{
		trace_event("End BuildAutReqToNw (NOK) : ERROR Getting Acquirer bank",ERROR);
		return(NOK);
	}
	nRetCode = GetAquirerId( nIndexCtx, szAcquirerBank, NET_VISA, szAcquirerId)  ;
	if (nRetCode != OK)
	{
		sprintf(sLine,"End BuildAutReqToNw (NOK) : ERROR Getting AquirerId for bank[%s]",szAcquirerBank);
		trace_event(sLine,ERROR);
		return(NOK);
	}
	AddSmsField (SMS_ACQR_ID, SmsInfo, szAcquirerId, strlen(szAcquirerId));

	/* Field 37: Reference Number */
	/*START NAB26042019 Certif SG faut construire le champs 37 */
	/*GetIsoField (ISO_REFERENCE_NBR, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_REFERENCE_NBR, SmsInfo, tmpField, 12);*/
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
		memcpy(sAcqId+(11-strlen(szAcquirerId)), szAcquirerId  , strlen(szAcquirerId));
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
		/*memcpy(sRefNbr + 6, tmpField2 + 4     , 6);*//*NAB02052019 on passe pas par le AUT_REVERSAL_PROCEDURE donc le 56 contient le internal */
		memcpy(sRefNbr + 6, exStan , 6);

		PutSmsField (SMS_REFERENCE_NBR, SmsInfo, sRefNbr, 12);
	}
	/*END NAB26042019*/

	/* Field 38: Authorization Identification Response */
	if (GetIsoField (ISO_AUTHOR_ID, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_AUTHOR_ID, SmsInfo, tmpField, tmpFieldLen);

	/* Field 41: Terminal Identification */
	if (GetIsoField (ISO_TERMINAL_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AddSmsField (SMS_TERMINAL_NBR, SmsInfo, tmpField, tmpFieldLen);

	/* Field 42: Card Acceptor Identification Code */
	/*    if (GetIsoField (ISO_OUTLET_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)*/
	GetAbrevBankName (nIndexCtx, AcquirerBank, sAbrevBankName);
	AddSmsField (SMS_OUTLET_NBR, SmsInfo, sAbrevBankName, 15);

	/* Field 43: Card Acceptor Location */
	/*START NAB10052019 Certif SG -ne recuperer plus le champs 19 et 43 du sms_resours_param */
	/*memset (tmpField1,       ' ', sizeof(tmpField1));
	GetIsoField (ISO_TERMINAL_ADR, IsoInfo, tmpField, &tmpFieldLen);*/
/*
	if (tmpFieldLen > 25)
		memcpy (tmpField1, tmpField, 24);
	else
		memcpy (tmpField1, tmpField, tmpFieldLen);
*/
       /* if (tmpFieldLen > 37)
        {
                memcpy (tmpField1, tmpField, 23);
                memcpy (tmpField1 + 25, tmpField + 23, 13);
        }
        else
        {
                memcpy (tmpField1, tmpField, tmpFieldLen);
                memcpy (tmpField1 + 25, BankLocation, strlen(BankLocation));
        }*/


/*	memcpy (tmpField1 + 25, BankLocation, strlen(BankLocation));*/
	/*memcpy (tmpField1 + 38, AlphaCode2, 2);
	AddSmsField (SMS_TERMINAL_ADR, SmsInfo, tmpField1, 40);*/
	if (GetIsoField(ISO_TERMINAL_ADR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		replace(tmpField, '>', ' ', tmpFieldLen);
		replace(tmpField, '_', ' ', tmpFieldLen);
		AddSmsField(SMS_TERMINAL_ADR, SmsInfo, tmpField, 40);
	}
    /*END NAB10052019*/

	/* Field 49: Transaction Currency Code */
	GetIsoField (ISO_TRANS_CRNCY, IsoInfo, tmpField, &tmpFieldLen);
	AddSmsField (SMS_TRANS_CRNCY, SmsInfo, tmpField, 3);

	/**** -------------------------------------------- ****/
	/** Field 55 :ICC CHIP DATA  **/
	/** ------------------------------------------------**/
/*START NAB18042019 --Certif SG =>Le champs 55 ne doit pas etre envoyé dans les messages des annulations seulement dans le cas de« issuer authentication failure »--REF : Transaction Acceptance Device Guide(TADG)--car on pas ça dans les dernnieres spec */
	 if(GetIsoField(ISO_MSG_REASON,IsoInfo, tmpField, &tmpFieldLen)== SUCCESS && memcmp(tmpField, "4020", 4) == 0)
	 {
		if (GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		{
			IsChipTrx = 1;
			/*trace_event ( sLine , PROCESSING ); ??*/
			InitIsoIcTag ( &IsoIccInfo );
			AnalyseIsoIc  ( tmpField, tmpFieldLen , &IsoIccInfo );
			InitSmsIcTag ( &SmsIccInfo );
			/** Field 9F10 and 95 and 9F5B present in case of authentification failure  **/

			memset(FieldData, 0, sizeof(FieldData));
			/*TAG 9F5B :if the original response message from the issuer contained an issuer script,the issuer script results are provided in this field */
			if ( GetIsoIcTag ( ISO_TAG_ISS_SCRIPT_RESULT, &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddSmsIcTag ( SMS_TAG_ISS_SCRIPT_RESULT, &SmsIccInfo, FieldData,  FieldLen );


			memset(FieldData, 0, sizeof(FieldData));
			if ( GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddSmsIcTag(SMS_TAG_ISS_APP_DATA , &SmsIccInfo, FieldData,  FieldLen );

			memset(FieldData, 0, sizeof(FieldData));
			if ( GetIsoIcTag (ISO_TAG_TVR , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddSmsIcTag (SMS_TAG_TVR , &SmsIccInfo, FieldData,  FieldLen );

			FieldLen = SmsBuildIcFld ( sIccBuffer, &SmsIccInfo);

			/*START NAB25042019 Certif SG faut ajouter le DATA set ID au chmaps 55*/
			memset(tmpField1, 0, sizeof(tmpField1));
			tmpField1[0] = 0x01;
			tmpField1[1] = 0x00;
			tmpField1[2] = FieldLen ;
			memcpy(tmpField1+3, sIccBuffer, FieldLen);

			AddSmsField (SMS_ICC_DATA, SmsInfo, tmpField1, FieldLen+3);


			/*if ( FieldLen > 0 )
				AddSmsField (SMS_ICC_DATA, SmsInfo, sIccBuffer, FieldLen);
			*/
			/*END NAB25042019*/

		}

	 }
/*	if (GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		IsChipTrx = 1;

		trace_event ( sLine , PROCESSING );
		InitIsoIcTag ( &IsoIccInfo );
		AnalyseIsoIc  ( tmpField, tmpFieldLen , &IsoIccInfo );
		InitSmsIcTag ( &SmsIccInfo );

		memset(FieldData, 0, sizeof(FieldData));
		if ( GetIsoIcTag (ISO_TAG_ATC , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_ATC , &SmsIccInfo, FieldData,  FieldLen );
*/

		/** Field 9F10 and 95 present in case of authentification failer  **/
/*		memset(FieldData, 0, sizeof(FieldData));
		if ( GetIsoIcTag (ISO_TAG_ISS_APP_DATA , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag(SMS_TAG_ISS_APP_DATA , &SmsIccInfo, FieldData,  FieldLen );

		memset(FieldData, 0, sizeof(FieldData));
		if ( GetIsoIcTag (ISO_TAG_TVR , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddSmsIcTag (SMS_TAG_TVR , &SmsIccInfo, FieldData,  FieldLen );

		FieldLen = SmsBuildIcFld ( sIccBuffer, &SmsIccInfo);

		if ( FieldLen > 0 )
			AddSmsField (SMS_ICC_DATA, SmsInfo, sIccBuffer, FieldLen);
	}
*/

/*END  NAB18042019*/


	/* Field 60: Additional POS Information */

	if (IsChipTrx == 0)
		/*AddSmsField (SMS_ADTNL_POS_INFO, SmsInfo, "22", 2);*/
	 	AddSmsField (SMS_ADTNL_POS_INFO, SmsInfo, "25", 2);
	else
	{
		memset(tmpField1, 0, sizeof(tmpField1));
		tmpField1 [0] = '2';
		tmpField1 [1] = '5';
		tmpField1 [2] = '0';
		memcpy(tmpField1 +3, "000", 3);
		tmpField1 [6] = '1';
		tmpField1 [7] = '0';
		tmpField1 [8] = '\0';
		AddSmsField (SMS_ADTNL_POS_INFO,SmsInfo, tmpField1,strlen(tmpField1));
	}
	/* Field 62: CPS Fields */

	if (GetIsoField (ISO_CPS_FIELDS, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitTlvPrivate (&kTlvInfo);
		InitCpsInfo (&CpsInfo);
		AnalyseTlvPrivate (tmpField, tmpFieldLen, &kTlvInfo);

		if (GetTlvPrivate (CPS_ACI_TAG,&kTlvInfo, sCpsId, &tmpFieldLen) == SUCCESS)
		{
			if ( sCpsId[0] == 'E')
				AddCpsSubField (CPS_ACI, &CpsInfo, sCpsId, 1);
		}

		if (GetTlvPrivate (CPS_TRANS_ID_TAG, &kTlvInfo, tmpField, &tmpFieldLen) == SUCCESS)
			AddCpsSubField (CPS_TRANS_ID, &CpsInfo, tmpField, 15);

		tmpFieldLen = CpsBuildField (tmpField, &CpsInfo);

		if( tmpFieldLen > 0 )
		{
			AddSmsField (SMS_CPS_FIELDS, SmsInfo, tmpField, tmpFieldLen);
		}

	}



	/* Field 63: SMS Private Use Fields */
	InitPufInfo (&PufInfo);
	AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0000", 4);
	if (GetIsoField (ISO_MSG_REASON, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		IsoToSmsReasonCode(tmpField, tmpField1);
		AddPufSubField (PUF_MSG_REASON, &PufInfo, tmpField1, tmpFieldLen);
	}



	tmpFieldLen = PufBuildField (tmpField, &PufInfo);
	AddSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, tmpField, tmpFieldLen);
	/* End SMS Private Use Fields */

	/* Field 90: Original Data Elements */
	GetIsoField (ISO_ORIG_DATA, IsoInfo, tmpField, &tmpFieldLen);
	memcpy (tmpField1, "0200", 4);                  /* Message Number */
	/*START NAB02052019*/
	/*memcpy (tmpField1 +  4, tmpField + 4, 6);     */  /* Audit Number */
	memcpy (tmpField1 +  4, exStan, 6);      /* Audit Number */
	/*END NAB02052019*/
        memcpy (tmpField1 + 10, tmpField + 12, 10);      /* Xmit Date & Time */
	/*memcpy (tmpField1 + 10, tmpField + 10, 10);*/     /* Xmit Date & Time */
	memcpy (tmpField1 + 20, RightAdjust (AcquirerId, 11), 11);

	memcpy (tmpField1 + 31, "00000000000", 11);

	AddSmsField (SMS_ORIG_DATA, SmsInfo, tmpField1, 42);

	/*Start IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if (GetTlvPrivate(P7_F048_LOAN_DATA, &kTlvInfo, tmpField , &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kIsoRecurringPayTlv);

		if( GetTlvInfoField (P7_F048S24_NBR_INSTALMENTS, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T18_NBR_RECURRING_PAY, &kSmsRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_FREQ_RECUR_PAY, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T18_FREQ_RECURRING_PAY, &kSmsRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_REGISTRATION_REF_NBR, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T18_REGISTR_REF_NBR, &kSmsRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}

		if( GetTlvInfoField (P7_F048S24_MONTHLY_RPYMNT_AMNT, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T18_MAX_RECURRING_PAY_AMNT, &kSmsRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}
		/*Start FZL 03102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/
		if( GetTlvInfoField (P7_F048S24_VALIDATION_IND, &kIsoRecurringPayTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{
			AddTlvInfoField(SMS_F104T18_VALIDATION_IND, &kSmsRecurringPayTlv, tmpField, tmpFieldLen);
			nNbrTags++;
		}
		/*End FZL 03102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/ 
	}

	GetIsoField (ISO_FUNCTION_CODE, IsoInfo,sFunctionCode,&tmpFieldLen);
	if(memcmp(sFunctionCode, P7_F024_RECURRING_MODIF, P7_F024_LEN) == 0 && atoi(TrxAmount) == 0)
	{
		AddTlvInfoField(SMS_F104T18_RECURRING_PAY_TYPE, &kSmsRecurringPayTlv, SMS_F104T18_T80_MODIFICATION, SMS_F104T18_RECURRING_PAY_TYPE_LEN);
		nNbrTags++;
	}
	else if(memcmp(sFunctionCode, P7_F024_RECURRING_CANCEL, P7_F024_LEN) == 0 && atoi(TrxAmount) == 0)
	{
		AddTlvInfoField(SMS_F104T18_RECURRING_PAY_TYPE, &kSmsRecurringPayTlv, SMS_F104T18_T80_CANCELLATION, SMS_F104T18_RECURRING_PAY_TYPE_LEN);
		nNbrTags++;
	}
	else if ((GetTlvPrivate( RECURRING_FLAG_TAG , &kTlvInfo, RecurringFlag, &tmpFieldLen)== SUCCESS)
		&&(RecurringFlag[0] ==P7_F048_P71_CRED_ON_FILE
		||RecurringFlag[0]==P7_F048_P71_INSTALLMENT
		||RecurringFlag[0]==P7_F048_P71_RECURRING
		||RecurringFlag[0]==P7_F048_P71_STANDING))
	{
		if((GetTlvPrivate( AUTH_ADD_INFO , &kTlvInfo, sAddAuthInfo, &tmpFieldLen)== SUCCESS)
			&& sAddAuthInfo[P7_F048_P64_CRDNTL_REASON] != P7_F048_P64_P7_UNSPECIFIED)
			{
				AddTlvInfoField(SMS_F104T18_RECURRING_PAY_TYPE, &kSmsRecurringPayTlv, SMS_F104T18_T80_REGISTRATION_FIRST_TRANS, SMS_F104T18_RECURRING_PAY_TYPE_LEN);
				nNbrTags++;
			}
			else
			{
				AddTlvInfoField(SMS_F104T18_RECURRING_PAY_TYPE, &kSmsRecurringPayTlv, SMS_F104T18_T80_SUBSEQ_TRANS, SMS_F104T18_RECURRING_PAY_TYPE_LEN);
				nNbrTags++;
			}
	}

	if (strlen(RecurringFlag) > 0
		&&(RecurringFlag[0]==P7_F048_P71_RECURRING
		||RecurringFlag[0]==P7_F048_P71_INSTALLMENT))
	{
		AddTlvInfoField(SMS_F104T18_PAY_AMNT_IND_PER_TRANS, &kSmsRecurringPayTlv, SMS_F104T18_T81_FIX_AMNT_IND_PER_TRANS, SMS_F104T18_PAY_AMNT_IND_PER_TRANS_LEN);
		nNbrTags++;
	}

	if(strlen(RecurringFlag) > 0
		&&(RecurringFlag[0]==P7_F048_P71_STANDING
		||RecurringFlag[0]==P7_F048_P71_CRED_ON_FILE))
	{
		AddTlvInfoField(SMS_F104T18_PAY_AMNT_IND_PER_TRANS, &kSmsRecurringPayTlv, SMS_F104T18_T81_RECUR_PAY_MAX_AMNT, SMS_F104T18_PAY_AMNT_IND_PER_TRANS_LEN);
		nNbrTags++;
	}

	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &kSmsRecurringPayTlv);
	if( tmpFieldLen > 0 )
	{
		AddTlvInfoField(SMS_F104_RECURRING_PAY, &kSmsTransSpecTlv, tmpField, tmpFieldLen);
	}

	if(strlen(RecurringFlag) > 0
		&& nNbrTags != SMS_F104_T18_NB_FIELDS /*- 1*/  /*FZL 03102022 PLUTONL-5185 ENH 22.Q4 VISA Art3.17*/
		&& GetIsoField(ISO_PAN_COUNTRY, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS
		&& memcmp(tmpField,"356", 3) == 0) /*Indian Cards*/
	{
		trace_message(P7_TL_ERROR, "Tags missing in TLV Field 104, Usage 2, Dataset ID 18 for Indian Card");
		trace_message(P7_TL_PROCESSING, "End  BuildAutReqToNw ERROR(NOK)");
		return (NOK);
	}

	/*End IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kSmsTransSpecTlv);	/*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if (tmpFieldLen > 0)
		AddSmsField(SMS_TRANS_DESC, SmsInfo, tmpField, tmpFieldLen);


	trace_event("End ReversalAdviceToSms()", PROCESSING);

	return(OK);
}


/*************************************************************************/
/*  ReversalResponseFromSms()                                            */
/*                                                                       */
/*  Puts SMS reversal advice response message in a ISO message.          */
/*  Params: OldSmsInfo: original SMS reversal request                    */
/*          NewSmsInfo: SMS reversal advice response message             */
/*          szResponseCode: response code                                */
/*************************************************************************/
/*int ReversalResponseFromSms ( int         P_nIndexCtx,
	TSSmsInfo   *SmsInfo,
	TSIsoInfo   *IsoInfo)
{
	char    tmpField   [256];
	char    tmpField1  [256];
	int     tmpFieldLen;

	trace_event("Start ReversalResponseFromSms()", PROCESSING);

	/* Message type is 1430 *
	IsoInfo->nMsgType = 1430;

	/* Field 20: PAN extended, Country Code *
	GetSmsField (SMS_PAN_COUNTRY, SmsInfo, tmpField, &tmpFieldLen);
	InsertIsoField (ISO_PAN_COUNTRY, IsoInfo, tmpField, 3);

	/* Field 39: Response Code *
	GetSmsField (SMS_RESPONSE_CODE, SmsInfo, tmpField, &tmpFieldLen);
	TranslateRespCode (P_nIndexCtx, ResourceId, tmpField, "RP", tmpField1);
	InsertIsoField (ISO_RESPONSE_CODE, IsoInfo, tmpField1, 3);

	/* Field 44: Additional Response Data *
	GetSmsField (SMS_ADTNL_RESP_DATA, SmsInfo, tmpField, &tmpFieldLen);
	InsertIsoField (ISO_ADTNL_RESP_DATA, IsoInfo, tmpField, tmpFieldLen);

	trace_event("End ReversalResponseFromSms()", PROCESSING);

	return(OK);
}*/



/*************************************************************************/
/* ReversalRepToNw()                                                   */
/* */
/* Prepares an reversal response message (0410) to SMS.               */
/* Input:  ISO format message                                          */
/* Output: SMS message                                                */
/*************************************************************************/
int
	ReversalRepToNw(int nIndexCtx,
	TSIsoInfo * IsoInfo,
	TSSmsInfo * OrigSmsInfo)
{

	TSSmsInfo 		NewSmsInfo;
	TSTlvPrivate    TlvInfo;
	char            tmpField[256];
	char          	tmpField1[256];
	char            isoResponse[4];
	char            FieldData[256];
	int             tmpFieldLen;
	int             i;
	int				Length;
	char            sBuffer[1024];

	trace_event("Start  ReversalRepToNw()", PROCESSING);
	
	InitSmsInfo(&NewSmsInfo);
	memset(sBuffer, 0, sizeof(sBuffer));

   if (OrigSmsInfo->nMsgType == 420)
	NewSmsInfo.nMsgType = 430;
   else
	NewSmsInfo.nMsgType = 410;

	for (i = 0; i < MAX_SMS_FIELDS; i++) {
		switch (i)
		{
		case SMS_PRIV_USE_FIELDS:
			if (SmsEchoedField_0400[i] == 1)
			{
				if (GetSmsField (i, OrigSmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
				{
					if((tmpField[0]&0x80)==0x80)
					{
						memset(tmpField1,0,sizeof(tmpField1));
						tmpField1[0]=0x80;
						tmpField1[1]=0x00;
						tmpField1[2]=0x00;
						memcpy(tmpField1+3 ,tmpField+3,2);
						AddSmsField (i, &NewSmsInfo, tmpField1, 5);
					}
				}
			}
			break;
		case SMS_RESPONSE_CODE:
			/*EBE150917
			if (  memcmp(SITE_REG,"E",1) == 0   )
			{
				GetIsoField (ISO_RESPONSE_CODE, IsoInfo, isoResponse, &tmpFieldLen);
				TranslateRespCode (nIndexCtx,ResourceId, isoResponse, "PR", tmpField);
				AddSmsField (SMS_RESPONSE_CODE, NewSmsInfo, tmpField, 2);
			}
			else
				AddSmsField (SMS_RESPONSE_CODE, NewSmsInfo, "00", 2);
				*/
#if SMS_EU
			GetIsoField (ISO_RESPONSE_CODE, IsoInfo, isoResponse, &tmpFieldLen);
			TranslateRespCode (nIndexCtx,ResourceId, isoResponse, "PR", tmpField);
			AddSmsField (SMS_RESPONSE_CODE, &NewSmsInfo, tmpField, 2);
#else
			AddSmsField (SMS_RESPONSE_CODE, &NewSmsInfo, "00", 2);
#endif
			break;
		case SMS_ADTNL_RESP_DATA:
			memset(FieldData, 0, sizeof(FieldData));
			FieldData[0] = '5';
			AddSmsField(SMS_ADTNL_RESP_DATA, &NewSmsInfo, FieldData, strlen(FieldData));
			break;
		case SMS_ORIG_DATA:
			if (GetSmsField(SMS_ORIG_DATA, OrigSmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
				AddSmsField(SMS_ORIG_DATA, &NewSmsInfo, tmpField, tmpFieldLen);
			break;
		case SMS_REPLACE_AMOUNTS:
			if (GetSmsField(SMS_REPLACE_AMOUNTS, OrigSmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
				AddSmsField(SMS_REPLACE_AMOUNTS, &NewSmsInfo, tmpField, tmpFieldLen);
			break;
		default:
			if (SmsEchoedField_0400[i] == 1)
			{
				if (GetSmsField(i, OrigSmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
					AddSmsField(i, &NewSmsInfo, tmpField, tmpFieldLen);
			}
			break;
		}
	}

	/* send response to network */
	Length = SmsBuildMsg(sBuffer + SMS_HDR_LEN, &NewSmsInfo);
	SmsBuildHeader(RESPONSE, &(NewSmsInfo.sHeader), &(OrigSmsInfo->sHeader), Length, StationId);
	memcpy(sBuffer, (char *) &(NewSmsInfo.sHeader), SMS_HDR_LEN);
	Length += SMS_HDR_LEN;

	DumpSms(sBuffer);
	if (WriteLineMsg(sBuffer, Length))
	{
		trace_message ( P7_TL_ERROR,"Error when calling WriteLineMsg(): [Length:%d]",Length);
		trace_message(P7_TL_INFO,"End ReversalRepToNw(NOK)");
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}


	MsgRepSent(IsoInfo->msgId,isoResponse);

	trace_event("End  ReversalRepToNw(OK)", PROCESSING);
	return (OK);
}

