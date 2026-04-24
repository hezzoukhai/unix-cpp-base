#ifndef HPS_X25_VAR_H
#define HPS_X25_VAR_H
#include <comms_param.h>


#define  X25_MSG_DATA      0
#define  X25_MSG_DISCON    1

char   X25_DeviceName   [16 +1];
char   CallingAdress    [16 +1];
char   CalledAdress     [16 +1];
char   CallUserData     [16 +1];
char   ConnectMode;
int    nFdListen;
int    nFdConnect;

int    IsClientX25;

/*** To link Incomig Call and Accept ***/
int   x25_call_flags;
ulong x25_call_seqno;


#endif /*** HPS_X25_VAR_H ***/
