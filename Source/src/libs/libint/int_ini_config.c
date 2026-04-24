#include <libint.h>


int InitIntConfig(int nIndexCtx, char* szConfigFile)
{

	int		nResult;
	
	trace_event("Start InitIntConfig()",PROCESSING);


	nResult = InitTlvPrivateTags(nIndexCtx);


	trace_event("End   InitIntConfig(OK)",PROCESSING);

	return OK;
}
