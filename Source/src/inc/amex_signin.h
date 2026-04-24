/*
** <Name>
**  amex_signin.h
**
** <Descr>
**  general header file for "amex" SIGNIN management
**
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20030214   M.A.WAHBI      Adaptation to AMEX
**
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef AMEX_SIGNIN_H
#define AMEX_SIGNIN_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */
#include <amex_net_mng.h>

   /* - Private Header Files */

/**
        #include <iso_amex.h>
        #include <iso_amex_f127.h>
**/
        #include <iso_hps.h>
        #include <queue.h>
        #include <struct_line.h>
        #include <tlv_private.h>

/* - Constantes Definition Section ----------------------------------------- */

/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */
   void * thread_signin(void * arg);
   int GenerateEchoTest( int P_nIndiceCtx );
   int PrepareNetMsg (int P_nIndexCtx, char *P_szFctCode, TSIsoInfo *P_psIsoInfo);
   
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

#endif /* AMEX_SIGNIN_H */


