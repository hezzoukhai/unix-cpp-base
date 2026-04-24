#include <stdio.h>
#include <string.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_visa.h>
#include <visa_param.h>
#include <tlv_private.h>
#include <security_data.h>
#include <visa_puf.h>
#include <visa_puf_fields.h>
#include <visa_define.h> /* SNO060815 add response 130 */
#include <p7_sid.h>
/*HAL20200304 for advice to nw*/
#include <list_thr.h>
#include <queue.h>
#include <p7_define.h>
#include <nw_raw_msg.h>
#include <iso8583.h>
#include <tlv_fields.h>

/*EBE140523: Adding static to avoid conflict with the once defined in visa_reversal.c*/
static int             VisaEchoedField_0400[] =
{
	0,            /* BIT_MAP2                       */
	1,            /* CARD_NBR                       */
	1,            /* PROC_CODE                      */
	1,            /* TRANS_AMOUNT                   */
	0,            /* STLMNT_AMOUNT                  */
	0,            /* CARDHOLDER BILLING AMOUNT      */
	1,            /* XMIT_TIME                      */
	0,            /* CARDHOLDER BILLING FEE         */
	0,            /* CONVRSN_RATE_STLMNT            */
	0,            /* CONVRSN_RATE_CARD_HLDR_BILLING */

	1,            /* AUDIT_NBR             */
	0,            /* TRANS_TIME            */
	0,            /* TRANS_DATE            */
	0,            /* EXPIRY_DATE           */
	0,            /* STLMNT_DATE           */
	0,            /* CNVRSN_DATE           */
	0,            /* CAPTURE_DATE          */
	0,            /* MERCHANT_TYPE         */
	1,            /* ACQR_COUNTRY          */
	1,            /* PAN_COUNTRY           */

	0,            /* FORWD_COUNTRY                     */
	0,            /* POS_ENTRY_MODE                    */
	0,            /* CARD SEQUENCE NUMBER              */
	0,            /* NETWORK INTERNATIONAL IDENTIFIER  */
	1,            /* POS_CONDITION                     */
	0,            /* POS_PIN_CAPTURE                   */
	0,            /* AUTHORIZATION ID RESPONSE LENGTH  */
	0,            /* TRANS_FEE                         */
	0,            /* AMOUNT SETTLEMENT FEE             */
	0,            /* AMOUNT TRANSACTION PROCESSING FEE */

	0,            /* AMOUNT SETLMNT PROCESSING FEE     */
	1,            /* ACQR_ID                           */
	0,            /* FORWD_ID                          */
	0,            /* PAN_EXTENDED                      */
	0,            /* T2_DATA                           */
	0,            /* TRACK 3 DATA                      */
	1,            /* REFERENCE_NBR                     */
	0,            /* AUTHOR_ID                         */
	0,            /* RESPONSE_CODE                     */
	0,            /* SERVICE RESTRICTION CODE          */

	1,            /* TERMINAL_NBR                      */
	1,            /* OUTLET_NBR                        */
	0,            /* TERMINAL_ADR                      */
	1,            /* ADTNL_RESP_DATA                   *//* must * be echoed *//* OBE20061222 */
	0,            /* T1_DATA                           */
	0,            /* ADDITRIONAL DATA ISO              */
	0,            /* ADDITIONAL DATA NATIONAL          */
	0,            /* ADTNL_DATA_PRIV                   */
	1,            /* TRANS_CRNCY                       */
	0,            /* STLMNT_CRNCY                      */

	0,            /* BILLING_CRNCY         */
	0,            /* PIN_DATA              */
	0,            /* SECURITY_DATA         */
	0,            /* ADTNL_AMOUNTS         */
	0,            /* Reserved Iso          */
	0,            /* Reserved Iso          */
	0,            /* Reserved National     */
	0,            /* Reserved National     */
	0,            /* GEOGR_DATA            */
	0,            /* ADTNL_POS_INFO        */

	0,            /* OTHER_AMOUNTS         */
	0,            /* CPS_FIELDS            */
	1,            /* PRIV_USE_FIELDS       *//*SEW280208 ENH April 2008*/
	0,            /* MAC1                  */
	0,            /* */
	0,            /* STLMNT_CODE           */
	0,            /* Extended Payment Code */
	0,            /* RECEIVER_COUNTRY      */
	0,            /* STLMNT_INST_COUNTRY   */
	0,            /* NW_MNGMT_INFO         */

	0,            /* Message Number              */
	0,            /* Message number Last         */
	0,            /* ACTION_DATE                 */
	0,            /* CR_NBR                      */
	0,            /* CR_REV_NBR                  */
	0,            /* DB_NBR                      */
	0,            /* DB_REV_NBR                  */
	0,            /* Transfer Number             */
	0,            /* Transfer Reversal Number    */
	0,            /* Inquiries Number            */

	0,            /* Authorizations Number          */
	0,            /* Credit, Processing Fee Amount  */
	0,            /* Credit, Transaction Fee Amount */
	0,            /* Debit, Processing Fee Amount   */
	0,            /* Debit, Transaction Fee Amount  */
	0,            /* CR_AMOUNT                      */
	0,            /* CR_REV_AMOUNT                  */
	0,            /* DB_AMOUNT                      */
	0,            /* DB_REV_AMOUNT                  */
	0,            /* ORIG_DATA                      */

	0,            /* FILE_UPDATE           */
	0,            /* FILE_SECURITY         */
	0,            /* Response Indicator    */
	0,            /* Service Indicator     */
	0,            /* REPLACE_AMOUNTS       */
	0,            /* MSG_SECURITY          */
	0,            /* NET_AMOUNT            */
	0,            /* Payee                 */
	0,            /* STLMNT_INSTITN        */
	0,            /* RECEIVER_ID           */

	0,            /* FILE_NAME             */
	0,            /* ACCOUNT_ID1           */
	0,            /* ACCOUNT_ID2           */
	0,            /* Transaction Description   */
	0,            /* Reserved Iso          */
	0,            /* Reserved Iso          */
	0,            /* Reserved Iso          */
	0,            /* Reserved Iso          */
	0,            /* Reserved Iso          */
	0,            /* Reserved Iso          */

	0,            /* Reserved Iso          */
	0,            /* Reserved National     */
	0,            /* Reserved National     */
	0,            /* Reserved National     */
	0,            /* ADTNL_TRACE_DATA      */
	0,            /* Reserved National     */
	0,            /* Reserved National     */
	0,            /* Intra Country Data    */
	0,            /* Reserved National     */
	0,            /* Original Message Type Id  */

	0,            /* Issuing Institution Id        */
	0,            /* Remaining Open to Use         */
	0,            /* ADR_VERIF_DATA                */
	0,            /* Free Form Text - Japan        */
	0,            /* SUPPORT_INFO                  */
	0,            /* Reserved Private              */
	0,            /* File Record Action and Data   */
	0,            /* MAC2                          */
	0,            /* Field 129                     */
	0,            /* Terminal Capability Profile   */

	0,            /* Terminal Verification Results */
	0,            /* Unpredictable Number          */
	0,            /* Terminal Serial Number        */
	0,            /* Visa Discretionary Data       */
	0,            /* Issuer Discretionary Data     */
	0,            /* Issuer Discretionary Data     */
	0,            /* Application Trans. Counter    */
	0,            /* Application Inter. Profile    */
	0,            /* ARPC + Response Code          */
	0,            /* Field 140                     */

	0,            /* Field 141                     */
	0,            /* Issuer Script                 */
	0,            /* Issuer Script Results         */
	0,            /* Cryptogram Trans Type         */
	0,            /* Terminal Country Code         */
	0,            /* Terminal Trans  Date          */
	0,            /* Cryptogram Amount             */
	0,            /* Cryptogram Currency Code      */
	0,            /* Cryptogram Cash Back          */
	0,            /* Cardholder Card Scheme Crypto. */

	0,            /* Field 151             */
	0,            /* Field 152             */
	0,            /* Field 153             */
	0,            /* Field 154             */
	0,            /* Field 155             */
	0,            /* Field 156             */
	0,            /* Field 157             */
	0,            /* Field 158             */
	0,            /* Field 159             */
	0,            /* Field 160             */

	0,            /* Field 161             */
	0,            /* Field 162             */
	0,            /* Field 163             */
	0,            /* Field 164             */
	0,            /* Field 165             */
	0,            /* Field 166             */
	0,            /* Field 167             */
	0,            /* Field 168             */
	0,            /* Field 169             */
	0,            /* Field 170             */

	0,            /* Field 171             */
	0,            /* Field 172             */
	0,            /* Field 173             */
	0,            /* Field 174             */
	0,            /* Field 175             */
	0,            /* Field 176             */
	0,            /* Field 177             */
	0,            /* Field 178             */
	0,            /* Field 179             */
	0,            /* Field 180             */

	0,            /* Field 181             */
	0,            /* Field 182             */
	0,            /* Field 183             */
	0,            /* Field 184             */
	0,            /* Field 185             */
	0,            /* Field 186             */
	0,            /* Field 187             */
	0,            /* Field 188             */
	0,            /* Field 189             */
	0,            /* Field 190             */

	0,            /* Field 191             */
	0,            /* Message Authentication Code   */
};



