/*
** <Name>
**  mds_saf.h
** 
** <Descr>
**  general header file for "mds" SAF management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef PULSE_SAF_H
#define PULSE_SAF_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void * thread_SAF(void * arg);
   int SafManagement( int nIndiceCtx , char *sResource );
   int SafResponseReceived ( int       nIndiceCtx ,
                             char      *sMti ,
                             char      *sStan ,
                             char      *sSourceResource,
                             char      *sResource,
                             char      *sSlfMti ,
                             char      *sSlfResponseCode );
   int SafTimeOutReceived (  int       nIndiceCtx ,
                             char      *sMti ,
                             char      *sStan ,
                             char      *sSourceResource,
                             char      *sResource,
                             char      *sSlfMti ,
                             char      *sSlfResponseCode,
                             int       nSendCounter );
   int SafResponseTraitement(int nIndiceCtx ,
                             char *sTlvBuffer ,                                
                             char *sSlfMti,
                             char *sSlfResponseCode ,
                             char *sInternalRespCode );
   int SafTimeOutTraitement( int nIndiceCtx ,  char *sTlvBuffer  );


#endif /* PULSE_SAF_H */

