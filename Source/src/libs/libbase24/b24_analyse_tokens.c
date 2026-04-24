/*#include <base24.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <tools.h>
#include <base24/iso_base24.h>
#include <base24/base24_tokens.h>
/******************************************************************************/
#define TK_PRESENT			1
#define TK_NOT_PRESENT		0
/******************************************************************************/

static const token_info_type		tab_Token_BM_InfoType[] = {
	{B24_TK_BM_OFF_TXN_SUBTYPE,					4,	"B24_TK_BM_TXN_SUBTYPE"		},
	{B24_TK_BM_OFF_ACQ_PROC_CODE,				6,	"B24_TK_BM_ACQ_PROC_CODE"	},
	{B24_TK_BM_OFF_ISS_PROC_CODE,				6,	"B24_TK_BM_ISS_PROC_CODE"	},
	{B24_TK_BM_OFF_USR_FDL_1,					20,	"B24_TK_BM_USR_FDL_1"		},			/*POS Acq*/
	{B24_TK_BM_OFF_THIRD_PARTY_PHONE_NO,		10,	"B24_TK_BM_THIRD_PARTY_PHONE_NO"	},	/* ATM */
	{B24_TK_BM_OFF_RECIPIENT_PHONE_NO,			10,	"B24_TK_BM_RECIPIENT_PHONE_NO"		},
};

static const token_info_type		tab_Token_BP_InfoType[] = {
	{B24_TK_BP_OFF_INTENDED_USE_FLAG,			1,	"B24_TK_BP_INTENDED_USE_FLAG"	},
	{B24_TK_BP_OFF_BUSINESS_USE_FLAG,			1,	"B24_TK_BP_BUSINESS_USE_FLAG"	},
	{B24_TK_BP_OFF_TRACE_NUMBER,				15,	"B24_TK_BP_TRACE_NUMBER"		},
	{B24_TK_BP_OFF_THIRD_PARTY_ID,				60,	"B24_TK_BP_THIRD_PARTY_ID"		},
	{B24_TK_BP_OFF_THIRD_PARTY_ACC_NO,			25,	"B24_TK_BP_THIRD_PARTY_ACC_NO"	},
	{B24_TK_BP_OFF_THIRD_PARTY_PHONE_NO,		25,	"B24_TK_BP_THIRD_PARTY_PHONE_NO"},
	{B24_TK_BP_OFF_THIRD_PARTY_INVOICE_NO,		25,	"B24_TK_BP_THIRD_PARTY_INVOICE_NO"},
	{B24_TK_BP_OFF_THIRD_PARTY_EMAIL_ADDR,		60,	"B24_TK_BP_THIRD_PARTY_EMAIL_ADDR"},
	{B24_TK_BP_OFF_THIRD_PARTY_DESCRIPTION,		50,	"B24_TK_BP_THIRD_PARTY_DESCRIPTION"},
	{B24_TK_BP_OFF_RECIPIENT_EMAIL_ADDR,		60,	"B24_TK_BP_RECIPIENT_EMAIL_ADDR"},
	{B24_TK_BP_OFF_RECIPIENT_PHONE_NO,			40,	"B24_TK_BP_RECIPIENT_PHONE_NO"	},
	{B24_TK_BP_OFF_RECIPIENT_USR_FLD_1,			20,	"B24_TK_BP_RECIPIENT_USR_FLD_1"	},
};

static const token_info_type		tab_Token_B2_InfoType[] = {
	{B24_TK_B2_OFF_BITMAP,						4,	"B24_TK_B2_BITMAP"			},
	{B24_TK_B2_OFF_USR_FLD_1,					4,	"B24_TK_B2_USR_FLD_1"		},
	{B24_TK_B2_OFF_CRYPTO_INFO_DATA,			2,	"B24_TK_B2_CRYPTO_INFO_DATA"},
	{B24_TK_B2_OFF_TVR,							10,	"B24_TK_B2_TVR"				},
	{B24_TK_B2_OFF_ARQC,						16,	"B24_TK_B2_ARQC"			},
	{B24_TK_B2_OFF_AMOUNT_AUTH,					12,	"B24_TK_B2_AMOUNT_AUTH"		},
	{B24_TK_B2_OFF_AMOUNT_OTHER,				12,	"B24_TK_B2_AMOUNT_OTHER"	},
	{B24_TK_B2_OFF_AIP,							4,	"B24_TK_B2_AIP"				},
	{B24_TK_B2_OFF_ATC,							4,	"B24_TK_B2_ATC"				},
	{B24_TK_B2_OFF_TERM_COUNTRY_CODE,			3,	"B24_TK_B2_TERM_COUNTRY_CODE"},
	{B24_TK_B2_OFF_TRX_CURRENCY_CODE,			3,	"B24_TK_B2_TRX_CURRENCY_CODE"},
	{B24_TK_B2_OFF_TRX_DATE,					6,	"B24_TK_B2_TRX_DATE"		},
	{B24_TK_B2_OFF_TRX_TYPE,					2,	"B24_TK_B2_TRX_TYPE"		},
	{B24_TK_B2_OFF_UN,							8,	"B24_TK_B2_UN"				},
	{B24_TK_B2_OFF_ISS_APPL_DATA_LEN,			4,	"B24_TK_B2_ISS_APPL_DATA_LEN"},
	{B24_TK_B2_OFF_ISS_APPL_DATA,				64,	"B24_TK_B2_ISS_APPL_DATA"	},

	{B24_TK_B2_OFF_VISA_IAD,					64,	"B24_TK_B2_VISA_IAD"		},
	{B24_TK_B2_OFF_VISA_IAD_LEN,				2,	"B24_TK_B2_VISA_IAD_LEN"	},
	{B24_TK_B2_OFF_VISA_IAD_DKI,				2,	"B24_TK_B2_VISA_IAD_DKI"	},
	{B24_TK_B2_OFF_VISA_IAD_CVN,				2,	"B24_TK_B2_VISA_IAD_CVN"	},
	{B24_TK_B2_OFF_VISA_IAD_CVR,				8,	"B24_TK_B2_VISA_IAD_CVR"	},
	{B24_TK_B2_OFF_VISA_IAD_INFO,				50,	"B24_TK_B2_VISA_IAD_INFO"	},


	{B24_TK_B2_OFF_MCHIP_SEL2_IAD,				64,	"B24_TK_B2_MCHIP_SEL2_IAD"		},
	{B24_TK_B2_OFF_MCHIP_SEL2_IAD_LEN,			2,	"B24_TK_B2_MCHIP_SEL2_IAD_LEN"	},
	{B24_TK_B2_OFF_MCHIP_SEL2_IAD_DKI,			2,	"B24_TK_B2_MCHIP_SEL2_IAD_DKI"	},
	{B24_TK_B2_OFF_MCHIP_SEL2_IAD_CVN,			2,	"B24_TK_B2_MCHIP_SEL2_IAD_CVN"	},
	{B24_TK_B2_OFF_MCHIP_SEL2_IAD_CVR,			8,	"B24_TK_B2_MCHIP_SEL2_IAD_CVR"	},
	{B24_TK_B2_OFF_MCHIP_SEL2_IAD_DAC,			4,	"B24_TK_B2_MCHIP_SEL2_IAD_DAC"	},
	{B24_TK_B2_OFF_MCHIP_SEL2_IAD_INFO,			46,	"B24_TK_B2_MCHIP_SEL2_IAD_INFO"	},

	{B24_TK_B2_OFF_MCHIP_LT21_IAD,				64,	"B24_TK_B2_MCHIP_LT21_IAD"		},
	{B24_TK_B2_OFF_MCHIP_LT21_IAD_DKI,			2,	"B24_TK_B2_MCHIP_LT21_IAD_DKI"	},
	{B24_TK_B2_OFF_MCHIP_LT21_IAD_CVN,			2,	"B24_TK_B2_MCHIP_LT21_IAD_CVN"	},
	{B24_TK_B2_OFF_MCHIP_LT21_IAD_CVR,			8,	"B24_TK_B2_MCHIP_LT21_IAD_CVR"	},
	{B24_TK_B2_OFF_MCHIP_LT21_IAD_DAC,			4,	"B24_TK_B2_MCHIP_LT21_IAD_DAC"	},
	{B24_TK_B2_OFF_MCHIP_LT21_IAD_INFO,			48,	"B24_TK_B2_MCHIP_LT21_IAD_INFO"	},

	{B24_TK_B2_OFF_JCB_IAD,						64,	"B24_TK_B2_JCB_IAD"				},
	{B24_TK_B2_OFF_JCB_IAD_LEN,					2,	"B24_TK_B2_JCB_IAD_LEN"			},
	{B24_TK_B2_OFF_JCB_IAD_DKI,					2,	"B24_TK_B2_JCB_IAD_DKI"			},
	{B24_TK_B2_OFF_JCB_IAD_CVN,					2,	"B24_TK_B2_JCB_IAD_CVN"			},
	{B24_TK_B2_OFF_JCB_IAD_CVR,					10,	"B24_TK_B2_JCB_IAD_CVR"			},
	{B24_TK_B2_OFF_JCB_IAD_INFO,				48,	"B24_TK_B2_JCB_IAD_INFO"		},

	{B24_TK_B2_OFF_MCHIP4_IAD,					64,	"B24_TK_B2_MCHIP4_IAD"			},
	{B24_TK_B2_OFF_MCHIP4_IAD_DKI,				2,	"B24_TK_B2_MCHIP4_IAD_DKI"		},
	{B24_TK_B2_OFF_MCHIP4_IAD_CVN,				2,	"B24_TK_B2_MCHIP4_IAD_CVN"		},
	{B24_TK_B2_OFF_MCHIP4_IAD_CVR,				12,	"B24_TK_B2_MCHIP4_IAD_CVR"		},
	{B24_TK_B2_OFF_MCHIP4_IAD_DAC,				4,	"B24_TK_B2_MCHIP4_IAD_DAC"		},
	{B24_TK_B2_OFF_MCHIP4_IAD_CNTR,				16,	"B24_TK_B2_MCHIP4_IAD_CNTR"		},
	{B24_TK_B2_OFF_MCHIP4_IAD_INFO,				28,	"B24_TK_B2_MCHIP4_IAD_INFO"		},

};

