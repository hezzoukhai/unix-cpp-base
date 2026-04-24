#ifndef ISO_8583__STD__FIELDS__H
#define ISO_8583__STD__FIELDS__H


#define STD_ISO_BIT_MAP2              		0
#define STD_ISO_CARD_NBR              		1
#define STD_ISO_PROC_CODE             		2
#define STD_ISO_TRANS_AMOUNT          		3
#define STD_ISO_STLMNT_AMOUNT		  		4
#define STD_ISO_BILLING_AMOUNT		  		5 
#define STD_ISO_XMIT_TIME             		6
#define STD_ISO_BILLING_AMOUNT_FEES         7
#define STD_ISO_STLMNT_CNVRSN_RATE          8
#define STD_ISO_BILLING_CNVRSN_RATE		   	9  

#define STD_ISO_AUDIT_NBR            		10
#define STD_ISO_TRANS_DATE_TIME           	11
#define STD_ISO_START_EXPIRY_DATE          	12
#define STD_ISO_EXPIRY_DATE          		13
#define STD_ISO_STLMNT_DATE          		14
#define STD_ISO_CNVRSN_DATE          		15
#define STD_ISO_CAPTURE_DATE                16
#define STD_ISO_MERCHANT_TYPE        		17
#define STD_ISO_ACQR_COUNTRY         		18
#define STD_ISO_PAN_COUNTRY          		19

#define STD_ISO_FORWD_COUNTRY        		20
#define STD_ISO_POS_DATA					21
#define STD_ISO_CARD_SEQUENCE_NBR			22
#define STD_ISO_FUNCTION_CODE        		23
#define STD_ISO_MSG_REASON     				24
#define STD_ISO_ACCEPTOR_ACTIV_CODE    		25
#define STD_ISO_AUTHOR_ID_LENGTH     		26
#define STD_ISO_RECONC_DATE					27
#define STD_ISO_RECONC_INDICATOR			28
#define STD_ISO_ORIGINAL_AMOUNTS			29

#define STD_ISO_ACQR_REFERENCE_DATA			30
#define STD_ISO_ACQR_ID	 	         		31
#define STD_ISO_FORWD_ID					32
#define STD_ISO_PAN_EXTENDED		 		33
#define STD_ISO_T2_DATA              		34
#define STD_ISO_T3_DATA              		35
#define STD_ISO_REFERENCE_NBR        		36
#define STD_ISO_AUTHOR_ID			     	37
#define STD_ISO_RESPONSE_CODE        		38
#define STD_ISO_SERVICE_CODE				39

#define STD_ISO_TERMINAL_NBR         		40
#define STD_ISO_OUTLET_NBR					41
#define STD_ISO_TERMINAL_ADR		 		42
#define STD_ISO_ADTNL_RESP_DATA      		43
#define STD_ISO_T1_DATA              		44
#define STD_ISO_FEES_AMOUNT					45
#define STD_ISO_ADTNL_DATA_NATL				46
#define STD_ISO_ADTNL_DATA_PRIV      		47
#define STD_ISO_TRANS_CRNCY          		48
#define STD_ISO_STLMNT_CRNCY         		49

#define STD_ISO_BILLING_CRNCY        		50
#define STD_ISO_PIN_DATA					51
#define STD_ISO_SECURITY_DATA        		52
#define STD_ISO_ADTNL_AMOUNTS				53
#define STD_ISO_ICC_DATA					54
#define STD_ISO_ORIG_DATA					55
#define STD_ISO_AUTHORIZATION_CYCLE_CODE    56
#define STD_ISO_AUTHOR_INSTITN_ID_CODE		57
#define STD_ISO_GEOGR_DATA           		58
#define STD_ISO_ADTNL_POS_INFO       		59

#define STD_ISO_NET_POS_DATA		 		60
#define STD_ISO_CPS_FIELDS           		61
#define STD_ISO_NETWORK_DATA   		 		62
#define STD_ISO_MAC1                 		63
#define STD_ISO_BIT_MAP_2                   64
#define STD_ISO_STLMNT_CODE					65
#define STD_ISO_EXT_PAYMENT_CODE            66
#define STD_ISO_RECEIVER_COUNTRY     		67
#define STD_ISO_STLMNT_INST_COUNTRY		 	68
#define STD_ISO_NW_MNGMT_INFO	     		69

#define STD_ISO_MESSAGE_NBR 				70
#define STD_ISO_MESSAGE_NBR_LAST            71
#define STD_ISO_ACTION_DATE                 72
#define STD_ISO_CR_NBR                      73
#define STD_ISO_CR_REV_NBR                  74
#define STD_ISO_DB_NBR                      75
#define STD_ISO_DB_REV_NBR                  76
#define STD_ISO_TRANSFER_NBR                77
#define STD_ISO_TRANSFER_REV_NBR            78
#define STD_ISO_INQ_NBR                     79

#define STD_ISO_AUT_NBR                     80
#define STD_ISO_CR_PROC_FEE_AMNT            81
#define STD_ISO_CR_TANS_FEE_AMNT            82
#define STD_ISO_DB_PROC_FEE_AMNT            83
#define STD_ISO_DB_TRANS_FEE_AMNT           84
#define STD_ISO_CR_AMOUNT	                85
#define STD_ISO_CR_REV_AMOUNT	            86
#define STD_ISO_DB_AMOUNT	                87
#define STD_ISO_DB_REV_AMOUNT	            88