/* start  SNO060815 add response 130 */
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
	/* 015 */ /*0*/ 1,		/* STLMNT_DATE           */ /*NAB04032021 PLUTONL-2813*/
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
	/* 063 */ 1,			/* PRIV_USE_FIELDS       */			/*AMER20171121*/
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
	char            FieldData[BUF_LG_MAX];	/*AEA20210824 PLUTONL-3379*/
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
	char            Priv_Use_Field[255 + 1];	/*AMER20171121*/

	trace_event("Start  AdvReplyToNw()", PROCESSING);

	memset(sIsoRespCode,'\0',sizeof(sIsoRespCode));
	memset(caav_field               , 0 , sizeof(caav_field         ));
	memset(sBuffer, 0, sizeof(sBuffer));
	memset ( PosCond,  0 , sizeof ( PosCond ));
	memset ( sIsoProcCode,  0 , sizeof ( sIsoProcCode ));

	dump_p7_struct(IsoInfo);
	InitVisaInfo(&NewVisaInfo);
	if( OrigVisaInfo->nMsgType == 120 || OrigVisaInfo->nMsgType == 121 )
		NewVisaInfo.nMsgType = 130;
	else if( OrigVisaInfo->nMsgType == 220 || OrigVisaInfo->nMsgType == 221 )
		NewVisaInfo.nMsgType = 230;



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
		AddVisaField (VISA_RESPONSE_CODE, &NewVisaInfo, B1_F039_APPROVAL, B1_F039_LEN);
	}

	for (i = VISA_RESPONSE_CODE + 1; i < MAX_VISA_FIELDS; i++)
	{
		if (VisaEchoedField_0120[i])
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

	/* YDA20171225 (PROD00048309) */
	/* MsgRepSent(IsoInfo->msgId,sIsoRespCode);*/
	MsgIsoRepSent(IsoInfo->msgId,IsoInfo);

	trace_event("End  AdvReplyToNw(OK)", PROCESSING);
	return (OK);
}
/* end  SNO060815 add response 130 */

