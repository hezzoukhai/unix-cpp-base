#ifndef POSISO_DEFINE__H
#define POSISO_DEFINE__H

#define POSISO_F002_MAX_LEN					19
#define POSISO_F003_P1_LEN					2
#define POSISO_F003_P2_LEN					2
#define POSISO_F003_P3_LEN					2
#define POSISO_F003_LEN						6
#define POSISO_F004_LEN						12
#define POSISO_F005_LEN						12
#define POSISO_F007_LEN						10
#define POSISO_F011_LEN						6
#define POSISO_F012_LEN						12
#define POSISO_F014_LEN						4
#define POSISO_F015_LEN						6
#define POSISO_F017_LEN						4
#define POSISO_F022_LEN						12
#define POSISO_F023_LEN						3
#define POSISO_F024_LEN						3
#define POSISO_F025_LEN						4
#define POSISO_F026_LEN						4
#define POSISO_F032_MAX_LEN					11
#define POSISO_F034_MAX_LEN					28
#define POSISO_F035_MAX_LEN					37
#define POSISO_F037_LEN						12
#define POSISO_F038_LEN						6
#define POSISO_F039_LEN						3
#define POSISO_F041_LEN						8
#define POSISO_F042_LEN						15
#define POSISO_F043_MAX_LEN					99
#define POSISO_F044_MAX_LEN					999
#define POSISO_F048_MAX_LEN					999
#define POSISO_F049_LEN						3
#define POSISO_F050_LEN						3
#define POSISO_F052_LEN						16
#define POSISO_F053_MAX_LEN					99
#define POSISO_F054_MAX_LEN					120
#define POSISO_F055_MAX_LEN					299
#define POSISO_F056_MAX_LEN					60
#define POSISO_F062_MAX_LEN					999
#define POSISO_F063_MAX_LEN					999
#define POSISO_F064_LEN						8
#define POSISO_F071_LEN						8
#define POSISO_F072_MAX_LEN					999
#define POSISO_F073_LEN						6
#define POSISO_F074_LEN						10
#define POSISO_F075_LEN						10
#define POSISO_F076_LEN						10
#define POSISO_F077_LEN						10
#define POSISO_F083_LEN						10
#define POSISO_F084_LEN						10
#define POSISO_F086_LEN						16
#define POSISO_F087_LEN						16
#define POSISO_F088_LEN						16
#define POSISO_F089_LEN						16
#define POSISO_F097_LEN						17
#define POSISO_F101_MAX_LEN					17
#define POSISO_F110_MAX_LEN					999
#define POSISO_F128_LEN						8


#define POSISO_F024_INIT_REQUEST			"100"			/* Initial request						*/
#define POSISO_F024_PRE_AUTH				"101"			/* Pre authorization					*/
#define POSISO_F024_INIT_FIN_REQ			"200"			/* Initial request						*/
#define POSISO_F024_AUTH_PREV				"201"			/* Authorized previously				*/
#define POSISO_F024_DCC_AUTH				"299"			/*DCC Auth*/

#define POSISO_F024_END_OF_TRANSF			"300"			/* End of transfer						*/
#define POSISO_F024_ADD_RECORD				"301"			/* Add record							*/
#define POSISO_F024_CHG_RECORD				"302"			/* Change record (partial update)		*/
#define POSISO_F024_DEL_RECORD				"303"			/* Delete record						*/
#define POSISO_F024_FULL_CHG_RECORD			"304"			/* Change record (full update)			*/
#define POSISO_F024_REP_FILE				"306"			/* Replace file							*/
#define POSISO_F024_DEL_FILE				"308"			/* Delete file							*/
#define POSISO_F024_NO_ACTION				"309"			/* No Action							*/

#define POSISO_F024_FULL_REVERSAL			"400"			/* Full reversal						*/
#define POSISO_F024_UNDEF_REV				"401"			/* Reversal status undefined,			*/
#define POSISO_F024_PARTIAL_REVERSAL		"402"			/* Partial reversal						*/


