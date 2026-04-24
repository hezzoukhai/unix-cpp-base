#ifndef CBCOM_NODE_LIST_H
#define CBCOM_NODE_LIST_H

#include <pthread.h>
#include <sys/types.h>
#include <define.h>
#include <errno.h>


typedef struct SCBComNodeData
{
     time_t		tPurgeTime;
     char		szKey		[    128];
     int		nBufLen;
     char		sBuffer		[256];
}TSCBComNodeData;

typedef struct SCBComNodeStruct
{ 
     time_t						tCreationTime;
     TSCBComNodeData			data;
     struct SCBComNodeStruct	*Next;
     pthread_mutex_t			lock; 
} TSCBComNodeStruct;

typedef struct SCBComTimeExchange
{
    char				sens;
    TSCBComNodeData		sData;
}TSCBComTimeExchange;

void InitCBComList();
void InitCBComNodeData(TSCBComNodeData * data);
int  InsertCBComNode(char sens, TSCBComNodeData * data);
int  FetchCBComNode (char sens, TSCBComNodeData * data);
int  FreeTimeOutCBComNode (char sens);
void GetPI24PurgeTime( time_t * nPurgeTime);

#endif /* CBCOM_NODE_LIST_H */



