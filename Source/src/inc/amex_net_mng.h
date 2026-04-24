/*
** <Name>
**  amex_net_mng.h
** 
** <Descr>
**  general header file for "amex" network management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20030214   M.A.WAHBI      Adaptation to AMEX
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef AMEX_NET_MNG_H
#define AMEX_NET_MNG_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <iso_hps.h>
        #include <iso_amex.h>
        #include <iso_amex_f127.h>
        #include <iso_hps.h>
        #include <queue.h>
        #include <struct_line.h>
        #include <tlv_private.h>


/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   int  FCT_NW_MNG_REQ_TO_NW (int P_nIndexCtx, TSIsoInfo *P_psIsoInfo);
   int  FCT_NW_MNG_REP_FROM_NW (int P_nIndexCtx);
   int  FCT_NW_MNG_REQ_FROM_NW (int , TSAmexInfo *);
   int  initial_sign_on_to_nw (int nIndexCtx );
   int  SendNwMngMsg (int nIndexCtx, char *InfoCode );
   int  sign_off (int nIndexCtx );
   int  sign_on (int nIndexCtx );

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

#endif /* AMEX_NET_MNG_H */

