#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include <define.h>
#include <thread_var.h>

#include <thr_bal_extern.h>


#include <queue.h>
#include <param_arg.h>
#include <resources.h>
#include <security_data.h>
#include <tlv_fields.h>
#include <iso_fields.h>
#include <iso_hps.h>
#include <tlv_autho.h>

#ifdef LINUX
#include <sys/syscall.h>
#endif


static TSBalCtx  tab_BalCtx [MAX_THREAD];

int GetBalThreadIndex (pthread_t tid)
{
	char				sLine[MAX_LINE_TRC];
	int				i;
	char             sThreadId  [7];

	sprintf(sLine, "Start GetBalThreadIndex(%lu) ", tid );
	trace_event(sLine, PROCESSING);

	for (i = 0; i < MAX_THREAD; i++)
	{
		if (tab_BalCtx[i].tid == tid)
			break;
	}

	if ( i >= MAX_THREAD)
	{
		sprintf(sLine, "End GetThreadIndex (%d)", -1);
		trace_event(sLine, PROCESSING);
		return(-1);
	}

	sprintf(sLine, "End GetThreadIndex(%d)", i);
	trace_event(sLine, PROCESSING);
	return (i);
}




int  InfoBalThread(int nIndex)
{
	pthread_mutex_lock(&(tab_BalCtx[nIndex].BalMutex));
	pthread_cond_signal(&(tab_BalCtx[nIndex].BalCond));
	pthread_mutex_unlock(&(tab_BalCtx[nIndex].BalMutex));
	return(0);
}

void* thread_Bal(void *arg)
{
	TSBalExchange		sBalMsg;
	char				sLine[MAX_LINE_TRC];

	TSIsoInfo       	IsoInfo;
	TSTlvAutho			tlvInfo;
	char				sSecurityData[128];
	char				szUser  [	 16];
	char				szTid   [7+1];	/*PLUTONL-2402*/
	pthread_t 			tid;
	int					tmpFieldLen;
	int 				nIndex;
	int				nBalId;


	pthread_detach(pthread_self());
	ThreadMaskSignals();

	nBalId = ((TSArgParam *)arg)->nIndice;
	
	free(arg);

	while (1) 
	{
		if (ReadBalMsg(nBalId, getpid(), !IPC_NOWAIT, &sBalMsg) < 0)
		{
			fprintf(stderr, " Error Reading Message \n");
			pthread_exit(NULL);
		}

		if (strcmp(sBalMsg.sHeader.nFormat, "TLV") == 0)
		{
			InitTlvAutho(&tlvInfo);
			AnalyseTlvAutho(sBalMsg.sBody, &tlvInfo);
			GetTlvAutho( AUTO_SECURITY_DATA, &tlvInfo, sSecurityData, &tmpFieldLen);
		}
		else 
		{
			InitIsoInfo (&IsoInfo);
			AnalyseIso(sBalMsg.sBody, &IsoInfo);
			GetIsoField( ISO_SECURITY_DATA, &IsoInfo, sSecurityData, &tmpFieldLen);
		}

		GetSecurityData( USER, sSecurityData, szUser, &tmpFieldLen);
		memcpy (szTid, szUser+8, 7);	/*PLUTONL-2402*/
		tid = atoi(szTid);
		sprintf(sLine," Tread Id = %lu",tid);
		trace_event(sLine,PROCESSING);

		if ( (nIndex = GetBalThreadIndex (tid) ) != -1)
		{
			memcpy(tab_BalCtx[nIndex].szBufferBal, sBalMsg.sBody,sBalMsg.sHeader.nLen);
			tab_BalCtx[nIndex].nLenBal = sBalMsg.sHeader.nLen;
			InfoBalThread(nIndex);
		}

	}
}
int WaitBalResponseWithRcv (int nIndex, int nTimer,char *szBuffer )
{
	struct timeval  tv;
	struct timespec ts;
	int    retour;
	char		sLine[MAX_LINE_TRC];

	tab_BalCtx[nIndex].flag_bal = WAITING;

	if (gettimeofday(&tv, NULL) < 0)
	{
		fprintf(stderr,"gettimeofday Error : %d\n", errno);
		return(NOK);
	}

	ts.tv_sec  = tv.tv_sec + nTimer;
	ts.tv_nsec = tv.tv_usec * 1000;

	pthread_mutex_lock (&(tab_BalCtx[nIndex].BalMutex));
	if ( (retour =pthread_cond_timedwait(&(tab_BalCtx[nIndex].BalCond),
		&(tab_BalCtx[nIndex].BalMutex),
		&ts)) < 0)
	{
		if (retour == ETIMEDOUT)
		{
			retour = TIMEOUT;
			tab_BalCtx[nIndex].flag_bal = TIMEOUT;
		}
		else
		{
			retour = NOK;
			tab_BalCtx[nIndex].flag_bal = NOK;
		}
	}
	else
	{
		if (retour == ETIMEDOUT)
		{
			retour = TIMEOUT;
			tab_BalCtx[nIndex].flag_bal = TIMEOUT;
		}
		else
		{
			retour = OK;
			tab_BalCtx[nIndex].flag_bal = OK;
			memcpy(szBuffer, tab_BalCtx[nIndex].szBufferBal, tab_BalCtx[nIndex].nLenBal);
			szBuffer[tab_BalCtx[nIndex].nLenBal]='\0';
		}
	}

	pthread_mutex_unlock (&(tab_BalCtx[nIndex].BalMutex));
	return(retour);
}


