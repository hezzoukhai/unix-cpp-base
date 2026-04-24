#ifndef HPS_VISA_FIELDS_H
#define HPS_VISA_FIELDS_H

#define VISA_BIT_MAP2              		0
#define VISA_CARD_NBR              		1
#define VISA_PROC_CODE             		2
#define VISA_TRANS_AMOUNT          		3
#define VISA_STLMNT_AMOUNT		  	4
#define VISA_BILLING_AMOUNT		  	5 
#define VISA_XMIT_TIME             		6
#define VISA_CNVRSN_RATE           		8
#define VISA_BILLING_CNVRSN_RATE		     	9  
#define VISA_AUDIT_NBR            		10
#define VISA_TRANS_TIME           		11
#define VISA_TRANS_DATE           		12
#define VISA_EXPIRY_DATE          		13
#define VISA_STLMNT_DATE          		14
#define VISA_CNVRSN_DATE          		15
#define VISA_MERCHANT_TYPE        		17
#define VISA_ACQR_COUNTRY         		18
#define VISA_PAN_COUNTRY          		19
#define VISA_FORWD_COUNTRY        		20
#define VISA_POS_ENTRY_MODE       		21
#define VISA_CARD_SEQUENCE_NBR     		22
#define VISA_FUNCTION_CODE		 	23
#define VISA_POS_CONDITION        		24
#define VISA_POS_PIN_CAPTURE      		25
#define VISA_AUTHOR_ID_LENGTH     		26
#define VISA_TRANS_FEE				27
#define VISA_ACQR_ID	 	         	31
#define VISA_FORWD_ID				32
#define VISA_PAN_EXTENDED		 	33
#define VISA_T2_DATA              		34
#define VISA_T3_DATA              		35
#define VISA_REFERENCE_NBR        		36
#define VISA_AUTHOR_ID			     	37
#define VISA_RESPONSE_CODE        		38
#define VISA_SERVICE_CODE               39
#define VISA_TERMINAL_NBR         		40
#define VISA_OUTLET_NBR				41
#define VISA_TERMINAL_ADR		 	42
#define VISA_ADTNL_RESP_DATA      		43
#define VISA_T1_DATA              		44
#define VISA_ADTNL_DATA_PRIV      		47
#define VISA_TRANS_CRNCY          		48
#define VISA_STLMNT_CRNCY         		49
#define VISA_BILLING_CRNCY        		50
#define VISA_PIN_DATA				51
#define VISA_SECURITY_DATA        		52
#define VISA_ADTNL_AMOUNTS			53
#define VISA_ICC_DATA     			54
#define VISA_I_ORIG_DATA				55
#define VISA_GEOGR_DATA           		58
#define VISA_ADTNL_POS_INFO       		59
#define VISA_OTHER_AMOUNTS		 	60
#define VISA_CPS_FIELDS           		61
#define VISA_PRIV_USE_FIELDS		 	62
#define VISA_MAC1                 		63

#define VISA_STLMNT_CODE				65
#define VISA_RECEIVER_COUNTRY     		67
#define VISA_STLMNT_INST_COUNTRY		 	68
#define VISA_NW_MNGMT_INFO	     		69
#define VISA_ACTION_DATE			     	72

#define VISA_CR_NBR					73
#define VISA_CR_REV_NBR				74
#define VISA_DB_NBR					75
#define VISA_DB_REV_NBR				76

#define VISA_CR_AMOUNT				85
#define VISA_CR_REV_AMOUNT		 	86
#define VISA_DB_AMOUNT				87
#define VISA_DB_REV_AMOUNT		 	88

#define VISA_ORIG_DATA            		89
#define VISA_FILE_UPDATE				90
#define VISA_FILE_SECURITY		 	91
#define VISA_REPLACE_AMOUNTS		 	94
#define VISA_MSG_SECURITY		 	95
#define VISA_NET_AMOUNT				96
#define VISA_STLMNT_INSTITN		 	98

#define VISA_RECEIVER_ID				99 
#define VISA_FILE_NAME				100
#define VISA_ACCOUNT_ID1         		101
#define VISA_ACCOUNT_ID2         		102
#define VISA_TRANS_DESC					103
#define VISA_ENCRYPTION_DATA            109 /* FZL20220826 PLUTONL-4979 */
#define VISA_ADTNL_TRACE_DATA    		114
#define VISA_ORIG_MSG_TYPE              119

#define VISA_ADR_VERIF_DATA      		122
#define VISA_SUPPORT_INFO			124
#define VISA_PRIV_USE_FIELD      		125
#define VISA_RECORD_DATA              126
#define VISA_MAC2                		127

/* SMART CARD DATA */

#define VISA_TERM_CAPBLTY_PROFILE		129
#define VISA_TERM_VERIF_RESULTS			130
#define VISA_UNPREDICT_NBR			131
#define VISA_TERM_SERIAL_NBR			132
#define VISA_VISA_DISCRET_DATA			133
#define VISA_ISSUER_DISCRET_DATA			134
#define VISA_CRYPTOGRAM				135
#define VISA_APP_TRANS_COUNTER			136
#define VISA_APP_INTCHG_PROFILE			137
#define VISA_ARPC_RESP_CODE			138
#define VISA_ISSUER_SCRIPT			141
#define VISA_ISSUER_SCRIPT_RESULTS		142
#define VISA_CRYPTO_TRANS_TYPE			143
#define VISA_TERM_COUNTRY_CODE			144
#define VISA_TERM_TRANS_DATE			145
#define VISA_CRYPTO_AMOUNT			146
#define VISA_CRYPTO_CRNCY_CODE			147
#define VISA_CRYPTO_CBACK_AMOUNT		148



/* CPS fields */
#define CPS_ACI					0
#define CPS_TRANS_ID			1
#define CPS_MVV                19 	/*HAL15072020 PLUTONL-2111 ENHS 20.Q4 9.1.1*/
#define CPS_RISK_INFO   	   20



/* PUF: Privat Use Fields */
#define PUF_NETWORK_ID			0
#define PUF_MSG_REASON			2
#define PUF_STIP_REASON			3
#define PUF_PLUS_PMC			4	
#define PUF_NETWORK_FLAGS		6


#endif /** HPS_VISA_FIELDS_H **/
