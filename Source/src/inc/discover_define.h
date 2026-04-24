#ifndef DSCOVER_DEFINE__H
#define DSCOVER_DEFINE__H




#define  DE26_PIN_LEN   "04"


#define DS_F003_P1_PURCHASE				"00"
#define DS_F003_P1_CASH					"01"
#define DS_F003_P1_CR_ACC_VERIF			"18"
#define DS_F003_P1_PRCH_CASHOVER        "09"
#define DS_F003_P1_MERCH_RETURN			"20"
#define DS_F003_P1_LOAD_TRANSACTION     "28"
#define DS_F003_P1_BALANCE_INQ			"31"
#define DS_F003_PIN_CHNAGE              "98" 
#define DS_F003_PIN_UNBLOCK             "99" 

#define DS_F003_P1_LEN					  2


#define DS_F024_ORIG_REQUEST                     "100"
#define DS_F024_FULL_REVERSAL                    "400"
#define DS_F024_PARTIAL_REVERSAL                 "401"
#define DS_F024_LEN                	      	     3


#define  DS_F022_P1_UNKNOWN                         "00"
#define  DS_F022_P1_MANUAL                          "01"
#define  DS_F022_P1_MAGNETIC_STRIPE                 "02"
#define  DS_F022_P1_BAR_COD                         "03"
#define  DS_F022_P1_OPTICAL_CHARACTER_READER        "04"
#define  DS_F022_P1_INTEGRATED_CIRCUIT_CARD_READER  "05"
#define  DS_F022_P1_ELECTRONIC_COMMERCE             "07"
#define  DS_F022_P1_RADIO_FREQUENCY_MAG             "81"
#define  DS_F022_P1_MOBILE_COMMERCE                 "82"
#define  DS_F022_P1_RADIO_FREQUENCY_CHIP            "83"
#define  DS_F022_P1_CHIP_FALLBACK                   "85"
#define  DS_F022_P1_CONTACTLESS_INTERFACE           "86"
#define  DS_F022_P1_VOICE_AUTHORIZATIONS            "90"
#define  DS_F022_P1_VOICE_RESPONSE_UNIT             "91"




#define DS_F022_P2_UNKNOWN                               '0'
#define DS_F022_P2_PIN_ENTRY_CAPABILITY                  '1'
#define DS_F022_P2_TERMINAL_NO_PIN_CAPABILITY            '2'
#define DS_F022_P2_PIN_PAD_DOWN                          '8'
#define DS_F022_P2_PIN_VERIFIED_BY_TERMINAL              '9'


#define DS_F025_ISSUER_UNAVAILABLE                       "66"
#define DS_F025_CUSTOMER_CANCELLATION                    "00"
#define DS_F025_UNSPECIFIED_NO_ACTION_TAKEN              "01"
#define DS_F025_COMPLETED_PARTIALLY                      "04"
#define DS_F025_RESPONSE_RECEIVED_TOO_LATE               "06"
#define DS_F025_POS_UNABLE_TO_COMPLETE_TRANSACTION       "07"
#define DS_F025_TRACK_3_NOT_UPDATED                      "16"
#define DS_F025_SUSPECTED_NO_CASH_DISPENSED              "17"
#define DS_F025_TIMED_OUT_NO_CASH_DISPENSED              "18"
#define DS_F025_TIMED_OUT_CARD_RETAINED_NO_CASH          "19"
#define DS_F025_INVALID_RESPONSE_NO_ACTION_TAKEN         "20"
#define DS_F025_TIMED_OUT_WAITING_RESPONSE               "21"
#define DS_F025_UNABLE_TO_DELIVER_MESSAGE                "13"
#define DS_F025_MALFUNCTION_CARD_RETAINED                "14"
#define DS_F025_SUSPECTED_MALFUNCTION                    "02"
#define DS_F025_LEN                	      	              2


#define  DS_F061_P1_ATTENDED_TERMINAL                 '0'
#define  DS_F061_P1_UNATTENDED_TERMINAL               '1'
#define  DS_F061_P1_NO_TERMINAL_USED                  '2'
#define  DS_F061_P1_UNKNOWN                           '9'


#define  DS_F061_P2_PARTIAL_APPROVAL_NOT_SUPPORTED    '0'
#define  DS_F061_P2_PARTIAL_APPROVAL_SUPPORTED        '2'

