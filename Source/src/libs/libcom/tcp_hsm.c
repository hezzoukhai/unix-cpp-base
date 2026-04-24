#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <line_com.h>
#include <tcp_var.h>


int ReadLineHsm( int nFdConn, char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   char  sLine[ MAX_LINE_TRC ];
   int   nLenMsg, nRead;
   int   i;
   int		nEncrLen;
   

   sprintf(sLine, "START ReadLineHsm( nFdConn=[%d])",  nFdConn); 
   trace_event(sLine, PROCESSING);


   nRead = recv(nFdConn, sBuffRcv, 2, MSG_PEEK);
   if (nRead <= 0)
   {
      sprintf(sLine,"Error Reading Msg Length : %d nRead = %d", errno, nRead );
      fprintf(stderr, sLine); 
	  trace_event(sLine, PROCESSING);
      return (NOK);
   }

   nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + 2;
	nRead = recv(nFdConn, sBuffRcv, nLenMsg, MSG_WAITALL);
   if (nRead <= 0)
   {
      sprintf(sLine, "END   ReadLineHsm(NOK) errno=[%d]", errno); trace_event(sLine, PROCESSING);
	  fprintf(stderr,sLine);
      return (NOK);
   }


	
	if (debug_option == 1)
		dump_buffer(sBuffRcv, nLenMsg, 'A', 'L', 'I');
	
   
   memcpy( sBuffer, sBuffRcv + 2 + 4, nLenMsg - 2-4);
   *nLength = nLenMsg - 2-4;

   memset (sBuffRcv, 0, sizeof(sBuffRcv)); /*PADSS2020*/

   sprintf(sLine,"End   ReadLineHsm(OK)(%d)",*nLength);
   trace_event(sLine,PROCESSING);
   return (OK);
}

int WriteLineHsm( int nFdConn, char *sBuffer, int nLength)
{
   char		sBuffSnd[ LG_MAX ];
   char		sLine[ MAX_LINE_TRC ];
   int		retour;
   int		i;
   int		nEncrLen;
   char         sHeader[4];
   

   

   sprintf(sLine,"Start WriteLineHsm(  nFdConn=[%d])",nFdConn);
   trace_event(sLine,PROCESSING);



   memset (sBuffSnd, 0, sizeof(sBuffSnd));
   memset (sHeader, 0, sizeof(sHeader)); 

   sBuffSnd[0] = (nLength+4)/256;
   sBuffSnd[1] = (nLength+4)%256;

memcpy (sBuffSnd +2 , "HPS7", 4);

   AsciiToHex("58580000",sHeader,8);
   
   memcpy (sBuffSnd +2 , sHeader, 4);
   memcpy (sBuffSnd +2 +4, sBuffer, nLength);
   
   retour = write ( nFdConn, sBuffSnd, nLength+2+4);
   if (retour <= 0)
   {
      	sprintf(sLine,"Error Sending Msg : %d ", errno);
   	trace_event(sLine,ERROR);
	return(NOK);
		
   }

	if (debug_option == 1)
		dump_buffer(sBuffSnd, nLength+2+4, 'A', 'L', 'O');

   memset (sBuffSnd, 0, sizeof(sBuffSnd));  /*PADSS2020*/
   
   sprintf(sLine,"End   WriteLineHsm(%d)",retour);
   trace_event(sLine,PROCESSING);
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

	nResult = select(nFdConnect+1, &rfds, NULL, NULL, &tv ) ;  /* check it */
	sprintf(sLine," tcp_readcheck(ret:%d,errno:%d",nResult,errno);
	trace_event (sLine , PROCESSING); 

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
