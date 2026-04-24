/***********************************************************************
** nom	: atmopcode.h						**
** desc	: fichier de definition des constantes de		**
**		codification du menu des GAB NDC			**
** auteur	: Younes OUADI (25 Oct 1998)				**
**				(c) 1998 Hightech Payment Systems	**
***********************************************************************/

/* - code operation demandee (OR) ---------------------------------- */
/* - Divers -------------------- */
#ifndef HPS_ATMOPCODE_H
#define HPS_ATMOPCODE_H

#define OR_BAL						9

#define OR_TIMEOUT					10
#define OR_CANCEL					11
#define OR_END						12
#define OR_DISCONECT				13
#define OR_CLOSE                    14
#define OR_NOTICKET                 20
#define OR_NOCASEETTE               30
#define OR_NOTICKETCASSETTE         40
#define	OR_ERROR					-1




#define OR_ONUS_AUTHENTICATION		1111	/* OR_AUTHENTICATION */


#define OR_ONUS_MESSAGING		1116		/* OR_MESSAGING */
#define	OR_ONUS_ACCOUNT_LIST	1117		/* OR_ACCOUNT_LIST */
#define	OR_RECONC_DATA_REQ		1118



/* - Withdrawal ---------------- */
#define OR_ONUS_WITHDRAWAL			1120
#define OR_NAT_WITHDRAWAL			1121
#define OR_INT_WITHDRAWAL			1122
#define OR_ONUS_SND_CUR_WITHDRAWAL	1123
#define OR_ONUS_TRD_CUR_WITHDRAWAL	1124
#define OR_INT_DCC_CHECK			1126		/* internal only */
#define OR_INT_DCC_WITHDR           1127		/* from terminal */
#define OR_ACCESS_FEES_CHECK		1128		/* internal only */

/* - Alternative Amount -------- */
/*
#define OR_ALTERNATIVE_AMOUNT			1125
#define OR_OTHER_ALTERNATIVE_AMOUNT		1126
#define OR_WRONG_ALTERNATIVE_AMOUNT		1127
*/

/* - Balance ------------------- */
#define OR_ONUS_BALANCE		    	1130	/* OR_BALANCE */
#define OR_NAT_BALANCE				1131	/* OR_BALANCE */
#define OR_INT_BALANCE				1132	/* OR_BALANCE */
#define OR_CR_BALANCE				1133

/*#define OR_CREDIT_DEPOSIT_DETAIL	1150*/	/*SWI04062014 CARDLESS CREDIT DEPOSIT DTAIL*/
/*#define OR_DEBIT_DEPOSIT_DETAIL		1151*/	/*SWI04062014 CARDLESS DEBIT DEPOSIT DETAIL*/


#define  OR_ONUS_CASHSEND          1102  /*MISE A DISPOSITION  OPERATION  CASHSEND */
#define  OR_ONUS_CASHOUT          1103  /* MISE A DISPOSITION   OPERATION  CASHOUT  */

/* - Shrt Statement ------------ */
#define	OR_ONUS_SHORT_STAT			1140

/* - Request ------------------- */
#define	OR_ONUS_CHEQ_BOOK			1160
/*#define	OR_ONUS_CHEQ_BOOK_MAIL		1167*/	/* OR_CHEQUE_REQ */
#define	OR_ONUS_STAT_REQ			1161
/*#define	OR_ONUS_LOAN_INQUIRY		1162
#define	OR_ONUS_SND_LOAN_INQ		1163
*/

/* - Transfer ------------------ */
/*
#define	OR_ONUS_PRE_INT_TFR     	1170
#define	OR_ONUS_PRE_EXT_TFR			1171	*/
#define	OR_ONUS_INT_TFR				1172	/* OR_FUNDS_TRANSFER */
#define	OR_ONUS_EXT_TFR				1173	/* OR_EXT_TRANSFER */

/*#define	OR_ONUS_E_REMITTANCE			1174	/ * OR_E_REMITTANCE RJE 16032014 */
/* #define	OR_ONUS_HAYRAT_TFR			1176	/ * OR_HAYRAT_TRANSFER  RJE 16032014*/
/*
#define	OR_ONUS_PRE_TFR_TO_PP		1174    / * PRE FT To a Prepaid Card RJE 16032014 * /
#define	OR_ONUS_PRE_TFR_TO_CR		1176    / * PRE FT To a Credit Card (Credit card payement) RJE 16032014 * /
#define	OR_ONUS_EXT_TFR_TO_PP		1177    / * FT To a Prepaid Card TBO24112011 * /
#define	OR_ONUS_EXT_TFR_TO_CR		1178    / * FT To a Credit Card (Credit card payement) RJE 16032014 * /
#define OR_ONUS_PRE_TFR_TO_CR_BY_CC 1179    / * Credit payment service by Account for Credit card in ATM  RJE15062014 * / 
*/

