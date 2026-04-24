#ifndef HPS_EVENT_LOG_H
#define HPS_EVENT_LOG_H

#define INTERFACE_ABNORMAL_STOP    "INAB"
#define INTERFACE_DOWN             "INDW"
#define INTERFACE_SIGN_ON          "INSN"
#define INTERFACE_SIGN_OFF         "INSO"
#define INTERFACE_START            "INSR"
#define INTERFACE_TIME_OUT         "INTO"
#define INTERFACE_STOP             "INST"
#define INTERFACE_UP               "INUP"

#define ERROR_RETREIVE_RESOURCE    "ENRR"
#define ERROR_WRITING_IN_QUEUE     "ERWQ"
#define ERROR_TIME_OUT             "ERTO"
#define ERROR_READ_FROM_QUEUE      "ERRQ"
#define ERROR_UFS_NO_EXIST         "FSNE"
#define ERROR_UFS_FULL   	   "SYSE"
#define ERROR_IPC_FULL   	   "SYSE"
#define ERROR_IPC_NO_EXIST         "IPNE"
#define ERROR_DBTS_FULL   	   "DBWR"
#define ERROR_DBTS_NO_EXIST        "DBNE"
#define ERROR_INVALID_MAC          "INMA"
#define MAX_SEND_SIGN              "MXSS"
/*CFE PROD00052188 */
#define RECONCILIATION_FAILURE     "RCNL"
#define DOWNLOAD_FAILURE    	   "PARA"

typedef struct SEventLog
{

   char   ResourceId [ 6];
   char   EventId    [ 4];
   char   TeminalId  [15];
   char   ApiErrno1  [ 8];
   char   ApiErrno2  [ 8];
 
}TSEventLog;

#endif /**  HPS_EVENT_LOG_H **/
