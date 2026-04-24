#ifndef HPS_SMS_FIELDS_H
#define HPS_SMS_FIELDS_H

#define SMS_BIT_MAP2              	         0
#define SMS_CARD_NBR              	         1
#define SMS_PROC_CODE                     	 2
#define SMS_TRANS_AMOUNT          	         3
#define SMS_STLMNT_AMOUNT		  	         4
#define SMS_BILLING_AMOUNT		  	         5 
#define SMS_XMIT_TIME                	     6
#define SMS_CNVRSN_RATE                    	 8
#define SMS_BILLING_CNVRSN_RATE		         9  
#define SMS_AUDIT_NBR            	    	10
#define SMS_TRANS_TIME           		    11
#define SMS_TRANS_DATE           	    	12
#define SMS_EXPIRY_DATE          		    13
#define SMS_STLMNT_DATE          		    14
#define SMS_CNVRSN_DATE          		    15
#define SMS_CAPTURE_DATE          		    16
#define SMS_MERCHANT_TYPE        		    17
#define SMS_ACQR_COUNTRY         		    18
#define SMS_PAN_COUNTRY          		    19
#define SMS_FORWD_COUNTRY        		    20
#define SMS_POS_ENTRY_MODE       		    21
#define SMS_CARD_SEQUENCE_NBR               22
#define SMS_FUNCTION_CODE		 	        23
#define SMS_POS_CONDITION        		    24
#define SMS_POS_PIN_CAPTURE      		    25
#define SMS_AUTHOR_ID_LENGTH     		    26
#define SMS_TRANS_FEE				        27
#define SMS_ACQR_ID	 	         	        31
#define SMS_FORWD_ID				        32
#define SMS_PAN_EXTENDED		 	        33
#define SMS_T2_DATA              		    34
#define SMS_T3_DATA              		    35
#define SMS_REFERENCE_NBR        		    36
#define SMS_AUTHOR_ID			     	    37
#define SMS_RESPONSE_CODE        	    	38
#define SMS_TERMINAL_NBR         		    40
#define SMS_OUTLET_NBR						41
#define SMS_TERMINAL_ADR		 			42
#define SMS_ADTNL_RESP_DATA      		    43
#define SMS_T1_DATA              		    44
#define SMS_ADTNL_DATA_PRIV      		    47
#define SMS_TRANS_CRNCY          	    	48
#define SMS_STLMNT_CRNCY             		49
#define SMS_BILLING_CRNCY            		50
#define SMS_PIN_DATA						51
#define SMS_SECURITY_DATA            		52
#define SMS_ADTNL_AMOUNTS					53
#define SMS_ICC_DATA     					54
#define SMS_I_ORIG_DATA						55
#define SMS_GEOGR_DATA           		    58
#define SMS_ADTNL_POS_INFO       		    59
#define SMS_OTHER_AMOUNTS		 			60
#define SMS_CPS_FIELDS           		    61
#define SMS_PRIV_USE_FIELDS		 			62
#define SMS_MAC1                 	    	63

#define SMS_STLMNT_CODE						65
#define SMS_RECEIVER_COUNTRY        		67
#define SMS_STLMNT_INST_COUNTRY		     	68
#define SMS_NW_MNGMT_INFO	     			69
#define SMS_ACTION_DATE			     		72

#define SMS_CR_NBR							73
#define SMS_CR_REV_NBR						74
#define SMS_DB_NBR		    				75
#define SMS_DB_REV_NBR						76

#define SMS_CR_AMOUNT						85
#define SMS_CR_REV_AMOUNT					86
#define SMS_DB_AMOUNT		    			87
#define SMS_DB_REV_AMOUNT		 			88

#define SMS_ORIG_DATA            		89
#define SMS_FILE_UPDATE					90
#define SMS_FILE_SECURITY		 		91
#define SMS_REPLACE_AMOUNTS				94
#define SMS_MSG_SECURITY		 		95
#define SMS_NET_AMOUNT					96
#define SMS_STLMNT_INSTITN		 		98

#define SMS_RECEIVER_ID					99 
#define SMS_FILE_NAME					100
#define SMS_ACCOUNT_ID1         		101
#define SMS_ACCOUNT_ID2         		102
#define SMS_TRANS_DESC					103
#define SMS_ENCRYPTION_DATA             109 /* FZL20220829 PLUTONL-4979 */
#define SMS_ADTNL_TRACE_DATA    		114
#define SMS_SETT_SERVICE_DATA           118  /* AER 22112011 NNS integration -------*/


#define SMS_ADR_VERIF_DATA      		122
#define SMS_SUPPORT_INFO				124
#define SMS_PRIV_USE_FIELD              125  /* AMA */
#define SMS_RECORD_DATA                 126
#define SMS_MAC2                		127

#ifdef SMART_CARD
/* SMART CARD DATA */

#define SMS_TERM_CAPBLTY_PROFILE		129
#define SMS_TERM_VERIF_RESULTS			130
#define SMS_UNPREDICT_NBR				131
#define SMS_TERM_SERIAL_NBR				132
#define SMS_VISA_DISCRET_DATA			133
#define SMS_ISSUER_DISCRET_DATA			134
#define SMS_CRYPTOGRAM					135
#define SMS_APP_TRANS_COUNTER			136
#define SMS_APP_INTCHG_PROFILE			137
#define SMS_ARPC_RESP_CODE				138
#define SMS_ISSUER_SCRIPT				141
#define SMS_ISSUER_SCRIPT_RESULTS		142
#define SMS_CRYPTO_TRANS_TYPE		    143
#define SMS_TERM_COUNTRY_CODE			144
#define SMS_TERM_TRANS_DATE				145
#define SMS_CRYPTO_AMOUNT				146
#define SMS_CRYPTO_CRNCY_CODE			147

#endif 


/* CPS fields */
#define CPS_ACI									0
#define CPS_TRANS_ID							1


/* PUF: Privat Use Fields */
#define PUF_NETWORK_ID							0
#define PUF_MSG_REASON							2
#define PUF_STIP_REASON							3
#define PUF_PLUS_PMC							4	
#define PUF_CHARGEBACK_FLAGS					5/*ICH17012018 Chargeback reduction*/
#define PUF_NETWORK_FLAGS						6
#define PUF_FPI                                 18 /*SWI200904 ENH April 2009*/
#define CPS_RISK_INFO   	                    20

#endif /** HPS_SMS_FIELDS_H **/
