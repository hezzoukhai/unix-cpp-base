/*******************************************************************************/
/* (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/* */
/* */
/* Description                                                                 */
/* */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
0001MK     		18-02-2002   		Mahjoub KARROUM    		Initial Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <header_sms.h>
#include <define.h>

#include <iso_sms.h>
#include <iso_cps.h>
#include <iso_puf.h>
#include <sms_puf.h>
#include <sms_param.h>
#include <sms_define.h>

#include <iso8583.h>

#define SUCCES  0
#define ECHEC  -1

typedef struct{
	char *sTagDesc;
	char *sTagValue;
} SMSTAG;

static SMSTAG aIccSmsTag [] =
{
	{"VISA_TAG_ISS_SCRIPT1         ","7100"},
	{"VISA_TAG_ISS_SCRIPT2         ","7200"},
	{"VISA_TAG_APP_INTER_PROFILE   ","8200"},
	{"VISA_TAG_DED_FILE_NAME       ","8400"},
	{"VISA_TAG_ISS_AUTHEN_DATA     ","9100"},
	{"VISA_TAG_TVR                 ","9500"},
	{"VISA_TAG_TRANS_DATE          ","9A00"},
	{"VISA_TAG_TRANS_TYPE          ","9C00"},
	{"VISA_TAG_TRANS_CUR_CODE      ","5F2A"},
	{"VISA_TAG_TRANS_AMOUNT        ","9F02"},
	{"VISA_TAG_OTHER_AMOUNT        ","9F03"},
	{"VISA_TAG_TERM_APP_VER_NUM    ","9F09"},
	{"VISA_TAG_ISS_APP_DATA        ","9F10"},
	{"VISA_TAG_TERM_COUNTRY_CODE   ","9F1A"},
	{"VISA_TAG_IFD_SERIAL_NUM      ","9F1E"},
	{"VISA_TAG_APP_CRYPTOGRAM      ","9F26"},
	{"VISA_TAG_CRYPTO_INFO_DATA    ","9F27"},
	{"VISA_TAG_TERM_CAP            ","9F33"},
	{"VISA_TAG_CVM                 ","9F34"},
	{"VISA_TAG_TERM_TYPE           ","9F35"},
	{"VISA_TAG_ATC                 ","9F36"},
	{"VISA_TAG_UNPRED_NUMBER       ","9F37"},
	{"VISA_TAG_TRANS_SEQ_NUM       ","9F41"},
	{"VISA_TAG_TRANS_CATEG_CODE    ","9F53"},
};


