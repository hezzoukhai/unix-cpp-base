/*********************************************************************************************************************************/
/*  (c) Hightech Payment Systems 2002                                          													 */
/*  NAME                                                                       													 */
/*      dump_tlv.c                                                             													 */
/*                                                                             													 */
/* FUNCTION                                                                    													 */
/*         Dump of TLV data element                                            													 */
/* NOTES                                                                       													 */
/*                                                                             													 */
/* MODIFIED                                                                    													 */
/*        HY010302V201   H.YOUSFI   01/03/2002 - Modified V2.0.1 : Change of the size of the variable sLine                      */
/*********************************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <locale.h>
#include <stdlib.h>
#include <iso_hps.h>
#include <define.h>
#include <tlv_autho.h>
#include <dump.h>
#include <tlv_private.h>
/**Begin MKB240408**/
#ifdef SOLARIS
#include <sys/time.h>
#endif
/**End MKB240408**/


typedef struct s_tlv_print
{
	char *description;
	char *value;
} t_tlv_print;


static t_tlv_print sTlvPrint[] = {
	{			"PAN","002"		},
	{			"PROCESSING_CODE","003"		},
	{			"AMOUNT_TRANSACTION","004"		},
	{			"AMOUNT_SETTLEMENT","005"		},
	{			"AMOUNT_CARD_BILLING","006"		},
	{			"TRANS_DATE_TIME","007"		},
	{			"AMOUNT_CARD_BILL_FEE","008"		},
	{			"CONV_RATE_SETTLEMENT","009"		},
	{			"CONV_RATE_CARD_BILL","010"		},
	{			"INTERNAL STAN","011"		},
	{			"DATE_TIME_LOCAL_TRAN","012"		},
	{			"EFFECTIVE_DATE","013"		},
	{			"EXPIRY_DATE","014"		},
	{			"SETTLEMENT_DATE","015"		},
	{			"CONVERSION_DATE","016"		},
	{			"CAPTURE_DATE","017"		},
	{			"MERCHANT_TYPE","018"		},
	{			"ACQU_INST_COUNTRY","019"		},
	{			"PAN_EXTENDED_COUNTRY","020"		},
	{			"FORW_INST_COUNTRY","021"		},
	{			"POS_DATA","022"		},
	{			"CARD_SEQUENCE_NUMBER","023"		},
	{			"FUNCTION_CODE","024"		},
	{			"MESSAGE_REASON_CODE","025"		},
	{			"MERCHANT_ACTIVITY","026"		},
	{			"AUTH_NUMBER_LENGTH","027"		},
	{			"RECONCILIATION_DATE","028"		},
	{			"RECON_INDICATOR","029"		},
	{			"INITIAL_AMOUNTS","030"		},
	{			"ACQUIRER_REF_DATA","031"		},
	{			"ACQ_INST_IDENT_CODE","032"		},
	{			"FOR_INST_IDENT_CODE","033"		},
	{			"PAN_EXTENDED","034"		},
	{			"TRACK2_DATA","035"		},  /* Mehdi Elyajizi */
	{			"TRACK3_DATA","036"		},
	{			"RRN","037"		},
	{			"AUTH_NUMBER","038"		},
	{			"RESPONSE_CODE","039"		},
	{			"SERVICE_CODE","040"		},
	{			"CARD_ACC_TERMINAL","041"		},
	{			"CARD_ACC_ID","042"		},
	{			"CARD_ACC_LOCATION","043"		},
	{			"ADDITIONAL_RESP_DATA","044"		},
	{			"TRACK1_DATA","045"		},
	{			"AMOUNT_FEE","046"		},
	{			"ADD_DATA_NATIONAL","047"		},
	{			"ADD_DATA_PRIVATE","048"		},
	{			"TRAN_CURRENCY_CODE","049"		},
	{			"SETL_CURRENCY_CODE","050"		},
	{			"BILL_CURRENCY_CODE","051"		},
	{			"PIN_DATA","052"		},  /* Mehdi Elyajizi */
	{			"SECURITY_DATA","053"		},
	{			"ADD_AMOUNTS","054"		},
	{			"ICC_DATA","055"		},
	{			"ORIGINAL_DATA","056"		},
	{			"AUTH_CYCLE_CODE","057"		},
	{			"AUTH_INST_ID","058"		},
	{			"TRANSPORT_DATA","059"		},
	{			"ADD_POS_DATA","060"		},
	{			"NETWORK_POS_DATA","061"		},
	{			"CPS_DATA","062"		},
	{			"NETWORK_DATA","063"		},
	{			"MAC_2","064"		},
	{			"EXTENDED_BIT_MAP","065"		},
	{			"ORIGINAL_AMOUNT_FEE","066"		},
	{			"EXT_PAYMENT_CODE","067"		},
	{			"RECEIV_INST_COUNTRY","068"		},
	{			"SETTL_INST_COUNTRY","069"		},
	{			"AUTH_INST_CODE","070"		},
	{			"MESSAGE_NUMBER","071"		},
	{			"RECORD_DATA","072"		},
	{			"DATE_ACTION","073"		},
	{			"CREDITS_NUMBER","074"		},
	{			"CREDITS_REV_NUMBER","075"		},
	{			"DEBITS_NUMBER","076"		},
	{			"DEBITS_REV_NUMBER","077"		},
	{			"TRANSFER_NUMBER","078"		},
	{			"TRANSFER_REV_NUMBER","079"		},
	{			"INQUIRIES_NUMBER","080"		},
	{			"AUTHS_NUMBER","081"		},
	{			"INQUIRIES_REV_NUMBER","082"		},
	{			"PAYMENT_NUMBER","083"		},
	{			"PAYMENT_REV_NUMBER","084"		},
	{			"FEES_NUMBER","085"		},
	{			"CREDITS_AMOUNT","086"		},
	{			"CREDITS_REV_AMOUNT","087"		},
	{			"DEBITS_AMOUNT","088"		},
	{			"DEBITS_REV_AMOUNT","089"		},
	{			"AUTHS_REV_NUMBER","090"		},
	{			"REC_INST_COUNTRY","091"		},
	{			"INITIATOR_COUNTRY","092"		},
	{			"RECEIVING_INST_ID","093"		},
	{			"INITIATOR_INST_ID","094"		},
	{			"ISSUER_REF_DATA","095"		},
	{			"KEY_MANAGE_DATA","096"		},
	{			"AMOUNT_NET_SETTL","097"		},
	{			"PAYEE","098"		},
	{			"SETTL_INST_ID","099"		},
	{			"RECEIV_INST_ID","100"		},
	{			"FILE_NAME","101"		},
	{			"ACCOUNT_ID_1","102"		},
	{			"ACCOUNT_ID_2","103"		},
	{			"TRANS_DESCRIPTION","104"		},
	{			"CREDITS_REJECT_AMOUNT","105"		},
	{			"DEBITS_REJECT_AMOUNT","106"		},
	{			"CREDITS_REJECT_NUMBER","107"		},
	{			"DEBITS_REJECT_NUMBER","108"		},
	{			"CREDITS_FEE_AMOUNT","109"		},
	{			"CREDITS_FEE_NUMBER","110"		},
	{			"RESERVED_ISO_1","111"		},
	{			"RESERVED_ISO_2","112"		},
	{			"RESERVED_ISO_3","113"		},
	{			"RESERVED_ISO_4","114"		},
	{			"RESERVED_ISO_5","115"		},
	{			"RESERVED_NATIONAL_1","116"		},
	{			"RESERVED_NATIONAL_2","117"		},
	{			"RESERVED_NATIONAL_3","118"		},
	{			"RESERVED_NATIONAL_4","119"		},
	{			"RESERVED_NATIONAL_5","120"		},
	{			"RESERVED_NATIONAL_6","121"		},
	{			"RESERVED_NATIONAL_7","122"		},
	{			"RESERVED_PRIVATE_1","123"		},
	{			"RESERVED_PRIVATE_2","124"		},
	{			"RESERVED_PRIVATE_3","125"		},
	{			"RESERVED_PRIVATE_4","126"		},
	{			"RESERVED_PRIVATE_5","127"		},
	{			"MAC","128"		},
	{			"REVERSAL_STAN","129"		},
	{			"USER","130"		},
	{			"INTERNAT ROUTING CHECK","131"		},
	{			"CONTROL VERIF INDEX","132"		},
	{			"PIN RETRY MAX","133"		},
	{			"DB SERVICE CODE", "134"		},
	{			"GET_CARD_DATA_LEVEL", "135"		},
	{			"SCENARIO CODE", "136"		},
	{			"RECORD STATUS", "137"		},
	{			"LOYALTY POINTS SIGN", "138"		},
	{			"CURRENT MONTH IND", "139"		},
	{			"OFF LINE INDICATOR", "140"		},
	{			"NB PRODUCT GROUP", "141"		},
	{			"CATALOGUE CODE", "142"		},
	{			"BAL CLOSING DATE", "143"		},
	{			"CARD ACTIVITY UPD", "144"		},
	{			"PIN NBR", "145"		},

	/*****AdditionalTAGForAUTHO_ACTIVITY****/
	{			"MSG_TYPE","300"			},
	{			"ORIG_MSG_TYPE","301"		},
	{			"ROUTING_CODE","200"		},
	{			"CAPTURE_CODE","201"		},
	{			"ORIG_ACTION_CODE","202"		},
	{			"EVENT_CODE","203"		},
	{			"NETWORK_CODE","204"		},
	{			"ISSUING_BANK","205"		},
	{			"RESP_DATE_TIME","206"		},
	{			"INTERN_DATE_TIME","207"		},
	{			"BUSINESS_DATE","208"		},
	{			"PRODUCT_CODE","209"		},
	{			"CARD_TYPE","210"		},
	{			"VIP_LEVEL","211"		},
	{			"DST_ACCOUNT_BANK","212"		},
	{			"ISS_STLMNT_AMOUNT","213"		},
	{			"ISS_STLMNT_CURRENCY","214"		},
	{			"ISS_STLMNT_DATE","215"		},
	{			"ISS_CNV_RATE_STLMNT","216"		},
	{			"ISS_CNV_RAT_STL_DATE","217"		},
	{			"ACQ_STLMNT_AMOUNT","218"		},
	{			"ACQ_STLMNT_CURRENCY","219"		},
	{			"ACQ_STLMNT_DATE","220"		},
	{			"ACQ_CNV_RATE_STLMNT","221"		},
	{			"ACQ_CNV_RAT_STL_DATE","222"		},
	{			"ISS_STLMNT_FEE","223"		},
	{			"ACQ_STLMNT_FEE","224"		},
	{			"SERV_SETUP_CODE","225"		},
	{			"PRODUCT_CUR_CODE","226"		},
	{			"LIMITS_INDEXES","227"		},
	{			"PERIOD_CODE","228"		},
	{			"PERIOD_TYPE","229"		},
	{			"PERIOD_VALUE","230"		},
	{			"CR_CUR_CODE","231"		},
	{			"CR_CREDIT_LIMIT","232"		},
	{			"CR_CASH_LIMIT","233"		},
	{			"CR_LOAN_LIMIT","234"		},
	{			"CR_VIP_LEVEL","235"		},
	{			"CR_CREDIT_CUR_BAL","236"		},
	{			"CR_CASH_CUR_BAL","237"		},
	{			"CR_LOAN_CUR_BAL","238"		},
	{			"CR_RESPONSE_CODE","239"		},
	{			"CR_PEND_AUT_CREDIT","240"		},
	{			"CR_PEND_AUT_CASH","241"		},
	{			"CR_PEND_AUT_LOAN","242"		},
	{			"CR_AVAILABLE_BAL","243"		},
	{			"CR_FIRST_DUE_DATE","244"		},
	{			"CR_INSTALL_AMOUNT","245"		},
	{			"CR_TERM_COUNT","246"		},
	{			"CR_PRODUCT_GRP1","247"		},
	{			"CR_PRODUCT_GRP2","248"		},
	{			"CR_PRODUCT_GRP3","249"		},
	{			"CR_PRODUCT_GRP4","250"		},
	{			"CR_PRODUCT_GRP5","251"		},
	{			"LOYALTY_POINTS","252"		},
	{			"ACQUIRER_BANK","253"		},
	{			"FORWARD_BANK","254"		},
	{			"AUTHOR_SRC","255"		},
	{			"CARD_LEVEL","256"		},
	{			"BAL_LEVEL","257"		},
	{			"DB_CUR_BAL","258"		},
	{			"LIMIT_BEFORE_TRN","259"		},
	{			"LIMIT_ID","260"		},
	{			"SHADOW_ACT_LEVEL","261"		},
	{			"HOST_LEVEL","262"		},
	{			"CLIENT_LEVEL","263"		},
	{			"EXECPT_LEVEL","264"		},
	{			"ACTION_TRANSL_LEVEL","265"		},
	{			"CARD_LIMIT_EXP_LEVEL","266"		},
	{			"VIP_ACTION_TRL_LEVEL","267"		},
	{			"AUTHO_PERIOD_LEVEL","268"		},
	{			"SECURITY_VERIF_FLAGS","269"		},
	{			"SECURITY_VER_RESULTS","270"		},
	{			"ADDRESS_VERIF_DATA","271"		},
	{			"REJECT_CODE","272"		},
	{			"REJECT_REASON","273"		},
	{			"TIME_STAMP_PLUS_MSG","274"		},
	{			"ORIGNE_CODE","275"		},
	{			"ORIG_TRANS_DATE_TIME","276"		},
	{			"AUTHO_FLAG","277"		},
	{			"REVERSAL_FLAG","278"		},
	{			"TRANSACTION_FLAG","279"		},
	{			"MATCH_STATUS","280"		},
	{			"MATCH_DATE","281"		},
	{			"MATCH_LEVEL","282"		},
	{			"MATCH_PURGE_DATE","283"		},
	{			"TRACK_INFO","284"		},
	{			"ACQ_RESOURCE_CODE","285"		},
	{			"ISS_RESOURCE_CODE","286"		},
	{			"TABLE_INDICATOR","287"		},
	{			"CUR_TABLE_INDICATOR","288"		},
	{			"REVERSAL_TRANS_DATE","291"		},
	{			"CASH_BACK_AMOUNT","293"		},
	{			"REPLACEMENT_AMOUNT","294"		},
	{			"REPLACEMENT_BILLING","295"		},

	{			"TRANSACTION_IDENTIFIER","296"		},/*EBE140623*/
	{			"AUTHORIZATION_IDENTIFIER","297"	},
	{			"CARD_PRODUCT_TYPE","298"			},

	{			"REPLACEMENT_FEE", "302"  },
	{			"REPLACEMENT_SETTL", "304"  },	/*AMER20210414 PLUTONL-2991*/
	{			"SRC_ACC_ENT_COD", "321"  },
	{           "SRC_ACC_ENT_ID", "322" },
	{           "SRC_ACC_SEQ", "323"  },
	{           "SRC_ACC_TYPE", "324" },
	{           "SRC_ACC_ENT_LEVEL", "325" },
	{           "DST_ACC_ENT_COD", "326"  },
	{          "DST_ACC_ENT_ID", "327"},
	{          "DST_ACC_SEQ", "328" },
	{          "DST_ACC_TYPE", "329"},
	{          "DST_ACC_ENT_LEV", "330" },


	{           "ACCOUNT_LEVEL",         "331" },
	{           "UF_UPDATE_CODE",        "332" },
	{           "UF_FILENAME",           "333" },
	{           "UF_PURGE_DATE",         "334" },
	{           "UF_ACTION_CODE",        "335" },
	{           "UF_FILE_RECORD",        "336" },
	{           "DB_CARD_SEQ_NO",        "360" },/*ICH20170927*/
	{           "AUTO_REPLACEMENT_FEE_DTL", "361" },
	{           "HUNTING_SCENARIO",      "362" },/* PROD00069031 */

	{           "SRC_DBSERV_ID",	      "370"},
	{           "START_TIME",			  "371"},
	{           "END_TIME",				  "372"},
	{           "RECONCILIATION_ID	",	  "373"},
	{           "SRC_NB_FIN_AUTHORS",	  "374"},
	{           "SRC_NB_NFIN_AUTHORS",	  "375"},
	{           "SRC_NB_FIN_TRX",		  "376"},
	{           "SRC_NB_PREAUTHS",		  "377"},
	{           "SRC_NB_REVERSALS",		  "378"},
	{           "SRC_NB_SAF_RECORDS",	  "379"},
	{           "SRC_NB_SLF_RECORDS	",	  "380"},
	{           "SRC_TOTAL_FIN_AUTHORS",  "381" },
	{           "SRC_TOTAL_FIN_TRX",	  "382"},
	{           "SRC_TOTAL_PREAUTHS",	  "383"},
	{           "CORPORATE_PARENT",		  "390"},
	{           "DST_NB_SAF_SLF",	      "384"},

	{           "ACTION_FLAG",			"400"	},
	{           "NODE_ID",				"401"	},
	{           "DBSERV_ID",			"402"	},
	{           "CAPTURE_RES_INFO",		"403"	},
	{           "ROUTING_RES_INFO_1",	"404"	},
	{           "ROUTING_RES_INFO_2",	"405"	},
	{           "SWI_SERVER_ID",		"406"	},
	{           "AUTH_SERVER_ID",		"407"	},

	{           "SERV_CONTAINER",				"410"	},
	{           "SERV_PROC_ENTRY",				"411"	},
	{           "SERV_STANDIN_ENTRY",			"412"	},
	{           "SERV_ROUTING_REPLY_ENTRY",		"413"	},
	{           "SERV_ROUTING_TIMEOUT_ENTRY",	"414"	},
	{           "AUTO_SERV_ROUTING_PATH",		"415"	},
	/*{           "SERV_USER_PROC_1_PARAM",		"416"	},*/
	{           "SERV_USER_PROC_1_ENTRY",		"417"	},
	{           "SERV_USER_PROC_1_PARAM",		"418"	},


	{           "CARD_INFO_DATA",		"420"	},
	{           "ISSUER_INFO_DATA",		"421"	},
	{           "TERM_INFO_DATA",		"422"	},
	{           "ACQUIRER_INFO_DATA",	"423"	},
	{           "EMV_CARD_INFO_DATA",	"424"	},
	{           "EMV_CARD_INFO_PARAM",	"425"	},
	{           "CARD_SEC_INFO_DATA",	"426"	},
	{           "CARD_SEC_INFO_PARAM",	"427"	},
	{           "PAN_INFO",				"428"	},
	{           "DYNAMIC_AUTH_DATA",	"430"	},
	{           "DB_BILL_ADDRESS",		"429"	},	 /*PLUTONL-3514*/
	{           "EMV_AC_KEY",			"440"	},
	{           "EMV_SMI_KEY",			"441"	},
	{           "EMV_SMC_KEY",			"442"	},
	{           "PVK",					"443"	},
	{           "CVK",					"444"	},
	{           "DCVV_KEY",				"445"	},
	{           "AAV_KEY",				"446"	},
    {           "CUT_OFF_DATA",         "447"   },/*ICH30171205*/

	{           "DB_PROC_TRACES",		"499"	},
        {           "PVK_V1",           "448"   },/*ZKO 26032019 pvk IBM offset*/
        {           "FRAUD_PARAMS",           "460"   },/*ZKO PROD00069909*/
	{           "MSG_FLOW_MAPPING_ID",   "800" },
	{           "ICC_APPLICATION_INDX",  "900" },

};		


