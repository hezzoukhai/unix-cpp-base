/*
** <Name>
**  bdm_line_T24.h
** 
** <Descr>
**  general header file for "T24" line management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef BDM_T24_LINE_H
#define BDM_T24_LINE_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
        #include <iso_T24.h>
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
   
   int  FCT_FTR_REP_FROM_T24 (int nIndexCtx, TST24Info *T24Info);
   int  BuildFTReplyFromT24  (int nIndexCtx, TST24Info *T24Info,TSIsoInfo   *IsoInfo);
   int  FCT_AUT_REP_FROM_T24 (int nIndexCtx, TST24Info *T24Info);
   int  BuildAUTReplyFromT24  (int nIndexCtx, TST24Info *T24Info,TSIsoInfo   *IsoInfo);
   int  FCT_REV_REP_FROM_T24 (int nIndexCtx, TST24Info *T24Info);
   

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

#endif /* BDM_T24_LINE_H */

