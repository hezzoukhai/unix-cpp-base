
set(spec_libs netcom;pvisapuf;pcps;piso;ppuf;pvisa;pvisapuf;actions;saf;int;piso3nw;iso3)

set(app_sources	visa_adm.c 
				visa_bal.c 
				visa_ctl.c 
				visa_saf.c 
				visa_line.c 
				visa_sig.c 
				visa_timeout.c 
				visa_trait_adm.c 
				visa_trait_time.c 
				visa_utils.c 
				visa_services.c 
				visa_return.c 
				visa_ini.c 
				visa_main.c 
				visa_advice.c 
				visa_reversal.c 
				visa_reject.c 
				visa_aut_rep.c 
				visa_aut_req.c 
				visa_trait_bal.c 
				visa_trait_line.c 
				visa_net_mng.c 
				visa_file_upd.c 
				fct_shut_down.c 
				saf_processing.c 
				#cutover_processing.c 
				visa_0102_req.c
				visa_rs_params.c )



set(app_proc_sources 	visa_saf_mngt)


preproc_pc("${app_proc_sources}" app_sources	proc_lis_files)


set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_executable (BASE1 		${app_sources})
add_executable (BASE1_EU 	${app_sources})
set_target_properties(BASE1 	PROPERTIES COMPILE_DEFINITIONS "VISA_EU=0")
set_target_properties(BASE1_EU 	PROPERTIES COMPILE_DEFINITIONS "VISA_EU=1")

if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")
	target_link_libraries (BASE1 		${p7_dflt_libs};${spec_libs};${p7_ora_libs})
	target_link_libraries (BASE1_EU 	${p7_dflt_libs};${spec_libs};${p7_ora_libs})
else()
	target_link_libraries (BASE1 		${p7_dflt_libs};${spec_libs};${p7_ora_libs}	"-Wl,-brtl,-bnoipath,-bexpall")
	target_link_libraries (BASE1_EU 	${p7_dflt_libs};${spec_libs};${p7_ora_libs}	"-Wl,-brtl,-bnoipath,-bexpall")
endif()


install (	TARGETS 
						BASE1 
						BASE1_EU
			PERMISSIONS ${P7_DFLT_BIN_PERM}
			DESTINATION bin)
install (	FILES ${SHL_SOURCE_PATH}/start_base1 ${SHL_SOURCE_PATH}/stop_base1
			PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ
			DESTINATION ${SHL_INSTALL_PATH})

