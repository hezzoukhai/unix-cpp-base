/****************************************************************************/
/***************************************************************************/
/* UTILS_SID.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/* MESSAGE  SID 8583  EURO    (*F.M*)            */

/*change log*/
/*ref						Author			activity		date			comments
/*--------------------------------------------------------------------------------------------------------------
/*JOU20170713				JOUADIM			PROD00044789	13/07/2017		Dump all sub elemnt DE48 in dumpSid
*/
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <header_sms.h>
#include <define.h>

#include <iso_sid.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <security_data.h>

#define SUCCES  0
#define ECHEC  -1

typedef struct s_tlv_print {
	char           *description;
	char           *value;
}               t_tlv_print;

static char           *sSecurityDesc[] = {
	"SECURITY FORMAT         ",
	"PIN BLOCK FORMAT        ",
	"PIN ENCRYPTION KEY INDEX",
	"MAC KEY INDEX           ",
	"SOURCE PID              ",
	"SOURCE RESOURCE         ",
	"DESTINATION PID         ",
	"DESTINATION RESOURCE    ",
	"USER                    ",
	"PURGE TIME              ",
	"SOURCE NODE             ",
	"DESTINATION NODE        ",
};

static t_tlv_print     sAdditRespDataDescsid[] = {
	{"BAL_TYPE_TAG...........", "A09"}
};

static t_tlv_print     sPrivateDataDesc[] = {
	{"FILE_UPDATE_ERR_CODE...", "P01"},
	{"MESSAGE_TEXT............", "P02"},
	{"HPS_LIMITS_ID...........", "P03"},
	{"HPS_LIMITS_AMOUNT_TAG...", "P04"},
	{"HPS_HOST_SCENARIO_CODE..", "P05"},
	{"CHECK_SET_ON_OFF_LIMIT..", "P06"},
	{"TELECODE_VERIF_REQ......", "P07"},
	{"TELECODE_VERIF_RES......", "P08"},
	{"HPS_ACQUIRER_BANK.......", "P09"},
	{"KEY_EXCHANGE_BLOC_DATA..", "P11"},
	{"PRIS_INTERNAL_ID........", "P12"},
	{"EXTERNAL_STAN...........", "P13"},
	{"PVT_DATA................", "P15"},
	{"PIN_DATA................", "P16"}, /*JOU20170713*/
	{"REFEREBCE_INVOICE_NUMBER", "P17"}, /*JOU20170713*/
	{"PAYMENT_ACCOUNT_REF.....", "P18"}, /*JOU20170713*/
	{"E_COMMERCE_CERTIFICATE.", "P40"},
	{"VISA_MERCHANT_VERIF_VAL.", "P19"}, /*JOU20170713*/
	{"EXTERNAL_iCVV_RESULT....", "P20"}, /*JOU20170713*/
	{"TLV_SUBFIELDS...........", "P21"}, /*JOU20170713*/
	{"CARD_AUTH_RESULT........", "P25"}, /*JOU20170713*/
	{"EXTERNAL_PIN_VERIF_RES..", "P27"}, /*JOU20170713*/
	{"CHIP_AUTHO_ADD_INFO.....", "P30"}, /*JOU20170713*/
	{"MERCHANT_ID_BY_MC.......", "P32"}, /*JOU20170713*/
	{"MC_TRACE_ID.............", "P35"}, /*JOU20170713*/
	{"SECURITY_LEVEL_IND.....", "P41"},
	{"MC_UCAF................", "P43"}, /*JOU20170713*/
	{"EXTERNAL_CRYPT_RESULTS.", "P44"},
	{"ROUTING_NETWORK_CODE...", "P47"}, /*JOU20170713*/
	{"TCC....................", "P48"},
	{"DUKPT_KSN_DESCRIPTION..", "P49"}, /*JOU20170713*/
	{"KSN....................", "P50"}, /*JOU20170713*/
	{"ACCOUNT_AUTHENTIF_VAL..", "P51"}, /*JOU20170713*/
	{"EXTERNAL_ECOM_CRYPT_RES", "P54"}, /*JOU20170713*/
	{"TOKEN_DATA.............", "P55"}, /*JOU20170713*/
	{"DCC_DATA...............", "P56"}, /*JOU20170713*/
	{"AFD_REFERENCE_NUMBER...", "P57"}, /*JOU20170713*/
	{"ACCEPTOR_CARDPRODUCT_ID", "P58"}, /*JOU20170713*/
	{"WATCHLIST_SCORING_RES..", "P60"}, /*JOU20170713*/
	{"ADDITIONAL_POS_DATA....", "P61"}, /*JOU20170713*/
	{"TCC_AND_RFU............", "P64"}, /*JOU20170713*/
	{"AUTHORIZATION_ID.......", "P67"}, /*JOU20170713*/	
	{"TRANSACTION_ID.........", "P68"}, /*JOU20170713*/
	{"TERMINAL_INFORMATION...", "P70"}, /*JOU20170713*/
	{"RPI....................","P71" }, /* SLA 080212 Recurring Payment */
	{"DINERS_NETWORK_REF_ID..", "P80"}, /*JOU20170713*/
	{"ADDRESS_VERIF_REQ_TYPE.", "P82"},
	{"ADDRESS_VERIF_REP_TYPE.", "P83"},
	{"EXTERNAL_RESP_SOURCE    ", "P85"},
	{"EXTERNAL_CVV_RESULT_CODE", "P87"},
	{"EXTERNAL_CVV2_RESULT_CODE", "P88"},
	{"CVC1_TRACK_EDIT_ERROR   ", "P89"},
	{"FRAUD_SCORE.............", "P90"}, /*JOU20170713*/
	{"CVC2_DATA	           ", "P92"},
	{"HSM_RESULT_CODE         ", "P93"},
	{"SECONDARY_RESOURCE      ", "P94"},
	{"NETWORK_CODE............", "P95"}, /*JOU20170713*/
	{"MALFUNCTION_STIP        ", "P96"},
	{"ACQUIRER GENRE          ", "P97"},
	{"TPK/AWK                 ", "P10"},
	{"MONEY_TRANSFER_DATA.....", "S20"}, /*JOU20170713*/
	{"ADDRESS_DATA............", "S21"}, /*JOU20170713*/
	{"SMS_CHARGEBACK_DATA.....", "K01"}, /*JOU20170713*/
	{"NET_VISA_OVERALL_ASSESSMENT_SECURE_ECOM........", "N25"}, /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.1*/  
	{"NET_VISA_PURCHASE_IDENTIFIER........", "N26"} /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art8.3.1*/ 
};


