#include <string.h>
#include <define.h>
#include <stdio.h>
#include <iso_hps.h>
#include <iso_sms.h>
#include <sms_puf.h>
#include <iso_cps.h>
#include <cps_fields.h>
#include <sms_param.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <queue.h>
#include <security_data.h>
#include <hsm_inc.h>
#include <p7_sid.h>
#include <sms_define.h>
#include <sms_puf_fields.h>
#include <iso8583.h>
#include <general_macros.h>
#include <p7_services.h>
#include <nw_raw_msg.h>


int             SmsEchoedField_0200[] =
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
	/* 015 */ 1,			/* STLMNT_DATE           */
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
	/* 034 */ 1,			/* PAN_EXTENDED                        */
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
	/* 063 */ 1,			/* PRIV_USE_FIELDS       */
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
	/* 134 */ 0,			/* Sms Discretionary Data       */
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


int FCT_AUT_REP_TO_NW(int nIndexCtx, TSIsoInfo * IsoInfo)
{
	TSSmsInfo      OrigSmsInfo;
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

	nLength = AnalyseSms(Node.sFstBuf, &OrigSmsInfo);

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
	if (AutReplyToNw(nIndexCtx, IsoInfo, &OrigSmsInfo) == OK)
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
/******        FCT_AUT_REP_FROM_NW ()                             *********/
/******    Receives an authorization response message (0210) from Sms *****/
/**************************************************************************/

int FCT_AUT_REP_FROM_NW (int nIndexCtx, TSSmsInfo *SmsInfo)
{
	TSNodeData      Node;
	TSIsoInfo       IsoInfo;
	TSBalExchange   BalMessage;
	char            FieldData[BUF_LG_MAX];
	int             FieldLen;
	char            SourceId[8 + 1];
	int             Length;
	char            sSecurityData [ 128 + 1 ];
	char            sLine [ MAX_LINE_TRC ];
	int				nBal;
	char			SourceNodeId[ NODE_ID_LEN + 1];

	trace_event ("Start  FCT_AUT_REP_FROM_NW()", PROCESSING);

	memset (FieldData,         0, sizeof (FieldData));
	memset(SourceNodeId, '\0', sizeof(SourceNodeId));
	
	/* Get audit number to search for original transaction */
	GetSmsField (SMS_AUDIT_NBR, SmsInfo, FieldData, &FieldLen);

	InitNodeData (&Node);
	memcpy (Node.szFstKey, "1200", 4);
	memcpy (Node.szFstKey + 4,  FieldData, 6);

    GetSmsField(SMS_CARD_NBR, SmsInfo, FieldData, &FieldLen);
	memcpy(Node.szFstKey + 4 + 6, FieldData, FieldLen);

	/*AMER 20160727 Start: PADSS Fix*/
	if(debug_option == 1)
	{
	sprintf(sLine, "==> FetchKey = [%.32s]", Node.szFstKey);
	trace_event(sLine, PROCESSING);
	}
	/*AMER 20160727 End: PADSS Fix*/

	switch (FetchRequest ('B', &Node))
	{
	case ENOEXIST:
		trace_event ("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event ("End FCT_AUT_REP_FROM_NW (NOK)", ERROR);
		return (NOK);

	case ELATE:
		break;
	}

	trace_event ("FetchRequest Succeeded ", PROCESSING );

	MsgFwdRepReceived(Node.msgId);

	/* reconstruct original ISO message */
	InitIsoInfo (&IsoInfo);
	AnalyseIso (Node.sFstBuf, &IsoInfo);
	IsoInfo.msgId = Node.msgId;

	/* add response elements to ISO message */
	BuildAutReplyFromNw ( nIndexCtx , SmsInfo, &IsoInfo);

	/* forward it to requester */

	memset (sSecurityData, 0, sizeof(sSecurityData));
	memset (SourceId,      0, sizeof(SourceId));
	GetIsoField (ISO_SECURITY_DATA, &IsoInfo, sSecurityData , &FieldLen);
	GetSecurityData(SOURCE_PID,sSecurityData, SourceId,&Length );
	GetSecurityData(SOURCE_NODE_ID, sSecurityData, SourceNodeId, &FieldLen);

        /*Start IBO20200517 RAWMSG*/
	/*if (g_sCaptureMsg[NW_INC_RESP] == 'Y')
	{
		LogVisaMessage(nIndexCtx, SmsInfo, &IsoInfo, INC_RESP);
	}*/
	/*End IBO20200517 RAWMSG*/
	dump_p7_struct ( &IsoInfo );

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
	BalMessage.sHeader.nSourceId  = getpid();
	BalMessage.sHeader.nCommand   = IsoInfo.nMsgType;
	BalMessage.sHeader.msgId      = Node.msgId;
	Length = IsoBuildMsg (BalMessage.sBody, &IsoInfo);
	BalMessage.sHeader.nLen = Length;

	if (WriteBalMsg (nBal, &BalMessage) != OK)
	{
		trace_event ("End FCT_AUT_REP_FROM_NW(): WriteBal() failed.", ERROR);
		IntWriteBalExeption(nIndexCtx,&IsoInfo); /* IBE20220612 PLUTONL-5420 */
		return (NOK);
	}

	MsgFwdRepReplied(BalMessage.sHeader.msgId);

	InitIsoInfo(&IsoInfo);
	memset(FieldData, 0, sizeof(FieldData));
	InitNodeData (&Node);
	memset((char *) &BalMessage, 0, sizeof(TSBalExchange));

	trace_event ("End  FCT_AUT_REP_FROM_NW(OK)", PROCESSING);

	return (OK);
}

/****************************************************************************/
/*   BuildAutReplyFromNw()                                                  */
/*                                                                          */
/*   Processes au Authorization Response message (0210) received from SMS   */
/****************************************************************************/
int  BuildAutReplyFromNw ( int         nIndexCtx,
	TSSmsInfo   *SmsInfo,
	TSIsoInfo   *IsoInfo)
{
	TSTlvPrivate   kTlvInfo;
	TSTlvPrivate   kNetworkDataTlv;	/*AEA20210712 Enh21.Q4 Art9.3.3*/
	TSCpsInfo      CpsInfo; 
	TSPufInfo      PufInfo;
	

	char           FieldData [ BUF_LG_MAX ];
	char           sRespCode [ 3 + 1 ];
	char           sSmsRespCode [ 2 + 1 ];
	char           sCpsTlvData [ BUF_LG_MAX ];
	char           tmpField [ BUF_LG_MAX ];
	char           sLine    [ MAX_LINE_TRC ];
	char          	sIsoPosMode[12 + 1]; 						
	int            FieldLen;
	int            tmpFieldLen;
	int            offset;
	int            retour;
	 /*HAL  20201903 Enh Q2.2020 ART 9.3.3*/
	TlvInfo			kSmsElectComTlv;   		
	TlvInfo			kSmsTlvInfo;
	TlvInfo    		isoExemption;       /*HAL  20201903 Enh Q2.2020 ART 9.3.3*/
    int             nExempResLen = 0;
	char 			sPSD2AuthExempRes[3 + 1];			
	int				nExempIndLen = 0;
    char            sTokenResponseSms[1 + 1];  /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	TlvInfo			kSmsTransSpecTlv; /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	TlvInfo			kSmsAppIdentTlv; /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/

	sprintf (sLine, "Start  BuildAutReplyFromNw(%d)", nIndexCtx);
	trace_event (sLine, PROCESSING);

	IsoInfo->nMsgType = 1210;


	memset (FieldData, 0, sizeof(FieldData));
	memset (tmpField,          0, sizeof (tmpField));
	memset(sCpsTlvData, 0, sizeof(sCpsTlvData));
	memset(sPSD2AuthExempRes, ' ', 4);/*HAL   Enh Q2.2020 ART 9.3.3*/
	memset (sIsoPosMode,   0, sizeof(sIsoPosMode)); 
	memset(sTokenResponseSms, 0, sizeof(sTokenResponseSms));    /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/	

	InitTlvPrivate ( &kTlvInfo );
	InitSmsTransSpecDataTlv(&kSmsTransSpecTlv); /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	InitSmsAppIdentDataTlv(&kSmsAppIdentTlv);  /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/

	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
		AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);	

	/* Field 5 */
	if (GetSmsField (SMS_STLMNT_AMOUNT, SmsInfo, FieldData, &FieldLen) == SUCCES)
		PutIsoField (ISO_STLMNT_AMOUNT, IsoInfo, FieldData, FieldLen);

	/* Field 15 */
	current_date_yymmdd(FieldData);
	if (GetSmsField (SMS_STLMNT_DATE, SmsInfo, FieldData + 2, &FieldLen) == SUCCES)
		PutIsoField (ISO_STLMNT_DATE, IsoInfo, FieldData, 6);


	/* Field 20 */
	if (GetSmsField (SMS_PAN_COUNTRY, SmsInfo, FieldData, &FieldLen) == SUCCES)
		InsertIsoField (ISO_PAN_COUNTRY, IsoInfo, FieldData, 3);

	/* Field 38 */
	if (GetSmsField (SMS_AUTHOR_ID, SmsInfo, FieldData, &FieldLen) == SUCCES)
		InsertIsoField  (ISO_AUTHOR_ID, IsoInfo, FieldData, FieldLen);

	/* Field 39 */
	if (GetSmsField (SMS_RESPONSE_CODE, SmsInfo, sSmsRespCode, &FieldLen) == SUCCES) 
	{
		retour = TranslateRespCode (nIndexCtx, ResourceId, sSmsRespCode, "RP", sRespCode );
		if (retour != OK)
			trace_event (" Can not Translate the SMS response Code!!! ", PROCESSING);  

		InsertIsoField (ISO_RESPONSE_CODE, IsoInfo, sRespCode, 3);
	}

	/* Fields 44, 48, both go into ISO field 48 in TLV format */
	/*InitTlvPrivate ( &kTlvInfo );*/
	memset (FieldData, 0, sizeof(FieldData));
	if (GetSmsField (SMS_ADTNL_RESP_DATA, SmsInfo, FieldData, &FieldLen) == SUCCESS)
	{
		/* Subfield 44.1 */
		/*AddTlvPrivate (EXTERNAL_RESP_SOURCE, &kTlvInfo, FieldData, 1);

		memset (tmpField, 0, sizeof(tmpField));
		tmpFieldLen = PrivateBuildTlv (tmpField, &kTlvInfo);
		InsertIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, tmpFieldLen);*/
		
		/*Start AMER20210927 PLUTONL-3612*/
		GetIsoField (ISO_POS_DATA, IsoInfo,  sIsoPosMode, &tmpFieldLen);
		int T2DataPresenceFlag= GetIsoField (ISO_T2_DATA, IsoInfo, tmpField, &FieldLen);
		int IccDataPresenceFlag= GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &FieldLen);
		SmsToIsoAdtnlRespData(FieldData,T2DataPresenceFlag,IccDataPresenceFlag,sIsoPosMode,&kTlvInfo);
		/*End AMER20210927 PLUTONL-3612*/

	}
	
	/*Start HAL 20201903 ENH 2020Q2 ART 9.3.3*/
	if (GetSmsField(SMS_PAN_EXTENDED, SmsInfo, FieldData, &FieldLen) == SUCCESS)
	{	
		InitSmsElectComDataTlv(&kSmsElectComTlv); 	
		AnalyseTlvInfo(FieldData, FieldLen, &kSmsElectComTlv);

		/*InitExempAuthInfoDataTlv(&isoExemption);*/	
    
	    if(GetTlvInfoField (SMS_F34_EU_PSD2_SCA_CSC, &kSmsElectComTlv, FieldData, &FieldLen) == SUCCESS )
		{
			InitSmsEuPsd2ScaCsc(&kSmsTlvInfo);
			AnalyseTlvInfo(FieldData, FieldLen, &kSmsTlvInfo);
			/*InitTlvPrivate(&kTlvInfo);*/
			InitExempAuthInfoDataTlv(&isoExemption); 
			
			/*if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
				AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);*/

			if(GetTlvInfoField (SMS_F34T4A_TRST_MER_EXEM, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{	
				sPSD2AuthExempRes[P7_F048_EXMP_RES_TRST_MER] = FieldData[0];
			    nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_TRST_MER +1);  		
			}

			if(GetTlvInfoField (SMS_F34T4A_REASON_HONOR_EXEM, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
				PutTlvInfoField (P7_F048_S27_REASON_HONOR_EXEM , &isoExemption, FieldData, FieldLen);
			
			 if(GetTlvInfoField (SMS_F34T4A_TRA_SCORE, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
				PutTlvInfoField (P7_F048_S27_TRA_SCORE , &isoExemption, FieldData, FieldLen);

			if(GetTlvInfoField (SMS_F34T4A_TRA, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{	
				sPSD2AuthExempRes[P7_F048_EXMP_RES_TRA] = FieldData[0];
				nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_TRA +1);
			}

			/*Start AEA20210712 Enh21.Q4 Art9.3.3*/
			if(GetTlvInfoField (SMS_F34T4A_DELG_AUTH, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{
				sPSD2AuthExempRes[P7_F048_EXMP_RES_DELG_AUTH] = FieldData[0];
				nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_DELG_AUTH +1);
			}
			/*End AEA20210712 Enh21.Q4 Art9.3.3*/

			if(nExempResLen > 0)
				PutTlvInfoField (P7_F048_S27_AUTH_EXEMPTION_RES , &isoExemption, sPSD2AuthExempRes, nExempResLen);
		}
		
		tmpFieldLen = BuildTlvInfoBuffer (FieldData, &isoExemption);
        if( tmpFieldLen > 0 )
			PutTlvPrivate(PSD2_DATA, &kTlvInfo, FieldData, tmpFieldLen);

		/*tmpFieldLen = PrivateBuildTlv (FieldData, &kTlvInfo);
		PutIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, tmpFieldLen);*/	
	}
	/*End HAL 20201903 ENH 2020Q2 ART 9.3.3*/

	tmpFieldLen = PrivateBuildTlv (FieldData, &kTlvInfo);
	
	if(tmpFieldLen > 0)	/*AMER20210927 PLUTONL-3612*/
		PutIsoField (ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, tmpFieldLen);
		
	/* Field 50 */
	if (GetSmsField (SMS_STLMNT_CRNCY, SmsInfo, FieldData, &FieldLen) == SUCCES)
		PutIsoField (ISO_STLMNT_CRNCY, IsoInfo, FieldData, FieldLen);

	/* Field 54: for Balance Inquiry Messages */
	memset (tmpField, 0, sizeof(tmpField));
	memset(tmpField, 0, sizeof(tmpField));
	if( GetSmsField(SMS_ADTNL_AMOUNTS, SmsInfo, FieldData, &FieldLen) == SUCCESS )
	{
		SmsToIsoAddAmount(FieldData, tmpField);
		PutIsoField(ISO_ADTNL_AMOUNTS, IsoInfo, tmpField, strlen(tmpField));
	}

	GetSmsField (SMS_PROC_CODE, SmsInfo, FieldData, &FieldLen);




	/* Start ADDED BY KR Pour Gestion de Code Reponse de Issuer */
	if ( GetIsoField (ISO_NET_POS_DATA, IsoInfo, tmpField, &FieldLen ) == SUCCES )
		AnalyseTlvPrivate ( tmpField , FieldLen , &kTlvInfo);
	else
		InitTlvPrivate (&kTlvInfo);

	PutTlvPrivate (RESPONSE_CODE_TAG, &kTlvInfo, sSmsRespCode, 2);
	FieldLen = PrivateBuildTlv (tmpField, &kTlvInfo);
	if ( FieldLen > 0 )
		PutIsoField (ISO_NET_POS_DATA, IsoInfo, tmpField, FieldLen);

	/** Field 55: ICC Data **/
	if (GetSmsField (SMS_ICC_DATA , SmsInfo, FieldData, &FieldLen) == SUCCESS)
	{
		char          sIccBuffer[BUF_LG_MAX];
		TSTagIso      SmsIccInfo;
		TSTagIso      IsoIccInfo;

		memset(sIccBuffer, 0, sizeof(sIccBuffer));
		InitIsoIcTag  ( &SmsIccInfo );
		AnalyseIsoIc  ( FieldData+3, FieldLen-3, &SmsIccInfo );
		InitIsoIcTag ( &IsoIccInfo );

		if(GetIsoIcTag  (ISO_TAG_ISS_AUTHEN_DATA , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA, &IsoIccInfo, FieldData,  FieldLen );
		else
			AddIsoIcTag (ISO_TAG_AUTHOR_RESP_CODE, &IsoIccInfo, "00", 2);

		if(GetIsoIcTag  (ISO_TAG_ISS_SCRIPT1 , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag (ISO_TAG_ISS_SCRIPT1 , &IsoIccInfo, FieldData,  FieldLen );

		if(GetIsoIcTag  (ISO_TAG_ISS_SCRIPT2 , &SmsIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag (ISO_TAG_ISS_SCRIPT2 , &IsoIccInfo, FieldData,  FieldLen );

		FieldLen = IsoBuildIcFld ( sIccBuffer, &IsoIccInfo);
		if ( FieldLen > 0 )
			PutIsoField (ISO_ICC_DATA, IsoInfo, sIccBuffer, FieldLen);

		memset(sIccBuffer, 0, sizeof(sIccBuffer));
		InitIsoIcTag(&SmsIccInfo);
		InitIsoIcTag(&IsoIccInfo);
	}

	/* 62 Fields Cps Data */
	InitCpsInfo (&CpsInfo);

	if (GetSmsField (SMS_CPS_FIELDS , SmsInfo, FieldData, &FieldLen) == SUCCESS)
	{
		AnalyseCps(FieldData,&CpsInfo);
		InitTlvPrivate (&kTlvInfo);
		if (GetCpsSubField (CPS_ACI, &CpsInfo, FieldData, &FieldLen) == SUCCESS)
		{
			PutTlvPrivate(CPS_ACI_TAG ,&kTlvInfo,FieldData,1);
		}
		if (GetCpsSubField (CPS_TRANS_ID, &CpsInfo, FieldData, &FieldLen) == SUCCESS)
		{
			PutTlvPrivate(CPS_TRANS_ID_TAG ,&kTlvInfo, FieldData, FieldLen);
		}
		/* START HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.1*/
		if (GetCpsSubField(CPS_PRODUCT_ID, &CpsInfo, FieldData, &FieldLen) == SUCCESS)
		{
			PutTlvPrivate(CPS_PRODUCT_ID_TAG, &kTlvInfo, FieldData, FieldLen);
		}	
		/* END HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.1*/
		/* START HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.2*/
		if (GetCpsSubField(CPS_SPEND_QUALIFIED_IND, &CpsInfo, FieldData, &FieldLen) == SUCCESS)
		{
			PutTlvPrivate(CPS_SPEND_QUALIFIED_INDICATOR, &kTlvInfo, FieldData, FieldLen);
		}					
		/* END HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.2*/
		FieldLen = PrivateBuildTlv(sCpsTlvData, &kTlvInfo);

		InsertIsoField (ISO_CPS_FIELDS, IsoInfo, sCpsTlvData, FieldLen);
	} 


	/* Field 63: Private Use Fields */
	memset (FieldData, 0, sizeof(FieldData));
	/*Start AEA20210712 Enh21.Q4 Art9.3.3*/
	if (GetIsoField(ISO_NETWORK_DATA, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AnalyseTlvPrivate(tmpField, FieldLen, &kNetworkDataTlv);
	else
		InitTlvPrivate(&kNetworkDataTlv);
	/*End AEA20210712 Enh21.Q4 Art9.3.3*/
	if (GetSmsField (SMS_PRIV_USE_FIELDS, SmsInfo, FieldData, &FieldLen) == SUCCESS)
	{
		/* extraction du sous-champ 1 du champ 63 */
		InitPufInfo    (&PufInfo);
		AnalysePuf (FieldData, &PufInfo);
		GetPufSubField (PUF_NETWORK_ID, &PufInfo, FieldData, &FieldLen);

		AddTlvPrivate(NET_NETWORK_ID_TAG, &kNetworkDataTlv, FieldData, FieldLen);	/*AEA20210712 Enh21.Q4 Art9.3.3*/
	}
	
	/*Start AEA20210712 Enh21.Q4 Art9.3.3*/
	if (GetSmsField(SMS_PRIV_USE_FIELD, SmsInfo, FieldData, &FieldLen) == SUCCES)
	{
		InitPufInfo (&PufInfo);
		AnalysePuf (FieldData, &PufInfo);

		if (GetPufSubField(SPUF_VPUF_MER_ID, &PufInfo, FieldData, &FieldLen) == SUCCES)
		{
			AddTlvPrivate (NET_VISA_MERC_ID, &kNetworkDataTlv, FieldData,FieldLen);
		}
	}
	/*Start IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	if (GetSmsField(SMS_ADTNL_RESP_DATA, SmsInfo, FieldData, &FieldLen) == SUCCES)
	{
		sTokenResponseSms[0]=FieldData[2];
		AddTlvPrivate (NET_TOKEN_PROGAM, &kNetworkDataTlv, sTokenResponseSms ,1);
	}
	/*End IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/

	*Start PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	if (GetSmsField(SMS_TRANS_DESC, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsTransSpecTlv);
		
		if( GetTlvInfoField (SMS_F104_BUSINESS_APP_ID, &kSmsTransSpecTlv, tmpField, &tmpFieldLen) == SUCCESS )
		{ 
			AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsAppIdentTlv);
			memset(tmpField, '\0', sizeof(tmpField));
			if( GetTlvInfoField (SMS_F104_T57_MAX_PROC_DATE, &kSmsAppIdentTlv, tmpField, &tmpFieldLen) == SUCCESS )
			{
				memset(FieldData, '\0', sizeof(FieldData));
				julian_to_gmt(tmpField,FieldData);
				AddTlvPrivate(NET_MAX_PROC_DATE_TAG, &kNetworkDataTlv, FieldData,strlen(FieldData));
			}
		}
	}
	/*End PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/

	memset (tmpField, 0, sizeof(tmpField));
	tmpFieldLen = PrivateBuildTlv (tmpField, &kNetworkDataTlv);
	if (tmpFieldLen > 0)
		PutIsoField (ISO_NETWORK_DATA, IsoInfo, tmpField, tmpFieldLen);
	/*End AEA20210712 Enh21.Q4 Art9.3.3*/
	
	memset(FieldData, 0, sizeof(FieldData));
	InitTlvPrivate(&kTlvInfo);

	trace_event ("End  BuildAutReplyFromNw(OK)", PROCESSING);
	return (OK);
}
/****************************************************************************/
/* AutReplyToNw()                                                         */
/* */
/* Constructs and sends an authorization response message (0210) to SMS. */
/****************************************************************************/
int AutReplyToNw(	int nIndexCtx,
					TSIsoInfo * IsoInfo,
					TSSmsInfo * OrigSmsInfo)
{
	TSSmsInfo      NewSmsInfo;
	TSTlvPrivate    TlvInfo;
	char            sProcCode[256];
	char            FieldData[BUF_LG_MAX];	/*AEA20210824 PLUTONL-3379*/
	char            sPufData[256];
	char            sBuffer[256];
	char            tmpField[256];
	int             FieldLen;
	int             Length;
	char            CvvResult[1 + 1];
	char			ARQCResult [ 32 + 1 ];  /*AKH  200109*/
	char 			cavv_result[  1 + 1 ];
	char 			caav_field [256];
	char           cavv_valid_result = '2';
	char           cavv_invalid_result = '1';
	int             i;
	TSSmsPufInfo   SmspufInfo;
	char            sLine[MAX_LINE_TRC];
	char			PosCond[3];
	char           sSmsRespCode[256];
	char			sTransAmount[24];
	char            AvrResult[1 + 1];
	char			sIsoRespCode[6];
	char			sIsoProcCode[6 + 1];


	trace_event("Start  AutReplyToNw()", PROCESSING);

	memset(sIsoRespCode,'\0',sizeof(sIsoRespCode));
	memset(caav_field               , 0 , sizeof(caav_field         ));
	memset(sBuffer, 0, sizeof(sBuffer));
	memset ( PosCond,  0 , sizeof ( PosCond ));
	memset ( sIsoProcCode,  0 , sizeof ( sIsoProcCode ));

	dump_p7_struct(IsoInfo);
	InitSmsInfo(&NewSmsInfo);
	NewSmsInfo.nMsgType = 210;

	GetIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, &FieldLen);
	GetSmsField (SMS_TRANS_AMOUNT,OrigSmsInfo, sTransAmount, &FieldLen);

	for (i = 0; i < SMS_AUDIT_NBR; i++) 
	{
		if (SmsEchoedField_0200[i])
		{
			if (GetSmsField(i, OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
				AddSmsField(i, &NewSmsInfo, FieldData, FieldLen);
		}
		else if (	(i == SMS_BILLING_AMOUNT || i== SMS_BILLING_CNVRSN_RATE ) && 
					memcmp( sIsoProcCode,P7_F003_PURCHASE,P7_F003_P1_LEN) == 0 ) 
		{
			if (GetSmsField(i, OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
				AddSmsField(i, &NewSmsInfo, FieldData, FieldLen);

		}
	}

	/* restore SMS field 11 from ISO field 48 */
	InitTlvPrivate(&TlvInfo);
	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, &FieldLen) == SUCCESS) 
	{
		AnalyseTlvPrivate(FieldData, FieldLen, &TlvInfo);

		if (GetTlvPrivate(EXTERNAL_STAN_TAG, &TlvInfo, FieldData, &FieldLen) == SUCCESS)
			AddSmsField(SMS_AUDIT_NBR, &NewSmsInfo, FieldData, FieldLen);
	}

	for (i = SMS_AUDIT_NBR + 1; i < SMS_AUTHOR_ID; i++) 
	{
		if (SmsEchoedField_0200[i])
		{
			if (GetSmsField(i, OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
				AddSmsField(i, &NewSmsInfo, FieldData, FieldLen);
		}
		else if (	(i == SMS_CNVRSN_DATE )		&& 
					memcmp( sIsoProcCode,P7_F003_PURCHASE,P7_F003_P1_LEN) == 0) 
		{

			if (GetSmsField(i, OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
				AddSmsField(i, &NewSmsInfo, FieldData, FieldLen);
		}
	}

	/* Field 38 */

	if (GetSmsField (SMS_RESPONSE_CODE,OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
	{
		if ( memcmp(FieldData,SMS_F039_SUSPECTED_FRAUD,SMS_F039_LEN)== 0 )
			trace_event("SMS RESPONSE CODE : VRM",PROCESSING);
		else 
		{
			if (GetIsoField(ISO_AUTHOR_ID, IsoInfo, FieldData, &FieldLen) == SUCCES)
				AddSmsField(SMS_AUTHOR_ID, &NewSmsInfo, FieldData, FieldLen);
		}
	}
	else 
	{
		if (GetIsoField(ISO_AUTHOR_ID, IsoInfo, FieldData, &FieldLen) == SUCCES)
			AddSmsField(SMS_AUTHOR_ID, &NewSmsInfo, FieldData, FieldLen);
	}

	/* Field 39 */
	if ( GetIsoField  (ISO_RESPONSE_CODE, IsoInfo, sIsoRespCode, &FieldLen) == SUCCES )
	{
		GetSmsField (SMS_POS_CONDITION, OrigSmsInfo, PosCond, &FieldLen);
		if ( ( memcmp(sIsoRespCode, P7_F039_APPROVAL, P7_F039_LEN) == 0 ) && 
			( memcmp(PosCond,SMS_F025_ACC_VERIF,SMS_F025_LEN) == 0) )
		{
			memcpy(sSmsRespCode,SMS_F039_NO_RSN_TO_DECLINE,SMS_F039_LEN);    
		}
		else 
		{ 
			
			if ( TranslateRespCode (nIndexCtx, ResourceId, sIsoRespCode, "PR", sSmsRespCode) != SUCCESS )
			{
				if ( !memcmp(sIsoRespCode, P7_F039_APPROVAL, P7_F039_LEN) )
				{
					trace_event(">>> WARNNING. SEND 00 FOR AN APPROVED TRX IF RESP CODE TRANSLAT. IS NOT FOUND <<<", FATAL);
					strncpy(sSmsRespCode,SMS_F039_APPROVAL, SMS_F039_LEN);
				}
				else
				{
					trace_event(">>> WARNNING. SEND 05 FOR A DECLINE TRX IF RESP CODE TRANSLAT. IS NOT FOUND <<<", FATAL);
					strncpy(sSmsRespCode,SMS_F039_DECLINE, SMS_F039_LEN);
				}
			}
		}
		AddSmsField (SMS_RESPONSE_CODE, &NewSmsInfo, sSmsRespCode, SMS_F039_LEN);
	}
	
	for (i = SMS_RESPONSE_CODE + 1; i < SMS_ADTNL_RESP_DATA; i++) 
	{
		if (SmsEchoedField_0200[i])
			if (GetSmsField(i, OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
				AddSmsField(i, &NewSmsInfo, FieldData, FieldLen);
	}

	/* SMS field 44 is in ISO field 48 in TLV format */
	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, &FieldLen) == SUCCESS) 
	{
		InitTlvPrivate(&TlvInfo);
		AnalyseTlvPrivate(FieldData, FieldLen, &TlvInfo);
		/* Field 44 */
		/* Subfield 44.5: CVV Results Code */
		/* Subfields 44.1 to 44.4 are blank padded */
		memset(AvrResult, 0, sizeof(AvrResult));
		memset(CvvResult, 0, sizeof(CvvResult));
		memset(FieldData, 0, sizeof(FieldData));
		memset(FieldData, ' ', 25);
		FieldData[0] = '5';

		if (GetTlvPrivate(ADDRESS_VERIF_REP_TYPE_TAG, &TlvInfo, AvrResult, &FieldLen) == SUCCESS)
		{
			FieldData[1] = AvrResult[0];	
		}
		
		if (GetTlvPrivate(CVV_RESULT_CODE_TAG, &TlvInfo, CvvResult, &FieldLen) == SUCCESS) 
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
		if (GetTlvPrivate(CVV2_RESULT_CODE_TAG, &TlvInfo, CvvResult, &FieldLen) == SUCCESS) 
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
		if (GetTlvPrivate (CAVV_TAG       ,&TlvInfo , caav_field ,&FieldLen) == SUCCES )
		{
			char           AddPos[11];
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
			} 
		}
		/* Sms CVC2 SPECS */
		/***************************************/
		if (GetSmsField(SMS_PRIV_USE_FIELD, OrigSmsInfo, tmpField, &FieldLen) == SUCCES) 
		{
			InitSmsPufInfo(&SmspufInfo);
			AnalyseSmsPuf(tmpField, &SmspufInfo);
			memset(sPufData, 0, sizeof(sPufData));
			if (GetSmsPufSubField(SPUF_CVV2, &SmspufInfo, sPufData, &Length) == SUCCES) 
			{
				if (sPufData[0] == SMS_F126_P10_CVV2_ILGBL)
					FieldData[10] = SMS_F044_P10_CVV2_NOPROC;
				if (sPufData[0] == SMS_F126_P10_NO_CVV2)
					FieldData[10] = SMS_F044_P10_CVV2_IS_ON_CRD;
				if (sPufData[0] == SMS_F126_P10_CVV2_P 
					&& CvvResult[0] != '0' && CvvResult[0] != '1')
					FieldData[10] = SMS_F044_P10_CVV2_NOPROC;
			}
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
					FieldData [8] = '2'; 
				else if (GetTlvPrivate (CRYPT_RESULT_CODE_TAG, &TlvInfo, tmpField , &FieldLen) == SUCCESS)
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

		AddSmsField(SMS_ADTNL_RESP_DATA, &NewSmsInfo, FieldData, strlen(FieldData));
	}

	for (i = SMS_ADTNL_RESP_DATA + 1; i < SMS_ADTNL_DATA_PRIV; i++) 
	{
		if (SmsEchoedField_0200[i])
			if (GetSmsField(i, OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
				AddSmsField(i, &NewSmsInfo, FieldData, FieldLen);
	}

	if (GetSmsField(SMS_ADTNL_DATA_PRIV, OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
		AddSmsField(SMS_ADTNL_DATA_PRIV, &NewSmsInfo, FieldData, FieldLen);


	for (i = SMS_ADTNL_DATA_PRIV + 1; i < SMS_ADTNL_AMOUNTS; i++) 
	{
		if (SmsEchoedField_0200[i])
		{
			if (GetSmsField(i, OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
				AddSmsField(i, &NewSmsInfo, FieldData, FieldLen);
		}
		else if (	(i==SMS_BILLING_CRNCY )		&&
					memcmp( sIsoProcCode,P7_F003_PURCHASE,P7_F003_P1_LEN) == 0 )
		{
			if (GetSmsField(i, OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
				AddSmsField(i, &NewSmsInfo, FieldData, FieldLen);
		}
	}

	/* Field 54: for Balance Inquiry Messages */
	memset(sProcCode, 0, sizeof(sProcCode));
	memset(FieldData, 0, sizeof(FieldData));
	memset(tmpField, 0, sizeof(tmpField));

	if( GetIsoField(ISO_ADTNL_AMOUNTS, IsoInfo, FieldData, &FieldLen) == SUCCESS) 
	{
		IsoToSmsAddAmount(FieldData, tmpField);
		AddSmsField(SMS_ADTNL_AMOUNTS, &NewSmsInfo, tmpField, strlen(tmpField));
	}

	GetSmsField(SMS_PROC_CODE, OrigSmsInfo, sProcCode, &FieldLen);



	/** Field 55 **/
	trace_event("==> ICC 000 <==", FATAL);
	if (GetIsoField(ISO_ICC_DATA, IsoInfo, FieldData, &FieldLen) == SUCCESS) 
	{
		TSTagIso        IsoIccInfo;
		TSTagIso        SmsIccInfo;
		char            sIccBuffer[1024];
		int             tmpFieldLen;

		trace_event("==> ICC 001 <==", FATAL);
		InitIsoIcTag(&SmsIccInfo);
		InitIsoIcTag(&IsoIccInfo);
		AnalyseIsoIc(FieldData, FieldLen, &IsoIccInfo);
		if(GetIsoIcTag  (ISO_TAG_ATC , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			AddIsoIcTag (ISO_TAG_ATC , &SmsIccInfo, FieldData,  FieldLen );
		if ( memcmp(sSmsRespCode, SMS_F039_APPROVAL, SMS_F039_LEN) == 0 )
		{
			if(GetIsoIcTag  (ISO_TAG_ISS_AUTHEN_DATA , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA , &SmsIccInfo, FieldData,  FieldLen );

			if(GetIsoIcTag  (ISO_TAG_ISS_SCRIPT1 , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddIsoIcTag (ISO_TAG_ISS_SCRIPT1 , &SmsIccInfo, FieldData,  FieldLen );

			if(GetIsoIcTag  (ISO_TAG_ISS_SCRIPT2 , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddIsoIcTag (ISO_TAG_ISS_SCRIPT2 , &SmsIccInfo, FieldData,  FieldLen );
		}
		else
		{
			if(GetIsoIcTag  (ISO_TAG_ISS_AUTHEN_DATA , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
			{
				sprintf(FieldData+16,"%02X%02X",sSmsRespCode[0],sSmsRespCode[1]);
				AddIsoIcTag (ISO_TAG_ISS_AUTHEN_DATA , &SmsIccInfo, FieldData,  FieldLen );
			}
			if(GetIsoIcTag  (ISO_TAG_ISS_SCRIPT1 , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddIsoIcTag (ISO_TAG_ISS_SCRIPT1 , &SmsIccInfo, FieldData,  FieldLen );

			if(GetIsoIcTag  (ISO_TAG_ISS_SCRIPT2 , &IsoIccInfo, FieldData,  &FieldLen ) == SUCCES )
				AddIsoIcTag (ISO_TAG_ISS_SCRIPT2 , &SmsIccInfo, FieldData,  FieldLen );
		}

		FieldLen = IsoBuildIcFld ( sIccBuffer, &SmsIccInfo);
		dump_buffer(sIccBuffer, FieldLen,'X','X','X');
		if ( FieldLen > 0 )
		{
			memset(FieldData, 0, sizeof(FieldData));
			FieldData[0] = 0x01;
			FieldData[1] = 0x00;
			FieldData[2] = FieldLen;
			memcpy(FieldData+3, sIccBuffer, FieldLen);
			AddSmsField (SMS_ICC_DATA, &NewSmsInfo, FieldData, FieldLen+3);
		}


	}
	trace_event("==> ICC 002 <==", FATAL);

	for (i = SMS_ICC_DATA + 1; i <= MAX_SMS_FIELDS; i++) 
	{
		if (SmsEchoedField_0200[i])
			if (GetSmsField(i, OrigSmsInfo, FieldData, &FieldLen) == SUCCESS)
				AddSmsField(i, &NewSmsInfo, FieldData, FieldLen);
	}

	/* send it to SMS */
	Length = SmsBuildMsg(sBuffer + SMS_HDR_LEN, &NewSmsInfo);

	SmsBuildHeader(RESPONSE, &(NewSmsInfo.sHeader), &(OrigSmsInfo->sHeader), Length, StationId);
	memcpy(sBuffer, (char *) &(NewSmsInfo.sHeader), SMS_HDR_LEN);
	Length += SMS_HDR_LEN;

	dump_buffer(sBuffer, Length, 'E', 'L', 'O');

	DumpSms(sBuffer);

	if (WriteLineMsg(sBuffer, Length)) 
	{
		trace_event("End  AutReplyToNw(NOK): WriteLineMsg() failed", ERROR);
		return (NOK);
	}

	MsgRepSent(IsoInfo->msgId,sIsoRespCode);

	memset(caav_field, 0, sizeof(caav_field));
	memset(sBuffer, 0, sizeof(sBuffer));
	memset(FieldData, 0, sizeof(FieldData));
	memset(sPufData, 0, sizeof(sPufData));
	memset(tmpField, 0, sizeof(tmpField));

	InitSmsInfo(&NewSmsInfo);
	InitTlvPrivate(&TlvInfo);
	InitSmsPufInfo(&SmspufInfo);

	trace_event("End  AutReplyToNw(OK)", PROCESSING);
	return (OK);
}

int SmsField48ToIso(TSSmsInfo * SmsInfo,TSIsoInfo      *IsoInfo)
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

	TlvInfo			kSmsElectComTlv;
	TlvInfo			kSmsTlvInfo;
	char			sEcomAuthInfo[P7_F048_P66_LEN + 1];
	int				nEcomAuthInfo=0;
	TlvInfo    		isoExemption;       /*HAL  20201903 Enh Q2.2020 ART 9.3.3*/
	char 			sPSD2AuthExempRes[3 + 1];
	int				nExempResLen = 0;

	TlvInfo    		kTokenDataIso;       				/*HAL2402 PLUTONL-4391*/
	TlvInfo			smsPanMappingData;  				/*HAL2402 PLUTONL-4391*/
	TlvInfo			kSmsTokenTlv;   	    			/*HAL2402 PLUTONL-4391*/
	TlvInfo			smsAddVerifMappingData;  			/*HAL2402 PLUTONL-4391*/
	TlvInfo			isoEcomAuthentifData; /*PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 


	trace_event("Start SmsField48ToIso()", PROCESSING);
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
	InitSmsTokenDataTlvInfo(&smsPanMappingData);		/*HAL2402 PLUTONL-4391*/
	InitSmsVerifDataTlvInfo(&kSmsTokenTlv);				/*HAL2402 PLUTONL-4391*/

	InitSmsAddrVerifDataTlvInfo(&smsAddVerifMappingData);	/*HAL2402 PLUTONL-4391*/
	InitEcomAuthentificationInfoDataTlv(&isoEcomAuthentifData); /*PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 

	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
		AnalyseTlvPrivate(tmpField, tmpFieldLen, &kTlvInfo);

	/*Start HAL PROD00074189 20202801 Enh Q2.2020 Art 3.10*/
    if (GetSmsField(SMS_PAN_EXTENDED, SmsInfo, FieldData, &FieldLen) == SUCCESS)
	{
		InitSmsElectComDataTlv(&kSmsElectComTlv);
		AnalyseTlvInfo(FieldData, FieldLen, &kSmsElectComTlv);

		if(GetTlvInfoField (SMS_F34_AUTHEN_DATA, &kSmsElectComTlv, FieldData, &FieldLen) == SUCCESS )
		{
			InitSmsEcomAddDataTlv(&kSmsTlvInfo);
			AnalyseTlvInfo(FieldData, FieldLen, &kSmsTlvInfo);

				if(GetTlvInfoField (SMS_F34T01_PROTOCOL_VERSION_NUMBER, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
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

			}
			if(GetTlvInfoField (SMS_F34T01_AUTHENTICATION_PROGRAM, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{
				if(memcmp(FieldData,SMS_DAF,2)==0)
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
			/*Start PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
			if( GetTlvInfoField (SMS_F34T01_AUTHEN_DATA_QUALITY_INDC, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{
               	AddTlvInfoField (P7_F048S26_AUTHEN_DATA_QUALITY_INDC , &isoEcomAuthentifData, FieldData, FieldLen);
			}
			/*End PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
		}

        /*Start HAL 20201903 ENH 2020Q2 ART 9.3.3*/
        if(GetTlvInfoField (SMS_F34_EU_PSD2_SCA_CSC, &kSmsElectComTlv, FieldData, &FieldLen) == SUCCESS )
		{
			InitSmsEuPsd2ScaCsc(&kSmsTlvInfo);
			AnalyseTlvInfo(FieldData, FieldLen, &kSmsTlvInfo);

			InitExempAuthInfoDataTlv(&isoExemption);

			if(GetTlvInfoField (SMS_F34T4A_TRST_MER_EXEM, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{
				sPSD2AuthExempRes[P7_F048_EXMP_RES_TRST_MER] = FieldData[0];
			    nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_TRST_MER +1);
			}

			if(GetTlvInfoField (SMS_F34T4A_REASON_HONOR_EXEM, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
				PutTlvInfoField (P7_F048_S27_REASON_HONOR_EXEM , &isoExemption, FieldData, FieldLen);

			if(GetTlvInfoField (SMS_F34T4A_TRA_SCORE, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
				PutTlvInfoField (P7_F048_S27_TRA_SCORE , &isoExemption, FieldData, FieldLen);


			if(GetTlvInfoField (SMS_F34T4A_TRA, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
			{
				sPSD2AuthExempRes[P7_F048_EXMP_RES_TRA] = FieldData[0];
				nExempResLen = MAX(nExempResLen,P7_F048_EXMP_RES_TRA +1);
			}

			/*Start AEA20210712 Enh21.Q4 Art9.3.3*/
			if(GetTlvInfoField (SMS_F34T4A_DELG_AUTH, &kSmsTlvInfo, FieldData, &FieldLen) == SUCCESS )
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

	if (GetSmsField(SMS_ADTNL_RESP_DATA, SmsInfo, FieldData, &FieldLen) == SUCCESS)
	{
		/*if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, IsoFieldData, &IsoFieldLen) == SUCCESS)
			AnalyseTlvPrivate(IsoFieldData, IsoFieldLen, &kTlvInfo);*/

		/*Start ACH20191210  PLUTONL-500 */
		GetIsoField (ISO_POS_DATA, IsoInfo,  sIsoPosMode, &tmpFieldLen);
		/*int IccDataPresenceFlag= GetSmsField(SMS_ICC_DATA, SmsInfo, tmpField, &tmpFieldLen);
		int T2DataPresenceFlag= GetSmsField(SMS_T2_DATA, SmsInfo, tmpField, &tmpFieldLen);*/
		int T2DataPresenceFlag= GetIsoField (ISO_T2_DATA, IsoInfo, tmpField, &tmpFieldLen);
		int IccDataPresenceFlag= GetIsoField (ISO_ICC_DATA, IsoInfo, tmpField, &tmpFieldLen);
		SmsToIsoAdtnlRespData(FieldData,T2DataPresenceFlag,IccDataPresenceFlag,sIsoPosMode, 1, &kTlvInfo);
		/*End ACH20191210  PLUTONL-500 */
	}

	/*Start HAL2402 PLUTONL-4391*/
	if (GetSmsField(SMS_ADR_VERIF_DATA, SmsInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvInfo(tmpField, tmpFieldLen, &kSmsTokenTlv);

		if (GetTlvInfoField(SMS_F123_TOKEN_DATA, &kSmsTokenTlv, tmpField, &tmpFieldLen) == SUCCESS)
		{
			AnalyseTlvInfo(tmpField, tmpFieldLen, &smsPanMappingData);

			if (GetTlvInfoField(SMS_F123_T68_TOKEN, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
			{
				AddTlvInfoField(P7_F048P55_TOKEN, &kTokenDataIso, tmpField, tmpFieldLen);
			}
			if (GetTlvInfoField(SMS_F123_T68_TKN_ASS_LVL, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_ASSURANCE_LVL, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_REQ_ID, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_REQ_ID, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_RANGE, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_PAN, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_REFERENCE_ID, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_PAN_REF_ID, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_EXPIRATION_DATE, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_EXP_DT, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_TYPE, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_TYPE, &kTokenDataIso, tmpField, tmpFieldLen);

			if (GetTlvInfoField(SMS_F123_T68_TKN_STATUS, &smsPanMappingData, tmpField, &tmpFieldLen) == SUCCESS)
				AddTlvInfoField(P7_F048P55_TKN_STATUS, &kTokenDataIso, tmpField, tmpFieldLen);
		}
	}

		/*Start PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
	tmpFieldLen = BuildTlvInfoBuffer (tmpField, &isoEcomAuthentifData);
	if( tmpFieldLen > 0 )
	 	PutTlvPrivate(ECOM_AUTHENTIF_DATA, &kTlvInfo, tmpField, tmpFieldLen);
	/*End PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/ 
	
	tmpFieldLen = BuildTlvInfoBuffer(tmpField, &kTokenDataIso);
	if (tmpFieldLen > 0)
		PutTlvPrivate(TOKEN_DATA, &kTlvInfo, tmpField, tmpFieldLen);
	/*End HAL2402 PLUTONL-4391*/

	/*AMER20170311 Start: PROD00039737*/
	if (GetSmsField(SMS_RESPONSE_CODE, SmsInfo, FieldData, &FieldLen) == SUCCES)
	{
		if(!memcmp(FieldData,SMS_F039_PARTIAL_APPROVAL,2))
		{
			if(GetSmsField (SMS_TRANS_AMOUNT, SmsInfo, FieldData, &FieldLen) == SUCCES)
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

	trace_event("End   SmsField48ToIso(OK)", PROCESSING);
	return (OK);

}