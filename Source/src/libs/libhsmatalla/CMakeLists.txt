# directory: libhsmatalla

set(lib_sources	atalla_cmd_layouts.c 
				atalla_fields.c 
				atalla_cmd_utils.c
				atalla_cmd.c
				atalla_verification.c
				atalla_pin_mng.c
				atalla_translation.c
				atalla_key_mng.c
				atalla_rsa_mng.c
				atalla_ersb_mng.c
				)
				

#preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
#set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (hsmatalla 			STATIC	${lib_sources})
#add_library (hsmatalla_sh 			SHARED	${lib_sources})

install (	TARGETS hsmatalla 
			#hsmatalla_sh
			
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)

if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


add_library (hsmatalla_pic 		STATIC	${lib_sources})


set_property(TARGET hsmatalla_pic 		PROPERTY POSITION_INDEPENDENT_CODE
TRUE)

set_target_properties(hsmatalla_pic 	PROPERTIES COMPILE_DEFINITIONS
"P7_IMPORT_SYM")



install (	TARGETS 
				hsmatalla_pic 
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()



