#ifndef HPS_POSTILION_FIELDS_H
#define HPS_POSTILION_FIELDS_H

#define POSTILION_BIT_MAP2                           0
#define POSTILION_CARD_NBR                           1
#define POSTILION_PROC_CODE                          2
#define POSTILION_TRANS_AMOUNT                       3
#define POSTILION_STLMNT_AMOUNT                      4
#define POSTILION_BILLING_AMOUNT                     5
#define POSTILION_XMIT_TIME                          6
#define POSTILION_STLMNT_CNVRSN_RATE                 8
#define POSTILION_BILLING_CNVRSN_RATE                9
#define POSTILION_AUDIT_NBR                          10
#define POSTILION_TRANS_TIME                         11
#define POSTILION_TRANS_DATE                         12
#define POSTILION_EXPIRY_DATE                        13
#define POSTILION_STLMNT_DATE                        14
#define POSTILION_CNVRSN_DATE                        15
#define POSTILION_MERCHANT_TYPE                      17
#define POSTILION_ACQR_COUNTRY                       18
#define POSTILION_PAN_COUNTRY                        19
#define POSTILION_FORWD_COUNTRY                      20
#define POSTILION_POS_ENTRY_MODE                     21
#define POSTILION_CARD_SEQUENCE_NBR                  22
#define POSTILION_FUNCTION_CODE                      23
#define POSTILION_POS_CONDITION_CODE                 24
#define POSTILION_POS_PIN_CAPTURE                    25
#define POSTILION_AUTHOR_ID_LENGTH                   26
#define POSTILION_TRANS_FEE                          27
#define POSTILION_STLMNT_FEE                         28
#define POSTILION_TRANS_PORC_FEE                     29
#define POSTILION_STLMNT_PROC_FEE                    30
#define POSTILION_ACQR_ID                            31
#define POSTILION_FORWD_ID                           32
#define POSTILION_PAN_EXTENDED                       33
#define POSTILION_T2_DATA                            34
#define POSTILION_T3_DATA                            35
#define POSTILION_REFERENCE_NBR                      36
#define POSTILION_AUTHOR_ID                          37
#define POSTILION_RESPONSE_CODE                      38
#define POSTILION_SERVICE_CODE                       39
#define POSTILION_TERMINAL_NBR                       40
#define POSTILION_OUTLET_NBR                         41
#define POSTILION_TERMINAL_ADR                       42
#define POSTILION_ADTNL_RESP_DATA                    43
#define POSTILION_T1_DATA                            44
#define POSTILION_FEES_AMOUNT                        45
#define POSTILION_ADTNL_DATA_PRIV                    47
#define POSTILION_TRANS_CRNCY                        48
#define POSTILION_STLMNT_CRNCY                       49
#define POSTILION_BILLING_CRNCY                      50
#define POSTILION_PIN_DATA                           51
#define POSTILION_SECURITY_DATA                      52
#define POSTILION_ADTNL_AMOUNTS                      53
#define POSTILION_MSG_REASON_CODE                    55
#define POSTILION_AUTH_LIFE_CYCLE_CODE               56
#define POSTILION_AUTH_AGENT_INST                    57
#define POSTILION_ECHO_DATA                          58
#define POSTILION_ADTNL_POS_INFO                     59
#define POSTILION_OTHER_AMOUNTS                      60
#define POSTILION_CPS_FIELDS                         61
#define POSTILION_PRIV_USE_FIELDS                    62
#define POSTILION_MAC1                               63

#define POSTILION_STLMNT_CODE                        65
#define POSTILION_EXT_PAYMENT_CODE                   66
#define POSTILION_RECEIVER_COUNTRY                   67
#define POSTILION_STLMNT_INST_COUNTRY                68
#define POSTILION_NW_MNGMT_INFO_CODE                 69
#define POSTILION_RECORDS_DATA                       71
#define POSTILION_ACTION_DATE                        72

#define POSTILION_CR_NBR                             73
#define POSTILION_CR_REV_NBR                         74
#define POSTILION_DB_NBR                             75
#define POSTILION_DB_REV_NBR                         76
#define POSTILION_TRANSFER_NBR                       77
#define POSTILION_TRANSFER_REV_NBR                   78
#define POSTILION_INQUIRIES_NBR                      79
#define POSTILION_AUTHORIZATIONS_NBR                 80

