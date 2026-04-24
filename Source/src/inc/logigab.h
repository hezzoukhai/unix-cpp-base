#ifndef P7__LOGIGAB__H
#define P7__LOGIGAB__H

#ifdef WIN32
#include <p7_common.h>
#include <p7_globals.h>
#include <p7_tools.h>
#include <p7_trace.h>

#include <p7_com.h>
#else
    #ifdef SOLARIS
        #include <thread.h>
    #endif
    #include <pthread.h>
    #include <stdio.h>
    /*#include <stdlib.h>*/
    #include <string.h>
    #include <signal.h>
    #include <fcntl.h>
    #include <errno.h>
    #ifdef SOLARIS
        #include <limits.h>
    #endif

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/socketvar.h>
    #include <arpa/inet.h>
    #include <sys/un.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <netinet/tcp.h>

	#include <syslog.h>

    /* - Oracle Header Files */
    #include <sqlca.h>
    #include <sqlcpr.h>

    /* - Private Header Files */
    #include <define.h>
	#include <p7_define.h>
    #include <pwrmacros.h>
    #include <resources.h>
    #include <dump.h>
    #include <comms_param.h>
    #include <purge_time.h>
    #include <troubles_param.h>
    #include <event_log.h>
    #include <thread_var.h>
    #include <param_arg.h>
    #include <main_thr.h>
    #include <list_thr.h>
    #include <queue.h>
    #include <line_com.h>
	#include <tlv_switch_header.h>
    #include <struct_line.h>
    #include <struct_time.h>
    #include <security_data.h>
    #include <security_flags.h>
    #include <security_results.h>
    #include <hsm_inc.h>
    #include <tlv_fields.h>
    #include <tlv_private.h>
    #include <tlv_privfields.h>
    #include <tlv_autho.h>
    #include <iso_hps.h>
    #include <service.h>
    #include <hps_iso_8583.h>
	#include <p7_sid.h>
#endif


#include <iso8583.h>
#include <atmstatus.h>
#include <p7_services.h>
#include <lg_define.h>
#include <lg_fields.h>
#include <lg_states.h>
#include <openssl/rsa.h>

#define LG_MAX_MSG_FIELDS		256


typedef enum {
	LG_UNDEF		= 0	,
	LG_ALPHA			,
	LG_NUMBER			,
	LG_VAR_ALPHA		,
	LG_X_NUMBER			,
	LG_BIN			,
} E_LG_FIELD_TYPE;


typedef enum {
	LG_MF_NONE				,
	LG_MF_TERM_TO_SERVER	,
	LG_MF_SERVER_TO_TERM	,
	LG_MF_BOTH				,

} E_LG_MSG_FLOW_TP;

