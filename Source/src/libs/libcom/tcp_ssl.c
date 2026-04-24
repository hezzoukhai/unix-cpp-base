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



#include <openssl/bio.h>
#include <openssl/ssl.h>


#define CONN_TIMEOUT_PER		300

extern char LocalAppName[];

/*****************************************************************************/
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
static int CloseClientConn(int nSlotNbr)
{
	BIO_free_all(tab_bio[nSlotNbr].conn_bio);
	tab_bio[nSlotNbr].free = 1;
}
/*****************************************************************************/
void  StopCltComms()
{
	BIO_flush(root_bio);
	BIO_free_all(root_bio);
	/* Free error strings */
	/*ERR_free_strings();*/
	/* remove all ciphers and digests from the table */
	EVP_cleanup();
	/*TODO: Add close server fd*/
}
/*****************************************************************************/
void	InitSSL(char* szCertificatePath, char* szPrivateKeyPath)
{
	int		i;
	int		nSize;
	
	memset(szCertificateFile,'\0',sizeof(szCertificateFile));
	memset(szPrivateKeyFile,'\0',sizeof(szPrivateKeyFile));

	if( szCertificatePath && strlen(szCertificatePath) > 0 )
	{
		strncpy(szCertificateFile,szCertificatePath,sizeof(szCertificateFile)-1);
	}

	if( szPrivateKeyFile && strlen(szPrivateKeyFile) > 0 )
	{
		strncpy(szPrivateKeyFile,szPrivateKeyPath,sizeof(szPrivateKeyFile)-1);
	}

	nSize = sizeof(tab_bio)/sizeof(tab_bio[0]);
	for( i=0; i < nSize; i++)
	{
		tab_bio[i].free=1;
	}
}
/*****************************************************************************/
int InitSrvComms(int nPort)
{

	char	szPort[24];

	trace_event("Start InitSrvCommsSSL()",PROCESSING);

	memset(szPort,'\0',sizeof(szPort));

	
	SSL_library_init();
	/*Load libcrypto and libssl error strings for output*/
	SSL_load_error_strings(); 
	/*ERR_load_crypto_strings();
	ERR_load_SSL_strings();*/
	OpenSSL_add_all_algorithms();

	/*TODO: Seed PRNG */
	ctx = SSL_CTX_new(SSLv23_server_method());
	if( !ctx )
	{
		trace_event("Error in SSL_CTX_new()",PROCESSING);
		ERR_print_errors_fp(stderr);
		return NOK;
	}

	if (!SSL_CTX_use_certificate_file(ctx,szCertificateFile,SSL_FILETYPE_PEM)) 
	{
			trace_event("Error in SSL_CTX_use_certificate_file()",PROCESSING);
			ERR_print_errors_fp(stderr);
			return NOK;
	}

	if (!SSL_CTX_use_PrivateKey_file(ctx,szPrivateKeyFile,SSL_FILETYPE_PEM)) 
	{
			trace_event("Error in SSL_CTX_use_PrivateKey_file()",PROCESSING);
			ERR_print_errors_fp(stderr);
			return NOK;
	}
	if (!SSL_CTX_check_private_key(ctx)) 
	{
			trace_event("Error in SSL_CTX_check_private_key()",PROCESSING);
			ERR_print_errors_fp(stderr);
			return NOK;
	}

	root_bio=BIO_new_ssl(ctx,0);
	BIO_get_ssl(root_bio, &ssl);
	if(!ssl) 
	{
		trace_event("Error in BIO_get_ssl()",PROCESSING);
		return NOK;
	}

	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

	buffer_bio = BIO_new(BIO_f_buffer());
	root_bio = BIO_push(buffer_bio, root_bio);

	sprintf(szPort,"%u",nPort);	
	trace_event(szPort , FATAL);
	
	accept_bio=BIO_new_accept(szPort); 

  
	BIO_set_accept_bios(accept_bio,root_bio);


	BIO_set_bind_mode(accept_bio, BIO_BIND_REUSEADDR);

	if(BIO_do_accept(accept_bio) <= 0) 
	{
		trace_event("Error in BIO_do_accept(): Setup",PROCESSING);
		ERR_print_errors_fp(stderr);
		return NOK;
	}

	nNextFd = 0;

	trace_event("End InitSrvCommsSSL(OK)",PROCESSING);
	return OK;
}
/*****************************************************************************/
int AcceptCallSrv(char  calling_address [16])
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

	trace_event("Start AcceptCallSrvSSL()",PROCESSING);
	if(BIO_do_accept(accept_bio) <= 0) 
	{
		trace_event("Error in BIO_do_accept(): Connection",PROCESSING);
		ERR_print_errors_fp(stderr);
		trace_event("End AcceptCallSrvSSL(NOK)",PROCESSING);
		return NOK;
	}
	trace_event("Pop",PROCESSING);
	
	
	conn_bio= BIO_pop(accept_bio);
	
	/*sbio = BIO_pop(acpt);*/
	/*
	BIO_free_all(acpt);
	*/
	trace_event("Start get_fd",PROCESSING);

	if (BIO_get_fd(conn_bio, &fd) == -1)
	{
		trace_event("Error in BIO_get_fd()",PROCESSING);
		trace_event("End AcceptCallSrvSSL(NOK)",PROCESSING);
		return NOK;
	}
	
	

	/*EBE160224: let be done by slave thread to not block the main threadS
	result = tcp_readcheck(fd,CONN_TIMEOUT_PER); 
	if ( result != OK ) 
	{
		sprintf(sLine,"tcp_readcheck result %d",result);
		trace_event(sLine, FATAL);
		return(result);
	}
	if( BIO_do_handshake(conn_bio) <= 0) {
		trace_event("Error in SSL handshake",PROCESSING);
		ERR_print_errors_fp(stderr);
		trace_event("End AcceptCallSrvSSL(NOK)",PROCESSING);
		return NOK;
	}

	trace_event("Handshake Successful",PROCESSING);
	*/


	

	nLen = sizeof(struct	sockaddr_in);
	getpeername(fd,(struct sockaddr*)&adr_client, &nLen);

	addr[0]= adr_client.sin_addr.s_addr % 256;
    addr[1]= (adr_client.sin_addr.s_addr >> 8 ) % 256;
    addr[2]= (adr_client.sin_addr.s_addr >> 16) % 256;
    addr[3]= (adr_client.sin_addr.s_addr >> 24) % 256;

	/*EBE160216: let it be done by the caller
	nLocalFd = GetClientSlot(addr);
	if( nLocalFd >= 0 )
	{
		CloseClientConn(nLocalFd);
	}
	else*/
	{
		nLocalFd = GetFreeConnSlot();
		if( nLocalFd < 0 )
		{
			trace_event("No Free Slot",PROCESSING);
			trace_event("End AcceptCallSrvSSL(NOK)",PROCESSING);
			return NOK;
		}
	}

	tab_bio[nLocalFd].fd=fd;
	memcpy(tab_bio[nLocalFd].addr,addr,sizeof(tab_bio[nLocalFd].addr));
	tab_bio[nLocalFd].conn_bio= conn_bio;
	tab_bio[nLocalFd].free = 0;


	sprintf(sLine,"CALL Ad In: %u.%u.%u.%u (%d)",
			addr[0], addr[1], addr[2], addr[3],tab_bio[nLocalFd].fd);
	trace_event(sLine, PROCESSING);
	offset = 0;
	sprintf(calling_address + offset, "%03d", addr[3]); offset += 3;
	calling_address[offset++]='.';
	sprintf(calling_address + offset, "%03d", addr[2]); offset += 3;
	calling_address[offset++]='.';
	sprintf(calling_address + offset, "%03d", addr[1]); offset += 3;
	calling_address[offset++]='.';
	sprintf(calling_address + offset, "%03d", addr[0]); offset += 3;


	trace_event("End AcceptCallSrvSSL(OK)",PROCESSING);
	return nLocalFd;
}
/*****************************************************************************/
int ReadLineMsg( int nConnDesc, char *sBuffer, int *nLength)
{
	
   char  sBuffRcv[ LG_MAX ];
   char  sLine[ MAX_LINE_TRC ];
   int   nRead;
   

   int	nTimerType;
   int	result;

	trace_event("Start ReadLineMsgSSL()",PROCESSING);
	memset (sBuffRcv, 0, sizeof(sBuffRcv));
 
	
	

	/*nConnDesc = tab_PisoCtx[nIndexCtx - RESERVED_THREAD ].fd;*/


	result = tcp_readcheck(tab_bio[nConnDesc].fd,CONN_TIMEOUT_PER); 
	if ( result != OK ) 
	{ 
		sprintf(sLine,"tcp_readcheck result %d",result);
		trace_event(sLine, FATAL);
		return(result);
	}

	trace_event("End readcheck",PROCESSING);

	if(BIO_do_handshake(tab_bio[nConnDesc].conn_bio) <= 0) 
	{
		sprintf(sLine, "Error in SSL handshake");
		trace_event(sLine,TRACE);
		return (NOK);
	}
	trace_event("Handshake Successful",PROCESSING);

	nRead = BIO_read(tab_bio[nConnDesc].conn_bio,sBuffRcv,7); 
	if (nRead == 0)
	{
		sprintf(sLine,"End   ReadLineMsg(OR_DISCONECT:1) nRead %d errno : %d , nFdConn : %d",nRead,errno,nConnDesc);
		trace_event(sLine,TRACE);
		return (-1);
	}
	else if (nRead < 0)
	{
		sprintf(sLine,"End   ReadLineMsg(NOK:1) nRead %d errno : %d , nFdConn : %d",nRead,errno,nConnDesc);
		trace_event(sLine,TRACE);
		return (-1);
	}
	if( memcmp(sBuffRcv,"ISO",3) != 0 )
	{
		sprintf(sLine,"End   ReadLineMsg(NOK:1) Invalid buffer received [%s], nFdConn : %d",sBuffRcv,nConnDesc);
		trace_event(sLine,ERROR);
		return (-1);	
	}
	
    *nLength =    atoi(sBuffRcv+3);

	sprintf(sLine,"nLength : %d ",*nLength);
	trace_event(sLine,TRACE);

	sprintf(sLine,"Longueur brut [%s] nLength [%d]",sBuffRcv,*nLength);
	trace_event(sLine,TRACE);

	if (*nLength > LG_MAX)
	{
		sprintf(sLine , "--- ReadLineMsg (ERROR) --- Msg too BIG : %d",*nLength);
		trace_event(sLine, FATAL);
		return NOK;
	}
	
	if (*nLength <= 7)
	{
		sprintf(sLine , "--- ReadLineMsg (ERROR) --- invalid length : %d",*nLength);
		trace_event(sLine, FATAL);
		return NOK;
	}
	
	nRead = BIO_read(tab_bio[nConnDesc].conn_bio,sBuffRcv + 7,*nLength - 7);

	sprintf(sLine,"Read : %d ",nRead);
	trace_event(sLine,TRACE);

	if (nRead == 0)
	{
		sprintf(sLine,"End   ReadLineMsg(OR_DISCONECT) nRead %d errno : %d , nFdConn : %d",nRead,errno,nConnDesc);
		trace_event(sLine,TRACE);
		return (-1);
	}
	else if (nRead < 0)
	{
		sprintf(sLine,"End   ReadLineMsg(NOK) nRead %d errno : %d , nFdConn : %d",nRead,errno,nConnDesc);
		trace_event(sLine,TRACE);
		return -1;
	}
       
	
	memcpy( sBuffer, sBuffRcv , *nLength);
	

   	dump_buffer(sBuffer, *nLength, 'A', 'L', 'I');



	memset (sBuffRcv, 0, sizeof(sBuffRcv)); /**PA DSS*/

	sprintf(sLine,"End ReadLineMsgSSL(OK) %d,%d",tab_bio[nConnDesc].fd, *nLength);
	trace_event(sLine,PROCESSING);
         
	return OK;

}
/*****************************************************************************/
int WriteLineMsg( int nConnDesc, char *sBuffer, int nLength)
{
	char	sLine[ MAX_LINE_TRC ];
	int		nRet;
	
   
	trace_event("Start WriteLineMsgSSL()",PROCESSING);


	 
         

	dump_buffer(sBuffer, nLength, 'A', 'L', 'O');

	nRet = BIO_write(tab_bio[nConnDesc].conn_bio,sBuffer,nLength);
	BIO_flush(tab_bio[nConnDesc].conn_bio);

	if (nRet <= 0)
	{
		sprintf(sLine,"End   WriteLineMsgSSL(NOK) nRet %d errno : %d , nConnDesc : %d",nRet,errno,nConnDesc);
		trace_event(sLine,TRACE);
		return (NOK);
	}

	sprintf(sLine,"End WriteLineMsgSSL(OK) %d,%d",tab_bio[nConnDesc].fd, nLength);
	trace_event(sLine,PROCESSING);

	return OK;
}
/*****************************************************************************/

