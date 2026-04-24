#ifndef HPS_CIS_FIELDS_H
#define HPS_CIS_FIELDS_H

#define CIS_BIT_MAP2              		0
#define CIS_CARD_NBR              		1
#define CIS_PROC_CODE             		2
#define CIS_TRANS_AMOUNT          		3
#define CIS_STLMNT_AMOUNT		  	4
#define CIS_BILLING_AMOUNT		  	5 
#define CIS_XMIT_TIME             		6
#define CIS_BILLING_AMOUNT_FEES                 7
#define CIS_STLMNT_CNVRSN_RATE         		8
#define CIS_BILLING_CNVRSN_RATE		     	9  

#define CIS_AUDIT_NBR            		10
#define CIS_TRANS_TIME                		11
#define CIS_TRANS_DATE                 		12
#define CIS_EXPIRY_DATE          		13
#define CIS_STLMNT_DATE          		14
#define CIS_CNVRSN_DATE          		15
#define CIS_CAPTURE_DATE                        16
#define CIS_MERCHANT_TYPE        		17
#define CIS_ACQR_COUNTRY         		18
#define CIS_PAN_COUNTRY          		19

#define CIS_FORWD_COUNTRY        		20
#define CIS_POS_ENTRY_MODE				21
#define CIS_CARD_SEQUENCE_NBR			22
#define CIS_NETWORK_INT_ID       		23
#define CIS_POS_CONDITION				24
#define CIS_POS_PIN_CAPTURE_CODE   		25
#define CIS_AUTHOR_ID_LENGTH     		26
#define CIS_TRANS_AMOUNT_FEE			27
#define CIS_STLMNT_AMOUNT_FEE			28
#define CIS_TRANS_PROC_FEE 			29

#define CIS_STLMNT_PROC_FEE    			30
#define CIS_ACQR_ID	 	         	31
#define CIS_FORWD_ID				32
#define CIS_PAN_EXTENDED		 	33
#define CIS_T2_DATA              		34
#define CIS_T3_DATA              		35
#define CIS_REFERENCE_NBR        		36
#define CIS_AUTHOR_ID			     	37
#define CIS_RESPONSE_CODE        		38
#define CIS_SERVICE_CODE			39

#define CIS_TERMINAL_NBR         		40
#define CIS_OUTLET_NBR				41
#define CIS_TERMINAL_ADR		 	42
#define CIS_ADTNL_RESP_DATA      		43
#define CIS_T1_DATA              		44
#define CIS_ADTNL_DATA_ISO			45
#define CIS_ADTNL_DATA_NATL			46
#define CIS_ADTNL_DATA_PRIV      		47
#define CIS_TRANS_CRNCY          		48
#define CIS_STLMNT_CRNCY         		49

#define CIS_BILLING_CRNCY        		50
#define CIS_PIN_DATA				51
#define CIS_SECURITY_DATA        		52
#define CIS_ADTNL_AMOUNTS			53
#define CIS_ICC_DATA				54
/*#define CIS_RES_ISO1 	 			55*/
#define CIS_PAYMENT_ACCOUNT_DATA                55      /*AMER20170302: Enh17.Q2*/
#define CIS_RES_NATL1                           56
#define CIS_RES_NATL2              		57
#define CIS_RES_NATL3               		58
#define CIS_ADV_REASON_CODE      		59

#define CIS_ADTNL_POS_INFO		 	60
#define CIS_CPS_FIELDS             		61
#define CIS_BANKNET_DATA   		 	62
#define CIS_MAC1                 		63
#define CIS_BIT_MAP_2 					 64							  
#define CIS_STLMNT_CODE				65
#define CIS_EXTD_PYMT_CODE       		66
#define CIS_RCV_INST_COUNTRY     		67
#define CIS_STLMNT_INST_COUNTRY		 	68
#define CIS_NW_MNGMT_INFO	     		69