#define POSISO_F024_FINAL_SETTL				"500"			/* Final settlement,					*/
#define POSISO_F024_INTER_SETTL				"501"			/* Intermediate settlement,				*/
#define POSISO_F024_CURR_FINAL_SETTL		"502"			/* Final settlement in a specific currency,		*/
#define POSISO_F024_CURR_INTER_SETTL		"503"			/* Intermediate settlement in a specific currency.	*/
#define POSISO_F024_FLEET					"700"			/* Fleet				*/


#define POSISO_F024_OPENING					"801"			/* Opening						*/
#define POSISO_F024_KEY_CHG					"811"			/* Key change							*/
#define POSISO_F024_RECON_REQ				"821"			/* Last Reconciliation request				*/
#define POSISO_F024_INT_RECON_REQ			"822"			/* Intermediary Reconciliation request	*/
#define POSISO_F024_INIT_REQ				"880"			/* Initialization request				*/
#define POSISO_F024_LOAN_INST               "295"            /*Installment Loan*/ /*ZKO24092018*/




/*Start ZKO20092018: FLEET DATA*/
/*
#define POSISO_FLEET_INVOICE_NBR_LEN						10
#define POSISO_FLEET_MILEAGE_LEN							8
#define POSISO_FLEET_PRODUCT_CODE_LEN						2
#define POSISO_FLEET_VHCL_REG_NBR_LEN						17
#define POSISO_FLEET_FUEL_QUANTITY_LEN					10
#define POSISO_FLEET_FUEL_AMNT_LEN						12
#define POSISO_FLEET_OIL_QUANTITY_LEN						10
#define POSISO_FLEET_OIL_AMNT_LEN							12
#define POSISO_FLEET_TYRES_QUANTITY_LEN					10
#define POSISO_FLEET_TYRES_AMNT_LEN						12
#define POSISO_FLEET_BATTERIES_AMNT_LEN					12
#define POSISO_FLEET_SUNDRY_AMNT_LEN						12
#define POSISO_FLEET_GARAGE_AMNT_LEN						12



#define POSISO_FLEET_INVOICE_NBR_POS						0
#define POSISO_FLEET_MILEAGE_POS							10
#define POSISO_FLEET_PRODUCT_CODE_POS						18
#define POSISO_FLEET_VHCL_REG_NBR_POS						20
#define POSISO_FLEET_FUEL_QUANTITY_POS					37
#define POSISO_FLEET_FUEL_AMNT_POS						47
#define POSISO_FLEET_OIL_QUANTITY_POS						59
#define POSISO_FLEET_OIL_AMNT_POS							69
#define POSISO_FLEET_TYRES_QUANTITY_POS					81
#define POSISO_FLEET_TYRES_AMNT_POS						91
#define POSISO_FLEET_BATTERI_AMNT_POS						103
#define POSISO_FLEET_SUNDRY_AMNT_POS						115
#define POSISO_FLEET_GARAGE_AMNT_POS						127
*/
/*End ZKO20092018*/

