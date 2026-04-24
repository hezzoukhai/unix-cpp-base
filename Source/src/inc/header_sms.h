#ifndef HPS_HEADER_SMS_H
#define HPS_HEADER_SMS_H
#include <define.h>

#define SMS_HDR_LEN          22  

typedef struct
{
	BYTE  bHeaderLength;
	BYTE  bHeaderFlag;
	BYTE  bTextFormat;
	BYTE  abMsgLength	[2];
	BYTE  abDestStationId	[3];
	BYTE  abSrcStationId	[3];
	BYTE  bEchoInfo;
	BYTE  abBaseIIFlags	[2];
	BYTE  abMsgFlags	[3];
	BYTE  bBatchNumber;
	BYTE  abReserved	[3];
	BYTE  bUserInfo;
	BYTE  abBitMap		[2];
	BYTE  abRejectCode	[2];

}TSSmsHeader;

#endif /* HPS_HEADER_SMS_H */
