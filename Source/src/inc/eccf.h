#ifndef PWC__ECCF__H
#define PWC__ECCF__H

typedef struct fich
{
   char	transaction_code                         [   5];
   char	bit_map_1                                [  17];
   char	bit_map_2                                [  17];
   char	primary_account_number                   [  20];
   char	processing_code                          [   7];
   char	amount_transaction                       [12+1];
   char	amount_reconciliation                    [12+1];
   char	amount_cardholder                        [12+1];
   char	conv_rate_reconc                         [   9];
   char	conv_rate_cardhold_bill                  [   9];
   char	system_trace_audit_number                [ 6+1];
   char	date_time_transaction                    [  13];
   char	date_expiration                          [   5];
   char	date_settlement                          [   7];
   char	date_conversion                          [   5];
   char	pos_data_code                            [  13];
   char	card_sequence_number                     [   4];
   char	function_code                            [ 3+1];
   char	message_reason_code                      [ 4+1];
   char	card_acceptor_business_code              [   5];
   char	date_reconciliation                      [   7];
   char	reconciliation_indicator                 [   4];
   char	amounts_original                         [24+1];
   char	acquiring_reference_data                 [  24];
   char	acquiring_inst_id_code                   [  12];
   char	fid_code                                 [  10];
   char	retrieval_reference_nb                   [  13];
   char	approval_code                            [   7];
   char	action_code                              [   4];
   char	card_acceptor_term_id                    [   9];
   char	card_acceptor_id_code                    [  16];
   char	card_acceptor_name_location              [  55];
   char	amount_fees                              [  63];
   char	ccy_code_transaction                     [   4];
   char	ccy_code_reconciliation                  [   4];
   char	ccy_code_cardhold_bill                   [   4];

   char	chip_appli_cryptogram       			 [ 16+1];
   char	chip_amount_trans           			 [ 12+1];
   char	chip_crypto_info_data       			 [  2+1];
   char	chip_issuer_appli_data      			 [ 64+1];
   char	chip_unpredictable_num      			 [  8+1];
   char	chip_appli_trans_counter    			 [  4+1];
   char	chip_terminal_verif_result  			 [ 10+1];
   char	chip_appli_interch_profile  			 [  4+1];
   char	chip_transaction_date       			 [  6+1];
   char	chip_transaction_type       			 [  2+1];
   char	chip_trans_ccy_code         			 [  3+1];
   char	chip_cardh_verif_meth_res   			 [  6+1];
   char	chip_term_capabilities      			 [  6+1];
   char	chip_term_country_code      			 [  3+1];
   char	chip_terminal_type          			 [  2+1];
   char	chip_trans_categ_code       			 [  2+1];
   char	chip_dedicated_file_name    			 [ 32+1];
   char	chip_term_appli_ver_num     			 [  4+1];
   char	chip_amount_other           			 [ 12+1];
   char	chip_interf_device_seri_no  			 [ 16+1];
   char	chip_trans_seq_counter      			 [ 16+1];

   char	original_data_elts                       [  36];
   char	message_number                           [ 8+1];
   char	data_record                              [1000];
   char	credits_number                           [10+1];
   char	credits_rev_number                       [10+1];
   char	debits_number                            [10+1];
   char	debits_rev_number                        [10+1];
   char	fee_collection_number                    [10+1];
   char	credits_amount                           [16+1];
   char	credits_rev_amount                       [16+1];
   char	debits_amount                            [16+1];
   char	debits_rev_amount                        [16+1];
   char	dest_inst_id                             [  12];
   char	orig_inst_id                             [  12];
   char	card_issuer_ref_data                     [  11];
   char	amount_net_reconc                        [  18];
   char	rid_code                                 [  10];
   char	file_name                                [  19];
   char	transaction_description                  [   5];
   char	credits_chargeback_amount                [16+1];
   char	debits_chargeback_amount                 [16+1];
   char	credits_chargeback_nb                    [10+1];
   char	debits_chargeback_nb                     [10+1];
   char	credits_fee_amount                       [  73];
   char	debits_fee_amount                        [  73];
   char	miscellaneous_data                       [1000];
   char	evans_private_data                       [ 126];
   char	invalid_account_nb_data                  [  68];
   char	user_create                              [  31];
   char	date_create                              [  11];
} enr_fich;

#endif


