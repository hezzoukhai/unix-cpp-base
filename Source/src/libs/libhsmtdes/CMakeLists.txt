# directory: libhsmtdes

set(lib_sources	
				hsm_utils.c 
				Production.c
				hsm_thr.c 
				)
				

set(lib_hsmthales_src 		hsm_commands.c 
							hps_gen_key.c
							mac_mng.c)


set(lib_proc_sources	
						get_hsm_keys;
						)

if(	FO_POS_ST_CORE 	OR
	FO_POS_SA_CORE	)
	 set(lib_proc_sources 	${lib_proc_sources};
							pos_term_key_mng)

set(lib_hsmthales_src 		${lib_hsmthales_src}
							gen_pos_keys)
endif()

if( FO_ATM_ST_CORE	OR
	FO_ATM_SA_CORE )
	 set(lib_proc_sources 	${lib_proc_sources};
							term_key_mng;
							hps_key_mng)
	 set(lib_hsmthales_src 		${lib_hsmthales_src}
								gen_atm_keys)
endif()


if(	FO_SWI_ST_CORE	OR
	FO_SWI_SA_CORE	)
	 set(lib_hsmthales_src 		${lib_hsmthales_src}
								translation_lib
								verification_lib
								)
							
	set(lib_proc_sources 	${lib_proc_sources};
							#UpdateScriptStatus;
							get_hsm_emv_keys;
							get_3ds_keys)
endif()

if(	FO_ISS_ST_CORE	OR
	FO_ISS_SA_CORE	)
	set(lib_proc_sources 	${lib_proc_sources};
							#UpdateScriptStatus;
							get_hsm_emv_keys;
							get_3ds_keys)
endif()

preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (hsmtdes 			STATIC	${lib_sources})
add_library (hsmtdes_ext 		STATIC	${lib_sources})

add_library (hsmthales 			STATIC	${lib_hsmthales_src})

set_target_properties(hsmtdes_ext PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")

install (	TARGETS hsmtdes 
					hsmtdes_ext hsmthales
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)

if(PLATFORM MATCHES "LINUX" OR PLATFORM MATCHES "SOLARIS")


add_library (hsmtdes_ext_pic 		STATIC	${lib_sources})
add_library (hsmthales_pic                  STATIC  ${lib_hsmthales_src})

set_property(TARGET hsmtdes_ext_pic 		PROPERTY POSITION_INDEPENDENT_CODE TRUE)
set_property(TARGET hsmthales_pic             PROPERTY POSITION_INDEPENDENT_CODE TRUE)

set_target_properties(hsmtdes_ext_pic 	PROPERTIES COMPILE_DEFINITIONS "P7_IMPORT_SYM")



install (	TARGETS 
				hsmtdes_ext_pic hsmthales_pic
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
endif()
		
