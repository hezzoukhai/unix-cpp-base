/*
** <Name>
**  bank_net_mng.h
** 
** <Descr>
**  general header file for "bank" network management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef BANK_NET_MNG_H
#define BANK_NET_MNG_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   #include <iso_hps.h>

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   int  FCT_NW_MNG_REQ_TO_NW (int nIndexCtx, TSIsoInfo *IsoInfo);
   int  FCT_NW_MNG_REP_FROM_NW (int nIndexCtx);
   int  FCT_NW_MNG_REQ_FROM_NW (int nIndexCtx);
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

#endif /* BANK_NET_MNG_H */

