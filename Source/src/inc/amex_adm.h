/*
** <Name>
**  amex_adm.h
** 
** <Descr>
**  general header file for "amex" administration module
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20030214   M.A.WAHBI      Adaptation for AMEX
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef AMEX_ADM_H
#define AMEX_ADM_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <queue.h>
   #include <iso_hps.h>
        #include <iso_amex.h>
        #include <iso_amex_f127.h>
        #include <queue.h>
        #include <struct_line.h>
        #include <tlv_private.h>


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
   void  FCT_LINE_DOWN (int nIndexCtx, TSIsoInfo *IsoInfo);
   void  FCT_LINE_UP (int nIndexCtx, TSIsoInfo *IsoInfo);
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

#endif /* AMEX_ADM_H */