#define CIS_MSG_NBR    			     	70
#define CIS_MSG_LAST_NBR		     	71
#define CIS_ACTION_DATE 		     	72
#define CIS_CR_NBR				73
#define CIS_CR_REV_NBR				74
#define CIS_DB_NBR				75
#define CIS_DB_REV_NBR				76
#define CIS_TR_NBR				77
#define CIS_TR_REV_NBR				78
#define CIS_INQ_NBR				79

#define CIS_AUT_NBR				80
#define CIS_CR_PROC_FEE_AMOUNT			81
#define CIS_CR_TRANS_FEE_AMOUNT			82
#define CIS_DB_PROC_FEE_AMOUNT			83
#define CIS_DB_TRANS_FEE_AMOUNT			84
#define CIS_CR_AMOUNT				85
#define CIS_CR_REV_AMOUNT		 	86
#define CIS_DB_AMOUNT				87
#define CIS_DB_REV_AMOUNT		 	88
#define CIS_ORIG_DATA              		89

#define CIS_FILE_UPDATE				90
#define CIS_FILE_SECURITY		 	91
#define CIS_RESP_INDICATOR		 	92
#define CIS_SERV_INDICATOR		 	93
#define CIS_REPLACE_AMOUNTS		 	94
#define CIS_MSG_SECURITY		 	95
#define CIS_NET_AMOUNT				96
#define CIS_PAYEE     				97
#define CIS_STLMNT_INSTITN		 	98
#define CIS_RECEIVER_ID				99 

#define CIS_FILE_NAME				100
#define CIS_ACCOUNT_ID1         		101
#define CIS_ACCOUNT_ID2         		102
#define CIS_DIGITAL_PAYMENT_DATA        103
#define CIS_RES_MC_USE_1    			104
#define CIS_RES_MC_USE_2    			105
#define CIS_RES_MC_USE_3    			106								  
#define CIS_MONEYSEND_REF_DATA			107
#define CIS_RES_ISO_USE_1 				108
#define CIS_ADDNTL_DATA_2 				109
#define CIS_RES_ISO_USE_2 				110
#define CIS_ADDNTL_DATA_NAT_USE 		111
#define CIS_RES_NAT_USE_4 				112
#define CIS_RES_NAT_USE_5 				113
#define CIS_RES_NAT_USE_6 				114
#define CIS_RES_NAT_USE_7 				115
#define CIS_RES_NAT_USE_8 				116
#define CIS_RES_NAT_USE_9 				117
#define CIS_RES_NAT_USE_10 				118								 
#define CIS_RECORD_DATA         		119

#define CIS_AUTHOR_AGENT        		120
#define CIS_ADTNL_RECORD_DATA   		121
#define CIS_ADR_VERIF_DATA      		122
#define CIS_MEMBER_DEF_DATA   		    123 /* MMJ14092010 enh-GCMS-10.2 */
#define CIS_NEW_PIN_INFO			    124
#define CIS_PRIV_DATA_1 				125
#define CIS_PRIV_DATA_2 				126							   
#define CIS_MAC2                		127


/*Network Management Information Code */
#define	C_CIS_SAF_SESS			"060"	
/*#define	C_CIS_SIGN_ON			"061"	*/
/*#define	C_CIS_SIGN_OFF			"062"*/

/* START MMJ150109 */
#define C_CIS_ECHO_TEST        "270"
/* END MMJ150109 */

/*KCH13032012 SESSION ACTIVATION MNG*/
#define CIS_ACTIVATION         "081"
#define CIS_DEACTIVATION       "082"
#define CIS_KEY_EXCHANGE_SUCCESS  "164" /* AHA20220921 PLUTONL-4980*/
#define CIS_KEY_EXCHANGE_FAILURE  "165" /* AHA20220921 PLUTONL-4980*/
#define CIS_SOLICITATION          "163" /* AHA20220921 PLUTONL-4980*/
/*Tracks Separators */
#define TRACK2_SEPARATOR        0x3D
#define TRACK2_SEPARATOR2       0x44
#define TRACK1_SEPARATOR        0x5E




#endif /** HPS_CIS_FIELDS_H **/
