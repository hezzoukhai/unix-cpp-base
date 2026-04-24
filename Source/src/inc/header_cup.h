#ifndef HPS_HEADER_CUP_H
#define HPS_HEADER_CUP_H
#include <define.h>

#define CUP_HDR_LEN          46  

typedef struct
{
	char  bHeaderLength;
	char  bHeaderFlag;
	char  abMsgLength	[4];
	char  abDestStationId	[11];
	char  abSrcStationId	[11];
	char  abReserved	[3];
	char  bBatchNumber;
	char  abTransInfo	[8];
	char  bUserInfo;
	char  abRejectCode	[5];

}TSCupHeader;

#endif /* HPS_HEADER_CUP_H */