int BalQuery( int nIndiceCtx, int nBalId , int nDestinationPid  , int nMti ,TSIsoInfo  *isoInfo   , char *BuffRcv )
{
	int				BalStatus;
	TSBalExchange   sBalMessage;
	char			sLine[MAX_LINE_TRC];
	int             nLength;
	char             tmpField[256];
	char             szUser  [16];
	char             sThreadId  [7+1];	/*PLUTONL-2402*/
	int              tmpFieldLen;
	int              Length;
	char             sSecurityData[128+1];


	/* Add issuer purge time to Security Data Field (53) */

	memset(sSecurityData, 0, sizeof(sSecurityData));
	memset((char *)&sBalMessage, 0, sizeof(TSBalExchange        ));
	memset(szUser, 0, sizeof(szUser));
	memset(sThreadId, 0, sizeof(sThreadId));

	GetIsoField (ISO_SECURITY_DATA, isoInfo, sSecurityData, &tmpFieldLen);

	tab_BalCtx [ nIndiceCtx ].tid = p7_thread_id();
	sprintf( sThreadId , "%07ld" , tab_BalCtx [ nIndiceCtx ].tid);	/*PLUTONL-2402*/
	GetSecurityData( USER, sSecurityData , szUser, &tmpFieldLen);
	memcpy(szUser + 8, sThreadId, 7);	/*PLUTONL-2402*/
	PutSecurityData( USER , sSecurityData , szUser , strlen(szUser) );
	PutIsoField( ISO_SECURITY_DATA , isoInfo , sSecurityData ,strlen( sSecurityData ) );

	
	sprintf(sLine, "Sec [%s] ", sSecurityData);
	trace_event(sLine,TRACE);
	

	sBalMessage.sHeader.nPartnerId = nDestinationPid;
	sBalMessage.sHeader.nSourceId  = getpid();
	sBalMessage.sHeader.nCommand   = nMti;
	nLength = IsoBuildMsg ( sBalMessage.sBody, isoInfo);
	sBalMessage.sHeader.nLen = nLength;
	memset( tab_BalCtx[nIndiceCtx].szBufferBal, 0, sizeof(tab_BalCtx[nIndiceCtx].szBufferBal));

	if ( WriteBalMsg( nBalId, &sBalMessage) != OK)
	{
		trace_event("Can Not Write Message To Interface Queue", PROCESSING);
		trace_event ( "End  HsmQuery ( NOK )", PROCESSING );
		return( MALFUNCTION );
	}

	memset ( (char *)&sBalMessage , 0, sizeof ( sBalMessage ));

	BalStatus = WaitBalResponseWithRcv(nIndiceCtx, TIME_OUT_NBR , BuffRcv);

	if ( BalStatus  != OK )
	{
		trace_event ("Time Out Situation With Interface", PROCESSING);
		trace_event ( "End BalQuery ( NOK )", PROCESSING );
		return ( NOT_AVAILABLE );
	}

	trace_event ( "End BalQuery ( OK )", PROCESSING );
	return ( OK );
}


