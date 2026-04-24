# directory: libinterface


set(lib_sources	int_ini_config.c 
				)
						

add_library (int 		${lib_sources})
install (	TARGETS int 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
