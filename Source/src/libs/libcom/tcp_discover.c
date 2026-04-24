
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>

#include <tcp_var.h>
#include <atmopcode.h>

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
   char  sLine[ LG_MAX ];
   int   nLenMsg, nRead;
   int   nRetCode;
   int retour;
   char  sDest[ LG_MAX ]; 

   trace_event("Start ReadLineMsg()",TRACE);

   memset (sBuffRcv, 0, sizeof(sBuffRcv));
 
   memset (sLine, 0, sizeof(sLine));

   do
   {
      nRead = recv(nFdConnect, sBuffRcv, 2 , 0);
   } 
   while ( (nRead !=2) && (nRead > 0));
   if (nRead < 0)
   {
      fprintf(stderr,"Error Reading MSG_PEEK : %d \n", errno);
      return (OR_BAL);
   }
   if (nRead == 0)
   {
      fprintf(stderr,"Disconnect MSG_PEEK: %d \n", errno);
      return (OR_DISCONECT);
   }
   
   nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + 2 ;
    sprintf(sLine,"Lenght (OK) [%02X] [%02X] [%d]",sBuffRcv[0], sBuffRcv[1], nLenMsg);
   trace_event(sLine,TRACE);
   
   
   memset(sBuffRcv,0,sizeof(sBuffRcv));
   
   nRead = recv(nFdConnect, sBuffRcv, nLenMsg, 0);

   if (nRead <= 0)
   {
      sprintf(sLine,"End   ReadLineMsg(NOK) nRead [%d] errno : [%d] , nFdConn : [%d]",nRead,errno, nFdConnect);
      trace_event(sLine,TRACE);
      return (NOK);
   }


   memcpy( sBuffer, sBuffRcv , nRead );
   *nLength = nRead;
   
   sprintf(sLine,"Msg(OK) nRead [%d]",nRead);
   trace_event(sLine,TRACE);

   if (debug_option == 1)
      dump_buffer(sBuffer, *nLength, 'A', 'L', 'I');
   else
    EncryptBuff(sBuffRcv,*nLength,sDest);

   sprintf(sLine,"End   ReadLineMsg(OK | FD [%d], [%d])",nFdConnect, nRead);
   trace_event(sLine,TRACE);
   return (OK);
}


 
int WriteLineMsg( char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   char  sLine[ LG_MAX ];
   char  sDest[ LG_MAX ]; 
   int   retour;
   
   sprintf (sLine, "Start WriteLineMsg(nFdConn=[%d], [%d])", nFdConnect, nLength);
   trace_event(sLine,PROCESSING);

   memset (sBuffSnd, 0, LG_MAX);
   
   sBuffSnd[0] = (nLength)/256;
   sBuffSnd[1] = (nLength)%256;
   
   memcpy (sBuffSnd + 2, sBuffer, nLength);

   fprintf ( stderr,"\n av send Socket = %d longueur = %d \n", nFdConnect , nLength );
 
   sprintf(sLine, "longueur = %d", nLength);
   trace_event(sLine, PROCESSING);

   sprintf(sLine, "BuffSnd[%02X][%02X] ",sBuffSnd [ 0 ], sBuffSnd [ 1 ]);
   trace_event(sLine, PROCESSING);
   
   if (debug_option == 1)
      dump_buffer(sBuffSnd, nLength + 2, 'A', 'L', 'O');
   else
      EncryptBuff(sBuffSnd,nLength + 2,sDest);

   retour = write ( nFdConnect, sBuffSnd, nLength + 2);
   if (retour <= 0)
   {
      sprintf(sLine, "End   WriteLineMsg(NOK [%d], [%d]) Error Sending.",retour, errno);
      trace_event(sLine, FATAL);
      fprintf(stderr,"Error WriteLineMsg Sending Msg : [%d].\n", errno);
      return(NOK);
   }
   
   sprintf(sLine, "End   WriteLineMsg(OK FD:[%d], Len [%d])", nFdConnect, retour);
   trace_event(sLine, FATAL);
   return(OK);
}


 
