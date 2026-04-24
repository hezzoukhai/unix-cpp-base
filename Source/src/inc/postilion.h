/*
** <Name>
**  postilion.h
** 
** <Descr>
**  general header file for "postilion" interface
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       02Sep02   G.FAHMI        Initiale Version
**  1.0.0.1       14Oct02   M.A.WAHBI      Adaptation of POSTILION Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef POSTILION_H
#define POSTILION_H

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
   #include <list_thr.h>
   #include <security_data.h>
   #include <queue.h>
   #include <hsm_inc.h>
   #include <tlv_private.h>
   #include <tlv_fields.h>


/* - Constantes Definition Section ----------------------------------------- */
#define POSTILION_RSC_ID      "PSTLN0"
#define EI_APPROVED_REQ       "00"
/*JGH Start 23052010*/
#define PIN_ERROR       '0'
#define DECLINED_AUTH   '1'
#define APPROVED_AUTH   '2'
#define PAGESIZE              4096
/*End JGH*/

/* - Data Type definition Section ------------------------------------------ */
typedef struct TSPostilion {
   char *F_szRsrcId;
   char *F_szResourceStatus;
   char *F_szPostAcqBank;
   int  F_nSendNbr;
   int  F_nEchoTestPeriod;
} SPostilion;
/* Mac Management Flag*/
char  MacManagementFlag[ 1 +1];
/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   int  ThreadMaskSignals(void);
   void sig_segment_violation(int sig);
   void sig_alarm(int sig);
   void GetGlobals (TSResources * P_sResource);
   int  InitModule(char * P_szResourceId);
   void DumpGlobals (void);
   int  GetMaxSafCounter(void);
   int  GetAdviceInterval ( char *ResourceId );
   char GetDeclineAdvicesFlag ( char * Resource );
//MAB 04/11/2020 add postilion keys management 
int GenPostilionZPK ( int    nIndexCtx  ,
                      char  *pResourceId,
                      char  *pZpkZmk    ,
                      char  *pKCV
                    );
//MAB 04/11/2020 add postilion keys management 

   /* Resource Common Parameters ***/
   char  ResourceId         [ 6 +1];
   char  ResourceName       [15 +1];
   char  ResourceStatus     [ 3 +1];
   char  ResourceStatusSAF  [ 3 +1];
   char  ResourceInternalId [32 +1];

   /* - Private Functions Declaration */

/* - Variables Definition Section ----------------------------------------- */
   /* - Module Variables Declaration --------------------------------------- */
   #ifdef _FOR_INT_USE_
      SPostilion    G_sPostilion = {"000000", "OFF", "000000", 0, 30};
      char         AcquirerBank [ 6 + 1 ];
      char 	   AcquirerInstId [11 +1];
      char         ResourceInternalId [32 +1];
   #endif /* _FOR_INT_USE_ */

   /* - Module Variables Publication --------------------------------------- */
   #ifdef _FOR_EXT_USE_
      extern SPostilion G_sPostilion;
      extern char AcquirerBank [ 6 + 1 ];
      extern char ResourceInternalId [32 +1];
      extern char AcquirerInstId [11 +1];
      /*extern int  nFdConnect;*/
      extern int  nFdLocal;
   #endif /* _FOR_EXT_USE_ */

typedef struct TPostilionCTX
{
   char             szBufferLine        [LG_MAX];
   char             szBufferBal         [LG_MAX];
   char             gTlvBuffer          [LG_MAX];
   int              free;
   int              status;
   int              fd;
   int              flag_bal;
   int              flag_command;
   int              nLenLine;
   int              nLenBal;
   pthread_t        tid;
   pthread_mutex_t  PostilionMutex;
   pthread_cond_t   PostilionCond ;
} TSPostilionCtx;

TSPostilionCtx  tab_PostilionCtx [ MAX_THREAD ];
#define ACQR_COUNTRY_CODE     "480"

#endif /* POSTILION_H */

