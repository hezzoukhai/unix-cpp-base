# directory: libhost


set(lib_sources	 get_hsm_keys.c
            postilion_analyse.c
			postilion_dump.c
			postilion_emv_util.c
			postilion_f127_util.c
			postilion_ini.c
			postilion_util.c )
						

add_library (postilion ${lib_sources})
install (	TARGETS postilion 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
