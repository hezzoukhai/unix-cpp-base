  /***********************************************************************
  **  nom	: renewal.h						**
  ***********************************************************************/
#ifndef HPS_RENEWAL_H
#define HPS_RENEWAL_H

typedef struct SRenewalHeader
{
    char  record_type      [ 4];
    char  application_code [ 4];
    char  bank_nbr         [ 5];
    char  branch_nbr       [ 4];
    char  renewal_date     [ 9];
    char  product_code     [ 4];
}
TSRenewalHeader;

typedef struct SAccessCard
{
     char  dbi_key               [ 10 ];
     char  update_sync           [  4 ];
     char  renewal_date          [  9 ];
     char  primary_member_nbr    [  4 ];
     char  expiry_date           [  9 ];
     char  pin_nbr               [ 18 ];
     char  class_code            [  2 ];
     char  type_code             [  2 ];
     char  status_code           [  2 ];
     char  issue_level           [  4 ];
     char  member_acces_1        [  4 ];
     char  member_acces_2        [  4 ];

     char nbr_related_accounts   [  4 ];

     char account1_flag          [  4 ];
     char account1_appl_code     [  4 ];
     char account1_branch        [  4 ];
     char account1_nbr           [ 18 ];

     char account2_flag          [  4 ];
     char account2_appl_code     [  4 ];
     char account2_branch        [  4 ];
     char account2_nbr           [ 18 ];

     char account3_flag          [  4 ];
     char account3_appl_code     [  4 ];
     char account3_branch        [  4 ];
     char account3_nbr           [ 18 ];

     char account4_flag          [  4 ];
     char account4_appl_code     [  4 ];
     char account4_branch        [  4 ];
     char account4_nbr           [ 18 ];

     char filler                 [ 104];

} TSAccessCard;


typedef struct SCreditCard
{
     char  dbi_key               [ 10 ];
     char  update_sync           [  4 ];
     char  renewal_date          [  9 ];
     char  primary_member_nbr    [  4 ];
     char  expiry_date           [  9 ];
     char  pin_nbr               [ 18 ];
     char  class_code            [  2 ];
     char  type_code             [  2 ];
     char  status_code           [  2 ];
     char  issue_level           [  4 ];
     char  member_acces_1        [  4 ];
     char  member_acces_2        [  4 ];

     char nbr_related_accounts   [  4 ];

     char account1_flag          [  4 ];
     char account1_appl_code     [  4 ];
     char account1_branch        [  4 ];
     char account1_nbr           [ 18 ];

     char account2_flag          [  4 ];
     char account2_appl_code     [  4 ];
     char account2_branch        [  4 ];
     char account2_nbr           [ 18 ];

     char account3_flag          [  4 ];
     char account3_appl_code     [  4 ];
     char account3_branch        [  4 ];
     char account3_nbr           [ 18 ];

     char account4_flag          [  4 ];
     char account4_appl_code     [  4 ];
     char account4_branch        [  4 ];
     char account4_nbr           [ 18 ];

     char ledger_balance         [ 13 ];
     char auth_credit_line       [  9 ]; /* balance */
     char min_payment_amnt_due   [ 13 ];
     char account_currency       [  3 ];
     char acc_currency_conv_ind  [  1 ];
     char currency_discount      [  4 ]; 

     char filler                 [ 61 ];
}TSCreditCard;

typedef struct SDemandDeposit
{
     char  dbi_key               [ 10 ];
     char  renewal_date          [  9 ];

     char ledger_balance         [ 13 ];
     char float_amnt             [ 13 ];
     char collected_amnt         [ 13 ];
     char hold_amnt              [ 13 ];
     char interest_accured       [ 13 ];

     char  nb_rejected_items     [  4 ];
     char  class_code            [  2 ];
     char  type_code             [  2 ];
     char  status_code           [  2 ];

     char  overdraft_type        [  4 ];
     char  nbr_stop_payments     [  4 ];

     char account_currency       [  3 ];
     char acc_currency_conv_ind  [  1 ];
     char currency_discount      [  4 ]; 

     char filler                 [ 190];

}TSDemandDeposit;



typedef struct STimeDeposit
{
     char  dbi_key               [ 10 ];
     char  renewal_date          [  9 ];

     char ledger_balance         [ 13 ];
     char float_amnt             [ 13 ];
     char collected_amnt         [ 13 ];
     char hold_amnt              [ 13 ];
     char interest_accured       [ 13 ];

     char  nb_rejected_items     [  4 ];
     char  class_code            [  2 ];
     char  type_code             [  2 ];
     char  status_code           [  2 ];

     char  overdraft_type        [  4 ];
     char  nbr_stop_payments     [  4 ];

     char account_currency       [  3 ];
     char acc_currency_conv_ind  [  1 ];
     char currency_discount      [  4 ]; 

     char filler                 [ 190];

}TSTimeDeposit;


typedef struct SRenewalDetail
{
    char  record_type      [ 4];
    char  card_nbr         [18];
    char  delete_indicator [ 4];
    char  view_nbr         [ 4];
    char  recur_nbr        [ 4];
    union
    {
          TSAccessCard        acces_card;
          TSCreditCard       credit_card;
          TSDemandDeposit    demand_deposit;
          TSTimeDeposit      time_deposit;
    }view_data;
}
TSRenewalDetail;


typedef struct SRenewalTrailer
{
    char  record_type               [ 4];
    char  tot_nbr_detail_update     [ 9];
    char  tot_nbr_detail_add_or_del [ 4];
    char  tot_amount_bal_ledger     [13];
}
TSRenewalTrailer;

typedef struct SRenewal
{
     TSRenewalHeader   header;
     TSRenewalDetail   detail;
     TSRenewalTrailer  trailer;
} TSRenewal;

typedef struct SRenewalRow
{
   char  DeleteIndicator  [ 4 + 1];
   char  CardNbr          [18 + 1];
   char  ExpiryDate       [ 9 + 1];
   char  NbrAccounts      [ 4 + 1];
   char  AccountNbr1      [18 + 1];
   char  Account1Type     [ 4 + 1];
   char  AccountNbr2      [18 + 1];
   char  Account2Type     [ 4 + 1];
   char  AccountNbr3      [18 + 1];
   char  Account3Type     [ 4 + 1];
   char  AccountNbr4      [18 + 1];
   char  Account4Type     [ 4 + 1];
   char  StatusCode       [ 2 + 1];
   char  CurrencyCode     [ 3 + 1];
   char  Balance          [10 + 1];
} TSRenewalRow;

#endif /*** HPS_RENEWAL_H ***/
