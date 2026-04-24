/**************************************************
** POSTILION  ISO8583-1987
**
**************************************************/
#include <iso_com.h>
#include <visa_fields.h>
#include <iso_puf.h>


/***** Types de Champs  ******/
#define   POSTILION_F127_FIX_HEX       0
#define   POSTILION_F127_FIX_ALPHA     1
#define   POSTILION_F127_FIX_BIN       2
#define   POSTILION_F127_LL_HEX        3
#define   POSTILION_F127_LL_ALPHA      4
#define   POSTILION_F127_LL_BIN        5
#define   POSTILION_F127_LLL_HEX       6
#define   POSTILION_F127_LLL_ALPHA     7
#define   POSTILION_F127_LLL_BIN       8
#define   POSTILION_F127_FIX_BITMAP    9
#define   POSTILION_F127_VAR_BITMAP    10
#define   POSTILION_F127_LLLL_HEX      11
#define   POSTILION_F127_LLLL_ALPHA    12
#define   POSTILION_F127_LLLL_BIN      13
#define   POSTILION_F127_LLLLLL_HEX    14
#define   POSTILION_F127_LLLLLL_ALPHA  15
#define   POSTILION_F127_LLLLLL_BIN    16
#define   POSTILION_F127_LLLLL_ALPHA   17 /* MC 310507 */

/**** Longueur de champs  ****/
#define POSTILION_F127_BITMAP_LEN           24
#define POSTILION_F127_HEADER_LEN           15
/**Start Modif AKH 040907
#define MAX_POSTILION_F127_FIELDS           30 ************/
#define MAX_POSTILION_F127_FIELDS           32
/**Start Modif AKH 040907 ************/
/*#define MAX_POSTILION_F127_DATA_LEN       2048*/
/*#define MAX_POSTILION_F127_DATA_LEN       4096*//*MGA_20151118_0207962*/
#define MAX_POSTILION_F127_DATA_LEN       50000/*ABM  07022022 Add Structured Data field*/

typedef struct SPostilionF127Info
{
   int            nFieldPos    [ MAX_POSTILION_F127_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   char           sHeader      [ POSTILION_F127_HEADER_LEN + 1 ];
   char           sBitMap      [ POSTILION_F127_BITMAP_LEN   ];
   char           sData        [ MAX_POSTILION_F127_DATA_LEN ];
}TSPostilionF127Info;

void  InitPostilionF127Info(TSPostilionF127Info * msgInfo);