int BalQueryTlv( int nIndiceCtx, int nBalId , int nDestinationPid  , int nMti , TSTlvAutho *tlvInfo , char *BuffRcv )
{
	int				BalStatus;
	TSBalExchange   sBalMessage;
	char			sLine[MAX_LINE_TRC];
	int             nLength;
	char             tmpField[256];
	char             szUser  [16];
	char             sThreadId  [7+1];	/*PLUTONL-2402*/
	int              tmpFieldLen;
	int              Length;
	char             sSecurityData[128+1];



	trace_event("Start BalQueryTlv()",PROCESSING);

	
	memset(sSecurityData, 0, sizeof(sSecurityData));
	memset((char *)&sBalMessage, 0, sizeof(TSBalExchange        ));
	memset(szUser, 0, sizeof(szUser));
	memset(sThreadId, 0, sizeof(sThreadId));
	GetTlvAutho(AUTO_SECURITY_DATA,tlvInfo,sSecurityData,&tmpFieldLen);

	tab_BalCtx [ nIndiceCtx ].tid = p7_thread_id();
	sprintf( sThreadId , "%07ld" , tab_BalCtx [ nIndiceCtx ].tid);	/*PLUTONL-2402*/


	GetSecurityData( USER, sSecurityData , szUser, &tmpFieldLen);
	memcpy(szUser + 8, sThreadId, 7);	/*PLUTONL-2402*/
	PutSecurityData( USER , sSecurityData , szUser , strlen(szUser) );
	/** */
	{
		time_t nPurgeTime;
		char sPurgeTime[9 + 1];
		memset (sPurgeTime, 0, sizeof(sPurgeTime));
		time(&nPurgeTime);
		nPurgeTime += (( TIME_OUT_NBR * 3 ) - 2);
		nPurgeTime -=  1000000000;
		sprintf ( sPurgeTime , "%09ld" , nPurgeTime );
		/*memcpy (sSecurityData + 53, sPurgeTime, 9);*/
		PutSecurityData( PURGE_TIME , sSecurityData , sPurgeTime , strlen(sPurgeTime) );
		sprintf(sLine, ">>>>>MAN03022016 Sec %s ", sSecurityData);
		trace_event(sLine,TRACE);
	}

	
	PutTlvAutho( AUTO_SECURITY_DATA , tlvInfo , sSecurityData ,strlen( sSecurityData ) );

	
	sprintf(sLine, "Sec [%s] ", sSecurityData);
	trace_event(sLine,TRACE);
	
	memcpy(sBalMessage.sHeader.nFormat, "TLV",3);
	sBalMessage.sHeader.nPartnerId = nDestinationPid;
	sBalMessage.sHeader.nSourceId  = getpid();
	sBalMessage.sHeader.nCommand   = nMti;
	nLength = AuthoBuildTlv  ( sBalMessage.sBody, tlvInfo);
	sBalMessage.sHeader.nLen = nLength;
	memset( tab_BalCtx[nIndiceCtx].szBufferBal, 0, sizeof(tab_BalCtx[nIndiceCtx].szBufferBal));


	if ( WriteBalMsg( nBalId, &sBalMessage) != OK)
	{
		trace_event("Can Not Write Message To Interface Queue", PROCESSING);
		trace_event("End   BalQueryTlv(MALFUNCTION)",PROCESSING);
		return( MALFUNCTION );
	}

	memset ( (char *)&sBalMessage , 0, sizeof ( sBalMessage ));

	BalStatus = WaitBalResponseWithRcv(nIndiceCtx, TIME_OUT_NBR*3 , BuffRcv);

	if ( BalStatus  != OK )
	{
		trace_event ("Time Out Situation With Interface", PROCESSING);
		trace_event("End   BalQueryTlv(NOT_AVAILABLE)",PROCESSING);
		return ( NOT_AVAILABLE );
	}

	trace_event("End   BalQueryTlv(OK)",PROCESSING);
	return ( OK );
}