#define STD_ISO_FILE_UPDATE		            90
#define STD_ISO_FILE_SECURITY		        91
#define STD_ISO_RESP_INDICATOR              92
#define STD_ISO_REPLACE_AMOUNTS		        94
#define STD_ISO_SERVICE_IND                 93
#define STD_ISO_NET_AMOUNT	                96
#define STD_ISO_PAYEE                       97
#define STD_ISO_STLMNT_INSTITN	            98

#define STD_ISO_FILE_NAME	                100
#define STD_ISO_ACCOUNT_ID2                 102
#define STD_ISO_TRANS_DESC                  103
#define STD_ISO_RESERVED_ISO_1              104
#define STD_ISO_RESERVED_ISO_2              105
#define STD_ISO_RESERVED_ISO_3              106
#define STD_ISO_RESERVED_ISO_4              107
#define STD_ISO_RESERVED_ISO_5              108
#define STD_ISO_RESERVED_ISO_6              109

#define STD_ISO_RESERVED_ISO_7              110
#define STD_ISO_RESERVED_NAT_3              111
#define STD_ISO_RESERVED_NAT_4              112
#define STD_ISO_RESERVED_NAT_5              113
#define STD_ISO_ADTNL_TRACE_DATA            114
#define STD_ISO_CARD_ISSUER_REF_DATA        115
#define STD_ISO_NATIONAL_USE                116
#define STD_ISO_INTRA_COUNTRY_DATA          117
#define STD_ISO_RES_NAT_USE_10              118
#define STD_ISO_ORIG_MSG_TYPE               119

#define STD_ISO_ISS_INST_ID_CODE            120
#define STD_ISO_ADTNL_RECORD_DATA           121
#define STD_ISO_VERIFICATION_DATA           122
#define STD_ISO_MEMBER_DEF_DATA             123
/*PLUTONL-1822*/ 

#define STD_ISO_RESERVED_PRIV_USAGE_4		125
#define STD_ISO_FILE_SEC_CODE		        126

#define		STD_EMV_TAG_TRANS_AMOUNT        "9F02"
#define		STD_EMV_TAG_OTHER_AMOUNT        "9F03"
#define		STD_EMV_TAG_TERM_COUNTRY_CODE   "9F1A"
#define		STD_EMV_TAG_TVR                 "95"
#define		STD_EMV_TAG_TRANS_CUR_CODE      "5F2A"
#define		STD_EMV_TAG_TRANS_DATE          "9A"
#define		STD_EMV_TAG_TRANS_TYPE          "9C"
#define		STD_EMV_TAG_UNPRED_NUMBER       "9F37"
#define		STD_EMV_TAG_PAN_SEQ_NBR         "5F34"
#define		STD_EMV_TAG_APP_INTER_PROFILE   "82"
#define		STD_EMV_TAG_ISS_APP_DATA        "9F10"
#define		STD_EMV_TAG_ATC                 "9F36"
#define		STD_EMV_TAG_TRACK2				"57"
#define		STD_EMV_TAG_APP_CRYPTOGRAM		"9F26"
#define		STD_EMV_TAG_CRYPTO_INFO_DATA    "9F27"

#define    STD_EMV_TAG_IFD_SERIAL_NUM      "9F1E"
#define    STD_EMV_TAG_TERM_CAP            "9F33"
#define    STD_EMV_TAG_CVM                 "9F34"
#define    STD_EMV_TAG_TERM_TYPE           "9F35"
#define    STD_EMV_TAG_TERM_APP_VER_NUM    "9F09"
#define    STD_EMV_TAG_DED_FILE_NAME       "8400"
#define    STD_EMV_TAG_AUTHOR_RESP_CODE    "8A00"
#define    STD_EMV_TAG_ARPC                "9F54"
#define    STD_EMV_TAG_ISS_AUTHEN_DATA     "9100"
#define    STD_EMV_TAG_ISS_SCRIPT1         "7100"
#define    STD_EMV_TAG_ISS_SCRIPT2         "7200"
#define    STD_EMV_TAG_ISS_SCRIPT_RESULT   "9600"
/* ISO Message  type identifiers */
#define MSG_TYPE_VERSION				0
#define MSG_TYPE_CLASS					1
#define MSG_TYPE_FUNCTION				2
#define MSG_TYPE_TRX_ORIGIN				3


#define MSG_TYPE_P2_AUT					'1'			/* authorization				*/
#define MSG_TYPE_P2_FIN					'2'			/* financial					*/
#define MSG_TYPE_P2_FILE				'3'			/* file action					*/
#define MSG_TYPE_P2_REV					'4'			/* reversal/chargeback			*/
#define MSG_TYPE_P2_REC					'5'			/* reconciliation				*/
#define MSG_TYPE_P2_ADM					'6'			/* administrative				*/
#define MSG_TYPE_P2_FEE					'7'			/* fee collection				*/
#define MSG_TYPE_P2_NET					'8'			/* network management			*/


#define MSG_TYPE_P3_REQ					'0'
#define MSG_TYPE_P3_REQ_RESP			'1'
#define MSG_TYPE_P3_ADV					'2'
#define MSG_TYPE_P3_ADV_RESP			'3'
#define MSG_TYPE_P3_NOTIF				'4'

#define MSG_TYPE_P4_ACQ					'0'
#define MSG_TYPE_P4_ACQ_REP				'1'
#define MSG_TYPE_P4_ISS					'2'
#define MSG_TYPE_P4_ISS_REP				'3'
#define MSG_TYPE_P4_OTHER				'4'
#define MSG_TYPE_P4_OTHER_REP			'5'
/**********************************************************/


#endif


