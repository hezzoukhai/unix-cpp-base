  /***********************************************************************
  **  nom	: ndcglobls.h						**
  **  desc	: vaiables globales du serveur NDC			**
  **  auteur	: YO990817						**
  **			(C) 1997-2001 : Hightech Payment Systems	**
  ***********************************************************************/
#ifndef HPS_NDC_GLOBLS_H
#define HPS_NDC_GLOBLS_H
#include <define.h>
#include <iso_hps.h>

#define	YES		1
#define	NO		0

typedef struct SSystemT {
    int			fTrnInPrgFlg;
    char		fCardHolderMsg	[256];
} SSystem;

typedef	struct SContextT {
    char		fTransLang;
    int			fRespCode;
    int			fTransCode;
    int			fNextTrans;
    int			fAuthoSrc;
    char		fCoordNbr	[  4];
    TSIsoInfo	fP7Buff;
} SContext;

typedef struct STrnRefT{
    char    fRefNbr     [13];
    char    fCardNbr    [23];
    char    fTrnLocalDate   [13];
    char    fTrnAmt     [13];
} STrnRef;



#define   CASHSEND_CANCLED_BY_IHM          'U'
#define   CASHSEND_CANCLED_PIN_EXCEEDED    'N'
#define   CASHSEND_STATUT_VALIDE           'V'
#define   CASHSEND_STATUT_CONFIRMED        'C'
#define   CASHSEND_STATUT_EXPIRED          'E'
#define   CASHSEND_STATUT_PROCCED          'P'
#define   CASHSEND_CANCLED_BY_SYS          'S'

#endif /*** HPS_NDC_GLOBLS_H ***/
