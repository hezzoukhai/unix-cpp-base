#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <list_thr.h>
#include <mod_fraud.h>

/**
nFstPrvtDataLen: used for the timeout period
nTrdPrvtDataLen,sTrdPrvtData: used for response


Check the locks/unlocks, looks there's deadlocks
**/

extern TSNodeStruct ListHeadFrd;

int  InsertFraudRequest(TSNodeData * data)
{
	TSNodeStruct *prev,
		*current; 
	pthread_mutexattr_t mattr;
	time_t TimeSys;
	char	sLine[MAX_LINE_TRC];

	trace_event("Start InsertFraudRequest ()",PROCESSING);


	prev = &ListHeadFrd;
	pthread_mutex_lock(&(prev->lock)); 

	while ((current = prev->Next) != NULL) 
	{ 
		pthread_mutex_lock(&current->lock); 
		if ( (memcmp(current->data.szFstKey, data->szFstKey, sizeof(data->szFstKey)) == 0)  ) 
		{ 
			pthread_mutex_unlock(&current->lock); 
			pthread_mutex_unlock(&prev->lock);
			trace_event("End   InsertFraudRequest (EEXIST)",PROCESSING);
			return(EEXIST); 
		} 

		pthread_mutex_unlock(&prev->lock);
		prev = current; 
	} 

	if ( (current = malloc(sizeof(TSNodeStruct))) == NULL)
	{
		pthread_mutex_unlock(&prev->lock); 
		trace_event("End   InsertFraudRequest (ENOMEM)",PROCESSING);
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
		trace_event("-- FRD:stPurgeTime = 0, Considering tPurgeTime--",PROCESSING);
		current->data.stPurgeTime.tv_sec = data->tPurgeTime;
		current->data.stPurgeTime.tv_usec = current->stCreationTime.tv_usec;
	}

	sprintf(sLine," FRD:Creation time: %lu:%lu - PurgeTime: %lu:%lu",current->stCreationTime.tv_sec,current->stCreationTime.tv_usec,
		current->data.stPurgeTime.tv_sec,current->data.stPurgeTime.tv_usec);
	trace_event(sLine,PROCESSING);


	memcpy ( current->data.szFstKey, data->szFstKey, sizeof(data->szFstKey));

	current->data.nFstBufLen = data->nFstBufLen;
	memcpy ( current->data.sFstBuf, data->sFstBuf, data->nFstBufLen);

	current->data.nSndBufLen = data->nSndBufLen;
	memcpy ( current->data.sSndBuf, data->sSndBuf, data->nSndBufLen);

	current->data.nFstPrvtDataLen = data->nFstPrvtDataLen;
	/*JOU20171107: comment the next section, because nFstPrvtDataLen is used for timeout period,
	 and not as a length of the sFstPrvtData buffer.*/
	/**
	if ( data->nFstPrvtDataLen != 0)
		memcpy ( current->data.sFstPrvtData, data->sFstPrvtData, 
		data->nFstPrvtDataLen);
	*/
	current->data.nSndPrvtDataLen = data->nSndPrvtDataLen;
	if ( data->nSndPrvtDataLen != 0)
		memcpy ( current->data.sSndPrvtData, data->sSndPrvtData, 
		data->nSndPrvtDataLen);

	current->data.nTrdPrvtDataLen = data->nTrdPrvtDataLen;
	/*if ( data->nTrdPrvtDataLen != 0)*/
	memcpy ( current->data.sTrdPrvtData, data->sTrdPrvtData, 
		strlen(data->sTrdPrvtData));

	current->Next = NULL;
	pthread_mutexattr_init(&mattr);
	pthread_mutex_init( &(current->lock) , &mattr);
	pthread_mutexattr_destroy(&mattr);

	prev->Next = current;

	pthread_mutex_unlock(&prev->lock); 
	trace_event("End   InsertFraudRequest (OK)",PROCESSING);
	return(OK); 
}
/** JOU20170717
* 
*
*/
int  FetchFraudRequest ( TSNodeData * data)
{ 
	TSNodeStruct *prev,
		*current; 
	time_t TimeSys;
	struct	timeval		stTimeSys;
	char   sLine[MAX_LINE_TRC];

	trace_event("Start FetchFraudRequest ()",PROCESSING);
	prev = &ListHeadFrd;
	
	
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

			/*data->nFstPrvtDataLen = current->data.nFstPrvtDataLen ;
			if ( data->nFstPrvtDataLen != 0)
				memcpy ( data->sFstPrvtData, current->data.sFstPrvtData, 
				data->nFstPrvtDataLen);*/

			data->nSndPrvtDataLen = current->data.nSndPrvtDataLen ;
			if ( data->nSndPrvtDataLen != 0)
				memcpy ( data->sSndPrvtData, current->data.sSndPrvtData, 
				data->nSndPrvtDataLen);

			data->nTrdPrvtDataLen = current->data.nTrdPrvtDataLen ;
			if ( data->nTrdPrvtDataLen != 0)
				memcpy ( data->sTrdPrvtData, current->data.sTrdPrvtData, 
				data->nTrdPrvtDataLen);


			free(current);
			time(&TimeSys);
			gettimeofday(&stTimeSys,NULL);
			trace_event("#######> nice piece of cake",PROCESSING);

			/*JOU20170717  : remove "reponse tardive" section*/
		} 
		pthread_mutex_unlock(&prev->lock);
		prev = current; 
	}
	pthread_mutex_unlock(&prev->lock); 
	trace_event("End   FetchFraudRequest (ENOEXIST)",PROCESSING);
	return(ENOEXIST); 
}

