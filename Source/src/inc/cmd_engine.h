#ifndef HPS_CMD_ENGINE_H
#define HPS_CMD_ENGINE_H

#define MAX_LABEL_LEN                           30

typedef struct engine_t
{
        int     initial_status;
        int     event ;
        int     (*processing)() ;
        int     next_status;
} engine;

typedef struct TOrder{
    int         fOrder;
    char        fLabel  [MAX_LABEL_LEN];
} SOrder;

typedef struct TStatus{
    int         fStatus;
    char        fLabel  [MAX_LABEL_LEN];
} SStatus;

#define MAX_ORDER_NBR               51
#define MAX_STATUS_NBR              8

#endif /*** HPS_CMD_ENGINE_H ***/
