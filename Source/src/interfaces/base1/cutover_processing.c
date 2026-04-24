#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <define.h>
#include <define.h>
#include <iso_hps.h>
#include <tlv_autho.h>
#include <stdlib.h>


int CutOffManagement( int nIndiceCtx , char *sResource )
{
	int   result = NOK;
	char  sDate[ 13]; 
	char  sMti [ 4 + 1];
	char  sStan [ 4 + 1];
	char  sSrcResource [ 4 + 1];
	char  sResponseCode [ 4 + 1];
	char  sBuffer [ LG_MAX ];
	int   nSendCounter,nMti, nLength;
	TSIsoInfo  sMsgIso;

	memset ( sDate, 0 , sizeof ( sDate ));
	memset ( sMti , 0 , sizeof ( sMti  ));
	memset ( sStan, 0 , sizeof ( sStan ));
	memset ( sSrcResource,0 , sizeof ( sSrcResource ));
	memset ( sResponseCode, 0 ,sizeof ( sResponseCode ));
	memset ( sBuffer , 0 , sizeof ( sBuffer ));

	/*EBETODO: Add DB conn mng */
	do 
	{ 
		sleep ( GetAdviceInterval ( sResource ));
		result = GetSafData  ( 	nIndiceCtx,  sResource , 'R',
								GetDeclineAdvicesFlag(sResource), 
								sMti, sStan, sSrcResource, &nSendCounter,
								sResponseCode, &sMsgIso
								);
	} while ( result != OK );

	/* if this is a repeat message then update XMIT time and message number */
	if ( nSendCounter > 0 )    
	{
		memset ( sDate , 0, sizeof(sDate) );
		current_gmt_date(sDate);
		AddIsoField ( ISO_XMIT_TIME, &sMsgIso, sDate + 2, 10);
		nMti = atoi ( sMti );
		sMsgIso.nMsgType = nMti + 1; 
	}

	if (WriteLineMsg(sBuffer, nLength) != OK )
	{
		trace_event ( "CAN NOT WRITE SAF MESSAGE THROUGH LINE" , PROCESSING );
		IntWriteLineExeption(nIndiceCtx,&sMsgIso); /* ELKAM PLUTONL-3835 */
		return ( NOK );
	}

	WaitSafAnswer();

	return OK;
}

/******************************************************************************/
/* si reponse est recu le thread line va passer le message reponse et indiquer*/
/* le type du message reponse et le code reponse recu ( meme si redonadance ) */
/******************************************************************************/
int CutOffResponseReceived( int			nIndiceCtx , 
							TSIsoInfo	*sMsgIso,  /* la structure reponse */
							char		*sSourceResource, 
							char		*sDestResource,
							char		*sSlfMti ,    /* Type depuis message reponse */
							char		*sSlfResponseCode,
							int			nSendCounter ) 
{
	int  nLength;
	int  nMti;
	char sMti[ 4 + 1];
	char sStan[ 6 + 1];

	memset ( sMti  , 0 ,  sizeof ( sMti )); 
	memset ( sStan , 0 , sizeof ( sStan )); 

	SafRespReceived();

	GetIsoField( ISO_AUDIT_NBR , sMsgIso , sStan , &nLength );

	nMti = sMsgIso->nMsgType;
	sprintf ( sMti , "%04d", nMti - 10 );

	DeleteSafData  (	nIndiceCtx, sMti, 'R' , sStan, 
						sSourceResource , sDestResource , 
						sSlfMti, sSlfResponseCode
						);

	return OK;
}

/******************************************************************************/
/* meme si r�ponse non re�u le thread timeout doit changer le type du message et indiquer un code r�ponse qui voudra dire timeout                             */
/******************************************************************************/
int CutOffTimeOutReceived  (	int			nIndiceCtx , 
								TSIsoInfo	*sMsgIso,  
								char		*sSourceResource, 
								char		*sDestResource,
								char		*sSlfMti ,
								char		*sSlfResponseCode,
								int			nSendCounter ) 
{
	int		nLength;
	int		nMti;
	char	sMti[ 4 + 1];
	char	sStan[ 6 + 1];
	char	sLine[MAX_LINE_TRC];

	memset ( sMti , 0 ,   sizeof ( sMti )); 
	memset ( sStan , 0 ,  sizeof ( sStan )); 

	SafTimeOutDetected();


	nMti = sMsgIso->nMsgType;
	sprintf ( sMti , "%04d", nMti - 10 );
	GetIsoField( ISO_AUDIT_NBR , sMsgIso , sStan , &nLength );

	sprintf(sLine,">>> CutOffTimeOut  nSendCounter = %d, sStan = %s",nSendCounter,sStan);
	

	if ( nSendCounter + 1 <=  GetMaxSafCounter() )
	{
		UpdateSafData ( nIndiceCtx, sMti, 'R' , sStan,
						sSourceResource, sDestResource, 
						nSendCounter + 1);
	}
	else
	{
		DeleteSafData ( nIndiceCtx, sMti, 'R' , sStan, 
						sSourceResource , sDestResource , 
						sSlfMti, sSlfResponseCode);
	}

	return OK;
}


