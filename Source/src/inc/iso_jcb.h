#ifndef HPS_ISO_JCB_H
#define HPS_ISO_JCB_H

#include <iso_com.h>
#include <jcb_fields.h>
#include <jcb_ictag.h>
#include <jcb_pdstag.h>

/***** Types de Champs  ******/
#define   JCB_FIX_HEX       0
#define   JCB_FIX_ALPHA     1
#define   JCB_FIX_BIN       2
#define   JCB_LL_HEX        3
#define   JCB_LL_ALPHA      4
#define   JCB_LL_BIN        5
#define   JCB_LLL_HEX       6
#define   JCB_LLL_ALPHA     7
#define   JCB_LLL_BIN       8
#define   JCB_FIX_BCD       9
#define   JCB_VAR_BCD      10
#define   JCB_VAR_SBCD     11
#define   JCB_VAR_DBCD     12
#define   JCB_LL_VAR_BIN   13
#define   JCB_LLL_VAR_BIN  14 /*-- MTR20180903 --*/


/***** Types des TAG (Champ 55)  ******/
#define   JCB_TAG_FIX_BIN       0
#define   JCB_TAG_VAR_BIN       1
#define   JCB_TAG_FIX_BCD       2
#define   JCB_TAG_VAR_BCD       3
#define   JCB_TAG_FIX_ALPHA     4
#define   JCB_TAG_VAR_ALPHA     5
#define   JCB_TAG_VAR_HEX		6


/***** Types des PDS (Champ 48)  ******/
#define   JCB_PDS_FIX_ALPHA     0
#define   JCB_PDS_VAR_ALPHA     1

/**** Longueur de champs  ****/
#define JCB_BITMAP_LEN           16
#define MAX_JCB_FIELDS          128
#define MAX_JCB_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_JCB_CHIP_TAG       100
#define  MAX_JCB_CHIP_LEN       999

/**** PDS DATA   ****/
#define  MAX_JCB_PDS       100
#define  MAX_JCB_PDS_LEN   999

typedef struct SJcbInfo
{
   int           nFieldPos    [ MAX_JCB_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ JCB_BITMAP_LEN   ];
   char          sData        [ MAX_JCB_DATA_LEN ];
}TSJcbInfo;

typedef struct SPdsJcb
{
   int  nPresent  [ MAX_JCB_PDS ];
   int  nPosPds   [ MAX_JCB_PDS ];
   int  nLength;
   char sPdsData  [ MAX_JCB_PDS_LEN ];
   int	nMsgType;
}TSPdsJcb;

typedef struct STagJcb
{
   int  nPresent  [ MAX_JCB_CHIP_TAG ];
   int  nPosTag   [ MAX_JCB_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_JCB_CHIP_LEN ];
}TSTagJcb;

void  InitJcbInfo(TSJcbInfo * msgInfo);

int   AnalyseJcb    (char * buffer_rec, TSJcbInfo * msgInfo);
int   GetJcbField   (int    field_no,   TSJcbInfo * msgInfo, 
                                                  char * data, int *length);
int   AddJcbField   (int    field_no,   TSJcbInfo * msgInfo, 
                                                  char * data, int length);
int   InsertJcbField   (int    field_no,   TSJcbInfo * msgInfo, 
                                                  char * data, int length);
int   PutJcbField   (int    field_no,   TSJcbInfo * msgInfo, 
                                                  char * data, int length);
int   JcbBuildMsg (char * buffer_snd, TSJcbInfo *msgInfo);

/***** Prototypes des fonctions  TAG ICC ****/
void  InitJcbIcTag(TSTagJcb * tagInfo);
int   AnalyseJcbIc(char * buffer, int nLength, TSTagJcb * tagInfo);
int   GetJcbIcTag (char *tag_name, TSTagJcb *tagInfo,char *data, int *length);
int   AddJcbIcTag (char *tag_name, TSTagJcb *tagInfo, char *data, int length);
int   PutJcbIcTag (char *tag_name, TSTagJcb *tagInfo, char *data, int length);
int   JcbBuildIcFld (char * buffer_snd, TSTagJcb *tagInfo);

/***** Prototypes des fonctions  TAG PDS (champ 48) ****/
void  InitJcbPds(TSPdsJcb * pdsInfo);

int AnalyseJcbPds(char *buffer, int nLength, int nMsgType, TSPdsJcb *pdsInfo);
int   GetJcbPds (char *pds_name, TSPdsJcb *pdsInfo,char *data, int *length);
int   AddJcbPds (char *pds_name, TSPdsJcb *pdsInfo, char *data, int length);
int   PutJcbPds (char *pds_name, TSPdsJcb *pdsInfo, char *data, int length);




#endif /** HPS_ISO_JCB_H **/
