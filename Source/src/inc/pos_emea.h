#ifndef HPS_POS_EMEA_H
#define HPS_POS_EMEA_H


typedef struct SSEmeaMsg
{
    char        szTermNbr[13];
    char        szTrxNbr[5];
    char        szTermType[5];
    char        szMsgType[3];
    char        szMerchantNbr[10];
    char        szCardInf[41];
    char        szAmount[12];
} SEmeaMsg;

typedef struct SBatchHeaderEmea
{
  char sDevice                [ 1 ];
  char sRecordType            [ 1 ];
  char sTrxCode               [ 1 ];
  char sBatchDate             [ 4 ];
  char sBatchNumber           [ 5 ];
  char sFiller                [ 2 ];
  char sMerchantNbr           [ 10];
  char sStoreNbr              [ 4 ];
  char sMerTermNbr            [ 4 ];
  char sBlockFact             [ 4 ];
  char sCrncyCode             [ 3 ];
  char sBankCode              [ 24];
  char sTermNbr               [ 10];
  char sFiller2               [ 7 ];
} sBatchHeaderEmea;

typedef struct SBatchParamEmea
{
  char sDevice                [ 1 ];
  char sRecordType            [ 1 ];
  char sTrxCode               [ 1 ];
  char sStoreNbr              [ 4 ];
  char sMerTermNbr            [ 4 ];
  char sMerchName             [ 25 ];
  char sMerchCity             [ 13 ];
  char sMerchState            [ 2  ];
  char sMerchMcc              [ 4  ];
  char sMerchLoc              [ 4  ];
  char sTermSer               [ 8  ];
  char sMerchZip              [ 5  ];
  char sCntryCode             [ 3  ];
  char sBatchCredit           [ 1  ];
  char sTrxCredit             [ 1  ];
  char sBatchDebit            [ 1  ];
  char sTrxDebit              [ 1  ];
  char sVoid                  [ 1  ];
} sBatchParamEmea;

typedef struct SBatchDetailEmea
{
  char sDevice                [ 1 ];
  char sRecordType            [ 1 ];
  char sTrxCode               [ 1 ];
  char sStoreNbr              [ 4 ];
  char sMerTermNbr            [ 4 ];
  char sTrxDate               [ 4 ];
  char sCardNbr               [ 22 ];
  char sAccDataSource         [  1 ];
  char sCardIdMethod          [  1 ];
  char sTrxAmount             [ 12 ];
  char sVoidIndicator         [  1 ];
  char sAuthorSource          [  1 ];
  char sAuthorCode            [  6 ];
  char sRefNbr                [  4 ];
  char sTrxTime               [  4 ];
  char sCashBackAmnt          [ 12 ];
  char sFiller                [  1 ];
} sBatchDetailEmea;

typedef struct SBatchTrailerEmea
{
  char sDevice                [ 1 ];
  char sRecordType            [ 1 ];
  char sTrxCode               [ 1 ];
  char sBatchDate             [ 4 ];
  char sBatchNbr              [ 5 ];
  char sBatchCountTrx         [ 9 ];
  char sBatchAmntTotal        [ 16 ];
  char sFiller                [ 43 ];
} sBatchTrailerEmea;



/* Definition Parametre POS EMEA */
#define     ACKMSG            9 
#define     PURCHASE          10
#define     EMEA_REPEAT            16
#define     PURCHASE_KEYED    20
#define     PURCHASE_REPEAT   16
#define     PURCHASE_KEYED_REPEAT   26
#define     CASH_REPEAT   36
#define     CASH_KEYED_REPEAT   37
#define     PCASH             30
#define     PCASH_KEYED       31
#define		BATCH_UPLOAD      32
#define		EMEA_LEN_MSG      512
#define		PROCESSED         1  /* trx deja inserer dans autho activity     */
#define		NOT_PROCESSED     2  /* trx n'est pas inserer dans Autho_activity*/
#define		WAITING_UPD       3  /* trx doit faire la maj de autho_activity  */

#endif /*** HPS_POS_EMEA_H ***/

