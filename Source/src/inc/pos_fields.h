#ifndef HPS_POS_FIELDS_H
#define HPS_POS_FIELDS_H

#define POS_BIT_MAP2              		0
#define POS_CARD_NBR              		1
#define POS_PROC_CODE             		2
#define POS_TRANS_AMOUNT          		3
#define POS_STLMNT_AMOUNT		  	4
#define POS_BILLING_AMOUNT		  	5 
#define POS_XMIT_TIME             		6
#define POS_BILLING_AMOUNT_FEES                 7
#define POS_STLMNT_CNVRSN_RATE         		8
#define POS_BILLING_CNVRSN_RATE		     	9  

#define POS_AUDIT_NBR            		10
#define POS_TRANS_DATE_TIME           		11
#define POS_START_EXPIRY_DATE          		12
#define POS_EXPIRY_DATE          		13
#define POS_STLMNT_DATE          		14
#define POS_CNVRSN_DATE          		15
#define POS_CAPTURE_DATE                        16
#define POS_MERCHANT_TYPE        		17
#define POS_ACQR_COUNTRY         		18
#define POS_PAN_COUNTRY          		19

#define POS_FORWD_COUNTRY        		20
#define POS_POS_DATA				21
#define POS_CARD_SEQUENCE_NBR			22
#define POS_FUNCTION_CODE        		23
#define POS_MSG_REASON     			24
#define POS_ACCEPTOR_ACTIV_CODE    		25
#define POS_AUTHOR_ID_LENGTH     		26
#define POS_RECONC_DATE				27
#define POS_RECONC_INDICATOR			28
#define POS_ORGINAL_AMOUNTS			29

#define POS_ACQR_REFERENCE_DATA			30
#define POS_ACQR_ID	 	         	31
#define POS_FORWD_ID				32
#define POS_PAN_EXTENDED		 	33
#define POS_T2_DATA              		34
#define POS_T3_DATA              		35
#define POS_REFERENCE_NBR        		36
#define POS_AUTHOR_ID			     	37
#define POS_RESPONSE_CODE        		38
#define POS_SERVICE_CODE			39

#define POS_TERMINAL_NBR         		40
#define POS_OUTLET_NBR				41
#define POS_TERMINAL_ADR		 	42
#define POS_ADTNL_RESP_DATA      		43
#define POS_T1_DATA              		44
#define POS_FEES_AMOUNT				45
#define POS_ADTNL_DATA_NATL			46
#define POS_ADTNL_DATA_PRIV      		47
#define POS_TRANS_CRNCY          		48
#define POS_STLMNT_CRNCY         		49

#define POS_BILLING_CRNCY        		50
#define POS_PIN_DATA				51
#define POS_SECURITY_DATA        		52
#define POS_ADTNL_AMOUNTS			53
#define POS_ICC_DATA				54
#define POS_ORIG_DATA				55
#define POS_AUTHORIZATION_CYCLE_CODE            56
#define POS_AUTHOR_INSTITN_ID_CODE		57
#define POS_GEOGR_DATA           		58
#define POS_CUSTM_SEG_CODE       		59

#define POS_DUE_DATE     		 	60
#define POS_REDEMPTION_NBR       		61
#define POS_VERSIONS       		 	62
#define POS_MAC1                 		63
#define POS_STLMNT_CODE				65
#define POS_NBR_INSTLMNT                66 /*ZKO24092018*/
#define POS_RECEIVER_COUNTRY     		67
#define POS_STLMNT_INST_COUNTRY		 	68
#define POS_NW_MNGMT_INFO	     		69

#define POS_MESSAGE_NBR  	     		70
#define POS_RECORD_DATA			     	71
#define POS_ACTION_DATE			     	72
#define POS_CR_NBR				73
#define POS_CR_REV_NBR				74
#define POS_DB_NBR				75
#define POS_DB_REV_NBR				76

#define POS_PAYMENT_NBR 			82
#define POS_PAYMENT_REV_NBR 		83

#define POS_CR_AMOUNT				85
#define POS_CR_REV_AMOUNT		 	86
#define POS_DB_AMOUNT				87
#define POS_DB_REV_AMOUNT		 	88
#define POS_ADJUST_NBR             		89

#define POS_FILE_UPDATE				90
#define POS_FILE_SECURITY		 	91
#define POS_REPLACE_AMOUNTS		 	94
#define POS_MSG_SECURITY		 	95
#define POS_NET_AMOUNT				96
#define POS_STLMNT_INSTITN		 	98
#define POS_RECEIVER_ID				99 

#define POS_FILE_NAME				100
#define POS_ACCOUNT_ID1         		101
#define POS_ACCOUNT_ID2         		102

#define POS_RESERVED_ISO_2              109 /* HKETTANI 18/02/2004 ADD FIELD 110 CHIP DATA */
#define POS_ADTNL_TRACE_DATA    		114

#define POS_ADR_VERIF_DATA      		122
#define POS_SUPPORT_INFO			124
#define POS_MAC2                		127

#endif /** HPS_POS_FIELDS_H **/
