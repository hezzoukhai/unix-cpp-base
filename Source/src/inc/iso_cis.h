#ifndef HPS_ISO_CIS_H
#define HPS_ISO_CIS_H

#include <iso_com.h>
#include <cis_fields.h>
#include <cis_ictag.h>
#include <cis_pdstag.h>
#include <sys/time.h>

/***** Types de Champs  ******/
#define   CIS_FIX_HEX       0
#define   CIS_FIX_ALPHA     1
#define   CIS_FIX_BIN       2
#define   CIS_LL_HEX        3
#define   CIS_LL_ALPHA      4
#define   CIS_LL_BIN        5
#define   CIS_LLL_HEX       6
#define   CIS_LLL_ALPHA     7
#define   CIS_LLL_BIN       8

/***** Types des TAG (Champ 55)  ******/
#define   CIS_TAG_FIX_BIN       0
#define   CIS_TAG_VAR_BIN       1
#define   CIS_TAG_FIX_BCD       2
#define   CIS_TAG_VAR_BCD       3
#define   CIS_TAG_FIX_ALPHA     4
#define   CIS_TAG_VAR_ALPHA     5
#define   CIS_TAG_VAR_HEX		6


/***** Types des PDS (Champ 48)  ******/
#define   CIS_PDS_FIX_ALPHA     0
#define   CIS_PDS_VAR_ALPHA     1

/**** Longueur de champs  ****/
#define CIS_BITMAP_LEN           16
#define MAX_CIS_FIELDS          128
#define MAX_CIS_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_CIS_CHIP_TAG       100
#define  MAX_CIS_CHIP_LEN       999

/**** PDS DATA   ****/
#define  MAX_CIS_PDS       100
#define  MAX_CIS_PDS_LEN   999

/***  CIS ENCODING ***/
extern char  CisEncoding  [ ];  /*NAB27032020*/

typedef struct SCisInfo
{
   int              nFieldPos    [ MAX_CIS_FIELDS  +1 ];
   int              nMsgType;
   int              nLength;
   /** char          sBitMap      [ CIS_BITMAP_LEN   ]; **/
   char             sBitMap      [ CIS_BITMAP_LEN + 1 ];
   char             sData        [ MAX_CIS_DATA_LEN ];
   struct timeval   kCtime; /*PLUTONL-4411*/
}TSCisInfo;

typedef struct SPdsCis
{
   int  nPresent  [ MAX_CIS_PDS ];
   int  nPosPds   [ MAX_CIS_PDS ];
   int  nLength;
   char sPdsData  [ MAX_CIS_PDS_LEN ];
   int	nMsgType;
}TSPdsCis;

typedef struct STagCis
{
   int  nPresent  [ MAX_CIS_CHIP_TAG ];
   int  nPosTag   [ MAX_CIS_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_CIS_CHIP_LEN ];
}TSTagCis;

void  InitCisInfo(TSCisInfo * msgInfo);

int   AnalyseCis    (char * buffer_rec, TSCisInfo * msgInfo);
int   GetCisField   (int    field_no,   TSCisInfo * msgInfo, 
                                                  char * data, int *length);
int   AddCisField   (int    field_no,   TSCisInfo * msgInfo, 
                                                  char * data, int length);
int   InsertCisField   (int    field_no,   TSCisInfo * msgInfo, 
                                                  char * data, int length);
int   PutCisField   (int    field_no,   TSCisInfo * msgInfo, 
                                                  char * data, int length);
int   CisBuildMsg (char * buffer_snd, TSCisInfo *msgInfo);

/***** Prototypes des fonctions  TAG ICC ****/
void  InitCisIcTag(TSTagCis * tagInfo);
int   AnalyseCisIc(char * buffer, int nLength, TSTagCis * tagInfo);
int   GetCisIcTag (char *tag_name, TSTagCis *tagInfo,char *data, int *length);
int   AddCisIcTag (char *tag_name, TSTagCis *tagInfo, char *data, int length);
int   PutCisIcTag (char *tag_name, TSTagCis *tagInfo, char *data, int length);
int   CisBuildIcFld (char * buffer_snd, TSTagCis *tagInfo);

/***** Prototypes des fonctions  TAG PDS (champ 48) ****/
void  InitCisPds(TSPdsCis * pdsInfo);
/*int   AnalyseCisPds(char *buffer, int nLength, TSPdsCis *pdsInfo);*/
int AnalyseCisPds(char *buffer, int nLength, int nMsgType, TSPdsCis *pdsInfo);
int   GetCisPds (char *pds_name, TSPdsCis *pdsInfo,char *data, int *length);
int   AddCisPds (char *pds_name, TSPdsCis *pdsInfo, char *data, int length);
int   PutCisPds (char *pds_name, TSPdsCis *pdsInfo, char *data, int length);
int   CisBuildPds (char * buffer_snd, TSPdsCis *pdsInfo);



#endif /** HPS_ISO_CIS_H **/
