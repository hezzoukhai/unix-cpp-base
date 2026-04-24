#ifndef HPS_MDS_FIELDS_H
#define HPS_MDS_FIELDS_H

#define MDS_BIT_MAP2              		0
#define MDS_CARD_NBR              		1
#define MDS_PROC_CODE             		2
#define MDS_TRANS_AMOUNT          		3
#define MDS_STLMNT_AMOUNT		  	4
#define MDS_BILLING_AMOUNT		  	5 
#define MDS_XMIT_TIME             		6
#define MDS_BILLING_AMOUNT_FEES		  	7 
#define MDS_STLMNT_CNVRSN_RATE           		8
#define MDS_BILLING_CNVRSN_RATE		     	9  
#define MDS_AUDIT_NBR            		10
#define MDS_TRANS_TIME           		11
#define MDS_TRANS_DATE           		12
#define MDS_EXPIRY_DATE          		13
#define MDS_STLMNT_DATE          		14
#define MDS_CNVRSN_DATE          		15
#define MDS_CAPTURE_DATE          		16
#define MDS_MERCHANT_TYPE        		17
#define MDS_ACQR_COUNTRY         		18
#define MDS_PAN_COUNTRY          		19
#define MDS_FORWD_COUNTRY        		20
#define MDS_POS_ENTRY_MODE       		21
#define MDS_CARD_SEQUENCE_NBR     		22
#define MDS_NET_INT_ID 					23							  
#define MDS_POS_CONDITION        		24
#define MDS_POS_PIN_CAPTURE      		25
#define MDS_AUTHOR_ID_LENGTH     		26
#define MDS_TRANS_FEE				27
#define MDS_STLMNT_FEE				28/* MDS_COMP */
#define MDS_TRANS_PROCES_FEE			29/* MDS_COMP */	
#define MDS_STLMNT_PROCES_FEE			30/* MDS_COMP */	
#define MDS_ACQR_ID	 	         	31
#define MDS_FORWD_ID				32
#define MDS_PAN_EXTENDED		 	33
#define MDS_T2_DATA              		34
#define MDS_T3_DATA              		35
#define MDS_REFERENCE_NBR        		36
#define MDS_AUTHOR_ID			     	37
#define MDS_RESPONSE_CODE        		38
#define MDS_SERVICE_CODE               		39
#define MDS_TERMINAL_NBR         		40
#define MDS_OUTLET_NBR				41
#define MDS_TERMINAL_ADR		 	42
#define MDS_ADTNL_RESP_DATA      		43
#define MDS_T1_DATA              		44
#define MDS_ADTNL_DATA_ISO      		45
#define MDS_ADTNL_DATA_NAT      		46
#define MDS_ADTNL_DATA_PRIV      		47
#define MDS_TRANS_CRNCY          		48
#define MDS_STLMNT_CRNCY         		49
#define MDS_BILLING_CRNCY        		50
#define MDS_PIN_DATA				51
#define MDS_SECURITY_DATA        		52
#define MDS_ADTNL_AMOUNTS			53
#define MDS_ICC_DATA			        54
#define MDS_PAYMENT_ACCOUNT_DATA		55	/*AMER20170302: Enh17.Q2*/
#define MDS_RESERVED_NAT_1 				56								 
#define MDS_BIT_MAP_2 					64
#define MDS_AUTHOR_AGENT_ID			57
#define MDS_RESERVED_NAT_2 				58								 
#define MDS_ADVICE_REASON_CODE       		59
#define MDS_ADTNL_POS_INFO       		60
#define MDS_INT_NET_FACILITY           		61 
#define MDS_NETWORK_DATA 			62 
#define MDS_MAC1                 		63
#define MDS_ADDTIONAL_DATA               109 /*AHA20220829 PLUTONL-4980*/

#define MDS_STLMNT_CODE				65
#define MDS_EXT_PAY_CODE			66
#define MDS_RECEIVER_COUNTRY     		67
#define MDS_STLMNT_INST_COUNTRY		 	68
#define MDS_NW_MNGMT_INFO	     		69
#define MDS_MESG_NBR			     	70
#define MDS_MESG_NBR_LAST		     	71
#define MDS_ACTION_DATE			     	72

#define MDS_CREDIT_NBR				73
#define MDS_CREDIT_REV_NBR			74
#define MDS_DEBIT_NBR				75
#define MDS_DEBIT_REV_NBR			76

#define MDS_TRANSFER_NBR			77
#define MDS_TRANSFER_REV_NBR			78
#define MDS_INQUIRIES_NBR			79
#define MDS_AUTHOR_NBR				80

