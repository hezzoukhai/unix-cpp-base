#ifndef PWC_ISO_AMEX__H
#define PWC_ISO_AMEX__H


#include <iso_com.h>
#include <visa_fields.h>
#include <iso_puf.h>


/***** Types de Champs  ******/
#define   AMEX_FIX_HEX       0
#define   AMEX_FIX_ALPHA     1
#define   AMEX_FIX_BIN       2
#define   AMEX_LL_HEX        3
#define   AMEX_LL_ALPHA      4
#define   AMEX_LL_BIN        5
#define   AMEX_LLL_HEX       6
#define   AMEX_LLL_ALPHA     7
#define   AMEX_LLL_BIN       8
#define   AMEX_FIX_BITMAP    9
#define   AMEX_VAR_BITMAP    10
#define   AMEX_LLLL_BIN      11       /*FZL20221101 PLUTONL-4979*/

/**** Longueur de champs  ****/
#define AMEX_BITMAP_LEN           24
#define AMEX_HEADER_LEN           15
#define MAX_AMEX_FIELDS          192
#define MAX_AMEX_DATA_LEN       2048

typedef struct SAmexInfo
{
   int            nFieldPos    [ MAX_AMEX_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   char	   	  sHeader      [ AMEX_HEADER_LEN + 1 ];
   char           sBitMap      [ AMEX_BITMAP_LEN    ];
   char           sData        [ MAX_AMEX_DATA_LEN  ];
}TSAmexInfo;

void  InitAmexInfo(TSAmexInfo * msgInfo);

#endif

