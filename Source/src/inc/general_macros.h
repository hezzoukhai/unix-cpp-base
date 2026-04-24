/*
*** <Name>
**  trace_macros.h
**
** <Descr>
**  header file of common macros
**
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       04Jan04   Y. OUADI       Initiale Version
**
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef GENERAL_MACROS_H
#define GENERAL_MACROS_H

/* - Header File Inclusion Section ----------------------------------------- */
   /* - System Header Files */

   /* - Private Header Files */
   
   /* - External Declaration When It is missing from Private Header Files */

/* - Constantes Definition Section ----------------------------------------- */
   /* - Static Values */

   /* - Macros */
   #ifndef MIN
   #define MIN(a, b) ((a) < (b) ? (a) : (b))
   #endif
   #ifndef MAX
   #define MAX(a, b) ((a) > (b) ? (a) : (b)) /*macro defined method for MAX >*/
   #endif 
   #define TOUPPER(len, str) \
           {\
              int cntr;\
              char tmp[LG_MAX];\
              for (cntr = 0; cntr < MIN(len, LG_MAX); cntr++)\
                  tmp[cntr] = toupper((str)[cntr]);\
              memcpy((str), tmp, cntr);\
           }
   #define TOLOWER(len, str) \
           {\
              int cntr;\
              char tmp[LG_MAX];\
              for (cntr = 0; cntr < MIN(len, LG_MAX); cntr++)\
                  tmp[cntr] = tolower((str)[cntr]);\
              memcpy((str), tmp, cntr);\
           }

   #define LTRIM(lin, sin, lout, sout)\
           {\
              lout = 0;\
              while(lout < lin && sin[lout] == ' ') lout++;\
              memcpy(sout, sin + lout, lin - lout);\
              lout = lin - lout;\
           }
   #define RTRIM(lin, sin, lout, sout)\
           {\
              lout = 0;\
              while(lout < lin && sin[lin - lout - 1] == ' ') lout++;\
              memcpy(sout, sin, lin - lout);\
              lout = lin - lout;\
           }
   #define TRIM(lin, sin, lout, sout)\
           {\
              char stmp[LG_MAX];\
              int  ltmp;\
              LTRIM(lin, sin, ltmp, stmp);\
              RTRIM(ltmp, stmp, lout, sout);\
           }


/* - Data Type definition Section ------------------------------------------ */

/* - Function Declaration Section ------------------------------------------ */
   /* - Public Functions Declaration */

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

/*
#if defined(_AIX) || defined(__hpux)
#	define P7_INLINE	__inline__
#else
#	define P7_INLINE	inline
#endif
*/



/*
#if defined(_GCC)
#define P7_SYM_EXPORT			__attribute__((visibility("default")))
#define P7_SYM_IMPORT
#define P7_SYM_LOCAL			__attribute__ ((visibility ("hidden")))
#else
#define P7_SYM_EXPORT
#define P7_SYM_IMPORT
#define P7_SYM_LOCAL
#endif

#if defined(_AIX)
/ *#define P7_TLS			__thread* /
#define P7_TLS			
#elif defined(_GCC)
#define P7_TLS			__thread
#else
#define P7_TLS
#endif


/ * Macros to be used in futur to manage DB connections* /
#define ON_DB_CTX_START_USE(a)
#define ON_DB_CALL_START(a)
#define ON_DB_CALL_END(a)
#define ON_DB_CALL_EXCEPTION(a)
#define ON_DB_CTX_END_USE(a)

*/

#include <p7_macros.h>

#endif /* GENERAL_MACROS_H */