static const token_info_type		tab_Token_B3_InfoType[] = {
	{B24_TK_B3_OFF_BITMAP,						4,	"B24_TK_B3_BITMAP"			},
	{B24_TK_B3_OFF_IFD_SERIAL_NO,				8,	"B24_TK_B3_IFD_SERIAL_NO"	},
/*START NAB08102020 PLUTONL-2288*/
#if defined(B24_PROSA) 
	{B24_TK_B3_OFF_EMV_TERM_CAPABILITIES,		8,	"B24_TK_B3_EMV_TERM_CAPABILITIES"},
	{B24_TK_B3_OFF_USR_FLD_1,					4,	"B24_TK_B3_USR_FLD_1"		},
#else
	{B24_TK_B3_OFF_EMV_TERM_CAPABILITIES,		6,	"B24_TK_B3_EMV_TERM_CAPABILITIES"},
	{B24_TK_B3_OFF_USR_FLD_1,					6,	"B24_TK_B3_USR_FLD_1"		},
#endif
/*END NAB08102020 PLUTONL-2288*/
	{B24_TK_B3_OFF_USR_FLD_2,					8,	"B24_TK_B3_USR_FLD_2"		},
	{B24_TK_B3_OFF_EMV_TERM_TYPE,				2,	"B24_TK_B3_EMV_TERM_TYPE"	},
	{B24_TK_B3_OFF_APPL_VER_NO,					4,	"B24_TK_B3_APPL_VER_NO"		},
	{B24_TK_B3_OFF_CVM_RESULTS,					6,	"B24_TK_B3_CVM_RESULTS"		},
	{B24_TK_B3_OFF_DF_NAME_LEN,					4,	"B24_TK_B3_DF_NAME_LEN"		},
	{B24_TK_B3_OFF_DF_NAME,						32,	"B24_TK_B3_DF_NAME"			},

};



static const token_info_type		tab_Token_B4_InfoType[] = {
	{B24_TK_B4_OFF_POS_ENTRY_MODE,				3,	"B24_TK_B4_POS_ENTRY_MODE"	},
	{B24_TK_B4_OFF_TERM_CAPABILITIES,			1,	"B24_TK_B4_TERM_CAPABILITIES"},
	{B24_TK_B4_OFF_LAST_EMV_STAT,				1,	"B24_TK_B4_LAST_EMV_STAT"	},
	{B24_TK_B4_OFF_DATA_SUSPECT,				1,	"B24_TK_B4_DATA_SUSPECT"	},
	{B24_TK_B4_OFF_APPL_PAN_SEQ_NO,				2,	"B24_TK_B4_APPL_PAN_SEQ_NO"	},
	{B24_TK_B4_OFF_CAM_FLAGS,					6,	"B24_TK_B4_CAM_FLAGS"		},
	{B24_TK_B4_OFF_CVM_RESULTS,					6,	"B24_TK_B4_CVM_RESULTS"		},
	{B24_TK_B4_OFF_ICHG_DEF,					6,	"B24_TK_B4_ICHG_DEF"		},
	{B24_TK_B4_OFF_REASON_ONLINE_CODE,			4,	"B24_TK_B4_REASON_ONLINE_CODE"},
	{B24_TK_B4_OFF_ARQC_VERIFY,					1,	"B24_TK_B4_ARQC_VERIFY"		},
	{B24_TK_B4_OFF_ISO_RC_IND,					1,	"B24_TK_B4_ISO_RC_IND"		},
};

static const token_info_type		tab_Token_B5_InfoType[] = {
	{B24_TK_B5_OFF_ISS_AUTH_DATA_LEN,			4,	"B24_TK_B5_ISS_AUTH_DATA_LEN"},
	{B24_TK_B5_OFF_ISS_AUTH_DATA,				32,	"B24_TK_B5_ISS_AUTH_DATA"},
	{B24_TK_B5_OFF_ARPC,						16,	"B24_TK_B5_ARPC"			},
	{B24_TK_B5_OFF_VISA_ISS_RESP_CODE,			4,	"B24_TK_B5_VISA_ISS_RESP_CODE"},
	{B24_TK_B5_OFF_VISA_INFO,					12,	"B24_TK_B5_VISA_INFO"		},
	{B24_TK_B5_OFF_MCPA_ISS_AUTH_RESP_CODE,		4,	"B24_TK_B5_MCPA_ISS_AUTH_RESP_CODE"},
	{B24_TK_B5_OFF_MCPA_INFO,					12,	"B24_TK_B5_MCPA_INFO"		},
	{B24_TK_B5_OFF_MCHIP4_ARPC_RESP_CODE,		4,	"B24_TK_B5_MCHIP4_ARPC_RESP_CODE"},
	{B24_TK_B5_OFF_MCHIP4_INFO,					12,	"B24_TK_B5_MCHIP4_INFO"	},
	
};

static const token_info_type		tab_Token_B6_InfoType[] = {
	{B24_TK_B6_OFF_ISS_SCRIPT_DATA_LEN,			4,		"B24_TK_B6_ISS_SCRIPT_DATA_LEN"},
	{B24_TK_B6_OFF_ISS_SCRIPT_DATA,				256,	"B24_TK_B6_ISS_SCRIPT_DATA"},
};

static const token_info_type		tab_Token_BJ_InfoType[] = {
	{B24_TK_BJ_OFF_NUM_ISS_SCRIPT_RESULTS,		1,	"B24_TK_BJ_NUM_ISS_SCRIPT_RESULTS"},
	{B24_TK_BJ_OFF_USR_FLD_1,					1,	"B24_TK_BJ_USR_FLD_1"		},
	{B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_1,	1,	"B24_TK_BJ_ISS_SCRIPT_PROC_RESULT"},
	{B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_1,			1,	"B24_TK_BJ_ISS_SCRIPT_SEQ"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_ID_1,				8,	"B24_TK_BJ_ISS_SCRIPT_ID"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_2,	1,	"B24_TK_BJ_ISS_SCRIPT_PROC_RESULT"},
	{B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_2,			1,	"B24_TK_BJ_ISS_SCRIPT_SEQ"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_ID_2,				8,	"B24_TK_BJ_ISS_SCRIPT_ID"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_3,	1,	"B24_TK_BJ_ISS_SCRIPT_PROC_RESULT"},
	{B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_3,			1,	"B24_TK_BJ_ISS_SCRIPT_SEQ"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_ID_3,				8,	"B24_TK_BJ_ISS_SCRIPT_ID"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_4,	1,	"B24_TK_BJ_ISS_SCRIPT_PROC_RESULT"},
	{B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_4,			1,	"B24_TK_BJ_ISS_SCRIPT_SEQ"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_ID_4,				8,	"B24_TK_BJ_ISS_SCRIPT_ID"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_5,	1,	"B24_TK_BJ_ISS_SCRIPT_PROC_RESULT"},
	{B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_5,			1,	"B24_TK_BJ_ISS_SCRIPT_SEQ"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_ID_5,				8,	"B24_TK_BJ_ISS_SCRIPT_ID"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_6,	1,	"B24_TK_BJ_ISS_SCRIPT_PROC_RESULT"},
	{B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_6,			1,	"B24_TK_BJ_ISS_SCRIPT_SEQ"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_ID_6,				8,	"B24_TK_BJ_ISS_SCRIPT_ID"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_7,	1,	"B24_TK_BJ_ISS_SCRIPT_PROC_RESULT"},
	{B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_7,			1,	"B24_TK_BJ_ISS_SCRIPT_SEQ"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_ID_7,				8,	"B24_TK_BJ_ISS_SCRIPT_ID"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_8,	1,	"B24_TK_BJ_ISS_SCRIPT_PROC_RESULT"},
	{B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_8,			1,	"B24_TK_BJ_ISS_SCRIPT_SEQ"	},
	{B24_TK_BJ_OFF_ISS_SCRIPT_ID_8,				8,	"B24_TK_BJ_ISS_SCRIPT_ID"	},

};

/*********************[POS ISS]*************************************************/
static const token_info_type		tab_Token_01_InfoType[] = {
	{B24_TK_01_OFF_ADDR,					20,	"B24_TK_01_ADDR"		},
	{B24_TK_01_OFF_ZIP_CODE,				9,	"B24_TK_01_ZIP_CODE"	},
	{B24_TK_01_OFF_USR_FLD_1,				1,	"B24_TK_01_USR_FLD_1"	},
};



static const token_info_type		tab_Token_04_InfoType[] = {
	{B24_TK_04_OFF_ERROR_FLAG,				1,	"B24_TK_04_ERROR_FLAG"		},
	{B24_TK_04_OFF_ROUTING_GROUP,			11,	"B24_TK_04_ROUTING_GROUP"	},
	{B24_TK_04_OFF_CARD_VERIF_FLAG,			1,	"B24_TK_04_CARD_VERIF_FLAG"	},
	{B24_TK_04_OFF_CITY_EXT,				5,	"B24_TK_04_CITY_EXT"		},
	{B24_TK_04_OFF_TRACK2_CAP,				1,	"B24_TK_04_TRACK2_CAP"		},
	{B24_TK_04_OFF_UAF_FLAG,				1,	"B24_TK_04_UAF_FLAG"		},
};


