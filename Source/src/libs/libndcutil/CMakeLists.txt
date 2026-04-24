# directory: libndcutil

set(lib_sources_ndc		down_load_field.c 
						format_field.c 
						hps_lib.c 
						insert_text.c
						ndc_tools_advice.c
						ndc_tools_convert.c
						util_varchar.c)

set(lib_sources_wnc			down_load_field.c 
							format_field.c 
							hps_lib.c 
							insert_text.c
							ndc_tools_advice.c
							ndc_tools_convert.c
							util_varchar.c)

set(lib_proc_src_ndc	ndc_atm_mng;
						atm_seq_mng;
						get_reply_data;
						hps_atm_status_mng;
						hps_atm_cutoff;
						ndc_update_cassettes;
						get_cassette_info;
						ndc_watch_config;
						get_fit;
						get_prints;
						get_denomination_info;
						get_profile;
						get_screen;
						get_state;
						get_msg_indiv_group;
						maj_watch_activity;
						update_log_arrette;
						pcrd_billing;
						pcrd_accounts;
						pcrd_host_accounts;
						pcrd_utility;
						pcrd_accounts_by_type;
						pcrd_creditPayment;
						pcrd_services;
						get_action_info;
						ndc_util;
						ndc_emv_data;
						hps_atm_additional_periph_mng;
						pcrd_donations;
						pcrd_sub_donations;
						#pcrd_shadow_account_un_am
						)
						
set(lib_proc_src_wnc	ndc_atm_mng;
						atm_seq_mng;
						get_reply_data;
						hps_atm_status_mng;
						hps_atm_cutoff;
						ndc_update_cassettes;
						get_cassette_info;
						ndc_watch_config;
						get_fit;
						get_prints;
						get_denomination_info;
						get_profile;
						get_screen;
						get_state;
						get_msg_indiv_group;
						maj_watch_activity;
						update_log_arrette;
						pcrd_billing;
						pcrd_accounts;
						pcrd_host_accounts;
						pcrd_utility;
						pcrd_accounts_by_type;
						pcrd_creditPayment;
						pcrd_services;
						get_action_info;
						ndc_util;
						ndc_emv_data_wn;
						hps_atm_additional_periph_mng;
						pcrd_donations;
						pcrd_sub_donations;
						#pcrd_shadow_account_un_am
						)

if(	BO_ISS_ST_CORE	)
	 set(lib_proc_src_ndc 	${lib_proc_src_ndc};
							pcrd_shadow_account_un_am)
							
	 set(lib_proc_src_wnc 	${lib_proc_src_wnc};
							pcrd_shadow_account_un_am)
endif()

preproc_pc("${lib_proc_src_ndc}" lib_sources_ndc	proc_lis_files)
preproc_pc("${lib_proc_src_wnc}" lib_sources_wnc	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (ndcutil ${lib_sources_ndc})
add_library (ndcwncutil ${lib_sources_wnc})


set_target_properties(ndcwncutil PROPERTIES COMPILE_DEFINITIONS "WNC")




install (	TARGETS ndcutil
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
			
install (	TARGETS ndcwncutil
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