#define POSTILION_CREDITS_FEE_AMOUNT                 81 //AHA AHA 02082022
#define POSTILION_DEBITS_FEE_AMOUNT                  82 //AHA AHA 02082022

#define POSTILION_DEPOSIT_AUT_NBR                    83
#define POSTILION_DEPOSIT_REV_NBR                    84

#define POSTILION_CR_AMOUNT                          85
#define POSTILION_CR_REV_AMOUNT                      86
#define POSTILION_DB_AMOUNT                          87
#define POSTILION_DB_REV_AMOUNT                      88
#define POSTILION_ORIG_DATA                          89

#define POSTILION_FILE_UPDATE                        90
#define POSTILION_INITIATOR_ORGANISM                 93
#define POSTILION_REPLACEMENT_AMOUNT                 94
#define POSTILION_MSG_SECURITY                       95
#define POSTILION_NET_AMOUNT                         96
#define POSTILION_PAYEE                              97
#define POSTILION_STLMNT_INSTITN                     98

#define POSTILION_RECEIVER_ID                        99
#define POSTILION_FILE_NAME                          100
#define POSTILION_ACCOUNT_ID1                        101
#define POSTILION_ACCOUNT_ID2                        102

#define POSTILION_INTERN_USE                         105

#define POSTILION_ADTNL_TRACE_DATA                   114
#define POSTILION_PAYMENTS_NBR                       117
#define POSTILION_PAYMENTS_REV_NBR                   118
#define POSTILION_ORIG_MSG_TYPE                      119

#define POSTILION_POS_DATA_CODE                      122
#define POSTILION_NW_MNG_INFO                        124
#define POSTILION_PRIV_USE_FIELD                     125
#define POSTILION_PRIVATE_FIELD                      126
#define POSTILION_MAC2                               127

/* SMART CARD DATA */

#define POSTILION_TERM_CAPBLTY_PROFILE               129
#define POSTILION_TERM_VERIF_RESULTS                 130
#define POSTILION_UNPREDICT_NBR                      131
#define POSTILION_TERM_SERIAL_NBR                    132
#define POSTILION_POSTILION_DISCRET_DATA             133
#define POSTILION_ISSUER_DISCRET_DATA                134
#define POSTILION_CRYPTOGRAM                         135
#define POSTILION_APP_TRANS_COUNTER                  136
#define POSTILION_APP_INTCHG_PROFILE                 137
#define POSTILION_ARPC_RESP_CODE                     138
#define POSTILION_ISSUER_SCRIPT                      141
#define POSTILION_ISSUER_SCRIPT_RESULTS              142
#define POSTILION_CRYPTO_TRANS_TYPE                  143
#define POSTILION_TERM_COUNTRY_CODE                  144
#define POSTILION_TERM_TRANS_DATE                    145
#define POSTILION_CRYPTO_AMOUNT                      146
#define POSTILION_CRYPTO_CRNCY_CODE                  147
#define POSTILION_CRYPTO_CBACK_AMOUNT                148



/* CPS fields */
#define CPS_ACI                         0
#define CPS_TRANS_ID                    1



/* PUF: Privat Use Fields */
#define PUF_NETWORK_ID                  0
#define PUF_MSG_REASON                  2
#define PUF_STIP_REASON                 3
#define PUF_PLUS_PMC                    4
#define PUF_NETWORK_FLAGS               6

