  /***********************************************************************
  ** nom	: ubp_opcode.h						**
  ** desc	: definition of constants specific to UBP		**
  **		  using Dubai bank Webservices			**
  **				(c) 1998 Hightech Payment Systems	**
  ***********************************************************************/


#ifndef HPS_UBPOPCODE_H
#define HPS_UBPOPCODE_H

/* - code operation demandee (OR) ---------------------------------- */
/* - Divers -------------------- */

extern int FCT_UBP_INQUIRY_ATM_DED();
extern int FCT_UBP_PAYMENT_ATM_DED();

/*
#define POS_UBP                 1
#define UBP_START_OPCODE       'I'
*/


/* - ATM DEBIT CARD OP CODES for RTA RECHARGE------------------------------------------------------------- */
#define	OR_UBP_ENQUIRY_DED_ID		  8121 /* -- Utility Bill  FDK - A */
#define	OR_UBP_ENQUIRY_ATM_DED_SUMM_AA    8122 /* -- Utility Bill ENQ FDK - A */
#define	OR_UBP_PAYMENT_DED      	  8123 /* -- Utility Billpayment FDK - A */

#define	OR_UBP_ENQUIRY_DED_DC		  8131 /* -- DED Bill ENQ Debit Card */
#define	OR_UBP_ENQUIRY_ATM_DED_SUMM_DC    8132 /* -- DED Bill ENQ Debit Card */
#define	OR_UBP_PAYMENT_DED_DC   	  8133 /* -- Utility Billpayment DED Bill  Debit Card */

#define	OR_UBP_ENQUIRY_DED_CC	          8141 /* -- DED Bill ENQ Credit Card */
#define	OR_UBP_ENQUIRY_ATM_DED_SUMM_CC    8142 /* -- Utility Bill ENQ FDK - A */
#define	OR_UBP_PAYMENT_DED_CC   	  8143 /* -- Utility Billpayment FDK - A */

/* - ATM COVERED CARD OP CODES for RTA RECHARGE------------------------------------------------------------- *
#define	OR_UBP_ENQUIRY_ATM_CC_DEWA_ID_AA					4711 * -- Utility Bill  FDK - A *
#define	OR_UBP_ENQUIRY_ATM_CC_DEWA      				  4713 * -- Utility Bill ENQ FDK - A *
#define	OR_UBP_ENQUIRY_ATM_CC_DEWA_SUMM_AA      	4714 * -- Utility Bill ENQ FDK - A *
#define	OR_UBP_PAYMENT_ATM_CC_DEWA      				  4718 * -- Utility Billpayment FDK - A *
*/
#define NP_UBP_DED_ATM_TICKET_ENG 	    "710"
#define NP_UBP_DED_ATM_DEC_TICKET_ENG	  "711"
#define NP_UBP_DED_ATM_TICKET_AR 	  		"712"
#define NP_UBP_DED_ATM_DEC_TICKET_AR 		"713"
#define NP_UBP_DED_ATM_JOURNAL_ENG   		"714"

#define NP_UBP_DED_DC_TICKET_ENG 	    	"715"
#define NP_UBP_DED_DC_DEC_TICKET_ENG	  "716"
#define NP_UBP_DED_DC_TICKET_AR 	  		"717"
#define NP_UBP_DED_DC_DEC_TICKET_AR 		"718"
#define NP_UBP_DED_DC_JOURNAL_ENG   		"719"
                   
#define NP_UBP_DED_CC_TICKET_ENG 	      "720"
#define NP_UBP_DED_CC_DEC_TICKET_ENG	  "721"
#define NP_UBP_DED_CC_TICKET_AR 	  		"722"
#define NP_UBP_DED_CC_DEC_TICKET_AR 		"723"
#define NP_UBP_DED_CC_JOURNAL_ENG   		"724"
#define NP_UBP_DED_TIMED_OUT_ENG   		  "725"

typedef struct SNdcUbp
{
   char             gPnr        [256];
   char             gTran_Flag  [5];
   char             gStan 			[256];      
   char             gName       [256];
   char             gTerm_id    [22];
   char             gAccount    [256];
   char             gMobileNbr  [256];
   char             gAmount     [256];
   char             gTotAmount  [256];
   char             gFee     		[256];
   char             gError_id   [256];
   char             gTrans_ID   [256];
   char             gError_desc [256];     
} TSNdcUbp;

/*
TSNdcUbp  tab_NdcUbp [ MAX_THREAD ];
*/

#endif /** HPS_ATMOPCODE_H **/