int tcp_readcheck( int nFdConnect , int nTimer )
{
    int nResult ;
    fd_set rfds ;
    struct timeval tv;
    char  sLine[ MAX_LINE_TRC ];

    memset(sLine,0,sizeof(sLine));
    sprintf(sLine,"Start tcp_readcheck(%d)",nTimer);
    trace_event(sLine,PROCESSING);
 
 
    if( nFdConnect < 0 ) return -1 ;             /* bad socket id */
    if( nTimer <= 0 ) return -2 ;                /* bad timer */

    FD_ZERO(&rfds); FD_SET(nFdConnect , &rfds) ; /* check only sd */
 
    tv.tv_sec = nTimer ;                         /* set timer */
    tv.tv_usec = 0 ;                         /* set timer */
 
    nResult = select(nFdConnect+1, &rfds, NULL, NULL, &tv ) ;  /* check it */
    
    if( nResult == -1 )
    {
        trace_event ("tcp_readcheck ERROR / select() returned -1" , PROCESSING); 
        return (-1);
    }
    else if( nResult == 0 )
    {
        trace_event ("tcp_readcheck TIME OUT/ select() returned 0" , PROCESSING); 
        return (-1);
    }
    return (OK);
 }


int  CloseCldCom()
{
return(OK);
}


int StopCltCom(int nConnDesc)
{
	char  sLine[ MAX_LINE_TRC ];
	int		fd;

	
	sprintf(sLine,"Start StopCltCom(%d)",nConnDesc);
	trace_event (sLine , PROCESSING); 

	if( nConnDesc < 0 )
		return NOK;

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

	BIO_free(tab_bio[nConnDesc].conn_bio);
	tab_bio[nConnDesc].free = 1;


	return OK;
}

