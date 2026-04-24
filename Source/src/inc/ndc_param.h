#ifndef HPS_NDC_PARAM_H
#define HPS_NDC_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <ndcglobls.h>
#include <define.h>

#include <resources.h>

/*keep only defines in this file */

/*
int    lng;
int    gTransLanguage;
int    gPrintFlag;


#define TDES    0
*/

#define NDC_ACC_NBR_LG_MAX				24
#define NDC_ACC_CCY_LG_MAX				3
#define NDC_ACC_TYPE_LG_MAX				2

/*EBE141026: Adding some max length defines to avoid using extensive LG_MAX which breaks the stack for deep functions processing*/
#define NDC_BUFFER_MAX_LEN			2048
#define NDC_FLD_MAX_LEN				1024

/* Labels */
#define LBL_BAL_MAX_LEN				512
#define LBL_MAX_LEN					1024
#define   TSN_LEN        		    	4
#define   TERMINAL_LEN        			8

#define   PG_NDC_PIN_BLOCK_FORMAT                 "NDC_PIN_BLOCK_FORMAT" /*ZKO 15052020: PLUTONL-1087*/
extern char sgPinBlockFormat [];/*ZKO15052020*/


#define NDC_PAR_TLS_CERT			"001"
#define NDC_PAR_TLS_PK				"002"
#define NDC_PAR_TLS_CA_CERT			"003"
#define NDC_PAR_TLS_VERIF_CLT		"004"


#endif /*** HPS_NDC_PARAM_H ***/
