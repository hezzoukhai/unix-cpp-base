/****************************************************************************/
/***************************************************************************/
/*            UTILS_SID.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  SID 8583  EURO    (*F.M*)            */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <header_gb.h>
#include <define.h>

#include <iso_gb.h>
#include <tlv_privfields.h>
#include <tlv_private.h>

#define SUCCES 0
#define ECHEC -1

typedef struct s_tlv_print
{
    char *description;
    char *value;
} t_tlv_print;

static char *sSecurityDesc[] = {
    {"SECURITY FORMAT         "},
    {"PIN BLOCK FORMAT        "},
    {"PIN ENCRYPTION KEY INDEX"},
    {"MAC KEY INDEX           "},
    {"SOURCE PID              "},
    {"SOURCE RESOURCE         "},
    {"DESTINATION PID         "},
    {"DESTINATION RESOURCE    "},
    {"USER                    "},
    {"PURGE TIME              "}};

static char *sDCCPrivateDesc[] = {
    {"                             "},
    {"MERCHANT ID                  "},
    {"TERMINAL ID                  "},
    {"ACQUERER ID                  "},
    {"                             "},
    {"FOREIGN CURRENCY INFORMATION "},
    {"                             "},
    {"                             "},
    {"EXCHANGE RATE INFORMATION    "},
    {"RATE REQUEST REFERENCE ID    "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"CARD TYPE                    "},
    {"UPDATE CALL DATE & TIME      "},
    {"                             "},
    {"INVERTED RATE INFORMATION    "},
    {"                             "},
    {"FOREIGN CURRENCY MERCHANT ID "},
    {"FOREIGN CURRENCY TERMINAL ID "},
    {"TILL ID                      "},
    {"                             "},
    {"FOREIGN CURRENCY ALPHABITIC  "},
    {"                             "},
    {"MARK UP PURCENTAGE INFO      "},
    {"COMMISSION PERCENTAGE INFO   "},
    {"COMMISSION VALUE INFO        "},
    {"FOREIGN CURRENCY NUM & RATE  "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "},
    {"                             "}};

static t_tlv_print sPrivateDataDescDCC[] = {
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
    {"ACRO_ATM_PURCHASE_TAG...", "P20"},
    {"MSG_REASON_CODE_TAG.....", "P31"},
    {"CODE_USAGE_TAG..........", "P32"},
    {"INDICATEUR_DOC_TAG......", "P33"},
    {"CHGBCK_REF_TAG..........", "P34"},
    {"SMS_MESSAGE_TEXT_TAG....", "P35"},
    {"NETWORK_ID_TAG.........", "P40"},
    {"SECURITY_LEVEL_IND.....", "P41"},
    {"EXTERNAL_CRYPT_RESULTS.", "P44"},
    {"TCC....................", "P48"},
    {"BILLING_OUTLET_TAG..",    "P50"},
    {"MC_NETWORK_DATA_TAG....", "P63"},
    {"ADDRESS_VERIF_REQ_TYPE.", "P82"},
    {"ADDRESS_VERIF_REP_TYPE.", "P83"},
    {"EXTERNAL_RESP_SOURCE    ", "P85"},
    {"EXTERNAL_CVV_RESULT_CODE", "P87"},
    {"EXTERNAL_CVV2_RESULT_CODE", "P88"},
    {"CVC1_TRACK_EDIT_ERROR   ", "P89"},
    {"CVC2_DATA	           ",    "P92"},
    {"HSM_RESULT_CODE	       ", "P93"},
    {"SECONDARY_RESOURCE      ", "P94"},
    {"NETWORK_CODE_TAG........", "P95"},
    {"MALFUNCTION_STIP        ", "P96"},
    {"ACQUIRER GENRE          ", "P97"},
    {"DCC_FOR_CURR............", "D01"},
    {"DCC_FOR_AMOUNT..........", "D02"},
    {"DCC_FOR_ALPHA_CODE......", "D03"},
    {"DCC_RATE_VALUE..........", "D04"},
    {"DCC_INVERT_RATE.........", "D05"},
    {"DCC_MARCKUP_VALUE.......", "D06"},
    {"DCC_ORIGINE_AMOUNT......", "D07"},
    {"DCC_ORIGINE_CURR	......", "D08"},
    {"CARD_TYPE_TAG...........", "D09"},
    {"DCC_MSG_ERROR...........", "D10"},
    {"DCC_TRX_FLAG	..........", "D11"},
    {"DCC_MSG_NUMBER_TAG......", "D12"},
    {"DCC_ORIGINAL_STAN.......", "D13"}
};

void DumpGB(char *data)
{
    char 		sub_data[BUF_LG_MAX];
    char 		sLine[MAX_LINE_TRC];
	char		sFieldData[BUF_LG_MAX];
    char sAscii[512];
    int nLength;
    int iMapCtr, i, j, k;
    int nPos = 0;
	char*		sPtr;
    TSGBInfo msgInfo;
    TSTlvPrivate tlvInfo;
	TSGBInfo 	 GB63Info;
	
	

    trace_event("Start DumpGB()", PROCESSING);
	
    
    memset(sub_data, 0, sizeof(sub_data));


	sPtr = data;
    

    /** HMA - 23052013 **/
    trace_event("------------------------------------", TRACE);
    trace_event("- Header Fields                    -", TRACE);
    trace_event("------------------------------------", TRACE);
    trace_event(sLine, TRACE);


    memcpy(sFieldData, sPtr, 3);
    sPtr += 3;
	sFieldData[3]='\0';	
    sprintf(sLine, "- Protocol Version Number    : (%s) ", sFieldData);
    trace_event(sLine, TRACE);

    memcpy(sFieldData, sPtr, 6);
    sPtr += 6;
	sFieldData[6]='\0';	
    sprintf(sLine, "- Internal Routing Key       : (%s) ", sFieldData);
    trace_event(sLine, TRACE);

    
    memcpy(sFieldData, sPtr, 15);
    sPtr += 15;
	sFieldData[15]='\0';	
    sprintf(sLine, "- Merchant ID                : (%s) ", sFieldData);
    trace_event(sLine, TRACE);

    
    memcpy(sFieldData, sPtr, 7);
    sPtr += 7;
	sFieldData[7]='\0';	
    sprintf(sLine, "- Site ID                    : (%s) ", sFieldData);
    trace_event(sLine, TRACE);

    
    memcpy(sFieldData, sPtr, 11);
    sPtr += 11;
	sFieldData[11]='\0';	
    sprintf(sLine, "- Acquirer ID                : (%s) ", sFieldData);
    trace_event(sLine, TRACE);

    
    memcpy(sFieldData, sPtr, 3);
    sPtr += 3;
	sFieldData[3]='\0';	
    sprintf(sLine, "- Transaction Currency Code  : (%s) ", sFieldData);
    trace_event(sLine, TRACE);

    
    memcpy(sFieldData, sPtr, 8);
    sPtr += 8;
	sFieldData[8]='\0';	
    sprintf(sLine, "- Terminal ID                : (%s) ", sFieldData);
    trace_event(sLine, TRACE);

    
    memcpy(sFieldData, sPtr, 4);
    sPtr += 4;
	sFieldData[4]='\0';	
    sprintf(sLine, "- Status                     : (%s) ", sFieldData);
    trace_event(sLine, TRACE);

    
    memcpy(sFieldData, sPtr, 5);
    sPtr += 5;
	sFieldData[5]='\0';	
    sprintf(sLine, "- ISO 8583 Message Length    : (%s) ", sFieldData);
    trace_event(sLine, TRACE);


    InitGBInfo(&msgInfo);
    AnalyseGB(data, &msgInfo);

    iMapCtr = 1;
    while ((msgInfo.sBitMap[(iMapCtr - 1) * 8] & 0x80) && (iMapCtr < 3))
        ++iMapCtr;

    memcpy(sLine, "- BIT MAP  : ", 13);
    nPos += 13;
    for (i = 0; i < iMapCtr * 8; i++)
    {
        sprintf(sLine + nPos, " %02X", msgInfo.sBitMap[i]);
        nPos += 3;
    }
    trace_event(sLine, TRACE);

    trace_event("------------------------------------", TRACE);
    sprintf(sLine, "- M.T.I      : %04d", msgInfo.nMsgType);
    trace_event(sLine, TRACE);
    trace_event("------------------------------------", TRACE);
    trace_event("- FLD (FIELD): LENGTH :  DATA       ", TRACE);
    trace_event("------------------------------------", TRACE);

    for (i = 1; i < 64; i++)
    {
		if (GetGBField(i, &msgInfo, sFieldData, &nLength) != SUCCES)
		{
			continue;
		}
		
        switch (i + 1)
        {
        case 48:
                sprintf(sLine, "- FLD (%03d) : (%03d) : ", i + 1, nLength);
                trace_event(sLine, TRACE);

                InitTlvPrivate(&tlvInfo);
			AnalyseTlvPrivate(sFieldData, nLength, &tlvInfo);
                for (j = 0; j < (sizeof(sPrivateDataDescDCC) / sizeof(sPrivateDataDescDCC[0])); j++)
                {
				if (GetTlvPrivate(sPrivateDataDescDCC[j].value, &tlvInfo, sFieldData, &nLength) == SUCCES)
                    {
					sFieldData[nLength] = 0;
                            if (memcmp(sPrivateDataDescDCC[j].value, "P92", 3) == 0)
                            {
                                sprintf(sLine, "       >Tag: '%.3s' : (%25s) : [***]",
                                        sPrivateDataDescDCC[j].value,
                                        sPrivateDataDescDCC[j].description);
                            }
                            else
                            {
                                sprintf(sLine, "       >Tag: '%.3s' : (%25s) : [%s]",
                                        sPrivateDataDescDCC[j].value,
                                        sPrivateDataDescDCC[j].description,
								sFieldData);
                            }
                            trace_event(sLine, TRACE);
                        }
                    }
            break;
        case 53:
                sprintf(sLine, "- FLD (%03d) : (%03d) : ", i + 1, nLength);
                trace_event(sLine, TRACE);

                for (j = 0; j < 10; j++)
                {
				GetSecurityData(j, sFieldData, sub_data, &nLength);
                    sprintf(sLine, "       > %.25s ..............: [%s] ", sSecurityDesc[j], sub_data);
                    trace_event(sLine, TRACE);
                }
            break;
        case 63:
                sprintf(sLine, "- FLD (%03d) : (%03d) : ", i + 1, nLength);
                trace_event(sLine, TRACE);

                memset(sAscii, 0, sizeof(sAscii));
			BcdToAscii(sFieldData, sAscii, 2); /** HMA - 27052013 **/

                sprintf(sLine, "       | Priv Data Set ID : [%s] ", sAscii);
                trace_event(sLine, TRACE);

			/** HMA - 27052013 **/
                InitGBInfo(&GB63Info);
			AnalyseGBField63(sFieldData, &GB63Info);
                for (j = 1; j < 32; j++)
                {
				if (GetGBSubField63(j, &GB63Info, sFieldData, &nLength) == SUCCES)
                    {
					sprintf(sLine, "       > (%02d) %.29s .. : [%s] ", j + 1, sDCCPrivateDesc[j], sFieldData);
                        trace_event(sLine, TRACE);
                    }
                }
            break;
        default:
			sprintf(sLine, "- FLD (%03d) : (%03d) : [%s] ", i + 1, nLength, sFieldData);
                trace_event(sLine, TRACE);
            break;
        }
    }

    trace_event("------------------------------------", TRACE);
    trace_event("End   DumpGB()", PROCESSING);
}

void dump_gb_buffer(char *data)
{
    TSGBInfo msgInfo;


    InitGBInfo(&msgInfo);

    AnalyseGB(data, &msgInfo);

	DumpGB(&msgInfo);
	
	InitGBInfo(&msgInfo);
}