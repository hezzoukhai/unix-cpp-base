#ifndef HPS_BIC_PARAM_H
#define HPS_BIC_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>


#include <resources.h>


extern char  ReversalGeneration [ ];
extern char  sInterfaceMode     [ ];
extern char		g_pris_connect_mode[ ];


/* Start NAB01122014 *** Bank Code value for BZI ***/
#define        LOCAL_BANK     	"000001"
#define        VISA_BANK     	"000002"
#define        MCI_BANK     	"000003"
#define        SMT_BANK     	"000004"
/* End NAB02092014 ***/


#define BIC_MAX_MSG_LEN				3000
#define BIC_F048_DEF_VAL			"AVMBP                   40000078800000000000"
#define BIC_F048_DEF_NW_MNG_VAL		"6011000111N00788000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
#define BIC_F060_DEF_VAL			"0103BNA +000"


#endif /*** HPS_BIC_PARAM_H ***/
