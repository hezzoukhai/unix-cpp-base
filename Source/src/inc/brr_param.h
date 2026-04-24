#ifndef BRIDGE_PARAM__H
#define BRIDGE_PARAM__H

#define MAX_BRIDGE_CLIENT		12
#define MAX_BRIDGE_THREAD		40
#define MAX_RESOURCES			300
#define MAX_RECORDS_NBR			10
#define MAX_MSG_NBR				10
#define MAX_THREAD_MSG			2*MAX_MSG_NBR
#define MAX_CONN_NBR			10

/*
* This struct is used for bridge as a server for incoming connections
*/
typedef struct
{
	int					free;
	char				status;
	int					fd;
	int					flag_bal;
	int					flag_commande;
	pthread_t			tid;  
	pid_t				sys_tid;
	pthread_mutex_t		kMutex;
	pthread_cond_t		kCond ;

	char				sApplicationName[512];
	char				sApplicationId[256];
	char				sNodeId[4  + 1];
	char				sDBServId[4 + 1];
	pthread_mutex_t		SafManageMutex;
	pthread_cond_t		SafManageCond;
	pthread_mutex_t 	MsgManageMutex;
	pthread_cond_t		MsgManageCond;
	int					nSafResponse;
	int					nChannelIndex;/*ICH20160804*/
	int					nNbLostEchoTests;
	time_t				tLastUsage;/*PLUTONL-4900*/

} TSBridgeCtx;


typedef struct {
	int					fd;/*Used for signle connexion bridge routing*/
	int 				connNbr;
	char				status;
	char				connexion;/*ICH20160726*/
	int					nIndex;
	char				sNodeId[4 + 1];
	char				sHostAddr[512];
	int					nPort;
	int					bServices;
	char                sDBServId[4 + 1];/*AAB31052016*/
	pthread_mutex_t		SafManageMutex;
	pthread_cond_t		SafManageCond;
	pthread_mutex_t		ConnManageMutex;/*ICH20160726 to share connexion*/
	pthread_cond_t		ConnManageCond;/*ICH20160726*/
	int					nSafResponse;
	time_t				tLastUsage;/*PLUTONL-4900*/

} TSBridgeChannel;

/*ICH15072016*/
typedef struct {
	char                sResourceId[6 + 1];
	char				sResourceLive[1 + 1];
	char				sStatus[1 + 1];

} TSResStatus;






extern TSBridgeCtx		tab_BridgeCtx	[ ];
extern TSResStatus		tab_ResStatus	[ ];
extern TSBridgeChannel	tab_Channels	[ ];
extern int				g_nNbChannels;
extern int				g_nNbConnexion;
extern int				g_nNbResources;
extern char     		InterfaceMode[];


#endif
