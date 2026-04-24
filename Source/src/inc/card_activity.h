
#ifndef HPS_CARD_ACTIVITY_H
#define HPS_CARD_ACTIVITY_H


typedef struct SCardActivity
{

 char     card_number	[ 22];
 int      card_activity_set_index; /* Ce set indique quel set utiliser */

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
 char     start_validity_date	[ 4];
 char     end_validity_date	[ 4];
 char     last_activity_date	[ 8];
 char     last_app_auth_date	[ 8];
 double   last_app_auth_amount;
 char     last_dec_auth_date	[ 8];
 double   last_dec_auth_amount;
 char     last_can_auth_date	[ 8];
 double   last_can_auth_amount;
 char     last_code_action	[ 3];
 char     last_code_event	[ 3];
 char     last_service	[ 13];
 char     last_date_pin_error	[ 8];

 double   off_daily_onus_amnt_limit	; 
 int      off_daily_onus_nbr_limit	; 
 double   off_daily_nat_amnt_limit	; 
 int      off_daily_nat_nbr_limit	; 
 double   off_daily_internat_amnt_limit	; 
 int      off_daily_internat_nbr_limit	; 
 double   off_per_onus_amnt_limit	; 
 int      off_per_onus_nbr_limit	; 
 double   off_per_nat_amnt_limit	; 
 int      off_per_nat_nbr_limit	; 
 double   off_per_internat_amnt_limit	; 
 int      off_per_internat_nbr_limit	; 
 double   on_daily_onus_amnt_limit	; 
 int      on_daily_onus_nbr_limit	; 
 double   on_daily_nat_amnt_limit	; 
 int      on_daily_nat_nbr_limit	; 
 double   on_daily_internat_amnt_limit	; 
 int      on_daily_internat_nbr_limit	; 
 double   on_per_onus_amnt_limit	; 
 int      on_per_onus_nbr_limit	; 
 double   on_per_nat_amnt_limit	; 
 int      on_per_nat_nbr_limit	; 
 double   on_per_internat_amnt_limit	; 
 int      on_per_internat_nbr_limit	;

}TSCardActivity;

#endif /** HPS_CARD_ACTIVITY_H **/
