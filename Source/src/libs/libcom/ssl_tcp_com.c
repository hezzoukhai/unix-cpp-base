
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_com.c                                                              */
/*                                                                             */
/* Description                                                                 */
/*      SSL TCP-IP Tools Management                                                */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id                     Date                    Author                          Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001                   18-02-2002              Anass OUNAINI                 Initiale Version
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <errno.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#include <define.h>
#include <tcp_var.h>
#include <ssl_var.h>
#include <fcntl.h>
#include <globals.h>
#include <resources.h>

/*
SSL_CTX              				*SslCtx;
static SSL                  		*nSsl;
static SSL_METHOD           		*SslMeth;
*/

 
void openssl_error(SSL *ssl,int r)
{
    char  sLine[MAX_LINE_TRC];
    int er;
	memset (sLine, 0, sizeof(sLine));

	trace_event("Start Getting the Ssl Error : ",ERROR);
    switch ((er = SSL_get_error(ssl, r))) 
    {
			case SSL_ERROR_NONE:
				sprintf(sLine,"SSL_ERROR_NONE [%d] ", er);
				trace_event(sLine, ERROR);
			break;
			case SSL_ERROR_ZERO_RETURN:
				sprintf(sLine,"SSL_ERROR_ZERO_RETURN: SSL connection closed [%d] ", er);			
				trace_event(sLine, ERROR);
			break;
			case SSL_ERROR_WANT_READ:
				sprintf(sLine,"SSL_ERROR_WANT_READ: operation could not complete -- retry [%d] ", er);
				trace_event(sLine, ERROR);
			break;
			case SSL_ERROR_WANT_WRITE:
				sprintf(sLine,"SSL_ERROR_WANT_WRITE: operation could not complete -- retry [%d] ", er);
				trace_event(sLine, ERROR);
			break;
			case SSL_ERROR_WANT_CONNECT:
				sprintf(sLine,"SSL_ERROR_WANT_CONNECT: operation could not complete (not connected) [%d] ", er);
				trace_event(sLine, ERROR);
			break;
			case SSL_ERROR_WANT_ACCEPT:
				sprintf(sLine,"SSL_ERROR_WANT_ACCEPT:  operation could not complete (not accepted) [%d] ", er);
				trace_event(sLine, ERROR);
			break;
			case SSL_ERROR_WANT_X509_LOOKUP:
				sprintf(sLine,"SSL_ERROR_WANT_X509_LOOKUP:  [%d] ", er);
				trace_event(sLine, ERROR);
			break;
			case SSL_ERROR_SYSCALL:
				sprintf(sLine,"SSL_ERROR_SYSCALL: [%d] ", er);
				trace_event(sLine, ERROR);
			break;
			case SSL_ERROR_SSL:
				sprintf(sLine,"SSL_ERROR_SSL: [%d] ", er);
				trace_event(sLine, ERROR);
			break;
			default:
				sprintf(sLine,"unknown SSL error [%d] ", er);
				trace_event(sLine, ERROR);
			break;
    }
		trace_event("End Getting the Ssl Error ",ERROR);
}

int InitSslCltComms()
{
   
   return ( InitSslCltCom( RemoteAppName , RemoteAppId ) );
}

int is_valid_fd(int fd)
{
    return fcntl(fd, F_GETFL) != -1 || errno != EBADF;
}	

