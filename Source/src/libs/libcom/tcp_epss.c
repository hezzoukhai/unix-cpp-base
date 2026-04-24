/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/*  NAME                                                                       */
/*      tcp_visa.c                                                             */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Comunication with visa TCP/IP protocol                              */
/* NOTES                                                                       */
/*                                                                             */
/* MODIFIED                                                                    */
/*        H.YOUSFI   12/05/2002 -         V 2.0.1                              */
/*        Correction of the length in the ReadLine Msg function                */
/*        Delete of the printf calls                                           */
/*        See HYJCC120502                                                      */
/*******************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
/*#include <line_com.h>*/
#include <tcp_var.h>
#include <atmopcode.h>

#include <globals.h> /* MJA PCI 220411 */

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
   int   i;
   int   nFd = nFdConnect;
   char sLine[MAX_LINE_TRC];

char  sDest[ LG_MAX ];   /** MJA PCI 270411 **/

   memset(sDest,0,sizeof(sDest));  /** MJA PCI 270411 **/

   trace_event("Start ReadLineMsg()",PROCESSING);

   /*** Lecture de La Longueuer du message ****/
   do
   {
       nRead = recv(nFd, sBuffRcv, LG_VTCP_HDR, MSG_PEEK);
   } while ( (nRead !=LG_VTCP_HDR) && (nRead > 0));

   if (nRead == 0)  /*KS 170907 Pb crash Bank Member */
   {
      sprintf(sLine,"End   ReadLineMsg(DISCONECT) errno : %d nRead = %d ", errno, nRead );
          trace_event(sLine,PROCESSING);
      /*return (OR_DISCONECT);*/
      return (NOK);
   }

/*   if (nRead <= 0)*/
   if (nRead < 0) /*KS 170907 Pb crash Bank Member */
   {
      sprintf(sLine,"End   ReadLineMsg(NOK) errno : %d nRead = %d ", errno, nRead );
	  trace_event(sLine,PROCESSING);
      return (NOK);
   }

   nLenMsg = (sBuffRcv[2] * 256) + sBuffRcv[3] + LG_VTCP_HDR ;
   nRead = recv(nFd, sBuffRcv, nLenMsg, 0);
   if (nRead < 0)
   {
      sprintf(sLine,"End   ReadLineMsg(NOK) errno : %d nRead = %d ", errno, nRead );
	  trace_event(sLine,PROCESSING);
      return (NOK);
   }
/*Start KS 170907 Pb crash Bank Member*/
/*   if (nRead = 0)
   {
      sprintf(sLine,"End   ReadLineMsg(DISCONECT) errno : %d nRead = %d ", errno, nRead );
	  trace_event(sLine,PROCESSING);
      return (OR_DISCONECT);
   }

*/
/*End KS 170907 Pb crash Bank Member*/
   /* memcpy( sBuffer, sBuffRcv + LG_VTCP_HDR+2, nLenMsg - LG_VTCP_HDR-2); */
   memcpy( sBuffer, sBuffRcv + LG_VTCP_HDR, nLenMsg - LG_VTCP_HDR);
/* Start HY120502 */
   /* *nLength = nLenMsg; */
   *nLength = nLenMsg - LG_VTCP_HDR;
/* End   HY120502 */

  /** START MJA PCI 270411  **/

   if (debug_option == 1)
   dump_buffer(sBuffRcv, nLenMsg + LG_VTCP_HDR , 'A','L','I');
   else
        EncryptBuff(sBuffer, *nLength,sDest);

 /** END  MJA PCI 270411  **/


   sprintf(sLine,"End   ReadLineMsg (OK)  nRead = %d ", nRead );
   trace_event(sLine,PROCESSING);

   return (OK);
}

int WriteLineMsg(char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   int retour;
   int i;
   int   nFdC = nFdConnect;
   char  sLine[MAX_LINE_TRC];

   char  sDest[ LG_MAX ];   /** MJA PCI 270411 **/

   memset(sDest,0,sizeof(sDest));  /** MJA PCI 270411 **/

   trace_event("Start WriteLineMsg()",PROCESSING);

   memset (sBuffSnd, 0, LG_MAX);

   sBuffSnd [ 2 ] = nLength / 256;
   sBuffSnd [ 3 ] = nLength % 256;
   memcpy (sBuffSnd + LG_VTCP_HDR, sBuffer, nLength);

   retour = send ( nFdC, sBuffSnd, nLength + LG_VTCP_HDR, 0);
   if (retour <= 0)
   {
      sprintf(sLine,"End   WriteLineMsg(NOK) errno : %d ", errno);
	  trace_event(sLine,PROCESSING);
      return(NOK);
   }
   /** START MJA PCI 270411  **/

   if (debug_option == 1)
        dump_buffer(sBuffSnd, nLength + LG_VTCP_HDR , 'A','L','O');
   else
        EncryptBuff(sBuffer, nLength,sDest);

 /** END  MJA PCI 270411  **/

   sprintf(sLine,"End   WriteLineMsg(%d)",nLength + LG_VTCP_HDR);
   trace_event(sLine,PROCESSING);
   return(OK);
}

int  CloseCldCom()
{
return(OK);
}
