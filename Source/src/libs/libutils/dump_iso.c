/****************************************************************************/
/***************************************************************************/
/*            UTILS_ISO.H  : UTILITAIRE POUR COPIER ET AFFICHER            */
/*                           MESSAGE  ISO 8583  EURO    (*F.M*)            */
/***************************************************************************/
/***************************************************************************/
#include <stdio.h>
#include <header_sms.h>
#include <define.h>
#include <dump.h>



#include <iso_hps.h>
#include <iso8583.h>
#include <tlv_privfields.h>
#include <tlv_private.h>
#include <sys/time.h>

#define SUCCES  0
#define ECHEC  -1

typedef struct s_tlv_print
{
	char *description;
	char *value;
} t_tlv_print;



typedef struct s_icc_iso_tag{
	char *sTagDesc;
	char *sTagValue;
	int  nPrintable;
} t_icc_iso_tag;



static t_icc_iso_tag tIccIsoTag [] =
{
	{"ISO_TAG_ISS_SCRIPT1         ","7100",1},
	{"ISO_TAG_ISS_SCRIPT2         ","7200",1},
	{"ISO_TAG_ISS_SCRIPT_CMD      ","8600",1},
	{"ISO_TAG_APP_INTER_PROFILE   ","8200",1},
	{"ISO_TAG_DED_FILE_NAME       ","8400",1},
	{"ISO_TAG_ICC_APP_ID          ","4F00",1},
	{"ISO_TAG_AUTHOR_RESP_CODE    ","8A00",1},
	{"ISO_TAG_ISS_AUTHEN_DATA     ","9100",1},
	{"ISO_TAG_TVR                 ","9500",1},
	{"ISO_TAG_TRANS_DATE          ","9A00",1},
	{"ISO_TAG_TRANS_TYPE          ","9C00",1},
	{"ISO_TAG_APPLI_EXPIRY_DATE   ","5F24",1}, /* MTR - Enh CBAE4.12 */
	{"ISO_TAG_TRANS_CUR_CODE      ","5F2A",1},
	{"ISO_TAG_TRANS_AMOUNT        ","9F02",1},
	{"ISO_TAG_OTHER_AMOUNT        ","9F03",1},
	{"ISO_TAG_TERM_APP_VER_NUM    ","9F09",1},
	{"ISO_TAG_ISS_APP_DATA        ","9F10",1},
	{"ISO_TAG_APP_RGSTD_DATA      ","9F0A",1}, /* MTR-Enh 4.11  */
	{"ISO_TAG_TERM_COUNTRY_CODE   ","9F1A",1},
	{"ISO_TAG_IFD_SERIAL_NUM      ","9F1E",1},
	{"ISO_TAG_APP_CRYPTOGRAM      ","9F26",1},
	{"ISO_TAG_CRYPTO_INFO_DATA    ","9F27",1},/*AMER 20160520: 0 turned into 1 ==> "ISO_TAG_CRYPTO_INFO_DATA" is not a sensitive data*/	
	{"ISO_TAG_TERM_CAP            ","9F33",1},
	{"ISO_TAG_CVM                 ","9F34",1},
	{"ISO_TAG_TERM_TYPE           ","9F35",1},
	{"ISO_TAG_ATC                 ","9F36",1},
	{"ISO_TAG_UNPRED_NUMBER       ","9F37",1},
	{"ISO_TAG_TRANS_SEQ_NUM       ","9F41",1},
	{"ISO_TAG_TRANS_CATEG_CODE    ","9F53",1},
	{"ISO_TAG_APP_SEQ_NUMBER      ","5F34",0},
	{"ISO_TAG_ARPC                ","9F54",1},
	{"ISO_TAG_APP_SELECTED        ","9F06",1},
	{"ISO_TAG_TRACK2              ","5700",0},
	{"ISO_TAG_PAN                 ","5A00",0},
	{"ISO_TAG_APP_NAME   	      ","9F12",1},
	{"ISO_TAG_APP_LABEL	          ","5000",1},
	{"ISO_TAG_SECUR_ATT	          ","8C00",1},
	{"ISO_TAG_FFI                 ","9F6E",1}, /*SWI06032015 ENH15.1*/
	{"ISO_TAG_TRACK_1_DATA	      ","9F1F",1},
	{"ISO_TAG_APP_USE_CONTROL     ","9F07",1},
	{"ISO_TAG_APP_RGSTD_DATA      ","9F0A",1},
};




static char    *sSecurityDesc[] = {
	"SECURITY FORMAT         " ,
	"PIN BLOCK FORMAT        " ,
	"PIN ENCRYPTION KEY INDEX" ,
	"MAC KEY INDEX           " ,
	"SOURCE PID              " ,
	"SOURCE RESOURCE         " ,
	"DESTINATION PID         " ,
	"DESTINATION RESOURCE    " ,
	"USER                    " ,
	"PURGE TIME              " ,
	"SOURCE NODE             " ,
	"DESTINATION NODE        " ,
	"PURGE TIME MS           " ,
};

static t_tlv_print  sAdditRespDataDesc[] = {
	{ "BAL_TYPE_TAG..........." ,      "A09" }
};

