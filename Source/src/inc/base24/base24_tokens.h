#ifndef BASE24_TOKENS__H
#define BASE24_TOKENS__H

/*
* Note: B24_TK_ enums must follow same order as they are declared in tab_Token_XX_InfoType[]
*/

/*#define B24_TOKENS_HEADER_LEN					0*/
#define B24_TOKENS_HEADER_EYE_CATCHER			0
#define B24_TOKENS_HEADER_USR_FLD_1				1
#define B24_TOKENS_HEADER_COUNT					2
#define B24_TOKENS_HEADER_LENGTH				7

#define B24_TOKENS_HEADER_SZ					12
#define B24_TOKENS_HEADER_COUNT_SZ				5
#define B24_TOKENS_HEADER_LENGTH_SZ				5
#define B24_TOKENS_LEN_IND_SIZE					3
/**************************************************/
#define B24_TOKEN_HEADER_EYE_CATCHER			0
#define B24_TOKEN_HEADER_USR_FLD_1				1
#define B24_TOKEN_HEADER_ID						2
#define B24_TOKEN_HEADER_LENGTH					4
#define B24_TOKEN_HEADER_USR_FLD_2				9

#define B24_TOKEN_HEADER_SZ						10
#define B24_TOKEN_HEADER_ID_SZ					2
#define B24_TOKEN_HEADER_LENGTH_SZ				5
/**************************************************/
#define B24_MAX_NB_TOKENS						48
#define B24_MAX_LEN_TOKENS						1024

#define B24_MAX_TOKEN_NB_ITEMS					128

#define B24_TOKEN_ID_LEN						2


#define B24_TOKEN_BM			"BM"
#define B24_TOKEN_BP			"BP"
#define B24_TOKEN_B2			"B2"
#define B24_TOKEN_B3			"B3"
#define B24_TOKEN_B4			"B4"
#define B24_TOKEN_B5			"B5"
#define B24_TOKEN_B6			"B6"
#define B24_TOKEN_BJ			"BJ"
/* POS Issuer */
#define B24_TOKEN_01			"01"
#define B24_TOKEN_04			"04"
#define B24_TOKEN_C0			"C0"
#define B24_TOKEN_C4			"C4"
#define B24_TOKEN_BE			"BE"
/* ATM */
#define B24_TOKEN_PF			"PF"
#define B24_TOKEN_PG			"PG"
#define B24_TOKEN_P1			"P1"
#define B24_TOKEN_06			"06"
#define B24_TOKEN_PZ			"PZ"
#define B24_TOKEN_A5			"A5"

/*NCCC*/
#define B24_TOKEN_17			"17"
#define B24_TOKEN_21			"21"	/*EVT*/
#define B24_TOKEN_25			"25"
#define B24_TOKEN_B0			"B0"
#define B24_TOKEN_BY			"BY"
#define B24_TOKEN_C5			"C5"
#define B24_TOKEN_C6			"C6"
#define B24_TOKEN_CE			"CE"
#define B24_TOKEN_CH			"CH"
#define B24_TOKEN_S8			"S8"
#define B24_TOKEN_20			"20"

/*Evertec */
#define B24_TOKEN_CK			"CK"
#define B24_TOKEN_PP			"PP"

/* aha20150428 cost of transaction */
#define B24_TOKEN_QT			"QT"

/*START NAB08102020 PLUTONL-2288*/
#define B24_TOKEN_Q1            "Q1"
#define B24_TOKEN_Q2			"Q2"
#define B24_TOKEN_Q6			"Q6"
#define B24_TOKEN_CZ			"CZ"
#define B24_TOKEN_R4			"R4"
#define B24_TOKEN_RJ			"RJ"

/*END NAB08102020 PLUTONL-2288*/


enum {
	B24_TOKEN_BM_ID		= 0	,
	B24_TOKEN_BP_ID			,
	B24_TOKEN_B2_ID			,
	B24_TOKEN_B3_ID			,
	B24_TOKEN_B4_ID			,
	B24_TOKEN_B5_ID			,
	B24_TOKEN_B6_ID			,
	B24_TOKEN_BJ_ID			,
	B24_TOKEN_01_ID			,
	B24_TOKEN_04_ID			,
	B24_TOKEN_C0_ID			,
	B24_TOKEN_C4_ID			,
	B24_TOKEN_BE_ID			,
	B24_TOKEN_PF_ID			,
	B24_TOKEN_PG_ID			,
	B24_TOKEN_P1_ID			,
	B24_TOKEN_06_ID			,
	B24_TOKEN_PZ_ID			,
	B24_TOKEN_A5_ID			,

	B24_TOKEN_17_ID			,
	B24_TOKEN_21_ID			,
	B24_TOKEN_25_ID			,
	B24_TOKEN_B0_ID			,
	B24_TOKEN_BY_ID			,
	B24_TOKEN_C5_ID			,
	B24_TOKEN_C6_ID			,
	B24_TOKEN_CE_ID			,
	B24_TOKEN_CH_ID			,
	B24_TOKEN_S8_ID			,

	B24_TOKEN_CK_ID			,
	B24_TOKEN_PP_ID			,
	B24_TOKEN_20_ID			,

	B24_TOKEN_QT_ID			,
/*START NAB08102020 PLUTONL-2288*/
	B24_TOKEN_Q1_ID			,
	B24_TOKEN_Q2_ID			,
	B24_TOKEN_Q6_ID			,
	B24_TOKEN_CZ_ID			,
	B24_TOKEN_R4_ID			,
	B24_TOKEN_RJ_ID			,
/*END NAB08102020 PLUTONL-2288*/

	B24_TOKEN_ID_QTY
};


/******************************************************************************/
/******************************************************************************\ 
* BM – Transaction Subtype Token
* It contains values used to indicate that the processing associated with 
* particular transaction code should be altered. 
* It identifies the type of transaction is being processed.
\******************************************************************************/
#define B24_TK_BM_OFF_TXN_SUBTYPE				0		/* This is a subtype identifier to further describe this transaction. */
#define B24_TK_BM_OFF_ACQ_PROC_CODE				4		/* It contains the acquirer’s processing code. */
#define B24_TK_BM_OFF_ISS_PROC_CODE				10		/* It contains the issuer’s processing code */
#define B24_TK_BM_OFF_USR_FDL_1					16		/* This field is not used. */
#define B24_TK_BM_OFF_THIRD_PARTY_PHONE_NO		16
#define B24_TK_BM_OFF_RECIPIENT_PHONE_NO		26
#define B24_TK_BM_SIZE							36


enum {
	B24_TK_BM_TXN_SUBTYPE				=0	,
	B24_TK_BM_ACQ_PROC_CODE					,
	B24_TK_BM_ISS_PROC_CODE					,
	B24_TK_BM_USR_FDL_1						,
	B24_TK_BM_THIRD_PARTY_PHONE_NO			,
	B24_TK_BM_RECIPIENT_PHONE_NO			,
};

/******************************************************************************\ 
* BP – Person-to-Person transaction token
\******************************************************************************/
#define B24_TK_BP_OFF_INTENDED_USE_FLAG			0		/* It identifies the intended use of the Person-to-Person transaction.*/
#define B24_TK_BP_OFF_BUSINESS_USE_FLAG			1		/* It identifies the business use of the Person-to-Person transaction. */
#define B24_TK_BP_OFF_TRACE_NUMBER				2		/* It is the trace number assigned by the acquirer. This number is unique. */
#define B24_TK_BP_OFF_THIRD_PARTY_ID			17		/* It contains the identity of the third party involved in the Person-to-Person transaction. 
														The identification of the receiver, sender, payer and payee is populated by 
														the acquirer of the transaction. */
#define B24_TK_BP_OFF_THIRD_PARTY_ACC_NO		77		/* It includes the billing account number for the third party involved in the Person-to-Person transaction.*/
#define B24_TK_BP_OFF_THIRD_PARTY_PHONE_NO		102		/* It is a code to identify the third party’s phone or pager number */
#define B24_TK_BP_OFF_THIRD_PARTY_INVOICE_NO	127		/* It contains the invoice number associated with the third party. */
#define B24_TK_BP_OFF_THIRD_PARTY_EMAIL_ADDR	152		/* This is the third party’s e-mail address */
#define B24_TK_BP_OFF_THIRD_PARTY_DESCRIPTION	212		/* It includes a description of the third party involved in the Person-to-Person transaction. */
#define B24_TK_BP_OFF_RECIPIENT_EMAIL_ADDR		262		/* It is the recipient’s e-mail address */
#define B24_TK_BP_OFF_RECIPIENT_PHONE_NO		321		/* It contains the recipient’s phone, pager, and fax number. */
#define B24_TK_BP_OFF_RECIPIENT_USR_FLD_1		362		/* This is a short, free-form message from the sender to the receiver used in interbank transfer transactions. */
#define B24_TK_BP_SIZE							382

enum {
	B24_TK_BP_INTENDED_USE_FLAG				=0	,
	B24_TK_BP_BUSINESS_USE_FLAG					,
	B24_TK_BP_TRACE_NUMBER						,
	B24_TK_BP_THIRD_PARTY_ID					,
	B24_TK_BP_THIRD_PARTY_ACC_NO				,
	B24_TK_BP_THIRD_PARTY_PHONE_NO				,
	B24_TK_BP_THIRD_PARTY_INVOICE_NO			,
	B24_TK_BP_THIRD_PARTY_EMAIL_ADDR			,
	B24_TK_BP_THIRD_PARTY_DESCRIPTION			,
	B24_TK_BP_RECIPIENT_EMAIL_ADDR				,
	B24_TK_BP_RECIPIENT_PHONE_NO				,
	B24_TK_BP_RECIPIENT_USR_FLD_1				,
};
/******************************************************************************\ 
* B2 - The EMV Request Token B2 contains the thirteen minimum request 
* data elements required for inclusion in request messages
\******************************************************************************/
#define B24_TK_B2_OFF_BITMAP					0		/* "7FF9" Indicates whether data in each of the remaining fields in the token is present or absent.*/
#define B24_TK_B2_OFF_USR_FLD_1					4		/* N/A Zeroes */
#define B24_TK_B2_OFF_CRYPTO_INFO_DATA			8		/* EMV Tag 9F27 */
#define B24_TK_B2_OFF_TVR						10		/* EMV Tag 95 */
#define B24_TK_B2_OFF_ARQC						20		/* EMV Tag 9F26 */
#define B24_TK_B2_OFF_AMOUNT_AUTH				36		/* EMV Tag 9F02 */
#define B24_TK_B2_OFF_AMOUNT_OTHER				48		/* EMV Tag 9F03 */
#define B24_TK_B2_OFF_AIP						60		/* EMV Tag 82 */
#define B24_TK_B2_OFF_ATC						64		/* EMV Tag 9F36 */
#define B24_TK_B2_OFF_TERM_COUNTRY_CODE			68		/* EMV Tag 9F1A */
#define B24_TK_B2_OFF_TRX_CURRENCY_CODE			71		/* EMV Tag 5F2A */
#define B24_TK_B2_OFF_TRX_DATE					74		/* EMV Tag 9A */
#define B24_TK_B2_OFF_TRX_TYPE					80		/* EMV Tag 9C */
#define B24_TK_B2_OFF_UN						82		/* EMV Tag 9F37 */
#define B24_TK_B2_OFF_ISS_APPL_DATA_LEN			90		/* Length in bytes of the actual data in field ISS-APPL-DATA. */
#define B24_TK_B2_OFF_ISS_APPL_DATA				94		/* EMV Tag 9F10 (Left justified, padded with zeroes to the right) */

