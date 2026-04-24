/*
** <Name>
**  mds_line.h
** 
** <Descr>
**  general header file for "pulse" line management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef PULSE_LINE_H
#define PULSE_LINE_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
   void * thread_proc_LINE(void * arg);
   void * thread_wait_LINE(void * arg);
   void   LineProcMsg(int nIndexCtx, TSLineExchange *sLineMsg);

   /* - Private Header Files */


#endif /* PULSE_LINE_H */

