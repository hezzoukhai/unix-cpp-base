#ifndef HPS_EXCEPTION_FILE_H
#define HPS_EXCEPTION_FILE_H

typedef struct SExceptionFile
{

 char     card_number	[ 22];
 char     bank_code	[ 6];
 char     exception_start_date	[ 8];
 char     exception_end_date	[ 8];
 char     routing_code	[ 2];
 char     code_action	[ 3];
 char     services_setup_index	[ 4];
 char     vip_response_translation	[ 4];
 char     limits_indexes	[ 4];
 char     periodicity_code	[ 3];
 char     chk_exp_date_opt	[ 1];
 char     chk_start_exp_date_opt	[ 1];
 char     chk_pin_opt	[ 1];
 char     chk_cvv1_opt	[ 1];
 char     chk_cvv2_opt	[ 1];
 char     markup_code	[ 2];
 char     scenario_code	[ 1];
 char     aut_lvl_card	[ 1];
 char     aut_lvl_banking_account	[ 1];
 char     aut_lvl_client	[ 1];
 char     aut_lvl_shadow_account	[ 1];
}TSExceptionFile;

#endif  /** HPS_EXCEPTION_FILE_H  **/
