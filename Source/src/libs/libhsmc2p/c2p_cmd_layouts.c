#include <p7_macros.h>
#include <iso8583.h>
#include <hsm_c2p_define.h>
#include <hsm_c2p.h>

#include <p7_common.h>
#include <p7_globals.h>
#include <iso8583.h>

extern const C2PCmdInfoProperties	C2PFieldsDataProp;
extern const C2PCmdTagProperties	C2PCmdTagProp;

void ResetC2PCmdInfo(TSC2PCmd* pkC2PCmd);



#define C2P_LY_NB_FLD_VERIFY_AC_REQ			14
#define C2P_LY_NB_FLD_VERIFY_AC_RESP		3

#define C2P_LY_NB_FLD_GEN_ARPC_REQ			11
#define C2P_LY_NB_FLD_GEN_ARPC_RESP			3

#define C2P_LY_NB_FLD_GEN_MAC_REQ			11
#define C2P_LY_NB_FLD_GEN_MAC_RESP			4

#define C2P_LY_NB_FLD_ENC_DATA_REQ			9
#define C2P_LY_NB_FLD_ENC_DATA_RESP			3

#define C2P_LY_NB_FLD_VERIFY_DAC_REQ		6
#define C2P_LY_NB_FLD_VERIFY_DAC_RESP		2

#define C2P_LY_NB_FLD_VERIFY_IDN_REQ		8
#define C2P_LY_NB_FLD_VERIFY_IDN_RESP		2

#define C2P_LY_NB_FLD_PIN_CHG_REQ			18
#define C2P_LY_NB_FLD_PIN_CHG_RESP			5

#define C2P_LY_NB_FLD_GEN_KEY_REQ			21
#define C2P_LY_NB_FLD_GEN_KEY_RESP			6

#define C2P_LY_NB_FLD_SAVE_KEY_REQ			2
#define C2P_LY_NB_FLD_SAVE_KEY_RESP			2

#define C2P_LY_NB_FLD_IMPORT_KEY_REQ		16
#define C2P_LY_NB_FLD_IMPORT_KEY_RESP		3


#define C2P_LY_NB_FLD_DEL_KEY_REQ			3
#define C2P_LY_NB_FLD_DEL_KEY_RESP			3

#define C2P_LY_NB_FLD_VERIFY_KEY_REQ		3
#define C2P_LY_NB_FLD_VERIFY_KEY_RESP		9

#define C2P_LY_NB_FLD_AUTO_TEST_REQ			2
#define C2P_LY_NB_FLD_AUTO_TEST_RESP		16

#define C2P_LY_NB_FLD_STAT_REPORT_REQ		0
#define C2P_LY_NB_FLD_STAT_REPORT_RESP		7

#define C2P_LY_NB_FLD_GEN_SIG_REQ			12
#define C2P_LY_NB_FLD_GEN_SIG_RESP			4

#define C2P_LY_NB_FLD_VERIFY_SIG_REQ		10
#define C2P_LY_NB_FLD_VERIFY_SIG_RESP		3

#define C2P_LY_NB_FLD_GEN_FINGERPRINT_SHA_1_REQ		2
#define C2P_LY_NB_FLD_GEN_FINGERPRINT_SHA_1_RESP	3

#define C2P_LY_NB_FLD_GEN_HASH_REQ			10
#define C2P_LY_NB_FLD_GEN_HASH_RESP			6

#define C2P_LY_NB_FLD_VERIFY_HASH_REQ		9
#define C2P_LY_NB_FLD_VERIFY_HASH_RESP		2

#define C2P_LY_NB_FLD_GEN_AUTH_NO_REQ		4
#define C2P_LY_NB_FLD_GEN_AUTH_NO_RESP		3

#define C2P_LY_NB_FLD_PIN_TRSLT_REQ			14
#define C2P_LY_NB_FLD_PIN_TRSLT_RESP		5

#define C2P_LY_NB_FLD_VERIFY_PIN_REQ		15
#define C2P_LY_NB_FLD_VERIFY_PIN_RESP		3

#define C2P_LY_NB_FLD_VERIFY_CVX_REQ		6
#define C2P_LY_NB_FLD_VERIFY_CVX_RESP		2

#define C2P_LY_NB_FLD_VERIFY_CVX2_REQ		7
#define C2P_LY_NB_FLD_VERIFY_CVX2_RESP		2

#define C2P_LY_NB_FLD_EXPORT_KEY_REQ		5
#define C2P_LY_NB_FLD_EXPORT_KEY_RESP		4


#define C2P_LY_NB_FLD_GEN_PVV_REQ			11
#define C2P_LY_NB_FLD_GEN_PVV_RESP			4

#define C2P_LY_NB_FLD_GEN_CVX_REQ			5
#define C2P_LY_NB_FLD_GEN_CVX_RESP			3

#define C2P_LY_NB_FLD_GEN_CVX2_REQ		        6	
#define C2P_LY_NB_FLD_GEN_CVX2_RESP			3

#define C2P_LY_NB_FLD_BUILD_AUTH_TICKET_REQ		5
#define C2P_LY_NB_FLD_BUILD_AUTH_TICKET_RESP	4

#define C2P_LY_NB_FLD_VERIFY_AUTH_TICKET_REQ	6
#define C2P_LY_NB_FLD_VERIFY_AUTH_TICKET_RESP	4

#define C2P_LY_NB_FLD_GEN_SPA_HMAC_REQ		3
#define C2P_LY_NB_FLD_GEN_SPA_HMAC_RESP		3


#define C2P_LY_NB_FLD_VERIFY_SPA_HMAC_REQ	5
#define C2P_LY_NB_FLD_VERIFY_SPA_HMAC_RESP	2

#define C2P_LY_NB_FLD_VERIFY_CSC_REQ		8
#define C2P_LY_NB_FLD_VERIFY_CSC_RESP		2

#define C2P_LY_NB_FLD_GEN_CSC_REQ			7
#define C2P_LY_NB_FLD_GEN_CSC_RESP			6


#define C2P_LY_NB_FLD_GEN_CARD_KEY_KCV_REQ		6
#define C2P_LY_NB_FLD_GEN_CARD_KEY_KCV_RESP		3

#define C2P_LY_NB_FLD_VERIFY_CVC3_REQ			14
#define C2P_LY_NB_FLD_VERIFY_CVC3_RESP			3


#define C2P_LY_NB_FLD_GEN_IV_CVC3_REQ			6
#define C2P_LY_NB_FLD_GEN_IV_CVC3_RESP			3

#define C2P_LY_NB_FLD_GEN_RSA_REQ                   14
#define C2P_LY_NB_FLD_GEN_RSA_RESP                  3

#define C2P_LY_NB_FLD_IMP_SEC_KEY_RSA_REQ                   17
#define C2P_LY_NB_FLD_IMP_SEC_KEY_RSA_RESP                  3

#define C2P_LY_NB_FLD_EXP_SEC_KEY_RSA_REQ                   9
#define C2P_LY_NB_FLD_EXP_SEC_KEY_RSA_RESP                  4

#define C2P_LY_NB_FLD_ENCRYPT_RSA_REQ                4
#define C2P_LY_NB_FLD_ENCRYPT_RSA_RESP               3

#define C2P_LY_NB_FLD_DECRYPT_RSA_REQ                7
#define C2P_LY_NB_FLD_DECRYPT_RSA_RESP               3

#define C2P_LY_NB_FLD_DECRYPT_ASYM_KEY_REQ                12
#define C2P_LY_NB_FLD_DECRYPT_ASYM_KEY_RESP               3

#define C2P_LY_NB_FLD_EXT_PUBLIC_KEY_REQ                    4
#define C2P_LY_NB_FLD_EXT_PUBLIC_KEY_RESP                   6


/* UNKOWN_RESP */
static TSC2PMsgLayout gs_ly_UNKOWN_RESP = {
	C2P_MN_UNKOWN_FF01,2,2048,C2P_FN_CMD_RESP_UNKOWN_FF01,
	{
		/*{C2P_FN_CMD_RESP_UNKOWN_FF01			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		
	}
};

/* VERIFY_AC_REQ*/
static TSC2PMsgLayout gs_ly_VERIFY_AC_REQ = {
	C2P_MN_VERIFY_AC_E2,C2P_LY_NB_FLD_VERIFY_AC_REQ,2048,C2P_FN_CMD_REQ_VERIFY_AC_E2,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_AC_E2			},*/
		{C2P_FN_AC_ALGO_ID						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_EMV_CARD_MK_ALGO				},
		{C2P_FN_PAN								},
		{C2P_FN_PAN_SEQ_NO						},
		{C2P_FN_SESSION_KEY_GEN_USED_NBR		},
		{C2P_FN_SESSION_KEY_ALGO_PARAMS			},
		{C2P_FN_AC_GEN_DATA						},
		{C2P_FN_ISS_AC_BITMAP					},
		{C2P_FN_ARQC							},
		{C2P_FN_CSC_5							},
		{C2P_FN_MAC_DATA						},
		{C2P_FN_ENC_IAD_COUNTER					},

	}
};

/* VERIFY_AC_RESP*/
static TSC2PMsgLayout gs_ly_VERIFY_AC_RESP = {
	C2P_MN_VERIFY_AC_FF02,C2P_LY_NB_FLD_VERIFY_AC_RESP,2048,C2P_FN_CMD_RESP_VERIFY_AC_FF02,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_AC_FF02			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_IAD_COUNTER						},
	}
};

/* GEN_ARPC_REQ */
static TSC2PMsgLayout gs_ly_GEN_ARPC_REQ = {
	C2P_MN_GEN_ARPC_E3,C2P_LY_NB_FLD_GEN_ARPC_REQ,2048,C2P_FN_CMD_REQ_GEN_ARPC_E3,
	{
		/*{C2P_FN_CMD_REQ_GEN_ARPC_E3				},*/
		{C2P_FN_ARPC_GEN_ALGOID					},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_EMV_CARD_MK_ALGO				},
		{C2P_FN_PAN								},
		{C2P_FN_PAN_SEQ_NO						},
		{C2P_FN_SESSION_KEY_GEN_USED_NBR		},
		{C2P_FN_SESSION_KEY_ALGO_PARAMS			},
		{C2P_FN_ARQC							},
		{C2P_FN_ARC								},
		{C2P_FN_AUTH_DATA_2						},
	}
};