#define POSISO_FLEET_PURCHASE_TYPE_LEN                      1
#define POSISO_FLEET_SERVICE_TYPE_LEN                       1
#define POSISO_FLEET_FUEL_TYPE_LEN                          2
#define POSISO_FLEET_UNIT_MEASURE_LEN                       1
#define POSISO_FLEET_QUANTITY_LEN                           12
#define POSISO_FLEET_UNIT_COST_LEN                          12
#define POSISO_FLEET_GROSS_FUEL_PRICE_LEN                   12
#define POSISO_FLEET_NET_FUEL_PRICE_LEN                     12
#define POSISO_FLEET_GROSS_NON_FUEL_PRICE_LEN               12
#define POSISO_FLEET_NET_NON_FUEL_PRICE_LEN                 12
#define POSISO_FLEET_ODOMETER_LEN                           18
#define POSISO_FLEET_VAT_TAX_RATE_LEN                       4
#define POSISO_FLEET_MISC_FUEL_TAX_EX_STATUS_LEN            1
#define POSISO_FLEET_MISC_FUEL_TAX_LEN                      12
#define POSISO_FLEET_MISC_NON_FUEL_TAX_EX_STATUS_LEN        1
#define POSISO_FLEET_MISC_NON_FUEL_TAX_LEN                  12
#define POSISO_FLEET_LOCAL_TAX_INCLUDED_LEN                 1
#define POSISO_FLEET_LOCAL_TAX_LEN                          12
#define POSISO_FLEET_NATIONAL_TAX_INCLUDED_LEN              1
#define POSISO_FLEET_NATIONAL_TAX_LEN                       12
#define POSISO_FLEET_OTHER_TAX_LEN                          12
#define POSISO_FLEET_MER_VAT_REGISTRATION_LEN               20
#define POSISO_FLEET_CUSTOMER_VAT_REG_NUMBER_LEN            13
#define POSISO_FLEET_CUSTOMER_REFERENCE_NUMBER_LEN          17
#define POSISO_FLEET_MESSAGE_IDENTIFIER_LEN                 15
#define POSISO_FLEET_ADDITIONAL_DATA_INDICATOR_LEN          1
#define POSISO_FLEET_SUMMARY_COMMODITY_CODE_LEN             4
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_1_LEN            2
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_2_LEN            2
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_3_LEN            2
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_4_LEN            2
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_5_LEN            2
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_6_LEN            2
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_7_LEN            2
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_8_LEN            2
#define POSISO_FLEET_FUEL_BRAND_LEN                         4
#define POSISO_FLEET_FUEL_TRANS_VAL_RESULTS_LEN             5
#define POSISO_FLEET_FUEL_ACCEPTANCE_MODE_LEN               1
#define POSISO_FLEET_DRIVER_ID_LEN                          20
#define POSISO_FLEET_JOB_NUMBER_LEN                         10
#define POSISO_FLEET_FLEET_NUMBER_LEN                       8
#define POSISO_FLEET_VRN_LEN                                20
#define POSISO_FLEET_PRODUCT_QUALIFIER_LEN                  6
#define POSISO_FLEET_EXPANDED_FUEL_TYPE_LEN                 4

