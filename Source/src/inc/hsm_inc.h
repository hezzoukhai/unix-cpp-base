#ifndef HPS_HSM_INC_H
#define HPS_HSM_INC_H
#include <define.h>
#include <resources.h>

#define Validation_data 	"0123456789012345"
#define PINLENGTH             			4
#define PIN_LMK_LENGTH        	PINLENGTH + 1
#define PIN_LMK_MAX_LEN			13
#define SIZE_KEY                16

#define PG_DECIMALISATION_TABLE		"DECIMALISATION_TABLE"

#define   VERIFICATION_FAILED      1
#define   CRYPTOGRAPHIC_ERROR      2
#define   HSM_MALFUNCTION 		   3
#define   HSM_UNAVAILABLE          9


#define    DES_OPTION_TIRPLE      'T'
#define    DES_OPTION_SIMPLE      'S'

#define    KEY_LEN_SIMPLE      	   'S'
#define    KEY_LEN_DOUBLE     	   'D'

#define    TERMINAL_MODE			'T'
#define    SCHEME_MODE				'S'


#define   PIN_VERIFICATION_METHOD       '1'
#define   PVV_VERIFICATION_METHOD       '2'
#define   OFFSET_VERIFICATION_METHOD    '3'
#define   API_VERIFICATION_METHOD       '4'

#define   HSM_TIME_OUT_VALUE             5

#define HSM_MAC_OP_GEN_MAC					'0'
#define HSM_MAC_OP_CHECK_MAC				'1'

#define HSM_DATA_OP_ENC				'0'
#define HSM_DATA_OP_DEC				'1'


#define HSM_ENC_MODE_ECB			'0'
#define HSM_ENC_MODE_CBC			'1'
#define HSM_ENC_MODE_CFB8			'2'
#define HSM_ENC_MODE_CFB64			'3'


#define HSM_DATA_FMT_BINARY			'0'
#define HSM_DATA_FMT_HEX			'1'
#define HSM_DATA_FMT_TEXT			'2'





#define HSM_FORMAT_BINARY						'0'
#define	HSM_MAC_SIZE_8HEX_ALGO_ISO_9797_1		"01"
#define HSM_MAC_SIZE_8HEX_ALGO_ISO_9797_3               "03"/*ZKO PROD00070009 30082019*/
#define	HSM_PAD_MTHD_ISO_9797_1					'1'
#define HSM_KEY_TAK								"003"

#define HSM_FORMAT_TEXT                         '2'
#define HSM_ALGO_ISO_9797_3                     "03"
#define HSM_PAD_MTHD_ISO_9797_2					'2'
#define HSM_KEY_ZAK                             "008"
#define HSM_FORMAT_HEX							'1'

#define ERR_MAC_CHECK_FAILURE                      2


/*Thales*/
#define HSM_KEY_FLAG_ZEK			'0'
#define HSM_KEY_FLAG_ZAK			'1'

/*Thales*/
#define HSM_BDK_1				'0'
#define HSM_BDK_2				'1'
#define HSM_BDK_3				'2'
#define HSM_BDK_4				'3'

/*Thales: EY*/
#define HSM_HASH_ID_SHA_1					"01"
#define HSM_HASH_ID_MD5						"02"
#define HSM_HASH_ID_ISO_10118_2				"03"
#define HSM_HASH_ID_NO_HASH					"04"
#define HSM_HASH_ID_SHA_224					"05"
#define HSM_HASH_ID_SHA_256					"06"
#define HSM_HASH_ID_SHA_384					"07"
#define HSM_HASH_ID_SHA_512					"08"

#define HSM_SIG_ID_RSA						"01"

#define HSM_PAD_MODE_PKCS1_1_5				"01"		/* PKCS#1 v1.5 method (EME-PKCS1-v1_5)			*/
#define HSM_PAD_MODE_PKCS1_2_2				"02"		/* PKCS#1 v2.2 OAEP method (EME-OAEP-ENCODE)	*/

#define HSM_MGF_1							"01"		/* MGF1 as defined in PKCS#1 v2.2 (see Reference 3).	*/

/*GK*/
#define HSM_ENC_ID_RSA						"01"


/*Thales --NAB Key block 30012019 -- */
/*Key Usage   */
#define THALES_KU_TPK                       "71"
#define THALES_KU_TMK						"51"
#define THALES_KU_ZPK                       "72"
/*#define THALES_KU_TAK                       "M0"
#define THALES_KU_ZAK                       "M3"*/
#define ISO9797_1_ALGO3_MAC_KEY		    "M3"