#define B24_TK_B2_OFF_VISA_IAD					94
#define B24_TK_B2_OFF_VISA_IAD_LEN				94
#define B24_TK_B2_OFF_VISA_IAD_DKI				96
#define B24_TK_B2_OFF_VISA_IAD_CVN				98
#define B24_TK_B2_OFF_VISA_IAD_CVR				100
#define B24_TK_B2_OFF_VISA_IAD_INFO				108

#define B24_TK_B2_OFF_MCHIP_SEL2_IAD			94
#define B24_TK_B2_OFF_MCHIP_SEL2_IAD_LEN		94
#define B24_TK_B2_OFF_MCHIP_SEL2_IAD_DKI		96
#define B24_TK_B2_OFF_MCHIP_SEL2_IAD_CVN		98
#define B24_TK_B2_OFF_MCHIP_SEL2_IAD_CVR		100
#define B24_TK_B2_OFF_MCHIP_SEL2_IAD_DAC		108
#define B24_TK_B2_OFF_MCHIP_SEL2_IAD_INFO		112

#define B24_TK_B2_OFF_MCHIP_LT21_IAD			94
#define B24_TK_B2_OFF_MCHIP_LT21_IAD_DKI		94
#define B24_TK_B2_OFF_MCHIP_LT21_IAD_CVN		96
#define B24_TK_B2_OFF_MCHIP_LT21_IAD_CVR		98
#define B24_TK_B2_OFF_MCHIP_LT21_IAD_DAC		106
#define B24_TK_B2_OFF_MCHIP_LT21_IAD_INFO		110

#define B24_TK_B2_OFF_JCB_IAD					94
#define B24_TK_B2_OFF_JCB_IAD_LEN				94
#define B24_TK_B2_OFF_JCB_IAD_DKI				96
#define B24_TK_B2_OFF_JCB_IAD_CVN				98
#define B24_TK_B2_OFF_JCB_IAD_CVR				100
#define B24_TK_B2_OFF_JCB_IAD_INFO				110

#define B24_TK_B2_OFF_MCHIP4_IAD				94
#define B24_TK_B2_OFF_MCHIP4_IAD_DKI			94
#define B24_TK_B2_OFF_MCHIP4_IAD_CVN			96
#define B24_TK_B2_OFF_MCHIP4_IAD_CVR			98
#define B24_TK_B2_OFF_MCHIP4_IAD_DAC			110
#define B24_TK_B2_OFF_MCHIP4_IAD_CNTR			114
#define B24_TK_B2_OFF_MCHIP4_IAD_INFO			130
#define B24_TK_B2_SIZE							158


enum {
	B24_TK_B2_BITMAP					=0	,
	B24_TK_B2_USR_FLD_1						,
	B24_TK_B2_CRYPTO_INFO_DATA				,
	B24_TK_B2_TVR							,
	B24_TK_B2_ARQC							,
	B24_TK_B2_AMOUNT_AUTH					,
	B24_TK_B2_AMOUNT_OTHER					,
	B24_TK_B2_AIP							,
	B24_TK_B2_ATC							,
	B24_TK_B2_TERM_COUNTRY_CODE				,
	B24_TK_B2_TRX_CURRENCY_CODE				,
	B24_TK_B2_TRX_DATE						,
	B24_TK_B2_TRX_TYPE						,
	B24_TK_B2_UN							,
	B24_TK_B2_ISS_APPL_DATA_LEN				,
	B24_TK_B2_ISS_APPL_DATA					,
	B24_TK_B2_VISA_IAD						,
	B24_TK_B2_VISA_IAD_LEN					,
	B24_TK_B2_VISA_IAD_DKI					,
	B24_TK_B2_VISA_IAD_CVN					,
	B24_TK_B2_VISA_IAD_CVR					,
	B24_TK_B2_VISA_IAD_INFO					,
	B24_TK_B2_MCHIP_SEL2_IAD				,
	B24_TK_B2_MCHIP_SEL2_IAD_LEN			,
	B24_TK_B2_MCHIP_SEL2_IAD_DKI			,
	B24_TK_B2_MCHIP_SEL2_IAD_CVN			,
	B24_TK_B2_MCHIP_SEL2_IAD_CVR			,
	B24_TK_B2_MCHIP_SEL2_IAD_DAC			,
	B24_TK_B2_MCHIP_SEL2_IAD_INFO			,
	B24_TK_B2_MCHIP_LT21_IAD				,
	B24_TK_B2_MCHIP_LT21_IAD_DKI			,
	B24_TK_B2_MCHIP_LT21_IAD_CVN			,
	B24_TK_B2_MCHIP_LT21_IAD_CVR			,
	B24_TK_B2_MCHIP_LT21_IAD_DAC			,
	B24_TK_B2_MCHIP_LT21_IAD_INFO			,
	B24_TK_B2_JCB_IAD						,
	B24_TK_B2_JCB_IAD_LEN					,
	B24_TK_B2_JCB_IAD_DKI					,
	B24_TK_B2_JCB_IAD_CVN					,
	B24_TK_B2_JCB_IAD_CVR					,
	B24_TK_B2_JCB_IAD_INFO					,
	B24_TK_B2_MCHIP4_IAD					,
	B24_TK_B2_MCHIP4_IAD_DKI				,
	B24_TK_B2_MCHIP4_IAD_CVN				,
	B24_TK_B2_MCHIP4_IAD_CVR				,
	B24_TK_B2_MCHIP4_IAD_DAC				,
	B24_TK_B2_MCHIP4_IAD_CNTR				,
	B24_TK_B2_MCHIP4_IAD_INFO				,
};


enum {
	B24_BITMAP_TK_B2_CRYPTO_INFO_DATA			= 1	,
	B24_BITMAP_TK_B2_TVR							,
	B24_BITMAP_TK_B2_ARQC							,
	B24_BITMAP_TK_B2_AMOUNT_AUTH					,
	B24_BITMAP_TK_B2_AMOUNT_OTHER					,
	B24_BITMAP_TK_B2_AIP							,
	B24_BITMAP_TK_B2_ATC							,
	B24_BITMAP_TK_B2_TERM_COUNTRY_CODE				,
	B24_BITMAP_TK_B2_TRX_CURRENCY_CODE				,
	B24_BITMAP_TK_B2_TRX_DATE						,
	B24_BITMAP_TK_B2_TRX_TYPE						,
	B24_BITMAP_TK_B2_UN								,
	B24_BITMAP_TK_B2_ISS_APPL_DATA				= 15
};

/******************************************************************************\ 
* B3 - The EMV Discretionary Data Token consists of EMV-related data that is 
* not required for authorization. However, each data element is supported by 
* more than one EMV-compliant interface and, therefore, can be mapped between 
* interfaces by BASE24.
\******************************************************************************/
#define B24_TK_B3_OFF_BITMAP					0		/* "CD00" OR "CF00" Indicates whether data in each of the remaining fields in the token is present or absent */
#define B24_TK_B3_OFF_IFD_SERIAL_NO				4		/* EMV Tag 9F1E */
#define B24_TK_B3_OFF_EMV_TERM_CAPABILITIES		12		/* EMV Tag 9F33 */
#define B24_TK_B3_OFF_USR_FLD_1					20		/* N/A Zeroes */
#define B24_TK_B3_OFF_USR_FLD_2					24		/* N/A Zeroes */
#define B24_TK_B3_OFF_EMV_TERM_TYPE				32		/* EMV Tag 9F35 */
#define B24_TK_B3_OFF_APPL_VER_NO				34		/* EMV Tag 9F09 */
#define B24_TK_B3_OFF_CVM_RESULTS				38		/* EMV Tag 9F34 */
#define B24_TK_B3_OFF_DF_NAME_LEN				44		/* Length in bytes of the actual data in the field DF-NAME. */
#define B24_TK_B3_OFF_DF_NAME					48		/* EMV Tag 84 */
#define B24_TK_B3_SIZE							80


enum {
	B24_TK_B3_BITMAP						=0	,
	B24_TK_B3_IFD_SERIAL_NO						,
	B24_TK_B3_EMV_TERM_CAPABILITIES				,
	B24_TK_B3_USR_FLD_1							,
	B24_TK_B3_USR_FLD_2							,
	B24_TK_B3_EMV_TERM_TYPE						,
	B24_TK_B3_APPL_VER_NO						,
	B24_TK_B3_CVM_RESULTS						,
	B24_TK_B3_DF_NAME_LEN						,
	B24_TK_B3_DF_NAME							,

};

enum {
	B24_BITMAP_TK_B3_IFD_SERIAL_NO					=0	,
	B24_BITMAP_TK_B3_EMV_TERM_CAPABILITIES				,
	B24_BITMAP_TK_B3_USR_FLD_1							,
	B24_BITMAP_TK_B3_USR_FLD_2							,
	B24_BITMAP_TK_B3_EMV_TERM_TYPE						,
	B24_BITMAP_TK_B3_APPL_VER_NO						,
	B24_BITMAP_TK_B3_CVM_RESULTS						,
	B24_BITMAP_TK_B3_DF_NAME							,
};

