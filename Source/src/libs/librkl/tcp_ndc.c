/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_ndc.c                                                                 */
/*                                                                             */
/* Description                                                                 */
/*      TCP NDC Tools Management                                        */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <line_com.h>
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
int ReadLineMsgAvis(int nFd, char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   int   nLenMsg, nRead;
   int   i;

   nRead = recv(nFd, sBuffRcv, LG_MAX, 0);
   if (nRead < 0)
   {
      fprintf(stderr,"Error Reading Msg : %d \n", errno);
      return (OR_BAL);
   }
   if (nRead == 0)
   {
      fprintf(stderr,"Disconnect : %d \n", errno);
      return (OR_DISCONECT);
   }
   memcpy( sBuffer, sBuffRcv, nRead );
   *nLength = nRead;
   return (OK);
}

int WriteLineMsgAvis(int nFdC,char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   int retour;
   int i;

   memset (sBuffSnd, 0, LG_MAX);

   memcpy (sBuffSnd, sBuffer, nLength);
   fprintf ( stderr,"\n av send Socket = %d longueur = %d \n", nFdC , nLength );
   retour = send ( nFdC, sBuffSnd, nLength, 0);
   if (retour <= 0)
   {
      fprintf(stderr,"Error Sending Msg : %d \n", errno);
      return(NOK);
   }
   return(OK);
}

int  CloseCldCom()
{
return(OK);
}

int ReadLineMsg(int nFd, char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   char  sLine   [ LG_MAX ];
   int   nLenMsg =0, nRead=0;

   /*fprintf ( stderr,"\n av rec Socket = %d \n", nFd );*/
   memset(sBuffRcv,0,sizeof(sBuffRcv));
   *nLength = 0;
   do
   {
       nRead = recv(nFd, sBuffRcv, 2 , 0);
   } while ( (nRead !=2) && (nRead > 0));
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
   nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] ;
   /*sprintf(sLine,"NLENMSG = %d BUFF = %02X %02X \n", nLenMsg, (unsigned char)sBuffRcv[0], (unsigned char)sBuffRcv[1]);
   trace_event(sLine, PROCESSING);*/
   memset(sBuffRcv,0,sizeof(sBuffRcv));
   nRead = recv(nFd, sBuffRcv, nLenMsg, 0);
   if (nRead < 0)
   {
      fprintf(stderr,"Error Reading Msg : %d nRead :%d\n", errno,nRead);
      return (OR_BAL);
   }
   if (nRead == 0)
   {
      fprintf(stderr,"Disconnect : %d \n", errno);
      return (OR_DISCONECT);
   }
   /*memcpy( sBuffer, sBuffRcv +  2  , nLenMsg - 2);*/
   memcpy( sBuffer, sBuffRcv   , nLenMsg );
   *nLength = nLenMsg;
   return (OK);
}

int WriteLineMsg(int nFd, char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   int retour;
   int i;

   memset (sBuffSnd, 0, LG_MAX);
   sBuffSnd [ 0 ] = (nLength ) / 256;
   sBuffSnd [ 1 ] = (nLength ) % 256;
   memcpy (sBuffSnd +2 , sBuffer, nLength);
   /*fprintf ( stderr,"\n av send Socket = %d longueur = %d \n", nFd , nLength );*/
   retour = write ( nFd, sBuffSnd, nLength+2);
   if (retour <= 0)
   {
      fprintf(stderr,"Error Sending Msg : %d \n", errno);
      return(NOK);
   }
   return(OK);
}