int  UpdateFraudRequest (TSNodeData * data, char* sResponseCode)
{ 
	TSNodeStruct *prev,
		*current; 
	time_t TimeSys;
	struct	timeval		stTimeSys; 
	char   sLine[MAX_LINE_TRC];

	trace_event("Start UpdateFraudRequest ()",PROCESSING);
	prev = &ListHeadFrd;


	pthread_mutex_lock(&prev->lock); 
	while ((current = prev->Next) != NULL) 
	{
		pthread_mutex_lock(&current->lock); 
		if ( (memcmp(current->data.szFstKey, data->szFstKey, sizeof(data->szFstKey)) == 0)  )
		{ 
			/*JOU20170718: it's the node in the list that should be set*/
			/*
			memcpy(data->sTrdPrvtData,sResponseCode,3);
			data->nTrdPrvtDataLen = 3;
			*/
			memcpy(current->data.sTrdPrvtData,sResponseCode,3);
			current->data.nTrdPrvtDataLen = 3;
			
			pthread_mutex_unlock(&current->lock);
			pthread_mutex_unlock(&prev->lock); 
			sprintf(sLine," sResponseCode: %s", current->data.sTrdPrvtData);
			trace_event(sLine,PROCESSING);
			trace_event("End   UpdateFraudRequest (OK)",PROCESSING);
			return(OK); 

		} 
		pthread_mutex_unlock(&current->lock);
		prev = current; 
	}

	pthread_mutex_unlock(&prev->lock); 
	trace_event("End   UpdateFraudRequest (ENOEXIST)",PROCESSING);
	return(ENOEXIST); 
}

