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
/**
#include <line_com.h>
**/
#include <tcp_var.h>

#define LG_T24_HDR 4
#include <globals.h> /*BRO06122010: PCI*/
#define OR_DISCONECT -100

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
   char  sBuffRcv[ LG_MAX ];
   char  sLine[ 256 ];
   char  sLenMsg[ 4 + 1 ];
   char  sDest[ LG_MAX ]; /*BRO06122010: PCI*/
   int   nLenMsg, nRead;
   int   i;
   int   nFd = nFdConnect;
  



   memset(sBuffRcv, 0, sizeof(sBuffRcv));
   memset(sBuffer, 0, sizeof(sBuffer));
   memset(sLine, 0, sizeof(sLine));
   memset(sLenMsg, 0, sizeof(sLenMsg));
   memset (sDest, 0, sizeof(sDest));  /*BRO06122010: PCI*/

   
   sprintf(sLine, "Start ReadLineMsg(%d)", nFd);
   trace_event(sLine, PROCESSING);
   
   /*** Lecture de La Longueuer du message ****/
   do
   {
       nRead = recv(nFd, sBuffRcv, LG_T24_HDR, MSG_PEEK);
   } while ( (nRead !=LG_T24_HDR) && (nRead > 0));
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
   nLenMsg = atoi(sLenMsg) + LG_T24_HDR;

   memset(sBuffRcv, 0, sizeof(sBuffRcv));
   nRead = recv(nFd, sBuffRcv, nLenMsg, 0);
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

   /*Start BRO06122010: PCI*/
   if (debug_option == 1)
	dump_buffer ( sBuffRcv, nLenMsg , 'A', 'L', 'I' );
   else
	EncryptBuff(sBuffRcv,nLenMsg,sDest);
   /*End BRO06122010: PCI*/

   memcpy( sBuffer, sBuffRcv , nLenMsg );
   *nLength = nLenMsg;

   sprintf(sLine, "End   ReadLineMsg(%d)", *nLength);
   trace_event(sLine, PROCESSING);

   return (OK);
}