static const token_info_type		tab_Token_C0_InfoType[] = {
	{B24_TK_C0_OFF_CVD_FIELD,				4,	"B24_TK_C0_CVD_FIELD"		},	
	{B24_TK_C0_OFF_RESUB_STATUS,			1,	"B24_TK_C0_RESUB_STATUS"	},
	{B24_TK_C0_OFF_RESUB_CNT,				3,	"B24_TK_C0_RESUB_CNT"		},	
	{B24_TK_C0_OFF_TERM_POSTAL_CODE,		10,	"B24_TK_C0_TERM_POSTAL_CODE"},	
	{B24_TK_C0_OFF_ECOM_FLAG,				1,	"B24_TK_C0_ECOM_FLAG"		},	
/*START NAB08102020 PLUTONL-2288*/
#if !defined(B24_PROSA) 
	{B24_TK_C0_OFF_MOTO_FLAG,				1,	"B24_TK_C0_MOTO_FLAG"		},
#endif
/*END NAB08102020 PLUTONL-2288*/
	{B24_TK_C0_OFF_COM_CARD_TYPE,			1,	"B24_TK_C0_COM_CARD_TYPE"	},	
	{B24_TK_C0_OFF_ADNL_DATA_IND,			1,	"B24_TK_C0_ADNL_DATA_IND"	},	
	{B24_TK_C0_OFF_CVD_FLD_PRESENT,			1,	"B24_TK_C0_CVD_FLD_PRESENT"	},
	{B24_TK_C0_OFF_SAF_OR_FORCE_POST,		1,	"B24_TK_C0_SAF_OR_FORCE_POST"		},	
	{B24_TK_C0_OFF_AUTH_COLL_IND,			1,	"B24_TK_C0_AUTH_COLL_IND"	},	
	{B24_TK_C0_OFF_FRD_PRN_FLG,				1,	"B24_TK_C0_FRD_PRN_FLG"		},	
	{B24_TK_C0_OFF_CAVV_AAV_RESULT,			1,	"B24_TK_C0_CAVV_AAV_RESULT"	},
};


static const token_info_type		tab_Token_C4_InfoType[] = {
	{B24_TK_C4_OFF_TERM_ATTEND_IND,			1,	"B24_TK_C4_TERM_ATTEND_IND"		},
	{B24_TK_C4_OFF_TERM_OPER_IND,			1,	"B24_TK_C4_TERM_OPER_IND"		},
	{B24_TK_C4_OFF_TERM_LOC_IND,			1,	"B24_TK_C4_TERM_LOC_IND"		},
	{B24_TK_C4_OFF_CH_PRESENT_IND,			1,	"B24_TK_C4_CH_PRESENT_IND"		},
	{B24_TK_C4_OFF_CARD_PRESENT_IND,		1,	"B24_TK_C4_CARD_PRESENT_IND"	},
	{B24_TK_C4_OFF_CARD_CAPTURE_IND,		1,	"B24_TK_C4_CARD_CAPTURE_IND"	},
	{B24_TK_C4_OFF_TRX_STAT_IND,			1,	"B24_TK_C4_TRX_STAT_IND"		},
	{B24_TK_C4_OFF_TRX_SEC_IND,				1,	"B24_TK_C4_TRX_SEC_IND"			},
	{B24_TK_C4_OFF_TRX_RTN_IND,				1,	"B24_TK_C4_TRX_RTN_IND"			},
	{B24_TK_C4_OFF_CH_ACTVT_TERM_IND,		1,	"B24_TK_C4_CH_ACTVT_TERM_IND"	},
	{B24_TK_C4_OFF_TERM_INPUT_CAP_IND,		1,	"B24_TK_C4_TERM_INPUT_CAP_IND"	},
	{B24_TK_C4_OFF_CH_ID_METHOD,			1,	"B24_TK_C4_CH_ID_METHOD"		},
};


static const token_info_type		tab_Token_BE_InfoType[] = {
	{B24_TK_BE_OFF_AMOUNT,					19,	"B24_TK_BE_AMOUNT"			},
	{B24_TK_BE_OFF_ADD_AMOUNT,				19,	"B24_TK_BE_ADD_AMOUNT"		},
	{B24_TK_BE_OFF_CURRENCY,				3,	"B24_TK_BE_CURRENCY"		},
	{B24_TK_BE_OFF_CONV_RATE,				8,	"B24_TK_BE_CONV_RATE"		},
	{B24_TK_BE_OFF_CONV_DATE,				4,	"B24_TK_BE_CONV_DATE"		},
	{B24_TK_BE_OFF_USR_FLD_1,				9,	"B24_TK_BE_USR_FLD_1"		},
};



static const token_info_type		tab_Token_PF_InfoType[] = {
	{B24_TK_PF_OFF_PP_PROV_FIID,			4,	"B24_TK_PF_PP_PROV_FIID"	},
	{B24_TK_PF_OFF_PP_TYPE,					4,	"B24_TK_PF_PP_TYPE"			},
	{B24_TK_PF_OFF_PP_DENOM,				12,	"B24_TK_PF_PP_DENOM"		},
	{B24_TK_PF_OFF_PP_CTRL_NO,				15,	"B24_TK_PF_PP_CTRL_NO"		},
	{B24_TK_PF_OFF_USR_FLD_1,				1,	"B24_TK_PF_USR_FLD_1"		},
};


static const token_info_type		tab_Token_PG_InfoType[] = {
	{B24_TK_PG_OFF_PP_OP_CODE,				8,	"B24_TK_PG_PP_OP_CODE"		},
	{B24_TK_PG_OFF_PP_PIN_ACCESS_NO,		16,	"B24_TK_PG_PP_PIN_ACCESS_NO"},
	{B24_TK_PG_OFF_PP_PIN_LEN,				2,	"B24_TK_PG_PP_PIN_LEN"		},
};


static const token_info_type		tab_Token_P1_InfoType[] = {
	{B24_TK_P1_OFF_OP_CODE,					2,	"B24_TK_P1_OP_CODE"			},
	{B24_TK_P1_OFF_PHONE_NBR_FROM,			10,	"B24_TK_P1_PHONE_NBR_FROM"	},
	{B24_TK_P1_OFF_PHONE_NBR_TO,			10,	"B24_TK_P1_PHONE_NBR_TO"	},
	{B24_TK_P1_OFF_BIRTHDAY,				8,	"B24_TK_P1_BIRTHDAY"		},
	{B24_TK_P1_OFF_ACC_TYPE,				2,	"B24_TK_P1_ACC_TYPE"		},
	{B24_TK_P1_OFF_OP_CARD,					19,	"B24_TK_P1_OP_CARD"			},
	{B24_TK_P1_OFF_ATH_MOVIL_STATUS,		2,	"B24_TK_P1_ATH_MOVIL_STATUS"},
	{B24_TK_P1_OFF_CARD_AFF_NBR,			1,	"B24_TK_P1_CARD_AFF_NBR"	},
	{B24_TK_P1_OFF_CARD_NBR_1,				19,	"B24_TK_P1_CARD_NBR_1"		},
	{B24_TK_P1_OFF_ACC_TYPE_1,				2,	"B24_TK_P1_ACC_TYPE_1"		},
	{B24_TK_P1_OFF_CARD_NBR_2,				19,	"B24_TK_P1_CARD_NBR_2"		},
	{B24_TK_P1_OFF_ACC_TYPE_2,				2,	"B24_TK_P1_ACC_TYPE_2"		},
	{B24_TK_P1_OFF_CARD_NBR_3,				19,	"B24_TK_P1_CARD_NBR_3"		},
	{B24_TK_P1_OFF_ACC_TYPE_3,				2,	"B24_TK_P1_ACC_TYPE_3"		},
	{B24_TK_P1_OFF_CARD_NBR_4,				19,	"B24_TK_P1_CARD_NBR_4"		},
	{B24_TK_P1_OFF_ACC_TYPE_4,				2,	"B24_TK_P1_ACC_TYPE_4"		},
	{B24_TK_P1_OFF_CARD_NBR_5,				19,	"B24_TK_P1_CARD_NBR_5"		},
	{B24_TK_P1_OFF_ACC_TYPE_5,				2,	"B24_TK_P1_ACC_TYPE_5"		},
	{B24_TK_P1_OFF_LANGUAGE,				1,	"B24_TK_P1_LANGUAGE"		},
};


static const token_info_type		tab_Token_06_InfoType[] = {
	{B24_TK_06_OFF_NEW_PIN_FORMAT,			1,	"B24_TK_06_NEW_PIN_FORMAT"		},
	{B24_TK_06_OFF_NEW_PIN_OFFSET,			16,	"B24_TK_06_NEW_PIN_OFFSET"		},
	{B24_TK_06_OFF_NEW_PIN_COUNT,			1,	"B24_TK_06_NEW_PIN_COUNT"		},
	{B24_TK_06_OFF_NEW_PIN_SIZE,			2,	"B24_TK_06_NEW_PIN_SIZE"		},
	{B24_TK_06_OFF_NEW_PIN_1,				16,	"B24_TK_06_NEW_PIN_1"			},
	{B24_TK_06_OFF_NEW_PIN_2,				16,	"B24_TK_06_NEW_PIN_2"			},

};


