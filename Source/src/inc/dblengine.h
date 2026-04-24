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

#define	TRX_TO_NW_CODE_BASE			500
#define	TRX_AUT_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 1)
#define	TRX_BAL_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 2)
#define	TRX_STAT_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 3)
#define	TRX_CHEQ_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 4)
#define	TRX_TFR_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 5)
#define	TRX_REJ_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 6)

#define	TRX_FROM_NW_CODE_BASE			600
#define	TRX_AUT_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 1)
#define	TRX_BAL_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 2)
/* YOua 18Avr2004 : On-Line Stat Req Integration */
/*
#define	TRX_STAT_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 3)
*/
#define	TRX_STAT_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 10)
#define	TRX_SHORT_STAT_REP_FROM_NW		(TRX_FROM_NW_CODE_BASE + 3)
/* YOua 18Avr2004 : On-Line Stat Req Integration */
#define	TRX_CHEQ_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 4)
#define	TRX_TFR_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 5)
#define	TRX_REJ_REP_FROM_NW			(TRX_FROM_NW_CODE_BASE + 6)
#define TRX_BILL_PAY_REP_FROM_NW                (TRX_FROM_NW_CODE_BASE + 6)
#define TRX_CR_PAY_REP_FROM_NW                  (TRX_FROM_NW_CODE_BASE + 7)
/*BEGIN AHM 31/08/2006*/
#define     TRX_DEP_REP_FROM_NW                 (TRX_FROM_NW_CODE_BASE + 11)
/*END AHM*/
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

#endif /*** HPS_NDCENGINE_H ***/
