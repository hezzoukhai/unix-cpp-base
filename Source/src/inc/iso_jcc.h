#ifndef HPS_ISO_JCC_H
#define HPS_ISO_JCC_H

#include <iso_com.h>
#include <jcc_fields.h>
#include <jcc_ictag.h>
#include <jcc_pdstag.h>

/***** Types de Champs  ******/
#define   JCC_FIX_HEX       0
#define   JCC_FIX_ALPHA     1
#define   JCC_FIX_BIN       2
#define   JCC_FIX_BCD       3
#define   JCC_LL_HEX        4
#define   JCC_LL_BCD        5
#define   JCC_LL_ALPHA      6
#define   JCC_LL_BIN        7
#define   JCC_LLL_BCD       8
#define   JCC_LLL_HEX       9
#define   JCC_LLL_ALPHA    10
#define   JCC_LLL_BIN      11

/***** Types des TAG (Champ 55)  ******/
#define   JCC_TAG_FIX_BIN       0
#define   JCC_TAG_VAR_BIN       1
#define   JCC_TAG_FIX_BCD       2
#define   JCC_TAG_VAR_BCD       3
#define   JCC_TAG_FIX_ALPHA     4
#define   JCC_TAG_VAR_ALPHA     5
/*YK220206:EMV*/
#define   JCC_TAG_VAR_HEX       6
/*YK220206:EMV*/
/***** Types des PDS (Champ 48)  ******/
#define   JCC_PDS_FIX_ALPHA     0
#define   JCC_PDS_VAR_ALPHA     1

/**** Longueur de champs  ****/
#define JCC_BITMAP_LEN           16
#define MAX_JCC_FIELDS          128
#define MAX_JCC_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_JCC_CHIP_TAG       100
#define  MAX_JCC_CHIP_LEN       999

/**** PDS DATA   ****/
#define  MAX_JCC_PDS       100
#define  MAX_JCC_PDS_LEN   999

typedef struct SJccInfo
{
   int           nFieldPos    [ MAX_JCC_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ JCC_BITMAP_LEN   ];
   char          sData        [ MAX_JCC_DATA_LEN ];
}TSJccInfo;

typedef struct SPdsJcc
{
   int  nPresent  [ MAX_JCC_PDS ];
   int  nPosPds   [ MAX_JCC_PDS ];
   int  nLength;
   char sPdsData  [ MAX_JCC_PDS_LEN ];
}TSPdsJcc;

typedef struct STagJcc
{
   int  nPresent  [ MAX_JCC_CHIP_TAG ];
   int  nPosTag   [ MAX_JCC_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_JCC_CHIP_LEN ];
}TSTagJcc;

void  InitJccInfo(TSJccInfo * msgInfo);
int   AnalyseJcc    (char * buffer_rec, TSJccInfo * msgInfo);
int   GetJccField (int field_no, TSJccInfo * msgInfo, char * data, int *length);
int AddJccField   (int field_no, TSJccInfo * msgInfo, char * data, int length);
int InsertJccField (int field_no, TSJccInfo * msgInfo, char * data, int length);
int PutJccField  (int field_no, TSJccInfo * msgInfo, char * data, int length);
int JccBuildMsg (char * buffer_snd, TSJccInfo *msgInfo);

void  InitJccIcTag(TSTagJcc * tagInfo);
int   AnalyseJccIc(char * buffer, int nLength, TSTagJcc * tagInfo);
int   GetJccIcTag (char *tag_name, TSTagJcc *tagInfo,char *data, int *length);
int   AddJccIcTag (char *tag_name, TSTagJcc *tagInfo, char *data, int length);
int   PutJccIcTag (char *tag_name, TSTagJcc *tagInfo, char *data, int length);
int   JccBuildIcFld (char * buffer_snd, TSTagJcc *tagInfo);

void  InitJccPds(TSPdsJcc * pdsInfo);
int   AnalyseJccPds(char *buffer,int nLength,TSPdsJcc *pdsInfo);
int   GetJccPds (char *pds_name, TSPdsJcc *pdsInfo,char *data, int *length);
int   AddJccPds (char *pds_name, TSPdsJcc *pdsInfo, char *data, int length);
int   PutJccPds (char *pds_name, TSPdsJcc *pdsInfo, char *data, int length);
int   JccBuildPds (char * buffer_snd, TSPdsJcc *pdsInfo);

#endif /** HPS_ISO_JCC_H **/