int PrintTlvBuffer ( char *sTlvBuffer )
{

	TSTlvAutho      sTlvInfo;
	int				length;
	int				i,k;
	char			data[LG_MAX];
	char			sLine[LG_MAX];
	char			sLine2[LG_MAX]; /* Mehdi Elyajizi Gestion de retour a la ligne */
	char			sHeader[256];
	char			date [14 + 1];
	char			Mls  [ 4];
	char			Mls_tmp  [ 7]; /**SKA050508 Insure*/
	struct timeval tp;
	char		CardTrunc[22 + 1];
	TSTlvPrivate   TlvPrivateInfo;
	int				nIndice; 
	char			szAscTlvLen [4];


	int		nTotalLength;
	char*	sPtr;
	int		nLine2Offset;
	int		nIter;
	int		nNbBlanks;
	int		nHdrLen;
	int		nNbChar;
	int		nMaxData;
	int     nTlvValue;      /*AMER 20160603*/

	if (trace_level < PROCESSING)
		return(OK);

	trace_event("Start PrintTlvBuffer ()",PROCESSING);

	memset(CardTrunc, 0, sizeof(CardTrunc));
	memset(sLine2, 0, sizeof(sLine2)); /* Mehdi Elyajizi Gestion de retour a la ligne */
	memset(date, 0, sizeof(date));
	memset(Mls, 0, sizeof(Mls));
	memset(Mls_tmp, 0, sizeof(Mls_tmp));/*SKA050508 Insure*/

	/*YK 10g*/
	current_date(date);
	current_hour(date+8);
	gettimeofday(&tp,NULL);
	/*YK 10g*/
	/**SKA050508 Insure*/
	/*sprintf(Mls,"%03d",tp.tv_usec);*/
	sprintf(Mls_tmp,"%06ld",tp.tv_usec);
	memcpy(Mls,Mls_tmp,3);
	


	InitTlvAutho(&sTlvInfo);
	AnalyseTlvAutho( sTlvBuffer , &sTlvInfo );
	memset(data, 0, sizeof(data));

	sprintf(sHeader,"%.2s%.2s %.2s%.2s%.2s%.3s %08d %08lu|%d|",date,date+2,date+8,date+10,date+12,Mls,getpid(),p7_thread_id()/*pthread_self()*/,trace_level);

	sprintf(sLine,"%s -----------------------------------------",sHeader);
	length = strlen(sLine);
	memset(sLine + length,' ',130 - length);
	memcpy(sLine+130,".\n",2);
	trace_line(sLine,PROCESSING);
	/*write(fd_trace,sLine, 132);*/


	sprintf(sLine ,"%s %.3s  %25s  %s %s",sHeader, "TAG","DESC","LEN","VALUE");
	length = strlen(sLine);
	memset(sLine + length,' ',130 - length);
	memcpy(sLine+130,".\n",2);
	trace_line(sLine,PROCESSING);
	/*write(fd_trace,sLine, 132);*/



	sprintf(sLine ,"%s %.3s -%25s- %s %s",sHeader, "---","-----","---","-----");
	length = strlen(sLine);
	memset(sLine + length,' ',130 - length);
	memcpy(sLine+130,".\n",2);
	trace_line(sLine,PROCESSING);
	/*write(fd_trace,sLine, 132);*/


	for (i=0; i<(sizeof(sTlvPrint)/sizeof(sTlvPrint[0])); i++)
	{

		if ( GetTlvAutho(sTlvPrint[i].value,&sTlvInfo,data,&length) != SUCCES )
			continue;

		switch(i)
		{ 

		case 0:		/* Mehdi Elyajizi PCI  CARD ne doit pas apparaitre */
			if(debug_option !=1 )
			{
				TrunCard(data, CardTrunc);
				strcpy(data,CardTrunc);
			}
			break;
		case 46: 
			AnalyseTlvPrivate(data ,length ,&TlvPrivateInfo );
			memset (szAscTlvLen, '\0', 4);
			if(debug_option !=1)
			{
				nIndice = GetTlvPrivateIndice(ENCR_PIN_BLOCK_KEY_TAG);

				if (TlvPrivateInfo.nPresent[nIndice] == PRESENT)
				{
					memcpy ( szAscTlvLen, TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 3,3);
					memset ( TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 6,
						'*', 
						atoi(szAscTlvLen));        
				}
				/*AMER 20160526 Start: PVV_OFFSET_TAG & CVC2_DATA_TAG added*/
				nIndice = GetTlvPrivateIndice(PVV_OFFSET_TAG);

				if (TlvPrivateInfo.nPresent[nIndice] == PRESENT)
				{
					memcpy ( szAscTlvLen, TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 3,3);
					memset ( TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 6,
						'*',
						atoi(szAscTlvLen));
				}

				nIndice = GetTlvPrivateIndice(CVC2_DATA_TAG);

				if (TlvPrivateInfo.nPresent[nIndice] == PRESENT)
				{
					memcpy ( szAscTlvLen, TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 3,3);
					memset ( TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 6,
						'*',
						atoi(szAscTlvLen));
				}
				/*AMER 20160526 End*/
				/*AMER 20160602 Start : Add PVT_DATA_TAG verification*/
				nIndice = GetTlvPrivateIndice(MONEY_TRANSFER_DATA);
				if (TlvPrivateInfo.nPresent[nIndice] == PRESENT)
				{
					memcpy ( szAscTlvLen, TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 3,3);
					memset ( TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 6,
						'*',
						atoi(szAscTlvLen));
				}
				/*AMER 20160602 End*/
			}
			length = PrivateBuildTlv(data,&TlvPrivateInfo);
			data[length]='\0';
			break;				

		case 53: 
			AnalyseTlvPrivate(data ,length ,&TlvPrivateInfo );
			memset (szAscTlvLen, '\0', 4);
			if(debug_option !=1)
			{
				nIndice = GetTlvPrivateIndice(CHIP_T2_EQUIVALENT_DATA);

				if (TlvPrivateInfo.nPresent[nIndice] == PRESENT)
				{
					memcpy ( szAscTlvLen, TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 3,3);
					memset ( TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 6,
						'*', 
						atoi(szAscTlvLen));        
				}

				/*AMER 20160603 Start: Add CHIP_TRACK2_TAG and CHIP_PAN_TAG verification*/
				nIndice = GetTlvPrivateIndice(CHIP_TRACK2_TAG);

				if (TlvPrivateInfo.nPresent[nIndice] == PRESENT)
				{
					memcpy ( szAscTlvLen, TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 3,3);
					memset ( TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 6,
						'*',
						atoi(szAscTlvLen));
				}

				nIndice = GetTlvPrivateIndice(CHIP_PAN_TAG);

				if (TlvPrivateInfo.nPresent[nIndice] == PRESENT)
				{
					memcpy ( szAscTlvLen, TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 3,3);
					memset ( TlvPrivateInfo.sTlvData + TlvPrivateInfo.nPosTlv[nIndice] + 6,
						'*',
						atoi(szAscTlvLen));
				}
				/*AMER 20160603 End*/
			}
			length = PrivateBuildTlv(data,&TlvPrivateInfo);
			data[length]='\0';
			break;	
		case 43: /* Mehdi Elyajizi PCI  Piste 1 ne doit pas apparaitre */
		case 50: /* Mehdi Elyajizi PCI  Pin block ne doit pas apparaitre */
		case 33: 
		case 239:
			if(debug_option !=1)
			{
				strcpy(data,"************************");
			}
			break;
			/*AMER 20160729 Start: PADSS Fix*/
		case 100:
		case 101:
			if(debug_option !=1)
			{
				strcpy(data,"**********");
			}
			break;
			/*AMER 20160729 End: PADSS Fix*/
		default: /* Mehdi Elyajizi PCI  Pin block ne doit pas apparaitre */
			/*AMER 20160603 Start*/
			nTlvValue = atoi(sTlvPrint[i].value);
			switch(nTlvValue)
			{	
			case 322:	/* Source acc Id may contain card number */         
				if(debug_option !=1 )
				{
					TrunCard(data, CardTrunc);
					strcpy(data,CardTrunc);
				}
				break;
			case 327:       /* Destination account ID*/
				if(debug_option !=1 )
				{
					TrunCard(data, CardTrunc);
					strcpy(data,CardTrunc);
				}
				break;
			case 426:
			case 440:
			case 441:
			case 442:
			case 443:
			case 444:
			case 445:
			case 446:
				if(debug_option !=1)
				{
					strcpy(data,"************************");
				}
				break;
			}
			/*AMER 20160603 End*/
		}

		/*sprintf ( sLine ,"%s %.3s\t{%25s}\t%03d\t%s",sHeader,*/
		sprintf ( sLine ,"%s %.3s {%25s} %03d %s",sHeader,
			sTlvPrint[i].value,
			sTlvPrint[i].description,
			length,
			data );



		sLine2[0]='\0';
		if( strlen(sLine) > 130)
		{

			nTotalLength = strlen(sLine) - 130;

			if( nTotalLength > 2000 )
				nTotalLength = 2000;

			nHdrLen = strlen(sHeader);
			nMaxData = 130 - nHdrLen;

			nLine2Offset = 0;
			sPtr = sLine + 130;



			do {
				if( nTotalLength > nMaxData )
				{
					nNbChar = nMaxData;
				}
				else
				{
					nNbChar = nTotalLength;
				}
				memcpy(sLine2 + nLine2Offset ,sHeader,nHdrLen); nLine2Offset+=nHdrLen;
				memcpy(sLine2 + nLine2Offset ,sPtr ,nNbChar); nLine2Offset+=nNbChar;
				sPtr+=nNbChar;
				nTotalLength -= nNbChar;
				if( nHdrLen + nNbChar < 130 )
				{
					nNbBlanks = 130 - (nHdrLen + nNbChar);
					memset(sLine2 + nLine2Offset,' ',nNbBlanks);
					nLine2Offset+=nNbBlanks;
				}
				memcpy(sLine2 + nLine2Offset,".\n",2); nLine2Offset+=2;
			} while (nTotalLength > 0 );
			/**
			strcpy(sLine2,sHeader);
			strcpy(sLine2 + strlen(sLine2), sLine + 130);
			length = strlen(sLine2);
			if( length > 129 )
			length = 129;
			memset(sLine2 + length,' ',130 - length);
			memcpy(sLine2 + 130,".\n",2);
			**/
		}
		else if(strlen(sLine) < 130 )
		{
			length = strlen(sLine);
			memset(sLine + length,' ',130 - length);
		}

		memcpy(sLine+130,".\n",2);
		trace_line(sLine,PROCESSING);
		/*write(fd_trace,sLine, 132);*/

		if( strlen(sLine2) > 0)
		{
			/*write(fd_trace,sLine2, nLine2Offset);*/
			trace_line(sLine2,PROCESSING);
		}

	}


	sprintf(sLine,"%s -----------------------------------------",sHeader);
	length = strlen(sLine);
	memset(sLine + length,' ',130 - length);
	memcpy(sLine+130,".\n",2);
	trace_line(sLine,PROCESSING);
	/*write(fd_trace,sLine, 132);*/


	InitTlvAutho(&sTlvInfo); /**PA DSS**/
	memset(data, 0, sizeof(data)); /** PA DSS*/
	memset(sLine, 0, sizeof(sLine));/***PA DSS*/
	memset(sLine2, 0, sizeof(sLine2)); /***PA DSS*/
	InitTlvPrivate( &TlvPrivateInfo ); /* PA DSS */

	trace_event("End   PrintTlvBuffer ()",PROCESSING);
	/*SKO290509*/ 
	return(OK);
}