static const token_info_type		tab_Token_PZ_InfoType[] = {
	{B24_TK_PZ_OFF_RSRV_RCPT_FIID,			4,	"B24_TK_PZ_RSRV_RCPT_FIID"		},
	{B24_TK_PZ_OFF_RSRV_DSPLY_BAL,			18,	"B24_TK_PZ_RSRV_DSPLY_BAL"		},
	{B24_TK_PZ_OFF_RSRV_AVAIL_BAL,			18,	"B24_TK_PZ_RSRV_AVAIL_BAL"		},
	{B24_TK_PZ_OFF_RSRV_LEDGER_BAL,			18,	"B24_TK_PZ_RSRV_LEDGER_BAL"		},
	{B24_TK_PZ_OFF_CHKG_DSPLY_BAL,			18,	"B24_TK_PZ_CHKG_DSPLY_BAL"		},
	{B24_TK_PZ_OFF_CHKG_AVAIL_BAL,			18,	"B24_TK_PZ_CHKG_AVAIL_BAL"		},
	{B24_TK_PZ_OFF_CHKG_LEDGER_BAL,			18,	"B24_TK_PZ_CHKG_LEDGER_BAL"		},
	{B24_TK_PZ_OFF_RSRV_USR_FLD,			12,	"B24_TK_PZ_RSRV_USR_FLD"		},
};

static const token_info_type		tab_Token_A5_InfoType[] = {
	{B24_TK_A5_OFF_ITEM_QTY,				2,	"B24_TK_A5_ITEM_QTY"			},
	{B24_TK_A5_OFF_HOPR_CONTENTS,			2,	"B24_TK_A5_HOPR_CONTENTS"		},
};

static const token_info_type		tab_Token_17_InfoType[] = {
	{B24_TK_17_OFF_SERVICE_IND,				1,	"B24_TK_17_SERVICE_IND"				},
	{B24_TK_17_OFF_TID,						15,	"B24_TK_17_TID"						},
	{B24_TK_17_OFF_VALIDATION_CODE,			4,	"B24_TK_17_VALIDATION_CODE"			},
	{B24_TK_17_OFF_MARKET_SPECIFIC,			1,	"B24_TK_17_MARKET_SPECIFIC"			},
	{B24_TK_17_OFF_DURATION,				2,	"B24_TK_17_DURATION"				},
	{B24_TK_17_OFF_PRESTIGIOUS_PROP_IND,	1,	"B24_TK_17_PRESTIGIOUS_PROP_IND"	},
};

static const token_info_type		tab_Token_21_InfoType[] = {
	{B24_TK_21_OFF_SERVICE_IND,				1,	"B24_TK_21_SERVICE_IND"				},
	{B24_TK_21_OFF_TID,						15,	"B24_TK_21_TID"						},
	{B24_TK_21_OFF_VALIDATION_CODE,			4,	"B24_TK_21_VALIDATION_CODE"			},
};


static const token_info_type		tab_Token_PP_InfoType[] = {
	{B24_TK_PP_OFF_TAXE_1,				12,	"B24_TK_PP_TAXE_1"			},
	{B24_TK_PP_OFF_TAXE_2,				12,	"B24_TK_PP_TAXE_2"			},
	{B24_TK_PP_OFF_TAXE_3,				12,	"B24_TK_PP_TAXE_3"			},
};

static const token_info_type		tab_Token_20_InfoType[] = {
	{B24_TK_20_OFF_FILLER_1,			1,	"B24_TK_20_FILLER_1"			},
	{B24_TK_20_OFF_TRACE_ID,			15,	"B24_TK_20_TRACE_ID"			},
	{B24_TK_20_OFF_FILLER_2,			8,	"B24_TK_20_FILLER_2"			},
};

/******NCCC*******/
/*
static const token_info_type		tab_Token_17_InfoType[] = {
	{B24_TK_17_OFF_TRACK_1,				2,	"B24_TK_17_TRACK_1"			},
	{B24_TK_17_OFF_TID,					5,	"B24_TK_17_TID"				},
	{B24_TK_17_OFF_FILLER,				8,	"B24_TK_17_FILLER"			},
};
*/

static const token_info_type		tab_Token_25_InfoType[] = {
	{B24_TK_25_OFF_TRX_FEE,				19,	"B24_TK_25_TRX_FEE"			},
	{B24_TK_25_OFF_ORIG_TRX_FEE,		19,	"B24_TK_25_ORIG_TRX_FEE"	},
	{B24_TK_25_OFF_FILLER,				32,	"B24_TK_25_FILLER"			},
};

static const token_info_type		tab_Token_B0_InfoType[] = {
	{B24_TK_B0_OFF_LENGTH,				3,	"B24_TK_B0_LENGTH"					},
	{B24_TK_B0_OFF_FILLER_1,			1,	"B24_TK_B0_FILLER_1"				},
	{B24_TK_B0_OFF_FIID,				4,	"B24_TK_B0_FIID"					},
	{B24_TK_B0_OFF_FILLER_2,			12,	"B24_TK_B0_FILLER_2"				},
	{B24_TK_B0_OFF_ADV_REASON_CODE,		3,	"B24_TK_B0_ADV_REASON_CODE"			},
	{B24_TK_B0_OFF_FILLER_3,			15,	"B24_TK_B0_FILLER_3"				},
	{B24_TK_B0_OFF_ON_BEHALF,			30,	"B24_TK_B0_ON_BEHALF"				},
	{B24_TK_B0_OFF_ON_BEHALF_SVC_1,		2,	"B24_TK_B0_ON_BEHALF_SVC_1"			},
	{B24_TK_B0_OFF_ON_BEHALF_RESULT_1,	1,	"B24_TK_B0_ON_BEHALF_RESULT_1"		},
	{B24_TK_B0_OFF_ON_BEHALF_SVC_2,		2,	"B24_TK_B0_ON_BEHALF_SVC_2"			},
	{B24_TK_B0_OFF_ON_BEHALF_RESULT_2,	1,	"B24_TK_B0_ON_BEHALF_RESULT_2"		},
	{B24_TK_B0_OFF_ON_BEHALF_SVC_3,		2,	"B24_TK_B0_ON_BEHALF_SVC_3"			},
	{B24_TK_B0_OFF_ON_BEHALF_RESULT_3,	1,	"B24_TK_B0_ON_BEHALF_RESULT_3"		},
	{B24_TK_B0_OFF_ON_BEHALF_SVC_4,		2,	"B24_TK_B0_ON_BEHALF_SVC_4"			},
	{B24_TK_B0_OFF_ON_BEHALF_RESULT_4,	1,	"B24_TK_B0_ON_BEHALF_RESULT_4"		},
	{B24_TK_B0_OFF_ON_BEHALF_SVC_5,		2,	"B24_TK_B0_ON_BEHALF_SVC_5"			},
	{B24_TK_B0_OFF_ON_BEHALF_RESULT_5,	1,	"B24_TK_B0_ON_BEHALF_RESULT_5"		},
	{B24_TK_B0_OFF_ON_BEHALF_SVC_6,		2,	"B24_TK_B0_ON_BEHALF_SVC_6"			},
	{B24_TK_B0_OFF_ON_BEHALF_RESULT_6,	1,	"B24_TK_B0_ON_BEHALF_RESULT_6"		},
	{B24_TK_B0_OFF_ON_BEHALF_SVC_7,		2,	"B24_TK_B0_ON_BEHALF_SVC_7"			},
	{B24_TK_B0_OFF_ON_BEHALF_RESULT_7,	1,	"B24_TK_B0_ON_BEHALF_RESULT_7"		},
	{B24_TK_B0_OFF_ON_BEHALF_SVC_8,		2,	"B24_TK_B0_ON_BEHALF_SVC_8"			},
	{B24_TK_B0_OFF_ON_BEHALF_RESULT_8,	1,	"B24_TK_B0_ON_BEHALF_RESULT_8"		},
	{B24_TK_B0_OFF_ON_BEHALF_SVC_9,		2,	"B24_TK_B0_ON_BEHALF_SVC_9"			},
	{B24_TK_B0_OFF_ON_BEHALF_RESULT_9,	1,	"B24_TK_B0_ON_BEHALF_RESULT_9"		},
	{B24_TK_B0_OFF_ON_BEHALF_SVC_10,	2,	"B24_TK_B0_ON_BEHALF_SVC_10"		},
	{B24_TK_B0_OFF_ON_BEHALF_RESULT_10,	1,	"B24_TK_B0_ON_BEHALF_RESULT_10"		},
	{B24_TK_B0_OFF_FILLER_4,			17,	"B24_TK_B0_FILLER_4"				},
	{B24_TK_B0_OFF_ADV_DETAIL_CODE,		4,	"B24_TK_B0_ADV_DETAIL_CODE"			},
	{B24_TK_B0_OFF_AUTH_AGENT_ID_CODE,	6,	"B24_TK_B0_AUTH_AGENT_ID_CODE"		},
	{B24_TK_B0_OFF_FILLER_5,			129,	"B24_TK_B0_FILLER_5"			},

};


static const token_info_type		tab_Token_BY_InfoType[] = {
	{B24_TK_BY_OFF_FILLER_1,			52,	"B24_TK_BY_FILLER_1"				},
	{B24_TK_BY_OFF_PRODUCT_ID,			2,	"B24_TK_BY_PRODUCT_ID"				},
	{B24_TK_BY_OFF_FILLER_2,			6,	"B24_TK_BY_FILLER_2"				},
};


