/*******************************************************************************/
/*  (c) Hightech Payment Systems 2001                                          */
/* Module                                                                      */
/*      tcp_com.c                                                              */
/*                                                                             */
/* Description                                                                 */
/*      TCP-IP Tools Management                                                */
/*******************************************************************************/
/*---------------------------------------------------------------------------------------------------------------------------------
 Id              	Date             	Author                       	Comment
-----------------------------------------------------------------------------------------------------------------------------------
 0001MK     		18-02-2002   		Mahjoub KARROUM    		Initiale Version	
 0002NG                 15-07-2003              Nabil   Gamrane               Modification de l'addresse IP 
                                                                               dans les fcts : AcceptCallSrv et AcceptCall
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

#include <define.h>
#include <tcp_var.h>
#include <globals.h>


extern char RemoteAppName [];
extern char RemoteAppId [];

int InitCltComms()
{
   return ( InitCltCom( RemoteAppName , RemoteAppId ) );
}


int InitCltHsm(char *sAddress, int nPort)
{
   char                         ligne[134];
   struct sockaddr_in           sAddrServ;
   int            nFdConn;
   int            on;
   int            retour;


   sprintf(ligne, "Start InitCltHsm(%s, %d)", sAddress, nPort);
   trace_event(ligne,  PROCESSING);

   bzero((char *)&sAddrServ, sizeof(sAddrServ));
   sAddrServ.sin_family = AF_INET;
   sAddrServ.sin_port   = htons(nPort);
   sAddrServ.sin_addr.s_addr = inet_addr(sAddress);


   /* creation et attachement du socket en local                       */
   if ( (nFdConn = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      sprintf(ligne, "SOCKET ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
      return(NOK);
   }
/*---
   on=1 ;
   retour = setsockopt (nFdConn, IPPROTO_TCP, TCP_RFC1323, &on,sizeof(on));
   if(retour != 0)
   {
      fprintf( stderr,"Can not Use Socket Option TPC_RFC1323" );
      fprintf( stderr,"End   open_device(NOK)");
      return(NOK);
   }
---*/
   on=1 ;
   retour = setsockopt (nFdConn, IPPROTO_TCP, TCP_NODELAY, &on,sizeof(on));
   if(retour != 0)
   {
      fprintf( stderr,"Can not Use Socket Option TPC_NODELAY");
      fprintf( stderr,"End   open_device(NOK)");
      return(NOK);
   }

   on=1 ;
   retour = setsockopt (nFdConn, SOL_SOCKET, SO_KEEPALIVE, &on,sizeof(on));
   if(retour != 0)
   {
      fprintf( stderr,"Can not Use Socket Option SO_KEEPALIVE");
      fprintf( stderr,"End   open_device(NOK)");
      return(NOK);
   }

   on=1 ;
#if defined(LINUX) || defined(SOLARIS) 
   retour = setsockopt (nFdConn, SOL_SOCKET , SO_REUSEADDR, &on,sizeof(on));
#else
   retour = setsockopt (nFdConn, IPPROTO_TCP , SO_REUSEADDR, &on,sizeof(on));
#endif
   if(retour != 0)
   {
      fprintf( stderr,"Can not Use Socket Option SO_REUSEPORT errno: %d\n",errno);
      fprintf( stderr,"End   open_device(NOK)");
      return(NOK);
   }
   if (connect(nFdConn,(struct sockaddr*)&sAddrServ,
					    sizeof(struct sockaddr)) == -1)
   {
      sprintf(ligne, "CONNECT ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   IniCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   trace_event("End   IniCltCom(OK)", PROCESSING);
   return (nFdConn);
}

int InitCltHsmByName(char *sServerName, int nPort)
{
   char                         ligne[134];
   int                          sock;
   struct hostent               *hp;
   struct sockaddr_in           sAddrServ;
   struct servent               *serveur;
   int    nFdTemp;


   /***
   nPort = atoi ( sPort );
   sprintf(ligne, "Start InitCltCom(%s, %s)", sServerName, sServiceName);
   ****/
   sprintf(ligne, "Start InitCltCom(%s, %d)", sServerName, nPort);
   trace_event(ligne,  PROCESSING);


   /* creation et attachement du socket en local                       */
   if ( (nFdTemp = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      sprintf(ligne, "SOCKET ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   if ((hp=gethostbyname(sServerName)) == NULL)
   {
      sprintf(ligne,"Machine %s unknown (See Config /etc/hosts)", sServerName);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   /******
   serveur=getservbyname(sServiceName, "tcp");
   if (serveur == NULL)
   {
      sprintf(ligne, "Service name %.16s not found in /etc/services", sServiceName);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
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

   if (connect(nFdTemp,(struct sockaddr*)&sAddrServ,
					    sizeof(struct sockaddr)) == -1)
   {
      sprintf(ligne, "CONNECT ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   IniCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   trace_event("End   IniCltCom(OK)", PROCESSING);
   return (nFdTemp);
}

int InitCltCom(char *sServerName, char *sPort)
{
   char                         ligne[134];
   int                          sock;
   struct hostent               *hp;
   struct sockaddr_in           sAddrServ;
   struct servent               *serveur;
   unsigned 					nPort;


   

   nPort = atoi ( sPort );
   /***
   sprintf(ligne, "Start InitCltCom(%s, %s)", sServerName, sServiceName);
   ****/
   sprintf(ligne, "Start InitCltCom(%s, %d)", sServerName, nPort);
   trace_event(ligne,  PROCESSING);
   /**
   if (ssl_active == 1)
		return ( InitSslCltCom( sServerName , sPort ) );
	*/

   /* creation et attachement du socket en local                       */
   if ( (nFdConnect = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      sprintf(ligne, "SOCKET ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   if ((hp=gethostbyname(sServerName)) == NULL)
   {
      sprintf(ligne,"Machine %s unknown (See Config /etc/hosts)", sServerName);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   /******
   serveur=getservbyname(sServiceName, "tcp");
   if (serveur == NULL)
   {
      sprintf(ligne, "Service name %.16s not found in /etc/services", sServiceName);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
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

   if (connect(nFdConnect,(struct sockaddr*)&sAddrServ,
					    sizeof(struct sockaddr)) == -1)
   {
      sprintf(ligne, "CONNECT ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   IniCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   trace_event("End   IniCltCom(OK)", PROCESSING);
   return (OK);
}

int InitSrvComms(int nPort)
{
   char              ligne[134];
   char		     sLine[MAX_LINE_TRC];
   int               sDesc;
   struct sockaddr_in adr_local;
   unsigned int     Longueur = sizeof(struct sockaddr_in);
   int                       retour;
   struct servent            *serveur;
   int 		on;

   sprintf(sLine, "Start InitSrvCom(%d)", nPort);
   trace_event(sLine, PROCESSING);


   if ( (sDesc = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      sprintf(ligne, "InitSrvCom: SOCKET ERROR : errno = %d ", errno); 
      trace_event(ligne, ERROR);
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


   if (bind(sDesc, (struct sockaddr *)&adr_local, Longueur) == -1)
   {
      sprintf(ligne, "BIND FCT ERROR : errno = %d", errno);
      trace_event(ligne, ERROR);
      close(sDesc);
      trace_event("End   InitSrvCom(NOK)", PROCESSING);
      return(NOK);
   }

   if(&adr_local != NULL)
      getsockname(sDesc, (struct sockaddr *)&adr_local, &Longueur);

   retour = listen(sDesc, 5);
   if ( retour == -1 )
   {
      sprintf(ligne, "LISTEN ERROR : errno = %d", errno);   
      trace_event(ligne, ERROR);
      trace_event("End  InitSrvCom(NOK)", PROCESSING);
      return(NOK);
   }

   nFdLocal = sDesc;
   trace_event("End   InitSrvCom(OK)", PROCESSING);
   return(OK);
}



int AcceptCallSrv(char calling_address[16])
{
   int                long_inet;
   struct sockaddr_in adr_client;
   char               ligne[134];
   int                nFdConn;


   sprintf(ligne, "Start AcceptCallSrv()");
   trace_event(ligne, PROCESSING);

   long_inet = sizeof(struct sockaddr_in);
   nFdConn = accept(nFdLocal,
                       (struct sockaddr *)&adr_client,
                       &long_inet);
   if (nFdConn < 0 )
   {
      sprintf(ligne, "AcceptCall: ACCEPT ERROR : errno = %d", errno);
      trace_event(ligne, ERROR);
      return(NOK);
   }

	inet_ntop( AF_INET, &adr_client.sin_addr.s_addr, calling_address, INET_ADDRSTRLEN );
	sprintf(ligne,"CALL Ad In: [%s]", calling_address);
	trace_event(ligne, PROCESSING);

   
   sprintf(ligne, "End   AcceptCallSrv(%d)", nFdConn);
   trace_event(ligne, PROCESSING);
   return(nFdConn);
}

int AcceptCall(char  calling_address [ 16])
{
  
   nFdConnect = AcceptCallSrv(calling_address);
   if( nFdConnect >= 0 )
   {
	   return OK;
   }
   return NOK;

}


void StopCltCom(int nFd)
{
  char		sLine[MAX_LINE_TRC];	
  trace_event ( "Stop Communication...Begin", PROCESSING );
  shutdown (  nFd  , 2 );
  if (close(nFd) == NOK) {
    sprintf(sLine, "SHUTDOWN ERROR : errno = %d ", errno);
    trace_event(sLine, ERROR);
   }
  trace_event ( "Stop Communication...End", PROCESSING );
}

int InitCentralCom(char *sServerName, char *sPort,int *NFD)
{
   char                         ligne[134];
   int                          sock;
   struct hostent               *hp;
   struct sockaddr_in           sAddrServ;
   struct servent               *serveur;
   unsigned                     nPort;


   nPort = atoi ( sPort );
   sprintf(ligne, "Start InitCentralCom(%s, %d)", sServerName, nPort);
   trace_event(ligne,  PROCESSING);


   /* creation et attachement du socket en local                       */
   if ( (*NFD = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      sprintf(ligne, "SOCKET ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   InitCentralCom(NOK)", PROCESSING);
      return(NOK);
   }
 if ((hp=gethostbyname(sServerName)) == NULL)
   {
      sprintf(ligne,"Machine %s unknown (See Config /etc/hosts)", sServerName);
      trace_event(ligne, ERROR);
      trace_event("End   InitCentralCom(NOK)", PROCESSING);
      return(NOK);
   }

   sAddrServ.sin_family = AF_INET;
#ifdef INTEL
   sAddrServ.sin_port   = serveur->s_port;
#else
   sAddrServ.sin_port   = htons(nPort);
#endif
   memcpy(&sAddrServ.sin_addr.s_addr, hp->h_addr, hp->h_length);

   if (connect(*NFD,(struct sockaddr*)&sAddrServ,
                                            sizeof(struct sockaddr)) == -1)
   {
      sprintf(ligne, "CONNECT ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   IniCentralCom(NOK)", PROCESSING);
      return(NOK);
  }

   trace_event("End   IniCentralCom(OK)", PROCESSING);
   return (OK);
}

int InitCltCom_Test(char *sServerName, char *sPort)
{
   char                         ligne[134];
   int                          sock;
   struct hostent               *hp;
   struct sockaddr_in           sAddrServ;
   struct servent               *serveur;
   unsigned 			nPort;


   nPort = atoi ( sPort );
   sprintf(ligne, "Start InitCltCom_Test(%s, %d)", sServerName, nPort);
   trace_event(ligne,  PROCESSING);

   /* creation et attachement du socket en local                       */
   if ( (nFdConnect = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      sprintf(ligne, "SOCKET ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   if ((hp=gethostbyname(sServerName)) == NULL)
   {
      sprintf(ligne,"Machine %s unknown (See Config /etc/hosts)", sServerName);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   sAddrServ.sin_family = AF_INET;
#ifdef INTEL
   sAddrServ.sin_port   = serveur->s_port;
#else
   sAddrServ.sin_port   = htons(nPort);
#endif
   memcpy(&sAddrServ.sin_addr.s_addr, hp->h_addr, hp->h_length);

   if (connect(nFdConnect,(struct sockaddr*)&sAddrServ,
					    sizeof(struct sockaddr)) == -1)
   {
      sprintf(ligne, "CONNECT ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   IniCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   trace_event("End   IniCltCom_Test(OK)", PROCESSING);
   return (nFdConnect);
}
int StopCltComms()
{
StopCltCom( nFdConnect );
/*SKO290509*/ return(0);
}
int InitCltComFd(char *sServerName, char *sPort)
{
   char                         ligne[134];
   int                          sock;
   int                          nFd;
   struct hostent               *hp;
   struct sockaddr_in           sAddrServ;
   struct servent               *serveur;
   unsigned                                     nPort;


   nPort = atoi ( sPort );
   /***
   sprintf(ligne, "Start InitCltCom(%s, %s)", sServerName, sServiceName);
   ****/
   sprintf(ligne, "Start InitCltCom(%s, %d)", sServerName, nPort);
   trace_event(ligne,  PROCESSING);


   /* creation et attachement du socket en local                       */
   if ( (nFd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
   {
      sprintf(ligne, "SOCKET ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   if ((hp=gethostbyname(sServerName)) == NULL)
   {
      sprintf(ligne,"Machine %s unknown (See Config /etc/hosts)", sServerName);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   /******
   serveur=getservbyname(sServiceName, "tcp");
   if (serveur == NULL)
   {
      sprintf(ligne, "Service name %.16s not found in /etc/services", sServiceName);
      trace_event(ligne, ERROR);
      trace_event("End   InitCltCom(NOK)", PROCESSING);
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

   if (connect(nFd,(struct sockaddr*)&sAddrServ,
                                            sizeof(struct sockaddr)) == -1)
   {
      sprintf(ligne, "CONNECT ERROR : errno = %d ", errno);
      trace_event(ligne, ERROR);
      trace_event("End   IniCltCom(NOK)", PROCESSING);
      return(NOK);
   }

   trace_event("End   IniCltCom(OK)", PROCESSING);
   return (nFd);
}


