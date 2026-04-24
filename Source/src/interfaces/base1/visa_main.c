#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <define.h>
#include <visa_param.h>
#include <main_thr.h>
#include <thread_var.h>
#include <errno.h>
#include <resources.h>


/******************************************************************************/

char		NodeId[NODE_ID_LEN + 1];
char		ResourceId[RESOURCE_ID_LEN + 1];
char		ResourceInternalId[RESOURCE_INTERNAL_ID_LEN + 1];
char		ResourceName[RESOURCE_NAME_LEN + 1];
char		ResourceStatus[RESOURCE_STATUS_LEN + 1];
char 		CommsId[RES_COMMS_ID_LEN + 1];
char 		ProtocolId[RES_PROTO_ID_LEN + 1];
char 		DeviceName[RES_DEV_NAME_LEN + 1];
char 		RemoteAppName[RES_REMOTE_APP_NAME_LEN + 1];
char 		RemoteAppId[RES_REMOTE_APP_ID_LEN + 1];
char 		LocalAppName[RES_LOCAL_APP_NAME_LEN + 1];
char 		LocalAppId[RES_LOCAL_APP_ID_LEN + 1];
int 		nTimerAcq;
int 		nIssAcqDelta;
int 		MinTroublesInd;
int 		MaxTroublesInd;
int 		max_thread;
int 		nUserThread;
char		AcquirerBank[ACQUIRER_BANK_LEN + 1];
int	  		EchoTestTimer;  /*HAL26082020 PLUTONL-2185*/

char  StationId    [ 6 +1];
char  CountryCode  [ 3 +1];
char  AlphaCode3   [ 3 +1];
char  AlphaCode2   [ 2 +1];
char  BankLocation [11 +1];
char  AcquirerId   [11 +1];
char  ReversalGeneration [ 1 +1];
char  sInterfaceMode [ 1 + 1];

char  BASE1_REJ_NON3DS; /*NAB07042020 PLUTONL-1477*/
int   g_nAuthReplyBackPeriod; /*ZKO10032022 PLUTONL-4411*/

char  g_pris_connect_mode[ 1 + 1 ];
char	g_sCaptureMsg			[4 + 1];/*IBO20200622 PLUTONL-1967*/

/******************************************************************************/

int main(int argc, char *argv[])
{
	int             result;
	char*			sPtr;


	EnableThreads();
	memset(sInterfaceMode,'\0',sizeof(sInterfaceMode));
	memcpy (sInterfaceMode, "A", 1);

	/* Get resource id from command line argument */
	if (argc != 2) 
	{
		printf("Usage : <ProgramName> <ResourceId> ; OR : <ProgramName> - <v> \n");
		exit(0);
	}

    if (ProcessOpt(argc, argv) == OK )
    {
	   exit(0);
    }

	memset(NodeId, '\0', sizeof(NodeId));
	memset(ResourceId, '\0', sizeof(ResourceId));
	memset(ResourceInternalId, '\0', sizeof(ResourceInternalId));
	memset(ResourceName, '\0', sizeof(ResourceName));
	memset(ResourceStatus, '\0', sizeof(ResourceStatus));
	memset(CommsId, '\0', sizeof(CommsId));
	memset(ProtocolId, '\0', sizeof(ProtocolId));
	memset(DeviceName, '\0', sizeof(DeviceName));
	memset(RemoteAppName, '\0', sizeof(RemoteAppName));
	memset(RemoteAppId, '\0', sizeof(RemoteAppId));
	memset(LocalAppName, '\0', sizeof(LocalAppName));
	memset(LocalAppId, '\0', sizeof(LocalAppId));

	memset(StationId, '\0', sizeof(StationId));
	memset(CountryCode, '\0', sizeof(CountryCode));
	memset(AlphaCode3, '\0', sizeof(AlphaCode3));
	memset(AlphaCode2, '\0', sizeof(AlphaCode2));
	memset(BankLocation, '\0', sizeof(BankLocation));
	memset(AcquirerId, '\0', sizeof(AcquirerId));
	memset(ReversalGeneration, '\0', sizeof(ReversalGeneration));
	memset(sInterfaceMode, '\0', sizeof(sInterfaceMode));
	memset(g_pris_connect_mode, '\0', sizeof(g_pris_connect_mode));
	memset(g_sCaptureMsg, '\0', sizeof(g_sCaptureMsg));/*IBO20200622 PLUTONL-1967*/


	strncpy(ResourceId, argv[1], RESOURCE_ID_LEN);
	debug_option = 0;


	ThreadMaskSignals();

	

	/**** Initialisation du Module VISA **/
	result = InitModule(ResourceId);

	sPtr = getenv("DEBUG_MODE");
	
	if (sPtr && sPtr[0] == '1')
	{
		trace_event("In debuging_mode", TRACE);
		debug_option = 1;
	}

	/**** Creation du Thread de traitement des Messages BAL ***/
	if (pthread_create(&(tab_ThrInfo[BAL_THREAD_ID].tid), NULL,
		thread_wait_BAL, NULL) < 0) {
			fprintf(stderr, "pthread_create Error %d\n", errno);
			exit(0);
	}
	/**** Creation du Thread de Traitement des messages ligne**/
	if (pthread_create(&(tab_ThrInfo[LINE_THREAD_ID].tid), NULL,
		thread_wait_LINE, NULL) < 0) {
			fprintf(stderr, "pthread_create Error %d\n", errno);
			exit(0);
	}
	/**** Creation du Thread  De controle  de  l'interface  ***/
	if (pthread_create(&(tab_ThrInfo[CTL_THREAD_ID].tid), NULL,
		thread_CTL, NULL) < 0) {
			fprintf(stderr, "pthread_create Error %d\n", errno);
			exit(0);
	}
	/**** Creation du Thread  De Detection de TimeOut       ***/
	if (pthread_create(&(tab_ThrInfo[TIME_THREAD_ID].tid), NULL,
		thread_TIMEOUT, NULL) < 0) {
			fprintf(stderr, "pthread_create Error %d\n", errno);
			exit(0);
	}
	/*HAL26082020 PLUTONL-2185*/
	/**** Creation du Thread  Echo Test Periodique ****/
	if (EchoTestTimer > 0 )
	{
   		if (pthread_create(&(tab_ThrInfo[SIGNIN_THREAD_ID].tid), NULL,thread_signin, NULL) < 0)
   		{
      		fprintf(stderr,"pthread_create Error %d\n", errno);
      		exit(0);
   		}
  	} 
	/*HAL26082020 PLUTONL-2185*/
	/*sleep(4);*/ /*KCH Crash Base 1 inteface under Linux*/
	/**** Creation du Thread de traitement des Messages StandIn ***/
	if (pthread_create(&(tab_ThrInfo[SAF_THREAD_ID].tid), NULL,
		thread_SAF, NULL) < 0) {
			fprintf(stderr, "pthread_create Error %d\n", errno);
			exit(0);
	}


	while (1)
		pause();
}
