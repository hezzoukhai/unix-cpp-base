#ifndef SWITCH_MON__H
#define SWITCH_MON__H


#ifdef SOLARIS
	#include <thread.h>
#endif
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <sys/shm.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#include <limits.h>

#include <define.h>
#include <resources.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <event_log.h>
#include <thread_var.h>
#include <param_arg.h>
#include <main_thr.h>
#include <queue.h>
#include <line_com.h>
#include <struct_line.h>
#include <struct_time.h>
#include <security_data.h>
#include <security_flags.h>
#include <security_results.h>
#include <tlv_fields.h>
#include <tlv_private.h>
#include <tlv_privfields.h>
#include <tlv_autho.h>
#include <iso_hps.h>
#include <service.h>

#include <general_macros.h>
#include <msg_flow_mapping.h>
#include <switch_monitoring.h>
#include <gen_tlv_msg_mapping_flow.h>


#endif

