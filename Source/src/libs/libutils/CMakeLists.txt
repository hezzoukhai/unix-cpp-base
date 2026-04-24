# directory: libutils


#libutils
set(lib_sources		bal_thr.c iso_to_tlv.c 
					tlv_to_iso.c 
					iso_util.c
					dump_iso.c 
					dump_piso.c 
					dump_visa.c 
					dump_sms.c 
					#dump_sid.c 
					dump_mds.c 
					dump_cup.c
					emv_visa_tools.c
					emv_sms_tools.c 
					emv_euro_tools.c 
					dump_euro.c 
				        dump_jcc.c
					dump_pulse.c
				        #emv_jcc_tools.c
					field_utils.c
					iso3_utils.c)

#ClearQeue
set(app_clearqueue_src	ClearQueue.c)

set(lib_proc_sources	adjust_tlv_amount;
						get_internal_stan;
						log_event;
						init_resource;
						translate_resp_code;
						check_resource_node;
						#ext_ndc_tools
						)

						
if(FO_ATM_ST_CORE OR FO_ATM_SA_CORE )
	set(lib_proc_sources ${lib_proc_sources};log_atm_event;ext_ndc_tools)
endif()


preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (utils 		STATIC	${lib_sources})
#add_library (utils_sh 	SHARED	${lib_sources})
add_executable (ClearQueue 			${app_clearqueue_src})
TARGET_LINK_LIBRARIES(ClearQueue                piso;pthread;privtlv;authotlv;iso3;monitoring;get;comora;utils;tools;queue;clntsh;dl;m)

install (	TARGETS utils 
					#utils_sh
					ClearQueue
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			RUNTIME
				PERMISSIONS ${P7_DFLT_BIN_PERM}
				DESTINATION bin
		)
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")

add_library (utils_pic 		STATIC	${lib_sources})


set_property(TARGET utils_pic 	PROPERTY POSITION_INDEPENDENT_CODE TRUE)


install (	TARGETS utils_pic
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
