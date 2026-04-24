/*
** <Name>
**  bdm_adm_T24.h
** 
** <Descr>
**  general header file for "T24" administration module
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef BANK_T24_ADM_H
#define BANK_T24_ADM_H

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
   void  FCT_LINE_DOWN (int nIndexCtx, TSIsoInfo *IsoInfo);
   void  FCT_LINE_UP (int nIndexCtx, TSIsoInfo *IsoInfo);
   int SendShutDownSignalsToLineThreads(void);
   int shutdown_resource(int nIndexCtx, char *ResourceId);


#endif /* BANK_T24_ADM_H */

