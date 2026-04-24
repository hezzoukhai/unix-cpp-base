/*
** <Name>
**  amex.h
** 
** <Descr>
**  general header file for "amex" interface
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20030214   M.A.WAHBI      Adaptation of AMEX Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef AMEX_H
#define AMEX_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <resources.h>
   #include <dump.h>
   #include <comms_param.h>
   #include <purge_time.h>
   #include <troubles_param.h>
   #include <define.h>
   #include <event_log.h>
   #include <thread_var.h>
   #include <main_thr.h>

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   int  ThreadMaskSignals(void);
   void sig_segv(int sig);
   void sig_alarm(int sig);
   void GetGlobals (TSResources * P_sResource);
   int  InitModule(char * P_szResourceId);
   void DumpGlobals (void);
   int  GetMaxSafCounter(void);
   char GetDeclineAdvicesFlag ( char * Resource );

#endif /* AMEX_H */

