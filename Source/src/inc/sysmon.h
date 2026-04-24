#ifndef PWC_SYS_MON__H
#define PWC_SYS_MON__H

#ifdef SOLARIS
	#include <thread.h>
	#include <limits.h>
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


#	if defined(_AIX)

#include <libperfstat.h>
#include <sys/systemcfg.h>
#include <sys/statvfs.h>
#include <sys/msg.h>
#include <sys/procfs.h>

#	elif defined(LINUX)

#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/statvfs.h>

#	elif defined(HPUX)
#include <sys/pstat.h>

#endif

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


#include <system_monitoring.h>



typedef struct {

	pid_t nPid ;
	unsigned long long last_time_user ;
	unsigned long long last_time_system ;
	 
} TSResLastTime;

#define NB_RESSOURCES		256


#endif


