#ifndef ISO_BIC____H
#define ISO_BIC____H


#include <header_bic.h>
#include <bic_fields.h>
#include <iso_com.h>
#include <bic_ictag.h>

/** Start RHA Define Emv Tag Type **/
#define   BIC_TAG_FIX_BIN       0
#define   BIC_TAG_VAR_BIN       1
#define   BIC_TAG_FIX_BCD       2
#define   BIC_TAG_VAR_BCD       3
#define   BIC_TAG_FIX_ALPHA     4
#define   BIC_TAG_VAR_ALPHA     5
#define   BIC_TAG_VAR_HEX       6
#define   BIC_TAG_FIX_HEX       7

#define  MAX_BIC_CHIP_TAG       100
#define  MAX_BIC_CHIP_LEN       258


/***** Types de Champs  ******/

/***** Types de Champs  ******/
#define   BIC_FIX_HEX       0
#define   BIC_FIX_ALPHA     1
#define   BIC_FIX_BIN       2
#define   BIC_LL_HEX        3
#define   BIC_LL_ALPHA      4
#define   BIC_LL_BIN        5
#define   BIC_LLL_HEX       6
#define   BIC_LLL_ALPHA     7
#define   BIC_LLL_BIN       8
#define   BIC_VAR_ALPHA     9
#define   BIC_LLLL_ALPHA     20

/**RHA DEFINE TOKEN **/
#define TOKEN_B2           "B2"
#define TOKEN_B3           "B3"
#define TOKEN_B4           "B4" /* Nabil Abouloula 17112009 */

/**** Longueur de champs  ****/
#define BIC_BITMAP_LEN           16
#define MAX_BIC_FIELDS          128
#define MAX_BIC_DATA_LEN       2048

#define BIC_ICC_BITMAP_LEN           4 
#define BIC_MAX_ICC_TAG				14 


typedef struct SBicIccInfo
{
   int            nFieldPos    [ BIC_MAX_ICC_TAG  +1 ];
   int            nLength;
   char           sBitMap      [ BIC_ICC_BITMAP_LEN   ];
   char           sData        [ MAX_BIC_DATA_LEN ];
}TSBicIccInfo;

/*****BHS080409BEGIN***************/
typedef struct SBicInfo
{
   int            nFieldPos    [ MAX_BIC_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   TSBicHeader   sHeader;
   char           sBitMap      [ BIC_BITMAP_LEN   ];
   char           sData        [ MAX_BIC_DATA_LEN ];
}TSBicInfo;

void  InitBicInfo(TSBicInfo * msgInfo);

int   AnalyseBic    (char * buffer_rec, TSBicInfo * msgInfo);
int   GetBicField   (int    field_no,   TSBicInfo * msgInfo, 
                                                   char * data, int *length);
int   InsertBicField(int    field_no,   TSBicInfo * msgInfo, 
                                                   char * data, int length);
int   PutBicField   (int    field_no,   TSBicInfo * msgInfo, 
                                                   char * data, int length);
int   AddBicField   (int    field_no,   TSBicInfo * msgInfo, 
                                                   char * data, int length);
int   BicBuildMsg   ( char * buffer_snd,   TSBicInfo * msgInfo);

void BicBuildHeader(int     	    nReqResp,
                     TSBicHeader*  pNewHeader,
                     TSBicHeader*  pIncomingHeader,
                     char* 	    pBicProduct);

/**Start RHA Define Emv Fct Mng**/
typedef struct STagBic
{
   int  nPresent  [ MAX_BIC_CHIP_TAG ];
   int  nPosTag   [ MAX_BIC_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_BIC_CHIP_LEN ];
}TSTagBic;


void  InitBicIcTag(TSTagBic * tagInfo);
int   AnalyseBicIc(char * buffer, int nLength, TSTagBic * tagInfo);
int   GetBicIcTag (char *tag_name, TSTagBic *tagInfo,char *data, int *length);
int   AddBicIcTag (char *tag_name, TSTagBic *tagInfo, char *data, int length);
int   PutBicIcTag (char *tag_name, TSTagBic *tagInfo, char *data, int length);
int   BicBuildIcFld (char * buffer_snd, TSTagBic *tagInfo);


/** Start BHS**/

void  InitBicIccInfo(TSBicIccInfo * msgInfo);
int GetBicIccField(int field_no, TSBicIccInfo * msgInfo, char *data, int *nLength);
int AddBicIccField(int field_no, TSBicIccInfo *msgInfo, char *data, int nLength);
int BicBuildIccMsg (char * buffer_snd, TSBicIccInfo *msgInfo);


#endif


