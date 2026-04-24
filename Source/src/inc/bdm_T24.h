/*
** <Name>
**  bdm_T24.h
** 
** <Descr>
**  general header file for "T24" interface
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef T24_H
#define T24_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <T24_fields.h>
   #include <resources.h>
   #include <dump.h>
   #include <comms_param.h>
   #include <purge_time.h>
   #include <troubles_param.h>
   #include <define.h>
   #include <event_log.h>
   #include <thread_var.h>
   #include <main_thr.h>


/* - Constantes Definition Section ----------------------------------------- */
#define BANK_RSC_ID       "000000"

/* - Data Type definition Section ------------------------------------------ */
typedef struct TSBank {
   char F_szRsrcId[6 + 1];
   char F_szResourceStatus[3 +1];
} SBank;


/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   int ThreadMaskSignals(void);
   void sig_segment_violation(int sig);
   void sig_alarm(int sig);
   void GetGlobals (TSResources * P_sResource);
   int  InitModule(char * P_szResourceId);
   void DumpGlobals (void);
   int GetMaxSafCounter(void);
   int GetAdviceInterval ( char *ResourceId );
   char GetDeclineAdvicesFlag ( char * Resource );

	extern SBank G_sBank;
	extern int nAuthCalculMethod;

#define AUTH_MTH_STD		0
#define AUTH_MTH_MAA		1


#endif /* T24_H */

