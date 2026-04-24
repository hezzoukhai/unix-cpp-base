# directory: libactions



set(lib_sources			auth_utils.c 
						p7_ctrl_utils.c 
						p7_service_processing.c 
						pin_change_inquiry.c
						#check_security.c
						check_auth_security.c
						pin_chg_key_mng.c)

set(lib_proc_sources	amount_mng;
						check_resource;
						check_ip_address;
						check_resource_services;
						get_action_code;
						get_auth_number;
						get_original_data;
						load_issuer_data;
						maj_autho;
						proc_preauth;
						stop_list_reply_processing;
						get_service;
						get_res_auth_proc_mode;
						exponent;
						currency;
						ctrl_events;
						get_acquirer_bin;
						get_country_code;
						auth_broadcast;
						get_node_bridge;
						#load_security_keys;
						auth_amount_mng;
						reversal_procedure;
						bank_update_utils;
						get_mer_acq_bank;
						balance_inquiry;
						control_processing;
						get_pin_info;
						get_security_flags;
						maj_balance;
						pin_chg_mng;
						stand_in;
						check_loan_account;
						oct_auth_processing;
						settlement_mng;
						maj_card_activity;
						fraud_update;
						preControl_acq;
						autho_bridge_proc;
						eval_decision_matrix;
						check_prdct_digit_eligibility;
						complete_tokenization;
						evaluate_digitization_matrix;
						process_tokenization_event_notif;
						send_activation_code;
						)
						
if(	FO_ATM_ST_CORE	OR
	FO_ATM_SA_CORE )
	set(lib_proc_sources 	${lib_proc_sources};
							maj_autho_atm;
							statement_request;
							check_card_inquiry;
							cheque_control;
							cheque_inquiry;
							deposit_inquiry;
							bill_payment_inquiry;
							rib_inquiry)

	set(lib_sources 		${lib_sources};
							autho_conf_proc.c)
endif()

if(	FO_POS_ST_CORE	OR
	FO_POS_SA_CORE	)
	set(lib_proc_sources 	${lib_proc_sources};
							update_terminal_pos_watch_act)
endif()

						
if(	FO_SWI_ST_CORE	OR
	FO_SWI_SA_CORE	)
	set(lib_proc_sources 	${lib_proc_sources};
							statement_request;
							check_fallback;
							control_limit;
							proc_preauth;
							get_routing;
							activate_card;
							get_sec_checks_params;
							autho_saf_processing;
							check_atc;
							get_timers)
endif()

if(	BO_ISS_ST_CORE )
	set(lib_proc_sources 	${lib_proc_sources};
							#statement_inquiry;
							#rib_inquiry;
							credit_card_payment_inquiry)
endif()



preproc_pc("${lib_proc_sources}" lib_sources proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (actions 		STATIC ${lib_sources})
#add_library (actions_sh 	SHARED ${lib_sources})


install (	TARGETS actions 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")

add_library (actions_pic 	STATIC ${lib_sources})
set_property(TARGET actions_pic PROPERTY POSITION_INDEPENDENT_CODE TRUE)
install (	TARGETS actions_pic 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
