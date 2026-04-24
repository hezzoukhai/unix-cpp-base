#ifndef HPS_LIST_THR_H
#define HPS_LIST_THR_H
#include <pthread.h>
#include <sys/types.h>
#include <define.h>
#include <errno.h>

#define EPOCH_TIME_ADJUSTMENT	1000000000

typedef struct SNodeData
{
     time_t			tPurgeTime;
	 struct	timeval stPurgeTime;		/*EBE130306*/
     char		szFstKey	[    56];
     char		szSndKey	[    56];
     char		szTrdKey	[    56];
     char		szFrtKey	[    56];
     char		szFftKey	[    56];
     int		nFstBufLen;
     char		sFstBuf		[LG_MAX];
     int		nSndBufLen;
     char		sSndBuf		[LG_MAX];
     int		nFstPrvtDataLen;
     char		sFstPrvtData	[    56];
     int  		nSndPrvtDataLen;
     char		sSndPrvtData	[    56];
     int		nTrdPrvtDataLen;
     char		sTrdPrvtData	[    56];
	 msg_id_t		msgId;		/*EBE130925:SWMON*/
}TSNodeData;

typedef struct SNodeStruct 
{ 
     time_t				tCreationTime;
	 struct	timeval		stCreationTime;		/*EBE130306*/
     TSNodeData         data;
     struct SNodeStruct *Next;
     pthread_mutex_t    lock; 
} TSNodeStruct;

TSNodeStruct ListHeadAcq;
TSNodeStruct ListHeadIss;

void InitList();
void InitNodeData(TSNodeData * data);
int  InsertRequest(char sens, TSNodeData * data);
int  FetchRequest (char sens, TSNodeData * data);
int  FreeTimeOutRequest (char sens);
void GetNextTimeOut(struct	timeval* stNextTimeOut);

#endif /* HPS_LIST_THR_H */
