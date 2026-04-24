/*
** <Name>
**  bdm_timeout_T24.h
** 
** <Descr>
**  general header file for "T24" timeouts management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef BANK_T24_TIMEOUT_H
#define BANK_T24_TIMEOUT_H

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
   int IssuerTimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg);
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

#endif /* BANK_T24_TIMEOUT_H */

