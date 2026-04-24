#ifndef HPS_RKL_FIELDS_H
#define HPS_RKL_FIELDS_H

#define RKL_BIT_MAP2              		0
#define RKL_CARD_NBR              		1
#define RKL_PROC_CODE             		2
#define RKL_TRANS_AMOUNT          		3
#define RKL_STLMNT_AMOUNT		  		4
#define RKL_BILLING_AMOUNT		  		5 
#define RKL_XMIT_TIME             		6
#define RKL_BILLING_AMOUNT_FEES         7
#define RKL_STLMNT_CNVRSN_RATE          8
#define RKL_BILLING_CNVRSN_RATE		   	9  

#define RKL_AUDIT_NBR            		10
#define RKL_TRANS_DATE_TIME           	11
#define RKL_START_EXPIRY_DATE          	12
#define RKL_EXPIRY_DATE          		13
#define RKL_STLMNT_DATE          		14
#define RKL_CNVRSN_DATE          		15
#define RKL_CAPTURE_DATE                16
#define RKL_MERCHANT_TYPE        		17
#define RKL_ACQR_COUNTRY         		18
#define RKL_PAN_COUNTRY          		19

#define RKL_FORWD_COUNTRY        		20
#define RKL_POS_DATA					21
#define RKL_CARD_SEQUENCE_NBR			22
#define RKL_FUNCTION_CODE        		23
#define RKL_MSG_REASON     				24
#define RKL_ACCEPTOR_ACTIV_CODE    		25
#define RKL_AUTHOR_ID_LENGTH     		26
#define RKL_RECONC_DATE					27
#define RKL_RECONC_INDICATOR			28
#define RKL_ORIGINAL_AMOUNTS			29

#define RKL_ACQR_REFERENCE_DATA			30
#define RKL_ACQR_ID	 	         		31
#define RKL_FORWD_ID					32
#define RKL_PAN_EXTENDED		 		33
#define RKL_T2_DATA              		34
#define RKL_T3_DATA              		35
#define RKL_REFERENCE_NBR        		36
#define RKL_AUTHOR_ID			     	37
#define RKL_RESPONSE_CODE        		38
#define RKL_SERVICE_CODE			39

#define RKL_TERMINAL_NBR         		40
#define RKL_OUTLET_NBR				41
#define RKL_TERMINAL_ADR		 	42
#define RKL_ADTNL_RESP_DATA      		43
#define RKL_T1_DATA              		44
#define RKL_FEES_AMOUNT				45
#define RKL_ADTNL_DATA_NATL			46
#define RKL_ADTNL_DATA_PRIV      		47
#define RKL_TRANS_CRNCY          		48
#define RKL_STLMNT_CRNCY         		49

#define RKL_BILLING_CRNCY        		50
#define RKL_PIN_DATA					51
#define RKL_SECURITY_DATA        		52
#define RKL_ADTNL_AMOUNTS				53
#define RKL_ICC_DATA					54
#define RKL_ORIG_DATA					55
#define RKL_AUTHORIZATION_CYCLE_CODE    56
#define RKL_AUTHOR_INSTITN_ID_CODE		57
#define RKL_GEOGR_DATA           		58
#define RKL_ADTNL_POS_INFO       		59

#define RKL_NET_POS_DATA		 		60
#define RKL_CPS_FIELDS           		61
#define RKL_NETWORK_DATA   		 		62
#define RKL_MAC1                 		63
#define RKL_STLMNT_CODE					65
#define RKL_RECEIVER_COUNTRY     		67
#define RKL_STLMNT_INST_COUNTRY		 	68
#define RKL_NW_MNGMT_INFO	     		69
#define RKL_MESSAGE_NBR 				70

#define RKL_RECORD_DATA			     	71
#define RKL_ACTION_DATE			     	72
#define RKL_CR_NBR						73
#define RKL_CR_REV_NBR					74
#define RKL_DB_NBR						75
#define RKL_DB_REV_NBR					76
#define RKL_TRANSFER_NBR                77
#define RKL_TRANSFER_REV_NBR            78
#define RKL_INQ_NBR             79
#define RKL_AUT_NBR                  80
#define RKL_INF_REV_NBR         		81
#define RKL_PAYMENT_NBR                 82
#define RKL_PAYMENT_REV_NBR             83
#define RKL_FEES_COLLCT_NBR             84


#define RKL_CR_AMOUNT					85
#define RKL_CR_REV_AMOUNT		 		86
#define RKL_DB_AMOUNT					87
#define RKL_DB_REV_AMOUNT		 		88
#define RKL_ADJUST_NBR             		89

#define RKL_TRX_RECEIVER_COUNTRY_CODE	90
#define RKL_TRX_ORIGINATOR_COUNTRY_CODE	91
#define RKL_TRX_RECEIVER_ID         	92
#define RKL_TRX_ORIGINATOR_ID			93
#define RKL_CARD_ISSUER_REF_DATA 		94
#define RKL_KEY_MNG_DATA		 		95
#define RKL_NET_AMOUNT					96
#define RKL_STLMNT_INSTITN		 		98
#define RKL_RECEIVER_ID					99 

#define RKL_FILE_NAME					100
#define RKL_ACCOUNT_ID1         		101
#define RKL_ACCOUNT_ID2         		102

#define RKL_TRANS_DESC                  103
#define RKL_CR_REJ_AMOUNT               104
#define RKL_DB_REJ_AMOUNT               105
#define RKL_CR_REJ_NUMBER               106
#define RKL_DB_REJ_NUMBER               107
#define RKL_CR_FEES_AMOUNT              108
#define RKL_DB_FEES_AMOUNT              109
#define RKL_RESERVED_RKL_2              110
#define RKL_RESERVED_RKL_3              111
#define RKL_RESERVED_RKL_4              112
#define RKL_RESERVED_RKL_5              113

#define RKL_TRANS_FEE              115 
#define RKL_STLMNT_FEE              116
#define RKL_PROCES_FEE              117

#define RKL_ADTNL_TRACE_DATA    		114
#define RKL_ORIG_MSG_TYPE               119
#define RKL_RESERV_NATIONAL_USAGE_6     120
#define RKL_RESERV_PRIV_USAGE_1         121
#define RKL_ADR_VERIF_DATA      		122
#define RKL_RESERV_PRIV_USAGE_3         123
#define RKL_SUPPORT_INFO				124
#define RKL_RESERVED_PRIVE_USAGE_5      125
#define RKL_FILE_SEC_CODE               126
#define RKL_MAC2                		127

#endif /** HPS_RKL_FIELDS_H **/
