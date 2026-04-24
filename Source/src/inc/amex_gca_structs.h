#ifndef HPS_AMEX_GCA_STRUCTS_H
#define HPS_AMEX_GCA_STRUCTS_H

#include<amex_gca_define.h>

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

/*
typedef struct SAmexCtx
{
   int              free;
   int              status;
   pthread_t        tid;
   int              fd;
   int              flag_bal;
   char             szBufferLine[ LG_MAX ];
   int              nLenLine;
   char             szBufferBal [ LG_MAX ];
   int              nLenBal;
   pthread_mutex_t  AmexMutex;
   pthread_cond_t   AmexCond ;
} TSAmexCtx;

TSAmexCtx  tab_AmexCtx [ MAX_THREAD ];

void  InitTabAmexCtx();
int   GetFreeIndex  ();
int   GetThreadIndex(pthread_t tid);
*/

#endif /** HPS_AMEX_GCA_STRUCTS_H **/
