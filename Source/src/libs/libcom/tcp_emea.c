#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <srv_com.h>
#include <tcp_var.h>
#include <ascii.h>
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


int ReadLineMsg( int nFdConn, char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   char  sLine[ LG_MAX ];
   int   nLenMsg, nRead;
   int   nRetCode;
   char  sDest[ LG_MAX ]; /*MKB060309 PCI Norme Integration */
   
   sprintf(sLine,"Start ReadLineMsg(%d)",nFdConn);
   trace_event(sLine,PROCESSING);

   memset (sBuffRcv, 0, sizeof(sBuffRcv));
   /*nRead = recv(nFdConn, sBuffRcv, LG_MAX, 0); */
   nRead = recv(nFdConn, sBuffRcv, 256, 0);
   if (nRead <= 0)
   {
      sprintf(sLine,"End   ReadLineMsg(NOK) errno : %d ",errno);
	  trace_event(sLine,TRACE);
      return (NOK);
   }
	if (sBuffRcv[nRead - 1] == ETX)
	{
		nRead += 1;
	}


   	memcpy( sBuffer, sBuffRcv , nRead );
   *nLength = nRead;

   /*Begin MKB060309 PCI Norme Integration */
   if (debug_option == 1)
   	dump_buffer(sBuffer, *nLength, 'A', 'L', 'I');
   else
	EncryptBuff(sBuffer,*nLength,sDest);
   /*End MKB060309 PCI Norme Integration */

   trace_event("End   ReadLineMsg(OK)",TRACE);
   return (OK);
}

int WriteLineMsg( int nFdConn, char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   int retour;
   char  sDest[ LG_MAX ]; /*MKB060309 PCI Norme Integration */

   trace_event("Start WriteLineMsg()",TRACE);

   /*Begin MKB060309 PCI Norme Integration */
   if (debug_option == 1)
   	dump_buffer(sBuffer, nLength, 'A', 'L', 'O');
   else
	EncryptBuff(sBuffer,nLength,sDest);
   /*End MKB060309 PCI Norme Integration */

   memset (sBuffSnd, 0, LG_MAX);
   memcpy (sBuffSnd, sBuffer, nLength);
   retour = write ( nFdConn, sBuffSnd, nLength);
   if (retour <= 0)
   {
      fprintf(stderr,"Error Sending Msg : %d \n", errno);
      return(NOK);
   }
   trace_event("End   WriteLineMsg(OK)",TRACE);
   return(OK);
}

