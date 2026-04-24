#ifndef HPS_DSCOVER_HPS_H
#define HPS_DSCOVER_HPS_H



#include <iso_com.h>
#include <discover_fields.h>
#include <iso_ictag.h>

/***** Types de Champs  ******/
#define   DSCOVER_FIX_HEX       0
#define   DSCOVER_FIX_ALPHA     1
#define   DSCOVER_FIX_BIN       2
#define   DSCOVER_LL_HEX        3
#define   DSCOVER_LL_ALPHA      4
#define   DSCOVER_LL_BIN        5
#define   DSCOVER_LLL_HEX       6
#define   DSCOVER_LLL_ALPHA     7
#define   DSCOVER_LLL_BIN       8

/**** Longueur de champs  ****/
#define DSCOVER_BITMAP_LEN           16
#define MAX_DSCOVER_FIELDS          128
#define MAX_DSCOVER_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_DSCOVER_CHIP_TAG       100
#define  MAX_DSCOVER_CHIP_LEN       999

/***** Types des TAG (Champ 55)  ******/
#define   DSCOVER_TAG_FIX_BIN       0
#define   DSCOVER_TAG_VAR_BIN       1
#define   DSCOVER_TAG_FIX_BCD       2
#define   DSCOVER_TAG_VAR_BCD       3
#define   DSCOVER_TAG_FIX_ALPHA     4
#define   DSCOVER_TAG_VAR_ALPHA     5
#define   DSCOVER_TAG_VAR_HEX       6

typedef struct SDscoverInfo
{
   int           nFieldPos    [ MAX_DSCOVER_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ DSCOVER_BITMAP_LEN   ];
   char          sData        [ MAX_DSCOVER_DATA_LEN ];
}TSDscoverInfo;

typedef struct STagDscover
{
   int  nPresent  [ MAX_DSCOVER_CHIP_TAG ];
   int  nPosTag   [ MAX_DSCOVER_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_DSCOVER_CHIP_LEN ];
}TSTagDscover;

void  InitDscoverInfo(TSDscoverInfo * msgInfo);

int   AnalyseDscover    (char * buffer_rec, TSDscoverInfo * msgInfo);
int   GetDscoverField   (int    field_no,   TSDscoverInfo * msgInfo, char * data, int *length);
int   AddDscoverField   (int    field_no,   TSDscoverInfo * msgInfo, char * data, int length);
int   InsertDscoverField   (int    field_no,   TSDscoverInfo * msgInfo, char * data, int length);
int   PutDscoverField   (int    field_no,   TSDscoverInfo * msgInfo, char * data, int length);
int   DscoverBuildMsg (int nIndiceCtx, char * buffer_snd, TSDscoverInfo *msgInfo,char *BadField);

void  InitDscoverIcTag(TSTagDscover * msgInfo);
int   AnalyseDscoverIc(char * buffer, int nLength, TSTagDscover * msgInfo);
int   GetDscoverIcTag (char *tag_name, TSTagDscover * msgInfo, char *data, int *length);
int   AddDscoverIcTag (char *tag_name, TSTagDscover * msgInfo, char *data, int length);
int   PutDscoverIcTag (char *tag_name, TSTagDscover * msgInfo, char *data, int length);

#endif /** HPS_DSCOVER_HPS_H **/
