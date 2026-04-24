/*
** <Name>
**  mds.h
** 
** <Descr>
**  general header file for "mds" interface
** 
** <Module History>
**  Version       Date      Author         Comments
**  1.0.0.0       20Jan02   Y. OUADI       Initiale Version
** 
** (C) 1999-2001 : Hightech Payment Systems
*/

#ifndef GB_H
#define GB_H

/* - Header File Inclusion Section ----------------------------------------- */
/* - System Header Files */
#include <tcp_var.h>
#include <comms_param.h>
#include <dump.h>
#include <event_log.h>

/* - Private Header Files */
#include <resources.h>
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <param_arg.h>
#include <thread_var.h>
#include <errno.h>
#include <signal.h>
#include <define.h>
#include <iso_hps.h>
#include <iso_fields.h>
#include <iso_cps.h>
#include <iso_gb.h>
#include <tlv_private.h>
#include <tlv_fields.h>
#include <list_thr.h>
#include <security_data.h>
#include <queue.h>
#include <tlv_privfields.h>
#include <tlv_autho.h>
#include <header_gb.h>
#include <msg_flow_mapping.h>
#include <struct_line.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <struct_time.h>







/*****************************************************************************/
#include <gb/gb_line.h>
#include <gb/gb_adm.h>
#include <gb/gb_sig.h>
#include <gb/gb_bal.h>
#include <gb/gb_saf.h>
#include <gb/gb_param.h>
#include <gb/gb_net_mng.h>
#include <gb/gb_fields.h>
#include <gb/gb_define.h>
#include <gb/gb_tlv.h>








/* - Function Declaration Section ------------------------------------------ */
/* - Public Functions Declaration */
int ThreadMaskSignals(void);
void sig_alarm(int sig);
void GetGlobals(TSResources *P_sResource);
int InitModule(char *P_szResourceId);
void DumpGlobals(void);

/* - Private Functions Declaration */
#ifdef _FOR_INT_USE_
#endif /* _FOR_INT_USE_ */

/* - Module Variables Publication --------------------------------------- */
#ifdef _FOR_EXT_USE_
#endif /* _FOR_EXT_USE_ */

#endif /* GB_H */
