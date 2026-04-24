#ifndef BRIDGE_PARAM__H
#define BRIDGE_PARAM__H

#define BRS_MAX_BRIDGE_CLIENT			40
#define BRS_MAX_BRIDGE_THREAD			40
#define BRS_MAX_RESOURCES				300
#define BRS_MAX_RECORDS_NBR				10
#define BRS_MAX_MSG_NBR					10
#define BRS_MAX_THREAD_MSG				(2*BRS_MAX_MSG_NBR)
#define BRS_MAX_CONN_NBR				10

typedef struct
{
	int					free;				/* flag to indicate if current slot is free or busy */
	char				status;				/* flag to indicate if the connection is connected	*/
	int					fd;					/* fd of the client connection						*/
	pthread_t			tid;  
	pid_t				sys_tid;
	pthread_mutex_t		kMutex;					/* Mutext to manage waiting for new requests		*/
	pthread_cond_t		kCond ;
	int					nSafStatus;				/* flag to indicates status of the saf message sent to the client	*/
	char				sApplicationName[512];
	char				sApplicationId[256];
	char				sNodeId[4  + 1];
	char				sDBServId[4 + 1];
	int					nDBServBridgeIndex;		/* index into tab_DBServBridge(maybe to be removed, not needed after all	*/


} TSBridgeCtx;


typedef struct
{

	pthread_mutex_t		SafManageMutex;
	pthread_cond_t		SafManageCond;
	pthread_mutex_t 	MsgManageMutex;
	pthread_mutex_t		kMutex;

	int					free;
	int					nSafResponse;
	char				sDBServId[4  + 1];
	/*char				sApplicationName[512];*/
	int					nbrMsg;
	time_t				nMsgTimestamp;			/* timestamp when did we send saf messages to client	*/

} TSDBServBridge;


typedef struct {
	
	int 				tabFd[BRS_MAX_CONN_NBR];	/*Used for multi Connexion bridg synchro*/
	int 				connNbr;					/* nbr of TCPIP connection to server		*/
	char				status;
	/*int					nIndex;*/
	char				sNodeId[4 + 1];
	char				sHostAddr[512];
	int					nPort;
	char                sDBServId[4 + 1];/*AAB31052016*/
	/*pthread_mutex_t		SafManageMutex;
	pthread_cond_t		SafManageCond;*/
	/*pthread_mutex_t		ConnManageMutex;
	pthread_cond_t		ConnManageCond;*/
	/*int					nSafResponse;*/

} TSBridgeChannel;


typedef struct {
	char                sMti[4 + 1];
	char				sStan[6 + 1];
	char				sResponseCode[3 + 1];
	char				sMessage[LG_MAX];
	char				sCardNumber[24 + 1];
	int					sendCounter;
	char				status;
} TSDbservSAF;


typedef struct {
	char*	sBigBuffer;
	int		nBBOffset;
	int		nCurrMsgNo;
} TSDbservBB;


extern TSBridgeCtx		tab_BridgeCtx	[ ];
/*extern TSBridgeNode		tab_BridgeNode	[ ];*/
extern TSDBServBridge	tab_DBServBridge[ ];
extern TSBridgeChannel	tab_Channels	[ ];
extern int				g_nNbChannels;
extern int				g_nNbConnexion;
extern int				g_nNbResources;
extern char     		InterfaceMode[];

extern TSDbservBB		tab_BBData[];

#endif