/******************************************************************************\ 
* B4 - The EMV Status Token holds data identifying the status of the transaction.
\******************************************************************************/
#define B24_TK_B4_OFF_POS_ENTRY_MODE			0		/* Point of service entry mode. This field indicates the manner in which the 
														 * card details were entered at the device and the PIN entry capability of the terminal */
#define B24_TK_B4_OFF_TERM_CAPABILITIES			3		/* The capability of the terminal. This field is set by the acquiring process */
#define B24_TK_B4_OFF_LAST_EMV_STAT				4		/* Indicates whether the card used to initiate a magnetic stripe transaction is a chip card.*/
#define B24_TK_B4_OFF_DATA_SUSPECT				5		/* Indicates whether the card authentication method (CAM) data is reliable. This flag is set by the acquiring process. */
#define B24_TK_B4_OFF_APPL_PAN_SEQ_NO			6		/* The application PAN sequence number extracted from EMV Tag 5F34 */
#define B24_TK_B4_OFF_CAM_FLAGS					8		/* Identifies conditions encountered in the terminal. */
#define B24_TK_B4_OFF_CVM_RESULTS				8		/* The results of the last Cardholder Verification Method (CVM) performed */
#define B24_TK_B4_OFF_ICHG_DEF					8		/* Used by the Visanet Interchange only */
#define B24_TK_B4_OFF_REASON_ONLINE_CODE		14		/* The message reason code specifies why a transaction is to be authorized online or 
														 * why a transaction has been completed locally. */
#define B24_TK_B4_OFF_ARQC_VERIFY				18		/* The result of the authorization request cryptogram verification */
#define B24_TK_B4_OFF_ISO_RC_IND				19		/* The ISO 8583 (1987) Response Code Indicator. This field indicates whether the 
														 * ISO response code indicator sent to the interchange should be used in generating the 
														 * Authorization Response Cryptogram (ARPC), or if the ISO response code received from 
														 * the interchange should be returned to the terminal as the Authorization Response Code. */
#define B24_TK_B4_SIZE							20

enum {
	B24_TK_B4_POS_ENTRY_MODE			=0	,
	B24_TK_B4_TERM_CAPABILITIES				,
	B24_TK_B4_LAST_EMV_STAT					,
	B24_TK_B4_DATA_SUSPECT					,
	B24_TK_B4_APPL_PAN_SEQ_NO				,
	B24_TK_B4_CAM_FLAGS						,
	B24_TK_B4_CVM_RESULTS					,
	B24_TK_B4_ICHG_DEF						,
	B24_TK_B4_REASON_ONLINE_CODE			,
	B24_TK_B4_ARQC_VERIFY					,
	B24_TK_B4_ISO_RC_IND					,
};
/******************************************************************************\ 
* B5 - The EMV Response Data Token contains the response cryptogram, 
* data required to generate the response cryptogram and flags used to identify 
* the scripts to be returned to the acquirer. If authorization is performed 
* on BASE24, the BASE24 Authorization process creates this token. 
* If the transaction is routed to an interchange for authorization, 
* the BASE24 Interchange Interface process creates the token. 
* This field corresponds to the EMV Tag 91.
\******************************************************************************/
#define B24_TK_B5_OFF_ISS_AUTH_DATA_LEN			0		/* The length in bytes of the ISS-AUTH-DATA field */
#define B24_TK_B5_OFF_ISS_AUTH_DATA				4
#define B24_TK_B5_OFF_ARPC						4		/* The Authorization Response Cryptogram computed by the card application for online issuer authentication. */
#define B24_TK_B5_OFF_VISA_ISS_RESP_CODE		20		/* The response code used when generating the ARPC. This field is sent to and from VISA in EBCEDIC. The Visa Interchange Interface converts it to ASCII when placing it in the token. */
#define B24_TK_B5_OFF_VISA_INFO					24		/* Issuer discretionary data */
#define B24_TK_B5_OFF_MCPA_ISS_AUTH_RESP_CODE	20		/* The ISO issuer authorization response code used in generating the ARPC. */
#define B24_TK_B5_OFF_MCPA_INFO					24		/* Issuer discretionary data */
#define B24_TK_B5_OFF_MCHIP4_ARPC_RESP_CODE		20		/* The issuer authorization response code used in generating the authorization response cryptogram. */
#define B24_TK_B5_OFF_MCHIP4_INFO				24		/* Issuer discretionary data */


#define B24_TK_B5_SIZE							36

enum {
	B24_TK_B5_ISS_AUTH_DATA_LEN				=0	,
	B24_TK_B5_ISS_AUTH_DATA						,
	B24_TK_B5_ARPC								,
	B24_TK_B5_VISA_ISS_RESP_CODE				,
	B24_TK_B5_VISA_INFO							,
	B24_TK_B5_MCPA_ISS_AUTH_RESP_CODE			,
	B24_TK_B5_MCPA_INFO							,
	B24_TK_B5_MCHIP4_ARPC_RESP_CODE				,
	B24_TK_B5_MCHIP4_INFO						,

};

/******************************************************************************\ 
* B6 - The EMV Script Data Token holds the EMV Script Data. 
* The issuer process creates this token.
\******************************************************************************/
#define B24_TK_B6_OFF_ISS_SCRIPT_DATA_LEN		0		/* The length in bytes of the ISS-SCRIPT-DATA field. */
#define B24_TK_B6_OFF_ISS_SCRIPT_DATA			4		/* The Issuer Script Templates (EMV Tag 71 or 72) sent to the terminal for processing by the card application. */
#define B24_TK_B6_SIZE							260

enum {
	B24_TK_B6_ISS_SCRIPT_DATA_LEN			= 0	,
	B24_TK_B6_ISS_SCRIPT_DATA					,
};
/******************************************************************************\ 
* BJ - The EMV Issuer Script Results Token holds information about the 
* processing of EMV Script Data. This token is created by the acquirer interface 
* process or sent by the acquirer. This token corresponds to the EMV Tag 9F5B.
\******************************************************************************/
#define B24_TK_BJ_OFF_NUM_ISS_SCRIPT_RESULTS	0	/* The number of completed issuer script results contained within the token */
#define B24_TK_BJ_OFF_USR_FLD_1					1	/* Reserved for future use. */
#define B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_1	2	/* A code indicating the result of the script processing. */
#define B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_1			3	/* The details of the Script Sequence in the processing. */
#define B24_TK_BJ_OFF_ISS_SCRIPT_ID_1			4	/* The issuer script identifier */
#define B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_2	12
#define B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_2			13
#define B24_TK_BJ_OFF_ISS_SCRIPT_ID_2			14
#define B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_3	22
#define B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_3			23
#define B24_TK_BJ_OFF_ISS_SCRIPT_ID_3			24
#define B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_4	32
#define B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_4			33
#define B24_TK_BJ_OFF_ISS_SCRIPT_ID_4			34
#define B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_5	42
#define B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_5			43
#define B24_TK_BJ_OFF_ISS_SCRIPT_ID_5			44
#define B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_6	52
#define B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_6			53
#define B24_TK_BJ_OFF_ISS_SCRIPT_ID_6			54
#define B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_7	62
#define B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_7			63
#define B24_TK_BJ_OFF_ISS_SCRIPT_ID_7			64
#define B24_TK_BJ_OFF_ISS_SCRIPT_PROC_RESULT_8	72
#define B24_TK_BJ_OFF_ISS_SCRIPT_SEQ_8			73
#define B24_TK_BJ_OFF_ISS_SCRIPT_ID_8			74


#define B24_TK_BJ_ISS_SCRIPT_SZ					10
#define B24_TK_BJ_SIZE							82


enum {
	B24_TK_BJ_NUM_ISS_SCRIPT_RESULTS		=0	,
	B24_TK_BJ_USR_FLD_1							,
	B24_TK_BJ_ISS_SCRIPT_PROC_RESULT_1			,
	B24_TK_BJ_ISS_SCRIPT_SEQ_1					,
	B24_TK_BJ_ISS_SCRIPT_ID_1					,
	B24_TK_BJ_ISS_SCRIPT_PROC_RESULT_2			,
	B24_TK_BJ_ISS_SCRIPT_SEQ_2					,
	B24_TK_BJ_ISS_SCRIPT_ID_2					,
	B24_TK_BJ_ISS_SCRIPT_PROC_RESULT_3			,
	B24_TK_BJ_ISS_SCRIPT_SEQ_3					,
	B24_TK_BJ_ISS_SCRIPT_ID_3					,
	B24_TK_BJ_ISS_SCRIPT_PROC_RESULT_4			,
	B24_TK_BJ_ISS_SCRIPT_SEQ_4					,
	B24_TK_BJ_ISS_SCRIPT_ID_4					,
	B24_TK_BJ_ISS_SCRIPT_PROC_RESULT_5			,
	B24_TK_BJ_ISS_SCRIPT_SEQ_5					,
	B24_TK_BJ_ISS_SCRIPT_ID_5					,
	B24_TK_BJ_ISS_SCRIPT_PROC_RESULT_6			,
	B24_TK_BJ_ISS_SCRIPT_SEQ_6					,
	B24_TK_BJ_ISS_SCRIPT_ID_6					,
	B24_TK_BJ_ISS_SCRIPT_PROC_RESULT_7			,
	B24_TK_BJ_ISS_SCRIPT_SEQ_7					,
	B24_TK_BJ_ISS_SCRIPT_ID_7					,
	B24_TK_BJ_ISS_SCRIPT_PROC_RESULT_8			,
	B24_TK_BJ_ISS_SCRIPT_SEQ_8					,
	B24_TK_BJ_ISS_SCRIPT_ID_8					,
};
/******************************************************************************\ 
* 01 - ADDRESS VERIFICATION TOKEN – ADDR-VER-TKN
\******************************************************************************/
#define B24_TK_01_OFF_ADDR						0		/* The billing address for the cardholder */
#define B24_TK_01_OFF_ZIP_CODE					20		/* The ZIP code or postal code for the billing address for the cardholder. The ZIP code
														 * must be five or nine digits long. If the ZIP code is five digits long, 
														 * it is left-justified and blank filled. */
#define B24_TK_01_OFF_USR_FLD_1					29
#define B24_TK_01_SIZE							30

enum {
	B24_TK_01_ADDR					=0	,
	B24_TK_01_ZIP_CODE					,
	B24_TK_01_USR_FLD_1					,
};

