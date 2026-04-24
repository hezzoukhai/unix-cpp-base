#ifndef HPS_DBL_PARAM_H
#define HPS_DBL_PARAM_H
#include <thread_var.h>
#include <dump.h>
#include <comms_param.h>
#include <purge_time.h>
#include <troubles_param.h>
#include <dblglobls.h>
#include <define.h>

/* Resource Common Parameters ***/
char  ResourceId         [ 6 +1];
char  ResourceName       [15 +1];       

char  ResourceStatus     [ 3 +1];
char  ResourceInternalId [32 +1];

char  AcquirerBank [6 +1]; 
/* Resource Specific Parameters */
char  terminal           [10 +1];
char  Coord_nbr_general  [ 5 ];

char   buffer_rec[LG_MAX];
int    lng;
int    gTransLanguage;
int    gPrintFlag;

#define GN_DBL_EMV_STATES             1
#define GN_DBL_EMV_AIDS               2
#define GN_DBL_EMV_SCHEME_TAGS        3
#define GS_DBL_EMV_BUFFER             "\x1C\x39\x30\x37\x37"
#define GS_DBL_SMART_CARD_OP_ID       "\x1F\x53\x56"

#define MAX_PRODUIT        20

typedef struct SProduit
{
   int    ProduitId   ;
   char   ProduitCode [ 5 ];
}TSProduit;

TSProduit  tab_Produit[MAX_PRODUIT];

#endif /*** HPS_DBL_PARAM_H ***/


