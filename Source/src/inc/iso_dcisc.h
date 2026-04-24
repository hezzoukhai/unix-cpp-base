#ifndef HPS_DCISC_HPS_H
#define HPS_DCISC_HPS_H

#include <iso_com.h>
#include <dcisc_fields.h>
#include <iso_ictag.h>

/***** Types de Champs  ******/
#define   DCISC_FIX_HEX       0
#define   DCISC_FIX_ALPHA     1
#define   DCISC_FIX_BIN       2
#define   DCISC_LL_HEX        3
#define   DCISC_LL_ALPHA      4
#define   DCISC_LL_BIN        5
#define   DCISC_LLL_HEX       6
#define   DCISC_LLL_ALPHA     7
#define   DCISC_LLL_BIN       8

/**** Longueur de champs  ****/
#define DCISC_BITMAP_LEN           16
#define MAX_DCISC_FIELDS          128
#define MAX_DCISC_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_DCISC_CHIP_TAG       100
#define  MAX_DCISC_CHIP_LEN       999

/***** Types des TAG (Champ 55)  ******/
#define   DCISC_TAG_FIX_BIN       0
#define   DCISC_TAG_VAR_BIN       1
#define   DCISC_TAG_FIX_BCD       2
#define   DCISC_TAG_VAR_BCD       3
#define   DCISC_TAG_FIX_ALPHA     4
#define   DCISC_TAG_VAR_ALPHA     5
#define   DCISC_TAG_VAR_HEX       6

typedef struct SDciscInfo
{
   int           nFieldPos    [ MAX_DCISC_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ DCISC_BITMAP_LEN   ];
   char          sData        [ MAX_DCISC_DATA_LEN ];
}TSDciscInfo;

typedef struct STagDcisc
{
   int  nPresent  [ MAX_DCISC_CHIP_TAG ];
   int  nPosTag   [ MAX_DCISC_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_DCISC_CHIP_LEN ];
}TSTagDcisc;

void  InitDciscInfo(TSDciscInfo * msgInfo);

int   AnalyseDcisc    (char * buffer_rec, TSDciscInfo * msgInfo);
int   GetDciscField   (int    field_no,   TSDciscInfo * msgInfo, char * data, int *length);
int   AddDciscField   (int    field_no,   TSDciscInfo * msgInfo, char * data, int length);
int   InsertDciscField   (int    field_no,   TSDciscInfo * msgInfo, char * data, int length);
int   PutDciscField   (int    field_no,   TSDciscInfo * msgInfo, char * data, int length);
int   DciscBuildMsg (int nIndiceCtx, char * buffer_snd, TSDciscInfo *msgInfo,char *BadField);

void  InitDciscIcTag(TSTagDcisc * msgInfo);
int   AnalyseDciscIc(char * buffer, int nLength, TSTagDcisc * msgInfo);
int   GetDciscIcTag (char *tag_name, TSTagDcisc * msgInfo, char *data, int *length);
int   AddDciscIcTag (char *tag_name, TSTagDcisc * msgInfo, char *data, int length);
int   PutDciscIcTag (char *tag_name, TSTagDcisc * msgInfo, char *data, int length);

#endif /** HPS_DCISC_HPS_H **/