/******************************************************************************\ 
* 04 - B24-pos RELEASE 5.0 TOKEN – PS50-TKN
\******************************************************************************/
#define B24_TK_04_OFF_ERROR_FLAG						0
#define B24_TK_04_OFF_ROUTING_GROUP						1	
#define B24_TK_04_OFF_CARD_VERIF_FLAG					12
#define B24_TK_04_OFF_CITY_EXT							13
#define B24_TK_04_OFF_TRACK2_CAP						18
#define B24_TK_04_OFF_UAF_FLAG							19
#define B24_TK_04_SIZE									20

enum {
	B24_TK_04_ERROR_FLAG				=0	,
	B24_TK_04_ROUTING_GROUP					,
	B24_TK_04_CARD_VERIF_FLAG				,
	B24_TK_04_CITY_EXT						,
	B24_TK_04_TRACK2_CAP					,
	B24_TK_04_UAF_FLAG						,
};

/******************************************************************************\ 
* C0 - 
\******************************************************************************/
#define B24_TK_C0_OFF_CVD_FIELD							0
#define B24_TK_C0_OFF_RESUB_STATUS						4			
#define B24_TK_C0_OFF_RESUB_CNT							5
#define B24_TK_C0_OFF_TERM_POSTAL_CODE					8
#define B24_TK_C0_OFF_ECOM_FLAG							18
#define B24_TK_C0_OFF_MOTO_FLAG							18
#define B24_TK_C0_OFF_COM_CARD_TYPE						19
#define B24_TK_C0_OFF_ADNL_DATA_IND						20
#define B24_TK_C0_OFF_CVD_FLD_PRESENT					21
#define B24_TK_C0_OFF_SAF_OR_FORCE_POST					22
#define B24_TK_C0_OFF_AUTH_COLL_IND						23
#define B24_TK_C0_OFF_FRD_PRN_FLG						24
#define B24_TK_C0_OFF_CAVV_AAV_RESULT					25
#define B24_TK_C0_SIZE									26

enum {
	B24_TK_C0_CVD_FIELD					=0	,
	B24_TK_C0_RESUB_STATUS					,
	B24_TK_C0_RESUB_CNT						,
	B24_TK_C0_TERM_POSTAL_CODE				,
	B24_TK_C0_ECOM_FLAG						,
	B24_TK_C0_MOTO_FLAG						,
	B24_TK_C0_COM_CARD_TYPE					,
	B24_TK_C0_ADNL_DATA_IND					,
	B24_TK_C0_CVD_FLD_PRESENT				,
	B24_TK_C0_SAF_OR_FORCE_POST				,
	B24_TK_C0_AUTH_COLL_IND					,
	B24_TK_C0_FRD_PRN_FLG					,
	B24_TK_C0_CAVV_AAV_RESULT				,
};

/******************************************************************************\ 
* C4 - 
\******************************************************************************/
#define B24_TK_C4_OFF_TERM_ATTEND_IND					0
#define B24_TK_C4_OFF_TERM_OPER_IND						1
#define B24_TK_C4_OFF_TERM_LOC_IND						2
#define B24_TK_C4_OFF_CH_PRESENT_IND					3
#define B24_TK_C4_OFF_CARD_PRESENT_IND					4
#define B24_TK_C4_OFF_CARD_CAPTURE_IND					5
#define B24_TK_C4_OFF_TRX_STAT_IND						7 /* TODO:(check offsets!!) Is it really 7 or 6? */
#define B24_TK_C4_OFF_TRX_SEC_IND						8
#define B24_TK_C4_OFF_TRX_RTN_IND						9
#define B24_TK_C4_OFF_CH_ACTVT_TERM_IND					10
#define B24_TK_C4_OFF_TERM_INPUT_CAP_IND				10
#define B24_TK_C4_OFF_CH_ID_METHOD						11
#define B24_TK_C4_SIZE									12


enum {
	B24_TK_C4_TERM_ATTEND_IND			=0	,
	B24_TK_C4_TERM_OPER_IND					,
	B24_TK_C4_TERM_LOC_IND					,
	B24_TK_C4_CH_PRESENT_IND				,
	B24_TK_C4_CARD_PRESENT_IND				,
	B24_TK_C4_CARD_CAPTURE_IND				,
	B24_TK_C4_TRX_STAT_IND					,
	B24_TK_C4_TRX_SEC_IND					,
	B24_TK_C4_TRX_RTN_IND					,
	B24_TK_C4_CH_ACTVT_TERM_IND				,
	B24_TK_C4_TERM_INPUT_CAP_IND			,
	B24_TK_C4_CH_ID_METHOD					,
};
/*START NAB08102020 PLUTONL-2288*/
/***********************\
 *Q1 -
/***********************/

#define B24_TK_Q1_OFF_AUTH_MODE_ID				0
#define B24_TK_Q1_OFF_CRYPTO_VAL_ID				1
#define B24_TK_Q1_SIZE							2

enum {
	B24_TK_Q1_AUTH_MODE_ID				=0	,
	B24_TK_Q1_CRYPTO_VAL_ID					,

};

/***********************\
 *Q2 -
/***********************/

#define B24_TK_Q2_OFF_ACCESS_AUTH_ID			0
#define B24_TK_Q2_SIZE							2

enum {
	B24_TK_Q2_ACCESS_AUTH_ID				=0	,

};
/***********************\
 *Q6 -

/***********************/

#define B24_TK_Q6_OFF_DEFERRAL_NB_MONTH_EXCLUDED		0			
#define B24_TK_Q6_OFF_NUM_PAYMENTS						2
#define B24_TK_Q6_OFF_PLAN_TYPE							4
#define B24_TK_Q6_SIZE									6
enum {
	B24_TK_Q6_DEFERRAL_NB_MONTH_EXCLUDED				=0	,
	B24_TK_Q6_NUM_PAYMENTS					,
	B24_TK_Q6_PLAN_TYPE					,

};

/***********************\
 *CZ -
/***********************/
#define B24_TK_CZ_OFF_ATC						0
#define B24_TK_CZ_OFF_FORM_FACTR_IND			4
#define B24_TK_CZ_OFF_USER_FLD_ACI				12
#define B24_TK_CZ_SIZE							40
enum {
	B24_TK_CZ_ATC				=0	,
	B24_TK_CZ_FORM_FACTR_IND					,
	B24_TK_CZ_USER_FLD_ACI					,

};

/***********************\
 *R4 -
/***********************/
#define B24_TK_R4_OFF_CONTRACT_NUMBER			0
#define B24_TK_R4_SIZE							20

enum {
	B24_TK_R4_CONTRACT_NUMBER				=0	,


};

/***********************\
 *RJ -
/***********************/
#define B24_TK_RJ_OFF_3DS_PROTOCOL_IND			0
#define B24_TK_RJ_OFF_DS_TXN_ID					2
#define B24_TK_RJ_OFF_USER_FLD3				    38
#define B24_TK_RJ_SIZE							40
enum {
	B24_TK_RJ_3DS_PROTOCOL_IND			=0	,
	B24_TK_RJ_DS_TXN_ID					,
	B24_TK_RJ_USER_FLD3					,


};
/*END NAB08102020 PLUTONL-2288*/
/******************************************************************************\ 
* BE - Original Currency
\******************************************************************************/
#define B24_TK_BE_OFF_AMOUNT						0
#define B24_TK_BE_OFF_ADD_AMOUNT					19
#define B24_TK_BE_OFF_CURRENCY						38
#define B24_TK_BE_OFF_CONV_RATE						41
#define B24_TK_BE_OFF_CONV_DATE						49
#define B24_TK_BE_OFF_USR_FLD_1						53
#define B24_TK_BE_SIZE								62


enum {
	B24_TK_BE_AMOUNT				=0	,
	B24_TK_BE_ADD_AMOUNT				,
	B24_TK_BE_CURRENCY					,
	B24_TK_BE_CONV_RATE					,
	B24_TK_BE_CONV_DATE					,
	B24_TK_BE_USR_FLD_1					,
};

/******************************************************************************\ 
* TOKEN PF – “HACIENDA” AUTHORIZATION TOKEN
\******************************************************************************/
#define B24_TK_PF_OFF_PP_PROV_FIID					0
#define B24_TK_PF_OFF_PP_TYPE						4
#define B24_TK_PF_OFF_PP_DENOM						8
#define B24_TK_PF_OFF_PP_CTRL_NO					20
#define B24_TK_PF_OFF_USR_FLD_1						35
#define B24_TK_PF_SIZE								36


enum {
	B24_TK_PF_PP_PROV_FIID			=0	,
	B24_TK_PF_PP_TYPE					,
	B24_TK_PF_PP_DENOM					,
	B24_TK_PF_PP_CTRL_NO				,
	B24_TK_PF_USR_FLD_1					,
};

/******************************************************************************\ 
* TOKEN ‘PG’ – ATM TOKEN
\******************************************************************************/
#define B24_TK_PG_OFF_PP_OP_CODE					0
#define B24_TK_PG_OFF_PP_PIN_ACCESS_NO				8
#define B24_TK_PG_OFF_PP_PIN_LEN					24
#define B24_TK_PG_SIZE								26

enum {
	B24_TK_PG_PP_OP_CODE				=0	,
	B24_TK_PG_PP_PIN_ACCESS_NO				,
	B24_TK_PG_PP_PIN_LEN					,
};

/******************************************************************************\ 
* TOKEN P1 (RPQ) – PINLESS TRANSACTIONS
\******************************************************************************/
#define B24_TK_P1_OFF_OP_CODE						0
#define B24_TK_P1_OFF_PHONE_NBR_FROM				2
#define B24_TK_P1_OFF_PHONE_NBR_TO					12
#define B24_TK_P1_OFF_BIRTHDAY						22
#define B24_TK_P1_OFF_ACC_TYPE						30
#define B24_TK_P1_OFF_OP_CARD						32
#define B24_TK_P1_OFF_ATH_MOVIL_STATUS				51
#define B24_TK_P1_OFF_CARD_AFF_NBR					53
#define B24_TK_P1_OFF_CARD_NBR_1					54
#define B24_TK_P1_OFF_ACC_TYPE_1					73
#define B24_TK_P1_OFF_CARD_NBR_2					75
#define B24_TK_P1_OFF_ACC_TYPE_2					94
#define B24_TK_P1_OFF_CARD_NBR_3					96
#define B24_TK_P1_OFF_ACC_TYPE_3					115
#define B24_TK_P1_OFF_CARD_NBR_4					117
#define B24_TK_P1_OFF_ACC_TYPE_4					136
#define B24_TK_P1_OFF_CARD_NBR_5					138
#define B24_TK_P1_OFF_ACC_TYPE_5					157
#define B24_TK_P1_OFF_LANGUAGE						159
#define B24_TK_P1_SIZE								160


