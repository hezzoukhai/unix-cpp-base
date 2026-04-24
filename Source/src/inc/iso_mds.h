#ifndef HPS_ISO_MDS_H
#define HPS_ISO_MDS_H

#include <iso_com.h>
#include <mds_fields.h>
#include <mds_ictag.h>
#include <mds_pdstag.h>

/***** Types de Champs  ******/
#define   MDS_FIX_HEX       0
#define   MDS_FIX_ALPHA     1
#define   MDS_FIX_BIN       2
#define   MDS_LL_HEX        3
#define   MDS_LL_ALPHA      4
#define   MDS_LL_BIN        5
#define   MDS_LLL_HEX       6
#define   MDS_LLL_ALPHA     7
#define   MDS_LLL_BIN       8

/***** Types des TAG (Champ 55)  ******/
#define   MDS_TAG_FIX_BIN       0
#define   MDS_TAG_VAR_BIN       1
#define   MDS_TAG_FIX_BCD       2
#define   MDS_TAG_VAR_BCD       3
#define   MDS_TAG_FIX_ALPHA     4
#define   MDS_TAG_VAR_ALPHA     5
#define   MDS_TAG_VAR_HEX       6

/***** Types des PDS (Champ 48)  ******/
#define   MDS_PDS_FIX_ALPHA     0
#define   MDS_PDS_VAR_ALPHA     1

/**** Longueur de champs  ****/
#define MDS_BITMAP_LEN           16
#define MAX_MDS_FIELDS          128
#define MAX_MDS_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_MDS_CHIP_TAG       100
#define  MAX_MDS_CHIP_LEN       999

/**** PDS DATA   ****/
#define  MAX_MDS_PDS       100
#define  MAX_MDS_PDS_LEN   999

typedef struct SMdsInfo
{
   int           nFieldPos    [ MAX_MDS_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ MDS_BITMAP_LEN   ];
   char          sData        [ MAX_MDS_DATA_LEN ];
}TSMdsInfo;

typedef struct SPdsMds
{
   int  nPresent  [ MAX_MDS_PDS ];
   int  nPosPds   [ MAX_MDS_PDS ];
   int  nLength;
   int  nMsgType;
   char sPdsData  [ MAX_MDS_PDS_LEN ];
}TSPdsMds;

typedef struct STagMds
{
   int  nPresent  [ MAX_MDS_CHIP_TAG ];
   int  nPosTag   [ MAX_MDS_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_MDS_CHIP_LEN ];
}TSTagMds;

void  BuildPosMode ( char *IsoPosMode , char *mdsPosMode );

void  InitMdsInfo(TSMdsInfo * msgInfo);

int   AnalyseMds    (char * buffer_rec, TSMdsInfo * msgInfo);
int   GetMdsField   (int    field_no,   TSMdsInfo * msgInfo, 
                                                  char * data, int *length);
int   AddMdsField   (int    field_no,   TSMdsInfo * msgInfo, 
                                                  char * data, int length);
int   InsertMdsField   (int    field_no,   TSMdsInfo * msgInfo, 
                                                  char * data, int length);
int   PutMdsField   (int    field_no,   TSMdsInfo * msgInfo, 
                                                  char * data, int length);
int   MdsBuildMsg (char * buffer_snd, TSMdsInfo *msgInfo);

void  InitMdsIcTag(TSTagMds * tagInfo);
int   AnalyseMdsIc(char * buffer, int nLength, TSTagMds * tagInfo);
int   GetMdsIcTag (char *tag_name, TSTagMds *tagInfo,char *data, int *length);
int   AddMdsIcTag (char *tag_name, TSTagMds *tagInfo, char *data, int length);
int   PutMdsIcTag (char *tag_name, TSTagMds *tagInfo, char *data, int length);
int   MdsBuildIcFld (char * buffer_snd, TSTagMds *tagInfo);



#endif /** HPS_ISO_MDS_H **/