typedef enum {
	LGM_HEADER		= 0			,
	LGM_PERIODIC_REQ			,
	LGM_CASST_WAG				,
	LGM_CASST					,
	LGM_PERIODIC_RESP			,
	LGM_MSG_ACK_CHANNEL_2		,
	LGM_EXCEPTION_MSG			,
	LGM_REENTRY_RESP			,
	LGM_KEYS_DL_REQ_1			,
	LGM_KEYS_DL_RESP_1			,
	LGM_PARAM_FILE_DL_REQ		,
	LGM_PARAM_FILE_DL_RESP		,
	LGM_TERM_SPEC_REQ			,
	LGM_TERM_SPEC_RESP			,
	LGM_TERM_SPEC_PARAM_REQ		,
	LGM_TERM_SPEC_PARAM_RESP	,
	LGM_TRANSFER_REQ			,
	LGM_EXP_CMD_REPORT_RMT_1	, /* 11, 16, 22, 35, 53, 57, 65, 69, 72 et 75*/
	LGM_EXP_CMD_REPORT_RMT_2	, /* other than 11, 16, 22, 35, 53, 57, 65, 69, 72 et 75 */
	LGM_EXP_CMD_REPORT_RMT_RESP	,
	LGM_INIT_TLP_REQ			,
	LGM_INIT_TLP_RESP			,
	LGM_TBL_TRNSF_REQ			,
	LGM_TBL_TRNSF_RESP			,
	LGM_END_TLP_REQ				,
	LGM_END_TLP_RESP			,
	LGM_KEYS_DL_REQ_2			,
	LGM_KEYS_DL_RESP_2			,
	LGM_EXP_CMD_REPORT_MAN_1	,	/* 22, 13, 17, 42, 44, 46, 47, 48, 49, 55, 56, 57, 65, 66, 67, 71 et 88*/
	LGM_EXP_CMD_REPORT_MAN_2	,	/* Other than 22, 13, 17, 42, 44, 46, 47, 48, 49, 55, 56, 57, 65, 66, 67, 71 et 88*/
	LGM_EXP_CMD_REPORT_MAN_RESP	,
	LGM_INIT_TRN_REQ			,
	LGM_INITIAL_REQ				,
	LGM_INITIAL_REQ_RESP		,
	LGM_MIDDLE_REQ				,
	LGM_MIDDLE_REQ_RESP			,
	LGM_WAG_PROD_1				,
	LGM_WAG_PROD_2				,
	LGM_WAG_COM					,
	LGM_WAG_EMV					,
	LGM_INQ_REQ					,
	LGM_INQ_RESP				,
	LGM_INQ_RESP_BLK			,
	LGM_ILS_REQ					,
	LGM_ILS_RESP				,
	LGM_ILS_RESP_BLK			,
	LGM_PRINTER_PG_TRNSF_REQ	,
	LGM_PRINTER_PG_TRNSF_RESP	,
	LGM_SCREEN_PG_TRNSF_REQ		,
	LGM_SCREEN_PG_TRNSF_RESP	,
	LGM_SALES_COND_REQ			,
	LGM_SALES_COND_RESP			,
	LGM_MULTI_SALES_COND_REQ	,
	LGM_MULTI_SALES_COND_RESP	,
	LGM_AUTH_REQ				,
	LGM_AUTH_RESP				,
	LGM_AUTH_REQ_F_CRNCY		,
	LGM_AUTH_RESP_F_CRNCY		,
	LGM_BAL_INQ					,
	LGM_BAL_INQ_RESP			,
	LGM_MONEO_CTX_REQ			,
	LGM_MONEO_CTX_REQ_RESP		,
	LGM_TOPUP_REQ				,
	LGM_TOPUP_RESP				,
	LGM_WAG_MONEO				,
	LGM_WITHDRAWAL_ADVICE		,
	LGM_DEGRADE					,
	LGM_FUND_TRNSF_ADVICE		,
	LGM_OTHER_OP_ADVICE			,
	LGM_WTHDRL_ADVICE_F_CRNCY	,
	LGM_DEPOSIT_ADVICE			,
	LGM_WAG_BUNDLE_DEPOSIT		,
	LGM_BUNDLE					,
	LGM_DEN						,
	LGM_TOPUP_ADVICE			,
	LGM_OP_REPORT_RESP			,
	LGM_END_OF_TRN_REQ			,

	LGM_QTY
} E_LG_LAYOUT_TYPE;



typedef enum {
	SRVMSG_PERIODIC_RESP				= 0	,
	SRVMSG_MSG_ACK_CHANNEL_2				,
	SRVMSG_EXCEPTION_MSG					,
	SRVMSG_REENTRY_RESP						,
	SRVMSG_KEYS_DL_RESP_1					,
	SRVMSG_PARAM_FILE_DL_RESP				,
	SRVMSG_TERM_SPEC_RESP					,
	SRVMSG_TERM_SPEC_PARAM_RESP				,
	SRVMSG_EXP_CMD_REPORT_RMT_RESP			,
	SRVMSG_INIT_TLP_RESP					,
	SRVMSG_TBL_TRNSF_RESP					,
	SRVMSG_END_TLP_RESP						,
	SRVMSG_KEYS_DL_RESP_2					,
	SRVMSG_EXP_CMD_REPORT_MAN_RESP			,
	SRVMSG_INITIAL_REQ_RESP					,
	SRVMSG_MIDDLE_REQ_RESP					,
	SRVMSG_WAG_PROD_1						,
	SRVMSG_WAG_PROD_2						,
	SRVMSG_WAG_COM							,
	SRVMSG_INQ_RESP							,
	SRVMSG_ILS_RESP							,
	SRVMSG_PRINTER_PG_TRNSF_RESP			,
	SRVMSG_SCREEN_PG_TRNSF_RESP				,
	SRVMSG_SALES_COND_RESP					,
	SRVMSG_MULTI_SALES_COND_RESP			,
	SRVMSG_AUTH_RESP						,
	SRVMSG_AUTH_RESP_F_CRNCY				,
	SRVMSG_BAL_INQ_RESP						,
	SRVMSG_MONEO_CTX_REQ_RESP				,
	SRVMSG_TOPUP_RESP						,
	SRVMSG_OP_REPORT_RESP					,
	SRVMSG_END_OF_TRN_REQ					,

	SRVMSG_QTY

} E_SERVER_MSG;