enum {
	B24_TK_P1_OP_CODE					=0	,
	B24_TK_P1_PHONE_NBR_FROM				,
	B24_TK_P1_PHONE_NBR_TO					,
	B24_TK_P1_BIRTHDAY						,
	B24_TK_P1_ACC_TYPE						,
	B24_TK_P1_OP_CARD						,
	B24_TK_P1_ATH_MOVIL_STATUS				,
	B24_TK_P1_CARD_AFF_NBR					,
	B24_TK_P1_CARD_NBR_1					,
	B24_TK_P1_ACC_TYPE_1					,
	B24_TK_P1_CARD_NBR_2					,
	B24_TK_P1_ACC_TYPE_2					,
	B24_TK_P1_CARD_NBR_3					,
	B24_TK_P1_ACC_TYPE_3					,
	B24_TK_P1_CARD_NBR_4					,
	B24_TK_P1_ACC_TYPE_4					,
	B24_TK_P1_CARD_NBR_5					,
	B24_TK_P1_ACC_TYPE_5					,
	B24_TK_P1_LANGUAGE						,
};
/******************************************************************************\ 
* TOKEN ‘06’ – PIN CHANGE TOKEN
\******************************************************************************/
#define B24_TK_06_OFF_NEW_PIN_FORMAT				0
#define B24_TK_06_OFF_NEW_PIN_OFFSET				1
#define B24_TK_06_OFF_NEW_PIN_COUNT					17
#define B24_TK_06_OFF_NEW_PIN_SIZE					18
#define B24_TK_06_OFF_NEW_PIN_1						20
#define B24_TK_06_OFF_NEW_PIN_2						36
#if defined(B24_NCCC)
#define B24_TK_06_OFF_NCCC_STANDIN_IND				52
#define B24_TK_06_OFF_PVV_ON_CARD_FLAG				53
#define B24_TK_06_SIZE								54
#else 
#define B24_TK_06_SIZE								52
#endif


enum {
	B24_TK_06_NEW_PIN_FORMAT		=0	,
	B24_TK_06_NEW_PIN_OFFSET			,
	B24_TK_06_NEW_PIN_COUNT				,
	B24_TK_06_NEW_PIN_SIZE				,
	B24_TK_06_NEW_PIN_1					,
	B24_TK_06_NEW_PIN_2					,
#if defined(B24_NCCC)
	B24_TK_06_NCCC_STANDIN_IND			,
	B24_TK_06_PVV_ON_CARD_FLAG			,
#endif
};

/******************************************************************************\ 
* TOKEN ‘PZ’ – CREDIT LINE INFORMATION
\******************************************************************************/
#define B24_TK_PZ_OFF_RSRV_RCPT_FIID				0
#define B24_TK_PZ_OFF_RSRV_DSPLY_BAL				4
#define B24_TK_PZ_OFF_RSRV_AVAIL_BAL				22
#define B24_TK_PZ_OFF_RSRV_LEDGER_BAL				40
#define B24_TK_PZ_OFF_CHKG_DSPLY_BAL				58
#define B24_TK_PZ_OFF_CHKG_AVAIL_BAL				76
#define B24_TK_PZ_OFF_CHKG_LEDGER_BAL				94
#define B24_TK_PZ_OFF_RSRV_USR_FLD					106
#define B24_TK_PZ_SIZE								149


enum {
	B24_TK_PZ_RSRV_RCPT_FIID		=0	,
	B24_TK_PZ_RSRV_DSPLY_BAL			,
	B24_TK_PZ_RSRV_AVAIL_BAL			,
	B24_TK_PZ_RSRV_LEDGER_BAL			,
	B24_TK_PZ_CHKG_DSPLY_BAL			,
	B24_TK_PZ_CHKG_AVAIL_BAL			,
	B24_TK_PZ_CHKG_LEDGER_BAL			,
	B24_TK_PZ_RSRV_USR_FLD				,
};

/******************************************************************************\ 
* TOKEN ‘A5’ – Non-Currency Dispense Token
\******************************************************************************/
#define B24_TK_A5_OFF_ITEM_QTY						0
#define B24_TK_A5_OFF_HOPR_CONTENTS					2
#define B24_TK_A5_SIZE								4

enum {
	B24_TK_A5_ITEM_QTY				=0	,
	B24_TK_A5_HOPR_CONTENTS				,
};

/******************************************************************************\ 
* TOKEN ‘17’
\******************************************************************************/
/*
#define B24_TK_17_OFF_TRACK_1						0
#define B24_TK_17_OFF_TID							1
#define B24_TK_17_OFF_FILLER						16
#define B24_TK_17_SIZE								24


enum {
	B24_TK_17_TRACK_1			=0	,
	B24_TK_17_TID					,
	B24_TK_17_FILLER				,
};
*/

#define B24_TK_17_OFF_SERVICE_IND				0
#define B24_TK_17_OFF_TID						1
#define B24_TK_17_OFF_VALIDATION_CODE			16
#define B24_TK_17_OFF_MARKET_SPECIFIC			20
#define B24_TK_17_OFF_DURATION					21
#define B24_TK_17_OFF_PRESTIGIOUS_PROP_IND		23
#define B24_TK_17_SIZE							24

enum
{
	B24_TK_17_SERVICE_IND			=0	,
	B24_TK_17_TID						,
	B24_TK_17_VALIDATION_CODE			,
	B24_TK_17_MARKET_SPECIFIC			,
	B24_TK_17_DURATION					,
	B24_TK_17_PRESTIGIOUS_PROP_IND		,
};

/******************************************************************************\ 
* TOKEN ‘21’ PS2000 ATM Token
\******************************************************************************/
#define B24_TK_21_OFF_SERVICE_IND				0
#define B24_TK_21_OFF_TID						1
#define B24_TK_21_OFF_VALIDATION_CODE			16
#define B24_TK_21_SIZE							20
enum
{
	B24_TK_21_SERVICE_IND			=0	,
	B24_TK_21_TID						,
	B24_TK_21_VALIDATION_CODE			,

};
/******************************************************************************\ 
* TOKEN ‘25’: Transaction surcharge amount
\******************************************************************************/
#define B24_TK_25_OFF_TRX_FEE						0
#define B24_TK_25_OFF_ORIG_TRX_FEE					19
#define B24_TK_25_OFF_FILLER						38
#define B24_TK_25_SIZE								70

enum {
	B24_TK_25_TRX_FEE			=0	,
	B24_TK_25_ORIG_TRX_FEE			,
	B24_TK_25_FILLER				,
};



/******************************************************************************\ 
* TOKEN ‘B0’: Transaction surcharge amount
\******************************************************************************/
#define B24_TK_B0_OFF_LENGTH						0
#define B24_TK_B0_OFF_FILLER_1						3
#define B24_TK_B0_OFF_FIID							4
#define B24_TK_B0_OFF_FILLER_2						8
#define B24_TK_B0_OFF_ADV_REASON_CODE				20
#define B24_TK_B0_OFF_FILLER_3						23
#define B24_TK_B0_OFF_ON_BEHALF						38
#define B24_TK_B0_OFF_ON_BEHALF_SVC_1				38
#define B24_TK_B0_OFF_ON_BEHALF_RESULT_1			40
#define B24_TK_B0_OFF_ON_BEHALF_SVC_2				41
#define B24_TK_B0_OFF_ON_BEHALF_RESULT_2			43
#define B24_TK_B0_OFF_ON_BEHALF_SVC_3				44
#define B24_TK_B0_OFF_ON_BEHALF_RESULT_3			46
#define B24_TK_B0_OFF_ON_BEHALF_SVC_4				47
#define B24_TK_B0_OFF_ON_BEHALF_RESULT_4			49
#define B24_TK_B0_OFF_ON_BEHALF_SVC_5				50
#define B24_TK_B0_OFF_ON_BEHALF_RESULT_5			52
#define B24_TK_B0_OFF_ON_BEHALF_SVC_6				53
#define B24_TK_B0_OFF_ON_BEHALF_RESULT_6			55
#define B24_TK_B0_OFF_ON_BEHALF_SVC_7				57
#define B24_TK_B0_OFF_ON_BEHALF_RESULT_7			58
#define B24_TK_B0_OFF_ON_BEHALF_SVC_8				60
#define B24_TK_B0_OFF_ON_BEHALF_RESULT_8			61
#define B24_TK_B0_OFF_ON_BEHALF_SVC_9				63
#define B24_TK_B0_OFF_ON_BEHALF_RESULT_9			64
#define B24_TK_B0_OFF_ON_BEHALF_SVC_10				66
#define B24_TK_B0_OFF_ON_BEHALF_RESULT_10			67
#define B24_TK_B0_OFF_FILLER_4						68
#define B24_TK_B0_OFF_ADV_DETAIL_CODE				85
#define B24_TK_B0_OFF_AUTH_AGENT_ID_CODE			89
#define B24_TK_B0_OFF_FILLER_5						95
#define B24_TK_B0_SIZE								224


