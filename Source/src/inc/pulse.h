/*
** <Name>
**  mds.h
** 
** <Descr>
**  general header file for "mds" interface
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef PULSE_H
#define PULSE_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
   #include <tcp_var.h>
   #include <comms_param.h>
   #include <dump.h>
   #include <event_log.h>

   /* - Private Header Files */
   #include <resources.h>
	#include <p7_sid.h>
   #include <pulse/pulse_define.h>
   #include <pulse/pulse_fields.h>
   #include <pulse/pulse_fieldstlv.h>
   #include <pulse/pulse_ictag.h>
   #include <pulse/pulse_param.h>
   #include <pulse/pulse_privfields.h>
   #include <pulse/iso_pulse.h>
   #include <pulse/pulse_pdstag.h>
   #include <pulse/pulse_tlv.h>

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


#endif /* PULSE_H */