/*Algorithm*/ 
#define THALES_ALGO_AES_1							"A1"  /*128-bit AES key*/
#define THALES_ALGO_AES_2							"A2"  /*192-bit AES key*/
#define THALES_ALGO_AES_3							"A3"  /*256-bit AES key*/
#define THALES_ALGO_DES_1							"D1"  /*single length DES key*/
#define THALES_ALGO_DES_2							"T2"  /*double length DES key*/
#define THALES_ALGO_DES_3							"T3"  /*triple length DES key*/

/*Mode of Use  */
#define THALES_MU_NO_RESTRICT                       "N"
#define THALES_MU_GENERATION                        "S"
#define THALES_MU_ENCRYPTION                        "E"
#define THALES_MU_ENCRYPT_DECRYPT                   "B"
#define THALES_MU_MAC_CALCULAT                      "C"
#define THALES_MU_MAC_GENERAT                       "G"
#define THALES_MU_VERIFICATION                      "V"
#define THALES_MU_DERIVATION                        "X"


/*Key Version Number 00 to 99 */

#define THALES_VN_00                               "00"

/*Exportability*/

#define THALES_EX_E                                "E"  /*May only be exported in a trusted key block, provided the wrapping key itself is in a trusted format*/
#define THALES_EX_N                                "N"  /*No export permitted*/
#define THALES_EX_S                                "S"  /*Sensitive; all other export possibilities are permitted, provided such export has been enabled*/




typedef struct s_pin_form 
{
      char   field0          [ 24];
      char   field1          [ 40];
      char   field2          [ 30];
      char   field3          [ 90];
      char   field4          [ 70];
      char   field5          [  7];
      char   field6          [  7];
}t_pin_form;

typedef struct s_fabrication 
{
  	char  pan  	      [22];
  	char  expiry_date     [ 4];
  	char  service_code    [ 3];
  	char  cvka            [16];
  	char  cvkb            [16];
  	char  pvka            [16];
  	char  pvkb            [16];
  	char  pvki            [ 1];
  	char  pvv             [ 4];
  	char  cvc1            [ 3];
  	char  cvc2            [ 3];
  	t_pin_form            pin_form;
}t_fabrication;



#define THALES_KEY_TP_ZMK              "000"	/* Zone Master Key (also known as ZCMK)				*/
#define THALES_KEY_TP_ZMKC             "100"	/* Zone Master Key Component (legacy commands only)	*/
#define THALES_KEY_TP_KML              "200"	/* Master Load Key (Visa Cash)						*/
#define THALES_KEY_TP_ZPK              "001"	/* Zone PIN Key										*/
#define THALES_KEY_TP_PVK              "002"	/* PIN Verification Key								*/
#define THALES_KEY_TP_TMK              "002"	/* Terminal Master Key								*/
#define THALES_KEY_TP_TPK              "002"	/* Terminal PIN Key									*/
#define THALES_KEY_TP_CVK              "402"	/* Card Verification Key								*/
#define THALES_KEY_TP_CSCK             "402"	/* Card Security Code Key								*/
#define THALES_KEY_TP_TAK              "003"	/* Terminal Authentication Key						*/
#define THALES_KEY_TP_WWK              "006"	/* Watchword Key										*/
#define THALES_KEY_TP_ZAK              "008"	/* Zone Authentication Key							*/
#define THALES_KEY_TP_BDK              "009"	/* Base Derivation Key Type 1							*/
#define THALES_KEY_TP_MK_AC            "109"	/* Master Key for Application Cryptograms				*/
#define THALES_KEY_TP_MK_SMI           "209"	/* Master Key for Secure Messaging (for Integrity)	*/
#define THALES_KEY_TP_MK_SMC           "309"	/* Master Key for Secure Messaging (for Confidentiality)	*/
#define THALES_KEY_TP_MK_DAC           "409"	/* Master Key for Data Authentication Codes			*/
#define THALES_KEY_TP_MK_DN            "509"	/* Master Key for Dynamic Numbers						*/
#define THALES_KEY_TP_BDK2             "609"	/* Base Derivation Key Type 2							*/
#define THALES_KEY_TP_MK_CVC3          "709"	/* Master Key for CVC3 (Contactless)					*/
#define THALES_KEY_TP_ZEK              "00A"	/* Zone Encryption Key								*/
#define THALES_KEY_TP_DEK              "00B"	/* Data Encryption Key								*/

