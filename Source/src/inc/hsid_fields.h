#ifndef HPS_HSID_FIELDS_H
#define HPS_HSID_FIELDS_H

#define HSID_BIT_MAP2              		0
#define HSID_CARD_NBR              		1
#define HSID_PROC_CODE             		2
#define HSID_TRANS_AMOUNT          		3
#define HSID_STLMNT_AMOUNT		  		4
#define HSID_BILLING_AMOUNT		  		5
#define HSID_XMIT_TIME             		6
#define HSID_BILLING_AMOUNT_FEES        7
#define HSID_STLMNT_CNVRSN_RATE         8
#define HSID_BILLING_CNVRSN_RATE		9

#define HSID_AUDIT_NBR            		10
#define HSID_TRANS_DATE_TIME           	11
#define HSID_START_EXPIRY_DATE          12
#define HSID_EXPIRY_DATE          		13
#define HSID_STLMNT_DATE          		14
#define HSID_CNVRSN_DATE          		15
#define HSID_CAPTURE_DATE               16
#define HSID_MERCHANT_TYPE        		17
#define HSID_ACQR_COUNTRY         		18
#define HSID_PAN_COUNTRY          		19

#define HSID_FORWD_COUNTRY        		20
#define HSID_POS_DATA					21
#define HSID_CARD_SEQUENCE_NBR			22
#define HSID_FUNCTION_CODE        		23
#define HSID_MSG_REASON     			24
#define HSID_ACCEPTOR_ACTIV_CODE    	25
#define HSID_AUTHOR_ID_LENGTH     		26
#define HSID_RECONC_DATE				27
#define HSID_RECONC_INDICATOR			28
#define HSID_ORIGINAL_AMOUNTS			29

#define HSID_ACQR_REFERENCE_DATA		30
#define HSID_ACQR_ID	 	         	31
#define HSID_FORWD_ID					32
#define HSID_PAN_EXTENDED		 		33
#define HSID_T2_DATA              		34
#define HSID_T3_DATA              		35
#define HSID_REFERENCE_NBR        		36
#define HSID_AUTHOR_ID			     	37
#define HSID_RESPONSE_CODE        		38
#define HSID_SERVICE_CODE				39

#define HSID_TERMINAL_NBR         		40
#define HSID_OUTLET_NBR					41
#define HSID_TERMINAL_ADR		 		42
#define HSID_ADTNL_RESP_DATA      		43
#define HSID_T1_DATA              		44
#define HSID_FEES_AMOUNT				45
#define HSID_ADTNL_DATA_NATL			46
#define HSID_ADTNL_DATA_PRIV      		47
#define HSID_TRANS_CRNCY          		48
#define HSID_STLMNT_CRNCY         		49

#define HSID_BILLING_CRNCY        		50
#define HSID_PIN_DATA					51
#define HSID_SECURITY_DATA        		52
#define HSID_ADTNL_AMOUNTS				53
#define HSID_ICC_DATA					54
#define HSID_ORIG_DATA					55
#define HSID_AUTHORIZATION_CYCLE_CODE   56
#define HSID_AUTHOR_INSTITN_ID_CODE		57
#define HSID_GEOGR_DATA           		58
#define HSID_ADTNL_POS_INFO       		59

#define HSID_NET_POS_DATA		 		60
#define HSID_CPS_FIELDS           		61
#define HSID_NETWORK_DATA   		 	62
#define HSID_MAC1                 		63
#define HSID_STLMNT_CODE				65
#define HSID_RECEIVER_COUNTRY     		67
#define HSID_STLMNT_INST_COUNTRY		68
#define HSID_NW_MNGMT_INFO	     		69
#define HSID_MESSAGE_NBR 				70

#define HSID_RECORD_DATA			    71
#define HSID_ACTION_DATE			    72
#define HSID_CR_NBR						73
#define HSID_CR_REV_NBR					74
#define HSID_DB_NBR						75
#define HSID_DB_REV_NBR					76
#define HSID_TRANSFER_NBR               77
#define HSID_TRANSFER_REV_NBR           78
#define HSID_INQ_NBR             		79
#define HSID_AUT_NBR                  	80
#define HSID_INF_REV_NBR         		81
#define HSID_PAYMENT_NBR                82
#define HSID_PAYMENT_REV_NBR            83
#define HSID_FEES_COLLCT_NBR            84


#define HSID_CR_AMOUNT					85
#define HSID_CR_REV_AMOUNT		 		86
#define HSID_DB_AMOUNT					87
#define HSID_DB_REV_AMOUNT		 		88
#define HSID_ADJUST_NBR             	89

#define HSID_TRX_RECEIVER_COUNTRY_CODE	90
#define HSID_TRX_ORIGINATOR_COUNTRY_CODE 91
#define HSID_TRX_RECEIVER_ID         	92
#define HSID_TRX_ORIGINATOR_ID			93
#define HSID_CARD_ISSUER_REF_DATA 		94
#define HSID_KEY_MNG_DATA		 		95
#define HSID_NET_AMOUNT					96
#define HSID_STLMNT_INSTITN		 		98
#define HSID_RECEIVER_ID				99

#define HSID_FILE_NAME					100
#define HSID_ACCOUNT_ID1         		101
#define HSID_ACCOUNT_ID2         		102

#define HSID_TRANS_DESC                 103
#define HSID_CR_REJ_AMOUNT              104
#define HSID_DB_REJ_AMOUNT              105
#define HSID_CR_REJ_NUMBER              106
#define HSID_DB_REJ_NUMBER              107
#define HSID_CR_FEES_AMOUNT             108
#define HSID_DB_FEES_AMOUNT             109
#define HSID_RESERVED_HSID_2            110
#define HSID_RESERVED_HSID_3            111
#define HSID_RESERVED_HSID_4            112
#define HSID_RESERVED_HSID_5            113

#define HSID_TRANS_FEE              	115
#define HSID_STLMNT_FEE              	116
#define HSID_PROCES_FEE              	117

#define HSID_ADTNL_TRACE_DATA    		114
#define HSID_ORIG_MSG_TYPE              119
#define HSID_RESERV_NATIONAL_USAGE_6    120
#define HSID_RESERV_PRIV_USAGE_1        121
#define HSID_ADR_VERIF_DATA      		122
#define HSID_RESERV_PRIV_USAGE_3        123
#define HSID_SUPPORT_INFO				124
#define HSID_RESERVED_PRIVE_USAGE_5     125
#define HSID_FILE_SEC_CODE              126
#define HSID_MAC2                		127

#endif /** HPS_HSID_FIELDS_H **/