static const token_info_type		tab_Token_C5_InfoType[] = {
	{B24_TK_C5_OFF_MERCHANT_ID,				10,	"B24_TK_C5_MERCHANT_ID"					},
	{B24_TK_C5_OFF_STORE_ID,				6,	"B24_TK_C5_STORE_ID"					},
	{B24_TK_C5_OFF_TRX_CODE,				2,	"B24_TK_C5_TRX_CODE"					},
	{B24_TK_C5_OFF_REF_NBR,					11,	"B24_TK_C5_REF_NBR"						},
	{B24_TK_C5_OFF_GOODS_ID,				6,	"B24_TK_C5_GOODS_ID"					},
	{B24_TK_C5_OFF_PERSONAL_ID,				10,	"B24_TK_C5_PERSONAL_ID"					},
	{B24_TK_C5_OFF_PIN,						4,	"B24_TK_C5_PIN"							},
	{B24_TK_C5_OFF_EC_FLAG,					1,	"B24_TK_C5_EC_FLAG"						},

	/* Installment */
	{B24_TK_C5_OFF_ORDER_NO,				40,	"B24_TK_C5_ORDER_NO"					},
	{B24_TK_C5_OFF_INSTALLMENT_TRX_IND,		1,	"B24_TK_C5_INSTALLMENT_TRX_IND"		},
	{B24_TK_C5_OFF_INSTALLMENT_RESP_CODE,	2,	"B24_TK_C5_INSTALLMENT_RESP_CODE"	},
	{B24_TK_C5_OFF_FILLER,					27,	"B24_TK_C5_FILLER"						},
	{B24_TK_C5_OFF_PERIOD_NBR,				2,	"B24_TK_C5_PERIOD_NBR"					},
	{B24_TK_C5_OFF_DOWN_PAYMENT,			8,	"B24_TK_C5_DOWN_PAYMENT"				},
	{B24_TK_C5_OFF_INSTALLMENT_PAYMENTS,	8,	"B24_TK_C5_INSTALLMENT_PAYMENTS"	},
	{B24_TK_C5_OFF_FORMALITY_FEE,			6,	"B24_TK_C5_FORMALITY_FEE"			},

	/* Bonus points */
	{B24_TK_C5_OFF_LOY_SETTLEMENT_FLAG,		2,	"B24_TK_C5_LOY_SETTLEMENT_FLAG"		},
	{B24_TK_C5_OFF_LOY_ORDER_NO,			38,	"B24_TK_C5_LOY_ORDER_NO"			},
	{B24_TK_C5_OFF_LOY_TRX_IND,				1,	"B24_TK_C5_LOY_TRX_IND"				},
	{B24_TK_C5_OFF_LOY_RESP_CODE,			2,	"B24_TK_C5_LOY_RESP_CODE"			},
	{B24_TK_C5_OFF_LOY_PTS_OF_REDEMPTION,	8,	"B24_TK_C5_LOY_PTS_OF_REDEMPTION"	},
	{B24_TK_C5_OFF_LOY_SIGN_PTS_OF_BALANCE,	1,	"B24_TK_C5_LOY_SIGN_PTS_OF_BALANCE"	},
	{B24_TK_C5_OFF_LOY_PTS_OF_BALANCE,		8,	"B24_TK_C5_LOY_PTS_OF_BALANCE"		},
	{B24_TK_C5_OFF_LOY_PAID_CREDIT_AMNT,	10,	"B24_TK_C5_LOY_PAID_CREDIT_AMNT"	},

};

static const token_info_type		tab_Token_C6_InfoType[] = {
	{B24_TK_C6_OFF_XID,					40,		"B24_TK_C6_XID"					},
	{B24_TK_C6_OFF_TRX_STAIN_CAVV,		40,		"B24_TK_C6_TRX_STAIN_CAVV"		},
};

static const token_info_type		tab_Token_CE_InfoType[] = {
	{B24_TK_CE_OFF_AUTH_IND_FLAG,		2,		"B24_TK_CE_AUTH_IND_FLAG"		},
	{B24_TK_CE_OFF_AUTH_IND_DATA,		200,	"B24_TK_CE_AUTH_IND_DATA"		},
};

static const token_info_type		tab_Token_CH_InfoType[] = {
	{B24_TK_CH_OFF_FILLER_1,			33,		"B24_TK_CH_FILLER_1"			},
	{B24_TK_CH_OFF_REV_REASON_IND,		1,		"B24_TK_CH_REV_REASON_IND"		},
	{B24_TK_CH_OFF_FILLER_2,			6,		"B24_TK_CH_FILLER_2"			},
};


/*TODO: In spec, acc and acc_ind lengths are interchanged*/
static const token_info_type		tab_Token_S8_InfoType[] = {
	{B24_TK_S8_OFF_ACC_NO_IND,			1,		"B24_TK_S8_ACC_NO_IND"		},
	{B24_TK_S8_OFF_ACC_NO,				19,		"B24_TK_S8_ACC_NO"			},
	{B24_TK_S8_OFF_EXPIRY_DATE,			4,		"B24_TK_S8_EXPIRY_DATE"		},
	{B24_TK_S8_OFF_FILLER,				6,		"B24_TK_S8_FILLER"			},
};


static const token_info_type		tab_Token_CK_InfoType[] = {
	{B24_TK_CK_OFF_INDUSTRY_TYPE,		2,		"B24_TK_CK_INDUSTRY_TYPE"			},
	{B24_TK_CK_OFF_LG_ARRIVAL_DATE,		6,		"B24_TK_CK_LG_ARRIVAL_DATE"			},
	{B24_TK_CK_OFF_LG_DEPARTURE_DATE,	6,		"B24_TK_CK_LG_DEPARTURE_DATE"		},
	{B24_TK_CK_OFF_LG_NB_ROOM_NIGHTS,	4,		"B24_TK_CK_LG_NB_ROOM_NIGHTS"		},
	{B24_TK_CK_OFF_LG_ROOM_RATE,		12,		"B24_TK_CK_LG_ROOM_RATE"			},
	{B24_TK_CK_OFF_LG_ROOM_TAX,			12,		"B24_TK_CK_LG_ROOM_TAX"				},
	{B24_TK_CK_OFF_LG_PHONE_CHARGES,	12,		"B24_TK_CK_LG_PHONE_CHARGES"		},
	{B24_TK_CK_OFF_LG_LAUNDRY_CHARGES,	12,		"B24_TK_CK_LG_LAUNDRY_CHARGES"		},
	{B24_TK_CK_OFF_LG_GIFT_SHOP_CHARGES,12,		"B24_TK_CK_LG_GIFT_SHOP_CHARGES"	},
	{B24_TK_CK_OFF_LG_BAR_CHARGES,		12,		"B24_TK_CK_LG_BAR_CHARGES"			},
	{B24_TK_CK_OFF_LG_OTHER_CHARGES,	12,		"B24_TK_CK_LG_OTHER_CHARGES"		},
	{B24_TK_CK_OFF_LG_TOTAL_TAX_AMNT,	12,		"B24_TK_CK_LG_TOTAL_TAX_AMNT"		},
	{B24_TK_CK_OFF_LG_PROPERTY_PHONE_NO,15,		"B24_TK_CK_LG_PROPERTY_PHONE_NO"	},
	{B24_TK_CK_OFF_LG_CUST_SERV_PHONE_NO,15,	"B24_TK_CK_LG_CUST_SERV_PHONE_NO"	},
	{B24_TK_CK_OFF_LG_FOLIO_NO,			10,		"B24_TK_CK_LG_FOLIO_NO"				},
	{B24_TK_CK_OFF_LG_FIRE_SAFETY_IND,	1,		"B24_TK_CK_LG_FIRE_SAFETY_IND"		},
	{B24_TK_CK_OFF_LG_NO_SHOW_IND,		1,		"B24_TK_CK_LG_NO_SHOW_IND"			},
	{B24_TK_CK_OFF_LG_USER_FLD_1,		16,		"B24_TK_CK_LG_USER_FLD_1"			},
	{B24_TK_CK_OFF_CR_RENTER_NAME,		29,		"B24_TK_CK_CR_RENTER_NAME"			},
	{B24_TK_CK_OFF_CR_RENTAL_CLASS_ID,	4,		"B24_TK_CK_CR_RENTAL_CLASS_ID"		},
	{B24_TK_CK_OFF_CR_RENTAL_DATE,		6,		"B24_TK_CK_CR_RENTAL_DATE"			},
	{B24_TK_CK_OFF_CR_RENTAL_CITY,		18,		"B24_TK_CK_CR_RENTAL_CITY"			},
	{B24_TK_CK_OFF_CR_RENTAL_STATE,		3,		"B24_TK_CK_CR_RENTAL_STATE"			},
	{B24_TK_CK_OFF_CR_RENTAL_COUNTRY,	3,		"B24_TK_CK_CR_RENTAL_COUNTRY"		},
	{B24_TK_CK_OFF_CR_RETURN_DATE,		6,		"B24_TK_CK_CR_RETURN_DATE"			},
	{B24_TK_CK_OFF_CR_RETURN_CITY,		18,		"B24_TK_CK_CR_RETURN_CITY"			},
	{B24_TK_CK_OFF_CR_RETURN_STATE,		3,		"B24_TK_CK_CR_RETURN_STATE"			},
	{B24_TK_CK_OFF_CR_RETURN_COUNTRY,	3,		"B24_TK_CK_CR_RETURN_COUNTRY"		},
	{B24_TK_CK_OFF_CR_RETURN_LOC_ID,	10,		"B24_TK_CK_CR_RETURN_LOC_ID"		},
	{B24_TK_CK_OFF_CR_NB_DAYS_RENTED,	4,		"B24_TK_CK_CR_NB_DAYS_RENTED"		},
	{B24_TK_CK_OFF_CR_DAILY_RENTAL_RATE,12,		"B24_TK_CK_CR_DAILY_RENTAL_RATE"	},
	{B24_TK_CK_OFF_CR_EXTRA_CHARGES,	12,		"B24_TK_CK_CR_EXTRA_CHARGES"		},
	{B24_TK_CK_OFF_CR_TOTAL_TAX_AMNT,	12,		"B24_TK_CK_CR_TOTAL_TAX_AMNT"		},
	{B24_TK_CK_OFF_CR_CUST_SERV_PHONE_NO,15,	"B24_TK_CK_CR_CUST_SERV_PHONE_NO"	},
	{B24_TK_CK_OFF_CR_AGREEMENT_NO,		9,		"B24_TK_CK_CR_AGREEMENT_NO"			},
	{B24_TK_CK_OFF_CR_TAX_EXEMPT_IND,	1,		"B24_TK_CK_CR_TAX_EXEMPT_IND"		},
	{B24_TK_CK_OFF_CR_NO_SHOW_IND,		1,		"B24_TK_CK_CR_NO_SHOW_IND"			},
	{B24_TK_CK_OFF_CR_USER_FLD_2,		1,		"B24_TK_CK_CR_USER_FLD_2"			},
};

