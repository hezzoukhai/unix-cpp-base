/*******************************************************************************/
/*  (c) Hightech Payment Systems 2005                                          */
/*  NAME                                                                       */
/*      iso_iccprod.h                                                          */
/*                                                                             */
/* FUNCTION                                                                    */
/*         Management emv fields that have tlv format (file card production)   */
/* NOTES                                                                       */
/*                                                                             */
/* INITIAL VERSION                                                             */
/*        Y.LAMRANI   25/01/2005 -         V 2.2.0                             */
/*******************************************************************************/

#ifndef HPS_ISO_ICCPROD_H
#define HPS_ISO_ICCPROD_H

#include <iso_com.h>
#include <iccprod_fields.h>
#include <iccprod_ictag.h>
#include <tlv_private_emv.h>

/***** Types de Champs  ******/
#define   ICCPROD_FIX_HEX       0
#define   ICCPROD_FIX_ALPHA     1
#define   ICCPROD_FIX_BIN       2
#define   ICCPROD_LL_HEX        3
#define   ICCPROD_LL_ALPHA      4
#define   ICCPROD_LL_BIN        5
#define   ICCPROD_LLL_HEX       6
#define   ICCPROD_LLL_ALPHA     7
#define   ICCPROD_LLL_BIN       8

/***** Types des TAG (Champ 55)  ******/
#define   ICCPROD_TAG_FIX_BIN       0
#define   ICCPROD_TAG_VAR_BIN       1
#define   ICCPROD_TAG_FIX_BCD       2
#define   ICCPROD_TAG_VAR_BCD       3
#define   ICCPROD_TAG_FIX_ALPHA     4
#define   ICCPROD_TAG_VAR_ALPHA     5
#define   ICCPROD_TAG_VAR_HEX       6
#define   ICCPROD_TAG_VAR_EBR2      7
#define   ICCPROD_TAG_VAR_EBR3      8 
#define   ICCPROD_TAG_VAR_ALPHA_EBR3     9 


/**** Longueur de champs  ****/
#define ICCPROD_BITMAP_LEN           16
#define MAX_ICCPROD_FIELDS          128
#define MAX_ICCPROD_DATA_LEN       2048

/**** CHIP DATA   ****/
#define  MAX_ICCPROD_CHIP_TAG       100
#define  MAX_ICCPROD_CHIP_LEN       4000

/**** PDS DATA   ****/
#define  MAX_ICCPROD_PDS       100
#define  MAX_ICCPROD_PDS_LEN   999

typedef struct SIccProdInfo
{
   int           nFieldPos    [ MAX_ICCPROD_FIELDS  +1 ];
   int           nMsgType;
   int           nLength;
   char          sBitMap      [ ICCPROD_BITMAP_LEN   ];
   char          sData        [ MAX_ICCPROD_DATA_LEN ];
}TSICCPRODInfo;

typedef struct SPdsIccProd
{
   int  nPresent  [ MAX_ICCPROD_PDS ];
   int  nPosPds   [ MAX_ICCPROD_PDS ];
   int  nLength;
   char sPdsData  [ MAX_ICCPROD_PDS_LEN ];
   int	nMsgType;
}TSPdsIccProd;

typedef struct STagIccProd
{
   int  nPresent  [ MAX_ICCPROD_CHIP_TAG ];
   int  nPosTag   [ MAX_ICCPROD_CHIP_TAG ];
   int  nLength;
   char sChipData [ MAX_ICCPROD_CHIP_LEN ];
}TSTagIccProd;

/***** Prototypes des fonctions  TAG ICC ****/
void  InitIccProdIcTag(TSTagIccProd * tagInfo);
int   AnalyseIccProdIc(char * buffer, int nLength, TSTagIccProd * tagInfo);
int   GetIccProdIcTag (char *tag_name, TSTagIccProd *tagInfo,char *data, int *length);
int   AddIccProdIcTag (char *tag_name, TSTagIccProd *tagInfo, char *data, int length);
int   PutIccProdIcTag (char *tag_name, TSTagIccProd *tagInfo, char *data, int length);

#endif /** HPS_ISO_ICCPROD_H **/