/* GEN_ARPC_RESP */
static TSC2PMsgLayout gs_ly_GEN_ARPC_RESP = {
	C2P_MN_GEN_ARPC_FF03,C2P_LY_NB_FLD_GEN_ARPC_RESP,2048,C2P_FN_CMD_RESP_GEN_ARPC_FF03,
	{
		/*{C2P_FN_CMD_RESP_GEN_ARPC_FF03				},*/
		{C2P_FN_RETURN_CODE							},
		{C2P_FN_COMPL_INFO							},
		{C2P_FN_ARPC								},
	}
};


/* GEN_MAC_REQ */
static TSC2PMsgLayout gs_ly_GEN_MAC_REQ = {
	C2P_MN_GEN_MAC_E4,C2P_LY_NB_FLD_GEN_MAC_REQ,2048,C2P_FN_CMD_REQ_GEN_MAC_E4,
	{
		/*{C2P_FN_CMD_REQ_GEN_MAC_E4				},*/
		{C2P_FN_MAC_ALGO_ID						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_EMV_CARD_MK_ALGO				},
		{C2P_FN_PAN								},
		{C2P_FN_PAN_SEQ_NO						},
		{C2P_FN_SESSION_KEY_GEN_USED_NBR		},
		{C2P_FN_SESSION_KEY_ALGO_PARAMS			},
		{C2P_FN_MAC_DATA						},
		{C2P_FN_MAC_LEN							},
		{C2P_FN_PAD_ALGO_ID						},
	}
};

