/****************************************************************************/
/***************************************************************************/
/* UTILS_SID.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/* MESSAGE  SID 8583  EURO    (*F.M*)            */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <header_sms.h>
#include <define.h>

#include <iso_sid.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <security_data.h>
#include <iso8583.h>/*HAL PROD00073823 20201102 Enh Q2.2020 Art 2941*/

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
	{"MERCHANT_ADD_DATA.......", "P21"},
	{"E_COMMERCE_CERTIFICATE.", "P40"},
	{"SECURITY_LEVEL_IND.....", "P41"},
	{"EXTERNAL_CRYPT_RESULTS.", "P44"},
	{"TCC....................", "P48"},
	{"DCC....................", "P56"},

	{"RPI....................","P71" }, /* SLA 080212 Recurring Payment */
	{"ADDRESS_VERIF_REQ_TYPE.", "P82"},
	{"ADDRESS_VERIF_REP_TYPE.", "P83"},
	{"EXTERNAL_RESP_SOURCE    ", "P85"},
	{"EXTERNAL_CVV_RESULT_CODE", "P87"},
	{"EXTERNAL_CVV2_RESULT_CODE", "P88"},
	{"CVC1_TRACK_EDIT_ERROR   ", "P89"},
	{"CVC2_DATA	           ", "P92"},
	{"HSM_RESULT_CODE         ", "P93"},
	{"SECONDARY_RESOURCE      ", "P94"},
	{"MALFUNCTION_STIP        ", "P96"},
	{"ACQUIRER GENRE          ", "P97"},
	{"ATM PURCHASE ACRO        ","P20"},
	{"CTLS DEVICE TYPE        ", "R01"},
	{"TPK/AWK                 ", "P10"},
	{"SMS_CHG_DATA            ", "K01"},
	{"SMS_VCR_DATA            ", "K04"},/*ICH17012018*/
	{"DIGITAL_PAYMENT_DATA    ", "P58"},/*AMER20191023 PROD00072515*/
	{"MONEY_TRANSFER_DATA.....", "S20"},  		/*HAL PLUTONL-4154 4.2.4	AN 4972 */
	{"ECOM_AUTHENTIF_DATA.....", "S26"}, /*HAL PROD00073823 20200402 Enh Q2.2020 Art 2941*/
	{"PSD2_DATA...............", "S27"}, /* HAL  20201803 Enh Q2.2020 Art 9.3.3*/ 
	{"NET_VISA_MERC_ID........", "N21"}, /* HAL  20201803 Enh Q2.2020 Art 9.3.3*/ 
	{"NET_VISA_EXT_STIP_REASON_CODE", "N22"},	/* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/  
	{"SEC_SERV_ADD_DATA       ", "N54"},/*AMER20191023 PROD00072515*/
	{"NET_MCI_FRAUD_SCORE.....", "N55"},   /*AMER20210416 PLUTONL-2985: MC Enh19.Q2 AN2122*/
	{"MC_TRACE_ID.............", "N57"},			/*HAL PLUTONL-4154 4.2.4	AN 4972 */
	{"NET_TOKEN_PROGAM........", "N58"}, /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	{"MCI_MIT_CIT_INDICATOR...", "N59"}, /*IZL 03062022 PLUTONL-4810 ENH 22Q2 MC AN5524*/	

	{"ON_BEHALF_SERV..........", "P51"},   /*HAL 20201902  Q2.2020  AN 2941*/
	/*Start AMER201211126 PLUTONL-3140*/
	{"TCC_AND_RFU.............", "P64"}, 	 	/*HAL PLUTONL-4154 4.2.4	AN 4972 */
	{"ECOM_AUTH_INFO..........", "P66"},  
	{"DS_TRX_ID...............", "K06"},   
	{"ACS_TRX_ID..............", "K17"} 
	/*End AMER201211126 PLUTONL-3140*/
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



