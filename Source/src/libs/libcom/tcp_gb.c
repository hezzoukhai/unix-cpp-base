/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_gb.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      TCP GLOBAL BLUE Tools Management                                        */
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

void GetFdConnect(char *sFdStr)
{
   sprintf(sFdStr, "%.6d", nFdConnect);
}

void SetFdConnect(char *sFdStr)
{
   nFdConnect = atoi(sFdStr);
}
int ReadLineMsg(char *sBuffer, int *nLength)
{
   char sBuffRcv[LG_MAX];
   int nLenMsg, nRead;
   int i;
   int nFd = nFdConnect;
	char sLine[MAX_LINE_TRC];

	sprintf(sLine, "Start ReadLineMsg( nFd=[%d] )", nFd);
	trace_event(sLine, PROCESSING);

   do
   {
		nRead = recv(nFd, sBuffRcv, 2, MSG_PEEK);
   } while ((nRead != 2) && (nRead > 0));

   if (nRead <= 0)
   {
      fprintf(stderr, "Error Reading Msg Length : %d nRead = %d \n", errno, nRead);
      sprintf(sLine, "END   ReadLineMsg( NOK. 1. ERROR recv errno=[%d] )", errno);
      trace_event(sLine, FATAL);
      return (NOK);
   }

   nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + 2;


	nRead = recv(nFd, sBuffRcv, nLenMsg, MSG_WAITALL);
   if (nRead < 0)
   {
      fprintf(stderr, "Error Reading Msg : %d \n", errno);
      sprintf(sLine, "END   ReadLineMsg( NOK. 2. ERROR recv errno=[%d] )", errno);
      trace_event(sLine, FATAL);
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

	dump_buffer(sBuffRcv, nLenMsg, 'A', 'L', 'I');

	memcpy(sBuffer, sBuffRcv + 2, nLenMsg - 2);
	*nLength = nLenMsg - 2;


   /* Check Msg Length */
   if (nRead <= 20)
   {
      sprintf(sLine, "Error in Msg Length, nRead = %d", nRead);
      trace_event(sLine, PROCESSING);
      return (MALFUNCTION);
   }

	memset(sBuffRcv,0,sizeof(sBuffRcv)); /**PA DSS**/


	sprintf(sLine, "End   ReadLineMsg( OK )");
	trace_event(sLine, PROCESSING);

   return (OK);
}

int WriteLineMsg(char *sBuffer, int nLength)
{
   char sBuffSnd[LG_MAX];
   int retour;
   int i;
   int nFdC = nFdConnect;
   char    sLine[MAX_LINE_TRC];

   sprintf(sLine, "Start WriteLineMsg(%d, %d)", nFdC, nLength);
   trace_event(sLine, PROCESSING);

	sBuffSnd [ 0 ] = nLength / 256;
	sBuffSnd [ 1 ] = nLength % 256;



	memcpy(sBuffSnd + 2, sBuffer, nLength);

	dump_buffer(sBuffSnd, nLength + 2, 'A', 'L', 'O');

	retour = send(nFdC, sBuffSnd, nLength + 2, 0);
   if (retour <= 0)
   {
      sprintf(sLine, "Err When Calling send() : %d", errno);
      trace_event(sLine, PROCESSING);
      sprintf(sLine, "End   WriteLineMsg(NOK)");
      trace_event(sLine, PROCESSING);
      return (NOK);
   }

   sprintf(sLine, "End   WriteLineMsg(OK)");
   trace_event(sLine, PROCESSING);
   return (OK);
}


