/*******************************************************************************/
/*  (c) Hightech Payment Systems 2012                                          */
/* Module                                                                      */
/*      ssl_tcp_ndc.c                                                          */
/*                                                                             */
/* Description                                                                 */
/*      SSL TCP NDC Tools Management                                           */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001BRO     		25-06-2012   		Bryan ROSALES    		Initiale Version	
---------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socketvar.h>

#include <define.h>
#include <line_com.h>
#include <tcp_var.h>
#include <atmopcode.h>


#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <ssl_var.h>

#include <globals.h> /*DDA12122012*/

#ifdef LINUX
	#include <sys/syscall.h>
#endif


void  GetSslConnect(SSL *sSslStruct)
{
	sprintf(sSslStruct,"%s", nSsl);
}

void  SetCtxConnect(SSL_CTX *sCtxStruct)
{
  sprintf(sCtxStruct,"%s", SslCtx); 
}


int ReadSslLineMsg(int nFd, char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   char  sLine[MAX_LINE_TRC];
   int   nLenMsg =0, nRead=0, result=0;
   SSL   *Ssl; 
   int  nIndexCtx;
   int    nLeft = 0;
   char   *sPtr;
   int    nReadCount = 0,ret=0;  

	memset( sBuffRcv, 0, sizeof(sBuffRcv));
	memset( sLine, 0, sizeof(sLine));
	
trace_event("Start ReadSslLineMsg ndc",PROCESSING);

#ifdef LINUX
        nIndexCtx = GetThreadIndex((long int)syscall(SYS_gettid));
#else
        nIndexCtx = GetThreadIndex(pthread_self());
#endif

	if (nIndexCtx < 0 ) 
	{
      	sprintf(sLine, "End ReadSslLineMsg...(nIndexCtx %d)",nIndexCtx);
		trace_event(sLine,PROCESSING);
        return (OR_DISCONECT);
	 }
	  
    
	GetSslByOraCtx(nIndexCtx +RESERVED_THREAD ,Ssl);

	result = SSL_get_fd(Ssl);
	
	if (result < 0)
	{
		sprintf(sLine, "SSL_get_fd...(%d)",result);
		trace_event(sLine,PROCESSING);
		trace_event("End ReadSslLineMsg...(OR_DISCONECT)",PROCESSING);
		return (OR_DISCONECT);
	}


   fprintf ( stderr,"\n av rec Socket = %d \n", nFd );
   do
   {
       nRead = SSL_read(Ssl, sBuffRcv, 2);
	   
   } while ( (nRead !=2) && (nRead > 0));
   if (nRead < 0)
   {
      fprintf(stderr,"Error Reading MSG_PEEK : %d \n", errno);
      return (OR_BAL);
   }
   if (nRead == 0)
   {
      fprintf(stderr,"Disconnect MSG_PEEK: %d \n", errno);
      return (OR_DISCONECT);
   }
   nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1] + 2 ;
   
   	if (nLenMsg > 4000 ) 
   	{
   		sprintf ( sLine,"ReadSslLineMsg Msg Too Big %d)", nLenMsg );
   		trace_event(sLine,ERROR);

		nRead = SSL_read (Ssl, sBuffer, LG_MAX);

		trace_event("ATM Will Be Disconnected",ERROR);
        return (OR_DISCONECT);
   	}
	
   
    memset(sBuffRcv,0,sizeof(sBuffRcv));
	
	sPtr = sBuffRcv;
	nLeft = nLenMsg;
	nRead = 0;
    nReadCount = 0; 
	
	while (nLeft)
	{
		nRead = SSL_read (Ssl, sPtr, nLeft);
		ret = SSL_get_error(Ssl,nRead);

		switch(ret)
		{
			case SSL_ERROR_NONE:
				 break;
			case SSL_ERROR_WANT_READ:
					trace_event("SSL_read SSL_ERROR_WANT_READ",PROCESSING);
					continue;
					break;
			case SSL_ERROR_ZERO_RETURN:
					trace_event("SSL_read error SSL_ERROR_ZERO_RETURN - Disconnect",FATAL);
					return (OR_DISCONECT);
					break;
			case SSL_ERROR_SYSCALL:                                        
					sprintf(sLine, "SSL_read error : Premature close : nRead = %d, nLeft= %d", nRead, nLeft);
					trace_event(sLine, ERROR);	
					break;
			default:                                       
				   sprintf(sLine, "SSL_read problem : nRead = %d, nLeft= %d, SSL_get_error =%d", nRead, nLeft, ret);
				   trace_event(sLine, ERROR);	
				   break;
		}
		
        if (nRead == 0)
        {
                sprintf(sLine,"Disconnect Errno: %d ", errno);
                trace_event(sLine,FATAL);
                return (OR_DISCONECT);
        }
		
        if (nRead < 0) 
				nRead = 0;

		sPtr  += nRead;
		nLeft -= nRead;
                     
                      	 
		nReadCount +=1;
		if (nReadCount > 20 )
		{
			sprintf(sLine,"Error Reading Msg nLeft = : %d ", nLeft);
			trace_event(sLine,FATAL);
			trace_event("ATM Will Be Disconnected by Force !! ",ERROR);
			return (OR_DISCONECT);

		} 
 										 
	}
				
				
   	if (nRead < 0)
   	{
      		sprintf(sLine,"Error Reading Msg errno : %d ", errno);
      		trace_event(sLine,FATAL);
      		return (OR_BAL);
   	}
   	if (nRead == 0)
   	{
      		sprintf(sLine,"Disconnect Msg Errno: %d ", errno);
      		trace_event(sLine,FATAL);
      		return (OR_DISCONECT);
   	}

   	memcpy( sBuffer, sBuffRcv   , nLenMsg );
   	*nLength = nLenMsg;
	

   memset(sBuffRcv, 0, sizeof(sBuffRcv));  

   	sprintf ( sLine,"End   ReadSslLineMsg(OK(%d))", nLenMsg );
   	trace_event(sLine,TRACE);

   return (OK);
}

