  /***********************************************************************
  **  nom	: ndcengine.h						                        **
  **  desc	: declaration des constantes pour l'automate IsoSni	        **
  **  auteur	: youssef KHALOUFI				            **
  **			(C) 1998 Hightech Payment Systems		                **
  ***********************************************************************/

#ifndef HPS_ISO_SNI_ENGINE_H
#define HPS_ISO_SNI_ENGINE_H


#define		TRX_ADMIN_REQ_FROM_GAB    100	 	 
#define		TRX_NETMNG_REQ_FROM_GAB	  101		       	
#define		TRX_PAR_REQ_FROM_GAB	  102        
#define		TRX_PAR_REP_FROM_GAB      103        
#define		TRX_AUT_REQ_FROM_GAB   	  104
#define		TRX_ANNUL_REQ_FROM_GAB    105
#define		TRX_ADVICE_REQ_FROM_GAB   106


#define	TRX_TO_NW_CODE_BASE			500
#define	TRX_AUT_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 1)
#define	TRX_BAL_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 2)
#define	TRX_STAT_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 3)
#define	TRX_CHEQ_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 4)
#define	TRX_TFR_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 5)
#define	TRX_REJ_REQ_TO_NW			(TRX_TO_NW_CODE_BASE + 6)



#define	MAX_LABEL_LEN				30

#define MAX_ORDER_NBR               12
#define MAX_STATUS_NBR              2

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

#endif /*** HPS_ISO_SNI_ENGINE_H ***/
