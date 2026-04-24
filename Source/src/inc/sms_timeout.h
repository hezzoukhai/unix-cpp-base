/*
** <Name>
**  sms_timeout.h
** 
** <Descr>
**  general header file for "sms" timeouts management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       040303    G.FAHMI        Initiale Version
** 
** (C) 2003 : Hightech Payment Systems
*/

#ifndef SMS_TIMEOUT_H
#define SMS_TIMEOUT_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <iso_hps.h>
   #include <struct_time.h>

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void * thread_proc_TIME(void * arg);
   void * thread_TIMEOUT(void * arg);
   int AcquirerTimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg);
   int IssuerTimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg);
   int OutgoingTimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg);
   int TimeProcMsg(int nIndiceCtx, TSTimeExchange *sTimeMsg);
   int BuildTimeOutReversalMsg (TSIsoInfo *IsoInfo ,TSIsoInfo * IsoRevInfo);

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

#endif /* SMS_TIMEOUT_H */

