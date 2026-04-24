/*******************************************************************************/
/*  Communication with POSTILION uses a 2-byte header to hold the actual       */
/*  data length.                                                               */
/*******************************************************************************/
#include <stdio.h>
#include <errno.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <tcp_var.h>
#include <string.h>/*SWI10112009*/

#define  DISCONNECT  99
#define  OR_BAL      13

#ifndef P7_COMM_NO_TLS
#include <openssl/bio.h>
#include <openssl/ssl.h>


BIO* get_bio(int nConnDesc);



void  GetFdConnect(char *sFdStr)
{
	sprintf(sFdStr,"%.6d", nFdConnect);
}

void  SetFdConnect(char *sFdStr)
{
	nFdConnect = atoi(sFdStr);
}

int  CloseCldCom()
{
	return(OK);
}


int ReadLineMsgTLS(int nConnId, char *sBuffer, int *nLength)
{
	int				nLenMsg, nRead=0;
	char			sLine[MAX_LINE_TRC];
	int				i;

	BIO *			conn_bio;

	sprintf ( sLine,"Start ReadLineMsgTLS(%d)", nConnId );
	trace_event(sLine,PROCESSING);

	conn_bio = get_bio(nConnId);

	if(BIO_do_handshake(conn_bio) <= 0) 
	{
		sprintf(sLine, "Error in SSL handshake");
		trace_event(sLine,ERROR);
		return (NOK);
	}


	trace_event("Handshake Successful",PROCESSING);


	nRead = BIO_read(conn_bio,sBuffer,2); 
	if (nRead <= 0)
	{
		sprintf(sLine, "END   ReadLineMsgTLS( NOK.errno=[%d][%d] )", errno ,nRead);
		trace_event(sLine, FATAL);
		return (NOK);
	}
	sBuffer[2]='\0';

	nLenMsg = ((unsigned char)sBuffer[0] * 256) + (unsigned char)sBuffer[1] + 2 ;

	/*if (nLenMsg > LG_MAX -1 )
	{
		sprintf ( sLine,"ReadLineMsg Msg Too Big %d)", nLenMsg );
		trace_event(sLine,ERROR);
		nRead =  BIO_read(conn_bio,sBuffer,LG_MAX -1); 


		trace_event("ATM Will Be Disconnected",ERROR);
		return (OR_DISCONECT);
	}*/


	nRead = BIO_read(conn_bio,sBuffer,nLenMsg - 2);
	if (nRead < 0)
	{
		sprintf(sLine, "Error Reading Msg : %d|%d", errno,nRead);
		trace_event(sLine, ERROR);
		if ( errno == EINTR)
		{
			return (LINE_INTR);
		}

		return(LINE_DISCONECT);
	}

	if (nRead == 0)
	{
		sprintf(sLine, "Disconnect : %d", errno);
		trace_event(sLine, ERROR);
		return (LINE_DISCONECT);
	}

	*nLength = nLenMsg -2;

	if (debug_option == 1)
		dump_buffer ( sBuffer, *nLength , 'A', 'L', 'I' );

	sprintf(sLine, "End   ReadLineMsgTLS(%d)", *nLength);
	trace_event(sLine, PROCESSING);

	return (OK);
}



int WriteLineMsgTLS(int nConnId, char *sBuffer, int nLength)
{
	int retour;
	int i;
	char  sLine   [ MAX_LINE_TRC ]; 
	char  sBuffSnd[ LG_MAX_POSTILION ];
	BIO *			conn_bio;

	sprintf(sLine, "Start WriteLineMsgTLS(%d, %d)", nConnId, nLength);
	trace_event(sLine, PROCESSING);
	
	memset (sBuffSnd, 0, LG_MAX_POSTILION);
	
	conn_bio = get_bio(nConnId);

	sBuffSnd [ 0 ] = (nLength ) / 256;
	sBuffSnd [ 1 ] = (nLength ) % 256;
	memcpy (sBuffSnd +2 , sBuffer, nLength);

	if (debug_option == 1)
		dump_buffer ( sBuffSnd, nLength + 2 , 'A', 'L', 'O' );


	retour = BIO_write(conn_bio,sBuffSnd,nLength + 2);
	BIO_flush(conn_bio);
	if (retour <= 0)
	{
		sprintf(sLine, "Error Sending Msg retour, nFd, errno :(%d, %d, %d)", retour, nConnId, errno);
		trace_event(sLine, ERROR); 
		return(NOK);
	}

	sprintf ( sLine,"End   WriteLineMsgTLS(%d)", retour );
	trace_event(sLine,PROCESSING);
	return(OK);
}