int WriteSslLineMsg(int nFd, char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   char sLine [MAX_LINE_TRC];
   int retour,ret,neg;
   int i;
   SSL     *Ssl; 
   int     	nIndexCtx;
   
   
	sprintf(sLine, "Start WriteSslLineMsg...(%d)",nLength);
	trace_event(sLine,PROCESSING);
		
   memset(sLine, 0, sizeof(sLine));
   memset (sBuffSnd, 0, sizeof(sBuffSnd));
   
#ifdef LINUX
	nIndexCtx = GetThreadIndex((long int)syscall(SYS_gettid));
#else
	nIndexCtx = GetThreadIndex(pthread_self());
#endif
	if (nIndexCtx < 0 ) 
	{
      	sprintf(sLine, "End WriteSslLineMsg...(nIndexCtx %d)",nIndexCtx);
		trace_event(sLine,PROCESSING);
        return (OR_DISCONECT);
	 }
   
    GetSslByOraCtx(nIndexCtx +RESERVED_THREAD ,Ssl);
	 
	 
   sBuffSnd [ 0 ] = (nLength ) / 256;
   sBuffSnd [ 1 ] = (nLength ) % 256;
   memcpy (sBuffSnd +2 , sBuffer, nLength);
   
   fprintf ( stderr,"\n av send Socket = %d longueur = %d \n", nFd , nLength );
 
   sprintf(sLine, "BuffSnd[%02X][%02X] ",sBuffSnd [ 0 ], sBuffSnd [ 1 ]);
   trace_event(sLine, PROCESSING);


	sprintf(sLine, "SSL State for the FD[%d],Status[%d]",nFd,SSL_get_state(Ssl));
	trace_event(sLine, TRACE);
	
		
	if (SSL_get_state(Ssl) != SSL_ST_OK) 
	{
		neg = SSL_accept(Ssl);
		if (neg <= 0) 
		{
			 sprintf(sLine, "[MAN12102013] Re-negotiate Failed for the FD[%d],Status[%d]",nFd,neg);
		 trace_event(sLine, TRACE);
		   openssl_error(Ssl,neg);
		}
	}	

	retour = SSL_write( Ssl, sBuffSnd, nLength+2);
	sprintf( sLine,"SSL_write return (%d) bytes written", retour );
	trace_event(sLine,TRACE);
	
	if (retour < 1)
      {
		ret = SSL_get_error(Ssl,retour);
		
		fprintf(stderr,"Error Sending Msg : %d \n", errno);
		
		sprintf(sLine, "SSL WRITE ERROR : result =>%d, SSL_get_errors=>%d, Errno=>%d", retour, ret,errno);
		trace_event(sLine, ERROR);	
		
		openssl_error(Ssl,retour);
		
		sprintf(sLine, "ERROR[%s]",ERR_error_string(ERR_get_error(), NULL));
		trace_event(sLine, ERROR);
		
		StopCltCom(nFd);
		memset (sBuffSnd, 0, sizeof(sBuffSnd));
		trace_event("End WriteSslLineMsg(NOK)", PROCESSING);								      		
		return (NOK);      		
      }		
	  

   memset (sBuffSnd, 0, sizeof(sBuffSnd));
   trace_event("End WriteSslLineMsg(OK)",PROCESSING);
   return(OK);
}