#define  DS_F061_P3_ON_PREMISES_OF_MERCHANT_FACILITY     '0'
#define  DS_F061_P3_OFF_PREMISES_OF_MERCHANT_FACILITY    '1'
#define  DS_F061_P3_ON_PREMISES_OF_CARDHOLDER            '2'
#define  DS_F061_P3_NO_TERMINAL_USED                     '3'
#define  DS_F061_P3_UNKNOWN                              '9'


#define  DS_F061_P4_CARDHOLDER_PRESENT                   '0'
#define  DS_F061_P4_CARDHOLDER_NOT_PRESENT               '1'
#define  DS_F061_P4_CARDHOLDER_NOT_PRESENT_MAIL          '2'
#define  DS_F061_P4_CARDHOLDER_NOT_PRESENT_TELEPHONE     '3'
#define  DS_F061_P4_CARDHOLDER_NP_STAND_OR_RECUR         '4'
#define  DS_F061_P4_ELECTRONIC_ORDER                     '5'
#define  DS_F061_P4_UNKNOWN                              '9'






#define  DS_F061_P5_CARD_PRESENT          '0'
#define  DS_F061_P5_CARD_NOT_PRESENT      '1'
#define  DS_F061_P5_UNKNOWN               '9'


#define  DS_F061_P6_TERM_NO_CARD_CAPTURE_CAPABILITY     '0'
#define  DS_F061_P6_TERM_CARD_CAPTURE_CAPABILITY        '1'
#define  DS_F061_P6_UNKNOWN                             '9'

#define  DS_F061_P7_NORMAL_REQUEST                       '0'
#define  DS_F061_P7_PREAUTHORIZED_REQUEST                '4'
#define  DS_F061_P7_PARTIAL_SPLIT_SHIPMENT               'P'
#define  DS_F061_P7_RECURRING_PAYMENT                    'R'
#define  DS_F061_P7_RE_AUTHORIZE_FOR_FULL_AMOUNT         'A'




#define  DS_F061_P8_NO_SECURITY_CONCERN                            '0'
#define  DS_F061_P8_SUSPECTED_FRAUD                                '1'
#define  DS_F061_P8_ID_VERIFIED                                    '2'
#define  DS_F061_P8_UNKNOWN                                        '9'


                                                                
                                                                
                                                                
#define  DS_F061_P9_UNKNOWN                                        '0'
#define  DS_F061_P9_TRANSACTION_NOT_E_COMMERCE                     '1'
#define  DS_F061_P9_IN_APP_AUTHENTICATION                          '4'
#define  DS_F061_P9_CARDHOLDER_AUTHENTICATED                       '5'
#define  DS_F061_P9_MERCHANT_ATTEMPTED_AUTHENTICATE                '6'
#define  DS_F061_P9_E_COMMERCE_CHANNEL_ENCRYPTION                  '7'
#define  DS_F061_P9_E_COMMERCE_WITHOUT_DATA_PROTECTION             '8'
#define  DS_F061_P9_RESERVED                                       '9'
                                                                 
                                                             
                                                                 
#define  DS_F061_P10_MOBILE_POS                                    'M'
#define  DS_F061_P10_UNSPECIFIED                                   '0'
#define  DS_F061_P10_UNKNOWN                                       '9'
                                                                  
                                                                  
#define  DS_F061_P11_UNSPECIFIED                                   '0'
#define  DS_F061_P11_NO_TERMINAL                                   '1'
#define  DS_F061_P11_MAGNETIC_STRIPE_READER                        '2'
#define  DS_F061_P11_BAR_CODE_                                     '3'
#define  DS_F061_P11_OPTICAL_CHARACTER_RECOGNITION                 '4'
#define  DS_F061_P11_ICC_READER                '5'
#define  DS_F061_P11_KEY_ENTRY_ONLY                                '6'
#define  DS_F061_P11_MAGNETIC_STRIPE                               '7'
#define  DS_F061_P11_RFID_CHIP                                     'C'
#define  DS_F061_P11_HYBRID_ICC_CONTACTLESS                        'H'
#define  DS_F061_P11_RFID_MAGNETIC_STRIPE                          'R'
#define  DS_F061_P11_SECURE_ELECTRONIC_TRANSACTION                 'S'
#define  DS_F061_P11_SET_WITHOUT_CERTIFICATE                       'T'
#define  DS_F061_P11_CHANNEL_ENCRYPTED_E_COMMERCE                  'U'
#define  DS_F061_P11_NON_SECURE_E_COMMERCE_TRANS                   'V'


#define  DS_F061_P12_RESERVED  "00"


#define  DE70_REQUESTING_STATUS   "061"

#endif
