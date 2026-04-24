/*
** <Name>
**  bdm_bal_T24.h
** 
** <Descr>
**  general header file for "T24" IPC module
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef BDM_T24_BAL_H
#define BDM_T24_BAL_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
	#include <iso_hps.h>
	#include <struct_line.h>
	#include <tlv_private.h>
        #include <list_thr.h>
   /* - Private Header Files */
   #include <queue.h>
   #include <iso_T24.h>

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void * thread_proc_BAL(void * arg);
   void * thread_wait_BAL(void * arg);
   void   BalProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);

   int  FCT_FTR_REQ_TO_T24 ( int nIndexCtx,TSIsoInfo *IsoInfo);
   int  BuildFTRReqToT24 ( int nIndexCtx,TSIsoInfo *IsoInfo, TST24Info *T24Info);
   int  FCT_AUT_REQ_TO_T24 ( int nIndexCtx,TSIsoInfo *IsoInfo);
   int  BuildAUTReqToT24 ( int nIndexCtx,TSIsoInfo *IsoInfo, TST24Info *T24Info);
   
   /* - Private Functions Declaration */
   #ifdef _FOR_INT_USE_
   #endif /* _FOR_INT_USE_ */

/* - Varuiables Definition Section ----------------------------------------- */
   /* - Module Variables Declaration --------------------------------------- */
   #ifdef _FOR_INT_USE_                                                        
   #endif /* _FOR_INT_USE_ */                                                  
                                                                               
   /* - Module Variables Publication --------------------------------------- */
   #ifdef _FOR_EXT_USE_                                                        
   #endif /* _FOR_INT_USE_ */                                                  

#endif /* BDM_T24_BAL_H */

