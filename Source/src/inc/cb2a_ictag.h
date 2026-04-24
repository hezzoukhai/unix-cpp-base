#ifndef HPS_ISO_ICTAG_H
#define HPS_ISO_ICTAG_H

#define    ISO_TAG_ISS_SCRIPT1         "7100"
#define    ISO_TAG_ISS_SCRIPT2         "7200"
#define    ISO_TAG_ISS_SCRIPT_CMD      "8600"
#define    ISO_TAG_APP_INTER_PROFILE   "8200"
#define    ISO_TAG_DED_FILE_NAME       "8400"
#define    ISO_TAG_ICC_APP_ID          "4F00"
#define    ISO_TAG_TERM_CAP_PROFILE    "9F33"
#define    ISO_TAG_AUTHOR_RESP_CODE    "8A00"
#define    ISO_TAG_ISS_AUTHEN_DATA     "9100"
#define    ISO_TAG_TVR                 "9500"
#define    ISO_TAG_TRANS_DATE          "9A00"
#define    ISO_TAG_TRANS_TYPE          "9C00"
#define    ISO_TAG_TRANS_CUR_CODE      "5F2A"
#define    ISO_TAG_TRANS_AMOUNT        "9F02"
#define    ISO_TAG_OTHER_AMOUNT        "9F03"
#define    ISO_TAG_TERM_APP_VER_NUM    "9F09"
#define    ISO_TAG_ISS_APP_DATA        "9F10"
#define    ISO_TAG_TERM_COUNTRY_CODE   "9F1A"
#define    ISO_TAG_IFD_SERIAL_NUM      "9F1E"
#define    ISO_TAG_APP_CRYPTOGRAM      "9F26"
#define    ISO_TAG_CRYPTO_INFO_DATA    "9F27"
#define    ISO_TAG_TERM_CAP            "9F33"
#define    ISO_TAG_CVM                 "9F34"
#define    ISO_TAG_TERM_TYPE           "9F35"
#define    ISO_TAG_ATC                 "9F36"
#define    ISO_TAG_UNPRED_NUMBER       "9F37"
#define    ISO_TAG_TRANS_SEQ_NUM       "9F41"
#define    ISO_TAG_TRANS_CATEG_CODE    "9F53"
#define    ISO_TAG_APP_SEQ_NUMBER      "5F34"
#define    ISO_TAG_ARPC                "9F54"
/* *****************************TAG FIELD 31************************************ */
#define    ISO_TAG_DATA_PRINT          "01  " /* SUBFIELD : 01   FIELD 31 */
/* ABE: Add tag for the field 44*/
/* *****************************TAG FIELD 44************************************ */
#define    ISO_TAG_WRONG_FILED          "AA  " /* SUBFIELD : AA   FIELD 44 */
#define    ISO_TAG_CONV_FILED          	"AC  " /* SUBFIELD : AC   FIELD 44 */
#define    ISO_TAG_MSG_TRX_INTERN       "BC  " /* SUBFIELD : BC   FIELD 44 */
#define    ISO_TAG_INFO_CVV_CVC         "CA  " /* SUBFIELD : CA   FIELD 44 */
#define    ISO_TAG_INFO_CTRL_CRYPT      "CB  " /* SUBFIELD : CB   FIELD 44 */

