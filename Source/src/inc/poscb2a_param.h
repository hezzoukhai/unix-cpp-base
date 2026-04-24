#ifndef HPS_POSCB2A_PARAM_H
#define HPS_POSCB2A_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>

/* Resource Common Parameters ***/
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];

char AcquirerBank [ 6 + 1 ];



int Step; /* Les etapes de la telecollecte  0: Batch Header,
										    1: Detail,
											2: Trailler 
		  */

#define		PROCESSED     1 /* transaction deja inserer dans autho activity */
#define		NOT_PROCESSED 2 /* trans n'est pas inserer dans Autho_activity */
#define		WAITING_UPD   3 /* transction doit faire la maj de autho_activity */


#define OK_END_TRANSFER 2


#define   NOT_USED      -1

#define   CB2A_TLV_FIX_BIN       0
#define   CB2A_TLV_VAR_BIN       1
#define   CB2A_TLV_FIX_BCD       2
#define   CB2A_TLV_VAR_BCD       3
#define   CB2A_TLV_FIX_ALPHA     4
#define   CB2A_TLV_VAR_ALPHA     5
#define   CB2A_TLV_NUMERIC_58    6
#define   CB2A_TLV_NUMERIC_46    7
#define   CB2A_TLV_ROUTAGE_EXT_FIX_ALPHA    8
#define   CB2A_TLV_ROUTAGE_EXT_VAR_ALPHA    9
#define   CB2A_TLV_ROUTAGE_EXT_58_0412 		10
#define   CB2A_TLV_ROUTAGE_EXT_ALPHA		11
#define   CB2A_TLV_ROUTAGE_EXT_59_0001		12
  

#define  MAX_CB2A_TLV       999
#define  MAX_CB2A_TLV_LEN  4096

typedef struct STlvCb2a
{
   int  nPresent  [ MAX_CB2A_TLV ];
   int  nPosTlv   [ MAX_CB2A_TLV ];
   int  nLength;
   char sTlvData  [ MAX_CB2A_TLV_LEN ];

}TSTlvCb2a;


typedef struct cb2a_tlv_info_s
{
   char    sTlvName[4];
   int     nType;
   int     nLength;
}cb2a_tlv_info_t;

#endif /*** HPS_POSCB2A_PARAM_H ***/
