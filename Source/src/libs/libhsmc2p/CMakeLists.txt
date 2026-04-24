# directory: libhsmc2p

set(lib_sources	c2p_cmd_layouts.c 
				c2p_fields.c 
				c2p_cmd_utils.c 
				c2p_verification.c
				c2p_pin_mng.c
				c2p_translation.c
				c2p_mac_mng.c
				c2p_key_mng.c
				c2p_rsa_mng.c
				c2p_ersb_mng.c
				)
				
set(lib_proc_sources	#get_emv_script; #not used?
						get_c2p_keys;
						
						)
preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (hsmc2p 			STATIC	${lib_sources})
#add_library (hsmc2p_sh 			SHARED	${lib_sources})

install (	TARGETS hsmc2p 
			#hsmc2p_sh
			
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)

if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


add_library (hsmc2p_pic 		STATIC	${lib_sources})


set_property(TARGET hsmc2p_pic 		PROPERTY POSITION_INDEPENDENT_CODE
TRUE)

set_target_properties(hsmc2p_pic 	PROPERTIES COMPILE_DEFINITIONS
"P7_IMPORT_SYM")



install (	TARGETS 
				hsmc2p_pic 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()