static t_tlv_print     sPrivateCpsDescsid[] = {
	{"AUTH_CHAR_IND...........", "F01"},
	{"TRANSACTION_IND.........", "F02"},
	{"VALIDATION_CODE.........", "F03"},
	{"MARKET_SPECIFIC_DATA....", "F04"},
	{"DURATION................", "F05"},
	{"PRESTIGEOUS_PROP_IND....", "F06"},
	{"MCI_INTERCHANGE_COMP....", "F17"}
};

static t_tlv_print     sPrivateAddPosDescsid[] = {
	{"CARD_AUTH_REL_IND ......", "L01"},
	{"CHIP_CONDITION_CODE.....", "L02"},
	{"CCPS_TRANSACTION_IND....", "L03"}
};

static t_tlv_print     sPrivateNetworkDescsid[] = {
	{"N_POS_ENTRY_MODE........", "022"},
	{"N_POS_CONDITION_CODE....", "025"},
	{"N_RESPONSE_CODE.........", "039"},
	{"N_POS_DATA_VISA.........", "060"},
	{"N_POS_DATA_EURO.........", "061"},
	{"N_FILE_UPDATE_CODE......", "091"},
	{"N_UF_ACTION_CODE........", "127"}
};



void DumpSidStruct(TSSidInfo *pkSidInfo)
{
	char            sub_data[LG_MAX];
	char            sWork[BUF_LG_MAX];
	char            buffer[LG_MAX];
	char            sLine[MAX_LINE_TRC];
	char            TI[2];
	int             nLength;
	int             iMapCtr, i, j, k;
	int             nPos = 0;
	TSTlvPrivate    tlvInfo;
	
	char CardTrunc[22 + 1];
	
	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sWork, 0, sizeof(sWork));
	


	trace_event("Start DumpSid()", PROCESSING);


	iMapCtr = 1;
	while ((pkSidInfo->sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 3))
		++iMapCtr;

	memcpy(sLine, "- BIT MAP  : ", 13);
	nPos += 13;
	for (i = 0; i < iMapCtr * 8; i++) 
	{
		sprintf(sLine + nPos, " %02X", (unsigned char)pkSidInfo->sBitMap[i]);
		nPos += 3;
	}
	trace_event(sLine, TRACE);

	trace_event("------------------------------------", TRACE);
	sprintf(sLine, "- M.T.I      : %04d", pkSidInfo->nMsgType);
	trace_event(sLine, TRACE);
	trace_event("------------------------------------", TRACE);
	trace_event("- FLD (FIELD): LENGTH :  DATA       ", TRACE);
	trace_event("------------------------------------", TRACE);


	for (i = 1; i < 128; i++) 
	{
		if (GetSidField(i, pkSidInfo, sWork, &nLength) != SUCCES)
		{
			continue;
		}
		switch (i + 1) 
		{
		case 2: 
			if (debug_option == 1)
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength, sWork );
			else        
			{
				TrunCard(sWork, CardTrunc);
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s]" , i+1 , nLength, CardTrunc);
			}			
			trace_event(sLine,TRACE);
			break;
		case 48 :
			sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
			trace_event(sLine,TRACE);

			InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate( sWork,nLength,&tlvInfo);
			for (j=0; j<(sizeof(sPrivateDataDesc)/sizeof(sPrivateDataDesc[0])); j++)
			{
				if ( GetTlvPrivate(sPrivateDataDesc[j].value,&tlvInfo,sub_data,&nLength) != SUCCES )
				{
					continue;
				}
				sub_data[nLength] = 0;

				if( memcmp(sPrivateDataDesc[j].value, "P10", 3) == 0 ||
					memcmp(sPrivateDataDesc[j].value, "P15", 3) == 0 ||
					memcmp(sPrivateDataDesc[j].value, "P92", 3) == 0 

					)

				{
					if (debug_option == 1)
						sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
						sPrivateDataDesc[j].value,
						sPrivateDataDesc[j].description,
						sub_data );
					else
						sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) : [****************]",
						sPrivateDataDesc[j].value,
						sPrivateDataDesc[j].description
						);
					trace_event(sLine,TRACE);
				}
				else
				{
					sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
						sPrivateDataDesc[j].value,
						sPrivateDataDesc[j].description,
						sub_data );
					trace_event(sLine,TRACE);
				}
			}

			break;
		case 35: 
		case 45:
		case 52:
			if (debug_option == 1)
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength, sWork );
			else
				sprintf(sLine,"- FLD (%03d) : (%03d) : [*******************] " , i+1 , nLength );
			trace_event(sLine,TRACE);
			break;
		case 53 :
			sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
			trace_event(sLine,TRACE);
			for ( j=0; j< SECURITY_DATA_QTY ; j ++ )
			{
				GetSecurityData (j,sWork,sub_data,&nLength);
				sprintf(sLine ,"       > %.25s ..............: [%s] " , sSecurityDesc [ j ] , sub_data    );
				trace_event(sLine,TRACE);
			}
			break;
		case 60:
			sprintf(sLine, "- FLD (%03d) : (%03d) : ", i + 1, nLength);
			trace_event(sLine, TRACE);

			InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate(sWork, nLength, &tlvInfo);
			for (j = 0; j < (sizeof(sPrivateAddPosDescsid) / sizeof(sPrivateAddPosDescsid[0])); j++) 
			{
				if (GetTlvPrivate(sPrivateAddPosDescsid[j].value, &tlvInfo, sub_data, &nLength) == SUCCES) 
				{
					sub_data[nLength] = 0;
					sprintf(sLine, "       >Tag: '%.3s' : (%25s) : [%s]",
						sPrivateAddPosDescsid[j].value,
						sPrivateAddPosDescsid[j].description,
						sub_data);
					trace_event(sLine, TRACE);
				}
			}

			break;

		case 61:
			sprintf(sLine, "- FLD (%03d) : (%03d) : ", i + 1, nLength);
			trace_event(sLine, TRACE);

			InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate(sWork, nLength, &tlvInfo);
			for (j = 0; j < (sizeof(sPrivateNetworkDescsid) / sizeof(sPrivateNetworkDescsid[0])); j++) {
				if (GetTlvPrivate(sPrivateNetworkDescsid[j].value, &tlvInfo, sub_data, &nLength) == SUCCES) {
					sub_data[nLength] = 0;
					sprintf(sLine, "       >Tag: '%.3s' : (%25s) : [%s]",
						sPrivateNetworkDescsid[j].value,
						sPrivateNetworkDescsid[j].description,
						sub_data);
					trace_event(sLine, TRACE);
				}
			}

			break;

		case 62:

			sprintf(sLine, "- FLD (%03d) : (%03d) : ", i + 1, nLength);
			trace_event(sLine, TRACE);

			InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate(sWork, nLength, &tlvInfo);
			for (j = 0; j < (sizeof(sPrivateCpsDescsid) / sizeof(sPrivateCpsDescsid[0])); j++) {
				if (GetTlvPrivate(sPrivateCpsDescsid[j].value, &tlvInfo, sub_data, &nLength) == SUCCES) {
					sub_data[nLength] = 0;
					sprintf(sLine, "       >Tag: '%.3s' : (%25s) : [%s]",
						sPrivateCpsDescsid[j].value,
						sPrivateCpsDescsid[j].description,
						sub_data);
					trace_event(sLine, TRACE);
				}
			}
			break;

		default:
			sprintf(sLine, "- FLD (%03d) : (%03d) : [%s] ", i + 1, nLength, sWork);
			trace_event(sLine, TRACE);

			break;
		}
	}

	trace_event("------------------------------------", TRACE);
	trace_event("End   DumpSid()", PROCESSING);
}


