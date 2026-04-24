#ifndef __TCP_CB2A_H__
#define __TCP_CB2A_H__

#include <string.h>
#include <stdlib.h>

#ifdef SSL_MODE

#define InitSrvComms	InitSrvCommsSSL
#define AcceptCallSrv	AcceptCallSrvSSL
#define ReadLineMsg		ReadLineMsgSSL
#define WriteLineMsg	WriteLineMsgSSL
#define StopCltComms	StopCltCommsSSL

#endif

#endif
