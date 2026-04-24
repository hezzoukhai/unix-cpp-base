/*
** <Name>
**  mds_bal.h
** 
** <Descr>
**  general header file for "mds" IPC module
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef PULSE_BAL_H
#define PULSE_BAL_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <queue.h>

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void * thread_proc_BAL(void * arg);
   void * thread_wait_BAL(void * arg);
   void   BalProcMsg(int nIndexCtx, TSBalExchange *sBalMsg);


#endif /* PULSE_BAL_H */