/* Field 127 Mng **/
#define POSTILION_F127_01                        0
#define POSTILION_F127_02                        1
#define POSTILION_F127_03_ROUT_INFO              2
#define POSTILION_F127_04_POS_DATA               3
#define POSTILION_F127_05_SERVICE_STATION_DATA   4
#define POSTILION_F127_06_AUTH_PROFILE           5
#define POSTILION_F127_07_CHECK_DATA             6
#define POSTILION_F127_08_RETENTION_DATA         7
#define POSTILION_F127_09_ADD_NODE_DATA          8
#define POSTILION_F127_10_CVV2                   9
#define POSTILION_F127_11                       10
#define POSTILION_F127_12_TERM_OWNER            11
#define POSTILION_F127_13_POS_GEO_DATA          12
#define POSTILION_F127_14_SPONSOR_BANK          13
#define POSTILION_F127_15_ADR_VERIF_DATA        14
#define POSTILION_F127_16_ADR_VERIF_RES         15
#define POSTILION_F127_17_CARDHOLDER_INF        16
#define POSTILION_F127_18_VALIDATION_DATA       17
#define POSTILION_F127_19_PAYEE_BANK_ACC        18
#define POSTILION_F127_20_AUTHZER_DATE_SETT     19
#define POSTILION_F127_21_RECORD_ID             20
#define POSTILION_F127_22_STRUCTURED_DATA       21
#define POSTILION_F127_23_PAYEE_NAME_AND_ADR    22
#define POSTILION_F127_24_PAYER_ACCOUNT         23
#define POSTILION_F127_25_ICC_DATA              24
#define POSTILION_F127_26_ORIGINAL_NODE         25
#define POSTILION_F127_27_CARD_VERIF_RESULT     26
#define POSTILION_F127_28_AMEX_CID              27
#define POSTILION_F127_29_3D_SECURE_DATA        28
#define POSTILION_F127_30_3D_SECURE_RESULT      29
#define POSTILION_F127_31    			              30 
#define POSTILION_F127_32     			             31 

/* EMV Field  **/
#define POSTILION_EMV_01                                                    0
#define POSTILION_EMV_02_AMOUNT_AUTHORIZED                                  1
#define POSTILION_EMV_03_AMOUNT_OTHER                                       2
#define POSTILION_EMV_04_APPLICATION_IDENTIFIER                             3
#define POSTILION_EMV_05_APPLICATION_INTERCHANGE_PROFILE                    4
#define POSTILION_EMV_06_APPLICATION_TRANSACTION_COUNTER                    5
#define POSTILION_EMV_07_APPLICATION_USAGE_CONTROL                          6
#define POSTILION_EMV_08_AUTHORIZATION_RESPONSE_CODE                        7
#define POSTILION_EMV_09_CARD_AUTHENTICATION_RELIABILITY_INDICATOR          8
#define POSTILION_EMV_10_CARD_AUTHENTICATION_RESULTS_CODE                   9
#define POSTILION_EMV_11_CHIP_CONDITION_CODE                                10
#define POSTILION_EMV_12_CRYPTOGRAM                                         11
#define POSTILION_EMV_13_CRYPTOGRAM_INFORMATION_DATA                        12
#define POSTILION_EMV_14_CVM_LIST                                           13
#define POSTILION_EMV_15_CVM_RESULTS                                        14
#define POSTILION_EMV_16_INTERFACE_DEVICE_SERIAL_NUMBER                     15
#define POSTILION_EMV_17_ISSUER_ACTION_CODE                                 16
#define POSTILION_EMV_18_ISSUER_APPLICATION_DATA                            17
#define POSTILION_EMV_19_ISSUER_SCRIPT_RESULTS                              18
#define POSTILION_EMV_20_TERMINAL_APPLICATION_VERSION_NUMBER                19
#define POSTILION_EMV_21_TERMINAL_CAPABILITIES                              20
#define POSTILION_EMV_22_TERMINAL_COUNTRY_CODE                              21
#define POSTILION_EMV_23_TERMINAL_TYPE                                      22
#define POSTILION_EMV_24_TERMINAL_VERIFICATION_RESULT                       23
#define POSTILION_EMV_25_TRANSACTION_CATEGORY_CODE                          24
#define POSTILION_EMV_26_TRANSACTION_CURRENCY_CODE                          25
#define POSTILION_EMV_27_TRANSACTION_DATE                                   26
#define POSTILION_EMV_28_TRANSACTION_SEQUENCE_COUNTER                       27
#define POSTILION_EMV_29_TRANSACTION_TYPE                                   28
#define POSTILION_EMV_30_UNPREDICTABLE_NUMBER                               29
#define POSTILION_EMV_31_ISSUER_AUTHENTICATION_DATA  			                  30 
#define POSTILION_EMV_32_ISSUER_SCRIPT_TEMPLATE_1    			                  31
#define POSTILION_EMV_33_ISSUER_SCRIPT_TEMPLATE_2                           32


#endif /** HPS_POSTILION_FIELDS_H **/