/* Start AMER20190225 PROD00065991: VISA Enh19.Q2 Art2.4*/
static void DumpSmsTransSpecData(char* sTransSpecData, int nTransSpecLength)
{
	char			sLine[MAX_LINE_TRC];
	char			sData[1024];
	int				nLength;
	TlvInfo			kSmsTransSpecTlv;
	TlvInfo			kSmsAppIdentTlv;
	TlvInfo			kSmsSenderDataTlv;
	/*TlvInfo			kSmsAddSenderDataTlv;*/
	TlvInfo			kSmsPayFacTlv;
	TlvInfo			kSmsRecurringPayTlv;	/*ZKO 25072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	InitSmsTransSpecDataTlv(&kSmsTransSpecTlv);
	InitSmsAppIdentDataTlv(&kSmsAppIdentTlv);
	InitSmsSenderDataTlv(&kSmsSenderDataTlv);
	/*InitSmsAddSenderDataTlv(&kSmsAddSenderDataTlv);*/
	InitSmsPayFacDataTlv(&kSmsPayFacTlv);
	InitSmsRecurringPayDataTlv(&kSmsRecurringPayTlv);	/*ZKO 25072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

	
	AnalyseTlvInfo(sTransSpecData, nTransSpecLength, &kSmsTransSpecTlv);

	if( GetTlvInfoField (SMS_F104_PAY_FAC_DATA, &kSmsTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kSmsPayFacTlv);

		sprintf(sLine,"- DS: [%s] ",SMS_F104_PAY_FAC_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kSmsPayFacTlv);
	}

	if( GetTlvInfoField (SMS_F104_BUSINESS_APP_ID, &kSmsTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kSmsAppIdentTlv);

		sprintf(sLine,"- DS: [%s] ",SMS_F104_BUSINESS_APP_ID);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kSmsAppIdentTlv);
	}
	if( GetTlvInfoField (SMS_F104_SENDER_DATA, &kSmsTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kSmsSenderDataTlv);

		sprintf(sLine,"- DS: [%s] ",SMS_F104_SENDER_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kSmsSenderDataTlv);
	}
	
	/*Start ZKO 25072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
	if( GetTlvInfoField (SMS_F104_RECURRING_PAY, &kSmsTransSpecTlv, sData, &nLength) == SUCCESS )
	{
		AnalyseTlvInfo(sData, nLength, &kSmsRecurringPayTlv);

		sprintf(sLine,"- DS: [%s] ",SMS_F104_RECURRING_PAY);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kSmsRecurringPayTlv);
	}
	/*End ZKO 25072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/

}
/* End AMER20190225 PROD00065991: VISA Enh19.Q2 Art2.4*/

/* Start IBO20190726 PROD00070403 Enh19.Q4 9.1.3 & 9.1.4 */
static void DumpSmsElectComData(char* sElectComData, int nElectComLength)
{
	char			sLine[MAX_LINE_TRC];
	char			sData[1024];
	int				nLength;
	TlvInfo			kSmsElectComTlv;   			
	TlvInfo			kSmsTlvInfo;		
	BerInfo			kSmsBerInfo;	/*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/	

	
	InitSmsElectComDataTlv(&kSmsElectComTlv);			


	AnalyseTlvInfo(sElectComData, nElectComLength, &kSmsElectComTlv);
	
	if( GetTlvInfoField (SMS_F34_EU_PSD2_SCA_CSC, &kSmsElectComTlv, sData, &nLength) == SUCCESS )
	{
		InitSmsEuPsd2ScaCsc(&kSmsTlvInfo);
		AnalyseTlvInfo(sData, nLength, &kSmsTlvInfo); 

		sprintf(sLine,"- DS: [%s] ",SMS_F34_EU_PSD2_SCA_CSC);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kSmsTlvInfo);
	}
		
	/*Start HAL 20202003 ENHS2020Q2 ART 9.3.4*/
	if( GetTlvInfoField (SMS_F34_SUPP_DATA, &kSmsElectComTlv, sData, &nLength) == SUCCESS )/*NAB PLUTONL-9857-Enh 2025Q2 VISA 3.8*/
	{

		InitSmsSupplementalData(&kSmsBerInfo); 
		AnalyseBerInfo(sData, nLength, &kSmsBerInfo); 

		sprintf(sLine,"- DS: [%s] ",SMS_F34_SUPP_DATA);
		trace_event(sLine,TRACE);

		DumpBerInfo(&kSmsBerInfo);
	}
	/*End HAL 20202003 ENHS2020Q2 ART 9.3.4*/
		
	if( GetTlvInfoField (SMS_F34_ACCEPTANCE_ENV_ADD_DATA, &kSmsElectComTlv, sData, &nLength) == SUCCESS )
	{
		InitSmsAccEnvAddDataTlv(&kSmsTlvInfo);	
		AnalyseTlvInfo(sData, nLength, &kSmsTlvInfo); 

		sprintf(sLine,"- DS: [%s] ",SMS_F34_ACCEPTANCE_ENV_ADD_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kSmsTlvInfo);
	}

	/*Start PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/
	if( GetTlvInfoField (SMS_F34_AUTHEN_DATA, &kSmsElectComTlv, sData, &nLength) == SUCCESS )
	{
		InitSmsEcomAddDataTlv(&kSmsTlvInfo);
		AnalyseTlvInfo(sData, nLength, &kSmsTlvInfo); 
		
		sprintf(sLine,"- DS: [%s] ", SMS_F34_AUTHEN_DATA);
		trace_event(sLine,TRACE);

		DumpTlvInfo(&kSmsTlvInfo);
	}
	/*End PLUTONL-8809 AMANS Enh2024Q4 VISA 3.9*/
	
}
/* End IBO20190726 PROD00070403 Enh19.Q4 9.1.3 & 9.1.4 */

static void GetRejectCode(TSSmsHeader * reject_sms,char           *RejectCode)
{
	RejectCode[0] = ((reject_sms->abRejectCode[0] >> 4) & 0x0F) + 0x30;
	RejectCode[1] = (reject_sms->abRejectCode[0] & 0x0F) + 0x30;
	RejectCode[2] = ((reject_sms->abRejectCode[1] >> 4) & 0x0F) + 0x30;
	RejectCode[3] = (reject_sms->abRejectCode[1] & 0x0F) + 0x30;
}

void DumpSmsData(char *data)
{
	char            sPufData[LG_MAX];
	char            sCpsData[LG_MAX];
	char            sWork[LG_MAX];
	char            sLine[MAX_LINE_TRC];
	char            TI[2];
	int             nLength;
	int             iMapCtr, i, j, k, nPos = 0;
	TSSmsInfo       msgInfo;
	TSCpsInfo       cpsInfo;
	TSPufInfo       pufInfo;
	TSSmsPufInfo    SmspufInfo;
	TlvInfo         kTlvInfo;
	int 			nFldType;
	/* Mehdi Elyajizi PCI */
	char            CardTrunc[22 + 1];
	memset(CardTrunc, 0, sizeof(CardTrunc));
	/* Mehdi Elyajizi PCI */
	InitPufInfo(&pufInfo);
	InitSmsPufInfo(&SmspufInfo);
	InitSmsInfo(&msgInfo);
	InitCpsInfo(&cpsInfo);


	AnalyseSms(data, &msgInfo);
	nPos = 0;

	iMapCtr = 1;
	while ((msgInfo.sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 3))
		++iMapCtr;

	memcpy(sLine, "- BIT MAP  : ", 13);
	nPos += 13;
	for (i = 0; i < iMapCtr * 8; i++) {
		sprintf(sLine + nPos, " %02X",(unsigned char) msgInfo.sBitMap[i]);/*NAB11032020*/
		nPos += 3;
	}
	trace_event(sLine, TRACE);


	sprintf(sLine, "-----------------------------------");
	trace_event(sLine, TRACE);
	sprintf(sLine, "- M.T.I      : [%04d]", msgInfo.nMsgType);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- FLD (FIELD): LENGTH : DATA        ");
	trace_event(sLine, TRACE);
	sprintf(sLine, "-----------------------------------");
	trace_event(sLine, TRACE);

	for (i = 1; i <= iMapCtr * 64; i++)
	{

		memset(sWork, 0, sizeof(sWork));
		/*Begin MKB060309 PCI Norme Integration */
		switch (i)
		{
		case SMS_CARD_NBR:
			if (GetSmsField(i, &msgInfo, sWork, &nLength) == SUCCES)
			{
				if (debug_option == 1)
					sprintf(sLine, "- FLD (%03d)   (%03d)    [%s]", i + 1, nLength, sWork);
				else
				{
					TrunCard(sWork, CardTrunc);
					sprintf(sLine, "- FLD (%03d)   (%03d)    [%s]", i + 1, nLength, CardTrunc);
				}
				trace_event(sLine, TRACE);
			}
			break;
		case 33:	/*IBO20190724 PROD00070403 Enh19.Q4 Art 9.1.3*/
			if ( GetSmsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d)   (%03d)   " , i+1, nLength);
				trace_event(sLine,TRACE);
				DumpSmsElectComData(sWork,nLength);
			}
			break;

		case SMS_T1_DATA:
		case SMS_T2_DATA:
		case SMS_PIN_DATA:
			if (GetSmsField(i, &msgInfo, sWork, &nLength) == SUCCES)
			{
				if (debug_option == 1)
					sprintf(sLine, "- FLD (%03d)   (%03d)    [%s]", i + 1, nLength, sWork);
				else
					sprintf(sLine, "- FLD (%03d)   (%03d)    [**********************]", i + 1, nLength);
				trace_event(sLine, TRACE);
			}
			break;
			/*Begin MKB060309 PCI Norme Integration */
		case 54 :
			if ( GetSmsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				char sTag [ 4 + 1], sValue[1024];
				unsigned char leftTag,rightTag;
				int  nLen;



				sprintf(sLine,"- FLD (%03d)   (%03d)" , i+1, nLength );
				trace_event(sLine,TRACE);
				for( j = 3 ; j < nLength; )
				{
					memset(sTag, 0, sizeof(sTag));
					memset(sValue, 0, sizeof(sValue));

					if (((unsigned char)sWork[j] & 0x1F) == 0x1F)
					{
						/*sprintf(sTag, "%02X%02X", (unsigned char)sWork[j++], (unsigned char)sWork[j++]);*/
						leftTag=(unsigned char)sWork[j++];
						rightTag=(unsigned char)sWork[j++];
						sprintf(sTag, "%02X%02X", leftTag, rightTag);
					}
					else
						sprintf(sTag, "%02X  ", (unsigned char)sWork[j++]);

					nLen = (unsigned char)sWork[j++];

					HexToAscii(sWork+j,sValue,nLen*2);

					if (debug_option != 1)
					{
						if (   	memcmp ( sTag, "5F34", 4) == 0 ||
							memcmp (sTag, "9F27", 4) == 0 ||
							memcmp (sTag, "5A", 2) == 0 ||
							memcmp (sTag, "57", 2) == 0 ||
							memcmp (sTag, "C0", 2) == 0)	/*AMER20180405 PROD00055121: Enh 18.Q2 Sms Art 8.1.1*/
							sprintf(sLine,"      > TAG (%.4s) ....: [***** Len %lu *****]" , sTag , strlen( sValue )   );
						else
							sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );
					}
					else
						sprintf(sLine,"      > TAG (%.4s) ....: [%s]" , sTag , sValue    );


					trace_event(sLine,TRACE);

					j += nLen;
				}
				memset(sValue, 0, sizeof(sValue));
			}
			break;
		/*Start PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2*/
		case SMS_I_ORIG_DATA:	
			if ( GetSmsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d)   (%03d)   " , i+1, nLength);
				trace_event(sLine,TRACE);	
				
				InitSmsCustmRelatdDataTlv(&kTlvInfo);					
				AnalyseTlvInfo(sWork, nLength, &kTlvInfo);

				if( GetTlvInfoField (SMS_F056_ACCOUNT_OWNER_DATA, &kTlvInfo, sWork, &nLength) == SUCCESS )
				{
					InitSmsAccOwnerDataTlv(&kTlvInfo); 
					AnalyseTlvInfo(sWork, nLength, &kTlvInfo); 

					sprintf(sLine,"- DS: [%s] ",SMS_F056_ACCOUNT_OWNER_DATA);
					trace_event(sLine,TRACE);

					DumpTlvInfo(&kTlvInfo);
				} 
			}
			break;
		/*End PLUTONL-8807 AMANS Enh2024Q4 VISA 3.2*/
		case 61:
			if (GetSmsField(i, &msgInfo, sWork, &nLength) == SUCCES) {
				sprintf(sLine, "- FLD (%03d) (%03d)", i + 1, nLength);
				trace_event(sLine, TRACE);
				AnalyseCps(sWork, &cpsInfo);
				memset(sCpsData, 0, sizeof(sCpsData));
				for (j = 0; j < 25; j++) /*  HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.1 18->25*/
					if (GetCpsSubField(j, &cpsInfo, sCpsData, &nLength) == SUCCES)
					{
						sprintf(sLine, "      > SUBFIELD (%02d) ....: [%s]"
							,j + 1, sCpsData);
						trace_event(sLine, TRACE);
					}
			}
			break;

		case 62:
			if (GetSmsField(i, &msgInfo, sWork, &nLength) == SUCCES) {
				sprintf(sLine, "- FLD (%03d) (%03d)", i + 1, nLength);
				trace_event(sLine, TRACE);
				AnalysePuf(sWork, &pufInfo);
				memset(sPufData, 0, sizeof(sPufData));
				/*for( j = 0 ; j < 18 ; j++) ENH SWI200904*/
				for( j = 0 ; j < 19 ; j++)
					if (GetPufSubField(j, &pufInfo, sPufData, &nLength) == SUCCES)
					{
						sprintf(sLine, "      > SUBFIELD (%02d) ....: [%s]"
							,j + 1, sPufData);
						trace_event(sLine, TRACE);
					}
			}
			break;
		case 103:	/*AMER20190225 PROD00065991: VISA Enh19.Q2 Art2.4*/
			if ( GetSmsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d)   (%03d)   " , i+1, nLength);
				trace_event(sLine,TRACE);
				DumpSmsTransSpecData(sWork,nLength);
			}
			break;

		case 109: /* FZL20220906 PLUTONL-4979	*/
			if ( GetSmsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d)   (%03d) " , i+1, nLength);
				trace_event(sLine,TRACE);
				InitSmsAdditionalDataTlv(&kTlvInfo);
				AnalyseTlvInfo(sWork,nLength,&kTlvInfo);
				if( GetTlvInfoField (SMS_F110T04_KEY_DATA , &kTlvInfo, sWork, &nLength) == SUCCESS )
				{
					InitSmsKeyDataTlv(&kTlvInfo);
					AnalyseTlvInfo(sWork, nLength, &kTlvInfo);
					sprintf(sLine,"- DS: [%s]", SMS_F110T04_KEY_DATA);
					trace_event(sLine,TRACE);
					DumpTlvInfo(&kTlvInfo);
				};
			}
			break;
		case 125:
			if (GetSmsField(i, &msgInfo, sWork, &nLength) == SUCCES)
			{
				sprintf(sLine, "- FLD (%03d) (%03d)", i + 1, nLength);
				trace_event(sLine, TRACE);
				AnalyseSmsPuf(sWork, &SmspufInfo);
				memset(sPufData, 0, sizeof(sPufData));
				for (j = 0; j < 13; j++)
				{
					if (GetSmsPufSubField(j, &SmspufInfo, sPufData, &nLength) == SUCCES)
					{
						sprintf(sLine, "      > SUBFIELD (%02d) ....: [%d|%s]"
							,j + 1, nLength, sPufData);
						trace_event(sLine, TRACE);
						memset(sPufData, 0, sizeof(sPufData));
						/*
						* nPos = 31 +
						* strlen(sPufData);
						*
						* for(k=0; k<nLength; k++) {
						* sprintf(sLine + nPos
						* ,"%02X|" , sPufData[k] );
						* nPos += 3; } sprintf(sLine
						* + nPos,"]" );
						*/

					}
				}
			}
			break;
		default:
		nFldType = GetSmsFieldType(i);

			if ( GetSmsField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if( nFldType != SMS_FIX_BIN &&
					nFldType != SMS_VAR_BIN )
				{
					sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sWork );
					trace_event(sLine,TRACE);
				}
				else
				{
					nLength = HexToAscii(sWork, sPufData, nLength*2);
					sprintf(sLine,"- FLD (%03d)   (%03d)    [%s]" , i+1, nLength,sPufData);
					trace_event(sLine,TRACE);
				}
			}

			break;
		}
	}

	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sPufData, 0, sizeof(sPufData));
	memset(sCpsData, 0, sizeof(sCpsData));
	memset(sWork, 0, sizeof(sWork));

	InitPufInfo(&pufInfo);
	InitSmsPufInfo(&SmspufInfo);
	InitSmsInfo(&msgInfo);
	InitCpsInfo(&cpsInfo);
}

