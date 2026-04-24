 

#ifndef HPS_CARD_ACTIVITY_OFFUS_H
#define HPS_CARD_ACTIVITY_OFFUS_H


typedef struct SCardActivityOffus
{

 char     card_number	[ 22];
 char     bank_code	[ 6];
 char     service_code	[ 3];
 int      pin_verification_number;
 int      pin_error_cum;
 char     last_outlet_number	[ 15];
 char     last_card_acceptor_mcc	[ 4];
 char     last_card_acceptor_term_id	[ 15];
 char     last_country_code	[ 3];
 double   cycle_amount;
 int      cycle_start;
 int      last_usage_date;
 char     start_validity_date	[  8];
 char     end_validity_date	[  8];
 char     last_activity_date	[  8];
 char     last_app_auth_date	[  8];
 double   last_app_auth_amount	;
 char     last_dec_auth_date	[  8];
 double   last_dec_auth_amount	;
 char     last_can_auth_date	[  8];
 double   last_can_auth_amount	;
 char     last_code_action	[ 3];
 char     last_code_event	[ 3];
 char     last_service	[ 13];
 double   daily_amount;
 int      daily_number;
 double   periodic_amnt;
 int      periodic_number;

}TSCardActivityOffus;

#endif /** HPS_CARD_ACTIVITY_OFFUS_H **/