/*-----------------------------------*/ 
/* Start TBO23112010 CREDIT CARD FCT */
/*-----------------------------------*/ 
/*
#define OR_ONUS_CR_PIN_CHANGE		      1134
#define OR_ONUS_CR_CASH_DPT  		      1136
#define OR_ONUS_CR_CHEQ_DPT  		      1137
#define OR_ONUS_CR_INT_TFR			      1138
*/
/*
#define OR_ONUS_CR_PRE_CASH_DPT        1139
#define OR_ONUS_CR_PRE_CHEQ_DPT        1142
*/
/* End TBO23112010 CREDIT CARD FCT */


/* - Deposit ------------------- */
#define OR_ONUS_PRE_CASH_DPT        1180
#define OR_ONUS_PRE_CHEQ_DPT        1181
#define OR_ONUS_PRE_DOC_DPT         1182
#define OR_ONUS_CASH_DPT                        1183
#define OR_ONUS_CHEQ_DPT                        1184
#define OR_ONUS_DOC_DPT                         1186
#define OR_ONUS_PRE_MIX_DPT         1187
#define OR_ONUS_MIX_DPT                         1188

/* - Pin Change ---------------- */
/*#define		OR_ONUS_PRE_PIN_CHANGE	1190*/
#define 	OR_ONUS_PIN_CHANGE		1191
#define 	OR_NAT_PIN_CHANGE		1192
#define		OR_INT_PIN_CHANGE		1193

#define OR_ONUS_PIN_UNBLOCK			1196
#define OR_NAT_PIN_UNBLOCK			1197
#define OR_INT_PIN_UNBLOCK			1198



/* - Spec CBK ------------------------------------------------------------- */
/* - general functions -------------------------- */
/*#define	OR_FAST_CASH				9000*/
/*#define	OR_LOAN_BALANCE				4490*/


#define OR_ONUS_HOST_ACCOUNTS 			4482
/*
#define	OR_CASH_US					4100
*/

/* - autobank plus ------------------------------ */
/*
#define	OR_AUTO_CASH				4690
#define	OR_AUTO_BALANCE				4680
#define	OR_AUTO_SHORTSTAT			4670
*/

/* - other functions ---------------------------- */
/*
#define OR_BILL_PAYMENT             4441
#define OR_LOAD_BILLING             4442
#define OR_LOAD_SERVICES            4443  / * RJE 15032014 * /
#define OR_LOAD_UTILITY             4444   / * RJE 15032014 * /
#define	OR_DRAFT_PRINTING			4490
#define	OR_PAYMENT_DESTINATION_LIST	9002
*/
         


/*--- E-Banking demande de recharge ----*/
/*
#define OR_PRE_BILL_PAYMENT_RECHG   4000
#define OR_BILL_PAYMENT_RECHAG      4001
#define OR_PRE_BILL_PAYMENT_ELECTRICITY 4002
#define OR_PRE_BILL_PAYMENT_CREDIT      4003
*/


#define OR_UBP_INQUIRY				9000



/**Begin MKB290408**/
/*--- Online Cash Deposit and Cheque Scan */
#define OR_ACCOUNT_ONLINE_CASH       7002
/*#define OR_ACCOUNT_LIST_ONL_CASH     7003*/




/**End MKB290408**/
/**Begin MKB140508**/
/*#define OR_ONUS_CARDLESS_CASH_D           7007*/  /*RJE23052014 for debit card */
/*#define OR_ONUS_CARDLESS_CASH_CARD        7020*/  /*RJE23052014 BUFFER B Contains Debit Card Number*/
/*#define OR_ONUS_CARDLESS_CASH_C           7707*/  /*RJE23052014 for credit card */
/*#define OR_ONUS_CARDLESS_CHEQUE_D         7008*/
/*#define OR_ONUS_LOAD_CR_ACC				  7009*/
/**End MKB140508**/

/*Start MUW 05052010*/
/*
#define OR_ACCOUNT_ONLINE_CHECK_DEPOSIT   7011
#define OR_ONUS_CHECK_ACCOUNT_CHEQUE      7012                
#define OR_ONUS_CARD_CHEQUE_D             7013 
#define OR_ONUS_CHECK_ACCOUNT_SEL         7014 
#define OR_ONUS_CHQ_ACCOUNT_SEL           7017   
#define OR_ACC_ONLINE_CR_CHECK_DEPOSIT    7018             */
/*End MUW 05052010*/ 

/*#define OR_ACC_ONLINE_CASH_CR             7027*/   	  /* CREDIT CARD, To CREDIT CARD DEPOSIT                */
/*#define OR_ACC_LIST_ONL_CASH_CR_TO_CR     7028*/	      /* CREDIT CARD,LOAD CUSTOMER CREDIT ACCOUNT LIST      */
/*#define OR_ACCOUNT_ONLINE_CASH_WN         7029*/
/*#define OR_ACCOUNT_LIST_ONL_CASH_WN       7030*/


/* - Donation---------------- */
#define OR_ONUS_PRE_DONATION                  1319 
#define OR_ONUS_DONATION                      1320
#define OR_NAT_DONATION                       1321
#define OR_INT_DONATION                       1322
#define OR_LOAD_DONATION                      1326 
#define OR_LOAD_SUB_DONATION				  1327 

#endif /** HPS_ATMOPCODE_H **/