#define MDS_CR_PROCESS_FEE			81
#define MDS_CR_TRANS_FEE		 	82
#define MDS_DB_PROCESS_FEE			83
#define MDS_DB_TRANS_FEE			84

#define MDS_CR_AMOUNT				85
#define MDS_CR_REV_AMOUNT		 	86
#define MDS_DB_AMOUNT				87
#define MDS_DB_REV_AMOUNT		 	88

#define MDS_ORIG_DATA            		89
#define MDS_FILE_UPDATE				90
#define MDS_FILE_SECURITY		 	91
#define MDS_RESPONSE_IND		 	92
#define MDS_SERVICE_IND 		 	93
#define MDS_REPLACE_AMOUNTS		 	94
#define MDS_MSG_SECURITY		 	95
#define MDS_NET_AMOUNT				96
#define MDS_PAYEE				97
#define MDS_STLMNT_INSTI_ID		 	98

#define MDS_RECEIVER_INSTI_ID			99
#define MDS_FILE_NAME				100
#define MDS_ACCOUNT_ID1         		101
#define MDS_ACCOUNT_ID2         		102
#define MDS_DIGITAL_PAYMENT_DATA       103
#define MDS_RESERVED_MCI_1			   104
#define MDS_RESERVED_MCI_2			   105
#define MDS_RESERVED_MCI_3			   106
#define MDS_MONEYSEND_REF_DATA		   107
#define MDS_RESERVED_ISO_4	 		   108
#define MDS_ADDITIONAL_DATA_2   	   109
#define MDS_AMOUNT_ASSESS 			   110
#define MDS_ADDITIONAL_DATA_NATL	   111
#define MDS_RESERVED_NATL_3 		   112
#define MDS_RESERVED_NATL_4 		   113
#define MDS_ADTNL_TRACE_DATA    		114
#define MDS_RESERVED_NATL_6 		   115
#define MDS_RESERVED_NATL_7 		   116
#define MDS_RESERVED_NATL_8 		   117
#define MDS_RESERVED_NATL_9 		   118
#define MDS_ORIG_MSG_TYPE              119
#define MDS_AUTH_AGENT_ID   		   120
#define MDS_RECORD_DATA_ADD 		   121
#define MDS_ADR_VERIF_DATA      		122
#define MDS_MEMBER_DEF_DATA				123
#define MDS_SUPPORT_INFO			124
#define MDS_PRIV_USE_FIELD      		125
#define MDS_RECORD_DATA        		      126
#define MDS_MAC2                		127

/* SMART CARD DATA */

#define MDS_TERM_CAPBLTY_PROFILE		129
#define MDS_TERM_VERIF_RESULTS			130
#define MDS_UNPREDICT_NBR			131
#define MDS_TERM_SERIAL_NBR			132
#define MDS_MDS_DISCRET_DATA			133
#define MDS_ISSUER_DISCRET_DATA			134
#define MDS_CRYPTOGRAM				135
#define MDS_APP_TRANS_COUNTER			136
#define MDS_APP_INTCHG_PROFILE			137
#define MDS_ARPC_RESP_CODE			138
#define MDS_ISSUER_SCRIPT			141
#define MDS_ISSUER_SCRIPT_RESULTS		142
#define MDS_CRYPTO_TRANS_TYPE			143
#define MDS_TERM_COUNTRY_CODE			144
#define MDS_TERM_TRANS_DATE			145
#define MDS_CRYPTO_AMOUNT			146
#define MDS_CRYPTO_CRNCY_CODE			147
#define MDS_CRYPTO_CBACK_AMOUNT		 	148

/*Network Management Information Code */
#define	C_MDS_SAF_SESS			"060"
#define	C_MDS_SIGN_ON			"061"
#define	C_MDS_SIGN_OFF			"062"
#define C_MDS_ENCRYPTION_KEY_EXCHANGE "161" /* AHA20220929 PLUTONL-4980*/
/*....to be completed ! */
#define	C_MDS_PASSWD			"PASWD" /* Password given to authorized Processors */
#define	MDS_SOLICITATION		   "163" /* AHA20220829 PLUTONL-4980*/
#define	MDS_KEY_EXCHANGE_SUCCESS   "164" /* AHA20220829 PLUTONL-4980*/
#define MDS_KEY_EXCHANGE_FAILURE   "165" /* AHA20220829 PLUTONL-4980*/

/* CPS fields */
#define CPS_ACI				0
#define CPS_TRANS_ID			1



/* PUF: Privat Use Fields */
#define PUF_NETWORK_ID			0
#define PUF_MSG_REASON			2
#define PUF_STIP_REASON			3
#define PUF_PLUS_PMC			4
#define PUF_NETWORK_FLAGS		6


#endif /** HPS_MDS_FIELDS_H **/