/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      execute_task.h                                                         */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Include file for Principal Program For PowerCARD Batch Processing   */
/*         execute_task.c                                                      */
/*                                                                             */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   10/10/2001 - Modified V 2.0.1                             */
/*        Ajustement des constantes pour etre conforme au PL/SQL               */
/*                                                                             */
/*******************************************************************************/

#ifndef HPS_EXECUTE_TASK_H
#define HPS_EXECUTE_TASK_H
/*
#define ERROR_WAIT   9 
#define ERROR_FILE  11 
#define ERROR_EXEC 127 
*/
#define ERROR_WAIT   128 
#define ERROR_FILE   126 
#define ERROR_EXEC   127 
/*
#define BIN_DIRECTORY 	"/pcard5/referentiel/usr/bin"
#define TRACE_DIRECTORY "/pcard5/referentiel/trace"
#define TRACE_FILE      "EXEC_TASKS.TRC000"
#define MAX_SIZE 2048 
*/

#endif  /** HPS_EXECUTE_TASK_H  **/
