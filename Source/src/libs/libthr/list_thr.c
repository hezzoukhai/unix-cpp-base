#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <list_thr.h>
#include <define.h>
#include <errno.h>
#include <struct_time.h>


static struct timeval s_stNextTimeOut = {0,0};
extern char ResourceId[];

int  InsertRequest(char sens, TSNodeData * data)
{
	TSNodeStruct *prev,
		*current; 
	pthread_mutexattr_t mattr;
	time_t TimeSys;
	char	sLine[LG_MAX];
	int 	nPurgeTime = 0; /*PLUTONL-4411: ZKO10032022*/
	unsigned int long	nRecCounter = 0; /*IBO20220810 PLUTONL-5032*/

	if (sens == 'B')
	{
		trace_event("Start InsertRequest (ACQ)",PROCESSING);
		prev = &ListHeadAcq;
	}
	else
	{
		trace_event("Start InsertRequest (ISS)",PROCESSING);
		prev = &ListHeadIss;
	}

	pthread_mutex_lock(&(prev->lock)); 

	while ((current = prev->Next) != NULL) 
	{ 
		pthread_mutex_lock(&current->lock); 

		if ( (memcmp(current->data.szFstKey, data->szFstKey, sizeof(data->szFstKey)) == 0) &&
			(memcmp(current->data.szSndKey, data->szSndKey, sizeof(data->szSndKey)) == 0) &&
			(memcmp(current->data.szTrdKey, data->szTrdKey, sizeof(data->szTrdKey)) == 0) &&
			(memcmp(current->data.szFrtKey, data->szFrtKey, sizeof(data->szFrtKey)) == 0) &&
			(memcmp(current->data.szFftKey, data->szFftKey, sizeof(data->szFftKey)) == 0)   ) 
		{ 
			pthread_mutex_unlock(&current->lock); 
			pthread_mutex_unlock(&prev->lock);
			sprintf(sLine,"[NODELISTNO:%c]: %lu ",sens,nRecCounter);	/*IBO20220810 PLUTONL-5032*/
			trace_event(sLine,STREAM);
			trace_event("End   InsertRequest (EEXIST)",PROCESSING);
			return(EEXIST); 
		} 
		pthread_mutex_unlock(&prev->lock);
		prev = current;
		nRecCounter++;  /*IBO20220810 PLUTONL-5032*/
	}
	sprintf(sLine,"[NODELISTNO:%c]: %lu ",sens,nRecCounter);	/*IBO20220810 PLUTONL-5032*/
	trace_event(sLine,STREAM);

	if ( (current = malloc(sizeof(TSNodeStruct))) == NULL)
	{
		pthread_mutex_unlock(&prev->lock); 
		trace_event("End   InsertRequest (ENOMEM)",PROCESSING);
		return(ENOMEM);
	}

	InitNodeData( &(current->data) );
	time(&TimeSys);
	gettimeofday(&current->stCreationTime,NULL);
	current->stCreationTime.tv_sec -= EPOCH_TIME_ADJUSTMENT;
	current->tCreationTime = TimeSys;
	current->data.tPurgeTime = data->tPurgeTime;
	current->data.stPurgeTime.tv_sec = data->stPurgeTime.tv_sec;
	current->data.stPurgeTime.tv_usec = data->stPurgeTime.tv_usec;
	if( current->data.stPurgeTime.tv_sec == 0 )
	{
		trace_event("-- stPurgeTime = 0, Considering tPurgeTime--",PROCESSING);
		current->data.stPurgeTime.tv_sec = data->tPurgeTime;
		current->data.stPurgeTime.tv_usec = current->stCreationTime.tv_usec;
	}

	nPurgeTime = (current->data.stPurgeTime.tv_sec + EPOCH_TIME_ADJUSTMENT)%3600; /*PLUTONL-4411: ZKO10032022*/

	/*sprintf(sLine," Creation time: %lu:%lu - PurgeTime: %lu:%lu",current->stCreationTime.tv_sec,current->stCreationTime.tv_usec,
		current->data.stPurgeTime.tv_sec,current->data.stPurgeTime.tv_usec);*/
	sprintf(sLine," Creation time: %lu:%lu - PurgeTime: %d:%d",current->stCreationTime.tv_sec,current->stCreationTime.tv_usec,
		nPurgeTime/60,nPurgeTime%60);
	trace_event(sLine,PROCESSING);


	current->data.msgId = data->msgId;	/*EBE130925:SWMON*/
	memcpy ( current->data.szFstKey, data->szFstKey, sizeof(data->szFstKey));
	memcpy ( current->data.szSndKey, data->szSndKey, sizeof(data->szSndKey));
	memcpy ( current->data.szTrdKey, data->szTrdKey, sizeof(data->szTrdKey));
	memcpy ( current->data.szFrtKey, data->szFrtKey, sizeof(data->szFrtKey));
	memcpy ( current->data.szFftKey, data->szFftKey, sizeof(data->szFftKey));

	current->data.nFstBufLen = data->nFstBufLen;
	memcpy ( current->data.sFstBuf, data->sFstBuf, data->nFstBufLen);

	current->data.nSndBufLen = data->nSndBufLen;
	memcpy ( current->data.sSndBuf, data->sSndBuf, data->nSndBufLen);

	current->data.nFstPrvtDataLen = data->nFstPrvtDataLen;
	if ( data->nFstPrvtDataLen != 0)
		memcpy ( current->data.sFstPrvtData, data->sFstPrvtData, 
		data->nFstPrvtDataLen);

	current->data.nSndPrvtDataLen = data->nSndPrvtDataLen;
	if ( data->nSndPrvtDataLen != 0)
		memcpy ( current->data.sSndPrvtData, data->sSndPrvtData, 
		data->nSndPrvtDataLen);

	current->data.nTrdPrvtDataLen = data->nTrdPrvtDataLen;
	if ( data->nTrdPrvtDataLen != 0)
		memcpy ( current->data.sTrdPrvtData, data->sTrdPrvtData, 
		data->nTrdPrvtDataLen);

	current->Next = NULL;
	pthread_mutexattr_init(&mattr);
	pthread_mutex_init( &(current->lock) , &mattr);
	pthread_mutexattr_destroy(&mattr);
	prev->Next = current;

	pthread_mutex_unlock(&prev->lock); 
	trace_event("End   InsertRequest (OK)",PROCESSING);
	return(OK); 
}

