#ifndef HPS_JCC_FIELDS_H
#define HPS_JCC_FIELDS_H

#define JCC_BIT_MAP2              		0
#define JCC_CARD_NBR              		1
#define JCC_PROC_CODE             		2
#define JCC_TRANS_AMOUNT          		3
#define JCC_STLMNT_AMOUNT		  		4
#define JCC_BILLING_AMOUNT		  		5 
#define JCC_XMIT_TIME             		6
#define JCC_BILLING_AMOUNT_FEES         7
#define JCC_STLMNT_CNVRSN_RATE         	8
#define JCC_BILLING_CNVRSN_RATE		   	9  

#define JCC_AUDIT_NBR            		10
#define JCC_TRANS_TIME                	11
#define JCC_TRANS_DATE                 	12
#define JCC_EXPIRY_DATE          		13
#define JCC_STLMNT_DATE          		14
#define JCC_CNVRSN_DATE          		15
#define JCC_CAPTURE_DATE                16
#define JCC_MERCHANT_TYPE        		17
#define JCC_ACQR_COUNTRY         		18
#define JCC_PAN_COUNTRY          		19

#define JCC_FORWD_COUNTRY        		20
#define JCC_POS_ENTRY_MODE				21
#define JCC_CARD_SEQUENCE_NBR			22
#define JCC_NETWORK_INT_ID       		23
#define JCC_POS_CONDITION   			24
#define JCC_POS_PIN_CAPTURE_CODE   		25
#define JCC_AUTHOR_ID_LENGTH     		26
#define JCC_TRANS_AMOUNT_FEE			27
#define JCC_STLMNT_AMOUNT_FEE			28
#define JCC_TRANS_PROC_FEE 				29

#define JCC_STLMNT_PROC_FEE    			30
#define JCC_ACQR_ID	 	         		31
#define JCC_FORWD_ID					32
#define JCC_PAN_EXTENDED		 		33
#define JCC_T2_DATA              		34
#define JCC_T3_DATA              		35
#define JCC_REFERENCE_NBR        		36
#define JCC_AUTHOR_ID			     	37
#define JCC_RESPONSE_CODE        		38
#define JCC_SERVICE_CODE				39

#define JCC_TERMINAL_NBR         		40
#define JCC_OUTLET_NBR					41
#define JCC_TERMINAL_ADR		 		42
#define JCC_ADTNL_RESP_DATA      		43
#define JCC_T1_DATA              		44
#define JCC_ADTNL_DATA_ISO				45
#define JCC_ADTNL_DATA_NATL				46
#define JCC_ADTNL_DATA_PRIV      		47
#define JCC_TRANS_CRNCY          		48
#define JCC_STLMNT_CRNCY         		49

#define JCC_BILLING_CRNCY        		50
#define JCC_PIN_DATA					51
#define JCC_SECURITY_DATA        		52
#define JCC_ADTNL_AMOUNTS				53
#define JCC_ICC_DATA					54
#define JCC_RES_ISO1 	 				55
#define JCC_RES_NATL1                   56
#define JCC_RES_NATL2              		57
#define JCC_NAT_POS_GEO_DATA        	58
#define JCC_ADTNL_POS_INFO       		59

#define JCC_POS_DATA      		 		60
#define JCC_INF_DATA             		61
#define JCC_BANKNET_DATA   		 		62
#define JCC_MAC1                 		63
#define JCC_STLMNT_CODE					65
#define JCC_EXTD_PYMT_CODE       		66
#define JCC_RCV_INST_COUNTRY     		67
#define JCC_STLMNT_INST_COUNTRY		 	68
#define JCC_NW_MNGMT_INFO	     		69

#define JCC_MSG_NBR    			     	70
#define JCC_MSG_LAST_NBR		     	71
#define JCC_ACTION_DATE 		     	72
#define JCC_CR_NBR						73
#define JCC_CR_REV_NBR					74
#define JCC_DB_NBR						75
#define JCC_DB_REV_NBR					76
#define JCC_TR_NBR						77
#define JCC_TR_REV_NBR					78
#define JCC_INQ_NBR						79

#define JCC_AUT_NBR						80
#define JCC_CR_PROC_FEE_AMOUNT			81
#define JCC_CR_TRANS_FEE_AMOUNT			82
#define JCC_DB_PROC_FEE_AMOUNT			83
#define JCC_DB_TRANS_FEE_AMOUNT			84
#define JCC_CR_AMOUNT					85
#define JCC_CR_REV_AMOUNT		 		86
#define JCC_DB_AMOUNT					87
#define JCC_DB_REV_AMOUNT		 		88
#define JCC_ORIG_DATA              		89

#define JCC_FILE_UPDATE					90
#define JCC_FILE_SECURITY		 		91
#define JCC_RESP_INDICATOR		 		92
#define JCC_SERV_INDICATOR		 		93
#define JCC_REPLACE_AMOUNTS		 		94
#define JCC_MSG_SECURITY		 		95
#define JCC_NET_AMOUNT					96
#define JCC_PAYEE     					97
#define JCC_STLMNT_INSTITN		 		98
#define JCC_RECEIVER_ID					99 

#define JCC_FILE_NAME					100
#define JCC_TRANS_DESC                  103  /*SWI24022015 ENH15.1*/
#define JCC_PRIV_USE_FIELD      		        125  /*YK March 2006 Visa e-commerce*/
#define JCC_FILE_RECORD 				126
#define JCC_MAC2                		127

#endif /** HPS_JCC_FIELDS_H **/
