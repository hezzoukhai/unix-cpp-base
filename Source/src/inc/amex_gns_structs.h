#ifndef HPS_AMEX_GNS_STRUCTS_H
#define HPS_AMEX_GNS_STRUCTS_H

#include<amex_gns_define.h>

typedef struct STagAmex
{
   int  nPresent  [ MAX_AMEX_CHIP_TAG ];
   int  nPosTag   [ MAX_AMEX_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_AMEX_CHIP_LEN ]; 
}TSTagAmex;

void  InitAmexIccTag(TSTagAmex * msgInfo);
int   AnalyseAmexIcc(char * buffer , int nMsgType, int nLength, TSTagAmex * msgInfo);
int   GetAmexIccTag (char *tag_name, int nMsgType, TSTagAmex * msgInfo, char *data, int *length);
int   AddAmexIccTag (char *tag_name, int nMsgType, TSTagAmex * msgInfo, char *data, int length);
int   PutAmexIccTag (char *tag_name, int nMsgType, TSTagAmex * msgInfo, char *data, int length);

 

#endif /** HPS_AMEX_FIELDS_H **/
