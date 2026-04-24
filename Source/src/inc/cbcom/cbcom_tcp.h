/*
** <Name>
**  cbae_tcp.h
**
** <Descr>
**  general header file for "tcp" module for cbae interface
**
** <History>
**  Version       Date      Author         Comments
**  1.0.0.0       30Sept04  Y. OUADI       Initiale Version
**
** (C) 1999-2005 : Hightech Payment Systems
*/
#ifndef CBAE_TCP_H
#define CBAE_TCP_H

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <define.h>
#include <trace_macros.h>




/* - PowerCARD Modules Public Functions Members ---------------------------- */
extern int  InitCltCom    (char *sServerName, char *sPort);
extern int  InitSrvComms  (int nPort);
extern int  AcceptCall    (char calling_address[]);
extern int  AcceptCallSrv (char calling_address[]);
extern void StopCltCom    (int nFd);
extern int  ReadLineMsg   (int nFd, char *sBuffer, int *nLength);
extern int  WriteLineMsg  (int nFd, char *sBuffer, int nLength);


/* - Constantes Definition Section ----------------------------------------- */
#define DISCONNECT          99



#endif

