/*
** <Name>
**  postilion_saf.h
** 
** <Descr>
**  general header file for "postilion" SAF management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       02Sep02   G.FAHMI        Initiale Version
**  1.0.0.1       14Oct02   M.A.WAHBI      Adaptation to POSTILION
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef POSTILION_SAF_H
#define POSTILION_SAF_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
        #include <iso_postilion.h>
        #include <iso_postilion_f127.h>
        #include <iso_hps.h>
        #include <queue.h>
        #include <struct_line.h>
        #include <tlv_private.h>


/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void * thread_SAF(void * arg);
   int SafManagement( int nIndiceCtx , char *sResource );
   int SafResponseReceived ( int       nIndiceCtx ,
                             char      *sPan ,
                             char      *sMti ,
                             char      *sStan ,
                             char      *sSourceResource,
                             char      *sResource,
                             char      *sSlfMti ,
                             char      *sSlfResponseCode );
    int SafHostResponseReceived ( int       nIndiceCtx ,
                             char      *sPan ,
                             char      *sMti ,
                             char      *sStan ,
                             char      *sSourceResource,
                             char      *sResource,
                             char      *sSlfMti ,
                             char      *sSlfResponseCode );
   int SafTimeOutReceived (  int       nIndiceCtx ,
                             char      *sPan ,
                             char      *sMti ,
                             char      *sStan ,
                             char      *sSourceResource,
                             char      *sResource,
                             char      *sSlfMti ,
                             char      *sSlfResponseCode,
                             int       nSendCounter );
   int SafHostTimeOutReceived (  int       nIndiceCtx ,
                             char      *sPan ,
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

#endif /* POSTILION_SAF_H */