void DumpHsidStruct(TSSidInfo *pkSidInfo)
{
	char            sub_data[LG_MAX];
	char            sWork[BUF_LG_MAX];
	char            buffer[LG_MAX];
	char            sLine[MAX_LINE_TRC];
	char            TI[2];
	int             nLength;
	int             iMapCtr, i, j, k;
	int             nPos = 0;
	/*TSTlvPrivate    tlvInfo;*/
	TlvInfo			tlvInfo; /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/	
	TlvInfo		   isoSaleInfoDataTlvECOM; /*HAL PROD00073823 20201102 Enh Q2.2020 Art 2941*/
    TlvInfo	       isoExempAuthInfoData;    /*HAL  20201803 Enh Q2.2020 Art 9.3.3*/
	char CardTrunc[22 + 1];

	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sWork, 0, sizeof(sWork));
	


	trace_event("Start DumpHsid()", PROCESSING);


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
		if (GetHsidField(i, pkSidInfo, sWork, &nLength) != SUCCES)
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

			/*InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate( sWork,nLength,&tlvInfo);*/
			/*Start IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
			InitHsidPrivateDataTlv(&tlvInfo);
			AnalyseTlvInfo(sWork,nLength,&tlvInfo);
			/*End IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
			for (j=0; j<(sizeof(sPrivateDataDesc)/sizeof(sPrivateDataDesc[0])); j++)
			{
				memset(sub_data, 0, sizeof(sub_data));  /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
				/*if ( GetTlvPrivate(sPrivateDataDesc[j].value,&tlvInfo,sub_data,&nLength) != SUCCES )*/
				if ( GetTlvInfoField(sPrivateDataDesc[j].value,&tlvInfo,sub_data,&nLength) != SUCCES ) /*IZL*/
				
				{
					continue;
				}
				sub_data[nLength] = 0;
				if( memcmp(sPrivateDataDesc[j].value, "P10", 3) == 0 ||
					memcmp(sPrivateDataDesc[j].value, "P15", 3) == 0 ||
					memcmp(sPrivateDataDesc[j].value, "P92", 3) == 0 )

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
				
				/*Start HAL PROD00073823 20201102 Enh Q2.2020 Art 2941*/ 
				else if( memcmp(sPrivateDataDesc[j].value, ECOM_AUTHENTIF_DATA, 3) == 0)
				{
					sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
					sPrivateDataDesc[j].value,
					sPrivateDataDesc[j].description);
					trace_event(sLine,TRACE);
					InitEcomAuthentificationInfoDataTlv(&isoSaleInfoDataTlvECOM);
					AnalyseTlvInfo(sub_data, nLength, &isoSaleInfoDataTlvECOM);
					DumpTlvInfo(&isoSaleInfoDataTlvECOM);
				}
				/*End HAL PROD00073823 20201102 Enh Q2.2020 Art 2941*/ 

                /*Start HAL  20201803 Enh Q2.2020 Art 9.3.3*/ 
						else if( memcmp(sPrivateDataDesc[j].value, PSD2_DATA, 3) == 0)
					{
						sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
						sPrivateDataDesc[j].value,
						sPrivateDataDesc[j].description);
						trace_event(sLine,TRACE);

						InitExempAuthInfoDataTlv(&isoExempAuthInfoData);
						AnalyseTlvInfo(sub_data, nLength, &isoExempAuthInfoData);

						DumpTlvInfo(&isoExempAuthInfoData);
					}
				/*End HAL  20201803 Enh Q2.2020 Art 9.3.3*/
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
		case 55 :
			if (GetHsidField(i, pkSidInfo, sWork, &nLength) != SUCCES)
			{
				char sTag [4 +1];
				char sValue[512];
				int  nLen;
				unsigned char leftTag,rightTag;

				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);
				for( j = 0 ; j < nLength; )
				{
					memset(sTag, 0, sizeof(sTag));
					memset(sValue, 0, sizeof(sValue));

					if (((unsigned char)sWork[j] & 0x1F) == 0x1F)
					{
						leftTag=(unsigned char)sWork[j++];
						rightTag=(unsigned char)sWork[j++];
						sprintf(sTag, "%02X%02X", leftTag, rightTag);
					}
					else
					{
						leftTag=(unsigned char)sWork[j++];
						sprintf(sTag, "%02X  ", leftTag);
					}

					nLen = sWork[j++];

					HexToAscii(sWork+j,sValue,nLen*2);
					if (debug_option != 1)
					{
						if ( memcmp(sTag, "5F34", 4) == 0 || memcmp(sTag, "57", 2) == 0 || memcmp(sTag, "5A", 2) == 0)
							sprintf(sLine,"      > TAG (%.4s) ....: [**** Len %lu ****]" , sTag  , strlen(sValue)   );
						else
							sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
					}
					else
						sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
					trace_event(sLine,TRACE);


					j += nLen;
				}
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
	trace_event("End   DumpHsid()", PROCESSING);
}


void dump_Hsid_buffer(char *data)
{
	TSSidInfo       msgInfo;

	InitHsidInfo(&msgInfo);

	AnalyseHsid(data, &msgInfo);

	DumpHsidStruct(&msgInfo);
	
	InitHsidInfo(&msgInfo);
}



void  HsidDumpIccFields(char *sIccData, int nIccLen)
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

	trace_event("Start HsidDumpIccFields()", PROCESSING);

	memset(sWork, 0, sizeof(sWork));

	InitHsidIcTag(&lTagSid);



	AnalyseHsidIc(sIccData, nIccLen, &lTagSid);



	trace_event("         Field                        |  Len  |              Value", TRACE);

	for (i = 0; i < (sizeof(aIccSidTag) / sizeof(aIccSidTag[0])); i++) {
		memset(sWork, 0, sizeof(sWork));

		if (GetHsidIcTag(aIccSidTag[i].sTagValue, &lTagSid, sWork, &nLen) == SUCCESS)
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



	trace_event("End   HsidDumpIccFields()", PROCESSING);
}
