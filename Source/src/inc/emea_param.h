#ifndef HPS_EMEA_PARAM_H
#define HPS_EMEA_PARAM_H
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

#endif /*** HPS_EMEA_PARAM_H ***/
