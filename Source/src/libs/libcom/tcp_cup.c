/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_host.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      TCP HOST Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <tcp_var.h>
#include <atmopcode.h>

#include <string.h>
#include <cup_define.h> 
#define LG_CUP_HDR 4

void  GetFdConnect(char *sFdStr)
{
   sprintf(sFdStr,"%.6d", nFdConnect);
}

void  SetFdConnect(char *sFdStr)
{
   nFdConnect = atoi(sFdStr);
}
int ReadLineMsg(char *sBuffer, int *nLength)
{
   char  sBuffRcv[ CUP_LG_MAX ];
   char  sLine[ MAX_LINE_TRC ];
   char  sLenMsg[ 20 ];
   int   nLenMsg, nRead;
   int   i;
   int   nFd = nFdConnect;

   memset(sBuffRcv, 0, sizeof(sBuffRcv));
   memset(sLine, 0, sizeof(sLine));
   memset(sLenMsg, 0, sizeof(sLenMsg));
   
   sprintf(sLine, "Start ReadLineMsg(%d)", nFd);
   trace_event(sLine, PROCESSING);


   /*** Lecture de La Longueuer du message ****/
   do
   {
       memset(sBuffRcv, 0, sizeof(sBuffRcv));   
       nRead = recv(nFd, sBuffRcv, LG_CUP_HDR, MSG_PEEK);

       sprintf(sLine, "Nombre d'octets lus = %d, %s", nRead, sBuffRcv);
       trace_event(sLine, ERROR);

       if(!memcmp(sBuffRcv,"0000",4) && nRead == 4)  
		nRead = recv(nFd, sBuffRcv, 4, 0);

   } while (((nRead !=LG_CUP_HDR) && (nRead > 0)) || (!memcmp(sBuffRcv,"0000",4)));

   if (nRead < 0)
   {
      sprintf(sLine, "Error Reading Msg Length : %d nRead = %d", errno, nRead);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadLineMsg(NOK)");
      trace_event(sLine, PROCESSING);
      return (NOK);
   }
   if (nRead == 0)
   {
      sprintf(sLine, "Disconnect : %d", errno);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadLineMsg(DISCONNECT)");
      trace_event(sLine, PROCESSING);
      return (OR_DISCONECT);
   }

   sprintf(sLenMsg,"%.4s", sBuffRcv);
  /* La longueur dans le header du message n'inclu pas la longueur de la longueur */
   nLenMsg = atoi(sLenMsg);

   sprintf(sLine, "BUFFER LENGTH : [%d]", nLenMsg);
   trace_event(sLine, TRACE);

   memset(sBuffRcv, 0, sizeof(sBuffRcv));
   nRead = recv(nFd, sBuffRcv, nLenMsg+4, 0);
   if (nRead < 0)
   {
      sprintf(sLine, "Error Reading Msg : %d", errno);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadLineMsg(NOK)");
      trace_event(sLine, PROCESSING);
      return (NOK);
   }
   if (nRead == 0)
   {
      sprintf(sLine, "Disconnect : %d", errno);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadLineMsg(DISCONNECT)");
      trace_event(sLine, PROCESSING);
      return (OR_DISCONECT);
   }

   dump_buffer ( sBuffRcv, nLenMsg, 'A', 'L', 'I' );

   memcpy( sBuffer, sBuffRcv, nLenMsg+ 4 );
   *nLength = nLenMsg;

   sprintf(sLine, "End   ReadLineMsg(%d)", *nLength);
   trace_event(sLine, PROCESSING);

   return (OK);
}

int WriteLineMsg(char *sBuffer, int nLength)
{
   char  sBuffSnd[ CUP_LG_MAX ];
   char  sLine   [ MAX_LINE_TRC ];
   int retour;
   int i;
   int nFdC = nFdConnect;

	memset(sBuffSnd, 0, sizeof(sBuffSnd)); 
	memset(sLine, 0, sizeof(sLine)); 

   sprintf(sLine, "Start WriteLineMsg(%d, %d)", nFdC, nLength);
   trace_event(sLine, PROCESSING);

  

   memcpy (sBuffSnd , sBuffer, nLength);

   dump_buffer ( sBuffSnd, nLength , 'A', 'L', 'O' );

   retour = send ( nFdC, sBuffSnd, nLength , 0);
   if (retour <= 0)
   {
      sprintf(sLine, "Err When Calling send() : %d", errno);
      trace_event(sLine, PROCESSING);
      sprintf(sLine, "End   WriteLineMsg(NOK)");
      trace_event(sLine, PROCESSING);
      return(NOK);
   }

   sprintf(sLine, "End   WriteLineMsg(OK)");
   trace_event(sLine, PROCESSING);
   return(OK);
}

int  CloseCldCom()
{
return(OK);
}

int WriteLineConn( int nFdConn, char *sBuffer, int nLength)
{
   char  sBuffSnd[ CUP_LG_MAX ];
   char  sLine   [ 256 ];
   int retour;
   int i;

	memset(sLine, 0, sizeof(sLine)); 
	memset(sBuffSnd, 0, sizeof(sBuffSnd)); 

	
   sprintf(sLine, "Start WriteLineMsg(%d, %d)", nFdConn, nLength);
   trace_event(sLine, PROCESSING);


   memcpy (sBuffSnd , sBuffer, nLength);

   dump_buffer ( sBuffSnd, nLength , 'A', 'L', 'O' );

   retour = send ( nFdConn, sBuffSnd, nLength , 0);
   if (retour <= 0)
   {
      sprintf(sLine, "Err When Calling send() : %d", errno);
      trace_event(sLine, PROCESSING);
      sprintf(sLine, "End   WriteLineMsg(NOK)");
      trace_event(sLine, PROCESSING);
      return(NOK);
   }

   sprintf(sLine, "End   WriteLineMsg(OK)");
   trace_event(sLine, PROCESSING);
   return(OK);
}

