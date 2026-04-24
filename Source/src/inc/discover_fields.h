#ifndef HPS_DSCOVER_FIELDS_H
#define HPS_DSCOVER_FIELDS_H



#define DSCOVER_BIT_MAP2              		   0
#define DSCOVER_CARD_NBR              		   1
#define DSCOVER_PROC_CODE             		   2
#define DSCOVER_TRANS_AMOUNT          		   3
#define DSCOVER_STLMNT_AMOUNT		  		       4
#define DSCOVER_BILLING_AMOUNT		  		   5 
#define DSCOVER_XMIT_TIME             		   6
#define DSCOVER_BILLING_AMOUNT_FEES              7
#define DSCOVER_STLMNT_CNVRSN_RATE               8
#define DSCOVER_BILLING_CNVRSN_RATE              9  

#define DSCOVER_AUDIT_NBR                        10
#define DSCOVER_TRANS_TIME                       11
#define DSCOVER_TRANS_DATE                       12    
#define DSCOVER_EXPIRY_DATE                      13
#define DSCOVER_STLMNT_DATE                      14
#define DSCOVER_CNVRSN_DATE                      15
#define DSCOVER_CAPTURE_DATE                     16
#define DSCOVER_MERCHANT_TYPE                    17
#define DSCOVER_ACQR_COUNTRY                     18
#define DSCOVER_PAN_COUNTRY                      19
                                            
#define DSCOVER_FORWD_COUNTRY                    20
#define DSCOVER_POS_DATA                         21
#define DSCOVER_CARD_SEQUENCE_NBR                22
#define DSCOVER_FUNCTION_CODE                    23
#define DSCOVER_MSG_REASON                       24
#define DSCOVER_POS_ID_PIN_CAP_CODE              25
#define DSCOVER_AUTHOR_ID_LENGTH                 26
#define DSCOVER_RECONC_DATE                      27
#define DSCOVER_RECONC_INDICATOR                 28
#define DSCOVER_ORIGINAL_AMOUNTS                 29

#define DSCOVER_ACQR_REFERENCE_DATA              30
#define DSCOVER_ACQR_ID                          31
#define DSCOVER_FORWD_ID                         32
#define DSCOVER_PAN_EXTENDED                     33
#define DSCOVER_T2_DATA                          34
#define DSCOVER_T3_DATA                          35
#define DSCOVER_REFERENCE_NBR                    36
#define DSCOVER_AUTHOR_ID                        37
#define DSCOVER_RESPONSE_CODE                    38
#define DSCOVER_SERVICE_CODE                     39
                                            
#define DSCOVER_TERMINAL_NBR                     40
#define DSCOVER_OUTLET_NBR                       41
#define DSCOVER_TERMINAL_ADR                     42
#define DSCOVER_ADTNL_RESP_DATA                  43
#define DSCOVER_T1_DATA                          44
#define DSCOVER_FEES_AMOUNT                      45
#define DSCOVER_ADTNL_DATA_NATL                  46
#define DSCOVER_ADTNL_DATA_PRIV                  47
#define DSCOVER_TRANS_CRNCY                      48
#define DSCOVER_STLMNT_CRNCY                     49

#define DSCOVER_BILLING_CRNCY                    50
#define DSCOVER_PIN_DATA                         51
#define DSCOVER_SECURITY_CTRL_INFO               52
#define DSCOVER_ADTNL_AMOUNTS                    53
#define DSCOVER_ICC_DATA                         54
#define DSCOVER_ORIG_DATA                        55
#define DSCOVER_AUTHORIZATION_CYCLE_CODE         56
#define DSCOVER_AUTHOR_INSTITN_ID_CODE           57
#define DSCOVER_GEOGR_DATA                       58
#define DSCOVER_ADTNL_POS_INFO                   59

#define DSCOVER_NET_POS_DATA                     60
#define DSCOVER_GEOGRAPHIC_DATA                  61
#define DSCOVER_ADR_VERIF_DATA                   62
#define DSCOVER_MAC1                             63
#define DSCOVER_STLMNT_CODE                      65
#define DSCOVER_RECEIVER_COUNTRY                 67
#define DSCOVER_STLMNT_INST_COUNTRY              68
#define DSCOVER_NW_MNGMT_INFO                    69
#define DSCOVER_MESSAGE_NBR                      70

#define DSCOVER_RECORD_DATA                      71
#define DSCOVER_ACTION_DATE                      72
#define DSCOVER_CR_NBR                           73
#define DSCOVER_CR_REV_NBR                       74
#define DSCOVER_DB_NBR                           75
#define DSCOVER_DB_REV_NBR                       76
#define DSCOVER_TRANSFER_NBR                     77
#define DSCOVER_TRANSFER_REV_NBR                 78
#define DSCOVER_INQ_NBR                          79
#define DSCOVER_AUT_NBR                          80
#define DSCOVER_INF_REV_NBR                      81
#define DSCOVER_PAYMENT_NBR                      82
#define DSCOVER_PAYMENT_REV_NBR                  83
#define DSCOVER_FEES_COLLCT_NBR                  84
#define DSCOVER_CR_AMOUNT                        85
#define DSCOVER_CR_REV_AMOUNT                    86
#define DSCOVER_DB_AMOUNT                        87
#define DSCOVER_DB_REV_AMOUNT                    88
#define DSCOVER_ORIGINAL_DATA                    89

#define DSCOVER_TRX_RECEIVER_COUNTRY_CODE        90
#define DSCOVER_TRX_ORIGINATOR_COUNTRY_CODE      91
#define DSCOVER_TRX_RECEIVER_ID                  92
#define DSCOVER_TRX_ORIGINATOR_ID                93
#define DSCOVER_REPLACEMENT_AMOUNTS              94
#define DSCOVER_KEY_MNG_DATA                     95
#define DSCOVER_NET_AMOUNT                       96
#define DSCOVER_STLMNT_INSTITN                   98
#define DSCOVER_RECEIVER_ID                      99 

#define DSCOVER_FILE_NAME                        100
#define DSCOVER_ACCOUNT_ID1                      101
#define DSCOVER_ACCOUNT_ID2                      102

#define DSCOVER_TRANS_DESC                       103
#define DSCOVER_CR_REJ_AMOUNT                    104
#define DSCOVER_DB_REJ_AMOUNT                    105
#define DSCOVER_CR_REJ_NUMBER                    106
#define DSCOVER_DB_REJ_NUMBER                    107
#define DSCOVER_CR_FEES_AMOUNT                   108
#define DSCOVER_DB_FEES_AMOUNT                   109
#define DSCOVER_RESERVED_DSCOVER_2                 110
#define DSCOVER_RESERVED_DSCOVER_3                 111
#define DSCOVER_RESERVED_DSCOVER_4                 112
#define DSCOVER_RESERVED_DSCOVER_5                 113
                                               
#define DSCOVER_TRANS_FEE                        115 
#define DSCOVER_STLMNT_FEE                       116
#define DSCOVER_PROCES_FEE                       117

#define DSCOVER_ADTNL_TRACE_DATA                 114
#define DSCOVER_TRIPLE_DES_KEY_DATA              119
#define DSCOVER_RESERV_NATIONAL_USAGE_6          120
#define DSCOVER_RESERV_PRIV_USAGE_1              121
#define DSCOVER_RESERV_PRIV_USAGE_3              123
#define DSCOVER_SUPPORT_INFO                     124
#define DSCOVER_CARD_IDENTIFIER                  125
#define DSCOVER_VERSION_INDICATOR                126
#define DSCOVER_MAC2                             127

#endif 
