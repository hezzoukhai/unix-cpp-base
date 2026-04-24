/*
** <Name>
**  mds.h
** 
** <Descr>
**  general header file for "mds" interface
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef PULSE_DEFINE_H
#define PULSE_DEFINE_H

/*....to be completed ! */
#define	C_PULSE_PASSWD			"72172   " /* HAA 08102012 remplace PASWD Password given to authorized Processors */

/* ---- Processing Code ---- */
#define PLS_F003_P1_PURCHASE				"00"	/* Purchase & services  */
#define PLS_F003_P1_WITHDRAWAL				"01"	/* Cash Withdrawal  */
#define PLS_F003_P1_CASHBACK				"09"	/* Purchase with Cash Back  */
#define PLS_F003_P1_AVS						"18"	/* AVS  */
#define PLS_F003_P1_MERCHANDISE_RETURN		"20"	/* Merchandise Return  */
#define PLS_F003_P1_DEPOSIT					"21"	/* Deposit  */
#define PLS_F003_P1_RECHARGE_RELOAD			"28"	/* Recharge / Reload  */
#define PLS_F003_P1_BALANCE_INQ				"31"	/* Balance inquiry  */
#define PLS_F003_P1_BALANCE_INQR			"39"	/* Balance inquiry  */
#define PLS_F003_P1_TRANSFER				"40"	/* Transfer  */
#define PLS_F003_P1_PIN_UNBLOCK				"73"	/* PIN Unblock  */
#define PLS_F003_P1_PIN_CHANGE				"90"	/* PIN Change  */

#define PLS_F003_P2_FND						"00"	/* FND         */
#define PLS_F003_P2_SAV						"10"	/* SAV         */
#define PLS_F003_P2_DDA						"20"	/* DDA         */
#define PLS_F003_P2_CRD						"30"	/* CRD         */
#define PLS_F003_P2_Universal				"40"	/* Universal   */

#define PLS_F003_P3_FND						"00"	/* FND         */
#define PLS_F003_P3_SAV						"10"	/* SAV         */
#define PLS_F003_P3_DDA						"20"	/* DDA         */
#define PLS_F003_P3_CRD						"30"	/* CRD         */
#define PLS_F003_P3_Universal				"40"	/* Universal   */

/* ---- Point-of-Service Entry Mode ---- */
#define PLS_F022_P1_UNSPECIFIED				"00"	/* Unspecified  */
#define PLS_F022_P1_MANUAL					"01"	/* Manual  */
#define PLS_F022_P1_MAGN_STRP				"02"	/* Magnetic Stripe  */
#define PLS_F022_P1_BAR_CODE				"03"	/* Bar code  */
#define PLS_F022_P1_OCR						"04"	/* OCR   */
#define PLS_F022_P1_ICC						"05"	/* Integrated circuit card (ICC)  */
#define PLS_F022_P1_MANUAL_KEY_ENTR			"06"	/* Manual (key entered)   */
#define PLS_F022_P1_CLSS_ICC				"07"	/* Contactless via chip data rules   */
#define PLS_F022_P1_ECOM_REMOTE_CHIP		"09"	/* PAN entry via electronic commerce, including remote chip   */
#define PLS_F022_P1_PAN_ENTR_MANUAL			"79"	/* PAN entered manually  */
#define PLS_F022_P1_PAN_ENTR_MAGN_STRP		"80"	/* PAN entered via magnetic stripe  */
#define PLS_F022_P1_FROM_FILE				"81"	/* From file  */
#define PLS_F022_P1_PAN_AUT_SERVER			"82"	/* PAN auto entry via Server (issuer, acquirer or third party system) */
#define PLS_F022_P1_FULL_MAGN_STRP			"90"	/* Full magnetic stripe read  */
#define PLS_F022_P1_CLSS_MAGN_STRP			"91"	/* Contactless via magnetic stripe data rules  */
#define PLS_F022_P1_ICC_CVV_UNRELIABLE		"95"	/* Integrated Circuit Card (ICC), CVV data may be unreliable  */

