#ifndef HPS_ISO_BANK_H
#define HPS_ISO_BANK_H

#include <iso_com.h>
#include <bank_fields.h>
#include <bank_ictag.h>

/***** Types de Champs  ******/
#define   BANK_FIX_HEX       0
#define   BANK_FIX_ALPHA     1
#define   BANK_FIX_BIN       2
#define   BANK_LL_HEX        3
#define   BANK_LL_ALPHA      4
#define   BANK_LL_BIN        5
#define   BANK_LLL_HEX       6
#define   BANK_LLL_ALPHA     7
#define   BANK_LLL_BIN       8
#define   BANK_HDR_T24_LEN	 4 /** MBB 160224 SIPEM_HOST_BANK T24 INTEG FOR  */

/**** Longueur de champs  ****/
#define BANK_BITMAP_LEN           16
#define BANK_HDR_LEN              15
#define MAX_BANK_FIELDS          128
#define MAX_BANK_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_BANK_CHIP_TAG       100
#define  MAX_BANK_CHIP_LEN       999

/***** Types des TAG (Champ 55)  ******/
#define   BANK_TAG_FIX_BIN       0
#define   BANK_TAG_VAR_BIN       1
#define   BANK_TAG_FIX_BCD       2
#define   BANK_TAG_VAR_BCD       3
#define   BANK_TAG_FIX_ALPHA     4
#define   BANK_TAG_VAR_ALPHA     5
#define   BANK_TAG_VAR_HEX       6

typedef struct SBankInfo
{
   int           nFieldPos    [ MAX_BANK_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sHeader      [ BANK_HDR_LEN + 1 ]; 
   char          sBitMap      [ BANK_BITMAP_LEN   ];
   char          sData        [ MAX_BANK_DATA_LEN ];
}TSBankInfo;

typedef struct STagBank
{
   int  nPresent  [ MAX_BANK_CHIP_TAG ];
   int  nPosTag   [ MAX_BANK_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_BANK_CHIP_LEN ];
}TSTagBank;

void  InitBankInfo(TSBankInfo * msgInfo);

int   AnalyseBank    (char * buffer_rec, TSBankInfo * msgInfo);
int   GetBankField   (int    field_no,   TSBankInfo * msgInfo, 
                                                  char * data, int *length);
int   AddBankField   (int    field_no,   TSBankInfo * msgInfo, 
                                                  char * data, int length);
int   InsertBankField   (int    field_no,   TSBankInfo * msgInfo, 
                                                  char * data, int length);
int   PutBankField   (int    field_no,   TSBankInfo * msgInfo, 
                                                  char * data, int length);
int   BankBuildMsg (char * buffer_snd, TSBankInfo *msgInfo);

int   BankBuildHeader(char*   pNewHeader,int sLen , TSBankInfo   *BankInfo);

void  InitBankIcTag(TSTagBank * msgInfo);
int   AnalyseBankIc(char * buffer, int nLength, TSTagBank * msgInfo);
int   GetBankIcTag (char *tag_name, TSTagBank * msgInfo, char *data, int *length);
int   AddBankIcTag (char *tag_name, TSTagBank * msgInfo, char *data, int length);
int   PutBankIcTag (char *tag_name, TSTagBank * msgInfo, char *data, int length);

#endif /** HPS_ISO_BANK_H **/