/****************************************************************************/
/* FCT_AUT_ADV_FROM_NW()                                                  */
/* */
/* Processes an authorization advice message (0120) received from VISA.   */
/****************************************************************************/
int 
	FCT_AUT_ADV_FROM_NW(int nIndexCtx, TSVisaInfo * VisaInfo)
{

	TSIsoInfo       IsoInfo;
	TSTlvPrivate    TlvInfo;
	int             nRetCode;
	char            sTlvBuffer[LG_MAX];


	trace_event("Start  FCT_AUT_ADV_FROM_NW()", PROCESSING);
	/* Initialize ISO info structure */
	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));


	InitIsoInfo(&IsoInfo);

	BuildAutReqFromNw(nIndexCtx,
			          VisaInfo,
					  &IsoInfo);

	/*EBE150912: keep the repeat ind. it could also be a 220 for completion advices
	IsoInfo.nMsgType = 1120;*/
	IsoInfo.nMsgType = VisaInfo->nMsgType + 1000;

	IsoInfo.msgId = MsgIsoReqReceived(ResourceId,&IsoInfo); /*EBE130925:SWMON*/


	nRetCode = ServiceProcessing(	nIndexCtx,
		ResourceId,
		"ADM",
		AcquirerBank,
		"G",
		"N",
		"N",
		sTlvBuffer, 
		&IsoInfo);

	/*Start IBO20200517 RAWMSG*/
	if (g_sCaptureMsg[NW_INC_REQ] == 'Y')
	{
		LogVisaMessage(nIndexCtx, VisaInfo, &IsoInfo, INC_REQ);
	}
	/*End IBO20200517 RAWMSG*/

	switch (nRetCode) 
	{
	/*PLUTONL-4242 ZKO Start 27012022*/
	case SEVERE_ERROR:
		if (AdvReplyToNw(nIndexCtx, &IsoInfo, VisaInfo) != OK) 
		{
			trace_message (P7_TL_ERROR,"Error when calling AdvReplyToNw(): [IndexCtx:%d]",nIndexCtx);
			trace_message(P7_TL_INFO,"End FCT_AUT_ADV_FROM_NW(NOK)");
			return (NOK);
		}
		trace_message (P7_TL_ERROR,"Error when calling ServiceProcessing(): [IndexCtx:%d, ResourceId:%s]",nIndexCtx, ResourceId);
		trace_message(P7_TL_INFO,"End FCT_AUT_ADV_FROM_NW(NOK):SEVERE_ERROR");
		return (NOK);
		/*shutdown_resource(nIndexCtx, ResourceId);*/
	/*PLUTONL-4242 ZKO End 27012022*/
	/* start  SNO060815 add response 130 */
	case OK:		/* the interface itself replies to the network */
	default:
		if (AdvReplyToNw(nIndexCtx, &IsoInfo, VisaInfo) != OK) 
		{
			trace_message (P7_TL_ERROR,"Error when calling AdvReplyToNw(): [IndexCtx:%d]",nIndexCtx);
			trace_message(P7_TL_INFO,"End FCT_AUT_ADV_FROM_NW(NOK)");
			return (NOK);
		}
		break;
	/* end  SNO060815 add response 130 */
	}
	/*   SNO060815 add response 130
	MsgIsDone(IsoInfo.msgId);*/

	trace_message(P7_TL_PROCESSING,"End FCT_AUT_ADV_FROM_NW(OK): [nIndexCtx:%d]", nIndexCtx);
	return (OK);
}



