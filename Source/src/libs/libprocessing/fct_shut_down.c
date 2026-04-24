/*
+------------------------------------------------------------------------------+
|                                  PROCESO DE DATOS                            |
|                     Copyright (C) 2002 - HIGTECH PAYMENT SYSTEMS             |
+-----------------+------------------------------------------------------------+
| F. Mudule       | fct_shut_down.c                                            |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     | Control of the Server                                      |
+-----------------+------------------------------------------------------------+
| Programme        | posiso                                                    |
+-----------------+---------------------------------+---------+----------------+
| Aplication      | Powercard                       | Module  | Front End      |
+-----------------+-----------------------------+---+---------+-----+----------+
| Created by      |  El-Hadj OUADIE             | Date de Creation  | 01/10/02 |
+-----------------+-----------------------------+-------------------+----------+
| Lst. mod.       |  El-Hadj OUADIE              01/10/02 15:28:31             |
+-----------------+------------------------------------------------------------+
| Public          |                                                            |
| Objects         |                                                            |
| of the source   |                                                            |
|                 |                                                            |
+-----------------+------------------------------------------------------------+
| Dependances     |                                                            |
+-----------------+------------------------------------------------------------+
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <dump.h>

#include <iso_hps.h>
#include <define.h>
#include <struct_line.h>
#include <param_arg.h>
#include <thread_var.h>
#include <queue.h>
#include <event_log.h>
#include <resources.h>

/*
+-----------------+------------------------------------------------------------+
| FUNCTION        | int StopProcThread( int nIndiceCtx)                        |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     | Stop the processing thread                                |
+-----------------+------------------------------------------------------------+
| ARGUMENTS       | int nIndiceCtxthe context index                            |
+-----------------+------------------------------------------------------------+
| RETURN          | int                                                        |
+-----------------+------------+------------+----------------------------------+
| HISTORIQUE      | AUTHOR     | DATE 	    | DESCRIPTION                      |
+-----------------+------------+------------+----------------------------------+
|                 | E.  OUADIE |            |  Creation           01/10/02     |
+-----------------+------------+------------+----------------------------------+
*/
int StopProcThread( int nIndiceCtx)
{
   char  sLine [MAX_LINE_TRC];

   sprintf(sLine, "Shuting down Thread = %d", pthread_self());
   trace_event(sLine, FATAL);
   DeconnectOracle(nIndiceCtx);
   FreeOracleCtx(nIndiceCtx);
   pthread_exit(NULL);
}


/*
+-----------------+------------------------------------------------------------+
| FUNCTION        | int shutdown_server ( int nIndexCtx , char *ResourceId  )  |
+-----------------+------------------------------------------------------------+
| DESCRIPTION     | Shut down the server                                       |
+-----------------+------------------------------------------------------------+
| ARGUMENTS       |  int nIndexCtx                                             |
|                 | char *ResourceId                                           |
+-----------------+------------------------------------------------------------+
| RETURN          | int shutdown_server                                        |
+-----------------+------------+------------+----------------------------------+
| HISTORIQUE      | AUTHOR     | DATE 	    | DESCRIPTION                      |
+-----------------+------------+------------+----------------------------------+
|                 | E.  OUADIE |            |  Creation           01/10/02     |
+-----------------+------------+------------+----------------------------------+
*/
/* HKETTANI 31052005
int shutdown_server ( int nIndexCtx , char *ResourceId  )
{
   TSBalExchange   BalMessage;
   int             i;

   trace_event("Shutdown Server...In Progress", FATAL);
   ShutDownProcessingThreads ();

   DeconnectOracle(nIndexCtx);
   FreeOracleCtx(nIndexCtx);
   trace_event("About to Exit The Program       ", FATAL);
   sleep(9);

   exit (0);
}
*/
int shutdown_server ( int nIndexCtx , char *ResourceId  )
{
   TSBalExchange   BalMessage;
   int             i;
   char           app_pid  [9];
   char           app_ppid [9];
   int nFdConn;

   trace_event("Shutdown Server...In Progress", FATAL);
   ShutDownProcessingThreads ();

   sprintf(app_pid,  "%08d", getpid());
   sprintf(app_ppid, "%08d", getppid());

   if ( UpdateResource ( nIndexCtx , ResourceId, NOTLIVE, OFF ) != OK )
     trace_event ( "Cannot update resource status at ShutDown ", PROCESSING );


   if ( ResourceActivity ( nIndexCtx , ResourceId, STOP ) != OK )
     trace_event ( "Cannot update resource activity at ShutDown ", PROCESSING );


   EmptyQueue(nBalMsg, getpid());
   EmptyQueue(nBalAdm, getpid());

   /*StopCltCom();*/
   /*YK240505*/
   /*
   nFdConn = GetFdByOraCtx(nIndexCtx);
   CloseCldCom(nFdConn);
   */

   LogEvent (nIndexCtx, ResourceId, "##", INTERFACE_STOP, "##",
             "##", "##", app_pid, app_ppid);

   

   /*YK230605*/
   /*
   DeconnectOracle(nIndexCtx);
   FreeOracleCtx(nIndexCtx);
   */

   trace_event("About to Exit The Program       ", FATAL);

   /*sleep(3);*//*YK240505*/

   exit (0);
