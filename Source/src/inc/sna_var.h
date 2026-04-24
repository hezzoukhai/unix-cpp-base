#ifndef HPS_SNA_VAR_H
#define HPS_SNA_VAR_H
#include <stdio.h>
#include <comms_param.h>
#include <lua_c.h>

#define  MAX_SNA_TERM     4

#define  SNA_TERM_WRITER    0
#define  SNA_TERM_READER    1  

#define  SNA_BIND           2000
#define  SNA_UNBIND         2001  
#define  SNA_SDT            2002  

char    param_luname   [ 8 + 1];

typedef struct SSnaInfo
{
    unsigned long lua_sid; 
    char  lu_name    [ 8 + 1];
    int   data_read;
    int   bind_read;
    int   unbind_read;
    int   sdt_read;
    int   bid_read;
    int   failure;
    int   rqd;
    int   reinit;
    char  szBuffer[LG_MAX];
    int   nLength;
    unsigned long sense;
    LUA_VERB_RECORD  verb;
} TSSnaInfo;

TSSnaInfo tab_SnaInfo [MAX_SNA_TERM];

#define WAITING      100

#endif /*** HPS_SNA_VAR_H ***/
