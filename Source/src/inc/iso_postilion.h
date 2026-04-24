/**************************************************
** POSTILION  ISO8583-1987
**
**************************************************/
#include <iso_com.h>
#include <visa_fields.h>
#include <iso_puf.h>


/***** Types de Champs  ******/
#define   POSTILION_FIX_HEX       0
#define   POSTILION_FIX_ALPHA     1
#define   POSTILION_FIX_BIN       2
#define   POSTILION_LL_HEX        3
#define   POSTILION_LL_ALPHA      4
#define   POSTILION_LL_BIN        5
#define   POSTILION_LLL_HEX       6
#define   POSTILION_LLL_ALPHA     7
#define   POSTILION_LLL_BIN       8
#define   POSTILION_FIX_BITMAP    9
#define   POSTILION_VAR_BITMAP    10
#define   POSTILION_LLLLLL_HEX    11
#define   POSTILION_LLLLLL_ALPHA  12
#define   POSTILION_LLLLLL_BIN    13

/**** Longueur de champs  ****/
#define POSTILION_BITMAP_LEN           24
#define POSTILION_HEADER_LEN           15
#define MAX_POSTILION_FIELDS          192
/*#define MAX_POSTILION_DATA_LEN       2048*/
/*#define MAX_POSTILION_DATA_LEN       4096/*MGA_20151118_0207962*/
#define MAX_POSTILION_DATA_LEN       50000/*ABM Structure Data 17022020*/


typedef struct SPostilionInfo
{
   int            nFieldPos    [ MAX_POSTILION_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   char           sHeader      [ POSTILION_HEADER_LEN + 1 ];
   char           sBitMap      [ POSTILION_BITMAP_LEN   ];
   char           sData        [ MAX_POSTILION_DATA_LEN ];
}TSPostilionInfo;

void  InitPostilionInfo(TSPostilionInfo * msgInfo);