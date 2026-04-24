#ifndef HPS_ICCPROD_FIELDS_H
#define HPS_ICCPROD_FIELDS_H

#define ICCPROD_BIT_MAP2              		0
#define ICCPROD_CARD_NBR              		1
#define ICCPROD_PROC_CODE             		2
#define ICCPROD_TRANS_AMOUNT          		3
#define ICCPROD_STLMNT_AMOUNT		  	4
#define ICCPROD_BILLING_AMOUNT		  	5 
#define ICCPROD_XMIT_TIME             		6
#define ICCPROD_BILLING_AMOUNT_FEES             7
#define ICCPROD_STLMNT_CNVRSN_RATE     		8
#define ICCPROD_BILLING_CNVRSN_RATE	     	9  

#define ICCPROD_AUDIT_NBR            		10
#define ICCPROD_TRANS_TIME             		11
#define ICCPROD_TRANS_DATE             		12
#define ICCPROD_EXPIRY_DATE          		13
#define ICCPROD_STLMNT_DATE          		14
#define ICCPROD_CNVRSN_DATE          		15
#define ICCPROD_CAPTURE_DATE                    16
#define ICCPROD_MERCHANT_TYPE        		17
#define ICCPROD_ACQR_COUNTRY         		18
#define ICCPROD_PAN_COUNTRY          		19

#define ICCPROD_FORWD_COUNTRY        		20
#define ICCPROD_POS_ENTRY_MODE			21
#define ICCPROD_CARD_SEQUENCE_NBR		22
#define ICCPROD_NETWORK_INT_ID       		23
#define ICCPROD_POS_CONDITION			24
#define ICCPROD_POS_PIN_CAPTURE_CODE   		25
#define ICCPROD_AUTHOR_ID_LENGTH     		26
#define ICCPROD_TRANS_AMOUNT_FEE		27
#define ICCPROD_STLMNT_AMOUNT_FEE		28
#define ICCPROD_TRANS_PROC_FEE 			29

#define ICCPROD_STLMNT_PROC_FEE    		30
#define ICCPROD_ACQR_ID	 	         	31
#define ICCPROD_FORWD_ID			32
#define ICCPROD_PAN_EXTENDED		 	33
#define ICCPROD_T2_DATA              		34
#define ICCPROD_T3_DATA              		35
#define ICCPROD_REFERENCE_NBR        		36
#define ICCPROD_AUTHOR_ID		     	37
#define ICCPROD_RESPONSE_CODE        		38
#define ICCPROD_SERVICE_CODE			39

#define ICCPROD_TERMINAL_NBR         		40
#define ICCPROD_OUTLET_NBR			41
#define ICCPROD_TERMINAL_ADR		 	42
#define ICCPROD_ADTNL_RESP_DATA      		43
#define ICCPROD_T1_DATA              		44
#define ICCPROD_ADTNL_DATA_ISO			45
#define ICCPROD_ADTNL_DATA_NATL			46
#define ICCPROD_ADTNL_DATA_PRIV      		47
#define ICCPROD_TRANS_CRNCY          		48
#define ICCPROD_STLMNT_CRNCY         		49

#define ICCPROD_BILLING_CRNCY        		50
#define ICCPROD_PIN_DATA			51
#define ICCPROD_SECURITY_DATA        		52
#define ICCPROD_ADTNL_AMOUNTS			53
#define ICCPROD_ICC_DATA			54
#define ICCPROD_RES_ISO1 	 		55
#define ICCPROD_RES_NATL1                       56
#define ICCPROD_RES_NATL2              		57
#define ICCPROD_RES_NATL3              		58
#define ICCPROD_ADV_REASON_CODE      		59

#define ICCPROD_ADTNL_POS_INFO		 	60
#define ICCPROD_CPS_FIELDS             		61
#define ICCPROD_BANKNET_DATA   		 	62
#define ICCPROD_MAC1                 		63
#define ICCPROD_STLMNT_CODE			65
#define ICCPROD_EXTD_PYMT_CODE       		66
#define ICCPROD_RCV_INST_COUNTRY     		67
#define ICCPROD_STLMNT_INST_COUNTRY	 	68
#define ICCPROD_NW_MNGMT_INFO	     		69

#define ICCPROD_MSG_NBR    		     	70
#define ICCPROD_MSG_LAST_NBR		     	71
#define ICCPROD_ACTION_DATE 		     	72
#define ICCPROD_CR_NBR				73
#define ICCPROD_CR_REV_NBR			74
#define ICCPROD_DB_NBR				75
#define ICCPROD_DB_REV_NBR			76
#define ICCPROD_TR_NBR				77
#define ICCPROD_TR_REV_NBR			78
#define ICCPROD_INQ_NBR				79

#define ICCPROD_AUT_NBR				80
#define ICCPROD_CR_PROC_FEE_AMOUNT		81
#define ICCPROD_CR_TRANS_FEE_AMOUNT		82
#define ICCPROD_DB_PROC_FEE_AMOUNT		83
#define ICCPROD_DB_TRANS_FEE_AMOUNT		84
#define ICCPROD_CR_AMOUNT			85
#define ICCPROD_CR_REV_AMOUNT		 	86
#define ICCPROD_DB_AMOUNT			87
#define ICCPROD_DB_REV_AMOUNT		 	88
#define ICCPROD_ORIG_DATA              		89

#define ICCPROD_FILE_UPDATE			90
#define ICCPROD_FILE_SECURITY		 	91
#define ICCPROD_RESP_INDICATOR		 	92
#define ICCPROD_SERV_INDICATOR		 	93
#define ICCPROD_REPLACE_AMOUNTS		 	94
#define ICCPROD_MSG_SECURITY		 	95
#define ICCPROD_NET_AMOUNT			96
#define ICCPROD_PAYEE     			97
#define ICCPROD_STLMNT_INSTITN		 	98
#define ICCPROD_RECEIVER_ID			99 

#define ICCPROD_FILE_NAME			100
#define ICCPROD_ACCOUNT_ID1         		101
#define ICCPROD_ACCOUNT_ID2         		102
#define ICCPROD_TRANS_DESC          		103
#define ICCPROD_RECORD_DATA         		119

#define ICCPROD_AUTHOR_AGENT        		120
#define ICCPROD_ADTNL_RECORD_DATA   		121
#define ICCPROD_ADR_VERIF_DATA      		122
#define ICCPROD_SUPPORT_INFO			124
#define ICCPROD_MAC2                		127


/*Network Management Information Code */
#define	C_ICCPROD_SAF_SESS			"060"	
#define	C_ICCPROD_SIGN_ON			"061"	
#define	C_ICCPROD_SIGN_OFF			"062"

#endif /** HPS_ICCPROD_FIELDS_H **/
