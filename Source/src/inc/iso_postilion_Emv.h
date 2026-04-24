
/**************************************************
** POSTILION  ISO8583-1987
**
**************************************************/
#include <iso_com.h>
#include <visa_fields.h>
#include <iso_puf.h>


/***** Types de Champs  ******/
#define   POSTILION_EMV_FIX_HEX       0
#define   POSTILION_EMV_FIX_ALPHA     1
#define   POSTILION_EMV_FIX_BIN       2
#define   POSTILION_EMV_LL_HEX        3
#define   POSTILION_EMV_LL_ALPHA      4
#define   POSTILION_EMV_LL_BIN        5
#define   POSTILION_EMV_LLL_HEX       6
#define   POSTILION_EMV_LLL_ALPHA     7
#define   POSTILION_EMV_LLL_BIN       8
#define   POSTILION_EMV_FIX_BITMAP    9
#define   POSTILION_EMV_VAR_BITMAP    10
#define   POSTILION_EMV_LLLL_HEX      11
#define   POSTILION_EMV_LLLL_ALPHA    12
#define   POSTILION_EMV_LLLL_BIN      13
#define   POSTILION_EMV_LLLLLL_HEX    14
#define   POSTILION_EMV_LLLLLL_ALPHA  15
#define   POSTILION_EMV_LLLLLL_BIN    16
#define   POSTILION_EMV_LLLLL_ALPHA   17
#define   POSTILION_EMV_L_ALPHA       18 

/**** Longueur de champs  ****/
#define POSTILION_EMV_BITMAP_LEN           24
#define POSTILION_EMV_HEADER_LEN           15
#define MAX_POSTILION_EMV_FIELDS           32
#define MAX_POSTILION_EMV_DATA_LEN       2048

typedef struct SPostilionEmvInfo
{
   int            nFieldPos    [ MAX_POSTILION_EMV_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   char           sHeader      [ POSTILION_EMV_HEADER_LEN + 1 ];
   char           sBitMap      [ POSTILION_EMV_BITMAP_LEN   ];
   char           sData        [ MAX_POSTILION_EMV_DATA_LEN ];
}TSPostilionEmvInfo;

void  InitPostilionEmvInfo(TSPostilionEmvInfo * msgInfo);

