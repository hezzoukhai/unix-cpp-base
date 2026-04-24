# directory: liblink


set(lib_link_src	link_ini.c 
						)
				

add_library (pisolink 					${lib_link_src})


install (	TARGETS pisolink 
			ARCHIVE 
			PERMISSIONS ${P7_DFLT_LIB_PERM}
			DESTINATION lib)


