#include <stdio.h>
#include <iso_hps.h>
#include <define.h>
#include <service.h>
#include <event_auth.h>

int PreControl (        int 			nIndiceCtx, 
                        char            cAuthorisationMode 	,
                      	char            *sTlvAuth,
					    char			*sPrimaryResource   	, 
					    char			*sSecondaryResource 
                )
{
	char		slResource			[      6 + 1 ];
	char		slCardNumber		[     22 + 1 ];
	char		slProcCode			[      6 + 1 ];
	char		sLine				[MAX_LINE_TRC];
    int     	nLength;
	int 		nEventCode   =  SYSTEM_MALFUNCTION;
	
	trace_event("Start PreControl()",PROCESSING);

/******************************************************************************/
/* 				          Control Processing                 				  */
/******************************************************************************/


	sprintf(sLine,"End   PreControl(%d)",nEventCode);
	trace_event(sLine,PROCESSING);
	return(nEventCode);

} 