/* GEN_MAC_RESP */
static TSC2PMsgLayout gs_ly_GEN_MAC_RESP = {
	C2P_MN_GEN_MAC_FF04,C2P_LY_NB_FLD_GEN_MAC_RESP,2048,C2P_FN_CMD_RESP_GEN_MAC_FF04,
	{
		/*{C2P_FN_CMD_RESP_GEN_MAC_FF04			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_MAC_LEN							},
		{C2P_FN_MAC								},
	}
};



/* ENC_DATA_REQ */
static TSC2PMsgLayout gs_ly_ENC_DATA_REQ = {
	C2P_MN_ENC_DATA_E5,C2P_LY_NB_FLD_ENC_DATA_REQ,2048,C2P_FN_CMD_REQ_ENC_DATA_E5,
	{
		/*{C2P_FN_CMD_REQ_ENC_DATA_E5				},*/
		{C2P_FN_ENC_ALGO_ID						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_EMV_CARD_MK_ALGO				},
		{C2P_FN_PAN								},
		{C2P_FN_PAN_SEQ_NO						},
		{C2P_FN_SESSION_KEY_GEN_USED_NBR		},
		{C2P_FN_SESSION_KEY_ALGO_PARAMS			},
		{C2P_FN_DATA_TO_ENC						},
	}
};

/* ENC_DATA_RESP */
static TSC2PMsgLayout gs_ly_ENC_DATA_RESP = {
	C2P_MN_ENC_DATA_FF05,C2P_LY_NB_FLD_ENC_DATA_RESP,2048,C2P_FN_CMD_RESP_ENC_DATA_FF05,
	{
		/*{C2P_FN_CMD_RESP_ENC_DATA_FF05			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_ENC_DATA						},
	}
};



/* VERIFY_DAC_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_DAC_REQ = {
	C2P_MN_VERIFY_DAC_E6,C2P_LY_NB_FLD_VERIFY_DAC_REQ,2048,C2P_FN_CMD_REQ_VERIFY_DAC_E6,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_DAC_E6			},*/
		{C2P_FN_DAC_GEN_ALGO_ID					},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_PAN								},
		{C2P_FN_PAN_SEQ_NO						},
		{C2P_FN_DAC								},
	}
};
/* VERIFY_DAC_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_DAC_RESP = {
	C2P_MN_VERIFY_DAC_FF06,C2P_LY_NB_FLD_VERIFY_DAC_RESP,2048,C2P_FN_CMD_RESP_VERIFY_DAC_FF06,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_DAC_FF06		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
	}
};


/* VERIFY_IDN_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_IDN_REQ = {
	C2P_MN_VERIFY_IDN_E7,C2P_LY_NB_FLD_VERIFY_IDN_REQ,2048,C2P_FN_CMD_REQ_VERIFY_IDN_E7,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_IDN_E7			},*/
		{C2P_FN_IDN_GEN_ALGO_ID					},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_EMV_CARD_MK_ALGO				},
		{C2P_FN_PAN								},
		{C2P_FN_PAN_SEQ_NO						},
		{C2P_FN_IDN_GEN_DATA					},
		{C2P_FN_IDN								},
	}
};
/* VERIFY_IDN_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_IDN_RESP = {
	C2P_MN_VERIFY_IDN_FF07,C2P_LY_NB_FLD_VERIFY_IDN_RESP,2048,C2P_FN_CMD_RESP_VERIFY_IDN_FF07,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_IDN_FF07		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
	}
};


/* PIN_CHG_REQ */
static TSC2PMsgLayout gs_ly_PIN_CHG_REQ = {
	C2P_MN_PIN_CHG_E8,C2P_LY_NB_FLD_PIN_CHG_REQ,2048,C2P_FN_CMD_REQ_PIN_CHG_E8,
	{
		/*{C2P_FN_CMD_REQ_PIN_CHG_E8				},*/
		{C2P_FN_ENC_ALGO_ID						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_EMV_CARD_MK_ALGO				},
		{C2P_FN_PAN								},
		{C2P_FN_PAN_SEQ_NO						},
		{C2P_FN_SESSION_KEY_GEN_USED_NBR		},
		{C2P_FN_SESSION_KEY_ALGO_PARAMS			},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_ENC_PIN_BLK						},
		{C2P_FN_PIN_BLK_FMT						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_ENC_PIN_BLK						},
		{C2P_FN_PIN_BLK_FMT						},
	}
};
/* PIN_CHG_RESP */
static TSC2PMsgLayout gs_ly_PIN_CHG_RESP = {
	C2P_MN_PIN_CHG_FF08,C2P_LY_NB_FLD_PIN_CHG_RESP,2048,C2P_FN_CMD_RESP_PIN_CHG_FF08,
	{
		/*{C2P_FN_CMD_RESP_PIN_CHG_FF08			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_ENC_DATA						},
		{C2P_FN_PIN_BLK_NA_IN					},
		{C2P_FN_PIN_BLK_NA_IN					},
	}
};



/* GEN_KEY_REQ */
static TSC2PMsgLayout gs_ly_GEN_KEY_REQ = {
	C2P_MN_GEN_KEY_E9,C2P_LY_NB_FLD_GEN_KEY_REQ,2048,C2P_FN_CMD_REQ_GEN_KEY_E9,
	{
		/*{C2P_FN_CMD_REQ_GEN_KEY_E9				},*/
		{C2P_FN_KEY_ID							},
		{C2P_FN_KEY_ID							},
		{C2P_FN_KCV_TYPE						},
		{C2P_FN_IMPLICIT_ENFORCEMENT_FLAG		},
		{C2P_FN_KEY_USAGE						},
		{C2P_FN_USAGE_MODE						},
		{C2P_FN_TOKEN_RANGE						},
		{C2P_FN_KEY_EXP_DATE					},
		{C2P_FN_KEY_ACTIVATION_DATE				},
		{C2P_FN_GROUP_ID						},
		{C2P_FN_CLASS							},
		{C2P_FN_KEY_LEN							},
		{C2P_FN_PARITY							},
		{C2P_FN_PARITY_BITS						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_RSA_PUB_KEY_BLOC				},
		{C2P_FN_DES_KEY_ENC_RSA_ALGO_ID			},
		{C2P_FN_ENC_MTHD_ID						},
		{C2P_FN_KEY_IDENT_DATA					},
		{C2P_FN_OAEP_ENC_PARAM					},
	}
};
/* GEN_KEY_RESP */
static TSC2PMsgLayout gs_ly_GEN_KEY_RESP = {
	C2P_MN_GEN_KEY_FF09,C2P_LY_NB_FLD_GEN_KEY_RESP,2048,C2P_FN_CMD_RESP_GEN_KEY_FF09,
	{
		/*{C2P_FN_CMD_RESP_GEN_KEY_FF09			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_KEY_KCV							},
		{C2P_FN_KEY_KEV							},
		{C2P_FN_RSA_PUB_KEY_ENC_KEY				},
	}
};



/* SAVE_KEY_REQ */
static TSC2PMsgLayout gs_ly_SAVE_KEY_REQ = {
	C2P_MN_SAVE_KEY_EA,C2P_LY_NB_FLD_SAVE_KEY_REQ,2048,C2P_FN_CMD_REQ_SAVE_KEY_EA,
	{
		/*{C2P_FN_CMD_REQ_SAVE_KEY_EA				},*/
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_TOKEN_RANGE						},	
	}
};
/* SAVE_KEY_RESP */
static TSC2PMsgLayout gs_ly_SAVE_KEY_RESP = {
	C2P_MN_SAVE_KEY_FF0A,C2P_LY_NB_FLD_SAVE_KEY_RESP,2048,C2P_FN_CMD_RESP_SAVE_KEY_FF0A,
	{
		/*{C2P_FN_CMD_RESP_SAVE_KEY_FF0A			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
	}
};


/* IMPORT_KEY_REQ */
static TSC2PMsgLayout gs_ly_IMPORT_KEY_REQ = {
	C2P_MN_IMPORT_KEY_EB,C2P_LY_NB_FLD_IMPORT_KEY_REQ,2048,C2P_FN_CMD_REQ_IMPORT_KEY_EB,
	{
		/*{C2P_FN_CMD_REQ_IMPORT_KEY_EB			},*/
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_KEY_ID							},
		{C2P_FN_KEY_ID							},
		{C2P_FN_KEY_USAGE						},
		{C2P_FN_USAGE_MODE						},
		{C2P_FN_TOKEN_RANGE						},
		{C2P_FN_KEY_EXP_DATE					},
		{C2P_FN_KEY_ACTIVATION_DATE				},
		{C2P_FN_GROUP_ID						},
		{C2P_FN_CLASS							},
		{C2P_FN_KEY_KCV							},
		{C2P_FN_KEY_KEV							},
		{C2P_FN_ENC_MTHD_ID						},
		{C2P_FN_KEY_IDENT_DATA					},
		{C2P_FN_KEY_ID							},
		{C2P_FN_IMPLICIT_ENFORCEMENT_FLAG		},

	}
};
/* IMPORT_KEY_RESP */
static TSC2PMsgLayout gs_ly_IMPORT_KEY_RESP = {
	C2P_MN_IMPORT_KEY_FF0B,C2P_LY_NB_FLD_IMPORT_KEY_RESP,2048,C2P_FN_CMD_RESP_IMPORT_KEY_FF0B,
	{
		/*{C2P_FN_CMD_RESP_IMPORT_KEY_FF0B		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
	}
};


/* DEL_KEY_REQ */
static TSC2PMsgLayout gs_ly_DEL_KEY_REQ = {
	C2P_MN_DEL_KEY_EC,C2P_LY_NB_FLD_DEL_KEY_REQ,2048,C2P_FN_CMD_REQ_DEL_KEY_EC,
	{
		/*{C2P_FN_CMD_REQ_DEL_KEY_EC				},*/
		{C2P_FN_KEY_ID							},
		{C2P_FN_KEY_DEL_FILTER					},
		{C2P_FN_TOKEN_RANGE						},
	}
};
/* DEL_KEY_RESP */
static TSC2PMsgLayout gs_ly_DEL_KEY_RESP = {
	C2P_MN_DEL_KEY_FF0C,C2P_LY_NB_FLD_DEL_KEY_RESP,2048,C2P_FN_CMD_RESP_DEL_KEY_FF0C,
	{
		/*{C2P_FN_CMD_RESP_DEL_KEY_FF0C			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_KEYS_NBR						},
	}
};



/* VERIFY_KEY_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_KEY_REQ = {
	C2P_MN_VERIFY_KEY_ED,C2P_LY_NB_FLD_VERIFY_KEY_REQ,2048,C2P_FN_CMD_REQ_VERIFY_KEY_ED,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_KEY_ED			},*/
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_KEY_ID							},
		{C2P_FN_AUTO_TEST_RESULT_FMT			},

	}
};
/* VERIFY_KEY_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_KEY_RESP = {
	C2P_MN_VERIFY_KEY_FF0D,C2P_LY_NB_FLD_VERIFY_KEY_RESP,2048,C2P_FN_CMD_RESP_VERIFY_KEY_FF0D,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_KEY_FF0D		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_KEY_KCV							},
		{C2P_FN_KEY_USAGE						},
		{C2P_FN_TOKEN_RANGE						},
		{C2P_FN_KEY_EXP_DATE					},
		{C2P_FN_KEY_ACTIVATION_DATE				},
		{C2P_FN_CLASS							},
		{C2P_FN_KEY_LEN							},
	}
};


/* AUTO_TEST_REQ */
static TSC2PMsgLayout gs_ly_AUTO_TEST_REQ = {
	C2P_MN_AUTO_TEST_F2,C2P_LY_NB_FLD_AUTO_TEST_REQ,2048,C2P_FN_CMD_REQ_AUTO_TEST_F2,
	{
		/*{C2P_FN_CMD_REQ_AUTO_TEST_F2			},*/
		{C2P_FN_AUTO_TEST_RESULT_FMT			},
		{C2P_FN_AUTO_TEST_TYPE					},
	}
};
/* AUTO_TEST_RESP */
static TSC2PMsgLayout gs_ly_AUTO_TEST_RESP = {
	C2P_MN_AUTO_TEST_FF12,C2P_LY_NB_FLD_AUTO_TEST_RESP,2048,C2P_FN_CMD_RESP_AUTO_TEST_FF12,
	{
		/*{C2P_FN_CMD_RESP_AUTO_TEST_FF12			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_C2P_NUMBER						},
		{C2P_FN_MODULE_STATE					},
		{C2P_FN_TRACE_MODE						},
		{C2P_FN_APP_ID							},
		{C2P_FN_VER_NO							},
		{C2P_FN_SERIAL_NO						},
		{C2P_FN_C2P_IDP							},
		{C2P_FN_AVAIL_OPTIONS					},
		{C2P_FN_LOADED_OPTIONS					},
		{C2P_FN_MASTER_KEY_NBR					},
		{C2P_FN_USAGE_PROFILE					},
		{C2P_FN_APP_STATE						},
		{C2P_FN_OP_MODE							},
		{C2P_FN_SECURITY_POLICY_FLAGS			},
	}
};


/* STAT_REPORT_REQ */
static TSC2PMsgLayout gs_ly_STAT_REPORT_REQ = {
	C2P_MN_STAT_REPORT_F3,C2P_LY_NB_FLD_STAT_REPORT_REQ,2048,C2P_FN_CMD_REQ_STAT_REPORT_F3/*,
	{
		{C2P_FN_CMD_REQ_STAT_REPORT_F3			},
	}*/
};
/* STAT_REPORT_RESP */
static TSC2PMsgLayout gs_ly_STAT_REPORT_RESP = {
	C2P_MN_STAT_REPORT_FF13,C2P_LY_NB_FLD_STAT_REPORT_RESP,2048,C2P_FN_CMD_RESP_STAT_REPORT_FF13,
	{
		/*{C2P_FN_CMD_RESP_STAT_REPORT_FF13		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_C2P_NUMBER						},
		{C2P_FN_PIN_VERIF_NEG_RESP_NBR			},
		{C2P_FN_PIN_VERIF_REQ_NBR				},
		{C2P_FN_CVX_VERIF_NEG_RESP_NBR			},
		{C2P_FN_CVX_VERIF_REQ_NBR				},
	}
};


/* GEN_SIG_REQ */
static TSC2PMsgLayout gs_ly_GEN_SIG_REQ = {
	C2P_MN_GEN_SIG_F5,C2P_LY_NB_FLD_GEN_SIG_REQ,2048,C2P_FN_CMD_REQ_GEN_SIG_F5,
	{
		/*{C2P_FN_CMD_REQ_GEN_SIG_F5				},*/
		{C2P_FN_SIG_ALGO_ID						},
		{C2P_FN_HASH_ALGO						},
		{C2P_FN_MGF_GEN_FUNC					},
		{C2P_FN_DIVERT_DATA_LEN					},
		{C2P_FN_FINGERPRINT						},
		{C2P_FN_HASHED_MSG_LEN					},
		{C2P_FN_KEY_ID							},
		{C2P_FN_PRIV_KEY_BLOC					},
		{C2P_FN_AUTHEN_MTHD						},
		{C2P_FN_RANDOM_NBR						},
		{C2P_FN_DYN_AUTH_DATA					},
		{C2P_FN_SIG_DATA						},
	}
};
/* GEN_SIG_RESP */
static TSC2PMsgLayout gs_ly_GEN_SIG_RESP = {
	C2P_MN_GEN_SIG_FF15,C2P_LY_NB_FLD_GEN_SIG_RESP,2048,C2P_FN_CMD_RESP_GEN_SIG_FF15,
	{
		/*{C2P_FN_CMD_RESP_GEN_SIG_FF15			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_SIGNATURE						},
		{C2P_FN_TRANS_DATA						},	
	}
};


/* VERIFY_SIG_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_SIG_REQ = {
	C2P_MN_VERIFY_SIG_F6,C2P_LY_NB_FLD_VERIFY_SIG_REQ,2048,C2P_FN_CMD_REQ_VERIFY_SIG_F6,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_SIG_F6			},*/
		{C2P_FN_SIG_VERIF_ALGO_ID				},
		{C2P_FN_HASH_ALGO						},
		{C2P_FN_MGF_GEN_FUNC					},
		{C2P_FN_DIVERT_DATA_LEN					},
		{C2P_FN_RSA_PUB_KEY_BLOC				},
		{C2P_FN_ECDSA_PUB_KEY_BLOC				},
		{C2P_FN_DSA_PUB_KEY_BLOC				},
		{C2P_FN_CERT_TKN_BLOC					},
		{C2P_FN_TRANS_DATA						},
		{C2P_FN_SIGNATURE						},
	}
};
/* VERIFY_SIG_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_SIG_RESP = {
	C2P_MN_VERIFY_SIG_FF16,C2P_LY_NB_FLD_VERIFY_SIG_RESP,2048,C2P_FN_CMD_RESP_VERIFY_SIG_FF16,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_SIG_FF16		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_SIG_DATA						},
	}
};


/* GEN_FINGERPRINT_SHA_1_REQ*/
static TSC2PMsgLayout gs_ly_GEN_FINGERPRINT_SHA_1_REQ = {
	C2P_MN_GEN_FINGERPRINT_SHA_1_F7,C2P_LY_NB_FLD_GEN_FINGERPRINT_SHA_1_REQ,2048,C2P_FN_CMD_REQ_GEN_FINGERPRINT_SHA_1_F7,
	{
		/*{C2P_FN_CMD_REQ_GEN_FINGERPRINT_SHA_1_F7	},*/
		{C2P_FN_FINGERPRINT_GEN_DATA				},
		{C2P_FN_HASH_ALGO							},
	}
};
/* GEN_FINGERPRINT_SHA_1_RESP */
static TSC2PMsgLayout gs_ly_GEN_FINGERPRINT_SHA_1_RESP = {
	C2P_MN_GEN_FINGERPRINT_SHA_1_FF17,C2P_LY_NB_FLD_GEN_FINGERPRINT_SHA_1_RESP,2048,C2P_FN_CMD_RESP_GEN_FINGERPRINT_SHA_1_FF17,
	{
		/*{C2P_FN_CMD_RESP_GEN_FINGERPRINT_SHA_1_FF17		},*/
		{C2P_FN_RETURN_CODE								},
		{C2P_FN_COMPL_INFO								},
		{C2P_FN_FINGERPRINT								},
	}
};


/* GEN_HASH_REQ */
static TSC2PMsgLayout gs_ly_GEN_HASH_REQ = {
	C2P_MN_GEN_HASH_F8,C2P_LY_NB_FLD_GEN_HASH_REQ,2048,C2P_FN_CMD_REQ_GEN_HASH_F8,
	{
		/*{C2P_FN_CMD_REQ_GEN_HASH_F8				},*/
		{C2P_FN_PAD_ALGO_ID						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_DIVER_VALUE						},
		{C2P_FN_MAC_DATA						},
		{C2P_FN_MAC_ALGO_ID_2					},
		{C2P_FN_MAC_LEN							},
		{C2P_FN_ENC_STEP						},
		{C2P_FN_ENC_INIT_VECTOR					},
		{C2P_FN_REPORT_DATA						},
	}
};
/* GEN_HASH_RESP */
static TSC2PMsgLayout gs_ly_GEN_HASH_RESP = {
	C2P_MN_GEN_HASH_FF18,C2P_LY_NB_FLD_GEN_HASH_RESP,2048,C2P_FN_CMD_RESP_GEN_HASH_FF18,
	{
		/*{C2P_FN_CMD_RESP_GEN_HASH_FF18			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_MAC_LEN							},
		{C2P_FN_MAC								},
		{C2P_FN_ENC_INIT_VECTOR					},
		{C2P_FN_REPORT_DATA						},
	}
};


/* VERIFY_HASH_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_HASH_REQ = {
	C2P_MN_VERIFY_HASH_F9,C2P_LY_NB_FLD_VERIFY_HASH_REQ,2048,C2P_FN_CMD_REQ_VERIFY_HASH_F9,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_HASH_F9			},*/
		{C2P_FN_PAD_ALGO_ID						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_MAC_DATA						},
		{C2P_FN_MAC_ALGO_ID_2					},
		{C2P_FN_MAC_LEN							},
		{C2P_FN_ENC_INIT_VECTOR					},
		{C2P_FN_REPORT_DATA						},
		{C2P_FN_MAC								},
	}
};
/* VERIFY_HASH_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_HASH_RESP = {
	C2P_MN_VERIFY_HASH_FF19,C2P_LY_NB_FLD_VERIFY_HASH_RESP,2048,C2P_FN_CMD_RESP_VERIFY_HASH_FF19,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_HASH_FF19		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
	}
};


/* GEN_AUTH_NO_REQ */
static TSC2PMsgLayout gs_ly_GEN_AUTH_NO_REQ = {
	C2P_MN_GEN_AUTH_NO_FA,C2P_LY_NB_FLD_GEN_AUTH_NO_REQ,2048,C2P_FN_CMD_REQ_GEN_AUTH_NO_FA,
	{
		/*{C2P_FN_CMD_REQ_GEN_AUTH_NO_FA			},*/
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_AUTH_DATA						},
		{C2P_FN_AUTH_CODE_LEN					},

	}
};
/* GEN_AUTH_NO_RESP */
static TSC2PMsgLayout gs_ly_GEN_AUTH_NO_RESP = {
	C2P_MN_GEN_AUTH_NO_FF1A,C2P_LY_NB_FLD_GEN_AUTH_NO_RESP,2048,C2P_FN_CMD_RESP_GEN_AUTH_NO_FF1A,
	{
		/*{C2P_FN_CMD_RESP_GEN_AUTH_NO_FF1A		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_AUTH_NO							},
	}
};


/* PIN_TRSLT_REQ */
static TSC2PMsgLayout gs_ly_PIN_TRSLT_REQ = {
	C2P_MN_PIN_TRSLT_FB,C2P_LY_NB_FLD_PIN_TRSLT_REQ,2048,C2P_FN_CMD_REQ_PIN_TRSLT_FB,
	{
		/*{C2P_FN_CMD_REQ_PIN_TRSLT_FB			},*/
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_ENC_PIN_BLK						},
		{C2P_FN_PIN_LEN							},
		{C2P_FN_DEC_MODE						},
		{C2P_FN_PIN_BLK_FMT						},
		{C2P_FN_ENC_MODE						},
		{C2P_FN_PIN_BLK_FMT						},
		{C2P_FN_DIVER_PIN_BLK_IN				},
		{C2P_FN_DIVER_PIN_BLK_OUT				},
		{C2P_FN_ISO_TRACK_2						},
		{C2P_FN_ANSI2_AUTH_KEY					},
	}
};
/* PIN_TRSLT_RESP */
static TSC2PMsgLayout gs_ly_PIN_TRSLT_RESP = {
	C2P_MN_PIN_TRSLT_FF1B,C2P_LY_NB_FLD_PIN_TRSLT_RESP,2048,C2P_FN_CMD_RESP_PIN_TRSLT_FF1B,
	{
		/*{C2P_FN_CMD_RESP_PIN_TRSLT_FF1B			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_ENC_PIN_BLK						},
		{C2P_FN_PIN_BLK_NA_IN					},
		{C2P_FN_PIN_BLK_NA_OUT					},
	}
};



/* VERIFY_PIN_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_PIN_REQ = {
	C2P_MN_VERIFY_PIN_FC,C2P_LY_NB_FLD_VERIFY_PIN_REQ,2048,C2P_FN_CMD_REQ_VERIFY_PIN_FC,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_PIN_FC			},*/
		{C2P_FN_ISO_TRACK_2						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_ENC_PIN_BLK						},
		{C2P_FN_DEC_MODE						},
		{C2P_FN_PIN_BLK_FMT						},
		{C2P_FN_DIVER_PIN_BLK_IN				},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_ANSI2_AUTH_KEY					},
		{C2P_FN_CODE_VERIF_ALGO					},
		{C2P_FN_VAL_DATA						},
		{C2P_FN_TRACK2_CTRL_VALUE_OFST			},
		{C2P_FN_PIN_OFST_LEN					},
		{C2P_FN_PIN_LEN							},
	}
};
/* VERIFY_PIN_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_PIN_RESP = {
	C2P_MN_VERIFY_PIN_FF1C,C2P_LY_NB_FLD_VERIFY_PIN_RESP,2048,C2P_FN_CMD_RESP_VERIFY_PIN_FF1C,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_PIN_FF1C		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_PIN_BLK_NA_IN					},
	}
};


/* VERIFY_CVX_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_CVX_REQ = {
	C2P_MN_VERIFY_CVX_FD,C2P_LY_NB_FLD_VERIFY_CVX_REQ,2048,C2P_FN_CMD_REQ_VERIFY_CVX_FD,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_CVX_FD			},*/
		{C2P_FN_CVX_GEN_ALGO					},
		{C2P_FN_ISO_TRACK_2						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_EMV_CARD_MK_ALGO				},
		{C2P_FN_TRACK2_CTRL_VALUE_OFST			},
	}
};
/* VERIFY_CVX_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_CVX_RESP = {
	C2P_MN_VERIFY_CVX_FF1D,C2P_LY_NB_FLD_VERIFY_CVX_RESP,2048,C2P_FN_CMD_RESP_VERIFY_CVX_FF1D,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_CVX_FF1D		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
	}
};



/* VERIFY_CVX2_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_CVX2_REQ = {
	C2P_MN_VERIFY_CVX2_FE,C2P_LY_NB_FLD_VERIFY_CVX2_REQ,2048,C2P_FN_CMD_REQ_VERIFY_CVX2_FE,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_CVX2_FE			},*/
		{C2P_FN_CVX_GEN_ALGO					},
		{C2P_FN_PAN								},
		{C2P_FN_EXPIRY_DATE						},
		{C2P_FN_SERVICE_CODE					},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_CVX2							},
	}
};
/* VERIFY_CVX2_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_CVX2_RESP = {
	C2P_MN_VERIFY_CVX2_FF1E,C2P_LY_NB_FLD_VERIFY_CVX2_RESP,2048,C2P_FN_CMD_RESP_VERIFY_CVX2_FF1E,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_CVX2_FF1E		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
	}
};



/* EXPORT_KEY_REQ */
static TSC2PMsgLayout gs_ly_EXPORT_KEY_REQ = {
	C2P_MN_EXPORT_KEY_FF20,C2P_LY_NB_FLD_EXPORT_KEY_REQ,2048,C2P_FN_CMD_REQ_EXPORT_KEY_FF20,
	{
		/*{C2P_FN_CMD_REQ_EXPORT_KEY_FF20			},*/
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_KCV_TYPE						},
		{C2P_FN_ENC_MTHD_ID						},

	}
};
/* EXPORT_KEY_RESP */
static TSC2PMsgLayout gs_ly_EXPORT_KEY_RESP = {
	C2P_MN_EXPORT_KEY_FF30,C2P_LY_NB_FLD_EXPORT_KEY_RESP,2048,C2P_FN_CMD_RESP_EXPORT_KEY_FF30,
	{
		/*{C2P_FN_CMD_RESP_EXPORT_KEY_FF30		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_KEY_KEV							},
		{C2P_FN_KEY_KCV							},
	}
};



/* GEN_PVV_REQ */
static TSC2PMsgLayout gs_ly_GEN_PVV_REQ = {
	C2P_MN_GEN_PVV_FF29,C2P_LY_NB_FLD_GEN_PVV_REQ,2048,C2P_FN_CMD_REQ_GEN_PVV_FF29,
	{
		/*{C2P_FN_CMD_REQ_GEN_PVV_FF29			},*/
		{C2P_FN_ISO_TRACK_2						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_ENC_PIN_BLK						},
		{C2P_FN_PIN_BLK_FMT						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_CODE_VERIF_ALGO					},
		{C2P_FN_VAL_DATA						},
		{C2P_FN_TRACK2_CTRL_VALUE_OFST			},
		{C2P_FN_PIN_LEN							},
	}
};
/* GEN_PVV_RESP */
static TSC2PMsgLayout gs_ly_GEN_PVV_RESP = {
	C2P_MN_GEN_PVV_FF39,C2P_LY_NB_FLD_GEN_PVV_RESP,2048,C2P_FN_CMD_RESP_GEN_PVV_FF39,
	{
		/*{C2P_FN_CMD_RESP_GEN_PVV_FF39			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_ISO_TRACK_2						},
		{C2P_FN_PIN_BLK_NA_IN					},
	}
};


/* GEN_CVX_REQ */
static TSC2PMsgLayout gs_ly_GEN_CVX_REQ = {
	C2P_MN_GEN_CVX_FF2A,C2P_LY_NB_FLD_GEN_CVX_REQ,2048,C2P_FN_CMD_REQ_GEN_CVX_FF2A,
	{
		/*{C2P_FN_CMD_REQ_GEN_CVX_FF2A			},*/
		{C2P_FN_CVX_GEN_ALGO					},
		{C2P_FN_ISO_TRACK_2						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_TRACK2_CTRL_VALUE_OFST			},
	}
};
/* GEN_CVX_RESP */
static TSC2PMsgLayout gs_ly_GEN_CVX_RESP = {
	C2P_MN_GEN_CVX_FF3A,C2P_LY_NB_FLD_GEN_CVX_RESP,2048,C2P_FN_CMD_RESP_GEN_CVX_FF3A,
	{
		/*{C2P_FN_CMD_RESP_GEN_CVX_FF3A			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_ISO_TRACK_2						},
	}
};


/* GEN_CVX2_REQ */
static TSC2PMsgLayout gs_ly_GEN_CVX2_REQ = {
	C2P_MN_GEN_CVX2_FF2B,C2P_LY_NB_FLD_GEN_CVX2_REQ,2048,C2P_FN_CMD_REQ_GEN_CVX2_FF2B,
	{
		/*{C2P_FN_CMD_REQ_GEN_CVX2_FF2B			},*/
		{C2P_FN_CVX_GEN_ALGO					},
		{C2P_FN_PAN								},
		{C2P_FN_EXPIRY_DATE						},
		{C2P_FN_SERVICE_CODE					},
		{C2P_FN_KEY_ID							},
                {C2P_FN_SYM_KEY_TKN_BLOC                                },
	}
};
/* GEN_CVX2_RESP */
static TSC2PMsgLayout gs_ly_GEN_CVX2_RESP = {
	C2P_MN_GEN_CVX2_FF3B,C2P_LY_NB_FLD_GEN_CVX2_RESP,2048,C2P_FN_CMD_RESP_GEN_CVX2_FF3B,
	{
		/*{C2P_FN_CMD_RESP_GEN_CVX2_FF3B			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_CVX2							},
	}
};


/* BUILD_AUTH_TICKET_REQ */
static TSC2PMsgLayout gs_ly_BUILD_AUTH_TICKET_REQ = {
	C2P_MN_BUILD_AUTH_TICKET_FF2D,C2P_LY_NB_FLD_BUILD_AUTH_TICKET_REQ,2048,C2P_FN_CMD_REQ_BUILD_AUTH_TICKET_FF2D,
	{
		/*{C2P_FN_CMD_REQ_BUILD_AUTH_TICKET_FF2D	},*/
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_SERIAL_NO_2						},
		{C2P_FN_TICKETS_COUNTER					},
		{C2P_FN_TICKETS_PRIO					},

	}
};
/* BUILD_AUTH_TICKET_RESP */
static TSC2PMsgLayout gs_ly_BUILD_AUTH_TICKET_RESP = {
	C2P_MN_BUILD_AUTH_TICKET_FF3D,C2P_LY_NB_FLD_BUILD_AUTH_TICKET_RESP,2048,C2P_FN_CMD_RESP_BUILD_AUTH_TICKET_FF3D,
	{
		/*{C2P_FN_CMD_RESP_BUILD_AUTH_TICKET_FF3D	},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_AUTH_TICKET						},
		{C2P_FN_TICKETS_COUNTER					},
	}
};


/* VERIFY_AUTH_TICKET_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_AUTH_TICKET_REQ = {
	C2P_MN_VERIFY_AUTH_TICKET_FF2E,C2P_LY_NB_FLD_VERIFY_AUTH_TICKET_REQ,2048,C2P_FN_CMD_REQ_VERIFY_AUTH_TICKET_FF2E,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_AUTH_TICKET_FF2E	},*/
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_AUTH_TICKET						},
		{C2P_FN_SERIAL_NO_2						},
		{C2P_FN_TICKETS_COUNTER					},
		{C2P_FN_TICKETS_PRIO					},
	}
};
/* VERIFY_AUTH_TICKET_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_AUTH_TICKET_RESP = {
	C2P_MN_VERIFY_AUTH_TICKET_FF3E,C2P_LY_NB_FLD_VERIFY_AUTH_TICKET_RESP,2048,C2P_FN_CMD_RESP_VERIFY_AUTH_TICKET_FF3E,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_AUTH_TICKET_FF3E	},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_AUTH_TICKET_VERIF_CERT			},
		{C2P_FN_TICKETS_COUNTER					},
	}
};


/* GEN_SPA_HMAC_REQ */
static TSC2PMsgLayout gs_ly_GEN_SPA_HMAC_REQ = {
	C2P_MN_GEN_SPA_HMAC_FF2F,C2P_LY_NB_FLD_GEN_SPA_HMAC_REQ,2048,C2P_FN_CMD_REQ_GEN_SPA_HMAC_FF2F,
	{
		/*{C2P_FN_CMD_REQ_GEN_SPA_HMAC_FF2F		},*/
		{C2P_FN_HMAC_GEN_DATA					},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
	}
};
/* GEN_SPA_HMAC_RESP */
static TSC2PMsgLayout gs_ly_GEN_SPA_HMAC_RESP = {
	C2P_MN_GEN_SPA_HMAC_FF3F,C2P_LY_NB_FLD_GEN_SPA_HMAC_RESP,2048,C2P_FN_CMD_RESP_GEN_SPA_HMAC_FF3F,
	{
		/*{C2P_FN_CMD_RESP_GEN_SPA_HMAC_FF3F		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_HMAC							},
	}
};


/* VERIFY_SPA_HMAC_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_SPA_HMAC_REQ = {
	C2P_MN_VERIFY_SPA_HMAC_FF42,C2P_LY_NB_FLD_VERIFY_SPA_HMAC_REQ,2048,C2P_FN_CMD_REQ_VERIFY_SPA_HMAC_FF42,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_SPA_HMAC_FF42	},*/
		{C2P_FN_HMAC_GEN_DATA					},
		{C2P_FN_HMAC_LEN						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_HMAC							},
	}
};
/* VERIFY_SPA_HMAC_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_SPA_HMAC_RESP = {
	C2P_MN_VERIFY_SPA_HMAC_FF52,C2P_LY_NB_FLD_VERIFY_SPA_HMAC_RESP,2048,C2P_FN_CMD_RESP_VERIFY_SPA_HMAC_FF52,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_SPA_HMAC_FF52	},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
	}
};


/* VERIFY_CSC_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_CSC_REQ = {
	C2P_MN_VERIFY_CSC_FF43,C2P_LY_NB_FLD_VERIFY_CSC_REQ,2048,C2P_FN_CMD_REQ_VERIFY_CSC_FF43,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_CSC_FF43			},*/
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_PAN								},
		{C2P_FN_EXPIRY_DATE						},
		{C2P_FN_SERVICE_CODE					},
		{C2P_FN_CSC_3							},
		{C2P_FN_CSC_4							},
		{C2P_FN_CSC_5							},
	}
};
/* VERIFY_CSC_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_CSC_RESP = {
	C2P_MN_VERIFY_CSC_FF53,C2P_LY_NB_FLD_VERIFY_CSC_RESP,2048,C2P_FN_CMD_RESP_VERIFY_CSC_FF53,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_CSC_FF53		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
	}
};


/* GEN_CSC_REQ */
static TSC2PMsgLayout gs_ly_GEN_CSC_REQ = {
	C2P_MN_GEN_CSC_FF44,C2P_LY_NB_FLD_GEN_CSC_REQ,2048,C2P_FN_CMD_REQ_GEN_CSC_FF44,
	{
		/*{C2P_FN_CMD_REQ_GEN_CSC_FF44			},*/
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_PAN								},
		{C2P_FN_EXPIRY_DATE						},
		{C2P_FN_SERVICE_CODE					},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
	}
};
/* GEN_CSC_RESP */
static TSC2PMsgLayout gs_ly_GEN_CSC_RESP = {
	C2P_MN_GEN_CSC_FF54,C2P_LY_NB_FLD_GEN_CSC_RESP,2048,C2P_FN_CMD_RESP_GEN_CSC_FF54,
	{
		/*{C2P_FN_CMD_RESP_GEN_CSC_FF54			},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_CSC_3							},
		{C2P_FN_CSC_4							},
		{C2P_FN_CSC_5							},
		{C2P_FN_ENC_CSC_CHAIN					},
	}
};


/* GEN_CARD_KEY_KCV_REQ */
static TSC2PMsgLayout gs_ly_GEN_CARD_KEY_KCV_REQ = {
	C2P_MN_GEN_CARD_KEY_KCV_FF45,C2P_LY_NB_FLD_GEN_CARD_KEY_KCV_REQ,2048,C2P_FN_CMD_REQ_GEN_CARD_KEY_KCV_FF45,
	{
		/*{C2P_FN_CMD_REQ_GEN_CARD_KEY_KCV_FF45	},*/
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_CLASS							},
		{C2P_FN_KEY_KEV							},
		{C2P_FN_KEY_KEV							},
		{C2P_FN_CARD_KEY_KCV_LEN				},
	}
};
/* GEN_CARD_KEY_KCV_RESP */
static TSC2PMsgLayout gs_ly_GEN_CARD_KEY_KCV_RESP = {
	C2P_MN_GEN_CARD_KEY_KCV_FF55,C2P_LY_NB_FLD_GEN_CARD_KEY_KCV_RESP,2048,C2P_FN_CMD_RESP_GEN_CARD_KEY_KCV_FF55,
	{
		/*{C2P_FN_CMD_RESP_GEN_CARD_KEY_KCV_FF55	},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_CARD_KEY_KCV					},
	}
};


/* VERIFY_CVC3_REQ */
static TSC2PMsgLayout gs_ly_VERIFY_CVC3_REQ = {
	C2P_MN_VERIFY_CVC3_FF8108,C2P_LY_NB_FLD_VERIFY_CVC3_REQ,2048,C2P_FN_CMD_REQ_VERIFY_CVC3_FF8108,
	{
		/*{C2P_FN_CMD_REQ_VERIFY_CVC3_FF8108		},*/
		{C2P_FN_ISO_TRACK_2						},
		{C2P_FN_ISO_TRACK_1						},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_EMV_CARD_MK_ALGO				},
		{C2P_FN_CVC3_TRACK2_BITMAP				},
		{C2P_FN_UN_ATC_TRACK2_BITMAP			},
		{C2P_FN_CVC3_TRACK2_BITMAP				},
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_CVC3_TRACK1_BITMAP				},
		{C2P_FN_UN_ATC_TRACK1_BITMAP			},
		{C2P_FN_PAN_SEQ_NO						},
		{C2P_FN_ATC								},
	}
};
/* VERIFY_CVC3_RESP */
static TSC2PMsgLayout gs_ly_VERIFY_CVC3_RESP = {
	C2P_MN_VERIFY_CVC3_FF8118,C2P_LY_NB_FLD_VERIFY_CVC3_RESP,2048,C2P_FN_CMD_RESP_VERIFY_CVC3_FF8118,
	{
		/*{C2P_FN_CMD_RESP_VERIFY_CVC3_FF8118		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_ATC								},
	}
};


/* GEN_IV_CVC3_REQ */
static TSC2PMsgLayout gs_ly_GEN_IV_CVC3_REQ = {
	C2P_MN_GEN_IV_CVC3_FF8109,C2P_LY_NB_FLD_GEN_IV_CVC3_REQ,2048,C2P_FN_CMD_REQ_GEN_IV_CVC3_FF8109,
	{
		/*{C2P_FN_CMD_REQ_GEN_IV_CVC3_FF8109		},*/
		{C2P_FN_KEY_ID							},
		{C2P_FN_SYM_KEY_TKN_BLOC				},
		{C2P_FN_EMV_CARD_MK_ALGO				},
		{C2P_FN_PAN								},
		{C2P_FN_PAN_SEQ_NO						},
		{C2P_FN_IVCVC3_INIT_VEC_DATA			},
	}
};
/* GEN_IV_CVC3_RESP */
static TSC2PMsgLayout gs_ly_GEN_IV_CVC3_RESP = {
	C2P_MN_GEN_IV_CVC3_FF8119,C2P_LY_NB_FLD_GEN_IV_CVC3_RESP,2048,C2P_FN_CMD_RESP_GEN_IV_CVC3_FF8119,
	{
		/*{C2P_FN_CMD_RESP_GEN_IV_CVC3_FF8119		},*/
		{C2P_FN_RETURN_CODE						},
		{C2P_FN_COMPL_INFO						},
		{C2P_FN_IVCVC3							},
	}
};

/* None */  
static TSC2PMsgLayout gs_ly_none = {
	C2P_LAYOUT_TYPE_QTY,0,2048,C2P_CMD_CODE_QTY,
	NULL
};
/*  
static TSC2PMsgLayout gs_ly_ = {
	,,2048,
	{
		
	}
};*/

/* GEN_RSA_KEY*/
static TSC2PMsgLayout gs_ly_GEN_RSA_REQ = {
        C2P_MN_GEN_RSA_F0,C2P_LY_NB_FLD_GEN_RSA_REQ,2048,C2P_FN_CMD_REQ_GEN_RSA_F0,
        {
                /*{C2P_FN_CMD_REQ_GEN_RSA_F0                  },*/
                {C2P_FN_KEY_ID                               },
                {C2P_FN_RSA_KEY_MOD_LEN                                               },
                {C2P_FN_RSA_PUB_KEY_EXP                                    },
                {C2P_FN_KEY_ID                                 },
                {C2P_FN_RSA_PRIV_KEY_FMT_ID                              },
                {C2P_FN_KEY_USAGE                                   },
                {C2P_FN_USAGE_MODE                                              },
                {C2P_FN_TOKEN_RANGE                                               },
                {C2P_FN_KEY_EXP_DATE                                        },
                {C2P_FN_KEY_ACTIVATION_DATE                                        },
                {C2P_FN_GROUP_ID                                         },
                {C2P_FN_INIT_PWD                                       },
                {C2P_FN_ACTIV_SIG_NBR                                       },
                {C2P_FN_AUTHEN_MTHD                                      }


        }
};

/* GEN_RSA_KEY_RESP */
static TSC2PMsgLayout gs_ly_GEN_RSA_RESP = {
        C2P_MN_GEN_RSA_FF10,C2P_LY_NB_FLD_GEN_RSA_RESP,2048,C2P_FN_CMD_RESP_GEN_RSA_FF10,
        {
                /*{C2P_FN_CMD_RESP_GEN_RSA_FF10           },*/
                {C2P_FN_RETURN_CODE                                             },
                {C2P_FN_COMPL_INFO                                              },
                {C2P_FN_PRIV_KEY_BLOC                                                  },
        }

};

/* IMP_SEC_KEY_RSA_REQ */
static TSC2PMsgLayout gs_ly_IMP_SEC_KEY_RSA_REQ = {
        C2P_MN_IMP_SEC_KEY_RSA_FF8123,C2P_LY_NB_FLD_IMP_SEC_KEY_RSA_REQ,2048,C2P_FN_CMD_REQ_IMP_SEC_KEY_RSA_FF8123,
        {
                /*{C2P_FN_CMD_REQ_IMP_SEC_KEY_RSA_FF8123                            },*/
					{C2P_FN_KEY_ID                              },
					{C2P_FN_PRIV_KEY_BLOC                       },
					{C2P_FN_KEY_ID                              },
                    {C2P_FN_KEY_USAGE                           },
                    {C2P_FN_USAGE_MODE                          },
                    {C2P_FN_TOKEN_RANGE                         },
                    {C2P_FN_KEY_EXP_DATE                        },
                    {C2P_FN_KEY_ACTIVATION_DATE                 },
                    {C2P_FN_GROUP_ID                            },
                    {C2P_FN_CLASS                               },
                    {C2P_FN_KEY_KCV                             },
                    {C2P_FN_RSA_PUB_KEY_ENC_KEY                 },
                    {C2P_FN_RSA_ENC_ALGO_ID                     },
                    {C2P_FN_HASH_ALGO                           },
                    {C2P_FN_MGF_GEN_FUNC                        },
                    {C2P_FN_MSG_LABEL                           },
                    {C2P_FN_KEY_ID                              }

        }
};
/* IMP_SEC_KEY_RSA_RESP */
static TSC2PMsgLayout gs_ly_IMP_SEC_KEY_RSA_RESP = {
        C2P_MN_IMP_SEC_KEY_RSA_FF8133,C2P_LY_NB_FLD_IMP_SEC_KEY_RSA_RESP,2048,C2P_FN_CMD_RESP_IMP_SEC_KEY_RSA_FF8133,
        {
                /*{C2P_FN_CMD_RESP_IMP_SEC_KEY_RSA_FF8133                 },*/
                {C2P_FN_RETURN_CODE                                             },
                {C2P_FN_COMPL_INFO                                              },
                {C2P_FN_SYM_KEY_TKN_BLOC                                }
        }
};

/*EXP_SEC_KEY_RSA_REQ*/
static TSC2PMsgLayout gs_ly_EXP_SEC_KEY_RSA_REQ = {
        C2P_MN_EXP_SEC_KEY_RSA_FF8124,C2P_LY_NB_FLD_EXP_SEC_KEY_RSA_REQ,2048,C2P_FN_CMD_REQ_EXP_SEC_KEY_RSA_FF8124,
        {
                /*{C2P_FN_CMD_REQ_EXP_SEC_KEY_RSA_FF8124                            },*/
               {C2P_FN_SYM_KEY_TKN_BLOC                        },                          
               {C2P_FN_RSA_ENC_ALGO_ID                        },                          
               {C2P_FN_HASH_ALGO                        },                          
               {C2P_FN_MGF_GEN_FUNC                        },                          
               {C2P_FN_MSG_LABEL                        },                          
               {C2P_FN_KEY_ID                        },                          
               {C2P_FN_CERT_TKN_BLOC                        },                          
               {C2P_FN_RSA_PUB_KEY_BLOC                        },                          
               {C2P_FN_KCV_TYPE                        }                          

        }
};
/* EXP_SEC_KEY_RSA_RESP */
static TSC2PMsgLayout gs_ly_EXP_SEC_KEY_RSA_RESP = {
        C2P_MN_EXP_SEC_KEY_RSA_FF8134,C2P_LY_NB_FLD_EXP_SEC_KEY_RSA_RESP,2048,C2P_FN_CMD_RESP_EXP_SEC_KEY_RSA_FF8134,
        {
                /*{C2P_FN_CMD_RESP_EXP_SEC_KEY_RSA_FF8134                 },*/
                {C2P_FN_RETURN_CODE                                             },
                {C2P_FN_COMPL_INFO                                              },
                {C2P_FN_RSA_PUB_KEY_ENC_KEY                        },                          
                {C2P_FN_KEY_KCV                        }                         

        }
};
/*ENCRYPT_RSA_REQ*/
static TSC2PMsgLayout gs_ly_ENCRYPT_RSA_REQ = {
        C2P_MN_ENCRYPT_RSA_FF4C,C2P_LY_NB_FLD_ENCRYPT_RSA_REQ,2048,C2P_FN_CMD_REQ_ENCRYPT_RSA_FF4C,
        {
                /*{C2P_FN_CMD_REQ_ENCRYPT_RSA_FF4C                            },*/
              {C2P_FN_RSA_ENC_ALGO_ID                        },                          
              {C2P_FN_RSA_PUB_KEY_BLOC                        },                          
              {C2P_FN_CERT_TKN_BLOC                        },                          
              {C2P_FN_RSA_ENC_DATA_IN                        }                          
                        
        }
};
/* ENCRYPT_RSA_RESP */
static TSC2PMsgLayout gs_ly_ENCRYPT_RSA_RESP = {
        C2P_MN_ENCRYPT_RSA_FF5C,C2P_LY_NB_FLD_ENCRYPT_RSA_RESP,2048,C2P_FN_CMD_RESP_ENCRYPT_RSA_FF5C,
        {
                /*{C2P_FN_CMD_RESP_ENCRYPT_RSA_FF5C                 },*/
                {C2P_FN_RETURN_CODE                        },                          
				{C2P_FN_COMPL_INFO                        },                          
				{C2P_FN_RSA_ENC_DATA_OUT                        }                          
                    
        }
};

/*DECRYPT_RSA_REQ*/
static TSC2PMsgLayout gs_ly_DECRYPT_RSA_REQ = {
        C2P_MN_DECRYPT_RSA_FF8201,C2P_LY_NB_FLD_DECRYPT_RSA_REQ,2048,C2P_FN_CMD_REQ_DECRYPT_RSA_FF8201,
        {
                /*{C2P_FN_CMD_REQ_DECRYPT_RSA_FF8124                            },*/
                {C2P_FN_RSA_ENC_ALGO_ID                        },                          
                {C2P_FN_HASH_ALGO                        },                          
                {C2P_FN_MGF_GEN_FUNC                        },                          
                {C2P_FN_MSG_LABEL                        },                          
                {C2P_FN_KEY_ID                        },                          
                {C2P_FN_PRIV_KEY_BLOC                        },                          
                {C2P_FN_RSA_ENC_DATA_OUT                        },                         
                        
                        
        }
};
/* DECRYPT_RSA_RESP */
static TSC2PMsgLayout gs_ly_DECRYPT_RSA_RESP = {
        C2P_MN_DECRYPT_RSA_FF8211,C2P_LY_NB_FLD_DECRYPT_RSA_RESP,2048,C2P_FN_CMD_RESP_DECRYPT_RSA_FF8211,
        {
                /*{C2P_FN_CMD_RESP_DECRYPT_RSA_FF8134                 },*/
                {C2P_FN_RETURN_CODE                        },                          
				{C2P_FN_COMPL_INFO                        },                          
				{C2P_FN_RSA_ENC_DATA_IN                        }                        
                                             
        }
};

/*DECRYPT_ASYM_KEY_REQ*/
static TSC2PMsgLayout gs_ly_DECRYPT_ASYM_KEY_REQ = {
        C2P_MN_DECRYPT_ASYM_KEY_FF6B,C2P_LY_NB_FLD_DECRYPT_ASYM_KEY_REQ,2048,C2P_FN_CMD_REQ_DECRYPT_ASYM_KEY_FF6B,
        {
                /*{C2P_FN_CMD_REQ_DECRYPT_ASYM_KEY_FF6B                            },*/
                  {C2P_FN_PRIV_KEY_BLOC 						},
                  {C2P_FN_RSA_PRIV_KEY_FMT_ID 					},
                  {C2P_FN_ENC_KEY_ALGO_ID 						},
                  {C2P_FN_KEY_USAGE 							},
                  {C2P_FN_USAGE_MODE 							},
                  {C2P_FN_TOKEN_RANGE 							},
                  {C2P_FN_KEY_EXP_DATE							},
                  {C2P_FN_KEY_ACTIVATION_DATE 					},
                  {C2P_FN_GROUP_ID 								},
                  {C2P_FN_KEY_ID 								},
                  {C2P_FN_DISTR_ACC_FLAG 						}

             
        }
};
/* DECRYPT_ASYM_KEY_RESP */
static TSC2PMsgLayout gs_ly_DECRYPT_ASYM_KEY_RESP = {
        C2P_MN_DECRYPT_ASYM_KEY_FF7B,C2P_LY_NB_FLD_DECRYPT_ASYM_KEY_RESP,2048,C2P_FN_CMD_RESP_DECRYPT_ASYM_KEY_FF7B,
        {
                /*{C2P_FN_CMD_RESP_DECRYPT_ASYM_KEY_FF7B                 },*/
                {C2P_FN_RETURN_CODE                        },                          
				{C2P_FN_COMPL_INFO                         },                          
				{C2P_FN_PRIV_KEY_BLOC 					   }                      
                                             
        }
};

/*EXT_PUBLIC_KEY_REQ*/
static TSC2PMsgLayout gs_ly_EXT_PUBLIC_KEY_REQ = {
        C2P_MN_EXT_PUBLIC_KEY_F1,C2P_LY_NB_FLD_EXT_PUBLIC_KEY_REQ,2048,C2P_FN_CMD_REQ_EXT_PUBLIC_KEY_F1,
        {
                /*{C2P_FN_CMD_REQ_EXT_PUBLIC_KEY_F1                            },*/
                  {C2P_FN_KEY_ID },
                  {C2P_FN_PRIV_KEY_BLOC },
                  {C2P_FN_RSA_PUB_KEY_EXT_FMT_ID },
                  {C2P_FN_PUB_KEY_FINGERPRINT_REQ }
        }
};
/* EXT_PUBLIC_KEY_RESP */
static TSC2PMsgLayout gs_ly_EXT_PUBLIC_KEY_RESP = {
        C2P_MN_EXT_PUBLIC_KEY_FF11,C2P_LY_NB_FLD_EXT_PUBLIC_KEY_RESP,2048,C2P_FN_CMD_RESP_EXT_PUBLIC_KEY_FF11,
        {
                /*{C2P_FN_CMD_RESP_EXT_PUBLIC_KEY_FF11                 },*/
                  {C2P_FN_RETURN_CODE },
                  {C2P_FN_COMPL_INFO },
                  {C2P_FN_RSA_PUB_KEY_BLOC },
                  {C2P_FN_ECDSA_PUB_KEY_BLOC },
                  {C2P_FN_DSA_PUB_KEY_BLOC },
                  {C2P_FN_PUB_KEY_HASH }                                              
        }
};
/* TODO: remaining layouts */



static TSC2PMsgLayout* tab_c2p_layouts[C2P_LAYOUT_TYPE_QTY] = {

&gs_ly_UNKOWN_RESP,							/*	C2P_MN_UNKOWN_FF01						*/
&gs_ly_VERIFY_AC_REQ,                       /*	C2P_MN_VERIFY_AC_E2						*/
&gs_ly_VERIFY_AC_RESP,                      /*	C2P_MN_VERIFY_AC_FF02					*/
&gs_ly_GEN_ARPC_REQ,                        /*	C2P_MN_GEN_ARPC_E3						*/
&gs_ly_GEN_ARPC_RESP,                       /*	C2P_MN_GEN_ARPC_FF03					*/
&gs_ly_GEN_MAC_REQ,                         /*	C2P_MN_GEN_MAC_E4						*/
&gs_ly_GEN_MAC_RESP,                        /*	C2P_MN_GEN_MAC_FF04						*/
&gs_ly_ENC_DATA_REQ,                        /*	C2P_MN_ENC_DATA_E5						*/
&gs_ly_ENC_DATA_RESP,                       /*	C2P_MN_ENC_DATA_FF05					*/
&gs_ly_VERIFY_DAC_REQ,                      /*	C2P_MN_VERIFY_DAC_E6					*/
&gs_ly_VERIFY_DAC_RESP,                     /*	C2P_MN_VERIFY_DAC_FF06					*/
&gs_ly_VERIFY_IDN_REQ,                      /*	C2P_MN_VERIFY_IDN_E7					*/
&gs_ly_VERIFY_IDN_RESP,                     /*	C2P_MN_VERIFY_IDN_FF07					*/
&gs_ly_PIN_CHG_REQ,                         /*	C2P_MN_PIN_CHG_E8						*/
&gs_ly_PIN_CHG_RESP,                        /*	C2P_MN_PIN_CHG_FF08						*/
&gs_ly_GEN_KEY_REQ,                         /*	C2P_MN_GEN_KEY_E9						*/
&gs_ly_GEN_KEY_RESP,                        /*	C2P_MN_GEN_KEY_FF09						*/
&gs_ly_SAVE_KEY_REQ,                        /*	C2P_MN_SAVE_KEY_EA						*/
&gs_ly_SAVE_KEY_RESP,                       /*	C2P_MN_SAVE_KEY_FF0A					*/
&gs_ly_IMPORT_KEY_REQ,                      /*	C2P_MN_IMPORT_KEY_EB					*/
&gs_ly_IMPORT_KEY_RESP,                     /*	C2P_MN_IMPORT_KEY_FF0B					*/
&gs_ly_DEL_KEY_REQ,                         /*	C2P_MN_DEL_KEY_EC						*/
&gs_ly_DEL_KEY_RESP,                        /*	C2P_MN_DEL_KEY_FF0C						*/
&gs_ly_VERIFY_KEY_REQ,                      /*	C2P_MN_VERIFY_KEY_ED					*/
&gs_ly_VERIFY_KEY_RESP,                     /*	C2P_MN_VERIFY_KEY_FF0D					*/
&gs_ly_GEN_RSA_REQ,                         /*	C2P_MN_GEN_RSA_F0						*/
&gs_ly_GEN_RSA_RESP,                        /*	C2P_MN_GEN_RSA_FF10					    */
&gs_ly_EXT_PUBLIC_KEY_REQ,                  /*	C2P_MN_EXT_PUBLIC_KEY_F1				*/
&gs_ly_EXT_PUBLIC_KEY_RESP,                 /*	C2P_MN_EXT_PUBLIC_KEY_FF11			    */
&gs_ly_AUTO_TEST_REQ,                       /*	C2P_MN_AUTO_TEST_F2						*/
&gs_ly_AUTO_TEST_RESP,                      /*	C2P_MN_AUTO_TEST_FF12					*/
&gs_ly_STAT_REPORT_REQ,                     /*	C2P_MN_STAT_REPORT_F3					*/
&gs_ly_STAT_REPORT_RESP,                    /*	C2P_MN_STAT_REPORT_FF13					*/
&gs_ly_none,                                /*	C2P_MN_RFU_F4_F4						*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF14_FF14					*/
&gs_ly_GEN_SIG_REQ,                         /*	C2P_MN_GEN_SIG_F5						*/
&gs_ly_GEN_SIG_RESP,                        /*	C2P_MN_GEN_SIG_FF15						*/
&gs_ly_VERIFY_SIG_REQ,                      /*	C2P_MN_VERIFY_SIG_F6					*/
&gs_ly_VERIFY_SIG_RESP,                     /*	C2P_MN_VERIFY_SIG_FF16					*/
&gs_ly_GEN_FINGERPRINT_SHA_1_REQ,           /*	C2P_MN_GEN_FINGERPRINT_SHA_1_F7			*/
&gs_ly_GEN_FINGERPRINT_SHA_1_RESP,          /*	C2P_MN_GEN_FINGERPRINT_SHA_1_FF17		*/
&gs_ly_GEN_HASH_REQ,                        /*	C2P_MN_GEN_HASH_F8						*/
&gs_ly_GEN_HASH_RESP,                       /*	C2P_MN_GEN_HASH_FF18					*/
&gs_ly_VERIFY_HASH_REQ,                     /*	C2P_MN_VERIFY_HASH_F9					*/
&gs_ly_VERIFY_HASH_RESP,                    /*	C2P_MN_VERIFY_HASH_FF19					*/
&gs_ly_GEN_AUTH_NO_REQ,                     /*	C2P_MN_GEN_AUTH_NO_FA					*/
&gs_ly_GEN_AUTH_NO_RESP,                    /*	C2P_MN_GEN_AUTH_NO_FF1A					*/
&gs_ly_PIN_TRSLT_REQ,                       /*	C2P_MN_PIN_TRSLT_FB						*/
&gs_ly_PIN_TRSLT_RESP,                      /*	C2P_MN_PIN_TRSLT_FF1B					*/
&gs_ly_VERIFY_PIN_REQ,                      /*	C2P_MN_VERIFY_PIN_FC					*/
&gs_ly_VERIFY_PIN_RESP,                     /*	C2P_MN_VERIFY_PIN_FF1C					*/
&gs_ly_VERIFY_CVX_REQ,                      /*	C2P_MN_VERIFY_CVX_FD					*/
&gs_ly_VERIFY_CVX_RESP,                     /*	C2P_MN_VERIFY_CVX_FF1D					*/
&gs_ly_VERIFY_CVX2_REQ,                     /*	C2P_MN_VERIFY_CVX2_FE					*/
&gs_ly_VERIFY_CVX2_RESP,                    /*	C2P_MN_VERIFY_CVX2_FF1E					*/
&gs_ly_EXPORT_KEY_REQ,                      /*	C2P_MN_EXPORT_KEY_FF20					*/
&gs_ly_EXPORT_KEY_RESP,                     /*	C2P_MN_EXPORT_KEY_FF30					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF21_FF21					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF31_FF31					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF22_FF22					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF32_FF32					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF23_FF23					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF33_FF33					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF24_FF24					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF34_FF34					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF25_FF25					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF35_FF35					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF26_FF26					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF36_FF36					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF27_FF27					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF37_FF37					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF28_FF28					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF38_FF38					*/
&gs_ly_GEN_PVV_REQ,                         /*	C2P_MN_GEN_PVV_FF29						*/
&gs_ly_GEN_PVV_RESP,                        /*	C2P_MN_GEN_PVV_FF39						*/
&gs_ly_GEN_CVX_REQ,                         /*	C2P_MN_GEN_CVX_FF2A						*/
&gs_ly_GEN_CVX_RESP,                        /*	C2P_MN_GEN_CVX_FF3A						*/
&gs_ly_GEN_CVX2_REQ,                        /*	C2P_MN_GEN_CVX2_FF2B					*/
&gs_ly_GEN_CVX2_RESP,                       /*	C2P_MN_GEN_CVX2_FF3B					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF2C_FF2C					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF3C_FF3C					*/
&gs_ly_BUILD_AUTH_TICKET_REQ,               /*	C2P_MN_BUILD_AUTH_TICKET_FF2D			*/
&gs_ly_BUILD_AUTH_TICKET_RESP,              /*	C2P_MN_BUILD_AUTH_TICKET_FF3D			*/
&gs_ly_VERIFY_AUTH_TICKET_REQ,              /*	C2P_MN_VERIFY_AUTH_TICKET_FF2E			*/
&gs_ly_VERIFY_AUTH_TICKET_RESP,             /*	C2P_MN_VERIFY_AUTH_TICKET_FF3E			*/
&gs_ly_GEN_SPA_HMAC_REQ,                    /*	C2P_MN_GEN_SPA_HMAC_FF2F				*/
&gs_ly_GEN_SPA_HMAC_RESP,                   /*	C2P_MN_GEN_SPA_HMAC_FF3F				*/
&gs_ly_VERIFY_SPA_HMAC_REQ,                 /*	C2P_MN_VERIFY_SPA_HMAC_FF42				*/
&gs_ly_VERIFY_SPA_HMAC_RESP,                /*	C2P_MN_VERIFY_SPA_HMAC_FF52				*/
&gs_ly_VERIFY_CSC_REQ,                      /*	C2P_MN_VERIFY_CSC_FF43					*/
&gs_ly_VERIFY_CSC_RESP,                     /*	C2P_MN_VERIFY_CSC_FF53					*/
&gs_ly_GEN_CSC_REQ,                         /*	C2P_MN_GEN_CSC_FF44						*/
&gs_ly_GEN_CSC_RESP,                        /*	C2P_MN_GEN_CSC_FF54						*/
&gs_ly_GEN_CARD_KEY_KCV_REQ,                /*	C2P_MN_GEN_CARD_KEY_KCV_FF45			*/
&gs_ly_GEN_CARD_KEY_KCV_RESP,               /*	C2P_MN_GEN_CARD_KEY_KCV_FF55			*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF46_FF46					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF56_FF56					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF47_FF47					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF57_FF57					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF48_FF48					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF58_FF58					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF49_FF49					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF59_FF59					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF4A_FF4A					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF5A_FF5A					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF4B_FF4B					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF5B_FF5B					*/
&gs_ly_ENCRYPT_RSA_REQ,                     /*	C2P_MN_ENCRYPT_RSA_FF4C					*/
&gs_ly_ENCRYPT_RSA_RESP,                    /*	C2P_MN_ENCRYPT_RSA_FF5C					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF4D_FF4D					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF5D_FF5D					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF4E_FF4E					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF5E_FF5E					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF4F_FF4F					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF5F_FF5F					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF60_FF60					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF70_FF70					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF61_FF61					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF71_FF71					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF62_FF62					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF72_FF72					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF63_FF63					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF73_FF73					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF64_FF64					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF74_FF74					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF65_FF65					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF75_FF75					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF66_FF66					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF76_FF76					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF67_FF67					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF77_FF77					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF68_FF68					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF78_FF78					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF69_FF69					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF79_FF79					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF6A_FF6A					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF7A_FF7A					*/
&gs_ly_DECRYPT_ASYM_KEY_REQ,                 /*	C2P_MN_DECRYPT_ASYM_KEY_FF6B				*/
&gs_ly_DECRYPT_ASYM_KEY_RESP,                /*	C2P_MN_DECRYPT_ASYM_KEY_FF7B				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF6C_FF6C					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF7C_FF7C					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF6D_FF6D					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF7D_FF7D					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF6E_FF6E					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF7E_FF7E					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF6F_FF6F					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF7F_FF7F					*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8100_FF8100				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8110_FF8110				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8101_FF8101				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8111_FF8111				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8102_FF8102				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8112_FF8112				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8103_FF8103				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8113_FF8113				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8104_FF8104				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8114_FF8114				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8105_FF8105				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8115_FF8115				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8106_FF8106				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8116_FF8116				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8107_FF8107				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8117_FF8117				*/
&gs_ly_VERIFY_CVC3_REQ,                     /*	C2P_MN_VERIFY_CVC3_FF8108				*/
&gs_ly_VERIFY_CVC3_RESP,                    /*	C2P_MN_VERIFY_CVC3_FF8118				*/
&gs_ly_GEN_IV_CVC3_REQ,                     /*	C2P_MN_GEN_IV_CVC3_FF8109				*/
&gs_ly_GEN_IV_CVC3_RESP,                    /*	C2P_MN_GEN_IV_CVC3_FF8119				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF810A_FF810A				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF811A_FF811A				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF810B_FF810B				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF811B_FF811B				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF810C_FF810C				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF811C_FF811C				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF810D_FF810D				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF811D_FF811D				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF810E_FF810E				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF811E_FF811E				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF810F_FF810F				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF811F_FF811F				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8120_FF8120				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8130_FF8130				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8121_FF8121				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8131_FF8131				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8122_FF8122				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8132_FF8132				*/
&gs_ly_IMP_SEC_KEY_RSA_REQ,                 /*  C2P_MN_IMP_SEC_KEY_RSA_FF8123                           */
&gs_ly_IMP_SEC_KEY_RSA_RESP,                /*  C2P_MN_IMP_SEC_KEY_RSA_FF8133                           */
&gs_ly_EXP_SEC_KEY_RSA_REQ,                 /*  C2P_MN_EXP_SEC_KEY_RSA_FF8124                           */
&gs_ly_EXP_SEC_KEY_RSA_RESP,                /*  C2P_MN_EXP_SEC_KEY_RSA_FF8134                           */
&gs_ly_none,                                /*	C2P_MN_RFU_FF8125_FF8125				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8135_FF8135				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8126_FF8126				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8136_FF8136				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8127_FF8127				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8137_FF8137				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8128_FF8128				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8138_FF8138				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8129_FF8129				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8139_FF8139				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF812A_FF812A				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF813A_FF813A				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF812B_FF812B				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF813B_FF813B				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF812C_FF812C				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF813C_FF813C				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF812D_FF812D				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF813D_FF813D				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF812E_FF812E				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF813E_FF813E				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF812F_FF812F				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF813F_FF813F				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8160_FF8160				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8170_FF8170				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8161_FF8161				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8171_FF8171				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8162_FF8162				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8172_FF8172				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8163_FF8163				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8173_FF8173				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8164_FF8164				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8174_FF8174				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8165_FF8165				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8175_FF8175				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8166_FF8166				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8176_FF8176				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8167_FF8167				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8177_FF8177				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8168_FF8168				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8178_FF8178				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8169_FF8169				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8179_FF8179				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF816A_FF816A				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF817A_FF817A				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF816B_FF816B				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF817B_FF817B				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF816C_FF816C				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF817C_FF817C				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF816D_FF816D				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF817D_FF817D				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF816E_FF816E				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF817E_FF817E				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF816F_FF816F				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF817F_FF817F				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8200_FF8200				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8210_FF8210				*/
&gs_ly_DECRYPT_RSA_REQ,                     /*	C2P_MN_DECRYPT_RSA_FF8201				*/
&gs_ly_DECRYPT_RSA_RESP,                    /*	C2P_MN_DECRYPT_RSA_FF8211				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8202_FF8202				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8212_FF8212				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8203_FF8203				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8213_FF8213				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8204_FF8204				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8214_FF8214				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8205_FF8205				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8215_FF8215				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8206_FF8206				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8216_FF8216				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8207_FF8207				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8217_FF8217				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8208_FF8208				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8218_FF8218				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8209_FF8209				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8219_FF8219				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF820A_FF820A				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF821A_FF821A				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF820B_FF820B				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF821B_FF821B				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF820C_FF820C				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF821C_FF821C				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF820D_FF820D				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF821D_FF821D				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF820E_FF820E				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF821E_FF821E				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF820F_FF820F				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF821F_FF821F				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8220_FF8220				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8230_FF8230				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8221_FF8221				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8231_FF8231				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8222_FF8222				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8232_FF8232				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8223_FF8223				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8233_FF8233				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8224_FF8224				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8234_FF8234				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8225_FF8225				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8235_FF8235				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8226_FF8226				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8236_FF8236				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8227_FF8227				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8237_FF8237				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8228_FF8228				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8238_FF8238				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8229_FF8229				*/
&gs_ly_none,                                /*	C2P_MN_RFU_FF8239_FF8239				*/

};




int InitC2PCmd(TSC2PCmd* pkC2PCmd, E_C2P_LAYOUT_TYPE eCmdType)
{
	char sLine[MAX_LINE_TRC];
	TSC2PMsgLayout* pkC2PMsgLayout;

	if( eCmdType >= C2P_LAYOUT_TYPE_QTY )
	{
		sprintf(sLine,"Invalid C2P Layout (%d)",eCmdType);
		trace_event(sLine,ERROR);
		return NOK;
	}

	pkC2PMsgLayout = tab_c2p_layouts[eCmdType];

	if( pkC2PMsgLayout->eLayoutType == C2P_LAYOUT_TYPE_QTY )
	{
		sprintf(sLine,"Undefined C2P Layout (%d)",eCmdType);
		trace_event(sLine,ERROR);
	}
	ResetC2PCmdInfo(pkC2PCmd);
	pkC2PCmd->pkLayout = pkC2PMsgLayout;
	pkC2PCmd->pkProperties = &C2PFieldsDataProp;
	pkC2PCmd->pkCmdTagProperties = &C2PCmdTagProp;
	return OK;
}

