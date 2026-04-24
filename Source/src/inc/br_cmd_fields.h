#ifndef P7_BRIDGE_FIELDS__H
#define P7_BRIDGE_FIELDS__H

#define BRCMD_INFO_TAG_MAX_LEN		6
#define  MAX_BRCMD_TLV				999
#define  MAX_BRCMD_TLV_LEN			4098


typedef struct 
{
   char		sTlvName[BRCMD_INFO_TAG_MAX_LEN + 1];
   int		nType;
   int		nLength;
   int		nPrint;
} br_tlv_info_t;

typedef struct
{
   int  nPresent  [ MAX_BRCMD_TLV ];
   int  nPosTlv   [ MAX_BRCMD_TLV ];
   int  nLength;
   char sTlvData  [ MAX_BRCMD_TLV_LEN ];
} TSTlvBrCmd;


#define   BRCMD_TLV_FIX           0
#define   BRCMD_TLV_VAR           1

#define BRCMD_COMMAND						"001"
#define BRCMD_PAN 							"002" 
#define BRCMD_PROCESSING_CODE  				"003"
#define BRCMD_AMOUNT_TRANSACTION        	"004" 
#define BRCMD_AMOUNT_SETTLEMENT         	"005" 
#define BRCMD_AMOUNT_CARD_BILLING       	"006" 
#define BRCMD_AMOUNT_CARD_BILL_FEE      	"008" 
#define BRCMD_CONV_RATE_SETTLEMENT      	"009"
#define BRCMD_CONV_RATE_CARD_BILL       	"010"
#define BRCMD_INTERNAL_STAN              	"011" 
#define BRCMD_DATE_TIME_LOCAL_TRAN      	"012" 
#define BRCMD_EFFECTIVE_DATE            	"013"
#define BRCMD_EXPIRY_DATE               	"014" 
#define BRCMD_SETTLEMENT_DATE           	"015"
#define BRCMD_CONVERSION_DATE           	"016"
#define BRCMD_CAPTURE_DATE              	"017"
#define BRCMD_MERCHANT_TYPE             	"018"
#define BRCMD_ACQU_INST_COUNTRY         	"019" 
#define BRCMD_POS_DATA                  	"022"
#define BRCMD_CARD_SEQUENCE_NUMBER      	"023"
#define BRCMD_FUNCTION_CODE             	"024"
#define BRCMD_MESSAGE_REASON_CODE       	"025"
#define BRCMD_RRN                       	"037"
#define BRCMD_AUTH_NUMBER               	"038"
#define BRCMD_RESPONSE_CODE             	"039"
#define BRCMD_ADDITIONAL_RESP_DATA      	"044"
#define BRCMD_AMOUNT_FEE					"046"
#define BRCMD_ADD_DATA_PRIVATE   			"048"
#define BRCMD_TRAN_CURRENCY_CODE    		"049"
#define BRCMD_SETL_CURRENCY_CODE    		"050"
#define BRCMD_BILL_CURRENCY_CODE    		"051"
#define BRCMD_PIN_DATA						"052"
#define BRCMD_SECURITY_DATA					"053"
#define BRCMD_ADD_AMOUNTS					"054"
#define BRCMD_ORIGINAL_DATA					"056"
#define BRCMD_MESSAGE_NUMBER				"071" 
#define BRCMD_UF_FILE_RECORD 				"072"
#define BRCMD_UF_ACTION_DATE 				"073"
#define BRCMD_UF_FILENAME 					"101"
#define BRCMD_ACCOUNT_ID_1  				"102"
#define BRCMD_ACCOUNT_ID_2  				"103"

#define BRCMD_REQUESTOR_ID					"995"
#define BRCMD_REQ_KEY						"996"
#define	BRCMD_EVENT_CODE					"997"
#define BRCMD_MESSAGE						"998"
#define BRCMD_RESULT						"999"


#define BRCMD_RES_LEN						3
#define BRCMD_RES_OK						"000"
#define BRCMD_RES_MISS_ARG					"001"
#define BRCMD_RES_ERROR						"999"


void InitTlvBrCmd(TSTlvBrCmd * tlvInfo);
int  AnalyseTlvBrCmd(char *buffer,TSTlvBrCmd *tlvInfo);
int  GetTlvBrCmd (char *tlv_name, TSTlvBrCmd *tlvInfo,char *data, int *length);
int  AddTlvBrCmd (char *tlv_name, TSTlvBrCmd *tlvInfo, char *data, int length);
int  PutTlvBrCmd (char *tlv_name, TSTlvBrCmd *tlvInfo, char *data, int length);
int  BrCmdBuildTlv (char * buffer_snd, TSTlvBrCmd *tlvInfo);
void DumpTlvBrCMd(TSTlvBrCmd *tlvInfo);


#endif


