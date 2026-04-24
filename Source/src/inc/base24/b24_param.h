#ifndef BASE24___PARAM_H
#define BASE24___PARAM_H

#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* Resource Common Parameters ***/
#include <resources.h>

/*extern int  nFdConnect;
 * extern int  nFdLocal;*/

/* Resource Specific Parameters read from P7_BASE24_RESOURCE_PARAM table */
extern int   nNbChannels;
extern char  sDPCNumber  [ ];
extern char  szStationIndex [ ];
extern char  szLogicalNetwork   [ ];
extern char  sNetworkIdentifier   [ ];
extern char  sBankId   [ ];
extern char  sCuttofStatus   [ ];
extern char  sCuttofTime   [ ];
extern char  sKeyMaxErr   [ ];
extern char  sKeyMaxUsage   [ ];
extern char  sKeyMaxPeriod   [ ];


#define		PROSA_NB_CHANNEL            "R01"
#define		PROSA_DPC_NUMBER            "R02"
#define		PROSA_STATION_INDEX         "R03"
#define		PROSA_LOGICAL_NETWORK       "R04"
#define		PROSA_NETWORK_ID	        "R05"
#define		PROSA_BANK_ID               "R06"
#define		PROSA_CUTOFF_STATUS         "R07"
#define		PROSA_CUTOVER_TIME          "R08"
#define		PROSA_KEY_MAX_ERR           "R09"
#define		PROSA_KEY_MAX_USAGE         "R10"
#define		PROSA_KEY_MAX_PERIOD        "R11"

#endif /*** BASE24_PARAM_H ***/
