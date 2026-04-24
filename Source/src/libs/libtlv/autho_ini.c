#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tlv_autho.h>
#include <define.h>


tlv_info_t  tab_TlvAuthoInfo[] = 
{
   { "002", AUTHO_TLV_VAR, 0 }, /* PAN (Primary Account Number)              */
   { "003", AUTHO_TLV_VAR, 0 }, /* Processing Code                           */
   { "004", AUTHO_TLV_VAR, 0 }, /* Amount, Transaction                       */
   { "005", AUTHO_TLV_VAR, 0 }, /* Amount, Settlement                        */
   { "006", AUTHO_TLV_VAR, 0 }, /* Amount, Cardholder Billing                */
   { "007", AUTHO_TLV_VAR, 0 }, /* Transmission Date and Time                */
   { "008", AUTHO_TLV_VAR, 0 }, /* Amount, Cardholder Billing Fee            */
   { "009", AUTHO_TLV_VAR, 0 }, /* Conversion Rate, Settlement               */
   { "010", AUTHO_TLV_VAR, 0 }, /* Conversion Rate, Cardholder Billing       */
   { "011", AUTHO_TLV_VAR, 0 }, /* Systems Trace Audit Number (Internal Stan)*/
   { "012", AUTHO_TLV_VAR, 0 }, /* Date & Time, Local Transaction            */
   { "013", AUTHO_TLV_VAR, 0 }, /* Effective Date                            */
   { "014", AUTHO_TLV_VAR, 0 }, /* Expiry Date                               */
   { "015", AUTHO_TLV_VAR, 0 }, /* Date, Settlement                          */
   { "016", AUTHO_TLV_VAR, 0 }, /* Date, Conversion                          */
   { "017", AUTHO_TLV_VAR, 0 }, /* Date, Capture                             */
   { "018", AUTHO_TLV_VAR, 0 }, /* Merchant's Type                           */
   { "019", AUTHO_TLV_VAR, 0 }, /* Acquiring Institution Country Code        */
   { "020", AUTHO_TLV_VAR, 0 }, /* PAN Extended, Country Code                */
   { "021", AUTHO_TLV_VAR, 0 }, /* Forwarding Institution Country Code       */
   { "022", AUTHO_TLV_VAR, 0 }, /* POS DATA                                  */
   { "023", AUTHO_TLV_VAR, 0 }, /* Card Sequence Number                      */
   { "024", AUTHO_TLV_VAR, 0 }, /* Function Code                             */
   { "025", AUTHO_TLV_VAR, 0 }, /* Message Reason Code                       */
   { "026", AUTHO_TLV_VAR, 0 }, /* Merchant Activity Code                    */
   { "027", AUTHO_TLV_VAR, 0 }, /* Authorization Idntification Response Lengt*/
   { "028", AUTHO_TLV_VAR, 0 }, /* Reconciliation Date                       */
   { "029", AUTHO_TLV_VAR, 0 }, /* Reconciliation Indicator                  */
   { "030", AUTHO_TLV_VAR, 0 }, /* Initial Amounts                           */
   { "031", AUTHO_TLV_VAR, 0 }, /* Acquirer Reference Data                   */
   { "032", AUTHO_TLV_VAR, 0 }, /* Acquiring Institution Identification Code */
   { "033", AUTHO_TLV_VAR, 0 }, /* Forwarding Institution Identification Code*/
   { "034", AUTHO_TLV_VAR, 0 }, /* PAN, EXtended                             */
   { "035", AUTHO_TLV_VAR, 0 }, /* Track 2 Data                              */
   { "036", AUTHO_TLV_VAR, 0 }, /* Track 3 Data                              */
   { "037", AUTHO_TLV_VAR, 0 }, /* Retrieval Reference Number                */
   { "038", AUTHO_TLV_VAR, 0 }, /* Autorization Identification Response      */
   { "039", AUTHO_TLV_VAR, 0 }, /* Response Code                             */
   { "040", AUTHO_TLV_VAR, 0 }, /* Service Code                              */
   { "041", AUTHO_TLV_VAR, 0 }, /* Card Acceptor Terminal Identification     */
   { "042", AUTHO_TLV_VAR, 0 }, /* Card Acceptor Identification Code         */
   { "043", AUTHO_TLV_VAR, 0 }, /* Card Acceptor Name/Location               */
   { "044", AUTHO_TLV_VAR, 0 }, /* Additional Response Data                  */
   { "045", AUTHO_TLV_VAR, 0 }, /* Track 1 Data                              */
   { "046", AUTHO_TLV_VAR, 0 }, /* Amount Fee                                */
   { "047", AUTHO_TLV_VAR, 0 }, /* Additional Data - National                */
   { "048", AUTHO_TLV_VAR, 0 }, /* Additional Data - Private                 */
   { "049", AUTHO_TLV_VAR, 0 }, /* Currency Code, Transaction                */
   { "050", AUTHO_TLV_VAR, 0 }, /* Currency Code, Settlement                 */
   { "051", AUTHO_TLV_VAR, 0 }, /* Currency Code, Cardholder Billing         */
   { "052", AUTHO_TLV_VAR, 0 }, /* PIN Data                                  */
   { "053", AUTHO_TLV_VAR, 0 }, /* Security Related Control Information      */
   { "054", AUTHO_TLV_VAR, 0 }, /* Additional Amounts                        */
   { "055", AUTHO_TLV_VAR, 0 }, /* ICC DATA                                  */
   { "056", AUTHO_TLV_VAR, 0 }, /* Original Data                             */
   { "057", AUTHO_TLV_VAR, 0 }, /* Authorization Cycle Code                  */
   { "058", AUTHO_TLV_VAR, 0 }, /* Authorization Institution Id              */
   { "059", AUTHO_TLV_VAR, 0 }, /* Transport Data                            */
   { "060", AUTHO_TLV_VAR, 0 }, /* RFU                                       */
   { "061", AUTHO_TLV_VAR, 0 }, /* RFU                                       */
   { "062", AUTHO_TLV_VAR, 0 }, /* Reserved Private                          */
   { "063", AUTHO_TLV_VAR, 0 }, /* Debit Private-Use Fields                  */
   { "064", AUTHO_TLV_VAR, 0 }, /* Message Authentication Code               */
   { "065", AUTHO_TLV_VAR, 0 }, /* Bit Map, Extended                         */
   { "066", AUTHO_TLV_VAR, 0 }, /* Original Amount Fee                       */
   { "067", AUTHO_TLV_VAR, 0 }, /* Extended Payment Code                     */
   { "068", AUTHO_TLV_VAR, 0 }, /* Receiving Institution Country Code        */
   { "069", AUTHO_TLV_VAR, 0 }, /* Settlement Institution Country Code       */
   { "070", AUTHO_TLV_VAR, 0 }, /* Authorisation Institution Code            */
   { "071", AUTHO_TLV_VAR, 0 }, /* Message Number                            */
   { "072", AUTHO_TLV_VAR, 0 }, /* Record Data                               */
   { "073", AUTHO_TLV_VAR, 0 }, /* Date, Action                              */
   { "074", AUTHO_TLV_VAR, 0 }, /* Credits, Number                           */
   { "075", AUTHO_TLV_VAR, 0 }, /* Credits, Reversal Number                  */
   { "076", AUTHO_TLV_VAR, 0 }, /* Debits, Number                            */
   { "077", AUTHO_TLV_VAR, 0 }, /* Debits, Reversal Number                   */
   { "078", AUTHO_TLV_VAR, 0 }, /* Transfer, Number                          */
   { "079", AUTHO_TLV_VAR, 0 }, /* Transfer, Reversal Number                 */
   { "080", AUTHO_TLV_VAR, 0 }, /* Inquiries, Number                         */
   { "081", AUTHO_TLV_VAR, 0 }, /* Autorizations, Number                     */
   { "082", AUTHO_TLV_VAR, 0 }, /* Inquiries, Reversal Number                */
   { "083", AUTHO_TLV_VAR, 0 }, /* Payment, Number                           */
   { "084", AUTHO_TLV_VAR, 0 }, /* Payment, Reversal Number                  */
   { "085", AUTHO_TLV_VAR, 0 }, /* Fees, Number                              */
   { "086", AUTHO_TLV_VAR, 0 }, /* Credits, Amount                           */
   { "087", AUTHO_TLV_VAR, 0 }, /* Credits, Reversal Amount                  */
   { "088", AUTHO_TLV_VAR, 0 }, /* Debits, Amount                            */
   { "089", AUTHO_TLV_VAR, 0 }, /* Debits, Reversal Amount                   */
   { "090", AUTHO_TLV_VAR, 0 }, /* Authorization, Reversal Number            */
   { "091", AUTHO_TLV_VAR, 0 }, /* Receiving Institution Country Code        */
   { "092", AUTHO_TLV_VAR, 0 }, /* Initiator  Institution Country Code       */
   { "093", AUTHO_TLV_VAR, 0 }, /* Receiving Institution Id                  */
   { "094", AUTHO_TLV_VAR, 0 }, /* Initiator  Institution Id                 */
   { "095", AUTHO_TLV_VAR, 0 }, /* Issuer Reference Data                     */
   { "096", AUTHO_TLV_VAR, 0 }, /* Message Security Code                     */
   { "097", AUTHO_TLV_VAR, 0 }, /* Amount, Net Settlement                    */
   { "098", AUTHO_TLV_VAR, 0 }, /* Payee                                     */
   { "099", AUTHO_TLV_VAR, 0 }, /* Settlement Institution Identification Code*/
   { "100", AUTHO_TLV_VAR, 0 }, /* Receiving Institution Identification Code */
   { "101", AUTHO_TLV_VAR, 0 }, /* File Name                                 */
   { "102", AUTHO_TLV_VAR, 0 }, /* Account Identification_1                  */
   { "103", AUTHO_TLV_VAR, 0 }, /* Account Identification_2                  */
   { "104", AUTHO_TLV_VAR, 0 }, /* Transaction Description                   */
   { "105", AUTHO_TLV_VAR, 0 }, /* Credit, Rejects Amount                    */
   { "106", AUTHO_TLV_VAR, 0 }, /* Debit,  Rejects Amount                    */
   { "107", AUTHO_TLV_VAR, 0 }, /* Credit, Rejects Number                    */
   { "108", AUTHO_TLV_VAR, 0 }, /* Debit,  Rejects Number                    */
   { "109", AUTHO_TLV_VAR, 0 }, /* Credit, Fees Amount                       */
   { "110", AUTHO_TLV_VAR, 0 }, /* Credit, Fees Number                       */
   { "111", AUTHO_TLV_VAR, 0 }, /* Reserved ISO                              */
   { "112", AUTHO_TLV_VAR, 0 }, /* Reserved ISO                              */
   { "113", AUTHO_TLV_VAR, 0 }, /* Reserved ISO                              */
   { "114", AUTHO_TLV_VAR, 0 }, /* Reserved ISO                              */
   { "115", AUTHO_TLV_VAR, 0 }, /* Reserved ISO                              */
   { "116", AUTHO_TLV_VAR, 0 }, /* Reserved National                         */
   { "117", AUTHO_TLV_VAR, 0 }, /* Reserved National                         */
   { "118", AUTHO_TLV_VAR, 0 }, /* Reserved National                         */
   { "119", AUTHO_TLV_VAR, 0 }, /* Reserved National                         */
   { "120", AUTHO_TLV_VAR, 0 }, /* Reserved National                         */
   { "121", AUTHO_TLV_VAR, 0 }, /* Reserved National                         */
   { "122", AUTHO_TLV_VAR, 0 }, /* Reserved National                         */
   { "123", AUTHO_TLV_VAR, 0 }, /* Reserved Private                          */
   { "124", AUTHO_TLV_VAR, 0 }, /* Reserved Private                          */
   { "125", AUTHO_TLV_VAR, 0 }, /* Reserved Private                          */
   { "126", AUTHO_TLV_VAR, 0 }, /* Reserved Private                          */
   { "127", AUTHO_TLV_VAR, 0 }, /* Reserved Private                          */
   { "128", AUTHO_TLV_VAR, 0 }, /* MAC2                                      */
   { "129", AUTHO_TLV_VAR, 0 }, /* AUTO REVERSAL STAN                        */
   { "130", AUTHO_TLV_VAR, 0 }, /* USER                                      */
   { "131", AUTHO_TLV_VAR, 0 }, /* INTERNATIONAL CHECK ROUTING               */
   { "132", AUTHO_TLV_VAR, 0 }, /* CONTROL VERIF INDEX                       */
   { "133", AUTHO_TLV_VAR, 0 }, /* PIN RETRY MAX                             */
   { "134", AUTHO_TLV_VAR, 0 }, /* DATA BASE SERVICE CODE                    */
   { "135", AUTHO_TLV_VAR, 0 }, /* AUTO_GET_CARD_DATA_LEVEL                  */
   { "136", AUTHO_TLV_VAR, 0 }, /* AUTO_SCENARIO_CODE                        */
   { "137", AUTHO_TLV_VAR, 0 }, /* AUTO_RECORD_STATUS                        */
   { "138", AUTHO_TLV_VAR, 0 }, /* AUTO_LOYALTY_POINTS_SIGN                  */
   { "139", AUTHO_TLV_VAR, 0 }, /* AUTO_CURRENT_MOUNTH_IND                   */
   { "140", AUTHO_TLV_VAR, 0 }, /* AUTO_OFF_LINE_INDICATOR                   */
   { "141", AUTHO_TLV_VAR, 0 }, /* AUTO_NUMBER_OF_PRODUCT_GROUP              */
   { "142", AUTHO_TLV_VAR, 0 }, /* AUTO_CATALOGUE_CODE                        */
   { "143", AUTHO_TLV_VAR, 0 }, /* AUTO_BAL_CLOSING_DATE                      */
   { "144", AUTHO_TLV_VAR, 0 }, /* CARD ACTIVITY UPDATE FLAG                  */
   { "145", AUTHO_TLV_VAR, 0 }, /* PIN NBR                                    */
   { "146", AUTHO_TLV_VAR, 0 }, /* FEE TRANSACTION AMOUNT                     *//*ENH VISA SWI200904 */
   { "147", AUTHO_TLV_VAR, 0 }, /* FEE Program Indicator                      *//*ENH VISA SWI200904 */
   { "150", AUTHO_TLV_VAR, 0 }, 
   { "196", AUTHO_TLV_VAR, 0 }, /* Security Verification Results              */
   { "199", AUTHO_TLV_VAR, 0 }, 
   { "300"	, AUTHO_TLV_VAR, 0}, /* AUTO_MSG_TYPE                  */
   { "301"	, AUTHO_TLV_VAR, 0}, /* AUTO_ORIG_MSG_TYPE             */
   { "200"	, AUTHO_TLV_VAR, 0}, /* AUTO_ROUTING_CODE              */
   { "201"	, AUTHO_TLV_VAR, 0}, /* AUTO_CAPTURE_CODE              */
   { "202"	, AUTHO_TLV_VAR, 0}, /* AUTO_ORIG_ACTION_CODE          */
   { "203"	, AUTHO_TLV_VAR, 0}, /* AUTO_EVENT_CODE                */
   { "204"	, AUTHO_TLV_VAR, 0}, /* AUTO_NETWORK_CODE              */
   { "205"	, AUTHO_TLV_VAR, 0}, /* AUTO_ISSUING_BANK              */
   { "206"	, AUTHO_TLV_VAR, 0}, /* AUTO_RESP_DATE_TIME            */
   { "207"	, AUTHO_TLV_VAR, 0}, /* AUTO_INTERN_DATE_TIME          */
   { "208"	, AUTHO_TLV_VAR, 0}, /* AUTO_BUSINESS_DATE             */
   { "209"	, AUTHO_TLV_VAR, 0}, /* AUTO_PRODUCT_CODE              */
   { "210"	, AUTHO_TLV_VAR, 0}, /* AUTO_CARD_TYPE                 */
   { "211"	, AUTHO_TLV_VAR, 0}, /* AUTO_VIP_LEVEL                 */
   { "212"	, AUTHO_TLV_VAR, 0}, /* AUTO_DST_ACCOUNT_BANK          */
   { "213"	, AUTHO_TLV_VAR, 0}, /* AUTO_ISS_STLMNT_AMOUNT         */
   { "214"	, AUTHO_TLV_VAR, 0}, /* AUTO_ISS_STLMNT_CURRENCY       */
   { "215"	, AUTHO_TLV_VAR, 0}, /* AUTO_ISS_STLMNT_DATE           */
   { "216"	, AUTHO_TLV_VAR, 0}, /* AUTO_ISS_CNV_RATE_STLMNT       */
   { "217"	, AUTHO_TLV_VAR, 0}, /* AUTO_ISS_CNV_RAT_STL_DATE      */
   { "218"	, AUTHO_TLV_VAR, 0}, /* AUTO_ACQ_STLMNT_AMOUNT         */
   { "219"	, AUTHO_TLV_VAR, 0}, /* AUTO_ACQ_STLMNT_CURRENCY       */
   { "220"	, AUTHO_TLV_VAR, 0}, /* AUTO_ACQ_STLMNT_DATE           */
   { "221"	, AUTHO_TLV_VAR, 0}, /* AUTO_ACQ_CNV_RATE_STLMNT       */
   { "222"	, AUTHO_TLV_VAR, 0}, /* AUTO_ACQ_CNV_RAT_STL_DATE      */
   { "223"	, AUTHO_TLV_VAR, 0}, /* AUTO_ISS_STLMNT_FEE            */
   { "224"	, AUTHO_TLV_VAR, 0}, /* AUTO_ACQ_STLMNT_FEE            */
   { "225"	, AUTHO_TLV_VAR, 0}, /* AUTO_SERV_SETUP_CODE           */
   { "226"	, AUTHO_TLV_VAR, 0}, /* AUTO_PRODUCT_CUR_CODE          */
   { "227"	, AUTHO_TLV_VAR, 0}, /* AUTO_LIMITS_INDEXES            */
   { "228"	, AUTHO_TLV_VAR, 0}, /* AUTO_PERIOD_CODE               */
   { "229"	, AUTHO_TLV_VAR, 0}, /* AUTO_PERIOD_TYPE               */
   { "230"	, AUTHO_TLV_VAR, 0}, /* AUTO_PERIOD_VALUE              */
   { "231"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_CUR_CODE               */
   { "232"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_CREDIT_LIMIT           */
   { "233"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_CASH_LIMIT             */
   { "234"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_LOAN_LIMIT             */
   { "235"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_VIP_LEVEL              */
   { "236"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_CREDIT_CUR_BAL         */
   { "237"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_CASH_CUR_BAL           */
   { "238"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_LOAN_CUR_BAL           */
   { "239"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_RESPONSE_CODE          */
   { "240"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_PEND_AUT_CREDIT        */
   { "241"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_PEND_AUT_CASH          */
   { "242"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_PEND_AUT_LOAN          */
   { "243"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_AVAILABLE_BAL          */
   { "244"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_FIRST_DUE_DATE         */
   { "245"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_INSTALL_AMOUNT         */
   { "246"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_TERM_COUNT             */
   { "247"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_PRODUCT_GRP1           */
   { "248"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_PRODUCT_GRP2           */
   { "249"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_PRODUCT_GRP3           */
   { "250"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_PRODUCT_GRP4           */
   { "251"	, AUTHO_TLV_VAR, 0}, /* AUTO_CR_PRODUCT_GRP5           */
   { "252"	, AUTHO_TLV_VAR, 0}, /* AUTO_LOYALTY_POINTS            */
   { "253"	, AUTHO_TLV_VAR, 0}, /* AUTO_ACQUIRER_BANK             */
   { "254"	, AUTHO_TLV_VAR, 0}, /* AUTO_FORWARD_BANK              */
   { "255"	, AUTHO_TLV_VAR, 0}, /* AUTO_AUTHOR_SRC                */
   { "256"	, AUTHO_TLV_VAR, 0}, /* AUTO_CARD_LEVEL                */
   { "257"	, AUTHO_TLV_VAR, 0}, /* AUTO_BAL_LEVEL                 */
   { "258"	, AUTHO_TLV_VAR, 0}, /* AUTO_DB_CUR_BAL                */
   { "259"	, AUTHO_TLV_VAR, 0}, /* AUTO_LIMIT_BEFORE_TRN          */
   { "260"	, AUTHO_TLV_VAR, 0}, /* AUTO_LIMIT_ID                  */
   { "261"	, AUTHO_TLV_VAR, 0}, /* AUTO_SHADOW_ACT_LEVEL          */
   { "262"	, AUTHO_TLV_VAR, 0}, /* AUTO_HOST_LEVEL                */
   { "263"	, AUTHO_TLV_VAR, 0}, /* AUTO_CLIENT_LEVEL              */
   { "264"	, AUTHO_TLV_VAR, 0}, /* AUTO_EXECPT_LEVEL              */
   { "265"	, AUTHO_TLV_VAR, 0}, /* AUTO_ACTION_TRANSL_LEVEL       */
   { "266"	, AUTHO_TLV_VAR, 0}, /* AUTO_CARD_LIMIT_EXP_LEVEL      */
   { "267"	, AUTHO_TLV_VAR, 0}, /* AUTO_VIP_ACTION_TRL_LEVEL      */
   { "268"	, AUTHO_TLV_VAR, 0}, /* AUTO_AUTHO_PERIOD_LEVEL        */
   { "269"	, AUTHO_TLV_VAR, 0}, /* AUTO_SECURITY_VERIF_FLAGS      */
   { "270"	, AUTHO_TLV_VAR, 0}, /* AUTO_SECURITY_VER_RESULTS      */
   { "271"	, AUTHO_TLV_VAR, 0}, /* AUTO_ADDRESS_VERIF_DATA        */
   { "272"	, AUTHO_TLV_VAR, 0}, /* AUTO_REJECT_CODE               */
   { "273"	, AUTHO_TLV_VAR, 0}, /* AUTO_REJECT_REASON             */
   { "274"	, AUTHO_TLV_VAR, 0}, /* AUTO_TIME_STAMP_PLUS_MSG       */
   { "275"	, AUTHO_TLV_VAR, 0}, /* AUTO_ORIGNE_CODE               */
   { "276"	, AUTHO_TLV_VAR, 0}, /* AUTO_ORIG_TRANS_DATE_TIME      */
   { "277"	, AUTHO_TLV_VAR, 0}, /* AUTO_AUTHO_FLAG                */
   { "278"	, AUTHO_TLV_VAR, 0}, /* AUTO_REVERSAL_FLAG             */
   { "279"	, AUTHO_TLV_VAR, 0}, /* AUTO_TRANSACTION_FLAG          */
   { "280"	, AUTHO_TLV_VAR, 0}, /* AUTO_MATCH_STATUS              */
   { "281"	, AUTHO_TLV_VAR, 0}, /* AUTO_MATCH_DATE                */
   { "282"	, AUTHO_TLV_VAR, 0}, /* AUTO_MATCH_LEVEL               */
   { "283"	, AUTHO_TLV_VAR, 0}, /* AUTO_MATCH_PURGE_DATE          */
   { "284"	, AUTHO_TLV_VAR, 0}, /* AUTO_TRACK_INFO                */
   { "285"	, AUTHO_TLV_VAR, 0}, /* AUTO_ACQ_RESOURCE_CODE         */
   { "286"	, AUTHO_TLV_VAR, 0}, /* AUTO_ISS_RESOURCE_CODE         */
   { "287"	, AUTHO_TLV_VAR, 0}, /* AUTO_TABLE_INDICATOR           */
   { "288"	, AUTHO_TLV_VAR, 0}, /* AUTO_CUR_TABLE_INDICATOR       */
   { "291"	, AUTHO_TLV_VAR, 0}, /* AUTO_REVERSAL_DATE             */
   { "293"	, AUTHO_TLV_VAR, 0}, /* AUTO_CASH_BACK_AMOUNT          */
   { "294"	, AUTHO_TLV_VAR, 0}, /* AUTO_REPLACEMENT_AMOUNT        */
   { "295"	, AUTHO_TLV_VAR, 0}, /* AUTO_REPLACEMENT_BILLING       */
   { "296"	, AUTHO_TLV_VAR, 0}, /* AUTO_TRANSACTION_IDENTIFIER    */
   { "297"	, AUTHO_TLV_VAR, 0}, /* AUTO_AUTHORIZATION_IDENTIFIER   */
   { "298"	, AUTHO_TLV_VAR, 0}, /* AUTO_CARD_PRODUCT_TYPE			*/
   { "299"  , AUTHO_TLV_VAR, 0 }, 
   { "302"	, AUTHO_TLV_VAR, 0}, /* AUTO_REPLACEMENT_FEE */
   { "303"	, AUTHO_TLV_VAR, 0},
   { "304"	, AUTHO_TLV_VAR, 0}, /*AMER20210414 PLUTONL-2991: AUTO_REPLACEMENT_SETTL*/
   { "310"	, AUTHO_TLV_VAR, 0},
   { "311"	, AUTHO_TLV_VAR, 0},
   { "312"	, AUTHO_TLV_VAR, 0},
   { "321"	, AUTHO_TLV_VAR, 0}, /* AUTO_SRC_ACC_ENT_COD */
   { "322"	, AUTHO_TLV_VAR, 0}, /* AUTO_SRC_ACC_ENT_ID */
   { "323"	, AUTHO_TLV_VAR, 0}, /* AUTO_SRC_ACC_SEQ */
   { "324"	, AUTHO_TLV_VAR, 0}, /* AUTO_SRCACC_TYPE */
   { "325"	, AUTHO_TLV_VAR, 0}, /* AUTO_SRC_ACC_ENT_LEVEL */
   { "326"	, AUTHO_TLV_VAR, 0}, /* AUTO_DST_ACC_ENT_COD */
   { "327"	, AUTHO_TLV_VAR, 0}, /* AUTO_DST_ACC_ENT_ID */
   { "328"	, AUTHO_TLV_VAR, 0}, /*  AUTO_DST_ACC_SEQ */
   { "329"	, AUTHO_TLV_VAR, 0}, /* AUTO_DST_ACC_TYPE */
   { "330"	, AUTHO_TLV_VAR, 0}, /* AUTO_DST_ACC_ENT_LEV */
   { "331"	, AUTHO_TLV_VAR, 0}, /* AUTO_UF_ACTION_DATE */
   { "332"	, AUTHO_TLV_VAR, 0}, /* AUTO_UF_UPDATE_CODE */
   { "333"	, AUTHO_TLV_VAR, 0}, /* AUTO_UF_FILENAME */
   { "334"	, AUTHO_TLV_VAR, 0}, /* AUTO_UF_PURGE_DATE */
   { "335"	, AUTHO_TLV_VAR, 0}, /* AUTO_UF_ACTION_CODE */
   { "336"	, AUTHO_TLV_VAR, 0}, /* AUTO_UF_FILE_RECORD */
   /* --- ADDE BY KR POUR ISO 3 --*/
   { "355"	, AUTHO_TLV_VAR, 0}, /* BANK LOCAL AMOUNT */
   { "356"	, AUTHO_TLV_VAR, 0}, /* BANK LOCAL CCY */
   { "360"	, AUTHO_TLV_VAR, 0}, /* DB_CARD_SEQ_NO */
   { "361"	, AUTHO_TLV_VAR, 0}, /* AUTO_ISO3_SMART_PROC_MODE */
   { "362"	, AUTHO_TLV_VAR, 0}, /* AUTO_ROT_MEM_OPTION */
   { "363"	, AUTHO_TLV_VAR, 0}, /* AUTO_ROT_MEM_CTRL_LIMIT */
   { "364"	, AUTHO_TLV_VAR, 0}, /* AUTO_ROT_MEMCTRL_AVAILABLE */
   { "365"	, AUTHO_TLV_VAR, 0}, /* AUTO_ROT_MEMCTRL_AVAILABLE */
   { "366"	, AUTHO_TLV_VAR, 0}, /* AUTO_ROT_MEM_LAST_USAGE_DATE */
   { "367"	, AUTHO_TLV_VAR, 0}, /* AUTO_OFFLINE_ATM_PERIOD */
   { "368"	, AUTHO_TLV_VAR, 0}, /* AUTO_OFF_LINE_ATM_LIMIT_ONUS */
   { "369"	, AUTHO_TLV_VAR, 0}, /* AUTO_OFF_LINE_ATM_LIMIT_OFFUS */
   
   {"370" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_DBSERV_ID	    */				    
   {"371" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_START_TIME		    */				  
   {"372" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_END_TIME		    */				      
   {"373" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_RECONCILIATION_ID   */				
   {"374" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_NB_FIN_AUTHORS  */		
   {"375" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_NB_NFIN_AUTHORS	*/	
   {"376" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_NB_FIN_TRX		*/		    
   {"377" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_NB_PREAUTHS		*/	    
   {"378" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_NB_REVERSALS	*/		    
   {"379" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_NB_SAF_RECORDS	*/	
   {"380" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_NB_SLF_RECORDS	*/	
   {"381" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_TOTAL_FIN_AUTHORS*/	
   {"382" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_TOTAL_FIN_TRX	 */	
   {"383" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_SRC_TOTAL_PREAUTHS	 */
   {"384" 	, AUTHO_TLV_VAR, 0}, /*    AUTO_DST_NB_SAF_SLF	 */

   { "390"	, AUTHO_TLV_VAR, 0},
   { "400"	, AUTHO_TLV_VAR, 0},
   { "401"	, AUTHO_TLV_VAR, 0},
   { "402"	, AUTHO_TLV_VAR, 0},
   { "403"	, AUTHO_TLV_VAR, 0},
   { "404"	, AUTHO_TLV_VAR, 0},
   { "405"	, AUTHO_TLV_VAR, 0},
   { "406"	, AUTHO_TLV_VAR, 0},
   { "407"	, AUTHO_TLV_VAR, 0},
   { "408"	, AUTHO_TLV_VAR, 0},
   { "409"	, AUTHO_TLV_VAR, 0},


   { "410"	, AUTHO_TLV_VAR, 0},
   { "411"	, AUTHO_TLV_VAR, 0},
   { "412"	, AUTHO_TLV_VAR, 0},
   { "413"	, AUTHO_TLV_VAR, 0},
   { "414"	, AUTHO_TLV_VAR, 0},
   { "415"	, AUTHO_TLV_VAR, 0},
   { "416"	, AUTHO_TLV_VAR, 0},
   { "417"	, AUTHO_TLV_VAR, 0},
   { "418"	, AUTHO_TLV_VAR, 0},

   { "420"	, AUTHO_TLV_VAR, 0},
   { "421"	, AUTHO_TLV_VAR, 0},
   { "422"	, AUTHO_TLV_VAR, 0},
   { "423"	, AUTHO_TLV_VAR, 0},
   { "424"	, AUTHO_TLV_VAR, 0},
   { "425"	, AUTHO_TLV_VAR, 0},
   { "426"	, AUTHO_TLV_VAR, 0},
   { "427"	, AUTHO_TLV_VAR, 0},
   { "428"	, AUTHO_TLV_VAR, 0},/*AUTO_PAN_INFO*/
   { "429"	, AUTHO_TLV_VAR, 0},
   { "440"	, AUTHO_TLV_VAR, 0},
   { "441"	, AUTHO_TLV_VAR, 0},
   { "442"	, AUTHO_TLV_VAR, 0},
   { "443"	, AUTHO_TLV_VAR, 0},
   { "444"	, AUTHO_TLV_VAR, 0},
   { "445"	, AUTHO_TLV_VAR, 0},
   { "446"	, AUTHO_TLV_VAR, 0},
   { "447"	, AUTHO_TLV_VAR, 0},/*ICH20171205 Cut off info*/
   { "448"  , AUTHO_TLV_VAR, 0},/*ZKO26032019 pvk for IBM offset*/
   { "460"  , AUTHO_TLV_VAR, 0},
   { "461"	, AUTHO_TLV_VAR, 0},
   { "462"	, AUTHO_TLV_VAR, 0},
   { "470"	, AUTHO_TLV_VAR, 0},
   { "471"	, AUTHO_TLV_VAR, 0},
   { "499"	, AUTHO_TLV_VAR, 0},



   /* --- ADDE BY KR POUR ISO 3 --*/
   { "420"	, AUTHO_TLV_VAR, 0}, /* AUTO_PVV_TAG */
   { "421"	, AUTHO_TLV_VAR, 0}, /* AUTO_OFFSET_TAG */
   { "422"	, AUTHO_TLV_VAR, 0}, /* AUTO_PVK_INDEX_TAG */

   { "430"	, AUTHO_TLV_VAR, 0}, /*DYNAMIC AUTHENTICATION TYPE*/ /*MJI 28/01/2021*/

   { "800"	, AUTHO_TLV_VAR, 0},  /* AUTO_MSG_FLOW_MAPPING_ID*/ /*EBE130925:SWMON*/

   /** ME140606 Added for issuer script **/
   { "900"	, AUTHO_TLV_VAR, 0}, /* ICC_APPLICATION_INDX */
   { "901"	, AUTHO_TLV_VAR, 0},  /* ISSUER_BIN           */
   { "998"	, AUTHO_TLV_VAR, 0},
   { "999"	, AUTHO_TLV_VAR, 0},
   
};

void  InitTlvAutho  (TSTlvAutho * tlvInfo)
{
   int i=0;
  
   for (i = 0; i < MAX_AUTHO_TLV; i++)
   {
      tlvInfo->nPosTlv [i] = 0;
      tlvInfo->nPresent[i] = NOT_PRESENT;
   }

   tlvInfo->nLength  = 0;
   memset(tlvInfo->sTlvData,   '\0', MAX_AUTHO_TLV_LEN);

}
/* EBE130424 : Below is wrong: Consider having:
TAG1 LEN1 DATA1 TAG6 LEN6 DATA6 TAG3 LEN3 DATA3
if TAG6 changes its length, then TAG3 will not having its position adjusted
void AdjustTlvAutho (TSTlvAutho * tlvInfo, int nIndice, int nLength)
{
   int i;
   for (i = nIndice; i < MAX_AUTHO_TLV; i++)
   {
      if ( tlvInfo->nPresent[i] == PRESENT)
         tlvInfo->nPosTlv [i]  += nLength;
   }
}
*/

void AdjustTlvAutho (TSTlvAutho * tlvInfo, int nIndice, int nLength)
{
   int i;
   for (i = 0; i < MAX_AUTHO_TLV; i++)
   {
       if ( tlvInfo->nPresent[i] == PRESENT && tlvInfo->nPosTlv [i] > tlvInfo->nPosTlv [nIndice] )
       {
            tlvInfo->nPosTlv [i]  += nLength;
       }

   }
}

int GetTlvAuthoIndice ( char * szTlvName)
{
   int nIndice, nSize;
   nSize = sizeof(tab_TlvAuthoInfo) /sizeof(tlv_info_t);
   nIndice = 0;

   do
   {
      if (memcmp(szTlvName, tab_TlvAuthoInfo[nIndice].sTlvName, 3) == 0)
         return(nIndice);
      nIndice++;

   }while (nIndice < nSize);
   return(-1);
}


int GetTlvAuthoType ( int nIndice )
{
   return ( tab_TlvAuthoInfo[nIndice].nType);
}

int GetTlvAuthoLength ( int nIndice )
{
   return ( tab_TlvAuthoInfo[nIndice].nLength);
}



int ValidateTlvData (char *pTlvBuffer)
{
int nIndice, nSize,length,nRetCode;
char sLine[MAX_LINE_TRC];
char tlv_name[256];
char data[LG_MAX];

TSTlvAutho  tlvInfo;
   trace_event("Start ValidateTlvData()",PROCESSING);
   InitTlvAutho    ( &tlvInfo );
   AnalyseTlvAutho ( pTlvBuffer , &tlvInfo );
   nSize = sizeof(tab_TlvAuthoInfo) /sizeof(tlv_info_t);
   nIndice = 0;
   do
   {
   		if (tlvInfo.nPresent[nIndice] == PRESENT)
   		{
   			memset(tlv_name, 0, sizeof(tlv_name));
			memcpy(tlv_name, tab_TlvAuthoInfo[nIndice].sTlvName, 3);
   			memset(data, 0, sizeof(data));
			nRetCode = GetTlvAutho (tlv_name, &tlvInfo, data , &length);
            if (nRetCode != SUCCESS)
            {
    			sprintf(sLine,"End   ValidateTlvData|Presence Error(NOK|%s)",tlv_name);
    			trace_event(sLine,PROCESSING);
    			return(NOK);
            }
            if(length != strlen(data))
            {
    			sprintf(sLine,"End   ValidateTlvData|Length Error(NOK|%s|%d|%lu|%d)",tlv_name,length,strlen(data),nIndice);
    			trace_event(sLine,PROCESSING);
				/*dump_buffer(data, length,'A','X','X');*/ /** PA DSS **/
				/*dump_buffer(tlvInfo.sTlvData, tlvInfo.nLength,'A','X','X');*/ /** PA DSS **/
    			return(NOK);
            }
        }
   nIndice ++;
   }while (nIndice < nSize);

   InitTlvAutho    ( &tlvInfo ); /* MMJ PA DSS */
   memset(data, 0, sizeof(data)); /**PA DSS*/

    trace_event("End   ValidateTlvData(OK)",PROCESSING);
    return(OK);
}