enum {
	B24_TK_B0_LENGTH					=0	,
	B24_TK_B0_FILLER_1						,
	B24_TK_B0_FIID							,
	B24_TK_B0_FILLER_2						,
	B24_TK_B0_ADV_REASON_CODE				,
	B24_TK_B0_FILLER_3						,
	B24_TK_B0_ON_BEHALF						,
	B24_TK_B0_ON_BEHALF_SVC_1				,
	B24_TK_B0_ON_BEHALF_RESULT_1			,
	B24_TK_B0_ON_BEHALF_SVC_2				,
	B24_TK_B0_ON_BEHALF_RESULT_2			,
	B24_TK_B0_ON_BEHALF_SVC_3				,
	B24_TK_B0_ON_BEHALF_RESULT_3			,
	B24_TK_B0_ON_BEHALF_SVC_4				,
	B24_TK_B0_ON_BEHALF_RESULT_4			,
	B24_TK_B0_ON_BEHALF_SVC_5				,
	B24_TK_B0_ON_BEHALF_RESULT_5			,
	B24_TK_B0_ON_BEHALF_SVC_6				,
	B24_TK_B0_ON_BEHALF_RESULT_6			,
	B24_TK_B0_ON_BEHALF_SVC_7				,
	B24_TK_B0_ON_BEHALF_RESULT_7			,
	B24_TK_B0_ON_BEHALF_SVC_8				,
	B24_TK_B0_ON_BEHALF_RESULT_8			,
	B24_TK_B0_ON_BEHALF_SVC_9				,
	B24_TK_B0_ON_BEHALF_RESULT_9			,
	B24_TK_B0_ON_BEHALF_SVC_10				,
	B24_TK_B0_ON_BEHALF_RESULT_10			,
	B24_TK_B0_FILLER_4						,
	B24_TK_B0_ADV_DETAIL_CODE				,
	B24_TK_B0_AUTH_AGENT_ID_CODE			,
	B24_TK_B0_FILLER_5						,
};


/******************************************************************************\ 
* TOKEN ‘BY’: 
\******************************************************************************/
#define B24_TK_BY_OFF_FILLER_1						0
#define B24_TK_BY_OFF_PRODUCT_ID					52
#define B24_TK_BY_OFF_FILLER_2						54
#define B24_TK_BY_SIZE								60


enum {
	B24_TK_BY_FILLER_1				=0	,
	B24_TK_BY_PRODUCT_ID				,
	B24_TK_BY_FILLER_2					,
};

/******************************************************************************\ 
* TOKEN ‘C5’: Installment Payment transaction 
\******************************************************************************/
#define B24_TK_C5_OFF_MERCHANT_ID					0
#define B24_TK_C5_OFF_STORE_ID						10
#define B24_TK_C5_OFF_TRX_CODE						16
#define B24_TK_C5_OFF_REF_NBR						18
#define B24_TK_C5_OFF_GOODS_ID						29
#define B24_TK_C5_OFF_PERSONAL_ID					35
#define B24_TK_C5_OFF_PIN							45
#define B24_TK_C5_OFF_EC_FLAG						49

#define B24_TK_C5_OFF_ORDER_NO						50
#define B24_TK_C5_OFF_INSTALLMENT_TRX_IND			90
#define B24_TK_C5_OFF_INSTALLMENT_RESP_CODE			91
#define B24_TK_C5_OFF_FILLER						93
#define B24_TK_C5_OFF_PERIOD_NBR					120
#define B24_TK_C5_OFF_DOWN_PAYMENT					122
#define B24_TK_C5_OFF_INSTALLMENT_PAYMENTS			130
#define B24_TK_C5_OFF_FORMALITY_FEE					138
#define B24_TK_C5_SIZE								144

#define B24_TK_C5_OFF_LOY_SETTLEMENT_FLAG			50
#define B24_TK_C5_OFF_LOY_ORDER_NO					52
#define B24_TK_C5_OFF_LOY_TRX_IND					90
#define B24_TK_C5_OFF_LOY_RESP_CODE					91
#define B24_TK_C5_OFF_LOY_PTS_OF_REDEMPTION			93
#define B24_TK_C5_OFF_LOY_SIGN_PTS_OF_BALANCE		101
#define B24_TK_C5_OFF_LOY_PTS_OF_BALANCE			102
#define B24_TK_C5_OFF_LOY_PAID_CREDIT_AMNT			110
/*??#define B24_TK_C5_SIZE								124
*/

enum {
	B24_TK_C5_MERCHANT_ID				=0	,
	B24_TK_C5_STORE_ID						,
	B24_TK_C5_TRX_CODE						,
	B24_TK_C5_REF_NBR						,
	B24_TK_C5_GOODS_ID						,
	B24_TK_C5_PERSONAL_ID					,
	B24_TK_C5_PIN							,
	B24_TK_C5_EC_FLAG						,

	B24_TK_C5_ORDER_NO						,
	B24_TK_C5_INSTALLMENT_TRX_IND			,
	B24_TK_C5_INSTALLMENT_RESP_CODE			,
	B24_TK_C5_FILLER						,
	B24_TK_C5_PERIOD_NBR					,
	B24_TK_C5_DOWN_PAYMENT					,
	B24_TK_C5_INSTALLMENT_PAYMENTS			,
	B24_TK_C5_FORMALITY_FEE					,

	B24_TK_C5_LOY_SETTLEMENT_FLAG			,
	B24_TK_C5_LOY_ORDER_NO					,
	B24_TK_C5_LOY_TRX_IND					,
	B24_TK_C5_LOY_RESP_CODE					,
	B24_TK_C5_LOY_PTS_OF_REDEMPTION			,
	B24_TK_C5_LOY_SIGN_PTS_OF_BALANCE		,
	B24_TK_C5_LOY_PTS_OF_BALANCE			,
	B24_TK_C5_LOY_PAID_CREDIT_AMNT			,
};

/******************************************************************************\ 
* TOKEN ‘C6’: 
\******************************************************************************/
#define B24_TK_C6_OFF_XID							0
#define B24_TK_C6_OFF_TRX_STAIN_CAVV				40
#define B24_TK_C6_SIZE								80

enum {
	B24_TK_C6_XID				=0	,
	B24_TK_C6_TRX_STAIN_CAVV		,
};

/******************************************************************************\ 
* TOKEN ‘CE’: MasterCARD UCAF use
\******************************************************************************/
#define B24_TK_CE_OFF_AUTH_IND_FLAG					0
#define B24_TK_CE_OFF_AUTH_IND_DATA					2
#define B24_TK_CE_SIZE								202


enum {
	B24_TK_CE_AUTH_IND_FLAG			=0	,
	B24_TK_CE_AUTH_IND_DATA				,
};


/******************************************************************************\ 
* TOKEN ‘CH’: 
\******************************************************************************/

#define B24_TK_CH_OFF_FILLER_1						0
#if !defined(B24_NCCC)
#	define B24_TK_CH_OFF_RESP_SRC_RSN_CD				0
#	define B24_TK_CH_OFF_CRD_VRFY_FLG2					1
#	define B24_TK_CH_OFF_ONLINE_LMT						2
#	define B24_TK_CH_OFF_RETAILER_CLASS_CODE			14
#	define B24_TK_CH_OFF_EMV_CAPABLE_OUTLET				18
#	define B24_TK_CH_OFF_RECUR_PMNT_IND					19
#	define B24_TK_CH_OFF_NUM_INSTL						20
#	define B24_TK_CH_OFF_NUM_MONTH_GRATUITY				22
#	define B24_TK_CH_OFF_PMNT_PLAN						24
#	define B24_TK_CH_OFF_TERM_OUTPUT_CAP_IND			27
#	define B24_TK_CH_OFF_CH_AUTHN_CAP_IND				28
#	define B24_TK_CH_OFF_PARTIAL_AUTH_OPT				29
#	define B24_TK_CH_OFF_INSTL_PLAN_TYPE				30
#	define B24_TK_CH_OFF_INSTL_GRATUITY_PRD				32
#endif
#define B24_TK_CH_OFF_REV_REASON_IND				33
#define B24_TK_CH_OFF_FILLER_2						34
#define B24_TK_CH_SIZE								40


enum {
	B24_TK_CH_FILLER_1					=0	,
#if !defined(B24_NCCC)
	B24_TK_CH_RESP_SRC_RSN_CD				,
	B24_TK_CH_CRD_VRFY_FLG2					,
	B24_TK_CH_ONLINE_LMT					,
	B24_TK_CH_RETAILER_CLASS_CODE			,
	B24_TK_CH_EMV_CAPABLE_OUTLET			,
	B24_TK_CH_RECUR_PMNT_IND				,
	B24_TK_CH_NUM_INSTL						,
	B24_TK_CH_NUM_MONTH_GRATUITY			,
	B24_TK_CH_PMNT_PLAN						,
	B24_TK_CH_TERM_OUTPUT_CAP_IND			,
	B24_TK_CH_CH_AUTHN_CAP_IND				,
	B24_TK_CH_PARTIAL_AUTH_OPT				,
	B24_TK_CH_INSTL_PLAN_TYPE				,
	B24_TK_CH_INSTL_GRATUITY_PRD			,
#endif
	B24_TK_CH_REV_REASON_IND				,
	B24_TK_CH_FILLER_2						,
};


/******************************************************************************\ 
* TOKEN ‘S8’: 
\******************************************************************************/
#define B24_TK_S8_OFF_ACC_NO_IND					0
#define B24_TK_S8_OFF_ACC_NO						1
#define B24_TK_S8_OFF_EXPIRY_DATE					20
#define B24_TK_S8_OFF_FILLER						24
#define B24_TK_S8_SIZE								30


enum {
	B24_TK_S8_ACC_NO_IND			=0	,
	B24_TK_S8_ACC_NO					,
	B24_TK_S8_EXPIRY_DATE				,
	B24_TK_S8_FILLER					,
};

/******************************************************************************\ 
* TOKEN ‘CK’: Industry data
\******************************************************************************/
#define B24_TK_CK_OFF_INDUSTRY_TYPE					0
#define B24_TK_CK_OFF_LG_ARRIVAL_DATE				2
#define B24_TK_CK_OFF_LG_DEPARTURE_DATE				8
#define B24_TK_CK_OFF_LG_NB_ROOM_NIGHTS				14
#define B24_TK_CK_OFF_LG_ROOM_RATE					18
#define B24_TK_CK_OFF_LG_ROOM_TAX					30
#define B24_TK_CK_OFF_LG_PHONE_CHARGES				42
#define B24_TK_CK_OFF_LG_LAUNDRY_CHARGES			54
#define B24_TK_CK_OFF_LG_GIFT_SHOP_CHARGES			66
#define B24_TK_CK_OFF_LG_BAR_CHARGES				78
#define B24_TK_CK_OFF_LG_OTHER_CHARGES				90
#define B24_TK_CK_OFF_LG_TOTAL_TAX_AMNT				102
#define B24_TK_CK_OFF_LG_PROPERTY_PHONE_NO			114
#define B24_TK_CK_OFF_LG_CUST_SERV_PHONE_NO			129
#define B24_TK_CK_OFF_LG_FOLIO_NO					144
#define B24_TK_CK_OFF_LG_FIRE_SAFETY_IND			154
#define B24_TK_CK_OFF_LG_NO_SHOW_IND				155
#define B24_TK_CK_OFF_LG_USER_FLD_1					156

