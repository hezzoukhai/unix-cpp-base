#ifndef HPS_HSM_PARAM_H
#define HPS_HSM_PARAM_H


#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

#define  MAX_HSM_NBR      9
#define  MSG_HSM_DICONNECTED      "ND99"

/* Resource Specific Parameters read from P7_HSM_RESOURCE_PARAM table */
typedef struct SHsmProto
{
   char				ProtoId [ 2 ];
   char				Address [ 30 ];
   int				PortNbr;
   pthread_mutex_t	FdMutex;
   pthread_cond_t	FdCond;
   int				fd;
   time_t			time_last_activity;
}TSHsmProto;


extern TSHsmProto  tab_HsmLine[];
extern char  ProdVerif;
extern int   HsmNbr  ;
extern int nIsMultiThrPerCon;


#endif /*** HPS_HSM_PARAM_H ***/