void InitTabBalCtx(int nBalAdmMsg)
{
	int	i;
	int	nSize;
	pthread_mutexattr_t mattr;
	pthread_condattr_t  cattr;

	pthread_mutexattr_init(&mattr);
	pthread_condattr_init(&cattr);
        TSArgParam        *p_arg_adm;
        TSArgParam        *p_arg_msg;

        char              sLine[MAX_LINE_TRC];


	sprintf(sLine, "InitTabBalCtx [%d] ", nBalAdmMsg);
        trace_event(sLine,TRACE);
	


	
	nSize = sizeof(tab_BalCtx)/sizeof(tab_BalCtx[0]);

	for (i=0; i < nSize; i++)
	{
		tab_BalCtx[i].tid       = -1;
		tab_BalCtx[i].nLenBal   = 0;
		memset (tab_BalCtx[i].szBufferBal, 0 , sizeof(tab_BalCtx[i].szBufferBal) );
		pthread_mutex_init( &(tab_BalCtx[i].BalMutex) , &mattr);
		pthread_cond_init ( &(tab_BalCtx[i].BalCond) , &cattr);
	}
       
       if(nBalAdmMsg == BAL_MESSAGE)
	{
		p_arg_msg = (TSArgParam *) malloc(sizeof(TSArgParam));
	        p_arg_msg->nIndice = nBalMsg;

	        if (pthread_create(&(tab_ThrInfo[BAL_THREAD_ID].tid), NULL, thread_Bal, p_arg_msg) < 0)
        	{
                	fprintf(stderr, "pthread_create (Cmd ) Error %d\n", errno);
                	exit(0);
        	}

	}
	else if(nBalAdmMsg == BAL_ADMIN){
		
		p_arg_adm = (TSArgParam *) malloc(sizeof(TSArgParam));
		
		p_arg_adm->nIndice = nBalAdm;

        	if (pthread_create(&(tab_ThrInfo[ADM_THREAD_ID].tid), NULL, thread_Bal, p_arg_adm) < 0)
        	{
                	fprintf(stderr, "pthread_create (Cmd ) Error %d\n", errno);
                	exit(0);
        	}

	}
	else{

	       p_arg_adm = (TSArgParam *) malloc(sizeof(TSArgParam));
               p_arg_msg = (TSArgParam *) malloc(sizeof(TSArgParam));

               p_arg_adm->nIndice = nBalAdm;

		if (pthread_create(&(tab_ThrInfo[ADM_THREAD_ID].tid), NULL, thread_Bal, p_arg_adm) < 0)
        	{
                	fprintf(stderr, "pthread_create (Cmd ) Error %d\n", errno);
                	exit(0);
        	}
	
  		

		p_arg_msg->nIndice = nBalMsg;
                if (pthread_create(&(tab_ThrInfo[BAL_THREAD_ID].tid), NULL, thread_Bal, p_arg_msg) < 0)
                {
                        fprintf(stderr, "pthread_create (Cmd ) Error %d\n", errno);
                        exit(0);
                }


	 }


}


