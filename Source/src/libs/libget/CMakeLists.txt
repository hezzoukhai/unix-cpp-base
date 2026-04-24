# directory: libget

set(lib_proc_sources	get_resources;
						get_pwc_globals;
						get_hsm_res_ids;
						get_res_params;
						get_pwc_globals_bank)
						

preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (get 		STATIC		${lib_sources})
#add_library (get_sh		SHARED		${lib_sources})

install (	TARGETS get 
			#get_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			)
if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


add_library (get_pic 		STATIC		${lib_sources})


set_property(TARGET get_pic 		PROPERTY POSITION_INDEPENDENT_CODE TRUE)


install (	TARGETS 
				get_pic 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
