#ifndef HPS_CUP_FIELDS_H
#define HPS_CUP_FIELDS_H

#define CUP_BIT_MAP2              		0
#define CUP_CARD_NBR              		1
#define CUP_PROC_CODE             		2
#define CUP_TRANS_AMOUNT          		3
#define CUP_STLMNT_AMOUNT		  	4
#define CUP_BILLING_AMOUNT		  	5 
#define CUP_XMIT_TIME             		6
#define CUP_CNVRSN_RATE           		8
#define CUP_BILLING_CNVRSN_RATE		     	9  
#define CUP_AUDIT_NBR            		10
#define CUP_TRANS_TIME           		11
#define CUP_TRANS_DATE           		12
#define CUP_EXPIRY_DATE          		13
#define CUP_STLMNT_DATE          		14
#define CUP_CNVRSN_DATE          		15
#define CUP_MERCHANT_TYPE        		17
#define CUP_ACQR_COUNTRY         		18
/*#define CUP_PAN_COUNTRY          		19*/
/*#define CUP_FORWD_COUNTRY        		20*/
#define CUP_POS_ENTRY_MODE       		21
#define CUP_CARD_SEQUENCE_NBR     		22
/*#define CUP_FUNCTION_CODE		 	23*/
#define CUP_POS_CONDITION        		24
#define CUP_POS_PIN_CAPTURE      		25
/*#define CUP_AUTHOR_ID_LENGTH     		26*/
#define CUP_TRANS_FEE				27
#define CUP_ACQR_ID	 	         	31
#define CUP_FORWD_ID				32
/*#define CUP_PAN_EXTENDED		 	33*/
#define CUP_T2_DATA              		34
#define CUP_T3_DATA              		35
#define CUP_REFERENCE_NBR        		36
#define CUP_AUTHOR_ID			     	37
#define CUP_RESPONSE_CODE        		38
/*#define CUP_SERVICE_CODE               	39*/
#define CUP_TERMINAL_NBR         		40
#define CUP_OUTLET_NBR				41
#define CUP_TERMINAL_ADR		 	42
#define CUP_ADTNL_RESP_DATA      		43
#define CUP_T1_DATA              		44
#define CUP_ADTNL_DATA_PRIV      		47
#define CUP_TRANS_CRNCY          		48
#define CUP_STLMNT_CRNCY         		49
#define CUP_BILLING_CRNCY        		50
#define CUP_PIN_DATA				51
#define CUP_SECURITY_DATA        		52
#define CUP_ADTNL_AMOUNTS			53
#define CUP_ICC_DATA     			54
/*#define CUP_I_ORIG_DATA			55*/
/*#define CUP_GEOGR_DATA           		58*/
#define CUP_ADTNL_POS_INFO       		59	/*the name of this field is Self-Defined Field */
#define CUP_AUTHEN_INFORMATION		 	60
/*#define CUP_CPS_FIELDS           		61*/
/*#define CUP_PRIV_USE_FIELDS		 	62*/	/*the name of this field  is Financial Network Data, but we don't have any field in ISO wich 								can be mapped with this field*/
/*#define CUP_MAC1                 		63

#define CUP_STLMNT_CODE				65
#define CUP_RECEIVER_COUNTRY     		67
#define CUP_STLMNT_INST_COUNTRY		 	68*/
#define CUP_NW_MNGMT_INFO	     		69
/*#define CUP_ACTION_DATE			     	72

#define CUP_CR_NBR				73
#define CUP_CR_REV_NBR				74
#define CUP_DB_NBR				75
#define CUP_DB_REV_NBR				76

#define CUP_CR_AMOUNT				85
#define CUP_CR_REV_AMOUNT		 	86
#define CUP_DB_AMOUNT				87
#define CUP_DB_REV_AMOUNT		 	88*/

#define CUP_ORIG_DATA            		89
/*#define CUP_FILE_UPDATE			90
#define CUP_FILE_SECURITY		 	91
#define CUP_REPLACE_AMOUNTS		 	94*/
#define CUP_MSG_SECURITY		 	95
/*#define CUP_NET_AMOUNT			96
#define CUP_STLMNT_INSTITN		 	98*/

#define CUP_RECEIVER_ID				99 
/*#define CUP_FILE_NAME				100*/
#define CUP_ACCOUNT_ID1         		101	/*the name of this field is CUP_ACCOUNT_ID*/
/*#define CUP_ACCOUNT_ID2         		102*/

/*#define CUP_ADTNL_TRACE_DATA    		114*/
/*#define CUP_ORIG_MSG_TYPE              	119*/
#define CUP_SYSTEM_RESERVED            		120
#define CUP_ACQ_INST_RESERVED                   121
/*#define CUP_ADR_VERIF_DATA      		122	the name of this field is Issuer Institution Reserved*/
#define CUP_ISS_INST_RESERVED                   122 
#define CUP_SUPPORT_INFO			124	
/*#define CUP_PRIV_USE_FIELD      		125*/
/*#define CUP_RECORD_DATA                       126*/
#define CUP_MAC2                		127

/* SMART CARD DATA */

#define CUP_TERM_CAPBLTY_PROFILE		129
#define CUP_TERM_VERIF_RESULTS			130
#define CUP_UNPREDICT_NBR			131
#define CUP_TERM_SERIAL_NBR			132
#define CUP_CUP_DISCRET_DATA			133
#define CUP_ISSUER_DISCRET_DATA			134
#define CUP_CRYPTOGRAM				135
#define CUP_APP_TRANS_COUNTER			136
#define CUP_APP_INTCHG_PROFILE			137
#define CUP_ARPC_RESP_CODE			138
#define CUP_ISSUER_SCRIPT			141
#define CUP_ISSUER_SCRIPT_RESULTS		142
#define CUP_CRYPTO_TRANS_TYPE			143
#define CUP_TERM_COUNTRY_CODE			144
#define CUP_TERM_TRANS_DATE			145
#define CUP_CRYPTO_AMOUNT			146
#define CUP_CRYPTO_CRNCY_CODE			147
#define CUP_CRYPTO_CBACK_AMOUNT			148



/* CPS fields */
/*#define CPS_ACI			0
#define CPS_TRANS_ID			1*/



/* PUF: Privat Use Fields */
/*#define PUF_NETWORK_ID		0
#define PUF_MSG_REASON			2
#define PUF_STIP_REASON			3
#define PUF_PLUS_PMC			4	
#define PUF_NETWORK_FLAGS		6*/


#endif /** HPS_CUP_FIELDS_H **/
