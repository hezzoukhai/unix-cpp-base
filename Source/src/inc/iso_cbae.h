#ifndef ISO_CBAE__H
#define ISO_CBAE__H
/**************************************************
** CBAE  ISO8583-1987
**
**************************************************/
#include <iso_com.h>


/***** Types de Champs  ******/
#define   CBAE_FIX_BCD       0
#define   CBAE_FIX_ALPHA     1
#define   CBAE_FIX_BIN       2
#define   CBAE_VAR_BCD       3
#define   CBAE_VAR_SBCD      4
#define   CBAE_VAR_ALPHA     5
#define   CBAE_VAR_BIN       6
#define   CBAE_VAR_BITMAP    7
#define   CBAE_FIX_BITMAP    8
#define   CBAE_FIX_HEX       9
#define   CBAE_VAR_HEX      10
#define   CBAE_FIX_X_BCD    11 /*EBE130917*/
#define   CBAE_VAR2_BIN     12 /*AMER20190228 PROD00066569: Enh5.0 1062*/
#define   CBAE_VAR_HEX_2      13 /*PLUTONL-4280 Enh5.03*/
/**** Longueur de champs  ****/
#define CBAE_BITMAP_LEN				24
#define CBAE_HEADER_LEN				256
#define MAX_CBAE_FIELDS				192
#define MAX_CBAE_DATA_LEN			2048
/**** EMV UPGRADE OBE05082009***/
/***** Types des TAG (Champ 55)  ******/
#define   CBAE_TAG_FIX_BIN       0
#define   CBAE_TAG_VAR_BIN       1
#define   CBAE_TAG_FIX_BCD       2
#define   CBAE_TAG_VAR_BCD       3
#define   CBAE_TAG_FIX_ALPHA     4
#define   CBAE_TAG_VAR_ALPHA     5
#define   CBAE_TAG_VAR_HEX       6

/**** CHIP DATA   ****/
#define  MAX_CBAE_CHIP_TAG       100
#define  MAX_CBAE_CHIP_LEN       999


#define   VAR2_LEN_LEN		 2

/**** EMV UPGRADE OBE05082009***/
typedef struct SCbaeInfo
{
   int            nFieldPos    [ MAX_CBAE_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   char           sHeader      [ CBAE_HEADER_LEN + 1 ];
   char           sPi24        [ 64 + 1 ];
   int            LPi24;
   char           sBitMap      [ CBAE_BITMAP_LEN   ];
   char           sData        [ MAX_CBAE_DATA_LEN ];
}TSCbaeInfo;


/**** EMV UPGRADE OBE05082009***/
typedef struct STagCbae
{
   int  nPresent  [ MAX_CBAE_CHIP_TAG ];
   int  nPosTag   [ MAX_CBAE_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_CBAE_CHIP_LEN ];
}TSTagCbae;
/**** EMV UPGRADE OBE05082009***/
void  InitCbaeInfo(TSCbaeInfo * msgInfo);

#endif
