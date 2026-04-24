# directory: libasync

#libasync
set(lib_sources		async_com.c 
					com.c)

add_library (asyn 		STATIC	${lib_sources})
add_library (asyn_sh 	SHARED	${lib_sources})

install (	TARGETS asyn
					asyn_sh
			ARCHIVE 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
			LIBRARY 
				PERMISSIONS ${P7_DFLT_LIB_PERM}
				DESTINATION lib
		)
