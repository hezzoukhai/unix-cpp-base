#include <stdio.h>
#include <string.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_visa.h>
#include <visa_puf.h>
#include <visa_puf_fields.h>
#include <visa_param.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <queue.h>
#include <security_data.h>
#include <hsm_inc.h>
#include <visa_define.h>
#include <p7_sid.h>
#include <p7_define.h>
#include <p7_services.h>
#include <security_flags.h>
#include <iso8583.h>
#include <general_macros.h>
#include <nw_raw_msg.h>
#include <tlv_fields.h> /*PLUTONL-5283 FZL20221013*/

static int             VisaEchoedField_0100[] =
{
	/* 001 */ 0,			/* BIT_MAP2                            */
	/* 002 */ 1,			/* CARD_NBR                            */
	/* 003 */ 1,			/* PROC_CODE                           */
	/* 004 */ 1,			/* TRANS_AMOUNT                        */
	/* 005 */ 0,			/* STLMNT_AMOUNT                       */
	/* 006 */ 0,			/* CARDHOLDER BILLING AMOUNT           */
	/* 007 */ 1,			/* XMIT_TIME                           */
	/* 008 */ 0,			/* CARDHOLDER BILLING FEE              */
	/* 009 */ 0,			/* CONVRSN_RATE_STLMNT                 */
	/* 010 */ 0,			/* CONVRSN_RATE_CARD_HLDR_BILLING      */
	/* 011 */ 1,			/* AUDIT_NBR             */
	/* 012 */ 0,			/* TRANS_TIME            */
	/* 013 */ 0,			/* TRANS_DATE            */
	/* 014 */ 0,			/* EXPIRY_DATE           */
	/* 015 */ /*0*/ 1,		/* STLMNT_DATE           *//*NAB04032021 PLUTONL-2813*/
	/* 016 */ 0,			/* CNVRSN_DATE           */
	/* 017 */ 0,			/* CAPTURE_DATE          */
	/* 018 */ 0,			/* MERCHANT_TYPE         */
	/* 019 */ 1,			/* ACQR_COUNTRY          */
	/* 020 */ 1,			/* PAN_COUNTRY           */
	/* 021 */ 0,			/* FORWD_COUNTRY                       */
	/* 022 */ 0,			/* POS_ENTRY_MODE                      */
/*023*/   1,   /* CARD SEQUENCE NUMBER                */
	/* 024 */ 0,			/* NETWORK INTERNATIONAL IDENTIFIER    */
	/* 025 */ 1,			/* POS_CONDITION                       */
	/* 026 */ 0,			/* POS_PIN_CAPTURE                     */ /*AER 12092011 Field must be present only in requests and advices and must be not be in the response*/
	/* 027 */ 0,			/* AUTHORIZATION ID RESPONSE LENGTH    */
	/* 028 */ 0,			/* TRANS_FEE                           */
	/* 029 */ 0,			/* AMOUNT SETTLEMENT FEE               */
	/* 030 */ 0,			/* AMOUNT TRANSACTION PROCESSING FEE   */
	/* 031 */ 0,			/* AMOUNT SETLMNT PROCESSING FEE       */
	/* 032 */ 1,			/* ACQR_ID                             */
	/* 033 */ 0,			/* FORWD_ID                            */
	/* 034 */ 0,			/* PAN_EXTENDED                        */
	/* 035 */ 0,			/* T2_DATA                             */
	/* 036 */ 0,			/* TRACK 3 DATA                        */
	/* 037 */ 1,			/* REFERENCE_NBR                       */
	/* 038 */ 0,			/* AUTHOR_ID                           */
	/* 039 */ 0,			/* RESPONSE_CODE                       */
	/* 040 */ 0,			/* SERVICE RESTRICTION CODE            */
	/* 041 */ 1,			/* TERMINAL_NBR                        */
	/* 042 */ 1,			/* OUTLET_NBR                          */
	/* 043 */ 0,			/* TERMINAL_ADR                        */
	/* 044 */ 0,			/* ADTNL_RESP_DATA                     */
	/* 045 */ 0,			/* T1_DATA                             */
	/* 046 */ 0,			/* ADDITRIONAL DATA ISO                */
	/* 047 */ 0,			/* ADDITIONAL DATA NATIONAL            */
	/* 048 */ 1,			/* ADTNL_DATA_PRIV                     */
	/* 049 */ 1,			/* TRANS_CRNCY                         */
	/* 050 */ 0,			/* STLMNT_CRNCY                        */
	/* 051 */ 0,			/* BILLING_CRNCY         */
	/* 052 */ 0,			/* PIN_DATA              */
	/* 053 */ 0,			/* SECURITY_DATA         */
	/* 054 */ 0,			/* ADTNL_AMOUNTS         */
	/* 055 */ 0,			/* Reserved Iso          */
	/* 056 */ 0,			/* Reserved Iso          */
	/* 057 */ 0,			/* Reserved National     */
	/* 058 */ 0,			/* Reserved National     */
	/* 059 */ 0,			/* GEOGR_DATA            */
	/* 060 */ 0,			/* ADTNL_POS_INFO        */
	/* 061 */ 0,			/* OTHER_AMOUNTS         */
	/* 062 */ 1,			/* CPS_FIELDS            */
	/* 063 */ 1,			/* PRIV_USE_FIELDS       */		/*AMER20171121*/
	/* 064 */ 0,			/* MAC1                  */
	/* 065 */ 0,			/* */
	/* 066 */ 0,			/* STLMNT_CODE           */
	/* 067 */ 0,			/* Extended Payment Code */
	/* 068 */ 0,			/* RECEIVER_COUNTRY      */
	/* 069 */ 0,			/* STLMNT_INST_COUNTRY   */
	/* 070 */ 0,			/* NW_MNGMT_INFO         */
	/* 071 */ 0,			/* Message Number              */
	/* 072 */ 0,			/* Message number Last         */
	/* 073 */ 0,			/* ACTION_DATE                 */
	/* 074 */ 0,			/* CR_NBR                      */
	/* 075 */ 0,			/* CR_REV_NBR                  */
	/* 076 */ 0,			/* DB_NBR                      */
	/* 077 */ 0,			/* DB_REV_NBR                  */
	/* 078 */ 0,			/* Transfer Number             */
	/* 079 */ 0,			/* Transfer Reversal Number    */
	/* 080 */ 0,			/* Inquiries Number            */
	/* 081 */ 0,			/* Authorizations Number           */
	/* 082 */ 0,			/* Credit, Processing Fee Amount   */
	/* 083 */ 0,			/* Credit, Transaction Fee Amount  */
	/* 084 */ 0,			/* Debit, Processing Fee Amount    */
	/* 085 */ 0,			/* Debit, Transaction Fee Amount   */
	/* 086 */ 0,			/* CR_AMOUNT                       */
	/* 087 */ 0,			/* CR_REV_AMOUNT                   */
	/* 088 */ 0,			/* DB_AMOUNT                       */
	/* 089 */ 0,			/* DB_REV_AMOUNT                   */
	/* 090 */ 0,			/* ORIG_DATA                       */
	/* 091 */ 0,			/* FILE_UPDATE           */
	/* 092 */ 0,			/* FILE_SECURITY         */
	/* 093 */ 0,			/* Response Indicator    */
	/* 094 */ 0,			/* Service Indicator     */
	/* 095 */ 0,			/* REPLACE_AMOUNTS       */
	/* 096 */ 0,			/* MSG_SECURITY          */
	/* 097 */ 0,			/* NET_AMOUNT            */
	/* 098 */ 0,			/* Payee                 */
	/* 099 */ 0,			/* STLMNT_INSTITN        */
	/* 100 */ 0,			/* RECEIVER_ID           */
	/* 101 */ 0,			/* FILE_NAME             */
	/* 102 */ 0,			/* ACCOUNT_ID1           */
	/* 103 */ 0,			/* ACCOUNT_ID2           */
	/* 104 */ 0,			/* Transaction Description   */
	/* 105 */ 0,			/* Reserved Iso          */
	/* 106 */ 0,			/* Reserved Iso          */
	/* 107 */ 0,			/* Reserved Iso          */
	/* 108 */ 0,			/* Reserved Iso          */
	/* 109 */ 0,			/* Reserved Iso          */
	/* 110 */ 0,			/* Reserved Iso          */
	/* 111 */ 0,			/* Reserved Iso          */
	/* 112 */ 0,			/* Reserved National     */
	/* 113 */ 0,			/* Reserved National     */
	/* 114 */ 0,			/* Reserved National     */
	/* 115 */ 0,			/* ADTNL_TRACE_DATA      */
	/* 116 */ 0,			/* Reserved National     */
	/* 117 */ 0,			/* Reserved National     */
	/* 118 */ 0,			/* Intra Country Data    */
	/* 119 */ 0,			/* Reserved National     */
	/* 120 */ 0,			/* Original Message Type Id  */
	/* 121 */ 0,			/* Issuing Institution Id    */
	/* 122 */ 0,			/* Remaining Open to Use     */
	/* 123 */ 0,			/* ADR_VERIF_DATA            */
	/* 124 */ 0,			/* Free Form Text - Japan    */
	/* 125 */ 0,			/* SUPPORT_INFO              */
	/* 126 */ 0,			/* Reserved Private          */
	/* 127 */ 0,			/* File Record Action and Data   */
	/* 128 */ 0,			/* MAC2                          */
	/* 129 */ 0,			/* Field 129                     */
	/* 130 */ 0,			/* Terminal Capability Profile   */
	/* 131 */ 0,			/* Terminal Verification Results */
	/* 132 */ 0,			/* Unpredictable Number          */
	/* 133 */ 0,			/* Terminal Serial Number        */
	/* 134 */ 0,			/* Visa Discretionary Data       */
	/* 135 */ 0,			/* Issuer Discretionary Data     */
	/* 136 */ 0,			/* Issuer Discretionary Data     */
	/* 137 */ 0,			/* Application Trans. Counter    */
	/* 138 */ 0,			/* Application Inter. Profile    */
	/* 139 */ 0,			/* ARPC + Response Code          */
	/* 140 */ 0,			/* Field 140                     */
	/* 141 */ 0,			/* Field 141                     */
	/* 142 */ 0,			/* Issuer Script                 */
	/* 143 */ 0,			/* Issuer Script Results         */
	/* 144 */ 0,			/* Cryptogram Trans Type         */
	/* 145 */ 0,			/* Terminal Country Code         */
	/* 146 */ 0,			/* Terminal Trans  Date          */
	/* 147 */ 0,			/* Cryptogram Amount             */
	/* 148 */ 0,			/* Cryptogram Currency Code      */
	/* 149 */ 0,			/* Cryptogram Cash Back          */
	/* 150 */ 0,			/* Cardholder Card Scheme Crypto. */
	/* 151 */ 0,			/* Field 151             */
	/* 152 */ 0,			/* Field 152             */
	/* 153 */ 0,			/* Field 153             */
	/* 154 */ 0,			/* Field 154             */
	/* 155 */ 0,			/* Field 155             */
	/* 156 */ 0,			/* Field 156             */
	/* 157 */ 0,			/* Field 157             */
	/* 158 */ 0,			/* Field 158             */
	/* 159 */ 0,			/* Field 159             */
	/* 160 */ 0,			/* Field 160             */
	/* 161 */ 0,			/* Field 161             */
	/* 162 */ 0,			/* Field 162             */
	/* 163 */ 0,			/* Field 163             */
	/* 164 */ 0,			/* Field 164             */
	/* 165 */ 0,			/* Field 165             */
	/* 166 */ 0,			/* Field 166             */
	/* 167 */ 0,			/* Field 167             */
	/* 168 */ 0,			/* Field 168             */
	/* 169 */ 0,			/* Field 169             */
	/* 170 */ 0,			/* Field 170             */
	/* 171 */ 0,			/* Field 171             */
	/* 172 */ 0,			/* Field 172             */
	/* 173 */ 0,			/* Field 173             */
	/* 174 */ 0,			/* Field 174             */
	/* 175 */ 0,			/* Field 175             */
	/* 176 */ 0,			/* Field 176             */
	/* 177 */ 0,			/* Field 177             */
	/* 178 */ 0,			/* Field 178             */
	/* 179 */ 0,			/* Field 179             */
	/* 180 */ 0,			/* Field 180             */
	/* 181 */ 0,			/* Field 181             */
	/* 182 */ 0,			/* Field 182             */
	/* 183 */ 0,			/* Field 183             */
	/* 184 */ 0,			/* Field 184             */
	/* 185 */ 0,			/* Field 185             */
	/* 186 */ 0,			/* Field 186             */
	/* 187 */ 0,			/* Field 187             */
	/* 188 */ 0,			/* Field 188             */
	/* 189 */ 0,			/* Field 189             */
	/* 190 */ 0,			/* Field 190             */
	/* 191 */ 0,			/* Field 191             */
	/* 192 */ 0,			/* Message Authentication Code   */
};