#define B24_TK_CK_OFF_CR_RENTER_NAME				2
#define B24_TK_CK_OFF_CR_RENTAL_CLASS_ID			31
#define B24_TK_CK_OFF_CR_RENTAL_DATE				35
#define B24_TK_CK_OFF_CR_RENTAL_CITY				41
#define B24_TK_CK_OFF_CR_RENTAL_STATE				59
#define B24_TK_CK_OFF_CR_RENTAL_COUNTRY				62
#define B24_TK_CK_OFF_CR_RETURN_DATE				65
#define B24_TK_CK_OFF_CR_RETURN_CITY				71
#define B24_TK_CK_OFF_CR_RETURN_STATE				89
#define B24_TK_CK_OFF_CR_RETURN_COUNTRY				92
#define B24_TK_CK_OFF_CR_RETURN_LOC_ID				95
#define B24_TK_CK_OFF_CR_NB_DAYS_RENTED				105
#define B24_TK_CK_OFF_CR_DAILY_RENTAL_RATE			109
#define B24_TK_CK_OFF_CR_EXTRA_CHARGES				121
#define B24_TK_CK_OFF_CR_TOTAL_TAX_AMNT				133
#define B24_TK_CK_OFF_CR_CUST_SERV_PHONE_NO			145
#define B24_TK_CK_OFF_CR_AGREEMENT_NO				160
#define B24_TK_CK_OFF_CR_TAX_EXEMPT_IND				169
#define B24_TK_CK_OFF_CR_NO_SHOW_IND				170
#define B24_TK_CK_OFF_CR_USER_FLD_2					171
#define B24_TK_CK_SIZE								172

enum {
	B24_TK_CK_INDUSTRY_TYPE			=0	,
	B24_TK_CK_LG_ARRIVAL_DATE			,
	B24_TK_CK_LG_DEPARTURE_DATE			,
	B24_TK_CK_LG_NB_ROOM_NIGHTS			,
	B24_TK_CK_LG_ROOM_RATE				,
	B24_TK_CK_LG_ROOM_TAX				,
	B24_TK_CK_LG_PHONE_CHARGES			,
	B24_TK_CK_LG_LAUNDRY_CHARGES		,
	B24_TK_CK_LG_GIFT_SHOP_CHARGES		,
	B24_TK_CK_LG_BAR_CHARGES			,
	B24_TK_CK_LG_OTHER_CHARGES			,
	B24_TK_CK_LG_TOTAL_TAX_AMNT			,
	B24_TK_CK_LG_PROPERTY_PHONE_NO		,
	B24_TK_CK_LG_CUST_SERV_PHONE_NO		,
	B24_TK_CK_LG_FOLIO_NO				,
	B24_TK_CK_LG_FIRE_SAFETY_IND		,
	B24_TK_CK_LG_NO_SHOW_IND			,
	B24_TK_CK_LG_USER_FLD_1				,
	B24_TK_CK_CR_RENTER_NAME			,
	B24_TK_CK_CR_RENTAL_CLASS_ID		,
	B24_TK_CK_CR_RENTAL_DATE			,
	B24_TK_CK_CR_RENTAL_CITY			,
	B24_TK_CK_CR_RENTAL_STATE			,
	B24_TK_CK_CR_RENTAL_COUNTRY			,
	B24_TK_CK_CR_RETURN_DATE			,
	B24_TK_CK_CR_RETURN_CITY			,
	B24_TK_CK_CR_RETURN_STATE			,
	B24_TK_CK_CR_RETURN_COUNTRY			,
	B24_TK_CK_CR_RETURN_LOC_ID			,
	B24_TK_CK_CR_NB_DAYS_RENTED			,
	B24_TK_CK_CR_DAILY_RENTAL_RATE		,
	B24_TK_CK_CR_EXTRA_CHARGES			,
	B24_TK_CK_CR_TOTAL_TAX_AMNT			,
	B24_TK_CK_CR_CUST_SERV_PHONE_NO		,
	B24_TK_CK_CR_AGREEMENT_NO			,
	B24_TK_CK_CR_TAX_EXEMPT_IND			,
	B24_TK_CK_CR_NO_SHOW_IND			,
	B24_TK_CK_CR_USER_FLD_2				,

};

/******************************************************************************\ 
* TOKEN ‘PP’: Taxes
\******************************************************************************/
#define B24_TK_PP_OFF_TAXE_1			0
#define B24_TK_PP_OFF_TAXE_2			12
#define B24_TK_PP_OFF_TAXE_3			24
#define B24_TK_PP_SIZE					36

enum {
	B24_TK_PP_TAXE_1			=0	,
	B24_TK_PP_TAXE_2				,
	B24_TK_PP_TAXE_3				,
};

/******************************************************************************\ 
* TOKEN ‘20’: MasterCARD TraceID
\******************************************************************************/
#define B24_TK_20_OFF_FILLER_1			0
#define B24_TK_20_OFF_TRACE_ID			1
#define B24_TK_20_OFF_FILLER_2			16
#define B24_TK_20_SIZE					22

enum {
	B24_TK_20_FILLER_1			=0	,
	B24_TK_20_TRACE_ID				,
	B24_TK_20_FILLER_2				,
};

/*******************************************************************************\

* TOKEN  'QT' : COST OF TRANSACTION 

\*******************************************************************************/

#define		B24_TK_QT_OFF_TRANS_CODE					0
#define		B24_TK_QT_OFF_REF_NBR_TRANS					6
#define		B24_TK_QT_OFF_COST_OF_TRANS					18
#define		B24_TK_QT_OFF_INDICATOR_REPORT_TRANS 		30
#define		B24_TK_QT_OFF_SPACE  						31

#define		B24_TK_QT_SIZE								32


enum{	
	B24_TK_QT_TRANS_CODE = 0			,	
	B24_TK_QT_REF_NBR_TRANS			,	
	B24_TK_QT_COST_OF_TRANS			,	
	B24_TK_QT_INDICATOR_REPORT_TRANS ,	
	B24_TK_QT_SPACE		,
	
	};
/******************************************************************************/
/******************************************************************************/

#define TK_B4_01_CHIP					"051"
#define TK_B4_01_FALLBACK				"801"

#define TK_B4_02_UNKNOWN				'0'
#define TK_B4_02_MAGSTRIP				'2'
#define TK_B4_02_ICC					'5'
#define TK_B4_02_OCR_NO_ICC				'8'
/******************************************************************************/
#define TK_C4_01_ATTENDED				'0'
#define TK_C4_01_UNATTENDED				'1'
#define TK_C4_01_NO_TERMINAL			'2'

#define TK_C4_03_ON_PREMISES_CA			'0'
#define TK_C4_03_OFF_PREMISES_CA		'1'
#define TK_C4_03_ON_PREMISES_CH			'2'
#define TK_C4_03_NO_TERMINAL			'3'

#define TK_C4_04_CH_PRESENT				'0'
#define TK_C4_04_CH_NOT_PRESENT			'1'
#define TK_C4_04_CH_NP_MAIL_OR_FAX		'2'
#define TK_C4_04_CH_NP_PHONE_OR_ARU		'3'
#define TK_C4_04_CH_NP_REC_OR_STANDIN	'4'
#define TK_C4_04_CH_NP_EC				'5'

#define TK_C4_05_CARD_PRESENT			'0'
#define TK_C4_05_CARD_NOT_PRESENT		'1'


#define TK_C4_06_NO_CAPTURE				'0'
#define TK_C4_06_DO_CAPTURE				'1'

/******************************************/
#define TK_C4_07_NORMAL_REQ				'0'
#define TK_C4_07_PRE_AUTH_REQ			'4'
#define TK_C4_07_AVR					'6'
#if defined(B24_NCCC)

#define TK_C4_07_SEC_PHONE_ORDER		'2'
#define TK_C4_07_INSTLLMNT_INQ_REQ		'3'
#define TK_C4_07_ACC_STATUS_INQ			'8'

#elif defined(B24_EVERTEC)

#define TK_C4_07_MER_AUTH				'1'
#define TK_C4_07_STANDIN				'5'
#define TK_C4_07_CASHBACK				'7'
#define TK_C4_07_DOWNTIME_SUB_REQ		'8'
#endif
/******************************************/


#define TK_C4_08_NO_SEC_CONCERN			'0'
#define TK_C4_08_SUSPECTED_FRAUD		'1'
#define TK_C4_08_IDENTITY_VERIFIED		'2'

#define TK_C4_10_NOT_CH_ACT				'0'
#define TK_C4_10_LVL_1					'1'
#define TK_C4_10_LVL_2					'2'
#define TK_C4_10_LVL_3					'3'
#define TK_C4_10_LVL_4					'4'
#define TK_C4_10_SCRIPT_DEV				'5'
#define TK_C4_10_EC						'6'
#define TK_C4_10_RADIO_FREQ_DEV			'7'

#define TK_C4_11_UNKNOWN				'0'
#define TK_C4_11_NO_TERMINAL			'1'
#define TK_C4_11_MAGSTRIP_READER		'2'
#define TK_C4_11_CONTACTLESS_CHP		'3'
#define TK_C4_11_CONTACTLESS_MAGSTRIP	'4'
#define TK_C4_11_MAGSTRIP_ICC			'5'
#define TK_C4_11_KEY_ENTRY				'6'
#define TK_C4_11_MAGSTRIP_KEY_ENTRY		'7'
#define TK_C4_11_MAGSTRIP_KEY_ENTRY_ICC	'8'
#define TK_C4_11_ICC					'9'

#define TK_C4_12_UNKNOWN				'0'
#define TK_C4_12_SIGNATURE				'1'
#define TK_C4_12_PIN					'2'
#define TK_C4_12_NONE_CH_PRESENT		'3'
#define TK_C4_12_NONE_CH_NP				'4'
#define TK_C4_12_AUTHENTICATION_VAL		'5'
#define TK_C4_12_E_SIG_ANALYSIS			'6'
#define TK_C4_12_BIOMETRICS				'7'
#define TK_C4_12_BIOGRAPHIC				'8'
#define TK_C4_12_OTHER					'9'