/* *****************************TAG FIELD 46************************************ */
#define    ISO_TAG_NUM_CONTRAT_ACCEPTEUR 	"DF5F" /* SUBFIELD : DF5F FIELD 46 */
/* ABE: Add more tag for the field 46 */
#define    ISO_TAG_TERMINAL_ID 				"DF50" /* SUBFIELD : DF50 FIELD 46 */
#define    ISO_TAG_SYS_ACCEPTOR_NUM 		"DF51" /* SUBFIELD : DF51 FIELD 46 */
#define    ISO_TAG_ARCH_ACCEPTOR_TYPE 		"DF52" /* SUBFIELD : DF52 FIELD 46 */
#define    ISO_TAG_APPLICATION_STATUS 		"DF54" /* SUBFIELD : DF54 FIELD 46 */
#define    ISO_TAG_LOGIC_NBR_PA 			"DF5B" /* SUBFIELD : DF5B FIELD 46 */
#define    ISO_TAG_IDPA 					"DF5C" /* SUBFIELD : DF5C FIELD 46 */
#define    ISO_TAG_SIRET 					"DF5D" /* SUBFIELD : DF5D FIELD 46 */
#define    ISO_TAG_IDSA 					"DF5E" /* SUBFIELD : DF5E FIELD 46 */
#define    ISO_TAG_CONTRACT_NUM_ACCPT 		"DF5F" /* SUBFIELD : DF5F FIELD 46 */
#define    ISO_TAG_REMETTANT_RGP_CODE 		"DF61" /* SUBFIELD : DF61 FIELD 46 */

/* *****************************TAG FIELD 47************************************ */
/* ABE: Add more tag for the field 47 */
#define    ISO_TAG_AUTHO_RAISON       	"01  " /* SUBFIELD : 01 FIELD 47 */
#define    ISO_TAG_TRANS_TECHNIQUE      "02  " /* SUBFIELD : 02 FIELD 47 */
#define    ISO_TAG_BILLING_TYPE       	"03  " /* SUBFIELD : 03 FIELD 47 */
#define    ISO_TAG_AUTHO_DATE       	"04  " /* SUBFIELD : 04 FIELD 47 */
#define    ISO_TAG_FORCAGE_CODE_TRX     "05  " /* SUBFIELD : 05 FIELD 47 */
#define    ISO_TAG_YEAR                	"07  " /* SUBFIELD : 07 FIELD 47 */
#define    ISO_TAG_TRANS_NUMBRE_KO     	"09  " /* SUBFIELD : 09 FIELD 47 */
#define    ISO_TAG_TRANS_NUMBRE       	"10  " /* SUBFIELD : 10 FIELD 47 */
#define    ISO_TAG_VALID_BEGIN_DATE     "15  " /* SUBFIELD : 15 FIELD 47 */
#define    ISO_TAG_CODE_COUNTRY       	"18  " /* SUBFIELD : 18 FIELD 47 */
#define    ISO_TAG_REF_NUMBRE       	"24  " /* SUBFIELD : 24 FIELD 47 */
#define    ISO_TAG_CODE_ANOMALIE       	"47  " /* SUBFIELD : 47 FIELD 47 */
#define    ISO_TAG_APPLICATIF_TYPE      "48  " /* SUBFIELD : 48 FIELD 47 */

