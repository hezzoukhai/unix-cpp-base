#ifndef HPS_SHADOW_ACCOUNT_ACTIVITY_H
#define HPS_SHADOW_ACCOUNT_ACTIVITY_H

typedef struct SShadowAccountActivity
{

 char     shadow_account_nbr	[ 24];
 char     bank_code	[ 6];
 char     client_code	[ 24];
 char     currency_code	[ 3];
 double     credit_limit;
 double     cash_transfer_limit;
 double     loan_limit;
 char     vip_level	[ 1];
 char     initial_profile_code	[ 3];
 char     current_profile_code	[ 3];
 int      loan_due_day;
 char     basic_shadow_account_flag	[ 1];
 char     basic_shadow_account_number	[ 24];

 double     credit_balance	;
 double     cash_transfer_balance;
 double     loan_balance	;
 double     pending_aut_credit	;
 double     pending_aut_cash_transfer	;
 double     pending_aut_loan	;
 double     live_deposit	;
 double     loyalty_point_balance;	
 double     last_pending_aut_credit	;
 double     last_pending_aut_cash_transfer	;
 double     last_pending_aut_loan	;
 double     last_live_deposit	;
 double     redemption_point_balance;	
 double     behaviour_point_balance;	

 char     date_last_aut_credit	[ 8];
 char     date_last_aut_cash_transfer	[ 8];
 char     date_last_aut_loan	[ 8];
 char     code_action	[ 3];
 char     last_code_action	[ 3];
 char     statement_number	[ 12];
 char     rowid_record	[ 18];
 char     sensitive_operation_record	[ 1];

}TSShadowAccountActivity;

#endif /** HPS_SHADOW_ACCOUNT_ACTIVITY_H **/
