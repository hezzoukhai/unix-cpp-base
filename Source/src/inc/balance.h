#ifndef HPS_BALANCE_H
#define HPS_BALANCE_H

typedef struct SBalance
{
 char     account_number		[ 24];
 char     bank_code			[ 6];
 char     branch_code			[ 6];
 char     client_code			[ 24];
 double   balance;
 double   facilities;
 double   blocked_amount;
 double   current_trn;
 double   current_auth;
 double   pending_trn;
 double   pending_auth;
 char     trn_cut_off_date		[ 8];
 char     trn_cut_off_id		[ 8];
 char     bal_cut_off_date		[ 8];
 char     bal_cut_off_id		[ 8];
 char     currency_code			[ 3];
 char     account_status		[ 1];
 char     last_autho_accept_date	[ 8];
 char     last_autho_refus_date		[ 8];
}TSBalance;

#endif /** HPS_BALANCE_H **/
