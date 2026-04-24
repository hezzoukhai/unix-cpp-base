#ifndef HPS_CARD_PRODUCT_H
#define HPS_CARD_PRODUCT_H

typedef struct SCardProduct
{

 char     bank_code	[ 6];
 char     product_code	[ 3];
 char     on_us_product_flag	[ 1];
 char     product_type	[ 2];
 char     network_code	[ 2];
 char     network_card_type	[ 2];
 char     service_code	[ 3];
 char     business_card_flag	[ 1];
 char     cobranded_card_flag	[ 1];
 char     purchase_card_flag	[ 1];
 char     services_setup_index	[ 4];
 char     vip_response_translation	[ 4];
 char     currency_code	[ 3];
 char     limits_indexes	[ 4];
 char     periodicity_code	[ 3];
 char     enable_card_limits_exception	[ 1];
 char     prod_pin_offset_pvv	[ 3];
 char     prod_cvv1	[ 1];
 char     prod_cvv2	[ 1];
 char     prod_ccd	[ 1];
 char     prod_telecode	[ 1];
 char     off_line_atm_period	[ 3];
 double   off_line_atm_limit_onus;
 double   off_line_atm_limit_offus;
 char     rot_mem_option	[ 1];
 char     rot_mem_ctrl_limit	[ 1];
 char     rot_mem_ctrl_available	[ 1];
 char     rot_mem_scanf_iso2iso3	[ 1];
 char     rot_mem_last_usage_date	[ 1];
 char     iso3_smart_proc_mode	[ 7];
 char     chk_exp_date_opt	[ 1];
 char     chk_start_exp_date_opt	[ 1];
 char     chk_pin_opt	[ 3];
 char     chk_cvv1_opt	[ 1];
 char     chk_cvv2_opt	[ 1];
 char     delivery_flag	[ 1];
 char     card_carrier_option	[ 1];
 char     advice_option	[ 1];
 int      validity_code_first;
 int      validity_code_renewal;
 char     charging_condition	[ 3];
 char     settlement_period	[ 1];
 char     settlement_period_cash	[ 1];
 char     markup_code	[ 2];
 char     scenario_code	[ 1];
 char     aut_lvl_card	[ 1];
 char     aut_lvl_banking_account	[ 1];
 char     aut_lvl_client	[ 1];
 char     aut_lvl_shadow_account	[ 1];
 char     iso3_smart_option	[ 1];
 char     default_language_code	[ 3];

} TSCardProduct;

#endif /** HPS_CARD_PRODUCT_H **/