void DumpSms(char           *data)
{
	char           *texte;
	char            RejectCode[5];
	TSSmsHeader    *header;
	TSSmsHeader    *reject_sms;
	char            sLine[MAX_LINE_TRC];
	trace_event("Start DumpSms()", PROCESSING);

	header = (TSSmsHeader *) data;
	sprintf(sLine, "-----------------------------------");
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(Header field) : CONTENT");
	trace_event(sLine, TRACE);
	sprintf(sLine, "-----------------------------------");
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(01) Header Length.........: %02X", header->bHeaderLength);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(02) Header flag & format..: %02X", header->bHeaderFlag);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(03) Text Format...........: %02X", header->bTextFormat);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(04) Total Message Length..: %02X %02X", header->abMsgLength[0], header->abMsgLength[1]);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(05) Dest Station Id.......: %02X %02X %02X", header->abDestStationId[0],
		header->abDestStationId[1], header->abDestStationId[2]);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(06) Src  Station Id.......: %02X %02X %02X", header->abSrcStationId[0],
		header->abSrcStationId[1], header->abSrcStationId[2]);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(07) Round-Trip Ctrl Info..: %02X", header->bEchoInfo);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(08) Base I Flags..........: %02X %02X", header->abBaseIIFlags[0], header->abBaseIIFlags[1]);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(09) Message Status Flags..: %02X %02X %02X", header->abMsgFlags[0],
		header->abMsgFlags[1], header->abMsgFlags[2]);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(10) Batch Number..........: %02X", header->bBatchNumber);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(11) Reserved..............: %02X %02X %02X", header->abReserved[0],
		header->abReserved[1], header->abReserved[2]);
	trace_event(sLine, TRACE);
	sprintf(sLine, "- H(12) User Information......: %02X ", header->bUserInfo);
	trace_event(sLine, TRACE);

	if (header->bHeaderLength >= 26) {
		reject_sms = (TSSmsHeader *) data;
		sprintf(sLine, "               MESSAGE WAS REJECTED");
		trace_event(sLine, TRACE);
		sprintf(sLine, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		trace_event(sLine, TRACE);
		GetRejectCode(reject_sms, RejectCode);
		sprintf(sLine, "------> Reject Code :    %.4s                ", RejectCode);
		trace_event(sLine, TRACE);
		sprintf(sLine, "-++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		trace_event(sLine, TRACE);

		if (reject_sms->abBitMap[0] & 0x80) {
			texte = data + reject_sms->bHeaderLength;
			header = (TSSmsHeader *) texte;
			DumpSmsData(data);
		}
	}
	else
		DumpSmsData(data);

	trace_event("End   DumpSms()", PROCESSING);
}

void DumpSmsStruct(TSSmsInfo * SmsInfo)
{
	char            sWork[LG_MAX];
	int             Length;

	trace_event("Start DumpSmsStruct()", TRACE);
	memset(sWork, 0, sizeof(sWork));

	Length = SmsBuildMsg(sWork + SMS_HDR_LEN, SmsInfo);
	SmsBuildHeader(RESPONSE, &(SmsInfo->sHeader), NULL, Length, StationId);
	memcpy(sWork, (char *) &(SmsInfo->sHeader), SMS_HDR_LEN);

	DumpSmsData(sWork);
	trace_event("End   DumpSmsStruct()", TRACE);

}

void SmsDumpIccFields (char *sIccData, int nIccLen)
{
	TSTagSms       lTagSms;


	int  i = 0;
	int  nLen = 0;
	char sWork[LG_MAX];
	char sLine[MAX_LINE_TRC];

	trace_event("Start SmsDumpIccFields()",PROCESSING);

	memset(sWork, 0, sizeof(sWork));
	memset(sLine, 0, sizeof(sLine));


	InitSmsIcTag (&lTagSms);


	AnalyseSmsIc (sIccData, nIccLen, &lTagSms);

	trace_event("         Field                         | Len  |              Value",TRACE);

	for (i=0; i<(sizeof(aIccSmsTag)/sizeof(aIccSmsTag[0])); i++)
	{
		memset(sWork, 0, sizeof(sWork));


		if ( GetSmsIcTag (aIccSmsTag[i].sTagValue , &lTagSms,sWork,&nLen) == SUCCESS)
		{

			if (debug_option != 1)
			{
				if (   memcmp(aIccSmsTag[i].sTagValue, "5F34", 4) == 0 ||
					memcmp(aIccSmsTag[i].sTagValue, "9F27", 4) == 0 ||
					memcmp(aIccSmsTag[i].sTagValue, "5A", 2) == 0 ||
					memcmp(aIccSmsTag[i].sTagValue, "57", 2) == 0)
					sprintf(sLine, " >> %s|%s : [%03d] : [********************]", aIccSmsTag[i].sTagValue, aIccSmsTag[i].sTagDesc, nLen);
				else
					sprintf(sLine, " >> %s|%s : [%03d] : [%s]", aIccSmsTag[i].sTagValue, aIccSmsTag[i].sTagDesc, nLen, sWork);
			}
			else
				sprintf(sLine, " >> %s|%s : [%03d] : [%s]", aIccSmsTag[i].sTagValue, aIccSmsTag[i].sTagDesc, nLen, sWork);
			trace_event(sLine,TRACE);
		}
	}


	trace_event("End SmsDumpIccFields()",PROCESSING);
}