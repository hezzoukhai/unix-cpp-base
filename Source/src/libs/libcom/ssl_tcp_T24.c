/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_T24.c                                                             */
/*                                                                             */
/* Description                                                                 */
/*      SSL TCP T24 Tools Management                                          */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id                     Date                    Author                          Comment
-----------------------------------------------------------------------------------------------------------------------------------
001                     21/11/12               Anass OUNAINI                   Initial Version 		
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
#include <atmopcode.h>
#define LG_T24_HDR 4
#include <globals.h> /*BRO06122010: PCI*/

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <ssl_var.h>
#include <pthread.h> /*MAN07112013*/

pthread_mutex_t  sslwrite_mutex = PTHREAD_MUTEX_INITIALIZER; /*MAN07112013*/


/*
static SSL                  		*nSsl;
static SSL_METHOD           		*SslMeth;
*/


void  GetSslConnect(SSL *sSslStruct)
{
   sprintf(sSslStruct,"%s", nSsl);
}

void  GetCtxConnect(SSL_CTX *sCtxStruct)
{
   sprintf(sCtxStruct,"%s", SslCtx);
}

void  SetSslConnect(SSL *sSslStruct)
{
   nSsl = sSslStruct;
}

int WriteSslLineMsg(char *sBuffer, int nLength)
{
   char         sBuffSnd[ LG_MAX ];
   char         sDest[ LG_MAX ];
   char         sLine   [ 256 ];
   int          retour;
   int          i;
   

   if (ssl_active != 1)
		return(WriteLineMsg (sBuffer, nLength));
   
   memset (sBuffSnd, 0, sizeof(sBuffSnd));
   memset (sLine, 0, sizeof(sLine));
   memset (sDest, 0, sizeof(sDest));
   
   
   
   
   sprintf(sLine, "Start WriteSslLineMsg( %d)", nLength);
   trace_event(sLine, PROCESSING);

  
   
   memcpy (sBuffSnd , sBuffer, nLength);

   if (debug_option == 1)
        dump_buffer ( sBuffSnd, nLength , 'A', 'L', 'O' );
   else
        EncryptBuff(sBuffSnd,nLength,sDest);

	 pthread_mutex_lock(&sslwrite_mutex); 
   retour =    SSL_write( nSsl, sBuffSnd, nLength);
   if (retour <= 0)
   {
      sprintf(sLine, "Err When Calling SSL_write() : %d", errno);
      trace_event(sLine, PROCESSING);
      pthread_mutex_unlock(&sslwrite_mutex); /*MAN07112013*/
      sprintf(sLine, "End   WriteSslLineMsg(NOK)");
      trace_event(sLine, PROCESSING);
      return(NOK);
   }
	 pthread_mutex_unlock(&sslwrite_mutex); /*MAN07112013*/
   sprintf(sLine, "End   WriteSslLineMsg(OK)");
   trace_event(sLine, PROCESSING);
   return(OK);
}

int ReadSslLineMsg(char *sBuffer, int *nLength)
{
   SSL   *ssl;
   char  sBuffRcv[ LG_MAX ];
   char  sLine[ MAX_LINE_TRC ];
   char  sLenMsg[ 4 + 1 ];
   char  sDest[ LG_MAX ]; /*BRO06122010: PCI*/
   int   nLenMsg, nRead;
   int   i;
   
   
   int   result=0;


   memset(sBuffRcv, 0, sizeof(sBuffRcv));
   memset(sLine, 0, sizeof(sLine));
   memset(sLenMsg, 0, sizeof(sLenMsg));
   memset(sBuffer, 0, sizeof(sBuffer));
   memset(sDest, 0, sizeof(sDest));  /*BRO06122010: PCI*/

   
    if (ssl_active != 1)
		return(ReadLineMsg (sBuffer, nLength));
   
   sprintf(sLine, "Start ReadSslLineMsg()");
   trace_event(sLine, PROCESSING);



   /*** Lecture de La Longueuer du message ****/
   do
   {
       nRead = SSL_read(nSsl, sBuffRcv, LG_T24_HDR);
   } while ( (nRead !=LG_T24_HDR) && (nRead > 0));
   if (nRead < 0)
   {
      result=SSL_get_error(nSsl,nRead);
      sprintf(sLine, "Error Reading Msg Length : %d nRead = %d,SSL_get_error= %d ", errno, nRead, result);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadSslLineMsg(NOK)");
      trace_event(sLine, PROCESSING);
      return (NOK);
   }
   if (nRead == 0)
   {
      sprintf(sLine, "Disconnect : %d", errno);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadSslLineMsg(DISCONNECT)");
      trace_event(sLine, PROCESSING);
      return (OR_DISCONECT);
   }

   sprintf(sLenMsg,"%.4s", sBuffRcv);
  /* La longueur dans le header du message n'inclu pas la longueur de la longueur */
   nLenMsg = atoi(sLenMsg) + LG_T24_HDR;

   memset(sBuffRcv, 0, sizeof(sBuffRcv));
   nRead = SSL_read(nSsl, sBuffRcv, nLenMsg);
   if (nRead < 0)
   {
      sprintf(sLine, "Error Reading Msg : %d", errno);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadSslLineMsg(NOK)");
      trace_event(sLine, PROCESSING);
      return (NOK);
   }
   if (nRead == 0)
   {
      sprintf(sLine, "Disconnect : %d", errno);
      trace_event(sLine, ERROR);
      sprintf(sLine, "End   ReadSslLineMsg(DISCONNECT)");
      trace_event(sLine, PROCESSING);
      return (OR_DISCONECT);
   }

   /*Start BRO06122010: PCI*/
   if (debug_option == 1)
        dump_buffer ( sBuffRcv, nLenMsg , 'A', 'L', 'I' );
   else
        EncryptBuff(sBuffRcv,nLenMsg,sDest);
   /*End BRO06122010: PCI*/
   sprintf(sLenMsg,"%.4s", sBuffRcv);
   memcpy( sBuffer+4, sBuffRcv , nLenMsg );
   *nLength = nLenMsg+4;

   sprintf(sLine, "End   ReadSslLineMsg(%d)", *nLength);
   trace_event(sLine, PROCESSING);

   return (OK);
}