static t_tlv_print  sPrivateDataDesc[] = {
	{ "ATM_LANG_REQ............" , "B01" },
	{ "CARDLESS_TYPE..........." , "B02" },
	{ "AUTH_REQUESTED_AMOUNT..." , "B03" },
	{ "FILE_UPDATE_ERR_CODE...." , "P01" },
	{ "MESSAGE_TEXT............" , "P02" },
	{ "HPS_LIMITS_CURRENCY....." , "P03" },
	{ "HPS_LIMITS_AMOUNT_TAG..." , "P04" },
	{ "HPS_HOST_SCENARIO_CODE.." , "P05" },
	{ "CHECK_SET_ON_OFF_LIMIT.." , "P06" },
	{ "SPEC_LIMIT_ID..........." , "P07" },
	{ "ORIG BILL..............." , "P08" },
	{ "HPS_ACQUIRER_BANK......." , "P09" },
	{ "TPK/AWK................." , "P10" },
	{ "ORIGINAL_TRX_ID........." , "P11" },
	{ "PRIS_INTERNAL_ID........" , "P12" },
	{ "EXTERNAL_STAN..........." , "P13" },
	{ "EVENT_ACTION.......... ." , "P14" },/*SNO290915*/

	{ "PVV_OFFSET_TAG.........." , "P16" },
	{ "RRN_RETREVIAL_TAG......." , "P17" },
	{ "PAYMENT_ACC_REFERENCE..." , "P18" },/*AMER20170302: PROD00038916 MCI Enh17.Q2*/
	{ "MVV_TAG................." , "P19" }, 
	{ "ICVV_RESULT_CODE_TAG...." , "P20" },
	{ "MERCHANT_ADDIT_DATA....." , "P21" }, /* AHA20161214  PROD00036000*/
	/* { "DYN_NBR_RESULT_CODE....." , "P22" }, 		/* ELKAM20221221 PLUTONL-5555 * /
	{ "DAC_RESULT_CODE_TAG....." , "P23" }, */ 		/* ELKAM20221221 PLUTONL-5555 */
	{ "CMC7_LINE_TAG..........." , "P24" },
	{ "CRYPT_RESULT_CODE_TAG..." , "P25" },
	/* { "AUC_RESULT_CODE_TAG....." , "P26" }, */	/* ELKAM20221221 PLUTONL-5555 */
	{ "PIN_RESULT_CODE_TAG....." , "P27" },	/*AMER20170801: PROD00042216 "P28 turned into P27"*/
	{ "EXT_SECURITY_VER_RES_TAG" , "P28" },	/*AMER20170801: PROD00042216*/
	{ "SECURITY_VER_RESULTS...." , "P29" },
	{ "CHIP_AUTH_ADD_INFO......" , "P30" },
	{ "MCI_DEVICE_TYPE_IND....." , "P31" },
	{ "MCI_ASSIGNED_ID........." , "P32" },
	{ "MCI_PAN_MAPPING_INFO...." , "P33" },
	{ "ON_BEHALF_SERV.........." , "P34" },
	{ "MCI_TRACEID............." , "P35" },
	{ "ISSUER_BANK............." , "P36" },
	{ "EXT_SECURITY_RESULTS_DTL" , "P38" },/*AMER20190228 PROD00066569: CBAE Enh5.0 989*/
	{ "MERCHANT BANK CODE......" , "P39" },
	{ "E_COMMERCE_CERTIFICATE.." , "P40" },
	{ "MERCHANT LABEL.........." , "P41" },
	{ "SECURITY_LEVEL_IND......" , "P42" }, 
	{ "UCAF...................." , "P43" }, 
	{ "EXTERNAL_CRYPT_RESULTS.." , "P44" },
	{ "EXTERNAL_PROCESS_IND...." , "P45" },
	{ "ACQUIRER NETWORK CODE..." , "P46" },
	{ "ROUTING NETWORK CODE...." , "P47" },
	{ "TCC....................." , "P48" },
	{ "KSN TAG................." , "P49" },
	{ "KSN DESC TAG............" , "P50" },
	{ "AAV TAG................." , "P51" },
	{ "DIGITAL_PAYMENT_DATA...." , "P52" },
	{ "ATM_DEPOSIT_NOTES ......" , "P53" },
	{ "AAV RESULT.............." , "P54" },
	{ "TOKEN DATA.............." , "P55" },
	{ "DCC DATA................" , "P56" },
	{ "AFD REF NBR............." , "P57" },/*ICH2017 for AFD*/
	{ "ACC PT CARD PRD ID......" , "P58" },
	{ "OPERATION CODE.........." , "P59" },
	{ "WATCH LIST SCORING RES.." , "P60" },
	{ "POS DATA EXTENTION......" , "P61" }, 
	{ "FEES AMOUNTS............" , "P62" },
	{ "AUTHORIZED AMOUNT......." , "P63" },
	{ "ADD AUTH_ADD_INFO......." , "P64" },
	{ "PAYMENT_AUTH_INFO......." , "P65" },
	{ "ECOM_AUTH_INFO.........." , "P66" },
	{ "AUTHORIZATION ID........" , "P67" },
	{ "TRANSACTION ID.........." , "P68" },
	{ "ORIGINAL TERMINAL ID...." , "P69" },
	{ "TERM_INFO_DATA.........." , "P70" },
	{ "RECURRING_FLAG.........." , "P71" },  
	{ "TRANSACTION_ID_TAG_1...." , "P72" }, 
	{ "ADD_AUTH_METHOD_TAG....." , "P73" }, 
	{ "ADD_AUTH_RES_CODE_TAG..." , "P74" }, 
	{ "ACCOUNT_RESULT_TAG......" , "P75" }, 
	{ "MC_ELEC_ACCPT_IND......." , "P76" }, 
	{ "TRANS_XID_TAG..........." , "P77" }, 
	{ "TRANS_STAIN_TAG........." , "P78" }, 
	{ "SETTLEMENT_DATE_TAG....." , "P79" }, 
	{ "DINERS_NET_REF_ID......." , "P80" }, 
	{ "CARD_ISS_DATA..........." , "P81" }, 
	{ "ADDRESS_VERIF_REQ_TYPE.." , "P82" },
	{ "ADDRESS_VERIF_REP_TYPE.." , "P83" },
	{ "CVM DATA................" , "P84" },
	{ "EXTERNAL_RESP_SOURCE...." , "P85" },
	{ "FRD_CTRL_RESULT_TAG....." , "P86" },
	{ "EXT_CVV_RESULT_CODE....." , "P87" },
	{ "EXT_CVV2_RESULT_CODE...." , "P88" },
	{ "CVC1_TRACK_EDIT_ERROR..." , "P89" },
	{ "FRAUD SCORE............." , "P90" },
	{ "AMEX TRX ID............." , "P91" },  /*AMER20170919 PROD00046952*/
	{ "CVC2_DATA..............." , "P92" },
	{ "HSM_RESULT_CODE........." , "P93" },
	{ "SECONDARY_RESOURCE......" , "P94" },
	{ "NETWORK_CODE............" , "P95" },
	{ "MALFUNCTION_STIP........" , "P96" },
	{ "ACQUIRER GENRE.........." , "P97" },
	{ "MARKET_SPECIFIC........." , "P98" },
	{ "DURATION_TAG............" , "P99" },
	{ "MONEY TRANSFER DATA....." , "S20" },
	{ "ADDR VERIFICATION DATA.." , "S21" },
	/*{ "SOLUTION IDENTIFIER DATA" , "S22" },*/
	{"SALE_INFO_DATA..........", 	"S22"}, /*AMER20170921 PROD00046952*/
	/*{ "TRANSACTION TYPOLOGY DATA", "S23" },*/
	{"SALE_ITEMS_DATA.........", "S23"}, /*AMER20170921 PROD00046952*/
	{"INSTALLMENT LOAN DATA...", "S24" },/*ZKO25092018*/
	{ "DIGITIZATION_DATA........", "S25" }, 
	{ "ECOM_AUTHENTIF_DATA......", "S26" },
    { "PSD2_DATA................", "S27"}, /*HAL  20201803 Enh Q2.2020 Art 9.3.3*/ 
	/*{ "DELIVERY_ADDRESS_DATA....", "S27" },*/
	{ "IP_ADDRESS_DATA..........", "S28" },
	/*{ "ITEMS_NUMBER_DATA........", "S29" },*//* PLUTONL-4280 :Enh5.03 :1372*/
	{ "3DS_DATA.................", "S29" },/* PLUTONL-4280 :Enh5.03 :1372*/
	{ "FLEET DATA...............", "S30" },
	{ "FOREIGN RATE INFO........", "S31" },
	{ "BASE24_DE048............" , "B48" }, 
	{ "BASE24_DE060............" , "B60" }, 
	{ "ISS_AUTH_OPT............" , "O01" },
	{ "LIMIT_ID_INFO..........." , "O02" },
	{ "SPEC_LIMIT_ID_INFO......" , "O03" },
	{ "UPDATE_FILE_DATA........" , "D90" },/*SNO281015 */
	{ "FUND_TRANSFER_DATA......" , "D91" },
	{ "CB_SERVICES_DATA........" , "D92" },
	{ "CB_RESERVED_DATA........" , "D93" },
	{ "CB_DATA................." , "G01" }, /*SNO241115*/
	{ "NET_POS_ENTRY_MODE......",  "022" }, /*AMER20171218 PROD00050359*/
	{ "NET_REFERENCE_NUMBER...." , "037" }, /*HAL 20201303 Enh Q2.2020 Art 1.1 */

	{ "SMS_CHBK DATA..........." , "K01" },
	{ "PARTIAL_AMOUNT_TYPE....." , "K02" },	/*AMER20170308: PROD00039552*/
	{ "ROUTING_CODE............" , "K03" },	/*YDA20171113 (PROD00048309)*/
	{ "SMS_VCR_DATA............" , "K04" }, /*ICH17012018*/
	{ "RESUBMISSION_COUNTER...." , "K05" }, /*MTR20180406*/
    { "DIR_SRV_TRN_ID.........." , "K06" },/*ZKO:PROD00058820*/
	{ "CASHBACK_AMNT_IN_LIMIT_CRY" , "K07" },
	{ "CASHBACK_LIMIT_ID......." , "K08" },
	/*{ "AUTH_EXEMPTION_IND......" , "K09" },*//*ZKO29122021 Start PLUTONL-3345*/
	{ "MOB_PAY_SOLUTION_ID....." , "K10" },	/*ACH20200103 */
	{ "FRAUD_CASE_SEQ_ID......." , "K11" },
	{ "FLEET_CONTROLS.........." , "K12" }, /*FLEET CONTROLS*/ /*MJI 26/02/2021*/
	{ "FLEET_CONTROLS_RESULT..." , "K13" }, /*FLEET CONTROLS RESULT*/ /*MJI 26/02/2021*/
	{ "DYNAMIC_AUTHENTICATION.." , "K14" }, /*DYNAMIC AUTHENTICATION TYPE*/ /*MJI 28/01/2021*/
	{ "CORPORATE_ID............" , "K15" },	
	{ "AUTH_MSG_UUID..........." , "K16" },	
	{ "ACS_TRANS_ID............" , "K17" },/*AMER20190314 PROD00066569: CBAE Enh5.00 1073*/
	{ "NET_ISS_SETTL_INFO......" , "K18" },
	{ "NET_ACQ_SETTL_INFO......" , "K19" },
	{ "PRIV_ORIG_DATA_ELMNT...." , "K20" },	/*AMER20210402 PLUTONL-2947*/	
	{ "BR_ROUTING_PATH........." , "K21" }, /*SKA20220822 PLUTONL-5086 */
	{ "ROUTING_PATH............" , "K22" },	/*SKA20220922 PLUTONL-5106*/
	{ "SOURCE_POS_ENTRY_MODE..." , "K30" }, /*AMER20210925 PLUTONL-3607*/	
	{ "REATTEMPT_FROZEN_PERIOD." , "K40" }, /*Reattempt frozen period*//*PLUTONL-4280 :Enh5.03 :1348*/
	{ "REATTEMPT_CONDITIONS...." , "K41" }, /*Reattempt conditions*//*PLUTONL-4280 :Enh5.03 :1348*/
	{ "ORIGIN_CODE............." , "K50" }, /*Origine Code*/ /*IBO20221121 PLUTONL-5416*/
	{ "BANK_XMIT_TIME.........." , "K51" }, /*BANK_XMIT_TIME*/ /*IBO20221121 PLUTONL-5416*/
	{ "INTERNAL_DATA     ......" , "K99" },/*NAB02042021 PLUTONL-2974*/
	{ "NET_BANKNET_DATA........" , "N00" },                                            
	{ "NET_NETWORK_ID.........." , "N01" },
	{ "NET_REFERENCE_ID_TAG...." , "N02" },
	{ "NET_MSG_REASON_CODE....." , "N03" },
	{ "NET_STIP_REASON_CODE...." , "N04" },
	{ "NET_CHARGEBACK_FLAGS...." , "N06" },
	{ "NET_NET_PARTI_FLAGS....." , "N07" },
	{ "NET_VISA_ABI............" , "N08" },
	{ "NET_FRAUD_DATA.........." , "N09" },
	{ "NET_REIMB_ATTR.........." , "N11" },
	{ "NET_SHARE_GRP_CODE......" , "N12" },
	{ "NET_DECIMAL_POS........." , "N13" },
	{ "NET_ISS_CUR_CONV_DATA..." , "N14" },
	{ "NET_STLMNT_AMOUT........" , "N15" },
	{ "NET_IEW_SBA............." , "N16" },
	{ "NET_ADTL_DATA_IND......." , "N17" },
	{ "NET_VOL_REQ_IND........." , "N18" },
	{ "NET_VISA_ACT_CHK_RES...." , "N20" },
	{ "NET_VISA_MERC_ID........" , "N21" },/*HAL  20201803 Enh Q2.2020 Art 9.3.3*/ 
	{ "NET_VISA_EXT_STIP_REASON_CODE", "N22"},	/* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/  
	{ "NET_VISA_ECOM_DATA......" , "N23"},	/*PLUTONL-4280 :Enh5.03 :1372*/
	{ "VISA_WATCH_LIST_RC......" , "N24" } ,/*PLUTONL-4280 :Enh5.03 :1383*/
	{ "NET_VISA_OVERALL_ASSESSMENT_SECURE_ECOM..." , "N25" }, /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.1*/
	{ "NET_VISA_PURCHASE_IDENTIFIER..." , "N26" }, /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art8.3.1*/
	{ "NET_MCI_ACC_DT_COMP_INFO" , "N51" },/*AMER20190314 PROD00066569: CBAE Enh5.00 936*/
	{ "NET_MCI_SEC_SERV_ADD_DATA_ISS" , "N52" },/*AMER20190314 PROD00066569: CBAE Enh5.00 936*/ 
	{ "NET_MCI_TRX_ANALYSIS...." , "N53" }, /*ACH20200103 */
	{ "NET_MCI_TKN_TRX_ID......" , "N54" }, /*ACH20200103 */
	{ "NET_MCI_TRACE_ID........" , "N57" },			/*HAL PLUTONL-4154 4.2.4	AN 4972 */
	{ "NET_MCI_SEC_SERV_IND..."  , "N59" }, /*  AHA20221221 PLUTONL-5548   AN6773  Enh 2023Q2*/
	{ "NET_MCI_SEC_SERV_DATA..." , "N60" }, /*  AHA20221221 PLUTONL-5548   AN6773  Enh 2023Q2*/
	{ "NET_MAX_PROC_DATE......." , "N62" }, /*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/
	{ "ROUTING_SEQUENCE_ID....." , "Y04" }, /*tag for the seq of the selected route*//* HAL04092020 PLUTONL-2197*/
	{ "C_MSG_FLOW_ID..........." , "Y10" },
	{ "PRIV_ORIG_FUNCTION_CODE." , "024" },	/*AEA20210712 Enh21.Q4 AN4779*/
	{ "NET_TOKEN_PROGAM......" 	 , "N56" }, /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/ 
	{ "MCI_MIT_CIT_INDICATOR..." , "N58" }, /*IZL 03062022 PLUTONL-4810 ENH 22Q2 MC AN5524*/
	
 
};


