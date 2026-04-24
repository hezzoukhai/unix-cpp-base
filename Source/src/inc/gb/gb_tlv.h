#ifndef GB_TLV_H
#define GB_TLV_H

#ifndef HPS_TLV_GB_H
#define HPS_TLV_GB_H

#include <tlv_com.h>

/**************************************/
#define GB_TLV_FIX 0
#define GB_TLV_VAR 1
/**************************************/
#define GB_TLV_CVM 2
#define GB_TLV_CERT_SERIAL_NBR 3
#define GB_TLV_SECURITY_IND 4
#define GB_TLV_CVC2 5

/**************************************/

#define MAX_GB_TLV 999
#define MAX_GB_TLV_LEN 4098

typedef struct STlvGB
{
   int nPresent[MAX_GB_TLV];
   int nPosTlv[MAX_GB_TLV];
   int nLength;
   char sTlvData[MAX_GB_TLV_LEN];
} TSTlvGB;

void InitTlvGB(TSTlvGB *tlvInfo);
int AnalyseTlvGB(char *buffer, TSTlvGB *tlvInfo);
int GetTlvGB(char *tlv_name, TSTlvGB *tlvInfo, char *data, int *length);
int AddTlvGB(char *tlv_name, TSTlvGB *tlvInfo, char *data, int length);
int PutTlvGB(char *tlv_name, TSTlvGB *tlvInfo, char *data, int length);
int GBBuildTlv(char *buffer_snd, TSTlvGB *tlvInfo);

#endif /** HPS_TLV_GB_H **/

#endif /* GB_TLV_H */
