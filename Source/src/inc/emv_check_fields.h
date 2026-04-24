/*******************************************************************************/
/*  (c) Hightech Payment Systems 2005                                          */
/*  NAME                                                                       */
/*      iccprod_ictag.h                                                        */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Management emv fields that have tlv format (file card production)   */
/* NOTES                                                                       */
/*                                                                             */
/* INITIAL VERSION                                                             */
/*        Y.LAMRANI   01/04/2005 -         V 2.2.0                             */
/*******************************************************************************/

#ifndef HPS_EMV_CHECK_FIELDS_H
#define HPS_EMV_CHECK_FIELDS_H

#define N   0
#define P   1

typedef struct SEmvFields
{
   char           nPresence; /* N: Not Present P: Present */
}TSEmvFields;


TSEmvFields EmvField[] =
{
   {'P'},/* HEADER                       */ 
   {'P'},/* TRACK_2_DATA                 */
   {'P'},/* CARDHOLDER_NAME              */
   {'P'},/* TRACK_1_DATA                 */
   {'P'},/* APPL_EXPIRATION_DATE         */
   {'P'},/* APPL_PRIMARY_ACCOUNT         */
   {'P'},/* APPL_VERSION_NUMBER          */
   {'P'},/* APPL_CURRENCY_CODE_1         */
   {'P'},/* APPL_EFFECTIVE_DATE          */
   {'P'},/* APPL_SEQ_NBR                 */
   {'P'},/* APPL_USAGE_CONTROL           */
   {'P'},/* ISSUER_COUNTRY_CODE          */
   {'P'},/* ISSUER_COUNTRY_CODE1         */
   {'P'},/* APPL_SERVICE_CODE            */
   {'P'},/* APPL_CURRENCY_EXP            */
   {'P'},/* DYNAMIC_DATA_OBJECT_LIST     */
   {'P'},/* ISSUER_CODE_TABLE_INDEX      */
   {'P'},/* LANGUAGE_PREFERENCE          */
   {'P'},/* APPLICATION_IDENTIFIER       */
   {'P'},/* APPLICATION_LABEL            */
   {'P'},/* APPL_PRIORITY_INDICATOR      */
   {'P'},/* APPL_PREFERRED_NAME          */
   {'P'},/* APPL_INTERCHANGE_PROFILE     */
   {'P'},/* APPL_CURRENCY_CODE_2         */
   {'P'},/* APPL_DEFAULT_ACTION          */
   {'P'},/* APPLICATION_TRX_COUNTER      */
   {'P'},/* CONSECUTIVE_TRX_LIMIT_INT_T  */
   {'P'},/* CONSECUTIVE_TRX_LIMIT_INT_C  */
   {'P'},/* CUMUL_TRX_AMOUNT_LIMIT_TOT   */
   {'P'},/* CUMUL_TRX_AMNT_UPLIMIT_TOT   */
   {'P'},/* GEOGRAPHIC_INDICATOR         */
   {'P'},/* ISSUER_AUTHENT_INDICATOR     */
   {'P'},/* LOWER_CONSECUTIVE_OFF_LIM    */
   {'P'},/* LOWER_OFFLINE_LIMIT          */
   {'P'},/* UPPER_CONSECUTIVE_OFFLIMIT   */
   {'P'},/* UPPER_CONSECUTIVE_OFFLIMIT1  */
   {'P'},/* LAST_ONLINE_ATC_REGISTER     */
   {'P'},/* PIN_TRY_COUNTER              */
   {'P'},/* SPECIFIC DATA                */
    /***** Start Specific Data ****/
   {'P'},/* KEK_KEY                      */
   {'P'},/* KCV_KEK                      */
   {'P'},/* APPLICATION_CONTROL          */
   {'P'},/* NON_DOMESTIC_CTRL_FACTOT_EXP */
   {'P'},/* LOWER_CUMUL_OFFLINE_TRX_AMNT */
   {'P'},/* UPPER_CUMUL_OFFLINE_TRX_AMNT */
   {'P'},/* CARD_TVR_ACTION_CODE         */
   {'P'},/* CARDISS_ACTION_CODE_OFFLINE  */
   {'P'},/* CARDISS_ACTION_CODE_ONLINE   */
   {'P'},/* CARDISS_ACTION_CODE_DECLINE  */
   {'P'},/* PEK_KEY                      */
    /***** End Specific Data ****/
   {'P'},/* CARD_RISK_MANAG_CDOL1         */
   {'P'},/* CARD_RISK_MANAG_CDOL2         */
   {'P'},/* CA_PUBLIC_KEY_INDEX           */
   {'P'},/* ISS_PUBLIC_KEY_CERTIFICATE    */
   {'P'},/* PUBLIC_KEY_CERTIFICATE        */
   {'P'},/* ISS_PUBLIC_KEY_EXPONENT       */
   {'P'},/* PUBLIC_KEY_EXPONENT           */
   {'P'},/* SIGNED_STATIC_APPL_DATA       */
   {'P'},/* SIGNED_DYNAMIC_DATA           */
   {'P'},/* ISS_PUBLIC_KEY_REMAINDER      */
   {'P'},/* PUBLIC_KEY_REMAINDER          */
   {'P'},/* APPLICATION_FILE_LOCATOR      */
   {'P'},/* STATIC_DATA_AUTHENT_TAG_LIST  */
   {'P'},/* CVM_LIST                      */
   {'P'},/* ISSUER_ACTION_CODE_DEFAULT    */
   {'P'},/* ISSUER_ACTION_CODE_DENIAL     */
   {'P'},/* ISSUER_ACTION_CODE_ONLINE     */
   {'P'},/* CRYPTOGRAM_DATA               */
   {'P'},/* PIN_BLOCK                     */
   {'P'},/* KEY BUFFER                    */
   {'P'},/* CRYPTOGRAM VERSION NUMBER     */
   {'P'},/* DERIVATION KEY INDEX          */
   {'P'},/* PIN TRY LIMIT_TAG             */
   {'P'},/* ONLINE AUTHORIZATION INDICATOR */
   {'P'},/* REFERENCE PIN CODE */
   {'P'},/* END_CODE                      */
};
#endif