/****************************************************************************/
/* FCT_REV_ADV_FROM_NW()                                                  */
/* */
/* Processes a reversal advice message (0420) received from VISA.         */
/****************************************************************************/
int FCT_REV_ADV_FROM_NW(int nIndexCtx, TSVisaInfo * VisaInfo)
{
	TSVisaInfo      NewVisaInfo;
	TSIsoInfo       IsoInfo;
	int             result;
	int             Length;
	char            sPid[8 + 1];
	char			sTlvBuffer[LG_MAX];

	


	trace_event("Start  FCT_REV_ADV_FROM_NW()", PROCESSING);


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
	/*PLUTONL-4242 ZKO Start 27012022*/
	case OK: 	/* the interface itself responds */
	case NOK:
		if (RevAdviceRepToNw(nIndexCtx, &IsoInfo, VisaInfo) != OK) {
			trace_message (P7_TL_ERROR,"Error when calling RevAdviceRepToNw(): [IndexCtx:%d]",nIndexCtx);
			trace_message(P7_TL_INFO,"End FCT_REV_ADV_FROM_NW(NOK)");
			return (NOK);
		}
		trace_message(P7_TL_PROCESSING,"End FCT_REV_ADV_FROM_NW(OK): [nIndexCtx:%d]", nIndexCtx);
		break;
	
	case SEVERE_ERROR:
		/*shutdown_resource(nIndexCtx, ResourceId);*/
		if (RevAdviceRepToNw(nIndexCtx, &IsoInfo, VisaInfo) != OK) {
			trace_message (P7_TL_ERROR,"Error when calling RevAdviceRepToNw(): [IndexCtx:%d]",nIndexCtx);
			trace_message(P7_TL_INFO,"End FCT_REV_ADV_FROM_NW(NOK)");
			return (NOK);
		}
		trace_message (P7_TL_ERROR,"Error when calling ServiceProcessing(): [IndexCtx:%d, ResourceId:%s]",nIndexCtx, ResourceId);
		trace_message(P7_TL_INFO,"End FCT_REV_ADV_FROM_NW(NOK):SEVERE_ERROR");
		return (NOK);
	/*PLUTONL-4242 ZKO End 27012022*/
	}

	return (OK);
}


/*************************************************************************/
/* ReversalRepToNw()                                                   */
/* AER add reply to 400 messages*/
/* Prepares an reversal response message (0410) to VISA.               */
/* Input:  ISO format message                                          */
/* Output: VISA message                                                */
/*************************************************************************/