void dump_sid_buffer(char *data)
{
	TSSidInfo       msgInfo;

	InitSidInfo(&msgInfo);

	AnalyseSid(data, &msgInfo);

	DumpSidStruct(&msgInfo);
}



void  SidDumpIccFields(char *sIccData, int nIccLen)
{

	TSTagSid        lTagSid;
	typedef struct {
		char           *sTagDesc;
		char           *sTagValue;
	}               SIDTAG;

	SIDTAG          aIccSidTag[] =
	{
		{"SID_TAG_ISS_SCRIPT1         ", "7100"},
		{"SID_TAG_ISS_SCRIPT2         ", "7200"},
		{"SID_TAG_ISS_SCRIPT_CMD      ", "8600"},
		{"SID_TAG_APP_INTER_PROFILE   ", "8200"},
		{"SID_TAG_DED_FILE_NAME       ", "8400"},
		{"SID_TAG_ICC_APP_ID          ", "4F00"},
		{"SID_TAG_AUTHOR_RESP_CODE    ", "8A00"},
		{"SID_TAG_ISS_AUTHEN_DATA     ", "9100"},
		{"SID_TAG_TVR                 ", "9500"},
		{"SID_TAG_TRANS_DATE          ", "9A00"},
		{"SID_TAG_TRANS_TYPE          ", "9C00"},
		{"SID_TAG_TRANS_CUR_CODE      ", "5F2A"},
		{"SID_TAG_TRANS_AMOUNT        ", "9F02"},
		{"SID_TAG_OTHER_AMOUNT        ", "9F03"},
		{"SID_TAG_TERM_APP_VER_NUM    ", "9F09"},
		{"SID_TAG_ISS_APP_DATA        ", "9F10"},
		{"SID_TAG_TERM_COUNTRY_CODE   ", "9F1A"},
		{"SID_TAG_IFD_SERIAL_NUM      ", "9F1E"},
		{"SID_TAG_APP_CRYPTOGRAM      ", "9F26"},
		{"SID_TAG_CRYPTO_INFO_DATA    ", "9F27"},
		{"SID_TAG_TERM_CAP            ", "9F33"},
		{"SID_TAG_CVM                 ", "9F34"},
		{"SID_TAG_TERM_TYPE           ", "9F35"},
		{"SID_TAG_ATC                 ", "9F36"},
		{"SID_TAG_UNPRED_NUMBER       ", "9F37"},
		{"SID_TAG_TRANS_SEQ_NUM       ", "9F41"},
		{"SID_TAG_TRANS_CATEG_CODE    ", "9F53"},
		{"SID_TAG_APP_SEQ_NUMBER      ", "5F34"},
		{"SID_TAG_ARPC                ", "9F54"},
	};

	int             i = 0;
	int             nLen = 0;
	char            sWork[LG_MAX];
	char            sLine[MAX_LINE_TRC];

	trace_event("Start SidDumpIccFields()", PROCESSING);

	memset(sWork, 0, sizeof(sWork));

	InitSidIcTag(&lTagSid);



	AnalyseSidIc(sIccData, nIccLen, &lTagSid);



	trace_event("         Field                        |  Len  |              Value", TRACE);

	for (i = 0; i < (sizeof(aIccSidTag) / sizeof(aIccSidTag[0])); i++) {
		memset(sWork, 0, sizeof(sWork));

		if (GetSidIcTag(aIccSidTag[i].sTagValue, &lTagSid, sWork, &nLen) == SUCCESS)
		{

			if (debug_option != 1)
			{
				if (   memcmp(aIccSidTag[i].sTagValue, "5F34", 4) == 0 || 
					memcmp(aIccSidTag[i].sTagValue, "9F27", 4) == 0 || 
					memcmp(aIccSidTag[i].sTagValue, "5A", 2) == 0 ||
					memcmp(aIccSidTag[i].sTagValue, "57", 2) == 0)
					sprintf(sLine, " >> %s|%s : [%03d] : [********************]", aIccSidTag[i].sTagValue, aIccSidTag[i].sTagDesc, nLen);
				else
					sprintf(sLine, " >> %s|%s : [%03d] : [%s]", aIccSidTag[i].sTagValue, aIccSidTag[i].sTagDesc, nLen, sWork);
			}
			else
				sprintf(sLine, " >> %s|%s : [%03d] : [%s]", aIccSidTag[i].sTagValue, aIccSidTag[i].sTagDesc, nLen, sWork);

			trace_event(sLine, TRACE);
		}
	}



	trace_event("End   SidDumpIccFields()", PROCESSING);
}
