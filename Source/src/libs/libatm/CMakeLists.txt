# directory: libatm

set(lib_proc_sources			
								get_ptserv_name;
								get_terminal_profile;
								log_journal;
								resource;
								terminal;
								get_account_number;
								get_curr_account;
								get_branch_name)



preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (atm 		STATIC	${lib_sources})
#add_library (atm_sh 	SHARED	${lib_sources})

install (	TARGETS atm
					#atm_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")

add_library (atm_pic 	STATIC ${lib_sources})
set_property(TARGET atm_pic PROPERTY POSITION_INDEPENDENT_CODE TRUE)
install (	TARGETS atm_pic 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
