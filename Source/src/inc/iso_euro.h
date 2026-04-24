#ifndef HPS_ISO_EURO_H
#define HPS_ISO_EURO_H

#include <iso_com.h>
#include <euro_fields.h>
#include <euro_ictag.h>
#include <euro_pdstag.h>

/***** Types de Champs  ******/
#define   EURO_FIX_HEX       0
#define   EURO_FIX_ALPHA     1
#define   EURO_FIX_BIN       2
#define   EURO_LL_HEX        3
#define   EURO_LL_ALPHA      4
#define   EURO_LL_BIN        5
#define   EURO_LLL_HEX       6
#define   EURO_LLL_ALPHA     7
#define   EURO_LLL_BIN       8

/***** Types des TAG (Champ 55)  ******/
#define   EURO_TAG_FIX_BIN       0
#define   EURO_TAG_VAR_BIN       1
#define   EURO_TAG_FIX_BCD       2
#define   EURO_TAG_VAR_BCD       3
#define   EURO_TAG_FIX_ALPHA     4
#define   EURO_TAG_VAR_ALPHA     5
#define   EURO_TAG_VAR_HEX       6

/***** Types des PDS (Champ 48)  ******/
#define   EURO_PDS_FIX_ALPHA     0
#define   EURO_PDS_VAR_ALPHA     1

/**** Longueur de champs  ****/
#define EURO_BITMAP_LEN           16
#define MAX_EURO_FIELDS          128
#define MAX_EURO_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_EURO_CHIP_TAG       100
#define  MAX_EURO_CHIP_LEN       999

/**** PDS DATA   ****/
#define  MAX_EURO_PDS       100
#define  MAX_EURO_PDS_LEN   999

typedef struct SEuroInfo
{
   int           nFieldPos    [ MAX_EURO_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ EURO_BITMAP_LEN   ];
   char          sData        [ MAX_EURO_DATA_LEN ];
}TSEuroInfo;

typedef struct SPdsEuro
{
   int  nPresent  [ MAX_EURO_PDS ];
   int  nPosPds   [ MAX_EURO_PDS ];
   int  nLength;
   int  nMsgType;
   char sPdsData  [ MAX_EURO_PDS_LEN ];
}TSPdsEuro;

typedef struct STagEuro
{
   int  nPresent  [ MAX_EURO_CHIP_TAG ];
   int  nPosTag   [ MAX_EURO_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_EURO_CHIP_LEN ];
}TSTagEuro;

void  InitEuroInfo(TSEuroInfo * msgInfo);

int   AnalyseEuro    (char * buffer_rec, TSEuroInfo * msgInfo);
int   GetEuroField   (int    field_no,   TSEuroInfo * msgInfo, 
                                                  char * data, int *length);
int   AddEuroField   (int    field_no,   TSEuroInfo * msgInfo, 
                                                  char * data, int length);
int   InsertEuroField   (int    field_no,   TSEuroInfo * msgInfo, 
                                                  char * data, int length);
int   PutEuroField   (int    field_no,   TSEuroInfo * msgInfo, 
                                                  char * data, int length);
int   EuroBuildMsg (char * buffer_snd, TSEuroInfo *msgInfo);

void  InitEuroIcTag(TSTagEuro * tagInfo);
int   AnalyseEuroIc(char * buffer, int nLength, TSTagEuro * tagInfo);
int   GetEuroIcTag (char *tag_name, TSTagEuro *tagInfo,char *data, int *length);
int   AddEuroIcTag (char *tag_name, TSTagEuro *tagInfo, char *data, int length);
int   PutEuroIcTag (char *tag_name, TSTagEuro *tagInfo, char *data, int length);
int   EuroBuildIcFld (char * buffer_snd, TSTagEuro *tagInfo);

void  InitEuroPds(TSPdsEuro * pdsInfo);
int   AnalyseEuroPds(char *buffer,int nLength,int nMsgType,TSPdsEuro *pdsInfo);
int   GetEuroPds (char *pds_name, TSPdsEuro *pdsInfo,char *data, int *length);
int   AddEuroPds (char *pds_name, TSPdsEuro *pdsInfo, char *data, int length);
int   PutEuroPds (char *pds_name, TSPdsEuro *pdsInfo, char *data, int length);
int   EuroBuildPds (char * buffer_snd, TSPdsEuro *pdsInfo);

#endif /** HPS_ISO_EURO_H **/
