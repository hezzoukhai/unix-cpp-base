# directory: libhsm

set(lib_sources	hsm_commands.c 
				hsm_utils.c 
				hsm_pin_chg.c 
				hsm_thr.c 
				translation_lib.c 
				verification_lib.c 
				Production.c
				trans_key.c
				hps_gen_key.c
				mac_mng.c)

set(lib_proc_sources	get_hsm_keys)

if(	FO_POS_ST_CORE 	OR
	FO_POS_SA_CORE	OR
	FO_ATM_ST_CORE	OR
	FO_ATM_SA_CORE )
	 set(lib_proc_sources ${lib_proc_sources};term_key_mng;hps_key_mng)
endif()


preproc_pc("${lib_proc_sources}" lib_sources	proc_lis_files)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${proc_lis_files}" )

add_library (hsm ${lib_sources})
install (	TARGETS hsm 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
