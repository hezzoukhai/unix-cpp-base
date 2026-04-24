#ifndef HPS_ISO_FIELDS_H
#define HPS_ISO_FIELDS_H

#define ISO_BIT_MAP2              		0
#define ISO_CARD_NBR              		1
#define ISO_PROC_CODE             		2
#define ISO_TRANS_AMOUNT          		3
#define ISO_STLMNT_AMOUNT		  		4
#define ISO_BILLING_AMOUNT		  		5 
#define ISO_XMIT_TIME             		6
#define ISO_BILLING_AMOUNT_FEES         7
#define ISO_STLMNT_CNVRSN_RATE          8
#define ISO_BILLING_CNVRSN_RATE		   	9  

#define ISO_AUDIT_NBR            		10
#define ISO_TRANS_DATE_TIME           	11
#define ISO_START_EXPIRY_DATE          	12
#define ISO_EXPIRY_DATE          		13
#define ISO_STLMNT_DATE          		14
#define ISO_CNVRSN_DATE          		15
#define ISO_CAPTURE_DATE                16
#define ISO_MERCHANT_TYPE        		17
#define ISO_ACQR_COUNTRY         		18
#define ISO_PAN_COUNTRY          		19

#define ISO_FORWD_COUNTRY        		20
#define ISO_POS_DATA					21
#define ISO_CARD_SEQUENCE_NBR			22
#define ISO_FUNCTION_CODE        		23
#define ISO_MSG_REASON     				24
#define ISO_ACCEPTOR_ACTIV_CODE    		25
#define ISO_AUTHOR_ID_LENGTH     		26
#define ISO_RECONC_DATE					27
#define ISO_RECONC_INDICATOR			28
#define ISO_ORIGINAL_AMOUNTS			29

#define ISO_ACQR_REFERENCE_DATA			30
#define ISO_ACQR_ID	 	         		31
#define ISO_FORWD_ID					32
#define ISO_PAN_EXTENDED		 		33
#define ISO_T2_DATA              		34
#define ISO_T3_DATA              		35
#define ISO_REFERENCE_NBR        		36
#define ISO_AUTHOR_ID			     	37
#define ISO_RESPONSE_CODE        		38
#define ISO_SERVICE_CODE			39

#define ISO_TERMINAL_NBR         		40
#define ISO_OUTLET_NBR				41
#define ISO_TERMINAL_ADR		 	42
#define ISO_ADTNL_RESP_DATA      		43
#define ISO_T1_DATA              		44
#define ISO_FEES_AMOUNT				45
#define ISO_ADTNL_DATA_NATL			46
#define ISO_ADTNL_DATA_PRIV      		47
#define ISO_TRANS_CRNCY          		48
#define ISO_STLMNT_CRNCY         		49

#define ISO_BILLING_CRNCY        		50
#define ISO_PIN_DATA					51
#define ISO_SECURITY_DATA        		52
#define ISO_ADTNL_AMOUNTS				53
#define ISO_ICC_DATA					54
#define ISO_ORIG_DATA					55
#define ISO_AUTHORIZATION_CYCLE_CODE    56
#define ISO_AUTHOR_INSTITN_ID_CODE		57
#define ISO_GEOGR_DATA           		58
#define ISO_ADTNL_POS_INFO       		59

#define ISO_NET_POS_DATA		 		60
#define ISO_CPS_FIELDS           		61
#define ISO_NETWORK_DATA   		 		62
#define ISO_MAC1                 		63
#define ISO_STLMNT_CODE					65
#define ISO_RECEIVER_COUNTRY     		67
#define ISO_STLMNT_INST_COUNTRY		 	68
#define ISO_NW_MNGMT_INFO	     		69
#define ISO_MESSAGE_NBR 				70

#define ISO_RECORD_DATA			     	71
#define ISO_ACTION_DATE			     	72
#define ISO_CR_NBR						73
#define ISO_CR_REV_NBR					74
#define ISO_DB_NBR						75
#define ISO_DB_REV_NBR					76
#define ISO_TRANSFER_NBR                77
#define ISO_TRANSFER_REV_NBR            78
#define ISO_INQ_NBR             79
#define ISO_AUT_NBR                  80
#define ISO_INF_REV_NBR         		81
#define ISO_PAYMENT_NBR                 82
#define ISO_PAYMENT_REV_NBR             83
#define ISO_FEES_COLLCT_NBR             84


#define ISO_CR_AMOUNT					85
#define ISO_CR_REV_AMOUNT		 		86
#define ISO_DB_AMOUNT					87
#define ISO_DB_REV_AMOUNT		 		88
#define ISO_ADJUST_NBR             		89

#define ISO_TRX_RECEIVER_COUNTRY_CODE	90
#define ISO_TRX_ORIGINATOR_COUNTRY_CODE	91
#define ISO_TRX_RECEIVER_ID         	92
#define ISO_TRX_ORIGINATOR_ID			93
#define ISO_CARD_ISSUER_REF_DATA 		94
#define ISO_KEY_MNG_DATA		 		95
#define ISO_NET_AMOUNT					96
#define ISO_STLMNT_INSTITN		 		98
#define ISO_RECEIVER_ID					99 

#define ISO_FILE_NAME					100
#define ISO_ACCOUNT_ID1         		101
#define ISO_ACCOUNT_ID2         		102

#define ISO_TRANS_DESC                  103
#define ISO_CR_REJ_AMOUNT               104
#define ISO_DB_REJ_AMOUNT               105
#define ISO_CR_REJ_NUMBER               106
#define ISO_DB_REJ_NUMBER               107
#define ISO_CR_FEES_AMOUNT              108

#define ISO_RESERVED_ISO_2              109 /* HKETTANI 19022004 CORRECTION FIELDS 110 */
#define ISO_RESERVED_ISO_3              111
#define ISO_RESERVED_ISO_4              112
#define ISO_RESERVED_ISO_5              113

#define ISO_TRANS_FEE              115 /* MDS - Dec 24, 2002 */
/*#define ISO_QR_CODE              116*/
#define ISO_STLMNT_FEE              116
#define ISO_PEOCES_FEE              117
#define ISO_RESERVED_PRIVE_USAGE_4      118   /* New Pin Block MKB 25 Nov 2006 Pin change */

#define ISO_ADTNL_TRACE_DATA    		114
#define ISO_ORIG_MSG_TYPE               119
#define ISO_RESERV_NATIONAL_USAGE_6     120
#define ISO_RESERV_PRIV_USAGE_1         121
#define ISO_ADR_VERIF_DATA      		122
#define ISO_RESERV_PRIV_USAGE_3         123
#define ISO_SUPPORT_INFO				124
#define ISO_RESERVED_PRIVE_USAGE_5      125
#define ISO_FILE_SEC_CODE               126
#define ISO_MAC2                		127

#endif /** HPS_ISO_FIELDS_H **/