/**************************************************************************/
/* FCT_AUT_REP_TO_NW()                                                  */
/* */
/* Sends an authorization response (0110) message to BASEI.             */
/**************************************************************************/
int FCT_AUT_REP_TO_NW(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	TSVisaInfo      OrigVisaInfo;
	TSNodeData      Node;
	TSTlvPrivate    TlvInfo;
	char            sStan[6 + 1];
	char            FieldData[BUF_LG_MAX];	/*AEA20210824 PLUTONL-3379*/
	int             FieldLen;
	int             response;
	char            sDestinationPid[10];
	char            sSecurityData[128 + 1];
	char            sDestResource[6 + 1];
	char            sTlvBuffer[LG_MAX];
	int             i, nLength;
	char            sLine[MAX_LINE_TRC];


	trace_event("Start  FCT_AUT_REP_TO_NW()", PROCESSING);
	memset(FieldData, 0, sizeof(FieldData));
	memset(sStan, 0, sizeof(sStan));


	/*MsgRepRetrieved(IsoInfo->msgId);*//*PLUTONL-3762*/

	dump_p7_struct(IsoInfo);


	GetIsoField(ISO_SECURITY_DATA, IsoInfo, sSecurityData, &FieldLen);
	GetSecurityData(DESTINATION_RESOURCE, sSecurityData, sDestResource, &FieldLen);


	/* Get audit number to search for original transaction */
	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, &FieldLen) == SUCCESS)
	{

		AnalyseTlvPrivate(FieldData, FieldLen, &TlvInfo);
		if (GetTlvPrivate(EXTERNAL_STAN_TAG, &TlvInfo, sStan, &FieldLen) != SUCCESS)
		{
			trace_event("Can Not FetchRequest (ENOEXIST): External Stan Missing", ERROR);
			trace_event("End FCT_AUT_REP_TO_NW (NOK)", ERROR);
			return (NOK);
		}
		sprintf(sLine, ">>> STAN = %.6s", sStan);
		trace_event(sLine, PROCESSING);
	}


	InitNodeData(&Node);
	sprintf(Node.szFstKey, "%04d", IsoInfo->nMsgType - 1010);
	memcpy(Node.szFstKey + 4, sStan, 6);

	GetIsoField(ISO_CARD_NBR, IsoInfo, FieldData, &FieldLen);
	memcpy(Node.szFstKey + 4 + 6, FieldData, FieldLen);

	sprintf(sLine, "==> Fetch Key =[%.32s]", Node.szFstKey);
	trace_event(sLine, FATAL);

	switch (FetchRequest('L', &Node))
	{
	case ENOEXIST:
		trace_event("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event("End FCT_AUT_REP_TO_NW (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;
	}

	trace_event("FetchRequest SUCCES ", PROCESSING);

	/* build original received VISA message */
	nLength = AnalyseVisa(Node.sFstBuf, &OrigVisaInfo);

	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	memcpy(sTlvBuffer, Node.sSndBuf, Node.nSndBufLen);

	/* update authorizations table */
	response = ServiceReplyProcessing(nIndexCtx, IsoInfo, sTlvBuffer,FWD_MSG_REPLIED);
	if( response == ROUTAGE )
	{
		trace_event(" ROUTAGE Returned ", PROCESSING);
		trace_event("End  FCT_AUT_REP_TO_NW(OK): nothing to be done", PROCESSING);
		return OK;
	}


	/* build response message & send it */
	if (AutReplyToNw(nIndexCtx, IsoInfo, &OrigVisaInfo) == OK)
	{
		trace_event("End  FCT_AUT_REP_TO_NW(OK)", PROCESSING);
		return (OK);
	}
	else
	{
		trace_event("End  FCT_AUT_REP_TO_NW(NOK)", PROCESSING);
		return (NOK);
	}

}

/**************************************************************************/
/* FCT_AUT_REP_FROM_NW()                                                */
/* */
/* Receives an authorization response message (0110) from BASEI         */
/**************************************************************************/
int
FCT_AUT_REP_FROM_NW(int nIndexCtx, TSVisaInfo * VisaInfo)
{
	TSNodeData      Node;
	TSIsoInfo       IsoInfo;
	TSBalExchange   BalMessage;
	char            FieldData[256];
	int             FieldLen;
	char            SourceId[8 + 1];
	int             Length;
	char            sSecurityData[128];
	char            sLine[MAX_LINE_TRC];
	char			SourceNodeId[NODE_ID_LEN + 1];
	int				nBal;

	trace_event("Start  FCT_AUT_REP_FROM_NW()", PROCESSING);


	/* Get audit number to search for original transaction */
	GetVisaField(VISA_AUDIT_NBR, VisaInfo, FieldData, &FieldLen);

	sprintf(sLine, "Stan en retour = %s ", FieldData);
	trace_event(sLine, PROCESSING);

	InitNodeData(&Node);
	memcpy(Node.szFstKey, "1100", 4);
	memcpy(Node.szFstKey + 4, FieldData, 6);

	GetVisaField(VISA_CARD_NBR, VisaInfo, FieldData, &FieldLen);
	memcpy(Node.szFstKey + 4 + 6, FieldData, FieldLen);

	/*AMER 20160727 Start: PADSS Fix*/
	if(debug_option == 1)
	{
	sprintf(sLine, "==> FetchKey = [%.32s]", Node.szFstKey);
	trace_event(sLine, PROCESSING);
	}
	/*AMER 20160727 End: PADSS Fix*/

	switch (FetchRequest('B', &Node))
	{
	case ENOEXIST:
		trace_event("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event("End FCT_AUT_REP_FROM_NW (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;
	}

	trace_event("FetchRequest Succeeded ", PROCESSING);

	MsgFwdRepReceived(Node.msgId);

	/* reconstruct original ISO message */
	InitIsoInfo(&IsoInfo);
	AnalyseIso(Node.sFstBuf, &IsoInfo);
	IsoInfo.msgId = Node.msgId;

	/* add response elements to ISO message */
	BuildAutReplyFromNw(nIndexCtx, VisaInfo, &IsoInfo);

	/* forward it to requester */

	memset(sSecurityData, 0, sizeof(sSecurityData));
	memset(SourceId, 0, sizeof(SourceId));
	memset(SourceNodeId, 0, sizeof(SourceNodeId));
	GetIsoField(ISO_SECURITY_DATA, &IsoInfo, sSecurityData, &FieldLen);
	GetSecurityData(SOURCE_PID, sSecurityData, SourceId, &Length);
	GetSecurityData(SOURCE_NODE_ID, sSecurityData, SourceNodeId, &Length);

	/*Start IBO20200517 RAWMSG*/
	if (g_sCaptureMsg[NW_INC_RESP] == 'Y')
	{
		LogVisaMessage(nIndexCtx, VisaInfo, &IsoInfo, INC_RESP);
	}
	/*End IBO20200517 RAWMSG*/

	sprintf(sLine, "Source Id = [%s] ", SourceId);
	trace_event(sLine, PROCESSING);

	dump_p7_struct(&IsoInfo);

	if (memcmp(NodeId, SourceNodeId, NODE_ID_LEN) != 0)
	{
		BalMessage.sHeader.nPartnerId = getuid();
		nBal = nBalBrr;
	}
	else
	{
		BalMessage.sHeader.nPartnerId = atoi(SourceId);
		nBal = nBalMsg;
	}
	BalMessage.sHeader.nSourceId = getpid();
	BalMessage.sHeader.nCommand = IsoInfo.nMsgType;
	BalMessage.sHeader.msgId = Node.msgId;

	Length = IsoBuildMsg(BalMessage.sBody, &IsoInfo);
	BalMessage.sHeader.nLen = Length;

	if (WriteBalMsg(nBal, &BalMessage) != OK)
	{
		trace_event("End FCT_AUT_REP_FROM_NW(): WriteBal() failed.", ERROR);
		IntWriteBalExeption(nIndexCtx,&IsoInfo); /* ELKAM PLUTONL-4061 */
		return (NOK);
	}

	MsgFwdRepReplied(BalMessage.sHeader.msgId);

	memset(FieldData, 0, sizeof(FieldData));
	InitNodeData(&Node);
	InitIsoInfo(&IsoInfo);
	memset((char *)&BalMessage, 0, sizeof(TSBalExchange));

	trace_event("End  FCT_AUT_REP_FROM_NW(OK)", PROCESSING);

	return (OK);
}


/****************************************************************************/
/* BuildAutReplyFromNw()                                                  */
/* */
/* Processes au Authorization Response message (0110) received from VISA. */
/****************************************************************************/
int
BuildAutReplyFromNw(int				nIndexCtx,
					TSVisaInfo		*VisaInfo,
					TSIsoInfo		*IsoInfo)
{
	TSTlvPrivate    sTlvPrivateInfo;
	TSTlvPrivate    sTlvInfo;		/*AMER20170311: PROD00039737*/
	TSVisaPufInfo   VisapufInfo;	/*AEA20210712 Enh21.Q4 Art9.3.3*/
	char            FieldData[LG_MAX];
	char            sLine[MAX_LINE_TRC];
	char            sCpsTlvData[128];
	char            sRespCode[3 + 1];
	char            sVisaRespCode[2 + 1];
	char            tmpField[256];
	char            tmpField2[256];/*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	int             FieldLen;
	int             nRetCode;
	TSCpsInfo       CpsInfo;
	int             tmpFieldLen; /* END HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.2*/
	char            sTokenResponse[1 + 1];  /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	/*char 		AmntType[1 + 1];*/
	char            MerchantType[5]; /*Start PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	char            PosMode[7];
	char            PosCond[3];
	char 			AddPos[24+1];
   	int             AddPosLen;
	int				nEcomInfoLen = 0;
	char			sEcomAuthInfo[P7_F048_P66_LEN + 1];/*END PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	TlvInfo			kVisaTransSpecTlv; /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	TlvInfo			kVisaAppIdentTlv; /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/

	trace_event("Start  BuildAutReplyFromNw()", PROCESSING);
	memset(FieldData, 0, sizeof(FieldData));
	memset(sCpsTlvData, 0, sizeof(sCpsTlvData));
	memset(sTokenResponse, 0, sizeof(sTokenResponse));    /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	memset(PosMode, 0, sizeof(PosMode));/*Start PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	memset(PosCond, 0, sizeof(PosCond));
	memset(AddPos, 0, sizeof(AddPos));
	memset(MerchantType, 0, sizeof(MerchantType));
 	memset(sEcomAuthInfo, ' ', sizeof(sEcomAuthInfo));
	/*End PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	InitVisaPufInfo(&VisapufInfo);	/*AEA20210712 Enh21.Q4 Art9.3.3*/
	InitVisaTransSpecDataTlv(&kVisaTransSpecTlv); /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	InitVisaAppIdentDataTlv(&kVisaAppIdentTlv);  /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	/*memset(AmntType, 0, sizeof(AmntType));

	InitTlvPrivate(&TlvInfo);*/

	/* IsoInfo->nMsgType = 1110; */
	IsoInfo->nMsgType = IsoInfo->nMsgType + 10;

	/* Field 38 */
	if (GetVisaField(VISA_AUTHOR_ID, VisaInfo, FieldData, &FieldLen) == SUCCES)
		PutIsoField(ISO_AUTHOR_ID, IsoInfo, FieldData, FieldLen);

	/* Field 39 */
	if (GetVisaField(VISA_RESPONSE_CODE, VisaInfo, sVisaRespCode, &FieldLen) == SUCCES)
	{
		nRetCode = TranslateRespCode(nIndexCtx, ResourceId, sVisaRespCode, "RP", sRespCode);
		if( nRetCode == OK )
			PutIsoField(ISO_RESPONSE_CODE, IsoInfo, sRespCode, 3);
		else
			PutIsoField(ISO_RESPONSE_CODE, IsoInfo, MALFUNCTION_ACTION, 3);

		/*AMER20180320: The code below is moved to VisaField48ToIso function*/
		/*AMER20170311 Start: PROD00039737*/
		/*if(!memcmp(sVisaRespCode,B1_F039_PARTIAL_APPROVAL,2))
		{
			if(GetVisaField (VISA_TRANS_AMOUNT, VisaInfo, FieldData, &FieldLen) == SUCCES)
				PutIsoField (ISO_TRANS_AMOUNT, IsoInfo, FieldData, FieldLen);

			if (GetIsoField (ISO_ADTNL_DATA_PRIV , IsoInfo, FieldData, &FieldLen) == SUCCESS)
        	{
            		AnalyseTlvPrivate (FieldData, FieldLen, &TlvInfo);
        	}

			AmntType[0] = P7_F048_K02_TRX_AMNT_TYPE;
			PutTlvPrivate ( PARTIAL_AMOUNT_TYPE , &TlvInfo  , AmntType, 1 );
			memset(FieldData, 0, sizeof(FieldData));
            		FieldLen = PrivateBuildTlv (FieldData, &TlvInfo);

            		if ( FieldLen > 0 )
				PutIsoField (ISO_ADTNL_DATA_PRIV , IsoInfo, FieldData , FieldLen);
		}*/
		/*AMER20170311 End: PROD00039737*/
	}

	/* Fields 44, 48, both go into ISO field 48 in TLV format */
	nRetCode = VisaField48ToIso(VisaInfo, IsoInfo);


	/* Field 54: for Balance Inquiry Messages */
	memset(tmpField, 0, sizeof(tmpField));
	memset(tmpField2, 0, sizeof(tmpField2));/*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/

	/* START MOD CFE PROD00045718 20170828 */
	VisaToIsoAddAmount(VisaInfo, tmpField);
	PutIsoField(ISO_ADTNL_AMOUNTS, IsoInfo, tmpField, strlen(tmpField));
  /* END MOD CFE PROD00045718 20170828*/

    /* Start PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	/**********************************************************************/
	/* (60) Additional POS Data */
	/**********************************************************************/
	if (GetVisaField(VISA_ADTNL_POS_INFO, VisaInfo, AddPos, &AddPosLen) == SUCCESS)
	{
		GetVisaField(VISA_MERCHANT_TYPE, VisaInfo, MerchantType, &tmpFieldLen);
		GetVisaField(VISA_POS_ENTRY_MODE, VisaInfo, PosMode, &tmpFieldLen);
		GetVisaField(VISA_POS_CONDITION, VisaInfo, PosCond, &tmpFieldLen);
		VisaToIsoPosMode(MerchantType, PosMode, PosCond, AddPos, tmpField2, sEcomAuthInfo, &nEcomInfoLen);

		if (nEcomInfoLen > 0)
		{
			if (GetIsoField (ISO_ADTNL_DATA_PRIV , IsoInfo, FieldData, &FieldLen) == SUCCESS)
        	{
            	AnalyseTlvPrivate (FieldData, FieldLen, &sTlvInfo);
        	}

			PutTlvPrivate(ECOM_AUTH_INFO, &sTlvInfo, sEcomAuthInfo, nEcomInfoLen);
			memset(FieldData, 0, sizeof(FieldData));
            FieldLen = PrivateBuildTlv (FieldData, &sTlvInfo);

            if ( FieldLen > 0 )
			{
				PutIsoField (ISO_ADTNL_DATA_PRIV , IsoInfo, FieldData , FieldLen);
			}
		}
	}
	/* End PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/

	/**********************************************************************/
	/* (61) Network Pos Data */
	/**********************************************************************/
	if (GetIsoField(ISO_NET_POS_DATA, IsoInfo, tmpField, &FieldLen) == SUCCES)
		AnalyseTlvPrivate(tmpField, FieldLen, &sTlvInfo);
	else
		InitTlvPrivate(&sTlvInfo);

	PutTlvPrivate(RESPONSE_CODE_TAG, &sTlvInfo, sVisaRespCode, 2);
	FieldLen = PrivateBuildTlv(tmpField, &sTlvInfo);
	if (FieldLen > 0)
		PutIsoField(ISO_NET_POS_DATA, IsoInfo, tmpField, FieldLen);

	/* 62 Fields Cps Data */
	if (GetVisaField(VISA_CPS_FIELDS, VisaInfo, FieldData, &FieldLen) == SUCCESS)
	{
		AnalyseCps(FieldData, &CpsInfo);
		InitTlvPrivate(&sTlvPrivateInfo);
		if (GetCpsSubField(CPS_ACI, &CpsInfo, FieldData, &FieldLen) == SUCCESS)
		{
			PutTlvPrivate(CPS_ACI_TAG, &sTlvPrivateInfo, FieldData, 1);
		}
		if (GetCpsSubField(CPS_TRANS_ID, &CpsInfo, FieldData, &FieldLen) == SUCCESS)
		{
			PutTlvPrivate(CPS_TRANS_ID_TAG, &sTlvPrivateInfo, FieldData, 15);
		}
		if (GetCpsSubField(CPS_VALIDATION_CODE, &CpsInfo, FieldData, &FieldLen) == SUCCESS)
		{
			sprintf(sLine, ">>>>> VALID CODE %s", FieldData);
			trace_event(sLine, TRACE);
			PutTlvPrivate(CPS_VALIDATION_DATE_TAG, &sTlvPrivateInfo, FieldData, FieldLen);
		}
		/* START HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.1*/
		if (GetCpsSubField(CPS_PRODUCT_ID, &CpsInfo, FieldData, &FieldLen) == SUCCESS)
		{
			PutTlvPrivate(CPS_PRODUCT_ID_TAG, &sTlvPrivateInfo, FieldData, FieldLen);
		}
		/* END HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.1*/

		/* START HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.2*/
		if (GetCpsSubField(CPS_SPEND_QUALIFIED_IND, &CpsInfo, FieldData, &FieldLen) == SUCCESS)
		{
			PutTlvPrivate(CPS_SPEND_QUALIFIED_INDICATOR, &sTlvPrivateInfo, FieldData, FieldLen);
		}
		/* END HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.2*/
		FieldLen = PrivateBuildTlv(sCpsTlvData, &sTlvPrivateInfo);

		PutIsoField(ISO_CPS_FIELDS, IsoInfo, sCpsTlvData, FieldLen);
	}

	/*Field 63 */
	/*Start AEA20210712 Enh21.Q4 Art9.3.3*/
	if (GetIsoField(ISO_NETWORK_DATA, IsoInfo, tmpField, &FieldLen) == SUCCES)
		AnalyseTlvPrivate(tmpField, FieldLen, &sTlvInfo);
	else
		InitTlvPrivate(&sTlvInfo);
	if (GetVisaField(VISA_PRIV_USE_FIELD, VisaInfo, FieldData, &FieldLen) == SUCCES)
	{
		AnalyseVisaPuf(FieldData, &VisapufInfo);
		if (GetVisaPufSubField(VPUF_MER_ID, &VisapufInfo, FieldData, &FieldLen) == SUCCES)
		{
			AddTlvPrivate (NET_VISA_MERC_ID, &sTlvInfo, tmpField,tmpFieldLen);
		}

		/*tmpFieldLen = PrivateBuildTlv(tmpField, &TlvInfo);
		if( tmpFieldLen > 0 )
		{
			AddIsoField(ISO_NETWORK_DATA, IsoInfo, tmpField, tmpFieldLen);
		}*/
	}
	/*End AEA20210712 Enh21.Q4 Art9.3.3*/

	/*Start PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	if (GetVisaField(VISA_TRANS_DESC, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTransSpecTlv);
		
		if( GetTlvInfoField (B1_F104_BUSINESS_APP_ID, &kVisaTransSpecTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{ 
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaAppIdentTlv);
			memset(tmpField, '\0', sizeof(tmpField));
			if( GetTlvInfoField (B1_F104_T57_MAX_PROC_DATE, &kVisaAppIdentTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				memset(tmpField2, '\0', sizeof(tmpField2));
				julian_to_gmt(tmpField,tmpField2);
				AddTlvPrivate(NET_MAX_PROC_DATE_TAG, &sTlvInfo, tmpField2,strlen(tmpField2));
			}
		}
	}
	/*End PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/

	/*Start IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	if (GetVisaField(VISA_ADTNL_RESP_DATA, VisaInfo, FieldData, &FieldLen) == SUCCES)
	{
		sTokenResponse[0]=FieldData[2];
		AddTlvPrivate (NET_TOKEN_PROGAM, &sTlvInfo, sTokenResponse ,1);
	}
	memset(tmpField, 0, sizeof(tmpField));
	tmpFieldLen = PrivateBuildTlv(tmpField, &sTlvInfo);
	if( tmpFieldLen > 0 )
	{
		PutIsoField(ISO_NETWORK_DATA, IsoInfo, tmpField, tmpFieldLen);/*ZKO 29072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	}
	/*End IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/


	/* Smart Data */
	VisaToIso55Icc    (	VisaInfo,IsoInfo);

	memset(FieldData, 0, sizeof(FieldData));
	memset(tmpField, 0, sizeof(tmpField));
	InitTlvPrivate(&sTlvInfo);
	InitTlvPrivate(&sTlvPrivateInfo);

	trace_event("End  BuildAutReplyFromNw(OK)", PROCESSING);
	return (OK);
}

/****************************************************************************/
/* AutReplyToNw()                                                         */
/* */
/* Constructs and sends an authorization response message (0110) to VISA. */
/****************************************************************************/
int AutReplyToNw(	int nIndexCtx,
					TSIsoInfo * IsoInfo,
					TSVisaInfo * OrigVisaInfo)
{
	TSVisaInfo      NewVisaInfo;
	TSTlvPrivate    kTlvInfo; /*PLUTONL-5283 FZL20221013*/
	char            OrigVisaElectCom[256];/*PLUTONL-5283 FZL20221013*/
	char            sProcCode[6 + 1];
	char            FieldData[BUF_LG_MAX /*256*/];		/*AMER20171123 PROD00049633*/
	char            sPufData[256];
	char            sBuffer[256];
	char            tmpField[256];
	char            tmpField1[256];
	int             FieldLen;
	int             Length;
	char            CvvResult[1 + 1];
	char 			cavv_result[  1 + 1 ];
	char 			caav_field [256];
	char           cavv_valid_result = '2';
	char           cavv_invalid_result = '1';
	int             i;
	TSVisaPufInfo   VisapufInfo;
	char            sLine[MAX_LINE_TRC];
	char			PosCond[3];
	char           sVisaRespCode[256];
	char			sTransAmount[24];
	char			sTransCurrency[3 + 1];
	char			sBillingCurrency[3 + 1];
	char			sIssBillingCurrency[3 + 1];
	char			sOrigBillingInfo[64];
	char            AvrResult[1 + 1];
	char			sIsoRespCode[6 + 1];
	char			sIsoProcCode[6 + 1];
	char			sNewBillingAmount[24];
	int				nResult;
	int 			billing_amnt_presence = 0;
	int 			nF44_len = 0;
	BitMappedInfo	kVIPufData;					/*AMER20171121*/
	BitMappedInfo	kVIPufDataResp;					/*AMER20171121*/
	char          	vBitMap[3 +1];				/*AMER20171121*/
	char            Priv_Use_Field[255 + 1];	/*AMER20171121*/
	char            sVisaResCodes [25+1]; /*NAB08012010*/
	int	        NwAddRespDataLen =0; /*NAB08012010*/
/*PLUTONL-5283 FZL20221013*/
	char            PSD2_data[256];
	TlvInfo			kVisaTlvInfo;
	TlvInfo			OrigVisaElectComTlv;
	TlvInfo			ExemAuthInfo;
	TlvInfo         VisaEuPsd2Tlv;
	int             nLen;
    int             flag_fld_48_exist =0;
    char		   	sActionFlag[ 1 + 1 ];
/*PLUTONL-5283 FZL20221013*/


	trace_event("Start  AutReplyToNw()", PROCESSING);

	memset(sIsoRespCode,'\0',sizeof(sIsoRespCode));
	memset(caav_field               , 0 , sizeof(caav_field         ));
	memset(sBuffer, 0, sizeof(sBuffer));
	memset ( PosCond,  0 , sizeof ( PosCond ));
	memset ( sIsoProcCode,  0 , sizeof ( sIsoProcCode ));
	memset ( sTransAmount,  0 , sizeof ( sTransAmount ));
	memset ( sOrigBillingInfo,  0 , sizeof ( sOrigBillingInfo ));
	memset ( sNewBillingAmount,  0 , sizeof ( sNewBillingAmount ));
	memset ( sTransCurrency,  0 , sizeof ( sTransCurrency ));
	memset ( sBillingCurrency,  0 , sizeof ( sBillingCurrency ));
	memset ( sIssBillingCurrency,  0 , sizeof ( sIssBillingCurrency ));
	memset ( tmpField,  0 , sizeof ( tmpField ));
	memset ( tmpField1,  0 , sizeof ( tmpField1 ));
	memset ( sVisaResCodes,  0 , sizeof ( sVisaResCodes ));
	/*PLUTONL-5283 FZL20221013*/
	memset ( PSD2_data,  0 , sizeof ( PSD2_data ));
   	memset ( OrigVisaElectCom,  0 , sizeof ( OrigVisaElectCom ));
    /*PLUTONL-5283 FZL20221013*/
	memset(sActionFlag,'\0',sizeof(sActionFlag));

	dump_p7_struct(IsoInfo);
	InitVisaInfo(&NewVisaInfo);
	InitTlvPrivate(&kTlvInfo);
	InitVisaVIPPufDataTlv(&kVIPufData);		/*AMER20171121*/
	InitVisaVIPPufDataTlv(&kVIPufDataResp);		/*AMER20171121*/
	InitVisaElectComDataTlv(&OrigVisaElectComTlv); /*PLUTONL-5283 FZL20221013*/
	NewVisaInfo.nMsgType = 110;

	GetIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, &FieldLen);
	GetIsoField(ISO_BILLING_CRNCY, IsoInfo, sIssBillingCurrency, &FieldLen);

	GetVisaField (VISA_TRANS_AMOUNT,OrigVisaInfo, sTransAmount, &FieldLen);
	GetVisaField (VISA_TRANS_CRNCY,OrigVisaInfo, sTransCurrency, &FieldLen);
	GetVisaField (VISA_BILLING_CRNCY,OrigVisaInfo, sBillingCurrency, &FieldLen);
	/*NAB18072019 Certif SG si PWC n a pas verifiÃƒÂ© le CVV2 et VISA a verifie alors on envoie la valeur de VISA */
	GetVisaField(VISA_ADTNL_RESP_DATA, OrigVisaInfo, sVisaResCodes, &NwAddRespDataLen) ;

	GetIsoField  (ISO_RESPONSE_CODE, IsoInfo, sIsoRespCode, &FieldLen);

	if(memcmp(sIsoRespCode,APPROVED_ACTION,3) != 0) /*IBO20231115 PLUTONL-7395*/
	{
		GetActionFlag(nIndexCtx, sIsoRespCode, sActionFlag);
	}
	else
	{
		sActionFlag[0] = ACTION_FLAG_APPROVAL;
	}

	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, &FieldLen) == SUCCESS)
	{
		AnalyseTlvPrivate(FieldData, FieldLen, &kTlvInfo);
		flag_fld_48_exist = 1;/*PLUTONL-5283 FZL20221013*/
	}

	if( memcmp(sIsoRespCode,P7_F039_PARTIAL_APPROVAL,P7_F039_LEN) == 0 )
	{
		GetTlvPrivate(ORIG_BILLING_INFO, &kTlvInfo, sOrigBillingInfo, &FieldLen);
		memcpy(FieldData, sOrigBillingInfo + OBAI_REPL_BILL_AMOUNT_O,OBAI_REPL_BILL_AMOUNT_L);
		FieldData[OBAI_REPL_BILL_AMOUNT_L]='\0';
		nResult = AdjustIsoAmount (  nIndexCtx , FieldData , sBillingCurrency , sNewBillingAmount );
		sprintf(sLine,"New Billing amount [%s]",sNewBillingAmount);
		trace_event(sLine,TRACE);
	}


	for (i = 0; i < VISA_AUDIT_NBR; i++)
	{
		switch(i)
		{
			case VISA_BILLING_AMOUNT:
				if( memcmp(sIsoRespCode,P7_F039_PARTIAL_APPROVAL,P7_F039_LEN) == 0 )
				{
					if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
					{
						AddVisaField(i, &NewVisaInfo, sNewBillingAmount, B1_F006_LEN);
						billing_amnt_presence = 1;	/*AMER20171116 PROD00049633*/
					}
					else
					{
						PutVisaField(VISA_TRANS_AMOUNT, &NewVisaInfo, sNewBillingAmount, B1_F004_LEN);/*NAB24042020*/
					}
				}
				/*AMER20171116 PROD00049633: Don't send F6 if it's not a partial approval*/
				/*else
				{
					if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
						AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
				}*/

				break;
			case VISA_BILLING_CNVRSN_RATE:
				/*if( memcmp( sIsoProcCode,P7_F003_PURCHASE,P7_F003_P1_LEN) == 0 )*/
				if( billing_amnt_presence == 1 )	/*AMER20171116 PROD00049633: Send F10 just if F6 is present*/
				{
					if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
						AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
				}
				break;
			default:
			if (VisaEchoedField_0100[i])
			{
				if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
					AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
			}
			break;
		}

	}

	/* restore VISA field 11 from ISO field 48 */
	if (GetTlvPrivate(EXTERNAL_STAN_TAG, &kTlvInfo, FieldData, &FieldLen) == SUCCESS)
		AddVisaField(VISA_AUDIT_NBR, &NewVisaInfo, FieldData, FieldLen);

	for (i = VISA_AUDIT_NBR + 1; i < VISA_PAN_EXTENDED; i++)  /*PLUTONL-5283 FZL20221013*/
	{
		if (VisaEchoedField_0100[i])
		{
			if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
		}
		else if (	(i == VISA_CNVRSN_DATE )		&&
					memcmp( sIsoProcCode,P7_F003_PURCHASE,P7_F003_P1_LEN) == 0)
		{

			if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
		}

	}

	/*START PLUTONL-5283 FZL20221013*/
	memset(FieldData, 0, sizeof(FieldData));

    if (GetVisaField(VISA_PAN_EXTENDED /*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/, OrigVisaInfo, sBuffer, &FieldLen) == SUCCESS)
	{
		InitVisaElectComDataTlv(&OrigVisaElectComTlv);
		AnalyseTlvInfo(sBuffer, FieldLen, &OrigVisaElectComTlv);

		if ( GetTlvInfoField(B1_F34_EU_PSD2_SCA_CSC, &OrigVisaElectComTlv, OrigVisaElectCom, &nLen) == SUCCESS && GetTlvPrivate(PSD2_DATA, &kTlvInfo, PSD2_data, &FieldLen) == SUCCESS)
		{
			InitExempAuthInfoDataTlv(&ExemAuthInfo);
			AnalyseTlvInfo(PSD2_data, FieldLen, &ExemAuthInfo);

			InitVisaEuPsd2ScaCsc(&VisaEuPsd2Tlv);
			AnalyseTlvInfo(OrigVisaElectCom, nLen, &VisaEuPsd2Tlv);

			InitVisaEuPsd2ScaCsc(&kVisaTlvInfo);

			if ( GetTlvInfoField(P7_F048_S27_AUTH_EXEMPTION_RES, &ExemAuthInfo, tmpField, &FieldLen) == SUCCESS)
			{

				if(  GetTlvInfoField(B1_F34T4A_TRST_MER_EXEM, &VisaEuPsd2Tlv, FieldData, &FieldLen ) == SUCCESS )
				{
					if(tmpField[P7_F048_S27T002_TRUSTED_MER_OFST] == '2')
					{
						AddTlvInfoField(B1_F34T4A_TRST_MER_EXEM, &kVisaTlvInfo, "2", 1);
					}
					else if(tmpField[P7_F048_S27T002_TRUSTED_MER_OFST] == '3')
					{
						AddTlvInfoField(B1_F34T4A_TRST_MER_EXEM, &kVisaTlvInfo, "3", 1);
					}
				}

				if( GetTlvInfoField(B1_F34T4A_LOW_VAL_EXEM, &VisaEuPsd2Tlv, FieldData, &FieldLen ) == SUCCESS )
				{
					if(tmpField[P7_F048_S27T002_LVA_OFST] == '2')
					{
						AddTlvInfoField(B1_F34T4A_LOW_VAL_EXEM, &kVisaTlvInfo, "2", 1);
					}
					else if(tmpField[P7_F048_S27T002_LVA_OFST] == '3')
					{
						AddTlvInfoField(B1_F34T4A_LOW_VAL_EXEM, &kVisaTlvInfo, "3", 1);
					}
				}

				if( GetTlvInfoField(B1_F34T4A_SPC, &VisaEuPsd2Tlv, FieldData, &FieldLen ) == SUCCESS )
				{
					if(tmpField[P7_F048_S27T002_SCP_IND_OFST] == '2')
					{
						AddTlvInfoField(B1_F34T4A_SPC, &kVisaTlvInfo, "2", 1);
					}
					else if(tmpField[P7_F048_S27T002_SCP_IND_OFST] == '3')
					{
						AddTlvInfoField(B1_F34T4A_SPC, &kVisaTlvInfo, "3", 1);
					}
				}

				if( GetTlvInfoField(B1_F34T4A_TRA, &VisaEuPsd2Tlv, FieldData, &FieldLen ) == SUCCESS )
				{
					if(tmpField[P7_F048_S27T002_TRA_OFST] == '2')
					{
						AddTlvInfoField(B1_F34T4A_TRA, &kVisaTlvInfo, "2", 1);
					}
					else if(tmpField[P7_F048_S27T002_TRA_OFST] == '3')
					{
						AddTlvInfoField(B1_F34T4A_TRA, &kVisaTlvInfo, "3", 1);
					}
				}

				nLen = BuildTlvInfoBuffer (tmpField, &kVisaTlvInfo);

				if( nLen > 0 )
				{
					PutTlvInfoField(B1_F34_EU_PSD2_SCA_CSC, &OrigVisaElectComTlv, tmpField, nLen);
	}
			}

		}/*Start PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*//*Start PLUTONL-5757 FZL20230125*/
				nLen = BuildTlvInfoBuffer(tmpField, &OrigVisaElectComTlv);

				if (nLen > 0)
				{
			/*AOU17102024*/
			if (VisaEchoedField_0100[VISA_PAN_EXTENDED])
			{
				AddVisaField(VISA_PAN_EXTENDED, &NewVisaInfo, tmpField, nLen);
			}
		}
		/* END PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*//*END PLUTONL-5757  FZL20230125*/
    }

	memset(sBuffer, 0, sizeof(sBuffer));

	for (i = VISA_PAN_EXTENDED /*PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/ +1 ; i < VISA_AUTHOR_ID ; i++){
		if (VisaEchoedField_0100[i])
		{
			if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
		}
	}
	/* END PLUTONL-5283 FZL20221013*/
	/* Field 38 */

	if (sActionFlag[0] == ACTION_FLAG_APPROVAL) /*IBO20231115 PLUTONL-7395*/
	{
	if (GetVisaField (VISA_RESPONSE_CODE,OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
	{
		if ( memcmp(FieldData,B1_F039_SUSPECTED_FRAUD,B1_F039_LEN)== 0 )
			trace_event("VISA RESPONSE CODE : VRM",PROCESSING);
		else
		{
			if (GetIsoField(ISO_AUTHOR_ID, IsoInfo, FieldData, &FieldLen) == SUCCES)
				AddVisaField(VISA_AUTHOR_ID, &NewVisaInfo, FieldData, FieldLen);
		}
	}
	else
	{
		if (GetIsoField(ISO_AUTHOR_ID, IsoInfo, FieldData, &FieldLen) == SUCCES)
			AddVisaField(VISA_AUTHOR_ID, &NewVisaInfo, FieldData, FieldLen);
	}
	}

	/* Field 39 */
	if ( GetIsoField  (ISO_RESPONSE_CODE, IsoInfo, sIsoRespCode, &FieldLen) == SUCCES )
	{
		GetVisaField (VISA_POS_CONDITION, OrigVisaInfo, PosCond, &FieldLen);
		if ( ( memcmp(sIsoRespCode, P7_F039_APPROVAL, P7_F039_LEN) == 0 ) &&
			( memcmp(PosCond,B1_F025_ACC_VERIF,B1_F025_LEN) == 0) )
		{
			memcpy(sVisaRespCode,B1_F039_NO_RSN_TO_DECLINE,B1_F039_LEN);
		}
		else
		{

			if ( TranslateRespCode (nIndexCtx, ResourceId, sIsoRespCode, "PR", sVisaRespCode) != SUCCESS )
			{
				if ( !memcmp(sIsoRespCode, P7_F039_APPROVAL, P7_F039_LEN) )
				{
					trace_event(">>> WARNNING. SEND 00 FOR AN APPROVED TRX IF RESP CODE TRANSLAT. IS NOT FOUND <<<", FATAL);
					strncpy(sVisaRespCode,B1_F039_APPROVAL, B1_F039_LEN);
				}
				else
				{
					trace_event(">>> WARNNING. SEND 05 FOR A DECLINE TRX IF RESP CODE TRANSLAT. IS NOT FOUND <<<", FATAL);
					strncpy(sVisaRespCode,B1_F039_DECLINE, B1_F039_LEN);
				}
			}
		}
		AddVisaField (VISA_RESPONSE_CODE, &NewVisaInfo, sVisaRespCode, B1_F039_LEN);
	}

	for (i = VISA_RESPONSE_CODE + 1; i < VISA_ADTNL_RESP_DATA; i++)
	{
		if (VisaEchoedField_0100[i])
			if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
	}

	/* VISA field 44 is in ISO field 48 in TLV format */
	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, &FieldLen) == SUCCESS)
	{
		InitTlvPrivate(&kTlvInfo);
		AnalyseTlvPrivate(FieldData, FieldLen, &kTlvInfo);
		/* Field 44 */
		/* Subfield 44.5: CVV Results Code */
		/* Subfields 44.1 to 44.4 are blank padded */
		memset(AvrResult, 0, sizeof(AvrResult));
		memset(CvvResult, 0, sizeof(CvvResult));
		memset(FieldData, 0, sizeof(FieldData));
		memset(FieldData, ' ', 25);

		FieldData[0] = '5';

		if (GetTlvPrivate(ADDRESS_VERIF_REP_TYPE_TAG, &kTlvInfo, AvrResult, &FieldLen) == SUCCESS)
		{

			switch(AvrResult[0])
			{
				case 'A': AvrResult[0]='A'; break;
				case 'N': AvrResult[0]='N'; break;
				case 'U': AvrResult[0]='N'; break;
				case 'X': AvrResult[0]='D'; break;
				case 'W': AvrResult[0]='Z'; break;
				case '0': AvrResult[0]='I'; break;
			}


			FieldData[1] = AvrResult[0];

		}

		if (GetTlvPrivate(CVV_RESULT_CODE_TAG, &kTlvInfo, CvvResult, &FieldLen) == SUCCESS)
		{

			sprintf(sLine, " CvvResult = [%.1s] ", CvvResult);
			trace_event(sLine, TRACE);
			switch (CvvResult[0]) {
			case '0':
			case OK:
				FieldData[4] = '2';

				break;
			case '1':
			case VERIFICATION_FAILED:
				FieldData[4] = '1';

				break;
			case CRYPTOGRAPHIC_ERROR:
				break;
			case HSM_MALFUNCTION:
				break;
			default:
				break;

			}
		}
		if (GetTlvPrivate(CVV2_RESULT_CODE_TAG, &kTlvInfo, CvvResult, &FieldLen) == SUCCESS)
		{

			sprintf(sLine, " CvvResult2 = [%.1s] ", CvvResult);
			trace_event(sLine, TRACE);

			switch (CvvResult[0]) {
			case OK:
			case '0':
				FieldData[10] = 'M';

				break;
			case VERIFICATION_FAILED:
			case '1':
				FieldData[10] = 'N';

				break;
			case CRYPTOGRAPHIC_ERROR:
				break;
			case HSM_MALFUNCTION:
				break;
			default:
				break;
			}
		}
		if (GetVisaField(VISA_PRIV_USE_FIELD, OrigVisaInfo, tmpField, &FieldLen) == SUCCES)
		{
			InitVisaPufInfo(&VisapufInfo);
			AnalyseVisaPuf(tmpField, &VisapufInfo);
			memset(sPufData, 0, sizeof(sPufData));

			if (GetVisaPufSubField(VPUF_CVV2, &VisapufInfo, sPufData, &Length) == SUCCES)
			{
				if (sPufData[0] == B1_F126_P10_CVV2_ILGBL)
				{
					FieldData[10] = B1_F044_P10_CVV2_NOPROC;
				}
				else if (sPufData[0] == B1_F126_P10_NO_CVV2)
				{
					FieldData[10] = B1_F044_P10_CVV2_IS_ON_CRD;
				}
				else if (sPufData[0] == B1_F126_P10_CVV2_P && CvvResult[0] != '0' && CvvResult[0] != '1')
				{
					/*NAB1807 Certif SG si PWC n a pas verifiÃƒÂ© le CVV2 et VISA a verifie alors on envoie la valeur de VISA */
					if(CvvResult[0] == '4' && NwAddRespDataLen >= 11 && sVisaResCodes[10] !=' ')
					{
						FieldData[10] = sVisaResCodes[10];
					}
					else
						FieldData[10] = B1_F044_P10_CVV2_NOPROC;
				}
			}
		}
		if (GetTlvPrivate (CAVV_TAG       ,&kTlvInfo , caav_field ,&FieldLen) == SUCCES )
		{

			sprintf(sLine, " 1 CAVV_TAG = [%s]  ",   caav_field);
			trace_event(sLine, TRACE);

			/*Start AMER20171120*/
			if (GetTlvPrivate (CAVV_RESULT_CODE_TAG, &kTlvInfo, cavv_result , &FieldLen) == SUCCESS)
			{
				sprintf(sLine, " cavv_result = [%.1s] ", cavv_result);
				trace_event(sLine, TRACE);

				switch ( cavv_result [ 0 ] )
				{
					case  P7_F048_P54_CAVV_AUTHEN_OK:
					case  P7_F048_P54_CAVV_AUTHEN_NOK:
					case  P7_F048_P54_CAVV_ATTEMP_OK:
					case  P7_F048_P54_CAVV_ATTEMP_NOK:
					case  P7_F048_P54_CAVV_VALID_OK_VISA:/*PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
					case  P7_F048_P54_CAVV_NOK_ATT_PART_ACS_NOT_AVAILABLE: /*NAB21062019 Certif SG */
						FieldData [14] = cavv_result[0];
						break;
					/*case P7_F048_P54_CAVV_VAL_NOT_PERF :
					case P7_F048_P54_CAVV_NOT_VALIDATED_ATTE : PLUTONL-3134*/
					default: /* FZL20220923 PLUTONL-5214  */
						if (NwAddRespDataLen >= 15 && sVisaResCodes[14] !=' ') /*PLUTONL-3043*/
						{
							FieldData [14] = sVisaResCodes[14];
						}
						else
							FieldData [14] = '0';
						break;

				}
			}
			/*END AMER20171120*/

			/*char           AddPos[11];
			int            AddPosLen;
			char			FieldData1[512];
			TSTlvPrivate    TlvInfo1;
			memset ( cavv_result , 0 , sizeof ( cavv_result ));
			memset (FieldData1 , 0 , sizeof ( FieldData1 ));
			memset ( AddPos ,  0 , sizeof ( AddPos  ));
			InitTlvPrivate (&TlvInfo1);
			if (GetIsoField (ISO_NET_POS_DATA, IsoInfo, FieldData1, &FieldLen)              == SUCCESS)
			{
				AnalyseTlvPrivate (FieldData1, FieldLen, &TlvInfo1);
				GetTlvPrivate (ADTNL_POS_DATA_TAG, &TlvInfo1, AddPos , &AddPosLen);
			}

			if ( !memcmp (AddPos + 8 , "05" , 2) )
			{
				cavv_valid_result = '2';
				cavv_invalid_result = '1';
			}

			if ( !memcmp (AddPos + 8 , "06" , 2) )
			{
				cavv_valid_result = '3';
				cavv_invalid_result = '4';
			}

			if (GetTlvPrivate (CAVV_RESULT_CODE_TAG, &TlvInfo, cavv_result , &FieldLen) == SUCCESS)
			{
				switch ( cavv_result [ 0 ] )
				{
				case '0' :
				case OK :
					FieldData [14 ] = cavv_valid_result;
					break;
				case '1' :
				case VERIFICATION_FAILED :
					FieldData [14 ] = cavv_invalid_result;
					break;
				case CRYPTOGRAPHIC_ERROR :
					break;
				case HSM_MALFUNCTION :
					break;
				default  : break;
				}
			}
			else
			{
				FieldData [14 ] = cavv_valid_result;
			} */
		}
		/***************************************/
		trace_event("==> 44.8 <==", PROCESSING);
		memset(tmpField, 0, sizeof(tmpField));
		if ( GetIsoField  (ISO_ICC_DATA, IsoInfo, tmpField, &FieldLen) == SUCCESS )
		{

			TSTagIso tlvInfo;
			dump_buffer(tmpField, FieldLen, 'A', 'X', 'X');
			InitIsoIcTag  ( &tlvInfo );
			AnalyseIsoIc  ( tmpField,FieldLen,&tlvInfo);


			if ( GetIsoIcTag (ISO_TAG_APP_CRYPTOGRAM, &tlvInfo, tmpField, &FieldLen) == SUCCESS )
			{
				if( GetIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA, &tlvInfo, tmpField, &FieldLen) == SUCCESS )
				{
					FieldData [8] = '2';
				}

				else if (GetTlvPrivate (CRYPT_RESULT_CODE_TAG, &kTlvInfo, tmpField , &FieldLen) == SUCCESS)
				{
					switch(tmpField[0])
					{
					case '0': FieldData [8] = '2'; break;
					case '1': FieldData [8] = '1'; break;
					case '2':
					case '3':
					case '4':
						break;
					}
				}
			}
		}

		/*Start AMER20171116 PROD00049633*/
		for(i = 0; i < 25; i++)
		{
			if(FieldData[i] != ' ')
				nF44_len = i+1;
		}
		/*End AMER20171116 PROD00049633*/

		AddVisaField(VISA_ADTNL_RESP_DATA, &NewVisaInfo, FieldData, nF44_len /*strlen(FieldData)*/); /*AMER20171116 PROD00049633*/
	}

	for (i = VISA_ADTNL_RESP_DATA + 1; i < VISA_ADTNL_DATA_PRIV; i++)
	{
		if (VisaEchoedField_0100[i])
			if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
	}

	if (GetVisaField(VISA_ADTNL_DATA_PRIV, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
		AddVisaField(VISA_ADTNL_DATA_PRIV, &NewVisaInfo, FieldData, FieldLen);


	for (i = VISA_ADTNL_DATA_PRIV + 1; i < VISA_ADTNL_AMOUNTS; i++)
	{
		if (VisaEchoedField_0100[i])
		{
			if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
		}
		/*else if (	(i==VISA_BILLING_CRNCY )		&&
					memcmp( sIsoProcCode,P7_F003_PURCHASE,P7_F003_P1_LEN) == 0 )*/

		else if ((i==VISA_BILLING_CRNCY ) && billing_amnt_presence == 1 )	/*AMER20171116 PROD00049633: Send F51 just if F6 is present*/
		{
			if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
		}
	}

	/* Field 54: for Balance Inquiry Messages */
	memset(sProcCode, 0, sizeof(sProcCode));
	memset(FieldData, 0, sizeof(FieldData));
	memset(tmpField, 0, sizeof(tmpField));

	/*AMER20171123 PROD00049633: Send F54 in the response just if the authorization is a balance inquiry or withdrawal or the response is a partial approval*/
	GetVisaField(VISA_PROC_CODE, OrigVisaInfo, sProcCode, &FieldLen);
	GetIsoField  (ISO_RESPONSE_CODE, IsoInfo, sIsoRespCode, &FieldLen);
	if((GetIsoField(ISO_ADTNL_AMOUNTS, IsoInfo, FieldData, &FieldLen) == SUCCESS)
		&& (((memcmp(sProcCode, B1_F003_P1_WITHDRAWAL, 2) == 0)  || (memcmp (sProcCode, B1_F003_P1_BALANCE_INQ, 2) == 0))
		&& (memcmp (sIsoRespCode, P7_F039_APPROVAL, P7_F039_LEN) == 0))/* NAB13032020 Balance information F054 should not be present if the author is declined*/
		|| (memcmp (sIsoRespCode, P7_F039_PARTIAL_APPROVAL, P7_F039_LEN) == 0))
	{
			IsoToVisaAddAmount(FieldData, tmpField);
			AddVisaField(VISA_ADTNL_AMOUNTS, &NewVisaInfo, tmpField, strlen(tmpField));
			/*NAB13032020  the account type subfield code of every data set in Field 54 must be the account type code in field 3 of the response*/
		if(memcmp(sProcCode +2 ,tmpField,2) != 0)
		{

			memcpy(sProcCode +2 , tmpField  , 2);
			PutVisaField(VISA_PROC_CODE , &NewVisaInfo, sProcCode, B1_F003_LEN);


		}
		/*Start NAB05042020 */
		if (memcmp (sIsoRespCode, P7_F039_PARTIAL_APPROVAL, P7_F039_LEN) == 0)
		{

			memset (tmpField, 0, sizeof(tmpField));
			memcpy(tmpField ,B1_F054_ACCT_TYPE_UN ,B1_F054_ACCT_TYPE_LEN);

			memcpy(tmpField + 2,B1_F054_AT_ORIG_AMNT ,B1_F054_AT_LEN);

			GetVisaField (VISA_TRANS_CRNCY, OrigVisaInfo, FieldData, &FieldLen);

			memcpy(tmpField + 4, FieldData,3);
			tmpField[7] = 'C';
			GetVisaField (VISA_TRANS_AMOUNT, OrigVisaInfo, FieldData, &FieldLen);

			memcpy(tmpField + 4 + 3 + 1 , FieldData,12);
			memset (FieldData, 0, sizeof(FieldData));
			memcpy(FieldData, tmpField, 20);

			PutVisaField (VISA_ADTNL_AMOUNTS,&NewVisaInfo,  FieldData, 20);
		}

		/*End NAB05042020*/
	}

	/*GetVisaField(VISA_PROC_CODE, OrigVisaInfo, sProcCode, &FieldLen);*/

	/** Field 55 **/
	trace_event("==> ICC 000 <==", FATAL);
	if (GetIsoField(ISO_ICC_DATA, IsoInfo, FieldData, &FieldLen) == SUCCESS)
	{
		TSTagIso        IsoIccInfo;
		TSTagIso        VisaIccInfo;
		char            sIccBuffer[1024];
		int             tmpFieldLen;

		trace_event("==> ICC 001 <==", FATAL);
		InitIsoIcTag(&VisaIccInfo);
		InitIsoIcTag(&IsoIccInfo);
		AnalyseIsoIc(FieldData, FieldLen, &IsoIccInfo);
		if(GetIsoIcTag  (ISO_TAG_ATC , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag (ISO_TAG_ATC , &VisaIccInfo, FieldData,  FieldLen );
		if ( memcmp(sVisaRespCode, B1_F039_APPROVAL, B1_F039_LEN) == 0 )
		{
			if(GetIsoIcTag  (ISO_TAG_ISS_AUTHEN_DATA , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA , &VisaIccInfo, FieldData,  FieldLen );

			if(GetIsoIcTag  (ISO_TAG_ISS_SCRIPT1 , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddIsoIcTag (ISO_TAG_ISS_SCRIPT1 , &VisaIccInfo, FieldData,  FieldLen );

			if(GetIsoIcTag  (ISO_TAG_ISS_SCRIPT2 , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddIsoIcTag (ISO_TAG_ISS_SCRIPT2 , &VisaIccInfo, FieldData,  FieldLen );
		}
		else
		{
			if(GetIsoIcTag  (ISO_TAG_ISS_AUTHEN_DATA , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			{
				/*sprintf(FieldData+16,"%02X%02X",sVisaRespCode[0],sVisaRespCode[1]);NAB13032020 PLUTONL-1369*/
				AddIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA , &VisaIccInfo, FieldData,  FieldLen );
			}
			if(GetIsoIcTag  (ISO_TAG_ISS_SCRIPT1 , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddIsoIcTag (ISO_TAG_ISS_SCRIPT1 , &VisaIccInfo, FieldData,  FieldLen );

			if(GetIsoIcTag  (ISO_TAG_ISS_SCRIPT2 , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddIsoIcTag (ISO_TAG_ISS_SCRIPT2 , &VisaIccInfo, FieldData,  FieldLen );
		}

		FieldLen = IsoBuildIcFld ( sIccBuffer, &VisaIccInfo);
		dump_buffer(sIccBuffer, FieldLen,'X','X','X');
		if ( FieldLen > 0 )
		{
			memset(FieldData, 0, sizeof(FieldData));
			FieldData[0] = 0x01;
			FieldData[1] = 0x00;
			FieldData[2] = FieldLen;
			memcpy(FieldData+3, sIccBuffer, FieldLen);
			AddVisaField (VISA_ICC_DATA, &NewVisaInfo, FieldData, FieldLen+3);
		}


	}
	trace_event("==> ICC 002 <==", FATAL);

	for (i = VISA_ICC_DATA + 1; i <= MAX_VISA_FIELDS; i++)
	{
		if (VisaEchoedField_0100[i])
		{
			/*if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
					AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);*/

			/*Start AMER20171121 PROD00049633*/
			if(i == VISA_PRIV_USE_FIELDS)
			{
				if (GetVisaField (VISA_PRIV_USE_FIELDS, OrigVisaInfo, tmpField, &FieldLen)== SUCCESS)
				{
					if((tmpField[0]&0xC0)==0xC0)
					{
						memset(Priv_Use_Field, 0, sizeof(Priv_Use_Field));
						Priv_Use_Field[0]=0xC0;
						Priv_Use_Field[1]=0x00;
						Priv_Use_Field[2]=0x00;
						memcpy(Priv_Use_Field+3 ,tmpField+3,4);
						AddVisaField(VISA_PRIV_USE_FIELDS, &NewVisaInfo, Priv_Use_Field, 7);
					}
					else if((tmpField[0]&0x80)==0x80)
					{
						memset(Priv_Use_Field, 0, sizeof(Priv_Use_Field));
						Priv_Use_Field[0]=0x80;
						Priv_Use_Field[1]=0x00;
						Priv_Use_Field[2]=0x00;
						memcpy(Priv_Use_Field+3 ,tmpField+3,2);
						AddVisaField(VISA_PRIV_USE_FIELDS, &NewVisaInfo, Priv_Use_Field, 5);
					}


					/*AnalyseBitMappedInfo(tmpField, FieldLen, &kVIPufData);
					memset(Priv_Use_Field, 0, sizeof(Priv_Use_Field));

					if (GetBitMappedInfoField (B1_F063_NETWORK_ID, &kVIPufData, tmpField, &FieldLen) == SUCCESS)
						AddBitMappedInfoField (B1_F063_NETWORK_ID, &kVIPufDataResp, tmpField, FieldLen);

					memset(tmpField, 0, sizeof(tmpField));
					if (GetBitMappedInfoField (B1_F063_PREAUTH_TIME_LIMIT, &kVIPufData, tmpField, &FieldLen) == SUCCESS)
						AddBitMappedInfoField (B1_F063_PREAUTH_TIME_LIMIT, &kVIPufDataResp, tmpField, FieldLen);

					sprintf(sLine, "tmpField %s,",  tmpField);
					trace_event(sLine, TRACE);

					FieldLen = BuildBitMappedInfoBuffer(Priv_Use_Field, &kVIPufDataResp);
					AddVisaField(VISA_PRIV_USE_FIELDS, &NewVisaInfo, Priv_Use_Field, 7);*/


				}
			}
			else
			{
				if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
					AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
			}
			/*End AMER20171121 PROD00049633*/
		}
	}

	/* send it to VISA */
	Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, &NewVisaInfo);

	VisaBuildHeader(RESPONSE, &(NewVisaInfo.sHeader), &(OrigVisaInfo->sHeader), Length, StationId);
	memcpy(sBuffer, (char *) &(NewVisaInfo.sHeader), VISA_HDR_LEN);
	Length += VISA_HDR_LEN;

	/*Start IBO20200517 RAWMSG*/
	if (g_sCaptureMsg[NW_OUTG_RESP] == 'Y')
	{
		LogVisaMessage(nIndexCtx, &NewVisaInfo, IsoInfo, OUTG_RESP);
	}
	/*End IBO20200517 RAWMSG*/

	dump_buffer(sBuffer, Length, 'E', 'L', 'O');

	DumpVisa(sBuffer);
	if (WriteLineMsg(sBuffer, Length))
	{
		trace_event("End  AutReplyToNw(NOK): WriteLineMsg() failed", ERROR);
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}

	/* YDA20171113 (PROD00048309) */
	/*MsgRepSent(IsoInfo->msgId,sIsoRespCode);*/
	MsgIsoRepSent(IsoInfo->msgId,IsoInfo);

	memset(sBuffer, 0, sizeof(sBuffer));
	memset ( tmpField,  0 , sizeof ( tmpField ));
	memset(tmpField1, 0, sizeof(tmpField1));
	memset(sPufData, 0, sizeof(sPufData));
	memset(caav_field , 0 , sizeof(caav_field ));

	InitVisaPufInfo(&VisapufInfo);
	InitVisaInfo(&NewVisaInfo);
	InitTlvPrivate(&kTlvInfo);
	InitVisaVIPPufDataTlv(&kVIPufData);
	InitVisaVIPPufDataTlv(&kVIPufDataResp);

	trace_event("End  AutReplyToNw(OK)", PROCESSING);
	return (OK);
}

int VisaField48ToIso(TSVisaInfo * VisaInfo,TSIsoInfo      *IsoInfo)
{
	TSTlvPrivate    kTlvInfo;
	char            FieldData[LG_MAX];
	char            FieldData1[2];
	char            IsoFieldData[LG_MAX];
	char            sRespCode[3 + 1];
	int             IsoFieldLen;
	int             FieldLen;
	int             offset;
	char            tmpField[BUF_LG_MAX]; 							/*ACH 20191021*/
	int             tmpFieldLen; 									/*ACH 20191021*/
	char          	sIsoPosMode[12 + 1]; 						/*ACH 20191107*/

	TlvInfo			kVisaElectComTlv;
	TlvInfo			kVisaTlvInfo;
	char			sEcomAuthInfo[P7_F048_P66_LEN + 1];
	int				nEcomAuthInfo=0;/*PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
	TlvInfo    		isoExemption;       /*HAL  20201903 Enh Q2.2020 ART 9.3.3*/
	char 			sPSD2AuthExempRes[3 + 1];
	int				nExempResLen = 0;

	TlvInfo    		kTokenDataIso;       				/*HAL2402 PLUTONL-4391*/
	TlvInfo			visaPanMappingData;  				/*HAL2402 PLUTONL-4391*/
	TlvInfo			kVisaTokenTlv;   	    			/*HAL2402 PLUTONL-4391*/
	TlvInfo			visaAddVerifMappingData;  			/*HAL2402 PLUTONL-4391*/
	TlvInfo			isoEcomAuthentifData; /*PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/


	trace_event("Start VisaField48ToIso()", PROCESSING);
	offset = 0;
	InitTlvPrivate(&kTlvInfo);
	memset(FieldData, 0, sizeof(FieldData));
	memset(IsoFieldData, 0, sizeof(IsoFieldData));
	memset(FieldData1, 0, sizeof(FieldData1));
	memset(tmpField, 0, sizeof(tmpField)); 						/*ACH 20191021*/
	memset (sIsoPosMode,   0, sizeof(sIsoPosMode)); 			/*ACH 20191107*/
	memset(sEcomAuthInfo, ' ', P7_F048_P66_LEN);/*HAL PROD00074189 20202801 Enh Q2.2020 Art 3.10*/
	memset(sPSD2AuthExempRes, ' ', 4);/*HAL   Enh Q2.2020 ART 9.3.3*/


	IniTknDataTlv(&kTokenDataIso);						/*HAL2402 PLUTONL-4391*/
	InitB1TokenDataTlvInfo(&visaPanMappingData);		/*HAL2402 PLUTONL-4391*/
	InitB1VerifDataTlvInfo(&kVisaTokenTlv);				/*HAL2402 PLUTONL-4391*/
	InitEcomAuthentificationInfoDataTlv(&isoEcomAuthentifData); /*PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
	
	InitB1AddrVerifDataTlvInfo(&visaAddVerifMappingData);	/*HAL2402 PLUTONL-4391*/

	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);

	/*Start HAL PROD00074189 20202801 Enh Q2.2020 Art 3.10*/
    if (GetVisaField(VISA_PAN_EXTENDED, VisaInfo, FieldData, &FieldLen) == SUCCESS)
	{
		InitVisaElectComDataTlv(&kVisaElectComTlv);
		AnalyseTlvInfo(FieldData, FieldLen, &kVisaElectComTlv);

		if(GetTlvInfoField (B1_F34_AUTHEN_DATA, &kVisaElectComTlv, FieldData, &FieldLen) == SUCCESS )
		{
			InitVisaEcomAddDataTlv(&kVisaTlvInfo);
			AnalyseTlvInfo(FieldData, FieldLen, &kVisaTlvInfo);

				if(GetTlvInfoField (B1_F34T01_PROTOCOL_VERSION_NUMBER, &kVisaTlvInfo, FieldData, &FieldLen) == SUCCESS )
				{
					if (FieldData[0] == '1')
					{
						sEcomAuthInfo[1] = P7_F048_P66_POS2_3DSEC_1_0;
					nEcomAuthInfo=MAX(nEcomAuthInfo,1+1);
					}
					else if (FieldData[0] == '2')
					{
						sEcomAuthInfo[1] = P7_F048_P66_POS2_3DSEC_2_0;
					nEcomAuthInfo=MAX(nEcomAuthInfo,1+1);
					}
					else
					{
						sEcomAuthInfo[1] = P7_F048_P66_POS2_3DSEC_UNKNOWN;
					nEcomAuthInfo=MAX(nEcomAuthInfo,1+1);
					}

				/*AddTlvPrivate(ECOM_AUTH_INFO, &kTlvInfo, sEcomAuthInfo, 2);*/  /* changed using put instead of add PLUTONL-5548  Enh 2023Q2  Visa 3.2 AHA20221221*/
			}
			/*Start PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/
			if(GetTlvInfoField (B1_F34T01_AUTHENTICATION_PROGRAM, &kVisaTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{
				if(memcmp(FieldData,VISA_DAF,2)==0)
				{
					sEcomAuthInfo[P7_F048_P66_AUTHEN_FRMWRK_OFST]=P7_F048_P66_VISA_DAF;
					nEcomAuthInfo=MAX(nEcomAuthInfo,P7_F048_P66_AUTHEN_FRMWRK_OFST+1);
				}
				else
				{
					sEcomAuthInfo[P7_F048_P66_AUTHEN_FRMWRK_OFST]=P7_F048_P66_AF_UNSPECIFED;
					nEcomAuthInfo=MAX(nEcomAuthInfo,P7_F048_P66_AUTHEN_FRMWRK_OFST+1);
				}

				}
			if(nEcomAuthInfo>0)
			{
				PutTlvPrivate(ECOM_AUTH_INFO, &kTlvInfo, sEcomAuthInfo, nEcomAuthInfo);
			}
			/*End PLUTONL-5548 Enh 2023Q2  Visa 1.2 AHA20221221*/
			/*Start PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
			if( GetTlvInfoField (B1_F34T01_AUTHEN_DATA_QUALITY_INDC, &kVisaTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{
               	AddTlvInfoField (P7_F048S26_AUTHEN_DATA_QUALITY_INDC , &isoEcomAuthentifData, FieldData, FieldLen);
			}
			/*End PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
		}

        /*Start HAL 20201903 ENH 2020Q2 ART 9.3.3*/
        if(GetTlvInfoField (B1_F34_EU_PSD2_SCA_CSC, &kVisaElectComTlv, FieldData, &FieldLen) == SUCCESS )
		{
			InitVisaEuPsd2ScaCsc(&kVisaTlvInfo);
			AnalyseTlvInfo(FieldData, FieldLen, &kVisaTlvInfo);

			InitExempAuthInfoDataTlv(&isoExemption);

			if(GetTlvInfoField (B1_F34T4A_TRST_MER_EXEM, &kVisaTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{
				sPSD2AuthExempRes[P7_F048_EXMP_RES_TRST_MER] = FieldData[0];
			    nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_TRST_MER +1);
			}

			if(GetTlvInfoField (B1_F34T4A_REASON_HONOR_EXEM, &kVisaTlvInfo, FieldData, &FieldLen) == SUCCESS )
				PutTlvInfoField (P7_F048_S27_REASON_HONOR_EXEM , &isoExemption, FieldData, FieldLen);

			if(GetTlvInfoField (B1_F34T4A_TRA_SCORE, &kVisaTlvInfo, FieldData, &FieldLen) == SUCCESS )
				PutTlvInfoField (P7_F048_S27_TRA_SCORE , &isoExemption, FieldData, FieldLen);


			if(GetTlvInfoField (B1_F34T4A_TRA, &kVisaTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{
				sPSD2AuthExempRes[P7_F048_EXMP_RES_TRA] = FieldData[0];
				nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_TRA +1);
			}

			/*Start AEA20210712 Enh21.Q4 Art9.3.3*/
			if(GetTlvInfoField (B1_F34T4A_DELG_AUTH, &kVisaTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{
				sPSD2AuthExempRes[P7_F048_EXMP_RES_DELG_AUTH] = FieldData[0];
				nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_DELG_AUTH +1);
			}
			/*End AEA20210712 Enh21.Q4 Art9.3.3*/

			if(nExempResLen > 0)
		    {
				PutTlvInfoField (P7_F048_S27_AUTH_EXEMPTION_RES , &isoExemption, sPSD2AuthExempRes, nExempResLen);
			}

		}

		tmpFieldLen = BuildTlvInfoBuffer (FieldData, &isoExemption);
        if( tmpFieldLen > 0 )
	    	PutTlvPrivate(PSD2_DATA, &kTlvInfo, FieldData, tmpFieldLen);
		/*End HAL 20201903 ENH 2020Q2 ART 9.3.3*/
	}

	/*End HAL PROD00074189 20202801 Enh Q2.2020 Art 3.10*/

	if (GetVisaField(VISA_ADTNL_RESP_DATA, VisaInfo, FieldData, &FieldLen) == SUCCESS)
	{
		/*if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, IsoFieldData, &IsoFieldLen) == SUCCESS)
			AnalyseTlvPrivate(IsoFieldData, IsoFieldLen, &kTlvInfo);*/

		/*Start ACH20191210  PLUTONL-500 */
		GetIsoField (ISO_POS_DATA, IsoInfo,  sIsoPosMode, &tmpFieldLen);
		/*int IccDataPresenceFlag= GetVisaField(VISA_ICC_DATA, VisaInfo, tmpField, &tmpFieldLen);
		int T2DataPresenceFlag= GetVisaField(VISA_T2_DATA, VisaInfo, tmpField, &tmpFieldLen);*/
		int T2DataPresenceFlag= GetIsoField (ISO_T2_DATA, IsoInfo, tmpField, &tmpFieldLen);
		int IccDataPresenceFlag= GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen);
		VisaToIsoAdtnlRespData(FieldData,T2DataPresenceFlag,IccDataPresenceFlag,sIsoPosMode, 1, &kTlvInfo);
		/*End ACH20191210  PLUTONL-500 */
	}

	/*Start HAL2402 PLUTONL-4391*/
	if (GetVisaField(VISA_ADR_VERIF_DATA, VisaInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kVisaTokenTlv);

		if (GetTlvInfoField(B1_F123_TOKEN_DATA, &kVisaTokenTlv, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AnalyseTlvInfo(tmpField, tmpFieldLen, &visaPanMappingData);

			if (GetTlvInfoField(B1_F123_T68_TOKEN, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
			{
				AddTlvInfoField(P7_F048P55_TOKEN, &kTokenDataIso, tmpField, tmpFieldLen);
			}
			if (GetTlvInfoField(B1_F123_T68_TKN_ASS_LVL, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_ASSURANCE_LVL, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_REQ_ID, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_REQ_ID, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_RANGE, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_PAN, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_REFERENCE_ID, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_PAN_REF_ID, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_EXPIRATION_DATE, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_EXP_DT, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_TYPE, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_TYPE, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(B1_F123_T68_TKN_STATUS, &visaPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_STATUS, &kTokenDataIso, tmpField, tmpFieldLen);
		}
	}
	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kTokenDataIso);
	if (tmpFieldLen > 0)
		PutTlvPrivate(TOKEN_DATA, &kTlvInfo, tmpField, tmpFieldLen);
	/*End HAL2402 PLUTONL-4391*/

	/*Start PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &isoEcomAuthentifData);
	if( tmpFieldLen > 0 )
	 	PutTlvPrivate(ECOM_AUTHENTIF_DATA, &kTlvInfo, tmpField, tmpFieldLen);
	/*End PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
	
	/*AMER20170311 Start: PROD00039737*/
	if (GetVisaField(VISA_RESPONSE_CODE, VisaInfo, FieldData, &FieldLen) == SUCCES)
	{
		if(!memcmp(FieldData,B1_F039_PARTIAL_APPROVAL,2))
		{
			if(GetVisaField (VISA_TRANS_AMOUNT, VisaInfo, FieldData, &FieldLen) == SUCCES)
				PutIsoField (ISO_TRANS_AMOUNT, IsoInfo, FieldData, FieldLen);

			/*if (GetIsoField (ISO_ADTNL_DATA_PRIV , IsoInfo, FieldData, &FieldLen) == SUCCESS)
			{
					AnalyseTlvPrivate (FieldData, FieldLen, &kTlvInfo);
			}*/

			FieldData1[0] = P7_F048_K02_TRX_AMNT_TYPE;
			PutTlvPrivate ( PARTIAL_AMOUNT_TYPE , &kTlvInfo  , FieldData1, 1 );
		}
	}
	/*AMER20170311 End: PROD00039737*/

	memset(FieldData, 0, sizeof(FieldData));
	FieldLen = PrivateBuildTlv(FieldData, &kTlvInfo);
	if (FieldLen > 0)
		PutIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, FieldLen);

	memset(FieldData, 0, sizeof(FieldData));
	InitTlvPrivate(&kTlvInfo);

	trace_event("End   VisaField48ToIso(OK)", PROCESSING);
	return (OK);

}