static t_tlv_print  sPrivateCpsDesc[] = {
	{ "AUTH_CHAR_IND..........." , "F01" },
	{ "TRANSACTION_IND........." , "F02" },
	{ "VALIDATION_CODE........." , "F03" },
	{ "MARKET_SPECIFIC_DATA...." , "F04" },
	{ "DURATION................" , "F05" },
	{ "PRESTIGEOUS_PROP_IND...." , "F06" },
	{ "MCI_INTERCHANGE_COMP...." , "F17" },
	{ "MCI_MVV................." , "F19" },
	{ "PRODUCT_ID.............." , "F23" }, 
	{ "SPEND_QUALIF_IND........" , "F25" }, /*HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.2*/
};

static t_tlv_print  sPrivateAddPosDesc[] = {
	{ "CARD_AUTH_REL_IND ......" , "L01" },
	{ "CHIP_CONDITION_CODE....." , "L02" },
	{ "CCPS_TRANSACTION_IND...." , "L03" }
};

static t_tlv_print  sPrivateNetworkDesc[] = {
	{ "N_POS_TRANS_STATUS......" , "007" },
	{ "N_POS_ENTRY_MODE........" , "022" },
	{ "N_POS_CONDITION_CODE...." , "025" },
	{ "N_RESPONSE_CODE........." , "039" },
	{ "N_POS_DATA_VISA........." , "060" },
	{ "N_POS_DATA_EURO........." , "061" },
	{ "N_CUSTOM_PAYMENT_SERVICE" , "062" },/* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.1*/
	{ "N_FILE_UPDATE_CODE......" , "091" },
	{ "N_UF_ACTION_CODE........" , "127" }
};



