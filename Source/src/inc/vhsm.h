/*
** <Name>
**  vhsm.h
** 
** <Descr>
**  general header file for "vhsm" interface
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       11Mar02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef VHSM_H
#define VHSM_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <resources.h>

/* - Constantes Definition Section ----------------------------------------- */
#define VHSM_RSC_ID       "000000"

/* - Data Type definition Section ------------------------------------------ */
typedef struct TSVHsm {
   char F_szRsrcId[6 + 1];
   char F_szResourceStatus[1 + 1];
} SVHsm;


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
      SVHsm    G_sVHsm = {"000000", "OFF"};
      char         AcquirerBank [ 6 + 1 ];
      char         ResourceInternalId [32 +1];
   #endif /* _FOR_INT_USE_ */

   /* - Module Variables Publication --------------------------------------- */
   #ifdef _FOR_EXT_USE_
      extern SVHsm G_sVHsm;
      extern char AcquirerBank [ 6 + 1 ];
      extern char ResourceInternalId [32 +1];
   #endif /* _FOR_EXT_USE_ */

#endif /* VHSM_H */

