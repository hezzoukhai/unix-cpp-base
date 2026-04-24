#ifndef HPS_AMEX_FIELDS_H
#define HPS_AMEX_FIELDS_H

#define AMEX_BIT_MAP2              		0
#define AMEX_CARD_NBR              		1
#define AMEX_PROC_CODE             		2
#define AMEX_TRANS_AMOUNT          		3
#define AMEX_STLMNT_AMOUNT		  	4
#define AMEX_BILLING_AMOUNT		  	5 
#define AMEX_XMIT_TIME             		6
#define AMEX_STLMNT_CNVRSN_RATE     		8
#define AMEX_BILLING_CNVRSN_RATE	     	9  
#define AMEX_AUDIT_NBR            		10
#define AMEX_TRANS_TIME           		11
#define AMEX_TRANS_DATE           		12
#define AMEX_EXPIRY_DATE          		13
#define AMEX_STLMNT_DATE          		14
#define AMEX_CNVRSN_DATE          		15
#define AMEX_MERCHANT_TYPE        		17
#define AMEX_ACQR_COUNTRY         		18
#define AMEX_PAN_COUNTRY          		19
#define AMEX_FORWD_COUNTRY        		20
#define AMEX_POS_ENTRY_MODE       		21
#define AMEX_CARD_SEQUENCE_NBR     		22
#define AMEX_FUNCTION_CODE		 	23
#define AMEX_ISO_MSG_REASON                     24
#define AMEX_ACCEPTOR_ACTIV_CODE      		25
#define AMEX_AUTHOR_ID_LENGTH     		26
#define AMEX_TRANS_FEE				27
#define AMEX_ORIGINAL_AMOUNTS			29
#define AMEX_ACQR_ID	 	         	31
#define AMEX_FORWD_ID				32
#define AMEX_PAN_EXTENDED		 	33
#define AMEX_T2_DATA              		34
#define AMEX_T3_DATA              		35
#define AMEX_REFERENCE_NBR        		36
#define AMEX_AUTHOR_ID			     	37
#define AMEX_RESPONSE_CODE        		38
#define AMEX_SERVICE_CODE               	39
#define AMEX_TERMINAL_NBR         		40
#define AMEX_OUTLET_NBR				41
#define AMEX_TERMINAL_ADR		 	42
#define AMEX_ADTNL_RESP_DATA      		43
#define AMEX_T1_DATA              		44
#define AMEX_FEES_AMOUNT                        45
#define AMEX_ADTNL_DATA_PRIV      		47
#define AMEX_TRANS_CRNCY          		48
#define AMEX_STLMNT_CRNCY         		49
#define AMEX_BILLING_CRNCY        		50
#define AMEX_PIN_DATA				51
#define AMEX_SECURITY_DATA        		52
#define AMEX_ADTNL_AMOUNTS			53
#define AMEX_ORIG_DATA			        55
#define AMEX_GEOGR_DATA           		58
#define AMEX_ADTNL_POS_INFO       		59
#define AMEX_OTHER_AMOUNTS		 	60
#define AMEX_CPS_FIELDS           		61
#define AMEX_PRIV_USE_FIELDS		 	62
#define AMEX_MAC1                 		63

#define AMEX_STLMNT_CODE			65
#define AMEX_RECEIVER_COUNTRY     		67
#define AMEX_STLMNT_INST_COUNTRY		68
#define AMEX_NW_MNGMT_INFO	     		69
#define AMEX_RECORDS_DATA		     	71
#define AMEX_ACTION_DATE		     	72

#define AMEX_BILLPAYMENT_NBR			73
#define AMEX_BILLPAYMENT_NBR_REV		74
#define AMEX_ECHARGE_NBR			75
#define AMEX_ECHARGE_NBR_REV			76

#define AMEX_DEPOSIT_NBR			82
#define AMEX_DEPOSIT_NBR_REV			83

#define AMEX_CR_AMOUNT	 		85
#define AMEX_CR_REV_AMOUNT	 		86
#define AMEX_DB_AMOUNT			87
#define AMEX_DB_REV_AMOUNT		 	88

#define AMEX_INITIATOR_ORGANISM		92
#define AMEX_DESTINATION_ORGANISM		93
#define AMEX_NET_AMOUNT				96
#define AMEX_STLMNT_INSTITN		 	98

#define AMEX_RECEIVER_ID			99 
#define AMEX_ACCOUNT_ID1         		101
#define AMEX_ACCOUNT_ID2         		102

#define AMEX_ADTNL_TRACE_DATA    		114
#define AMEX_ORIG_MSG_TYPE              119

#define AMEX_ADR_VERIF_DATA      		122
#define AMEX_SUPPORT_INFO			124
#define AMEX_PRIV_USE_FIELD      		125
#define AMEX_MAC2                		127

/* SMART CARD DATA */

#define AMEX_TERM_CAPBLTY_PROFILE		129
#define AMEX_TERM_VERIF_RESULTS			130
#define AMEX_UNPREDICT_NBR			131
#define AMEX_TERM_SERIAL_NBR			132
#define AMEX_AMEX_DISCRET_DATA			133
#define AMEX_ISSUER_DISCRET_DATA			134
#define AMEX_CRYPTOGRAM				135
#define AMEX_APP_TRANS_COUNTER			136
#define AMEX_APP_INTCHG_PROFILE			137
#define AMEX_ARPC_RESP_CODE			138
#define AMEX_ISSUER_SCRIPT			141
#define AMEX_ISSUER_SCRIPT_RESULTS		142
#define AMEX_CRYPTO_TRANS_TYPE			143
#define AMEX_TERM_COUNTRY_CODE			144
#define AMEX_TERM_TRANS_DATE			145
#define AMEX_CRYPTO_AMOUNT			146
#define AMEX_CRYPTO_CRNCY_CODE			147
#define AMEX_CRYPTO_CBACK_AMOUNT		148



/* CPS fields */
#define CPS_ACI				0
#define CPS_TRANS_ID			1



/* PUF: Privat Use Fields */
#define PUF_NETWORK_ID			0
#define PUF_MSG_REASON			2
#define PUF_STIP_REASON			3
#define PUF_PLUS_PMC			4	
#define PUF_NETWORK_FLAGS		6

#endif /** HPS_AMEX_FIELDS_H **/