int ReadSslLineMsg_Org(int nFd, SSL *Ssl, char *sBuffer, int *nLength)
{
   char  sBuffRcv[ LG_MAX ];
   char  sLine[MAX_LINE_TRC];

   int   nLenMsg =0, nRead=0;
   int   result = 0;	
   int    nLeft = 0;
   char   *sPtr;
   char   sDest[ LG_MAX ]; /*BRO06122010: PCI*/
   int    nReadCount = 0; /*MUW08052013:*/
   int    ret = 0; /*MUW08052013:*/

	sprintf(sLine, "[MAN12102013]Start ReadSslLineMsg...(%d)",SSL_get_fd(Ssl));
   	trace_event(sLine,PROCESSING);

   	memset( sBuffRcv, 0, sizeof(sBuffRcv));
   	memset( sBuffer, 0, LG_MAX);
   	memset( sDest, 0, sizeof(sDest));  /*BRO06122010: PCI*/

   	do
   	{
       		if (ssl_active == 1)	
		{
			trace_event("ssl read active",PROCESSING);
			nRead = SSL_read(Ssl, sBuffRcv, 2);
		}
		else
		{
			trace_event("ssl read not actvie",PROCESSING);
			nRead = recv(nFd, sBuffRcv, 2 , 0);
		}
   	} while ( (nRead !=2) && (nRead > 0));

   	if (nRead < 0)
   	{
		if (ssl_active == 1) 
      			result=SSL_get_error(Ssl,nRead);

      		sprintf(sLine, "Error Reading Msg Length : %d nRead = %d,SSL_get_error= %d ", errno, nRead, result);
      		trace_event(sLine,FATAL);
      		
      		/*MAN29052013*/
      		 openssl_error(Ssl,nRead);
					/*MAN29052013*/
      		
      		/*START SMT27032012*/
      		if (errno == 9)
      			return (OR_DISCONECT);
      		/*END SMT27032012*/
      		return (OR_BAL);
   	}
   	if (nRead == 0)
   	{
      				/* Start HGH280512 */
			
			/*sprintf(sLine,"Disconnect Errno: %d ", errno);
      		trace_event(sLine,FATAL);
      		return (OR_DISCONECT);*/
			
			nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1];	
			sprintf ( sLine,"Disconnect Errno : %d   Buffer Lenght is : %d",errno,nLenMsg); 	
			trace_event(sLine, FATAL);
			return (NOK);
	
	       /* End HGH280512 */
   	}
   	nLenMsg = ((unsigned char)sBuffRcv[0] * 256) + (unsigned char)sBuffRcv[1];

   	if (nLenMsg > 4000 ) /*LG_MAX )*/
   	{
   		sprintf ( sLine,"ReadSslLineMsg Msg Too Big %d)", nLenMsg );
   		trace_event(sLine,ERROR);

		if (ssl_active == 1)
			nRead = SSL_read (Ssl, sBuffer, LG_MAX);
		else
			nRead = recv (nFd, sBuffer, LG_MAX, 0);

		trace_event("DUMP OF INVALID DATA",FATAL);

    /*Start BRO06122010: PCI*/
     		if (debug_option == 1) 
			dump_buffer(sBuffer, nRead, 'A', 'L','I');
    		else
			EncryptBuff(sBuffer,nRead,sDest);
		/*End BRO06122010: PCI*/

		trace_event("ATM Will Be Disconnected",ERROR);
        	return (OR_DISCONECT);
   	}

	memset(sBuffRcv,0,sizeof(sBuffRcv));

	sPtr = sBuffRcv;
	nLeft = nLenMsg;
	nRead = 0;
        nReadCount = 0; /*MUW08052013:*/
    
	if (ssl_active == 1) {
		trace_event("SSLREAD",PROCESSING);

		while (nLeft){
                	nRead = SSL_read (Ssl, sPtr, nLeft);
                	  ret = SSL_get_error(Ssl,nRead);
                	  
                        switch(ret){
                       		case SSL_ERROR_NONE:
                                	break;
                                case SSL_ERROR_WANT_READ:
                                        trace_event("SSL_read SSL_ERROR_WANT_READ",PROCESSING);
                                        continue;
                                case SSL_ERROR_ZERO_RETURN:
                                        trace_event("SSL_read error SSL_ERROR_ZERO_RETURN - Disconnect",FATAL);
                                        return (OR_DISCONECT);
                                case SSL_ERROR_SYSCALL:                                        
                                        sprintf(sLine, "SSL_read error : Premature close : nRead = %d, nLeft= %d", nRead, nLeft);
								                        trace_event(sLine, ERROR);	
                                        break;
                                default:                                       
                                       sprintf(sLine, "SSL_read problem : nRead = %d, nLeft= %d, SSL_get_error =%d", nRead, nLeft, ret);
								                       trace_event(sLine, ERROR);	
                                       break;
                        }
/**MAN12102013*********************************************************/
        if (nRead == 0)
        {
                sprintf(sLine,"Disconnect Errno: %d ", errno);
                trace_event(sLine,FATAL);
                return (OR_DISCONECT);
        }
        if (nRead < 0) 
				nRead = 0;

/**MAN12102013*********************************************************/
		        sPtr  += nRead;
                        nLeft -= nRead;
                     
                      	/*start MUW 07052013  To stop infinite read during error*/  
                      	nReadCount +=1;
                      	if (nReadCount > 20 ) /*LG_MAX )*//*MAN12102013 : 20 instead of 20000*/
												   	{
												      		sprintf(sLine,"Error Reading Msg nLeft = : %d ", nLeft);
												      		trace_event(sLine,FATAL);
                									trace_event("ATM Will Be Disconnected by Force !! ",ERROR);
                									return (OR_DISCONECT);

												   	} 
 												/*end MUW 07052013  To stop infinite read during error*/  
                }
	}
	else {
		trace_event("NONSSLREAD",PROCESSING);

		while (nLeft  ){
                	if ( (nRead = recv (nFd, sPtr, nLeft, 0)) <= 0)
                        	break;

                	sPtr  += nRead;
                	nLeft -= nRead;
        	}

	}

   	if (nRead < 0)
   	{
      		sprintf(sLine,"Error Reading Msg errno : %d ", errno);
      		trace_event(sLine,FATAL);
      		return (OR_BAL);
   	}
   	if (nRead == 0)
   	{
      		sprintf(sLine,"Disconnect Msg Errno: %d ", errno);
      		trace_event(sLine,FATAL);
      		return (OR_DISCONECT);
   	}

   	memcpy( sBuffer, sBuffRcv   , nLenMsg );
   	*nLength = nLenMsg;

    	/*Start BRO06122010: PCI*/
     	if (debug_option == 1) 
				dump_buffer(sBuffer, *nLength, 'A', 'L','I');
    	else
				EncryptBuff(sBuffer,*nLength,sDest);
			/*End BRO06122010: PCI*/

   	sprintf ( sLine,"End   ReadSslLineMsg(OK(%d))", nLenMsg );
   	trace_event(sLine,TRACE);
   	return (OK);
}
int WriteSslLineMsg_Org(int nFd, SSL *Ssl, char *sBuffer, int nLength)
{
   char  sBuffSnd[ LG_MAX ];
   int 		retour,neg;
   int 		i;
   int 		ret;
   char 	sLine [LG_MAX];

   sprintf ( sLine,"Start WriteSslLineMsg [MAN12102013] Len:[%d] Fd:[%d] ", nLength,nFd );
   trace_event(sLine,TRACE);

   memset(sLine, 0, sizeof(sLine));
   memset (sBuffSnd, 0, LG_MAX);
   
   sBuffSnd [ 0 ] = (nLength ) / 256;
   sBuffSnd [ 1 ] = (nLength ) % 256;
   
   memcpy (sBuffSnd +2 , sBuffer, nLength);
   dump_buffer(sBuffSnd, nLength+2, 'A', 'L','O');

   sprintf (sLine,"ssl_active flag =(%d) and buff Length =(%d) ", ssl_active,nLength );
   trace_event(sLine,TRACE);
   
   if (ssl_active == 1)
   { 
   		/*MAN12102013**/
    	sprintf(sLine, "SSL State for the FD[%d],Status[%d]",nFd,SSL_get_state(Ssl));
    	trace_event(sLine, TRACE);
			if (SSL_get_state(Ssl) != SSL_ST_OK) 
			{
				neg = SSL_accept(Ssl);
				if (neg <= 0) 
				{
					 sprintf(sLine, "[MAN12102013] Re-negotiate Failed for the FD[%d],Status[%d]",nFd,neg);
    			 trace_event(sLine, TRACE);
				   openssl_error(Ssl,neg);
				}
			}	
			/*MAN12102013**/
   		retour = SSL_write( Ssl, sBuffSnd, nLength+2);
		  sprintf( sLine,"SSL_write return (%d) bytes written", retour );
	   	trace_event(sLine,TRACE);
 	    if (retour < 1)
      {
      	  /* MAN15092013 Get the Error and Close the current ATM connection */
      	  ret = SSL_get_error(Ssl,retour);
		      sprintf(sLine, "SSL WRITE ERROR : result =>%d, SSL_get_errors=>%d, Errno=>%d", retour, ret,errno);
		      trace_event(sLine, ERROR);								      
      		openssl_error(Ssl,retour);
      		sprintf(sLine, "ERROR[%s]",ERR_error_string(ERR_get_error(), NULL));
      		trace_event(sLine, ERROR);
					/* StopSslCltComNew(nFd,Ssl); */
					StopCltCom(nFd);
          trace_event("End SSL_write(NOK)", PROCESSING);								      		
		      return (NOK);      		
      }			      
   }
   else
	 retour = write ( nFd, sBuffSnd, nLength+2);
   
   if (retour <= 0)
   {
      sprintf(sLine,"Error Sending Msg errno : %d ", errno);
      trace_event(sLine,FATAL);
      return(NOK);
   }

   trace_event("End WriteSslLineMsg(OK)",PROCESSING);
   return(OK);
}
