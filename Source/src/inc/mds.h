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

#ifndef MDS_H
#define MDS_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
   #include <tcp_var.h>
   #include <comms_param.h>
   #include <dump.h>
   #include <event_log.h>


   /* - Private Header Files */
   #include <resources.h>

/* - Constantes Definition Section ----------------------------------------- */
#define MDS_RSC_ID       "000088"

/* - Data Type definition Section ------------------------------------------ */
typedef struct TSMds {
   char F_szRsrcId[6 + 1];
   char F_szResourceStatus[3 + 1];
} SMds;


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

   /* - Private Functions Declaration */
   #ifdef _FOR_INT_USE_
   #endif /* _FOR_INT_USE_ */

/* - Varuiables Definition Section ----------------------------------------- */
   /* - Module Variables Declaration --------------------------------------- */
   #ifdef _FOR_INT_USE_
      SMds    G_sMds = {"000000", "OFF"};
      char         AcquirerBank [ 6 + 1 ];
      char         ResourceInternalId [32 +1];
	  char       sInterfaceMode [ 1 + 1];
   #endif /* _FOR_INT_USE_ */

   /* - Module Variables Publication --------------------------------------- */
   #ifdef _FOR_EXT_USE_
      extern SMds G_sMds;
      extern char AcquirerBank [ 6 + 1 ];
      extern char ResourceInternalId [32 +1];
	  char       sInterfaceMode [ 1 + 1];
   #endif /* _FOR_EXT_USE_ */

#endif /* MDS_H */
