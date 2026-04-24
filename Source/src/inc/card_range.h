#ifndef HPS_CARD_RANGE_H
#define HPS_CARD_RANGE_H

typedef struct SCardRange
{
 char     card_nbr  	        [ 22];
 char     issuing_bank_code	[ 6];
 char     issuer_bin	[ 11];
 char     issuer_ica	[ 4];
 char     network_code	[ 2];
 int      card_number_length_min;
 int      card_number_length_max;
 char     get_product_data_level	[ 1];
 char     get_card_data_level	[ 1];
 char     single_product_flag	[ 1];
 char     product_code	[ 3];
 char     services_setup_index	[ 4];
 char     vip_response_translation	[ 4];
 char     currency_code	[ 3];
 char     limits_indexes	[ 4];
 char     periodicity_code	[ 3];
 int      primary_pin_key_number;
 char     network_card_type	[ 2];
 int      alternate_pin_key_number;
 char     start_pin_alternate_key	[ 8];
 char     primary_cvv_key_number	[ 3];
 char     alternate_cvv_key_number	[ 3];
 char     start_cvv_alternate_key	[ 8];
 char     exception_cvv_key_number	[ 3];
 char     exception_cvv_key_valdate	[ 8];
 int      pvki;
 int      key_set_number_1;
 int      key_set_number_2;
 int      key_set_number_3;
 int      key_set_number_4;
 int      key_set_number_5;
 int      key_set_number_6;
 int      key_set_number_7;
 int      key_set_number_8;
 int      key_set_number_9;
 int      pin_retry_max;
 char     def_prod_pin_offset_pvv	[ 3];
 char     def_prod_cvv1	[ 1];
 char     def_prod_cvv2	[ 1];
 char     def_prod_ccd	[ 1];
 char     def_prod_telecode	[ 1];
 char     def_prod_encod_iso_1	[ 1];
 char     def_prod_encod_iso_2	[ 1];
 char     def_prod_encod_iso_3	[ 1];
 char     def_prod_encod_ship	[ 1];
 char     def_off_line_atm_period	[ 3];
 double   def_off_line_atm_limit_onus;
 double   def_off_line_atm_limit_offus;
 char     def_iso3_smart_option	[ 1];
 char     def_rot_mem_option	[ 1];
 char     def_rot_mem_ctrl_limit	[ 1];
 char     def_rot_mem_ctrl_available	[ 1];
 char     def_rot_mem_scanf_iso2iso3	[ 1];
 char     def_rot_mem_last_usage_date	[ 1];
 char     def_iso3_smart_proc_mode	[ 7];
 char     def_chk_exp_date_opt	[ 1];
 char     def_chk_start_exp_date_opt	[ 1];
 char     def_chk_pin_opt	[ 1];
 char     def_chk_cvv1_opt	[ 1];
 char     def_chk_cvv2_opt	[ 1];
 char     def_language_code	[ 3];

}TSCardRange;


#endif  /** HPS_CARD_RANGE_H **/
