#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <line_com.h>
#include <tcp_var.h>
#define PKT_READ_SIZE 10
#define MAX_MSG 8192

int ReadLineHsm( int nFdConn, char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   char  sLine[ MAX_LINE_TRC ];
   int   nLenMsg, nRead;
   int   i;
   int		nEncrLen;
   int 		rsp_start = 0;
   int 	msg_done = 0;
   char retmsg[MAX_MSG];
   int 	rsp_ptr = 0;


   sprintf(sLine, "START ReadLineHsm( nFdConn=[%d])",  nFdConn);
   trace_event(sLine, PROCESSING);

   rsp_start = 0;
   	do
   	{
   		int i = 0;
   		nRead = recv(nFdConn, sBuffRcv, (size_t)(PKT_READ_SIZE), 0);
   		if (nRead <= 0)
   		{
   			sprintf(sLine,"Error Reading Msg Length : %d nRead = %d", errno, nRead );
   		    fprintf(stderr, sLine);
   			trace_event(sLine, PROCESSING);
   		    return (NOK);
   		}
   		i = 0;
   		if (rsp_start == 0)
   		{
   			/*
   			Search for the start of the response
   			*/
   			for (; i<nRead; i++)
   			{
   				if (sBuffRcv[i] == '<')
   				{
   					/*
   					* Found the start of the response
   					*/
   					rsp_start = 1;
   					break;
   				}
   			}
   		}
   		if (rsp_start != 0)
   		{
   			/*
   			* Process the response, copy characters into the output buffer
   			*/
   			for (; i<nRead; i++)
   			{
   				/*
   				* Error if response get too big for the buffer allocated
   				*/
   				if (rsp_ptr >= MAX_MSG - 1)
   				{
   					return (NOK);
   				}
   				retmsg[rsp_ptr++] = sBuffRcv[i];
   				if (sBuffRcv[i] == '>')
   				{
   					msg_done = 1;
   					break;
   				}
   			}
   		}
   	/*
   	* Continue to perform socket reads to get the whole response
   	*/
   	} while (msg_done == 0);

   	retmsg[rsp_ptr] = 0;

   	if (debug_option == 1)
   		dump_buffer(retmsg, rsp_ptr, 'A', 'L', 'I');

    memcpy( sBuffer, retmsg, rsp_ptr);
    *nLength = rsp_ptr;

   memset (sBuffRcv, 0, sizeof(sBuffRcv));

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

   retour = write ( nFdConn, sBuffer, nLength);
   if (retour <= 0)
   {
      	sprintf(sLine,"Error Sending Msg : %d ", errno);
   	trace_event(sLine,ERROR);
	return(NOK);
		
   }

	if (debug_option == 1)
		dump_buffer(sBuffer, nLength, 'A', 'L', 'O');

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
