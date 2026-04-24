# directory: libiccprod


set(lib_sources	iccbuild_buf_header.c 
				iccprod_analyse.c 
				iccprod_dump.c 
				iccprod_ini.c
				iccprod_util.c)
						

add_library (iccprod 		STATIC	${lib_sources})
add_library (iccprod_sh 	SHARED	${lib_sources})
install (	TARGETS iccprod iccprod_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