#endif


int ReadLineMsgTCP ( int nFd, char *sBuffer, int *nLength)
{
   char  sBuffRcv [ LG_MAX_POSTILION ];
   int   nLenMsg =0, nRead=0;
   char  sLine   [ 256 ];

/*sprintf(sLine, "Start ReadLineMsg(%d, %d)", nFd, nLength); SWI10112009*/
sprintf(sLine, "Start ReadLineMsgTCP(%d)", nFd);
trace_event(sLine, PROCESSING);

   do
   {
       nRead = recv (nFd, sBuffRcv, 2 , MSG_PEEK);
   }   while ((nRead !=2) && (nRead > 0));

   if (nRead < 0)
   {
      fprintf (stderr,"Error Reading Msg : %d \n", errno);
      sprintf(sLine, "Error Reading Msg : %d %d\n", errno,nRead);
      trace_event(sLine, PROCESSING);
      /** return  (-1); **/
      return (OR_BAL);
   }

   if (nRead == 0)
   {
      fprintf (stderr,"Disconnect : %d \n", errno);
      sprintf(sLine, "Disconnect : %d \n", errno);
      trace_event(sLine, PROCESSING);
  return  (DISCONNECT); 
 
   }

   nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + 2 ;   

   memset (sBuffRcv, 0, sizeof(sBuffRcv));
   /** nRead = recv (nFdConnect, sBuffRcv, nLenMsg, 0); **/
   nRead = recv (nFd, sBuffRcv, nLenMsg, 0);

   if (nRead < 0)
   {
      fprintf (stderr,"Error Reading Msg : %d \n", errno);
      sprintf(sLine, "Error Reading Msg 2 : %d %d\n", errno,nRead);
      trace_event(sLine, PROCESSING);
      /** return  (-1); **/
      return (OR_BAL);
   }

   if (nRead == 0)
   {
      fprintf (stderr,"Disconnect : %d \n", errno);
      sprintf(sLine, "Disconnect 2: %d \n", errno);
      trace_event(sLine, PROCESSING);
      return  (DISCONNECT);
   }

/*Start 10112009*/
/*  Check Msg Length */
   if ( nRead <= 20)
   {
     sprintf ( sLine,"Error in Postilion Msg Length, nRead = %d nLenMsg=%d", nRead, nLenMsg );
     trace_event(sLine,PROCESSING);
     return ( MALFUNCTION );
   }
/*END 10112009*/

   memcpy ( sBuffer, sBuffRcv +  2  , nLenMsg - 2);  

   /*MGA_20151006*/
   /*nLength = nLenMsg;*/
   *nLength = nLenMsg - 2;
   /*MGA_20151006*/

/* fprintf (stderr,"Read Socket OK : %d \n", nLenMsg); SWI10112009*/
   sprintf ( sLine,"Read Socket OK : nRead = %d nLenMsg=%d", nRead, nLenMsg );
   trace_event(sLine,PROCESSING);
   sprintf(sLine, "End   ReadLineMsgTCP(OK)");
   trace_event(sLine, PROCESSING);
   return (OK);
}


/*int WriteLineMsg( int nFd, char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   int retour;
   int i;

   memset (sBuffSnd, 0, LG_MAX);
   sBuffSnd [ 0 ] = (nLength ) / 256;
   sBuffSnd [ 1 ] = (nLength ) % 256;
   memcpy (sBuffSnd +2 , sBuffer, nLength);

   retour = write ( nFd, sBuffSnd, nLength+2);
   if (retour <= 0)
   {
      fprintf (stderr,"Error Sending Msg : %d \n", errno);
      return (NOK);
   }
   return(OK);
}*/

