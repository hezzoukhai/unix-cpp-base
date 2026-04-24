/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      async_com.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      Asynchrone Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <fcntl.h>
#include <termio.h>
#include <errno.h>
#include <define.h>
#include <tcp_var.h>
#include <signal.h>

void arret_alarm()
{
   trace_event("Start arret_alarm()",  PROCESSING);
   return;
}

int InitCltHsm(char *sAddress, int nPort)
{
   char           sLine[ MAX_LINE_TRC ];
   int            nFdConn;
   int            test;
   struct termio  arg;


   sprintf(sLine, "Start InitCltHsm(%s)", sAddress);
   trace_event(sLine,  PROCESSING);

/*   install_handler(SIGALRM, arret_alarm);
   alarm(10);*/
   /* creation et attachement du socket en local                       */
   nFdConn = open(sAddress, O_RDWR);
  /* alarm(0);
   desinstall_handler(SIGALRM);*/
   if ( nFdConn < 0 )
   {
      trace_event("Can't Open Device(NOK)", PROCESSING);
      trace_event("End InitCltHsm(NOK)",  PROCESSING);
      return(NOK);
   }

   test = ioctl( nFdConn, TCGETA, &arg );

   if (test < 0)
   {
      trace_event("ioctl TCGETA(NOK)", PROCESSING);
      trace_event("End InitCltHsm(NOK)",  PROCESSING);
      return(NOK);
   }

   arg.c_cflag = B9600+CS8+CREAD+CLOCAL;
   arg.c_lflag     = 0;
   arg.c_cc[VTIME] = 0;
   arg.c_cc[VMIN]  = 1;

   test = ioctl( nFdConn, TCSETA, &arg );
   if (test < 0)
   {
      trace_event("ioctl  TCSETA(NOK)", PROCESSING);
      trace_event("End InitCltHsm(NOK)",  PROCESSING);
      return(NOK);
   }

   sprintf(sLine, "End InitCltHsm(OK Fd = %d)", nFdConn);
   trace_event(sLine,  PROCESSING);
   return (nFdConn);
}

