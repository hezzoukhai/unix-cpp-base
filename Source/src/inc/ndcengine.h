  /***********************************************************************
  **  nom	: ndcengine.h						**
  **  desc	: declaration des constantes pour l'automate NDC	**
  **  auteur	: Younes OUADI (04 Nov 1998)				**
  **			(C) 1998 Hightech Payment Systems		**
  ***********************************************************************/

#ifndef HPS_NDCENGINE_H
#define HPS_NDCENGINE_H

#define	TRANSACTION_REQ				37
#define	CAISSE_CPT				38
#define	SEND_CONFIG				39

#define	TRX_TO_NW_CODE_BASE			    500
#define	TRX_AUT_REQ_TO_NW			    (TRX_TO_NW_CODE_BASE + 1)
#define	TRX_BAL_REQ_TO_NW			    (TRX_TO_NW_CODE_BASE + 2)
#define	TRX_STAT_REQ_TO_NW			    (TRX_TO_NW_CODE_BASE + 3)
#define	TRX_CHEQ_REQ_TO_NW			    (TRX_TO_NW_CODE_BASE + 4)
#define	TRX_TFR_REQ_TO_NW			    (TRX_TO_NW_CODE_BASE + 5)
#define	TRX_REJ_REQ_TO_NW			    (TRX_TO_NW_CODE_BASE + 6)

#define	TRX_FROM_NW_CODE_BASE			600
#define	TRX_AUT_REP_FROM_NW			    (TRX_FROM_NW_CODE_BASE + 1)
#define	TRX_BAL_REP_FROM_NW			    (TRX_FROM_NW_CODE_BASE + 2)
#define	TRX_STAT_REP_FROM_NW		    (TRX_FROM_NW_CODE_BASE + 3)
#define	TRX_STATEMENT_REP_FROM_NW	    (TRX_FROM_NW_CODE_BASE + 10)  /***MKB230508**UPS QRF**/
#define	TRX_CHEQ_REP_FROM_NW		    (TRX_FROM_NW_CODE_BASE + 4)
#define	TRX_TFR_REP_FROM_NW			    (TRX_FROM_NW_CODE_BASE + 5)

#define	TRX_REJ_REP_FROM_NW			    (TRX_FROM_NW_CODE_BASE + 6)
#define TRX_BILL_PAY_REP_FROM_NW        (TRX_FROM_NW_CODE_BASE + 7)
#define TRX_PIN_CHANGE_REP_FROM_NW      (TRX_FROM_NW_CODE_BASE + 8)
#define TRX_PIN_UNBLOCK_REP_FROM_NW     (TRX_FROM_NW_CODE_BASE + 9)
#define	TRX_STATEMENT_REP_FROM_NW	    (TRX_FROM_NW_CODE_BASE + 10)  
#define TRX_DEP_REP_FROM_NW             (TRX_FROM_NW_CODE_BASE + 11) /***MKB070508**/
#define TRX_ACCT_REP_FROM_NW            (TRX_FROM_NW_CODE_BASE + 12) /***MKB070508**/
#define TRX_DON_REP_FROM_NW             (TRX_FROM_NW_CODE_BASE + 13)
#define TRX_DCC_INQUIRY_REP_FROM_NW     (TRX_FROM_NW_CODE_BASE + 14)
#define TRX_CASHSEND_REP_FROM_NW        (TRX_FROM_NW_CODE_BASE + 15)
#define TRX_CASHOUT_REP_FROM_NW         (TRX_FROM_NW_CODE_BASE + 16)
#define	TRX_CP_TFR_REP_FROM_NW		    (TRX_FROM_NW_CODE_BASE + 17) /*RJE 23032014*/



#define	MAX_LABEL_LEN				30

#define MAX_ORDER_NBR               51
#define MAX_STATUS_NBR              8

typedef struct TOrder{
    int		fOrder;
    char	fLabel	[MAX_LABEL_LEN];
} SOrder;

typedef struct TStatus{
    int		fStatus;
    char	fLabel	[MAX_LABEL_LEN];
} SStatus;

typedef struct moteur_t
{
        int     initial_status;
        int     event ;
        int     (*processing)() ;
        int     next_status;
} engine;

typedef struct ubp_t
{
        int     initial_status;
        int     event ;
        int     (*processing)() ;
        int     next_status;
} ubp_engine;

#endif /*** HPS_NDCENGINE_H ***/
