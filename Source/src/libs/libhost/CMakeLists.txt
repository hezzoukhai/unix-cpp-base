# directory: libhost


set(lib_sources	host_analyse.c 
				host_dump.c 
				host_ini.c 
				host_util.c)
						

add_library (host ${lib_sources})
install (	TARGETS host 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)
