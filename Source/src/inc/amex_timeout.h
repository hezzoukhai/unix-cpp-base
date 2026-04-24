/*
** <Name>
**  amex_timeout.h
** 
** <Descr>
**  general header file for "amex" timeouts management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20030214   M.A.WAHBI      Adaptation for AMEX
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef AMEX_TIMEOUT_H
#define AMEX_TIMEOUT_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <iso_hps.h>
   #include <struct_time.h>
        #include <iso_amex.h>
        #include <iso_amex_f127.h>
        #include <iso_hps.h>
        #include <queue.h>
	#include <struct_line.h>
        #include <tlv_private.h>


/* - Constantes Definition Section ----------------------------------------- */
#define    MAX_ECHO_TEST            3

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void * thread_proc_TIME(void * arg);
   void * thread_TIMEOUT(void * arg);
   int AcquirerTimeProcMsg(int nIndiceCtx, TSTimeExchange * sTimeMsg);
   int IssuerTimeProcMsg(int P_nIndexCtx, TSTimeExchange * sTimeMsg);
   int TimeProcMsg(int P_nIndexCtx, TSTimeExchange *sTimeMsg);
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

#endif /* AMEX_TIMEOUT_H */

