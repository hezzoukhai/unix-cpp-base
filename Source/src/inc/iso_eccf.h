#ifndef HPS_ISO_ECCF_H
#define HPS_ISO_ECCF_H

#include <iso_com.h>
#include <eccf_fields.h>
#include <eccf_ictag.h>
#include <eccf_pdstag.h>

/***** Types de Champs  ******/
#define   ECCF_FIX_HEX       0
#define   ECCF_FIX_ALPHA     1
#define   ECCF_FIX_BIN       2
#define   ECCF_LL_HEX        3
#define   ECCF_LL_ALPHA      4
#define   ECCF_LL_BIN        5
#define   ECCF_LLL_HEX       6
#define   ECCF_LLL_ALPHA     7
#define   ECCF_LLL_BIN       8

/***** Types des TAG (Champ 55)  ******/
#define   ECCF_TAG_FIX_BIN       0
#define   ECCF_TAG_VAR_BIN       1
#define   ECCF_TAG_FIX_BCD       2
#define   ECCF_TAG_VAR_BCD       3
#define   ECCF_TAG_FIX_ALPHA     4
#define   ECCF_TAG_VAR_ALPHA     5
#define   ECCF_TAG_FIX_HEX       6
#define   ECCF_TAG_VAR_HEX       7

/***** Types des PDS (Champ 48)  ******/
#define   ECCF_PDS_FIX_ALPHA     0
#define   ECCF_PDS_VAR_ALPHA     1

/**** Longueur de champs  ****/
#define ECCF_BITMAP_LEN           16
#define MAX_ECCF_FIELDS          128
#define MAX_ECCF_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_ECCF_CHIP_TAG       100
#define  MAX_ECCF_CHIP_LEN       999

/**** PDS DATA   ****/
#define  MAX_ECCF_PDS       100
#define  MAX_ECCF_PDS_LEN   999

typedef struct SEccfInfo
{
   int           nFieldPos    [ MAX_ECCF_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ ECCF_BITMAP_LEN   ];
   char          sData        [ MAX_ECCF_DATA_LEN ];
}TSEccfInfo;

typedef struct SPdsEccf
{
   int  nPresent  [ MAX_ECCF_PDS ];
   int  nPosPds   [ MAX_ECCF_PDS ];
   int  nLength;
   char sPdsData  [ MAX_ECCF_PDS_LEN ];
}TSPdsEccf;

typedef struct STagEccf
{
   int  nPresent  [ MAX_ECCF_CHIP_TAG ];
   int  nPosTag   [ MAX_ECCF_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_ECCF_CHIP_LEN ];
}TSTagEccf;

void  InitEccfInfo(TSEccfInfo * msgInfo);

int   AnalyseEccf    (char * buffer_rec, TSEccfInfo * msgInfo);
int   GetEccfField   (int    field_no,   TSEccfInfo * msgInfo, 
                                                  char * data, int *length);
int   AddEccfField   (int    field_no,   TSEccfInfo * msgInfo, 
                                                  char * data, int length);
int   InsertEccfField   (int    field_no,   TSEccfInfo * msgInfo, 
                                                  char * data, int length);
int   PutEccfField   (int    field_no,   TSEccfInfo * msgInfo, 
                                                  char * data, int length);
int   EccfBuildMsg (char * buffer_snd, TSEccfInfo *msgInfo);

void  InitEccfIcTag(TSTagEccf * tagInfo);
int   AnalyseEccfIc(char * buffer, int nLength, TSTagEccf * tagInfo);
int   GetEccfIcTag (char *tag_name, TSTagEccf *tagInfo,char *data, int *length);
int   AddEccfIcTag (char *tag_name, TSTagEccf *tagInfo, char *data, int length);
int   PutEccfIcTag (char *tag_name, TSTagEccf *tagInfo, char *data, int length);
int   EccfBuildIcFld (char * buffer_snd, TSTagEccf *tagInfo);

void  InitEccfPds(TSPdsEccf * pdsInfo);
int   AnalyseEccfPds(char *buffer,int nLength,TSPdsEccf *pdsInfo);
int   GetEccfPds (char *pds_name, TSPdsEccf *pdsInfo,char *data, int *length);
int   AddEccfPds (char *pds_name, TSPdsEccf *pdsInfo, char *data, int length);
int   PutEccfPds (char *pds_name, TSPdsEccf *pdsInfo, char *data, int length);
int   EccfBuildPds (char * buffer_snd, TSPdsEccf *pdsInfo);

#endif /** HPS_ISO_ECCF_H **/