/* ABE: Add tag for the field 55*/
/* *****************************TAG FIELD 55************************************ */
#define    ISO_TAG_CHIP_TRX_TYPE        "0082" /* SUBFIELD : 0082 FIELD 55 */
#define    ISO_TAG_LIST_CVM        		"008E" /* SUBFIELD : 008E FIELD 55 */
#define    ISO_TAG_APP_EXPIRY_DATE      "5F24" /* SUBFIELD : 5F24 FIELD 55 */
#define    ISO_TAG_APP_EFFECTIV_DATE    "5F25" /* SUBFIELD : 5F25 FIELD 55 */
#define    ISO_TAG_APP_IDENTIFIER     	"9F06" /* SUBFIELD : 9F06 FIELD 55 */
#define    ISO_TAG_APP_USAGE_CTRL       "9F07" /* SUBFIELD : 9F07 FIELD 55 */
#define    ISO_TAG_ISSUER_ACT_DEF       "9F0D" /* SUBFIELD : 9F0D FIELD 55 */
#define    ISO_TAG_ISSUER_ACT_DEN       "9F0E" /* SUBFIELD : 9F0E FIELD 55 */
#define    ISO_TAG_ISSUER_ACT_ON        "9F0F" /* SUBFIELD : 9F0F FIELD 55 */
#define    ISO_TAG_ISSUER_APP_DATA      "9F10" /* SUBFIELD : 9F10 FIELD 55 */
#define    ISO_TAG_APP_CRYPTOGRAME      "9F26" /* SUBFIELD : 9F26 FIELD 55 */
#define    ISO_TAG_CRYPT_INFO_DATA      "9F27" /* SUBFIELD : 9F27 FIELD 55 */	
#define    ISO_TAG_APP_TRX_COUNTER      "9F36" /* SUBFIELD : 9F36 FIELD 55 */
#define    ISO_TAG_CHIP_UNPRED_NBR      "DF70" /* SUBFIELD : DF70 FIELD 55 */
#define    ISO_TAG_CALC_CERTIF_PRDCT    "DF71" /* SUBFIELD : DF71 FIELD 55 */
#define    ISO_TAG_PRDCT_CERTIF_PA      "DF72" /* SUBFIELD : DF72 FIELD 55 */
#define    ISO_TAG_CARD_APP_TYPE        "DF73" /* SUBFIELD : DF73 FIELD 55 */
#define    ISO_TAG_CARD_APP_CRNCY       "DF74" /* SUBFIELD : DF74 FIELD 55 */
#define    ISO_TAG_ISSUER_CAI           "DF75" /* SUBFIELD : DF75 FIELD 55 */
#define    ISO_TAG_DATA_CARD_ISSUER     "DF76" /* SUBFIELD : DF76 FIELD 55 */
#define    ISO_TAG_ACQUIRER_CAI         "DF77" /* SUBFIELD : DF77 FIELD 55 */		
#define    ISO_TAG_DATA_CARD_ACQ        "DF78" /* SUBFIELD : DF78 FIELD 55 */
#define    ISO_TAG_VERIF_CRYPTO_MODE    "DFAB" /* SUBFIELD : DFAB FIELD 55 */
#define    ISO_TAG_ISSUER_SCRIPT_RES    "FF00" /* SUBFIELD : FF00 FIELD 55 */

/* *****************************TAG FIELD 57************************************ */
#define    ISO_TAG_CARRIER_LIB1         "FF01" /* SUBFIELD : FF01 FIELD 57 */
#define    ISO_TAG_CARRIER_LIB2        	"FF02" /* SUBFIELD : FF02 FIELD 57 */
#define    ISO_TAG_DEST_AIRLINE_TICKET  "FF0E" /* SUBFIELD : FF0E FIELD 57 */
#define    ISO_TAG_ID_AIRLINE_TICKET    "FF0F" /* SUBFIELD : FF0F FIELD 57 */

