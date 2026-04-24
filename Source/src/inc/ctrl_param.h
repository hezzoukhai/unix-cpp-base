#ifndef CTRL_PARAM_H
#define CTRL_PARAM_H
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

/* Resource Specific Parameters read from P7_CIS_RESOURCE_PARAM table */
char  StationId    [ 6 +1];
char  CountryCode  [ 3 +1];
char  AlphaCode3   [ 3 +1];
char  AlphaCode2   [ 2 +1];
char  BankLocation [11 +1];
char  AcquirerId   [11 +1];
char  ReversalGeneration [ 1 +1];


char AcquirerBank [ 6 + 1 ];

int FdBal[PTHREAD_THREADS_MAX];
int nBal1;
int nBal2;
int nBal3;
int nBal4;
#endif /*** CTRL_PARAM_H ***/
