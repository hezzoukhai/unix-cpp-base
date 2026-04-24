  /***********************************************************************
  **  nom	: iso_sni_globls.h						                       **
  **  desc	: vaiables globales du serveur ISO_ISN			           **
  **  auteur	: YK  						                           **
  **			(C) 2003-2004 : Hightech Payment Systems	           **
  ***********************************************************************/
#ifndef HPS_ISO_SNI_GLOBLS_H
#define HPS_ISO_SNI_GLOBLS_H
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

typedef struct admin_processing_globals_t
{
	char                              current_version [5];
	char                              record_type     [2];
	char                              stop_list_code  [4];
	int                               flag_send_files;
	int                               flag_send_keys;
	int                               last_block;    

} admin_processing_globals;

typedef struct iso_net_processing_globals_t
{
    char    	par_record_data[LG_MAX];
	int 	    par_record_lg;
	int		    par_offset ;
	int		    desc_file;
	int		    key_trans_flag;
	int		    compteur;

} iso_net_processing_globals;

typedef struct main_iso_sni_globals_t
{
   char             update_code[2];     /*specifique iso*/
   char   key_transfert_type [ 3];      /*à initialiser*/
   int              cpt_message;        /*specifique iso à initialiser par 0*/
   int				type_trait;         /*specifique iso*/

} main_iso_sni_globals;

SContext	gContext;

SSystem		gSystem;

STrnRef     gTrnRef;

#endif /*** HPS_ISO_SNI_GLOBLS_H ***/