/* *****************************TAG FIELD 58************************************ */
#define    ISO_TAG_CONVERSION_RATE        "FF31" /* SUBFIELD : FF31 FIELD 58 */
#define    ISO_TAG_SUPPORT_TYPE           "FF52" /* SUBFIELD : FF52 FIELD 58 */
#define    ISO_TAG_GESTION_TRANSFERT      "FF53" /* SUBFIELD : FF53 FIELD 58 */
#define    ISO_TAG_POTENTIEL_RECYCLAGE    "FF54" /* SUBFIELD : FF54 FIELD 58 */
#define    ISO_TAG_FILE_DATA              "FF55" /* SUBFIELD : FF55 FIELD 58 */
#define    ISO_TAG_REMITTANCE_ID          "FF56" /* SUBFIELD : FF56 FIELD 58 */
#define    ISO_TAG_DATE_LIMITE_RECYCLAGE  "FF60" /* SUBFIELD : FF60 FIELD 58 */
#define    ISO_TAG_FICHIER_INFO_INITIAUX  "FF61" /* SUBFIELD : FF61 FIELD 58 */
#define    ISO_TAG_SERVICE_INFO_INITIAUX  "FF62" /* SUBFIELD : FF62 FIELD 58 */
#define    ISO_TAG_REF_ACQ_REMETTANT      "FF63" /* SUBFIELD : FF63 FIELD 58 */
#define    ISO_TAG_REF_REMETTANT_ACQ      "FF64" /* SUBFIELD : FF64 FIELD 58 */
#define    ISO_TAG_LOT_INFO_INITIAUX      "FF65" /* SUBFIELD : FF65 FIELD 58 */
/* ABE: Add more tag for the field 58 */
#define    ISO_TAG_TRML_VERIF_RES		  "0095" /* SUBFIELD : 0095 FIELD 58 */
#define    ISO_TAG_TRANS_TYPE        	  "009C" /* SUBFIELD : 009C FIELD 58 */
#define    ISO_TAG_INFO_CRYPT_CMC         "0409" /* SUBFIELD : 0409 FIELD 58 */
#define    ISO_TAG_ISS_AUTH_METHOD_CH     "0410" /* SUBFIELD : 0410 FIELD 58 */
#define    ISO_TAG_CRYPT_CALC_CMC         "0411" /* SUBFIELD : 0411 FIELD 58 */
#define    ISO_TAG_ARCH_SETL_VADS_RES     "0412" /* SUBFIELD : 0412 FIELD 58 */
#define    ISO_TAG_TRML_CAPABILITY        "0433" /* SUBFIELD : 0433 FIELD 58 */
#define    ISO_TAG_CVM_RESULT        	  "0434" /* SUBFIELD : 0434 FIELD 58 */
#define    ISO_TAG_TRML_UNPREDICTABLE     "0437" /* SUBFIELD : 0437 FIELD 58 */
#define    ISO_TAG_TRML_ACT_DEFAULT       "FF0D" /* SUBFIELD : FF0D FIELD 58 */
#define    ISO_TAG_TRML_ACT_DENIAL        "FF0E" /* SUBFIELD : FF0E FIELD 58 */
#define    ISO_TAG_TRML_ACT_ONLINE        "FF0F" /* SUBFIELD : FF0F FIELD 58 */
#define    ISO_TAG_NBR_TOT_TRX_D_DIF      "FF35" /* SUBFIELD : FF35 FIELD 58 */
#define    ISO_TAG_AMNT_TOT_TRX_D_DIF     "FF36" /* SUBFIELD : FF36 FIELD 58 */
#define    ISO_TAG_IDTEL_PAYMNT_ACQ       "FF45" /* SUBFIELD : FF45 FIELD 58 */
#define    ISO_TAG_ACCEPT_CERTIF          "FF46" /* SUBFIELD : FF46 FIELD 58 */
#define    ISO_TAG_TRX_SECUR_TYPE         "FF47" /* SUBFIELD : FF47 FIELD 58 */
#define    ISO_TAG_TRX_ID_ACCEPT          "FF48" /* SUBFIELD : FF48 FIELD 58 */
#define    ISO_TAG_CMC_ELECTR_CRYPT       "FF49" /* SUBFIELD : FF49 FIELD 58 */
#define    ISO_TAG_NBR_TOT_TRX_D_REV      "FF58" /* SUBFIELD : FF58 FIELD 58 */
#define    ISO_TAG_AMNT_TOT_TRX_D_REV     "FF59" /* SUBFIELD : FF59 FIELD 58 */


/* *****************************TAG FIELD 118************************************ */
#define    ISO_TAG_MODE_OPERATOIRE        "0A  " /* SUBFIELD : 0A   FIELD 118 */
#define    ISO_TAG_NATURE_LOTS            "0B  " /* SUBFIELD : 0B   FIELD 118 */
#define    ISO_TAG_NOMBRE_LOTS_INIT       "0C  " /* SUBFIELD : 0B   FIELD 118 */


#endif /** HPS_ISO_ICTAG_H **/
