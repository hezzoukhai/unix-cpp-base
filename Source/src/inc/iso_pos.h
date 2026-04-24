#ifndef HPS_ISO_POS_H
#define HPS_ISO_POS_H

#include <define.h>
#include <iso_com.h>
#include <pos_fields.h>
#include <pos_ictag.h>

/***** Types de Champs  ******/
#define   POS_FIX_HEX       0
#define   POS_FIX_ALPHA     1
#define   POS_FIX_BIN       2
#define   POS_LL_HEX        3
#define   POS_LL_ALPHA      4
#define   POS_LL_BIN        5
#define   POS_LLL_HEX       6
#define   POS_LLL_ALPHA     7
#define   POS_LLL_BIN       8

/**** Longueur de champs  ****/
#define POS_BITMAP_LEN           16
#define MAX_POS_FIELDS          128
#define MAX_POS_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_POS_CHIP_TAG       100
#define  MAX_POS_CHIP_LEN       999

/***** Types des TAG (Champ 55)  ******/
#define   POS_TAG_FIX_BIN       0
#define   POS_TAG_VAR_BIN       1
#define   POS_TAG_FIX_BCD       2
#define   POS_TAG_VAR_BCD       3
#define   POS_TAG_FIX_ALPHA     4
#define   POS_TAG_VAR_ALPHA     5



/***** Traitement 72  ******/
#define   POS_PROCESS_PERSO_REQ          3
#define   POS_PROCESS_COMPTE_REQ         4
#define   POS_PROCESS_BIN_LOC_REQ        5
#define   POS_PROCESS_PREFIXES_REQ       6
#define   POS_PROCESS_ACTIVATEE_REQ      7

typedef struct SPosHeader
{
   char   sProto  [ 3 ];
   char   sMsgLen [ 4 ];
   char   sPos    [ 1 ];
   char   sVersion[ 5 ];
   char   sReject [ 3 ];
}TSPosHeader;

typedef struct SPosInfo
{
   int           nFieldPos    [ MAX_POS_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   TSPosHeader   sHeader;
   char          sBitMap      [ POS_BITMAP_LEN   ];
   char          sData        [ MAX_POS_DATA_LEN ];
}TSPosInfo;

typedef struct STagPos
{
   int  nPresent  [ MAX_POS_CHIP_TAG ];
   int  nPosTag   [ MAX_POS_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_POS_CHIP_LEN ];
}TSTagPos;


typedef struct STelecollectData
{
  char   Buffer  [ LG_MAX ];
}TSTelecollectData;


void  InitPosInfo(TSPosInfo * msgInfo);

int   AnalysePos    (char * buffer_rec, TSPosInfo * msgInfo);
int   GetPosField   (int    field_no,   TSPosInfo * msgInfo, 
                                                  char * data, int *length);
int   AddPosField   (int    field_no,   TSPosInfo * msgInfo, 
                                                  char * data, int length);
int   InsertPosField   (int    field_no,   TSPosInfo * msgInfo, 
                                                  char * data, int length);
int   PutPosField   (int    field_no,   TSPosInfo * msgInfo, 
                                                  char * data, int length);
int   PosBuildMsg (char * buffer_snd, TSPosInfo *msgInfo);

void  InitPosIcTag(TSTagPos * msgInfo);
int   AnalysePosIc(char * buffer, int nLength, TSTagPos * msgInfo);
int   GetPosIcTag (char *tag_name, TSTagPos * msgInfo, char *data, int *length);
int   AddPosIcTag (char *tag_name, TSTagPos * msgInfo, char *data, int length);
int   PutPosIcTag (char *tag_name, TSTagPos * msgInfo, char *data, int length);

#endif /** HPS_ISO_POS_H **/