/** aha20150428 token QT for cost of transaction **/
static const token_info_type		tab_Token_QT_InfoType[] = {
	{B24_TK_QT_OFF_TRANS_CODE,			6,		"B24_TK_QT_TRANS_CODE"		},
	{B24_TK_QT_OFF_REF_NBR_TRANS,		12,		"B24_TK_QT_REF_NBR_TRANS"		},
	{B24_TK_QT_OFF_COST_OF_TRANS,		12,		"B24_TK_QT_COST_OF_TRANS"		},
	{B24_TK_QT_OFF_INDICATOR_REPORT_TRANS, 1,	"B24_TK_QT_INDICATOR_REPORT_TRANS"},
	{B24_TK_QT_OFF_SPACE,				1,		"B24_TK_QT_SPACE"				},
};
/*START NAB08102020 PLUTONL-2288*/
static const token_info_type		tab_Token_Q1_InfoType[] = {
	{B24_TK_Q1_OFF_AUTH_MODE_ID  ,			1,		"B24_TK_Q1_AUTH_MODE_ID"		},
	{B24_TK_Q1_OFF_CRYPTO_VAL_ID ,		    1,		"B24_TK_Q1_CRYPTO_VAL_ID"		},
};

static const token_info_type		tab_Token_Q2_InfoType[] = {
	{B24_TK_Q2_OFF_ACCESS_AUTH_ID,			2,		"B24_TK_Q2_ACCESS_AUTH_ID"		},
};
static const token_info_type		tab_Token_Q6_InfoType[] = {
	{B24_TK_Q6_OFF_DEFERRAL_NB_MONTH_EXCLUDED,	2,		"B24_TK_Q6_DEFERRAL_NB_MONTH_EXCLUDED"		},
	{B24_TK_Q6_OFF_NUM_PAYMENTS,				2,		"B24_TK_Q6_NUM_PAYMENTS"		},
	{B24_TK_Q6_OFF_PLAN_TYPE,					2,		"B24_TK_Q6_PLAN_TYPE"		},
	
};
static const token_info_type		tab_Token_CZ_InfoType[] = {
	{B24_TK_CZ_OFF_ATC,   				4,		"B24_TK_CZ_ATC"		},
	{B24_TK_CZ_OFF_FORM_FACTR_IND,		8,		"B24_TK_CZ_FORM_FACTR_IND"		},
	{B24_TK_CZ_OFF_USER_FLD_ACI,		28,		"B24_TK_CZ_USER_FLD_ACI"		},
	
};

static const token_info_type		tab_Token_R4_InfoType[] = {
	{B24_TK_R4_OFF_CONTRACT_NUMBER,		20,		"B24_TK_R4_CONTRACT_NUMBER"		},
	
};

static const token_info_type		tab_Token_RJ_InfoType[] = {
	{B24_TK_RJ_OFF_3DS_PROTOCOL_IND,	2,		"B24_TK_RJ_3DS_PROTOCOL_IND"		},
	{B24_TK_RJ_OFF_DS_TXN_ID,			36,		"B24_TK_RJ_DS_TXN_ID"		},
	{B24_TK_RJ_OFF_USER_FLD3,			2,		"B24_TK_RJ_USER_FLD3"		},
};

/*END NAB08102020 PLUTONL-2288*/
 
/******************************************************************************/
static tokens_info_type		tab_TokenInfoType[] = {
	{B24_TOKEN_BM,		1024,	"BM",	tab_Token_BM_InfoType,	sizeof(tab_Token_BM_InfoType)/sizeof(token_info_type),	B24_TK_BM_SIZE},
	{B24_TOKEN_BP,		1024,	"BP",	tab_Token_BP_InfoType,	sizeof(tab_Token_BP_InfoType)/sizeof(token_info_type),	B24_TK_BP_SIZE},
	{B24_TOKEN_B2,		1024,	"B2",	tab_Token_B2_InfoType,	sizeof(tab_Token_B2_InfoType)/sizeof(token_info_type),	B24_TK_B2_SIZE},
	{B24_TOKEN_B3,		1024,	"B3",	tab_Token_B3_InfoType,	sizeof(tab_Token_B3_InfoType)/sizeof(token_info_type),	B24_TK_B3_SIZE},
	{B24_TOKEN_B4,		1024,	"B4",	tab_Token_B4_InfoType,	sizeof(tab_Token_B4_InfoType)/sizeof(token_info_type),	B24_TK_B4_SIZE},
	{B24_TOKEN_B5,		1024,	"B5",	tab_Token_B5_InfoType,	sizeof(tab_Token_B5_InfoType)/sizeof(token_info_type),	B24_TK_B5_SIZE},
	{B24_TOKEN_B6,		1024,	"B6",	tab_Token_B6_InfoType,	sizeof(tab_Token_B6_InfoType)/sizeof(token_info_type),	B24_TK_B6_SIZE},
	{B24_TOKEN_BJ,		1024,	"BJ",	tab_Token_BJ_InfoType,	sizeof(tab_Token_BJ_InfoType)/sizeof(token_info_type),	B24_TK_BJ_SIZE},
	{B24_TOKEN_01,		1024,	"01",	tab_Token_01_InfoType,	sizeof(tab_Token_01_InfoType)/sizeof(token_info_type),	B24_TK_01_SIZE},
	{B24_TOKEN_04,		1024,	"04",	tab_Token_04_InfoType,	sizeof(tab_Token_04_InfoType)/sizeof(token_info_type),	B24_TK_04_SIZE},
	{B24_TOKEN_C0,		1024,	"C0",	tab_Token_C0_InfoType,	sizeof(tab_Token_C0_InfoType)/sizeof(token_info_type),	B24_TK_C0_SIZE},
	{B24_TOKEN_C4,		1024,	"C4",	tab_Token_C4_InfoType,	sizeof(tab_Token_C4_InfoType)/sizeof(token_info_type),	B24_TK_C4_SIZE},
	{B24_TOKEN_BE,		1024,	"BE",	tab_Token_BE_InfoType,	sizeof(tab_Token_BE_InfoType)/sizeof(token_info_type),	B24_TK_BE_SIZE},
	{B24_TOKEN_PF,		1024,	"PF",	tab_Token_PF_InfoType,	sizeof(tab_Token_PF_InfoType)/sizeof(token_info_type),	B24_TK_PF_SIZE},
	{B24_TOKEN_PG,		1024,	"PG",	tab_Token_PG_InfoType,	sizeof(tab_Token_PG_InfoType)/sizeof(token_info_type),	B24_TK_PG_SIZE},
	{B24_TOKEN_P1,		1024,	"P1",	tab_Token_P1_InfoType,	sizeof(tab_Token_P1_InfoType)/sizeof(token_info_type),	B24_TK_P1_SIZE},
	{B24_TOKEN_06,		1024,	"06",	tab_Token_06_InfoType,	sizeof(tab_Token_06_InfoType)/sizeof(token_info_type),	B24_TK_06_SIZE},
	{B24_TOKEN_PZ,		1024,	"PZ",	tab_Token_PZ_InfoType,	sizeof(tab_Token_PZ_InfoType)/sizeof(token_info_type),	B24_TK_PZ_SIZE},
	{B24_TOKEN_A5,		1024,	"A5",	tab_Token_A5_InfoType,	sizeof(tab_Token_A5_InfoType)/sizeof(token_info_type),	B24_TK_A5_SIZE},
	{B24_TOKEN_17,		1024,	"17",	tab_Token_17_InfoType,	sizeof(tab_Token_17_InfoType)/sizeof(token_info_type),	B24_TK_17_SIZE},
	{B24_TOKEN_21,		1024,	"21",	tab_Token_21_InfoType,	sizeof(tab_Token_21_InfoType)/sizeof(token_info_type),	B24_TK_21_SIZE},
	{B24_TOKEN_25,		1024,	"25",	tab_Token_25_InfoType,	sizeof(tab_Token_25_InfoType)/sizeof(token_info_type),	B24_TK_25_SIZE},
	{B24_TOKEN_B0,		1024,	"B0",	tab_Token_B0_InfoType,	sizeof(tab_Token_B0_InfoType)/sizeof(token_info_type),	B24_TK_B0_SIZE},
	{B24_TOKEN_BY,		1024,	"BY",	tab_Token_BY_InfoType,	sizeof(tab_Token_BY_InfoType)/sizeof(token_info_type),	B24_TK_BY_SIZE},
	{B24_TOKEN_C5,		1024,	"C5",	tab_Token_C5_InfoType,	sizeof(tab_Token_C5_InfoType)/sizeof(token_info_type),	B24_TK_C5_SIZE},
	{B24_TOKEN_C6,		1024,	"C6",	tab_Token_C6_InfoType,	sizeof(tab_Token_C6_InfoType)/sizeof(token_info_type),	B24_TK_C6_SIZE},
	{B24_TOKEN_CE,		1024,	"CE",	tab_Token_CE_InfoType,	sizeof(tab_Token_CE_InfoType)/sizeof(token_info_type),	B24_TK_CE_SIZE},
	{B24_TOKEN_CH,		1024,	"CH",	tab_Token_CH_InfoType,	sizeof(tab_Token_CH_InfoType)/sizeof(token_info_type),	B24_TK_CH_SIZE},
	{B24_TOKEN_S8,		1024,	"S8",	tab_Token_S8_InfoType,	sizeof(tab_Token_S8_InfoType)/sizeof(token_info_type),	B24_TK_S8_SIZE},
	{B24_TOKEN_CK,		1024,	"CK",	tab_Token_CK_InfoType,	sizeof(tab_Token_CK_InfoType)/sizeof(token_info_type),	B24_TK_CK_SIZE},
	{B24_TOKEN_PP,		1024,	"PP",	tab_Token_PP_InfoType,	sizeof(tab_Token_PP_InfoType)/sizeof(token_info_type),	B24_TK_PP_SIZE},
	{B24_TOKEN_20,		1024,	"20",	tab_Token_20_InfoType,	sizeof(tab_Token_20_InfoType)/sizeof(token_info_type),	B24_TK_20_SIZE},
	{B24_TOKEN_QT,		1024,	"QT",	tab_Token_QT_InfoType,	sizeof(tab_Token_QT_InfoType)/sizeof(token_info_type),	B24_TK_QT_SIZE},
	/*START NAB08102020 PLUTONL-2288*/
	{B24_TOKEN_Q1,		1024,	"Q1",	tab_Token_Q1_InfoType,	sizeof(tab_Token_Q1_InfoType)/sizeof(token_info_type),	B24_TK_Q1_SIZE},
	{B24_TOKEN_Q2,		1024,	"Q2",	tab_Token_Q2_InfoType,	sizeof(tab_Token_Q2_InfoType)/sizeof(token_info_type),	B24_TK_Q2_SIZE},
	{B24_TOKEN_Q6,		1024,	"Q6",	tab_Token_Q6_InfoType,	sizeof(tab_Token_Q6_InfoType)/sizeof(token_info_type),	B24_TK_Q6_SIZE},
	{B24_TOKEN_CZ,		1024,	"CZ",	tab_Token_CZ_InfoType,	sizeof(tab_Token_CZ_InfoType)/sizeof(token_info_type),	B24_TK_CZ_SIZE},
	{B24_TOKEN_R4,		1024,	"R4",	tab_Token_R4_InfoType,	sizeof(tab_Token_R4_InfoType)/sizeof(token_info_type),	B24_TK_R4_SIZE},
	{B24_TOKEN_RJ,		1024,	"RJ",	tab_Token_RJ_InfoType,	sizeof(tab_Token_RJ_InfoType)/sizeof(token_info_type),	B24_TK_RJ_SIZE},
	/*END NAB08102020 PLUTONL-2288*/
};

