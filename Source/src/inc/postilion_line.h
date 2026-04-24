/*
** <Name>
**  postilion_line.h
** 
** <Descr>
**  general header file for "postilion" line management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       02Sep02   G.FAHMI        Initiale Version
**  1.0.0.1       14Oct02   M.A.WAHBI      Adaptation to POSTILION
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef POSTILION_LINE_H
#define POSTILION_LINE_H


/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
        #include <iso_postilion.h>
        #include <iso_postilion_f127.h>
        #include <iso_hps.h>
        #include <queue.h>
        #include <struct_line.h>
        #include <tlv_private.h>


   /* - Private Header Files */

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   void * thread_proc_LINE(void * arg);
   void * thread_wait_LINE(void * arg);
   void   LineProcMsg(int nIndexCtx , TSLineExchange *sLineMsg);

   /* - Public Functions Declaration */
   int ConnectInterface  (int   P_nIndiceCtx);
   int ReconnectInterface(int   P_nIndiceCtx);
   int  FCT_AUT_REQ_FROM_POSTILION(int , TSPostilionInfo  *, char*);

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

#endif /* POSTILION_LINE_H */

