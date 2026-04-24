#ifndef HPS_STRUCT_VRS
#define HPS_STRUCT_VRS




typedef struct SVrsSignOut
{
   char   Terminal_number   [ 4 + 1];
   char   aba_number        [ 3 + 1];
   char   branch_number     [ 4 + 1];
} TSVrsSignOut;

typedef struct SVrsSignIn
{
   char   operator_number   [ 4 + 1];
   char   password          [ 5 + 1];
   char   service           [ 2 + 1];
   char   date              [ 2 + 1];
} TSVrsSignIn;

typedef struct SVrsInfo
{
   char   merchant_number   [10 + 1];
   char   card_number       [16 + 1];
   char   expiry_date       [ 4 + 1];
   char   amount            [ 8 + 1];
   char   trans_type        [ 1 + 1];
   char   text              [30 + 1];
   char   field_sc          [ 2 + 1];
   char   orig_recpt_num    [ 8 + 1];
   char   mcc               [ 4 + 1];
   char   merchant_name     [21 + 1];
   char   cur_code          [ 3 + 1];
   char   merchant_reqs     [ 3 + 1];
   char   available_bal     [12 + 1];
   char   card_auths        [11 + 1];
   char   resid_status      [16 + 1];
   char   card_status       [24 + 1];
   char   cardholder_status [18 + 1];
   char   response          [53 + 1];
   char   origin_code        [ 1 + 1];
   char   response_time     [ 3 + 1];
   char   seq_number        [ 4 + 1];
   char   trans_date        [ 4 + 1];
   char   user              [ 3 + 1];
   char   osn               [ 6 + 1];
   char   sys_date          [ 9 + 1];
   int    nDigitAmount;
} TSVrsInfo;

#endif /** HPS_STRUCT_VRS **/
