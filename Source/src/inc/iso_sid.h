#ifndef HPS_SID_HPS_H
#define HPS_SID_HPS_H

#include <iso_com.h>
#include <sid_fields.h>
#include <iso_ictag.h>

/***** Types de Champs  ******/
#define   SID_FIX_HEX       0
#define   SID_FIX_ALPHA     1
#define   SID_FIX_BIN       2
#define   SID_LL_HEX        3
#define   SID_LL_ALPHA      4
#define   SID_LL_BIN        5
#define   SID_LLL_HEX       6
#define   SID_LLL_ALPHA     7
#define   SID_LLL_BIN       8

/**** Longueur de champs  ****/
#define SID_BITMAP_LEN           16
#define SID_HEADER_LEN           11 /*NAB02042021 PLUTONL-2974*/
#define MAX_SID_FIELDS          128
#define MAX_SID_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_SID_CHIP_TAG       100
#define  MAX_SID_CHIP_LEN       999

/***** Types des TAG (Champ 55)  ******/
#define   SID_TAG_FIX_BIN       0
#define   SID_TAG_VAR_BIN       1
#define   SID_TAG_FIX_BCD       2
#define   SID_TAG_VAR_BCD       3
#define   SID_TAG_FIX_ALPHA     4
#define   SID_TAG_VAR_ALPHA     5
#define   SID_TAG_VAR_HEX       6

typedef struct SSidInfo
{
   int           nFieldPos    [ MAX_SID_FIELDS  +1 ];
   char          sHeader      [ SID_HEADER_LEN+1];/*NAB02042021 PLUTONL-2974*/
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ SID_BITMAP_LEN   ];
   char          sData        [ MAX_SID_DATA_LEN ];
}TSSidInfo;

typedef struct STagSid
{
   int  nPresent  [ MAX_SID_CHIP_TAG ];
   int  nPosTag   [ MAX_SID_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_SID_CHIP_LEN ];
}TSTagSid;

void  InitSidInfo(TSSidInfo * msgInfo);

int   AnalyseSid    (char * buffer_rec, TSSidInfo * msgInfo);
int   GetSidField   (int    field_no,   TSSidInfo * msgInfo, char * data, int *length);
int   AddSidField   (int    field_no,   TSSidInfo * msgInfo, char * data, int length);
int   InsertSidField   (int    field_no,   TSSidInfo * msgInfo, char * data, int length);
int   PutSidField   (int    field_no,   TSSidInfo * msgInfo, char * data, int length);
int   SidBuildMsg (int nIndiceCtx, char * buffer_snd, TSSidInfo *msgInfo,char *BadField);

void  InitSidIcTag(TSTagSid * msgInfo);
int   AnalyseSidIc(char * buffer, int nLength, TSTagSid * msgInfo);
int   GetSidIcTag (char *tag_name, TSTagSid * msgInfo, char *data, int *length);
int   AddSidIcTag (char *tag_name, TSTagSid * msgInfo, char *data, int length);
int   PutSidIcTag (char *tag_name, TSTagSid * msgInfo, char *data, int length);

#endif /** HPS_SID_HPS_H **/