int  FetchRequest (char sens, TSNodeData * data)
{ 
	TSNodeStruct *prev,
		*current; 
	time_t TimeSys;
	struct	timeval		stTimeSys; /*EBE130306*/
	char   sLine[MAX_LINE_TRC];

	if (sens == 'B')
	{
		trace_event("Start FetchRequest (ACQ)",PROCESSING);
		prev = &ListHeadAcq;
	}
	else
	{
		trace_event("Start FetchRequest (ISS)",PROCESSING);
		prev = &ListHeadIss;
	}

	pthread_mutex_lock(&prev->lock); 
	while ((current = prev->Next) != NULL) 
	{
		pthread_mutex_lock(&current->lock); 
		if ( (memcmp(current->data.szFstKey, data->szFstKey, 
			sizeof(data->szFstKey)) == 0) &&
			(memcmp(current->data.szSndKey, data->szSndKey, 
			sizeof(data->szSndKey)) == 0) &&
			(memcmp(current->data.szTrdKey, data->szTrdKey, 
			sizeof(data->szTrdKey)) == 0) &&
			(memcmp(current->data.szFrtKey, data->szFrtKey, 
			sizeof(data->szFrtKey)) == 0) &&
			(memcmp(current->data.szFftKey, data->szFftKey, 
			sizeof(data->szFftKey)) == 0)  )
		{ 
			prev->Next = current->Next;
			pthread_mutex_unlock(&current->lock); 
			pthread_mutex_destroy(&(current->lock));
			pthread_mutex_unlock(&prev->lock);

			data->nFstBufLen = current->data.nFstBufLen;
			memcpy ( data->sFstBuf, current->data.sFstBuf, 
				current->data.nFstBufLen);
			data->nSndBufLen = current->data.nSndBufLen;
			memcpy ( data->sSndBuf, current->data.sSndBuf, 
				current->data.nSndBufLen);
			data->tPurgeTime = current->data.tPurgeTime;

			data->nFstPrvtDataLen = current->data.nFstPrvtDataLen ;
			if ( data->nFstPrvtDataLen != 0)
				memcpy ( data->sFstPrvtData, current->data.sFstPrvtData, 
				data->nFstPrvtDataLen);

			data->nSndPrvtDataLen = current->data.nSndPrvtDataLen ;
			if ( data->nSndPrvtDataLen != 0)
				memcpy ( data->sSndPrvtData, current->data.sSndPrvtData, 
				data->nSndPrvtDataLen);

			data->nTrdPrvtDataLen = current->data.nTrdPrvtDataLen ;
			if ( data->nTrdPrvtDataLen != 0)
				memcpy ( data->sTrdPrvtData, current->data.sTrdPrvtData, 
				data->nTrdPrvtDataLen);

			/*EBE160310: do not free now, still used below
			free(current);*/
			time(&TimeSys);
			gettimeofday(&stTimeSys,NULL);


			/**** Reponse Tardive ***/
			if ((stTimeSys.tv_sec - EPOCH_TIME_ADJUSTMENT) > current->data.stPurgeTime.tv_sec || 
				((stTimeSys.tv_sec - EPOCH_TIME_ADJUSTMENT) == current->data.stPurgeTime.tv_sec && stTimeSys.tv_usec > current->data.stPurgeTime.tv_usec) )
			{
				data->msgId = current->data.msgId;
				InitNodeData( &(current->data) );	/*AMER 20160730 PADSS Fix*/
				free(current);
				trace_event("End   FetchRequest (ELATE)",PROCESSING);
				return(ELATE);
			}
			else
			{
				data->msgId = current->data.msgId;
				InitNodeData( &(current->data) );	/*AMER 20160730 PADSS Fix*/
				free(current);
				trace_event("End   FetchRequest (OK)",PROCESSING);

				return(OK); 
			}
		} 
		pthread_mutex_unlock(&prev->lock);
		prev = current; 
	}
	pthread_mutex_unlock(&prev->lock); 
	trace_event("End   FetchRequest (ENOEXIST)",PROCESSING);
	return(ENOEXIST); 
}

