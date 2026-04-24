#ifndef HPS_EURO_PARAM_H
#define HPS_EURO_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* Resource Common Parameters ***/
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];
char  ReversalGeneration [ 1 +1];

/* Resource Specific Parameters read from P7_EPSS_RESOURCE_PARAM table */
char  CountryCode  [ 3 +1];
char  AlphaCode3   [ 3 +1];
char  AlphaCode2   [ 2 +1];
char  BankLocation [11 +1];
char  AcquirerId   [11 +1];
char  ForwId       [11 +1];

char ConnectDelay [ 3 + 1];
char SignOnDelay  [ 3 + 1];
char Timer        [ 3 + 1];
char TraficFlags  [ 2 + 1];
char TestProdFlags[ 2 + 1];

char AcquirerBank [ 6 + 1 ];
char ReceivingInst[ 11 + 1 ];
char TransOriginIdCode [ 11 +  1 ];

#endif /*** HPS_EURO_PARAM_H ***/