void dump_raw_tlv_buffer(char* sTlvBuffer)
{
	char			sHeader[256];
	
	int 	nTraceLevel;
	int		i;
	char			date [14 + 1];
	char			Mls  [ 4];
	char			Mls_tmp  [ 7];
	char  			sLLLL [ 4 + 1];
	char			sLine2[MAX_LINE_TRC];
	int				nTotalLength;
	int   			nLLLL;
	int				nLenStrlen;
	int				nHdrLen;
	int				nLine2Offset;
	int				nMaxData;
	int				nNbChar;
	int				nNbBlanks;
	
	char*			sPtr;
	
	struct timeval tp;


	trace_event("Start dump_raw_tlv_buffer ()",ERROR);
	
	memset(sHeader,'\0',sizeof(sHeader));
	memset(date,'\0',sizeof(date));
	memset(Mls,'\0',sizeof(Mls));
	memset(Mls_tmp,'\0',sizeof(Mls_tmp));
	
	nTraceLevel = ERROR;
	
	current_date(date);
	current_hour(date+8);
	gettimeofday(&tp,NULL);
	sprintf(Mls_tmp,"%06ld",tp.tv_usec);
	memcpy(Mls,Mls_tmp,3);


	sprintf(sHeader,"%.2s%.2s %.2s%.2s%.2s%.3s %08d %08lu|%d|[",date,date+2,date+8,date+10,date+12,Mls,getpid(),p7_thread_id(),nTraceLevel);

	memcpy ( sLLLL , sTlvBuffer 	 , 4 );
	nLLLL   = atoi ( sLLLL );	
	nLenStrlen = strlen(sTlvBuffer) - 4;
	if( nLLLL >= nLenStrlen )
	{
		nTotalLength = nLLLL;
	}
	else
	{
		nTotalLength = nLenStrlen;
	}
	
	if( nLLLL != nLenStrlen )
	{
		sprintf(sLine2,">>> TLV length %d vs strlen=%d", nLLLL,nLenStrlen);
		trace_event(sLine2, nTraceLevel );
	}
		
	nHdrLen = strlen(sHeader);
	nMaxData = 129 - nHdrLen;

	
	sPtr = sTlvBuffer;
	nTotalLength += 4;
	
	do {
		if( nTotalLength > nMaxData )
		{
			nNbChar = nMaxData;
		}
		else
		{
			nNbChar = nTotalLength;
		}
		
		nLine2Offset = 0;
		memcpy(sLine2 + nLine2Offset ,sHeader,nHdrLen); nLine2Offset+=nHdrLen;
		memcpy(sLine2 + nLine2Offset ,sPtr ,nNbChar); nLine2Offset+=nNbChar;
		
		sPtr+=nNbChar;
		
		nTotalLength -= nNbChar;
		if( nHdrLen + nNbChar < 129 )
		{
			nNbBlanks = 129 - (nHdrLen + nNbChar);
			memset(sLine2 + nLine2Offset,' ',nNbBlanks);
			nLine2Offset+=nNbBlanks;
		}

		memcpy(sLine2 + nLine2Offset,"].\n",3); nLine2Offset+=3;
		sLine2[nLine2Offset]='\0';
		trace_line(sLine2,nTraceLevel);
	} while (nTotalLength > 0 );

	
	trace_event("End   dump_raw_tlv_buffer ()",nTraceLevel);
	return; /* ELKAM20221031 PLUTONL-5358 */
	
}