int WriteLineMsgTCP(int nFdC, char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX_POSTILION ];
   char  sLine   [ 256 ];
   int retour;
   int i;
   /*nFdC = nFdConnect;*/

   sprintf(sLine, "Start WriteLineMsgTCP(%d, %d)", nFdC, nLength);
   trace_event(sLine, PROCESSING);

   memset (sBuffSnd, 0, LG_MAX_POSTILION);
   sBuffSnd [ 0 ] = (nLength ) / 256;
   sBuffSnd [ 1 ] = (nLength ) % 256;
   memcpy (sBuffSnd +2 , sBuffer, nLength);
   
   
  dump_buffer ( sBuffSnd, nLength + 2 , 'A', 'L', 'O' );

   retour = send ( nFdC, sBuffSnd, nLength + 2 , 0);
   if (retour <= 0)
   {
      sprintf(sLine, "Err When Calling send() : %d", errno);
      trace_event(sLine, PROCESSING);
      sprintf(sLine, "End   WriteLineMsg(NOK)");
      trace_event(sLine, PROCESSING);
      return(NOK);
   }

   sprintf(sLine, "End   WriteLineMsgTCP(OK)");
   trace_event(sLine, PROCESSING);
   return(OK);
   
   
}

/*Start MGA_20151006*/
/*******************************************************************************/
/*  To resolve segmented messages causing core of national switch interface    */
/*******************************************************************************/

int ReadLineMsgAll ( int nFd, char *sBuffer, int *nLength)
{
   char  sBuffRcv [ LG_MAX_POSTILION ]; /*ABM  07022022 Add Structured Data field*/
   int   nLenMsg=0, nRead=0, nLen=0;
   char  sLine   [ 256 ];

   memset (sLine, 0, sizeof(sLine));
   memset (sBuffRcv, 0, sizeof(sBuffRcv));
	
	sprintf(sLine, "Start ReadLineMsgAll(%d)", nFd);
	trace_event(sLine, PROCESSING);

   do
   {
       nRead = recv (nFd, sBuffRcv, 2 , MSG_PEEK);
   }   while ((nRead !=2) && (nRead > 0));

   if (nRead < 0)
   {
      fprintf (stderr,"Error Reading Msg : %d \n", errno);
      sprintf(sLine, "Error Reading Msg : %d %d\n", errno,nRead);
      trace_event(sLine, PROCESSING);
      return (OR_BAL);
   }

   if (nRead == 0)
   {
      fprintf (stderr,"Disconnect : %d \n", errno);
      sprintf(sLine, "Disconnect : %d \n", errno);
      trace_event(sLine, PROCESSING);
	  return  (DISCONNECT);  
   }

   nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + 2 ;   

   memset (sBuffRcv, 0, sizeof(sBuffRcv));

   nRead = recv (nFd, sBuffRcv, nLenMsg, MSG_WAITALL);

   if (nRead < 0)
   {
      fprintf (stderr,"Error Reading Msg : %d \n", errno);
      sprintf(sLine, "Error Reading Msg 2 : %d %d\n", errno,nRead);
      trace_event(sLine, PROCESSING);
      return (OR_BAL);
   }

   if (nRead == 0)
   {
      fprintf (stderr,"Disconnect : %d \n", errno);
      sprintf(sLine, "Disconnect 2: %d \n", errno);
      trace_event(sLine, PROCESSING);
      return  (DISCONNECT);
   }

   if ( nRead <= 20)
   {
     sprintf ( sLine,"Error in Postilion Msg Length, nRead = %d nLenMsg=%d", nRead, nLenMsg );
     trace_event(sLine,PROCESSING);
     return ( MALFUNCTION );
   }
   
   memcpy ( sBuffer, sBuffRcv +  2  , nLenMsg - 2);
   *nLength = nLenMsg - 2;
   
   sprintf ( sLine,"Read Socket OK : nRead = %d nLenMsg=%d", nRead, nLenMsg );
   trace_event(sLine,PROCESSING);
   
   dump_buffer(sBuffer, *nLength, 'A','I','L');
   
   sprintf(sLine, "End   ReadLineMsgAll(OK)");
   trace_event(sLine, PROCESSING);
   
   return (OK);
}

/*End MGA_20151006*/