/*MTR*/
void DumpFile( char *data )
{
	TSIsoInfo      msgInfo;
	TSTlvPrivate   tlvInfo;
	char           sLine[MAX_LINE_TRC];
	char	     sLine61[MAX_LINE_TRC];
	char           sTlvField[MAX_LINE_TRC + 40];
	char sub_data[LG_MAX];
	char sWork[LG_MAX];
	char buffer[LG_MAX];
	char TI[2];
	char           CardTrunc[22 + 1];
	int nLength;
	int iMapCtr,i,j,k;
	int nPos = 0;
	int pos =0;
	int nLoops;
	int nCounter;

	trace_event("Start DumpFile()",PROCESSING);
	memset(sLine,        '\0', sizeof(sLine));
	memset(sLine,        '\0', sizeof(sLine61));
	memset(sub_data,     '\0', sizeof(sub_data));
	memset(sWork,        '\0', sizeof(sWork));
	memset(buffer,       '\0', sizeof(buffer));
	memset(TI,           '\0', sizeof(TI));
	memset(CardTrunc,    '\0', sizeof(CardTrunc));
	memset(sTlvField,    '\0', sizeof(sTlvField));

	InitIsoInfo(&msgInfo);
	AnalyseIso  ( data , &msgInfo);

	iMapCtr = 1;
	while ( ( (unsigned char)msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
		++iMapCtr;

	for (i=0; i<iMapCtr * 8;i++)
	{
		sprintf(sLine + nPos," %02X", (unsigned char)msgInfo.sBitMap[i]);
		nPos += 3;
	}
	trace_event(sLine,TRACE);
	trace_event("------------------------------------",TRACE);
	sprintf(sLine,"MESSAGE_ISO_XML_FORMAT_START[%04d]" , msgInfo.nMsgType );
	trace_event(sLine,TRACE);

	sprintf(sLine," <?xml version=\"1.0\"?>");
	trace_event(sLine,TRACE);
	sprintf(sLine," <MessageInfo>");
	trace_event(sLine,TRACE);
	sprintf(sLine," <MsgType Value=\"%04d\"/>" , msgInfo.nMsgType );
	trace_event(sLine,TRACE);

	for( i = 1 ; i < 127 ; i++)
	{
		nLoops = 0;
		nCounter = 0;
		switch (i+1)
		{
		case 2:
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option != 1)
				{
					TrunCard(sWork, CardTrunc);
					memset(sWork, '\0', sizeof(sWork));
					memcpy(sWork,CardTrunc,strlen(CardTrunc));
				}
				sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sWork );
				trace_event(sLine,TRACE);
			}
			break;
		case 48 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{

					/* --------- MTR17012018 -  New code --------- */
					sprintf(sTlvField ,"<Field Number=\"%03d\" Value=\"%s\"/>", i + 1 , sWork);		      
					nLoops = (strlen(sTlvField) / 90 ) + ( 0 == strlen(sTlvField) % 90 ? 0 : 1 );
					for ( ; nCounter < nLoops ; nCounter++)
					{	
						sprintf(sLine," %.90s", sTlvField + (90 * nCounter));
						trace_event(sLine,TRACE);	
					}	
					/* ------------- END MTR17012018 ------------- */
			}
			break;
		case 55 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				PrintIccSubFieldsF(sWork,nLength);
			}
			break;
		case 60 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{

				InitTlvPrivate(&tlvInfo);
				AnalyseTlvPrivate( sWork,nLength,&tlvInfo);

				sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sWork );
				trace_event(sLine,TRACE);

			}
			break;
		case 61 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sWork);
				trace_event(sLine,TRACE);
			}
			break;
		case 62 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				InitTlvPrivate(&tlvInfo);
				AnalyseTlvPrivate( sWork,nLength,&tlvInfo);
				sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sWork );
				trace_event(sLine,TRACE);
			}
			break;
		case 31:
		case 35:
		case 45:
		case 52:
		case 102:
		case 103:
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option != 1)
				{
					memset(sWork,'*',nLength);
				}
				sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sWork );
				trace_event(sLine,TRACE);
			}
			break;
		case 53 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
					/*
				if ( strlen(sWork) < 63 )
				{
					sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sWork );
					trace_event(sLine,TRACE);
				}
					else 
					{
					sprintf(sLine," <Field Number=\"%03d\" Value=\"%.*s" ,i+1 ,63, sWork );
					trace_event(sLine,TRACE);
					sprintf(sLine," %s\" />", sWork + 63 );
					trace_event(sLine,TRACE);
				}
					*/
					/* --------- MTR17012018 -  New code --------- */		

					sprintf(sTlvField ,"<Field Number=\"%03d\" Value=\"%s\"/>", i + 1 , sWork);
					nLoops = (strlen(sTlvField) / 90 ) + ( 0 == strlen(sTlvField) % 90 ? 0 : 1 );
					for ( ; nCounter < nLoops ; nCounter++)
					{
						sprintf(sLine," %.90s", sTlvField + (90 * nCounter));
						trace_event(sLine,TRACE);
			}
					/* ------------- END MTR17012018 ------------- */
		

				}
				break;   
		default :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine," <Field Number=\"%03d\" Value=\"%s\"/>" , i+1 , sWork );
				trace_event(sLine,TRACE);
			}
			break;
		}
	}

	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sub_data, '\0', sizeof(sub_data));
	memset(sWork, '\0', sizeof(sWork));
	memset(buffer, '\0', sizeof(buffer));
	memset(sTlvField,    '\0', sizeof(sTlvField));
	InitIsoInfo(&msgInfo);
	InitTlvPrivate(&tlvInfo);

	sprintf(sLine," </MessageInfo>");
	trace_event(sLine,TRACE);
	sprintf(sLine,"MESSAGE_ISO_XML_FORMAT_END[%04d]" , msgInfo.nMsgType );
	trace_event(sLine,TRACE);
	trace_event("------------------------------------",TRACE);
	trace_event("End   DumpFile()",PROCESSING);
}

