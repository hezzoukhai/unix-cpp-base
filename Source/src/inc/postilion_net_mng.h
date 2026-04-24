/*
** <Name>
**  postilion_net_mng.h
** 
** <Descr>
**  general header file for "postilion" network management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       02Sep02   G.FAHMI        Initiale Version
**  1.0.0.1       14Oct02   M.A.WAHBI      Adaptation to POSTILION
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef POSTILION_NET_MNG_H
#define POSTILION_NET_MNG_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <iso_hps.h>
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
   int  FCT_NW_MNG_REQ_TO_NW (int P_nIndexCtx, TSIsoInfo *P_psIsoInfo);
   /*int  FCT_NW_MNG_REP_FROM_NW (int P_nIndexCtx);*/
   int  FCT_NW_MNG_REQ_FROM_NW (int , TSPostilionInfo *);
   /*int  initial_sign_on_to_nw (int nIndexCtx );*/
   int  SendNwMngMsg (int nIndexCtx, char *InfoCode );
   int  sign_off (int nIndexCtx );
   int  sign_on (int nIndexCtx );
    /* Resource Common Parameters ***/
      char  ResourceId         [ 6 +1];
      char  ResourceStatus     [ 3 +1];
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

#endif /* POSTILION_NET_MNG_H */

