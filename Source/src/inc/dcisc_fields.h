#ifndef HPS_DCISC_FIELDS_H
#define HPS_DCISC_FIELDS_H

#define DCISC_BIT_MAP2              		0
#define DCISC_CARD_NBR              		1
#define DCISC_PROC_CODE             		2
#define DCISC_TRANS_AMOUNT          		3
#define DCISC_STLMNT_AMOUNT		  		4
#define DCISC_BILLING_AMOUNT		  		5 
#define DCISC_XMIT_TIME             		6
#define DCISC_BILLING_AMOUNT_FEES         7
#define DCISC_STLMNT_CNVRSN_RATE          8
#define DCISC_BILLING_CNVRSN_RATE		   	9  

#define DCISC_AUDIT_NBR            		10
#define DCISC_TRANS_DATE_TIME           	11
#define DCISC_START_EXPIRY_DATE          	12
#define DCISC_EXPIRY_DATE          		13
#define DCISC_STLMNT_DATE          		14
#define DCISC_CNVRSN_DATE          		15
#define DCISC_CAPTURE_DATE                16
#define DCISC_MERCHANT_TYPE        		17
#define DCISC_ACQR_COUNTRY         		18
#define DCISC_PAN_COUNTRY          		19

#define DCISC_FORWD_COUNTRY        		20
#define DCISC_POS_DATA					21
#define DCISC_CARD_SEQUENCE_NBR			22
#define DCISC_FUNCTION_CODE        		23
#define DCISC_MSG_REASON     				24
#define DCISC_ACCEPTOR_ACTIV_CODE    		25
#define DCISC_AUTHOR_ID_LENGTH     		26
#define DCISC_RECONC_DATE					27
#define DCISC_RECONC_INDICATOR			28
#define DCISC_ORIGINAL_AMOUNTS			29

#define DCISC_ACQR_REFERENCE_DATA			30
#define DCISC_ACQR_ID	 	         		31
#define DCISC_FORWD_ID					32
#define DCISC_PAN_EXTENDED		 		33
#define DCISC_T2_DATA              		34
#define DCISC_T3_DATA              		35
#define DCISC_REFERENCE_NBR        		36
#define DCISC_AUTHOR_ID			     	37
#define DCISC_RESPONSE_CODE        		38
#define DCISC_SERVICE_CODE			39

#define DCISC_TERMINAL_NBR         		40
#define DCISC_OUTLET_NBR				41
#define DCISC_TERMINAL_ADR		 	42
#define DCISC_ADTNL_RESP_DATA      		43
#define DCISC_T1_DATA              		44
#define DCISC_FEES_AMOUNT				45
#define DCISC_ADTNL_DATA_NATL			46
#define DCISC_ADTNL_DATA_PRIV      		47
#define DCISC_TRANS_CRNCY          		48
#define DCISC_STLMNT_CRNCY         		49

#define DCISC_BILLING_CRNCY        		50
#define DCISC_PIN_DATA					51
#define DCISC_SECURITY_DATA        		52
#define DCISC_ADTNL_AMOUNTS				53
#define DCISC_ICC_DATA					54
#define DCISC_ORIG_DATA					55
#define DCISC_AUTHORIZATION_CYCLE_CODE    56
#define DCISC_AUTHOR_INSTITN_ID_CODE		57
#define DCISC_GEOGR_DATA           		58
#define DCISC_ADTNL_POS_INFO       		59

#define DCISC_NET_POS_DATA		 		60
#define DCISC_ADD_AUTH_DATA           		61 /* ELKAM20230217 PLUTONL-5914 */
#define DCISC_NETWORK_DATA   		 		62
#define DCISC_MAC1                 		63
#define DCISC_STLMNT_CODE					65
#define DCISC_RECEIVER_COUNTRY     		67
#define DCISC_STLMNT_INST_COUNTRY		 	68
#define DCISC_NW_MNGMT_INFO	     		69
#define DCISC_MESSAGE_NBR 				70

#define DCISC_RECORD_DATA			     	71
#define DCISC_ACTION_DATE			     	72
#define DCISC_CR_NBR						73
#define DCISC_CR_REV_NBR					74
#define DCISC_DB_NBR						75
#define DCISC_DB_REV_NBR					76
#define DCISC_TRANSFER_NBR                77
#define DCISC_TRANSFER_REV_NBR            78
#define DCISC_INQ_NBR             79
#define DCISC_AUT_NBR                  80
#define DCISC_INF_REV_NBR         		81
#define DCISC_PAYMENT_NBR                 82
#define DCISC_PAYMENT_REV_NBR             83
#define DCISC_FEES_COLLCT_NBR             84


#define DCISC_CR_AMOUNT					85
#define DCISC_CR_REV_AMOUNT		 		86
#define DCISC_DB_AMOUNT					87
#define DCISC_DB_REV_AMOUNT		 		88
#define DCISC_ADJUST_NBR             		89

#define DCISC_TRX_RECEIVER_COUNTRY_CODE	90
#define DCISC_TRX_ORIGINATOR_COUNTRY_CODE	91
#define DCISC_TRX_RECEIVER_ID         	92
#define DCISC_TRX_ORIGINATOR_ID			93
#define DCISC_CARD_ISSUER_REF_DATA 		94
#define DCISC_KEY_MNG_DATA		 		95
#define DCISC_NET_AMOUNT					96
#define DCISC_STLMNT_INSTITN		 		98
#define DCISC_RECEIVER_ID					99 

#define DCISC_FILE_NAME					100
#define DCISC_ACCOUNT_ID1         		101
#define DCISC_ACCOUNT_ID2         		102

#define DCISC_TRANS_DESC                  103
#define DCISC_CR_REJ_AMOUNT               104
#define DCISC_DB_REJ_AMOUNT               105
#define DCISC_CR_REJ_NUMBER               106
#define DCISC_DB_REJ_NUMBER               107
#define DCISC_CR_FEES_AMOUNT              108
#define DCISC_DB_FEES_AMOUNT              109
#define DCISC_RESERVED_DCISC_2              110
#define DCISC_RESERVED_DCISC_3              111
#define DCISC_RESERVED_DCISC_4              112
#define DCISC_RESERVED_DCISC_5              113

#define DCISC_TRANS_FEE              115 
#define DCISC_STLMNT_FEE              116
#define DCISC_PROCES_FEE              117

#define DCISC_ADTNL_TRACE_DATA    		114
#define DCISC_ORIG_MSG_TYPE               119
#define DCISC_RESERV_NATIONAL_USAGE_6     120
#define DCISC_RESERV_PRIV_USAGE_1         121
#define DCISC_NET_REF_ID      		        122  /* ELKAM20230217 PLUTONL-5914 */
#define DCISC_RESERV_PRIV_USAGE_3         123
#define DCISC_SUPPORT_INFO				124
#define DCISC_RESERVED_PRIVE_USAGE_5      125
#define DCISC_FILE_SEC_CODE               126
#define DCISC_MAC2                		127

#endif /** HPS_DCISC_FIELDS_H **/