#define PLS_F022_P2_UNSPECIFIED				"0"	/* Unspecified  */
#define PLS_F022_P2_PIN_ENTRY				"1"	/* PIN entry capability  */
#define PLS_F022_P2_NO_PIN_ENTRY			"2"	/* No PIN entry capability  */
#define PLS_F022_P2_PIN_PAD_INOP			"6"	/* PIN pad inoperative  */
#define PLS_F022_P2_PIN_PAD_DOWN			"8"	/* Terminal PIN Pad is Down  */
#define PLS_F022_P2_PIN_VERIF_BY_TERM		"9"	/* PIN verification by terminal device  */


/* ---- Response Codes ---- */
#define PLS_F039_APPROVED 					"00"	/*	Approved or completed successfully	*/
#define PLS_F039_REF_CARD_ISS 				"01"	/*	Refer to card issuer	*/
#define PLS_F039_REF_CARD_ISS_SPEC 			"02"	/*	Refer to card issuers special conditions	*/
#define PLS_F039_INVALID_MERCH 				"03"	/*	Invalid merchant	*/
#define PLS_F039_PICKUP 					"04"	/*	Pick-up	*/
#define PLS_F039_NOT_HONOR 					"05"	/*	Do not honor	*/
#define PLS_F039_ERROR 						"06"	/*	Error	*/
#define PLS_F039_PICKUP_SPEC 				"07"	/*	Pick-up card, special conditions	*/
#define PLS_F039_HONOR_WITH_IDENT 			"08"	/*	Honor with identification	*/
#define PLS_F039_IN_PROGRESS 				"09"	/*	Request in progress	*/
#define PLS_F039_APPROVED_PARTIAL 			"10"	/*	Approved for partial amount	*/
#define PLS_F039_APRROVED_VIP 				"11"	/*	Approved (VIP)	*/
#define PLS_F039_INVALID_TRNS 				"12"	/*	Invalid transaction	*/
#define PLS_F039_INVALID_AMOUNT 			"13"	/*	Invalid amount	*/
#define PLS_F039_INVALID_CARD_NUM 			"14"	/*	Invalid card number (no such number)	*/
#define PLS_F039_NO_SUCH_ISS 				"15"	/*	No such issuer	*/
#define PLS_F039_APPROVED_UPD_TRACK3 		"16"	/*	Approved, update Track 3	*/
#define PLS_F039_REVERSAL 					"17"	/*	Customer cancellation, reversal (unsupported)	*/
#define PLS_F039_REENTER_TRNS 				"19"	/*	Re-enter transaction	*/
#define PLS_F039_INVALID_RESPONSE 			"20"	/*	Invalid response	*/
#define PLS_F039_UNACCEPT_TRNS_FEE 			"23"	/*	Unacceptable transaction fee	*/
#define PLS_F039_FILE_UPD_NOT_SUPP 			"24"	/*	File update not supported by receiver (0302 message only)	*/
#define PLS_F039_UN_LOCATE_REC_ON_FILE 		"25"	/*	Unable to locate record on file (0302 message only)	*/
#define PLS_F039_DUP_FILE_UPD_REC 			"26"	/*	Duplicate file update record, no action (0302 message only)	*/
#define PLS_F039_FILE_UPD_FILED_ERROR 		"27"	/*	File update field edit error (0302 message only)	*/
#define PLS_F039_FILE_UPD_REC_LOCKED 		"28"	/*	File update record locked out (0302 message only)	*/
#define PLS_F039_FILE_UPD_NOT_SUCCESS 		"29"	/*	File update not successful, contact acquirer (0302 message only)	*/
#define PLS_F039_FORMAT_ERROR 				"30"	/*	Format error (may also be a reversal)	*/
#define PLS_F039_BANK_NOT_SUPPORTED 		"31"	/*	Bank not supported by switch	*/
#define PLS_F039_EXPIRED_CARD 				"33"	/*	Expired card, pick-up	*/
#define PLS_F039_SUSPECTED_FRAUD 			"34"	/*	Suspected fraud, pick-up	*/
#define PLS_F039_CARD_ACC_CONT_ACQ 			"35"	/*	Card acceptor contact acquirer, pick-up	*/
#define PLS_F039_RESTRICTED_CARD 			"36"	/*	Restricted card, pick-up	*/
#define PLS_F039_CARD_ACC_CALL_ACQ 			"37"	/*	Card acceptor call acquirer security, pick-up	*/
#define PLS_F039_PIN_TRIE_EXCEEDED 			"38"	/*	Allowable PIN tries exceeded, pick-up	*/
#define PLS_F039_SYS_MALFUNCTION 			"96"	/*	Allowable PIN tries exceeded, pick-up	*/