void DumpIso( char *data )
{
	TSIsoInfo      msgInfo;
	TSTlvPrivate   tlvInfo;
	TlvInfo		    isoTlvData;/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART 4022 */
	char           	sLine[MAX_LINE_TRC];
	char 			sWork[LG_MAX];
	char          	CardTrunc[22 + 1];
	char 		   	sub_data[LG_MAX];
	int 			nLength;
	int 			iMapCtr,i,j,k;
	int 			nPos = 0;

	trace_event("Start DumpIso()",PROCESSING);

	memset(sLine,        '\0', sizeof(sLine));
	memset(sWork,        '\0', sizeof(sWork));
	memset(CardTrunc,    '\0', sizeof(CardTrunc));

	InitIsoInfo(&msgInfo);

	AnalyseIso  ( data , &msgInfo);

	iMapCtr = 1;
	while ( ( (unsigned char)msgInfo.sBitMap [(iMapCtr - 1) * 8] & 0x80)  && (iMapCtr < 3) )
		++iMapCtr;

	memcpy(sLine,"- BIT MAP  : ",13); nPos+=13;
	for (i=0; i<iMapCtr * 8;i++)
	{
		sprintf(sLine + nPos," %02X", (unsigned char)msgInfo.sBitMap[i]);
		nPos += 3;
	}
	trace_event(sLine,TRACE);

	trace_event("------------------------------------",TRACE);
	sprintf(sLine,"- M.T.I      : %04d" , msgInfo.nMsgType );
	trace_event(sLine,TRACE);
	trace_event("------------------------------------",TRACE);
	trace_event("- FLD (FIELD): LENGTH :  DATA       ",TRACE);
	trace_event("------------------------------------",TRACE);

	for( i = 1 ; i <= 127 ; i++)
	{
		switch (i+1)
		{
		case 2: 
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option != 1)
				{
					TrunCard(sWork, CardTrunc);
					memset(sWork, '\0', sizeof(sWork));
					memcpy(sWork,CardTrunc,strlen(CardTrunc));
				}
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength, sWork );
				trace_event(sLine,TRACE);
			}
			break;

		case 48 :
		case 63 : /*  AHA20221221 PLUTONL-5548   AN6773  Enh 2023Q2*/
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);

				InitTlvPrivate(&tlvInfo);
				AnalyseTlvPrivate( sWork,nLength,&tlvInfo);
				for (j=0; j<(sizeof(sPrivateDataDesc)/sizeof(sPrivateDataDesc[0])); j++)
				{
					if ( GetTlvPrivate(sPrivateDataDesc[j].value,&tlvInfo,sub_data,&nLength) == SUCCES )
					{
						sub_data[nLength] = 0;
						if( memcmp(sPrivateDataDesc[j].value, "P10", 3) == 0 ||
							memcmp(sPrivateDataDesc[j].value, "P16", 3) == 0 ||
							memcmp(sPrivateDataDesc[j].value, "P15", 3) == 0 ||	/*AMER 20160602 Tag P15 added*/
							memcmp(sPrivateDataDesc[j].value, "P92", 3) == 0   /*AMER 20160526 "P92" Added */
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

						/*Start HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022 */
						else if( memcmp(sPrivateDataDesc[j].value, MONEY_TRANSFER_DATA, 3) == 0)
						{
							sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
								sPrivateDataDesc[j].value,
								sPrivateDataDesc[j].description);
							trace_event(sLine,TRACE);

							IniIsoMoneyTransDataTlv(&isoTlvData);
							AnalyseTlvInfo(sub_data, nLength, &isoTlvData);

							DumpTlvInfo(&isoTlvData);	
						}
						/*End HAL15072020 PLUTONL-2111 ENHS 20.Q4 ART4022 */

						/*Start AMER20170921 PROD00046952*/
						else if( memcmp(sPrivateDataDesc[j].value, SALE_INFO, 3) == 0)
						{
							sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
								sPrivateDataDesc[j].value,
								sPrivateDataDesc[j].description);
							trace_event(sLine,TRACE);

							IniSaleInfoDataTlv(&isoTlvData);
							AnalyseTlvInfo(sub_data, nLength, &isoTlvData);

							DumpTlvInfo(&isoTlvData);	/*AMER20171219 PROD00050359*/

							/*for (k=0; k<(sizeof(sPrivateSaleInfoDataDesc)/sizeof(sPrivateSaleInfoDataDesc[0])); k++)
							{
							if ( GetTlvInfoField(sPrivateSaleInfoDataDesc[k].value,&isoSaleInfoDataTlv,sWork,&nLength) != SUCCES )
							{
							continue;
							}
							sWork[nLength] = 0;

							sprintf ( sLine ,"             >Tag: '%.3s' : (%25s) : [%s]",
							sPrivateSaleInfoDataDesc[k].value,
							sPrivateSaleInfoDataDesc[k].description,
							sWork);
							trace_event(sLine,TRACE);
							}*/	
						}
						else if( memcmp(sPrivateDataDesc[j].value, SALE_ITEMS, 3) == 0)
						{
							sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
								sPrivateDataDesc[j].value,
								sPrivateDataDesc[j].description);
							trace_event(sLine,TRACE);

							IniSaleItemsDataTlv(&isoTlvData);
							AnalyseTlvInfo(sub_data, nLength, &isoTlvData);

							DumpTlvInfo(&isoTlvData);	/*AMER20171219 PROD00050359*/

							/*for (k=0; k<(sizeof(sPrivateSaleItemsDataDesc)/sizeof(sPrivateSaleItemsDataDesc[0])); k++)
							{
							if ( GetTlvInfoField(sPrivateSaleItemsDataDesc[k].value,&isoSaleItemsDataTlv,sWork,&nLength) != SUCCES )
							{
							continue;
							}
							sWork[nLength] = 0;

							sprintf ( sLine ,"             >Tag: '%.3s' : (%25s) : [%s]",
							sPrivateSaleItemsDataDesc[k].value,
							sPrivateSaleItemsDataDesc[k].description,
							sWork);
							trace_event(sLine,TRACE);
							}*/
						}
						/*End AMER20170921 PROD00046952*/
						/* Start AMER20190225 PROD00065991: VISA Enh19.Q2 Art2.4*/
						else if( memcmp(sPrivateDataDesc[j].value, MERCHANT_ADDITIONAL_DATA, 3) == 0)
						{
							sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
							sPrivateDataDesc[j].value,
							sPrivateDataDesc[j].description);
							trace_event(sLine,TRACE);
							
							IniIsoPayFacDataTlv(&isoTlvData);
							AnalyseTlvInfo(sub_data, nLength, &isoTlvData);
							
							DumpTlvInfo(&isoTlvData);
						}
						/* End AMER20190225 PROD00065991: VISA Enh19.Q2 Art2.4*/

						/*Start HAL2402 PLUTONL-4391*/
						else if (memcmp(sPrivateDataDesc[j].value, TOKEN_DATA, 3) == 0)
						{
							sprintf(sLine, "       >Tag: '%.3s' : (%25s) ",
									sPrivateDataDesc[j].value,
									sPrivateDataDesc[j].description);
							trace_event(sLine, TRACE);

							IniTknDataTlv(&isoTlvData);
							AnalyseTlvInfo(sub_data, nLength, &isoTlvData);

							DumpTlvInfo(&isoTlvData);
						}
						/*End HAL2402 PLUTONL-4391*/

						/*Start HAL PROD00073823 20201102 Enh Q2.2020*/ 
				    	else if( memcmp(sPrivateDataDesc[j].value, ECOM_AUTHENTIF_DATA, 3) == 0)
						{
							sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
								sPrivateDataDesc[j].value,
								sPrivateDataDesc[j].description);
							trace_event(sLine,TRACE);

							InitEcomAuthentificationInfoDataTlv(&isoTlvData);
							AnalyseTlvInfo(sub_data, nLength, &isoTlvData);

							DumpTlvInfo(&isoTlvData);
						}
					    /*End HAL PROD00073823 20201102 Enh Q2.2020*/ 
						/*Start HAL 20202003 ENHS20202003 ART 9.3.3*/
						else if( memcmp(sPrivateDataDesc[j].value, PSD2_DATA, 3) == 0)
						{
							sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
								sPrivateDataDesc[j].value,
								sPrivateDataDesc[j].description);
							trace_event(sLine,TRACE);

							InitExempAuthInfoDataTlv(&isoTlvData);
							AnalyseTlvInfo(sub_data, nLength, &isoTlvData);

							DumpTlvInfo(&isoTlvData);	
						}
						/*End HAL 20202003 ENHS20202003 ART 9.3.3*/
						
						/*Start ZKO 25072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
						else if( memcmp(sPrivateDataDesc[j].value, P7_F048_LOAN_DATA, 3) == 0)
						{
							sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
								sPrivateDataDesc[j].value,
								sPrivateDataDesc[j].description);
							trace_event(sLine,TRACE);

							IniIsoInstallmentLoanTlv(&isoTlvData);
							AnalyseTlvInfo(sub_data, nLength, &isoTlvData);

							DumpTlvInfo(&isoTlvData);	
						}
						/*Start NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
						else if( memcmp(sPrivateDataDesc[j].value,DATA_3DS , 3) == 0)
						{
							sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) ",
								sPrivateDataDesc[j].value,
								sPrivateDataDesc[j].description);
							trace_event(sLine,TRACE);

							InitInfo3DSDataTlv(&isoTlvData);
							AnalyseTlvInfo(sub_data, nLength, &isoTlvData);

							DumpTlvInfo(&isoTlvData);	
						}
						/*End NAB PLUTONL-9856-Enh 2025Q2 VISA 3.7*/
						/*End ZKO 25072022 PLUTONL-4902 ENH 22.Q4 VISA Art3.17*/
						else
						{
							sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
								sPrivateDataDesc[j].value,
								sPrivateDataDesc[j].description,
								sub_data );
							trace_event(sLine,TRACE);
						}
					}
				}
			}
			break;

		case 55 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);
				PrintIccSubFields(sWork,nLength);
			}
			break;

		case 60 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);

				InitTlvPrivate(&tlvInfo);
				AnalyseTlvPrivate( sWork,nLength,&tlvInfo);
				for (j=0; j<(sizeof(sPrivateAddPosDesc)/sizeof(sPrivateAddPosDesc[0])); j++)
				{
					if ( GetTlvPrivate(sPrivateAddPosDesc[j].value,&tlvInfo,sub_data,&nLength) == SUCCES )
					{
						sub_data[nLength] = 0;
						sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
							sPrivateAddPosDesc[j].value,
							sPrivateAddPosDesc[j].description,
							sub_data );
						trace_event(sLine,TRACE);
					}
				}
			}
			break;

		case 61 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);

				InitTlvPrivate(&tlvInfo);
				AnalyseTlvPrivate( sWork,nLength,&tlvInfo);
				for (j=0; j<(sizeof(sPrivateNetworkDesc)/sizeof(sPrivateNetworkDesc[0])); j++)
				{
					if ( GetTlvPrivate(sPrivateNetworkDesc[j].value,&tlvInfo,sub_data,&nLength) == SUCCES )
					{
						sub_data[nLength] = 0;
						sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
							sPrivateNetworkDesc[j].value,
							sPrivateNetworkDesc[j].description,
							sub_data );
						trace_event(sLine,TRACE);
					}
				}
			}
			break;

		case 62 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);

				InitTlvPrivate(&tlvInfo);
				AnalyseTlvPrivate( sWork,nLength,&tlvInfo);
				for (j=0; j<(sizeof(sPrivateCpsDesc)/sizeof(sPrivateCpsDesc[0])); j++)
				{
					if ( GetTlvPrivate(sPrivateCpsDesc[j].value,&tlvInfo,sub_data,&nLength) == SUCCES )
					{
						sub_data[nLength] = 0;
						sprintf ( sLine ,"       >Tag: '%.3s' : (%25s) : [%s]",
							sPrivateCpsDesc[j].value,
							sPrivateCpsDesc[j].description,
							sub_data );
						trace_event(sLine,TRACE);
					}
				}
			}
			break;
		case 31:   
		case 35: 
		case 45: 
		case 52:
		case 102:
		case 103:
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				if (debug_option != 1)
				{
					memset(sWork,'*',nLength);
				}
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength, sWork );
				trace_event(sLine,TRACE);

			}
			break;
		case 53 :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : " , i+1 , nLength );
				trace_event(sLine,TRACE);

				for ( j=0; j< sizeof(sSecurityDesc)/sizeof(sSecurityDesc[0]) ; j ++ )
				{
					GetSecurityData (j,sWork,sub_data,&nLength);
					sprintf(sLine ,"       > %.25s ..............: [%s] " , sSecurityDesc [ j ] , sub_data    );
					trace_event(sLine,TRACE);

				}
			}
			break;

		default :
			if ( GetIsoField(i, &msgInfo, sWork, &nLength ) == SUCCES )
			{
				sprintf(sLine,"- FLD (%03d) : (%03d) : [%s] " , i+1 , nLength ,sWork );
				trace_event(sLine,TRACE);
			}

			break;
		}

	}

	memset(CardTrunc, 0, sizeof(CardTrunc));
	InitIsoInfo(&msgInfo);
	InitTlvPrivate(&tlvInfo);
	memset(sub_data, 0, sizeof(sub_data));
	memset(sWork,        '\0', sizeof(sWork));

	trace_event("------------------------------------",TRACE);
	trace_event("End   DumpIso()",PROCESSING);

	/* MTR17012018 - Add debug_option condition*/ 
	if (debug_option == 1)
		DumpFile(data);

}


