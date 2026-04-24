#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <line_com.h>
#include <tcp_var.h>

void  CloseCldCom()
{
   shutdown(nFdConnect, 2);
}

void  GetFdConnect(char *sFdStr)
{
   sprintf(sFdStr,"%.6d", nFdConnect);
}

void  SetFdConnect(char *sFdStr)
{
   nFdConnect = atoi(sFdStr);
}

int ReadLineMsg( char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   int   nLenMsg, nRead;

   memset (sBuffRcv, 0, sizeof(sBuffRcv));
   nRead = recv(nFdConnect, sBuffRcv, 512, 0);
   if (nRead <= 0)
   {
      fprintf(stderr,"Error Reading Msg : %d \n", errno);
      return (NOK);
   }
   memcpy( sBuffer, sBuffRcv , nRead );
   *nLength = nRead;
   return (OK);
}

int WriteLineMsg( char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   int retour;

   memset (sBuffSnd, 0, LG_MAX);
   memcpy (sBuffSnd, sBuffer, nLength);
   retour = write ( nFdConnect, sBuffSnd, nLength);
   if (retour <= 0)
   {
      fprintf(stderr,"Error Sending Msg : %d \n", errno);
      return(NOK);
   }
   return(OK);
}