/******************************************************************************/
/*PROD00068163: Start*/
#define EMV_MKD_A				'A'
#define EMV_MKD_B				'B'
#define EMV_MKD_C				'C'

#define EMV_SKD_NONE			'0'
#define EMV_SKD_EMV2000			'1'
#define EMV_SKD_CSK				'2'
#define EMV_SKD_MCI_SKD			'3'
#define EMV_SKD_UPI_SKD			'4'
#define EMV_SKD_JCB_SKD			'5'

#define EMV_ARPC_MTHD_1			'1'
#define EMV_ARPC_MTHD_2			'2'

#define EMV_CVN_01				"01"
#define EMV_CVN_02				"02"
#define EMV_CVN_04				"04"
#define EMV_CVN_05				"05"
#define EMV_CVN_06				"06"
#define EMV_CVN_10				"0A"
#define EMV_CVN_12				"0C"
#define EMV_CVN_13				"0D"
#define EMV_CVN_14				"0E"
#define EMV_CVN_15				"0F"
#define EMV_CVN_17				"11"
#define EMV_CVN_18				"12"
#define EMV_CVN_LEN				2

#define EMV_OFST_ARPC_MTHD		10
#define EMV_OFST_MKD			11
#define EMV_OFST_SKD			12


#define HTC_KW_SC_MK_A_SK_EMV2000			'0'		/* EMV Option 'A' Card Key Derivation and EMV2000 Session Key Derivation 			*/
#define HTC_KW_SC_MK_B_SK_EMV2000			'1'		/* EMV Option 'B' Card Key Derivation and EMV2000 Session Key Derivation			*/
#define HTC_KW_SC_MK_A_SK_CSK				'2'		/* EMV Option 'A' Card Key Derivation and EMV Common Session Key Derivation 		*/
#define HTC_KW_SC_MK_B_SK_CSK				'3'		/* EMV Option 'B' Card Key Derivation and EMV Common Session Key Derivation			*/
#define HTC_KW_SC_MK_C_SK_CSK				'9'		/* EMV Option 'C' Card Key Derivation and EMV Common Session Key Derivation			*/
#define HTC_KW_SC_MK_A_SK_NONE				'A'		/* JCB (CVN 01): EMV Option 'A' Card Key Derivation and no Session Key Derivation	*/
#define HTC_KW_SC_MK_A_SK_JCB				'B'		/* JCB (CVN 02): EMV Option 'A' Card Key Derivation and JCB Session Key Derivation	*/
#define HTC_KW_SC_MK_A_SK_UPI				'C'		/* UPI(4.2): EMV Option 'A' Card Key Derivation and Union Pay Session Key Derivation*/

#define HTC_KQ_SC_MK_A_SK_NONE				'0'		/* EMV Option 'A' ICC Master Key Derivation (Visa VIS) 								*/
#define HTC_KQ_SC_MK_A_SK_MCI_SKD			'1'		/* EMV Option 'A' ICC Master Key Derivation and Mastercard Proprietary SKD (M/Chip)	*/

/*PROD00068163: End*/

/*Key Generate*/
int GenerateTPKKey( int nIndexCtx,char *pTermNbr,char* sAcqBank, char *pTpkLmk,char *pTpkTmk, char* szCheckValue);
int	GeneratePosIsoBDKKey(int nIndexCtx,char *pResourceId, char *pTermNbr, char *pBdkLmk, char *pBdkZmk, char *pCheckValue);
int GenerateTMKKey(int nIndexCtx,char *pTermNbr,char* sAcqBank, char *pTmkLmk,char *pTmkTmk, char* szCheckValue);
int GenerateMACKey(int nIndexCtx,char *pTermNbr,char* sAcqBank, char *pMacLmk,char *pMacTmk, char* szCheckValue);
int GenerateZPK( int nIndexCtx,char sDesOption,char* szZmkLmk,char* szZpkLmk,char* szZpkZmk,char* szCheckValue,char* keySchemeZmk,char*  KeyBlockVersionId);/* AHA20220928 PLUTONL-4980*/
int GenerateResourceZPK(int nIndexCtx,char sDesOption,char* sResourceId,char* szZpkLmk,char* szZpkZmk,char* szCheckValue,char* keySchemeZmk,char*  KeyBlockVersionId);/* AHA20220928 PLUTONL-4980*/
int GenerateResourceTAK(int nIndexCtx,char sDesOption,char *pResourceId,char *pTakLmk,char *pTakZmk,char *pCheckValue);

int GetHsmDecTable(char* sDecTable);





#endif /*** HPS_HSM_INC_H ***/