/******************************************************************************/
static int gs_defined_tokens[B24_TOKEN_ID_QTY];

int DefineTokens()
{
	int		i;

	for(i=0; i < sizeof(gs_defined_tokens)/sizeof(gs_defined_tokens[0]); i++ )
		gs_defined_tokens[i]=NOK;
/*START NAB08102020 PLUTONL-2288 PROSA TOKENS */
#if defined(B24_PROSA)
	gs_defined_tokens[B24_TOKEN_B2_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_B3_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_B4_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_BJ_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_04_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_C0_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_C4_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_C6_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_CE_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_Q1_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_Q2_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_Q6_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_CZ_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_R4_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_RJ_ID]=		OK;

#else 
	gs_defined_tokens[B24_TOKEN_B2_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_B3_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_B4_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_BJ_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_C0_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_C4_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_04_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_B5_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_B6_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_06_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_17_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_21_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_25_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_B0_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_BY_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_C5_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_CH_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_S8_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_20_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_BM_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_BP_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_01_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_BE_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_PF_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_PG_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_P1_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_PZ_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_A5_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_CK_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_PP_ID]=		OK;
	gs_defined_tokens[B24_TOKEN_QT_ID]=		OK;

#endif
/*END NAB08102020 PLUTONL-2288 */
	return 0;
}

