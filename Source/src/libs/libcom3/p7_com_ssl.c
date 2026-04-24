#include <p7_common.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <p7_types.h>
#include <p7_com_prop.h>
#include <p7_com_ssl.h>

#if defined(WIN32) && !defined(INET_ADDRSTRLEN)
	#define INET_ADDRSTRLEN 16
#endif

/******************************************************************************/
void InitCltSSLComInfo(TSCtlSSLComInfo* pkCtlSSLComInfo)
{
	pkCtlSSLComInfo->eStatus = CLT_CONN_FREE;
	pkCtlSSLComInfo->conn_bio = NULL;
	pkCtlSSLComInfo->bConnected = NOK;
	memset(pkCtlSSLComInfo->szCltAddress,'\0',sizeof(pkCtlSSLComInfo->szCltAddress));
}
/******************************************************************************/
void InitSSLComInfo(TSSSLComInfo* pkSSLComInfo)
{
	pkSSLComInfo->nNbConn		= 0;
	pkSSLComInfo->ComInfoProperties = NULL;
	pkSSLComInfo->tab_conn		= NULL;
	pkSSLComInfo->ctx			= NULL;
	pkSSLComInfo->ssl			= NULL;
	pkSSLComInfo->root_bio		= NULL; 
	pkSSLComInfo->accept_bio	= NULL;
	pkSSLComInfo->buffer_bio	= NULL;
	memset(pkSSLComInfo->szHostAddr,'\0',sizeof(pkSSLComInfo->szHostAddr));
	pkSSLComInfo->nPortNbr = 0;
	memset(pkSSLComInfo->szCertificateFile,'\0',sizeof(pkSSLComInfo->szCertificateFile));
	memset(pkSSLComInfo->szPrivateKeyFile,'\0',sizeof(pkSSLComInfo->szPrivateKeyFile));
	memset(pkSSLComInfo->szCA,'\0',sizeof(pkSSLComInfo->szCA));
}
/******************************************************************************/
/*Start 22112022 - PLUTONL-4231*/
int SSL_StopCurrentCltCom(BIO *root_bio)
{
    char  sLine[ MAX_LINE_TRC ];
    int		fd;

    trace_event ("Start SSL_StopCurrentCltCom()" , PROCESSING);

    if (root_bio && BIO_get_fd(root_bio, &fd) != -1)
    {
        sprintf(sLine,"Closing fd(%d)",fd);
        trace_event (sLine , PROCESSING);

        shutdown (  fd  , 2 );
        if (close(fd) == NOK)
        {
            sprintf(sLine, "SHUTDOWN ERROR : errno = %d ", errno);
            trace_event(sLine, ERROR);
        }
    }
    if (root_bio)
	{		
		BIO_free(root_bio);
	}
	
	sprintf(sLine,"End SSL_StopCurrentCltCom(OK)");
    trace_event (sLine , PROCESSING);
    
	return OK;

}
/*End 22112022 - PLUTONL-4231*/
/******************************************************************************/
int SSL_InitCltCom(TSSSLComInfo*	pkSSLComInfo)
{

	struct hostent				*hp;
	struct sockaddr_in			sAddrServ;
	int							nFd;
	char						sBuffer[512];
	X509*						server_cert;
	long						verif_res;

	if( pkSSLComInfo->ComInfoProperties->eConnMode != E_CM_CLIENT )
	{
		trace_message(P7_TL_FATAL, "Entity is not a client...");
		return NOK;
	}
	if( pkSSLComInfo->ComInfoProperties->nTotalLengthLen < pkSSLComInfo->ComInfoProperties->nLengthLen )
	{
		trace_message(P7_TL_FATAL, "Invalid length len VS total length len values %d , %d",pkSSLComInfo->ComInfoProperties->nLengthLen,pkSSLComInfo->ComInfoProperties->nTotalLengthLen);
		return NOK;
	}
	if( pkSSLComInfo->ComInfoProperties->nLengthOffset > 0 && pkSSLComInfo->ComInfoProperties->bExcludeLength != LDT_LENGTH_INCLUDED )
	{
		trace_message(P7_TL_FATAL, "Invalid length Length offset VS LengthExcluded");
		return NOK;
	}
	
	

	pkSSLComInfo->ctx = SSL_CTX_new(SSLv23_client_method());
	if( !pkSSLComInfo->ctx )
	{
		trace_message(P7_TL_FATAL,"Error in SSL_CTX_new()");
		ERR_print_errors_fp(stderr);
		return NOK;
	}

	SSL_CTX_set_options(pkSSLComInfo->ctx, SSL_OP_NO_SSLv2);

	if( strlen(pkSSLComInfo->szCertificateFile) > 0 )
	{
		if (!SSL_CTX_use_certificate_file(pkSSLComInfo->ctx,pkSSLComInfo->szCertificateFile,SSL_FILETYPE_PEM)) 
		{
			trace_message(P7_TL_FATAL,"Error in SSL_CTX_use_certificate_file()");
			ERR_print_errors_fp(stderr);
			return NOK;
		}
	
		if( strlen(pkSSLComInfo->szPrivateKeyFile) <= 0 )
		{
			trace_event("End SSL_InitCltCom(NOK): Missing Private key file",ERROR);
			return NOK;
		}

		if (!SSL_CTX_use_PrivateKey_file(pkSSLComInfo->ctx,pkSSLComInfo->szPrivateKeyFile,SSL_FILETYPE_PEM)) 
		{
			trace_message(P7_TL_FATAL,"Error in SSL_CTX_use_PrivateKey_file()");
			ERR_print_errors_fp(stderr);
			return NOK;
		}
		if (!SSL_CTX_check_private_key(pkSSLComInfo->ctx)) 
		{
			trace_message(P7_TL_FATAL,"Error in SSL_CTX_check_private_key()");
			ERR_print_errors_fp(stderr);
			return NOK;
		}
	}



	pkSSLComInfo->root_bio = BIO_new_ssl_connect(pkSSLComInfo->ctx);
	BIO_get_ssl(pkSSLComInfo->root_bio, &pkSSLComInfo->ssl);
	if(!pkSSLComInfo->ssl) 
	{
		SSL_StopCurrentCltCom(pkSSLComInfo->root_bio); /*22112022 - PLUTONL-4231*/
		trace_message(P7_TL_FATAL,"Error in BIO_get_ssl()");
		return NOK;
	}

	
	SSL_CTX_set_verify(pkSSLComInfo->ctx,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,0);
	if( pkSSLComInfo->szCA )
	{
		SSL_CTX_load_verify_locations(pkSSLComInfo->ctx,pkSSLComInfo->szCA,NULL);
	}

	SSL_set_mode(pkSSLComInfo->ssl, SSL_MODE_AUTO_RETRY);
	sprintf(sBuffer,"%s:%d",pkSSLComInfo->szHostAddr,pkSSLComInfo->nPortNbr);
	BIO_set_conn_hostname(pkSSLComInfo->root_bio, sBuffer);

	if (BIO_do_connect(pkSSLComInfo->root_bio) <= 0) {
		SSL_StopCurrentCltCom(pkSSLComInfo->root_bio); /*22112022 - PLUTONL-4231*/
		fprintf(stderr, "Error connecting to server\n");
		ERR_print_errors_fp(stderr);
		return NOK;
	}

	if( pkSSLComInfo->nVerifyPeer == TLS_SRV_VERIF_CLT_YES )
	{
		server_cert = SSL_get_peer_certificate (pkSSLComInfo->ssl);
		if( server_cert == NULL )
		{
			SSL_StopCurrentCltCom(pkSSLComInfo->root_bio); /*22112022 - PLUTONL-4231*/
			fprintf(stderr, "No Certificate present\n");
			return NOK;
		}

		verif_res =  SSL_get_verify_result(pkSSLComInfo->ssl);
		if( verif_res != X509_V_OK )
		{
			SSL_StopCurrentCltCom(pkSSLComInfo->root_bio); /*22112022 - PLUTONL-4231*/
			fprintf(stderr, "Certificate verification failure\n");
			X509_free (server_cert); /*04122022 - PLUTONL-4231*/
			return NOK;
		}

		X509_free (server_cert);
	}

	if (BIO_do_handshake(pkSSLComInfo->root_bio) <= 0) 
	{
		SSL_StopCurrentCltCom(pkSSLComInfo->root_bio); /*22112022 - PLUTONL-4231*/
		fprintf(stderr, "Error establishing SSL connection\n");
		ERR_print_errors_fp(stderr);
		return NOK;
	}



	return OK;
}
/******************************************************************************/
int SSL_InitSrvCom(TSSSLComInfo*	pkSSLComInfo)
{

	char		szPort[24];
	int			nIndex;

	memset(szPort,'\0',sizeof(szPort));

	nIndex = 0;

	if( pkSSLComInfo->ComInfoProperties->eConnMode != E_CM_SERVER )
	{
		trace_message(P7_TL_FATAL, "Entity is not a server...");
		return NOK;
	}
	if( pkSSLComInfo->ComInfoProperties->nTotalLengthLen < pkSSLComInfo->ComInfoProperties->nLengthLen )
	{
		trace_message(P7_TL_FATAL, "Invalid length len VS total length len values %d , %d",pkSSLComInfo->ComInfoProperties->nLengthLen,pkSSLComInfo->ComInfoProperties->nTotalLengthLen);
		return NOK;
	}
	if( pkSSLComInfo->ComInfoProperties->nLengthOffset > 0 && pkSSLComInfo->ComInfoProperties->bExcludeLength != LDT_LENGTH_INCLUDED )
	{
		trace_message(P7_TL_FATAL, "Invalid length Length offset VS LengthExcluded");
		return NOK;
	}
	
	/*TODO: Seed PRNG */
	pkSSLComInfo->ctx = SSL_CTX_new(SSLv23_server_method());
	if( !pkSSLComInfo->ctx )
	{
		trace_message(P7_TL_FATAL,"Error in SSL_CTX_new()");
		ERR_print_errors_fp(stderr);
		return NOK;
	}

	if( strlen(pkSSLComInfo->szCertificateFile) > 0 )
	{
		if (!SSL_CTX_use_certificate_file(pkSSLComInfo->ctx,pkSSLComInfo->szCertificateFile,SSL_FILETYPE_PEM)) 
		{
			trace_message(P7_TL_FATAL,"Error in SSL_CTX_use_certificate_file()");
			ERR_print_errors_fp(stderr);
			return NOK;
		}

		if( strlen(pkSSLComInfo->szPrivateKeyFile) <= 0 )
		{
			trace_event("End SSL_InitSrvCom(NOK): Missing Private key file",ERROR);
			return NOK;
		}

		if (!SSL_CTX_use_PrivateKey_file(pkSSLComInfo->ctx,pkSSLComInfo->szPrivateKeyFile,SSL_FILETYPE_PEM)) 
		{
			trace_message(P7_TL_FATAL,"Error in SSL_CTX_use_PrivateKey_file()");
			ERR_print_errors_fp(stderr);
			return NOK;
		}
		if (!SSL_CTX_check_private_key(pkSSLComInfo->ctx)) 
		{
			trace_message(P7_TL_FATAL,"Error in SSL_CTX_check_private_key()");
			ERR_print_errors_fp(stderr);
			return NOK;
		}
	}

	if( pkSSLComInfo->nVerifyPeer == TLS_SRV_VERIF_CLT_YES )
	{
		SSL_CTX_set_verify(pkSSLComInfo->ctx,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,0);
		if( strlen(pkSSLComInfo->szCA) <= 0 )
		{
			trace_event("End InitSrvTLSComms(NOK): Missing CA Certificate",ERROR);
			return NOK;
		}
		SSL_CTX_load_verify_locations(pkSSLComInfo->ctx,pkSSLComInfo->szCA,NULL);
	}

	pkSSLComInfo->root_bio = BIO_new_ssl(pkSSLComInfo->ctx,0);
	BIO_get_ssl(pkSSLComInfo->root_bio, &pkSSLComInfo->ssl);
	if(!pkSSLComInfo->ssl) 
	{
		trace_message(P7_TL_FATAL,"Error in BIO_get_ssl()");
		return NOK;
	}

	SSL_set_mode(pkSSLComInfo->ssl, SSL_MODE_AUTO_RETRY);

	pkSSLComInfo->buffer_bio = BIO_new(BIO_f_buffer());
	pkSSLComInfo->root_bio = BIO_push(pkSSLComInfo->buffer_bio, pkSSLComInfo->root_bio);

	sprintf(szPort,"%u",pkSSLComInfo->nPortNbr);
	pkSSLComInfo->accept_bio=BIO_new_accept(szPort);

	BIO_set_accept_bios(pkSSLComInfo->accept_bio,pkSSLComInfo->root_bio);

	BIO_set_bind_mode(pkSSLComInfo->accept_bio, BIO_BIND_REUSEADDR);

	if(BIO_do_accept(pkSSLComInfo->accept_bio) <= 0) 
	{
		trace_message(P7_TL_FATAL,"Error in BIO_do_accept(): Setup");
		ERR_print_errors_fp(stderr);
		return NOK;
	}


	trace_message(P7_TL_PROCESSING,"End InitSrvCommsSSL(OK)");
	return OK;
}
/******************************************************************************/
int SSL_AcceptCallSrv(TSSSLComInfo*	pkSSLComInfo, int* nClientConnIndex)
{
	int					nLocalFd;
	int					fd,i;
	struct				sockaddr_in client_addr;
	int					nLen;
	unsigned char		addr[4];
	int					offset;
	BIO*				conn_bio;
	int					nIndex;
	char				szCallingAddress[24];
	int					nClientIndex;


	memset(szCallingAddress,'\0',sizeof(szCallingAddress));

	trace_message(P7_TL_PROCESSING,"Start AcceptCallSrvSSL()");

	if( pkSSLComInfo->ComInfoProperties->eConnMode != E_CM_SERVER )
	{
		trace_message(P7_TL_FATAL, "Entity is not a server...");
		return NOK;
	}

	nIndex = 0;


	if(BIO_do_accept(pkSSLComInfo->accept_bio) <= 0) 
	{
		trace_message(P7_TL_FATAL,"Error in BIO_do_accept(): Connection");
		ERR_print_errors_fp(stderr);
		trace_message(P7_TL_FATAL,"End AcceptCallSrvSSL(NOK)");
		return NOK;
	}



	conn_bio= BIO_pop(pkSSLComInfo->accept_bio);

	

	

	if (BIO_get_fd(conn_bio, &fd) == -1)
	{
		trace_message(P7_TL_FATAL,"Error in BIO_get_fd()");
		trace_message(P7_TL_FATAL,"End AcceptCallSrvSSL(NOK)");
		return NOK;
	}

	nLen = sizeof(struct	sockaddr_in);
	getpeername(fd,(struct sockaddr*)&client_addr, &nLen);

	inet_ntop( AF_INET, &client_addr.sin_addr.s_addr, szCallingAddress, INET_ADDRSTRLEN );


	nClientIndex = -1;
	for(i=0; i < pkSSLComInfo->nNbConn; i++ )
	{
		if( strcmp(szCallingAddress,pkSSLComInfo->tab_conn[i].szCltAddress) == 0 )
		{
			nClientIndex = i;
			break;
		}
	}
	if( nClientIndex >= 0 && pkSSLComInfo->ComInfoProperties->bMultiConnPerClient == NOK)
	{

		SSL_StopCom(pkSSLComInfo,nClientIndex);
	}
	else if( pkSSLComInfo->nNbConn >= pkSSLComInfo->ComInfoProperties->nMaxClients )
	{
		trace_message(P7_TL_FATAL, "MAX clients reached(%d)", pkSSLComInfo->nNbConn);
		return NOK;
	}
	else
	{
		nClientIndex = pkSSLComInfo->nNbConn++;
	}

	(*nClientConnIndex) = nClientIndex;



	memcpy(	pkSSLComInfo->tab_conn[nClientIndex].szCltAddress,
		szCallingAddress,
		sizeof(pkSSLComInfo->tab_conn[nClientIndex].szCltAddress));
	pkSSLComInfo->tab_conn[nClientIndex].eStatus = CLT_CONN_ONLINE;

	pkSSLComInfo->tab_conn[nClientIndex].conn_bio= conn_bio;


	trace_message(P7_TL_PROCESSING,"End AcceptCallSrvSSL(OK)");

	return nLocalFd;
}
/******************************************************************************/
void SSL_StopCom(TSSSLComInfo*	pkSSLComInfo, int nClientConnIndex)
{

	char  sLine[ MAX_LINE_TRC ];
	int		fd;

	
	sprintf(sLine,"Start StopCltCom(%d)",nClientConnIndex);
	trace_event (sLine , PROCESSING); 


	if( pkSSLComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		if (BIO_get_fd(pkSSLComInfo->tab_conn[nClientConnIndex].conn_bio, &fd) != -1)
		{
			sprintf(sLine,"Closing fd(%d)",fd);
			trace_event (sLine , PROCESSING); 

			shutdown (  fd  , 2 );
			if (close(fd) == NOK) 
			{
				sprintf(sLine, "SHUTDOWN ERROR : errno = %d ", errno);
				trace_event(sLine, ERROR);
			}
		}
		BIO_free_all(pkSSLComInfo->tab_conn[nClientConnIndex].conn_bio);
		pkSSLComInfo->tab_conn[nClientConnIndex].eStatus=CLT_CONN_FREE;
	}
	else
	{
		if (BIO_get_fd(pkSSLComInfo->root_bio, &fd) != -1)
		{
			sprintf(sLine,"Closing fd(%d)",fd);
			trace_event (sLine , PROCESSING); 

			shutdown (  fd  , 2 );
			if (close(fd) == NOK) 
			{
				sprintf(sLine, "SHUTDOWN ERROR : errno = %d ", errno);
				trace_event(sLine, ERROR);
			}
		}
		BIO_flush(pkSSLComInfo->root_bio);
		BIO_free_all(pkSSLComInfo->root_bio);
	}

}
/******************************************************************************/
void SSL_StopSrvCom(TSSSLComInfo*	pkSSLComInfo)
{
	int		fd;
	char	sLine[ MAX_LINE_TRC ];


	if (BIO_get_fd(pkSSLComInfo->root_bio, &fd) != -1)
	{
		sprintf(sLine,"Closing fd(%d)",fd);
		trace_event (sLine , PROCESSING); 

		shutdown (  fd  , 2 );
		if (close(fd) == NOK) 
		{
			sprintf(sLine, "SHUTDOWN ERROR : errno = %d ", errno);
			trace_event(sLine, ERROR);
		}
	}
	BIO_flush(pkSSLComInfo->root_bio);
	BIO_free_all(pkSSLComInfo->root_bio);

}
/******************************************************************************/
int SSL_readcheck(TSSSLComInfo*	pkSSLComInfo, int nConnIndex, int nTimer)
{

	int			nClientIndex;
	BIO*		conn_bio;
	int			nFd;
	int			nResult;
	fd_set		rfds ;
    struct		timeval tv;

	if( pkSSLComInfo->ComInfoProperties->eConnMode == E_CM_SERVER )
	{
		nClientIndex = nConnIndex;
		conn_bio = pkSSLComInfo->tab_conn[nClientIndex].conn_bio;
	}
	else
	{
		conn_bio = pkSSLComInfo->root_bio;
	}

	if (BIO_get_fd(conn_bio, &nFd) < 0)
	{
		trace_message(P7_TL_PROCESSING,"Error BIO_get_fd()");
		return NOK;
	}


	if( nFd < 0 )		return -1 ;             /* bad socket id */
	if( nTimer <= 0 )	return -2 ;             /* bad timer */

	FD_ZERO(&rfds); FD_SET(nFd , &rfds) ;		/* check only sd */

	tv.tv_sec = nTimer ;                         /* set timer */
	tv.tv_usec = 0 ;                         /* set timer */

	nResult = select(nFd+1, &rfds, NULL, NULL, &tv ) ;  /* check it */

	if( nResult == -1 )
	{
		/*trace_event ("tcp_readcheck ERROR / select() returned -1" , PROCESSING); */
		return (NOK);
	}

	if( nResult == 0 )
	{
		/*trace_event ("tcp_readcheck TIME OUT/ select() returned 0" , PROCESSING); */
		return (NOK);
	}

	return OK;
}
/******************************************************************************/

