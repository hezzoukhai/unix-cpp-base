
/**************************************************
** AMEX  ISO8583-1987
**
**************************************************/
#ifndef PWC_ISO_AMX_F127__H
#define PWC_ISO_AMX_F127__H

#include <iso_com.h>
#include <visa_fields.h>
#include <iso_puf.h>


/***** Types de Champs  ******/
#define   AMEX_F127_FIX_HEX       0
#define   AMEX_F127_FIX_ALPHA     1
#define   AMEX_F127_FIX_BIN       2
#define   AMEX_F127_LL_HEX        3
#define   AMEX_F127_LL_ALPHA      4
#define   AMEX_F127_LL_BIN        5
#define   AMEX_F127_LLL_HEX       6
#define   AMEX_F127_LLL_ALPHA     7
#define   AMEX_F127_LLL_BIN       8
#define   AMEX_F127_FIX_BITMAP    9
#define   AMEX_F127_VAR_BITMAP    10

/**** Longueur de champs  ****/
#define AMEX_F127_BITMAP_LEN           24
#define AMEX_F127_HEADER_LEN           15
#define MAX_AMEX_F127_FIELDS           20
#define MAX_AMEX_F127_DATA_LEN       2048

typedef struct SAmexF127Info
{
   int            nFieldPos    [ MAX_AMEX_F127_FIELDS  +1 ];
   int            nMsgType;
   int            nLength;
   char           sHeader      [ AMEX_F127_HEADER_LEN + 1 ];
   char           sBitMap      [ AMEX_F127_BITMAP_LEN   ];
   char           sData        [ MAX_AMEX_F127_DATA_LEN ];
}TSAmexF127Info;

void  InitAmexF127Info(TSAmexF127Info * msgInfo);

#endif