/******************************************************************************/
int GetTokenIndice(const char* tokenId)
{

	int		i;
	for(i=0; i < sizeof(tab_TokenInfoType)/sizeof(tokens_info_type); i++)
	{
		if( memcmp(tab_TokenInfoType[i].sTokenName,tokenId,strlen(tab_TokenInfoType[i].sTokenName)) == 0 )
		{
			return i;
		}
	}
	return -1;
}
/******************************************************************************/
void InitTokens(TSTokens* tokens)
{
	tokens->nLength = 0;
	tokens->nNbTokens = 0;
	memset(tokens->sData,' ',sizeof(tokens->sData));
	memset(tokens->nTokenPos,0,sizeof(tokens->nTokenPos));
	memset(tokens->nPresent,0,sizeof(tokens->nPresent));
	memset(tokens->nTokenLength,0,sizeof(tokens->nTokenLength));
	
}
/******************************************************************************/
int AnalyseTokens(char* buffer_in, TSTokens* tokens)
{

	char	sNbTokens[12];
	char	sLength[12];
	int		nOffset=0;
	int		nIndice;
	int		nTokenLen;
	char*	sPtr;
	

	memset(sNbTokens,'\0',sizeof(sNbTokens));
	memset(sLength,'\0',sizeof(sLength));

	memcpy(sNbTokens,buffer_in + B24_TOKENS_HEADER_COUNT, B24_TOKENS_HEADER_COUNT_SZ);
	memcpy(sLength, buffer_in + B24_TOKENS_HEADER_LENGTH,B24_TOKENS_HEADER_LENGTH_SZ);

	/*tokens->nNbTokens = atoi(sNbTokens);*/
	tokens->nLength = atoi(sLength) - B24_TOKENS_HEADER_SZ;
	memcpy(tokens->sData,buffer_in + B24_TOKENS_HEADER_SZ,tokens->nLength);


	sPtr = tokens->sData;
	nOffset = 0;

	while( nOffset < tokens->nLength )
	{
		nIndice = GetTokenIndice(sPtr + B24_TOKEN_HEADER_ID);
		if( nIndice < 0 )
			return NOK;

		memset(sLength,'\0',sizeof(sLength));
		memcpy(sLength, sPtr + B24_TOKEN_HEADER_LENGTH,B24_TOKEN_HEADER_LENGTH_SZ);
		nTokenLen = atoi(sLength) + B24_TOKEN_HEADER_SZ;

		tokens->nTokenPos[nIndice]	= nOffset;
		tokens->nPresent[nIndice]	= TK_PRESENT;
		tokens->nTokenLength[nIndice] = nTokenLen;
		
		nOffset += nTokenLen;
		sPtr += nTokenLen;
		tokens->nNbTokens++;
	}

	return OK;
	
}
/******************************************************************************/
int IsTokenPresent(int nIndice, TSTokens* tokens)
{
	
	if( tokens->nPresent[nIndice] != TK_PRESENT )
		return NOK;

	return OK;
}
/******************************************************************************/
int GetToken(const char* tokenId, TSTokens* tokens, char* data, int* length)
{
	int		nIndice;

	nIndice = GetTokenIndice(tokenId);
	if( nIndice < 0 )
		return NOK;

	if( tokens->nPresent[nIndice] != TK_PRESENT )
		return NOK;

	memcpy(data,tokens->sData + tokens->nTokenPos[nIndice],tokens->nTokenLength[nIndice]);

	*length = tokens->nTokenLength[nIndice];
	data[*length]='\0';
	return OK;

}
/******************************************************************************/
int AddToken(const char* tokenId, TSTokens* tokens, char* data, int length)
{

	int		nIndice;

	nIndice = GetTokenIndice(tokenId);
	if( nIndice < 0 )
		return NOK;

	if( gs_defined_tokens[nIndice] != OK )
		return NOK;

	if( tokens->nPresent[nIndice] == TK_PRESENT )
		return NOK;

	tokens->nTokenPos[nIndice]	= tokens->nLength;
	tokens->nPresent[nIndice] = TK_PRESENT;
	tokens->nTokenLength[nIndice] = length;

	memcpy(tokens->sData + tokens->nLength, data, length);
	tokens->nLength += length;
	tokens->nNbTokens++;

	return OK;
}
/******************************************************************************/
int PutToken(const char* tokenId, TSTokens* tokens, char* data, int length)
{

	int		nIndice;
	char	buffer[B24_MAX_LEN_TOKENS];
	int		nBufferLen;
	int		nOldLen;
	int		nAdjustLen;
	int		i;
	
	nIndice = GetTokenIndice(tokenId);
	if( nIndice < 0 )
		return NOK;

	if( gs_defined_tokens[nIndice] != OK )
		return NOK;

	if( tokens->nPresent[nIndice] != TK_PRESENT )
		return AddToken(tokenId,tokens,data,length);

	if( tokens->nTokenLength[nIndice] == length )
	{
		memcpy(tokens->sData + tokens->nTokenPos[nIndice], data, length);
		return OK;
	}
	
	
	nBufferLen = tokens->nLength - tokens->nTokenPos[nIndice] - tokens->nTokenLength[nIndice];
	memcpy(buffer, 
		tokens->sData + tokens->nTokenPos[nIndice] + tokens->nTokenLength[nIndice], 
		nBufferLen);

	nOldLen = tokens->nLength;
	nAdjustLen = length - tokens->nTokenLength[nIndice];
	

	memcpy(tokens->sData + tokens->nTokenPos[nIndice], data, length);
	tokens->nTokenLength[nIndice] = length;
	memcpy(tokens->sData + tokens->nTokenPos[nIndice] + tokens->nTokenLength[nIndice], buffer, nBufferLen);
	
	tokens->nLength += nAdjustLen;
	
	for(i=0; i < sizeof(tab_TokenInfoType)/sizeof(tokens_info_type); i++)
	{
		if( tokens->nTokenPos[i] > tokens->nTokenPos[nIndice] )
		{
			tokens->nTokenPos[i] += nAdjustLen;
		}
	}
	
	return OK;
}
/******************************************************************************/
int BuildTokens(char* buffer_out,TSTokens* tokens)
{
	char*	sPtr;
	char	buffer[12];
	
	
	if( tokens->nLength == 0 )
		return 0;
	
	
	sPtr = buffer_out;
	*sPtr++='&';
	*sPtr++=' ';
	/*EBE150212 : Header token should be included
	sprintf(buffer,"%.05d",tokens->nNbTokens);*/
	sprintf(buffer,"%.05d",tokens->nNbTokens + 1);
	memcpy(sPtr,buffer,B24_TOKENS_HEADER_COUNT_SZ); sPtr+=B24_TOKENS_HEADER_COUNT_SZ;
	sprintf(buffer,"%.05d",tokens->nLength + B24_TOKENS_HEADER_SZ);
	memcpy(sPtr,buffer,B24_TOKENS_HEADER_LENGTH_SZ); sPtr+=B24_TOKENS_HEADER_LENGTH_SZ;
	memcpy(sPtr, tokens->sData, tokens->nLength );
	
	
	return tokens->nLength + B24_TOKENS_HEADER_SZ;
}
/******************************************************************************/
void DumpTokens(TSTokens* tokens)
{
	int i;
	TSToken token;
	for(i=0; i < sizeof(tab_TokenInfoType)/sizeof(tokens_info_type); i++)
	{
		if( tokens->nPresent[i] != TK_PRESENT )
			continue;
		trace_message(TRACE,"========================================");
		AnalyseToken(tokens->sData + tokens->nTokenPos[i],&token);
		DumpToken(&token);
		trace_message(TRACE,"========================================");
	}

}
/******************************************************************************/
const char* GetTokenName(int tokenId)
{
	return tab_TokenInfoType[tokenId].sTokenName;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
static void InitToken(TSToken* token)
{
	memset(token->sTokenId,'\0',sizeof(token->sTokenId));
	token->nLength = 0;
	token->nTokenIndice = 0;
	token->nNbTokens = 0;
	token->tab_token_info_type = NULL;
	memset(token->sData,' ',sizeof(token->sData));
	memset(token->nPos,0,sizeof(token->nPos));
	memset(token->nPresent,0,sizeof(token->nPresent));
}
/******************************************************************************/
int InitTokenType(char* sTokenName, TSToken* token)
{
	int		nIndice;
	char	szData[12];

	nIndice = GetTokenIndice(sTokenName);
	if( nIndice < 0 )
		return NOK;
	
	memset(szData,'\0',sizeof(szData));
	memcpy(szData,sTokenName,B24_TOKEN_ID_LEN);
	
	InitToken(token);
	memcpy(token->sTokenId,szData,B24_TOKEN_ID_LEN);

	token->nTokenIndice = nIndice;
	token->tab_token_info_type = tab_TokenInfoType[nIndice].tab_token_info_type;
	token->nNbTokens = tab_TokenInfoType[nIndice].nNbTokens;
	/*token->nLength = tab_TokenInfoType[nIndice].nTokenLen;*/
	token->nLength = 0;
	return OK;
}
/******************************************************************************/
int InitTokenId(int TokenId, TSToken* token)
{
	InitToken(token);
	memcpy(token->sTokenId,tab_TokenInfoType[TokenId].sTokenName,B24_TOKEN_ID_LEN);
	token->nTokenIndice = TokenId;
	token->tab_token_info_type = tab_TokenInfoType[TokenId].tab_token_info_type;
	token->nNbTokens = tab_TokenInfoType[TokenId].nNbTokens;
	/*token->nLength = tab_TokenInfoType[TokenId].nTokenLen;*/
	token->nLength = 0;

	return OK;
}
/******************************************************************************/
int AnalyseToken(char* buffer_in, TSToken* token)
{
	char	sLength[12];
	char	data[1024];
	int		nLength;
	int		i;

	memset(sLength,'\0',sizeof(sLength));
	InitToken(token);
	

	memcpy(token->sTokenId, buffer_in + B24_TOKEN_HEADER_ID, B24_TOKEN_HEADER_ID_SZ);
	
	if( InitTokenType(token->sTokenId,token) != OK )
		return NOK;


	memcpy(sLength, buffer_in + B24_TOKEN_HEADER_LENGTH,B24_TOKEN_HEADER_LENGTH_SZ);
	token->nLength = atoi(sLength);

	memcpy(token->sData,buffer_in + B24_TOKEN_HEADER_SZ,token->nLength);

	for( i=0; i < token->nNbTokens; i++ )
	{
		memset(data,'\0',sizeof(data));
		nLength = token->tab_token_info_type[i].nLength;
		memcpy(data,token->sData + token->tab_token_info_type[i].nOffset,nLength);
		data[nLength]='\0';
		nLength = trim(data,nLength);
		if(nLength > 0 )
			token->nPresent[i] = TK_PRESENT;
		else
			token->nPresent[i] = TK_NOT_PRESENT;
	}


	return OK;
}
/******************************************************************************/
int GetTokenElement(int tokenElemId, TSToken* token, char* data, int* length)
{
	/*BJ has repeatable elements!!*/
	if( token->tab_token_info_type[tokenElemId].nOffset >= token->nLength )
		return NOK;
	if( token->nPresent[tokenElemId] != TK_PRESENT )
		return NOK;

	*length=token->tab_token_info_type[tokenElemId].nLength;
	memcpy(data,token->sData + token->tab_token_info_type[tokenElemId].nOffset,*length);
	data[*length]='\0';
	return OK;
}
/******************************************************************************/
int AddTokenElement(int tokenElemId, TSToken* token, char* data, int length)
{
	/*BJ has repeatable elements!!*/
	if( token->tab_token_info_type[tokenElemId].nLength != length )
		return NOK;
	if( token->nPresent[tokenElemId] == TK_PRESENT )
		return NOK;
	memcpy(token->sData + token->tab_token_info_type[tokenElemId].nOffset,data,length);
	if( token->tab_token_info_type[tokenElemId].nOffset + length > token->nLength )
		token->nLength = token->tab_token_info_type[tokenElemId].nOffset + length;

	token->nPresent[tokenElemId] = TK_PRESENT;

	return OK;
}
/******************************************************************************/
int PutTokenElement(int tokenElemId, TSToken* token, char* data, int length)
{
	if( token->tab_token_info_type[tokenElemId].nLength != length )
		return NOK;

	memcpy(token->sData + token->tab_token_info_type[tokenElemId].nOffset,data,length);
	if( token->tab_token_info_type[tokenElemId].nOffset + length > token->nLength )
		token->nLength = token->tab_token_info_type[tokenElemId].nOffset + length;

	token->nPresent[tokenElemId] = TK_PRESENT;
	return OK;
}
/******************************************************************************/
int BuildToken(char* buffer_out,TSToken* token)
{
	char*	sPtr;
	char	buffer[12];

	if( token->nLength == 0 )
		return 0;

	sPtr = buffer_out;
	*sPtr++='!';
	*sPtr++=' ';
	memcpy(sPtr,token->sTokenId,B24_TOKEN_ID_LEN); sPtr+=B24_TOKEN_ID_LEN;
	sprintf(buffer,"%.05d",token->nLength);
	memcpy(sPtr,buffer,B24_TOKEN_HEADER_LENGTH_SZ); sPtr+=B24_TOKEN_HEADER_LENGTH_SZ;
	*sPtr++=' ';
	memcpy(sPtr,token->sData,token->nLength);
	return token->nLength + B24_TOKEN_HEADER_SZ;
}
/******************************************************************************/
void DumpToken(TSToken* token)
{
	int		i;
	char	data[B24_MAX_LEN_TOKENS];
	
	trace_message(TRACE,"Token: %s",token->sTokenId);
	for( i=0; i < token->nNbTokens; i++ )
	{
		if( token->nPresent[i] != TK_PRESENT )
			continue;

		memset(data,'\0',sizeof(data));

		memcpy(data,token->sData + token->tab_token_info_type[i].nOffset,token->tab_token_info_type[i].nLength);
		data[token->tab_token_info_type[i].nLength]='\0';
		trace_message(TRACE,"-> %.32s: [%s]",token->tab_token_info_type[i].sDescription,data);
	}
}
/******************************************************************************/
int GetTokenElementLength(int tokenId, int tokenElemId)
{
	return tab_TokenInfoType[tokenId].tab_token_info_type[tokenElemId].nLength;
}
/******************************************************************************/