void dump_p7_struct( TSIsoInfo *isoinfo )
{
	char data[LG_MAX];
	memset(data, 0, sizeof(data));
	IsoBuildMsg ( data , isoinfo );
	DumpIso( data );
	memset(data, 0, sizeof(data)); /**PA DSS*/
}
/*
int LogObjectEntry(TSP7LogEntry* pkLogEntry);

void dump_p7_iss_struct( TSIsoInfo *isoinfo )
{
	
	int					nResult;

	nResult = LogObjectEntry(TP_LOG_ENT_ISO_ISS, isoinfo, sizeof(TSIsoInfo));
}
*/

void IsoDumpIccFields (char *sIccData, int nIccLen)
{

	trace_event("Start IsoDumpIccFields()",PROCESSING);
	trace_event("         Field                        |  Len  |              Value",TRACE);
	trace_event("-------------------------------------------------------------------",TRACE);
	PrintIccSubFields (sIccData, nIccLen);
	trace_event("-------------------------------------------------------------------",TRACE);
	trace_event("End   IsoDumpIccFields()",PROCESSING);

}


void DumpTlvPrivate(TSTlvPrivate* tlvInfo)
{

	int		j;
	char	sub_data[1024];
	int		nLength;
	char	sLine[MAX_LINE_TRC];

	trace_event("Start DumpTlvPrivate()",PROCESSING);

	for (j=0; j<(sizeof(sPrivateDataDesc)/sizeof(sPrivateDataDesc[0])); j++)
	{
		if ( GetTlvPrivate(sPrivateDataDesc[j].value,tlvInfo,sub_data,&nLength) == SUCCES )
		{
			sub_data[nLength] = 0;
			if( memcmp(sPrivateDataDesc[j].value, "P10", 3) == 0 ||
				memcmp(sPrivateDataDesc[j].value, "P15", 3) == 0 ||
				memcmp(sPrivateDataDesc[j].value, "P92", 3) == 0 ||
				memcmp(sPrivateDataDesc[j].value, "I73", 3) == 0 ||
				memcmp(sPrivateDataDesc[j].value, "I74", 3) == 0 ||
				memcmp(sPrivateDataDesc[j].value, "P16", 3) == 0 
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
	}

	trace_event("End   DumpTlvPrivate(OK)",PROCESSING);
}
/* MTR */
int PrintIccSubFieldsF (char *sIccData, int nIccLen)
{
	TSTagIso       lTagIso;
	char           sWork[LG_MAX];
	char           sLine[MAX_LINE_TRC];
	char	       sLine2[MAX_LINE_TRC];
    char 	       sTlvField[MAX_LINE_TRC + 40];
	int            i = 0;
	int            nLen = 0;
	int 	       nCounter = 0;
    int 	       nLoops   = 0;


	memset(sWork, '\0', sizeof(sWork));
	memset(sLine, '\0', sizeof(sLine));
	memset(sLine2, '\0', sizeof(sLine2));
	memset(sTlvField, '\0', sizeof(sTlvField));

	InitIsoIcTag (&lTagIso);
	AnalyseIsoIc (sIccData, nIccLen, &lTagIso);


	for (i=0; i<(sizeof(tIccIsoTag)/sizeof(tIccIsoTag[0])); i++)
	{
		memset(sWork, 0, sizeof(sWork));

		if ( GetIsoIcTag (tIccIsoTag[i].sTagValue , &lTagIso,sWork,&nLen) == SUCCESS)
		{
		/* -- MTR290318 - Single byte tag management(82,9A,9C,...) -- */
		if ( memcmp(tIccIsoTag[i].sTagValue + 2 ,"00",2) == 0 )
			sprintf(sLine2,"%s%.02s%02X%s",sLine2,tIccIsoTag[i].sTagValue ,nLen/2 ,sWork);
		else 
			sprintf(sLine2,"%s%s%02X%s",sLine2,tIccIsoTag[i].sTagValue ,nLen/2 ,sWork);	
	
		}
	}

	/* --------- MTR17012018 -  New code --------- */
	sprintf(sTlvField ,"<Field Number=\"%03d\" Value=\"%s\"/>", 55 , sLine2);

    nLoops = (strlen(sTlvField) / 90 ) + ( 0 == strlen(sTlvField) % 90 ? 0 : 1 );
    for ( ; nCounter < nLoops ; nCounter++)
		{
    	sprintf(sLine," %.90s", sTlvField + (90 * nCounter));
			trace_event(sLine,TRACE);
		}
	/* ------------- END MTR17012018 ------------- */

	return(OK);
}


int PrintIccSubFields (char *sIccData, int nIccLen)
{
	TSTagIso       lTagIso;
	char           sWork[LG_MAX];
	char           sLine[MAX_LINE_TRC];
	int            i = 0;
	int            nLen = 0;

	memset(sWork, '\0', sizeof(sWork));
	memset(sLine, '\0', sizeof(sLine));

	InitIsoIcTag (&lTagIso);
	AnalyseIsoIc (sIccData, nIccLen, &lTagIso);

	for (i=0; i<(sizeof(tIccIsoTag)/sizeof(tIccIsoTag[0])); i++)
	{
		memset(sWork, 0, sizeof(sWork));

		if ( GetIsoIcTag (tIccIsoTag[i].sTagValue , &lTagIso,sWork,&nLen) == SUCCESS)
		{
			if (debug_option != 1 && tIccIsoTag[i].nPrintable == NO_FLAG)
			{
				memset(sWork,'*',nLen);
			}
			sprintf(sLine,"     > [%s]|%s ....: [%03d] : [%s]", tIccIsoTag[i].sTagValue,tIccIsoTag[i].sTagDesc,nLen,sWork);
			trace_event(sLine,TRACE);
		}
	}

	memset(sWork, '\0', sizeof(sWork));
	InitIsoIcTag (&lTagIso);
	return(OK);
}

/******************************************************************************/
#define BIN_DATA_SEP_START		'('
#define BIN_DATA_SEP_END		')'
#define IS_NDC_BIN(a)		( iscntrl(a) || a == '\x7F' || a == BIN_DATA_SEP_START || a == BIN_DATA_SEP_END )
/******************************************************************************/
int P7_FromRawData(char* buffer_in, int nLengthIn, int nMaxLength, char* buffer_out, int* pnLengthOut)
{

	int		nOffset;
	int		i;
	int		nIsBinData;


	nOffset = 0;
	nIsBinData = NOK;

	for(i=0; i < nLengthIn; i++ )
	{
		
		
		if( !IS_NDC_BIN(buffer_in[i]))
		{
			if( nIsBinData == OK )
			{
				buffer_out[nOffset++] = BIN_DATA_SEP_END;
				nIsBinData = NOK;
			}
			
			buffer_out[nOffset++] = buffer_in[i];
			
		}
		else
		{
			if( nIsBinData != OK )
			{
				buffer_out[nOffset++] = BIN_DATA_SEP_START;
				nIsBinData = OK;
			}

			buffer_out[nOffset++] = Hex_To_Ascii((char)((buffer_in[i] & 0xf0) >> 4)) ;
			buffer_out[nOffset++] = Hex_To_Ascii((char)(buffer_in[i] & 0x0f ) ) ;
		}
		
		if( nOffset >= nMaxLength )
		{
			(*pnLengthOut) = nOffset;
			return NOK;
		}

	}

	if( nIsBinData == OK )
	{
		buffer_out[nOffset++] = BIN_DATA_SEP_END;
		nIsBinData = NOK;
	}

	buffer_out[nOffset]='\0';

	(*pnLengthOut) = nOffset;

	return OK;
}

/******************************************************************************/

void DumpNDCFile( char *data, int nLength )
{

	char	sLocBuffer[LG_MAX];
	char	sLine[MAX_LINE_TRC];
	int		nLocLen;
	int		i;
	int		nCount;

	trace_event("Start DumpNDCFile()",TRACE);

	P7_FromRawData(data, nLength, LG_MAX, sLocBuffer, &nLocLen);

	nCount = nLocLen/80 + 1;

	for(i=0; i < nCount; i++ )
	{
		sprintf(sLine,"%.80s",sLocBuffer + i*80);
		trace_event(sLine,TRACE);
	}

	trace_event("End   DumpNDCFile()",TRACE);
}


