/*
** <Name>
**  bank_bal.h
** 
** <Descr>
**  general header file for "bank" IPC module
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef BDM_BAL_H
#define BDM_BAL_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
	#include <iso_hps.h>
	#include <struct_line.h>
	#include <tlv_private.h>
        #include <list_thr.h>
   /* - Private Header Files */
   #include <queue.h>
   #include <iso_bank.h>

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void * thread_proc_BAL(void * arg);
   void * thread_wait_BAL(void * arg);
   void   BalProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);

   int  FCT_FTR_REQ_TO_BANK ( int nIndexCtx,TSIsoInfo *IsoInfo);
   int  BuildFTRReqToBank ( int nIndexCtx,TSIsoInfo *IsoInfo, TSBankInfo *BankInfo);
   int  FCT_AUT_REQ_TO_BANK ( int nIndexCtx,TSIsoInfo *IsoInfo);
   int  BuildAUTReqToBank ( int nIndexCtx,TSIsoInfo *IsoInfo, TSBankInfo *BankInfo);
   
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

#endif /* BDM_BAL_H */

