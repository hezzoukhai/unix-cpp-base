#include <stdio.h>
#include <pthread.h>
#include <list_thr.h>

void InitNodeData( TSNodeData *data)
{
   data->tPurgeTime = 0;
   memset((char*)&data->stPurgeTime, 0 , sizeof(data->stPurgeTime));
   data->nFstBufLen = 0;
   data->nSndBufLen = 0;
   data->nFstPrvtDataLen = 0;
   data->nSndPrvtDataLen = 0;
   data->nTrdPrvtDataLen = 0;
   memset(data->szFstKey	, '\0', sizeof (data->szFstKey));
   memset(data->szSndKey	, '\0', sizeof (data->szSndKey));
   memset(data->szTrdKey	, '\0', sizeof (data->szTrdKey));
   memset(data->szFrtKey	, '\0', sizeof (data->szFrtKey));
   memset(data->szFftKey	, '\0', sizeof (data->szFftKey));
   memset(data->sFstBuf		, '\0', sizeof (data->sFstBuf));
   memset(data->sSndBuf		, '\0', sizeof (data->sSndBuf));
   memset(data->sFstPrvtData	, '\0', sizeof (data->sFstPrvtData));
   memset(data->sSndPrvtData	, '\0', sizeof (data->sSndPrvtData));
   memset(data->sTrdPrvtData	, '\0', sizeof (data->sTrdPrvtData));
   data->msgId = 0;
}

void InitList()
{
   pthread_mutexattr_t mattr;

   ListHeadAcq.Next = NULL;
   ListHeadIss.Next = NULL;

   pthread_mutexattr_init(&mattr);
   pthread_mutex_init( &(ListHeadAcq.lock) , &mattr);
   pthread_mutex_init( &(ListHeadIss.lock) , &mattr);
   pthread_mutexattr_destroy(&mattr);

   InitNodeData( &(ListHeadAcq.data));
   InitNodeData( &(ListHeadIss.data));
}
