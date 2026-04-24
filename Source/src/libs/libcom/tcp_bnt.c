#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <line_com.h>
#include <tcp_var.h>

int ReadLineBnt( int nFdConn, char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   char  sLine[ MAX_LINE_TRC ];
   int   nLenMsg, nRead;
   int   i;

   /*** Lecture de La Longueuer du message ****/
   sprintf(sLine,"Start ReadLineHsm(%d)",nFdConn);
   trace_event(sLine,PROCESSING);

   nRead = recv(nFdConn, sBuffRcv, 2, MSG_PEEK);
   if (nRead <= 0)
   {
      fprintf(stderr,"Error Reading Msg Length : %d nRead = %d \n", errno, nRead );
      return (NOK);
   }

   nLenMsg = (sBuffRcv[0] * 256) + sBuffRcv[1] + 2;
   nRead = recv(nFdConn, sBuffRcv, nLenMsg, 0);
   if (nRead <= 0)
   {
      fprintf(stderr,"Error Reading Msg : %d \n", errno);
      return (NOK);
   }
   memcpy( sBuffer, sBuffRcv + 2 , nLenMsg - 2);
   *nLength = nLenMsg - 2;
   sprintf(sLine,"End   ReadLineHsm(OK)(%d)",*nLength);
   trace_event(sLine,PROCESSING);
   return (OK);
}

int WriteLineBnt( int nFdConn, char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   char  sLine[ MAX_LINE_TRC ];
   int retour;
   int i;

   sprintf(sLine,"Start WriteLineHsm(%d)",nFdConn);
   trace_event(sLine,PROCESSING);

   memset (sBuffSnd, 0, LG_MAX);
   sBuffSnd[0] = (nLength)/256;
   sBuffSnd[1] = (nLength)%256;
   memcpy (sBuffSnd +2 , sBuffer, nLength);
   retour = write ( nFdConn, sBuffSnd, nLength+2);
   if (retour <= 0)
   {
      	sprintf(sLine,"Error Sending Msg : %d ", errno);
   	trace_event(sLine,ERROR);
      return(NOK);
   }
   sprintf(sLine,"End   WriteLineHsm(%d)",retour);
   trace_event(sLine,PROCESSING);
   return(OK);
}

