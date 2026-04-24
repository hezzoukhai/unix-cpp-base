#ifndef HPS_DSCOVER_PARAM_H
#define HPS_DSCOVER_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

#define  INSERT  1
#define  UPDATE  2

 
#define 	DSCOVER_ACQUIRER_ID                  "R01"
#define 	DSCOVER_FORWARDING_ID                "R02"
#define 	DSCOVER_DESTINATION_ID               "R03"
#define 	DSCOVER_ORIGINATOR_ID                "R04"
 

char  CountryCode        [ 3 +1];
char  AlphaCode3         [ 3 +1];
char  AlphaCode2         [ 2 +1];
char  BankLocation       [11 +1];
char  AcquirerId         [11 +1];
char  ForwardingId       [11 +1];
char  OriginatorId       [11 +1];
char  DestinationId      [11 +1];
char  DiscoverVersionNbr [5 +1];
char  AcquirerBank       [ 6 + 1 ];

char  ReversalGeneration [ 1 +1]; 

char  DynamicKeyExchange [ 1 +1];
char  SignOnFlag         [ 1 +1];

int   EchoTestTimer;
int   MaxSendEchoTest;
 
 
 
#endif /*** HPS_DSCOVER_PARAM_H ***/
