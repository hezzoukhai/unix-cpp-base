
set(spec_libs psms;psmspuf;pcps;ppuf;saf;pvisa;pvisapuf;netcom;int)

set(app_sources		fct_shut_down.c
					sms_adm.c
					sms_dispute_advice.c
					sms_aut_rep.c
					sms_aut_req.c
					get_abrev_bank_name.c
					sms_bal.c
					sms_chargeback.c
					sms_reconciliation.c
					sms_ctl.c
					sms_ini.c
					sms_line.c
					sms_main.c
					sms_net_mng.c
					sms_reject.c
					sms_return.c
					sms_reversal.c
					sms_services.c
					sms_sig.c
					sms_advice.c
					sms_file_upd.c
					sms_timeout.c
					sms_trait_adm.c
					sms_trait_bal.c
					sms_trait_line.c
					sms_trait_time.c
					sms_utils.c
					#sms_params.c
					sms_fee_inc.c
					sms_incoming.c
					sms_to_tlv.c
					tlv_to_sms.c
					sms_text_inc.c
					sms_outg_manage.c
					sms_outg_processing.c
					sms_outgoing_thr.c
					sms_saf.c
					saf_processing.c
					sms_nns_mng.c
					sms_rs_params.c )

set(app_proc_sources 	get_abrev_bank_name;
						#saf_manage;
						sms_incoming;
						sms_nns_mng;
						sms_outg_manage
						#sms_params
						)


preproc_pc("${app_proc_sources}" app_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_executable (SMS ${app_sources})

if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")
target_link_libraries (SMS 	${p7_dflt_libs};${spec_libs};${p7_ora_libs})
else()
target_link_libraries (SMS 	${p7_dflt_libs};${spec_libs};${p7_ora_libs}			"-Wl,-brtl,-bnoipath,-bexpall")
endif()

install (	TARGETS SMS 
			PERMISSIONS ${P7_DFLT_BIN_PERM}
			DESTINATION bin)
install (	FILES ${SHL_SOURCE_PATH}/start_sms ${SHL_SOURCE_PATH}/stop_sms 
			PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ
			DESTINATION ${SHL_INSTALL_PATH})