/* ---- National Point-Of-Service Condition Code ---- */
#define PLS_F058_P1_ATTENDED					"0"  /*	  Attended				*/
#define PLS_F058_P1_UNATTENDED					"1"  /*	  Unattended			*/

#define PLS_F058_P2_CUSTOMER					"0"  /*	  Customer-operated			*/
#define PLS_F058_P2_CARD_ACCEPTOR				"1"  /*	  Card Acceptor-operated	*/
#define PLS_F058_P2_ADMINISTRATIVE				"2"  /*	  Administrative			*/

#define PLS_F058_P3_ON_PREMISE					"0"  /*	  On premise		*/
#define PLS_F058_P3_OFF_PREMISE					"1"  /*	  Off premise		*/

#define PLS_F058_P4_CSTM_PRESENT				"0"  /*   Customer present		*/
#define PLS_F058_P4_CSTM_NOT_PRESENT			"1"  /*	  Customer not present		*/
#define PLS_F058_P4_MAIL_TEL_ORDER				"2"  /*	  Mail and/or telephone order (customer not present)		*/
#define PLS_F058_P4_TELEPHONE					"3"  /*	  Telephone		*/
#define PLS_F058_P4_RECCURING_PYMNT				"4"  /*	  Recurring payment		*/
#define PLS_F058_P4_PREAUTH_PURCHASE			"8"  /*	  Pre-Authorized purchase (must be present on pre-authorized purchases)		*/
#define PLS_F058_P4_DEFERRED_BILLING			"9"  /*	  Deferred Billing		*/
#define PLS_F058_P4_INSTALLMNT_PYMNT			"S"  /*	  Installment Payment		*/

#define PLS_F058_P5_CARD_PRESENT				"0"  /*   Card present		*/
#define PLS_F058_P5_CARD_NOT_PRESENT			"1"  /*	  Card not present		*/
#define PLS_F058_P5_PREAUTH_PURCHASE			"8"  /*	  Pre-Authorized purchase (must be present on pre-authorized purchases)		*/

#define PLS_F058_P6_NO_CARD_RTNTN_CPLTY			"0"  /*   Device does not have card retention capability		*/
#define PLS_F058_P6_CARD_RTNTN_CPLTY			"1"  /*	  Device has card retention capability		*/

#define PLS_F058_P7_ORGNL_PRSNTMNT				"0"  /*	  Original presentment				*/
#define PLS_F058_P7_FIRST_PRSNTMNT				"1"  /*	  First re-presentment				*/
#define PLS_F058_P7_SECOND_PRSNTMNT				"2"  /*	  Second re-presentment				*/
#define PLS_F058_P7_THIRD_PRSNTMNT				"3"  /*	  Third re-presentment				*/
#define PLS_F058_P7_PRVS_AUTHORIZED_REQ			"4"  /*	  Previously authorized request				*/
#define PLS_F058_P7_RESUBMISSION				"5"  /*	  Resubmission				*/
#define PLS_F058_P7_ACCOUNT_STATUS				"8"  /*	  Card Validation/Account status inquiry				*/

