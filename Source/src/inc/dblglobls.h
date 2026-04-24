  /***********************************************************************
  **  nom	: dbl_globls.h						**
  **  desc	: vaiables globales du serveur DIE BOLD			**
  **  auteur	: Adil HMAMI 						**
  **			(C) 1997-2001 : Hightech Payment Systems	**
  ***********************************************************************/
#ifndef HPS_DBL_GLOBLS_H
#define HPS_DBL_GLOBLS_H
#include <define.h>
#include <iso_hps.h>

#define	YES		1
#define	NO		0

typedef struct SSystemT {
    int			fTrnInPrgFlg;
    char		fCardHolderMsg	[256];
    char		fCrdAccId	[ 16];
} SSystem;

typedef	struct SContextT {
    char		fTransLang;
    int			fRespCode;
    int			fTransCode;
    int			fNextTrans;
    int			fAuthoSrc;
    char		fCoordNbr	[  4];
    TSIsoInfo	fP7Buff;
    TSIsoInfo	fP7FeesBuff;
} SContext;

typedef struct STrnRefT{
    char    fRefNbr     [13];
    char    fCardNbr    [23];
    char    fTrnLocalDate   [13];
    char    fTrnAmt     [13];
} STrnRef;

SContext	gContext;

SSystem		gSystem;

STrnRef     gTrnRef;

#endif /*** HPS_DBL_GLOBLS_H ***/