/*
typedef enum {
	TS_TERM_NUMBER					=0	,
	TS_PROVIDER_NAME					,
	TS_TERM_COM_NAME					,
	TS_RELEASE_VER_NO					,
	TS_OS_NAME_VER						,
	TS_MICRO_PROC_TYPE					,
	TS_MEM_SIZE							,
	TS_MODEL_CARD_READER				,
	TS_MODEL_ICC_READER					,
	TS_PERSO_SOFT_VER_NO				,
	TS_TERM_TYPE						,
	TS_MODEL_KEYBOARD					,
	TS_MODEL_SUPERVISOR_SCREE			,
	TS_MODEL_CLIENT_SCREEN				,
	TS_MODEL_CD_READER					,
	TS_MODEL_JOURNAL_PRINTER			,
	TS_MODEL_TICKET_PRINTER				,
	TS_MODEL_PRINTER					,
	TS_MODEL_DEPOSIT_MOD				,
	TS_MODEL_DISTRIBUTION_MOD			,
	TS_MODEL_SECURITY_CARD				,
	TS_MODEL_CHECK_SCANNER				,
	TS_MODEL_NOTES_AUTH_MOD				,
	TS_MODEL_DEVICES_MNG				,
	TS_MODEL_AUDIO_CARD					,
	TS_MODEL_NETWORK_CARD				,
	TS_NOTES_RECOGNITION_FILES_VER		,
	TS_PLUG_FILE_VER					,
	TS_TERM_SERIAL_NO					,
	TS_AFAS_DEVICE						,
	TS_CRYPTO_MODULE_VER_NO				,
	TS_PRIVATE							,



} E_TERM_SPEC;
*/

#define LG_MSG_HEADER_LEN		48

typedef struct {
	int					nFieldNo;
	E_LG_FIELD_TYPE		eFieldType;
	int					nFieldLength;
	char*				szLabel;
} TSLGField;

typedef struct {
	int		nFieldNo;
	int		nOffset;
	int		nSize;
} TSLGMsgFieldProp;



typedef struct {
	E_LG_LAYOUT_TYPE	eLayoutType;
	int					nNbFields;
	int					nSize;
	char				dummy;
	TSLGMsgFieldProp	tab_fields[LG_MAX_MSG_FIELDS];
} TSLGMsgLayout;

typedef struct {
	char					data[LG_MAX];
	int						nLength;
	char					header[LG_MSG_HEADER_LEN + 1];
	const TSLGMsgLayout*	layout;
} TSLGMsgInfo;


/*****Crypto *****/

typedef struct {
	char		template_code[2 + 1];
	char*		szLabel;
} TSLGCrytTmplDesc;

typedef struct {
	E_LG_TMPLT_LAYOUT		eTemplate;
	char					template_code[2 + 1];
	int						nNbFields;
	E_LG_CRPT_FIELDS		tab_fields[12];

}TSLGCryptoTemplate;

typedef struct {
	
	int				nLength;
	char			sData				[ MAX_STRUCT_DATA_LEN ];
	int				nPresent			[ MAX_STRUCT_FIELDS + 1];
	int				nPosTlv				[ MAX_STRUCT_FIELDS + 1];
	const TSLGCryptoTemplate*			pkTemplateLayout;
	const BerInfoProperties*			pkTagProperties;
} TSLGCryptoInfo;

typedef struct {

	int						nNbFields;
	int						nLength;
	char					sData				[ MAX_STRUCT_DATA_LEN ];
	int						nPresent			[ LG_TMPLT_LAY_QTY ];
	int						nPosTlv				[ LG_TMPLT_LAY_QTY ];
	TSLGCryptoInfo			tabCryptoInfo		[ LG_TMPLT_LAY_QTY ];

} TSLGCryptoMsg;
/*
typedef struct {

	char crptHashAlgo  	[ 4+1 ];
	char crptEncAlgo  	[ 4+1 ];
	char crptSigAlgo 	[ 4+1 ];
	char oaepHashAlgo 	[ 4+1 ];
	char oaepMask  		[ 4+1 ];
	char oaepPParamInd  [ 2+1 ];
	char pssHashAlgo 	[ 4+1 ];
	char pssMask   		[ 4+1 ];
	char pssPParamInd	[ 2+1 ];
	char pkcsHashAlgo  	[ 4+1 ];
	char crptP  		[ 512+1 ];
	char crptSalt 		[ 512+1 ];

} TSLGCryptoElts;
*/