#define PLS_F058_P8_NO_SECURITY					"0"  /*	  No security concern	*/
#define PLS_F058_P8_SUSPECT_FRAUD				"1"  /*	  Suspected fraud	*/
#define PLS_F058_P8_IDENTIF_VERIFIED			"2"  /*	  Identification verified	*/
#define PLS_F058_P8_ELCTRNC_COMM_TRNS			"3"  /*	  Electronic Commerce transaction with digital signature	*/
#define PLS_F058_P8_NON_SECURE					"4"  /*	  Non-secure/Security unknown electronic commerce transaction	*/
#define PLS_F058_P8_SCR_ELEC_COMM_WTH			"5"  /*	  Secure electronic commerce with cardholder certificate	*/
#define PLS_F058_P8_SCR_ELEC_COMM_WTHT			"6"  /*	  Secure electronic commerce without cardholder certificate	*/
#define PLS_F058_P8_CHANNEL_ENCRYPT				"7"  /*	  Channel encrypted electronic commerce transaction	*/
#define PLS_F058_P8_CVV_VALID					"8"  /*	  CVV/CVC validated and valid	*/
#define PLS_F058_P8_CVV_INVALID					"9"  /*	  CVV/CVC validated and invalid	*/
#define PLS_F058_P8_INTRNT_PIN_D				"A"  /*	  Internet PIN Debit (IPD)	*/

#define PLS_F058_P9_ADM_TERM					"00" /*   Administrative Terminal		*/
#define PLS_F058_P9_POS_TERM					"01" /*   POS Terminal		*/
#define PLS_F058_P9_ATM_TERM					"02" /*   ATM		*/
#define PLS_F058_P9_HOME_TERM					"03" /*   Home Terminal		*/
#define PLS_F058_P9_ECR							"04" /*   ECR		*/
#define PLS_F058_P9_DIAL_TERM					"05" /*   Dial Terminal		*/
#define PLS_F058_P9_TRVELERS_CHECK				"06" /*   Travelers Check Machine		*/
#define PLS_F058_P9_FUEL_MACHINE				"07" /*   Fuel Machine		*/
#define PLS_F058_P9_SCRIP_MACHINE				"08" /*   Scrip Machine		*/
#define PLS_F058_P9_COUPON_MACHINE				"09" /*   Coupon Machine		*/
#define PLS_F058_P9_TICKET_MACHINE				"10" /*   Ticket Machine		*/
#define PLS_F058_P9_POB_TERM					"11" /*   Point-of-Banking Terminal		*/
#define PLS_F058_P9_TELLER						"12" /*   Teller		*/
#define PLS_F058_P9_FARNCHISE_TELLER			"13" /*   Franchise Teller		*/
#define PLS_F058_P9_PERONAL_BANKING				"14" /*   Personal Banking		*/
#define PLS_F058_P9_PUBLIC_UTILITY				"15" /*   Public Utility		*/
#define PLS_F058_P9_VEDING						"16" /*   Vending		*/
#define PLS_F058_P9_SELF_SERVICE				"17" /*   Self Service		*/
#define PLS_F058_P9_AUTHORIZATION				"18" /*   Authorization		*/
#define PLS_F058_P9_PAYMENT						"19" /*   Payment		*/
#define PLS_F058_P9_VRU							"20" /*   VRU		*/
#define PLS_F058_P9_SMARTPHONE					"21" /*   Smart Phone		*/
#define PLS_F058_P9_INTERACTIVE_TELEV			"22" /*   Interactive Television		*/
#define PLS_F058_P9_PDA							"23" /*   Personal Digital Assistant (PDA)		*/
#define PLS_F058_P9_TABLET						"24" /*   Screen Phone (tablet)		*/
#define PLS_F058_P9_ECOMMERCE			"25" /*   Electronic Commerce		*/
#define PLS_F058_P9_MISC_TERM_AT_POS			"26" /*   MICR terminals at POS		*/