int RevAdviceRepToNw(	int			nIndexCtx,
						TSIsoInfo * IsoInfo,
						TSVisaInfo * OrigVisaInfo
						)
{
	TSTlvPrivate    TlvInfo;
	TSVisaInfo 		NewVisaInfo;
	char            tmpField[256];
	char            tmpField1[256];
	char            FieldData[256];
	char            isoResponse[4];
	char            sBuffer[1024];
	int             tmpFieldLen;
	int				Length;
	int             i;

	trace_event("Start  RevAdviceRepToNw()", PROCESSING);
	
	
	InitVisaInfo(&NewVisaInfo);
	memset(sBuffer, 0, sizeof(sBuffer));

	NewVisaInfo.nMsgType = 430;

	for (i = 0; i < MAX_VISA_FIELDS; i++) 
	{
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
			GetIsoField(ISO_RESPONSE_CODE, IsoInfo, isoResponse, &tmpFieldLen);
			TranslateRespCode(nIndexCtx, ResourceId, isoResponse, "PR", tmpField);
			AddVisaField(VISA_RESPONSE_CODE, &NewVisaInfo, tmpField, 2);
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
				/*Start AMER20171116 PROD00049633: Don't send F4 and F49 in 0430 message*/
				if(NewVisaInfo.nMsgType == 430 && (i== VISA_TRANS_AMOUNT || i == VISA_TRANS_CRNCY))
					continue;
				/*End AMER20171116 PROD00049633*/
				
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


	dump_buffer(sBuffer, Length, 'E', 'L', 'O');

	DumpVisa(sBuffer);

	if (WriteLineMsg(sBuffer, Length)) 
	{
		trace_message ( P7_TL_ERROR,"Error when calling WriteLineMsg(): [Length:%d]",Length);
		trace_message(P7_TL_INFO,"End RevAdviceRepToNw(NOK)");
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}

    /* YDA20171225 (PROD00048309) */
    /* MsgRepSent(IsoInfo.msgId,"000");*/
	MsgIsoRepSent(IsoInfo->msgId,IsoInfo);

	trace_event("End  RevAdviceRepToNw(OK)", PROCESSING);
	return (OK);
}

/*Start HAL ENHQ2.20 completion advice Art 1.1*/
/*************************************************************************/
/* FCT_AUT_ADVICE_TO_NW()                                                 */
/* 
/*************************************************************************/
int
	FCT_AUT_ADVICE_TO_NW(int nIndexCtx, TSIsoInfo * IsoInfo, char *sTlvBuffer)
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
	struct timeval  tPurgeTime;
	char 			sSecurityData [128 + 1];




	trace_event("Start  FCT_AUT_ADVICE_TO_NW(S)", PROCESSING);

	memset(sBuffer, 0, sizeof(sBuffer));
	memset(DestId, 0, sizeof(DestId));
	memset (sSecurityData, 0, sizeof(sSecurityData));



	/* Convert message to VISA format */
	dump_p7_struct(IsoInfo);

	result = AdviceReqToNw(nIndexCtx,IsoInfo, &VisaInfo);

	/* Insert original ISO message in list for later retrieval */
	InitNodeData(&Node);
	GetSafPurgeTimeMS(&tPurgeTime);
	Node.tPurgeTime = tPurgeTime.tv_sec;
	Node.stPurgeTime.tv_sec = tPurgeTime.tv_sec;
	Node.stPurgeTime.tv_usec = tPurgeTime.tv_usec;


	memcpy(Node.szFstKey, "1120", 4);

	GetVisaField(VISA_AUDIT_NBR, &VisaInfo, tmpField, &tmpFieldLen);

	memcpy(Node.szFstKey + 4, tmpField, 6);

	memcpy(Node.sFstPrvtData, "SAF", 3);
	Node.nFstPrvtDataLen = 3;

	Length = IsoBuildMsg(Node.sFstBuf, IsoInfo);
	Node.nFstBufLen = Length;

	Node.nSndBufLen = strlen(sTlvBuffer);
	memcpy(Node.sSndBuf, sTlvBuffer, Node.nSndBufLen);

	if (InsertRequest('B', &Node) != OK) 
	{
		sprintf(tmpField, "%.3d", SYSTEM_MALFUNCTION);
		trace_event("FCT_AUT_ADVICE_TO_NW(NOK): InsertRequest() failure.", ERROR);
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
		trace_event("FCT_AUT_ADVICE_TO_NW(NOK): WriteLineMsg() failure.", ERROR);
		IntWriteLineExeption(nIndexCtx,IsoInfo); /* ELKAM PLUTONL-3835 */
		return (NOK);
	}


	trace_event("End  FCT_AUT_ADVICE_TO_NW(OK)", PROCESSING);
	return (OK);
}