int  FreeTimeOutRequest (char sens)
{
	TSNodeStruct *prev,
		*current; 
	char			   sLine[MAX_LINE_TRC];
	TSTimeExchange  sTimeMsg;
	time_t TimeSys;
	struct	timeval		stTimeSys; /*EBE130306*/
	struct	timeval		stNextTimeOut;
	int 	nPurgeTime = 0; /*PLUTONL-4411: ZKO10032022*/
	unsigned int long	nRecCounter = 0; /*IBO20220810 PLUTONL-5032*/

	if (sens == 'B')
		prev = &ListHeadAcq;
	else
		prev = &ListHeadIss;

	time(&TimeSys);
	gettimeofday(&stTimeSys,NULL);
	pthread_mutex_lock(&prev->lock); 
	while ((current = prev->Next) != NULL) 
	{

		pthread_mutex_lock(&current->lock); 
		if ( current->data.stPurgeTime.tv_sec < (stTimeSys.tv_sec - EPOCH_TIME_ADJUSTMENT)  || 
			( current->data.stPurgeTime.tv_sec == (stTimeSys.tv_sec - EPOCH_TIME_ADJUSTMENT) && stTimeSys.tv_usec > current->data.stPurgeTime.tv_usec) )
		{ 
			/*Start PLUTONL-4411: ZKO10032022*/
			nPurgeTime = (current->data.stPurgeTime.tv_sec + EPOCH_TIME_ADJUSTMENT)%3600; 
			/*sprintf(sLine,"PurgeTime: %lu:%lu | SysTime: %lu:%lu - ", current->data.stPurgeTime.tv_sec,current->data.stPurgeTime.tv_usec,
				stTimeSys.tv_sec,stTimeSys.tv_usec);*/
			sprintf(sLine,"PurgeTime: %d:%d | SysTime: %ld:%ld - ", nPurgeTime/60, nPurgeTime%60,
				stTimeSys.tv_sec,stTimeSys.tv_usec);
			/*End PLUTONL-4411: ZKO10032022*/
			trace_event(sLine,PROCESSING);

			prev->Next = current->Next;
			pthread_mutex_unlock(&current->lock); 
			pthread_mutex_destroy(&(current->lock));

			sTimeMsg.sens = sens;
			memcpy( (char *) &(sTimeMsg.sData), (char *) &(current->data), sizeof(TSNodeData));


			/* this is called by individual interfaces, e.g we dont want this flag for SAF messages
			MsgTimeout(current->data.msgId,ResourceId);*/
			/*** Give Data to TimeOut Threads ***/
			PTimeWaitEntry();
			PTimePutMsg   (&sTimeMsg);
			PTimeConsoInfo();

			InitNodeData( &(current->data) );	/*AMER 20160730 PADSS Fix*/
			free(current);

			/*PROD00055494(8):reset on every call(consumed)*/
			/*s_stNextTimeOut.tv_sec = 0;
			s_stNextTimeOut.tv_usec = 0;*/

			if ( (current = prev->Next) == NULL)
				break;
		} 
		/* Start HYV2.0.1 ***************************
		pthread_mutex_unlock(&prev->lock);
		prev = current; 
		*********************************************/
		else
		{
			pthread_mutex_unlock(&prev->lock);
			prev = current;
			nRecCounter++;  /*IBO20220810 PLUTONL-5032*/
			/*EBE130306: If we get here, this means that all upcoming nodes are older than current one and will not time out */
			stNextTimeOut.tv_sec	= current->data.stPurgeTime.tv_sec - (stTimeSys.tv_sec - EPOCH_TIME_ADJUSTMENT);
			if( current->data.stPurgeTime.tv_usec < stTimeSys.tv_usec )
			{
				stNextTimeOut.tv_sec = stNextTimeOut.tv_sec - 1;
				stNextTimeOut.tv_usec	= current->data.stPurgeTime.tv_usec - stTimeSys.tv_usec + 1000000;
			}
			else
			{
				stNextTimeOut.tv_usec	= current->data.stPurgeTime.tv_usec - stTimeSys.tv_usec;
			}

			if( ( s_stNextTimeOut.tv_sec == 0 && s_stNextTimeOut.tv_usec == 0 ) ||
				stNextTimeOut.tv_sec < s_stNextTimeOut.tv_sec ||
				(stNextTimeOut.tv_sec == s_stNextTimeOut.tv_sec && stNextTimeOut.tv_usec < s_stNextTimeOut.tv_usec))
			{
				s_stNextTimeOut.tv_sec = stNextTimeOut.tv_sec;
				s_stNextTimeOut.tv_usec = stNextTimeOut.tv_usec;
			}
			/* break; PLUTONL-4081 */
		}
		/* End   HYV2.0.1 ***************************/
	}
	pthread_mutex_unlock(&prev->lock);
	sprintf(sLine,"[NODELISTNO:%c]: %lu ",sens,nRecCounter);	/*IBO20220810 PLUTONL-5032*/
	trace_event(sLine,STREAM);
	return(OK);
}

void GetNextTimeOut(struct	timeval* stNextTimeOut)
{
	(*stNextTimeOut) = s_stNextTimeOut;
	/*PROD00055494(8):reset on every call(consumed) */
	s_stNextTimeOut.tv_sec = 0;
	s_stNextTimeOut.tv_usec = 0;
}
