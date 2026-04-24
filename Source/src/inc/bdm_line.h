/*
** <Name>
**  bdm_line.h
** 
** <Descr>
**  general header file for "bdm" line management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef BDM_LINE_H
#define BDM_LINE_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
        #include <iso_bdm.h>
	#include <iso_hps.h>
	#include <queue.h>
	#include <struct_line.h>
	#include <tlv_private.h>

   /* - Private Header Files */

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void * thread_proc_LINE(void * arg);
   void * thread_wait_LINE(void * arg);
   void   LineProcMsg(int nIndexCtx, TSLineExchange *sLineMsg);
   
   int  FCT_FTR_REP_FROM_BANK (int nIndexCtx, TSBankInfo *BankInfo);
   int  BuildFTReplyFromBank  (int nIndexCtx, TSBankInfo *BankInfo,TSIsoInfo   *IsoInfo);
   int  FCT_AUT_REP_FROM_BANK (int nIndexCtx, TSBankInfo *BankInfo);
   int  BuildAUTReplyFromBank  (int nIndexCtx, TSBankInfo *BankInfo,TSIsoInfo   *IsoInfo);
   int  FCT_REV_REP_FROM_BANK (int nIndexCtx, TSBankInfo *BankInfo);
   

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

#endif /* BDM_LINE_H */