int AdviceReqToNw(int nIndexCtx, TSIsoInfo * IsoInfo, TSVisaInfo * VisaInfo)
{
	TSCpsInfo       CpsInfo;
	TSTlvPrivate    TlvInfo;
	TSTlvPrivate    TlvAddDataPriv;
	TSTagIso        TagIso;
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
	char          Priv_Use_Field[255 + 1];
	char          vBitMap[3 +1];
	char			szAcquirerBank[6 + 1];
	char 			isoFunctionCode[4];
	char			isoTrxAmnt[13];
	int				verification_only = 0;
	char			RecurringFlag[2];
	TSVisaPufInfo   VisapufInfo;
	char            sVisaPufData[512];
	char            sAddAuthInfo[P7_F048_P64_LEN + 1];
	char 			sPosExtData[P7_F048_P61_LEN +1];
	TSPufInfo   	PufInfo; /*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 6.1.2*/
	
	char 			sExtStan[P7_F011_LEN+1];
	char 			sXmiTime[P7_F007_LEN+1];



	memset(tmpField, 0, sizeof(tmpField));
	memset(tmpField1, 0, sizeof(tmpField1));
	memset(tmpField, 0, sizeof(tmpField));
	memset(MerchantType, 0, sizeof(MerchantType));
	memset(isoProcCode, 0, sizeof(isoProcCode));
	memset(isoPosMode, 0, sizeof(isoPosMode));
	memset(sTraceAudit, 0, sizeof(sTraceAudit));
	memset(szAcquirerBank, 0, sizeof(szAcquirerBank));
	memset(OutletNumber, 0, sizeof(OutletNumber));
	memset(NwPosMode, 0, sizeof(NwPosMode));
	memset(isoFunctionCode, 0, sizeof(isoFunctionCode));
	memset(RecurringFlag, 0, sizeof(RecurringFlag));
	memset(sAddAuthInfo, 0, sizeof(sAddAuthInfo));
	memset(sPosExtData, 0, sizeof(sPosExtData));
	memset (tmpField1, 0, sizeof(tmpField1));
	memset (tmpField2, 0, sizeof(tmpField2));
	memset (sExtStan, 0, sizeof(sExtStan));
	memset (sXmiTime, 0, sizeof(sXmiTime));

	memset (sExtStan, 0, sizeof(sExtStan));
	memset (sXmiTime, 0, sizeof(sXmiTime));

	trace_event("Start  AdviceReqToNw()", PROCESSING);

	InitVisaInfo(VisaInfo);
	InitTlvPrivate(&TlvAddDataPriv);

	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS) 
	{
		AnalyseTlvPrivate(tmpField, tmpFieldLen, &TlvAddDataPriv);
	}

	VisaInfo->nMsgType = IsoInfo->nMsgType - 1000;

	/* (2) Primary Account Number */
	if (GetIsoField(ISO_CARD_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AddVisaField(VISA_CARD_NBR, VisaInfo, tmpField, tmpFieldLen);
	}

	/* (3) Processing Code */
	strcpy(sMcc, "0000");
	GetIsoField(ISO_PROC_CODE, IsoInfo, isoProcCode, &tmpFieldLen);
	GetIsoField(ISO_MERCHANT_TYPE, IsoInfo, sMcc, &tmpFieldLen);
	IsoToVisaProcessingCode(isoProcCode, tmpField, sMcc);
	AddVisaField(VISA_PROC_CODE, VisaInfo, tmpField, 6);

	/* (4): Transaction Amount */
	if (GetIsoField(ISO_TRANS_AMOUNT, IsoInfo, isoTrxAmnt , &tmpFieldLen)==SUCCES)
	{
        AddVisaField(VISA_TRANS_AMOUNT, VisaInfo, isoTrxAmnt , 12);
	}

	/* (7) Transaction Date/Time */
	
	/* ELKAM START PLUTONL-3914 */

	if (GetIsoField(ISO_XMIT_TIME, IsoInfo, sXmiTime, &tmpFieldLen)==SUCCES)
	{
	    AddVisaField(VISA_XMIT_TIME, VisaInfo, sXmiTime, 10);
	}
	/* if (GetIsoField (ISO_AUDIT_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AddVisaField(VISA_AUDIT_NBR, VisaInfo, tmpField, 6);
	} */

	
	/* restore VISA field 11 from ISO field 48 */
	InitTlvPrivate(&TlvInfo);
	
	if (GetIsoField(ISO_ADTNL_DATA_PRIV, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		AnalyseTlvPrivate(tmpField, tmpFieldLen, &TlvInfo);
		

		if (GetTlvPrivate(EXTERNAL_STAN_TAG, &TlvInfo, sExtStan, &tmpFieldLen) == SUCCESS)
			AddVisaField(VISA_AUDIT_NBR, VisaInfo, sExtStan, 6);
	}

	/* ELKAM END PLUTONL-3914 */

	/* (18) Merchant Type */
	if (GetIsoField(ISO_MERCHANT_TYPE, IsoInfo, MerchantType, &tmpFieldLen)==SUCCES)
	{
	    AddVisaField(VISA_MERCHANT_TYPE, VisaInfo, MerchantType, 4);
	}

	/* (19) Acquiring Institution Country Code */
	if(GetIsoField(ISO_ACQR_COUNTRY, IsoInfo, tmpField, &tmpFieldLen)==SUCCES)
	{
	    AddVisaField(VISA_ACQR_COUNTRY, VisaInfo, tmpField, 3);
	}

	/* (22) POS Entry Mode */
	if (GetIsoField(ISO_POS_DATA, IsoInfo, isoPosMode, &tmpFieldLen) == SUCCESS) 
	{
		IsoToVisaPosMode(isoPosMode, NwPosMode);
		AddVisaField(VISA_POS_ENTRY_MODE, VisaInfo, NwPosMode , 4);
	}

    /* (23) CARD SEQUENCE NBR */
	if (GetIsoField(ISO_CARD_SEQUENCE_NBR, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
        AddVisaField(VISA_CARD_SEQUENCE_NBR, VisaInfo, tmpField, 3);
	}
		
	/* (25) POS Condition Code */
	IsoToVisaPosCondition(isoPosMode, MerchantType, 0, tmpField);
	AddVisaField(VISA_POS_CONDITION, VisaInfo, tmpField, B1_F025_LEN);	

	/* (32) Acquiring Institution Code */
	if (GetTlvPrivate(ACQUIRER_BANK_TAG, &TlvAddDataPriv, szAcquirerBank , &tmpFieldLen) != SUCCESS)
	{
		trace_event("End AdviceReqToNw (NOK) : ERROR Getting Acquirer bank",ERROR);
		return(NOK);
	}
	nRetCode = GetAquirerBin( nIndexCtx, szAcquirerBank, NET_VISA, AquirerBin)  ;
	if (nRetCode != OK)
	{
		sprintf(sLine,"End AdviceReqToNw (NOK) : ERROR Getting AquirerBin for bank[%s]",szAcquirerBank);
		trace_event(sLine,ERROR);
		return(NOK);
	}
	AddVisaField (VISA_ACQR_ID, VisaInfo, AquirerBin , strlen(AquirerBin));	


	/* (37) Reference Number */
	 
	if(GetTlvPrivate (NET_REFERENCE_NUMBER, &TlvAddDataPriv, tmpField, &tmpFieldLen) == SUCCESS )
	{	    
		AddVisaField(VISA_REFERENCE_NBR, VisaInfo, tmpField, 12); 
	}
    
     /* Field 38: Authorization Id */
	if (GetIsoField(ISO_AUTHOR_ID, IsoInfo, tmpField, &tmpFieldLen)==SUCCES)
	{
        AddVisaField(VISA_AUTHOR_ID, VisaInfo, tmpField, 6); 
	}
	

	/* (42) Card Acceptor Id */
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

	/* (49) Transaction Currency Code */
	if (GetIsoField(ISO_TRANS_CRNCY, IsoInfo, tmpField, &tmpFieldLen)==SUCCES)
	{
        AddVisaField(VISA_TRANS_CRNCY, VisaInfo, tmpField, 3);
	}
	
	/* (60) Additional POS Info */
	if (GetIsoField(ISO_POS_DATA, IsoInfo, isoPosMode, &tmpFieldLen) == SUCCESS) 
	{
        memset(tmpField, 0, sizeof(tmpField));
		GetTlvPrivate( RECURRING_FLAG_TAG , &TlvAddDataPriv, RecurringFlag, &tmpFieldLen);
		GetTlvPrivate( AUTH_ADD_INFO , &TlvAddDataPriv, sAddAuthInfo, &tmpFieldLen);		
		GetTlvPrivate( POS_DATA_EXT_TAG , &TlvAddDataPriv, sPosExtData, &tmpFieldLen);	/*PLUTONL-3326*/
		BuildAddPosInfo(isoPosMode, MerchantType, isoProcCode, RecurringFlag, sAddAuthInfo,sPosExtData, tmpField, IsoInfo);	
		AddVisaField(VISA_ADTNL_POS_INFO, VisaInfo, tmpField, strlen(tmpField));
	}

	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 6.1.2*/
	/* (62)   CPS Fields */
	if (GetIsoField (ISO_CPS_FIELDS, IsoInfo, tmpField, &tmpFieldLen) == SUCCESS)
	{
		InitTlvPrivate (&TlvInfo);
		AnalyseTlvPrivate (tmpField, tmpFieldLen, &TlvInfo);
		InitCpsInfo (&CpsInfo);

		if (GetTlvPrivate (CPS_TRANS_ID_TAG, &TlvInfo, tmpField, &tmpFieldLen)  == SUCCESS)
			AddCpsSubField (CPS_TRANS_ID, &CpsInfo, tmpField, 15);

		tmpFieldLen = CpsBuildField (tmpField, &CpsInfo);
		if (tmpFieldLen > 8)
			AddVisaField (VISA_CPS_FIELDS, VisaInfo, tmpField, tmpFieldLen);
	}

	/* Field 63:*/
	InitPufInfo (&PufInfo);
	AddPufSubField (PUF_NETWORK_ID, &PufInfo, "0000", 4);

	if (GetIsoField (ISO_MERCHANT_TYPE, IsoInfo,tmpField,&tmpFieldLen) == SUCCESS)
	{
		if(memcmp(tmpField, MCC_AFD, tmpFieldLen) == 0)
		AddPufSubField (PUF_MSG_REASON, &PufInfo, MSG_REASON_COMPLETION_ADVICE, 4);
	}
	
	tmpFieldLen = PufBuildField (tmpField, &PufInfo);
	if (tmpFieldLen > 0)      	                    
	AddVisaField (VISA_PRIV_USE_FIELDS, VisaInfo, tmpField, tmpFieldLen);
	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 Art 6.1.2*/

	trace_event("End  AdviceReqToNw(OK)", PROCESSING);
	return (OK);
}


int
	FCT_ADV_REP_FROM_NW (int nIndexCtx, TSVisaInfo * VisaInfo)
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

	trace_event("Start  FCT_ADV_REP_FROM_NW ()", PROCESSING);
	memset(RespCode, 0, sizeof(RespCode));
	memset(tmpField, 0, sizeof(tmpField));
	memset(SourceResource, 0, sizeof(SourceResource));
	memset(sResource, 0, sizeof(sResource));
	memset(sTlvBuffer, 0, sizeof(sTlvBuffer));
	memset (sSecurityData, 0, sizeof(sSecurityData));


	/* free request from list */
	GetVisaField(VISA_AUDIT_NBR, VisaInfo, tmpField, &tmpFieldLen);
	InitNodeData(&Node);
	memcpy(Node.szFstKey, "1120", 4);
	memcpy(Node.szFstKey + 4, tmpField, 6);
	sprintf(sLine, "Cle REP Is :%s", Node.szFstKey);
	trace_event(sLine, TRACE);


	switch (FetchRequest('B', &Node)) 
	{
	case ENOEXIST:
		trace_event("FetchRequest failed (ENOEXIST)", ERROR);
		trace_event("End FCT_ADV_REP_FROM_NW (NOK)", ERROR);
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
		"1130", 
		RespCode, 
		tmpField);

	/*Start IBO20200517 RAWMSG*/
	if (g_sCaptureMsg[NW_INC_RESP] == 'Y')
	{
		LogVisaMessage(nIndexCtx, VisaInfo, &IsoInfo, INC_RESP);
	}
	/*End IBO20200517 RAWMSG*/

	trace_event("End  FCT_ADV_REP_FROM_NW(OK)", PROCESSING);
	return (OK);
}
/*End HAL ENHQ2.20 completion advice Art 1.1*/
