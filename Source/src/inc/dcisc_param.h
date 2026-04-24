#ifndef HPS_DCISC_PARAM_H
#define HPS_DCISC_PARAM_H

#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <resources.h>

/* File Update Constants */
/*
#define  INSERT  1
#define  UPDATE  2
*/

/* Resource Common Parameters ***/
/*
char  BankCode           [ 6 +1];
char  CenterCode         [ 2 +1];
char  NetworkCode        [ 2 +1];
*/
/* Tak */
/*
char  TakLmk             [ 256 +1];
char  TakZmk             [ 256 +1];
char  CheckValueTak      [ 256 +1];
*/
/* Zpk */
/*
char  ZpkLmk             [ 256 +1];
char  ZpkZmk             [ 256 +1];
char  CheckValueZpk      [ 256 +1];
*/

/** DCISC interface Params from P7_DCISC_RESOURCE_PARAM **/
extern char  ForwardingId   [ ];
extern char  OriginatorId   [ ];
extern char  DestinationId  [ ];
extern char  AcquirerId   [ ];




/* Resource Echo Test Timer */
int   EchoTestTimer;
int   MaxSendEchoTest;

#define 	DCISC_ACQUIRER_ID                  "D01"
#define 	DCISC_FORWARDING_ID                "D02"
#define 	DCISC_DESTINATION_ID               "D03"
#define 	DCISC_ORIGINATOR_ID                "D04"


#endif /*** HPS_DCISC_PARAM_H ***/