#define POSISO_FLEET_PURCHASE_TYPE_POS                      1
#define POSISO_FLEET_SERVICE_TYPE_POS                       2
#define POSISO_FLEET_FUEL_TYPE_POS                          4
#define POSISO_FLEET_UNIT_MEASURE_POS                       5
#define POSISO_FLEET_QUANTITY_POS                           17
#define POSISO_FLEET_UNIT_COST_POS                          29
#define POSISO_FLEET_GROSS_FUEL_PRICE_POS                   41
#define POSISO_FLEET_NET_FUEL_PRICE_POS                     53
#define POSISO_FLEET_GROSS_NON_FUEL_PRICE_POS               65
#define POSISO_FLEET_NET_NON_FUEL_PRICE_POS                 77
#define POSISO_FLEET_ODOMETER_POS                           95
#define POSISO_FLEET_VAT_TAX_RATE_POS                       99
#define POSISO_FLEET_MISC_FUEL_TAX_EX_STATUS_POS            100
#define POSISO_FLEET_MISC_FUEL_TAX_POS                      112
#define POSISO_FLEET_MISC_NON_FUEL_TAX_EX_STATUS_POS        113
#define POSISO_FLEET_MISC_NON_FUEL_TAX_POS                  125
#define POSISO_FLEET_LOCAL_TAX_INCLUDED_POS                 126
#define POSISO_FLEET_LOCAL_TAX_POS                          138
#define POSISO_FLEET_NATIONAL_TAX_INCLUDED_POS              139
#define POSISO_FLEET_NATIONAL_TAX_POS                       151
#define POSISO_FLEET_OTHER_TAX_POS                          163
#define POSISO_FLEET_MER_VAT_REGISTRATION_POS               183
#define POSISO_FLEET_CUSTOMER_VAT_REG_NUMBER_POS            196
#define POSISO_FLEET_CUSTOMER_REFERENCE_NUMBER_POS          213
#define POSISO_FLEET_MESSAGE_IDENTIFIER_POS                 228
#define POSISO_FLEET_ADDITIONAL_DATA_INDICATOR_POS          229
#define POSISO_FLEET_SUMMARY_COMMODITY_CODE_POS             233
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_1_POS            235
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_2_POS            237
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_3_POS            239
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_4_POS            241
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_5_POS            243
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_6_POS            245
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_7_POS            247
#define POSISO_FLEET_NON_FUEL_PRODUCT_CODE_8_POS            249
#define POSISO_FLEET_FUEL_BRAND_POS                         253
#define POSISO_FLEET_FUEL_TRANS_VAL_RESULTS_POS             258
#define POSISO_FLEET_FUEL_ACCEPTANCE_MODE_POS               259
#define POSISO_FLEET_DRIVER_ID_POS                          279
#define POSISO_FLEET_JOB_NUMBER_POS                         289
#define POSISO_FLEET_FLEET_NUMBER_POS                       297
#define POSISO_FLEET_VRN_POS                                317
#define POSISO_FLEET_PRODUCT_QUALIFIER_POS                  323
#define POSISO_FLEET_EXPANDED_FUEL_TYPE_POS                 327



/*Start ZKO 25092018 : LOAN DATA*/


#define POSISO_LOAN_MNTLY_AMNT_LEN						12
#define POSISO_LOAN_RATE_LEN							8
#define POSISO_LOAN_RPYMNT_DATE_LEN					8
#define POSISO_LOAN_NBR_INSTALMNT_LEN					3



#define POSISO_LOAN_MNTLY_AMNT_POS						0
#define POSISO_LOAN_RATE_POS							12
#define POSISO_LOAN_RPYMNT_DATE_POS						20
#define POSISO_LOAN_NBR_INSTALMNT_POS					28


/*End ZKO 25092018*/




/* Files */

#define POSISO_FILE_KEY                "KEY"
#define POSISO_FILE_STOP_LIST          "STOP LIST"
#define POSISO_FILE_MAC_KEY            "MAC KEY"
#define POSISO_FILE_EMV_AID            "EMV_AID"
#define POSISO_FILE_EMV_KEYS           "EMV_KEYS"
#define POSISO_FILE_EMV_TAC            "EMV_TAC"
/*#define POSISO_FILE_CTLS_TAC           "CTLS_TAC"*/
#define POSISO_FILE_EMV_TDOL           "EMV_TDOL"
#define POSISO_FILE_EMV_DDOL           "EMV_DDOL"
#define POSISO_FILE_EMV_RAND_CALL      "EMV_RAND_CALL"
#define POSISO_FILE_EMV_TCC            "EMV_TCC"
#define POSISO_FILE_POS_PARAMETER      "POS PARAMETER"
#define POSISO_FILE_TRANSACTION        "TRANSACTION"
/*#define POSISO_FILE_DCC_RATES          "DCC_RATES"
#define POSISO_FILE_DCC_RATES2         "DCC RATES"
#define POSISO_FILE_DUKPT              "DUKPT"
*/
#define POSISO_FILE_PREFIXES			"PREFIXES"
#define POSISO_FILE_BIN_LOC				"BIN_LOCAUX"
/*
#define POSISO_FILE_COMPTE				"COMPTE"
#define POSISO_FILE_DOM					"DOMICILIATION"
*/
#define POSISO_FILE_CONTACTLESS			"CONTACTLESS"
#define POSISO_FILE_PAYWAVE				"PAY_WAVE"


#endif

