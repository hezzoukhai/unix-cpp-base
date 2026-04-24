/*
** <Name>
**  amex_line.h
** 
** <Descr>
**  general header file for "amex" line management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20030214   M.A.WAHBI      Adaptation to AMEX
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef AMEX_LINE_H
#define AMEX_LINE_H


/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
        #include <iso_amex.h>
        #include <iso_amex_f127.h>
        #include <iso_hps.h>
        #include <queue.h>
        #include <struct_line.h>
        #include <tlv_private.h>


   /* - Private Header Files */

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   void * thread_proc_LINE(void * arg);
   void * thread_wait_LINE(void * arg);
   int    LineProcMsg(int nIndexCtx , TSLineExchange *sLineMsg);

   /* - Public Functions Declaration */
   int ConnectInterface  (int   P_nIndiceCtx);
   int ReconnectInterface(int   P_nIndiceCtx);
   int  FCT_AUT_REQ_FROM_AMEX(int , TSAmexInfo  *);

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

#endif /* AMEX_LINE_H */

