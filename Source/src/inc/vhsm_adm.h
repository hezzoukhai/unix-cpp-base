/*
** <Name>
**  vhsm_adm.h
** 
** <Descr>
**  general header file for "vhsm" administration module
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       11Mar02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef VHSM_ADM_H
#define VHSM_ADM_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <queue.h>
   #include <iso_hps.h>

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void *thread_ADMIN(void * arg);
   void  AdmProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);
   void  FCT_STOP_INTERFACE (int nIndexCtx, TSIsoInfo *IsoInfo);
   void  FCT_REFRESH_PARAMS (int nIndexCtx, TSIsoInfo *IsoInfo);
   void  FCT_REPORT_STATUS (int nIndexCtx, TSIsoInfo *IsoInfo);
   void  FCT_STATISTICS (int nIndexCtx, TSIsoInfo *IsoInfo);
   int SendShutDownSignalsToLineThreads(void);
   int shutdown_resource(int nIndexCtx, char *ResourceId);

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

#endif /* VHSM_ADM_H */