#define PLS_F058_P10_UNKNOWN					"0"  /*   Unknown   */
#define PLS_F058_P10_MANUAL						"1"  /*   Manual, no terminal   */
#define PLS_F058_P10_MAG_READ					"2"  /*   Magnetic stripe   */
#define PLS_F058_P10_BARCODE					"3"  /*   Bar Code   */
#define PLS_F058_P10_OCR						"4"  /*   OCR   */
#define PLS_F058_P10_ICC						"5"  /*   Integrated Circuit Card (ICC)   */
#define PLS_F058_P10_KEY_ENTRED					"6"  /*   Key Entered   */
#define PLS_F058_P10_FILE						"9"  /*   File   */
#define PLS_F058_P10_CONTACTLESS_MGN_STRP		"A"  /*   Contactless Read Capability via Magnetic Stripe Rules   */
#define PLS_F058_P10_CONTACTLESS_CHIP			"M"  /*   Contactless read capability via Chip Rules   */
#define PLS_F058_P10_MGN_KEY					"S"  /*   Magnetic stripe reader and key entry   */
#define PLS_F058_P10_MGN_KEY_ICC				"T"  /*   Magnetic stripe reader and key entry and EMV-compatible ICC reader   */
#define PLS_F058_P10_MGN_ICC					"U"  /*   Magnetic stripe and key EMV-compatible ICC reader   */


/* ---- Advice/Reversal Reason Codes ---- */
#define PLS_F060_P2_NOT_REVERSAL				"00"  /*	  Not a reversal				*/
#define PLS_F060_P2_CA_TERM_ERROR				"16"  /*	  Terminal error				*/
#define PLS_F060_P2_CA_NOT_SPECIFIED			"18"  /*	  Terminal processor error				*/
#define PLS_F060_P2_ACQ_TERM_PROC_ERROR			"22"  /*	  Customer cancellation				*/
#define PLS_F060_P2_ACQ_TERM_COMM_ERROR			"25"  /*	  Terminal communication error				*/
#define PLS_F060_P2_ACQ_LATE_RESP				"27"  /*	  Late or unsolicited response � inbound				*/
#define PLS_F060_P2_ACQ_CSTM_CANCEL				"28"  /*	  Customer cancellation				*/
#define PLS_F060_P2_IF_TERM_PROC_ERROR			"32"  /*	  Customer cancellation				*/
#define PLS_F060_P2_IF_LATE_RESP				"37"  /*	  Late or unsolicited response � inbound				*/


/* ---- Network Management Information Code ---- */
#define PLS_F070_SIGNON_ISS			"061"  /*	  Sign on as issuer				*/
#define PLS_F070_SIGNON_ACQ			"071"  /*	  Sign on as acquirer				*/
#define PLS_F070_SIGNOFF_ISS		"062"  /*	  Sign off as issuer				*/
#define PLS_F070_SIGNOFF_ACQ		"072"  /*	  Sign off as acquirer				*/
#define PLS_F070_KEY_EXCHG_ISS		"161"  /*	  Issuer key change				*/
#define PLS_F070_KEY_EXCHG_ACQ		"171"  /*	  Acquirer key change				*/
#define PLS_F070_KEY_CHG_REQ_ACQ	"181"  /*	  Acquirer key change request			*/
#define PLS_F070_KEY_CHG_ISS		"191"  /*	  Issuer key change request			*/
#define PLS_F070_I_CUTOFF_ISS		"261"  /*	  Initiate cutoff as issuer			*/
#define PLS_F070_I_CUTOFF_ACQ		"271"  /*	  Initiate cutoff as acquirer			*/
#define PLS_F070_F_CUTOFF_ISS		"262"  /*	  Cutoff complete as issuer			*/
#define PLS_F070_F_CUTOFF_ACQ		"272"  /*	  Cutoff complete as acquirer			*/
#define PLS_F070_ECHO_ISS			"361"  /*	  Issuer echo test (reply required)		*/
#define PLS_F070_ECHO_ACQ			"371"  /*	  Acquirer echo test (reply required)		*/
#define PLS_F070_MSG_ERR			"900"  /*	  Message format/syntax error messages		*/

/* ---- File Update Codes ---- */
#define PLS_F091_NOT_SUPPORTED					"0"  /*	  Not Supported			*/
#define PLS_F091_ADD_REC						"1"  /*	  Not Supported			*/
#define PLS_F091_CHANGE_REC						"2"  /*	  Not Supported			*/
#define PLS_F091_DELETE_REC						"3"  /*	  Not Supported			*/