typedef struct {

	char keyType  		[ 3+1 ];
	char keyIndex  		[ 1+1 ];
	char keySeqNo 		[ 3+1 ];
	char keyVersionNo 	[ 2+1 ];
	char keySerialNo  	[ 24+1 ];
	char keyValue  		[ 256+1 ];
	char keyEncValue 	[ 256+1 ];
	char keyCheckValue 	[ 6+1 ];

} TSLGKeyElts;


typedef enum {
	P_AC_NO_ACTION					= 0	,
	P_AC_REENTRY						,
	P_AC_INIT_SCD						,
	P_AC_PARAM_FILE_DL					,
	P_AC_ATM_MASTER_KEY_RENEWAL			,
	P_AC_SRV_MASTER_KEY_RENEWAL			,
	P_AC_KTK_KEY_RENEWAL				,
	P_AC_KT_KS_KEY_RENEWAL				,
	P_AC_REMOTE_ATM_CMD					,
	P_AC_KEY_LOADING					,
	P_AC_ATM_PARAM_UPD					,
	P_AC_SEND_TERM_ATTR					,
	P_AC_TLP_REQUEST					,

	LG_SRV_ACTIONS_QTY

} E_SERVER_ACTIONS;


typedef enum {
	PARAM_FILE_AUTOMA		=0	,
	PARAM_FILE_TYPES			,
	PARAM_FILE_PARG				,
	PARAM_FILE_PARC				,
	PARAM_FILE_OPER				,
	PARAM_FILE_FONC				,
	PARAM_FILE_LIBEL			,
	PARAM_FILE_ECRAN			,
	PARAM_FILE_SEGMENTS			,
	PARAM_FILE_ETAT1			,
	PARAM_FILE_TICKET			,
	PARAM_FILE_PRODUITS			,
	PARAM_FILE_DEVISES			,
	PARAM_FILE_LASER			,

	PARAM_FILE_QTY
} E_LG_PARAM_FILE;		

typedef enum {
	LG_TLP_TBL_BIN			= 0	,
	LG_TLP_TBL_AID				,
	LG_TLP_TBL_TAC_DENIAL		,
	LG_TLP_TBL_RSA_KEYS			,

	LG_TLP_TBL_QTY
} E_LG_TLP_TABLES;


typedef enum {
	LG_LEVEL_HEADER				=0,
	LG_LEVEL_MSG					,
	LG_DUMP_LEVEL_QTY
} E_LG_DUMP_LEVEL;



typedef	struct SContextT {
    int			fRespCode;
    int			fTransCode;
    int			fAuthoSrc;
    char  		fTlvBuffer [LG_MAX];
    char		fPINBlk[16 + 1];
    char  		fPrdWag [LG_MAX];
    TSIsoInfo	fIsoBuffer;
} SContext;


typedef struct {

	char				szBufferLine	[LG_MAX];
	char				szBufferBal		[LG_MAX];
/*	char  				sTlvBuffer		[LG_MAX];*/
	char				adress			[  17  ];
	char				terminal		[  16  ];
	char				sExternalId		[  16  ];
	int					free;
	int					status;
	int					fd;
	int					line_id;
	int					flag_bal;
	int					flag_commande;
	int					nLenLine;
	int					nLenBal;
	pthread_t			tid;  
	pthread_t			tid_spec;
	pthread_mutex_t		ThrMutex;
	pthread_cond_t		ThrCond ;
	pthread_mutex_t		BalMutex  ;
	SContext			Context;

	int					nGmtTimeOffst;
	char				AcquirerBank[6 + 1];
	char				AcquirerId[24];
	char				atm_profile[3 + 1];
	char                gSourceAccount		[  25  ];
	int					pending_actions[LG_SRV_ACTIONS_QTY];

	/* Parameters files */
	char				sParamFilesVer[12];
	char				sParamFilesPath[256];
	int					nCurrParamFileId;
	int					nCurrParamFileFd;
	int					nCurrParamFileSize;
	int					nCUrrParamFileOfst;
	int					nLastBlkSent;

	/* TLP */
	char*				pTLPTablesData[LG_TLP_TBL_QTY];
	int					nTLPTablesSizes[LG_TLP_TBL_QTY];
	char				sTLPBINTblVerNo[8];
	char				sTLPAIDTblVerNo[8];
	char				sTLPTACTblVerNo[8];
	char				sTLPRSATblVerNo[8];
	/*int					nTLPOffset;*/
	int					nTLPBlkSize;
	int					nIsFistBlk;
	/*int					nLastTLPBlkSent;*/
	/*TSLGCryptoElts	 	cryptoElts;*/
	TSLGKeyElts	 			keyElts	 ;
} TSLGStruct;


