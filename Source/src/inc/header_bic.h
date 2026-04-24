#ifndef HPS_HEADER_BIC_H
#define HPS_HEADER_BIC_H
#include <define.h>

#define BIC_HDR_LEN          12  

typedef struct
{
	char  bicStart 		[3];
	char  bicProduct 		[2];
	char  bicVersion 		[2];
	char  bicStatus  		[3];
	char  bicOriginatorCode;
	char  bicResponderCode ;

}TSBicHeader;

#endif /* HPS_HEADER_BIC_H */
