#ifndef HPS_ISO_HPS_H
#define HPS_ISO_HPS_H

#include <iso_com.h>
#include <iso_fields.h>
#include <iso_ictag.h>
#include <define.h>

/***** Types de Champs  ******/
#define   ISO_FIX_HEX       0
#define   ISO_FIX_ALPHA     1
#define   ISO_FIX_BIN       2
#define   ISO_LL_HEX        3
#define   ISO_LL_ALPHA      4
#define   ISO_LL_BIN        5
#define   ISO_LLL_HEX       6
#define   ISO_LLL_ALPHA     7
#define   ISO_LLL_BIN       8

/**** Longueur de champs  ****/
#define ISO_BITMAP_LEN           16
#define MAX_ISO_FIELDS          128
#define MAX_ISO_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_ISO_CHIP_TAG       100
#define  MAX_ISO_CHIP_LEN       999

/***** Types des TAG (Champ 55)  ******/
#define   ISO_TAG_FIX_BIN       0
#define   ISO_TAG_VAR_BIN       1
#define   ISO_TAG_FIX_BCD       2
#define   ISO_TAG_VAR_BCD       3
#define   ISO_TAG_FIX_ALPHA     4
#define   ISO_TAG_VAR_ALPHA     5
#define   ISO_TAG_VAR_HEX       6

typedef struct SIsoInfo
{
   int					nFieldPos    [ MAX_ISO_FIELDS  +1 ];
   int					nMsgType;
   int					nLength;
   char					sBitMap      [ ISO_BITMAP_LEN   ];
   char					sData        [ MAX_ISO_DATA_LEN ];
   msg_id_t				msgId;
}TSIsoInfo;

typedef struct STagIso
{
   int  nPresent  [ MAX_ISO_CHIP_TAG ];
   int  nPosTag   [ MAX_ISO_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_ISO_CHIP_LEN ];
}TSTagIso;

void  InitIsoInfo(TSIsoInfo * msgInfo);

int   AnalyseIso    (char * buffer_rec, TSIsoInfo * msgInfo);
int   GetIsoField   (int    field_no,   TSIsoInfo * msgInfo, 
                                                  char * data, int *length);
int   AddIsoField   (int    field_no,   TSIsoInfo * msgInfo, 
                                                  char * data, int length);
int   InsertIsoField   (int    field_no,   TSIsoInfo * msgInfo, 
                                                  char * data, int length);
int   PutIsoField   (int    field_no,   TSIsoInfo * msgInfo, 
                                                  char * data, int length);
int   IsoBuildMsg (char * buffer_snd, TSIsoInfo *msgInfo);

void  InitIsoIcTag(TSTagIso * msgInfo);
int   AnalyseIsoIc(char * buffer, int nLength, TSTagIso * msgInfo);
int   GetIsoIcTag (char *tag_name, TSTagIso * msgInfo, char *data, int *length);
int   AddIsoIcTag (char *tag_name, TSTagIso * msgInfo, char *data, int length);
int   PutIsoIcTag (char *tag_name, TSTagIso * msgInfo, char *data, int length);

#endif /** HPS_ISO_HPS_H **/
