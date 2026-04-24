#ifndef HPS_CB2A_GLB_H
#define HPS_CB2A_GLB_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ipc.h>
#include <math.h>

#include <tools.h>
#include <dump.h>
#include <struct_line.h>
#include <param_arg.h>
#include <thread_var.h>
#include <queue.h>
#include <event_log.h>

#include <poscb2a_param.h>
#include <param_arg.h>
#include <thr_poscb2a.h>
#include <tlv_private.h>
#include <list_thr.h>
#include <ascii.h>
#include <define.h>
#include <iso_hps.h>
#include <tlv_autho.h>
#include <resources.h>
#include <tlv_fields.h>
#include <security_data.h>
#include <iso_aut_cb2a.h>
#include <cb2a_define.h>
#include <thr_poscb2a.h>
#include <poscb2a_param.h>
#include <tlv_cb2afields.h>
#include <iso_cb2a.h>
#include <tcp_cb2a.h>
#include <thr_poscb2a.h>
#include <cb2a_define.h>


typedef struct tlv_info_st
{						
   char    sTlvName[8];	
   int     nType;		
   int     nLength;
   char		*description;
}tlv_info_type;	

typedef struct msg_map_st {
	int		nFieldNo;
	char	info;
	int		nCondition;
} msg_map_t;

typedef struct msg_field_format_st {
	int		nFieldNo;
	char	format;
} msg_field_format_t;

typedef struct msg_subfield_format_st {
	char*	szTag;
	char	format;
} msg_subfield_format_t;



#include <cb2a/gen_tlv_add_data.h>
#include <cb2a/gen_tlv_add_data_aut.h>
#include <cb2a/gen_tlv_add_resp_data.h>
#include <cb2a/gen_tlv_add_resp_data_aut.h>
#include <cb2a/gen_tlv_cad_sq_data.h>
#include <cb2a/gen_tlv_nat_data.h>
#include <cb2a/gen_tlv_nat_data_aut.h>
#include <cb2a/gen_tlv_print_display_data.h>
#include <cb2a/gen_tlv_reserved_nat_data.h>
#include <cb2a/gen_tlv_uf_data_aut.h>
#include <cb2a/gen_tlv_record_data.h>


#ifdef SSL_MODE 
#define StopCltComms     StopCltCommsSSL 
#define InitSrvComms     InitSrvCommsSSL 
#define AcceptCallSrv    AcceptCallSrvSSL 
#define ReadLineMsg      ReadLineMsgSSL 
#define WriteLineMsg     WriteLineMsgSSL 
#endif 



const char* GetFunctionCodeDesc(char* sFunctionCode);
const char*	GetCurrStateLabel(int nIndiceCtx);
const char*	GetCurrIncidentLabel(int nReconResult);
const char*	GetTransMngCodeDesc(char* sCode);
const char*	GetSyncModeLabel(char sync_mode);
const char*	GetReconCodeLabel(char recon_mode);
const char*	GetIncCodeLabel(char* inc_code);
const char*	GetUploadCtrlLabel(char code);


#endif


