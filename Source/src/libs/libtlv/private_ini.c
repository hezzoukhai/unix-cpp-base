#include <stdio.h>
#include <stdlib.h>
#include <tlv_private.h>
#include <define.h>


static tlv_info_t  tab_TlvPrivateInfo[] =
{
	{ "A09", PRIVATE_TLV_VAR, 0 } ,/* "BAL_TYPE_TAG            " */

	{ "P01", PRIVATE_TLV_VAR, 0 } ,/* "FILE_UPDATE_ERR_CODE_TAG" */
	{ "P02", PRIVATE_TLV_VAR, 0 } ,/* "MESSAGE_TEXT            " */
	{ "P03", PRIVATE_TLV_VAR, 0 } ,/* "HPS_LIMITS_CRNCY_TAG    " */
	{ "P04", PRIVATE_TLV_VAR, 0 } ,/* "HPS_LIMITS_AMOUNT_TAG   " */
	{ "P05", PRIVATE_TLV_VAR, 0 } ,/* "HPS_HOST_SCENARIO_CODE  " */
	{ "P06", PRIVATE_TLV_VAR, 0 } ,/* "CHECK_SET_ON_OFF_LIMIT  " */
	{ "P07", PRIVATE_TLV_VAR, 0 } ,/* "SPEC_LIMIT_ID			 " */
	{ "P08", PRIVATE_TLV_VAR, 0 } ,/* "ORIG_BILLING_INFO      " */
	{ "P09", PRIVATE_TLV_VAR, 0 } ,/* "HPS_ACQUIRER_BANK       " */
	{ "P10", PRIVATE_TLV_VAR, 0 } ,/* "ENCR_PIN_BLOCK_KEY      " */
	{ "P11", PRIVATE_TLV_VAR, 0 } ,/* "ORIGINAL_TRX_ID         " */		/*IBO20190722 PROD00070403 Enh19.Q4 Art 2.17*/
	{ "P12", PRIVATE_TLV_VAR, 0 } ,/* "PRIS_INTERNAL_ID        " */
	{ "P13", PRIVATE_TLV_VAR, 0 } ,/* "EXTERNAL_STAN_TAG       " */
	{ "P14", PRIVATE_TLV_VAR, 0 } ,/* "EVENT_ACTION            " */

	{ "P16", PRIVATE_TLV_VAR, 0 } ,/* "PVV_OFFSET_TAG          " */
	{ "P17", PRIVATE_TLV_VAR, 0 } ,/* RRN_RETREVIAL_TAG */
	{ "P18", PRIVATE_TLV_VAR, 0 } ,/* "PAYMENT_ACC_REFERENCE           " */ /*AMER20170302: MCI Enh17.Q2*/
	{ "P19", PRIVATE_TLV_VAR, 0 } ,/* "MVV_TAG                " */
	{ "P20", PRIVATE_TLV_VAR, 0 } ,/* "ICVV_RESULT_CODE_TAG   " TBO05082014 */        
	{ "P21", PRIVATE_TLV_VAR, 0 } ,/* MERCHANT_ADDITIONAL_DATA */  /* AHA20161214  PROD00036000*/
	/* { "P22", PRIVATE_TLV_VAR, 0 } , *//* DYNAMIC_NBR_RESULT_CODE_TAG */ /* ELKAM20221221 PLUTONL-5555 */
	/* { "P23", PRIVATE_TLV_VAR, 2 }, */ /* "MCI_DEVICE_TYPE_IND     " */  /* ENH 11.1 OHD10052011 */ /* ELKAM20221221 PLUTONL-5555 */
	{ "P24", PRIVATE_TLV_VAR, 2 }, /* "CMC7_LINE_TAG				" */
	{ "P25", PRIVATE_TLV_VAR, 0 } ,/* "CRYPT_RESULT_CODE_TAG  " */
	/* { "P26", PRIVATE_TLV_VAR, 0 } , *//* "AUC_RESULT_CODE_TAG  " */ /* ELKAM20221221 PLUTONL-5555 */
	{ "P27", PRIVATE_TLV_VAR, 0 } ,/* "PIN_RESULT_CODE_TAG  " */	/*AMER20170801 PROD00042216: P28 turned into P27*/
	{ "P28", PRIVATE_TLV_VAR, 0 } ,/* "EXTERNAL_SECURITY_VER_RESULTS_TAG  " */	/*AMER20170801: PROD00042216*/
	{ "P29", PRIVATE_TLV_VAR, 0 } ,/* "SECURITY_VER_RESULTS_TAG*/
	{ "P30", PRIVATE_TLV_VAR, 0 } ,/* "CHIP_AUTH_ADD_INFO" */
	{ "P31", PRIVATE_TLV_VAR, 0 } ,/* "MCI_DEVICE_TYPE_IND" */
	{ "P32", PRIVATE_TLV_VAR, 6 }, /* "MCI_ASSIGNED_ID         " */  /* ENH 11.1 OHD10052011 */
	{ "P33", PRIVATE_TLV_VAR, 6 }, /* "MCI_PAN_MAPPING_INFO    " */  /* ENH 13.2 OAD28082013 */  
	{ "P34", PRIVATE_TLV_VAR, 6 }, /* "MCI_ON_BEHALF_SERV      " */  /* ENH 13.2 OAD28082013 */ 
	{ "P35", PRIVATE_TLV_VAR, 0 } ,/* "MCI_TRACEID" */
	{ "P36", PRIVATE_TLV_VAR, 0 } ,/* "ISSUER_BANK" */
	
	{ "P38", PRIVATE_TLV_VAR, 0 } ,/* EXT_SECURITY_RESULTS_DTL */ /*AMER20190228 PROD00066569: CBAE Enh5.0 989*/
	{ "P39", PRIVATE_TLV_VAR, 0 } , /* aha20150428  merchant bank code */
	{ "P40", PRIVATE_TLV_VAR, 0 } ,/* "E_COMMERCE_CERTIFICATE_TAG" */
	{ "P41", PRIVATE_TLV_VAR, 0 } ,/* "MERCHANT_ACRONYM" */
	{ "P42", PRIVATE_TLV_VAR, 0 } ,/* "SECURITY_LEVEL_IND_TAG  " *//*YK27032006*/
	{ "P43", PRIVATE_TLV_VAR, 0 } ,/* "UCAF_TAG                " */  /** MAW20051109: Europay 05.2 **/
	{ "P44", PRIVATE_TLV_VAR, 0 } ,/* "XTERNAL_CRYPT_RESULTS_TAG" */
	
	{ "P46", PRIVATE_TLV_VAR, 0 } , /* aha20150428  acquirer bank code */
	{ "P47", PRIVATE_TLV_VAR, 0 } , /* ROUTING_NETWORK_CODE */
	{ "P48", PRIVATE_TLV_VAR, 0 } ,/* "TCC                     " */
	{ "P49", PRIVATE_TLV_VAR, 0 } ,/* "KSN_DESC_TAG            " */
	{ "P50", PRIVATE_TLV_VAR, 0 } ,/* "KSN_TAG                 " */
	{ "P51", PRIVATE_TLV_VAR, 0 } ,/* AAV_TAG */
	{ "P52", PRIVATE_TLV_VAR, 0 } ,/* "DIGITAL_PAYMENT_DATA" */ 
	{ "P53", PRIVATE_TLV_VAR, 0 } ,/* "ATM_DEPOSIT_NOTES" */ 
	{ "P54", PRIVATE_TLV_VAR, 0 } ,/* AAV_RESULT */
	{ "P55", PRIVATE_TLV_VAR, 0 } ,/* TOKEN_DATA */
	{ "P56", PRIVATE_TLV_VAR, 0 } , /*ICH See PROD00039494*/ 
	{ "P57", PRIVATE_TLV_VAR, 0 } , /*ICH2017 for AFD*/
	{ "P58", PRIVATE_TLV_VAR, 0 } , /* ACC_PT_CARD_PRD_ID  */
	{ "P59", PRIVATE_TLV_VAR, 0 } , /* aha20150428  operation code  */
	{ "P60", PRIVATE_TLV_VAR, 0 } ,/* WATCH_LIST_SCORING_RES */
	{ "P61", PRIVATE_TLV_VAR, 0 } ,/* "POS DATA EXTENTION			 */  /* Mehdi Elyajizi Enh72 */
	{ "P62", PRIVATE_TLV_VAR, 0 } ,/* "FEES_AMOUNT					*/   
	{ "P63", PRIVATE_TLV_VAR, 0 } ,/* "AUTHORIZED_AMOUNT		     */   
	{ "P64", PRIVATE_TLV_VAR, 0 } ,/* "AUTH_ADD_INFO				*/
	{ "P65", PRIVATE_TLV_VAR, 0 } ,/* "PAYMENT_AUTH_INFO					*/
	{ "P66", PRIVATE_TLV_VAR, 0 } ,/* "ECOM_AUTH_INFO				*/
	{ "P67", PRIVATE_TLV_VAR, 0 } ,/* "AUTHORIZATION_IDENTIFIER		*/
	{ "P68", PRIVATE_TLV_VAR, 0 } ,/* "TRANSACTION_IDENTIFIER		*/
	{ "P69", PRIVATE_TLV_VAR, 0 } ,/* "ACTUAL_OUTLET_NUMBER_TAG" */ /** MAW20041223 **/
	{ "P70", PRIVATE_TLV_VAR, 0 } ,/* "TERM_INFO_DATA		*/   
	{ "P71", PRIVATE_TLV_VAR, 0 } ,/* "Recurring Payment       " */   /* SLA 120213 Recurring Payment*/
	{ "P72", PRIVATE_TLV_VAR, 0 } ,/* "TRANSACTION_ID_TAG_1" *//** OAD22032013 **/
	{ "P76", PRIVATE_TLV_VAR, 0 } ,/* "MCI_ELEC_ACCPT_IND_TAG  " */  /** MAW20051109: Europay 05.2 **/
	{ "P77", PRIVATE_TLV_VAR, 0 } ,/* "TRANS_XID_TAG           " */
	{ "P78", PRIVATE_TLV_VAR, 0 } ,/* "TRANS_STAIN_TAG         " */
	{ "P80", PRIVATE_TLV_VAR, 0 } ,/* "DINERS_NET_REF_ID " */
	{ "P81", PRIVATE_TLV_VAR, 0 } ,/* "CARD_ISS_DATA " */
	{ "P82", PRIVATE_TLV_VAR, 0 } ,/* "ADDRESS_VERIF_REQ_TYPE_TAG " */
	{ "P83", PRIVATE_TLV_VAR, 0 } ,/* "ADDRESS_VERIF_REP_TYPE_TAG " */
	{ "P84", PRIVATE_TLV_VAR, 0 } ,/* CVM_DATA */
	{ "P85", PRIVATE_TLV_VAR, 0 } ,/* "EXTERNAL_RESP_SOURCE    " */
	{ "P86", PRIVATE_TLV_VAR, 0 } ,/* "FRD_CTRL_RESULT_TAG" */
	{ "P87", PRIVATE_TLV_VAR, 0 } ,/* "EXTERNAL_CVV_RESULT_CODE" */
	{ "P88", PRIVATE_TLV_VAR, 0 } ,/* "EXTERNAL_CVV2_RESULT_CODE" */
	{ "P89", PRIVATE_TLV_VAR, 0 } ,/* "CVC1_TRACK_EDIT_ERROR   " */
	{ "P90", PRIVATE_TLV_VAR, 0 } ,/* FRAUD_SCORE */
	{ "P91", PRIVATE_TLV_VAR, 0 } ,/* AMEX_TRX_ID */
	{ "P92", PRIVATE_TLV_VAR, 0 } ,/* "CVC2_DATA               " */
	{ "P93", PRIVATE_TLV_VAR, 0 } ,/* "HSM_RESULT_CODE         " */
	{ "P94", PRIVATE_TLV_VAR, 0 } ,/* "SECONDARY_RESOURCE_TAG  " */
	{ "P95", PRIVATE_TLV_VAR, 0 } ,/* NETWORK_CODE				*/
	{ "P96", PRIVATE_TLV_VAR, 0 } ,/* "MALFUNCTION_STIP_TAG "  " */
	{ "P97", PRIVATE_TLV_VAR, 0 } ,/* "ACQUIRER GENRE       "  " */
	{ "P98", PRIVATE_TLV_VAR, 0 } ,/* "MARKET_SPECIFIC_DATA " */
	{ "P99", PRIVATE_TLV_VAR, 0 } ,/* "DURATION  " */
	{ "B01", PRIVATE_TLV_VAR, 0 } ,/* "ATM_LANG_REQ" */ 
	{ "B02", PRIVATE_TLV_VAR, 0 } ,/* "CARDLESS_TYPE" */
	{ "B03", PRIVATE_TLV_VAR, 0 } ,/* "AUTH_REQUESTED_AMOUNT" */
	{ "O01", PRIVATE_TLV_VAR, 0 } ,/*ISS_AUTH_OPT*/
	{ "O02", PRIVATE_TLV_VAR, 0 } ,/*LIMIT_ID_INFO*/
	{ "O03", PRIVATE_TLV_VAR, 0 } ,/*SPEC_LIMIT_ID_INFO*/
	{ "S20", PRIVATE_TLV_VAR, 0 } ,/* "MONEY_TRANSFER_DATA  " */
	{ "S21", PRIVATE_TLV_VAR, 0 } ,/* "Adress_Verification_Data "*/
    { "S22", PRIVATE_TLV_VAR, 0 } ,/* "Identifiant de solution de paiement mobile "*/
    { "S23", PRIVATE_TLV_VAR, 0 } ,/* "Typologie de transaction "*/ 
    { "S24", PRIVATE_TLV_VAR, 0 } ,/* "Installment Loan"*/  /*ZKO25092018*/
    { "S25", PRIVATE_TLV_VAR, 0 } ,/* "DIGITIZATION_DATA "*/
    { "S26", PRIVATE_TLV_VAR, 0 } ,/* "Code produit carte SA"*/
    { "S27", PRIVATE_TLV_VAR, 0 } ,/* "Adresse de livraison"*/
    { "S28", PRIVATE_TLV_VAR, 0 } ,/* "Adresse IP"*/
    { "S29", PRIVATE_TLV_VAR, 0 } ,/* "3DS DATA "*//* PLUTONL-4280 :Enh5.03 :1372*/
    { "S30", PRIVATE_TLV_VAR, 0 } ,/* "FLEET DATA"*/ /*ZKO24092018*/
	 { "S31", PRIVATE_TLV_VAR, 0 } ,/* "INDICATEUR TYPOLOGIE CARTE "*/
	{ "007", PRIVATE_TLV_VAR, 1 } ,/* "POS_TRANS_STATUS        " */ /* ENH 11.1 OHD10052011 */
	{ "022", PRIVATE_TLV_VAR, 0 } ,/* "NET_POS_ENTRY_MODE      " */
	{ "024", PRIVATE_TLV_FIX, 3 } ,/* "Original function code  " */ 	/*AEA20210712 Enh21.Q4 AN4779*/
	{ "037", PRIVATE_TLV_VAR, 0 } ,/* "NET_REFERENCE_NUMBER" */  /*HAL 20201303 Enh Q2.2020 Art 1.1 */
	{ "025", PRIVATE_TLV_VAR, 0 } ,/* "NET_POS_CONDITION_CODE  " */
	{ "039", PRIVATE_TLV_VAR, 0 } ,/* "NET_RESPONSE_CODE       " */
	{ "060", PRIVATE_TLV_VAR, 0 } ,/* "NET_ADD_POS_DATA        " */
	{ "061", PRIVATE_TLV_VAR, 0 } ,/* "NET_ADD_POS_DATA1       " */
	{ "091", PRIVATE_TLV_VAR, 0 } ,/* "FILE_UPDATE_CODE_TAG    " */
	{ "127", PRIVATE_TLV_VAR, 0 } ,/* "ACTION_CODE_TAG         " */

	{ "L01", PRIVATE_TLV_VAR, 0 } ,/* "CARD_AUTH_REL_IND       " */
	{ "L02", PRIVATE_TLV_VAR, 0 } ,/* "CHIP_CONDITION_CODE     " */
	{ "L03", PRIVATE_TLV_VAR, 0 } ,/* "VSDC_TRAN_IND           " */

	{ "F01", PRIVATE_TLV_VAR, 0 } ,/* "CPS_ACI_TAG                     */
	{ "F02", PRIVATE_TLV_VAR, 0 } ,/* "CPS_TRANS_ID_TAG                */
	{ "F03", PRIVATE_TLV_VAR, 0 } ,/* "CPS_VALIDATION_DATE_TAG         */
	{ "F04", PRIVATE_TLV_VAR, 0 } ,/* "CPS_MARKET_SPECIFIC_TAG         */
	{ "F05", PRIVATE_TLV_VAR, 0 } ,/* "CPS_DURATION_TAG                */
	{ "F06", PRIVATE_TLV_VAR, 0 } ,/* "CPS_PRESTIGIOUS_IND_TAG         */
	{ "F17", PRIVATE_TLV_VAR, 0 } ,/* "CPS_MCI_INTER_TAG               */
	{ "F19", PRIVATE_TLV_VAR, 0 } ,/* "MVV_TAG						 */
	{ "F23", PRIVATE_TLV_VAR, 0 } ,/* "PRODUCT_ID					 */
	{ "F25", PRIVATE_TLV_VAR, 0 } ,/* SPEND_QUALIFIED_INDICATOR     */ /*  HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 8.1.2*/
	{ "C01", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APP_CRYPTOGRAM_TAG*/
	{ "C02", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CRYPTO_INFO_DATA_TAG*/
	{ "C03", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TRANS_CUR_CODE_TAG*/
	{ "C04", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TRANS_DATE_TAG*/
	{ "C05", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TRANS_TYPE_TAG*/
	{ "C06", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TRANS_AMOUNT_TAG*/
	{ "C07", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ATC_TAG*/
	{ "C08", PRIVATE_TLV_VAR, 0 } ,/*CHIP_UNPRED_NUMBER_TAG*/
	{ "C09", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CATEGORY_CODE_TAG*/
	{ "C10", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TVR_TAG*/
	{ "C11", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TERM_COUNTRY_CODE_TAG*/
	{ "C12", PRIVATE_TLV_VAR, 0 } ,/*CHIP_AIP_TAG*/
	{ "C13", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CVM_RESULTS_TAG*/
	{ "C14", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TERM_CAPABILITY_TAG*/
	{ "C15", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TERM_TYPE_TAG*/
	{ "C16", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPLI_ID_TAG*/
	{ "C17", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APPLI_VER_TAG*/
	{ "C18", PRIVATE_TLV_VAR, 0 } ,/*CHIP_IFD_SERIAL_NUM_TAG*/
	{ "C19", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TRX_SEQ_COUNT_TAG*/
	{ "C20", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_APPLI_DATA_TAG*/
	{ "C21", PRIVATE_TLV_VAR, 0 } ,/*CHIP_OTHER_AMOUNT_TAG*/
	{ "C22", PRIVATE_TLV_VAR, 0 } ,/*CHIP_DERIV_KEY_INDEX_TAG*/
	{ "C23", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_AUTHEN_DATA_TAG*/
	{ "C24", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_ARPC_RESP_CODE_TAG*/
	{ "C25", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_SCRIPT_RESULT_TAG*/
	{ "C26", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APP_SELECTED*/  /**start LHI 29 08 *****/
	{ "C27", PRIVATE_TLV_VAR, 0 } ,/*CHIP_TRACK2*/
	{ "C28", PRIVATE_TLV_VAR, 0 } ,/*CHIP_PAN*/
	{ "C29", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APP_NAME*/
	{ "C30", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APP_LABEL*/
	{ "C31", PRIVATE_TLV_VAR, 0 } ,/*CHIP_SECUR_ATT*/    /*End LHI 29 08****/
	{ "C32", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_SCRIPT_1*/          /* Mehdi Elyajizi Issuer Script */
	{ "C33", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ISS_SCRIPT_2*/          /* Mehdi Elyajizi Issuer Script */
	{ "C34", PRIVATE_TLV_VAR, 0 } ,/*CHIP_PAN_SEQ_NBR*/


	{ "C36", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_ISS_SCRIPT_CMD				*/
	{ "C37", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_TERM_CAP	*/
	{ "C38", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_APP_SEQ_NUMBER				*/
	{ "C39", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_CVM_LIST				*/
	{ "C40", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_APPL_EFF_DATE		*/
	{ "C41", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_APPL_EXP_DATE				*/

	{ "C43", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_PIN_BLOCK_TAG*/ /* ACK PIN CHANGE MARCH2015 */

	{ "C60", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_TAG_KERNEL_ID					*/
	{ "C61", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_TAG_ICC_PROCESSING_RESULT		*/
	{ "C62", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_TAG_APPL_TYPE					*/
	{ "C63", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_TAG_RTT_RESULT					*/
	{ "C64", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_TAG_CONTACTLESS_DEVICE			*/
	{ "C65", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_TAG_EMV_ACQ_MODE					*/
	{ "C66", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_TAG_CARD_AUTH_RES_CODE			*/
	{ "C71", PRIVATE_TLV_VAR, 0 } ,		/*CHIP_TAG_TRACK_1_DATA					*/

	{ "C72", PRIVATE_TLV_VAR, 0 } ,/*CHIP_FFI_TAG*/    /*SWI06032015 ENH15.1*/
	{ "C73", PRIVATE_TLV_VAR, 0 } ,/*CHIP_AUTHOR_RESP_CODE*/
	{ "C80", PRIVATE_TLV_VAR, 0 } ,/*CHIP_DED_FILE_NAME_TAG */ 
	{ "C81", PRIVATE_TLV_VAR, 0 } ,/*CHIP_APP_USE_CONTROL_TAG */ 

	{ "C82", PRIVATE_TLV_VAR, 0 } ,/*CHIP_IAC_DEFAULT */ 
	{ "C83", PRIVATE_TLV_VAR, 0 } ,/*CHIP_IAC_DENIAL */ 
	{ "C84", PRIVATE_TLV_VAR, 0 } ,/*CHIP_IAC_ONLINE */ 

	{ "C96", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CVR */ 
	{ "C97", PRIVATE_TLV_VAR, 0 } ,/*CHIP_ORIG_ARC */ /*SNO060116*/
	{ "C98", PRIVATE_TLV_VAR, 0 } ,/*CHIP_CALC_ARC */ /*ARC Feature ICH20191010*/
	{ "C99", PRIVATE_TLV_VAR, 0 } ,/*APP_RGSTD_DAT */ /*SNO060116*/

	{ "N00", PRIVATE_TLV_VAR, 0 } ,/*NET_BANKNET_DATA_TAG      */
	/* { "N00", PRIVATE_TLV_FIX, 12 } ,
	{ "N01", PRIVATE_TLV_VAR, 4 } , MEL Comment : Version avant intï¿½gration EMV Issuing */
	{ "N01", PRIVATE_TLV_VAR, 0 } ,/*NET_NETWORK_ID_TAG        */
	{ "N02", PRIVATE_TLV_VAR, 0 } ,/* "NET_REFERENCE_ID_TAG    " */  
	{ "N03", PRIVATE_TLV_VAR, 0 } ,/*NET_MSG_REASON_CODE_TAG   */
	{ "N04", PRIVATE_TLV_VAR, 0 } ,/*NET_STIP_REASON_CODE_TAG  */
	{ "N06", PRIVATE_TLV_VAR, 0 } ,/*NET_CHARGEBACK_FLAGS_TAG  */
	{ "N07", PRIVATE_TLV_VAR, 0 } ,/*NET_NET_PARTI_FLAGS_TAG   */
	{ "N08", PRIVATE_TLV_VAR, 0 } ,/*NET_VISA_ABI_TAG          */
	{ "N09", PRIVATE_TLV_VAR, 0 } ,/*NET_FRAUD_DATA_TAG        */
	{ "N11", PRIVATE_TLV_VAR, 0 } ,/*NET_REIMB_ATTR_TAG        */
	{ "N12", PRIVATE_TLV_VAR, 0 } ,/*NET_SHARE_GRP_CODE_TAG    */
	{ "N13", PRIVATE_TLV_VAR, 0 } ,/*NET_DECIMAL_POS_TAG       */
	{ "N14", PRIVATE_TLV_VAR, 0 } ,/*NET_ISS_CUR_CONV_DATA_TAG */
	{ "N15", PRIVATE_TLV_VAR, 0 } ,/*NET_STLMNT_AMOUT_TAG      */
	{ "N16", PRIVATE_TLV_VAR, 0 } ,/*NET_IEW_SBA_TAG           */
	{ "N17", PRIVATE_TLV_VAR, 0 } ,/*NET_ADTL_DATA_IND_TAG     */
	{ "N18", PRIVATE_TLV_VAR, 0 } ,/*NET_VOL_REQ_IND_TAG       */
	{ "N20", PRIVATE_TLV_VAR, 0 } ,/*VISA_ACT_CHK_RES		  */
	{ "N21", PRIVATE_TLV_VAR, 0 } ,/*NET_VISA_MERC_ID		  */ /*HAL 20201803  Q2.2020  AN 9.3.3*/
	{ "N22", PRIVATE_TLV_VAR, 0 } ,/*NET_VISA_EXT_STIP_REASON_CODE		  */ /* HAL PLUTONL-2629 20210127 Enh Q2.2021 Art 1.1*/ 
	{ "N23", PRIVATE_TLV_VAR, 0 } ,/*NET_VISA_ECOM_DATA*/	/*PLUTONL-4280 :Enh5.03 :1372*/
	{ "N24", PRIVATE_TLV_VAR, 0 } ,/*VISA_WATCH_LIST_RC*//*PLUTONL-4280 :Enh5.03 :1383*/
	{ "N25", PRIVATE_TLV_FIX, 2 } ,	/*NET_VISA_OVERALL_ASSESSMENT_SECURE_ECOM */ /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art3.1*/	
	{ "N26", PRIVATE_TLV_FIX, 26 } ,/*NET_VISA_PURCHASE_IDENTIFIER */ /*IZL 27062022 PLUTONL-4902 ENH 22.Q4 VISA Art8.3.1*/	
	{ "N53", PRIVATE_TLV_VAR, 0 } ,/*NET_MCI_TRX_ANALYSIS	   */  /*ACH20190910 PROD00070986*/
	{ "N54", PRIVATE_TLV_VAR, 0 } ,/*NET_MCI_TKN_TRX_ID		   */  /*ACH20190910 PROD00070986*/
    { "N55", PRIVATE_TLV_FIX, 2 } ,/*HAL 20201902  Q2.2020  AN 2941*/	/*NET_MCI_MERCHANT_ADVICE_CODE*/ /*AEA20210712 Enh21.Q4 AN4761*/
	{ "N56", PRIVATE_TLV_FIX, 1 } ,/*NET_TOKEN_PROGAM           */  /*IZL 20220131 PLUTONL-4015 ENH 22.Q2 VISA Art1.1*/
	{ "N57", PRIVATE_TLV_FIX, 0 } ,	/*HAL PLUTONL-4154 4.2.4	AN 4972 */
	{ "N58", PRIVATE_TLV_FIX, 4 } ,	/*MCI_MIT_CIT_INDICATOR	*/ /*IZL 03062022 PLUTONL-4810 ENH 22Q2 MC AN5524*/
	{ "N59", PRIVATE_TLV_VAR, 0 } ,	 /* PLUTONL-5548   AN6773  Enh 2023Q2 AHA20221221*/ /*Security Service Indicator*/
	{ "N60", PRIVATE_TLV_VAR, 0 } ,	/* PLUTONL-5548   AN6773  Enh 2023Q2 AHA20221221*/  /*Security Service Data*/
	{ "N62", PRIVATE_TLV_VAR, 0 } ,	/*PLUTONL-8878 Enh24Q4 AMANS AN 3.20*/



	{ "B48", PRIVATE_TLV_VAR, 0 } ,		/* BASE24_DE048				*/
	{ "B60", PRIVATE_TLV_VAR, 0 } ,		/* BASE24_DE060				*/
	{ "D90", PRIVATE_TLV_VAR, 0 } ,		/* "UPDATE_FILE_DATA" 		*/
	{ "G01", PRIVATE_TLV_VAR, 0 } ,		/* CB_DATA					*/
	{ "K01", PRIVATE_TLV_VAR, 0 } ,      /*CHRG DATA*/
	{ "K02", PRIVATE_TLV_VAR, 0 } ,      /*PARTIAL_AMOUNT_TYPE*/
    { "K03", PRIVATE_TLV_VAR, 0 } ,      /*ROUTING_CODE*//*YDA20171113 (PROD00048309)*/
   	{ "K04", PRIVATE_TLV_VAR, 0 } ,      /*SMS_VCR_DATA ICH17012018*/
	{ "K05", PRIVATE_TLV_VAR, 0 } , 	 /*RESUBMISSION_COUNTER -  MTR20180406 */
    { "K06", PRIVATE_TLV_VAR, 0 } , 	 /*Directory Server Transaction ID-  ZKO:PROD00058820 */
    { "K07", PRIVATE_TLV_VAR, 0 } , 	 /*Cashback amount in limit currency */
	{ "K08", PRIVATE_TLV_VAR, 0 } , 	 /*Cashback limit id */
	/*ZKO29122021 Start PLUTONL-3345*
	{ "K09", PRIVATE_TLV_VAR, 0 } , 	 /*Authorization exemption indicator*/ 	/*AMER20190314 PROD00066569: CBAE Enh5.00 1075*/
	{ "K10", PRIVATE_TLV_VAR, 0 } ,      /*MOBILE PAYMENT SOLUTION*/  /*ACH20190910 PROD00070986*/
	{ "K11", PRIVATE_TLV_VAR, 0 } ,      /*Fraud case sequence id*/
	{ "K12", PRIVATE_TLV_VAR, 0 } ,      /*FLEET CONTROLS*/ /*MJI 26/02/2021*/
	{ "K13", PRIVATE_TLV_VAR, 0 } ,      /*FLEET CONTROLS RESULT*/ /*MJI 26/02/2021*/
	{ "K14", PRIVATE_TLV_VAR, 0 } ,      /*DYNAMIC AUTHENTICATION TYPE*/ /*MJI 28/01/2021*/
	{ "K15", PRIVATE_TLV_VAR, 0 } ,      /*Corporate id*/
	{ "K16", PRIVATE_TLV_VAR, 0 } ,      /*Authorization message UUID*/
	{ "K17", PRIVATE_TLV_VAR, 0 } , 	 /*ACS Transaction ID*/ 	/*AMER20190314 PROD00066569: CBAE Enh5.00 1073*/
	{ "K18", PRIVATE_TLV_VAR, 0 } ,		 /* NET_ISS_SETTL_INFO */
	{ "K19", PRIVATE_TLV_VAR, 0 } ,		 /* NET_ACQ_SETTL_INFO */
	{ "K20", PRIVATE_TLV_VAR, 0 } ,      /*Private original data elements*/   /*AMER20210402 PLUTONL-2947*/
	{ "K21", PRIVATE_TLV_VAR, 0 } ,		 /*SKA20220822 PLUTONL-5086 */
	{ "K22", PRIVATE_TLV_VAR, 0 } ,		 /*SKA20220822 PLUTONL-5086 */
	{ "K30", PRIVATE_TLV_VAR, 0 } ,		 /*SOURCE_POS_ENTRY_MODE*/  /*Pos entry mode of the channel who captured the message*/ /*AMER20210925 PLUTONL-3607*/
	{ "K40", PRIVATE_TLV_VAR, 0 } ,		 /*Reattempt frozen period*//*PLUTONL-4280 :Enh5.03 :1348*/
	{ "K41", PRIVATE_TLV_VAR, 0 } ,	     /*Reattempt conditions*//*PLUTONL-4280 :Enh5.03 :1348*/
	{ "K50", PRIVATE_TLV_VAR, 0 } ,	     /*Origine Code*/ /*IBO20221121 PLUTONL-5416*/
	{ "K51", PRIVATE_TLV_VAR, 0 } ,		/*BANK_XMIT_TIME*/ /*IBO20221121 PLUTONL-5416*/
	{ "K99", PRIVATE_TLV_VAR, 8 } , 	 /*INTERNAL DATA */ /*NAB02042021 PLUTONL-2974*/
	{ "Y04", PRIVATE_TLV_VAR, 0 } ,      /*tag for the seq of the selected route*//* HAL04092020 PLUTONL-2197*/
	{ "Y10", PRIVATE_TLV_FIX, 0 } ,      /*C_MSG_FLOW_ID*/ /*SKARROUMI PLUTONL-4900*/
    { "Y11", PRIVATE_TLV_FIX, 1 } ,      /*SDR20230731 PLUTONL-6626*/
	{ "U01", PRIVATE_TLV_FIX, 1 } ,     /*DS TRANSACTION ID (Expiry date)*/
};

static tlv_info_t  tab_user_TlvPrivateInfo[MAX_USER_PRIVATE_TLV];
static int			g_nNbUserPrivTags;
/******************************************************************************/

int InitTlvPrivateTags(int nIndexCtx)
{
	int		nResult;
	int		nSize;

	trace_event("Start InitTlvPrivateTags()",PROCESSING);
	/*
	nSize = sizeof(tab_user_TlvPrivateInfo)/sizeof(tab_user_TlvPrivateInfo[0]);

	nResult = LoadPrivateDataTags(nIndexCtx,"48",tab_user_TlvPrivateInfo,nSize,&g_nNbUserPrivTags);
	*/
	trace_event("End   InitTlvPrivateTags(OK)",PROCESSING);
	return OK;
}


void  InitTlvPrivate  (TSTlvPrivate * tlvInfo)
{
	int	i;
	int	nSize;

	nSize = sizeof(tlvInfo->nPresent)/sizeof(tlvInfo->nPresent[0]);
	for (i = 0; i < nSize; i++)
	{
		tlvInfo->nPosTlv [i] = 0;
		tlvInfo->nPresent[i] = NOT_PRESENT;
	}

	tlvInfo->nLength  = 0;
	memset(tlvInfo->sTlvData,   '\0', sizeof(tlvInfo->sTlvData));
}

int GetTlvPrivateIndice ( char * szTlvName)
{
	int nIndice, nSize;

	nSize = sizeof(tab_TlvPrivateInfo) /sizeof(tlv_info_t);
	nIndice = 0;

	do
	{
		if (memcmp(szTlvName, tab_TlvPrivateInfo[nIndice].sTlvName, 3) == 0)
			return(nIndice);
		nIndice++;
	}while (nIndice < nSize);


	nIndice = 0;
	do
	{
		if (memcmp(szTlvName, tab_user_TlvPrivateInfo[nIndice].sTlvName, 3) == 0)
			return(nIndice + nSize);
		nIndice++;
	}while (nIndice < g_nNbUserPrivTags);


	return(-1);
}

int GetTlvPrivateType ( int nIndice )
{
	int nSize = sizeof(tab_TlvPrivateInfo)/sizeof(tab_TlvPrivateInfo[0]);

	if( nIndice < nSize )
		return ( tab_TlvPrivateInfo[nIndice].nType);

	nSize = sizeof(tab_user_TlvPrivateInfo)/sizeof(tab_user_TlvPrivateInfo[0]);

	if( nIndice < nSize )
		return ( tab_user_TlvPrivateInfo[nIndice - nSize].nType);
	return -1;
}

int GetTlvPrivateLength ( int nIndice )
{
	int nSize = sizeof(tab_TlvPrivateInfo)/sizeof(tab_TlvPrivateInfo[0]);

	if( nIndice < nSize )
		return ( tab_TlvPrivateInfo[nIndice].nLength);

	nSize = sizeof(tab_user_TlvPrivateInfo)/sizeof(tab_user_TlvPrivateInfo[0]);

	if( nIndice < nSize )
		return ( tab_user_TlvPrivateInfo[nIndice - nSize].nLength);
	return -1;
}

/*TODO: fix this as for autho_ini.c*/
/*EBE10527 Fixed
void AdjustTlvPrivate (TSTlvPrivate * tlvInfo, int nIndice, int nLength)
{
int i;
for (i = nIndice; i < MAX_PRIVATE_TLV; i++)
{
if ( tlvInfo->nPresent[i] == PRESENT)
tlvInfo->nPosTlv [i]  += nLength;
}

}
*/

void AdjustTlvPrivate (TSTlvPrivate * tlvInfo, int nIndice, int nLength)
{
	int i;
	for (i = 0; i < MAX_PRIVATE_TLV; i++)
	{
		if ( tlvInfo->nPresent[i] == PRESENT && tlvInfo->nPosTlv [i] > tlvInfo->nPosTlv [nIndice] )
			tlvInfo->nPosTlv [i]  += nLength;
	}

}


