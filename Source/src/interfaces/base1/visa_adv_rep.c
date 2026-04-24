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


int             VisaEchoedField_0120[] =
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
	/* 015 */ 0,			/* STLMNT_DATE           */
	/* 016 */ 0,			/* CNVRSN_DATE           */
	/* 017 */ 0,			/* CAPTURE_DATE          */
	/* 018 */ 0,			/* MERCHANT_TYPE         */
	/* 019 */ 1,			/* ACQR_COUNTRY          */
	/* 020 */ 0,			/* PAN_COUNTRY           */
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
	/* 039 */ 1,			/* RESPONSE_CODE                       */
	/* 040 */ 0,			/* SERVICE RESTRICTION CODE            */
	/* 041 */ 1,			/* TERMINAL_NBR                        */
	/* 042 */ 1,			/* OUTLET_NBR                          */
	/* 043 */ 0,			/* TERMINAL_ADR                        */
	/* 044 */ 0,			/* ADTNL_RESP_DATA                     */
	/* 045 */ 0,			/* T1_DATA                             */
	/* 046 */ 0,			/* ADDITRIONAL DATA ISO                */
	/* 047 */ 0,			/* ADDITIONAL DATA NATIONAL            */
	/* 048 */ 0,			/* ADTNL_DATA_PRIV                     */
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
	/* 100 */ 1,			/* RECEIVER_ID           */
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
	/* 116 */ 1,			/* Reserved National     */
	/* 117 */ 1,			/* Reserved National     */
	/* 118 */ 1,			/* Intra Country Data    */
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

#include <p7_services.h>



/****************************************************************************/
/* AutReplyToNw()                                                         */
/* */
/* Constructs and sends an authorization response message (0110) to VISA. */
/****************************************************************************/
int AdvReplyToNw(	int nIndexCtx,
					TSIsoInfo * IsoInfo,
					TSVisaInfo * OrigVisaInfo)
{
	TSVisaInfo      NewVisaInfo;
	TSTlvPrivate    TlvInfo;
	char            sProcCode[256];
	char            FieldData[256];
	char            sPufData[256];
	char            sBuffer[256];
	char            tmpField[256];
	int             FieldLen;
	int             Length;
	char            CvvResult[1 + 1];
	char			ARQCResult [ 32 + 1 ];
	char 			cavv_result[  1 + 1 ];
	char 			caav_field [256];
	char           cavv_valid_result = '2';
	char           cavv_invalid_result = '1';
	int             i;
	TSVisaPufInfo   VisapufInfo;
	char            sLine[MAX_LINE_TRC];
	char			PosCond[3];
	char           sVisaRespCode[256];

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
	InitVisaInfo(&NewVisaInfo);
	NewVisaInfo.nMsgType = 130;

	GetIsoField(ISO_PROC_CODE, IsoInfo, sIsoProcCode, &FieldLen);

	for (i = 0; i < VISA_AUDIT_NBR; i++) 
	{
		if (VisaEchoedField_0120[i])
		{
			if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
		}
	}

	/* restore VISA field 11 from ISO field 48 */
	InitTlvPrivate(&TlvInfo);
	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, FieldData, &FieldLen) == SUCCESS) 
	{
		AnalyseTlvPrivate(FieldData, FieldLen, &TlvInfo);

		if (GetTlvPrivate(EXTERNAL_STAN_TAG, &TlvInfo, FieldData, &FieldLen) == SUCCESS)
			AddVisaField(VISA_AUDIT_NBR, &NewVisaInfo, FieldData, FieldLen);
	}

	for (i = VISA_AUDIT_NBR + 1; i < VISA_AUTHOR_ID; i++) 
	{
		if (VisaEchoedField_0120[i])
		{
			if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
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
	
	for (i = VISA_RESPONSE_CODE + 1; i < MAX_VISA_FIELDS; i++)
	{
		if (VisaEchoedField_0120[i])
			if (GetVisaField(i, OrigVisaInfo, FieldData, &FieldLen) == SUCCESS)
				AddVisaField(i, &NewVisaInfo, FieldData, FieldLen);
	}




	/* send it to VISA */
	Length = VisaBuildMsg(sBuffer + VISA_HDR_LEN, &NewVisaInfo);

	VisaBuildHeader(RESPONSE, &(NewVisaInfo.sHeader), &(OrigVisaInfo->sHeader), Length, StationId);
	memcpy(sBuffer, (char *) &(NewVisaInfo.sHeader), VISA_HDR_LEN);
	Length += VISA_HDR_LEN;

	dump_buffer(sBuffer, Length, 'E', 'L', 'O');

	DumpVisa(sBuffer);
	if (WriteLineMsg(sBuffer, Length)) 
	{
		trace_event("End  AutReplyToNw(NOK): WriteLineMsg() failed", ERROR);
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}

	/* YDA20171225 (PROD00048309) */
	/*MsgRepSent(IsoInfo->msgId,sIsoRespCode);*/
	MsgIsoRepSent(IsoInfo->msgId,IsoInfo);

	trace_event("End  AutReplyToNw(OK)", PROCESSING);
	return (OK);
}