/* ---------------   LENGTHS   ----------------- */
#define PLS_F002_LEN		19
#define PLS_F003_LEN		6
#define PLS_F003_P1_LEN		2
#define PLS_F003_P2_LEN		2
#define PLS_F003_P3_LEN		2
#define PLS_F004_LEN		12
#define PLS_F005_LEN		12
#define PLS_F006_LEN		12
#define PLS_F007_LEN		10
#define PLS_F009_LEN		8
#define PLS_F010_LEN		8
#define PLS_F011_LEN		6
#define PLS_F012_LEN		6
#define PLS_F013_LEN		4
#define PLS_F014_LEN		4
#define PLS_F015_LEN		4
#define PLS_F016_LEN		4
#define PLS_F017_LEN		4
#define PLS_F018_LEN		4
#define PLS_F019_LEN		3
#define PLS_F022_LEN		3
#define PLS_F022_P1_LEN 	2
#define PLS_F022_P2_LEN 	1
#define PLS_F023_LEN 		3
#define PLS_F026_LEN 		2
#define PLS_F028_LEN 		9
#define PLS_F029_LEN 		9
#define PLS_F032_LEN 		11
#define PLS_F033_LEN 		11
#define PLS_F035_LEN 		37
#define PLS_F037_LEN 		12
#define PLS_F038_LEN 		6
#define PLS_F039_LEN 		2
#define PLS_F040_LEN 		3
#define PLS_F041_LEN 		8
#define PLS_F042_LEN 		15
#define PLS_F043_LEN 		40
#define PLS_F044_LEN 		25
#define PLS_F046_LEN 		88
#define PLS_F047_LEN 		100
#define PLS_F048_LEN 		25
#define PLS_F049_LEN 		3
#define PLS_F050_LEN 		3
#define PLS_F051_LEN 		3
#define PLS_F052_LEN 		16
#define PLS_F054_LEN 		120
#define PLS_F055_LEN 		255
#define PLS_F056_LEN 		88
#define PLS_F057_LEN 		3
#define PLS_F058_LEN 		11
#define PLS_F058_P1_LEN 	1
#define PLS_F058_P2_LEN 	1
#define PLS_F058_P3_LEN 	1
#define PLS_F058_P4_LEN 	1
#define PLS_F058_P5_LEN 	1
#define PLS_F058_P6_LEN 	1
#define PLS_F058_P7_LEN 	1
#define PLS_F058_P8_LEN 	1
#define PLS_F058_P9_LEN 	2
#define PLS_F058_P10_LEN 	1
#define PLS_F059_LEN 		17
#define PLS_F060_LEN 		6
#define PLS_F060_P1_LEN		2
#define PLS_F060_P2_LEN		2
#define PLS_F060_P3_LEN		2
#define PLS_F061_LEN 		100
#define PLS_F062_LEN 		100
#define PLS_F063_LEN 		50
#define PLS_F070_LEN 		3
#define PLS_F090_LEN 		42
#define PLS_F091_LEN 		1
#define PLS_F095_LEN 		42
#define PLS_F096_LEN 		8
#define PLS_F098_LEN 		25
#define PLS_F101_LEN 		17
#define PLS_F102_LEN 		28
#define PLS_F103_LEN 		28
#define PLS_F104_LEN 		100
#define PLS_F105_LEN 		999
#define PLS_F109_LEN 		255
#define PLS_F110_LEN 		255
#define PLS_F111_LEN 		255
#define PLS_F113_LEN 		11
#define PLS_F114_LEN 		3
#define PLS_F120_LEN 		6
#define PLS_F121_LEN 		255
#define PLS_F122_LEN 		11
#define PLS_F123_LEN 		255
#define PLS_F124_LEN 		255
#define PLS_F125_LEN 		255
#define PLS_F126_LEN 		100
#define PLS_F127_LEN 		100

/* start FZL 20220913 PLUTONL-4979*/
#define PLS_F125_NB_FIELDS			1
#define PLS_F110T_KP_KEY_BLOCK	  "KP"

#define PLS_F125_TKP_NB_FIELDS     2
#define PLS_F125_TKP_KEY_DATA     "01"
#define PLS_F125_TKP_CHECK_VALUE  "02"

/* end FZL 20220913 PLUTONL-4979*/

#endif /* PULSE_H */