/*
** <Name>
**  amex_bal.h
** 
** <Descr>
**  general header file for "amex" IPC module
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.1       20030214   M.A.WAHBI      Adaptation to AMEX
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef AMEX_BAL_H
#define AMEX_BAL_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <queue.h>
        #include <iso_amex.h>
        #include <iso_amex_f127.h>
        #include <iso_hps.h>
        #include <queue.h>
        #include <struct_line.h>
        #include <tlv_private.h>


/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void * thread_proc_BAL(void * arg);
   void * thread_wait_BAL(void * arg);
   void   BalProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);

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

#endif /* AMEX_BAL_H */

