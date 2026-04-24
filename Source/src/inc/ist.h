/*
-----------------------------------------------------------------------------------------------------------------------------------------
-- Description : 
	-------------------------------------------------------------------------------------------------------------------------------------
	
-----------------------------------------------------------------------------------------------------------------------------------------
-- Modification History : 
	-------------------------------------------------------------------------------------------------------------------------------------
	-- Version    Date          	Author       		Jalon                     Activity                          Comment
	-------------------------------------------------------------------------------------------------------------------------------------
	-- V3.0.0     29/12/2014    	HPS                                                                           	Initial version
-----------------------------------------------------------------------------------------------------------------------------------------
*/

#ifndef IST_H
#define IST_H


#ifdef SOLARIS
   #include <thread.h>
#endif
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
/*#include <string.h>*/
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#ifdef SOLARIS
#include <limits.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
/*
#include <sys/stream.h>
*/
#include <sys/socketvar.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>


#include <sqlca.h>
#include <sqlcpr.h>


#include <define.h>
#include <pwrmacros.h>
#include <resources.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <event_log.h>
#include <thread_var.h>
#include <param_arg.h>
#include <main_thr.h>
#include <list_thr.h>
#include <queue.h>
#include <line_com.h>
#include <struct_line.h>
#include <struct_time.h>
#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <hsm_inc.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <tlv_autho.h>
#include <iso_hps.h>
#include <service.h>
#include <iso_ist.h>
#include <ist_fields.h>
#include <ist_tcp.h>
#include <p7_services.h>

#define IST_ACQ_INST_ID          "06000001"
#define IST_FWD_INST_ID          "06010001"
#define A  0 
#define M  1 
#define C  2 
#define MR 3 
#define CR 4 

#define PROCESSED     1 
#define NOT_PROCESSED 2 
#define WAITING_UPD   3 


typedef struct STransType {
   int           nNwMsgType;
   char          *szNwTransType;
   char          *szIsoTransType;
   char          *szIsoFuncCode;
} TSTransType;

typedef struct SMsgBitMap {
   int         nPresenceFlag;
} TSMsgBitMap;


extern void * thread_ECHOTEST(void * arg);

extern int        nFdConnect;
extern void       DumpGlobals (void);

#endif


