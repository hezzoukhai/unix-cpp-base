#include <stdlib.h>
#include <string.h>
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

#include <define.h>
#include <p7_trace.h>


#include <openssl/bio.h>
#include <openssl/ssl.h>


#define CONN_TIMEOUT_PER		300

extern char LocalAppName[];

#define TLS_SRV_VERIF_CLT_NO		0
#define TLS_SRV_VERIF_CLT_YES		1

/*****************************************************************************/
/*TODO
* 1- S'assurer qu'on n'a pas de leak
*/
/*****************************************************************************/
typedef struct {

	unsigned char	addr[4];
	BIO *			conn_bio;
	int				free;
	int				fd;
} client_bio_t;
/*****************************************************************************/
static SSL_CTX *ctx;
static SSL *ssl;
static BIO *root_bio, *accept_bio, *buffer_bio;

static client_bio_t tab_bio[MAX_THREAD];

static int nNextFd=0;
static char szCertificateFile[1024];
static char szPrivateKeyFile[1024];
static char szCACertificateFile[1024];
static int gs_nVerifyPeer = TLS_SRV_VERIF_CLT_NO;

/*****************************************************************************/
int dump_tab_bio()
{
	int		i;
	int		nSize;
	char	sLine[MAX_LINE_TRC];

	nSize = sizeof(tab_bio)/sizeof(tab_bio[0]);
	for (i = 0; i < nSize; i++)
	{

		sprintf(sLine,"[i:%d][free:%d][fd:%d][addr:%c.%c.%c.%c]",
			i,
			tab_bio[i].free,
			tab_bio[i].fd,
			tab_bio[i].addr[0],
			tab_bio[i].addr[1],
			tab_bio[i].addr[2],
			tab_bio[i].addr[3]
			);
		trace_event(sLine,TRACE);
	}

	return OK;
}
/*****************************************************************************/
BIO* get_bio(int nConnDesc)
{
	if( nConnDesc < 0 || nConnDesc >= MAX_THREAD )
		return NULL;

	return tab_bio[nConnDesc].conn_bio;

}
/*****************************************************************************/
static int	GetFreeConnSlot()
{
	int		i;
	int		nSize;

	nSize = sizeof(tab_bio)/sizeof(tab_bio[0]);
	for( i=0; i < nSize; i++)
	{
		if( tab_bio[i].free == 1 )
			return i;
	}
	
	dump_tab_bio();

	return -1;
}
/*****************************************************************************/
static int GetClientSlot(unsigned char* addr)
{
	int i;
	int		nSize;

	nSize = sizeof(tab_bio)/sizeof(tab_bio[0]);
	for( i=0; i < nSize; i++)
	{
		if( memcmp(addr,tab_bio[i].addr,sizeof(tab_bio[i].addr)) == 0 )
			return i;
	}
	return -1;
}
/*****************************************************************************/
/*
static int CloseClientConn(int nSlotNbr)
{
	BIO_free_all(tab_bio[nSlotNbr].conn_bio);
	tab_bio[nSlotNbr].free = 1;
}*/
/*****************************************************************************/
void  StopAllComms()
{
	BIO_flush(root_bio);
	BIO_free_all(root_bio);
	/* Free error strings */
	/*ERR_free_strings();*/
	/* remove all ciphers and digests from the table */
	EVP_cleanup();
}
/*****************************************************************************/
void	InitTLSCom(char* szCertificatePath, char* szPrivateKeyPath, char* szCACertificatePath, int nVerifPeer)
{
	int		i;
	int		nSize;
	
	
	trace_event("Start InitTLSCom()",PROCESSING);
	trace_message(P7_TL_STREAM,"Certificate path: %.64s",szCertificatePath);
	trace_message(P7_TL_STREAM,"Private key path: %.64s",szPrivateKeyPath);
	trace_message(P7_TL_STREAM,"CA Certificate path: %.64s",szCACertificatePath);
	trace_message(P7_TL_STREAM,"Verify peer: %d",nVerifPeer);

	memset(szCertificateFile,'\0',sizeof(szCertificateFile));
	memset(szPrivateKeyFile,'\0',sizeof(szPrivateKeyFile));
	memset(szCACertificateFile,'\0',sizeof(szCACertificateFile));


	if( szCertificatePath && strlen(szCertificatePath) > 0 )
	{
		strncpy(szCertificateFile,szCertificatePath,sizeof(szCertificateFile)-1);
	}

	if( szPrivateKeyPath && strlen(szPrivateKeyPath) > 0 )
	{
		strncpy(szPrivateKeyFile,szPrivateKeyPath,sizeof(szPrivateKeyFile)-1);
	}

	if( szCACertificatePath && strlen(szCACertificatePath) > 0 )
	{
		strncpy(szCACertificateFile,szCACertificatePath,sizeof(szCACertificateFile)-1);
	}

	gs_nVerifyPeer = nVerifPeer;

	nSize = sizeof(tab_bio)/sizeof(tab_bio[0]);
	for( i=0; i < nSize; i++)
	{
		tab_bio[i].free=1;
	}

	SSL_library_init();
	/*Load libcrypto and libssl error strings for output*/
	SSL_load_error_strings(); 
	/*ERR_load_crypto_strings();
	ERR_load_SSL_strings();*/
	OpenSSL_add_all_algorithms();

	trace_event("End   InitTLSCom()",PROCESSING);
}
/*****************************************************************************/
/*Start 21112022 - PLUTONL-5394*/
int StopCurrentCltTLSCom()
{
    char  sLine[ MAX_LINE_TRC ];
    int		fd;

    trace_event ("Start StopCurrentCltTLSCom()" , PROCESSING);

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
	
    trace_event ("End StopCurrentCltTLSCom(OK)" , PROCESSING);
    
	return OK;

}
/*End 21112022 - PLUTONL-5394*/
/*****************************************************************************/
int InitCltTLSCom(char *sServerName, char *sPort)
{
	char						sLine[MAX_LINE_TRC];
	struct hostent				*hp;
	struct sockaddr_in			sAddrServ;
	int							fd;
	char						sBuffer[512];
	X509*						server_cert;
	long						verif_res;
	int							nLocalFd;
	
	trace_event("Start InitCltTLSCom()",PROCESSING);

	trace_message(P7_TL_STREAM,"Adress: %.64s, Port:%.64s",sServerName,sPort);
	
	
	ctx = SSL_CTX_new(TLSv1_2_client_method());
	if( !ctx )
	{
		
		ERR_print_errors_fp(stderr);
		trace_message(P7_TL_ERROR,"Error in SSL_CTX_new()");
		trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
		return -1;
	}

	SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

	if( strlen(szCertificateFile) > 0 )
	{
		if (!SSL_CTX_use_certificate_file(ctx,szCertificateFile,SSL_FILETYPE_PEM)) 
		{
			
			ERR_print_errors_fp(stderr);
			trace_message(P7_TL_ERROR,"Error in SSL_CTX_use_certificate_file(): [File: %.256s]",szCertificateFile);
			trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
			return -1;
		}

		if( strlen(szPrivateKeyFile) <= 0 )
		{
			trace_message(P7_TL_ERROR,"Missing Private key file");
			trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
			return -1;
		}

		if (!SSL_CTX_use_PrivateKey_file(ctx,szPrivateKeyFile,SSL_FILETYPE_PEM)) 
		{
			
			ERR_print_errors_fp(stderr);
			trace_message(P7_TL_ERROR,"Error in SSL_CTX_use_PrivateKey_file(): [File: %.256s]",szPrivateKeyFile);
			trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
			return -1;
		}

		if (!SSL_CTX_check_private_key(ctx)) 
		{
			ERR_print_errors_fp(stderr);
			trace_message(P7_TL_ERROR,"Error in SSL_CTX_check_private_key(): [File: %.256s]",szPrivateKeyFile);
			trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
			return -1;
		}
	}


	root_bio = BIO_new_ssl_connect(ctx);
	BIO_get_ssl(root_bio, &ssl);
	if(!ssl) 
	{
		StopCurrentCltTLSCom(); /*21112022 - PLUTONL-5394*/
		trace_message(P7_TL_ERROR,"Error in BIO_get_ssl()");
		trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
		return -1;
	}

	SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,0);
	if( strlen(szCACertificateFile) )
	{
		if(!SSL_CTX_load_verify_locations(ctx,szCACertificateFile,NULL))
		{
			trace_message(P7_TL_ERROR,"Error in SSL_CTX_load_verify_locations(): [File: %.256s]",szCACertificateFile);
			trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
			return -1;
		}
	}

	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

	sprintf(sBuffer,"%s:%s",sServerName,sPort);
	BIO_set_conn_hostname(root_bio, sBuffer);

	if (BIO_do_connect(root_bio) <= 0) 
	{
		StopCurrentCltTLSCom(); /*21112022 - PLUTONL-5394*/
		ERR_print_errors_fp(stderr);
		trace_message(P7_TL_ERROR,"Error connecting to server: [hostname:%.128s]",sBuffer);
		trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
		return -1;
	}


	if( gs_nVerifyPeer == TLS_SRV_VERIF_CLT_YES )
	{
		server_cert = SSL_get_peer_certificate (ssl);
		if( server_cert == NULL )
		{
			StopCurrentCltTLSCom(); /*21112022 - PLUTONL-5394*/
			trace_message(P7_TL_ERROR,"No Certificate presented by peer");
			trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
			return -1;
		}

		verif_res =  SSL_get_verify_result(ssl);
		if( verif_res != X509_V_OK )
		{
			StopCurrentCltTLSCom(); /*21112022 - PLUTONL-5394*/
			X509_free (server_cert);
			trace_message(P7_TL_ERROR,"Peer certificate verification failure");
			trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
			return -1;
		}

		X509_free (server_cert);
	}

	if (BIO_do_handshake(root_bio) <= 0) 
	{
		StopCurrentCltTLSCom(); /*21112022 - PLUTONL-5394*/
		ERR_print_errors_fp(stderr);
		trace_message(P7_TL_ERROR,"Handshare error on establishing TLS connection");
		trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
		return -1;
	}


	nLocalFd = GetFreeConnSlot();
	if( nLocalFd < 0 )
	{
		StopCurrentCltTLSCom(); /*21112022 - PLUTONL-5394*/
		trace_message(P7_TL_ERROR,"No Free Slot");
		trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
		return -1;
	}

	if (BIO_get_fd(root_bio, &fd) == -1)
	{
		StopCurrentCltTLSCom(); /*21112022 - PLUTONL-5394*/
		trace_message(P7_TL_ERROR,"Error in BIO_get_fd()");
		trace_message(P7_TL_INFO,"End InitCltTLSCom(NOK)");
		return -1;
	}

	tab_bio[nLocalFd].fd=fd;
	memcpy(tab_bio[nLocalFd].addr,sServerName,sizeof(tab_bio[nLocalFd].addr));
	tab_bio[nLocalFd].conn_bio= root_bio;
	tab_bio[nLocalFd].free = 0;


	trace_message(P7_TL_PROCESSING,"End   InitCltTLSCom(OK): [Index:%d,fd:%d,Host:%.128s]",nLocalFd,tab_bio[nLocalFd].fd,sServerName);
	return nLocalFd;
}
/*****************************************************************************/
int InitSrvTLSComms(int nPort)
{

	char	szPort[24];

	trace_message(P7_TL_PROCESSING,"Start InitSrvTLSComms(): [Port:%d]",nPort);

	memset(szPort,'\0',sizeof(szPort));

	

	/*TODO: Seed PRNG */
	ctx = SSL_CTX_new(TLSv1_2_server_method());
	if( !ctx )
	{
		ERR_print_errors_fp(stderr);
		trace_message(P7_TL_ERROR,"Error in SSL_CTX_new()");
		trace_message(P7_TL_INFO,"End   InitSrvTLSComms(NOK)");
		return NOK;
	}

	if( strlen(szCertificateFile) > 0 )
	{
		if (!SSL_CTX_use_certificate_file(ctx,szCertificateFile,SSL_FILETYPE_PEM)) 
		{
			
			ERR_print_errors_fp(stderr);
			trace_message(P7_TL_ERROR,"Error in SSL_CTX_use_certificate_file(): [File: %.128s]",szCertificateFile);
			trace_message(P7_TL_INFO,"End   InitSrvTLSComms(NOK)");
			return NOK;
		}

		if( strlen(szPrivateKeyFile) <= 0 )
		{
			trace_message(P7_TL_ERROR,"Missing Private key file");
			trace_message(P7_TL_INFO,"End   InitSrvTLSComms(NOK)");
			return NOK;
		}
		if (!SSL_CTX_use_PrivateKey_file(ctx,szPrivateKeyFile,SSL_FILETYPE_PEM)) 
		{
			
			ERR_print_errors_fp(stderr);
			trace_message(P7_TL_ERROR,"Error in SSL_CTX_use_PrivateKey_file(): [File: %.128s]",szPrivateKeyFile);
			trace_message(P7_TL_INFO,"End   InitSrvTLSComms(NOK)");
			return NOK;
		}

		if (!SSL_CTX_check_private_key(ctx)) 
		{
			ERR_print_errors_fp(stderr);
			trace_message(P7_TL_ERROR,"Error in SSL_CTX_check_private_key(): [File: %.128s]",szPrivateKeyFile);
			trace_message(P7_TL_INFO,"End   InitSrvTLSComms(NOK)");
			return NOK;
		}

	}


	if( gs_nVerifyPeer == TLS_SRV_VERIF_CLT_YES )
	{
		SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,0);
		if( strlen(szCACertificateFile) <= 0 )
		{
			trace_message(P7_TL_ERROR,"Missing CA Certificate");
			trace_message(P7_TL_INFO,"End   InitSrvTLSComms(NOK)");
			return NOK;
		}
		if(!SSL_CTX_load_verify_locations(ctx,szCACertificateFile,NULL))
		{
			trace_message(P7_TL_ERROR,"Error in SSL_CTX_load_verify_locations(): [File: %.256s]",szCACertificateFile);
			trace_message(P7_TL_INFO,"End   InitSrvTLSComms(NOK)");
			return NOK;
		}

	}

	root_bio=BIO_new_ssl(ctx,0);
	BIO_get_ssl(root_bio, &ssl);
	if(!ssl) 
	{
		trace_message(P7_TL_ERROR,"Error in BIO_get_ssl()");
		trace_message(P7_TL_INFO,"End   InitSrvTLSComms(NOK)");
		return NOK;
	}

	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

	buffer_bio = BIO_new(BIO_f_buffer());
	root_bio = BIO_push(buffer_bio, root_bio);

	sprintf(szPort,"%u",nPort);	
	trace_event(szPort , TRACE);
	
	accept_bio=BIO_new_accept(szPort); 

  
	BIO_set_accept_bios(accept_bio,root_bio);


	BIO_set_bind_mode(accept_bio, BIO_BIND_REUSEADDR);

	if(BIO_do_accept(accept_bio) <= 0) 
	{
		
		ERR_print_errors_fp(stderr);
		trace_message(P7_TL_ERROR,"Error in BIO_do_accept(): [Port: %.12s]",szPort);
		trace_message(P7_TL_INFO,"End   InitSrvTLSComms(NOK)");
		return NOK;
	}

	nNextFd = 0;

	trace_event("End InitSrvTLSComms(OK)",PROCESSING);
	return OK;
}
/*****************************************************************************/
int AcceptTLSCallSrv(char  calling_address [16])
{

	int		nLocalFd;
	int		fd;
	struct	sockaddr_in adr_client;
	int		nLen;
	unsigned char      addr[4];
	int 	offset;
	char    sLine[MAX_LINE_TRC];
	BIO *	conn_bio;
	int		result;
	

	trace_event("Start AcceptTLSCallSrv()",PROCESSING);

	if(BIO_do_accept(accept_bio) <= 0) 
	{
		ERR_print_errors_fp(stderr);
		trace_message(P7_TL_ERROR,"Error in BIO_do_accept()");
		trace_message(P7_TL_INFO,"End   AcceptTLSCallSrv(NOK)");
		return NOK;
	}
	
	conn_bio= BIO_pop(accept_bio);
	

	if (BIO_get_fd(conn_bio, &fd) == -1)
	{
		trace_message(P7_TL_ERROR,"Error in BIO_get_fd()");
		trace_message(P7_TL_INFO,"End   AcceptTLSCallSrv(NOK)");
		return NOK;
	}
	
	if (BIO_do_handshake(conn_bio) <= 0) 
	{
		ERR_print_errors_fp(stderr);
		trace_message(P7_TL_ERROR,"Handshare error on establishing TLS connection");
		trace_message(P7_TL_INFO,"End AcceptTLSCallSrv(NOK)");
		return NOK;
	}


	nLen = sizeof(struct	sockaddr_in);
	getpeername(fd,(struct sockaddr*)&adr_client, &nLen);

	inet_ntop( AF_INET, &adr_client.sin_addr.s_addr, calling_address, INET_ADDRSTRLEN );


	nLocalFd = GetFreeConnSlot();
	if( nLocalFd < 0 )
	{
		trace_message(P7_TL_ERROR,"No Free Slot");
		trace_message(P7_TL_INFO,"End   AcceptTLSCallSrv(NOK)");
		return NOK;
	}


	tab_bio[nLocalFd].fd=fd;
	memcpy(tab_bio[nLocalFd].addr,addr,sizeof(tab_bio[nLocalFd].addr));
	tab_bio[nLocalFd].conn_bio= conn_bio;
	tab_bio[nLocalFd].free = 0;

	trace_message(P7_TL_PROCESSING,"End   AcceptTLSCallSrv(OK): [Index:%d,fd:%d,Host:%.128s]",nLocalFd,tab_bio[nLocalFd].fd,calling_address);
	return nLocalFd;
}

int StopCltTLSCom(int nConnDesc)
{
	char  sLine[ MAX_LINE_TRC ];
	int		fd;

	
	sprintf(sLine,"Start StopCltTLSCom(): [Index:%d]",nConnDesc);
	trace_event (sLine , PROCESSING); 

	if( nConnDesc < 0 )
		return NOK;

	if( tab_bio[nConnDesc].free == 1 )
	{
		trace_message(P7_TL_ERROR,"Index is free: [Index:%d]",nConnDesc);
		trace_message(P7_TL_INFO,"End   StopCltTLSCom(NOK)");
		return NOK;
	}

	if (BIO_get_fd(tab_bio[nConnDesc].conn_bio, &fd) != -1)
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

	BIO_free(tab_bio[nConnDesc].conn_bio); /* or BIO_free_all?*/
	tab_bio[nConnDesc].free = 1;

	trace_message(P7_TL_INFO,"End   StopCltTLSCom(OK): [Index:%d,fd:%d,Addr:%.4s]",nConnDesc,fd,tab_bio[nConnDesc].addr);
	return OK;
}

int tls_readcheck( int nConnDesc , int nTimer )
{
    return tcp_readcheck(tab_bio[nConnDesc].fd,nTimer);
 }