/******************************************************************************/
#define TK_C5_10_ISTLMT_INC_FEES		'I'
#define TK_C5_10_ISTLMT_PLUS_FEES		'E'

#define TK_C5_19_LOY_FULL_REDEM						'1' /* Full Redemption Transaction							*/
#define TK_C5_19_LOY_PARTIAL_REDEM					'2' /* Partial Redemption Transaction						*/
#define TK_C5_19_LOY_CLM_FULL_REDEM					'3' /* CLM Full Redemption Transaction						*/
#define TK_C5_19_LOY_CLM_PARTIAL_REDEM				'4' /* CLM Partial Redemption Transaction					*/
#define TK_C5_19_LOY_CLM_PARTIAL_REDEM_NO_SETL		'6' /* CLM Partial Redemption Transaction No settlement		*/
#define TK_C5_19_LOY_MPAS_FULL_REDEM				'7' /* MPAS full redemption transaction						*/

/******************************************************************************/
#define TK_BM_PYMNT_TRX					"C005"		/* Payment Transaction */
#define TK_BM_LOY_TRX					"C007"		/* Loyalty/Sweepstakes/Extras Transactions */
#define TK_BM_QUASICASH_TRX				"C008"		/* Quasi-cash Transaction */
#define TK_BM_TRX_SUBTYPE_LEN			4
/******************************************************************************/

#define TK_C0_EC_NOT_EC								'0'		/* Not an electronic commerce transaction						*/
#define TK_C0_EC_MOTO								'1'		/* Single mail or telephone order transaction					*/
#define TK_C0_EC_RECURRING_MOTO						'2'		/* Recurring mail or telephone order transaction				*/
#define TK_C0_EC_INSTLMNT_MOTO						'3'		/* Mail or telephone order installment payment					*/
#define TK_C0_EC_UNKNOWN_MOTO						'4'		/* Mail or telephone order unknown classification				*/
#define TK_C0_EC_SET_WITH_CC						'5'		/* Secure electronic transaction with cardholder certificate	*/
#define TK_C0_EC_SET_WITHOUT_CC						'6'		/* Secure electronic transaction without cardholder certificate	*/
#define TK_C0_EC_ENCRYPTED_EC						'7'		/* Encrypted electronic commerce transaction					*/
#define TK_C0_EC_NON_SECURE_EC						'8'		/* Non-secure electronic commerce transaction					*/
#define TK_C0_EC_NON_SET_MER_SET					'9'		/* Non-authenticated security transaction that does not comply with secure	
																electronic transaction and the merchant supports secure electronic transaction.	*/
#define TK_C0_EC_INTERNET_ELECTRONIC_DELIVERY		'S'		/* Internet electronic delivery									*/
#define TK_C0_EC_INTERNET_PHYSICAL_DELIVERY			'T'		/* Internet physical delivery									*/


#define AUTHN_COLL_IND_NOT_SUPPORTED		'0'		/* Universal Cardholder Authentication field (UCAF) data collection is not supported 
														at the merchant’s Web site */
#define AUTHN_COLL_IND_NOT_PRESENT			'1'		/* Universal Cardholder Authentication field (UCAF) data collection is supported 
														by the merchant, but UCAF data was not populated. */
#define AUTHN_COLL_IND_PRESENT				'2'		/* Universal Cardholder Authentication field (UCAF) data collection is supported 
														by the merchant, and UCAF data was populated */


#define CVV2_PR_NOT_PROVIDED		'0'		/* CVV2 value is deliberately bypassed or not provided by the merchant	*/
#define CVV2_PR_PRESENT				'1'		/* CVV2 value is present						*/
#if defined(B24_NCCC)
#define CVV2_PR_ILLEGIBLE			'2'		/* CVV2 value is on the card, but is illegible	*/
#define CVV2_PR_NO_CVV2				'9'		/* C/H states that the card has no CVV2 imprint	*/
#elif defined(B24_EVERTEC)
#define CVV2_PR_NO_EXPIRY_DATE		'3'		/* The expiration date is not required to be sent from the interchange, 
												so the CVD2 field cannot be validated. */
#define CVV2_PR_NO_CVV2				'4'		/* Cardholder states that the card has no CVV2 imprint.	*/
#endif

#define CAVV_AVV_RSLT_ERR_DATA		'0' 	/* Not validated due to erroneous data						*/
#define CAVV_AVV_RSLT_FAILED		'1' 	/* Failed validation										*/
#define CAVV_AVV_RSLT_PASSED		'2' 	/* Passed validation										*/
#define CAVV_AVV_RSLT_NO_EAF		'3' 	/* CAVV/AVV validation could not be performed (no EAF)		*/
#define CAVV_AVV_RSLT_SYS_ERR		'4' 	/* CAVV/AVV validation could not be performed due to system error, 
											or failure prevented authentication (error accessing EAF)	*/
#define CAVV_AVV_RSLT_ISS_NS		'5' 	/* The acquirer is participating in authentication, but the issuer is not participating.	*/
#define CAVV_AVV_RSLT_ISS_NV		'6' 	/* The issuer BIN is participating in authentication, but not verification.	*/
#define CAVV_AVV_RSLT_DUPLICATE		'7' 	/* Duplicate CAVV/AAV										*/


/* Token 04 Card verif flag */

#define CRD_VRF_FLG_CVD_INVALID_CONT		'C'	/* Card verification was performed and the card verification digits (CVD) were invalid*/
#define CRD_VRF_FLG_CVD_INVALID_DENY		'D' /* Card verification was performed and CVD was invalid. The transaction was denied
												and the ERR-FLG field was set to C.*/
#define CRD_VRF_FLG_DEV_ERROR				'N' /* Could not verify the CVD due to a security device error */
#define CRD_VRF_FLG_CVD_MISSING				'O' /* CVD checking was not performed because the CVD was not on the card	*/
#define CRD_VRF_FLG_ISS_NA					'U' /* The issuer has not certified or has not provided the encryption keys to the interchange*/
#define CRD_VRF_FLG_CVD_VALID				'Y' /* Card verification was performed and the CVD was valid*/

#if !defined(B24_NCCC)

#define CRD_VRF_FLG_CHECK_NOEXEC			'0' /* Card verification was not performed	because the transaction was denied
												before card verification processing started*/
#define CRD_VRF_FLG_TRACK_LEN_ERROR			'J' /* CVV checking was not performed. The track length was in error.*/
#define CRD_VRF_FLG_TRACK_LEN_ERR_REFER		'K' /* Card verification was not performed. The track length was in error. The situation
												was noted and the transaction was referred (POS only)*/
#define CRD_VRF_FLG_TRACK_LEN_ERR_CONT		'L' /* CVV checking was not performed. The track length was in error. The BAD
												TRACK ACTION flag in the CPF indicates that processing should continue*/
#define CRD_VRF_FLG_CVD_NOT_PRESENT			'P' /* Card verification was not performed; CVD was not on the card*/
#define CRD_VRF_FLG_CVD_INVALID_REFER		'R' /* Card verification was performed and the CVD was invalid. The situation was noted
												and the transaction should be referred*/

#endif

#define SAF_OR_FP_IS_FORCE_POST				'F'
#define SAF_OR_FP_IS_SAF					'S'

/******************************************************************************/
#define TK_06_01_CLEAR_PIN				'0'
#define TK_06_01_PIN_BLK_ANSI			'1'

#if defined(B24_NCCC)
#define TK_06_01_PIN_BLK_PINPAD			'2'
#elif defined(B24_EVERTEC)

#define TK_06_01_PIN_BLK_PINPAD			'3'

#elif defined(B24_NBK)

#define TK_06_01_PIN_BLK_PINPAD			'3'
#elif defined(B24_PROSA)
#define TK_06_01_PIN_BLK_PINPAD                 '3'/*NAB08102020 PLUTONL-2288 To Confirm*/

#endif

/******************************************************************************/
/******************************************************************************/
typedef struct {
	int		tokenElemId;
	int		nIsUsed;
} token_item_def;
/******************************************************************************/
/******************************************************************************/
typedef struct
{						
   int		nOffset;
   int		nLength;
   char*   sDescription;
} token_info_type;

typedef struct
{						
   char*					sTokenName;
   int						nMaxLength;
   char*					sDescription;
   const token_info_type*	tab_token_info_type;
   int						nNbTokens;
   int						nTokenLen;
}tokens_info_type;


typedef struct {

	int				nLength;
	int				nNbTokens;
	char			sData			[ B24_MAX_LEN_TOKENS];
	int				nTokenPos		[ B24_MAX_NB_TOKENS];
	int				nTokenLength	[ B24_MAX_NB_TOKENS];
	int				nPresent		[ B24_MAX_NB_TOKENS ];
} TSTokens;

extern void InitTokens(TSTokens* tokens);
extern int AnalyseTokens(char* buffer_in, TSTokens* tokens);
extern int GetToken(const char* tokenId, TSTokens* tokens, char* data, int* length);
extern int AddToken(const char* tokenId, TSTokens* tokens, char* data, int length);
extern int PutToken(const char* tokenId, TSTokens* tokens, char* data, int length);
extern int BuildTokens(char* buffer_out,TSTokens* tokens);
extern void DumpTokens(TSTokens* tokens);


/******************************************************************************/
/******************************************************************************/
typedef struct {

	char				sTokenId[4];
	int					nTokenIndice;
	int					nNbTokens;
	int					nLength;
	char				sData			[ B24_MAX_LEN_TOKENS];
	int					nPos			[ B24_MAX_TOKEN_NB_ITEMS];
	int					nPresent		[ B24_MAX_TOKEN_NB_ITEMS ];
	const token_info_type*     tab_token_info_type;
} TSToken;


extern int InitTokenType(char* sTokenName, TSToken* token);
extern int InitTokenId(int TokenId, TSToken* token);
extern int AnalyseToken(char* buffer_in, TSToken* token);
extern int GetTokenElement(int tokenElemId, TSToken* token, char* data, int* length);
extern int AddTokenElement(int tokenElemId, TSToken* token, char* data, int length);
extern int PutTokenElement(int tokenElemId, TSToken* token, char* data, int length);
extern int BuildToken(char* buffer_out,TSToken* token);
extern void DumpToken(TSToken* token);
/******************************************************************************/
extern const char* GetTokenName(int tokenId);
extern int	GetTokenElementLength(int tokenId, int tokenElemId);


#endif





