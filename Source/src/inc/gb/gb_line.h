/*
** <Name>
**  gb_line.h
** 
** <Descr>
**  general header file for "gb" line management
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef GB_LINE_H
#define GB_LINE_H

/* - Header File Inclusion Section ----------------------------------------- */
/* - System Header Files */
void *thread_proc_LINE(void *arg);
void *thread_wait_LINE(void *arg);
void LineProcMsg(int nIndexCtx, TSLineExchange *sLineMsg);

/* - Private Header Files */

/* - Constantes Definition Section ----------------------------------------- */

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

#endif /* GB_LINE_H */