int  TryFetchFraudResponse (TSNodeData * data, char* sResponseCode, unsigned int* nRemainingTime)
{ 
	TSNodeStruct *prev,
		*current; 
	time_t TimeSys;
	struct	timeval		stTimeSys;
	struct	timeval		stTimeOutTime;
	char   sLine[MAX_LINE_TRC];
	unsigned int nSleepTime;
	int			nResult;


	trace_event("Start FetchFraudResponse ()",PROCESSING);
	prev = &ListHeadFrd;


	pthread_mutex_lock(&prev->lock); 
	while ((current = prev->Next) != NULL) 
	{
		pthread_mutex_lock(&current->lock); 
		if ( (memcmp(current->data.szFstKey, data->szFstKey, sizeof(data->szFstKey)) == 0)  )
		{ 
			/*Response has not yet been received */
			if( current->data.nTrdPrvtDataLen == 0 )
			{
				if( current->data.nFstPrvtDataLen > 0 )
				{
					/*check the remaining time and sleep*/
					gettimeofday(&stTimeSys,NULL);
					stTimeOutTime.tv_sec	= current->data.stPurgeTime.tv_sec - (stTimeSys.tv_sec - EPOCH_TIME_ADJUSTMENT);
					if( current->data.stPurgeTime.tv_usec < stTimeSys.tv_usec )
					{
						stTimeOutTime.tv_sec = stTimeOutTime.tv_sec - 1;
						stTimeOutTime.tv_usec	= current->data.stPurgeTime.tv_usec - stTimeSys.tv_usec + 1000000;
					}
					else
					{
						stTimeOutTime.tv_usec	= current->data.stPurgeTime.tv_usec - stTimeSys.tv_usec;
					}

					nSleepTime = stTimeOutTime.tv_sec*1000000 + stTimeOutTime.tv_usec;
					pthread_mutex_unlock(&current->lock);
					pthread_mutex_unlock(&prev->lock);
					(*nRemainingTime) = nSleepTime;
					return NOK;
				}
			}

			

			
			prev->Next = current->Next;

			pthread_mutex_unlock(&current->lock); 
			pthread_mutex_destroy(&(current->lock));
			pthread_mutex_unlock(&prev->lock);

			data->nFstBufLen = current->data.nFstBufLen;
			memcpy ( data->sFstBuf, current->data.sFstBuf, current->data.nFstBufLen);
			data->nSndBufLen = current->data.nSndBufLen;
			memcpy ( data->sSndBuf, current->data.sSndBuf, current->data.nSndBufLen);
			data->tPurgeTime = current->data.tPurgeTime;

			data->nFstPrvtDataLen = current->data.nFstPrvtDataLen ;
			/*JOU20170718: sFstPrvtData is not used*/
			/*if ( data->nFstPrvtDataLen != 0)
				memcpy ( data->sFstPrvtData, current->data.sFstPrvtData, data->nFstPrvtDataLen);
			*/
			data->nSndPrvtDataLen = current->data.nSndPrvtDataLen ;
			if ( data->nSndPrvtDataLen != 0)
				memcpy ( data->sSndPrvtData, current->data.sSndPrvtData, data->nSndPrvtDataLen);

			data->nTrdPrvtDataLen = current->data.nTrdPrvtDataLen ;
			if ( data->nTrdPrvtDataLen != 0)
				memcpy ( data->sTrdPrvtData, current->data.sTrdPrvtData, data->nTrdPrvtDataLen);

			
			time(&TimeSys);
			gettimeofday(&stTimeSys,NULL);

			if( current->data.nTrdPrvtDataLen > 0 )
			{
				memcpy(sResponseCode,current->data.sTrdPrvtData,3);
				nResult = OK;
				

			}
			else
			{
				/*default action is already saved here*/
				memcpy(sResponseCode,current->data.sTrdPrvtData,3);
				nResult = NOK;		
					
			}

			free(current);
			
			if(nResult == OK)
				trace_event("End   FetchFraudResponse (OK)",PROCESSING);
			else
				trace_event("End   FetchFraudResponse (NOK)",PROCESSING);
			return(nResult); 

		} 
		pthread_mutex_unlock(&prev->lock);
		prev = current; 
	}
	pthread_mutex_unlock(&prev->lock); 
	trace_event("End   FetchFraudResponse (ENOEXIST)",PROCESSING);
	return(ENOEXIST); 
}


int  FetchFraudResponse (TSNodeData * data, char* sResponseCode)
{
	int nResult;
	unsigned int	nRemainingTime;
	char sRespCode[4];
	
	memset(sRespCode, 0, sizeof(sRespCode));

	nResult = TryFetchFraudResponse(data,sResponseCode,&nRemainingTime);
	switch(nResult)
	{
	case OK:
	case ENOEXIST:
		return nResult;
	}
	
	memcpy(sRespCode, sResponseCode, 3);

	usleep(nRemainingTime + 1);

	nResult = TryFetchFraudResponse(data,sResponseCode,&nRemainingTime);
	
	
	if(nResult == ENOEXIST) 
	{
		memcpy(sResponseCode,sRespCode, 3);
	}
		

	return nResult;

}


int  FreeTimeOutFraudRequest ()
{
	TSNodeStruct *prev, *current; 
	char			   sLine[MAX_LINE_TRC];
	
	time_t TimeSys;
	struct	timeval		stTimeSys;

	prev = &ListHeadFrd;


	time(&TimeSys);
	gettimeofday(&stTimeSys,NULL);
	pthread_mutex_lock(&prev->lock); 
	while ((current = prev->Next) != NULL) 
	{

		pthread_mutex_lock(&current->lock); 
		if ( current->data.stPurgeTime.tv_sec < (stTimeSys.tv_sec - EPOCH_TIME_ADJUSTMENT)  || 
			( current->data.stPurgeTime.tv_sec == (stTimeSys.tv_sec - EPOCH_TIME_ADJUSTMENT) && stTimeSys.tv_usec > current->data.stPurgeTime.tv_usec) )
		{ 
			sprintf(sLine,"PurgeTime: %lu:%lu | SysTime: %lu:%lu - ", current->data.stPurgeTime.tv_sec,current->data.stPurgeTime.tv_usec,
				stTimeSys.tv_sec,stTimeSys.tv_usec);
			trace_event(sLine,PROCESSING);

			prev->Next = current->Next;
			pthread_mutex_unlock(&current->lock); 
			pthread_mutex_destroy(&(current->lock));

			
			free(current);

		}
		else
		{
			pthread_mutex_unlock(&prev->lock);
			prev = current;
		}
	}


	pthread_mutex_unlock(&prev->lock); 
	return(OK);
}

