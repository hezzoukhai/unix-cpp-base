#ifndef HPS_STRUCT_LINE_H
#define HPS_STRUCT_LINE_H
#include <define.h>
#include <sys/time.h>
#include <tlv_switch_header.h>

typedef struct SLineExchange
{
	int   nLength;
	char  sBuffer[LG_MAX];
	struct timeval  Stime;
	TSTlvSwitchHeader	sHeader;	/*EBE131005*/
}TSLineExchange;

#endif /* HPS_STRUCT_LINE_H */
