#ifndef MQ_PARAM___H
#define MQ_PARAM___H





#define Q_MANAGER_NM_MAX_LEN	64
#define Q_NM_MAX_LEN			64
#define Q_SRV_MAX_SERVICES		32
#define MAX_QUEUES				10

#define HDR_MAGIC_VALUE			"MQHDR"		/* header Magic value */
#define Q_SRV_VER_10			"10"
#define Q_SRV_CURR_VER			Q_SRV_VER_10

#define Q_HDR_MV_OFST			0
#define Q_HDR_VER_OFST			5
#define Q_HDR_SERVICE_OFST		7
#define Q_HDR_SERVICE_TP_OFST	9
#define Q_HDR_HDR_LEN_OFST		10
#define Q_HDR_MSG_LEN_OFST		13
#define Q_HDR_MSG_ID_OFST		17

#define Q_HDR_MV_LEN			5
#define Q_HDR_VER_LEN			2
#define Q_HDR_SERVICE_LEN		2
#define Q_HDR_SERVICE_TP_LEN	1
#define Q_HDR_HDR_LEN_LEN		3
#define Q_HDR_MSG_LEN_LEN		4
#define Q_HDR_MSG_ID_LEN		8


typedef int (*fnQSrvService)(int nIndexCtx,char* sMsgIn, char* sMsgOut);


typedef struct SMQIncChannel
{
	char			q_manager[Q_MANAGER_NM_MAX_LEN + 1];
	char			q_name[Q_NM_MAX_LEN + 1];
	char			q_outg[Q_NM_MAX_LEN + 1];
	char			proc_type;
	char			proc_container[64 + 1];
	char			proc_entry[64 + 1];
	void*			hServiceHandle;
	fnQSrvService	fnService;
	int				nChannelIndex;
	int				nOutgChannelIndex;
}TSMQIncChannel;

typedef struct SMQOutgChannel
{
	char			q_manager[Q_MANAGER_NM_MAX_LEN + 1];
	char			q_name[Q_NM_MAX_LEN + 1];
	char			q_inc[Q_NM_MAX_LEN + 1];
	int				services;
	int				nChannelIndex;
	int				nIncChannelIndex;	
}TSMQOutgChannel;

typedef struct 
{
	char			q_manager[Q_MANAGER_NM_MAX_LEN + 1];
	char			q_name[Q_NM_MAX_LEN + 1];
	char			traffic_flag;
	char			status;
	MQCNO			Connect_options;
	MQCD			ClientConn;
	MQHCONN			Hcon; 
	MQHOBJ			Hobj;
} TSMQChannels;



typedef struct 
{
	struct	timeval			stPurgeTime;
	
	char		szKey		[128];
	char		sData		[256];
	int		nDataLen;
}TSQListData;

typedef struct  QListStruct
{ 
	struct	timeval			stCreationTime;
	TSQListData				data;
	struct QListStruct*		Next;
	pthread_mutex_t			lock; 
} TSQListStruct;

extern int				nNbIncQueue;
extern int				nNbOutgQueue;
extern int				nNbQueues;
extern TSMQIncChannel	tab_IncChannels[];
extern TSMQOutgChannel	tab_OutgChannels[];
extern TSMQChannels		tab_MQChannels[];

#define Q_CHNL_OPEN				'O'
#define Q_CHNL_ONLINE			'C'
#define Q_CHNL_OFFLINE			'F'
#define Q_CHNL_UNDEF			'U'


#define Q_PROC_TP_USR_PROC		'U'
#define Q_PROC_TP_DB_PROC		'P'
#define Q_PROC_TP_FWD			'F'



#endif