int WriteLineMsg(char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   char  sLine   [ MAX_LINE_TRC ];
   char  sDest[ LG_MAX ]; /*BRO06122010: PCI*/
   int   retour;
   int   i;
   int   nFdC = nFdConnect;
   

   sprintf(sLine, "Start WriteLineMsg(%d, %d)", nFdC, nLength);
   trace_event(sLine, PROCESSING);

   memset (sBuffSnd, 0, sizeof(sBuffSnd));
   memset (sLine, 0, sizeof(sLine));
   memset (sDest, 0, sizeof(sDest));  /*BRO06122010: PCI*/
   
   
   memcpy (sBuffSnd , sBuffer, nLength);

   /*Start BRO06122010: PCI*/
   if (debug_option == 1) 
	dump_buffer ( sBuffSnd, nLength , 'A', 'L', 'O' );
   else
	EncryptBuff(sBuffSnd,nLength,sDest);
  /*End BRO06122010: PCI*/

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
/** Start IBO15122009 : Add Read And Write Specifique Functions For Instant Issuing Interface **/
/*------ RJE151109 ---------------------------------------------------*/
int ReadLineMsgInst(int   nFd, char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   char  sLine[ 256 ];
   char  sLenMsg[ 256 ];
   int   nLenMsg, nRead;
   int   i;
   char  sDest[ LG_MAX ]; /*BRO06122010: PCI*/

   sprintf(sLine, "Start ReadLineMsgInst(%d)", nFd);
   trace_event(sLine, PROCESSING);

   memset (sDest, 0, sizeof(sDest));  /*BRO06122010: PCI*/
   memset(sBuffRcv, 0, sizeof(sBuffRcv));
   /*** Lecture de La Longueuer du message ****/
   do
   {
       nRead = recv(nFd, sBuffRcv, LG_T24_HDR, MSG_PEEK);
   } while ( (nRead !=LG_T24_HDR) && (nRead > 0));
   if (nRead < 0)
   {
      sprintf(sLine, "Error Reading Msg Length : %d nRead = %d", errno, nRead);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadLineMsgInst(NOK)");
      trace_event(sLine, PROCESSING);
      return (NOK);
   }
   if (nRead == 0)
   {
      sprintf(sLine, "Disconnect : %d", errno);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadLineMsgInst(DISCONNECT)");
      trace_event(sLine, PROCESSING);
      return (OR_DISCONECT);
   }

   sprintf(sLenMsg,"%.4s", sBuffRcv);
  /* La longueur dans le header du message n'inclu pas la longueur de la longueur */
   nLenMsg = atoi(sLenMsg) + LG_T24_HDR;

   memset(sBuffRcv, 0, sizeof(sBuffRcv));
   nRead = recv(nFd, sBuffRcv, nLenMsg, 0);
   if (nRead < 0)
   {
      sprintf(sLine, "Error Reading Msg : %d", errno);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadLineMsgInst(NOK)");
      trace_event(sLine, PROCESSING);
      return (NOK);
   }
   if (nRead == 0)
   {
      sprintf(sLine, "Disconnect : %d", errno);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadLineMsgInst(DISCONNECT)");
      trace_event(sLine, PROCESSING);
      return (OR_DISCONECT);
   }

   /*Start BRO06122010: PCI*/
   if (debug_option == 1)
	dump_buffer ( sBuffRcv, nLenMsg , 'A', 'L', 'I' );
   else
	EncryptBuff(sBuffRcv,nLenMsg,sDest);
   /*End BRO06122010: PCI*/

   memcpy( sBuffer, sBuffRcv , nLenMsg );
   *nLength = nLenMsg;

   sprintf(sLine, "End   ReadLineMsgInst(%d)", *nLength);
   trace_event(sLine, PROCESSING);

   return (OK);
}
/*------ BHS MDA 11112008 ---------------------------------------------------*/
/*------  RJE151109 ---------------------------------------------------*/
int WriteLineMsgInst(int nFdC, char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   char  sLine   [ 256 ];
   int   retour;
   int   i;
   char  sDest[ LG_MAX ]; /*BRO06122010: PCI*/

   sprintf(sLine, "Start WriteLineMsgInst(%d, %d)", nFdC, nLength);
   trace_event(sLine, PROCESSING);

   memset (sBuffSnd, 0, LG_MAX);
   memset (sDest, 0, sizeof(sDest));  /*BRO06122010: PCI*/ 
   memcpy (sBuffSnd , sBuffer, nLength);

   /*Start BRO06122010: PCI*/
   if (debug_option == 1) 
	dump_buffer ( sBuffSnd, nLength , 'A', 'L', 'O' );
   else
	EncryptBuff(sBuffSnd,nLength,sDest);
   /*End BRO06122010: PCI*/

   retour = send ( nFdC, sBuffSnd, nLength , 0);
   if (retour <= 0)
   {
      sprintf(sLine, "Err When Calling send() : %d", errno);
      trace_event(sLine, PROCESSING);
      sprintf(sLine, "End   WriteLineMsg(NOK)");
      trace_event(sLine, PROCESSING);
      return(NOK);
   }

   sprintf(sLine, "End   WriteLineMsgInst(OK)");
   trace_event(sLine, PROCESSING);
   return(OK);
}
/*------ BHS MDA 11112008 ---------------------------------------------------*/

/** End   IBO15122009 : Add Read And Write Specifique Functions For Instant Issuing Interface **/

int  CloseCldCom()
{
return(OK);
}

int tcp_readcheck( int nFdConnect , int nTimer )
{
    int nResult ;
    fd_set rfds ;
    struct timeval tv;
    char  sLine[ MAX_LINE_TRC ];

    sprintf(sLine,"Start tcp_readcheck(%d)",nTimer);
    trace_event(sLine,PROCESSING);


    if( nFdConnect < 0 ) return -1 ;             /* bad socket id */
    if( nTimer <= 0 ) return -2 ;                /* bad timer */

    FD_ZERO(&rfds); FD_SET(nFdConnect , &rfds) ; /* check only sd */

    tv.tv_sec = nTimer ;                         /* set timer */
    tv.tv_usec = 0 ;                         /* set timer */

    nResult = select(nFdConnect+1, &rfds, 0, 0, &tv ) ;  /* check it */
    if( nResult == -1 )
    {
        trace_event ("tcp_readcheck ERROR / select() returned -1" , PROCESSING);
        return (NOK);
    }

    if( nResult == 0 )
    {
        trace_event ("tcp_readcheck TIME OUT/ select() returned 0" , PROCESSING);
        return (NOK);
    }
    return (OK);
 }