typedef struct {
    char		sAcctNbr[ 25];
    char		sAcctType[  3];
    char		sAcctCurrency[  4];
    char		sBalance[  13];
    char		sDC_CRCode[  2];
    char		sAcctAuthorisedOp[  100];

} TSLGAccounts;


#define THR_IDX_FROM_CTX(a)		(a) - RESERVED_THREAD


extern TSLGStruct			tab_ATMThr [ ];
extern TSLGField			tab_LGFields[];
extern TSLGCrytTmplDesc 	tab_templatesDesc[];
extern int					g_nFd_1;
extern int					g_nFd_2;




void InitLGMsgInfo(TSLGMsgInfo* msgInfo, E_LG_LAYOUT_TYPE eMsgType);
/*void AnalyseLGMsgInfo(char* data, TSLGMsgInfo* msgInfo);*/
int GetLGMsgField(int field_no, TSLGMsgInfo * msgInfo, char *data, int *nLength);
int AddLGMsgField(int field_no, TSLGMsgInfo *msgInfo, char *data, int nLength);
/*int GetLGMacData(TSLGMsgInfo * msgInfo, char *data, int *nLength);*/
int BuildLGMsg (char * data, TSLGMsgInfo *msgInfo);
void DumpLGMsgStruct( TSLGMsgInfo *msgInfo ,E_LG_DUMP_LEVEL DumpLevel);


int AnalyseLGMsgHeader(char* data, TSLGMsgInfo* msgInfo);
int AnalyseLGTermMessage(char* data,int nDataLen, TSLGMsgInfo* msgInfo);
int InitServerLGMessage(E_SERVER_MSG eSrvMsgTp, TSLGMsgInfo* msgInfo);


int  GetLGCryptoTemplateId(char* template_code);
int  InitLGCryptoMsg(TSLGCryptoMsg* pkLGCryptoMsg);
int  InitLGCryptoInfo(TSLGCryptoInfo* pkLGCryptoInfo,E_LG_TMPLT_LAYOUT eTemplate);
int  AdjustLGCryptoMsg(TSLGCryptoMsg* 	cryptoMsgIn,TSLGCryptoMsg* 	cryptoMsgOut);
int  AnalyseLGCryptoInfo(const char *buffer_in, TSLGCryptoInfo* pkLGCryptoInfo);
int  AnalyseLGCryptoMsg(const char *buffer_in, int nLength, TSLGCryptoMsg* pkLGCryptoMsg);
int  GetLGCryptoInfoField (E_LG_CRPT_FIELDS nFieldId, const TSLGCryptoInfo* pkLGCryptoInfo, char *data, int* nLength);
int  AddLGCryptoInfoField (E_LG_CRPT_FIELDS nFieldId, TSLGCryptoInfo* pkLGCryptoInfo, const char *data, int nLength);
int  PutLGCryptoInfoField (E_LG_CRPT_FIELDS nFieldId, TSLGCryptoInfo* pkLGCryptoInfo, const char *data, int nLength);
int  BuildLGCryptoInfoBuffer (char * buffer_out, TSLGCryptoInfo* pkLGCryptoInfo);
void DumpLGCryptoMsg(const TSLGCryptoMsg* pkLGCryptoMsg);
void DumpLGCryptoInfo(const TSLGCryptoInfo* pkLGCryptoInfo);



int CheckAndSetPendingAction(int nIndexCtx,char* pTermNumber,char* pBankCode, TSLGMsgInfo* msgInfo );






/***************TMP****************/



typedef enum {
	LGSIM_EMV_AUT_REQ = 0,
	LGSIM_EMV_WITHDRAWAL_REQ,


} LGSIM_MSG;





#endif


