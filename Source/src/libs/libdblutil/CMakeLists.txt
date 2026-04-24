# directory: libdblutil


set(lib_sources	dbl_down_load_field.c
					dbl_format_field.c
					dbl_insert_text.c
					dbl_tools_advice.c
					dbl_tools_convert.c
					hps_lib.c
					util_varchar.c)

set(lib_proc_sources	
						dbl_watch_config;
						get_fit;
						dbl_get_profile;
						dbl_get_screen; 
						dbl_get_state;
						dbl_util;
						dbl_get_cassette_info;
						dbl_atm_mng; 
						hps_atm_cutoff;
						dbl_get_reply_data;
						maj_watch_activity;
						get_msg_indiv_group;
						dbl_get_prints;
						dbl_atm_mng;
						dbl_update_cassettes;
						dbl_hps_atm_status_mng;
						dbl_atm_seq_mng;
						get_action_info;
						update_log_arrette;
						get_statement_label;
						dbl_emv_data;
						get_last_transaction;
						dbl_emv_init_data)
						
if(	FO_SWI_ST_CORE	OR
	FO_SWI_SA_CORE	)
	 set(lib_proc_sources 	${lib_proc_sources};
							dbl_pcrd_accounts;
							pcrd_remittance_accounts)
endif()

if(	FO_ISS_ST_CORE	OR
	FO_ISS_SA_CORE	)
	set(lib_proc_sources 	${lib_proc_sources};
							dbl_pcrd_accounts;
							pcrd_remittance_accounts)
endif()
preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (dblutil ${lib_sources})
install (	TARGETS dblutil 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
