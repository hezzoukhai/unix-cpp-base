#ifndef HPS_LIMITS_SETUP_H
#define HPS_LIMITS_SETUP_H

typedef struct SLimitsSetup
{

 char     limit_index	[ 4];
 char     limits_id	[ 1];
 char     currency_code	[ 3];
 char     host_scenario_processing	[ 1];
 double   off_daily_onus_amnt	; 
 int      off_daily_onus_nbr	; 
 double   off_daily_nat_amnt	; 
 int      off_daily_nat_nbr	; 
 double   off_daily_internat_amnt	; 
 int      off_daily_internat_nbr	; 
 double   off_daily_tot_amnt	; 
 long     off_daily_tot_nbr	; 
 double   off_per_onus_amnt	; 
 int      off_per_onus_nbr	; 
 double   off_per_nat_amnt	; 
 int      off_per_nat_nbr	; 
 double   off_per_internat_amnt	; 
 int      off_per_internat_nbr	; 
 double   off_per_tot_amnt	; 
 long     off_per_tot_nbr	; 
 double   on_daily_onus_amnt	; 
 int      on_daily_onus_nbr	; 
 double   on_daily_nat_amnt	; 
 int      on_daily_nat_nbr	; 
 double   on_daily_internat_amnt	; 
 int      on_daily_internat_nbr	; 
 double   on_daily_tot_amnt	; 
 long     on_daily_tot_nbr	; 
 double   on_per_onus_amnt	; 
 int      on_per_onus_nbr	; 
 double   on_per_nat_amnt	; 
 int      on_per_nat_nbr	; 
 double   on_per_internat_amnt	; 
 int      on_per_internat_nbr	; 
 double   on_per_tot_amnt	; 
 long      on_per_tot_nbr	;

} TSLimitsSetup;

#endif /** HPS_LIMITS_SETUP_H **/