int InitSslCltCom(char *sServerName, char *sPort)
{
   char                         sLine[MAX_LINE_TRC];
   int                          sock;
   int                          verify_client = NOK; /* To verify a client certificate, set ON */
   char                         *str;
   SSL_CTX                      *ctx;
   SSL                          *ssl;
   SSL_METHOD                   *meth;
   X509                         *server_cert;
   EVP_PKEY                     *pkey;
   char                         *sPtr;
   struct hostent               *hp;
   struct sockaddr_in           sAddrServ;
   struct servent               *serveur;
   unsigned                     nPort;
   char                         sRsaCltCert   [500 +1];
   char                         sRsaCltKey    [500 +1];
   char                         sRsaCltCaCert [500 +1];
   int				result;
   int				ret;
   int				Len;
   long				ssl_opt;
   

   memset(sLine, 		  0, sizeof(sLine));
   memset(sRsaCltCert   , 0, sizeof(sRsaCltCert   ));
   memset(sRsaCltKey    , 0, sizeof(sRsaCltKey    ));
   memset(sRsaCltCaCert , 0, sizeof(sRsaCltCaCert ));
   
   sPtr = getenv("HOME");
   
   Len  = strlen(sPtr);
   if (Len == 0)
   {
		trace_event("Error getenv HOME",PROCESSING);
		return(NOK);
   }
   
   memcpy( sRsaCltCert,   sPtr,  Len);
   memcpy( sRsaCltKey,    sPtr,  Len);
   memcpy( sRsaCltCaCert, sPtr,  Len);
   
   memcpy( sRsaCltCert+Len,   RSA_CLIENT_CERT,    strlen(RSA_CLIENT_CERT));
   memcpy( sRsaCltKey+Len,    RSA_CLIENT_KEY,     strlen(RSA_CLIENT_KEY));
   memcpy( sRsaCltCaCert+Len, RSA_CLIENT_CA_CERT, strlen(RSA_CLIENT_CA_CERT));
   

		
		
		
		
   nPort = atoi ( sPort );

   sprintf(sLine, "Start InitSslCltCom(%s, %d)", sServerName, nPort);
   trace_event(sLine,  PROCESSING);
   
   
   sprintf(sLine, "sRsaCltCaCert(%s)", sRsaCltCaCert);
   trace_event(sLine,  PROCESSING);


   
   /* Load encryption & hashing algorithms for the SSL program */
   SSL_library_init();
   /* Load the error strings for SSL & CRYPTO APIs */
   SSL_load_error_strings();

   /* Create an SSL_METHOD structure (choose an SSL/TLS protocol version) */
   SslMeth = SSLv23_client_method();




   /* Create an SSL_CTX structure */
   SslCtx = SSL_CTX_new(SslMeth);

   #ifdef SSL_OP_NO_TLSv1_1
		ssl_opt = SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3|SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1;
   #else
		ssl_opt = SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3|SSL_OP_NO_TLSv1 | 1<<3;
   #endif
  
   ssl_opt =  SSL_CTX_set_options (SslCtx, ssl_opt);

   /*Set cipher list*/
   if (SSL_CTX_set_cipher_list(SslCtx,CIPHER_LIST) <= 0) 
   {
	trace_event("Error setting the cipher list",PROCESSING);
	return(NOK);
    }

   /*----------------------------------------------------------*/
   if(verify_client == OK)
   {
        /* Load the client certificate into the SSL_CTX structure */
        if (SSL_CTX_use_certificate_file(SslCtx, sRsaCltCert,SSL_FILETYPE_PEM) <= 0) {
   		   trace_event("SSL_CTX_use_certificate_file NOK",  PROCESSING);
                   return(NOK);
        }

        /* Load the private-key corresponding to the client certificate */
        if (SSL_CTX_use_PrivateKey_file(SslCtx, sRsaCltKey,SSL_FILETYPE_PEM) <= 0) {
                     trace_event("SSL_CTX_use_PrivateKey_file NOK",  PROCESSING);
		     return(NOK);
        }
        /* Check if the client certificate and private-key matches */
        if (!SSL_CTX_check_private_key(SslCtx)) {
		trace_event("Private key does not match the certificate public key" ,  PROCESSING);
                return(NOK);
        }
   }

   /* Load the RSA CA certificate into the SSL_CTX structure */
   /* This will allow this client to verify the server's     */
   /* certificate.                                           */
   if (SSL_CTX_load_verify_locations(SslCtx, sRsaCltCaCert, NULL)<1) {
		ERR_print_errors_fp(stderr);
		sprintf(sLine, "Load the RSA CA certificate failed: %s errno:%d  ", sRsaCltCaCert, errno);
		trace_event(sLine ,  PROCESSING);
                return(NOK);
   }

   /* Set flag in context to require peer (server) certificate */
   /* verification */

   SSL_CTX_set_verify(SslCtx,SSL_VERIFY_PEER,NULL);
   SSL_CTX_set_verify_depth(SslCtx,1);

   trace_event("New SSl Creation", PROCESSING);
   nSsl = SSL_new (SslCtx);

   /* creation et attachement du socket en local                       */
   if ( (nFdConnect = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      sprintf(sLine, "SOCKET ERROR : errno = %d ", errno);
      trace_event(sLine, ERROR);
      trace_event("End   InitSslCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   if ((hp=gethostbyname(sServerName)) == NULL)
   {
      sprintf(sLine,"Machine %s unknown (See Config /etc/hosts)", sServerName);
      trace_event(sLine, ERROR);
      trace_event("End   InitSslCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   /******
   serveur=getservbyname(sServiceName, "tcp");
   if (serveur == NULL)
   {
      sprintf(sLine, "Service name %.16s not found in /etc/services", sServiceName);
      trace_event(sLine, ERROR);
      trace_event("End   InitSslCltCom(NOK)", PROCESSING);
      return(NOK);
   }
   ***/

   sAddrServ.sin_family = AF_INET;
#ifdef INTEL
   sAddrServ.sin_port   = serveur->s_port;
#else
   /****
   sAddrServ.sin_port   = htons(serveur->s_port);
   ****/
   sAddrServ.sin_port   = htons(nPort);
#endif
   memcpy(&sAddrServ.sin_addr.s_addr, hp->h_addr, hp->h_length);

   if (connect(nFdConnect,(struct sockaddr*)&sAddrServ, sizeof(struct sockaddr)) == -1)
   {
      sprintf(sLine, "CONNECT ERROR : errno = %d ", errno);
      trace_event(sLine, ERROR);
      trace_event("End   InitSslCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   /* Assign the socket into the SSL structure (SSL and socket without BIO) */
   trace_event("start SSL_set_fd ", PROCESSING);
   SSL_set_fd(nSsl, nFdConnect);

   /* Perform SSL Handshake on the SSL client */
   trace_event("start SSL_connect ", PROCESSING);
   result = SSL_connect(nSsl); 
   if (result<1)
   {
      ret = SSL_get_error(nSsl,result);
      sprintf(sLine, "SSL CONNECT ERROR : result = %d, errno = %d , SSL_get_errors= %d", result, errno, ret);
      trace_event(sLine, ERROR);
      trace_event("End   InitSslCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   /* Informational output (optional) */
   sprintf(sLine, "SSL connection using %s ", SSL_get_cipher (nSsl));
   trace_event(sLine, ERROR);

   /* Get the server's certificate (optional) */
   server_cert = SSL_get_peer_certificate (nSsl);

   if (server_cert != NULL)
   {
        printf ("Server certificate:\n");
        str = X509_NAME_oneline(X509_get_subject_name(server_cert),0,0);
        printf ("\t subject: %s\n", str);
        free (str);

        str = X509_NAME_oneline(X509_get_issuer_name(server_cert),0,0);
        printf ("\t issuer: %s\n", str);
        free(str);

        X509_free (server_cert);


   }
   else
   trace_event("The SSL server does not have certificate",PROCESSING);


   trace_event("End   InitSslCltCom(OK)", PROCESSING);
   return (OK);
}

int InitSslSrvComms(int nPort)
{
   char                 sLine[MAX_LINE_TRC];
   char                 sRsaServCaCert[500 +1];
   int                  sDesc;
   struct               sockaddr_in adr_local;
   unsigned int         Longueur = sizeof(struct sockaddr_in); 
   int                  retour;
   struct servent       *serveur;
   char					*sPtr;
   int                  on;
   int                  Len;
   long                  ssl_opt;
   int                  verify_client = NOK; /* To verify a client certificate, set ON */
   SSL_METHOD           *meth;
   
   
   memset(sLine, 0, sizeof(sLine));
   memset(sRsaServCaCert, 0, sizeof(sRsaServCaCert));
   
   
   sprintf(sLine, "Start InitSslSrvComms(%d)", nPort);
   trace_event(sLine, PROCESSING);


   sPtr = getenv("HOME");
   
   Len  = strlen(sPtr);
   if (Len == 0)
   {
		trace_event("Error getenv HOME",PROCESSING);
		return(NOK);
   }
   

   memcpy( sRsaServCaCert, sPtr,  Len);

   memcpy( sRsaServCaCert+Len, RSA_SERVER_CA_CERT, strlen(RSA_SERVER_CA_CERT));
   
   /*----------------------------------------------------------------*/
   /* Load encryption & hashing algorithms for the SSL program */
   SSL_library_init();
   trace_event("SSL_library_init", PROCESSING);

   /* Load the error strings for SSL & CRYPTO APIs */
   SSL_load_error_strings();
   trace_event("SSL_load_error_strings", PROCESSING);

   /* Create a SSL_METHOD structure (choose a SSL/TLS protocol version) */
   meth = SSLv23_server_method();
   trace_event("SSLv23_server_method", PROCESSING);

   /* Create a SSL_CTX structure */
   SslCtx = SSL_CTX_new(meth);
   
   
   #ifdef SSL_OP_NO_TLSv1_1
		ssl_opt = SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3|SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1;
   #else
		ssl_opt = SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3|SSL_OP_NO_TLSv1 | 1<<3;
   #endif
   
   ssl_opt =  SSL_CTX_set_options (SslCtx, ssl_opt);
   
   if (!SslCtx) 
   {
	printf(sLine, "Create a SSL_CTX structure failed, errno= %d ",errno);
        trace_event(sLine, ERROR);
        return(NOK);
   }

   /*Set cipher list*/
   /*if (SSL_CTX_set_cipher_list(SslCtx,CIPHER_LIST) <= 0) {
        trace_event("Error setting the cipher list");
        return(NOK);
   }*/

   /* Load the server certificate into the SSL_CTX structure */
   if (SSL_CTX_use_certificate_file(SslCtx, LocalAppName, SSL_FILETYPE_PEM) <= 0) {
	ERR_print_errors_fp(stderr);
        sprintf(sLine, "Load server certificate failed %s :errno= %d ", LocalAppName,errno);
        trace_event(sLine, ERROR);
	return(NOK);
   }

   /* Load the private-key corresponding to the server certificate */
   /*SSL_CTX_set_default_passwd_cb_userdata(SslCtx,"xyz1xyz!");*/
   if (SSL_CTX_use_PrivateKey_file(SslCtx, RemoteAppId, SSL_FILETYPE_PEM) <= 0) {
        sprintf(sLine, "InitSslSrvComms: Load the private-key corresponding to the server certificate failed %s : errno = %d ", RemoteAppId, errno);
        trace_event(sLine, ERROR);
        return(NOK);
   }

   /* Check if the server certificate and private-key matches */
   if (!SSL_CTX_check_private_key(SslCtx)) {
        sprintf(sLine,"InitSslSrvComms: Private key does not match the certificate public key : errno = %d", errno);
        trace_event(sLine,ERROR);
        return(NOK);
    }
    if(verify_client == OK)
    {
         /* Load the RSA CA certificate into the SSL_CTX structure */
         if (!SSL_CTX_load_verify_locations(SslCtx, sRsaServCaCert, NULL)) {
         sprintf(sLine,"InitSslSrvComms: Load the RSA CA certificate failed : errno = %d", errno);
         trace_event(sLine,ERROR);
         return(NOK);
         }

         /* Set to require peer (client) certificate verification */
         SSL_CTX_set_verify(SslCtx,SSL_VERIFY_PEER,NULL);

         /* Set the verification depth to 1 */
         SSL_CTX_set_verify_depth(SslCtx,1);

     }

	
        /* ----------------------------------------------- */
        /* Set up a TCP socket */

   if ( (sDesc = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      sprintf(sLine, "InitSslSrvComms: SOCKET ERROR : errno = %d ", errno);
      trace_event(sLine, ERROR);
      return(NOK);
   }

   adr_local.sin_family          = AF_INET;
   adr_local.sin_addr.s_addr     = htonl(INADDR_ANY);
   adr_local.sin_port            = htons(nPort);
   on=1 ;
   retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on));
   /* retour = setsockopt (sDesc, IPPROTO_TCP , SO_REUSEADDR, &on,sizeof(on)); */
   if(retour != 0)
   {
      sprintf( sLine,"Can not Use Socket Option SO_REUSEADDR %d|%d",retour,errno);
      trace_event(sLine,TRACE);
      sprintf( sLine,"End   open_device(NOK)");
      trace_event(sLine,TRACE);
      return(NOK);
   }
   /*retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEPORT, &on,sizeof(on)); AER 13/7/2002 */
   retour = setsockopt (sDesc, SOL_SOCKET, SO_REUSEADDR, &on,sizeof(on));
   if(retour != 0)
   {
      sprintf( sLine,"Can not Use Socket Option SO_REUSEPORT %d|%d",retour,errno);
      trace_event(sLine,TRACE);
      sprintf( sLine,"End   open_device(NOK)");
      trace_event(sLine,TRACE);
      return(NOK);
   }

/* Start BBO08112009 Add Keep Alive option */

   trace_event("Start  ADD KEEP ALIVE PARAMETER",TRACE);

   on=1 ;

   retour = setsockopt (sDesc, SOL_SOCKET, SO_KEEPALIVE, &on,sizeof(on));
   if(retour != 0)
   {
      sprintf( sLine,"Can not Use Socket Option SO_KEEPALIVE");
      trace_event(sLine,TRACE);
      sprintf( sLine,"End   InitSslSrvComms(NOK)");
      trace_event(sLine,ERROR);
      return(NOK);
   }

   trace_event("End ADD KEEP ALIVE PARAMETER",TRACE);

/* End BBO08112009 Add Keep Alive option */

   if (bind(sDesc, (struct sockaddr *)&adr_local, Longueur) == -1)
   {
      sprintf(sLine, "BIND FCT ERROR : errno = %d", errno);
      trace_event(sLine, ERROR);
      close(sDesc);
      trace_event("End   InitSslSrvComms(NOK)", PROCESSING);
      return(NOK);
   }
   
   if(&adr_local != NULL)
      getsockname(sDesc, (struct sockaddr *)&adr_local, &Longueur);

   retour = listen(sDesc, 5);
   if ( retour == -1 )
   {
      sprintf(sLine, "LISTEN ERROR : errno = %d \n", errno);
      trace_event(sLine, ERROR);
      trace_event("End  InitSslSrvComms(NOK)", PROCESSING);
      return(NOK);
   }

   nFdLocal = sDesc;
   trace_event("End   InitSslSrvCom(OK)", PROCESSING);
   return(OK);
}

int AcceptSslCallSrv(char calling_address[16], SSL *ssl)
{
   unsigned int                long_inet;
   struct sockaddr_in adr_client;
   char               sLine[MAX_LINE_TRC];
   int                nFdConn;
   unsigned char      adr[4];
   int                offset;
   int                retour;
   int                verify_client=NOK;
   X509               *client_cert = NULL;
   int                sslaccept_return;

   
    BIO                *sbio;
	

	memset(sLine, 0, sizeof(sLine));
	
   sprintf(sLine, "Start AcceptSslCallSrv()");
   trace_event(sLine, PROCESSING);
   
   long_inet = sizeof(struct sockaddr_in);
   nFdConn = accept(nFdLocal,
                    (struct sockaddr *)&adr_client,
                     &long_inet);
   if (nFdConn < 0 )
   {
      sprintf(sLine, "AcceptSslCallSrv: ACCEPT ERROR : errno = %d \n", errno);
      trace_event(sLine, ERROR);
      return(NOK);
   }

   memset(adr, 0, sizeof(adr));

   adr[0]= adr_client.sin_addr.s_addr % 256;
   adr[1]= (adr_client.sin_addr.s_addr >> 8 ) % 256;
   adr[2]= (adr_client.sin_addr.s_addr >> 16) % 256;
   adr[3]= (adr_client.sin_addr.s_addr >> 24) % 256;

   sprintf(sLine,"CALL Ad In: %u.%u.%u.%u",adr[3], adr[2], adr[1], adr[0]);
   trace_event(sLine, PROCESSING);

   offset = 0;
   sprintf(calling_address + offset, "%03d", adr[3]); offset += 3;
   calling_address[offset++]='.';
   sprintf(calling_address + offset, "%03d", adr[2]); offset += 3;
   calling_address[offset++]='.';
   sprintf(calling_address + offset, "%03d", adr[1]); offset += 3;
   calling_address[offset++]='.';
   sprintf(calling_address + offset, "%03d", adr[0]); offset += 3;

   /* -----------------------------------------------   */
   /* TCP connection is ready.                  	*/
  

   trace_event("Creating SSL structure", PROCESSING);

   /*ssl = SSL_new(SslCtx);*/
   if(ssl == NULL)
   {
	trace_event("Error creating SSL structure", PROCESSING);
	return(NOK);
   }
   
   trace_event("Assign the socket into the SSL structure (SSL and socket without BIO)", PROCESSING);
   SSL_set_fd(ssl, nFdConn);

   trace_event("Perform SSL Handshake on the SSL server", PROCESSING);
   sslaccept_return = SSL_accept(ssl);
   if(sslaccept_return <= 0 )
   {
		retour=SSL_get_error(ssl,sslaccept_return);
		sprintf(sLine,"SSL_Accept FAILED due to error %d sslaccept_return %d ",retour,sslaccept_return);
		trace_event(sLine, PROCESSING);
		return(NOK);
   }

	sprintf(sLine, "SSL_accept OK return (%d)", sslaccept_return);
   	trace_event(sLine, PROCESSING);


   /* Informational output (optional) */
   sprintf(sLine,"SSL connection using : %s",SSL_get_cipher (ssl));
   trace_event(sLine, PROCESSING);

   if (verify_client == OK)
   {
        trace_event("Verify client certificate", PROCESSING);

        /* Get the client's certificate (optional) */
        client_cert = SSL_get_peer_certificate(ssl);
        if (client_cert != NULL)
        {
                sprintf(sLine, "Client certificate (%s)", X509_NAME_oneline(X509_get_subject_name(client_cert), 0, 0));
                trace_event(sLine, PROCESSING);         
                X509_free(client_cert);
        }
        else
        {
                trace_event("The SSL client does not have certificate.", PROCESSING);
        }
  }


   sprintf(sLine, "End   AcceptSslCallSrv(%d)", SSL_get_fd(ssl));
   trace_event(sLine, PROCESSING);

   return(nFdConn);
}

int AcceptSslCall(char  calling_address [ 16], SSL *ssl)
{
   unsigned int		  long_inet;
   struct sockaddr_in adr_client;
   char               sLine[134];
   unsigned char      adr[4];
   int 				  offset;
   int                nFdConn;
   int                sslaccept_return;
   int                retour;


   sprintf(sLine, "Start AcceptSslCall()");
   trace_event(sLine, PROCESSING);

   long_inet = sizeof(struct sockaddr_in);
   nFdConnect = accept(nFdLocal,
                       (struct sockaddr *)&adr_client,
                       &long_inet);
   if (nFdConnect < 0 )
   {
      sprintf(sLine, "AcceptSslCall: ACCEPT ERROR : errno = %d \n", errno);
      trace_event(sLine, ERROR);
      return(NOK);
   }
	memset(adr, 0, sizeof(adr));

    adr[0]= adr_client.sin_addr.s_addr % 256;
    adr[1]= (adr_client.sin_addr.s_addr >> 8 ) % 256;
    adr[2]= (adr_client.sin_addr.s_addr >> 16) % 256;
    adr[3]= (adr_client.sin_addr.s_addr >> 24) % 256;

    sprintf(sLine,"CALL Ad In: %u.%u.%u.%u",
                adr[0], adr[1], adr[2], adr[3]);
     trace_event(sLine, PROCESSING);
     offset = 0;
     sprintf(calling_address + offset, "%03d", adr[3]); offset += 3;
     calling_address[offset++]='.';
     sprintf(calling_address + offset, "%03d", adr[2]); offset += 3;
     calling_address[offset++]='.';
     sprintf(calling_address + offset, "%03d", adr[1]); offset += 3;
     calling_address[offset++]='.';
     sprintf(calling_address + offset, "%03d", adr[0]); offset += 3;


   if(ssl == NULL)
   {
	trace_event("Error creating SSL structure", PROCESSING);
	return(NOK);
   }
   
	SSL_set_fd(ssl, nFdConn);
	
	sslaccept_return = SSL_accept(ssl);
	
   if(sslaccept_return <= 0 )
   {
		retour=SSL_get_error(ssl,sslaccept_return);
		sprintf(sLine,"SSL_Accept FAILED due to error %d sslaccept_return %d ",retour,sslaccept_return);
		trace_event(sLine, PROCESSING);
		return(NOK);
   }
   
   /* Informational output (optional) */
   sprintf(sLine,"SSL connection using : %s",SSL_get_cipher (ssl));
   trace_event(sLine, PROCESSING);
   
   sprintf(sLine, "End   AcceptSslCallSrv(%d)", SSL_get_fd(ssl));
   trace_event(sLine, PROCESSING);
   return(nFdConn);
}